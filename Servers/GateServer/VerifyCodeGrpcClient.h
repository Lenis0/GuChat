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

class VerifyCodeGrpcClient: public Singleton<VerifyCodeGrpcClient> {
	friend class Singleton<VerifyCodeGrpcClient>;
public:
	GetVerifyCodeRsp GetVerifyCode(std::string email);
private:
	VerifyCodeGrpcClient();
	std::unique_ptr<VerifyCodeService::Stub> _stub; // пей╧
};

