#include "GSSimLayer.h"
#include "MissionEvent.h"
#include <dbg/dbg.hpp >
#include "CommonPacket.h"
#include "CenterPacket.h"
#include "UserEvent.h"
#include "SignBoard.h"
#include "LoudMessage.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KMissionEvent );
ImplOstmOperatorW2A( KMissionEvent );
NiImplementRootRTTI( KMissionEvent );

KMissionEvent::KMissionEvent(void)
:m_bChanged(false)
,m_dwTickGap(1000)
{
    m_dwLastTick = ::GetTickCount();
    m_setMissionList.clear();
}

KMissionEvent::~KMissionEvent(void)
{
}

ImplToStringW( KMissionEvent )
{
    KLocker lock( m_csMissionEvent );
    START_TOSTRINGW
        << TOSTRINGWb( m_bChanged );
    DumpCurrentEvent( stm_ );
    return stm_;
}

void KMissionEvent::UpdateFromCenter( IN KECN_MISSION_EVENT_UPDATE_NOT& kPacket_ )
{
    // 이 함수가 호출되었는 것은 DB에서 수정이 발생했다는 것이다.
    KLocker lock( m_csMissionEvent );
    m_vecCurrentMissions = kPacket_.m_vecMissions;
    m_setItemList.clear();
    std::vector<KMissionDailyData>::iterator vit;
    for( vit = m_vecCurrentMissions.begin() ; vit != m_vecCurrentMissions.end() ; ++vit )
    {
        m_setItemList.insert( vit->m_nItemID );
    }


    m_setMissionList.clear();
    std::set<int>::iterator sit;
    for( sit = kPacket_.m_setItemList.begin(); sit != kPacket_.m_setItemList.end(); ++sit )
    {
        m_setMissionList.insert( *sit );
    }
    bool bDateChange = kPacket_.m_bDateChanged;

    if( bDateChange )
    {
        SetEventChanged( true );
    }
    START_LOG( clog, L"센터서버에서 수정이 발생하였습니다." )
        << BUILD_LOG( kPacket_.m_vecMissions.size() )
        << BUILD_LOG( kPacket_.m_setItemList.size() )
        << BUILD_LOG( kPacket_.m_bDateChanged ) << END_LOG;
}

void KMissionEvent::NotifyFromCenter( IN KECN_MISSION_NOTIFY_NOT& kPacket_ )
{
    // 이 함수가 호출되었는 것은 공지를 출력해야 한다는 것이다.
    KLocker lock( m_csMissionEvent );

    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID          = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
    kData.m_strSenderNick.clear();
    if( SiKLoudMessage()->GetMissionMsg( kPacket_.m_dwTime, kData.m_strMsg ) )
        SiKSignBoard()->QueueingAdminData( kData );
}

void KMissionEvent::Tick()
{
    // 시간체크 1분마다 체크하면 될듯..,굳이 매프레임마다 체크할 필요는 없을듯..,
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) return;
    m_dwLastTick = ::GetTickCount();

    if( !IsEventChanged() ) return;
    // 전체 클라이언트에게 변경된 ItemID를 전송해 준다.
    // 굳이 필요는 없겠지만 클라이언트가 메세지를 뿌릴 때 어떤 ItemID를 받을지를 받자..,!
    std::set<int> setItems;
    GetItemList( setItems );
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_MISSION_DATE_CHANGE_NOT, setItems );
    SetEventChanged( false );
}

bool KMissionEvent::IsEventChanged()
{
    KLocker lock(m_csMissionEvent);
    return m_bChanged;
}

void KMissionEvent::SetEventChanged( bool bChange_ )
{
    KLocker lock(m_csMissionEvent);
    m_bChanged = bChange_;
}

bool KMissionEvent::IsMissionEventTerm()
{
    KLocker lock( m_csMissionEvent );
    return ( !m_setMissionList.empty() );
}

bool KMissionEvent::GetEventData( OUT KMissionCurrentEvent& kEventData_ )
{
    if( !IsMissionEventTerm() ) return false;

    KLocker lock( m_csMissionEvent );
    kEventData_.m_vecMissions = m_vecCurrentMissions;
    kEventData_.m_setItemList = m_setMissionList;
    return true;
}

void KMissionEvent::GetItemList( OUT std::set<int>& setItems_ )
{
    KLocker lock( m_csMissionEvent );
    setItems_ = m_setItemList;
}

void KMissionEvent::DumpCurrentEvent( std::wostream& stm_ ) const 
{
    stm_ << L" --- Current Event ---- " << std::endl;
    std::vector<KMissionDailyData>::const_iterator vit;
    for( vit = m_vecCurrentMissions.begin() ; vit != m_vecCurrentMissions.end() ; ++vit )
    {
        stm_ << L"[" << vit->m_nEventID
        << L", " << vit->m_nOrderID
        << L", " << vit->m_nItemID
        << L"]" << std::endl;
    }
}