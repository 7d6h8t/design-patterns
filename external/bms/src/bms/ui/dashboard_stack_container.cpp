#include "pch.h"
#include "dashboard_stack_container.h"
#include "controls/autoSizeGridCtrl.h"
#include "controls/cbsetGridItem.h"
#include "cmdmanager.h"
#include "mainFrame.h"
#include <format>
#include "vc_chart_container.h"
#include "temp_chart_container.h"
#include "controls/colorIndicaterGridItem.h"
#include <utils.h>
#include "info/grid_info.h"
#include "controls/barLabelPosChangeImpl.h"

const float dashboard_stack_container::m_label_font_size = 22;

dashboard_stack_container::dashboard_stack_container(const view_mode::dashboard_view_option stack) noexcept
    : dashboard_view_stack_adapter(stack),
    m_stack(stack),
    m_stk_cb_grid(getGridObject((stack == view_mode::dashboard_view_option::Stack1) ? CB_GRID_STK1 : CB_GRID_STK2)),
    m_stk_vc_chart(getChartObject((stack == view_mode::dashboard_view_option::Stack1) ? VC_CHART_STK1 : VC_CHART_STK2)),
    m_stk_stat_grid(getGridObject((stack == view_mode::dashboard_view_option::Stack1) ? VC_STAT_GRID_STK1 : VC_STAT_GRID_STK2)),
    m_stk_sensor_temp_chart(getChartObject((stack == view_mode::dashboard_view_option::Stack1) ? TEMP_CHART1_STK1 : TEMP_CHART1_STK2)),
    m_stk_thermistor_temp_chart(getChartObject((stack == view_mode::dashboard_view_option::Stack1) ? TEMP_CHART2_STK1 : TEMP_CHART2_STK2)),
    m_stk_temp_grid(getGridObject((stack == view_mode::dashboard_view_option::Stack1) ? TEMP_STAT_GRID_STK1 : TEMP_STAT_GRID_STK2)),
    m_stk_sensor_temp_title(getTextImpl((stack == view_mode::dashboard_view_option::Stack1) ? TEMP_SENSOR_TITLE_STK1 : TEMP_SENSOR_TITLE_STK2)),
    m_stk_thermistor_temp_title(getTextImpl((stack == view_mode::dashboard_view_option::Stack1) ? TEMP_THERMISTOR_TITLE_STK1 : TEMP_THERMISTOR_TITLE_STK2)),
    m_stk_vc_title(getTextImpl((stack == view_mode::dashboard_view_option::Stack1) ? VC_TITLE_STK1 : VC_TITLE_STK2)),
    m_stk_temp_title(getTextImpl((stack == view_mode::dashboard_view_option::Stack1) ? TEMP_CHART_TITLE_STK1 : TEMP_CHART_TITLE_STK2)),
    m_stk_cbset_button(getTextImpl((stack == view_mode::dashboard_view_option::Stack1) ? CBSET_BUTTON_STK1 : CBSET_BUTTON_STK2)),
    m_bCBSET_Pressed(false),
    m_bEnable(true)
{
    EnableLayout();
}

dashboard_stack_container::~dashboard_stack_container() noexcept
{
}

void dashboard_stack_container::OnAfterCreateWnd()
{
    dashboard_view_stack_adapter::OnAfterCreateWnd();
    init_gui();
    init_layout();
}

void dashboard_stack_container::onChangeTheme()
{
    m_stk_cbset_button.SetFillBrush(getCBSETButton_fillcolor(m_bCBSET_Pressed));
    m_stk_cbset_button.SetOutlineBrush(getCBSETButton_outlinecolor(m_bCBSET_Pressed));
    m_stk_cbset_button.SetTextBrush(getCBSETButton_textcolor(m_bCBSET_Pressed));
    m_stk_cbset_button.Redraw();
}

void dashboard_stack_container::onClikedCBSETButton()
{
    if (BMS::cmdmanager::isConnected() == false || getMainFrame()->getRepetitionState())
        return;

    if (m_bEnable == false)
        return;

    m_bCBSET_Pressed = !m_bCBSET_Pressed;

    m_stk_cbset_button.SetFillBrush(getCBSETButton_fillcolor(m_bCBSET_Pressed));
    m_stk_cbset_button.SetOutlineBrush(getCBSETButton_outlinecolor(m_bCBSET_Pressed));
    m_stk_cbset_button.SetTextBrush(getCBSETButton_textcolor(m_bCBSET_Pressed));
    m_stk_cbset_button.Redraw();

    if (m_bCBSET_Pressed)
        m_stk_cb_grid.GetGridCtrl()->EnableWindow(false);
    else
        m_stk_cb_grid.GetGridCtrl()->EnableWindow(true);
}

