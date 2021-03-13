#include <iostream>
#include <unistd.h>
#include "ebz.hpp"

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
  m_sensortime = 0;
  m_now = 0;
  m_mqtt = nullptr;
  m_rate = 0;
  m_price = 0;
  m_datagram = new char[Ebz::SERIAL_BUFFER_SIZE];
  m_serialnum = new char[Ebz::OBIS_BUFFER_SIZE];
  m_customid = new char[Ebz::OBIS_BUFFER_SIZE];
  m_deviceid = new char[Ebz::OBIS_BUFFER_SIZE];
  m_status = new char[Ebz::OBIS_BUFFER_SIZE];
}

Ebz::~Ebz(void)
{
  if (m_serialport > 0) {
    close(m_serialport);
    
    if (m_debug) {
      std::cout << "Serial port closed" << std::endl;
    }
  }

  if (m_debug) {
    std::cout << "Ebz destructor method called" << std::endl;
  }
  delete[] m_datagram;
  delete[] m_customid;
  delete[] m_deviceid;
  delete[] m_serialnum;
  delete[] m_status;
  delete m_mqtt;
}

void Ebz::setDebug(void)
{
  m_debug = true;
}

void Ebz::setTariff(double const& t_rate, double const& t_price)
{
  if (t_rate < 0) {
    throw std::runtime_error("Basic rate must be greater than zero");
  }
  if (t_price < 0) {
    throw std::runtime_error("Price per kWh must be greater than zero");
  } 
  m_rate = t_rate;
  m_price = t_price;
}
