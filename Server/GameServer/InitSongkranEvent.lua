TRUE = 1
FALSE = 0

BeginTime = { 2012, 03, 13, 00 }-- Y, M, D, H �̺�Ʈ ���� �Ⱓ. Y, M, D �� �ϳ��� 0���� ������ ũ���� ���ϴ�. üũ�� ������ ����...
EndTime = { 2012, 05, 01, 00 }	-- Y, M, D, H �̺�Ʈ ���� �Ⱓ.
Version = 1						-- �̺�Ʈ ����
MaxGrade = 6			 		-- �ִ� ��� �Ѱ�ġ
OneTimeMaxScore = 30			-- ����ź �ѹ��� ȹ���� �� �ִ� �ִ� ���ھ� ( ��ŷ ������ )
PlayerMoveSpeed = 3.0			-- ���� ĳ���� �̵� �ӵ�
WaterBombItem = 8880			-- ����ź ItemID
WaterBombFylingSpeed = 10.0		-- ����ź ���ư��� �ӵ� ( �� ���� )
WaterBombRange = 20.0			-- ����ź ���� ����
WaterBombSupplyInterval = 1 	-- ����ź ���� �ð� �� ( �� ���� )
WaterBombSupplyDuration = 5		-- ����ź ���� ����
DailyMaxSupplyCount = 30		-- ����ź �Ϸ� �ִ� ���� ���� ���� ����

-- Ÿ�� Ÿ��
--	CHAR_ELESIS = 0,
--	CHAR_ZERO	= 1,
--	CHAR_LAS	= 2,
--	CHAR_ARME	= 3,
--	CHAR_REY	= 4,
--	CHAR_SIZE	= 5,

TargetInfoList = 
{
	-- { Ÿ�� Ÿ��, Ÿ�� �̵� �ӵ�, Ÿ�� ����, �ڷ���Ʈ �ֱ�, ��ź �°� ���� �Ǵ� �ð� },
	{ CharType = 0, MoveSpeed = 2.0, Score = 1, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 1, MoveSpeed = 1.0, Score = 2, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 2, MoveSpeed = 4.0, Score = 3, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 3, MoveSpeed = 1.0, Score = 4, TelpoPeriod = 3.0, ResetTime = 1.0 },
	{ CharType = 4, MoveSpeed = 1.0, Score = 5, TelpoPeriod = 3.0, ResetTime = 1.0 },

}

GradeRewardList = 
{
	-- { ���, �����EndPoint, SpecialReward, Reward = { ItemID, Duration, Period, .., }
	{ Grade = 1, GradeEndScore = 40, SpecialReward = FALSE, Reward = { 395110, -1, -1, }, },
	{ Grade = 2, GradeEndScore = 70, SpecialReward = FALSE, Reward = { 395100, -1, -1, }, },
	{ Grade = 3, GradeEndScore = 100, SpecialReward = TRUE, Reward = { 395090, -1, -1, }, },
	{ Grade = 4, GradeEndScore = 150, SpecialReward = FALSE, Reward = { 395080, -1, -1, }, },
	{ Grade = 5, GradeEndScore = 200, SpecialReward = FALSE, Reward = { 395070, -1, -1, }, },
	{ Grade = 6, GradeEndScore = 270, SpecialReward = TRUE, Reward = { 394350, -1, -1, }, }, 
}
