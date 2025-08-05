require( "Enum.stg" )

TRUE = 1
FALSE = 0

MaxLevel = 85
Channel_Category = {
    -- Channel, <Modes...>
    { CT_DUNGEON, {  GC_GMC_DUNGEON, }, },
    { CT_GUILD, { GC_GMC_GUILD_BATTLE, }, },
    { CT_INDIGO, { GC_GMC_INDIGO, }, },
	{ CT_UNLIMITED, { GC_GMC_MATCH, GC_GMC_DEATHMATCH, }, }, --GC_GMC_FATAL_DEATHMATCH, }, },
    { CT_SQUARE, { GC_GMC_MINIGAME, }, },
	{ CT_AGIT, { GC_GMC_AGIT, }, },
	{ CT_GUILD_UNLIMITED, { GC_GMC_GUILD_BATTLE, }, },
	{ CT_GUILD_MASTER, { GC_GMC_GUILD_BATTLE, }, },
	{ CT_GUILD_EXPERT, { GC_GMC_GUILD_BATTLE, }, },
	{ CT_GUILD_ROOKIE, { GC_GMC_GUILD_BATTLE, }, },
	{ CT_GUILD_BEGINNER, { GC_GMC_GUILD_BATTLE, }, },
	{ CT_AUTOMATCH, { GC_GMC_AUTOMATCH, }, },
}

Category_Mode = {
	{ GC_GMC_MATCH, { GC_GM_TEAM_MODE, GC_GM_SURVIVAL_MODE, }, },
	{ GC_GMC_GUILD_BATTLE, { GC_GM_GUILD_BATTLE,}, },
	{ GC_GMC_INDIGO, { GC_GM_INDIGO_TEAM, }, },
	{ GC_GMC_TUTORIAL, { GC_GM_TUTORIAL, }, },
	{ GC_GMC_TAG_MATCH, { GC_GM_TAG_TEAM, GC_GM_TAG_SURVIVAL, }, },
	{ GC_GMC_MONSTER_HUNT, { GC_GM_MONSTER_HUNT, }, },
	{ GC_GMC_DEATHMATCH, { GC_GM_DEATH_TEAM, GC_GM_DEATH_SURVIVAL }, },
	--{ GC_GMC_FATAL_DEATHMATCH, { GC_GM_FATAL_DEATH_TEAM, GC_GM_FATAL_DEATH_SURVIVAL, }, },
	{ GC_GMC_DUNGEON, 
		{
			GC_GM_QUEST0,
			GC_GM_QUEST1,
			GC_GM_QUEST2,
			GC_GM_QUEST3,
			GC_GM_QUEST4,
			GC_GM_QUEST5,
			GC_GM_QUEST6,
			GC_GM_QUEST7,
			GC_GM_QUEST8,
			GC_GM_QUEST9,
			GC_GM_QUEST10,
			GC_GM_QUEST11,
			GC_GM_QUEST12,
			GC_GM_QUEST13,
			GC_GM_QUEST14,
			GC_GM_QUEST15,
			GC_GM_QUEST16,
			GC_GM_QUEST17,
			GC_GM_QUEST18,
			GC_GM_QUEST19,
			GC_GM_QUEST20,
			GC_GM_QUEST21,
			GC_GM_QUEST22,
			GC_GM_QUEST23,
			GC_GM_QUEST24,
			GC_GM_QUEST25,
			GC_GM_QUEST26,
			GC_GM_QUEST27,
			GC_GM_QUEST28, -- 원시의섬
			GC_GM_QUEST31, -- 조율의 신전
			
			GC_GM_QUEST29, --실버랜드 용암지대
			GC_GM_QUEST30, -- 실버랜드 빅터의 요새
			GC_GM_QUEST32, -- 제니아 돌산
			GC_GM_QUEST33, -- 제니아 숲지역
			GC_GM_QUEST34, -- 제니아 지배의 성
--			GC_GM_QUEST35, -- 베르메시아 보스 던전
--			GC_GM_QUEST36, -- 실버랜드 보스 던전
--			GC_GM_QUEST37, -- 엘리아 보스 던전
--			GC_GM_QUEST38, -- 제니아 보스 던전
			GC_GM_QUEST39, -- GoblinLaborCamp
			GC_GM_QUEST40, -- UndergroundTunnel
			GC_GM_QUEST41, -- KungjiVillage
			GC_GM_QUEST42, -- DwarvenBase
			GC_GM_QUEST43, -- mirage desert
			GC_GM_QUEST44, -- dwarf supply
			GC_GM_QUEST45, -- 
			GC_GM_QUEST46, -- 파멸의 성소(영던)
			GC_GM_QUEST47, -- 영웅던전 베르메시아
			GC_GM_QUEST48, -- 고대왕국의 잔해
--			GC_GM_QUEST49, -- 이벤트 카니발
--			GC_GM_QUEST50, -- 이벤트 릴레이
--			GC_GM_QUEST51, -- 이벤트 카니발( 드라마 추가 버전 )
			GC_GM_QUEST52, -- 릴레이 영던
--			GC_GM_QUEST53, -- 명계열차
--			GC_GM_QUEST54, -- 배고픈 펫들의 반란
			GC_GM_QUEST55, -- 카루엘 접경지
			GC_GM_QUEST56, -- 카루엘
--			GC_GM_QUEST57, -- 야시장
			GC_GM_QUEST58, -- 레이드던전
			GC_GM_QUEST59, -- 아툼 사막 도적마
			GC_GM_QUEST60, -- 라 협곡
			GC_GM_QUEST61, -- 세트의 피라미드
			GC_GM_QUEST62, -- 코우나트 붕괴지
--			GC_GM_QUEST63, -- 코우나트 멘탈 붕괴지
--			GC_GM_QUEST64, -- 탈린의 역습
			GC_GM_QUEST65, -- 저주받은 정원
			GC_GM_QUEST66, -- 오염된 지하실
			GC_GM_QUEST67, -- 망령이 깃든 복도
			GC_GM_QUEST68, -- 배신자의 화실
			GC_GM_QUEST69, -- 어둠에 물든 서재
			GC_GM_QUEST70, -- 아라크네의 둥지
			GC_GM_QUEST71, -- 골리아스 서식지
			GC_GM_QUEST72, -- 고대 골렘의 무덤
			GC_GM_QUEST73, -- 그림자 밀림
			GC_GM_QUEST74, -- 라세르타의 사냥터
			GC_GM_QUEST75, -- 절망의 대지
			GC_GM_QUEST76, -- 차원의 경계
			GC_GM_QUEST77, -- 엘리아 사냥터
			GC_GM_QUEST78, -- 제니아 사냥터 
			GC_GM_QUEST79, -- 아툼 사냥터 
			GC_GM_QUEST80, -- 아케메디아 사냥터 
		}, 
	},
	{ GC_GMC_MINIGAME, { GC_GM_MINIGAME_TREEDROP, GC_GM_MINIGAME_BALLOON }, },
--  { GC_GMC_ANGELS_EGG, { GC_GM_ANGELS_EGG,  }, },
--	{ GC_GMC_CAPTAIN, { GC_GM_CAPTAIN,  }, },
	{ GC_GMC_AGIT, { GC_GM_AGIT, }, },
	{ GC_GMC_AUTOMATCH, { GC_GM_AUTOMATCH_TEAM, }, },
}

