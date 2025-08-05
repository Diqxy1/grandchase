--[[
이벤트 및 클라이언트 컨텐츠 정보를 기간별로 설정합니다
개발자: 장원형, 손기운, 이환수, 곽재영
--]]

ICCUpdatePlan =
{
    {
        CheckTime   = { 2012, 01, 18, 20, 00 }, -- YYYY, MM, DD, HH, mm
        FileName    = "InitClientContents.lua",
    },
    -------------------------------------------------------------------
    -- edit below this line
    -------------------------------------------------------------------
    --[[
    {
        CheckTime   = { 2013, 05, 20, 00, 00 }, -- YYYY, MM, DD, HH, mm
        FileName    = "InitClientContents_1.lua",
    },
    --]]
    --[[
    {
        CheckTime   = { 2013, 04, 09, 23, 30 }, -- YYYY, MM, DD, HH, mm
        FileName    = "InitClientContents_2.lua",
    },
    --]]
}

-- 아래 enum은 UpdatePlanEventID.h와 맞춰줍니다
EI_DICE_PLAY        = 0
EI_RITAS_CHRISTMAS  = 1

EventUpdatePlan =
{
    --[[ 현재 미사용
    {
        EventID     = EI_DICE_PLAY,
        BeginTime   = { 2013, 01, 01, 00, 00 }, -- YYYY, MM, DD, HH, mm
        EndTime     = { 2013, 01, 02, 00, 00 }, -- YYYY, MM, DD, HH, mm
    },
    {
        EventID     = EI_DICE_PLAY,
        BeginTime   = { 2013, 02, 01, 00, 00 }, -- YYYY, MM, DD, HH, mm
        EndTime     = { 2013, 02, 02, 00, 00 }, -- YYYY, MM, DD, HH, mm
    },
    {
        EventID     = EI_RITAS_CHRISTMAS,
        BeginTime   = { 2013, 02, 04, 00, 00 }, -- YYYY, MM, DD, HH, mm
        EndTime     = { 2013, 02, 05, 00, 00 }, -- YYYY, MM, DD, HH, mm
    },
    --]]
}

-- 아래 enum은 UpdatePlanScriptID.h와 맞춰줍니다
SCI_INITDROP        = 0
SCI_GAMEDROP        = 1
SCI_GAWIBAWIBO      = 2
SCI_MANUFACTURE     = 3
SCI_ERRAND          = 4
SCI_ECLIPSE_TIME    = 5
SCI_RK_TORNADO      = 6
SCI_SONGKRAN        = 7
SCI_ACCUMULATE_ATTENDANCE = 8
SCI_GACHA           = 9
SCI_DICE_BOARD      = 10
SCI_HERO_DUNGEON    = 11

ScriptUpdatePlan =
{
    ---[[
    {
        CheckTime   = { 2013, 05, 28, 00, 00 }, -- YYYY, MM, DD, HH, mm
        ScriptInfo   = {
            --{ SCI_INITDROP,         "InitDrop_1.lua", },
            --{ SCI_GAMEDROP,         "GameDrop.lua", },
            --{ SCI_GAWIBAWIBO,       "InitGawibawibo.lua", },
            --{ SCI_MANUFACTURE,      "InitManufactureS3.lua", },
            --{ SCI_ERRAND,           "InitErrand.lua", },
            --{ SCI_ECLIPSE_TIME,     "InitEclipseTimeEvent.lua", },
            --{ SCI_RK_TORNADO,       "InitRKTornado.lua", },
            --{ SCI_SONGKRAN,         "InitSongkranEvent.lua", },
            --{ SCI_ACCUMULATE_ATTENDANCE, "InitAccumulateAttendance.lua", },
            --{ SCI_GACHA,            "InitGacha.lua", },
            { SCI_DICE_BOARD,       "InitBoardInfo.lua", },
            { SCI_HERO_DUNGEON,     "InitHeroDungeon_1.lua", },
        },
    },
    --]]
    --[[
	{
        CheckTime   = { 2013, 04, 02, 05, 00 }, -- YYYY, MM, DD, HH, mm
        ScriptInfo   = {
            { SCI_INITDROP, "InitDrop_1.lua", },
            { SCI_GAMEDROP, "GameDrop_1.lua", },
        },
    },
	{
        CheckTime   = { 2013, 04, 03, 05, 00 }, -- YYYY, MM, DD, HH, mm
        ScriptInfo   = {
            { SCI_INITDROP, "InitDrop_2.lua", },
            { SCI_GAMEDROP, "GameDrop_2.lua", },
        },
    },
    --]]
}