bool dashboard_stack_container::isPressedCBSET()
{
    return m_bCBSET_Pressed;
}

void dashboard_stack_container::enableStack(bool bEnable)
{
    m_bEnable = bEnable;

    m_stk_cb_grid.GetGridCtrl()->EnableWindow(bEnable);
    m_stk_stat_grid.GetGridCtrl()->EnableWindow(bEnable);
    m_stk_temp_grid.GetGridCtrl()->EnableWindow(bEnable);
}

std::vector<std::wstring> dashboard_stack_container::getCBSET()
{
    std::vector<std::wstring> cbset;
    cbset.reserve(cb_max_row);

    CBCGPGridCtrl* pGrid = m_stk_cb_grid.GetGridCtrl();
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

void dashboard_stack_container::init_gui()
{
    try
    {
        enableStack(true);
        init_cb_grid();
        init_cbset_button();
        init_vc_chart();
        init_stat_grid();
        init_sensor_temp_chart();
        init_thermistor_temp_chart();
        init_temp_grid();

        Redraw();
    } catch (const std::exception& e) {
        DebugPrint("ERROR : stack dashboard init gui fail [{}]", e.what());
    }
}

void dashboard_stack_container::init_layout()
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

    pLayout->AddAnchor(GetByID((m_stack == view_mode::dashboard_view_option::Stack1) ? CB_STATIC_STK1 : CB_STATIC_STK2), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(0, 0), CPoint(50, 100));
    pLayout->AddAnchor(GetByID((m_stack == view_mode::dashboard_view_option::Stack1) ? CB_TITLE_STK1 : CB_TITLE_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(30, 0));
    pLayout->AddAnchor(GetByID((m_stack == view_mode::dashboard_view_option::Stack1) ? TEMP_STATIC_STK1 : TEMP_STATIC_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 0), CPoint(50, 100));
    pLayout->AddAnchor(GetByID((m_stack == view_mode::dashboard_view_option::Stack1) ? TEMP_TITLE_STK1 : TEMP_TITLE_STK2), CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(75, 0));
    pLayout->AddAnchor(&m_stk_vc_title, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(25, 0));
    pLayout->AddAnchor(&m_stk_temp_title, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeNone, CPoint(75, 0));
    pLayout->AddAnchor(&m_stk_sensor_temp_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 10));
    pLayout->AddAnchor(&m_stk_thermistor_temp_title, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone, CPoint(50, 60));
    pLayout->AddAnchor(&m_stk_cb_grid, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeVert, CPoint(0, 0), CPoint(0, 100));
    pLayout->AddAnchor(&m_stk_vc_chart, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(0, 0), CPoint(50, 100));
    pLayout->AddAnchor(&m_stk_stat_grid, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeVert, CPoint(50, 0), CPoint(0, 100));
    pLayout->AddAnchor(&m_stk_sensor_temp_chart, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 0), CPoint(50, 25));
    pLayout->AddAnchor(&m_stk_thermistor_temp_chart, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeBoth, CPoint(50, 25), CPoint(50, 75));
    pLayout->AddAnchor(&m_stk_temp_grid, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeVert, CPoint(100, 24), CPoint(0, 77));
    pLayout->AddAnchor(&m_stk_cbset_button, CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeNone, CPoint(0, 100));
}

void dashboard_stack_container::init_cb_grid()
{
    m_stk_cb_grid.SetAutoDestroy(TRUE);

    autoSizeGridCtrl* cb_grid = dynamic_cast<autoSizeGridCtrl*>(m_stk_cb_grid.GetGridCtrl());
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

    CRect gird_rect;
    cb_grid->GetClientRect(&gird_rect);

    for (const auto& [key, value] : grid_info::CB_columns)
    {
        switch (key)
        {
        case grid_info::CB::SET:
            cb_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.4));
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
        enableEditCBGrid(false);
    else
        enableEditCBGrid(true);
}

void dashboard_stack_container::init_cbset_button()
{
    m_bCBSET_Pressed = false;

    m_stk_cbset_button.SetFillBrush(getCBSETButton_fillcolor(false));
    m_stk_cbset_button.SetOutlineBrush(getCBSETButton_outlinecolor(false));
    m_stk_cbset_button.SetTextBrush(getCBSETButton_textcolor(false));
    m_stk_cbset_button.Redraw();
}

