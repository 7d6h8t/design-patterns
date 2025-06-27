/*****************************************************************//**
 * @file   dashboard_container.h
 * @brief  dashboard container class.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "dashboard_view_all_adapter.h"
#include <vector>
#include <string>
#include "MEAS_info.h"
#include "CB_info.h"

class mainFrame;

/**
 * @brief dashboard container class.
 */
class dashboard_container final : public dashboard_view_all_adapter
{
public:
    DECLARE_SERIAL(dashboard_container)

    enum class stack : uint32_t
    {
        stack1 = 1,
        stack2
    };

public:
    /**
     * @brief constructs the object.
     */
    dashboard_container() noexcept;

    /**
     * @brief destructs the object.
     */
    ~dashboard_container() noexcept;

    // nonecopyable
    dashboard_container(const dashboard_container&) = delete;
    dashboard_container& operator=(const dashboard_container&) = delete;
    // nonemovable
    dashboard_container(dashboard_container&&) = delete;
    dashboard_container& operator=(dashboard_container&&) = delete;

public:
    virtual void OnAfterCreateWnd() override;

    void onChangeTheme();

    /**
     * @brief send "*CBSET" command to serial communication about stack1 checked CBSET grid data
     * (handler about stack1 cbset button click).
     */
    void onClikedCBSETButton_STK1();

    /**
     * @brief send "*CBSET" command to serial communication about stack2 checked CBSET grid data
     * (handler about stack2 cbset button click).
     */
    void onClikedCBSETButton_STK2();

    void drawChartLabelBorder(bool bDraw);

private:
    /**
     * @brief initialize gui.
     */
    void init_gui();

    /**
     * @brief function to setting layout auto adjust..
     */
    void init_layout();

    /**
     * @brief initialize stack gui.
     *
     * @param[in] nStack    target stack
     */
    void init_stack(const stack& nStack);

    /**
     * @brief initialize CBSET gird of stack dashboard.
     * 
     * @param[in] nStack        target stack
     */
    void init_cb_grid(const stack& nStack);

    /**
     * @brief initialize CBSET button of stack dashboard.
     * 
     * @param[in] nStack        target stack
     */
    void init_cbset_button(const stack& nStack);

    /**
     * @brief initialize cell voltage bar chart for stack.
     * 
     * @param[in] nStack        target stack
     */
    void init_vc_chart(const stack& nStack);

    /**
     * @brief initialize cell voltage status grid of stack dashboard.
     * 
     * @param[in] nStack        target stack
     */
    void init_stat_grid(const stack& nStack);

    /**
     * @brief initialize sensor temperature value bar chart for stack.
     * 
     * @param[in] nStack        target stack
     */
    void init_sensor_temp_chart(const stack& nStack);

    /**
     * @brief initialize thermistor temperature value bar chart for stack.
     * 
     * @param[in] nStack        target stack
     */
    void init_thermistor_temp_chart(const stack& nStack);

    /**
     * @brief initialize temperature status grid of stack dashboard.
     * 
     * @param[in] nStack        target stack
     */
    void init_temp_grid(const stack& nStack);

    /**
     * @brief initialize VBAT gauge of stack dashboard.
     * 
     * @param[in] nStack        target stack
     */
    void init_vbat_gauge(const stack& nStack);

    /**
     * @brief initialize stacked battery voltage chart.
     */
    void init_sum_chart();

public:
    /**
     * @brief get VC data.
     * 
     * @return VC
     */
    MEAS_info::VC getMEAS_VC() const noexcept;

    /**
     * @brief get TEMP data.
     * 
     * @return TEMP
     */
    MEAS_info::TEMP getMEAS_TEMP() const noexcept;

    /**
     * @brief get VBAT data.
     * 
     * @return VBAT
     */
    MEAS_info::VBAT getMEAS_VBAT() const noexcept;

    /**
     * @brief get STAT data.
     * 
     * @return STAT
     */
    MEAS_info::STAT getMEAS_STAT() const noexcept;

