TRUE = 1
FALSE = 0

BeginTime = { 2012, 06, 20, 00 }-- Y, M, D, H 이벤트 시작 기간. Y, M, D 중 하나에 0값을 넣으면 크래쉬 납니다. 체크는 하지만 주의...
EndTime = { 2012, 07, 30, 10 }	-- Y, M, D, H 이벤트 종료 기간.
Version = 1						-- 이벤트 버전
MaxGrade = 130			 		-- 최대 등급 한계치
OneTimeMaxScore = 50			-- 물폭탄 한번에 획득할 수 있는 최대 스코어 ( 해킹 방지용 )
PlayerMoveSpeed = 3.0			-- 유저 캐릭터 이동 속도
WaterBombFylingSpeed = 10.0		-- 물폭탄 날아가는 속도 ( 초 단위 )
WaterBombRange = 20.0			-- 물폭탄 폭발 범위
WaterBombSupplyInterval = 10 	-- 물폭탄 지급 시간 텀 ( 분 단위 )
WaterBombItem = 876100			-- 물폭탄 교환 캐쉬 아이템 ItemID
WaterBombSupplyDuration = 5		-- 물폭탄 지급 개수
ExchangeDuration = 1			-- 물폭탄 교환 캐쉬 아이템 사용 시 충전 개수
OneadaySupplyDuration = 10 		-- 접속 시 지급하는 물폭탄 개수
DailyMaxSupplyCount = 150		-- 물폭탄 하루 최대 지급 가능 수량 제한

-- 타겟 타입
--	CHAR_ELESIS = 0, -- 무브
--	CHAR_ZERO	= 1, -- 무브
--	CHAR_LAS	= 2, -- 무브
--	CHAR_ARME	= 3, -- 텔레포트
--	CHAR_REY	= 4, -- 텔레포트
--	CHAR_SIZE	= 5,

TargetInfoList = 
{
	-- { 타겟 타입, 타겟 이동 속도, 타겟 점수, 텔레포트 주기, 폭탄 맞고 리셋 되는 시간 },
	{ CharType = 0, MoveSpeed = 2.0, Score = 5, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 1, MoveSpeed = 1.0, Score = 5, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 2, MoveSpeed = 4.0, Score = 5, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 3, MoveSpeed = 1.0, Score = 5, TelpoPeriod = 2.0, ResetTime = 1.0 },
	{ CharType = 4, MoveSpeed = 1.0, Score = 5, TelpoPeriod = 2.0, ResetTime = 1.0 },

}

