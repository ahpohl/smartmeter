#include <iostream>
#include <getopt.h>
#include "ebz.hpp"

int main(int argc, char* argv[])
{
  bool debug = false;
  bool version = false;
  bool help = false;
  char const* serial_device = nullptr;
  char const* ramdisk = nullptr;
  char const* mqtt_host = nullptr;
  char const* mqtt_topic = nullptr;
  int mqtt_port = 0;
  double basic_rate = 0;
  double price_kwh = 0;

  const struct option longOpts[] = {
    { "help", no_argument, nullptr, 'h' },
    { "version", no_argument, nullptr, 'V' },
    { "debug", no_argument, nullptr, 'D' },
    { "serial", required_argument, nullptr, 's' },
    { "ramdisk", required_argument, nullptr, 'r' },
    { "host", required_argument, nullptr, 'H' },
    { "port", required_argument, nullptr, 'p' },
    { "topic", required_argument, nullptr, 't' },
    { "rate", required_argument, nullptr, 'R' },
    { "price", required_argument, nullptr, 'P' },
    { nullptr, 0, nullptr, 0 }
  };

  const char* const optString = "hVDs:r:H:p:t:R:P:";
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
    case 'D':
      debug = true;
      break;
    case 's':
      serial_device = optarg;
      break;
    case 'r':
      ramdisk = optarg;
      break;
    case 'H':
      mqtt_host = optarg;
      break;
    case 'p':
      mqtt_port = atoi(optarg);
      break;
    case 't':
      mqtt_topic = optarg;
      break;
    case 'R':
      basic_rate = atof(optarg);
      break;
    case 'P':
      price_kwh = atof(optarg);
      break;
    default:
      break;
    }

  } while (opt != -1);

  if (help)
  {
    std::cout << "Energy Smartmeter " << VERSION_TAG << std::endl;
    std::cout << std::endl << "Usage: " << argv[0] << " [options]" << std::endl << std::endl;
    std::cout << "\
  -h --help         Show help message\n\
  -V --version      Show build info\n\
  -D --debug        Show debug messages\n\
  -s --serial       Serial device\n\
  -r --ramdisk      Shared memory device\n\
  -H --host         MQTT broker host or ip\n\
  -p --port         MQTT broker port\n\
  -t --topic        MQTT topic to publish\n\
\n\
Electricity tariff:\n\
  -b --rate         Optional basic rate per month\n\
  -k --price        Optional price per kWh" 
    << std::endl << std::endl;
    return 0;
  }

  if (version)
  {
      std::cout << "Version " << VERSION_TAG 
        << " (" << VERSION_BUILD << ") built " 
        << VERSION_BUILD_DATE 
        << " by " << VERSION_BUILD_MACHINE << std::endl;
      return 0;
  }

  std::cout << "Smartmeter " << VERSION_TAG
    << " (" << VERSION_BUILD << ")" << std::endl;

  Ebz* meter = new Ebz();

  if (debug) {
    meter->setDebug();
  }

  meter->openSerialPort(serial_device);
  meter->initMqtt(mqtt_host, mqtt_port, mqtt_topic);
  meter->setTariff(basic_rate, price_kwh);
  meter->createObisPath(ramdisk);

  int ret = 0;
  while (true) {
    ret = meter->readSerialPort();
    if (!ret) {
      meter->readDatagram();
      meter->writeObisCodes();
      meter->publishMqtt();
    }
  }
 
  return 0;
}
