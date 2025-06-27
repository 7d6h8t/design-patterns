#pragma once
#include "export.h"
#include <string>

#pragma warning(push)
#pragma warning(disable:4251)

namespace BMS_info {
    struct DLL_EXPORT IDN
    {
        std::wstring Device_ID;
        std::wstring Firmware_Ver;
    }; //struct IDN

    struct DLL_EXPORT CONF
    {
        uint32_t Total_STK;
        uint32_t STK1_VC_Ch;
        uint32_t STK1_TEMP_Ch;
        uint32_t STK2_VC_Ch;
        uint32_t STK2_TEMP_Ch;
        double OV_THR;
        double UV_THR;
        double VCB_THR;
        double OTCB_THR;
        double OT_THR;
        double UT_THR;
    }; //struct CONF
} //namespace BMS_info

#pragma warning(pop)
