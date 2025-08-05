// DlgTGKillMaster.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGKillMaster.h"
#include ".\dlgtgkillmaster.h"
#include "ObjectDataManager.h"
// CDlgTGKillMaster 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGKillMaster, CDlgTGBase)
CDlgTGKillMaster::CDlgTGKillMaster(CWnd* pParent /*=NULL*/)
{
}

CDlgTGKillMaster::~CDlgTGKillMaster()
{
}

void CDlgTGKillMaster::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MONSTER_SLOT, m_cbSlot);
	DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_cbMonID);
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


	Refresh();
	DDX_Control(pDX, IDC_STATIC_KILLMASTER_1, m_stKillMaster1);
	DDX_Control(pDX, IDC_STATIC_KILLMASTER_2, m_stKillMaster2);
	DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE2, m_cbMonsterIsDie);

	m_cbMonsterIsDie.ResetContent();
	m_cbMonsterIsDie.InsertString(0, L"Morto");
	m_cbMonsterIsDie.InsertString(1, L"Vivo");
	if( m_cbMonsterIsDie.GetCount() > 0 ) m_cbMonsterIsDie.SetCurSel(0);


}


BEGIN_MESSAGE_MAP(CDlgTGKillMaster, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgTGKillMaster 메시지 처리기입니다.

void CDlgTGKillMaster::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGKillMaster::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

//CKillMaster CDlgTGKillMaster::GetValue(void)
//{
//    return m_KillMaster;
//}
//
//void CDlgTGKillMaster::SetValue(CKillMaster KillMaster)
//{
//    m_KillMaster = KillMaster;
//}

void CDlgTGKillMaster::Refresh(void)
{
    if( m_cbSlot.GetCount() > m_KillMaster.m_iMonIndex ) m_cbSlot.SetCurSel(m_KillMaster.m_iMonIndex);
    if( m_cbMonID.GetCount() > m_KillMaster.m_iMonType ) m_cbMonID.SetCurSel(m_KillMaster.m_iMonType);
}

void CDlgTGKillMaster::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strSlot = L"X";
        CString strMonID = L"X";
        if( m_cbSlot.GetCurSel() >= 0 )
            m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);
        if( m_cbMonID.GetCurSel() >= 0 )
        {
            if( (int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size() > m_cbMonID.GetCurSel() )
                strMonID = SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[m_cbMonID.GetCurSel()];
        }

        CString strText1, strText2;
        m_stKillMaster1.GetWindowText(strText1);
        m_stKillMaster2.GetWindowText(strText2);
        m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 + L" " + MiddleBraceSidebyString(strMonID) + L" " + strText2;

		CString strUIOnOff = L"";
		if( m_cbMonsterIsDie.GetCurSel() >= 0 )
			m_cbMonsterIsDie.GetLBText(m_cbMonsterIsDie.GetCurSel(), strUIOnOff);

		m_strDesc += strUIOnOff;
    }

    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_KillMaster.m_iMonIndex = m_cbSlot.GetCurSel();

        CString strMonID;
        m_cbMonID.GetWindowText(strMonID);

        int iType = 0;
        for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size();++i)
        {
            if( strMonID == SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i] )
            {
                iType = i;
                break;
            }
        }
        m_KillMaster.m_iMonType = iType;
		m_KillMaster.m_bAlive = m_cbMonsterIsDie.GetCurSel();
        m_KillMaster.m_strDesc = GetDesc();
    }
}