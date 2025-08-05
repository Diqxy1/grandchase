#include "MatchUser.h"
#include "MatchManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include "MatchSimLayer.h"
#include "MatchEvent.h"

ImplementSingleton( KMatchManager );
ImplOstmOperatorW2A( KMatchManager );
NiImplementRootRTTI( KMatchManager );

KMatchManager::KMatchManager(void)
{
}

KMatchManager::~KMatchManager(void)
{
}

ImplToStringW( KMatchManager )
{
    KLocker lock( m_csMatchInfo );

    PrintMatchModeInfo( stm_ );

    return START_TOSTRINGW
        << TOSTRINGW( m_kMatchInfo.m_nStartSearchScope )
        << TOSTRINGW( m_kMatchInfo.m_nMaxIncreaseScope )
        << TOSTRINGW( m_kMatchInfo.m_nIncreaseScope )
        << TOSTRINGW( m_kMatchInfo.m_mapLimitMatchTable.size() );
}

bool KMatchManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KMatchManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    KMatchInfo kMatchInfo;
    const std::string strScriptName = "InitMatchInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "StartSearchScope", kMatchInfo.m_nStartSearchScope ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxIncreaseScope", kMatchInfo.m_nMaxIncreaseScope ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "IncreaseScope", kMatchInfo.m_nIncreaseScope ) == S_OK, return false );
    _JIF( LoadLimitMatchTable( kLuaMng, kMatchInfo.m_mapLimitMatchTable ), return false );

    {
        KLocker lock( m_csMatchInfo );
        m_kMatchInfo = kMatchInfo;
        std::map<std::pair<DWORD,int>, KMatchGroupPtr >::iterator mit;
        std::map<int,std::pair<int,bool>>::iterator mitLimit;
        for( mit = m_mapMatchGroupManager.begin(); mit != m_mapMatchGroupManager.end(); ++mit ) {
            if ( mit->second == NULL ) {
                continue;
            }
            int nLimitMatchGroupNum = 0;
            bool bLimitMatchEnable = false;
            mitLimit = m_kMatchInfo.m_mapLimitMatchTable.find( mit->first.second );
            if( mitLimit != m_kMatchInfo.m_mapLimitMatchTable.end() ) {
                nLimitMatchGroupNum = mitLimit->second.first;
                bLimitMatchEnable = mitLimit->second.second;
            }

            mit->second->SetMatchCheckInfo( m_kMatchInfo.m_nStartSearchScope, m_kMatchInfo.m_nMaxIncreaseScope, m_kMatchInfo.m_nIncreaseScope, bLimitMatchEnable, nLimitMatchGroupNum );
        }
    }

    START_LOG( cerr, L"MatchInfo 정보 읽기 완료." )
        << BUILD_LOG( kMatchInfo.m_nStartSearchScope )
        << BUILD_LOG( kMatchInfo.m_nMaxIncreaseScope )
        << BUILD_LOG( kMatchInfo.m_nIncreaseScope )
        << BUILD_LOG( m_kMatchInfo.m_mapLimitMatchTable.size() )
        << END_LOG;

    return true;
}

void KMatchManager::Tick()
{
    DistributeEvent(); // EventQueue Tick.
    EachTick();
}

KMatchGroupPtr KMatchManager::GetMatchGroup( IN DWORD& dwServerUID_, IN int& nModeInfo_ )
{
    std::map<std::pair<DWORD,int>, KMatchGroupPtr >::iterator mit;
    std::pair<DWORD,int> prData;
    prData.first = dwServerUID_;
    prData.second = nModeInfo_;

    KLocker lock( m_csMatchInfo );
    mit = m_mapMatchGroupManager.find( prData );

    if ( mit == m_mapMatchGroupManager.end() ) {
        return KMatchGroupPtr();
    }

    return mit->second;
}

void KMatchManager::EachTick()
{
    std::map<std::pair<DWORD,int>, KMatchGroupPtr >::iterator mit;

    KLocker lock( m_csMatchInfo );
    for( mit = m_mapMatchGroupManager.begin(); mit != m_mapMatchGroupManager.end(); ++mit ) {
        if ( mit->second == NULL ) {
            continue;
        }
        mit->second->Tick();
    }
}

