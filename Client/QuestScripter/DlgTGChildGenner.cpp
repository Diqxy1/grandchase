// DlgTGChildGenner.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "QuestScripterDlg.h"
#include "DlgTGChildGenner.h"
#include ".\dlgtgchildgenner.h"

#include "DlgTrigger.h"
#include "DlgTriggerContent.h"

//************************************************************************
// 이벤트, 액션
//************************************************************************
#include "DlgTGBase.h"
#include "DlgTGWin.h"
#include "DlgTGLose.h"
#include "DlgTGMonsterGenerater.h"
#include "DlgTGSoundOnOff.h"
#include "DlgTGLoading.h"
#include "DlgTriggerOnOff.h"
#include "DlgTGReward.h"
#include "DlgTGNextStage.h"
#include "DlgTGSuperPlayer.h"
#include "DlgTGClearOfStage.h"
#include "DlgTGPlayerPosition.h"
#include "DlgTGCommon.h"
#include "DlgTGObjectGenerater.h"
#include "DlgTGOpendGate.h"
#include "DlgTGTitleText.h"
#include "DlgTriggerReady.h"
#include "DlgTGDialogue.h"
#include "DlgTGDeleteMonster.h"
#include "DlgTGFadeInOut.h"
#include "DlgTGHp.h"
#include "DlgTGMp.h"
#include "DlgTGPlayerMotion.h"
#include "DlgTGPositionParticle.h"
#include "DlgTGUnitMovement.h"
#include "DlgTGUnitParticle.h"
#include "DlgTGPlayerMovingToArea.h"
#include "DlgTGChangeObjState.h"
#include "DlgTGGateObjectGenerater.h"
#include "DlgTGAddDamage.h"
#include "DlgTGWater.h"
#include "DlgTGWeather.h"
#include "DlgTGEmbark.h"
#include "DlgTGWall.h"
#include "DlgTGSubject.h"
#include "DlgTGInfinityMonsterGen.h"

//************************************************************************
// 조건
//************************************************************************
#include "DlgTGKillMaster.h"
#include "DlgTGTimer.h"
#include "DlgTGPosition.h"
#include "DlgTGObjDestroy.h"
#include "DlgTGDialogueState.h"
#include "DlgTGCheckHP.h"
#include "DlgTGCheckWall.h"
#include "DlgTGKillAllMonster.h"

// CDlgTGChildGenner 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGChildGenner, CDialog)
CDlgTGChildGenner::CDlgTGChildGenner(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTGChildGenner::IDD, pParent)
{
	Init();
}

CDlgTGChildGenner::CDlgTGChildGenner(TG_TYPE enType, TG_EACTION enAction, TG_CONDITION enCondition, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTGChildGenner::IDD, pParent)
{
	Init();

	m_enType = enType;
	m_enAction = enAction;
	m_enCondition = enCondition;
}

void CDlgTGChildGenner::SetType(TG_TYPE enType, TG_EACTION enAction, TG_CONDITION enCondition)
{
	m_enType = enType;
	m_enAction = enAction;
	m_enCondition = enCondition;
}

CDlgTGChildGenner::~CDlgTGChildGenner()
{
	std::map<TG_EACTION, CDlgTGBase**>::iterator itDlgEAction = m_mapDlgEAction.begin();
	for (; itDlgEAction != m_mapDlgEAction.end(); ++itDlgEAction)
	{
		SAFE_DELETE(*itDlgEAction->second);
	}
	m_mapDlgEAction.clear();

	std::map<TG_CONDITION, CDlgTGBase**>::iterator itDlgCondition = m_mapDlgCondition.begin();
	for (; itDlgCondition != m_mapDlgCondition.end(); ++itDlgCondition)
	{
		SAFE_DELETE(*itDlgCondition->second);
	}
	m_mapDlgCondition.clear();

	if (m_pdlgTGCommon) SAFE_DELETE(m_pdlgTGCommon);
}

void CDlgTGChildGenner::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TYPE, m_lbTypeList);
	DDX_Control(pDX, IDC_STATIC_TYPE, m_stMain);
}

BEGIN_MESSAGE_MAP(CDlgTGChildGenner, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_TYPE, OnLbnSelchangeListType)
END_MESSAGE_MAP()

// CDlgTGChildGenner 메시지 처리기입니다.

void CDlgTGChildGenner::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	switch (m_enType)
	{
	case TG_TYPE_EACTION:
		if (m_mapDlgEAction.count(m_enAction))
		{
			if (m_mapDlgEAction[m_enAction])
				(*m_mapDlgEAction[m_enAction])->CalcResult();
		}
		break;

	case TG_TYPE_CONDITION:
		if (m_mapDlgCondition.count(m_enCondition))
		{
			if (m_mapDlgCondition[m_enCondition])
				(*m_mapDlgCondition[m_enCondition])->CalcResult();
		}
		break;
	}
	//OnOK();

	SendCurrentEActionToDlgTGContent();
	ShowWindow(SW_HIDE);
}

void CDlgTGChildGenner::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnCancel();
	m_iFunctionType = -1;
	ShowWindow(SW_HIDE);
}

void CDlgTGChildGenner::SetType(TG_TYPE enType)
{
	m_enType = enType;
}

void CDlgTGChildGenner::SetEAction(TG_EACTION enAction)
{
	m_enAction = enAction;
}

void CDlgTGChildGenner::SetCondition(TG_CONDITION enCondition)
{
	m_enCondition = enCondition;
}

void CDlgTGChildGenner::AllDialogHide(void)
{
	std::map<TG_EACTION, CDlgTGBase**>::iterator itDlgEAction = m_mapDlgEAction.begin();
	for (; itDlgEAction != m_mapDlgEAction.end(); ++itDlgEAction)
	{
		if (*itDlgEAction->second) (*itDlgEAction->second)->ShowWindow(SW_HIDE);
	}

	std::map<TG_CONDITION, CDlgTGBase**>::iterator itDlgCondition = m_mapDlgCondition.begin();
	for (; itDlgCondition != m_mapDlgCondition.end(); ++itDlgCondition)
	{
		if (*itDlgCondition->second) (*itDlgCondition->second)->ShowWindow(SW_HIDE);
	}

	if (m_pdlgTGCommon) m_pdlgTGCommon->ShowWindow(SW_HIDE);
}

