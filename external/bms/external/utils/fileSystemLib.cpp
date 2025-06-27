#include "precompile.h"
#include <filesystem>
#include <sstream>
#include <chrono>
#include <array>

using namespace utils;
namespace fs = std::filesystem;

fileSystemLib::stringtype fileSystemLib::GetFileExtension(const stringtype& path) noexcept
{
    if (false == IsFile(path))
        return stringtype();
    stringtype strFileExt;
    try
    {
        strFileExt = fs::path(path).extension();
    } catch (const std::exception& e)
    {
        DebugPrint("GetFileExtension Fail[{}]", e.what());
        strFileExt.clear();
    }

    return strFileExt;
}

fileSystemLib::stringtype fileSystemLib::GetFileName(const stringtype& path) noexcept
{
    if (false == IsFile(path))
        return stringtype();
    stringtype strFileName;
    try
    {
        strFileName = fs::path(path).filename();
    } catch (const std::exception& e)
    {
        DebugPrint("GetFileName Fail[{}]", e.what());
        strFileName.clear();
    }

    return strFileName;
}

fileSystemLib::stringtype fileSystemLib::GetParentDirPath(const stringtype& path) noexcept
{
    if (true == path.empty())
        return stringtype();

    stringtype strDirPath;
    try
    {
        strDirPath = fs::path(path).parent_path();
    } catch (const std::exception& e)
    {
        DebugPrint("GetParentDirPath Fail[{}]", e.what());
        strDirPath.clear();
    }

    return strDirPath;
}

fileSystemLib::stringtype fileSystemLib::GetAbsolutePath(const stringtype& basePath, const stringtype& relativePath) noexcept
{
    if (basePath.empty() || relativePath.empty())
        return stringtype();

    stringtype strAbsoultePath;
    stringtype baseDirPath;
    bool isTempDir = false;

    try
    {
        fs::path orgPath = std::filesystem::current_path();

        if (IsDir(basePath) == false)
        {
            baseDirPath = GetParentDirPath(basePath);
            if (IsExists(baseDirPath) == false)
            {
                fs::create_directories(baseDirPath);
                isTempDir = true;
            }
        }
        else
        {
            baseDirPath = basePath;
            if (IsExists(baseDirPath) == false)
            {
                fs::create_directories(baseDirPath);
                isTempDir = true;
            }
        }
        fs::current_path(baseDirPath);

        strAbsoultePath = fs::absolute(relativePath);

        std::filesystem::current_path(orgPath);
    } catch (const std::exception& e)
    {
        DebugPrint("GetAbsolutePath Fail[{}]", e.what());
        strAbsoultePath.clear();
    }

    if(isTempDir)
        RemoveDir(baseDirPath);

    return strAbsoultePath;
}

fileSystemLib::stringtype fileSystemLib::GetRelativePath(const stringtype& basePath, const stringtype& absolutePath) noexcept
{
    if (basePath.empty() || absolutePath.empty())
        return stringtype();

    stringtype strRelativePath;
    try
    {
        strRelativePath = fs::relative(absolutePath, basePath);
    } catch (const std::exception& e)
    {
        DebugPrint("GetRelativePath Fail[{}]", e.what());
        strRelativePath.clear();
    }

    return strRelativePath;
}

bool fileSystemLib::RenameFile(const stringtype& oldPath, const stringtype& newPath) noexcept
{
    //기존 파일이 없으면 에러
    if (false == IsFile(oldPath))
        return false;
    //동일한 경우 true 처리
    if (oldPath == newPath)
        return true;
    //바뀔이름의 경로가 이미 존재하면 에러
    if (true == IsExists(newPath))
        return false;

    try
    {
        fs::rename(oldPath, newPath);
    } catch (const std::exception& e)
    {
        DebugPrint("RenameFile Fail[{}]", e.what());
        return false;
    }

    return true;
}

bool fileSystemLib::CreateDir(const stringtype& path) noexcept
{
    if (true == IsDir(path))
        return true;
    if (true == IsExists(path))
        return false;

    try
    {
        if (false == fs::create_directories(path))
            return false;
    } catch (const std::exception& e)
    {
        DebugPrint("CreateDirectory Fail[{}]", e.what());
        return false;
    }
    return true;
}

bool fileSystemLib::RemoveDir(const stringtype& path) noexcept
{
    if (false == IsDir(path))
        return false;

    try
    {
        if (false == fs::remove_all(path))
            return false;
    } catch (const std::exception& e)
    {
        DebugPrint("RemoveDirectory Fail[{}]", e.what());
        return false;
    }
    return true;
}

bool fileSystemLib::RemoveFile(const stringtype& path) noexcept
{
    if (false == IsFile(path))
        return false;

    try
    {
        if (false == fs::remove(path))
            return false;
    } catch (const std::exception& e)
    {
        DebugPrint("RemoveFile Fail[{}]", e.what());
        return false;
    }

    return true;
}

bool fileSystemLib::IsExists(const stringtype& path) noexcept
{
    if (path.empty())
        return false;

    bool bExists = false;
    try
    {
        bExists = fs::exists(path);
    } catch (const std::exception& e)
    {
        DebugPrint("IsExists Fail[{}]", e.what());
        return false;
    }

    return bExists;
}

bool fileSystemLib::IsFile(const stringtype& path) noexcept
{
    if (path.empty())
        return false;

    bool bFile = false;
    try
    {
        bFile = fs::is_regular_file(path);
    } catch (const std::exception& e)
    {
        DebugPrint("IsFile Fail[{}]", e.what());
        return false;
    }

    return bFile;
}

