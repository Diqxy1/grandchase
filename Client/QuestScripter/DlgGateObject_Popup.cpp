// DlgGateObject_Popup.cpp : 구현 파일입니다.
//
#include "stdafx.h"

#include "DlgGateObject_Popup.h"
#include "QuestScripter.h"
#include "DlgMapToObject.h"
#include "DlgTriggerContent.h"
#include "ObjectDataManager.h"
#include "QuestScripterDlg.h"

// CDlgGateObject_Popup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgGateObject_Popup, CDialog)
CDlgGateObject_Popup::CDlgGateObject_Popup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGateObject_Popup::IDD, pParent)
{
}

CDlgGateObject_Popup::~CDlgGateObject_Popup()
{
}

void CDlgGateObject_Popup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DIRECTION, m_cbDirection);
    DDX_Control(pDX, IDC_COMBO_ID, m_cbType);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEX, m_ebEntranceX);
    DDX_Control(pDX, IDC_EDIT_ENTRANCEY, m_ebEntranceY);
    DDX_Control(pDX, IDC_COMBO_DIRECTION2, m_cbRight);
    DDX_Control(pDX, IDC_COMBO_GENERATE, m_cbGenerate);
    DDX_Control(pDX, IDC_COMBO_EACTION, m_cbFunctionType);
    DDX_Control(pDX, IDC_EDIT_DELAY, m_ebDelay);

    m_cbRight.InsertString(0, L"왼쪽");
    m_cbRight.InsertString(1, L"오른쪽");
    if( m_cbRight.GetCount() > 0 )
        m_cbRight.SetCurSel(0);

    for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DOOR).size();i++)
    {
        m_cbType.InsertString(i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DOOR)[i]);
    }
    if( m_cbType.GetCount() > 0 )
        m_cbType.SetCurSel(0);

    m_cbGenerate.ResetContent();
    m_cbGenerate.InsertString(0, L"제거합니다.");
    m_cbGenerate.InsertString(1, L"생성합니다.");

    m_cbFunctionType.ResetContent();
    m_cbFunctionType.InsertString(0, L"Event");
    m_cbFunctionType.InsertString(1, L"Action");

    m_cbDirection.ResetContent();
    m_cbDirection.InsertString(0, L"왼쪽문 이동");
    m_cbDirection.InsertString(1, L"위쪽문 이동");
    m_cbDirection.InsertString(2, L"오른쪽문 이동");
    m_cbDirection.InsertString(3, L"아래쪽문 이동");
    if( m_cbDirection.GetCount() > 1 ) m_cbDirection.SetCurSel(0);

    CString str;
    str.Format(L"%f", m_GateObject.m_fEntranceX);
    m_ebEntranceX.SetWindowText(str);
    str.Format(L"%f", m_GateObject.m_fEntranceY);
    m_ebEntranceY.SetWindowText(str);
    str.Format(L"%d", m_GateObject.m_iDelayTime);
    m_ebDelay.SetWindowText(str);
    if( m_cbType.GetCount() > m_GateObject.m_iType ) m_cbType.SetCurSel(m_GateObject.m_iType);
    if( m_cbDirection.GetCount() > ConvertToComboBoxNumber(m_GateObject.m_iDirection) )
        m_cbDirection.SetCurSel( ConvertToComboBoxNumber(m_GateObject.m_iDirection) );
    if( m_cbRight.GetCount() > (int)m_GateObject.m_bRight ) m_cbRight.SetCurSel((int)m_GateObject.m_bRight);
    if( m_cbGenerate.GetCount() > (int)m_GateObject.m_bGenerate ) m_cbGenerate.SetCurSel((int)m_GateObject.m_bGenerate);
    if( m_cbFunctionType.GetCount() > 1 ) m_cbFunctionType.SetCurSel(1);
}


BEGIN_MESSAGE_MAP(CDlgGateObject_Popup, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


// CDlgGateObject_Popup 메시지 처리기입니다.
void CDlgGateObject_Popup::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_GateObject.m_bGenerate       = m_cbGenerate.GetCurSel();
    m_GateObject.m_iType           = m_cbType.GetCurSel();
    m_GateObject.m_iDirection      = ConvertToImageNumber( m_cbDirection.GetCurSel() );

    CString str;
    m_ebEntranceX.GetWindowText(str);
    m_GateObject.m_fEntranceX = (float)_wtof(str);

    m_ebEntranceY.GetWindowText(str);
    m_GateObject.m_fEntranceY = (float)_wtof(str);

    m_ebDelay.GetWindowText(str);
    m_GateObject.m_iDelayTime = (int)_wtoi(str);

    m_GateObject.m_bRight = m_cbRight.GetCurSel();
    m_bFnAction = m_cbFunctionType.GetCurSel();

    OnOK();
}

void CDlgGateObject_Popup::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void CDlgGateObject_Popup::SetMapObjectList(std::vector<CGateObjectGenerater*> *pvtGateObjList)
{
    m_pvtGateObjList = pvtGateObjList;
}

void CDlgGateObject_Popup::OnBnClickedDelete()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( !m_pvtGateObjList ) return;

    for(int i=0;i<(int)m_pvtGateObjList->size();i++)
    {
        if( m_GateObject.m_hItem == (*m_pvtGateObjList)[i]->m_hItem )
        {
            m_pvtGateObjList->erase((m_pvtGateObjList->begin()+i));
            CQuestScripterDlg *pDlgQuestScript = ((CQuestScripterDlg*)AfxGetMainWnd());
            if( pDlgQuestScript )
            {
                if( !pDlgQuestScript->m_dlgMapToObject.m_pDlgTGContent->DelActionItem(m_GateObject.m_hItem) )
                    pDlgQuestScript->m_dlgMapToObject.m_pDlgTGContent->DelEventItem(m_GateObject.m_hItem);
            }
            break;
        }
    }

    OnCancel();
}

// 이미지 넘버를 넣으면 콤보박스 번호로 리턴해준다.
int CDlgGateObject_Popup::ConvertToComboBoxNumber(int iGateNum)
{
    switch(iGateNum)
    {
    case DIRECTION_LEFT:             // left
        return 0;
        break;
    case DIRECTION_UP:                // top
        return 1;
        break;
    case DIRECTION_RIGHT:             // right
        return 2;
        break;
    case DIRECTION_DOWN:             // bottom
        return 3;
        break;
    }

    return 0;
}


// 콤보박스의 0, 1, 2, 3을 기준으로 left right up down 값을 판단하여 리턴
int CDlgGateObject_Popup::ConvertToImageNumber(int iGateNum)
{
    switch(iGateNum)
    {
    case 0:             // left
		iGateNum = DIRECTION_LEFT;
		break;
    case 1:             // top
		iGateNum = DIRECTION_UP;
		break;
   case 2:             // right
		iGateNum = DIRECTION_RIGHT;
		break;
   case 3:             // bottom
	   iGateNum = DIRECTION_DOWN;
	   break;
   default:
		iGateNum = 0;
		break;
    }

    return iGateNum;
}