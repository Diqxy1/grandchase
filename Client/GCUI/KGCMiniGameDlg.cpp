#include "stdafx.h"
#include "KGCMiniGameDlg.h"

//
//
//
//
#include "gcui/KGCMiniGameRanker.h"
#include "gcui/KGCMiniGameCategory.h"
//
#include "KGCMinigameManager.h"
#include "Square/GCSquare.h"
//
//
#include "MyD3D.h"


IMPLEMENT_CLASSNAME( KGCMinigameDlg );
IMPLEMENT_WND_FACTORY( KGCMinigameDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCMinigameDlg, "gc_minigame_select_box" );

KGCMinigameDlg::KGCMinigameDlg(void)
: m_pkCloseBtn( NULL )
, m_pkLeftBtn( NULL )
, m_pkRightBtn( NULL )
, m_pkStartGameBtn( NULL )
, m_pkMyHighScore( NULL )
, m_iCurrentGameIndex( 0 )
, m_iPrevGameIndex( -1 )
{
    for ( int i = 0; i < RANKER_NUM; ++i )
    {
        m_pkRanker[i] = NULL;

        if ( i < MINIGAME_NUM )
        {
            m_pkGameImage[i] = NULL;
            m_pkMiniGameCategory[i] = NULL;
            m_pkMiniGameBtn[i] = NULL;
        }
    }

    LINK_CONTROL( "close_btn",          m_pkCloseBtn );
    LINK_CONTROL( "left_btn",           m_pkLeftBtn );
    LINK_CONTROL( "right_btn",          m_pkRightBtn );
    LINK_CONTROL( "game_start_btn",     m_pkStartGameBtn );
    LINK_CONTROL( "myscore",            m_pkMyHighScore );

    char str[MAX_PATH];
    for ( int i = 0; i < RANKER_NUM ; ++i )
    {
        sprintf( str, "ranker%d", i+1 );
        LINK_CONTROL( str, m_pkRanker[i] );

        if ( i < MINIGAME_NUM )
        {
            sprintf( str, "game_back%d", i+1 );
            LINK_CONTROL( str, m_pkGameImage[i] );
            sprintf( str, "minigame_%d", i+1 );
            LINK_CONTROL( str, m_pkMiniGameCategory[i] );
            sprintf( str, "minigame%d_btn", i+1 );
            LINK_CONTROL( str, m_pkMiniGameBtn[i] );
        }
    }
}

KGCMinigameDlg::~KGCMinigameDlg(void)
{
}

void KGCMinigameDlg::OnCreate( void )
{
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
    m_pkLeftBtn->InitState( true, true, this );
    m_pkLeftBtn->Lock( true );
    m_pkRightBtn->InitState( true, true, this );
    m_pkRightBtn->Lock( true );
    m_pkStartGameBtn->InitState( true, true, this );
    m_pkMyHighScore->InitState( true );
    m_pkMyHighScore->SetAlign( DT_RIGHT );
    m_pkMyHighScore->SetFontColor( D3DCOLOR_ARGB(255,255,255,0) );

    for ( int i = 0; i < RANKER_NUM ; ++i )
    {
        m_pkRanker[i]->InitState( true );

        if ( i < MINIGAME_NUM )
        {
            m_pkGameImage[i]->InitState( true );
            m_pkMiniGameCategory[i]->InitState( false );
            m_pkMiniGameBtn[i]->InitState( true, true, this );
        }
    }

    m_pkMiniGameBtn[2]->Lock( true );
}

void KGCMinigameDlg::OnCreateComplete( void )
{
    m_iPrevGameIndex = -1;
    UpdateMiniGameData( m_iCurrentGameIndex );
}

void KGCMinigameDlg::ActionPerformed( const KActionEvent& event )
{
    // 요 밑으로는 클릭질~
    if ( event.m_dwCode != D3DWE_BUTTON_CLICK ) return;

    if ( event.m_pWnd == m_pkCloseBtn )
    {
        g_KDSound.Play( "31" );
        SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    }
    else if ( event.m_pWnd == m_pkLeftBtn )
    {
        m_iCurrentGameIndex = --m_iCurrentGameIndex % MINIGAME_NUM;

        if ( m_iCurrentGameIndex == -1 )
            m_iCurrentGameIndex = MINIGAME_NUM - 1;

        UpdateMiniGameData( m_iCurrentGameIndex );
    }
    else if ( event.m_pWnd == m_pkRightBtn )
    {
        m_iCurrentGameIndex = ++m_iCurrentGameIndex % MINIGAME_NUM;
        UpdateMiniGameData( m_iCurrentGameIndex );
    }
    else if ( event.m_pWnd == m_pkStartGameBtn )
    {
        KP2P::GetInstance()->Send_StartMiniGameReq( static_cast<char>(m_iCurrentGameIndex) );
        g_MyD3D->WaitForServerAck( Result_Start_MiniGame, INT_MAX, 5000, TIME_OUT_VALUE );

        switch( Result_Start_MiniGame )
        {
        case 0: // 성공
            {
                SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
#if defined (PARTY_SYSTEM)
                if (SiKGCPartyManager()->IsParty())
                    SiKGCPartyManager()->LeaveParty();
#endif 
                SiGCSquare()->CloseSquare();
                SiKGCMinigameManager()->SetMiniGameModeID( static_cast<EGCGameMode>(GC_GM_MINIGAME_TREEDROP + m_iCurrentGameIndex) );
                SiKGCMinigameManager()->StartMiniGame();
                SiKGCMinigameManager()->SetExitGame( false );
                break;
            }
        default:
            std::wstring strTemp = g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 );
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                KUserEvent::EVENT_START_MINIGAME_ACK, Result_Start_MiniGame );	

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ), 
                                    strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        }

        Result_Start_MiniGame = INT_MAX;
    }

    for ( int i = 0; i < MINIGAME_NUM; ++i )
    {
        if ( event.m_pWnd == m_pkMiniGameBtn[i] )
        {
            UpdateMiniGameData( i );
        }
    }
}

