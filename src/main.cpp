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

  const struct option longOpts[] = {
    { "help", no_argument, nullptr, 'h' },
    { "version", no_argument, nullptr, 'V' },
    { "debug", no_argument, nullptr, 'D' },
    { "serial", required_argument, nullptr, 's' },
    { "ramdisk", required_argument, nullptr, 'r' },
    { nullptr, 0, nullptr, 0 }
  };

  const char* const optString = "hVDs:r:";
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
    default:
      break;
    }

  } while (opt != -1);

  if (help)
  {
    cout << "Energy Pulsemeter " << VERSION_TAG << endl;
    cout << endl << "Usage: " << argv[0] << " [options]" << endl << endl;
    cout << "\
  -h --help              Show help message\n\
  -V --version           Show build info\n\
  -D --debug             Show debug messages\n\
  -s --serial [dev]      Serial device\n\
  -r --ramdisk [dev]     Shared memory device"    
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

  thread ramdisk_thread;
  meter->createObisPath(ramdisk);
  ramdisk_thread = thread(&Ebz::runWriteSharedMem, meter);

  if (serial_thread.joinable()) {
    serial_thread.join();
  }

  if (ramdisk_thread.joinable()) {
    ramdisk_thread.join();
  }

  return 0;
}