Category_Info = {
    -- Mode, Random, <Maps>
	{ GC_GMC_TUTORIAL, FALSE, { GC_GS_FOREST_OF_ELF, }, },
	{ GC_GMC_MATCH, TRUE, 
		{
			--GC_GS_AMAZON_FOREST,
			--GC_GS_BAHIA,
			--GC_GS_CASTLE_OF_RULE,
			--GC_GS_MARKET_WATER,	
			GC_GS_VALLEY_OF_OATH,
			GC_GS_FOREST_OF_ELF,
			GC_GS_FOREST_OF_ELF_REVERS,
			GC_GS_GUILD,
			GC_GS_HELL_BRIDGE,
			GC_GS_VALLEY_OF_OATH_REVERS, 
			GC_GS_FLYING_SHIP,
			GC_GS_SHOOTING_FIELD,
			GC_GS_FOGOTTEN_CITY,
			GC_GS_BABEL_OF_X_MAS,
			GC_GS_SWAMP_OF_OBLIVION,
			GC_GS_TEMPLE_OF_FIRE,
			GC_GS_CARRIBEACH,
			GC_GS_TEMPLE_OF_ORC,
			GC_GS_FOREST_OF_TRIAL,
			GC_GS_GRAVEYARD_OF_DECEASED,
		
		}, 
	},

	{ GC_GMC_GUILD_BATTLE, TRUE,
		{
			GC_GS_GUILD,
			GC_GS_FOREST_OF_ELF,
			GC_GS_SWAMP_OF_OBLIVION,
			GC_GS_FLYING_SHIP,
			GC_GS_VALLEY_OF_OATH,
			GC_GS_TEMPLE_OF_ORC,
			GC_GS_FOGOTTEN_CITY,
			GC_GS_TEMPLE_OF_FIRE,
			GC_GS_BABEL_OF_X_MAS,
			GC_GS_HELL_BRIDGE,
			GC_GS_SHOOTING_FIELD,
			GC_GS_CARRIBEACH,
		}, 
	},

	{ GC_GMC_TAG_MATCH, TRUE, 
		{
			GC_GS_FOREST_OF_ELF_REVERS,
			GC_GS_VALLEY_OF_OATH_REVERS, 
			GC_GS_FOREST_OF_ELF,
			GC_GS_SWAMP_OF_OBLIVION,
			GC_GS_FLYING_SHIP,
			GC_GS_VALLEY_OF_OATH,
			GC_GS_FOGOTTEN_CITY,
			GC_GS_BABEL_OF_X_MAS,
			GC_GS_TEMPLE_OF_FIRE,
			GC_GS_SHOOTING_FIELD,
			GC_GS_HELL_BRIDGE,
			GC_GS_TEMPLE_OF_ORC,
			GC_GS_GUILD,
			GC_GS_CARRIBEACH,
			GC_GS_FOREST_OF_TRIAL,
			GC_GS_GRAVEYARD_OF_DECEASED,
		}, 
	},

	{ GC_GMC_INDIGO, TRUE, 
		{
			GC_GS_FOREST_OF_ELF,
			GC_GS_SWAMP_OF_OBLIVION,
			GC_GS_TEMPLE_OF_ORC,
			GC_GS_CARRIBEACH,
			GC_GS_FLYING_SHIP,
			GC_GS_GUILD,
		}, 
	},

	{ GC_GMC_DEATHMATCH, TRUE, 
		{ 
			GC_GS_VALLEY_OF_OATH,
			GC_GS_FOREST_OF_ELF,
			GC_GS_FOREST_OF_ELF_REVERS,
			GC_GS_GUILD,			
			GC_GS_HELL_BRIDGE,
			GC_GS_VALLEY_OF_OATH_REVERS, 
			GC_GS_FLYING_SHIP,
			GC_GS_SHOOTING_FIELD,
			GC_GS_FOGOTTEN_CITY,			
			GC_GS_SWAMP_OF_OBLIVION,
			GC_GS_TEMPLE_OF_FIRE,			
			GC_GS_CARRIBEACH,
			GC_GS_TEMPLE_OF_ORC,
			GC_GS_FOREST_OF_TRIAL,
			GC_GS_GRAVEYARD_OF_DECEASED,
		}, 
	},	
	{ GC_GMC_CAPTAIN, FALSE, 
		{ 
			GC_GS_FOREST_OF_ELF,
			GC_GS_SWAMP_OF_OBLIVION,
			GC_GS_FLYING_SHIP,
			GC_GS_VALLEY_OF_OATH,
			GC_GS_FOGOTTEN_CITY,
			GC_GS_TEMPLE_OF_FIRE,
			GC_GS_SHOOTING_FIELD,
			GC_GS_HELL_BRIDGE,
			GC_GS_TEMPLE_OF_ORC,
			GC_GS_GUILD,
			GC_GS_CARRIBEACH,
			GC_GS_FOREST_OF_TRIAL,
			GC_GS_GRAVEYARD_OF_DECEASED,
		} , },

	{ GC_GMC_ANGELS_EGG, FALSE, 
		{ 
			GC_GS_FOREST_OF_ELF,
			GC_GS_GUILD,
		}, 
	},

	{ GC_GMC_MONSTER_CRUSADER, FALSE, { GC_GS_FOREST_OF_ELF, }, },
	{ GC_GMC_DOTA, FALSE, { GC_GS_FOREST_OF_ELF, }, },
	{ GC_GMC_AUTOMATCH, TRUE, 
		{	
			--GC_GS_AMAZON_FOREST,
			--GC_GS_BAHIA,
			--GC_GS_CASTLE_OF_RULE,
			GC_GS_FOREST_OF_ELF_REVERS,
--			GC_GS_VALLEY_OF_OATH_REVERS, 
			GC_GS_FOREST_OF_ELF,
--			GC_GS_SWAMP_OF_OBLIVION,
--			GC_GS_FLYING_SHIP,
			GC_GS_VALLEY_OF_OATH,
--			GC_GS_FOGOTTEN_CITY,
--			GC_GS_BABEL_OF_X_MAS,
--			GC_GS_TEMPLE_OF_FIRE,
--			GC_GS_SHOOTING_FIELD,
			GC_GS_HELL_BRIDGE,
--			GC_GS_TEMPLE_OF_ORC,
			GC_GS_GUILD,
--			GC_GS_CARRIBEACH,
--			GC_GS_FOREST_OF_TRIAL,
--			GC_GS_GRAVEYARD_OF_DECEASED,
		}, 
	},
	
	{ GC_GMC_FATAL_DEATHMATCH, TRUE, 
		{
			--GC_GS_AMAZON_FOREST,
			--GC_GS_BAHIA,
			--GC_GS_CASTLE_OF_RULE,
			--GC_GS_MARKET_WATER,	
			GC_GS_VALLEY_OF_OATH,
			GC_GS_FOREST_OF_ELF,
			GC_GS_FOREST_OF_ELF_REVERS,
			GC_GS_GUILD,
			GC_GS_HELL_BRIDGE,
			GC_GS_VALLEY_OF_OATH_REVERS, 
			GC_GS_FLYING_SHIP,
			GC_GS_SHOOTING_FIELD,
			GC_GS_FOGOTTEN_CITY,
			GC_GS_BABEL_OF_X_MAS,
			GC_GS_SWAMP_OF_OBLIVION,
			GC_GS_TEMPLE_OF_FIRE,
			GC_GS_CARRIBEACH,
			GC_GS_TEMPLE_OF_ORC,
			GC_GS_FOREST_OF_TRIAL,
			GC_GS_GRAVEYARD_OF_DECEASED,
		}, 
	},
}

