-- SystemResource Monitor
-- 체크 사항
--( cpu, memory, HDD )

--[[
-- Gropu Desc
-- 각각의 서버별로 번호를 구분한다. 해당 그룹핑이 필요한 이유는 다수의 서버가 떠 있을 경우를 대비해서이다.
CenterServer = 0
GameServer = 1
MsgRouter = 2
MsgServer = 3
AgentServer = 4
MatchServer = 5
]]--

-- 서버 그룹
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

-- 각각의 서버에 Disk의 파티션 정보를 써준다.
-- 파티션 정보를 가져오는 시스템함수가 있지만 해당 함수는 네트워크 파티션까지 다 가져오는 문제가 있다.
-- 게임서버에서는 네트워크 파티션이 보통 없지만 있는 곳에서 쓸데없는 데이터가 쓰여 질수 있기 때문에 스크립트로 분리한다.
-- "C:\\" <-- 다음과 같은 형식으로 입력한다.
HDD_DiskDesc = { 'C:\\', 'D:\\' }
