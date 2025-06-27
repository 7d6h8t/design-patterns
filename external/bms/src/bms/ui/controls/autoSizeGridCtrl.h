#pragma once
/*****************************************************************//**
 * @file   autoSizeGridCtrl.h
 * @brief  grid control class for row size auto adjust.
 * 
 * @author aslkdj1
 * @date   February 2023
 *********************************************************************/

/**
 * @brief grid control class for row size auto adjust.
 */
class autoSizeGridCtrl : public CBCGPGridCtrl
{
public:
    DECLARE_SERIAL(autoSizeGridCtrl)

    /**
     * @brief constructs the object.
     */
    autoSizeGridCtrl();

    /**
     * @brief destructs the object.
     */
    ~autoSizeGridCtrl() = default;

public:
    void setReverseLineNumber(bool bEnable);
    void setDrawRowHeader(bool bEnable);
    void setFixHeaderHeight(bool bEnable);

    bool isLineNumberReverse();
    bool isDrawRowHeader();
    bool isHeaderHeightFixed();

protected:
    virtual CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
    virtual void OnDrawLineNumber(CDC* pDC, CBCGPGridRow* pRow, CRect rect, BOOL bSelected, BOOL bPressed) override;
    virtual void SetRowHeight() override;
    virtual void SetRowHeaderWidth() override;

protected:
    bool m_bReverseLineNumber;
    bool m_bDrawRowHeader;
    bool m_bFixHeaderHeight;
};
