#pragma once
#include <vector>
#include <string>
#include "enumBitOperation.h"

namespace meas_info
{
    enum class type : uint32_t
    {
        none = 0,
        MEAS,
        CB_MEAS
    };

    enum class mode : uint32_t
    {
        none = 0x00000000,

        connect = 0x00000001,
        disconnect = connect << 1,
        VC = disconnect << 1,
        TEMP = VC << 1,
        VBAT = TEMP << 1,
        STAT = VBAT << 1,

        ALL = VC | TEMP | VBAT
    };

    const static std::vector<std::wstring> MEAS_Repetition = {
        L"1 Samples/sec", L"2 Samples/sec", L"3 Samples/sec"
    };
    const static std::vector<std::wstring> CBMEAS_Repetition = {
        L"1 Samples/min", L"2 Samples/min", L"3 Samples/min", L"4 Samples/min", L"5 Samples/min", L"6 Samples/min", L"3 Samples/10s"
    };

} //namespace meas_info

DEFINE_BITMASK_OPERATION(meas_info::mode);
