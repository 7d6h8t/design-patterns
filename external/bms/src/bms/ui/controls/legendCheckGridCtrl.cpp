#include "pch.h"
#include "legendCheckGridCtrl.h"

IMPLEMENT_SERIAL(legendCheckGridCtrl, CBCGPGridCtrl, 1)

void legendCheckGridCtrl::registCallback(fNotifyChecked&& fCheckedNotify)
{
    m_fCheckedNotify = fCheckedNotify;
}

void legendCheckGridCtrl::OnRowCheckBoxClick(CBCGPGridRow* pRow)
{
    CBCGPGridCtrl::OnRowCheckBoxClick(pRow);

    std::vector<int32_t> rows = { pRow->GetRowId() };

    notify(GetDlgCtrlID(), rows, pRow->GetCheck() == BST_CHECKED);
}

void legendCheckGridCtrl::OnHeaderCheckBoxClick(int nColumn)
{
    CBCGPGridCtrl::OnHeaderCheckBoxClick(nColumn);

    bool bEnable = GetColumnsInfo().GetCheckBoxState() == BST_CHECKED ? true : false;
    CheckAll(bEnable);

    int32_t rowCount = GetRowCount();

    std::vector<int32_t> rows;
    rows.reserve(rowCount);
    for (int32_t i = 0; i < rowCount; ++i)
        rows.push_back(i);

    notify(GetDlgCtrlID(), rows, bEnable);
}

void legendCheckGridCtrl::notify(const uint32_t ctrlID, const std::vector<int32_t> nRows, const bool bVisible)
{
    if (m_fCheckedNotify != nullptr)
        m_fCheckedNotify(ctrlID, nRows, bVisible);
}
