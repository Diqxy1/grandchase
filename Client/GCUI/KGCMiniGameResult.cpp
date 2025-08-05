#include "stdafx.h"
#include "KGCMinigameResult.h"

//
//
//
//
//
#include "KGCMinigameManager.h"
//
#include ".\gcquestranktotalscore.h"
//

#include "KGCGameBoard.h"

IMPLEMENT_CLASSNAME( KGCMinigameResult );
IMPLEMENT_WND_FACTORY( KGCMinigameResult );
IMPLEMENT_WND_FACTORY_NAME( KGCMinigameResult, "gc_minigame_result_box" );

KGCMinigameResult::KGCMinigameResult(void)
: m_pkRestartBtn( NULL )
, m_pkEndGameBtn( NULL )
, m_pkRankStarScore( NULL )
, m_pkRankLowDesc( NULL )
, m_bIniValue( false )
, m_iGrade( 0 )
, m_pkRankDesc( NULL )
{
    for ( int i = 0; i < 4; ++i )
    {
        m_pkMyCurrentRanking[i] = NULL;
        m_pkHighScore[i] = NULL;
        m_pkCurrentScore[i] = NULL;
        m_pkPlayTime[i] = NULL;
        m_pkRankGrade[i] = NULL;
    }

    LINK_CONTROL( "restart_btn",          m_pkRestartBtn );
    LINK_CONTROL( "endgame_btn",          m_pkEndGameBtn );
    LINK_CONTROL( "rankstar_score",       m_pkRankStarScore );
    LINK_CONTROL( "sayballon",            m_pkRankLowDesc );
	LINK_CONTROL( "rank_desc",			  m_pkRankDesc );

    LINK_CONTROL( "quest_total_score_fail",             m_pkRankGrade[0] );
    LINK_CONTROL( "quest_total_score_bad",              m_pkRankGrade[1] );
    LINK_CONTROL( "quest_total_score_good",             m_pkRankGrade[2] );
    LINK_CONTROL( "quest_total_score_perfect",          m_pkRankGrade[3] );

    char str[MAX_PATH];
    for ( int i = 0; i < 4 ; ++i )
    {
        sprintf( str, "number%d", i );
        LINK_CONTROL( str, m_pkMyCurrentRanking[i] );

        sprintf( str, "playtime_number%d", i );
        LINK_CONTROL( str, m_pkPlayTime[i] );

        sprintf( str, "highscore_number%d", i );
        LINK_CONTROL( str, m_pkHighScore[i] );

        sprintf( str, "curscore_number%d", i );
        LINK_CONTROL( str, m_pkCurrentScore[i] );
    }
}

KGCMinigameResult::~KGCMinigameResult(void)
{
}

void KGCMinigameResult::OnCreate( void )
{
    m_pkRestartBtn->InitState( true, true, this );
    m_pkEndGameBtn->InitState( true, true, this );
    m_pkRankStarScore->InitState( true, false, this );
    m_pkRankLowDesc->InitState( false );
	m_pkRankDesc->InitState( false );

    for ( int i = 0; i < 4; ++i )
    {
        m_pkMyCurrentRanking[i]->InitState( false );
        m_pkHighScore[i]->InitState( false );
        m_pkCurrentScore[i]->InitState( false );
        m_pkPlayTime[i]->InitState( false );
    }

    InitData();
}

void KGCMinigameResult::ActionPerformed( const KActionEvent& event )
{
    // 요 밑으로는 클릭질~
    if ( event.m_dwCode != D3DWE_BUTTON_CLICK ) return;

    if ( event.m_pWnd == m_pkRestartBtn )
    {
        // 다시시작이에요 함수 하나 만들어주센
        SiKGCMinigameManager()->SetExitGame( false );
        SiKGCMinigameManager()->RestartMinigame();
        KP2P::GetInstance()->Send_RestartMiniGame( static_cast<char>(SiKGCMinigameManager()->GetMinigameModeID() - GC_GM_MINIGAME_TREEDROP) );
        this->ToggleRender( false );
        InitData();
    }

    if ( event.m_pWnd == m_pkEndGameBtn )
    {
        // 끝내쟈!!
        KP2P::GetInstance()->Send_ExitMiniGameReq();
        this->ToggleRender( false );
    }
}

