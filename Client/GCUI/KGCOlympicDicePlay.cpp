#include "StdAfx.h"
#include "KGCOlympicDicePlay.h"

#include "KGCOlympicDicePlay_DiceView.h"
#include "../GCItemManager.h"
#include "GCItemReceiveBox.h"

IMPLEMENT_CLASSNAME( KGCOlympicDicePlay );
IMPLEMENT_WND_FACTORY( KGCOlympicDicePlay );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicDicePlay, "gc_halloween_board_olympic" );

KGCOlympicDicePlay::KGCOlympicDicePlay(void)
{
	m_dwCurrentPosition = 0;
    m_dwMovePosition    = 0;
	m_dwTotalCount		= 0;
	m_dwNumDice			= 0;
	m_iState			= 0;
	m_iFrame			= 0;

    m_tmTodayDate       = 0;
    m_nGameType         = 0;  
    m_nTeamType         = 0;

	m_pkCloseBtn = NULL;
	m_pkPlayeBtn = NULL;
	m_pkMyPlayer = NULL;
	m_pkStaticGoal = NULL;
	m_pkStaticLimit = NULL;
	m_pkStaticDice = NULL;
	
	LINK_CONTROL( "close_button", m_pkCloseBtn );
	LINK_CONTROL( "play_button", m_pkPlayeBtn );
	LINK_CONTROL( "static_goal", m_pkStaticGoal );
	LINK_CONTROL( "static_limit", m_pkStaticLimit );
	LINK_CONTROL( "static_dice", m_pkStaticDice );
    LINK_CONTROL( "myplayer", m_pkMyPlayer );

    char str[256] = "";
	for(int i = 0 ; i < MAX_BOARD_NUM; ++i)
	{
		m_pkBoardItem[i] = NULL;

		sprintf(str, "item_img%d", i);
		LINK_CONTROL( str, m_pkBoardItem[i] );
	}

	for( int i = 0; i < NUM_DICE; ++i)
	{
		m_pkDice[i] = NULL;

		sprintf( str, "dice%d", i);
		LINK_CONTROL( str, m_pkDice[i] );
	}
}

KGCOlympicDicePlay::~KGCOlympicDicePlay(void)
{
}

void KGCOlympicDicePlay::OnCreate( void )
{
	if(m_pkCloseBtn)
	{
		m_pkCloseBtn->InitState( true, true, this );
		m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
	}
	if(m_pkPlayeBtn)
		m_pkPlayeBtn->InitState( true, true, this );

	if(m_pkMyPlayer)
		m_pkMyPlayer->InitState( true );
	
	for(int i = 0 ; i < MAX_BOARD_NUM; ++i)
	{
		if(m_pkBoardItem[i])
			m_pkBoardItem[i]->InitState( false );
	}


	if(m_pkStaticGoal)
	{
		m_pkStaticGoal->InitState(true);
		m_pkStaticGoal->SetFontColor( 0xFFFFFF00 );
		m_pkStaticGoal->SetAlign( DT_CENTER );
	}

	if(m_pkStaticLimit)
	{
		m_pkStaticLimit->InitState(false);
		m_pkStaticLimit->SetFontColor( 0xFFFFFF00 );
		m_pkStaticLimit->SetAlign( DT_CENTER );
	}

	if(m_pkStaticDice)
	{
		m_pkStaticDice->InitState(true);
		m_pkStaticDice->SetFontColor( 0xFFFFFF00 );
		m_pkStaticDice->SetAlign( DT_CENTER );
	}

	if(m_pkDice[0])
		m_pkDice[0]->InitState(true);

	if(m_pkDice[1])
		m_pkDice[1]->InitState(true);	

	m_iState = REFRESH;

}

void KGCOlympicDicePlay::ActionPerformed( const KActionEvent& event )
{
	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
		return;

	GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	GCWND_MSG_MAP( m_pkPlayeBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnDicePlay );

}

