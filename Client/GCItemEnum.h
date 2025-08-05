/// @file   : GCItemEnum.h
/// @since  : 2004-10-08 오후 7:55:25
/// @author : pch413@kogstudios.com
/// @date   : 2004-12-11 오전 10:14:54 크리스탈 상품 플래그 추가

#ifndef _GCITEMENUM_H_
#define _GCITEMENUM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// dwExtra1
enum EItemCharFlag
{
	ITEM_CHAR0 = 0x00000001,   // 기사 - 엘리시스      ( DB value = 0 )
	ITEM_CHAR1 = 0x00000002,   // 궁수 - 리르          ( DB value = 1 )
	ITEM_CHAR2 = 0x00000004,   // 법사 - 아르메        ( DB value = 2 )
	ITEM_CHAR3 = 0x00000008,   // 도적 - 라스			( DB value = 4 )
};

// dwExtra1
enum EItemAttributeFlag1
{
	ITEM_HEAD = 0x00000010,   // 머리                 ( DB value = 0 )
	ITEM_BODY1 = 0x00000020,   // 상의                 ( DB value = 1 )
	ITEM_BODY2 = 0x00000040,   // 하의                 ( DB value = 2 )
	ITEM_WEAP = 0x00000080,   // 무기                 ( DB value = 3 )

	ITEM_HAND_ACC = 0x00000100,   // 손 악세서리          ( DB value = 16 )
	ITEM_BODY1_ACC = 0x00000200,   // 상의 악세서리        ( DB value = 12 )
	ITEM_BACKCLOTH_ACC = 0x00000400,   // 망토 악세서리        ( DB value = 13 )
	ITEM_BODY2_ACC = 0x00000800,   // 하의 악세서리        ( DB value = 14 )

	ITEM_HAND = 0x00001000,   // 장갑                 ( DB value = 8 )
	ITEM_SHOOE = 0x00002000,   // 신발                 ( DB value = 9 )
	ITEM_NOT_SALE = 0x00004000,   // 판매 여부
	ITEM_JOB_LV2 = 0x00008000,   // 전직 무기

	ITEM_PATTERN_A = 0x00010000,   // 패턴A
	ITEM_PATTERN_B = 0x00020000,   // 패턴B
	ITEM_EXTRA1_RESERVED = 0x00040000,   // Reserved flag
	ITEM_SHOW_UNIQUE_ID = 0x00080000,   // 아이템 유니크 아이디 보여주기

	ITEM_SET = 0x00100000,   // 세트 아이템
	ITEM_FACEUP_ACC = 0x00200000,   // 눈 위 악세서리   (ex:천사링)      ( DB value = 10 )
	ITEM_FACEDOWN_ACC = 0x00400000,   // 눈 아래 악세서리 (ex:안경,마스크) ( DB value = 11 )
	ITEM_ARM_ACC = 0x00800000,   // 팔 악세서리      (ex:방패 종류)   ( DB value = 15 )

	ITEM_SHOOE_ACC = 0x01000000,   // 발 악세서리                       ( DB value = 17 )
	ITEM_NEWLAYER_ACC = 0x02000000,   // 특별 악세서리    (ex:목걸이 종류) ( DB value = 18 )
	ITEM_NEW_GOODS = 0x04000000,   // 신상품
	ITEM_SCROLL_FOR_JOB_LV2 = 0x08000000,   // 전직 증명서

	ITEM_UNABLE_EQUIP = 0x10000000,   // 장착 불능 아이템
	ITEM_2D = 0x20000000,   // 2D 이미지 아이템
	ITEM_ENCHANT = 0x40000000,   // 강화 아이템
	ITEM_MAGIC_PAPER = 0x80000000,   // 마법서 아이템
};

// dwExtra2
enum EItemEffect
{
	ITM_EFF_HPUP = 0,   // 회복아이템효력업 - 초콜릿
	ITM_EFF_SUPER = 1,   // 무적             - Reserved
	ITM_EFF_HASTE = 2,   // 헤이스트 무효    - 절대강자 반지
	ITM_EFF_ROLL = 3,   // 롤링 액션        - 타이거 반지

