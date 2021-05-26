#ifndef SmartmeterConfig_h
#define SmartmeterConfig_h
#include <string>
#include <map>

class SmartmeterConfig
{
private:
  std::map<std::string, std::string> KeyValuePairs;
  std::string File;
  std::string ErrorMessage;

public:
  SmartmeterConfig(const std::string &file);
  bool Begin(void);
};

#endif
