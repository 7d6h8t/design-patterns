#include "pch.h"
#include "serialImpl.h"
#include "convert.h"
#include <utils.h>

using namespace BMS;

const std::string serialImpl::endflag = ">>";

serialImpl::serialImpl() noexcept
{
}

serialImpl::~serialImpl() noexcept
{
}

bool serialImpl::serial_connect(const uint32_t nPort, const uint32_t nBaudrate)
{
    bool result = m_port.connect(nPort, nBaudrate);
    if (result)
        repeat_read(50, 3);

    return result;
}

bool serialImpl::serial_disconnect()
{
    return m_port.disconnect();
}

bool serialImpl::isConnected()
{
    return m_port.IsConnected();
}

BMS_info::IDN serialImpl::sendIDN()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string idn_cmd = "*IDN?\r\n";
    m_port.write(idn_cmd);

    std::string idn = repeat_read(30, 3);
    if (idn.empty())
        throw std::exception("ERROR : [*IDN?] read fail!");

    if (isValid(idn) == false)
        throw std::exception("ERROR : [*IDN?] result is \"Error Command!\"");

    return convert::toIDN(idn);
}

BMS_info::CONF serialImpl::sendCONF()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string conf_cmd = "*CONF?\r\n";
    m_port.write(conf_cmd);

    std::string conf = repeat_read(30, 3);
    if (conf.empty())
        throw std::exception("ERROR : [*CONF?] read fail!");

    if (isValid(conf) == false)
        throw std::exception("ERROR : [*CONF?] result is \"Error Command!\"");

    return convert::toCONF(conf);
}

bool serialImpl::sendWAKEUP()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string wakeup_cmd = "*WAKEUP\r\n";
    m_port.write(wakeup_cmd);

    std::string wakeup = repeat_read(50, 3);
    if (wakeup.empty())
        throw std::exception("ERROR : [*WAKEUP] read fail!");

    return isValid(wakeup);
}

MEAS_info::VC serialImpl::sendMEAS_VC()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string meas_vc_cmd = "*MEAS:VC?\r\n";
    m_port.write(meas_vc_cmd);

    std::string meas_vc = repeat_read(130, 4);
    if (meas_vc.empty())
        throw std::exception("ERROR : [*MEAS:VC?] read fail!");

    if (isValid(meas_vc) == false)
        throw std::exception("ERROR : [*MEAS:VC?] result is \"Error Command!\"");

    return convert::toMEAS_VC(meas_vc);
}

MEAS_info::TEMP serialImpl::sendMEAS_TEMP()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string meas_temp_cmd = "*MEAS:TEMP?\r\n";
    m_port.write(meas_temp_cmd);

    std::string meas_temp = repeat_read(90, 3);
    if (meas_temp.empty())
        throw std::exception("ERROR : [*MEAS:TEMP?] read fail!");

    if (isValid(meas_temp) == false)
        throw std::exception("ERROR : [*MEAS:TEMP?] result is \"Error Command!\"");

    return convert::toMEAS_TEMP(meas_temp);
}

MEAS_info::VBAT serialImpl::sendMEAS_VBAT()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string meas_vbat_cmd = "*MEAS:VBAT?\r\n";
    m_port.write(meas_vbat_cmd);

    std::string meas_vbat = repeat_read(50, 3);
    if (meas_vbat.empty())
        throw std::exception("ERROR : [*MEAS:VBAT?] read fail!");

    if (isValid(meas_vbat) == false)
        throw std::exception("ERROR : [*MEAS:VBAT?] result is \"Error Command!\"");

    return convert::toMEAS_VBAT(meas_vbat);
}

MEAS_info::STAT serialImpl::sendMEAS_STAT()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string meas_stat_cmd = "*MEAS:STAT?\r\n";
    m_port.write(meas_stat_cmd);

    std::string meas_stat = repeat_read(50, 3);
    if (meas_stat.empty())
        throw std::exception("ERROR : [*MEAS:STAT?] read fail!");

    if (isValid(meas_stat) == false)
        throw std::exception("ERROR : [*MEAS:STAT?] result is \"Error Command!\"");

    return convert::toMEAS_STAT(meas_stat);
}