    /**
     * @brief get CBTIME data.
     * 
     * @return CBTIME
     */
    CB_info::CBTIME getCB_TIME() const noexcept;

    /**
     * @brief get CBSET data from checked CBSET gird.
     * 
     * @param[in] nStack        target stack
     * @return CBSET string value
     */
    std::vector<std::wstring> getCBSET(const stack& nStack);

    /**
     * @brief set VC data.
     * 
     * @param[in] vc        setting VC
     */
    void setMEAS_VC(const MEAS_info::VC& vc);

    /**
     * @brief set TEMP data.
     * 
     * @param[in] temp      setting TEMP
     */
    void setMEAS_TEMP(const MEAS_info::TEMP& temp);

    /**
     * @brief set VBAT data.
     * 
     * @param[in] vbat      setting VBAT
     */
    void setMEAS_VBAT(const MEAS_info::VBAT& vbat);

    /**
     * @brief set STAT data.
     * 
     * @param[in] stat      setting STAT
     */
    void setMEAS_STAT(const MEAS_info::STAT& stat);

    /**
     * @brief set CBTIME data.
     * 
     * @param[in] cbtime    setting CBTIME
     */
    void setCB_TIME(const CB_info::CBTIME& cbtime);

    /**
     * @brief draw cell voltage bar chart.
     * 
     * @param[in] vc        drawing VC
     */
    void drawMEAS_VC(const MEAS_info::VC& vc);

    /**
     * @brief draw temperature value bar chart.
     * 
     * @param[in] temp      drawing TEMP
     */
    void drawMEAS_TEMP(const MEAS_info::TEMP& temp);

    /**
     * @brief draw VBAT gauge.
     * 
     * @param[in] vbat      drawing VBAT
     */
    void drawMEAS_VBAT(const MEAS_info::VBAT& vbat);

    /**
     * @brief draw status grid.
     * 
     * @param[in] stat      drawing STAT
     */
    void drawMEAS_STAT(const MEAS_info::STAT& stat);

    /**
     * @brief draw CBTIME data on CBSET grid.
     * 
     * @param[in] cbtime    drawing CBTIME
     */
    void drawCB_TIME(const CB_info::CBTIME& cbtime);

    /**
     * @brief draw cell voltage bar chart.
     * 
     * @param[in] nStack        target stack
     * @param[in] VC            drawing VC
     * @return 
     */
    bool drawVCChart(const stack& nStack, const std::vector<double>& VC);

    /**
     * @brief draw sensor temperature value bar chart.
     * 
     * @param[in] nStack        target stack
     * @param[in] TEMP          drawing TEMP
     * @return success or not
     */
    bool drawSensorTempChart(const stack& nStack, const std::vector<double>& TEMP);

    /**
     * @brief draw thermistor temperature value bar chart.
     * 
     * @param[in] nStack        target stack
     * @param[in] TEMP          drawing TEMP
     * @return success or not
     */
    bool drawThermistorTempChart(const stack& nStack, const std::vector<double>& TEMP);

    /**
     * @brief draw threshold line of cell voltage bar chart.
     * 
     * @param[in] nStack        target stack
     * @param[in] THR           drawing threshold value
     * @param[in] thr_color     drawing color
     * @return success or not
     */
    bool drawVCChart_THR(const stack& nStack, const double& THR, const CBCGPBrush& thr_color);

    /**
     * @brief draw threshold line of thermistor temperature bar chart.
     * 
     * @param[in] nStack        target stack
     * @param[in] TEMP_THR      drawing threshold value
     * @param[in] thr_color     drawing color
     * @return success or not
     */
    bool drawThermistorTempChart_THR(const stack& nStack, const double& TEMP_THR, const CBCGPBrush& thr_color);

    /**
     * @brief draw VBAT gauge.
     * 
     * @param[in] nStack        target stack
     * @param[in] vbat          drawing vbat value
     * @return success or not
     */
    bool drawVbatGauge(const stack& nStack, const double vbat);

    bool drawCBSET(const stack& nStack, const std::vector<std::wstring>& cbset);

