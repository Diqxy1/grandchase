#include "ConnectionGiftBox.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include <boost/bind.hpp>
#include "EventType.h"
#include <boost/foreach.hpp>

ImplementSingleton( KConnectionGiftBox );
ImplOstmOperatorW2A( KConnectionGiftBox );
NiImplementRootRTTI( KConnectionGiftBox );

ImplToStringW( KConnectionGiftBox )
{
    KLocker lock( m_csConnectionGiftBox );

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

KConnectionGiftBox::KConnectionGiftBox( void )
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

KConnectionGiftBox::~KConnectionGiftBox( void )
{
}

bool KConnectionGiftBox::SetFromConnectionGiftBoxInfoNot( IN const KECN_CONNECTION_GIFTBOX_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csConnectionGiftBox );

    m_bUseGiftFirstCharacter        = kPacket_.m_bUseGiftFirstCharacter;
    m_mapGiftFirstCharacter         = kPacket_.m_mapGiftFirstCharacter;
    m_bUseGiftNewUser               = kPacket_.m_bUseGiftNewUser;
    m_tmDecideBeginTimeNewUser      = kPacket_.m_tmDecideBeginTimeNewUser;
    m_nNewUserEffectDuration        = kPacket_.m_nNewUserEffectDuration;
    m_mapGiftNewUser                = kPacket_.m_mapGiftNewUser;
    m_nGiftEventUserVersion         = kPacket_.m_nGiftEventUserVersion;
    m_tmGiftEventBegin              = kPacket_.m_tmGiftEventBegin;
    m_tmGiftEventEnd                = kPacket_.m_tmGiftEventEnd;
    m_mapGiftEventUser              = kPacket_.m_mapGiftEventUser;

    return true;
}

bool KConnectionGiftBox::IsRunGiftFirstCharacter()
{
    KLocker lock( m_csConnectionGiftBox );

    return m_bUseGiftFirstCharacter;
}

bool KConnectionGiftBox::GetGiftFirstCharacter( IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecGiftFirstCharacter_ )
{
    vecGiftFirstCharacter_.clear();

    KLocker lock( m_csConnectionGiftBox );

    std::map< int, KCharGiftItem >::iterator mitGiftFirstCharacter;
    mitGiftFirstCharacter = m_mapGiftFirstCharacter.find( nCharType_ );
    if ( mitGiftFirstCharacter == m_mapGiftFirstCharacter.end() ) {
        return false;
    }

    vecGiftFirstCharacter_ = mitGiftFirstCharacter->second.m_vecItems;

    return true;
}

bool KConnectionGiftBox::IsGiftEventUserTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csConnectionGiftBox );

    return ( m_tmGiftEventBegin <= tmCurrent && tmCurrent <= m_tmGiftEventEnd );
}

int KConnectionGiftBox::GetGiftEventUserVersion()
{
    KLocker lock( m_csConnectionGiftBox );

    return m_nGiftEventUserVersion;
}

bool KConnectionGiftBox::GetNewUserGiftGradeInfo( IN const int nNewUserCurrentGrade_, OUT KGiftGradeInfo& kGiftGradeInfo_ )
{
    KLocker lock( m_csConnectionGiftBox );

    std::map< int, KGiftGradeInfo >::iterator mitGiftNewUser;
    mitGiftNewUser = m_mapGiftNewUser.find( nNewUserCurrentGrade_ );
    if ( mitGiftNewUser == m_mapGiftNewUser.end() ) {
        return false;
    }

    kGiftGradeInfo_ = mitGiftNewUser->second;

    return true;
}

bool KConnectionGiftBox::GetEventUserGiftGradeInfo( IN const int nEventUserCurrentGrade_, OUT KGiftGradeInfo& kGiftGradeInfo_ )
{
    KLocker lock( m_csConnectionGiftBox );

    std::map< int, KGiftGradeInfo >::iterator mitGiftEventUser;
    mitGiftEventUser = m_mapGiftEventUser.find( nEventUserCurrentGrade_ );
    if ( mitGiftEventUser == m_mapGiftEventUser.end() ) {
        return false;
    }

    kGiftGradeInfo_ = mitGiftEventUser->second;

    return true;
}

