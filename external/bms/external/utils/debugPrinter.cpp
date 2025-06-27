#include "precompile.h"

using namespace utils;

std::unique_ptr<debugPrinter, debugPrinter::deleter> debugPrinter::m_pInstance = nullptr;
std::once_flag debugPrinter::m_bOnceFlag;

debugPrinter::debugPrinter() noexcept
    : fStrPrint([](const char* str) {
        ::OutputDebugStringA(str);
    }),
    fWstrPrint([](const wchar_t* str) {
            ::OutputDebugStringW(str);
    }),
    strAppName("PID:" + std::to_string(getCurrentProcessID())),
    wstrAppName(L"PID:" + std::to_wstring(getCurrentProcessID()))
{
}

uint32_t debugPrinter::getCurrentProcessID()
{
    return static_cast<uint32_t>(::GetCurrentProcessId());
}

void debugPrinter::LogMessage(const char* pMessage)
{
    fStrPrint(pMessage);
}

void debugPrinter::LogMessageW(const wchar_t* pMessage)
{
    fWstrPrint(pMessage);
}

debugPrintTracker::debugPrintTracker(const char* pszFunction)
    : m_pszFunction(pszFunction)
{
    debugPrinter::GetInstance().LogFunction("Enter", m_pszFunction);
}

debugPrintTracker::~debugPrintTracker()
{
    debugPrinter::GetInstance().LogFunction("Leave", m_pszFunction);
}
