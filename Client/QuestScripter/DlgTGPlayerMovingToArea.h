#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "PlayerMovingToArea.h"

// CDlgTGPlayerMovingToArea 대화 상자입니다.

class CDlgTGPlayerMovingToArea : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGPlayerMovingToArea)

public:
	CDlgTGPlayerMovingToArea(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGPlayerMovingToArea();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_PLAYER_MOVING_TO_AREA };

    void SetMonsterInfo(bool bPlayer, int iSlot, int iType);
    void SetArea(CRect rtArea);
    void RectRevision(CRect &rtArea);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_cbSlot;
    CComboBox m_cbPlayerOrMonster;
    CACEdit m_cbMonsterID;
    CEdit m_ebLeft;
    CEdit m_ebRight;
    CEdit m_ebTop;
    CEdit m_ebBottom;
    CStatic m_stPlayerMovingToArea1;
    CStatic m_stPlayerMovingToArea2;

    CPlayerMovingToArea m_TGPlayerMovingToArea;
    void InitValue(void) { m_TGPlayerMovingToArea.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CPlayerMovingToArea*)pValue = m_TGPlayerMovingToArea; }
    void SetValue(CBaseTGFunction *pValue) { m_TGPlayerMovingToArea = *(CPlayerMovingToArea*)pValue; }

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonSelectFromMapSelMon();
    afx_msg void OnBnClickedButtonSelectFromMapSelArea();
};
