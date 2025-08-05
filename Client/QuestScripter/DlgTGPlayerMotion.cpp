// DlgTGPlayerMotion.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGPlayerMotion.h"
#include "FunctionToScriptFile.h"
#include "EnumOfTrigger.h"

// CDlgTGPlayerMotion 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGPlayerMotion, CDlgTGBase)
CDlgTGPlayerMotion::CDlgTGPlayerMotion(CWnd* pParent /*=NULL*/)
{
}

CDlgTGPlayerMotion::~CDlgTGPlayerMotion()
{
}

void CDlgTGPlayerMotion::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PLAYER_SLOT, m_cbSlot);
    DDX_Control(pDX, IDC_COMBO_TG_PLAYERPOSITION_RIGHT, m_cbMotion);
    DDX_Control(pDX, IDC_STATIC_PLAYER_MOTION_1, m_stPlayerMotion1);
    DDX_Control(pDX, IDC_STATIC_PLAYER_MOTION_2, m_stPlayerMotion2);

    m_cbMotion.Init();

    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_PLAYER;i++)
    {
        CString strText;
        strText.Format(L"%d", i);
        m_cbSlot.InsertString(i, strText);
    }
    if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

    std::vector<CString> vtMotion;
    vtMotion.push_back(L"ATK1");
    vtMotion.push_back(L"ATKEND");
    vtMotion.push_back(L"WAIT");
    vtMotion.push_back(L"FATAL_WAIT");
    vtMotion.push_back(L"WALK");
    vtMotion.push_back(L"SPECIAL1_W");
    vtMotion.push_back(L"SPECIAL2_W");
    vtMotion.push_back(L"SPECIAL3_W");
    vtMotion.push_back(L"SPECIAL1_B");
    vtMotion.push_back(L"SPECIAL2_B");
    vtMotion.push_back(L"SPECIAL3_B");
    vtMotion.push_back(L"ORIGINAL1_W");
    vtMotion.push_back(L"ORIGINAL2_W");
    vtMotion.push_back(L"ORIGINAL3_W");
    vtMotion.push_back(L"ORIGINAL1_B");
    vtMotion.push_back(L"ORIGINAL2_B");
    vtMotion.push_back(L"ORIGINAL3_B");
    vtMotion.push_back(L"DASH");
    vtMotion.push_back(L"DASH_ATK");
    vtMotion.push_back(L"WIN");
    vtMotion.push_back(L"START");
    vtMotion.push_back(L"SELECT");
    vtMotion.push_back(L"RESULT_WIN");
    vtMotion.push_back(L"RESULT_LOSE");
    vtMotion.push_back(L"JOKE");
    vtMotion.push_back(L"RUN");
    vtMotion.push_back(L"JUMP_DASH_ATK");
    vtMotion.push_back(L"ATK1_FASTINPUT");
    vtMotion.push_back(L"JUMP");
    vtMotion.push_back(L"JUMP_ATK");
    vtMotion.push_back(L"JUMP_DASH");
    vtMotion.push_back(L"JUMP_READY");
    vtMotion.push_back(L"DOUBLE_JUMP");
    vtMotion.push_back(L"DOUBLE_JUMP_ATK");
    vtMotion.push_back(L"DOUBLE_JUMP_LAND");
    vtMotion.push_back(L"CRITICAL_ATK");
    vtMotion.push_back(L"DOUBLE_ATK");

    for(int i=0;i<(int)vtMotion.size();++i)
    {
        m_cbMotion.AddSearchString(vtMotion[i]);
        m_cbMotion.InsertString(i, vtMotion[i]);
    }
}


BEGIN_MESSAGE_MAP(CDlgTGPlayerMotion, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGPlayerMotion 메시지 처리기입니다.

void CDlgTGPlayerMotion::Refresh(void)
{
    if( m_cbSlot.GetCount() > (int)m_TGPlayerMotion.m_iSlot ) m_cbSlot.SetCurSel((int)m_TGPlayerMotion.m_iSlot);
    if( m_cbMotion.GetCount() > (int)m_TGPlayerMotion.m_iMotion ) m_cbMotion.SetCurSel((int)m_TGPlayerMotion.m_iMotion);
}

void CDlgTGPlayerMotion::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************

    CString strSlot;
    if( m_cbSlot.GetCurSel() >= 0 && m_cbSlot.GetCount() > m_cbSlot.GetCurSel() )
        m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);

    CString strMotion;
    if( m_cbMotion.GetCurSel() >= 0 && m_cbMotion.GetCount() > m_cbMotion.GetCurSel() )
        m_cbMotion.GetLBText(m_cbMotion.GetCurSel(), strMotion);

    CString strText1, strText2;
    m_stPlayerMotion1.GetWindowText(strText1);
    m_stPlayerMotion2.GetWindowText(strText2);

    m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 
                + L" " + MiddleBraceSidebyString(strMotion) + strText2;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGPlayerMotion.m_iSlot            = (bool)m_cbSlot.GetCurSel();
    m_TGPlayerMotion.m_iMotion          = (int)m_cbMotion.GetCurSel();
    m_TGPlayerMotion.m_strDesc = GetDesc();
}

void CDlgTGPlayerMotion::OnBnClickedOk()
{

}

void CDlgTGPlayerMotion::OnBnClickedCancel()
{

}