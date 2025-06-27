#include "pch.h"
#include "dashboard_container.h"
#include <exception>
#include <utils.h>
#include <format>
#include "controls/cbsetGridItem.h"
#include "controls/colorIndicaterGridItem.h"
#include "controls/autoSizeGridCtrl.h"
#include "mainFrame.h"
#include "cmdmanager.h"
#include "vc_chart_container.h"
#include "temp_chart_container.h"
#include "bmsApp.h"
#include "info/grid_info.h"
#include "controls/barLabelPosChangeImpl.h"

IMPLEMENT_SERIAL(dashboard_container, dashboard_view_all_adapter, 1)

dashboard_container::dashboard_container() noexcept
    : m_stk1_cb_grid(getGridObject(CB_GRID_STK1)),
    m_stk1_vc_chart(getChartObject(VC_CHART_STK1)),
    m_stk1_stat_grid(getGridObject(VC_STAT_GRID_STK1)),
    m_stk1_temp1_chart(getChartObject(TEMP_CHART1_STK1)),
    m_stk1_temp2_chart(getChartObject(TEMP_CHART2_STK1)),
    m_stk1_temp_grid(getGridObject(TEMP_STAT_GRID_STK1)),
    m_stk1_vbat_gauge(getGaugeImpl(VBAT_GAUGE_STK1)),
    m_stk1_temp1_title(getTextImpl(TEMP_SENSOR_TITLE_STK1)),
    m_stk1_temp2_title(getTextImpl(TEMP_THERMISTOR_TITLE_STK1)),
    m_stk1_vc_title(getTextImpl(VC_TITLE_STK1)),
    m_stk1_temp_title(getTextImpl(TEMP_CHART_TITLE_STK1)),
    m_stk1_cbset_button(getTextImpl(CBSET_BUTTON_STK1)),
    m_stk2_cb_grid(getGridObject(CB_GRID_STK2)),
    m_stk2_vc_chart(getChartObject(VC_CHART_STK2)),
    m_stk2_stat_grid(getGridObject(VC_STAT_GRID_STK2)),
    m_stk2_temp1_chart(getChartObject(TEMP_CHART1_STK2)),
    m_stk2_temp2_chart(getChartObject(TEMP_CHART2_STK2)),
    m_stk2_temp_grid(getGridObject(TEMP_STAT_GRID_STK2)),
    m_stk2_vbat_gauge(getGaugeImpl(VBAT_GAUGE_STK2)),
    m_stk2_temp1_title(getTextImpl(TEMP_SENSOR_TITLE_STK2)),
    m_stk2_temp2_title(getTextImpl(TEMP_THERMISTOR_TITLE_STK2)),
    m_stk2_vc_title(getTextImpl(VC_TITLE_STK2)),
    m_stk2_temp_title(getTextImpl(TEMP_CHART_TITLE_STK2)),
    m_stk2_cbset_button(getTextImpl(CBSET_BUTTON_STK2)),
    m_sumchart_title(getTextImpl(SUM_CHART_TITLE)),
    m_sum_chart(getChartObject(SUM_CHART)),
    m_bCBSET_Pressed_STK1(false),
    m_bCBSET_Pressed_STK2(false),
    m_bEnable_STK1(true),
    m_bEnable_STK2(true)
{
    EnableLayout();
}

dashboard_container::~dashboard_container() noexcept
{
}

void dashboard_container::OnAfterCreateWnd()
{
    dashboard_view_all_adapter::OnAfterCreateWnd();
    init_gui();
    init_layout();
}

void dashboard_container::onChangeTheme()
{
    getCBSETButton(stack::stack1).SetFillBrush(getCBSETButton_fillcolor(m_bCBSET_Pressed_STK1));
    getCBSETButton(stack::stack1).SetOutlineBrush(getCBSETButton_outlinecolor(m_bCBSET_Pressed_STK1));
    getCBSETButton(stack::stack1).SetTextBrush(getCBSETButton_textcolor(m_bCBSET_Pressed_STK1));
    getCBSETButton(stack::stack1).Redraw();

    getCBSETButton(stack::stack2).SetFillBrush(getCBSETButton_fillcolor(m_bCBSET_Pressed_STK2));
    getCBSETButton(stack::stack2).SetOutlineBrush(getCBSETButton_outlinecolor(m_bCBSET_Pressed_STK2));
    getCBSETButton(stack::stack2).SetTextBrush(getCBSETButton_textcolor(m_bCBSET_Pressed_STK2));
    getCBSETButton(stack::stack2).Redraw();
}

void dashboard_container::onClikedCBSETButton_STK1()
{
    if (BMS::cmdmanager::isConnected() == false || getMainFrame()->getRepetitionState())
        return;

    if (m_bEnable_STK1 == false)
        return;

    m_bCBSET_Pressed_STK1 = !m_bCBSET_Pressed_STK1;

    getCBSETButton(stack::stack1).SetFillBrush(getCBSETButton_fillcolor(m_bCBSET_Pressed_STK1));
    getCBSETButton(stack::stack1).SetOutlineBrush(getCBSETButton_outlinecolor(m_bCBSET_Pressed_STK1));
    getCBSETButton(stack::stack1).SetTextBrush(getCBSETButton_textcolor(m_bCBSET_Pressed_STK1));
    getCBSETButton(stack::stack1).Redraw();

    if (m_bCBSET_Pressed_STK1)
        getCBGrid(stack::stack1).GetGridCtrl()->EnableWindow(false);
    else
        getCBGrid(stack::stack1).GetGridCtrl()->EnableWindow(true);
}

void dashboard_container::onClikedCBSETButton_STK2()
{
    if (BMS::cmdmanager::isConnected() == false || getMainFrame()->getRepetitionState())
        return;

    if (m_bEnable_STK2 == false)
        return;

    m_bCBSET_Pressed_STK2 = !m_bCBSET_Pressed_STK2;

    getCBSETButton(stack::stack2).SetFillBrush(getCBSETButton_fillcolor(m_bCBSET_Pressed_STK2));
    getCBSETButton(stack::stack2).SetOutlineBrush(getCBSETButton_outlinecolor(m_bCBSET_Pressed_STK2));
    getCBSETButton(stack::stack2).SetTextBrush(getCBSETButton_textcolor(m_bCBSET_Pressed_STK2));
    getCBSETButton(stack::stack2).Redraw();

    if (m_bCBSET_Pressed_STK2)
        getCBGrid(stack::stack2).GetGridCtrl()->EnableWindow(false);
    else
        getCBGrid(stack::stack2).GetGridCtrl()->EnableWindow(true);
}

