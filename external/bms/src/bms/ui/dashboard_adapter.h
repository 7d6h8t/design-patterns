/*****************************************************************//**
 * @file   dashboard_adapter.h
 * @brief  adapter class for dashboard_container class.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "resource.h"
#include <cstdint>

/**
 * @brief adapter class for dashboard_container class.
 */
class dashboard_adapter : public CBCGPVisualContainer
{
public:
    enum
    {
        CB_TITLE_STK1 = 1001,
        TEMP_TITLE_STK1 = 1002,
        VBAT_TITLE_STK1 = 1003,
        CB_STATIC_STK1 = 1004,
        TEMP_STATIC_STK1 = 1005,
        VBAT_STATIC_STK1 = 1006,
        TEMP_SENSOR_TITLE_STK1 = 1007,
        TEMP_THERMISTOR_TITLE_STK1 = 1008,
        VC_TITLE_STK1 = 1009,
        TEMP_CHART_TITLE_STK1 = 1010,
        CB_GRID_STK1 = 102,
        VC_CHART_STK1 = 103,
        VC_STAT_GRID_STK1 = 104,
        TEMP_CHART1_STK1 = 105,
        TEMP_CHART2_STK1 = 106,
        TEMP_STAT_GRID_STK1 = 107,
        VBAT_GAUGE_STK1 = 108,
        CBSET_BUTTON_STK1 = 109,

        CB_TITLE_STK2 = 2001,
        TEMP_TITLE_STK2 = 2002,
        VBAT_TITLE_STK2 = 2003,
        CB_STATIC_STK2 = 2004,
        TEMP_STATIC_STK2 = 2005,
        VBAT_STATIC_STK2 = 2006,
        TEMP_SENSOR_TITLE_STK2 = 2007,
        TEMP_THERMISTOR_TITLE_STK2 = 2008,
        VC_TITLE_STK2 = 2009,
        TEMP_CHART_TITLE_STK2 = 2010,
        CB_GRID_STK2 = 202,
        VC_CHART_STK2 = 203,
        VC_STAT_GRID_STK2 = 204,
        TEMP_CHART1_STK2 = 205,
        TEMP_CHART2_STK2 = 206,
        TEMP_STAT_GRID_STK2 = 207,
        VBAT_GAUGE_STK2 = 208,
        CBSET_BUTTON_STK2 = 209,

        SUM_CHART_TITLE = 3001,
        SUM_CHART = 301,
    };

public:
    /**
     * @brief constructs the object.
     */
    dashboard_adapter() noexcept;

    /**
     * @brief destructs the object.
     */
    virtual ~dashboard_adapter() noexcept;

    // nonecopyable
    dashboard_adapter(const dashboard_adapter&) = delete;
    dashboard_adapter& operator=(const dashboard_adapter&) = delete;
    // nonemovable
    dashboard_adapter(dashboard_adapter&&) = delete;
    dashboard_adapter& operator=(dashboard_adapter&&) = delete;

public:
    /**
     * @brief get dashboard grid object with .visual-bcgsoft file's resource id.
     * 
     * @param[in] nId       .visual-bcgsoft file's resource id
     * @return dashboard grid object
     */
    CBCGPGridVisualObject& getGridObject(const uint32_t nId);

    /**
     * @brief get dashboard chart object with .visual-bcgsoft file's resource id.
     * 
     * @param[in] nId       .visual-bcgsoft file's resource id
     * @return dashboard chart object
     */
    CBCGPChartVisualObject& getChartObject(const uint32_t nId);

    /**
     * @brief get dashboard gauge object with .visual-bcgsoft file's resource id.
     * 
     * @param[in] nId       .visual-bcgsoft file's resource id
     * @return dashboard gauge object
     */
    CBCGPCircularGaugeImpl& getGaugeImpl(const uint32_t nId);

    /**
     * @brief get dashboard text object with .visual-bcgsoft file's resource id.
     * 
     * @param[in] nId       .visual-bcgsoft file's resource id
     * @return dashboard text object
     */
    CBCGPTextGaugeImpl& getTextImpl(const uint32_t nId);

protected:
    /**
     * @brief return changed fill color of CBSET button at button press.
     *
     * @param[in] bPressed      pressed or not
     * @return button press is DeepSkyBlue, button not press is WhiteSmoke
     */
    CBCGPBrush getCBSETButton_fillcolor(bool bPressed);

    /**
     * @brief return changed outline color of CBSET button at button press.
     *
     * @param[in] bPressed      pressed or not
     * @return button press is DodgerBlue, button not press is LightGray
     */
    CBCGPBrush getCBSETButton_outlinecolor(bool bPressed);

    /**
     * @brief return changed text color of CBSET button at button press.
     *
     * @param[in] bPressed      pressed or not
     * @return button press is White, button not press is Gray
     */
    CBCGPBrush getCBSETButton_textcolor(bool bPressed);

public:
    static constexpr size_t cb_max_row = 18;
    static constexpr size_t sensor_temp_max_row = 3;
    static constexpr size_t thermistor_temp_max_row = 10;

    static constexpr double vbat_min = 0;
    static constexpr double vbat_max = 90;

    static const CBCGPBrush VC_default;
    static const CBCGPBrush SensorTemp_default;

    static const CBCGPBrush OV_thr_color;
    static const CBCGPBrush VCB_thr_color;
    static const CBCGPBrush UV_thr_color;

    static const CBCGPBrush OT_thr_color;
    static const CBCGPBrush OTCB_thr_color;
    static const CBCGPBrush UT_thr_color;

    static const CBCGPBrush CBSET_defualt_fill_color;
    static const CBCGPBrush CBSET_defualt_outline_color;
    static const CBCGPBrush CBSET_defualt_text_color;

    static const CBCGPBrush CBSET_defualt_dark_fill_color;
    static const CBCGPBrush CBSET_defualt_dark_outline_color;
    static const CBCGPBrush CBSET_defualt_dark_text_color;

    static const CBCGPBrush CBSET_pressed_fill_color;
    static const CBCGPBrush CBSET_pressed_outline_color;
    static const CBCGPBrush CBSET_pressed_text_color;

    static constexpr double vc_chart_label_thr = 1.5;
    static constexpr double temp_chart_label_thr = 20;
};