void CDlgTGChildGenner::RefreshDialog(void)
{
	AllDialogHide();

	switch (m_enType)
	{
	case TG_TYPE_EACTION:
		if (m_mapDlgEAction.count(m_enAction))
		{
			if (m_mapDlgEAction[m_enAction])
				(*m_mapDlgEAction[m_enAction])->ShowWindow(SW_SHOW);
		}
		break;

	case TG_TYPE_CONDITION:
		if (m_mapDlgCondition.count(m_enCondition))
		{
			if (m_mapDlgCondition[m_enCondition])
				(*m_mapDlgCondition[m_enCondition])->ShowWindow(SW_SHOW);
		}
	}

	if (m_pdlgTGCommon) m_pdlgTGCommon->ShowWindow(SW_SHOW);
}

void CDlgTGChildGenner::InitMap(void)
{
	m_mapEAction.clear();
	m_mapEAction[L"승리"] = TG_EACTION_WIN;
	m_mapEAction[L"패배"] = TG_EACTION_LOSE;
	m_mapEAction[L"몬스터생성"] = TG_EACTION_MON_GENNER;
	m_mapEAction[L"배경음악설정"] = TG_EACTION_SOUND_ONOFF;
	m_mapEAction[L"로딩"] = TG_EACTION_LOADING;
	m_mapEAction[L"트리거 활성화"] = TG_EACTION_TRIGGERONOFF;
	m_mapEAction[L"보상"] = TG_EACTION_REWARD;
	m_mapEAction[L"다음 스테이지로 이동"] = TG_EACTION_NEXTSTAGE;
	m_mapEAction[L"플레이어 무적"] = TG_EACTION_SUPERPLAYER;
	m_mapEAction[L"스테이지 클리어"] = TG_EACTION_CLEAROFSTAGE;
	m_mapEAction[L"플레이어 위치 설정"] = TG_EACTION_PLAYERPOSITION;
	m_mapEAction[L"오브젝트 추가제거"] = TG_EACTION_OBJECTGENERATER;
	m_mapEAction[L"게이트를 통히여 이동"] = TG_EACTION_OPEN_DGATE;
	m_mapEAction[L"화면에 텍스트 쓰기"] = TG_EACTION_TITLETEXT;
	m_mapEAction[L"트리거 초기화"] = TG_EACTION_TRIGGERREADY;

	m_mapEAction[L"대화상자"] = TG_EACTION_DIALOGUE;
	m_mapEAction[L"몬스터 제거"] = TG_EACTION_DELETEMONSTER;
	m_mapEAction[L"페이드 인아웃"] = TG_EACTION_FADE_INOUT;
	m_mapEAction[L"HP 설정"] = TG_EACTION_HP;
	m_mapEAction[L"MP 설정"] = TG_EACTION_MP;
	m_mapEAction[L"플레이어 모션"] = TG_EACTION_PLAYERMOTION;
	//m_mapEAction["맵에 파티클 추가"]        = TG_EACTION_POSITIONPARTICLE;
	m_mapEAction[L"유닛의 행동여부"] = TG_EACTION_UNITMOVEMENT;
	//m_mapEAction["유닛에 파티클 추가"]      = TG_EACTION_UNIT_PARTICLE;
	m_mapEAction[L"위치로 플레이어이동"] = TG_EACTION_PLAYERMOVINGTOAREA;
	m_mapEAction[L"오브젝트 상태 변경"] = TG_EACTION_CHANGE_OBJ_STATE;
	m_mapEAction[L"게이트 오브젝트 추가"] = TG_EACTION_GATE_OBJECT_GEN;
	m_mapEAction[L"위치에 데미지 추가"] = TG_EACTION_ADD_DAMAGE;
	m_mapEAction[L"물 설정"] = TG_EACTION_WATER;
	m_mapEAction[L"날씨 설정"] = TG_EACTION_WEATHER;
	m_mapEAction[L"탑승물배치"] = TG_EACTION_EMBARK;
	m_mapEAction[L"벽 설치"] = TG_EACTION_WALL;
	m_mapEAction[L"달성과제 설정"] = TG_EACTION_SUBJECT;
	m_mapEAction[L"몬스터를 무한 생성"] = TG_EACTION_INFINITY_MON_GEN;

	m_mapCondition.clear();
	m_mapCondition[L"몬스터죽음"] = TG_CONDITION_KILLMASTER;
	m_mapCondition[L"타이머"] = TG_CONDITION_TIMER;
	m_mapCondition[L"특정위치"] = TG_CONDITION_POSITION;
	m_mapCondition[L"오브젝트제거여부"] = TG_CONDITION_OBJ_DESTROY;
	m_mapCondition[L"다이얼로그표시여부"] = TG_CONDITION_DIALOGUE_STATE;
	m_mapCondition[L"HP 변화"] = TG_CONDITION_HP_CHANGE;
	m_mapCondition[L"벽 있나?"] = TG_CONDITION_WALL;
	m_mapCondition[L"모든 몬스터 죽음"] = TG_CONDITION_KILLALLMONSTER;
}

