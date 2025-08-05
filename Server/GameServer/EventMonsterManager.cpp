#include "EventMonsterManager.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG
#include "EclipseTimeEvent.h"

ImplementSingleton( KEventMonsterManager );
ImplOstmOperatorW2A( KEventMonsterManager );
NiImplementRootRTTI( KEventMonsterManager );

ImplToStringW( KEventMonsterManager )
{
    KLocker lock1( m_csEventMonster );
    KLocker lock2( m_csModeEvent );
    KLocker lock3( m_csDungeonLevel );
    KLocker lock4( m_csMonsterLevel );

    return START_TOSTRINGW
        << TOSTRINGW( m_bActiveEvent )
        << TOSTRINGW( m_mapEventMonster.size() )
        << TOSTRINGW( m_mapModeEvent.size() )
        << TOSTRINGW( m_mapDungeonLevel.size() )
        << TOSTRINGW( m_mapMonsterLevel.size() );
}

KEventMonsterManager::KEventMonsterManager(void)
:m_kTickTimer(60*1000,false)
,m_bActiveEvent(false)
{
}

KEventMonsterManager::~KEventMonsterManager(void)
{
}

void KEventMonsterManager::Tick()
{
    if ( m_kTickTimer.CheckTime() ) {
        UpdateEventInfo();
        m_kTickTimer.Reset();
    }
}

bool KEventMonsterManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEventMonsterManager::_LoadScript( OUT std::string& strScript_ )
{
    _JIF( LoadEventMonster(strScript_), return false );
    _JIF( LoadDungeonLevel(strScript_), return false );
    _JIF( LoadMonsterLevel(strScript_), return false );

    UpdateEventInfo();
    UpdateEclipseEventInfo();
    return true;
}

