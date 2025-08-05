// DlgTGWater.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGWater.h"
#include ".\dlgtgwater.h"


// CDlgTGWater 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGWater, CDlgTGBase)
CDlgTGWater::CDlgTGWater(CWnd* pParent /*=NULL*/)
{
}

CDlgTGWater::~CDlgTGWater()
{
}

void CDlgTGWater::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_ebWaterHeight);
    DDX_Control(pDX, IDC_COMBO_ONOFF, m_cbWaterOnOff);

    InitValue();

    CString strHeight;
    strHeight.Format(L"%d", m_TGWater.m_iWaterHeight);
    m_ebWaterHeight.SetWindowText(strHeight);

    m_cbWaterOnOff.ResetContent();
    m_cbWaterOnOff.InsertString(0, L"물을 표시하지 않습니다..");
    m_cbWaterOnOff.InsertString(1, L"물을 표시 합니다.");
    if( m_cbWaterOnOff.GetCount() > 0 ) m_cbWaterOnOff.SetCurSel(0);

    Refresh();
}


BEGIN_MESSAGE_MAP(CDlgTGWater, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_MAP_ON_HEIGHT, OnBnClickedButtonMapOnHeight)
END_MESSAGE_MAP()


// CDlgTGWater 메시지 처리기입니다.

void CDlgTGWater::Refresh(void)
{
    CString strHeight;
    strHeight.Format(L"%d", m_TGWater.m_iWaterHeight);
    m_ebWaterHeight.SetWindowText(strHeight);

    if( m_cbWaterOnOff.GetCount() > (int)m_TGWater.m_bOnOff ) m_cbWaterOnOff.SetCurSel( (int)m_TGWater.m_bOnOff );
}

void CDlgTGWater::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strWaterOnOff = L"X";
        if( m_cbWaterOnOff.GetCurSel() >= 0 && m_cbWaterOnOff.GetCount() > m_cbWaterOnOff.GetCurSel() )
            m_cbWaterOnOff.GetLBText(m_cbWaterOnOff.GetCurSel(), strWaterOnOff);

        CString strWaterHeight;
        m_ebWaterHeight.GetWindowText(strWaterHeight);

        
        CString strResult;
        strResult.Format(L"[%s] 높이의 물을 [%s]", strWaterHeight, strWaterOnOff);
        m_strDesc = strResult;
    }


    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        CString strWaterHeight;
        m_ebWaterHeight.GetWindowText(strWaterHeight);

        m_TGWater.m_bOnOff          = (bool)m_cbWaterOnOff.GetCurSel();
        m_TGWater.m_iWaterHeight    = _wtoi(strWaterHeight);
        m_TGWater.m_strDesc = GetDesc();
    }
}

void CDlgTGWater::OnBnClickedOk()
{

}

void CDlgTGWater::OnBnClickedCancel()
{

}

void CDlgTGWater::OnBnClickedButtonMapOnHeight()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( AfxGetMainWnd() )
    {
        CalcResult();
        // GetParent()->GetParent() 는 CDlgTGChildGenner 클래스입니다.
        AfxGetMainWnd()->SendMessage(WM_POS_SEL_WINDOW, m_TGWater.m_iWaterHeight, 0);
        SetFocus();
    }
}
