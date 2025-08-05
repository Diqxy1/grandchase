#include "CenterPacket.h"

SERIALIZE_DEFINE_PUT(KECN_VERIFY_ACCOUNT_REQ, obj, ks)
{
	return  PUT(m_nProtocolVer) &&
		PUT(m_nUserProtocolVer) &&
		PUT(m_strName) &&
		PUT(m_strIP) &&
		PUT(m_usPort) &&
		PUT(m_dwUID);
}

SERIALIZE_DEFINE_GET(KECN_VERIFY_ACCOUNT_REQ, obj, ks)
{
	return  GET(m_nProtocolVer) &&
		GET(m_nUserProtocolVer) &&
		GET(m_strName) &&
		GET(m_strIP) &&
		GET(m_usPort) &&
		GET(m_dwUID);
}

SERIALIZE_DEFINE_PUT(KECN_VERIFY_ACCOUNT_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kServerInfo) &&
		PUT(m_vecSquares);
}

SERIALIZE_DEFINE_GET(KECN_VERIFY_ACCOUNT_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kServerInfo) &&
		GET(m_vecSquares);
}

SERIALIZE_DEFINE_PUT(KECN_KICK_USER_NOT, obj, ks)
{
	return  PUT(m_dwDestUserUID) &&
		PUT(m_nReason) &&
		PUT(m_wstrLogin);
}

SERIALIZE_DEFINE_GET(KECN_KICK_USER_NOT, obj, ks)
{
	return  GET(m_dwDestUserUID) &&
		GET(m_nReason) &&
		GET(m_wstrLogin);
}

SERIALIZE_DEFINE_PUT(KECN_REPETITION_PROVIDE_REQ, obj, ks)
{
	return  PUT(m_dwEventUID) &&
		PUT(m_nCommentID);
}

SERIALIZE_DEFINE_GET(KECN_REPETITION_PROVIDE_REQ, obj, ks)
{
	return  GET(m_dwEventUID) &&
		GET(m_nCommentID);
}
SERIALIZE_DEFINE_PUT(KMiniGameRankList, obj, ks)
{
	return PUT(m_mapRank);
}

SERIALIZE_DEFINE_GET(KMiniGameRankList, obj, ks)
{
	return GET(m_mapRank);
}

SERIALIZE_DEFINE_PUT(KConnectedUser, obj, ks)
{
	return  PUT(m_dwUID);
}

SERIALIZE_DEFINE_GET(KConnectedUser, obj, ks)
{
	return  GET(m_dwUID);
}

SERIALIZE_DEFINE_PUT(KECN_RAINBOW_UPDATE_NOT, obj, ks)
{
	return  PUT(m_nRainBowID) &&
		PUT(m_nDailyEventID) &&
		PUT(m_nJackPotEventID) &&
		PUT(m_vecEventList);
}

SERIALIZE_DEFINE_GET(KECN_RAINBOW_UPDATE_NOT, obj, ks)
{
	return  GET(m_nRainBowID) &&
		GET(m_nDailyEventID) &&
		GET(m_nJackPotEventID) &&
		GET(m_vecEventList);
}

SERIALIZE_DEFINE_PUT(KECN_MISSION_EVENT_UPDATE_NOT, obj, ks)
{
	return  PUT(m_vecMissions) &&
		PUT(m_setItemList) &&
		PUT(m_bDateChanged);
}

SERIALIZE_DEFINE_GET(KECN_MISSION_EVENT_UPDATE_NOT, obj, ks)
{
	return  GET(m_vecMissions) &&
		GET(m_setItemList) &&
		GET(m_bDateChanged);
}

SERIALIZE_DEFINE_PUT(KECN_MISSION_NOTIFY_NOT, obj, ks)
{
	return  PUT(m_dwTime);
}

SERIALIZE_DEFINE_GET(KECN_MISSION_NOTIFY_NOT, obj, ks)
{
	return  GET(m_dwTime);
}

SERIALIZE_DEFINE_PUT(KECN_ITEM_DONATION_UPDATE_NOT, obj, ks)
{
	return  PUT(m_vecGradeInfo);
}

SERIALIZE_DEFINE_GET(KECN_ITEM_DONATION_UPDATE_NOT, obj, ks)
{
	return  GET(m_vecGradeInfo);
}

SERIALIZE_DEFINE_PUT(KECN_ENTER_SQUARE_ACK, obj, ks) // Center -> Center Proxy
{
	return  PUT(m_nOK) &&
		PUT(m_dwUserUID) &&
		PUT(m_dwSquareUID) &&
		PUT(m_vecUserInfo) &&
		PUT(m_dwRelayServerIP) &&
		PUT(m_usRelayServerPort) &&
		PUT(m_kMyPos);
};

SERIALIZE_DEFINE_GET(KECN_ENTER_SQUARE_ACK, obj, ks) // Center -> Center Proxy
{
	return  GET(m_nOK) &&
		GET(m_dwUserUID) &&
		GET(m_dwSquareUID) &&
		GET(m_vecUserInfo) &&
		GET(m_dwRelayServerIP) &&
		GET(m_usRelayServerPort) &&
		GET(m_kMyPos);
};

