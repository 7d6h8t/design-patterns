#include "pch.h"
#include "include/cmdmanager.h"
#include "cmdmanagerImpl.h"

using namespace BMS;

bool cmdmanager::serial_connect(const uint32_t nPort, const uint32_t nBaudrate)
{
    return cmdmanagerImpl::getInstance().serial_connect(nPort, nBaudrate);
}

bool cmdmanager::serial_disconnect()
{
    return cmdmanagerImpl::getInstance().serial_disconnect();
}

bool cmdmanager::isConnected()
{
    return cmdmanagerImpl::getInstance().isConnected();
}

BMS_info::IDN cmdmanager::sendIDN()
{
    return cmdmanagerImpl::getInstance().sendIDN();
}

BMS_info::CONF cmdmanager::sendCONF()
{
    return cmdmanagerImpl::getInstance().sendCONF();
}

bool cmdmanager::sendWAKEUP()
{
    return cmdmanagerImpl::getInstance().sendWAKEUP();
}

MEAS_info::VC cmdmanager::sendMEAS_VC()
{
    return cmdmanagerImpl::getInstance().sendMEAS_VC();
}

MEAS_info::TEMP BMS::cmdmanager::sendMEAS_TEMP()
{
    return cmdmanagerImpl::getInstance().sendMEAS_TEMP();
}

MEAS_info::VBAT BMS::cmdmanager::sendMEAS_VBAT()
{
    return cmdmanagerImpl::getInstance().sendMEAS_VBAT();
}

MEAS_info::STAT BMS::cmdmanager::sendMEAS_STAT()
{
    return cmdmanagerImpl::getInstance().sendMEAS_STAT();
}

MEAS_info::ALL cmdmanager::sendMEAS_ALL()
{
    return cmdmanagerImpl::getInstance().sendMEAS_ALL();
}

bool BMS::cmdmanager::sendCBSET(const std::wstring& stack, const std::vector<std::wstring>& cbset)
{
    return cmdmanagerImpl::getInstance().sendCBSET(stack, cbset);
}

bool BMS::cmdmanager::sendCBWRITE()
{
    return cmdmanagerImpl::getInstance().sendCBWRITE();
}

bool BMS::cmdmanager::sendCBSTART_MAN()
{
    return cmdmanagerImpl::getInstance().sendCBSTART_MAN();
}

bool BMS::cmdmanager::sendCBSTART_AUTO()
{
    return cmdmanagerImpl::getInstance().sendCBSTART_AUTO();
}

CB_info::CBTIME BMS::cmdmanager::sendCBTIME()
{
    return cmdmanagerImpl::getInstance().sendCBTIME();
}
