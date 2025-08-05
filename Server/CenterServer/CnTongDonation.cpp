#include "CenterSimLayer.h"
#include "CnTongDonation.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include <boost/bind.hpp>

ImplementSingleton( KCnTongDonation );
ImplOstmOperatorW2A( KCnTongDonation );
NiImplementRootRTTI( KCnTongDonation );

ImplToStringW( KCnTongDonation )
{
    KLocker lock( m_csCnTongDonation );

    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmVersion ) )
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_dwClientUIType )
        << TOSTRINGW( m_mapNPCs.size() );
}

KCnTongDonation::KCnTongDonation( void )
: m_tmVersion( 0 )
, m_tmBegin( 0 )
, m_tmEnd( 0 )
, m_dwClientUIType( 0 )
{
    m_mapNPCs.clear();
}

KCnTongDonation::~KCnTongDonation( void )
{
}

bool KCnTongDonation::LoadScript()
{
    KLuaManager kLuaMng;
    time_t tmVersion = 0;
    time_t tmBegin = 0;
    time_t tmEnd = 0;
    DWORD dwClientUIType = 0;
    std::map< DWORD, KTongDonationNPCInfo > mapNPCs;

    _JIF( kLuaMng.DoFile( "CnTongDonation.lua" ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("VersionTime"), tmVersion ), return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    _JIF( kLuaMng.GetValue( "ClientUIType", dwClientUIType ) == S_OK, return false );

    _JIF( LoadNPCInfo( kLuaMng, std::string( "NPCInfo" ), mapNPCs ), return false );

    {
        KLocker lock( m_csCnTongDonation );

        m_tmVersion         = tmVersion;
        m_tmBegin           = tmBegin;
        m_tmEnd             = tmEnd;
        m_dwClientUIType    = dwClientUIType;
        m_mapNPCs.swap( mapNPCs );
    }

    START_LOG( cerr, L"센터 통합 기부 스크립트 읽기 완료. " ) << END_LOG;

    {
        // 스크립트 로드시 통합 기부 이벤트 정보 전달
        KECN_TONG_DONATION_INFO_NOT kPacket;
        SiKCnTongDonation()->GetTongDonationInfoNot( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_TONG_DONATION_INFO_NOT, kPacket, true );
        START_LOG( cerr, L"스크립트 로드시 게임 서버들에 통합 기부 이벤트 정보 전송." ) << END_LOG;
    }

    return true;
}

bool KCnTongDonation::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"통합 기부 이벤트 스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );

    return true;
}

bool KCnTongDonation::LoadNPCInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KTongDonationNPCInfo >& mapNPCs_ )
{
    mapNPCs_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KTongDonationNPCInfo kTongDonationNPCInfo;
        _JIF( kLuaMng_.GetValue( "NPCType", kTongDonationNPCInfo.m_dwNPCType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DonationItemID", kTongDonationNPCInfo.m_dwDonationItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DonationUnit", kTongDonationNPCInfo.m_dwDonationUnit ) == S_OK, return false );
        _JIF( LoadDonationReward( kLuaMng_, std::string( "DonationReward" ), kTongDonationNPCInfo.m_vecDonationRewards ), return false );
        _JIF( LoadGoalInfo( kLuaMng_, std::string( "GoalInfo" ), kTongDonationNPCInfo.m_vecGoalInfo ), return false );

        if ( false == mapNPCs_.insert( std::make_pair( kTongDonationNPCInfo.m_dwNPCType, kTongDonationNPCInfo ) ).second ) {
            START_LOG( cerr, L"통합 기부 이벤트 스크립트에 중복된 NPC 정보가 있습니다.: " << kTongDonationNPCInfo.m_dwNPCType ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCnTongDonation::LoadDonationReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KItem >& vecDonationRewards_ )
{
    vecDonationRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    KItem kItem;
    _JIF( kLuaMng_.GetValue( 1, kItem.m_ItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, kItem.m_nCount ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kItem.m_nPeriod ) == S_OK, return false );

    vecDonationRewards_.push_back( kItem );

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCnTongDonation::LoadGoalInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KTongDonationGoalInfo >& vecGoalInfo_ )
{
    vecGoalInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KTongDonationGoalInfo kTongDonationGoalInfo;
        _JIF( kLuaMng_.GetValue( "Goal", kTongDonationGoalInfo.m_dwGoalCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Str", kTongDonationGoalInfo.m_dwStrNumber ) == S_OK, return false );

        vecGoalInfo_.push_back( kTongDonationGoalInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    std::sort( vecGoalInfo_.begin(), vecGoalInfo_.end(),
        boost::bind( &KTongDonationGoalInfo::m_dwGoalCount, _1 ) < boost::bind( &KTongDonationGoalInfo::m_dwGoalCount, _2 ) );

    return true;
}

bool KCnTongDonation::GetTongDonationInfoNot( OUT KECN_TONG_DONATION_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csCnTongDonation );

    kPacket_.m_tmVersion        = m_tmVersion;
    kPacket_.m_tmBegin          = m_tmBegin;
    kPacket_.m_tmEnd            = m_tmEnd;
    kPacket_.m_dwClientUIType   = m_dwClientUIType;
    kPacket_.m_mapNPCs          = m_mapNPCs;

    return true;
}
