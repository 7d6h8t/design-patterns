#include "pch.h"
#include "mainFrame.h"
#include "cmdmanager.h"
#include "bmsView.h"
#include <numeric>
#include "dialog/setting_dlg.h"
#include <utils.h>
#include <format>

const std::size_t mainFrame::outlookbarResWidth = 32;
const std::size_t mainFrame::outlookbarResHeight = 32;

IMPLEMENT_DYNCREATE(mainFrame, CBCGPFrameWnd)
BEGIN_MESSAGE_MAP(mainFrame, CBCGPFrameWnd)
    ON_WM_CREATE()
    ON_COMMAND(IDC_BUTTON_SETTING, OnSettingButton)
    ON_COMMAND(IDC_BUTTON_CONNECTION, OnConnectionButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_CONNECTION, OnUpdateConnectionButton)
    ON_COMMAND(IDC_BUTTON_CBSTART, OnCBSTARTButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_CBSTART, OnUpdateCBSTARTButton)
    ON_COMMAND(IDC_BUTTON_CBSTART_AUTO, OnCBSTARTAutoButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_CBSTART_AUTO, OnUpdateCBSTARTAutoButton)
    ON_COMMAND(IDC_BUTTON_MEAS, OnMEASButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_MEAS, OnUpdateMEASButton)
    ON_COMMAND(IDC_BUTTON_CBMEAS, OnCBMEASButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_CBMEAS, OnUpdateCBMEASButton)
    ON_COMMAND(IDC_BUTTON_VC, OnVCButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_VC, OnUpdateVCButton)
    ON_COMMAND(IDC_BUTTON_TEMP, OnTEMPButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_TEMP, OnUpdateTEMPButton)
    ON_COMMAND(IDC_BUTTON_VBAT, OnVBATButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_VBAT, OnUpdateVBATButton)
    ON_COMMAND(IDC_BUTTON_STATUS, OnSTATUSButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_STATUS, OnUpdateSTATUSButton)
    ON_COMMAND(IDC_COMBO_MEAS, OnMEASCombobox)
    ON_UPDATE_COMMAND_UI(IDC_COMBO_MEAS, OnUpdateMEASCombobox)
    ON_COMMAND(IDC_COMBO_CBMEAS, OnCBMEASCombobox)
    ON_UPDATE_COMMAND_UI(IDC_COMBO_CBMEAS, OnUpdateCBMEASCombobox)
    ON_COMMAND(IDC_BUTTON_MEAS_SINGLE, OnMEASSingleButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_MEAS_SINGLE, OnUpdateMEASSingleButton)
    ON_COMMAND(IDC_BUTTON_MEAS_REPEAT, OnMEASRepeatButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_MEAS_REPEAT, OnUpdateMEASRepeatButton)
    ON_COMMAND(IDC_BUTTON_CONF_VIEWER, OnConfDockViewerButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_CONF_VIEWER, OnUpdateConfDockViewerButton)
    ON_COMMAND_RANGE(ID_OUTLOOK_1, ID_OUTLOOK_3, OnOutlookBar)
    ON_UPDATE_COMMAND_UI_RANGE(ID_OUTLOOK_1, ID_OUTLOOK_3, OnUpdateOutlookBar)
    ON_COMMAND(IDC_BUTTON_VIEW_ALL, OnViewAllButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_VIEW_ALL, OnUpdateViewAllButton)
    ON_COMMAND(IDC_BUTTON_VIEW_STK1, OnViewStack1Button)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_VIEW_STK1, OnUpdateViewStack1Button)
    ON_COMMAND(IDC_BUTTON_VIEW_STK2, OnViewStack2Button)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_VIEW_STK2, OnUpdateViewStack2Button)
    ON_COMMAND(IDC_BUTTON_LABEL_BORDER, OnLabelBorderButton)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_LABEL_BORDER, OnUpdateLabelBorderButton)
    ON_MESSAGE(SET_ITEMS, OnSetItems)
END_MESSAGE_MAP()

mainFrame::mainFrame() noexcept
    : m_measType(meas_info::type::none),
    m_measMode(meas_info::mode::none),
    m_MEAS_Repetition(0),
    m_CBMEAS_Repetition(0),
    m_portNum(0),
    m_baudrate(230400),
    m_bRepetition(false),
    m_type(view_mode::page_type::main),
    m_dashboard_view_option(view_mode::dashboard_view_option::All),
    m_bSending(false),
    m_bTerminate(false),
    m_bDrawLabelBorder(false)
{
}

mainFrame::~mainFrame()
{
    terminate_tasks();
}

bool mainFrame::CreateRibbonBar()
{
    if (m_wndRibbonBar.Create(this) == false)
        return false;

    //http://help.bcgsoft.com/BCGControlBarPro/page_ribbon_designer.html
    //for using designer
    if (m_wndRibbonBar.LoadFromXML(_T("IDR_BCGP_RIBBON_XML")) == false)
        return false;

    initMEASCombobox();
    initCBMEASCombobox();
    initThemeButton();

    return true;
}

bool mainFrame::CreateDockBar()
{
    if (CreateConfigurationDockBar() == false)
        return false;

    if (EnableDocking(CBRS_ALIGN_ANY) == false)
        return false;
    return true;
}

bool mainFrame::CreateConfigurationDockBar()
{
    if (m_confDockbar.Create(_T("Configuration"), this, TRUE, configurationDockbar::IDD,
        WS_CHILD | CBRS_RIGHT | CBRS_BCGP_CLOSE | CBRS_FLOAT_MULTI, configurationDockbar::TASK) == false)
        return false;

    m_confDockbar.EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_confDockbar);

    return true;
}

