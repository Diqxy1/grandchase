TRUE = 1
FALSE = 0

-- 개기일식 고서 모으기 이벤트
BeginTime   = { 2012, 01, 29, 09 }   -- Y, M, D, H
EndTime     = { 2012, 02, 05, 10 }   -- Y, M, D, H
EclipseCollectVersion = 1	-- 해당 이벤트 재사용시 해당 버전을 1씩 높이는 작업 필요.
EclipseCollectItemList = { 36560,36570,36580,36590,36600,38240,38250,38260,38270 } -- 고서 ItemList


-- { ItemID, Duration, Period, .., }
RewardItemList = { 8800,-1,-1, }