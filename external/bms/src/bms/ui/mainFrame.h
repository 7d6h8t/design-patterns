/*****************************************************************//**
 * @file   mainFrame.h
 * @brief  mainframe class of application.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "configurationDockbar.h"
#include "info/meas_info.h"
#include "info/view_mode.h"
#include <MEAS_info.h>
#include <BMS_info.h>
#include "CB_info.h"
#include <future>
#include <deque>
#include <mutex>
#include <condition_variable>

#define SET_ITEMS     WM_USER + (0x01)

class bmsView;

/**
 * @brief mainframe class of application.
 */
class mainFrame final : public CBCGPFrameWnd
{
    DECLARE_DYNCREATE(mainFrame)

public:
    /**
     * @brief constructs the object.
     */
    mainFrame() noexcept;

    /**
     * @brief destructs the object.
     */
    virtual ~mainFrame() noexcept;

private:
    /**
     * @brief create rebbonbar of mainframe.
     * 
     * @return success or not
     */
    bool CreateRibbonBar();

    /**
     * @brief create dockbar.
     * 
     * @return success or not
     */
    bool CreateDockBar();

    /**
     * @brief create configuration dockbar.
     * 
     * @return success or not
     */
    bool CreateConfigurationDockBar();

    /**
     * @brief create outlook bar for change viewing mode.
     * 
     * @return success or not
     */
    bool CreateOutlookBar();

    /**
     * @brief initialize MEAS combobox of ribbon bar.
     */
    void initMEASCombobox();

    /**
     * @brief initialize CB-MEAS combobox of ribbon bar.
     */
    void initCBMEASCombobox();

    /**
     * @brief initialize change theme button ribbon bar.
     */
    void initThemeButton();

public:
    /**
     * @brief function to access configurationDockbar object.
     * 
     * @return configurationDockbar object
     */
    configurationDockbar& getConfDockbar();

    /**
     * @brief get setted port number.
     * 
     * @return port number
     */
    uint32_t getPortNum() const noexcept;

    /**
     * @brief get setted baud rate.
     * 
     * @return baud rate
     */
    uint32_t getBaudrate() const noexcept;

    /**
     * @brief get current setted MEAS type.
     * 
     * @return setted MEAS type
     */
    meas_info::type getMeasType() const noexcept;

    /**
     * @brief get current setted dashboard view option.
     * 
     * @return setted dashboard view option
     */
    view_mode::dashboard_view_option getDashboardViewOption() const noexcept;

    /**
     * @brief get current setted MEAS mode.
     * 
     * @return setted MEAS mode
     */
    meas_info::mode getMeasMode() const noexcept;

    /**
     * @brief get current setted MEAS repetition time.
     * 
     * @return repetition time (ms)
     */
    uint32_t getMeasRepetition() const noexcept;

    /**
     * @brief get current setted CB-MEAS repetition time.
     * 
     * @return repetition time (ms)
     */
    uint32_t getCBMeasRepetition() const noexcept;

    /**
     * @brief get current repetition state.
     * 
     * @return true is repeating
     */
    bool getRepetitionState() const noexcept;

    /**
     * @brief set port number.
     * 
     * @param[in] portNum       setting value
     */
    void setPortNum(const uint32_t portNum) noexcept;

    /**
     * @brief set baud rate.
     * 
     * @param[in] baudrate      setting value
     */
    void setBaudrate(const uint32_t baudrate) noexcept;

    /**
     * @brief set MEAS type.
     * 
     * @param[in] type          setting type
     */
    void setMeasType(const meas_info::type& type) noexcept;

    /**
     * @brief set dashboard view option.
     * 
     * @param[in] view          setting dashboard view option
     */
    void setDashboardViewOption(const view_mode::dashboard_view_option& view) noexcept;

    /**
     * @brief set MEAS mode.
     * 
     * @param[in] mode          setting mode
     */
    void setMeasMode(const meas_info::mode& mode) noexcept;

    /**
     * @brief set MEAS repetition time.
     * 
     * @param[in] repetition        setting time (ms)
     */
    void setMeasRepetition(const uint32_t repetition) noexcept;

    /**
     * @brief set CB-MEAS repetition time.
     * 
     * @param[in] repetition        setting time (ms)
     */
    void setCBMeasRepetition(const uint32_t repetition) noexcept;

    /**
     * @brief if click CBSET button send "*CBSET" command to serial communication by asynchronous
     * 
     * @param[in] stack         stack information required for "*CBSET" command
     * @param[in] cbset         cb(cell balancing) time information required for "*CBSET" command
     */
    void OnCBSETButton(const std::wstring& stack, const std::vector<std::wstring>& cbset);

    void setDrawLabelBorder(bool bDraw);

    bool isDrawLabelBorder();

private:
    virtual void OnUpdateFrameTitle(BOOL bAddToTitle) override;
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    afx_msg void OnSettingButton();

