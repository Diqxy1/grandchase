#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL, FolderName, Header) VAL,
enum
{
#endif

	_ENUM(LT_ABUSE, abuse, none)
	_ENUM(LT_EXPGP, gp_exp_err, none)
	_ENUM(LT_DURATION, duration_Hack, none)
	_ENUM(LT_CLIENT_ERR, Client_Error, none)
	_ENUM(LT_PASSWORD, Wrong_Password, none)
	_ENUM(LT_SEND_BUFF_FULL, Send_Buff_Full, none)
	//_ENUM( LT_CC_CHANNEL,           CC_Channel , none )
	_ENUM(LT_CLIENT_ERR_COMMENT, Error_Comment, none)
	//_ENUM( LT_EVENT_ERR,            Event_Error , none )
	_ENUM(LT_GASH_HK, Gash_Problem, none)
	_ENUM(LT_GASH_ERR_STAT, Gash_Err_Stat, none)
	_ENUM(LT_P2P, P2P_Stat, none)
	_ENUM(LT_INDIGO, Indigo, none)
	_ENUM(LT_ADMIN_NOTICE, Admin_Notice, none)
	//_ENUM( LT_PAYLETTER_ERR,        PayLetter_Billing_err , none )
	_ENUM(LT_BILLING_LOG, billing_log, none)
	//_ENUM( LT_WEEKLY_REWARD_ERR,    Weekly_Attendance_Reward_Err , none )
	//_ENUM( LT_MONTHLY_REWARD_ERR,   Monthly_Attendance_Reward_Err , none )
	//_ENUM( LT_RELAY_DATA,           Relay_Data , none )
	_ENUM(LT_ADD_EXP_USER, Add_Exp_User, none)
	_ENUM(LT_DEATH_KILL_HACK, Death_Kill_Hack, none)
	_ENUM(LT_KICK_USER, Kick_User, none)
	_ENUM(LT_WIM_WIN, win_win_hack, none)
	_ENUM(LT_DUNGEON_HACK, Dungeon_Hack, none)
	//_ENUM( LT_GEM_LOST,             Gem_Lost , none )
	//_ENUM( LT_OVERNIGHT_USER,       Overnight , none )
	_ENUM(LT_DB_QUEUE_SIZE, DBLayer_queue_size, none)
	_ENUM(LT_TRAFFIC_ATTACK, Traffic_Attack, none)
	//_ENUM( LT_SQUARE_COUNT,         Square_Count , none )
	//_ENUM( LT_GUILD_COUNT,          Guild_Count , none )
	_ENUM(LT_PACKET_HACK, Packet_Hacking, none)
	_ENUM(LT_RECV_PACKET, Recv_Packet, none)
	_ENUM(LT_LOW_PING, Low_Ping_User, none)
	_ENUM(LT_LONG_PROCESS, Long_Process_Event, none)
	_ENUM(LT_29_ERR_COMMENT, 29_Error_Comment, none)
	_ENUM(LT_EXP_STAT_ERR, Exp_Stat_Error, ThreadID | Time | UID | LoginID | CharType | DB Exp | Local Exp | Local Lv)
	_ENUM(LT_EXP_CALC_ERR, Exp_Calc_Error, Time | Case | ModeID | LoginID | CharType | DB Exp | TotalRecvExp | Local Exp | DB Lv | Local Lv)
	_ENUM(LT_TICK_MANAGER, TickManager, none)
	_ENUM(LT_TICK_MANAGER_QUEUE, TickManager_Queue, none)
	_ENUM(LT_PACKET_COUNT, PacketCount, Time | Count)
	_ENUM(LT_GP_INC, GpInc, ModeID | CharType | Lv | Promotion | Gp | Count)
	_ENUM(LT_GP_DEC, GpDec, ModeID | CharType | Lv | Promotion | Gp | Count)
	_ENUM(LT_USER_EVENT_PROCESS_TIME, User_Event_Process_Time, none)
	_ENUM(LT_PVP_HACK, PvP_Hack, none)
	_ENUM(LT_ST_MEMBER_NOT_ING_P2P, MemberNot_P2P, none)
	_ENUM(LT_USER_START_FAIL_STATE, StartFail_State, none)
	//_ENUM( LT_NEW_DELETE_USER,      NewDeleteCount, none  )
	_ENUM(LT_USER_INVENTORY_ITEM_LIST, UserInventoryItemList, none)

	_ENUM(LT_MAX, LT_MAX, none)

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif