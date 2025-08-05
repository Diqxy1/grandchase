// DlgTGDeleteMonster.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGDeleteMonster.h"
#include "FunctionToScriptFile.h"
#include "ObjectDataManager.h"
#include ".\dlgtgdeletemonster.h"

// CDlgTGDeleteMonster 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGDeleteMonster, CDlgTGBase)
CDlgTGDeleteMonster::CDlgTGDeleteMonster(CWnd* pParent /*=NULL*/)
{
}

CDlgTGDeleteMonster::~CDlgTGDeleteMonster()
{
}

void CDlgTGDeleteMonster::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_MONSTER_SLOT, m_cbSlot);
    DDX_Control(pDX, IDC_STATIC_DELETE_MONSTER_1, m_stDeleteMonster1);
    DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_cbMonsterID);
    DDX_Control(pDX, IDC_STATIC_DELETE_MONSTER_2, m_stDeleteMonster2);
    DDX_Control(pDX, IDC_CHECK_DELETE_ALL, m_chDeleteAll);

    m_cbMonsterID.Init();

    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_MONSTER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

    m_cbMonsterID.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size();++i)
    {
        m_cbMonsterID.InsertString( i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i] );
        m_cbMonsterID.AddSearchString( SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i] );
    }
    if( m_cbMonsterID.GetCount() > 0 ) m_cbMonsterID.SetCurSel(0);
    m_chDeleteAll.SetCheck(false);
}


BEGIN_MESSAGE_MAP(CDlgTGDeleteMonster, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK_DELETE_ALL, OnBnClickedCheckDeleteAll)
END_MESSAGE_MAP()


// CDlgTGDeleteMonster 메시지 처리기입니다.

void CDlgTGDeleteMonster::Refresh(void)
{
    if( m_cbSlot.GetCount() > (int)m_TGDeleteMonster.m_iSlot ) m_cbSlot.SetCurSel((int)m_TGDeleteMonster.m_iSlot);
    if( m_cbMonsterID.GetCount() > (int)m_TGDeleteMonster.m_iMonsterID ) m_cbMonsterID.SetCurSel((int)m_TGDeleteMonster.m_iMonsterID);
    m_chDeleteAll.SetCheck(m_TGDeleteMonster.m_bAllSlot);
}

void CDlgTGDeleteMonster::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strSlot;
    if( m_cbSlot.GetCurSel() >= 0 && m_cbSlot.GetCount() > m_cbSlot.GetCurSel() )
        m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);

    CString strMonsterID;
    if( m_cbMonsterID.GetCurSel() >= 0 && m_cbMonsterID.GetCount() > m_cbMonsterID.GetCurSel() )
        m_cbMonsterID.GetLBText(m_cbMonsterID.GetCurSel(), strMonsterID);

    CString strText1, strText2;
    m_stDeleteMonster1.GetWindowText(strText1);
    m_stDeleteMonster2.GetWindowText(strText2);

    if( m_chDeleteAll.GetCheck() )
    {
        m_strDesc = "[Todos] Remove um monstro de um slot de monstro";
    }
    else
    {
        m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 + L" " 
            + MiddleBraceSidebyString(strMonsterID) + L" " + strText2;
    }

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGDeleteMonster.m_iSlot           = (int)m_cbSlot.GetCurSel();
    m_TGDeleteMonster.m_iMonsterID      = (int)m_cbMonsterID.GetCurSel();
    m_TGDeleteMonster.m_bAllSlot        = m_chDeleteAll.GetCheck();
    m_TGDeleteMonster.m_strDesc = GetDesc();
}

void CDlgTGDeleteMonster::OnBnClickedOk()
{

}

void CDlgTGDeleteMonster::OnBnClickedCancel()
{

}
void CDlgTGDeleteMonster::OnBnClickedCheckDeleteAll()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    if( m_chDeleteAll.GetCheck() )
    {
        m_cbSlot.EnableWindow(false);
    }
    else
    {
        m_cbSlot.EnableWindow(true);
    }
}
