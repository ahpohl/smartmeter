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
  m_mqtt = nullptr;
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
