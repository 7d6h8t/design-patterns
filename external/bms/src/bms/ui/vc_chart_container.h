/*****************************************************************//**
 * @file   vc_chart_container.h
 * @brief  cell voltage historical chart container class.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "vc_chart_adapter.h"
#include "MEAS_info.h"
#include <vector>

/**
 * @brief cell voltage historical chart container class.
 */
class vc_chart_container final : public vc_chart_adapter
{
public:
    DECLARE_SERIAL(vc_chart_container)

    enum class stack : uint32_t
    {
        stack1 = 1,
        stack2
    };

public:
    /**
     * @brief constructs the object.
     */
    vc_chart_container() noexcept;

    /**
     * @brief destructs the object.
     */
    ~vc_chart_container() noexcept;

public:
    virtual void OnAfterCreateWnd() override;

public:
    /**
     * @brief draw historical chart about VC.
     * 
     * @param[in] vc        drawing VC
     */
    void drawMEAS_VC(const MEAS_info::VC& vc);

    /**
     * @brief draw historical chart about specified stack cell voltage.
     * 
     * @param[in] nStack        drawing target stack
     * @param[in] VC            cell volatage
     */
    void drawVCChart(const stack& nStack, const std::vector<double>& VC);

    /**
     * @brief draw threshold line of chart about specified stack, value, color.
     * 
     * @param[in] nStack        drawing target stack
     * @param[in] THR           drawing threshold value
     * @param[in] thr_color     drawing color
     */
    void drawVCChart_THR(const stack& nStack, const double& THR, const CBCGPBrush& thr_color);

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
    void updateVCGrid(const uint32_t ctrlID, const std::vector<int32_t> nRows, const bool bVisible);

    /**
     * @brief clear stack chart.
     * 
     * @param[in] nStack        target stack
     */
    void clearVCChart_THR(const stack& nStack);

    /**
     * @brief clear ui.
     */
    void claer();

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
     * @brief initialize stack cell voltage historical chart.
     * 
     * @param[in] nStack    target stack
     */
    void init_vc_chart(const stack& nStack);

    /**
     * @brief initialize legend grid of stack chart.
     * 
     * @param[in] nStack    target stack
     */
    void init_vc_grid(const stack& nStack);

private:
    /**
     * @brief get container chart object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container chart object
     */
    CBCGPChartVisualObject& getVCChart(const stack& nStack);

    /**
     * @brief get container grid object of specified stack.
     * 
     * @param[in] nStack        target stack
     * @return container grid object
     */
    CBCGPGridVisualObject& getVCGrid(const stack& nStack);

private:
    CBCGPChartVisualObject& m_stk1_vc_chart;
    CBCGPGridVisualObject& m_stk1_vc_grid;

    CBCGPChartVisualObject& m_stk2_vc_chart;
    CBCGPGridVisualObject& m_stk2_vc_grid;

public:
    constexpr static double vc_chart_min_y = 0;
    constexpr static double vc_chart_max_y = 5;
    constexpr static double vc_chart_unit_y = 1;

    constexpr static int32_t vc_chart_interval_width = 40;
    constexpr static int32_t vc_chart_interval_x = 10;

    constexpr static double vc_chart_lineWidth = 2;

    static const std::vector<CBCGPColor> vc_chart_color;
};