void dashboard_stack_container::init_vc_chart()
{
    m_stk_vc_chart.SetAutoDestroy(TRUE);

    m_stk_vc_chart.CleanUp();

    m_stk_vc_chart.ShowAxis(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
    m_stk_vc_chart.ShowAxisGridLines(BCGP_CHART_Y_PRIMARY_AXIS, 0, 0);
    m_stk_vc_chart.SetLegendPosition(BCGPChartLayout::LP_NONE);
    m_stk_vc_chart.ShowDataLabels(TRUE, FALSE, FALSE);
    m_stk_vc_chart.SetSeriesShadow(TRUE);

    CBCGPChartAxis* pY_axis = m_stk_vc_chart.GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pY_axis->SetFixedDisplayRange(vc_chart_container::vc_chart_min_y, vc_chart_container::vc_chart_max_y);
    pY_axis->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
    pY_axis->m_axisLabelsFormat = BCGPChartFormatLabel();

    CBCGPChartAxis* pX_axis = m_stk_vc_chart.GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pX_axis->m_crossType = CBCGPChartAxis::CT_MINIMUM_AXIS_VALUE;

    CBCGPChartSeries* pVoltage_series = m_stk_vc_chart.CreateSeries(_T("Cell Voltage"));
    pVoltage_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE);
    pVoltage_series->SetTreatNulls(CBCGPChartSeries::TN_NO_PAINT);

    CBCGPTextFormat format;
    format.SetFontSize(m_label_font_size);
    pVoltage_series->SetDataLabelTextFormat(format);

    for (int i = 0; i < cb_max_row; ++i)
    {
        pVoltage_series->AddEmptyDataPoint();

        CString vc_num = std::format(L"VC{}", i + 1).c_str();
        pVoltage_series->SetDataLabelDataFormat(vc_num + L"=%.3f V", i);
    }
}

void dashboard_stack_container::init_stat_grid()
{
    m_stk_stat_grid.SetAutoDestroy(TRUE);

    autoSizeGridCtrl* stat_grid = dynamic_cast<autoSizeGridCtrl*>(m_stk_stat_grid.GetGridCtrl());
    stat_grid->ShowVertScrollBar(FALSE);
    stat_grid->ShowHorzScrollBar(FALSE);
    stat_grid->RemoveAll();
    stat_grid->DeleteAllColumns();

    stat_grid->EnableHeader(TRUE, 0);
    stat_grid->EnableHeaderSelectAllMarker(FALSE);
    stat_grid->EnableRowHeader(TRUE, 0, 1);

    stat_grid->SetReadOnly(TRUE);

    CRect gird_rect;
    stat_grid->GetClientRect(&gird_rect);

    for (const auto& [key, value] : grid_info::VC_columns)
    {
        switch (key)
        {
        case grid_info::STAT_VC::VCB_DONE:
            stat_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.5));
            break;
        case grid_info::STAT_VC::UV:
            stat_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.25));
            break;
        case grid_info::STAT_VC::OV:
            stat_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.25));
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

void dashboard_stack_container::init_sensor_temp_chart()
{
    m_stk_sensor_temp_chart.CleanUp();

    m_stk_sensor_temp_chart.SetAutoDestroy(TRUE);

    // set title
    m_stk_sensor_temp_title.SetText(_T("On-Die\nTemp.Sensor"));

    m_stk_sensor_temp_chart.ShowAxis(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
    m_stk_sensor_temp_chart.ShowAxis(BCGP_CHART_Y_PRIMARY_AXIS, FALSE);
    m_stk_sensor_temp_chart.SetLegendPosition(BCGPChartLayout::LP_NONE);
    m_stk_sensor_temp_chart.ShowDataLabels(TRUE, FALSE, FALSE);
    m_stk_sensor_temp_chart.SetSeriesShadow(TRUE);

    CBCGPChartAxis* pY_axis = m_stk_sensor_temp_chart.GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pY_axis->SetFixedDisplayRange(temp_chart_container::temp_chart_min_y, temp_chart_container::temp_chart_max_y);

    CBCGPChartAxis* pX_axis = m_stk_sensor_temp_chart.GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pX_axis->SetAxisOffsets(3, 0);
    pX_axis->m_crossType = CBCGPChartAxis::CT_MINIMUM_AXIS_VALUE;

    CBCGPChartSeries* pTemp_series = m_stk_sensor_temp_chart.CreateSeries(_T("On-Die Temp.Sensor"));
    pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE);
    pTemp_series->SetTreatNulls(CBCGPChartSeries::TN_NO_PAINT);

    //auto destroy
    barLabelPosChangeImpl* bar = new barLabelPosChangeImpl(&m_stk_sensor_temp_chart);
    pTemp_series->SetChartImpl(bar);

    CBCGPTextFormat format;
    format.SetFontSize(m_label_font_size);
    pTemp_series->SetDataLabelTextFormat(format);

    for (int i = 0; i < sensor_temp_max_row; ++i)
    {
        pTemp_series->AddEmptyDataPoint();

        CString temp_num = std::format(L"DT{}", i + 1).c_str();
        pTemp_series->SetDataLabelDataFormat(temp_num + L"=%.1f °C", i);
    }
}

