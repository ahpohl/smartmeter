#include <iostream>
#include <string>
#include <memory>
#include <set>
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

  std::set<std::string> keys {"mqtt_broker", "mqtt_password", "mqtt_port", "mqtt_topic", "mqtt_user", "plan_basic_rate", "plan_price_kwh", "serial_device"};

  if (!cfg->VerifyKeys(keys))
  {
    std::cout << cfg->GetErrorMessage() << std::endl;
  }
 
  return EXIT_SUCCESS;
}
