#pragma once
#include <string>

class bmsApp : public CBCGPWinApp
{
public:
    bmsApp() noexcept;
    virtual ~bmsApp() noexcept;

public:
    virtual BOOL InitInstance();
    static const std::wstring& getManualPath() noexcept;

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnAppAbout();

private:
    static const std::wstring manual_html;
};

extern bmsApp theApp;
