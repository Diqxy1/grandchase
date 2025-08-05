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
			GC_GM_QUEST28, -- �����Ǽ�
			GC_GM_QUEST31, -- ������ ����
			
			GC_GM_QUEST29, --�ǹ����� �������
			GC_GM_QUEST30, -- �ǹ����� ������ ���
			GC_GM_QUEST32, -- ���Ͼ� ����
			GC_GM_QUEST33, -- ���Ͼ� ������
			GC_GM_QUEST34, -- ���Ͼ� ������ ��
--			GC_GM_QUEST35, -- �����޽þ� ���� ����
--			GC_GM_QUEST36, -- �ǹ����� ���� ����
--			GC_GM_QUEST37, -- ������ ���� ����
--			GC_GM_QUEST38, -- ���Ͼ� ���� ����
			GC_GM_QUEST39, -- GoblinLaborCamp
			GC_GM_QUEST40, -- UndergroundTunnel
			GC_GM_QUEST41, -- KungjiVillage
			GC_GM_QUEST42, -- DwarvenBase
			GC_GM_QUEST43, -- mirage desert
			GC_GM_QUEST44, -- dwarf supply
			GC_GM_QUEST45, -- 
			GC_GM_QUEST46, -- �ĸ��� ����(����)
			GC_GM_QUEST47, -- �������� �����޽þ�
			GC_GM_QUEST48, -- ���ձ��� ����
--			GC_GM_QUEST49, -- �̺�Ʈ ī�Ϲ�
--			GC_GM_QUEST50, -- �̺�Ʈ ������
--			GC_GM_QUEST51, -- �̺�Ʈ ī�Ϲ�( ��� �߰� ���� )
			GC_GM_QUEST52, -- ������ ����
--			GC_GM_QUEST53, -- ��迭��
--			GC_GM_QUEST54, -- ����� ����� �ݶ�
			GC_GM_QUEST55, -- ī�翤 ������
			GC_GM_QUEST56, -- ī�翤
--			GC_GM_QUEST57, -- �߽���
			GC_GM_QUEST58, -- ���̵����
			GC_GM_QUEST59, -- ���� �縷 ������
			GC_GM_QUEST60, -- �� ����
			GC_GM_QUEST61, -- ��Ʈ�� �Ƕ�̵�
			GC_GM_QUEST62, -- �ڿ쳪Ʈ �ر���
--			GC_GM_QUEST63, -- �ڿ쳪Ʈ ��Ż �ر���
--			GC_GM_QUEST64, -- Ż���� ����
			GC_GM_QUEST65, -- ���ֹ��� ����
			GC_GM_QUEST66, -- ������ ���Ͻ�
			GC_GM_QUEST67, -- ������ ��� ����
			GC_GM_QUEST68, -- ������� ȭ��
			GC_GM_QUEST69, -- ��ҿ� ���� ����
			GC_GM_QUEST70, -- �ƶ�ũ���� ����
			GC_GM_QUEST71, -- �񸮾ƽ� ������
			GC_GM_QUEST72, -- ��� ���� ����
			GC_GM_QUEST73, -- �׸��� �и�
			GC_GM_QUEST74, -- �󼼸�Ÿ�� �����
			GC_GM_QUEST75, -- ������ ����
			GC_GM_QUEST76, -- ������ ���
			GC_GM_QUEST77, -- ������ �����
			GC_GM_QUEST78, -- ���Ͼ� ����� 
			GC_GM_QUEST79, -- ���� ����� 
			GC_GM_QUEST80, -- ���ɸ޵�� ����� 
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
-- �̺�Ʈ ��� texture ��뿩��.
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

-- ĳ���� ��ųƮ�� ON/OFF 
-- ���µ� ĳ���� TYPE�� ���ؼ��� �߰�.
CHARSKILL_ENABLE = { }

-- Ŭ���̾�Ʈ���� ����� STRING
ClientMsg = "";

-- �� �̺�Ʈ (Ŭ���̾�Ʈ UI )
HOT_EVENT_ID = -1

-- ����4 ������Ʈ ��¥
TimeS4 = { 2011, 03, 29, 09 }

BenefitMsg = {
    --{ 0, "Square_Halloween.lua" },
	--{ 1, "EXP 30%" },
	--{ 2, "EXP 30% + Mission Drop 5%" },
	{ 13, "������� ������� �ɷ�ġ�� ������ Ư�� �̺�Ʈ����" },

}

ChampionAbilityRate = {}

