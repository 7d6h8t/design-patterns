#include "pch.h"
#include "aboutDlg.h"
#include "versioninfo/versionUtils.h"
#include "../bmsApp.h"
#include <utils.h>

aboutDlg::aboutDlg(CWnd* pParentWnd) noexcept
    : CBCGPDialog(IDD, pParentWnd)
{
    EnableVisualManagerStyle(TRUE, TRUE);
}

aboutDlg::~aboutDlg()
{
}

BOOL aboutDlg::OnInitDialog()
{
    CBCGPDialog::OnInitDialog();
    if (SetVersion() == false)
        return false;
    if (SetTitle() == false)
        return false;

    static const std::wstring ManualPath = bmsApp::getManualPath();
    m_manualBtn.SetURL(ManualPath.c_str());

    return TRUE;
}

void aboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CBCGPDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_VERSION, m_version);
    DDX_Control(pDX, MANUAL_BTN, m_manualBtn);
}

bool aboutDlg::SetVersion()
{
    constexpr const wchar_t* wsErrorVersion = L"GetVersion Error";

    //get exe file path
    const std::wstring wsFileName = ::utils::fileSystemLib::GetModuleFilePath();
    std::wstring wsVersion = versionUtils::GetFileVersion(wsFileName);

    bool bResult = false;
    if (wsVersion.empty() == true)
    {
        wsVersion = wsErrorVersion;
        bResult = false;
    }

    m_version = wsVersion.c_str();
    bResult = true;
    UpdateData(false);

    return bResult;
}

bool aboutDlg::SetTitle()
{
    CString strTitle;
    if (strTitle.LoadString(AFX_IDS_APP_TITLE) == FALSE)
        return false;

    SetWindowText(strTitle);
    return true;
}
