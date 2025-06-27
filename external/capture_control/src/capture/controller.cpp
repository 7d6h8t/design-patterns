#include "pch.h"
#include "../../include/capture/controller.h"

#include <Shlwapi.h>
#include <mfapi.h>
#include <Mferror.h>
#include <wincodec.h>

#include <ScreenGrab.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/mcc.hpp>

#include "../../include/com_utils.h"
#include "../../device_resources.h"
#include "../../media_type_converter.h"
#include "../../conversion/nv12_converter.h"
#include "../../conversion/yuy2_converter.h"
#include "../../conversion/i420_converter.h"
#include "../../conversion/yv12_converter.h"

using namespace capture;
using Microsoft::WRL::ComPtr;

Controller::Controller() noexcept {
  com::ThrowIfFailed(
      CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

  com::ThrowIfFailed(MFStartup(MF_VERSION));

  dx::DeviceResources::GetSingleton().CreateDeviceResources();

  pixel_calculator_ = std::make_unique<PixelStatsCalculator>();
}

Controller::~Controller() noexcept {
  if (IsRecording()) RecordOff();

  CloseDevice();
  Disconnect();

  MFShutdown();
  CoUninitialize();

  dx::DeviceResources::GetSingleton().ClearDeviceResources();

#ifdef _DEBUG
  com::CheckDXObjectsLeak();
#endif
}

void Controller::Init(const HWND& wnd) {
  renderer_.Init(wnd);
}

bool Controller::IsConnected() { return devices_.empty() == false; }

void Controller::Connect() {
  Disconnect();

  ComPtr<IMFAttributes> attributes = nullptr;
  IMFActivate** devices = nullptr;
  uint32_t count = 0;

  com::ThrowIfFailed(MFCreateAttributes(attributes.GetAddressOf(), 1));
  com::ThrowIfFailed(
      attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                          MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));

  com::ThrowIfFailed(MFEnumDeviceSources(attributes.Get(), &devices, &count));

  devices_.reserve(count);
  for (uint32_t i = 0; i < count; ++i) devices_.push_back(devices[i]);

  CoTaskMemFree(devices);
}

void Controller::Disconnect() {
  if (devices_.empty()) return;

  for (auto& dev : devices_) {
    ComPtr<IMFMediaSource> source = nullptr;
    com::ThrowIfFailed(
        dev->ActivateObject(IID_PPV_ARGS(source.ReleaseAndGetAddressOf())));

    source->Shutdown();
    source->Release();
  }

  devices_.clear();
}

bool Controller::IsOpend() { return reader_ != nullptr; }

void Controller::OpenDevice(const uint32_t index) {
  CloseDevice();

  std::lock_guard<std::mutex> lock(mutex_);

  ComPtr<IMFMediaSource> source = nullptr;
  ComPtr<IMFAttributes> attributes = nullptr;

  com::ThrowIfFailed(devices_[index]->ActivateObject(
      IID_PPV_ARGS(source.ReleaseAndGetAddressOf())));

  com::ThrowIfFailed(MFCreateAttributes(&attributes, 4));
  com::ThrowIfFailed(attributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, this));
  com::ThrowIfFailed(attributes->SetUINT32(
      MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, true));
  com::ThrowIfFailed(attributes->SetUINT32(
      MF_SOURCE_READER_DISCONNECT_MEDIASOURCE_ON_SHUTDOWN, true));
  com::ThrowIfFailed(attributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, false));

  com::ThrowIfFailed(MFCreateSourceReaderFromMediaSource(
      source.Get(), attributes.Get(), reader_.GetAddressOf()));

  com::ThrowIfFailed(
      reader_->SetStreamSelection(MF_SOURCE_READER_FIRST_VIDEO_STREAM, true));

  InitDeviceMediaTypes();
}

void Controller::CloseDevice() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (IsOpend()) {
    reader_.Reset();
    ClearDeivceMediaTypes();
  }
}