bool KConnectionGiftBox::GetNewUserNextGift( IN const int nCharType_, IN const int nCurrentGrade_, OUT std::vector< std::pair< int, std::vector< KDropItemInfo > > >& vecNextGift_ )
{
    vecNextGift_.clear();

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csConnectionGiftBox );

    // 신규유저의 다음 보상이 있는 경우 한개의 보상 정보를 전달한다
    std::map< int, KGiftGradeInfo >::iterator mitGiftNewUser;
    mitGiftNewUser = m_mapGiftNewUser.find( nCurrentGrade_ );
    if ( mitGiftNewUser == m_mapGiftNewUser.end() ) {
        return false;
    }

    int& nCountTime = mitGiftNewUser->second.m_nCountTime; // 카운트 할 시간(분)

    std::map< int, KCharGiftItem >::iterator mitCharGiftItem;
    mitCharGiftItem = mitGiftNewUser->second.m_mapCharGiftItem.find( nCharType_ );
    if ( mitCharGiftItem == mitGiftNewUser->second.m_mapCharGiftItem.end() ) {
        return false;
    }

    vecNextGift_.push_back( std::make_pair( nCountTime, mitCharGiftItem->second.m_vecItems ) );

    return true;
}

bool KConnectionGiftBox::GetEventUserNextGift( IN const int nCharType_, IN const int nCurrentGrade_, OUT std::vector< std::pair< int, std::vector< KDropItemInfo > > >& vecNextGift_ )
{
    bool bResult = false;

    vecNextGift_.clear();

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csConnectionGiftBox );

    // 이벤트유저의 다음 보상이 있는 경우 최대 두개의 보상 정보를 전달한다
    for ( int i = 0; i < 2; ++i ) {
        std::map< int, KGiftGradeInfo >::iterator mitGiftEventUser;
        mitGiftEventUser = m_mapGiftEventUser.find( nCurrentGrade_ + i );
        if ( mitGiftEventUser == m_mapGiftEventUser.end() ) {
            break;
        }

        int& nCountTime = mitGiftEventUser->second.m_nCountTime; // 카운트 할 시간(분)
        if ( m_tmGiftEventEnd < tmCurrent + nCountTime ) { // 목표 시간이 이벤트 기간 내에 있는지 확인
            break;
        }

        std::map< int, KCharGiftItem >::iterator mitCharGiftItem;
        mitCharGiftItem = mitGiftEventUser->second.m_mapCharGiftItem.find( nCharType_ );
        if ( mitCharGiftItem == mitGiftEventUser->second.m_mapCharGiftItem.end() ) {
            break;
        }

        vecNextGift_.push_back( std::make_pair( nCountTime, mitCharGiftItem->second.m_vecItems ) );

        bResult = true;
    }

    return bResult;
}

bool KConnectionGiftBox::GetNewUserCurrentGradeRewards( IN const int nNewUserCurrentGrade_, IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecRewards_ )
{
    vecRewards_.clear();

    KLocker lock( m_csConnectionGiftBox );

    std::map< int, KGiftGradeInfo >::iterator mitGiftNewUser;
    mitGiftNewUser = m_mapGiftNewUser.find( nNewUserCurrentGrade_ );
    if ( mitGiftNewUser == m_mapGiftNewUser.end() ) {
        return false;
    }

    std::map< int, KCharGiftItem >::iterator mitCharGiftItem;
    mitCharGiftItem = mitGiftNewUser->second.m_mapCharGiftItem.find( nCharType_ );
    if ( mitCharGiftItem == mitGiftNewUser->second.m_mapCharGiftItem.end() ) {
        return false;
    }

    BOOST_FOREACH( KDropItemInfo& rDropItemInfo, mitCharGiftItem->second.m_vecItems ) {
        vecRewards_.push_back( rDropItemInfo );
    }

    return true;
}

