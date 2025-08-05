-- Survey Types ���� �˾� ��Ȳ
GAME_CONNECT = 0 -- �������ӽ�
GAME_CLEAR   = 1 -- ����Ŭ�����
CAHR_LEVELUP = 2 -- ��������

-- ä�θ� Ÿ��
PUBLISHER1 = 1	-- ä��Ÿ�� 1 ( �ݸ���,  �ݹڽ� )
PUBLISHER2 = 2  -- ä��Ÿ�� 2 ( ���Ϸ���, �÷�����ũ )
PUBLISHER3 = 3  -- ä��Ÿ�� 3 ( ����Ʈ.., )

-- Target User Types ���� ���
NEWBIE   = 1	-- �ű� ����
SAVEUSER = 2    -- ���� ����
RETURNER = 3	-- ���� ����

-- ���� �ڵ� ����
EU_EN = 0 -- ���� ����
EU_GE = 1 -- ���Ͼ�
KR_KO = 2 -- �ѱ���
TW_TW = 3 -- �븸��
BR_PT = 4 -- ����Ʈ��
US_EN = 5 -- �̱� ����
PH_EN = 6 -- �ʸ��� ����
TH_TH = 7 -- �±���
ID_ID = 8 -- �ε��׽þƾ�
LA_ES = 9 -- �����Ĵ�


BeginTime = { 2011, 9, 1, 00 }	-- Y, M, D, H �̺�Ʈ ���� �Ⱓ.
EndTime = { 2012, 9, 30, 00 }	-- Y, M, D, H �̺�Ʈ ���� �Ⱓ.
ReturnTime = { 2011, 9, 30, 00 } -- Y, M, D, H ���� ��¥.
TickGap = 1000 * 60			-- ms
Version = 1

LanguageInfo = {
	KR_KO,
}

GameConnectInfo = {
	Type = GAME_CONNECT,
	TypeInfo = {
		{ ChannelType = PUBLISHER1, UserType = NEWBIE, SurveyIDList = {}, },
		{ ChannelType = PUBLISHER1, UserType = SAVEUSER, SurveyIDList = {}, },
		{ ChannelType = PUBLISHER1, UserType = RETURNER, SurveyIDList = {}, },
	}
}

DungeonClearInfo = {
	Type = GAME_CLEAR,
	DungeonInfo = {
		{ DungeonID = 18, Difficult = 0, SurveyIDList = { 1,2,3, }, },
		{ DungeonID = 26, Difficult = 0, SurveyIDList = { 1,2,3, }, },
		{ DungeonID = 46, Difficult = 0, SurveyIDList = { 1,2,3, }, },
		{ DungeonID = 50, Difficult = 0, SurveyIDList = { 1,2,3, }, },
	},
}

LevelUpInfo = {
	Type = CAHR_LEVELUP,
	-- { LevelRange, 
	LevelInfo = {
		{ CharType = 0, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 0, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 0, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 1, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 1, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 1, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 2, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 2, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 2, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 3, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 3, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 3, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 4, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 4, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 4, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 5, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 5, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 5, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 6, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 6, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 6, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 7, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 7, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 7, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 8, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 8, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 8, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 9, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 9, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 9, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 10, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 10, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 10, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 11, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 11, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 11, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 12, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 12, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 12, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
	}
}

SurveyRewardInfo = {
	-- { RewardType, Reward{ ItemID, Duration, Period.., } }
	{ RewardType = GAME_CONNECT, RewardInfo = { 114150, 3, -1, 71440, 3, -1}, },
	{ RewardType = GAME_CLEAR, RewardInfo = { 114150, 3, -1, 71440, 3, -1}, },
	{ RewardType = CAHR_LEVELUP, RewardInfo = { 114150, 3, -1, 71440, 3, -1, 149150, -1, 1 }, },
}
