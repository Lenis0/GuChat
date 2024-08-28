#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory> // ����ָ��
#include <iostream>
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h> // ����
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
	Error_Json = 1001, // Json��������
	RPCFailed = 1002, // RPC�������
	RedisFailed = 1003, // Redis����ʧ��
	VerifyCodeExpired = 1004, // ��֤�����
	VerifyCodeErr = 1005, // ��֤�����
	UserExist = 1006, // �û��Ѵ���
	PasswdErr = 1007, // �������
	EmailNotMatch = 1008, // ���䲻ƥ��
	PasswdUpFailed = 1009, // ��������ʧ��
	PasswdInvalid = 10010, // ����ʧЧ
	TokenInvalid = 1011,   //TokenʧЧ
	UidInvalid = 1012,  //uid��Ч
};

// Defer��
class Defer {
public:
	// ����һ��lambda���ʽ���ߺ���ָ��
	Defer(std::function<void()> func): func_(func) {}

	// ����������ִ�д���ĺ���
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define CODEPREFIX "code_"
