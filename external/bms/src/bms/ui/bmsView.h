/*****************************************************************//**
 * @file   bmsView.h
 * @brief  view class of application.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "resource.h"
#include "dashboard_container.h"
#include "vc_chart_container.h"
#include "temp_chart_container.h"
#include "info/view_mode.h"
#include "dashboard_stack_container.h"
#include "dashboard_stack_container_ctrl.h"

/**
 * @brief view class of application.
 */
class bmsView : public CView
{
    DECLARE_DYNCREATE(bmsView)

public:
    /**
     * @brief constructs the object.
     */
    bmsView() noexcept;

    /**
     * @brief destructs the object.
     */
    virtual ~bmsView();

public:
    /**
     * @brief setting viewing mode.
     * 
     * @param[in] type      main dashbord, cell voltage chart, temperature chart
     */
    void setViewMode(const view_mode::page_type& type, const view_mode::dashboard_view_option& option);

    /**
     * @brief function to access dashboard_container object.
     * 
     * @return dashboard_container
     */
    dashboard_container& getDashboard();

    dashboard_stack_container& getDashboardStk1();
    dashboard_stack_container& getDashboardStk2();

    /**
     * @brief function to access vc_chart_container object.
     * 
     * @return vc_chart_container
     */
    vc_chart_container& getVCChart_container();

    /**
     * @brief function to access temp_chart_container object.
     * 
     * @return temp_chart_container
     */
    temp_chart_container& getTempChart_container();

protected:
    virtual void OnDraw(CDC* pDC) override;

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg LRESULT OnDashboardCBSETClick(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnChangeVisualManager(WPARAM wp, LPARAM lp);
    DECLARE_MESSAGE_MAP()

protected:
    CBCGPVisualContainerCtrl dashboard_ctrl;
    dashboard_stack_container_ctrl dashboard_stk1_ctrl;
    dashboard_stack_container_ctrl dashboard_stk2_ctrl;
    CBCGPVisualContainerCtrl vc_chart_ctrl;
    CBCGPVisualContainerCtrl temp_chart_ctrl;

    BOOL m_bFrameIsMaximized;

    view_mode::page_type m_current_page;
    view_mode::dashboard_view_option m_current_option;
};