void dashboard_container::drawChartLabelBorder(bool bDraw)
{
    drawVCChartLabelBorder(stack::stack1, bDraw);
    drawVCChartLabelBorder(stack::stack2, bDraw);
    drawSensorTempChartLabelBorder(stack::stack1, bDraw);
    drawSensorTempChartLabelBorder(stack::stack2, bDraw);
    drawThermistorTempChartLabelBorder(stack::stack1, bDraw);
    drawThermistorTempChartLabelBorder(stack::stack2, bDraw);
}

void dashboard_container::init_gui()
{
    try
    {
        init_stack(stack::stack1);
        init_stack(stack::stack2);

        init_sum_chart();

        Redraw();
    } catch (const std::exception& e) {
        DebugPrint("ERROR : dashboard init gui fail [{}]", e.what());
    }
}

void dashboard_container::init_layout()
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

    pLayout->AddAnchor(GetByID(CB_STATIC_STK1), CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 50), CPoint(50, 50));
    pLayout->AddAnchor(GetByID(CB_TITLE_STK1), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(30, 50));
    pLayout->AddAnchor(GetByID(TEMP_STATIC_STK1), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 50), CPoint(30, 50));
    pLayout->AddAnchor(GetByID(TEMP_TITLE_STK1), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeHorz, CPoint(15, 50));
    pLayout->AddAnchor(GetByID(VBAT_STATIC_STK1), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(80, 50), CPoint(20, 50));
    pLayout->AddAnchor(GetByID(VBAT_TITLE_STK1), CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeHorz, CPoint(40, 50));
    pLayout->AddAnchor(&m_stk1_vc_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(27, 50));
    pLayout->AddAnchor(&m_stk1_temp_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(63, 50));
    pLayout->AddAnchor(&m_stk1_temp1_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 57));
    pLayout->AddAnchor(&m_stk1_temp2_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 75));
    pLayout->AddAnchor(&m_stk1_cb_grid, CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 50), CPoint(15, 50));
    pLayout->AddAnchor(&m_stk1_vc_chart, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(15, 50), CPoint(25, 50));
    pLayout->AddAnchor(&m_stk1_stat_grid, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(40, 50), CPoint(10, 50));
    pLayout->AddAnchor(&m_stk1_temp1_chart, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 50), CPoint(25, 15));
    pLayout->AddAnchor(&m_stk1_temp2_chart, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 65), CPoint(25, 35));
    pLayout->AddAnchor(&m_stk1_temp_grid, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(75, 65), CPoint(5, 40));
    pLayout->AddAnchor(&m_stk1_vbat_gauge, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(80, 50), CPoint(20, 75));
    pLayout->AddAnchor(&m_stk1_cbset_button, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(15, 100));

    pLayout->AddAnchor(GetByID(CB_STATIC_STK2), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 100), CPoint(50, 50));
    pLayout->AddAnchor(GetByID(CB_TITLE_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(30, 100));
    pLayout->AddAnchor(GetByID(TEMP_STATIC_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 100), CPoint(30, 50));
    pLayout->AddAnchor(GetByID(TEMP_TITLE_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeHorz, CPoint(15, 100));
    pLayout->AddAnchor(GetByID(VBAT_STATIC_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(80, 100), CPoint(20, 50));
    pLayout->AddAnchor(GetByID(VBAT_TITLE_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeHorz, CPoint(40, 100));
    pLayout->AddAnchor(&m_stk2_vc_title, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(27, 0));
    pLayout->AddAnchor(&m_stk2_temp_title, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(63, 0));
    pLayout->AddAnchor(&m_stk2_temp1_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 5));
    pLayout->AddAnchor(&m_stk2_temp2_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 25));
    pLayout->AddAnchor(&m_stk2_cb_grid, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(100, 100), CPoint(15, 50));
    pLayout->AddAnchor(&m_stk2_vc_chart, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(15, 100), CPoint(25, 50));
    pLayout->AddAnchor(&m_stk2_stat_grid, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(40, 100), CPoint(10, 50));
    pLayout->AddAnchor(&m_stk2_temp1_chart, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 100), CPoint(25, 15));
    pLayout->AddAnchor(&m_stk2_temp2_chart, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 15), CPoint(25, 35));
    pLayout->AddAnchor(&m_stk2_temp_grid, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(75, 15), CPoint(5, 40));
    pLayout->AddAnchor(&m_stk2_vbat_gauge, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(80, 100), CPoint(20, 75));
    pLayout->AddAnchor(&m_stk2_cbset_button, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(15, 50));

    pLayout->AddAnchor(&m_sumchart_title, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(100, 100));
    pLayout->AddAnchor(&m_sum_chart, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeVert, CPoint(100, 100), CPoint(100, 100));
}

void dashboard_container::init_stack(const stack& nStack)
{
    enableStack(nStack, true);

    init_cb_grid(nStack);
    init_cbset_button(nStack);
    init_vc_chart(nStack);
    init_stat_grid(nStack);
    init_sensor_temp_chart(nStack);
    init_thermistor_temp_chart(nStack);
    init_temp_grid(nStack);
    init_vbat_gauge(nStack);
}

