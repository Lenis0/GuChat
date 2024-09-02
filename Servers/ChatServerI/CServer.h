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
	// �������Ӻ�Ĵ���ص�
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& error);
	// ��ʼ��������
	void StartAccept();
	boost::asio::io_context& _io_context;
	short _port;
	tcp::acceptor _acceptor;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;
	std::mutex _mutex; // ��֤session map���̰߳�ȫ
};
