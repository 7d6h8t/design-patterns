#include "pch.h"
#include "include/serial_port.h"
#include <exception>
#include <utils.h>
#include <format>
#include <array>

using namespace comm;

serial_port::serial_port() noexcept
    : m_hSerial(nullptr),
    m_bDisconnecting(false)
{
}

serial_port::~serial_port() noexcept
{
    disconnect();
}

bool serial_port::connect(const uint32_t nPort, const uint32_t nBaudrate) noexcept
{
    try
    {
        if (init_serialport_handle(nPort) == false)
            throw std::exception("init handle fail!");
        if (init_dcb(nBaudrate) == false)
            throw std::exception("init dcb fail!");
        if (init_timeout() == false)
            throw std::exception("init timeout fail!");
        if (clear_commbuffer() == false)
            throw std::exception("clear commbuffer fail!");

        static constexpr uint32_t wait_ms = 2000;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
    } catch (const std::exception& e) {
        disconnect();
        DebugPrint("[connection error] {}", e.what());
        return false;
    }
    return true;
}

bool serial_port::disconnect() noexcept
{
    try
    {
        utils::functionalGuard gaurd([&] {m_bDisconnecting = true; }, [&] {m_bDisconnecting = false; });

        if (IsConnected())
        {
            if (CancelIoEx(m_hSerial, NULL) == false)
            {
                if (GetLastError() != ERROR_NOT_FOUND)
                    throw std::exception("cancel I/O fail!");
            }
        }

        close_serialport_handle();
    } catch (const std::exception& e) {
        DebugPrint("[disconnect error] [{}] error code : %d", e.what(), GetLastError());
        return false;
    }
    return true;
}

bool serial_port::IsConnected() const noexcept
{
    if (m_hSerial != nullptr && m_hSerial != INVALID_HANDLE_VALUE)
        return true;
    return false;
}

void serial_port::write(const std::string& str)
{
    if (IsConnected() == false)
        throw std::exception("[write erorr] connection error!");

    OVERLAPPED ov_write = { 0 };
    DWORD transferred_byte;
    // hFile 이 FILE_FLAG_OVERLAPPED 로 열린 경우 lpNumberOfBytesRead (4번째 인자) 는 NULL 로 설정 되어야한다.
    // -> 잠재적으로 잘못된 결과를 피하기 위해 비동기 작업의 경우 이 매개변수를 NULL 로 사용해야한다.
    // ref : https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
    if (WriteFile(m_hSerial, str.c_str(), static_cast<DWORD>(str.length()), NULL, &ov_write) == false)
    {
        DWORD error = GetLastError();
        if (error == ERROR_IO_PENDING)
        {
            if (GetOverlappedResult(m_hSerial, &ov_write, &transferred_byte, TRUE) == false)
                throw std::exception(std::format("[write error] {}", GetLastError()).c_str());
        }
        else
            throw std::exception(std::format("[write error] {}", error).c_str());
    }
}

std::future<void> serial_port::async_write(const std::string& str)
{
    return std::async(std::launch::async, [&]() {
        return write(str);
    });
}

std::string serial_port::read()
{
    if (IsConnected() == false)
        throw std::exception("[read erorr] connection error!");

    std::string read_string;
    OVERLAPPED ov_read = { 0 };

    while (m_bDisconnecting == false)
    {
        static constexpr size_t buffer_size = 1024;
        std::array<char, buffer_size> buffer{ 0, };
        // hFile 이 FILE_FLAG_OVERLAPPED 로 열린 경우 lpNumberOfBytesRead (4번째 인자) 는 NULL 로 설정 되어야한다.
        // -> 잠재적으로 잘못된 결과를 피하기 위해 비동기 작업의 경우 이 매개변수를 NULL 로 사용해야한다.
        // ref : https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
        BOOL result = ReadFile(m_hSerial, buffer.data(), buffer_size, NULL, &ov_read);
        if (result == TRUE)
        {
            if (ov_read.InternalHigh == 0)
                break;
            read_string += std::string(buffer.data(), ov_read.InternalHigh);
        }
        else
        {
            DWORD error = GetLastError();
            if (error == ERROR_IO_PENDING)
            {
                DWORD transferred_byte = 0;
                BOOL overlapped_result = GetOverlappedResult(m_hSerial, &ov_read, &transferred_byte, TRUE);
                error = GetLastError();
                if (overlapped_result == false)
                {
                    // 비정상 종료
                    if (error == ERROR_ACCESS_DENIED)
                        throw std::system_error(std::error_code(error, std::system_category()), "[read error]");

                    // 정상 종료
                    if (m_bDisconnecting || error == ERROR_OPERATION_ABORTED)
                        break;
                }

                if (transferred_byte == 0)
                    break;
                read_string += std::string(buffer.data(), transferred_byte);
            }
            else
                throw std::system_error(std::error_code(error, std::system_category()), "[read error]");
        }
    }

    if (read_string.empty())
        throw std::exception("[read erorr] no data");

    return read_string;
}

std::string serial_port::read_until_flag(const std::string& endflag)
{
    std::string str;
    do
    {
        str += read();
    } while (isEndflag(str, endflag) == false);

    return str;
}

std::future<std::string> serial_port::async_read()
{
    return std::async(std::launch::async, [&]() {
        return read();
    });
}

std::future<std::string> serial_port::async_read_until_flag(const std::string& endflag)
{
    return std::async(std::launch::async, [&, endflag]() {
        std::string str;
        do
        {
            str += read();
        } while (isEndflag(str, endflag) == false);

        return str;
    });
}

bool serial_port::init_serialport_handle(const uint32_t nPort)
{
    std::string portName = "\\\\.\\COM" + std::to_string(nPort);

    m_hSerial = CreateFile(portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0);

    if (m_hSerial == INVALID_HANDLE_VALUE)
    {
        m_hSerial = nullptr;
        // 포트가 존재하지 않는 경우 포트를 열려고 시도한 오류
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
            throw std::exception(std::format("[ERROR] Handle was not attached. Reasom: {} not available.", portName).c_str());
        return false;
    }
    return true;
}

bool serial_port::init_dcb(const uint32_t nBaudrate)
{
    DCB dcbSerialParams = { 0 };

    if (GetCommState(m_hSerial, &dcbSerialParams) == false)
        return false;

    dcbSerialParams.DCBlength = sizeof(DCB);
    dcbSerialParams.BaudRate = nBaudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

    if (SetCommState(m_hSerial, &dcbSerialParams) == false)
        return false;

    return true;
}

bool serial_port::init_timeout()
{
    // read, write 시 timeout 시간 설정 공식 참고
    // ref : https://www.lookrs232.com/com_port_programming/api_commtimeouts.htm
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    // 통신 장치에서 읽을 때, 제한 시간 값을 설정하지 않으면 예측할 수 없는 결과가 발생할 수 있음.
    // ref : https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
    if (SetCommTimeouts(m_hSerial, &timeouts) == false)
        return false;
    return true;
}

bool serial_port::clear_commbuffer()
{
    return PurgeComm(m_hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}

void serial_port::close_serialport_handle()
{
    if (IsConnected())
    {
        if (CloseHandle(m_hSerial) == false)
            throw std::exception("close serial port handle fail");
        m_hSerial = nullptr;
    }
}

bool serial_port::isEndflag(const std::string& str, const std::string& endflag) const noexcept
{
    if (str.empty())
        return false;

    size_t pos = str.rfind(endflag);
    if (pos == std::string::npos)
        return false;

    return str.size() == (pos + endflag.size());
}
