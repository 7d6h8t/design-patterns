#include "pch.h"
#include "Resource.h"
#include "bmsApp.h"
#include "mainFrame.h"
#include "bmsDoc.h"
#include "bmsView.h"
#include "ui/dialog/aboutDlg.h"
#include "dumpManager/dumpManager.h"
#include <utils.h>

const std::wstring bmsApp::manual_html = L"doc\\manual\\manual.html";

BEGIN_MESSAGE_MAP(bmsApp, CBCGPWinApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()

bmsApp theApp;

bmsApp::bmsApp() noexcept
{
    SetAppID(_T("bms.AppID.NoVersion"));
    AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2022_WHITE, ID_LIGHTTHEME);
    AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2022_BLACK, ID_DARKTHEME);

    SetVisualTheme(BCGP_VISUAL_THEME_OFFICE_2022_WHITE);
}

bmsApp::~bmsApp() noexcept
{
}

BOOL bmsApp::InitInstance()
{
    ::utils::debugPrinter::GetInstance().SetApplicationName("BMS");
    dumpManager::registerDumpHandle();

    CWinApp::InitInstance();
    EnableTaskbarInteraction(FALSE);

    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    LoadStdProfileSettings();

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(bmsDoc),
        RUNTIME_CLASS(mainFrame),
        RUNTIME_CLASS(bmsView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);

    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    SetVisualTheme(GetVisualTheme());

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

const std::wstring& bmsApp::getManualPath() noexcept
{
    static const std::wstring ManualPath = utils::fileSystemLib::GetModuleDirectory() + manual_html;
    return ManualPath;
}

void bmsApp::OnAppAbout()
{
    aboutDlg about_dlg;
    about_dlg.DoModal();
}
