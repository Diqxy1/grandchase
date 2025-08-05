#include "stdafx.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "KGCInfinityDungeonManager.h"

ImplementSingleton( KGCInfinityDungeonManager )

KGCInfinityDungeonManager::KGCInfinityDungeonManager()
{
    Init();
}

KGCInfinityDungeonManager::~KGCInfinityDungeonManager()
{
}


void KGCInfinityDungeonManager::Init()
{
}

void KGCInfinityDungeonManager::InitBeforeStartGame( void )
{
    m_dwRoundSquence = 0;
}

bool KGCInfinityDungeonManager::LoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr, "etc.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr , "InfinityDungeonInfo.lua" ) == false )
        return false;


    m_mapInfinityDungeonInfo.clear();
    m_nGuideGameMode = GC_GM_INVALID;

    
    LUA_BEGIN_TABLE( "GUIDE_INFINITY_DUNGEON", return false)
    {
        LUA_GET_VALUE( 1, m_nGuideGameMode, return false )
    }
    LUA_END_TABLE(return false)

    LUA_BEGIN_TABLE("InfinityDungeonInfo",return false)
    {
        for(int i = 1; ; i++)
        {
            LUA_BEGIN_TABLE(i,break)
            {
                KGCInfinityDungeon kInfinityDungeonInfo;

                LUA_GET_VALUE( "DUNGEON_ID", kInfinityDungeonInfo.m_nGameMode, return false );

                LUA_BEGIN_TABLE("STAGE_INFO",return false)
                {
                    int iLoop = 1;
                    std::string strStageMap;
                    while( SUCCEEDED( luaMgr.GetValue( iLoop++, strStageMap ) ) )
                    {
                        kInfinityDungeonInfo.m_vecStageInfo.push_back( strStageMap );
                    }
                }
                LUA_END_TABLE(return false)

                LUA_BEGIN_TABLE("RANK_SCORE_INFO",return false)
                {   
                    for(int j = 1; ; j++)
                    {
                        LUA_BEGIN_TABLE(j,break)
                        {
                            std::pair<int,int> pairRank;

                            LUA_GET_VALUE( 1, pairRank.first, return false );
                            LUA_GET_VALUE( 2, pairRank.second, return false );
                            
                            kInfinityDungeonInfo.m_vecRankScoreInfo.push_back( pairRank );
                        }
                        LUA_END_TABLE(return false)
                    }

                    if ( kInfinityDungeonInfo.m_vecRankScoreInfo.size() != EM_RANK_SIZE )
                    {
                        MessageBox( NULL, L"RANK_SCORE_INFO size error", L"InfinityDungeonManager.lua", MB_OK );
                        return false;
                    }                   

                }
                LUA_END_TABLE(return false)

                LUA_GET_VALUE( "ATK_TOTAL", kInfinityDungeonInfo.m_iTotalAtk, return false );

                LUA_BEGIN_TABLE("BOSS_REGEN_INFO",return false)
                {
                    LUA_GET_VALUE( "BOSS_REGEN_RANK", kInfinityDungeonInfo.m_nBossRegenRank, return false );
                    LUA_GET_VALUE( "REGEN_WAIT_TIME", kInfinityDungeonInfo.m_nBossRegenWaitTime, return false );
                    LUA_GET_VALUE( "MIN_BOSS_REGEN_TIME", kInfinityDungeonInfo.m_nMinBossRegenTime, return false );
                    LUA_GET_VALUE( "MAX_BOSS_REGEN_TIME", kInfinityDungeonInfo.m_nMaxBossRegenTime, return false );

                    LUA_BEGIN_TABLE("BOSS_MONSTER_GROUP",return false)
                    {
                        for(int j = 1; ; j++)
                        {
                            LUA_BEGIN_TABLE(j,break)
                            {
                                SBossRankGroup sBossRankGroup;
                                LUA_GET_VALUE( "RANK", sBossRankGroup.m_nRank, return false );
                                LUA_GET_VALUE( "RNADOM", sBossRankGroup.m_bRandom, return false );
                                LUA_GET_VALUE( "LEVEL", sBossRankGroup.m_nLevel, return false );

                                LUA_BEGIN_TABLE("BOSS_LIST",return false)
                                {
                                    int iLoop = 1, iTemp;
                                    while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
                                    {
                                        sBossRankGroup.m_vecBossList.push_back( iTemp );
                                    }  
                                }
                                LUA_END_TABLE(return false)

                                kInfinityDungeonInfo.m_mapBossRankGroup.insert( std::pair<int,SBossRankGroup>( sBossRankGroup.m_nRank, sBossRankGroup ) );

                            }
                            LUA_END_TABLE(return false)
                        }

                        if ( kInfinityDungeonInfo.m_mapBossRankGroup.size() != EM_RANK_SIZE )
                        {
                            MessageBox( NULL, L"BOSS_MONSTER_GROUP size error", L"InfinityDungeonManager.lua", MB_OK );
                            return false;
                        } 
                    }
                    LUA_END_TABLE(return false)
                }
                LUA_END_TABLE(return false)

                LUA_BEGIN_TABLE("MONSTER_INFO",return false)
                {
                    LUA_GET_VALUE( "MONSTER_REGEN_TIME", kInfinityDungeonInfo.m_nMonsterRegenTime, return false );

                    LUA_BEGIN_TABLE("MAX_MONSTER_NUM",return false)
                    {
                        int iLoop = 1, iTemp;
                        while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
                        {
                            kInfinityDungeonInfo.m_vecMaxMonsterNum.push_back( iTemp );
                        }

                        if ( kInfinityDungeonInfo.m_vecMaxMonsterNum.size() != MAX_PLAYER_NUM )
                        {
                            MessageBox( NULL, L"MAX_MONSTER_NUM size error", L"InfinityDungeonManager.lua", MB_OK );
                            return false;
                        } 
                    }
                    LUA_END_TABLE(return false)

                    LUA_GET_VALUE( "START_MONSTER_LEVEL", kInfinityDungeonInfo.m_nStartMonsterLevel, return false );
                    LUA_BEGIN_TABLE("START_MONSTER_GROUP",return false)
                    {
                        int iLoop = 1, iTemp;
                        while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
                        {
                            kInfinityDungeonInfo.m_vecStartMonsterGroup.push_back( iTemp );
                        }  
                    }
                    LUA_END_TABLE(return false)

                    LUA_BEGIN_TABLE("RANK_MONSTER_INFO",return false)
                    {
                        for(int j = 1; ; j++)
                        {
                            LUA_BEGIN_TABLE(j,break)
                            {
                                SMonsterRankGroup sMonsterRankGroup;
                                LUA_GET_VALUE( "RANK", sMonsterRankGroup.m_nRank, return false );
                                LUA_GET_VALUE( "LEVEL", sMonsterRankGroup.m_nLevel, return false );

                                LUA_BEGIN_TABLE("REGEN_GROUP_RATE",return false)
                                {   
                                    for(int k = 1; ; k++)
                                    {
                                        LUA_BEGIN_TABLE(k,break)
                                        {
                                            std::pair<int,int> pairRank;

                                            LUA_GET_VALUE( 1, pairRank.first, return false );
                                            LUA_GET_VALUE( 2, pairRank.second, return false );

                                            sMonsterRankGroup.m_vecRegenRate.push_back( pairRank );
                                        }
                                        LUA_END_TABLE(return false)
                                    }
                                }
                                LUA_END_TABLE(return false)

                                LUA_BEGIN_TABLE("REGEN_MONSTER_GROUP",return false)
                                {
                                    int iLoop = 1, iTemp;
                                    while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
                                    {
                                        sMonsterRankGroup.m_vecMonsterGroup.push_back( iTemp );
                                    }  
                                }
                                LUA_END_TABLE(return false)
                                
                                kInfinityDungeonInfo.m_mapMonsterRankGroup.insert( std::pair<int, SMonsterRankGroup>( sMonsterRankGroup.m_nRank, sMonsterRankGroup ) );
                                
                            }
                            LUA_END_TABLE(return false)              
                            
                        }


                        if ( kInfinityDungeonInfo.m_mapMonsterRankGroup.size() != EM_RANK_SIZE )
                        {
                            MessageBox( NULL, L"RANK_MONSTER_INFO size error", L"InfinityDungeonManager.lua", MB_OK );
                            return false;
                        } 
                    }
                    LUA_END_TABLE(return false)

                }
                LUA_END_TABLE(return false)

                m_mapInfinityDungeonInfo.insert( std::pair<int, KGCInfinityDungeon>( kInfinityDungeonInfo.m_nGameMode, kInfinityDungeonInfo ) );

            }
            LUA_END_TABLE(return false)
        }
    }
    LUA_END_TABLE(return false)

    return true;
}

