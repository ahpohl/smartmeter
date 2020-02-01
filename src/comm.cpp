#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>

#include <cstring>
#include <termios.h>    // contains POSIX terminal control definition
#include <fcntl.h>      // contains file controls like 0_RDWR
#include <unistd.h>     // write(), read(), close()
#include <errno.h>      // error integer and strerror() function
#include <sys/ioctl.h>  // contains ioctl_tty

#include "ebz.hpp"

using namespace std;

int const Ebz::D0_DATAGRAM_SIZE = 368;

void Ebz::openSerialPort(const char* const t_device)
{
  if (!t_device) {
    throw runtime_error("Serial device argument empty");
  }
 
  m_serialport = open(t_device, O_RDWR | O_NOCTTY);
  if (m_serialport < 0) {
    throw runtime_error(string("Error opening device ") + t_device + ": "
      + strerror(errno) + " (" + to_string(errno) + ")");
  }

  int ret = ioctl(m_serialport, TIOCEXCL);
  if (ret < 0) {
    throw runtime_error(string("Error getting device lock on") 
      + t_device + ": " + strerror(errno) + " (" + to_string(errno) + ")");
  }

  if (m_debug) {
    cout << "Opened serial device " << t_device << endl;
  }

  // set vmin and vtime for blocking read
  // vmin: returning when max 1 byte is available
  // vtime: wait for up to 0.1 second between characters
  configureSerialPort(1, 1);
}

void Ebz::configureSerialPort(unsigned char const& t_vmin, 
  unsigned char const& t_vtime) const
{
  struct termios attr;

  memset(&attr, 0, sizeof(attr));
  int ret = tcgetattr(m_serialport, &attr);
  if (ret) {
    throw runtime_error(string("Error getting serial port attributes: ") 
      + strerror(errno) + " (" + to_string(errno) + ")");
  }

  cfmakeraw(&attr);
  attr.c_cflag &= ~CSIZE;
  attr.c_cflag |= CS7;

  attr.c_cc[VMIN] = t_vmin;
  attr.c_cc[VTIME] = t_vtime;
  cfsetispeed(&attr, B9600);
  cfsetospeed(&attr, B9600);

  ret = tcsetattr(m_serialport, TCSANOW, &attr);
  if (ret != 0) {
    throw runtime_error(string("Error setting serial port attributes: ") 
      + strerror(errno) + " (" + to_string(errno) + ")");
  }
}

int Ebz::readSerialPort(void)
{
  char byte = '\0';
  int bytes_received = 0;
  int count = 0;
  memset(m_datagram, '\0', Ebz::SERIAL_BUFFER_SIZE);
  char *p = m_datagram;

  do {
    bytes_received = read(m_serialport, &byte, 1);
    if (bytes_received == -1) {
      throw runtime_error(string("Error reading serial port: ")
        + strerror(errno) + " (" + to_string(errno) + ")");
    }
    ++count;
    if (count == Ebz::SERIAL_BUFFER_SIZE) {
      throw runtime_error("Serial buffer overflow");
    }
    *p = byte;
    ++p;
  } while (byte != '!');

  if (count != Ebz::D0_DATAGRAM_SIZE) {
    if (m_debug) {
      cout << "Incomplete datagram (" << count << ")" << endl;
    }
    return EXIT_FAILURE;
  }
  if (m_debug) {
    cout << m_datagram << endl;
    cout << "EOM " << endl;
  }

  return EXIT_SUCCESS;
}

void Ebz::readDatagram(void)
{
  std::mutex mutex;
  std::lock_guard<std::mutex> guard(mutex);

  char *p = m_datagram;
  memset(m_serialnum, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(m_serialnum, p+=0x03, 18);
  memset(m_customid, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(m_customid, p+=0x24, 14);
  memset(m_deviceid, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(m_deviceid, p+=0x20, 14);
  char tmp[Ebz::OBIS_BUFFER_SIZE] = {0};
  strncpy(tmp, p+=0x1F, 15);
  m_energy = atof(tmp);
  memset(tmp, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(tmp, p+=0x25, 9);
  m_power = atof(tmp);
  memset(tmp, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(tmp, p+=0x1D, 9);
  m_powerl1 = atof(tmp);
  memset(tmp, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(tmp, p+=0x1D, 9);
  m_powerl2 = atof(tmp);
  memset(tmp, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(tmp, p+=0x1D, 9);
  m_powerl3 = atof(tmp);
  memset(tmp, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(tmp, p+=0x1D, 5);
  m_voltagel1 = atof(tmp);
  memset(tmp, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(tmp, p+=0x19, 5);
  m_voltagel2 = atof(tmp);
  memset(tmp, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(tmp, p+=0x19, 5);
  m_voltagel3 = atof(tmp);
  memset(m_status, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(m_status, p+=0x19, 8);
  memset(m_sensortime, '\0', Ebz::OBIS_BUFFER_SIZE);
  strncpy(m_sensortime, p+=0x1A, 8);
  
  if (m_debug) {
    cout << "Serial number: " << m_serialnum << endl;
    cout << "Custom id: " << m_customid << endl;
    cout << "Device id: " << m_deviceid << endl;
    cout << "Energy: " << m_energy << " kWh" << endl;
    cout << "Total power: " << m_power << " W" << endl;
    cout << "Power L1: " << m_powerl1 << " W" << endl;
    cout << "Power L2: " << m_powerl2 << " W" << endl;
    cout << "Power L3: " << m_powerl3 << " W" << endl;
    cout << "Voltage L1: " << m_voltagel1 << " V" << endl;
    cout << "Voltage L2: " << m_voltagel2 << " V" << endl;
    cout << "Voltage L3: " << m_voltagel3 << " V" << endl;
    cout << "Status: " << m_status << endl;
    unsigned long secindex = strtoul(m_sensortime, 0, 16);
    cout << "Sensor time: " << m_sensortime << " (" << 
      secindex << " sec)" << endl;
  }
}
