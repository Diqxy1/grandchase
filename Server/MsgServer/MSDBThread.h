#pragma once
#include "SubThread.h"
#include "IntEvent.h"
#include "MSPacket.h"
#include "odbc/Odbc.h"
#include "MSEvent.h"


class KMSSimLayer;
class KMSDBThread : public KSubThread< KMSEvent >
{
    typedef KMSSimLayer SLayerType;
    DeclareDBThread;
    DeclareDBThreadName( Main );

public:

    KMSDBThread();
    virtual ~KMSDBThread(void);

protected:
    // derived from KThreadManager
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );

    bool UpdateJoinGuildMemberInfo( IN const DWORD dwGuildUID_, const DWORD dwUserUID_, OUT KFriend& kInfo_ );
    void GetGuildMemeberInfoFromDB( IN DWORD dwGuildUID, IN DWORD dwUserUID, OUT KNGuildUserInfo& kGuildUserInfo, OUT time_t& tmJoinDate, OUT time_t& tmLastLogin );

protected:

    INT_DECL_ON_FUNC( DB_EVENT_SERVER_INFO_REQ );
    //New Packet
    INT_DECL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_REQ );
    INT_DECL_ON_FUNC( EMS_S2_ADD_FRIEND_REQ );
    INT_DECL_ON_FUNC( EMS_S2_DELETE_FRIEND_REQ );
   _INT_DECL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_REQ, DWORD );
   _INT_DECL_ON_FUNC( EMS_S2_REJECT_FRIEND_REQ, DWORD );
    INT_DECL_ON_FUNC( EMS_S2_BLOCK_FRIEND_REQ );
   _INT_DECL_ON_FUNC( EMS_S2_UNBLOCK_FRIEND_REQ, DWORD );
    INT_DECL_ON_FUNC( EMS_S2_BLOCK_MOVE_FRIEND_REQ );
   _INT_DECL_ON_FUNC( EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, DWORD );
    //Group
   _INT_DECL_ON_FUNC( EMS_S2_ADD_GROUP_REQ, KEMS_S2_ADD_GROUP_ACK );
   _INT_DECL_ON_FUNC( EMS_S2_DELETE_GROUP_REQ, int );
    INT_DECL_ON_FUNC( EMS_S2_RENAME_GROUP_REQ );
    INT_DECL_ON_FUNC( EMS_S2_MOVE_FRIEND_TO_GROUP_REQ );
    //Emoticon
   _INT_DECL_ON_FUNC( EMS_S2_SET_EMOTICON_REQ, DWORD );
   // 쪽지
    INT_DECL_ON_FUNC( EMS_S2_SEND_PAPER_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EMS_S2_LOAD_SENT_PAPERBOX_REQ );
    INT_DECL_ON_FUNC_NOPARAM( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
   _INT_DECL_ON_FUNC( EMS_S2_READ_PAPER_REQ, std::set<DWORD> );
   _INT_DECL_ON_FUNC( EMS_S2_DEL_SENT_PAPER_REQ, std::set<DWORD> );
   _INT_DECL_ON_FUNC( EMS_S2_DEL_RECV_PAPER_REQ, std::set<DWORD> );
   _INT_DECL_ON_FUNC( EMS_S2_SEAL_PAPER_REQ, std::set<DWORD> );
    INT_DECL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_REQ );
    INT_DECL_ON_FUNC( EMS_S2_LOGOUT_LOG_NOT );

   _INT_DECL_ON_FUNC( EMS_ADD_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq );

    INT_DECL_ON_FUNC( EMS_GUILD_ALL_MEMBER_LIST_REQ );


};
