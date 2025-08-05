#include "CenterSimLayer.h"
#include "CnConnectionGiftBox.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include "CommonPacket.h"
#include <boost/bind.hpp>

ImplementSingleton( KCnConnectionGiftBox );
ImplOstmOperatorW2A( KCnConnectionGiftBox );
NiImplementRootRTTI( KCnConnectionGiftBox );

ImplToStringW( KCnConnectionGiftBox )
{
    KLocker lock( m_csCnConnectionGiftBox );

    return START_TOSTRINGW
        << TOSTRINGW( m_bUseGiftFirstCharacter )
        << TOSTRINGW( m_bUseGiftNewUser )
        << TOSTRINGW( m_bUseGiftFirstCharacter )
        << TOSTRINGWt( CTime( m_tmDecideBeginTimeNewUser ) )
        << TOSTRINGW( m_nNewUserEffectDuration )
        << TOSTRINGW( m_nGiftEventUserVersion )
        << TOSTRINGWt( CTime( m_tmGiftEventBegin ) )
        << TOSTRINGWt( CTime( m_tmGiftEventEnd ) )
        << TOSTRINGW( m_mapGiftFirstCharacter.size() )
        << TOSTRINGW( m_mapGiftNewUser.size() )
        << TOSTRINGW( m_mapGiftEventUser.size() );
}

KCnConnectionGiftBox::KCnConnectionGiftBox( void )
: m_bUseGiftFirstCharacter( false )
, m_bUseGiftNewUser( false )
, m_tmDecideBeginTimeNewUser( 0 )
, m_nNewUserEffectDuration( 0 )
, m_nGiftEventUserVersion( 1 )
, m_tmGiftEventBegin( 0 )
, m_tmGiftEventEnd( 0 )
{
    m_mapGiftFirstCharacter.clear();
    m_mapGiftNewUser.clear();
    m_mapGiftEventUser.clear();
}

KCnConnectionGiftBox::~KCnConnectionGiftBox( void )
{
}