Character_Open_Info = {
    -- char Type, set<Promotion>
    { CharType = 0, CharPromotion = {0,1,2,3}, },	
    { CharType = 1, CharPromotion = {0,1,2,3}, },	
    { CharType = 2, CharPromotion = {0,1,2,3}, },	
    { CharType = 3, CharPromotion = {0,1,2,3}, },	
    { CharType = 4, CharPromotion = {0,1,2,3}, },	
    { CharType = 5, CharPromotion = {0,1,2,3}, },	
    { CharType = 6, CharPromotion = {0,1,2,3}, },	
    { CharType = 7, CharPromotion = {0,1,2,3}, },	
    { CharType = 8, CharPromotion = {0,1,2,3}, },	
    { CharType = 9, CharPromotion = {0,1,2,3}, },	
    { CharType = 10, CharPromotion = {0,1,2,3}, }, 
    { CharType = 11, CharPromotion = {0,1,2,3}, }, 
	{ CharType = 12, CharPromotion = {0,1,2,3}, }, 
	{ CharType = 13, CharPromotion = {0,1,2,3}, }, 
	{ CharType = 14, CharPromotion = {0,1,2,3}, }, 
	{ CharType = 15, CharPromotion = {0,1}, }, 
	{ CharType = 16, CharPromotion = {0,1}, }, 
	{ CharType = 17, CharPromotion = {0}, }, 
	{ CharType = 18, CharPromotion = {0}, }, 
}