void dashboard_stack_container::init_thermistor_temp_chart()
{
    m_stk_thermistor_temp_chart.CleanUp();
    // set title
    m_stk_thermistor_temp_title.SetText(_T("GPIO Thermistor"));

    m_stk_thermistor_temp_chart.SetAutoDestroy(TRUE);

    m_stk_thermistor_temp_chart.ShowAxis(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
    m_stk_thermistor_temp_chart.ShowAxisGridLines(BCGP_CHART_Y_PRIMARY_AXIS, 0, 0);
    m_stk_thermistor_temp_chart.SetLegendPosition(BCGPChartLayout::LP_NONE);
    m_stk_thermistor_temp_chart.ShowDataLabels(TRUE, FALSE, FALSE);
    m_stk_thermistor_temp_chart.SetSeriesShadow(TRUE);

    CBCGPChartAxis* pY_axis = m_stk_thermistor_temp_chart.GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
    pY_axis->SetFixedDisplayRange(temp_chart_container::temp_chart_min_y, temp_chart_container::temp_chart_max_y);
    pY_axis->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
    pY_axis->m_axisLabelsFormat = BCGPChartFormatLabel();

    CBCGPChartAxis* pX_axis = m_stk_thermistor_temp_chart.GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
    pX_axis->m_crossType = CBCGPChartAxis::CT_MINIMUM_AXIS_VALUE;

    CBCGPChartSeries* pTemp_series = m_stk_thermistor_temp_chart.CreateSeries(_T("GPIO Thermistor"));
    pTemp_series->SetDataLabelPosition(BCGPChartDataLabelOptions::LabelPosition::LP_INSIDE_BASE);
    pTemp_series->SetTreatNulls(CBCGPChartSeries::TN_NO_PAINT);

    //auto destroy
    barLabelPosChangeImpl* bar = new barLabelPosChangeImpl(&m_stk_thermistor_temp_chart);
    pTemp_series->SetChartImpl(bar);

    CBCGPTextFormat format;
    format.SetFontSize(m_label_font_size);
    pTemp_series->SetDataLabelTextFormat(format);

    for (int i = 0; i < thermistor_temp_max_row; ++i)
    {
        pTemp_series->AddEmptyDataPoint();

        CString temp_num = std::format(L"NTC{}", i + 1).c_str();
        pTemp_series->SetDataLabelDataFormat(temp_num + L"=%.1f °C", i);
    }
}

void dashboard_stack_container::init_temp_grid()
{
    m_stk_temp_grid.SetAutoDestroy(TRUE);

    autoSizeGridCtrl* temp_grid = dynamic_cast<autoSizeGridCtrl*>(m_stk_temp_grid.GetGridCtrl());
    temp_grid->ShowVertScrollBar(FALSE);
    temp_grid->ShowHorzScrollBar(FALSE);
    temp_grid->RemoveAll();
    temp_grid->DeleteAllColumns();

    temp_grid->EnableHeader(TRUE, 0);
    temp_grid->EnableHeaderSelectAllMarker(FALSE);
    temp_grid->EnableRowHeader(TRUE);
    temp_grid->EnableLineNumbers();
    temp_grid->setReverseLineNumber(true);

    temp_grid->SetReadOnly(TRUE);

    CRect gird_rect;
    temp_grid->GetClientRect(&gird_rect);

    for (const auto& [key, value] : grid_info::TEMP_columns)
    {
        switch (key)
        {
        case grid_info::STAT_TEMP::UT:
            temp_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.25));
            break;
        case grid_info::STAT_TEMP::OT:
            temp_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.25));
            break;
        case grid_info::STAT_TEMP::OTCB:
            temp_grid->InsertColumn(key, value.c_str(), static_cast<uint32_t>(gird_rect.Width() * 0.4));
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

