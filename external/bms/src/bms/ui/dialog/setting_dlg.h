/*****************************************************************//**
 * @file   setting_dlg.h
 * @brief  dialog class for serial communication setting (port number, baudrate).
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/
#pragma once
#include "resource.h"
#include <vector>

class mainFrame;

/**
 * @brief dialog class for serial communication setting (port number, baudrate).
 */
class setting_dlg final : public CBCGPDialog
{
public:
    using baudrates = std::vector<uint32_t>;

public:
    enum
    {
        IDD = IDD_SETTING_DLG,
        IDC_PORT = IDC_COMBO_PORT,
        IDC_SPEED = IDC_COMBO_SPEED
    };
    /**
     * @brief constructs the dialog object.
     * 
     * @param[in] pParentWnd    parent window
     */
    explicit setting_dlg(CWnd* pParentWnd = nullptr) noexcept;

    /**
     * @brief destructs the dialog object.
     */
    ~setting_dlg() noexcept;

private:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override;

    /**
     * @brief initialize GUI.
     * 
     * @return success or not
     */
    bool initGUI();

    /**
     * @brief initialize combobox with port number list.
     */
    void initPortComboCtrl();

    /**
     * @brief initialize combobox with baudrate list.
     */
    void initSpeedComboCtrl();

    /**
     * @brief setting port number.
     */
    void setPortNum() noexcept;

    /**
     * @brief setting baudrate.
     */
    void setBaudrate() noexcept;

    DECLARE_MESSAGE_MAP()
    afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);

private:
    /**
     * @brief function to access mainFrame class.
     * 
     * @return mainFrame
     */
    mainFrame* getMainFrame() const;

private:
    CBCGPComboBox m_portComboCtrl;
    CBCGPComboBox m_speedComboCtrl;

    static const baudrates speed_list;
};
