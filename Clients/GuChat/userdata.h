#ifndef USERDATA_H
#define USERDATA_H
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <memory>
#include <vector>

class SearchInfo {
public:
    SearchInfo(int uid, QString name, QString nickname, QString desc, int sex, QString icon);
    int _uid;
    QString _name;
    QString _nickname;
    QString _desc;
    int _sex;
    QString _icon;
};

class AddFriendApply {
public:
    AddFriendApply(int from_uid, QString name, QString desc, QString icon, QString nickname, int sex);
    int _from_uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nickname;
    int _sex;
};

struct ApplyInfo {
    ApplyInfo(
        int uid, QString name, QString desc, QString icon, QString nickname, int sex, int status):
        _uid(uid), _name(name), _desc(desc), _icon(icon), _nickname(nickname), _sex(sex),
        _status(status) {}

    ApplyInfo(std::shared_ptr<AddFriendApply> addinfo):
        _uid(addinfo->_from_uid), _name(addinfo->_name), _desc(addinfo->_desc),
        _icon(addinfo->_icon), _nickname(addinfo->_nickname), _sex(addinfo->_sex), _status(0) {}
    void SetIcon(QString head) {
        _icon = head;
    }
    int _uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nickname;
    int _sex;
    int _status;
};

struct AuthInfo {
    AuthInfo(int uid, QString name, QString nickname, QString icon, int sex):
        _uid(uid), _name(name), _nickname(nickname), _icon(icon), _sex(sex) {}
    int _uid;
    QString _name;
    QString _nickname;
    QString _icon;
    int _sex;
};

struct AuthRsp {
    AuthRsp(int peer_uid, QString peer_name, QString peer_nickname, QString peer_icon, int peer_sex):
        _uid(peer_uid), _name(peer_name), _nickname(peer_nickname), _icon(peer_icon),
        _sex(peer_sex) {}

    int _uid;
    QString _name;
    QString _nickname;
    QString _icon;
    int _sex;
};

struct TextChatData;
struct FriendInfo {
    FriendInfo(int uid,
               QString name,
               QString nickname,
               QString icon,
               int sex,
               QString desc,
               QString back,
               QString last_msg = ""):
        _uid(uid), _name(name), _nickname(nickname), _icon(icon), _sex(sex), _desc(desc),
        _back(back), _last_msg(last_msg) {}

    FriendInfo(std::shared_ptr<AuthInfo> auth_info):
        _uid(auth_info->_uid), _name(auth_info->_name), _nickname(auth_info->_nickname),
        _icon(auth_info->_icon), _sex(auth_info->_sex) {}

    FriendInfo(std::shared_ptr<AuthRsp> auth_rsp):
        _uid(auth_rsp->_uid), _name(auth_rsp->_name), _nickname(auth_rsp->_nickname),
        _icon(auth_rsp->_icon), _sex(auth_rsp->_sex) {}

    void AppendChatMsgs(const std::vector<std::shared_ptr<TextChatData>> text_vec);

    int _uid;
    QString _name;
    QString _nickname;
    QString _icon;
    int _sex;
    QString _desc;
    QString _back;
    QString _last_msg;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};

struct UserInfo {
    UserInfo(int uid, QString name, QString nickname, QString icon, int sex, QString last_msg = ""):
        _uid(uid), _name(name), _nickname(nickname), _icon(icon), _sex(sex), _last_msg(last_msg) {}

    UserInfo(std::shared_ptr<AuthInfo> auth):
        _uid(auth->_uid), _name(auth->_name), _nickname(auth->_nickname), _icon(auth->_icon),
        _sex(auth->_sex), _last_msg("") {}

    UserInfo(int uid, QString name, QString icon):
        _uid(uid), _name(name), _nickname(name), _icon(icon), _sex(0), _last_msg("") {}

    UserInfo(std::shared_ptr<AuthRsp> auth):
        _uid(auth->_uid), _name(auth->_name), _nickname(auth->_nickname), _icon(auth->_icon),
        _sex(auth->_sex), _last_msg("") {}

    UserInfo(std::shared_ptr<SearchInfo> search_info):
        _uid(search_info->_uid), _name(search_info->_name), _nickname(search_info->_nickname),
        _icon(search_info->_icon), _sex(search_info->_sex), _last_msg("") {}

    UserInfo(std::shared_ptr<FriendInfo> friend_info):
        _uid(friend_info->_uid), _name(friend_info->_name), _nickname(friend_info->_nickname),
        _icon(friend_info->_icon), _sex(friend_info->_sex), _last_msg("") {
        _chat_msgs = friend_info->_chat_msgs;
    }

    int _uid;
    QString _name;
    QString _nickname;
    QString _icon;
    int _sex;
    QString _last_msg;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};

struct TextChatData {
    TextChatData(QString msg_id, QString msg_content, int fromuid, int touid):
        _msg_id(msg_id), _msg_content(msg_content), _from_uid(fromuid), _to_uid(touid) {}
    QString _msg_id;
    QString _msg_content;
    int _from_uid;
    int _to_uid;
};

struct TextChatMsg {
    TextChatMsg(int fromuid, int touid, QJsonArray arrays): _to_uid(touid), _from_uid(fromuid) {
        for (auto msg_data : arrays) {
            auto msg_obj = msg_data.toObject();
            auto content = msg_obj["content"].toString();
            auto msgid = msg_obj["msgid"].toString();
            auto msg_ptr = std::make_shared<TextChatData>(msgid, content, fromuid, touid);
            _chat_msgs.push_back(msg_ptr);
        }
    }
    int _to_uid;
    int _from_uid;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};

#endif
