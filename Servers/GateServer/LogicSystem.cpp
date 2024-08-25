#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyCodeGrpcClient.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"

LogicSystem::~LogicSystem() {
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con) {
	if (_get_handlers.find(path) == _get_handlers.end()) {
		return false;
	}

	_get_handlers[path](con);
	return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con) {
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}

	_post_handlers[path](con);
	return true;
}

void LogicSystem::RegGet(std::string url, HttpHandler handler) {
	_get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler) {
	_post_handlers.insert(make_pair(url, handler));

}

LogicSystem::LogicSystem() {
	RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->_response.body()) << "测试receive get_test req " << std::endl;
		int i = 0;
		for (auto& elem : connection->_get_params) {
			++ i;
			beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;
			beast::ostream(connection->_response.body()) << ", " << " value is " << elem.second << std::endl;
		}
	});

	RegPost("/get_verifycode", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success || !src_root.isMember("email")) {
			std::cout << "Failed to parse JSON data" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->_response.body()) << json_str;
			return true;
		}

		auto email = src_root["email"].asString();
		GetVerifyCodeRsp rsp = VerifyCodeGrpcClient::GetInstance()->GetVerifyCode(email);
		std::cout << "email is " << email << std::endl;
		root["error"] = rsp.error();
		root["email"] = src_root["email"];
		std::string json_str = root.toStyledString();
		beast::ostream(connection->_response.body()) << json_str;
		return true;
	});

	RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		auto email = src_root["email"].asString();
		auto name = src_root["user"].asString();
		auto pwd = src_root["passwd"].asString();
		auto verifycode = src_root["verifycode"].asString();

		//先查找redis中email对应的验证码是否合理
		//std::string  verify_code;
		//bool b_get_verify = RedisMgr::GetInstance()->Get(CODEPREFIX + email, verify_code);
		//if (!b_get_verify) {
		//	std::cout << " get varify code expired" << std::endl;
		//	root["error"] = ErrorCodes::VerifyCodeExpired;
		//	std::string jsonstr = root.toStyledString();
		//	beast::ostream(connection->_response.body()) << jsonstr;
		//	return true;
		//}

		//if (verify_code != verifycode) {
		//	std::cout << " varify code error" << std::endl;
		//	root["error"] = ErrorCodes::VerifyCodeErr;
		//	std::string jsonstr = root.toStyledString();
		//	beast::ostream(connection->_response.body()) << jsonstr;
		//	return true;
		//}

		//查找数据库判断用户是否存在
		int uid = MysqlMgr::GetInstance()->RegUser(name, email, pwd);
		if (uid == 0 || uid == -1) {
			std::cout << "user or email exist" << std::endl;
			root["error"] = ErrorCodes::UserExist;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		root["error"] = 0;
		root["email"] = email;
		root["uid"] = uid;
		root["user"] = name;
		root["passwd"] = pwd;
		root["repasswd"] = src_root["repasswd"].asString();
		root["verifycode"] = verifycode;
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;
	});
}
