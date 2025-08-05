-- 매칭서버 설정값 스크립트.
TRUE = 1
FALSE = 0

-- 모드 정보.
PVP1vs1 = 0
PVP2vs2 = 1
PVP3vs3 = 2

StartSearchScope = 50 -- 최초 Rating 비율값.
MaxIncreaseScope = 400 -- 최대 증가하는 RP 비율값.
IncreaseScope = 50 -- 증가하는 RP 비율값.
-- 게임서버 InitMatchHelper.lua에서 사용되기 때문에 아래의 2개값이 수정될때에는 게임서버에 값도 변경 필요.

--ModeID 매칭 모드 정보.
--bLimitMatchEnable 최소 설정값을 제어할 수 있도록 ON/OFF 기능 추가.
--LimitMatchGroupNum 최소 설정값 이상의 그룹이 되어야 매칭검색을 시작한다.
LimitMatchTable = 
{
	{ ModeID = PVP1vs1, LimitMatchGroupNum = 15, bLimitMatchEnable = FALSE },
	{ ModeID = PVP2vs2, LimitMatchGroupNum = 7, bLimitMatchEnable = TRUE },
	{ ModeID = PVP3vs3, LimitMatchGroupNum = 15, bLimitMatchEnable = FALSE },
}