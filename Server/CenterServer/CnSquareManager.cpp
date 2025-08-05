#include "Center.h"
#include "NetError.h"
#include <dbg/dbg.hpp>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include "CnSquare.h"
#include "CnSquareManager.h"
#include "CenterServer.h"
#include "CenterPacket.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

//void KSquareSetInfo::SetInfo( const char* szName_, DWORD dwMaxNum_ )
//{
//    m_strName       = szName_;
//    m_dwMaxUserNum  = dwMaxNum_;
//}

void KSquareSetInfo::SetInfo( const char* szName_, DWORD dwMaxNum_, int nType_ )
{
    m_strName       = szName_;
    m_dwMaxUserNum  = dwMaxNum_;
    m_nType         = nType_;
}

void KSquareSetInfo::SetAddress( const char* szAddress_, unsigned short usPort_ )
{
    m_Address.sin_addr.s_addr = inet_addr( szAddress_ );
    m_Address.sin_port        = htons( usPort_ );
    m_strServerIP             = szAddress_;
}

ImplementSingleton( KCnSquareManager );
NiImplementRootRTTI( KCnSquareManager );
ImplOstmOperatorW2A( KCnSquareManager );
ImplToStringW( KCnSquareManager )
{
    KLocker lock( m_csSquares );
    //KLocker lock1( m_csSquareInfo );
    START_TOSTRINGW
        << TOSTRINGW( m_mapSquares.size() );

    return stm_;
}

KCnSquareManager::KCnSquareManager()
:m_kTimer( 10000 )
{
    m_iLastUID = 0;
}

KCnSquareManager::~KCnSquareManager()
{
}



bool KCnSquareManager::LoadScript( IN std::shared_ptr<lua_State>& sptLua_ )
{
    KLuaManager luaMgr(sptLua_.get());
    _JIF( luaMgr.DoFile("InitSquare.lua") == S_OK, return false );

    return true;
}

KCnSquarePtr KCnSquareManager::AddSquareWithGet( KSquareSetInfo& kInfo_, const char* szTableName_ )
{
    _JIF( szTableName_ != NULL, return KCnSquarePtr() );
    // Name
    std::wstring strKey  = KncUtil::toWideString( kInfo_.m_strName );
    wchar_t szSquareName[255] = {0};
    GetPrivateProfileStringW( L"Squares", strKey.c_str(), L"", szSquareName, 255, SiKCenterServer()->m_stringTable.GetCurrentFile().c_str() );
    std::wstring strName(szSquareName);
    _JIF( !strName.empty(), return KCnSquarePtr() );
    _JIF( kInfo_.m_dwMaxUserNum > 0, return KCnSquarePtr() );

    DWORD   dwIPAddress = kInfo_.m_Address.sin_addr.S_un.S_addr;
    USHORT  usPort = ntohs(kInfo_.m_Address.sin_port);

    KCnSquarePtr spSquare = KCnSquarePtr( new KCnSquare );

    spSquare->SetName( strName );
    spSquare->SetUID( ++m_iLastUID );
    spSquare->SetType( KSquareInfo::SQUARE_TYPE( kInfo_.m_nType ) );
    //spSquare->SetType( KSquareInfo::ST_NORMAL );
    spSquare->SetMaxUser( kInfo_.m_dwMaxUserNum );
    _JIF( spSquare->LoadInfo( szTableName_ ), return KCnSquarePtr() );
    _JIF( spSquare->SetTRAddress( dwIPAddress, usPort ), return KCnSquarePtr() );
    std::wstring strIP  = KncUtil::toWideString( kInfo_.m_strServerIP );
    spSquare->SetTRServerIP( strIP );

    KLocker lock( m_csSquares );
    std::pair< std::map<DWORD,KCnSquarePtr>::iterator, bool> prRet;
    prRet = m_mapSquares.insert( std::make_pair(spSquare->GetUID(), spSquare) );
    m_vecSquares.push_back( spSquare );

    _JIF( prRet.second, return KCnSquarePtr() );
    //m_vecSquares.push_back( spSquare );

    BuildSquareList( true );
    return spSquare;
}

