#include "User.h" // winsock
#include "CnConnector.h" // winsock
#include "ChannelManager.h"
#include "UserEvent.h"
#include "GameServer.h"
#include "KncUtilLua.h" // luabind::object
#include "GSDBLayer.h"
#include "Channel.h"
#include "Log4.h"

#define MAX_CHANNEL_NUM 40
ImplementSingleton( KChannelManager );

unsigned int KChannelManager::ms_uiSendChannelListGap( 10000 );

KChannelManager::KChannelManager(void)
{
    m_uiLastSendChannelListTick = ::GetTickCount();
    InitGameCategoryModeID();
}

KChannelManager::~KChannelManager(void)
{
}

ImplToStringW( KChannelManager )
{
    stm_ << L"KChannelManager dump." << std::endl
         << TOSTRINGW( KChannel::ms_uiSendRoomListGap )
         << TOSTRINGW( KChannel::ms_uiSendUserListGap )
         << TOSTRINGW( ms_uiSendChannelListGap )
         << TOSTRINGW( GetNumChannel() )
         << *KChannel::ms_spChannelLobby;

    KLocker lock(m_csChannel);
    std::map<DWORD,KChannelPtr>::const_iterator mit;
    for( mit = m_mapChannel.begin(); mit != m_mapChannel.end(); mit++ )
    {
        stm_ << *mit->second;
    }

    return stm_;
}

void KChannelManager::SetChannelLobby( IN int nMaxUser_ )
{
    if( KChannel::ms_spChannelLobby )
    {
        START_LOG( cerr, L"**Already Exist ChannelLobby.....**" ) << END_LOG;
        return;
    }
    
    if( nMaxUser_ <= 0 )
    {
        START_LOG( cerr, L" **ALERT** Low Number....." )
            << BUILD_LOG( nMaxUser_ ) << END_LOG;
        return;
    }

    KChannel* pChannel = new KChannel;
    pChannel->SetChannelName( L"Channel Lobby" );
    pChannel->SetMaxUsers( nMaxUser_ );
    pChannel->SetRoomNum( 500 ); // Q-Zero 없어지면 제거해봅시다.
    KChannel::ms_spChannelLobby.reset( pChannel );
}

void KChannelManager::ModifyChannelLobby( IN int nMaxUser_ )
{
    if( !KChannel::ms_spChannelLobby )
    {
        START_LOG( cerr, L"**Not Exist ChannelLobby.....**" ) << END_LOG;
        return;
    }
    KChannel::ms_spChannelLobby->SetMaxUsers( nMaxUser_ );
}

