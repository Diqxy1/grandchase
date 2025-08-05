// DlgTGHp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGHp.h"
#include ".\dlgtghp.h"
#include "FunctionToScriptFile.h"
#include "ObjectDataManager.h"

// CDlgTGHp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGHp, CDlgTGBase)
CDlgTGHp::CDlgTGHp(CWnd* pParent /*=NULL*/)
{
}

CDlgTGHp::~CDlgTGHp()
{
}

void CDlgTGHp::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PLAYER_OR_MONSTER, m_cbPlayerOrMonster);
    DDX_Control(pDX, IDC_COMBO_MON_SLOT, m_cbSlot);
    DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_cbMonsterID);
    DDX_Control(pDX, IDC_EDIT_TG_HP, m_ebHP);
    DDX_Control(pDX, IDC_STATIC_UNIT_HP_1, m_stTG_HP1);
    DDX_Control(pDX, IDC_STATIC_UNIT_HP_2, m_stTG_HP2);
    DDX_Control(pDX, IDC_BUTTON_SELECT_FROM_MAP_SEL_MON, m_btMonster);

    m_cbMonsterID.Init();

    m_cbPlayerOrMonster.ResetContent();
    m_cbPlayerOrMonster.InsertString(0, L"Monstro");
    m_cbPlayerOrMonster.InsertString(1, L"Player");
    if( m_cbPlayerOrMonster.GetCount() )
    {
        m_cbPlayerOrMonster.SetCurSel(0);
        ResetSlotByPlayer();
    }

    m_cbMonsterID.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON ).size();++i)
        m_cbMonsterID.InsertString( i, SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON )[i] );
    if( m_cbMonsterID.GetCount() ) m_cbMonsterID.SetCurSel(0);

    Refresh();
}


BEGIN_MESSAGE_MAP(CDlgTGHp, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP_SEL_MON, OnBnClickedButtonSelectFromMapSelMon)
    ON_CBN_SELCHANGE(IDC_COMBO_PLAYER_OR_MONSTER, OnCbnSelchangeComboPlayerOrMonster)
END_MESSAGE_MAP()


// CDlgTGHp 메시지 처리기입니다.

void CDlgTGHp::OnBnClickedButtonSelectFromMapSelMon()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        AfxGetMainWnd()->SendMessage(WM_MONSTER_SEL_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGHp::Refresh(void)
{
    if( m_cbPlayerOrMonster.GetCount() > (int)m_TGHP.m_bPlayer ) m_cbPlayerOrMonster.SetCurSel((int)m_TGHP.m_bPlayer);
    if( m_cbSlot.GetCount() > (int)m_TGHP.m_iSlot ) m_cbSlot.SetCurSel((int)m_TGHP.m_iSlot);
    if( m_cbMonsterID.GetCount() > (int)m_TGHP.m_iMonsterID ) m_cbMonsterID.SetCurSel((int)m_TGHP.m_iMonsterID);
    
    CString strHP;
    strHP.Format(L"%f", m_TGHP.m_fHp);
    m_ebHP.SetWindowText(strHP);

    OnCbnSelchangeComboPlayerOrMonster();
}

void CDlgTGHp::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strHP;
    m_ebHP.GetWindowText(strHP);

    CString strPlayerOrMonster;
    if( m_cbPlayerOrMonster.GetCurSel() >= 0 && m_cbPlayerOrMonster.GetCount() > m_cbPlayerOrMonster.GetCurSel() )
        m_cbPlayerOrMonster.GetLBText(m_cbPlayerOrMonster.GetCurSel(), strPlayerOrMonster);

    CString strSlot;
    if( m_cbSlot.GetCurSel() >= 0 && m_cbSlot.GetCount() > m_cbSlot.GetCurSel() )
        m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);

    CString strMonsterID;
    if( m_cbMonsterID.GetCurSel() >= 0 && m_cbMonsterID.GetCount() > m_cbMonsterID.GetCurSel() )
        m_cbMonsterID.GetLBText(m_cbMonsterID.GetCurSel(), strMonsterID);

    CString strText1, strText2;
    m_stTG_HP1.GetWindowText(strText1);
    m_stTG_HP2.GetWindowText(strText2);

    switch( m_cbPlayerOrMonster.GetCurSel() )
    {
    case 0:     // 몬스터
        m_strDesc = MiddleBraceSidebyString(strPlayerOrMonster) + L" " + MiddleBraceSidebyString(strSlot)
            + L" " + MiddleBraceSidebyString(strMonsterID) + L" " + strText1 + L" " + MiddleBraceSidebyString(strHP)
            + strText2;
        break;
    case 1:     // 플레이어
        {
            m_strDesc = MiddleBraceSidebyString(strPlayerOrMonster) + L" " + MiddleBraceSidebyString(strSlot)
                + L" " + strText1 + L" " + MiddleBraceSidebyString(strHP) + strText2;
        }
        break;
    }

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGHP.m_bPlayer              = (bool)m_cbPlayerOrMonster.GetCurSel();
    m_TGHP.m_iSlot                = (int)m_cbSlot.GetCurSel();
    m_TGHP.m_iMonsterID           = (int)m_cbMonsterID.GetCurSel();
    m_TGHP.m_fHp                  = (float)_wtof(strHP);
    m_TGHP.m_strDesc = GetDesc();
}

void CDlgTGHp::OnBnClickedOk()
{

}

void CDlgTGHp::OnBnClickedCancel()
{

}
void CDlgTGHp::OnCbnSelchangeComboPlayerOrMonster()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    switch( m_cbPlayerOrMonster.GetCurSel() )
    {
    case 0:         // Monster의 경우
        ResetSlotByMonster();
        m_cbSlot.EnableWindow(true);
        m_cbMonsterID.EnableWindow(true);
        m_btMonster.EnableWindow(true);
        break;
    case 1:
        ResetSlotByPlayer();
        m_cbMonsterID.EnableWindow(false);
        m_btMonster.EnableWindow(false);
        break;      // Player의 경우
    }
}

void CDlgTGHp::ResetSlotByMonster(void)
{
    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_MONSTER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > m_TGHP.m_iSlot ) m_cbSlot.SetCurSel(m_TGHP.m_iSlot);
}

void CDlgTGHp::ResetSlotByPlayer(void)
{
    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_PLAYER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > m_TGHP.m_iSlot ) m_cbSlot.SetCurSel(m_TGHP.m_iSlot);
}

void CDlgTGHp::SetMonsterInfo(bool bPlayer, int iSlot, int iType)
{
    m_TGHP.m_bPlayer = bPlayer;
    m_TGHP.m_iSlot = iSlot;
    m_TGHP.m_iMonsterID = iType;

    Refresh();
}