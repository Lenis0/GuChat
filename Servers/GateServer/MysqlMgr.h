#pragma once
#include "const.h"
#include "Singleton.h"
#include "MysqlDao.h"
class MysqlMgr: public Singleton<MysqlMgr> {
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdatePasswd(const std::string& name, const std::string& pwd);
private:
	MysqlMgr();
	MysqlDao _dao;
};