#pragma once

class CoordinateProvider {
 public:
  virtual RECT ScreenToPhysical(const RECT& screen) = 0;
};
