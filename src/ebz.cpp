#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "ebz.hpp"

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
}

void Ebz::setDebug(void)
{
  m_debug = true;
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
    this_thread::sleep_for(chrono::seconds(1));
  }
}

void Ebz::setSharedMemoryDevice(char const* t_ramdisk)
{
  m_sharedmem = t_ramdisk;
}
