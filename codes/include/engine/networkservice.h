#ifndef _TNG_NETWORKSERVICE_H_
#define _TNG_NETWORKSERVICE_H_

#include "service.h"
#include <vector>
#include <string>

#ifdef TNG_OS_FAMILY_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

namespace tng
{
	class ENGINE_API TcpClient : public Service
	{
	public:
		ENGINE_CLASS_DEF
		typedef Delegate2<char*, u32> ReadCallBack;
		typedef Delegate0<> OnLostConnection;
		typedef Delegate0<> OnConnection;

		TcpClient();
		~TcpClient();

		virtual bool Initialize(Loop* loop);
		virtual void LoadFromConfig(JsonObject& doc) {}
		virtual void Destroy();

		bool Connect(const char* ip, const u16 port);
		bool Connect6(const char* ip, const u16 port);
		void Disconnect();

		void SetConnectedCallback(const OnConnection& callbacks) { on_connected_ = callbacks; }
		void SetMessageCallBack(const ReadCallBack& msgcallback) { on_message_ = msgcallback; }
		void SetOnLostConnectionCallBack(const OnLostConnection& lostcallback) { on_lost_ = lostcallback; }
		
		bool SendMessage(char* buf, u32 len);
		void _onMessage(char* buf, u32 len);
		void _onLost();
		void _onConnected() {
			if (on_connected_) {
				on_connected_();
			}
		}

	protected:
		ReadCallBack on_message_;
		OnLostConnection on_lost_;
		OnConnection on_connected_;
		
	private:
#ifdef TNG_OS_FAMILY_WINDOWS
		SOCKET socket_;
#else
		int socket_;
#endif
		bool is_connected_;
		std::vector<char> read_buffer_;
		static const size_t BUFFER_SIZE = 8192;
	};

	class ENGINE_API TcpPeer
	{
	public:
		TcpPeer();
		~TcpPeer();

		void OnConnected();
		void OnShutdown();
		void SendMessage(char* data, int len);
		std::string GetPeerKey();

#ifdef TNG_OS_FAMILY_WINDOWS
		SOCKET socket_;
#else
		int socket_;
#endif
		void* user_data_;
		class TcpServer* server_;
	};

	class ENGINE_API TcpServer : public Service
	{
	public:
		ENGINE_CLASS_DEF
		typedef Delegate3<TcpPeer*, char*, u32> PeerMsgCallback;
		typedef Delegate1<TcpPeer*> OnPeerCallback;

		TcpServer();
		virtual void LoadFromConfig(JsonObject& doc) {}
		virtual void Destroy();
		virtual bool Initialize(Loop* loop);

		bool Listen(const char* ip, const u16 port);
		bool Listen6(const char* ip, const u16 port);

		void SetMsgCallback(const PeerMsgCallback& cb) { on_peer_msg_ = cb; }
		void SetShutdownCallbcak(const OnPeerCallback& cb) { on_shut_down_ = cb; }
		void SetConnectCallback(const OnPeerCallback& cb) { on_connect_ = cb; }

		void _onMessage(TcpPeer* peer, char* buf, u32 len);
		void _onShutDown(TcpPeer* peer);
		void _onPeerConnect(TcpPeer* peer);

	protected:
		PeerMsgCallback on_peer_msg_;
		OnPeerCallback on_connect_;
		OnPeerCallback on_shut_down_;

		bool _listen();

	private:
#ifdef TNG_OS_FAMILY_WINDOWS
		SOCKET listen_socket_;
#else
		int listen_socket_;
#endif
		std::vector<TcpPeer*> peers_;
		static const int MAX_PEERS = 1024;
		static const size_t BUFFER_SIZE = 8192;
	};
}

#endif