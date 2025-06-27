#pragma once
#include <string>
#include <map>

namespace grid_info
{
    enum CB
    {
        SET = 0,
        TIME_LEFT
    };

    enum STAT_VC
    {
        VCB_DONE = 0,
        UV,
        OV
    };

    enum STAT_TEMP
    {
        UT = 0,
        OT,
        OTCB
    };

    static const std::map<CB, std::wstring> CB_columns = {
        {CB::SET, L"SET"},
        {CB::TIME_LEFT, L"Time Left"}
    };

    static const std::map<STAT_VC, std::wstring> VC_columns = {
        {STAT_VC::VCB_DONE, L"VCB DONE"},
        {STAT_VC::UV, L"UV"},
        {STAT_VC::OV, L"OV"}
    };

    static const std::map<STAT_TEMP, std::wstring> TEMP_columns = {
        {STAT_TEMP::UT, L"UT"},
        {STAT_TEMP::OT, L"OT"},
        {STAT_TEMP::OTCB, L"OTCB"}
    };
}
