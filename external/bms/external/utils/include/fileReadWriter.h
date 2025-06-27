#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "tempFileStream.h"

namespace utils {
    class fileReadWriter final
    {
    public:
        template<typename path_t, typename str_t = std::string>
            static str_t readFileToString(const path_t& filePath)
        {
            str_t contents;
            if (false == readFileToString(filePath, contents))
                std::logic_error("fileToString Error");
            return std::move(contents);
        }
        template<typename path_t, typename str_t,
            typename char_type = typename str_t::value_type>
            static bool readFileToString(const path_t& filePath, str_t& contents)
        {
            std::basic_ifstream<char_type> ReadFile(filePath);
            if (ReadFile.is_open() == false)
                return false;

            ReadFile.seekg(0, std::ios::end);
            std::streamoff length = ReadFile.tellg();
            if (length == -1)
            {
                ReadFile.close();
                return false;
            }
            contents.reserve(static_cast<size_t>(length));
            ReadFile.seekg(0, std::ios::beg);

            contents.assign((std::istreambuf_iterator<char_type>(ReadFile)),
                            std::istreambuf_iterator<char_type>());

            ReadFile.close();
            return true;
        }

        template<typename path_t, typename str_t,
            typename char_type = typename str_t::value_type>
            static bool writeStringToFile(const path_t& filePath, const str_t& contents)
        {
            //SMB(server)상의 파일 Write 시에 Read | Write 옵션으로 createFile 호출해야 더빠르다.
            //test시 대략 3배정도 빠름.
            //https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea#files
            namespace fs = std::filesystem;
            std::ios_base::openmode open_mode = std::ios::out | std::ios::trunc;
            if (fs::exists(filePath))
                open_mode |= std::ios::in;

            std::basic_ofstream<char_type> write_file(filePath, open_mode);
            if (write_file.is_open() == false)
                return false;
            write_file.clear();
            write_file << contents;
            write_file.close();
            return true;
        }

        //temp file에 한번 쓴다음에 타겟으로 복사한다.
        //저장중에 뻑나는 경우 대비위함.
        template<typename path_t, typename str_t,
            typename char_type = typename str_t::value_type>
            static bool writeStringToFile(const path_t& temp_dir, const path_t& filePath, const str_t& contents)
        {
            const path_t temp_path = utils::fileSystemLib::temp_file_path(temp_dir);
            if (temp_path.empty())
                return false;
            utils::tempFileStream temp_stream(temp_path);
            if (temp_stream.is_open() == false)
                return false;
            temp_stream << contents;
            temp_stream.flush();
            BOOL copy_ret = ::CopyFileW(reinterpret_cast<LPCWSTR>(temp_path.c_str()),
                                       reinterpret_cast<LPCWSTR>(filePath.c_str()), FALSE);
            if (copy_ret == FALSE)
                return false;

            return true;
        }
    };
}//namespace utils
