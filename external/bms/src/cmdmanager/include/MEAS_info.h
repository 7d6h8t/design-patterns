#pragma once
#include "export.h"
#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)

namespace MEAS_info {
    struct DLL_EXPORT VC
    {
        std::vector<double> STK1;
        std::vector<double> STK2;

    }; //struct VC

    struct DLL_EXPORT TEMP
    {
        std::vector<double> STK1_sensor;
        std::vector<double> STK1_thermistor;
        std::vector<double> STK2_sensor;
        std::vector<double> STK2_thermistor;
    }; //struct TEMP

    struct DLL_EXPORT VBAT
    {
        double STK1;
        double STK2;
    }; //struct VBAT

    struct DLL_EXPORT STAT
    {
        std::vector<bool> OV_STK1;
        std::vector<bool> OV_STK2;
        std::vector<bool> UV_STK1;
        std::vector<bool> UV_STK2;
        std::vector<bool> VCB_STK1;
        std::vector<bool> VCB_STK2;
        std::vector<bool> UT_STK1;
        std::vector<bool> UT_STK2;
        std::vector<bool> OT_STK1;
        std::vector<bool> OT_STK2;
        std::vector<bool> OTCB_STK1;
        std::vector<bool> OTCB_STK2;
    }; //struct STAT

    struct DLL_EXPORT ALL
    {
        VC vc;
        TEMP temp;
        VBAT vbat;
    }; //struct ALL
} //namespace MEAS_info

#pragma warning(pop)
