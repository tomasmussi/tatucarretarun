#ifndef CONFIG_H
#define CONFIG_H

#include <sstream>

class Config
{
protected:
	Config() {}
	virtual std::string readString(const std::string& key) const = 0;

public:
	virtual ~Config() {}

	template <typename T>
	T read(const std::string& key) const
	{
		std::istringstream iss(readString(key));
		T ret;
		iss >> ret;

		return ret;
	}
};


#endif
