#include <iostream>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
#include "SmartmeterSerial.h"

const unsigned char SmartmeterSerial::BufferSize = 64;

SmartmeterSerial::SmartmeterSerial(void) : SerialPort(0), Log(false)
{
}

SmartmeterSerial::~SmartmeterSerial(void)
{
	if (SerialPort > 0) {
		close(SerialPort);
	}
}

void SmartmeterSerial::SetDebug(const bool &debug)
{
	Log = debug;
}

bool SmartmeterSerial::Begin(const std::string &device)
{
	if (device.empty()) {
		ErrorMessage = "Serial error: Device argument empty.";
		return false;
	}
	if ((SerialPort = open(device.c_str(), (O_RDONLY | O_NOCTTY))) < 0)
	{
		ErrorMessage = std::string("Opening serial device failed: ")
    		  + strerror(errno) + " (" + std::to_string(errno) + ")";
		return false;
	}
	if(!isatty(SerialPort))
	{
		ErrorMessage = std::string("Serial error: Device ") + device + " is not a tty.";
		return false;
	}
	if (flock(SerialPort, LOCK_EX | LOCK_NB) < 0)
	{
		ErrorMessage = std::string("Locking serial device failed: ")
    		  + strerror(errno) + " (" + std::to_string(errno) + ")";
		return false;
	}
	if (ioctl(SerialPort, TIOCEXCL) < 0)
	{
		ErrorMessage = std::string("Setting exclusive access faild: ")
    		  + strerror(errno) + " (" + std::to_string(errno) + ")";
		return false;
	}

	struct termios serial_port_settings;

	memset(&serial_port_settings, 0, sizeof(serial_port_settings));
	if (tcgetattr(SerialPort, &serial_port_settings))
	{
		ErrorMessage = std::string("Getting serial port attributes failed: ")
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
		ErrorMessage = std::string("Setting serial port attributes failed: ")
    		  + strerror(errno) + " (" + std::to_string(errno) + ")";
		return false;
	}
	tcflush(SerialPort, TCIOFLUSH);

	return true;
}

bool SmartmeterSerial::ReadBytes(char *packet, const int &length)
{
	char buffer[SmartmeterSerial::BufferSize] = {0};
	char *b = buffer;
	int bytes_received = 0;

	int bytes_processed = 0;
	char *p = packet;
	bool message_begin = false;

	while (bytes_processed < length)
	{
		if ((b - buffer) >= bytes_received)
		{
			memset(buffer, '\0', SmartmeterSerial::BufferSize);
			bytes_received = read(SerialPort, buffer, SmartmeterSerial::BufferSize);
			if (bytes_received < 0)
			{
				ErrorMessage = std::string("Reading serial device failed: ")
					+ strerror(errno) + " (" + std::to_string(errno) + ")";
			    return false;
			}
			else if (bytes_received == 0)
			{
				ErrorMessage = "Serial error: serial buffer empty.";
				std::this_thread::sleep_for(std::chrono::seconds(1));
				return false;
			}
			else
			{
				b = buffer;
				std::cout << "bytes_received (" << bytes_received << ")" << std::endl;
			}
		}
		memcpy(p, b, 1);
		++b;
		if (*p == '/')
		{
			message_begin = true;
		}
		if (message_begin)
		{
			++p;
			++bytes_processed;
		}
	}
	if (*(p-3) != '!')
	{
		ErrorMessage = "Serial error: packet stream not in sync.";
		return false;
	}
	if (Log)
	{
		std::cout << packet;
	}

	return true;
}

std::string SmartmeterSerial::GetErrorMessage(void)
{
	return ErrorMessage;
}
