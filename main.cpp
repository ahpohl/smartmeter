#include "SmartmeterConfig.h"
#include <memory>

int main(int argc, char *argv[])
{
  std::unique_ptr<SmartmeterConfig> cfg(new SmartmeterConfig("resources/smartmeter.conf"));
  cfg->Begin();

  return EXIT_SUCCESS;
}
