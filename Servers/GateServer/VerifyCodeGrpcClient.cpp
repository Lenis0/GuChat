#include "VerifyCodeGrpcClient.h"

GetVerifyCodeRsp VerifyCodeGrpcClient::GetVerifyCode(std::string email) {
	{
		ClientContext context;
		GetVerifyCodeReq request;
		GetVerifyCodeRsp response;

		Status status = _stub->GetVerifyCode(&context, request, &response);

		if (status.ok()) {
			return response;
		} else {
			response.set_error(ErrorCodes::RPCFailed);
			return response;
		}
	}
}

VerifyCodeGrpcClient::VerifyCodeGrpcClient() {
	std::shared_ptr<Channel> channel = grpc::CreateChannel("0.0.0.0:50051",
		grpc::InsecureChannelCredentials());
	_stub = VerifyCodeService::NewStub(channel);
}
