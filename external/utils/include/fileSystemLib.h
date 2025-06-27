#pragma once
#include <string>
#include <list>
#include <ctime>
#include <functional>

namespace utils {
    class fileSystemLib final
    {
    public:
        using stringtype = std::wstring;
        using filelistCallback = std::function<bool(const stringtype&)>;

    public:
        fileSystemLib() = delete;
        ~fileSystemLib() = delete;

    public:
        static stringtype GetFileExtension(const stringtype& path) noexcept;
        static stringtype GetFileName(const stringtype& path) noexcept;
        static stringtype GetParentDirPath(const stringtype& path) noexcept;
        static stringtype GetAbsolutePath(const stringtype& basePath, const stringtype& relativePath) noexcept;
        static stringtype GetRelativePath(const stringtype& basePath, const stringtype& absolutePath) noexcept;
        static bool RenameFile(const stringtype& oldPath, const stringtype& newPath) noexcept;

        static bool CreateDir(const stringtype& path) noexcept;
        static bool RemoveDir(const stringtype& path) noexcept;
        static bool RemoveFile(const stringtype& path) noexcept;

        static bool IsExists(const stringtype& path) noexcept;
        static bool IsFile(const stringtype& path) noexcept;
        static bool IsDir(const stringtype& path) noexcept;

        static std::list<stringtype> GetFileList(const stringtype& path, bool bRecursive = true, bool bIncludeDir = false) noexcept;
        static bool GetFileList(const stringtype& path, filelistCallback&& fCallback, bool bRecursive = true, bool bIncludeDir = false) noexcept;


        static std::wstring GetModuleFilePath();
        static std::wstring GetModuleDirectory(bool bIncludeLastSep = true);

        static std::time_t GetFileTime(const stringtype& path);
        static stringtype temp_file_path(const stringtype& dir, const stringtype& prefix = L"TMP");
    };//fileSystemLib
}//namespace utils
