#include <iostream>
#include <thread>
#include <getopt.h>
#include "ebz.hpp"

using namespace std;

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

  const struct option longOpts[] = {
    { "help", no_argument, nullptr, 'h' },
    { "version", no_argument, nullptr, 'V' },
    { "debug", no_argument, nullptr, 'D' },
    { "serial", required_argument, nullptr, 's' },
    { "ramdisk", required_argument, nullptr, 'r' },
    { "host", required_argument, nullptr, 'H' },
    { "port", required_argument, nullptr, 'p' },
    { "topic", required_argument, nullptr, 't' },
    { nullptr, 0, nullptr, 0 }
  };

  const char* const optString = "hVDs:r:H:p:t:";
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
    default:
      break;
    }

  } while (opt != -1);

  if (help)
  {
    cout << "Energy Smartmeter " << VERSION_TAG << endl;
    cout << endl << "Usage: " << argv[0] << " [options]" << endl << endl;
    cout << "\
  -h --help         Show help message\n\
  -V --version      Show build info\n\
  -D --debug        Show debug messages\n\
  -s --serial       Serial device\n\
  -r --ramdisk      Shared memory device\n\
  -H --host         MQTT broker host or ip\n\
  -p --port         MQTT broker port\n\
  -t --topic        MQTT topic to publish"    
    << endl << endl;
    return 0;
  }

  if (version)
  {
      cout << "Version " << VERSION_TAG 
        << " (" << VERSION_BUILD << ") built " 
        << VERSION_BUILD_DATE 
        << " by " << VERSION_BUILD_MACHINE << endl;
      return 0;
  }

  cout << "Smartmeter " << VERSION_TAG
    << " (" << VERSION_BUILD << ")" << endl;

  shared_ptr<Ebz> meter(new Ebz());

  if (debug) {
    meter->setDebug();
  }

  thread serial_thread;
  meter->openSerialPort(serial_device);
  serial_thread = thread(&Ebz::runReadSerial, meter);

  thread mqtt_thread;
  meter->initMqtt(mqtt_host, mqtt_port, mqtt_topic);
  mqtt_thread = thread(&Ebz::runMqtt, meter);

  thread obis_thread;
  meter->createObisPath(ramdisk);
  obis_thread = thread(&Ebz::runObis, meter);

  if (serial_thread.joinable()) {
    serial_thread.join();
  }
  if (obis_thread.joinable()) {
    obis_thread.join();
  }
  if (mqtt_thread.joinable()) {
    mqtt_thread.join();
  }
  
  return 0;
}
