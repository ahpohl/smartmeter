#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "ebz.hpp"

using namespace std;

int const Ebz::SERIAL_BUFFER_SIZE = 512;
int const Ebz::OBIS_SERIAL_NUMBER_SIZE = 32;
int const Ebz::OBIS_DEVICE_ID_SIZE = 32;

Ebz::Ebz(void)
{
  bool m_debug = false;
  char const* m_sharedmem = nullptr;      
  int m_serialport = 0;         
  char const* m_serialnum = nullptr; 
  char const* m_deviceid = nullptr;
  double m_energy = 0;      
  double m_power = 0;      
  double m_powerl1 = 0;  
  double m_powerl2 = 0; 
  double m_powerl3 = 0;
  double m_voltagel1 = 0;    
  double m_voltagel2 = 0; 
  double m_voltagel3 = 0;
  char m_status[4] = "\0";
  char m_secindex[4] = "\0";  
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

  free(m_datagram);
}

void Ebz::setDebug(void)
{
  m_debug = true;
}

void Ebz::runEbz(void)
{
  m_datagram = (char*)malloc(Ebz::SERIAL_BUFFER_SIZE * sizeof(char));
  m_serialnum = (char*)malloc(Ebz::OBIS_SERIAL_NUMBER_SIZE * sizeof(char));
  m_deviceid = (char*)malloc(Ebz::OBIS_DEVICE_ID_SIZE * sizeof(char));
  
  while (true) {
    readSerialPort();
    readDatagram();
  }
}

void Ebz::runSharedMem(void) const
{
  while (true) {
    writeSharedMem();
    this_thread::sleep_for(chrono::seconds(1));
  }
}