bool mainFrame::CreateOutlookBar()
{
    CRect rect;
    GetClientRect(&rect);

    if (m_wndBarOutlook.Create(_T("Dashboard:"), this, CRect(0, 0, 150, rect.bottom),
        ID_OUTLOOKBAR, WS_CHILD | WS_VISIBLE | CBRS_LEFT) == false)
        return false;

    CBCGPOutlookWnd* pOutlookBar_container = dynamic_cast<CBCGPOutlookWnd*>(m_wndBarOutlook.GetUnderlinedWindow());
    if (pOutlookBar_container == nullptr)
        return false;

    CBCGPToolBarImages images;
    images.SetImageSize(CSize(outlookbarResWidth, outlookbarResHeight));
    images.Load(IDB_SHORTCUTS32);

    m_wndOutlookPane.Create(&m_wndBarOutlook, dwDefaultToolbarStyle, ID_OUTLOOKBAR_PANE, CBRS_BCGP_RESIZE);
    m_wndOutlookPane.SetOwner(this);
    m_wndOutlookPane.EnableTextLabels(TRUE);

    m_wndOutlookPane.AddButton(images.ExtractIcon(2), _T("Main"), static_cast<int32_t>(view_mode::page_type::main));
    m_wndOutlookPane.AddButton(images.ExtractIcon(1), _T("Voltage Chart"), static_cast<int32_t>(view_mode::page_type::cell_voltage));
    m_wndOutlookPane.AddButton(images.ExtractIcon(1), _T("Temperature Chart"), static_cast<int32_t>(view_mode::page_type::temperature));

    pOutlookBar_container->AddTab(&m_wndOutlookPane, _T("Dashboard:"), (UINT)-1, FALSE);
    pOutlookBar_container->SetSingleTabStatic();

    return TRUE;
}

void mainFrame::initMEASCombobox()
{
    m_MEAS = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox, m_wndRibbonBar.FindByID(IDC_COMBO_MEAS));

    for (const std::wstring& repetition : meas_info::MEAS_Repetition)
        m_MEAS->AddItem(repetition.c_str());

    if (m_MEAS->GetCount() >= 1)
    {
        m_MEAS->SelectItem(0);
        OnMEASCombobox();
    }
}

void mainFrame::initCBMEASCombobox()
{
    m_CBMEAS = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox, m_wndRibbonBar.FindByID(IDC_COMBO_CBMEAS));

    for (const std::wstring& repetition : meas_info::CBMEAS_Repetition)
        m_CBMEAS->AddItem(repetition.c_str());

    if (m_CBMEAS->GetCount() >= 1)
    {
        m_CBMEAS->SelectItem(0);
        OnCBMEASCombobox();
    }
}

void mainFrame::initThemeButton()
{
    //auto destroy
    //ribbonButton nID -1 로 안하면 disable 됨
    CBCGPRibbonButton* pThemeButton = new CBCGPRibbonButton(-1, _T("Theme\ns"));
    pThemeButton->SetMenu(IDR_THEME_MENU, TRUE /* Right align */);
    m_wndRibbonBar.AddToTabs(pThemeButton);
}

configurationDockbar& mainFrame::getConfDockbar()
{
    return m_confDockbar;
}

uint32_t mainFrame::getPortNum() const noexcept
{
    return m_portNum;
}

uint32_t mainFrame::getBaudrate() const noexcept
{
    return m_baudrate;
}

meas_info::type mainFrame::getMeasType() const noexcept
{
    return m_measType;
}

view_mode::dashboard_view_option mainFrame::getDashboardViewOption() const noexcept
{
    return m_dashboard_view_option;
}

meas_info::mode mainFrame::getMeasMode() const noexcept
{
    return m_measMode;
}

uint32_t mainFrame::getMeasRepetition() const noexcept
{
    return m_MEAS_Repetition;
}

uint32_t mainFrame::getCBMeasRepetition() const noexcept
{
    return m_CBMEAS_Repetition;
}

bool mainFrame::getRepetitionState() const noexcept
{
    return m_bRepetition;
}

void mainFrame::setPortNum(const uint32_t portNum) noexcept
{
    m_portNum = portNum;
}

void mainFrame::setBaudrate(const uint32_t baudrate) noexcept
{
    m_baudrate = baudrate;
}

void mainFrame::setMeasType(const meas_info::type& type) noexcept
{
    m_measType = type;
}

void mainFrame::setDashboardViewOption(const view_mode::dashboard_view_option& view) noexcept
{
    m_dashboard_view_option = view;
}

void mainFrame::setMeasMode(const meas_info::mode& mode) noexcept
{
    m_measMode = mode;
}

void mainFrame::setMeasRepetition(const uint32_t repetition) noexcept
{
    m_MEAS_Repetition = 1000 / repetition;
}

void mainFrame::setCBMeasRepetition(const uint32_t repetition) noexcept
{
    m_CBMEAS_Repetition = 60 * 1000 / repetition;
}

void mainFrame::OnCBSETButton(const std::wstring& stack, const std::vector<std::wstring>& cbset)
{
    task_queue.push_back(std::async(std::launch::async, [&, stack, cbset]() {
        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [&] {
                return m_bSending == false || m_bTerminate == true;
            });

            if (m_bTerminate == true)
                return;

            utils::functionalGuard guard([&] {m_bSending = true; }, [&] {m_bSending = false; });
            try
            {
                if (BMS::cmdmanager::sendCBSET(stack, cbset) == false)
                    throw std::exception("CBSET error");
                if (m_bTerminate == true)
                    return;
            } catch (const std::exception& e) {
                AfxMessageBox(CString(e.what()));
            }
            m_cv.notify_one();
        }
        PostMessage(SET_ITEMS);
    }));
}