void KGCMinigameResult::OnDestroyComplete( void )
{
    //SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCMinigameResult::FrameMoveInEnabledState( void )
{
    FrameMove_GradeEffect(&m_GradeInfo[m_iGrade], m_pkRankGrade[m_iGrade]);

    if ( (timeGetTime() - m_dwStartTime) > 5000/*5초*/ )
    {
        m_pkRestartBtn->Lock( false );
        m_pkEndGameBtn->Lock( false );
    }
    else
    {
        m_pkRestartBtn->Lock( true );
        m_pkEndGameBtn->Lock( true );
    }
}

void KGCMinigameResult::InitNumberTextureCoordinate( void )
{
    // 아 귀찮아.. 한나나에게 잘 부탁하면 이런짓 안해도 될텐데 ㅎㅎ
    D3DXVECTOR4 vStartCoord1( 216.0f, 157.0f, 243.0f, 189.0f );
    D3DXVECTOR4 vStartCoord2( 237.0f, 159.0f, 250.0f, 176.0f );
    int iOffsetArray1[] = { 0, 29, 30, 29, 28, 29, 29, 28, 30, 28 };
    int iOffsetArray2[] = { 0, 16, 16, 17, 17, 16, 17, 17, 16, 16 };

    for ( int i = 0; i < 10; ++i )
    {
        vStartCoord1.x += iOffsetArray1[i];
        vStartCoord2.x += iOffsetArray2[i];
        vStartCoord1.z += iOffsetArray1[i];
        vStartCoord2.z += iOffsetArray2[i];

        m_pkMyCurrentRanking[0]->SetNumberCoordi( i, vStartCoord1 );
        m_pkHighScore[0]->SetNumberCoordi( i, vStartCoord2 );
    }

    for ( int i = 0; i < 4; ++i )
    {
        m_pkMyCurrentRanking[i]->SetTexCoordi( m_pkMyCurrentRanking[0]->GetTexCoordi() );
        m_pkPlayTime[i]->SetTexCoordi( m_pkHighScore[0]->GetTexCoordi() );
        m_pkHighScore[i]->SetTexCoordi( m_pkHighScore[0]->GetTexCoordi() );
        m_pkCurrentScore[i]->SetTexCoordi( m_pkHighScore[0]->GetTexCoordi() );
    }
}

void KGCMinigameResult::SetMyRanking( const int iRank )
{
    int iTmp;
    if ( iRank < 0 )
        iTmp = 0;
    else
        iTmp = iRank;

    int iDigit = GetDigit(iTmp);
    if ( iDigit > 4 )
    {
        ASSERT( !"Rank Must smaller than 9999" );
        return;
    }

    for ( int i = 0; i < 4; ++i )
        JIF( m_pkMyCurrentRanking[i] != NULL );

    for ( int i = 0; i < 4; ++i )
    {
        if ( i < iDigit )
        {
            m_pkMyCurrentRanking[i]->SetNumber( GetDigitNumber( iTmp, i+1 ) );
            m_pkMyCurrentRanking[i]->ToggleRender( true );
        }
        else
        {
            m_pkMyCurrentRanking[i]->ToggleRender( false );
        }
    }
}

void KGCMinigameResult::SetHighScore( const int iHighScore )
{
    int iTmp;
    if ( iHighScore < 0 )
        iTmp = 0;
    else
        iTmp = iHighScore;

    int iDigit = GetDigit(iTmp);
    if ( iDigit > 4 )
    {
        ASSERT( !"HighScore Must smaller than 9999" );
        return;
    }

    for ( int i = 0; i < 4; ++i )
        JIF( m_pkHighScore[i] != NULL );

    for ( int i = 0; i < 4; ++i )
    {
        if ( i < iDigit )
        {
            m_pkHighScore[i]->SetNumber( GetDigitNumber( iTmp, i+1 ) );
            m_pkHighScore[i]->ToggleRender( true );
        }
        else
        {
            m_pkHighScore[i]->ToggleRender( false );
        }
    }
}

void KGCMinigameResult::SetCurrentScore( const int iCurrentScore )
{
    int iTmp;
    if ( iCurrentScore < 0 )
        iTmp = 0;
    else
        iTmp = iCurrentScore;

    int iDigit = GetDigit(iTmp);
    if ( iDigit > 4 )
    {
        ASSERT( !"CurrentScore Must smaller than 9999" );
        return;
    }

    for ( int i = 0; i < 4; ++i )
        JIF( m_pkCurrentScore[i] != NULL );

    for ( int i = 0; i < 4; ++i )
    {
        if ( i < iDigit )
        {
            m_pkCurrentScore[i]->SetNumber( GetDigitNumber( iTmp, i+1 ) );
            m_pkCurrentScore[i]->ToggleRender( true );
        }
        else
        {
            m_pkCurrentScore[i]->ToggleRender( false );
        }
    }
}

void KGCMinigameResult::SetPlayTime( void )
{
    DWORD sec   = g_pGameBoard->GetPlayTimeInFrame()/GC_FPS_LIMIT%60;
    DWORD minute= g_pGameBoard->GetPlayTimeInFrame()/GC_FPS_LIMIT/60;
    SetSec( static_cast<int>( sec ) );
    SetMinute( static_cast<int>( minute ) );
}

int KGCMinigameResult::GetDigit( const int iValue )
{
    int count = 0;
    int tmp = iValue;

    // if you want print '0'
    if ( tmp == 0 )
        return 1;

    while ( tmp != 0 )
    {
        tmp = static_cast<int>(tmp * 0.1);
        count++;
    }

    return count;
}

int KGCMinigameResult::GetDigitNumber( const int iValue, const int iOffset )
{
    if ( iOffset < 1 )
        assert(!"일의자리 보다 작은자리가 어딧니? 소수는 안구해줘!!");
    int iRet = iValue % static_cast<int>(powf( 10, (float)iOffset ));
    iRet /= static_cast<int>(powf( 10, (float)(iOffset - 1) ));

    return iRet;
}

void KGCMinigameResult::InitData( void )
{
    InitNumberTextureCoordinate();
    InitShowGradeEffect();

    m_bIniValue = false;
    m_iGrade = 0;
    m_dwStartTime = timeGetTime();
}

void KGCMinigameResult::SetTotalStar( const int iNumOfStars )
{
    m_pkRankStarScore->SetTotalScore(iNumOfStars);

    if( iNumOfStars == 5 )
        SetTotalScoreRankImage(QUEST_RG_PERFECT);
    else if( iNumOfStars == 4 || iNumOfStars == 3 || iNumOfStars == 2 )
        SetTotalScoreRankImage(QUEST_RG_GOOD);
    else if( iNumOfStars == 1 )
        SetTotalScoreRankImage(QUEST_RG_BAD);
}

void KGCMinigameResult::SetTotalScoreRankImage( int iRank )
{
    if( 0 > iRank ) iRank = 0;
    if( (NUM_OF_QUEST_RG) <= iRank ) iRank = (NUM_OF_QUEST_RG-1);

    for(int i=0;i<NUM_OF_QUEST_RG;++i)
    {
        m_pkRankGrade[i]->ToggleRender(false);
    }

    m_iGrade = iRank;
    SetShowGradeEffect(iRank, (int)QUEST_RANK_SCALE_FRAME, (int)QUEST_RANK_SHOW_FRAME, QUEST_RANK_MAX_SCALE, QUEST_RANK_MIN_SCALE);
}

void KGCMinigameResult::SetShowGradeEffect(int iGrade, int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent)
{
    KGCD3DWndEffect     *pEffect        = NULL;
    KD3DWnd             *pkWindow       = NULL;

    if( iGrade >= 0 && iGrade < NUM_OF_QUEST_RG )
    {
        pEffect                 = &m_GradeInfo[iGrade];
        pkWindow                = m_pkRankGrade[iGrade];
    }

    if( pEffect && pkWindow )
    {
        pEffect->SetFramePerAlterationScale(iScaleFrame, iShowFrame, iStartScalePercent, iDestScalePercent);
        pEffect->SetFramePerAlterationBlend(iScaleFrame, iShowFrame, 255, 0);

        // 처음 시작 값 설정
        pkWindow->SetWidthDirect((DWORD)pEffect->GetCurrentWidth());
        pkWindow->SetHeightDirect((DWORD)pEffect->GetCurrentHeight());

        // 스케일이 변경됨에 따라 UI의 위치가 조금씩 보정되야 함... 
        const float fChangeX = (pEffect->GetCurrentWidth()*0.5f - pEffect->GetOriginalWidth()*0.5f);
        const float fChangeY = (pEffect->GetCurrentHeight()*0.5f - pEffect->GetOriginalHeight()*0.5f);
        pkWindow->SetWindowPosDirect( pEffect->GetOriginalPos() - D3DXVECTOR2(fChangeX,fChangeY));
    }
}

void KGCMinigameResult::InitShowGradeEffect(void)
{
    // 이펙트로 표시해줄 (윈도우)KD3DWnd의 초기 위치를 저장 및 설정한다.
    if( !m_bIniValue )
    {
        for(int i=0;i<NUM_OF_QUEST_RG;++i)
        {
            m_GradeInfo[i].InitValue( (int)m_pkRankGrade[i]->GetFixedWindowLocalPos().x, (int)m_pkRankGrade[i]->GetFixedWindowLocalPos().y
                , (int)m_pkRankGrade[i]->GetWidth(), (int)m_pkRankGrade[i]->GetHeight() );
        }
        m_bIniValue = true;
    }
    else
    {
        for(int i=0;i<NUM_OF_QUEST_RG;++i)
        {
            m_pkRankGrade[i]->SetFixedWindowLocalPos( m_GradeInfo[i].GetOriginalPos() );
            m_pkRankGrade[i]->SetWidthDirect(m_GradeInfo[i].GetOriginalWidth());
            m_pkRankGrade[i]->SetHeightDirect(m_GradeInfo[i].GetOriginalHeight());
        }
    }
}

void KGCMinigameResult::FrameMove_GradeEffect( KGCD3DWndEffect *pEffectInfo, KD3DWnd *pWindow, bool bEvent /*= false */ )
{
    if( !pEffectInfo || !pWindow ) return;

    // 표시 여부를 확인하는 부분
    if( !pEffectInfo->IsEndOfShow() )
    {
        pEffectInfo->ShowStep();

        // 스케일을 진행하는 부분
        if( !pEffectInfo->IsEndOfScale() )
        {
            pEffectInfo->ScaleStep();        

            // 스케일이 변경됨에 따라 UI의 위치가 조금씩 보정되야 함... 
            const float fChangeX = static_cast<float>(pEffectInfo->GetCurrentWidth()/2 - pEffectInfo->GetOriginalWidth()/2);
            const float fChangeY = static_cast<float>(pEffectInfo->GetCurrentHeight()/2 - pEffectInfo->GetOriginalHeight()/2);

            pWindow->SetWindowPosDirect( pEffectInfo->GetOriginalPos() - D3DXVECTOR2(fChangeX,fChangeY));
            pWindow->SetWidthDirect((int)pEffectInfo->GetCurrentWidth());
            pWindow->SetHeightDirect((int)pEffectInfo->GetCurrentHeight());
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 123, 0xff, 0xff, 0xff));

            if( pEffectInfo->IsEndOfScale() )
                g_KDSound.Play( "136" );
        }
        else
        {
            pWindow->SetWindowColor(D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff));
        }
        pWindow->ToggleRender( (pEffectInfo->GetCurrentWidth() < 1000) );
    }
    else if( !pEffectInfo->IsEndOfBlend() )
    {
        pEffectInfo->BlendStep();
    }

    if( pEffectInfo->IsEndOfShow() )
    {
        pWindow->ToggleRender(true);
    }
}

