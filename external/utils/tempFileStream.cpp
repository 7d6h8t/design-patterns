#include "precompile.h"

using namespace utils;

tempFileStream::tempFileStream(const std::wstring& filePath)
    : state_(none_error), fileHandle_(nullptr)
{
    HANDLE fileHandle = CreateFileW(
        filePath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,    //자동 삭제됨으로 read share를 열어서 복사할 기회제공.
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, // 가능한 한 기록 안함 + 닫을때 자동 삭제
        nullptr
    );

    if (INVALID_HANDLE_VALUE == fileHandle)
    {
        state_ = GetLastError();
        return;
    }

    fileHandle_ = fileHandle;
}

tempFileStream::~tempFileStream()
{
    close();
}

tempFileStream::tempFileStream(tempFileStream&& rhs)
{
    //rhs가 open 되있는지 체크해야되나? 
    //client에서 판단할 문제로 체크 하지 않는다.
    //file handle의 소유권 이전.
    fileHandle_ = rhs.fileHandle_;
    state_ = rhs.state_;
    rhs.clear();
}

tempFileStream& tempFileStream::operator=(tempFileStream&& rhs)
{
    //기존에 가지고 있던 file 핸들 반납한다.
    close();
    fileHandle_ = rhs.fileHandle_;
    state_ = rhs.state_;
    rhs.clear();

    return *this;
}


bool tempFileStream::is_open() const noexcept
{
    if (fileHandle_ == nullptr || fileHandle_ == INVALID_HANDLE_VALUE)
        return false;

    return true;
}

uint64_t tempFileStream::state() const noexcept
{
    return state_;
}

void tempFileStream::flush()
{
    if (is_open() == false)
        return;

    FlushFileBuffers(fileHandle_);
}

void tempFileStream::close()
{
    if (is_open() == false)
    {
        clear();
        return;
    }

    CloseHandle(fileHandle_);
    clear();
}

void tempFileStream::clear()
{
    state_ = none_error;
    fileHandle_ = nullptr;
}
