//#include "dbg/dbg.hpp"
#include "user.h" // winsock 땜시롱..
#include "PeriodNotice.h"
#include "UserEvent.h"
#include "GSSimLayer.h"
#include "GCHelper.h"
#include "Log4.h"
#include "SignBoard.h"
#include "ChannelManager.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KPeriodNotice );

KPeriodNotice::KPeriodNotice(void)
:m_nMinimumPeriod(1)
{
}

KPeriodNotice::~KPeriodNotice(void)
{
}

bool KPeriodNotice::CompareNoticeUID( IN const std::vector<int>& vecNotice_ )
{
    // DB에서 가져온 공지의 ID 가 서버에 없는 새 매세지인지 알아내기
    if( m_vecPeriodNoticeID != vecNotice_ ) {
        m_vecPeriodNoticeID = vecNotice_;
        return true;
    }
    MakeNoticePacket();
    return false;
}

bool KPeriodNotice::GetNoticePacket( OUT std::vector<int>& vecNotice_, OUT KSerBuffer& kbuff_ )
{
    return false;
}

// 게임서버에서 전체공지(DB)에 변화가 있을 때 호출하는 함수
void KPeriodNotice::UpdatePeriodNotice( IN const std::map<int, KPeriodNoticeInfo>& mapNoticeInfo_ )
{
    { // Lock 깊이 최소화.
        KLocker lock( m_csPeriodNotice );
        m_mapNoticeInfo = mapNoticeInfo_;
    }

    SetNoticeCntInfo(); // 공지 세팅.

    MakeNoticePacket(); // 패킷을 미리 만들어 놓음

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
        DumpNotice();
}

void KPeriodNotice::MakeNoticePacket()
{
    std::map<int, KPeriodNoticeInfo>::iterator mitNotice;
    std::map<int, int>::iterator mitCount;

    KLocker lock( m_csPeriodNotice );

    for( mitCount = m_mapNoticeCnt.begin(); mitCount != m_mapNoticeCnt.end(); ++mitCount ) {
        if( mitCount->second == 0 ) {
            mitNotice = m_mapNoticeInfo.find( mitCount->first );
            if( mitNotice != m_mapNoticeInfo.end() ) {
                SendAdminData( mitNotice->second.m_wstrNotice );
                mitCount->second = mitNotice->second.m_nPeriod;
                continue;
            }
        }
        --mitCount->second;
    }
}

void KPeriodNotice::DumpNotice()
{
    std::map<int, KPeriodNoticeInfo>::iterator mit;

    START_LOG( cwarn, L"[Period Notice Dump(size:" << m_mapNoticeInfo.size() << L")"  ) << END_LOG;
    for( mit = m_mapNoticeInfo.begin(); mit != m_mapNoticeInfo.end(); ++mit ) {
        START_LOG( cwarn, L"Interval : " << mit->first << L", Msg : " << mit->second.m_wstrNotice ) << END_LOG;
    }
}

void KPeriodNotice::SetNoticeCntInfo()
{
    std::map<int, KPeriodNoticeInfo>::iterator mit;
    KLocker lock( m_csPeriodNotice );

    m_mapNoticeCnt.clear();

    for( mit = m_mapNoticeInfo.begin(); mit != m_mapNoticeInfo.end(); ++mit ) {
        m_mapNoticeCnt[mit->first] = mit->second.m_nPeriod;
    }
}

void KPeriodNotice::SendAdminData( IN const std::wstring& wstrMsg_ )
{
    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID          = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;
    kData.m_strMsg          = wstrMsg_;
    kData.m_strSenderNick.clear();
    SiKSignBoard()->QueueingAdminData( kData );
}

void KPeriodNotice::ClearPeriodNotice()
{
    { // Lock 깊이 최소화.
        KLocker lock( m_csPeriodNotice );
        m_mapNoticeInfo.clear();
    }
}