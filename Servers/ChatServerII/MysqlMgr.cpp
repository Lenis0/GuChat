#include "MysqlMgr.h"

MysqlMgr::~MysqlMgr() {

}

int MysqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd) {
	return _dao.RegUser(name, email, pwd);
}

bool MysqlMgr::CheckEmail(const std::string& name, const std::string& email) {
	return _dao.CheckEmail(name, email);
}

bool MysqlMgr::UpdatePasswd(const std::string& name, const std::string& pwd) {
	return _dao.UpdatePasswd(name, pwd);
}

bool MysqlMgr::CheckPasswd(const std::string& name, const std::string& pwd, UserInfo& userInfo) {
	return _dao.CheckPasswd(name, pwd, userInfo);
}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(int uid) {
	return _dao.GetUser(uid);
}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(std::string name) {
	return _dao.GetUser(name);
}


MysqlMgr::MysqlMgr() {

}