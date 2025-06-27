/*****************************************************************//**
 * @file   serial_port.h
 * @brief  serial communication class.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "export.h"
#include <windows.h>
#include <string>
#include <atomic>
#include <future>

#pragma warning(push)
#pragma warning(disable:4251)

namespace comm
{
    /**
     * @brief class for serial communication.
     */
    class DLLAPI serial_port final
    {
    public:
        /**
         * @brief constructs the object.
         */
        serial_port() noexcept;

        /**
         * @brief destructs the object.
         */
        ~serial_port() noexcept;

    public:
        /**
         * @brief connect serial communication.
         * 
         * @param[in] nPort         port number
         * @param[in] nBaudrate     baudrate
         * @return connection success or not
         */
        bool connect(const uint32_t nPort, const uint32_t nBaudrate) noexcept;

        /**
         * @brief disconnect serial communication.
         * 
         * @return disconnection success or not
         */
        bool disconnect() noexcept;

        /**
         * @brief checks whether that is connected.
         * 
         * @return connected or disconnected
         */
        bool IsConnected() const noexcept;

        /**
         * @brief write to the serial communication.
         * 
         * @param[in] str       writing string
         */
        void write(const std::string& str);

        /**
         * @brief write to the serial communication by asynchronous.
         * 
         * @param[in] str       writing string
         * @return result of asynchronous operations
         */
        std::future<void> async_write(const std::string& str);

        /**
         * @brief read from the serial communication.
         * 
         * @return read string
         */
        std::string read();

        /**
         * @brief read from the serial communication until flag.
         * 
         * @param[in] endflag       last read flag
         * @return read string
         */
        std::string read_until_flag(const std::string& endflag);

        /**
         * @brief read from the serial communication by asynchronous.
         * 
         * @return result future string of asynchronous operations
         */
        std::future<std::string> async_read();

        /**
         * @brief read from the serial communication until flag by asynchronous.
         * 
         * @param[in] endflag last read flag
         * @return result future string of asynchronous operations
         */
        std::future<std::string> async_read_until_flag(const std::string& endflag);

    private:
        /**
         * @brief create serial port handle.
         * 
         * @param[in] nPort     port number
         * @return create success or not
         */
        bool init_serialport_handle(const uint32_t nPort);

        /**
         * @brief create dcb object and setting communication state.
         * 
         * @param[in] nBaudrate     baudrate
         * @return setting success or not
         */
        bool init_dcb(const uint32_t nBaudrate);

        /**
         * @brief setting coumminication timeout.
         * 
         * @return setting success or not
         */
        bool init_timeout();

        /**
         * @brief clear serial communication buffer.
         * 
         * @return clear success or not
         */
        bool clear_commbuffer();

        /**
         * @brief close serial port handle.
         */
        void close_serialport_handle();

        /**
         * @brief checks whether the string is endflag.
         * 
         * @param[in] str       checking string
         * @param[in] endflag   checking endflag
         * @return true if string ends with endflag
         */
        bool isEndflag(const std::string& str, const std::string& endflag) const noexcept;

    private:
        HANDLE m_hSerial;
        std::atomic_bool m_bDisconnecting;
    };
}

#pragma warning(pop)
