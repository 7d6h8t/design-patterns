#pragma once
#include "resource.h"
#include <cstdint>

namespace view_mode
{
    enum class page_type : uint32_t
    {
        main = ID_OUTLOOK_1,
        cell_voltage = ID_OUTLOOK_2,
        temperature = ID_OUTLOOK_3
    };

    enum class dashboard_view_option : uint32_t
    {
        All = 0,
        Stack1,
        Stack2
    };
} //namespace view_mode
