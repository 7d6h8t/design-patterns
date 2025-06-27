#include "precompile.h"
#include <cctype>
#include <cwctype>
#include <algorithm>

using namespace utils;

bool stringLib::ToUnicodeString(const char* pszMultiByteString, size_t nMultiByteStringBytes, std::wstring& wsWideString) noexcept
{
    return ToWideString(CP_ACP, pszMultiByteString, nMultiByteStringBytes, wsWideString);
}

std::wstring stringLib::ToUnicodeString(const char* pszMultiByteString, size_t nMultiByteStringBytes)
{
    std::wstring wsOutput;
    if (ToUnicodeString(pszMultiByteString, nMultiByteStringBytes, wsOutput) == false)
        throw std::runtime_error("ToUnicodeString Error");

    return wsOutput;
}

bool stringLib::ToUnicodeString(const std::string& sMultiByteString, std::wstring& wsWideString) noexcept
{
    return ToUnicodeString(sMultiByteString.c_str(), sMultiByteString.size(), wsWideString);
}

std::wstring stringLib::ToUnicodeString(const std::string& sMultiByteString)
{
    std::wstring wsOutput;
    if (ToUnicodeString(sMultiByteString, wsOutput) == false)
        throw std::logic_error("ToUnicodeString FALSE");

    return wsOutput;
}

bool stringLib::FromUTF8String(const char* pszUTF8String, size_t nUTF8StringBytes, std::wstring& wsWideString) noexcept
{
    return ToWideString(CP_UTF8, pszUTF8String, nUTF8StringBytes, wsWideString);
}

std::wstring stringLib::FromUTF8String(const char* pszUTF8String, size_t nUTF8StringBytes)
{
    std::wstring wsOutput;

    if (FromUTF8String(pszUTF8String, nUTF8StringBytes, wsOutput) == false)
        throw std::runtime_error("FromUTF8String");

    return wsOutput;
}

bool stringLib::FromUTF8String(const std::string& sUTF8String, std::wstring& wsWideString) noexcept
{
    return FromUTF8String(sUTF8String.c_str(), sUTF8String.size(), wsWideString);
}

std::wstring stringLib::FromUTF8String(const std::string& sUTF8String)
{
    std::wstring wsOutput;

    if (FromUTF8String(sUTF8String, wsOutput) == false)
        throw std::runtime_error("FromUTF8String");

    return wsOutput;
}

bool stringLib::ToWideString(uint32_t nCodePage, const char* pszMultiByteString, size_t nMultiByteStringBytes, std::wstring& wsWideString) noexcept
{
    //Init Output String
    wsWideString.clear();

    //Check Null ptr
    if (nullptr == pszMultiByteString)
        return false;

    //Check empty
    if ('\0' == pszMultiByteString[0] &&
        0 == nMultiByteStringBytes)
        return true;

    //Recalc Size
    if (0 == nMultiByteStringBytes)
        nMultiByteStringBytes = strlen(pszMultiByteString);

    //Calc Wchar Size
    const int32_t nRequiredLength = MultiByteToWideChar(nCodePage, 0, pszMultiByteString, (int)nMultiByteStringBytes, nullptr, 0);
    if (0 > nRequiredLength)
        return false;
    wsWideString.resize(nRequiredLength);

    //Convert Multibytes to Wchar
    const int32_t nConvertedLength = MultiByteToWideChar(nCodePage, 0, pszMultiByteString, (int)nMultiByteStringBytes, (LPWSTR)&wsWideString[0], nRequiredLength);
    if (0 > nConvertedLength)
    {
        //Fail
        wsWideString.clear();
        return false;
    }

    return true;
}

bool stringLib::ToMultiByteString(const wchar_t* pwszWideString, size_t nWideStringLength, std::string& sMultiByteString) noexcept
{
    return ToMultiByteString(CP_ACP, pwszWideString, nWideStringLength, sMultiByteString);
}

std::string stringLib::ToMultiByteString(const wchar_t* pwszWideString, size_t nWideStringLength)
{
    std::string strOutput;
    if (ToMultiByteString(pwszWideString, nWideStringLength, strOutput) == false)
        throw std::logic_error("ToMultiByteString FALSE");

    return strOutput;
}

