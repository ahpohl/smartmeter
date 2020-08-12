#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <unistd.h>
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
  m_mqtt = new Mosq(t_host, t_port);
  if (m_debug) {
    m_mqtt->set_debug();
  }
}

void Ebz::publishMqtt(void) const
{
  std::stringstream payload;
  std::mutex mutex;
  std::lock_guard<std::mutex> guard(mutex);

  payload << "{Serial: " << m_serialnum
  << ", Custom_ID: " << m_customid
  << ", Device_ID: " << m_deviceid
  << ", Status: " << m_status 
  << ", {Lifetime: " << strtoul(m_sensortime, 0, 16) << ", Unit: s}"
  << ", {Energy: " << std::fixed << std::setprecision(2) << m_energy << ", Unit: kWh}"
  << ", {Power_Total: " << m_power << ", Unit: W}"
  << ", {Power_L1: " << m_powerl1 << ", Unit: W}"
  << ", {Power_L2: " << m_powerl2 << ", Unit: W}"
  << ", {Power_L3: " << m_powerl3 << ", Unit: W}"
  << ", {Voltage_L1: " << std::setprecision(1) << m_voltagel1 << ", Unit: V}"
  << ", {Voltage_L2: " << m_voltagel2 << ", Unit: V}"
  << ", {Voltage_L3: " << m_voltagel3 << ", Unit: V}}";
  m_mqtt->send_message("smartmeter/json/state", payload.str().c_str());
}

void Ebz::runMqtt(void) const
{
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    publishMqtt();
  }
}
