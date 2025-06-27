#pragma once

class ResourceProvider {
 public:
  virtual SIZE GetFrameSize() const = 0;
  virtual HWND GetRenderingWindow() const = 0;
};
