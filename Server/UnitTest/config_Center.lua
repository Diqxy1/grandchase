SimLayer = GetSimLayer()
SimLayer:SetNUserWaitTime( 10000 )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NEXON )
SimLayer:AddAuthTypeFlag( KSimLayer.AT_GASH )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_SSO )
SimLayer:AddAuthTypeFlag( KSimLayer.AT_LEVELUP )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NMJ )

SimLayer:AddTypeFlag( KCenterSimLayer.ST_CHECK_DUPL_LOGIN )

-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
NetLayer = GetCenterNetLayer()
NetLayer.m_usPortMaster = 9500
NetLayer.m_usPortSlave  = 9501

Init_CenterDB( 'odbc_internal.dsn' )
Init_MainDB( 'odbc_internal.dsn' ) -- ���� ������ ���� DB���� ��� �´�.

DBHandler = GetDBHandler()
DBHandler:Init( 2 )

Server = GetCenterServer()
Server:SetRepetitionInfo( 3, 1 ) -- ��õ� 3ȸ, ������ 1�� �̻�.
Server:SetServerListProtocol( 241 )


if( SimLayer:CheckAuthTypeFlag(KSimLayer.AT_GASH) ) then

	Init_GashInfo( '210.208.93.41', 5411, 5412, 5413, 300036, 'TA' )
	AuthMgr = GetGashManager()
	AuthMgr:Init( 1 )
	
elseif( SimLayer:CheckAuthTypeFlag( KSimLayer.AT_SSO ) ) then

	Init_SSOAgent( '210.74.250.158', 80, 'grandchase.kog.segame.com', '11111' )
	AuthMgr = GetSSOHandler()
	AuthMgr:Init_SSODaemon( 1010 )
	AuthMgr:Init( 1 )
end
