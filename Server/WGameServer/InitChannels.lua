
DUNGEON      = 0
GUILD        = 1
INDIGO       = 2
UNLIMITED    = 3
BEGINNER     = 4
ROOKIE       = 5
EXPERT       = 6
MASTER       = 7
AUTOMATCH	 = 22 -- ������Ī ä��

GUILD_UNLIMITED = 9 -- ��� ����
GUILD_BEGINNER = 20 -- ��� ������
GUILD_ROOKIE = 18 -- ��� �뺴
GUILD_EXPERT = 14 -- ��� �ո�
GUILD_MASTER = 10 -- ��� ��


ChannelManager = GetChannelManager()
ChannelManager:SetBroadCastingTime( 10000, 2000, 3000 ) -- Channel, Room, User
SetGuildBroadCastingTime( 7000, 60000 )                 -- Member, DB Sync.

ChannelManager:SetChannelLobby( 5000 )
--ChannelManager:ModifyChannelLobby( MaxUser )
--( ID, str, MaxUser, Grade, RoomNum, ModeList, Type )

--[[
1 ������ ��
2 ������ ��
3 ������ ��
4 �뺴 ��
5 �뺴 ��
6 �뺴 ��
7 �ո� ��
8 �ո� ��
9 �ո� ��
10 �� ��
11 �� ��
12 �� ��
--]]

-- Garde List
AllGrade        = { 1, 12 }
BeginnerGrade   = { 1, 2 }
RookieGrade     = { 3, 5 }
ExpertGrade     = { 6, 7 }
MasterGrade     = { 8, 12 }

FreeGuild = { 0, 9999 } -- ���� ��� ä��
WanderGuild = { 0 , 1199 } -- �����̱��
MercenaryGuild = { 1200, 1349 } -- �뺴 ���
RoyalGuild = { 1350, 1499 } -- �ո� ���
DivineGuild = { 1500, 9999 } -- ���� ���

-- IP ����Ʈ
all     = {'0.0.0.0', '255.255.255.255'}
seoul   = {'211.110.91.113', '211.110.91.204'}
incheon = {'124.28.138.131', '124.28.138.210'}
daejeon = {'61.32.195.131', '61.32.195.220'}
daegu   = {'123.140.253.1', '123.140.253.172'}
busan   = {'210.206.115.131', '210.206.115.212'}
guangju = {'59.0.205.11', '59.0.205.108'}
kog     = {'116.120.238.1', '116.120.238.255'}

IndigoModeList = {6,}
BattleModeList = { }
NormalModeList = {1, 2, 3, 4, 28, 31, 32, 37, 38, 65, 96, 97,}
EveryModeList  = {}
DungeonList  = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,22, 23, 24, 25, 26, 27, 30, 36, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 98, 99, 100, 101 }
GuildModeList  = { 1, 5,}
MatchModeList = {77,}


ChannelManager:AddChannel('str_01', 300, AllGrade, 150, NormalModeList, UNLIMITED, all )
ChannelManager:AddChannel('str_53', 300, AllGrade, 150, DungeonList, DUNGEON, all )

--if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_PVP ) ) then -- ��������
--	ChannelManager:AddChannel('str_01', 300, AllGrade, 150, NormalModeList, UNLIMITED, all )
--	ChannelManager:AddChannel('str_66', 300, AllGrade, 150, MatchModeList, AUTOMATCH, all )
--else -- ��������
--        ChannelManager:AddChannel('str_53', 300, AllGrade, 150, DungeonList, DUNGEON, all )
--end

--[[
if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_INDIGO ) ) then -- Indigo

    ChannelManager:AddChannel( 'str_23', 400, AllGrade, 150, IndigoModeList, INDIGO, all )

else if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_GUILD )) then -- Guild

    ChannelManager:AddChannel('str_61', 300, FreeGuild, 150, GuildModeList, GUILD_UNLIMITED, all )
    ChannelManager:AddChannel('str_62', 300, WanderGuild, 150, GuildModeList, GUILD_BEGINNER, all )
    ChannelManager:AddChannel('str_63', 300, MercenaryGuild, 150, GuildModeList, GUILD_ROOKIE, all )
    ChannelManager:AddChannel('str_64', 300, RoyalGuild, 150, GuildModeList, GUILD_EXPERT, all )
    ChannelManager:AddChannel('str_65', 300, DivineGuild, 150, GuildModeList, GUILD_MASTER, all )

    

    else -- Normal
        ChannelManager:AddChannel('str_01', 300, AllGrade, 150, NormalModeList, UNLIMITED, all )
        ChannelManager:AddChannel('str_09', 300, BeginnerGrade, 150, NormalModeList, BEGINNER, all )
        ChannelManager:AddChannel('str_14', 300, RookieGrade, 150, NormalModeList, ROOKIE, all )
        ChannelManager:AddChannel('str_19', 300, ExpertGrade, 150, NormalModeList, EXPERT, all )
        ChannelManager:AddChannel('str_24', 300, MasterGrade, 150, NormalModeList, MASTER, all )
        ChannelManager:AddChannel('str_53', 300, AllGrade, 150, DungeonList, DUNGEON, all )
    end
end
--]]



