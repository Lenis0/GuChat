#include "MysqlMgr.h"

MysqlMgr::~MysqlMgr() {

}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(int uid) {
	return _dao.GetUser(uid);
}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(std::string name) {
	return _dao.GetUser(name);
}

bool MysqlMgr::AddFriendApply(const int& from, const int& to) {
	return _dao.AddFriendApply(from, to);
}

bool MysqlMgr::AuthFriendApply(const int& from, const int& to) {
	return _dao.AuthFriendApply(from, to);
}

bool MysqlMgr::AddFriend(const int& from, const int& to, std::string back_name) {
	return _dao.AddFriend(from, to, back_name);
}

bool MysqlMgr::GetApplyList(int touid, std::vector<std::shared_ptr<ApplyInfo>>& applyList, int begin, int limit) {
	return _dao.GetApplyList(touid, applyList, begin, limit);
}

bool MysqlMgr::GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>>& user_info_list) {
	return _dao.GetFriendList(self_id, user_info_list);
}

MysqlMgr::MysqlMgr() {

}