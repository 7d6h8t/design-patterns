#include "pch.h"
#include "vc_chart_container.h"
#include <exception>
#include <utils.h>
#include "dashboard_container.h"
#include "controls/legendCheckGridCtrl.h"
#include <functional>
#include "controls/legendGridItem.h"
#include <format>

IMPLEMENT_SERIAL(vc_chart_container, vc_chart_adapter, 1)

const std::vector<CBCGPColor> vc_chart_container::vc_chart_color = {
    CBCGPColor::Indigo,
    CBCGPColor::Gray,
    CBCGPColor::LightGreen,
    CBCGPColor::Brown,
    CBCGPColor::Purple,
    CBCGPColor::Orange,
    CBCGPColor::Pink,
    CBCGPColor::Yellow,
    CBCGPColor::Green,
    CBCGPColor::Red,
    CBCGPColor::Lime,
    CBCGPColor::Blue,
    CBCGPColor::Teal,
    CBCGPColor::LightGray,
    CBCGPColor::MediumPurple,
    CBCGPColor::OrangeRed,
    CBCGPColor::Cyan,
    CBCGPColor::LightBlue,
};

vc_chart_container::vc_chart_container() noexcept
    : m_stk1_vc_chart(getChartObject(VC_CHART_STK1)),
    m_stk1_vc_grid(getGridObject(VC_GRID_STK1)),
    m_stk2_vc_chart(getChartObject(VC_CHART_STK2)),
    m_stk2_vc_grid(getGridObject(VC_GRID_STK2))
{
    EnableLayout();
}

vc_chart_container::~vc_chart_container() noexcept
{
}

void vc_chart_container::OnAfterCreateWnd()
{
    vc_chart_adapter::OnAfterCreateWnd();
    init_gui();
    init_layout();
}

void vc_chart_container::drawMEAS_VC(const MEAS_info::VC& vc)
{
    drawVCChart(stack::stack1, vc.STK1);
    drawVCChart(stack::stack2, vc.STK2);
}

void vc_chart_container::drawVCChart(const stack& nStack, const std::vector<double>& VC)
{
    int32_t index = 0;
    for (const double value : VC)
    {
        CBCGPChartSeries* pVoltage_series = getVCChart(nStack).GetSeries(index);
        pVoltage_series->AddDataPoint(value);
        ++index;
    }

    getVCChart(nStack).SetDirty();
    getVCChart(nStack).Redraw();
}

void vc_chart_container::drawVCChart_THR(const stack& nStack, const double& THR, const CBCGPBrush& thr_color)
{
    CBCGPStrokeStyle type;
    type.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH);

    getVCChart(nStack).AddChartLineObject(THR, TRUE, thr_color, 2, &type)->SetShadow();
    getVCChart(nStack).AddChartAxisMarkObject(THR, std::format(L"{:.1f}", THR).c_str(), TRUE, TRUE, CBCGPBrush(CBCGPColor::White), thr_color);

    getVCChart(nStack).SetDirty();
    getVCChart(nStack).Redraw();
}

void vc_chart_container::showChartLine(const stack& nStack, const std::vector<int32_t> nRows, bool bVisible)
{
    for (const int32_t row : nRows)
        getVCChart(nStack).GetSeries(row)->m_bVisible = bVisible;

    getVCChart(nStack).SetDirty();
    getVCChart(nStack).Redraw();
}

void vc_chart_container::updateVCGrid(const uint32_t ctrlID, const std::vector<int32_t> nRows, const bool bVisible)
{
    if (ctrlID == VC_GRID_STK1)
        showChartLine(stack::stack1, nRows, bVisible);
    else if (ctrlID == VC_GRID_STK2)
        showChartLine(stack::stack2, nRows, bVisible);
}

void vc_chart_container::clearVCChart_THR(const stack& nStack)
{
    getVCChart(nStack).RemoveAllChartObjects();
    getVCChart(nStack).Invalidate();
}

void vc_chart_container::claer()
{
    clearVCChart_THR(stack::stack1);
    clearVCChart_THR(stack::stack2);

    init_gui();
}

void vc_chart_container::init_gui()
{
    try
    {
        init_stack(stack::stack1);
        init_stack(stack::stack2);

        Redraw();
    }
    catch (const std::exception& e) {
        DebugPrint("ERROR : chart init gui fail [{}]", e.what());
    }
}

