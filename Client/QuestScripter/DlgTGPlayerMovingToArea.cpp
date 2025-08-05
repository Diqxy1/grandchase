// DlgTGPlayerMovingToArea.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGPlayerMovingToArea.h"
#include "FunctionToScriptFile.h"
#include "ObjectDataManager.h"
#include ".\dlgtgplayermovingtoarea.h"

// CDlgTGPlayerMovingToArea 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGPlayerMovingToArea, CDlgTGBase)
CDlgTGPlayerMovingToArea::CDlgTGPlayerMovingToArea(CWnd* pParent /*=NULL*/)
{
}

CDlgTGPlayerMovingToArea::~CDlgTGPlayerMovingToArea()
{
}

void CDlgTGPlayerMovingToArea::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PLAYER_OR_MONSTER, m_cbPlayerOrMonster);
    DDX_Control(pDX, IDC_COMBO_MON_SLOT, m_cbSlot);
    DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_cbMonsterID);
    DDX_Control(pDX, IDC_EDIT_LEFT, m_ebLeft);
    DDX_Control(pDX, IDC_EDIT_RIGHT, m_ebRight);
    DDX_Control(pDX, IDC_EDIT_TOP, m_ebTop);
    DDX_Control(pDX, IDC_EDIT_BOTTOM, m_ebBottom);
    DDX_Control(pDX, IDC_STATIC_PLAYER_MOVING_TO_AREA_1, m_stPlayerMovingToArea1);
    DDX_Control(pDX, IDC_STATIC_PLAYER_MOVING_TO_AREA_2, m_stPlayerMovingToArea2);

    m_cbMonsterID.Init();

    m_cbPlayerOrMonster.ResetContent();
    m_cbPlayerOrMonster.InsertString(0, L"몬스터");
    m_cbPlayerOrMonster.InsertString(1, L"플레이어");
    if( m_cbPlayerOrMonster.GetCount() )
        m_cbPlayerOrMonster.SetCurSel(0);

    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_PLAYER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

    m_cbMonsterID.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON ).size();++i)
    {
        m_cbMonsterID.InsertString( i, SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON )[i] );
        m_cbMonsterID.AddSearchString( SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_MON )[i] );
    }
    if( m_cbMonsterID.GetCount() > 0 ) m_cbMonsterID.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CDlgTGPlayerMovingToArea, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP_SEL_MON, OnBnClickedButtonSelectFromMapSelMon)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP_SEL_AREA, OnBnClickedButtonSelectFromMapSelArea)
END_MESSAGE_MAP()


// CDlgTGPlayerMovingToArea 메시지 처리기입니다.

void CDlgTGPlayerMovingToArea::Refresh(void)
{
    if( m_cbPlayerOrMonster.GetCount() > (int)m_TGPlayerMovingToArea.m_bPlayer ) m_cbPlayerOrMonster.SetCurSel((int)m_TGPlayerMovingToArea.m_bPlayer);
    if( m_cbSlot.GetCount() > (int)m_TGPlayerMovingToArea.m_iSlot ) m_cbSlot.SetCurSel((int)m_TGPlayerMovingToArea.m_iSlot);
    if( m_cbMonsterID.GetCount() > (int)m_TGPlayerMovingToArea.m_iMonsterID ) m_cbMonsterID.SetCurSel((int)m_TGPlayerMovingToArea.m_iMonsterID);
    
    CString strLeft, strRight, strTop, strBottom;
    strLeft.Format(L"%d", m_TGPlayerMovingToArea.m_iLeft);
    strRight.Format(L"%d", m_TGPlayerMovingToArea.m_iRight);
    strTop.Format(L"%d", m_TGPlayerMovingToArea.m_iTop);
    strBottom.Format(L"%d", m_TGPlayerMovingToArea.m_iBottom);

    m_ebLeft.SetWindowText(strLeft);
    m_ebRight.SetWindowText(strRight);
    m_ebTop.SetWindowText(strTop);
    m_ebBottom.SetWindowText(strBottom);
}