MEAS_info::ALL serialImpl::sendMEAS_ALL()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string meas_all_cmd = "*MEAS:ALL?\r\n";
    m_port.write(meas_all_cmd);

    std::string meas_all = repeat_read(220, 3);
    if (meas_all.empty())
        throw std::exception("ERROR : [*MEAS:ALL?] read fail!");

    if (isValid(meas_all) == false)
        throw std::exception("ERROR : [*MEAS:ALL?] result is \"Error Command!\"");

    MEAS_info::ALL all;
    all.vc = convert::toMEAS_VC(meas_all);
    all.temp = convert::toMEAS_TEMP(meas_all);
    all.vbat = convert::toMEAS_VBAT(meas_all);

    return all;
}

bool serialImpl::sendCBSET(const std::wstring& stack, const std::vector<std::wstring>& cbset)
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    std::string cbset_cmd = convert::toCBSETString(stack, cbset);
    m_port.write(cbset_cmd);

    std::string cbset_result = repeat_read(600, 3);
    if (cbset_result.empty())
        throw std::exception("ERROR : [*CBS] read fail!");

    return isValid(cbset_result);
}

bool serialImpl::sendCBWRITE()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string cbwrite_cmd = "*CBWRITE\r\n";
    m_port.write(cbwrite_cmd);

    std::string cbwrite_result = repeat_read(50, 3);
    if (cbwrite_result.empty())
        throw std::exception("ERROR : [*CBWRITE] read fail!");

    return isValid(cbwrite_result);
}

bool serialImpl::sendCBSTART_MAN()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string cbstart_cmd = "*CBSTART:MAN\r\n";
    m_port.write(cbstart_cmd);

    std::string cbstart_result = repeat_read(80, 3);
    if (cbstart_result.empty())
        throw std::exception("ERROR : [*CBSTART:MAN] read fail!");

    return isValid(cbstart_result);
}

bool serialImpl::sendCBSTART_AUTO()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string cbstart_cmd = "*CBSTART:AUTO\r\n";
    m_port.write(cbstart_cmd);

    std::string cbstart_result = repeat_read(80, 3);
    if (cbstart_result.empty())
        throw std::exception("ERROR : [*CBSTART:AUTO] read fail!");

    return isValid(cbstart_result);
}

CB_info::CBTIME serialImpl::sendCBTIME()
{
    if (isConnected() == false)
        throw std::exception("connection error!");

    static const std::string cbtime_cmd = "*CBTIME?\r\n";
    m_port.write(cbtime_cmd);

    std::string cbtime = repeat_read(240, 3);
    if (cbtime.empty())
        throw std::exception("ERROR : [*CBTIME?] read fail!");

    if (isValid(cbtime) == false)
        throw std::exception("ERROR : [*CBTIME?] result is \"Error Command!\"");

    return convert::toCBTIME(cbtime);
}

bool serialImpl::isValid(const std::string& str)
{
    return std::string::npos == str.find("Error Command!");
}

std::string serialImpl::repeat_read(const uint32_t wait_ms, const uint32_t repeat, const std::source_location location)
{
    std::string read_data;

    for (uint32_t i = 1; i <= repeat; ++i)
    {
        try
        {
            Sleep(wait_ms);
            read_data += m_port.read();
            if (isEndflag(read_data))
                break;

            std::string error_log = std::format("[Time out] {} receive fail. received data is not total data ({}ms waiting, {}/{} try)", location.function_name(), wait_ms, i, repeat);
            m_errorlogger.log_message(error_log);
        }
        catch (const std::exception& e) {
            std::string error_log = std::format("[Time out] {} receive fail. {} ({}ms waiting, {}/{} try)", location.function_name(), e.what(), wait_ms, i, repeat);
            m_errorlogger.log_message(error_log);
            DebugPrint(error_log);
        }
    }

    if (isValid(read_data) == false)
    {
        std::string error_log = std::format("[Error command] {} receive fail. receive data is Error Command!", location.function_name());
        m_errorlogger.log_message(error_log);
        throw std::exception(error_log.c_str());
    }


    if (isEndflag(read_data) == false)
    {        
        std::string error_log = std::format("[Time out] {} receive fail. received data is not total data ({}ms waiting, {}/{} try)", location.function_name(), wait_ms, repeat, repeat);
        m_errorlogger.log_message(error_log);
        throw std::exception(error_log.c_str());
    }

    return read_data;
}

bool serialImpl::isEndflag(const std::string& str) const noexcept
{
    if (str.empty())
        return false;

    size_t pos = str.rfind(endflag);
    if (pos == std::string::npos)
        return false;

    return str.size() == (pos + endflag.size());
}
