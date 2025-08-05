// DlgTGAddDamage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGAddDamage.h"
#include ".\dlgtgadddamage.h"
#include "DlgTGOptionWaterPole.h"

// CDlgTGAddDamage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGAddDamage, CDlgTGBase)
CDlgTGAddDamage::CDlgTGAddDamage(CWnd* pParent /*=NULL*/)
{
}

CDlgTGAddDamage::~CDlgTGAddDamage()
{
    std::map<DAMAGE_OPTION_TYPE, CDialog*>::iterator itDlg = m_mapDlgOption.begin();
    for(;m_mapDlgOption.end() != itDlg;itDlg++)
    {
        SAFE_DELETE( itDlg->second );
    }
    m_mapDlgOption.clear();
}

void CDlgTGAddDamage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_X, m_ebEntranceX);
	DDX_Control(pDX, IDC_EDIT_Y, m_ebEntranceY);
	DDX_Control(pDX, IDC_COMBO_DAMAGEID, m_cbDamageID);
	DDX_Control(pDX, IDC_EDIT_RECYCLE, m_ebReCycle);
	DDX_Control(pDX, IDC_STATIC_ADD_DAMAGE_1, m_stAddDamage_1);
	DDX_Control(pDX, IDC_STATIC_ADD_DAMAGE_2, m_stAddDamage_2);
	DDX_Control(pDX, IDC_COMBO_DAMAGE_DIRECTION2, m_cbDir);
	DDX_Control(pDX, IDC_EDIT_STARTTIME, m_ebStartTime);
	m_cbDamageID.Init();

	InitValue();

	m_cbDamageID.ResetContent();
	//for(int i=0;i<(int)10;++i)
	//{
	m_cbDamageID.AddSearchString(L"Coluna de Agua");
	m_cbDamageID.InsertString(0, L"Coluna de Agua");
	m_cbDamageID.AddSearchString(L"Fumaça");
	m_cbDamageID.InsertString(1, L"Fumaça");
	m_cbDamageID.AddSearchString(L"Cortina de Fumaça");
	m_cbDamageID.InsertString(2, L"Cortina de Fumaça");		
	m_cbDamageID.AddSearchString(L"Névoa");
	m_cbDamageID.InsertString(3, L"Névoa");	
	m_cbDamageID.AddSearchString(L"Coluna de Fogo");
	m_cbDamageID.InsertString(4, L"Coluna de Fogo");		
	//}
	if( m_cbDamageID.GetCount() > 0 ) m_cbDamageID.SetCurSel(0);

	m_cbDir.ResetContent();
	m_cbDir.InsertString(0, L"Direita");
	m_cbDir.InsertString(1, L"Esquerda");	
	Refresh();		
}


BEGIN_MESSAGE_MAP(CDlgTGAddDamage, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FROM_MAP, OnBnClickedButtonSelectFromMap)
    ON_CBN_SELCHANGE(IDC_COMBO_DAMAGEID, OnCbnSelchangeComboDamageid)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgTGAddDamage 메시지 처리기입니다.

void CDlgTGAddDamage::OnBnClickedButtonSelectFromMap()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_DAMAGEOBJECT_ADD_WINDOW, 0, 0);
        SetFocus();
    }
}

void CDlgTGAddDamage::OnBnClickedOk()
{

}

void CDlgTGAddDamage::OnBnClickedCancel()
{

}

void CDlgTGAddDamage::Refresh(void)
{
    // 매우 하드... -ㅁ-;
    int iIndex = 0;

    if( 51 == m_TGAddDamage.m_iDamageID )
    {
        iIndex = 0;

        CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[DOT_WATER_POLE]);
        pWaterPole->GetValue();
        m_TGAddDamage.m_iLife           = pWaterPole->m_iLife;                
        m_TGAddDamage.m_iContinueance   = pWaterPole->m_iContinueance;
    }
    else if( 288 == m_TGAddDamage.m_iDamageID )
    {
        iIndex = 1;
    }
    else if( 291 == m_TGAddDamage.m_iDamageID )
    {
        iIndex = 2;
    }
	else if(876 == m_TGAddDamage.m_iDamageID ) // 여전히 매우 하드 
	{
		iIndex = 3;
		CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[DOT_WATER_POLE]);
		pWaterPole->GetValue();
		m_TGAddDamage.m_iLife           = pWaterPole->m_iLife;        		      
		m_TGAddDamage.m_iContinueance   = pWaterPole->m_iContinueance;
	}
	else if(40 == m_TGAddDamage.m_iDamageID)
	{
		iIndex = 4;
	}

    if( m_cbDamageID.GetCount() > (int)iIndex ) m_cbDamageID.SetCurSel((int)iIndex);
	OnCbnSelchangeComboDamageid();

    CString str;
    str.Format(L"%d", m_TGAddDamage.m_iEntranceX);
    m_ebEntranceX.SetWindowText(str);
    str.Format(L"%d", m_TGAddDamage.m_iEntranceY);
    m_ebEntranceY.SetWindowText(str);
    str.Format(L"%d", m_TGAddDamage.m_dwRecycleTime);
    m_ebReCycle.SetWindowText(str);

	m_cbDir.SetCurSel(m_TGAddDamage.m_iDir);
	str.Format(L"%d", m_TGAddDamage.m_iStartTime);
	m_ebStartTime.SetWindowText(str);	
}

