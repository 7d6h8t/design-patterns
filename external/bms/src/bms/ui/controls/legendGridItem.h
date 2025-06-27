/*****************************************************************//**
 * @file   legendGridItem.h
 * @brief  grid item class for indicate legend of chart.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include <string>

/**
 * @brief grid item class for indicate legend of chart.
 */
class legendGridItem final : public CBCGPGridItem
{
public:
    /**
     * @brief constructs the object.
     * 
     * @param[in] color     color of chart legend
     * @param[in] str       name of chart legend
     */
    legendGridItem(const CBCGPColor& color, const std::wstring& str) noexcept;

    /**
     * @brief destructs the object.
     */
    ~legendGridItem() noexcept;

public:
    virtual CWnd* CreateInPlaceEdit(CRect, BOOL&) override;
    virtual void OnDrawValue(CDC* pDC, CRect rect) override;

private:
    const CBCGPColor m_color;
    std::wstring m_str;

    static const int32_t thickness = 2;
    static const int32_t margin_x = 10;
};