void mainFrame::setDrawLabelBorder(bool bDraw)
{
    m_bDrawLabelBorder = bDraw;
}

bool mainFrame::isDrawLabelBorder()
{
    return m_bDrawLabelBorder;
}

void mainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    AfxSetWindowText(GetSafeHwnd(), L"BMS");
}

BOOL mainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (cs.hMenu != nullptr)
        ::DestroyMenu(cs.hMenu);
    cs.hMenu = nullptr;

    return CBCGPFrameWnd::PreCreateWindow(cs);
}

int mainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (CreateRibbonBar() == false)
        return -1;
    if (CreateDockBar() == false)
        return -1;
    if (CreateOutlookBar() == false)
        return -1;

    return 0;
}

void mainFrame::OnSettingButton()
{
    setting_dlg setdlg;
    setdlg.DoModal();
}

void mainFrame::OnConnectionButton()
{
    try
    {
        if (BMS::cmdmanager::isConnected())
        {
            if (BMS::cmdmanager::serial_disconnect() == false)
                throw std::exception("serial disconnection error");
            else
                PostMessage(SET_ITEMS, NULL, static_cast<LPARAM>(meas_info::mode::disconnect));
        }
        else
        {
            if (BMS::cmdmanager::serial_connect(m_portNum, m_baudrate) == false)
                throw std::exception("serial connection setting error");

            m_bTerminate = false;
            task_queue.push_back(std::async(std::launch::async, [&]() {
                {
                    std::unique_lock lock(m_mutex);
                    m_cv.wait(lock, [&] {
                        return m_bSending == false || m_bTerminate == true;
                    });

                    if (m_bTerminate == true)
                        return;

                    utils::functionalGuard guard([&] {m_bSending = true; }, [&] {m_bSending = false; });

                    try
                    {
                        m_confDockbar.setIDN(BMS::cmdmanager::sendIDN());
                        if (m_bTerminate == true)
                            return;
                        m_confDockbar.setCONF(BMS::cmdmanager::sendCONF());
                        if (m_bTerminate == true)
                            return;
                        if (BMS::cmdmanager::sendWAKEUP() == false)
                            throw std::exception("WAKEUP error");
                        if (m_bTerminate == true)
                            return;

                        uint32_t total_stk_size = m_confDockbar.getCONF().Total_STK;
                        if (total_stk_size < 2)
                        {
                            getView()->getDashboard().enableStack(dashboard_container::stack::stack2, false);
                            getView()->getDashboardStk2().enableStack(false);
                        }
                        if (total_stk_size < 1)
                        {
                            getView()->getDashboard().enableStack(dashboard_container::stack::stack1, false);
                            getView()->getDashboardStk1().enableStack(false);
                        }

                    }
                    catch (const std::exception& e) {
                        std::string error_message = std::format("Connection fail {}", e.what());
                        AfxMessageBox(CString(error_message.c_str()));
                        m_cv.notify_one();
                        PostMessage(SET_ITEMS, NULL, static_cast<LPARAM>(meas_info::mode::disconnect));
                        BMS::cmdmanager::serial_disconnect();
                        return;
                    }

                    m_cv.notify_one();
                }
                PostMessage(SET_ITEMS, NULL, static_cast<LPARAM>(meas_info::mode::connect));
            }));
        }
    } catch (const std::exception& e) {
        std::string error_message = std::format("Connection fail {}", e.what());
        AfxMessageBox(CString(error_message.c_str()));
        BMS::cmdmanager::serial_disconnect();
    }
}

void mainFrame::OnUpdateConnectionButton(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(BMS::cmdmanager::isConnected());
}

void mainFrame::OnCBSTARTButton()
{
    task_queue.push_back(std::async(std::launch::async, [&]() {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [&] {
            return m_bSending == false || m_bTerminate == true;
        });

        if (m_bTerminate == true)
            return;

        utils::functionalGuard guard([&] {m_bSending = true; }, [&] {m_bSending = false; });
        try
        {
            if (BMS::cmdmanager::sendCBSTART_MAN() == false)
                throw std::exception("CBSTART:MAN error");

            if (m_bTerminate == true)
                return;
        } catch (const std::exception& e) {
            AfxMessageBox(CString(e.what()));
        }

        m_cv.notify_one();
        PostMessage(SET_ITEMS);
    }));
}

void mainFrame::OnUpdateCBSTARTButton(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(BMS::cmdmanager::isConnected() && m_bRepetition == false);
}

void mainFrame::OnCBSTARTAutoButton()
{
    task_queue.push_back(std::async(std::launch::async, [&]() {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [&] {
            return m_bSending == false || m_bTerminate == true;
            });

        if (m_bTerminate == true)
            return;

        utils::functionalGuard guard([&] {m_bSending = true; }, [&] {m_bSending = false; });
        try
        {
            if (BMS::cmdmanager::sendCBSTART_AUTO() == false)
                throw std::exception("CBSTART:AUTO error");

            if (m_bTerminate == true)
                return;
        }
        catch (const std::exception& e) {
            AfxMessageBox(CString(e.what()));
        }

        m_cv.notify_one();
        PostMessage(SET_ITEMS);
        }));
}

void mainFrame::OnUpdateCBSTARTAutoButton(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(BMS::cmdmanager::isConnected() && m_bRepetition == false);
}

