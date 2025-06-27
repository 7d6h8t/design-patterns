#include "pch.h"
#include "../include/com_utils.h"

#include <format>
#include <dxgi1_3.h>
#include <wrl/client.h>
#include <winerror.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

const char* com::com_exception::what() const noexcept {
  std::string s_str = std::format("Failure with HRESULT of {:08X}",
                                  static_cast<unsigned int>(result));

  return s_str.length() > 0 ? s_str.c_str() : "Unknown exception";
}

void com::ThrowIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    throw com_exception(hr);
  }
}

#ifdef _DEBUG
void com::CheckDXObjectsLeak() {
  Microsoft::WRL::ComPtr<IDXGIDebug1> dxgi_debug = nullptr;
  ThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgi_debug)));

  OutputDebugString(L"========== Starting Live DX Object Dump ==========\r\n");
  ThrowIfFailed(dxgi_debug->ReportLiveObjects(
      DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY |
                                           DXGI_DEBUG_RLO_IGNORE_INTERNAL)));
  OutputDebugString(L"========== Completed Live DX Object Dump ==========\r\n");
}
#endif
