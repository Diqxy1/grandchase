#include "GSSimLayer.h"
#include "RankManager.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "NetError.h"
#include <boost/bind.hpp>
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "Log4.h"
#define PAGE_SIZE 10

//FILE_NAME_FOR_LOG
ImplementSingleton( KRankManager );
NiImplementRootRTTI( KRankManager );
ImplOstmOperatorW2A( KRankManager );

unsigned int KRankManager::ms_uiRankSearchGap(1000);

ImplToStringW( KRankManager )
{
    KLocker lock( m_csRankData );
    return START_TOSTRINGW
        << TOSTRINGW( m_nUpdateHour )
        << TOSTRINGW( m_nUpdateMinute )
        << L"    Next Update : " << (LPCTSTR)m_tmUpdateTime.Format( KNC_TIME_FORMAT )
        << TOSTRINGW( ms_uiRankSearchGap )
        << TOSTRINGW( m_mapRank.size() )
        << TOSTRINGW( m_mapCompRankData.size() )
        << TOSTRINGW( m_mapRankRewardInfo.size() )
        << TOSTRINGW( m_mapMatchRank.size() )
        << TOSTRINGW( m_mapCompMatchRankData.size() )
        << TOSTRINGW( m_mapMatchExpRank.size() );
}

KRankManager::KRankManager(void)
:m_nUpdateHour( 8 )
,m_nUpdateMinute( 0 )
{
    m_dwLastTick = ::GetTickCount();

    m_mapExpRank.clear();
    m_mapRank.clear();
    m_mapCompRankData.clear();
    m_mapRankRewardInfo.clear();
    m_mapMatchRank.clear();
    m_mapCompMatchRankData.clear();
    m_mapMatchExpRank.clear();
}

KRankManager::~KRankManager(void)
{
}

void KRankManager::Tick()
{
    if( ::GetTickCount() - m_dwLastTick > 10 * 60 * 1000 ) {
        if( CheckUpdate() ) {
            SiKGSDBLayer()->QueueingID( KUserEvent::EVENT_DB_UPDATE_RANK_DATA_REQ, L"", 0 );

            SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_UPDATE_MATCH_RANK_DATA_REQ, L"", 0 ); // 매칭랭킹.
            SendToDayChangeNotice();
        }
        m_dwLastTick = ::GetTickCount();
    }
}

void KRankManager::SetUpdateTime( int nHour_, int nMiute_ )
{
    JIF( nHour_ >= 0 && nMiute_ >= 0 );

    KLocker lock( m_csRankData );
    m_nUpdateHour = nHour_;
    m_nUpdateMinute = nMiute_;
    UpdateFlushTime();
}

void KRankManager::UpdateFlushTime()
{
    CTime tmCurrent = CTime::GetCurrentTime();
    CTimeSpan ts(1,0,0,0); // 내일.
    KLocker lock( m_csRankData );
    tmCurrent += ts;
    m_tmUpdateTime = CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), m_nUpdateHour, m_nUpdateMinute, 0 );
}

bool KRankManager::CheckUpdate()
{
    KLocker lock( m_csRankData );
    if( m_tmUpdateTime > CTime::GetCurrentTime() ) return false;
    UpdateFlushTime();
    return true;
}

bool KRankManager::GetPageData( char cRankType_, char cCharType_, char cPage_, KEVENT_RANK_PAGE_ACK& kPacket_ )
{
    KLocker lock( m_csRankData );

    std::map<char, std::map<char, std::vector<KListPage> > >::iterator mit;
    mit = m_mapCompRankData.find( cRankType_ );
    _JIF( mit != m_mapCompRankData.end(), SET_ERROR( ERR_RANK_00 ); return false );

    std::map<char, std::vector<KListPage> >::iterator mit1;
    mit1 = mit->second.find( cCharType_ );
    _JIF( mit1 != mit->second.end(), SET_ERROR( ERR_RANK_01 ); return false );

    std::vector<KListPage>& vecPages = mit1->second;
    std::vector<KListPage>::iterator vit;
    vit = std::find_if( vecPages.begin(), vecPages.end(),
            boost::bind( &KListPage::m_nPageNum, _1) == (int)(cPage_) );
    _JIF( vit != vecPages.end(), SET_ERROR( ERR_RANK_02 ); return false );

    kPacket_.m_cRnkType         = cRankType_;
    kPacket_.m_cCharType        = cCharType_;
    kPacket_.m_cPageNum         = cPage_;
    kPacket_.m_cTotalPageCount  = (char)(vecPages.size());
    kPacket_.m_kCompBuff        = vit->m_buffComList;

    return true;
}