void dashboard_container::init_cb_grid(const stack& nStack)
{
    getCBGrid(nStack).SetAutoDestroy(TRUE);

    autoSizeGridCtrl* cb_grid = dynamic_cast<autoSizeGridCtrl*>(getCBGrid(nStack).GetGridCtrl());
    cb_grid->ShowVertScrollBar(FALSE);
    cb_grid->ShowHorzScrollBar(FALSE);
    cb_grid->RemoveAll();
    cb_grid->DeleteAllColumns();

    // diable header click
    cb_grid->EnableHeader(TRUE, 0);
    cb_grid->EnableHeaderSelectAllMarker(FALSE);
    cb_grid->EnableRowHeader(TRUE);
    cb_grid->EnableLineNumbers();
    cb_grid->setReverseLineNumber(true);

    cb_grid->SetWholeRowSel(TRUE);
    cb_grid->SetSingleSel(TRUE);
    cb_grid->EnableColumnAutoSize(TRUE);

    CRect gird_rect;
    cb_grid->GetClientRect(&gird_rect);

    for (const auto& [key, value] : grid_info::CB_columns)
    {
        switch (key)
        {
        case grid_info::CB::SET:
            cb_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.3));
            break;
        case grid_info::CB::TIME_LEFT:
            cb_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.6));
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < cb_max_row; ++i)
    {
        CBCGPGridRow* pRow = cb_grid->CreateRow(cb_grid->GetColumnCount());
        cb_grid->AddRow(pRow, FALSE);

        pRow->ReplaceItem(grid_info::CB::SET, new cbsetGridItem());
        pRow->GetItem(grid_info::CB::TIME_LEFT)->SetReadOnly(TRUE);
        pRow->GetItem(grid_info::CB::TIME_LEFT)->Enable(FALSE);
    }
    cb_grid->AdjustLayout();

    if (BMS::cmdmanager::isConnected() == false || getMainFrame()->getRepetitionState())
        enableEditCBGrid(nStack, false);
    else
        enableEditCBGrid(nStack, true);
}

void dashboard_container::init_cbset_button(const stack& nStack)
{
    if (nStack == stack::stack1)
        m_bCBSET_Pressed_STK1 = false;
    else if (nStack == stack::stack2)
        m_bCBSET_Pressed_STK2 = false;

    getCBSETButton(nStack).SetFillBrush(getCBSETButton_fillcolor(false));
    getCBSETButton(nStack).SetOutlineBrush(getCBSETButton_outlinecolor(false));
    getCBSETButton(nStack).SetTextBrush(getCBSETButton_textcolor(false));
    getCBSETButton(nStack).Redraw();
}

void dashboard_container::init_vc_chart(const stack& nStack)
{
    getVCChart(nStack).SetAutoDestroy(TRUE);

    getVCChart(nStack).CleanUp();

    getVCChart(nStack).ShowAxis(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
    getVCChart(nStack).ShowAxisGridLines(BCGP_CHART_Y_PRIMARY_AXIS, 0, 0);
    getVCChart(nStack).SetLegendPosition(BCGPChartLayout::LP_NONE);
    getVCChart(nStack).ShowDataLabels(TRUE, FALSE, FALSE);
    getVCChart(nStack).SetSeriesShadow(TRUE);

    CBCGPChartAxis* pY_axis = getVCChart(nStack).GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pY_axis->SetFixedDisplayRange(vc_chart_container::vc_chart_min_y, vc_chart_container::vc_chart_max_y);
    pY_axis->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
    pY_axis->m_axisLabelsFormat = BCGPChartFormatLabel();

    CBCGPChartAxis* pX_axis = getVCChart(nStack).GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pX_axis->m_crossType = CBCGPChartAxis::CT_MINIMUM_AXIS_VALUE;

    CBCGPChartSeries* pVoltage_series = getVCChart(nStack).CreateSeries(_T("Cell Voltage"));
    pVoltage_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE);
    pVoltage_series->SetTreatNulls(CBCGPChartSeries::TN_NO_PAINT);

    for (int i = 0; i < cb_max_row; ++i)
    {
        pVoltage_series->AddEmptyDataPoint();

        CString vc_num = std::format(L"VC{}", i + 1).c_str();
        pVoltage_series->SetDataLabelDataFormat(vc_num + L"=%.3f V", i);
    }
}

void dashboard_container::init_stat_grid(const stack& nStack)
{
    getStatGrid(nStack).SetAutoDestroy(TRUE);

    autoSizeGridCtrl* stat_grid = dynamic_cast<autoSizeGridCtrl*>(getStatGrid(nStack).GetGridCtrl());
    stat_grid->ShowVertScrollBar(FALSE);
    stat_grid->ShowHorzScrollBar(FALSE);
    stat_grid->RemoveAll();
    stat_grid->DeleteAllColumns();

    stat_grid->EnableHeader(TRUE, 0);
    stat_grid->EnableHeaderSelectAllMarker(FALSE);
    stat_grid->setDrawRowHeader(false);

    stat_grid->SetReadOnly(TRUE);
    stat_grid->EnableColumnAutoSize(TRUE);

    CRect gird_rect;
    stat_grid->GetClientRect(&gird_rect);

    for (const auto& [key, value] : grid_info::VC_columns)
    {
        switch (key)
        {
        case grid_info::STAT_VC::VCB_DONE:
            stat_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.6));
            break;
        case grid_info::STAT_VC::UV:
            stat_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.3));
            break;
        case grid_info::STAT_VC::OV:
            stat_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.3));
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < cb_max_row; ++i)
    {
        CBCGPGridRow* pRow = stat_grid->CreateRow(stat_grid->GetColumnCount());
        pRow->ReplaceItem(grid_info::STAT_VC::VCB_DONE, new colorIndicaterGridItem(VCB_thr_color.GetColor()));
        pRow->ReplaceItem(grid_info::STAT_VC::UV, new colorIndicaterGridItem(UV_thr_color.GetColor()));
        pRow->ReplaceItem(grid_info::STAT_VC::OV, new colorIndicaterGridItem(OV_thr_color.GetColor()));
        stat_grid->AddRow(pRow, FALSE);
    }

    stat_grid->EnableWindow(FALSE);
    stat_grid->AdjustLayout();
}

void dashboard_container::init_sensor_temp_chart(const stack& nStack)
{
    getSensorTempChart(nStack).CleanUp();

    getSensorTempChart(nStack).SetAutoDestroy(TRUE);

    // set title
    getSensorTempTitle(nStack).SetText(_T("On-Die\nTemp.Sensor"));

    getSensorTempChart(nStack).ShowAxis(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
    getSensorTempChart(nStack).ShowAxis(BCGP_CHART_Y_PRIMARY_AXIS, FALSE);
    getSensorTempChart(nStack).SetLegendPosition(BCGPChartLayout::LP_NONE);
    getSensorTempChart(nStack).ShowDataLabels(TRUE, FALSE, FALSE);
    getSensorTempChart(nStack).SetSeriesShadow(TRUE);

    CBCGPChartAxis* pY_axis = getSensorTempChart(nStack).GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pY_axis->SetFixedDisplayRange(temp_chart_container::temp_chart_min_y, temp_chart_container::temp_chart_max_y);

    CBCGPChartAxis* pX_axis = getSensorTempChart(nStack).GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pX_axis->SetAxisOffsets(3, 0);
    pX_axis->m_crossType = CBCGPChartAxis::CT_MINIMUM_AXIS_VALUE;

    CBCGPChartSeries* pTemp_series = getSensorTempChart(nStack).CreateSeries(_T("On-Die Temp.Sensor"));
    pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE);
    pTemp_series->SetTreatNulls(CBCGPChartSeries::TN_NO_PAINT);

    //auto destroy
    barLabelPosChangeImpl* bar = new barLabelPosChangeImpl(&getSensorTempChart(nStack));
    pTemp_series->SetChartImpl(bar);

    for (int i = 0; i < sensor_temp_max_row; ++i)
    {
        pTemp_series->AddEmptyDataPoint();

        CString temp_num = std::format(L"DT{}", i + 1).c_str();
        pTemp_series->SetDataLabelDataFormat(temp_num + L"=%.1f °C", i);
    }
}

