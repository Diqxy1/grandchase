
SimLayer = GetSimLayer()
SimLayer:SetNUserWaitTime( 10000 )
SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
SimLayer:AddTypeFlag( KCenterSimLayer.ST_INTERNAL_TESOP )
SimLayer:SetPingConfig(1000, 4000, 1000, 1, true, -1 ) -- �� ���� ( üũ ����, �� ���� �ִ� ����, �� �Ѱ�, �ݺ� Ƚ��, ���̱׷��̼�/���� ���� )
SimLayer.m_bConnectCountEnable = true
SimLayer.m_bDynamicResourceLoadEnable = true
SimLayer:SetCheckOverlapped( false )
SimLayer.m_nServerListAuthLevel = 1

-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
NetLayer = GetCenterNetLayer()
NetLayer.m_usPortMaster = 9500
NetLayer.m_usPortSlave  = 9501
NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketLimitNum( 100 )

Init_CenterDB( 'odbc_internal.dsn' )
Init_MainDB( 'odbc_internal.dsn' ) -- ���� ������ ���� DB���� ��� �´�.


DBHandler = GetDBHandler()
DBHandler:Init( 2 )
DBHandler:AddODBC( 'stat', 'odbc_stat.dsn' )

Server = GetCenterServer()
Server:SetRepetitionInfo( 3, 1 ) -- ��õ� 3ȸ, ������ 1�� �̻�.
Server:SetServerListProtocol( 327 )
Server:SetProblemMsg( "str_03", "str_04" )
Server:SetUserNumGap( 1 )
Server:AddPhoneNumber( "str_05" )

Checksum = Server:GetChecksumManager()
Checksum:SetCheckEnable(false)

log( 2 ) -- loglevel : 0 ~ 2
