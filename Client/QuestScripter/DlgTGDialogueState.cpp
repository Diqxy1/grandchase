// DlgTGDialogueState.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGDialogueState.h"

// CDlgTGDialogueState 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGDialogueState, CDlgTGBase)
CDlgTGDialogueState::CDlgTGDialogueState(CWnd* pParent /*=NULL*/)
{
}

CDlgTGDialogueState::~CDlgTGDialogueState()
{
}

void CDlgTGDialogueState::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DIALOGUE_STATE, m_cbDialogueState);

    m_cbDialogueState.ResetContent();
    m_cbDialogueState.InsertString(0, L"Dialogo oculto");
    m_cbDialogueState.InsertString(1, L"Diaologo visivel");

    if( m_cbDialogueState.GetCount() > 1 ) m_cbDialogueState.SetCurSel(1);

    Refresh();
}


BEGIN_MESSAGE_MAP(CDlgTGDialogueState, CDialog)
END_MESSAGE_MAP()


// CDlgTGDialogueState 메시지 처리기입니다.


void CDlgTGDialogueState::Refresh(void)
{
    if( m_cbDialogueState.GetCount() > (int)m_DialogueState.m_bOnOff ) m_cbDialogueState.SetCurSel( (int)m_DialogueState.m_bOnOff );
}

void CDlgTGDialogueState::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strDialogueState = L"";
    if( m_cbDialogueState.GetCurSel() >= 0 )
        m_cbDialogueState.GetLBText(m_cbDialogueState.GetCurSel(), strDialogueState);

    m_strDesc = MiddleBraceSidebyString(strDialogueState);

    //************************************************************************
    // 값 저장
    //************************************************************************
    m_DialogueState.m_bOnOff = (bool)m_cbDialogueState.GetCurSel();
    m_DialogueState.m_strDesc = GetDesc();
}