KGCInfinityDungeon* KGCInfinityDungeonManager::GetInfinityDungeon( int nGameMod )
{
    std::map<int,KGCInfinityDungeon>::iterator iter = m_mapInfinityDungeonInfo.find( nGameMod );

    if ( iter != m_mapInfinityDungeonInfo.end() )
        return &iter->second;

    return NULL;
}

bool KGCInfinityDungeonManager::GetRankScore( int nGameMode, int nRank, int& nScore, int& nTime )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return false;

    return pDungeon->GetRankScore( nRank, nScore, nTime );
}

int KGCInfinityDungeonManager::GetRegenBossRank( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return EM_RANK_SIZE;

    return pDungeon->GetRegenBossRank();
}

int KGCInfinityDungeonManager::GetRegenBossLevel( int nGameMode, int nRank )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return 1;

    return pDungeon->GetRegenBossLevel( nRank );
}


int KGCInfinityDungeonManager::GetRegenBossWaitTime( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return -1;

    return pDungeon->GetRegenBossWaitTime();
}

int KGCInfinityDungeonManager::GetMinBossRegenTime( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return -1;

    return pDungeon->GetMinBossRegenTime();
}

int KGCInfinityDungeonManager::GetMaxBossRegenTime( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return -1;

    return pDungeon->GetMaxBossRegenTime();
}

