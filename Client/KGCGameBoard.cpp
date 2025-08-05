#include "stdafx.h"
#include "MyD3D.h"
#include "KGCGameBoard.h"


#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "GCAdventureManager.h"

KGCGameBoard* g_pGameBoard = NULL;

KGCGameBoard::KGCGameBoard(void)
{
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
        m_pStartingMember[i].clear();

	g_pGameBoard = this;
	Init();
    BindLua();
}

KGCGameBoard::~KGCGameBoard(void)
{
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
        m_pStartingMember[i].clear();
}

void KGCGameBoard::Init()
{
	m_mapMonNum.clear();
	m_mapMonKillNum.clear();
    m_vtMonKillTGID.clear();
    
	m_iGameScore = 0;
	m_dwStartTime = timeGetTime();

	for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
	{
        m_pStartingMember[i].clear();	
        Change_How_Score[i] = 0;
        Change_How_Exp[i].clear();
		Change_How_SPExp[i].clear();
	}

    for( int i=0 ; i<TEAM_NUM; ++i )
        m_aiTeamScore[i] = 0;

    m_bComboBreak = true;
}

unsigned short KGCGameBoard::GetMonKillNum( int iMon )
{
	std::map< int, unsigned short >::iterator itor = m_mapMonKillNum.find( iMon );
	if( itor == m_mapMonKillNum.end() )
		return 0;

	return itor->second;
}

unsigned short KGCGameBoard::GetMonsterNum( int iMon )
{
	std::map< int, unsigned short >::iterator itor = m_mapMonNum.find( iMon );
	if( itor == m_mapMonNum.end() )
		return 0;

	return itor->second;
}

void KGCGameBoard::AddMonsterNum( int iMon, int iMonNum )
{
	std::map< int, unsigned short >::iterator itor;

	// 현재 몬스터 개수 세팅
	itor = m_mapMonNum.find( iMon );
	if( itor == m_mapMonNum.end() )
	{
		itor = m_mapMonNum.insert( std::map< int, unsigned short >::value_type( iMon, iMonNum ) ).first;
	}
	else
	{
		itor->second += iMonNum;
	}

	// AI쪽 구조 문제상 몬스터 수가 0보다 작을 수도 있다. ㅡㅡ;
	if( itor->second < 0 )
	{
		itor->second = 0;
		ASSERT( false && "Invalid GameBoard : Monster Num!!" );
		return;
	}

	if( iMonNum < 0 )
	{
		// 잡은 몬스터 개수 세팅
		itor = m_mapMonKillNum.find( iMon );
		if( itor == m_mapMonKillNum.end() )
		{
			itor = m_mapMonKillNum.insert( std::map< int, unsigned short >::value_type( iMon, -iMonNum ) ).first;
		}
		else
		{
			itor->second += (-iMonNum);
		}
	}
}

void KGCGameBoard::AddMonKillNum( int iMon, int iMonNum, int iTGID )
{
    std::map< int, unsigned short >::iterator itor;

    // 현재 몬스터 개수 세팅
    itor = m_mapMonKillNum.find( iMon );
    if( itor == m_mapMonKillNum.end() )
    {
        m_mapMonKillNum[iMon] = iMonNum;
        m_mapMonKillNum.insert( std::map< int, unsigned short >::value_type( iMon, iMonNum ) );
    }
    else
    {
        itor->second += iMonNum;
    }

    m_vtMonKillTGID.push_back( iTGID );

    //GC 원정대(모험자 이벤트) 관련 전리품 획득했다고 띄우는 부분
    if ( SiGCAdventureManager()->isValidEventPeriod() )
    {
        if ( SiGCAdventureManager()->IsValidTriggerID( iTGID ) )
            SiGCAdventureManager()->GetTriggerIDInfoAndAddCnt(iTGID);
    }

    // 방장이면 서버한테 이 몹 죽였따고 보낸다.
    // 게임결과가 결정되었을때는 보내지 않는다.
    if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost && 0 == Result_EndGame )
    {
        EGCGameMode emGameMode = SiKGCRoomManager()->GetGameMode();

        if ( SiKGCWorldMapManager()->IsInfinityDungeon( emGameMode ) )
        {
            SiKP2P()->Send_InfinityDungeonRewardExpReq( iTGID );
        }
        else
        {
            SiKP2P()->Send_DungeonRewardExpReq( iTGID );
        }
    }
}

