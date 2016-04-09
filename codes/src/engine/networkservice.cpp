/*
#include "MathGeoLib/Algorithm/Random/LCG.h"
#include "engine/networkservice.h"
#include "engine/engine.h"
#include "core/log.h"
namespace tng
{
	static uv_buf_t alloc_cb(uv_handle_t* handle, size_t suggested_size) {
		char* buf = new char[suggested_size];
		return uv_buf_init(buf, sizeof suggested_size);
	}

	static void ClientReadCB(uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) {
		Assert(tcp != NULL);
		TcpClient* client =(TcpClient*)tcp->data;
		if (nread >= 0) {
			
			client->_onMessage(buf.base,u32(nread));
			delete []buf.base;
		}
		else {
			Log::GetLog()->Printf(Log::WARNING_CHN,"server error:%s",uv_err_name(uv_last_error(tcp->loop)));
			client->_onLost();
		}
	}

	TcpClient::TcpClient()
	{

	}
	TcpClient::~TcpClient(){

	}

	void OnCloseTcp(uv_handle_t* handle)
	{
		uv_tcp_t* timer = (uv_tcp_t*)handle;
		delete timer;
	}

	void TcpClient::Destroy()
	{
#ifndef TNG_NO_UVLOOP
		if (internal_handle_)
			uv_close(internal_handle_, OnCloseTcp);
		internal_handle_ = NULL;
#endif
	}
	bool TcpClient::Initialize(Loop* loop)
	{
		uv_tcp_t* tcp = new uv_tcp_t;
		internal_handle_ = reinterpret_cast<uv_handle_t*>(tcp);

		int r = uv_tcp_init(loop->_getInternalLoop(), tcp);

		internal_handle_->data = this;
		return r == 0;
	}
	void TcpClient::_onMessage(char* buf,u32 len)
	{
		if (on_message_)
		{
			on_message_(buf,len);
		}
	}
	void TcpClient::_onLost()
	{
		if (on_lost_)
		{
			on_lost_();
		}
	}
	void afterConnect(uv_connect_t* req, int status)
	{
		TcpClient* client = (TcpClient*)req->data;
		if (status == 0)
		{
			int r = uv_read_start(req->handle, alloc_cb, ClientReadCB);
			client->_onConnected();
		}
		else
		{
			Log::GetLog()->Printf(Log::ERROR_CHN, "connect failed:%s",uv_strerror(uv_last_error(req->handle->loop)));
		}
		//r = uv_shutdown(&client->shutdown_req_, req->handle, ShutDownCB);
	}
	void afterWrite(uv_write_t* req, int status)
	{
		//delete []req->write_buffer.base;
		delete req;
	}
	bool TcpClient::Connect(const char* ip,const u16 port)
	{
		sockaddr_in address = uv_ip4_addr(ip, port);
		uv_tcp_t* tcp = (uv_tcp_t*)internal_handle_;
		int r = uv_tcp_connect(&req_, tcp, address,
			afterConnect);
		req_.data = this;
		return r==0;
	}
	void TcpClient::Disconnect()
	{
		Destroy();
	}
	bool TcpClient::Connect6(const char* ip,const u16 port)
	{
		sockaddr_in6  address = uv_ip6_addr(ip, port);
		uv_tcp_t* tcp = (uv_tcp_t*)internal_handle_;
		int r = uv_tcp_connect6(&req_, tcp, address,
			afterConnect);
		req_.data = this;
		return r==0;
	}
	bool TcpClient::SendMessage(char* buf,u32 len)
	{
		uv_write_t* req = new uv_write_t;
		uv_buf_t buffer = uv_buf_init(buf, len);
		int r = uv_write(req,req_.handle,&buffer,1,afterWrite);
		return r==0;
	}
	static void close_cb(uv_handle_t* handle) {
		delete handle;
	}
	TcpPeer::TcpPeer() :server_(NULL), user_data_(NULL){}
	TcpPeer::~TcpPeer()
	{
		uv_close((uv_handle_t*)(tcp_), close_cb);
	}
	void TcpPeer::OnShutdown()
	{
		if (server_)
		{
			server_->_onShutDown(this);
		}
	}
	void TcpPeer::OnConnected()
	{
		if (server_)
		{
			server_->_onPeerConnect(this);
		}
	}

	std::string TcpPeer::GetPeerKey()
	{
		struct sockaddr_storage  address;
		int namelen = sizeof(address);
		int family;
		int port;
		char ip[INET6_ADDRSTRLEN];
		if (uv_tcp_getpeername(tcp_, (sockaddr*)(&address), &namelen) != -1)
		{
			family = address.ss_family;
			if (family == AF_INET) {
				struct sockaddr_in* addrin = (struct sockaddr_in*)&address;
				uv_inet_ntop(AF_INET, &(addrin->sin_addr), ip, INET6_ADDRSTRLEN);
				port = ntohs(addrin->sin_port);
			}
			else if (family == AF_INET6) {
				struct sockaddr_in6* addrin6 = (struct sockaddr_in6*)&address;
				uv_inet_ntop(AF_INET6, &(addrin6->sin6_addr), ip, INET6_ADDRSTRLEN);
				port = ntohs(addrin6->sin6_port);
			}

			return str(format("%s:%d") % ip%port);
		}
		return "";
	}

	void TcpPeer::SendMessage(char* data, int len)
	{
		uv_write_t* req = new uv_write_t;
		uv_buf_t buffer = uv_buf_init(data,len);
		int r = uv_write(req, (uv_stream_t*)tcp_, &buffer, 1, afterWrite);
	}

	TcpServer::TcpServer()
	{

	}
	void TcpServer::Destroy()
	{
#ifndef TNG_NO_UVLOOP

		if (internal_handle_)
			uv_close(internal_handle_, OnCloseTcp);
		internal_handle_ = NULL;
#endif
	}
	bool TcpServer::Initialize(Loop* loop)
	{
		uv_tcp_t* tcp = new uv_tcp_t;
		internal_handle_ = reinterpret_cast<uv_handle_t*>(tcp);

		int r = uv_tcp_init(loop->_getInternalLoop(), tcp);
		Assert(r == 0);
		r = uv_tcp_keepalive(tcp, 1, 60);
		Assert(r == 0);
		internal_handle_->data= this;
		return r == 0;
	}


	bool TcpServer::Listen(const char* ip,const u16 port)
	{
		struct sockaddr_in address = uv_ip4_addr(ip, port);

		uv_tcp_t* tcp = (uv_tcp_t*)internal_handle_;

		int r = uv_tcp_bind(tcp, address);
	
		if (r!=0)
		{
			return false;
		}
		if (_listen())
		{
			Log::GetLog()->Printf(Log::SYS_CHN,"listen at %s:%d",ip,port);
			return true;
		}
		else
		{
			Log::GetLog()->Printf(Log::ERROR_CHN,"listen failed at %s:%d:%s",ip,port,uv_strerror(uv_last_error(tcp->loop)));
			return false;
		}
		
	}
	bool TcpServer::Listen6(const char* ip,const u16 port)
	{
		struct sockaddr_in6 address = uv_ip6_addr(ip, port);

		uv_tcp_t* tcp = (uv_tcp_t*)internal_handle_;

		int r = uv_tcp_bind6(tcp, address);
	
		if (r!=0)
		{
			return false;
		}
		
		return _listen();
	}
	static void OnTcpPeerRead(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
	{
		TcpPeer* client= (TcpPeer*)stream->data;
		if (nread>0)
		{
			Assert(client);
			Assert(client->server_);
			client->server_->_onMessage(client,buf.base,u32(nread));
		}
		else
		{
			///fix it
			if (uv_last_error(stream->loop).code != UV_EAGAIN)
			{
				if(uv_last_error(stream->loop).code != UV_ECONNRESET &&
					uv_last_error(stream->loop).code != UV_EOF)
				{
					Log::GetLog()->Printf(Log::ERROR_CHN,"%s",uv_strerror(uv_last_error(stream->loop)));
				}
				client->OnShutdown();
			}
			
			//uv_close((uv_handle_t*)stream,NULL);
		}
		if (buf.len&&buf.base)
			delete []buf.base;
	}
	static void OnConnection(uv_stream_t* handle, int status)
	{
		TcpPeer* client= new TcpPeer;
		TcpServer* server = (TcpServer*)handle->data;
		client->tcp_ = new uv_tcp_t;
		//LCG lcg;
		//int i = lcg.Int(0,server->ThreadNumber()-1);
		//std::string name = str(format("TcpServerThread%d")%i);
		//Loop* loop = Engine::GetInstance()->GetLoop(name);
		Assert(uv_tcp_init(handle->loop, client->tcp_)==0);
		client->tcp_->data = client;
		client->server_ = server;
		int r = uv_accept(handle, (uv_stream_t*)client->tcp_);
		r = uv_read_start((uv_stream_t*)(client->tcp_), alloc_cb, OnTcpPeerRead);
		client->OnConnected();
		Assert(r==0);
	}
	void TcpServer::_onPeerConnect(TcpPeer* peer)
	{
		if (on_connect_)
		{
			on_connect_(peer);
		}
	}
	void TcpServer::_onMessage(TcpPeer* peer,char* buf,u32 len)
	{
		if (on_peer_msg_)
		{
			on_peer_msg_(peer,buf,len);
		}
	}
	void TcpServer::_onShutDown(TcpPeer* peer)
	{
		if (on_shut_down_)
		{
			on_shut_down_(peer);
		}
		delete peer;
	}
	bool TcpServer::_listen()
	{
		int r = uv_listen((uv_stream_t*)internal_handle_, 1, OnConnection);
		internal_handle_->data = this;
		return r==0;
	}


	START_SUB_CLASS(TcpClient,UVService)
	END_CLASS

	START_SUB_CLASS(TcpServer,UVService)
	END_CLASS
}
*/