#pragma once
#include "DlgTGBase.h"
#include "stdafx.h"
#include "ACEdit.h"
#include "PlayerMotion.h"

// CDlgTGPlayerMotion 대화 상자입니다.

class CDlgTGPlayerMotion : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGPlayerMotion)

public:
	CDlgTGPlayerMotion(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGPlayerMotion();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_PLAYER_MOTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox   m_cbSlot;
    CACEdit     m_cbMotion;
    CStatic     m_stPlayerMotion1;
    CStatic     m_stPlayerMotion2;

    CPlayerMotion m_TGPlayerMotion;
    void InitValue(void) { m_TGPlayerMotion.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CPlayerMotion*)pValue = m_TGPlayerMotion; }
    void SetValue(CBaseTGFunction *pValue) { m_TGPlayerMotion = *(CPlayerMotion*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