bool KConnectionGiftBox::GetEventUserCurrentGradeRewards( IN const int nEventUserCurrentGrade_, IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecRewards_ )
{
    vecRewards_.clear();

    KLocker lock( m_csConnectionGiftBox );

    std::map< int, KGiftGradeInfo >::iterator mitGiftEventUser;
    mitGiftEventUser = m_mapGiftEventUser.find( nEventUserCurrentGrade_ );
    if ( mitGiftEventUser == m_mapGiftEventUser.end() ) {
        return false;
    }

    std::map< int, KCharGiftItem >::iterator mitCharGiftItem;
    mitCharGiftItem = mitGiftEventUser->second.m_mapCharGiftItem.find( nCharType_ );
    if ( mitCharGiftItem == mitGiftEventUser->second.m_mapCharGiftItem.end() ) {
        return false;
    }

    BOOST_FOREACH( KDropItemInfo& rDropItemInfo, mitCharGiftItem->second.m_vecItems ) {
        vecRewards_.push_back( rDropItemInfo );
    }

    return true;
}

// -------------------------------------------------------------------------------------------

KConnectionGiftBoxUserData::KConnectionGiftBoxUserData()
: m_bIsNewUser( false )
{
    m_mapNewUser.clear();
    m_mapEventUser.clear();
}

KConnectionGiftBoxUserData::~KConnectionGiftBoxUserData()
{
}

void KConnectionGiftBoxUserData::SetData( IN const int nCharType_, IN const KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ )
{
    m_bIsNewUser                                    = kConnectionGiftBoxUpdateData_.m_bIsNewUser;
    m_mapNewUser[ nCharType_ ].m_nLastGetGrade      = kConnectionGiftBoxUpdateData_.m_nNewUserLastGetGrade;
    m_mapNewUser[ nCharType_ ].m_nAccPlayTime       = kConnectionGiftBoxUpdateData_.m_nNewUserAccPlayTime;
    m_mapEventUser[ nCharType_ ].m_nLastGetGrade    = kConnectionGiftBoxUpdateData_.m_nEventUserLastGetGrade;
    m_mapEventUser[ nCharType_ ].m_nAccPlayTime     = kConnectionGiftBoxUpdateData_.m_nEventUserAccPlayTime;
}

int KConnectionGiftBoxUserData::GetNewUserRemainAccPlayTime( IN const int nCharType_ )
{
    int nNewUserRemainAccPlayTime = 0;

    KGiftGradeInfo kNewUserGiftGradeInfo;
    if ( true == SiKConnectionGiftBox()->GetNewUserGiftGradeInfo( GetNewUserCurrentGrade( nCharType_ ), kNewUserGiftGradeInfo ) ) {
        nNewUserRemainAccPlayTime = kNewUserGiftGradeInfo.m_nCountTime - m_mapNewUser[ nCharType_ ].m_nAccPlayTime;
    }

    nNewUserRemainAccPlayTime = std::max< int >( 0, nNewUserRemainAccPlayTime );

    return nNewUserRemainAccPlayTime;
}

int KConnectionGiftBoxUserData::GetEventUserRemainAccPlayTime( IN const int nCharType_ )
{
    int nEventUserRemainAccPlayTime = 0;

    KGiftGradeInfo kEventUserGiftGradeInfo;
    if ( true == SiKConnectionGiftBox()->GetEventUserGiftGradeInfo( GetEventUserCurrentGrade( nCharType_ ), kEventUserGiftGradeInfo ) ) {
        nEventUserRemainAccPlayTime = kEventUserGiftGradeInfo.m_nCountTime - m_mapEventUser[ nCharType_ ].m_nAccPlayTime;
    }

    nEventUserRemainAccPlayTime = std::max< int >( 0, nEventUserRemainAccPlayTime );

    return nEventUserRemainAccPlayTime;
}
