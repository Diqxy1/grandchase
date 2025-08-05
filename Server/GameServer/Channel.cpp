#include "User.h"
#include "Channel.h"
#include "UserEvent.h"
#include "UserFSM.h"
#include "GSSimLayer.h"
#include "IndigoUtil.h"
#include "GameServer.h"
#include <KncUtilLua.h> // boost::bind
#include "ChannelManager.h"
#include "GCHelper.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG
namespace
{
    class KComp
    {
    public:
        bool operator() ( const KInviteUserInfo a, const KInviteUserInfo b )
        {
            if( a.m_strNickName < b.m_strNickName ) return true;
            return false;
        }
    };
};

#define USER_PAGE_SIZE  7

NiImplementRTTI( KChannel, KSimObject );

KChannelPtr KChannel::ms_spChannelLobby;
unsigned int KChannel::ms_uiSendRoomListGap( 2000 );
unsigned int KChannel::ms_uiSendUserListGap( 3000 );

KChannel::KChannel()
:m_cType( CT_UNLIMITED )
{
    m_uiLastSendRoomList = ::GetTickCount();
    m_uiLastSendUserList = ::GetTickCount();
    m_prIPRange          = std::pair<DWORD,DWORD>( 0x00000000, 0xFFFFFFFF );
    m_prGradeRange       = std::pair<int,int>( 1, 12 );

    vecDummyStatus.reserve( 4 );
    for ( int i = 0 ; i < 4 ; ++ i ) {
        KGameModeState status;
        status.m_nDifficulty = i;
        vecDummyStatus.push_back( status );
    }
}

KChannel::~KChannel()
{
}

ImplOstmOperatorW( KChannel );
ImplToStringW( KChannel )
{

    stm_ << GetRTTI()->GetName()
        << L"ID (" << GetUID() << L") "
        << L", Name : " << GetChannelName() 
        << L", ( " << m_mapSob.size() << L" / " << m_usMaxUsers << L" )"
        << L", Type : " << (int)m_cType;
    if( m_spRoomManager )
        m_spRoomManager->DumpInfo( stm_ );
    
    return stm_<< std::endl;
        
}

void KChannel::SetRoomNum( int nRoomNum_ )
{
    if( nRoomNum_ > 0 )
    {
        if( m_spRoomManager == NULL )
            m_spRoomManager.reset( new KRoomManager( (unsigned short)nRoomNum_ ) );
        else
            m_spRoomManager->ReserveRoomObject( (unsigned short)nRoomNum_ );
    }
}

bool KChannel::IsCorrectIPRange( DWORD dwIP_ )
{
    KLocker lock( m_csChannelInfo );
    return (m_prIPRange.first <= dwIP_ && m_prIPRange.second >= dwIP_);
}

