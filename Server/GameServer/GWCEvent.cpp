#include "GWCEvent.h"
#include <dbg/dbg.hpp >
#include "UserPacket.h"
#include "CenterPacket.h"
#include "UserEvent.h"
#include "SignBoard.h"
#include "LoudMessage.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KGWCEvent );
ImplOstmOperatorW2A( KGWCEvent );
NiImplementRootRTTI( KGWCEvent );

KGWCEvent::KGWCEvent(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_tmDBUpdateTime(1)
,m_dwCountryID(0)
,m_dwCurrentGrade(0)
,m_dwCurrentRanking(0)
{
    // Init.
    m_mapGradeRewardInfo.clear();
    m_mapRankingRewardInfo.clear();
    m_mapTotalCountryInfo.clear();
}

KGWCEvent::~KGWCEvent(void)
{
}

ImplToStringW( KGWCEvent )
{
    KLocker lock( m_csGWCEvent );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tmDBUpdateTime) )
        << TOSTRINGW( m_dwCountryID )
        << TOSTRINGW( m_dwCurrentGrade )
        << TOSTRINGW( m_dwCurrentRanking )
        << TOSTRINGW( m_mapGradeRewardInfo.size() )
        << TOSTRINGW( m_mapRankingRewardInfo.size() )
        << TOSTRINGW( m_mapTotalCountryInfo.size() );
}

void KGWCEvent::UpdateGWCEventData( IN const KECN_GWC_EVENT_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csGWCEvent );

    m_dwCountryID = kPacket_.m_dwCountryID;
    m_wstrCountryName = kPacket_.m_wstrCountryName;
    m_tmBegin = kPacket_.m_tmBegin;
    m_tmEnd = kPacket_.m_tmEnd;
    m_tmDBUpdateTime = kPacket_.m_tmDBUpdateTime;
    m_mapGradeRewardInfo = kPacket_.m_mapGradeRewardInfo;
    m_mapRankingRewardInfo = kPacket_.m_mapRankingRewardInfo;
    m_mapTotalCountryInfo = kPacket_.m_mapTotalCountryInfo;
    m_mapGradeDesc = kPacket_.m_mapGradeDesc;

    if( !UpdateCountryGrade( m_dwCountryID, m_mapGradeDesc, m_mapTotalCountryInfo ) ) {
        START_LOG( cerr, L"국가 등급 정보 가져오기 실패 CountryID : " << m_dwCountryID ) << END_LOG;
    }
}

bool KGWCEvent::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csGWCEvent );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KGWCEvent::GetTotalCountryInfo( OUT std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo_ )
{
    KLocker lock( m_csGWCEvent );
    mapTotalCountryInfo_ = m_mapTotalCountryInfo;
}

void KGWCEvent::GetRankingRewardInfo( OUT std::map<DWORD, VEC_ITEMS>& mapRankingRewardInfo_ )
{
    KLocker lock( m_csGWCEvent );
    mapRankingRewardInfo_ = m_mapRankingRewardInfo;
}

bool KGWCEvent::GetRankingReward( OUT VEC_ITEMS& vecRankingReward_ )
{
    std::map<DWORD, VEC_ITEMS>::iterator mit;
    KLocker lock( m_csGWCEvent );
    mit = m_mapRankingRewardInfo.find( m_dwCurrentRanking );
    if( mit == m_mapRankingRewardInfo.end() ) {
        return false;
    }
    vecRankingReward_ = mit->second;
    return true;
}

bool KGWCEvent::GetGradeReward( OUT float& fExpReward_, OUT float& GPReward_ )
{
    std::map<DWORD, std::pair<float,float>>::iterator mit;
    KLocker lock( m_csGWCEvent );
    mit = m_mapGradeRewardInfo.find( m_dwCurrentGrade );
    if( mit == m_mapGradeRewardInfo.end() ) {
        return false;
    }
    fExpReward_ = mit->second.first;
    GPReward_   = mit->second.second;
    return true;
}

bool KGWCEvent::UpdateCountryGrade( IN DWORD& dwCountryID_, IN std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc_, IN std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo_ )
{
    std::map<DWORD, KGWCCountryInfo>::iterator mitCountryInfo;
    DWORD dwCurrentGrade = 0;
    mitCountryInfo = mapTotalCountryInfo_.find( dwCountryID_ );
    if( mitCountryInfo == mapTotalCountryInfo_.end() ) {
        START_LOG( cerr, L"해당 국가정보의 데이터 없음 CountryID : " << dwCountryID_ ) << END_LOG;
        return false;
    }
    m_dwCurrentRanking = mitCountryInfo->second.m_dwRanking;
    m_dwCurrentGrade = mitCountryInfo->second.m_dwGrade;

    return true;
}

bool KGWCEvent::GetCurrentGrade( IN DWORD dwCountryCnt_, IN std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc_, OUT DWORD& dwGrade_ )
{
    std::map<DWORD,std::pair<DWORD,DWORD>>::iterator mit;
    for( mit = mapGradeDesc_.begin(); mit != mapGradeDesc_.end(); ++mit ) {
        if( mit->second.first <= dwCountryCnt_ && mit->second.second >= dwCountryCnt_ ) {
            dwGrade_ = mit->first;
            return true;
        } // if
    } // for
    return false;
}

void KGWCEvent::SetNoticeInfo( IN DWORD dwCountryNum_ )
{
    std::wstring wstrData;

    KLocker lock( m_csGWCEvent );
    if( !SiKLoudMessage()->GetGWCEventMsg( m_wstrCountryName, m_dwCurrentGrade, wstrData ) ) {
        START_LOG( cerr, L"공지 내용 가져오기 실패 국가이름 : " << m_wstrCountryName << L",등급 : " << m_dwCurrentGrade ) << END_LOG;
        return;
    }

    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID          = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
    kData.m_strSenderNick.clear();
    kData.m_strMsg = wstrData;
    SiKSignBoard()->QueueingAdminData( kData );
}