SERIALIZE_DEFINE_PUT(KECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT, obj, ks) // Center -> Center Proxy
{
	return  PUT(m_dwUID) &&
		PUT(m_vecNeighborhood);
}
SERIALIZE_DEFINE_GET(KECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT, obj, ks) // Center -> Center Proxy
{
	return  GET(m_dwUID) &&
		GET(m_vecNeighborhood);
}

SERIALIZE_DEFINE_PUT(KECN_LEAVE_SQUARE_BROAD, obj, ks) // Center -> Center Proxy
{
	return PUT(m_dwUID) &&
		PUT(m_dwSquareUID);
};

SERIALIZE_DEFINE_GET(KECN_LEAVE_SQUARE_BROAD, obj, ks) // Center -> Center Proxy
{
	return GET(m_dwUID) &&
		GET(m_dwSquareUID);
};

SERIALIZE_DEFINE_PUT(KECN_ENTER_GUILD_SQUARE_REQ, obj, ks)
{
	return  PUT(m_kSquareUserInfo) &&
		PUT(m_iGuildSquareIndex);
};

SERIALIZE_DEFINE_GET(KECN_ENTER_GUILD_SQUARE_REQ, obj, ks)
{
	return  GET(m_kSquareUserInfo) &&
		GET(m_iGuildSquareIndex);
};

SERIALIZE_DEFINE_PUT(KECN_ENTER_GUILD_SQUARE_ACK, obj, ks)
{
	return  PUT(m_kEnterSquare) &&
		PUT(m_kSquareInfo);
};

SERIALIZE_DEFINE_GET(KECN_ENTER_GUILD_SQUARE_ACK, obj, ks)
{
	return  GET(m_kEnterSquare) &&
		GET(m_kSquareInfo);
};

SERIALIZE_DEFINE_PUT(KECN_ADD_GUILD_USER_REQ, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_kUserInfo);
};

SERIALIZE_DEFINE_GET(KECN_ADD_GUILD_USER_REQ, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_kUserInfo);
};

SERIALIZE_DEFINE_PUT(KECN_CHAT_REQ, obj, ks)
{
	return  PUT(m_kChatData) &&
		PUT(m_dwServerUID);
};

SERIALIZE_DEFINE_GET(KECN_CHAT_REQ, obj, ks)
{
	return  GET(m_kChatData) &&
		GET(m_dwServerUID);
};

SERIALIZE_DEFINE_PUT(KECN_CHAT_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kChatData) &&
		PUT(m_dwServerUID);
};

SERIALIZE_DEFINE_PUT( KECNBillboardChat , obj, ks )
{
	return  PUT( Nickname )		&&
			PUT( MessageColor )	&&
			PUT( Message );
};	

SERIALIZE_DEFINE_GET( KECNBillboardChat , obj, ks )
{
	return  GET( Nickname )		&&
			GET( MessageColor )	&&
			GET( Message );
};

SERIALIZE_DEFINE_GET(KECN_CHAT_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kChatData) &&
		GET(m_dwServerUID);
};

SERIALIZE_DEFINE_PUT(KECN_LEAVE_ROOM_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_dwUserUID);
};

SERIALIZE_DEFINE_GET(KECN_LEAVE_ROOM_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_dwUserUID);
};
SERIALIZE_DEFINE_PUT(KECN_JOIN_ROOM_BROAD, obj, ks)
{
	return  PUT(m_usRoomID) &&
		PUT(m_kIndoorUserInfo);
};

SERIALIZE_DEFINE_GET(KECN_JOIN_ROOM_BROAD, obj, ks)
{
	return  GET(m_usRoomID) &&
		GET(m_kIndoorUserInfo);
};
SERIALIZE_DEFINE_PUT(KECN_LEAVE_ROOM_BROAD, obj, ks)
{
	return  PUT(m_usRoomID) &&
		PUT(m_dwUserUID);
};

SERIALIZE_DEFINE_GET(KECN_LEAVE_ROOM_BROAD, obj, ks)
{
	return  GET(m_usRoomID) &&
		GET(m_dwUserUID);
};

SERIALIZE_DEFINE_PUT(KECN_CHANGE_ROOMUSER_INFO_REQ, obj, ks)
{
	return  PUT(m_kInDoorUserInfo) &&
		PUT(m_kChangeRoomUserInfo);
};

SERIALIZE_DEFINE_GET(KECN_CHANGE_ROOMUSER_INFO_REQ, obj, ks)
{
	return  GET(m_kInDoorUserInfo) &&
		GET(m_kChangeRoomUserInfo);
};

SERIALIZE_DEFINE_PUT(KECN_CHANGE_ROOMUSER_INFO_BROAD, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_usRoomID) &&
		PUT(m_ucChangeType) &&
		PUT(m_kInDoorUserInfo);
};

SERIALIZE_DEFINE_GET(KECN_CHANGE_ROOMUSER_INFO_BROAD, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_usRoomID) &&
		GET(m_ucChangeType) &&
		GET(m_kInDoorUserInfo);
};

