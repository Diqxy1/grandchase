Server = GetAgentServer()
Server:SetServerProtocol( 327 )
Server:SetSID(1) -- 나중에 DB로 빼자

SimLayer = GetSimLayer()
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- 종료시 db 처리 대기시간 : 1분.
SimLayer.m_dwSendTimeLimit = 10000           -- 내 정보 보내는 시간 주기 

SimLayer:SetUseAgentMaster(true) -- 에이전트 서버 중앙통제기능 사용여부
SimLayer:SetAgentSlave(true) -- 중앙통제용 에이전트에 접속하는 하위 에이전트인가
SimLayer:SetAgentMaster(true) -- 중앙통제용 에이전트인가.

-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9590 -- 임시
NetLayer.m_usPortSlave = 9591 -- 임시

NetLayer:AddTRAddress( '14.45.79.22', 9600 ) -- Tcp
NetLayer:AddURAddress( '14.45.79.22', 9700 ) -- Udp

DBLayer = GetDBLayer()
DBLayer:Init_ODBC( 'odbc.dsn', 'odbc_stat.dsn' )
DBLayer:Init( 10 )

AgentSlave = GetAGConnector()
AgentSlave:SetRemoteAddress( '192.168.61.17', 9571 )

--log( 2 ) -- loglevel : 0 ~ 2

-- Test Func call
--GetAgitManager():TestFunc_CreateAgit(55)
