#pragma once
//#include <vector>
//#include <map>
#include "stdafx.h"
#include "ACEdit.h"
#include "PaletteStruct.h"
#include "Resource.h"

// CDlgPaletteContent 대화 상자입니다.
class CxSkinButton;

class CDlgPaletteContent : public CDialog
{
    DECLARE_DYNAMIC(CDlgPaletteContent)

public:
    CDlgPaletteContent(CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~CDlgPaletteContent();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_DIALOG_PALETTE_CONTENT };

    OBJECT_TYPE_EACTION     m_enType;
    void SetObjectType(OBJECT_TYPE_EACTION enType) { m_enType = enType; }
    OBJECT_TYPE_EACTION GetObjectType(void) { return m_enType; }

    std::vector<CxSkinButton*>   m_vtButton;
    void CreateButtonWindow(void);

    int ReSizeButton(int iStartVerLine);
    void Refresh(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg LRESULT OnClickToolButton(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCbnSelchangeComboId();
    CACEdit m_cbToolPalette;
    virtual BOOL OnInitDialog();
};
