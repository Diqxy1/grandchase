#include "User.h"
#include "SignBoard.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "SquareManager.h"
#include "ChannelManager.h"
#include "UserFSM.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KSignBoard );
NiImplementRootRTTI( KSignBoard );
ImplOstmOperatorW2A( KSignBoard );

ImplToStringW( KSignBoard )
{
    KLocker lock( m_csSignBoard );
    START_TOSTRINGW
        << TOSTRINGW( m_dwFlushGap )
        << TOSTRINGW( m_dwAdminFlushGap )
        << TOSTRINGW( m_dwCurrentFlushGap )
        << TOSTRINGW( m_dwMaxCapacity )
        << TOSTRINGW( m_queSignBoardAdminData.size() )
        << TOSTRINGW( m_queSignBoardData.size() )
        << TOSTRINGW( m_mapScopeData.size() );

    return stm_;
}

KSignBoard::KSignBoard(void)
:m_dwFlushGap( 15000 )
,m_dwCurrentFlushGap( 15000 )
,m_dwAdminFlushGap( 15000 )
,m_dwMaxCapacity( 30 )
{
    SetTick();
    m_mapScopeData.clear();


}

KSignBoard::~KSignBoard(void)
{
}

bool KSignBoard::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSignBoard::_LoadScript( OUT std::string& strScript_ )
{
    DWORD dwMaxCapacity     = 0;
    DWORD dwFlushGap        = 20000;
    DWORD dwAdminFlushGap   = 15000;

    std::map<int,std::vector<GCITEMID> >   mapScopeData;
    mapScopeData.clear();
    
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitSignBoardInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "SignBoardInfo" ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "dwMaxCapacity", dwMaxCapacity ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "dwFlushGap", dwFlushGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "dwAdminFlushGap", dwAdminFlushGap ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "SquareScope" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        GCITEMID dwItemID;
        if( kLuaMng.GetValue( i, dwItemID ) != S_OK ) break;
        mapScopeData[KSignBoardData::SBT_SQUARE].push_back( dwItemID );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "ServerScope" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        GCITEMID dwItemID;
        if( kLuaMng.GetValue( i, dwItemID ) != S_OK ) break;
        mapScopeData[KSignBoardData::SBT_SERVER].push_back( dwItemID );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"전광판 정보 로드 .." << mapScopeData.size() )
        << BUILD_LOG( dwMaxCapacity )
        << BUILD_LOG( dwFlushGap ) << END_LOG;

    KLocker lock( m_csSignBoard );

    m_dwFlushGap            = dwFlushGap;
    m_dwAdminFlushGap       = dwAdminFlushGap;
    m_dwCurrentFlushGap     = dwFlushGap;
    m_dwMaxCapacity         = dwMaxCapacity;
    m_mapScopeData.swap( mapScopeData );
    return true;
}

void KSignBoard::Tick()
{
    if( IsLoudTick() )
    {
        LoudSignBoard();
        SetTick();
    }
}

DWORD KSignBoard::GetCapacity()
{
    KLocker lock( m_csSignBoard );
    return m_dwMaxCapacity;
}
DWORD KSignBoard::GetSize()
{
    KLocker lock( m_csSignBoard );
    return (DWORD)(m_queSignBoardData.size() + m_queSignBoardAdminData.size());
}
bool KSignBoard::IsCorrectScope( IN const int nType, IN const GCITEMID itemID_ )
{
    KLocker lock( m_csSignBoard );
    std::map<int,std::vector<GCITEMID> >::const_iterator mit;
    mit = m_mapScopeData.find( nType );
    if( mit == m_mapScopeData.end() ) return false;

    std::vector<GCITEMID>::const_iterator vit;
    vit = std::find( mit->second.begin(), mit->second.end(), itemID_ );
    if( vit == mit->second.end() ) return false;

    return true;
}

DWORD KSignBoard::QueueingData( KSignBoardData& kData )
{
    KLocker lock( m_csSignBoard );

    m_dwCurrentFlushGap = m_dwFlushGap;
    m_queSignBoardData.push( kData );

    return GetSize();
}

DWORD KSignBoard::QueueingAdminData( KSignBoardData& kData )
{
    KLocker lock( m_csSignBoard );

    m_dwCurrentFlushGap = m_dwAdminFlushGap;
    m_queSignBoardAdminData.push( kData );

    return GetSize();
}

void KSignBoard::SendDataToSpecificPlayer(DWORD dwUID, KSignBoardData& kData)
{
    SiKSquareManager()->SendTo(dwUID, KUserEvent::EVENT_SIGN_BOARD_NOT, kData);
}

void KSignBoard::LuaMsg( IN const char* szMsg_ )
{
    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID          = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;
    kData.m_strMsg          = KncUtil::toWideString( szMsg_ );
    kData.m_strSenderNick.clear();
    QueueingAdminData( kData );
}

void KSignBoard::LoudSignBoard()
{
    KSignBoardData kData;
    if( !GetData( kData ) ) return; // 데이터를 꺼내온다. 운영자 데이터 우선

    switch( kData.m_eType )
    {
    case KSignBoardData::SBT_SQUARE:
    case KSignBoardData::SBT_SQUARE_ADMIN:
        {
            SiKSquareManager()->SendToAll( KUserEvent::EVENT_SIGN_BOARD_NOT, kData );
        }
        break;
    case KSignBoardData::SBT_SERVER:
    case KSignBoardData::SBT_SERVER_ADMIN:
        {
            SiKChannelManager()->SendToAll( KUserEvent::EVENT_SIGN_BOARD_NOT, kData );
            SiKSquareManager()->SendToAll( KUserEvent::EVENT_SIGN_BOARD_NOT, kData );
        }
        break;
    case KSignBoardData::SBT_SERVER_ADMIN_EX_GAME:
        {
            std::set<int> setState;
            setState.insert( KUserFSM::STATE_CHANNEL );
            setState.insert( KUserFSM::STATE_ROOM );
            SiKChannelManager()->SendToAllState( KUserEvent::EVENT_SIGN_BOARD_NOT, kData, setState );
            SiKSquareManager()->SendToAll( KUserEvent::EVENT_SIGN_BOARD_NOT, kData );
        }
        break;
    }
}

bool KSignBoard::GetData( KSignBoardData& kData_ )
{
    KLocker lock( m_csSignBoard );
    
    if( !m_queSignBoardAdminData.empty() )
    {
        kData_ = m_queSignBoardAdminData.front();
        m_queSignBoardAdminData.pop();
        return true;
    }

    if( m_queSignBoardData.empty() ) return false;

    kData_ = m_queSignBoardData.front();
    m_queSignBoardData.pop();
    return true;
}

bool KSignBoard::IsLoudTick()
{
    KLocker lock( m_csSignBoard );
    return ( GetSize() > 0 && ::GetTickCount() - GetTick() >= m_dwCurrentFlushGap );
}

void KSignBoard::ClearLuaMsg()
{
    KLocker lock( m_csSignBoard );
    m_dwCurrentFlushGap = m_dwAdminFlushGap;
    while( !m_queSignBoardAdminData.empty() ) {
        m_queSignBoardAdminData.pop();
    }
}