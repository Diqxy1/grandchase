#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif
	_ENUM(DRW_NONE = 0)                    // default
	_ENUM(DRW_PROTECTED_TABLE_TIMEOUT)     // 프로텍티트 테이블 체크섬 타임아웃//프로텍티드 타입에서 테이블 체크섬
	_ENUM(DRW_LOGIN_TIME_OUT)              // 로그인시 타임아웃
	_ENUM(DRW_DUPLICATE_CONNECTION)        // 이중접속 종료
	_ENUM(DRW_EMPTY_LOGIN)                 // DB에서 얻어온 LoginID가 비어있음
	_ENUM(DRW_KICK_BY_ADMIN)               // 운영자에게 강제종료
	_ENUM(DRW_BY_CRASH)                    // 클라이언트 크래쉬
	_ENUM(DRW_PROTECTED_TABLE_VERIFY)      // 프로텍티트 테이블 체크섬 인증
	_ENUM(DRW_ZOMBIE)                      // 좀비
	_ENUM(DRW_AUTHEN_FAIL)                 // 인증 실패// 인증 실패
	_ENUM(DRW_PROTOCOL_VERION_DIFF)        // 프로토콜 버전 다름
	_ENUM(DRW_SEND_BUFFER_FULL)            // Send Buffer Full
	_ENUM(DRW_BAD_USER)                    // 불량 유저
	_ENUM(DRW_ADD_NEW_USER_FAIL)           // 신규유저 추가 실패
	_ENUM(DRW_WRONG_PASSWORD)              // 접속 암호 틀림
	_ENUM(DRW_CHECKSUM_FAIL)               // 실행파일 체크섬 에러
	_ENUM(DRW_GASH_AUTH_FAIL)              // 대만 인증 실패
	_ENUM(DRW_NOT_USER)                    // 존재하지 않는 LoginID
	_ENUM(DRW_AUTH_TICK_FAIL)              // 인증 Tick Count가 다르다
	_ENUM(DRW_SERVER_MIGRATION)            // 서버 이동
	_ENUM(DRW_SCRIPT_CHKSUM)               // 로그인서버 스크립트 체크섬 오류
	_ENUM(DRW_TRAFFIC_ATTACK)              // 트래픽 공격
	_ENUM(DRW_NORMAL_EXIT)                 // 정상 종료
	_ENUM(DRW_DUPLICATE_FILE)              // 중복 파일( ENU_OVERLAP_FILE_INFO )
	_ENUM(DRW_SERVER_COMMEND)              // 서버 명령어 종료
	_ENUM(DRW_SERVER_FULL)                 // 서버 최대 유저 초과
	_ENUM(DRW_SERVER_BLOCKIP)              // 블럭 IP 종료.
	_ENUM(DRW_SHUTDOWN_USER)               // 셧다운제 대상유저
	_ENUM(DRW_CLIENT_HACKING)              // 클라이언트 해킹
	_ENUM(DRW_PACKET_ATTACK)               // 유저 패킷 공격
	_ENUM(DRW_DUPLICATE_CONNECTION_CYOU)   // 이중접속 종료
	// Disconnect Reason enum값 추가시, DB에 알려줄 것.

	_ENUM(DRW_MAX)

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
