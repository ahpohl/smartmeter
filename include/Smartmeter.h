#ifndef Smartmeter_h
#define Smartmeter_h
#include <sstream>
#include "SmartmeterSerial.h"
#include "SmartmeterMqtt.h"

class Smartmeter
{
private:
  static const int ReceiveBufferSize; 
  SmartmeterSerial *Serial;
  SmartmeterMqtt *Mqtt;
  std::stringstream Payload;
  std::string Topic;
  std::string Username;
  std::string Password;
  std::string CaFile;
  std::string CaPath;
  char *ReceiveBuffer;
  std::string ErrorMessage;
  double BasicRate;
  double PricePerKwh;
  bool Log;
 
  template <typename T_STR, typename T_CHAR>
  T_STR RemoveLeading(T_STR const &str, T_CHAR c);

public:
  Smartmeter(const bool &log);
  ~Smartmeter(void);
  bool Setup(const std::string &device, const std::string &host, const int &port);
  bool Receive(void);
  bool Publish(void);
  bool SetUserPass(const std::string &user, const std::string &pass);
  bool SetEnergyPlan(double const& basic_rate, double const& price_per_kwh);
  bool SetTopic(const std::string &topic);
  bool SetTlsFilePath(const std::string &cafile, const std::string &capath);
  std::string GetErrorMessage(void) const;
  std::string GetReceiveBuffer(void) const;
  std::string GetPayload(void) const;
  
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
};

#endif
