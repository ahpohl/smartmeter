#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

class SmartmeterConfig
{
private:
  std::map<std::string, std::string> KeyValuePairs;
  std::string FileName;
  std::string ErrorMessage;

  template <typename T_VAL>
  T_VAL string_to_T(const std::string &val)
  {
    std::istringstream ss(val);
    T_VAL result;
    if (!(ss >> result))
    {
      ErrorMessage = "Smartmeter config: Not a valid " + (std::string)typeid(T_VAL).name() + " received.";
    }
    return result;
  }

  bool ExtractKeys(void)
  {
	  std::ifstream file;
	  file.open(FileName.c_str());
	  if (!file)
    {
		  ErrorMessage = "Smartmeter config: File " + FileName + " couldn't be found!\n";
      return false;
    }
	  std::string line;
	  size_t line_no = 0;
	  while (std::getline(file, line))
	  {
		  ++line_no;
		  std::string line_copy = line;
		  if (line_copy.empty())
      {
			  continue;
      }
		  if (line_copy.find('#') != line_copy.npos)
      {
        line_copy.erase(line_copy.find('#'));
      }
      if (line_copy.find_first_not_of(' ') == line_copy.npos)
      {
			  continue;
      }
		  ParseLine(line_copy, line_no);
	  }
	  file.close();
    return true;
  }

  bool ValidLine(const std::string &line)
  {
	  std::string temp = line;
	  temp.erase(0, temp.find_first_not_of("\t "));
	  if (temp[0] == '=')
    {
		  return false;
    }
	  for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
    {
		  if (temp[i] != ' ')
      {
			  return true;
      }
    }
	  return false;
  }

  bool ParseLine(const std::string &line, size_t const line_no)
  {
	  if (line.find('=') == line.npos)
    {
		  ErrorMessage = "Smartmeter config: Couldn't find separator on line: " + std::to_string(line_no);
      return false;
    }
	  if (!ValidLine(line))
    {
	    ErrorMessage = "Smartmeter config: Bad format for line: " + std::to_string(line_no);
      return false;
    }

    std::string temp = line;
    temp.erase(0, temp.find_first_not_of("\t "));
    size_t sep_pos = temp.find('=');

    std::string key, value;
    key = temp.substr(0, sep_pos);
    if (key.find('\t') != temp.npos || key.find(' ') != temp.npos)
    {
      key.erase(key.find_first_of("\t "));
    }
    value = temp.substr(sep_pos + 1);
    value.erase(0, value.find_first_not_of("\t "));
    value.erase(value.find_last_not_of("\t ") + 1);

    if (!KeyExists(key))
    {
      KeyValuePairs.insert(std::pair<std::string, std::string>(key, value));
    }
    else
    {
      ErrorMessage = "Smartmeter config: Can only have unique key names.";
      return false;
    }
    return true;
  }  

public:
  SmartmeterConfig(const std::string &file)
  {  
	  FileName = file;
	  ExtractKeys();
  }

  bool KeyExists(const std::string &key)
  {
    return KeyValuePairs.find(key) != KeyValuePairs.end();
  }

  template <typename T_VAL>
  T_VAL GetValueOfKey(const std::string &key, const T_VAL &default_value = T_VAL())
  {
	  if (!KeyExists(key))
		  return default_value;

	  return string_to_T<T_VAL>(KeyValuePairs.find(key)->second);
  }
};