bool KChannel::Enter( KUserPtr::element_type& kUser_ )
{
    // 071122. kkurrung. 채널 입장 제한 풀렸음. 
    //if( !kUser_.IsAdmin() && GetMaxUser() <= GetCount() ) // 영자가 아니고, 정원이 다 찬 경우
    //{
    //    SET_ERROR( ERR_CHANNEL_02 );
    //    return false;
    //}
    if( this != ms_spChannelLobby.get() &&  // 채널 로비가 아니고.
        !kUser_.IsAdmin() &&                // 문영자가 아니고.
        !IsCorrectIPRange( ::ntohl( kUser_.GetIP() ) ) )    // IP 대역이 맞지 않으면.
    {
        SET_ERROR( ERR_CHANNEL_13 ); //IP 대역이 옳지 않음.
        return false;
    }

    KNGuildInfo kGuildInfo;

    const KCharacterInfo* pkCharacterInfo = kUser_.GetCurrentCharacter();
    LIF( pkCharacterInfo != NULL );

    switch( GetType() ) {
    case CT_DUNGEON: // 일반 채널

        //한국에서만 사용...
    case CT_UNLIMITED:  // 대전 퀘스트 모두 할수 있고 레벨 제한이 없는 채널.
    case CT_BEGINNER:   // 동 떠돌이기사단  ~ 은 떠돌이기사단
    case CT_ROOKIE:     // 금 떠돌이기사단  ~ 은 용병기사단
    case CT_EXPERT:     // 금 용병기사단    ~ 동 왕립기사단
    case CT_MASTER:     // 은 왕립기사단    ~ 금 신의기사단
    case CT_AUTOMATCH:  // 레벨 제한없고 대전 매칭으로 플레이 하는 채널.
        if( !kUser_.IsAdmin() && !CheckEnteranceGrade( kUser_.m_cGrade ) )
        {
            SET_ERROR( ERR_CHANNEL_08 ); // 입장 가능한 레벨이 아님
            return false;
        }
        break;

    case CT_INDIGO: // indigo 채널
        if( !SiKServerOpenTime()->CheckEntranceEnable() )
        {
            SET_ERROR( ERR_CHANNEL_07 );
            return false;
        }

        if ( !kUser_.GetBestCharacter() ) {
            SET_ERROR( ERR_CHANNEL_04 );
            return false;
        }

        if( !kUser_.IsAdmin() && kUser_.GetBestCharacter()->m_dwLevel < SiKGSSimLayer()->m_dwIndigoLevel )
        {
            START_LOG_WITH_NAME( clog )
                << BUILD_LOG( SiKGSSimLayer()->m_dwIndigoLevel )
                << BUILD_LOG( kUser_.GetBestCharacter()->m_dwLevel ) << END_LOG;

            SET_ERROR( ERR_CHANNEL_04 );
            return false;
        }

        if( !kUser_.IsAdmin() && SiKGSSimLayer()->m_bCheckIndigoTicket && !kUser_.CheckIfAllItemExist( KUser::INDIGO_TICKET_ID, -1 ) )    // 입장권이 없음
        {
            SET_ERROR( ERR_CHANNEL_06 );
            return false;
        }

        if( !kUser_.IsAdmin() && kUser_.m_iRP <= 0 )         // RP가 0 이하인 경우
        {
            SET_ERROR( ERR_CHANNEL_05 );
            return false;
        }

        if( kUser_.IsExpAccount() ) {                   // 체험계정으로 용사의 섬 입장 불가.
            SET_ERROR( ERR_CHANNEL_11 );
            return false;
        }
        break;

    case CT_GUILD_UNLIMITED:
    case CT_GUILD_MASTER:
    case CT_GUILD_EXPERT:
    case CT_GUILD_ROOKIE:
    case CT_GUILD_BEGINNER:
        //KGuildPtr spGuild = SiKGuildManager()->GetGuild( kUser_.m_kGuildUserInfo.m_dwGuildUID );

        kUser_.GetGuildInfo( kGuildInfo ); //spGuild->GetNGuildInfo();
        if ( !kUser_.IsAdmin() && !CheckGuildEnteranceGrade( kGuildInfo.m_GuildBattlePoint ) ) {
            SET_ERROR( ERR_CHANNEL_14 );
            return false;
        }
            
        break;
    default:
        SET_ERROR( ERR_UNKNOWN );
        START_LOG( cerr, L"Invalid ChannelType : " << (int)GetType() )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kUser_.GetName() ) << END_LOG;
        return false;
    } // end switch

    if( kUser_.GetChannel() != NULL ) {
        kUser_.GetChannel()->Leave( kUser_ );
    }

    {
        KUserPtr spkUser = FindUser( kUser_.GetName() );
        if ( spkUser ) {
            Leave( kUser_ );
        }
    }

    if( !KChannel::KSobManager<KUserPtr>::Add( kUser_ ) )
    {
        SET_ERROR( ERR_CHANNEL_10 );
        return false;
    }

    kUser_.SetChannel( this );

    if( this != ms_spChannelLobby.get() )
        kUser_.StateTransition( KUserFSM::INPUT_TO_CHANNEL );
    
    return true;
}