SERIALIZE_DEFINE_PUT(KECN_HOST_MIGRATED_NOT, obj, ks)
{
	return  PUT(m_usRoomID) &&
		PUT(m_dwHostUID);
};

SERIALIZE_DEFINE_GET(KECN_HOST_MIGRATED_NOT, obj, ks)
{
	return  GET(m_usRoomID) &&
		GET(m_dwHostUID);
};

SERIALIZE_DEFINE_PUT(KECN_START_GAME_BROAD, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_usRoomID) &&
		PUT(m_dwRandomSeed) &&
		PUT(m_vecStartingUsers) &&
		PUT(m_dwUniqueNumber) &&
		PUT(m_dwUserUID);
};

SERIALIZE_DEFINE_GET(KECN_START_GAME_BROAD, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_usRoomID) &&
		GET(m_dwRandomSeed) &&
		GET(m_vecStartingUsers) &&
		GET(m_dwUniqueNumber) &&
		GET(m_dwUserUID);
};

SERIALIZE_DEFINE_PUT(KECN_LEAVE_GAME_REQ, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_usRoomID) &&
		PUT(m_kGameResult);
};

SERIALIZE_DEFINE_GET(KECN_LEAVE_GAME_REQ, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_usRoomID) &&
		GET(m_kGameResult);
};

SERIALIZE_DEFINE_PUT(KECN_ACCEPT_GUILD_JOINER_NOT, obj, ks)
{
	return  PUT(m_iGuildID) &&
		PUT(m_setNewcomerList);
};

SERIALIZE_DEFINE_GET(KECN_ACCEPT_GUILD_JOINER_NOT, obj, ks)
{
	return  GET(m_iGuildID) &&
		GET(m_setNewcomerList);
};

SERIALIZE_DEFINE_PUT(KECN_REJECT_GUILD_JOINER_NOT, obj, ks)
{
	return  PUT(m_iGuildID) &&
		PUT(m_setRejectList);
};

SERIALIZE_DEFINE_GET(KECN_REJECT_GUILD_JOINER_NOT, obj, ks)
{
	return  GET(m_iGuildID) &&
		GET(m_setRejectList);
};

SERIALIZE_DEFINE_PUT(KECN_PREMIUM_NOT, obj, ks)
{
	return  PUT(m_usRoomID) &&
		PUT(m_mapPremium);
};

SERIALIZE_DEFINE_GET(KECN_PREMIUM_NOT, obj, ks)
{
	return  GET(m_usRoomID) &&
		GET(m_mapPremium);
};

SERIALIZE_DEFINE_PUT(KECN_GUILD_MEMEBER_LIST_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_mapGuildUserList);
};

SERIALIZE_DEFINE_GET(KECN_GUILD_MEMEBER_LIST_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_mapGuildUserList);
};

SERIALIZE_DEFINE_PUT(KNOneGuildData, obj, ks)
{
	return  PUT(m_kGuildInfo) &&
		PUT(m_vecGuildNotice) &&
		PUT(m_mapGuildUserList);
};

SERIALIZE_DEFINE_GET(KNOneGuildData, obj, ks)
{
	return  GET(m_kGuildInfo) &&
		GET(m_vecGuildNotice) &&
		GET(m_mapGuildUserList);
};

SERIALIZE_DEFINE_PUT(KECN_UPDATE_GUILD_MEMBER_COMMENT_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_dwUserUID) &&
		PUT(m_strUserComment);
};

SERIALIZE_DEFINE_GET(KECN_UPDATE_GUILD_MEMBER_COMMENT_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_dwUserUID) &&
		GET(m_strUserComment);
};

SERIALIZE_DEFINE_PUT(KECN_GUILD_LIST_NOT, obj, ks)
{
	return  PUT(m_mapGuildList) &&
		PUT(m_mapGuildNotice) &&
		PUT(m_mapGuildMember);
};

SERIALIZE_DEFINE_GET(KECN_GUILD_LIST_NOT, obj, ks)
{
	return  GET(m_mapGuildList) &&
		GET(m_mapGuildNotice) &&
		GET(m_mapGuildMember);
};

SERIALIZE_DEFINE_PUT(KECN_DRUMOUT_GUILD_USER_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_setUserUID) &&
		PUT(m_cOutType);
};

SERIALIZE_DEFINE_GET(KECN_DRUMOUT_GUILD_USER_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_setUserUID) &&
		GET(m_cOutType);
};

SERIALIZE_DEFINE_PUT(KECN_CHANGE_GUILD_MEMBER_LEVEL_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_mapChangeMemberLevel);
};

SERIALIZE_DEFINE_GET(KECN_CHANGE_GUILD_MEMBER_LEVEL_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_mapChangeMemberLevel);
};

SERIALIZE_DEFINE_PUT(KECN_UPDATE_GUILD_NOTICE_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_vecNotice);
};

SERIALIZE_DEFINE_GET(KECN_UPDATE_GUILD_NOTICE_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_vecNotice);
};

SERIALIZE_DEFINE_PUT(KECN_UPDATE_GUILD_NAME_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_strGuildName);
};

