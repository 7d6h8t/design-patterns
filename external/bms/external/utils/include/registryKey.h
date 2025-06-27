#pragma once

#include <string>
#include <vector>
#include <list>
#include "registryKeyType.h"

namespace utils {
    class registryKey final
    {
    public:
        registryKey() noexcept;
        ~registryKey() noexcept;

        registryKey(registryKey&& rhs) noexcept;
        registryKey& operator=(registryKey&& rhs) noexcept;

        //none copyable
        registryKey(const registryKey&) = delete;
        registryKey& operator=(const registryKey&) = delete;

        //for treat HKEY
    public:
        // Create include ROOT
        bool Create(HKEY hKey, const std::wstring& wsSubKey, registryDesiredAccess desiredAccess);
        bool Create(registryRootKey rootKey, const std::wstring& wsSubKey, registryDesiredAccess desiredAccess);

        // Open include ROOT
        bool Open(HKEY hKey, const std::wstring& wsSubKey, registryDesiredAccess desiredAccess);
        bool Open(registryRootKey rootKey, const std::wstring& wsSubKey, registryDesiredAccess desiredAccess);

        void Attach(HKEY hKey) noexcept;
        HKEY Detach() noexcept;
        HKEY Handle() const noexcept;
        //Closes a handle to the specified registry key.
        void Close() noexcept;
        bool IsOpened() const noexcept;

        //For get/set Value
        //ref https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types
    public:
        // REG_SZ 및 REG_EXPAND_SZ 쿼리
        // REG_EXPAND_SZ 타입 여부에 상관없이 확장 여부에 따라 환경변수 확장 수행
        // bDoExpend : ExpandEnvironmentString 을 이용한 확장
        // ex) %PATH% => C:\\\\folder
        bool GetString(const std::wstring& wsValueName, std::wstring& wsValue, bool bDoExpand = false) const;
        // REG_MULTI_SZ 쿼리
        bool GetStringMulti(const std::wstring& wsValueName, std::list<std::wstring>& lstValues) const;
        // 모든 타입을 바이너리 데이터로 쿼리
        bool GetBinary(const std::wstring& wsValueName, std::vector<uint8_t>& vValue) const;
        // 모든 타입을 바이너리 데이터로 쿼리 + 타입
        bool GetBinary(const std::wstring& wsValueName, std::vector<uint8_t>& vValue, registryValueType& valueType) const;
        // REG_DWORD 쿼리
        bool GetUInt32(const std::wstring& wsValueName, uint32_t& nValue) const;
        // REG_QWORD 쿼리
        bool GetUInt64(const std::wstring& wsValueName, uint64_t& nValue) const;
        // bool type 없지만 편의성을 위해서 REG_DWORD를 이용하여 bool 값 제공한다.
        bool GetBool(const std::wstring& wsValueName, bool& bValue) const;

        // REG_SZ 설정
        bool SetString(const std::wstring& wsValueName, const std::wstring& wsValue);
        // REG_SZ 또는 REG_EXPAND_SZ 설정
        bool SetString(const std::wstring& wsValueName, const std::wstring& wsValue, registryValueType valueType);
        // REG_MULTI_SZ 설정
        // "String1\0String2\0"와 같은 형식이어야 함
        bool SetStringMulti(const std::wstring& wsValueName, const std::wstring& wsValues);
        // REG_MULTI_SZ 설정
        bool SetStringMulti(const std::wstring& wsValueName, const std::list<std::wstring>& lstValues);
        // REG_BINARY 설정
        bool SetBinary(const std::wstring& wsValueName, const std::vector<uint8_t>& vValue);
        // 형식 제한 없음 + 타입
        bool SetBinary(const std::wstring& wsValueName, const std::vector<uint8_t>& vValue, registryValueType valueType);
        // REG_DWORD
        bool SetUInt32(const std::wstring& wsValueName, uint32_t nValue);
        // REG_QWORD
        bool SetUInt64(const std::wstring& wsValueName, uint64_t nValue);
        // bool type 없지만 편의성을 위해서 REG_DWORD를 이용하여 bool 값 제공한다.
        bool SetBool(const std::wstring& wsValueName, bool bValue);

    private:
        // 로우 레벨
        bool GetValue(const std::wstring& wsValueName, registryValueType* pValueType, void* pValue, uint32_t* pnValueBytes) const;
        bool SetValue(const std::wstring& wsValueName, registryValueType valueType, const void* pValue, uint32_t nValueBytes);

    private:
        static HKEY getKey(const registryRootKey root) noexcept;

    private:
        HKEY m_hKey;
    };
} //namespace utils

