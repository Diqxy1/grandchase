#include "TimeDrop.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "GSDBLayer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KTimeDrop );
ImplOstmOperatorW2A( KTimeDrop );
NiImplementRootRTTI( KTimeDrop );

KTimeDrop::KTimeDrop(void)
:m_kTimer(60*1000)
,m_tmBegin(1)
,m_tmEnd(1)
,m_nDropInterval(60)
,m_nDailyMaxDrop(0)
,m_bIsEventTerm(false)
{
}

KTimeDrop::~KTimeDrop(void)
{
}

ImplToStringW( KTimeDrop )
{
    KLocker lock( m_csTimeDrop );

    START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_kTimer.GetInterval() )
        << TOSTRINGW( m_nDropInterval )
        << TOSTRINGW( m_nDailyMaxDrop )
        << TOSTRINGW( m_mapDropItem.size() );
    return stm_;
}

void KTimeDrop::Tick()
{
    if ( m_kTimer.CheckTime() == false ) {
        return;
    }

    UpdateEventTerm();
}

bool KTimeDrop::LoadScript()
{
    return _LoadScript(std::string());
}

bool KTimeDrop::_LoadScript( OUT std::string& strScript_ )
{
    time_t tmBegin;
    time_t tmEnd;
    DWORD dwTickGap = 0;
    int nDropInterval = 0;
    int nDailyMaxDrop = 0;
    std::map< int, std::map<USHORT,std::vector<KItem> > > mapDropItem;

    KLuaManager kLuaMng;

    const std::string strScriptName = "InitTimeDrop.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DropInterval", nDropInterval ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DailyMaxDrop", nDailyMaxDrop ) == S_OK, return false );
    _JIF( LoadDropItem( kLuaMng, std::string("DropItem"), mapDropItem ), return false );

    START_LOG( cerr, L"Load Compleate.. TimeDrop Info" ) << END_LOG;

    KLocker lock( m_csTimeDrop );
    m_tmBegin = tmBegin;
    m_tmEnd = tmEnd;
    m_kTimer.SetInterval(dwTickGap);
    m_nDropInterval = nDropInterval;
    m_nDailyMaxDrop = nDailyMaxDrop;
    m_mapDropItem.swap( mapDropItem );
    return true;
}

bool KTimeDrop::LoadTime( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT time_t& tmTime_ )
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