bool KGCInfinityDungeonManager::GetRegenBossList( int nGameMode, bool bRankGen, int nRank, std::vector<int>& vecBossMon )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return false;

    return pDungeon->GetRegenBossList( bRankGen, nRank, vecBossMon );
}


int KGCInfinityDungeonManager::GetRegenMonsterTime( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return -1;

    return pDungeon->GetRegenMonsterTime();
}

int KGCInfinityDungeonManager::GetMaxMonsterNum( int nGameMode, int nPlayerNum )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return -1;

    return pDungeon->GetMaxMonsterNum( nPlayerNum );
}

// 최초 게임 시작시 생성될 몬스터 그룹을 요청한다.
bool KGCInfinityDungeonManager::GetStartMonsterList( int nGameMode, int nMonsterNum, std::vector<int>& vecMonsterList )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return false;

    return pDungeon->GetStartMonsterList( nMonsterNum, vecMonsterList );
}

int KGCInfinityDungeonManager::GetStartMonsterLevel( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return 1;

    return pDungeon->GetStartMonsterLevel();
}

// 특정 랭크의 소환될 몬스터를 가져온다.
bool KGCInfinityDungeonManager::GetRankMonsterList( int nGameMode, int nRank, int nMonsterNum, std::vector<int>& vecMonsterList )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return false;

    return pDungeon->GetRankMonsterList( nRank, nMonsterNum, vecMonsterList );
}


int KGCInfinityDungeonManager::GetRankMonsterLevel( int nGameMode, int nRank )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return 1;

    return pDungeon->GetRankMonsterLevel( nRank );
}


std::string KGCInfinityDungeonManager::GetRandomStageName( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return std::string();

    return pDungeon->GetStageName();
}

std::string KGCInfinityDungeonManager::GetStageName( int nGameMode, int nindex )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if( NULL == pDungeon ) { 
        return std::string();
    }

    return pDungeon->GetStageName( nindex );
}

int KGCInfinityDungeonManager::GetStageCount( int nGameMode )
{
    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if( NULL == pDungeon ) { 
        return 0;
    }    

    return pDungeon->GetStageNum();
}

