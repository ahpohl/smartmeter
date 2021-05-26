#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <cstring>
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
    std::cout << line << std::endl;
	}
	ifs.close();

  return true;
}

std::string SmartmeterConfig::GetErrorMessage(void)
{
  return ErrorMessage;
}
