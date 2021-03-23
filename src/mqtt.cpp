#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
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
  std::string topic = m_topic + "/state";
  std::stringstream payload;

  // json string with influxdb fields and tags 
  payload << "[{"
    << "\"lifetime\":" << m_sensortime << "," 
    << "\"energy\":" << std::fixed << m_energy << ","
    << "\"power\":" << std::setprecision(2) << m_power << ","
    << "\"power_l1\":" << m_powerl1 << ","
    << "\"power_l2\":" << m_powerl2 << ","
    << "\"power_l3\":" << m_powerl3 << ","
    << "\"voltage_l1\":" << std::setprecision(1) << m_voltagel1 << ","
    << "\"voltage_l2\":" << m_voltagel2 << ","
    << "\"voltage_l3\":" << m_voltagel3 << ","
    << "\"rate\":" << std::setprecision(2) << m_rate << ","
    << "\"price\":" << std::setprecision(4) << m_price << ","
    << "\"status\":\"" << m_status << "\"" << ","
    << "\"time\":" << m_now
    << "},{"
    << "\"serial\":\"" << m_serialnum << "\","
    << "\"custom_id\":\"" << m_customid << "\","
    << "\"device_id\":\"" << m_deviceid << "\""
    << "}]";

  m_mqtt->send_message(topic.c_str(), payload.str().c_str());
  if (m_debug) { 
    std::cout << payload.str() << std::endl;
  }
}