bool KCnSquareManager::AddSquare( KSquareSetInfo& kInfo_, const char* szTableName_ ) //, DWORD dwGuildID_ ) // 스크립트에서 부르거나, 길드 광장 생성건으로 부른다.
{
    return AddSquareWithGet( kInfo_, szTableName_ ) != NULL;
}

bool KCnSquareManager::EnterSquare( IN KSquareUserInfo& kPacket_, IN KCenterPtr spCenter_, OUT KECN_ENTER_SQUARE_ACK& kPacket )
{
    KCnSquarePtr spSquare;

    std::map<DWORD,KCnSquarePtr>::iterator mit;

    {
        KLocker lock( m_csSquares );

        mit = m_mapSquares.find( kPacket_.m_dwSquareID );

        // 광장 존재 여부
        _JIF( mit != m_mapSquares.end(), SET_ERROR(ERR_SQUARE_01); return false );
        // 광장 인원 체크
        _JIF( !mit->second->IsFull(), SET_ERROR(ERR_SQUARE_02); return false );

        spSquare = mit->second;
    }

    if( !spSquare )
    {
        SET_ERROR(ERR_SQUARE_01);
        return false;
    }

    return AddUserToSquare( spSquare, kPacket_, spCenter_, kPacket );
}

bool KCnSquareManager::EnterGuildSquare( IN KECN_ENTER_GUILD_SQUARE_REQ& kPacket_, IN KCenterPtr spCenter_, OUT KECN_ENTER_GUILD_SQUARE_ACK& kPacket )
{
    GuildContainer::iterator mitGuild;
    GuildIndexContainer::iterator mitGuildIndex;

    KLocker lock( m_csSquares );

    if( m_vecGuildSquareSetInfo.empty() )
    {
        START_LOG( cerr, L"Guild Square Set Info vector 가 비어 있습니다. 광장 생성 실패!!" ) << END_LOG;
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    DWORD dwGuildUID = kPacket_.m_kSquareUserInfo.m_dwGuildUID;

    mitGuild  = m_mapGuildSquare.find( dwGuildUID );

    if( mitGuild == m_mapGuildSquare.end() )
    {
        mitGuild = m_mapGuildSquare.insert( GuildContainer::value_type( dwGuildUID, GuildIndexContainer() ) ).first;
    }

    mitGuildIndex = mitGuild->second.find( kPacket_.m_iGuildSquareIndex );

    KCnSquarePtr spSquare;
    KSquareSetInfo kInfo;
    static int iGuildSquareSetInfoCount = 0;

    if ( mitGuildIndex == mitGuild->second.end() ) {

        ++iGuildSquareSetInfoCount;
        if( iGuildSquareSetInfoCount >= (int)m_vecGuildSquareSetInfo.size() )
        {
            iGuildSquareSetInfoCount = 0;
        }

        kInfo = m_vecGuildSquareSetInfo[iGuildSquareSetInfoCount];

        spSquare = AddSquareWithGet( kInfo, "SquareInfo");
        spSquare->SetType( KSquareInfo::ST_GUILD );
        mitGuild->second.insert( GuildIndexContainer::value_type( kPacket_.m_iGuildSquareIndex, spSquare->GetUID() ));
    }
    else {
        spSquare = GetSquare( mitGuildIndex->second );
    }

    if ( !spSquare ) {
        SET_ERROR(ERR_SQUARE_01);
        return false;
    }

    kPacket_.m_kSquareUserInfo.m_dwSquareID = spSquare->GetUID();
    spSquare->GetSquareInfo( kPacket.m_kSquareInfo );
    
    // 길드 광장의 인덱스( 광장 총 4개 * 유저 50명 = 200명 제한 기준 )
    kPacket.m_kSquareInfo.m_nGuildSquareIndex = kPacket_.m_iGuildSquareIndex;
    kPacket.m_kSquareInfo.m_dwGuildUID = kPacket_.m_kSquareUserInfo.m_dwGuildUID;

    return AddUserToSquare( spSquare, kPacket_.m_kSquareUserInfo, spCenter_, kPacket.m_kEnterSquare );
}


bool KCnSquareManager::AddUserToSquare( IN KCnSquarePtr spSquare, IN KSquareUserInfo& kPacket_, IN KCenterPtr spCenter_, OUT KECN_ENTER_SQUARE_ACK& kPacket )
{
    if ( spSquare->EnterSquareReserve( kPacket_, spCenter_, kPacket ) == false ) {
        return false;
    }

    kPacket.m_dwUserUID = kPacket_.m_dwUID; // 서버로 보내는 패킷에 유저의 UID를 넣겠음.(어떤 유저가 받을지 찾아야 되니까)
    kPacket.m_dwSquareUID = kPacket_.m_dwSquareID;
    kPacket.m_nOK = 0; // 에러 처리 필요

    KLocker lock( m_csSquares );

    m_mapUserInSquare.insert( std::map<DWORD,DWORD>::value_type( kPacket_.m_dwUID,kPacket_.m_dwSquareID) );

    return true;
}

void KCnSquareManager::GetSquareList( OUT std::vector<KSquareInfo>& vecSquareList )
{
    KLocker lock( m_csSquares );
    vecSquareList.insert( vecSquareList.end(), m_vecBuildList.begin(), m_vecBuildList.end() );
}

void KCnSquareManager::BuildSquareList( IN bool bInit_ )
{
    std::vector<KSquareInfo> tempList;
    std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
    std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
    SiKCenterServer()->GetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );

    std::vector<KCnSquarePtr>::iterator vit;
    std::vector<KSimpleServerInfo>::iterator vitServer;

    KLocker lock( m_csSquares );

    for( vit = m_vecSquares.begin() ; vit != m_vecSquares.end() ; ++vit ) {

        if( (*vit)->GetType() != KSquareInfo::ST_NORMAL )
            continue;

        KSquareInfo kInfo;
        (*vit)->GetSquareInfo( kInfo );

        if( !bInit_ && !vecNoUseTCPServerInfo.empty() ) {
            for( vitServer = vecNoUseTCPServerInfo.begin(); vitServer != vecNoUseTCPServerInfo.end(); ++vitServer ) {
                if( (*vit)->GetTRServerIP() == vitServer->m_strIP && (*vit)->GetTRServerPort() == vitServer->m_usPort ) {
                    kInfo.m_nStatus = KSquareInfo::SS_CLOSE;
                }
            }
        }
        tempList.push_back( kInfo );
    }

    m_vecBuildList.swap( tempList );
}