void Controller::SetMediaType(const uint32_t index) {
  if (!IsOpend()) throw std::exception("[error] SourceReader is nullptr");

  MFRatio fps{};
  com::ThrowIfFailed(
      MFGetAttributeRatio(types_[index].Get(), MF_MT_FRAME_RATE,
                          reinterpret_cast<uint32_t*>(&fps.Numerator),
                          reinterpret_cast<uint32_t*>(&fps.Denominator)));

  uint32_t width = 0, height = 0;
  com::ThrowIfFailed(MFGetAttributeSize(types_[index].Get(), MF_MT_FRAME_SIZE,
                                        &width, &height));

  GUID sub_type{};
  com::ThrowIfFailed(types_[index]->GetGUID(MF_MT_SUBTYPE, &sub_type));
  SetConverter(sub_type);

  ComPtr<IMFMediaType> output_type = nullptr;
  com::ThrowIfFailed(MFCreateMediaType(&output_type));
  com::ThrowIfFailed(output_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
  com::ThrowIfFailed(
      output_type->SetGUID(MF_MT_SUBTYPE, sub_type));

  com::ThrowIfFailed(output_type->SetUINT32(MF_MT_INTERLACE_MODE,
                                            MFVideoInterlace_Progressive));
  com::ThrowIfFailed(
      output_type->SetUINT64(MF_MT_FRAME_SIZE, PackSize(width, height)));
  com::ThrowIfFailed(output_type->SetUINT64(
      MF_MT_FRAME_RATE, PackSize(fps.Numerator, fps.Denominator)));

  com::ThrowIfFailed(reader_->SetCurrentMediaType(
      MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, output_type.Get()));

  SIZE frame_size{static_cast<long>(width), static_cast<long>(height)};

  renderer_.SetSize(frame_size);
  renderer_.AdjustViewport();
}

std::vector<std::wstring> Controller::GetDeviceListName() {
  std::vector<std::wstring> device_names;
  device_names.reserve(devices_.size());

  for (auto& device : devices_) {
    wchar_t* name = nullptr;
    uint32_t length = 0;

    HRESULT hr = device->GetAllocatedString(
        MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &name, &length);
    if (FAILED(hr)) break;

    if (name != nullptr && length != 0) device_names.push_back({name, length});
    CoTaskMemFree(name);
  }

  return device_names;
}

std::vector<std::wstring> Controller::GetDeviceMediaTypesString() {
  std::vector<std::wstring> type_strings;
  type_strings.reserve(types_.size());

  for (const auto& type : types_)
    type_strings.push_back(MediaTypeConverter::ToString(type));

  return type_strings;
}

void Controller::Play() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (reader_ == nullptr) throw std::exception("Play error!");

  state_ = capture::Satate::play;
  com::ThrowIfFailed(reader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0,
                                         nullptr, nullptr, nullptr, nullptr));
}

bool Controller::IsRecording() { return state_ == capture::Satate::recording; }

void Controller::RecordOn(const std::filesystem::path& path) {
  std::lock_guard<std::mutex> lock(mutex_);
  state_ = capture::Satate::recording;

  record_path_ = path;
  if (record_path_.extension() != L".yuv") InitWriter(record_path_);
}

void Controller::RecordOff() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (IsRecording()) {
    state_ = capture::Satate::play;
    record_path_.clear();
  }

  if (writer_) {
    com::ThrowIfFailed(writer_->Finalize());
    writer_.Reset();
  }
}

void Controller::EnableCenterPoint(bool enable) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (IsOpend()) renderer_.EnableCenterPoint(enable);
}

void Controller::EnableRoiRect(bool enable) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (IsOpend()) renderer_.EnableRoiRect(enable);
}

void Controller::EnableColorChecker(bool enable) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (IsOpend()) renderer_.EnableColorChecker(enable);
}

void Controller::AutoDetect() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (IsOpend() == false) return;

  cv::Mat cv_frame(cv::Size{output_frame_.size_.cx, output_frame_.size_.cy},
                   CV_8UC4, output_frame_.bits_.get());
  cv::cvtColor(cv_frame, cv_frame, cv::COLOR_BGRA2BGR);

  cv::Ptr<cv::mcc::CCheckerDetector> detector =
      cv::mcc::CCheckerDetector::create();

  if (detector->process(cv_frame, cv::mcc::MCC24) == false) return;

  std::vector<cv::Ptr<cv::mcc::CChecker>> checkers =
      detector->getListColorChecker();

  for (cv::Ptr<cv::mcc::CChecker> checker : checkers) {
    auto chart = checker->getColorCharts();

    std::vector<POINT> checker_boxs;
    checker_boxs.reserve(chart.size());

    for (const auto& point : chart)
      checker_boxs.push_back(
          {static_cast<int32_t>(point.x), static_cast<int32_t>(point.y)});

    renderer_.SetPhysicalCheckerBox(checker_boxs);
    return;
  }
}

void Controller::SaveFrame(const std::filesystem::path& path) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (path.extension() == L".yuv")
    converter_->SaveFrame(path, input_frame_);
  else {
    GUID container_format = {};
    bool force_rgb = false;

    if (path.extension() == L".jpg") {
      container_format = GUID_ContainerFormatJpeg;
    } else if (path.extension() == L".png") {
      container_format = GUID_ContainerFormatPng;
      force_rgb = true;
    } else if (path.extension() == L".bmp") {
      container_format = GUID_ContainerFormatBmp;
    }

    ComPtr<ID3D11Resource> frame_resource = nullptr;
    renderer_.GetRenderingSRV()->GetResource(
        frame_resource.ReleaseAndGetAddressOf());

    com::ThrowIfFailed(DirectX::SaveWICTextureToFile(
        dx::DeviceResources::GetSingleton().GetDeviceContext(),
        frame_resource.Get(), container_format, path.c_str(), nullptr, nullptr,
        force_rgb));
  }
}

