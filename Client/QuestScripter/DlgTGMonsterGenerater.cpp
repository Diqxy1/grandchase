// DlgTGMonsterGenerater.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGMonsterGenerater.h"
#include "ObjectDataManager.h"
#include ".\dlgtgmonstergenerater.h"
// CDlgTGMonsterGenerater 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGMonsterGenerater, CDlgTGBase)
CDlgTGMonsterGenerater::CDlgTGMonsterGenerater(CWnd* pParent /*=NULL*/)
{
}

CDlgTGMonsterGenerater::~CDlgTGMonsterGenerater()
{
}

void CDlgTGMonsterGenerater::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MONSTER_SLOT, m_cbSlot);
	DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_cbMonID);
	DDX_Control(pDX, IDC_COMBO_MONSTER_BOSS, m_cbBoss);
	DDX_Control(pDX, IDC_COMBO_MONSTER_RIGHT, m_cbRight);
	DDX_Control(pDX, IDC_STATIC_MONGEN_1, m_stMonGen1);
	DDX_Control(pDX, IDC_STATIC_MONGEN_2, m_stMonGen2);
	DDX_Control(pDX, IDC_STATIC_MONGEN_3, m_stMonGen3);
	DDX_Control(pDX, IDC_STATIC_MONGEN_4, m_stMonGen4);
	DDX_Control(pDX, IDC_STATIC_MONGEN_5, m_stMonGen5);
	DDX_Control(pDX, IDC_STATIC_MONGEN_6, m_stMonGen6);
    DDX_Control(pDX, IDC_STATIC_MONGEN_7, m_stMonGen7);
	DDX_Control(pDX, IDC_EDIT_MONSTER_LEVEL, m_edMonsterLevel);
	DDX_Control(pDX, IDC_EDIT_MONSTER_X, m_ebX);
	DDX_Control(pDX, IDC_EDIT_MONSTER_Y, m_ebY);
    DDX_Control(pDX, IDC_EDIT_MONGEN_PERCENT, m_ebPercent);
	DDX_Control(pDX, IDC_CHECK_CHAMPION, m_chChapion);
    DDX_Control(pDX, IDC_RADIO_MONGEN1, m_rdRandom);
    DDX_Control(pDX, IDC_RADIO_MONGEN2, m_rdPosition);


	m_cbMonID.Init();

	m_cbSlot.ResetContent();
	for(int i=0;i<MAX_SLOT_MONSTER;i++)
	{
		CString strText;
		strText.Format(L"%d", i);
		m_cbSlot.InsertString(i, strText);
	}
	if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

	m_cbMonID.ResetContent();
	for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size();++i)
	{
		m_cbMonID.InsertString( i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i] );
		m_cbMonID.AddSearchString( SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i] );
	}
	if( m_cbMonID.GetCount() > 0 ) m_cbMonID.SetCurSel(0);

	m_cbBoss.ResetContent();
	m_cbBoss.InsertString(0, L"일반몬스터");
	m_cbBoss.InsertString(1, L"보스");
	m_cbBoss.InsertString(2, L"중간보스");
	if( m_cbBoss.GetCount() > 0 ) m_cbBoss.SetCurSel(0);

	m_cbRight.ResetContent();
	m_cbRight.InsertString(0, L"왼쪽");
	m_cbRight.InsertString(1, L"오른쪽");
	if( m_cbRight.GetCount() > 0 ) m_cbRight.SetCurSel(0);
	m_edMonsterLevel.SetWindowText(L"");

	Refresh();
}


BEGIN_MESSAGE_MAP(CDlgTGMonsterGenerater, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_CHAMPION, &CDlgTGMonsterGenerater::OnBnClickedCheckChampion)
    ON_BN_CLICKED(IDC_RADIO_MONGEN1, &CDlgTGMonsterGenerater::OnBnClickedRadioMongen1)
    ON_BN_CLICKED(IDC_RADIO_MONGEN2, &CDlgTGMonsterGenerater::OnBnClickedRadioMongen2)
END_MESSAGE_MAP()


