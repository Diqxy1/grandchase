#include "User.h"
#include "Square.h"
#include "UserFSM.h"
#include <boost/mem_fn.hpp>
#include <boost/bind.hpp>
#include "Lua/KLuaManager.h"
#include "CnConnector.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG
#define EVENT_TYPE  KUserEvent
#define CLASS_TYPE  KSquare

#define QUEUING_TO_ALL( id, data )  QueueingToAll( EVENT_TYPE::id, data )
#define SEND_TO_ALL( id, data )     SendToAll( EVENT_TYPE::id, data )

#define USER_PAGE_SIZE  7

NiImplementRTTI( KSquare, KPerformer );
ImplToStringW( KSquare )
{
    return START_TOSTRING_PARENTW( KPerformer );
}

KSquare::KSquare(void)
:m_dwGuildUID(0)
,m_nGuildSquareIndex(0)
,m_dwTRServerIP(0)
,m_usTRPort(0)
,m_dwMaxUserNum(0)
{
    m_mapUsers.clear();
    m_mapContainer.clear();
    m_vecSpawnPos.clear();
    m_vecSquareUserList.clear();
    m_vecSquareAllUserList.clear();
    m_mapMiniGameUsers.clear();
    m_eType = KSquareInfo::ST_NORMAL;
}

KSquare::~KSquare(void)
{
}