DLLBlackList = {}

-- �̴ϰ��� 
EventOpenInfo = {
--[[
  EventID(STR)
  8616	�ڿ쳪Ʈ ��������
  8788	�ֻ��� ����
  8789	���� ���� ����
  8790	���������� ����
  8791	����ũ�� OX ����
  8792	�������� �̺�Ʈ
  8793	�縻�ɱ� �̺�Ʈ
  8794	�������� �̺�Ʈ
  8795	���� ����̵�
  EventBoxID(Enum)
  GC_MBOX_HALLOWEEN_DLG			=104 	-- �ֻ���		-- tex_gc_mbox_halloween_dlg.dds (�׸����� �� �� ��Ģ��)
  GC_MBOX_BINGO_DLG				=117 	-- ����			-- tex_gc_mbox_bingo_dlg.dds
  GC_MBOX_PUZZLE_DLG			=126 	-- ���� (��������)
  GC_MBOX_GAWIBAWIBO_DLG 		=142	-- ����������
  GC_MBOX_SOCKS_TREE_DLG 		=145	-- �縻�ɱ�
  GC_MBOX_PLANT_TREE_DLG 		=149	-- ��������
  GC_MBOX_MAGIC_BOX 			=159	-- �ڿ쳪Ʈ ��������
  GC_MBOX_RICKOON_TORNADO_DLG 	=162	-- ��������̵�
  GC_MBOX_EVENT_SPHINX_DLG 		=163	-- ����ũ�� OX
  GC_MBOX_SONGKRAN_DLG 			=191	-- ��ũ��
  GC_MBOX_OLYMPIC_DLG			=195	-- �ø��� 
  GC_MBOX_GC9th_EVENT_DLG 		=207	-- 9�ֳ� �̺�Ʈ 
  GC_MBOX_GC_ADVENTURE_DLG		=194	-- GC������
  GC_MBOX_ATTENDANCE_STAMP_DLG = 220
  
  MINIGAME_WEB_EVENT			=9999	-- �� �̺�Ʈ��, �� ��� �뵵�� ��� 
  
	{
		EventID = 6996, -- �̺�Ʈ �̸� ��Ʈ�� ���̵�, str.stg
		EventMBoxID = GC_MBOX_HALLOWEEN_DLG, -- ���� DLG
		EventTextureFileName = "tex_gc_mbox_halloween_dlg.dds", -- �̺�Ʈ �׸� ���� �̸�(tex_DGL�ҹ��ڹ���)
		EventItemList = { 8880, 110,}, -- ������ ȹ��� �˶� ���� ������ ����Ʈ.
	},
--]]

	--[[
	-- ����ũ��
	{
		EventID = 8791,
		EventMBoxID = 163,
		EventTextureFileName = "tex_gc_mbox_event_sphinx_dlg.dds",
		EventItemList = {596860,},
	},
	--]]

	--[[	
	--�ֻ����̺�Ʈ
	{
		EventID = 8788,
		EventMBoxID = 104,
		EventTextureFileName = "tex_gc_mbox_halloween_dlg.dds",
		EventItemList = {329430,},
	},
	--]]
	
	--[[
	--��������̺�Ʈ
	{
		EventID = 8789,
		EventMBoxID = 117,
		EventTextureFileName = "tex_gc_mbox_bingo_dlg.dds",
		EventItemList = {206770,},
	},
	--]]
	
	--[[
	--��������������
	{
		EventID = 8790,
		EventMBoxID = 142,
		EventTextureFileName = "tex_gc_mbox_element_dlg.dds",
		EventItemList = {413430,},
	},
	--]]
	
	--[[
	--�������� �̺�Ʈ
	{
		EventID = 8792,
		EventMBoxID = 126,
		EventTextureFileName = "tex_gc_mbox_puzzle_dlg.dds",
		EventItemList = {301750,301760,301770,301780,301790,301800,301810,301820,301830,},
	},
	--]]
	
	--[[
	--�縻�ɱ� �̺�Ʈ
	{
		EventID = 8793,
		EventMBoxID = 145,
		EventTextureFileName = "tex_gc_mbox_socks_tree_dlg.dds",
		EventItemList = {451070,451080,451090,451100,451110,451120,},
	},
	--]]
	
	--[[
	--�������� �̺�Ʈ
	{
		EventID = 8794,
		EventMBoxID = 149,
		EventTextureFileName = "tex_gc_mbox_plant_tree_dlg.dds",
		EventItemList = {541910,541920,541930,},
	},
	--]]
	
	--[[
	--���� ����̵�
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
	-- �ڿ쳪Ʈ ��������
	{
		EventID = 8616, 
		EventMBoxID = 159, 
		EventTextureFileName = "tex_gc_mbox_magic_box.dds", -- �̺�Ʈ �׸� ���� �̸�(tex_DGL�ҹ��ڹ���)
		EventItemList = { }, -- ������ ȹ��� �˶� ���� ������ ����Ʈ.
	},
	--]]

	--[[
	-- ũ�������� �̺�Ʈ
	{
		EventID = 9536,
		EventMBoxID = 179,
		EventTextureFileName = "tex_gc_mbox_gift_shop.dds",
		EventItemList = { },
	},
	--]]
	
	--[[
	-- ���� �Ͻ�
	{
		EventID = 9854,
		EventMBoxID = 183,
		EventTextureFileName = "tex_gc_eclipse_plot_dlg.dds",
		EventItemList = { 791120, },
	},
	--]]

	--[[
	-- �� 
	{
		EventID = 9855,
		EventMBoxID = 184,
		EventTextureFileName = "tex_gc_old_book_dlg.dds",
		EventItemList = { 791150, 791160, 791170, 791180, 791190, 791200, 791210, 791220, 791230 },
	},
	--]]

	--[[	
	-- �� �̺�Ʈ
	{
		EventID = 10298,
		EventMBoxID = 9999,
		EventTextureFileName = "tex_gc_web_event.dds",
		EventItemList = {},
	},
	--]]
	
	--[[
	-- ��ũ�� 
	{
		EventID = 10463,
		EventMBoxID = 191,
		EventTextureFileName = "tex_gc_mbox_songkran_dlg.dds",
		EventItemList = {},
	},
	--]]
	--[[
	-- �ø��� 
	{
		EventID = 10586,
		EventMBoxID = 195, 
		EventTextureFileName = "tex_gc_mbox_olympic_dlg.dds",
		EventItemList = {},
	},
	--]]
	
	--[[
	-- 9�ֳ� �̺�Ʈ 
	{
		EventID = 10794, 
		EventMBoxID = 207, 
		EventTextureFileName = "tex_gc_9th_event.dds", 
		EventItemList = { }, 
	},
	--]]

	--[[	
	-- ������ �ռ�
	{
		EventID = 10909, 
		EventMBoxID = 209, 
		EventTextureFileName = "tex_gc_item_combination_dlg.dds", 
		EventItemList = { }, 
	},
	--]]
		
	--[[
	-- GC������
	{
		EventID = 10909, 
		EventMBoxID = 194, 
		EventTextureFileName = "tex_gc_adventure_dlg.dds", 
		EventItemList = { }, 
	},
	--]]
	
	--[[
	-- ��Ÿ�� ũ��������
	{
		EventID = 11230, 
		EventMBoxID = 218, 
		EventTextureFileName = "tex_gc_ritas_christmas.dds", 
		EventItemList = { }, 
	},
	--]]
	
	---[[
	-- ȭ���� ������
	{
		EventID = 11463,
		EventMBoxID = 220,
		EventTextureFileName = "tex_gc_play_with_zero.dds",
		EventItemList = { },
	},
	--]]

	--[[
	-- ��í ����
	{
		EventID = 10772,
		EventMBoxID = 210,
		EventTextureFileName = "tex_lucky_coupon.dds",
		EventItemList = { },
	},
	--]]
	
	--[[
	-- ���� �ɺθ� ��
	{
		EventID = 11874,
		EventMBoxID = 227,
		EventTextureFileName = "tex_gc_errand_dlg.dds",
		EventItemList = { },
	},
	--]]
}

ExtendSHAFileList = {}

EventMonsterAbility = { -- �ڱ�͸� ����Ǹ�, �ڱ���� ������ ����(-1)�� ���� (100�� 100%, 50�� 50%)
--[[
	{
		MonsterID = -1, -- -1 �̸� ��� ����.
		AttackRatio = 120,  -- 20%�ɷ�ġ ���
		DefenceRatio = 120,  -- 20%�ɷ�ġ ���
		HPRatio = 120,  -- 20%�ɷ�ġ ���
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
	{ URL, "http://gc.changyou.com/content/launcher/tzzy/" }, -- �ּ�
	{ IMAGE, "http://files2.changyou.com/yhmx/pic/" }, -- �̹���
--]]
}
SlotItemNum = 1213960
