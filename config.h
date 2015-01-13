#ifndef CONFIG_H__
#define CONFIG_H__
#include "head.h"
class Config
{
public:
	Config();
	~Config();
	bool Init(const std::string& path);
	bool GetValue(const std::string& key, std::string& value);
private:
	std::string path;
	map <string, string> configs;
	std::string& trim_right(
		std::string& s,
		const std::string& delimiters = " \f\n\r\t\v");
	std::string& trim_left(
		std::string& s,
		const std::string& delimiters = " \f\n\r\t\v");
	std::string& trim(
		std::string& s,
		const std::string& delimiters = " \f\n\r\t\v");
};
inline std::string& Config::trim_right(
	std::string& s,
	const std::string& delimiters)
{
	return s.erase(s.find_last_not_of(delimiters) + 1);
}

inline std::string& Config::trim_left(
	std::string& s,
	const std::string& delimiters)
{
	return s.erase(0, s.find_first_not_of(delimiters));
}

inline std::string& Config::trim(
	std::string& s,
	const std::string& delimiters)
{
	return trim_left(trim_right(s, delimiters), delimiters);
}
#endif
