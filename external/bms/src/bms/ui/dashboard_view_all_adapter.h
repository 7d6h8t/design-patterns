#pragma once
#include "dashboard_adapter.h"

class dashboard_view_all_adapter : public dashboard_adapter
{
public:
    dashboard_view_all_adapter() noexcept;
    virtual ~dashboard_view_all_adapter() noexcept;

    // nonecopyable
    dashboard_view_all_adapter(const dashboard_view_all_adapter&) = delete;
    dashboard_view_all_adapter& operator=(const dashboard_view_all_adapter&) = delete;
    // nonemovable
    dashboard_view_all_adapter(dashboard_view_all_adapter&&) = delete;
    dashboard_view_all_adapter& operator=(dashboard_view_all_adapter&&) = delete;
};
