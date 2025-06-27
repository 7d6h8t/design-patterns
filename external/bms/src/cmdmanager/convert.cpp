#include "pch.h"
#include "convert.h"
#include <utils.h>
#include <regex>

namespace convert
{
    BMS_info::IDN toIDN(const std::string& idn_str)
    {
        static const std::string idn_pattern = R"(Device_ID=(.+), Firmware_Ver.=(.+))";

        static const std::regex idn_regex(idn_pattern);
        std::smatch match;
        if (std::regex_search(idn_str, match, idn_regex) == false)
            throw std::exception("IDN string to data convert error!");

        BMS_info::IDN idn;
        idn.Device_ID = utils::stringLib::ToUnicodeString(match[1].str());
        idn.Firmware_Ver = utils::stringLib::ToUnicodeString(match[2].str());

        return idn;
    }

    BMS_info::CONF toCONF(const std::string& conf_str)
    {
        static const std::string total_stk = R"(Total_STK=(.+))";
        static const std::regex total_stk_regex(total_stk);
        std::smatch total_stk_match;
        if (std::regex_search(conf_str, total_stk_match, total_stk_regex) == false)
            throw std::exception("CONF string to data convert error!");

        total_stk_size = std::stoul(total_stk_match[1].str());

        BMS_info::CONF conf;
        conf.Total_STK = total_stk_size;

        if (conf.Total_STK == 1)
        {
            static const std::string conf_stk1_pattern =
                R"(STK1_VC_Ch.=(.+), STK1_TEMP_Ch.=(.+)\r
OV_THR=(.+)V, UV_THR=(.+)V, VCB_THR=(.+)V, OTCB_THR=(.+)deg, OT_THR=(.+)deg, UT_THR=(.+)deg)";

            static const std::regex conf_regex(conf_stk1_pattern);
            std::smatch match;
            if (std::regex_search(conf_str, match, conf_regex) == false)
                throw std::exception("CONF string to data convert error!");

            conf.STK1_VC_Ch = std::stoul(match[1].str());
            conf.STK1_TEMP_Ch = std::stoul(match[2].str());
            conf.STK2_VC_Ch = 0;
            conf.STK2_TEMP_Ch = 0;
            conf.OV_THR = std::stod(match[3].str());
            conf.UV_THR = std::stod(match[4].str());
            conf.VCB_THR = std::stod(match[5].str());
            conf.OTCB_THR = std::stod(match[6].str());
            conf.OT_THR = std::stod(match[7].str());
            conf.UT_THR = std::stod(match[8].str());
        }
        else if (conf.Total_STK == 2)
        {
            static const std::string conf_stk2_pattern =
                R"(STK1_VC_Ch.=(.+), STK1_TEMP_Ch.=(.+), STK2_VC_Ch.=(.+), STK2_TEMP_Ch.=(.+)\r
OV_THR=(.+)V, UV_THR=(.+)V, VCB_THR=(.+)V, OTCB_THR=(.+)deg, OT_THR=(.+)deg, UT_THR=(.+)deg)";

            static const std::regex conf_regex(conf_stk2_pattern);
            std::smatch match;
            if (std::regex_search(conf_str, match, conf_regex) == false)
                throw std::exception("CONF string to data convert error!");

