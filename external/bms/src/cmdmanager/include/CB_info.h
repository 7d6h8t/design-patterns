#pragma once
#include "export.h"
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable:4251)

namespace CB_info {
    struct DLL_EXPORT CBTIME
    {
        std::vector<std::wstring> STK1;
        std::vector<std::wstring> STK2;
    }; //struct CBTIME
} //namespace CB_info

#pragma warning(pop)
