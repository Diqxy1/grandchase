#include "CharacterPollManager.h"
#include "userevent.h"
#include "UserPacket.h"
#include "GSDBLayer.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KCharacterPollManager );
ImplOstmOperatorW2A( KCharacterPollManager );
NiImplementRootRTTI( KCharacterPollManager );

KCharacterPollManager::KCharacterPollManager(void)
:m_bActive(false)
,m_tmBegin(1)
,m_tmEnd(1)
,m_dwPollItemID(0)
{
    InitTicks();
}

KCharacterPollManager::~KCharacterPollManager(void)
{
}

ImplToStringW( KCharacterPollManager )
{
    KLocker lock(m_csPollData);
    START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWb( m_bActive )
        << TOSTRINGW( m_adwTickGap[PTI_TICK] )
        << TOSTRINGW( m_adwTickGap[PTI_SYNC] )
        << TOSTRINGW( m_dwPollItemID );

    DumpAdjustChar( stm_ );
    DumpPollData( stm_ );
    return stm_;
}

bool KCharacterPollManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCharacterPollManager::_LoadScript( OUT std::string& strScript_ )
{
    time_t tmBegin;
    time_t tmEnd;
    DWORD dwTickGap;
    DWORD dwSyncGap;
    GCITEMID dwPollItemID;
    std::set<char> setAdjustChar;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitPollInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadAdjustChar( kLuaMng, setAdjustChar ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DBSyncGap", dwSyncGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PollItem", dwPollItemID ) == S_OK, return false );
    
    
    START_LOG( cerr, L"캐릭터 투표 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csPollData );
        m_tmBegin               = tmBegin;
        m_tmEnd                 = tmEnd;
        m_adwTickGap[PTI_SYNC]  = dwSyncGap;
        m_adwTickGap[PTI_TICK]  = dwTickGap;
        m_dwPollItemID          = dwPollItemID;
        m_setAdjustChar.swap( setAdjustChar );
    }
    UpdateState();
    return true;
}

bool KCharacterPollManager::LoadAdjustChar( IN KLuaManager& kLuaMng_, OUT std::set<char>& setAdjustChar_ )
{
    _JIF( kLuaMng_.BeginTable( "CharInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        int nCharIndex = -1;
        if( kLuaMng_.GetValue( i, nCharIndex ) != S_OK ) break;
        setAdjustChar_.insert( (char)(nCharIndex) );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCharacterPollManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCharacterPollManager::IsEnablePoll()
{
    KLocker lock( m_csPollData );
    return m_bActive;
}

bool KCharacterPollManager::IsPollItem( GCITEMID dwItemID_ )
{
    KLocker lock( m_csPollData );
    return (m_dwPollItemID == dwItemID_);
}

void KCharacterPollManager::AddPoll( char cCharType_, int nCount_ )
{
    KLocker lock( m_csPollData );
    m_mapPollData[cCharType_] += nCount_;
}

void KCharacterPollManager::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();
    if( dwCurrentTick - GetTick( PTI_TICK ) < GetTickGap( PTI_TICK ) ) return;
    UpdateState();
    SetTick( PTI_TICK );
    
    if( dwCurrentTick - GetTick( PTI_SYNC ) < GetTickGap( PTI_SYNC ) ) return;
    UpdateData();
}

void KCharacterPollManager::UpdateData()
{
    {
        KLocker lock( m_csPollData );
        if( !m_mapPollData.empty() )
            SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_CHAR_POLL_UPDATE_DB, NULL, 0, 0, m_mapPollData );
        m_mapPollData.clear();
    }

    SetTick( PTI_SYNC );
}

void KCharacterPollManager::InitTicks()
{
    for( int i = 0 ; i < PTI_MAX ; ++i )
    {
        SetTick( i );
        m_adwTickGap[i] = 1000 * 60;
    }
}

DWORD KCharacterPollManager::GetTick( int nIndex_ )
{
    _JIF( PTI_MAX > nIndex_ && 0 <= nIndex_, return 0 );
    KLocker lock( m_csPollData );
    return m_adwLastTic[nIndex_];
}

DWORD KCharacterPollManager::GetTickGap( int nIndex_ )
{
    _JIF( PTI_MAX > nIndex_ && 0 <= nIndex_, return 0xFFFFFFFF );
    KLocker lock( m_csPollData );
    return m_adwTickGap[nIndex_];
}

void KCharacterPollManager::SetTick( int nIndex_ )
{
    JIF( PTI_MAX > nIndex_ && 0 <= nIndex_ );
    KLocker lock( m_csPollData );
    m_adwLastTic[nIndex_] = ::GetTickCount();
}

void KCharacterPollManager::UpdateState()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KLocker lock( m_csPollData );

    if( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent )
        m_bActive = true;
    else
        m_bActive = false;
}

bool KCharacterPollManager::IsCorrectChar( char cCharType_ )
{
    KLocker lock( m_csPollData );
    return ( m_setAdjustChar.find( cCharType_ ) != m_setAdjustChar.end() );
}

GCITEMID KCharacterPollManager::GetPollItemID()
{
    KLocker lock( m_csPollData );
    return m_dwPollItemID;    
}

void KCharacterPollManager::DumpAdjustChar( std::wostream& stm_ ) const
{
    stm_ << "    " << L"AdjustChar : ";
    std::set<char>::const_iterator sit;
    for( sit = m_setAdjustChar.begin() ; sit != m_setAdjustChar.end() ; ++sit )
    {
        stm_ << (int)*sit << L", ";
    }
    stm_ << std::endl;
}

void KCharacterPollManager::DumpPollData( std::wostream& stm_ ) const
{
    stm_ << "    ---- Poll Info ----" << std::endl;
    if( m_mapPollData.empty() )
    {
        stm_<< "    ---- Empty ----" << std::endl;
        return;
    }

    std::map<char,int>::const_iterator mit;
    for( mit = m_mapPollData.begin() ; mit != m_mapPollData.end() ; ++mit )
    {
        stm_ << "    " << (int)mit->first << L"  : " << mit->second << std::endl;
    }
}

void KCharacterPollManager::TestPoll( int nCharType, int nCount )
{
    AddPoll( nCharType, nCount );
    std::cout << *this;
}