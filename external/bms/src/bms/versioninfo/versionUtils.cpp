#include "pch.h"
#include "versionUtils.h"
#include <vector>
#include <format>

std::wstring versionUtils::GetFileVersion(const std::wstring& wsFilePath) noexcept
{
    if (wsFilePath.empty())
        return std::wstring();

    uint64_t nHandleID = 0;
    uint64_t nVersionInfoSize = GetFileVersionInfoSize(wsFilePath.c_str(), reinterpret_cast<LPDWORD>(&nHandleID));
    if (nVersionInfoSize == 0)
        return std::wstring();

    std::vector<byte> vBuffer(nVersionInfoSize, 0);
    GetFileVersionInfo(wsFilePath.c_str(), static_cast<DWORD>(nHandleID), static_cast<DWORD>(nVersionInfoSize), vBuffer.data());


    VS_FIXEDFILEINFO* pVSFileInfo = nullptr;
    uint32_t nVSFileInfoSize = 0;
    VerQueryValue(vBuffer.data(), L"\\", reinterpret_cast<void**>(&pVSFileInfo), &nVSFileInfoSize);
    if (pVSFileInfo == nullptr)
        return std::wstring();

    return std::format(L"{}.{}.{}.{}",
        (pVSFileInfo->dwProductVersionMS >> 16) & 0x0000FFFF,
        pVSFileInfo->dwProductVersionMS & 0x0000FFFF,
        (pVSFileInfo->dwProductVersionLS >> 16) & 0x0000FFFF,
        pVSFileInfo->dwProductVersionLS & 0x0000FFFF);
}