void dashboard_container::init_thermistor_temp_chart(const stack& nStack)
{
    getThermistorChart(nStack).CleanUp();
    // set title
    getThermistorTitle(nStack).SetText(_T("GPIO Thermistor"));

    getThermistorChart(nStack).SetAutoDestroy(TRUE);

    getThermistorChart(nStack).ShowAxis(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
    getThermistorChart(nStack).ShowAxisGridLines(BCGP_CHART_Y_PRIMARY_AXIS, 0, 0);
    getThermistorChart(nStack).SetLegendPosition(BCGPChartLayout::LP_NONE);
    getThermistorChart(nStack).ShowDataLabels(TRUE, FALSE, FALSE);
    getThermistorChart(nStack).SetSeriesShadow(TRUE);

    CBCGPChartAxis* pY_axis = getThermistorChart(nStack).GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pY_axis->SetFixedDisplayRange(temp_chart_container::temp_chart_min_y, temp_chart_container::temp_chart_max_y);
    pY_axis->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
    pY_axis->m_axisLabelsFormat = BCGPChartFormatLabel();

    CBCGPChartAxis* pX_axis = getThermistorChart(nStack).GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pX_axis->m_crossType = CBCGPChartAxis::CT_MINIMUM_AXIS_VALUE;

    CBCGPChartSeries* pTemp_series = getThermistorChart(nStack).CreateSeries(_T("GPIO Thermistor"));
    pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE);
    pTemp_series->SetTreatNulls(CBCGPChartSeries::TN_NO_PAINT);

    //auto destroy
    barLabelPosChangeImpl* bar = new barLabelPosChangeImpl(&getThermistorChart(nStack));
    pTemp_series->SetChartImpl(bar);

    for (int i = 0; i < thermistor_temp_max_row; ++i)
    {
        pTemp_series->AddEmptyDataPoint();

        CString temp_num = std::format(L"NTC{}", i + 1).c_str();
        pTemp_series->SetDataLabelDataFormat(temp_num + L"=%.1f °C", i);
    }
}

void dashboard_container::init_temp_grid(const stack& nStack)
{
    getTempGrid(nStack).SetAutoDestroy(TRUE);

    autoSizeGridCtrl* temp_grid = dynamic_cast<autoSizeGridCtrl*>(getTempGrid(nStack).GetGridCtrl());
    temp_grid->ShowVertScrollBar(FALSE);
    temp_grid->ShowHorzScrollBar(FALSE);
    temp_grid->RemoveAll();
    temp_grid->DeleteAllColumns();

    temp_grid->SetReadOnly(TRUE);
    temp_grid->EnableColumnAutoSize(TRUE);

    temp_grid->EnableHeaderSelectAllMarker(FALSE);
    temp_grid->EnableRowHeader(TRUE);
    temp_grid->EnableLineNumbers();
    temp_grid->setReverseLineNumber(true);

    CRect gird_rect;
    temp_grid->GetClientRect(&gird_rect);

    for (const auto& [key, value] : grid_info::TEMP_columns)
    {
        switch (key)
        {
        case grid_info::STAT_TEMP::UT:
            temp_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.3));
            break;
        case grid_info::STAT_TEMP::OT:
            temp_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.3));
            break;
        case grid_info::STAT_TEMP::OTCB:
            temp_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.5));
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < thermistor_temp_max_row; ++i)
    {
        CBCGPGridRow* pRow = temp_grid->CreateRow(temp_grid->GetColumnCount());
        pRow->ReplaceItem(grid_info::STAT_TEMP::UT, new colorIndicaterGridItem(UT_thr_color.GetColor()));
        pRow->ReplaceItem(grid_info::STAT_TEMP::OT, new colorIndicaterGridItem(OT_thr_color.GetColor()));
        pRow->ReplaceItem(grid_info::STAT_TEMP::OTCB, new colorIndicaterGridItem(OTCB_thr_color.GetColor()));
        temp_grid->AddRow(pRow, FALSE);
    }

    temp_grid->EnableWindow(FALSE);
    temp_grid->AdjustLayout();
}

void dashboard_container::init_vbat_gauge(const stack& nStack)
{
    getVBATGauge(nStack).SetAutoDestroy(TRUE);

    getVBATGauge(nStack).SetValue(vbat_min);

    CBCGPTextGaugeImpl* pGauge_text = dynamic_cast<CBCGPTextGaugeImpl*>(getVBATGauge(nStack).GetSubGaugeByID(getVBATGauge(nStack).GetID()));
    pGauge_text->SetText(_T("00.00V"));
    pGauge_text->SetFillBrush(CBCGPBrush(CBCGPColor::BCGP_COLOR::Black));
}

