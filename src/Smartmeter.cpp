#include <cstring>
#include <iostream>
#include <sstream>
#include <charconv>
#include <chrono>
#include <thread>
#include "Smartmeter.h"

const int Smartmeter::ReceiveBufferSize = 368; 

Smartmeter::Smartmeter(const bool &log): Log(log)
{
  ReceiveBuffer = new char[Smartmeter::ReceiveBufferSize] ();
  Serial = new SmartmeterSerial();
  Mqtt = new SmartmeterMqtt(Log);
}

Smartmeter::~Smartmeter(void)
{
  if (Mqtt->GetConnectStatus())
  {
    Mqtt->PublishMessage("offline", Topic + "/status", 1, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  if (Mqtt) { delete Mqtt; }
  if (Serial) { delete Serial; }
  if (ReceiveBuffer) { delete[] ReceiveBuffer; }
}

bool Smartmeter::SetTopic(const std::string &topic)
{
  if (topic.empty())
  {
    ErrorMessage = "Smartmeter error: Topic argument empty.";
    return false;
  }
  Topic = topic;
  return true;
}

bool Smartmeter::SetUserPass(const std::string &user, const std::string &pass)
{
  if (!(user.empty()) && pass.empty())
  {
    ErrorMessage = "Smartmeter error: Username without a password.";
    return false;
  }
  if (user.empty() && !(pass.empty()))
  {
    ErrorMessage = "Smartmeter error: Password without a username.";
    return false;
  }
  Username = user;
  Password = pass;

  return true;
}

void Smartmeter::SetCaFile(const std::string &cafile)
{
  CaFile = cafile;
}

bool Smartmeter::Setup(const std::string &device, const std::string &host, const int &port)
{
  if (!Serial->Begin(device))
  {
    ErrorMessage = Serial->GetErrorMessage();
    return false;
  }
  if (!Mqtt->Begin())
  {
    ErrorMessage = Mqtt->GetErrorMessage();
    return false;
  }
  if (!Mqtt->SetLastWillTestament("offline", Topic + "/status", 1, true))
  {
    ErrorMessage = Mqtt->GetErrorMessage();
    return false;
  }
  if (!(Username.empty()) || !(Password.empty()))
  {
    if (!Mqtt->SetUserPassAuth(Username, Password))
    {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
  }
  if (!(CaFile.empty()))
  {
    if (!Mqtt->SetTls(CaFile))
    {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
  }
  if (!Mqtt->Connect(host, port, 60))
  {
    ErrorMessage = Mqtt->GetErrorMessage();
    return false;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  if (Mqtt->GetConnectStatus())
  {
    std::cout << "Smartmeter is online." << std::endl;
  }
  if (!Mqtt->PublishMessage("online", Topic + "/status", 1, true))
  {
    ErrorMessage = Mqtt->GetErrorMessage();
    return false;
  }
  return true;
}

bool Smartmeter::Receive(void)
{
  memset(ReceiveBuffer, '\0', Smartmeter::ReceiveBufferSize);  
  if (!Serial->ReadBytes(ReceiveBuffer, Smartmeter::ReceiveBufferSize)) 
  {
    ErrorMessage = Serial->GetErrorMessage();
    return false;
  }

  char *p = ReceiveBuffer;
  Datagram.SerialNum.assign(p+0x1, 18);
  Datagram.CustomId.assign(p+0x25, 14);
  Datagram.DeviceId.assign(p+0x45, 14);
  Datagram.Energy.assign(p+0x64, 15);
  Datagram.Power.assign(p+0x89, 9);
  Datagram.PowerL1.assign(p+0xA6, 9);
  Datagram.PowerL2.assign(p+0xC3, 9);
  Datagram.PowerL3.assign(p+0xE0, 9);
  Datagram.VoltageL1.assign(p+0xFD, 5);
  Datagram.VoltageL2.assign(p+0x116, 5);
  Datagram.VoltageL3.assign(p+0x12F, 5);
  Datagram.Status.assign(p+0x148, 8);
  std::from_chars(p+0x162, p+0x162+8, Datagram.SensorTime, 16);

  return true;
}

bool Smartmeter::Publish(void)
{
  unsigned long long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  Payload.str(std::string());

  Payload << "[{"
    << "\"lifetime\":" << Datagram.SensorTime << "," 
    << "\"energy\":" << RemoveLeading(Datagram.Energy, '0') << ","
    << "\"power\":" << RemoveLeading(Datagram.Power, '0') << ","
    << "\"power_l1\":" << RemoveLeading(Datagram.PowerL1, '0') << ","
    << "\"power_l2\":" << RemoveLeading(Datagram.PowerL2, '0') << ","
    << "\"power_l3\":" << RemoveLeading(Datagram.PowerL3, '0') << ","
    << "\"voltage_l1\":" << RemoveLeading(Datagram.VoltageL1, '0') << ","
    << "\"voltage_l2\":" << RemoveLeading(Datagram.VoltageL2, '0') << ","
    << "\"voltage_l3\":" << RemoveLeading(Datagram.VoltageL3, '0') << ","
    << "\"status\":\"" << Datagram.Status << "\"" << ","
    << "\"rate\":" << BasicRate << ","
    << "\"price\":" << PricePerKwh << ","
    << "\"time\":" << now
    << "},{"
    << "\"serial\":\"" << Datagram.SerialNum << "\","
    << "\"custom_id\":\"" << Datagram.CustomId << "\","
    << "\"device_id\":\"" << Datagram.DeviceId << "\""
    << "}]";

  static bool last_connect_status = true;
  if (Mqtt->GetConnectStatus())
  {
    if (!(Mqtt->PublishMessage(Payload.str(), Topic + "/state", 0, false)))
    {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
    if (!last_connect_status)
    {
      if (!(Mqtt->PublishMessage("online", Topic + "/status", 1, true)))
      {
        ErrorMessage = Mqtt->GetErrorMessage();
        return false;
      }
      std::cout << "Smartmeter is online." << std::endl;
    }
  }
  last_connect_status = Mqtt->GetConnectStatus();
 
  return true;
}

bool Smartmeter::SetEnergyPlan(double const& basic_rate, double const& price_per_kwh)
{
  if (basic_rate < 0) {
     ErrorMessage = "Smartmeter error: Basic rate per month < 0";
     return false;
  }
  if (price_per_kwh < 0) {
     ErrorMessage = "Smartmeter error: Price per kWh < 0";
     return false;
  } 
  BasicRate = basic_rate;
  PricePerKwh = price_per_kwh;

  return true;
}

std::string Smartmeter::GetErrorMessage(void) const
{
  return ErrorMessage;
}

std::string Smartmeter::GetReceiveBuffer(void) const
{
  return ReceiveBuffer;
}

std::string Smartmeter::GetPayload(void) const
{
  return Payload.str();
}

template <typename T_STR, typename T_CHAR>
T_STR Smartmeter::RemoveLeading(T_STR const &str, T_CHAR c)
{
  auto end = str.cend();
  for (auto i = str.cbegin(); i != end; ++i)
  {
    if (*i != c)
    {
      return T_STR(i, end);
    }
  }
  return T_STR();
}