void KGCGameBoard::SetGameScore( int iPoint )
{ 
	//승패가 결정 났다면 더이상 점수의 변화가 없다.
	if( IsDetermineWin )
		return;
	m_iGameScore = iPoint; 
	if(m_iGameScore < 0)
		m_iGameScore = 0;
}

void KGCGameBoard::OnStartGame()
{
    m_iGameScore = 0;
	m_mapMonNum.clear();
	m_mapMonKillNum.clear();
    m_vtMonKillTGID.clear();
	
    for (int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        m_pStartingMember[i].clear();
    }


    for (int i = 0; i < TEAM_NUM; i++ )
    {
        m_aiTeamScore[i] = 0;
    }

	m_dwStartTime = timeGetTime();
    m_iAttackCount = 0;
}

DWORD KGCGameBoard::GetPlayTime()
{
	return timeGetTime() - m_dwStartTime;
}

int KGCGameBoard::GetHowKill( int iPlayerIndex )
{
	return m_pStartingMember[iPlayerIndex].How_Kill;
}

int KGCGameBoard::GetHowDie( int iPlayerIndex )
{
	return m_pStartingMember[iPlayerIndex].How_Die;
}

bool KGCGameBoard::IsLevelUp( int iPlayerIndex )
{
	return m_pStartingMember[iPlayerIndex].IsLevelUp;
}

int KGCGameBoard::IsSPLevelUp( int iPlayerIndex )
{
	return m_pStartingMember[iPlayerIndex].IsSPLevelUp;
}


bool KGCGameBoard::IsWin( int iPlayerIndex )
{
	return m_pStartingMember[iPlayerIndex].IsWin;
}

bool KGCGameBoard::IsWin()
{
	bool bWin = false;
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( m_pStartingMember[i].IsWin == true )
		{
			bWin = true;
			break;
		}
	}
	return bWin;
}

int KGCGameBoard::GetRank( int iPlayerIndex )

{
	int iScore;
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
         SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        iScore = GetHowKill( iPlayerIndex );
    }
    else
    {
        iScore = m_pStartingMember[iPlayerIndex].GetKillDieScore();
    }
    
	int iCount = 0;
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
        int iCurrentPlayerScore;
        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
             SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
        {
            iCurrentPlayerScore = GetHowKill( i );
        }
        else
        {
            iCurrentPlayerScore = m_pStartingMember[i].GetKillDieScore();
        }

		if( iCurrentPlayerScore > iScore &&
            ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) ) )
		{
			++iCount;
		}
	}
	return iCount;
}

GCEXPTYPE KGCGameBoard::GetChangedExp( int iPlayerIndex )
{
	if( Change_How_Exp.count( iPlayerIndex ) == 0 
        || g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.bLive == false )
		return 0;

	SCharInfo& CurrentChar = g_MyD3D->MyPlayer[iPlayerIndex]->GetCurrentChar();
	if( Change_How_Exp[iPlayerIndex].count( CurrentChar.iCharType ) == 0 )
		return 0;
	return Change_How_Exp[iPlayerIndex][CurrentChar.iCharType];
}

int KGCGameBoard::GetChangeGp( int iPlayerIndex )
{
	return Change_How_Score[iPlayerIndex];
}