bool KChannel::Leave( KUserPtr::element_type& pkUser_ )
{
    SET_ERROR( NET_OK );    // 051005. florist. 함수의 성공여부를 이값으로 판단하므로, 초기화 필요함.

    // 060323. florist. 난데없이 STATE_INIT상태가 되어, 
    // 미확인 컨테이너들(채널,방 등..) 을 채 벗어나지 못해 refcount가 초과되는경우 발생.
    // STATE_INIT 상태에서도 채널을 빠져나갈 수 있도록 수정하기 위해 아래의 코드를 주석처리함.

    //// 051126. 6번 서버 오류 관련. 상태가 올바르지 못한 유저가 Leave하지 않도록 수정.
    //switch( pkUser_.GetStateID() )
    //{
    //case KUserFSM::STATE_INIT:
    //case KUserFSM::STATE_CONNECTED:
    //case KUserFSM::STATE_LOGINING:
    //case KUserFSM::STATE_EXIT:
    //    START_LOG( cerr, L"사용자 상태가 올바르지 못함. Name : " << pkUser_.m_strName )
    //        << BUILD_LOG( pkUser_.GetStateIDString() )
    //        << BUILD_LOG( ::GetTickCount() )
    //        << END_LOG;
    //    SET_ERROR( ERR_UNKNOWN );
    //    return false;
    //}

    LIF( KSobManager<KUserPtr>::Delete( pkUser_.GetName() ) );  // 실패했다는 건, 이미 없다는 이야기이므로, 계속 진행한다.

    pkUser_.SetChannel( NULL );

    if( this != ms_spChannelLobby.get() )               // 일반 채널에서 나가는 경우
    {
        pkUser_.StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
        ms_spChannelLobby->Enter( pkUser_ );                            // ChannelLobby로 입장.
    }

    return ( NetError::GetLastNetError() == NetError::NET_OK );
}

bool KChannel::ReserveRoomObject( unsigned short usRoomNum_ )
{
    if( m_spRoomManager == NULL )
    {
        NetError::SetLastNetError( NetError::ERR_CHANNEL_00 );
        return false;
    }
    return m_spRoomManager->ReserveRoomObject( usRoomNum_ );
}

void KChannel::GetFilteredPvPRoomList(char cType, KRoomOptions kRoomOptions, OUT KPagePvPInfo& kRoomPage_)
{
    std::map<int, std::vector<KListPage> >  mapPvPList;

    m_spRoomManager->BuildFilteredBattleRoomList(mapPvPList, kRoomOptions, cType);

    if (mapPvPList.empty())
    {
        SET_ERROR(ERR_LIST_00);
        return;
    }

    std::map<int, std::vector<KListPage> >::iterator mit;
    mit = mapPvPList.find(cType);

    if (mit == mapPvPList.end())
    {
        SET_ERROR(ERR_LIST_01);
        return;
    }

    if (mit->second.empty())
    {
        SET_ERROR(ERR_LIST_00);
        return;
    }

    KListPage kListPage = mit->second.at(0);
    kRoomPage_.m_buffCompList = kListPage.m_buffComList;
    SET_ERROR(NET_OK);
}

