#include "CenterSimLayer.h"
#include "CnSurvey.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h" // centerpacket
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KCnSurvey );
ImplOstmOperatorW2A( KCnSurvey );
NiImplementRootRTTI( KCnSurvey );


KCnSurvey::KCnSurvey(void)
:m_dwTickGap( 60000 )
,m_nVersion( 0 )
,m_tmBegin( 1 )
,m_tmEnd( 1 )
,m_bChanged( false )
,m_tmReturnTime( 1 )
{
    m_dwLastTick = ::GetTickCount();
    m_mapSurveyAll.clear();
    m_mapCompressedSurveyInfoPack.clear();
    m_mapSurveyTypeInfo.clear();
    m_mapSurveyRewardInfo.clear();
}

KCnSurvey::~KCnSurvey(void)
{

}

ImplToStringW( KCnSurvey )
{
    KLocker lock( m_csSurvey );
    return START_TOSTRINGW
        << TOSTRINGWb( IsRun() )
        << TOSTRINGW( m_nVersion )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_bChanged )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_mapSurveyAll.size() )
        << TOSTRINGW( m_mapCompressedSurveyInfoPack.size() )
        << TOSTRINGW( m_mapSurveyTypeInfo.size() )
        << TOSTRINGW( m_mapSurveyRewardInfo.size() );
}

bool KCnSurvey::LoadScript()
{
    KLuaManager kLuaMng;
    int nVersion;
    time_t tmBegin;
    time_t tmEnd;
    time_t tmReturnTime;
    DWORD dwTickGap;
    std::map< int, std::map< std::pair<int,int>, std::vector<int> > > mapSurveyTypeInfo; // 각 타입별 설문지 정보.
    std::map< int, std::vector< KDropItemInfo > > mapSurveyRewardInfo; // 각 타입별 보상정보.
    std::vector< int > vecLanguageInfo;
    mapSurveyTypeInfo.clear();

    _JIF( kLuaMng.DoFile( "InitCnSurvey.lua" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string( "BeginTime" ), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string( "EndTime" ), tmEnd ), return false );
    _JIF( LoadTime( kLuaMng, std::string( "ReturnTime" ), tmReturnTime ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "Version", nVersion ) == S_OK, return false );

    _JIF( LoadLanguageInfo( kLuaMng, std::string( "LanguageInfo" ), vecLanguageInfo ), return false );

    // 설문 Type정보 Load.
    _JIF( LoadGameConnectTypeInfo( kLuaMng, std::string( "GameConnectInfo" ), mapSurveyTypeInfo ), return false );
    _JIF( LoadDungeonClearTypeInfo( kLuaMng, std::string( "DungeonClearInfo" ), mapSurveyTypeInfo ), return false );
    _JIF( LoadLevelUpTypeInfo( kLuaMng, std::string( "LevelUpInfo" ), mapSurveyTypeInfo ), return false );

    // 보상정보 Load.
    _JIF( LoadRewardInfo( kLuaMng, std::string("SurveyRewardInfo"), mapSurveyRewardInfo ), return false );

    START_LOG( cerr, L"센터 설문 스크립트 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csSurvey );
        m_nVersion = nVersion;
        m_tmBegin = tmBegin;
        m_tmReturnTime = tmReturnTime;
        m_tmEnd = tmEnd;
        m_dwTickGap = dwTickGap;
        m_vecLanguageInfo.swap( vecLanguageInfo );
        m_mapSurveyTypeInfo.swap( mapSurveyTypeInfo );
        m_mapSurveyRewardInfo.swap( mapSurveyRewardInfo );
        m_bChanged = true;
    }

    return true;
}

