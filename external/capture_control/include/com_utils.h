#pragma once
#include <exception>

namespace com {
// Helper class for COM exceptions
class com_exception : public std::exception {
 public:
  com_exception(HRESULT hr) : result(hr) {}

  const char* what() const noexcept override;
 private:
  HRESULT result;
};

// Helper utility converts D3D API failures into exceptions.
void ThrowIfFailed(HRESULT hr);

#ifdef _DEBUG
void CheckDXObjectsLeak();
#endif
}  // namespace com
