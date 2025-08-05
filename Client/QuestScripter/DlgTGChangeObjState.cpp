// DlgTGChangeObjState.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGChangeObjState.h"
#include "ObjectDataManager.h"

// CDlgTGChangeObjState 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGChangeObjState, CDialog)
CDlgTGChangeObjState::CDlgTGChangeObjState(CWnd* pParent /*=NULL*/)
{
}

CDlgTGChangeObjState::~CDlgTGChangeObjState()
{
}

void CDlgTGChangeObjState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHANGE_OBJ_STATE_SLOT, m_cbSlot);
	DDX_Control(pDX, IDC_COMBO_CHANGE_OBJ_STATE_OBJ, m_cbType);
	DDX_Control(pDX, IDC_COMBO_CHANGE_OBJ_STATE, m_cbState);
	DDX_Control(pDX, IDC_STATIC_CHANGE_OBJ_STATE_1, m_stChangeObjState_1);
	DDX_Control(pDX, IDC_STATIC_CHANGE_OBJ_STATE_2, m_stChangeObjState_2);
	DDX_Control(pDX, IDC_COMBO_CHANGE_OBJ_STATE_OBJ2, m_cbObjType);
	DDX_Control(pDX, IDC_STATIC_CHANGE_OBJ_STATE_3, m_stChangeObjState_3);

	m_cbType.Init();
	m_cbState.Init();

	m_cbSlot.ResetContent();
	for(int i=0;i<MAX_SLOT_OBJECT;++i)
	{
		CString strSlot;
		strSlot.Format(L"%d", i);
		m_cbSlot.InsertString(i, strSlot);
	}
	if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

	m_cbType.ResetContent();
	for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT).size();++i)
	{
		m_cbType.InsertString( i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] );
		m_cbType.AddSearchString( SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] );
	}
	if( m_cbType.GetCount() > 0 ) m_cbType.SetCurSel(0);

	m_cbObjType.ResetContent();
	m_cbObjType.InsertString(0,L"Objeto");
	m_cbObjType.InsertString(1,L"Monstro");
	if( m_cbObjType.GetCount() > 0 ) m_cbObjType.SetCurSel(0);

	SetStateByObjType(0);

}


BEGIN_MESSAGE_MAP(CDlgTGChangeObjState, CDialog)
	ON_CBN_SELENDOK(IDC_COMBO_CHANGE_OBJ_STATE_OBJ2, &CDlgTGChangeObjState::OnCbnSelendokComboChangeObjStateObj2)
END_MESSAGE_MAP()


// CDlgTGChangeObjState 메시지 처리기입니다.

void CDlgTGChangeObjState::Refresh(void)
{
    if( m_cbSlot.GetCount() > m_TGChangeObjState.m_iSlot ) m_cbSlot.SetCurSel(m_TGChangeObjState.m_iSlot);
    if( m_cbType.GetCount() > m_TGChangeObjState.m_iType ) m_cbType.SetCurSel(m_TGChangeObjState.m_iType);
    if( m_cbState.GetCount() > m_TGChangeObjState.m_iState ) m_cbState.SetCurSel(m_TGChangeObjState.m_iState);
}

void CDlgTGChangeObjState::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strSlot = L"X";
        CString strObj = L"X";
        CString strState = L"X";

        int index = CB_ERR;

        m_cbSlot.GetWindowText(strSlot);        
        index = m_cbSlot.FindStringExact(0, strSlot);
        if( CB_ERR != index )
            m_cbSlot.SetCurSel(index);
        else
            strSlot = L"X";

        m_cbType.GetWindowText(strObj);        
        index = m_cbType.FindStringExact(0, strObj);
        if( CB_ERR != index )
            m_cbType.SetCurSel(index);
        else
            strObj = "X";

        m_cbState.GetWindowText(strState);        
        index = m_cbState.FindStringExact(0, strState);
        if( CB_ERR != index )
            m_cbState.SetCurSel(index);
        else
            strState = "X";

		CString strUIOnOff = L"";
		if( m_cbObjType.GetCurSel() >= 0 )
			m_cbObjType.GetLBText(m_cbObjType.GetCurSel(), strUIOnOff);

        CString strText1, strText2, strTxt3;
        m_stChangeObjState_1.GetWindowText(strText1);
		m_stChangeObjState_2.GetWindowText(strText2);
		m_stChangeObjState_3.GetWindowText(strTxt3);

        m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1+L" " + strUIOnOff+strTxt3+L" " + MiddleBraceSidebyString(strObj) + L" " 
            + MiddleBraceSidebyString(strState) + L" " + strText2;
    }


    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_TGChangeObjState.m_iSlot  = m_cbSlot.GetCurSel();
        m_TGChangeObjState.m_iType  = m_cbType.GetCurSel();
		m_TGChangeObjState.m_iState = m_cbState.GetCurSel();
		m_TGChangeObjState.m_iOBJType = m_cbObjType.GetCurSel();

        m_TGChangeObjState.m_strDesc = GetDesc();
    }
}


void CDlgTGChangeObjState::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGChangeObjState::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGChangeObjState::SetStateByObjType(int iObjType)
{
    m_cbState.ResetContent();
    switch( iObjType )
    {
        case 0:     // IRON_DOOR
            m_cbState.InsertString(0, L"Indestrutivel");
            m_cbState.AddSearchString(L"Indestrutivel");
            m_cbState.InsertString(1, L"Destrutivel");
            m_cbState.AddSearchString(L"Destrutivel");
            m_cbState.InsertString(2, L"Padrão");
            m_cbState.AddSearchString(L"Padrão");
            m_cbState.InsertString(3, L"Dano");
            m_cbState.AddSearchString(L"Dano");
            m_cbState.InsertString(4, L"Destruido");
            m_cbState.AddSearchString(L"Destruido");

			m_cbType.ResetContent();
			for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT).size();++i)
			{
				m_cbType.InsertString( i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] );
				m_cbType.AddSearchString( SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] );
			}
			if( m_cbType.GetCount() > 0 ) m_cbType.SetCurSel(0);

            break;
		case 1:
			m_cbState.InsertString(0, L"Invencibilidade");
			m_cbState.AddSearchString(L"Invencivel");
			m_cbType.ResetContent();
			break;
		default:
			break;
    }

    if( m_cbState.GetCount() > 0 ) m_cbState.SetCurSel(0);
}
void CDlgTGChangeObjState::OnCbnSelendokComboChangeObjStateObj2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetStateByObjType(m_cbObjType.GetCurSel());

}