// 랭크 데이터 생성 관련.
void KRankManager::UpdateExpRankData( std::map<char, std::vector<KExpRank> >& mapExpRank_ )
{
    KLocker lock( m_csRankData );
    m_mapExpRank = mapExpRank_;
}

void KRankManager::UpdateWinRankData( char cRankType_, std::map<char, std::vector<KWinRateRank> >& mapWinRank_ )
{
   switch( cRankType_)
   {
   case RT_DAILY_RANK:
   case RT_WEEFLY_RANK:
   case RT_MONTHLY_RANK:
   case RT_GUILD_DAILY_RANK:
   case RT_GUILD_WEEKLY_RANK:
   case RT_GUILD_MONTHLY_RANK:
   case RT_GUILD_TOTAL_RANK:
       {
           KLocker lock( m_csRankData );
           m_mapRank[cRankType_] = mapWinRank_;
       }
       break;

   default:
       START_LOG( cerr, L" 랭킹 정보 업데이트중 알수 없는 타입.." )
           << BUILD_LOGc( cRankType_ )
           << BUILD_LOG( mapWinRank_.size() ) << END_LOG;
   }
}

void KRankManager::BuildRankData()
{
    KLocker lock( m_csRankData );
    m_mapCompRankData.clear();

    std::map< int, std::map<char, std::vector<KWinRateRank> > >::iterator mit;
    for( mit = m_mapRank.begin(); mit != m_mapRank.end() ; ++mit )
    {
        char kRankType = mit->first;
        std::map<char, std::vector<KWinRateRank> >::iterator mitRank;
        for( mitRank = mit->second.begin() ; mitRank != mit->second.end() ; ++mitRank )
    {
        std::vector<KListPage> vecRankCompData;
            BuildWinRankData( mitRank->second, vecRankCompData );
            AssignCompData( kRankType, mitRank->first, vecRankCompData );
    }
    }

    std::map<char, std::vector<KExpRank> >::iterator mit1;
    for( mit1 = m_mapExpRank.begin() ; mit1 != m_mapExpRank.end() ; ++mit1 )
    {
        std::vector<KListPage> vecRankCompData;
        BuildExpRankData( mit1->second, vecRankCompData );
        AssignCompData( RT_EXP_RANK, mit1->first, vecRankCompData );
    }
}

void KRankManager::BuildWinRankData( IN std::vector<KWinRateRank>& vecRankData_,
                                     OUT std::vector<KListPage>& vecRankCompData_ )
{
    std::vector<KWinRateRank>::iterator vit;
    std::vector<KWinRateRank> vecRankPage;

    vecRankCompData_.clear();

    vecRankPage.clear();
    vecRankPage.reserve( 10 );
    int nPageCount = 0;
    for( vit = vecRankData_.begin() ; vit != vecRankData_.end() ; ++vit )
    {
        vecRankPage.push_back( *vit );
        if( vecRankPage.size() == PAGE_SIZE )
        {
            KListPage kListPage;
            kListPage.m_nPageNum = nPageCount;
            KSerializer         ks;
            // serialize - only data
            ks.BeginWriting( &kListPage.m_buffComList );
            ks.Put( vecRankPage );
            ks.EndWriting();
            
            vecRankCompData_.push_back( kListPage );

            vecRankPage.clear();
            vecRankPage.reserve( 10 );

            ++nPageCount;
        }
    }

    if( !vecRankPage.empty() )
    {
        KListPage kListPage;
        kListPage.m_nPageNum = nPageCount;
        KSerializer         ks;
        // serialize - only data
        ks.BeginWriting( &kListPage.m_buffComList );
        ks.Put( vecRankPage );
        ks.EndWriting();

        vecRankCompData_.push_back( kListPage );
    }
}

