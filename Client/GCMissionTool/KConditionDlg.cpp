// KConditionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "define.h"
#include "GCMissionTool.h"
#include "KConditionDlg.h"
#include ".\kconditiondlg.h"
#include "GCMissionManager.h"
#include "GlobalFunc.h"
#include "IniReader.h"

// KConditionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KConditionDlg, CDialog)
KConditionDlg::KConditionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KConditionDlg::IDD, pParent)
    , m_bWin(FALSE)
    , m_Ratio(_T(""))
{
    m_iConditionID = -1;
}

KConditionDlg::~KConditionDlg()
{
}

void KConditionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT12, m_Stage);
    DDX_Text(pDX, IDC_EDIT11, m_DifficultRange);
    DDX_Text(pDX, IDC_EDIT10, m_MapID);
    DDX_Text(pDX, IDC_EDIT9, m_ScoreRange);
    DDX_Text(pDX, IDC_EDIT8, m_ClearTime);
    DDX_Text(pDX, IDC_EDIT7, m_PlayerKill);
    DDX_Text(pDX, IDC_EDIT6, m_Die);
    DDX_Text(pDX, IDC_EDIT5, m_ExLife);
    DDX_Check(pDX, IDC_CHECK1, m_bWin);
    DDX_Text(pDX, IDC_EDIT4, m_Ratio);
    DDX_Control(pDX, IDC_COMBO1, m_combo_Monster);
    DDX_Control(pDX, IDC_COMBO2, m_combo_GameMode);
}


BEGIN_MESSAGE_MAP(KConditionDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// KConditionDlg 메시지 처리기입니다.
#define GET_INTEGER( x, y ) \
    if( x.Compare( L"" ) != 0 ) \
    GC::GetIntegerFromString( x, y );
#define GET_PAIR( x, y ) \
    if( x.Compare( L"" ) != 0 ) \
    GC::GetPairDataFromString( x, y );

void KConditionDlg::OnBnClickedOk()
{
    OnOK();
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    SConditionInfo sInfo;
    sInfo.m_iMonsterID = ( m_combo_Monster.GetCurSel() == m_combo_Monster.GetCount() - 1 ? -1 : m_combo_Monster.GetCurSel() );
    sInfo.m_iGameMode = ( m_combo_GameMode.GetCurSel() == m_combo_GameMode.GetCount() - 1 ? -1 : m_combo_GameMode.GetCurSel() );
    GET_INTEGER( m_Stage, sInfo.m_iStage );
    GET_INTEGER( m_MapID, sInfo.m_iMapID );
    GET_INTEGER( m_ClearTime, sInfo.m_iTime );
    GET_INTEGER( m_PlayerKill, sInfo.m_iKill );
    GET_INTEGER( m_Die, sInfo.m_iDie );
    GET_INTEGER( m_ExLife, sInfo.m_iExLife );
    GET_PAIR( m_DifficultRange, sInfo.m_pairDifficultRange );
    GET_PAIR( m_ScoreRange, sInfo.m_pairScoreRange );
    sInfo.m_bWin = ( m_bWin == TRUE ? true : false );
    sInfo.m_fConditionRatio = GC::ToFloat( m_Ratio.GetBuffer() );

    m_iConditionID = SiKGCMissionManager()->AddConditionInfo( sInfo );
}

void KConditionDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
    m_iConditionID = -1;
}

BOOL KConditionDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    InitCombo_Monster();
    GC::FillComboBoxFromINI( m_combo_GameMode, "GameMode", "GM%d" );
    m_combo_GameMode.SetCurSel( m_combo_GameMode.AddString( L"없음" ) );
    FillControls( m_iConditionID );
    return TRUE;
}

void KConditionDlg::InitCombo_Monster()
{
    GC::FillComboBoxFromINI( m_combo_Monster, "Monster", "Monster%d" );
    m_combo_Monster.SetCurSel( m_combo_Monster.AddString( L"없음" ) );
}

void KConditionDlg::SetContidionID( int iConditionID )
{
    m_iConditionID = iConditionID;
}

void KConditionDlg::FillControls( int iConditionID )
{
    const SConditionInfo* sInfo = SiKGCMissionManager()->GetConditionInfo( iConditionID );
    if( sInfo == NULL )
        return;

    WCHAR strText[MAX_PATH] = {0, };

    m_Stage = GC::IntToWchar( sInfo->m_iStage ).c_str();
    m_DifficultRange = GC::ToWString( sInfo->m_pairDifficultRange ).c_str();
    m_MapID = GC::IntToWchar( sInfo->m_iMapID ).c_str();
    m_ScoreRange = GC::ToWString( sInfo->m_pairScoreRange ).c_str();
    m_ClearTime = GC::IntToWchar( sInfo->m_iTime ).c_str();
    m_PlayerKill = GC::IntToWchar( sInfo->m_iKill ).c_str();
    m_Die = GC::IntToWchar( sInfo->m_iDie ).c_str();
    m_ExLife = GC::IntToWchar( sInfo->m_iExLife ).c_str();
    m_bWin = sInfo->m_bWin ? TRUE : FALSE;
    m_Ratio = GC::ToWString( sInfo->m_fConditionRatio ).c_str();
    m_iConditionID = iConditionID;
    m_combo_Monster.SetCurSel( sInfo->m_iMonsterID == -1 ? m_combo_Monster.GetCount() - 1 : sInfo->m_iMonsterID );
    m_combo_GameMode.SetCurSel( sInfo->m_iGameMode == -1 ? m_combo_GameMode.GetCount() - 1 : sInfo->m_iGameMode );

    Invalidate();
}