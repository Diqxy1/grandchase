#include "StdAfx.h"
#include "KGCDicePlay.h"

#include "KGCDicePlay_DiceView.h"
#include "../GCItemManager.h"
#include "GCItemReceiveBox.h"

IMPLEMENT_CLASSNAME( KGCDicePlay );
IMPLEMENT_WND_FACTORY( KGCDicePlay );
IMPLEMENT_WND_FACTORY_NAME( KGCDicePlay, "gc_halloween_board" );

KGCDicePlay::KGCDicePlay(void)
{
	m_dwCurrentPosition = 0;
	m_dwTotalCount		= 0;
	m_dwRemainCount		= 0;
	m_dwGiveCount		= 0;
    m_dwFreeDiceTick    = 0;
	m_dwRemainTime		= 0;
	m_dwNumDice			= 0;
	m_dwAccTimeNot		= 0;
	m_iState			= 0;
	m_iFrame			= 0;
	m_iErrorStringNum	= -1;
	m_bNoFreeDice		= false;
	m_bRcvDice			= false;


	m_vecRanker.clear();

	m_pkCloseBtn = NULL;
	m_pkPlayeBtn = NULL;
	m_pkReciveBtn = NULL;
	m_pkMyPlayer = NULL;
	m_pkStaticMin = NULL;
	m_pkStaticSec = NULL;
	m_pkStaticGoal = NULL;
	m_pkStaticLimit = NULL;
	m_pkStaticDice = NULL;
	
	LINK_CONTROL( "close_button", m_pkCloseBtn );

	LINK_CONTROL( "play_button", m_pkPlayeBtn );

	LINK_CONTROL( "recive_button", m_pkReciveBtn );
	LINK_CONTROL( "myplayer", m_pkMyPlayer );
	LINK_CONTROL( "static_min", m_pkStaticMin );
	LINK_CONTROL( "static_sec", m_pkStaticSec );
	LINK_CONTROL( "static_goal", m_pkStaticGoal );
	LINK_CONTROL( "static_limit", m_pkStaticLimit );
	LINK_CONTROL( "static_dice", m_pkStaticDice );

	char str[256] = "";
	for( int i = 0; i < MAX_RANKING ; ++i )
	{
		m_pkRanker[i] = NULL;
		
		sprintf( str, "player%d", i );
		LINK_CONTROL( str, m_pkRanker[i] );
	}

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

KGCDicePlay::~KGCDicePlay(void)
{
}

void KGCDicePlay::OnCreate( void )
{
	if(m_pkCloseBtn)
	{
		m_pkCloseBtn->InitState( true, true, this );
		m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
	}
	if(m_pkPlayeBtn)
		m_pkPlayeBtn->InitState( true, true, this );
	if(m_pkReciveBtn)
		m_pkReciveBtn->InitState( true, true, this );

	if(m_pkMyPlayer)
		m_pkMyPlayer->InitState( true );
	
	for( int i = 0; i < MAX_RANKING ; ++i )
	{
		if(m_pkRanker[i])
			m_pkRanker[i]->InitState( false );
	}

	for(int i = 0 ; i < MAX_BOARD_NUM; ++i)
	{
		if(m_pkBoardItem[i])
			m_pkBoardItem[i]->InitState( false );
	}

	if(m_pkStaticMin)
	{
		m_pkStaticMin->InitState(true);
		m_pkStaticMin->SetFontColor( 0xFFFFFFFF );
		m_pkStaticMin->SetAlign( DT_CENTER );
	}

	if(m_pkStaticSec)
	{
		m_pkStaticSec->InitState(true);
		m_pkStaticSec->SetFontColor( 0xFFFFFFFF );
		m_pkStaticSec->SetAlign( DT_CENTER );
	}

	if(m_pkStaticGoal)
	{
		m_pkStaticGoal->InitState(true);
		m_pkStaticGoal->SetFontColor( 0xFFFFFFFF );
		m_pkStaticGoal->SetAlign( DT_CENTER );
	}

	if(m_pkStaticLimit)
	{
		m_pkStaticLimit->InitState(false);
		m_pkStaticLimit->SetFontColor( 0xFFFFFFFF );
		m_pkStaticLimit->SetAlign( DT_CENTER );
	}

	if(m_pkStaticDice)
	{
		m_pkStaticDice->InitState(true);
		m_pkStaticDice->SetFontColor( 0xFFFFFFFF );
		m_pkStaticDice->SetAlign( DT_CENTER );
	}

	if(m_pkDice[0])
		m_pkDice[0]->InitState(true);

	if(m_pkDice[1])
		m_pkDice[1]->InitState(true);	

	//InitNumberTextureCoordinate();
	m_iState = REFRESH;
	SettingBoard();
}

void KGCDicePlay::ActionPerformed( const KActionEvent& event )
{
	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
		return;

	GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	GCWND_MSG_MAP( m_pkPlayeBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnDicePlay );
	GCWND_MSG_MAP( m_pkReciveBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnRecive );

}

void KGCDicePlay::FrameMoveInEnabledState( void )
{
	int iTime = (timeGetTime() - m_dwAccTimeNot );

	if( iTime >= 1000 )	// 1초 이상 지났으면
	{
		m_dwAccTimeNot = timeGetTime();
		m_dwRemainTime--;
		
        if( m_dwRemainTime < 0 || m_dwRemainTime > m_dwFreeDiceTick )
        {
            m_dwRemainTime = m_dwFreeDiceTick;
            if( !m_bNoFreeDice )
            {
                m_bRcvDice = true;
                m_pkReciveBtn->Lock( false );
            }
        }

		SetPlayTime();
	}

	switch( m_iState)
	{
		case PLAYINGDICE:
			{
				//굴릴때는 활성화 시키지 않겠다.
				m_pkPlayeBtn->Lock( true );
				m_pkReciveBtn->Lock( true );

				m_pkDice[0]->FrameMoveIn_DiceView( m_iFrame );
				m_pkDice[1]->FrameMoveIn_DiceView( m_iFrame );

				m_iFrame--;
				if( m_iFrame < 0)
				{
					m_iState = (int)MOVEPLAYER;
					m_iFrame = (( m_dwMovePosition + MAX_BOARD_NUM - m_dwCurrentPosition ) % MAX_BOARD_NUM) * 25;	// 이동해야할 칸 수 * 이동할 프레임
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

				if( m_iFrame == 0 && m_iErrorStringNum != -1 )
				{
					if( m_iErrorStringNum == (int)STR_ID_REGISTER_MISSION_ERROR0 )
					{
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK2,g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR0 ), 
							L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
					}
					else if( m_iErrorStringNum == (int)STR_ID_REGISTER_MISSION_ERROR6 )
					{
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK2,g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR6 ), 
							L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
					}
					m_iErrorStringNum = -1;
				}
			}
			break;
		case RECEIVEITEM:
			{				
				bool bRecive = false;

                if(m_pkDice[0]->GetDiceNum()  == m_pkDice[1]->GetDiceNum() && m_pkDice[0]->GetDiceNum() != 0) {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_DICE_DOUBLE ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );						
                }
				
				if( !m_vecReciveItem.empty() )
				{
					for( int i = 0; i < (int)m_vecReciveItem.size() ; i++ )
                    {
                        KItem& kReward = m_vecReciveItem[i];
						//g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, kReward.m_ItemID/10, 0, false, true, true );
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, kReward.m_ItemID/10, 0, false, true );
                    }

                    g_pItemMgr->m_kInventory.AddItemList( m_vecReciveItem );
					bRecive = true;
					m_vecReciveItem.clear();
				}

				if( bRecive )
				{
					g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
					g_pkUIScene->m_pkMyInfoScene->UpdateData();
				}
				m_iState = (int)REFRESH;
				m_pkCloseBtn->Lock(false);
			}
			break;
		case REFRESH:
			{
				if( m_dwNumDice <= 0 )
					m_pkPlayeBtn->Lock( true );
				else
					m_pkPlayeBtn->Lock( false );

				SetMyPosition();
				SetRankerPosition();
				SetStatic();
				SetPlayTime();

				m_iState = -1;
			}
			break;
		default:
			m_pkReciveBtn->Lock( !m_bRcvDice );
			break;
	}
}

