// DlgTGWeather.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGWall.h"
#include "ObjectDataManager.h"

// CDlgTGWeather 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGWall, CDlgTGBase)
CDlgTGWall::CDlgTGWall(CWnd* pParent /*=NULL*/)
{
}

CDlgTGWall::~CDlgTGWall()
{
}

void CDlgTGWall::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_WALL_TYPE, m_ebWallType);
    DDX_Control(pDX, IDC_EDIT_WALL_SLOT, m_ebWallSlot);
    DDX_Control(pDX, IDC_EDIT_WALL_POS_X, m_ebPosX);
    DDX_Control(pDX, IDC_COMBO_WALL_IS_DESTROY, m_cbIsDestroy);
    DDX_Control(pDX, IDC_COMBO_WALL_IS_CHECK_INBOUND, m_cbIsCheckInBound);

    CString strTemp;
    strTemp.Format(L"%d", m_TGWall.m_iWallType);
    m_ebWallType.SetWindowText( strTemp );
    strTemp.Format(L"%d", m_TGWall.m_iWallSlot);
    m_ebWallSlot.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGWall.m_fPosX);
    m_ebPosX.SetWindowText( strTemp );

    m_cbIsDestroy.ResetContent();
    m_cbIsDestroy.InsertString( 0, L"벽 설치");
    m_cbIsDestroy.InsertString( 1, L"벽 제거");
    if( m_TGWall.m_bDestroy )
        m_cbIsDestroy.SetCurSel( 1 );
    else
        m_cbIsDestroy.SetCurSel( 0 );

    m_cbIsCheckInBound.ResetContent();
    m_cbIsCheckInBound.InsertString( 0, L"Check BoundChar");
    m_cbIsCheckInBound.InsertString( 1, L"Not Check BoundChar");
    if( m_TGWall.m_bIsCheckInBound )
        m_cbIsCheckInBound.SetCurSel( 1 );
    else
        m_cbIsCheckInBound.SetCurSel( 0 );
    
}


BEGIN_MESSAGE_MAP(CDlgTGWall, CDialog)
END_MESSAGE_MAP()


// CDlgTGWall 메시지 처리기
void CDlgTGWall::Refresh(void)
{
    CString strTemp;
    strTemp.Format(L"%d", m_TGWall.m_iWallType);
    m_ebWallType.SetWindowText( strTemp );
    strTemp.Format(L"%d", m_TGWall.m_iWallSlot);
    m_ebWallSlot.SetWindowText( strTemp );
    strTemp.Format(L"%f", m_TGWall.m_fPosX);
    m_ebPosX.SetWindowText( strTemp );

    if( m_cbIsDestroy.GetCount() > 1 )
    {
        if( m_TGWall.m_bDestroy )
            m_cbIsDestroy.SetCurSel( 1 );
        else
            m_cbIsDestroy.SetCurSel( 0 );
    }

    if( m_cbIsCheckInBound.GetCount() > 1 )
    {
        if( m_TGWall.m_bIsCheckInBound )
            m_cbIsCheckInBound.SetCurSel( 0 );
        else
            m_cbIsCheckInBound.SetCurSel( 1 );
    }
    
}

// 값 저장
void CDlgTGWall::CalcResult(void)
{
    //************************************************************************
    // 설명 생성
    //************************************************************************
    {
        CString strType, strSlot, strPosX;
        m_ebWallType.GetWindowText(strType);
        m_ebWallSlot.GetWindowText(strSlot);
        m_ebPosX.GetWindowText(strPosX);
        int iIsDestroyIndex = m_cbIsDestroy.GetCurSel();
        if( 0 < iIsDestroyIndex )
            m_strDesc =  L"[" + strSlot + L"] 벽을 제거한다";
        else
            m_strDesc =  L"[" + strSlot + L"] " + strType + L"번 타입 벽을 X축 " + strPosX + L"에 설치한다.";
    }

    //************************************************************************
    // 값 저장
    //************************************************************************
    {
        CString strTemp;
        m_ebWallType.GetWindowText(strTemp);
        m_TGWall.m_iWallType = (int)_wtof(strTemp);
        m_ebWallSlot.GetWindowText(strTemp);
        m_TGWall.m_iWallSlot = (int)_wtof(strTemp);
        m_ebPosX.GetWindowText(strTemp);
        m_TGWall.m_fPosX = (float)_wtof(strTemp);
        m_TGWall.m_bDestroy = (0 < m_cbIsDestroy.GetCurSel());
        m_TGWall.m_bIsCheckInBound = (0 == m_cbIsCheckInBound.GetCurSel());
        m_TGWall.m_strDesc = GetDesc();
    }
}