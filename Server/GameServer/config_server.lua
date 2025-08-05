fr = GetFailRate()
fr:SetWriteToDB( false )

SimLayer = GetSimLayer()
SimLayer.m_uiNickNameAvailable  = 12
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- 종료시 db 처리 대기시간 : 1분.
SimLayer.m_iMaxGamePoint        = 999999
SimLayer.m_bRecommendEnable     = true
SimLayer.m_bWriteClientInfo     = true
SimLayer.m_bEnableIndigoLog     = true;
-- 상하이 드랍 적용 가입일.
SimLayer:SetSHAdhustTime( 2007, 04, 27, 09, 30 )
SimLayer:SetChatEventOption( 60000, 6, 4 )

SimLayer.m_nMaxReportNum        = 3 -- 신고 최대 회수 제한
SimLayer.m_bTWUserAgreement     = false -- 대만 유저 정형화
SimLayer.m_bWelcomeFriend       = false -- 반갑다친구야2 이벤트
SimLayer.m_bPacketCheckInEnable = true -- 중복 패킷 처리 여부
SimLayer.m_bEnableLimitKeyBuy   = true

SimLayer:SetTableChecksum( 0 )
SimLayer:SetChatEventOption( 60000, 2, 2 ) -- 채팅이벤트 입력제한시간, PVP일때의 최대 인원, Dungeon모드일때의 최대인원.
SimLayer:SetPacketHackCheck(true) -- inner패킷 해킹 체크여부
SimLayer:SetHostMigrationByPing( false ) -- ping 방장 교체
SimLayer:SetExpDBUpdateTick( 60 * 1000 * 5 ) -- Exp DB업데이트 주기
SimLayer:SetServerListAuthLevel( 3 )
SimLayer:SetAgitEnable(true) -- 아지트 기능 사용여부
SimLayer:SetNMCafeEnable(false) -- 넷마블 PC방차감 사용여부
SimLayer:SetNMCafeUpdateTickGap(3*60*1000) -- 넷마블PC방 플레이시간 차감주기
SimLayer:SetLvDesignEnable(false) -- 레벨디자인 서버 설정인지.

SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
SimLayer:AddTypeFlag( KGSSimLayer.ST_DEBUG_AUTH )
-- ST_INDIGO : 용사의섬
-- ST_GUILD : 길드서버
-- ST_BROADCASTING : 방송용
-- ST_ENTER_LIMIT : 입장제한(대회서버)
-- ST_PVP_BALANCE : PVP시 능력치 균등화(대회서버)

SimLayer:AddTypeFlag( KGSSimLayer.ST_INTERNAL_TESOP + KGSSimLayer.ST_DP_LOG )
-- + KGSSimLayer.ST_GUILD ) -- 
--SimLayer:AddTypeFlag( KGSSimLayer.ST_INDIGO + KGSSimLayer.ST_BROADCASTING ) -- 용섬
--SimLayer:AddTypeFlag( KGSSimLayer.ST_ENTER_LIMIT + KGSSimLayer.ST_PVP_BALANCE + KGSSimLayer.ST_BROADCASTING ) -- 대회서버
--SimLayer:AddTypeFlag( KGSSimLayer.ST_NPGG_KILL )


-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9400
NetLayer.m_usUdpPort    = 9401
NetLayer:AddURServerAddress( '14.45.79.22', 9600 )
NetLayer:AddTRServerAddress( '14.45.79.22', 9700 )
NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketLimitNum( 100 )


CenterProxy = GetCenterProxy()
CenterProxy:SetRemoteAddress( '14.45.79.22', 9500 )

AgentProxy = GetAgentConnector()
AgentProxy:SetRemoteAddress( 0, '14.45.79.22', 9570 )
AgentProxy:SetRemoteAddress( 1, '14.45.79.22', 9580 )
AgentProxy:SetRemoteAddress( 2, '14.45.79.22', 9590 )
AgentProxy:SetAgentMasterSID( 0 )