bool KChannel::Send_ChatMsg( const KChatData& kNot_ )
{

    KSerBuffer kbuff;
    kbuff.Clear();
    SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_CHAT_NOT, kNot_, kbuff, 100 );

    std::map<std::wstring, KUserPtr>::iterator mit;
    switch( kNot_.m_cChatType) {
    case KChatData::MSG_COMMON:       //일반 채팅이면 채널에 방이나 게임중이 아닌 유저에게 전송
    case KChatData::MSG_COLOR_CHAT:   // 컬러 채팅(특정 아이템을 가지고 있으면 컬러 체팅이 가능하다.
    case KChatData::MSG_ADMIN_COMMON:
        for( mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit )
        {
            if( mit->second->GetStateID() == KUserFSM::STATE_CHANNEL )
            {
                mit->second->SendPacket( kbuff, L"KUserEvent::EVENT_CHAT_NOT" );
            }
        }
        break;
    case KChatData::MSG_NOTIFY: // 채널에 포함된 모든 유저에게 전달.
        for( mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit )
        {
            mit->second->SendPacket( kbuff, L"KUserEvent::EVENT_CHAT_NOT" );
        }
        break;
    default:
        SET_ERROR( ERR_CHANNEL_01 );
        return false;
        break;
    }

    SET_ERROR( NET_OK );
    return true;
}
void KChannel::BroadPacket( KSerBuffer& kbuff_, const wchar_t* szDataDesc_ )
{
    std::map<std::wstring,KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit )
    {
        mit->second->SendPacket( kbuff_, szDataDesc_ );
    }
}

void KChannel::BroadPacketState( KSerBuffer& kbuff_, const wchar_t* szDataDesc_, std::set<int>& setState_ )
{
    std::map<std::wstring,KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit )
    {
        if( setState_.find( mit->second->GetStateID() ) != setState_.end() )
            mit->second->SendPacket( kbuff_, szDataDesc_ );
    }
}

void KChannel::BroadCasting()
{
    if( ::GetTickCount() - m_uiLastSendRoomList >= KChannel::ms_uiSendRoomListGap && m_spRoomManager != NULL )
    {
        BuildRoomList( m_mapWaitList, m_mapFullList, m_mapModeState );

        m_uiLastSendRoomList = ::GetTickCount();
    }

    // 050628. florist. 용사의 섬에서는 유저 리스트를 보내지 않는다. 090910 woosh.
    if( //SiKGSSimLayer()->CheckTypeFlag(ST_BROADCASTING) ||
        GetType() == KChannel::CT_INDIGO )
        return;

    if( ::GetTickCount() - m_uiLastSendUserList >= ms_uiSendUserListGap )
    {
        BuildUserList( m_vecLobbyUserList, m_vecLobbyFullUserList );

        m_uiLastSendUserList = ::GetTickCount();
    }
}

void KChannel::GetInfo( OUT KChannelInfo& kInfo_ )
{
    kInfo_.m_dwChannelUID   = GetUID();
    kInfo_.m_strChannelName = GetChannelName();
    kInfo_.m_usMaxUsers     = GetMaxUser();
    kInfo_.m_usUsers        = m_mapSob.size();
    kInfo_.m_cType          = GetType();
}

void KChannel::BuildUserList( OUT std::vector<KListPage>& vecLobbyUserList_, OUT std::vector<KListPage>& vecLobbyFullUserList_ )
{
    DWORD dwMapSize = 0;
    DWORD dwCapacity = 0;
    vecLobbyUserList_.clear();
    vecLobbyFullUserList_.clear();

    if( GetType() == CT_INDIGO ) return;

    std::vector<KInviteUserInfo> vecLobbyUserList;
    std::vector<KInviteUserInfo> vecLobbyFullUserList;

    // 로그를 위해서.
    dwMapSize = m_mapSob.size();
    dwCapacity = vecLobbyUserList.capacity();

    vecLobbyUserList.reserve( std::min<DWORD>( dwMapSize, dwCapacity ) );
    vecLobbyFullUserList.reserve( dwMapSize );

    std::map<std::wstring, KUserPtr>::iterator mit = m_mapSob.begin();

    try{

        for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
        {
            KInviteUserInfo kInviteInfo;
            KInviteUserInfo kAllInviteInfo;

            // 전체목록 작성(GM툴용)
            if( !mit->second->GetInviteInfo( kAllInviteInfo, true ) )
                continue;

            vecLobbyFullUserList.push_back( kAllInviteInfo );

            // 실제목록 작성
            if( !mit->second->GetInviteInfo( kInviteInfo ) )
                continue;

            vecLobbyUserList.push_back( kInviteInfo );
        }

        if( !vecLobbyFullUserList.empty() )
        {
            std::sort( vecLobbyFullUserList.begin(), vecLobbyFullUserList.end(), KComp() );
        }

        if( !vecLobbyUserList.empty() )
        {
            std::sort( vecLobbyUserList.begin(), vecLobbyUserList.end(), KComp() );
        }
    }catch( ... )
    {
        START_LOG( cerr, L"Exception .. mapSize : " << dwMapSize << L", capacity : " << dwCapacity ) << END_LOG;
        vecLobbyUserList.clear();
        vecLobbyFullUserList.clear();
    }

    //유저목록압축
    CompressUserList( vecLobbyFullUserList, vecLobbyFullUserList_ );
    CompressUserList( vecLobbyUserList, vecLobbyUserList_ );
}