void CDlgTGChildGenner::InitListBox(void)
{
	m_lbTypeList.ResetContent();
	switch (m_enType)
	{
	case TG_TYPE_EACTION:
	{
		CString strCurEAction;
		std::map<CString, TG_EACTION>::iterator itorEAction = m_mapEAction.begin();
		for (int iCnt = 0; m_mapEAction.end() != itorEAction; itorEAction++, iCnt++)
		{
			if (m_enAction == itorEAction->second) strCurEAction = itorEAction->first;
			m_lbTypeList.InsertString(iCnt, itorEAction->first);
		}

		int index = m_lbTypeList.FindStringExact(0, strCurEAction.GetBuffer());
		if (LB_ERR != index) m_lbTypeList.SetCurSel(index);
	}
	break;
	case TG_TYPE_CONDITION:
	{
		CString strCurCondition;
		std::map<CString, TG_CONDITION>::iterator itorCondition = m_mapCondition.begin();
		for (int iCnt = 0; m_mapCondition.end() != itorCondition; itorCondition++, iCnt++)
		{
			if (m_enCondition == itorCondition->second) strCurCondition = itorCondition->first;
			m_lbTypeList.InsertString(iCnt, itorCondition->first);
		}

		int index = m_lbTypeList.FindStringExact(0, strCurCondition.GetBuffer());
		if (LB_ERR != index) m_lbTypeList.SetCurSel(index);
	}
	break;
	}

	//if( m_lbTypeList.GetCount() ) m_lbTypeList.SetCurSel(0);
}
void CDlgTGChildGenner::OnLbnSelchangeListType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strText;
	m_lbTypeList.GetText(m_lbTypeList.GetCurSel(), strText);

	switch (m_enType)
	{
	case TG_TYPE_EACTION:
	{
		if (m_mapEAction.end() != m_mapEAction.find(strText))
			m_enAction = m_mapEAction[strText];
	}
	break;
	case TG_TYPE_CONDITION:
	{
		if (m_mapCondition.end() != m_mapCondition.find(strText))
			m_enCondition = m_mapCondition[strText];
	}
	break;
	}

	RefreshDialog();
}

BOOL CDlgTGChildGenner::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect rtClient;
	int iStartY = 0;

	m_pdlgTGCommon = new CDlgTGCommon;
	m_pdlgTGCommon->Create(CDlgTGCommon::IDD, &m_stMain);
	m_pdlgTGCommon->GetClientRect(rtClient);
	iStartY = rtClient.bottom;

	std::map<TG_EACTION, CDlgTGBase**>::iterator itDlgEAction = m_mapDlgEAction.begin();
	for (; itDlgEAction != m_mapDlgEAction.end(); ++itDlgEAction)
	{
		//if( *itDlgEAction->second ) SAFE_DELETE(*itDlgEAction->second);
		GetAllocTGDialog(itDlgEAction->second, &m_stMain, TG_TYPE_EACTION, itDlgEAction->first);
		//itDlgEAction->second->SetValue(&m_Win);
		(*itDlgEAction->second)->Refresh();
		(*itDlgEAction->second)->GetClientRect(rtClient);
		rtClient.OffsetRect(0, iStartY);
		(*itDlgEAction->second)->MoveWindow(rtClient, 1);
	}

	std::map<TG_CONDITION, CDlgTGBase**>::iterator itDlgCondition = m_mapDlgCondition.begin();
	for (; itDlgCondition != m_mapDlgCondition.end(); ++itDlgCondition)
	{
		//if( *itDlgCondition->second ) SAFE_DELETE(itDlgCondition->second);
		GetAllocTGDialog(itDlgCondition->second, &m_stMain, TG_TYPE_CONDITION, itDlgCondition->first);
		(*itDlgCondition->second)->Refresh();
		(*itDlgCondition->second)->GetClientRect(rtClient);
		rtClient.OffsetRect(0, iStartY);
		(*itDlgCondition->second)->MoveWindow(rtClient, 1);
	}

	InitListBox();
	RefreshDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTGChildGenner::SetValue(CBaseEventAction *pEAction)
{
	if (!pEAction) return;

	if (m_mapDlgEAction.count(pEAction->m_enType))
	{
		if (m_mapDlgEAction[pEAction->m_enType])
		{
			(*m_mapDlgEAction[pEAction->m_enType])->SetValue(pEAction);
			if (m_pdlgTGCommon) m_pdlgTGCommon->SetValue(pEAction);
		}
	}
}

void CDlgTGChildGenner::SetValue(CBaseCondition *pCondition)
{
	if (!pCondition) return;

	if (m_mapDlgCondition.count(pCondition->m_enType))
	{
		if (m_mapDlgCondition[pCondition->m_enType])
		{
			(*m_mapDlgCondition[pCondition->m_enType])->SetValue(pCondition);
			if (m_pdlgTGCommon) m_pdlgTGCommon->SetValue(pCondition);
		}
	}
}

void CDlgTGChildGenner::GetValue(CBaseEventAction *pEAction)
{
	if (!pEAction) return;

	if (m_mapDlgEAction.count(pEAction->m_enType))
	{
		if (m_mapDlgEAction[pEAction->m_enType])
		{
			(*m_mapDlgEAction[pEAction->m_enType])->GetValue(pEAction);
			if (m_pdlgTGCommon) m_pdlgTGCommon->GetValue(pEAction);
		}
	}
}

void CDlgTGChildGenner::GetValue(CBaseCondition *pCondition)
{
	if (!pCondition) return;

	if (m_mapDlgCondition.count(pCondition->m_enType))
	{
		if (m_mapDlgCondition[pCondition->m_enType])
			(*m_mapDlgCondition[pCondition->m_enType])->GetValue(pCondition);
	}

	if (m_pdlgTGCommon) m_pdlgTGCommon->GetValue(pCondition);
}

CBaseCondition *CDlgTGChildGenner::GetCurCondition(void)
{
	CBaseCondition *pCondition = NULL;

	if (m_mapDlgCondition.count(m_enCondition))
	{
		if (m_mapDlgCondition[m_enCondition])
		{
			GetAllocTGFunction((CBaseTGFunction**)&pCondition, TG_TYPE_CONDITION, (int)m_enCondition);
			(*m_mapDlgCondition[m_enCondition])->GetValue(pCondition);
			if (m_pdlgTGCommon) m_pdlgTGCommon->GetValue(pCondition);
		}
	}

	return pCondition;
}

CBaseEventAction *CDlgTGChildGenner::GetCurEAction(void)
{
	CBaseEventAction *pEAction = NULL;

	if (m_mapDlgEAction.count(m_enAction))
	{
		if (m_mapDlgEAction[m_enAction])
		{
			GetAllocTGFunction((CBaseTGFunction**)&pEAction, TG_TYPE_EACTION, (int)m_enAction);
			(*m_mapDlgEAction[m_enAction])->GetValue(pEAction);
			if (m_pdlgTGCommon) m_pdlgTGCommon->GetValue(pEAction);
		}
	}

	return pEAction;
}

