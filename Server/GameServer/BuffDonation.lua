-- 발렌타인 때 쓰인 기부 이벤트
-- 기부 수량만큼 교환받는 아이템이 있다
-- 일정 수치 달성 시에 버프를 걸라는 통지를 하고,
-- 일정 기부 수치를 감소시키는 이벤트
-- 센터서버의 CnBuffDonation.lua 에서 나머지 설정이 가능하다

FALSE = 0
TRUE = 1

BeginTime       			  = { 2013, 01, 25, 08 }   -- Y, M, D, H
EndTime         		      = { 2013, 02, 25, 10 }   -- Y, M, D, H
DonationItem	    		  = 102030				   -- 기부에 사용 되는 아이템
ActiveBuffPoint				  = 2000				   -- 버프를 걸어 주는 기준 (클라 전달용 데이터/센터랑 같은 숫자 사용)
RecoveryChance				  = 20					   -- 스킬 사용 시 HP/MP회복 확률 0~100 (클라 전달용 데이터)
TypeNumber					  = 1					   -- [기획쪽 세팅 필요 없음] 기간과 type으로 데이터를 구분한다 (센터랑 같은 숫자 사용)
-- 기부 하고 교환 받을 아이템 { ItemID, Duration, Period }
-- Duration 0으로 입력하면 기부 수량만큼 지급
RewardItem 					  = { 8880, 0, -1, }

-- 개별 버프 시스템 사용할 것인지 (기부를 한번이라도 한 유저에 대해 버프 거는 것)
-- 기획서에는 나와 있지 않으나 나중에 전체 버프 이벤트를 대비해서 추가 해 둠
UseIndividualBuffSystem		  = TRUE

BuffStartNotifyString = { "[화령의 축복] 효과가 시작되었습니다! 모두 행복한 화령의 축제를 즐기세요!", }