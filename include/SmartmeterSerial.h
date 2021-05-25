#ifndef SmartmeterSerial_h
#define SmartmeterSerial_h

class SmartmeterSerial
{
static const unsigned char BufferSize;

private:
  int SerialPort;
  std::string ErrorMessage;
  char GetByte(void);	

public:
  ~SmartmeterSerial(void);
  bool Begin(const std::string &device);
  bool ReadBytes(char *buffer, const int &length);
  std::string GetErrorMessage(void);
};

#endif
