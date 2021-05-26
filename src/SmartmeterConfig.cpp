#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

class SmartmeterConfig
{
private:
  std::map<std::string, std::string> Contents;
  std::string FileName;
  std::string ErrorMessage;

  template <typename T_VAL>
  std::string T_to_String(const T_VAL &val)
  {
    std::ostringstream ss;
    ss << val;

    return ss.str();
  }

  template <typename T_VAL>
  T_VAL string_to_T(std::string const &val)
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
		  ErrorMessage = "CFG: File " + FileName + " couldn't be found!\n";
      return false;
    }
	  std::string line;
	  size_t line_no = 0;
	  while (std::getline(file, line))
	  {
		  ++line_no;
		  std::string temp = line;
		  if (temp.empty())
      {
			  continue;
      }
		  RemoveComment(temp);
		  if (OnlyWhitespace(temp))
      {
			  continue;
      }
		  ParseLine(temp, line_no);
	  }
	  file.close();
  }

  void RemoveComment(std::string &line) const
  {
    if (line.find(';') != line.npos)
    {
	    line.erase(line.find(';'));
    }
  }

  bool OnlyWhitespace(const std::string &line) const
  {
    return (line.find_first_not_of(' ') == line.npos);
  }

  bool ValidLine(const std::string &line) const
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

  void ExtractKey(std::string &key, size_t const &sep_pos, const std::string &line) const
  {
    key = line.substr(0, sep_pos);
    if (key.find('\t') != line.npos || key.find(' ') != line.npos)
    {
	    key.erase(key.find_first_of("\t "));
    }
  }

  void ExtractValue(std::string &value, size_t const &sep_pos, const std::string &line) const
  {
	  value = line.substr(sep_pos + 1);
	  value.erase(0, value.find_first_not_of("\t "));
	  value.erase(value.find_last_not_of("\t ") + 1);
  }

  bool ExtractContents(const std::string &line) 
  {
	  std::string temp = line;
	  temp.erase(0, temp.find_first_not_of("\t "));
	  size_t sep_pos = temp.find('=');

	  std::string key, value;
	  ExtractKey(key, sep_pos, temp);
	  ExtractValue(value, sep_pos, temp);

	  if (!KeyExists(key))
    {
		  Contents.insert(std::pair<std::string, std::string>(key, value));
    }
	  else
    {
		  ErrorMessage = "Smartmeter config: Can only have unique key names.";
      return false;
    }
    return true;
  }

  bool ParseLine(const std::string &line, size_t const line_no)
  {
	  if (line.find('=') == line.npos)
    {
		  ErrorMessage = "Smartmeter config: Couldn't find separator on line: " + T_to_String(line_no);
      return false;
    }
	  if (!ValidLine(line))
    {
	    ErrorMessage = "Smartmeter config: Bad format for line: " + T_to_String(line_no);
      return false;
    }
	  ExtractContents(line);
    return true;
  }  

public:
  SmartmeterConfig(const std::string &file)
  {  
	  FileName = file;
	  ExtractKeys();
  }

  bool KeyExists(const std::string &key) const
  {
    return Contents.find(key) != Contents.end();
  }

  template <typename T_VAL>
  T_VAL GetValueOfKey(const std::string &key, const T_VAL &default_value = T_VAL()) const
  {
	  if (!KeyExists(key))
		  return default_value;

	  return string_to_T<T_VAL>(Contents.find(key)->second);
  }
};

