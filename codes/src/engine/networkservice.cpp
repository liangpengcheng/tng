#include "engine/networkservice.h"
#include "engine/engine.h"
#include "core/log.h"

namespace tng
{
	TcpClient::TcpClient() : socket_(INVALID_SOCKET), is_connected_(false) {
		read_buffer_.resize(BUFFER_SIZE);
	}

	TcpClient::~TcpClient() {
		Destroy();
	}

	void TcpClient::Destroy() {
		if (socket_ != INVALID_SOCKET) {
#ifdef TNG_OS_FAMILY_WINDOWS
			closesocket(socket_);
#else
			close(socket_);
#endif
			socket_ = INVALID_SOCKET;
		}
		is_connected_ = false;
	}

	bool TcpClient::Initialize(Loop* loop) {
#ifdef TNG_OS_FAMILY_WINDOWS
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return false;
		}
#endif
		return true;
	}

	bool TcpClient::Connect(const char* ip, const u16 port) {
		socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socket_ == INVALID_SOCKET) {
			return false;
		}

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);

		if (connect(socket_, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
			Destroy();
			return false;
		}

		// Set non-blocking mode
#ifdef TNG_OS_FAMILY_WINDOWS
		u_long mode = 1;
		ioctlsocket(socket_, FIONBIO, &mode);
#else
		int flags = fcntl(socket_, F_GETFL, 0);
		fcntl(socket_, F_SETFL, flags | O_NONBLOCK);
#endif

		is_connected_ = true;
		_onConnected();
		return true;
	}

	bool TcpClient::Connect6(const char* ip, const u16 port) {
		socket_ = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
		if (socket_ == INVALID_SOCKET) {
			return false;
		}

		struct sockaddr_in6 addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(port);
		inet_pton(AF_INET6, ip, &addr.sin6_addr);

		if (connect(socket_, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
			Destroy();
			return false;
		}

		// Set non-blocking mode
#ifdef TNG_OS_FAMILY_WINDOWS
		u_long mode = 1;
		ioctlsocket(socket_, FIONBIO, &mode);
#else
		int flags = fcntl(socket_, F_GETFL, 0);
		fcntl(socket_, F_SETFL, flags | O_NONBLOCK);
#endif

		is_connected_ = true;
		_onConnected();
		return true;
	}

	void TcpClient::Disconnect() {
		Destroy();
	}

	bool TcpClient::SendMessage(char* buf, u32 len) {
		if (!is_connected_) return false;

		int sent = send(socket_, buf, len, 0);
		if (sent < 0) {
#ifdef TNG_OS_FAMILY_WINDOWS
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK) {
#else
			if (errno != EWOULDBLOCK && errno != EAGAIN) {
#endif
				_onLost();
				return false;
			}
		}
		return true;
	}

	void TcpClient::_onMessage(char* buf, u32 len) {
		if (on_message_) {
			on_message_(buf, len);
		}
	}

	void TcpClient::_onLost() {
		if (on_lost_) {
			on_lost_();
		}
	}

	TcpPeer::TcpPeer() : socket_(INVALID_SOCKET), server_(NULL), user_data_(NULL) {
	}

	TcpPeer::~TcpPeer() {
		if (socket_ != INVALID_SOCKET) {
#ifdef TNG_OS_FAMILY_WINDOWS
			closesocket(socket_);
#else
			close(socket_);
#endif
		}
	}

	void TcpPeer::OnConnected() {
		if (server_) {
			server_->_onPeerConnect(this);
		}
	}

	void TcpPeer::OnShutdown() {
		if (server_) {
			server_->_onShutDown(this);
		}
	}

	std::string TcpPeer::GetPeerKey() {
		char ip[INET6_ADDRSTRLEN];
		struct sockaddr_storage addr;
		socklen_t len = sizeof(addr);

		if (getpeername(socket_, (struct sockaddr*)&addr, &len) == 0) {
			if (addr.ss_family == AF_INET) {
				struct sockaddr_in* s = (struct sockaddr_in*)&addr;
				inet_ntop(AF_INET, &s->sin_addr, ip, sizeof(ip));
				return str(format("%s:%d") % ip % ntohs(s->sin_port));
			} else if (addr.ss_family == AF_INET6) {
				struct sockaddr_in6* s = (struct sockaddr_in6*)&addr;
				inet_ntop(AF_INET6, &s->sin6_addr, ip, sizeof(ip));
				return str(format("%s:%d") % ip % ntohs(s->sin6_port));
			}
		}
		return "";
	}

	void TcpPeer::SendMessage(char* data, int len) {
		send(socket_, data, len, 0);
	}

	TcpServer::TcpServer() : listen_socket_(INVALID_SOCKET) {
	}

	void TcpServer::Destroy() {
		if (listen_socket_ != INVALID_SOCKET) {
#ifdef TNG_OS_FAMILY_WINDOWS
			closesocket(listen_socket_);
#else
			close(listen_socket_);
#endif
			listen_socket_ = INVALID_SOCKET;
		}

		for (TcpPeer* peer : peers_) {
			delete peer;
		}
		peers_.clear();
	}

	bool TcpServer::Initialize(Loop* loop) {
#ifdef TNG_OS_FAMILY_WINDOWS
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return false;
		}
#endif
		return true;
	}

	bool TcpServer::Listen(const char* ip, const u16 port) {
		listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listen_socket_ == INVALID_SOCKET) {
			return false;
		}

		// Enable address reuse
		int opt = 1;
		setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);

		if (bind(listen_socket_, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
			Destroy();
			return false;
		}

		if (listen(listen_socket_, SOMAXCONN) != 0) {
			Destroy();
			return false;
		}

		// Set non-blocking mode
#ifdef TNG_OS_FAMILY_WINDOWS
		u_long mode = 1;
		ioctlsocket(listen_socket_, FIONBIO, &mode);
#else
		int flags = fcntl(listen_socket_, F_GETFL, 0);
		fcntl(listen_socket_, F_SETFL, flags | O_NONBLOCK);
#endif

		Log::GetLog()->Printf(Log::SYS_CHN, "listening at %s:%d", ip, port);
		return true;
	}

	bool TcpServer::Listen6(const char* ip, const u16 port) {
		listen_socket_ = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
		if (listen_socket_ == INVALID_SOCKET) {
			return false;
		}

		// Enable address reuse
		int opt = 1;
		setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

		struct sockaddr_in6 addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(port);
		inet_pton(AF_INET6, ip, &addr.sin6_addr);

		if (bind(listen_socket_, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
			Destroy();
			return false;
		}

		if (listen(listen_socket_, SOMAXCONN) != 0) {
			Destroy();
			return false;
		}

		// Set non-blocking mode
#ifdef TNG_OS_FAMILY_WINDOWS
		u_long mode = 1;
		ioctlsocket(listen_socket_, FIONBIO, &mode);
#else
		int flags = fcntl(listen_socket_, F_GETFL, 0);
		fcntl(listen_socket_, F_SETFL, flags | O_NONBLOCK);
#endif

		Log::GetLog()->Printf(Log::SYS_CHN, "listening at [%s]:%d", ip, port);
		return true;
	}

	void TcpServer::_onMessage(TcpPeer* peer, char* buf, u32 len) {
		if (on_peer_msg_) {
			on_peer_msg_(peer, buf, len);
		}
	}

	void TcpServer::_onShutDown(TcpPeer* peer) {
		if (on_shut_down_) {
			on_shut_down_(peer);
		}
		auto it = std::find(peers_.begin(), peers_.end(), peer);
		if (it != peers_.end()) {
			peers_.erase(it);
			delete peer;
		}
	}

	void TcpServer::_onPeerConnect(TcpPeer* peer) {
		if (on_connect_) {
			on_connect_(peer);
		}
	}

	bool TcpServer::_listen() {
		if (listen_socket_ == INVALID_SOCKET) {
			return false;
		}

		struct sockaddr_storage client_addr;
		socklen_t client_len = sizeof(client_addr);
		
#ifdef TNG_OS_FAMILY_WINDOWS
		SOCKET client_socket = accept(listen_socket_, (struct sockaddr*)&client_addr, &client_len);
#else
		int client_socket = accept(listen_socket_, (struct sockaddr*)&client_addr, &client_len);
#endif

		if (client_socket == INVALID_SOCKET) {
#ifdef TNG_OS_FAMILY_WINDOWS
			if (WSAGetLastError() != WSAEWOULDBLOCK) {
#else
			if (errno != EWOULDBLOCK && errno != EAGAIN) {
#endif
				return false;
			}
			return true;
		}

		// Set non-blocking mode for client socket
#ifdef TNG_OS_FAMILY_WINDOWS
		u_long mode = 1;
		ioctlsocket(client_socket, FIONBIO, &mode);
#else
		int flags = fcntl(client_socket, F_GETFL, 0);
		fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
#endif

		TcpPeer* peer = new TcpPeer();
		peer->socket_ = client_socket;
		peer->server_ = this;
		peers_.push_back(peer);
		peer->OnConnected();

		return true;
	}
}