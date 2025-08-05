#include "CooperationEvent.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCooperationEvent );
ImplOstmOperatorW2A( KCooperationEvent );
NiImplementRootRTTI( KCooperationEvent );

KCooperationEvent::KCooperationEvent(void)
:m_nEventID(-1)
,m_tmBegin(1)
,m_tmEnd(1)
,m_dwMoneyItemID(0)
,m_bIsEventTerm(false)
,m_dwTickGap(6000)
{
    m_dwLastTick = ::GetTickCount();
    m_mapItemList.clear();
}

KCooperationEvent::~KCooperationEvent(void)
{
}

ImplToStringW( KCooperationEvent )
{
    KLocker lock( m_csCoopEvent );
    START_TOSTRINGW
        << TOSTRINGWb( m_bIsEventTerm )
        << TOSTRINGW( m_nEventID )
        << TOSTRINGW( m_dwMoneyItemID )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_mapItemList.size() );

    return stm_;
}

bool KCooperationEvent::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCooperationEvent::_LoadScript( OUT std::string& strScript_ )
{
    time_t tmBegin(1);
    time_t tmEnd(1);
    int nEventID(-1);
    DWORD dwTickGap(6000 );
    GCITEMID dwMoneyItemID(0);
    std::map<int,KCoopEventItem>  mapItemList;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitCoopEvent.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "EventID", nEventID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MoneyItemID", dwMoneyItemID ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "ItemList" ) == S_OK, return false );
    for( int i = 1; ; ++i )
    {

        KCoopEventItem kItem;
        if( !LoadItems( kLuaMng, i, kItem ) ) break;
        mapItemList.insert( std::make_pair( kItem.m_nSlotID, kItem ) );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    // Log 찍자..
    START_LOG( cerr, L"제휴 이벤트 스크립트 로드 완료..Items : " << mapItemList.size() )
        << BUILD_LOG( nEventID )
        << BUILD_LOG( dwMoneyItemID ) << END_LOG;

    KLocker lock( m_csCoopEvent );
    m_nEventID          = nEventID;
    m_tmBegin           = tmBegin;
    m_tmEnd             = tmEnd;
    m_dwMoneyItemID     = dwMoneyItemID;
    m_dwTickGap         = dwTickGap;
    m_mapItemList.swap( mapItemList );
    Update();

    return true;
}

void KCooperationEvent::Tick()
{
    KLocker lock( m_csCoopEvent );
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) return;
    m_dwLastTick = ::GetTickCount();
    Update();
}

int KCooperationEvent::GetEventID()
{
    KLocker lock( m_csCoopEvent );
    return m_nEventID;
}

GCITEMID KCooperationEvent::GetMoneyItem()
{
    KLocker lock( m_csCoopEvent );
    return m_dwMoneyItemID;
}

bool KCooperationEvent::IsEventerTerm()
{
    KLocker lock( m_csCoopEvent );
    return m_bIsEventTerm;
}

bool KCooperationEvent::GetItemList( OUT std::map<int,KCoopEventItem>& mapItemList_ )
{
    KLocker lock( m_csCoopEvent );
    if( m_mapItemList.empty() ) return false;
    mapItemList_ = m_mapItemList;
    return true;
}

bool KCooperationEvent::GetItemInfo( IN const int& nSlotID_, OUT KCoopEventItem& kItemInfo_ )
{
    std::map<int,KCoopEventItem>::iterator mit;
    KLocker lock( m_csCoopEvent );
    mit = m_mapItemList.find( nSlotID_ );
    if( mit == m_mapItemList.end() ) return false;
    kItemInfo_ = mit->second;
    return true;
}

bool KCooperationEvent::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCooperationEvent::LoadItems( IN KLuaManager& kLuaMng_, IN int nIndex_, OUT KCoopEventItem& kItem_ )
{
    if( kLuaMng_.BeginTable( nIndex_ ) != S_OK ) return false;
    _JIF( kLuaMng_.GetValue( 1, kItem_.m_nSlotID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, kItem_.m_dwItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kItem_.m_nCount ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, kItem_.m_nPeriod ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 5, kItem_.m_nPrice ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KCooperationEvent::Update()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    if( m_tmBegin > tmCurrent || m_tmEnd < tmCurrent )
        m_bIsEventTerm = false;
    else
        m_bIsEventTerm = true;
}

void KCooperationEvent::PrintItemList()
{
    std::wcout << L"---- Item List ----" << std::endl;
    KLocker lock( m_csCoopEvent );
    std::map<int,KCoopEventItem>::const_iterator mit;
    for( mit = m_mapItemList.begin() ; mit != m_mapItemList.end() ; ++mit )
    {
        std::wcout << L"[" << mit->second.m_nSlotID
            << L", " << mit->second.m_dwItemID
            << L", " << mit->second.m_nCount
            << L", " << mit->second.m_nPeriod
            << L", " << mit->second.m_nPrice
            << L"]" << std::endl;
    }

}