void KGCMinigameDlg::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCMinigameDlg::FrameMoveInEnabledState( void )
{
}

void KGCMinigameDlg::SetGameCategory( int iCurrentGameIndex )
{
    if ( iCurrentGameIndex < 0 || iCurrentGameIndex > MINIGAME_NUM )
    {
        assert( !"o미? 이건 아니자나!! 게임 인덱스가 이상해" );
        return;
    }

    for ( int i = 0; i < MINIGAME_NUM; ++i )
    {
        if ( iCurrentGameIndex == i )
        {
            m_pkMiniGameCategory[i]->SetGameDesc( i );
            m_pkMiniGameCategory[i]->ToggleRender( true );
        }
        else
        {
            m_pkMiniGameCategory[i]->ToggleRender( false );
        }
    }

    m_iPrevGameIndex = iCurrentGameIndex;
}

void KGCMinigameDlg::SetRanker( std::vector<KMiniGameRankInfo>& vecRankInfo )
{
    for ( int i = 0; i < RANKER_NUM; ++i )
    {
        m_pkRanker[i]->SetNickName( L"" );
        m_pkRanker[i]->SetScore( 0 );
    }

    if ( true == vecRankInfo.empty() )
        return;

    std::vector<KMiniGameRankInfo>::iterator vit = vecRankInfo.begin();

    int iRank;
    for ( ; vit != vecRankInfo.end(); ++vit )
    {
        if ( vit->m_cGameMode != m_iCurrentGameIndex )
            return;

        iRank = vit->m_nRank-1;
        if ( iRank >= RANKER_NUM || iRank < 0 )
        {
            assert(!"난 1위부터 5위까지만 줄껀데" );
            return;
        }

        m_pkRanker[iRank]->SetNickName( vit->m_strNick );
        m_pkRanker[iRank]->SetScore( vit->m_dwScore );
    }
}

void KGCMinigameDlg::UpdateMiniGameData( int iCurrentGameIndex )
{
    m_iCurrentGameIndex = iCurrentGameIndex;
    SetMyHighScore( m_iCurrentGameIndex );
    if ( m_iPrevGameIndex != iCurrentGameIndex )
    {
		std::vector<KMiniGameRankInfo> vecRankInfo;
		SiKGCMinigameManager()->GetRankingInfo(iCurrentGameIndex, vecRankInfo );
        SetRanker( vecRankInfo );
    }
    SetGameCategory( iCurrentGameIndex );
}

std::wstring KGCMinigameDlg::GetRandkerName( int iRank )
{
    if ( iRank < 0 || iRank >= RANKER_NUM )
        assert( !"incorrect ranker index" );

    return m_pkRanker[iRank-1]->GetNickName();
}

void KGCMinigameDlg::SetNPCSpeech( void )
{
    // i'm so sorry but i'm love you... 
    // 랭킹이 없을때는 랭킹 말 하지 말쟈 ㅋ 
    int iTmp;
    std::wstring tmp = SiKGCMinigameManager()->GetRankerName( 0, 1 );
    std::wstring tmp1 = SiKGCMinigameManager()->GetRankerName( 1, 1 );
    SiGCSquare()->ReloadNPCSpeechList( SOST_MINIGAME_NPC );

    if ( false == SiKGCMinigameManager()->IsInRanker( g_kGlobalValue.m_kUserInfo.strNickName ) )
    {
        for ( int i = 0; i < 5; ++i )
        {
            iTmp = STR_ID_MINIGAME_NPC_SPEECH_MYNAME0 + i;
            SiGCSquare()->SetNPCSpeechList( SOST_MINIGAME_NPC, g_pkStrLoader->GetReplacedString( iTmp, "l", g_kGlobalValue.m_kUserInfo.strNickName ) );
        }
    }

    if ( tmp != L"" )
    {
        for ( int i = 0; i < 3; ++i )
        {
            iTmp = STR_ID_MINIGAME_NPC_SPEECH_WOOD0 + i;
            SiGCSquare()->SetNPCSpeechList( SOST_MINIGAME_NPC, g_pkStrLoader->GetReplacedString( iTmp, "l", tmp ) );
        }
    }

    if ( tmp1 != L"" )
    {
        for ( int i = 0; i < 3; ++i )
        {
            iTmp = STR_ID_MINIGAME_NPC_SPEECH_BALLOON0 + i;
            SiGCSquare()->SetNPCSpeechList( SOST_MINIGAME_NPC, g_pkStrLoader->GetReplacedString( iTmp, "l", tmp1 ) );
        }
    }
}

void KGCMinigameDlg::SetMyHighScore( int iCurrentGameIndex )
{
    m_pkMyHighScore->SetText( L"" );
    int iMyRank = SiKGCMinigameManager()->GetMyRanking( iCurrentGameIndex );
    int iMyScore = SiKGCMinigameManager()->GetMyHighScore( iCurrentGameIndex );

	if( iMyScore > 9999 )
		iMyScore = 9999;
	else if( iMyScore < 0 )
		iMyScore = 0;

    if ( iMyRank <= 100 && iMyRank > 0 )
    {
        m_pkMyHighScore->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MYSCORE_AND_RANK, "ii", iMyScore, iMyRank ) );
    }
    else
    {
        m_pkMyHighScore->SetText( iMyScore );
    }
}