bool KCnSquareManager::LeaveSquare( IN DWORD dwUID )
{
    KLocker lock( m_csSquares );

    std::map<DWORD,DWORD>::iterator mitUser = m_mapUserInSquare.find( dwUID );

    if( mitUser == m_mapUserInSquare.end() )
    {
        std::vector<KCnSquarePtr>::iterator vit;

        for( vit = m_vecSquares.begin() ; vit != m_vecSquares.end() ; ++vit )
        {
            (*vit)->RemoveUser( dwUID );
        }
        return false;
    }

    DWORD dwSquareID = m_mapUserInSquare[dwUID];
    m_mapUserInSquare.erase( mitUser );

    std::map<DWORD,KCnSquarePtr>::iterator mitSquare;
    mitSquare = m_mapSquares.find( dwSquareID );

    if( mitSquare == m_mapSquares.end() )
        return false;

    KCnSquarePtr spSquare = mitSquare->second;
    
    _JIF( spSquare, return false );

    KECN_LEAVE_SQUARE_BROAD kPacket;
    kPacket.m_dwUID = dwUID;
    kPacket.m_dwSquareUID = spSquare->GetUID();

    spSquare->SendToAll( KCenterEvent::ECN_LEAVE_SQUARE_BROAD, kPacket );

    if( spSquare->RemoveUser( dwUID ) == false )
        return false;

    if( spSquare->IsEmpty() && spSquare->GetType() == KSquareInfo::ST_GUILD )
    {
        RemoveSquare( spSquare );
    }

    return true;
}

