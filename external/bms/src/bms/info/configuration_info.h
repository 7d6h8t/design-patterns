#pragma once
#include <map>
#include <string>

namespace confDock_info
{
    enum class GroupIndex : uint32_t
    {
        IDN = 0,
        CONF
    };

    enum class IdnItemIndex : uint32_t
    {
        DEV_ID = 0,
        FirmVer
    };

    enum class ConfItemIndex : uint32_t
    {
        T_STACK = 0,
        STACK1_CELL_NO,
        STACK1_TEMP_NO,
        STACK2_CELL_NO,
        STACK2_TEMP_NO,
        OV_THR,
        UV_THR,
        VCB_THR,
        UT_THR,
        OT_THR,
        OTCB_THR,
    };

    extern const std::map<GroupIndex, std::wstring> groupStringMap;
    extern const std::map<IdnItemIndex, std::wstring> idnStringMap;
    extern const std::map<ConfItemIndex, std::wstring> confStringMap;
} //namespace info