void dashboard_stack_container::enableEditCBGrid(bool bEnable)
{
    m_stk_cb_grid.GetGridCtrl()->EnableWindow(bEnable);
}

void dashboard_stack_container::drawChartLabelBorder(bool bDraw)
{
    drawVCChartLabelBorder(bDraw);
    drawSensorTempChartLabelBorder(bDraw);
    drawThermistorTempChartLabelBorder(bDraw);
}

void dashboard_stack_container::drawVCChartLabelBorder(bool bDraw)
{
    CBCGPChartSeries* pVoltage_series = m_stk_vc_chart.GetSeries(0);

    for (int32_t index = 0; index < pVoltage_series->GetDataPointCount(); ++index)
        pVoltage_series->SetDataLabelDrawBorder(bDraw, index);

    m_stk_vc_chart.SetDirty();
    m_stk_vc_chart.Redraw();
}

void dashboard_stack_container::drawSensorTempChartLabelBorder(bool bDraw)
{
    CBCGPChartSeries* pTemp_series = m_stk_sensor_temp_chart.GetSeries(0);

    for (int32_t index = 0; index < pTemp_series->GetDataPointCount(); ++index)
        pTemp_series->SetDataLabelDrawBorder(bDraw, index);

    m_stk_sensor_temp_chart.SetDirty();
    m_stk_sensor_temp_chart.Redraw();
}

void dashboard_stack_container::drawThermistorTempChartLabelBorder(bool bDraw)
{
    CBCGPChartSeries* pTemp_series = m_stk_thermistor_temp_chart.GetSeries(0);

    for (int32_t index = 0; index < pTemp_series->GetDataPointCount(); ++index)
        pTemp_series->SetDataLabelDrawBorder(bDraw, index);

    m_stk_thermistor_temp_chart.SetDirty();
    m_stk_thermistor_temp_chart.Redraw();
}

bool dashboard_stack_container::drawVCChart(const std::vector<double>& VC)
{
    if (isEnableStack() == false)
        return true;

    CBCGPChartSeries* pVoltage_series = m_stk_vc_chart.GetSeries(0);
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

        CBCGPRect bar_rect = pVoltage_series->GetDataPointBoundingRect(index);
        CBCGPRect label_rect = pVoltage_series->GetDataPointLabelRect(index);

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

    m_stk_vc_chart.SetDirty();
    m_stk_vc_chart.Redraw();

    return true;
}

bool dashboard_stack_container::drawSensorTempChart(const std::vector<double>& TEMP)
{
    if (isEnableStack() == false)
        return true;

    CBCGPChartSeries* pTemp_series = m_stk_sensor_temp_chart.GetSeries(0);
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

    m_stk_sensor_temp_chart.SetDirty();
    m_stk_sensor_temp_chart.Redraw();

    return true;
}

bool dashboard_stack_container::drawThermistorTempChart(const std::vector<double>& TEMP)
{
    if (isEnableStack() == false)
        return true;

    CBCGPChartSeries* pTemp_series = m_stk_thermistor_temp_chart.GetSeries(0);
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

        CBCGPRect bar_rect = pTemp_series->GetDataPointBoundingRect(index);
        CBCGPRect label_rect = pTemp_series->GetDataPointLabelRect(index);

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

    m_stk_thermistor_temp_chart.SetDirty();
    m_stk_thermistor_temp_chart.Redraw();

    return true;
}

bool dashboard_stack_container::drawVCChart_THR(const double& THR, const CBCGPBrush& thr_color)
{
    if (isEnableStack() == false)
        return true;

    CBCGPStrokeStyle type;
    type.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH);

    m_stk_vc_chart.AddChartLineObject(THR, TRUE, thr_color, 2, &type)->SetShadow();
    m_stk_vc_chart.AddChartAxisMarkObject(THR, std::format(L"{:.1f} V", THR).c_str(), TRUE, TRUE, CBCGPBrush(CBCGPColor::White), thr_color);

    m_stk_vc_chart.SetDirty();
    m_stk_vc_chart.Redraw();

    return true;
}