void CDlgTGChildGenner::SetArea(CRect rtArea)
{
	if (m_pdlgTGPosition->IsWindowVisible()) m_pdlgTGPosition->SetArea(rtArea);
	if (m_pdlgTGPlayerMovingToArea->IsWindowVisible()) m_pdlgTGPlayerMovingToArea->SetArea(rtArea);
	if (m_pdlgTGPositionParticle->IsWindowVisible()) m_pdlgTGPositionParticle->SetArea(rtArea);
}

void CDlgTGChildGenner::SetMonsterInfo(bool bPlayer, int iSlot, int iType)
{
	if (m_pdlgTGPosition->IsWindowVisible()) m_pdlgTGPosition->SetMonsterInfo(0, iSlot, iType);
	if (m_pdlgTGHp->IsWindowVisible()) m_pdlgTGHp->SetMonsterInfo(0, iSlot, iType);
	if (m_pdlgTGMp->IsWindowVisible()) m_pdlgTGMp->SetMonsterInfo(0, iSlot, iType);
	if (m_pdlgTGUnitMovement->IsWindowVisible()) m_pdlgTGUnitMovement->SetMonsterInfo(0, iSlot, iType);
	if (m_pdlgTGUnitParticle->IsWindowVisible()) m_pdlgTGUnitParticle->SetMonsterInfo(0, iSlot, iType);
	if (m_pdlgTGPlayerMovingToArea->IsWindowVisible()) m_pdlgTGPlayerMovingToArea->SetMonsterInfo(0, iSlot, iType);
}

void CDlgTGChildGenner::SetTGInfo(void)
{
	if (m_pdlgTGTriggerOnOff) m_pdlgTGTriggerOnOff->InitTGName();
	if (m_pdlgTGTriggerReady) m_pdlgTGTriggerReady->InitTGName();
}

void CDlgTGChildGenner::Init(void)
{
	m_pdlgTGWin = NULL;
	m_pdlgTGLose = NULL;
	m_pdlgTGMonGenerater = NULL;
	m_pdlgTGSoundOnOff = NULL;
	m_pdlgTGLoading = NULL;
	m_pdlgTGKillMaster = NULL;
	m_pdlgTGTimer = NULL;
	m_pdlgTGPosition = NULL;
	m_pdlgTGDialogueState = NULL;
	m_pdlgTGTriggerOnOff = NULL;
	m_pdlgTGReward = NULL;
	m_pdlgTGNextStage = NULL;
	m_pdlgTGSuperPlayer = NULL;
	m_pdlgTGClearOfStage = NULL;
	m_pdlgTGPlayerPosition = NULL;
	m_pdlgTGCommon = NULL;
	m_pdlgTGObjectGenerater = NULL;
	m_pdlgTGOpenDGate = NULL;
	m_pdlgTGTitleText = NULL;
	m_pdlgTGTriggerReady = NULL;
	m_pdlgTGDialogue = NULL;
	m_pdlgTGDeleteMonster = NULL;
	m_pdlgTGFadeInOut = NULL;
	m_pdlgTGHp = NULL;
	m_pdlgTGMp = NULL;
	m_pdlgTGPlayerMotion = NULL;
	m_pdlgTGPositionParticle = NULL;
	m_pdlgTGUnitMovement = NULL;
	m_pdlgTGUnitParticle = NULL;
	m_pdlgTGPlayerMovingToArea = NULL;
	m_pdlgTGObjDestroy = NULL;
	m_pdlgTGChangeObjState = NULL;
	m_TGGateObjectGenerater = NULL;
	m_TGAddDamage = NULL;
	m_pdlgTGWater = NULL;
	m_pdlgTGWeather = NULL;
	m_pdlgTGEmbark = NULL;
	m_pdlgTGWall = NULL;
	m_pdlgTGInfinityMonsterGen = NULL;

	m_mapDlgEAction.clear();
	m_mapDlgEAction[TG_EACTION_POSITIONPARTICLE] = (CDlgTGBase**)&m_pdlgTGPositionParticle;
	m_mapDlgEAction[TG_EACTION_WIN] = (CDlgTGBase**)&m_pdlgTGWin;
	m_mapDlgEAction[TG_EACTION_LOSE] = (CDlgTGBase**)&m_pdlgTGLose;
	m_mapDlgEAction[TG_EACTION_MON_GENNER] = (CDlgTGBase**)&m_pdlgTGMonGenerater;
	m_mapDlgEAction[TG_EACTION_UNITMOVEMENT] = (CDlgTGBase**)&m_pdlgTGUnitMovement;
	m_mapDlgEAction[TG_EACTION_SOUND_ONOFF] = (CDlgTGBase**)&m_pdlgTGSoundOnOff;
	m_mapDlgEAction[TG_EACTION_LOADING] = (CDlgTGBase**)&m_pdlgTGLoading;
	m_mapDlgEAction[TG_EACTION_CLEAROFSTAGE] = (CDlgTGBase**)&m_pdlgTGClearOfStage;
	m_mapDlgEAction[TG_EACTION_TRIGGERONOFF] = (CDlgTGBase**)&m_pdlgTGTriggerOnOff;
	m_mapDlgEAction[TG_EACTION_REWARD] = (CDlgTGBase**)&m_pdlgTGReward;
	m_mapDlgEAction[TG_EACTION_NEXTSTAGE] = (CDlgTGBase**)&m_pdlgTGNextStage;
	m_mapDlgEAction[TG_EACTION_SUPERPLAYER] = (CDlgTGBase**)&m_pdlgTGSuperPlayer;
	m_mapDlgEAction[TG_EACTION_PLAYERPOSITION] = (CDlgTGBase**)&m_pdlgTGPlayerPosition;
	m_mapDlgEAction[TG_EACTION_OBJECTGENERATER] = (CDlgTGBase**)&m_pdlgTGObjectGenerater;
	m_mapDlgEAction[TG_EACTION_OPEN_DGATE] = (CDlgTGBase**)&m_pdlgTGOpenDGate;
	m_mapDlgEAction[TG_EACTION_TITLETEXT] = (CDlgTGBase**)&m_pdlgTGTitleText;
	m_mapDlgEAction[TG_EACTION_TRIGGERREADY] = (CDlgTGBase**)&m_pdlgTGTriggerReady;
	m_mapDlgEAction[TG_EACTION_DIALOGUE] = (CDlgTGBase**)&m_pdlgTGDialogue;
	m_mapDlgEAction[TG_EACTION_DELETEMONSTER] = (CDlgTGBase**)&m_pdlgTGDeleteMonster;
	m_mapDlgEAction[TG_EACTION_FADE_INOUT] = (CDlgTGBase**)&m_pdlgTGFadeInOut;
	m_mapDlgEAction[TG_EACTION_HP] = (CDlgTGBase**)&m_pdlgTGHp;
	m_mapDlgEAction[TG_EACTION_MP] = (CDlgTGBase**)&m_pdlgTGMp;
	m_mapDlgEAction[TG_EACTION_PLAYERMOTION] = (CDlgTGBase**)&m_pdlgTGPlayerMotion;
	m_mapDlgEAction[TG_EACTION_UNIT_PARTICLE] = (CDlgTGBase**)&m_pdlgTGUnitParticle;
	m_mapDlgEAction[TG_EACTION_PLAYERMOVINGTOAREA] = (CDlgTGBase**)&m_pdlgTGPlayerMovingToArea;
	m_mapDlgEAction[TG_EACTION_CHANGE_OBJ_STATE] = (CDlgTGBase**)&m_pdlgTGChangeObjState;
	m_mapDlgEAction[TG_EACTION_GATE_OBJECT_GEN] = (CDlgTGBase**)&m_TGGateObjectGenerater;
	m_mapDlgEAction[TG_EACTION_ADD_DAMAGE] = (CDlgTGBase**)&m_TGAddDamage;
	m_mapDlgEAction[TG_EACTION_WATER] = (CDlgTGBase**)&m_pdlgTGWater;
	m_mapDlgEAction[TG_EACTION_WEATHER] = (CDlgTGBase**)&m_pdlgTGWeather;
	m_mapDlgEAction[TG_EACTION_EMBARK] = (CDlgTGBase**)&m_pdlgTGEmbark;
	m_mapDlgEAction[TG_EACTION_WALL] = (CDlgTGBase**)&m_pdlgTGWall;
	m_mapDlgEAction[TG_EACTION_SUBJECT] = (CDlgTGBase**)&m_pdlgTGSubject;
	m_mapDlgEAction[TG_EACTION_INFINITY_MON_GEN] = (CDlgTGBase**)&m_pdlgTGInfinityMonsterGen;

	m_mapDlgCondition.clear();
	m_mapDlgCondition[TG_CONDITION_KILLMASTER] = (CDlgTGBase**)&m_pdlgTGKillMaster;
	m_mapDlgCondition[TG_CONDITION_TIMER] = (CDlgTGBase**)&m_pdlgTGTimer;
	m_mapDlgCondition[TG_CONDITION_POSITION] = (CDlgTGBase**)&m_pdlgTGPosition;
	m_mapDlgCondition[TG_CONDITION_OBJ_DESTROY] = (CDlgTGBase**)&m_pdlgTGObjDestroy;
	m_mapDlgCondition[TG_CONDITION_DIALOGUE_STATE] = (CDlgTGBase**)&m_pdlgTGDialogueState;
	m_mapDlgCondition[TG_CONDITION_HP_CHANGE] = (CDlgTGBase**)&m_pdlgTGCheckHP;
	m_mapDlgCondition[TG_CONDITION_WALL] = (CDlgTGBase**)&m_pdlgTGCheckWall;
	m_mapDlgCondition[TG_CONDITION_KILLALLMONSTER] = (CDlgTGBase**)&m_pdlgTGKillAllMonster;

	m_Mode = CREATE_MODE;

	InitMap();
}

