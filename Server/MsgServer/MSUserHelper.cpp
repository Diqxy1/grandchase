#include "MSUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"
#include "MSSimLayer.h"
#include "MSDBLayer.h"
#include "NetError.h"
#include "MRPacket.h"
#include "SlangFilter.h"
#include "support_MSUser.h"
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

bool KMSUser::IsBlockedFriend_S2( IN const DWORD dwFriendUID_ )
{
    return m_kBlockGroup.m_mapMembers.find( dwFriendUID_ ) != m_kBlockGroup.m_mapMembers.end();
}

bool KMSUser::IsBlockedFriend_S2( IN const std::wstring& strNick_ )
{
    for( std::map<DWORD,KFriend>::const_iterator cmit = m_kBlockGroup.m_mapMembers.begin() ;
        cmit != m_kBlockGroup.m_mapMembers.end() ; ++cmit )
    {
        if( cmit->second.m_strNickName == strNick_ ) return true;
    }

    return false;
}

KFriend KMSUser::GetMyFriendInfo_S2( DWORD dwReceiverUID /* = 0  */ )
{
    KFriend kInfo;

    kInfo.m_dwUserUID =GetUID();
    kInfo.m_strNickName = m_strNickName;
    kInfo.m_strNickColor = m_strNickColor;
    kInfo.m_bIsOnline = true;
    kInfo.m_bIsFriend = true;
    kInfo.m_bIsGamming = m_bGamming;
    kInfo.m_iLocationNum = m_iLocationNum;
    kInfo.m_strLocation = m_strLocation;
    kInfo.m_strServerName = m_strServerName;
    kInfo.m_dwEmoticon = m_dwEmoticon;
    kInfo.m_cChatRoomCount = m_cChatRoomCount;

    if( dwReceiverUID > 0 && ( IsBlockedFriend_S2( dwReceiverUID ) || !IsMyFriend(dwReceiverUID) ) )
        kInfo.m_bIsOnline = false;

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( m_strNickColor )
        << BUILD_LOG( m_bGamming )
        << BUILD_LOG( m_iLocationNum )
        << BUILD_LOG( m_strLocation )
        << BUILD_LOG( m_strServerName )
        << BUILD_LOG( m_dwEmoticon )
        << BUILD_LOGc( m_cChatRoomCount )
        << BUILD_LOG( IsMyFriend(dwReceiverUID) )
        << BUILD_LOG( dwReceiverUID ) << END_LOG;

    return kInfo;
}

bool KMSUser::UpdateFriendInfo_S2( IN const KFriend& kInfo_ )
{
    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mitMember;

    //대상이 블럭이면 리턴
    if( IsBlockedFriend_S2( kInfo_.m_dwUserUID ) ) return false;

    //내 친구가 아니고, 친구 최대수 초과하면 리턴
    if( !IsMyFriend( kInfo_.m_dwUserUID ) && GetAllFriendCount() >= MAX_FRIEND_LIMIT )  return false;

    // 친구의 정보를 갱신해 줍니다.
    do 
    {
        for( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit ) {
            mitMember = mit->second.m_mapMembers.find( kInfo_.m_dwUserUID );
            if( mitMember != mit->second.m_mapMembers.end() )
            break;
    }

        if( mit == m_mapFriendGroup.end() ) {
            break;
        }

        mitMember->second = kInfo_;

    } while (false);
	
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kInfo_.m_dwUserUID )
        << BUILD_LOG( kInfo_.m_strNickName )
        << BUILD_LOG( kInfo_.m_strNickColor )<< END_LOG;

    return true;
}

int KMSUser::GetAllFriendCount()
{
    int nCount = 0;
    std::map<int,KGroup>::iterator mit;
    for( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit )
    {
        // 길드그룹은 친구가 아니다.
        if ( mit->first == KGroup::GUILD_BASE ) {
            continue;
        }

        nCount += mit->second.m_mapMembers.size();
    }
    return nCount;
}