bool KCnSurvey::LoadLanguageInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT std::vector< int >& vecLanguageInfo_ )
{
    vecLanguageInfo_.clear();
    int nLangID;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.GetValue( i, nLangID ) != S_OK ) break;
        vecLanguageInfo_.push_back( nLangID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadGameConnectTypeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::map< std::pair<int,int>, std::vector<int> > >& mapSurveyTypeInfo_ )
{
    int nType = 0;
    std::map< std::pair<int,int>, std::vector<int> > mapSurveyInfo;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Type", nType ) == S_OK, return false );
    _JIF( LoadGameConnectSurveyInfo( kLuaMng_, std::string( "TypeInfo" ), mapSurveyInfo ), return false );

    if ( false == mapSurveyTypeInfo_.insert( std::make_pair( nType, mapSurveyInfo ) ).second ) {
        START_LOG( cerr, L" 중복된 게임서버 접속 Type 있음. Type : " << nType ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadGameConnectSurveyInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< std::pair<int,int>, std::vector<int> >& mapSurveyTypeInfo_ )
{
    mapSurveyTypeInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::pair<int,int> prData;
        std::vector<int> vecSurveyIDList;

        _JIF( kLuaMng_.GetValue( "ChannelType", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "UserType", prData.second ) == S_OK, return false );
        _JIF( LoadSurveyIDInfo( kLuaMng_, std::string("SurveyIDList"), vecSurveyIDList ), return false );

        if ( false == mapSurveyTypeInfo_.insert( std::make_pair( prData, vecSurveyIDList ) ).second ) {
            START_LOG( cerr, L" 중복된 UserType정보가 있음. ChannelType : " << prData.first <<  L", UserType : " << prData.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadDungeonClearTypeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::map< std::pair<int,int>, std::vector<int> > >& mapSurveyTypeInfo_ )
{
    int nType = 0;
    std::map< std::pair<int,int>, std::vector<int> > mapSurveyInfo;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Type", nType ) == S_OK, return false );
    _JIF( LoadDungeonClearSurveyInfo( kLuaMng_, std::string( "DungeonInfo" ), mapSurveyInfo ), return false );

    if ( false == mapSurveyTypeInfo_.insert( std::make_pair( nType, mapSurveyInfo ) ).second ) {
        START_LOG( cerr, L" 중복된 던전 클리어Type정보가 있음. Type : " << nType ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadDungeonClearSurveyInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< std::pair<int,int>, std::vector<int> >& mapSurveyTypeInfo_ )
{
    mapSurveyTypeInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::pair<int,int> prData;
        std::vector<int> vecSurveyIDList;

        _JIF( kLuaMng_.GetValue( "DungeonID", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Difficult", prData.second ) == S_OK, return false );
        _JIF( LoadSurveyIDInfo( kLuaMng_, std::string("SurveyIDList"), vecSurveyIDList ), return false );

        if ( false == mapSurveyTypeInfo_.insert( std::make_pair( prData, vecSurveyIDList ) ).second ) {
            START_LOG( cerr, L" 중복된 던전 정보가 있음. DungeonID : " << prData.first << L", Difficult : " << prData.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadLevelUpTypeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::map< std::pair<int,int>, std::vector<int> > >& mapSurveyTypeInfo_ )
{
    int nType = 0;
    std::map< std::pair<int,int>, std::vector<int> > mapSurveyInfo;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Type", nType ) == S_OK, return false );
    _JIF( LoadLevelUpSurveyInfo( kLuaMng_, std::string( "LevelInfo" ), mapSurveyInfo ), return false );

    if ( false == mapSurveyTypeInfo_.insert( std::make_pair( nType, mapSurveyInfo ) ).second ) {
        START_LOG( cerr, L" 중복된 LevelUp Type정보가 있음. Type : " << nType ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadLevelUpSurveyInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< std::pair<int,int>, std::vector<int> >& mapSurveyTypeInfo_ )
{
    mapSurveyTypeInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::pair<int,int> prData;
        std::vector<int> vecSurveyIDList;

        _JIF( kLuaMng_.GetValue( "CharType", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "CharLevel", prData.second ) == S_OK, return false );
        _JIF( LoadSurveyIDInfo( kLuaMng_, std::string("SurveyIDList"), vecSurveyIDList ), return false );

        if ( false == mapSurveyTypeInfo_.insert( std::make_pair( prData, vecSurveyIDList ) ).second ) {
            START_LOG( cerr, L" 중복된 캐릭터 레벨 정보가 있음. CharType : " << prData.first << L", CharLevel : " << prData.second ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadSurveyIDInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< int >& vecSurveyIDList_ )
{
    vecSurveyIDList_.clear();
    int nSurveyID;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.GetValue( i, nSurveyID ) != S_OK ) break;
        vecSurveyIDList_.push_back( nSurveyID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime( nYear, nMonth, nDay, nHour, 0, 0 ) );

    return true;
}

bool KCnSurvey::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, VEC_ITEMS >& mapRewards_ )
{
    mapRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        int nRewardType;
        VEC_ITEMS vecItems;
        vecItems.clear();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "RewardType", nRewardType ) == S_OK, return false );
        _JIF( LoadItems( kLuaMng_, std::string("RewardInfo"), vecItems ), return false );

        if( !mapRewards_.insert( std::make_pair( nRewardType, vecItems ) ).second ) {
            START_LOG( cerr, L" 중복된 RewardType 정보가 있음. RewardType : " << nRewardType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_ITEMS& vecRewards_ )
{
    vecRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewards_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnSurvey::CheckTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }
    m_dwLastTick = ::GetTickCount();

    return true;
}

bool KCnSurvey::CheckChange()
{
    KLocker lock( m_csSurvey );
    if ( m_bChanged ) {
        m_bChanged = false;
        return true;
    }
    return false;
}

int KCnSurvey::GetSurveyVersion()
{
    KLocker lock( m_csSurvey );

    return m_nVersion;
}

void KCnSurvey::GetSurveyData( OUT KECN_SURVEY_DATA_NOT& kPacket_ ) // 겜서버에 전달할 패킷 설정
{
    KLocker lock( m_csSurvey );
    kPacket_.m_nVersion = m_nVersion;
    kPacket_.m_tmBegin = m_tmBegin;
    kPacket_.m_tmEnd = m_tmEnd;
    kPacket_.m_tmReturnTime = m_tmReturnTime;
    kPacket_.m_mapCompressedSurveyInfoPack = m_mapCompressedSurveyInfoPack;
    kPacket_.m_mapSurveyTypeInfo = m_mapSurveyTypeInfo;
    kPacket_.m_mapSurveyRewardInfo = m_mapSurveyRewardInfo;
}

void KCnSurvey::UpdateSurveyData( IN std::map< int, std::map< int, KSurveyQuestion > >& mapSurveyAll_ )
{
    KLocker lock( m_csSurvey );
    m_mapSurveyAll = mapSurveyAll_;
    CompressSurveys( mapSurveyAll_ );
    m_bChanged = true;
}

void KCnSurvey::Tick()
{
    if ( CheckTick() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_SURVEY_DATA_NOT, L"", 0 );
    }

    if ( CheckChange() ) {
        KECN_SURVEY_DATA_NOT kPacket;
        GetSurveyData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_SURVEY_DATA_NOT, kPacket );
    }
}

void KCnSurvey::CompressSurveys( IN std::map< int, std::map< int, KSurveyQuestion > >& mapSurveyAll_ )
{
    // lock은 호출부에 걸려있음
    m_mapCompressedSurveyInfoPack.clear();
    std::map< int, std::map< int, KSurveyQuestion > >::iterator mitTotal;
    std::map< int, KSurveyQuestion >::iterator mitType;
    std::map< int, KSerBuffer > mapTypeSurveyInfo;
    mapTypeSurveyInfo.clear();

    for( mitTotal = mapSurveyAll_.begin(); mitTotal != mapSurveyAll_.end(); ++mitTotal ) {
        for( mitType = mitTotal->second.begin(); mitType != mitTotal->second.end(); ++mitType ) {
            KSerBuffer kBuff;
            KSerializer ks;
            ks.BeginWriting( &kBuff );
            if( !ks.Put( mitType->second ) ) continue;
            ks.EndWriting();
            kBuff.Compress();
            if ( !mapTypeSurveyInfo.insert( std::make_pair( mitType->first, kBuff) ).second ) {
                START_LOG( cerr, L"문제 압축중 중복된 문제가 존재 ID : " << mitType->first ) << END_LOG;
            }
        }
        if ( !m_mapCompressedSurveyInfoPack.insert( std::make_pair( mitTotal->first, mapTypeSurveyInfo) ).second ) {
            START_LOG( cerr, L"문제 압축중 중복된 국가정보 존재 ID : " << mitTotal->first ) << END_LOG;
        }
    }
}

bool KCnSurvey::GetLanguageInfo( OUT std::vector< int >& vecLanguageInfo_ )
{
    vecLanguageInfo_.clear();

    KLocker lock( m_csSurvey );
    vecLanguageInfo_ = m_vecLanguageInfo;

    return true;
}

bool KCnSurvey::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csSurvey );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}