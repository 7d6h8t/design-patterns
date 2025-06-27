#include "pch.h"
#include "dashboard_adapter.h"
#include "utils.h"
#include "ui/bmsApp.h"

const CBCGPBrush dashboard_adapter::VC_default = CBCGPBrush(CBCGPColor::MediumAquamarine);
const CBCGPBrush dashboard_adapter::SensorTemp_default = CBCGPBrush(CBCGPColor::CornflowerBlue);
const CBCGPBrush dashboard_adapter::OV_thr_color = CBCGPBrush(CBCGPColor::DodgerBlue);
const CBCGPBrush dashboard_adapter::VCB_thr_color = CBCGPBrush(CBCGPColor::Peru);
const CBCGPBrush dashboard_adapter::UV_thr_color = CBCGPBrush(CBCGPColor::OrangeRed);
const CBCGPBrush dashboard_adapter::OT_thr_color = CBCGPBrush(CBCGPColor::OrangeRed);
const CBCGPBrush dashboard_adapter::OTCB_thr_color = CBCGPBrush(CBCGPColor::Peru);
const CBCGPBrush dashboard_adapter::UT_thr_color = CBCGPBrush(CBCGPColor::DodgerBlue);
const CBCGPBrush dashboard_adapter::CBSET_defualt_fill_color = CBCGPBrush(CBCGPColor::WhiteSmoke);
const CBCGPBrush dashboard_adapter::CBSET_defualt_outline_color = CBCGPBrush(CBCGPColor::LightGray);
const CBCGPBrush dashboard_adapter::CBSET_defualt_text_color = CBCGPBrush(CBCGPColor::Gray);
const CBCGPBrush dashboard_adapter::CBSET_defualt_dark_fill_color = CBCGPBrush(0x606060);
const CBCGPBrush dashboard_adapter::CBSET_defualt_dark_outline_color = CBCGPBrush();
const CBCGPBrush dashboard_adapter::CBSET_defualt_dark_text_color = CBCGPBrush(0xC4C4C4);
const CBCGPBrush dashboard_adapter::CBSET_pressed_fill_color = CBCGPBrush(CBCGPColor::DeepSkyBlue);
const CBCGPBrush dashboard_adapter::CBSET_pressed_outline_color = CBCGPBrush(CBCGPColor::DodgerBlue);
const CBCGPBrush dashboard_adapter::CBSET_pressed_text_color = CBCGPBrush(CBCGPColor::White);

dashboard_adapter::dashboard_adapter() noexcept
{
}

dashboard_adapter::~dashboard_adapter() noexcept
{
}

CBCGPGridVisualObject& dashboard_adapter::getGridObject(const uint32_t nId)
{
    CBCGPGridVisualObject* pGridObject = dynamic_cast<CBCGPGridVisualObject*>(GetByID(nId));
    if (pGridObject == nullptr)
        DebugPrint("ERROR : ID [{}] is not a Grid Object !", nId);

    return *pGridObject;
}

CBCGPChartVisualObject& dashboard_adapter::getChartObject(const uint32_t nId)
{
    CBCGPChartVisualObject* pChartObject = dynamic_cast<CBCGPChartVisualObject*>(GetByID(nId));
    if (pChartObject == nullptr)
        DebugPrint("ERROR : ID [{}] is not a Chart Object !", nId);

    return *pChartObject;
}

CBCGPCircularGaugeImpl& dashboard_adapter::getGaugeImpl(const uint32_t nId)
{
    CBCGPCircularGaugeImpl* pGaugeImpl = dynamic_cast<CBCGPCircularGaugeImpl*>(GetByID(nId));
    if (pGaugeImpl == nullptr)
        DebugPrint("ERROR : ID [{}] is not a Gauge Impl !", nId);

    return *pGaugeImpl;
}

CBCGPTextGaugeImpl& dashboard_adapter::getTextImpl(const uint32_t nId)
{
    CBCGPTextGaugeImpl* pTextImpl = dynamic_cast<CBCGPTextGaugeImpl*>(GetByID(nId));
    if (pTextImpl == nullptr)
        DebugPrint("ERROR : ID [{}] is not a Text Impl !", nId);

    return *pTextImpl;
}

CBCGPBrush dashboard_adapter::getCBSETButton_fillcolor(bool bPressed)
{
    bmsApp::BCGP_VISUAL_THEME cur_theme = theApp.GetVisualTheme();

    if (cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE)
        return bPressed ? CBSET_pressed_fill_color : CBSET_defualt_fill_color;
    else
        return bPressed ? CBSET_pressed_fill_color : CBSET_defualt_dark_fill_color;
}

CBCGPBrush dashboard_adapter::getCBSETButton_outlinecolor(bool bPressed)
{
    bmsApp::BCGP_VISUAL_THEME cur_theme = theApp.GetVisualTheme();

    if (cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE)
        return bPressed ? CBSET_pressed_outline_color : CBSET_defualt_outline_color;
    else
        return bPressed ? CBSET_pressed_outline_color : CBSET_defualt_dark_outline_color;
}

CBCGPBrush dashboard_adapter::getCBSETButton_textcolor(bool bPressed)
{
    bmsApp::BCGP_VISUAL_THEME cur_theme = theApp.GetVisualTheme();

    if (cur_theme == bmsApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE)
        return bPressed ? CBSET_pressed_text_color : CBSET_defualt_text_color;
    else
        return bPressed ? CBSET_pressed_text_color : CBSET_defualt_dark_text_color;
}
