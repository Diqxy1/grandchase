-- SystemResource Monitor
-- üũ ����
--( cpu, memory, HDD )

--[[
-- Gropu Desc
-- ������ �������� ��ȣ�� �����Ѵ�. �ش� �׷����� �ʿ��� ������ �ټ��� ������ �� ���� ��츦 ����ؼ��̴�.
CenterServer = 0
GameServer = 1
MsgRouter = 2
MsgServer = 3
AgentServer = 4
MatchServer = 5
]]--

-- ���� �׷�
CenterServer = 0
GameServer = 1
MsgRouter = 2
MsgServer = 3
AgentServer = 4
MatchServer = 5

TRUE = 1
FALSE = 0

ServerGroup = GameServer
ServerUniqueNum = 0
UseEnable = TRUE
TickGap = 1000 * 60  -- ms

-- ������ ������ Disk�� ��Ƽ�� ������ ���ش�.
-- ��Ƽ�� ������ �������� �ý����Լ��� ������ �ش� �Լ��� ��Ʈ��ũ ��Ƽ�Ǳ��� �� �������� ������ �ִ�.
-- ���Ӽ��������� ��Ʈ��ũ ��Ƽ���� ���� ������ �ִ� ������ �������� �����Ͱ� ���� ���� �ֱ� ������ ��ũ��Ʈ�� �и��Ѵ�.
-- "C:\\" <-- ������ ���� �������� �Է��Ѵ�.
HDD_DiskDesc = { 'C:\\', 'D:\\' }
