#include "GSSimLayer.h"
#include "EventDungeonManager.h"
#include "Lua/KLuaManager.h"
#include <boost/bind.hpp>
#include <dbg/dbg.hpp >
#include "UserEvent.h"
#include "Log4.h"

ImplementSingleton( KEventDungeonManager );
ImplOstmOperatorW2A( KEventDungeonManager );
NiImplementRootRTTI( KEventDungeonManager );

ImplToStringW( KEventDungeonManager )
{
    KLocker lock( m_csEventDungeon );
    return START_TOSTRINGW << TOSTRINGW( m_vecDungeonInfo.size() );
}

KEventDungeonManager::KEventDungeonManager(void)
:m_kTickCheck( 60 * 1000, false )
{
}

KEventDungeonManager::~KEventDungeonManager(void)
{
}

bool KEventDungeonManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEventDungeonManager::_LoadScript( OUT std::string& strScript_ )
{
    _JIF( LoadEventDungeon(strScript_), return false );
    SendEventDungeonInfoNot();
    return true;
}

bool KEventDungeonManager::LoadEventDungeon( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    strScript_ = "InitEventDungeon.lua";
    std::vector<KEventDungeon> vecDungeonInfo;
    _JIF( kLuaMng.DoFile( strScript_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( std::string( "EventDungeon" ).c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        KEventDungeon kInfo;
        if ( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( "ModeID", kInfo.m_nModeID ) == S_OK, return false );
        _JIF( LoadTimeTable( kLuaMng, kInfo ), return false );
        vecDungeonInfo.push_back(kInfo);
        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );
    START_LOG( cerr, L"Load EventDungeon script.. " ) << BUILD_LOG(vecDungeonInfo.size()) << END_LOG;
    KLocker lock( m_csEventDungeon );
    return true;
}

bool KEventDungeonManager::LoadTimeTable( IN KLuaManager& kLuaMng_, OUT KEventDungeon& kEventdungeon_)
{
    KEventDungeon EventDungeonTemp;
    std::vector<int> vecDays_;
    vecDays_.clear();
    _JIF( kLuaMng_.BeginTable( std::string( "OpenDays" ).c_str() ) == S_OK, return false );
    int nOpenDay = -1;
    _JIF( kLuaMng_.GetValue( 1, nOpenDay ) == S_OK, nOpenDay = -1 );
    vecDays_.push_back( nOpenDay );
    _JIF( kLuaMng_.GetValue( 2, nOpenDay ) == S_OK, nOpenDay = -1 );
    vecDays_.push_back( nOpenDay );
    _JIF( kLuaMng_.GetValue( 3, nOpenDay ) == S_OK, nOpenDay = -1 );
    vecDays_.push_back( nOpenDay );
    _JIF( kLuaMng_.GetValue( 4, nOpenDay ) == S_OK, nOpenDay = -1 );
    vecDays_.push_back( nOpenDay );
    _JIF( kLuaMng_.GetValue( 5, nOpenDay ) == S_OK, nOpenDay = -1 );
    vecDays_.push_back( nOpenDay );
    _JIF( kLuaMng_.GetValue( 6, nOpenDay ) == S_OK, nOpenDay = -1 );
    vecDays_.push_back( nOpenDay );
    _JIF( kLuaMng_.GetValue( 7, nOpenDay ) == S_OK, nOpenDay = -1 );
    vecDays_.push_back( nOpenDay );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    EventDungeonTemp.m_vecOpenDays.swap(vecDays_);
    kEventdungeon_ = EventDungeonTemp;
    return true;
}

bool KEventDungeonManager::IsEventDungeon( IN const int nModeID_ )
{
    KLocker lock( m_csEventDungeon );
    std::vector<KEventDungeon>::const_iterator cvit;
    cvit = std::find_if( m_vecDungeonInfo.begin(), m_vecDungeonInfo.end(), boost::bind(&KEventDungeon::m_nModeID,_1) == nModeID_ );
    return ( cvit != m_vecDungeonInfo.end() );
}

bool KEventDungeonManager::GetEventDungeonInfo( IN const int nModeID_, OUT KEventDungeon& kInfo_ )
{
    KLocker lock( m_csEventDungeon );
    std::vector<KEventDungeon>::const_iterator cvit;
    cvit = std::find_if( m_vecDungeonInfo.begin(), m_vecDungeonInfo.end(), boost::bind(&KEventDungeon::m_nModeID,_1) == nModeID_ );
    if ( cvit == m_vecDungeonInfo.end() ) {
        START_LOG( cwarn, L"영웅던전 ModeID가 아닙니다. ModeID : " << nModeID_ ) << END_LOG;
        return false;
    }
    kInfo_ = *cvit;
    return true;
}

void KEventDungeonManager::GetEventDungeonList( OUT std::vector<KEventDungeon>& vecList_ )
{
    KLocker lock( m_csEventDungeon );
    vecList_ = m_vecDungeonInfo;
}

void KEventDungeonManager::GetEventDungeonIDList( OUT std::set<int>& setModeID_ )
{
    KLocker lock( m_csEventDungeon );
    std::vector<KEventDungeon>::const_iterator cvit;
    for ( cvit = m_vecDungeonInfo.begin() ; cvit != m_vecDungeonInfo.end() ; ++cvit ) {
        setModeID_.insert( cvit->m_nModeID );
    }
}

bool KEventDungeonManager::SendEventDungeonInfoNot()
{
    DWORD dwTemp = 0;
    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_UPDATEPLAN_EVENT_DUNGEON_INFO_NOT, dwTemp );
    return true;
}