	ITM_EFF_RENASS = 4,   // 일정시간생명력업 - 레나스
	ITM_EFF_JINRENASS = 5,   // 일정시간생명력업 - 진레나스
	ITM_EFF_HALFCOUNT = 6,   // 카운터공격 반감  - 왕립 반지
	ITM_EFF_NOCOUNT = 7,   // 카운터공격 무효  - 캐슬킹 반지, 흑반지

	ITM_EFF_2X_EXP = 8,   // 경험치 2배 획득  - 거북이 등껍질
	ITM_EFF_BERSERK = 9,   // 공격업, 방어다운 - 광전사의 반지
	ITM_EFF_JINBERSERK = 10,   // 공격업, 방어다운 - 진광전사의 반지
	ITM_EFF_GORGOS = 11,   // 공격력 상승      - 고르고스 무기

	ITM_EFF_ACCUM_1ST = 12,   // 방어력 상승(8%)  - 누적 1위 길드 방패
	ITM_EFF_ACCUM_2ND = 13,   // 방어력 상승(6%)  - 누적 2위 길드 방패
	ITM_EFF_ACCUM_3RD = 14,   // 방어력 상승(5%)  - 누적 3위 길드 방패
	ITM_EFF_WEEKN_1ST = 15,   // 방어력 상승(8%)  - 주간 1위 길드 방패

	ITM_EFF_SANTA = 16,   // 공격+1.5%/방어+3.5%/생명+2.5% - 산타복장
	ITM_EFF_QUEST_REBIRTH = 17,   // 퀘스트에서 부활  - 수호의 반지
	ITM_EFF_GAIKOZ = 18,   // 방어력 상승      - 가이코즈 방패
	ITM_EFF_PATUSEI_LANCE = 19,   // 레벨 8 미만 생명력 6% 상승 8이상 생명력 10%  - 파투세이 창

	ITM_EFF_PEACOCK_WHING = 20,   // 방어력 8%상승    - 공작깃털 날개

	ITM_EFF_10COUNT = 21,	//  카운터공격 10%감소  - 영웅의 반지
	ITM_EFF_15COUNT = 22,
	ITM_EFF_20COUNT = 23,
	ITM_EFF_30COUNT = 24,

	ITM_EFF_CARNIVAL = 25,   // 공격력 5% 상승 20%상승  생명력 20%상승
	ITM_EFF_AP_DP_HP_10_PER = 26,	// 공방생 10% 업
	ITE_EFF_DEFENCE_COUNT = 27,	// 카운터공격 디펜스율( 소스트에서 셋팅한 값을 이용함)
	ITM_EFF_AP_DP_HP_15_PER = 28,

	ITM_EFF_IMMUN_XENIA5DOTDAMAGE = 29,	//	제나아 동상 면역
};

