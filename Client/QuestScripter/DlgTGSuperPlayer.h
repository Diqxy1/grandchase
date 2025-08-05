#pragma once
#include "DlgTGBase.h"
#include "SuperPlayer.h"
#include "stdafx.h"

// CDlgTGSuperPlayer 대화 상자입니다.

class CDlgTGSuperPlayer : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGSuperPlayer)

public:
	CDlgTGSuperPlayer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGSuperPlayer();

    CSuperPlayer m_TGSuperPlayer;
    void InitValue(void) { m_TGSuperPlayer.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CSuperPlayer*)pValue = m_TGSuperPlayer; }
    void SetValue(CBaseTGFunction *pValue) { m_TGSuperPlayer = *(CSuperPlayer*)pValue; }   

    void Refresh(void);
    void CalcResult(void);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_SUPER_PLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_cbPlayerSlot;
    CComboBox m_cbOnOff;
    CStatic m_stSuperPlayer1;
    CStatic m_stSuperPlayer2;
};
