// AddDamage_Popup.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "QuestScripter.h"
#include "AddDamage_Popup.h"
#include "ObjectDataManager.h"
#include "DlgTriggerContent.h"
#include "DlgMapToObject.h"
#include "QuestScripterDlg.h"
#include "DlgTGOptionWaterPole.h"

// CAddDamage_Popup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddDamage_Popup, CDialog)
CAddDamage_Popup::CAddDamage_Popup(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDamage_Popup::IDD, pParent)
{
    std::map<CDlgTGAddDamage::DAMAGE_OPTION_TYPE, CDialog*>::iterator itDlg = m_mapDlgOption.begin();
    for(;m_mapDlgOption.end() != itDlg;itDlg++)
    {
        SAFE_DELETE( itDlg->second );
    }
    m_mapDlgOption.clear();

    m_iDefWidth = 0;
}

CAddDamage_Popup::~CAddDamage_Popup()
{
}

void CAddDamage_Popup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ID, m_cbID);
	DDX_Control(pDX, IDC_EDIT_ENTRANCEX, m_ebEntranceX);
	DDX_Control(pDX, IDC_EDIT_ENTRANCEY, m_ebEntranceY);
	DDX_Control(pDX, IDC_EDIT_RECYCLETIME, m_ebRecycleTime);
	DDX_Control(pDX, IDC_COMBO_EACTION, m_cbFunctionType);
	DDX_Control(pDX, IDC_COMBO_DAMAGE_DIRECTION2, m_cbDir);
	DDX_Control(pDX, IDC_EDIT_STARTTIME2, m_ebStartTime);

	m_cbID.ResetContent();
	for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DAMAGE).size();i++)
	{
		m_cbID.InsertString(i, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DAMAGE)[i]);
	}

	if( 51 == m_AddDamage.m_iDamageID )
	{
		m_cbID.SetCurSel(0);
	}
	else if( 288 == m_AddDamage.m_iDamageID )
	{
		m_cbID.SetCurSel(1);
	}
	else if( 291 == m_AddDamage.m_iDamageID )
	{
		m_cbID.SetCurSel(2);
	}
	else if(876 == m_AddDamage.m_iDamageID)
	{
		m_cbID.SetCurSel(3);
	}
	else if(40 == m_AddDamage.m_iDamageID)
	{
		m_cbID.SetCurSel(4);
	}
	else
	{
		m_cbID.SetCurSel(0);
	}

	OnCbnSelchangeComboId();

	CString str;
	str.Format(L"%d", m_AddDamage.m_iEntranceX);
	m_ebEntranceX.SetWindowText(str);
	str.Format(L"%d", m_AddDamage.m_iEntranceY);
	m_ebEntranceY.SetWindowText(str);
	str.Format(L"%d", m_AddDamage.m_dwRecycleTime);
	m_ebRecycleTime.SetWindowText(str);
	str.Format(L"%d", m_AddDamage.m_iStartTime);
	m_ebStartTime.SetWindowText(str);

	m_cbFunctionType.ResetContent();
	m_cbFunctionType.InsertString(0, L"Event");
	m_cbFunctionType.InsertString(1, L"Action");
	if( m_cbFunctionType.GetCount() > 1 ) m_cbFunctionType.SetCurSel(1);

	m_cbDir.ResetContent();
	m_cbDir.InsertString(0, L"오른쪽");
	m_cbDir.InsertString(1, L"왼쪽");
	m_cbDir.SetCurSel(m_AddDamage.m_iDir);
	
}


BEGIN_MESSAGE_MAP(CAddDamage_Popup, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
    ON_CBN_SELCHANGE(IDC_COMBO_ID, OnCbnSelchangeComboId)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CAddDamage_Popup 메시지 처리기입니다.
void CAddDamage_Popup::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    CString strDamageID;
    if( m_cbID.GetCurSel() >= 0 && m_cbID.GetCount() > m_cbID.GetCurSel() )
        m_cbID.GetLBText(m_cbID.GetCurSel(), strDamageID);

    CString strX, strY, strReCycle, strT;
    m_ebEntranceX.GetWindowText(strX);
    m_ebEntranceY.GetWindowText(strY);
    m_ebRecycleTime.GetWindowText(strReCycle);
	m_ebStartTime.GetWindowText(strT);


    // 매우 하드한 코딩...
    // 일정때문에 훗날 되게 만들어 줄게...

	// 훗날 받은 사람도 하드코딩
	// 나중에 누군가 만들어 줘요 
    int iIndex = 0;
    if( "물기둥" == strDamageID )
    {
        iIndex = 51;
    }
    else if( "가스" == strDamageID )
    {
        iIndex = 288;
    }
    else if( "큰가스" == strDamageID )
    {
        iIndex = 291;
    }
	else if("불기둥"==strDamageID)
	{
		iIndex = 876;
	}
	else if("파이어볼" == strDamageID)
	{
		iIndex = 40;
	}

	if( "물기둥" == strDamageID || 
		"불기둥" == strDamageID )		
	{
        CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[CDlgTGAddDamage::DOT_WATER_POLE]);
        pWaterPole->GetValue();
        m_AddDamage.m_iLife           = pWaterPole->m_iLife;
        m_AddDamage.m_iContinueance   = pWaterPole->m_iContinueance;		    
    }	
	
	m_AddDamage.m_iStartTime		= _wtoi(strT);
    m_AddDamage.m_iDamageID         = iIndex;
    m_AddDamage.m_iEntranceX        = _wtoi(strX);
    m_AddDamage.m_iEntranceY        = _wtoi(strY);
    m_AddDamage.m_dwRecycleTime     = _wtoi(strReCycle);
	m_AddDamage.m_iDir				= m_cbDir.GetCurSel();	
    m_bFnAction = m_cbFunctionType.GetCurSel();

    OnOK();
}

