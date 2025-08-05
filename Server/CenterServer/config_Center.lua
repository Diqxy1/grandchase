
SimLayer = GetSimLayer()
SimLayer:SetNUserWaitTime( 10000 )
SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
SimLayer:AddTypeFlag( KCenterSimLayer.ST_INTERNAL_TESOP )
SimLayer:SetPingConfig(1000, 4000, 1000, 1, true, -1 ) -- 핑 설정 ( 체크 전송, 핑 지연 최대 간격, 핑 한계, 반복 횟수, 마이그레이션/강퇴 여부 )
SimLayer.m_bConnectCountEnable = true
SimLayer.m_bDynamicResourceLoadEnable = true
SimLayer:SetCheckOverlapped( false )
SimLayer.m_nServerListAuthLevel = 1

-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetCenterNetLayer()
NetLayer.m_usPortMaster = 9500
NetLayer.m_usPortSlave  = 9501
NetLayer:SetCheckSequenceNum( true )
NetLayer:SetPacketLimitNum( 100 )

Init_CenterDB( 'odbc_internal.dsn' )
Init_MainDB( 'odbc_internal.dsn' ) -- 서버 정보를 메인 DB에서 얻어 온다.


DBHandler = GetDBHandler()
DBHandler:Init( 2 )
DBHandler:AddODBC( 'stat', 'odbc_stat.dsn' )

Server = GetCenterServer()
Server:SetRepetitionInfo( 3, 1 ) -- 재시도 3회, 서버에 1명 이상.
Server:SetServerListProtocol( 327 )
Server:SetProblemMsg( "str_03", "str_04" )
Server:SetUserNumGap( 1 )
Server:AddPhoneNumber( "str_05" )

Checksum = Server:GetChecksumManager()
Checksum:SetCheckEnable(false)

log( 2 ) -- loglevel : 0 ~ 2
