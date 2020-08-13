#ifndef MOSQ_H
#define MOSQ_H 

#include <mosquitto.h>

class Mosq
{
public:
  Mosq(char const* t_host, int t_port, bool t_debug);
  ~Mosq();
  void send_message(std::string t_topic, std::string t_message);

private:
  struct mosquitto* m_mosq;
  static void on_publish(struct mosquitto* t_mosq, void* t_obj, int t_mid);
  static void on_connect(struct mosquitto* t_mosq, void* t_obj, int t_rc);
  static void on_disconnect(struct mosquitto* t_mosq, void* t_obj, int t_rc);
};

#endif // MOSQ_H
