#include "Center.h"
#include "CnSquare.h"
#include "NetError.h"
#include "Lua/KLuaManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KCnSquare, KPerformer );
ImplToStringW( KCnSquare )
{
    return START_TOSTRING_PARENTW( KPerformer );
}
KCnSquare::KCnSquare(void)
{
    m_eSquareType = KSquareInfo::ST_NORMAL;
    m_dwMaxUserNum = 0;
    m_dwGuildID = 0;
    m_dwTRServerIP = 0;
    m_usTRPort = 0;
}

KCnSquare::~KCnSquare(void)
{
}

void KCnSquare::GetSquareInfo( OUT KSquareInfo& kInfo )
{
    kInfo.m_dwID = GetUID();
    kInfo.m_strName = GetName();
    kInfo.m_dwCurrentUserNum = GetSize();
    kInfo.m_dwMaxUserNum = m_dwMaxUserNum;
    kInfo.m_nType = m_eSquareType;
    kInfo.m_dwTRServerIP = m_dwTRServerIP;
    kInfo.m_usTRPort = m_usTRPort;
    kInfo.m_strIP = m_strTRServerIP;
}

bool KCnSquare::SetTRAddress( IN DWORD dwIPAddress_, IN unsigned short usPort_ )
{
    _JIF( usPort_ != 0 && usPort_ != 0, return false );
    m_dwTRServerIP  = dwIPAddress_;
    m_usTRPort      = usPort_;
    return true;
}

bool KCnSquare::AddUser( IN const KSquareUserInfo& kUserInfo_ )
{
    _JIF( !IsInSquare( kUserInfo_.m_dwUID ), SET_ERROR( ERR_SQUARE_03 ); return false );

    std::pair< std::map<DWORD,KSquareUserInfo>::iterator, bool > prRet;

    KLocker lock( m_csSquare );

    prRet = m_mapUserList.insert( std::map<DWORD,KSquareUserInfo>::value_type( kUserInfo_.m_dwUID, kUserInfo_ ) );

    return prRet.second;
}
 
bool KCnSquare::RemoveUser( IN DWORD dwUserUID_ )
{
    KLocker lock( m_csSquare );

    std::map<DWORD,KSquareUserInfo>::iterator mit = m_mapUserList.find( dwUserUID_ );
    if( mit == m_mapUserList.end() )
    {
        START_LOG( cwarn, L"이미 없는 유저, UserUID : " << dwUserUID_ ) << END_LOG;
        return false;
    }
    RemoveServer( mit->second.m_dwServerUID );
    m_mapUserList.erase( mit );

    std::map<DWORD,KSquarePos>::iterator mitPos = m_mapUserPosList.find( dwUserUID_ );
    if( mitPos != m_mapUserPosList.end() )
    {
        m_mapUserPosList.erase( mitPos );
    }
    EraseFromContainer( dwUserUID_ );

    return true;
}

bool KCnSquare::IsInSquare( IN const DWORD dwUID_ )
{
    KLocker lock( m_csSquare );
    return ( m_mapUserList.find( dwUID_ ) != m_mapUserList.end() );
}

bool KCnSquare::EnterSquareReserve( IN KSquareUserInfo& kUserInfo_, IN KCenterPtr spCenter_, OUT KECN_ENTER_SQUARE_ACK& kPacket )
{
    GetBestPos( kPacket.m_kMyPos );
    kUserInfo_.m_kSquarePos = kPacket.m_kMyPos;
    if( AddUser( kUserInfo_ ) == false )
    {
        RemoveUser( kUserInfo_.m_dwUID );
        return false;
    }

    GetSquareUserInfo( kPacket.m_vecUserInfo );
    GetRelayServerInfo( kPacket );

    AddServer( spCenter_ );
    QueueingToAll( KCenterEvent::ECN_NEW_SQUARE_USER_NOT, kUserInfo_ );

    return true;
}

void KCnSquare::GetSquareUserInfo( OUT std::vector< KSquareUserInfo >& m_vecUserInfo_ )
{
    m_vecUserInfo_.clear();
    m_vecUserInfo_.reserve( GetSize() );

    std::map<DWORD,KSquareUserInfo>::iterator mit;
    KLocker lock( m_csSquare );
    for( mit = m_mapUserList.begin() ; mit != m_mapUserList.end() ; ++mit )
    {
        KSquareUserInfo kInfo;
        kInfo = mit->second;
        m_vecUserInfo_.push_back( kInfo );
    }

}