void mainFrame::OnMEASButton()
{
    // if already checked
    if ((getMeasType() == meas_info::type::MEAS))
        setMeasType(meas_info::type::none);
    else
        setMeasType(meas_info::type::MEAS);
}

void mainFrame::OnUpdateMEASButton(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bRepetition == false);
    pCmdUI->SetCheck((getMeasType() == meas_info::type::MEAS));
}

void mainFrame::OnCBMEASButton()
{
    // if already checked
    if ((getMeasType() == meas_info::type::CB_MEAS))
        setMeasType(meas_info::type::none);
    else
        setMeasType(meas_info::type::CB_MEAS);
}

void mainFrame::OnUpdateCBMEASButton(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bRepetition == false);
    pCmdUI->SetCheck((getMeasType() == meas_info::type::CB_MEAS));
}

void mainFrame::OnVCButton()
{
    // if already checked
    if ((getMeasMode() & meas_info::mode::VC) == meas_info::mode::VC)
        setMeasMode(getMeasMode() & ~meas_info::mode::VC);
    else
        setMeasMode(getMeasMode() | meas_info::mode::VC);
}

void mainFrame::OnUpdateVCButton(CCmdUI* pCmdUI)
{
    if (m_bRepetition)
    {
        pCmdUI->Enable(false);
        return;
    }

    bool bEnable = (getMeasType() != meas_info::type::none);
    pCmdUI->Enable(bEnable);
    if (bEnable == false)
    {
        pCmdUI->SetCheck(false);
        if ((getMeasMode() & meas_info::mode::VC) == meas_info::mode::VC)
            setMeasMode(getMeasMode() & ~meas_info::mode::VC);
    }
    else
        pCmdUI->SetCheck((getMeasMode() & meas_info::mode::VC) == meas_info::mode::VC);
}

void mainFrame::OnTEMPButton()
{
    // if already checked
    if ((getMeasMode() & meas_info::mode::TEMP) == meas_info::mode::TEMP)
        setMeasMode(getMeasMode() & ~meas_info::mode::TEMP);
    else
        setMeasMode(getMeasMode() | meas_info::mode::TEMP);
}

void mainFrame::OnUpdateTEMPButton(CCmdUI* pCmdUI)
{
    if (m_bRepetition)
    {
        pCmdUI->Enable(false);
        return;
    }

    bool bEnable = (getMeasType() != meas_info::type::none);
    pCmdUI->Enable(bEnable);
    if (bEnable == false)
    {
        pCmdUI->SetCheck(false);
        if ((getMeasMode() & meas_info::mode::TEMP) == meas_info::mode::TEMP)
            setMeasMode(getMeasMode() & ~meas_info::mode::TEMP);
    }
    else
        pCmdUI->SetCheck((getMeasMode() & meas_info::mode::TEMP) == meas_info::mode::TEMP);
}

void mainFrame::OnVBATButton()
{
    // if already checked
    if ((getMeasMode() & meas_info::mode::VBAT) == meas_info::mode::VBAT)
        setMeasMode(getMeasMode() & ~meas_info::mode::VBAT);
    else
        setMeasMode(getMeasMode() | meas_info::mode::VBAT);
}

void mainFrame::OnUpdateVBATButton(CCmdUI* pCmdUI)
{
    if (m_bRepetition)
    {
        pCmdUI->Enable(false);
        return;
    }

    bool bEnable = (getMeasType() != meas_info::type::none);
    pCmdUI->Enable(bEnable);
    if (bEnable == false)
    {
        pCmdUI->SetCheck(false);
        if ((getMeasMode() & meas_info::mode::VBAT) == meas_info::mode::VBAT)
            setMeasMode(getMeasMode() & ~meas_info::mode::VBAT);
    }
    else
        pCmdUI->SetCheck((getMeasMode() & meas_info::mode::VBAT) == meas_info::mode::VBAT);
}

void mainFrame::OnSTATUSButton()
{
    // if already checked
    if ((getMeasMode() & meas_info::mode::STAT) == meas_info::mode::STAT)
        setMeasMode(getMeasMode() & ~meas_info::mode::STAT);
    else
        setMeasMode(getMeasMode() | meas_info::mode::STAT);
}

void mainFrame::OnUpdateSTATUSButton(CCmdUI* pCmdUI)
{
    if (m_bRepetition)
    {
        pCmdUI->Enable(false);
        return;
    }

    bool bEnable = (getMeasType() == meas_info::type::CB_MEAS);
    pCmdUI->Enable(bEnable);
    if (bEnable == false)
    {
        pCmdUI->SetCheck(false);
        if ((getMeasMode() & meas_info::mode::STAT) == meas_info::mode::STAT)
            setMeasMode(getMeasMode() & ~meas_info::mode::STAT);
    }
    else
        pCmdUI->SetCheck((getMeasMode() & meas_info::mode::STAT) == meas_info::mode::STAT);
}

void mainFrame::OnMEASCombobox()
{
    std::wstring repetition = m_MEAS->GetItem(m_MEAS->GetCurSel());
    repetition = repetition.substr(0, repetition.find_first_of(L"Samples/sec"));

    setMeasRepetition(std::stoul(repetition));
}

void mainFrame::OnUpdateMEASCombobox(CCmdUI* pCmdUI)
{
    if (m_bRepetition)
    {
        pCmdUI->Enable(false);
        return;
    }

    bool bEnable = (getMeasType() == meas_info::type::MEAS);
    pCmdUI->Enable(bEnable);
}