void KGCOlympicDicePlay::FrameMoveInEnabledState( void )
{
	switch( m_iState)
	{
		case PLAYINGDICE:
			{
				m_pkPlayeBtn->Lock( true );

				m_pkDice[0]->FrameMoveIn_DiceView( m_iFrame );
				m_pkDice[1]->FrameMoveIn_DiceView( m_iFrame );

				m_iFrame--;
				if( m_iFrame < 0)
				{
					m_iState = (int)MOVEPLAYER;
					m_iFrame = (( m_dwMovePosition + MAX_BOARD_NUM - m_dwCurrentPosition ) % MAX_BOARD_NUM) * 25;
				}
			}
			break;
		case MOVEPLAYER:
			{
				if( m_iFrame < 0)
				{
					m_iState = (int)RECEIVEITEM;
				}
				else
				{
					SetMyPosition();
					m_iFrame--;
				}
			}
			break;
        case RECEIVEITEM:
            {
                if(m_pkDice[0]->GetDiceNum()  == m_pkDice[1]->GetDiceNum() && m_pkDice[0]->GetDiceNum() != 0)
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_DICE_DOUBLE ),
                        L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );						
                }

                m_iState = (int)REFRESH;
                m_pkCloseBtn->Lock(false);
            }
		case REFRESH:
			{
				if( m_dwNumDice <= 0 )
					m_pkPlayeBtn->Lock( true );
				else
					m_pkPlayeBtn->Lock( false );

				SetMyPosition();
				SetStatic();

				m_iState = -1;
			}
			break;
		default:
			break;
	}
}

void KGCOlympicDicePlay::OnDestroy( void )
{
}

void KGCOlympicDicePlay::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	ReleaseCapture();

	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCOlympicDicePlay::OnDicePlay()
{
	m_pkPlayeBtn->Lock( true );
	m_pkCloseBtn->Lock( true );
    
	if( m_iState == DEFAULT )
		KP2P::GetInstance()->Send_OlympicDicePlayReq(m_tmTodayDate, m_nGameType, m_nTeamType );
}

void KGCOlympicDicePlay::SetCloseBtn()
{
    m_pkCloseBtn->Lock( false );
}

void KGCOlympicDicePlay::SetMyBoardInfo( KEVENT_OLYMPIC_JOIN_GAME_ACK kResult )
{
        
    m_dwMovePosition = m_dwTotalCount = kResult.m_nMyAccumulateData;       
    m_dwNumDice = kResult.m_nDefaultCount;

    m_tmTodayDate       = kResult.m_tmTodayDate;
    m_nGameType         = kResult.m_nGameType;  
    m_nTeamType         = kResult.m_nTeamType;


}

void KGCOlympicDicePlay::SetStatic()
{
	WCHAR strTemp[20];
	swprintf( strTemp, 20, L"%02d", (int)m_dwTotalCount );
	m_pkStaticGoal->SetText( strTemp );

	swprintf( strTemp, 20, L"%02d", (int)m_dwNumDice );
	m_pkStaticDice->SetText( strTemp );
}

void KGCOlympicDicePlay::SetMyPosition()
{
	if( m_dwMovePosition < 0  )
	{
		m_dwMovePosition = m_dwCurrentPosition;
	}

	if( m_iState == REFRESH )
		m_dwCurrentPosition = m_dwMovePosition;

	else if( m_iFrame > 0 && (m_iFrame % 25 == 0))
	{
		m_dwCurrentPosition++;
		g_KDSound.Play("MovePlayer");
	}

    while ( m_dwCurrentPosition >= MAX_BOARD_NUM )
    {
        m_dwCurrentPosition -= MAX_BOARD_NUM;
    }

	m_pkMyPlayer->SetWindowPos( m_pkBoardItem[m_dwCurrentPosition]->GetFixedWindowLocalPos() );     
}

void KGCOlympicDicePlay::PlayResult( KEVENT_OLYMPIC_DICEPLAY_RESULT_ACK  kResult )
{

    m_dwTotalCount      = kResult.m_nMyAccumulateData;                           
	m_dwNumDice         = kResult.m_nRemainTryCount;                                
    m_dwMovePosition    = m_dwTotalCount;
    m_tmTodayDate       = kResult.m_tmTodayDate;
        
	m_pkDice[0]->SetDiceNum( kResult.m_nFirstDiceNum );
	m_pkDice[1]->SetDiceNum( kResult.m_nSecondDiceNum );

	m_iFrame = 167;
	m_iState = PLAYINGDICE;
	g_KDSound.Play("RollDice");
}
