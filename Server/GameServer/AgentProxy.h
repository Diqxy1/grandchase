#pragma once
#include "ActorProxy.h"
#include <windows.h>
#include "Thread/Thread.h"
#include "AgentEvent.h"
#include "AgentPacket.h"
#include <ToString.h>
SmartPointer( KAgentProxy );

class KGSSimLayer;
class KUserEvent;
class KAgentConnector;
class KAgentProxy : public KActorProxy
{
    typedef KGSSimLayer SLayerType;
    typedef KUserEvent IDVenderType;
    NiDeclareRTTI;
    DeclToStringW;

public:
    KAgentProxy(void);
    virtual ~KAgentProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    virtual KEvent* CreateKEvent() { return new KAgentEvent; }

    bool Connect( std::string strIP, unsigned short usPort );
    bool IsAfterVerify() { return m_bAfterVerify; };
    bool CheckRecvUser( IN const std::map<DWORD,std::vector<DWORD> >& mapServerUser );

protected:
    HANDLE          m_hVerifyEvent;
    int             m_nSavedResult;
    bool            m_bAfterVerify;

    friend class    KAgentConnector;

protected:
    DECL_ON_FUNC( EAG_VERIFY_ACCOUNT_ACK );
   _DECL_ON_FUNC( EAG_SERVER_RELAY_PACKET_REQ, KServerRelayPacket );
   _DECL_ON_FUNC( EAG_AGIT_RELAY_PACKET_REQ, KAgitRelayPacket );

   _DECL_ON_FUNC( EAG_AGIT_CONFIG_NOT, KAgitConfig );
   _DECL_ON_FUNC( EAG_AGIT_MAP_NOT, KAgitMapInfo );
   _DECL_ON_FUNC( EAG_AGIT_FLOWER_NOT, KFlowerPotConfig );
   _DECL_ON_FUNC( EAG_ENTER_AGIT_ACK, KEnterAgitAck );
   _DECL_ON_FUNC( EAG_ENTER_AGIT_BROAD, KAgitUserInfo );
   _DECL_ON_FUNC( EAG_AGIT_LOADING_COMPLETE_ACK, KAgitLoadingCompleteAck );
    DECL_ON_FUNC( EAG_ENTER_AGIT_BROAD );
    DECL_ON_FUNC( EAG_LEAVE_AGIT_ACK );
    DECL_ON_FUNC( EAG_LEAVE_AGIT_BROAD );

    _DECL_ON_FUNC( EAG_BUY_AGIT_MAP_ACK, KBuyAgitMapAck );
    DECL_ON_FUNC( EAG_CHANGE_AGIT_MAP_BROAD );
   _DECL_ON_FUNC( EAG_SET_AGIT_OPTION_ACK, KSetAgitOptionAck );
   _DECL_ON_FUNC( EAG_SEED_FLOWER_POT_ACK, KSeedFlowerPotAck );
    DECL_ON_FUNC( EAG_CHANGE_FLOWER_POT_STATE_BROAD );
   _DECL_ON_FUNC( EAG_HARVEST_FLOWER_POT_ACK, KHarvestFlowerPotAck );

   _DECL_ON_FUNC( EAG_AGIT_CHAT_ACK, KAgitChatAck );
    DECL_ON_FUNC( EAG_AGIT_CHAT_BROAD );
   _DECL_ON_FUNC( EAG_AGIT_BAN_USER_ACK, KAgitBanUserAck );
   _DECL_ON_FUNC( EAG_AGIT_BAN_USER_NOT, KAgitBanUserReq );