void KChannelManager::AddChannel( const char* sz_str_, int nMaxUser_, const luabind::object& objGrade_, int nRoomNum_,
                                  const luabind::object& ModeListv_, char cType_, const luabind::object& obIPRange_ )
{
    JIF( objGrade_.type() == LUA_TTABLE );
    std::pair<int,int> prGradeRange;
    KncUtil::Table2pair( objGrade_, prGradeRange );

    JIF( ModeListv_.type() == LUA_TTABLE );
    std::vector<int> vecModeList;
    KncUtil::Table2vector( ModeListv_, vecModeList );

    JIF( obIPRange_.type() == LUA_TTABLE );
    std::string strBegin( luabind::object_cast<std::string>( obIPRange_[ 1 ] ) );
    std::string strEnd( luabind::object_cast<std::string>( obIPRange_[ 2 ] ) );

    std::pair< DWORD, DWORD > prIPRange( 0x00000000, 0xFFFFFFFF );
    if( !strBegin.empty() && !strEnd.empty() )
    {
        prIPRange.first     = ::ntohl( inet_addr( strBegin.c_str() ) );
        prIPRange.second    = ::ntohl( inet_addr( strEnd.c_str() ) );
    }

    // 채널의 개수가 이미 초과했는지 확인한다.
    if( GetNumChannel() >= MAX_CHANNEL_NUM )
    {
        START_LOG( cerr , L"Channel List Is Full...." )
            << BUILD_LOG( GetNumChannel() ) << END_LOG;
    }
    
    // 채널이름이 유효한지 체크 한다.
    std::wstring strKey  = KncUtil::toWideString( sz_str_ );
    
    wchar_t szChannelName[255] = {0};
    GetPrivateProfileStringW( L"Channels", strKey.c_str(), L"", szChannelName, 255, SiKGameServer()->m_stringTable.GetCurrentFile().c_str() );
    std::wstring strName = szChannelName;

    if( strName.empty() )
    {
        START_LOG( cerr, L"채널이름이 목록에 없습니다." )
            << BUILD_LOG( sz_str_ ) << END_LOG;
        return;
    }

    // 채널중에 동일한 아이디가 있는지 확인한다.
    DWORD dwID;
    dwID = GetNumChannel();
    dwID += 1;

    KLocker lock(m_csChannel);

    if( m_mapChannel.find( dwID ) != m_mapChannel.end() )
    {
        START_LOG( cerr, L"Already Exist Same ID In Channel List...." )
            << BUILD_LOG( dwID ) << END_LOG;
        return;
    }

    JIF( cType_ >= 0 && cType_ < KChannel::CT_MAX );

    // 채널을 생성해서 채널 목록에 넣는다.
    KChannelPtr spChannel( new KChannel );
    spChannel->SetUID( dwID );
    spChannel->SetChannelName( strName.c_str() );
    spChannel->SetMaxUsers( nMaxUser_ );
    spChannel->SetRoomNum( nRoomNum_ );
    spChannel->SetType( cType_ );
    spChannel->SetModeList( vecModeList );
    spChannel->SetGradeRange( prGradeRange );
    spChannel->SetIPRange( prIPRange );
    m_mapChannel[ spChannel->GetUID() ] = spChannel;

    START_LOG( cerr, L"채널 추가 완료 , ID :" << dwID << L", Name : " << strName << L", Type : " << (int)cType_ ) << END_LOG;

}



void KChannelManager::Remove( const DWORD dwChannelUID_ )
{
    KLocker lock(m_csChannel);
    std::map<DWORD, KChannelPtr>::iterator mit;
    mit = m_mapChannel.find( dwChannelUID_ );

    if( mit == m_mapChannel.end() )
    {
        START_LOG( cerr, L"제거하려는 채널이 존재하지 않음" )
            << BUILD_LOG( dwChannelUID_ )
            << BUILD_LOG( m_mapChannel.size() )
            << END_LOG;
    }

    m_mapChannel.erase( mit );
}

KChannel* KChannelManager::Get( const DWORD dwChannelUID_ )
{
    KLocker lock(m_csChannel);

    if ( m_mapChannel.empty() ) {
        START_LOG( cwarn, L"채널이 비어있음." )
            << BUILD_LOG( GetNumChannel() ) << END_LOG;
        return NULL;
    }

    std::map<DWORD, KChannelPtr>::iterator mit;
    mit = m_mapChannel.find( dwChannelUID_ );

    if( mit == m_mapChannel.end() )
    {
        START_LOG( cwarn, L"해당 이름을 가진 채널 없음" )
            << BUILD_LOG( dwChannelUID_ )
            << BUILD_LOG( GetNumChannel() ) << END_LOG;

        return m_mapChannel.begin()->second.get();
    }

    return mit->second.get();
}

KChannelPtr KChannelManager::GetChannelSP( const DWORD dwChannelUID_ )
{
    KChannelPtr spChannel;
    spChannel.reset();
    std::map<DWORD, KChannelPtr>::iterator mit;
    {
        KLocker lock( m_csChannel );
        mit = m_mapChannel.find( dwChannelUID_ );

        if( mit == m_mapChannel.end() )
        {
            START_LOG( cwarn, L"해당 이름을 가진 채널 없음" )
                << BUILD_LOG( dwChannelUID_ )
                << BUILD_LOG( GetNumChannel() ) << END_LOG;

            return spChannel;
        }

        spChannel = mit->second;
    }

    return spChannel;
}

