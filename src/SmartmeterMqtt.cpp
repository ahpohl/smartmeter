#include <iostream>
#include <thread>
#include <chrono>
#include "SmartmeterMqtt.h"

SmartmeterMqtt::SmartmeterMqtt(const bool &log): Log(log) 
{
  IsConnected = false;
}

SmartmeterMqtt::~SmartmeterMqtt(void)
{
  if (IsConnected)
  {
    mosquitto_disconnect(Mosq);
  }
  mosquitto_loop_stop(Mosq, false);
  mosquitto_destroy(Mosq);
  mosquitto_lib_cleanup();
}

bool SmartmeterMqtt::Begin(void)
{
  mosquitto_lib_init();
  bool clean_session = true;
  if (!(Mosq = mosquitto_new(nullptr, clean_session, this)))
  {
    ErrorMessage = std::string("Mosquitto: Out of memory.");
    return false;
  }
  mosquitto_connect_callback_set(Mosq, OnConnectCallbackWrapper);
  mosquitto_log_callback_set(Mosq, LogCallbackWrapper);
  return true;
}

bool SmartmeterMqtt::Connect(const std::string &host, const int &port, const int &keepalive)
{  
  int rc = 0;
  if ((rc = mosquitto_connect_async(Mosq, host.c_str(), port, keepalive)))
  {
    ErrorMessage = std::string("Mosquitto unable to connect: ") + mosquitto_strerror(rc);
    return false;
  }
  int count = 0;
  int timeout = 100;
  while (!IsConnected)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    if ((rc = mosquitto_loop(Mosq, 0, 1)))
    {
      //std::cout << "Mosquitto loop failed: " << mosquitto_strerror(rc) << std::endl;
      //return false;
    }
    if (count > timeout)
    {
      ErrorMessage = "Mosquitto unable to connect: Timeout";
      return false;
    }
    ++count;
  }
  if ((rc = mosquitto_loop_start(Mosq)))
  {
    ErrorMessage = std::string("Mosquitto loop start failed: ") + mosquitto_strerror(rc);
    return false;
  }

  return true;
}

bool SmartmeterMqtt::SetLastWillTestament(const std::string &message, const std::string &topic, const int &qos, const bool &retain)
{
  int rc = 0;
  if ((rc = mosquitto_will_set(Mosq, topic.c_str(), message.size(), message.c_str(), qos, retain)))
  {
    ErrorMessage = std::string("Mosquitto unable to set last will testament: ") + mosquitto_strerror(rc);
    return false;
  }
  return true;
}

bool SmartmeterMqtt::PublishMessage(const std::string &message, const std::string &topic, const int &qos, const bool &retain)
{
  int rc = 0;
  if ((rc = mosquitto_publish(Mosq, nullptr, topic.c_str(), message.size(), message.c_str(), qos, retain)))
  {
    ErrorMessage = std::string("Mosquitto publish failed: ") + mosquitto_strerror(rc);
    IsConnected = false;
    return false;
  }
  return true;
}

std::string SmartmeterMqtt::GetErrorMessage(void) const
{
  return ErrorMessage;
}

bool SmartmeterMqtt::GetConnectStatus(void) const
{
  return IsConnected;
}

void SmartmeterMqtt::OnConnectCallback(struct mosquitto *mosq, void *obj, int reason_code)
{
  if (!reason_code)
  {
    IsConnected = true;
  }
}

void SmartmeterMqtt::OnConnectCallbackWrapper(struct mosquitto *mosq, void *obj, int reason_code)
{
  auto *p = reinterpret_cast<SmartmeterMqtt*>(obj);
  return p->SmartmeterMqtt::OnConnectCallback(mosq, obj, reason_code);
}

void SmartmeterMqtt::LogCallback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
  if (Log)
  {
    std::cout << str << std::endl;
  }
}

void SmartmeterMqtt::LogCallbackWrapper(struct mosquitto *mosq, void *obj, int level, const char *str)
{
  auto *p = reinterpret_cast<SmartmeterMqtt*>(obj);
  return p->SmartmeterMqtt::LogCallback(mosq, obj, level, str); 
}
