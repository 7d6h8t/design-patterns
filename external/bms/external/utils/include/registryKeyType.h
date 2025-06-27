#pragma once

#include <cstdint>

namespace utils {
    //HKLM/HKCU, ...
    enum class registryRootKey : uint32_t
    {
        ClassesRoot = 0,
        CurrentUser,
        LocalMachine,
        Users,

        PerformanceData,
        PerformanceText,
        PerformanceNlsText,

        CurrentConfig,
        DynData,
        CurrentUserLocalSettings,

        Max = CurrentUserLocalSettings
    };

    //DATA TYPE
    enum class registryValueType : uint32_t
    {
        None = REG_NONE,
        String = REG_SZ,							// 유니코드 문자열 (NUL 포함)
        StringExpand = REG_EXPAND_SZ,				// 유니코드 문자열 (NUL 포함), 환경 변수 포함
        Binary = REG_BINARY,						// 바이너리
        UInt32 = REG_DWORD,							// 32비트 부호 없는 정수
        UInt32BigEndian = REG_DWORD_BIG_ENDIAN,		// 32비트 부호 없는 정수, 빅엔디안
        Link = REG_LINK,
        StringMulti = REG_MULTI_SZ,					// 유니코드 문자열 (NUL 포함), String1\0String2\0String3\0LastString\0\0
        UInt64 = REG_QWORD							// 64비트 부호 없는 정수
    };

    //Access MASK
    enum class registryDesiredAccess : uint32_t
    {
        None = 0,

        QueryValue = KEY_QUERY_VALUE,
        SetValue = KEY_SET_VALUE,
        CreateSubKey = KEY_CREATE_SUB_KEY,
        EnumerateSubKeys = KEY_ENUMERATE_SUB_KEYS,
        Notify = KEY_NOTIFY,
        CreateLink = KEY_CREATE_LINK,
        Wow64Key32 = KEY_WOW64_32KEY,
        Wow64Key64 = KEY_WOW64_64KEY,
        Wow64Res = KEY_WOW64_RES,

        Read = KEY_READ,
        Write = KEY_WRITE,
        Execute = KEY_EXECUTE,

        All = KEY_ALL_ACCESS
    };
} //namespace utils
