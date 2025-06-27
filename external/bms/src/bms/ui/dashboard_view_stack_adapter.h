#pragma once
#include "info/view_mode.h"
#include "dashboard_adapter.h"

class dashboard_view_stack_adapter : public dashboard_adapter
{
public:
    dashboard_view_stack_adapter(const view_mode::dashboard_view_option stack) noexcept;
    virtual ~dashboard_view_stack_adapter() noexcept;

    // nonecopyable
    dashboard_view_stack_adapter(const dashboard_view_stack_adapter&) = delete;
    dashboard_view_stack_adapter& operator=(const dashboard_view_stack_adapter&) = delete;
    // nonemovable
    dashboard_view_stack_adapter(dashboard_view_stack_adapter&&) = delete;
    dashboard_view_stack_adapter& operator=(dashboard_view_stack_adapter&&) = delete;
};
