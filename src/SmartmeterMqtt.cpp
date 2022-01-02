#include <iostream>
#include <thread>
#include <chrono>
#include "SmartmeterMqtt.h"
#include "SmartmeterEnums.h"

SmartmeterMqtt::SmartmeterMqtt(void): Log(0) 
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

void SmartmeterMqtt::SetLogLevel(const unsigned char &log_level)
{
  Log = log_level;
}

bool SmartmeterMqtt::Begin(void)
{
  mosquitto_lib_init();
  bool clean_session = true;
  if (!(Mosq = mosquitto_new(nullptr, clean_session, this)))
  {
    ErrorMessage = std::string("Mosquitto error: Out of memory.");
    return false;
  }
  mosquitto_connect_callback_set(Mosq, OnConnectCallbackWrapper);
  mosquitto_log_callback_set(Mosq, LogCallbackWrapper);
  return true;
}

bool SmartmeterMqtt::SetUserPassAuth(const std::string &user, const std::string &pass)
{
  /*
  if (user.empty()) 
  {
    ErrorMessage = std::string("Mosquitto unable to enable password authentication: User argument empty.");
    return false;
  }
  if (pass.empty())
  {
    ErrorMessage = std::string("Mosquitto unable to enable password authentication: Password argument empty.");
    return false;
  }
  */
  int rc = 0;
  if ((rc = mosquitto_username_pw_set(Mosq, user.c_str(), pass.c_str())))
  {
    ErrorMessage = std::string("Mosquitto unable to enable password authentication: ") + mosquitto_strerror(rc);
    return false;
  }
  return true;
}

bool SmartmeterMqtt::SetTlsConnection(const std::string &cafile, const std::string &capath)
{
  int rc = 0;
  if (!(cafile.empty()))
  {
    if ((rc = mosquitto_tls_set(Mosq, cafile.c_str(), NULL, NULL, NULL, NULL)))
    {
      ErrorMessage = std::string("Mosquitto unable to enable TLS: ") + mosquitto_strerror(rc);
      return false;
    }
  }
  else if (!(capath.empty()))
  {
    if ((rc = mosquitto_tls_set(Mosq, NULL, capath.c_str(), NULL, NULL, NULL)))
    {
      ErrorMessage = std::string("Mosquitto unable to enable TLS: ") + mosquitto_strerror(rc);
      return false;
    }
  }
  /*
  else
  {
    ErrorMessage = std::string("Mosquitto unable to enable TLS: Need either cafile or capath argument.");
    return false;
  }
  */
  return true;
}

bool SmartmeterMqtt::Connect(const std::string &host, const int &port, const int &keepalive)
{
  /*
  if (host.empty())
  {
    ErrorMessage = std::string("Mosquitto unable to connect: Mqtt host argument empty.");
    return false;
  }
  if (!port)
  {
    ErrorMessage = std::string("Mosquitto unable to connect: Mqtt port argument empty.");
    return false;
  }
  */
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
    if (!(ErrorMessage.empty()))
    {
      return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    mosquitto_loop(Mosq, 0, 1);
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
    ErrorMessage = std::string("Mosquitto unable to set last will: ") + mosquitto_strerror(rc);
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

void SmartmeterMqtt::OnConnectCallback(struct mosquitto *mosq, void *obj, int connack_code)
{
  if (!connack_code)
  {
    IsConnected = true;
  }
  else
  {
    ErrorMessage = mosquitto_connack_string(connack_code);
    IsConnected = false;
  }
}

void SmartmeterMqtt::OnConnectCallbackWrapper(struct mosquitto *mosq, void *obj, int connack_code)
{
  auto *p = reinterpret_cast<SmartmeterMqtt*>(obj);
  return p->SmartmeterMqtt::OnConnectCallback(mosq, obj, connack_code);
}

void SmartmeterMqtt::LogCallback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
  if (Log & static_cast<unsigned char>(LogLevelEnum::MQTT))
  {
    std::cout << str << std::endl;
  }
}

void SmartmeterMqtt::LogCallbackWrapper(struct mosquitto *mosq, void *obj, int level, const char *str)
{
  auto *p = reinterpret_cast<SmartmeterMqtt*>(obj);
  return p->SmartmeterMqtt::LogCallback(mosq, obj, level, str); 
}
