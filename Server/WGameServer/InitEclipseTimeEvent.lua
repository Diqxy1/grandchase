-- 개기 일식 이벤트 - 개기 일식의 전조

EclipseTimeInfo =
{
    Begin = { 2011, 12, 19, 00, 00 },   -- YYYY, MM, DD, HH, mm
    End = { 2012, 02, 01, 05, 00 },     -- YYYY, MM, DD, HH, mm
    Period = 20,                        -- 일식 발동 주기(분; 2 * 60 -> 두시간)
    Duration = 10,                      -- 일식 진행 시간(효과 적용되는 시간) (분; 1 * 60 -> 한시간)
    MonsterID = { 531 },                -- 이벤트 몬스터 등록, 여럿 가능(EventMonster.stg에 설정, 기간 정보를 유효하지 않게 설정 함)
}
