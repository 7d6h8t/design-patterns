#pragma once
#include <windef.h>
#include <memory>

struct Frame {
  std::unique_ptr<BYTE[]> bits_ = nullptr;
  SIZE size_ = {};
  int32_t pitch_ = 0;
};