bool fileSystemLib::IsDir(const stringtype& path) noexcept
{
    if (path.empty())
        return false;

    bool bDir = false;
    try
    {
        bDir = fs::is_directory(path);
    } catch (const std::exception& e)
    {
        DebugPrint("IsDirectory Fail[{}]", e.what());
        return false;
    }

    return bDir;
}

std::list<fileSystemLib::stringtype> fileSystemLib::GetFileList(const stringtype& path, bool bRecursive/* = true*/, bool bIncludeDir/* = false*/) noexcept
{
    //권한 없는 파일 건너뜀
    static constexpr fs::directory_options dirOption = fs::directory_options::skip_permission_denied;

    std::list<stringtype> lstPath;
    if (false == IsDir(path))
        return lstPath;
    static auto fAddPath = [&](const auto &ci) {
        if (bIncludeDir == IsDir(ci) && !bIncludeDir == IsFile(ci))
        {
            lstPath.emplace_back(ci);
        }
    };

    static auto fSkipError = [&](std::error_code& code, const auto& ci) {
        if (code)
        {
            DebugPrintW(L"FileSystem Increment Error Path[{}]", ci.wstring());
            DebugPrint("ErrorCode[{}]", code.message());
            code.clear();
        } else
        {
            fAddPath(ci);
        }
    };

    std::error_code errorCode;
    try
    {
        if (true == bRecursive)
        {
            auto begin = fs::recursive_directory_iterator(path, dirOption);
            auto end = fs::recursive_directory_iterator();
            for (auto& iter = begin; iter !=end; iter.increment(errorCode))
            {
                fSkipError(errorCode, iter->path());
            }
        } else
        {
            auto begin = fs::directory_iterator(path, dirOption);
            auto end = fs::directory_iterator();
            for (auto& iter = begin; iter !=end; iter.increment(errorCode))
            {
                fSkipError(errorCode, iter->path());
            }
        }
    } catch (const std::exception& e)
    {
        DebugPrint("GetFileList Fail[{}]", e.what());
        lstPath.clear();
        return lstPath;
    }
    return lstPath;
}

bool fileSystemLib::GetFileList(const stringtype& path, filelistCallback&& fCallback, bool bRecursive/* = true*/, bool bIncludeDir/* = false*/) noexcept
{
    //권한 없는 파일 건너뜀
    static constexpr fs::directory_options dirOption = fs::directory_options::skip_permission_denied;

    if (fCallback == nullptr)
        return false;

    if (false == IsDir(path))
        return false;

    static auto fAddPath = [&](const auto &ci) -> bool {
        if (bIncludeDir == IsDir(ci) && !bIncludeDir == IsFile(ci))
        {
            return fCallback(ci);
        }
        return true;
    };

    static auto fSkipError = [&](std::error_code& code, const auto& ci) -> bool {
        if (code)
        {
            DebugPrintW(L"FileSystem Increment Error Path[{}]", ci.wstring());
            DebugPrint("ErrorCode[{}]", code.message());
            code.clear();
        } else
        {
            return fAddPath(ci);
        }
        return true;
    };

    std::error_code errorCode;
    try
    {
        if (true == bRecursive)
        {
            auto begin = fs::recursive_directory_iterator(path, dirOption);
            auto end = fs::recursive_directory_iterator();
            for (auto& iter = begin; iter !=end; iter.increment(errorCode))
            {
                if (false == fSkipError(errorCode, iter->path()))
                    return false;
            }
        } else
        {
            auto begin = fs::directory_iterator(path, dirOption);
            auto end = fs::directory_iterator();
            for (auto& iter = begin; iter !=end; iter.increment(errorCode))
            {
                if (false == fSkipError(errorCode, iter->path()))
                    return false;
            }
        }
    } catch (const std::exception& e)
    {
        DebugPrint("GetFileList Fail[{}]", e.what());
        return false;
    }

    return true;
}

std::wstring utils::fileSystemLib::GetModuleFilePath()
{
    static constexpr std::size_t MAX_FILE_NAME = 2048;
    std::wstring wsFileName(MAX_FILE_NAME, 0);
    ::GetModuleFileNameW(GetModuleHandle(NULL), const_cast<LPWSTR>(wsFileName.data()), MAX_FILE_NAME);

    return std::move(wsFileName);
}

std::wstring utils::fileSystemLib::GetModuleDirectory(bool bIncludeLastSep)
{
    static constexpr wchar_t wSeparator = L'\\';
    const std::wstring wsFileName = utils::fileSystemLib::GetModuleFilePath();
    if (wsFileName.empty() == true)
        throw std::logic_error("common::GetModuleFileName Error");

    std::size_t nLastPos = wsFileName.find_last_of(wSeparator);
    if (bIncludeLastSep)
        ++nLastPos;

    return std::move(std::wstring(wsFileName.cbegin(), wsFileName.cbegin() + nLastPos));
}

std::time_t utils::fileSystemLib::GetFileTime(const stringtype& path)
{
    using namespace std::chrono;
    auto ftime = fs::last_write_time(path);

    auto systemtime = time_point_cast<system_clock::duration>(ftime - decltype(ftime)::clock::now()
                                                              + system_clock::now());
    return system_clock::to_time_t(systemtime);
}

utils::fileSystemLib::stringtype utils::fileSystemLib::temp_file_path(const stringtype& dir, const stringtype& prefix)
{
    namespace fs = std::filesystem;
    stringtype target_dir = dir;
    if (target_dir.empty())
        target_dir = fs::temp_directory_path();

    std::array<wchar_t, MAX_PATH> temp_path = { 0, };
    const auto ret = ::GetTempFileNameW(target_dir.c_str(), prefix.c_str(), 0, temp_path.data());
    if (ret == 0)
    {
        DebugPrint("GetTempFileName API Error [{:#10x}]", GetLastError());
        return stringtype();
    }

    return temp_path.data();
}
