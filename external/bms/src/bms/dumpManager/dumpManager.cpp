#include "pch.h"
#include "dumpManager.h"
#include <format>
#include <intrin.h>

const std::wstring dumpManager::wsDumpFolderName = L"MiniDumps";

void dumpManager::registerDumpHandle()
{
    SetUnhandledExceptionFilter(crashHandlerOnUnhandledException);
    std::set_terminate(crashHandlerOnTerminate);
}

LONG WINAPI dumpManager::crashHandlerOnUnhandledException(PEXCEPTION_POINTERS ExceptionInfo)
{
    HMODULE dbgHelpDLL = nullptr;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    try
    {
        //1. Load dbghelp.dll from Win32
        dbgHelpDLL = loadSystemLibrary(L"DbgHelp.dll");
        if (dbgHelpDLL == nullptr)
        {
            //Windows XP나 특정 Windows의 경우 dll이 없을 수 있다 
            //다른 App들의 경우 꼼수로 app에 내포하기도 하지만 MSDN에서 재배포 하지 않는걸 권장 하기 때문에
            //해당 부분은 에러로 덤프 뜨지 않는다.
            //https://docs.microsoft.com/en-us/windows/win32/debug/dbghelp-versions
            throw std::logic_error("LoadSystemLibrary(\"DbgHelp.dll\") FAIL");
        }

        //2. GetFunction Address From DLL
        FARPROC lpfnGetProcessID = GetProcAddress(dbgHelpDLL, "MiniDumpWriteDump");
        if (lpfnGetProcessID == nullptr)
            throw std::logic_error("MiniDumpWriteDump Function NULL");
        fMiniDumpWriteDump pMinidumpFunc = reinterpret_cast<fMiniDumpWriteDump>(lpfnGetProcessID);

        //3. Open Dump File
        std::wstring wsDumpFilePath = getDumpPath();
        hFile = CreateFileW(wsDumpFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE)
            throw std::logic_error("Fail Open Dump File");

        MINIDUMP_EXCEPTION_INFORMATION MinidumpExceptionInfo = { 0 };
        MinidumpExceptionInfo.ThreadId = GetCurrentThreadId();
        MinidumpExceptionInfo.ExceptionPointers = ExceptionInfo;
        MinidumpExceptionInfo.ClientPointers = TRUE;

        //4. Write Dump File
        pMinidumpFunc(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hFile,
            (MINIDUMP_TYPE)(MiniDumpNormal),
            &MinidumpExceptionInfo,
            nullptr,
            nullptr);
    } catch (const std::exception&) {
        if (hFile != INVALID_HANDLE_VALUE)
            CloseHandle(hFile);

        if (dbgHelpDLL != nullptr)
            FreeLibrary(dbgHelpDLL);
        //다른 상위 Handler에게 Exception을 던진다.
        return EXCEPTION_CONTINUE_SEARCH;
    }

    if (hFile != INVALID_HANDLE_VALUE)
        CloseHandle(hFile);

    if (dbgHelpDLL != nullptr)
        FreeLibrary(dbgHelpDLL);

    return EXCEPTION_EXECUTE_HANDLER;
}

void dumpManager::crashHandlerOnTerminate()
{
    CONTEXT Context;
    RtlCaptureContext(&Context);

    EXCEPTION_RECORD Exception = { 0 };
#ifdef _WIN64	
    Exception.ExceptionAddress = (PVOID)Context.Rip;
#else	//_WIN64
    Exception.ExceptionAddress = (PVOID)Context.Eip;
#endif	//_WIN64
    Exception.ExceptionCode = ERROR_ACCESS_DENIED;

    EXCEPTION_POINTERS ExceptionInfo = { 0 };
    ExceptionInfo.ExceptionRecord = &Exception;
    ExceptionInfo.ContextRecord = &Context;

    crashHandlerOnUnhandledException(&ExceptionInfo);
}

HMODULE dumpManager::loadSystemLibrary(const std::wstring& wsDllName) noexcept
{
    //GetSystemDiretory String Size
    uint32_t nSize = GetSystemDirectoryW(nullptr, 0);
    if (nSize == 0)
        return nullptr;

    //Get System Path
    std::wstring wsSystemPath;
    wsSystemPath.resize(nSize);
    if (GetSystemDirectoryW(&wsSystemPath[0], static_cast<UINT>(wsSystemPath.size())) == 0)
        return nullptr;

    //Append를 위한 Null 문자 회피를 위한 복사
    std::wstring wsDllPath(wsSystemPath.c_str());

    //Check Last '\\'
    if (wsDllPath.back() != L'\\')
        wsDllPath.push_back(L'\\');

    wsDllPath.append(wsDllName);
    return LoadLibraryExW(wsDllPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
}

std::wstring dumpManager::getDumpPath()
{
    std::wstring wsDumpfolderPath = getModulePath();

    SYSTEMTIME currentTime;
    GetLocalTime(&currentTime);

    std::wstring wsDumpFilePath = std::format(L"{}\\{:0>4}{:0>2}{:0>2}_{:0>2}{:0>2}{:0>2}{:0>3}.dmp",
        wsDumpfolderPath.c_str(),
        currentTime.wYear,
        currentTime.wMonth,
        currentTime.wDay,
        currentTime.wHour,
        currentTime.wMinute,
        currentTime.wSecond,
        currentTime.wMilliseconds
    );

    return wsDumpFilePath;
}

std::wstring dumpManager::getModulePath()
{
    wchar_t wszDumpDirectory[MAX_PATH] = { 0 };
    wszDumpDirectory[GetModuleFileNameW(nullptr, wszDumpDirectory, MAX_PATH)] = L'\0';
    PathRemoveFileSpecW(wszDumpDirectory);
    PathAppendW(wszDumpDirectory, wsDumpFolderName.c_str());
    CreateDirectoryW(wszDumpDirectory, nullptr);

    return std::wstring(wszDumpDirectory);
}