bool KGCInfinityDungeonManager::SummonStartMonsterReq( const int nGameMode_, const int nNumPlayer_,  const int nRank_ )
{
    if( m_dwRoundSquence != 0 ) { 
        return false;
    }

    if( !g_kGlobalValue.m_kUserInfo.bHost ) { 
        return false;
    }

    // 마릿수 정하고 
    int nNumMaxMonster = GetMaxMonsterNum( nGameMode_, nNumPlayer_ );

    // 소환 몬스터 정보 얻고 
    std::vector<int> vecMonsterList;
    if( false == GetStartMonsterList( nGameMode_, nNumMaxMonster, vecMonsterList ) ) { 
        return false;
    }

    // 각 몬스터의 적절한 레벨을 세팅 
    std::vector< PAIR_INT > vecSummonList;
    std::vector< int >::const_iterator itMonsterList = vecMonsterList.begin();
    int iLevel = GetStartMonsterLevel( nGameMode_ );
    for( ; itMonsterList != vecMonsterList.end() ; ++itMonsterList ) { 
        vecSummonList.push_back( std::make_pair( *itMonsterList, iLevel ) );
    }

    // 서버로 요청!
    KP2P::GetInstance()->Send_InfinityDungeonMonsterSummonReq( nRank_, m_dwRoundSquence, vecSummonList, KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_NORMAL );
    return true;
}

bool KGCInfinityDungeonManager::SummonMonsterReq( const int nGameMode_, const int nNumPlayer_, const int nRank_, const bool bBoss_, const bool bGenRankBoss_ )
{
    if( !g_kGlobalValue.m_kUserInfo.bHost ) { 
        return false;
    }

    // 소환할 몬스터 정보 세팅 
    std::vector< int > vecMonsterList;
    if( bBoss_ ) { 
        if( false == GetRegenBossList( nGameMode_, bGenRankBoss_, nRank_, vecMonsterList ) ) { 
            return false;
        }
    }
    else { 
        // 마릿수 정하고 
        int nNumMaxMonster = GetMaxMonsterNum( nGameMode_, nNumPlayer_ );
        int nNumCurMonster = g_kMonsterManager.GetLiveInfinityMonsterNormalNum();
        if( false == GetRankMonsterList( nGameMode_, nRank_, ( nNumMaxMonster - nNumCurMonster ), vecMonsterList ) ) { 
            return false;
        }
    }

    // 각 몬스터의 레벨을 세팅 
    int iLevel = bBoss_ ? GetRegenBossLevel( nGameMode_, nRank_ ) : GetRankMonsterLevel( nGameMode_, nRank_ );
    std::vector< PAIR_INT > vecSummonList;
    std::vector< int >::const_iterator itMonsterList = vecMonsterList.begin();
    for( ; itMonsterList != vecMonsterList.end() ; ++itMonsterList ) { 
        vecSummonList.push_back( std::make_pair( *itMonsterList, iLevel ) );
    }

    // 소환 안되면 false
    if( vecSummonList.empty() ) { 
        return false;
    }

    // 서버로 요청 
    if( bBoss_ ) { 
        KP2P::GetInstance()->Send_InfinityDungeonMonsterSummonReq( nRank_, m_dwRoundSquence, vecSummonList, ( bGenRankBoss_ == true ? KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_BOSS_RANK : KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_BOSS_INTERVAL ) );
    }
    else {
        KP2P::GetInstance()->Send_InfinityDungeonMonsterSummonReq( nRank_, m_dwRoundSquence, vecSummonList, KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_NORMAL );
    }
    return true;
}

