#include "pch.h"
#include "configuration_info.h"

namespace confDock_info
{
    const std::map<GroupIndex, std::wstring> groupStringMap = {
        {GroupIndex::IDN, L"IDN"},
        {GroupIndex::CONF, L"CONF"}
    };

    const std::map<IdnItemIndex, std::wstring> idnStringMap = {
        {IdnItemIndex::DEV_ID, L"DEV ID"},
        {IdnItemIndex::FirmVer, L"Firmware Ver"}
    };

    const std::map<ConfItemIndex, std::wstring> confStringMap = {
        {ConfItemIndex::T_STACK, L"T STACK"},
        {ConfItemIndex::STACK1_CELL_NO, L"STACK1 Cell No."},
        {ConfItemIndex::STACK1_TEMP_NO, L"STACK1 Temp No."},
        {ConfItemIndex::STACK2_CELL_NO, L"STACK2 Cell No."},
        {ConfItemIndex::STACK2_TEMP_NO, L"STACK2 Temp No."},
        {ConfItemIndex::OV_THR, L"OV THR"},
        {ConfItemIndex::UV_THR, L"UV THR"},
        {ConfItemIndex::VCB_THR, L"VCB THR"},
        {ConfItemIndex::UT_THR, L"UT THR"},
        {ConfItemIndex::OT_THR, L"OT THR"},
        {ConfItemIndex::OTCB_THR, L"OTCB THR"},
    };
}