bool stringLib::ToMultiByteString(const std::wstring& wsWideString, std::string& sMultiByteString) noexcept
{
    return ToMultiByteString(wsWideString.c_str(), wsWideString.size(), sMultiByteString);
}

std::string stringLib::ToMultiByteString(const std::wstring& wsWideString)
{
    std::string strOutput;
    if (ToMultiByteString(wsWideString, strOutput) == false)
        throw std::logic_error("ToMultiByteString FALSE");

    return strOutput;
}

bool stringLib::ToUTF8String(const wchar_t* pwszWideString, size_t nWideStringLength, std::string& sUTF8String) noexcept
{
    return ToMultiByteString(CP_UTF8, pwszWideString, nWideStringLength, sUTF8String);
}

std::string stringLib::ToUTF8String(const wchar_t* pwszWideString, size_t nWideStringLength)
{
    std::string strOutput;
    if (ToUTF8String(pwszWideString, nWideStringLength, strOutput) == false)
        throw std::logic_error("ToUTF8String FALSE");

    return strOutput;
}

bool stringLib::ToUTF8String(const std::wstring& wsWideString, std::string& sUTF8String) noexcept
{
    return ToUTF8String(wsWideString.c_str(), wsWideString.size(), sUTF8String);
}

std::string stringLib::ToUTF8String(const std::wstring& wsWideString)
{
    std::string strOutput;
    if (ToUTF8String(wsWideString, strOutput) == false)
        throw std::logic_error("ToUTF8String FALSE");
    return strOutput;
}

bool stringLib::ToMultiByteString(uint32_t nCodePage, const wchar_t* pwszWideString, size_t nWideStringLength, std::string& sMultiByteString) noexcept
{
    //Init OutputString
    sMultiByteString.clear();

    //Check Nullptr
    if (nullptr == pwszWideString)
        return false;

    //Check Empty String
    if (L'\0' == pwszWideString[0] &&
        0 == nWideStringLength)
        return true;

    //Recalc Input Size
    if (0 == nWideStringLength)
        nWideStringLength = wcslen(pwszWideString);

    //Calc Output String Size
    int nRequiredBytes = WideCharToMultiByte(nCodePage, 0, pwszWideString, (int)nWideStringLength, nullptr, 0, nullptr, nullptr);
    if (0 > nRequiredBytes)
        return false;
    sMultiByteString.resize(nRequiredBytes);

    //Wchar -> Char
    int nConvertedBytes = WideCharToMultiByte(nCodePage, 0, pwszWideString, (int)nWideStringLength, (LPSTR)&sMultiByteString[0], nRequiredBytes, nullptr, nullptr);
    if (0 > nConvertedBytes)
    {
        //fail
        sMultiByteString.clear();
        return false;
    }

    return true;
}

void stringLib::to_upper(std::string& str)
{
    std::for_each(str.begin(), str.end(), [](char& c) {
        c = std::toupper(c);
    });
}

std::string stringLib::to_upper(const std::string& str)
{
    std::string upper = str;
    to_upper(upper);
    return upper;
}

void stringLib::to_lower(std::string& str)
{
    std::for_each(str.begin(), str.end(), [](char& c) {
        c = std::tolower(c);
    });
}

std::string stringLib::to_lower(const std::string& str)
{
    std::string lower = str;
    to_lower(lower);
    return lower;
}

void stringLib::to_wupper(std::wstring& wstr)
{
    std::for_each(wstr.begin(), wstr.end(), [](wchar_t& wchar) {
        wchar = std::towupper(wchar);
    });
}

std::wstring stringLib::to_wupper(const std::wstring& wstr)
{
    std::wstring upper = wstr;
    to_wupper(upper);
    return upper;
}

void stringLib::to_wlower(std::wstring& wstr)
{
    std::for_each(wstr.begin(), wstr.end(), [](wchar_t& wchar) {
        wchar = std::towlower(wchar);
    });
    
}
std::wstring stringLib::to_wlower(const std::wstring& wstr)
{
    std::wstring lower = wstr;
    to_wlower(lower);
    return lower;
}
