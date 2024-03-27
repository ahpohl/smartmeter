#ifndef SmartmeterSerial_h
#define SmartmeterSerial_h

class SmartmeterSerial
{
	static const unsigned char BufferSize;

public:
	SmartmeterSerial(void);
	~SmartmeterSerial(void);
	void SetDebug(const bool &debug);
	bool Begin(const std::string &device);
	bool ReadBytes(char *buffer, const int &length);
	std::string GetErrorMessage(void);

private:
	int SerialPort;
	std::string ErrorMessage;
	bool Log;
};

#endif
