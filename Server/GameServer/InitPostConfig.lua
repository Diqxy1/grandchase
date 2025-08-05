TRUE = 1
FALSE = 0

-- 우편기능(전체) 사용할지 여부
PostEnable = FALSE

-- 기능제한
SantaPost = FALSE -- 산타시스템 우편으로 사용하는가?
NewPostAlarm = TRUE -- 우편 도착시, 실시간으로 알려줄 것인가?
UserSendPost = FALSE -- 유저 우편보내기 기능 사용할 것인가?
UserPostItem = FALSE -- 유저 우편보내기시 아이템 첨부 사용할 것인가?
UserPostGP = FALSE -- 유저 우편보내기시 GP 첨부 사용할 것인가?

-- 시스템설정
CheckNewPostTickGap = 1000 * 60 * 3 -- 접속중인 유저의 새 우편 DB에서 받아오는 주기(ms단위)
OnePageLetterCount = 5 -- 1페이지당 보여주는 우편 개수
NewLetterTitleCount = 5 -- 한번에 보여주는 신규우편 개수

-- 우편설정
TitleLength = 20 -- 우편 제목 길이제한
MessageLength = 200 -- 우편 내용 길이제한
LetterCost = 1000 -- 우편 기본 수수료
fGPCostRatio = 0.05 -- GP첨부시 첨부GP에 대한 수수료 비율
fItemCostRatio = 0.05 -- 아이템첨부시 첨부아이템에 대한 수수료 비율
SystemPostStorageTime = 30 -- 시스템우편 유통기한 (일단위,-1:영구보관)
UserPostStorageTime = -1 -- 유저우편 유통기한 (일단위,-1:영구보관)

-- 첨부할 수 없는 ItemID 리스트 (PC방 아이템은 코드상에서 첨부차단)
NotPostItemID = {
	--[[ (예제)
	605280, -- 일반화분
	605260, -- 씨앗
	--]]
}