bool KGCInfinityDungeonManager::GenMonster( const std::vector< KIDMonsterInfo > vecMonsterList_ )
{
    if( !g_kGlobalValue.m_kUserInfo.bHost ) { 
        return false;
    }

    if( vecMonsterList_.empty() ) { 
        return false;
    }

    // 소환!!
    std::vector< KIDMonsterInfo >::const_iterator itMonsterList = vecMonsterList_.begin();
    for( ; itMonsterList != vecMonsterList_.end() ; ++itMonsterList ) { 
        KInfinitySummonInfo kInfinitySummonInfo;
        kInfinitySummonInfo.iMonType = itMonsterList->m_nMonID;
        kInfinitySummonInfo.iTGid = itMonsterList->m_nMonUID;
        kInfinitySummonInfo.iLevel = itMonsterList->m_nMonLv;
        kInfinitySummonInfo.SetState( "WAIT" );
        kInfinitySummonInfo.fX = SiKGCFantasticMap()->GetRandomPosX();
        kInfinitySummonInfo.fY = SiKGCFantasticMap()->GetRandomPosY();
        kInfinitySummonInfo.bCheckSafePostion = true;
        kInfinitySummonInfo.bBoss = ( itMonsterList->m_nMonType == KIDMonsterInfo::EMT_BOSS_INTERVAL ||  itMonsterList->m_nMonType == KIDMonsterInfo::EMT_BOSS_RANK );

        g_kMonsterManager.SummonInfinityMonster( &kInfinitySummonInfo );
    }

    
    ++m_dwRoundSquence;
    return true;
}


void KGCInfinityDungeonManager::CheckShowGuide()
{
    bool bPlay = SiKGCWorldMapManager()->IsPlayableDungeon( (EGCGameMode)m_nGuideGameMode, g_kGlobalValue.m_kUserInfo.GetCurrentCharType() );
    
    m_bShowGuide = !bPlay;

}

void KGCInfinityDungeonManager::ShowGuide()
{
    m_bShowGuide = false;
    
    // 가이드 출력한다.
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_INFINITY_GUIDE_DLG );
}

bool KGCInfinityDungeonManager::IsShowGuide()
{
    if ( !m_bShowGuide )
        return false;

    bool bPlay = SiKGCWorldMapManager()->IsPlayableDungeon( (EGCGameMode)m_nGuideGameMode, g_kGlobalValue.m_kUserInfo.GetCurrentCharType() );

    return bPlay;
}



void KGCInfinityDungeonManager::PreGeneraterStageMonster( int nGameMode )
{
    std::set<int> setMonsterList;

    KGCInfinityDungeon* pDungeon = GetInfinityDungeon( nGameMode );
    if ( !pDungeon ) 
        return;

    pDungeon->GetAllMonsterList( setMonsterList );

    std::set<int>::iterator iter = setMonsterList.begin();
    for ( ; iter != setMonsterList.end(); ++iter )
    {
        g_MyD3D->CreateMeshAnimationForPrevLoad( *iter );
    }
}

int KGCInfinityDungeonManager::GetExpRate( const int nGameMode_, const int iMonsterLevel_ )
{
    if( !SiKGCWorldMapManager()->IsInfinityDungeon( static_cast<EGCGameMode>(nGameMode_) ) ) { 
        return EXP_RANK_NONE;
    }

    if( g_MyD3D->GetMyPlayer()->GetCurrentCharLevel() == MAX_LEVEL ) { 
        return EXP_RANK_NONE;
    }

    // 현재 소환된 랭크와 몬스터 레벨을 비교하여 사냥 효율 보여준다. 
    int iDiff = iMonsterLevel_ - g_MyD3D->GetMyPlayer()->GetCurrentCharLevel();

    if( nGameMode_ == GC_GM_QUEST77 || nGameMode_ == GC_GM_QUEST78 ) {
        if( -2 > iDiff )
            return EXP_RANK_VBAD;
        else if( -2 <= iDiff && iDiff <= -1 ) 
            return EXP_RANK_BAD;
        else if( 0 <= iDiff && iDiff <= 1 ) 
            return EXP_RANK_NORMAL;
        else if( 2 <= iDiff && iDiff <= 3 ) 
            return EXP_RANK_GOOD;
        else
            return EXP_RANK_VGOOD;
    }
    else if( nGameMode_ == GC_GM_QUEST79 || nGameMode_ == GC_GM_QUEST80 ) { 
        if( -1 > iDiff )
            return EXP_RANK_VBAD;
        else if( -1 == iDiff )
            return EXP_RANK_BAD;
        else if( 0 == iDiff )
            return EXP_RANK_NORMAL;
        else if( 1 == iDiff )
            return EXP_RANK_GOOD;
        else
            return EXP_RANK_VGOOD;
    }

    return EXP_RANK_NONE;
}
