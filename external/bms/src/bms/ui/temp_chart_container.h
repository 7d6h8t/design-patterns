/*****************************************************************//**
 * @file   temp_chart_container.h
 * @brief  temperature value historical chart container class.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "temp_chart_adapter.h"
#include "MEAS_info.h"
#include <vector>

/**
 * @brief temperature value historical chart container class.
 */
class temp_chart_container final : public temp_chart_adapter
{
public:
    DECLARE_SERIAL(temp_chart_container)

    enum class stack : uint32_t
    {
        stack1 = 1,
        stack2
    };

public:
    /**
     * @brief constructs the object.
     */
    temp_chart_container() noexcept;

    /**
     * @brief destructs the object.
     */
    ~temp_chart_container() noexcept;

public:
    virtual void OnAfterCreateWnd() override;

public:
    /**
     * @brief draw historical chart about TEMP.
     * 
     * @param[in] temp      drawing TEMP
     */
    void drawMEAS_TEMP(const MEAS_info::TEMP& temp);

    /**
     * @brief draw historical chart about specified stack temperature value.
     * 
     * @param[in] nStack            drawing target stack
     * @param[in] thermistor        thermistor temperature values
     * @param[in] sensor            sensor temperature values
     */
    void drawTEMPChart(const stack& nStack, const std::vector<double>& thermistor, const std::vector<double>& sensor);

    /**
     * @brief draw threshold line of chart about specified stack, value, color..
     * 
     * @param[in] nStack        drawing target stack
     * @param[in] THR           drawing threshold value
     * @param[in] thr_color     drawing color
     */
    void drawTEMPChart_THR(const stack& nStack, const double& THR, const CBCGPBrush& thr_color);

    /**
     * @brief show chart line enable or disable about specified legend of stack.
     *
     * @param[in] nStack        target stack
     * @param[in] nRows         target legend grid row
     * @param[in] bVisible      show enable or disable
     */
    void showChartLine(const stack& nStack, const std::vector<int32_t> nRows, bool bVisible);

    /**
     * @brief update viewing chart line (callback handler about legend grid checkbox operation).
     *
     * @param[in] ctrlID        grid control resource id
     * @param[in] nRows         grid row number about checkbox operation
     * @param[in] bVisible      visible on, off
     */
    void updateTEMPGrid(const uint32_t ctrlID, const std::vector<int32_t> nRows, const bool bVisible);

    /**
     * @brief clear stack chart.
     *
     * @param[in] nStack        target stack
     */
    void clearTEMPChart_THR(const stack& nStack);

    /**
     * @brief clear ui.
     */
    void clear();

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
     * @brief initialize stack temperature value historical chart.
     *
     * @param[in] nStack    target stack
     */
    void init_temp_chart(const stack& nStack);

    /**
     * @brief initialize legend grid of stack chart.
     *
     * @param[in] nStack    target stack
     */
    void init_temp_grid(const stack& nStack);

private:
    /**
     * @brief get container chart object of specified stack.
     *
     * @param[in] nStack        target stack
     * @return container chart object
     */
    CBCGPChartVisualObject& getTEMPChart(const stack& nStack);

    /**
     * @brief get container grid object of specified stack.
     *
     * @param[in] nStack        target stack
     * @return container grid object
     */
    CBCGPGridVisualObject& getTEMPGrid(const stack& nStack);

private:
    CBCGPChartVisualObject& m_stk1_temp_chart;
    CBCGPGridVisualObject& m_stk1_temp_grid;

    CBCGPChartVisualObject& m_stk2_temp_chart;
    CBCGPGridVisualObject& m_stk2_temp_grid;

    MEAS_info::TEMP m_temp;

public:
    constexpr static double temp_chart_min_y = -40;
    constexpr static double temp_chart_max_y = 125;
    constexpr static double temp_chart_unit_y = 20;

    constexpr static int32_t temp_chart_interval_width = 40;
    constexpr static int32_t temp_chart_interval_x = 10;

    constexpr static double temp_chart_lineWidth = 2;

    static const std::vector<CBCGPColor> thermistor_temp_color;
    static const std::vector<CBCGPColor> sensor_temp_color;
};
