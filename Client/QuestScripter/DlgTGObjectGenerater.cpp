// DlgTGObjectGenerater.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGObjectGenerater.h"
#include ".\dlgtgobjectgenerater.h"
#include "ObjectDataManager.h"

// CDlgTGObjectGenerater 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGObjectGenerater, CDlgTGBase)
CDlgTGObjectGenerater::CDlgTGObjectGenerater(CWnd* pParent /*=NULL*/)
{
}

CDlgTGObjectGenerater::~CDlgTGObjectGenerater()
{
}

void CDlgTGObjectGenerater::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OBJ_SLOT, m_cbSlot);
	DDX_Control(pDX, IDC_COMBO_OBJ_TYPE, m_cbType);
	DDX_Control(pDX, IDC_COMBO_OBJ_RIGHT, m_cbRight);
	DDX_Control(pDX, IDC_EDIT_OBJ_X, m_ebEntranceX);
	DDX_Control(pDX, IDC_EDIT_OBJ_Y, m_ebEntranceY);
	DDX_Control(pDX, IDC_COMBO_OBJ_GEN, m_cbGen);
	DDX_Control(pDX, IDC_STATIC_OBJGEN_1, m_stObjGen1);
	DDX_Control(pDX, IDC_STATIC_OBJGEN_2, m_stObjGen2);
	DDX_Control(pDX, IDC_STATIC_OBJGEN_3, m_stObjGen3);
	DDX_Control(pDX, IDC_STATIC_OBJGEN_4, m_stObjGen4);
	DDX_Control(pDX, IDC_COMBO_CRASHABLE, m_cbCrashable);
	DDX_Control(pDX, IDC_EDIT_OBJ_Jump, m_edJump);

	m_cbType.Init();

	m_cbSlot.ResetContent();
	for(int i=0;i<MAX_SLOT_MONSTER;i++)
	{
		CString strText;
		strText.Format(L"%d", i);
		m_cbSlot.InsertString(i, strText);
	}
	if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

	m_cbType.ResetContent();
	for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_OBJECT ).size();++i)
	{
		m_cbType.InsertString( i, SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_OBJECT )[i] );
		m_cbType.AddSearchString( SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_OBJECT )[i] );
	}
	if( m_cbType.GetCount() > 0 ) m_cbType.SetCurSel(0);

	m_cbRight.ResetContent();
	m_cbRight.InsertString(0, L"왼쪽");
	m_cbRight.InsertString(1, L"오른쪽");
	if( m_cbRight.GetCount() > 0 ) m_cbRight.SetCurSel(0);

	m_ebEntranceX.SetWindowText(L"");
	m_ebEntranceY.SetWindowText(L"");

	m_cbCrashable.ResetContent();
	m_cbCrashable.InsertString(0, L"기본 형태로");
	m_cbCrashable.InsertString(1, L"파괴가능 형태로");
	if( m_cbCrashable.GetCount() > 0 ) m_cbCrashable.SetCurSel(0);

	m_cbGen.ResetContent();
	m_cbGen.InsertString(0, L"제거합니다.");
	m_cbGen.InsertString(1, L"생성합니다.");
	if( m_cbGen.GetCount() > 1 ) m_cbGen.SetCurSel(1);	
}


BEGIN_MESSAGE_MAP(CDlgTGObjectGenerater, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ_TYPE, OnCbnSelchangeComboObjType)
END_MESSAGE_MAP()


// CDlgTGObjectGenerater 메시지 처리기입니다.

