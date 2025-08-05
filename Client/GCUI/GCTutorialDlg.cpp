#include "stdafx.h"
#include "GCTutorialDlg.h"
//
//
//



//
//
//
//
#include "GCSkillCheckBtn.h"

/*
사실은 게임 내에서 여러 Dialog를 컨트롤 하려고 만들었는데
튜토리얼의 Custom한 Dialog가 되고 말았다-_-;;
*/
IMPLEMENT_CLASSNAME( KGCTutorialDlg );
IMPLEMENT_WND_FACTORY( KGCTutorialDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCTutorialDlg, "gc_tutorialdlg" );

KGCTutorialDlg*   g_pkTutorialDlg       = NULL;

KGCTutorialDlg::KGCTutorialDlg( void )
{
    g_pkTutorialDlg                 = this;
    m_bEnable                       = false;
    m_uiUsedBtnCount                = 0;

    m_dwPastTime                    = 0;
    m_dwCurTime                     = 0;
    m_dwElapsedTime                 = 0;

    for ( int i = 0; i < SKILL_CHECK_BUTTON_NUMBER; i++ )
    {
        m_pkSkillCheckBtn[i] = NULL;
    }
    LINK_CONTROL( "char_name",                  m_pkCharName );
    LINK_CONTROL( "tab",                        m_pkTab );
    LINK_CONTROL( "exit",                       m_pkExit );
    LINK_CONTROL( "tutorialoption",             m_pkNewTutorialOption );
    LINK_CONTROL( "skill_check_button0",        m_pkSkillCheckBtn[0] );
    LINK_CONTROL( "skill_check_button1",        m_pkSkillCheckBtn[1] );
    LINK_CONTROL( "skill_check_button2",        m_pkSkillCheckBtn[2] );
    LINK_CONTROL( "skill_check_button3",        m_pkSkillCheckBtn[3] );
    LINK_CONTROL( "skill_check_button4",        m_pkSkillCheckBtn[4] );
    LINK_CONTROL( "skill_check_button5",        m_pkSkillCheckBtn[5] );
    LINK_CONTROL( "skill_check_button6",        m_pkSkillCheckBtn[6] );
    LINK_CONTROL( "skill_check_button7",        m_pkSkillCheckBtn[7] );
    LINK_CONTROL( "skill_check_button8",        m_pkSkillCheckBtn[8] );
    LINK_CONTROL( "skill_check_button9",        m_pkSkillCheckBtn[9] );
    LINK_CONTROL( "skill_check_button10",       m_pkSkillCheckBtn[10] );
    LINK_CONTROL( "skill_check_button11",       m_pkSkillCheckBtn[11] );
    LINK_CONTROL( "skill_check_button12",       m_pkSkillCheckBtn[12] );
    LINK_CONTROL( "skill_check_button13",       m_pkSkillCheckBtn[13] );
    LINK_CONTROL( "skill_check_button14",       m_pkSkillCheckBtn[14] );

    m_iLocalPlayerIndex = 0;
    m_iNowPage = 0;
    m_uiPlayerPastMotion = 0;
    m_mapTutorialSkillCheckBtn.clear();
    m_mapTutorialSkillClearList.clear();
}

KGCTutorialDlg::~KGCTutorialDlg( void )
{
    m_mapTutorialSkillCheckBtn.clear();
    m_mapTutorialSkillClearList.clear();
}

void KGCTutorialDlg::OnCreate( void )
{
    Init();
}