GradeRewardList = 
{
	-- { 등급, 등급의EndPoint, SpecialReward, Reward = { ItemID, Duration, Period, .., }
{	Grade	=	1	,	GradeEndScore	=	300		,	SpecialReward	=	TRUE	,	Reward	=	{	102030	,	1	,	-1	,	}	,	}	,
{	Grade	=	2	,	GradeEndScore	=	500		,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	1	,	-1	,	}	,	}	,
{	Grade	=	3	,	GradeEndScore	=	800		,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	1	,	-1	,	}	,	}	,
{	Grade	=	4	,	GradeEndScore	=	1000	,	SpecialReward	=	TRUE	,	Reward	=	{	99600	,	1	,	-1	,	}	,	}	,
{	Grade	=	5	,	GradeEndScore	=	1200	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	1	,	-1	,	}	,	}	,
{	Grade	=	6	,	GradeEndScore	=	1500	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	1	,	-1	,	}	,	}	,
{	Grade	=	7	,	GradeEndScore	=	1700	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	1	,	-1	,	}	,	}	,
{	Grade	=	8	,	GradeEndScore	=	1900	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	1	,	-1	,	}	,	}	,
{	Grade	=	9	,	GradeEndScore	=	2200	,	SpecialReward	=	TRUE	,	Reward	=	{	627320	,	1	,	-1	,	}	,	}	,
{	Grade	=	10	,	GradeEndScore	=	2400	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	1	,	-1	,	}	,	}	,
{	Grade	=	11	,	GradeEndScore	=	2700	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	1	,	-1	,	}	,	}	,
{	Grade	=	12	,	GradeEndScore	=	2900	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	1	,	-1	,	}	,	}	,
{	Grade	=	13	,	GradeEndScore	=	3200	,	SpecialReward	=	TRUE	,	Reward	=	{	627340	,	1	,	-1	,	}	,	}	,
{	Grade	=	14	,	GradeEndScore	=	3500	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	2	,	-1	,	}	,	}	,
{	Grade	=	15	,	GradeEndScore	=	3700	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	2	,	-1	,	}	,	}	,
{	Grade	=	16	,	GradeEndScore	=	4000	,	SpecialReward	=	TRUE	,	Reward	=	{	876440	,	1	,	-1	,	}	,	}	,
{	Grade	=	17	,	GradeEndScore	=	4200	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	2	,	-1	,	}	,	}	,
{	Grade	=	18	,	GradeEndScore	=	4500	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	2	,	-1	,	}	,	}	,
{	Grade	=	19	,	GradeEndScore	=	4800	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	2	,	-1	,	}	,	}	,
{	Grade	=	20	,	GradeEndScore	=	5100	,	SpecialReward	=	TRUE	,	Reward	=	{	102030	,	1	,	-1	,	}	,	}	,
{	Grade	=	21	,	GradeEndScore	=	5300	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	2	,	-1	,	}	,	}	,
{	Grade	=	22	,	GradeEndScore	=	5600	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	2	,	-1	,	}	,	}	,
{	Grade	=	23	,	GradeEndScore	=	5900	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	2	,	-1	,	}	,	}	,
{	Grade	=	24	,	GradeEndScore	=	6200	,	SpecialReward	=	TRUE	,	Reward	=	{	42340	,	-1	,	3	,	}	,	}	,
{	Grade	=	25	,	GradeEndScore	=	6500	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	2	,	-1	,	}	,	}	,
{	Grade	=	26	,	GradeEndScore	=	6800	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	27	,	GradeEndScore	=	7000	,	SpecialReward	=	TRUE	,	Reward	=	{	102030	,	1	,	-1	,	}	,	}	,
{	Grade	=	28	,	GradeEndScore	=	7300	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	29	,	GradeEndScore	=	7600	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	30	,	GradeEndScore	=	7900	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	31	,	GradeEndScore	=	8200	,	SpecialReward	=	TRUE	,	Reward	=	{	627310	,	1	,	-1	,	}	,	}	,
{	Grade	=	32	,	GradeEndScore	=	8500	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	33	,	GradeEndScore	=	8800	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	34	,	GradeEndScore	=	9200	,	SpecialReward	=	TRUE	,	Reward	=	{	627300	,	1	,	-1	,	}	,	}	,
{	Grade	=	35	,	GradeEndScore	=	9500	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	36	,	GradeEndScore	=	9800	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	37	,	GradeEndScore	=	10100	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	38	,	GradeEndScore	=	10400	,	SpecialReward	=	TRUE	,	Reward	=	{	876440	,	1	,	-1	,	}	,	}	,
{	Grade	=	39	,	GradeEndScore	=	10700	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	40	,	GradeEndScore	=	11100	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	41	,	GradeEndScore	=	11400	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	42	,	GradeEndScore	=	11700	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	43	,	GradeEndScore	=	12000	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	44	,	GradeEndScore	=	12400	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	45	,	GradeEndScore	=	12700	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	46	,	GradeEndScore	=	13100	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	47	,	GradeEndScore	=	13400	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	48	,	GradeEndScore	=	13700	,	SpecialReward	=	TRUE	,	Reward	=	{	102030	,	1	,	-1	,	}	,	}	,
{	Grade	=	49	,	GradeEndScore	=	14100	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	50	,	GradeEndScore	=	14400	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	51	,	GradeEndScore	=	14800	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	52	,	GradeEndScore	=	15100	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	53	,	GradeEndScore	=	15500	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	54	,	GradeEndScore	=	15900	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	55	,	GradeEndScore	=	16200	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	56	,	GradeEndScore	=	16600	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	57	,	GradeEndScore	=	16900	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	58	,	GradeEndScore	=	17300	,	SpecialReward	=	TRUE	,	Reward	=	{	876440	,	1	,	-1	,	}	,	}	,
{	Grade	=	59	,	GradeEndScore	=	17700	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	60	,	GradeEndScore	=	18100	,	SpecialReward	=	FALSE	,	Reward	=	{	102030	,	3	,	-1	,	}	,	}	,
{	Grade	=	61	,	GradeEndScore	=	18400	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	62	,	GradeEndScore	=	18800	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	63	,	GradeEndScore	=	19200	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	64	,	GradeEndScore	=	19600	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	65	,	GradeEndScore	=	20000	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	66	,	GradeEndScore	=	20400	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	67	,	GradeEndScore	=	20700	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	68	,	GradeEndScore	=	21100	,	SpecialReward	=	TRUE	,	Reward	=	{	457060	,	2	,	-1	,	}	,	}	,
{	Grade	=	69	,	GradeEndScore	=	21500	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	70	,	GradeEndScore	=	21900	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	71	,	GradeEndScore	=	22300	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	72	,	GradeEndScore	=	22700	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	73	,	GradeEndScore	=	23100	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	74	,	GradeEndScore	=	23600	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	75	,	GradeEndScore	=	24000	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	76	,	GradeEndScore	=	24400	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	77	,	GradeEndScore	=	24800	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	78	,	GradeEndScore	=	25200	,	SpecialReward	=	TRUE	,	Reward	=	{	876460	,	-1	,	-1	,	}	,	}	,
{	Grade	=	79	,	GradeEndScore	=	25600	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	80	,	GradeEndScore	=	26100	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	81	,	GradeEndScore	=	26500	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	82	,	GradeEndScore	=	26900	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	83	,	GradeEndScore	=	27300	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	84	,	GradeEndScore	=	27800	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	85	,	GradeEndScore	=	28200	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	86	,	GradeEndScore	=	28700	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	87	,	GradeEndScore	=	29100	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	88	,	GradeEndScore	=	29500	,	SpecialReward	=	TRUE	,	Reward	=	{	102030	,	1	,	-1	,	}	,	}	,
{	Grade	=	89	,	GradeEndScore	=	30000	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	90	,	GradeEndScore	=	30400	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	91	,	GradeEndScore	=	30900	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	92	,	GradeEndScore	=	31300	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	93	,	GradeEndScore	=	31800	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	94	,	GradeEndScore	=	32300	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	95	,	GradeEndScore	=	32700	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	96	,	GradeEndScore	=	33200	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	97	,	GradeEndScore	=	33600	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	98	,	GradeEndScore	=	34100	,	SpecialReward	=	TRUE	,	Reward	=	{	876440	,	1	,	-1	,	}	,	}	,
{	Grade	=	99	,	GradeEndScore	=	34600	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	100	,	GradeEndScore	=	35100	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	101	,	GradeEndScore	=	35500	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	102	,	GradeEndScore	=	36000	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	103	,	GradeEndScore	=	36500	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	104	,	GradeEndScore	=	37000	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	105	,	GradeEndScore	=	37500	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	106	,	GradeEndScore	=	38000	,	SpecialReward	=	TRUE	,	Reward	=	{	876450	,	1	,	-1	,	}	,	}	,
{	Grade	=	107	,	GradeEndScore	=	38400	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	108	,	GradeEndScore	=	38900	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	109	,	GradeEndScore	=	39400	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	110	,	GradeEndScore	=	39900	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	111	,	GradeEndScore	=	40400	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	112	,	GradeEndScore	=	40900	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	113	,	GradeEndScore	=	41400	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	114	,	GradeEndScore	=	42000	,	SpecialReward	=	TRUE	,	Reward	=	{	876450	,	1	,	-1	,	}	,	}	,
{	Grade	=	115	,	GradeEndScore	=	42500	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	116	,	GradeEndScore	=	43000	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	117	,	GradeEndScore	=	43500	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	118	,	GradeEndScore	=	44000	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	119	,	GradeEndScore	=	44500	,	SpecialReward	=	FALSE	,	Reward	=	{	558990	,	3	,	-1	,	}	,	}	,
{	Grade	=	120	,	GradeEndScore	=	45100	,	SpecialReward	=	FALSE	,	Reward	=	{	559000	,	3	,	-1	,	}	,	}	,
{	Grade	=	121	,	GradeEndScore	=	45600	,	SpecialReward	=	FALSE	,	Reward	=	{	559010	,	3	,	-1	,	}	,	}	,
{	Grade	=	122	,	GradeEndScore	=	46100	,	SpecialReward	=	TRUE	,	Reward	=	{	876450	,	1	,	-1	,	}	,	}	,
{	Grade	=	123	,	GradeEndScore	=	46600	,	SpecialReward	=	FALSE	,	Reward	=	{	559020	,	3	,	-1	,	}	,	}	,
{	Grade	=	124	,	GradeEndScore	=	47200	,	SpecialReward	=	FALSE	,	Reward	=	{	559030	,	3	,	-1	,	}	,	}	,
{	Grade	=	125	,	GradeEndScore	=	47700	,	SpecialReward	=	FALSE	,	Reward	=	{	559040	,	3	,	-1	,	}	,	}	,
{	Grade	=	126	,	GradeEndScore	=	48300	,	SpecialReward	=	FALSE	,	Reward	=	{	559050	,	3	,	-1	,	}	,	}	,
{	Grade	=	127	,	GradeEndScore	=	48800	,	SpecialReward	=	FALSE	,	Reward	=	{	559060	,	3	,	-1	,	}	,	}	,
{	Grade	=	128	,	GradeEndScore	=	49300	,	SpecialReward	=	FALSE	,	Reward	=	{	559070	,	3	,	-1	,	}	,	}	,
{	Grade	=	129	,	GradeEndScore	=	49900	,	SpecialReward	=	FALSE	,	Reward	=	{	558980	,	3	,	-1	,	}	,	}	,
{	Grade	=	130	,	GradeEndScore	=	50400	,	SpecialReward	=	TRUE	,	Reward	=	{	876450	,	1	,	-1	,	}	,	}	,
}