BOOL CDlgTGChildGenner::ShowWindow(int nCmdShow)
{
	bool bShow = CDialog::ShowWindow(nCmdShow);

	if (AfxGetMainWnd())
		AfxGetMainWnd()->PostMessage(WM_FOCUS_WINDOW, 0, 0);

	if (SW_HIDE != nCmdShow)
		InitialWindowVariable();

	return bShow;
}

void CDlgTGChildGenner::AllDialogInitValue(void)
{
	std::map<TG_EACTION, CDlgTGBase**>::iterator itDlgEAction = m_mapDlgEAction.begin();
	for (; itDlgEAction != m_mapDlgEAction.end(); ++itDlgEAction)
	{
		if (*itDlgEAction->second) (*itDlgEAction->second)->InitValue();
	}

	std::map<TG_CONDITION, CDlgTGBase**>::iterator itDlgCondition = m_mapDlgCondition.begin();
	for (; itDlgCondition != m_mapDlgCondition.end(); ++itDlgCondition)
	{
		if (*itDlgCondition->second) (*itDlgCondition->second)->InitValue();
	}
	if (m_pdlgTGCommon) m_pdlgTGCommon->InitValue();
}

void CDlgTGChildGenner::AllDialogRefresh(void)
{
	std::map<TG_EACTION, CDlgTGBase**>::iterator itDlgEAction = m_mapDlgEAction.begin();
	for (; itDlgEAction != m_mapDlgEAction.end(); ++itDlgEAction)
	{
		if (*itDlgEAction->second) (*itDlgEAction->second)->Refresh();
	}

	std::map<TG_CONDITION, CDlgTGBase**>::iterator itDlgCondition = m_mapDlgCondition.begin();
	for (; itDlgCondition != m_mapDlgCondition.end(); ++itDlgCondition)
	{
		if (*itDlgCondition->second) (*itDlgCondition->second)->Refresh();
	}

	m_pdlgTGCommon->m_bCheck = false;
	m_pdlgTGCommon->Refresh();
}

