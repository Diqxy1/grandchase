Server = GetTRServer()
Server:SetServerProtocol( 327 )
Server:SetSID(100) -- ���߿� DB�� ����

SimLayer = GetSimLayer()
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- ����� db ó�� ���ð� : 1��.
SimLayer.m_dwSendTimeLimit = 10000           -- �� ���� ������ �ð� �ֱ� 

-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9700

DBLayer = GetDBLayer()
DBLayer:Init_ODBC( 'odbc.dsn', 'odbc_stat.dsn' )
DBLayer:Init( 2 )