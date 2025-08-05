#pragma once

#include "stdafx.h"
#include "DlgMapToObject.h"
#include "DlgTriggerContent.h"
#include "ObjectDataManager.h"
#include "QuestScripterDlg.h"
#include "MonsterGenerater.h"
#include "resource.h"

//#include <vector>

// CDlgStageMonster_Popup 대화 상자입니다.

class CDlgStageMonster_Popup : public CDialog
{
	DECLARE_DYNAMIC(CDlgStageMonster_Popup)

public:
	CDlgStageMonster_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgStageMonster_Popup();

    bool m_bFnAction;
    CMonsterGenerater monstergen;

    void SetMapObjectList(std::vector<CMonsterGenerater*> *pvtMonList);
    std::vector<CMonsterGenerater*> *m_pvtMonList;

    // 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STAGE_MONSTER_POPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_cbBoss;
    CComboBox m_cbID;
    CEdit m_edEntranceX;
    CEdit m_edEntranceY;
    CEdit m_edMonLv;
    CComboBox m_cbRight;
    afx_msg void OnBnClickedDelete();
    CComboBox m_cbFunctionType;
};

