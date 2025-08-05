// DlgTGObjDestroy.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGObjDestroy.h"
#include "ObjectDataManager.h"

// CDlgTGObjDestroy 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGObjDestroy, CDialog)
CDlgTGObjDestroy::CDlgTGObjDestroy(CWnd* pParent /*=NULL*/)
{
}

CDlgTGObjDestroy::~CDlgTGObjDestroy()
{
}

void CDlgTGObjDestroy::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_OBJ, m_cbSlot);
    DDX_Control(pDX, IDC_COMBO_OBJ_DESTROYED_TYPE, m_cbType);
    DDX_Control(pDX, IDC_STATIC_OBJ_DESTROYED_1, m_stObjDestroy);
    DDX_Control(pDX, IDC_STATIC_OBJ_DESTROYED_2, m_stObjDestroy2);

    m_cbSlot.ResetContent();
    for(int i=0;i<MAX_SLOT_OBJECT;++i)
    {
        CString strSlot;
        strSlot.Format(L"%d", i);
        m_cbSlot.InsertString(i, strSlot);
    }
    if( m_cbSlot.GetCount() > 0 ) m_cbSlot.SetCurSel(0);

    m_cbType.Init();
    m_cbType.ResetContent();
    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT).size();++i)
    {
        m_cbType.InsertString( i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] );
        m_cbType.AddSearchString( SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] );
    }
    if( m_cbType.GetCount() > 0 ) m_cbType.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CDlgTGObjDestroy, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGObjDestroy 메시지 처리기입니다.

void CDlgTGObjDestroy::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGObjDestroy::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgTGObjDestroy::Refresh(void)
{
    if( m_cbSlot.GetCount() > m_ObjDestroy.m_iObjIndex ) m_cbSlot.SetCurSel(m_ObjDestroy.m_iObjIndex);
    if( m_cbType.GetCount() > m_ObjDestroy.m_iObjType ) m_cbType.SetCurSel(m_ObjDestroy.m_iObjType);
}

void CDlgTGObjDestroy::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strSlot = L"X";
        CString strType = L"X";
        if( m_cbSlot.GetCurSel() >= 0 )
            m_cbSlot.GetLBText(m_cbSlot.GetCurSel(), strSlot);
        if( m_cbType.GetCurSel() >= 0 )
        {
            if( (int)SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_OBJECT ).size() > m_cbType.GetCurSel() )
                strType = SiCObjectDataManager()->GetEnumItem( OBJECTTYPE_EACTION_OBJECT )[m_cbType.GetCurSel()];
        }

        CString strText1, strText2;
        m_stObjDestroy.GetWindowText(strText1);
        m_stObjDestroy2.GetWindowText(strText2);
        m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 + L" " + MiddleBraceSidebyString(strType) + L" " + strText2;
    }

    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        CString strType;
        m_cbType.GetWindowText(strType);

        int iType = 0;
        for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT).size();++i)
        {
            if( strType == SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] )
            {
                iType = i;
                break;
            }
        }

        m_ObjDestroy.m_iObjIndex = m_cbSlot.GetCurSel();
        m_ObjDestroy.m_iObjType = iType;
        m_ObjDestroy.m_strDesc = GetDesc();
    }
}