void mainFrame::OnCBMEASCombobox()
{
    std::wstring repetition = m_CBMEAS->GetItem(m_CBMEAS->GetCurSel());

    if (repetition.find(L"Samples/10s") != std::wstring::npos)
    {
        repetition = repetition.substr(0, repetition.find_first_of(L"Samples/10s"));
        setCBMeasRepetition(std::stoul(repetition) * 6);
    }
    else
    {
        repetition = repetition.substr(0, repetition.find_first_of(L"Samples/min"));
        setCBMeasRepetition(std::stoul(repetition));
    }
}

void mainFrame::OnUpdateCBMEASCombobox(CCmdUI* pCmdUI)
{
    if (m_bRepetition)
    {
        pCmdUI->Enable(false);
        return;
    }

    bool bEnable = (getMeasType() == meas_info::type::CB_MEAS);
    pCmdUI->Enable(bEnable);
}

void mainFrame::OnMEASSingleButton()
{
    task_queue.push_back(std::async(std::launch::async, [&]() {
        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [&] {
                return m_bSending == false || m_bTerminate == true;
            });

            if (m_bTerminate == true)
                return;

            utils::functionalGuard guard([&] {m_bSending = true; }, [&] {m_bSending = false; });
            try
            {
                meas_info::type type = getMeasType();
                if (type == meas_info::type::CB_MEAS)
                {
                    getView()->getDashboard().setCB_TIME(BMS::cmdmanager::sendCBTIME());
                    if (m_bTerminate == true)
                        return;
                }

                meas_info::mode mode = getMeasMode();
                if ((mode & meas_info::mode::ALL) == meas_info::mode::ALL)
                {
                    MEAS_info::ALL all = BMS::cmdmanager::sendMEAS_ALL();

                    getView()->getDashboard().setMEAS_VC(all.vc);
                    getView()->getDashboard().setMEAS_TEMP(all.temp);
                    getView()->getDashboard().setMEAS_VBAT(all.vbat);
                }
                else
                {
                    if ((mode & meas_info::mode::VC) == meas_info::mode::VC)
                    {
                        getView()->getDashboard().setMEAS_VC(BMS::cmdmanager::sendMEAS_VC());
                        if (m_bTerminate == true)
                            return;
                    }

                    if ((mode & meas_info::mode::TEMP) == meas_info::mode::TEMP)
                    {
                        getView()->getDashboard().setMEAS_TEMP(BMS::cmdmanager::sendMEAS_TEMP());
                        if (m_bTerminate == true)
                            return;
                    }

                    if ((mode & meas_info::mode::VBAT) == meas_info::mode::VBAT)
                    {
                        getView()->getDashboard().setMEAS_VBAT(BMS::cmdmanager::sendMEAS_VBAT());
                        if (m_bTerminate == true)
                            return;
                    }
                }

                if ((mode & meas_info::mode::STAT) == meas_info::mode::STAT)
                {
                    getView()->getDashboard().setMEAS_STAT(BMS::cmdmanager::sendMEAS_STAT());
                    if (m_bTerminate == true)
                        return;
                }

            } catch (const std::exception& e) {
                AfxMessageBox(CString(e.what()));
                m_bTerminate.store(true);
                PostMessage(SET_ITEMS, NULL, static_cast<LPARAM>(meas_info::mode::disconnect));
                BMS::cmdmanager::serial_disconnect();
                return;
            }

            if (m_bTerminate == true)
                return;

            m_cv.notify_one();
        }
        PostMessage(SET_ITEMS, static_cast<WPARAM>(getMeasType()), static_cast<LPARAM>(getMeasMode()));
    }));
}

void mainFrame::OnUpdateMEASSingleButton(CCmdUI* pCmdUI)
{
    bool bEnable = true;
    if (BMS::cmdmanager::isConnected() == false)
        bEnable = false;
    else if (m_bRepetition)
        bEnable = false;
    else if (getMeasType() == meas_info::type::none)
        bEnable = false;
    else if (getMeasType() == meas_info::type::MEAS && getMeasMode() == meas_info::mode::none)
        bEnable = false;

    pCmdUI->Enable(bEnable);
}

void mainFrame::OnMEASRepeatButton()
{
    if (getMeasType() == meas_info::type::none)
        return;

    m_bRepetition = !m_bRepetition;

    uint32_t repetition_sample;
    if (getMeasType() == meas_info::type::MEAS)
        repetition_sample = m_MEAS_Repetition;
    else if (getMeasType() == meas_info::type::CB_MEAS)
        repetition_sample = m_CBMEAS_Repetition;

    if (getView()->getDashboard().isEnableStack(dashboard_container::stack::stack1))
        getView()->getDashboard().enableEditCBGrid(dashboard_container::stack::stack1, !m_bRepetition);

    if (getView()->getDashboard().isEnableStack(dashboard_container::stack::stack2))
        getView()->getDashboard().enableEditCBGrid(dashboard_container::stack::stack2, !m_bRepetition);

    if (getView()->getDashboardStk1().isEnableStack())
        getView()->getDashboardStk1().enableEditCBGrid(!m_bRepetition);

    if (getView()->getDashboardStk2().isEnableStack())
        getView()->getDashboardStk2().enableEditCBGrid(!m_bRepetition);

    if (m_bRepetition == false)
    {
        m_cv_repeat.notify_one();
        return;
    }

    m_repetition_task = std::async(std::launch::async, [&, repetition_sample] {
        while (m_bRepetition && m_bTerminate == false && BMS::cmdmanager::isConnected())
        {
            OnMEASSingleButton();
            std::unique_lock lock(m_mutex_repeat);
            m_cv_repeat.wait_for(lock, std::chrono::milliseconds(repetition_sample), [&] {return m_bRepetition == false || m_bTerminate == true; });
        }
    });
}