void KMSUser::SendToMyFriends_S2( IN const KFriend& kInfo_, IN const bool bConditionCheck_ /*= true*/ )
{
    std::set<DWORD> setFriendUIDList;

    // 상태가 같으면 보내지 않아
    if ( bConditionCheck_ && kInfo_ == m_ReservedLoc_S2 ) {
        return;
    }

    // 받을 친구들 리스트를 뽑자!
    std::map<int,KGroup>::iterator mit;
    for ( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit ) {
        GetSendableFriends( mit->second.m_mapMembers, setFriendUIDList, false );
    }

    if( !setFriendUIDList.empty() )
    {
        // 메세지 전송이 실패하면 EMS_S2_FRIEND_NOT_EXIST_NOT가 올거야.
        std::vector<DWORD> vecRecvUIDs;
        SetRecvUIDs( setFriendUIDList, vecRecvUIDs );
        REMOTE_QUEUEING_PACK_ACK( vecRecvUIDs, EMS_S2_UPDATE_FRIEND_NOT, kInfo_ ); // kInfo_ 는 내 정보 --> 친구들에게 알려줌.
        m_ReservedLoc_S2      = kInfo_;        //전송 할 때의 상태를 저장하도록 하자..

        START_LOG( clog, L"Remote Queueing Success" )
            << BUILD_LOG( GetTickGap( TI_SEND_USER_INFO ) )
            << BUILD_LOG( kInfo_.m_strNickName )
            << BUILD_LOG( kInfo_.m_strNickColor )
            << BUILD_LOG( kInfo_.m_strServerName )
            << BUILD_LOG( kInfo_.m_strLocation )
            << BUILD_LOG( kInfo_.m_iLocationNum )
            << BUILD_LOG( setFriendUIDList.size() ) << END_LOG;
    }
}

void KMSUser::GetSendableFriends( IN const std::map<DWORD,KFriend>& mapFriends_, OUT std::set<DWORD>& setFriendUIDList_
                                 , IN bool bFriendCheck_ /*= true*/, IN bool bOnlineCheck_ /*= true*/, IN bool bBlockFriendCheck_/* = true*/ )
{
    std::map<DWORD,KFriend>::const_iterator mit;
    for( mit = mapFriends_.begin(); mit != mapFriends_.end(); ++mit )
    {
        // 차단한 친구라면 알릴 필요 없다.
        if( bBlockFriendCheck_ && IsBlockedFriend_S2( mit->second.m_dwUserUID ) )
            continue;

        // 접속중이 아니면 보내지 않는다
        if( bOnlineCheck_ && !mit->second.m_bIsOnline )
            continue;

        if( bFriendCheck_ && !mit->second.m_bIsFriend )
            continue;

        // 전송할 친구들의 UID를 따로 추려낸다
        setFriendUIDList_.insert( mit->first );
    }
}

bool KMSUser::_IsMyFriend( IN const std::map<DWORD,KFriend>& mapMembers_, IN const DWORD dwFriendUID_ )
{
    std::map<DWORD,KFriend>::const_iterator cmit;
    cmit = mapMembers_.find( dwFriendUID_ );
        // 친구 리스트에 있고,
    if( cmit != mapMembers_.end() )
    {
            // 등록된 친구이면.
        if( cmit->second.m_bIsFriend )
                return true;
    }
    return false;
}

bool KMSUser::_IsMyFriend( IN const std::map<DWORD,KFriend>& mapMembers_, IN const std::wstring& strNick_ )
{
    std::map<DWORD,KFriend>::const_iterator cmit;
    for( cmit = mapMembers_.begin() ; cmit != mapMembers_.end() ; ++cmit )
    {
        if( cmit->second.m_strNickName == strNick_ && cmit->second.m_bIsFriend )
        {
            return true;
        }
    }
    return false;
}

bool KMSUser::IsMyFriend( IN const DWORD dwFriendUID_ )
{
    std::map<int,KGroup>::const_iterator cmit;

    for( cmit = m_mapFriendGroup.begin() ; cmit != m_mapFriendGroup.end() ; ++cmit )
    {
        // 길드그룹은 친구가 아니다.
        if ( cmit->first == KGroup::GUILD_BASE ) {
            continue;
        }

        if( _IsMyFriend( cmit->second.m_mapMembers, dwFriendUID_ ) )
            return true;
    }

    return false;
}