void KSquare::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );
    switch( spEvent_->m_usEventID )
    {
    case 1: break;
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

void KSquare::Tick()
{
    KPerformer::Tick();// 이 안에서 ProcessEvent 호출이 일어 난다.
}

KUserPtr KSquare::GetUserPtr( const DWORD dwUID_ )
{
    std::map<DWORD,KUserPtr>::iterator mit;
    KLocker lock( m_csUsers );
    mit = m_mapUsers.find( dwUID_ );
    _JIF( mit != m_mapUsers.end(), return KUserPtr() );

    return mit->second;
}

bool KSquare::LoadInfo( const char* szTableName_ )
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
        _JIF( kLuaMng.GetValue( 1, kPos.m_fXpos ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, kPos.m_fYpos ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        vecPos.push_back( kPos );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"광장내 유저 생성 위치 정보 로드 완료.. : "
        << KncUtil::toWideString( szTableName_ ) << L" : " << vecPos.size() )  << END_LOG;

    KLocker lock( m_csUsers );
    m_vecSpawnPos = vecPos;
    return true;
}

void KSquare::GetInfo( KSquareInfo& kInfo_ )
{
    kInfo_.m_dwID               = GetUID();
    kInfo_.m_strName            = GetName();
    kInfo_.m_dwMaxUserNum       = m_dwMaxUserNum;
    kInfo_.m_dwCurrentUserNum   = GetSize();
    kInfo_.m_nType              = GetType();
    kInfo_.m_dwGuildUID         = GetGuildUID();
    kInfo_.m_nGuildSquareIndex  = GetGuildSquareIndex();
}

void KSquare::EnterSquareReserve( IN OUT KUserPtr kUser_ )
{
    _JIF( AddUser( kUser_ ), SET_ERROR( ERR_SQUARE_03 ); return );
    kUser_->StateTransition( KUserFSM::INPUT_TO_SQUARE_JOIN );
    kUser_->m_spSquare = std::static_pointer_cast<KSquare>(shared_from_this());

    SET_ERROR( NET_OK );
}

bool KSquare::AddUser( IN KUserPtr kUser_ )
{
    KLocker lock( m_csUsers );
    std::pair< std::map<DWORD,KUserPtr>::iterator, bool > prRet; // insert에 성공하면 true ? 였던 것으로 기억나는..
    prRet = m_mapUsers.insert( std::map<DWORD,KUserPtr>::value_type( kUser_->GetUID(), kUser_ ) );
    return prRet.second;
}

void KSquare::GetSquareUserInfo( OUT std::vector< KSquareUserInfo >& m_vecUserInfo_ )
{
    m_vecUserInfo_.clear();
    m_vecUserInfo_.reserve( GetSize() );

    std::map<DWORD,KUserPtr>::iterator mit;
    KLocker lock( m_csUsers );
    for( mit = m_mapUsers.begin() ; mit != m_mapUsers.end() ; ++mit )
    {
        KSquareUserInfo kInfo;
        if( !mit->second->GetSquareUserInfo( kInfo ) ) continue;
        kInfo.m_dwSquareID = GetUID(); // 광장의 ID ( or UID )
        m_vecUserInfo_.push_back( kInfo );
    }
}

bool KSquare::IsInSquare( IN const DWORD dwUID_ )
{
    KLocker lock( m_csUsers );
    return ( m_mapUsers.find( dwUID_ ) != m_mapUsers.end() );
}

bool KSquare::SetTRAddress( DWORD dwIPAddress_, USHORT usPort_ )
{
    _JIF( usPort_ != 0 && usPort_ != 0, return false );
    m_dwTRServerIP  = dwIPAddress_;
    m_usTRPort      = usPort_;
    return true;
}

bool KSquare::LeaveSquare( DWORD dwUID_ )
{
    bool bOK = false;
    __JIF( IsInSquare( dwUID_ ), return true );
    bOK = RemoveUser( dwUID_ );
    
    return bOK;
}

bool KSquare::RemoveUser( IN const DWORD dwUID_ )
{
    KLocker lock( m_csUsers );
    m_mapUsers.erase( dwUID_ );
    return true;
}

std::wstring KSquare::GetAddress()
{
    in_addr address;
    address.s_addr = m_dwTRServerIP;
    return KncUtil::toWideString( ::inet_ntoa( address ) );
}



void KSquare::Send_NotMsg( std::wstring& strMsg_ )
{
    KChatData kPacket;
    kPacket.m_cChatType = KChatData::MSG_NOTIFY;
    kPacket.m_strChatMsg = strMsg_;
    SEND_TO_ALL( EVENT_CHAT_NOT, kPacket );
}

void KSquare::BuildUserList()
{
    std::vector<KInviteUserInfo> vecSquareUserList;
    std::vector<KInviteUserInfo> vecSquareAllUserList;

    vecSquareUserList.reserve( m_mapUsers.size() );
    vecSquareAllUserList.reserve( m_mapUsers.size() );

    std::map<DWORD,KUserPtr>::iterator mit = m_mapUsers.begin();

    for( mit = m_mapUsers.begin() ; mit != m_mapUsers.end() ; ++mit )
    {
        KInviteUserInfo kAllInviteInfo;
        KInviteUserInfo kInviteInfo;

        // 전체 목록 작성(GM툴용)
        if( !mit->second->GetInviteInfo( kAllInviteInfo, true ) )
            continue;

        vecSquareAllUserList.push_back( kAllInviteInfo );

        // 실제 목록 작성
        if( !mit->second->GetInviteInfo( kInviteInfo ) )
            continue;

        vecSquareUserList.push_back( kInviteInfo );
    }

    if( !vecSquareAllUserList.empty() )
    {
        std::sort( vecSquareAllUserList.begin(), vecSquareAllUserList.end(),
            boost::bind(&KInviteUserInfo::m_strNickName, _1) < boost::bind(&KInviteUserInfo::m_strNickName, _2) );
    }

    if( !vecSquareUserList.empty() )
    {
        std::sort( vecSquareUserList.begin(), vecSquareUserList.end(),
            boost::bind(&KInviteUserInfo::m_strNickName, _1) < boost::bind(&KInviteUserInfo::m_strNickName, _2) );
    }

    m_vecSquareAllUserList.clear();
    m_vecSquareUserList.clear();
    //광장유저목록압축
    CompressUserList( vecSquareAllUserList, m_vecSquareAllUserList );
    CompressUserList( vecSquareUserList, m_vecSquareUserList );
}

void KSquare::GetUserList( IN int nPage_, OUT KPageInfo& kPageUser_ )
{
    if( nPage_ < 0 )
        nPage_ = 0;

    if( m_vecSquareUserList.empty() )
    {
        SET_ERROR( ERR_LIST_00 );
        return;
    }

    std::vector<KListPage>::iterator vit;
    vit = std::find_if( m_vecSquareUserList.begin(), m_vecSquareUserList.end(),
        boost::bind( &KListPage::m_nPageNum, _1 ) == nPage_ );

    if( vit == m_vecSquareUserList.end() )
    {
        --vit;
    }

    kPageUser_.m_nCurrentPage = vit->m_nPageNum;
    kPageUser_.m_nMaxPage = (int)m_vecSquareUserList.size();
    kPageUser_.m_buffCompList = vit->m_buffComList;
    kPageUser_.m_nOK = 0;
    SET_ERROR( NET_OK );
}

bool KSquare::StartMiniGame( DWORD dwUID_ ) // 미니게임을 시작한다. User에서 미니게임 시작 패킷이 오면 호출
{
    bool bOK = false;
    KUserPtr spUser = GetUserPtr( dwUID_ );
    
    {
        KLocker lock( m_csMiniGameUsers );
        // 미니게임에 유저가 이미 들어있지 않은지 체크하기
        if( m_mapMiniGameUsers.end() != m_mapMiniGameUsers.find( dwUID_ ) )
        {
            SET_ERROR( ERR_MINIGAME_03 );
            return bOK;
        }

        if ( spUser ) {
            m_mapMiniGameUsers.insert( std::make_pair( dwUID_, spUser ) );    
        }
        else {
            SET_ERROR( ERR_MINIGAME_00 );
            return bOK;
        }
        
        //START_LOG( clog, L"StartMiniGame , map size = " << m_mapMiniGameUsers.size()  ) << END_LOG;
    }
    //std::map<DWORD, KUserPtr>::iterator mit;
    //for( mit = m_mapMiniGameUsers.begin(); mit != m_mapMiniGameUsers.end() ; ++mit )
    //{
    //    START_LOG( clog, L"User UID : " << mit->first << ", Name : " << mit->second->GetName()  ) << END_LOG;
    //}
    // 미니게임에 일단 유저 포인터를 넣고, 광장에서 제거 -_-

    __JIF( IsInSquare( dwUID_ ), return true ); // 유저가 광장에 있는가
    bOK = RemoveUser( dwUID_ ); // 광장에서 제거한다.

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_ENTER_MINIGAME_NOT, dwUID_ );
    return bOK;
}