void dashboard_container::init_sum_chart()
{
    static const CBCGPBrush stack1_color = CBCGPBrush(CBCGPColor::MediumAquamarine);
    static const CBCGPBrush stack2_color = CBCGPBrush(CBCGPColor::Gold);

    m_sum_chart.SetAutoDestroy(TRUE);
    m_sum_chart.CleanUp();

    // set title
    m_sumchart_title.SetText(_T("Stacked \nBattery \nVoltage"));

    m_sum_chart.SetLegendPosition(BCGPChartLayout::LP_NONE);
    m_sum_chart.ShowDataLabels(TRUE, TRUE, FALSE);
    m_sum_chart.SetSeriesShadow(TRUE);
    m_sum_chart.ShowAxisGridLines(BCGP_CHART_Y_PRIMARY_AXIS, FALSE);
    m_sum_chart.ShowAxisGridLines(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
    m_sum_chart.GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS)->SetFixedDisplayRange(0, 200, 20);

    CBCGPChartSeries* pStack1_series = m_sum_chart.CreateSeries(_T("stack 1"));
    pStack1_series->SetDataLabelDataFormat(L"%.3f V");
    pStack1_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_DEFAULT_POS);
    pStack1_series->SetSeriesFill(stack1_color);
    pStack1_series->SetSeriesLineColor(stack1_color);

    CBCGPChartSeries* pStack2_series = m_sum_chart.CreateSeries(_T("stack 2"));
    pStack2_series->SetDataLabelDataFormat(L"%.3f V");
    pStack2_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_DEFAULT_POS);
    pStack2_series->SetSeriesFill(stack2_color);
    pStack2_series->SetSeriesLineColor(stack2_color);

    CBCGPChartAxisY* pAxisY = dynamic_cast<CBCGPChartAxisY*>(m_sum_chart.GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS));
    pAxisY->m_axisLabelsFormat = BCGPChartFormatLabel();

    CBCGPChartAxisX* pAxisX = dynamic_cast<CBCGPChartAxisX*>(m_sum_chart.GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS));
    pAxisX->m_arCategories.Add(_T("VBAT_SUM1\n(VBAT1+VBAT2)"));
    pAxisX->m_arCategories.Add(_T("VBAT_SUM2\n[ Σ{VC1~18,Stack1}\n+ Σ{VC1~18,Stack2} ]"));
    pAxisX->m_axisLabelsFormat = BCGPChartFormatLabel();

    for (int i = 0; i < 2; ++i)
        pStack1_series->AddEmptyDataPoint();

    for (int i = 0; i < 2; ++i)
        pStack2_series->AddEmptyDataPoint();

    m_sum_chart.Redraw();
}

MEAS_info::VC dashboard_container::getMEAS_VC() const noexcept
{
    return m_vc;
}

MEAS_info::TEMP dashboard_container::getMEAS_TEMP() const noexcept
{
    return m_temp;
}

MEAS_info::VBAT dashboard_container::getMEAS_VBAT() const noexcept
{
    return m_vbat;
}

MEAS_info::STAT dashboard_container::getMEAS_STAT() const noexcept
{
    return m_stat;
}

CB_info::CBTIME dashboard_container::getCB_TIME() const noexcept
{
    return m_leftTime;
}

std::vector<std::wstring> dashboard_container::getCBSET(const stack& nStack)
{
    std::vector<std::wstring> cbset;
    cbset.reserve(cb_max_row);

    CBCGPGridCtrl* pGrid = getCBGrid(nStack).GetGridCtrl();
    for (int32_t i = pGrid->GetRowCount() - 1; i >= 0; --i)
    {
        CString value = pGrid->GetRow(i)->GetItem(grid_info::CB::SET)->GetValue();

        if (value.IsEmpty())
            cbset.push_back(L"0");
        else
            cbset.push_back(value.GetBuffer());
    }

    return cbset;
}

void dashboard_container::setMEAS_VC(const MEAS_info::VC& vc)
{
    m_vc = vc;
}

void dashboard_container::setMEAS_TEMP(const MEAS_info::TEMP& temp)
{
    m_temp = temp;
}

void dashboard_container::setMEAS_VBAT(const MEAS_info::VBAT& vbat)
{
    m_vbat = vbat;
}

void dashboard_container::setMEAS_STAT(const MEAS_info::STAT& stat)
{
    m_stat = stat;
}

void dashboard_container::setCB_TIME(const CB_info::CBTIME& cbtime)
{
    m_leftTime = cbtime;
}

void dashboard_container::drawMEAS_VC(const MEAS_info::VC& vc)
{
    if (drawVCChart(stack::stack1, vc.STK1) == false)
        throw std::exception("Stack 1 VC Chart draw error");
    if (drawVCChart(stack::stack2, vc.STK2) == false)
        throw std::exception("Stack 1 VC Chart draw error");
}

void dashboard_container::drawMEAS_TEMP(const MEAS_info::TEMP& temp)
{
    if (drawSensorTempChart(stack::stack1, temp.STK1_sensor) == false)
        throw std::exception("Stack 1 Sensor Temp Chart draw error");
    if (drawThermistorTempChart(stack::stack1, temp.STK1_thermistor) == false)
        throw std::exception("Stack 1 Thermistor Temp Chart draw error");
    if (drawSensorTempChart(stack::stack2, temp.STK2_sensor) == false)
        throw std::exception("Stack 2 Sensor Temp Chart draw error");
    if (drawThermistorTempChart(stack::stack2, temp.STK2_thermistor) == false)
        throw std::exception("Stack 2 Thermistor Temp Chart draw error");
}

void dashboard_container::drawMEAS_VBAT(const MEAS_info::VBAT& vbat)
{
    if (drawVbatGauge(stack::stack1, vbat.STK1) == false)
        throw std::exception("Stack 1 Vbat Gauge draw error");
    if (drawVbatGauge(stack::stack2, vbat.STK2) == false)
        throw std::exception("Stack 2 Vbat Gauge draw error");
}

void dashboard_container::drawMEAS_STAT(const MEAS_info::STAT& stat)
{
    if (drawStatOV(stack::stack1, stat.OV_STK1) == false)
        throw std::exception("Stack 1 Stat OV draw error");
    if (drawStatOV(stack::stack2, stat.OV_STK2) == false)
        throw std::exception("Stack 2 Stat OV draw error");
    if (drawStatUV(stack::stack1, stat.UV_STK1) == false)
        throw std::exception("Stack 1 Stat UV draw error");
    if (drawStatUV(stack::stack2, stat.UV_STK2) == false)
        throw std::exception("Stack 2 Stat UV draw error");
    if (drawStatVCB(stack::stack1, stat.VCB_STK1) == false)
        throw std::exception("Stack 1 Stat VCB draw error");
    if (drawStatVCB(stack::stack2, stat.VCB_STK2) == false)
        throw std::exception("Stack 2 Stat VCB draw error");
    if (drawStatUT(stack::stack1, stat.UT_STK1) == false)
        throw std::exception("Stack 1 Stat UT draw error");
    if (drawStatUT(stack::stack2, stat.UT_STK2) == false)
        throw std::exception("Stack 2 Stat UT draw error");
    if (drawStatOT(stack::stack1, stat.OT_STK1) == false)
        throw std::exception("Stack 1 Stat OT draw error");
    if (drawStatOT(stack::stack2, stat.OT_STK2) == false)
        throw std::exception("Stack 2 Stat OT draw error");
    if (drawStatOTCB(stack::stack1, stat.OTCB_STK1) == false)
        throw std::exception("Stack 1 Stat OTCB draw error");
    if (drawStatOTCB(stack::stack2, stat.OTCB_STK2) == false)
        throw std::exception("Stack 2 Stat OTCB draw error");
}

