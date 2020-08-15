#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <mutex>
#include "ebz.hpp"

namespace fs = std::filesystem;

const char* const Ebz::OBIS_CUSTOM_ID =       "1-0:0.0.0";
const char* const Ebz::OBIS_DEVICE_ID =       "1-0:96.1.0";
const char* const Ebz::OBIS_ENERGY =          "1-0:1.8.0";
const char* const Ebz::OBIS_POWER_TOTAL =     "1-0:16.7.0";
const char* const Ebz::OBIS_POWER_L1 =        "1-0:36.7.0";
const char* const Ebz::OBIS_POWER_L2 =        "1-0:56.7.0";
const char* const Ebz::OBIS_POWER_L3 =        "1-0:76.7.0";
const char* const Ebz::OBIS_VOLTAGE_L1 =      "1-0:32.7.0";
const char* const Ebz::OBIS_VOLTAGE_L2 =      "1-0:52.7.0";
const char* const Ebz::OBIS_VOLTAGE_L3 =      "1-0:72.7.0";
const char* const Ebz::OBIS_STATUS =          "1-0:96.5.0";
const char* const Ebz::OBIS_SECONDS_INDEX =   "0-0:96.8.0";

void Ebz::createObisPath(const char* const t_ramdisk) const
{
  fs::path dir(t_ramdisk);
  dir /= "obis";
  if (!fs::exists(dir)) {
    fs::create_directories(dir);
    if (m_debug) {
      std::cout << "Created directory " << dir << std::endl;
    }
  }
  if (fs::is_directory(dir)) {
    fs::current_path(dir);
  } else {
    throw std::runtime_error(std::string("Path '") + dir.string() 
      + "' is not a directory"); 
  }
}

void Ebz::writeObisCodes(void) const
{
  std::mutex mutex;
  std::lock_guard<std::mutex> guard(mutex);

  std::ofstream ofs;
  ofs.open("ebz_serial", std::ios::out);
  ofs << m_serialnum;
  ofs.close();
  ofs.open(Ebz::OBIS_CUSTOM_ID, std::ios::out);
  ofs << m_customid;
  ofs.close();
  ofs.open(Ebz::OBIS_DEVICE_ID, std::ios::out);
  ofs << m_deviceid;
  ofs.close();
  ofs.open(Ebz::OBIS_STATUS, std::ios::out);
  ofs << m_status;
  ofs.close();
  ofs.open(Ebz::OBIS_SECONDS_INDEX, std::ios::out);
  ofs << m_sensortime;
  ofs.close();
  ofs.open("ebz_lifetime", std::ios::out);
  ofs << "lifetime" << "(" << m_sensortime / 86400.0 << "*d)";
  ofs.close();

  ofs.open(Ebz::OBIS_ENERGY, std::ios::out);
  ofs << Ebz::OBIS_ENERGY << "(" << std::fixed << std::setprecision(2) << m_energy*1000 << "*Wh)";
  ofs.close();
  ofs.open(Ebz::OBIS_POWER_TOTAL, std::ios::out);
  ofs << Ebz::OBIS_POWER_TOTAL << "(" << m_power << "*W)";
  ofs.close();
  ofs.open(Ebz::OBIS_POWER_L1, std::ios::out);
  ofs << Ebz::OBIS_POWER_L1 << "(" << m_powerl1 << "*W)";
  ofs.close();
  ofs.open(Ebz::OBIS_POWER_L2, std::ios::out);
  ofs << Ebz::OBIS_POWER_L2 << "(" << m_powerl2 << "*W)";
  ofs.close();
  ofs.open(Ebz::OBIS_POWER_L3, std::ios::out);
  ofs << Ebz::OBIS_POWER_L3 << "(" << m_powerl3 << "*W)";
  ofs.close();
  ofs.open(Ebz::OBIS_VOLTAGE_L1, std::ios::out);
  ofs << Ebz::OBIS_VOLTAGE_L1 << "(" << std::setprecision(1) << m_voltagel1 << "*V)";
  ofs.close();
  ofs.open(Ebz::OBIS_VOLTAGE_L2, std::ios::out);
  ofs << Ebz::OBIS_VOLTAGE_L2 << "(" << m_voltagel2 << "*V)";
  ofs.close();
  ofs.open(Ebz::OBIS_VOLTAGE_L3, std::ios::out);
  ofs << Ebz::OBIS_VOLTAGE_L3 << "(" << m_voltagel3 << "*V)";
  ofs.close();
}

void Ebz::runObis(void) const
{
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    writeObisCodes();
  }
}