void KGCGameBoard::OnEnterPlayer( int iPlayerIndex )
{
    if ( 0 > iPlayerIndex || iPlayerIndex > MAX_PLAYER_NUM )
        return;
    

    Player_Work& History = m_pStartingMember[iPlayerIndex];
    //History.clear();

    // 처음 골랐던 케릭터를 찾음..
    History.IsLive		= true;
    History.m_iChar		= g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.GetCurrentChar().iCharType;    
    History.dwUID       = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.dwUID;
    History.m_mapStartExp   = g_MyD3D->MyPlayer[iPlayerIndex]->GetCharacterExpMap();
    History.Start_Score = (int)g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.GetCurrentChar().GetCurrentGP();
    History.How_Kill = 0;
    History.How_Die = 0;

    
        
    int   iMyLevel = Exp_2_Level( GetCharacterExp( 
        m_pStartingMember[iPlayerIndex].m_mapStartExp, g_MyD3D->MyPlayer[iPlayerIndex]->GetCurrentChar().iCharType ) ); /// 내 레벨 기록

    if( GC_GMC_GUILD_BATTLE == SiKGCRoomManager()->GetGameModeCategory()  )
    {
        if(iMyLevel <= 10)
            m_pStartingMember[iPlayerIndex].fGuildPointPercent = 2.2f;		//22포인트를 얻음
        else if( iMyLevel <= 20)
            m_pStartingMember[iPlayerIndex].fGuildPointPercent = 2.0f;
        else if( iMyLevel <= 29)
            m_pStartingMember[iPlayerIndex].fGuildPointPercent = 1.7f;
        else if( iMyLevel <= 37)
            m_pStartingMember[iPlayerIndex].fGuildPointPercent = 1.5f;
        else if( iMyLevel <= 44)
            m_pStartingMember[iPlayerIndex].fGuildPointPercent = 1.2f;
        else 
            m_pStartingMember[iPlayerIndex].fGuildPointPercent = 1.0f;

        if( g_MyD3D->CheckTeamCombination(g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.IsBlueTeam()) )
        {
            m_pStartingMember[iPlayerIndex].fGuildPointPercent*=2.0f;
        }
    }
    else
    {
        m_pStartingMember[iPlayerIndex].fGuildPointPercent=1.0f;
    }

}


void KGCGameBoard::OnLeavePlayer( int iPlayerIndex )
{
    if ( 0 > iPlayerIndex || iPlayerIndex > MAX_PLAYER_NUM )
        return;

    m_pStartingMember[iPlayerIndex].clear();
    //다른거 다 초기화 해도 이 자리에 플레이어가 있었다는것은 남겨야 한다.
    m_pStartingMember[iPlayerIndex].IsLive = true;

}

int KGCGameBoard::GetTeamScore( unsigned int iTeam_ )
{    
    if ( iTeam_ >= TEAM_NUM )
    {
        return 0;
    }
    return m_aiTeamScore[iTeam_];
}

void KGCGameBoard::SetTeamScore( unsigned int iTeam_ ,int iScore_ )
{
    if ( iTeam_ >= TEAM_NUM )
    {
        return;
    }
    m_aiTeamScore[iTeam_] = iScore_;

}

GCEXPTYPE KGCGameBoard::GetChangedSPExp( int iPlayerIndex )
{
	if( Change_How_SPExp.count( iPlayerIndex ) == 0 
	|| g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.bLive == false )
		return 0;

	SCharInfo& CurrentChar = g_MyD3D->MyPlayer[iPlayerIndex]->GetCurrentChar();
	if( Change_How_SPExp[iPlayerIndex].count( CurrentChar.iCharType ) == 0 )
		return 0;
	return Change_How_SPExp[iPlayerIndex][CurrentChar.iCharType];
}

int KGCGameBoard::GetStartingMemberCount()
{
	int iNumPlayer             = 0;      // 총 인원

	// 게임에 참여했던 총 인원을 구한다.
	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		// 게임을 최초로 시작했을 때의 인원을 기준으로 한다.i
		if ( m_pStartingMember[i].IsLive )
		{
			++iNumPlayer;
		}
	}

	return iNumPlayer;
}

void KGCGameBoard::AddAttackCount()
{
    ++m_iAttackCount;
    SiKGCAchieveManager()->OccurAction( SAchieve::HIT_COUNT, g_MyD3D->GetMyPlayer()->GetCharType() );
}

void KGCGameBoard::BindLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCGameBoard>( L, "KGCGameBoardClass" );
    lua_tinker::class_def<KGCGameBoard>( L,  "GetPlayTimeInFrame", 		    &KGCGameBoard::GetPlayTimeInFrame  );
    lua_tinker::class_def<KGCGameBoard>( L,  "GetStartingMemberCount", 		&KGCGameBoard::GetStartingMemberCount  );

    lua_tinker::decl( L, "KGCGameBoard", this );
}

float KGCGameBoard::GetTotalDamage( int iPlayerIndex )
{
    return m_pStartingMember[iPlayerIndex].fTotalDamage;
}

float KGCGameBoard::GetComboDamage( int iPlayerIndex )
{
    return m_pStartingMember[iPlayerIndex].fComboDamage;
}

void KGCGameBoard::SetComboBreak(bool bBreak)
{
    m_bComboBreak = bBreak;
}