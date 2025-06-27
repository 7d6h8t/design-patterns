/*****************************************************************//**
 * @file   colorIndicaterGridItem.h
 * @brief  grid item class for indicate color circle.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once

/**
 * @brief grid item class for indicate color circle.
 */
class colorIndicaterGridItem final : public CBCGPGridItem
{
public:
    /**
     * @brief constructs the object.
     * 
     * @param[in] color     color of indicator
     */
    colorIndicaterGridItem(const CBCGPColor& color) noexcept;

    /**
     * @brief destructs the object.
     * 
     * @return 
     */
    ~colorIndicaterGridItem() noexcept;

    //nonecopyable
    colorIndicaterGridItem(const colorIndicaterGridItem&) = delete;
    colorIndicaterGridItem& operator=(const colorIndicaterGridItem&) = delete;
    //nonemovable
    colorIndicaterGridItem(colorIndicaterGridItem&&) = delete;
    colorIndicaterGridItem& operator=(colorIndicaterGridItem&&) = delete;

public:
    virtual void OnDrawValue(CDC* pDC, CRect rect) override;

    /**
     * @brief enable color indicate or disable.
     * 
     * @param[in] bEnable   true is enable
     */
    void enableIndicater(bool bEnable);

private:
    static constexpr size_t radius = 7;
    const CBCGPColor m_color;
    bool m_bEnable;
}; //class colorIndicaterGridItem : public CBCGPGridItem
