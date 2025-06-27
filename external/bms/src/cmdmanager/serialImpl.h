/*****************************************************************//**
 * @file   serialImpl.h
 * @brief  implementation class for manage command read, write of serial communication.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "cmdmanagerImpl.h"
#include "include/BMS_info.h"
#include "include/MEAS_info.h"
#include "include/CB_info.h"
#include <string>
#include "serial_port.h"
#include <source_location>
#include "error_log.h"

namespace BMS {
    /**
     * @brief implementation class for manage command read, write of serial communication.
     */
    class serialImpl : public BMS::cmdmanagerImpl
    {
    public:
        /**
         * @brief constructs the object.
         */
        serialImpl() noexcept;

        /**
         * @brief destructs the object.
         */
        virtual ~serialImpl() noexcept;

        serialImpl(const serialImpl&) = delete;
        serialImpl& operator=(const serialImpl&) = delete;

        serialImpl(serialImpl&&) = delete;
        serialImpl& operator=(serialImpl&&) = delete;

    public:
        /**
         * @brief connect serial communication.
         * 
         * @param[in] nPort         port number
         * @param[in] nBaudrate     baudrate
         * @return connection success or not
         */
        virtual bool serial_connect(const uint32_t nPort, const uint32_t nBaudrate) override;

        /**
         * @brief disconnect serial communication.
         *
         * @return disconnection success or not
         */
        virtual bool serial_disconnect() override;

        /**
         * @brief checks whether that is connected.
         *
         * @return connected or disconnected
         */
        virtual bool isConnected() override;

        /**
         * @brief send "*IDN?" command and receive result.
         *
         * @return IDN info
         */
        virtual BMS_info::IDN sendIDN() override;

        /**
         * @brief send "*CONF?" command and receive result.
         *
         * @return CONF info
         */
        virtual BMS_info::CONF sendCONF() override;

        /**
         * @brief send "*WAKEUP" command and receive result.
         *
         * @return success or not
         */
        virtual bool sendWAKEUP() override;

        /**
         * @brief send "*MEAS_VC?" command and receive result.
         *
         * @return VC info
         */
        virtual MEAS_info::VC sendMEAS_VC() override;

        /**
         * @brief send "*MEAS_TEMP?" command and receive result.
         *
         * @return TEMP info
         */
        virtual MEAS_info::TEMP sendMEAS_TEMP() override;

        /**
         * @brief send "*MEAS_VBAT?" command and receive result.
         *
         * @return VBAT info
         */
        virtual MEAS_info::VBAT sendMEAS_VBAT() override;

        /**
         * @brief send "*MEAS_STAT?" command and receive result.
         *
         * @return STAT info
         */
        virtual MEAS_info::STAT sendMEAS_STAT() override;

        /**
         * @brief send "*MEAS_ALL?" command and receive result.
         *
         * @return ALL info
         */
        virtual MEAS_info::ALL sendMEAS_ALL() override;

        /**
         * @brief send "*CBSET" command and receive result.
         *
         * @param[in] stack     stack information required for "*CBSET" command
         * @param[in] cbset     cb(cell balancing) time information required for "*CBSET" command
         * @return success or not
         */
        virtual bool sendCBSET(const std::wstring& stack, const std::vector<std::wstring>& cbset) override;

        /**
         * @brief send "*CBWRITE" command and receive result.
         *
         * @return success or not
         */
        virtual bool sendCBWRITE() override;

        /**
         * @brief send "*CBSTART_MAN" command and receive result..
         *
         * @return success or not
         */
        virtual bool sendCBSTART_MAN() override;

        /**
         * @brief send "*CBSTART_AUTO" command and receive result.
         * 
         * @return success or not
         */
        virtual bool sendCBSTART_AUTO() override;

        /**
         * @brief send "*CBTIME?" command and receive result..
         *
         * @return the cb(cell balancing) remaining time info
         */
        virtual CB_info::CBTIME sendCBTIME() override;

    protected:

        /**
         * @brief check whether the string is valid.
         * 
         * @param[in] str   checking string
         * @return valid or not
         */
        bool isValid(const std::string& str);

        /**
         * @brief read a string from the serial buffer a specified number of repeats
         * 
         * @param[in] wait_ms       wait time (ms)
         * @param[in] repeat        number of repeats
         * @return string read from serial buffer
         */
        std::string repeat_read(const uint32_t wait_ms, const uint32_t repeat, const std::source_location location = std::source_location::current());

        /**
         * @brief checks wheter the string is endflag.
         * 
         * @param[in] str       checking string
         * @return true if string ends with endflag
         */
        bool isEndflag(const std::string& str) const noexcept;

    protected:
        comm::serial_port m_port;
        filemanager::error_log m_errorlogger;
        static const std::string endflag;
    }; //class serialImpl : public BMS::cmdmanagerImpl
} //namespace BMS