// 미니게임유저리스트 에서 찾는 것인데 함수 이름이 잘못된 듯.
bool KCnSquare::GetSquareUserInfo( IN DWORD dwUserUID_, OUT KSquareUserInfo& kUserInfo_ )
{
    std::map<DWORD,KSquareUserInfo>::iterator mit = m_mapMinigameUserList.find( dwUserUID_ );

    if( mit == m_mapMinigameUserList.end() )
    {
        START_LOG( cerr, L"미니게임 유저 리스트에 유저 없음, UID : " << dwUserUID_ ) << END_LOG;
            
        //헉 왜 없지!!!
        return false;
    }

    kUserInfo_ = mit->second;
    return true;
}

void KCnSquare::GetRelayServerInfo( OUT KECN_ENTER_SQUARE_ACK& kInfo_ )
{
    kInfo_.m_dwRelayServerIP    = m_dwTRServerIP;
    kInfo_.m_usRelayServerPort  = m_usTRPort;
}

void KCnSquare::AddServer( IN KCenterPtr spCenter_ )
{
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mitServer;

    KLocker lock( m_csSquare );

    mitServer = m_mapServers.find( spCenter_->GetUID() );
    if( mitServer == m_mapServers.end() )
    {
        mitServer = m_mapServers.insert(std::map<DWORD,std::pair<KCenterPtr,int>>::value_type(spCenter_->GetUID(),std::pair<KCenterPtr,int>(spCenter_,0)) ).first;
    }

    mitServer->second.first = spCenter_;
    ++(mitServer->second.second);
}

void KCnSquare::RemoveServer( IN DWORD dwUID )
{
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mitServer;

    KLocker lock( m_csSquare );

    mitServer = m_mapServers.find( dwUID );
    if( mitServer == m_mapServers.end() )
    {
        return;
    }

    --(mitServer->second.second);

    if( mitServer->second.second <= 0 )
    {
        m_mapServers.erase( mitServer );
    }
}
void KCnSquare::UpdateUserPos( IN KSquarePos& kPos_ )
{
    // 해당 유저가 존재하는지 확인한다.
    __JIF( IsInSquare( kPos_.m_dwUID ), SET_ERROR( ERR_SQUARE_04 ); return );

    m_mapUserPosList[kPos_.m_dwUID] = kPos_;
    EraseFromContainer( kPos_.m_dwUID );
    InsertContainer( (int)kPos_.m_fXpos, kPos_.m_dwUID );
}

void KCnSquare::InsertContainer( IN int nIndex_, IN DWORD dwUserUID_ )
{
    KLocker lock( m_csSquare );
    m_mapContainer[nIndex_].push_back( dwUserUID_ );
}

bool KCnSquare::EraseFromContainer( IN DWORD dwUserUID_ )
{
    bool bOk = false;
    std::map<int, std::vector<DWORD> >::iterator mit;

    KLocker lock( m_csSquare );

    for( mit = m_mapContainer.begin() ; mit != m_mapContainer.end() ; ++mit )
    {
        std::vector<DWORD>::iterator vit;
        vit = std::find( mit->second.begin(), mit->second.end(),
            dwUserUID_ );
        if( vit != mit->second.end() )
        {
            mit->second.erase( vit );
            bOk = true;
        }
    }
    return bOk;
}

void KCnSquare::GetNeighborhood( int nIndex_, std::vector<DWORD>& vecUserUIDs_ )
{
    int nStart = nIndex_ - NEAR_NUM;
    int nEndIndex = nIndex_ + NEAR_NUM;

    for( int i = nStart ; i <= nEndIndex ; ++i )
    {
        AppendContainerUser( i, vecUserUIDs_ );
    }
}

void KCnSquare::AppendContainerUser( int nIndex_, std::vector<DWORD>& vecUserUIDs_ )
{
    std::map<int, std::vector<DWORD> >::iterator mit;

    KLocker lock( m_csSquare );

    mit = m_mapContainer.find( nIndex_ );
    if( mit != m_mapContainer.end() )
    {
        std::vector<DWORD>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            vecUserUIDs_.push_back( *vit );
        }
    }
}