void dashboard_container::drawCB_TIME(const CB_info::CBTIME& cbtime)
{
    if (drawCBLeft(stack::stack1, cbtime.STK1) == false)
        throw std::exception("Stack 1 CB left time error");
    if (drawCBLeft(stack::stack2, cbtime.STK2) == false)
        throw std::exception("Stack 2 CB left time draw error");
}

bool dashboard_container::drawVCChart(const stack& nStack, const std::vector<double>& VC)
{
    if (isEnableStack(nStack) == false)
        return true;

    CBCGPChartSeries* pVoltage_series = getVCChart(nStack).GetSeries(0);
    if (VC.empty())
        return true;
    if (VC.size() != pVoltage_series->GetDataPointCount())
        return false;

    int32_t index = 0;
    for (const double cell_voltage : VC)
    {
        if (pVoltage_series->SetDataPointValue(index, cell_voltage) == false)
            return false;

        pVoltage_series->SetDataLabelDrawBorder(getMainFrame()->isDrawLabelBorder(), index);

        if (cell_voltage < vc_chart_label_thr)
            pVoltage_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_OUTSIDE_END, index);
        else
            pVoltage_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE, index);

        CBCGPBrush color = VC_default;
        if (getMainFrame()->getConfDockbar().getCONF().OV_THR < cell_voltage)
            color = OV_thr_color;
        else if (getMainFrame()->getConfDockbar().getCONF().UV_THR > cell_voltage)
            color = UV_thr_color;
        else if (getMainFrame()->getConfDockbar().getCONF().VCB_THR > cell_voltage)
            color = VCB_thr_color;

        pVoltage_series->SetSeriesFill(color, index);
        pVoltage_series->SetSeriesLineColor(color, index);

        ++index;
    }

    getVCChart(nStack).SetDirty();
    getVCChart(nStack).Redraw();

    return true;
}

bool dashboard_container::drawSensorTempChart(const stack& nStack, const std::vector<double>& TEMP)
{
    if (isEnableStack(nStack) == false)
        return true;

    CBCGPChartSeries* pTemp_series = getSensorTempChart(nStack).GetSeries(0);
    if (TEMP.empty())
        return true;
    if (TEMP.size() != pTemp_series->GetDataPointCount())
        return false;

    int32_t index = 0;
    for (const double temperature : TEMP)
    {
        if (pTemp_series->SetDataPointValue(index, temperature) == false)
            return false;

        pTemp_series->SetDataLabelDrawBorder(getMainFrame()->isDrawLabelBorder(), index);

        if (temperature < temp_chart_label_thr)
            pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_OUTSIDE_END, index);
        else
            pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE, index);

        pTemp_series->SetSeriesFill(SensorTemp_default, index);
        pTemp_series->SetSeriesLineColor(SensorTemp_default, index);

        ++index;
    }

    getSensorTempChart(nStack).SetDirty();
    getSensorTempChart(nStack).Redraw();

    return true;
}

bool dashboard_container::drawThermistorTempChart(const stack& nStack, const std::vector<double>& TEMP)
{
    if (isEnableStack(nStack) == false)
        return true;

    CBCGPChartSeries* pTemp_series = getThermistorChart(nStack).GetSeries(0);
    if (TEMP.empty())
        return true;
    if (TEMP.size() != pTemp_series->GetDataPointCount())
        return false;

    int32_t index = 0;
    for (const double temperature : TEMP)
    {
        if (pTemp_series->SetDataPointValue(index, temperature) == false)
            return false;

        pTemp_series->SetDataLabelDrawBorder(getMainFrame()->isDrawLabelBorder(), index);

        if (temperature < temp_chart_label_thr)
            pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_OUTSIDE_END, index);
        else
            pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE, index);

        CBCGPBrush color = SensorTemp_default;
        if (getMainFrame()->getConfDockbar().getCONF().OT_THR < temperature)
            color = OT_thr_color;
        else if (getMainFrame()->getConfDockbar().getCONF().OTCB_THR < temperature)
            color = OTCB_thr_color;
        else if (getMainFrame()->getConfDockbar().getCONF().UT_THR > temperature)
            color = UT_thr_color;

        pTemp_series->SetSeriesFill(color, index);
        pTemp_series->SetSeriesLineColor(color, index);

        ++index;
    }

    getThermistorChart(nStack).SetDirty();
    getThermistorChart(nStack).Redraw();

    return true;
}

bool dashboard_container::drawVCChart_THR(const stack& nStack, const double& THR, const CBCGPBrush& thr_color)
{
    if (isEnableStack(nStack) == false)
        return true;

    CBCGPStrokeStyle type;
    type.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH);

    getVCChart(nStack).AddChartLineObject(THR, TRUE, thr_color, 2, &type)->SetShadow();
    getVCChart(nStack).AddChartAxisMarkObject(THR, std::format(L"{:.1f} V", THR).c_str(), TRUE, TRUE, CBCGPBrush(CBCGPColor::White), thr_color);

    getVCChart(nStack).SetDirty();
    getVCChart(nStack).Redraw();

    return true;
}

bool dashboard_container::drawThermistorTempChart_THR(const stack& nStack, const double& TEMP_THR, const CBCGPBrush& thr_color)
{
    if (isEnableStack(nStack) == false)
        return true;

    CBCGPStrokeStyle type;
    type.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH);

    getThermistorChart(nStack).AddChartLineObject(TEMP_THR, TRUE, thr_color, 2, &type)->SetShadow();
    getThermistorChart(nStack).AddChartAxisMarkObject(TEMP_THR, std::format(L"{:.0f} °C", TEMP_THR).c_str(), TRUE, TRUE, CBCGPBrush(CBCGPColor::White), thr_color);

    getThermistorChart(nStack).SetDirty();
    getThermistorChart(nStack).Redraw();

    return true;
}

