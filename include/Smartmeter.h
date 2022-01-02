#ifndef Smartmeter_h
#define Smartmeter_h
#include "SmartmeterSerial.h"
#include "SmartmeterMqtt.h"
#include "SmartmeterConfig.h"
#include "SmartmeterEnums.h"

class Smartmeter
{
  static const int ReceiveBufferSize;
  static const std::set<std::string> ValidKeys;

public:
  Smartmeter(void);
  ~Smartmeter(void);
  void SetLogLevel(void);
  bool Setup(const std::string &config);
  bool Receive(void);
  bool Publish(void);
  std::string GetErrorMessage(void) const;
  unsigned char GetLogLevel(void) const;
  
  struct Datagram
  {
    std::string SerialNum;    // serial number and software version
    std::string CustomId;     // custom id specific to user
    std::string DeviceId;     // device id DIN 43863-5
    std::string Energy;       // energy counter [kWh], resolution 10 µW*h, format %f6.8
    std::string Power;        // power output (3-phases), resolution 0.01 W, format %f5.2
    std::string PowerL1;      // power phase L1
    std::string PowerL2;      // power phase L2
    std::string PowerL3;      // power phase L3
    std::string VoltageL1;    // voltage phase L1, 0.1 V resolution
    std::string VoltageL2;    // voltage phase L2
    std::string VoltageL3;    // voltage phase L3
    unsigned long SensorTime; // time of operation, in seconds
    std::string Status;       // status word, 4 byte hex
  } Datagram;

private:
  SmartmeterSerial *Serial;
  SmartmeterMqtt *Mqtt;
  SmartmeterConfig *Cfg;
  std::stringstream Payload;
  std::string Config;
  char *ReceiveBuffer;
  std::string ErrorMessage;
  unsigned char Log;

  template <typename T_STR, typename T_CHAR>
  T_STR RemoveLeading(T_STR const &str, T_CHAR c) const;

  template <typename T>
  T StringTo(const std::string &str) const;
};

#endif