SERIALIZE_DEFINE_GET(KECN_UPDATE_GUILD_NAME_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_strGuildName);
};

SERIALIZE_DEFINE_PUT(KECN_JOIN_GUILD_REQ, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID) &&
		PUT(m_strMyComment) &&
		PUT(m_kGuildInfo);
};

SERIALIZE_DEFINE_GET(KECN_JOIN_GUILD_REQ, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_dwGuildUID) &&
		GET(m_strMyComment) &&
		GET(m_kGuildInfo);
};

SERIALIZE_DEFINE_PUT(KECN_JOIN_GUILD_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID) &&
		PUT(m_kGuildUserInfo) &&
		PUT(m_kGuildInfo);
};

SERIALIZE_DEFINE_GET(KECN_JOIN_GUILD_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_dwUserUID) &&
		GET(m_dwGuildUID) &&
		GET(m_kGuildUserInfo) &&
		GET(m_kGuildInfo);
};

SERIALIZE_DEFINE_PUT(KECN_ADD_GUILD_USER_BROAD_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_kGuildUserInfo);
};

SERIALIZE_DEFINE_GET(KECN_ADD_GUILD_USER_BROAD_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_kGuildUserInfo);
};

SERIALIZE_DEFINE_PUT(KECN_CANCEL_JOIN_GUILD_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID);
};

SERIALIZE_DEFINE_GET(KECN_CANCEL_JOIN_GUILD_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_dwUserUID) &&
		GET(m_dwGuildUID);
};

SERIALIZE_DEFINE_PUT(KECN_CHANGE_GUILD_MEMBER_LEVEL_REQ, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_dwGuildMasterUID) &&
		PUT(m_mapChangeMemberLevel);
}

SERIALIZE_DEFINE_GET(KECN_CHANGE_GUILD_MEMBER_LEVEL_REQ, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_dwGuildMasterUID) &&
		GET(m_mapChangeMemberLevel);
}

SERIALIZE_DEFINE_PUT(KECN_CHANGE_GUILD_MEMBER_LEVEL_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_dwGuildUID) &&
		PUT(m_dwGuildMasterUID) &&
		PUT(m_mapChangeMemberLevel);
}

SERIALIZE_DEFINE_GET(KECN_CHANGE_GUILD_MEMBER_LEVEL_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_dwGuildUID) &&
		GET(m_dwGuildMasterUID) &&
		GET(m_mapChangeMemberLevel);
}

SERIALIZE_DEFINE_PUT(KECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_mapChangeMemberLevel) &&
		PUT(m_cOutType);
}

SERIALIZE_DEFINE_GET(KECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_mapChangeMemberLevel) &&
		GET(m_cOutType);
}

SERIALIZE_DEFINE_PUT(KECN_ACCEPT_GUILD_JOINER_REQ, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID) &&
		PUT(m_setUserUID);
}

SERIALIZE_DEFINE_GET(KECN_ACCEPT_GUILD_JOINER_REQ, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_dwGuildUID) &&
		GET(m_setUserUID);
}

SERIALIZE_DEFINE_PUT(KECN_ACCEPT_GUILD_JOINER_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID) &&
		PUT(m_setUserUID);
}

SERIALIZE_DEFINE_GET(KECN_ACCEPT_GUILD_JOINER_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_dwUserUID) &&
		GET(m_dwGuildUID) &&
		GET(m_setUserUID);
}

SERIALIZE_DEFINE_PUT(KECN_BREAKUP_GUILD_REQ, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID);
}

SERIALIZE_DEFINE_GET(KECN_BREAKUP_GUILD_REQ, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_dwGuildUID);
}

SERIALIZE_DEFINE_PUT(KECN_BREAKUP_GUILD_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID);
}

SERIALIZE_DEFINE_GET(KECN_BREAKUP_GUILD_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_dwUserUID) &&
		GET(m_dwGuildUID);
}

SERIALIZE_DEFINE_PUT(KCnNGuildJoinPolicy, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_kPolicy);
}

SERIALIZE_DEFINE_GET(KCnNGuildJoinPolicy, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_kPolicy);
}

SERIALIZE_DEFINE_PUT(KECN_UPDATE_GUILD_MEMBER_LIST_REQ, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_mapMemberList);
}

SERIALIZE_DEFINE_GET(KECN_UPDATE_GUILD_MEMBER_LIST_REQ, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_mapMemberList);
}

SERIALIZE_DEFINE_PUT(KECN_LOG_OUT_NOT, obj, ks)
{
	return  PUT(m_strLogin) &&
		PUT(m_strPassword) &&
		PUT(m_dwUID) &&
		PUT(m_dwIPNum) &&
		PUT(m_dwAuthType);
}

SERIALIZE_DEFINE_GET(KECN_LOG_OUT_NOT, obj, ks)
{
	return  GET(m_strLogin) &&
		GET(m_strPassword) &&
		GET(m_dwUID) &&
		GET(m_dwIPNum) &&
		GET(m_dwAuthType);
}

