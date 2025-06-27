/*****************************************************************//**
 * @file   aboutDlg.h
 * @brief  dialog class about version, copyright etc.
 * 
 * @author aslkdj1
 * @date   March 2023
 *********************************************************************/

#pragma once
#include "Resource.h"

/**
 * @brief dialog class about version, copyright etc.
 */
class aboutDlg final : public CBCGPDialog
{
public:
    enum
    {
        IDD = IDD_ABOUTBOX,
        MANUAL_BTN = IDC_BUTTON_MANUAL
    };

public:
    /**
     * @brief constructs the dialog object.
     * 
     * @param[in] pParentWnd    parent window
     */
    explicit aboutDlg(CWnd* pParentWnd = nullptr) noexcept;

    /**
     * @brief destructs the dialog object.
     */
    virtual ~aboutDlg();

public:
    virtual BOOL OnInitDialog() override;

private:
    virtual void DoDataExchange(CDataExchange* pDX) override;

    /**
     * @brief setting vsersion.
     * 
     * @return success or not
     */
    bool SetVersion();

    /**
     * @brief setting title.
     * 
     * @return success or not
     */
    bool SetTitle();

private:
    CString m_version;
    CBCGPURLLinkButton m_manualBtn;
};
