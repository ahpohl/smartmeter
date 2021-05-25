#ifndef SmartmeterConfig_h
#define SmartmeterConfig_h
#include <map>
#include <set>
#include <sstream>

class SmartmeterConfig
{
private:
  std::map<std::string, std::string> KeyValuePair;
  std::string ErrorMessage;

public:
  bool Begin(const std::string &file);
  void ShowConfig(void) const;
  std::string GetErrorMessage(void) const;

  bool KeyExists(const std::string &key);
  bool ValidateKeys(std::set<std::string> valid_keys);
  std::string GetValue(const std::string &key) const; 
};

#endif
