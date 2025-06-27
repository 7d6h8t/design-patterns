#pragma once
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <filesystem>
#include <functional>

#include "state.h"
#include "data.h"
#include "../export.h"
#include "../../renderer.h"
#include "../../conversion/format_converter.h"

#pragma warning(push)
#pragma warning(disable : 4251)

struct IMFSample;
struct IMFMediaEvent;
struct IMFMediaType;

namespace capture {
class CAPTURE_API Controller final : public IMFSourceReaderCallback {
 public:
  using CheckBox = std::array<POINT, 4>;

  Controller() noexcept;
  ~Controller() noexcept;

  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

  Controller(Controller&&) = delete;
  Controller& operator=(Controller&&) = delete;

  void Init(const HWND& wnd);

  bool IsConnected();
  void Connect();
  void Disconnect();

  bool IsOpend();
  void OpenDevice(const uint32_t index);
  void CloseDevice();

  void SetMediaType(const uint32_t index);
  std::vector<std::wstring> GetDeviceListName();
  std::vector<std::wstring> GetDeviceMediaTypesString();

  void Play();
  void Pause();
  void Stop();

  capture::Satate GetState() const;
  SIZE GetFrameSize();
  FrameData GetRenderingFrameData();

  bool IsRecording();
  void RecordOn(const std::filesystem::path& path);
  void RecordOff();

  void EnableCenterPoint(bool enable);
  void EnableRoiRect(bool enable);
  void EnableColorChecker(bool enable);
  void AutoDetect();

  void SaveFrame(const std::filesystem::path& path);
  void AdjustViewport();

  void SetCheckerBoxsPhysical(const std::vector<CheckBox>& checker_boxs);
  void SetRoiScreenRect(const RECT& rect);
  void SetRoiPhysicalRect(const RECT& rect);
  void SetZoom(const int16_t delta, const POINT& screen_point);
  void SetRendererPosition(const POINT& last, const POINT& current);

  RECT GetRoiPhysicalRect();
  std::vector<CheckBox> GetPhysicalCheckerBoxs();

  POINT ScreenToPhysical(const POINT& screen);

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppv) override;
  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;
  HRESULT STDMETHODCALLTYPE OnEvent(DWORD, IMFMediaEvent*) override;
  HRESULT STDMETHODCALLTYPE OnFlush(DWORD) override;

  // callback read buffer
  HRESULT STDMETHODCALLTYPE OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex,
                                         DWORD dwStreamFlags,
                                         LONGLONG llTimestamp,
                                         IMFSample* pSample) override;

 private:
  void InitDeviceMediaTypes();
  void ClearDeivceMediaTypes();
  void SetConverter(const GUID& sub_type);

  void InitWriter(const std::filesystem::path& path);
  Microsoft::WRL::ComPtr<IMFSample> CreateSample(const Frame& frame);

  Frame CopyFrame(const Frame& frame);

  std::vector<Microsoft::WRL::ComPtr<IMFActivate>> devices_;
  std::vector<Microsoft::WRL::ComPtr<IMFMediaType>> types_;
  Microsoft::WRL::ComPtr<IMFSourceReader> reader_ = nullptr;
  Microsoft::WRL::ComPtr<IMFSinkWriter> writer_ = nullptr;

  Renderer renderer_;
  std::unique_ptr<conversion::FormatConverter> converter_ = nullptr;
  std::unique_ptr<PixelStatsCalculator> pixel_calculator_ = nullptr;

  Frame input_frame_{};
  Frame output_frame_{};

  std::mutex mutex_;
  std::atomic<capture::Satate> state_ = capture::Satate::stop;
  uint32_t ref_count_ = 1;

  std::filesystem::path record_path_{};
};
}  // namespace capture

#pragma warning(pop)
