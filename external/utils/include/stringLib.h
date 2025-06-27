#pragma once

#include <string>
#include <stdarg.h>
#include <stdexcept>


namespace utils {
    class stringLib final
    {
    public:
        stringLib() = delete;
        ~stringLib() = delete;

    public:
        //////////////////////////////////////////////////////////////////////////
        //  Convert String Code Page 
        //////////////////////////////////////////////////////////////////////////

        //Char(ANSI) -> WString(Unicode)
        static bool ToUnicodeString(const char* pszMultiByteString, size_t nMultiByteStringBytes, std::wstring& wsWideString) noexcept;
        static std::wstring ToUnicodeString(const char* pszMultiByteString, size_t nMultiByteStringBytes);
        static bool ToUnicodeString(const std::string& sMultiByteString, std::wstring& wsWideString) noexcept;
        static std::wstring ToUnicodeString(const std::string& sMultiByteString);

        //Char(UTF-8) -> WString(Unicode)
        static bool FromUTF8String(const char* pszUTF8String, size_t nUTF8StringBytes, std::wstring& wsWideString) noexcept;
        static std::wstring FromUTF8String(const char* pszUTF8String, size_t nUTF8StringBytes);
        static bool FromUTF8String(const std::string& sUTF8String, std::wstring& wsWideString) noexcept;
        static std::wstring FromUTF8String(const std::string& sUTF8String);

        //MutiByte To Wide String
        static bool ToWideString(uint32_t nCodePage, const char* pszMultiByteString, size_t nMultiByteStringBytes, std::wstring& wsWideString) noexcept;

        //WString(Unicode) -> Char(ANSI)
        static bool ToMultiByteString(const wchar_t* pwszWideString, size_t nWideStringLength, std::string& sMultiByteString) noexcept;
        static std::string ToMultiByteString(const wchar_t* pwszWideString, size_t nWideStringLength);
        static bool ToMultiByteString(const std::wstring& wsWideString, std::string& sMultiByteString) noexcept;
        static std::string ToMultiByteString(const std::wstring& wsWideString);

        //WString(Unicode) -> Char(UTF-8)
        static bool ToUTF8String(const wchar_t* pwszWideString, size_t nWideStringLength, std::string& sUTF8String) noexcept;
        static std::string ToUTF8String(const wchar_t* pwszWideString, size_t nWideStringLength);
        static bool ToUTF8String(const std::wstring& wsWideString, std::string& sUTF8String) noexcept;
        static std::string ToUTF8String(const std::wstring& wsWideString);

        //Wide String to MultiByte
        static bool ToMultiByteString(uint32_t nCodePage, const wchar_t* pwszWideString, size_t nWideStringLength, std::string& sMultiByteString) noexcept;
             

        //////////////////////////////////////////////////////////////////////////
        // String Upper/Lower
        //////////////////////////////////////////////////////////////////////////
        static void to_upper(std::string& str);
        static std::string to_upper(const std::string& str);
        static void to_lower(std::string& str);
        static std::string to_lower(const std::string& str);

        static void to_wupper(std::wstring& wstr);
        static std::wstring to_wupper(const std::wstring& wstr);
        static void to_wlower(std::wstring& wstr);
        static std::wstring to_wlower(const std::wstring& wstr);

    };//stringLib
} //namespace utils