void CDlgTGObjectGenerater::Refresh(void)
{
    if( m_cbRight.GetCount() > (int)m_TGObjectGenerater.m_bRight ) m_cbRight.SetCurSel(m_TGObjectGenerater.m_bRight);
    if( m_cbSlot.GetCount() > m_TGObjectGenerater.m_iSlot ) m_cbSlot.SetCurSel(m_TGObjectGenerater.m_iSlot);
    if( m_cbType.GetCount() > m_TGObjectGenerater.m_iType ) m_cbType.SetCurSel(m_TGObjectGenerater.m_iType);
    if( m_cbGen.GetCount() > (int)m_TGObjectGenerater.m_bGenerate ) m_cbGen.SetCurSel((int)m_TGObjectGenerater.m_bGenerate);
    if( m_cbCrashable.GetCount() > (int)m_TGObjectGenerater.m_bCrashAble ) m_cbCrashable.SetCurSel(m_TGObjectGenerater.m_bCrashAble);

    CString strX, strY;
    strX.Format(L"%f", m_TGObjectGenerater.m_fEntranceX);
    strY.Format(L"%f", m_TGObjectGenerater.m_fEntranceY);
    m_ebEntranceX.SetWindowText(strX);
    m_ebEntranceY.SetWindowText(strY);
	CString strj;
	strj.Format(L"%f", m_TGObjectGenerater.m_fProperty);	
	m_edJump.SetWindowText(strj);
}

void CDlgTGObjectGenerater::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strSlot = L"X";
        CString strObj = L"X";
        if( m_cbSlot.GetCurSel() >= 0 )
            m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);

        m_cbType.GetWindowText(strObj);        
        int index = m_cbType.FindStringExact(0, strObj);
        if( CB_ERR != index )
            m_cbType.SetCurSel(index);
        else
            strObj = L"X";

        CString strRight;
        if( m_cbRight.GetCount() > m_cbRight.GetCurSel() ) m_cbRight.GetLBText(m_cbRight.GetCurSel(), strRight);

        CString strX, strY,strP;
        m_ebEntranceX.GetWindowText(strX);
        m_ebEntranceY.GetWindowText(strY);
		m_edJump.GetWindowText(strP);

        CString strGen;
        m_cbGen.GetWindowText(strGen);        
        index = m_cbGen.FindStringExact(0, strGen);
        if( CB_ERR != index )
            m_cbGen.SetCurSel(index);
        else
            strGen = "X";

        CString strCrashAble;
        m_cbCrashable.GetWindowText(strCrashAble);
        index = m_cbCrashable.FindStringExact(0, strCrashAble);
        if( CB_ERR != index )
            m_cbCrashable.SetCurSel(index);
        else
            strCrashAble = L"X";

        CString strText1, strText2, strText3, strText4, strText5;
        m_stObjGen1.GetWindowText(strText1);
        m_stObjGen2.GetWindowText(strText2);
        m_stObjGen3.GetWindowText(strText3);
        m_stObjGen4.GetWindowText(strText4);

        m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 + L" " + MiddleBraceSidebyString(strObj) + L" " + strText2 
            + L" " + MiddleBraceSidebyString(strRight) + strText3
            + L" (" + MiddleBraceSidebyString(strX) + L"," + MiddleBraceSidebyString(strY) + L") " + strText4 + L" " +
            + MiddleBraceSidebyString(strCrashAble) + L" " + MiddleBraceSidebyString(strGen);
    }


    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        m_TGObjectGenerater.m_bRight = (bool)m_cbRight.GetCurSel();
        m_TGObjectGenerater.m_iSlot = m_cbSlot.GetCurSel();
        m_TGObjectGenerater.m_iType = m_cbType.GetCurSel();
        m_TGObjectGenerater.m_bGenerate = m_cbGen.GetCurSel();
        m_TGObjectGenerater.m_bCrashAble = m_cbCrashable.GetCurSel();

        CString strX, strY, strP;
        m_ebEntranceX.GetWindowText(strX);
        m_ebEntranceY.GetWindowText(strY);		
		m_edJump.GetWindowText(strP);

        m_TGObjectGenerater.m_fEntranceX = (float)_wtof(strX);
        m_TGObjectGenerater.m_fEntranceY = (float)_wtof(strY);
		m_TGObjectGenerater.m_fProperty = (float)_wtof(strP);
        m_TGObjectGenerater.m_strDesc = GetDesc();
    }
}


void CDlgTGObjectGenerater::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGObjectGenerater::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGObjectGenerater::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_OBJECT_ADD_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGObjectGenerater::OnCbnSelchangeComboObjType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
