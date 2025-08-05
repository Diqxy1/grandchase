#include "CharismasPresent.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include <kncutil.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCharismasPresent );
ImplOstmOperatorW2A( KCharismasPresent );
NiImplementRootRTTI( KCharismasPresent );

KCharismasPresent::KCharismasPresent(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwVersion(1)
,m_SledgeItem(0)
,m_dwMaxGrade(1)
,m_bLoopEnable(false)
,m_dwMaxPoint(0)
{
    m_mapPresentItemInfo.clear();
    m_mapDonationInfo.clear();
    m_mapGradeRewardList.clear();
    m_mapGradeDesc.clear();
}

KCharismasPresent::~KCharismasPresent(void)
{
}

ImplToStringW( KCharismasPresent )
{
    KLocker lock( m_csCharismasPresent );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_dwVersion )
        << TOSTRINGW( m_SledgeItem )
        << TOSTRINGW( m_dwMaxGrade )
        << TOSTRINGW( m_dwMaxPoint )
        << TOSTRINGWb( m_bLoopEnable )
        << TOSTRINGW( m_mapPresentItemInfo.size() )
        << TOSTRINGW( m_mapDonationInfo.size() )
        << TOSTRINGW( m_mapGradeRewardList.size() );
}

bool KCharismasPresent::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCharismasPresent::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    GCITEMID    SledgeItem;
    DWORD       dwVersion;
    DWORD       dwMaxGrade;
    bool        bLoopEnable;
    DWORD       dwMaxPoint;
    std::map<KDropItemInfo,std::pair<DWORD,DWORD>>  mapPresentItemInfo;
    std::map<DWORD,DWORD>                           mapDonationInfo;
    std::map<DWORD,VEC_REWARD>                      mapGradeRewardList;
    std::map<DWORD,std::pair<DWORD,DWORD>>          mapGradeDesc;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitCharismasPresent.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    // 이벤트 설정값 읽어오기.
    _JIF( kLuaMng.GetValue( "Version", dwVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SledgeItem", SledgeItem ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "LoopEnable", bLoopEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxGrade", dwMaxGrade ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxPoint", dwMaxPoint ) == S_OK, return false );

    _JIF( LoadPresentItemInfo( kLuaMng, std::string("PresentItemInfo"), mapPresentItemInfo ), return false );
    _JIF( LoadDonationInfo( kLuaMng, std::string("DonationInfo"), mapDonationInfo ), return false );
    _JIF( LoadGradeRewardList( kLuaMng, std::string("GradeRewardList"), mapGradeRewardList ), return false );
    _JIF( LoadGradeDescInfo( kLuaMng, std::string( "GradeDesc" ), mapGradeDesc ), return false );

    START_LOG( cerr, L"크리스마스 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csCharismasPresent );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwVersion = dwVersion;
        m_SledgeItem = SledgeItem;
        m_dwMaxGrade = dwMaxGrade;
        m_dwMaxPoint = dwMaxPoint;
        m_bLoopEnable = bLoopEnable;

        m_mapPresentItemInfo.swap( mapPresentItemInfo );
        m_mapDonationInfo.swap( mapDonationInfo );
        m_mapGradeRewardList.swap( mapGradeRewardList );
        m_mapGradeDesc.swap( mapGradeDesc );

        return true;
    }
}

bool KCharismasPresent::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCharismasPresent::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
{
    vecRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardInfo_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharismasPresent::LoadPresentItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<KDropItemInfo,std::pair<DWORD,DWORD>>& mapPresentItemInfo_ )
{
    mapPresentItemInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        KDropItemInfo kDropItemInfo;
        std::pair<DWORD,DWORD> prData;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( LoadDropItemInfo( kLuaMng_, std::string("ItemInfo"), kDropItemInfo ), return false );
        _JIF( LoadItemConfig( kLuaMng_, std::string("Config"), prData ), return false );

        if( !mapPresentItemInfo_.insert( std::make_pair( kDropItemInfo, prData ) ).second ) {
            START_LOG( cerr, L" 중복된 Item 정보가 있음. ItemID : " << kDropItemInfo.m_ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharismasPresent::LoadDropItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KDropItemInfo& kDropItemInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, kDropItemInfo_.m_ItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, kDropItemInfo_.m_nDuration ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kDropItemInfo_.m_nPeriod ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharismasPresent::LoadItemConfig( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair<DWORD,DWORD>& prData_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "SledgeUseCnt", prData_.first ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "BonusPoint", prData_.second ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharismasPresent::LoadDonationInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,DWORD>& mapDonationInfo_ )
{
    mapDonationInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        DWORD dwSledgeUseCnt;
        DWORD dwBonusPoint;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "SledgeUseCnt", dwSledgeUseCnt ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "BonusPoint", dwBonusPoint ) == S_OK, return false );

        if( !mapDonationInfo_.insert( std::make_pair( dwSledgeUseCnt, dwBonusPoint ) ).second ) {
            START_LOG( cerr, L" 중복된 사용카운트 정보가 있음. SledgeUseCnt : " << dwSledgeUseCnt ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharismasPresent::LoadGradeRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,VEC_REWARD>& mapGradeRewardList_ )
{
    mapGradeRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        DWORD dwGrade;
        VEC_REWARD vecReward;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "Grade", dwGrade ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("Reward"), vecReward ), return false );

        if( !mapGradeRewardList_.insert( std::make_pair( dwGrade, vecReward ) ).second ) {
            START_LOG( cerr, L" 중복된 Grade 정보가 있음. Grade : " << dwGrade ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharismasPresent::LoadGradeDescInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc_ )
{
    mapGradeDesc_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        DWORD dwGradeID;
        std::pair<DWORD,DWORD> prData;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "GradeID", dwGradeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "StratPoint", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EndPoint", prData.second ) == S_OK, return false );

        if( !mapGradeDesc_.insert( std::make_pair( dwGradeID, prData ) ).second ) {
            START_LOG( cerr, L" 중복된 등급 Desc 정보가 있음. GraneID : " << dwGradeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCharismasPresent::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csCharismasPresent );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KCharismasPresent::GetItemConfig( IN KDropItemInfo kDropItemInfo, OUT DWORD& dwSledgeUseCnt_, OUT DWORD& dwBonusPoint_ )
{
    std::map<KDropItemInfo,std::pair<DWORD,DWORD>>::iterator mit;

    KLocker lock( m_csCharismasPresent );
    mit = m_mapPresentItemInfo.find( kDropItemInfo );
    if( mit == m_mapPresentItemInfo.end() ) {
        return false;
    }
    dwSledgeUseCnt_ = mit->second.first;
    dwBonusPoint_ = mit->second.second;
    return true;
}

bool KCharismasPresent::GetDonationInfo( IN const DWORD dwSledgeUseCnt_, OUT DWORD& dwBonusPoint_ )
{
    std::map<DWORD,DWORD>::iterator mit;

    KLocker lock( m_csCharismasPresent );
    mit = m_mapDonationInfo.find( dwSledgeUseCnt_ );
    if( mit == m_mapDonationInfo.end() ) {
        return false;
    }
    dwBonusPoint_ = mit->second;
    return true;
}

bool KCharismasPresent::GetGradeRewardList( IN const DWORD dwGrade_, OUT VEC_REWARD& vecReward_ )
{
    std::map<DWORD,VEC_REWARD>::iterator mit;

    KLocker lock( m_csCharismasPresent );
    mit = m_mapGradeRewardList.find( dwGrade_ );
    if( mit == m_mapGradeRewardList.end() ) {
        return false;
    }
    vecReward_ = mit->second;
    return true;
}

bool KCharismasPresent::GetPointGrade( IN const DWORD dwBonusPoint_, OUT DWORD& dwGrade_ )
{
    std::map<DWORD,std::pair<DWORD,DWORD>>::iterator mit;
    for( mit = m_mapGradeDesc.begin(); mit != m_mapGradeDesc.end(); ++mit ) {
        if( mit->second.first <= dwBonusPoint_ && mit->second.second >= dwBonusPoint_ ) {
            dwGrade_ = mit->first;
            return true;
        }
    }
    return false;
}

void KCharismasPresent::GetCharismasPresentInfo( OUT KEVENT_CHARISMAS_EVENT_NOT& kPacket_ )
{
    KLocker lock( m_csCharismasPresent );

    kPacket_.m_SledgeItem = m_SledgeItem;
    kPacket_.m_mapPresentItemInfo = m_mapPresentItemInfo;
    kPacket_.m_mapDonationInfo = m_mapDonationInfo;
    kPacket_.m_mapGradeRewardList = m_mapGradeRewardList;
    kPacket_.m_mapGradeDesc = m_mapGradeDesc;
}

//=========================================================================================//
KCharismasUserData::KCharismasUserData()
:m_bLoaded(false)
,m_dwGrade(1)
,m_dwBonusPoint(0)
{
}

KCharismasUserData::~KCharismasUserData()
{
}

void KCharismasUserData::SetUserCharismasEventData( IN const DWORD dwGrade_, IN const DWORD& dwBonusPoint_, IN const DWORD& dwMaxPoint_, IN const bool& bInitEnable_ )
{
    m_bLoaded = true;
    m_dwGrade = dwGrade_;
    if( bInitEnable_ ) {
        m_dwBonusPoint = 0;
    } else {
        m_dwBonusPoint += dwBonusPoint_;
    }
    m_dwBonusPoint = std::min<DWORD>( m_dwBonusPoint, dwMaxPoint_ );
}

void KCharismasUserData::GetUserCharismasEventData( OUT DWORD& dwGrade_, OUT DWORD& dwBonusPoint_ )
{
    dwGrade_ = m_dwGrade;
    dwBonusPoint_ = m_dwBonusPoint;
}

bool KCharismasUserData::CheckUserGrade( IN const DWORD dwGrade_ )
{
    if( dwGrade_ == m_dwGrade ) {
        return true;
    }
    return false;
}