void KGCTutorialDlg::Init()
{
    ToggleRender( false );
    UINT iNumChild = GetNumChild();
    KD3DWnd* pChildWnd = NULL;
    for ( UINT i = 0; i < iNumChild; i++ )
    {
        GetChild( i )->SetWindowColor( D3DCOLOR_ARGB(100,255,255,255) );
    }
    m_pkNewTutorialOption->ToggleRender( true );
    m_pkExit->ToggleRender( true );
    m_pkTab->ToggleRender( true );
    m_pkCharName->ToggleRender( true );
    for ( int i = 0; i < SKILL_CHECK_BUTTON_NUMBER; i++ )
    {
        m_pkSkillCheckBtn[i]->SetWindowColor( D3DCOLOR_ARGB(255,255,255,255) );
        m_pkSkillCheckBtn[i]->ToggleRender( true );
        m_pkSkillCheckBtn[i]->Init();
        m_pkSkillCheckBtn[i]->SetBtnIndex(i);
    }
    ClearList();
    m_dwPastTime                    = 0;
    m_dwCurTime                     = 0;
    m_dwElapsedTime                 = 0;

    m_iLocalPlayerIndex = 0;
    m_iNowPage = 0;
    m_iPrevPage = 0;
    m_uiPlayerPastMotion = 0;
}

void KGCTutorialDlg::Enable()
{
    ToggleRender( true );
    m_bEnable = true;
}
void KGCTutorialDlg::Disable()
{
    ToggleRender( false );
    m_bEnable = false;
}

void KGCTutorialDlg::AddStringToList( std::wstring str_, DWORD dwColor_ )
{
}

void KGCTutorialDlg::AddCharNameToList( std::wstring str_, DWORD dwColor_ )
{
    m_pkCharName->SetText( str_ );
    m_pkCharName->SetFontColor( dwColor_ );
}

void KGCTutorialDlg::ClearList()
{
    for ( int i = 0; i < SKILL_CHECK_BUTTON_NUMBER; i++ )
    {
        m_pkSkillCheckBtn[i]->m_pkSkillDesc->SetText( L"" );
    }
    m_pkNewTutorialOption->SetText(L"");
    m_pkExit->SetText(L"");
    m_pkTab->SetText(L"");
    m_pkCharName->SetText(L"");
    m_uiUsedBtnCount = 0;
}
KGCSkillCheckBtn* KGCTutorialDlg::GetUnUsedBtn()
{
    if ( SKILL_CHECK_BUTTON_NUMBER  == m_uiUsedBtnCount )
        return NULL;
    return m_pkSkillCheckBtn[m_uiUsedBtnCount++];
}

void KGCTutorialDlg::FrameMove()
{
    if( m_iPrevForm != g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->MetamorphosisForm ||
        m_uiWaitMotion != g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->m_SwapTemplate.WAIT ||
        m_iPrevPage != m_iNowPage )
    {
        SetSkillBox();
    }

    m_iPrevForm = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->MetamorphosisForm;
    m_uiWaitMotion = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->m_SwapTemplate.WAIT;
    m_iPrevPage = m_iNowPage;

    CalcElapsedTime();
    for ( int i = 0; i < SKILL_CHECK_BUTTON_NUMBER; i++ )
    {
        m_pkSkillCheckBtn[i]->FrameMove( m_dwElapsedTime );
    }

    //////////////////////////////////////////////////////////////////////////
    // 캐릭터 기술 사용 성공시 이펙트 발생하는 부분 처리
    if ( m_uiPlayerPastMotion != g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->uiMotion )
    {
        UINT uiNowMotion = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->uiMotion;
        if ( MID_LAS_SPECIAL2_FAIL == uiNowMotion )
            uiNowMotion = MID_LAS_SPECIAL2;
        if( MID_AMY1_DANCE_SPECIAL2_LEFT == uiNowMotion )
            uiNowMotion = MID_AMY1_DANCE_SPECIAL2;
        if( MID_AMY1_DANCE_SPECIAL3_LEFT == uiNowMotion )
            uiNowMotion = MID_AMY1_DANCE_SPECIAL3;
        if ( m_mapTutorialSkillCheckBtn.find( uiNowMotion ) != m_mapTutorialSkillCheckBtn.end() )
        {
            std::vector<KGCSkillCheckBtn*>::iterator vit;
            for ( vit = m_mapTutorialSkillCheckBtn[uiNowMotion].begin(); vit != m_mapTutorialSkillCheckBtn[uiNowMotion].end(); ++vit )
            {
                (*vit)->EnableStarAni();
                (*vit)->EnableBarAni();
                if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL &&
                    SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
                {
                    (*vit)->ChangeStringColor( D3DCOLOR_ARGB( 255, 100, 100, 100 ) );
                }            
                UINT uiBtnIndex = (*vit)->GetBtnIndex();
                if ( uiBtnIndex != SKILL_CHECK_BUTTON_NUMBER - 1 && m_pkSkillCheckBtn[uiBtnIndex+1]->GetMotionID() == g_pGraphicsHelper->GetPlayerMotionNum() )
                {
                    uiBtnIndex++;
                    m_pkSkillCheckBtn[uiBtnIndex]->EnableStarAni();
                    m_pkSkillCheckBtn[uiBtnIndex]->EnableBarAni();
                    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL &&
                        SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
                    {
                        m_pkSkillCheckBtn[uiBtnIndex]->ChangeStringColor( D3DCOLOR_ARGB( 255, 100, 100, 100 ) );
                    }
                }
            }

            m_mapTutorialSkillClearList[uiNowMotion] = true;
        }
    }

    m_uiPlayerPastMotion = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->uiMotion;
    //////////////////////////////////////////////////////////////////////////
}