SERIALIZE_DEFINE_PUT(KECN_QUIZ_BINGO_DATA_NOT, obj, ks)
{
	return  PUT(m_nEventID) &&
		PUT(m_tmBegin) &&
		PUT(m_tmEnd) &&
		PUT(m_mapCompressedQuestionList);
}

SERIALIZE_DEFINE_GET(KECN_QUIZ_BINGO_DATA_NOT, obj, ks)
{
	return  GET(m_nEventID) &&
		GET(m_tmBegin) &&
		GET(m_tmEnd) &&
		GET(m_mapCompressedQuestionList);
}

SERIALIZE_DEFINE_PUT(KECN_UPGRADE_GUILD_GRADE_GIFT_REQ, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_dwGuildMasterUID) &&
		PUT(m_kPostItem);
}

SERIALIZE_DEFINE_GET(KECN_UPGRADE_GUILD_GRADE_GIFT_REQ, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_dwGuildMasterUID) &&
		GET(m_kPostItem);
}

SERIALIZE_DEFINE_PUT(KECN_UPGRADE_GUILD_GRADE_GIFT_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_dwGuildUID) &&
		PUT(m_dwGuildMasterUID) &&
		PUT(m_WUID);
};

SERIALIZE_DEFINE_GET(KECN_UPGRADE_GUILD_GRADE_GIFT_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_dwGuildUID) &&
		GET(m_dwGuildMasterUID) &&
		GET(m_WUID);
};

SERIALIZE_DEFINE_PUT(KECN_SYNC_INDOORUSERINFO_BROAD, obj, ks)
{
	return PUT(m_usRoomID) &&
		PUT(m_mapInDoorUserInfo);
}

SERIALIZE_DEFINE_GET(KECN_SYNC_INDOORUSERINFO_BROAD, obj, ks)
{
	return GET(m_usRoomID) &&
		GET(m_mapInDoorUserInfo);
}

SERIALIZE_DEFINE_PUT(KECN_REMOVE_GUILD_USER_BROAD_NOT, obj, ks)
{
	return  PUT(m_dwGuildUID) &&
		PUT(m_dwUserUID) &&
		PUT(m_cOutType);
};

SERIALIZE_DEFINE_GET(KECN_REMOVE_GUILD_USER_BROAD_NOT, obj, ks)
{
	return  GET(m_dwGuildUID) &&
		GET(m_dwUserUID) &&
		GET(m_cOutType);
};
SERIALIZE_DEFINE_PUT(KECN_INVITE_GUILD_USER_REQ, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_dwGuildUID) &&
		PUT(m_vecInviteList) &&
		PUT(m_kInviteInfo);
};

SERIALIZE_DEFINE_GET(KECN_INVITE_GUILD_USER_REQ, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_dwGuildUID) &&
		GET(m_vecInviteList) &&
		GET(m_kInviteInfo);
};

SERIALIZE_DEFINE_PUT(KECN_INVITE_GUILD_USER_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_vecUserUID) &&
		PUT(m_kInviteInfo);
};

SERIALIZE_DEFINE_GET(KECN_INVITE_GUILD_USER_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_vecUserUID) &&
		GET(m_kInviteInfo);
};

SERIALIZE_DEFINE_PUT(KECN_UPDATE_GUILD_POINT_REQ, obj, ks)
{
	return  PUT(m_GuildUID) &&
		PUT(m_dwUserUID) &&
		PUT(m_dwDiffPoint) &&
		PUT(m_dwGuildPoint) &&
		PUT(m_dwBattlePoint);
};

SERIALIZE_DEFINE_GET(KECN_UPDATE_GUILD_POINT_REQ, obj, ks)
{
	return  GET(m_GuildUID) &&
		GET(m_dwUserUID) &&
		GET(m_dwDiffPoint) &&
		GET(m_dwGuildPoint) &&
		GET(m_dwBattlePoint);
};

SERIALIZE_DEFINE_PUT(KGuildPageInfo, obj, ks)
{
	return  PUT(m_ucSortType) &&
		PUT(m_nPageNum) &&
		PUT(m_kPageBuffer);
};

SERIALIZE_DEFINE_GET(KGuildPageInfo, obj, ks)
{
	return  GET(m_ucSortType) &&
		GET(m_nPageNum) &&
		GET(m_kPageBuffer);
};

SERIALIZE_DEFINE_PUT(KECN_SQUARE_USER_SYNC_ACK, obj, ks)
{
	return  PUT(m_kUserPos) &&
		PUT(m_vecPosUID);
};

SERIALIZE_DEFINE_GET(KECN_SQUARE_USER_SYNC_ACK, obj, ks)
{
	return  GET(m_kUserPos) &&
		GET(m_vecPosUID);
};

SERIALIZE_DEFINE_PUT(KECN_DEL_USER_REQ, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_nReason) &&
		PUT(m_strName);
};

SERIALIZE_DEFINE_GET(KECN_DEL_USER_REQ, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_nReason) &&
		GET(m_strName);
};

SERIALIZE_DEFINE_PUT(KECN_SOCKET_ITEM_GP_TABLE_NOT, obj, ks)
{
	return  PUT(m_mapItemUseCount) &&
		PUT(m_mapConsumeGP);
};

