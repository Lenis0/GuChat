#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOContextPool.h"

// 如果成员是const、引用，或者属于某种未提供默认构造函数的类类型，
// 我们必须通过构造函数初始值列表为这些成员提供初值。
CServer::CServer(net::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {

}

void CServer::Start() {
	auto self = shared_from_this(); // 生成智能指针 防止回调没有回调过来时类被析构
	auto& io_context = AsioIOContextPool::GetInstance()->GetIOContext();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			// 出错放弃这次连接，继续监听其他连接
			if (ec) {
				self->Start();
				return;
			}

			// 创建新连接，并且创建HttpConnection类管理这个链接
			//std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			new_con->Start(); // 启动连接
			// 继续监听
			self->Start();
		} catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
		}
	});
}
