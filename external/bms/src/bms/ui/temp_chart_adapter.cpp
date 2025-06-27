#include "pch.h"
#include "temp_chart_adapter.h"
#include "utils.h"

temp_chart_adapter::temp_chart_adapter() noexcept
{
    LoadFromXML(CHART_XML);
}

temp_chart_adapter::~temp_chart_adapter() noexcept
{
}

CBCGPChartVisualObject& temp_chart_adapter::getChartObject(const uint32_t nId)
{
    CBCGPChartVisualObject* pChartObject = dynamic_cast<CBCGPChartVisualObject*>(GetByID(nId));
    if (pChartObject == nullptr)
        DebugPrint("ERROR : ID [{}] is not a Chart Object !", nId);

    return *pChartObject;
}

CBCGPGridVisualObject& temp_chart_adapter::getGridObject(const uint32_t nId)
{
    CBCGPGridVisualObject* pGridObject = dynamic_cast<CBCGPGridVisualObject*>(GetByID(nId));
    if (pGridObject == nullptr)
        DebugPrint("ERROR : ID [{}] is not a Grid Object !", nId);

    return *pGridObject;
}
