fr = GetFailRate()
fr:SetWriteToDB( false )

SimLayer = GetSimLayer()
SimLayer.m_uiNickNameAvailable  = 12
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- ����� db ó�� ���ð� : 1��.
SimLayer.m_iMaxGamePoint        = 999999
SimLayer.m_bRecommendEnable     = true
SimLayer.m_bWriteClientInfo     = true
SimLayer.m_bEnableIndigoLog     = true;
-- ������ ��� ���� ������.
SimLayer:SetSHAdhustTime( 2007, 04, 27, 09, 30 )
SimLayer:SetChatEventOption( 60000, 6, 4 )

SimLayer.m_nMaxReportNum        = 3 -- �Ű� �ִ� ȸ�� ����
SimLayer.m_bTWUserAgreement     = false -- �븸 ���� ����ȭ
SimLayer.m_bWelcomeFriend       = false -- �ݰ���ģ����2 �̺�Ʈ
SimLayer.m_bPacketCheckInEnable = true -- �ߺ� ��Ŷ ó�� ����
SimLayer.m_bEnableLimitKeyBuy   = true

SimLayer:SetTableChecksum( 0 )
SimLayer:SetChatEventOption( 60000, 2, 2 ) -- ä���̺�Ʈ �Է����ѽð�, PVP�϶��� �ִ� �ο�, Dungeon����϶��� �ִ��ο�.
SimLayer:SetPacketHackCheck(true) -- inner��Ŷ ��ŷ üũ����
SimLayer:SetHostMigrationByPing( false ) -- ping ���� ��ü
SimLayer:SetExpDBUpdateTick( 60 * 1000 * 5 ) -- Exp DB������Ʈ �ֱ�
SimLayer:SetServerListAuthLevel( 3 )
SimLayer:SetAgitEnable(true) -- ����Ʈ ��� ��뿩��
SimLayer:SetNMCafeEnable(false) -- �ݸ��� PC������ ��뿩��
SimLayer:SetNMCafeUpdateTickGap(3*60*1000) -- �ݸ���PC�� �÷��̽ð� �����ֱ�
SimLayer:SetLvDesignEnable(false) -- ���������� ���� ��������.

SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
SimLayer:AddTypeFlag( KGSSimLayer.ST_DEBUG_AUTH )
-- ST_INDIGO : ����Ǽ�
-- ST_GUILD : ��弭��
-- ST_BROADCASTING : ��ۿ�
-- ST_ENTER_LIMIT : ��������(��ȸ����)
-- ST_PVP_BALANCE : PVP�� �ɷ�ġ �յ�ȭ(��ȸ����)

SimLayer:AddTypeFlag( KGSSimLayer.ST_INTERNAL_TESOP + KGSSimLayer.ST_DP_LOG )
-- + KGSSimLayer.ST_GUILD ) -- 
--SimLayer:AddTypeFlag( KGSSimLayer.ST_INDIGO + KGSSimLayer.ST_BROADCASTING ) -- �뼶
--SimLayer:AddTypeFlag( KGSSimLayer.ST_ENTER_LIMIT + KGSSimLayer.ST_PVP_BALANCE + KGSSimLayer.ST_BROADCASTING ) -- ��ȸ����
--SimLayer:AddTypeFlag( KGSSimLayer.ST_NPGG_KILL )


-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
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

--Init_EvtDB( 'odbc_event.dsn' ) -- 2006. �ݸ��� 6�ֳ� ��� �̺�Ʈ
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

Server:AddChecksum( 0 )             -- 051108. 98������ Ŭ���̾�Ʈ�� üũ���� ����� ������ ���Ѵ�!

if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_XTRAP_ON ) ) then
	xtrap = GetXTrap()
	xtrap:SetConfig( 60000, 600, "xtrap.CS3", false )
end

if( SimLayer:CheckAuthTypeFlag( KSimLayer.AT_GASH ) ) then -- �븸 ���� ������ ���
    GashMgr = GetGashManager()
    GashMgr:Init( 1 )
    
end

if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_INDIGO ) ) then

    tm = GetOpenTime()
    tm:SetEnable( true )
    
end

-- �߿� �Ŵ���
MissionManager  = GetMissionManager()
GameDrop        = GetGameDrop()
RecipeManager   = GetRecipeManager()
RankManager     = GetRankManager()
GCHelper        = GetGCHelper()
SignBoard       = GetSignBoard()

-- ��ŷ ����
--RankManager:SetUpdateTime(8,0) -- 08:00 �ð��� ������Ʈ.

-- �߿� �Ŵ���
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

-- �븸 ���� �̺�Ʈ...



-- ���
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

-- �г��� ���� ����
SlangFilter = GetSlangFilter()
SlangFilter:AddLanguage( 0, 127 ) -- Latin basic (Alphabet and numbers)
--SlangFilter:AddLanguage( 48, 57 ) -- Latin basic (Alphabet and numbers)
--SlangFilter:AddLanguage( 93, 93 )

SlangFilter:SetEnableUnicode( false ) -- ���� ��ϵ� �����ڵ带 ����� �� true

cm = GetCollectionManager() -- �÷���

---- FTP ---
FTP = GetFTPFactory()
FTP:AddConnectionInfo( 'GuildMark', '116.120.238.38', 21, 'grandchase', 'gpfrpdlxm310TT^^' )