void CDlgTGPlayerMovingToArea::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    
    CString strPlayerOrMonster;
    if( m_cbPlayerOrMonster.GetCurSel() >= 0 && m_cbPlayerOrMonster.GetCount() > m_cbPlayerOrMonster.GetCurSel() )
        m_cbPlayerOrMonster.GetLBText(m_cbPlayerOrMonster.GetCurSel(), strPlayerOrMonster);

    CString strSlot;
    if( m_cbSlot.GetCurSel() >= 0 && m_cbSlot.GetCount() > m_cbSlot.GetCurSel() )
        m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);

    CString strMonsterID;
    if( m_cbMonsterID.GetCurSel() >= 0 && m_cbMonsterID.GetCount() > m_cbMonsterID.GetCurSel() )
        m_cbMonsterID.GetLBText(m_cbMonsterID.GetCurSel(), strMonsterID);

    CString strLeft, strRight, strTop, strBottom, strTotal;
    m_ebLeft.GetWindowText(strLeft);
    m_ebRight.GetWindowText(strRight);
    m_ebTop.GetWindowText(strTop);
    m_ebBottom.GetWindowText(strBottom);
    strTotal.Format(L"LTRB (%s, %s, %s, %s)", strLeft, strTop, strRight, strBottom);

    CString strText1, strText2;
    m_stPlayerMovingToArea1.GetWindowText(strText1);
    m_stPlayerMovingToArea2.GetWindowText(strText2);

    switch( m_cbPlayerOrMonster.GetCurSel() )
    {
    case 0:     // 몬스터
        m_strDesc = MiddleBraceSidebyString(strPlayerOrMonster) + L" " + MiddleBraceSidebyString(strSlot)
            + L" " + MiddleBraceSidebyString(strMonsterID) + L" " + strText1 + L" " + MiddleBraceSidebyString(strTotal)
            + strText2;
        break;
    case 1:     // 플레이어
        {
            m_strDesc = MiddleBraceSidebyString(strPlayerOrMonster) + L" " + MiddleBraceSidebyString(strSlot)
                + L" " + strText1 + L" " + MiddleBraceSidebyString(strTotal) + strText2;
        }
        break;
    }

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGPlayerMovingToArea.m_bPlayer          = (bool)m_cbPlayerOrMonster.GetCurSel();
    m_TGPlayerMovingToArea.m_iSlot            = (int)m_cbSlot.GetCurSel();
    m_TGPlayerMovingToArea.m_iMonsterID       = (int)m_cbMonsterID.GetCurSel();
    m_TGPlayerMovingToArea.m_iLeft            = (int)_wtoi(strLeft);
    m_TGPlayerMovingToArea.m_iRight           = (int)_wtoi(strRight);
    m_TGPlayerMovingToArea.m_iTop             = (int)_wtoi(strTop);
    m_TGPlayerMovingToArea.m_iBottom          = (int)_wtoi(strBottom);
    m_TGPlayerMovingToArea.m_strDesc = GetDesc();
}

void CDlgTGPlayerMovingToArea::OnBnClickedOk()
{

}

void CDlgTGPlayerMovingToArea::OnBnClickedCancel()
{

}

void CDlgTGPlayerMovingToArea::SetMonsterInfo(bool bPlayer, int iSlot, int iType)
{
    m_TGPlayerMovingToArea.m_bPlayer = bPlayer;
    m_TGPlayerMovingToArea.m_iSlot = iSlot;
    m_TGPlayerMovingToArea.m_iMonsterID = iType;

    Refresh();
}

void CDlgTGPlayerMovingToArea::OnBnClickedButtonSelectFromMapSelMon()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_MONSTER_SEL_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGPlayerMovingToArea::OnBnClickedButtonSelectFromMapSelArea()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        CalcResult();
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_AREA_SEL_WINDOW, 0, 0);
        SetFocus();
    }
}

//************************************************************************
// 영역의 오른쪽이 왼쪽보다 큰값, 아래쪽이 위쪽보다 큰값을 가지게 보정
//************************************************************************
void CDlgTGPlayerMovingToArea::RectRevision(CRect &rtArea)
{
    if( rtArea.left > rtArea.right)
    {
        int iTemp = rtArea.left;
        rtArea.left = rtArea.right;
        rtArea.right = iTemp;
    }
    if( rtArea.top > rtArea.bottom)
    {
        int iTemp = rtArea.top;
        rtArea.top = rtArea.bottom;
        rtArea.bottom = iTemp;
    }
};

void CDlgTGPlayerMovingToArea::SetArea(CRect rtArea)
{
    RectRevision(rtArea);
    m_TGPlayerMovingToArea.m_iLeft = rtArea.left;
    m_TGPlayerMovingToArea.m_iRight = rtArea.right;
    m_TGPlayerMovingToArea.m_iTop = rtArea.top;
    m_TGPlayerMovingToArea.m_iBottom = rtArea.bottom;

    CString strLeft, strRight, strTop, strBottom;
    strLeft.Format(L"%d", m_TGPlayerMovingToArea.m_iLeft);
    strRight.Format(L"%d", m_TGPlayerMovingToArea.m_iRight);
    strTop.Format(L"%d", m_TGPlayerMovingToArea.m_iTop);
    strBottom.Format(L"%d", m_TGPlayerMovingToArea.m_iBottom);

    if( m_ebLeft.GetSafeHwnd() )
        m_ebLeft.SetWindowText(strLeft);
    if( m_ebRight.GetSafeHwnd() )
        m_ebRight.SetWindowText(strRight);
    if( m_ebTop.GetSafeHwnd() )
        m_ebTop.SetWindowText(strTop);
    if( m_ebBottom.GetSafeHwnd() )
        m_ebBottom.SetWindowText(strBottom);
}