#pragma once
#include "const.h"

class HttpConnection: public std::enable_shared_from_this<HttpConnection> {
public:
	friend class LogicSystem;
	HttpConnection(tcp::socket socket);
	void Start();
private:
	void CheckDeadline(); // 定时器
	void PreParseGetParam(); // 参数解析
	void WriteResponse(); // 应答
	void HandleReq(); // 处理请求 解析请求头 解析包体内容
	tcp::socket _socket;
	beast::flat_buffer _buffer{8192}; // 8K缓存区 接受数据
	http::request<http::dynamic_body> _request; // 动态body类型
	http::response<http::dynamic_body> _response;
	// The timer for putting a deadline on connection processing.
	net::steady_timer _deadline{_socket.get_executor(), std::chrono::seconds(60)};

	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

