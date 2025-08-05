#include "Survey.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "CenterPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "GSDBLayer.h"
#include <boost/bind.hpp>

//FILE_NAME_FOR_LOG

ImplementSingleton( KSurvey );
ImplOstmOperatorW2A( KSurvey );
NiImplementRootRTTI( KSurvey );

KSurvey::KSurvey( void )
:m_nVersion( 0 )
,m_tmBegin( 1 )
,m_tmEnd( 0 )
,m_tmReturnTime(0)
{
    m_mapCompressedSurveyInfoPack.clear();
    m_mapSurveyTypeInfo.clear();
    m_mapSurveyRewardInfo.clear();
}

KSurvey::~KSurvey( void )
{
}

ImplToStringW( KSurvey )
{
    KLocker lock(m_csSurvey);

    return START_TOSTRINGW
        << TOSTRINGWb( IsRun() )
        << TOSTRINGW( m_nVersion )
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGWt( CTime( m_tmReturnTime ) )
        << TOSTRINGW( m_mapCompressedSurveyInfoPack.size() )
        << TOSTRINGW( m_mapSurveyTypeInfo.size() )
        << TOSTRINGW( m_mapSurveyRewardInfo.size() );
}

void KSurvey::UpdateSurveyData( IN const KECN_SURVEY_DATA_NOT& kCenterPacket_ )
{
    KLocker lock( m_csSurvey );

    m_nVersion = kCenterPacket_.m_nVersion;
    m_tmBegin = kCenterPacket_.m_tmBegin;
    m_tmEnd = kCenterPacket_.m_tmEnd;
    m_tmReturnTime = kCenterPacket_.m_tmReturnTime;
    m_mapCompressedSurveyInfoPack = kCenterPacket_.m_mapCompressedSurveyInfoPack;
    m_mapSurveyTypeInfo = kCenterPacket_.m_mapSurveyTypeInfo;
    m_mapSurveyRewardInfo = kCenterPacket_.m_mapSurveyRewardInfo;
}

bool KSurvey::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csSurvey );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KSurvey::IsNewUser( IN const time_t& tmFirstLogin_ )
{
    KLocker lock( m_csSurvey );
    bool bResult = ( m_tmBegin <= tmFirstLogin_ );

    START_LOG( clog, L"IsNewUser : " << bResult )
        << BUILD_LOGtm( CTime(tmFirstLogin_) )
        << BUILD_LOGtm( CTime(m_tmBegin) ) << END_LOG;
    return bResult;
}

bool KSurvey::IsReturnUser( IN const time_t& tmLastLogin_ )
{
    KLocker lock( m_csSurvey );
    bool bResult = ( tmLastLogin_ < m_tmReturnTime );

    START_LOG( clog, L"IsReturnUser : " << bResult )
        << BUILD_LOGtm( CTime( tmLastLogin_ ) )
        << BUILD_LOGtm( CTime( m_tmReturnTime ) ) << END_LOG;

    return bResult;
}

bool KSurvey::GetSurveyList( IN const int& nLanguageCode_, 
                                            IN const int& nType_, 
                                                IN const int& nDataFirst_,
                                                    IN const int& nDataSecond_,
                                                        OUT std::map< int, KSerBuffer >& mapData_ )
{
    mapData_.clear();
    std::map< int, std::map< std::pair<int,int>, std::vector<int> > >::iterator mitTotal;
    std::map< std::pair<int,int>, std::vector<int> >::iterator mitData;
    std::map< int, std::map< int, KSerBuffer > >::iterator mitTotalSurvey;
    std::map< int, KSerBuffer >::iterator mitSurvey;
    std::vector<int>::iterator vit;
    std::pair<int,int> prData;
    prData.first = nDataFirst_;
    prData.second = nDataSecond_;

    KLocker lock( m_csSurvey );
    mitTotalSurvey = m_mapCompressedSurveyInfoPack.find( nLanguageCode_ );
    if( mitTotalSurvey == m_mapCompressedSurveyInfoPack.end() ) {
        START_LOG( clog, L"Not exist LanguageCode : " << nLanguageCode_ ) << END_LOG;
        return false;
    }

    mitTotal = m_mapSurveyTypeInfo.find( nType_ );
    if( mitTotal == m_mapSurveyTypeInfo.end() ) {
        START_LOG( clog, L"Not exist Type : " << nType_ ) << END_LOG;
        return false;
    }

    mitData = mitTotal->second.find( prData );
    if( mitData == mitTotal->second.end() ) {
        START_LOG( clog, L"Not exist Data : " << prData.first << L", " << prData.second ) << END_LOG;
        return false;
    }

    for( vit = mitData->second.begin(); vit != mitData->second.end(); ++vit ) {
        mitSurvey = mitTotalSurvey->second.find( *vit );
        if( mitSurvey == mitTotalSurvey->second.end() ) {
            START_LOG( clog, L"Not exist Survey : " << *vit ) << END_LOG;
            continue;
        }

        if ( !mapData_.insert( std::make_pair( mitSurvey->first, mitSurvey->second ) ).second ) {
            START_LOG( cerr, L"전달해야 문제중 중복 UID 있음 UID : " << mitSurvey->first ) << END_LOG;
        }
    }

    START_LOG( clog, L"GetSurveyList.. size : " << mapData_.size() )
        << BUILD_LOG( nLanguageCode_ )
        << BUILD_LOG( nType_ )
        << BUILD_LOG( nDataFirst_ )
        << BUILD_LOG( nDataSecond_ ) << END_LOG;
    return true;
}

