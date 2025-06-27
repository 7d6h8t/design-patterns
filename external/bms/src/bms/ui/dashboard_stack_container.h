#pragma once
#include "dashboard_view_stack_adapter.h"
#include "info/view_mode.h"
#include <vector>
#include <string>

class mainFrame;

class dashboard_stack_container final : public dashboard_view_stack_adapter
{
public:
    dashboard_stack_container(const view_mode::dashboard_view_option stack) noexcept;
    ~dashboard_stack_container() noexcept;

    // nonecopyable
    dashboard_stack_container(const dashboard_stack_container&) = delete;
    dashboard_stack_container& operator=(const dashboard_stack_container&) = delete;
    // nonemovable
    dashboard_stack_container(dashboard_stack_container&&) = delete;
    dashboard_stack_container& operator=(dashboard_stack_container&&) = delete;

public:
    virtual void OnAfterCreateWnd() override;
    void onChangeTheme();
    void onClikedCBSETButton();
    bool isPressedCBSET();

    void enableStack(bool bEnable);

    std::vector<std::wstring> getCBSET();

private:
    void init_gui();
    void init_layout();

    void init_cb_grid();
    void init_cbset_button();
    void init_vc_chart();
    void init_stat_grid();
    void init_sensor_temp_chart();
    void init_thermistor_temp_chart();
    void init_temp_grid();

public:
    void enableEditCBGrid(bool bEnable);

    void drawChartLabelBorder(bool bDraw);
    void drawVCChartLabelBorder(bool bDraw);
    void drawSensorTempChartLabelBorder(bool bDraw);
    void drawThermistorTempChartLabelBorder(bool bDraw);

    bool drawVCChart(const std::vector<double>& VC);
    bool drawSensorTempChart(const std::vector<double>& TEMP);
    bool drawThermistorTempChart(const std::vector<double>& TEMP);
    bool drawVCChart_THR(const double& THR, const CBCGPBrush& thr_color);
    bool drawThermistorTempChart_THR(const double& TEMP_THR, const CBCGPBrush& thr_color);
    bool drawCBSET(const std::vector<std::wstring>& cbset);
    bool drawCBLeft(const std::vector<std::wstring>& CBTIME_LEFT);
    bool drawStatOV(const std::vector<bool>& stat_ov);
    bool drawStatUV(const std::vector<bool>& stat_uv);
    bool drawStatVCB(const std::vector<bool>& stat_vcb);
    bool drawStatUT(const std::vector<bool>& stat_ut);
    bool drawStatOT(const std::vector<bool>& stat_ot);
    bool drawStatOTCB(const std::vector<bool>& stat_otcb);

    void clearVCChart_THR();
    void clearThermistorTempChart_THR();
    void clear();

    bool isEnableStack();

private:
    virtual BOOL OnMouseDown(int nButton, const CBCGPPoint& pt) override;

private:
    mainFrame* getMainFrame() const noexcept;

private:
    CBCGPGridVisualObject& m_stk_cb_grid;
    CBCGPChartVisualObject& m_stk_vc_chart;
    CBCGPGridVisualObject& m_stk_stat_grid;
    CBCGPChartVisualObject& m_stk_sensor_temp_chart;
    CBCGPChartVisualObject& m_stk_thermistor_temp_chart;
    CBCGPGridVisualObject& m_stk_temp_grid;
    CBCGPTextGaugeImpl& m_stk_vc_title;
    CBCGPTextGaugeImpl& m_stk_temp_title;
    CBCGPTextGaugeImpl& m_stk_sensor_temp_title;
    CBCGPTextGaugeImpl& m_stk_thermistor_temp_title;
    CBCGPTextGaugeImpl& m_stk_cbset_button;

    bool m_bCBSET_Pressed;
    bool m_bEnable;

    view_mode::dashboard_view_option m_stack;
    static const float m_label_font_size;
};
