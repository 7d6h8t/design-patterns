#include "pch.h"
#include "cbsetGridItem.h"

const std::vector<std::wstring> cbsetGridItem::settable_CB_Time = {L"0", L"10sec", L"30sec", L"60sec", L"5min",
L"10min", L"20min", L"30min", L"40min", L"50min", L"60min", L"70min", L"80min", L"90min", L"100min", L"110min", L"120min",
L"150min", L"180min", L"210min", L"240min", L"270min", L"300min", L"330min", L"360min", L"390min", L"420min", L"450min", L"480min",
L"510min", L"540min", L"600min"
};

cbsetGridItem::cbsetGridItem() noexcept
{
    m_dwFlags = BCGP_GRID_ITEM_HAS_LIST;
    SetReadOnly(TRUE);
    initComboList();
}

cbsetGridItem::~cbsetGridItem() noexcept
{
    // auto destroy
}

void cbsetGridItem::OnSelectCombo()
{
    SetValue(GetOption(m_pWndCombo->GetCurSel()));
}

void cbsetGridItem::initComboList() noexcept
{
    for (const std::wstring& cb_time : settable_CB_Time)
        AddOption(cb_time.c_str());
}
