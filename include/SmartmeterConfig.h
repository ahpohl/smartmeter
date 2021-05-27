#ifndef SmartmeterConfig_h
#define SmartmeterConfig_h
#include <string>
#include <map>
#include <vector>
#include <sstream>

class SmartmeterConfig
{
private:
  std::map<std::string, std::string> KeyValuePair;
  std::string ErrorMessage;

  template <typename T_VAL>
  T_VAL StringToVal(const std::string &val);

public:
  bool Begin(const std::string &file);
  std::string GetErrorMessage(void);
  void ShowConfig(void);
  std::vector<std::string> GetKeys(void);
  std::map<std::string, std::string> GetKeyValuePairs(void);
  bool KeyExists(const std::string &key);
  
  template <typename T>
  inline T GetValue(const std::string &key)
  {
    auto p = KeyValuePair.find(key);
    if (p == KeyValuePair.end())
    {
      return T();
    }
    std::istringstream iss(p->second);
    T result;
    iss >> result;
    if (iss.fail())
    {
      ErrorMessage = std::string("Unable to convert string to ") + typeid(T).name();
      return T();
    }
    return result;
  }
};

#endif