// CDlgTGMonsterGenerater 메시지 처리기입니다.
void CDlgTGMonsterGenerater::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGMonsterGenerater::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGMonsterGenerater::Refresh(void)
{
    if( m_cbBoss.GetCount() > (int)m_MonsterGenerater.m_iBoss ) m_cbBoss.SetCurSel(m_MonsterGenerater.m_iBoss);
    if( m_cbRight.GetCount() > (int)m_MonsterGenerater.m_bRight ) m_cbRight.SetCurSel(m_MonsterGenerater.m_bRight);
    if( m_cbSlot.GetCount() > m_MonsterGenerater.m_iMonIndex ) m_cbSlot.SetCurSel(m_MonsterGenerater.m_iMonIndex);
    if( m_cbMonID.GetCount() > m_MonsterGenerater.m_iMonType ) m_cbMonID.SetCurSel(m_MonsterGenerater.m_iMonType);
	m_chChapion.SetCheck(m_MonsterGenerater.m_bNotChampion);
	CString strX, strY, strMonLevel, strPercent;
    strX.Format(L"%f", m_MonsterGenerater.m_fEntranceX);
    strY.Format(L"%f", m_MonsterGenerater.m_fEntranceY);
    strMonLevel.Format(L"%d", m_MonsterGenerater.m_iMonLevel);
    m_ebX.SetWindowText(strX);
    m_ebY.SetWindowText(strY);
    m_edMonsterLevel.SetWindowText(strMonLevel);
    strPercent.Format(L"%d", m_MonsterGenerater.m_iPercent);
    m_ebPercent.SetWindowText(strPercent);
    m_rdRandom.SetCheck(m_MonsterGenerater.m_bRandom);
}

void CDlgTGMonsterGenerater::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    CString strSlot = L"X";
    CString strMonID = L"X";
    CString strMonLevel = L"0";
    {
        if( m_cbSlot.GetCurSel() >= 0 )
            m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);
        
        m_cbMonID.GetWindowText(strMonID);        
        int index = m_cbMonID.FindStringExact(0, strMonID);
        if( CB_ERR != index )
            m_cbMonID.SetCurSel(index);
        else
            strMonID = "X";

        CString strBoss, strRight;
        if( m_cbBoss.GetCount() > m_cbBoss.GetCurSel() ) m_cbBoss.GetLBText(m_cbBoss.GetCurSel(), strBoss);
        if( m_cbRight.GetCount() > m_cbRight.GetCurSel() ) m_cbRight.GetLBText(m_cbRight.GetCurSel(), strRight);

        CString strX, strY, strPercent;
        m_ebX.GetWindowText(strX);
        m_ebY.GetWindowText(strY);
        m_ebPercent.GetWindowText(strPercent);

        m_edMonsterLevel.GetWindowText(strMonLevel);

        CString strText1, strText2, strText3, strText4, strText5, strText6, strText7;
        m_stMonGen1.GetWindowText(strText1);
        m_stMonGen2.GetWindowText(strText2);
        m_stMonGen3.GetWindowText(strText3);
        m_stMonGen4.GetWindowText(strText4);
        m_stMonGen5.GetWindowText(strText5);
        m_stMonGen6.GetWindowText(strText6);
        m_stMonGen7.GetWindowText(strText7);

        CString strLevel;
        strLevel.Format(L"%s.%s", strText6, strMonLevel);

        m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 + L" " + MiddleBraceSidebyString(strLevel) + MiddleBraceSidebyString(strMonID) 
            + L" " + strText2 + L" " + MiddleBraceSidebyString(strBoss) + L" " + strText3 + L" " + MiddleBraceSidebyString(strRight) + strText4
            + L" " + MiddleBraceSidebyString(strPercent) + strText7 + L","
            + L" (" + MiddleBraceSidebyString(strX) + L"," + MiddleBraceSidebyString(strY) + L") " + strText5;
    }


    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_MonsterGenerater.m_iBoss =m_cbBoss.GetCurSel();
        m_MonsterGenerater.m_bRight = (bool)m_cbRight.GetCurSel();
        m_MonsterGenerater.m_iMonIndex = m_cbSlot.GetCurSel();;
        m_MonsterGenerater.m_iMonType = m_cbMonID.GetCurSel();
        m_MonsterGenerater.m_iMonLevel = _wtoi(strMonLevel);

        CString strX, strY, strPercent;
        m_ebX.GetWindowText(strX);
        m_ebY.GetWindowText(strY);
        m_ebPercent.GetWindowText(strPercent);
        m_MonsterGenerater.m_bRandom = m_rdRandom.GetCheck() != 0;
        m_MonsterGenerater.m_fEntranceX = (float)_wtof(strX);
        m_MonsterGenerater.m_fEntranceY = (float)_wtof(strY);
        m_MonsterGenerater.m_iPercent = (int)_wtoi(strPercent);
        m_MonsterGenerater.m_strDesc = GetDesc();
    }
}

void CDlgTGMonsterGenerater::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        AfxGetMainWnd()->SendMessage(WM_MONSTER_ADD_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGMonsterGenerater::OnBnClickedCheckChampion()
{
	m_MonsterGenerater.m_bNotChampion = m_chChapion.GetCheck() != 0;
}

void CDlgTGMonsterGenerater::OnBnClickedRadioMongen1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_MonsterGenerater.m_bRandom = m_rdRandom.GetCheck() != 0;
}

void CDlgTGMonsterGenerater::OnBnClickedRadioMongen2()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
