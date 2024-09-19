#pragma once
#include "const.h"
#include "Singleton.h"
#include "MysqlDao.h"
class MysqlMgr: public Singleton<MysqlMgr> {
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	std::shared_ptr<UserInfo> GetUser(int uid);
	std::shared_ptr<UserInfo> GetUser(std::string name);
	bool AddFriendApply(const int& from, const int& to);
	bool AuthFriendApply(const int& from, const int& to);
	bool AddFriend(const int& from, const int& to, std::string back_name);
	bool GetApplyList(int touid, std::vector<std::shared_ptr<ApplyInfo>>& applyList, int begin, int limit = 10);
	bool GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo> >& user_info_list);
private:
	MysqlMgr();
	MysqlDao _dao;
};