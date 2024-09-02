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

void CServer::ClearSession(std::string uuid) {
	if (_sessions.find(uuid) != _sessions.end()) {
		UserMgr::GetInstance()->RemoveUserSession(_sessions[uuid]->GetUserId());
	}

	{
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions.erase(uuid);
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
