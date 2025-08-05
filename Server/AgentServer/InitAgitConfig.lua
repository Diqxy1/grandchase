MaxCashingAgitInfo = 2000 -- 최대 캐싱할 수 있는 아지트 정보수

MaxAgitNameLength = 12 -- 아지트명 최대길이(차후 바이트)
MaxProfileLength = 50 -- 최대 프로필 길이
MaxUserCount = 20 -- 아지트 입장가능한 최대 인원
MaxPasswordLength = 8 -- 최대 암호 길이
MaxInvenSize = 100 -- 아지트오브젝트 인벤토리(창고랑 다름)
SellRewardFlowerCount = 1 -- 오브젝트 아이템 판매시, 보상 꽃 개수
MaxFavoriteCount = 20 -- 최대 즐겨찾기 개수제한
ReserveRoomID = 500 -- 방번호 예약 개수(초기값,증가치)
BuildPageTick = 1000 * 3 -- 페이지 빌드 텀(ms)
AgitCountOnePage = 11 -- 1페이지당 디스플레이되는 아지트 개수

BrownieID = { 605570, } -- 도우미ID

-- 튜토리얼용 아이템 지급(1회)
TutorialItem = {
	-- ItemID, Count, Period
	{ 605570, -1, -1 }, -- 플로라(도우미)
	-- { 605260, 5, -1 }, --씨앗
	{ 605630, -1, -1 }, -- 나무 식탁
	{ 605650, -1, -1 }, -- 나무 의자
}

RankDBUpdateHour = 8 -- DB에서 정보 퍼오는 시간(시)
MaxGuestbookMsgLenth = 60 -- 낙서장 최대 메세지 길이
GuestbookWriteTerm = 1000 * 10 -- 낙서장 글쓰기 주기 (ms)
GuestBookPageSize = 100 -- 클라이언트에 전달되는 낙서장 글 개수
GuestbookWritePerDay = 3 -- 일일 낙서장 글작성 제한

TrainingItemID = 605300 -- 훈련소 ItemID
TrainingMaterialItemID = 605320 -- 장작 ItemID
TrainingExpRatio = 3 -- 훈련소 장작1개당 경험치 배율
TrainingMaterialRegenTerm = 5 -- 장작 추가되는 시간 (분)
TrainingMaterialLimit = 10 -- 최대 쌓이는 장작개수

-- 캐쉬 아이템이나, 아지트상점에서 구입할 수 없는 배치 오브젝트는 여기에 등록.
ObjectItemID = {
	605300, -- 훈련소
	605310, -- 낙서장
}

CharacterOpenCost = 1 -- 캐릭터배치 오픈시 필요한 꽃의 개수
CharacterSpeechLength = 30 -- 배치캐릭터 대사 글자제한

-- 되팔기 안되는 오브젝트 ItemID
NotResellObjectItemID = {
    605280, -- 일반화분
	605260, -- 씨앗
	605270, -- 꽃
}

-- 인벤토리 보유량 제한
ObjectItemInvenLimit = {
	-- ItemID, 최대보유량(1이상)
    { 605290, 2 }, -- 별사탕 금장식 화분 2개 
    { 606260, 5 }, -- 하얀 작은 구름 5개 
    { 606270, 5 }, -- 하얀 중간 구름 5개
    { 667900, 1 }, -- 깃발 1개
    { 667910, 1 }, -- 주사위 1개
    { 605320, 900 }, -- 나무토막 900개
}
