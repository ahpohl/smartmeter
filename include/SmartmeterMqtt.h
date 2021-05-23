#ifndef SmartmeterMqtt_h
#define SmartmeterMqtt_h
#include <string>
#include <mosquitto.h>

class SmartmeterMqtt
{
  static void OnConnectCallbackWrapper(struct mosquitto *mosq, void *obj, int reason_code);
  static void LogCallbackWrapper(struct mosquitto *mosq, void *obj, int level, const char *str);

private:
  struct mosquitto *Mosq;
  void OnConnectCallback(struct mosquitto *mosq, void *obj, int reason_code);
  void LogCallback(struct mosquitto *mosq, void *obj, int level, const char *str);
  std::string ErrorMessage;
  volatile bool IsConnected;
  bool Log;

public:
  SmartmeterMqtt(const bool &log);
  ~SmartmeterMqtt(void);
  bool Begin(void);
  bool Connect(const std::string &host, const int &port, const int &keepalive);
  bool SetLastWillTestament(const std::string &message, const std::string &topic, const int &qos, const bool &retain);
  bool PublishMessage(const std::string &message, const std::string &topic, const int &qos, const bool &retain);
  std::string GetErrorMessage(void) const;
  bool GetConnectStatus(void) const;
};

#endif