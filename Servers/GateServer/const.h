#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory> // 智能指针
#include <iostream>
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h> // 解析
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sw/redis++/redis++.h>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001, // Json解析错误
	RPCFailed = 1002, // RPC请求错误
	RedisFailed = 1003, // Redis服务失败
	VerifyCodeExpired = 1004, // 验证码过期
	VerifyCodeErr = 1005, // 验证码错误
	UserExist = 1006, // 用户已存在
	PasswdErr = 1007, // 密码错误
	EmailNotMatch = 1008, // 邮箱不匹配
	PasswdUpFailed = 1009, // 更新密码失败
	PasswdInvalid = 10010, // 密码失效
	TokenInvalid = 1011,   //Token失效
	UidInvalid = 1012,  //uid无效
};

// Defer类
class Defer {
public:
	// 接受一个lambda表达式或者函数指针
	Defer(std::function<void()> func): func_(func) {}

	// 析构函数中执行传入的函数
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define CODEPREFIX "code_"
