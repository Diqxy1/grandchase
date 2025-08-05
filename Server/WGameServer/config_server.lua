fr = GetFailRate()
fr:SetWriteToDB( false ) -- ���� ���������� 1�� ���Ӽ����� �� �׸��� true �� �Ѵ�. DB�� ����� ��� ����ġ�̴�.

SimLayer = GetSimLayer()
SimLayer.m_bBlockMaxUser = true
SimLayer.m_uiNickNameAvailable  = 12
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- ����� db ó�� ���ð� : 1��.
SimLayer.m_iMaxGamePoint        = 10000000

SimLayer.m_bWriteClientInfo     = true -- Ŭ���̾�Ʈ ��� ����
SimLayer.m_bLoadingCheck	= false  -- ���� �ε� üũ
SimLayer.m_dwGameStartWaitTime = 180000
SimLayer.m_bEnableLimitKeyBuy  = false
SimLayer:SetHBGap( 180000 )
-- ������ ��� ���� ������.
--SimLayer:SetSHAdhustTime( 2007, 04, 27, 09, 30 )

SimLayer.m_nMaxReportNum        = 3 -- �Ű� �ִ� ȸ�� ����
SimLayer.m_bP2PJudgement        = false
SimLayer.m_bMiniGameScoreCheck  = true -- �̴ϰ��� ��ŷ�ǽ� ���� üũ ����
SimLayer.m_bCheckJoinRoomPing   = false
SimLayer:SetServerListAuthLevel( 3 ) -- �Ⱥ��̴� ���� ���尡�� �������
SimLayer:SetHostMigrationByPing( false ) -- ping ���� ��ü.
SimLayer:SetExpDBUpdateTick( 60 * 1000 * 10 ) -- Exp DB������Ʈ �ֱ�
SimLayer:SetServerRoomInfoTick( 10 * 1000 ) -- ����,�� ������û �ֱ�
SimLayer:SetAgitEnable(true) -- ����Ʈ ��� Ȱ��ȭ
SimLayer:SetNMCafeUpdateTickGap(3*60*1000) -- �ݸ���PC�� �÷��̽ð� �����ֱ�
SimLayer.m_nLimitAcceptQueueCount  = 3000
--SimLayer:SetLvDesignEnable(true) -- ���������� ���� ��������.
--SimLayer.m_bTimeSync = false -- DB�� �ð� ����ȭ���� ����

--SimLayer:SetP2PStat( true, 60, 180, 10 ) -- ( bool bEnable_,int nMinPing_, int nMaxPing_, int nMaxLossCount_ )

SimLayer:SetTableChecksum( 0 )
--SimLayer:SetChatEventOption( 30000, 4, 4 ) -- ä���̺�Ʈ �Է����ѽð�, PVP�϶��� �ִ� �ο�, Dungeon����϶��� �ִ��ο�.
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )


SimLayer:AddAuthTypeFlag( KSimLayer.AT_CYOU )
-- ST_INDIGO : ����Ǽ�
-- ST_GUILD : ��弭��
-- ST_BROADCASTING : ��ۿ�
-- ST_TOURNAMENT : ������(��ȸ)����
-- ST_ENTER_LIMIT : �������� ���� (DB��ϵ� ������ ���� ����) (��ȸ��)
-- ST_OBSERVER_ROOM : �������� ������ ���尡���ϰԵ�
-- ST_DEBUG_AUTH : ����ó�� �н�

--{{ Internal Setting
SimLayer:AddTypeFlag( KGSSimLayer.ST_DEBUG_AUTH )
SimLayer:AddTypeFlag( KGSSimLayer.ST_INTERNAL_TESOP + KGSSimLayer.ST_DP_LOG )
SimLayer:AddTypeFlag( KGSSimLayer.ST_OBSERVER_ROOM ) 
--SimLayer:AddTypeFlag( KGSSimLayer.ST_PVP_BALANCE ) 
--SimLayer:AddTypeFlag( KGSSimLayer.ST_DUNGEON )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_XTRAP_KILL + KGSSimLayer.ST_XTRAP_ON ) -- XTrap Ȱ��ȭ
--}} Internal Setting

-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
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

SimLayer:SetNMCafeEnable(false) -- �ݸ��� PC������ ��뿩��

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
ShutdownSystem:SetUsePlayAuthSystem( false ) -- �������̸� ���̺� �� �������� �� ��
--ShutdownSystem:SetAutoReconnectGap( 60000 ) -- �������� ������ �⺻���� 1��


Server = GetGameServer()
Server:ClearChecksum()

if( SimLayer:CheckAuthTypeFlag( KSimLayer.AT_GASH ) ) then -- �븸 ���� ������ ���
    GashMgr = GetGashManager()
    GashMgr:Init( 1 )
end


if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_XTRAP_ON ) ) then
	xtrap = GetXTrap()
	xtrap:SetConfig( 10000, 300, "xtrap.CS3", false )
end

-- �߿� �Ŵ���
MissionManager = GetMissionManager()
GameDrop = GetGameDrop()

GCHelper = GetGCHelper()
RankManager = GetRankManager()
ResultMng = GetResultManager()
SignBoard = GetSignBoard()
rainbow = GetRainbow()

rainbow:AddItem(559250,2,-1) -- ���κ��� �̺�Ʈ NPC ������ ����-1
rainbow:AddItem(177560,2,-1) -- ���κ��� �̺�Ʈ NPC ������ ����-2
rainbow:AddItem(71770,1,-1) -- ���κ��� �̺�Ʈ NPC ������ ����-3


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
    SignBoard:LuaMsg( "�ȳ��ϼ���,�׷���ü�̽� ����Դϴ�. ����� ������Ʈ�� ���� �����ٿ��� ���� �����Դϴ�. ���� �̿뿡 �������ּ���. �����մϴ�." )
    print( "[System] ServerDown UpdateMsg Send.. OK" )
end

-- ���
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

-- �г��� ���� ����
SlangFilter = GetSlangFilter()
SlangFilter:AddLanguage( 65, 90 ) -- ����(�빮��) 0041 - 005A
SlangFilter:AddLanguage( 97, 122 ) -- ����(�ҹ���) 0061 - 007A
SlangFilter:AddLanguage( 48, 57 ) -- ���� 0030 - 0039
SlangFilter:AddLanguage( 44032, 55203 ) -- �ѱ� AC00 - D7A3
SlangFilter:SetEnableUnicode( true ) -- ���� ��ϵ� �����ڵ带 ����� �� true



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
