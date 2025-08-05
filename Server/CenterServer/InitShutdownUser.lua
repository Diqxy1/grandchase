TRUE = 1
FALSE = 0

-- 셧다운제 기능 사용유무
Enable = FALSE
CenterLoginCheck = FALSE
GameLoginCheck = FALSE
GameTickCheck = FALSE

-- 대상 연령대 (0세~15세)
Age = { 0, 15 }
-- 적용 시간대 (00:00 ~ 06:00)
StartTime = { 00, 00 }
EndTime = { 06, 00 }
-- 유저종료 체크주기(ms)
ShutdownTick = 5 * 60 * 1000

-- 안내메세지 시간 (시작시간기준 60분전, 30분전, 정각)
AlarmMsgTime = { 60, 30, 0, }

-- 테스트 IP만 적용할 것인지 (본섭 환경에서 테스트용)
TestIPOnly = TRUE
TestIPRange = {
    { "116.120.238.0", "116.120.238.255", "KOG" },
    { "221.148.85.234", "221.148.85.234", "cj" },
}