Character_SP4_Open_Info = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 }

Char_Cash_Goods = {

	{  0,  99135, },
	{  1,  99136, },
	{  2,  99137, },
	{  3,  99138, },
	{  4,  99139, },
	{  5,  99140, },
	{  6,  99141, },
	{  7,  99142, },
	{  8,  99143, },
	{  9,  99144, },
	{ 10,  99145, },
	{ 11,  99146, },
	{ 12,  99147, },
	{ 13,  99148, },
	{ 14,  99149, },
	{ 15,  99150, },
	{ 16,  99151, },
	{ 17,  123698, },
	{ 18,  123975, },
}

Char_Open_Type = 
{
	-- CharType , OpenType ( 0:Free 1:Billing 2:Have 3:Event 4:New, 5:Discount, 6:SealedEvent, Default = 0)
    { 0, 2, },
    { 1, 2, },
    { 2, 2, },
    { 3, 1, },
    { 4, 1, },
    { 5, 1, },
    { 6, 1, },
    { 7, 1, },
    { 8, 0, },
    { 9, 1, },
    { 10,1, },
    { 11,1, },
    { 12,1, },
    { 13,0, },
    { 14,0, },
    { 15,0, },
    { 16,0, },
	{ 17,0, },
	{ 18,0, }
}

Disable_Menu_Btn_List = { SMB_CARD, SMB_VIP }--{ SMB_VIP } --, SMB_CARD, }

