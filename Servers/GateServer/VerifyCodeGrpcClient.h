#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVerifyCodeReq;
using message::GetVerifyCodeRsp;
using message::VerifyCodeService;

class RPCConPool {
public:
	RPCConPool(std::size_t poolsize, std::string host, std::string port);
	~RPCConPool();
	void Close();
	std::unique_ptr<VerifyCodeService::Stub> getConnection();
	void reclaimConnection(std::unique_ptr<VerifyCodeService::Stub> context);
private:
	std::atomic<bool> _b_stop;
	std::size_t _poolSize;
	std::string _host;
	std::string _port;
	// �Ż��ռ䣺����ʹ��listʵ�ֶ��� ���ҿ�������ͷ��β�������� Ч�ʺܸ�
	std::queue<std::unique_ptr<VerifyCodeService::Stub>> _connections;
	std::mutex _mutex;
	std::condition_variable	_cond;
};

class VerifyCodeGrpcClient: public Singleton<VerifyCodeGrpcClient> {
	friend class Singleton<VerifyCodeGrpcClient>;
public:
	GetVerifyCodeRsp GetVerifyCode(std::string email);
private:
	VerifyCodeGrpcClient();
	std::unique_ptr<RPCConPool> _pool;
	//std::unique_ptr<VerifyCodeService::Stub> _stub; // ��ʹ
};

