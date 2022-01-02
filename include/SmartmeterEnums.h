#ifndef SmartmeterEnums_h
#define SmartmeterEnums_h

enum class LogLevelEnum : unsigned char
{
  CONFIG = 0x01,
  JSON = 0x02,
  MQTT = 0x04,
  SERIAL = 0x08
};

#endif
