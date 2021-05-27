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

public:
  bool Begin(const std::string &file);
  std::string GetErrorMessage(void) const;
  void ShowConfig(void) const;
  std::vector<std::string> GetKeys(void) const;
  std::map<std::string, std::string> GetKeyValuePair(void) const;
  bool KeyExists(const std::string &key);
  std::string GetValue(const std::string &key) const; 
 
  template <typename T = std::string>
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
