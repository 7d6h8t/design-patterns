/*****************************************************************//**
 * @file   testImpl.h
 * @brief  implementation class for test.
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

namespace BMS {
    /**
     * @brief implementation class for test.
     */
    class testImpl : public BMS::cmdmanagerImpl
    {
    public:
        /**
         * @brief constructs the object.
         */
        testImpl() noexcept;

        /**
         * @brief destructs the object.
         */
        virtual ~testImpl() noexcept;

        testImpl(const testImpl&) = delete;
        testImpl& operator=(const testImpl&) = delete;

        testImpl(testImpl&&) = delete;
        testImpl& operator=(testImpl&&) = delete;

    public:
        /**
         * @brief test connection function.
         * 
         * @param[in] nPort         port number
         * @param[in] nBaudrate     baudrate
         * @return success or not
         */
        virtual bool serial_connect(const uint32_t nPort, const uint32_t nBaudrate) override;

        /**
         * @brief test disconnection function.
         * 
         * @return success or not
         */
        virtual bool serial_disconnect() override;

        /**
         * @brief test checking connection function.
         * 
         * @return connected or not
         */
        virtual bool isConnected() override;

        /**
         * @brief test setting IDN function.
         * 
         * @return IDN info
         */
        virtual BMS_info::IDN sendIDN() override;

        /**
         * @brief test setting CONF function.
         * 
         * @return CONF info
         */
        virtual BMS_info::CONF sendCONF() override;

        /**
         * @brief test wakeup function.
         * 
         * @return true
         */
        virtual bool sendWAKEUP() override;

        /**
         * @brief test setting VC function.
         * 
         * @return VC info
         */
        virtual MEAS_info::VC sendMEAS_VC() override;

        /**
         * @brief test setting TEMP function.
         * 
         * @return TEMP info
         */
        virtual MEAS_info::TEMP sendMEAS_TEMP() override;

        /**
         * @brief test setting VBAT function.
         * 
         * @return VBAT info
         */
        virtual MEAS_info::VBAT sendMEAS_VBAT() override;

        /**
         * @brief test setting STAT function.
         * 
         * @return STAT info
         */
        virtual MEAS_info::STAT sendMEAS_STAT() override;

        /**
         * @brief test setting ALL function.
         * 
         * @return ALL info
         */
        virtual MEAS_info::ALL sendMEAS_ALL() override;

        /**
         * @brief test setting CBSET function.
         * 
         * @param[in] stack     stack information required for "*CBSET" command
         * @param[in] cbset     cb(cell balancing) time information required for "*CBSET" command
         * @return success or not
         */
        virtual bool sendCBSET(const std::wstring& stack, const std::vector<std::wstring>& cbset) override;

        /**
         * @brief test setting CBWRITE function.
         * 
         * @return success or not
         */
        virtual bool sendCBWRITE() override;

        /**
         * @brief test setting CBSTART_MAN function.
         * 
         * @return success or not
         */
        virtual bool sendCBSTART_MAN() override;

        /**
         * @brief test setting CBSTART_AUTO function.
         * 
         * @return success or not
         */
        virtual bool sendCBSTART_AUTO() override;

        /**
         * @brief test setting CBTIME function.
         * 
         * @return CBTIME info
         */
        virtual CB_info::CBTIME sendCBTIME() override;

    protected:
        bool m_bConnect;
    }; //class testImpl : public BMS::cmdmanagerImpl
} //namespace BMS
