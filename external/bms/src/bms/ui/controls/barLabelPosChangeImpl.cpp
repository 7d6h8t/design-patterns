#include "pch.h"
#include "barLabelPosChangeImpl.h"

barLabelPosChangeImpl::barLabelPosChangeImpl()
    : m_bLabelBaseAtTop(false)
{
}

barLabelPosChangeImpl::barLabelPosChangeImpl(CBCGPChartVisualObject* pRelatedChartControl)
    : CBCGPBarChartImpl(pRelatedChartControl),
    m_bLabelBaseAtTop(false)
{
}

barLabelPosChangeImpl::~barLabelPosChangeImpl()
{
}

void barLabelPosChangeImpl::OnCalcDataPointLabelRect(CBCGPGraphicsManager* pGM, CBCGPChartDataPoint* pDataPoint, const CBCGPRect& rectDiagramArea, CBCGPChartSeries* pSeries, int nDataPointIndex)
{
    ASSERT_VALID(this);
    ASSERT_VALID(pSeries);

    if (pDataPoint == NULL || pSeries == NULL || pSeries->IsDataPointScreenPointsEmpty(nDataPointIndex))
    {
        return;
    }

    BCGPChartFormatSeries* pFormatSeries = NULL;
    CBCGPSize szDataLabelSize;
    CBCGPPoint ptMarker;
    CBCGPRect rectBounds;

    if (!OnPrepareDataToCalcDataPointLabelRect(pGM, rectDiagramArea, pDataPoint, pSeries, nDataPointIndex,
        &pFormatSeries, szDataLabelSize, ptMarker, rectBounds))
    {
        return;
    }

    // Get X axis
    CBCGPChartAxis* pXAxis = pSeries->GetRelatedAxis(CBCGPChartSeries::AI_X);

    if (pXAxis == NULL)
    {
        return;
    }

    ASSERT_VALID(pXAxis);

    CBCGPRect rectDataLabel;
    CBCGPPoint ptCenter = rectBounds.CenterPoint();

    BOOL bBaseAtTop = pXAxis->IsVertical() ? ptMarker.x == rectBounds.left : ptMarker.y == rectBounds.bottom;

    if (pXAxis->IsVertical() && rectBounds.Width() == 0 || !pXAxis->IsVertical() && rectBounds.Height() == 0)
    {
        bBaseAtTop = FALSE;
    }

    if (m_bLabelBaseAtTop == false)
        bBaseAtTop = m_bLabelBaseAtTop;

    double dblDistance = pSeries->GetDataPointLabelDistance(nDataPointIndex);

    if (pFormatSeries->m_dataLabelFormat.m_options.m_position == BCGPChartDataLabelOptions::LP_INSIDE_BASE ||
        pFormatSeries->m_dataLabelFormat.m_options.m_position == BCGPChartDataLabelOptions::LP_INSIDE_END)
    {
        dblDistance = pXAxis->IsVertical() ? dblDistance * rectBounds.Width() / 2. / 100. :
            dblDistance * rectBounds.Height() / 2. / 100.;
    }

    BCGPChartDataLabelOptions::LabelPosition pos = pFormatSeries->m_dataLabelFormat.m_options.m_position;

    if (pos == BCGPChartDataLabelOptions::LP_DEFAULT_POS)
    {
        pos = pSeries->IsStakedSeries() ? (m_pRelatedChart->IsChart3D() ? BCGPChartDataLabelOptions::LP_CENTER : BCGPChartDataLabelOptions::LP_INSIDE_BASE) :
            BCGPChartDataLabelOptions::LP_OUTSIDE_END;
    }

    if (pXAxis->IsVertical() && m_pRelatedChart->IsChart3D())
    {
        pos = BCGPChartDataLabelOptions::LP_CENTER;
    }

    switch (pos)
    {
    case BCGPChartDataLabelOptions::LP_CENTER:
        rectDataLabel.SetRect(ptCenter.x - szDataLabelSize.cx / 2,
            ptCenter.y - szDataLabelSize.cy / 2,
            ptCenter.x + szDataLabelSize.cx / 2,
            ptCenter.y + szDataLabelSize.cy / 2);
        ptMarker = rectDataLabel.CenterPoint();
        break;
    case BCGPChartDataLabelOptions::LP_INSIDE_BASE:
        if (pXAxis->IsVertical())
        {
            rectDataLabel.top = ptCenter.y - szDataLabelSize.cy / 2;
            rectDataLabel.bottom = rectDataLabel.top + szDataLabelSize.cy;

            if (bBaseAtTop)
            {
                rectDataLabel.right = rectBounds.right - dblDistance;
                rectDataLabel.left = rectDataLabel.right - szDataLabelSize.cx;
            }
            else
            {
                rectDataLabel.left = rectBounds.left + dblDistance;
                rectDataLabel.right = rectDataLabel.left + szDataLabelSize.cx;
            }
        }
        else
        {
            rectDataLabel.left = ptCenter.x - szDataLabelSize.cx / 2;
            rectDataLabel.right = rectDataLabel.left + szDataLabelSize.cx;

            if (bBaseAtTop)
            {
                rectDataLabel.top = rectBounds.top + dblDistance;
                rectDataLabel.bottom = rectDataLabel.top + szDataLabelSize.cy;
            }
            else
            {
                rectDataLabel.bottom = rectBounds.bottom - dblDistance;
                rectDataLabel.top = rectDataLabel.bottom - szDataLabelSize.cy;
            }
        }
        AlignRectToArea(rectBounds, rectDataLabel, !pXAxis->IsVertical(), pXAxis->IsVertical());
        ptMarker = rectDataLabel.CenterPoint();
        break;

    case BCGPChartDataLabelOptions::LP_INSIDE_END:
        if (pXAxis->IsVertical())
        {
            rectDataLabel.top = ptCenter.y - szDataLabelSize.cy / 2;
            rectDataLabel.bottom = rectDataLabel.top + szDataLabelSize.cy;

            if (bBaseAtTop)
            {
                rectDataLabel.left = rectBounds.left + dblDistance;
                rectDataLabel.right = rectDataLabel.left + szDataLabelSize.cx;
            }
            else
            {
                rectDataLabel.right = rectBounds.right - dblDistance;
                rectDataLabel.left = rectDataLabel.right - szDataLabelSize.cx;
            }
        }
        else
        {
            rectDataLabel.left = ptCenter.x - szDataLabelSize.cx / 2;
            rectDataLabel.right = rectDataLabel.left + szDataLabelSize.cx;

            if (bBaseAtTop)
            {
                rectDataLabel.bottom = rectBounds.bottom - dblDistance;
                rectDataLabel.top = rectDataLabel.bottom - szDataLabelSize.cy;
            }
            else
            {
                rectDataLabel.top = rectBounds.top + dblDistance;
                rectDataLabel.bottom = rectDataLabel.top + szDataLabelSize.cy;
            }
        }

        AlignRectToArea(rectBounds, rectDataLabel, !pXAxis->IsVertical(), pXAxis->IsVertical());
        ptMarker = rectDataLabel.CenterPoint();
        break;
    case BCGPChartDataLabelOptions::LP_OUTSIDE_END:
        if (pXAxis->IsVertical())
        {
            if (m_pRelatedChart->IsChart3D())
            {
                rectDataLabel.top = ptMarker.y - szDataLabelSize.cy / 2;
            }
            else
            {
                rectDataLabel.top = ptCenter.y - szDataLabelSize.cy / 2;
            }

            rectDataLabel.bottom = rectDataLabel.top + szDataLabelSize.cy;

            if (!m_pRelatedChart->IsChart3D())
            {
                if (bBaseAtTop)
                {
                    rectDataLabel.right = rectBounds.left - dblDistance;
                    rectDataLabel.left = rectDataLabel.right - szDataLabelSize.cx;
                }
                else
                {
                    rectDataLabel.left = rectBounds.right + dblDistance;
                    rectDataLabel.right = rectDataLabel.left + szDataLabelSize.cx;
                }
            }
            else
            {
                rectDataLabel.left = ptMarker.x + dblDistance;
                rectDataLabel.right = rectDataLabel.left + szDataLabelSize.cx;
            }
        }
        else
        {
            rectDataLabel.left = ptCenter.x - szDataLabelSize.cx / 2;
            rectDataLabel.right = rectDataLabel.left + szDataLabelSize.cx;

            if (bBaseAtTop)
            {
                rectDataLabel.top = rectBounds.bottom + dblDistance;
                rectDataLabel.bottom = rectDataLabel.top + szDataLabelSize.cy;
            }
            else
            {
                rectDataLabel.bottom = rectBounds.top - dblDistance;
                rectDataLabel.top = rectDataLabel.bottom - szDataLabelSize.cy;
            }
        }
        break;

    default:
        CBCGPBarChartImpl::OnCalcDataPointLabelRect(pGM, pDataPoint, rectDiagramArea, pSeries, nDataPointIndex);
        return;
    }

    SetDataPointLabelRectAndDropLine(pSeries, nDataPointIndex, pFormatSeries, rectDataLabel, ptMarker);
}

void barLabelPosChangeImpl::setLabelBaseAtTop(bool bEnable)
{
    m_bLabelBaseAtTop = bEnable;
}

bool barLabelPosChangeImpl::isLabelBaseAtTop()
{
    return m_bLabelBaseAtTop;
}
