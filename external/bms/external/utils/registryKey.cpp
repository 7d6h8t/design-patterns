#include "precompile.h"

using namespace utils;

registryKey::registryKey() noexcept
    : m_hKey(nullptr)
{

}

registryKey::~registryKey() noexcept
{
    Close();
}

registryKey::registryKey(registryKey&& rhs) noexcept
    : registryKey()
{
    *this = std::move(rhs);
}
registryKey& registryKey::operator=(registryKey&& rhs) noexcept
{
    return *this;
}

bool registryKey::Create(HKEY hKey, const std::wstring& wsSubKey, registryDesiredAccess desiredAccess)
{
    Close();

    //Creates the specified registry key
    if (ERROR_SUCCESS != RegCreateKeyExW(hKey, wsSubKey.c_str(), 0, nullptr, 0, (REGSAM)desiredAccess, nullptr, &m_hKey, nullptr))
        return false;

    return true;
}

bool registryKey::Create(registryRootKey rootKey, const std::wstring& wsSubKey, registryDesiredAccess desiredAccess)
{
    HKEY hRootKey = getKey(rootKey);
    if (nullptr == hRootKey)
        return false;

    return Create(hRootKey, wsSubKey, desiredAccess);
}

bool registryKey::Open(HKEY hKey, const std::wstring& wsSubKey, registryDesiredAccess desiredAccess)
{
    Close();

    //Opens the specified registry key.
    if (ERROR_SUCCESS != RegOpenKeyExW(hKey, wsSubKey.c_str(), 0, (REGSAM)desiredAccess, &m_hKey))
        return false;

    return true;
}

bool registryKey::Open(registryRootKey rootKey, const std::wstring & wsSubKey, registryDesiredAccess desiredAccess)
{
    HKEY hRootKey = getKey(rootKey);
    if (nullptr == hRootKey)
        return false;

    return Open(hRootKey, wsSubKey, desiredAccess);
}

void registryKey::Attach(HKEY hKey) noexcept
{
    Close();
    m_hKey = hKey;
}

HKEY registryKey::Detach() noexcept
{
    HKEY hDetachedKey = m_hKey;
    m_hKey = nullptr;
    return hDetachedKey;
}

HKEY registryKey::Handle() const noexcept
{
    return m_hKey;
}

void registryKey::Close() noexcept
{
    if (IsOpened())
    {
        RegCloseKey(m_hKey);
        m_hKey = nullptr;
    }
}

bool registryKey::IsOpened() const noexcept
{
    return nullptr != m_hKey;
}

bool registryKey::GetString(const std::wstring& wsValueName, std::wstring& wsValue, bool bDoExpand /*= false */) const
{
    registryValueType valueType = registryValueType::None;
    uint32_t nValueBytes = 0;

    if (false == GetValue(wsValueName, &valueType, nullptr, &nValueBytes))
        return false;
    if (registryValueType::String != valueType && registryValueType::StringExpand != valueType)
        return false;

    wsValue.clear();
    //Byte -> length(wchar라서 /2)
    wsValue.resize((nValueBytes / 2) - 1);

    if (false == GetValue(wsValueName, &valueType, reinterpret_cast<void*>(&wsValue[0]), &nValueBytes))
    {
        wsValue.clear();
        return false;
    }

    if (bDoExpand)
    {
        std::wstring wsExpandedValue;

        DWORD dwLength = ExpandEnvironmentStringsW(wsValue.c_str(), nullptr, 0);
        if (0 == dwLength)
            return false;

        wsExpandedValue.resize(dwLength - 1);
        dwLength = ExpandEnvironmentStringsW(wsValue.c_str(), &wsExpandedValue[0], dwLength);
        if (0 == dwLength)
            return false;

        wsValue = std::move(wsExpandedValue);
    }

    return true;
}

bool registryKey::GetStringMulti(const std::wstring& wsValueName, std::list<std::wstring>& lstValues) const
{
    registryValueType valueType = registryValueType::None;
    uint32_t nValueBytes = 0;

    if (false == GetValue(wsValueName, &valueType, nullptr, &nValueBytes))
        return false;
    if (registryValueType::StringMulti != valueType)
        return false;

    std::wstring wsBuffer;
    wsBuffer.resize((nValueBytes / 2) - 1, L'\0');

    if (false == GetValue(wsValueName, &valueType, reinterpret_cast<void*>(&wsBuffer[0]), &nValueBytes))
        return false;

    lstValues.clear();

    size_t nCurrentPos = 0;
    size_t nNulPos = std::wstring::npos;

    while (std::wstring::npos != (nNulPos = wsBuffer.find(L'\0', nCurrentPos)))
    {
        lstValues.emplace_back(std::move(wsBuffer.substr(nCurrentPos, nNulPos - nCurrentPos)));
        nCurrentPos = nNulPos + 1;
    }

    return true;
}

bool registryKey::GetBinary(const std::wstring& wsValueName, std::vector<uint8_t>& vValue) const
{
    registryValueType valueType = registryValueType::None;

    if (false == GetBinary(wsValueName, vValue, valueType))
        return false;

    return true;
}

bool registryKey::GetBinary(const std::wstring& wsValueName, std::vector<uint8_t>& vValue, registryValueType& valueType) const
{
    uint32_t nValueBytes = 0;

    if (false == GetValue(wsValueName, &valueType, nullptr, &nValueBytes))
        return false;

    vValue.clear();
    vValue.resize(nValueBytes);

    if (false == GetValue(wsValueName, &valueType, reinterpret_cast<void*>(vValue.data()), &nValueBytes))
        return false;

    return true;
}

