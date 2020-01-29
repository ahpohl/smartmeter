#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>

#include <cstring>
#include <termios.h>    // contains POSIX terminal control definition
#include <fcntl.h>      // contains file controls like 0_RDWR
#include <unistd.h>     // write(), read(), close()
#include <errno.h>      // error integer and strerror() function
#include <sys/ioctl.h>  // contains ioctl_tty

#include "ebz.hpp"

using namespace std;

void Ebz::openSerialPort(char const* t_device)
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

void Ebz::readSerialPort()
{
  char byte = '\0';
  int bytes_received = 0;
  int count = 0;
  memset(m_datagram, '\0', Ebz::D0_DATAGRAM_SIZE);
  char *p = m_datagram;

  do {
    bytes_received = read(m_serialport, &byte, 1);
    if (bytes_received == -1) {
      throw runtime_error(string("Error reading serial port: ")
        + strerror(errno) + " (" + to_string(errno) + ")");
    }
    ++count;
    if (count > Ebz::D0_DATAGRAM_SIZE) {
      throw runtime_error("Serial buffer overflow");
    }
    *p = byte;
    ++p;
  } while (byte != '!');

  if (count < Ebz::D0_DATAGRAM_SIZE) {
    if (m_debug) {
      cout << "Incomplete message received (" << count << ")" << endl;
    }
    return;
  }
  if (m_debug) {
    cout << m_datagram << endl;
    cout << "EOM " << endl;
  }
}