void KChannel::BuildRoomList( OUT std::map<int, std::vector<KListPage> >& mapWaitList_,
                              OUT std::map<int, std::vector<KListPage> >& mapFullList_,
                              OUT std::map<int, std::vector<KGameModeState> >& mapModeState_ )
{
    mapWaitList_.clear();
    mapFullList_.clear();
    mapModeState_.clear();

    if( GetType() == CT_INDIGO ) return;

    if( !m_spRoomManager )
        return;

    if( GetType() == CT_DUNGEON )
    {
        m_spRoomManager->BuildDungeonRoomList( mapWaitList_, mapFullList_, mapModeState_ );
    }
    else if ( GetType() == CT_GUILD ) {

    }
    else
    {
        m_spRoomManager->BuildBattleRoomList( mapWaitList_, mapFullList_ );
    }

}

void KChannel::GetCorrectRoomList( bool bWait_, int nType_, int nDifficult_, int nPage_, OUT KPageInfo& kRoomPage_ )
{
    // 대기 인지 전체인지.. 선택한다..
    if( bWait_ )
        GetRoomList( m_mapWaitList, nType_, nDifficult_, nPage_, kRoomPage_ );
    else
        GetRoomList( m_mapFullList, nType_, nDifficult_, nPage_, kRoomPage_ );
}

void KChannel::GetCorrectGuildRoomList( bool bWait_, int nType_, int nDifficult_, int nPage_, OUT KPageInfo& kRoomPage_, IN DWORD guildUID_ )
{
    std::map<int, std::vector<KListPage> >  mapGuildWaitList;
    std::map<int, std::vector<KListPage> >  mapGuildFullList;

    m_spRoomManager->BuildGuildBattleRoomList( mapGuildWaitList, mapGuildFullList, guildUID_ );

    if( bWait_ ) {
        GetRoomList( mapGuildWaitList, nType_, nDifficult_, nPage_, kRoomPage_ );
    }
    else {
        GetRoomList( mapGuildFullList, nType_, nDifficult_, nPage_, kRoomPage_ );
    }
}

