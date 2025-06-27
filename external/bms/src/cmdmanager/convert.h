#pragma once
#include "include/BMS_info.h"
#include "include/MEAS_info.h"
#include "include/CB_info.h"
#include <string>
#include <vector>
#include <map>

namespace convert {
    BMS_info::IDN toIDN(const std::string& idn_str);
    BMS_info::CONF toCONF(const std::string& conf_str);

    MEAS_info::VC toMEAS_VC(const std::string& meas_vc_str);
    MEAS_info::TEMP toMEAS_TEMP(const std::string& meas_temp_str);
    MEAS_info::VBAT toMEAS_VBAT(const std::string& meas_vbat_str);
    MEAS_info::STAT toMEAS_STAT(const std::string& meas_stat_str);

    std::string toCBSETString(const std::wstring& stack, const std::vector<std::wstring>& cbset);
    CB_info::CBTIME toCBTIME(const std::string& cbtime_str);

    static const std::map<std::wstring, std::string> CBSET_map = { 
        {L"0", "0"}, {L"10sec", "1"}, {L"30sec", "2"}, {L"60sec", "3"}, {L"5min", "4"}, {L"10min", "5"}, {L"20min", "6"},
        {L"30min", "7"}, {L"40min", "8"}, {L"50min", "9"}, {L"60min", "A"}, {L"70min", "B"}, {L"80min", "C"}, {L"90min", "D"},
        {L"100min", "E"}, {L"110min", "F"}, {L"120min", "G"}, {L"150min", "H"}, {L"180min", "I"}, {L"210min", "J"}, {L"240min", "K"},
        {L"270min", "L"}, {L"300min", "M"}, {L"330min", "N"}, {L"360min", "O"}, {L"390min", "P"}, {L"420min", "Q"}, {L"450min", "R"},
        {L"480min", "S"}, {L"510min", "T"}, {L"540min", "U"}, {L"600min", "V"}
    };

    static uint32_t total_stk_size = 2;
}
