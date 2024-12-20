#include "Smartmeter.h"
#include <charconv>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <set>
#include <thread>
#include <vector>

const int Smartmeter::ReceiveBufferSize = 368;
const std::set<std::string> Smartmeter::ValidKeys{
    "log_level",  "mqtt_broker", "mqtt_password", "mqtt_port",
    "mqtt_topic", "mqtt_user",   "mqtt_cafile",   "mqtt_capath",
    "basic_rate", "price_kwh",   "serial_device"};

Smartmeter::Smartmeter(void) {
  ReceiveBuffer = new char[Smartmeter::ReceiveBufferSize]();
  Serial = new SmartmeterSerial();
  Mqtt = new SmartmeterMqtt();
  Cfg = new SmartmeterConfig();
}

Smartmeter::~Smartmeter(void) {
  if (Mqtt->GetConnectStatus()) {
    Mqtt->PublishMessage("offline", Cfg->GetValue("mqtt_topic") + "/status", 1,
                         true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  if (Mqtt) {
    delete Mqtt;
  }
  if (Serial) {
    delete Serial;
  }
  if (ReceiveBuffer) {
    delete[] ReceiveBuffer;
  }
  if (Cfg) {
    delete Cfg;
  };
}

bool Smartmeter::Setup(const std::string &config) {
  if (!Cfg->Begin(config)) {
    ErrorMessage = Cfg->GetErrorMessage();
    return false;
  }
  if (!Cfg->ValidateKeys(Smartmeter::ValidKeys)) {
    ErrorMessage = Cfg->GetErrorMessage();
    return false;
  }
  this->SetLogLevel();
  if (Log & static_cast<unsigned char>(LogLevel::CONFIG)) {
    Cfg->ShowConfig();
  }
  if (!(Cfg->KeyExists("serial_device"))) {
    ErrorMessage = Cfg->GetErrorMessage();
    return false;
  }
  if (!(Cfg->KeyExists("mqtt_broker"))) {
    ErrorMessage = Cfg->GetErrorMessage();
    return false;
  }
  if (!(Cfg->KeyExists("mqtt_topic"))) {
    ErrorMessage = Cfg->GetErrorMessage();
    return false;
  }
  if (!(Cfg->KeyExists("price_kwh"))) {
    ErrorMessage = Cfg->GetErrorMessage();
    return false;
  }
  if (!(Cfg->KeyExists("basic_rate"))) {
    ErrorMessage = Cfg->GetErrorMessage();
    return false;
  }

  if (!Serial->Begin(Cfg->GetValue("serial_device"))) {
    ErrorMessage = Serial->GetErrorMessage();
    return false;
  }
  if (Log & static_cast<unsigned char>(LogLevel::SERIAL)) {
    Serial->SetDebug(true);
  }

  if (!Mqtt->Begin()) {
    ErrorMessage = Mqtt->GetErrorMessage();
    return false;
  }
  if (Log & static_cast<unsigned char>(LogLevel::MQTT)) {
    Mqtt->SetDebug(true);
  }
  if ((Cfg->KeyExists("mqtt_user") && Cfg->KeyExists("mqtt_password"))) {
    if (!Mqtt->SetUserPassAuth(Cfg->GetValue("mqtt_user"),
                               Cfg->GetValue("mqtt_password"))) {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
  }
  if (Cfg->KeyExists("mqtt_cafile") || Cfg->KeyExists("mqtt_capath")) {
    if (!Mqtt->SetTlsConnection(Cfg->GetValue("mqtt_cafile"),
                                Cfg->GetValue("mqtt_capath"))) {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
  }
  if (Cfg->KeyExists("mqtt_port")) {
    if (!Mqtt->Connect(Cfg->GetValue("mqtt_broker"),
                       StringTo<int>(Cfg->GetValue("mqtt_port")), 60)) {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
  } else {
    if (!Mqtt->Connect(Cfg->GetValue("mqtt_broker"), 1883, 60)) {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
  }
  if (!Mqtt->SetLastWillTestament(
          "offline", Cfg->GetValue("mqtt_topic") + "/status", 1, true)) {
    ErrorMessage = Mqtt->GetErrorMessage();
    return false;
  }

  return true;
}

bool Smartmeter::Receive(void) {
  memset(ReceiveBuffer, '\0', Smartmeter::ReceiveBufferSize);
  if (!Serial->ReadBytes(ReceiveBuffer, Smartmeter::ReceiveBufferSize)) {
    ErrorMessage = Serial->GetErrorMessage();
    Serial->Close();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!Serial->Begin(Cfg->GetValue("serial_device"))) {
      ErrorMessage = Serial->GetErrorMessage();
    }
    return false;
  }

  char *p = ReceiveBuffer;
  Datagram.SerialNum.assign(p + 0x1, 18);
  Datagram.CustomId.assign(p + 0x25, 14);
  Datagram.DeviceId.assign(p + 0x45, 14);
  Datagram.Energy.assign(p + 0x64, 15);
  Datagram.Power.assign(p + 0x89, 9);
  Datagram.PowerL1.assign(p + 0xA6, 9);
  Datagram.PowerL2.assign(p + 0xC3, 9);
  Datagram.PowerL3.assign(p + 0xE0, 9);
  Datagram.VoltageL1.assign(p + 0xFD, 5);
  Datagram.VoltageL2.assign(p + 0x116, 5);
  Datagram.VoltageL3.assign(p + 0x12F, 5);
  Datagram.Status.assign(p + 0x148, 8);
  std::from_chars(p + 0x162, p + 0x162 + 8, Datagram.SensorTime, 16);

  return true;
}

bool Smartmeter::Publish(void) {
  unsigned long long now =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();

  std::ios::fmtflags old_settings = Payload.flags();
  Payload.str(std::string());
  Payload.setf(std::ios::fixed, std::ios::floatfield);

  Payload << "[{" << "\"lifetime\":" << Datagram.SensorTime << ","
          << "\"energy\":" << RemoveLeading(Datagram.Energy, '0') << ","
          << "\"power\":" << RemoveLeading(Datagram.Power, '0') << ","
          << "\"power_l1\":" << RemoveLeading(Datagram.PowerL1, '0') << ","
          << "\"power_l2\":" << RemoveLeading(Datagram.PowerL2, '0') << ","
          << "\"power_l3\":" << RemoveLeading(Datagram.PowerL3, '0') << ","
          << "\"voltage_l1\":" << RemoveLeading(Datagram.VoltageL1, '0') << ","
          << "\"voltage_l2\":" << RemoveLeading(Datagram.VoltageL2, '0') << ","
          << "\"voltage_l3\":" << RemoveLeading(Datagram.VoltageL3, '0') << ","
          << "\"status\":\"" << Datagram.Status << "\"" << ","
          << "\"rate\":" << Cfg->GetValue("basic_rate") << ","
          << "\"price\":" << Cfg->GetValue("price_kwh") << ","
          << "\"time\":" << now << "},{" << "\"serial\":\""
          << Datagram.SerialNum << "\","
          << "\"custom_id\":\"" << Datagram.CustomId << "\","
          << "\"device_id\":\"" << Datagram.DeviceId << "\"" << "}]";

  if (Mqtt->GetNotifyOnlineFlag()) {
    std::cout << "Smartmeter is online." << std::endl;
    if (!Mqtt->PublishMessage("online", Cfg->GetValue("mqtt_topic") + "/status",
                              1, true)) {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
    Mqtt->SetNotifyOnlineFlag(false);
  }

  if (Mqtt->GetConnectStatus()) {
    if (!(Mqtt->PublishMessage(
            Payload.str(), Cfg->GetValue("mqtt_topic") + "/live", 0, false))) {
      ErrorMessage = Mqtt->GetErrorMessage();
      return false;
    }
  }

  if (Log & static_cast<unsigned char>(LogLevel::JSON)) {
    std::cout << Payload.str() << std::endl;
  }
  Payload.flags(old_settings);

  return true;
}

std::string Smartmeter::GetErrorMessage(void) const { return ErrorMessage; }

void Smartmeter::SetLogLevel(void) {
  if (Cfg->KeyExists("log_level")) {
    std::string line = Cfg->GetValue("log_level");
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> log_level;

    while (std::getline(iss, token, ',')) {
      log_level.push_back(token);
    }
    for (auto it = log_level.cbegin(); it != log_level.cend(); ++it) {
      if (!(*it).compare("config")) {
        Log |= static_cast<unsigned char>(LogLevel::CONFIG);
      } else if (!(*it).compare("json")) {
        Log |= static_cast<unsigned char>(LogLevel::JSON);
      } else if (!(*it).compare("mqtt")) {
        Log |= static_cast<unsigned char>(LogLevel::MQTT);
      } else if (!(*it).compare("serial")) {
        Log |= static_cast<unsigned char>(LogLevel::SERIAL);
      }
    }
  } else {
    Log = false;
  }
}

unsigned char Smartmeter::GetLogLevel(void) const { return Log; }

template <typename T_STR, typename T_CHAR>
T_STR Smartmeter::RemoveLeading(T_STR const &str, T_CHAR c) const {
  auto end = str.cend();
  for (auto i = str.cbegin(); i != end; ++i) {
    if (*i != c) {
      return T_STR(i, end);
    }
  }
  return T_STR();
}

template <typename T> T Smartmeter::StringTo(const std::string &str) const {
  T value;
  std::istringstream iss(str);
  iss >> value;
  if (iss.fail()) {
    return T();
  }
  return value;
}
