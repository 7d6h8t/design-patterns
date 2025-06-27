#include "pch.h"
#include "autoSizeGridCtrl.h"

IMPLEMENT_SERIAL(autoSizeGridCtrl, CBCGPGridCtrl, 1)

autoSizeGridCtrl::autoSizeGridCtrl()
    : m_bReverseLineNumber(false),
    m_bDrawRowHeader(true),
    m_bFixHeaderHeight(true)
{
}

void autoSizeGridCtrl::setReverseLineNumber(bool bEnable)
{
    m_bReverseLineNumber = bEnable;
}

void autoSizeGridCtrl::setDrawRowHeader(bool bEnable)
{
    EnableRowHeader(TRUE);
    m_bDrawRowHeader = bEnable;
}

void autoSizeGridCtrl::setFixHeaderHeight(bool bEnable)
{
    m_bFixHeaderHeight = bEnable;
}

bool autoSizeGridCtrl::isLineNumberReverse()
{
    return m_bReverseLineNumber;
}

bool autoSizeGridCtrl::isDrawRowHeader()
{
    return m_bDrawRowHeader;
}

bool autoSizeGridCtrl::isHeaderHeightFixed()
{
    return m_bFixHeaderHeight;
}

CRect autoSizeGridCtrl::OnGetHeaderRect(CDC* pDC, const CRect& rectDraw)
{
    if (GetRowCount() == 0 || m_bFixHeaderHeight == false)
        return CBCGPGridCtrl::OnGetHeaderRect(pDC, rectDraw);

    CRect rect = GetHeaderRect();
    return rect;
}

void autoSizeGridCtrl::OnDrawLineNumber(CDC* pDC, CBCGPGridRow* pRow, CRect rect, BOOL bSelected, BOOL bPressed)
{
    ASSERT_VALID(pDC);
    ASSERT_VALID(this);

    if (pRow != NULL)
    {
        ASSERT_VALID(pRow);

        //-----------
        // Draw text:
        //-----------
        COLORREF clrText = (COLORREF)-1;

        if (IsDisabledText())
        {
            clrText = GetDisabledTextColor();
        }
        else
        {
            clrText = (bSelected && m_ColorData.m_HeaderSelColors.m_clrText != (COLORREF)-1)
                ? m_ColorData.m_HeaderSelColors.m_clrText
                : m_ColorData.m_HeaderColors.m_clrText;
            if (clrText == (COLORREF)-1)
            {
                clrText = CBCGPVisualManager::GetInstance()->GetGridHeaderItemTextColor(this, bSelected, FALSE);
            }
        }

        COLORREF clrTextOld = (COLORREF)-1;
        if (clrText != (COLORREF)-1)
        {
            clrTextOld = pDC->SetTextColor(clrText);
        }

        CRect rectLabel = rect;
        rectLabel.DeflateRect(GetTextMarginDPI(), 0);

        UINT uiTextFlags = DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX | DT_CENTER;

        if (pRow->GetLinesNumber() == 1)
        {
            uiTextFlags |= DT_VCENTER;
        }
        else
        {
            rectLabel.DeflateRect(0, GetTextVMarginDPI());
        }

        CString num;
        if (m_bReverseLineNumber)
        {
            int32_t current_row_num = pRow->GetRowId();
            int32_t reverse_row_num = GetRowCount() - current_row_num;

            num.Format(_T("%d"), reverse_row_num);
        }
        else
            num = GetRowNumber(pRow);


        pDC->DrawText(num, rectLabel, uiTextFlags);

        if (clrTextOld != (COLORREF)-1)
        {
            pDC->SetTextColor(clrTextOld);
        }
    }
}

void autoSizeGridCtrl::SetRowHeight()
{
    if (GetRowCount() == 0)
    {
        CBCGPGridCtrl::SetRowHeight();
        return;
    }

    CRect rect;
    GetClientRect(&rect);

    int grid_height = rect.Height();
    int header_hegiht = GetHeaderRect().Height();

    int adjusted_height = (grid_height - header_hegiht) / GetRowCount();

    m_nBaseHeight = adjusted_height;
    m_nRowHeight = m_nBaseHeight;
    m_nLargeRowHeight = m_nBaseHeight;
    m_nButtonWidth = m_nBaseHeight;
}

void autoSizeGridCtrl::SetRowHeaderWidth()
{
    if (m_bDrawRowHeader == false)
        m_nRowHeaderWidth = 0;
    else
        CBCGPGridCtrl::SetRowHeaderWidth();
}