void Controller::AdjustViewport() { renderer_.AdjustViewport(); }

void Controller::SetCheckerBoxsPhysical(
    const std::vector<Controller::CheckBox>& checker_boxs) {
  std::vector<POINT> boxs;
  boxs.reserve(checker_boxs.size() * 4);

  for (const auto& checkbox : checker_boxs)
    for (const auto& point : checkbox) boxs.push_back(point);

  renderer_.SetPhysicalCheckerBox(boxs);
}

void Controller::SetRoiScreenRect(const RECT& rect) {
  std::lock_guard<std::mutex> lock(mutex_);
  renderer_.SetRoiScreenRect(rect);
}

void Controller::SetRoiPhysicalRect(const RECT& rect) {
  std::lock_guard<std::mutex> lock(mutex_);
  renderer_.SetRoiPhysicalRect(rect);
}

void Controller::SetZoom(const int16_t delta, const POINT& screen_point) {
  renderer_.SetZoom((delta > 0) ? true : false, screen_point);
}

void Controller::SetRendererPosition(const POINT& last, const POINT& current) {
  renderer_.SetViewportPosition(last, current);
}

RECT Controller::GetRoiPhysicalRect() {
  std::lock_guard<std::mutex> lock(mutex_);

  return ROIRectLayer::GetRoiPhysicalRect();
}

std::vector<Controller::CheckBox> Controller::GetPhysicalCheckerBoxs() {
  return ColorCheckerLayer::GetPhysicalCheckerBoxs();
}

POINT Controller::ScreenToPhysical(const POINT& screen) {
  return renderer_.ScreenToPhysical(screen);
}

HRESULT STDMETHODCALLTYPE Controller::QueryInterface(REFIID iid, void** ppv) {
  static const QITAB qit[] = {
      QITABENT(Controller, IMFSourceReaderCallback),
      {0},
  };
  return QISearch(this, qit, iid, ppv);
}

ULONG STDMETHODCALLTYPE Controller::AddRef() {
  return InterlockedIncrement(&ref_count_);
}

ULONG STDMETHODCALLTYPE Controller::Release() {
  ULONG count = InterlockedDecrement(&ref_count_);
  if (count == 0) delete this;
  return count;
}

HRESULT STDMETHODCALLTYPE Controller::OnEvent(DWORD, IMFMediaEvent*) {
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Controller::OnFlush(DWORD) { return S_OK; }

HRESULT STDMETHODCALLTYPE Controller::OnReadSample(HRESULT hrStatus,
                                                DWORD dwStreamIndex,
                                                DWORD dwStreamFlags,
                                                LONGLONG llTimestamp,
                                                IMFSample* pSample) {
  std::lock_guard<std::mutex> lock(mutex_);

  HRESULT hr = S_OK;
  ComPtr<IMFMediaBuffer> buffer = nullptr;

  if (FAILED(hrStatus)) hr = hrStatus;

  if (SUCCEEDED(hr)) {
    if (pSample) {
      if (state_ == capture::Satate::play) {
        com::ThrowIfFailed(
            pSample->ConvertToContiguousBuffer(buffer.GetAddressOf()));

        if (input_frame_.bits_) input_frame_.bits_.reset();
        input_frame_ =
            converter_->GetFormatedFrameData(buffer, renderer_.GetFrameSize());
      }

      output_frame_ =
          converter_->ConvertToRGB(input_frame_, *pixel_calculator_);

      renderer_.Render(output_frame_);

      if (IsRecording()) {
        if (writer_)
          com::ThrowIfFailed(writer_->WriteSample(
              dwStreamIndex, CreateSample(output_frame_).Get()));
        else
          converter_->RecordFrame(record_path_, input_frame_);
      }
    }
  }

  if (SUCCEEDED(hr) && state_ != capture::Satate::stop || IsRecording())
    hr = reader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr,
                             nullptr, nullptr, nullptr);
  return hr;
}

void Controller::InitDeviceMediaTypes() {
  ClearDeivceMediaTypes();

  HRESULT hr = S_OK;

  for (uint32_t media_type_index = 0; SUCCEEDED(hr); ++media_type_index) {
    IMFMediaType* type = nullptr;

    if (SUCCEEDED(hr))
      hr = reader_->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                       media_type_index, &type);
    if (hr == MF_E_NO_MORE_TYPES) {
      break;
    } else if (SUCCEEDED(hr)) {
      GUID major_type{};
      hr = type->GetGUID(MF_MT_MAJOR_TYPE, &major_type);
      if (major_type != MFMediaType_Video) continue;

      types_.push_back(type);
    }
  }
}

