#include "VerifyCodeGrpcClient.h"
#include "ConfigMgr.h"

GetVerifyCodeRsp VerifyCodeGrpcClient::GetVerifyCode(std::string email) {
	{
		ClientContext context;
		GetVerifyCodeReq request;
		GetVerifyCodeRsp response;

		request.set_email(email);
		auto stub = _pool->getConnection();
		Status status = stub->GetVerifyCode(&context, request, &response);
		if (status.ok()) {
			_pool->reclaimConnection(std::move(stub));
			return response;
		} else {
			_pool->reclaimConnection(std::move(stub));
			response.set_error(ErrorCodes::RPCFailed);
			return response;
		}
	}
}

VerifyCodeGrpcClient::VerifyCodeGrpcClient() {
	auto& gCfgMgr = ConfigMgr::Inst();
	std::string host = gCfgMgr["VerifyCodeServer"]["Host"];
	std::string port = gCfgMgr["VerifyCodeServer"]["Port"];
	_pool.reset(new RPCConPool(5, host, port));
}

RPCConPool::RPCConPool(std::size_t poolsize, std::string host, std::string port)
	:_poolSize(poolsize), _host(host), _port(port), _b_stop(false) {
	for (size_t i = 0; i < _poolSize; ++ i) {

		std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
			grpc::InsecureChannelCredentials());

		_connections.push(VerifyCodeService::NewStub(channel));
	}
}

RPCConPool::~RPCConPool() {
	std::lock_guard<std::mutex> lock(_mutex);
	Close();
	while (!_connections.empty()) {
		_connections.pop();
	}
}

void RPCConPool::Close() {
	_b_stop = true;
	_cond.notify_all(); // 通知挂起的线程 池子要回收了
}

std::unique_ptr<VerifyCodeService::Stub> RPCConPool::getConnection() {
	std::unique_lock<std::mutex> lock(_mutex);
	// true就继续往下执行并且加锁 false的话 先将线程挂起并且解锁 不会往下走 等待notify通知到 再继续往下走
	_cond.wait(lock, [this]() {
		return _b_stop ? true : !_connections.empty();
	});

	//如果停止则直接返回空指针
	if (_b_stop) {
		return nullptr;
	}

	auto context = std::move(_connections.front());
	_connections.pop();
	return context;
}

void RPCConPool::reclaimConnection(std::unique_ptr<VerifyCodeService::Stub> context) {
	std::lock_guard<std::mutex> lock(_mutex);

	// 资源不可用就不需要往里面放东西了
	if (_b_stop) {
		return;
	}
	_connections.push(std::move(context));
	_cond.notify_one(); // 通知一个挂起线程
}
