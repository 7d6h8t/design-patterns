#pragma once
#include "filemanager_export.h"
#include <string>
#include <list>

#pragma warning(push)
#pragma warning(disable:4251)

namespace filemanager
{
    class EXPORT error_log
    {
    public:
        using file_list = std::list<std::wstring>;

    public:
        error_log() noexcept = default;
        ~error_log() noexcept = default;

        error_log(const error_log&) = delete;
        error_log& operator=(const error_log&) = delete;

        error_log(error_log&&) = delete;
        error_log& operator=(error_log&&) = delete;

    public:
        void log_message(const std::string& message);

    private:
        size_t get_string_byte_size(const std::string& str) noexcept;
        size_t get_file_byte_size(const std::wstring& file_path) noexcept;
        file_list get_file_list(const std::wstring& folder_path) noexcept;

        std::wstring get_local_appdata_path();
        std::wstring get_log_dir_path();
        std::wstring get_log_file_path(const size_t& file_number);

        std::string get_current_local_timestamp();

        bool append_to_file(const std::wstring& file_path, const std::string& str);

    private:
        static const std::wstring m_logFoloderName;
        static const std::wstring m_moduleFileName;
        static const size_t m_max_byte;
    };
}

#pragma warning(pop)