bool KEventMonsterManager::LoadEventMonster( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    bool bActiveEvent = false;
    std::map<int,KEventMonInfo> mapEventMonster;
    mapEventMonster.clear();

    _JIF( kLuaMng.DoFile( "EventMonster.stg" ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "EventMonster" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ActiveEvent", bActiveEvent ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {
        KEventMonInfo kInfo;

        if ( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( "EventMonsterID", kInfo.m_nMonsterID ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "AutoLevel", kInfo.m_bAutoLevel ) == S_OK, return false );
        _JIF( LoadDropItem( kLuaMng, "DropItem", kInfo.m_vecReward ), return false );
        _JIF( LoadAppearRate( kLuaMng, "AppearChance", kInfo.m_mapAppearRate ), return false );
        _JIF( LoadTimeTable( kLuaMng, "BeginTime", kInfo.m_tmBegin ), return false );
        _JIF( LoadTimeTable( kLuaMng, "EndTime", kInfo.m_tmEnd ), return false );
        _JIF( kLuaMng.GetValue( "IsBoss", kInfo.m_bIsBoss ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "IsSpecial", kInfo.m_nSpecial ) == S_OK, return false );
        _JIF( LoadGameMode( kLuaMng, "GameMode", kInfo.m_setModeID ), return false );
        _JIF( LoadModeStage( kLuaMng, "ModeStage", kInfo.m_mapModeStage ), return false );
        _JIF( LoadChildMonsterInfo( kLuaMng, "ChildMonster", kInfo.m_vecChildMonster ), return false );
        _JIF( LoadPartyDropItem( kLuaMng, "PartyDropItem", kInfo.m_vecPartyReward ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        //_JIF( mapEventMonster.insert( std::make_pair( kInfo.m_nMonsterID, kInfo ) ).second == true, return false );
        if ( !mapEventMonster.insert( std::make_pair( kInfo.m_nMonsterID, kInfo ) ).second  ) {
            // 맵에 삽입 실패면 몬스터 아이디가 중복이니 확인해주세요!
            START_LOG( cerr, L"중복된 몬스터ID가 있습니다. EventMonsterID: " << kInfo.m_nMonsterID ) << END_LOG;
            return false;
        }
    }

    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load EventMonster script.. IsActive : " << bActiveEvent << L", size : " << mapEventMonster.size() ) << END_LOG;

    KLocker lock( m_csEventMonster );
    m_bActiveEvent = bActiveEvent;
    m_mapEventMonster.swap( mapEventMonster );
    return true;
}

bool KEventMonsterManager::LoadDropItem( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::vector<std::pair<KDropItemInfo,float> >& vecItem_ )
{
    vecItem_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; i += 4 ) {
        KDropItemInfo kTemp;
        float fDropRate;
        if ( kLuaMng_.GetValue( i, kTemp.m_ItemID ) != S_OK ) break;
        if ( kLuaMng_.GetValue( i+1, kTemp.m_nDuration ) != S_OK ) break;
        if ( kLuaMng_.GetValue( i+2, kTemp.m_nPeriod ) != S_OK ) break;
        if ( kLuaMng_.GetValue( i+3, fDropRate ) != S_OK ) break;
        vecItem_.push_back( std::make_pair( kTemp, fDropRate ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KEventMonsterManager::LoadPartyDropItem( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::vector<std::pair<KDropItemInfo,float> >& vecItem_ )
{
    vecItem_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; i += 4 ) {
        KDropItemInfo kTemp;
        float fDropRate;
        if ( kLuaMng_.GetValue( i, kTemp.m_ItemID ) != S_OK ) break;
        if ( kLuaMng_.GetValue( i+1, kTemp.m_nDuration ) != S_OK ) break;
        if ( kLuaMng_.GetValue( i+2, kTemp.m_nPeriod ) != S_OK ) break;
        if ( kLuaMng_.GetValue( i+3, fDropRate ) != S_OK ) break;
        vecItem_.push_back( std::make_pair( kTemp, fDropRate ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KEventMonsterManager::LoadAppearRate( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<int, float >& mapData_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( UINT i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) { break; }

        int nLevel = 0;
        float fRate = 0.f;
        _JIF( kLuaMng_.GetValue( "Level", nLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Prob", fRate ) == S_OK, return false );
                
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapData_.insert( std::make_pair( nLevel, fRate ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    
    return true;
}

bool KEventMonsterManager::LoadTimeTable( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KEventMonsterManager::LoadGameMode( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::set<int>& setMode_ )
{
    setMode_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nModeID = 0;
        if ( kLuaMng_.GetValue( i, nModeID ) != S_OK ) break;
        LIF( setMode_.insert( nModeID ).second );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KEventMonsterManager::LoadModeStage( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<int,std::vector<int> >& mapList_ )
{
    mapList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nModeID = 0;
        std::vector<int> vecStageID;
        vecStageID.clear();

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "ModeID", nModeID ) == S_OK, return false );

        _JIF( kLuaMng_.BeginTable( "StageID" ) == S_OK, return false );
        for ( int j = 1 ; ; ++j ) {
            int nStageID = 0;
            if ( kLuaMng_.GetValue( j, nStageID ) != S_OK ) break;
            vecStageID.push_back( nStageID );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        LIF( mapList_.insert( std::make_pair( nModeID, vecStageID ) ).second );
        START_LOG( clog, L" ModeID : " << nModeID << L", stage size :" << vecStageID.size() ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"LoadModeStage.. size : " << mapList_.size() ) << END_LOG;
    return true;
}

bool KEventMonsterManager::LoadDungeonLevel( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map<int,std::map<int,KDungeonLevel> > mapDungeonLevel;

    _JIF( kLuaMng.DoFile( "enum.stg" ) == S_OK, return false );
    _JIF( kLuaMng.DoFile( "etc.stg" ) == S_OK, return false );
    _JIF( kLuaMng.DoFile( "QuestSign.stg" ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "quest" ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {
        char szQuestSign[128];
        sprintf(szQuestSign, "QuestSign%d", i-1);

        std::map<int, KDungeonLevel> mapDungeonLv;
        int nModeID = 0;

        if ( kLuaMng.BeginTable( szQuestSign ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( "ID", nModeID ) == S_OK, return false );

        std::vector<std::string>::iterator vit;
        
        _JIF( LoadLevelInfo( kLuaMng, "LevelInfo", mapDungeonLv ), return false );

       _JIF( kLuaMng.EndTable() == S_OK, return false ); // end table 'szQuestSign'

       if ( !mapDungeonLevel.insert( std::make_pair( nModeID, mapDungeonLv ) ).second ) {
           START_LOG( cerr, L"QuestSign로드 시 동일한 ID있음 : " << nModeID ) << END_LOG;
    }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // end of table "quest"

    START_LOG( cerr, L"Load DungeonLevel script.. size : " << mapDungeonLevel.size() ) << END_LOG;

    KLocker lock( m_csDungeonLevel );
    m_mapDungeonLevel.swap( mapDungeonLevel );
    return true;
}

bool KEventMonsterManager::LoadLevelInfo( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<int, KDungeonLevel>& mapDungeonLevel_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int j = 1 ; ; ++j ) 
    {
        if( kLuaMng_.BeginTable( j ) != S_OK ) { break; }
        KDungeonLevel kInfo;
        int nLevel = -1;
        if ( kLuaMng_.GetValue( "Level", nLevel ) != S_OK ) {
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            continue;
        }
        if ( kLuaMng_.GetValue( "LOW_LEVEL", kInfo.m_nLowLv ) != S_OK ) {
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            continue;
        }
        if ( kLuaMng_.GetValue( "HIGH_LEVEL", kInfo.m_nHighLv ) != S_OK ) {
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
            continue;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // end table for kLuaMng_.BeginTable( j )

        mapDungeonLevel_.insert( std::make_pair( nLevel, kInfo ) );  
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KEventMonsterManager::LoadMonsterLevel( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::map<int,int> mapMonsterLevel;

    _JIF( kLuaMng.DoFile( "MonsterTemplate.stg" ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "MonsterList" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nMonsterID = 0;
        int nLevel = 0;

        if ( kLuaMng.BeginTable( i ) != S_OK ) break;
        if ( kLuaMng.GetValue( "ID", nMonsterID ) != S_OK ) continue;
        if ( kLuaMng.GetValue( "iBaseLevel", nLevel ) != S_OK ) continue;
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        LIF( mapMonsterLevel.insert( std::make_pair( nMonsterID, nLevel ) ).second );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load MonsterLevel script.. size : " << mapMonsterLevel.size() ) << END_LOG;

    KLocker lock( m_csMonsterLevel );
    m_mapMonsterLevel.swap( mapMonsterLevel );
    return true;
}

bool KEventMonsterManager::LoadChildMonsterInfo( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::vector<KEventChildMonInfo>& vecChildMonsterInfo_ )
{
    vecChildMonsterInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( UINT i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) { break; }

        KEventChildMonInfo childMonster;

        _JIF( kLuaMng_.GetValue( "ChildMonsterID", childMonster.m_nChildMonsterID ) == S_OK, return false );
        _JIF( LoadDropItem( kLuaMng_, "ChildMonsterDropItem", childMonster.m_vecChildReward ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecChildMonsterInfo_.push_back( childMonster );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load ChildMonster table.. size : " << vecChildMonsterInfo_.size() ) << END_LOG;

    return true;
}


void KEventMonsterManager::UpdateEventInfo()
{
    std::map<int,std::set<int> > mapModeEvent;
    std::map<int,KEventMonInfo> mapEventMonster;
    GetEventMonster( mapEventMonster );

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    std::map<int,KEventMonInfo>::iterator mit;
    for ( mit = mapEventMonster.begin() ; mit != mapEventMonster.end() ; ++mit ) {
        // 이벤트 기간이 아니다.
        if ( mit->second.m_tmBegin > tmCurrent || mit->second.m_tmEnd < tmCurrent ) {
            continue;
        }

        // 이벤트 기간이면,모드ID별로 몬스터ID 설정
        std::set<int>::iterator sit;
        for ( sit = mit->second.m_setModeID.begin() ; sit != mit->second.m_setModeID.end() ; ++sit ) {
            mapModeEvent[*sit].insert( mit->second.m_nMonsterID );
        }
    }

    START_LOG( clog, L"이벤트 몬스터 정보 갱신.. size : " << mapModeEvent.size() << L" / " << mapEventMonster.size() ) << END_LOG;

    KLocker lock( m_csModeEvent );
    m_mapModeEvent.swap( mapModeEvent );
}

bool KEventMonsterManager::CheckEvent( IN const int nModeID_, OUT std::set<int>& setMonsterID_ )
{
    setMonsterID_.clear();

    KLocker lock( m_csModeEvent );

    std::map<int,std::set<int> >::iterator mit;
    mit = m_mapModeEvent.find( nModeID_ );
    if ( mit == m_mapModeEvent.end() ) {
        return false;
    }

    setMonsterID_ = mit->second;
    return true;
}

bool KEventMonsterManager::GetEventMonInfo( IN const int nMonsterID_, OUT KEventMonInfo& kInfo_ )
{
    KLocker lock( m_csEventMonster );

    std::map<int,KEventMonInfo>::iterator mit;
    mit = m_mapEventMonster.find( nMonsterID_ );
    if ( mit == m_mapEventMonster.end() ) {
        START_LOG( cwarn, L"해당 몬스터ID는 이벤트 목록에 없음. 몬스터ID : " << nMonsterID_ ) << END_LOG;
        return false;
    }

    kInfo_ = mit->second;
    return true;
}

bool KEventMonsterManager::GetDungeonLevel( IN const int nModeID_, IN const int nDifficulty_, OUT PAIR_INT& prLevel_ )
{
    KLocker lock( m_csDungeonLevel );

    prLevel_.first = 0;
    prLevel_.second = 0;

    std::map<int,std::map<int,KDungeonLevel> >::iterator mit;
    mit = m_mapDungeonLevel.find( nModeID_ );
    if ( mit == m_mapDungeonLevel.end() ) {
        START_LOG( cwarn, L"해당 던전ID의 레벨정보가 없음. ModeID : " << nModeID_ ) << END_LOG;
        return false;
    }

    if ( nDifficulty_ > MAX_DIFFICULTY ) {
        START_LOG( cwarn, L"난이도 최대수치 초과. Difficulty : " << nDifficulty_ ) << END_LOG;
        return false;
    }

    prLevel_.first = mit->second[nDifficulty_].m_nLowLv;
    prLevel_.second = mit->second[nDifficulty_].m_nHighLv;
    return true;
}

int KEventMonsterManager::GetMonsterLevel( IN const int nMonsterID_ )
{
    KLocker lock( m_csMonsterLevel );

    std::map<int,int>::iterator mit;
    mit = m_mapMonsterLevel.find( nMonsterID_ );
    if ( mit == m_mapMonsterLevel.end() ) {
        START_LOG( cwarn, L"해당 몬스터의 레벨정보가 없음. MonsterID : " << nMonsterID_ ) << END_LOG;
        return 1;
    }

    return mit->second;
}

void KEventMonsterManager::DumpEventMonster()
{
    //KLocker lock( m_csEventMonster );

    //std::cout << L"--- Dump EventMonster Info (size:" << m_mapEventMonster.size() << L")---" << dbg::endl;
    //std::cout << L"Is Active Event? " << (m_bActiveEvent?L"True":L"False") << dbg::endl;

    //std::map<int,KEventMonInfo>::iterator mit;
    //for ( mit = m_mapEventMonster.begin() ; mit != m_mapEventMonster.end() ; ++mit ) {
    //    std::cout << L"MonsterID : " << mit->second.m_nMonsterID
    //        << L",AutoLevel : " << mit->second.m_bAutoLevel
    //        << L",DropItem (";

    //    std::vector<std::pair<KDropItemInfo,float> >::iterator vit;
    //    for ( vit = mit->second.m_vecReward.begin() ; vit != mit->second.m_vecReward.end() ; ++vit ) {
    //        std::cout << L"ItemID:" << vit->first.m_ItemID
    //            << L",Count:" << vit->first.m_nDuration
    //            << L",Period:" << vit->first.m_nPeriod
    //            << L",DropRate:" << vit->second << L",";
    //    }

    //    std:: cout << L"),AppearRate (";
    //    for ( UINT i = 0 ; i < mit->second.m_pAppearRate.size() ; ++i ) {
    //        std::cout << mit->second.m_pAppearRate[i] << L",";
    //    }
    //    std::cout << L"),";

    //    std::cout << L",BeginDate : " << mit->second.m_tmBegin
    //        << L",EndDate : " << mit->second.m_tmEnd
    //        << L",IsBoss : "<< mit->second.m_bIsBoss
    //        << L",Special : "<< mit->second.m_nSpecial
    //        << L",ModeID(";

    //    std::set<int>::iterator sit;
    //    for ( sit = mit->second.m_setModeID.begin() ; sit != mit->second.m_setModeID.end() ; ++sit ) {
    //        std::cout << *sit << L",";
    //    }
    //    std::cout << L")" << dbg::endl;
    //}

    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KEventMonsterManager::DumpModeEvent()
{
    //KLocker lock( m_csModeEvent );

    //std::cout << L"--- Dump ModeEvent Info (size:" << m_mapModeEvent.size() << L")---" << dbg::endl;
    //std::cout << L"ModeID" << std::tab << L"MonsterID()" << dbg::endl;

    //std::map<int,std::set<int> >::iterator mit;
    //for ( mit = m_mapModeEvent.begin() ; mit != m_mapModeEvent.end() ; ++mit ) {
    //    std::cout << mit->first << std::tab
    //              << mit->second.size() << L"(";

    //    std::set<int>::iterator sit;
    //    for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {
    //        std::cout << *sit << L",";
    //    }
    //    std::cout << L")" << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KEventMonsterManager::DumpDungeonLevel()
{
    //KLocker lock( m_csDungeonLevel );

    //std::cout << L"--- Dump DungeonLevel Info (size:" << m_mapDungeonLevel.size() << L")---" << dbg::endl;
    //std::cout << L"ModeID" << std::tab << L"ModeLv()" << dbg::endl;

    //std::map<int,boost::array<KDungeonLevel,MAX_DIFFICULTY>>::iterator mit;
    //for ( mit = m_mapDungeonLevel.begin() ; mit != m_mapDungeonLevel.end() ; ++mit ) {
    //    std::cout << mit->first << std::tab
    //              << L"(";

    //    for ( UINT i = 0 ; i < mit->second.size() ; ++i ) {
    //        std::cout << mit->second[i].m_nLowLv << L"/"
    //                  << mit->second[i].m_nHighLv << L", ";
    //    }
    //    std::cout << L")" << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KEventMonsterManager::DumpMonsterLevel()
{
    //KLocker lock( m_csMonsterLevel );

    //std::cout << L"--- Dump MonsterLevel Info (size:" << m_mapMonsterLevel.size() << L")---" << dbg::endl;
    //std::cout << L"MonsterID" << std::tab << L"MonsterLv" << dbg::endl;

    //std::map<int,int>::iterator mit;
    //for ( mit = m_mapMonsterLevel.begin() ; mit != m_mapMonsterLevel.end() ; ++mit ) {
    //    std::cout << mit->first << std::tab
    //              << mit->second << dbg::endl;
    //}
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

void KEventMonsterManager::UpdateEclipseEventInfo()
{
    std::map<int,std::set<int> > mapEclipseModeEvent;
    std::map<int,KEventMonInfo> mapEventMonster;
    GetEventMonster( mapEventMonster );

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    std::map<int,KEventMonInfo>::iterator mit;
    for ( mit = mapEventMonster.begin() ; mit != mapEventMonster.end() ; ++mit ) {
        // 모드ID별로 몬스터ID 설정
        std::set<int>::iterator sit;
        for ( sit = mit->second.m_setModeID.begin() ; sit != mit->second.m_setModeID.end() ; ++sit ) {
            mapEclipseModeEvent[*sit].insert( mit->second.m_nMonsterID );
        }
    }

    START_LOG( clog, L"Eclipse 이벤트 몬스터 정보 갱신.. size : " << mapEclipseModeEvent.size() << L" / " << mapEventMonster.size() ) << END_LOG;

    KLocker lock( m_csModeEvent );
    m_mapEclipseModeEvent.swap( mapEclipseModeEvent );
}

bool KEventMonsterManager::CheckEclipseEvent( IN const int nModeID_, OUT std::set<int>& setMonsterID_ )
{
    KLocker lock( m_csModeEvent );

    std::map<int,std::set<int> >::iterator mit;
    mit = m_mapEclipseModeEvent.find( nModeID_ );
    if ( mit == m_mapEclipseModeEvent.end() ) {
        return false;
    }

    std::set<int>::iterator sit;
    for ( sit = mit->second.begin(); sit != mit->second.end(); ++sit ) {
        if ( SiKEclipseTimeEvent()->CheckEclipseEventMonster( *sit ) ) {
            setMonsterID_.insert( *sit );
        }
    }
    return true;
}