bool KMSUser::IsMyFriend( IN const std::wstring& strNick_ )
{
    std::map<int,KGroup>::const_iterator cmit;

    for( cmit = m_mapFriendGroup.begin() ; cmit != m_mapFriendGroup.end() ; ++cmit )
    {
        if ( cmit->first == KGroup::GUILD_BASE ) {
            continue;
        }
        if( _IsMyFriend( cmit->second.m_mapMembers, strNick_ ) ) {
            return true;
        }
    }

    return false;
}

void KMSUser::InitChatRoom()
{
    m_mapChatRoomInfo.clear();
    m_cChatRoomCount = 0;
}

bool KMSUser::IsChatRoomEnable()
{
    return ( m_mapChatRoomInfo.size() < MAX_CHAT_ROOM_LIMIT );
}

void KMSUser::ChkChatEnable( IN OUT std::set<DWORD>& setInviteUIDs_, OUT std::set<DWORD>& setUnableUIDs_, OUT std::map<DWORD, std::pair<std::wstring, std::wstring>>& mapMemberInfo_, OUT std::vector<DWORD>& vecRecvUIDs_ )
{
    setUnableUIDs_.clear();
    mapMemberInfo_.clear();
    vecRecvUIDs_.clear();

    std::set<DWORD> setCorrectUIDs;

    std::set<DWORD>::iterator sit;
    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    // 대화초대할 수 있는 UID인지.
    for( sit = setInviteUIDs_.begin() ; sit != setInviteUIDs_.end() ; ++sit )
    {
        if( IsMyFriend( *sit ) == false )
        {
            // 존재하지 않는 친구UID.
            setUnableUIDs_.insert( *sit );
            START_LOG( clog, L"-대화초대중,존재하지 않는 친구:" << *sit ) << END_LOG;
            continue;
        }

        if( IsBlockedFriend_S2( *sit ) == true )
        {
            // 블럭인 친구UID.
            setUnableUIDs_.insert( *sit );
            START_LOG( clog, L"-대화초대중,블럭 친구:" << *sit ) << END_LOG;
            continue;
        }

        setCorrectUIDs.insert( *sit );
    }

    //1차 유효한 데이터
    setInviteUIDs_ = setCorrectUIDs;
    setCorrectUIDs.clear();

    // 대화초대할 수 있는 상황인지
    for( sit = setInviteUIDs_.begin() ; sit != setInviteUIDs_.end() ; ++sit )
    {
        for( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit )
        {
            mit2 = mit->second.m_mapMembers.find( *sit );

            if( mit2 != mit->second.m_mapMembers.end() )
            {
                break;
            }
        }

        if( mit == m_mapFriendGroup.end() )
        {
            // 존재하지 않는 친구UID.
            setUnableUIDs_.insert( *sit );
            START_LOG( clog, L"-대화초대중,존재하지 않는 친구:" << *sit ) << END_LOG;
            continue;
        }

        if( mit2->second.m_bIsOnline == false )
        {
            // 상대가 오프라인입니다.
            setUnableUIDs_.insert( *sit );
            START_LOG( clog, L"-대화초대중,오프라인 친구:" << *sit ) << END_LOG;
            continue;
        }

        if( mit2->second.m_bIsGamming == true )
        {
            // 대상이 게임중입니다(초대는 게임중이 아닐때만 가능합니다.)
            setUnableUIDs_.insert( *sit );
            START_LOG( clog, L"-대화초대중,게임중 친구:" << *sit ) << END_LOG;
            continue;
        }

        if( mit2->second.m_cChatRoomCount >= MAX_CHAT_ROOM_LIMIT )
        {
            // 대상의 모든 대화방이 꽉 찼습니다.
            setUnableUIDs_.insert( *sit );
            START_LOG( clog, L"-대화초대중,대화방이 꽉찬 친구:" << *sit ) << END_LOG;
            continue;
        }

        //초대 가능한 유저리스트
        setCorrectUIDs.insert( *sit );
        mapMemberInfo_.insert( std::make_pair( *sit, std::make_pair(mit2->second.m_strNickName, mit2->second.m_strNickColor) ) );
        vecRecvUIDs_.push_back( *sit );
    }

    //초대 가능한 유저리스트
    setInviteUIDs_ = setCorrectUIDs;
    setCorrectUIDs.clear();

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( setInviteUIDs_.size() )
        << BUILD_LOG( mapMemberInfo_.size() )
        << BUILD_LOG( setUnableUIDs_.size() )
        << BUILD_LOG( vecRecvUIDs_.size() ) << END_LOG;
}