void KRankManager::BuildExpRankData( IN std::vector<KExpRank>& vecRankData_,
                                     OUT std::vector<KListPage>& vecRankCompData_ )
{
    std::vector<KExpRank>::iterator vit;
    std::vector<KExpRank> vecRankPage;

    vecRankCompData_.clear();

    vecRankPage.clear();
    vecRankPage.reserve( 10 );
    int nPageCount = 0;
    for( vit = vecRankData_.begin() ; vit != vecRankData_.end() ; ++vit )
    {
        vecRankPage.push_back( *vit );
        if( vecRankPage.size() == PAGE_SIZE )
        {
            KListPage kListPage;
            kListPage.m_nPageNum = nPageCount;
            KSerializer         ks;
            // serialize - only data
            ks.BeginWriting( &kListPage.m_buffComList );
            ks.Put( vecRankPage );
            ks.EndWriting();

            vecRankCompData_.push_back( kListPage );

            vecRankPage.clear();
            vecRankPage.reserve( 10 );

            ++nPageCount;
        }
    }

    if( !vecRankPage.empty() )
    {
        KListPage kListPage;
        kListPage.m_nPageNum = nPageCount;
        KSerializer         ks;
        // serialize - only data
        ks.BeginWriting( &kListPage.m_buffComList );
        ks.Put( vecRankPage );
        ks.EndWriting();

        vecRankCompData_.push_back( kListPage );
    }
}

void KRankManager::AssignCompData( char cRankType_, char cCharType_, std::vector<KListPage>& vecCompData_ )
{
    m_mapCompRankData[cRankType_][cCharType_] = vecCompData_;
}

void KRankManager::DumpExpRank()
{
    KLocker lock( m_csRankData );

    START_LOG( cerr, L"===== ExpRank ====="  ) << END_LOG;
    std::map<char, std::vector<KExpRank> >::const_iterator mit;
    for( mit = m_mapMatchExpRank.begin() ; mit != m_mapMatchExpRank.end() ; ++mit ) {
        START_LOG( cerr, L"( " << mit->first << L") "
            << L"Size : " << mit->second.size() )
            << END_LOG;
    }
}

void KRankManager::DumpMatchRank()
{
    KLocker lock( m_csRankData );

    START_LOG( cerr, L"===== MatchRank ====="  ) << END_LOG;
    std::map< int, std::map<char, std::vector<KMatchRank>>>::const_iterator mit;
    for( mit = m_mapMatchRank.begin() ; mit != m_mapMatchRank.end() ; ++mit ) {
        START_LOG( cerr, L"( " << mit->first << L") "
            << L"Size : " << mit->second.size() )
            << END_LOG;
    }
}

void KRankManager::SetRankSearchTime( IN unsigned int uiRankSearchTime_ )
{
    ms_uiRankSearchGap = uiRankSearchTime_;
}

bool KRankManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KRankManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    int nUpdateHour;
    int nUpdateMinute;
    std::map<int, std::map<char, std::vector<KRankRewardInfo>>> mapRankRewardInfo;

    const std::string strScriptName = "InitRankInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadUpdateTime( kLuaMng, std::string("DBUpdateTime"), nUpdateHour, nUpdateMinute ), return false );
    _JIF( LoadRankRewardInfo( kLuaMng, std::string("RankRewardInfo"), mapRankRewardInfo ), return false );

    START_LOG( cerr, L"랭킹 스크립트 로드 완료. " ) << END_LOG;

    SetUpdateTime( nUpdateHour, nUpdateMinute );
    {
        KLocker lock( m_csRankData );
        m_mapRankRewardInfo.swap( mapRankRewardInfo );
    }
    return true;
}

bool KRankManager::LoadUpdateTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT int& nUpdateHour_, OUT int& nUpdateMinute_ )
{
    int nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    nUpdateHour_ = nHour;
    nUpdateMinute_ = nMin;

    return true;
}

