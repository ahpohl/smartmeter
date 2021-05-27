#include <iostream>
#include <string>
#include <memory>
#include "SmartmeterConfig.h"


int main(int argc, char *argv[])
{
  std::unique_ptr<SmartmeterConfig> cfg(new SmartmeterConfig());
  
  if (!(cfg->Begin(argv[1])))
  {
    std::cout << cfg->GetErrorMessage() << std::endl;
    exit(EXIT_FAILURE);  
  }
  
  cfg->ShowConfig();
  if (!(cfg->KeyExists("mqtt_user")))
  {
    std::cout << cfg->GetErrorMessage() << std::endl;
  }
  std::cout << "MQTT username: " << cfg->GetValue("mqtt_user") << std::endl;
  
  std::string pass = cfg->GetValue("mqtt_pas");
  if (pass.empty())
  {
    std::cout << "pass is empty string" << std::endl;
  }
 
  return EXIT_SUCCESS;
}
