#include "EventDivideDispatcher.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KEventDivideDispatcher );
ImplOstmOperatorW2A( KEventDivideDispatcher );
NiImplementRootRTTI( KEventDivideDispatcher );

KEventDivideDispatcher::KEventDivideDispatcher(void)
{
    m_mapEvents.clear();
}

KEventDivideDispatcher::~KEventDivideDispatcher(void)
{
}

ImplToStringW( KEventDivideDispatcher )
{
    KLocker lock( m_csEventInfo );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapDispatchTable.size() )
        << TOSTRINGW( m_mapEvents.size() );
}

bool KEventDivideDispatcher::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEventDivideDispatcher::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "EventDivideDispatchInfo.lua";
    strScript_ = strScriptName;

    std::map< unsigned short, KEventInfo >                           mapDispatchTable;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadEventInfo( kLuaMng, std::string( "EventInfo" ), mapDispatchTable ), return false );

    {
        KLocker lock( m_csEventInfo );

        m_mapDispatchTable.swap( mapDispatchTable );
    }
    START_LOG( cerr, L"Event divide dispatcher script loaded." ) << END_LOG;

    return true;
}

bool KEventDivideDispatcher::LoadEventInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< unsigned short, KEventInfo >& mapDispatchTable_ )
{
    mapDispatchTable_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KEventInfo kEventInfo;
        std::string strEventID;

        _JIF( kLuaMng_.GetValue( "ProcessTimeInterval", kEventInfo.m_dwProcessTimeInterval ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EventID", strEventID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "QueueSizeThreshold", kEventInfo.m_nQueueSizeThreshold ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ProcessCount", kEventInfo.m_nProcessCount ) == S_OK, return false );

        kEventInfo.m_dwLastTick = 0;
        kEventInfo.m_usEventID = GetEventID( strEventID );

        if ( kEventInfo.m_usEventID == KUserEvent::EVENT_EVENTID_SENTINEL ) {
            START_LOG( cerr, L"이벤트 이름이 정확하게 설정되었는지 확인하세요." ) << END_LOG;
            return false;
        }

        if ( !mapDispatchTable_.insert( std::make_pair( kEventInfo.m_usEventID, kEventInfo ) ).second ) {
            START_LOG( cerr, L"중복된 이벤트가 설정되었는지 확인하세요." ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

void KEventDivideDispatcher::Tick()
{
    KLocker lock( m_csEventInfo );

    std::map< unsigned short, KEventInfo >::iterator mitEvents;
    for ( mitEvents = m_mapDispatchTable.begin(); mitEvents != m_mapDispatchTable.end(); ++mitEvents ) {
        if ( mitEvents->second.CheckTime() ) {
            std::map< unsigned short, std::deque< KIntEventPtr > >::iterator mit;
            mit = m_mapEvents.find( mitEvents->first );
            if ( mit == m_mapEvents.end() ) {   // 아직 이벤트 큐가 등록되지 않았음
                continue;
            }
            if ( mit->second.empty() ) {    // 아직 이벤트 큐가 등록되지 않았음2
                continue;
            }

            /*
            GetPassCount() 내부에서 SiKGSDBLayer()->GetQueueSize()를 호출하기 때문에
            호출 빈도를 줄여서 DBLayer의 lock을 최소화 하자
            쌓여있는 이벤트가 없는 경우 호출하지 않도록 이곳에서 계산한다
            */
            int nPassCount = mitEvents->second.GetPassCount();

            // GetPassCount() 결과는 음수확인을 반드시 해준다
            if ( 0 >= nPassCount ) {
                continue;
            }

            // 지정된 수 만큼 or 가능한 만큼.. threshold를 넘지않게 nPassCount만큼 큐잉
            for ( int i = 0; i < nPassCount ; ++i ) {
                KIntEventPtr spEvent;

                if ( mit->second.empty() ) {
                    continue;
                }

                spEvent = mit->second.front();
                SiKGSDBLayer()->QueueingEvent( spEvent );
                mit->second.pop_front();

                // pop_front() 이후 덱 크기가0이 되어도 삭제없이 그냥 둔다
            }
        } // end if
    }
}

void KEventDivideDispatcher::QueueingEvent( IN KIntEventPtr spEvent_ )
{
    KLocker lock( m_csEventInfo );

    std::map< unsigned short, std::deque< KIntEventPtr > >::iterator mit;
    mit = m_mapEvents.find( spEvent_->m_usEventID );
    if ( mit == m_mapEvents.end() ) {   // 큐에 처음 등록되는 이벤트인 경우
        std::deque< KIntEventPtr > deqNew;
        deqNew.clear();

        std::pair< std::map< unsigned short, std::deque< KIntEventPtr > >::iterator, bool > prResult;
        prResult = m_mapEvents.insert( std::make_pair( spEvent_->m_usEventID, deqNew ) );
        if ( !prResult.second ) {
            START_LOG( cerr, L"이벤트 큐 생성 실패" ) << END_LOG;
            return;
        }
        mit = prResult.first;
    }
    mit->second.push_back( spEvent_ );
}

void KEventDivideDispatcher::PrintStatus()
{
    START_LOG( cerr, L"Event divide dispatcher status." ) << END_LOG;
    PrintEventInfo();
}

void KEventDivideDispatcher::PrintEventInfo()
{
    KLocker lock( m_csEventInfo );

    START_LOG( cerr, L"Print event status." ) << END_LOG;

    std::map< unsigned short, std::deque< KIntEventPtr > >::iterator mit;
    for ( mit = m_mapEvents.begin(); mit != m_mapEvents.end(); ++mit ) {
        START_LOG( cerr, L"EventID: " << mit->first << " size: " << mit->second.size() ) << END_LOG;
    }
}

unsigned short KEventDivideDispatcher::GetEventID( IN const std::string& strEventID_ )
{
    unsigned short usEventID = KUserEvent::EVENT_HEART_BIT;
    for ( ; usEventID < KUserEvent::EVENT_EVENTID_SENTINEL; ++usEventID ) {
        if ( 0 == wcsncmp( KncUtil::toWideString( strEventID_ ).c_str(), KUserEvent::GetEventIDString( usEventID ), wcslen( KncUtil::toWideString( strEventID_ ).c_str() ) ) ) {
            START_LOG( cerr, L"이벤트 찾음!!: " << KncUtil::toWideString( strEventID_ ) << L" : " << usEventID ) << END_LOG;
            break;
        }
    }
    // KUserEvent::EVENT_EVENTID_SENTINEL 가 리턴되면 이벤트를 찾지 못한 것이다
    return usEventID;
}
