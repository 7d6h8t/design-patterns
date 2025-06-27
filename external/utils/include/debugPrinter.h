#pragma once
#include <Windows.h>
#include <functional>
#include <memory>
#include <mutex>
#include <format>
#include <string_view>
#include <string>

#ifndef DebugPrint
#if defined(_DEBUG) || defined(DEBUGPRINT_FORCE_ENABLE)
#	ifdef DEBUGPRINT_SHOW_FUNCTION_NAME
#		define DebugPrint(format, ...) utils::debugPrinter::GetInstance().LogFunction(format, __FUNCTION__, __VA_ARGS__)
#		define DebugPrintW(format, ...) utils::debugPrinter::GetInstance().LogFunctionW(format, __FUNCTIONW__, __VA_ARGS__)
#		define DebugPrintEx(format, ...) utils::debugPrinter::GetInstance().LogFunction(format, __FUNCTION__, __VA_ARGS__)
#		define DebugPrintExW(format, ...) utils::debugPrinter::GetInstance().LogFunctionW(format, __FUNCTIONW__, __VA_ARGS__)
#		define DebugPrintTrack() utils::debugPrintTracker __debugprint_tracker(__FUNCTION__)
#	else//DEBUGPRINT_SHOW_FUNCTION_NAME
#		define DebugPrint(format, ...) utils::debugPrinter::GetInstance().Log(format, __VA_ARGS__)
#		define DebugPrintW(format, ...) utils::debugPrinter::GetInstance().LogW(format, __VA_ARGS__)
#		define DebugPrintEx(format, ...) utils::debugPrinter::GetInstance().Log(format, __VA_ARGS__)
#		define DebugPrintExW(format, ...) utils::debugPrinter::GetInstance().LogW(format, __VA_ARGS__)
#		define DebugPrintTrack() utils::debugPrintTracker __debugprint_tracker(__FUNCTION__)
#	endif//DEBUGPRINT_SHOW_FUNCTION_NAME
#else//defined(_DEBUG) || defined(DEBUGPRINT_FORCE_ENABLE)
#	define DebugPrint(format, ...) utils::debugPrinter::GetInstance().Empty(format, __VA_ARGS__)
#	define DebugPrintW(format, ...) utils::debugPrinter::GetInstance().EmptyW(format, __VA_ARGS__)
#	define DebugPrintEx(format, ...) utils::debugPrinter::GetInstance().Log(format, __VA_ARGS__)
#	define DebugPrintExW(format, ...) utils::debugPrinter::GetInstance().LogW(format, __VA_ARGS__)
#	define DebugPrintTrack()
#endif//defined(_DEBUG) || defined(DEBUGPRINT_FORCE_ENABLE)
#endif//DebugPrint

namespace utils {
    class debugPrinter
    {
    public:
        using strPrintFunc = std::function<void(const char*)>;
        using wstrPrintFunc = std::function<void(const wchar_t*)>;

    private:
        debugPrinter() noexcept;
        ~debugPrinter() noexcept = default;

        debugPrinter(const debugPrinter&) = delete;
        debugPrinter& operator=(const debugPrinter&) = delete;

        debugPrinter(debugPrinter&&) = delete;
        debugPrinter& operator=(debugPrinter&&) = delete;

        struct deleter
        {
            void operator()(debugPrinter* pPrinter) noexcept
            {
                delete pPrinter;
                pPrinter = nullptr;
            }
        };

        static uint32_t getCurrentProcessID();

    public:
        static debugPrinter& GetInstance() noexcept
        {
            std::call_once(m_bOnceFlag, []() {
                //make_unique의 경우 deleter 지정 할수 없어서 일반 초기화 사용.(생성자/소멸자 private를 위함.)
                m_pInstance = { new debugPrinter(), deleter() };
                });
            return *m_pInstance;
        }

        void SetApplicationName(const std::string_view pAppname)
        {
            strAppName = pAppname;
            wstrAppName = stringLib::ToUnicodeString(strAppName);
        }

        void SetPrintAFunc(strPrintFunc&& fPrintA)
        {
            fStrPrint = fPrintA;
        }

        void SetPrintWFunc(wstrPrintFunc&& fPrintW)
        {
            fWstrPrint = fPrintW;
        }

        template<class ...Args>
        void LogFunction(const std::string_view pFormat, Args&& ...args)
        {
            const std::string formatter = "{}():: " + std::string(pFormat);
            Log(formatter, std::forward<Args>(args)...);
        }

        template<class ...Args>
        void LogFunctionW(const std::wstring_view pFormat, Args&& ...args)
        {
            const std::wstring formatter = L"{}():: " + std::wstring(pFormat);
            LogW(formatter, std::forward<Args>(args)...);
        }

        template<class ...Args>
        void Log(const std::string_view pFormat, Args&& ...args)
        {
            const std::string formatter = "[{}] " + std::string(pFormat) + (::IsDebuggerPresent() ? "\r\n" : "");
            const std::string strMessage = std::vformat(formatter, std::make_format_args(strAppName, args...));
            LogMessage(strMessage.c_str());
        }

        template<class ...Args>
        void LogW(const std::wstring_view pFormat, Args&& ...args)
        {
            const std::wstring formatter = L"[{}] " + std::wstring(pFormat) + (::IsDebuggerPresent() ? L"\r\n" : L"");
            const std::wstring strMessage = std::vformat(formatter, std::make_wformat_args(wstrAppName, args...));
            LogMessageW(strMessage.c_str());
        }

        void LogMessage(const char* pMessage);
        void LogMessageW(const wchar_t* pMessage);

        template <class... Args>
        void Empty(const std::string_view pFormat, Args&&... args) {
        }

        template <class... Args>
        void EmptyW(const std::wstring_view pFormat, Args&&... args) {            
        }


    private:
        static std::once_flag m_bOnceFlag;
        static std::unique_ptr<debugPrinter, deleter> m_pInstance;

        strPrintFunc fStrPrint;
        wstrPrintFunc fWstrPrint;

        std::string strAppName;
        std::wstring wstrAppName;
    };

    class debugPrintTracker final
    {
    public:
        debugPrintTracker(const char* pszFunction);
        ~debugPrintTracker();
    private:
        const char* m_pszFunction;
    };
} //namespace utils