    /**
     * @brief draw time left data to CBSET gird.
     * 
     * @param[in] nStack        target stack
     * @param[in] CBTIME_LEFT   drawing time left value
     * @return success or not
     */
    bool drawCBLeft(const stack& nStack, const std::vector<std::wstring>& CBTIME_LEFT);

    /**
     * @brief draw stacked battery voltage chart.
     * 
     * @param[in] vbat1         drawing stack1 vbat value
     * @param[in] vbat2         drawing stack2 vbat value
     * @param[in] sumVC1        drawing sum of stack1 cell voltage
     * @param[in] sumVC2        drawing sum of stack2 cell voltage
     * @return success or not
     */
    bool drawSumChart(const double vbat1, const double vbat2, const double sumVC1, const double sumVC2);

    /**
     * @brief draw OV stat on cell voltage status grid.
     * 
     * @param[in] nStack        target stack
     * @param[in] stat_ov       drawing ov value
     * @return success or not
     */
    bool drawStatOV(const stack& nStack, const std::vector<bool>& stat_ov);

    /**
     * @brief draw UV stat on cell voltage status grid.
     * 
     * @param[in] nStack        target stack
     * @param[in] stat_uv       drawing uv value
     * @return success or not
     */
    bool drawStatUV(const stack& nStack, const std::vector<bool>& stat_uv);

    /**
     * @brief draw VCB stat on cell voltage status grid.
     * 
     * @param[in] nStack        target stack
     * @param[in] stat_vcb      drawing vcb value
     * @return success or not
     */
    bool drawStatVCB(const stack& nStack, const std::vector<bool>& stat_vcb);

    /**
     * @brief draw UT stat on temperature status grid.
     * 
     * @param[in] nStack        target stack
     * @param[in] stat_ut       drawing ut value
     * @return success or not
     */
    bool drawStatUT(const stack& nStack, const std::vector<bool>& stat_ut);

    /**
     * @brief draw OT stat on temperature status grid.
     * 
     * @param[in] nStack        target stack
     * @param[in] stat_ot       drawing ot value
     * @return success or not
     */
    bool drawStatOT(const stack& nStack, const std::vector<bool>& stat_ot);

    /**
     * @brief draw OTCB stat on temperature status grid.
     * 
     * @param[in] nStack        target stack
     * @param[in] stat_otcb     drawing otcb value
     * @return success or not
     */
    bool drawStatOTCB(const stack& nStack, const std::vector<bool>& stat_otcb);

    /**
     * @brief clear threshold line of cell voltage chart.
     * 
     * @param[in] nStack        target stack
     */
    void clearVCChart_THR(const stack& nStack);

    /**
     * @brief clear threshold line of thermistor temperature chart.
     * 
     * @param[in] nStack        target stack
     */
    void clearThermistorTempChart_THR(const stack& nStack);

    /**
     * @brief clear data and ui.
     */
    void clear();

    /**
     * @brief enable edit cb grid.
     * 
     * @param[in] nStack        target stasck
     * @param[in] bEnable       enable or disable
     */
    void enableEditCBGrid(const stack& nStack, bool bEnable);

    /**
     * @brief enable dashboard stack.
     * 
     * @param[in] nStack        target stack
     * @param[in] bEnable       enable or disable
     */
    void enableStack(const stack& nStack, bool bEnable);

    /**
     * @brief checks whether the dashboard stack is enable.
     * 
     * @param[in] nStack        target stack
     * @return enable or disable
     */
    bool isEnableStack(const stack& nStack);

    void drawVCChartLabelBorder(const stack& nStack, bool bDraw);
    void drawSensorTempChartLabelBorder(const stack& nStack, bool bDraw);
    void drawThermistorTempChartLabelBorder(const stack& nStack, bool bDraw);

private:
    virtual BOOL OnMouseDown(int nButton, const CBCGPPoint& pt) override;

private:
    /**
     * @brief get container cbset grid object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container cbset grid object
     */
    CBCGPGridVisualObject& getCBGrid(const stack& nStack);

