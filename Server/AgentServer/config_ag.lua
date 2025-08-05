Server = GetAgentServer()
Server:SetServerProtocol( 327 )
Server:SetSID(1) -- ���߿� DB�� ����

SimLayer = GetSimLayer()
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- ����� db ó�� ���ð� : 1��.
SimLayer.m_dwSendTimeLimit = 10000           -- �� ���� ������ �ð� �ֱ� 

SimLayer:SetUseAgentMaster(true) -- ������Ʈ ���� �߾�������� ��뿩��
SimLayer:SetAgentSlave(true) -- �߾������� ������Ʈ�� �����ϴ� ���� ������Ʈ�ΰ�
SimLayer:SetAgentMaster(true) -- �߾������� ������Ʈ�ΰ�.

-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9590 -- �ӽ�
NetLayer.m_usPortSlave = 9591 -- �ӽ�

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
