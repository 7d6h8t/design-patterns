/*****************************************************************//**
 * @file   cmdmanager.h
 * @brief  manage command read, write of serial communication.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "export.h"
#include "BMS_info.h"
#include "MEAS_info.h"
#include "CB_info.h"
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)

namespace BMS {
    /**
     * @brief class for manage command read, write of serial communication.
     */
    class DLL_EXPORT cmdmanager
    {
    public:
        cmdmanager() noexcept = delete;
        ~cmdmanager() noexcept = delete;

    public:
        /**
         * @brief connect serial communication.
         * 
         * @param[in] nPort         port number
         * @param[in] nBaudrate     baudrate
         * @return connection success or not
         */
        static bool serial_connect(const uint32_t nPort, const uint32_t nBaudrate);

        /**
         * @brief disconnect serial communication.
         * 
         * @return disconnection success or not
         */
        static bool serial_disconnect();

        /**
         * @brief checks whether that is connected.
         * 
         * @return connected or disconnected
         */
        static bool isConnected();

        /**
         * @brief send "*IDN?" command and receive result.
         * 
         * @return IDN info
         */
        static BMS_info::IDN sendIDN();

        /**
         * @brief send "*CONF?" command and receive result.
         * 
         * @return CONF info
         */
        static BMS_info::CONF sendCONF();

        /**
         * @brief send "*WAKEUP" command and receive result.
         * 
         * @return success or not
         */
        static bool sendWAKEUP();

        /**
         * @brief send "*MEAS_VC?" command and receive result.
         * 
         * @return VC info
         */
        static MEAS_info::VC sendMEAS_VC();

        /**
         * @brief send "*MEAS_TEMP?" command and receive result.
         * 
         * @return TEMP info
         */
        static MEAS_info::TEMP sendMEAS_TEMP();

        /**
         * @brief send "*MEAS_VBAT?" command and receive result.
         * 
         * @return VBAT info
         */
        static MEAS_info::VBAT sendMEAS_VBAT();

        /**
         * @brief send "*MEAS_STAT?" command and receive result.
         * 
         * @return STAT info
         */
        static MEAS_info::STAT sendMEAS_STAT();

        /**
         * @brief send "*MEAS_ALL?" command and receive result.
         * 
         * @return ALL info
         */
        static MEAS_info::ALL sendMEAS_ALL();

        /**
         * @brief send "*CBSET" command and receive result.
         * 
         * @param[in] stack     stack information required for "*CBSET" command
         * @param[in] cbset     cb(cell balancing) time information required for "*CBSET" command
         * @return success or not
         */
        static bool sendCBSET(const std::wstring& stack, const std::vector<std::wstring>& cbset);

        /**
         * @brief send "*CBWRITE" command and receive result.
         * 
         * @return success or not
         */
        static bool sendCBWRITE();

        /**
         * @brief send "*CBSTART_MAN" command and receive result..
         * 
         * @return success or not
         */
        static bool sendCBSTART_MAN();

        /**
         * @brief send "*CBSTART_AUTO" command and receive result.
         * 
         * @return success or not
         */
        static bool sendCBSTART_AUTO();

        /**
         * @brief send "*CBTIME?" command and receive result..
         * 
         * @return the cb(cell balancing) remaining time info
         */
        static CB_info::CBTIME sendCBTIME();
    }; //class cmdmanager
} //namespace BMS

#pragma warning(pop)
