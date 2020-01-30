#include <iostream>
#include "ebz.hpp"

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

void Ebz::writeSharedMem() const
{

}
