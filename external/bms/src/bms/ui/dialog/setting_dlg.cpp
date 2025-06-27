#include "pch.h"
#include "setting_dlg.h"
#include "ui/mainFrame.h"
#include <device_manager.h>
#include <string>
#include <algorithm>

const setting_dlg::baudrates setting_dlg::speed_list = {
    110, 300, 600, 1200, 2400, 4800, 9600,
    14400, 19200, 38400, 57600, 115200, 230400,
    460800, 921600
};

BEGIN_MESSAGE_MAP(setting_dlg, CBCGPDialog)
    ON_WM_DEVICECHANGE()
END_MESSAGE_MAP()

setting_dlg::setting_dlg(CWnd* pParentWnd) noexcept
    : CBCGPDialog(IDD, pParentWnd)
{
    EnableVisualManagerStyle(TRUE, TRUE);
}

setting_dlg::~setting_dlg() noexcept
{
}

void setting_dlg::DoDataExchange(CDataExchange* pDX)
{
    CBCGPDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PORT, m_portComboCtrl);
    DDX_Control(pDX, IDC_SPEED, m_speedComboCtrl);
}

BOOL setting_dlg::OnInitDialog()
{
    CBCGPDialog::OnInitDialog();

    if (initGUI() == false)
        return false;

    return 0;
}

void setting_dlg::OnOK()
{
    setPortNum();
    setBaudrate();

    CBCGPDialog::OnOK();
}

bool setting_dlg::initGUI()
{
    initPortComboCtrl();
    initSpeedComboCtrl();

    return true;
}

void setting_dlg::initPortComboCtrl()
{
    m_portComboCtrl.ResetContent();

    std::list<uint32_t> port_list = device_manager::getCOMPortList();
    for (const uint32_t port : device_manager::getCOMPortList())
    {
        std::wstring port_name = L"COM" + std::to_wstring(port);
        m_portComboCtrl.AddString(port_name.c_str());
    }

    uint32_t curPortNum = getMainFrame()->getPortNum();

    auto itr = std::find(port_list.begin(), port_list.end(), curPortNum);
    if (itr != port_list.end())
    {
        int32_t index = static_cast<int32_t>(std::distance(port_list.begin(), itr));
        m_portComboCtrl.SetCurSel(index);
    }
}

void setting_dlg::initSpeedComboCtrl()
{
    m_speedComboCtrl.ResetContent();

    for (const uint32_t speed : speed_list)
        m_speedComboCtrl.AddString(std::to_wstring(speed).c_str());

    uint32_t curBaudrate = getMainFrame()->getBaudrate();

    auto itr = std::find(speed_list.begin(), speed_list.end(), curBaudrate);
    if (itr != speed_list.end())
    {
        int32_t index = static_cast<int32_t>(std::distance(speed_list.begin(), itr));
        m_speedComboCtrl.SetCurSel(index);
    }
}

void setting_dlg::setPortNum() noexcept
{
    CString tempstring;
    m_portComboCtrl.GetWindowText(tempstring);

    std::wstring remove_string = L"COM";
    std::wstring port(tempstring);

    if (port.empty())
        return;

    port = port.substr(port.find(remove_string) + remove_string.size());
    getMainFrame()->setPortNum(std::stoul(port));
}

void setting_dlg::setBaudrate() noexcept
{
    CString tempstring;
    m_speedComboCtrl.GetWindowText(tempstring);

    getMainFrame()->setBaudrate(std::stoul(tempstring.GetBuffer()));
}

BOOL setting_dlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
    initPortComboCtrl();
    return TRUE;
}

mainFrame* setting_dlg::getMainFrame() const
{
    return reinterpret_cast<mainFrame*>(AfxGetMainWnd());
}
