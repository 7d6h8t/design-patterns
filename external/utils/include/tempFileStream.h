#pragma once
#include <string>

namespace utils {
//임시파일 생성을 위한 stream
//1. 파일 close 시에 자동으로 file 삭제.
//2. Buffer를 이용하여 파일에 Write 최소화
class tempFileStream final
{
public:
    using stream_size = DWORD;
private:
    friend tempFileStream& write_string(tempFileStream&, const char*, const size_t);

public:
    tempFileStream() = delete;
    explicit tempFileStream(const std::wstring& filePath);
    ~tempFileStream();

    //none copyable
    //추후 복사 필요시에는 duplicate handle 을 이용한 파일 핸들 복사 필요
    tempFileStream(const tempFileStream& rhs) = delete;
    tempFileStream& operator=(const tempFileStream& rhs) = delete;

    //moveable
    tempFileStream(tempFileStream&& rhs);
    tempFileStream& operator=(tempFileStream&& rhs);

public:
    bool is_open() const noexcept;
    uint64_t state() const noexcept;
    void flush();
    void close();

private:
    void clear();

    //throw exception
    //invalid_arg : 현재 1byte type만 지원한다.
    //std::ios_base::failure : write file error
    template<class _Elem>
    stream_size write(const _Elem* _ptr, const stream_size _count)
    {
        //TODO: 현재는 1byte type(char, int8_t...) 들만 지워한다.
        //추후에 1byte 가 아닌 타입 필요한 경우에는 Writen함수와 같은 
        //추가 함수를 call 하는 방식으로 처리할 필요가 있다.
        if constexpr (sizeof(_Elem) != 1)
            throw std::invalid_argument("only write 1Byte Type");
        if (is_open() == false)
            return 0;
        if (_ptr == nullptr || _count == 0)
            return 0;
        //이미 이전에 error 발생
        if (state_ != none_error)
            return 0;

        stream_size written_size = 0;
        if (FALSE == ::WriteFile(fileHandle_, reinterpret_cast<LPCVOID>(_ptr), _count,
                                 &written_size, nullptr))
            throw std::ios_base::failure("writefile error");

        return written_size;
    }

private:
    HANDLE fileHandle_;
    uint64_t state_;

public:
    //bitmask type
    static constexpr uint64_t none_error = 0x00;
    static constexpr uint64_t write_error = 0x01;
}; //tempFileStream

template<class _string>
inline 
tempFileStream& operator<<(tempFileStream& _stream, const _string& _str)
{
    return write_string(_stream, _str.c_str(), _str.size());
}

inline
tempFileStream& operator<<(tempFileStream& _stream, const char* _str)
{
    return write_string(_stream, _str, strlen(_str));
}

inline 
tempFileStream& write_string(tempFileStream& _stream, const char* _str, const size_t _len)
{
    using stream_size = tempFileStream::stream_size;
    stream_size str_size = static_cast<stream_size>(_len);

    try
    {
        str_size -= _stream.write(_str, str_size);
        if (str_size != 0)
            throw std::logic_error("write size error");
    } catch (const std::exception& e)
    {
        const char* msg = e.what();
        DebugPrint(msg);
        _stream.state_ |= tempFileStream::write_error;
    }

    return _stream;
}
} //namespace utils
