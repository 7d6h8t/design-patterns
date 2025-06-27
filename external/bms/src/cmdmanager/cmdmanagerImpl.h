/*****************************************************************//**
 * @file   cmdmanagerImpl.h
 * @brief  interface for manage command read, write of serial communication.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "include/export.h"
#include <string>
#include <vector>

namespace BMS_info {
    struct IDN;
    struct CONF;
}
namespace MEAS_info {
    struct VC;
    struct TEMP;
    struct VBAT;
    struct STAT;
    struct ALL;
}
namespace CB_info {
    struct CBTIME;
}

namespace BMS {
    /**
     * @brief interface class for manage command read, write of serial communication.
     */
    class cmdmanagerImpl
    {
    protected:
        /**
         * @brief construct the object.
         */
        cmdmanagerImpl() noexcept;

        /**
         * @brief destruct the object.
         */
        virtual ~cmdmanagerImpl() noexcept;

        cmdmanagerImpl(const cmdmanagerImpl&) = delete;
        cmdmanagerImpl& operator=(const cmdmanagerImpl&) = delete;

        cmdmanagerImpl(cmdmanagerImpl&&) = delete;
        cmdmanagerImpl& operator=(cmdmanagerImpl&&) = delete;

    public:
        /**
         * @brief get cmdmanagerImpl instance.
         * 
         * @return cmdmanagerImpl instance
         */
        static cmdmanagerImpl& getInstance();

        // abstract functions for read, write of serial communication
        virtual bool serial_connect(const uint32_t nPort, const uint32_t nBaudrate) = 0;
        virtual bool serial_disconnect() = 0;
        virtual bool isConnected() = 0;

        virtual BMS_info::IDN sendIDN() = 0;
        virtual BMS_info::CONF sendCONF() = 0;
        virtual bool sendWAKEUP() = 0;
        virtual MEAS_info::VC sendMEAS_VC() = 0;
        virtual MEAS_info::TEMP sendMEAS_TEMP() = 0;
        virtual MEAS_info::VBAT sendMEAS_VBAT() = 0;
        virtual MEAS_info::STAT sendMEAS_STAT() = 0;
        virtual MEAS_info::ALL sendMEAS_ALL() = 0;
        virtual bool sendCBSET(const std::wstring& stack, const std::vector<std::wstring>& cbset) = 0;
        virtual bool sendCBWRITE() = 0;
        virtual bool sendCBSTART_MAN() = 0;
        virtual bool sendCBSTART_AUTO() = 0;
        virtual CB_info::CBTIME sendCBTIME() = 0;
    }; //class cmdmanagerImpl
} //namespace BMS
