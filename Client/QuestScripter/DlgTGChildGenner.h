#pragma once
#include "stdafx.h"
#include "BaseTrigger.h"
//#include <map>

// CDlgTGChildGenner 대화 상자입니다.

//************************************************************************
// 이벤트, 액션
//************************************************************************
class CDlgTGBase;
class CDlgTGWin;
class CDlgTGLose;
class CDlgTGMonsterGenerater;
class CDlgTGSoundOnOff;
class CDlgTGLoading;
class CDlgTriggerOnOff;
class CDlgTGReward;
class CDlgTGNextStage;
class CDlgTGSuperPlayer;
class CDlgTGClearOfStage;
class CDlgTGPlayerPosition;
class CDlgTGCommon;
class CDlgTGObjectGenerater;
class CDlgTGOpenDGate;
class CDlgTGOpenGateRight;
class CDlgTGOpenGateTop;
class CDlgTGOpenGateBottom;
class CDlgTGTitleText;
class CDlgTriggerReady;
class CDlgTGDialogue;
class CDlgTGDeleteMonster;
class CDlgTGFadeInOut;
class CDlgTGHp;
class CDlgTGMP;
class CDlgTGPlayerMotion;
class CDlgTGPositionParticle;
class CDlgTGUnitMovement;
class CDlgTGUnitParticle;
class CDlgTGPlayerMovingToArea;
class CDlgTGChangeObjState;
class CDlgTGGateObjectGenerater;
class CDlgTGAddDamage;
class CDlgTGWater;
class CDlgTGWeather;
class CDlgTGEmbark;
class CDlgTGWall;
class CDlgTGSubject;
class CDlgTGInfinityMonsterGen;

//************************************************************************
// 조건
//************************************************************************
class CDlgTGKillMaster;
class CDlgTGTimer;
class CDlgTGPosition;
class CDlgTGObjDestroy;
class CDialogueState;
class CDlgTGCheckHP;
class CDlgTGCheckWall;
class CDlgTGKillAllMonster;

