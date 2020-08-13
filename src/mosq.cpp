#include <iostream>
#include <string>
#include <mosquitto.h>

#include "mosq.hpp"

Mosq::Mosq(char const* t_host, int t_port, bool t_debug)
{
  int keepalive = 120;
  bool clean_session = true;
  int rc = 0;

  mosquitto_lib_init();
  m_mosq = mosquitto_new(nullptr, clean_session, nullptr);
  if (!m_mosq) {
    throw std::runtime_error(">> Mosq - Out of memory!");
  }
  if (t_debug) {
    mosquitto_connect_callback_set(m_mosq, on_connect);
    mosquitto_disconnect_callback_set(m_mosq, on_disconnect);
    mosquitto_publish_callback_set(m_mosq, on_publish);
  }
  rc = mosquitto_connect_async(m_mosq, t_host, t_port, keepalive);
  if (rc != MOSQ_ERR_SUCCESS) {
    std::cout << ">> Mosq - Connection with server failed (" << rc << ")"  << std::endl;
  }
  if (mosquitto_loop_start(m_mosq) != MOSQ_ERR_SUCCESS) {
    std::cout << ">> Mosq - loop_start() failed" << std::endl;
  }
}

Mosq::~Mosq()
{
  if (mosquitto_disconnect(m_mosq) != MOSQ_ERR_SUCCESS) {
    std::cout << ">> Mosq - Disconnect from server failed" << std::endl;
  }
  if (mosquitto_loop_stop(m_mosq, false) != MOSQ_ERR_SUCCESS) {
    std::cout << ">> Mosq - loop_stop() failed" << std::endl;
  }
  mosquitto_destroy(m_mosq);
  mosquitto_lib_cleanup();
}

void Mosq::send_message(std::string t_topic, std::string t_message)
{
  int ret = mosquitto_publish(m_mosq, nullptr, t_topic.c_str(), t_message.size(),
    t_message.c_str(), 1, false);
  if (ret != MOSQ_ERR_SUCCESS) {
    std::cout << ">> Mosq - Sending message failed (" << ret << ")" << std::endl;
  }
}

void Mosq::on_publish(struct mosquitto* t_mosq, void* t_obj, int t_mid)
{
  std::cout << ">> Mosq - Sending message (" << t_mid << ")" << std::endl;
}

void Mosq::on_connect(struct mosquitto* t_mosq, void* t_obj, int t_rc)
{
  std::cout << ">> Mosq - Connected to server (" << t_rc << ")" << std::endl;
}

void Mosq::on_disconnect(struct mosquitto* t_mosq, void* t_obj, int t_rc)
{
  std::cout << ">> Mosq - Disconnected from server (" << t_rc << ")" << std::endl;
}