bool dashboard_container::drawVbatGauge(const stack& nStack, const double vbat)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (vbat < vbat_min)
        getVBATGauge(nStack).SetValue(vbat_min);
    else if (vbat > vbat_max)
        getVBATGauge(nStack).SetValue(vbat_max);
    else
        getVBATGauge(nStack).SetValue(vbat);

    CBCGPTextGaugeImpl* pGauge_static = dynamic_cast<CBCGPTextGaugeImpl*>(getVBATGauge(nStack).GetSubGaugeByID(getVBATGauge(nStack).GetID()));
    pGauge_static->SetText(std::format(L"{:.2f}V", vbat).c_str());

    return true;
}

bool dashboard_container::drawCBSET(const stack& nStack, const std::vector<std::wstring>& cbset)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (cbset.empty())
        return true;

    CBCGPGridCtrl* cb_grid = getCBGrid(nStack).GetGridCtrl();

    if (cb_grid->GetRowCount() != cbset.size())
        return false;

    uint32_t index = cb_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const std::wstring& cb : cbset)
        cb_grid->GetRow(--index)->GetItem(grid_info::CB::SET)->SetValue(cb.c_str());

    return true;
}

bool dashboard_container::drawCBLeft(const stack& nStack, const std::vector<std::wstring>& CBTIME_LEFT)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (CBTIME_LEFT.empty())
        return true;

    CBCGPGridCtrl* cb_grid = getCBGrid(nStack).GetGridCtrl();

    if (cb_grid->GetRowCount() != CBTIME_LEFT.size())
        return false;

    uint32_t index = cb_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const std::wstring& left_time : CBTIME_LEFT)
        cb_grid->GetRow(--index)->GetItem(grid_info::CB::TIME_LEFT)->SetValue(left_time.c_str());

    return true;
}

bool dashboard_container::drawSumChart(const double vbat1, const double vbat2, const double sumVC1, const double sumVC2)
{
    static constexpr double text_margin = 30;
    static const CBCGPBrush text_color = CBCGPBrush(CBCGPColor::Red);
    static const CBCGPBrush text_fill = CBCGPBrush(CBCGPColor::White);
    static const CBCGPBrush text_border = CBCGPBrush(CBCGPColor::White);

    if (isEnableStack(stack::stack2))
    {
        CBCGPChartSeries* pStack2_series = m_sum_chart.GetSeries(1);
        pStack2_series->SetDataPointValue(0, vbat2);
        pStack2_series->SetDataPointValue(1, sumVC2);
    }

    if (isEnableStack(stack::stack1))
    {
        CBCGPChartSeries* pStack1_series = m_sum_chart.GetSeries(0);
        pStack1_series->SetDataPointValue(0, vbat1);
        pStack1_series->SetDataPointValue(1, sumVC1);
    }

    m_sum_chart.RemoveAllChartObjects();

    // auto destroy;
    CBCGPChartTextObject* pVbatObject = new CBCGPChartTextObject (&m_sum_chart,
        std::format(L"{:.3f} V", vbat1 + vbat2).c_str(), 1, vbat1 + vbat2,
        text_color, text_fill, text_border,
        text_margin, 0, false);
    m_sum_chart.AddChartObject(pVbatObject);

    // auto destroy;
    CBCGPChartTextObject* pSumVCObject = new CBCGPChartTextObject(&m_sum_chart,
        std::format(L"{:.3f} V", sumVC1 + sumVC2).c_str(), 2, sumVC1 + sumVC2,
        text_color, text_fill, text_border,
        text_margin, 0, false);
    m_sum_chart.AddChartObject(pSumVCObject);

    m_sum_chart.SetDirty();
    m_sum_chart.Redraw();

    return true;
}

bool dashboard_container::drawStatOV(const stack& nStack, const std::vector<bool>& stat_ov)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (stat_ov.empty())
        return true;

    CBCGPGridCtrl* stat_grid = getStatGrid(nStack).GetGridCtrl();

    if (stat_grid->GetRowCount() != stat_ov.size())
        return false;

    uint32_t index = stat_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const bool ov : stat_ov)
        dynamic_cast<colorIndicaterGridItem*>(stat_grid->GetRow(--index)->GetItem(grid_info::STAT_VC::OV))->enableIndicater(ov);

    stat_grid->Invalidate();
    return true;
}

bool dashboard_container::drawStatUV(const stack& nStack, const std::vector<bool>& stat_uv)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (stat_uv.empty())
        return true;

    CBCGPGridCtrl* stat_grid = getStatGrid(nStack).GetGridCtrl();

    if (stat_grid->GetRowCount() != stat_uv.size())
        return false;

    uint32_t index = stat_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const bool uv : stat_uv)
        dynamic_cast<colorIndicaterGridItem*>(stat_grid->GetRow(--index)->GetItem(grid_info::STAT_VC::UV))->enableIndicater(uv);

    stat_grid->Invalidate();
    return true;
}

bool dashboard_container::drawStatVCB(const stack& nStack, const std::vector<bool>& stat_vcb)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (stat_vcb.empty())
        return true;

    CBCGPGridCtrl* stat_grid = getStatGrid(nStack).GetGridCtrl();

    if (stat_grid->GetRowCount() != stat_vcb.size())
        return false;

    uint32_t index = stat_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const bool vcb : stat_vcb)
        dynamic_cast<colorIndicaterGridItem*>(stat_grid->GetRow(--index)->GetItem(grid_info::STAT_VC::VCB_DONE))->enableIndicater(vcb);

    stat_grid->Invalidate();
    return true;
}

bool dashboard_container::drawStatUT(const stack& nStack, const std::vector<bool>& stat_ut)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (stat_ut.empty())
        return true;

    CBCGPGridCtrl* temp_grid = getTempGrid(nStack).GetGridCtrl();

    if (temp_grid->GetRowCount() != stat_ut.size())
        return false;

    uint32_t index = temp_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const bool ut : stat_ut)
        dynamic_cast<colorIndicaterGridItem*>(temp_grid->GetRow(--index)->GetItem(grid_info::STAT_TEMP::UT))->enableIndicater(ut);

    temp_grid->Invalidate();
    return true;
}

