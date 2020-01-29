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

  const struct option longOpts[] = {
    { "help", no_argument, nullptr, 'h' },
    { "version", no_argument, nullptr, 'V' },
    { "debug", no_argument, nullptr, 'D' },
    { "device", required_argument, nullptr, 'd' },
    { nullptr, 0, nullptr, 0 }
  };

  const char * optString = "hVDd:";
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
    case 'd':
      serial_device = optarg;
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
  -d --device [dev]      Serial device"
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
  
  thread ebz_thread;
  meter->openSerialPort(serial_device);
  ebz_thread = thread(&Ebz::runEbz, meter);

  /*
  thread shm_thread;
  shm_thread = thread(&Ebz::runSharedMem, meter);
  */

  if (ebz_thread.joinable()) {
    ebz_thread.join();
  }

  /*
  if (shm_thread.joinable()) {
    shm_thread.join();
  }
  */

  return 0;
}
