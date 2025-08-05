#include "ChannelTypeEvent.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "UserPacket.h"
#include "GameServer.h"
#include "PetHelper.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KChannelTypeEvent );
ImplOstmOperatorW2A( KChannelTypeEvent );
NiImplementRootRTTI( KChannelTypeEvent );

KChannelTypeEvent::KChannelTypeEvent(void)
:m_tmBegin(0)
,m_tmEnd(0)
,m_tNewUserBegin(0)
{
    // Init.
    m_mapDailyConnectionReward.clear();
    m_mapOneConnectionReward.clear();
    m_mapNewConnectionReward.clear();
}

KChannelTypeEvent::~KChannelTypeEvent(void)
{
}

ImplToStringW( KChannelTypeEvent )
{
    KLocker lock( m_csChannelType );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tNewUserBegin) )
        << TOSTRINGW( m_mapDailyConnectionReward.size() )
        << TOSTRINGW( m_mapOneConnectionReward.size() )
        << TOSTRINGW( m_mapNewConnectionReward.size() )
        << TOSTRINGW( m_mapChannelRatioInfo.size() )
        << TOSTRINGW( m_mapPetReward.size() );
}

bool KChannelTypeEvent::LoadScript()
{
    return _LoadScript(std::string());
}

bool KChannelTypeEvent::_LoadScript( OUT std::string& strScript_ )
{
    time_t                              tmBegin;
    time_t                              tmEnd;
    time_t                              tNewUserBegin;
    std::map< DWORD, VEC_ITEMS >        mapDailyConnectionReward;
    std::map< DWORD, VEC_ITEMS >        mapOneConnectionReward;
    std::map< DWORD, VEC_ITEMS >        mapNewConnectionReward;
    std::map< DWORD, KChannelRatioInfo > mapChannelRatioInfo;
    std::map< DWORD, KRewardPet >       mapPetReward;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitChannelTypeEvent.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    // 신규 유저 체크 시작 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("NewBeginTime"), tNewUserBegin ), return false );

    // 보상 정보 읽어오기.
    _JIF( LoadRewardInfo( kLuaMng, std::string("DailyConnectionReward"), mapDailyConnectionReward ), return false );
    _JIF( LoadRewardInfo( kLuaMng, std::string("OneConnectionReward"), mapOneConnectionReward ), return false );
    _JIF( LoadRewardInfo( kLuaMng, std::string("NewConnectionReward"), mapNewConnectionReward ), return false );
    _JIF( LoadChannelRatio( kLuaMng, mapChannelRatioInfo ), return false ); // 추가 EXP/GP 배율 읽어오기.
    _JIF( LoadPetReward( kLuaMng, mapPetReward ), return false ); // 펫정보 읽어오기.

    START_LOG( cerr, L"채널 이벤트 정보 로드 완료. " ) << END_LOG;

    KLocker lock( m_csChannelType );
    m_tmBegin            = tmBegin;
    m_tmEnd              = tmEnd;
    m_tNewUserBegin      = tNewUserBegin;
    m_mapDailyConnectionReward.swap( mapDailyConnectionReward );
    m_mapOneConnectionReward.swap( mapOneConnectionReward );
    m_mapNewConnectionReward.swap( mapNewConnectionReward );
    m_mapChannelRatioInfo.swap( mapChannelRatioInfo );
    m_mapPetReward.swap( mapPetReward );

    return true;
}

bool KChannelTypeEvent::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_ITEMS& vecRewards_ )
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