SERIALIZE_DEFINE_GET(KECN_SOCKET_ITEM_GP_TABLE_NOT, obj, ks)
{
	return  GET(m_mapItemUseCount) &&
		GET(m_mapConsumeGP);
};

SERIALIZE_DEFINE_PUT(KECN_GUILD_PAGE_NOT, obj, ks)
{
	return  PUT(m_ucSortType) &&
		PUT(m_kPageBuffer);
};

SERIALIZE_DEFINE_GET(KECN_GUILD_PAGE_NOT, obj, ks)
{
	return  GET(m_ucSortType) &&
		GET(m_kPageBuffer);
};

SERIALIZE_DEFINE_PUT(KECN_USER_SERVER_ROOM_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kSendUser) &&
		PUT(m_kServerRoomInfo);
};

SERIALIZE_DEFINE_GET(KECN_USER_SERVER_ROOM_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kSendUser) &&
		GET(m_kServerRoomInfo);
};

SERIALIZE_DEFINE_PUT(KECN_USER_SERVER_ROOM_REQ, obj, ks)
{
	return  PUT(m_kSendUser) &&
		PUT(m_kTargetUser);
};

SERIALIZE_DEFINE_GET(KECN_USER_SERVER_ROOM_REQ, obj, ks)
{
	return  GET(m_kSendUser) &&
		GET(m_kTargetUser);
};

SERIALIZE_DEFINE_PUT(KECN_SPHINX_DATA_NOT, obj, ks)
{
	return  PUT(m_nVersion) &&
		PUT(m_tmBegin) &&
		PUT(m_tmEnd) &&
		PUT(m_mapCompressedSphinxQuestionInfo);
}

SERIALIZE_DEFINE_GET(KECN_SPHINX_DATA_NOT, obj, ks)
{
	return  GET(m_nVersion) &&
		GET(m_tmBegin) &&
		GET(m_tmEnd) &&
		GET(m_mapCompressedSphinxQuestionInfo);
}

SERIALIZE_DEFINE_PUT(KECN_USER_EXIT_GAME_NOT, obj, ks)
{
	return  PUT(m_wstrLogin) &&
		PUT(m_wstrIP) &&
		PUT(m_nGameCode) &&
		PUT(m_biUniqueKey) &&
		PUT(m_nUniqueKey);
};

SERIALIZE_DEFINE_GET(KECN_USER_EXIT_GAME_NOT, obj, ks)
{
	return  GET(m_wstrLogin) &&
		GET(m_wstrIP) &&
		GET(m_nGameCode) &&
		GET(m_biUniqueKey) &&
		GET(m_nUniqueKey);
};

SERIALIZE_DEFINE_PUT(KECN_SURVEY_DATA_NOT, obj, ks)
{
	return  PUT(m_nVersion) &&
		PUT(m_tmBegin) &&
		PUT(m_tmEnd) &&
		PUT(m_tmReturnTime) &&
		PUT(m_mapCompressedSurveyInfoPack) &&
		PUT(m_mapSurveyTypeInfo) &&
		PUT(m_mapSurveyRewardInfo);
};

SERIALIZE_DEFINE_GET(KECN_SURVEY_DATA_NOT, obj, ks)
{
	return  GET(m_nVersion) &&
		GET(m_tmBegin) &&
		GET(m_tmEnd) &&
		GET(m_tmReturnTime) &&
		GET(m_mapCompressedSurveyInfoPack) &&
		GET(m_mapSurveyTypeInfo) &&
		GET(m_mapSurveyRewardInfo);
};

SERIALIZE_DEFINE_PUT(KECN_GWC_EVENT_DATA_NOT, obj, ks)
{
	return  PUT(m_dwCountryID) &&
		PUT(m_wstrCountryName) &&
		PUT(m_tmBegin) &&
		PUT(m_tmEnd) &&
		PUT(m_tmDBUpdateTime) &&
		PUT(m_mapGradeRewardInfo) &&
		PUT(m_mapRankingRewardInfo) &&
		PUT(m_mapTotalCountryInfo) &&
		PUT(m_mapGradeDesc);
};

SERIALIZE_DEFINE_GET(KECN_GWC_EVENT_DATA_NOT, obj, ks)
{
	return  GET(m_dwCountryID) &&
		GET(m_wstrCountryName) &&
		GET(m_tmBegin) &&
		GET(m_tmEnd) &&
		GET(m_tmDBUpdateTime) &&
		GET(m_mapGradeRewardInfo) &&
		GET(m_mapRankingRewardInfo) &&
		GET(m_mapTotalCountryInfo) &&
		GET(m_mapGradeDesc);
};

SERIALIZE_DEFINE_PUT(KECN_ADVENTURE_DATA_NOT, obj, ks)
{
	return  PUT(m_mapAdventureMonsterInfo) &&
		PUT(m_mapContinentRewardInfo) &&
		PUT(m_kFinalRewardInfo) &&
		PUT(m_mapAdventureEventDate);
};