--ChannelNewsImageNumber = { 1, 2, 3, 4, 5, 6, 7}
ChannelNewsImageNumber = { 1, 2, 3 }

LoadingImageFileName = {
	{ "Load1_1.dds", "Load1_2.dds", "Load1_3.dds", "LoadGauge1.dds", 25},
	--{ "Load2_1.dds", "Load2_2.dds", "Load2_3.dds", "LoadGauge1.dds", 25},
	--{ "Load3_1.dds", "Load3_2.dds", "Load3_3.dds", "LoadGauge1.dds", 25},
	--{ "Load4_1.dds", "Load4_2.dds", "Load4_3.dds", "LoadGauge1.dds", 25},
}

PVPLoadingImageFileName = {
    {"ui_match_load1.dds",  "ui_match_load2.dds",  "ui_match_load3.dds", },
	--{"ui_match2_load1.dds", "ui_match2_load2.dds", "ui_match2_load3.dds", },
	--{"ui_match3_load1.dds", "ui_match3_load2.dds", "ui_match3_load3.dds", },
	--{"ui_match4_load1.dds", "ui_match4_load2.dds", "ui_match4_load3.dds", },
}

ClientScriptName = {
    { 0, "Square.lua" },
	--{ 0, "Square_festival.lua" },
	--{ 1, "SquareObject_amyfree.lua" },
    --{ 2, "Square3DObject_amyfree.lua" },
    { 1, "SquareObject.lua" },
    { 2, "Square3DObject.lua" },
}

--[[
ExceptionMotionID = {
			675,
			901,
			1325,

			1184,1209,
			1461,1483,
			2097,2075,
 
			1811,
			2179,
			2371,

			2527,

			1855,
			2299,
		 }
]]--

ExceptionMotionID = { }
NEW = 0
GOING = 1
END = 2
-- 이벤트 배너 texture 사용여부.
EVENT_BANNER_TEXTURE = { 
	{ "banner1.dds", NEW, },
	{ "banner2.dds", NEW, },
	{ "banner3.dds", GOING, },
	{ "banner4.dds", END, },
--	{ "banner5.dds", GOING, },
--	{ "banner6.dds", GOING, },
--	{ "banner7.dds", GOING, },
--	{ "banner8.dds", GOING, },
}

-- 캐릭터 스킬트리 ON/OFF 
-- 오픈된 캐릭터 TYPE에 대해서만 추가.
CHARSKILL_ENABLE = { }

-- 클라이언트에서 사용할 STRING
ClientMsg = "";

-- 핫 이벤트 (클라이언트 UI )
HOT_EVENT_ID = -1

-- 시즌4 업데이트 날짜
TimeS4 = { 2011, 03, 29, 09 }

BenefitMsg = {
    --{ 0, "Square_Halloween.lua" },
	--{ 1, "EXP 30%" },
	--{ 2, "EXP 30% + Mission Drop 5%" },
	{ 13, "장착장비 관계없이 능력치가 고정된 특수 이벤트서버" },

}

ChampionAbilityRate = {}

DLLBlackList = {}