bool KSquare::IsInMiniGame( IN const DWORD dwUID_ )
{
    KLocker lock( m_csMiniGameUsers );
    
    START_LOG( clog, L"IsInMiniGame, 미니게임 유저 검색, " << dwUID_  ) << END_LOG;

    return ( m_mapMiniGameUsers.find( dwUID_ ) != m_mapMiniGameUsers.end() );
}

bool KSquare::RemoveMiniGameUser( IN const DWORD dwUID_ )
{
    KLocker lock( m_csUsers );
    m_mapMiniGameUsers.erase( dwUID_ );
    
    //std::clog << L"미니게임 유저 삭제 후 미니게임유저수 : " << m_mapMiniGameUsers.size();

    if( m_mapMiniGameUsers.end() == m_mapMiniGameUsers.find( dwUID_ ) )
        return true;

    return false;
}

bool KSquare::BackToSquareFromMiniGame( IN KUserPtr kUser_, OUT KEVENT_ENTER_SQUARE_ACK& kPacket_ ) // 미니게임에서 광장으로 가기
{
    __JIF( IsInMiniGame( kUser_->GetUID() ), SET_ERROR( ERR_MINIGAME_00 ); return false ); // 미니게임에 유저 체크
    bool bOK = RemoveMiniGameUser( kUser_->GetUID() );
    if( !bOK )
    {
        //std::cout << L"미니게임에서 유저 삭제 실패 : " << kUser_->GetUID() << dbg::endl;
    }

    _JIF( !IsInSquare( kUser_->GetUID() ), SET_ERROR( ERR_SQUARE_03 ); return false );
    _JIF( AddUser( kUser_ ), SET_ERROR( ERR_SQUARE_03 ); return false );
    kUser_->StateTransition( KUserFSM::INPUT_TO_SQUARE_JOIN );
    GetSquareUserInfo( kPacket_.m_vecUserInfo );

    KSquarePos kPos;    
    kUser_->GetSquarePos( kPos.m_fXpos, kPos.m_fYpos ); // 미니게임 시작할 때의 광장에서의 유저 위치가 있을 것
    kUser_->SetSquarePos( kPos.m_fXpos, kPos.m_fYpos );

    //kUser_.m_spSquare = std::static_pointer_cast<KSquare>(shared_from_this());
    // 미니 게임 할 때 아직은 광장 포인터를 초기화 하지 않고 있다. 그렇다면.. 새로 넣어줄 필요 없는가
    kPacket_.m_dwRelayServerIP      = m_dwTRServerIP;
    kPacket_.m_usRelayServerPort    = m_usTRPort;

    // 모든 광장 유저에게 나의 입장을 알린다.
    //DWORD kPacket = kUser_->GetUID() ;
    //QUEUING_TO_ALL( EVENT_NEW_SQUARE_USER_NOT, kPacket ); // Queuing 하는 이유는 로딩중이 유저에게 킵 하게 하기 위해..
    SET_ERROR( NET_OK );
    return true;
}

