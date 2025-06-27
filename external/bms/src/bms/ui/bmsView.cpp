#include "pch.h"
#include "bmsView.h"
#include "bmsApp.h"
#include "mainFrame.h"
#include <cmdmanager.h>

IMPLEMENT_DYNCREATE(bmsView, CView)
BEGIN_MESSAGE_MAP(bmsView, CView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_WINDOWPOSCHANGED()
    ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_CLICK, OnDashboardCBSETClick)
    ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
END_MESSAGE_MAP()

bmsView::bmsView() noexcept
    : m_bFrameIsMaximized(FALSE),
    dashboard_stk1_ctrl(view_mode::dashboard_view_option::Stack1),
    dashboard_stk2_ctrl(view_mode::dashboard_view_option::Stack2),
    m_current_page(view_mode::page_type::main),
    m_current_option(view_mode::dashboard_view_option::All)
{
}

bmsView::~bmsView()
{
}

void bmsView::setViewMode(const view_mode::page_type& type, const view_mode::dashboard_view_option& option)
{
    if (m_current_option != option && BMS::cmdmanager::isConnected())
    {
        switch (m_current_option)
        {
        case view_mode::dashboard_view_option::All:
            getDashboardStk1().drawCBSET(getDashboard().getCBSET(dashboard_container::stack::stack1));
            getDashboardStk2().drawCBSET(getDashboard().getCBSET(dashboard_container::stack::stack2));
            break;
        case view_mode::dashboard_view_option::Stack1:
            getDashboard().drawCBSET(dashboard_container::stack::stack1, getDashboardStk1().getCBSET());
            break;
        case view_mode::dashboard_view_option::Stack2:
            getDashboard().drawCBSET(dashboard_container::stack::stack2, getDashboardStk2().getCBSET());
            break;
        default:
            break;
        }
    }

    dashboard_ctrl.ShowWindow((type == view_mode::page_type::main) && (option == view_mode::dashboard_view_option::All) ? SW_SHOWNOACTIVATE : SW_HIDE);
    dashboard_stk1_ctrl.ShowWindow((type == view_mode::page_type::main) && (option == view_mode::dashboard_view_option::Stack1) ? SW_SHOWNOACTIVATE : SW_HIDE);
    dashboard_stk2_ctrl.ShowWindow((type == view_mode::page_type::main) && (option == view_mode::dashboard_view_option::Stack2) ? SW_SHOWNOACTIVATE : SW_HIDE);
    vc_chart_ctrl.ShowWindow(type == view_mode::page_type::cell_voltage ? SW_SHOWNOACTIVATE : SW_HIDE);
    temp_chart_ctrl.ShowWindow(type == view_mode::page_type::temperature ? SW_SHOWNOACTIVATE : SW_HIDE);

    m_current_page = type;
    m_current_option = option;
}

dashboard_container& bmsView::getDashboard()
{
    return *reinterpret_cast<dashboard_container*>(dashboard_ctrl.GetVisualContainer());
}

dashboard_stack_container& bmsView::getDashboardStk1()
{
    return *reinterpret_cast<dashboard_stack_container*>(dashboard_stk1_ctrl.GetVisualContainer());
}

dashboard_stack_container& bmsView::getDashboardStk2()
{
    return *reinterpret_cast<dashboard_stack_container*>(dashboard_stk2_ctrl.GetVisualContainer());
}

vc_chart_container& bmsView::getVCChart_container()
{
    return *reinterpret_cast<vc_chart_container*>(vc_chart_ctrl.GetVisualContainer());
}

temp_chart_container& bmsView::getTempChart_container()
{
    return *reinterpret_cast<temp_chart_container*>(temp_chart_ctrl.GetVisualContainer());
}

void bmsView::OnDraw(CDC* pDC)
{
    return;
}

int bmsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    mainFrame* pMainFrame = DYNAMIC_DOWNCAST(mainFrame, GetParent());
    ASSERT_VALID(pMainFrame);

    m_bFrameIsMaximized = pMainFrame->IsZoomed();

    dashboard_ctrl.SetContainerRTI(RUNTIME_CLASS(dashboard_container));
    dashboard_ctrl.Create(CRect(0, 0, 0, 0), this, IDD_DASHBOARD, WS_CHILD);

    dashboard_stk1_ctrl.Create(CRect(0, 0, 0, 0), this, IDD_DASHBOARD, WS_CHILD);

    dashboard_stk2_ctrl.Create(CRect(0, 0, 0, 0), this, IDD_DASHBOARD, WS_CHILD);

    vc_chart_ctrl.SetContainerRTI(RUNTIME_CLASS(vc_chart_container));
    vc_chart_ctrl.Create(CRect(0, 0, 0, 0), this, IDD_DASHBOARD, WS_CHILD);

    temp_chart_ctrl.SetContainerRTI(RUNTIME_CLASS(temp_chart_container));
    temp_chart_ctrl.Create(CRect(0, 0, 0, 0), this, IDD_DASHBOARD, WS_CHILD);

    setViewMode(view_mode::page_type::main, view_mode::dashboard_view_option::All);

    return 0;
}

