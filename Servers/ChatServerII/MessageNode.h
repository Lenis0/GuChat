#pragma once
#include <string>
#include "const.h"
#include <iostream>
#include <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;
class LogicSystem;

class MessageNode {
public:
	MessageNode(short max_len);

	~MessageNode();

	void Clear();

	short _cur_len;
	short _total_len;
	char* _data;
};

class ReceiveNode:public MessageNode {
	friend class LogicSystem;
public:
	ReceiveNode(short max_len, short msg_id);
private:
	short _msg_id;
};

class SendNode:public MessageNode {
	friend class LogicSystem;
public:
	SendNode(const char* msg, short max_len, short msg_id);
private:
	short _msg_id;
};