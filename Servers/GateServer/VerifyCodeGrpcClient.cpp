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
	_cond.notify_all(); // ֪ͨ������߳� ����Ҫ������
}

std::unique_ptr<VerifyCodeService::Stub> RPCConPool::getConnection() {
	std::unique_lock<std::mutex> lock(_mutex);
	// true�ͼ�������ִ�в��Ҽ��� false�Ļ� �Ƚ��̹߳����ҽ��� ���������� �ȴ�notify֪ͨ�� �ټ���������
	_cond.wait(lock, [this]() {
		return _b_stop ? true : !_connections.empty();
	});

	//���ֹͣ��ֱ�ӷ��ؿ�ָ��
	if (_b_stop) {
		return nullptr;
	}

	auto context = std::move(_connections.front());
	_connections.pop();
	return context;
}

void RPCConPool::reclaimConnection(std::unique_ptr<VerifyCodeService::Stub> context) {
	std::lock_guard<std::mutex> lock(_mutex);

	// ��Դ�����þͲ���Ҫ������Ŷ�����
	if (_b_stop) {
		return;
	}
	_connections.push(std::move(context));
	_cond.notify_one(); // ֪ͨһ�������߳�
}
