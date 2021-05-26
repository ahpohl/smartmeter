#include "SmartmeterConfig.h"
#include <memory>

int main(int argc, char *argv[])
{
  std::unique_ptr<SmartmeterConfig> cfg(new SmartmeterConfig(argv[1]));
  cfg->Begin();

  return EXIT_SUCCESS;
}
