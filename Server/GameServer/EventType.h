#pragma once

enum UserEventType
{
    UET_LUNA_NEW_YEAR                   = 0, // 세벳돈 이벤트
    UET_IP_ADVANTAGE                    = 1, // PC방 혜택 오늘 받았는지
    UET_EVENT_INFO_CHACK                = 2, // 이벤트 내용 출력 여부.
    UET_DICE_PLAY_TWICE                 = 4,
    UET_CHANNELING_EVENT                = 5, // 채널링 유저 아이템 지급 이벤트.
    UET_TIME_DROP_COUNT                 = 6, // 시간별 아이템 드랍(발렌타인데이)
    UET_TIME_DROP_ACC_TIME              = 7, // TimeDrop AccPlayTime(발렌타인데이)
    UET_CHANNELING_DAILY_EVENT          = 8, // 채널링 유저 일별 아이템 지급 이벤트
    UET_CHANNELING_ONE_EVENT            = 9, // 채널링 유저 이벤트 기간에 아이템 한번만 지급 이벤트
    UET_CHANNELING_NEWUSER_EVENT        = 10, // 채널링 신규 유저 이벤트 기간에 아이템 한번만 지급 이벤트
    UET_DICE_EVENT                      = 11, // 주사위 이벤트.
    UDT_GAWIBAWIBO_EVENT                = 12, // 가위바위보 이벤트.
    UDT_NICKNAME_EVENT                  = 13, // 닉네임 변경 켐페인
    UDT_USERAUTH_VIRTUALCASH_EVENT      = 14, // 남미 미국 유저 가상 캐시 지급 이벤트.
    UDT_OPEN_CALENDAR_BONUS_GCPOINT     = 15, // 달력 오픈시, 일일1회 보너스 GCPoint 지급
    UDT_TODAY_SEED_DROP_COUNT           = 16, // 일일 획득한 씨앗 개수
    UDT_TODAY_SEED_DROP_INIT            = 17, // 일일 씨앗 초기화 받았는지.
    UDT_TODAY_USE_FRUIT                 = 18, // 그날 요정의나무 열매 먹었는지.
    UDT_AGIT_TUTORAIL_DONE              = 19, // 아지트 튜토리얼 진행여부

    UET_USER_SQUARE_STAT_TIME           = 21, // 유저의 광장 접속해 있던 시간정보.
    UET_GWC_EVENT                       = 22, // GWC이벤트 보상 아이템 지급.
    UET_ECLIPSE_COLLECT_EVENT           = 23, // 고서 모으기 이벤트.
    UET_ECLIPSE_PLOT_PROGRESS           = 24, // 개기일식의 음모(미니게임)
    UET_ECLIPSE_PLOT_FINAL_REWARD       = 25, // 개기일식 최종 보상(미니게임)
    UDT_GACHA_LEVEL_NOTICE              = 26, // 가챠 레벨 공지 읽었는지
    UET_SONGKRAN                        = 27, // 송크란 이벤트. 새로운 sp에 맞게 이벤트 타입 구분용으로 사용.
    UET_GET_NEW_CHAR                    = 28, // 신캐릭터 획득 팝업 . 아신
    UET_GET_NEW_CHAR_ATTEND             = 29, // 출석 n 회를 통한 신캐릭터 카드 획득.
    UET_CONNECT_EVENT_REWARD            = 30, // 접속 보상 이벤트에서 몇 번 접속을 했는지
    UET_RECOM_HELPER_EVENT              = 31, // 반갑다 친구야 이벤트
    UET_NEWUSER_TUTORIAL                = 32, // 신규 유저 튜토리얼
    UET_JUMPING_CHARACTER               = 33, // 점핑 캐릭터
    UET_TONG_DONATION_1                 = 34, // 통합 기부 이벤트 누적(NPC 1)
    UET_TONG_DONATION_2                 = 35, // 통합 기부 이벤트 누적(NPC 2)
    UET_TONG_DONATION_3                 = 36, // 통합 기부 이벤트 누적(NPC 3)
    UET_TONG_DONATION_4                 = 37, // 통합 기부 이벤트 누적(NPC 4)
    UET_TONG_DONATION_5                 = 38, // 통합 기부 이벤트 누적(NPC 5)
    UET_TONG_DONATION_6                 = 39, // 통합 기부 이벤트 누적(NPC 6)
    UET_TONG_DONATION_7                 = 40, // 통합 기부 이벤트 누적(NPC 7)
    UET_TONG_DONATION_8                 = 41, // 통합 기부 이벤트 누적(NPC 8)
    UET_TONG_DONATION_9                 = 42, // 통합 기부 이벤트 누적(NPC 9)
    UET_TONG_DONATION_10                = 43, // 통합 기부 이벤트 누적(NPC 10)
    UET_GET_CHANGE_GUILDMARK_ITEM       = 44, // 길드 승급시 길드 마크 변경권 받았는지 여부(0:이미 받음 또는 대상아님, 1: 받아야 함)
    UET_CONNECTION_GIFTBOX_NEW_USER     = 45, // 접속 선물상자 신규유저 대상 여부
    UET_BONUS_POINT_REFILL              = 46, // 기본 보너스 포인트 리필여부(캐릭터별)

    UET_MONSTERCARD_MIGRATION           = 251, // 20130716 몬스터 카드 마이그레이션
    UET_STRENGTH_MIGRATION              = 255, // 20110818 강화 마이그레이션

    UET_MAX,
};
