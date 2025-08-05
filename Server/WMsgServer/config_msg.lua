Server = GetMsgServer()

SimLayer = GetSimLayer()
SimLayer.m_dwShutdownTimeLimit  = 60 * 1000  -- ����� db ó�� ���ð� : 1��.
SimLayer.m_dwSendTimeLimit = 10000           -- �� ���� ������ �ð� �ֱ� 

-- �޽��� ����(���� ��å �ٲ� ������ ���ϵ���)
SimLayer.m_usMaxFriendLimit = 150 -- �ִ� ģ����
SimLayer.m_usMaxBlockLimit  = 150 -- �ִ� ����
SimLayer.m_usMaxGroupLimit  = 10  -- �ִ� �׷��
SimLayer.m_usMaxGroupName   = 12  -- �׷�� ����
SimLayer.m_usMaxChatRoom    = 10  -- �ִ� ä�ù� ������
SimLayer.m_usMaxChatMember  = 20  -- �ִ� ä�ù� �ɹ���

Server.m_bWriteLogoutLog 	= true -- �ݸ������� ���.

-- NetLayer�� �Ӽ��� �ʱ�ȭ�� �ѹ��� ��ȿ�ϴ�. ���� �ֿܼ��� ������ �� ����.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9300

Init_MSDB( 'odbc.dsn' )                 -- msg DB
Init_MSMainDB( 'odbc_internal.dsn' )    -- Main DB

DBHandler = GetDBLayer()
DBHandler:Init( 3 )

log( 2 ) -- loglevel : 0 ~ 2
