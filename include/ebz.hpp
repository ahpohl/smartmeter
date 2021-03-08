#ifndef EBZ_HPP
#define EBZ_HPP

#include <ctime>
#include <chrono>
#include <string>
#include "mosq.hpp"

class Ebz
{
public:
  Ebz(void);
  ~Ebz(void);
  void setDebug(void);
  
  void runReadSerial(void);
  void openSerialPort(const char* const t_device);
  int readSerialPort(void);
  void readDatagram(void);
  
  void runObis(void) const;
  void createObisPath(const char* const t_ramdisk) const;
  void writeObisCodes(void) const;
  
  void runMqtt(void) const;
  void initMqtt(char const* t_host, int t_port, char const* t_topic);
  void setTariff(double const& t_rate, double const& t_price);
  void publishMqtt(void) const;

  static const char* const OBIS_SERIAL_NUMBER;
  static const char* const OBIS_CUSTOM_ID;
  static const char* const OBIS_DEVICE_ID;
  static const char* const OBIS_ENERGY;       // Meter reading Active Energy import( A+ ) [kWh]
  static const char* const OBIS_POWER_TOTAL;  // Active total power [W]
  static const char* const OBIS_POWER_L1;     // Active power L1 [W]
  static const char* const OBIS_POWER_L2;     // Active power L2 [W]
  static const char* const OBIS_POWER_L3;     // Active power L3 [W]
  static const char* const OBIS_VOLTAGE_L1;   // Voltage L1 [V]
  static const char* const OBIS_VOLTAGE_L2;   // Voltage L2 [V]
  static const char* const OBIS_VOLTAGE_L3;   // Voltage L3 [V]
  static const char* const OBIS_STATUS;
  static const char* const OBIS_SECONDS_INDEX;
  static int const D0_DATAGRAM_SIZE;
  static int const SERIAL_BUFFER_SIZE;
  static int const OBIS_BUFFER_SIZE;

private:
  bool m_debug;
  int m_serialport;           // serial port
  char* m_serialnum;          // serial number and software version
  char* m_customid;           // custom id specific to user
  char* m_deviceid;           // device id DIN 43863-5
  double m_energy;            // energy counter [kWh], resolution 10 µW*h, format %f6.8
  double m_power;             // power output (3-phases), resolution 0.01 W, format %f5.2
  double m_powerl1;           // power phase L1
  double m_powerl2;           // power phase L2
  double m_powerl3;           // power phase L3
  double m_voltagel1;         // voltage phase L1, 0.1 V resolution
  double m_voltagel2;         // voltage phase L2
  double m_voltagel3;         // voltage phase L3
  unsigned long m_sensortime; // time of operation, in seconds
  unsigned long long m_now;   // unix epoch, in ms
  char* m_status;             // status word, 4 byte hex
  char* m_datagram;           // buffer for a d0 datagram
  Mosq* m_mqtt;               // pointer to mosquitto client object
  std::string m_topic;        // MQTT topic to publish to
  double m_rate;              // electricity tariff basic rate per year
  double m_price;             // electricity tariff price per kWh
 
  void configureSerialPort(unsigned char const& t_vmin,
    unsigned char const& t_vtime) const;
};

#endif // EBZ_HPP