    afx_msg void OnConnectionButton();
    afx_msg void OnUpdateConnectionButton(CCmdUI* pCmdUI);

    afx_msg void OnCBSTARTButton();
    afx_msg void OnUpdateCBSTARTButton(CCmdUI* pCmdUI);

    afx_msg void OnCBSTARTAutoButton();
    afx_msg void OnUpdateCBSTARTAutoButton(CCmdUI* pCmdUI);

    afx_msg void OnMEASButton();
    afx_msg void OnUpdateMEASButton(CCmdUI* pCmdUI);

    afx_msg void OnCBMEASButton();
    afx_msg void OnUpdateCBMEASButton(CCmdUI* pCmdUI);

    afx_msg void OnVCButton();
    afx_msg void OnUpdateVCButton(CCmdUI* pCmdUI);

    afx_msg void OnTEMPButton();
    afx_msg void OnUpdateTEMPButton(CCmdUI* pCmdUI);

    afx_msg void OnVBATButton();
    afx_msg void OnUpdateVBATButton(CCmdUI* pCmdUI);

    afx_msg void OnSTATUSButton();
    afx_msg void OnUpdateSTATUSButton(CCmdUI* pCmdUI);

    afx_msg void OnMEASCombobox();
    afx_msg void OnUpdateMEASCombobox(CCmdUI* pCmdUI);

    afx_msg void OnCBMEASCombobox();
    afx_msg void OnUpdateCBMEASCombobox(CCmdUI* pCmdUI);

    /**
     * @brief if click MEAS_SINGLE? send "MEAS:ALL?" command to serial communication and setting the received data
     * and than post message SET_ITEMS event by asynchronous
     */
    afx_msg void OnMEASSingleButton();
    afx_msg void OnUpdateMEASSingleButton(CCmdUI* pCmdUI);

    afx_msg void OnMEASRepeatButton();
    afx_msg void OnUpdateMEASRepeatButton(CCmdUI* pCmdUI);

    afx_msg void OnConfDockViewerButton();
    afx_msg void OnUpdateConfDockViewerButton(CCmdUI* pCmdUI);

    afx_msg void OnOutlookBar(UINT id);
    afx_msg void OnUpdateOutlookBar(CCmdUI* pCmdUI);

    afx_msg void OnViewAllButton();
    afx_msg void OnUpdateViewAllButton(CCmdUI* pCmdUI);

    afx_msg void OnViewStack1Button();
    afx_msg void OnUpdateViewStack1Button(CCmdUI* pCmdUI);

    afx_msg void OnViewStack2Button();
    afx_msg void OnUpdateViewStack2Button(CCmdUI* pCmdUI);

    afx_msg void OnLabelBorderButton();
    afx_msg void OnUpdateLabelBorderButton(CCmdUI* pCmdUI);

    /**
     * @brief draw all gui items (message handler of SET_ITEMS event).
     * 
     * @param[in] wParam        drawing meas_info::type
     * @param[in] lParam        drwaing meas_info::mode
     * @return 0
     */
    afx_msg LRESULT OnSetItems(WPARAM wParam, LPARAM lParam);

private:
    /**
     * @brief terminate all asynchronous tasks
     */
    void terminate_tasks() noexcept;

private:
    /**
     * @brief initialize threshold line of cell voltage bar chart.
     */
    void initVCChart_THR();

    /**
     * @brief initialize threshold line of thermistor temperature bar chart.
     */
    void initThermistorTempChart_THR();

    /**
     * @brief function to access bmsView object.
     *
     * @return bmsView
     */
    bmsView* getView() const noexcept;

private:
    CBCGPRibbonBar m_wndRibbonBar;
    configurationDockbar m_confDockbar;
    CBCGPRibbonComboBox* m_MEAS;
    CBCGPRibbonComboBox* m_CBMEAS;

    CBCGPOutlookBar m_wndBarOutlook;
    CBCGPOutlookBarPane m_wndOutlookPane;

    uint32_t m_MEAS_Repetition;
    uint32_t m_CBMEAS_Repetition;

    meas_info::type m_measType;
    meas_info::mode m_measMode;

    view_mode::page_type m_type;
    view_mode::dashboard_view_option m_dashboard_view_option;

    uint32_t m_portNum;
    uint32_t m_baudrate;

    std::deque<std::future<void>> task_queue;
    std::future<void> m_repetition_task;

    std::mutex m_mutex_repeat;
    std::condition_variable m_cv_repeat;

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic_bool m_bSending;
    std::atomic_bool m_bTerminate;
    std::atomic_bool m_bRepetition;

    bool m_bDrawLabelBorder;

    static const std::size_t outlookbarResWidth;
    static const std::size_t outlookbarResHeight;
};