bool KChannelTypeEvent::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KChannelTypeEvent::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, VEC_ITEMS >& mapRewards_ )
{
    mapRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i )
    {
        DWORD dwChannelType;
        VEC_ITEMS vecItems;
        vecItems.clear();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "ChannelType", dwChannelType ) == S_OK, return false );
        _JIF( LoadItems( kLuaMng_, std::string("Reward"), vecItems ), return false );

        if( !mapRewards_.insert( std::make_pair( dwChannelType, vecItems ) ).second )
        {
            START_LOG( cerr, L" 중복된 ChannelType정보가 있음. ChannelType : " << dwChannelType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KChannelTypeEvent::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csChannelType );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KChannelTypeEvent::GetDailyConnectionReward( IN const DWORD dwChannelType_, OUT VEC_ITEMS& vecItems_ )
{
    KLocker lock(m_csChannelType);

    std::map< DWORD, VEC_ITEMS >::iterator mit;
    mit = m_mapDailyConnectionReward.find( dwChannelType_ );
    if( mit == m_mapDailyConnectionReward.end() ) return false;
    vecItems_ = mit->second;
    return true;
}

bool KChannelTypeEvent::GetOneConnectionReward( IN const DWORD dwChannelType_, OUT VEC_ITEMS& vecItems_ )
{
    KLocker lock(m_csChannelType);

    std::map< DWORD, VEC_ITEMS >::iterator mit;
    // 전체유저 대상
    mit = m_mapOneConnectionReward.find( 0 );
    if( mit != m_mapOneConnectionReward.end() ) {
        vecItems_ = mit->second;
    }

    // 특정 채널링 대상
    mit = m_mapOneConnectionReward.find( dwChannelType_ );
    if( mit != m_mapOneConnectionReward.end() ) {
        vecItems_.insert( vecItems_.end(), mit->second.begin(), mit->second.end() );
    }

    return ( !vecItems_.empty() );
}

void KChannelTypeEvent::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock(m_csChannelType);
    tmDate_ = m_tmBegin;
}

void KChannelTypeEvent::GetNewUserCheckTime( OUT time_t& tmDate_ )
{
    KLocker lock(m_csChannelType);
    tmDate_ = m_tNewUserBegin;
}

bool KChannelTypeEvent::IsNewUserCheck( IN const time_t tmDate_ )
{
    KLocker lock( m_csChannelType );
    return ( m_tNewUserBegin <= tmDate_ );
}

bool KChannelTypeEvent::GetNewConnectionReward( IN const DWORD dwChannelType_, OUT VEC_ITEMS& vecItems_ )
{
    KLocker lock(m_csChannelType);

    std::map< DWORD, VEC_ITEMS >::iterator mit;
    // 전체유저 대상
    mit = m_mapNewConnectionReward.find( 0 );
    if( mit != m_mapNewConnectionReward.end() ) {
        vecItems_ = mit->second;
    }

    // 특정 채널링 대상
    mit = m_mapNewConnectionReward.find( dwChannelType_ );
    if( mit != m_mapNewConnectionReward.end() ) {
        vecItems_.insert( vecItems_.end(), mit->second.begin(), mit->second.end() );
    }

    return ( !vecItems_.empty() );
}

