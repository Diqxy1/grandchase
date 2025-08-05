SimLayer = GetSimLayer()
SimLayer:SetNUserWaitTime( 10000 )

SimLayer:AddAuthTypeFlag( KSimLayer.AT_CYOU ) -- â�� ������ ����Ѵ�.
SimLayer:AddTypeFlag( KCenterSimLayer.ST_INTERNAL_TESOP ) -- �׽�Ʈ ���� �ɼ��� �Ҵ�. ���ȭ ���� ������ �� �ɼ��� �����Ѵ�.
SimLayer:AddTypeFlag( KCenterSimLayer.ST_KOG_AUTH_TEST ) -- 
SimLayer:AddTypeFlag( KCenterSimLayer.ST_CHECK_DUPL_LOGIN ) -- ���� ������ ���� �ɼ�.
SimLayer:SetBillingType( KSimLayer.BT_CN_CYOU )
SimLayer.m_bConnectCountEnable = true -- ������ �α伭���� ������ ī��Ʈ�� DB �� ����ϴ� �ɼ�.

SimLayer:SetPingConfig(40000, 40000, 40000, 1, false, -1 )
--SimLayer:SetFakeCCU( -1 )
--SimLayer:SetCheckOverlapped( true )
--SimLayer:SetUseBlockWrongPassword( true ) -- �н����� nȸ Ʋ�� �� ����
SimLayer.m_nAccOnlineTimeTest = 300
SimLayer:SetBillConnIP( '192.168.199.104' )

NetLayer = GetCenterNetLayer()
NetLayer.m_usPortMaster = 9500
NetLayer.m_usPortSlave  = 9501

NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketLimitNum( 10 )

Init_CenterDB( 'odbc_internal.dsn' )
Init_MainDB( 'odbc_internal.dsn' )


DBLayer = GetDBHandler()
DBLayer:Init( 20 ) -- number of thread for game DB
DBLayer:AddODBC( 'stat', 'odbc_stat.dsn' )

CYOUAuth = GetCYOUAuth()
InitCYOUBillingInfo( '123.126.70.69', 15780, 'odbc_cash.dsn', 6, 8, 0.2 * 1000 )
--InitCYOUBillingInfo( '119.167.137.17', 18049, 'odbc_cash.dsn', 6, 8, 0.2 * 1000 )
-- InitCYOUBillingInfo( 'billing server IP', billing server port, 'dsn file name for logging in game DB', Zone ID, Group ID )
CYOUAuth:Init(1) -- number of thread for billing server.



Server = GetCenterServer()
Server:SetRepetitionInfo( 3, 100 ) 
Server:SetServerListProtocol( 328 )
Server:SetLoginStayLimitTime( 180 * 60 * 1000 )
Server:SetConnectStayLimitTime( 180 * 60 * 1000 )


Checksum = Server:GetChecksumManager()
Checksum:SetCheckEnable( false )


BlockIP = GetIPBlockList()

StatManager = GetCnStatisticsManager()
StatManager:AddStatistics( KCnStatisticsManager.SI_LOGINOUT_STAT, 60 * 1000, false, 10 )