void mainFrame::OnUpdateMEASRepeatButton(CCmdUI* pCmdUI)
{
    bool bEnable = true;
    if (BMS::cmdmanager::isConnected() == false)
        bEnable = false;
    if (getMeasType() == meas_info::type::none)
        bEnable = false;
    else if (getMeasType() == meas_info::type::MEAS && getMeasMode() == meas_info::mode::none)
        bEnable = false;

    pCmdUI->Enable(bEnable);
    pCmdUI->SetCheck(m_bRepetition && bEnable);
}

void mainFrame::OnConfDockViewerButton()
{
    m_confDockbar.ShowControlBar(!m_confDockbar.IsVisible(), FALSE, TRUE);
}

void mainFrame::OnUpdateConfDockViewerButton(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_confDockbar.IsVisible());
}

void mainFrame::OnOutlookBar(UINT id)
{
    m_type = static_cast<view_mode::page_type>(id);
    getView()->setViewMode(m_type, m_dashboard_view_option);
}

void mainFrame::OnUpdateOutlookBar(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(static_cast<uint32_t>(m_type) == pCmdUI->m_nID);
}

void mainFrame::OnViewAllButton()
{
    setDashboardViewOption(view_mode::dashboard_view_option::All);
    getView()->setViewMode(m_type, m_dashboard_view_option);
}

void mainFrame::OnUpdateViewAllButton(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((getDashboardViewOption() == view_mode::dashboard_view_option::All));
}

void mainFrame::OnViewStack1Button()
{
    setDashboardViewOption(view_mode::dashboard_view_option::Stack1);
    getView()->setViewMode(m_type, m_dashboard_view_option);
}

void mainFrame::OnUpdateViewStack1Button(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((getDashboardViewOption() == view_mode::dashboard_view_option::Stack1));
}

void mainFrame::OnViewStack2Button()
{
    setDashboardViewOption(view_mode::dashboard_view_option::Stack2);
    getView()->setViewMode(m_type, m_dashboard_view_option);
}

void mainFrame::OnUpdateViewStack2Button(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((getDashboardViewOption() == view_mode::dashboard_view_option::Stack2));
}

void mainFrame::OnLabelBorderButton()
{
    setDrawLabelBorder(!m_bDrawLabelBorder);

    getView()->getDashboard().drawChartLabelBorder(isDrawLabelBorder());
    getView()->getDashboardStk1().drawChartLabelBorder(isDrawLabelBorder());
    getView()->getDashboardStk2().drawChartLabelBorder(isDrawLabelBorder());
}

void mainFrame::OnUpdateLabelBorderButton(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(isDrawLabelBorder());
}

