#include "pch.h"
#include "dashboard_stack_container_ctrl.h"
#include "dashboard_stack_container.h"

dashboard_stack_container_ctrl::dashboard_stack_container_ctrl(const view_mode::dashboard_view_option stack, CRuntimeClass* pContainerRTI)
    : CBCGPVisualContainerCtrl(pContainerRTI),
    m_stack(stack)
{
}

dashboard_stack_container_ctrl::~dashboard_stack_container_ctrl()
{
}

CBCGPVisualContainer* dashboard_stack_container_ctrl::GetVisualContainer()
{
    if (m_pContainer == NULL)
        m_pContainer = new dashboard_stack_container(m_stack);

    return m_pContainer;
}
