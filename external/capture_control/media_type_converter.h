#pragma once
#include <guiddef.h>
#include <wrl/client.h>

#include <string>

struct IMFMediaType;

class MediaTypeConverter final {
 public:
  static std::wstring ToString(
      const Microsoft::WRL::ComPtr<IMFMediaType>& type);

  static std::wstring SubTypeGuidToString(const GUID& guid);
};