bool KChannelTypeEvent::LoadChannelRatio( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, KChannelRatioInfo >& mapChannelRatioInfo_ )
{
    mapChannelRatioInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "ChannelTypeRatio" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD dwChannelType = 0;
        KChannelRatioInfo kChannelRatioInfo;

        _JIF( kLuaMng_.GetValue( "ChannelType", dwChannelType ) == S_OK, return false );
        _JIF( LoadRatioInfo( kLuaMng_, std::string("Ratio"), kChannelRatioInfo ), return false );

        if( !mapChannelRatioInfo_.insert( std::make_pair( dwChannelType, kChannelRatioInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 ChannelType정보가 있음. ChannelType : " << dwChannelType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KChannelTypeEvent::LoadRatioInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KChannelRatioInfo& kChannelRatioInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "Exp", kChannelRatioInfo_.m_fExpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Gp", kChannelRatioInfo_.m_fGPRatio ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KChannelTypeEvent::GetExpGpBonus( IN const DWORD dwChannelType_, OUT float& fExpRatio_, OUT float& fGpRatio_ )
{
    KLocker lock(m_csChannelType );
    std::map<DWORD, KChannelRatioInfo>::iterator mit;
    mit = m_mapChannelRatioInfo.find( dwChannelType_ );
    if( mit == m_mapChannelRatioInfo.end() ) return;

    fExpRatio_ = mit->second.m_fExpRatio;
    fGpRatio_  = mit->second.m_fGPRatio;
}

bool KChannelTypeEvent::LoadPetReward( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, KRewardPet >& mapPetReward_ )
{
    mapPetReward_.clear();

    _JIF( kLuaMng_.BeginTable( "PetConnectionReward" ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD dwChannelType = 0;
        KRewardPet kPetInfo;

        _JIF( kLuaMng_.GetValue( "ChannelType", dwChannelType ) == S_OK, return false );
        _JIF( LoadDefaultPet( kLuaMng_, kPetInfo.m_vecDefaultPet ), return false );
        _JIF( LoadEventPet( kLuaMng_, kPetInfo.m_mapEventPet ), return false );

        if ( false == mapPetReward_.insert( std::make_pair( dwChannelType, kPetInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 ChannelType정보가 있음. ChannelType : " << dwChannelType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KChannelTypeEvent::LoadDefaultPet( IN KLuaManager& kLuaMng_, OUT std::vector<KPetInfo>& vecPet_ )
{
    vecPet_.clear();

    _JIF( kLuaMng_.BeginTable( "Default" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        KPetInfo kPet;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        if ( false == LoadPetInfo( kLuaMng_, kPet ) ) continue;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecPet_.push_back( kPet );
        START_LOG( clog, L"Default Pet ID : " << kPet.m_dwID << L", UID : " << kPet.m_dwUID  ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load DefaultPet size : " << vecPet_.size() ) << END_LOG;
    return true;
}

bool KChannelTypeEvent::LoadEventPet( IN KLuaManager& kLuaMng_, OUT std::map<PAIR_INT,std::vector<KPetInfo> >& mapPet_ )
{
    mapPet_.clear();

    _JIF( kLuaMng_.BeginTable( "Event" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nMonth, nDay;
        KPetInfo kPet;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        if ( kLuaMng_.BeginTable( "Date" ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nMonth ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDay ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == LoadPetInfo( kLuaMng_, kPet ) ) continue;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapPet_[ std::make_pair(nMonth,nDay) ].push_back( kPet );
        START_LOG( clog, L"Event Pet (" << nMonth << L"/" << nDay << L") ID : " << kPet.m_dwID << L", UID : " << kPet.m_dwUID  ) << END_LOG;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( clog, L"Load EventPet size : " << mapPet_.size() ) << END_LOG;
    return true;
}

bool KChannelTypeEvent::LoadPetInfo( IN KLuaManager& kLuaMng_, OUT KPetInfo& kPet_ )
{
    std::string strPetName;
    kPet_.Init();
    _JIF( kLuaMng_.GetValue( "PetItemID", kPet_.m_dwID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PetItemUID", kPet_.m_dwUID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PetStrName", strPetName ) == S_OK, return false );

    kPet_.m_strName = SiKGameServer()->m_stringTable.GetValue( KncUtil::toWideString( strPetName ).c_str() );
    _JIF( LoadSimpleItems( kLuaMng_, std::string("PetEquipItem"), kPet_.m_vecEquipItem ), return false );
    SetChannelPetInfo( kPet_ );
    return true;
}

bool KChannelTypeEvent::LoadSimpleItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KSimpleItem>& vecSimpleItem_ )
{
    vecSimpleItem_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        KSimpleItem kSimpleItem;
        int nItemType;
        if( kLuaMng_.GetValue( i++, kSimpleItem.m_dwID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, nItemType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kSimpleItem.m_dwUID ) == S_OK, return false );
        kSimpleItem.m_cItemType = nItemType;
        vecSimpleItem_.push_back( kSimpleItem );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KChannelTypeEvent::SetChannelPetInfo( IN OUT KPetInfo& kPetInfo_ )
{
    // Pet 초기 생성할 때의 기본적인 세팅.
    for( int i = 0; i <= 2; i++ ) {
        kPetInfo_.m_mapInitExp.insert( std::make_pair( i , SiKPetHelper()->GetPetMinExp() ) );
    }
    kPetInfo_.m_dwEXP = SiKPetHelper()->GetPetMinExp();
    kPetInfo_.m_iSatiation = KChannelTypeEvent::MININUM_PET_SATIATION;
    kPetInfo_.m_iInitSatiation = kPetInfo_.m_iSatiation;
    kPetInfo_.m_iLevel = SiKPetHelper()->GetPetLevel( kPetInfo_.m_dwEXP );
    kPetInfo_.m_nHatchingID = SiKPetHelper()->GetNextID( kPetInfo_.m_dwID );
}

void KChannelTypeEvent::GetChannelPetItem( IN const DWORD dwChannelType_, IN OUT std::map<GCITEMUID, KPetInfo>& m_mapPetInfo_, IN OUT std::vector<KItem>& vecItems_ )
{
    KLocker lock( m_csChannelType );

    std::map< DWORD, KRewardPet >::iterator mit;

    // 전체지급
    mit = m_mapPetReward.find( 0 );
    if ( mit != m_mapPetReward.end() ) {
        GetPetItem( mit->second, m_mapPetInfo_, vecItems_ );
    }

    // 채널링 지급
    mit = m_mapPetReward.find( dwChannelType_ );
    if ( mit != m_mapPetReward.end() ) {
        GetPetItem( mit->second, m_mapPetInfo_, vecItems_ );
    }
}

void KChannelTypeEvent::GetPetItem( IN const KRewardPet& kReward_, IN OUT std::map<GCITEMUID, KPetInfo>& m_mapPetInfo_, IN OUT std::vector<KItem>& vecItems_ )
{
    CTime tmToday = CTime::GetCurrentTime();
    time_t tmCurrent = KncUtil::TimeToInt( tmToday );
    time_t tmYesterday = KncUtil::TimeToInt( tmToday - CTimeSpan( 1, 0, 0, 0 ) );
    PAIR_INT prDate( tmToday.GetMonth(), tmToday.GetDay() );

    KItem kItem;
    kItem.m_cEnchantLevel   = 0;
    kItem.m_cGradeID        = 0;
    kItem.m_nPeriod         = -1;
    kItem.m_tRegDate        = tmCurrent;
    kItem.m_tStartDate      = tmCurrent;
    kItem.m_tEndDate        = tmYesterday;

    // 이벤트 기간 체크
    std::map<PAIR_INT,std::vector<KPetInfo> >::const_iterator mit;
    mit = kReward_.m_mapEventPet.find( prDate );
    if ( mit != kReward_.m_mapEventPet.end() ) {

        int nPetCount = 0;

        std::vector<KPetInfo>::const_iterator vit;
        for ( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit ) {
            if ( false == m_mapPetInfo_.insert( std::make_pair( vit->m_dwUID, *vit ) ).second ) {
                START_LOG( cwarn, L"채널링 이벤트 펫 입력시 중복 데이터. PetItemID : " << vit->m_dwID ) << END_LOG;
                continue;
            }

            // PetItem UserInven에 넣어주기.
            kItem.m_ItemID = vit->m_dwID;
            kItem.m_ItemUID = vit->m_dwUID;
            vecItems_.push_back( kItem );
            ++nPetCount;

            // Pet 장착 정보 UserInven에 넣어주기.
            std::vector<KSimpleItem>::const_iterator vitItem;
            for ( vitItem = vit->m_vecEquipItem.begin() ; vitItem != vit->m_vecEquipItem.end() ; ++vitItem ) {
                kItem.m_ItemID = vitItem->m_dwID;
                kItem.m_ItemUID = vitItem->m_dwUID;
                kItem.m_nInitCount = (int)vitItem->m_cItemType;
                kItem.m_nCount = (int)vitItem->m_cItemType;
                kItem.m_nPeriod = KItem::UNLIMITED_ITEM;
                vecItems_.push_back( kItem );
            }
        }

        START_LOG( clog, L"이벤트기간 채널링 펫 지급.. Date : " << prDate.first << L"/" << prDate.second << L" Pet size : " << nPetCount ) << END_LOG;
        return;
    }

    // 이벤트 기간이 아님. 일반 지급 확인
    {
        int nPetCount = 0;
        std::vector<KPetInfo>::const_iterator vit;
        for ( vit = kReward_.m_vecDefaultPet.begin() ; vit != kReward_.m_vecDefaultPet.end() ; ++vit ) {
            if ( false == m_mapPetInfo_.insert( std::make_pair( vit->m_dwUID, *vit ) ).second ) {
                START_LOG( cwarn, L"채널링 기본 펫 입력시 중복 데이터. PetItemID : " << vit->m_dwID ) << END_LOG;
                continue;
            }

            // PetItem UserInven에 넣어주기.
            kItem.m_ItemID = vit->m_dwID;
            kItem.m_ItemUID = vit->m_dwUID;
            vecItems_.push_back( kItem );
            ++nPetCount;

            // Pet 장착 정보 UserInven에 넣어주기.
            std::vector<KSimpleItem>::const_iterator vitItem;
            for ( vitItem = vit->m_vecEquipItem.begin() ; vitItem != vit->m_vecEquipItem.end() ; ++vitItem ) {
                kItem.m_ItemID = vitItem->m_dwID;
                kItem.m_ItemUID = vitItem->m_dwUID;
                kItem.m_nInitCount = (int)vitItem->m_cItemType;
                kItem.m_nCount = (int)vitItem->m_cItemType;
                kItem.m_nPeriod = KItem::UNLIMITED_ITEM;
                vecItems_.push_back( kItem );
            }
        }

        START_LOG( clog, L"기본 채널링 펫 지급.. Date : " << prDate.first << L"/" << prDate.second << L" Pet size : " << nPetCount ) << END_LOG;
    }
}