            conf.STK1_VC_Ch = std::stoul(match[1].str());
            conf.STK1_TEMP_Ch = std::stoul(match[2].str());
            conf.STK2_VC_Ch = std::stoul(match[3].str());
            conf.STK2_TEMP_Ch = std::stoul(match[4].str());
            conf.OV_THR = std::stod(match[5].str());
            conf.UV_THR = std::stod(match[6].str());
            conf.VCB_THR = std::stod(match[7].str());
            conf.OTCB_THR = std::stod(match[8].str());
            conf.OT_THR = std::stod(match[9].str());
            conf.UT_THR = std::stod(match[10].str());
        }
        else
            throw std::exception("CONF string to data convert error!");

        return conf;
    }

    MEAS_info::VC toMEAS_VC(const std::string& meas_vc_str)
    {
        MEAS_info::VC vc;
        if (total_stk_size >= 1)
        {
            static const std::string vc_stack1_pattern =
                R"(VC,STK1,18=(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+))";
            static const std::regex vc_stack1_regex(vc_stack1_pattern);
            std::smatch stack1_match;
            if (std::regex_search(meas_vc_str, stack1_match, vc_stack1_regex) == false)
                throw std::exception("MEAS:VC string to data convert error!");

            vc.STK1.reserve(18);

            for (int i = 1; i <= 18; ++i)
                vc.STK1.push_back(std::stod(stack1_match[i]) / 1000);
        }
        if (total_stk_size >= 2)
        {
            static const std::string vc_stack2_pattern =
                R"(VC,STK2,18=(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+))";
            static const std::regex vc_stack2_regex(vc_stack2_pattern);
            std::smatch stack2_match;
            if (std::regex_search(meas_vc_str, stack2_match, vc_stack2_regex) == false)
                throw std::exception("MEAS:VC string to data convert error!");

            vc.STK2.reserve(18);

            for (int i = 1; i <= 18; ++i)
                vc.STK2.push_back(std::stod(stack2_match[i]) / 1000);
        }

        return vc;
    }

    MEAS_info::TEMP toMEAS_TEMP(const std::string& meas_temp_str)
    {
        MEAS_info::TEMP temp;

        if (total_stk_size >= 1)
        {
            static const std::string temp_stack1_pattern =
                R"(TEMP,STK1,13=(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+))";
            static const std::regex temp_stack1_regex(temp_stack1_pattern);
            std::smatch stack1_match;
            if (std::regex_search(meas_temp_str, stack1_match, temp_stack1_regex) == false)
                throw std::exception("MEAS:TEMP string to data convert error!");

            temp.STK1_sensor.reserve(3);
            temp.STK1_thermistor.reserve(10);

            for (int i = 1; i <= 10; ++i)
                temp.STK1_thermistor.push_back(std::stod(stack1_match[i]) / 10);

            for (int i = 11; i <= 13; ++i)
                temp.STK1_sensor.push_back(std::stod(stack1_match[i]) / 10);
        }
        if (total_stk_size >= 2)
        {
            static const std::string temp_stack2_pattern =
                R"(TEMP,STK2,13=(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+))";
            static const std::regex temp_stack2_regex(temp_stack2_pattern);
            std::smatch stack2_match;
            if (std::regex_search(meas_temp_str, stack2_match, temp_stack2_regex) == false)
                throw std::exception("MEAS:TEMP string to data convert error!");

            temp.STK2_sensor.reserve(3);
            temp.STK2_thermistor.reserve(10);

            for (int i = 1; i <= 10; ++i)
                temp.STK2_thermistor.push_back(std::stod(stack2_match[i]) / 10);

            for (int i = 11; i <= 13; ++i)
                temp.STK2_sensor.push_back(std::stod(stack2_match[i]) / 10);
        }

        return temp;
    }

    MEAS_info::VBAT toMEAS_VBAT(const std::string& meas_vbat_str)
    {
        MEAS_info::VBAT vbat;

        if (total_stk_size >= 1)
        {
            static const std::string vbat_stk1_pattern =
                R"(VBAT,STK1=(\d+))";
            static const std::regex vbat_stk1_regex(vbat_stk1_pattern);
            std::smatch match;
            if (std::regex_search(meas_vbat_str, match, vbat_stk1_regex) == false)
                throw std::exception("MEAS:VBAT string to data convert error!");

            vbat.STK1 = std::stod(match[1]) / 100;
            vbat.STK2 = 0;
        }
        if (total_stk_size >= 2)
        {
            static const std::string vbat_stk2_pattern =
                R"(VBAT,STK2=(\d+))";
            static const std::regex vbat_stk2_regex(vbat_stk2_pattern);
            std::smatch match;
            if (std::regex_search(meas_vbat_str, match, vbat_stk2_regex) == false)
                throw std::exception("MEAS:VBAT string to data convert error!");

            vbat.STK2 = std::stod(match[1]) / 100;
        }

        return vbat;
    }

    MEAS_info::STAT toMEAS_STAT(const std::string& meas_stat_str)
    {
        MEAS_info::STAT stat;

        if (total_stk_size >= 1)
        {
            static const std::string stat_ov_stk1_pattern =
                R"(STAT-OV,STK1,18=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex ov_stk1_regex(stat_ov_stk1_pattern);
            std::smatch ov_stk1_match;
            if (std::regex_search(meas_stat_str, ov_stk1_match, ov_stk1_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_uv_stk1_pattern =
                R"(STAT-UV,STK1,18=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex uv_stk1_regex(stat_uv_stk1_pattern);
            std::smatch uv_stk1_match;
            if (std::regex_search(meas_stat_str, uv_stk1_match, uv_stk1_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_vcb_stk1_pattern =
                R"(STAT-VCB,STK1,18=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex vcb_stk1_regex(stat_vcb_stk1_pattern);
            std::smatch vcb_stk1_match;
            if (std::regex_search(meas_stat_str, vcb_stk1_match, vcb_stk1_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_ut_stk1_pattern =
                R"(STAT-UT,STK1,10=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex ut_stk1_regex(stat_ut_stk1_pattern);
            std::smatch ut_stk1_match;
            if (std::regex_search(meas_stat_str, ut_stk1_match, ut_stk1_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_ot_stk1_pattern =
                R"(STAT-OT,STK1,10=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex ot_stk1_regex(stat_ot_stk1_pattern);
            std::smatch ot_stk1_match;
            if (std::regex_search(meas_stat_str, ot_stk1_match, ot_stk1_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_otcb_stk1_pattern =
                R"(STAT-OTCB,STK1,10=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex otcb_stk1_regex(stat_otcb_stk1_pattern);
            std::smatch otcb_stk1_match;
            if (std::regex_search(meas_stat_str, otcb_stk1_match, otcb_stk1_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            stat.OV_STK1.reserve(18);
            stat.UV_STK1.reserve(18);
            stat.VCB_STK1.reserve(18);

            for (int i = 1; i <= 18; ++i)
            {
                stat.OV_STK1.push_back(std::stoi(ov_stk1_match[i].str()));
                stat.UV_STK1.push_back(std::stoi(uv_stk1_match[i].str()));
                stat.VCB_STK1.push_back(std::stoi(vcb_stk1_match[i].str()));
            }

            stat.UT_STK1.reserve(10);
            stat.OT_STK1.reserve(10);
            stat.OTCB_STK1.reserve(10);
            for (int i = 1; i <= 10; ++i)
            {
                stat.UT_STK1.push_back(std::stoi(ut_stk1_match[i].str()));
                stat.OT_STK1.push_back(std::stoi(ot_stk1_match[i].str()));
                stat.OTCB_STK1.push_back(std::stoi(otcb_stk1_match[i].str()));
            }
        }
        if (total_stk_size >= 2)
        {
            static const std::string stat_ov_stk2_pattern =
                R"(STAT-OV,STK2,18=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex ov_stk2_regex(stat_ov_stk2_pattern);
            std::smatch ov_stk2_match;
            if (std::regex_search(meas_stat_str, ov_stk2_match, ov_stk2_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_uv_stk2_pattern =
                R"(STAT-UV,STK2,18=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex uv_stk2_regex(stat_uv_stk2_pattern);
            std::smatch uv_stk2_match;
            if (std::regex_search(meas_stat_str, uv_stk2_match, uv_stk2_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_vcb_stk2_pattern =
                R"(STAT-VCB,STK2,18=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex vcb_stk2_regex(stat_vcb_stk2_pattern);
            std::smatch vcb_stk2_match;
            if (std::regex_search(meas_stat_str, vcb_stk2_match, vcb_stk2_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_ut_stk2_pattern =
                R"(STAT-UT,STK2,10=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex ut_stk2_regex(stat_ut_stk2_pattern);
            std::smatch ut_stk2_match;
            if (std::regex_search(meas_stat_str, ut_stk2_match, ut_stk2_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_ot_stk2_pattern =
                R"(STAT-OT,STK2,10=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex ot_stk2_regex(stat_ot_stk2_pattern);
            std::smatch ot_stk2_match;
            if (std::regex_search(meas_stat_str, ot_stk2_match, ot_stk2_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");

            static const std::string stat_otcb_stk2_pattern =
                R"(STAT-OTCB,STK2,10=(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1),(0|1))";
            static const std::regex otcb_stk2_regex(stat_otcb_stk2_pattern);
            std::smatch otcb_stk2_match;
            if (std::regex_search(meas_stat_str, otcb_stk2_match, otcb_stk2_regex) == false)
                throw std::exception("MEAS:STAT string to data convert error!");


            stat.OV_STK2.reserve(18);
            stat.UV_STK2.reserve(18);
            stat.VCB_STK2.reserve(18);
            for (int i = 1; i <= 18; ++i)
            {
                stat.OV_STK2.push_back(std::stoi(ov_stk2_match[i].str()));
                stat.UV_STK2.push_back(std::stoi(uv_stk2_match[i].str()));
                stat.VCB_STK2.push_back(std::stoi(vcb_stk2_match[i].str()));
            }

            stat.UT_STK2.reserve(10);
            stat.OT_STK2.reserve(10);
            stat.OTCB_STK2.reserve(10);
            for (int i = 1; i <= 10; ++i)
            {
                stat.UT_STK2.push_back(std::stoi(ut_stk2_match[i].str()));
                stat.OT_STK2.push_back(std::stoi(ot_stk2_match[i].str()));
                stat.OTCB_STK2.push_back(std::stoi(otcb_stk2_match[i].str()));
            }
        }

        return stat;
    }

    std::string toCBSETString(const std::wstring& stack, const std::vector<std::wstring>& cbset)
    {
        std::string cbset_cmd = "*CBS:" + utils::stringLib::ToMultiByteString(stack) + "=";

        for (const auto& cb : cbset)
            cbset_cmd += CBSET_map.at(cb);

        cbset_cmd += "\r\n";
        return cbset_cmd;
    }

    CB_info::CBTIME toCBTIME(const std::string& cbtime_str)
    {
        CB_info::CBTIME cbtime;

        if (total_stk_size >= 1)
        {
            static const std::string cbtime_stk1_pattern =
                R"(CBTIME_LEFT:STK1:18=(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+))";
            static const std::regex cbtime_stk1_regex(cbtime_stk1_pattern);
            std::smatch cbtime_stk1_match;
            if (std::regex_search(cbtime_str, cbtime_stk1_match, cbtime_stk1_regex) == false)
                throw std::exception("CBTIME string to data convert error!");

            cbtime.STK1.reserve(18);

            for (int i = 1; i <= 18; ++i)
                cbtime.STK1.push_back(utils::stringLib::ToUnicodeString(cbtime_stk1_match[i].str()));
        }
        if (total_stk_size >= 2)
        {
            static const std::string cbtime_stk2_pattern =
                R"(CBTIME_LEFT:STK2:18=(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+),(\d+m\d+))";
            static const std::regex cbtime_stk2_regex(cbtime_stk2_pattern);
            std::smatch cbtime_stk2_match;
            if (std::regex_search(cbtime_str, cbtime_stk2_match, cbtime_stk2_regex) == false)
                throw std::exception("CBTIME string to data convert error!");

            cbtime.STK2.reserve(18);

            for (int i = 1; i <= 18; ++i)
                cbtime.STK2.push_back(utils::stringLib::ToUnicodeString(cbtime_stk2_match[i].str()));
        }

        return cbtime;
    }
}