void KGCMinigameResult::SetRankLowDesc( bool bToggle )
{
    m_pkRankLowDesc->ToggleRender( bToggle );
	m_pkRankDesc->ToggleRender( !bToggle );
	for ( int i = 0; i < 4 ; ++i )
	{
		m_pkMyCurrentRanking[i]->ToggleRender( !bToggle );
	}
}

void KGCMinigameResult::SetSec( const int iSec )
{
    int iTmp;
    if ( iSec < 0 )
        iTmp = 0;
    else
        iTmp = iSec;

    int iDigit = GetDigit(iSec);
    if ( iDigit > 2 )
    {
        ASSERT( !"CurrentScore Must smaller than 9999" );
        return;
    }

    for ( int i = 0; i < 2; ++i )
        JIF( m_pkPlayTime[i] != NULL );

    for ( int i = 0; i < 2; ++i )
    {
        if ( i < iDigit )
        {
            m_pkPlayTime[i]->SetNumber( GetDigitNumber( iTmp, i+1 ) );
            m_pkPlayTime[i]->ToggleRender( true );
        }
        else
        {
            m_pkPlayTime[i]->ToggleRender( false );
        }
    }
}

void KGCMinigameResult::SetMinute( const int iMinute )
{
    int iTmp;
    if ( iMinute < 0 )
        iTmp = 0;
    else
        iTmp = iMinute;

    int iDigit = GetDigit(iMinute);
    if ( iDigit > 2 )
    {
        ASSERT( !"CurrentScore Must smaller than 9999" );
        return;
    }

    for ( int i = 2; i < 4; ++i )
        JIF( m_pkPlayTime[i] != NULL );

    for ( int i = 0; i < 2; ++i )
    {
        if ( i < iDigit )
        {
            m_pkPlayTime[i+2]->SetNumber( GetDigitNumber( iTmp, i+1 ) );
            m_pkPlayTime[i+2]->ToggleRender( true );
        }
        else
        {
            m_pkPlayTime[i+2]->ToggleRender( false );
        }
    }
}
