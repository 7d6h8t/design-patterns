#pragma once
#include "functionGuard.h"

namespace utils
{
    template<typename fn, typename ...Args,
           typename ret = std::invoke_result_t<fn, Args...>>
    inline ret safetyCallback(fn& func, Args&&... args)
    {
        fn org_func = func;
        utils::functionalGuard fCallbackOff([&]() {func = nullptr; }, [&]() {func = org_func; });
        if (org_func == nullptr)
            return ret();

        return org_func(std::forward<Args>(args)...);
    }
} //namespace utils
