#include "pch.h"
#include "colorIndicaterGridItem.h"

colorIndicaterGridItem::colorIndicaterGridItem(const CBCGPColor& color) noexcept
    : m_color(color),
    m_bEnable(false)
{
    SetReadOnly(TRUE);
}

colorIndicaterGridItem::~colorIndicaterGridItem() noexcept
{
}

void colorIndicaterGridItem::OnDrawValue(CDC* pDC, CRect rect)
{
    if (m_bEnable == false)
    {
        CBCGPGridItem::OnDrawValue(pDC, rect);
        return;
    }

    CBrush newBrush;
    newBrush.CreateSolidBrush(m_color);
    CBrush* pOldBrush = pDC->SelectObject(&newBrush);

    CPoint center = rect.CenterPoint();

    pDC->Ellipse(center.x - radius, center.y - radius, center.x + radius + 1, center.y + radius + 1);

    pDC->SelectObject(pOldBrush);
    newBrush.DeleteObject();
}

void colorIndicaterGridItem::enableIndicater(bool bEnable)
{
    m_bEnable = bEnable;
}
