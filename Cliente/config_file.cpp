#include "config_file.h"
#include <fstream>

namespace
{
	void readConfigData(std::map<std::string, std::string>& data, 
		std::ifstream& configFile)
	{
		std::string line;

		while (std::getline(configFile, line))
		{
			if (line[0] == '#')
				continue;

			size_t eqPos = line.find('=');
			if (eqPos == line.npos)
				continue;
			std::string key = line.substr(0, eqPos);
			std::string value = line.substr(eqPos + 1);

			data[key] = value;
		}
	}
}

ConfigFile::ConfigFile(const std::string& filename)
{
	std::ifstream configFile(filename.c_str());

	if (configFile.is_open())
		readConfigData(data_, configFile);
}

ConfigFile::~ConfigFile()
{
}

std::string ConfigFile::readString(const std::string& key) const
{
	TStrStrMap::const_iterator it = data_.find(key);
	if (it == data_.end())
		return std::string();
	else
		return it->second;
}