void CAddDamage_Popup::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void CAddDamage_Popup::SetMapDamageList(std::vector<CAddDamage*> *pvtDamageList)
{
    m_pvtDamageList = pvtDamageList;
}

void CAddDamage_Popup::OnBnClickedDelete()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( !m_pvtDamageList ) return;

    for(int i=0;i<(int)m_pvtDamageList->size();i++)
    {
        if( m_AddDamage.m_hItem == (*m_pvtDamageList)[i]->m_hItem )
        {
            m_pvtDamageList->erase((m_pvtDamageList->begin()+i));
            CQuestScripterDlg *pDlgQuestScript = ((CQuestScripterDlg*)AfxGetMainWnd());
            if( pDlgQuestScript )
            {
                if( !pDlgQuestScript->m_dlgMapToObject.m_pDlgTGContent->DelActionItem(m_AddDamage.m_hItem) )
                    pDlgQuestScript->m_dlgMapToObject.m_pDlgTGContent->DelEventItem(m_AddDamage.m_hItem);
            }
            break;
        }
    }

    OnCancel();
}

void CAddDamage_Popup::OnCbnSelchangeComboId()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    CString strDamageID = L"X";

    if( m_cbID.GetCurSel() >= 0 && m_cbID.GetCount() > m_cbID.GetCurSel() )
        m_cbID.GetLBText(m_cbID.GetCurSel(), strDamageID);

    CRect rtWindow;
	if( L"물기둥" == strDamageID || 
		L"불기둥" == strDamageID )
	{
        m_mapDlgOption[CDlgTGAddDamage::DOT_WATER_POLE]->ShowWindow( SW_SHOW );		

        GetWindowRect(rtWindow);
        rtWindow.right = rtWindow.left + m_iDefWidth + 330;
        MoveWindow(rtWindow, 1);
    }
	
    else
    {
        m_mapDlgOption[CDlgTGAddDamage::DOT_WATER_POLE]->ShowWindow( SW_HIDE );	
		
        GetWindowRect(rtWindow);
        rtWindow.right = rtWindow.left + m_iDefWidth;
        MoveWindow(rtWindow, 1);
    }
}

int CAddDamage_Popup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.

    CDlgTGOptionWaterPole *pOptionWater = new CDlgTGOptionWaterPole;
    if( pOptionWater )
    {
        if( -1 != pOptionWater->Create(CDlgTGOptionWaterPole::IDD, this) )
        {
            CRect rtWindow;
            pOptionWater->GetWindowRect(rtWindow);
            ScreenToClient(rtWindow);
            rtWindow.OffsetRect(300, 0);
            pOptionWater->MoveWindow(rtWindow, 1);
        }
    }


    m_mapDlgOption.clear();
    m_mapDlgOption.insert( std::make_pair(CDlgTGAddDamage::DOT_WATER_POLE, pOptionWater) );

    CRect rtWindow;
    GetWindowRect(rtWindow);
    m_iDefWidth = rtWindow.Width();
    
    if( 51 == m_AddDamage.m_iDamageID ||
		876 == m_AddDamage.m_iDamageID )    
    {
        CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[CDlgTGAddDamage::DOT_WATER_POLE]);
        pWaterPole->SetValue( m_AddDamage.m_iLife, m_AddDamage.m_iStartTime
                            , m_AddDamage.m_iCrashStart, m_AddDamage.m_iMySpeed
                            , m_AddDamage.m_iJumpFrame, m_AddDamage.m_iContinueance );
    }


    return 0;
}
