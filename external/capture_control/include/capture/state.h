#pragma once
#include <cstdint>

namespace capture {
enum class Satate : uint32_t {
  stop = 0,
  play,
  pause,
  recording
};
}  // namespace capture