--Init_EvtDB( 'odbc_event.dsn' ) -- 2006. 넷마블 6주년 기념 이벤트
-- Init_GashInfo( '210.208.94.134', 5411, 5412, 5413, 300036, 'TA' )

DBLayer   = GetDBLayer()
DBLayer:Init_ODBC( 'odbc_internal.dsn', 'odbc_stat.dsn' )
DBLayer:Init( 1 )

DBLayer:AddODBC( '__odbcNMPCBang', 'odbc_internal_pcbang.dsn' )
DBLayer:AddODBC( '__odbcGCPCBang', 'odbc_internal.dsn' )
DBLayer:AddODBC( '__odbcMsgServer', 'odbc_msg.dsn' )


Worker = GetWorkerManager()
Worker:Init(1)

NMNetcafe = GetNetCafeManager()
NMNetcafe:Init(1)

log( 2 ) -- loglevel : 0 ~ 2
Server = GetGameServer()
Server:ClearChecksum()

Server:AddChecksum( 0 )             -- 051108. 98에서는 클라이언트가 체크섬을 제대로 구하지 못한다!

if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_XTRAP_ON ) ) then
	xtrap = GetXTrap()
	xtrap:SetConfig( 60000, 600, "xtrap.CS3", false )
end

if( SimLayer:CheckAuthTypeFlag( KSimLayer.AT_GASH ) ) then -- 대만 인증 설정된 경우
    GashMgr = GetGashManager()
    GashMgr:Init( 1 )
    
end

if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_INDIGO ) ) then

    tm = GetOpenTime()
    tm:SetEnable( true )
    
end

-- 중요 매니저
MissionManager  = GetMissionManager()
GameDrop        = GetGameDrop()
RecipeManager   = GetRecipeManager()
RankManager     = GetRankManager()
GCHelper        = GetGCHelper()
SignBoard       = GetSignBoard()

-- 랭킹 설정
--RankManager:SetUpdateTime(8,0) -- 08:00 시간에 업데이트.

-- 중요 매니저
MissionManager = GetMissionManager()
GameDrop = GetGameDrop()
RecipeManager = GetRecipeManager()
GCHelper = GetGCHelper()
RankManager = GetRankManager()
ResultMng = GetResultManager();
SignBoard       = GetSignBoard()
rainbow = GetRainbow()

function LoadAllScript()
	MissionManager:LoadScript()
	GameDrop:LoadScript()
	RecipeManager:LoadScript()
	GCHelper:LoadScript()
end

function msg( m )
    SignBoard:LuaMsg( m )
    print( m )
end

function msg_clear()
    SignBoard:ClearLuaMsg()
    print( "[System] Clear All Msg... OK" )
end

-- 대만 야후 이벤트...



-- 통계
StatManager = GetStatisticsManager()
StatManager:AddStatistics( KStatisticsManager.SI_CPL, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_PLAY_TIME, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_MAP_COUNT, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_LOGINOUT_STAT, 30 * 60 * 1000, false, 10 ) -- tslayer
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHECK_IN, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_P2P_CRITICAL, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_MINIGAME, 3 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_LEAVE_GAME, 3 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAMELOADING, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DEATH, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_SPX, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DISCONNECT, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_COUPLE_ROOM, 30 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAME_START_GAP, 30 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DUNGEON, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_LOADINGINFO, 60 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GP, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHARACTER, 60 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHAR_EQUIP, 1 * 60 * 1000, true, 10 )

-- 닉네임 금지 필터
SlangFilter = GetSlangFilter()
SlangFilter:AddLanguage( 0, 127 ) -- Latin basic (Alphabet and numbers)
--SlangFilter:AddLanguage( 48, 57 ) -- Latin basic (Alphabet and numbers)
--SlangFilter:AddLanguage( 93, 93 )

SlangFilter:SetEnableUnicode( false ) -- 위에 등록된 유니코드를 허용할 때 true

cm = GetCollectionManager() -- 컬렉션

---- FTP ---
FTP = GetFTPFactory()
FTP:AddConnectionInfo( 'GuildMark', '116.120.238.38', 21, 'grandchase', 'gpfrpdlxm310TT^^' )