bool registryKey::GetUInt32(const std::wstring& wsValueName, uint32_t& nValue) const
{
    registryValueType valueType = registryValueType::None;
    uint32_t nValueBytes = 0;

    if (false == GetValue(wsValueName, &valueType, nullptr, &nValueBytes))
        return false;
    if (registryValueType::UInt32 != valueType || sizeof(uint32_t) != nValueBytes)
        return false;

    if (false == GetValue(wsValueName, &valueType, reinterpret_cast<void*>(&nValue), &nValueBytes))
        return false;

    return true;
}

bool registryKey::GetUInt64(const std::wstring& wsValueName, uint64_t& nValue) const
{
    registryValueType valueType = registryValueType::None;
    uint32_t nValueBytes = 0;

    if (false == GetValue(wsValueName, &valueType, nullptr, &nValueBytes))
        return false;
    if (registryValueType::UInt64 != valueType && sizeof(uint64_t) != nValueBytes)
        return false;

    if (false == GetValue(wsValueName, &valueType, &nValue, &nValueBytes))
        return false;

    return true;
}

bool registryKey::GetBool(const std::wstring& wsValueName, bool& bValue) const
{
    uint32_t nValue = 0;
    if( false == GetUInt32(wsValueName, nValue))
        return false;

    bValue = (nValue == 1 ? true : false);
    return true;
}

bool registryKey::SetString(const std::wstring& wsValueName, const std::wstring& wsValue)
{
    return SetValue(wsValueName, registryValueType::String, reinterpret_cast<const void*>(wsValue.c_str()), static_cast<uint32_t>((wsValue.size() + 1) * sizeof(wchar_t)));
}

bool registryKey::SetString(const std::wstring& wsValueName, const std::wstring& wsValue, registryValueType valueType)
{
    if (registryValueType::String != valueType && registryValueType::StringExpand != valueType)
        return false;

    return SetValue(wsValueName, valueType, reinterpret_cast<const void*>(wsValue.c_str()), static_cast<uint32_t>((wsValue.size() + 1) * sizeof(wchar_t)));
}

bool registryKey::SetStringMulti(const std::wstring& wsValueName, const std::wstring& wsValues)
{
    return SetValue(wsValueName, registryValueType::StringMulti, reinterpret_cast<const void*>(wsValues.c_str()), static_cast<uint32_t>((wsValues.size() + 1) * sizeof(wchar_t)));
}

bool registryKey::SetStringMulti(const std::wstring& wsValueName, const std::list<std::wstring>& lstValues)
{
    size_t nTotalLength = 0;

    for (const std::wstring& wsValue : lstValues)
        nTotalLength += wsValue.size() + 1;

    std::wstring wsBuffer;
    wsBuffer.reserve(nTotalLength);

    for (const std::wstring& wsValue : lstValues)
    {
        wsBuffer.append(wsValue);
        wsBuffer.push_back(L'\0');
    }

    return SetStringMulti(wsValueName, wsBuffer);
}

bool registryKey::SetBinary(const std::wstring& wsValueName, const std::vector<uint8_t>& vValue)
{
    return SetValue(wsValueName, registryValueType::Binary, reinterpret_cast<const void*>(vValue.data()), static_cast<uint32_t>(vValue.size()));
}

bool registryKey::SetBinary(const std::wstring& wsValueName, const std::vector<uint8_t>& vValue, registryValueType valueType)
{
    return SetValue(wsValueName, valueType, reinterpret_cast<const void*>(vValue.data()), static_cast<uint32_t>(vValue.size()));
}

bool registryKey::SetUInt32(const std::wstring& wsValueName, uint32_t nValue)
{
    return SetValue(wsValueName, registryValueType::UInt32, reinterpret_cast<const void*>(&nValue), sizeof(nValue));
}

bool registryKey::SetUInt64(const std::wstring& wsValueName, uint64_t nValue)
{
    return SetValue(wsValueName, registryValueType::UInt64, reinterpret_cast<const void*>(&nValue), sizeof(nValue));
}

bool registryKey::SetBool(const std::wstring& wsValueName, bool bValue)
{
    return SetUInt32(wsValueName, bValue == true ? 1 : 0);
}

bool registryKey::GetValue(const std::wstring& wsValueName, registryValueType* pValueType, void* pValue, uint32_t* pnValueBytes) const
{
    if (false == IsOpened())
        return false;

    if (ERROR_SUCCESS != RegQueryValueExW(m_hKey, wsValueName.c_str(), 0, reinterpret_cast<LPDWORD>(pValueType), reinterpret_cast<LPBYTE>(pValue), reinterpret_cast<LPDWORD>(pnValueBytes)))
        return false;

    return true;
}

bool registryKey::SetValue(const std::wstring& wsValueName, registryValueType valueType, const void* pValue, uint32_t nValueBytes)
{
    if (false == IsOpened())
        return false;

    if (ERROR_SUCCESS != RegSetValueExW(m_hKey, wsValueName.c_str(), 0, static_cast<DWORD>(valueType), reinterpret_cast<const BYTE*>(pValue), nValueBytes))
        return false;

    return true;
}

HKEY registryKey::getKey(const registryRootKey root) noexcept
{
    static const HKEY RootKeys[] = {
            HKEY_CLASSES_ROOT,
            HKEY_CURRENT_USER,
            HKEY_LOCAL_MACHINE,
            HKEY_USERS,
            HKEY_PERFORMANCE_DATA,
            HKEY_PERFORMANCE_TEXT,
            HKEY_PERFORMANCE_NLSTEXT,
            HKEY_CURRENT_CONFIG,
            HKEY_DYN_DATA,
            HKEY_CURRENT_USER_LOCAL_SETTINGS
    };

    const uint32_t nIndex = static_cast<uint32_t>(root);

    if (nIndex > static_cast<uint32_t>(registryRootKey::Max))
        return nullptr; // 최대 범위보다 큼

    return RootKeys[nIndex];
}
