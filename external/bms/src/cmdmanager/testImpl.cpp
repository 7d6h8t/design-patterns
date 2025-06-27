#include "pch.h"
#include "testImpl.h"
#include <random>

using namespace BMS;

testImpl::testImpl() noexcept
    : m_bConnect(false)
{
}

testImpl::~testImpl() noexcept
{
}

bool testImpl::serial_connect(const uint32_t nPort, const uint32_t nBaudrate)
{
    m_bConnect = true;
    return true;
}

bool testImpl::serial_disconnect()
{
    m_bConnect = false;
    return true;
}

bool testImpl::isConnected()
{
    return m_bConnect;
}

BMS_info::IDN testImpl::sendIDN()
{
    BMS_info::IDN idn;
    idn.Device_ID = L"CBMS-R0";
    idn.Firmware_Ver = L"V1p0-Dec-2022";

    return idn;
}

BMS_info::CONF testImpl::sendCONF()
{
    BMS_info::CONF conf;
    conf.Total_STK = 2;
    conf.STK1_VC_Ch = 18;
    conf.STK1_TEMP_Ch = 9;
    conf.STK2_VC_Ch = 18;
    conf.STK2_TEMP_Ch = 9;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis_double(2.5, 4.2);

    conf.OV_THR = dis_double(gen);
    conf.UV_THR = dis_double(gen);
    conf.VCB_THR = dis_double(gen);

    std::uniform_int_distribution<int> dis_int(-5, 85);
    conf.OTCB_THR = dis_int(gen);
    conf.OT_THR = dis_int(gen);
    conf.UT_THR = dis_int(gen);

    return conf;
}

bool testImpl::sendWAKEUP()
{
    return true;
}

MEAS_info::VC testImpl::sendMEAS_VC()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis_double(2.777, 4.333);

    MEAS_info::VC vc;
    vc.STK1.reserve(18);
    vc.STK2.reserve(18);

    for (int i = 0; i < 18; ++i)
    {
        vc.STK1.push_back(dis_double(gen));
        vc.STK2.push_back(dis_double(gen));
    }

    return vc;
}

MEAS_info::TEMP BMS::testImpl::sendMEAS_TEMP()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis_double(31.1, 88.5);

    MEAS_info::TEMP temp;
    temp.STK1_thermistor.reserve(10);
    temp.STK1_sensor.reserve(3);

    temp.STK2_thermistor.reserve(10);
    temp.STK2_sensor.reserve(3);

    for (int i = 0; i < 10; ++i)
    {
        temp.STK1_thermistor.push_back(dis_double(gen));
        temp.STK2_thermistor.push_back(dis_double(gen));
    }

    for (int i = 0; i < 3; ++i)
    {
        temp.STK1_sensor.push_back(dis_double(gen));
        temp.STK2_sensor.push_back(dis_double(gen));
    }

    return temp;
}

MEAS_info::VBAT BMS::testImpl::sendMEAS_VBAT()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis_double(65.1, 68.9);

    MEAS_info::VBAT vbat;
    vbat.STK1 = dis_double(gen);
    vbat.STK2 = dis_double(gen);

    return vbat;
}

MEAS_info::STAT BMS::testImpl::sendMEAS_STAT()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis_bool(0.1);

    MEAS_info::STAT stat;
    stat.OV_STK1.reserve(18);
    stat.OV_STK2.reserve(18);
    stat.UV_STK1.reserve(18);
    stat.UV_STK2.reserve(18);
    stat.VCB_STK1.reserve(18);
    stat.VCB_STK2.reserve(18);

    for (int i = 0; i < 18; ++i)
    {
        stat.OV_STK1.push_back(dis_bool(gen));
        stat.OV_STK2.push_back(dis_bool(gen));
        stat.UV_STK1.push_back(dis_bool(gen));
        stat.UV_STK2.push_back(dis_bool(gen));
        stat.VCB_STK1.push_back(dis_bool(gen));
        stat.VCB_STK2.push_back(dis_bool(gen));
    }

    stat.UT_STK1.reserve(10);
    stat.UT_STK2.reserve(10);
    stat.OT_STK1.reserve(10);
    stat.OT_STK2.reserve(10);
    stat.OTCB_STK1.reserve(10);
    stat.OTCB_STK2.reserve(10);

    for (int i = 0; i < 10; ++i)
    {
        stat.UT_STK1.push_back(dis_bool(gen));
        stat.UT_STK2.push_back(dis_bool(gen));
        stat.OT_STK1.push_back(dis_bool(gen));
        stat.OT_STK2.push_back(dis_bool(gen));
        stat.OTCB_STK1.push_back(dis_bool(gen));
        stat.OTCB_STK2.push_back(dis_bool(gen));
    }

    return stat;
}

MEAS_info::ALL testImpl::sendMEAS_ALL()
{
    MEAS_info::ALL all;
    all.vc = sendMEAS_VC();
    all.temp = sendMEAS_TEMP();
    all.vbat = sendMEAS_VBAT();

    return all;
}

bool BMS::testImpl::sendCBSET(const std::wstring& stack, const std::vector<std::wstring>& cbset)
{
    return true;
}

bool BMS::testImpl::sendCBWRITE()
{
    return true;
}

bool BMS::testImpl::sendCBSTART_MAN()
{
    return  true;
}

bool BMS::testImpl::sendCBSTART_AUTO()
{
    return true;
}

CB_info::CBTIME BMS::testImpl::sendCBTIME()
{
    CB_info::CBTIME cbtime;

    cbtime.STK1.reserve(18);
    cbtime.STK2.reserve(18);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis_int(0, 59);

    for (int i = 0; i < 18; ++i)
    {
        cbtime.STK1.push_back(std::to_wstring(dis_int(gen)) + L"m" + std::to_wstring(dis_int(gen)));
        cbtime.STK2.push_back(std::to_wstring(dis_int(gen)) + L"m" + std::to_wstring(dis_int(gen)));
    }

    return cbtime;
}
