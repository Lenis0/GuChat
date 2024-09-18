#pragma once
#include "ConfigMgr.h"
#include "const.h"
#include <mysql/jdbc.h>
#include <mutex>
#include <queue>
#include "data.h"

class SqlConnection {
public:
	SqlConnection(sql::Connection* con, int64_t lasttime);
	std::unique_ptr<sql::Connection> _con;
	int64_t _last_oper_time;
};

class MySqlPool {
public:
	MySqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize);
	std::unique_ptr<SqlConnection> getConnection();
	void checkConnection();
	void reclaimConnection(std::unique_ptr<SqlConnection> con);
	void Close();
	~MySqlPool();

private:
	std::string url_;
	std::string user_;
	std::string pass_;
	std::string schema_;
	int poolSize_;
	std::queue<std::unique_ptr<SqlConnection>> pool_;
	std::mutex mutex_;
	std::condition_variable cond_;
	std::atomic<bool> b_stop_;
	std::thread _check_thread;
};


class MysqlDao {
public:
	MysqlDao();
	~MysqlDao();
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdatePasswd(const std::string& name, const std::string& newpwd);
	bool CheckPasswd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
	bool AddFriendApply(const int& from, const int& to);

	std::shared_ptr<UserInfo> GetUser(int uid);
	std::shared_ptr<UserInfo> GetUser(std::string name);
private:
	std::unique_ptr<MySqlPool> pool_;
};

