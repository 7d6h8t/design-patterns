#include "pch.h"
#include "legendGridItem.h"

legendGridItem::legendGridItem(const CBCGPColor& color, const std::wstring& str) noexcept
    : CBCGPGridItem(str.c_str()),
    m_color(color),
    m_str(str)
{
    SetReadOnly(TRUE);
}

legendGridItem::~legendGridItem() noexcept
{
}

CWnd* legendGridItem::CreateInPlaceEdit(CRect, BOOL&)
{
    return nullptr;
}

void legendGridItem::OnDrawValue(CDC* pDC, CRect rect)
{
    ASSERT_VALID(pDC);

    CRect colorRect = rect;
    colorRect.DeflateRect(0, 0, margin_x, 0);

    pDC->DrawText(m_str.c_str(), colorRect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

    colorRect.DeflateRect(margin_x, 0);
    colorRect.right -= pDC->GetTextExtent(m_str.c_str()).cx;

    CPen linePen;
    linePen.CreatePen(PS_SOLID, thickness, m_color);
    CPen* pOldPen = pDC->SelectObject(&linePen);

    pDC->MoveTo(colorRect.left, colorRect.CenterPoint().y);
    pDC->LineTo(colorRect.right, colorRect.CenterPoint().y);

    pDC->SelectObject(pOldPen);
    linePen.DeleteObject();
}
