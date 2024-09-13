#include "CServer.h"
#include "AsioIOContextPool.h"
#include <iostream>
#include "UserMgr.h"

CServer::CServer(boost::asio::io_context& io_context, short port):_io_context(io_context), _port(port),
_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
	std::cout << "Server start success, listen on port : " << _port << std::endl;
	StartAccept();
}

CServer::~CServer() {
	std::cout << "Server destruct listen on port : " << _port << std::endl;
}

void CServer::ClearSession(std::string session_id) {
	if (_sessions.find(session_id) != _sessions.end()) {
		// 清理本内存用户和Session的连接信息
		UserMgr::GetInstance()->RemoveUserSession(_sessions[session_id]->GetUserId());
	}

	// 加局部作用域
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions.erase(session_id);
	}
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code& error) {
	if (!error) {
		new_session->Start();
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions.insert(make_pair(new_session->GetSessionId(), new_session));
	} else {
		std::cout << "session accept failed, error is " << error.what() << std::endl;
	}

	StartAccept();
}

void CServer::StartAccept() {
	auto& io_context = AsioIOContextPool::GetInstance()->GetIOContext();
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
	//_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, boost::asio::placeholders::error));
}