void bmsView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    dashboard_ctrl.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    dashboard_stk1_ctrl.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    dashboard_stk2_ctrl.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    vc_chart_ctrl.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    temp_chart_ctrl.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void bmsView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
    CView::OnWindowPosChanged(lpwndpos);

    const BOOL bFrameWasMaximized = m_bFrameIsMaximized;

    mainFrame* pMainFrame = DYNAMIC_DOWNCAST(mainFrame, GetParent());
    ASSERT_VALID(pMainFrame);

    m_bFrameIsMaximized = pMainFrame->IsZoomed();

    if (m_bFrameIsMaximized != bFrameWasMaximized)
    {
        dashboard_ctrl.GetVisualContainer()->SetScrollOffset(CBCGPPoint(0.0, 0.0), TRUE);
        dashboard_ctrl.SetWindowPos(NULL, -1, -1, lpwndpos->cx, lpwndpos->cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

        dashboard_stk1_ctrl.GetVisualContainer()->SetScrollOffset(CBCGPPoint(0.0, 0.0), TRUE);
        dashboard_stk1_ctrl.SetWindowPos(NULL, -1, -1, lpwndpos->cx, lpwndpos->cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

        dashboard_stk2_ctrl.GetVisualContainer()->SetScrollOffset(CBCGPPoint(0.0, 0.0), TRUE);
        dashboard_stk2_ctrl.SetWindowPos(NULL, -1, -1, lpwndpos->cx, lpwndpos->cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

        vc_chart_ctrl.GetVisualContainer()->SetScrollOffset(CBCGPPoint(0.0, 0.0), TRUE);
        vc_chart_ctrl.SetWindowPos(NULL, -1, -1, lpwndpos->cx, lpwndpos->cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

        temp_chart_ctrl.GetVisualContainer()->SetScrollOffset(CBCGPPoint(0.0, 0.0), TRUE);
        temp_chart_ctrl.SetWindowPos(NULL, -1, -1, lpwndpos->cx, lpwndpos->cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

LRESULT bmsView::OnDashboardCBSETClick(WPARAM wp, LPARAM lp)
{
    std::wstring stack;
    std::vector<std::wstring> cbset;
    bool bPressed = false;

    if (dashboard_container::CBSET_BUTTON_STK1 == (int)wp)
    {
        getDashboard().onClikedCBSETButton_STK1();
        getDashboardStk1().onClikedCBSETButton();

        stack = L"1";
        if (m_current_option == view_mode::dashboard_view_option::All)
            cbset = getDashboard().getCBSET(dashboard_container::stack::stack1);
        else
            cbset = getDashboardStk1().getCBSET();

        bPressed = getDashboardStk1().isPressedCBSET();
    }
    else if (dashboard_container::CBSET_BUTTON_STK2 == (int)wp)
    {
        getDashboard().onClikedCBSETButton_STK2();
        getDashboardStk2().onClikedCBSETButton();

        stack = L"2";
        if (m_current_option == view_mode::dashboard_view_option::All)
            cbset = getDashboard().getCBSET(dashboard_container::stack::stack2);
        else
            cbset = getDashboardStk2().getCBSET();

        bPressed = getDashboardStk2().isPressedCBSET();
    }

    mainFrame* pMainFrame = DYNAMIC_DOWNCAST(mainFrame, GetParent());
    ASSERT_VALID(pMainFrame);

    if (bPressed)
        pMainFrame->OnCBSETButton(stack, cbset);

    return 0;
}

LRESULT bmsView::OnChangeVisualManager(WPARAM wp, LPARAM lp)
{
    bmsApp::BCGP_VISUAL_THEME cur_theme = theApp.GetVisualTheme();

    getDashboard().SelectVisualTheme(cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE ? 0 : 1);
    getDashboardStk1().SelectVisualTheme(cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE ? 0 : 1);
    getDashboardStk2().SelectVisualTheme(cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE ? 0 : 1);
    getVCChart_container().SelectVisualTheme(cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE ? 0 : 1);
    getTempChart_container().SelectVisualTheme(cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE ? 0 : 1);

    getDashboard().onChangeTheme();
    getDashboardStk1().onChangeTheme();
    getDashboardStk2().onChangeTheme();
    return 0;
}
