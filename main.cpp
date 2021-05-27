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
  if (!(cfg->KeyExists("plan_price_kwh")))
  {
    std::cout << cfg->GetErrorMessage() << std::endl;
  }
  double price = cfg->GetValue<double>("plan_price_kwh");
  std::cout << "Price per kWh: " << std::to_string(price+1) << std::endl;
  
  return EXIT_SUCCESS;
}