void KMSUser::MakeChatRoom( IN const std::pair<DWORD,DWORD>& prRoomKey_, IN const std::map<DWORD,std::pair<std::wstring,std::wstring>>& mapMemberInfo_ )
{
    //대화방 등록
    m_mapChatRoomInfo.insert( std::make_pair( prRoomKey_, mapMemberInfo_ ) );

    //대화방 현재수 갱신
    m_cChatRoomCount = m_mapChatRoomInfo.size();
}

void KMSUser::CloseChatRoom( IN const std::pair<DWORD,DWORD>& prRoomKey_ )
{
    KMyChatRoom::iterator mit;
    mit = m_mapChatRoomInfo.find( prRoomKey_ );

    if( mit != m_mapChatRoomInfo.end() )
    {
        m_mapChatRoomInfo.erase( mit );//대화방 삭제
        m_cChatRoomCount = m_mapChatRoomInfo.size(); //대화방 현재수 갱신
    }

    //내 채팅방 카운트정보 친구들에게 전달.
    SendUpdateMyChatCountNot();
}

void KMSUser::SendUpdateMyChatCountNot()
{
    KEMS_S2_UPDATE_MY_CHAT_COUNT_NOT kPacket;
    kPacket.m_dwSenderUID = GetUID();
    kPacket.m_cChatRoomCount = GetChatRoomCount();

    std::vector<DWORD> vecFriendUIDList;

    std::map<int,KGroup>::const_iterator cmit;
    for( cmit = m_mapFriendGroup.begin() ; cmit != m_mapFriendGroup.end() ; ++cmit )
    {
        std::map<DWORD,KFriend>::const_iterator cmitFriend;
        for( cmitFriend = cmit->second.m_mapMembers.begin(); cmitFriend != cmit->second.m_mapMembers.end(); ++cmitFriend )
        {
            // 차단한 친구라면 알릴 필요 없다.
            if( IsBlockedFriend_S2( cmit->first ) ) continue;
            // 접속중이 아니면 보내지 않는다
            if( !cmitFriend->second.m_bIsOnline ) continue;

            vecFriendUIDList.push_back( cmitFriend->first );   // 전송할 친구들의 UID를 따로 추려낸다..
        }
    }

    if( !vecFriendUIDList.empty() )
    {
        // 이제는 모다서 보내자.
        REMOTE_QUEUEING_PACK( vecFriendUIDList, EMS_S2_UPDATE_MY_CHAT_COUNT_NOT, kPacket );
        m_ReservedLoc_S2.m_cChatRoomCount = kPacket.m_cChatRoomCount;
    }

}

void KMSUser::LeaveAllChatRoom()
{
    //해당 채팅방을 떠난다고 통보할 것.
    KMyChatRoom::const_iterator cmit;
    for ( cmit = m_mapChatRoomInfo.begin() ; cmit != m_mapChatRoomInfo.end() ; ++cmit ) {

        std::vector<DWORD> vecSendUIDs;

        //패킷을 보낼 채팅방 UID 리스트를 뽑아내자
        std::map<DWORD,std::pair<std::wstring,std::wstring>>::const_iterator cmitUser;
        for ( cmitUser = cmit->second.begin() ; cmitUser != cmit->second.end() ; ++cmitUser ) {
            if ( cmitUser->first != GetUID() ) {
                vecSendUIDs.push_back( cmitUser->first );
            }
        }

        if ( !vecSendUIDs.empty() ) {
            KEMS_S2_LEAVE_CHAT_MEMBER_NOT kPacket;
            kPacket.m_prRoomKey = cmit->first;
            kPacket.m_dwLeaveUID = GetUID();

            //그 채팅방을 떠났다고 통보
            REMOTE_QUEUEING_PACK( vecSendUIDs, EMS_S2_LEAVE_CHAT_MEMBER_NOT, kPacket );
        }
    }

    START_LOG( clog, L"LeaveAllChatRoom")
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName ) << END_LOG;
}

