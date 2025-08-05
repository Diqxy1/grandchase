-- 60 * 1000 = 1분
CheckDataTickGap 	 = 60 * 1000 	  -- DB 데이터 변경 확인 주기 (다른 이벤트와 동일한 Global Value 확인)
DataUpdateTickGap 	 = 60 * 1000 * 30 -- 소셜커머스 전체 데이터 갱신 주기(공동구매 성공여부 갱신)
GetRewardListTickGap = 60 * 1000 * 30 -- 공동구매 성공 한 아이템을 구매 한 유저 목록 얻는 주기
GetUserCount 		 = 900	 		  -- 한번에 얻어 올 공동구매 성공 유저 수