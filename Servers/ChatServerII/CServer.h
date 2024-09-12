#pragma once
#include <boost/asio.hpp>
#include "CSession.h"
#include <map>

using tcp = boost::asio::ip::tcp;

class CServer {
public:
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	void ClearSession(std::string);
private:
	// 接受连接后的处理回调
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& error);
	// 开始接受连接
	void StartAccept();
	boost::asio::io_context& _io_context;
	short _port;
	tcp::acceptor _acceptor;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;
	std::mutex _mutex; // 保证session map的线程安全
};