void Controller::ClearDeivceMediaTypes() { types_.clear(); }

void Controller::SetConverter(const GUID& sub_type) {
  if (sub_type == MFVideoFormat_NV12)
    converter_ = std::make_unique<conversion::NV12Converter>();
  else if (sub_type == MFVideoFormat_YUY2)
    converter_ = std::make_unique<conversion::YUY2Converter>();
  else if (sub_type == MFVideoFormat_I420)
    converter_ = std::make_unique<conversion::I420Converter>();
  else if (sub_type == MFVideoFormat_YV12)
    converter_ = std::make_unique<conversion::YV12Converter>();
  else
    throw std::exception("This input format is not supported.");
}

void Controller::InitWriter(const std::filesystem::path& path) {
  com::ThrowIfFailed(MFCreateSinkWriterFromURL(
      path.c_str(), nullptr, nullptr, writer_.ReleaseAndGetAddressOf()));

  DWORD stream = 0;
  {
    // init output type
    ComPtr<IMFMediaType> output_type = nullptr;
    reader_->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                 output_type.ReleaseAndGetAddressOf());
    com::ThrowIfFailed(output_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264));
    com::ThrowIfFailed(writer_->AddStream(output_type.Get(), &stream));
  }

  {
    // init input type
    ComPtr<IMFMediaType> input_type = nullptr;
    reader_->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                 input_type.ReleaseAndGetAddressOf());
    com::ThrowIfFailed(
        input_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_ARGB32));
    com::ThrowIfFailed(
        writer_->SetInputMediaType(stream, input_type.Get(), nullptr));
  }

  com::ThrowIfFailed(writer_->BeginWriting());
}

ComPtr<IMFSample> Controller::CreateSample(const Frame& frame) {
  ComPtr<IMFMediaBuffer> output_buffer = nullptr;
  com::ThrowIfFailed(
      MFCreateMemoryBuffer(frame.pitch_ * frame.size_.cy,
                           output_buffer.ReleaseAndGetAddressOf()));

  BYTE* buffer_data = nullptr;
  DWORD max = 0, current = 0;
  com::ThrowIfFailed(output_buffer->Lock(&buffer_data, &max, &current));

  memcpy(buffer_data, frame.bits_.get(),
         frame.pitch_ * frame.size_.cy);

  com::ThrowIfFailed(output_buffer->Unlock());

  com::ThrowIfFailed(output_buffer->SetCurrentLength(frame.pitch_ *
                                                     frame.size_.cy));

  ComPtr<IMFSample> output_sample = nullptr;
  com::ThrowIfFailed(MFCreateSample(output_sample.ReleaseAndGetAddressOf()));

  com::ThrowIfFailed(output_sample->AddBuffer(output_buffer.Get()));
  return output_sample;
}

Frame Controller::CopyFrame(const Frame& frame) {
  size_t frame_size = frame.pitch_ * frame.size_.cy;

  Frame copy_frame{.bits_ = std::make_unique<BYTE[]>(frame_size),
                   .size_ = frame.size_,
                   .pitch_ = frame.pitch_};

  std::memcpy(copy_frame.bits_.get(), frame.bits_.get(),
              sizeof(BYTE) * frame_size);
  return std::move(copy_frame);
}

void Controller::Pause() {
  std::lock_guard<std::mutex> lock(mutex_);
  state_ = capture::Satate::pause;
}

void Controller::Stop() {
  state_ = capture::Satate::stop;

  CloseDevice();
}

capture::Satate Controller::GetState() const { return state_; }

SIZE Controller::GetFrameSize() {
  std::lock_guard<std::mutex> lock(mutex_);

  return renderer_.GetFrameSize();
}

FrameData Controller::GetRenderingFrameData() {
  std::lock_guard<std::mutex> lock(mutex_);

  capture::FrameData frame{.yuv_ = CopyFrame(input_frame_),
                           .rgb_ = CopyFrame(output_frame_),
                           .y_ = pixel_calculator_->GetPixelInfoY(),
                           .u_ = pixel_calculator_->GetPixelInfoU(),
                           .v_ = pixel_calculator_->GetPixelInfoV(),
                           .r_ = pixel_calculator_->GetPixelInfoR(),
                           .g_ = pixel_calculator_->GetPixelInfoG(),
                           .b_ = pixel_calculator_->GetPixelInfoB()};

  return frame;
}
