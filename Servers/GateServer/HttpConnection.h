#pragma once
#include "const.h"

class HttpConnection: public std::enable_shared_from_this<HttpConnection> {
public:
	friend class LogicSystem;
	HttpConnection(tcp::socket socket);
	void Start();
private:
	void CheckDeadline(); // ��ʱ��
	void PreParseGetParam(); // ��������
	void WriteResponse(); // Ӧ��
	void HandleReq(); // �������� ��������ͷ ������������
	tcp::socket _socket;
	beast::flat_buffer _buffer{8192}; // 8K������ ��������
	http::request<http::dynamic_body> _request; // ��̬body����
	http::response<http::dynamic_body> _response;
	// The timer for putting a deadline on connection processing.
	net::steady_timer _deadline{_socket.get_executor(), std::chrono::seconds(60)};

	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