SERIALIZE_DEFINE_GET(KECN_ADVENTURE_DATA_NOT, obj, ks)
{
	return  GET(m_mapAdventureMonsterInfo) &&
		GET(m_mapContinentRewardInfo) &&
		GET(m_kFinalRewardInfo) &&
		GET(m_mapAdventureEventDate);
};

SERIALIZE_DEFINE_PUT(KECN_OLYMPIC_RANK_DATA_NOT, obj, ks)
{
	return  PUT(m_vecRegDateRank) &&
		PUT(m_vecTotalRank);
};

SERIALIZE_DEFINE_GET(KECN_OLYMPIC_RANK_DATA_NOT, obj, ks)
{
	return  GET(m_vecRegDateRank) &&
		GET(m_vecTotalRank);
};

SERIALIZE_DEFINE_PUT(KECN_OLYMPIC_SYNC_SOLO_DATA_NOT, obj, ks)
{
	return  PUT(m_tmTodayDate) &&
		PUT(m_nGameType) &&
		PUT(m_nTeamType) &&
		PUT(m_nTopPlayerData) &&
		PUT(m_strNickName);
};

SERIALIZE_DEFINE_GET(KECN_OLYMPIC_SYNC_SOLO_DATA_NOT, obj, ks)
{
	return  GET(m_tmTodayDate) &&
		GET(m_nGameType) &&
		GET(m_nTeamType) &&
		GET(m_nTopPlayerData) &&
		GET(m_strNickName);
};

SERIALIZE_DEFINE_PUT(KECN_OLYMPIC_SYNC_TEAM_DATA_REQ, obj, ks)
{
	return  PUT(m_tmTodayDate) &&
		PUT(m_nGameType) &&
		PUT(m_nTeamType) &&
		PUT(m_nDeltaData);
};

SERIALIZE_DEFINE_GET(KECN_OLYMPIC_SYNC_TEAM_DATA_REQ, obj, ks)
{
	return  GET(m_tmTodayDate) &&
		GET(m_nGameType) &&
		GET(m_nTeamType) &&
		GET(m_nDeltaData);
};

SERIALIZE_DEFINE_PUT(KECN_OLYMPIC_SYNC_TEAM_DATA_ACK, obj, ks)
{
	return  PUT(m_tmTodayDate) &&
		PUT(m_nGameType) &&
		PUT(m_nTeamType) &&
		PUT(m_biTotalData);
};

SERIALIZE_DEFINE_GET(KECN_OLYMPIC_SYNC_TEAM_DATA_ACK, obj, ks)
{
	return  GET(m_tmTodayDate) &&
		GET(m_nGameType) &&
		GET(m_nTeamType) &&
		GET(m_biTotalData);
};

SERIALIZE_DEFINE_PUT(KECN_OLYMPIC_EVENT_DATA_NOT, obj, ks)
{
	return  PUT(m_tmRegDate) &&
		PUT(m_nGameType) &&
		PUT(m_nTeamType) &&
		PUT(m_strNickName) &&
		PUT(m_biTotalData) &&
		PUT(m_vecRegDateRank) &&
		PUT(m_vecTotalRank);
};

SERIALIZE_DEFINE_GET(KECN_OLYMPIC_EVENT_DATA_NOT, obj, ks)
{
	return  GET(m_tmRegDate) &&
		GET(m_nGameType) &&
		GET(m_nTeamType) &&
		GET(m_strNickName) &&
		GET(m_biTotalData) &&
		GET(m_vecRegDateRank) &&
		GET(m_vecTotalRank);
};

SERIALIZE_DEFINE_PUT(KECN_BUFF_DONATION_TOTAL_DATA_NOT, obj, ks)
{
	return  PUT(m_biTotalDonationPoint) &&
		PUT(m_nBuffStateFlag);
};

SERIALIZE_DEFINE_GET(KECN_BUFF_DONATION_TOTAL_DATA_NOT, obj, ks)
{
	return  GET(m_biTotalDonationPoint) &&
		GET(m_nBuffStateFlag);
};

SERIALIZE_DEFINE_PUT(KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT, obj, ks)
{
	return  PUT(m_nVersion) &&
		PUT(m_tmBegin) &&
		PUT(m_tmEnd);
};

SERIALIZE_DEFINE_GET(KECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT, obj, ks)
{
	return  GET(m_nVersion) &&
		GET(m_tmBegin) &&
		GET(m_tmEnd);
};

SERIALIZE_DEFINE_PUT(KECN_ADD_USER_REQ, obj, ks)
{
	return  PUT(m_dwUID) &&
		PUT(m_strName) &&
		PUT(m_strServerIP) &&
		PUT(m_strPassword);
};

SERIALIZE_DEFINE_GET(KECN_ADD_USER_REQ, obj, ks)
{
	return  GET(m_dwUID) &&
		GET(m_strName) &&
		GET(m_strServerIP) &&
		GET(m_strPassword);
};

SERIALIZE_DEFINE_PUT(KECN_CYOU_USER_LOGIN_ACK, obj, ks)
{
	return  PUT(m_dwUID) &&
		PUT(m_nOK);
};