//namespace{
//    float GetDistance( KSquarePos& left, KSquarePos& right )
//    {
//        float deltaX = abs( left.m_fXpos - right.m_fXpos );
//        float deltaY = abs( left.m_fYpos - right.m_fYpos );
//        return deltaX + deltaY;
//    }
//}
//
//void KCnSquare::GetNeighborhood( IN DWORD dwUserUID, OUT std::vector<DWORD>& vecNeighbor )
//{
//    static const float NEAR_DISTANCE = 2.0f;    // 1.0 은 약 400픽셀
//
//    std::map<DWORD,KSquarePos>::iterator mit;
//
//    KLocker lock( m_csSquare );
//
//    KSquarePos& kUserPos = m_mapUserPosList[dwUserUID];
//    for( mit = m_mapUserPosList.begin() ; mit != m_mapUserPosList.end() ; ++mit )
//    {
//        if( mit->first == dwUserUID )
//            continue;
//
//        if( GetDistance( kUserPos, mit->second ) > NEAR_DISTANCE )
//            continue;
//
//        vecNeighbor.push_back( mit->first );
//    }
//}

bool KCnSquare::LoadInfo( const char* szTableName_ )
{
    std::vector< KSquarePos > vecPos;
    vecPos.clear();

    _JIF( szTableName_ != NULL, return false );
    KLuaManager kLuaMng;
    _JIF( kLuaMng.DoFile( "SquareInfo.lua" ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( szTableName_ ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "PosInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        KSquarePos kPos;
        kPos.m_dwSquareID = GetUID();
        kPos.m_dwUID = 0;               // 초기화
        _JIF( kLuaMng.GetValue( 1, kPos.m_fXpos ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, kPos.m_fYpos ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        vecPos.push_back( kPos );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( clog, L"광장내 유저 생성 위치 정보 로드 완료.. : "
        << KncUtil::toWideString( szTableName_ ) << L" : " << vecPos.size() )  << END_LOG;

    KLocker lock( m_csSquare );
    m_vecSpawnPos = vecPos;
    return true;
}

void KCnSquare::GetBestPos( KSquarePos& kPos_ )
{
    if( m_vecSpawnPos.empty() )
    {
        return;
    }

    kPos_ = m_vecSpawnPos[ rand() % m_vecSpawnPos.size()];

    START_LOG( clog, L"광장 유저 위치 결정")
        << BUILD_LOG( kPos_.m_dwSquareID )
        << BUILD_LOG( kPos_.m_fXpos )
        << BUILD_LOG( kPos_.m_fYpos )
        << END_LOG;
    //DWORD dwUserNum = 0xffffffff;
    //std::vector<KSquarePos>::iterator vit;
    //std::map<int, std::vector<KUserPtr> >::iterator mit;
    //for( vit = m_vecSpawnPos.begin() ; vit != m_vecSpawnPos.end() ; ++vit )
    //{
    //    int nIndex = GetIndex( *vit );
    //    mit = m_mapContainer.find( nIndex );
    //    if( mit == m_mapContainer.end() )
    //    {
    //        kPos_ = *vit;
    //        return;
    //    }

    //    if( dwUserNum > mit->second.size() )
    //    {
    //        dwUserNum = mit->second.size();
    //        kPos_ = *vit;
    //    }
    //}
}

void KCnSquare::GetNeighborhoodEachServer( IN DWORD dwUserUID, OUT std::map< DWORD,std::vector<DWORD> >& mapNeighborhood_ )
{
    static const float NEAR_DISTANCE = 2.0f;    // 1.0 은 약 400픽셀

    KLocker lock( m_csSquare );

    KSquarePos& kUserPos = m_mapUserPosList[dwUserUID];

    std::vector<DWORD> vecNeighborhood;
    GetNeighborhood( (int)kUserPos.m_fXpos, vecNeighborhood );

    std::vector<DWORD>::iterator vit;
    for( vit = vecNeighborhood.begin() ; vit != vecNeighborhood.end() ; ++vit )
    {
        //if( (*vit) == dwUserUID ) // '나'는 제외
        //    continue;

        if( m_mapUserList.find( *vit ) == m_mapUserList.end() )
            continue;

        KSquareUserInfo& kUser = m_mapUserList[*vit];
        mapNeighborhood_[kUser.m_dwServerUID].push_back( *vit );

        START_LOG( clog, L"내 이웃(" << dwUserUID << L") : " << (*vit) << L", " << kUser.m_dwServerUID ) << END_LOG;
    }
}

void KCnSquare::GetAllUserEachServer( IN DWORD dwUserUID, OUT std::map< DWORD,std::vector<DWORD> >& mapNeighborhood_ )
{
    KLocker lock( m_csSquare );

    std::map<DWORD,KSquareUserInfo>::iterator mitUser;

    for ( mitUser = m_mapUserList.begin() ; mitUser != m_mapUserList.end() ; ++mitUser ) 
    {
        KSquareUserInfo& kUser = mitUser->second;
        mapNeighborhood_[kUser.m_dwServerUID].push_back( mitUser->first );
    }

    START_LOG( clog, L"광장 전체유저 size : " << m_mapUserList.size() )
        << BUILD_LOG( mapNeighborhood_.size() ) << END_LOG;
}

bool KCnSquare::IsEmpty()
{
    KLocker lock( m_csSquare );
    return m_mapUserList.empty();
}

bool KCnSquare::EnterMinigame( IN DWORD dwUserUID_ )
{
    KLocker lock( m_csSquare );

    std::map<DWORD,KSquareUserInfo>::iterator mit = m_mapUserList.find( dwUserUID_ );

    if( mit == m_mapUserList.end() )
    {
        START_LOG( cerr, L"미니게임 입장시 유저가 광장에 없음, User UID : " << dwUserUID_ ) << END_LOG;
        return false;
    }

    m_mapMinigameUserList[mit->first] = mit->second;
    return true;
}

bool KCnSquare::ExitMinigame( IN DWORD dwUserUID_ )
{
    KLocker lock( m_csSquare );

    std::map<DWORD,KSquareUserInfo>::iterator mit = m_mapMinigameUserList.find( dwUserUID_ );

    if( mit == m_mapMinigameUserList.end() )
        return false;

    m_mapMinigameUserList.erase( mit );
    return true;
}

void KCnSquare::GetNeighborhoodUserPos( int nIndex_, std::vector<KPosUID>& vecUserPos_ )
{
    int nStart = nIndex_ - NEAR_NUM;
    int nEndIndex = nIndex_ + NEAR_NUM;

    for ( int i = nStart ; i <= nEndIndex ; ++i ) {
        AppendContainerUserPos( i, vecUserPos_ );
    }
}

void KCnSquare::AppendContainerUserPos( int nIndex_, OUT std::vector<KPosUID>& vecUserPos_ )
{
    vecUserPos_.clear();
    std::map<int, std::vector<DWORD> >::iterator mit;
    std::map<DWORD,KSquarePos>::iterator mitPos;

    KLocker lock( m_csSquare );

    mit = m_mapContainer.find( nIndex_ );

    if ( mit != m_mapContainer.end() ) {

        std::vector<DWORD>::iterator vit;
        
        for ( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit ) {
            mitPos = m_mapUserPosList.find( *vit );

            if ( mitPos == m_mapUserPosList.end() ) {
                continue;
            }

            KPosUID kInfo;
            kInfo.m_dwUID = mitPos->first;
            kInfo.m_fXPos = mitPos->second.m_fXpos;
            kInfo.m_fYPos = mitPos->second.m_fYpos;

            vecUserPos_.push_back( kInfo );
        }
    }
    START_LOG( clog, L"광장 위치 동기화에서 이웃 유저들 size : " << vecUserPos_.size() ) << END_LOG;
}

void KCnSquare::DumpInfo()
{
    KLocker lock( m_csSquare );

    START_LOG( cerr, L"--- Dump CnSquareInfo ---" << dbg::endl
        << L"SquareType : " << m_eSquareType << dbg::endl
        << L"MaxUserNum : " << m_dwMaxUserNum << dbg::endl
        << L"GuildID : " << m_dwGuildID << dbg::endl
        << L"TRServerIP : " << m_dwTRServerIP << dbg::endl
        << L"TRPort : " << m_usTRPort ) << END_LOG;

    START_LOG( cerr, L"m_mapUserList.size() : " << m_mapUserList.size()  ) << END_LOG;
    START_LOG( cerr, L"m_mapServers.size() : " << m_mapServers.size()  ) << END_LOG;
    std::map<DWORD,std::pair<KCenterPtr,int> >::const_iterator cmit;
    for ( cmit = m_mapServers.begin() ; cmit != m_mapServers.end() ; ++cmit ) {
        START_LOG( cerr, L"ServerID : " << cmit->first << L", UserCount : " << cmit->second.second  ) << END_LOG;
    }
    START_LOG( cerr, L"--- End Dump ---"  ) << END_LOG;
}

void KCnSquare::SetTRServerIP( IN const std::wstring& strIP_ )
{
    m_strTRServerIP = strIP_;
}