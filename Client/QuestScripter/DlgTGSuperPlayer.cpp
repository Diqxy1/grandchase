// CDlgTGSuperPlayer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGSuperPlayer.h"


// CDlgTGSuperPlayer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGSuperPlayer, CDlgTGBase)
CDlgTGSuperPlayer::CDlgTGSuperPlayer(CWnd* pParent /*=NULL*/)
{
}

CDlgTGSuperPlayer::~CDlgTGSuperPlayer()
{
}

void CDlgTGSuperPlayer::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PLAYER_SLOT, m_cbPlayerSlot);
    DDX_Control(pDX, IDC_COMBO_SUPER_PLAYER, m_cbOnOff);

    m_cbPlayerSlot.ResetContent();
    m_cbPlayerSlot.InsertString(0, L"0");
    m_cbPlayerSlot.InsertString(1, L"1");
    m_cbPlayerSlot.InsertString(2, L"2");
    m_cbPlayerSlot.InsertString(3, L"3");
    m_cbPlayerSlot.InsertString(4, L"4");
    m_cbPlayerSlot.InsertString(5, L"5");

    m_cbOnOff.ResetContent();
    m_cbOnOff.InsertString(0, L"일반");
    m_cbOnOff.InsertString(1, L"무적");
    DDX_Control(pDX, IDC_STATIC_PLAYERGEN_1, m_stSuperPlayer1);
    DDX_Control(pDX, IDC_STATIC_PLAYERGEN_2, m_stSuperPlayer2);
}


BEGIN_MESSAGE_MAP(CDlgTGSuperPlayer, CDialog)
END_MESSAGE_MAP()


// CDlgTGSuperPlayer 메시지 처리기입니다.
void CDlgTGSuperPlayer::Refresh(void)
{
    if( m_cbPlayerSlot.GetCount() > m_TGSuperPlayer.m_iPlayerIndex ) m_cbPlayerSlot.SetCurSel(m_TGSuperPlayer.m_iPlayerIndex);
    m_cbOnOff.SetCurSel(m_TGSuperPlayer.m_bOnOff);
}

void CDlgTGSuperPlayer::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strPlayerSlot;
    if( m_cbPlayerSlot.GetCurSel() >= 0 )
        m_cbPlayerSlot.GetLBText(m_cbPlayerSlot.GetCurSel(), strPlayerSlot);
    CString strOnOff;
    if( m_cbOnOff.GetCurSel() >= 0 )
        m_cbOnOff.GetLBText(m_cbOnOff.GetCurSel(), strOnOff);

    CString strText1, strText2;
    m_stSuperPlayer1.GetWindowText(strText1);
    m_stSuperPlayer2.GetWindowText(strText2);

    m_strDesc = MiddleBraceSidebyString(strPlayerSlot) + L" " + strText1 + L" " 
                + MiddleBraceSidebyString(strOnOff) + L" " + strText2;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGSuperPlayer.m_bOnOff = m_cbOnOff.GetCurSel();
    m_TGSuperPlayer.m_iPlayerIndex = m_cbPlayerSlot.GetCurSel();
    m_TGSuperPlayer.m_strDesc = GetDesc();
}