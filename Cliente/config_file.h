#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include "config.h"
#include <map>

class ConfigFile : public Config
{
	typedef std::map<std::string, std::string> TStrStrMap;
	TStrStrMap data_;

protected:
	virtual std::string readString(const std::string& key) const;

public:
	ConfigFile(const std::string& filename);
	virtual ~ConfigFile();
};



#endif