    /**
     * @brief get container cell voltage chart object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container cell voltage chart object
     */
    CBCGPChartVisualObject& getVCChart(const stack& nStack);

    /**
     * @brief get container cell voltage stat grid object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container cell voltage stat grid object
     */
    CBCGPGridVisualObject& getStatGrid(const stack& nStack);

    /**
     * @brief get container sensor temperature chart object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container sensor temperature chart object
     */
    CBCGPChartVisualObject& getSensorTempChart(const stack& nStack);

    /**
     * @brief get container thermistor temperature chart object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container thermistor temperature chart object
     */
    CBCGPChartVisualObject& getThermistorChart(const stack& nStack);

    /**
     * @brief get container temperature stat grid object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container temperature stat grid object
     */
    CBCGPGridVisualObject& getTempGrid(const stack& nStack);

    /**
     * @brief get container VBAT gauge object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container VBAT gauge object
     */
    CBCGPCircularGaugeImpl& getVBATGauge(const stack& nStack);

    /**
     * @brief get container sensor temperature title object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container sensor temperature title object
     */
    CBCGPTextGaugeImpl& getSensorTempTitle(const stack& nStack);

    /**
     * @brief get container thermistor temperature title object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container thermistor temperature title object
     */
    CBCGPTextGaugeImpl& getThermistorTitle(const stack& nStack);

    /**
     * @brief get container cbset grid button object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container cbset grid button object
     */
    CBCGPTextGaugeImpl& getCBSETButton(const stack& nStack);

private:
    /**
     * @brief function to access mainFrame class.
     *
     * @return mainFrame
     */
    mainFrame* getMainFrame() const noexcept;

private:
    CBCGPGridVisualObject& m_stk1_cb_grid;
    CBCGPChartVisualObject& m_stk1_vc_chart;
    CBCGPGridVisualObject& m_stk1_stat_grid;
    CBCGPChartVisualObject& m_stk1_temp1_chart;
    CBCGPChartVisualObject& m_stk1_temp2_chart;
    CBCGPGridVisualObject& m_stk1_temp_grid;
    CBCGPCircularGaugeImpl& m_stk1_vbat_gauge;
    CBCGPTextGaugeImpl& m_stk1_vc_title;
    CBCGPTextGaugeImpl& m_stk1_temp_title;
    CBCGPTextGaugeImpl& m_stk1_temp1_title;
    CBCGPTextGaugeImpl& m_stk1_temp2_title;
    CBCGPTextGaugeImpl& m_stk1_cbset_button;

    CBCGPGridVisualObject& m_stk2_cb_grid;
    CBCGPChartVisualObject& m_stk2_vc_chart;
    CBCGPGridVisualObject& m_stk2_stat_grid;
    CBCGPChartVisualObject& m_stk2_temp1_chart;
    CBCGPChartVisualObject& m_stk2_temp2_chart;
    CBCGPGridVisualObject& m_stk2_temp_grid;
    CBCGPCircularGaugeImpl& m_stk2_vbat_gauge;
    CBCGPTextGaugeImpl& m_stk2_vc_title;
    CBCGPTextGaugeImpl& m_stk2_temp_title;
    CBCGPTextGaugeImpl& m_stk2_temp1_title;
    CBCGPTextGaugeImpl& m_stk2_temp2_title;
    CBCGPTextGaugeImpl& m_stk2_cbset_button;

    CBCGPTextGaugeImpl& m_sumchart_title;
    CBCGPChartVisualObject& m_sum_chart;

private:
    MEAS_info::VC m_vc;
    MEAS_info::TEMP m_temp;
    MEAS_info::VBAT m_vbat;
    MEAS_info::STAT m_stat;
    CB_info::CBTIME m_leftTime;

    bool m_bCBSET_Pressed_STK1;
    bool m_bCBSET_Pressed_STK2;

    bool m_bEnable_STK1;
    bool m_bEnable_STK2;
}; //class dashboard_container : public CBCGPVisualContainer
