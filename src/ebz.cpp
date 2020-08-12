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
#include "mqtt.hpp"

using namespace std;

int const Ebz::SERIAL_BUFFER_SIZE = 512;
int const Ebz::OBIS_BUFFER_SIZE = 32;

Ebz::Ebz(void)
{
  m_debug = false;
  m_serialport = 0;
  m_energy = 0;      
  m_power = 0;      
  m_powerl1 = 0;  
  m_powerl2 = 0; 
  m_powerl3 = 0;
  m_voltagel1 = 0;    
  m_voltagel2 = 0; 
  m_voltagel3 = 0;
  m_mqtt = nullptr;
}

Ebz::~Ebz(void)
{
  if (m_serialport > 0) {
    close(m_serialport);
    
    if (m_debug) {
      cout << "Serial port closed" << endl;
    }
  }

  if (m_debug) {
    cout << "Ebz destructor method called" << endl;
  }
  delete[] m_datagram;
  delete[] m_customid;
  delete[] m_deviceid;
  delete[] m_serialnum;
  delete[] m_status;
  delete[] m_sensortime;
  delete m_mqtt;
}

void Ebz::setDebug(void)
{
  m_debug = true;
}

void Ebz::initMqtt(char const* t_host, int t_port, char const* t_topic)
{
  if (!t_host) {
    throw runtime_error("MQTT broker host argument empty");
  }
  if (!t_port) {
    throw runtime_error("MQTT broker port argument empty");
  }
  m_mqtt = new Mosq(t_host, t_port);
}

void Ebz::publishMqtt(void) const
{
  stringstream payload;
  std::mutex mutex;
  std::lock_guard<std::mutex> guard(mutex);

  payload << "{Serial: " << m_serialnum
  << ", Custom_ID: " << m_customid
  << ", Device_ID: " << m_deviceid
  << ", Status: " << m_status 
  << ", {Lifetime: " << strtoul(m_sensortime, 0, 16) << ", Unit: s}"
  << ", {Energy: " << fixed << setprecision(2) << m_energy << ", Unit: kWh}"
  << ", {Power_Total: " << m_power << ", Unit: W}"
  << ", {Power_L1: " << m_powerl1 << ", Unit: W}"
  << ", {Power_L2: " << m_powerl2 << ", Unit: W}"
  << ", {Power_L3: " << m_powerl3 << ", Unit: W}"
  << ", {Voltage_L1: " << setprecision(1) << m_voltagel1 << ", Unit: V}"
  << ", {Voltage_L2: " << m_voltagel2 << ", Unit: V}"
  << ", {Voltage_L3: " << m_voltagel3 << ", Unit: V}}";
  m_mqtt->send_message("smartmeter/json/state", payload.str().c_str());
}

void Ebz::runReadSerial(void)
{
  m_datagram = new char[Ebz::SERIAL_BUFFER_SIZE];
  m_serialnum = new char[Ebz::OBIS_BUFFER_SIZE];
  m_customid = new char[Ebz::OBIS_BUFFER_SIZE];
  m_deviceid = new char[Ebz::OBIS_BUFFER_SIZE];
  m_status = new char[Ebz::OBIS_BUFFER_SIZE];
  m_sensortime = new char[Ebz::OBIS_BUFFER_SIZE];
  int ret = 0; 
 
  while (true) {
    ret = readSerialPort();
    if (!ret) {
      readDatagram();
    }
  }
}

void Ebz::runWriteSharedMem(void) const
{
  while (true) {
    writeObisCodes();
    publishMqtt();
    this_thread::sleep_for(chrono::seconds(1));
  }
}