bool KChannelManager::EnterChannel( IN DWORD dwChannelUID_, KUser* pkUser_ )
{
    KChannel* pkChannel = Get( dwChannelUID_ );
    if ( pkChannel == NULL ) {
        START_LOG( cerr, L"채널 없음" )
            << BUILD_LOG( dwChannelUID_ )
            << END_LOG;
        SET_ERROR( ERR_INTERNAL_00 );
        return false;
    }

    if ( pkChannel->Enter( *pkUser_ ) ) {
        return true;
    }

    if ( NetError::GetLastNetError() == NetError::ERR_CHANNEL_08 ) {
        pkChannel = GetByChannelType( KChannel::CT_UNLIMITED );
        if ( pkChannel == NULL ) {
            START_LOG( cerr, L"해당 타입의 채널 없음" )
                << BUILD_LOG( KChannel::CT_UNLIMITED )
                << END_LOG;
            SET_ERROR( ERR_INTERNAL_00 );
            return false;
        }

        if ( pkChannel->Enter( *pkUser_ ) ) {
            return true;
        }
    }

    return false;
}

void KChannelManager::SetBroadCastingTime( unsigned int uiSendChannelList_,
                                           unsigned int uiSendRoomList_, 
                                           unsigned int uiSendUserList_ )
{
    ms_uiSendChannelListGap         = uiSendChannelList_;
    KChannel::ms_uiSendRoomListGap  = uiSendRoomList_;
    KChannel::ms_uiSendUserListGap  = uiSendUserList_;
}

void KChannelManager::BuildChannelList()
{

    m_kbuffChannelListData.Clear();
    // 채널 정보 얻기
    std::vector< KChannelInfo > vecChannelInfo;
    vecChannelInfo.reserve( GetNumChannel() );
    KLocker lock( m_csChannel );

    KChannelInfo    kChannelInfo;
    std::map<DWORD, KChannelPtr>::iterator mit;
    for( mit = m_mapChannel.begin(); mit != m_mapChannel.end(); mit++ )
    {
        mit->second->GetInfo( kChannelInfo );
        vecChannelInfo.push_back( kChannelInfo );
    }

    KSerializer         ks;
    ks.BeginWriting( &m_kbuffChannelListData );
    ks.Put( vecChannelInfo );
    ks.EndWriting();

    m_kbuffChannelListData.Compress();   // 압축 데이터.
}

void KChannelManager::Tick()
{
    KLocker lock(m_csChannel);
    std::map<DWORD, KChannelPtr>::iterator mit = m_mapChannel.begin();
    for( ; mit != m_mapChannel.end() ; ++mit )
    {
        mit->second->GetRoomManager()->Tick();
    }

    // 050922. 리스트를 만들어 두기만 한다. 가져가는 것은 유저객체 각각이 핸들링 하도록 변경함.
    if( ::GetTickCount() - m_uiLastSendChannelListTick >= ms_uiSendChannelListGap )
    {
        BuildChannelList();

        m_uiLastSendChannelListTick = ::GetTickCount();
    }

    // 방 정보 리스트, 유저 정보 리스트는 각각의 채널이 알아서 처리한다.
    std::map<DWORD, KChannelPtr>::iterator mit2;
    for( mit2 = m_mapChannel.begin(); mit2 != m_mapChannel.end(); mit2++ )
    {
        mit2->second->BroadCasting();
    }

    KSerBuffer kbuffMsg;
    if( GetMsg( kbuffMsg ) )
    {
        BroadPacket( kbuffMsg, L"LOUD MSG..." );
    }

    ProcessRepetition();
}

KUserPtr KChannelManager::FindUser( const std::wstring& strName_ )
{
    KUserPtr spkUser;    
    // Channel lobby 에서 검색
    KLocker lock(m_csChannel);

    spkUser = KChannel::ms_spChannelLobby->FindUser( strName_ );
    if( spkUser != NULL && spkUser->GetName() == strName_ )
        return spkUser;

    // 각 Channel 에서 검색
    std::map<DWORD, KChannelPtr>::iterator mit;
    for( mit = m_mapChannel.begin(); mit != m_mapChannel.end(); mit++ )
    {
        spkUser = mit->second->FindUser( strName_ );
        if( spkUser != NULL && spkUser->GetName() == strName_ )
            break;    
    }
    if( mit == m_mapChannel.end() ) return KUserPtr();

    return spkUser;
}