bool KTimeDrop::LoadDropItem( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map< int, std::map<USHORT,std::vector<KItem> > >& mapItems_ )
{
    mapItems_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while ( true ) {
        int nCount = -1;
        std::map<USHORT,std::vector<KItem> > mapItems;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "Count", nCount ) == S_OK, return false );
        _JIF( LoadItem( kLuaMng_, std::string("Item"), mapItems ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( mapItems_.insert( std::make_pair( nCount, mapItems ) ).second == false ) {
            START_LOG( cerr, L"중복된 키가 있음.. Key : " << nCount ) << END_LOG;
            return false;
        }
        ++i;
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KTimeDrop::LoadItem( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<USHORT,std::vector<KItem> >& mapItems_ )
{
    mapItems_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int nIndex = 1;
    while ( true ) {
        USHORT usReciver = 0;
        KItem kItem;

        if( kLuaMng_.BeginTable( nIndex ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, usReciver ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItem.m_nCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kItem.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapItems_[usReciver].push_back( kItem );
        ++nIndex;
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KTimeDrop::DumpInfo()
{
    //KLocker lock( m_csTimeDrop );

    //std::cout << L"-- Dump TimeDrop --" << dbg::endl
    //    << L" Begin Time : " << (LPCTSTR)((CTime)m_tmBegin).Format( KNC_TIME_FORMAT ) << dbg::endl
    //    << L" End Time : " << (LPCTSTR)((CTime)m_tmEnd).Format( KNC_TIME_FORMAT ) << dbg::endl
    //    << L" Drop Interval : " << m_nDropInterval << dbg::endl
    //    << L" Daily Max Drop : " << m_nDailyMaxDrop << dbg::endl
    //    << L" Drop Item (Receiver,ItemID,Duration,Period) : " << dbg::endl;

    //std::map< int, std::map<USHORT,std::vector<KItem> > >::const_iterator cmit;
    //for ( cmit = m_mapDropItem.begin() ; cmit != m_mapDropItem.end() ; ++cmit ) {
    //    std::cout << std::tab << cmit->first << L" : ";

    //    std::map<USHORT,std::vector<KItem> >::const_iterator cmitItem;
    //    for ( cmitItem = cmit->second.begin() ; cmitItem != cmit->second.end() ; ++cmitItem ) {
    //        std::vector<KItem>::const_iterator cvit;
    //        for ( cvit = cmitItem->second.begin() ; cvit != cmitItem->second.end() ; ++cvit ) {
    //            std::cout << L"(" << cmitItem->first << L"," << cvit->m_ItemID << L"," << cvit->m_nDuration << L"," << cvit->m_nPeriod <<L"), ";
    //        }
    //        std::cout << dbg::endl;
    //    }
    //}

    //std::cout << L"-- End Dump --" << dbg::endl;
}

void KTimeDrop::UpdateEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KLocker lock( m_csTimeDrop );
    m_bIsEventTerm = (tmCurrent >= m_tmBegin && tmCurrent <= m_tmEnd);
}

bool KTimeDrop::GetCurrentDropItem( IN const int& nIdx_, IN const bool& bPCBang_, OUT std::vector<KItem>& vecItems_ )
{
    vecItems_.clear();

    if ( nIdx_ > GetDailyMaxDrop() ) {
        START_LOG( cwarn, L"일일 최대 회수만큼 아이템을 가져감. Index : " << nIdx_ << L", DailyMaxDrop : " << m_nDailyMaxDrop ) << END_LOG;
        return false;
    }

    KLocker lock( m_csTimeDrop );
    std::map< int, std::map<USHORT,std::vector<KItem> > >::const_iterator cmit = m_mapDropItem.find( nIdx_ );
    if ( cmit == m_mapDropItem.end() ) {
        START_LOG( cwarn, L"해당 차수의 아이템 리스트가 없습니다. Index : " << nIdx_ ) << END_LOG;
        return false;
    }

    std::map<USHORT,std::vector<KItem> >::const_iterator cmitItem;
    for ( cmitItem = cmit->second.begin() ; cmitItem != cmit->second.end() ; ++cmitItem ) {
        if ( cmitItem->first == DR_PC_BANG && false == bPCBang_ ) {
            continue;
        }

        if ( cmitItem->first == DR_NORMAL && bPCBang_ ) {
            continue;
        }

        vecItems_.insert( vecItems_.end(), cmitItem->second.begin(), cmitItem->second.end() );
    }

    return true;
}

//=========================================================================================//

KTimeDropCheck::KTimeDropCheck()
:m_nAccPlayTime(0)
,m_nDropCount(0)
{
}

KTimeDropCheck::~KTimeDropCheck()
{
}

void KTimeDropCheck::SetInfo( IN const int& nAccPlayTime_, IN const int& nCurrentDropCount_ )
{
    // DB에서 받아온 정보를 설정
    m_nAccPlayTime = nAccPlayTime_;
    m_nDropCount = nCurrentDropCount_;
}

void KTimeDropCheck::CheckTimeDrop( IN const DWORD& dwUID_, IN const std::wstring& strLogin_, IN const char cCharType_, IN const bool& bPCBang_, IN const bool& bChangeDate_ )
{
    CTime tDate = CTime::GetCurrentTime();

    if ( bChangeDate_ == true ) {
        // 날짜 바뀌면 어제 날짜로 기록 남김
        CTimeSpan tSpan( 1,0,0,0);
        tDate -= tSpan;

        // 초기화
        RestAccPlayTime();
        ResetDropCount();
        START_LOG( clog, L"날짜 바뀌어서 초기화함. Login : " << strLogin_ )
            << BUILD_LOGtm( tDate ) << END_LOG;
    }

    // 플레이시간 갱신
    IncAccPlayTime();
    START_LOG( clog, L"플레이 시간 갱신. AccPlayTime : " << GetAccPlayTime() << L",Login : " << strLogin_ )
        << BUILD_LOGtm( tDate ) << END_LOG;

    // DB에 유저의 기록 남기자.
    SendUpdateTimeDropToDB( dwUID_, strLogin_, cCharType_, KncUtil::TimeToInt( tDate ) );

    if ( GetAccPlayTime() < SiKTimeDrop()->GetDropInterval() ) {
        START_LOG( clog, L"플레이시간이 Drop주기보다 적음." )
            << BUILD_LOG( GetAccPlayTime() )
            << BUILD_LOG( SiKTimeDrop()->GetDropInterval() ) << END_LOG;
        return;
    }

    RestAccPlayTime();
    START_LOG( clog,L"Drop주기를 만족. AccPlayTime 초기화. DropInterval : " << SiKTimeDrop()->GetDropInterval() << L",Login : " << strLogin_ ) << END_LOG;

    std::vector<KItem> vecItems;
    bool bRet = SiKTimeDrop()->GetCurrentDropItem( GetNextDropIndex(), bPCBang_, vecItems );

    _LOG_CONDITION( bRet == true, clog, cwarn, L"GetDropItem.. result : " << bRet )
        << BUILD_LOG( bPCBang_ )
        << BUILD_LOG( GetNextDropIndex() )
        << BUILD_LOG( vecItems.size() ) << END_LOG;

    if ( bRet == true && !vecItems.empty() ) {
        // DB에 아이템 요청하자.
        KTimeDropItem kDBPacket;
        kDBPacket.m_nIndex = GetNextDropIndex();
        kDBPacket.m_vecItems.swap( vecItems );
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_TIME_DROP_REQ, strLogin_.c_str(), dwUID_, cCharType_, kDBPacket );

        // 낙장불입.카운트 올린다.
        IncDropCount();

        START_LOG( clog, L"DB에 TimeDrop Item 요청하고 DropCount 증가. Login : " << strLogin_ )
            << BUILD_LOG( kDBPacket.m_nIndex )
            << BUILD_LOG( kDBPacket.m_vecItems.size() )
            << BUILD_LOG( GetDropCount() ) << END_LOG;
    }
}

void KTimeDropCheck::SendUpdateTimeDropToDB( IN const DWORD& dwUID_, IN const std::wstring& strLogin_, IN const char cCharType_, IN const time_t& tmDate_ )
{
    // 1분단위로 부르자.
    KDB_EVENT_UPDATE_TIME_DROP_NOT kDBPacket;
    kDBPacket.m_nCurruntDropCount = GetDropCount();
    kDBPacket.m_nAccPlayTime = GetAccPlayTime();
    kDBPacket.m_tmRegDate = tmDate_;
    SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_TIME_DROP_NOT, strLogin_.c_str(), dwUID_, cCharType_, kDBPacket );
}
