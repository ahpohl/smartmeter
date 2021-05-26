#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include "SmartmeterConfig.h"

SmartmeterConfig::SmartmeterConfig(const std::string &file) : File(file)
{
} 

bool SmartmeterConfig::Begin(void)
{
  if (File.empty())
  {
    ErrorMessage = "Smartmeter config: Config argument empty.";
    return false;
  }
  std::ifstream ifs(File);
  if (!ifs)
  {
    ErrorMessage = "Smartmeter config: Opening config file failed.";
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
