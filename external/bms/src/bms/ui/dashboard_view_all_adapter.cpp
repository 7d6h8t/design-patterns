#include "pch.h"
#include "dashboard_view_all_adapter.h"
#include "resource.h"

dashboard_view_all_adapter::dashboard_view_all_adapter() noexcept
{
    LoadFromXML(IDR_BCGP_DASHBOARD);
}

dashboard_view_all_adapter::~dashboard_view_all_adapter() noexcept
{
}