void KGCTutorialDlg::CalcElapsedTime()
{
    m_dwCurTime = timeGetTime();
    m_dwElapsedTime = m_dwCurTime - m_dwPastTime;
    m_dwPastTime = m_dwCurTime;
}

void KGCTutorialDlg::InitSkillList()
{
    ClearList();
    m_mapTutorialSkillCheckBtn.clear();
    m_mapTutorialSkillClearList.clear();

    SetLocalPlayerIndex();
    m_iPrevForm = FORM_NORMAL;
    m_uiWaitMotion = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->m_SwapTemplate.WAIT;
    m_iNowPage = 0;
    m_iPrevPage = 0;

    SetSkillBox();
}

void KGCTutorialDlg::SetSkillBox()
{
    std::map<int, SSkillCommandList>::size_type mapSize;
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL &&
        SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
    {
        mapSize = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->GetCurFormTemplate().m_mapTutorialCommandList.size();
    }
    else
    {
        mapSize = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->GetCurFormTemplate().m_mapCommandList.size();
    }

    if ( m_iNowPage >= static_cast<int>(mapSize) )
        return;

    ClearList();
    m_mapTutorialSkillCheckBtn.clear();

    SSkillCommandList sSkillCommandList;

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL &&
        SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
    {
        sSkillCommandList = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->GetCurFormTemplate().m_mapTutorialCommandList[m_iNowPage];
    }
    else
    {
        sSkillCommandList = g_MyD3D->MyPlayer[m_iLocalPlayerIndex]->GetCurFormTemplate().m_mapCommandList[m_iNowPage];
    }

    AddCharNameToList( g_pkStrLoader->GetString( sSkillCommandList.m_iNameStringID ).c_str(), sSkillCommandList.m_dwColor );

    std::vector<SSkillCommand>::iterator vit;
    for ( vit = sSkillCommandList.m_vecCommandList.begin(); vit != sSkillCommandList.m_vecCommandList.end(); ++vit )
    {
        if ( !(vit->m_vecMotionID.empty()) )
        {
            LinkSkillToCheckButtonWithBeforeColor( g_pkStrLoader->GetString( vit->m_iSkillID ).c_str(), vit->m_vecMotionID );
        }
        else
        {
            LinkSkillToCheckButton( g_pkStrLoader->GetString( vit->m_iSkillID ).c_str(), -1, D3DCOLOR_ARGB( 255, 255, 255, 0) );
        }
    }

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL &&
        SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
    {
        SetSkillBox_Common();
    }
}

void KGCTutorialDlg::SetSkillBox_Common()
{
    m_pkTab->SetText( g_pkStrLoader->GetString( STR_ID_TUTORIAL_TOGGLE ).c_str() );
    m_pkTab->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 0) );
    m_pkExit->SetText( g_pkStrLoader->GetString( STR_ID_TUTORIAL_EXIT ).c_str() );
    m_pkExit->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 0) );

    m_pkNewTutorialOption->SetText( g_pkStrLoader->GetString( STR_ID_TUTORIAL_TUTORIALOPTION ).c_str() );
    m_pkNewTutorialOption->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 0) );
}

