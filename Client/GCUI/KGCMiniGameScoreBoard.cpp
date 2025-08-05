#include "stdafx.h"
#include "KGCMinigameScoreBoard.h"
//

//


IMPLEMENT_CLASSNAME( KGCMinigameScoreBoard );
IMPLEMENT_WND_FACTORY( KGCMinigameScoreBoard );
IMPLEMENT_WND_FACTORY_NAME( KGCMinigameScoreBoard, "gc_minigame_scoreboard" );

KGCMinigameScoreBoard::KGCMinigameScoreBoard(void)
{
    /*
    for ( int i = 0; i < 4; ++i )
    {
        //m_pkHighScore[i] = NULL;
        m_pkCurrentScore[i] = NULL;
        
    //    if ( i < 3 )
    //    {
    //        m_pkGP[i] = NULL;
    //    }
    }

    char str[MAX_PATH];
    for ( int i = 0; i < 4 ; ++i )
    {
        sprintf( str, "number%d", i );
        LINK_CONTROL( str, m_pkCurrentScore[i] );

        //sprintf( str, "highscore_number%d", i );
        //LINK_CONTROL( str, m_pkHighScore[i] );

        //if ( i < 3 )
        //{
        //    sprintf( str, "gp%d", i );
        //    LINK_CONTROL( str, m_pkGP[i] );
        //}
    }
*/

    m_pkScore = NULL;
    LINK_CONTROL( "score", m_pkScore );
}

KGCMinigameScoreBoard::~KGCMinigameScoreBoard(void)
{
}

void KGCMinigameScoreBoard::OnCreate( void )
{
    //for ( int i = 0; i < 4; ++i )
    //{
    //    m_pkHighScore[i]->InitState( false );
    //    m_pkCurrentScore[i]->InitState( false );
    //    if ( i < 3 )
    //        m_pkGP[i]->InitState( false );
    //}

    m_pkScore->InitState( true );
    m_pkScore->SetStringType( "orange_gradation_num4", false );
    m_pkScore->SetWriteToRight( false );

    InitData();
}

void KGCMinigameScoreBoard::SetHighScore( const int iHighScore )
{
    /*int iTmp;
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
    }*/
}

void KGCMinigameScoreBoard::SetCurrentScore( const int iCurrentScore )
{
    // 
    m_pkScore->SetNumber( iCurrentScore );

    //int iTmp;
    //if ( iCurrentScore < 0 )
    //    iTmp = 0;
    //else
    //    iTmp = iCurrentScore;

    //int iDigit = GetDigit(iTmp);
    //if ( iDigit > 4 )
    //{
    //    ASSERT( !"CurrentScore Must smaller than 9999" );
    //    return;
    //}

    //for ( int i = 0; i < 4; ++i )
    //    JIF( m_pkCurrentScore[i] != NULL );

    //for ( int i = 0; i < 4; ++i )
    //{
    //    if ( i < iDigit )
    //    {
    //        m_pkCurrentScore[i]->SetNumber( GetDigitNumber( iTmp, i+1 ) );
    //        m_pkCurrentScore[i]->ToggleRender( true );
    //    }
    //    else
    //    {
    //        m_pkCurrentScore[i]->ToggleRender( false );
    //    }
    //}
}

int KGCMinigameScoreBoard::GetDigit( const int iValue )
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

int KGCMinigameScoreBoard::GetDigitNumber( const int iValue, const int iOffset )
{
    if ( iOffset < 1 )
        assert(!"일의자리 보다 작은자리가 어딧니? 소수는 안구해줘!!");
    int iRet = iValue % static_cast<int>(powf( 10, (float)iOffset ));
    iRet /= static_cast<int>(powf( 10, (float)(iOffset - 1 )));

    return iRet;
}

void KGCMinigameScoreBoard::InitData( void )
{
    //InitNumberTextureCoordinate();
    SetCurrentScore( 0 );
    //SetHighScore( 0 );
    //SetGP( 0 );
}

void KGCMinigameScoreBoard::InitNumberTextureCoordinate( void )
{
    // 아 귀찮아.. 한나나에게 잘 부탁하면 이런짓 안해도 될텐데 ㅎㅎ
    D3DXVECTOR4 vStartCoord1( 22.0f, 194.0f, 35.0f, 210.0f );
    //D3DXVECTOR4 vStartCoord2( 237.0f, 159.0f, 250.0f, 176.0f );
    int iOffsetArray1[] = { 0, 17, 17, 17, 17, 16, 17, 17, 16, 16 };
    //int iOffsetArray2[] = { 0, 16, 16, 17, 17, 16, 17, 17, 16, 16 };

    for ( int i = 0; i < 10; ++i )
    {
        vStartCoord1.x += iOffsetArray1[i];
        //vStartCoord2.x += iOffsetArray2[i];
        vStartCoord1.z += iOffsetArray1[i];
        //vStartCoord2.z += iOffsetArray2[i];

        m_pkCurrentScore[0]->SetNumberCoordi( i, vStartCoord1 );
        //m_pkHighScore[0]->SetNumberCoordi( i, vStartCoord2 );
    }

    for ( int i = 0; i < 4; ++i )
    {
        m_pkCurrentScore[i]->SetTexCoordi( m_pkCurrentScore[0]->GetTexCoordi() );
        //m_pkHighScore[i]->SetTexCoordi( m_pkHighScore[0]->GetTexCoordi() );
        //if ( i < 3 )
        //    m_pkGP[i]->SetTexCoordi( m_pkCurrentScore[0]->GetTexCoordi() );
    }
}

void KGCMinigameScoreBoard::SetGP( const int iGp )
{
    /*int iTmp;
    if ( iGp < 0 )
        iTmp = 0;
    else
        iTmp = iGp;

    int iDigit = GetDigit(iGp);
    if ( iDigit > 3 )
    {
        ASSERT( !"CurrentScore Must smaller than 9999" );
        return;
    }

    for ( int i = 0; i < 3; ++i )
        JIF( m_pkGP[i] != NULL );

    for ( int i = 0; i < 3; ++i )
    {
        if ( i < iDigit )
        {
            m_pkGP[i]->SetNumber( GetDigitNumber( iTmp, i+1 ) );
            m_pkGP[i]->ToggleRender( true );
        }
        else
        {
            m_pkGP[i]->ToggleRender( false );
        }
    }*/
}