void KGCDicePlay::OnDestroy( void )
{
}

void KGCDicePlay::InitNumberTextureCoordinate( void )
{
	for( int j = 0; j< 2; ++j)
	{
		m_pkNumber[0][j] = NULL;

		char str[256] = "";
		sprintf( str, "number%d", j );
		m_pkNumber[0][j] = static_cast<KD3DNumber*>(m_pkRanker[0]->GetChildByName( str ));
	}

	D3DXVECTOR4 vStartCoord1( 216.0f, 157.0f, 243.0f, 189.0f );
	int iOffsetArray1[] = { 0, 29, 30, 29, 28, 29, 29, 28, 30, 28 };

	for ( int i = 0; i < 10; ++i )
	{
		vStartCoord1.x += iOffsetArray1[i];
		vStartCoord1.z += iOffsetArray1[i];

		m_pkNumber[0][0]->SetNumberCoordi( i, vStartCoord1 );
	}

	m_pkNumber[0][1]->SetTexCoordi( m_pkNumber[0][0]->GetTexCoordi() );
}
void KGCDicePlay::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	ReleaseCapture();

	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCDicePlay::OnDicePlay()
{
	m_pkPlayeBtn->Lock( true );
	m_pkCloseBtn->Lock( true );

	if( m_iState == DEFAULT )
		KP2P::GetInstance()->Send_DicePlayReq();

	//g_pItemMgr->DecInventoryDurationItemForItemID( KGCItemManager::ITEM_HALLOWEEN_DICE );
	//g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCDicePlay::SetMyBoardInfo( KUserBoardInfo kBoardinfo )
{
	m_dwTotalCount = kBoardinfo.m_dwTotalCount;
	m_dwRemainCount = kBoardinfo.m_dwRemainCount;
	m_dwGiveCount = kBoardinfo.m_dwGiveCount;
	m_dwRemainTime = kBoardinfo.m_dwRemainTime;
	m_dwNumDice = kBoardinfo.m_dwDiceCount;
	m_dwMovePosition = kBoardinfo.m_dwCurrentPosition;
}

void KGCDicePlay::SetBoardItemInfo( std::map<DWORD, DWORD> mapBoardInfo_, std::map<DWORD, std::vector<KDropItemInfo> > mapRewardInfo_ )
{
//	mapBoardInfo_ first : 말판 번호 second : 아이템 타입.
//	mapRewardInfo_ first : 아이템 타입 second : 아이템 정보.
//	m_mapBoardInfo first : 말판 번호 second : 아이템 아이디
	m_mapBoardInfo.clear();

	std::map<DWORD,DWORD>::iterator iterboardInfo = mapBoardInfo_.begin();
	std::map<DWORD, std::vector<KDropItemInfo> >::iterator IterReword; 
	for(;iterboardInfo != mapBoardInfo_.end();iterboardInfo++)
	{
		std::pair<DWORD,DWORD> pairInfo;
		IterReword = mapRewardInfo_.find(iterboardInfo->second);
		if(IterReword != mapRewardInfo_.end() && 
            IterReword->second.begin() != IterReword->second.end() ){ //&&
            //iterboardInfo->second != KEVENT_BOARD_GAME_INFO_ACK::P_NORMAL ){
			m_mapBoardInfo.insert(std::map<DWORD,DWORD>::value_type(iterboardInfo->first,IterReword->second.begin()->m_ItemID));
		}
		else{
			m_mapBoardInfo.insert(std::map<DWORD,DWORD>::value_type(iterboardInfo->first,0));
		}
	}
}

void KGCDicePlay::SettingBoard()
{
//         P_NORMAL            = 0,            // 일반칸
//         P_ACCESSARIES       = 1,            // 악세사리 지급칸.
//         P_POSITION          = 2,            // 포션 지급칸
//         P_GEM               = 3,            // GEM 칸
//         P_AGINGCORE         = 4,            // 비쥬 칸
//         P_CHAMPION          = 5,            // 챔피언 입장권 칸
//         P_CONTINENT1        = 6,            // 대륙별 상자칸1
//         P_CONTINENT2        = 7,            // 대륙별 상자칸2
//         P_CONTINENT3        = 8,            // 대륙별 상자칸3
//         P_CONTINENT4        = 9,            // 대륙별 상자칸4
//         P_SPECIALMISSION    = 10,           // 특수 미션칸

	std::map<DWORD,DWORD>::iterator mapIter = m_mapBoardInfo.begin();
	
	for(;mapIter != m_mapBoardInfo.end();mapIter++){
#if defined(NATION_KOREA)
        if( (mapIter->second != 0) && (mapIter->first % 6 != 0) )
#else
		if(mapIter->second != 0)
#endif
		{
			m_pkBoardItem[mapIter->first]->ToggleRender( true );
			m_pkBoardItem[mapIter->first]->SetItemInfo( mapIter->second/10, 0xFFFFFFFF );
			m_pkBoardItem[mapIter->first]->ShowItem( true );

		}
		else
		{
			m_pkBoardItem[mapIter->first]->ToggleRender( false );
		}
	}
}

void KGCDicePlay::SetPlayTime()
{
	if( m_bNoFreeDice )
	{
		m_pkStaticMin->SetText( 0 );
		m_pkStaticSec->SetText( 0 );
	}
	else
	{
		WCHAR strTemp[20];
		swprintf( strTemp, 20, L"%02d", (int)( m_dwRemainTime / 60 ));
		m_pkStaticMin->SetText( strTemp );

		swprintf( strTemp, 20, L"%02d", (int)( m_dwRemainTime % 60 ) );
		m_pkStaticSec->SetText( strTemp );
	}	
}

void KGCDicePlay::SetStatic()
{
	WCHAR strTemp[20];
	swprintf( strTemp, 20, L"%02d", (int)m_dwTotalCount );
	m_pkStaticGoal->SetText( strTemp );

	swprintf( strTemp, 20, L"%02d", (int)m_dwRemainCount );
	m_pkStaticLimit->SetText( strTemp );

	swprintf( strTemp, 20, L"%02d", (int)m_dwNumDice );
	m_pkStaticDice->SetText( strTemp );
}

void KGCDicePlay::SetMyPosition()
{
	if( m_dwMovePosition < 0 || m_dwMovePosition >= MAX_BOARD_NUM )
	{
		m_dwMovePosition = m_dwCurrentPosition;
	}

	if( m_dwMovePosition == m_dwCurrentPosition )
		return;

	if( m_iState == REFRESH )
		m_dwCurrentPosition = m_dwMovePosition;
	else if( m_iFrame > 0 && (m_iFrame % 25 == 0))
	{
		m_dwCurrentPosition++;
		g_KDSound.Play("MovePlayer");
	}

	if( m_dwCurrentPosition >= MAX_BOARD_NUM )
		m_dwCurrentPosition -= MAX_BOARD_NUM;
	m_pkMyPlayer->SetWindowPos( m_pkBoardItem[m_dwCurrentPosition]->GetFixedWindowLocalPos() );
}

void KGCDicePlay::SetRankerPosition()
{
	return;
	// 이거 왜 리턴되어있지 ;;
	// 대만에서 사용시 실시간 변경이 되지 않아... 사용 안되었음...
	// 한국도 사용 안함...
	// 

	if( m_vecRanker.empty())
		return;


	for( int i = 0; i < MAX_RANKING; ++i )
	{
		if( m_vecRanker[i].m_dwCurrentPosition < 0 || m_vecRanker[i].m_dwCurrentPosition >= 24 )
			continue;

		m_pkRanker[i]->ToggleRender( true );
		D3DXVECTOR2 vPos = m_pkBoardItem[m_vecRanker[i].m_dwCurrentPosition]->GetFixedWindowLocalPos();
		m_pkRanker[i]->SetWindowPos( vPos );

		m_pkNumber[i][0]->SetNumber( (UINT)m_vecRanker[i].m_dwTotalCount%10 );	// 1자리
		m_pkNumber[i][1]->SetNumber( (m_vecRanker[i].m_dwTotalCount >= 10 ? (UINT)m_vecRanker[i].m_dwTotalCount/10 : 0) );
	}
}

void KGCDicePlay::OnRecive()
{
	KP2P::GetInstance()->Send_BoardGameInfoReq();
	m_bRcvDice = false;
}

void KGCDicePlay::PlayResult( KEVENT_DICE_PLAY_ACK kResult )
{
	m_dwNumDice--;

	SetMyBoardInfo(kResult.m_kMyBoardInfo);

	if( kResult.m_nOK == 0 || kResult.m_nOK == 8 )
	{
		m_vecReciveItem = kResult.m_vecItem;
	}

	if( !kResult.m_vecMissionList.empty() )
	{
		for ( int i = 0 ; i < (int)kResult.m_vecMissionList.size() ; i++ )
		{
			g_kGlobalValue.m_kUserMission.AddMission( kResult.m_vecMissionList[i].m_dwMissionID );
			g_kGlobalValue.m_kUserCollectionMission.AddCollectionMission( kResult.m_vecMissionList[i].m_dwMissionID );
		}

		g_kGlobalValue.m_kUserMission.SetAutoMission( kResult.m_vecMissionList );

		m_iErrorStringNum = (int)STR_ID_REGISTER_MISSION_ERROR0;		
	}	

    if( kResult.m_nOK == 8 )
		m_iErrorStringNum = (int)STR_ID_REGISTER_MISSION_ERROR6;

	m_pkDice[0]->SetDiceNum( kResult.m_FirstDiceNum );
	m_pkDice[1]->SetDiceNum( kResult.m_SecondDiceNum );



	m_iFrame = 167;
	m_iState = PLAYINGDICE;
	g_KDSound.Play("RollDice");
}