// dwExtra3
enum EItemElemental
{
	ITEM_ELEMENTAL_NONE = 0,
	ITEM_ELEMENTAL_FIRE = 1,   // 불
	ITEM_ELEMENTAL_ICE = 2,   // 얼음
	ITEM_ELEMENTAL_LIGHTNING = 3,   // 번개
	ITEM_ELEMENTAL_DARK = 4,   // 암흑
	ITEM_ELEMENTAL_WIND = 5,   // 바람
	ITEM_ELEMENTAL_CASH_SHIELD = 6,   // 실드(캐쉬)
	ITEM_ELEMENTAL_GP_SHIELD = 7,   // 실드(GP)
	ITEM_ELEMENTAL_SHINING = 8,   // 빛
	ITEM_ELEMENTAL_LOVE = 9,	// 러브
	ITEM_ELEMENTAL_ROSE = 10,	// 로즈
	ITEM_ELEMENTAL_DEATH = 11,	// 사신
	ITEM_ELEMENTAL_TIME = 12,	// 시간
	ITEM_ELEMENTAL_LIFE = 13,	// 생명
	ITEM_ELEMENTAL_DARK_DRAGON_RED = 14,	// 흑룡( 빨강 )
	ITEM_ELEMENTAL_DARK_DRAGON_BLUE = 15,	// 흑룡( BLUE )
	ITEM_ELEMENTAL_FOOLS_DAY = 16,	// 만우절
	ITEM_ELEMENTAL_ARIES_ANKLE = 17,   // 별자리 양 발찌
	ITEM_ELEMENTAL_TAURUS_ANKLE = 18,   // 별자리 황소 발찌
	ITEM_ELEMENTAL_ZORRO_ANKLE = 19,   // 조로냥 발찌
	ITEM_ELEMENTAL_GEMINI_ANKLE = 20,   // 별자리 쌍둥이 발찌
	ITEM_ELEMENTAL_CANCER_ANKLE = 21,
	ITEM_ELEMENTAL_LEO_ANKLE = 22,
	ITEM_ELEMENTAL_VIRGO_ANKLE = 23,
	ITEM_ELEMENTAL_LIBRA_ANKLE = 24,
	ITEM_ELEMENTAL_SCORPIO_ANKLE = 25,
	ITEM_ELEMENTAL_SAGITTARIUS_ANKLE = 26,
	ITEM_ELEMENTAL_CAPRICORN_ANKLE = 27,
	ITEM_ELEMENTAL_AQUARIUS_ANKLE = 28,
	ITEM_ELEMENTAL_PISCES_ANKLE = 29,
	ITEM_ELEMENTAL_HALLOWEEN = 30,
	ITEM_ELEMENTAL_ELIA = 31,
	ITEM_ELEMENTAL_ELIA_ANKLE = 32,
	ITEM_ELEMENTAL_ARIES = 33, //    양자리 목걸이
	ITEM_ELEMENTAL_TAURUS = 34, //    황소 목걸이
	ITEM_ELEMENTAL_GEMINI = 35, //    쌍둥이 목걸이
	ITEM_ELEMENTAL_CANCER = 36, //    사자 목걸이
	ITEM_ELEMENTAL_LEO = 37, //    게 목걸이
	ITEM_ELEMENTAL_VIRGO = 38, //    처녀 목걸이
	ITEM_ELEMENTAL_LIBRA = 39, //    천칭 목걸이
	ITEM_ELEMENTAL_SCORPIO = 40, //    전갈 목걸이
	ITEM_ELEMENTAL_SAGITTARIUS = 41, //    사수 목걸이
	ITEM_ELEMENTAL_CAPRICORN = 42, //    염소 목걸이
	ITEM_ELEMENTAL_AQUARIUS = 43, //    물병 목걸이
	ITEM_ELEMENTAL_PISCES = 44, //    물고기 목걸이
	ITEM_ELEMENTAL_10TH = 45, //    10주년 발찌
	ITEM_ELEMENTAL_DIMENSION = 46, //    차원의 문 발찌
	ITEM_ELEMENTAL_PEGASUS_ANKLE = 47, //    페가수스 발찌
	ITEM_ELEMENTAL_PEGASUS_NECK = 48, //    페가수스 목걸이
	ITEM_ELEMENTAL_AUTUMN = 49,
	ITEM_ELEMENTAL_CELESTIAL_MASTER = 50,
	ITEM_ELEMENTAL_COLISEU = 51,
	ITEM_ELEMENTAL_FAIRY = 52,
};
/*
// dwExtra3
enum EItemAttributeFlag2
{
	ITEM_ENCHANT_UNABLE     = 0x00010000,   // 강화불가
	ITEM_GUILD_MASTER_ONLY  = 0x00020000,   // 길드마스터와 제너럴만 구입/착용 가능
	ITEM_CRYSTAL_GOODS      = 0x00040000,   // 크리스탈 상품
};
*/

#endif // _GCITEMENUM_H_
