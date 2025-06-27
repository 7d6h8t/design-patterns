#pragma once
#include <string>

//DbgHelp header의 Warning 회피를 위한 추가.
#pragma warning(push)
#pragma warning(disable:4091)
#include <DbgHelp.h>
#pragma warning(pop)

class dumpManager final
{
private:
    using fMiniDumpWriteDump = BOOL(WINAPI*)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE,
        PMINIDUMP_EXCEPTION_INFORMATION,
        PMINIDUMP_USER_STREAM_INFORMATION,
        PMINIDUMP_CALLBACK_INFORMATION);

private:
    dumpManager() = delete;
    ~dumpManager() = delete;

public:
    //Debug가 걸려 있지 않을때 미처리 Exception(ex.STATUS_ACCESS_VIOLATION(0xC0000005)) 발생시
    //memory Dump를 위한 Handle 등록
    static void registerDumpHandle();

private:
    //Unhandled Exception Handler
    static LONG WINAPI crashHandlerOnUnhandledException(PEXCEPTION_POINTERS ExceptionInfo);
    //CrashHandler
    static void crashHandlerOnTerminate();
    //Load Windows System DLL 
    static HMODULE loadSystemLibrary(const std::wstring& wsDllName) noexcept;
    static std::wstring getDumpPath();
    static std::wstring getModulePath();

private:
    static const std::wstring wsDumpFolderName;
};
