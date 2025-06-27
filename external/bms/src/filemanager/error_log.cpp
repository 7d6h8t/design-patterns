#include "pch.h"
#include "include/error_log.h"
#include <utils.h>
#include <fstream>
#include <exception>
#include <chrono>
#include <format>
#include <shlobj.h>

using namespace filemanager;

const std::wstring error_log::m_logFoloderName = L"log";
const std::wstring error_log::m_moduleFileName = L"BMS";
const size_t error_log::m_max_byte = 1024 * 1024; // 1MB

void error_log::log_message(const std::string& message)
{
    const std::string logging_message = get_current_local_timestamp() + ": " + message;

    size_t message_byte = get_string_byte_size(logging_message);
    std::wstring log_dir = get_log_dir_path();
    const auto& log_file_list = get_file_list(log_dir);

    if (log_file_list.empty())
    {
        if (utils::fileSystemLib::IsExists(log_dir) == false)
            utils::fileSystemLib::CreateDir(log_dir);

        std::wstring initial_log_file = get_log_file_path(1);
        utils::fileReadWriter::writeStringToFile(initial_log_file, logging_message);
    }
    else
    {
        std::wstring last_log_file = log_file_list.back();
        size_t last_file_btye_size = get_file_byte_size(last_log_file);

        if (last_file_btye_size + message_byte <= m_max_byte)
        {
            if (append_to_file(last_log_file, logging_message) == false)
                throw std::exception("[log error] append to file fail!");
        }
        else
        {
            std::wstring new_log_file = get_log_file_path(log_file_list.size() + 1);
            utils::fileReadWriter::writeStringToFile(new_log_file, logging_message);
        }
    }
}

size_t error_log::get_string_byte_size(const std::string& str) noexcept
{
    return str.size() * sizeof(char);
}

size_t error_log::get_file_byte_size(const std::wstring& file_path) noexcept
{
    std::string read_data = utils::fileReadWriter::readFileToString(file_path);
    return get_string_byte_size(read_data);
}

error_log::file_list error_log::get_file_list(const std::wstring& folder_path) noexcept
{
    namespace fs = std::filesystem;
    const fs::path folderPath = folder_path;

    file_list list;

    if (fs::exists(folderPath) == false || fs::is_directory(folderPath) == false) 
        return list;

    for (const auto& entry : fs::directory_iterator(folderPath)) 
    {
        if (entry.is_regular_file())
            list.push_back(entry.path().wstring());
    }

    return list;
}

std::wstring error_log::get_local_appdata_path()
{
    WCHAR path[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path);
    std::wstring local_path = path;

    return local_path;
}

std::wstring error_log::get_log_dir_path()
{
    std::wstring log_dir = get_local_appdata_path() + L"\\" + m_moduleFileName + L"\\" + m_logFoloderName + L"\\";
    return log_dir;
}

std::wstring error_log::get_log_file_path(const size_t& file_number)
{
    std::wstring log_dir = get_log_dir_path();
    std::wstring log_file = std::format(L"{}\\error_log_{:0>6}.log", log_dir, file_number);

    return log_file;
}

std::string error_log::get_current_local_timestamp()
{
    const auto now = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }.get_local_time();
    std::string time_stamp = std::format("{0:%Y}-{0:%m}-{0:%d}T{0:%H}:{0:%M}:{0:%S}", now);

    return time_stamp;
}

bool error_log::append_to_file(const std::wstring& file_path, const std::string& str)
{
    std::ofstream outFile(file_path, std::ios::binary | std::ios::app);
    if (outFile.is_open() == false) 
        return false;

    outFile << std::endl << str;
    outFile.close();
    return true;
}