void vc_chart_container::init_layout()
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

    pLayout->AddAnchor(GetByID(VC_TITLE_STK1), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 50));
    pLayout->AddAnchor(&m_stk1_vc_chart, CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 50), CPoint(100, 50));
    pLayout->AddAnchor(&m_stk1_vc_grid, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(100, 50), CPoint(100, 50));

    pLayout->AddAnchor(GetByID(VC_TITLE_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 50));
    pLayout->AddAnchor(&m_stk2_vc_chart, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 50), CPoint(100, 50));
    pLayout->AddAnchor(&m_stk2_vc_grid, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(100, 50), CPoint(100, 50));
}

void vc_chart_container::init_stack(const stack& nStack)
{
    init_vc_chart(nStack);
    init_vc_grid(nStack);
}

void vc_chart_container::init_vc_chart(const stack& nStack)
{
    getVCChart(nStack).SetAutoDestroy(TRUE);
    getVCChart(nStack).CleanUp();

    getVCChart(nStack).SetChartType(BCGPChartHistoricalLine, BCGP_CT_SIMPLE, FALSE);
    getVCChart(nStack).SetLegendPosition(BCGPChartLayout::LP_NONE);
    getVCChart(nStack).ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);

    CBCGPChartAxis* pXAxis = getVCChart(nStack).GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pXAxis->m_axisLabelType = CBCGPChartAxis::ALT_NO_LABELS;
    pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
    pXAxis->SetFixedIntervalWidth(vc_chart_interval_width, vc_chart_interval_x);

    CBCGPChartAxis* pYAxis = getVCChart(nStack).GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pYAxis->SetFixedDisplayRange(vc_chart_min_y, vc_chart_max_y, vc_chart_unit_y);
    pYAxis->m_bDisplayAxisName = FALSE;

    for (int32_t i = 0; i < dashboard_container::cb_max_row; ++i)
    {
        CBCGPChartSeries* series = getVCChart(nStack).CreateSeries(std::format(L"VC {}", i + 1).c_str());
        series->SetSeriesLineColor(CBCGPBrush(vc_chart_color[i]));
        series->SetSeriesLineWidth(vc_chart_lineWidth);
    }

    // 모든 series 의 show off 시 background 도 날라가는 현상 발생 -> 방지하기 위해 보이지 않는 dummy 추가
    CBCGPChartSeries* dummy = getVCChart(nStack).CreateSeries(_T("dummy"));
    dummy->SetSeriesLineWidth(0);
}

void vc_chart_container::init_vc_grid(const stack& nStack)
{
    getVCGrid(nStack).SetAutoDestroy(TRUE);

    legendCheckGridCtrl* grid = dynamic_cast<legendCheckGridCtrl*>(getVCGrid(nStack).GetGridCtrl());
    grid->RemoveAll();
    grid->DeleteAllColumns();

    using namespace std::placeholders;
    grid->registCallback(std::bind(&vc_chart_container::updateVCGrid, this, _1, _2, _3));
    grid->EnableHeader(TRUE, 0);
    grid->EnableRowHeader(FALSE, BCGP_GRID_HEADER_SELECT);
    grid->EnableCheckBoxes(TRUE);
    grid->EnableColumnAutoSize(TRUE);
    grid->EnableGridLines(FALSE);
    grid->SetSelectionBorder(FALSE);

    grid->InsertColumn(0, _T("Line name"), 40);

    for (int i = 0; i < dashboard_container::cb_max_row; ++i)
    {
        CBCGPGridRow* pRow = grid->CreateRow(grid->GetColumnCount());
        grid->AddRow(pRow, FALSE);
        grid->SetReadOnly(TRUE);
        pRow->SetCheck(BST_CHECKED);

        pRow->ReplaceItem(0, new legendGridItem(vc_chart_color[i], std::format(L"VC {}", i + 1)));
    }
    grid->AdjustLayout();
}

CBCGPChartVisualObject& vc_chart_container::getVCChart(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_vc_chart : m_stk2_vc_chart;
}

CBCGPGridVisualObject& vc_chart_container::getVCGrid(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_vc_grid : m_stk2_vc_grid;
}
