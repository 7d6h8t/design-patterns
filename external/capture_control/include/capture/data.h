#pragma once
#include "../pixel_stats.h"
#include "../../frame_data.h"

namespace capture {
struct FrameData {
  Frame yuv_;
  Frame rgb_;

  PixelInfo y_;
  PixelInfo u_;
  PixelInfo v_;

  PixelInfo r_;
  PixelInfo g_;
  PixelInfo b_;
};
}  // namespace capture