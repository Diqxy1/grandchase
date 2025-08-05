fr = GetFailRate()
fr:SetWriteToDB( false ) -- 동일 서버군에서 1번 게임서버는 이 항목을 true 로 한다. DB에 남기는 통계 스위치이다.

SimLayer = GetSimLayer()
SimLayer.m_bBlockMaxUser = true
SimLayer.m_uiNickNameAvailable  = 12
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- 종료시 db 처리 대기시간 : 1분.
SimLayer.m_iMaxGamePoint        = 10000000

SimLayer.m_bWriteClientInfo     = true -- 클라이언트 사양 조사
SimLayer.m_bLoadingCheck	= false  -- 난입 로딩 체크
SimLayer.m_dwGameStartWaitTime = 180000
SimLayer.m_bEnableLimitKeyBuy  = false
SimLayer:SetHBGap( 180000 )
-- 상하이 드랍 적용 가입일.
--SimLayer:SetSHAdhustTime( 2007, 04, 27, 09, 30 )

SimLayer.m_nMaxReportNum        = 3 -- 신고 최대 회수 제한
SimLayer.m_bP2PJudgement        = false
SimLayer.m_bMiniGameScoreCheck  = true -- 미니게임 해킹의심 점수 체크 여부
SimLayer.m_bCheckJoinRoomPing   = false
SimLayer:SetServerListAuthLevel( 3 ) -- 안보이는 서버 입장가능 유저등급
SimLayer:SetHostMigrationByPing( false ) -- ping 방장 교체.
SimLayer:SetExpDBUpdateTick( 60 * 1000 * 10 ) -- Exp DB업데이트 주기
SimLayer:SetServerRoomInfoTick( 10 * 1000 ) -- 서버,방 정보요청 주기
SimLayer:SetAgitEnable(true) -- 아지트 기능 활성화
SimLayer:SetNMCafeUpdateTickGap(3*60*1000) -- 넷마블PC방 플레이시간 차감주기
SimLayer.m_nLimitAcceptQueueCount  = 3000
--SimLayer:SetLvDesignEnable(true) -- 레벨디자인 서버 설정인지.
--SimLayer.m_bTimeSync = false -- DB와 시간 동기화할지 여부

--SimLayer:SetP2PStat( true, 60, 180, 10 ) -- ( bool bEnable_,int nMinPing_, int nMaxPing_, int nMaxLossCount_ )

SimLayer:SetTableChecksum( 0 )
--SimLayer:SetChatEventOption( 30000, 4, 4 ) -- 채팅이벤트 입력제한시간, PVP일때의 최대 인원, Dungeon모드일때의 최대인원.
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )


SimLayer:AddAuthTypeFlag( KSimLayer.AT_CYOU )
-- ST_INDIGO : 용사의섬
-- ST_GUILD : 길드서버
-- ST_BROADCASTING : 방송용
-- ST_TOURNAMENT : 점령전(대회)서버
-- ST_ENTER_LIMIT : 입장제한 서버 (DB등록된 유저만 입장 가능) (대회용)
-- ST_OBSERVER_ROOM : 옵저버가 방으로 입장가능하게됨
-- ST_DEBUG_AUTH : 인증처리 패스

--{{ Internal Setting
SimLayer:AddTypeFlag( KGSSimLayer.ST_DEBUG_AUTH )
SimLayer:AddTypeFlag( KGSSimLayer.ST_INTERNAL_TESOP + KGSSimLayer.ST_DP_LOG )
SimLayer:AddTypeFlag( KGSSimLayer.ST_OBSERVER_ROOM ) 
--SimLayer:AddTypeFlag( KGSSimLayer.ST_PVP_BALANCE ) 
--SimLayer:AddTypeFlag( KGSSimLayer.ST_DUNGEON )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_XTRAP_KILL + KGSSimLayer.ST_XTRAP_ON ) -- XTrap 활성화
--}} Internal Setting

-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9400
NetLayer.m_usUdpPort    = 9401
NetLayer:AddURServerAddress( '192.168.199.104', 9600 )
NetLayer:AddTRServerAddress( '192.168.199.104', 9700 )
NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketLimitNum( 100 )
NetLayer:SetUserPacketAttackInfo( true, 200, 100, 1000 )

--{{ Internal Setting
-- UDP Relay
NetLayer:AddURServerAddress( '14.45.79.22', 9600 )
-- TCP Relay
NetLayer:AddTRServerAddress('14.45.79.22', 9700 )

-- Center Server
CenterProxy = GetCenterProxy()
--CenterProxy:SetRemoteAddress( '192.168.199.104', 9500 )

AgentProxy = GetAgentProxy()
AgentProxy:SetRemoteAddress( 0, '14.45.79.22', 9800 )
AgentProxy:SetRemoteAddress( 1, '14.45.79.22', 9810 )
AgentProxy:SetAgentMasterSID( 0 )

DBLayer   = GetDBLayer()
DBLayer:Init_ODBC( 'odbc_internal.dsn', 'odbc_stat.dsn' )
DBLayer:Init( 15 )

DBLayer:AddODBC( '__odbcNMPCBang', 'odbc_internal_pcbang.dsn' )
DBLayer:AddODBC( '__odbcGCPCBang', 'odbc_internal.dsn' )
DBLayer:AddODBC( '__odbcMsgServer', 'odbc_msg.dsn' )

---- FTP ---
FTP = GetFTPFactory()
FTP:AddConnectionInfo( 'GuildMark', '192.168.61.7', 21, 'guildmark', 'guild123!@#' )

SimLayer:SetNMCafeEnable(false) -- 넷마블 PC방차감 사용여부

--log( 2 ) -- loglevel : 0 ~ 2
--}} Internal Setting


