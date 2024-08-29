#pragma once
#include "const.h"

struct SectionInfo {
	SectionInfo() = default;
	~SectionInfo();
	SectionInfo(const SectionInfo& src);
	SectionInfo& operator= (const SectionInfo& src);

	std::string  operator[] (const std::string& key);
	std::map<std::string, std::string> _section_datas;
};

class ConfigMgr {
public:
	ConfigMgr(const ConfigMgr& src);
	ConfigMgr& operator=(const ConfigMgr& src);
	~ConfigMgr();
	static ConfigMgr& Inst();
	SectionInfo operator[] (const std::string& key);
private:
	ConfigMgr();
	// ´æ´¢sectionºÍkey-value¶ÔµÄmap
	std::map<std::string, SectionInfo> _config_map;
};