KChannel* KChannelManager::FindUserChannel( const std::wstring& strName_ )
{
    KLocker lock(m_csChannel);

    std::map<DWORD, KChannelPtr>::iterator mit;
    for ( mit = m_mapChannel.begin() ; mit != m_mapChannel.end() ; ++mit )
    {
        KUserPtr spkUser = mit->second->FindUser( strName_ );
        if ( spkUser == NULL ) continue;
        if ( spkUser->GetName() != strName_ ) continue;

        return mit->second.get();
    }

    return m_mapChannel.begin()->second.get();
}

void KChannelManager::DeleteUserChannel( const std::wstring& strName_ )
{
    KLocker lock(m_csChannel);

    std::map<DWORD, KChannelPtr>::iterator mit;
    for ( mit = m_mapChannel.begin() ; mit != m_mapChannel.end() ; ++mit )  {

        KUserPtr spkUser = mit->second->FindUser( strName_ );
        if ( spkUser == NULL ) continue;
        if ( spkUser->GetName() != strName_ ) continue;

        mit->second->DeleteUser( strName_ );
    }
}

void KChannelManager::Send_NotMsgA( const char* szMessage )
{
    std::wstring wstrMessage = KncUtil::toWideString( szMessage );
    Send_NotMsg( wstrMessage.c_str() );
}

void KChannelManager::Send_NotMsg( const wchar_t* szMessage_ )
{
    KChatData kPacket;
    kPacket.m_cChatType      = KChatData::MSG_NOTIFY;
    kPacket.m_strChatMsg     = szMessage_;

    KLocker lock(m_csChannel);
    // 050620. florist.
    KChannel::ms_spChannelLobby->Send_ChatMsg( kPacket );

    // 각 Channel 에서 검색
    std::map<DWORD, KChannelPtr>::iterator mit;
    for( mit = m_mapChannel.begin(); mit != m_mapChannel.end(); ++mit )
    {
        mit->second->Send_ChatMsg( kPacket );
    }
}

void KChannelManager::BroadPacket( KSerBuffer& kbuff_, const wchar_t* szDataDesc_, bool bSendChannelLobby /*= true*/ )
{
    KLocker lock(m_csChannel);
    // 050620. florist.
    if( bSendChannelLobby )
        KChannel::ms_spChannelLobby->BroadPacket( kbuff_, szDataDesc_ );

    // 각 Channel 에서 검색
    std::map<DWORD, KChannelPtr>::iterator mit;
    for( mit = m_mapChannel.begin(); mit != m_mapChannel.end(); ++mit )
    {
        mit->second->BroadPacket( kbuff_, szDataDesc_ );
    }
}

bool KChannelManager::GetMsg( KSerBuffer& kbuff_ )
{
    KLocker lock( m_csMsgQueue );

    if( !m_queMsg.empty() )
    {
        kbuff_ = m_queMsg.front();
        m_queMsg.pop();
        return true;
    } 

    return false;
}

void KChannelManager::QueueingRepetition( KECN_REPETITION_PROVIDE_REQ& kPacket_ )
{
    KLocker lock( m_csRepetition ); m_queRepetition.push( kPacket_ );
};
bool KChannelManager::GetRepetition( KECN_REPETITION_PROVIDE_REQ& kPacket_ )
{

    KLocker lock( m_csRepetition );

    if( !m_queRepetition.empty() )
    {
        kPacket_ = m_queRepetition.front();
        m_queRepetition.pop();
        return true;
    } 
    return false;
}

