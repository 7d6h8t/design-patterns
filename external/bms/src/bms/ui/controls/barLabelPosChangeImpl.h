#pragma once

class barLabelPosChangeImpl : public CBCGPBarChartImpl
{
public:
    barLabelPosChangeImpl();
    barLabelPosChangeImpl(CBCGPChartVisualObject* pRelatedChartControl);

    virtual ~barLabelPosChangeImpl();

public:
    // bar chart 의 값이 음수인 경우, 시작 축 기준으로 양수와 반대방향으로 그래프가 그려지기 때문에 label 위치가 LP_OUTSIDE_END 인 경우
    // 반대방향에 label 이 생성됨 -> 음수인 경우에도 label 위치 정방향에 그리기 위해 override
    virtual void OnCalcDataPointLabelRect(CBCGPGraphicsManager* pGM, CBCGPChartDataPoint* pDataPoint, const CBCGPRect& rectDiagramArea,
        CBCGPChartSeries* pSeries, int nDataPointIndex) override;

    void setLabelBaseAtTop(bool bEnable);
    bool isLabelBaseAtTop();

protected:
    bool m_bLabelBaseAtTop;
};
