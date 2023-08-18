#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include "SmartmeterConfig.h"

bool SmartmeterConfig::Begin(const std::string &file)
{
	if (file.empty())
	{
		ErrorMessage = "Smartmeter config: Config argument empty.";
		return false;
	}
	std::ifstream ifs(file);
	if (!ifs)
	{
		ErrorMessage = std::string("Opening config file failed: ")
    		  + strerror(errno) + " (" + std::to_string(errno) + ")";
		return false;
	}
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
		{
			continue;
		}
		size_t pos = 0;
		if ((pos = line.find('#')) != std::string::npos)
		{
			line.erase(pos);
		}
		if ((pos = line.find_first_not_of(' ')) == std::string::npos)
		{
			continue;
		}
		std::string key, val;
		std::istringstream iss(line);
		std::getline(std::getline(iss >> std::ws, key, ' ') >> std::ws, val, ' ');
		if ((pos = val.find_first_not_of('\"')) != std::string::npos)
		{
			val.erase(0, pos);
		}
		if ((pos = val.find('\"')) != std::string::npos)
		{
			val.erase(pos);
		}
		KeyValuePair.insert(std::map<std::string, std::string>::value_type(key, val));
	}
	ifs.close();

	return true;
}

std::string SmartmeterConfig::GetErrorMessage(void) const
{
	return ErrorMessage;
}

void SmartmeterConfig::ShowConfig(void) const
{
	for(auto it = KeyValuePair.cbegin(); it != KeyValuePair.cend(); ++it)
	{
		std::cout << '{' << it->first << " => " << it->second << '}' << std::endl;
	}
}

bool SmartmeterConfig::KeyExists(const std::string &key)
{
	if (KeyValuePair.find(key) == KeyValuePair.cend())
	{
		ErrorMessage = std::string("Smartmeter config: Key \"") + key + "\" not found.";
		return false;
	}
	return true;
}

std::string SmartmeterConfig::GetValue(const std::string &key) const
{
	return KeyValuePair.find(key)->second;
}

bool SmartmeterConfig::ValidateKeys(std::set<std::string> valid_keys)
{
	for(auto it = KeyValuePair.cbegin(); it != KeyValuePair.cend(); ++it)
	{
		if ((valid_keys.find(it->first) == valid_keys.cend()))
		{
			ErrorMessage = std::string("Smartmeter config: Invalid key \"") + it->first + "\"";
			return false;
		}
		if ((it->second).empty())
		{
			ErrorMessage = std::string("Smartmeter config: Key \"") + it->first + "\" has no value.";
			return false;
		}
	}
	return true;
}
