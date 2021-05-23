#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include "SmartmeterSerial.h"

const unsigned char SmartmeterSerial::BufferSize = 255;

SmartmeterSerial::~SmartmeterSerial(void)
{
  if (SerialPort > 0) {
    close(SerialPort);
  }
}

bool SmartmeterSerial::Begin(const std::string &device)
{
  if (device.empty()) {
    ErrorMessage = "Serial device argument empty";
    return false;
  }
  if ((SerialPort = open(device.c_str(), (O_RDONLY | O_NOCTTY))) < 0)
  {
    ErrorMessage = std::string("Error opening serial device: ") 
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }
  if(!isatty(SerialPort))
  {
    ErrorMessage = std::string("Error: Device ") + device + " is not a tty.";
    return false;
  }
  if (flock(SerialPort, LOCK_EX | LOCK_NB) < 0)
  {
    ErrorMessage = std::string("Error locking serial device: ")
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }
  if (ioctl(SerialPort, TIOCEXCL) < 0)
  {
    ErrorMessage = std::string("Error setting exclusive access: ") 
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }

  struct termios serial_port_settings;

  memset(&serial_port_settings, 0, sizeof(serial_port_settings));
  if (tcgetattr(SerialPort, &serial_port_settings))
  {
    ErrorMessage = std::string("Error getting serial port attributes: ")
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }

  cfmakeraw(&serial_port_settings);

  // configure serial port
  // speed: 9600 baud, data bits: 7, stop bits: 1, parity: even
  cfsetispeed(&serial_port_settings, B9600);
  cfsetospeed(&serial_port_settings, B9600);
  serial_port_settings.c_cflag |= (CLOCAL | CREAD);
  serial_port_settings.c_cflag &= ~CSIZE;
  serial_port_settings.c_cflag |= (CS7 | PARENB);
  
  // vmin: read() returns when x byte(s) are available
  // vtime: wait for up to x * 0.1 second between characters
  serial_port_settings.c_cc[VMIN] = SmartmeterSerial::BufferSize;
  serial_port_settings.c_cc[VTIME] = 5;

  if (tcsetattr(SerialPort, TCSANOW, &serial_port_settings))
  {
    ErrorMessage = std::string("Error setting serial port attributes: ") 
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }
  tcflush(SerialPort, TCIOFLUSH);

  return true;
}

char SmartmeterSerial::GetByte(void)
{
  static char buffer[SmartmeterSerial::BufferSize] = {0};
  static char *p = buffer;
  static int count = 0;   

  if ((p - buffer) >= count)
  {
    if ((count = read(SerialPort, buffer, SmartmeterSerial::BufferSize)) < 0)
    {
      ErrorMessage = std::string("Read on serial device failed: ")
        + strerror(errno) + " (" + std::to_string(errno) + ")";
      return false;
    }
    p = buffer;
  }
  return *p++;
}

bool SmartmeterSerial::ReadBytes(char *buffer, const int &length)
{
  int bytes_received = 0;
  char *p = buffer;
  bool message_begin = false;
  
  tcflush(SerialPort, TCIOFLUSH);
  
  while (bytes_received < length)
  {
    if ((*p = GetByte()) == '/')
    {
      message_begin = true;
    }
    if (message_begin)
    {
      ++p;
      ++bytes_received;
    }
  }
  if (*(p-3) != '!')
  {
    ErrorMessage = "Serial datagram stream not in sync.";
    return false;
  }
  return true;
}

std::string SmartmeterSerial::GetErrorMessage(void)
{
  return ErrorMessage;
}
