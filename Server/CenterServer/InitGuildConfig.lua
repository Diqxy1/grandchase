TRUE = 1
FALSE = 0

IT_PERIOD   = 0 -- 기간제/영구
IT_QUANTITY = 1 -- 수량성 아이템
MarkServerURL = "http://fileboard.netmarble.net:80/BBS/Chase/GuildMark/"
MarkImageExt = { "jpg", "jpeg", "png", }
MarkDefaultName = "defaultmark.jpg"
ImageBuffMinSize = 100
ImageBuffMaxSize = 1024 * 4
-- 길드마크 업로드 아이템(승격시 지급설정)
GuildMarkUploadItemID = 206680
GuildMarkUploadItemFactor = 30
GuildMarkUploadItemType = IT_PERIOD
-- 길드명 변경 아이템
GuildChangeNameItemID = 8880
GuildMarkUploadCashItemID = 0

GuildPageSize = 8 -- 길드1페이지당 길드수
GuildNameMinLength = 1 -- 길드이름최소길이
GuildNameMaxLength = 6 -- 길드이름최대길이
GuildMasterMinLv = 30 -- 길드마스터 최소레벨 조건
GuildURLMinLength = 1 -- URL 최소 입력길이
GuildURLMaxLength = 100 -- URL 최대 입력길이
GuildCreateGP = 10000 -- 길드생성시 소모GP

OfficailGuildMemberNum = 5  -- 공식 길드가 되기위한 맴버수
NewbieGuildRemoveTerm = 30  -- 신생길드 생성후 제거되기 위한 날짜수
KeywordMinLength = 2        -- 최소 검색어 길이
JoinerRemoveTerm = 15       -- 길드가입자 가입 유예기간. 지나면 자동 거절됨

-- http://www.unicode.org/charts/ 이 사이트 참조 하세요.
AllowUnicode = TRUE
Unicode = {
    { tonumber( '0030', 16 ), tonumber( '0039', 16 ), }, -- 숫자
    { tonumber( '0041', 16 ), tonumber( '005A', 16 ), }, -- 알파벳 대문자
    { tonumber( '0061', 16 ), tonumber( '007A', 16 ), }, -- 알파켓 소문자
    { tonumber( '00C0', 16 ), tonumber( '00FF', 16 ), }, -- 라틴-1
-- 한글 Jamo
    { tonumber( 'FFA1', 16 ), tonumber( 'FFBE', 16 ), }, -- 한글
    { tonumber( 'FFC2', 16 ), tonumber( 'FFC7', 16 ), }, -- 한글
    { tonumber( 'FFCA', 16 ), tonumber( 'FFCF', 16 ), }, -- 한글
    { tonumber( 'FFD2', 16 ), tonumber( 'FFD7', 16 ), }, -- 한글
    { tonumber( 'FFDA', 16 ), tonumber( 'FFDC', 16 ), }, -- 한글
    { tonumber( 'FFDA', 16 ), tonumber( 'FFDC', 16 ), }, -- 한글
-- 한글 완성형
    { tonumber( 'AC00', 16 ), tonumber( 'D7A3', 16 ), }, -- 한글
}
GuildInfoClearTerm = 1200000 -- 길드정보 삭제 주기 (20분)
MemberListClearTerm = 600000 -- 맴버리스트 삭제 주기 (10분)
MaxPageSize = 15 -- 최대 길드Page 제한


-- 길드레벨/필요GPoint
GuildLvPoint =
{
    { 0,    0     },
    { 1,    1000  },
    { 2,    4000  },
    { 3,    10000 },
    { 4,    23000 },
    { 5,    42000 },
    { 6,    68000 },
    { 7,    110000 },
    { 8,    160000 },
    { 9,    250000 },
    { 10,   410000 },
}

-- 혜택
NONE                    = 0 -- 없음
EXP_BONUS               = 1 -- 경험치 보너스
STORE_TAB               = 2 -- 상점 탭 이용
RUNE_ENCHANT_BONUS      = 3 -- 룬강화 보너스
ACCESSORY_ENCHANT_BONUS = 4 -- 목걸이강화 보너스
MORE_EQUIP_POTION       = 5 -- 장착포션 증가

-- 길드상점 탭 index
GUILD_TAB_1 = 1
GUILD_TAB_2 = 2
GUILD_TAB_3 = 3

-- 길드레벨/혜택종류/세부혜택
GuildLvAdvantage = {
    { 0,    NONE,                    0 },  -- 없음
    { 1,    EXP_BONUS,               5 },  -- EXP +5%
    { 2,    STORE_TAB,               GUILD_TAB_1 },  -- 회복포션(TabID:1) 상점이용
    { 3,    RUNE_ENCHANT_BONUS,      5 },  -- 룬강화 +5%
    { 4,    EXP_BONUS,               5 },  -- EXP +5% (누적+10%)
    { 5,    ACCESSORY_ENCHANT_BONUS, 5 },  -- 목걸이강화 +5%
    { 6,    STORE_TAB,               GUILD_TAB_2 },  -- 버프포션(TabID:2) 상점이용
    { 7,    MORE_EQUIP_POTION,       2 },  -- 장착포션 +2개
    { 8,    RUNE_ENCHANT_BONUS,      5 },  -- 룬강화 +5% (누적+10%)
    { 9,    ACCESSORY_ENCHANT_BONUS, 5 },  -- 목걸이강화 +5% (누적+10%)
    { 10,   EXP_BONUS,               5 },  -- EXP +5% (누적+15%)
}

-- 길드기여도/적용된는 길드레벨 혜택
ContributePointAdventageLv = {
    { 0,     0 },
    { 200,   1 },
    { 600,   2 },
    { 1200,  3 },
    { 2000,  4 },
    { 3000,  5 },
    { 5000,  6 },
    { 7000,  7 },
    { 9000,  8 },
    { 12000, 9 },
    { 15000, 10 },
}

-- 길드포인트 갱신 주치
GuildUpdateCount = 50 -- 길드포인트 변경 길드 정보 몇개씩 전송할지 Max 변수
ChangeGuildTickGap = 60 * 1000 -- 동접 적은 시간대에 얼마마다 한번씩 변경된 길드 포인트 길드 정보 전달할지 TickGap 변수