void KChannelManager::ProcessRepetition()
{
    //안심하고 돌리자.. 쓰레드 안전이다.. ㅋㅋㅋ
    KECN_REPETITION_PROVIDE_REQ kData;
    if( !GetRepetition( kData ) ) return;

    KChannelPtr spChannel;
    spChannel.reset();
    if( !GetRandomChannel( spChannel, 1 ) )
    {
        SiKCnConnector()->SendPacket( KCenterEvent::ECN_REPETITION_PROVIDE_CANCEL_REQ, kData );
        START_LOG( cwarn, L"20 명 이상의 채널이 존재하지 않는다..." ) << END_LOG;
        return;
    }
    
    KUserPtr spUser;
    if( !spChannel->GetRepetitionUserInfo( spUser ) )
    {
        SiKCnConnector()->SendPacket( KCenterEvent::ECN_REPETITION_PROVIDE_CANCEL_REQ, kData );
        START_LOG( cwarn, L"룸에 있는 유저가 존재하지 않는다.." ) << END_LOG;
        return;
    }

    KEVENT_DB_REPETITION_PROVIDE_REQ kPacket;

    kPacket.m_strLogin      = spUser->GetName();
    kPacket.m_dwUID         = spUser->GetUID();
    kPacket.m_strNick       = spUser->GetNickName();
    kPacket.m_dwEventUID    = kData.m_dwEventUID;
    kPacket.m_nCommentID    = kData.m_nCommentID;
    kPacket.m_nLanguageType = spUser->GetLanguageCode();

    START_LOG( clog, L"한시간 이벤트 DB에 QUEUING.." )
        << BUILD_LOG( kPacket.m_strLogin )
        << BUILD_LOG( kPacket.m_dwUID )
        << BUILD_LOG( kPacket.m_strNick )
        << BUILD_LOG( kPacket.m_dwEventUID )
        << BUILD_LOG( kPacket.m_nCommentID )
        << BUILD_LOG( kPacket.m_nLanguageType ) << END_LOG;

    SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_DB_REPETITION_PROVIDE_REQ, kPacket.m_strLogin.c_str(), kPacket.m_dwUID, 0, kPacket );
}

bool KChannelManager::GetRandomChannel( KChannelPtr& spChannel, int nMinimalUser )
{
    spChannel.reset();
    std::vector<DWORD> vecChannelIDs;
    {
        KLocker lock( m_csChannel );
        std::map<DWORD, KChannelPtr>::const_iterator mit;
        for( mit = m_mapChannel.begin() ; mit != m_mapChannel.end() ; ++mit )
        {
            if( mit->second->GetCount() >= static_cast<unsigned int>(nMinimalUser) )
                vecChannelIDs.push_back( mit->first );
        }
    }

    _JIF( !vecChannelIDs.empty(), return false );

    std::random_shuffle( vecChannelIDs.begin(), vecChannelIDs.end() );

    spChannel = GetChannelSP( vecChannelIDs[0] ); // 비어 있지 않음을 보장 받은 상태.(실시간 채널 제거가 있지 않다는 가정)
   return true;
}

KChannel* KChannelManager::GetByChannelType( IN const int& nChannelType_ )
{
    if ( m_mapChannel.empty() ) {
        START_LOG( cwarn, L"채널이 비어있음." )
            << BUILD_LOG( GetNumChannel() ) << END_LOG;
        return NULL;
    }

    std::map<DWORD, KChannelPtr>::iterator mit;
    for ( mit = m_mapChannel.begin() ; mit != m_mapChannel.end() ; ++mit ) {
        if ( mit->second.get()->GetType() == nChannelType_ ) {
            break;
        }
    }

    if ( mit == m_mapChannel.end() ) {
        START_LOG( cwarn, L"해당 채널타입의 채널 없음" )
            << BUILD_LOG( nChannelType_ )
            << BUILD_LOG( GetNumChannel() ) << END_LOG;

        return m_mapChannel.begin()->second.get();
    }

    return mit->second.get();
}

