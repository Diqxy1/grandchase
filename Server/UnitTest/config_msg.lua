Server = GetMsgServer()

SimLayer = GetSimLayer()
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- ����� db ó�� ���ð� : 1��.
                    
-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9300

Init_MSDB( 'odbc_internal.dsn' )

DBHandler = GetDBLayer()
DBHandler:Init( 3 )

log( 2 ) -- loglevel : 0 ~ 2