   _DECL_ON_FUNC( EAG_SET_AGIT_OBJECT_POS_ACK, KSetAgitObjectPosAck );
    DECL_ON_FUNC( EAG_CHANGE_AGIT_OBJECT_POS_BROAD );
   _DECL_ON_FUNC( EAG_AGIT_FRIEND_LIST_ACK, KSimpleAgitPageList );
   _DECL_ON_FUNC( EAG_AGIT_TOP_RANK_LIST_ACK, KSimpleAgitPageList );
   _DECL_ON_FUNC( EAG_AGIT_FAVORITE_LIST_ACK, KSimpleAgitPageList );
   _DECL_ON_FUNC( EAG_AGIT_ADD_FAVORITE_ACK, KAgitAddFavoriteAck );
   _DECL_ON_FUNC( EAG_AGIT_REMOVE_FAVORITE_ACK, KAgitUserParamAck );

   _DECL_ON_FUNC( EAG_REGISTER_AGIT_PAGE_ACK, KRegisterAgitPageAck );
   _DECL_ON_FUNC( EAG_UNREGISTER_AGIT_PAGE_ACK, DWORD );
   _DECL_ON_FUNC( EAG_AGIT_PAGE_LIST_ACK, KAgitPageList );

   _DECL_ON_FUNC( EAG_EQUIP_BROWNIE_ACK, KEquipBrownieAck );
    DECL_ON_FUNC( EAG_CHANGE_BROWNIE_BROAD );

   _DECL_ON_FUNC( EAG_WATERING_FAIRY_TREE_ACK, KWateringFairyTreeAck );
    DECL_ON_FUNC( EAG_CHANGE_FAIRY_TREE_BROAD );
   _DECL_ON_FUNC( EAG_FAIRY_TREE_BUFF_NOT, KFairyTreeFruitBuff );
   _DECL_ON_FUNC( EAG_USE_FAIRY_TREE_FRUIT_ACK, KUseFairyTreeFruitAck );
    DECL_ON_FUNC( EAG_AGIT_VISITER_COUNT_BROAD );

   _DECL_ON_FUNC( EAG_ENTER_AGIT_SID_ACK, KEnterAgitSIDAck );
   _DECL_ON_FUNC( EAG_SET_AGIT_SID_BROAD, PAIR_DWORD_INT );
   _DECL_ON_FUNC( EAG_AGIT_FAIRY_TREE_NOT, KFairyTreeConfig );
    DECL_ON_FUNC( EAG_CHANGE_AGIT_OPTION_BROAD );

   _DECL_ON_FUNC( EAG_GUESTBOOK_PAGE_ACK, KGuestBookPageList );
   _DECL_ON_FUNC( EAG_GUESTBOOK_WRITE_ACK, KAgitUserParamAck );
   _DECL_ON_FUNC( EAG_GUESTBOOK_DELETE_ACK, KAgitUserParamAck );
    DECL_ON_FUNC( EAG_GUESTBOOK_WRITE_BROAD );
    DECL_ON_FUNC( EAG_GUESTBOOK_DELETE_BROAD );

   _DECL_ON_FUNC( EAG_USE_TRAINING_OBJ_ACK, KUseTrainingObjAck );
    DECL_ON_FUNC( EAG_UPDATE_TRAINING_OBJ_BROAD );

   _DECL_ON_FUNC( EAG_OPEN_AGIT_CHARACTER_ACK, KOpenAgitCharacterAck );
   _DECL_ON_FUNC( EAG_SET_AGIT_CHARACTER_POS_ACK, KSetAgitCharPosAck );
   _DECL_ON_FUNC( EAG_SET_AGIT_CHARACTER_MOTION_ACK, KSetAgitCharMotionAck );
   _DECL_ON_FUNC( EAG_SET_AGIT_CHARACTER_COORDI_ACK, KSetAgitCharCoordiAck );
   _DECL_ON_FUNC( EAG_SET_AGIT_CHARACTER_SPEECH_ACK, KSetAgitCharSpeechAck );
    DECL_ON_FUNC( EAG_UPDATE_AGIT_CHARACTER_BROAD );
    DECL_ON_FUNC( EAG_UPDATE_AGIT_CHARACTER_POS_BROAD );
};
DeclOstmOperatorA( KAgentProxy );