void KChannelManager::InitGameCategoryModeID()
{
    KLocker lock( m_csCategoryModeID );

    m_setGameCategoryModeID[GC_GMC_MATCH].insert(GC_GM_TEAM_MODE);
    m_setGameCategoryModeID[GC_GMC_MATCH].insert(GC_GM_SURVIVAL_MODE);
    m_setGameCategoryModeID[GC_GMC_GUILD_BATTLE].insert(GC_GM_GUILD_BATTLE);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST0);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST1);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST2);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST3);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST4);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST5);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST6);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST7);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST8);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST9);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST10);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST11);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST12);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST13);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST14);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST15);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST16);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST17);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST18);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST19);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST20);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST21);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST22);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST23);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST24);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST25);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST26);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST27);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST28);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST29);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST30);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST31);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST32);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST33);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST34);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST35);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST36);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST37);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST38);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST39);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST40);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST41);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST42);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST43);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST44);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST45);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST46);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST47);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST48);
    m_setGameCategoryModeID[GC_GMC_INDIGO].insert(GC_GM_INDIGO_TEAM);
    m_setGameCategoryModeID[GC_GMC_TUTORIAL].insert(GC_GM_TUTORIAL);
    m_setGameCategoryModeID[GC_GMC_TAG_MATCH].insert(GC_GM_TAG_TEAM);
    m_setGameCategoryModeID[GC_GMC_TAG_MATCH].insert(GC_GM_TAG_SURVIVAL);
    m_setGameCategoryModeID[GC_GMC_MONSTER_CRUSADER].insert(GC_GM_MONSTER_CRUSADER);
    m_setGameCategoryModeID[GC_GMC_MONSTER_HUNT].insert(GC_GM_MONSTER_HUNT);
    m_setGameCategoryModeID[GC_GMC_DEATHMATCH].insert(GC_GM_DEATH_TEAM);
    m_setGameCategoryModeID[GC_GMC_DEATHMATCH].insert(GC_GM_DEATH_SURVIVAL);
    m_setGameCategoryModeID[GC_GMC_MINIGAME].insert(GC_GM_MINIGAME_TREEDROP);
    m_setGameCategoryModeID[GC_GMC_MINIGAME].insert(GC_GM_MINIGAME_BALLOON);
    m_setGameCategoryModeID[GC_GMC_MINIGAME].insert(GC_GM_MINIGAME_BUTTERFRY);
    m_setGameCategoryModeID[GC_GMC_ANGELS_EGG].insert(GC_GM_ANGELS_EGG);
    m_setGameCategoryModeID[GC_GMC_CAPTAIN].insert(GC_GM_CAPTAIN);
    m_setGameCategoryModeID[GC_GMC_DOTA].insert(GC_GM_DOTA);
    m_setGameCategoryModeID[GC_GMC_AGIT].insert(GC_GM_AGIT);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST49);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST50);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST51);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST52);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST53);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST54);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST55);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST56);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST57);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST58);
    m_setGameCategoryModeID[GC_GMC_AUTOMATCH].insert(GC_GM_AUTOMATCH_TEAM);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST59);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST60);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST61);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST62);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST63);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST64);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST65);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST66);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST67);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST68);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST69);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST70);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST71);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST72);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST73);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST74);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST75);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST76);
    m_setGameCategoryModeID[GC_GMC_FATAL_DEATHMATCH].insert(GC_GM_FATAL_DEATH_TEAM);
    m_setGameCategoryModeID[GC_GMC_FATAL_DEATHMATCH].insert(GC_GM_FATAL_DEATH_SURVIVAL);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST77);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST78);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST79);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST80);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST81);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST82);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST83);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST84);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST85);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST86);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST87);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST88);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST89);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST90);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST91);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST92);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST93);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST94);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST95);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST96);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST97);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST98);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST99);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST100);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST101);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST102);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST103);
    m_setGameCategoryModeID[GC_GMC_DUNGEON].insert(GC_GM_QUEST104);
}

bool KChannelManager::GetModeCategory( IN const int& nModeID_, OUT int& nCategory_ )
{
    nCategory_ = GC_GMC_INVALID;

    KLocker lock( m_csCategoryModeID );

    std::map<int,std::set<int> >::const_iterator cmit;
    for ( cmit = m_setGameCategoryModeID.begin() ; cmit != m_setGameCategoryModeID.end() ; ++cmit ) {
        std::set<int>::const_iterator csit;
        csit = cmit->second.find( nModeID_ );
        if ( csit != cmit->second.end() ) {
            break;
        }
    }

    if ( cmit == m_setGameCategoryModeID.end() ) {
        return false;
    }

    nCategory_ = cmit->first;
    return true;
}
