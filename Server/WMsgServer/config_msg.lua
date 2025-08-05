Server = GetMsgServer()

SimLayer = GetSimLayer()
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- 종료시 db 처리 대기시간 : 1분.
SimLayer.m_dwSendTimeLimit = 10000           -- 내 정보 보내는 시간 주기 

-- 메신저 설정(차후 정책 바뀌어도 리빌드 안하도록)
SimLayer.m_usMaxFriendLimit = 150 -- 최대 친구수
SimLayer.m_usMaxBlockLimit  = 150 -- 최대 블럭수
SimLayer.m_usMaxGroupLimit  = 10  -- 최대 그룹수
SimLayer.m_usMaxGroupName   = 12  -- 그룹명 길이
SimLayer.m_usMaxChatRoom    = 10  -- 최대 채팅방 생성수
SimLayer.m_usMaxChatMember  = 20  -- 최대 채팅방 맴버수

Server.m_bWriteLogoutLog 	= true -- 넷마블에서만 사용.

-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9300

Init_MSDB( 'odbc.dsn' )                 -- msg DB
Init_MSMainDB( 'odbc_internal.dsn' )    -- Main DB

DBHandler = GetDBLayer()
DBHandler:Init( 3 )

log( 2 ) -- loglevel : 0 ~ 2