void CDlgTGChildGenner::GetAllocTGFunction(CBaseTGFunction **pBaseTGFuntion, int iTGFType, int iType)
{
	switch (iTGFType)
	{
	case TG_TYPE_EACTION:
	{
		switch (iType)
		{
		case TG_EACTION_WIN:
			(*pBaseTGFuntion) = new CWin;
			break;
		case TG_EACTION_LOSE:
			(*pBaseTGFuntion) = new CLose;
			break;
		case TG_EACTION_MON_GENNER:
			(*pBaseTGFuntion) = new CMonsterGenerater;
			break;
		case TG_EACTION_SOUND_ONOFF:
			(*pBaseTGFuntion) = new CSoundOnOff;
			break;
		case TG_EACTION_LOADING:
			(*pBaseTGFuntion) = new CLoading;
			break;
		case TG_EACTION_TRIGGERONOFF:
			(*pBaseTGFuntion) = new CTriggerOnOff;
			break;
		case TG_EACTION_REWARD:
			(*pBaseTGFuntion) = new CReward;
			break;
		case TG_EACTION_NEXTSTAGE:
			(*pBaseTGFuntion) = new CNextStage;
			break;
		case TG_EACTION_SUPERPLAYER:
			(*pBaseTGFuntion) = new CSuperPlayer;
			break;
		case TG_EACTION_CLEAROFSTAGE:
			(*pBaseTGFuntion) = new CClearOfStage;
			break;
		case TG_EACTION_PLAYERPOSITION:
			(*pBaseTGFuntion) = new CPlayerPosition;
			break;
		case TG_EACTION_OBJECTGENERATER:
			(*pBaseTGFuntion) = new CObjectGenerater;
			break;
		case TG_EACTION_OPEN_DGATE:
			(*pBaseTGFuntion) = new COpenDGate;
			break;
		case TG_EACTION_TITLETEXT:
			(*pBaseTGFuntion) = new CTitleText;
			break;
		case TG_EACTION_TRIGGERREADY:
			(*pBaseTGFuntion) = new CTriggerReady;
			break;
		case TG_EACTION_DIALOGUE:
			(*pBaseTGFuntion) = new CDialogue;
			break;
		case TG_EACTION_DELETEMONSTER:
			(*pBaseTGFuntion) = new CDeleteMonster;
			break;
		case TG_EACTION_FADE_INOUT:
			(*pBaseTGFuntion) = new CFadeInOut;
			break;
		case TG_EACTION_HP:
			(*pBaseTGFuntion) = new CHP;
			break;
		case TG_EACTION_MP:
			(*pBaseTGFuntion) = new CMP;
			break;
		case TG_EACTION_PLAYERMOTION:
			(*pBaseTGFuntion) = new CPlayerMotion;
			break;
		case TG_EACTION_POSITIONPARTICLE:
			(*pBaseTGFuntion) = new CPositionParticle;
			break;
		case TG_EACTION_UNITMOVEMENT:
			(*pBaseTGFuntion) = new CUnitMovement;
			break;
		case TG_EACTION_UNIT_PARTICLE:
			(*pBaseTGFuntion) = new CUnitParticle;
			break;
		case TG_EACTION_PLAYERMOVINGTOAREA:
			(*pBaseTGFuntion) = new CPlayerMovingToArea;
			break;
		case TG_EACTION_CHANGE_OBJ_STATE:
			(*pBaseTGFuntion) = new CChangeObjState;
			break;
		case TG_EACTION_GATE_OBJECT_GEN:
			(*pBaseTGFuntion) = new CGateObjectGenerater;
			break;
		case TG_EACTION_ADD_DAMAGE:
			(*pBaseTGFuntion) = new CAddDamage;
			break;
		case TG_EACTION_WATER:
			(*pBaseTGFuntion) = new CWater;
			break;
		case TG_EACTION_WEATHER:
			(*pBaseTGFuntion) = new CWeather;
			break;
		case TG_EACTION_EMBARK:
			(*pBaseTGFuntion) = new CEmbark;
			break;
		case TG_EACTION_WALL:
			(*pBaseTGFuntion) = new CWall;
			break;
		case TG_EACTION_SUBJECT:
			(*pBaseTGFuntion) = new CSubject;
		case TG_EACTION_INFINITY_MON_GEN:
			(*pBaseTGFuntion) = new CInfinityMonsterGen;
			break;
		}
	}
	break;
	case TG_TYPE_CONDITION:
		switch (iType)
		{
		case TG_CONDITION_KILLMASTER:
			(*pBaseTGFuntion) = new CKillMaster;
			break;
		case TG_CONDITION_TIMER:
			(*pBaseTGFuntion) = new CTimer;
			break;
		case TG_CONDITION_POSITION:
			(*pBaseTGFuntion) = new CPosition;
			break;
		case TG_CONDITION_OBJ_DESTROY:
			(*pBaseTGFuntion) = new CObjDestory;
			break;
		case TG_CONDITION_DIALOGUE_STATE:
			(*pBaseTGFuntion) = new CDialogueState;
			break;
		case TG_CONDITION_HP_CHANGE:
			(*pBaseTGFuntion) = new CCheckHP;
			break;
		case TG_CONDITION_WALL:
			(*pBaseTGFuntion) = new CCheckWall;
			break;
		case TG_CONDITION_KILLALLMONSTER:
			(*pBaseTGFuntion) = new CKillAllMonster;
			break;
		}
		break;
	}
}

