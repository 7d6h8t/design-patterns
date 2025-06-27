/*****************************************************************//**
 * @file   legendCheckGridCtrl.h
 * @brief  grid control class for legend of chart with checkbox.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include <functional>
#include <vector>
#include "autoSizeGridCtrl.h"

/**
 * @brief grid control class for legend of chart with checkbox.
 */
class legendCheckGridCtrl final : public CBCGPGridCtrl
{
public:
    using fNotifyChecked = std::function<void(const uint32_t ctrlID, const std::vector<int32_t> nRows, const bool bVisible)>;

public:
    DECLARE_SERIAL(legendCheckGridCtrl)

    /**
     * @brief constructs the object.
     */
    legendCheckGridCtrl() = default;

    /**
     * @brief destructs the object.
     */
    ~legendCheckGridCtrl() = default;

public:
    /**
     * @brief function to register callback function about checkbox operation.
     * 
     * @param[in] fCheckedNotify        function to handle about checkbox operation
     */
    void registCallback(fNotifyChecked&& fCheckedNotify);

private:
    virtual void OnRowCheckBoxClick(CBCGPGridRow* pRow) override;
    virtual void OnHeaderCheckBoxClick(int nColumn) override;

    /**
     * @brief notify about checkbox operation to registered callback function.
     * 
     * @param[in] ctrlID        grid control resource id
     * @param[in] nRows         grid row number about checkbox operation
     * @param[in] bVisible      visible on, off
     */
    void notify(const uint32_t ctrlID, const std::vector<int32_t> nRows, const bool bVisible);

private:
    fNotifyChecked m_fCheckedNotify;
};