void KChannel::GetRoomList( IN std::map<int, std::vector<KListPage> >& mapRoomList_,
                            int nType_, int nDifficult_, int nPage_, OUT KPageInfo& kRoomPage_ )
{

    kRoomPage_.m_buffCompList.Clear();
    kRoomPage_.m_nCurrentPage = 0;
    kRoomPage_.m_nMaxPage = 0;

    if( nPage_ < 0 ) // 음수 오면 안되지만.. 그냥.. 처리하겠삼..
        nPage_ = 0;

    if( mapRoomList_.empty() )
    {
        SET_ERROR( ERR_LIST_00 );
        return;
    }
    

    std::map<int, std::vector<KListPage> >::iterator mit;

    if( GetType() == CT_DUNGEON )
    {
        union {
            short   m_asKey[2];
            int     m_nKey;
        } uniKey;

        uniKey.m_asKey[0] = (short)nType_;
        uniKey.m_asKey[1] = (short)nDifficult_;
        mit = mapRoomList_.find( uniKey.m_nKey );
    }
    else
    {
        mit = mapRoomList_.find( nType_ );
    }
    
    if( mit == mapRoomList_.end() )
    {
        SET_ERROR( ERR_LIST_01 );
        return ;
    }

    if( mit->second.empty() )
    {
        SET_ERROR( ERR_LIST_00 );
        return;
    }

    std::vector<KListPage>::iterator vit1;
    vit1 = std::find_if( mit->second.begin(), mit->second.end(),
            boost::bind( &KListPage::m_nPageNum, _1 ) == nPage_ );

    if( vit1 == mit->second.end() )
    {
        // 보내준 페이지가 맞지 않아서.... 가장 마지막 요소를 설정...
        // Empty 검사는 한 다음이므로..
        --vit1;
    }

    kRoomPage_.m_nMaxPage       = mit->second.size();
    kRoomPage_.m_nCurrentPage   = vit1->m_nPageNum;
    kRoomPage_.m_buffCompList   = vit1->m_buffComList;
    SET_ERROR( NET_OK );

}

void KChannel::GetUserList( IN int nPage_, OUT KPageInfo& kPageUser_ )
{
    if( nPage_ < 0 )
        nPage_ = 0;

    if( m_vecLobbyUserList.empty() )
    {
        SET_ERROR( ERR_LIST_00 );
        return;
    }
    
    std::vector<KListPage>::iterator vit;
    vit = std::find_if( m_vecLobbyUserList.begin(), m_vecLobbyUserList.end(),
        boost::bind( &KListPage::m_nPageNum, _1 ) == nPage_ );

    if( vit == m_vecLobbyUserList.end() )
    {
        --vit;
    }

    kPageUser_.m_nCurrentPage = vit->m_nPageNum;
    kPageUser_.m_nMaxPage = (int)m_vecLobbyUserList.size();
    kPageUser_.m_buffCompList = vit->m_buffComList;
    kPageUser_.m_nOK = 0;
    SET_ERROR( NET_OK );
}

KUserPtr KChannel::FindUser( const std::wstring& strName_ )
{    
    std::map<std::wstring,KUserPtr>::iterator mit;
    mit = m_mapSob.find( strName_ );
    if( mit == m_mapSob.end() ) return KUserPtr();

    return mit->second;

}

bool KChannel::CheckEnteranceGrade( char cGrade_ )
{
    if( this == KChannel::ms_spChannelLobby.get() )
        return true;

    if( m_prGradeRange.first <= cGrade_ && m_prGradeRange.second >= cGrade_ )
        return true;

    return false;
}

bool KChannel::CheckGuildEnteranceGrade( const int Grade_ )
{
    if ( this == KChannel::ms_spChannelLobby.get() ) {
        return true;
    }

    if ( m_prGradeRange.first <= Grade_ && m_prGradeRange.second >= Grade_ ) {
        return true;
    }

    return false;
}


void KChannel::ModifyChannel( const char* sz_str_, int nMaxUser_, const luabind::object&  objGrade_, int nRoomNum_, const luabind::object& ModeListv_, char cType_ )
{
    JIF( objGrade_.type() == LUA_TTABLE );
    std::pair<int,int> prGradeRange;
    KncUtil::Table2pair( objGrade_, prGradeRange );

    JIF( ModeListv_.type() == LUA_TTABLE );
    std::vector<int> vecModeList;
    KncUtil::Table2vector( ModeListv_, vecModeList );

    // 채널 타입 검사.
    JIF( cType_ >= 0 && cType_ < KChannel::CT_MAX );

    // 채널이름이 유효한지 체크 한다.
    std::wstring strKey  = KncUtil::toWideString( sz_str_ );
    if( !strKey.empty() ) //빈 문자열이면 이름 바꾸지 않음.
    {
        wchar_t szChannelName[255] = {0};
        GetPrivateProfileStringW( L"Channels", strKey.c_str(), L"", szChannelName, 255, SiKGameServer()->m_stringTable.GetCurrentFile().c_str() );
        std::wstring strName = szChannelName;

        //해당 채널을 찾는다.
        SetChannelName( strName.c_str() );

    }

    SetMaxUsers( nMaxUser_ );
    SetRoomNum( nRoomNum_ );
    SetType( cType_ );
    SetModeList( vecModeList );
    SetGradeRange( prGradeRange );

}