bool KMSUser::IsSealedPaper( IN const DWORD dwPaperUID_ )
{
    KPAPERBOX::iterator mit = m_mapSentPaperBox.find( dwPaperUID_ );

    if( mit == m_mapSentPaperBox.end() ) return false;

    return ( mit->second.m_cState == KPaper::SEALED );
}

bool KMSUser::IsMySentPaper( IN const std::set<DWORD>& setPaperUIDs_ )
{
    std::set<DWORD>::const_iterator csit;

    //상태를 바꿀 쪽지UID가 있는지 받은쪽지함과 비교해보자.
    for( csit = setPaperUIDs_.begin() ; csit != setPaperUIDs_.end() ; ++csit )
    {
        if( m_mapSentPaperBox.find( *csit ) == m_mapSentPaperBox.end() )
            break;
    }
    //정상적으로 다 찾았으면 end()
    if( csit != setPaperUIDs_.end() )
    {
        return false;
    }

    return true;
}

bool KMSUser::IsMyRecvPaper( IN const std::set<DWORD>& setPaperUIDs_ )
{
    std::set<DWORD>::const_iterator csit;

    //상태를 바꿀 쪽지UID가 있는지 받은쪽지함과 비교해보자.
    for( csit = setPaperUIDs_.begin() ; csit != setPaperUIDs_.end() ; ++csit )
    {
        if( m_mapRecvPaperBox.find( *csit ) == m_mapRecvPaperBox.end() )
            break;
    }
    //정상적으로 다 찾았으면 end()
    if( csit != setPaperUIDs_.end() )
    {
        return false;
    }

    return true;
}

bool KMSUser::SetMyRecvPaper( IN const std::set<DWORD>& setPaperUIDs_, IN const int nState_ )
{
    std::set<DWORD>::const_iterator csit;
    KPAPERBOX::iterator mit;

    for( csit = setPaperUIDs_.begin() ; csit != setPaperUIDs_.end() ; ++csit )
    {
        mit = m_mapRecvPaperBox.find( *csit );

        //쪽지함에 없는 쪽지이면 예외처리.
        if( mit == m_mapRecvPaperBox.end() ) break;

        //읽었다로 변경.
        mit->second.m_cState = nState_;
    }
    //예외처리
    if( csit != setPaperUIDs_.end() )
    {
        return false;
    }

    return true;
}

int KMSUser::GetSealedPaperCount()
{
    int nCount = 0;
    for( KPAPERBOX::const_iterator cmit = m_mapSentPaperBox.begin() ;
       cmit != m_mapSentPaperBox.end() ; ++cmit)
    {
        if( cmit->second.m_cState == KPaper::SEALED )
            ++nCount;
    }

    return nCount;
}

void KMSUser::LogoutLog()
{
    //EMS_S2_LOGOUT_LOG_NOT
    KEMS_S2_LOGOUT_LOG_NOT kPacket;
    kPacket.m_uiIP     = ::ntohl( GetIP() );
    kPacket.m_tmLogin   = KncUtil::TimeToInt( m_tmLoginTime );
    kPacket.m_tmLogout  = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    kPacket.m_dwChannelType = GetUserChannelType();
    QUEUEING_EVENT_TO_DB( EMS_S2_LOGOUT_LOG_NOT, kPacket );

}

void KMSUser::SetRecvUIDs( IN const std::set<DWORD> setUID_, OUT std::vector<DWORD>& vecUID_ )
{
    vecUID_.clear();
    vecUID_.reserve( setUID_.size() );

    std::set<DWORD>::const_iterator csit;
    for ( csit = setUID_.begin() ; csit != setUID_.end() ; ++csit ) {
        vecUID_.push_back( *csit );
    }
}