void KCnSquareManager::RemoveSquare( IN KCnSquarePtr spSquare )
{
    if( spSquare->IsEmpty() == false )
    {
        START_LOG( cerr, L"삭제하려는 광장이 비어있지 않음, Square ID:" << spSquare->GetUID() ) << END_LOG;
        return;
    }

    KLocker lock( m_csSquares );

    GuildContainer::iterator mitGuild;
    for( mitGuild = m_mapGuildSquare.begin() ; mitGuild != m_mapGuildSquare.end() ; ++mitGuild )
    {
        bool bFind = false;
        GuildIndexContainer::iterator mitIndexGuild;
        
        for( mitIndexGuild = mitGuild->second.begin() ; mitIndexGuild != mitGuild->second.end() ; ++mitIndexGuild )
        {
            if( mitIndexGuild->second != spSquare->GetUID() )
                continue;

            mitGuild->second.erase( mitIndexGuild );
            bFind = true;
            break;
        }

        if( bFind )
        {
            if( mitGuild->second.empty() )
            {
                m_mapGuildSquare.erase( mitGuild );
            }
            break;
        }
    }

    std::map<DWORD,KCnSquarePtr>::iterator mit;
    mit = m_mapSquares.find( spSquare->GetUID() );
    if( mit == m_mapSquares.end() )
    {
        START_LOG( cerr, L"광장 맵에 삭제하려는 광장이 없음 , Square ID : " << spSquare->GetUID() ) << END_LOG;
    }
    else
    {
        m_mapSquares.erase( mit );
    }

    std::vector<KCnSquarePtr>::iterator vit;
    vit = std::find( m_vecSquares.begin(), m_vecSquares.end(), spSquare );

    if( vit != m_vecSquares.end() )
    {
        m_vecSquares.erase( vit );
    }
}

KCnSquarePtr KCnSquareManager::GetSquare( DWORD dwSquareID )
{
    std::map<DWORD,KCnSquarePtr>::iterator mit;

    KLocker lock( m_csSquares );

    mit = m_mapSquares.find( dwSquareID );

    if( mit == m_mapSquares.end() )
        return KCnSquarePtr();

    return mit->second;
}

bool KCnSquareManager::EnterMinigame( IN DWORD dwUserUID_ )
{
    KCnSquarePtr spSquare = GetUserSquare( dwUserUID_ );

    if( spSquare == NULL )
    {
        return false;
    }

    spSquare->EnterMinigame( dwUserUID_ );
    LeaveSquare( dwUserUID_ );

    // 실제로 광장을 나간 것이 아니므로, mapUserInSquare에 다시 넣어준다
    KLocker lock( m_csSquares );

    m_mapUserInSquare.insert( std::map<DWORD,DWORD>::value_type( dwUserUID_, spSquare->GetUID() ));

    return true;
}

bool KCnSquareManager::ExitMinigame( IN DWORD dwUserUID_, IN KCenterPtr spCenter_ )
{
    KCnSquarePtr spSquare = GetUserSquare( dwUserUID_ );

    if( spSquare == NULL )
    {
        return false;
    }

    KSquareUserInfo kUserInfo;
    if( spSquare->GetSquareUserInfo( dwUserUID_, kUserInfo ) == false )
    {
        //헉 미니게임에 입장한 유저 정보가 없음.
        return false;
    }

    spSquare->ExitMinigame( dwUserUID_ );
    spCenter_->QueueingEvent( KCenterEvent::ECN_ENTER_SQUARE_REQ, kUserInfo );

    return true;
}

KCnSquarePtr KCnSquareManager::GetUserSquare( IN DWORD dwUserUID_ )
{
    KLocker lock( m_csSquares );

    std::map<DWORD,DWORD>::iterator mit = m_mapUserInSquare.find( dwUserUID_ );

    if( mit == m_mapUserInSquare.end() )
        return KCnSquarePtr();

    return GetSquare( mit->second );
}

