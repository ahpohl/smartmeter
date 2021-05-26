#include <iostream>
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

  return EXIT_SUCCESS;
}