-- 미니게임 
EventOpenInfo = {
--[[
  EventID(STR)
  8616	코우나트 마법상자
  8788	주사위 게임
  8789	퀴즈 빙고 게임
  8790	가위바위보 게임
  8791	스핑크스 OX 게임
  8792	선물상자 이벤트
  8793	양말걸기 이벤트
  8794	나무나무 이벤트
  8795	릭쿤 토네이도
  EventBoxID(Enum)
  GC_MBOX_HALLOWEEN_DLG			=104 	-- 주사위		-- tex_gc_mbox_halloween_dlg.dds (그림파일 명 및 규칙임)
  GC_MBOX_BINGO_DLG				=117 	-- 빙고			-- tex_gc_mbox_bingo_dlg.dds
  GC_MBOX_PUZZLE_DLG			=126 	-- 퍼즐 (선물상자)
  GC_MBOX_GAWIBAWIBO_DLG 		=142	-- 가위바위보
  GC_MBOX_SOCKS_TREE_DLG 		=145	-- 양말걸기
  GC_MBOX_PLANT_TREE_DLG 		=149	-- 나무나무
  GC_MBOX_MAGIC_BOX 			=159	-- 코우나트 마법상자
  GC_MBOX_RICKOON_TORNADO_DLG 	=162	-- 릭쿤토네이도
  GC_MBOX_EVENT_SPHINX_DLG 		=163	-- 스핑크스 OX
  GC_MBOX_SONGKRAN_DLG 			=191	-- 송크란
  GC_MBOX_OLYMPIC_DLG			=195	-- 올림픽 
  GC_MBOX_GC9th_EVENT_DLG 		=207	-- 9주년 이벤트 
  GC_MBOX_GC_ADVENTURE_DLG		=194	-- GC원정대
  GC_MBOX_ATTENDANCE_STAMP_DLG = 220
  
  MINIGAME_WEB_EVENT			=9999	-- 웹 이벤트용, 웹 띄울 용도로 사용 
  
	{
		EventID = 6996, -- 이벤트 이름 스트링 아이디, str.stg
		EventMBoxID = GC_MBOX_HALLOWEEN_DLG, -- 위쪽 DLG
		EventTextureFileName = "tex_gc_mbox_halloween_dlg.dds", -- 이벤트 그림 파일 이름(tex_DGL소문자버젼)
		EventItemList = { 8880, 110,}, -- 아이템 획득시 알람 켜줄 아이템 리스트.
	},
--]]

	--[[
	-- 스핑크스
	{
		EventID = 8791,
		EventMBoxID = 163,
		EventTextureFileName = "tex_gc_mbox_event_sphinx_dlg.dds",
		EventItemList = {596860,},
	},
	--]]

	--[[	
	--주사위이벤트
	{
		EventID = 8788,
		EventMBoxID = 104,
		EventTextureFileName = "tex_gc_mbox_halloween_dlg.dds",
		EventItemList = {329430,},
	},
	--]]
	
	--[[
	--퀴즈빙고이벤트
	{
		EventID = 8789,
		EventMBoxID = 117,
		EventTextureFileName = "tex_gc_mbox_bingo_dlg.dds",
		EventItemList = {206770,},
	},
	--]]
	
	--[[
	--가위바위보게임
	{
		EventID = 8790,
		EventMBoxID = 142,
		EventTextureFileName = "tex_gc_mbox_element_dlg.dds",
		EventItemList = {413430,},
	},
	--]]
	
	--[[
	--선물상자 이벤트
	{
		EventID = 8792,
		EventMBoxID = 126,
		EventTextureFileName = "tex_gc_mbox_puzzle_dlg.dds",
		EventItemList = {301750,301760,301770,301780,301790,301800,301810,301820,301830,},
	},
	--]]
	
	--[[
	--양말걸기 이벤트
	{
		EventID = 8793,
		EventMBoxID = 145,
		EventTextureFileName = "tex_gc_mbox_socks_tree_dlg.dds",
		EventItemList = {451070,451080,451090,451100,451110,451120,},
	},
	--]]
	
	--[[
	--나무나무 이벤트
	{
		EventID = 8794,
		EventMBoxID = 149,
		EventTextureFileName = "tex_gc_mbox_plant_tree_dlg.dds",
		EventItemList = {541910,541920,541930,},
	},
	--]]
	
	--[[
	--릭쿤 토네이도
	{
		EventID = 8795,
		EventMBoxID = 162,
		EventTextureFileName = "tex_gc_rickoon_tornado_dlg.dds",
		EventItemList = {582430,},
	},
	--]]
	
	--[[
	-- GWC 2011	
	{
		EventID = 9416,
		EventMBoxID = 176,
		EventTextureFileName = "tex_gc_mbox_gwc_dlg.dds",
		EventItemList = {728180, 35030, 149150, },
	},
	--]]

	--[[
	-- 코우나트 마법상자
	{
		EventID = 8616, 
		EventMBoxID = 159, 
		EventTextureFileName = "tex_gc_mbox_magic_box.dds", -- 이벤트 그림 파일 이름(tex_DGL소문자버젼)
		EventItemList = { }, -- 아이템 획득시 알람 켜줄 아이템 리스트.
	},
	--]]

	--[[
	-- 크리스마스 이벤트
	{
		EventID = 9536,
		EventMBoxID = 179,
		EventTextureFileName = "tex_gc_mbox_gift_shop.dds",
		EventItemList = { },
	},
	--]]
	
	--[[
	-- 개기 일식
	{
		EventID = 9854,
		EventMBoxID = 183,
		EventTextureFileName = "tex_gc_eclipse_plot_dlg.dds",
		EventItemList = { 791120, },
	},
	--]]

	--[[
	-- 고서 
	{
		EventID = 9855,
		EventMBoxID = 184,
		EventTextureFileName = "tex_gc_old_book_dlg.dds",
		EventItemList = { 791150, 791160, 791170, 791180, 791190, 791200, 791210, 791220, 791230 },
	},
	--]]

	--[[	
	-- 웹 이벤트
	{
		EventID = 10298,
		EventMBoxID = 9999,
		EventTextureFileName = "tex_gc_web_event.dds",
		EventItemList = {},
	},
	--]]
	
	--[[
	-- 송크란 
	{
		EventID = 10463,
		EventMBoxID = 191,
		EventTextureFileName = "tex_gc_mbox_songkran_dlg.dds",
		EventItemList = {},
	},
	--]]
	--[[
	-- 올림픽 
	{
		EventID = 10586,
		EventMBoxID = 195, 
		EventTextureFileName = "tex_gc_mbox_olympic_dlg.dds",
		EventItemList = {},
	},
	--]]
	
	--[[
	-- 9주년 이벤트 
	{
		EventID = 10794, 
		EventMBoxID = 207, 
		EventTextureFileName = "tex_gc_9th_event.dds", 
		EventItemList = { }, 
	},
	--]]

	--[[	
	-- 아이템 합성
	{
		EventID = 10909, 
		EventMBoxID = 209, 
		EventTextureFileName = "tex_gc_item_combination_dlg.dds", 
		EventItemList = { }, 
	},
	--]]
		
	--[[
	-- GC원정대
	{
		EventID = 10909, 
		EventMBoxID = 194, 
		EventTextureFileName = "tex_gc_adventure_dlg.dds", 
		EventItemList = { }, 
	},
	--]]
	
	--[[
	-- 리타의 크리스마스
	{
		EventID = 11230, 
		EventMBoxID = 218, 
		EventTextureFileName = "tex_gc_ritas_christmas.dds", 
		EventItemList = { }, 
	},
	--]]
	
	---[[
	-- 화령의 스템프
	{
		EventID = 11463,
		EventMBoxID = 220,
		EventTextureFileName = "tex_gc_play_with_zero.dds",
		EventItemList = { },
	},
	--]]

	--[[
	-- 가챠 복권
	{
		EventID = 10772,
		EventMBoxID = 210,
		EventTextureFileName = "tex_lucky_coupon.dds",
		EventItemList = { },
	},
	--]]
	
	--[[
	-- 도전 심부름 왕
	{
		EventID = 11874,
		EventMBoxID = 227,
		EventTextureFileName = "tex_gc_errand_dlg.dds",
		EventItemList = { },
	},
	--]]
}