Worker = GetWorkerManager()
Worker:Init(1)

NMNetcafe = GetNetCafeManager()
NMNetcafe:Init(1)

CYOUBilling = GetCYOUBillingManager()
InitCYOUBillingInfo( '123.126.70.69', 15780, 'odbc_internal.dsn', 'odbc_cash.dsn', 6, 8 )
CYOUBilling:Init(1)

ShutdownSystem = GetPlayAuthManager()
ShutdownSystem:SetUsePlayAuthSystem( false ) -- 가급적이면 라이브 중 변경하지 말 것
--ShutdownSystem:SetAutoReconnectGap( 60000 ) -- 지정하지 않으면 기본값이 1분


Server = GetGameServer()
Server:ClearChecksum()

if( SimLayer:CheckAuthTypeFlag( KSimLayer.AT_GASH ) ) then -- 대만 인증 설정된 경우
    GashMgr = GetGashManager()
    GashMgr:Init( 1 )
end


if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_XTRAP_ON ) ) then
	xtrap = GetXTrap()
	xtrap:SetConfig( 10000, 300, "xtrap.CS3", false )
end

-- 중요 매니저
MissionManager = GetMissionManager()
GameDrop = GetGameDrop()

GCHelper = GetGCHelper()
RankManager = GetRankManager()
ResultMng = GetResultManager()
SignBoard = GetSignBoard()
rainbow = GetRainbow()

rainbow:AddItem(559250,2,-1) -- 레인보우 이벤트 NPC 아이템 지급-1
rainbow:AddItem(177560,2,-1) -- 레인보우 이벤트 NPC 아이템 지급-2
rainbow:AddItem(71770,1,-1) -- 레인보우 이벤트 NPC 아이템 지급-3


function LoadAllScript()
	MissionManager:LoadScript()
	GameDrop:LoadScript()

	GCHelper:LoadScript()
	ResultMng:LoadScript()
end

function msg( m )
    SignBoard:LuaMsg( m )
    print( m )
end

function msg_clear()
    SignBoard:ClearLuaMsg()
    print( "[System] Clear All Msg... OK" )
end

function msg_update()
    SignBoard:LuaMsg( "안녕하세요,그랜드체이스 운영팀입니다. 잠시후 업데이트를 위한 서버다운이 있을 예정입니다. 게임 이용에 참고해주세요. 감사합니다." )
    print( "[System] ServerDown UpdateMsg Send.. OK" )
end

-- 통계
StatManager = GetStatisticsManager()
StatManager:AddStatistics( KStatisticsManager.SI_CPL, 60 * 60 * 1000, true, 70 )
StatManager:AddStatistics( KStatisticsManager.SI_PLAY_TIME, 60 * 60 * 1000, true, 80 )
StatManager:AddStatistics( KStatisticsManager.SI_MAP_COUNT, 60 * 60 * 1000, true, 80 )
StatManager:AddStatistics( KStatisticsManager.SI_LOGINOUT_STAT, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_EMOTICON, 24 * 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_ITEM_GET, 60 *60 * 1000, false, 70 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAMELOADING, 60 * 60 * 1000, false, 70 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DEATH, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHECK_IN, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_P2P_CRITICAL, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_MINIGAME, 10 * 60 * 1000, true, 70 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_SPX, 60 * 60 * 1000, false, 70 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DISCONNECT, 62 * 60 * 1000, false, 70 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_LEAVE_GAME, 63 * 60 * 1000, true, 70 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAME_START_GAP, 60* 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DUNGEON, 60 * 1000 * 5, true, 10 )
--StatManager:AddStatistics( KStatisticsManager.SI_STAT_ROOMJOIN, 30 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GP, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHARACTER, 60 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHAR_EQUIP, 1 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_NETWORK, 60 * 60 * 1000, true, 1 )

-- 닉네임 금지 필터
SlangFilter = GetSlangFilter()
SlangFilter:AddLanguage( 65, 90 ) -- 영문(대문자) 0041 - 005A
SlangFilter:AddLanguage( 97, 122 ) -- 영문(소문자) 0061 - 007A
SlangFilter:AddLanguage( 48, 57 ) -- 숫자 0030 - 0039
SlangFilter:AddLanguage( 44032, 55203 ) -- 한글 AC00 - D7A3
SlangFilter:SetEnableUnicode( true ) -- 위에 등록된 유니코드를 허용할 때 true



---- FTP ---
FTP = GetFTPFactory()
FTP:AddConnectionInfo( 'GuildMark', '14.45.79.22', 21, 'grandchase', 'gpfrpdlxm310TT^^' )



FatigueSystem = GetFatigueSystem()
PenaltyTable ={ 
	{ 0, 0, 180, 100, 60 }, --type, Start, End, Ratio	, notice gap
	{ 1, 180, 300, 50, 30 }, --type, Start, End, Ratio	, notice gap
	{ 2, 300, -1, 0, 15 }, --type, Start, End, Ratio , notice gap
}
-- Enable, ClearAccOfflineTime, ThresholdTime, UnlimitedTime, LimitedTime, PenaltyTable
FatigueSystem:SetRewardLimit( false, 5, 1, 180, 300, PenaltyTable )
