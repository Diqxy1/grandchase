--[[
기부 이벤트 새로 작성하였습니다. 앞으로 개발되는 내용은 이 기능에 추가할 예정입니다
버프 기부는 별도로 동작합니다
기부 데이터를 초기화 하려면
방법 1) 해당 VersionTime의 DB의 값을 모두 지우고 모든 게임서버들을 재실행하거나
방법 2) 센터 서버에서 VersionTime의 날짜를 변경하고 스크립트를 다시 로드한 뒤 서버 이동이나 재접속을 하면 됩니다

작업자: 장상준, 이재준, 곽재영
--]]

VersionTime     = { 2013, 08, 20, 00, }   -- 버전으로 사용 할 시간. 시간이 아닌 일단위로 동작합니다(Y, M, D, HH)

BeginTime       = { 2013, 08, 20, 00, }   -- 이벤트 시작 시간(Y, M, D, HH)
EndTime         = { 2013, 08, 27, 00, }   -- 이벤트 종료 시간(Y, M, D, HH)


-- 클라이언트 UI 선택용 타입 설정
DONATION_UITYPE_NO_GRADE    = 0     -- 단계없는 기부 UI
DONATION_UITYPE_5_GRADE     = 1     -- 5 단계 기부 UI

ClientUIType    = DONATION_UITYPE_5_GRADE

NPCInfo =
{
    {
        NPCType         = 1, -- NPC 타입    
        DonationItemID  = 8880, -- 기부 아이템ID. 수량 아이템일 경우에만 동작함
        DonationUnit    = 1,    -- 기부 아이템의 기부 단위(개)
        DonationReward  = { 379820, -1, -1 },  -- 교환 개념 기부 보상((itemid, count, period); 비어있는 경우 지급하지 않음)
        GoalInfo = {
            { Goal = 1000, Str = 5739, },
            { Goal = 2500, Str = 5740, },
            { Goal = 5000, Str = 5741, },
            { Goal = 7000, Str = 5742, },
            { Goal = 10000, Str = 5743, },
        },
    },
    {
        NPCType         = 2, -- NPC 타입    
        DonationItemID  = 8880, -- 기부 아이템ID. 수량 아이템일 경우에만 동작함
        DonationUnit    = 2,    -- 기부 아이템의 기부 단위(개)
        DonationReward  = { 8880, 5, -1 },  -- 교환 개념 기부 보상((itemid, count, period); 비어있는 경우 지급하지 않음)
        GoalInfo = {
            { Goal = 1000, Str = 5739, },
            { Goal = 2000, Str = 5740, },
            { Goal = 3000, Str = 5741, },
            { Goal = 4000, Str = 5742, },
            { Goal = 5000, Str = 5743, },
        },
    },
}
