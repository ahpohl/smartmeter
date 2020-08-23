#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>
#include <mutex>
#include "ebz.hpp"
#include "mosq.hpp"

void Ebz::initMqtt(char const* t_host, int t_port, char const* t_topic)
{
  if (!t_host) {
    throw std::runtime_error("MQTT broker host argument empty");
  }
  if (!t_port) {
    throw std::runtime_error("MQTT broker port argument empty");
  }
  m_mqtt = new Mosq(t_host, t_port, m_debug ? true : false);
  m_topic = std::string(t_topic);
}

void Ebz::publishMqtt(void) const
{
  std::mutex mutex;
  std::lock_guard<std::mutex> guard(mutex);
  std::string topic = m_topic + "/state";
  std::stringstream payload;

  // json string with influxdb fields and tags 
  payload << "[{"
    << "\"lifetime\":" << m_sensortime << "," 
    << "\"energy\":" << std::fixed << std::setprecision(3) << m_energy << ","
    << "\"power\":" << std::setprecision(2) << m_power << ","
    << "\"power_l1\":" << m_powerl1 << ","
    << "\"power_l2\":" << m_powerl2 << ","
    << "\"power_l3\":" << m_powerl3 << ","
    << "\"voltage_l1\":" << std::setprecision(1) << m_voltagel1 << ","
    << "\"voltage_l2\":" << m_voltagel2 << ","
    << "\"voltage_l3\":" << m_voltagel3
    << "},{"
    << "\"serial\":\"" << m_serialnum << "\","
    << "\"custom_id\":\"" << m_customid << "\","
    << "\"device_id\":\"" << m_deviceid << "\","
    << "\"status\":\"" << m_status << "\","
    << "\"basic_rate\":" << std::setprecision(2) << m_rate << ","
    << "\"price_kwh\":" << std::setprecision(4) << m_price
    << "}]";

  m_mqtt->send_message(topic.c_str(), payload.str().c_str());
  if (m_debug) { 
    std::cout << payload.str() << std::endl;
  }
}

void Ebz::runMqtt(void) const
{
  std::this_thread::sleep_for(std::chrono::seconds(1));
  while (true) {
    publishMqtt();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
