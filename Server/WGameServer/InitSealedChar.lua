--[[
-- 캐릭터 지급은 DB요청으로 처리합니다
-- DB요청시 봉인된 상태로 지급하도록 합니다
-- 이벤트 종료 시점은 정기 점검시간으로 맞춥니다
-- 이벤트 종료 이후 장착정보 해제를 DB요청으로 처리합니다
-- (봉인 상태인 캐릭터의 장착정보 해제 요청: 장비, 코디, 무기체인지 정보, 펫 장착정보..는 일단 보류)
개발자: 구동현, 정기섭, 이상호, 곽재영
--]]


-- EGCCharType
GC_CHAR_ELESIS           = 0
GC_CHAR_LIRE             = 1
GC_CHAR_ARME             = 2
GC_CHAR_LAS              = 3
GC_CHAR_RYAN             = 4
GC_CHAR_RONAN            = 5
GC_CHAR_AMY              = 6
GC_CHAR_JIN              = 7
GC_CHAR_SIEG             = 8
GC_CHAR_MARI             = 9
GC_CHAR_DIO              = 10
GC_CHAR_ZERO             = 11
GC_CHAR_LEY              = 12
GC_CHAR_LUPUS            = 13
GC_CHAR_RIN              = 14
GC_CHAR_ASIN             = 15
GC_CHAR_LIME             = 16

SealedCharInfo =
{
    --[[
    예제)
    플레이어가 이벤트 기간에 60레벨 이상 라임으로 접속하거나 레벨업하면
    라임 캐릭터 봉인이 해제됨
    {
        CharType = GC_CHAR_LIME,
        ObjectLevel = 60,
        BeginTime = { 2012, 06, 20, 00 },
        EndTime = { 2012, 07, 19, 07 },

    },
    --]]

    {
        CharType = GC_CHAR_LIME,
        ObjectLevel = 60,
        BeginTime = { 2012, 06, 20, 00 },
        EndTime = { 2012, 07, 19, 07 },
    },
}
