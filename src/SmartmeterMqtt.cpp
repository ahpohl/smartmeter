#include "SmartmeterMqtt.h"
#include <chrono>
#include <iostream>
#include <mosquitto.h>
#include <thread>

SmartmeterMqtt::SmartmeterMqtt(void) : Mosq(nullptr), Log(false) {
  IsConnected = false;
  NotifyOnlineFlag = false;
}

SmartmeterMqtt::~SmartmeterMqtt(void) {
  if (IsConnected) {
    mosquitto_disconnect(Mosq);
  }
  mosquitto_loop_stop(Mosq, false);
  mosquitto_destroy(Mosq);
  mosquitto_lib_cleanup();
}

void SmartmeterMqtt::SetDebug(const bool &debug) { Log = debug; }

bool SmartmeterMqtt::Begin(void) {
  mosquitto_lib_init();
  bool clean_session = true;
  if (!(Mosq = mosquitto_new(nullptr, clean_session, this))) {
    ErrorMessage = std::string("Mosquitto error: Out of memory.");
    return false;
  }
  mosquitto_connect_callback_set(Mosq, OnConnectCallbackWrapper);
  mosquitto_log_callback_set(Mosq, LogCallbackWrapper);
  return true;
}

bool SmartmeterMqtt::SetUserPassAuth(const std::string &user,
                                     const std::string &pass) {
  int rc = 0;
  if ((rc = mosquitto_username_pw_set(Mosq, user.c_str(), pass.c_str()))) {
    ErrorMessage =
        std::string("Mosquitto unable to enable password authentication: ") +
        mosquitto_strerror(rc);
    return false;
  }
  return true;
}

bool SmartmeterMqtt::SetTlsConnection(const std::string &cafile,
                                      const std::string &capath) {
  int rc = 0;
  if (!(cafile.empty())) {
    if ((rc =
             mosquitto_tls_set(Mosq, cafile.c_str(), NULL, NULL, NULL, NULL))) {
      ErrorMessage = std::string("Mosquitto unable to enable TLS: ") +
                     mosquitto_strerror(rc);
      return false;
    }
  } else if (!(capath.empty())) {
    if ((rc =
             mosquitto_tls_set(Mosq, NULL, capath.c_str(), NULL, NULL, NULL))) {
      ErrorMessage = std::string("Mosquitto unable to enable TLS: ") +
                     mosquitto_strerror(rc);
      return false;
    }
  }
  return true;
}

bool SmartmeterMqtt::Connect(const std::string &host, const int &port,
                             const int &keepalive) {
  int rc = 0;
  if ((rc = mosquitto_loop_start(Mosq))) {
    ErrorMessage =
        std::string("Mosquitto loop start failed: ") + mosquitto_strerror(rc);
    return false;
  }
  if ((rc = mosquitto_connect_async(Mosq, host.c_str(), port, keepalive))) {
    ErrorMessage =
        std::string("Mosquitto unable to connect: ") + mosquitto_strerror(rc);
    return false;
  }

  return true;
}

bool SmartmeterMqtt::SetLastWillTestament(const std::string &message,
                                          const std::string &topic,
                                          const int &qos, const bool &retain) {
  int rc = 0;
  if ((rc = mosquitto_will_set(Mosq, topic.c_str(), message.size(),
                               message.c_str(), qos, retain))) {
    ErrorMessage = std::string("Mosquitto unable to set last will: ") +
                   mosquitto_strerror(rc);
    return false;
  }
  return true;
}

bool SmartmeterMqtt::PublishMessage(const std::string &message,
                                    const std::string &topic, const int &qos,
                                    const bool &retain) {
  int rc = 0;
  if ((rc = mosquitto_publish(Mosq, nullptr, topic.c_str(), message.size(),
                              message.c_str(), qos, retain))) {
    ErrorMessage =
        std::string("Mosquitto publish failed: ") + mosquitto_strerror(rc);
    IsConnected = false;
    return false;
  }
  return true;
}

std::string SmartmeterMqtt::GetErrorMessage(void) const { return ErrorMessage; }

bool SmartmeterMqtt::GetConnectStatus(void) const { return IsConnected; }

bool SmartmeterMqtt::GetNotifyOnlineFlag(void) const {
  return NotifyOnlineFlag;
}

void SmartmeterMqtt::SetNotifyOnlineFlag(const bool &flag) {
  NotifyOnlineFlag = flag;
}

void SmartmeterMqtt::OnConnectCallback(struct mosquitto *mosq, void *obj,
                                       int connack_code) {
  if (!connack_code) {
    IsConnected = true;
    NotifyOnlineFlag = true;
  } else {
    ErrorMessage = mosquitto_connack_string(connack_code);
    IsConnected = false;
  }
}

void SmartmeterMqtt::OnConnectCallbackWrapper(struct mosquitto *mosq, void *obj,
                                              int connack_code) {
  auto *p = reinterpret_cast<SmartmeterMqtt *>(obj);
  return p->SmartmeterMqtt::OnConnectCallback(mosq, obj, connack_code);
}

void SmartmeterMqtt::LogCallback(struct mosquitto *mosq, void *obj, int level,
                                 const char *str) {
  if (Log) {
    std::cout << str << std::endl;
  }
}

void SmartmeterMqtt::LogCallbackWrapper(struct mosquitto *mosq, void *obj,
                                        int level, const char *str) {
  auto *p = reinterpret_cast<SmartmeterMqtt *>(obj);
  return p->SmartmeterMqtt::LogCallback(mosq, obj, level, str);
}
