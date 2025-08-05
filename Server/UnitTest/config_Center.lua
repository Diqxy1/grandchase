SimLayer = GetSimLayer()
SimLayer:SetNUserWaitTime( 10000 )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NEXON )
SimLayer:AddAuthTypeFlag( KSimLayer.AT_GASH )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_SSO )
SimLayer:AddAuthTypeFlag( KSimLayer.AT_LEVELUP )
--SimLayer:AddAuthTypeFlag( KSimLayer.AT_NMJ )

SimLayer:AddTypeFlag( KCenterSimLayer.ST_CHECK_DUPL_LOGIN )

-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetCenterNetLayer()
NetLayer.m_usPortMaster = 9500
NetLayer.m_usPortSlave  = 9501

Init_CenterDB( 'odbc_internal.dsn' )
Init_MainDB( 'odbc_internal.dsn' ) -- 서버 정보를 메인 DB에서 얻어 온다.

DBHandler = GetDBHandler()
DBHandler:Init( 2 )

Server = GetCenterServer()
Server:SetRepetitionInfo( 3, 1 ) -- 재시도 3회, 서버에 1명 이상.
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
