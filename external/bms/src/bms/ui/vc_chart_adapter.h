/*****************************************************************//**
 * @file   vc_chart_adapter.h
 * @brief  adapter class for vc_chart_container class.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "resource.h"
#include <cstdint>

/**
 * @brief adapter class for vc_chart_container class.
 */
class vc_chart_adapter : public CBCGPVisualContainer
{
public:
    enum
    {
        CHART_XML = IDR_BCGP_VC_CHART,
        VC_CHART_STK1 = 101,
        VC_GRID_STK1 = 102,
        VC_TITLE_STK1 = 1001,

        VC_CHART_STK2 = 201,
        VC_GRID_STK2 = 202,
        VC_TITLE_STK2 = 2001
    };

public:
    /**
     * @brief constructs the object.
     */
    vc_chart_adapter() noexcept;

    /**
     * @brief destructos the object.
     */
    virtual ~vc_chart_adapter() noexcept;

public:
    /**
     * @brief get container chart object with .visual-bcgsoft file's resource id.
     *
     * @param[in] nId           .visual-bcgsoft file's resource id
     * @return container chart object
     */
    CBCGPChartVisualObject& getChartObject(const uint32_t nId);

    /**
     * @brief get container grid object with .visual-bcgsoft file's resource id.
     *
     * @param[in] nId           .visual-bcgsoft file's resource id
     * @return container grid object
     */
    CBCGPGridVisualObject& getGridObject(const uint32_t nId);
};
