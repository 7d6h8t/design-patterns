/*****************************************************************//**
 * @file   cbsetGridItem.h
 * @brief  grid item class for constructs cb(cell balancing) time list combobox.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include <vector>
#include <string>

/**
 * @brief grid item class for constructs cb(cell balancing) time list combobox.
 */
class cbsetGridItem final : public CBCGPGridItem
{
public:
    /**
     * @brief constructs the object.
     */
    cbsetGridItem() noexcept;

    /**
     * @brief destructs the object.
     */
    ~cbsetGridItem() noexcept;

    //nonecopyable
    cbsetGridItem(const cbsetGridItem&) = delete;
    cbsetGridItem& operator=(const cbsetGridItem&) = delete;
    //nonemovable
    cbsetGridItem(cbsetGridItem&&) = delete;
    cbsetGridItem& operator=(cbsetGridItem&&) = delete;

private:
    virtual void OnSelectCombo() override;

    /**
     * @brief initialize combobox with cb(cell balancing) time list.
     */
    void initComboList() noexcept;

private:
    static const std::vector<std::wstring> settable_CB_Time;
}; //class cbsetGridItem final : public CBCGPGridItem
