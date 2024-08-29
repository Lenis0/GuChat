#pragma once
#include "Singleton.h"
#include "const.h"

class RedisConPool {
public:
	RedisConPool(size_t poolSize, const char* host, int port, const char* usr, const char* pwd);
	~RedisConPool();
	redisContext* getConnection();
	void reclaimConnection(redisContext* context);
	void Close();
private:
	std::atomic<bool> _b_stop;
	size_t _poolSize;
	const char* _host;
	int _port;
	std::queue<redisContext*> _connections;
	std::mutex _mutex;
	std::condition_variable _cond;
};

class RedisMgr: public Singleton<RedisMgr>,
	public std::enable_shared_from_this<RedisMgr> {
	friend class Singleton<RedisMgr>;
public:
	~RedisMgr();
	//bool Auth(const std::string& username, const std::string& password);
	//bool Connect(const std::string& host, int port);
	bool Get(const std::string& key, std::string& value);
	bool Set(const std::string& key, const std::string& value);
	bool LPush(const std::string& key, const std::string& value);
	bool LPop(const std::string& key, std::string& value);
	bool RPush(const std::string& key, const std::string& value);
	bool RPop(const std::string& key, std::string& value);
	bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
	bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);
	std::string HGet(const std::string& key, const std::string& hkey);
	bool Del(const std::string& key);
	bool ExistsKey(const std::string& key);
	void Close();
private:
	RedisMgr();

	// µ¥
	//redisContext* _connect;
	//redisReply* _reply;

	// ¶à
	std::unique_ptr<RedisConPool> _con_pool;
};