class CDlgTGChildGenner : public CDialog
{
	DECLARE_DYNAMIC(CDlgTGChildGenner)

public:

public:
	CDlgTGChildGenner(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CDlgTGChildGenner(TG_TYPE enType, TG_EACTION enAction, TG_CONDITION enCondition, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGChildGenner();

	void SetType(TG_TYPE enType, TG_EACTION enAction, TG_CONDITION enCondition);

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_CHILD_GENNER };

	enum GENNER_MODE { CREATE_MODE, MODIFY_MODE };
	GENNER_MODE m_Mode;
	std::map<CString, TG_EACTION>    m_mapEAction;

	//************************************************************************
	// 이벤트, 액션
	//************************************************************************
	CDlgTGWin                   *m_pdlgTGWin;
	CDlgTGLose                  *m_pdlgTGLose;
	CDlgTGMonsterGenerater      *m_pdlgTGMonGenerater;
	CDlgTGSoundOnOff            *m_pdlgTGSoundOnOff;
	CDlgTGLoading               *m_pdlgTGLoading;
	CDlgTriggerOnOff            *m_pdlgTGTriggerOnOff;
	CDlgTGReward                *m_pdlgTGReward;
	CDlgTGNextStage             *m_pdlgTGNextStage;
	CDlgTGSuperPlayer           *m_pdlgTGSuperPlayer;
	CDlgTGClearOfStage          *m_pdlgTGClearOfStage;
	CDlgTGPlayerPosition        *m_pdlgTGPlayerPosition;
	CDlgTGCommon                *m_pdlgTGCommon;
	CDlgTGObjectGenerater       *m_pdlgTGObjectGenerater;
	CDlgTGOpenDGate             *m_pdlgTGOpenDGate;
	CDlgTGTitleText             *m_pdlgTGTitleText;
	CDlgTriggerReady            *m_pdlgTGTriggerReady;
	CDlgTGDialogue              *m_pdlgTGDialogue;
	CDlgTGDeleteMonster         *m_pdlgTGDeleteMonster;
	CDlgTGFadeInOut             *m_pdlgTGFadeInOut;
	CDlgTGHp                    *m_pdlgTGHp;
	CDlgTGMP                    *m_pdlgTGMp;
	CDlgTGPlayerMotion          *m_pdlgTGPlayerMotion;
	CDlgTGPositionParticle      *m_pdlgTGPositionParticle;
	CDlgTGUnitMovement          *m_pdlgTGUnitMovement;
	CDlgTGUnitParticle          *m_pdlgTGUnitParticle;
	CDlgTGPlayerMovingToArea    *m_pdlgTGPlayerMovingToArea;
	CDlgTGChangeObjState        *m_pdlgTGChangeObjState;
	CDlgTGGateObjectGenerater   *m_TGGateObjectGenerater;
	CDlgTGAddDamage             *m_TGAddDamage;
	CDlgTGWater                 *m_pdlgTGWater;
	CDlgTGWeather               *m_pdlgTGWeather;
	CDlgTGEmbark                *m_pdlgTGEmbark;
	CDlgTGWall                  *m_pdlgTGWall;
	CDlgTGSubject               *m_pdlgTGSubject;
	CDlgTGInfinityMonsterGen    *m_pdlgTGInfinityMonsterGen;

	std::map<TG_EACTION, CDlgTGBase**> m_mapDlgEAction;

	//************************************************************************
	// 조건
	//************************************************************************
	CDlgTGKillMaster            *m_pdlgTGKillMaster;
	CDlgTGTimer                 *m_pdlgTGTimer;
	CDlgTGPosition              *m_pdlgTGPosition;
	CDlgTGObjDestroy            *m_pdlgTGObjDestroy;
	CDlgTGDialogue              *m_pdlgTGDialogueState;
	CDlgTGCheckHP				*m_pdlgTGCheckHP;
	CDlgTGCheckWall				*m_pdlgTGCheckWall;
	CDlgTGKillAllMonster        *m_pdlgTGKillAllMonster;
	std::map<TG_CONDITION, CDlgTGBase**> m_mapDlgCondition;

	// Initial Value
	std::map<CString, TG_CONDITION>  m_mapCondition;

	TG_TYPE                     m_enType;
	TG_EACTION                  m_enAction;
	TG_CONDITION                m_enCondition;
	int                         m_iFunctionType;        // 0 : Event, 1 : condition, 2 : Action
	CListBox                    m_lbTypeList;
	CStatic                     m_stMain;

	void Init(void);
	void InitMap(void);
	void InitListBox(void);
	void SetType(TG_TYPE enType);
	void SetEAction(TG_EACTION enAction);
	void SetCondition(TG_CONDITION enCondition);

	void AllDialogInitValue(void);
	void AllDialogRefresh(void);
	void AllDialogHide(void);
	void RefreshDialog(void);

	void SetValue(CBaseEventAction *pEAction);
	void SetValue(CBaseCondition *pCondition);
	void GetValue(CBaseEventAction *pEAction);
	void GetValue(CBaseCondition *pCondition);

	void SetCurWindowToPos(POINT ptPos);

	CBaseCondition *GetCurCondition(void);
	CBaseEventAction *GetCurEAction(void);

	void GetAllocTGFunction(CBaseTGFunction **pBaseTGFuntion, int iTGFType, int iType);
	void GetAllocTGDialog(CDlgTGBase **pDlgTGBase, CWnd *pParent, int iTGFType, int iType);

	//************************************************************************
	// 각각의 다이얼로그에 개별적으로 적용시켜줘야 할 함수들은 여기서
	//************************************************************************
	void SetTGInfo(void);
	void SetMonsterInfo(bool bPlayer, int iSlot, int iType);
	void SetArea(CRect rtArea);

	void InitialWindowVariable(void);
	void SendCurrentEActionToDlgTGContent(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnSelchangeListType();
	virtual BOOL OnInitDialog();
	BOOL ShowWindow(int nCmdShow);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
