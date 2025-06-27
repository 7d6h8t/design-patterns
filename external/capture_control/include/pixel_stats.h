#pragma once
#include <cstdint>

struct PixelStats {
  uint32_t sum_;
  uint32_t min_;
  uint32_t max_;
};

struct PixelInfo {
  float avg_;
  uint32_t min_;
  uint32_t max_;
};
