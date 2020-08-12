#ifndef MQTT_H
#define MQTT_H 

#include <mosquitto.h>

class Mosq
{
public:
  Mosq(char const* t_host, int t_port);
  ~Mosq();
  void send_message(std::string t_topic, std::string t_message);

private:
  struct mosquitto* m_mosq;

  static void on_publish(struct mosquitto* t_mosq, void* t_obj, int t_mid);
  static void on_connect(struct mosquitto* t_mosq, void* t_obj, int t_rc);
};

#endif // MQTT_H
