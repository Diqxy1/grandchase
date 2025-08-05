-- 버전 1씩 올려주세요.
Version = 1

-- 이벤트기간
BeginTime   = { 2010, 12, 4, 0, 1 }   -- Y, M, D, H, M
EndTime     = { 2010, 12, 29, 23, 59 }   -- Y, M, D, H, M

-- 고대의 마법석
NormalKeyItemID = 8880
-- 코우나트의 마력석
SpecialKeyItemID = 12490

-- 키 증가비율 = 아이템 힉득횟수 * IncRate
-- 필요 키 개수 = 올림[ (캐쉬원가 / DivValue) * (1.0 + 키 증가비율) ]
DivValue = 100
IncRate = 0.1

-- DB에서 받아온 행운확률 서버적용시 곱하는 배율값
LuckRatioDBtoServer = 0.00001

-- 마력석 1회사용시 마력석 소모개수
DecKeyCount = 1