bool KSquare::SendIDTo( const DWORD dwUID_, const unsigned short usEventID_ )
{
    KUserPtr spUser =  GetUserPtr( dwUID_ );
    _JIF( spUser, return false );
    return spUser->SendID( usEventID_ );
}

void KSquare::SendIDToAll( const unsigned short usEventID_ )
{
    std::map<DWORD,KUserPtr>::iterator mit;
    KLocker lock( m_csUsers );
    for( mit = m_mapUsers.begin(); mit != m_mapUsers.end(); mit++ )
    {
        mit->second->SendID( usEventID_ );
    }
}

void KSquare::SendIDToAllExMe( const DWORD dwUID_, const unsigned short usEventID_ )
{
    std::map<DWORD,KUserPtr>::iterator mit;
    KLocker lock( m_csUsers );
    for( mit = m_mapUsers.begin(); mit != m_mapUsers.end(); mit++ )
    {
        if( mit->second->GetUID() == dwUID_ ) continue;
        mit->second->SendID( usEventID_ );
    }
}

void KSquare::CompressUserList( IN std::vector<KInviteUserInfo>& vecUserList_, OUT std::vector<KListPage>& vecUserListPage_ )
{
    KSerializer ks;
    KListPage kPage;
    kPage.m_buffComList.Clear();

    std::vector<KInviteUserInfo> vecTmp;
    vecTmp.reserve( USER_PAGE_SIZE );

    int nPageNum = 0;

    std::vector<KInviteUserInfo>::iterator vit;
    for( vit = vecUserList_.begin() ; vit != vecUserList_.end() ; ++vit )
    {
        vecTmp.push_back( *vit );
        if( vecTmp.size() == USER_PAGE_SIZE )
        {
            kPage.m_buffComList.Clear();

            ks.BeginWriting( &kPage.m_buffComList );
            ks.Put( vecTmp );
            ks.EndWriting();

            kPage.m_nPageNum = nPageNum;
            kPage.m_buffComList.Compress();
            vecUserListPage_.push_back( kPage );

            vecTmp.clear();
            vecTmp.reserve( USER_PAGE_SIZE );
            kPage.m_buffComList.Clear();

            ++nPageNum;
        }
    }

    if( !vecTmp.empty() ) // 유저가 있다면 (한페이지를 채우지 않지만)
    { // 이 부분은 없었는데 룸매니저를 보고 따라함.. 검토 부탁드립니다.
        KListPage kPage;
        ks.BeginWriting( &kPage.m_buffComList );
        ks.Put( vecTmp );
        ks.EndWriting();

        kPage.m_nPageNum = nPageNum;
        kPage.m_buffComList.Compress();
        vecUserListPage_.push_back( kPage );

        vecTmp.clear();
        vecTmp.reserve( USER_PAGE_SIZE );
        kPage.m_buffComList.Clear();
    }
}

void KSquare::GetAllUserList( IN int nPage_, OUT KPageInfo& kPageUser_ )
{
    if( nPage_ < 0 )
        nPage_ = 0;

    if( m_vecSquareAllUserList.empty() )
    {
        SET_ERROR( ERR_LIST_00 );
        return;
    }

    std::vector<KListPage>::iterator vit;
    vit = std::find_if( m_vecSquareAllUserList.begin(), m_vecSquareAllUserList.end(),
        boost::bind( &KListPage::m_nPageNum, _1 ) == nPage_ );

    if( vit == m_vecSquareAllUserList.end() )
    {
        --vit;
    }

    kPageUser_.m_nCurrentPage = vit->m_nPageNum;
    kPageUser_.m_nMaxPage = (int)m_vecSquareAllUserList.size();
    kPageUser_.m_buffCompList = vit->m_buffComList;
    kPageUser_.m_nOK = 0;
    SET_ERROR( NET_OK );
}

bool KSquare::SendRelayTo( const DWORD dwUID_, const unsigned short usEventID_, KSerBuffer& data_ )
{
    KUserPtr spUser =  GetUserPtr( dwUID_ );
    START_LOG( clog, L"광장 릴레이 데이터" )
        << BUILD_LOG( dwUID_ ) << END_LOG;

    if( !spUser ) return false;
        
    return spUser->SendPacket( data_, KUserEvent::GetEventIDString( usEventID_ ) );
}

bool KSquare::IsEmpty()
{
    KLocker lock( m_csUsers );
    return m_mapUsers.empty();
}