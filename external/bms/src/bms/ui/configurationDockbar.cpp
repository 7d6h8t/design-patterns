#include "pch.h"
#include "configurationDockbar.h"
#include "info/configuration_info.h"
#include <format>

BEGIN_MESSAGE_MAP(configurationDockbar, CBCGPDialogBar)
    ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
    ON_WM_SIZE()
END_MESSAGE_MAP()

configurationDockbar::configurationDockbar() noexcept
{
    EnableVisualManagerStyle();
    EnableLayout();
}

configurationDockbar::~configurationDockbar() noexcept
{
}

void configurationDockbar::drawIDN(const BMS_info::IDN& idn)
{
    drawDeviceID(idn.Device_ID);
    drawFirmwareVer(idn.Firmware_Ver);
}

void configurationDockbar::drawCONF(const BMS_info::CONF& conf)
{
    drawTStack(conf.Total_STK);
    drawStack1CellNum(conf.STK1_VC_Ch);
    drawStack1TempNum(conf.STK1_TEMP_Ch);
    drawStack2CellNum(conf.STK2_VC_Ch);
    drawStack2TempNum(conf.STK2_TEMP_Ch);
    drawOVThr(conf.OV_THR);
    drawUVThr(conf.UV_THR);
    drawVCBThr(conf.VCB_THR);
    drawOTCBThr(conf.OTCB_THR);
    drawOTThr(conf.OT_THR);
    drawUTThr(conf.UT_THR);
}

void configurationDockbar::drawDeviceID(const std::wstring& dev_id)
{
    CBCGPProp* pGroupIDN = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::IDN));
    pGroupIDN->GetSubItem(static_cast<uint32_t>(confDock_info::IdnItemIndex::DEV_ID))->SetValue(dev_id.c_str());
}

void configurationDockbar::drawFirmwareVer(const std::wstring& firm_ver)
{
    CBCGPProp* pGroupIDN = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::IDN));
    pGroupIDN->GetSubItem(static_cast<uint32_t>(confDock_info::IdnItemIndex::FirmVer))->SetValue(firm_ver.c_str());
}

void configurationDockbar::drawTStack(const uint32_t nStack)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::T_STACK))->SetValue(std::format(L"{}", nStack).c_str());
}

void configurationDockbar::drawStack1CellNum(const uint32_t num)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::STACK1_CELL_NO))->SetValue(std::format(L"{} Cell", num).c_str());
}

void configurationDockbar::drawStack1TempNum(const uint32_t num)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::STACK1_TEMP_NO))->SetValue(std::format(L"{} EA", num).c_str());
}

void configurationDockbar::drawStack2CellNum(const uint32_t num)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::STACK2_CELL_NO))->SetValue(std::format(L"{} Cell", num).c_str());
}

void configurationDockbar::drawStack2TempNum(const uint32_t num)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::STACK2_TEMP_NO))->SetValue(std::format(L"{} EA", num).c_str());
}

void configurationDockbar::drawOVThr(const double thr)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::OV_THR))->SetValue(std::format(L"{:.1f}V", thr).c_str());
}

void configurationDockbar::drawUVThr(const double thr)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::UV_THR))->SetValue(std::format(L"{:.1f}V", thr).c_str());
}

void configurationDockbar::drawVCBThr(const double thr)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::VCB_THR))->SetValue(std::format(L"{:.1f}V", thr).c_str());
}

void configurationDockbar::drawOTCBThr(const double thr)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::OTCB_THR))->SetValue(std::format(L"{:.0f}°C", thr).c_str());
}

void configurationDockbar::drawOTThr(const double thr)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::OT_THR))->SetValue(std::format(L"{:.0f}°C", thr).c_str());
}

void configurationDockbar::drawUTThr(const double thr)
{
    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    pGroupCONF->GetSubItem(static_cast<uint32_t>(confDock_info::ConfItemIndex::UT_THR))->SetValue(std::format(L"{:.0f}°C", thr).c_str());
}

void configurationDockbar::setIDN(const BMS_info::IDN& idn) noexcept
{
    m_idn = idn;
}

void configurationDockbar::setCONF(const BMS_info::CONF& conf) noexcept
{
    m_conf = conf;
}

void configurationDockbar::clear()
{
    setIDN(BMS_info::IDN());
    setCONF(BMS_info::CONF());

    CBCGPProp* pGroupIDN = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::IDN));
    for (int i = 0; i < pGroupIDN->GetSubItemsCount(); ++i)
        pGroupIDN->GetSubItem(i)->ResetOriginalValue();

    CBCGPProp* pGroupCONF = m_wndPropList.GetProperty(static_cast<uint32_t>(confDock_info::GroupIndex::CONF));
    for (int i = 0; i < pGroupCONF->GetSubItemsCount(); ++i)
        pGroupCONF->GetSubItem(i)->ResetOriginalValue();
}

void configurationDockbar::initLayout()
{
    CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();

    if (pLayout != nullptr)
        pLayout->AddAnchor(PROP_LIST, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
}

void configurationDockbar::initPropList()
{
    m_wndPropList.CreateOnPlaceHolder(this, IDC_PROPLIST_RECT, PROP_LIST, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, TRUE);

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea(TRUE);
    m_wndPropList.SetVSDotNetLook(TRUE);

    // auto destroy
    CBCGPProp* pGroupIDN = new CBCGPProp(confDock_info::groupStringMap.at(confDock_info::GroupIndex::IDN).c_str());
    for (const auto&[index, name] : confDock_info::idnStringMap)
        pGroupIDN->AddSubItem(new CBCGPProp(name.c_str(), _T("")));
    m_wndPropList.AddProperty(pGroupIDN);

    // auto destroy
    CBCGPProp* pGroupCONF = new CBCGPProp(confDock_info::groupStringMap.at(confDock_info::GroupIndex::CONF).c_str());
    for (const auto&[index, name] : confDock_info::confStringMap)
        pGroupCONF->AddSubItem(new CBCGPProp(name.c_str(), _T("")));
    m_wndPropList.AddProperty(pGroupCONF);

    enableEdit(false);
}

void configurationDockbar::enableEdit(bool bEnable)
{
    for (int group_index = 0; group_index < m_wndPropList.GetPropertyCount(); ++group_index)
        for (int item_index = 0; item_index < m_wndPropList.GetProperty(group_index)->GetSubItemsCount(); ++item_index)
            m_wndPropList.GetProperty(group_index)->GetSubItem(item_index)->AllowEdit(bEnable);
}

LRESULT configurationDockbar::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = CBCGPDialogBar::HandleInitDialog(wParam, lParam);
    initPropList();
    initLayout();

    return lRes;
}

void configurationDockbar::OnSize(UINT nType, int cx, int cy)
{
    CBCGPDialogBar::OnSize(nType, cx, cy);
}

BMS_info::IDN configurationDockbar::getIDN() const noexcept
{
    return m_idn;
}

BMS_info::CONF configurationDockbar::getCONF() const noexcept
{
    return m_conf;
}
