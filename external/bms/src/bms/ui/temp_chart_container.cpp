#include "pch.h"
#include "temp_chart_container.h"
#include "dashboard_container.h"
#include "controls/legendCheckGridCtrl.h"
#include "controls/legendGridItem.h"
#include <utils.h>
#include <format>

IMPLEMENT_SERIAL(temp_chart_container, temp_chart_adapter, 1)

const std::vector<CBCGPColor> temp_chart_container::thermistor_temp_color = {
    CBCGPColor::Indigo,
    CBCGPColor::Gray,
    CBCGPColor::LightGreen,
    CBCGPColor::Brown,
    CBCGPColor::Purple,
    CBCGPColor::Orange,
    CBCGPColor::Pink,
    CBCGPColor::Yellow,
    CBCGPColor::Green,
    CBCGPColor::Red
};

const std::vector<CBCGPColor> temp_chart_container::sensor_temp_color = {
    CBCGPColor::Lime,
    CBCGPColor::Blue,
    CBCGPColor::LightGray
};

temp_chart_container::temp_chart_container() noexcept
    : m_stk1_temp_chart(getChartObject(TEMP_CHART_STK1)),
    m_stk1_temp_grid(getGridObject(TEMP_GRID_STK1)),
    m_stk2_temp_chart(getChartObject(TEMP_CHART_STK2)),
    m_stk2_temp_grid(getGridObject(TEMP_GRID_STK2))
{
    EnableLayout();
}

temp_chart_container::~temp_chart_container() noexcept
{
}

void temp_chart_container::OnAfterCreateWnd()
{
    temp_chart_adapter::OnAfterCreateWnd();
    init_gui();
    init_layout();
}

void temp_chart_container::drawMEAS_TEMP(const MEAS_info::TEMP& temp)
{
    drawTEMPChart(stack::stack1, temp.STK1_thermistor, temp.STK1_sensor);
    drawTEMPChart(stack::stack2, temp.STK2_thermistor, temp.STK2_sensor);
}

void temp_chart_container::drawTEMPChart(const stack& nStack, const std::vector<double>& thermistor, const std::vector<double>& sensor)
{
    int32_t index = 0;
    for (const double value : thermistor)
    {
        CBCGPChartSeries* pVoltage_series = getTEMPChart(nStack).GetSeries(index);
        pVoltage_series->AddDataPoint(value);
        ++index;
    }

    for (const double value : sensor)
    {
        CBCGPChartSeries* pVoltage_series = getTEMPChart(nStack).GetSeries(index);
        pVoltage_series->AddDataPoint(value);
        ++index;
    }

    getTEMPChart(nStack).SetDirty();
    getTEMPChart(nStack).Redraw();
}

void temp_chart_container::drawTEMPChart_THR(const stack& nStack, const double& THR, const CBCGPBrush& thr_color)
{
    CBCGPStrokeStyle type;
    type.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH);

    getTEMPChart(nStack).AddChartLineObject(THR, TRUE, thr_color, 2, &type)->SetShadow();
    getTEMPChart(nStack).AddChartAxisMarkObject(THR, std::format(L"{:.0f}", THR).c_str(), TRUE, TRUE, CBCGPBrush(CBCGPColor::White), thr_color);

    getTEMPChart(nStack).SetDirty();
    getTEMPChart(nStack).Redraw();
}

void temp_chart_container::showChartLine(const stack& nStack, const std::vector<int32_t> nRows, bool bVisible)
{
    for (const int32_t row : nRows)
        getTEMPChart(nStack).GetSeries(row)->m_bVisible = bVisible;

    getTEMPChart(nStack).SetDirty();
    getTEMPChart(nStack).Redraw();
}

void temp_chart_container::updateTEMPGrid(const uint32_t ctrlID, const std::vector<int32_t> nRows, const bool bVisible)
{
    if (ctrlID == TEMP_GRID_STK1)
        showChartLine(stack::stack1, nRows, bVisible);
    else if (ctrlID == TEMP_GRID_STK2)
        showChartLine(stack::stack2, nRows, bVisible);
}

void temp_chart_container::clearTEMPChart_THR(const stack& nStack)
{
    getTEMPChart(nStack).RemoveAllChartObjects();
    getTEMPChart(nStack).Invalidate();
}

void temp_chart_container::clear()
{
    clearTEMPChart_THR(stack::stack1);
    clearTEMPChart_THR(stack::stack2);

    init_gui();
}

void temp_chart_container::init_gui()
{
    try
    {
        init_stack(stack::stack1);
        init_stack(stack::stack2);

        Redraw();
    }
    catch (const std::exception& e) {
        DebugPrint("ERROR : temp chart init gui fail [{}]", e.what());
    }
}