void KGCTutorialDlg::LinkSkillToCheckButton( std::wstring str_, UINT uiMotionID, DWORD dwColor_ )
{
    KGCSkillCheckBtn* pkSkillCheckBtn = GetUnUsedBtn();
    if ( pkSkillCheckBtn == NULL )
        return;

    m_mapTutorialSkillCheckBtn[uiMotionID].push_back(pkSkillCheckBtn);
    pkSkillCheckBtn->AddString( str_, dwColor_ );
    pkSkillCheckBtn->SetMotionID( (int)uiMotionID );
}

void KGCTutorialDlg::LinkSkillToCheckButtonWithBeforeColor( std::wstring str_, UINT uiMotionID )
{
    KGCSkillCheckBtn* pkSkillCheckBtn = GetUnUsedBtn();
    if ( pkSkillCheckBtn == NULL )
        return;

    DWORD dwColor_ = 0xFFFFFFFF;
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL &&
        SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
    {
        if( IsSkillClear(uiMotionID) )
            dwColor_ = D3DCOLOR_ARGB( 255, 100, 100, 100 );
    }

    m_mapTutorialSkillCheckBtn[uiMotionID].push_back(pkSkillCheckBtn);
    pkSkillCheckBtn->AddString( str_, dwColor_ );
    pkSkillCheckBtn->SetMotionID( (int)uiMotionID );
}

void KGCTutorialDlg::LinkSkillToCheckButton( std::wstring str_, std::vector<UINT> uiMotionID, DWORD dwColor_ )
{
    KGCSkillCheckBtn* pkSkillCheckBtn = GetUnUsedBtn();
    if ( pkSkillCheckBtn == NULL )
        return;
    for( int i = 0; i < (int)uiMotionID.size(); i++ )
        m_mapTutorialSkillCheckBtn[ uiMotionID[i] ].push_back(pkSkillCheckBtn);

    pkSkillCheckBtn->AddString( str_, dwColor_ );
    pkSkillCheckBtn->SetMotionID( (int)uiMotionID[0] );
}

void KGCTutorialDlg::LinkSkillToCheckButtonWithBeforeColor( std::wstring str_, std::vector<UINT> uiMotionID )
{
    KGCSkillCheckBtn* pkSkillCheckBtn = GetUnUsedBtn();
    if ( pkSkillCheckBtn == NULL )
        return;

    DWORD dwColor_ = 0xFFFFFFFF;    

    for( int i = 0; i < (int)uiMotionID.size(); i++ )
    {
        m_mapTutorialSkillCheckBtn[ uiMotionID[i] ].push_back(pkSkillCheckBtn);
        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL &&
            SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL )
        {
            if( IsSkillClear(uiMotionID[i]) )
                dwColor_ = D3DCOLOR_ARGB( 255, 100, 100, 100 );
        }
    }

    pkSkillCheckBtn->AddString( str_, dwColor_ );
    pkSkillCheckBtn->SetMotionID( (int)uiMotionID[0] );
}

bool KGCTutorialDlg::IsSkillClear( UINT uiMotion )
{
    if( m_mapTutorialSkillClearList.count( uiMotion ) )
    {
        return m_mapTutorialSkillClearList[uiMotion];
    }

    return false;
}

void KGCTutorialDlg::SetLocalPlayerIndex()
{
    for(int i = 0; i < MAX_PLAYER_NUM; i++)
    {	
        if( g_MyD3D->MyPlayer[i]->IsLocalPlayer() )
        {
            m_iLocalPlayerIndex = i;
            break;
        }
    }
}

void KGCTutorialDlg::NextSkillListPage()
{
    m_iNowPage++;
    if( m_iNowPage == 2 )
        m_iNowPage = 0;
}