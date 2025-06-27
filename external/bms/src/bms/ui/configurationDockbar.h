/*****************************************************************//**
 * @file   configurationDockbar.h
 * @brief  docking bar class for constructs the BMS configuration information.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "resource.h"
#include <string>
#include "BMS_info.h"

/**
 * @brief docking bar class for constructs the BMS configuration information.
 */
class configurationDockbar final : public CBCGPDialogBar
{
public:
    enum
    {
        IDD = IDD_CONF_DOCKBAR,
        TASK = ID_CONFDOCKBAR_TASK,
        PROP_LIST = IDC_PROPLIST
    };

public:
    /**
     * @brief constructs the object.
     */
    configurationDockbar() noexcept;

    /**
     * @brief destructs the object.
     */
    ~configurationDockbar() noexcept;

    //none copyable
    configurationDockbar(const configurationDockbar&) = delete;
    configurationDockbar& operator=(const configurationDockbar&) = delete;
    //none movable
    configurationDockbar(configurationDockbar&&) = delete;
    configurationDockbar& operator=(configurationDockbar&&) = delete;

public:
    /**
     * @brief get IDN data.
     * 
     * @return IDN
     */
    BMS_info::IDN getIDN() const noexcept;

    /**
     * @brief get CONF data.
     * 
     * @return CONF
     */
    BMS_info::CONF getCONF() const noexcept;

    /**
     * @brief draw IDN on property list.
     * 
     * @param[in] idn       drawing IDN
     */
    void drawIDN(const BMS_info::IDN& idn);

    /**
     * @brief draw CONF on property list.
     * 
     * @param[in] conf      drawing CONF
     */
    void drawCONF(const BMS_info::CONF& conf);

    /**
     * @brief draw device id on property list.
     * 
     * @param[in] dev_id        drawing device id
     */
    void drawDeviceID(const std::wstring& dev_id);

    /**
     * @brief draw firmware version on property list.
     * 
     * @param[in] firm_ver      drawing firmware version
     */
    void drawFirmwareVer(const std::wstring& firm_ver);

    /**
     * @brief draw total stack number on property list.
     * 
     * @param[in] nStack        drawing total stack number
     */
    void drawTStack(const uint32_t nStack);

    /**
     * @brief draw stack1 cell number on property list.
     * 
     * @param[in] num           drawing stack1 cell number
     */
    void drawStack1CellNum(const uint32_t num);

    /**
     * @brief draw stack1 temp number on property list.
     * 
     * @param[in] num           drawing stack1 temp number
     */
    void drawStack1TempNum(const uint32_t num);

    /**
     * @brief draw stack2 cell number on property list.
     * 
     * @param[in] num           drawing stack2 cell number
     */
    void drawStack2CellNum(const uint32_t num);

    /**
     * @brief draw stack2 temp number on property list.
     * 
     * @param[in] num           drawing stack2 temp number
     */
    void drawStack2TempNum(const uint32_t num);

    /**
     * @brief draw OV thr on property list.
     * 
     * @param[in] thr           drawing ov thr
     */
    void drawOVThr(const double thr);

    /**
     * @brief draw UV thr on property list.
     * 
     * @param[in] thr           drawing uv thr
     */
    void drawUVThr(const double thr);

    /**
     * @brief draw VCB thr on property list.
     * 
     * @param[in] thr           drawing vcb thr
     */
    void drawVCBThr(const double thr);

    /**
     * @brief draw OTCB thr on property list.
     * 
     * @param[in] thr           drawing otcb thr
     */
    void drawOTCBThr(const double thr);

    /**
     * @brief draw OT thr on property list.
     * 
     * @param[in] thr           drawing ot thr
     */
    void drawOTThr(const double thr);

    /**
     * @brief draw UT thr on property list.
     * 
     * @param[in] thr           drawing ut thr
     */
    void drawUTThr(const double thr);

    /**
     * @brief set IDN data.
     * 
     * @param[in] idn       setting IDN
     */
    void setIDN(const BMS_info::IDN& idn) noexcept;

    /**
     * @brief set CONF data.
     * 
     * @param[in] conf      setting CONF
     */
    void setCONF(const BMS_info::CONF& conf) noexcept;

    /**
     * @brief clear data and ui.
     */
    void clear();

private:
    /**
     * @brief function to setting layout auto adjust.
     */
    void initLayout();

    /**
     * @brief initialize about configuration property list.
     */
    void initPropList();

    /**
     * @brief edit on or off.
     * 
     * @param[in] bEnable   enable or disable
     */
    void enableEdit(bool bEnable);

public:
    afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP();

private:
    CBCGPPropList m_wndPropList;

    BMS_info::IDN m_idn;
    BMS_info::CONF m_conf;
}; //class configurationDockbar final : public CBCGPDialogBar