bool KCnConnectionGiftBox::LoadScript()
{
    KLuaManager kLuaMng;
    bool                                        bUseGiftFirstCharacter = false;
    std::map< int, KCharGiftItem >              mapGiftFirstCharacter;

    bool                                        bUseGiftNewUser = false;
    time_t                                      tmDecideBeginTimeNewUser = 0;
    int                                         nNewUserEffectDuration = 0;
    std::map< int, KGiftGradeInfo >             mapGiftNewUser;

    int                                         nGiftEventUserVersion = 1;
    time_t                                      tmGiftEventBegin = 0;
    time_t                                      tmGiftEventEnd = 0;
    std::map< int, KGiftGradeInfo >             mapGiftEventUser;

    _JIF( kLuaMng.DoFile( "InitCnConnectionGiftBox.lua" ) == S_OK, return false );

    // 시스템 1(첫 캐릭터 생성)
    _JIF( kLuaMng.GetValue( "UseGiftFirstCharacter", bUseGiftFirstCharacter ) == S_OK, return false );
    _JIF( LoadGiftFirstCharacterInfo( kLuaMng, std::string( "GiftFirstCharacter" ), mapGiftFirstCharacter ), return false );

    // 시스템 2(신규유저)
    _JIF( kLuaMng.GetValue( "UseGiftNewUser", bUseGiftNewUser ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("DecideBeginTimeNewUser"), tmDecideBeginTimeNewUser ), return false );
    _JIF( kLuaMng.GetValue( "NewUserEffectDuration", nNewUserEffectDuration ) == S_OK, return false );
    _JIF( LoadGiftUserInfo( kLuaMng, std::string( "GiftNewUser" ), mapGiftNewUser ), return false );

    // 시스템 3(이벤트유저)
    _JIF( kLuaMng.GetValue( "Version", nGiftEventUserVersion ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("EventBeginTime"), tmGiftEventBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EventEndTime"), tmGiftEventEnd ), return false );
    _JIF( LoadGiftUserInfo( kLuaMng, std::string( "GiftEventUser" ), mapGiftEventUser ), return false );

    {
        KLocker lock( m_csCnConnectionGiftBox );

        m_bUseGiftFirstCharacter    = bUseGiftFirstCharacter;
        m_mapGiftFirstCharacter     = mapGiftFirstCharacter;

        m_bUseGiftNewUser           = bUseGiftNewUser;
        m_tmDecideBeginTimeNewUser  = tmDecideBeginTimeNewUser;
        m_nNewUserEffectDuration    = nNewUserEffectDuration;
        m_kNewUserEffectDuration    = CTimeSpan( nNewUserEffectDuration, 0, 0, 0 );
        m_mapGiftNewUser            = mapGiftNewUser;

        m_nGiftEventUserVersion     = nGiftEventUserVersion;
        m_tmGiftEventBegin          = tmGiftEventBegin;
        m_tmGiftEventEnd            = tmGiftEventEnd;
        m_mapGiftEventUser          = mapGiftEventUser;
    }
    START_LOG( cerr, L"센터 접속 선물상자 스크립트 읽기 완료. " ) << END_LOG;

    {
        // 스크립트 로드시 접속 선물상자 정보 전달
        KECN_CONNECTION_GIFTBOX_INFO_NOT kPacket;
        SiKCnConnectionGiftBox()->GetConnectionGiftBoxInfoNot( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CONNECTION_GIFTBOX_INFO_NOT, kPacket, true );
        START_LOG( cerr, L"스크립트 로드시 게임 서버들에 접속 선물상자 정보 전송." ) << END_LOG;
    }

    return true;
}

bool KCnConnectionGiftBox::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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
            START_LOG( cerr, L"스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );

    return true;
}

bool KCnConnectionGiftBox::LoadGiftFirstCharacterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KCharGiftItem >& mapGiftFirstCharacter_ )
{
    _JIF( LoadCharGiftItemInfo( kLuaMng_, strTable_, mapGiftFirstCharacter_ ), return false );

    return true;
}

bool KCnConnectionGiftBox::LoadCharGiftItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KCharGiftItem >& mapCharGiftItem_ )
{
    mapCharGiftItem_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KCharGiftItem kCharGiftItem;
        KDropItemInfo kDropItemInfo;
        _JIF( kLuaMng_.GetValue( "CharType", kCharGiftItem.m_nCharType ) == S_OK, return false );

        _JIF( LoadGiftItemInfo( kLuaMng_, std::string( "GiftItem" ), kCharGiftItem.m_vecItems ), return false );

        if ( false == mapCharGiftItem_.insert( std::make_pair( kCharGiftItem.m_nCharType, kCharGiftItem ) ).second ) {
            START_LOG( cerr, L"접속 선물상자 스크립트에 중복된 캐릭터 정보가 있습니다.: " << kCharGiftItem.m_nCharType ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCnConnectionGiftBox::LoadGiftItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecItems_ )
{
    vecItems_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecItems_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnConnectionGiftBox::LoadGiftUserInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KGiftGradeInfo >& mapGiftNewUser_ )
{
    mapGiftNewUser_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KGiftGradeInfo kGiftGradeInfo;
        kGiftGradeInfo.m_nGrade = i;
        _JIF( kLuaMng_.GetValue( "GoalTime", kGiftGradeInfo.m_nCountTime ) == S_OK, return false );

        _JIF( LoadCharGiftItemInfo( kLuaMng_, std::string( "CharacterGiftItem" ), kGiftGradeInfo.m_mapCharGiftItem ), return false );

        if ( false == mapGiftNewUser_.insert( std::make_pair( kGiftGradeInfo.m_nGrade, kGiftGradeInfo ) ).second ) {
            START_LOG( cerr, L"접속 선물상자 스크립트에 중복된 등급 정보가 있습니다.: " << kGiftGradeInfo.m_nGrade ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCnConnectionGiftBox::GetConnectionGiftBoxInfoNot( OUT KECN_CONNECTION_GIFTBOX_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csCnConnectionGiftBox );

    kPacket_.m_bUseGiftFirstCharacter       = m_bUseGiftFirstCharacter;
    kPacket_.m_mapGiftFirstCharacter        = m_mapGiftFirstCharacter;
    kPacket_.m_bUseGiftNewUser              = m_bUseGiftNewUser;
    kPacket_.m_tmDecideBeginTimeNewUser     = m_tmDecideBeginTimeNewUser;
    kPacket_.m_nNewUserEffectDuration       = m_nNewUserEffectDuration;
    kPacket_.m_mapGiftNewUser               = m_mapGiftNewUser;
    kPacket_.m_nGiftEventUserVersion        = m_nGiftEventUserVersion;
    kPacket_.m_tmGiftEventBegin             = m_tmGiftEventBegin;
    kPacket_.m_tmGiftEventEnd               = m_tmGiftEventEnd;
    kPacket_.m_mapGiftEventUser             = m_mapGiftEventUser;

    return true;
}