void CDlgTGAddDamage::CalcResult(void)
{
    CString strDamageID = L"X";

    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        if( m_cbDamageID.GetCurSel() >= 0 && m_cbDamageID.GetCount() > m_cbDamageID.GetCurSel() )
            m_cbDamageID.GetLBText(m_cbDamageID.GetCurSel(), strDamageID);

        CString strX, strY, strReCycle;
        m_ebEntranceX.GetWindowText(strX);
        m_ebEntranceY.GetWindowText(strY);
        m_ebReCycle.GetWindowText(strReCycle);

        CString strText1, strText2;
        m_stAddDamage_1.GetWindowText(strText1);
        m_stAddDamage_2.GetWindowText(strText2);

        CString strPosition;
        strPosition.Format(L"X : %s, Y : %s, Term : %s", strX, strY, strReCycle);

        m_strDesc = MiddleBraceSidebyString(strPosition) + L" " + strText1 + L" " + MiddleBraceSidebyString(strDamageID)
            + L" " + strText2;
    }


    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        // 매우 하드한 코딩...
        // 일정때문에 훗날 되게 만들어 줄게...
        int iIndex = 0;
        if( L"Coluna de Agua" == strDamageID )
        {
            iIndex = 51;
        }
        else if( L"Fumaça" == strDamageID )
        {
            iIndex = 288;
        }
        else if( L"Cortina de Fumaça" == strDamageID )
        {
            iIndex = 291;
        }
		else if(L"Coluna de Fogo" == strDamageID)
		{
			iIndex = 876;
		}
		else if(L"Bola de Fogo" == strDamageID)
		{
			iIndex = 40;
		}
		
		

        if( L"Coluna de Agua" == strDamageID || 
			L"Coluna de Fogo" == strDamageID )
        {
            CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[DOT_WATER_POLE]);
            pWaterPole->GetValue();
            m_TGAddDamage.m_iLife           = pWaterPole->m_iLife;            
            m_TGAddDamage.m_iContinueance   = pWaterPole->m_iContinueance;
        }

        m_TGAddDamage.m_iDamageID = iIndex;

        CString strX, strY, strReCycle,strT;
        m_ebEntranceX.GetWindowText(strX);
        m_ebEntranceY.GetWindowText(strY);
        m_ebReCycle.GetWindowText(strReCycle);
		m_ebStartTime.GetWindowText(strT);

        m_TGAddDamage.m_iEntranceX = _wtoi(strX);
        m_TGAddDamage.m_iEntranceY = _wtoi(strY);
        m_TGAddDamage.m_dwRecycleTime   = (DWORD)_wtoi(strReCycle);
        m_TGAddDamage.m_strDesc = GetDesc();
		m_TGAddDamage.m_iDir	 =  m_cbDir.GetCurSel();
		m_TGAddDamage.m_iStartTime   = _wtoi(strT);
    }
}

void CDlgTGAddDamage::OnCbnSelchangeComboDamageid()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strDamageID = L"X";

    if( m_cbDamageID.GetCurSel() >= 0 && m_cbDamageID.GetCount() > m_cbDamageID.GetCurSel() )
        m_cbDamageID.GetLBText(m_cbDamageID.GetCurSel(), strDamageID);
    
    if (L"Coluna de Agua" == strDamageID ||
        L"Coluna de Fogo" == strDamageID)
	{
        m_mapDlgOption[DOT_WATER_POLE]->ShowWindow( SW_SHOW );		
    }
	else
	{
		m_mapDlgOption[DOT_WATER_POLE]->ShowWindow( SW_HIDE );	
	}
}

int CDlgTGAddDamage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDlgTGBase::OnCreate(lpCreateStruct) == -1)
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
            rtWindow.OffsetRect(0, 120);
            pOptionWater->MoveWindow(rtWindow, 1);
        }
    }


    m_mapDlgOption.clear();
    m_mapDlgOption.insert( std::make_pair(DOT_WATER_POLE, pOptionWater) );


    // 51번이 물기둥... HardCoding.!!
    if( 51 == m_TGAddDamage.m_iDamageID || 876 == m_TGAddDamage.m_iDamageID)
        //if( "물기둥" == strDamageID )
    {
        CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[CDlgTGAddDamage::DOT_WATER_POLE]);
        pWaterPole->SetValue( m_TGAddDamage.m_iLife, m_TGAddDamage.m_iStartTime
                            , m_TGAddDamage.m_iCrashStart, m_TGAddDamage.m_iMySpeed
                            , m_TGAddDamage.m_iJumpFrame, m_TGAddDamage.m_iContinueance );
    }

    return 0;
}

void CDlgTGAddDamage::GetValue(CBaseTGFunction *pValue)
{
    *(CAddDamage*)pValue = m_TGAddDamage; 

    // hardcoding
    if( 51 == m_TGAddDamage.m_iDamageID || 876 == m_TGAddDamage.m_iDamageID)
    {
        CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[DOT_WATER_POLE]);
        pWaterPole->GetValue();
        m_TGAddDamage.m_iLife           = pWaterPole->m_iLife;        
        m_TGAddDamage.m_iContinueance   = pWaterPole->m_iContinueance;
    }
}

void CDlgTGAddDamage::SetValue(CBaseTGFunction *pValue)
{
    m_TGAddDamage = *(CAddDamage*)pValue;

    // hardcoding
    if( 51 == m_TGAddDamage.m_iDamageID  || 876 == m_TGAddDamage.m_iDamageID)
    {
        CDlgTGOptionWaterPole *pWaterPole = static_cast<CDlgTGOptionWaterPole*>(m_mapDlgOption[DOT_WATER_POLE]);
        pWaterPole->SetValue( m_TGAddDamage.m_iLife, m_TGAddDamage.m_iStartTime
                            , m_TGAddDamage.m_iCrashStart, m_TGAddDamage.m_iMySpeed
                            , m_TGAddDamage.m_iJumpFrame, m_TGAddDamage.m_iContinueance );
    }
}