void CDlgTGChildGenner::GetAllocTGDialog(CDlgTGBase **pDlgTGBase, CWnd *pParent, int iTGFType, int iType)
{
	switch (iTGFType)
	{
	case TG_TYPE_EACTION:
	{
		switch (iType)
		{
		case TG_EACTION_WIN:
			*pDlgTGBase = new CDlgTGWin;
			((CDlgTGWin*)*pDlgTGBase)->Create(CDlgTGWin::IDD, pParent);
			break;
		case TG_EACTION_LOSE:
			(*pDlgTGBase) = new CDlgTGLose;
			(*pDlgTGBase)->Create(CDlgTGLose::IDD, pParent);
			break;
		case TG_EACTION_MON_GENNER:
			(*pDlgTGBase) = new CDlgTGMonsterGenerater;
			(*pDlgTGBase)->Create(CDlgTGMonsterGenerater::IDD, pParent);
			break;
		case TG_EACTION_SOUND_ONOFF:
			(*pDlgTGBase) = new CDlgTGSoundOnOff;
			(*pDlgTGBase)->Create(CDlgTGSoundOnOff::IDD, pParent);
			break;
		case TG_EACTION_LOADING:
			(*pDlgTGBase) = new CDlgTGLoading;
			(*pDlgTGBase)->Create(CDlgTGLoading::IDD, pParent);
			break;
		case TG_EACTION_TRIGGERONOFF:
			(*pDlgTGBase) = new CDlgTriggerOnOff;
			(*pDlgTGBase)->Create(CDlgTriggerOnOff::IDD, pParent);
			break;
		case TG_EACTION_REWARD:
			(*pDlgTGBase) = new CDlgTGReward;
			(*pDlgTGBase)->Create(CDlgTGReward::IDD, pParent);
			break;
		case TG_EACTION_NEXTSTAGE:
			(*pDlgTGBase) = new CDlgTGNextStage;
			(*pDlgTGBase)->Create(CDlgTGNextStage::IDD, pParent);
			break;
		case TG_EACTION_SUPERPLAYER:
			(*pDlgTGBase) = new CDlgTGSuperPlayer;
			(*pDlgTGBase)->Create(CDlgTGSuperPlayer::IDD, pParent);
			break;
		case TG_EACTION_CLEAROFSTAGE:
			(*pDlgTGBase) = new CDlgTGClearOfStage;
			(*pDlgTGBase)->Create(CDlgTGClearOfStage::IDD, pParent);
			break;
		case TG_EACTION_PLAYERPOSITION:
			(*pDlgTGBase) = new CDlgTGPlayerPosition;
			(*pDlgTGBase)->Create(CDlgTGPlayerPosition::IDD, pParent);
			break;
		case TG_EACTION_OBJECTGENERATER:
			(*pDlgTGBase) = new CDlgTGObjectGenerater;
			(*pDlgTGBase)->Create(CDlgTGObjectGenerater::IDD, pParent);
			break;
		case TG_EACTION_OPEN_DGATE:
			(*pDlgTGBase) = new CDlgTGOpenDGate;
			(*pDlgTGBase)->Create(CDlgTGOpenDGate::IDD, pParent);
			break;
		case TG_EACTION_TITLETEXT:
			(*pDlgTGBase) = new CDlgTGTitleText;
			(*pDlgTGBase)->Create(CDlgTGTitleText::IDD, pParent);
			break;
		case TG_EACTION_TRIGGERREADY:
			(*pDlgTGBase) = new CDlgTriggerReady;
			(*pDlgTGBase)->Create(CDlgTriggerReady::IDD, pParent);
			break;
		case TG_EACTION_DIALOGUE:
			(*pDlgTGBase) = new CDlgTGDialogue;
			(*pDlgTGBase)->Create(CDlgTGDialogue::IDD, pParent);
			break;
		case TG_EACTION_DELETEMONSTER:
			(*pDlgTGBase) = new CDlgTGDeleteMonster;
			(*pDlgTGBase)->Create(CDlgTGDeleteMonster::IDD, pParent);
			break;
		case TG_EACTION_FADE_INOUT:
			(*pDlgTGBase) = new CDlgTGFadeInOut;
			(*pDlgTGBase)->Create(CDlgTGFadeInOut::IDD, pParent);
			break;
		case TG_EACTION_HP:
			(*pDlgTGBase) = new CDlgTGHp;
			(*pDlgTGBase)->Create(CDlgTGHp::IDD, pParent);
			break;
		case TG_EACTION_MP:
			(*pDlgTGBase) = new CDlgTGMP;
			(*pDlgTGBase)->Create(CDlgTGMP::IDD, pParent);
			break;
		case TG_EACTION_PLAYERMOTION:
			(*pDlgTGBase) = new CDlgTGPlayerMotion;
			(*pDlgTGBase)->Create(CDlgTGPlayerMotion::IDD, pParent);
			break;
		case TG_EACTION_POSITIONPARTICLE:
			(*pDlgTGBase) = new CDlgTGPositionParticle;
			(*pDlgTGBase)->Create(CDlgTGPositionParticle::IDD, pParent);
			break;
		case TG_EACTION_UNITMOVEMENT:
			(*pDlgTGBase) = new CDlgTGUnitMovement;
			(*pDlgTGBase)->Create(CDlgTGUnitMovement::IDD, pParent);
			break;
		case TG_EACTION_UNIT_PARTICLE:
			(*pDlgTGBase) = new CDlgTGUnitParticle;
			(*pDlgTGBase)->Create(CDlgTGUnitParticle::IDD, pParent);
			break;
		case TG_EACTION_PLAYERMOVINGTOAREA:
			(*pDlgTGBase) = new CDlgTGPlayerMovingToArea;
			(*pDlgTGBase)->Create(CDlgTGPlayerMovingToArea::IDD, pParent);
			break;
		case TG_EACTION_CHANGE_OBJ_STATE:
			(*pDlgTGBase) = new CDlgTGChangeObjState;
			(*pDlgTGBase)->Create(CDlgTGChangeObjState::IDD, pParent);
			break;
		case TG_EACTION_GATE_OBJECT_GEN:
			(*pDlgTGBase) = new CDlgTGGateObjectGenerater;
			(*pDlgTGBase)->Create(CDlgTGGateObjectGenerater::IDD, pParent);
			break;
		case TG_EACTION_ADD_DAMAGE:
			(*pDlgTGBase) = new CDlgTGAddDamage;
			(*pDlgTGBase)->Create(CDlgTGAddDamage::IDD, pParent);
			break;
		case TG_EACTION_WATER:
			(*pDlgTGBase) = new CDlgTGWater;
			(*pDlgTGBase)->Create(CDlgTGWater::IDD, pParent);
			break;
		case TG_EACTION_WEATHER:
			(*pDlgTGBase) = new CDlgTGWeather;
			(*pDlgTGBase)->Create(CDlgTGWeather::IDD, pParent);
			break;
		case TG_EACTION_EMBARK:
			(*pDlgTGBase) = new CDlgTGEmbark;
			(*pDlgTGBase)->Create(CDlgTGEmbark::IDD, pParent);
			break;
		case TG_EACTION_WALL:
			(*pDlgTGBase) = new CDlgTGWall;
			(*pDlgTGBase)->Create(CDlgTGWall::IDD, pParent);
			break;
		case TG_EACTION_SUBJECT:
			(*pDlgTGBase) = new CDlgTGSubject;
			(*pDlgTGBase)->Create(CDlgTGSubject::IDD, pParent);
			break;
		case TG_EACTION_INFINITY_MON_GEN:
			(*pDlgTGBase) = new CDlgTGInfinityMonsterGen;
			(*pDlgTGBase)->Create(CDlgTGInfinityMonsterGen::IDD, pParent);
			break;
		}
	}
	break;
	case TG_TYPE_CONDITION:
		switch (iType)
		{
		case TG_CONDITION_KILLMASTER:
			(*pDlgTGBase) = new CDlgTGKillMaster;
			(*pDlgTGBase)->Create(CDlgTGKillMaster::IDD, pParent);
			break;
		case TG_CONDITION_TIMER:
			(*pDlgTGBase) = new CDlgTGTimer;
			(*pDlgTGBase)->Create(CDlgTGTimer::IDD, pParent);
			break;
		case TG_CONDITION_POSITION:
			(*pDlgTGBase) = new CDlgTGPosition;
			(*pDlgTGBase)->Create(CDlgTGPosition::IDD, pParent);
			break;
		case TG_CONDITION_OBJ_DESTROY:
			(*pDlgTGBase) = new CDlgTGObjDestroy;
			(*pDlgTGBase)->Create(CDlgTGObjDestroy::IDD, pParent);
			break;
		case TG_CONDITION_DIALOGUE_STATE:
			(*pDlgTGBase) = new CDlgTGDialogueState;
			(*pDlgTGBase)->Create(CDlgTGDialogueState::IDD, pParent);
			break;
		case TG_CONDITION_HP_CHANGE:
			(*pDlgTGBase) = new CDlgTGCheckHP;
			(*pDlgTGBase)->Create(CDlgTGCheckHP::IDD, pParent);
			break;
		case TG_CONDITION_WALL:
			(*pDlgTGBase) = new CDlgTGCheckWall;
			(*pDlgTGBase)->Create(CDlgTGCheckWall::IDD, pParent);
			break;
		case TG_CONDITION_KILLALLMONSTER:
			(*pDlgTGBase) = new CDlgTGKillAllMonster;
			(*pDlgTGBase)->Create(CDlgTGKillAllMonster::IDD, pParent);
			break;
		}
		break;
	}
}

