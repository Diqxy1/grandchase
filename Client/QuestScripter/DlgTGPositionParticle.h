#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "PositionParticle.h"


// CDlgTGPositionParticle 대화 상자입니다.

class CDlgTGPositionParticle : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGPositionParticle)

public:
	CDlgTGPositionParticle(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGPositionParticle();

    void RectRevision(CRect &rtArea);
    void SetArea(CRect rtArea);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_POSITION_PARTICLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_ebLeft;
    CEdit m_ebRight;
    CEdit m_ebTop;
    CEdit m_ebBottom;
    CACEdit m_cbParticleID;

    CPositionParticle m_TGPositionParticle;
    void InitValue(void) { m_TGPositionParticle.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CPositionParticle*)pValue = m_TGPositionParticle; }
    void SetValue(CBaseTGFunction *pValue) { m_TGPositionParticle = *(CPositionParticle*)pValue; }

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stPositionParticle1;
    CStatic m_stPositionParticle2;
    afx_msg void OnBnClickedButtonSelectFromMapSelArea();
};
