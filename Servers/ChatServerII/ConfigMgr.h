#pragma once
#include "const.h"
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  
#include <boost/filesystem.hpp>
#include <string>
#include <map>

struct SectionInfo {
	SectionInfo() = default;
	~SectionInfo();
	SectionInfo(const SectionInfo& src);
	SectionInfo& operator= (const SectionInfo& src);

	std::string  operator[] (const std::string& key);
	std::string GetValue(const std::string& key);
	std::map<std::string, std::string> _section_datas;
};

class ConfigMgr {
public:
	ConfigMgr(const ConfigMgr& src);
	ConfigMgr& operator=(const ConfigMgr& src);
	~ConfigMgr();
	static ConfigMgr& Inst();
	std::string GetValue(const std::string& section, const std::string& key);
	SectionInfo operator[] (const std::string& key);
private:
	ConfigMgr();
	// ´æ´¢sectionºÍkey-value¶ÔµÄmap
	std::map<std::string, SectionInfo> _config_map;
};