ExtendSHAFileList = {}

EventMonsterAbility = { -- 자기것만 적용되며, 자기것이 없을땐 공용(-1)을 적용 (100은 100%, 50은 50%)
--[[
	{
		MonsterID = -1, -- -1 이면 모든 몬스터.
		AttackRatio = 120,  -- 20%능력치 상승
		DefenceRatio = 120,  -- 20%능력치 상승
		HPRatio = 120,  -- 20%능력치 상승
	},
]]--
}

----------------------------------
SmartPanelMailListEnable = FALSE
SmartPanelMailList = 
{
--[[
	"@changyou.com",
	"@qq.com",
	"@163.com",
	"@game.sohu.com",
	"@sohu.com",
	"@126.com",
	"@sina.com",
	"@vip.qq.com",
	"@17173.com",
	"@hotmail.com",
	"@foxmail.com",
	"@yahoo.com",
	"@yahoo.com.cn",
	"@yahoo.cn",
	"@263.net",
	"@gmail.com",
	"@msn.com",
	"@tom.com",
	"@changren.com",	
--]]
}

URL = 0
IMAGE = 1
ChannelImageDownloadURLList = 
{
--[[
	{ URL, "http://gc.changyou.com/content/launcher/tzzy/" }, -- 주소
	{ IMAGE, "http://files2.changyou.com/yhmx/pic/" }, -- 이미지
--]]
}
SlotItemNum = 1213960
