// DlgTGUnitParticle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGUnitParticle.h"
#include ".\dlgtgunitparticle.h"
#include "ObjectDataManager.h"
#include "DlgTrigger.h"

// CDlgTGUnitParticle 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGUnitParticle, CDlgTGBase)
CDlgTGUnitParticle::CDlgTGUnitParticle(CWnd* pParent /*=NULL*/)
{
}

CDlgTGUnitParticle::~CDlgTGUnitParticle()
{
}

void CDlgTGUnitParticle::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PLAYER_OR_MONSTER, m_cbPlayerOrMonster);
    DDX_Control(pDX, IDC_COMBO_MON_SLOT, m_cbSlot);
    DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_cbMonsterID);
    DDX_Control(pDX, IDC_COMBO_UNIT_PARTICLE, m_cbParticleID);
    DDX_Control(pDX, IDC_STATIC_UNIT_PARTICLE_1, m_stUnitParticle1);
    DDX_Control(pDX, IDC_STATIC_UNIT_PARTICLE_2, m_stUnitParticle2);

    m_cbMonsterID.Init();
    m_cbParticleID.Init();

    m_cbPlayerOrMonster.ResetContent();
    m_cbPlayerOrMonster.InsertString(0, L"몬스터");
    m_cbPlayerOrMonster.InsertString(1, L"플레이어");
    if( m_cbPlayerOrMonster.GetCount() )
    {
        m_cbPlayerOrMonster.SetCurSel(0);
        ResetSlotByPlayer();
    }

    m_cbMonsterID.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON ).size();++i)
    {
        m_cbMonsterID.InsertString( i, SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON )[i] );
        m_cbMonsterID.AddSearchString( SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON )[i] );
    }
    if( m_cbMonsterID.GetCount() > 0 ) m_cbMonsterID.SetCurSel(0);
    DDX_Control(pDX, IDC_BUTTON_SELECT_FROM_MAP_SEL_MON, m_btMonster);
}


BEGIN_MESSAGE_MAP(CDlgTGUnitParticle, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP_SEL_MON, OnBnClickedButtonSelectFromMapSelMon)
    ON_CBN_SELCHANGE(IDC_COMBO_PLAYER_OR_MONSTER, OnCbnSelchangeComboPlayerOrMonster)
END_MESSAGE_MAP()


// CDlgTGUnitParticle 메시지 처리기입니다.


void CDlgTGUnitParticle::Refresh(void)
{
    if( m_cbPlayerOrMonster.GetCount() > (int)m_TGUnitParticle.m_bPlayer ) m_cbPlayerOrMonster.SetCurSel((int)m_TGUnitParticle.m_bPlayer);
    if( m_cbSlot.GetCount() > (int)m_TGUnitParticle.m_iSlot ) m_cbSlot.SetCurSel((int)m_TGUnitParticle.m_iSlot);
    if( m_cbMonsterID.GetCount() > (int)m_TGUnitParticle.m_iMonsterID ) m_cbMonsterID.SetCurSel((int)m_TGUnitParticle.m_iMonsterID);

    if( m_cbParticleID.GetCount() > (int)m_TGUnitParticle.m_iParticleID ) m_cbParticleID.SetCurSel((int)m_TGUnitParticle.m_iParticleID);

    OnCbnSelchangeComboPlayerOrMonster();
}

void CDlgTGUnitParticle::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strParticleID;
    if( m_cbParticleID.GetCurSel() >= 0 && m_cbParticleID.GetCount() > m_cbParticleID.GetCurSel() )
        m_cbParticleID.GetLBText(m_cbParticleID.GetCurSel(), strParticleID);

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
    m_stUnitParticle1.GetWindowText(strText1);
    m_stUnitParticle2.GetWindowText(strText2);

    switch( m_cbPlayerOrMonster.GetCurSel() )
    {
    case 0:     // 몬스터
        m_strDesc = MiddleBraceSidebyString(strPlayerOrMonster) + L" " + MiddleBraceSidebyString(strSlot)
            + L" " + MiddleBraceSidebyString(strMonsterID) + L" " + strText1 + L" " + MiddleBraceSidebyString(strParticleID)
            + strText2;
        break;
    case 1:     // 플레이어
        {
            m_strDesc = MiddleBraceSidebyString(strPlayerOrMonster) + L" " + MiddleBraceSidebyString(strSlot)
                + L" " + strText1 + L" " + MiddleBraceSidebyString(strParticleID) + strText2;
        }
        break;
    }

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGUnitParticle.m_bPlayer          = (bool)m_cbPlayerOrMonster.GetCurSel();
    m_TGUnitParticle.m_iSlot            = (int)m_cbSlot.GetCurSel();
    m_TGUnitParticle.m_iMonsterID       = (int)m_cbMonsterID.GetCurSel();
    m_TGUnitParticle.m_iParticleID      = (int)m_cbParticleID.GetCurSel();
    m_TGUnitParticle.m_strDesc = GetDesc();
}

void CDlgTGUnitParticle::OnBnClickedOk()
{

}

void CDlgTGUnitParticle::OnBnClickedCancel()
{

}

void CDlgTGUnitParticle::OnBnClickedButtonSelectFromMapSelMon()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_MONSTER_SEL_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGUnitParticle::SetMonsterInfo(bool bPlayer, int iSlot, int iType)
{
    m_TGUnitParticle.m_bPlayer = bPlayer;
    m_TGUnitParticle.m_iSlot = iSlot;
    m_TGUnitParticle.m_iMonsterID = iType;

    Refresh();
}

void CDlgTGUnitParticle::ResetSlotByMonster(void)
{
    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_MONSTER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > m_TGUnitParticle.m_iSlot ) m_cbSlot.SetCurSel(m_TGUnitParticle.m_iSlot);
}

void CDlgTGUnitParticle::ResetSlotByPlayer(void)
{
    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_PLAYER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > m_TGUnitParticle.m_iSlot ) m_cbSlot.SetCurSel(m_TGUnitParticle.m_iSlot);
}

void CDlgTGUnitParticle::OnCbnSelchangeComboPlayerOrMonster()
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
