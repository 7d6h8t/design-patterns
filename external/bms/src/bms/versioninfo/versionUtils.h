/*****************************************************************//**
 * @file   versionUtils.h
 * @brief  util class for get version string
 * 
 * @author aslkdj1
 * @date   March 2023
 *********************************************************************/

#pragma once
#include <string>

/**
 * @brief util class for get version string.
 */
class versionUtils final
{
private:
    versionUtils() = delete;
    ~versionUtils() = delete;

public:
    /**
     * @brief get version string about target *.exe file
     * 
     * @param[in] wsFilePath        target *.exe file path
     * @return                      version string formatted {}.{}.{}.{}
     */
    static std::wstring GetFileVersion(const std::wstring& wsFilePath) noexcept;
};