bool KRankManager::LoadRankRewardInfo( KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, std::map<char, std::vector<KRankRewardInfo>>>& mapRankRewardInfo_ )
{
    mapRankRewardInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nRankType = 0;
        std::map<char, std::vector<KRankRewardInfo>> mapRewardInfo;
        mapRewardInfo.clear();

        _JIF( kLuaMng_.GetValue( "RankType", nRankType ) == S_OK, return false );

        for( int i = 1; ; ++i ) {
            if( kLuaMng_.BeginTable( i ) != S_OK ) break;
            std::vector<KRankRewardInfo> vecRewardInfo;
            vecRewardInfo.clear();
            int nCharType = 0;
            _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );

            for( int i = 1; ; ++i ) {
                if( kLuaMng_.BeginTable( i ) != S_OK ) break;
                KRankRewardInfo kRankRewardInfo;
                kRankRewardInfo.m_nRankType = nRankType;
                _JIF( kLuaMng_.GetValue( "StartRank", kRankRewardInfo.m_nStartRank ) == S_OK, return false );
                _JIF( kLuaMng_.GetValue( "EndRank", kRankRewardInfo.m_nEndRank ) == S_OK, return false );
                _JIF( LoadItems( kLuaMng_, std::string("Reward"), kRankRewardInfo.m_vecRewardInfo ), return false );
                kRankRewardInfo.m_cCharType = static_cast<char>(nCharType);
                vecRewardInfo.push_back( kRankRewardInfo );
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }

            mapRewardInfo[nCharType] = vecRewardInfo;
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
        }

        if( !mapRankRewardInfo_.insert( std::make_pair( nRankType, mapRewardInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 RankType 정보가 있음 : " << nRankType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KRankManager::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<GCITEMID>& vecItems_ )
{
    vecItems_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        GCITEMID ItemID;
        if( kLuaMng_.GetValue( i++, ItemID ) != S_OK ) break;
        vecItems_.push_back( ItemID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KRankManager::UpdateMatchRankData( IN const char& cRankType_, std::map<char, std::vector<KMatchRank> >& mapMatchRank_ )
{
    KLocker lock( m_csRankData );
    m_mapMatchRank[cRankType_] = mapMatchRank_;
}

void KRankManager::UpdateMatchExpRankData( std::map<char, std::vector<KExpRank> >& mapMatchExpRank_ )
{
    KLocker lock( m_csRankData );
    m_mapMatchExpRank = mapMatchExpRank_;
}

void KRankManager::BuildMatchRankData()
{
    KLocker lock( m_csRankData );
    m_mapCompMatchRankData.clear();

    std::map< int, std::map<char, std::vector<KMatchRank> > >::iterator mitMatch;
    for( mitMatch = m_mapMatchRank.begin(); mitMatch != m_mapMatchRank.end() ; ++mitMatch ) {
        char cRankType = mitMatch->first;
        std::map<char, std::vector<KMatchRank> >::iterator mitMatchRank;
        for( mitMatchRank = mitMatch->second.begin() ; mitMatchRank != mitMatch->second.end() ; ++mitMatchRank ) {
            std::vector<KListPage> vecMatchRankCompData;
            BuildMatchRankData( mitMatchRank->second, vecMatchRankCompData );
            AssignMatchCompData( cRankType, mitMatchRank->first, vecMatchRankCompData );
        }
    }

    std::map<char, std::vector<KExpRank> >::iterator mitExp;
    for( mitExp = m_mapExpRank.begin() ; mitExp != m_mapExpRank.end() ; ++mitExp ) {
        std::vector<KListPage> vecRankCompData;
        BuildExpRankData( mitExp->second, vecRankCompData );
        AssignMatchCompData( MRT_EXP, mitExp->first, vecRankCompData );
    }
}

void KRankManager::BuildMatchRankData( IN std::vector<KMatchRank>& vecMatchRankData_,
                                    OUT std::vector<KListPage>& vecMatchRankCompData_ )
{
    std::vector<KMatchRank>::iterator vit;
    std::vector<KMatchRank> vecMatchRankPage;

    vecMatchRankCompData_.clear();

    vecMatchRankPage.clear();
    vecMatchRankPage.reserve( PAGE_SIZE );
    int nPageCount = 0;
    for( vit = vecMatchRankData_.begin() ; vit != vecMatchRankData_.end() ; ++vit ) {
        vecMatchRankPage.push_back( *vit );
        if( vecMatchRankPage.size() == PAGE_SIZE ) {
            KListPage kListPage;
            kListPage.m_nPageNum = nPageCount;
            KSerializer         ks;
            // serialize - only data
            ks.BeginWriting( &kListPage.m_buffComList );
            ks.Put( vecMatchRankPage );
            ks.EndWriting();

            vecMatchRankCompData_.push_back( kListPage );

            vecMatchRankPage.clear();
            vecMatchRankPage.reserve( 10 );

            ++nPageCount;
        }
    }

    if( !vecMatchRankPage.empty() ) {
        KListPage kListPage;
        kListPage.m_nPageNum = nPageCount;
        KSerializer         ks;
        // serialize - only data
        ks.BeginWriting( &kListPage.m_buffComList );
        ks.Put( vecMatchRankPage );
        ks.EndWriting();

        vecMatchRankCompData_.push_back( kListPage );
    }
}

void KRankManager::AssignMatchCompData( IN const char cRankType_, IN const char cCharType_, IN const std::vector<KListPage>& vecCompData_ )
{
    m_mapCompMatchRankData[cRankType_][cCharType_] = vecCompData_;
}

bool KRankManager::GetMatchRankPageData( IN const char cRankType_, IN const char cCharType_, IN const char cPage_, OUT KEVENT_MATCH_RANK_PAGE_ACK& kPacket_ )
{
    KLocker lock( m_csRankData );

    std::map<char, std::map<char, std::vector<KListPage> > >::iterator mitRankType;
    mitRankType = m_mapCompMatchRankData.find( cRankType_ );
    _JIF( mitRankType != m_mapCompMatchRankData.end(), SET_ERROR( ERR_RANK_00 ); return false );

    std::map<char, std::vector<KListPage> >::iterator mitCharType;
    mitCharType = mitRankType->second.find( cCharType_ );
    _JIF( mitCharType != mitRankType->second.end(), SET_ERROR( ERR_RANK_01 ); return false );

    std::vector<KListPage>& vecPages = mitCharType->second;
    std::vector<KListPage>::iterator vit;
    vit = std::find_if( vecPages.begin(), vecPages.end(),
        boost::bind( &KListPage::m_nPageNum, _1) == (int)(cPage_) );
    _JIF( vit != vecPages.end(), SET_ERROR( ERR_RANK_02 ); return false );

    kPacket_.m_cRankType        = cRankType_;
    kPacket_.m_cCharType        = cCharType_;
    kPacket_.m_cPageNum         = cPage_;
    kPacket_.m_cTotalPageCount  = (char)(vecPages.size());
    kPacket_.m_kCompBuff        = vit->m_buffComList;

    return true;
}

void KRankManager::GetRankRewardInfo( OUT KEVENT_MATCH_RANK_REWARD_NOT& kPacket_ )
{
    // 서버에서는 보상 데이터 구조를 확장성 있는 구조로 관리.
    // 클라에서 어차피 확장성이 없기 때문에 데이터 접근이 편한 구조로 패킷 나눠줌.
    kPacket_.m_mapRPRewardInfo.clear();
    kPacket_.m_mapWinRecordRewardInfo.clear();

    KLocker lock( m_csRankData );
    std::map<int, std::map<char, std::vector<KRankRewardInfo>>>::iterator mitRP;
    std::map<int, std::map<char, std::vector<KRankRewardInfo>>>::iterator mitWinRecord;
    mitRP = m_mapRankRewardInfo.find( MRT_RP );
    if( mitRP != m_mapRankRewardInfo.end() ) {
        kPacket_.m_mapRPRewardInfo = mitRP->second;
    }

    mitWinRecord = m_mapRankRewardInfo.find( MRT_WINRECORD );
    if( mitWinRecord != m_mapRankRewardInfo.end() ) {
        kPacket_.m_mapWinRecordRewardInfo = mitWinRecord->second;
    }
}

bool KRankManager::CheckRankType( IN const char& cRankType_ )
{
    if( cRankType_ < MRT_RP || cRankType_ > MRT_MAX ) {
        return false;
    }

    return true;
}

void KRankManager::GetUpdateTime( OUT KEVENT_MATCH_RANK_CHANGE_NOT& kPacket_ )
{
    KLocker lock( m_csRankData );
    kPacket_.m_nUpdateHour = m_nUpdateHour;
    kPacket_.m_nUpdateMinute = m_nUpdateMinute;
}

void KRankManager::SendToDayChangeNotice()
{
    KEVENT_MATCH_RANK_CHANGE_NOT kPacket;
    GetUpdateTime( kPacket );
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_MATCH_RANK_CHANGE_NOT, kPacket );
}
