#ifndef SmartmeterMqtt_h
#define SmartmeterMqtt_h
#include <mosquitto.h>
#include <string>

class SmartmeterMqtt {
  static void OnConnectCallbackWrapper(struct mosquitto *mosq, void *obj,
                                       int connack_code);
  static void LogCallbackWrapper(struct mosquitto *mosq, void *obj, int level,
                                 const char *str);

public:
  SmartmeterMqtt(void);
  ~SmartmeterMqtt(void);
  void SetDebug(const bool &debug);
  bool Begin(void);
  bool Connect(const std::string &host, const int &port, const int &keepalive);
  bool SetUserPassAuth(const std::string &user, const std::string &pass);
  bool SetLastWillTestament(const std::string &message,
                            const std::string &topic, const int &qos,
                            const bool &retain);
  bool SetTlsConnection(const std::string &cafile, const std::string &capath);
  bool PublishMessage(const std::string &message, const std::string &topic,
                      const int &qos, const bool &retain);
  std::string GetErrorMessage(void) const;
  bool GetConnectStatus(void) const;
  bool GetNotifyOnlineFlag(void) const;
  void SetNotifyOnlineFlag(const bool &flag);

private:
  struct mosquitto *Mosq;
  void OnConnectCallback(struct mosquitto *mosq, void *obj, int connack_code);
  void LogCallback(struct mosquitto *mosq, void *obj, int level,
                   const char *str);
  std::string ErrorMessage;
  volatile bool IsConnected;
  volatile bool NotifyOnlineFlag;
  bool Log;
};

#endif
