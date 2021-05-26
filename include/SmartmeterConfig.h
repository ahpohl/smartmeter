#ifndef SmartmeterConfig_h
#define SmartmeterConfig_h
#include <string>
#include <map>

class SmartmeterConfig
{
private:
  std::map<std::string, std::string> KeyValuePair;
  std::string ErrorMessage;
  void TruncateString(std::string &str, const char &delim);

  template <typename T_VAL>
  T_VAL StringToVal(const std::string &val);

public:
  bool Begin(const std::string &file);
  bool IsKeyDefined(const std::string &key);
  std::string GetErrorMessage(void);

  template <typename T_VAL>
  T_VAL GetValue(const std::string &key);
};

#endif