SERIALIZE_DEFINE_GET(KECN_CYOU_USER_LOGIN_ACK, obj, ks)
{
	return  GET(m_dwUID) &&
		GET(m_nOK);
};

SERIALIZE_DEFINE_PUT(KECN_CYOU_USER_HEART_BEAT_ACK, obj, ks)
{
	return  PUT(m_dwUID) &&
		PUT(m_nOK);
};

SERIALIZE_DEFINE_GET(KECN_CYOU_USER_HEART_BEAT_ACK, obj, ks)
{
	return  GET(m_dwUID) &&
		GET(m_nOK);
};

SERIALIZE_DEFINE_PUT(KECN_CONNECTION_GIFTBOX_INFO_NOT, obj, ks)
{
	return  PUT(m_bUseGiftFirstCharacter) &&
		PUT(m_mapGiftFirstCharacter) &&
		PUT(m_bUseGiftNewUser) &&
		PUT(m_tmDecideBeginTimeNewUser) &&
		PUT(m_nNewUserEffectDuration) &&
		PUT(m_mapGiftNewUser) &&
		PUT(m_nGiftEventUserVersion) &&
		PUT(m_tmGiftEventBegin) &&
		PUT(m_tmGiftEventEnd) &&
		PUT(m_mapGiftEventUser);
};

SERIALIZE_DEFINE_GET(KECN_CONNECTION_GIFTBOX_INFO_NOT, obj, ks)
{
	return  GET(m_bUseGiftFirstCharacter) &&
		GET(m_mapGiftFirstCharacter) &&
		GET(m_bUseGiftNewUser) &&
		GET(m_tmDecideBeginTimeNewUser) &&
		GET(m_nNewUserEffectDuration) &&
		GET(m_mapGiftNewUser) &&
		GET(m_nGiftEventUserVersion) &&
		GET(m_tmGiftEventBegin) &&
		GET(m_tmGiftEventEnd) &&
		GET(m_mapGiftEventUser);
};

SERIALIZE_DEFINE_PUT(KECN_EVENT_DB_UPDATE_NOT, obj, ks)
{
	return  PUT(m_nEventType);
};

SERIALIZE_DEFINE_GET(KECN_EVENT_DB_UPDATE_NOT, obj, ks)
{
	return  GET(m_nEventType);
};

SERIALIZE_DEFINE_PUT(KECN_TONG_DONATION_INFO_NOT, obj, ks)
{
	return  PUT(m_tmVersion) &&
		PUT(m_tmBegin) &&
		PUT(m_tmEnd) &&
		PUT(m_dwClientUIType) &&
		PUT(m_mapNPCs);
};

SERIALIZE_DEFINE_GET(KECN_TONG_DONATION_INFO_NOT, obj, ks)
{
	return  GET(m_tmVersion) &&
		GET(m_tmBegin) &&
		GET(m_tmEnd) &&
		GET(m_dwClientUIType) &&
		GET(m_mapNPCs);
};

SERIALIZE_DEFINE_PUT(KItemProvideUserData, obj, ks)
{
	return  PUT(m_vecRewardItem) &&
		PUT(m_dwUserUID) &&
		PUT(m_nGetType);
};

SERIALIZE_DEFINE_GET(KItemProvideUserData, obj, ks)
{
	return  GET(m_vecRewardItem) &&
		GET(m_dwUserUID) &&
		GET(m_nGetType);
};

SERIALIZE_DEFINE_PUT(KRewardUserListDataForSocialCommerce, obj, ks)
{
	return  PUT(kProvideData) &&
		PUT(m_dwRewardUID) &&
		PUT(m_dwEventUID);
};

SERIALIZE_DEFINE_GET(KRewardUserListDataForSocialCommerce, obj, ks)
{
	return  GET(kProvideData) &&
		GET(m_dwRewardUID) &&
		GET(m_dwEventUID);
};

SERIALIZE_DEFINE_PUT(KDB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ, obj, ks)
{
	return  PUT(m_dwEventUID) &&
		PUT(m_nGetType);
};

SERIALIZE_DEFINE_GET(KDB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ, obj, ks)
{
	return  GET(m_dwEventUID) &&
		GET(m_nGetType);
};

SERIALIZE_DEFINE_PUT(KDB_ECN_ITEM_PROVIDE_REQ, obj, ks)
{
	return  PUT(m_vecProvideInfo) &&
		PUT(m_nOneTimeProvisionUserCount);
};

SERIALIZE_DEFINE_GET(KDB_ECN_ITEM_PROVIDE_REQ, obj, ks)
{
	return  GET(m_vecProvideInfo) &&
		GET(m_nOneTimeProvisionUserCount);
};

SERIALIZE_DEFINE_PUT(KECN_DUNGEON_RANK_RANKING_CONFIG_NOT, obj, ks)
{
	return  PUT(m_nRankEntryPerPage) &&
		PUT(m_setRankingModes);
};

SERIALIZE_DEFINE_GET(KECN_DUNGEON_RANK_RANKING_CONFIG_NOT, obj, ks)
{
	return  GET(m_nRankEntryPerPage) &&
		GET(m_setRankingModes);
};