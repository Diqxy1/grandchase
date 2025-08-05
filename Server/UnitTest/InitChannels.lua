
BATTLE              =   0
BATTLE_UNLIMIT      =   1
NORMAL              =   2
GUILD               =   3
INDIGO              =   4
QZERO               =   5

UNLIMITED           =   6  --  ���� ����Ʈ ��� �Ҽ� �ְ� ���� ������ ���� ä��.
BEGINNER            =   7  --  �� �����̱���  ~ �� �����̱���
ROOKIE              =   8  --  �� �����̱���  ~ �� �뺴����
EXPERT              =   9  --  �� �뺴����    ~ �� �ո�����
MASTER              =   10  --  �� �ո�����    ~ �� ���Ǳ���

ONE_ON_ONE_ROOKIE    =   11
ONE_ON_ONE_EXPERT    =   12
ONE_ON_ONE_MASTER    =   13


ChannelGradeList = {
    { 1,  0,  5 },		-- Grade = 1, BeginLv =  0, EndLv =  5	
    { 2,  6,  12 },
    { 3,  13,  20 },
    { 4,  21,  26 },
    { 5,  27,  35 },
    { 6,  36,  45 },
    { 7,  46,  55 },
    { 8,  56,  75 },
}

ChannelManager = GetChannelManager()
ChannelManager:SetBroadCastingTime( 10000, 2000, 10000 ) -- Channel, Room, User
SetGuildBroadCastingTime( 7000, 60000 )                 -- Member, DB Sync.

ChannelManager:SetGrade( ChannelGradeList )
ChannelManager:SetChannelLobby( 5000 )
--ChannelManager:ModifyChannelLobby( MaxUser )
--( ID, str, MaxUser, Grade, RoomNum, ModeList, Type )

IndigoModeList = {28,}
BattleModeList = {0, 1, 8, 9 }
NormalModeList = {4, 6, 7, 10, 11, 12, 13, 14, 15, 27 }
EveryModeList  = {}
QuestZeroList  = {26, }
GuildModeList  = {2,}
OneOnOneModeList  = {34,}

if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_INDIGO ) ) then -- Indigo

    ChannelManager:AddChannel( 'str_32', 400, 0, 150, IndigoModeList, INDIGO )
    ChannelManager:AddChannel( 'str_33', 400, 0, 400, QuestZeroList, QZERO )

else if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_GUILD )) then -- Guild

    ChannelManager:AddChannel('str_29', 300, 0, 150, GuildModeList, GUILD )--��� 1
    ChannelManager:AddChannel('str_30', 300, 0, 150, GuildModeList, GUILD )--��� 2
    ChannelManager:AddChannel('str_31', 300, 0, 150, GuildModeList, GUILD )--��� 3
    ChannelManager:AddChannel('str_01', 300, 0, 150, EveryModeList, UNLIMITED )--�������� 
    ChannelManager:AddChannel('str_02', 300, 0, 150, EveryModeList, UNLIMITED )--�������� 
    ChannelManager:AddChannel('str_03', 300, 0, 150, EveryModeList, UNLIMITED )--�������� 
    ChannelManager:AddChannel('str_04', 300, 0, 150, EveryModeList, UNLIMITED )--�������� 

    ChannelManager:AddChannel('str_09', 300, 0, 150, EveryModeList, BEGINNER )--
    ChannelManager:AddChannel('str_14', 300, 0, 150, EveryModeList, ROOKIE   )--
    ChannelManager:AddChannel('str_19', 300, 0, 150, EveryModeList, EXPERT   )--
    ChannelManager:AddChannel('str_24', 300, 0, 150, EveryModeList, MASTER   )--

    ChannelManager:AddChannel('str_34', 300, 0, 150, OneOnOneModeList, ONE_ON_ONE_ROOKIE )--
    ChannelManager:AddChannel('str_35', 300, 0, 150, OneOnOneModeList, ONE_ON_ONE_EXPERT )--
    ChannelManager:AddChannel('str_36', 300, 0, 150, OneOnOneModeList, ONE_ON_ONE_MASTER )--

    ChannelManager:AddChannel('str_24', 400, 0, 400, QuestZeroList, QZERO )

    else -- Normal
        ChannelManager:AddChannel('str_01', 300, 0, 150, EveryModeList, UNLIMITED )--��������
        ChannelManager:AddChannel('str_02', 300, 0, 150, EveryModeList, UNLIMITED )--��������
        ChannelManager:AddChannel('str_03', 300, 0, 150, EveryModeList, UNLIMITED )--��������
       

        ChannelManager:AddChannel('str_09', 300, 0, 150, EveryModeList, BEGINNER )--
        ChannelManager:AddChannel('str_10', 300, 0, 150, EveryModeList, BEGINNER )--
        ChannelManager:AddChannel('str_11', 300, 0, 150, EveryModeList, BEGINNER )--
        ChannelManager:AddChannel('str_12', 300, 0, 150, EveryModeList, BEGINNER )--
        ChannelManager:AddChannel('str_13', 300, 0, 150, EveryModeList, BEGINNER )--

        ChannelManager:AddChannel('str_14', 300, 0, 150, EveryModeList, ROOKIE )--
        ChannelManager:AddChannel('str_15', 300, 0, 150, EveryModeList, ROOKIE )--
        ChannelManager:AddChannel('str_16', 300, 0, 150, EveryModeList, ROOKIE )--
        ChannelManager:AddChannel('str_17', 300, 0, 150, EveryModeList, ROOKIE )--
        ChannelManager:AddChannel('str_18', 300, 0, 150, EveryModeList, ROOKIE )--

        ChannelManager:AddChannel('str_19', 300, 0, 150, EveryModeList, EXPERT )--
        ChannelManager:AddChannel('str_20', 300, 0, 150, EveryModeList, EXPERT )--
        ChannelManager:AddChannel('str_21', 300, 0, 150, EveryModeList, EXPERT )--
        ChannelManager:AddChannel('str_22', 300, 0, 150, EveryModeList, EXPERT )--

        ChannelManager:AddChannel('str_24', 300, 0, 150, EveryModeList, MASTER )--
        ChannelManager:AddChannel('str_25', 300, 0, 150, EveryModeList, MASTER )--
        
        ChannelManager:AddChannel('str_34', 300, 0, 150, OneOnOneModeList, ONE_ON_ONE_ROOKIE )--
        ChannelManager:AddChannel('str_35', 300, 0, 150, OneOnOneModeList, ONE_ON_ONE_EXPERT )--
        ChannelManager:AddChannel('str_36', 300, 0, 150, OneOnOneModeList, ONE_ON_ONE_MASTER )--

        ChannelManager:AddChannel('str_33', 400, 0, 400, QuestZeroList, QZERO )
    end
end