bool KChannel::GetRepetitionUserInfo( KUserPtr& spUser )
{
    std::map<std::wstring,KUserPtr>::iterator mit;
    mit = m_mapSob.begin();

    std::vector<std::wstring> vecLogins;
    while( mit != m_mapSob.end() )
    {
        if( mit->second->GetStateID() == KUserFSM::STATE_ROOM ||
            //mit->second->GetStateID() == KUserFSM::STATE_JOIN ||
            mit->second->GetStateID() == KUserFSM::STATE_PLAYING ||
            mit->second->GetStateID() == KUserFSM::STATE_CHANNEL )
        {
            vecLogins.push_back( mit->second->GetName() );
        }
        ++mit;
    }

    if( vecLogins.empty() )
        return false;

    std::random_shuffle( vecLogins.begin(), vecLogins.end() );

    spUser = FindUser( vecLogins[0] );

    if( spUser == NULL )
        return false;

    return true;
}

void KChannel::CompressUserList( IN std::vector<KInviteUserInfo>& vecUserList_, OUT std::vector<KListPage>& vecUserListPage_ )
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

void KChannel::GetAllUserList( IN int nPage_, OUT KPageInfo& kPageUser_ )
{
    if( nPage_ < 0 )
        nPage_ = 0;

    if( m_vecLobbyFullUserList.empty() )
    {
        SET_ERROR( ERR_LIST_00 );
        return;
    }

    std::vector<KListPage>::iterator vit;
    vit = std::find_if( m_vecLobbyFullUserList.begin(), m_vecLobbyFullUserList.end(),
        boost::bind( &KListPage::m_nPageNum, _1 ) == nPage_ );

    if( vit == m_vecLobbyFullUserList.end() )
    {
        --vit;
    }

    kPageUser_.m_nCurrentPage = vit->m_nPageNum;
    kPageUser_.m_nMaxPage = (int)m_vecLobbyFullUserList.size();
    kPageUser_.m_buffCompList = vit->m_buffComList;
    kPageUser_.m_nOK = 0;
    SET_ERROR( NET_OK );
}

bool KChannel::IsPvPChannel()
{
    char cType = GetType();
    if( cType == CT_UNLIMITED || cType == CT_BEGINNER || cType == CT_ROOKIE ||
        cType == CT_EXPERT || cType == CT_MASTER  || cType == CT_AUTOMATCH )
    {
        return true;
    }

    return false;
}


void KChannel::GetGameModeState( IN const std::set<int>& setModeList_, OUT std::map<int, std::vector<KGameModeState> >& mapGameModeState_ )
{
    mapGameModeState_.clear();
    std::set<int>::const_iterator sit;
    std::map<int, std::vector<KGameModeState> >::iterator mit;

    for ( sit = setModeList_.begin() ; sit != setModeList_.end() ; ++sit ) {
        mit = m_mapModeState.find( *sit );
        if ( mit != m_mapModeState.end() ) {
            mapGameModeState_.insert( *mit );
        }
        else
            mapGameModeState_.insert( std::make_pair(*sit, vecDummyStatus) );
    }
}

void KChannel::DeleteUser( const IN std::wstring& strName_ )
{
    if ( KSobManager<KUserPtr>::Delete( strName_ ) ) {
        START_LOG( cerr, L"유저 객체가 남아있는 채널에서 유저 삭제, User : " << strName_ )
            << BUILD_LOG( GetChannelName() )
            << END_LOG;
    }
}