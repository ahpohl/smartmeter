#ifndef SmartmeterConfig_h
#define SmartmeterConfig_h
#include <string>
#include <map>

class SmartmeterConfig
{
private:
  std::map<std::string, std::string> KeyValuePairs;
  std::string ErrorMessage;

public:
  bool Begin(const std::string &file);
  bool IsKeyDefined(const std::string &key);
  std::string GetErrorMessage(void);

  template <typename T_VAL>
  T_VAL GetValue(const std::string &key);
};

#endif