bool KSurvey::GetTypeRewardList( IN const int& nType_, OUT std::vector< KDropItemInfo >& vecReward_ )
{
    vecReward_.clear();
    std::map< int, std::vector< KDropItemInfo > >::iterator mit;

    KLocker lock( m_csSurvey );
    mit = m_mapSurveyRewardInfo.find( nType_ );
    if( mit ==m_mapSurveyRewardInfo.end() ) {
        return false;
    }
    vecReward_ = mit->second;
    return true;
}

void KSurvey::GetRewardList( OUT std::map< int, std::vector< KDropItemInfo > >& mapSurveyRewardInfo_ )
{
    mapSurveyRewardInfo_.clear();

    KLocker lock( m_csSurvey );
    mapSurveyRewardInfo_ = m_mapSurveyRewardInfo;
}

bool KSurvey::CheckValidSurvey( IN const int& nLanguageCode_, IN std::map< int, std::vector<int> >& mapNumberAnswer_, IN std::map< int, std::wstring >& mapStringAnswer_ )
{
    std::map< int, std::map< int, KSerBuffer > >::iterator mitTotalSurvey;
    std::map< int, KSerBuffer >::iterator mitSurvey;
    std::map< int, std::vector<int> >::iterator mitNumber;
    std::map< int, std::wstring >::iterator mitString;

    KLocker lock( m_csSurvey );
    mitTotalSurvey = m_mapCompressedSurveyInfoPack.find( nLanguageCode_ );
    if( mitTotalSurvey == m_mapCompressedSurveyInfoPack.end() ) {
        START_LOG( cerr, L"잘못된 언어 LanguageID : " << nLanguageCode_ ) << END_LOG;
        return false;
    }

    for( mitNumber = mapNumberAnswer_.begin(); mitNumber != mapNumberAnswer_.end(); ++mitNumber ) {
        mitSurvey = mitTotalSurvey->second.find( mitNumber->first );
        if( mitSurvey == mitTotalSurvey->second.end() ) {
            START_LOG( cerr, L"잘못된 설문 UID : " << mitNumber->first ) << END_LOG;
            return false;
        }
    }

    for( mitString = mapStringAnswer_.begin(); mitString != mapStringAnswer_.end(); ++mitString ) {
        mitSurvey = mitTotalSurvey->second.find( mitString->first );
        if( mitSurvey == mitTotalSurvey->second.end() ) {
            START_LOG( cerr, L"잘못된 설문 UID : " << mitString->first ) << END_LOG;
            return false;
        }
    }

    return true;
}

//=========================================================================================//
KSurveyUserData::KSurveyUserData()
{
    m_mapUserSurveyInfo.clear();
}

KSurveyUserData::~KSurveyUserData()
{

}

void KSurveyUserData::SetUserSurveyData( IN const std::map< int, std::set< std::pair<int,int> > >& mapUserSurveyInfo_ )
{
    m_mapUserSurveyInfo = mapUserSurveyInfo_;
}

bool KSurveyUserData::CheckSurveyStatus( IN const int& nType_, IN const int& nDataFirst_, IN const int& nDataSecond_ )
{
    std::map< int, std::set< std::pair<int,int> > >::iterator mit;
    std::set< std::pair<int,int> >::iterator sit;
    std::set< std::pair<int,int> > setData;
    std::pair<int,int> prData;
    prData.first = nDataFirst_;
    prData.second = nDataSecond_;
    setData.insert( prData );

    mit = m_mapUserSurveyInfo.find( nType_ );
    if( mit == m_mapUserSurveyInfo.end() ) {
        return true;
    }

    if( nType_ == KSurvey::ST_GAME_CONNECT ) {
        return false;
    }

    sit = mit->second.find( prData );
    if( sit == mit->second.end() ) {
        return true;
    }

    return false;
}

bool KSurveyUserData::UpdateSurveyStatus( IN const int& nType_, IN const int& nDataFirst_, IN const int& nDataSecond_ )
{
    std::map< int, std::set< std::pair<int,int> > >::iterator mit;
    std::set< std::pair<int,int> >::iterator sit;
    std::set< std::pair<int,int> > setData;
    std::pair<int,int> prData;
    prData.first = nDataFirst_;
    prData.second = nDataSecond_;
    setData.insert( prData );

    mit = m_mapUserSurveyInfo.find( nType_ );
    if( mit == m_mapUserSurveyInfo.end() ) {
        m_mapUserSurveyInfo[nType_] = setData;
        return true;
    }

    if( nType_ == KSurvey::ST_GAME_CONNECT ) {
        return false;
    }

    sit = mit->second.find( prData );
    if( sit == mit->second.end() ) {
        mit->second.insert( prData );
        return true;
    }

    return false;
}