void KMatchManager::DistributeEvent()
{
    std::deque<KIntEventPtr> queEvent;
    {
        KLocker lock( m_csEventQueue );
        m_queEvent.swap( queEvent );
    }

    if ( queEvent.empty() ) return;

    std::deque<KIntEventPtr>::iterator qit;
    for ( qit = queEvent.begin() ; qit != queEvent.end() ; ++qit ) {
        KMatchGroupPtr spMatchGroup = GetMatchGroup( (*qit)->m_dwSenderUID, (*qit)->m_nModeType );
        if ( spMatchGroup == NULL ) {
            continue;
        }

        KEventPtr spEvent ( new KMatchEvent );
        spEvent->m_usEventID = (*qit)->m_usEventID;
        spEvent->m_kbuff = (*qit)->m_kbuff;

        spMatchGroup->QueueingEvent( spEvent );
    }
}

int KMatchManager::GetStartSearchScope()
{
    KLocker lock( m_csMatchInfo );
    return m_kMatchInfo.m_nStartSearchScope;
}

void KMatchManager::AddMatchInfo( IN KPartyData& kPartyData_ )
{
    KLocker lock( m_csMatchInfo );
    KMatchGroupPtr spMatchGroup = GetMatchGroup( kPartyData_.m_dwServerUID, kPartyData_.m_nModeInfo );
    if( spMatchGroup != NULL ) {
        START_LOG( clog, L"이미 존재하는 매치 Gruop" )
            << END_LOG;
        return;
    }
    std::map<int,std::pair<int,bool>>::iterator mitLimit;
    std::pair<DWORD, int> prData;
    int nLimitMatchGroup = LIMIT_MATCH_GROUP;
    bool bLimitMatchEnable = false;

    prData.first = kPartyData_.m_dwServerUID;
    prData.second = kPartyData_.m_nModeInfo;

    mitLimit = m_kMatchInfo.m_mapLimitMatchTable.find( prData.second );
    if( mitLimit != m_kMatchInfo.m_mapLimitMatchTable.end() ) {
        nLimitMatchGroup = mitLimit->second.first;
        bLimitMatchEnable = mitLimit->second.second;
    }

    spMatchGroup.reset( new KMatchGroup );
    spMatchGroup->SetServerInfo( kPartyData_.m_dwServerUID, kPartyData_.m_nModeInfo );
    spMatchGroup->SetMatchCheckInfo( m_kMatchInfo.m_nStartSearchScope, m_kMatchInfo.m_nMaxIncreaseScope, m_kMatchInfo.m_nIncreaseScope, bLimitMatchEnable, nLimitMatchGroup );
    m_mapMatchGroupManager[prData] = spMatchGroup;
}

void KMatchManager::PrintMatchModeInfo( std::wostream& stm_ ) const
{
    std::map<std::pair<DWORD,int>, KMatchGroupPtr >::const_iterator cmit;
    int nCount = 0;
    int nLimitMatchGroupNum = 0;
    bool bLimitMatchEnable = false;

    stm_ << L" --- MatchModeInfo( size:" << m_mapMatchGroupManager.size() << L") --- " << std::endl;
    for( cmit = m_mapMatchGroupManager.begin(); cmit != m_mapMatchGroupManager.end(); ++cmit ) {
        nCount = cmit->second->GetListCount();
        cmit->second->GetLimitInfo( nLimitMatchGroupNum, bLimitMatchEnable );
        stm_ << L" ServerUID : " << cmit->first.first << L" ModeType : " << cmit->first.second << L" Count : " << nCount << std::endl;
        stm_ << L" LimitMatchGroupNum : " << nLimitMatchGroupNum << std::endl;
        stm_ << L" bLimitMatchEnable : " << ( bLimitMatchEnable == true ? L"True" : L"False" ) << std::endl;
        stm_ << L" -------------------------------------- " << std::endl;
    }
    stm_ << L" --- MatchModeInfo Display End --- " << std::endl;
}

bool KMatchManager::LoadLimitMatchTable( IN KLuaManager& kLuaMng_, OUT std::map<int,std::pair<int,bool>>& mapLimitMatchTable_ )
{
    mapLimitMatchTable_.clear();
    _JIF( kLuaMng_.BeginTable( "LimitMatchTable" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nModeID;
        std::pair<int,bool> prData;
        _JIF( kLuaMng_.GetValue( "ModeID", nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "LimitMatchGroupNum", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "bLimitMatchEnable", prData.second ) == S_OK, return false );

        if( mapLimitMatchTable_.insert( std::make_pair( nModeID, prData ) ).second == false ) {
            START_LOG( cerr, L"중복된 모드 : " << nModeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}