LRESULT mainFrame::OnSetItems(WPARAM wParam, LPARAM lParam)
{
    meas_info::type type = static_cast<meas_info::type>(wParam);
    meas_info::mode mode = static_cast<meas_info::mode>(lParam);

    try
    {
        if ((mode & meas_info::mode::connect) == meas_info::mode::connect)
        {
            m_confDockbar.drawIDN(m_confDockbar.getIDN());
            m_confDockbar.drawCONF(m_confDockbar.getCONF());

            initVCChart_THR();
            initThermistorTempChart_THR();

            if (getView()->getDashboard().isEnableStack(dashboard_container::stack::stack1))
            {
                getView()->getDashboard().enableEditCBGrid(dashboard_container::stack::stack1, true);
                getView()->getDashboard().drawCBSET(dashboard_container::stack::stack1, getView()->getDashboard().getCBSET(dashboard_container::stack::stack1));
            }

            if (getView()->getDashboard().isEnableStack(dashboard_container::stack::stack2))
            {
                getView()->getDashboard().enableEditCBGrid(dashboard_container::stack::stack2, true);
                getView()->getDashboard().drawCBSET(dashboard_container::stack::stack2, getView()->getDashboard().getCBSET(dashboard_container::stack::stack2));
            }

            if (getView()->getDashboardStk1().isEnableStack())
            {
                getView()->getDashboardStk1().enableEditCBGrid(true);
                getView()->getDashboardStk1().drawCBSET(getView()->getDashboardStk1().getCBSET());
            }

            if (getView()->getDashboardStk2().isEnableStack())
            {
                getView()->getDashboardStk2().enableEditCBGrid(true);
                getView()->getDashboardStk2().drawCBSET(getView()->getDashboardStk2().getCBSET());
            }
        }

        if (type != meas_info::type::none)
        {
            if ((mode & meas_info::mode::VC) == meas_info::mode::VC)
            {
                MEAS_info::VC vc = getView()->getDashboard().getMEAS_VC();
                getView()->getDashboard().drawMEAS_VC(vc);
                getView()->getDashboardStk1().drawVCChart(vc.STK1);
                getView()->getDashboardStk2().drawVCChart(vc.STK2);
                getView()->getVCChart_container().drawMEAS_VC(vc);

                MEAS_info::VBAT vbat = getView()->getDashboard().getMEAS_VBAT();
                getView()->getDashboard().drawSumChart(vbat.STK1, vbat.STK2, std::reduce(vc.STK1.begin(), vc.STK1.end()), std::reduce(vc.STK2.begin(), vc.STK2.end()));
            }

            if ((mode & meas_info::mode::TEMP) == meas_info::mode::TEMP)
            {
                MEAS_info::TEMP temp = getView()->getDashboard().getMEAS_TEMP();
                getView()->getDashboard().drawMEAS_TEMP(temp);
                getView()->getDashboardStk1().drawSensorTempChart(temp.STK1_sensor);
                getView()->getDashboardStk1().drawThermistorTempChart(temp.STK1_thermistor);
                getView()->getDashboardStk2().drawSensorTempChart(temp.STK2_sensor);
                getView()->getDashboardStk2().drawThermistorTempChart(temp.STK2_thermistor);
                getView()->getTempChart_container().drawMEAS_TEMP(temp);
            }

            if ((mode & meas_info::mode::VBAT) == meas_info::mode::VBAT)
            {
                MEAS_info::VBAT vbat = getView()->getDashboard().getMEAS_VBAT();
                getView()->getDashboard().drawMEAS_VBAT(vbat);

                MEAS_info::VC vc = getView()->getDashboard().getMEAS_VC();
                getView()->getDashboard().drawSumChart(vbat.STK1, vbat.STK2, std::reduce(vc.STK1.begin(), vc.STK1.end()), std::reduce(vc.STK2.begin(), vc.STK2.end()));
            }

            if ((mode & meas_info::mode::STAT) == meas_info::mode::STAT)
            {
                MEAS_info::STAT stat = getView()->getDashboard().getMEAS_STAT();
                getView()->getDashboard().drawMEAS_STAT(stat);

                getView()->getDashboardStk1().drawStatOV(stat.OV_STK1);
                getView()->getDashboardStk1().drawStatUV(stat.UV_STK1);
                getView()->getDashboardStk1().drawStatVCB(stat.VCB_STK1);
                getView()->getDashboardStk1().drawStatUT(stat.UT_STK1);
                getView()->getDashboardStk1().drawStatOT(stat.OT_STK1);
                getView()->getDashboardStk1().drawStatOTCB(stat.OTCB_STK1);

                getView()->getDashboardStk2().drawStatOV(stat.OV_STK2);
                getView()->getDashboardStk2().drawStatUV(stat.UV_STK2);
                getView()->getDashboardStk2().drawStatVCB(stat.VCB_STK2);
                getView()->getDashboardStk2().drawStatUT(stat.UT_STK2);
                getView()->getDashboardStk2().drawStatOT(stat.OT_STK2);
                getView()->getDashboardStk2().drawStatOTCB(stat.OTCB_STK2);
            }

            if (type == meas_info::type::CB_MEAS)
            {
                CB_info::CBTIME cbtime = getView()->getDashboard().getCB_TIME();
                getView()->getDashboard().drawCB_TIME(cbtime);
                getView()->getDashboardStk1().drawCBLeft(cbtime.STK1);
                getView()->getDashboardStk2().drawCBLeft(cbtime.STK2);
            }
        }

        if ((mode & meas_info::mode::disconnect) == meas_info::mode::disconnect)
        {
            m_bRepetition.store(false);
            terminate_tasks();

            m_confDockbar.clear();

            getView()->getDashboard().clear();
            getView()->getDashboardStk1().clear();
            getView()->getDashboardStk2().clear();
            getView()->getVCChart_container().claer();
            getView()->getTempChart_container().clear();
        }

        for (auto itr = task_queue.begin(); itr != task_queue.end();)
        {
            if (itr->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                itr = task_queue.erase(itr);
            else
                ++itr;
        }
    } catch (const std::exception& e) {
        std::string error_message = std::format("Draw error {}", e.what());
        AfxMessageBox(CString(error_message.c_str()));
    }

    return 0;
}

void mainFrame::terminate_tasks() noexcept
{
    m_bTerminate.store(true);
    m_cv.notify_all();

    task_queue.clear();

    m_cv_repeat.notify_all();
}

void mainFrame::initVCChart_THR()
{
    getView()->getDashboard().clearVCChart_THR(dashboard_container::stack::stack1);
    getView()->getDashboard().clearVCChart_THR(dashboard_container::stack::stack2);

    getView()->getDashboard().drawVCChart_THR(dashboard_container::stack::stack1, m_confDockbar.getCONF().OV_THR, dashboard_container::OV_thr_color);
    getView()->getDashboard().drawVCChart_THR(dashboard_container::stack::stack1, m_confDockbar.getCONF().UV_THR, dashboard_container::UV_thr_color);
    getView()->getDashboard().drawVCChart_THR(dashboard_container::stack::stack1, m_confDockbar.getCONF().VCB_THR, dashboard_container::VCB_thr_color);

    getView()->getDashboard().drawVCChart_THR(dashboard_container::stack::stack2, m_confDockbar.getCONF().OV_THR, dashboard_container::OV_thr_color);
    getView()->getDashboard().drawVCChart_THR(dashboard_container::stack::stack2, m_confDockbar.getCONF().UV_THR, dashboard_container::UV_thr_color);
    getView()->getDashboard().drawVCChart_THR(dashboard_container::stack::stack2, m_confDockbar.getCONF().VCB_THR, dashboard_container::VCB_thr_color);

    getView()->getDashboardStk1().clearVCChart_THR();

    getView()->getDashboardStk1().drawVCChart_THR(m_confDockbar.getCONF().OV_THR, dashboard_container::OV_thr_color);
    getView()->getDashboardStk1().drawVCChart_THR(m_confDockbar.getCONF().UV_THR, dashboard_container::UV_thr_color);
    getView()->getDashboardStk1().drawVCChart_THR(m_confDockbar.getCONF().VCB_THR, dashboard_container::VCB_thr_color);

    getView()->getDashboardStk2().clearVCChart_THR();

    getView()->getDashboardStk2().drawVCChart_THR(m_confDockbar.getCONF().OV_THR, dashboard_container::OV_thr_color);
    getView()->getDashboardStk2().drawVCChart_THR(m_confDockbar.getCONF().UV_THR, dashboard_container::UV_thr_color);
    getView()->getDashboardStk2().drawVCChart_THR(m_confDockbar.getCONF().VCB_THR, dashboard_container::VCB_thr_color);

    getView()->getVCChart_container().clearVCChart_THR(vc_chart_container::stack::stack1);
    getView()->getVCChart_container().clearVCChart_THR(vc_chart_container::stack::stack2);

    getView()->getVCChart_container().drawVCChart_THR(vc_chart_container::stack::stack1, m_confDockbar.getCONF().OV_THR, dashboard_container::OV_thr_color);
    getView()->getVCChart_container().drawVCChart_THR(vc_chart_container::stack::stack1, m_confDockbar.getCONF().UV_THR, dashboard_container::UV_thr_color);
    getView()->getVCChart_container().drawVCChart_THR(vc_chart_container::stack::stack1, m_confDockbar.getCONF().VCB_THR, dashboard_container::VCB_thr_color);

    getView()->getVCChart_container().drawVCChart_THR(vc_chart_container::stack::stack2, m_confDockbar.getCONF().OV_THR, dashboard_container::OV_thr_color);
    getView()->getVCChart_container().drawVCChart_THR(vc_chart_container::stack::stack2, m_confDockbar.getCONF().UV_THR, dashboard_container::UV_thr_color);
    getView()->getVCChart_container().drawVCChart_THR(vc_chart_container::stack::stack2, m_confDockbar.getCONF().VCB_THR, dashboard_container::VCB_thr_color);
}

void mainFrame::initThermistorTempChart_THR()
{
    getView()->getDashboard().clearThermistorTempChart_THR(dashboard_container::stack::stack1);
    getView()->getDashboard().clearThermistorTempChart_THR(dashboard_container::stack::stack2);

    getView()->getDashboard().drawThermistorTempChart_THR(dashboard_container::stack::stack1, m_confDockbar.getCONF().UT_THR, dashboard_container::UT_thr_color);
    getView()->getDashboard().drawThermistorTempChart_THR(dashboard_container::stack::stack1, m_confDockbar.getCONF().OT_THR, dashboard_container::OT_thr_color);
    getView()->getDashboard().drawThermistorTempChart_THR(dashboard_container::stack::stack1, m_confDockbar.getCONF().OTCB_THR, dashboard_container::OTCB_thr_color);

    getView()->getDashboard().drawThermistorTempChart_THR(dashboard_container::stack::stack2, m_confDockbar.getCONF().UT_THR, dashboard_container::UT_thr_color);
    getView()->getDashboard().drawThermistorTempChart_THR(dashboard_container::stack::stack2, m_confDockbar.getCONF().OT_THR, dashboard_container::OT_thr_color);
    getView()->getDashboard().drawThermistorTempChart_THR(dashboard_container::stack::stack2, m_confDockbar.getCONF().OTCB_THR, dashboard_container::OTCB_thr_color);

    getView()->getDashboardStk1().clearThermistorTempChart_THR();
    getView()->getDashboardStk1().drawThermistorTempChart_THR(m_confDockbar.getCONF().UT_THR, dashboard_container::UT_thr_color);
    getView()->getDashboardStk1().drawThermistorTempChart_THR(m_confDockbar.getCONF().OT_THR, dashboard_container::OT_thr_color);
    getView()->getDashboardStk1().drawThermistorTempChart_THR(m_confDockbar.getCONF().OTCB_THR, dashboard_container::OTCB_thr_color);

    getView()->getDashboardStk2().clearThermistorTempChart_THR();
    getView()->getDashboardStk2().drawThermistorTempChart_THR(m_confDockbar.getCONF().UT_THR, dashboard_container::UT_thr_color);
    getView()->getDashboardStk2().drawThermistorTempChart_THR(m_confDockbar.getCONF().OT_THR, dashboard_container::OT_thr_color);
    getView()->getDashboardStk2().drawThermistorTempChart_THR(m_confDockbar.getCONF().OTCB_THR, dashboard_container::OTCB_thr_color);

    getView()->getTempChart_container().clearTEMPChart_THR(temp_chart_container::stack::stack1);
    getView()->getTempChart_container().clearTEMPChart_THR(temp_chart_container::stack::stack2);

    getView()->getTempChart_container().drawTEMPChart_THR(temp_chart_container::stack::stack1, m_confDockbar.getCONF().UT_THR, dashboard_container::UT_thr_color);
    getView()->getTempChart_container().drawTEMPChart_THR(temp_chart_container::stack::stack1, m_confDockbar.getCONF().OT_THR, dashboard_container::OT_thr_color);
    getView()->getTempChart_container().drawTEMPChart_THR(temp_chart_container::stack::stack1, m_confDockbar.getCONF().OTCB_THR, dashboard_container::OTCB_thr_color);

    getView()->getTempChart_container().drawTEMPChart_THR(temp_chart_container::stack::stack2, m_confDockbar.getCONF().UT_THR, dashboard_container::UT_thr_color);
    getView()->getTempChart_container().drawTEMPChart_THR(temp_chart_container::stack::stack2, m_confDockbar.getCONF().OT_THR, dashboard_container::OT_thr_color);
    getView()->getTempChart_container().drawTEMPChart_THR(temp_chart_container::stack::stack2, m_confDockbar.getCONF().OTCB_THR, dashboard_container::OTCB_thr_color);
}

bmsView* mainFrame::getView() const noexcept
{
    return reinterpret_cast<bmsView*>(GetActiveView());
}
