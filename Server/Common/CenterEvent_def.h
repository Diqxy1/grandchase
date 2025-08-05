#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM(EVENT_HEART_BIT = 0)
	_ENUM(EVENT_ACCEPT_CONNECTION_NOT = 1)

	//ECN = Event Center
	_ENUM(ECN_VERIFY_ACCOUNT_REQ)
	_ENUM(ECN_VERIFY_ACCOUNT_ACK)
	_ENUM(ECN_ADD_USER_REQ)
	_ENUM(ECN_ADD_USER_ACK)
	_ENUM(ECN_ADD_MULTI_USER_REQ)
	_ENUM(NoUse_ECN_SERVER_MIGRATION_REQ)
	_ENUM(NoUse_ECN_SERVER_MIGRATION_ACK)
	_ENUM(ECN_DEL_USER_REQ)
	_ENUM(NoUse_ECN_DEL_USER_ACK)

	_ENUM(ECN_KICK_USER_NOT)
	_ENUM(NoUse_ECN_KICK_USER_ACK)

	_ENUM(DB_EVENT_SERVER_LIST_REQ)  // 베이스 서버가 DB t에게 서버 리스트를 문의
	_ENUM(DB_ENU_VERIFY_ACCOUNT_REQ)

	_ENUM(ECN_LOUD_MSG_REQ)
	_ENUM(ECN_LOUD_MSG_ACK)

	_ENUM(DB_EVENT_FIRST_INIT_REQ) // 서버 기동후 초기 작업

	// 070317. kkurrung. 한시간 이벤트 관련.
	_ENUM(ECN_REPETITION_PROVIDE_REQ)         // 게임 서버로 이벤트 요청
	_ENUM(ECN_REPETITION_PROVIDE_CANCEL_REQ)  // 게임 서버에서 수행 할수 없는 상태라서 센터로 다시 보냄.

	_ENUM(DB_CONNECTION_COUNT_RECORD_REQ)
	_ENUM(DB_EVENT_MINIGAME_RANK_REQ)
	_ENUM(ECN_MINIGAME_RANK_REQ)   // 게임 서버에서 미니게임 TOP 100 순위 정보 요청
	_ENUM(ECN_MINIGAME_RANK_ACK)

	_ENUM(ECN_UPDATE_DONATION_REQ)
	_ENUM(ECN_UPDATE_DONATION_ACK)
	_ENUM(ECN_GET_TOTAL_DONATION_POINT_REQ)

	_ENUM(DB_UPDATE_SCRIPT_CHECKSUM_REQ)
	_ENUM(ECN_UPDATE_CONNECTION_COUNT_REQ)

	_ENUM(DB_UPDATE_CHANNEL_NEWS_REQ)

	_ENUM(ECN_DB_UPDATE_DISCONNECT_TYPE)

	_ENUM(ECN_SERVER_RELAY_PACKET_REQ)
	_ENUM(ECN_SERVER_RELAY_PACKET_NOT)

	_ENUM(ECN_DB_RAINBOW_UPDATE_REQ)
	_ENUM(ECN_RAINBOW_UPDATE_NOT)

	_ENUM(ECN_DB_UPDATE_CHECK_REQ) // DB업이 일어났는지 체크 함.

	_ENUM(ECN_DB_MISSION_EVENT_REQ)
	_ENUM(ECN_MISSION_EVENT_UPDATE_NOT)
	_ENUM(ECN_MISSION_NOTIFY_NOT) // Center -> Game 공지 알림.

	_ENUM(ECN_DB_NPC_GIFTS_UPDATE_REQ)
	_ENUM(ECN_NPC_GIFT_UPDATE_NOT)

	_ENUM(ECN_DB_SUBSCRIPTION_INFO_UPDATE_REQ)            // Center->DB.   DB에서 응모이벤트 정보 갱신받기
	_ENUM(ECN_SUBSCRIPTION_INFO_UPDATE_NOT)               // Center->Game. 바뀐 응모이벤트 정보 각 게임서버에 전달
	_ENUM(ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_REQ)   // Game->Center. 응모이벤트 경품 현재 수량 갱신 알림
	_ENUM(ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_ACK)   // Center->Game. 응모이벤트 경품 현재 수량 갱신 전달

	_ENUM(ECN_UPDATE_DICE_PLAY_REQ)
	_ENUM(ECN_UPDATE_DICE_PLAY_ACK)
	_ENUM(ECN_DICE_PLAY_TOTAL_RANK_REQ)

	// 송편 기부 추가사항
	_ENUM(ECN_ITEM_DONATION_UPDATE_NOT)
	_ENUM(ECN_DB_ITEM_DONATION_UPDATE) // Center -> Game 수정내용 전달.

	_ENUM(ECN_LOG_OUT_NOT) // 인증시 로그 아웃이 필요 한 경우 이걸로 처리 한다.

	_ENUM(ECN_COUPON_INFO_UPDATE_REQ)
	_ENUM(ECN_COUPON_INFO_NOT)

	// 091120. kkurrung. 퀴즈 빙고
	_ENUM(ECN_DB_QUIZ_BINGO_DATA_NOT)
	_ENUM(ECN_QUIZ_BINGO_DATA_NOT)

	// 091210. kkurrung. GWC Event
	_ENUM(ECN_DB_UPDATE_GWC_DATA)
	_ENUM(ECN_GWC_DATA_NOT)
	_ENUM(ECN_GWC_CONFIG_DATA_NOT)

	_ENUM(ECN_ENTER_SQUARE_REQ) // Game->Center. 광장 입장 요청
	_ENUM(ECN_ENTER_SQUARE_ACK) // Center->Game. 광장 입장 응답
	_ENUM(ECN_ENTER_SQUARE_BROAD) // Center-> Some Game server. 광장 입장을 여러 게임서버로 알림
	_ENUM(ECN_LEAVE_SQUARE_NOT) // Game->Center. 광장 퇴장 요청
	_ENUM(ECN_LEAVE_SQUARE_BROAD) // Center-> Some Game server. 광장 퇴장을 여러 게임서버로 알림
	_ENUM(ECN_NEW_SQUARE_USER_NOT) // Center -> Some Game Server. 새 유저 입장을 알림.
	_ENUM(ECN_UPDATE_USER_SQUARE_POS_NOT) // Gameserver -> Center. 유저의 위치 정보 알림.
	_ENUM(ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT) // Center -> Gameserver. 근처에 있는 유저를 알림.
	_ENUM(ECN_SQUARE_RELAY_PACKET_REQ) // GameServer -> Center. 광장 릴레이 패킷
	_ENUM(ECN_SQUARE_RELAY_PACKET_NOT) // Center -> GameServer(s)

	_ENUM(ECN_ENTER_GUILD_SQUARE_REQ) // Game->Center 길드 광장 입장 요청
	_ENUM(ECN_ENTER_GUILD_SQUARE_ACK) // Center->Game
	_ENUM(ECN_ENTER_MINIGAME_NOT) // Game->Center
	_ENUM(ECN_EXIT_MINIGAME_NOT) // Game->Center

	_ENUM(nouse_ECN_ADD_GUILD_USER_BROAD)
	_ENUM(ECN_OFFLINE_GUILD_USER_BROAD) // Center->Games. 길드유저 오프라인 알림
	_ENUM(ECN_ADD_GUILD_USER_REQ) // Game -> Center 길드에 유저 추가
	_ENUM(ECN_CHAT_REQ) // Game-> Center
	_ENUM(ECN_CHAT_ACK) // Center-> Game
	_ENUM(ECN_CHAT_BROAD) // Center->GameServer(s)

	_ENUM(nouse_ECN_CREATE_ROOM_REQ) // Game->Center 길드 룸 생성 요청
	_ENUM(nouse_ECN_CREATE_ROOM_ACK) // Center->Game 길드 룸 생성결과 전달
	_ENUM(nouse_ECN_CREATE_ROOM_BROAD) // Center->Game(s) 길드 룸 생성결과 각 게임서버에 전달
	_ENUM(nouse_ECN_ROOM_LIST_BROAD) // 미구현
	_ENUM(nouse_ECN_JOIN_ROOM_REQ) // Game->Center
	_ENUM(nouse_ECN_JOIN_ROOM_ACK) // Center->Game
	_ENUM(nouse_ECN_JOIN_ROOM_BROAD) // Center->GameServer(s)
	_ENUM(nouse_ECN_LEAVE_ROOM_REQ) // Game->Center
	_ENUM(nouse_ECN_LEAVE_ROOM_ACK) // Center->Game
	_ENUM(nouse_ECN_LEAVE_ROOM_BROAD) //Center->GameServer(s)
	_ENUM(nouse_ECN_CHANGE_ROOM_INFO_REQ) // Game->Center
	_ENUM(nouse_ECN_CHANGE_ROOM_INFO_ACK) // 미사용
	_ENUM(nouse_ECN_CHANGE_ROOM_INFO_BROAD) // Center->GameServer(s)
	_ENUM(nouse_ECN_CHANGE_ROOMUSER_INFO_REQ)
	_ENUM(nouse_ECN_CHANGE_ROOMUSER_INFO_BROAD)
	_ENUM(nouse_ECN_HOST_MIGRATED_NOT)
	_ENUM(nouse_ECN_START_GAME_REQ)
	_ENUM(nouse_ECN_START_GAME_BROAD)
	_ENUM(nouse_ECN_END_GAME_REQ)
	_ENUM(nouse_ECN_END_GAME_ACK)
	_ENUM(nouse_ECN_END_GAME_BROAD)
	_ENUM(nouse_ECN_END_RESULT_NOT)
	_ENUM(nouse_ECN_END_RESULT_BROAD)
	_ENUM(nouse_ECN_LEAVE_GAME_REQ)
	_ENUM(nouse_ECN_LEAVE_GAME_ACK)

	_ENUM(nouse_ECN_GUILD_UPDATE_NOTICE_REQ)
	_ENUM(nouse_ECN_DB_GUILD_GET_NOTICE_REQ)
	_ENUM(nouse_DB_ECN_GUILD_GET_NOTICE_ACK)
	_ENUM(ECN_GUILD_PAGE_REQ) // Game->Center. 길드 페이지정보 요청
	_ENUM(ECN_GUILD_RANK_NOT)

	_ENUM(nouse_ECN_PREMIUM_NOT)

	_ENUM(DB_ECN_UPDATE_GUILD_LIST_REQ) // CENTER->DB. 전체 길드정보 DB에서 받아오기(센터서버 기동시)
	_ENUM(DB_ECN_UPDATE_GUILD_INFO_CHECK_REQ) // CENTER->DB. 길드정보 업데이트 체크(Tick)
	_ENUM(ECN_GUILD_LIST_NOT) // Center->Game. 길드 리스트 전달(게임서버가 센터서버 접속성공시)
	_ENUM(nouse_ECN_INVITE_GUILD_REQ) // Game->Center.
	_ENUM(nouse_ECN_INVITE_GUILD_ACK) // Center->Game.
	_ENUM(ECN_INVITE_GUILD_NOT) // Center->Game. 길드 초대 알림
	_ENUM(ECN_ADD_GUILD_JOINER_NOT) // Game->Center. 길드가입 신청자 목록에 추가 KGuildUserInfo
	_ENUM(ECN_DEL_GUILD_JOINER_NOT) // Game->Center. 길드가입 신청자 목록에서 제거 PAIR_INT_DWORD
	_ENUM(ECN_ADD_NEW_GUILD_NOT) // Game->Center.Center->Game(s) 새로 생성된 길드, 길드목록에 추가 KGuildInfo

	_ENUM(ECN_UPDATE_GUILD_MEMBER_NICK_NOT) // Game->Center.Center->Game(s) 길드 맴버 닉네임 변경 알림.
	_ENUM(ECN_UPDATE_GUILD_NOTICE_NOT) // GAME->CENTER. 길드 공지 갱신
	_ENUM(ECN_UPDATE_GUILD_NAME_NOT) // GAME->CENTER. 길드명 변경 알림 PAIR_INT_WSTRING
	_ENUM(ECN_UPDATE_GUILD_JOIN_SETTING_NOT) // GAME->CENTER. 길드 가입설정 변경 알림 PAIR_INT_UCHAR
	_ENUM(ECN_UPDATE_GUILD_MARK_NOT) // GAME->CENTER. 길드 마크 변경 알림 ???????
	_ENUM(ECN_UPDATE_GUILD_URL_NOT) // GAME->CENTER. 길드URL 갱신
	_ENUM(ECN_BREAKUP_GUILD_NOT) // GAME->CENTER. 길드 해산 알림/길드목록에서 제거 int
	_ENUM(ECN_UPDATE_GUILD_STATE_NOT) // CENTER->GAME. 길드등급 갱신(시스템)(신규->정규/신규->삭제) PAIR_INT_UCHAR

	_ENUM(ECN_UPDATE_GUILD_MEMBER_LEVEL_NOT) // GAME->CENTER. 길드원 등급조정 알림(마스터/제너럴/일반/강퇴) std::vector< std::pair<DWORD,UCHAR> >
	_ENUM(ECN_ACCEPT_GUILD_JOINER_NOT) // GAME->CENTER. 길드가입 승인알림 int;SET_DWORD
	_ENUM(ECN_REJECT_GUILD_JOINER_NOT) // GAME->CENTER. 길드가입 거절알림 int;SET_DWORD

	_ENUM(ECN_GUILD_MEMEBER_LIST_NOT) // CENTER->GAME. 각 게임서버로 길드 맴버리스트 전달.
	_ENUM(ECN_GUILD_PAGE_NOT) // CENTER->GAME. 각 게임서버로 길드페이지 전달
	_ENUM(ECN_DRUMOUT_GUILD_USER_NOT) // Game -> Center 길드원 탈퇴/추방 알림.
	_ENUM(ECN_DRUMOUT_GUILD_USER_BROAD_NOT) // Center -> Game(s) 길드원 탈퇴/추방 알림.
	_ENUM(ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT) // Game->Center. 길드원 자기소개 변경 알림
	_ENUM(ECN_GUILD_MARK_REG_STATUS_REQ) // Game -> Center
	_ENUM(ECN_GUILD_MARK_STATUS_NOT)

	_ENUM(ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT) // Game->Center. 길드원 맴버레벨 변경 알림
	_ENUM(ECN_SQUARE_LIST_REQ) // GAME->CENTER. 광장 리스트 요청
	_ENUM(ECN_SQUARE_LIST_ACK) // CENTER->GAME.
	_ENUM(ECN_GUILD_MARK_INFO_UPDATE_REQ)
	_ENUM(ECN_GUILD_MARK_INFO_UPDATE_ACK)

	// 길드가입
	_ENUM(ECN_JOIN_GUILD_REQ) // GAME->CENTER. 길드 가입 신청
	_ENUM(ECN_JOIN_GUILD_ACK) // CENTER->GAME. 신청 응답
	_ENUM(ECN_ADD_GUILD_USER_BROAD_NOT) // Center->Games. 길드유저 추가 알림.
	_ENUM(ECN_CANCEL_JOIN_GUILD_REQ) // GAME->CENTER. 길드 가입 취소 요청
	_ENUM(ECN_CANCEL_JOIN_GUILD_ACK) // CENTER->GAME. 요청 응답
	_ENUM(ECN_REMOVE_GUILD_USER_BROAD_NOT) // Center->Games. 길드유저 제거 알림.
	// 길드원관리
	_ENUM(ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ) // GAME->CENTER. 길드원 등급조정(마스터/제너럴/일반/강퇴)
	_ENUM(ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK) // CNETER->GAME. 등급조정 결과
	_ENUM(ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT) // Center->Games. 길드원 맴버레벨 변경 알림.
	_ENUM(ECN_SELF_DRUMOUT_GUILD_REQ) // GAME->CENTER. 길드 탈퇴 요청
	_ENUM(ECN_SELF_DRUMOUT_GUILD_ACK) // CENTER->GAME. 요청결과
	// 가입자관리
	_ENUM(ECN_ACCEPT_GUILD_JOINER_REQ) // GAME->CENTER. 길드가입 승인
	_ENUM(ECN_ACCEPT_GUILD_JOINER_ACK) // CENTER->GAME. 승인 결과
	_ENUM(ECN_REJECT_GUILD_JOINER_REQ) // GAME->CENTER. 길드가입 거절
	_ENUM(ECN_REJECT_GUILD_JOINER_ACK) // S->C. 거절 결과
	// 길드해산
	_ENUM(ECN_BREAKUP_GUILD_REQ) // GAME->CENTER. 길드 해산
	_ENUM(ECN_BREAKUP_GUILD_ACK) // CENTER->GAME. 해산 결과
	_ENUM(ECN_REMOVE_GUILD_BROAD_NOT) // Center->Games. 길드 삭제 알림
	// 길드 승인방법 변경
	_ENUM(ECN_CHANGE_GUILD_JOIN_SETTING_NOT)
	_ENUM(ECN_CHANGE_GUILD_JOIN_SETTING_REQ)
	_ENUM(ECN_CHANGE_GUILD_JOIN_SETTING_ACK)
	_ENUM(ECN_AUTO_CHANGE_GUILD_JOIN_SETTING_NOT) // Center->DB. 맴버수full로 가입중지로 바뀜.
	// 길드설정/기타
	_ENUM(ECN_GUILD_CONFIG_NOT)
	_ENUM(ECN_UPGRADE_GUILD_GRADE_REQ) // Center->DB. 신규길드 정식길드로 변환
	_ENUM(ECN_UPGRADE_GUILD_GRADE_ACK) // DB->Center. 변환결과
	_ENUM(ECN_UPGRADE_GUILD_GRADE_BROAD_NOT) // Center->Games. 신규길드 정식길드로 변환 알림.
	// 길드관리
	_ENUM(ECN_REMOVE_GUILD_NOT) // Center->DB. 길드삭제(신규길드 생성후 30일까지,길드원수 5명 미만이면 삭제)
	_ENUM(ECN_NEWBIE_GUILD_DESTROY_CHECK_NOT) // CnGuildManager->CnGuild. 신생길드 삭제조건 체크
	_ENUM(ECN_UPDATE_GUILD_MEMBER_LIST_REQ) // Center->DB. DB에 길드맴버리스트 요청
	_ENUM(ECN_UPDATE_GUILD_MEMBER_LIST_ACK)
	_ENUM(ECN_AUTO_REJECT_GUILD_JOINER_REQ) // CENTER->DB. 가입신청 기간체크하여 자동 가입거절처리
	_ENUM(ECN_UPGRADE_GUILD_GRADE_GIFT_REQ) // CENTER->DB. 정식길드 승격시 선물(아이템)넣기 요청.
	_ENUM(ECN_UPGRADE_GUILD_GRADE_GIFT_ACK) // DB->Center. 아이템넣기 결과.
	_ENUM(ECN_GUILD_MEMEBER_LIST_SYNC_NOT) // Game->Center. 게임서버에 설정되어있는 길드 맴버리스트를 센터서버로 전달(싱크용)

	_ENUM(nouse_ECN_EQUIP_ITEM_BROAD) // EVENT_EQUIP_ITEM_BROAD 를  길드전 방에 접속한 모든 서버의 유저들에게 전달

	_ENUM(nouse_ECN_SYNC_INDOORUSERINFO_BROAD) //

	_ENUM(ECN_INVITE_GUILD_USER_REQ) // Game -> Center
	_ENUM(ECN_INVITE_GUILD_USER_ACK) // Center -> Game
	_ENUM(ECN_SET_GUILD_RANK_NOT) // CnGuildManager->CnGuild. 랭킹데이터 전달

	_ENUM(ECN_UPDATE_GUILD_POINT_REQ) // Game -> Center . 길드 포인트 변화량 적용 요청.
	_ENUM(ECN_UPDATE_GUILD_POINT_BROAD) // Center -> Game . 길드 포인트 변화 결과 전달
	_ENUM(ECN_GUILD_MEMBER_LEVELUP_NOT) // Game->Center/Center->Game(s). 길드맴버 레벨업 알림

	_ENUM(ECN_GUILD_INFO_LIST_NOT) // C->S. 길드 정보 전달(나눠서 전달)
	_ENUM(ECN_GUILD_NOTICE_LIST_NOT) // C->S. 모든 길드 공지 전달
	_ENUM(ECN_GUILD_MEMBER_LIST_NOT) // C->S. 온라인 길드 유저리스트 전달

	_ENUM(ECN_SQUARE_USER_SYNC_REQ) // Game->Center. 광장 위치 동기화 요청
	_ENUM(ECN_SQUARE_USER_SYNC_ACK) // Center->Game. 광장 위치 동기화 응답

	_ENUM(nouse_ECN_GUILD_PAGE_REQ)
	_ENUM(ECN_NEWBIE_GUILD_DESTROY_NOT) // Center->DB. DB에 신생길드 삭제처리 전달

	// 091230. tgkwon 새해 소원 성취 이벤트
	_ENUM(ECN_UPDATE_NEW_DONATION_REQ)
	_ENUM(ECN_UPDATE_NEW_DONATION_ACK)
	_ENUM(ECN_GET_TOTAL_NEW_DONATION_REQ)

	_ENUM(ECN_SOCKET_ITEM_GP_TABLE_NOT)

	_ENUM(ECN_GUIDE_BOOK_LIST_REQ) // nuser -> cnDB
	_ENUM(ECN_GUIDE_BOOK_LIST_ACK) // cnDB -> nuser

	_ENUM(ECN_CLIENT_PING_CONFIG_NOT)

	_ENUM(ECN_ADD_GUILD_USER_ACK) // CenterDB -> Center.
	_ENUM(ECN_ONEADAY_SELECT_REQ) // CENTER->DB. 원어데이 아이템 리스트 요청
	_ENUM(ECN_ONEADAY_UPDATE_REQ) // Game->Center. 원어데이 아이템 리스트 요청
	_ENUM(ECN_ONEADAY_UPDATE_ACK)
	_ENUM(DB_ECN_ONEADAY_UPDATE_REQ) // Center->DB 원어데이 아이템 한정판매 수량 깎인 것
	_ENUM(ECN_USER_SERVER_ROOM_REQ) // Game->Center. 유저의 서버/룸 정보 문의
	_ENUM(ECN_USER_SERVER_ROOM_ACK) // Center->Game. 응답.
	_ENUM(ECN_UPDATE_UNIQUE_USER_STAT_REQ) // S->DB

	_ENUM(ECN_DB_GUILD_POINT_UPDATE_REQ) // Center->DB. 길드포인트, 배틀포인트 동기화 요청
	_ENUM(ECN_DB_GUILD_POINT_UPDATE_ACK) // DB->Center. 동기화 응답

	_ENUM(ECN_UPDATE_BUY_LIMIT_ITEM_LIST_NOT) // Center->Game. 구매제한 아이템 리스트 전달

	_ENUM(ECN_DB_SPHINX_DATA_NOT)  // Center->DB 갱신 데이터 체크.
	_ENUM(ECN_SPHINX_DATA_NOT)     // Center->Game 스핑크스 데이터 전달.

	_ENUM(ECN_CREATE_PARTY_REQ) // Game1->Center. 파티 생성 요청
	_ENUM(ECN_CREATE_PARTY_ACK) // Center->Game1
	_ENUM(ECN_CREATE_PARTY_ASK) // Center->Game2->Client 최초 파티 참가 요청
	_ENUM(ECN_CREATE_PARTY_ANSWER) // C->Game2->Center. 파티 참가 수락/거부
	_ENUM(ECN_INVITE_PARTY_REQ)
	_ENUM(ECN_INVITE_PARTY_ASK)
	_ENUM(ECN_INVITE_PARTY_ANSWER)
	_ENUM(ECN_INVITE_PARTY_ACK)
	_ENUM(ECN_ADD_PARTY_MEMBER_BROAD)
	_ENUM(ECN_CHANGE_PARTY_HOST_REQ)
	_ENUM(ECN_CHANGE_PARTY_HOST_BROAD)
	_ENUM(ECN_LEAVE_PARTY_REQ)
	_ENUM(ECN_LEAVE_PARTY_BROAD)
	_ENUM(ECN_INVITE_PARTY_ROOM_REQ) // 파티방으로 초대 요청
	_ENUM(ECN_BREAK_PARTY_NOT)
	_ENUM(ECN_BAN_PARTY_USER_REQ)
	_ENUM(ECN_BAN_PARTY_USER_NOT)
	_ENUM(ECN_NOTICE_BROAD) // Game->Center. 게임서버가 공지 내용 전달.
	_ENUM(ECN_NOTICE_NOT)   // Center->Game(s). 공지 내용 전체 게임서버로 전달.

	_ENUM(ECN_INVITE_AGIT_REQ) // Game->Center. 아지트 초대
	_ENUM(ECN_INVITE_AGIT_NOT) // Center->Game. 초대 알림

	_ENUM(ECN_SHUTDOWN_USER_CONFIG_NOT) // Center->Game. 셧다운제 설정 전달

	_ENUM(ECN_UPDATE_LOGIN_CCU) // Center->DB. 로긴서버 동접 수 기록
	_ENUM(ECN_USER_EXIT_GAME_NOT) // Game->Center 유저의 로그아웃 정보 전달.

	_ENUM(ECN_DB_SURVEY_DATA_NOT) // Center->DB 설문 갱신 데이터 체크
	_ENUM(ECN_SURVEY_DATA_NOT) // Center->Game 설문 데이터 전달

	_ENUM(ECN_DB_GWC_EVENT_DATA_REQ) // Center->DB GWC 이벤트 데이터 가져오기.
	_ENUM(ECN_GWC_EVENT_DATA_NOT)    // Center->Game GWC 이벤트 데이터 전달.
	_ENUM(ECN_GWC_NOTICE_NOT)   // Center->Game(s). 공지 내용 전체 게임서버로 전달.

	_ENUM(ECN_ITEM_UPDATE_NOT) // Center->Game(s). 아이템 업데이트된 정보 알려주기.

	_ENUM(ECN_FIND_USER_SEND_LETTER_NOT) // Game->Center. 접속중인 상대를 찾아서 실시간 편지 전달
	_ENUM(ECN_RECEIVE_LETTER_NOT) // Center->Game. 접속중인 상대에게 실시간 편지 알림

	_ENUM(ECN_DB_EVENT_TIME_SYNCRONIZE_NOT) // Center <-> DB 시간 동기화

	_ENUM(ECN_DB_ADVENTURE_DATA_NOT) // Center->DB 모험 이벤트 기간 정보 요청
	_ENUM(ECN_ADVENTURE_DATA_NOT) // Center->Game 모험 이벤트 데이터 전달

	_ENUM(ECN_OLYMPIC_RANK_REQ) // DB->Center 올림픽 랭킹 정보 가져오기.
	_ENUM(ECN_OLYMPIC_RANK_DATA_NOT) // Center->Game 올림픽 이벤트 랭킹 데이터 전달.
	_ENUM(ECN_DB_OLYMPIC_DATA_NOT)  // Center->DB 데이터 갱신 체크.
	_ENUM(ECN_OLYMPIC_SYNC_SOLO_DATA_NOT) // Game->Center 올림픽 이벤트 개인전 데이터 전달.
	_ENUM(ECN_OLYMPIC_SYNC_TEAM_DATA_REQ) // Game->Center 올림픽 이벤트 단체전 데이터 전달.
	_ENUM(ECN_OLYMPIC_SYNC_TEAM_DATA_ACK) // Center->Game 올림픽 이벤트 단체전 누적 데이터 전달.
	_ENUM(ECN_DB_OLYMPIC_USER_DATA_UPDATE_NOT)  // Center->DB 통계 데이터 DB갱신.
	_ENUM(ECN_OLYMPIC_DB_USER_DATA_REQ) // Center->DB 누적된 유저 데이터 가져오기.
	_ENUM(ECN_OLYMPIC_EVENT_DATA_NOT) // Center->Game 올림픽 누적치 정보 전달.

	// 대전 매칭
	_ENUM(ECN_PARTY_USER_READY_REQ)
	_ENUM(ECN_PARTY_USER_READY_BROAD)

	// 발렌타인 버프 기부
	_ENUM(ECN_BUFF_DONATION_TOTAL_DATA_REQ) // Center->DB 기부 DB 데이터 얻기
	_ENUM(ECN_BUFF_DONATION_TOTAL_DATA_NOT) // DB->Center->Game 버프 기부 데이터 통지 (기부 수치, 버프 여부)
	_ENUM(ECN_BUFF_DONATION_DATA_UPDATE_REQ) // Game->Center 클라이언트에서 기부 받은 수량 갱신 요청

	// 누적출석
	_ENUM(ECN_ACCUMULATE_ATTENDANCE_DATA_UPDATE_NOT) // Center->DB 누적 출석 DB정보 얻기 요청
	_ENUM(ECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT) // Center->Game 누적 출석 DB정보 게임서버로 전달(버전, 기간)

	_ENUM(ECN_CASHBACK_RATIO_INFO_NOT) // 캐시백 비율 정보 전달 Center->Game

	_ENUM(ECN_CLIENT_CONTENTS_OPEN_INFO_NOT) // center->game 오픈된 맵 관련 정보, 캐릭터 관련 정보, 몬스터 관련 정보, 미니게임 오픈 정보 전달
	_ENUM(ECN_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT) // center->game, 클라 사용파일(광장, 로딩 등), 해킹 방어 관련 정보 전달
	_ENUM(ECN_ITEM_BUY_INFO_NOT) // center->game, 아이템 구매 가능 정보 전달(InitClientContents)
	_ENUM(ECN_EVENT_EVENT_STATE_INFO_NOT) // center->game 게임 서버 연결 시 이벤트 상태 전송(Events)
	_ENUM(ECN_EVENT_CHANGED_EVENT_STATE_INFO_NOT) // center->game->client 이벤트 전체의 변경 된 ON/OFF 상태 전송
	_ENUM(ECN_PLAN_SCRIPT_UPDATE_NOT) // (ScriptFiles)

	_ENUM(ECN_CYOU_USER_HEART_BEAT_REQ) // Game->Center->Billing. 중국 유저의 하트비트
	_ENUM(ECN_CYOU_USER_HEART_BEAT_ACK) // Billing->Center->Game->Client. 중국 유저의 하트비트 응답
	_ENUM(ECN_CYOU_CASH_CHANGE_REQ) // Game->Center . 캐시포인트의 게임내 캐시 전환.
	_ENUM(ECN_CYOU_CHANGE_CASH_ACK) // Center->Game. 캐시포인트 전환 후 결과와 현재 포인트
	_ENUM(ECN_CYOU_USER_AGREEMENT_REQ) // Game->Center. 중국 유저 동의. protocol A7
	_ENUM(ECN_CYOU_GET_CURRENT_CASH_REQ) // Game->Center. 캐시포인트 조회.
	_ENUM(ECN_CYOU_GET_CURRENT_CASH_ACK)
	_ENUM(ECN_CYOU_BUY_LOG_REQ) // Game->Center. 빌링으로 캐시 아이템 구매 기록 전송.
	_ENUM(ECN_CYOU_USER_LOGIN_REQ) // Game->Center->Billing. 유저 로그인(캐릭터 선택/변경할 때 전송)
	_ENUM(ECN_CYOU_USER_LOGIN_ACK) // Center->Game . 유저 로그인 응답.
	_ENUM(ECN_CYOU_USER_LOGOUT) // Game->Center->Billing. 유저 로그아웃(캐릭터 선택/변경할 때 전송) ,변경시에는 이전 캐릭터 로그아웃 후 새 캐릭터 로그인
	_ENUM(ECN_CYOU_CHANGE_CASH_LOG_NOT) // Billing->Center. 창유 포인트 전환 로그 기록

	_ENUM(ECN_CONNECTION_GIFTBOX_INFO_NOT) // Center->Game 접속 선물상자 정보 전송

	_ENUM(ECN_EVENT_PACKAGE_LIST_CHANGED_NOT) // center->game 패키지 정보 변경 알림
	_ENUM(ECN_EVENT_DB_UPDATE_NOT) // db->s EVENT 정보 갱신 전달.

	_ENUM(ECN_TONG_DONATION_INFO_NOT) // center->game 게임 서버 연결시 통합 기부 이벤트 정보 전송

	// 보부상
	_ENUM(ECN_SOCIAL_COMMERCE_DATA_UPDATE_NOT) // Center->DB 소셜커머스 DB정보 얻기
	_ENUM(ECN_SOCIAL_COMMERCE_INIT_DATA_NOT) // Center->Game 게임서버 연결 시 소셜커머스 설정 데이터 NOT
	_ENUM(ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ) // Center->DB 공동구매 신청 유저 정보 얻기
	_ENUM(DB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ) // Center->DB 공동구매 달성 보상 아이템 지급 요청

	// 실시간 아이템 지급
	_ENUM(DB_ECN_ITEM_PROVIDE_REQ) // Center->DB 아이템공급자 객체에서 아이템 지급 REQ

	// 던전 랭킹 시스템
	_ENUM(ECN_DB_DUNGEON_RANK_SEASON_DELIMIT_TIME_REQ) // Center->DB 시즌 기준시간 요청
	_ENUM(ECN_DUNGEON_RANK_RANKING_CONFIG_NOT) // Center->Game 랭킹관련 설정 알림(랭킹을 처리할 모드 리스트, 페이지당 엔트리 수)
	_ENUM(ECN_DB_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_REQ) // Center->DB 현재 시즌 랭크 리스트 요청
	_ENUM(ECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT) // Center->Game 현재 시즌 랭크 리스트 알림
	_ENUM(ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ) // Game->Center, Center->DB 현재 시즌 랭크 추가 요청
	_ENUM(ECN_DB_DUNGEON_RANK_SEASON_RESET_CHECK_REQ) // Center->DB 시즌 갱신 여부 확인 요청
	_ENUM(ECN_DB_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_REQ) // Center->DB 이전 시즌 랭크 리스트 요청
	_ENUM(ECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT) // Center->Game 이전 시즌 랭크 리스트 알림

	_ENUM( ECN_BILLBOARD_DATA_BROAD_REQ )
	_ENUM( ECN_BILLBOARD_DATA_BROAD )

	_ENUM( DB_ENU_ACCOUNT_REGISTER_REQ )

	_ENUM(EVENT_EVENTID_SENTINEL)

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
