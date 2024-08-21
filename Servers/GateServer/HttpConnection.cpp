#include "HttpConnection.h"
#include "LogicSystem.h"

HttpConnection::HttpConnection(tcp::socket socket):_socket(std::move(socket)) {

}

void HttpConnection::Start() {
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec,
		std::size_t bytes_transferred) {
		try {
			// 重载了bool()方法 所以可以直接判断
			if (ec) {
				std::cout << "http read err is " << ec.what() << std::endl;
				return;
			}

			boost::ignore_unused(bytes_transferred); // http服务器不需要粘包处理 忽略已经发送的字节数
			self->HandleReq();
			self->CheckDeadline();

		} catch (std::exception& exp) {
			std::cout << "exception is" << exp.what() << std::endl;
		}
	});
}

void HttpConnection::CheckDeadline() {
	auto self = shared_from_this();

	_deadline.async_wait([self](beast::error_code ec) {
		if (!ec) {
			// Close socket to cancel any outstanding operation.
			self->_socket.close(ec);
		}
	});
}

void HttpConnection::WriteResponse() {
	auto self = shared_from_this();
	_response.content_length(_response.body().size());
	http::async_write(_socket, _response, [self](beast::error_code ec, std::size_t) {
		self->_socket.shutdown(tcp::socket::shutdown_send, ec);
		self->_deadline.cancel();
	});
}

void HttpConnection::HandleReq() {
	// 设置版本
	_response.version(_request.version());
	_response.keep_alive(false); // 短连接
	if (_request.method() == http::verb::get) {
		bool success = LogicSystem::GetInstance()->HandleGet(_request.target(), shared_from_this());
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}

		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}
}