bool dashboard_stack_container::drawThermistorTempChart_THR(const double& TEMP_THR, const CBCGPBrush& thr_color)
{
    if (isEnableStack() == false)
        return true;

    CBCGPStrokeStyle type;
    type.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH);

    m_stk_thermistor_temp_chart.AddChartLineObject(TEMP_THR, TRUE, thr_color, 2, &type)->SetShadow();
    m_stk_thermistor_temp_chart.AddChartAxisMarkObject(TEMP_THR, std::format(L"{:.0f} °C", TEMP_THR).c_str(), TRUE, TRUE, CBCGPBrush(CBCGPColor::White), thr_color);

    m_stk_thermistor_temp_chart.SetDirty();
    m_stk_thermistor_temp_chart.Redraw();

    return true;
}

bool dashboard_stack_container::drawCBSET(const std::vector<std::wstring>& cbset)
{
    if (isEnableStack() == false)
        return true;

    if (cbset.empty())
        return true;

    CBCGPGridCtrl* cb_grid = m_stk_cb_grid.GetGridCtrl();

    if (cb_grid->GetRowCount() != cbset.size())
        return false;

    uint32_t index = cb_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const std::wstring& cb : cbset)
        cb_grid->GetRow(--index)->GetItem(grid_info::CB::SET)->SetValue(cb.c_str());

    return true;
}

bool dashboard_stack_container::drawCBLeft(const std::vector<std::wstring>& CBTIME_LEFT)
{
    if (isEnableStack() == false)
        return true;

    if (CBTIME_LEFT.empty())
        return true;

    CBCGPGridCtrl* cb_grid = m_stk_cb_grid.GetGridCtrl();

    if (cb_grid->GetRowCount() != CBTIME_LEFT.size())
        return false;

    uint32_t index = cb_grid->GetRowCount();
    if (index == 0)
        return false;

    for (const std::wstring& left_time : CBTIME_LEFT)
        cb_grid->GetRow(--index)->GetItem(grid_info::CB::TIME_LEFT)->SetValue(left_time.c_str());

    return true;
}

bool dashboard_stack_container::drawStatOV(const std::vector<bool>& stat_ov)
{
    if (isEnableStack() == false)
        return true;

    if (stat_ov.empty())
        return true;

    CBCGPGridCtrl* stat_grid = m_stk_stat_grid.GetGridCtrl();

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

bool dashboard_stack_container::drawStatUV(const std::vector<bool>& stat_uv)
{
    if (isEnableStack() == false)
        return true;

    if (stat_uv.empty())
        return true;

    CBCGPGridCtrl* stat_grid = m_stk_stat_grid.GetGridCtrl();

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

bool dashboard_stack_container::drawStatVCB(const std::vector<bool>& stat_vcb)
{
    if (isEnableStack() == false)
        return true;

    if (stat_vcb.empty())
        return true;

    CBCGPGridCtrl* stat_grid = m_stk_stat_grid.GetGridCtrl();

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

bool dashboard_stack_container::drawStatUT(const std::vector<bool>& stat_ut)
{
    if (isEnableStack() == false)
        return true;

    if (stat_ut.empty())
        return true;

    CBCGPGridCtrl* temp_grid = m_stk_temp_grid.GetGridCtrl();

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

bool dashboard_stack_container::drawStatOT(const std::vector<bool>& stat_ot)
{
    if (isEnableStack() == false)
        return true;

    if (stat_ot.empty())
        return true;

    CBCGPGridCtrl* temp_grid = m_stk_temp_grid.GetGridCtrl();

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

bool dashboard_stack_container::drawStatOTCB(const std::vector<bool>& stat_otcb)
{
    if (isEnableStack() == false)
        return true;

    if (stat_otcb.empty())
        return true;

    CBCGPGridCtrl* temp_grid = m_stk_temp_grid.GetGridCtrl();

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

void dashboard_stack_container::clearVCChart_THR()
{
    m_stk_vc_chart.RemoveAllChartObjects();
    m_stk_vc_chart.Invalidate();
}

void dashboard_stack_container::clearThermistorTempChart_THR()
{
    m_stk_thermistor_temp_chart.RemoveAllChartObjects();
    m_stk_thermistor_temp_chart.Invalidate();
}

void dashboard_stack_container::clear()
{
    clearVCChart_THR();
    clearThermistorTempChart_THR();

    init_gui();
}

bool dashboard_stack_container::isEnableStack()
{
    return m_bEnable;
}

BOOL dashboard_stack_container::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
    CBCGPGridItemID id;
    m_stk_cb_grid.GetGridCtrl()->SetCurSel(id, SM_NONE);

    return dashboard_adapter::OnMouseDown(nButton, pt);
}

mainFrame* dashboard_stack_container::getMainFrame() const noexcept
{
    return reinterpret_cast<mainFrame*>(AfxGetMainWnd());
}
