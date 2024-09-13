#include "RedisMgr.h"
#include "ConfigMgr.h"

RedisMgr::RedisMgr() {
	auto& gConfigMgr = ConfigMgr::Inst();
	auto host = gConfigMgr["Redis"]["Host"];
	auto port = gConfigMgr["Redis"]["Port"];
	auto usr = gConfigMgr["Redis"]["Username"];
	auto pwd = gConfigMgr["Redis"]["Passwd"];
	_con_pool.reset(new RedisConPool(5, host.c_str(), atoi(port.c_str()), usr.c_str(), pwd.c_str()));
}

//RedisMgr::~RedisMgr() {
//	RedisMgr::Close();
//}

//bool RedisMgr::Auth(const std::string& username, const std::string& password) {
//	this->_reply = (redisReply*)redisCommand(this->_connect, "AUTH %s %s", username.c_str(), password.c_str());
//	if (this->_reply->type == REDIS_REPLY_ERROR) {
//		std::cout << "认证失败" << std::endl;
//		//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
//		freeReplyObject(this->_reply);
//		return false;
//	} else {
//		//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
//		freeReplyObject(this->_reply);
//		std::cout << "认证成功" << std::endl;
//		return true;
//	}
//}

//bool RedisMgr::Connect(const std::string& host, int port) {
//	this->_connect = redisConnect(host.c_str(), port);
//	if (this->_connect == nullptr) {
//		return false;
//	}
//	if (this->_connect->err) {
//		std::cout << "connect error " << this->_connect->err << std::endl;
//		return false;
//	}
//	return true;
//}

bool RedisMgr::Get(const std::string& key, std::string& value) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "GET %s", key.c_str());
	if (reply == NULL) {
		std::cout << "[ GET  " << key << " ] failed" << std::endl;
		freeReplyObject(reply);
		return false;
	}

	if (reply->type != REDIS_REPLY_STRING) {
		std::cout << "[ GET  " << key << " ] failed" << std::endl;
		freeReplyObject(reply);
		return false;
	}

	value = reply->str;
	freeReplyObject(reply);

	std::cout << "Succeed to execute command [ GET " << key << "  ]" << std::endl;
	return true;
}

bool RedisMgr::Set(const std::string& key, const std::string& value) {
	//执行redis命令行
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "SET %s %s", key.c_str(), value.c_str());

	//如果返回NULL则说明执行失败
	if (NULL == reply) {
		std::cout << "Execut command [ SET " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}

	//如果执行失败则释放连接
	if (!(reply->type == REDIS_REPLY_STATUS && (strcmp(reply->str, "OK") == 0 || strcmp(reply->str, "ok") == 0))) {
		std::cout << "Execut command [ SET " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}

	//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
	freeReplyObject(reply);
	std::cout << "Execut command [ SET " << key << "  " << value << " ] success ! " << std::endl;
	return true;
}

bool RedisMgr::LPush(const std::string& key, const std::string& value) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "LPUSH %s %s", key.c_str(), value.c_str());
	if (NULL == reply) {
		std::cout << "Execut command [ LPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}

	if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
		std::cout << "Execut command [ LPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}

	std::cout << "Execut command [ LPUSH " << key << "  " << value << " ] success ! " << std::endl;
	freeReplyObject(reply);
	return true;
}

bool RedisMgr::LPop(const std::string& key, std::string& value) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "LPOP %s ", key.c_str());
	if (reply == nullptr || reply->type == REDIS_REPLY_NIL) {
		std::cout << "Execut command [ LPOP " << key << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}
	value = reply->str;
	std::cout << "Execut command [ LPOP " << key << " ] success ! " << std::endl;
	freeReplyObject(reply);
	return true;
}

bool RedisMgr::RPush(const std::string& key, const std::string& value) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "RPUSH %s %s", key.c_str(), value.c_str());
	if (NULL == reply) {
		std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}

	if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
		std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}

	std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] success ! " << std::endl;
	freeReplyObject(reply);
	return true;
}

bool RedisMgr::RPop(const std::string& key, std::string& value) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "RPOP %s ", key.c_str());
	if (reply == nullptr || reply->type == REDIS_REPLY_NIL) {
		std::cout << "Execut command [ RPOP " << key << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}
	value = reply->str;
	std::cout << "Execut command [ RPOP " << key << " ] success ! " << std::endl;
	freeReplyObject(reply);
	return true;
}

bool RedisMgr::HSet(const std::string& key, const std::string& hkey, const std::string& value) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str());
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] success ! " << std::endl;
	freeReplyObject(reply);
	return true;
}


bool RedisMgr::HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	const char* argv[4];
	size_t argvlen[4];
	argv[0] = "HSET";
	argvlen[0] = 4;
	argv[1] = key;
	argvlen[1] = strlen(key);
	argv[2] = hkey;
	argvlen[2] = strlen(hkey);
	argv[3] = hvalue;
	argvlen[3] = hvaluelen;
	auto reply = (redisReply*)redisCommandArgv(connect, 4, argv, argvlen);
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] success ! " << std::endl;
	freeReplyObject(reply);
	return true;
}

