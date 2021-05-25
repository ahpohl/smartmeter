#include <string>
#include <iostream>
#include <memory>
#include <getopt.h>
#include <csignal>
#include <Smartmeter.h>

volatile sig_atomic_t shutdown = false;

void sig_handler(int)
{
  shutdown = true;
}

int main(int argc, char* argv[])
{
  struct sigaction action;
  action.sa_handler = sig_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  
  int verbose_level = 0;
  bool version = false;
  bool help = false;
  std::string serial_device;
  std::string mqtt_host;
  std::string mqtt_topic;
  std::string mqtt_user;
  std::string mqtt_pass;
  std::string mqtt_tls_cafile;
  int mqtt_port = 0;
  double basic_rate = 0;
  double price_per_kwh = 0;

  const struct option longOpts[] = {
    { "help", no_argument, nullptr, 'h' },
    { "version", no_argument, nullptr, 'V' },
    { "verbose", no_argument, nullptr, 'v' },
    { "serial", required_argument, nullptr, 's' },
    { "host", required_argument, nullptr, 'H' },
    { "port", required_argument, nullptr, 'P' },
    { "topic", required_argument, nullptr, 't' },
    { "user", required_argument, nullptr, 'u' },
    { "pass", required_argument, nullptr, 'p' },
    { "cafile", required_argument, nullptr, 'c' },
    { "rate", required_argument, nullptr, 'R' },
    { "price", required_argument, nullptr, 'K' },
    { nullptr, 0, nullptr, 0 }
  };

  const char optString[] = "hVvs:H:P:t:u:p:c:R:K:";
  int opt = 0;
  int longIndex = 0;

  do {
    opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
    switch (opt) {
    case 'h':
      help = true;
      break;
    case 'V':
      version = true;
      break;
    case 'v':
      ++verbose_level;
      break;
    case 's':
      serial_device = optarg;
      break;
    case 'H':
      mqtt_host = optarg;
      break;
    case 'P':
      mqtt_port = atoi(optarg);
      break;
    case 't':
      mqtt_topic = optarg;
      break;
    case 'u':
      mqtt_user = optarg;
      break;
    case 'p':
      mqtt_pass = optarg;
      break;
    case 'c':
      mqtt_tls_cafile = optarg;
      break;
    case 'R':
      basic_rate = atof(optarg);
      break;
    case 'K':
      price_per_kwh = atof(optarg);
      break;
    default:
      break;
    }

  } while (opt != -1);

  if (help)
  {
    std::cout << "Energy Smartmeter " << VERSION_TAG << std::endl;
    std::cout << std::endl << "Usage: " << argv[0] << " [options]" << std::endl;
    std::cout << "\
  -h --help         Show help message\n\
  -V --version      Show build info\n\
  -v --verbose      Set verbose output level\n\
\n\
Required:\n\
  -s --serial       Serial device\n\
  -H --host         MQTT broker host or ip\n\
  -P --port         MQTT broker port\n\
  -t --topic        MQTT topic to publish\n\
  -R --rate         Basic rate per month\n\
  -K --price        Price per kWh\n\
\n\
Optional:\n\
  -u --user         MQTT username\n\
  -p --pass         MQTT password\n\
  -c --cafile       MQTT TLS CA file"
    << std::endl << std::endl;
    return EXIT_SUCCESS;
  }

  if (version)
  {
    std::cout << "Version " << VERSION_TAG 
      << " (" << VERSION_BUILD << ") built " 
      << VERSION_BUILD_DATE 
      << " by " << VERSION_BUILD_MACHINE << std::endl;
    return EXIT_SUCCESS;
  }

  std::cout << "Smartmeter " << VERSION_TAG
    << " (" << VERSION_BUILD << ")" << std::endl;

  bool log = (verbose_level == 3) ? true : false;
  std::unique_ptr<Smartmeter> meter(new Smartmeter(log));
  
  if (!meter->SetTopic(mqtt_topic))
  {
    std::cout << meter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  if (!meter->SetUserPass(mqtt_user, mqtt_pass))
  {
    std::cout << meter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  meter->SetCaFile(mqtt_tls_cafile);
  if (!meter->Setup(serial_device, mqtt_host, mqtt_port))
  {
    std::cout << meter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  if (!meter->SetEnergyPlan(basic_rate, price_per_kwh))
  {
    std::cout << meter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }

  while (shutdown == false)
  {
	  if (!meter->Receive())
	  {
	    std::cout << meter->GetErrorMessage() << std::endl;
      return EXIT_FAILURE;
 	  }
    if (!meter->Publish())
    {
      std::cout << meter->GetErrorMessage() << std::endl;
    }
    switch (verbose_level)
    {
      case 2:
        std::cout << meter->GetReceiveBuffer();
        break;
      case 1:
        std::cout << meter->GetPayload() << std::endl;
        break;
      default:
        break;
    }
  }
 
  return EXIT_SUCCESS;
}