void temp_chart_container::init_layout()
{
    CBCGPVisualLayout* pLayout = GetLayout();
    ASSERT_VALID(pLayout);

    CSize szMin(GetRect().Size());
    CBCGPRect rect(GetBoundsRect());
    if (szMin == CSize(0, 0))
    {
        CBCGPRect rect(GetBoundsRect());
        szMin = CSize((int)(rect.Width() + rect.left), (int)(rect.Height() + rect.top));
    }

    pLayout->SetMinSize(szMin);

    pLayout->AddAnchor(GetByID(TEMP_TITLE_STK1), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 50));
    pLayout->AddAnchor(&m_stk1_temp_chart, CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 50), CPoint(100, 50));
    pLayout->AddAnchor(&m_stk1_temp_grid, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(100, 50));

    pLayout->AddAnchor(GetByID(TEMP_TITLE_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 50));
    pLayout->AddAnchor(&m_stk2_temp_chart, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 50), CPoint(100, 50));
    pLayout->AddAnchor(&m_stk2_temp_grid, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(100, 50));
}

void temp_chart_container::init_stack(const stack& nStack)
{
    init_temp_chart(nStack);
    init_temp_grid(nStack);
}

void temp_chart_container::init_temp_chart(const stack& nStack)
{
    getTEMPChart(nStack).SetAutoDestroy(TRUE);
    getTEMPChart(nStack).CleanUp();

    getTEMPChart(nStack).SetChartType(BCGPChartHistoricalLine, BCGP_CT_SIMPLE, FALSE);
    getTEMPChart(nStack).SetLegendPosition(BCGPChartLayout::LP_NONE);
    getTEMPChart(nStack).ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);

    CBCGPChartAxis* pXAxis = getTEMPChart(nStack).GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pXAxis->m_axisLabelType = CBCGPChartAxis::ALT_NO_LABELS;
    pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
    pXAxis->SetFixedIntervalWidth(temp_chart_interval_width, temp_chart_interval_x);

    CBCGPChartAxis* pYAxis = getTEMPChart(nStack).GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pYAxis->SetFixedDisplayRange(temp_chart_min_y, temp_chart_max_y, temp_chart_unit_y);
    pYAxis->m_bDisplayAxisName = FALSE;

    for (int32_t i = 0; i < dashboard_container::thermistor_temp_max_row; ++i)
    {
        CBCGPChartSeries* series = getTEMPChart(nStack).CreateSeries(std::format(L"NTC {}", i + 1).c_str());
        series->SetSeriesLineColor(CBCGPBrush(thermistor_temp_color[i]));
        series->SetSeriesLineWidth(temp_chart_lineWidth);
    }

    for (int32_t i = 0; i < dashboard_container::sensor_temp_max_row; ++i)
    {
        CBCGPChartSeries* series = getTEMPChart(nStack).CreateSeries(std::format(L"DT {} °C", i + 1).c_str());
        series->SetSeriesLineColor(CBCGPBrush(sensor_temp_color[i]));
        series->SetSeriesLineWidth(temp_chart_lineWidth);
    }

    // 모든 series 의 show off 시 background 도 날라가는 현상 발생 -> 방지하기 위해 보이지 않는 dummy 추가
    CBCGPChartSeries* dummy = getTEMPChart(nStack).CreateSeries(_T("dummy"));
    dummy->SetSeriesLineWidth(0);
}

void temp_chart_container::init_temp_grid(const stack& nStack)
{
    getTEMPGrid(nStack).SetAutoDestroy(TRUE);

    legendCheckGridCtrl* grid = dynamic_cast<legendCheckGridCtrl*>(getTEMPGrid(nStack).GetGridCtrl());
    grid->RemoveAll();
    grid->DeleteAllColumns();

    using namespace std::placeholders;
    grid->registCallback(std::bind(&temp_chart_container::updateTEMPGrid, this, _1, _2, _3));
    grid->EnableHeader(TRUE, 0);
    grid->EnableRowHeader(FALSE, BCGP_GRID_HEADER_SELECT);
    grid->EnableCheckBoxes(TRUE);
    grid->EnableColumnAutoSize(TRUE);
    grid->EnableGridLines(FALSE);
    grid->SetSelectionBorder(FALSE);

    grid->InsertColumn(0, _T("Line name"), 40);

    for (int i = 0; i < dashboard_container::thermistor_temp_max_row; ++i)
    {
        CBCGPGridRow* pRow = grid->CreateRow(grid->GetColumnCount());
        grid->AddRow(pRow, FALSE);
        grid->SetReadOnly(TRUE);
        pRow->SetCheck(BST_CHECKED);

        pRow->ReplaceItem(0, new legendGridItem(thermistor_temp_color[i], std::format(L"NTC {}", i + 1)));
    }

    for (int i = 0; i < dashboard_container::sensor_temp_max_row; ++i)
    {
        CBCGPGridRow* pRow = grid->CreateRow(grid->GetColumnCount());
        grid->AddRow(pRow, FALSE);
        grid->SetReadOnly(TRUE);
        pRow->SetCheck(BST_CHECKED);

        pRow->ReplaceItem(0, new legendGridItem(sensor_temp_color[i], std::format(L"DT {}", i + 1)));
    }

    grid->AdjustLayout();
}

CBCGPChartVisualObject& temp_chart_container::getTEMPChart(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_temp_chart : m_stk2_temp_chart;
}

CBCGPGridVisualObject& temp_chart_container::getTEMPGrid(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_temp_grid : m_stk2_temp_grid;
}
