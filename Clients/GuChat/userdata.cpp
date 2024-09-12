#include "userdata.h"

SearchInfo::SearchInfo(int uid, QString name, QString nickname, QString desc, int sex, QString icon):
    _uid(uid), _name(name), _nickname(nickname), _desc(desc), _sex(sex), _icon(icon) {}

AddFriendApply::AddFriendApply(
    int from_uid, QString name, QString desc, QString icon, QString nickname, int sex):
    _from_uid(from_uid), _name(name), _desc(desc), _icon(icon), _nickname(nickname), _sex(sex) {}

void FriendInfo::AppendChatMsgs(const std::vector<std::shared_ptr<TextChatData> > text_vec) {
    for (const auto& text : text_vec) {
        _chat_msgs.push_back(text);
    }
}
