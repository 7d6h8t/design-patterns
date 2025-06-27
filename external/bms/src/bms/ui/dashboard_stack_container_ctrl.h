#pragma once
#include "info/view_mode.h"

class dashboard_stack_container_ctrl : public CBCGPVisualContainerCtrl
{
public:
    dashboard_stack_container_ctrl(const view_mode::dashboard_view_option stack, CRuntimeClass* pContainerRTI = NULL);
    virtual ~dashboard_stack_container_ctrl();

    virtual CBCGPVisualContainer* GetVisualContainer() override;

private:
    view_mode::dashboard_view_option m_stack;
};
