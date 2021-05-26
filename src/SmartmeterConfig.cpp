#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <cstring>
#include <typeinfo>
#include <iterator>
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
    if (line.find_first_not_of(' ') == std::string::npos)
    {
			continue;
    }
    std::string key, val;
    std::istringstream iss(line);
    while (std::getline(std::getline(iss, key, ' ') >> std::ws, val))
    {
      KeyValuePair.insert(std::map<std::string, std::string>::value_type(key, val));
    }
	}
	ifs.close();

  for(auto p = KeyValuePair.cbegin(); p != KeyValuePair.cend(); ++p)
  {
    std::cout << '{' << p->first << " => " << p->second << '}' << std::endl;
  }

  return true;
}

std::string SmartmeterConfig::GetErrorMessage(void)
{
  return ErrorMessage;
}

template <typename T>
T SmartmeterConfig::StringToVal(const std::string &str)
{
  std::istringstream iss(str);
  T result; 
  iss >> result;
  if (iss.fail())
  {
    ErrorMessage = std::string("Unable to convert string to ") + typeid(T).name();
    return T();
  }
  return result;
}
