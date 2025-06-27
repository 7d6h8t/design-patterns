#include "pch.h"
#include "dashboard_view_stack_adapter.h"
#include "resource.h"

dashboard_view_stack_adapter::dashboard_view_stack_adapter(const view_mode::dashboard_view_option stack) noexcept
{
    if (stack == view_mode::dashboard_view_option::Stack1)
        LoadFromXML(IDR_BCGP_DASHBOARD_STK1);
    else if (stack == view_mode::dashboard_view_option::Stack2)
        LoadFromXML(IDR_BCGP_DASHBOARD_STK2);
}

dashboard_view_stack_adapter::~dashboard_view_stack_adapter() noexcept
{
}