std::string RedisMgr::HGet(const std::string& key, const std::string& hkey) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return "";
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	const char* argv[3];
	size_t argvlen[3];
	argv[0] = "HGET";
	argvlen[0] = 4;
	argv[1] = key.c_str();
	argvlen[1] = key.length();
	argv[2] = hkey.c_str();
	argvlen[2] = hkey.length();
	auto reply = (redisReply*)redisCommandArgv(connect, 3, argv, argvlen);
	if (reply == nullptr || reply->type == REDIS_REPLY_NIL) {
		freeReplyObject(reply);
		std::cout << "Execut command [ HGet " << key << " " << hkey << "  ] failure ! " << std::endl;
		return "";
	}

	std::string value = reply->str;
	freeReplyObject(reply);
	std::cout << "Execut command [ HGet " << key << " " << hkey << " ] success ! " << std::endl;
	return value;
}

bool RedisMgr::HDel(const std::string& key, const std::string& field) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});

	redisReply* reply = (redisReply*)redisCommand(connect, "HDEL %s %s", key.c_str(), field.c_str());
	if (reply == nullptr) {
		std::cerr << "HDEL command failed" << std::endl;
		return false;
	}

	bool success = false;
	if (reply->type == REDIS_REPLY_INTEGER) {
		success = reply->integer > 0;
	}

	freeReplyObject(reply);
	return success;

}

bool RedisMgr::Del(const std::string& key) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "DEL %s", key.c_str());
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ Del " << key << " ] failure ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}
	std::cout << "Execut command [ Del " << key << " ] success ! " << std::endl;
	freeReplyObject(reply);
	return true;
}

bool RedisMgr::ExistsKey(const std::string& key) {
	auto connect = _con_pool->getConnection();
	if (connect == nullptr) {
		return false;
	}
	Defer defer([&connect, this]() {
		_con_pool->reclaimConnection(connect);
	});
	auto reply = (redisReply*)redisCommand(connect, "exists %s", key.c_str());
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER || reply->integer == 0) {
		std::cout << "Not Found [ Key " << key << " ]  ! " << std::endl;
		freeReplyObject(reply);
		return false;
	}
	std::cout << " Found [ Key " << key << " ] exists ! " << std::endl;
	freeReplyObject(reply);
	return true;
}

void RedisMgr::Close() {
	_con_pool->Close();
	_con_pool->ClearConnections();
}

RedisConPool::RedisConPool(size_t poolSize, const char* host, int port, const char* usr, const char* pwd):
	_poolSize(poolSize), _b_stop(false), _host(host), _port(port), _usr(usr), _pwd(pwd), _counter(0) {
	for (size_t i = 0; i < _poolSize; ++ i) {
		auto* context = redisConnect(host, port);
		if (context == nullptr || context->err != 0) {
			if (context != nullptr) {
				redisFree(context);
			}
			continue;
		}

		auto reply = (redisReply*)redisCommand(context, "AUTH %s %s", usr, pwd);
		if (reply->type == REDIS_REPLY_ERROR) {
			std::cout << "认证失败" << std::endl;
			//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
			freeReplyObject(reply);
			continue;
		}

		//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
		freeReplyObject(reply);
		std::cout << "认证成功" << std::endl;
		_connections.push(context);
	}
	_check_thread = std::thread([this]() {
		while (!_b_stop) {
			_counter++;
			if (_counter >= 60) {
				checkThread();
				_counter = 0;
			}

			std::this_thread::sleep_for(std::chrono::seconds(1)); // 每隔 30 秒发送一次 PING 命令
		}
	});
}

//RedisConPool::~RedisConPool() {
//	std::lock_guard<std::mutex> lock(_mutex);
//	while (!_connections.empty()) {
//		_connections.pop();
//	}
//}

redisContext* RedisConPool::getConnection() {
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock, [this] {
		if (_b_stop) {
			return true;
		}
		return !_connections.empty();
	});
	//如果停止则直接返回空指针
	if (_b_stop) {
		return  nullptr;
	}
	auto* context = _connections.front();
	_connections.pop();
	return context;
}

void RedisConPool::reclaimConnection(redisContext* context) {
	std::lock_guard<std::mutex> lock(_mutex);
	if (_b_stop) {
		return;
	}
	_connections.push(context);
	_cond.notify_one();
}

void RedisConPool::ClearConnections() {
	std::lock_guard<std::mutex> lock(_mutex);
	while (!_connections.empty()) {
		auto* context = _connections.front();
		redisFree(context);
		_connections.pop();
	}
}

void RedisConPool::Close() {
	_b_stop = true;
	_cond.notify_all();
	_check_thread.join();
}

void RedisConPool::checkThread() {
	std::lock_guard<std::mutex> lock(_mutex);
	if (_b_stop) {
		return;
	}
	auto pool_size = _connections.size();
	for (int i = 0; i < pool_size && !_b_stop; i++) {
		auto* context = _connections.front();
		_connections.pop();
		try {
			auto reply = (redisReply*)redisCommand(context, "PING");
			if (!reply) {
				std::cout << "redis ping failed" << std::endl;
				_connections.push(context);
				continue;
			}
			freeReplyObject(reply);
			_connections.push(context);
		} catch (std::exception& exp) {
			std::cout << "Error keeping connection alive: " << exp.what() << std::endl;
			redisFree(context);
			context = redisConnect(_host, _port);
			if (context == nullptr || context->err != 0) {
				if (context != nullptr) {
					redisFree(context);
				}
				continue;
			}

			auto reply = (redisReply*)redisCommand(context, "AUTH %s %s", _usr, _pwd);
			if (reply->type == REDIS_REPLY_ERROR) {
				std::cout << "认证失败" << std::endl;
				//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
				freeReplyObject(reply);
				continue;
			}

			//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
			freeReplyObject(reply);
			std::cout << "认证成功" << std::endl;
			_connections.push(context);
		}
	}
}