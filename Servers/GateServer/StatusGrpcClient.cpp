#include "StatusGrpcClient.h"
#include "ConfigMgr.h"

StatusConPool::StatusConPool(size_t poolSize, std::string host, std::string port): poolSize_(poolSize), host_(host), port_(port), b_stop_(false) {
	for (size_t i = 0; i < poolSize_; ++ i) {

		std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
			grpc::InsecureChannelCredentials());

		connections_.push(StatusService::NewStub(channel));
	}
}

StatusConPool::~StatusConPool() {
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty()) {
		connections_.pop();
	}
}

std::unique_ptr<StatusService::Stub> StatusConPool::getConnection() {
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this] {
		if (b_stop_) {
			return true;
		}
		return !connections_.empty();
	});
	if (b_stop_) {
		return  nullptr;
	}
	auto context = std::move(connections_.front());
	connections_.pop();
	return context;
}

void StatusConPool::reclaimConnection(std::unique_ptr<StatusService::Stub> context) {
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}
	connections_.push(std::move(context));
	cond_.notify_one();
}

void StatusConPool::Close() {
	b_stop_ = true;
	cond_.notify_all();
}

StatusGrpcClient::~StatusGrpcClient() {

}

GetChatServerRsp StatusGrpcClient::GetChatServer(int uid) {
	ClientContext context;
	GetChatServerRsp reply;
	GetChatServerReq request;
	request.set_uid(uid);
	auto stub = pool_->getConnection();
	Status status = stub->GetChatServer(&context, request, &reply);
	Defer defer([&stub, this]() {
		pool_->reclaimConnection(std::move(stub));
	});
	if (status.ok()) {
		return reply;
	} else {
		reply.set_error(ErrorCodes::RPCFailed);
		return reply;
	}
}

StatusGrpcClient::StatusGrpcClient() {
	auto& gConfigMgr = ConfigMgr::Inst();
	std::string host = gConfigMgr["StatusServer"]["Host"];
	std::string port = gConfigMgr["StatusServer"]["Port"];
	pool_.reset(new StatusConPool(5, host, port));
}
