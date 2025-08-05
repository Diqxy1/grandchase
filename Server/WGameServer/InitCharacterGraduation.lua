--[[
캐릭터 전용 던전 설정을 위한 스크립트입니다
CharType: 설정할 캐릭터 번호를 설정합니다
Conditions: 해당 캐릭터가 졸업하기 위해 클리어해야 할 던전 및 난이도를 설정합니다
Rewards: 졸업시 지급할 아이템들을 설정합니다
ClearModes: 졸업시 자동으로 클리어 처리(난이도 0만) 해줄 던전을 설정합니다
개발자: 구동현, 이세진, 곽재영
--]]

CHAR_ELESIS     = 0
CHAR_LIRE       = 1
CHAR_ARME       = 2
CHAR_LAS        = 3
CHAR_RYAN       = 4
CHAR_RONAN      = 5
CHAR_AMY        = 6
CHAR_JIN        = 7
CHAR_SIEG       = 8
CHAR_MARI       = 9
CHAR_DIO        = 10
CHAR_ZERO       = 11
CHAR_LEY        = 12
CHAR_LUPUS      = 13
CHAR_RIN        = 14
CHAR_ASIN       = 15
CHAR_LIME       = 16
-------------------------- 테스트 파일임.. 혼동하지 말자!!!!!!!!!!!!!!
Graduation =
{
    {
        CharType = CHAR_RIN,
        Condition =
        {
            { ModeID = GC_GM_QUEST42, Difficulty = { 0, 1, 2, }, },
            --[[
            { ModeID = GC_GM_QUEST44, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST45, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST48, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST55, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST56, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST62, Difficulty = { 0, 1, 2, }, },
            --]]
        },
        ClearModes =
        {
            GC_GM_QUEST0, GC_GM_QUEST1, GC_GM_QUEST2, GC_GM_QUEST3, GC_GM_QUEST4, GC_GM_QUEST5,
            GC_GM_QUEST6, GC_GM_QUEST7, GC_GM_QUEST8, GC_GM_QUEST9, --GC_GM_QUEST10, GC_GM_QUEST11,
        },
        Rewards =
        {-- { itemid, grade, period, count, }, ...
            { 833050, 3, -1, -1, },
        },
    },
}
