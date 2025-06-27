#pragma once
#include <concepts>

//#define DEBUGPRINT_FORCE_ENABLE // 릴리즈 모드에서도 표시
//#define DEBUGPRINT_SHOW_FUNCTION_NAME // 메시지 출력시 함수 이름 표시

namespace utils {
    class debugPrinter;

    template<typename T>
    concept enum_type = std::is_enum_v<std::remove_cvref_t<T>>;

    template<enum_type T>
    [[nodiscard]]
    inline constexpr auto enumToType(T&& value)
    {
        return static_cast<std::underlying_type_t<std::remove_cvref_t<T>>>(value);
    }
}

#if defined(_DEBUG) || defined(DEBUGPRINT_FORCE_ENABLE)
#	ifdef DEBUGPRINT_SHOW_FUNCTION_NAME
#		define DebugPrint(format, ...) ::utils::debugPrinter::GetInstance().LogFunction(format, __FUNCTION__, __VA_ARGS__)
#		define DebugPrintW(format, ...) ::utils::debugPrinter::GetInstance().LogFunctionW(format, __FUNCTIONW__, __VA_ARGS__)
#		define DebugPrintEx(format, ...) ::utils::debugPrinter::GetInstance().LogFunction(format, __FUNCTION__, __VA_ARGS__)
#		define DebugPrintExW(format, ...) ::utils::debugPrinter::GetInstance().LogFunctionW(format, __FUNCTIONW__, __VA_ARGS__)
#		define DebugPrintTrack() ::utils::debugPrintTracker __debugprint_tracker(__FUNCTION__)
#	else//DEBUGPRINT_SHOW_FUNCTION_NAME
#		define DebugPrint(format, ...) ::utils::debugPrinter::GetInstance().Log(format, __VA_ARGS__)
#		define DebugPrintW(format, ...) ::utils::debugPrinter::GetInstance().LogW(format, __VA_ARGS__)
#		define DebugPrintEx(format, ...) ::utils::debugPrinter::GetInstance().Log(format, __VA_ARGS__)
#		define DebugPrintExW(format, ...) ::utils::debugPrinter::GetInstance().LogW(format, __VA_ARGS__)
#		define DebugPrintTrack() ::utils::debugPrintTracker __debugprint_tracker(__FUNCTION__)
#	endif//DEBUGPRINT_SHOW_FUNCTION_NAME
#else//defined(_DEBUG) || defined(DEBUGPRINT_FORCE_ENABLE)
#	define DebugPrint(format, ...) ::utils::debugPrinter::GetInstance().Empty(format, __VA_ARGS__)
#	define DebugPrintW(format, ...) ::utils::debugPrinter::GetInstance().EmptyW(format, __VA_ARGS__)
#	define DebugPrintEx(format, ...) ::utils::debugPrinter::GetInstance().Log(format, __VA_ARGS__)
#	define DebugPrintExW(format, ...) ::utils::debugPrinter::GetInstance().LogW(format, __VA_ARGS__)
#	define DebugPrintTrack()
#endif//defined(_DEBUG) || defined(DEBUGPRINT_FORCE_ENABLE)

#include "functionGuard.h"
#include "stringLib.h"
#ifndef UNUSED_JSON
#include "jsonLib.h"
#endif //UNUSED_JSON
#include "fileSystemLib.h"
#include "debugPrinter.h"
#include "fileReadWriter.h"
#include "registryKey.h"
#include "serialization.h"
#include "safetyCallback.h"
#include "tempFileStream.h"