BOOL CDlgTGChildGenner::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		OnBnClickedOk();

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		OnBnClickedCancel();

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTGChildGenner::InitialWindowVariable(void)
{
	//************************************************************************
	// 비어있는 몬스터 인덱스를 설정해준다.
	//************************************************************************
	if (m_pdlgTGMonGenerater && CREATE_MODE == m_Mode
		)
	{
		CQuestScripterDlg *pScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
		if (pScriptDlg)
		{
			CDlgTrigger *pDlgTrigger = &pScriptDlg->m_dlgTrigger;
			if (pDlgTrigger)
			{
				int iBlankIndex = pDlgTrigger->m_dlgTriggerContent.GetBlankMonsterSlotInTrigger();
				m_pdlgTGMonGenerater->m_MonsterGenerater.m_iMonIndex = iBlankIndex;
			}
		}
	}

	SetTGInfo();
	InitListBox();
	RefreshDialog();
	AllDialogRefresh();
}

void CDlgTGChildGenner::SendCurrentEActionToDlgTGContent(void)
{
	CQuestScripterDlg *pScriptDlg = (CQuestScripterDlg*)AfxGetMainWnd();
	if (!pScriptDlg) return;

	//************************************************************************
	// 현재 이벤트, 액션 정보를 트리거정보창으로 메시지를 보냅니다.
	//************************************************************************
	if (CREATE_MODE == m_Mode)
	{
		switch (m_iFunctionType)
		{
		case 0:
			pScriptDlg->m_dlgTrigger.m_dlgTriggerContent.SendMessage(WM_ADD_EVENT, 0, 0);
			break;
		case 1:
			pScriptDlg->m_dlgTrigger.m_dlgTriggerContent.SendMessage(WM_ADD_CONDITION, 0, 0);
			break;
		case 2:
			pScriptDlg->m_dlgTrigger.m_dlgTriggerContent.SendMessage(WM_ADD_ACTION, 0, 0);
			break;
		}
	}

	if (MODIFY_MODE == m_Mode)
	{
		switch (m_iFunctionType)
		{
		case 0:
			pScriptDlg->m_dlgTrigger.m_dlgTriggerContent.SendMessage(WM_MODIFY_EVENT, 0, 0);
			break;
		case 1:
			pScriptDlg->m_dlgTrigger.m_dlgTriggerContent.SendMessage(WM_MODIFY_CONDITION, 0, 0);
			break;
		case 2:
			pScriptDlg->m_dlgTrigger.m_dlgTriggerContent.SendMessage(WM_MODIFY_ACTION, 0, 0);
			break;
		}
	}

	AllDialogInitValue();
}

void CDlgTGChildGenner::SetCurWindowToPos(POINT ptPos)
{
	switch (m_enType)
	{
	case TG_TYPE_EACTION:
		if (!m_mapDlgEAction.count(m_enAction)) return;

		if (m_enAction == TG_EACTION_WATER)
		{
			if (m_pdlgTGWater)
			{
				m_pdlgTGWater->m_TGWater.m_iWaterHeight = ptPos.y;
				m_pdlgTGWater->Refresh();
			}
		}
		break;
	case TG_TYPE_CONDITION:
		if (!m_mapDlgCondition.count(m_enCondition)) return;

		//if( m_enCondition == ? )
		//{
		//}
		break;
	}
}