void KCnSquareManager::GetGuildSquareList( IN DWORD dwGuildUID_, IN OUT std::vector<KSquareInfo>& vecSquareList_ )
{
    const int GUILD_SQUARE_NUM = 4;

    if ( dwGuildUID_ == 0 ) return;

    std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
    std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
    SiKCenterServer()->GetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );
    std::vector<KSimpleServerInfo>::iterator vitServer;

    KLocker lock( m_csSquares );

    GuildContainer::iterator mitGuild;
    mitGuild = m_mapGuildSquare.find( dwGuildUID_ );

    if( mitGuild == m_mapGuildSquare.end() )
    {
        for( int i = 0 ; i < GUILD_SQUARE_NUM ; ++i )
        {
            vecSquareList_.push_back( GetBlankGuildSquareInfo( dwGuildUID_, i) );
        }
        return;
    }

    for( int i = 0 ; i < GUILD_SQUARE_NUM ; ++i )
    {
        GuildIndexContainer::iterator mitGuildIndex;

        mitGuildIndex = mitGuild->second.find( i );

        if( mitGuildIndex == mitGuild->second.end() )
        {
            vecSquareList_.push_back( GetBlankGuildSquareInfo( dwGuildUID_, i ) );
        }
        else
        {
            KCnSquarePtr spSquare = GetSquare( mitGuildIndex->second );

            JIF( spSquare );
            
            KSquareInfo kInfo;
            spSquare->GetSquareInfo( kInfo );
            kInfo.m_nGuildSquareIndex = i;

            if( !vecNoUseTCPServerInfo.empty() ) {
                for( vitServer = vecNoUseTCPServerInfo.begin(); vitServer != vecNoUseTCPServerInfo.end(); ++vitServer ) {
                    if( kInfo.m_strIP == vitServer->m_strIP && kInfo.m_usTRPort == vitServer->m_usPort ) {
                        kInfo.m_nStatus = KSquareInfo::SS_CLOSE;
                    }
                }
            }

            vecSquareList_.push_back( kInfo );
        }
    }
}

KSquareInfo KCnSquareManager::GetBlankGuildSquareInfo( IN const DWORD dwGuildUID_, IN const int iIndex_ ) const
{
    KSquareInfo sInfo;
    sInfo.m_dwCurrentUserNum = 0;
    sInfo.m_dwGuildUID = dwGuildUID_;
    sInfo.m_dwID = 0;
    sInfo.m_dwMaxUserNum = 50;
    sInfo.m_dwTRServerIP = 0;
    sInfo.m_nGuildSquareIndex = iIndex_;
    sInfo.m_nType = KSquareInfo::ST_GUILD;
    sInfo.m_usTRPort = 0;
    return sInfo;
}

void KCnSquareManager::DumpSquareList()
{
    std::map<DWORD,KCnSquarePtr>::const_iterator mitSqaure;

    START_LOG( cerr, L"--- DumpSquareList (size : " << m_mapSquares.size() << L") ---"  ) << END_LOG;
    KLocker lock( m_csSquares );
    for ( mitSqaure = m_mapSquares.begin() ; mitSqaure != m_mapSquares.end() ; ++mitSqaure ) {
        START_LOG( cerr, L"Name : " << mitSqaure->second->GetName() 
            << L", UID :" << mitSqaure->second->GetUID()
            << L", Count : " <<mitSqaure->second->GetSize()
            << L", Type :" << mitSqaure->second->GetType() ) << END_LOG;
        mitSqaure->second->DumpInfo();
    }
    START_LOG( cerr, L"--- End List ---"  ) << END_LOG;
}

void KCnSquareManager::AddGuildSquareSetInfo( IN KSquareSetInfo& kInfo_ )
{
    KLocker lock( m_csSquares );

    m_vecGuildSquareSetInfo.push_back( kInfo_ );
}

void KCnSquareManager::Tick()
{
    if ( m_kTimer.CheckTime() == false ) {
        return;
    }

    BuildSquareList( false );
}

bool KCnSquareManager::ForceExitMinigame( IN DWORD dwUserUID_ )
{
    std::map<DWORD,KCnSquarePtr>::iterator mit;

    KLocker lock( m_csSquares );

    for ( mit = m_mapSquares.begin() ; mit != m_mapSquares.end() ; ++mit ) {
        mit->second->ExitMinigame( dwUserUID_ );
    }
    return true;
}

void KCnSquareManager::UserPositionSync( KSquarePos& kPacket_, std::vector<KPosUID>& kPacket )
{
    KCnSquarePtr spSquare;
    spSquare = GetSquare( kPacket_.m_dwSquareID );
    if ( spSquare == NULL ) {
        return;
    }

    spSquare->GetNeighborhoodUserPos( (int)kPacket_.m_fXpos, kPacket );
    
}