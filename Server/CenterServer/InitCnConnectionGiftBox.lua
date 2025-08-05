--[[
InitCnConnectionGiftBox.lua
�߱� ��û���� ���ߵ� ���ӽ� �������� �����ϴ� �ý���(��Ī �߱� �������� �ý���) ��ũ��Ʈ�Դϴ�
��� 1) ù ĳ���� ������ ���� ���� ���� ����
��� 2) �ű� �������� ���ӽð��� ī��Ʈ�Ͽ� ������ ����
��� 3) �̺�Ʈ �Ⱓ���� ������ �������� ���ӽð��� ī��Ʈ�Ͽ� ������ ����

�۾���: �Լ���, ������, ���翵
--]]

TRUE = 1
FALSE = 0

-- ��� 1)
UseGiftFirstCharacter = TRUE
GiftFirstCharacter = {
    { CharType = 0, GiftItem = { 1280500, 1, -1, }, },
    { CharType = 1, GiftItem = { 1280500, 1, -1, }, },
    { CharType = 2, GiftItem = { 1280500, 1, -1, }, },
	{ CharType = 3, GiftItem = { 1280500, 1, -1, }, },
    { CharType = 4, GiftItem = { 1280500, 1, -1, }, },
	{ CharType = 5, GiftItem = { 1280500, 1, -1, }, },
    { CharType = 6, GiftItem = { 1280500, 1, -1, }, },
	{ CharType = 7, GiftItem = { 1280500, 1, -1, }, },
    
}


-- ��� 2)
UseGiftNewUser = TRUE
DecideBeginTimeNewUser = { 2013, 09, 24, 00, } -- YYYY, MM, DD, HH
NewUserEffectDuration = 30 -- days
GiftNewUser = {    -- ������ ������� �ܰ躰 �����մϴ�
    {
        GoalTime = 1, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287050, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287150, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287250, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287350, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287450, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287550, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287650, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287750, 1, -1, }, },
        },
    },
    {
        GoalTime = 2, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287060, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287160, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287260, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287360, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287460, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287560, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287660, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287760, 1, -1, }, },
        },
    },
	{
        GoalTime = 3, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287070, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287170, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287270, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287370, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287470, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287570, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287670, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287770, 1, -1, }, },
        },
    },
	{
        GoalTime = 4, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1280630, 1, -1, }, },
			{ CharType = 1, GiftItem = { 1280630, 1, -1, }, },
			{ CharType = 2, GiftItem = { 1280630, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1280630, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1280630, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1280630, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1280630, 1, -1, }, },
            { CharType = 7, GiftItem = { 1280630, 1, -1, }, },         
        },
    },
	{
        GoalTime = 5, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287080, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287180, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287280, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287380, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287480, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287580, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287680, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287780, 1, -1, }, },			
        },
    },
	{
        GoalTime = 8, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287090, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287190, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287290, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287390, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287490, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287590, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287690, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287790, 1, -1, }, },			
        },
    },
	{
        GoalTime = 10, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287100, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287200, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287300, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287400, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287500, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287600, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287700, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287800, 1, -1, }, },			
        },
    },
	{
        GoalTime = 15, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1280670, 1, -1, }, },
            { CharType = 1, GiftItem = { 1280670, 1, -1, }, },
            { CharType = 2, GiftItem = { 1280670, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1280670, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1280670, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1280670, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1280670, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1280670, 1, -1, }, },			
        },
    },
	{
        GoalTime = 20, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1280680, 1, -1, }, },
            { CharType = 1, GiftItem = { 1280680, 1, -1, }, },
            { CharType = 2, GiftItem = { 1280680, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1280680, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1280680, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1280680, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1280680, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1280680, 1, -1, }, },
        },
    },
	{
        GoalTime = 30, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287110, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287210, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287310, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287410, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287510, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287610, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287710, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287810, 1, -1, }, },			
        },
    },
	{
        GoalTime = 45, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 1287120, 1, -1, }, },
            { CharType = 1, GiftItem = { 1287220, 1, -1, }, },
            { CharType = 2, GiftItem = { 1287320, 1, -1, }, },
			{ CharType = 3, GiftItem = { 1287420, 1, -1, }, },
			{ CharType = 4, GiftItem = { 1287520, 1, -1, }, },
			{ CharType = 5, GiftItem = { 1287620, 1, -1, }, },
			{ CharType = 6, GiftItem = { 1287720, 1, -1, }, },
			{ CharType = 7, GiftItem = { 1287820, 1, -1, }, },			
        },
    },
	
}


-- ��� 3)
Version = 1
EventBeginTime = { 2013, 09, 01, 00, } -- YYYY, MM, DD, HH
EventEndTime = { 2014, 09, 01, 00, } -- YYYY, MM, DD, HH
GiftEventUser = {    -- ������ ������� �ܰ躰 �����մϴ�
    {
        GoalTime = 1, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 8880, 1, -1, }, },
            { CharType = 1, GiftItem = { 8880, 1, -1, }, },
            { CharType = 2, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 3, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 4, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 5, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 6, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 7, GiftItem = { 8880, 1, -1, }, },
        },
    },
    {
        GoalTime = 2, -- minutes
        CharacterGiftItem = {
            { CharType = 0, GiftItem = { 8880, 1, -1, }, },
            { CharType = 1, GiftItem = { 8880, 1, -1, }, },
            { CharType = 2, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 3, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 4, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 5, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 6, GiftItem = { 8880, 1, -1, }, },
			{ CharType = 7, GiftItem = { 8880, 1, -1, }, },
        },
    },
}