bool dashboard_container::drawStatOT(const stack& nStack, const std::vector<bool>& stat_ot)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (stat_ot.empty())
        return true;

    CBCGPGridCtrl* temp_grid = getTempGrid(nStack).GetGridCtrl();

    if (temp_grid->GetRowCount() != stat_ot.size())
        return false;

    uint32_t index = temp_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const bool ot : stat_ot)
        dynamic_cast<colorIndicaterGridItem*>(temp_grid->GetRow(--index)->GetItem(grid_info::STAT_TEMP::OT))->enableIndicater(ot);

    temp_grid->Invalidate();
    return true;
}

bool dashboard_container::drawStatOTCB(const stack& nStack, const std::vector<bool>& stat_otcb)
{
    if (isEnableStack(nStack) == false)
        return true;

    if (stat_otcb.empty())
        return true;

    CBCGPGridCtrl* temp_grid = getTempGrid(nStack).GetGridCtrl();

    if (temp_grid->GetRowCount() != stat_otcb.size())
        return false;

    uint32_t index = temp_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const bool otcb : stat_otcb)
        dynamic_cast<colorIndicaterGridItem*>(temp_grid->GetRow(--index)->GetItem(grid_info::STAT_TEMP::OTCB))->enableIndicater(otcb);

    temp_grid->Invalidate();
    return true;
}

void dashboard_container::clearVCChart_THR(const stack& nStack)
{
    getVCChart(nStack).RemoveAllChartObjects();
    getVCChart(nStack).Invalidate();
}

void dashboard_container::clearThermistorTempChart_THR(const stack& nStack)
{
    getThermistorChart(nStack).RemoveAllChartObjects();
    getThermistorChart(nStack).Invalidate();
}

void dashboard_container::clear()
{
    setMEAS_VC(MEAS_info::VC());
    setMEAS_TEMP(MEAS_info::TEMP());
    setMEAS_VBAT(MEAS_info::VBAT());
    setMEAS_STAT(MEAS_info::STAT());
    setCB_TIME(CB_info::CBTIME());

    clearVCChart_THR(stack::stack1);
    clearVCChart_THR(stack::stack2);
    clearThermistorTempChart_THR(stack::stack1);
    clearThermistorTempChart_THR(stack::stack2);

    init_gui();
}

void dashboard_container::enableEditCBGrid(const stack& nStack, bool bEnable)
{
    getCBGrid(nStack).GetGridCtrl()->EnableWindow(bEnable);
}

void dashboard_container::enableStack(const stack& nStack, bool bEnable)
{
    if (nStack == stack::stack1)
        m_bEnable_STK1 = bEnable;
    else if (nStack == stack::stack2)
        m_bEnable_STK2 = bEnable;

    getCBGrid(nStack).GetGridCtrl()->EnableWindow(bEnable);
    getStatGrid(nStack).GetGridCtrl()->EnableWindow(bEnable);
    getTempGrid(nStack).GetGridCtrl()->EnableWindow(bEnable);
}

bool dashboard_container::isEnableStack(const stack& nStack)
{
    if (nStack == stack::stack1)
        return m_bEnable_STK1;
    else if (nStack == stack::stack2)
        return m_bEnable_STK2;

    throw std::exception("checking enable stack error");
}

void dashboard_container::drawVCChartLabelBorder(const stack& nStack, bool bDraw)
{
    CBCGPChartSeries* pVoltage_series = getVCChart(nStack).GetSeries(0);

    for (int32_t index = 0; index < pVoltage_series->GetDataPointCount(); ++index)
        pVoltage_series->SetDataLabelDrawBorder(bDraw, index);

    getVCChart(nStack).SetDirty();
    getVCChart(nStack).Redraw();
}

void dashboard_container::drawSensorTempChartLabelBorder(const stack& nStack, bool bDraw)
{
    CBCGPChartSeries* pTemp_series = getSensorTempChart(nStack).GetSeries(0);

    for (int32_t index = 0; index < pTemp_series->GetDataPointCount(); ++index)
        pTemp_series->SetDataLabelDrawBorder(bDraw, index);

    getSensorTempChart(nStack).SetDirty();
    getSensorTempChart(nStack).Redraw();
}

void dashboard_container::drawThermistorTempChartLabelBorder(const stack& nStack, bool bDraw)
{
    CBCGPChartSeries* pTemp_series = getThermistorChart(nStack).GetSeries(0);

    for (int32_t index = 0; index < pTemp_series->GetDataPointCount(); ++index)
        pTemp_series->SetDataLabelDrawBorder(bDraw, index);

    getThermistorChart(nStack).SetDirty();
    getThermistorChart(nStack).Redraw();
}

BOOL dashboard_container::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
    CBCGPGridItemID id;
    getCBGrid(stack::stack1).GetGridCtrl()->SetCurSel(id, SM_NONE);
    getCBGrid(stack::stack2).GetGridCtrl()->SetCurSel(id, SM_NONE);

    return dashboard_adapter::OnMouseDown(nButton, pt);
}

CBCGPGridVisualObject& dashboard_container::getCBGrid(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_cb_grid : m_stk2_cb_grid;
}

CBCGPChartVisualObject& dashboard_container::getVCChart(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_vc_chart : m_stk2_vc_chart;
}

CBCGPGridVisualObject& dashboard_container::getStatGrid(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_stat_grid : m_stk2_stat_grid;
}

CBCGPChartVisualObject& dashboard_container::getSensorTempChart(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_temp1_chart : m_stk2_temp1_chart;
}

CBCGPChartVisualObject& dashboard_container::getThermistorChart(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_temp2_chart : m_stk2_temp2_chart;
}

CBCGPGridVisualObject& dashboard_container::getTempGrid(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_temp_grid : m_stk2_temp_grid;
}

CBCGPCircularGaugeImpl& dashboard_container::getVBATGauge(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_vbat_gauge : m_stk2_vbat_gauge;
}

CBCGPTextGaugeImpl& dashboard_container::getSensorTempTitle(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_temp1_title : m_stk2_temp1_title;
}

CBCGPTextGaugeImpl& dashboard_container::getThermistorTitle(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_temp2_title : m_stk2_temp2_title;
}

CBCGPTextGaugeImpl & dashboard_container::getCBSETButton(const stack& nStack)
{
    return nStack == stack::stack1 ? m_stk1_cbset_button : m_stk2_cbset_button;
}

mainFrame* dashboard_container::getMainFrame() const noexcept
{
    return reinterpret_cast<mainFrame*>(AfxGetMainWnd());
}
