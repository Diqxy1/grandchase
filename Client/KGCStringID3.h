/// @file   KGCStringID.h
/// @author Chang-hyeon Park (pch413@kogstudios.com)
/// @since  2005-03-17 오전 10:30:54

#ifndef _KGCSTRINGID3_H_
#define _KGCSTRINGID3_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
    STR_ID_SLEEP_OUT                        =3230,// 5분동안 입력이 없어서 방에서 자동퇴장 되었습니다.
    STR_ID_DEATHMATCH_BONUS                 =3231,// 데스매치 보너스

    STR_ID_AMY2_TUTORIAL1                   =3232,//기본 콤보      : Z Z Z Z
    STR_ID_AMY2_TUTORIAL2                   =3233,//대쉬 공격      : 대쉬 중 Z
    STR_ID_AMY2_TUTORIAL3                   =3234,//크리티컬 공격  : 기본 콤보 중 →→  또는 ←←
    STR_ID_AMY2_TUTORIAL4                   =3235,//점프 공격      : 점프 중 Z
    STR_ID_AMY2_TUTORIAL5                   =3236,//점프 공격2     : 점프 중 ↓ Z
    STR_ID_AMY2_TUTORIAL6                   =3237,//스토니피어     : 대쉬 중 → Z
    STR_ID_AMY2_TUTORIAL7                   =3238,//소닉 노바      : 기본 콤보 중 ↑
    STR_ID_AMY2_TUTORIAL8                   =3239,//모드 전환      : X
    STR_ID_AMY2_TUTORIAL9                   =3240,//기본 콤보      : Z Z Z Z
    STR_ID_AMY2_TUTORIAL10                  =3241,//대쉬 공격      : 대쉬 중 Z
    STR_ID_AMY2_TUTORIAL11                  =3242,//크리티컬 공격  : 기본 콤보 중 →→  또는 ←←
    STR_ID_AMY2_TUTORIAL12                  =3243,//점프 공격      : 점프 중 Z
    STR_ID_AMY2_TUTORIAL13                  =3244,//더블 점프      : 점프 중 ↑
    STR_ID_AMY2_TUTORIAL14                  =3245,//점프 대쉬      : 점프 중 →→  또는 ←←
    STR_ID_AMY2_TUTORIAL15                  =3246,//매직 무브      : 점프 중 ↓ Z
    STR_ID_AMY2_TUTORIAL16                  =3247,//매직 무브 어택 : 매직 무브 착지 중 Z
    STR_ID_AMY2_PERFORMENCEMODE             =3248,//뮤즈(연주모드)
    STR_ID_AMY2_FIGHTMODE                   =3249,//뮤즈(파이팅모드)
    STR_ID_TITLE_ITEM_DECEASE_ABILITY       =3250,// 능력치 @ % 감소 : 이전단계 칭호가 부족함
    STR_ID_HELL_MODE_TITLE_ITEM_MSG			=3251,// 클리어 시 일정 확률로 칭호를 획득할 수 있습니다.
    STR_ID_TITLE_MISSION_COMPLETE_MSG1		=3252,// "@1" 칭호 획득 조건을 완료하였습니다.
    STR_ID_TITLE_MISSION_COMPLETE_MSG2		=3253,// 컬렉션에서 [칭호]를 확인하세요.
    STR_ID_AMY2_TUTORIAL17           		=3254,// 뮤즈 연주모드 3단필 설명
    STR_ID_DRAMA_DUNGEON                    =3255,//새로운 캐릭터가 등장하는 던젼입니다.
    STR_ID_MINIGAME_NPC_SPEECH_MYNAME0      =3256,//
    STR_ID_MINIGAME_NPC_SPEECH_MYNAME1      =3257,//
    STR_ID_MINIGAME_NPC_SPEECH_MYNAME2      =3258,//
    STR_ID_MINIGAME_NPC_SPEECH_MYNAME3      =3259,//
    STR_ID_MINIGAME_NPC_SPEECH_MYNAME4      =3260,//
    STR_ID_MINIGAME_NPC_SPEECH_BALLOON0     =3261,//
    STR_ID_MINIGAME_NPC_SPEECH_BALLOON1     =3262,//
    STR_ID_MINIGAME_NPC_SPEECH_BALLOON2     =3263,//
    STR_ID_MINIGAME_NPC_SPEECH_WOOD0        =3264,//
    STR_ID_MINIGAME_NPC_SPEECH_WOOD1        =3265,//
    STR_ID_MINIGAME_NPC_SPEECH_WOOD2        =3266,//
    STR_ID_MINIGAME_NPC_SPEECH0             =3267,//
    STR_ID_MINIGAME_NPC_SPEECH1             =3268,//
    STR_ID_MINIGAME_NPC_SPEECH2             =3269,//
    STR_ID_MINIGAME_NPC_SPEECH3             =3270,//
    STR_ID_MINIGAME_NPC_SPEECH4             =3271,//
    STR_ID_MINIGAME_EXIT                    =3272,//
    STR_ID_MINIGAME_DESC0                   =3273,//
    STR_ID_MINIGAME_DESC1                   =3274,//
    STR_ID_MINIGAME_DESC2                   =3275,//
    STR_ID_MINIGAME_NPC_NICK				=3276,//알아서들 지으센~ 홍콩이 부탁해서연;;
    STR_ID_MYSCORE_AND_RANK                 =3277,// @1 ( @2 위)

    STR_ID_ONLINE                           =3278,//온라인
    STR_ID_SEND_MAIL_BTN                    =3279,//쪽지보내기
    STR_ID_INBOX_BTN                        =3280,//받은쪽지함
    STR_ID_DRAFTS_BTN                       =3281,//보낸쪽지함
    STR_ID_NATEON_ONLINE_BUDDY_COUNT        =3282,//현재 @1명 접속 중..
    STR_ID_NATEON_ADD_BUDDY                 =3283,//친구추가
    STR_ID_NATEON_ADD_GROUP                 =3284,//그룹추가
    STR_ID_OFFLINE                          =3285,//오프라인
    STR_ID_NO_FRIENDS_LONEY                 =3286,//등록된 친구 없음
    STR_ID_NATEON_ADD_GROUP_DESC            =3287,//추가할 그룹이름을 입력하세요.
    STR_ID_NATEON_ADD_GROUP_DESC2           =3288,//12자 이내
    STR_ID_NATEON_ADD_GROUP_ERROR           =3289,//그룹추가 중 에러발생
    STR_ID_NATEON_ADD_GROUP_ERROR2          =3290,//더 이상 그룹을 생성할 수 없습니다.
    STR_ID_NATEON_ADD_GROUP_ERROR3          =3291,//그룹이름은 최대 12자 입니다.
    STR_ID_NATEON_ADD_GROUP_ERROR4          =3292,//잘못된 그룹이름입니다.
    STR_ID_NATEON_ADD_GROUP_WARN            =3293,//사용할 수 없는 이름이 포함되어있습니다.
    STR_ID_NATEON_ADD_GROUP_SUCCESS         =3294,//그룹이 생성되었습니다.
    STR_ID_NATEON_MULTIMENU1                =3295,//그룹이름변경
    STR_ID_NATEON_MULTIMENU2                =3296,//그룹삭제
    STR_ID_NATEON_MULTIMENU3                =3297,//대화하기
    STR_ID_NATEON_MULTIMENU4                =3298,//쪽지보내기
    STR_ID_NATEON_MULTIMENU5                =3299,//차단하기
    STR_ID_NATEON_MULTIMENU6                =3300,//친구삭제
    STR_ID_NATEON_MULTIMENU7                =3301,//친구등록
    STR_ID_NATEON_MULTIMENU8                =3302,//차단해제
    STR_ID_NATEON_MULTIMENU9                =3303,//친구이동▷
    STR_ID_NATEON_RENAME_GROUP              =3304,//그룹이름변경
    STR_ID_NATEON_RENAME_GROUP_ERROR        =3305,//그룹이름변경시 에러발생
    STR_ID_NATEON_RENAME_GROUP_SUCCESS      =3306,//그룹이름이 변경되었습니다.
    STR_ID_NATEON_GROUP_NAME_DIFFERENT      =3307,//그룹이름이 달라야합니다.
    STR_ID_NATEON_DELETE_GROUP_ASK1         =3308,//현재 그룹 내 등록된 친구의_정보도 모두 삭제됩니다._그래도 삭제하시겠습니까?
    STR_ID_NATEON_DELETE_GROUP_ASK2         =3309,//다시 한번 묻겠습니다._정말 그룹을 삭제하시겠습니까?
    STR_ID_NATEON_DELETE_GROUP_ERROR        =3310,//그룹삭제시 에러발생
    STR_ID_NATEON_DELETE_GROUP_SUCCESS      =3311,//그룹이 삭제되었습니다.
    STR_ID_NATEON_MOVE_GROUP_ERROR          =3312,//친구이동 중 문제발생
    STR_ID_NATEON_MOVE_GROUP_SUCCESS        =3313,//이동되었습니다.
    STR_ID_NATEON_BLOCK_BUDDY_ERROR         =3314,//친구차단 중 문제발생
    STR_ID_NATEON_BLOCK_BUDDY_SUCCESS       =3315,//차단되었습니다.
    STR_ID_NATEON_UNBLOCK_BUDDY_ERROR       =3316,//차단해제 중 문제발생
    STR_ID_NATEON_UNBLOCK_BUDDY_SUCCESS     =3317,//차단해제되었습니다.
    STR_ID_NATEON_DELETE_BUDDY_ERROR        =3318,//친구삭제 중 문제발생
    STR_ID_NATEON_DELETE_BUDDY_SUCCESS      =3319,//삭제되었습니다.
    STR_ID_NATEON_BLOCK_BUDDY               =3320,//차단하기
    STR_ID_NATEON_ADD_GROUP_DESC3           =3321,//@1자 이내
    STR_ID_NATEON_ADD_GROUP_DESC4           =3322,//차단할 닉네임을 입력하세요.
    STR_ID_NATEON_CHANGE_EMOTICON           =3323,//이모티콘을 변경하세요!!
    STR_ID_NATEON_CHANGE_EMOTICON_ERROR     =3324,//이모티콘 변경중 문제발생
    STR_ID_NATEON_CHANGE_EMOTICON_SUCCESS   =3325,//이모티콘이 변경되었습니다.
    STR_ID_NATEON_ADD_BUDDY_ADDMSG          =3326,//요청할 친구의 닉네임을 입력하세요.
    STR_ID_NATEON_ADD_BUDDY_RECEIVE         =3327,//친구 요청 메세지입니다.
    STR_ID_NATEON_ADD_BUDDY_NICKNAME        =3328,//닉네임
    STR_ID_NATEON_ADD_BUDDY_MSG             =3329,//메세지
    STR_ID_NATEON_ADD_BUDDY_REQMSG          =3330,//메세지를 입력하세요.
    STR_ID_NATEON_ADD_BUDDY_DEFMSG          =3331,//우리 친구해요 ^^;
    STR_ID_NATEON_ERROR_ADD_ME              =3332,//자신을 친구 추가 하려고 합니다.
    STR_ID_NATEON_ERROR_ADD_ALREADY         =3333,//이미 친구이거나 친구 추가 요청 중인 친구입니다.
    STR_ID_NATEON_ERROR_ADD_OVERFLOW        =3334,//친구 제한인원을 초과 하였습니다.
    STR_ID_NATEON_ERROR_ADD_DEFAULT         =3335,//친구 추가 중 오류가 발생하였습니다.
    STR_ID_NATEON_ERROR_TALK_DEFAULT        =3336,//대화 시작 중 오류가 발생되었습니다.
    STR_ID_NATEON_ERROR_TALK_CHAT_COUNT     =3337,//대화창 제한 갯수를 초과 하였습니다.
    STR_ID_NATEON_OUT_OF_TALKBOX		    =3338,//@1님이 대화방에서 나갔습니다.
    STR_ID_NATEON_ADD_COMPLETE				=3339,//친구 추가 요청을 완료하였습니다.
    STR_ID_NATEON_FRIEND_CANT_START_TALK	=3340,//상대가 대화할 수 있는 상태가 아닙니다.
    STR_ID_UNBLOCK_FRIEND_1					=3341,//@1 님과는 이전에 친구였습니다.
    STR_ID_UNBLOCK_FRIEND_2					=3342,//다시 친구로 추가 하시겠습니까?
    STR_ID_NATEON_ADD_GROUP_ALREADY_REG     =3343,//이미 등록된 그룹 이름 입니다.
    STR_ID_NATEON_BLOCK_BUDDY_ALREADY       =3344,//이미 차단 되었습니다.
    STR_ID_NATEON_BLOCK_FRIEND_TO_ADD       =3345,//차단 된 친구 입니다.
    STR_ID_NATEON_ADD_ALREADY_BLOCK         =3346,//친구 승인요청 중이거나 차단된 친구입니다.
    STR_ID_NATEON_BLOCK_ME					=3347,//자기 자신은 차단할 수 없습니다.
    STR_ID_NATEON_DONT_FIND					=3348,//상대방 정보가 존재하지 않습니다.
    STR_ID_NATEON_UNBLOCK_ME				=3349,//자기 자신은 차단해제 할 수 없습니다.
    STR_ID_NATEON_MOVE_NOT_FRIEND			=3350,//친구 요청중인 상태에서는 그룹 이동을 할 수 없습니다.
    STR_ID_NATEON_OVERFLOW_USER				=3351,//초대 가능 맴버수를 초과하였습니다.
    STR_ID_NATEON_SOME_USER_CANT_INVITE		=3352,//일부 친구를 초대할 수 없습니다.
    STR_ID_NATEON_DONT_EXIST_INVITE_ROOM	=3353,//초대할 채팅방이 존재 하지 않습니다.
    STR_ID_NATEON_CANT_INVITE				=3354,//초대 중 문제가 발생하였습니다.

    STR_ID_EXPID_NOT_ENTER_BRAVE_ISLAND     =3355,// 체험계정은 용사의 섬 입장이
    STR_ID_EXPID_NOT_ENTER_FREE_CHANNEL     =3356,// 체험계정은 자유채널 입장이
    STR_ID_EXPID_NOT_BUY_CASH_ITEM          =3357,// 체험계정은 캐쉬아이템 구매가
    STR_ID_EXPID_NOT_FILL_CASH              =3358,// 체험계정은 캐쉬 충전이
    STR_ID_EXPID_IMPOSSIBLE                 =3359,// 불가능 합니다.
    STR_ID_EXPID_NOT_RANKING                =3360,// 체험계정은 랭킹이 기록되지 않습니다.

    STR_ID_PET_NAME20						=3361,//팽팽
    STR_ID_PENGPENG_MENT1					=3362,//팽팽? 주인님이신가? 팽팽.. 움~ 저 물고기 잘먹어요~ \n 로그아웃 하지말고 나랑 놀아줘요~ 나가면 나 화낼꺼에요\n 우왕~ 그 정도면 팽팽이의 주인님이 되기 충분해요~ 팽팽♡ 함께해요~♡
    STR_ID_PENGPENG_MENT2					=3363,//우왕~ 팽팽! 내 주인님이구낭~ 방가 방가워요~♡ \n 전 싱싱한 물고기만 먹는답니다~ 물고기~ 주세요~
    STR_ID_PENGPENG_MENT3					=3364,//팽팽~ 팽팽~ 나 물고기 좋아해요~ 물고기 주세요~
    STR_ID_PENGPENG_MENT4					=3365,//귀엽고 잘 구르고, 수영도 잘한답니다~ 팽팽~
    STR_ID_PENGPENG_MENT5					=3366,//팽팽~ 물고기~ 사주세욤~ 우에에에에엥~~
    STR_ID_PENGPENG_MENT6					=3367,//레벨 업! 팽팽~ 꺄르르르~ 파워!! 팽팽~ 굴러굴러~ 팽팽~
    STR_ID_PENGPENG_MENT7					=3368,//꺄르르르르~~~ 우리 주인님 레벨 업! 팽팽!! 팽팽!! 나도 나도 키워줘요~~!

    STR_ID_NATEON_BLOCK_OVERFLOW			=3369,//차단 제한 인원을 초과했습니다.
    STR_ID_NATEON_BLOCK_ALREADY				=3370,//이미 차단 요청을 한 상태입니다.

    STR_ID_TRAINSKILL_ERROR                 =3371,//스킬 배우는 도중 알 수 없는 오류 발생
    STR_ID_TRAINSKILL_SUCCESS               =3372,//스킬을 배웠습니다.
    STR_ID_UNTRAINSKILL_ERROR               =3373,//스킬 취소 도중 알 수 없는 오류 발생
    STR_ID_UNTRAINSKILL_SUCCESS             =3374,//스킬을 취소했습니다.
    STR_ID_SKILLTREE_CURRENT_SP				=3375,//
    STR_ID_SKILLTREE_MAX_SP					=3376,//
    STR_ID_SKILLTREE_USING_SP				=3377,//
    STR_ID_SKILLTREE_CURRENT_MAX_SP			=3378,//
    STR_ID_SKILLTREE_CURRENT_SKILL_LEVEL	=3379,//
    STR_ID_SKILLTREE_SKILLSET1				=3380,//
    STR_ID_SKILLTREE_SKILLSET2				=3381,//
    STR_ID_REPLAY_NOT_YET                   =3382,//현재 리플레이 기능이 지원되지 않습니다._빠른시일내에 지원되도록 노력하겠습니다._감사합니다.
    STR_ID_SKILL_ALREADY_EXIST              =3383,//이미 획득한 스킬입니다.
    STR_ID_SKILL_TRAIN_CONDITION_ERROR1     =3384,//선행 스킬을 완료하지 않았습니다.
    STR_ID_SKILL_TRAIN_CONDITION_ERROR2     =3385,//전직 레벨을 만족하지 못했습니다.
    STR_ID_SKILL_TRAIN_CONDITION_ERROR3     =3386,//캐릭터 레벨을 만족하지 못했습니다.
    STR_ID_NOMORE_SKILL_POINT               =3387,//남은 스킬 포인트가 부족합니다.
    STR_ID_CHANGE_SKILL_SET_ERROR           =3388,//스킬세트 변경 도중 알 수 없는 오류 발생
    STR_ID_SKILL_UNTRAIN_CONDITION_ERROR1   =3389,//연결된 스킬이 있어서 삭제할 수 없습니다.
    STR_ID_SKILL_UNTRAIN_CONDITION_ERROR2   =3390,//스킬 취소 아이템이 없습니다.
    STR_ID_SKILL_UNTRAIN_CONDITION_ERROR3   =3391,//아직 배우지 않은 스킬입니다

    STR_ID_SKILL_SUPERARMOR                 =3392,//슈퍼아머 상태
    STR_ID_SKILL_SUPER_BERSERKER_EFFECT     =3393,//슈퍼버서커 효과
    STR_ID_SKILL_BIG_STEP_EFFECT            =3394,//빅 스텝 효과
    STR_ID_SKILL_INCREASE_SPEED             =3395,//이동 속도 증가

    STR_ID_CONNECTING_UR_SERVER             =3396,//연결중입니다. 잠시만 기다려 주세요.
    STR_ID_SKILLTREE_RESET_POINT			=3397,//스킬초기화\n포인트
    STR_ID_SKILLTREE1						=3398,//@1을(를) 획득하시겠습니까?\nSP : @2 감소
    STR_ID_SKILLTREE2						=3399,//SP 초기화 아이템 수량이 부족합니다.\n[상점]-[훈련소]에서 구입하세요.
    STR_ID_SKILLTREE3						=3400,//스킬 획득
    STR_ID_SKILLTREE4						=3401,//스킬 제거
    STR_ID_SKILLTREE5						=3402,//스킬 셋팅 초기화
    STR_ID_SKILLTREE6						=3403,//필요 레벨 : Lv @1
    STR_ID_SKILLTREE7						=3404,//획득방법 : [상점]-[훈련소]에서 구입
    STR_ID_SKILLTREE8						=3405,//조작 : @1
    STR_ID_SKILLTREE9						=3406,//필살기 1단
    STR_ID_SKILLTREE10						=3407,//필살기 2단
    STR_ID_SKILLTREE11						=3408,//필살기 3단
    STR_ID_SKILLTREE12						=3409,//SP : @1
    STR_ID_SKILLTREE13						=3410,//필요 스킬 : @1
    STR_ID_SKILL_UNTRAIN_CONDITION_ERROR4   =3411,//현재 직업이 배울 수 있는 스킬수를 초과했습니다._획득한 스킬을 제거하시고 다시 시도해 주세요.
    STR_ID_SKILLTREE14						=3412,//없음
    STR_ID_SKILLTREE15						=3413,//이 스킬을 제거하시겠습니까?

    STR_ID_SKILL_GROUP_OPEN                 =3414,//스킬이 열렸습니다.
    STR_ID_SKILL_GROUP_OPEN_UNKNOWNERROR    =3415,//스킬 그룹을 여는중 알 수 없는 오류 발생
    STR_ID_SKILL_GROUP_OPEN_ERROR1          =3416,//이미 열린 스킬그룹입니다.
    STR_ID_SKILL_GROUP_OPEN_ERROR2          =3417,//스킬 그룹을 열기 위한 아이템이 부족합니다
    STR_ID_SKILLTREE16						=3418,//현재 남아있는 스킬 포인트
    STR_ID_SKILLTREE17						=3419,//내가 지금까지 획득한 스킬 포인트
    STR_ID_SKILLTREE18						=3420,//이 전직에 사용한 스킬 포인트
    STR_ID_SKILLTREE19						=3421,//이 전직에 사용할 수 있는 스킬포인트
    STR_ID_SKILLTREE20						=3422,//봉인된 스킬을 봉인 해제 합니다.
    STR_ID_SKILLTREE21						=3423,//Lv @1
    STR_ID_SKILLTREE22						=3424,//SP @1
    STR_ID_SKILLTREE23						=3425,//빈 스킬 타이틀
    STR_ID_SKILLTREE24						=3426,//빈 스킬 중간 설명
    STR_ID_SKILLTREE25						=3427,//빈 스킬 설명
    STR_ID_USE_COUPON_ITEM_ERROR			=3428,//쿠폰 아이템 사용 도중
    STR_ID_USE_COUPON_ITEM_ERROR1			=3429,//해당 아이템은 쿠폰 아이템이 아닙니다.
    STR_ID_USE_COUPON_ITEM_ERROR2			=3430,//해당 쿠폰을 사용하면 최대 GP보유량을 초과 합니다.
    STR_ID_USE_COUPON_ITEM_ERROR3			=3431,//해당 쿠폰이 인벤토리에 존재하지 않습니다.
    STR_ID_USE_COUPON_ITEM_ERROR4			=3432,//쿠폰 아이템 제거 실패
    STR_ID_USE_COUPON_ITEM					=3433,//정말 @1을(를) 사용 하시겠습니까?
    STR_ID_BRUNCH_OF_FLOWERS                =3434,//꽃다발
    STR_ID_FRIENDS_DAY_EVENT				=3435,//프렌즈 데이 이벤트
    STR_ID_PET_CUCA_NAME                    =3436,// 쿠카
    STR_ID_PET_CUCA_HUNGRY                  =3437,// 쿠카
    STR_ID_PET_CUCA_LEVELUP                 =3438,// 쿠카
    STR_ID_PET_CUCA_STARVATION              =3439,// 쿠카
    STR_ID_PET_CUCA_SHOP                    =3440,// 쿠카
    STR_ID_PET_CUCA_SHOWROOM                =3441,// 쿠카
    STR_ID_PET_CUCA_NORMAL1                 =3442,// 쿠카
    STR_ID_PET_CUCA_NORMAL2                 =3443,// 쿠카
    STR_ID_PET_CUCA_NORMAL3                 =3444,// 쿠카
    STR_ID_CANT_ENTER_SQUARE_IN_INDIGO		=3445,// 용사의 섬에서는 광장에 입장하실 수 없습니다.
    STR_ID_RONAN4_ABYSSKNIGHT				=3446,//어비스나이트
    STR_ID_RONAN4_COMBO						=3447,//기본콤보 :Z Z Z Z
    STR_ID_RONAN4_AERIALCOMBO				=3448,//공중콤보 :기본콤보 중, ↑ Z Z
    STR_ID_RONAN4_MAGICCOMBO				=3449,//마법콤보 :기본콤보 중, ↓ Z
    STR_ID_RONAN4_DASHCOMBO   				=3450,//대쉬콤보 :대쉬 공격 중, ↑ Z
    STR_ID_RONAN4_JUMP_MAGICATK				=3451,//점프 마법공격:점프 중, ↓ Z
    STR_ID_RONAN4_RAGINGSWORD  				=3452,//레이징소드:대쉬 중, ← 또는 → Z Z
    STR_ID_RONAN4_CHARGE_FORCE 				=3453,//포스모으기:Z ↑
    STR_ID_RONAN4_MONOVOLT  				=3454,//모노볼트 :포스모으기 후, Z ← 또는 Z →
    STR_ID_RONAN4_DUSTCHIP   				=3455,//더스트 	 :포스모으기 후, Z ↓ 
    STR_ID_RONAN4_SPECIAL_LV1				=3456,//마검술Lv1:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_RONAN4_SPECIAL_LV2				=3457,//마검술Lv2:z키를 눌러 기를 2칸 채운 후 버튼을 뗀다
    STR_ID_RONAN4_SPECIAL_LV3				=3458,//마검술Lv3:z키를 눌러 기를 3칸 채운 후 버튼을 뗀다
    STR_ID_RONAN4_SPECIAL_LV1_2				=3459,//백마법Lv1:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_RONAN4_SPECIAL_LV2_2				=3460,//백마법Lv2:z키를 눌러 기를 2칸 채운 후 버튼을 뗀다
    STR_ID_RONAN4_SPECIAL_LV3_2				=3461,//백마법Lv3:z키를 눌러 기를 3칸 채운 후 버튼을 뗀다
    STR_ID_CLEAR_SEAL_COMMENT1				=3462,//봉인해제서가
    STR_ID_CLEAR_SEAL_COMMNET2				=3463,//@개소비됩니다.
    STR_ID_GACHA_ERROR1						=3464, //꽝입니다. 다음 기회에..
    STR_ID_GACHA_ERROR2						=3465, //가차 아이템인데 로터리에 등록 되지 않음
    STR_ID_GACHA_ERROR3						=3466, //해당 아이템은 가차 아이템이 아님
    STR_ID_GACHA_ERROR4						=3467, //해당 가차 아이템을 보유하고 있지 않음
    STR_ID_GACHA_ERROR5						=3468, //봉인 해제서가 부족합니다.
    STR_ID_GACHA_ERROR6						=3469, //트랙잭션 에러
    STR_ID_GACHA_ERROR7						=3470, //DB에 해당 유저가 존재하지 않음
    STR_ID_GACHA_ERROR8						=3471, //DB에 존재하지 않는 아이템 입니다.
    STR_ID_GACHA_ERROR9						=3472,  //가차 아이템 처리 도중
    STR_ID_GACHA_SET_ERROR1					=3473, //이미 획득한 가차 세트 입니다.
    STR_ID_GACHA_SET_ERROR2					=3474, //가차 수집 아이템 리스트가 없습니다.
    STR_ID_GACHA_SET_ERROR3					=3475, //가차 수집 아이템이 부족 합니다.
    STR_ID_GACHA_SET_ERROR4					=3476, //가차 세트 보상 리스트 정보 가져오기 실패.
    STR_ID_GACHA_SET_ERROR5					=3477, //가차 세트 보상 완료 기록 실패
    STR_ID_GACHA_SET_ERROR6					=3478, //가차 세트 보상 아이템 요청시 DB 오류
    STR_ID_GACHA_SET_ERROR7					=3479, //가차 세트 아이템 처리 중
    STR_ID_ITEM_RECEIVE3					=3480, //' @1 (@2일) ' 을/를 획득했습니다.
    STR_ID_NOT_ENOUGH_CLEAR_SPELL_ITEM		=3481, //봉인해제서가 부족합니다.
    STR_ID_NOT_ENOUGH_BEAD_ITEM				=3482, //봉인구슬이 부족합니다.
    STR_ID_QUEST22_DRAMA1					=3483,	//엇!! 이곳읜..?! 좀 이상하게 생겼네..?
    STR_ID_QUEST22_DRAMA2					=3484,	//그렇군요. 이곳은 신비로운 힘들이 작용하는 독특한 곳 이군요.
    STR_ID_QUEST22_DRAMA3					=3485,	//아~ 아름다워..
    STR_ID_QUEST22_DRAMA4					=3486,	//하지만 어둠의 힘도 함께하고 있어요. 왠지 불길한 곳..
    STR_ID_QUEST22_DRAMA5					=3487,	//또 수많은 몬스터들이 득실대고 있겠군요.
    STR_ID_QUEST22_DRAMA6					=3488,	//조율의 신전으로 가려면 어쩔 수 없지. 일단 이곳을 지날 수 밖에..
    STR_ID_QUEST22_DRAMA7					=3489,	//후.. 이만하면 이 근처의 몬스터들은 거의 다 정리된 건가?
    STR_ID_QUEST22_DRAMA8					=3490,	//하지만 이곳의 몬스터들은 정말 강하네요. 상대하기가 여간 힘든게 아니에요.
    STR_ID_QUEST22_DRAMA9					=3491,	//겨우 이정도로 약한 소리하지마. 앞으로 이보다 더 강한 녀석들도 많이 만나게 될테니..
    STR_ID_QUEST22_DRAMA10					=3492,	//맞는 말이다. 하지만 많이 만날 것 같지는 않군. 내가 너희가 만나는 마지막 상대가 될테니..
    STR_ID_QUEST22_DRAMA11					=3493,	//?!!!!
    STR_ID_QUEST22_DRAMA12					=3494,	//넌 누구냐?!!
    STR_ID_QUEST22_DRAMA13					=3495,	//내 이름은 아스타롯. 위대하신 지배의 신이 계시는 지배의 신전의 수호자이지.
    STR_ID_QUEST22_DRAMA14					=3496,	//지배의 신?!! 우릴 습격하러 온거냐?!!
    STR_ID_QUEST22_DRAMA15					=3497,	//그렇다. 지금 지배의 신께선 너의들이 한 일들에 대해 매우 불쾌하게 생각하고 계신다. 그래서 지배의 신을 대신해 내가 직접 너희들에게 벌을 내리러 온 것이지.
    STR_ID_QUEST22_DRAMA16					=3498,	//흥! 지배의 신의 졸개 따위 얼마든지 오라지. 우린 지배의 신이 직접 온다고 해도 겁안나!!
    STR_ID_QUEST22_DRAMA17					=3499,	//엘리시스님.. 조심하십시오. 저자에게서 매우 강한 힘이 느껴집니다.
    STR_ID_QUEST22_DRAMA18					=3500,	//너희들은 이제부터 죽음의 공포를 느끼게될 것이다.
    STR_ID_QUEST22_DRAMA19					=3501,	//훗! 제법이군. 그랜드체이스.
    STR_ID_QUEST22_DRAMA20					=3502,	//하지만 그것도 여기까지다. 이제 끝장을 내주마!!
    STR_ID_QUEST22_DRAMA21					=3503,	//엘리시스 : 큭!! 이.. 이건?!!
    STR_ID_QUEST22_DRAMA22					=3504,	//이.. 이런 강력한 마법을 쓸 수 있다니.. 믿을 수 없어..
    STR_ID_QUEST22_DRAMA23					=3505,	//로난!! 어떻게 좀 해봐..! 이대로는 전멸이야!!
    STR_ID_QUEST22_DRAMA24					=3506,	//이것이 바로 지배의 절대적인 힘이다.
    STR_ID_QUEST22_DRAMA25					=3507,	//더 이상은.. 버틸 수 없.. 어..
    STR_ID_QUEST22_DRAMA26					=3508,	//큭..! 방해하지 마라..! 바네사!! 이건 너의 싸움이 아냐!
    STR_ID_QUEST22_DRAMA27					=3509,	//나에겐 이들을 보호할 의무가 있다.
    STR_ID_QUEST22_DRAMA28					=3510,	//쳇..! 다 긑난 싸움이었는데..
    STR_ID_QUEST22_DRAMA29					=3511,	//감사합니다. 바네사님. 덕분에 목숨을 건질 수 있었습니다.
    STR_ID_QUEST22_DRAMA30					=3512,	//그런데 바네사님이 여길 어떻게..?
    STR_ID_QUEST22_DRAMA31					=3513,	//불길한 기운을 느끼신 순환의 신께서 친히 저더러 여러분들을 따라가 보호해주라 명하셨습니다.
    STR_ID_QUEST22_DRAMA32					=3514,	//역시 순환의 신께선 대단하신 분이시군요.
    STR_ID_QUEST22_DRAMA33					=3515,	//자, 아직 전투가 끝나지 않았습니다. 자세한 이야기는 나중에..
    STR_ID_QUEST22_DRAMA34					=3516,	//바네사.. 어째서 너와 순환의 신은 그런 하찮은 존재들에게 기대를 하고 있는 거지?  이들의 힘은 너무나 미약할 뿐인데도 말이야.
    STR_ID_QUEST22_DRAMA35					=3517,	//이들은 서로 힘을 합쳐 함께 싸워 나갈 때 어떠한 어려움도 이겨낼 수 있다는 것을 누구 보다도 잘 알고 있다. 그래서 이들은 항상 동료들을 지켜주며 그들의 힘이 되어주기 위해 싸우지. 그것이 이들의 싸우는 방식이야.
    STR_ID_QUEST22_DRAMA36					=3518,	//그렇기 때문에 이들은 강하다. 그리고 이들을 돕고자 하는 동료가 더욱 많아질수록 이들은 더욱 강해질 것이다.
    STR_ID_QUEST22_DRAMA37					=3519,	//나와 순환의 신께서 이들에게 기대를 거는 이유는 바로 그것이다.
    STR_ID_QUEST22_DRAMA38					=3520,	//훗! 그런 꿈같은 환상을 너도 믿는 거냐? 네가 나타나지 않았다면 이들은 모두 나의 손에 목숨을 잃었을 것이다. 그런데도 어떤 어려움도 이겨낼 수 있다고 말할 수 있나?
    STR_ID_QUEST22_DRAMA39					=3521,	//훗!! 지배의 신전에 있어야 할 네가 이런 곳까지 와서 직접 이들을 죽이려 한 이유가 바로 나의 대답이다. 지금 이들의 힘이 조금 부족하다면 내가 이들의 동료가 되어 그 부족함을 채워줄 것이다.
    STR_ID_QUEST22_DRAMA40					=3522,	//끝까지 방해할 생각인가 보군. 바네사.. 그렇다면 이들과 함께 너까지 지옥으로 보내주마.
    STR_ID_QUEST22_DRAMA41					=3523,	//앗! 저 녀석은..?
    STR_ID_QUEST22_DRAMA42					=3524,	//소환수를 소환했군요. 이번에도 쉽지 않은 전투가 될 것 같습니다.
    STR_ID_QUEST22_DRAMA43					=3525,	//가거라! 나의 종아!! 가서 저 어리석은 자들을 짓밟아 버려라!!
    STR_ID_QUEST22_DRAMA44					=3526,	//크아아앙!!
    STR_ID_QUEST22_DRAMA45					=3527,	//온닷!! 모두들 준비해!!
    STR_ID_QUEST22_DRAMA46					=3528,	//하.. 하아..!! 드디어 해치운 건가?
    STR_ID_QUEST22_DRAMA47					=3529,	//후.. 정말 힘든 싸움이었습니다.
    STR_ID_QUEST22_DRAMA48					=3530,	//헉.. 헉!! 우리의 힘을 잘 봤겠지!? 검은 갑옷!!  자, 이제 네 차례다!!
    STR_ID_QUEST22_DRAMA49					=3531,	//... ( 결국은 이겨낸 건가? 바네사의 도움이 있었다고는 하지만.. 역시 방심할 수 없는 녀석들이군. )
    STR_ID_QUEST22_DRAMA50					=3532,	//보았는가? 아스타롯!!? 이게 바로 지켜야 할 것들이 있는 자들의 강함이다!
    STR_ID_QUEST22_DRAMA51					=3533,	//바네사.. 지쳐 보이는군.
    STR_ID_QUEST22_DRAMA52					=3534,	//지친 건 어차피 너도 마찬가지. 그런 마법을 그렇게 많이 쓰고도 멀쩡할 리가 없어.
    STR_ID_QUEST22_DRAMA53					=3535,	//그래. 나 역시 좀 지쳤버렸다. 꼬마마법사. 신전으로 돌아가서 좀 쉬어야겠어.
    STR_ID_QUEST22_DRAMA54					=3536,	//우리가 이대로 널 보낼 것 같으냐? 어서 내려와 덤벼라!!
    STR_ID_QUEST22_DRAMA55					=3537,	//의기양양하긴..  만일 바네사가 없었다면 너희 모두 살아남지 못했을 터인데..
    STR_ID_QUEST22_DRAMA56					=3538,	//바네사.. 네가 우리의 지배에서 벗어나 적이 되었다는 것은 참으로 유감스러운 일이다.  어떤 의미에선 네가 순환의 신보다 더 상대하기 까다로운 존재였으니까 말이야.
    STR_ID_QUEST22_DRAMA57					=3539,	//...
    STR_ID_QUEST22_DRAMA58					=3540,	//널 봐서 이번만은 물러가주지. 서로 피차 본 모습을 드러내가며 싸워봤자 득이 될게 없을 테니까..
    STR_ID_QUEST22_DRAMA59					=3541,	//하지만 지켜보고 있겠다. 앞으로 너 없이 이들이 얼마나 더 살아가 수 있을지를 말이야. 하하하하핫
    STR_ID_QUEST22_DRAMA60					=3542,	//우릴 상당히 얕보고 있네요. 아휴~~ 기분 나빠!!
    STR_ID_QUEST22_DRAMA61					=3543,	//그렇지 않습니다. 그는 여러분들을 상당히 경계하고 있습니다. 원래 지배의 신전에 있어 야 할 그가 여기까지 직접 나섰다는 것이 바로 그 이유이지요.
    STR_ID_QUEST22_DRAMA62					=3544,	//아무튼 바네사님 덕분에 무사할 수 있었습니다. 정말 감사합니다.
    STR_ID_QUEST22_DRAMA63					=3545,	//햐~ 이제 바네사님도 우리들과 함께 가는 건가요? 이거 아주 든든한데..?
    STR_ID_QUEST22_DRAMA64					=3546,	//아뇨. 제가 도울 수 있는 것은 여기까지 입니다. 이곳 이후로는 다른 신들의 영역이라 제가 아무렇게 드나들 수 있는 곳이 아니죠.
    STR_ID_QUEST22_DRAMA65					=3547,	//하지만 저 없이도 여러분들은 분명 잘 해내실 수 있으실 겁니다. 지금처럼 동료들을 사랑 하고 하나되는 마음으로 언제나 함께 한다면 누구도 여러분들을 당해낼 수 없을테니까요.
    STR_ID_QUEST22_DRAMA66					=3548,	//네! 지배의 신 따윈 저희에게 맡겨만 주세요!!
    STR_ID_QUEST22_DRAMA67					=3549,	//자, 그럼 이제 슬슬 발걸음을 옮겨 볼까? 언제 다시 몬스터들이 모여들지 알 수 없으니..
    STR_ID_QUEST22_DRAMA68					=3550,	//다시 만나자마자 헤어지게 되는군요. 바네사님.
    STR_ID_QUEST22_DRAMA69					=3551,	//이런 곳에서 지체할 여유가 없으니까요.
    STR_ID_QUEST22_DRAMA70					=3552,	//바네사누님.. 그럼 이만..(흑흑)
    STR_ID_QUEST22_DRAMA71					=3553,	//여러분들에게 신들의 가호가 함께 하길..
    STR_ID_NAME_ASTAROT						=3554,	//아스타롯
    STR_ID_NAME_BULTAROS					=3555,	//불타로스
    STR_ID_GC_GM_QUEST22					=3556,	//소실의 협곡

    STR_ID_CHAT_EVENT_ENABLED				=3582,//하하하하 채팅 이벤트 활성화 되었음
    STR_ID_CHAT_EVENT_DISABLED1				=3583,//하하하하 채팅 이벤트 즐 되었음. 누군가 나감
    STR_ID_CHAT_EVENT_DISABLED2				=3584,//하하하하 채팅 이벤트 즐 되었음. 누군가 들어옴
    STR_ID_CHAT_EVENT_DISABLED3				=3585,//하하하하 채팅 이벤트 즐 되었음. 시간 초과
    STR_ID_CHAT_EVENT_DISABLED4				=3586,//하하하하 채팅 이벤트 즐 되었음. 레디 변경
    STR_ID_CHAT_EVENT_INFORMATION			=3587,//하하하하 채팅 이벤트 하는 중이삼.
    STR_ID_BUFF_RONAN2_HOLY_INFERNO         =3588,//홀리 인페르노
    STR_ID_FRIENDSHIP_RING					=3589,// 우정의 반지
    STR_ID_BUFF_AMY2_MANA_REGEN				=3590,//에미1차가 쓰는 마나회복버프
    STR_ID_INFO_MP_INC						=3591,//마나회복 속도

    STR_ID_TITLE_SHORTAGE_CONDITION         =3592,//앞에 칭호 다 획득해야 줄꺼야!!!!
    STR_ID_ENTER_CHANNEL_ERROR11			=3593, //"옳지 않은 IP 대역입니다._다른 체널로 입장해 주십시오."
    STR_ID_SKILLTREE_SWAPBTN1				=3594,//	흑마법으로 전환합니다. (단축키 : X )
    STR_ID_SKILLTREE_SWAPBTN2				=3595,//	백마법으로 전환합니다. (단축키 : X )
    STR_ID_GMC_ANGELS_EGG					=3596,//천사의 알 깨기 
    STR_ID_DESC_ANGELSEGG_1                 =3597,//천사 알 친절한 설명씨~
    STR_ID_DESC_ANGELSEGG_2                 =3598,//천사 알 친절한 설명씨~
    STR_ID_AMGEL_EGG                        =3599,//천사의 알 
    STR_ID_GACHA_KEY_BUY_ERROR1				=3600,//일일 구매 한도 횟수(@1회)를 초과 하셨습니다.
    STR_ID_TUTORIAL_GUIDE_STRING1			=3601,//ESC키를 누르면 연습모드가 종료되고\n초보자 가이드를 계속 진행할 수 있습니다.	
    STR_ID_TUTORIAL_GUIDE_STRING2			=3602,//마우스 왼쪽 버튼을 클릭하면\n연습용 케릭터의 위치를 변경할 수 있습니다
    STR_ID_SKILL_GROUP_OPEN_ERROR3			=3603,//방에서는 스킬의 봉인을 해제 하실 수 없습니다.
    STR_ID_MINIMIZED_EXPLORER_STRING		=3604,//넷마블
    STR_ID_BUFF_AMY_TOGETHER_BONUS			=3605,//에이미 이벤트					   
    STR_ID_SEIREN_DANCING					=3606,//세이렌(댄싱모드)
    STR_ID_SEIREN_DANCING_COMBO				=3607,//기본콤보        : Z Z Z
    STR_ID_SEIREN_DANCING_CRITICAL_ATK		=3608,//크리티컬공격    : 기본콤보 중 →→ 또는 ←←
    STR_ID_SEIREN_DANCING_MODE_CHANGE		=3609,//피이팅모드 전환 : X
    STR_ID_SEIREN_DANCING_DOUBLE_JUMP		=3610,//더블점프        : 점프 중 ↑
    STR_ID_SEIREN_DANCING_DASH_ATK			=3611,//대쉬공격        : 대쉬 중 Z
    STR_ID_SEIREN_DANCING_UNIQUE_ATK		=3612,//펫 던지기       : Z → 또는 Z ←
    STR_ID_SEIREN_DANCING_JUMP_UNIQUE_ATK1	=3613,//세이렌 샷       : 점프 중 ↓ Z
    STR_ID_SEIREN_DANCING_JUMP_UNIQUE_ATK2	=3614,//세이렌 샷 콤보  : 점프 중 ↓ Z, Z
    STR_ID_SEIREN_DANCING_SPECIAL1			=3615,//필살기Lv1:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_SEIREN_DANCING_SPECIAL2			=3616,//필살기Lv2:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_SEIREN_DANCING_SPECIAL3			=3617,//필살기Lv3:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_SEIREN_FIGHTING					=3618,//세이렌(파이팅모드)
    STR_ID_SEIREN_FIGHTING_COMBO			=3619,//기본콤보        : Z Z Z Z Z Z
    STR_ID_SEIREN_FIGHTING_CRITICAL_ATK		=3620,//크리티컬공격    : 기본콤보 중 →→ 또는 ←←
    STR_ID_SEIREN_FIGHTING_MODE_CHANGE		=3621,//댄싱모드 전환   : X
    STR_ID_SEIREN_FIGHTING_DOUBLE_JUMP		=3622,//더블점프        : 점프 중 ↑
    STR_ID_SEIREN_FIGHTING_DASH_ATK			=3623,//대쉬공격        : 대쉬 중 Z, Z
    STR_ID_SEIREN_FIGHTING_UNIQUE_ATK		=3624,//펫 샷           : Z → 또는 Z ←
    STR_ID_SEIREN_FIGHTING_JUMP_UNIQUE_ATK1	=3625,//세이렌 샷       : 점프 중 ↓ Z
    STR_ID_SEIREN_FIGHTING_JUMP_UNIQUE_ATK2	=3626,//세이렌 샷 콤보  : 점프 중 ↓ Z, Z
    STR_ID_SEIREN_FIGHTING_SPECIAL1			=3627,//필살기Lv1:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_SEIREN_FIGHTING_SPECIAL2			=3628,//필살기Lv2:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다	
    STR_ID_SEIREN_FIGHTING_SPECIAL3			=3629,//필살기Lv3:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_SEIREN_DANCING_SPECIAL1_BUFF		=3630,//필살기Lv3:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_SEIREN_DANCING_SPECIAL3_BUFF		=3631,//필살기Lv3:z키를 눌러 기를 1칸 채운 후 버튼을 뗀다
    STR_ID_NEW_ITEM_ADD						=3632,//인벤토리에 새로운 장비 아이템이 있습니다.
    STR_ID_NEW_SKILL_ADD					=3633,//새로운 스킬을 획득할 수 있습니다.
    STR_ID_COUPLE_ITEM_CONFIRM				=3634,//커플 아이템을 옷장에 넣겠는가?
    STR_ID_COUPLE_ITEM0						=3635,//커플링
    STR_ID_COUPLE_ITEM1						=3636,//커플모자
    STR_ID_COUPLE_ITEM2						=3637,//커플티
    STR_ID_COUPLE_ITEM3						=3638,//커플바지
    STR_ID_COUPLE_ITEM4						=3639,//커플신발
    STR_ID_COUPLE_SYSTEM0					=3640,//커플
    STR_ID_COUPLE_SYSTEM1					=3641,//커플맺기
    STR_ID_COUPLE_SYSTEM2					=3642,//커플해제
    STR_ID_COUPLE_SYSTEM3					=3643,//커플창보기
    STR_ID_COUPLE_SYSTEM4					=3644,//반지교체
    STR_ID_COUPLE_SYSTEM5					=3645,//커플모션
    STR_ID_COUPLE_SYSTEM6					=3646,//메세지
    STR_ID_COUPLE_SYSTEM7					=3647,//@1님이 커플맺기를 신청하셨습니다.
    STR_ID_COUPLE_SYSTEM8					=3648,//@1님과의 커플을 해제 하시겠습니까?
    STR_ID_COUPLE_SYSTEM9					=3649,//@1님이 커플해제를 요청하였습니다.
    STR_ID_COUPLE_SYSTEM10					=3650,//사용할 커플링을 선택하세요.
    STR_ID_COUPLE_SYSTEM11					=3651,//커플이 아니라 커플창을 볼 수 없음.
    STR_ID_COUPLE_SYSTEM12					=3652,//@1님이 커플맺기를 거절했습니다.
    STR_ID_COUPLE_SYSTEM13					=3653,//커플링에 오류가 있습니다.
    STR_ID_COUPLE_SYSTEM14					=3654,//상대방이 서버에 없습니다.
    STR_ID_COUPLE_SYSTEM15					=3655,//오류가 발생하여 커플 신청에 실패했습니다.
    STR_ID_COUPLE_SYSTEM16					=3656,//현재 커플이 아닙니다. 커플링을 구입해서 커플 신청을 해 보세요.
    STR_ID_COUPLE_SYSTEM17					=3657,//이미 커플 인벤토리에 등록된 아이템 입니다.
    STR_ID_COUPLE_SYSTEM18					=3658,//커플 아이템 등록 오류 ( @1 )
    STR_ID_COUPLE_SYSTEM19					=3659,//커플 아이템이 등록되었습니다.
    STR_ID_COUPLE_SYSTEM20					=3660,//커플 아이템 장착 오류 ( @1 )
    STR_ID_COUPLE_SYSTEM21					=3661,//커플링 교체 오류 ( @1 )
    STR_ID_COUPLE_SYSTEM22					=3662,//사용할 커플링이 없습니다.\n먼저 상점에서 커플링을 구입하세요.
    STR_ID_COUPLE_SYSTEM23					=3663,//커플맺기는 광장에서만 할 수 있습니다.
    STR_ID_COUPLE_SYSTEM24					=3664,//상대방의 대답을 기다리는 중입니다.
    STR_ID_COUPLE_SYSTEM25					=3665,//오늘 하루 커플 신청을 받지 않습니다.
    STR_ID_COUPLE_SYSTEM26					=3666,//오늘 하루 이 유저에게 커플 신청을 거부합니다.
    STR_ID_COUPLE_SYSTEM27					=3667,//@1님은 오늘 하루 커플신청을 받지 않습니다.
    STR_ID_COUPLE_SYSTEM28					=3668,//반지교체 요청이 왔습니다.
    STR_ID_COUPLE_SYSTEM29					=3669,//커플효과
    STR_ID_COUPLE_SYSTEM30					=3670,//GP와 Exp추가 획득
    STR_ID_COUPLE_SYSTEM31					=3671,//커플기념일효과
    STR_ID_COUPLE_SYSTEM32					=3672,//커플링이 교체되었습니다.
    STR_ID_COUPLE_BTN_TOOLTIP				=3673,//커플
    STR_ID_EMOTICON_CMD53					=3674,//고
    STR_ID_EMOTICON_CMD54					=3675,//스탑
    STR_ID_COUPLE_SYSTEM33					=3676,//채팅창 켜기
    STR_ID_COUPLE_SYSTEM34					=3677,//채팅창 끄기
    STR_ID_COUPLE_SYSTEM35					=3678,//커플창 오른쪽 아래의 채팅창 켜기 버튼으로 지난 채팅을 볼 수 있습니다.
    STR_ID_COUPLE_SYSTEM36					=3679,//유저가 게임에 접속하지 않았습니다.
    STR_ID_COUPLE_SYSTEM37					=3680,//위치를 확인할 수 없습니다.
    STR_ID_COUPLE_SYSTEM38					=3681,//커플이 되고 @1일이 지나면 활성화 됩니다.
    STR_ID_COUPLE_SYSTEM39					=3682,//커플 @1 일째
    STR_ID_PET_COSTUME0						=3683,//펫 코스츔을 획득하려면?
    STR_ID_PET_COSTUME1						=3684,//펫 코스츔은 이벤트로 배포하거나 상점에서 구입 후 사용할 수 있습니다.
    STR_ID_PET_COSTUME2						=3685,//펫 코스츔을 입히려면 펫을 보유하고 있어야 합니다.
    STR_ID_WEEKLY_ATTENDANCE_REWARD         =3686,//주간 개근 보상

    STR_ID_COUPLE_SYSTEM40					=3687,//커플 상태에선 차단할 수 없습니다.

    STR_ID_COUPLE_EVENT_NOTIFY_CNT			=3688,
    STR_ID_COUPLE_EVENT_ADAPT				=3689,
    STR_ID_COUPLE_EVENT_EXIT				=3690,

    STR_ID_COUPLE_EVENT_CONDITION1_CNT		=3691,
    STR_ID_COUPLE_EVENT_CONDITION2_CNT		=3692,
    STR_ID_COUPLE_EVENT_CONDITION3_CNT		=3693,

    STR_ID_COUPLE_EVENT_NOTIFY_PTIME		=3694,
    STR_ID_COUPLE_EVENT_CONDITION1_PTIME	=3695,
    STR_ID_COUPLE_EVENT_CONDITION2_PTIME	=3696,
    STR_ID_COUPLE_EVENT_CONDITION3_PTIME	=3697,

    STR_ID_COUPLE_EVENT_CONDITION1_CNT2		=3698,
    STR_ID_COUPLE_EVENT_CONDITION2_CNT2		=3699,
    STR_ID_COUPLE_EVENT_CONDITION3_CNT2		=3700,
    STR_ID_PET_COSTUME3						=3701,//구매 버튼을 눌러서 구매하신 후에 입힐 수 있습니다.

    STR_ID_GS_BAHIA		            =3702,//바히아 ~~ 맵
    
    STR_ID_SQUARE_PET_SPEECH1		=3703,	//펫 레벨이 특정 수치 이상이 되면 진화할 수 있어요~~ 
    STR_ID_SQUARE_PET_SPEECH2		=3704,	//더 성장시키고 싶으시면 펫 진화 마법서로 진화시켜주면 돼요~
    STR_ID_SQUARE_PET_SPEECH3		=3705,  //나를 가져봐~ 나를 가져봐~~ 여기에선 획득할 수 없지만 상점 팻샵에서 구매획득이 가능하지~
    STR_ID_SQUARE_PET_SPEECH4		=3706,	//성장한 모습이 마음에 들지 않으면 퇴화마법서를 사용해서 진화되기 전 그대로 돌아갈 수 있어요~
    STR_ID_SQUARE_PET_SPEECH5		=3707,	//사랑스런 아기 천사 큐피트에요~. 상점 펫샵에서 구매해서 저를 키워주세요. 당신의 수호천사가 되어줄께요.
    STR_ID_SQUARE_PET_SPEECH6		=3708,	//1차 진화한 난 '루나파이터' 라고 불러~ 많이 성숙하고 예쁘지♡ 내가 너의 힘이 되어줄게 아웅~ 우선 1차 진화한 루나를 얻기 위해선 펫 레벨이 특정 수치 이상이 되어야 진화할 수 있어요 
    STR_ID_CANNOT_EXIT_YET			=3709,//아직 나갈수 없습니다.
    STR_ID_GMC_CAPTAIN				=3710,// 대장전
    STR_ID_NOT_START_LESSTHEN_4		=3711,// 대장전에서는 4명 미만의 플레이어로 시작할 수 없다능~
    STR_ID_CAPTAIN_DEBUFF			=3712,// 대장 디버프에연
    STR_ID_CAPTAIN_BUFF				=3713,// 대장 버프에연
    STR_ID_JIN						=3714,// 진
    STR_ID_JIN_COMBO				=3715,// 기본콤보
    STR_ID_JIN_CRITICAL_ATK			=3716,// 크리티컬공격
    STR_ID_JIN_EVASION				=3717,// 회피
    STR_ID_JIN_DASH					=3718,// 대쉬
    STR_ID_JIN_DASH_ATK				=3719,// 대쉬공격
    STR_ID_JIN_DASH_COMBO_ATK		=3720,// 대쉬콤보
    STR_ID_JIN_BURNNING_CHARGE		=3721,// 버닝차지
    STR_ID_JIN_BURNNING_ACTIVE		=3722,// 버닝모드
    STR_ID_JIN_SEE_BURNNING_SKILL	=3723,// 버닝기술 사용법 보기
    STR_ID_JIN_SPECIAL1				=3724,// 필살기1
    STR_ID_JIN_SPECIAL2				=3725,// 필살기2
    STR_ID_JIN_SPECIAL3				=3726,// 필살기3
    STR_ID_JIN_BURNNING_MODE		=3727,// 진(버닝모드)
    STR_ID_JIN_BURNNING_HADOKEN		=3728,// 기공파
    STR_ID_JIN_BURNNING_SPINKICK	=3729,// 열풍각
    STR_ID_JIN_BURNNING_HUJIN		=3730,// 붕권
    STR_ID_JIN_BURNNING_SPECIAL1	=3731,// 버닝 필살기1
    STR_ID_JIN_BURNNING_SPECIAL1_2	=3732,// 버닝 필살기1_1
    STR_ID_JIN_BURNNING_SPECIAL2	=3733,// 버닝 필살기2
    STR_ID_JIN_BURNNING_SPECIAL2_2	=3734,// 버닝 필살기2_2
    STR_ID_JIN_BURNNING_SPECIAL3	=3735,// 버닝 필살기3
    STR_ID_JIN_BURNNING_SPECIAL3_2	=3736,// 버닝 필살기3_2
    STR_ID_JIN_SEE_NORMAL_SKILL		=3737,// 일반기술 사용법 보기
    STR_ID_JIN_DESCRIPTION			=3738,// 버닝기술설명
    STR_ID_JIN_NORMAL				=3739,// 진(격투가)
    STR_ID_CONTRIBUTION_RANKER		=3740,// @1( @2 @3 )
    STR_ID_CONTRIBUTION_MIN_GP		=3741,// 최소 @1@2 이상 기부 가능.
    STR_ID_CONTRIBUTION_ERROR01		=3742,// 현재는 기부 기간이 아닙니다.	
    STR_ID_CONTRIBUTION_ERROR02		=3743,// 보유 금액이 기부 금액 보다 적습니다.
    STR_ID_CONTRIBUTION_ERROR03		=3744,// 기부에 실패하셨습니다.
    STR_ID_CONTRIBUTION_ERROR04		=3745,// 잠시 후에 다시 시도해 주세요.
    STR_ID_CONTRIBUTION_EVENT_NAME		=3746,// @1 GP : 경험치/GP +100%
    STR_ID_CONTRIBUTION_EVENT_DESC1		=3747,// 23일부터 1월 7일까지 경험치와 GP가 100%상승합니다.
    STR_ID_CONTRIBUTION_EVENT_NAME2		=3748,// @1 GP : 강화율상승 이벤트
    STR_ID_CONTRIBUTION_EVENT_DESC2		=3749,// 23일부터 30일까지 강화율이 대폭 상승합니다.
    STR_ID_CONTRIBUTION_EVENT_NAME3		=3750,// @1 GP : 기부 천사의 날개 
    STR_ID_CONTRIBUTION_EVENT_DESC3		=3751,// 모든 참가자분들에게 기부 천사의 날개를 무제한으로 지급해 드립니다.
    STR_ID_CONTRIBUTION_EVENT_NAME4		=3752,// @1 GP : 캐쉬백 이벤트
    STR_ID_CONTRIBUTION_EVENT_DESC4		=3753,// 여러분들에게 무료로 케쉬 포인트를 얻을 수 있는 기회를 드립니다.
    STR_ID_CONTRIBUTION_EVENT_NAME5		=3754,// @1 GP : 1000만원 기부
    STR_ID_CONTRIBUTION_EVENT_DESC5		=3755,// 그랜드 체이스 용사들이 불우한 이웃들을 위해 사회에 1000만원을 기부합니다.
    STR_ID_GC_GM_QUEST23			=3756,// 성난 엔트의 숲
    STR_ID_GC_GM_QUEST24			=3757,// 실버나이츠의 폐허
    STR_ID_BUFF_THANKSGIVING        =3758,// 추수감사절 이벤트
    STR_ID_KELBY_NAME               =3759,
    STR_ID_KELBY_MENT1              =3760,
    STR_ID_KELBY_MENT2              =3761,
    STR_ID_KELBY_MENT3              =3762,
    STR_ID_KELBY_MENT4              =3763,
    STR_ID_KELBY_MENT5              =3764,
    STR_ID_KELBY_MENT6              =3765,
    STR_ID_KELBY_MENT7              =3766,
    STR_ID_KELBY_MENT8              =3767,
    STR_ID_CONTRIBUTION_ERROR05     =3768, // 최소 @1@2 이상 기부하셔야 합니다.
    STR_ID_BEAST_MOSQUITO			= 3769,//	비스트모스키토
    STR_ID_POISON_ENT				= 3770,//	독거미
    STR_ID_ENT_MINIMI				= 3771,//	엔트미니미
    STR_ID_DETERIORATION_ENT		= 3772,//	변질된 엔트
    STR_ID_ANCIENT_ENT				= 3773,//	에이션트 엔트
    STR_ID_FIRE_SPIDER				= 3774,//	불거미
    STR_ID_SKELETON_FIGHTER			= 3775,//	죽은 격투가
    STR_ID_QUEST23_DRAMA_START			= 3776,
    /// BLAH BLAH BLAH
    STR_ID_QUEST23_DRAMA_END			= 3843,
    STR_ID_SERVER_EVENT_STATIC1             =3844,
    STR_ID_SERVER_EVENT_STATIC2             =3845,
    STR_ID_SERVER_EVENT_STATIC3             =3846,

    STR_ID_BUFF_EXCEPT_CHAR                 =3847,
    STR_ID_BUFF_ESPECAIL_DUNGEON            =3848,

    STR_ID_CONTRIBUTION_NPC_MENT1			=3849,
    STR_ID_CONTRIBUTION_NPC_MENT2			=3850,
    STR_ID_CONTRIBUTION_NPC_MENT3			=3851,

    STR_ID_QUEST23_DRAMA_START2				=3852,
    /// BLAH BLAH BLAH
    STR_ID_QUEST23_DRAMA_END2				=3864,

    STR_ID_CAPTAIN_DESC1					=3865,
    STR_ID_CAPTAIN_DESC2					=3866,

    STR_ID_MESSENGER_FRIEND_LOGOUT			= 3867, // [**]님이 로그인 하셨습니다.

    STR_ID_PET_STRING1						=3869,
    STR_ID_PET_STRING2						=3870,
    STR_ID_PET_STRING3						=3871,
    STR_ID_PET_STRING4						=3872,
    STR_ID_PET_STRING5						=3873,
    STR_ID_PET_STRING6						=3874,
    STR_ID_PET_STRING7						=3875,
    STR_ID_PET_STRING8						=3876,
    STR_ID_PET_STRING9						=3877,
    STR_ID_PET_STRING10						=3878,
    STR_ID_PET_STRING11						=3879,
    STR_ID_PET_STRING12						=3880,
    STR_ID_PET_STRING13						=3881,
    STR_ID_PET_STRING14						=3882,
    STR_ID_PET_STRING15						=3883,
    STR_ID_PET_STRING16						=3884,
    STR_ID_PET_STRING17						=3885,
    STR_ID_PET_STRING18						=3886,
    STR_ID_PET_STRING19						=3887,		// 켈비 스트링
    STR_ID_PET_STRING20						=3888,
    STR_ID_PET_STRING21						=3889,
    STR_ID_PET_STRING22						=3890,
    STR_ID_PET_STRING23						=3891,
    STR_ID_PET_STRING24						=3892,
    STR_ID_PET_STRING25						=3893,
    STR_ID_PET_STRING26						=3894,
    STR_ID_PET_STRING27						=3895,

    STR_ID_JIN1_NORMAL_SKILL				=3896,	//진(태사자-일반스킬)
    STR_ID_JIN1_COMBO						=3897,	//기본콤보
    STR_ID_JIN1_AERIAL_COMBO				=3898,	//공중콤보
    STR_ID_JIN1_SUMMERSALT					=3899,	//언월각
    STR_ID_JIN1_EVASION						=3900,	//회피
    STR_ID_JIN1_DASH_ATK					=3901,	//대쉬공격
    STR_ID_JIN1_DASH_EVASION				=3902,	//대쉬회피
    STR_ID_JIN1_AERIAL_STEP					=3903,	//공중밟기
    STR_ID_JIN1_BURNNING					=3904,	//버닝
    STR_ID_JIN1_BURNNING_MODE				=3905,	//버닝모드
    STR_ID_JIN1_CHULSANGO					=3906,	//철산고
    STR_ID_JIN1_HUJIN						=3907,	//질풍권
    STR_ID_JIN1_BISANKWON					=3908,	//비상권
    STR_ID_JIN1_BURNNING_SKILL				=3909,	//진(태사자-버닝스킬)

    STR_HAPPY_NEWYEAR_NOT_EVENT_TIME		=3910,
    STR_HAPPY_NEWYEAR_ENOUGH_TODAY			=3911,
    STR_COMMON_ERROR_P1_MSG					=3912,
    STR_HAPPY_NEWYEAR_RECIVE_GP				=3913,
    STR_HAPPY_NEWYEAR_STRING1				=3914,
    STR_HAPPY_NEWYEAR_STRING2				=3915,
    STR_HAPPY_NEWYEAR_STRING3				=3916,
    STR_HAPPY_NEWYEAR_STRING4				=3917,
    STR_HAPPY_NEWYEAR_STRING5				=3918,
    STR_HAPPY_NEWYEAR_STRING6				=3919,
    STR_HAPPY_NEWYEAR_STRING7				=3920,
    STR_HAPPY_NEWYEAR_STRING8				=3921,

    STR_ID_VIRTUALCASH_MSG					=3922,
    STR_ID_USER_GUIDE31						=3923,

    STR_ID_NEW_STRING1						=3924,
    STR_ID_NEW_STRING2						=3925,
    STR_ID_NEW_STRING3						=3926,
    STR_ID_SKILLTREE_TOOLTIP				=3927,

    STR_ID_DELETE_MONSTER_MOTION_ERROR		=3928,

    STR_ID_VALENTINE_DAY_ELESIS_SPEECH		=3929,
    STR_ID_VALENTINE_DAY_LIRE_SPEECH		=3930,
    STR_ID_VALENTINE_DAY_ARME_SPEECH		=3931,
    STR_ID_VALENTINE_DAY_LAS_SPEECH			=3932,
    STR_ID_VALENTINE_DAY_RYAN_SPEECH		=3933,
    STR_ID_VALENTINE_DAY_RONAN_SPEECH		=3934,
    STR_ID_VALENTINE_DAY_AMY_SPEECH			=3935,
    STR_ID_VALENTINE_DAY_JIN_SPEECH			=3936,

    STR_ID_VALENTINE_DAY_ERROR1				=3937,//인기 투표 기간이 아닙니다.
    STR_ID_VALENTINE_DAY_ERROR2				=3938,//적용 되는 캐릭터가 아닙니다.
    STR_ID_VALENTINE_DAY_ERROR3				=3939,//투표용 아이템이 아닙니다.
    STR_ID_VALENTINE_DAY_ERROR4				=3940,//투표용 아이템을 1개 이상 사용해야 합니다.
    STR_ID_VALENTINE_DAY_ERROR5				=3941,//투표용 아이템을 보유 하고 있지 않습니다.
    STR_ID_VALENTINE_DAY_ERROR6				=3942,//투표용 아이템의 수량이 부족합니다.
    STR_ID_VALENTINE_DAY_SUCCESS			=3943,//@1에게 아이템을 @2개 선물하셨습니다.
    STR_ID_VALENTINE_DAY_DLG_TITLE			=3944,//@1에게 초콜릿 선물하기

    STR_ID_ATTACK_10PER_UP					=3945,//공격력 + 10%
    STR_ID_DEFFENCE_10PER_UP				=3946,//방어력 + 10%
    STR_ID_HP_10PER_UP						=3947,//생명력 + 10%
    STR_ID_VALENTINE_DAY_EMOTICON_CMD		=3948,//초코

    STR_ID_MONNAME_CASTER_LABSTER           =3949,//	캐스터 랍스터
    STR_ID_MONNAME_LAKE_MON                 =3950,//	호수 괴수
    STR_ID_MONNAME_ELEC_FISH                =3951,//	전기 아귀
    STR_ID_MONNAME_KRAKO                    =3952,//	크라코
    STR_ID_MONNAME_EMERALD_LABSTER          =3953,//	에메랄드 랍스터

    STR_ID_BUFF_THIEF_PENDANT       		=3954,	
    STR_ID_EVENT_DARK_ASSASIN    			=3955,
    STR_ID_RYAN_TRANSFORM         		    =3956,//스페이스 누르면 라이언 변신도해요 ㅋ 
    
    STR_ID_GC_GM_QUEST25                    =3963,// 미명의 호수
    STR_ID_GC_GM_QUEST26                    =3964,// 불던전 이름 미정

    STR_ID_COUPLE_SYSTEM41					=3965,

    STR_ID_SEAL_LEVEL_HIGH					=3966,
    STR_ID_SEAL_LEVEL_MIDDLE				=3967,
    STR_ID_SEAL_LEVEL_LOW					=3968,

    STR_ID_MONNAME_ROCHER                   =3969,
    STR_ID_MONNAME_KENKR                    =3970,
    STR_ID_MONNAME_MAGMON                   =3971,
    STR_ID_MONNAME_HAMMERMON                =3972,
    STR_ID_MONNAME_ZIG                      =3973,

    STR_ID_LIRE_MOVE_UP						=3974,	// 날쌘 엘프
    STR_ID_LIRE_MOVE_UP_DESC				=3975,	// 날쌘 엘프 설명
    STR_ID_LIRE_SHOT_UP						=3976,	// 속사의 명인
    STR_ID_LIRE_SHOT_UP_DESC				=3977,	// 속사의 명인 설명
    STR_ID_LIRE_MP_REGEN_UP					=3978,	// 정신력 향상
    STR_ID_LIRE_MP_REGEN_UP_DESC			=3979,	// 정신력 향상 설명
    STR_ID_COOLTIME_DESC					=3980,	// 날쌘 엘프

    STR_ID_MONNAME_SWAMP_FAIRY              =4043,
    STR_ID_MONNAME_SWAMP_PIXIE              =4044,
    STR_ID_MONNAME_SWAMP_GOLEM              =4045,
    STR_ID_MONNAME_SWAMP_ELEMENTAL          =4046,
    STR_ID_MONNAME_SWAMP_GOBLIN             =4047,
    STR_ID_MONNAME_SWAMP_NESSI              =4048,

    STR_ID_GC_GM_QUEST27					=4049, // 수룡의 늪

    STR_ID_EQUIP_SHORTCUT_ITEM              =4098,  // @1이(가) 장착 슬롯 [@2]에 장착되었습니다.
    STR_ID_SHORTCUT_ITEM_UN                 =4099,  // @1이(가) 장착 슬롯 [@2]에서 해제되었습니다.

    STR_ID_PVP_CONFERENCE_MSG0				=4124,
    STR_ID_PVP_CONFERENCE_MSG1				=4125,
    STR_ID_PVP_CONFERENCE_MSG2				=4126,

    STR_ID_PVP_CONFERENCE_CANT_EQUIP		=4127,
    
    STR_ID_JIN_EVENT_STRING1				=4128,
    STR_ID_JIN_EVENT_STRING2				=4129,
    STR_ID_JIN_EVENT_STRING3				=4130,
    STR_ID_JIN_EVENT_STRING4				=4131,
    STR_ID_JIN_EVENT_STRING5				=4132,

    STR_ID_JIN_EVENT_MSG					=4133,
    STR_ID_JIN_EVENT_ERROR_MSG				=4134,

    STR_ID_GP_ITEM_BUY_COMMON_ERROR_MSG		=4135,

    STR_ID_SKILL_LAS_MOVE_SPEEDUP			=4136,
    STR_ID_SKILL_LAS_HIDE					=4138,
    STR_ID_SKILL_LAS_HIDE_DESC				=4139,
    STR_ID_SKILL_LAS_STUN_SURIKEN			=4140,
    STR_ID_SKILL_LAS_STUN_SURIKEN_DESC		=4141,
    STR_ID_SKILL_LAS_KAKEMUSA				=4142,
    STR_ID_SKILL_LAS_KAKEMUSA_DESC			=4143,
    STR_ID_SKILL_LAS_SUPER_JUMP				=4144,
    STR_ID_FATAL_MATCH_ALL_WIN              =4145,
    STR_ID_BUFF_DESC_MP_REGEN               =4164,  // 정신력 회복 속도
    STR_ID_SKILL_AMY2_AMAZINGPAPER          =4165,

    // 추천
    STR_ID_ENTER_YOUR_NICKNAME              =4172, //   닉네임을 입력해주세요.	 	
    STR_ID_ENTER_FRIENDS_NICKNAME           =4173, //	함께할 친구의 닉네임을 입력해주세요.	 	
    STR_ID_DO_NOT_RECOMMEND                 =4174, //	친구 연계 하지않음	 	
    STR_ID_RECOMMEND_FRIEND_LATER           =4175, //	함께할 친구 나중에 등록하기	 	
    STR_ID_CAN_RECOMMEND_IN_SQUARE          =4176, //	광장에서 친구를 직접 선택하여 우클릭으로 [친구와 함께!!] 이벤트 신청을 할 수 있습니다.	 	
    STR_ID_CAN_RECOMMEND_IN_SQUARE2         =4177, //	[친구와 함께!!] 이벤트를 신청 하지않고 게임을 시작합니다.\n광장에서만 신청할 수 있게 됩니다.	 	
    STR_ID_WILL_YOU_RECOMMEND               =4178, //	@1 님에게 [친구와 함께!!] 이벤트를 신청합니다. 이벤트가 시작되면 45일 동안 해제할 수 없습니다. 신청하시겠습니까?	 	
    STR_ID_MAKE_RECOMMEND                   =4179, //	@1 님에게 [친구와 함께!!] 이벤트 신청이 완료되었습니다.	 	
    STR_ID_CAN_NOT_DELETE_RECOMMENDER       =4180, //	[친구와 함께!!] 함께하는 친구는 삭제 및 차단이 불가능 합니다.	 	
    STR_ID_REWARD_PYRAMID                   =4181, //	[친구와 함께!!]축하합니다!! @1 님의 출석일 @2일을 달성하셨습니다! 보상이 지급됩니다!	 	
    STR_ID_REWARD_ATTEND                    =4182, //	[친구와 함께!!] 70분 플레이 출석! 선물이 지급됩니다!	 	
    STR_ID_END_RECOMMEND_EVENT              =4183, //	[친구와 함께!!] 이벤트 기간이 만료되었습니다.\n친구와의 연계가 해제됩니다.	 	
    STR_ID_START_RECOMMEND_EVENT            =4184, //	@1 님과의 [친구와 함께!!] 이벤트가 시작됩니다.\n함께 플레이하면 혜택이 주어집니다.	 	
    STR_ID_WLONG_NICK                       =4185, //	[친구와 함께!!]잘못된 닉네임입니다. 다시 입력해주세요.	 	
    STR_ID_WILL_YOU_ACCEPT                  =4186, //	[친구와 함께] 이벤트를 수락하게 되면 45일 동안 해제할 수 없습니다. 수락하시겠습니까?	 	
    STR_ID_ALREADY_FULL_PYRAMID             =4187, //	@1 님께서는 친구 [친구와 함께!!]최대치인 12명과 이벤트 진행 중입니다. 다른 유저를 선택해주세요!	 	
    STR_ID_ATTEND_DAYS                      =4188, //	[친구와 함께!!]현재 @1 일 출석 중

    STR_ID_RECOMMEND_FRIEND					=4190, // 추천하기


    STR_ID_USERS_RECOMMEND_YOU              =4192, // @1명의 유저님께서 당신을 추천하였습니다.\n추천한 유저가 출석을 하면 보상이 지급됩니다.
    STR_ID_USERS_ATTEND                     =4193, // 현재 @1명의 추천한 유저가 출석을 한 상태입니다.
    STR_ID_DESC_EVENT_FRIEND                =4194,

    STR_ID_RECOMMEND_NOT                    =4195,  // 연계 안해

    STR_ID_CAN_NOT_ACCEPT_BECAUSE_TOO_MANY  =4196,// 너무많아서 추천 못해
    STR_ID_GACHA_AUTO1						=4197,	//자동 해제
    STR_ID_GACHA_AUTO2						=4198,	//체크를 풀기 전까지 구슬을 자동으로 봉인해제 합니다.
    STR_ID_GACHA_AUTO3						=4199,	//자동 해제를 시작하려고 합니다. 계속 진행 하시겠습니까?
    STR_ID_GACHA_AUTO4						=4200,	//모든 세트 아이템을 획득하여 자동 정지 되었습니다.
    STR_ID_GACHA_AUTO5						=4201,	//중단

    STR_ID_EXPACCOUNT_CANT_NOT_RECOMMEND	=4202, //체험 계정은 다른 유저를 추천할 수 없습니다.
    STR_ID_PET_AGGRESIVE					=4204,	//펫 광폭화
    STR_ID_PET_AGGRESIVE_DESC				=4205,	//공격력 x2
    STR_ID_JIN2								=4206,	//아수라
    STR_ID_JIN2_NORMAL_SKILL				=4207,	//진(아수라-일반스킬)
    STR_ID_JIN2_DAMA						=4208,	//연환포
    STR_ID_JIN2_DASHBACK_ATK				=4209,	//선풍연추
    STR_ID_JIN2_JANGPUNG					=4210,	//질풍권
    STR_ID_JIN2_THROWBONG					=4211,	//참마곤 던지기
    STR_ID_JIN2_THROWRUSH					=4212,	//돌진
    STR_ID_JIN2_SUMMON_BONG					=4213,	//참마곤 쥐기
    STR_ID_JIN2_JUMPUP_ATK					=4214,	//연환퇴
    STR_ID_JIN2_JUMPDOWN_ATK				=4215,	//내려찍기
    STR_ID_JIN2_BURNNING_SKILL				=4216,	//진(아수라-버닝스킬)
    STR_ID_JIN2_DASH_ATK					=4217,	//대쉬공격
    STR_ID_JIN2_SUMMON_BONG_DESC			=4218,	//참마곤 쥐기

    

    STR_ID_SIEG_GLADIATOR                   =4219,
    STR_ID_SIEG_NAME                        =4220,
    STR_ID_SIEG_FURY_MODE                   =4221,
    STR_ID_SIEG_FURY_MODE2                  =4222,
    STR_ID_SIEG_DRAMA1                      =4223, //▶ 지크하트 : 고블린 조무래기들…사람 귀찮게 하는 건 옛날이나 지금이나 똑같군~~.
    STR_ID_SIEG_DRAMA2                      =4224, //▶ 엘리시스 : 뭐야~ 저 느슨하고 건방져 보이는 사람은! ...... 옳거니! 고블린의 두목이로군!!!
    STR_ID_SIEG_DRAMA3                      =4225, //▶ 리르 : 저기요… 고블린들 두목치곤 너무 미남형이란 생각들지 않나요?
    STR_ID_SIEG_DRAMA4                      =4226, //▶ 지크하트 : 내 이름은 지크하트. 카나반에선 전설로 불리우던 검투사다. 하긴 너희들이 날\n    알아 볼 리 없겠지만. 그랜드체이스 일행을 찾고 있는데 행방을 아는 사람 없나?
    STR_ID_SIEG_DRAMA5                      =4227, //▶ 아르메 : (지크하트… 어디서 들어본 듯한 이름이에요~ 엘리시스?)
    STR_ID_SIEG_DRAMA6                      =4228, //▶ 엘리시스 : … 웃기지마! 이 건방진 녀석이 감히 우리 선조님 이름을 들이대? 혼나볼래?!!!
    STR_ID_SIEG_DRAMA7                      =4229, //▶ 지크하트 : 하하. 빨간 머리 아가씨. 자네에게서 웬지 친근한 느낌이 전해지는 군.\n그랜드체이스를 만나면 꼭 날 만나러 제니아 대륙으로 와달라고 전해줘. 그럼 이만~
    STR_ID_SIEG_DRAMA8                      =4230, //▶ 리르 : 이봐요!!! 우리가 그랜드체이스…. 휴~ 정말 급한 사람이군. 저 사람에 대해선 다음에\n알아보기로 하구 지금 당장 이 몬스터 녀석들부터 청소하자구요~~





    STR_ID_SIEG2_DRAMA1                     =4231, //▶ 지크하트 : 어라~. 세르딘 외곽에서 만났던 꼬맹이들이구나. 
    STR_ID_SIEG2_DRAMA2                     =4232, //▶ 리르 : 앗! 맞다! 그 때 봤던 사람이네! 이봐요! 우리가 당신이 찾던 그랜드체이스 랍니다.\n엘리시스! 저 사람… 정말 당신의 선조인 지크하트 맞죠?
    STR_ID_SIEG2_DRAMA3                     =4233, //▶ 지크하트 : 지금 이 곳, 제니아 대륙은 암흑과 파괴의 힘으로 뒤덮여 온통 위험함 투성이지.\n그 힘이 베르메시아 대륙마저 침몰시킬 음모를 꾸미고 있다. 
    STR_ID_SIEG2_DRAMA4                     =4234, //▶ 엘리시스 : (…내 선조란 말엔 관심도 없는 것 같은데..) 그럼 당장 돌아가봐야지! 서둘러!
    STR_ID_SIEG2_DRAMA5                     =4235, //▶ 지크하트 : 하하하. 성질 급한 건 여전하군 빨간머리 아가씨. 자~ 베르메시아의 용사 여러분~\n너희들의 왕국을 위해 내가 직접 이 곳의 모험을 함께 해주도록 하지.
    STR_ID_SIEG2_DRAMA6                     =4236, //▶ 아르메 : 와~ 낯선 곳이라 얼떨떨했는데 아르메는 웬지 저분이 너무너무 듬직해요~~
    STR_ID_SIEG2_DRAMA7                     =4237, //▶ 지크하트 : 난 먼저 여길 나갈테니 반드시 살아서 오라구~. 너희들의 실력을 기대해보겠어~\n특히 너. 빨간 머리. 선조의 기대를 저버리지 말거라~
    STR_ID_SIEG2_DRAMA8                     =4238, //▶ 엘리시스 : 으와아아아!!! 열받네!! 누가 받아준대!! 자기 멋대로 어쩌고 어째!!! 에잇!! 이 녀석들\n부터 모두 쓸어버리겠어!!!!
    STR_ID_BEANFUN_ERROR0					=4239, // 게임 클라이언트를 닫으시고, BeanFun 통해서 ..
    STR_ID_BEANFUN_ERROR1				    =4240,	//게임 클라이언트를 닫으시고, 게쉬 계정을 업그레이드한후Bean Fun통해서 접속해주세요. 사이트tw.beanfun.gamania.com.

    STR_ID_SIEG_SP3_EX_EX                   =4241,
    

    STR_ID_GP_CAPSULE_NOT_ENOUGH_GP			=4252,//GP가 부족함
    STR_ID_GP_CAPSULE_FULL_INVENTORY		=4253,//인벤공간 부족함
    STR_ID_EMPTY_GP_CAPSULE					=4254,//비어있는 캡슐
    STR_ID_GP_CAPSULE_DESC					=4255,//캡슐설명

    STR_ID_COUNT							=4256,//개
    STR_ID_GCCLUB_CONNECT_ITEM				=4257,	//GC 클럽 보너스 아아이템을 받았습니다.

    STR_ID_SKILL_RYAN_HP_TOTEM				=4267, //체력 회복 토템 폴
    STR_ID_SKILL_RYAN_MP_TOTEM				=4268, //마나 회복 토템 폴
    STR_ID_SKILL_RYAN_DP_TOTEM				=4269, //방어력 상승 토템 폴
    STR_ID_SKILL_RYAN_						=4270, //방어력 상승
    STR_ID_SKILL_RYAN_NATURE_COMMANDER		=4271, //네이쳐 커맨더
    STR_ID_SKILL_RYAN_LEOPARD				=4272, //표범의 질주
    STR_ID_SKILL_RYAN_STRONGLY_LIFE			=4273, //끈질긴 생명력
    STR_ID_SKILL_RYAN_RAMDOM_RESER			=4274, //일정 확률로 부활
    STR_ID_SKILL_RYAN_WILD_HOWL				=4275, //야성의 포효
    STR_ID_SKILL_RYAN_NATURE_SHIELD			=4276, //자연의 방패
    STR_ID_SKILL_RYAN_STONE_WALL			=4277, //돌벽 세우기
    STR_ID_SKILL_RYAN_DASH_CANCEL			=4278, //대쉬 캔슬   : 대쉬 공격 중 ← 또는 →

    STR_ID_GIVE_ITEM_DLG_TITLE				=4279,//
    STR_ID_GIVE_ITEM_DLG_DESC				=4280,//
    STR_ID_BUY_CASH_BOX01					=4281,	//▶ 가격 :
    STR_ID_BUY_CASH_BOX02					=4282,	//▶ 내돈
    STR_ID_BUY_CASH_BOX03					=4283,	//▶ 가상
    STR_ID_BUY_CASH_BOX04					=4284,	//▶ 가격
    STR_ID_BUY_CASH_BOX05					=4285,	//▶ 가상캐쉬
    STR_ID_BUY_CASH_BOX06					=4286,	//▶ CASH
    STR_ID_BUY_CASH_BOX07					=4287,	//▶ @1 CASH
    STR_ID_BUY_CASH_BOX08					=4288,	//▶ 가상 CASH 사용
    STR_ID_BUY_CASH_BOX09					=4289,	//▶ 가상 CASH 섞어 쓰기

    STR_ID_LOGIN_ERROR_MESSAGE				=4290,//패치중에는 접속할 수 없음

    STR_ID_LOGIN_ERROR_MESSAGE2				=4291,//웹인증실패

    STR_ID_RECOMMEND_LIST_TOLL_TIP			=4294,//추천인리스트
    STR_ID_RECOMMEND_LIST_WINDOW			=4295,//추천받은사람
    STR_ID_RECOMMEND_REJECT					=4296,//[친구와 함께!!] 신청을 거절당했음
    STR_ID_NOT_IN_TEAM						=4298,//현재 팀에 속해있지 않습니다.
    STR_ID_TEAM								=4299,//팀
    STR_ID_DUGEON_HACKING_CHECK				=4300,//해킹감지

    STR_ID_RECOMMEND_ERROR_MESSAGE4			=4310,//db처리 오류

    STR_ID_RECOMMEND_REWARD_MSG				=4324,//[친구와 함께!!] 추천보상메시지

    STR_ID_COUPON_NPC_TOOLTIP				=4333, 
    STR_ID_COUPON_NPC_EVENT_END				=4334, 
    STR_ID_COUPON_NPC_INCORRECT_COUPON		=4335, 
    STR_ID_COUPON_NPC_INPUT_SUCCESS			=4336, 
    STR_ID_COUPON_NPC_DESC					=4337, 
    STR_ID_COUPON_NP_OVER_INCORRECT_INPUT	=4338, 

    STR_ID_AMY4_NAME_MELO	                =4339,//	시스티나 (멜로 모드)
    STR_ID_AMY4_NAME_ACTION	                =4340,//	시스티나 (액션 모드)
    STR_ID_AMY4_BASIC_COMBO	                =4341,//	기본 콤보      : Z Z Z Z Z
    STR_ID_AMY4_SWAP_TO_ACTION	            =4342,//	액션 모드 전환 : X
    STR_ID_AMY4_SWAP_TO_MELO	            =4343,//	멜로 모드 전환 : X
    STR_ID_AMY4_CRITICAL_ATK	            =4344,//	크리티컬 공격  : 기본 콤보 중 →→ 또는 ←←
    STR_ID_AMY4_DOUBLE_JUMP	                =4345,//	더블 점프      : ↑ ↑
    STR_ID_AMY4_DASH_ATK	                =4346,//	대쉬 공격      : 대쉬중 Z Z
    STR_ID_AMY4_DASH_COMBO	                =4347,//	대쉬 콤보      : 대쉬중 Z ↑ Z
    STR_ID_AMY4_CANNON	                    =4348,//	시스터 캐논    : Z → 또는 Z ←
    STR_ID_AMY4_MACHINE_GUN	                =4349,//	시스터 머신건  : 점프 중 ↓ Z
    
    STR_ID_CANT_BUY_IN_ROOM                 =4350,//    방안에서는 아이템을 구매할 수 없습니다.
    
    STR_ID_CLOSE_CLIENT_DOUBLE_CONNECT       =4351,  // 이중접속이 감지되었습니다.클라이언트를 종료합니다.
    
    STR_ID_TOURNAMENT_STR01					=4352,//토너먼트 서버에서 접속시 환영 메시지
    STR_ID_TOURNAMENT_STR02					=4353,//토너먼트 서버 룰 설명 메시지
    STR_ID_TOURNAMENT_STR03					=4354,//토너먼트 서버에서 장착할 수 없는 아이템 입니다.
    STR_ID_GACHA_AUTO1_1					=4395,//자동 해제2
    STR_ID_CLEAR_SEAL_COMMENT3				=4396,//구슬가챠추가스트링
    STR_ID_RAINBOWEVENT						= 4397,	// 4397	[ Rainbow 7 이벤트!! 오늘은 [ @1 ] 진행중입니다!! 자세한 내용은 홈페이지를 참고해주세요~! ]
    STR_ID_RAINBOW0							= 4398, // 던전 Exp 20% 추가 획득
    STR_ID_RAINBOW1							= 4399, // GP 30% 추가 획득
    STR_ID_RAINBOW2							= 4400, // 던전 내 아이템 드랍율 상승
    STR_ID_RAINBOW3							= 4401, // 룬 강화
    STR_ID_RAINBOW4							= 4402, // 챔피언 모드 자유 입장
    STR_ID_RAINBOW5							= 4403, // 대전 Exp 20% 추가 획득
    STR_ID_RAINBOW6							= 4404, // 목걸이 강화
    STR_ID_RAINBOW7							= 4405, // 무지개가 떴다
    STR_ID_RAINBOW_SQUARE					= 4406, // [무지개가 떴다!!] 이벤트가 발동하면 [소망의 무지개] 아이템을 지급합니다. [소망의 무지개] 아이템이 많을 수록 더 많은 추가 보상이 지급됩니다.
    STR_ID_RAINBOW_JACKPOT					= 4407, // 운영자공지 : [무지개가 떴다!!!] 이벤트가 발동되었습니다!! 7가지 이벤트가 모두 적용됩니다!!
    STR_ID_RAINBOW_BUFF						= 4408, // Rainbow 7


    STR_ID_XTRAP_PATCH						= 4412, // Xtrap 버전이 맞지 않습니다. 다시 패치 받은 후 실행해 주세요.

    STR_SIEG2_JOB_NAME						= 4413, // 
    STR_SIEG2_TUTORIAL_DASH_UP				= 4414, //    
    STR_SIEG2_TUTORIAL_TUMBLING             = 4415, //
    STR_SIEG2_TUTORIAL_JUMP_DOWN			= 4416, //
    STR_SIEG2_TUTORIAL_TUMBLING_COMBO       = 4417, //
    STR_SIEG2_TUTORIAL_TUMBLING_UPPER       = 4418, //
    STR_ID_EXP_GP_DOUBLE_BONUS              = 4419, //2 x EXP/GP

    STR_ID_XENIA5_DRAMA1                    =4420, //▶ 엘리시스: 으으으~ 추워 추워! 온몸이 얼어버릴 것만 같잖아!
    STR_ID_XENIA5_DRAMA2                    =4421, //▶ 아르메: 제가 파이어볼로 녹여드릴까요?
    STR_ID_XENIA5_DRAMA3                    =4422, //▶ 엘리시스: ...;;
    STR_ID_XENIA5_DRAMA4                    =4423, //▶ 아르메: 그나저나 이 곳은 엄청난 마력에 의해 모두 얼어버린 것 같아요.
    STR_ID_XENIA5_DRAMA5                    =4424, //▶ 로난: 게다가 땅에 발을 너무 오래 딛고 있을 경우, 동상 데미지를 입을 수 있으니 수시로 점프를 해야 할 것 같습니다.
    STR_ID_XENIA5_DRAMA6                    =4425, //▶ 라이언: 지금도 얼어붙고 있는 지역이 계속해서 확장되고 있어. 한시라도 빨리 조율의 신을 만나봐야 해.
    STR_ID_XENIA5_DRAMA7                    =4426, //▶ 엘리시스: 오케이~ 그럼 빨리 출발하자고!
    STR_ID_XENIA5_DRAMA8                    =4427, //▶ 샤크아론: 누구냐! 감히 허락도 없이 제단으로 침입하려 하다니!
    STR_ID_XENIA5_DRAMA9                    =4428, //▶ 로난: 저희는 조율의 신을 만나 뵙기 위해 이 곳을 찾은 것뿐입니다. 나쁜 뜻은 없습니다.
    STR_ID_XENIA5_DRAMA10                   =4429, //▶ 샤크아론: 감히 너희가 만나고 싶다고 만날 수 있는 분이 아니다. 얼마 전 신께서는 아무도 근처에 오지 못하도록 지시를 내리셨다.
    STR_ID_XENIA5_DRAMA11                   =4430, //▶ 엘리시스: 보아하니 이번에도 일단은 힘으로 밀어 부쳐야 하겠군.
    STR_ID_XENIA5_DRAMA12                   =4431, //▶ 조율의 신: 누구냐. 감히 나의 신성한 조율을 방해하다니!
    STR_ID_XENIA5_DRAMA13                   =4432, //▶ 로난: 저희는 그저 도움을 요청 드리러 왔습니다. 부디 저희의 이야기를 들어주십시오.
    STR_ID_XENIA5_DRAMA14                   =4433, //▶ 조율의 신: 이야기는 너희가 살아남을 수 있다면 그때 들어보도록 하겠다. 자 받아라!
    STR_ID_XENIA5_DRAMA15                   =4434, //▶ 지크하트: 모두 조심해! 특히 저 천칭의 움직임을 주시하라고!
    STR_ID_XENIA5_DRAMA16                   =4435, //▶ 조율의 신: 지배의 마력을 해제해 주셔서 감사합니다. 조금 전 행동을 저의 신중치 못한 행동이었습니다.
    STR_ID_XENIA5_DRAMA17                   =4436, //▶ 로난: 당신의 의지가 아니었다는 것을 알고 있습니다. 괜찮습니다.
    STR_ID_XENIA5_DRAMA18                   =4437, //▶ 로난: 그보다 아까 전에 이야기하려고 했던..
    STR_ID_XENIA5_DRAMA19                   =4438, //▶ 조율의 신: 알고 있습니다. 정수를 얻기 위해서지요.
    STR_ID_XENIA5_DRAMA20                   =4439, //▶ 엘리시스: 와.. 역시 신이시라 대단하시군요. 그럼 그 정수를 저희에게 주실 수 있나요?
    STR_ID_XENIA5_DRAMA21                   =4440, //▶ 조율의 신: 물론 드리겠습니다. 지금 이 곳도 이미 회복하기 힘들 정도로 많은 피해를 입었습니다. 지배의 신이 계속해서 횡포를 부리도록 놔둘 수는 없지요.
    STR_ID_XENIA5_DRAMA22                   =4441, //▶ 리르: 감사합니다. 이로서 세 번째 정수도 모으게 되었네요.
    STR_ID_XENIA5_DRAMA23                   =4442, //▶ 조율의 신: 그랜드 체이스 용사들이여 그대들은 강합니다. 하지만 그 강함은 그대들이 하나됨에 있음을 잊지 마시기 바랍니다.

    STR_ID_SILVERLAND5_DRAMA1               =4443, //▶ 엘리시스: 여기가 바로 실버랜드 최고의 지상낙원인 원시의 섬이란 말이지!?
    STR_ID_SILVERLAND5_DRAMA2               =4444, //▶ 진: 뭔가 이상해. 예전 같지 않은 불길한 느낌이..
    STR_ID_SILVERLAND5_DRAMA3               =4445, //▶ 라이언: 나도 이 곳의 생물체들에게서 나쁜 기운이 느껴져.
    STR_ID_SILVERLAND5_DRAMA4               =4446, //▶ 로난: 모두 조심하세요. 주변의 모든 곤충과 식물들이 우리를 공격하려 하고 있어요!
    STR_ID_SILVERLAND5_DRAMA5               =4447, //▶ 진: 이 곳마저도 이미 누군가에 의해 오염되어버리다니! 용서할 수 없어!
    STR_ID_SILVERLAND5_DRAMA6               =4448, //▶ 로난: 아무래도 이 곳을 자세히 조사해볼 필요가 있을 것 같군요.
    STR_ID_SILVERLAND5_DRAMA7               =4449, //▶ 엘리시스: 자 그렇다면 빨리 모두 쓸어버리고 전진~!
    STR_ID_SILVERLAND5_DRAMA8               =4450, //▶ 리르: 저기 엄청나게 큰 꽃이 있어요.
    STR_ID_SILVERLAND5_DRAMA9               =4451, //▶ 아르메: 조심하세요! 저건 그냥 꽃이 아니라, 이미 몬스터화 된 돌연변이 같아요.
    STR_ID_SILVERLAND5_DRAMA10              =4452, //▶ 알뜨와네트: 모두 가까이 와서 나의 향기를 맡아보아라. 맛있게 생긴 것들 크크크..
    STR_ID_SILVERLAND5_DRAMA11              =4453, //▶ 진: 이 곳을 더럽히는 게 너냐? 용서하지 않아!
    STR_ID_SILVERLAND5_DRAMA12              =4454, //▶ 엘리시스: 이번엔 말하는 잡초를 상대할 차례인가. 모두 준비!
    STR_ID_SILVERLAND5_DRAMA13              =4455, //▶ 로난: 이 녀석을 처리하긴 했지만 오염의 주범은 아닌 것 같습니다.
    STR_ID_SILVERLAND5_DRAMA14              =4456, //▶ 진: 이 녀석도 단지 오염되어 이렇게 변한 것뿐이군. 도대체 누가 이런 짓을 벌이고 있는 거지.
    STR_ID_SILVERLAND5_DRAMA15              =4457, //▶ 엘리시스: 너무 조급해하지마. 이 못된 짓을 하는 녀석을 찾을 때까지 우린 절대 포기하지 않을 거야.
    STR_ID_SILVERLAND5_DRAMA16              =4458, //▶ 리르: 맞아요! 우린 절대 포기하지 않아요. 힘내자는 의미에서 오랜만에 다 같이 파이팅 어때요? 
    STR_ID_SILVERLAND5_DRAMA17              =4459, //▶ 엘리시스: 좋아 좋아! 내가 셀 거야! 하나.. 둘.. 셋! 파이팅~!
    STR_ID_SILVERLAND5_DRAMA18              =4460, //▶ 라스: ...

    STR_ID_MONNAME_POISONMOSQUITTO          =4461,
    STR_ID_MONNAME_PLANT_MINI               =4462,
    STR_ID_MONNAME_POISONSPIDER_QUEEN       =4463,
    STR_ID_MONNAME_POISONSPIDER_MINI        =4464,
    STR_ID_MONNAME_BLACKWENDY               =4465,
    STR_ID_MONNAME_ANTOINETTE               =4466,
    STR_ID_MONNAME_FREEZING_SABERTOOTH      =4467,
    STR_ID_MONNAME_FREEZING_KENKRE          =4468,
    STR_ID_MONNAME_ENDARION                 =4469,
    STR_ID_MONNAME_SHARKARON                =4470,
    STR_ID_MONNAME_JURIOR                   =4471,
    STR_ID_BUY_CASH_BOX10					=4472,	//▶ 가상 CASH 섞어 쓰기 혼용비율

    STR_ID_GP_CAPSULE_NOT_ENOUGH_CRYSTAL	= 4473, // 크리스탈이 부족함
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_01		= 4474, // 해당캡슐정보없음
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_03		= 4475, // 가격정보다름
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_04		= 4476, // 아이템등급확률이상
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_05		= 4477, // 꽝!!!
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_06		= 4478, // 잘못된 레이턴시
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_07		= 4479, // 없는 캡슐 아이템 등급
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_08		= 4480, // 캡슐아이템 얻기 DB작업실패
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_09		= 4481, // 크리스탈 아이템 가져오기 실패
    STR_ID_GP_CAPSULE_ERROR_MESSAGE_11		= 4482, // 캡슐 화폐 정보가 잘못 되었음
    STR_ID_XENIA5_DESC1						=4483, // 발이 땅에 오래 닿을 경우 동상데미지를 입게 됩니다
    STR_ID_XENIA5_DESC2						=4484, // 점프를 하면 동상을 풀 수가 있습니다
    STR_ID_JURIOR_DESC1						=4485, // 저주의 조율이 시작되었습니다
    STR_ID_JURIOR_DESC2						=4486, // 죽음의 조율이 시작되었습니다
    STR_ID_JURIOR_BUFF_NAME_PLUS			=4487,
    STR_ID_JURIOR_BUFF_DESC_PLUS			=4488,
    STR_ID_JURIOR_BUFF_NAME_MINUS			=4489,
    STR_ID_JURIOR_BUFF_DESC_MINUS			=4490,

    STR_ID_GP_CAPSULE_DESC_CRYSTAL			= 4491, // 크리스탈용 설명
    STR_ID_COOPERATION_EVENT_ERROR1       = 4492, //제휴 이벤트 기간이 아닙니다.
    STR_ID_COOPERATION_EVENT_ERROR2       = 4493, //요청한 아이템은 이벤트 아이템이 아닙니다.
    STR_ID_COOPERATION_EVENT_ERROR3       = 4494, //화폐 아이템이 부족합니다.
    STR_ID_COOPERATION_EVENT_ERROR4       = 4495, //제휴 아이템 얻기 실패(쿼리 실패)
    STR_ID_COOPERATION_EVENT_ERROR5       = 4496, //제휴 아이템 얻기 실패(없는 아이템)
    STR_ID_COOPERATION_EVENT_MY_POINT       = 4497, //현재 보유 포인트 : @1
    STR_ID_COOPERATION_EVENT_SUBJECT        = 4498, //제휴 이벤트 주제
    STR_ID_COOPERATION_EVENT_DESC           = 4499, //제휴 이벤트 설명
    STR_ID_WRONG_COUPON_CODE                = 4500, //잘못된 코드 입니다.
    STR_ID_COOPERATION_EVENT_DESC_TOOPTIP  = 4501, //제휴 이벤트 설명 툴팁
    STR_ID_ELO_MATCH_MESSAGE_01				= 4502, // 대기방에서는 대화를 할 수 없습니다.
    STR_ID_GC_GM_QUEST28					= 4503,
    STR_ID_GC_GM_QUEST31					=4504,
    STR_ID_RESOLUTION                       = 4505, // 해상도

    STR_ID_DUGEON_HACKING_CHECK2			=4506,
    STR_ID_ELO_MATCH_MESSAGE_02				= 4507, // ELO모드에서 무단 이탈하게 되면 포인트가 2배로 감소합니다.\n대전을 종료하시겟습니까?



    STR_ID_SILVERLAND6_DESC1				=4508, // 파라돔이 폭탄 근처로 오는 것을 막아야 합니다!
    STR_ID_XENIA6_DESC1						=4509, // 수정을 작동시키면 점프장치를 생성할 수 있습니다.
    STR_ID_XENIA6_DESC2						=4510, // 2단 점프를 하면 장치가 사라질 수 있습니다.\n점프장치만을 이용해 올라가 보세요.
    STR_ID_XENIA6_DESC3						=4511, // 비밀수정을 작동하였습니다. 어딘가에서 커다란 진동음이 들려옵니다.
    STR_ID_SILVERLAND6_DRAMA1				=4512, // 아니! 이 곳에 왜 오크무리들이!
    STR_ID_SILVERLAND6_DRAMA2				=4513, // 이 몬스터들이 갑자기 나타난 것이라면 뭔가 심각한 일이 벌어지고 있는 것이 분명합니다.
    STR_ID_SILVERLAND6_DRAMA3				=4514, // 혹시 이번 사건들과 연관이 있는 것은 아닐까?
    STR_ID_SILVERLAND6_DRAMA4				=4515, // 하지만 이 지역은 드레이크 웜님이 통치하고 있어서, 이런 오크들 따위가 활개치도록 놔둘 리가 없었을 텐데.. 설마 드레이크 웜님에게 무슨 일이??
    STR_ID_SILVERLAND6_DRAMA5				=4516, // 들어보니 왠지 사태가 심상치 않은 것 같아요. 빨리 그분을 찾으러 가봐요.
    STR_ID_SILVERLAND6_DRAMA6				=4517, // 왠 소란이냐!
    STR_ID_SILVERLAND6_DRAMA7				=4518, // 타이타로스님. 조금 전부터 갑자기 나타난 인간무리들이 저희의 작업을 계속 방해하고 있습니다.
    STR_ID_SILVERLAND6_DRAMA8				=4519, // 그깟 인간들 따위 하나 처리를 못하다니, 쓸모 없는 것들. 내가 직접 처리를 해주지.
    STR_ID_SILVERLAND6_DRAMA9				=4520, // 보아하니 저 녀석이 이번에 이 지역을 침공한 오크무리의 대장인 것 같군요.
    STR_ID_SILVERLAND6_DRAMA10				=4521, // 그럼 저놈을 처리하면 나머지 멍청한 오크들은 아무 쓸모가 없어지겠군!
    STR_ID_SILVERLAND6_DRAMA11				=4522, // 빨리 처리하고 드레이크 웜님에게 가보도록 하죠. 하아앗!
    STR_ID_SILVERLAND6_DRAMA12				=4523, // 드디어 왔구나 나의 친구 하스를 죽인 자들이여. 감히 이번에는 나한테까지 덤비려 하다니, 하스의 원수를 갚아주마!
    STR_ID_SILVERLAND6_DRAMA13				=4524, // 잠깐 저희 이야기를 들어주세요. 저희는 이번 사건의 배후를 찾고 있는 중입니다.
    STR_ID_SILVERLAND6_DRAMA14				=4525, // 하스의 원수하고 이야기 따윈 필요 없다. 당장 목숨을 내놓아라!
    STR_ID_SILVERLAND6_DRAMA15				=4526, // 온다. 모두 조심해!
    STR_ID_SILVERLAND6_DRAMA16				=4527, // 내가 당하다니.. 하스를 볼 면목이 없구나..
    STR_ID_SILVERLAND6_DRAMA17				=4528, // 아닙니다. 하스님의 죽음은 저희 때문이 아닙니다. 그분께서는 실버랜드를 위해서 스스로 목숨을 끊으셨습니다.
    STR_ID_SILVERLAND6_DRAMA18				=4529, // 그건 무슨 소리지? 자세히 말해보아라.
    STR_ID_SILVERLAND6_DRAMA19				=4530, // (그 동안의 일들을 설명해 줍니다)
    STR_ID_SILVERLAND6_DRAMA20				=4531, // 그렇군. 역시 하스다운 행동이야. 미안하구나 인간들이여 나의 행동이 경솔했다.
    STR_ID_SILVERLAND6_DRAMA21				=4532, // 하지만 그대들이 나를 찾아오기 전에 하스를 죽인 것이 그대들이라고 알려준 자가 있다. 하스가 죽었다는 사실에만 분노한 나머지 내가 올바른 판단을 하지 못한 것 같구나.
    STR_ID_SILVERLAND6_DRAMA22				=4533, // 그자가 분명 이번 사건들의 배후와 관련이 있을 겁니다.
    STR_ID_SILVERLAND6_DRAMA23				=4534, // 걱정 마세요. 저희가 그 놈을 반드시 찾아내 혼내줄 태니까요!
    STR_ID_XENIA6_DRAMA1					=4535, // 와~ 뜨거운 곳 차가운 곳을 지나니 드디어 정상적인 지역으로 오게 됐네요~
    STR_ID_XENIA6_DRAMA2					=4536, // 굳이 비교하자면 정상적이긴 하지만, 이 곳은 이상할 정도로 건조한걸?
    STR_ID_XENIA6_DRAMA3					=4537, // 아까 전부터 무언가 이상하다고 생각하고 있었는데, 이 곳 산맥 전체에서 아주 미약하긴 하지만 신비한 힘이 느껴지는 것 같아요.
    STR_ID_XENIA6_DRAMA4					=4538, // 제가 들은 바로는 이 곳은 아주 먼 옛날, 지금의 신들이 있기 전부터 살았다던 고대 거인들의 묘지라고 들은 적이 있어요.
    STR_ID_XENIA6_DRAMA5					=4539, // (깜짝) 그렇다면 혹시 지금 우리가 서있는 이 산이 고대 거인의 몸이라는 뜻인가요?
    STR_ID_XENIA6_DRAMA6					=4540, // 하하하. 그건 말도 안되! 그렇게 큰 생명체는 이세상에 존재하지 않다고.
    STR_ID_XENIA6_DRAMA7					=4541, // 어떤 것이 사실인지는 지금 알 수가 없을 것 같습니다. 일단 계속 파괴의 신을 찾아보는 것이 우선일 것 같아요.
    STR_ID_XENIA6_DRAMA8					=4542, // (쿵!)
    STR_ID_XENIA6_DRAMA9					=4543, // 앗! 저 커다란 거인은 뭐지?
    STR_ID_XENIA6_DRAMA10					=4544, // 설마 저것이 바로 그 고대의 거인?
    STR_ID_XENIA6_DRAMA11					=4545, // 고대 거인은 모두 죽었다고 들었는데, 어떻게 이런 일이?
    STR_ID_XENIA6_DRAMA12					=4546, // 와! 신기해요! 빨리 가까이 가봐요!
    STR_ID_XENIA6_DRAMA13					=4547, // 작은 생물들이여 이 곳은 무엇을 하러 왔느냐.
    STR_ID_XENIA6_DRAMA14					=4548, // 당신이 바로 고대의 거인인가요? 그런가요??
    STR_ID_XENIA6_DRAMA15					=4549, // 나의 몸은 고대 거인의 일부이긴 하지만, 파괴의 신 페리어트님에 의해 부활하였을 뿐이다.
    STR_ID_XENIA6_DRAMA16					=4550, // 그럼 고대의 거인들은 모두 당신처럼 그렇게 큰가요??
    STR_ID_XENIA6_DRAMA17					=4551, // 그들에 비하면 나는 아주 작은 거인일 뿐, 실제 그들은 나보다 수십 배는 더 크다.
    STR_ID_XENIA6_DRAMA18					=4552, // 와! 정말 크군요! 그럼 혹시 이 산이 정말 고대 거인의 몸인 건가요??
    STR_ID_XENIA6_DRAMA19					=4553, // 아르메는 거인의 말은 안 듣고 혼자 질문만 계속하고 있잖아.
    STR_ID_XENIA6_DRAMA20					=4554, // 시끄럽구나. 그만 돌아가지 않고 계속 이 곳을 어지럽힌다면 가만 두지 않겠다.
    STR_ID_XENIA6_DRAMA21					=4555, // 저희는 파괴의 신인 페리어트님을 만나러 왔습니다. 그분을 만나게 해주십시오.
    STR_ID_XENIA6_DRAMA22					=4556, // 페리어트님은 지금 아무도 만날 수가 없다. 너희들의 목적이 그렇다면 나도 그냥 보내줄 수가 없구나.
    STR_ID_XENIA6_DRAMA23					=4557, // 결국은 또 한번 힘을 겨뤄야 할 때군.
    STR_ID_XENIA6_DRAMA24					=4558, // 드디어 파괴의 신이 있는 곳까지 왔네요. 하지만 곧 무슨 일이 벌어질지 예상이 되는 건 왜일까요?
    STR_ID_XENIA6_DRAMA25					=4559, // 페리어트님 저희는 그랜드 체이스 일행이라고 합니다.
    STR_ID_XENIA6_DRAMA26					=4560, // 너희가 누구인지 이미 알고 있다. 나를 쓰러뜨릴 자신이 없다면 지금 당장 돌아가거라! 싸움이 시작된다면 너희들의 목숨은 나도 결코 장담할 수가 없다.
    STR_ID_XENIA6_DRAMA27					=4561, // 여기까지 온 이상 이대로 물러설 수는 없지!
    STR_ID_XENIA6_DRAMA28					=4562, // 역시 이 방법 외에는 다른 방법이 없겠군요. 그럼 다시 한번 모두 힘을 합쳐주세요!
    STR_ID_XENIA6_DRAMA29					=4563, // 역시 대단하구나. 나는 너희들의 힘을 믿고 있었다.
    STR_ID_XENIA6_DRAMA30					=4564, // 매번 쉽지는 않았지만, 저희는 그저 최선을 다할 뿐입니다.
    STR_ID_XENIA6_DRAMA31					=4565, // 나의 정수가 필요해서 왔겠지. 자 그대들에게 파괴의 정수를 주겠다.
    STR_ID_XENIA6_DRAMA32					=4566, // 여기의 신들은 모두 우리가 찾아온 이유를 미리 알고 있어서 참 편해.
    STR_ID_XENIA6_DRAMA33					=4567, // 그보다 지금은 소생의 신인 가이아가 많이 걱정되는구나.
    STR_ID_XENIA6_DRAMA34					=4568, // 가이아님도 이미 지배중인 상태겠지요?
    STR_ID_XENIA6_DRAMA35					=4569, // 나와 그녀는 지배의 신의 지배를 피하기 위해 각자 자신의 힘을 봉인하려 했다. 그녀는 좀더 자신을 확실하게 봉인시키기 위해 신목의 영혼과 융화를 시도했지만..
    STR_ID_XENIA6_DRAMA36					=4570, // 느낌이 좋지 않구나! 그랜드 체이스 일행이여 빨리 생명의 숲으로 찾아가 그녀를 도와주거라!
    STR_ID_XENIA6_DRAMA37					=4571, // 알겠습니다. 지금 바로 가이아님에게로 떠나겠습니다.
    STR_ID_LOWER_BLOODY_ORC					=4572, // 하급 피바람 오크
    STR_ID_LOWER_BLOODY_ORC_WARRIOR			=4573, // 하급 피바람 오크 전사
    STR_ID_MAGMA_PARADOM					=4574, // 마그마 파라돔
    STR_ID_TYTAROS							=4575, // 타이타로스
    STR_ID_DRAKE_WORM						=4576, // 드래이크 웜
    STR_ID_GIANT_WORM						=4577, // 자이언트 웜
    STR_ID_STATUE							=4578, // DK_Mark-I
    STR_ID_MYSTONE							=4579, // 미스톤
    STR_ID_GIANT							=4580, // 거상
    STR_ID_PERIOT							=4581, // 페리어트

    STR_ID_PLUS_10P							=4584, // 
    STR_ID_PLUS_15P							=4585, // 
    STR_ID_PLUS_20P							=4586, // 
    STR_ID_ATK_RAISE						=4587, // 
    STR_ID_VIT_RAISE						=4588, // 

    STR_ID_DUGEON_HACKING_CHECK_PARTNER		= 4589, // 해킹방에 있었던 방원들이 받는 메시지
    STR_ID_NPGG_CALLBACK_ERROR07			=4590, // 재부팅 하신 후 게임을 실행해주세요.
    STR_ID_GC_GM_QUEST29					=4592,
    STR_ID_GC_GM_QUEST32					=4593,
    STR_ID_EXCHANGE_GACHA_COIN				= 4600, // 가차코인교환권으로 가차코인교환했을 때의 메시지	

    STR_ID_DESC_ITEM_SKILL21				=4613,	//카운터시 MP 감소량 10% 감소
    STR_ID_DESC_ITEM_SKILL22				=4614,	//카운터시 MP 감소량 15% 감소
    STR_ID_DESC_ITEM_SKILL23				=4615,	//카운터시 MP 감소량 20% 감소
    STR_ID_DESC_ITEM_SKILL24				=4616,	//카운터시 MP 감소량 30% 감소

    STR_ID_GS_AMAZON_FOREST                 =4617, //아마존 숲


    STR_ID_SILVERLAND7_DRAMA1				=4618, // 이런 곳에 저렇게 중무장을 한 병사들이 왜 있는 거지?
    STR_ID_SILVERLAND7_DRAMA2				=4619, // 실버랜드에는 지금까지 실버나이츠를 제외하고 이렇게 정규화된 군대가 있다는 얘기는 들어본 적이 없어.
    STR_ID_SILVERLAND7_DRAMA3				=4620, // 게다가 이들에게선 암흑의 힘까지 느껴지는군. 조사해볼 필요가 있겠어.
    STR_ID_SILVERLAND7_DRAMA4				=4621, // 찬성이야. 여자의 육감으로도 뭔가 수상한 것이 느껴지거든!
    STR_ID_SILVERLAND7_DRAMA5				=4622, // 아니! 빅터!?
    STR_ID_SILVERLAND7_DRAMA6				=4623, // 용케 지금까지 살아있구나 진.
    STR_ID_SILVERLAND7_DRAMA7				=4624, // 설마.. 네가 이 모든 것을 저지른 것은 아니겠지??
    STR_ID_SILVERLAND7_DRAMA8				=4625, // 실버나이츠는 약하다. 나에게는 더 큰 힘이 있고 그리고 나는 실버랜드를 차지하기 위해 그것을 사용할 것이다.
    STR_ID_SILVERLAND7_DRAMA9				=4626, // 같이 실버랜드를 악으로부터 지키겠다던 너의 맹세는 어떻게 된 거냐!? 피와 땀을 함께 흘리고 너에게 충성했던 우리 실버나이츠 동료들은 그럼 무엇이란 말이냐!?
    STR_ID_SILVERLAND7_DRAMA10				=4627, // 약한 소리 말아라 진! 실버랜드 밖에는 네가 상상할 수도 없을 정도의 강한 자들이 존재한다.
    STR_ID_SILVERLAND7_DRAMA11				=4628, // 실버랜드를 진정으로 지켜내려면 더 큰 힘이 필요하고, 그리고 내가 먼저 모든 것을 통일하고 지배해야만 가능하다!
    STR_ID_SILVERLAND7_DRAMA12				=4629, // 웃기지마!! 결국 고작 너의 야심 때문에 동료들을 배신했단 말이냐! 더 큰 힘 따위는 필요 없어! 서로 사랑하고 서로를 아끼는 동료만 있다면 이세상에 무서울 것은 아무것도 없다 말이다!
    STR_ID_SILVERLAND7_DRAMA13				=4630, // 애송이 같은 놈! 그렇다면 진정한 힘의 공포가 무엇인지 내가 뼈저리게 느끼게 해주마.
    STR_ID_SILVERLAND7_DRAMA14				=4631, // 으아아~~!! 용서할 수 없어! 실버나이츠 진! 동료들의 빚을 갚겠다!!
    STR_ID_SILVERLAND7_DRAMA15				=4632, // (헉 헉..) 겨우 쓰러뜨렸나 싶었는데, 빅터가 암흑의 힘을 사용해 체력을 모두 회복하고 말았어요!!

    STR_ID_SILVERLAND7_DRAMA16				=4633, // (헉 헉..) 그깟 암흑의 힘 따위는 두렵지 않아! 진 절대 포기하지 않는다!!
    STR_ID_SILVERLAND7_DRAMA17				=4634, // (헉 헉..) 빅터가 체력을 또 다시 모두 회복하고 말았어요. 그는 정말로 쓰러뜨릴 수 없는 존재일까요?
    STR_ID_SILVERLAND7_DRAMA18				=4635, // (헉 헉..) 윽.. 조금만 더.. 나에게 조금만 더 힘이 있었다면.. 
    STR_ID_SILVERLAND7_DRAMA19				=4636, // (헉 헉..) 진! 나의 기를 받아!
    STR_ID_SILVERLAND7_DRAMA20				=4637, // (헉 헉..) 맞아요 저희의 기를 사용하세요!
    STR_ID_SILVERLAND7_DRAMA21				=4638, // (헉 헉..) 고마워. 너희들이 있는 한 나는 절대로 여기서 쓰리질 수 없어. 그럼 마지막으로 너희들의 기를 모두 모아보겠어!
    STR_ID_SILVERLAND7_DRAMA22				=4639, // (헉 헉..) 드디어 쓰러뜨린 건가..
    STR_ID_SILVERLAND7_DRAMA23				=4640, // 윽.. 이럴 수가.. 암흑의 군주여.. 나에게 조금만 더 힘을..
    STR_ID_SILVERLAND7_DRAMA24				=4641, // 빅터. 네가 할 일은 여기까지다. 그랜드체이스의 시간을 잘 끌어주었구나.
    STR_ID_SILVERLAND7_DRAMA25				=4642, // 으.. 그렇다면 단지 나를 이용한 것인가..
    STR_ID_SILVERLAND7_DRAMA26				=4643, // 지배의 신님은 드디어 우리세계의 다른 신들을 모두 지배하게 되었다. 이제 더 이상 너는 필요 없다. 죽어라!
    STR_ID_SILVERLAND7_DRAMA27				=4644, // 으아~~~~
    STR_ID_SILVERLAND7_DRAMA28				=4645, // 동료를 배신한자의 최후는 이런 거군.
    STR_ID_SILVERLAND7_DRAMA29				=4646, // 고마워. 모두 너희들 덕분이야.
    STR_ID_SILVERLAND7_DRAMA30				=4647, // 그래도 빅터를 직접 쓰러뜨린 건 결국 진님인걸요. 대단해요~
    STR_ID_SILVERLAND7_DRAMA31				=4648, // 하지만 그건 분명 너희들 없이는 불가능 했던 일이야. 이 은혜는 절대 잊지 않을게!
    STR_ID_SILVERLAND7_DRAMA32				=4649, // 당연한걸 가지고~ 우리는 영원한 동료이고, 또 그랜드체인스인걸!!
    STR_ID_XENIA7_DRAMA1					=4650, // 페리어트님의 예상대로 이곳도 이미 몬스터들로 가득하군요.
    STR_ID_XENIA7_DRAMA2					=4651, // 아까부터 왠지 누군가 훔쳐보는듯한 느낌이 들어. 모두 조심하는 게 좋겠어.
    STR_ID_XENIA7_DRAMA3					=4652, // 아무래도 좀 더 서두르는 것이 좋을 것 같습니다.
    STR_ID_XENIA7_DRAMA4					=4653, // 빠른 돌파라.. 마음에 드는걸!
    STR_ID_XENIA7_DRAMA5					=4654, // 멈춰라! 더 이상 앞으로 나아가려 한다면 내가 너희들을 가만두지 않겠다.
    STR_ID_XENIA7_DRAMA6					=4655, // 저희는 가이아님을 도와드리러 온 그랜드체이스라고 합니다. 필요 없는 전투보다는 평화롭게 지나가게 해주시면 좋겠습니다.
    STR_ID_XENIA7_DRAMA7					=4656, // 가소롭다. 가이아님은 너희의 도움 따위는 필요하지 않아. 게다가 너희가 숲에 들어오자마자 호전적으로 마구 전투를 벌이면서 여기까지 오는 것을 모두 지켜보았다. 평화라는 말이 어색하구나.
    STR_ID_XENIA7_DRAMA8					=4657, // 저희는 호전적인 외부인을 환영하지 않습니다. 부디 돌아가 주십시오.
    STR_ID_XENIA7_DRAMA9					=4658, // 드루이드여 우리는 호전적이지 않습니다. 단지 가이아님에게 급한 용무 때문에 마물들에게 시간을 허비할 수가 없었을 뿐입니다.
    STR_ID_XENIA7_DRAMA10					=4659, // 이유가 어떻든 가이아님의 허락 없이는 보내드릴 수가 없으니, 돌아가시거나 저희를 쓰러뜨려야 하는 선택을 하셔야 합니다.
    STR_ID_XENIA7_DRAMA11					=4660, // 선택이라고는 하지만, 저희에겐 선택의 여지가 없어 보이는군요.
    STR_ID_XENIA7_DRAMA12					=4661, // 캬아아~
    STR_ID_XENIA7_DRAMA13					=4662, // 아니! 이것이 신목!? 무시무시하네요.
    STR_ID_XENIA7_DRAMA14					=4663, // 가이아님은 이미 몬스터화 된 신목 속에 갇혀있는 것 같아.
    STR_ID_XENIA7_DRAMA15					=4664, // 생명의 숲이 왜 이렇게 변했는지 알 수 있을 것 같군요.
    STR_ID_XENIA7_DRAMA16					=4665, // 가이아님을 구하려면 먼저 이 끔찍한 가지들을 잘라내야 하겠죠?
    STR_ID_XENIA7_DRAMA17					=4666, // 이런 못생긴 나무를 배어버리는 것은 식은죽 먹기지. 어디 한번 시작해 볼까?
    STR_ID_XENIA7_DRAMA18					=4667, // 아.. 드디어 사악한 마력이 풀리게 되었군요. 고맙습니다. 여러분이 아니었다면 숲은 더 이상 회복할 수 없을 정도로 변해버렸을지도 모릅니다.
    STR_ID_XENIA7_DRAMA19					=4668, // 게다가 사실 조금만 더 늦었더라면 저와 타락한 신목은 영원히 하나가 되어버렸을지도 모르겠습니다.
    STR_ID_XENIA7_DRAMA20					=4669, // 휴.. 저희가 늦지 않아 다행이군요.
    STR_ID_XENIA7_DRAMA21					=4670, // 저 궁금한 게 하나 있는데요. 페리어트님이 가이아님을 상당히 걱정하시던데, 두 분은 어떤 사이인가요?
    STR_ID_XENIA7_DRAMA22					=4671, // 아.. 그.. 그게.. 그러니까.. 지금 이러고 있을 시간이 없습니다. 
    STR_ID_XENIA7_DRAMA23					=4672, // 다음으로 여러분들이 가야 할 곳은 타나토스가 있는 지배의 성입니다. 부디 조심하시고, 저의 정수가 도움이 되었으면 좋겠습니다.

    STR_ID_DIVINE_TREE						=4673, // 신목
    STR_ID_DIVINE_BRANCH					=4674, // 신목의 가지
    STR_ID_FURIOUS_STORM					=4675, // 퓨리어스 스톰
    STR_ID_DARK_STORM						=4676, // 다크 스톰
    STR_ID_WOODEN							=4677, // 우덴
    STR_ID_DK_MARK_II						=4678, // DK Mark-II
    STR_ID_DECOY							=4679, // 디코이
    STR_ID_POISONTHORN						=4680, // 독가시 넝쿨몬
    STR_ID_VICTOR							=4681, // 빅터
    STR_ID_BLACKTEMPEST						=4682, // 블랙 템페스트
    STR_ID_LOWER_DARKPRIEST					=4683, // 하급 암흑사제
    STR_ID_BLACKKNIGHT_DEFENDER				=4684, // 블랙나이츠 방패병
    STR_ID_BLACKKNIGHT_SPEARMAN				=4685, // 블랙나이츠 창병
    STR_ID_VICTOR_PHASE2					=4686, // 빅터 ( PHASE 2 )
    STR_ID_GITFS_MESSAGE					=4687, // Here lies two heroes who believed and fought for the freedom and democracy of every Filipino.\nThank you for the offerings.


    STR_ID_GAIA								=4688, // 가이아
    STR_ID_GITFS_MESSAGE_ALREADY_RECEIVED	=4689, // 1일 1회 보상을 이미 지급 받았음.

    STR_ID_GC_GM_QUEST30					=4690,
    STR_ID_GC_GM_QUEST33					=4691,

    STR_ID_NOT_EXIST_USER_INFO              =4704, //유저 정보가 존재하지 않습니다.
    STR_ID_ALREADY_USED_COUPON              =4709, //해당 쿠폰은 이미 사용되어진 쿠폰입니다 

    STR_ID_XENIA8_DRAMA1					=4755, // 드디어 우리가 여기까지 오게 되었군요.
    STR_ID_XENIA8_DRAMA2					=4756, // 이곳의 몬스터들은 지금까지의 몬스터들과는 차원이 다른 것 같아요.
    STR_ID_XENIA8_DRAMA3					=4757, // 아무래도 그냥 우리들의 힘만으로 상대하기에는 위험할 수도 있을 것 같군요. 그 동안 모은 다섯 가지의 신의 정수들을 잘 활용해야 할 것 같습니다.
    STR_ID_XENIA8_DRAMA4					=4758, // ...
    STR_ID_XENIA8_DRAMA5					=4759, // 음? 이런 곳에 왜 소녀가?
    STR_ID_XENIA8_DRAMA6					=4760, // 하지만 소녀 뒤에 붙어있는 몬스터는 보통 몬스터가 아닌 것 같아요.
    STR_ID_XENIA8_DRAMA7					=4761, // 왠지 예전에 나의 모습을 보는 것 같군. 그녀를 도와줘야겠어.
    STR_ID_XENIA8_DRAMA8					=4762, // 아.. 머리가..
    STR_ID_XENIA8_DRAMA9					=4763, // 정신이 드나요? 무리하진 마세요. 오랫동안 타나토스에게 지배를 당한 것 같아요.
    STR_ID_XENIA8_DRAMA10					=4764, // 타나토스님도 예전에는 참 상냥한 분이셨는데, 결국 저까지 이런 식으로밖에 잡아둘 수 없는 걸까요..
    STR_ID_XENIA8_DRAMA11					=4765, // 타나토스는 신이고 사람이고 가리지 않고 지배를 해대는군. 마음에 들지 않아! 당장 혼내주러 가자!
    STR_ID_XENIA8_DRAMA12					=4766, // 동감이야. 하지만 그보다 우선 이 소녀를 안전한 곳으로 대리고 가야 하지 않을까?
    STR_ID_XENIA8_DRAMA13					=4767, // 괜찮습니다. 저는 원래 이곳의 신녀라 저를 헤치지는 않을 겁니다.
    STR_ID_XENIA8_DRAMA14					=4768, // 그렇다면 안심하고 타나토스를 쓰러뜨리러 갈 수 있겠군.
    STR_ID_XENIA8_DRAMA15					=4769, // 저.. 염치없지만 한가지 부탁이 있습니다. 여러분이 타나토스님을 쓰러뜨리고, 혹시라도 그가 반성하고 예전처럼 돌아오고 싶어한다면, 부디 그를 헤지지 말아주세요..
    STR_ID_XENIA8_DRAMA16					=4770, // 어서 오십시오. 그랜드체이스 여러분. 기다리고 있었습니다.
    STR_ID_XENIA8_DRAMA17					=4771, // 기다리고 있지 말고 마중이라도 나오지 그랬어!
    STR_ID_XENIA8_DRAMA18					=4772, // 생각보다 무섭게 생기지도 않고 점잖은 것 같은데요?
    STR_ID_XENIA8_DRAMA19					=4773, // 모두 방심하면 안됩니다. 겉모습은 저도 조금 의외지만 그에게서 뿜어져 나오는 엄청난 기는 두려워해 마땅할 정도니까요.
    STR_ID_XENIA8_DRAMA20					=4774, // 여러분 하나하나는 보잘것없지만, 이렇게 하나가 되어 힘을 합치니 여기까지 올 수가 있게 되는군요. 정말 놀랍습니다.
    STR_ID_XENIA8_DRAMA21					=4775, // 이제 그만 항복하고 자신의 힘을 마음대로 사용하는 것을 그만두십시오.
    STR_ID_XENIA8_DRAMA22					=4776, // 아니면 착한 척 그만하고 본 모습을 드러내시던가.
    STR_ID_XENIA8_DRAMA23					=4777, // 아무래도 여러분들의 행운은 여기까지인 것 같습니다. 그럼 지금부터 진정한 두려움을 느끼게 해 드리지요.
    STR_ID_XENIA8_DRAMA24					=4778, // 재미있군요. 하지만 지금까지는 그냥 봐드린 것뿐. 이제 슬슬 본격적으로 시작해 볼까요?
    STR_ID_XENIA8_DRAMA25					=4779, // 흐아아!
    STR_ID_XENIA8_DRAMA26					=4780, // 장난은 여기서 끝이다! 나의 진정한 힘을 보여주마! 각오해라!!
    STR_ID_XENIA8_DRAMA27					=4781, // 우아아아!!
    STR_ID_XENIA8_DRAMA28					=4782, // 너희들이 지배의 신을 쓰러뜨리다니, 믿어지지 않는군..
    STR_ID_XENIA8_DRAMA29					=4783, // 아니 너는!? 어디 숨어있다가 이제 나타나는 거냐!
    STR_ID_XENIA8_DRAMA30					=4784, // 지금은 때가 아닌 것 같군, 지배의 정수는 내가 가져가겠다. 다음 번에 만날 때는 너희들의 뜻대로 되지 않을 것이다.
    STR_ID_XENIA8_DRAMA31					=4785, // 앗! 아스타롯이 지배의 정수를 가져가 버렸어요!
    STR_ID_XENIA8_DRAMA32					=4786, // 어떤 것이 진정 가치가 있는 것인지는 지금까지의 여행을 통해 우리는 이미 많이 배웠잖아~ 그깟 정수 따위는 이제 두려워할 필요 없다고.
    STR_ID_XENIA8_DRAMA33					=4787, // 그게 아니라. 나름 예쁜 정수를 수집하고 있었는데, 그것만 모으면 한 세트를 만들 수 있었단 말이에욧! 힝..
    STR_ID_XENIA8_DRAMA34					=4788, // 하하하. 아르메님은 이럴 땐 참 귀여우시단 말씀~
    STR_ID_GARGOYLE							=4789, // 가고일
    STR_ID_SATANIEL							=4790, // 사타니엘 전사
    STR_ID_DARKTEMPLAR						=4791, // 상급 암흑사제
    STR_ID_HERA								=4792, // 헤라
    STR_ID_IBLIS							=4793, // 이블리스
    STR_ID_THANATOS							=4794, // 타나토스
    STR_ID_IBLIS2							=4795, // 이블리스2
    STR_ID_THANATOS2						=4796, // 타나토스2
    STR_ID_THANATOS3						=4797, // 타나토스3
    STR_ID_GC_GM_QUEST34					=4798, // 지배의 성
    STR_ID_DESC_ITEM_SKILL27				=4799,
    STR_ID_DESC_ITEM_SKILL25				=4800,
    STR_ID_DESC_ITEM_SKILL26				=4801,
    STR_ID_XENIA8_DESC						=4802,
    STR_ID_TUNING							=4803,
    STR_ID_TUNING_DESC						=4804,

    STR_ID_DUNGEON_MISSION_LIST_TITLE       =4805,
    STR_ID_DETECT_HACK_FORCE_QUIT			=4812,	// 7번해킹 감지 후 로그아웃 하려고 할 시 메시지
    STD_ID_COUPON_NAME						=4814,
    STD_ID_COUPON_SPEECH1					=4815,
    STD_ID_COUPON_SPEECH2					=4816,
    STD_ID_COUPON_SPEECH3					=4817,

    STR_ID_DELETE_CASH_MISSION					= 4819, //진행 중이던 넷마블 캐쉬 응모권 일일미션이 삭제되었습니다. \n넷마블 캐쉬 응모권 일일미션은 매일 아침 6시에 자동 삭제됩니다.

    STR_ID_COMPLETE_CASH_MISSION				= 4842, //넷마블 캐쉬 응모권을 성공적으로 획득 했습니다.\n획득한 넷마블 캐쉬 응모권은 홈페이지에서 확인 할 수 있습니다.

    STR_ID_SUBSCRIPTION_ERROR_MSG1			=4843, // 1회 응모시 필요한 응모권 개수보다 적음 
    STR_ID_SUBSCRIPTION_ERROR_MSG2			=4844, // 요청한 응모권 수량보다 인벤 수량이 적다. 
    STR_ID_SUBSCRIPTION_ERROR_MSG3			=4845, // 남은 경품수량보다 많이 요청
    STR_ID_SUBSCRIPTION_ERROR_MSG4			=4846, // 이미 처리중인 작업이다.
    STR_ID_SUBSCRIPTION_ERROR_MSG5			=4847, // 해당 이벤트가 없음.
    STR_ID_SUBSCRIPTION_OK_MESSAGE			=4848, // You have successfully exchanged @1 Stubs for @2(s). 
    STR_ID_SUBSCRIPTION_ERROR_MSG6			=4849, // 응모권 개수를 입력하세요.

    STR_ID_JIN4_SP_ADDITION                 =4850,
    STR_ID_JIN4_STANCE                      =4851,
    STR_ID_JIN4_STANCE_UPPER                =4852,
    STR_ID_JIN4_STANCE_FRONT                =4853,
    STR_ID_JIN4_STANCE_KICK                 =4854,
    STR_ID_JIN4_STANCE_BACK                 =4855,
    STR_ID_JIN4_NORMAL_SKILL                =4856,
    STR_ID_JIN4_BURNNING_SKILL              =4857,

   STR_ID_CHATTING_BUFF						=4861,
    STR_ID_GC_GM_QUEST35					=4863, // 
    STR_ID_GC_GM_QUEST36					=4864, // 
    STR_ID_GC_GM_QUEST37					=4865, // 
    STR_ID_GC_GM_QUEST38					=4866, // 

    STR_ID_SUBSCRIPTION_DESC1				= 4867, // @1 @2(으)로 @3 @4(을)를
    STR_ID_SUBSCRIPTION_DESC2				= 4868, // 획득할 수 있습니다.

    STR_ID_GIFT_CASH_ITEM					=4899, //선물하기

    STR_ID_GIFT_RECEIVE						=4915, //@1 님으로부터 선물이 도착하였습니다. 
    STR_ID_GIFT_SEND						=4916, //@1 님에게 선물을 보내드렸습니다.
   
    STR_ID_GP_CAPSULE_DB_WORK_FAIL			=4917,//캡슐획득 DB작업 실패
   STR_ID_DESC_ITEM_SKILL28					=4919, // ITM_EFF_CARNIVAL
   STR_ID_DESC_ITEM_SKILL29					=4920, // ITM_EFF_AP_DP_HP_10_PER 
   STR_ID_DESC_ITEM_SKILL30					=4921, // ITM_EFF_TWENTYCOUNT     
   STR_ID_DESC_ITEM_SKILL31					=4922,

   STR_ID_AMY4_MACHINE_GUN_COMBO			=4923,
   STR_ID_AMY4_MELO_SPECIAL1 				=4924,
   STR_ID_AMY4_MELO_SPECIAL2 				=4925,
   STR_ID_AMY4_MELO_SPECIAL3 				=4926,
   STR_ID_AMY4_SPECIAL1		  				=4927,
   STR_ID_AMY4_SPECIAL2		  				=4928,
   STR_ID_AMY4_SPECIAL3		  				=4929,

   STR_ID_ITEM_TRADE_NORMAL_ITEM			=4930,
   STR_ID_ITEM_TRADE_DURATION_ITEM			=4931, // 수량아이템으로 교환됐을 시의 메시지
    STR_ID_EVENT_COUPOON_INPUT_ERROR		=4932, //잘못된 쿠폰 입력 횟수를 초과하여 더이상 입력을 할 수 없습니다.	
    STR_ID_INCOMPLETE_PATCH_MSG				=4933, //패치가 불완전하게 받혀진것으로 보입니다.\n게임에서 이상현상을 보일 수 있으니 패치를 새로 받고 게임을 시작하기를 권장합니다.

    STR_ID_DETECT_CHANGED_KOM_FILE			=4946, // 클라이언트에 올바르지 않은 파일들이 발견되었습니다.\n게임서버 선택화면으로 이동합니다.
    STR_ID_GPCAPSULE_NO_RECEIVE_MESSAGE		=4968, //GP캡슐 오픈 REQ 패킷보내고 ACK 못받을 시의 에러 메시지
    STR_ID_HALLOWEEN_ERROR					=4969,
    STR_ID_HALLOWEEN_GOAL					=4970,
    STR_ID_HALLOWEEN_ERROR_BUY				=4972,
    STR_ID_KEY_SETTING_CHARGINGSKILL		=5240,

    STR_ID_SIEG3_NAME                       =4975,
    STR_ID_SIEG3_TUTORIAL1                  =4976,
    STR_ID_SIEG3_TUTORIAL2                  =4977,
    STR_ID_SIEG3_TUTORIAL3                  =4978,
    STR_ID_SIEG3_TUTORIAL4                  =4979,
    STR_ID_SIEG3_TUTORIAL5                  =4980,
    STR_ID_SIEG3_TUTORIAL6                  =4981,
    STR_ID_DICE_DOUBLE                      =4987,
    STR_ID_DICE_EXCHANGED_COUNT_MSG			=4988,	

    STR_ID_INCOMPLETE_PATCH_MSG2			=4991, //패치가 불완전하게 받혀진것으로 보입니다.\n게임에서 이상현상을 보일 수 있으니 패치를 새로 받고 게임을 시작하기를 권장합니다.
    STR_ID_JIN4_SPECIAL1					=5004,// 필살기1
    STR_ID_JIN4_SPECIAL2					=5005,// 필살기2
    STR_ID_JIN4_SPECIAL3					=5006,// 필살기3
    STR_ID_JIN4_BURNNING_SPECIAL1       	=5007,
    STR_ID_JIN4_BURNNING_SPECIAL1_2     	=5008,
    STR_ID_JIN4_BURNNING_SPECIAL2       	=5009,
    STR_ID_JIN4_BURNNING_SPECIAL2_2     	=5010,
    STR_ID_JIN4_BURNNING_SPECIAL3       	=5011,
    STR_ID_JIN4_BURNNING_SPECIAL3_2     	=5012,

    STR_ID_GUILD_MESSENGER_ONLINE_MSG		=5036,//온라인    @1명 (@2/@3)
    STR_ID_GUILD_MESSENGER_OFFLINE_SIGN		=5037,//오프라인 표시
    STR_ID_GUILD_MESSENGER_BLOCK_SIGN		=5038,//차단인원 표시   
    STR_ID_ANTIALISING						=5039,  //화면 보정 Antialising
    STR_ID_BINGO_STRING_DESC          = 5263,

    STR_ID_TOURNAMENT_VOTE_C2				=5268,
    STR_ID_TOURNAMENT_VOTE_C3				=5269,
    STR_ID_TOURNAMENT_VOTE_C4				=5270,
    STR_ID_TOURNAMENT_VOTE_C5				=5271,
    STR_ID_TOURNAMENT_VOTE_C6				=5272,
    STR_ID_TOURNAMENT_VOTE_C7				=5273,
    STR_ID_TOURNAMENT_VOTE_C8				=5274,
    STR_ID_TOURNAMENT_VOTE_C9				=5275,
    STR_ID_TOURNAMENT_VOTE_C10				=5276,
    STR_ID_TOURNAMENT_VOTE_C11				=5277,
    STR_ID_TOURNAMENT_VOTE_C12				=5278,
    STR_ID_TOURNAMENT_VOTE_C13				=5279,
    STR_ID_TOURNAMENT_VOTE_C14				=5280,
    STR_ID_TOURNAMENT_VOTE_C15				=5281,
    STR_ID_TOURNAMENT_VOTE_C16				=5282,
    STR_ID_VOTE_SUCCESS						=5283,
    STR_ID_VOTE_FAILURE						=5284,
    STR_ID_LIMITED_BUY						=5285, //구매한도를 초과하였습니다.
    STR_ID_VOTE_NICKNAME					=5286, 
    STR_ID_VOTE_NOTICKET					=5287,
    STR_ID_NEW_GUILD_GRADE					=5288,
    STR_ID_NEW_GUILD_MASTER_NAME			=5289,
    STR_ID_NEW_GUILD_BIRTHDAY				=5290,
    STR_ID_NEW_GUILD_POINT					=5291,
    STR_ID_NEW_GUILD_MEMBER_NUM				=5292,
    STR_ID_NEW_GUILD_TOTAL_RANKING			=5293,
    STR_ID_NEW_GUILD_MONTH_RANKING			=5294,
    STR_ID_NEW_GUILD_WEEK_RANKING			=5295,
    STR_ID_NEW_GUILD_GRADE_REGULAR			=5296,
    STR_ID_NEW_GUILD_GRADE_NEW				=5297,
    STR_ID_NEW_GUILD_MASTER_COMMITION		=5311,
    STR_ID_NEW_GUILD_GENERAL_COMMITION		=5312,
    STR_ID_NEW_GUILD_NORMAL_COMMITION		=5313,
    STR_ID_NEW_GUILD_DRUM_OUT				=5314,

    STR_ID_NEW_GUILD_SQUARE_GUILD_MENU		=5328,
    STR_ID_NEW_GUILD_JOIN_CANCEL_REQ		=5330,
    STR_ID_NEW_GUILD_JOIN_CANCEL_MSG		=5331,
    STR_ID_NEW_GUILD_JOIN_CANCEL_SUCCESS	=5332,
    STR_ID_NEW_GUILD_JOINER_CAREER			=5333,
    STR_ID_NEW_GUILD_JOINER_ALLOW_TITLE		=5334,
    STR_ID_NEW_GUILD_JOINER_ALLOW_RESULT	=5335, //길드가입신청자가 받는 승인 메시지
    STR_ID_NEW_GUILD_JOINER_CANCEL_TITLE	=5336,
    STR_ID_NEW_GUILD_JOINER_CANCEL_RESULT	=5337, //길드가입신청자가 받는 거절 메시지
    STR_ID_NEW_GUILD_WAITING_JOINER_NUM		=5338,
    STR_ID_NEW_GUILD_JOINER_ALLOW_MSG		=5339,
    STR_ID_NEW_GUILD_JOINER_CANCEL_MSG		=5340,	
    STR_ID_NEW_GUILD_CREATE_CONDITION_1		=5341,//길드 생성 조건
    STR_ID_NEW_GUILD_CREATE_CONDITION_2		=5342,//계급 왕립 이상
    STR_ID_NEW_GUILD_CREATE_CONDITION_3		=5343,//30Lv 이상 캐릭터 보유
    STR_ID_NEW_GUILD_CREATE_CONDITION_4		=5344,//40,000 GP가 소모 됩니다.
    STR_ID_NEW_GUILD_CREATE_NAME_INFO_1		=5345,//① 길드 이름을 입력해 주세요.
    STR_ID_NEW_GUILD_CREATE_NAME_INFO_2		=5346,//길드 이름은 최소 1자에서 6자 내로 입력 가능합니다.\n특수문자나 띄어쓰기를 사용 할 수 없습니다.
    STR_ID_NEW_GUILD_CREATE_URL_INFO_1		=5347,//② 길드 까페 웹페이지 주소(URL)를 입력해 주세요.
    STR_ID_NEW_GUILD_CREATE_MORE_INFO_1		=5348,//⊙ 길드 생성 완료 시, 신생 길드로 임명되며 이 동안 길드 마크를 등록 할 수 없습니다.
    STR_ID_NEW_GUILD_CREATE_MORE_INFO_2		=5349,//⊙ 길드 생성 30일 내, 길드원 5명 이상 달성 시 정규 길드로 승급 됩니다.길드 마크 변경권이 상품으로 지급 되며, 길드 마크를 등록 할 수 있습니다.
    STR_ID_NEW_GUILD_CREATE_MORE_INFO_3		=5350,//⊙ 길드 생성 30일 동안 길드마스터 포함, 총 인원 1명에서 4명인 길드는 자동 삭제 되므로 주의하시기 바랍니다.

    STR_ID_NEW_GUILD_INTRODUCE_TITLE		=5351,
    STR_ID_NEW_GUILD_NOTICE1_TITLE			=5352,
    STR_ID_NEW_GUILD_NOTICE2_TITLE			=5353,
    STR_ID_MAXIMUM_LENGTH_REACHED			=5354,

    STR_ID_INVALID_TEXT_INPUT               =5358,//허용되지 않는 특수문자 또는 금지어가 포함되어 있습니다. 다시 확인하고 작성해 주세요.

    STR_ID_NEW_GUILD_MASTER_COMMITION_MSG2	=5359,//"위임하시겠습니까?" 문장이 있는 재확인 문장
    STR_ID_NEW_GUILD_NAME_CHANGE_DESC		=5360,
    STR_ID_NEW_GUILD_MARK_CHANGE_DESC		=5361,
    STR_ID_NEW_GUILD_JOIN_DIRECTLY			=5362,
    STR_ID_NEW_GUILD_JOIN_AFTER				=5363,
    STR_ID_NEW_GUILD_JOIN_STOP				=5364,
    STR_ID_NEW_GUILD_BREAK_UP_DESC			=5365,
    STR_ID_NEW_GUILD_NAME_CHANGE_SUCCESS	=5366,
    STR_ID_NEW_GUILD_NAME_CHANGE_ALERT_MSG	=5367,
    STR_ID_NEW_GUILD_MARK_CHAGE_COMPLETE	=5368,
    STR_ID_NEW_GUILD_DESTROY_ALERT_TITLE	=5371,
    STR_ID_NEW_GUILD_DESTROY_ALERT_MSG		=5372,
    STR_ID_NEW_GUILD_DESTROY_RESULT_MSG		=5373,
    STR_ID_NEW_GUILD_BREAK_UP_REQ			=5374,
    STR_ID_NEW_GUILD_DRUM_OUT_COMPLETE		=5375,
    STR_ID_NEW_GUILD_DRUM_OUT_COMPLETE_MSG	=5376,

    STR_ID_NEW_GUILD_REWARD_INFO			=5377,//주간, 월간, 전체 랭커 길드들에게 지급하는 보상 아이템 입니다.\n대상 길드 : 주간, 월간, 전체 랭킹 1~3위 랭커 길드(오픈 테스트 제외)\n지급일 : 주간, 월간, 전체 랭킹 갱신 시간과 동시에 지급
    STR_ID_NEW_GUILD_DAILY_RANK_INFO		=5378,//일일랭킹은 매일 새벽 5:30에 업데이트 되며 하루 동안의 G-Point 집계한 순위입니다.
    STR_ID_NEW_GUILD_WEEKLY_RANK_INFO		=5379,//주간랭킹은 매주 월요일 새벽 5:30에 업데이트 되며 한 주 동안의 G-Point를 집계한 순위입니다.
    STR_ID_NEW_GUILD_MONTHLY_RANK_INFO		=5380,//월간랭킹은 매달 1일 새벽 5:30에 업데이트 되며 월초부터 월말까지의 G-Point를 집계한 순위입니다.
    


    STR_ID_NEW_GUILD_NO_INFO				=5381,//현재 길드는 1번 이상의 길드전 경험이 없어 랭킹정보가 없습니다.
    STR_ID_BINGO_ERROR1                 	=5389,
    STR_ID_BINGO_ERROR2                 	=5390,
    STR_ID_BINGO_ERROR3                 	=5391,
    STR_ID_BINGO_ERROR4                 	=5392,
    STR_ID_BINGO_ERROR5                 	=5393,
    STR_ID_BINGO_ERROR6                 	=5394,
    STR_ID_BINGO_ERROR7                 	=5395,
    STR_ID_BINGO_ERROR8                 	=5396,
    STR_ID_BINGO_ERROR9                 	=5397,
    STR_ID_BINGO_ERROR10                 	=5398,
    STR_ID_BINGO_ERROR11                 	=5399,
    STR_ID_BINGO_ERROR12                 	=5400,

    STR_ID_NOT_ACQUIRE_COIN              	=5401,
    STR_ID_TRADE_COIN                    	=5402,
    STR_ID_GET_COIN							=5403,
    STR_ID_NEW_GUILD_INFO_TITLE				=5419,
    STR_ID_NEW_GUILD_FOUNDING_DAY			=5420,
    STR_ID_NEW_GUILD_MEMBER_COMMITION		=5421,

    STR_ID_NEW_GUILD_SQAURE_CHNNEL			=5424,
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR01 =5425, //길드원이 아닙니다. 
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR02 =5426, //길드마스터 임명은 한번에 한명씩만 가능합니다.
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR03 =5427, //길드정보가 없습니다.
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR04 =5428, //길드마스터가 아닙니다.
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR05 =5429, //변경대상이 일반길드원이나 제너럴이 아닙니다.
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR06 =5430, //입력인자의 등급이 잘못되었습니다.
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR07 =5431, //길드마스터를 탈퇴시킬 수 없습니다.
    STR_ID_NEW_GUILD_MEMBER_CHAGNGE_ERROR08 =5432, //정보가 비어 있습니다.
    STR_ID_ROOM_FILTER_GUILD                =5442, // 길드원의 방 

    STR_ID_NEW_GUILD_CREATE_SUCCESS_TITLE	=5433, //길성생성완료
    STR_ID_NEW_GUILD_CREATE_SUCCESS_DESC	=5434, //축하합니다! 길드가 성공적으로 생성되었습니다.	
    STR_ID_NEW_GUILD_CREATE_ERROR1			=5435, //길드명 길이가 적당하지 않습니다.
    STR_ID_NEW_GUILD_CREATE_ERROR2			=5436, //길드생성 조건레벨에 해당하는 캐릭터가 없어서 길드를 생성할 수 없습니다.
    STR_ID_NEW_GUILD_CREATE_ERROR3			=5437, //길드생성을 위한 GP가 부족합니다.
    STR_ID_NEW_GUILD_CREATE_ERROR4			=5438, //길드이름 중에 특수문자가 있습니다. 길드명을 다시 입력하세요.
    STR_ID_NEW_GUILD_CREATE_ERROR5			=5439, //이미 존재하는 길드이름입니다. 길드명을 다시 입력하세요.
    STR_ID_NEW_GUILD_CREATE_ERROR6			=5440, //이미 존재하는 URL 입니다. URL을 다시 입력하세요.
    STR_ID_NEW_GUILD_CREATE_ERROR7			=5441, //길드에 가입된 유저는 길드를 생성할 수 없습니다.

    STR_ID_MESSENGER_FRIEND_ADD_SUCCESS		=5443,
    STR_ID_NEW_GUILD_MY_COMMENT				=5444,
    STR_ID_FATAL_MATCH						=5445,
    STR_ID_NEW_GUILD_MASTER_COMMITION_MSG1	=5446, //"위임하시겠습니까?" 문장이 없는거
    STR_ID_NEW_GUILD_CHANGE_MY_COMMENT_MSG	=5458,
    STR_ID_NEW_GUILD_CHANGE_COMPLETE		=5459,

    STR_ID_NEW_GUILD_DESTROY_NOT_MASTER		=5460,

    STR_ID_NEW_GUILD_MEMBER_GRADE_CHANGE	=5461,
    STR_ID_NEW_GUILD_NO_MASTER_OR_GENERAL	=5462, //길마나 제너럴이 아님
    STR_ID_NEW_GUILD_NO_WAITING_JOINER		=5463, //가입대기자가 아님
    STR_ID_NEW_GUILD_NO_VERIFY_INFO			=5464, //가입대기자 또는 일반길드원이나 제너럴이 아님
    STR_ID_NEW_GUILD_INCLUDE_SLANG			=5465,
    STR_ID_NEW_GUILD_DB_NO_USE_CHARACTER	=5466,
    STR_ID_NEW_GUILD_NO_NAME_CHANGE_ITEM	=5467,
    STR_ID_NEW_GUILD_NO_EXIST_GUILD			=5468,
    STR_ID_NEW_GUILD_NO_REGIST_GRADE		=5469,
    STR_ID_NEW_GUILD_JOIN_STOP_STATE		=5470,
    STR_ID_NEW_GUILD_LIST_UPDATE_ERROR		=5471,
    STR_ID_NEW_GUILD_NO_DEFINED_SORT		=5472,
    STR_ID_NEW_GUILD_SEARCH_ERROR			=5473,
    STR_ID_NEW_GUILD_ALREADY_JOIN_STATE		=5474,
    STR_ID_NEW_GUILD_URL_LENGTH_ERROR		=5475,
    STR_ID_NEW_GUILD_JOIN_ALLOW_REQ			=5476,
    STR_ID_NEW_GUILD_JOIN_REQ_TITLE			=5477,
    STR_ID_NEW_GUILD_SQUARE_MENU_LIMIT_MSG	=5478,
    STR_ID_NEW_GUILD_JOIN_WAITING_STATE		=5479,
    STR_ID_NEW_GUILD_CHANGE_JOIN_USAGE		=5480,

    STR_ID_FILEDIALOG_DESKTOP				= 5481,
    STR_ID_FILEDIALOG_MYDOCUMENT			= 5482,
    STR_ID_FILEDIALOG_DEVICELIST			= 5483,

    STR_ID_FILEDIALOG_STATIC_FILENAME		= 5484,
    STR_ID_FILEDIALOG_STATIC_FILETYPE		= 5485,
    STR_ID_FILEDIALOG_STATIC_FILTER			= 5486,
    STR_ID_NEW_GUILD_INPUT_SEARCH_KEYWORD	= 5491,
    STR_ID_DONOT_RECOMUSER						= 5492,
    STR_ID_NEW_GUILD_BANISHMENT_MESSAGE		= 5493,

    STR_ID_NEW_GUILD_CHANGE_TITLE			= 5495,
    STR_ID_NEW_GUILD_CHANGE_ALERT_MSG		= 5496,
    STR_ID_NEW_GUILD_TOTAL_RANK_INFO		= 5497,

    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL1		= 5498,
    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL2		= 5499,
    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL3		= 5500,
    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL4		= 5501,
    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL5		= 5502,
    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL6		= 5503,
    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL7		= 5504,
    
    STR_ID_GWC_DONATION_DESC				=5505,
    STR_ID_GWC_STATE_DESC					=5506, // 던전에서 몬스터가 드랍하는 GW 2009 우승 기원 뱃지를 수집해서 우리나라가 우승 할 수 있도록 도와주세요~!
    STR_ID_GWC_DONATION_COMPLETE			=5507, // @1개의 아이템을 기부하셨습니다.
    
    STR_ID_NEW_GUILD_CREATE_EXIST_BLANK		= 5508,
    STR_ID_NEW_GUILD_MEMBER_EXPULSION		= 5509,
    STR_ID_NEW_GUILD_GRADE_UP_MESSAGE		= 5510,
    STR_ID_NEW_GUILD_MARK_CHANGE_STATE_MSG	= 5511,
    STR_ID_NEW_GUILD_OUT_MESSAGE_IN_GAME	= 5512,
    STR_ID_NEW_GUILD_GRADE_DOWN_MESSAGE		= 5513,

    STR_ID_NEW_GUILD_MARK_CHANGE_FAIL_MSG	= 5518, //길드마크 변경 신청의 승인이 실패하였습니다.\n다른 길드마크를 등록해주세요.
    STR_ID_NEW_GUILD_MARK_CHANGE_REQ_MSG	= 5519, //길드마크 변경신청이 완료되었습니다.\n길드마크 변경승인을 기다려 주세요

    STR_ID_NEW_GUILD_URL_ERROR					 = 5559, //등록된 URL입니다. 다른 URL로 변경하여 주십시오.
    STR_ID_NEW_GUILD_SLANG_FILTER_MSG		= 5560, //금칙어가 포함된 길드명입니다. 다른 길드명을 입력해주세요.

    STR_ID_NEW_GUILD_BATTLE_COMMING_SOON	= 5561,
    STR_ID_MARI1_TUTORIAL_1					= 5563,
    STR_ID_MARI1_TUTORIAL_2					= 5564,
    STR_ID_MARI1_TUTORIAL_3					= 5565,
    STR_ID_MARI1_TUTORIAL_4					= 5566,
    STR_ID_MARI1_TUTORIAL_5					= 5567,
    STR_ID_MARI1_TUTORIAL_6					= 5568,
    STR_ID_MARI1_TUTORIAL_7					= 5569,
    STR_ID_MARI1_TUTORIAL_8					= 5570,
    STR_ID_MARI1_SP_TUTORIAL_1				= 5571,
    STR_ID_MARI1_SP_TUTORIAL_2				= 5572,
    STR_ID_MARI1_SP_TUTORIAL_3				= 5573,
    STR_ID_MARI1_SP_TUTORIAL_4				= 5574,
    STR_ID_MARI1_SP_TUTORIAL_5				= 5575,
    STR_ID_MARI1_SP_TUTORIAL_6				= 5576,
    STR_ID_MARI1_SP_TUTORIAL_7				= 5577,
    STR_ID_MARI1_SP_TUTORIAL_8				= 5578,

    STR_ID_MARI_NAME						= 5579,
    STR_ID_MARI1_JOB_NAME   				= 5580,
    STR_ID_MARI_SPECIAL_SKILL				= 5581,
    STR_ID_MARI_NORMAL_SKILL   				= 5582,

    STR_ID_UNKNOWN_MARI_NAME                = 5690,
    STR_ID_NEW_GUILD_OVERLAP_GUILD_NAME     = 5694,
    STR_ID_ITEM_JOB_LEVEL_STRING            = 5696,
    STR_ID_ITEM_KIND_RECIPE                 = 5697,
    STR_ID_ITEM_KIND_MONSTERCARD            = 5698,
    STR_ID_ITEM_KIND_MISSION_SCROLL         = 5699,
    STR_ID_ITEM_KIND_EXTRA_ITEM             = 5700,
    STR_ID_ITEM_INFORMATION_BOX_EQUIP_STATE1 = 5701,
    STR_ID_ITEM_INFORMATION_BOX_EQUIP_STATE2 = 5702,
    STR_ID_ITEM_INFORMATION_BOX_EQUIP_STATE3 = 5703,
    STR_ID_ITEM_UNKNOWN_STRING              = 5704,
    STR_ID_ITEM_GRADE_STRING1               = 5705,
    STR_ID_ITEM_GRADE_STRING2               = 5706,
    STR_ID_ITEM_GRADE_STRING3               = 5707,
    STR_ID_ITEM_GRADE_STRING4               = 5708,
    STR_ID_ITEM_GRADE_STRING5               = 5709,
    STR_ID_ITEM_SOCKET_STATE_CLOSED         = 5710,
    STR_ID_ITEM_SOCKET_STATE_OPENED         = 5711,
    STR_ID_ITEM_SOCKET_STATE_UNUSABLE       = 5712,

    STR_ID_ITEM_ATTRIBUTE_STRING0           = 5713,
    STR_ID_ITEM_ATTRIBUTE_STRING1           = 5714,
    STR_ID_ITEM_ATTRIBUTE_STRING2           = 5715,
    STR_ID_ITEM_ATTRIBUTE_STRING3           = 5716,
    STR_ID_ITEM_ATTRIBUTE_STRING4           = 5717,
    STR_ID_ITEM_ATTRIBUTE_STRING5           = 5718,
    STR_ID_ITEM_ATTRIBUTE_STRING6           = 5719,
    STR_ID_ITEM_ATTRIBUTE_STRING7           = 5720,
    STR_ID_ITEM_ATTRIBUTE_STRING8           = 5721,
    STR_ID_ITEM_ATTRIBUTE_STRING9           = 5722,
    STR_ID_ITEM_ATTRIBUTE_STRING10          = 5723,
    STR_ID_ITEM_ATTRIBUTE_STRING11          = 5724,
    STR_ID_ITEM_ATTRIBUTE_STRING12          = 5725,
    STR_ID_ITEM_ATTRIBUTE_STRING13          = 5726,
    STR_ID_ITEM_ATTRIBUTE_STRING14          = 5727,
    STR_ID_ITEM_ATTRIBUTE_STRING15          = 5728,
    STR_ID_ITEM_ATTRIBUTE_STRING16          = 5729,
    STR_ID_ITEM_ATTRIBUTE_STRING17          = 5730,
    STR_ID_ITEM_ATTRIBUTE_STRING18          = 5731,
    STR_ID_ITEM_ATTRIBUTE_STRING19          = 5732,
    STR_ID_ITEM_ATTRIBUTE_STRING20          = 5733,
    STR_ID_ITEM_ATTRIBUTE_STRING21          = 5734,
    STR_ID_ITEM_ATTRIBUTE_STRING22          = 5735,
    STR_ID_ITEM_ATTRIBUTE_STRING23          = 5736,
    STR_ID_ITEM_ATTRIBUTE_STRING24          = 5737,
    STR_ID_ITEM_ATTRIBUTE_STRING25          = 5738,
    STR_ID_MY_MONSTER_CARD_COUNT            = 5744,
    STR_ID_MONSTER_CARD_BOOK_CURRENT_PAGE   = 5745,
    STR_ID_MONSTER_CARD_CONSUMPTION_GP      = 5746,

    STR_ID_SET_ITEM_ATTRIBUTE_EQUIP_COUNT   = 5754,
    STR_ID_SET_ITEM_TITLE_STRING            = 5755,
    STR_ID_ATTRIBUTE__PRE_STRING            = 5756,

    STR_ID_DICE_STRING0                     = 5760, //[주사위] 
    STR_ID_DICE_STRING1                     = 5761, //@1(@2) 
    STR_ID_BONUS_GP_STRING                  = 5762, //@1(@2) 

    STR_ID_STONECLAN_GOBLIN	                = 5520, //
    STR_ID_BLUTUS				            = 5523, //
    STR_ID_SETESH				            = 5525, //
    STR_ID_BARAKUP_OLD1		                = 5544, //
    STR_ID_BARAKUP_OLD2		                = 5763, //
    STR_ID_HATSHEPSUT			            = 5531, //
    STR_ID_KUNGJI				            = 5532, //
    STR_ID_KUNGJI_GAURDIAN		            = 5535, //
    STR_ID_KUNGJI_ELDER		                = 5536, //
    STR_ID_FIREHAMMER_TORCH	                = 5540, //
    STR_ID_TALIN_POOLGUARD	                = 5543, //

    STR_ID_GC_GM_QUEST39                    = 5766, //
    STR_ID_GC_GM_QUEST40                    = 5767, //
    STR_ID_GC_GM_QUEST41                    = 5768, //
    STR_ID_GC_GM_QUEST42                    = 5769, //
   
    STR_ID_HP_RECOVERY_RATE                 = 5778,
    STR_ID_MP_RECOVERY_RATE                 = 5779,

    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC1 = 5797, //
    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC2 = 5798, //
    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC3 = 5799, //
    
    STR_ID_SELECT_ATTRIBUTE_MESSAGE         = 5800,
    STR_ID_SELECT_ATTRIBUTE_SUCCESS_MSG     = 5801,
    STR_ID_SELECT_ATTRIBUTE_ERROR1          = 5802,
    STR_ID_SELECT_ATTRIBUTE_ERROR2          = 5803,
    STR_ID_SELECT_ATTRIBUTE_ERROR3          = 5804,
    STR_ID_SELECT_ATTRIBUTE_OVERLAP_SELECT  = 5805,

    STR_ID_ITEM_SOCKET_OPEN_SUCCESS         = 5806, //
    STR_ID_ITEM_SOCKET_OPEN_ERROR0          = 5807, //
    STR_ID_ITEM_SOCKET_OPEN_ERROR1          = 5808, //
    STR_ID_ITEM_SOCKET_OPEN_ERROR2          = 5809, //
    STR_ID_ITEM_SOCKET_OPEN_ERROR3          = 5810, //
    STR_ID_ITEM_SOCKET_CARD_EQUIP_SUCCESS   = 5811, //
    STR_ID_ITEM_SOCKET_CARD_EQUIP_ERROR0    = 5812, //
    STR_ID_ITEM_SOCKET_CARD_EQUIP_ERROR1    = 5813, //
    STR_ID_ITEM_SOCKET_CARD_EQUIP_ERROR2    = 5814, //
    STR_ID_ITEM_SOCKET_CARD_REMOVE_SUCCESS  = 5815, //
    STR_ID_ITEM_SOCKET_CARD_REMOVE_ERROR0   = 5816, //
    STR_ID_ITEM_SOCKET_CARD_REMOVE_ERROR1   = 5817, //
    STR_ID_ITEM_SOCKET_CARD_REMOVE_ERROR2   = 5818, //
    STR_ID_ITEM_SOCKET_CARD_REMOVE_ERROR3   = 5819, //
    STR_ID_ITEM_SOCKET_OPEN_ERROR4          = 5820, //
    STR_ID_ITEM_SOCKET_CARD_EQUIP_ERROR3    = 5821, //
    STR_ID_ITEM_CANT_EQUIP_MONSTER_CARD     = 5822, //
    STR_ID_TITLE_DONT_HAVE_PREV_TITLE		    = 5824, //
    STR_ID_SKILLTREE_RONAN_SWAPBTN1			= 5825, //마검술으로 전환합니다. (단축키 : X )

    STR_ID_ITEM_SOCKET_CONSUMPTION_CASH_ITEM = 5826, //
    STR_ID_ITEM_BEFORE_SELECT_CLOSED_SOCKET = 5827, //
    STR_ID_ITEM_AFTER_SELECT_CLOSED_SOCKET  = 5828, //
    STR_ID_ITEM_BEFORE_SELECT_OPENED_SOCKET = 5829, //
    STR_ID_ITEM_AFTER_SELECT_OPENED_SOCKET  = 5830, //
    STR_ID_ITEM_BEFORE_SELECT_USING_SOCKET  = 5831, //
    STR_ID_ITEM_AFTER_SELECT_USING_SOCKET   = 5832, //
    STR_ID_ITEM_BEFORE_SELECT_BREAK_SOCKET  = 5833, //
    STR_ID_ITEM_AFTER_SELECT_BREAK_SOCKET   = 5834, //

    STR_ID_MONEYTYPE_VIRTUAL_CASH           = 5835,
    STR_ID_MONEYTYPE_GCPOINT                = 5836,
    STR_ID_DONT_HAVE_MONSTER_CARD           = 5837,
    STR_ID_ITEM_CHAR_TYPE_COMMON            = 5840,

    STR_ID_SLOT_POSITION_TITLE              = 5843,
    STR_ID_MISSION_PENALTY                  = 5845, // 낮은 레벨의 미션이라서 보상을 일부만 받습니다. 

    STR_ID_MARI2_JOB_NAME                   = 5847,
    STR_ID_MARI2_COMBO                      = 5848,
    STR_ID_MARI2_CRITICAL_ATK               = 5849,
    STR_ID_MARI2_DASH_ATK                   = 5850,
    STR_ID_MARI2_TURNING_SHOT               = 5851,
    STR_ID_MARI2_JUMP_ATK                   = 5852,
    STR_ID_MARI2_DASH_JUMP                  = 5853,
    STR_ID_MARI2_MAGIC_FIRE                 = 5854,
    STR_ID_MARI2_PUSH_BOARD                 = 5855,
    STR_ID_MARI2_TRIPLE_MISSILE             = 5856,
    STR_ID_MARI2_EQUIP_MAGIC_BULLET         = 5857,
    STR_ID_MARI2_SUMMON_WDW                 = 5858,

    STR_ID_PERIOD_DAY                       = 5859, // @1 일
    STR_ID_ATTRIBUTE_SELECT                 = 5864, // 남은 속성 갯수 : @1

    STR_ID_ITEM_BREAKUP_ERROR0              = 5876,
    STR_ID_ITEM_BREAKUP_ERROR1              = 5877,
    STR_ID_ITEM_BREAKUP_ERROR2              = 5878,
    STR_ID_MARI2_HITTING_GAUGE_DESC1        = 5889,
    STR_ID_MARI2_HITTING_GAUGE_DESC2        = 5890,
    STR_ID_ITEM_BREAKUP_ERROR3              = 5897,
    STR_ID_ITEM_BREAKUP_ERROR4              = 5898,

    STR_ID_MON_SAND_RULER			        = 5944,
    STR_ID_MON_SENTRY_KEEPER_NO1	        = 5920,
    STR_ID_MON_SENTRY_KEEPER_NO2	        = 5921,
    STR_ID_MON_DWARF_GAURD      	        = 5950,

    STR_ID_ITEM_CREATION_ERROR0              = 5951,
    STR_ID_ITEM_CREATION_ERROR1              = 5952,
    STR_ID_ITEM_CREATION_ERROR2              = 5953,
    STR_ID_ITEM_CREATION_ERROR3              = 5954,
    STR_ID_ITEM_CREATION_ERROR4              = 5955,
    STR_ID_ITEM_CREATION_ERROR5              = 5956,

    STR_ID_VIP_GACHA_ERROR1                 =5958,  // 왕실 뱃지 부족합니다.
    STR_ID_VIP_GACHA_ERROR2                 =5959,  // vip 왕실뱃지는 게임 룸 안에서 사용이 불가합니다.
    STR_ID_VIP_GACHA_BUY_ITEM               =5960,  // 이 아이템을 구매 하시겠습니까?
    STR_ID_VIP_GACHA_DESC1                  =5961,  // 왕실뱃지 획득 방법!!
    STR_ID_VIP_GACHA_DESC2                  =5962,  // 이렇쿵 저러쿵 요렇게 저렇게~!

    STR_ID_MON_TALIN_HURT1      	         = 5963,
    STR_ID_ITEM_CREATION0                    = 5967, // 비용
    STR_ID_DESC_ITEM_SKILL32                 = 5987,
    
    STR_ID_SIEG4_TUTORIAL_COMBO             = 6035,
    STR_ID_SIEG4_TUTORIAL_CRITICAL          = 6036,
    STR_ID_SIEG4_TUTORIAL_DASHATK           = 6037,
    STR_ID_SIEG4_TUTORIAL_UNIQUEATK         = 6038,
    STR_ID_SIEG4_TUTORIAL_SLIDING           = 6039,
    STR_ID_SIEG4_TUTORIAL_JUMP_UNIQUEATK    = 6040,
    STR_ID_SIEG4_TUTORIAL_FURY_GAUGEATK     = 6041,
    STR_ID_SIEG4_TUTORIAL_NAME              = 6042,
    STR_ID_SIEG4_TUTORIAL_JF_SEE_DESCRIPTION= 6045,
    STR_ID_SIEG4_TUTORIAL_SEE_SKILL         = 6046,
    STR_ID_SIEG4_TUTORIAL_JF_DESCRIPTION1   = 6047,
    STR_ID_SIEG4_TUTORIAL_JF_DESCRIPTION2   = 6048,
    STR_ID_SIEG4_TUTORIAL_JF_DESCRIPTION3   = 6049,
    STR_ID_SIEG4_TUTORIAL_JF_DESCRIPTION4   = 6050,
    STR_ID_SIEG4_TUTORIAL_JF_DESCRIPTION5   = 6051,
    STR_ID_SIEG4_TUTORIAL_JF_DESCRIPTION6   = 6052,
    STR_ID_SIEG4_TUTORIAL_JF_DESCRIPTION7   = 6053,
    STR_ID_SIEG4_TUTORIAL_WHAT_JF           = 6054,

    STR_ID_GC_GM_QUEST43                    = 5977, //
    STR_ID_GC_GM_QUEST44                    = 5978, //

    STR_ID_INVENTORY_FULL_ERROR0             = 5999, // 인벤토리가 가득 차서 더 이상 아이템 획득이 불가능 합니다.\n여유 공간을 확보해 주세요.
    STR_ID_INVENTORY_FULL_ERROR1             = 6000, // 인벤토리의 공간이 부족합니다.
    STR_ID_INVENTORY_FULL_ERROR2             = 6012, // 인벤토리에 여유 공간이 없습니다.\n아이템을 획득할 수 없습니다.
    STR_ID_INVENTORY_FULL_ERROR3             = 6013, // 인벤토리 공간이 @1칸 이하 입니다.\n아이템 정리가 필요합니다.
    STR_ID_INVENTORY_FULL_ERROR4             = 6014, // #cff0000인벤토리에 여유 공간이 없어서 아이템을 획득할 수 없습니다.#cX
    STR_ID_INVENTORY_FULL_ERROR5             = 6015, // #cff0000아이템 획득 주사위에서 제외됩니다.#cX

    STR_ID_DUPLICATED_FILE_ERROR             = 6057, //중복 파일이 발견되었습니다. 게임을 종료 하겠습니다.

    STR_ID_GUILD_CREATE_ERROR_MSG            = 6090,






    STR_ID_ATTENDANCE_RECOMMAND0            = 6075, // 접속후 @1분이 지났습니다. \n출석체크 시간까지 @2분이 남았습니다.\n 출석부 방문을 해보시겠어요?
    STR_ID_ATTENDANCE_RECOMMAND1            = 6076, // 샵에 방문하시면 출석포인트로 여러 아이템의 구매가 가능합니다.
    STR_ID_ATTENDANCE_RECOMMAND2            = 6077, // 일일 출석 체크를 하실 경우 일일 보상 아이템을 획 to the 득 할 수 있습니다.
    STR_ID_ATTENDANCE_RECOMMAND3            = 6078, // 70분이 경과 하였습니다. 출석부로 이동하여 선물을 받아가세요.
    STR_ID_ATTENDANCE_RECOMMAND4            = 6079, // 주간 개근을 하실 경우 주간 보상 아이템을 획ㅋ득ㅋ 할 수 있습니다.
    STR_ID_ATTENDANCE_CALENDAR2				= 6080, // 출석부 : 남은시간@1분
    STR_ID_NOT_EXIST_PRICE_INFO             = 6107, // 가격 정보가 존재하지 않습니다.
    STR_ID_NOT_CASH_ITEM                    = 6108, // 캐쉬 아이템이 아닙니다.
    STR_ID_BUY_SUCCESS_BUT_INVENTORY_REFRESH_FAIL = 6109, // 구매에는 성공하였으나 인벤토리 갱신이 이루어지지 않았습니다._서버 이동을 하거나 게임을 다시 실행시켜 주세요.

    STR_ID_RAINBOW_ERROR_3					= 6115,//	잭팟 아이템을 이미 받으셨습니다.
    STR_ID_RAINBOW_ERROR_4					= 6116,//	현재 진행되지 않는 이벤트 입니다.
    STR_ID_RAINBOW_ERROR_5					= 6117,//	잭팟 보상 리스트가 존재하지 않습니다.
    STR_ID_SKILLTREE_MSG                    = 6130,//   새로운 스킬을 배울 수 있습니다. 스킬트리 버튼을 클릭하여 확인하세요!


    STR_ID_MARI3_CLASS_NAME                  = 6118,
    STR_ID_MARI3_DOUBLE_ATK                  = 6119,
    STR_ID_MARI3_MAGIC_VOLT                  = 6120,
    STR_ID_MARI3_JUMP_ATK                    = 6121,
    STR_ID_MARI3_JUMP_MAGIC_VOLT             = 6122,
    STR_ID_MARI3_MANA_DEVISION               = 6123,
    STR_ID_MARI3_MANA_CHARGE_MACHIME         = 6124,
    STR_ID_MARI3_MI3                         = 6125,
    STR_ID_MARI3_PORTAL                      = 6126,
    STR_ID_MARI3_MAGIC_BEAD                  = 6127,
    STR_ID_MARI3_SUMMON_JSJ                  = 6128,
    STR_ID_MARI3_JSJ_DASH                    = 6129,

    STR_ID_GUILD_MEMBER_LEVEL_UP_NOTICE      = 6146,
    STR_ID_GS_MARKET_WATER                   = 6151,
    STR_ID_NETWORK_TOO_SLOW                  = 6164,
    STR_ID_NETWORK_HOST_MIGRATED             = 6165,

    STR_ID_DICE_ERROR_01					 = 6153,
    STR_ID_DICE_ERROR_02					 = 6154,
    STR_ID_DICE_ERROR_03					 = 6155,
    STR_ID_DICE_ERROR_04					 = 6156,
    STR_ID_DICE_ERROR_05					 = 6157,
    STR_ID_DICE_ERROR_06					 = 6158,

    STR_ID_GS_RIZAL_PARK					 = 6186,
    STR_ID_HURRYUP							 = 6364, // 님을 재촉하였습니다.
    STR_ID_HURRYUP_OK						 = 6365, // 재촉 대답
    STR_ID_CHANGETO_IDLE					 = 6366, // 자리비움으로 상태 변환


    STR_ID_ARE_YOU_SURE_BUY_CASHITEM_MIX_VIRTUAL_CASH = 6231,    // 이벤트 버프 스트링 7번째꺼, 대만 가상캐쉬 혼용구매 확인메시지로 활용

    STR_ID_TUTORIAL_END_MSG3               = 6232,
    STR_ID_STATUS_INFO                       = 6234,
    STR_ID_STATUS_GRADE                      = 6235,
    STR_ID_SP_ATTACK_POWER                   = 6236,
    STR_ID_SP_DEFENCE_POWER                  = 6237,
    STR_ID_CRITICAL_RATE                     = 6238,
    STR_ID_COUNTER_DEFENCE_RATE              = 6239,
    STR_ID_HMP_RECOVERY_RATE                 = 6240,
    STR_ID_HMP_DRAIN_RATE                    = 6241,
    STR_ID_GP_EXP_PLUS_RATE                  = 6242,
    STR_ID_TUTORIAL_GUIDE_STRING           = 6243,

    STR_ID_HP_DRAIN_RATE                     = 6263,
    STR_ID_MP_DRAIN_RATE                     = 6264,
    STR_ID_GP_PLUS_RATE                      = 6265,
    STR_ID_EXP_PLUS_RATE                     = 6266,
    STR_ID_TUTORIAL_ELESIS_0               = 6267,
    STR_ID_TUTORIAL_ELESIS_1               = 6268,
    STR_ID_TUTORIAL_ELESIS_2               = 6269,
    STR_ID_TUTORIAL_ELESIS_3               = 6270,
    STR_ID_TUTORIAL_LIRE_0                 = 6271,
    STR_ID_TUTORIAL_LIRE_1                 = 6272,
    STR_ID_TUTORIAL_LIRE_2                 = 6273,
    STR_ID_TUTORIAL_LIRE_3                 = 6274,
    STR_ID_TUTORIAL_ARME_0                 = 6275,
    STR_ID_TUTORIAL_ARME_1                 = 6276,
    STR_ID_TUTORIAL_ARME_2                 = 6277,
    STR_ID_TUTORIAL_ARME_3                 = 6278,
    STR_ID_TUTORIAL_LAS_0                  = 6279,
    STR_ID_TUTORIAL_LAS_1                  = 6280,
    STR_ID_TUTORIAL_LAS_2                  = 6281,
    STR_ID_TUTORIAL_LAS_3                  = 6282,
    STR_ID_TUTORIAL_RYAN_0                 = 6283,
    STR_ID_TUTORIAL_RYAN_1                 = 6284,
    STR_ID_TUTORIAL_RYAN_2                 = 6285,
    STR_ID_TUTORIAL_RYAN_3                 = 6286,
    STR_ID_TUTORIAL_RONAN_0                = 6287,
    STR_ID_TUTORIAL_RONAN_1                = 6288,
    STR_ID_TUTORIAL_RONAN_2                = 6289,
    STR_ID_TUTORIAL_RONAN_3                = 6290,
    STR_ID_TUTORIAL_AMY_0                  = 6291,
    STR_ID_TUTORIAL_AMY_1                  = 6292,
    STR_ID_TUTORIAL_AMY_2                  = 6293,
    STR_ID_TUTORIAL_AMY_3                  = 6294,
    STR_ID_TUTORIAL_JIN_0                  = 6295,
    STR_ID_TUTORIAL_JIN_1                  = 6296,
    STR_ID_TUTORIAL_JIN_2                  = 6297,
    STR_ID_TUTORIAL_JIN_3                  = 6298,
    STR_ID_TUTORIAL_SIEG_0                 = 6299,
    STR_ID_TUTORIAL_SIEG_1                 = 6300,
    STR_ID_TUTORIAL_SIEG_2                 = 6301,
    STR_ID_TUTORIAL_SIEG_3                 = 6302,
    STR_ID_TUTORIAL_MARI_0                 = 6303,
    STR_ID_TUTORIAL_MARI_1                 = 6304,
    STR_ID_TUTORIAL_MARI_2                 = 6305,
    STR_ID_TUTORIAL_DIO_0                  = 6306,

    STR_ID_REMAIN_EXP                      = 6367,

    STR_ID_SKILL_INFOBOX_DESC_COOLTIME     = 6368,
    STR_ID_SKILL_INFOBOX_DESC_AP           = 6369,
    STR_ID_SKILL_INFOBOX_DESC_MP           = 6371,

    STR_ID_DIO_NAME                        = 6378, // 디오 

    STR_ID_STATUS_WINDOW_TOGGLE            = 6390,
    STR_ID_MONTHLY_RANKING                 = 6396,
    STR_ID_RANK                            = 6397,

    STR_ID_GASH_CERTIFICATION_FAIL           = 6416,
    STR_ID_BUY_USE_VURTUAL_GASH              = 6423,
    STR_ID_SEND_PRESENT_TO                   = 6425,
    STR_ID_GC_GM_QUEST45                     = 6431,    


    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC4 = 6417, // Shift : 해체 시 생성 아이템 표시
    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC5 = 6418, // 아이템 해체 비용 : @1 GP
    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC6 = 6419, // 해체 시 획득 가능한 아이템입니다.
    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC7 = 6420, // @1 : @2 ~ @3 개
    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC8 = 6421, // 내가 보유한 수량 : @1

    STR_BAD_PING_USER_JOIN_LIMIT_MESSAGE1    = 6434,// 네트워크 상태가 불안정하여 멀티플레이 방에 참여할 수 없습니다.   
    STR_BAD_PING_USER_JOIN_LIMIT_MESSAGE2    = 6435,// 네트워크 상태가 불안정하여 멀티플레이 방을 개설할 수 없습니다.

    STR_ID_GAME_EXIT_NEW1                   = 6436, // 게임 진행 중 나갈 경우 획득 한
    STR_ID_GAME_EXIT_NEW2                   = 6437, // 아이템 및
    STR_ID_GAME_EXIT_NEW3                   = 6438, // 경험치 보상이 모두 지급되지 않습니다. 
    STR_ID_GAME_EXIT_NEW4                   = 6439, // 그래도 나가시겠습니까?

    STR_ID_CHARACTER_SELECT_UP				= 6441,
    STR_ID_CHARACTER_SELECT_OTHER			= 6442,

    STR_ID_MISSION_COMPLETE_TOOLTIP         = 6447, // 선택 보상을 선택하셔야 미션이 완료 됩니다.
    STR_ID_DO_YOU_WANTTO_DELETE			    = 6448, // 삭제하시겠습니까? 
    

    STR_ID_CREATEANDBREAKUP_TOOLTIP         = 6455, // 제작/해체

    STR_ID_GET_NEW_CHARACTER3               = 6456,
    STR_ID_GET_NEW_CHARACTER4               = 6457,
    STR_ID_GET_NEW_CHARACTER5               = 6458,
    STR_ID_GET_NEW_CHARACTER6               = 6459,
    STR_ID_GET_NEW_CHARACTER7               = 6460,
    STR_ID_GET_NEW_CHARACTER8               = 6461,
    STR_ID_GET_NEW_CHARACTER9               = 6462,
    STR_ID_GET_NEW_CHARACTER10              = 6463,

    STR_ID_ITEM_EXTRA_DURATION              = 6464, // 남은 수량 : 
    STR_ID_ITEM_EXTRA_PERIOD                = 6465, // 남은 기간 : 
    STR_ID_SELL_BUNDLE_ITEM                 = 6466, // 리스트 내의 아이템들을 @1 GP / @2 Gem / @3 크리스탈에 판매합니다.

    STR_ID_TIP_SELL_COUNT_ITEM0             = 6467, // TIP : 스크롤을 조작하면 쉽게 수량을 조절할 수 있습니다.
    STR_ID_TIP_SELL_COUNT_ITEM1             = 6468, // TIP : 키보드의 숫자키를 이용하여 수량을 입력할 수 있습니다.

    STR_ID_COORDI_CHANGE0                   = 6471, // 스타일리쉬 쿠폰이 @1개 소비 됩니다.
    STR_ID_COORDI_INVENTORY_FULL_ERROR      = 6472, // 코디 옷장에 남는 공간이 없습니다.
    
    STR_ID_SELL_BUNDLE_ITEM_SUCCESS			= 6473, //@1 GP / @2 Gem / @3 크리스탈을 획득하였습니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL1			= 6474, //@1 아이템은 이미 판매된 아이템입니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL2			= 6475, //@1 아이템은 인벤토리에 없는 아이템 입니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL3			= 6476, //@1 아이템의 인벤토리 정보를 읽어 오지 못했습니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL4			= 6477, //@1 아이템은 PC방 아이템입니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL5			= 6478, //@1 아이템은 수량으로 판매할 수 없습니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL6			= 6479, //@1 아이템은 잘못된 수량 값이 입력되었습니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL7			= 6480, //@1 아이템의 기간 입력이 잘못되었습니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL8			= 6481, //@1 아이템은 출석 포인트 아이템입니다.
    STR_ID_SELL_BUNDLE_ITEM_FAIL_ALL		= 6482, //아이템 판매를 실패하였습니다.

    STR_ID_COORDI_CHANGE1                   = 6483, // 코디 아이템 변경 도중 오류가 발생했습니다. 
    STR_ID_COORDI_CHANGE2                   = 6484, // 스타일리쉬 쿠폰이 부족하여 변환하지 못했습니다.

    STR_ID_COUNTITEM_DLG_TIP1               = 6486, // TIP : 스크롤을 조작하면 쉽게 수량을 조절 할 수 있습니다.
    STR_ID_COUNTITEM_DLG_TIP2               = 6487, // TIP : 키보드의 숫자키를 이용해서 수량을 입력할 수 있습니다.

    STR_ID_DONATION_ITEM1                    = 6488,
    STR_ID_DONATION_ITEM2                    = 6489, 
    STR_ID_USE_DONATION_POINT_ITEM           = 6490,
    STR_ID_DONATION_ITEM_GET_ERROR           = 6492,

    STR_ID_DONATION_ITEM3                    = 6540,
    STR_ID_DONATION_ITEM4                    = 6541,
    STR_ID_DONATION_ITEM5                    = 6555,

    STR_ID_HERO_DUNGEON_ENTER_LIMIT_LEVEL   = 6493,
    STR_ID_HERO_DUNGEON_ENTER_INIT_TIME     = 6494,

    STR_ID_CONTINENT_BERMESIA               = 6495,
    STR_ID_CONTINENT_ELIA                   = 6496,
    STR_ID_CONTINENT_XENIA                  = 6497,
    STR_ID_CONTINENT_SILVERLAND             = 6498,
    STR_ID_CONTINENT_ATUM                   = 6499,
    STR_ID_CONTINENT_ACHAEMEDIA             = 6500,

    STR_ID_HERO_DUNGEON_INFO_DLG_INIT_TIME  = 6503,
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR0    = 6504,
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR1    = 6505,
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR2    = 6506,
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3    = 6507, //다음입장시각

    STR_ID_LEVELUP_MESSAGE                  = 6510, // @1 님의 레벨이 올랐습니다.
    STR_ID_SKILLUP_MESSAGE                  = 6511, // @1 님의 스킬포인트가 올랐습니다.

    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4    = 6512, 
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR5    = 6513,
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR6    = 6514,
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR7    = 6515,

    STR_ID_HERO_DUNGEON_ALRAM_BEFORE_OPEN   = 6516,
    STR_ID_HERO_DUNGEON_ALRAM_OPEN_TIME     = 6517,
    STR_ID_HERO_DUNGEON_ALRAM_BEFORE_CLOSE  = 6518,

    STR_ID_HERO_DUNGEON_START_GAME_ERROR    = 6519,

    STR_ID_HERO_DUNGEON_DESCRIPTION         = 6520,

    STR_ID_HERO_ITEM_BUY_ERROR0             = 6538, // 구매재료 아이템이 없습니다.
    STR_ID_HERO_ITEM_BUY_ERROR1             = 6539,	// 구매재료 아이템이 부족합니다.

    STR_ID_BUY_HERO_ITEM_HERO_COIN          = 6542, // 영웅 주화 @1 개가 소비됩니다.\n구입하시겠습니까?
    STR_ID_BUY_HERO_ITEM_HERO_COIN_PIECE    = 6543, // 영웅 주화 조각 @1 개가 소비됩니다.\n구입하시겠습니까?

    STR_ID_NOT_OPEN_DUNGEON_ENTRANCE_ERROR   = 6547,    
    STR_ID_MAGIC_PROMOTION_1                = 6549, // "@1" 캐릭터를 선택하셨습니다.
    STR_ID_MAGIC_PROMOTION_2                = 6550, // 캐릭터를 전직시키시겠습니까?

    STR_ID_REPETITION_MESSAGE6              = 6551, // 아이템 @1개를 지급 받으셨습니다.\n지금 바로 장비창에 가서 확인해 보세요!

    STR_ID_ERROR_CHAR_PROMOTION_UPDATE      = 6552, // 시스템 오류로 인하여 캐릭터 전직에 실패하였습니다. (오류코드 : @1)
    STR_ID_GC_GM_QUEST46                    = 6553,
    STR_ID_MAGIC_PROMOTION_3                = 6559, // 해당 전직 마법서로 전직할 수 있는 캐릭터가 없습니다.
    STR_ID_FROM_HERO_DUNGEON_TO_DUNGEON     = 6560,
    STR_ID_FROM_DUNGEON_TO_HERO_DUNGEON     = 6561,

    STR_ID_HERO_ITEM_BUY_ERROR2             = 6578, // 코디 옷장에 빈공간이 없어 해당 아이템을 구매할 수 없습니다.
    STR_ID_HERO_ITEM_BUY_ERROR3             = 6579, // 인벤토리에 빈공간이 없어 해당 아이템을 구매할 수 없습니다.
    STR_ID_HERO_ITEM_BUY_ERROR4             = 6580, // 해당 아이템을 구입하기 위한 주화 아이템이 부족합니다.
    STR_ID_HERO_ITEM_BUY_ERROR5             = 6581, // 해당 아이템을 구입하기 위한 영웅 주화 조각이 부족합니다.
    STR_ID_HERO_ITEM_BUY_ERROR6             = 6582, // 코디 인벤토리의 공간이 부족하여 해당 아이템을 구입 할 수 없습니다. 코디 인벤토리의 공간을 확보해주세요.

    STR_ID_NOT_BUY_TOONI                    = 6602,
    STR_ID_EVENT_BANNER                     = 6606, // 이벤트 정보

    STR_ID_REPLAY_RECORD                    = 6607, // 동영상 녹화
    STR_ID_ITEMBOX_ANI                      = 6608, // 아이템박스 애니메이션
    STR_ID_REPLAY_RECORD_PATH               = 6609, // 저장 경로 지정
    STR_ID_REPLAY_RECORD_QUALITY            = 6610, // 녹화 품질 설정
    STR_ID_REPLAY_RECORD_KEY                = 6611, // 단축키 안내
    STR_ID_REPLAY_RECORD_STRAT              = 6612, // 녹화 시작
    STR_ID_REPLAY_RECORD_END                = 6613, // 녹화 종료
    STR_ID_REPLAY_RECORD_CAPACITY           = 6614, // 5분 촬영시 예상 용량 : 약 @1 MB

    STR_ID_REPLAY_RECORD_RESOLUTION         = 6615, // 녹화 해상도
    STR_ID_REPLAY_RECORD_FRAME              = 6616, // 녹화 프레임
    STR_ID_REPLAY_RECORD_CUR_RESOLUTION     = 6617, // 현재 해상도
    STR_ID_REPLAY_RECORD_SOUND              = 6618,  // 소리
    STR_ID_REPLAY_RECORD_BGM                = 6619, // 효과음
    STR_ID_REPLAY_RECORD_SE                 = 6620, // 배경음 

    STR_ID_RECORD_EXPLAIN_TITLE1            = 6621, // #cffff001. 게임을 실행하고 동영상을 촬영하고 싶을 때 Ctrl + 숫자1 키를 동시에 누른다.#cX
    STR_ID_RECORD_EXPLAIN_EXPLAIN1          = 6622, // (동영상 촬영 중에는 게임 화면 우측 하단에 촬영중임을 알리는 #cff0000REC#cX가 깜박거리게 됩니다.
    STR_ID_RECORD_CAPACITY_EXPLAIN          = 6623, // 하드디스크의 용량이 충분히 확보되지 않을 경우 녹화가 안되거나, 녹화 도중 자동정지 될 수 있습니다.
    STR_ID_RECORD_EXPLAIN_TITLE2            = 6624, // cffff002. 촬영을 완료하고 싶을 때 Ctrl + 숫자2 키를 동시에 누른다.#cX\n(동영상 촬영을 종료하게 되면 #cff0000REC#cX가 화면에서 사라집니다.)
    STR_ID_RECORD_EXPLAIN_TITLE3            = 6625, // #cffff003. 촬영이 완료된 폴더에서 동영상을 본다.#cX
    STR_ID_RECORD_EXPLAIN_EXPLAIN3          = 6626, // #cffff004. 저사양 PC에서 동영상 촬영 시 게임이 느려질 수 있습니다.#cX (저사양 PC에서는 녹화품질 옵션을 낮춰서 사용하길 권장합니다.)
    STR_ID_RECORD_EXPLAIN_EXPLAIN4          = 6627, // (동영상 촬영을 종료하게 되면 #cff0000REC#cX가 화면에서 사라집니다.)

    STR_ID_COORDI_WORD_DEFINE               = 6631, // 연결이 끊어졌습니다.

    STR_ID_RETRY_LOGIN_ON_CLOSE_CONNECTION1 = 6632, // 연결이 끊어졌습니다.
    STR_ID_RETRY_LOGIN_ON_CLOSE_CONNECTION2 = 6633, // 현재 네트워크 상태가 좋지 않습니다./nYes를 선택하여 다시 접속하거나,/nNo를 선택하여 게임을 종료하세요.
    STR_ID_REPLAY_VIEW_READY                = 6636, // 게임 내 동영상 재생은\n현재 서비스 준비중입니다.
    STR_ID_TR_CONNECTION_ERROR				= 6638, // TR서버와의 연결이 좋지 않습니다. 게임을 종료 합니다.

    STR_ID_COORDI_CLOSET                    = 6631, // 코디옷장
    STR_ID_GUILD_SEARCH_ONE_TEXT_ERROR      = 6637,
    STR_ID_RECORD_EXPLAIN_EXPLAIN5          = 6639, // #cffff005. 저장된 파일은 그랜드체이스가 설치된 폴더의 Replay 폴더에서 확인할 수 있습니다. #cX 
    STR_ID_REPLAY_BOX_NEW_DESC              = 6640, // 서비스 준비중입니다. \n녹화된 영상은 그랜드체이스가 설치된 폴더의 Replay 폴더에서 확인할 수 있습니다.

    STR_ID_GAWIBAWIBO_MAIN_1				= 8107, //1. 접속후 10분 마다 도전 포인트가 1씩 쌓입니다. 6회 도전마다 재도전포인트가 1씩 쌓입니다.
    STR_ID_GAWIBAWIBO_MAIN_2				= 8108, //2. 도전 포인트를 사용하여 보스와 가위바위보 게임을 즐길 수 있습니다.
    STR_ID_GAWIBAWIBO_MAIN_3				= 8109, //3. 승리시 선물을 받고 다음 보스에게 도전을 진행 할 수 있습니다.
    STR_ID_GAWIBAWIBO_MAIN_4				= 8110, //4. 도전에 실패시 첫보스부터 다시 도전을 진행하게 됩니다. (재도전아이템을 사용하여 즉시 재도전 가능)

    STR_ID_GAWIBAWIBO_MAIN_ITEM_VIEW1		= 8111, //보스 몬스터 클릭시 선물 리스트를 확인 할 수 있습니다.
    STR_ID_GAWIBAWIBO_MAIN_ITEM_VIEW2		= 8112, //모든 보스 클리어시 선물이 더욱 풍성해 집니다.

    STR_ID_GAWIBAWIBO_LOSE_1				= 8113, //안타깝네요!
    STR_ID_GAWIBAWIBO_LOSE_2				= 8114, //다시 힘내서 파이팅!
    STR_ID_GAWIBAWIBO_LOSE_3				= 8115, //확인 : 첫 보스부터 다시 시작
    STR_ID_GAWIBAWIBO_LOSE_4				= 8116, //재도전 : 현재 보스와 재경기
    STR_ID_GAWIBAWIBO_WIN_1					= 8117, //축하합니다~
    STR_ID_GAWIBAWIBO_WIN_2					= 8118, //승리하셨습니다!!
    STR_ID_GAWIBAWIBO_WIN_3					= 8119, //확인 버튼 클릭시 선물 획득 후
    STR_ID_GAWIBAWIBO_WIN_4					= 8120, //다음 보스에게 도전하게 됩니다.
    STR_ID_GAWIBAWIBO_DRAW_1				= 8121, //앗..비겼네요~
    STR_ID_GAWIBAWIBO_DRAW_2				= 8122, //다시 한번 도전!!
    STR_ID_GAWIBAWIBO_DRAW_3				= 8123, //무슨부시에는 도전포인트가
    STR_ID_GAWIBAWIBO_DRAW_4				= 8124, //차감되지 않습니다.
    STR_ID_GAWIBAWIBO_ERR_1					= 8132, //	@1 : 오류가 발생 하였습니다.
    STR_ID_GAWIBAWIBO_ERR_2 				= 8133,	//가위바위보 도전 혹은 재도전 포인트가 부족합니다.
    STR_ID_GAWIBAWIBO_ERR_3					= 8134,	//가위바위보 스크립트에 없는 스테이지 보스입니다.
    STR_ID_GAWIBAWIBO_ERR_4					= 8135,	//가위바위보 정보 DB 기록을 실패 하였습니다.
    STR_ID_GAWIBAWIBO_ERR_5					= 8136,//	가위바위보 아이템 DB 넣기를 실패 하였습니다.
    STR_ID_GAWIBAWIBO_ERR_6					= 8137,//가위바위보 포인트 아이템이 인벤토리에 없습니다.
    STR_ID_GAWIBAWIBO_ERR_7					= 8138,	//가위바위보 포인트 아이템 교환정보가 없습니다.
    STR_ID_GAWIBAWIBO_ERR_8					= 8139,	//가위바위보 재도전을 할 수 없는 상태입니다.
    STR_ID_GAWIBAWIBO_ERR_10				= 8125,	//이벤트 기간이 아닙니다.
    STR_ID_GC_GM_QUEST47                    = 8159,

    STR_ID_IE9_ERR							= 8170,	//익스프롤러 9 사용자의 경우, 상점 캐쉬 아이템 구매시 비정상적인 종료가 발생 할 수 있습니다. 익스프롤러 9의 문제사항이 해결될 때 까지, 웹 구매를 이용을 권장드립니다.
    STR_ID_GUILDMARK_REGISTANCE_ERROR_MSG   = 8186,

    STR_ID_BUY_HERO_ITEM_COIN_MATERIAL      = 8189, // 구입에 다음 아이템이 소비됩니다.\n 영웅 주화 @1개\n @2개\n구입 하시겠습니까?

    STR_ID_MD5_PATCHER_PARAMETER_INFO		= 8190,
    STR_ID_RECORD_EXPLAIN_EXPLAIN6          = 8191, // 과거 리플레이 기능( GCR)은 기존과 동일한 ctrl + 1 키로 사용할 수 있습니다. 

    STR_ID_MISSTION_MODE_1                  = 8211,//미션 진행상태 모드 
    STR_ID_MISSTION_MODE_2                  = 8212,//연계미션 정보 모드

    STR_ID_INIT_ATTRIBUTE_SUC1				=	8192	,	//	@1 이/가 속성초기화 되었습니다.
    STR_ID_INIT_ATTRIBUTE_ERR1				=	8193	,	//	속성을 초기화 할 아이템이 인벤에 없음.
    STR_ID_INIT_ATTRIBUTE_ERR2				=	8194	,	//	속성 초기화할 아이템이 에픽 등급 이상이 아님.
    STR_ID_INIT_ATTRIBUTE_ERR3				=	8195	,	//	속성 초기화할 아이템이 코디아이템임.
    STR_ID_INIT_ATTRIBUTE_ERR4				=	8196	,	//	속성초기화 DB에 기록 하는데 오류 발생.
    STR_ID_INIT_ATTRIBUTE_ERR5				=	8197	,	//	@1 : 알 수 없는 오류 입니다.
    STR_ID_INIT_ATTRIBUTE_ERR6				=	8198	,	//	속성 초기화 아이템이 부족합니다. 상점을 통하여 구입을 하실 수 있습니다.
    STR_ID_INIT_ATTRIBUTE_SUC2				=	8199	,	//	속성 초기화 아이템 '@1를 사용하여 현재 아이템의 속성을 초기화 합니다. 초기화 된 아이템은 속성을 다시 선택 하실 수 있습니다. 속성을 초기화 하시겠습니까?
    STR_ID_GAMEMODE_ERR_1					= 8201,	//채널정보가 맞지 않아서 서버로 이동됩니다.
    STR_ID_INIT_ATTRIBUTE_ERR7					= 8202,	//속성 초기화를 할수 있는 아이템이 아님.
    STR_ID_SOCKS_GET_INFO					= 8213,	//	양말을 걸고 @1분 뒤 선물 받기 가능.
    STR_ID_SOCKS_SYSTEM_INFO				= 8214,	//	던전 및 대전에서 승리 시 오른쪽 빈칸이 채워집니다. 빈칸을 모두 채우면 양말 개수가 늘어납니다.
    STR_ID_SOCKS_PERCENT					= 8215,	//	@1%
    STR_ID_PLAYPARK_EMAIL_ERROR			    = 8216, // 플레이파크 이메일 로그인시에 이메일 중복으로 인한 오류 -800번, 플레이 파크로그인해서 이메일 바꾸라는 안내메시지
    STR_ID_EVENT_CREATE_INFO                = 8217, // [이벤트] ZERO 기념 상자를 제작해서 많은 상품 혜택을 누리세요.
    STR_ID_EVENT_CREATE_ENABLE              = 8218, // [이벤트]ZERO 기념 상자 제작이 가능합니다.

    STR_ID_ZERO_NAME                        = 8219, //제로

    STR_ID_GET_NEW_CHARACTER11              = 8249, // 나의 이름은 제로..제로 제피리움...너의 부름에 그란이 눈을 떴다...
    STR_ID_LIMITED_FUNC_IN_PVP_SERVER		= 8343, // 대회서버에서는 제한된 기능입니다.

    STR_ID_SOCKS_REMAINSEC					= 8250,	//	남은 시간 : @1초

    STR_ID_ITEM_ATTRIBUTE_STRING5_S4        = 8265,
    STR_ID_ITEM_ATTRIBUTE_STRING6_S4        = 8266,

    STR_ID_EXIT_GAME_STRING_S4              = 8289, // 현재 게임이 진행 중 입니다.\n나가시겠습니까?

    STR_ID_WC_STRING3                       = 8293, // WEAPONCHANGE_LV레벨 이상만 사용할 수 있습니다
    STR_ID_WC_STRING5                       = 8294, // 무기 체인지 슬롯을 오픈합니다.\n슬롯 오픈 열쇠 1개가 소모됩니다.
    STR_ID_WC_STRING8                       = 8295, // 보유 : 
    STR_ID_WC_STRING1                       = 8296, // 무기 체인지 슬롯이 오픈 되었습니다!\n\n게임 중 'Caps Lock'키를 이용해\n멋지게 무기를 교체할 수 있습니다.
    STR_ID_WC_STRING4                       = 8297, // 무기 체인지 슬롯 오픈 실패.\n\n슬롯 오픈 열쇠가 부족합니다.
    STR_ID_WC_STRING6                       = 8298, // 사용 키: 'Caps Lock'
    STR_ID_WC_STRING7                       = 8299, // 쿨 타임: 1초
    STR_ID_WC_STRING2                       = 8300, // 캐릭터도 없는데?

    STR_ID_CREATE_CODI_ITEM_ERR             = 8304, //	코디 아이템 제작 실패.\n코디 옷장의 공간이 가득 찼습니다.\n코디 아이템을 삭제하여 옷장 공간을\n확보하면 제작 할 수 있습니다.

    STR_ID_HERO_DUNGEON_MOVE                = 8307, // 실버랜드 대륙으로 이동 되었습니다.

    STR_ID_RANDOM_ATTRIBUTE_1                = 8309,//	일반
    STR_ID_RANDOM_ATTRIBUTE_2                = 8310, //	특수
    STR_ID_RANDOM_ATTRIBUTE_3                = 8311,  //	소비
    STR_ID_RANDOM_ATTRIBUTE_4 = 8312,	//속성 마법 주문서
    STR_ID_RANDOM_ATTRIBUTE_5 = 8313,	//속성 선택
    STR_ID_RANDOM_ATTRIBUTE_6 = 8314,	//속성 리스트
    STR_ID_RANDOM_ATTRIBUTE_7 = 8315,	//선택창에서 @1개의 속성을 직접 선택하거나, 랜덤 선택을 진행할 수 있습니다. 랜덤 선택을 클릭 할 경우 특수 속성이 포함 된 전체 속성 중 @1가지의 속성이 자동 결정 됩니다.
    STR_ID_RANDOM_ATTRIBUTE_8 = 8316,	//초록색의 속성이 부여 되었습니다.\n랜덤 선택을 클릭 시 특수 속성이 포함된 전체 속성 중 @1개의 속성이 자동으로 다시 결정 됩니다. (속성 주문서 소비)
    STR_ID_RANDOM_ATTRIBUTE_9 = 8317,	//천상의 창
    STR_ID_ITEM_ATTRIBUTE_STRING26 = 8318,	//천상의 창 소환 데미지 +
    STR_ID_RANDOM_ATTRIBUTE_11 = 8319,	//속성 랜덤 선택을 진행 할 아이템이 인벤토리에 없습니다.
    STR_ID_RANDOM_ATTRIBUTE_12 = 8320,	//속성 랜덤 선택을 진행 할 수 있는 아이템 등급이 아닙니다.
    STR_ID_RANDOM_ATTRIBUTE_13 = 8321,	//코디아이템은 속성 랜덤 선택을 진행 할 수 없습니다.
    STR_ID_RANDOM_ATTRIBUTE_14 = 8322,	//속성 랜덤 선택 진행 중 DB오류가 발생하였습니다.
    STR_ID_RANDOM_ATTRIBUTE_15 = 8323,	//속성 랜덤 선택을 진행 할 수 있는 아이템이 아닙니다.
    STR_ID_RANDOM_ATTRIBUTE_16 = 8324,	//속성 랜덤 선택을 진행 할 수없습니다.
    STR_ID_RANDOM_ATTRIBUTE_17 = 8325,	//속성 마법 주문서가 없습니다.
    STR_ID_RANDOM_ATTRIBUTE_18 = 8326,	//선택한 속성의 갯수가 틀립니다.

    STR_ID_GC_GM_QUEST48       = 8328,

    STR_ID_LEY_NAME            = 8345,  // 레이 

    STR_ID_GET_NEW_CHARACTER12              = 8404, // 아버님께서는 날 왜 이런 촌구석으로 보내신거야.. 이게 다 디오 그 바보 녀석 때문이야...
    STR_ID_FOLLOW_FRIEND_1 = 8407,//	요청 주기가 너무 짧습니다. 잠시 후 다시 시도해주세요.
    STR_ID_FOLLOW_FRIEND_2 = 8408,//	대상이 채널에 없습니다.
    STR_ID_FOLLOW_FRIEND_3 = 8409,//	대상이 방에 없습니다.
    STR_ID_FOLLOW_FRIEND_4 = 8410,//	대상 유저를 찾을 수 없습니다.
    STR_ID_FOLLOW_FRIEND_5 = 8411,//	대상이 서버에 없습니다.
    STR_ID_FOLLOW_FRIEND_6 = 8412,//	@1 : 알수없는오류
    STR_ID_FOLLOW_FRIEND_7 = 8413,//	따라가기
    STR_ID_FOLLOW_FRIEND_8 = 8414,//	3초 딜레이 

    STR_ID_TO_DAY_NOT_SEE               =8415,//오늘 하루 이 창을 보지 않습니다.
    STR_ID_TO_DAY_NOT_SEE_QUESTION      =8416,//오늘 하루 이창을 보지 않겠습니까?
    STR_ID_CHATTING_WARRING_MSG         =8417,//3월 3일 정기점검 이후로 닉네임에 공백, 특수문자 등이 포함되어 있을 경우 채팅사용에 제한이 있습니다.
    STR_ID_INVALID_NICKNAME             =8418,//적절한 닉네임이 아닙니다.;
    STR_ID_CHATTING_RESTRICTION         =8419,//채팅을 사용할 수 없습니다. 재 접속을 통해 닉네임을 변경 하신 후 채팅을 이용해주세요. 
    STR_ID_GACHA_TITLE_LEVEL    = 8420,
    STR_ID_CHAT_TOOLTIP_2 = 8421,//	전체 대화를 봅니다.
    STR_ID_CHAT_TOOLTIP_3 = 8422,//	전체 대화를 보지 않습니다.
    STR_ID_CHAT_TOOLTIP_4 = 8423,//	일반 대화를 봅니다.
    STR_ID_CHAT_TOOLTIP_5 = 8424,//	일반 대화를 보지 않습니다.
    STR_ID_CHAT_TOOLTIP_6 = 8425,//	귓속말을 봅니다.
    STR_ID_CHAT_TOOLTIP_7 = 8426,//	귓속말을 보지 않습니다.
    STR_ID_CHAT_TOOLTIP_8 = 8427,//	길드원들과의 대화를 봅니다.
    STR_ID_CHAT_TOOLTIP_9 = 8428,//	길드원들과의 대화를 보지 않습니다.
    STR_ID_CHAT_TOOLTIP_10 = 8429,//	현재 있는 곳의 사람들의 대화를 시도합니다
    STR_ID_CHAT_TOOLTIP_11 = 8430,//	타인과 귓속말을 합니다. #cFFFF00단축키 >#cX
    STR_ID_CHAT_TOOLTIP_12 = 8431,//	길드원들과 대화를 합니다. #cFFFF00단축키 @ #cX
    STR_ID_GACHA_COMING_SOON    = 8432,
    STR_ID_GACHA_DESCRIPTION    = 8433,
    STR_ID_CHAT_WHISPER_ERR_01 = 8434,	//상대방이 존재하지 않는 사용자이거나, 메시지를 입력하지 않았습니다.	2 ); // 귓속말 대상 유저 이름이 비었거나 메세지가 없음.:  비정상적인 유지 닉네임 입력 or 메시지 미입력 엔터
    STR_ID_CHAT_WHISPER_ERR_02 = 8435, //	운영자에게 귓속말은 제한되어 있습니다. 1:1 문의를 이용해주세요.	3 ); // 방송용 서버에서 운영자에게 귓속말을 할 수 없음. : 

    STR_ID_RESETSKILL_01        = 8436,//@1의 전직을 포함한 보유한 전체 SP를\n 되돌려 받을 수 있습니다.\n\n#cFFFF00환원 SP : @2
    STR_ID_RESETSKILL_ERROR_01  = 8437,//SP 전체 초기화 아이템이 아닙니다.
    STR_ID_RESETSKILL_ERROR_02  = 8438,//SP 전체 초기화 아이템 수량이 부족합니다.
    STR_ID_RESETSKILL_ERROR_03  = 8439,//보유하지 않은 캐릭터입니다.
    STR_ID_GUILDBATTLE_DESC	= 8441,	//	길드전 랭킹 설명
    STR_ID_GUILD_CONTRIBUTION = 8442,  //기여도 :
    STR_ID_GUILD_BENEFIT_1 = 8443,	//EXP 5% 추가 획득
    STR_ID_GUILD_BENEFIT_2 = 8444,	//길드 회복 포션 상점 이용
    STR_ID_GUILD_BENEFIT_3 = 8445,	//룬 강화 상대 확률 5% 상승
    STR_ID_GUILD_BENEFIT_4 = 8446,	//EXP 10% 추가 획득
    STR_ID_GUILD_BENEFIT_5 = 8447,	//목걸이 강화 상대 확률 5% 상승
    STR_ID_GUILD_BENEFIT_6 = 8448,	//길드 버프 포션 상점 이용
    STR_ID_GUILD_BENEFIT_7 = 8449,	//포션 장착개수 2개 추가
    STR_ID_GUILD_BENEFIT_8 = 8450,	//룬 강화 상대 확률 10% 상승
    STR_ID_GUILD_BENEFIT_9 = 8451,	//목걸이 강화 상대 확률 10% 상승
    STR_ID_GUILD_BENEFIT_10 = 8452,	//EXP 15% 추가 획득


    STR_ID_PLANT_TREE_01 = 8453,	//나무나무 이벤트 기간이 아님.

    STR_ID_PLANT_TREE_02 =8454,	//해당 아이템 인벤토리에 없음.	
    STR_ID_PLANT_TREE_03 =8455,	//나무를 심을수 없는 상태.	
    STR_ID_PLANT_TREE_04 =8456,	//잘못된 ItemID.	
    STR_ID_PLANT_TREE_05 =8457,	//잘못된 대륙 타입.	
    STR_ID_PLANT_TREE_06 =8458,	//물을 줄수 없는 상태.	
    STR_ID_PLANT_TREE_07 =8459,	//거름을 줄수 없는 상태.	
    STR_ID_PLANT_TREE_08 =8460,	//로터리에 등록되어 있지 않음.	
    STR_ID_PLANT_TREE_09 =8461,	//정의되지 않은 행동 Type.	
    STR_ID_PLANT_TREE_10 =8462,	//아직 처리중인 작업입니다.	
    STR_ID_PLANT_TREE_11 =8463,	//나무나무 이벤트 기간이 아님.	
    STR_ID_PLANT_TREE_12 =8464,	//보상을 받을수 있는 나무상태가 아님.
    STR_ID_PLANT_TREE_13 =8465,	//보상정보 가져오기 실패.	
    STR_ID_PLANT_TREE_14 =8466,	//아직 처리중인 작업입니다.	
    STR_ID_PLANT_TREE_15 =8467,	//DB에서 초기 데이터 이미 가져왔음.	
    STR_ID_PLANT_TREE_16 =8468,	//나무나무 이벤트 기간이 아님.	
    STR_ID_PLANT_TREE_17 =8469,	//아직 처리중인 작업입니다.
    STR_ID_PLANT_TREE_18 =8470,	//1. 나무를 심고 물을 주어 자란 나무에서 열매를 획득 해 푸짐한 선물을 받을 수 있습니다.
    STR_ID_PLANT_TREE_19 =8471,	//2. 물 - 나무를 자라게 하는 아이템, 거름 - 일정 확률로 나무가 황금 나무로 변하거나 죽게 됩니다.
    STR_ID_PLANT_TREE_20 =8472,	//3. 신선한 물과 기름진 비료는 던전 플레이를 통하여 일정 확률로 획득 하실 수 있습니다.
    STR_ID_PLANT_TREE_21 =8474,//	초록 나무
    STR_ID_PLANT_TREE_22=8475,//	황금 나무
    STR_ID_PLANT_TREE_23=8476,//	신선한 물
    STR_ID_PLANT_TREE_24=8477,//	기름진 비료
    STR_ID_GUILD_NO_ITEMINFO = 8478,	//구매 아이템 정보가 없습니다
    STR_ID_GUILD_NO_GUILDLEVELLIMIT = 8479,	//길드레벨조건이 안됨
    STR_ID_GUILD_LACK_CONTRIBUTE = 8480,	//개인기여도가 부족함.
    STR_ID_GUILD_OVER_PURCHASELIMIT = 8481,	//일일구매한도를 넘었습니다.
    STR_ID_GUILD_NO_TABINFO = 8482,	//없는 구매탭 정보
    STR_ID_GUILD_GUILDSTORE_COMMENT = 8483,	//상점 코멘트
    STR_ID_WELCOME_FIRST_VISIT  = 8488, //처음 방문을 해주셨네요.\n당신은 원하는 친구를 추천하실 수 있습니다. ^^
    STR_ID_WELCOME_NO_RECOMMEND = 8489,	//당신은 친구 추천을 할 수 없습니다.\n대신 친구에게 추천을 받아 추천상을 받아 보세요. ^^
    STR_ID_ELESIS_INTRODUCE             =8495,//묵직하며 파워 있는 공격기술과 더불어 순식간에 적진을 쓸어 버리는 검기술로 적들에게 있어선 두말할 나위없이 제일 두려운 존재이다. 상대팀의 궁수와 마법사가 활개를 치지 못하도록 항상 팀원의 선두에 서서 적진으로 돌진 할 때 상대팀은 제 기능을 발휘하지 못하고 무너지게 된다. 엘리시스는 기본적으로 궁수의 화살에 대해 80% 보너스 방어점수를 갖고 있으며, 적에게 강한 타격을 받을 시, 딜레이 소모점수가 제일 낮은 캐릭터이다.
    STR_ID_LIRE_INTRODUCE               =8496,//장거리 공격 기술과 더불어 날쌔고 가벼운 움직임으로, 다가오는 적에게 강력한 화살을 발사한다. 기사에게 쉽게 추적당하는 단점이 있지만, 지형의 유리함을 잘 이용하면 쫓아오는 적에게 치명적인 샷을 날릴 수 있는 기회는 반드시 온다. 특히, 당신이 정확한 각도조절에 능숙하다면 또한 기사 뒤에서 보호를 받게 된다면 리르에게 다가오는 것이 얼마나 무모한 짓이었나를 적들 스스로 깨닫게 될 것이다. 2 단 점프는 리르의 가장 큰 특기!!
    STR_ID_ARME_INTRODUCE               =8497,//공격형 레벨 스펠(흑마법)과 방어형 레벨 스펠(백마법) 모두를 자유롭게 구사하는 이상적인 마법사이다. 아르메의 기본 공격은 적들에게 웃음거리 밖에 될 지 모르나 MP가 충전된 상태의 마법사 라면 상황은 다르다. 강력한 라이트닝볼트와 리버스 그래비티 등 전후좌우를 가리지 않고 대대적인 전체마법을 구사하며, 방심하던 적들을 단 한방에 모두 보내버릴 수도 있다. 혹시 방심해서, 이 아가씨에게 MP를 보충할 여유를 주게 된다면, 그에 대한 대가가 얼마나 혹독한지 실감하게 될 것이다. 
    STR_ID_SIEG_INTRODUCE               =8498,//강력한 검술을 기본으로 신체의 모든 능력을 공격에 사용하는 돌파 능력이 뛰어난 캐릭터. 돌파 능력을 뒷받쳐 줄 분노 특성을 가지고 있는 유일한 캐릭터로서 자신이 공격 받거나 적을 공격 할 때 분노가 서서히 차오르게 된다. 분노 상태가 되면 모든 스킬의 공격력이 상승하고 중요 스킬에 추가 공격 판정이 생겨 다수의 적에게 돌진 시 매우 위협적이고 필살기 또한 더욱 강력한 데미지와 추가 타격을 가진 스킬로 업그레이드 된다. 

    STR_ID_RESETSKILL_02        = 8501,//더 이상 되돌려 받을 SP 가 없습니다.
    STR_ID_GUILD_EMPTY = 8520, //길드전 시 QuestionMark에 비어있음 이라고 띄울 스트링
    STR_ID_MISSION_LEVEL = 8521,  //레벨
    STR_ID_MISSION_TITLE = 8522,  //제목
    STR_ID_MISSION_LINK = 8523, //연계
    STR_ID_MISSION_PROGRESS = 8524, //진행상황
    STR_ID_NOT_GUILD_USER = 8527, //가입된 길드가 없습니다.
    STR_ID_GUILD_TAB_ALL_RANKING_DESC = 8535, //전체랭킹은 매일 새벽 8:00에 업데이트 되며 전체 G-Point를 집계한 순위입니다.
    STR_ID_GUILD_BATTLE_ROOM_ENTER_ERROR= 8536, //길드전 선수가 모두 입장하여 진입할 수 없습니다.

    STR_ID_OTHER_CHARACTER_PLAYABLE     =8525,//선택 캐릭터 외 다른 캐릭터도 플레이 가능합니다.

    STR_ID_CERTIFICATION_ERROR01 = 8528,
    STR_ID_CERTIFICATION_ERROR02 = 8529,
    STR_ID_CERTIFICATION_ERROR03 = 8530,
    STR_ID_CERTIFICATION_ERROR04 = 8531,
    STR_ID_CERTIFICATION_ERROR05 = 8532,
    STR_ID_CERTIFICATION_ERROR06 = 8533,
    STR_ID_CERTIFICATION_ERROR07 = 8534,

    STR_ID_NEW_GACHA_SYSTEM_MESSAGE01   = 8537,
    STR_ID_NEW_GACHA_SYSTEM_MESSAGE02   = 8538,
    STR_ID_NEW_GACHA_SYSTEM_MESSAGE03   = 8539,
    STR_ID_NEW_GACHA_SYSTEM_MESSAGE04   = 8540,
    STR_ID_NEW_GACHA_SYSTEM_MESSAGE05   = 8541,

    STR_ID_COORDI_ITEM_PERMANENT        = 8542, //	영구

    STR_ID_CHANGE_NICK_NAME_AGREE3      =8543, //닉네임을 @1로 하시겠습니까?

    STR_ID_FORMAL_GUILD_ENTER_ERROR_MSG = 8544,  //정식 길드 소속 길드원만 입장이 가능합니다.

    STR_ID_FORMAL_GUILD_ENTER_RECEIVE_POINT1 = 8563,  //출석
    STR_ID_FORMAL_GUILD_ENTER_RECEIVE_POINT2 = 8564,  //첫 접속
    STR_ID_FORMAL_GUILD_ENTER_RECEIVE_POINT3 = 8565,  //@1(으)로 @2의 길드포인트를 받았습니다.

    STR_ID_EVENT_POTION_ATTACK          = 8552,//	@1님이 고대 마족의 공격력 향상 포션을 사용하여 30초 동안 공격력이 2배가 되었습니다.
    STR_ID_EVENT_POTION_DEFENSE         = 8553,//	@1 님이 고대 마족의 방어력 향상 포션을 사용하여 30초 동안 방어력이 2배가 되었습니다.
    STR_ID_EVENT_POTION_CRITICAL        = 8554,//	@1 님이 고대 마족의 치명타 향상 포션을 사용하여 30초 동안 치명타가 2배가 되었습니다.
    STR_ID_EVENT_POTION_SUPER_ARMER     = 8555,//	@1 님이 고대 마족의 슈퍼아머 포션을 사용하여 30초 동안 슈퍼아머 상태가 되었습니다.
    STR_ID_EVENT_POTION_ARENA           = 8556,//	@1 님이 고대 마족의 아레나 포션을 사용하여 20초 동안 아레나 상태가 되었습니다.
    STR_ID_EVENT_POTION_ANTI_POTION     = 8557,//	@1 님이 고대 마족의 안티 포션을 사용하여 30초 동안 모든 상태이상 공격을 방어합니다.
    STR_ID_EVENT_POTION_BERSERK         = 8558,//	@1 님이 고대 마족의 광전사 포션을 사용하여 20초 동안 공격력과 치명타가 2배가 되며, 아레나 상태로 전환 되었습니다.


    STR_ID_INCLUDE_SlANG_WOARD          = 8566, //금지단어가 포함되었습니다.
    STR_ID_INCLUDE_DISTINCT_WOARD       = 8567, //특수문자가 포함되었습니다.

    STR_ID_WEAPON_TYPE                  = 8568,// 사용무기 : 
    STR_ID_NICKNAME                     = 8569,// 닉네임   

    STR_ID_CHOICE_DROP_RECEIVE_OK       = 8571,// 선택한 아이템을 모두 획득하였습니다.
    STR_ID_CHOICE_DROP_GUIDE            = 8572,// 아래 아이템 중 @1개를 받을 수 있습니다. 
    STR_ID_SELECT_ALL_ITEM              = 8573,// 아이템을 모두 선택해주세요.

    STR_ID_CHOICE_DROP_ERROR2           = 8579, //선택한 보상이 없습니다.
    STR_ID_CHOICE_DROP_ERROR3           = 8580, //선택한 보상개수가 전체 보상개수보다 많습니다.
    STR_ID_CHOICE_DROP_ERROR4           = 8581, //선택한 보상개수가 박스의 보상가능 개수와 다릅니다.
    STR_ID_CHOICE_DROP_ERROR5           = 8582, //인벤토리에서 해당 박스를 찾을 수 없습니다.
    STR_ID_CHOICE_DROP_ERROR6           = 8583, //선택한 보상이 보상 리스트에 없습니다.
    STR_ID_CHOICE_DROP_ERROR7           = 8584, //코디 인벤토리 공간이 부족합니다.  
    STR_ID_CHOICE_DROP_ERROR9           = 8585, //지급할 아이템 리스트가 없습니다.
    STR_ID_RAINBOW_NEW_ALARM            = 8586, //Rainbow 7 이벤트!! 가 진행 중입니다. 이벤트 타임 매일 30분부터 정각까지

    STR_ID_POSSIBLE_GET_ALL_ITEM        = 8587, // 아래 아이템을 모두 획득할 수 있습니다.
    STR_ID_BEAD_SEAL_CLEAR              = 8591, // 아래 아이템을 모두 획득할 수 있습니다.
    STR_ID_RICKOON_LUCKY_COIN_TOOLTIP   = 8655, //럭키 세븐 이벤트 포인트 입니다. 게임플레이 시 세 개 아이템이 일치 하지  않을 때 마다 1씩 증가하며 5가 되면 반드시 세 개 아이템이 일치하는 럭키 이벤트가 발동 됩니다
    STR_ID_RICKOON_TORNADO_ERROR1       = 8656, //릭쿤 토네이도 이벤트 기간이 아닙니다.
    STR_ID_RICKOON_TORNADO_ERROR2       = 8657, //릭쿤 토네이도 코인 아이템이 없습니다.
    STR_ID_RICKOON_TORNADO_ERROR3       = 8658, //럭키 세븐 이벤트 포인트가 부족합니다.
    STR_ID_RICKOON_TORNADO_ERROR4       = 8659, //시스템 오류입니다.
    STR_ID_RICKOON_TORNADO_ERROR5       = 8660, //꽝 보상 아이템 가져오기 실패.
    STR_ID_RICKOON_TORNADO_ERROR6       = 8661, //보상 아이템 가져오기 실패.
        
    STR_ID_LUCKY_COIN_ACK               = 8594,   //5월의 행운코인을 획득하였습니다. 넷마블 홈페이지에서 행운판을 돌리고 봉인 해제서 60장과 티렉 소환 구슬을 받아보세요!
    STR_ID_MAGIC_BOX_DESC               = 8595, //1. 던전&대전 플레이 혹은, 상점에서 고대의 마력석이나 마법의 가루를 얻을 수 있습니다.\n2. 아이템을 클릭할 때 마다 고대의 마력석이 1개씩 소모 됩니다.\n3. 아이템 획득에 필요한 마력석 숫자가 0이 되면 해당 아이템을 100% 획득할 수 있습니다.\n4. 코우나트의 마력석을 사용하면 랜덤한 확률로 아이템을 즉시 획득할 수 있습니다.\n5. 한번 획득한 아이템을 재 획득 하려면 필요한 마력석 숫자가 조금씩 증가합니다.
    STR_ID_END_EVENT                    = 8596,     //이벤트가 종료되었습니다.

    STR_ID_USER_MIGRATION_TITLE         = 8597,
    STR_ID_USER_MIGRATION_LOGIN_ID      = 8598,
    STR_ID_USER_MIGRATION_EMAIL         = 8599,
    STR_ID_USER_MIGRATION_NICKNAME      = 8600,
    STR_ID_USER_MIGRATION_BIRTHDAY      = 8601,
    STR_ID_USER_MIGRATION_MESSAGE       = 8611,
    STR_ID_USER_MIGRATION_ERROR01       = 8612,
    STR_ID_ANCIENT_STONE                = 8613,     //고대의 마력석
    STR_ID_KOUNAT_STONE                 = 8614,     //코우나트의 마력석
    STR_ID_CURRNET_HAVE_COUNT           = 8615,     //현재 보유 개수
    STR_ID_KOUNAT_MAGIC_BOX_TITLE       = 8616,     //코우나트 마법상자
    STR_ID_PERIOD7                      = 8602,     // 7 일 
    STR_COORDO_SHOP_NPC_SPEECH1         = 8603,     // 후훗..♡
    STR_COORDO_SHOP_NPC_SPEECH2         = 8604,     // 어서오세요! 오늘도 신나는 하루죠?
    STR_COORDO_SHOP_NPC_SPEECH3         = 8605,     // 하암..졸려.. 누가 물건 좀 사주지 않으려나?
    STR_COORDO_SHOP_NPC_SPEECH4         = 8606,     // 와~! 보는 안목이 좀 있는데요? 정말 잘 어울려요!
    STR_COORDO_SHOP_NPC_SPEECH5         = 8607,     // 야호~! 신난다~~!
    STR_COORDO_SHOP_NPC_SPEECH6         = 8608,     // 에고고..어지러워라..
    STR_COORDO_SHOP_NPC_SPEECH7         = 8609,     // 청소하자! 으쌰으쌰!
    STR_COORDO_SHOP_NPC_SPEECH8         = 8610,     // 아이 추워.. 성냥.. 아니, 옷 하나만 사주세요.. 배가 고파요..



    STR_ID_LEY_TUTORIAL2                = 8617,

    STR_ID_VIP_DESC                     = 8619, //이벤트 기간 동안 일정 CASH 이상을 구매한 유저에게\n제공되는 특별한 혜택입니다. 구매 완료 시 즉시 혜택이\n지급 됩니다. 이벤트는 1주 단위로 진행 됩니다. 
    STR_ID_VIP_MY_PUCHASE_AMOUNT        = 8620, //나의 구매 금액
    STR_ID_VIP_FINAL_REWARD             = 8621, //최종 보상
    STR_ID_VIP_NTH_WEEK                 = 8622, //@1주차 
    STR_ID_VIP_PERIOD                   = 8623, //@1년 @2월 @3일 @4:@5분 ~ @6월 @7일 @8:@9분
    STR_ID_VIP_REWARD                   = 8624, //VIP보상
    STR_ID_VIP_INTERVER                 = 8626, //@1원 ~ @2원 
    STR_ID_VIP_USED_MONEY               = 8625, //@1원

    STR_ID_MAGIC_BOX_DESC1              = 8627, //1. 원하는 아이템을 클릭하여 마법상자에 올려 놓습니다.
    STR_ID_MAGIC_BOX_DESC2              = 8628, //2. 사용하기를 통해 고대의 마력석, 코우나트 마력석을 사용할 수 있습니다.
    STR_ID_MAGIC_BOX_DESC3              = 8629, //3. 필요한 마력석 숫자가 0이 되면 아이템을 획득할 수 있습니다.
    STR_ID_MAGIC_BOX_DESC4              = 8630, //4. 코우나트 마력석 사용 시 랜덤한 확률로 즉시 아이템 획득이 가능합니다.
    STR_ID_MAGIC_BOX_DESC5              = 8631, //5. 반복하여 아이템을 획득 할 경우 필요한 마력석 숫자가 조금씩 증가합니다.

    STR_ID_NEW_BUY_USER_DESC            = 8619, //이벤트 기간 동안 일정 CASH 이상을 구매한 유저에게\n제공되는 특별한 혜택입니다. 구매 완료 시 즉시 혜택이\n지급 됩니다. 이벤트는 1주 단위로 진행 됩니다. 

    STR_ID_VIP_NEWBUY_BENEFIT           = 8635, //	이벤트 기간 동안 신규 가입을 한 유저 중 CASH 구매를 처음으로 한 분들에게 제공 되는 혜택입니다. 구매 완료 시 즉시 혜택이 지급 됩니다. 이벤트는 1주 단위로 진행 됩니다.
    STR_ID_VIP_VIP_BENEFIT              = 8636, //	이벤트 기간 동안 일정 CASH 이상을 구매한 유저에게 제공되는 특별한 혜택입니다. 구매 완료 시 즉시 혜택이 지급 됩니다. 이벤트는 1주 단위로 진행 됩니다.
    STR_ID_VIP_NOT_TARGET_NEWBUY        = 8637, //	첫 구매 이벤트 대상자가 아닙니다. VIP 특별 혜택을 노려 보세요.
    STR_ID_VIP_NEWBUY_TARGET            = 8638, //	첫 구매 혜택 대상자입니다. 구매를 완료하면 상단의 아이템이 자동으로 지급 됩니다.
    STR_ID_VIP_BUY_COMPLETE             = 8639, //	구매를 완료하여 상단의 아이템 지급이 완료 되었습니다.
    STR_ID_VIP_VIP_TARGET               = 8640, //	VIP 혜택 대상자입니다. 구매 금액이 충족되면  상단의 아이템이 자동으로 지급 됩니다.
    STR_ID_WC_STRING9                   = 8649, //  일시적인 장애가 발생하였습니다. 잠시 후에 다시 시도해주세요.
    STR_ID_ERROR_MAGICBOX1              = 8650, // 해당 아이템이 리스트에 없습니다. 리스트를 선택한 후 시도해주십시오.
    STR_ID_ERROR_MAGICBOX2              = 8651, // 마법상자를 여는데 필요한 마력석이 아닙니다.
    STR_ID_ERROR_MAGICBOX3              = 8652, // 마력석이 부족합니다.
    STR_ID_ERROR_MAGICBOX4              = 8653, // 일시적인 장애가 발생하였습니다. 잠시 후 다시 시도해주세요.
    STR_ID_ERROR_MAGICBOX5              = 8654, // 아이템이 선택되지 않았습니다. \n 아이템을 먼저 선택해주세요.
    STR_ID_ROOM_TITLE                   = 8691, // 방제목
    STR_ID_PASSWORD                     = 8692, // 비밀번호
    STR_ID_SURVIVAL                     = 8693, // 서바이벌
    STR_ID_SELECT_MAP                   = 8694, // 맵선택
    STR_ID_SELECT_REGION                = 8695, // 대륙 선택
    STR_ID_SELECT_DUNGEON               = 8696, // 던전 선택
    STR_ID_CHAMPION                     = 8697, // Champion


    STR_ID_MOVE_CASH_CARGE_WEB_MSG      = 8662,

    STR_ID_GMC_DOTA                     = 8663, //점령전
    STR_ID_P2P_ABILITY_FIXING           = 8684, //장착 장비 관계없이\n능력치 고정\n공격력 : 5000\n방어력 : 3800\n생명력 : 3000

    STR_ID_REQUIRE_HIGH_LEVEL           = 8686, //레벨이 부족합니다.

    STR_ID_VERIFY_ACCOUNT_ERROR_23 = 8687,

    STR_ID_NEW_GUILD_CREATE_ERROR8      = 8688, //길드이름 중에 금지어가 포함되어 있습니다.
    STR_ID_NEW_CHARACTER_TOOLTIP        = 8689, //새로운 그랜드 체이스 대원을 위해 준비된 선물을 장비창에서 확인해보세요!
    STR_ID_NEW_GUILD_CREATE_ERROR9      = 8690, //길드이름 중에 특수문자가 포함되어 있습니다.
    STR_ID_INVITE_PARTY_MENU            = 8698, //파티초대
    STR_ID_FIRST_OPEN_CALENDAR_GET_POINT= 8700, //#cFFFF00@3#cX 포인트를 받았습니다.

    STR_ID_ITEM_NAME_FULL_HP            = 8726, //최대 생명치 증가
    STR_ID_ITEM_NAME_CRITICAL           = 8727, //크리티컬 확률 증가
    STR_ID_ITEM_NAME_NPC_ATTACKUP       = 8728, //몬스터 공격력 증가
    STR_ID_ITEM_NAME_NPC_DEFANCEUP      = 8729, //몬스터 방어력 증가
    STR_ID_ITEM_NAME_NPC_HPUP           = 8730, //몬스터 생명력 증가
    STR_ID_ITEM_NAME_NPC_SPEEDUP        = 8731, //몬스터 속도 증가

    STR_ID_NOT_ENOUGH_GP                = 8701, //구입 하기에는 GP가 모자랍니다.
    STR_ID_SPHINX_EVENT_1 = 8702, //이 스핑크스님께서 출제하는\n문제를 풀면 푸짐한 상품을 줄께!\n어때? 도전해 볼테야~?!
    STR_ID_SPHINX_EVENT_2 = 8703, //맞으면 O, 틀리면 X를 선택해줘!\n만약 정답을 틀리면\n너를 잡아 먹을테다!! 야옹~!!
    STR_ID_SPHINX_EVENT_3 = 8704, //틀렸어~ 정답은 [O]\n틀렸으니 널 잡아먹겠다~야옹~!
    STR_ID_SPHINX_EVENT_4 = 8705, //틀렸어~ 정답은 [X]\n틀렸으니 널 잡아먹겠다~야옹~!
    STR_ID_SPHINX_EVENT_5 = 8706, //코인수량
    STR_ID_SPHINX_EVENT_6 = 8707, //난이도
    STR_ID_SPHINX_EVENT_7 = 8708, //누적 정답수
    STR_ID_SPHINX_EVENT_8 = 8709, //남은시간
    STR_ID_SPHINX_EVENT_9 = 8713, //누적 정답수 만큼의 보상을 획득할 수 있습니다.

    STR_ID_SPHINX_EVENT_10 = 8716, //이미 DB에서 초기 데이터를 가져왔습니다.
    STR_ID_SPHINX_EVENT_11 = 8717, //스핑크스 OX퀴즈 이벤트 기간이 아닙니다.
    STR_ID_SPHINX_EVENT_12 = 8718, //스핑크스 OX 코인 아이템이 없습니다.
    STR_ID_SPHINX_EVENT_13 = 8719, //퀴즈 문항을 가져오지 못했습니다.
    STR_ID_SPHINX_EVENT_14 = 8720, //시간 정보를 가져오지 못했습니다.
    STR_ID_SPHINX_EVENT_15 = 8721, //아직 처리중인 작업입니다.
    STR_ID_SPHINX_EVENT_16 = 8722, //존재하지 않는 문제입니다.
    STR_ID_SPHINX_EVENT_17 = 8723, //정답을 맞추지 못했습니다.
    STR_ID_SPHINX_EVENT_18 = 8724, //보상정보를 가져오지 못했습니다.
    STR_ID_SPHINX_EVENT_19 = 8725, //유저의 스핑크스 정보를 불러오지 못했습니다.



    STR_ID_PLAY_SAME_COUNTRY_MESSAGE0   = 8755,
    STR_ID_PLAY_SAME_COUNTRY_MESSAGE1   = 8756,
    STR_ID_PLAY_SAME_COUNTRY_MESSAGE2   = 8757,

    STR_ID_ATTENDANCE_GUIDE             = 8758, //어서 상점으로 가서 출석 포인트로 캐쉬 아이템을 구매하세요!\n위쪽의 (아이콘)을 누르시면\n구매 가능한 아이템들이 나와요!

    STR_ID_NOT_BUY_ATTEND_POINT         = 8764,//해당 아이템은 출석 포인트로 구입이 안 되는 아이템입니다.
    STR_ID_MONEYTYPE_CASH_POINT         = 8766,//캐시(포인트)

    STR_ID_DOTA_GAMEITEM_BUY_FAIL1      = 8759, //중복 구매는 불가능 합니다.
    STR_ID_DOTA_GAMEITEM_BUY_FAIL2      = 8760, //보유한 D-Point가 부족합니다.
    STR_ID_DOTA_GAMEITEM_BUY_FAIL3      = 8761, //아이템 슬롯이 꽉찼습니다.
    STR_ID_ITEM_DOTA_MOVE_ARENA         = 8762, //순간이동
    STR_ID_GAME_START_LIMIT             = 8765, //@1 Vs @2 이하로는 게임시작이 불가능합니다.

    STR_ID_GS_DOTA_PARK                 = 8768, //점령전 첫번째 맵
    STR_ID_BAD_PING_USER_END_GAME 		= 8778, // 핑 배드유져 강퇴시 나오는 메시지
    STR_ERR_GAME_START_TIME             = 8779, //게임을 진행할 수 있는 시간이 아닙니다.
    
    STR_ID_NOTHING_EVENT_LIST           = 8796,//현재 진행중인 이벤트가 없습니다.
    STR_ID_MINIGAMES                    = 8797,//mini game

    STR_TEAM_SERDIN                     = 8798, //세르딘
    STR_TEAM_CANABAN                    = 8799, //카나반
    STR_USE_DOTA_GAMEITEM               = 8800, //@1 아이템을 사용하셨습니다.

    STR_ID_DOTA_MODE_1                  = 8819, 
    STR_ID_DOTA_MODE_2                  = 8820, 
    STR_ID_DOTA_MODE_3                  = 8821, 

    STR_GCPOINT_ITEM_ACK_COUNT_ERROR	= 8822,	// 1일 구매 최대 4회를 초과했습니다.

    STR_ID_AGIT_BROWNIE_MENU_GREETING0  = 8824,//안녕~!
    STR_ID_AGIT_BROWNIE_MENU_GREETING1  = 8825,//뭐해?
    STR_ID_AGIT_BROWNIE_MENU_GREETING2  = 8826,//배고파
    STR_ID_AGIT_BROWNIE_MENU_GREETING3  = 8827,//착하아
    STR_ID_AGIT_BROWNIE_MENU_GREETING4  = 8828,//예쁘다
    STR_ID_AGIT_BROWNIE_MENU_OPTION     = 8829,//아지트 정보를 설정하자
    STR_ID_AGIT_BROWNIE_MENU_MOVE       = 8830,//우리 이사 갈까?
    STR_ID_AGIT_BROWNIE_MENU_QUESTION   = 8831,//궁금한 게 있어

    STR_ID_AGIT_BROWNIE_OPTION_MENT     = 8847,//아지트 이름, 최대허용인원, 아지트 공개상태, 그리고 소개 글을 설정할 수가 있어요.
    STR_ID_AGIT_OPTION_NAME             = 8848,//아지트 이름
    STR_ID_AGIT_OPTION_OPEN             = 8851,//공개
    STR_ID_AGIT_OPTION_PASSWORD         = 8855,//비밀번호 설정 (숫자 @1자 이내)
    STR_ID_AGIT_OPTION_NUM_PEOPLE       = 8850,//최대인원
    STR_ID_AGIT_OPTION_MENT             = 8856,//소개글 ( @1/ @2글자 )
    STR_ID_AGIT_OPEN_ALL                = 8852,//전체
    STR_ID_AGIT_OPEN_FRIEND             = 8853,//친구
    STR_ID_AGIT_OPEN_HIDE               = 8854,//비공개

    STR_ID_AGIT_MOVE                    = 8859,//마음을 정했나요?\n짐은 모두 제가 챙기도록 할게요~\n참! 이사비용으로\n별사탕 @1개가 들 거에요!
    STR_ID_AGIT_MOVE_FAIL1              = 8860,//지금은 이사비용이 부족해요.\n아쉽지만 다음에 꼭\n우리 집으로 만들도록 해요!
    
    STR_ID_AGIT_USER_MENU01				= 8885,//파티초대
    STR_ID_AGIT_USER_MENU02				= 8886,//아지트방문
    STR_ID_AGIT_USER_MENU03				= 8887,//친구추가
    STR_ID_AGIT_USER_MENU04				= 8888,//커플맺기
    STR_ID_AGIT_USER_MENU05				= 8889,//신고하기
    STR_ID_AGIT_USER_MENU06				= 8890,//차단하기
    STR_ID_AGIT_USER_MENU07				= 8891,//아지트추방

    STR_ID_AGIT_ITEM_COUNT              = 8892, //보유 개수 
    STR_ID_AGIT_SHOP_PERIOD             = 8893, //기간제 @1일
    STR_ID_BUY_AGIT_SHOP                = 8894, //@1 상품을\n구매하시겠습니까?
    STR_ID_BUY_AGIT_SHOP_NOT_ENOUGH_STAR    = 8895, //보유중인 별사탕이\n부족합니다.
    STR_ID_BUY_AGIT_SHOP_EXCEES_LIMIT       = 8896, //구매하려는 상품의\n보유한도를 초과하였습니다.
    STR_ID_BUY_AGIT_SHOP_NOT_ENOUGH_INVEN   = 8897, //더 이상 새로운 상품을\n구매할 수 없습니더.\n보유중인 상품을 판매해 보세요.
    STR_ID_SELL_AGIT_SHOP               = 8898, //@1 상품을 판매하고\n별사탕 1개를 획득합니다. 
    
    STR_ID_INVITE_AGIT                  = 8899, //초 대 
    STR_ID_VISIT_AGIT                   = 8900, //방 문 	

    STR_ID_NOW_CONNECT_FRIEND           = 8903, //접속중인 친구 (@1명)
    STR_ID_AGIT_LIST_USER_NUM_OPTION    = 8906,//@1명


    STR_ID_AGIT_ENTER_ACK_ERROR_7		= 8910,
    STR_ID_AGIT_ENTER_ACK_ERROR_6		= 8911,
    STR_ID_AGIT_ENTER_ACK_ERROR_8		= 8912,
    STR_ID_AGIT_ENTER_ACK_ERROR_9		= 8913,

    STR_ID_AGIT_LIST_FRIEND_PAGE_DESC	= 8915,//현재 접속중인 친구의 아지트만 표시됩니다.
    STR_ID_AGIT_LIST_FAVORITE_PAGE_DESC	= 8916,//등록된 즐겨 찾기 (@1 / @2)
    
    STR_ID_AGIT_TRAINING_CHAR_SELECT    = 8920,//캐릭터 선택
    STR_ID_AGIT_TRAINING_EXPLEVEL       = 8921,//레벨/경험치
    STR_ID_AGIT_TRAINING_READY_TILE     = 8922,//준비된 기왓장
    STR_ID_AGIT_TRAINING_REMAIN_TILE    = 8923,//남은 기왓장

    STR_ID_SEED_PLANT_DESC0             = 8947,//예쁘게 심기
    STR_ID_SEED_PLANT_DESC1             = 8948,//정성스럽게 심기
    STR_ID_SEED_PLANT_DESC2             = 8949,//사랑 듬뿍 담아 심기
    STR_ID_SEED_NEED_DESC               = 8950,//소비 씨앗
    STR_ID_GROWTH_TIME_DESC             = 8951,//성장 시간
    STR_ID_GET_STAR_DESC                = 8952,//별사탕 획득
    STR_ID_SEED_NUM                     = 8953,//@1개
    STR_ID_GROWTH_TIME                  = 8954,//@1분

    STR_ID_AGIT_MOVE_MENT               = 8858,//예쁜 집으로 이사 가는 건가요?

    STR_ID_AGIT_BROWNIE_QEUSTION_MENT   = 8861,//#c000000무엇이 궁금한가요?#cX
    STR_ID_AGIT_BROWNIE_QUESTION0       = 8862,//별사탕이 필요해
    STR_ID_AGIT_BROWNIE_QUESTION1       = 8863,//씨앗은 어떻게 모아?
    STR_ID_AGIT_BROWNIE_QUESTION2       = 8864,//집 앞에 심어진 나무!

    STR_ID_AGIT_INVITE_MESSAGE          = 8904,

    STR_ID_AGIT_ENTER_PASS              = 8909,//비공개 아지트입니다.\n비밀번호를 입력하세요.

    STR_ID_APOINTSHOP_DESC              = 8936,

    STR_ID_DOTA_TITLE_BUFF              = 8955, // @1님에 의해\n '@2' 칭호가 적용되었습니다.

    STR_ID_AGIT_MOVE_FAIL2              = 8965,//집에 방문중인 손님이 있으면\n이사를 할 수가 없어요.

    STR_ID_AGIT_NAME_DEFAULT            = 8966, //@1의 행복한 아지트 
    STR_ID_AGIT_TODAY                   = 8967, //Today
    STR_ID_AGIT_TOTAL                   = 8968, //Total

    STR_ID_REGISTER_CHATTING_AGIT       = 8969, //채팅 아지트 등록
    STR_ID_INVITE_FRIEND_AGIT           = 8970, //친구 초대 
    STR_ID_CHATTING_AGIT                = 8971, //채팅 아지트
    STR_ID_RANDOM_AGIT                  = 8972, //랜덤 아지트

    STR_ID_PARTY_ACCEPT                 = 8973, //파티 요청이 수락되었습니다.
    STR_ID_PARTY_DENY                   = 8974, //파티 요청이 거절 되었습니다.
    STR_ID_PARTY_INVITE_IMPOSSIBLE      = 8975, //파티 초대를 할 수 없습니다.
    STR_ID_PARTY_SUCCESS                = 8976, //파티가 구성 되었습니다.
    STR_ID_PARTY_INVITE_ERROR           = 8977, //다른 유저의 초대를 받고 있습니다.
    STR_ID_PARTY_EMPTY_LIST             = 8978, //비어 있는 항목이 있습니다.
    STR_ID_PARTY_NO_CHAMPION_DUNGEON    = 8979, //챔피언 모드가 없는 던전입니다.
    STR_ID_NO_HAVE_CHAMPION_ENTER_ITEM  = 8980, //챔피언 모드 입장권을 소유하고 있지 않습니다.

    STR_ID_FULL_AGIT_EXCEPT             = 8981, //꽉 찬 아지트 제외
    STR_ID_HIDE_AGIT_EXCEPT             = 8982, //비공개, 친구공개 아지트 제뢰
    STR_ID_SHOW_NUM_OVER_AGIT           = 8983, //이상만 보기 
    STR_ID_TODAY_RANK_AGIT              = 8984, //오늘의 Today 인기 아지트 
    STR_ID_TOTAL_RANK_AGIT              = 8985, //오늘의 Total 인기 아지트 
    
    STR_ID_PARTY_DUNGEON_MODE_SELECT    = 8986, //모드선택

    STR_ID_BUFF_EXP_LV1                 = 9026, //EXP +10%
    STR_ID_BUFF_GP_LV1                  = 9027, //GP +10%
    STR_ID_BUFF_ATK_LV1                 = 9028, //공격력 +10%
    STR_ID_BUFF_DEF_LV1                 = 9029, //방어력 +10%
    STR_ID_BUFF_HP_LV1                  = 9030, //생명력 +10%

    STR_ID_BUFF_EXP_LV2                 = 9031, //EXP +15%
    STR_ID_BUFF_GP_LV2                  = 9032, //GP +15%
    STR_ID_BUFF_ATK_LV2                 = 9033, //공격력 +15%
    STR_ID_BUFF_DEF_LV2                 = 9034, //방어력 +15%
    STR_ID_BUFF_HP_LV2                  = 9035, //생명력 +15%

    STR_ID_BUFF_EXP_LV3                 = 9036, //EXP +20%
    STR_ID_BUFF_GP_LV3                  = 9037, //GP +20%
    STR_ID_BUFF_ATK_LV3                 = 9038, //공격력 +20%
    STR_ID_BUFF_DEF_LV3                 = 9039, //방어력 +20%
    STR_ID_BUFF_HP_LV3                  = 9040, //생명력 +20%

    STR_ID_BUFF_EXP_LV4                 = 9041, //EXP +30%
    STR_ID_BUFF_GP_LV4                  = 9042, //GP +30%
    STR_ID_BUFF_ATK_LV4                 = 9043, //공격력 +30%
    STR_ID_BUFF_DEF_LV4                 = 9044, //방어력 +30%
    STR_ID_BUFF_HP_LV4                  = 9045, //생명력 +30%

    STR_ID_AGIT_WATCH_FAIL1             = 9046, //

    STR_ID_AGIT_CHANNEL					= 9051, //@1채널
    
    STR_ID_AGIT_ENTER_ACK_ERROR_1		= 9052,	//아지트 입장에 실패하였습니다. ( @1 )

    STR_ID_SET_AGIT_OPTION_SUCCESS      = 9053,//아지트 설정이 변경되었습니다. 
    STR_ID_SET_AGIT_OPTION_FAILED       = 9054,//아지트 설정 변경에 실패하였습니다. 
    STR_ID_INVALID_CHAR_AGIT_NAME       = 9055,//아지트 이름에 금지 단어가 포함되어 있습니다.\n다시 입력해주세요.
    STR_ID_INVALID_LENGTH_AGIT_NAME     = 9056,//아지트 이름 제한 길이를 초과합니다.\n다시 입력해주세요.
    STR_ID_INVALID_CHAR_AGIT_PASSWORD   = 9057,//아지트 비밀번호는 숫자만 지정할 수 있습니다.\n다시 입력해주세요. 
    STR_ID_INVALID_LENGTH_AGIT_PASSWORD = 9058,//아지트 비밀번호 제한 길이를 초과합니다.\n다시 입력해주세요.
    STR_ID_INVALID_CHAR_AGIT_MENT       = 9059,//아지트 인사말에 금지 단어가 포함되어 있습니다.\n다시 입력해주세요.
    STR_ID_INVALID_LENGTH_AGIT_MENT     = 9060,//아지트 인사말 제한 길이를 초과합니다.\n다시 입력해주세요. 
    STR_ID_AGIT_OPTION_TYPE_REGISTER    = 9061,//채팅 아지트를 등록하면, 채팅 아지트 목록을 통해 다른 유저들이 내 아지트로 쉽세 찾아올 수가 있습니다. 
    STR_ID_SELL_AGIT_OBJECT             = 9062,//@1 을 판매하시겠습니까?
    STR_ID_PARTY_DUNGEON_ERROR          = 9063, //던전 플레이 할 수 있는 인원을 초과했습니다.
    STR_ID_SELL_AGIT_OBJECT_SUCCESS     = 9064,//@1 를 뢱득하였습니다.
    STR_ID_ADD_FAVORITE_AGIT_SUCCESS    = 9065,//즐겨 찾기가 추가되었습니다.
    STR_ID_AGIT_REQUEST_FAIL            = 9066,//네트워크가 불안정하여 요청이 실패하였습니다. 잠시 후 다시 이용하세요.
    STR_ID_PLANT_FFLOWER_POT            = 9067,//이벤트기간이므로 별사탕을 두 배(@1개)로 획득하였습니다.

    STR_ID_ENCHANT_SYSTEM_BEFORE_EQUIP      = 8987,
    STR_ID_ENCHANT_SYSTEM_BEFORE_ENCHANT    = 8988,
    STR_ID_ENCHANT_SYSTEM_SUCCESS_ENCHANT   = 8989,
    STR_ID_ENCHANT_SYSTEM_FAILED_ENCHANT    = 8990,
    STR_ID_ENCHANT_SYSTEM_READY_ALCHEMY     = 8991,
    STR_ID_ENCHANT_SYSTEM_AFTER_ALCHEMY     = 8992,
    STR_ID_ENCHANT_SYSTEM_READY_BREAKUP     = 8993,
    STR_ID_ENCHANT_SYSTEM_SUCCESS_BREAKUP   = 8994,
    STR_ID_ENCHANT_SYSTEM_FAILED_BREAKUP    = 8995,

    STR_ID_ENCHANT_SYSTEM_ERROR_00          = 8996, //해당 작업에 필요한 아이템 중 일부가 인벤토리에 존재하지 않습니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_01          = 8997, //해당 강화석은 다른 아이템에서 이미 사용 중입니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_02          = 8998, //강화석의 스킬이 정의되어 있지 안습니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_03          = 8999, //강화석에 장착할 재료아이템이 영구 아이템이 아닙니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_04          = 9000, //잘못된 강화석 아이템 ID 입니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_05          = 9001, //강화 재료의 레벨이 낮습니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_06          = 9002, //설정되지 않은 강화확률 상승 아이템 입니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_07          = 9003, //강화에 필요한 Gem의 개수가 부족합니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_08          = 9004, //강화하려는 레벨의 확률 설정에 문제가 있습니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_09          = 9005, //설정되지 않은 특정 레벨강화 주문서 입니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_10          = 9006, //특정 레벨 강화보다 현재 아이템 레벨이 같거나 높습니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_11          = 9007, //강화석 DB 강화 에러.
    STR_ID_ENCHANT_SYSTEM_ERROR_12          = 9008, //강화석 재료 아이템 DB 삭제 중 에러.
    STR_ID_ENCHANT_SYSTEM_ERROR_13          = 9009, //강화석 보조 아이템 DB 삭제 중 에러.
    STR_ID_ENCHANT_SYSTEM_ERROR_14          = 9010, //등록되지 않은 아이템 해체 주문서입니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_15          = 9011, //아이템 해체가 실패.
    STR_ID_ENCHANT_SYSTEM_ERROR_16          = 9012, //정의되지 않은 합성 타입니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_17          = 9013, //아이템 합성에 실패하였습니다.
    STR_ID_ENCHANT_SYSTEM_ERROR_18          = 9014, //아이템 합성 결과 적용이 실패하였습니다.

    
    STR_ID_ADD_FAVORITE_AGIT_FAIL1      = 9068,//내 아지트를 즐겨찾기에 등록할 수 없습니다. 
    STR_ID_ADD_FAVORITE_AGIT_FAIL2      = 9069,//즐겨찾기 할 아지트 정보가 없습니다. 
    STR_ID_ADD_FAVORITE_AGIT_FAIL3      = 9070,// 아지트 정보가 없습니다. 
    STR_ID_ADD_FAVORITE_AGIT_FAIL4      = 9071,//이미 즐겨찾기에 등록된 아지트 입니다. 
    STR_ID_ADD_FAVORITE_AGIT_FAIL5      = 9072,//더 이상 즐겨찾기에 등록할 수 없습니다. 
    STR_ID_ADD_FAVORITE_AGIT_FAIL6      = 9073,//즐겨찾기 추가 DB작업 실패 

    STR_ID_REMOVE_FAVORITE_AGIT_FAIL1   = 9074,//즐겨찾기 목록에 없습니다. 
    STR_ID_REMOVE_FAVORITE_AGIT_FAIL2   = 9075,//즐겨찾기 제거 DB작업 실패 
    
    STR_ID_REGISTER_AGIT_FAIL1          = 9076,//자신의 아지트에서만 등록할 수 있습니다. 
    STR_ID_REGISTER_AGIT_FAIL2          = 9077,//등록하려는 아지트 정보가 없습니다. 
    STR_ID_REGISTER_AGIT_FAIL3          = 9078,//이미 알리기 아지트에 등록되어 있습니다. 
    STR_ID_REGISTER_AGIT_FAIL4          = 9079,//알리기 아지트 등록 중 DB처리 실패 
    STR_ID_REGISTER_AGIT_FAIL5          = 9080,//알리기 아지트 등록 중 서버처리 실패 
    
    STR_ID_LOADING_AGIT_FAIL1           = 9081,//입장할 아지트 정보가 정확하지 않습니다. 
    STR_ID_LOADING_AGIT_FAIL2           = 9082,//아지트에 입장할 캐릭터 정보가 없습니다. 
    STR_ID_LOADING_AGIT_FAIL3           = 9083,//입장할 아지트 정보가 없습니다. 
    STR_ID_LOADING_AGIT_FAIL4           = 9084,//입장할 아지트 정원이 초과되었습니다. 
    
    STR_ID_BUY_AGIT_MAP_FAIL1           = 9085,//내 아지트에서 구매 가능합니다. 
    STR_ID_BUY_AGIT_MAP_FAIL2           = 9086,//구매할 맵 정보가 서버에 없습니다. 
    STR_ID_BUY_AGIT_MAP_FAIL3           = 9087,//별사탕이 없습니다. 
    STR_ID_BUY_AGIT_MAP_FAIL4           = 9088,//별사탕이 부족합니다. 
    STR_ID_BUY_AGIT_MAP_FAIL5           = 9089,//맵 구매 진행할 아지트 정보가 없습니다. 
    STR_ID_BUY_AGIT_MAP_FAIL6           = 9090,//맵 구매 DB처리 실패 

    STR_ID_SET_AGIT_OPTION_FAIL1        = 9091,//내 아지트에서만 설정을 변경할 수 있습니다. 
    STR_ID_SET_AGIT_OPTION_FAIL2        = 9092,//아지트 이름에 사용할 수 없는 단어가 있습니다. 
    STR_ID_SET_AGIT_OPTION_FAIL3        = 9093,//아지트 이름에 사용할 수 없는 문자가 있습니다. 
    STR_ID_SET_AGIT_OPTION_FAIL4        = 9094,//소개글에 사용할 수 없는 단어가 있습니다. 
    STR_ID_SET_AGIT_OPTION_FAIL5        = 9095,//소개글에 사용할 수 없는 문자가 있습니다. 
    STR_ID_SET_AGIT_OPTION_FAIL6        = 9096,//비밀번호에 사용할 수 없는 문자가 있습니다. 
    STR_ID_SET_AGIT_OPTION_FAIL7        = 9097,//설정을 변경 할 아지트 정보가 없습니다 
    STR_ID_SET_AGIT_OPTION_FAIL8        = 9098,//아지트 이름 길이제한을 초과합니다. 
    STR_ID_SET_AGIT_OPTION_FAIL9        = 9099,//입장제한 인원수를 초과합니다. 
    STR_ID_SET_AGIT_OPTION_FAIL10       = 9100,//정의되지 않은 오픈 설정입니다. 
    STR_ID_SET_AGIT_OPTION_FAIL11       = 9101,//비밀번호 길이제한을 초과합니다. 
    STR_ID_SET_AGIT_OPTION_FAIL12       = 9102,//정의되지 않은 초대설정 입니다. 
    STR_ID_SET_AGIT_OPTION_FAIL13       = 9103,//소개글 길이 제한을 초과합니다. 
    STR_ID_SET_AGIT_OPTION_FAIL14       = 9104,//설정변경 DB처리 실패 

    STR_ID_AGIT_ENTER_USER              = 9105,//@1님이 아지트에 들어왔습니다.
    STR_ID_AGIT_LEAVE_USER              = 9106,//@1님이 아지트를 나갔습니다.

    STR_ID_HARVEST_FLOWER_POT_FAIL1     = 9107,//현재 아지트가 나의 아지트가 아닙니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL2     = 9108,//수확할 화분을 가지고 있지 않습니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL3     = 9109,//수확을 진행할 아지트 정보가 없습니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL4     = 9110,//수확할 화분 정보가 없습니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL5     = 9111,//수확 가능한 상태가 아닙니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL6     = 9112,//수확 사능한 시간이 아닙니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL7     = 9113,//정의되지 않은 씨앗 심기 타입입니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL8     = 9114,//수확할 화분의 정보가 서버와 다릅니다. 
    STR_ID_HARVEST_FLOWER_POT_FAIL9     = 9115,//화분 초기화 DB처리 실패 

    STR_ID_AGIT_PASSWORD_LIMIT          = 9116,//#cFAF8F0(숫자 6자 이내)#cX
    STR_ID_AGIT_INVITE_FRIEND_MGS       = 9117,//선택한 친구들에게 초대 메시지가 전달되었습니다. 

    STR_ID_BUY_AGIT_ITEM_FAIL1          = 9118,//구매 아이템 정보가 없습니다. 
    STR_ID_BUY_AGIT_ITEM_FAIL2          = 9119,//구매 아이템 재료정보가 없습니다.
    STR_ID_BUY_AGIT_ITEM_FAIL3          = 9120,//구매 재료 아이템이 인벤토리에 없습니다 
    STR_ID_BUY_AGIT_ITEM_FAIL4          = 9121,//구매 재료 영구 아이템이 없습니다. 
    STR_ID_BUY_AGIT_ITEM_FAIL5          = 9122,//구매 재료 수량아이템의 수량이 부족합니다. 
    STR_ID_BUY_AGIT_ITEM_FAIL6          = 9123,//아지트 아이템을 구매하기위한 인벤토리 공간이 부족합니다. 
    STR_ID_BUY_AGIT_ITEM_FAIL7          = 9124,//DB작업 실패 
    STR_ID_BUY_AGIT_ITEM_FAIL8          = 9125,//해당 아이템 보유량 제한입니다. 

    STR_ID_AGIT_ITEM_SELL_FAIL1         = 9134,//판매할 아이템 리스트가 비어있습니다. 
    STR_ID_AGIT_ITEM_SELL_FAIL2         = 9135,//인벤토리에 해당 아이템이 없습니다. 
    STR_ID_AGIT_ITEM_SELL_FAIL3         = 9136,//해당 아이템은 아지트 물품이 아닙니다. 
    STR_ID_AGIT_ITEM_SELL_FAIL4         = 9137,//아이템 판매 서버처리 실패 
    STR_ID_AGIT_ITEM_SELL_FAIL5         = 9138,//아이템 판매 DB처리 실패
    STR_ID_AGIT_ITEM_SELL_FAIL6         = 9139,//화분 아이템은 판매할 수 없습니다. 

    STR_ID_SEED_PLANT_CONFIRM           = 9140,//별사탕이 성장 완료까지 @1분이 남았습니다. 씨앗을 새로 심겠습니까?

    STR_ID_AGIT_OBJECT_POS_SAVE_FAIL1   = 9141,//내 아지트가 아닙니다. 
    STR_ID_AGIT_OBJECT_POS_SAVE_FAIL2   = 9142,//배치하려는 아이템이 인벤토리에 없습니다. 
    STR_ID_AGIT_OBJECT_POS_SAVE_FAIL3   = 9143,//오브젝트 아이템이 아닙니다. 
    STR_ID_AGIT_OBJECT_POS_SAVE_FAIL4   = 9144,//아이템을 배치할 아지트 정보가 없습니다. 
    STR_ID_AGIT_OBJECT_POS_SAVE_FAIL5   = 9145,//오브젝트 배치 DB처리 실패 

    STR_ID_BUY_AGIT_MAP_FAIL7			= 9146,//동일한 집으로 이사를 갈수 없습니다.

    STR_ID_ENCHANT_SYSTEM_START         = 9147,//강화시스템 시작 메시지

    STR_ID_AGIT_GUESTBOOK_NUMBER        = 9148, //No.@1
    STR_ID_AGIT_GUESTBOOK_TIME_AM       = 9149, //오전
    STR_ID_AGIT_GUESTBOOK_TIME_PM       = 9150, //오후 

    STR_ID_INVALID_TEXT_LENGTH          = 9151, //제한 길이를 초과했습니다. 다시 확인하고 작성해 주세요.  

    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL1   = 9152, //현재 상태가 아지트 상태가 아닙니다. 
    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL2   = 9153, //아지트 정보가 없습니다. 
    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL3   = 9154, //낙서장에 추가 할 글 내용이 비었습니다.  
    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL4   = 9155, //낙서장에 추가 할 글의 길이가 제한 길이를 초과합니다 .
    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL5   = 9156, //낙서장에 추가 할 글에 불량단어가 포함되어 있습니다.  
    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL6   = 9157, //새글쓰기 DB처리 실패  
    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL7   = 9158, //10초 이내에 연속으로 낙서를 저장할 수 없습니다. 

    STR_ID_DELETE_GUESTBOOK_MSG         = 9159, //선택한 낙서를 완전히 삭제합니다. 

    STR_ID_AGIT_GUESTBOOK_DELETE_FAIL7  = 9160,//삭제할 낙서장 글 번호가 비어있습니다. 
    STR_ID_AGIT_GUESTBOOK_DELETE_FAIL8  = 9161,//주인이 아닌 사람은 다른 사람의 글을 지울 수 없습니다.
    STR_ID_AGIT_GUESTBOOK_DELETE_FAIL9  = 9162,//낙서장 글 삭제 DB처리 실패 

    STR_ID_LEVEL_EXP_INFO               = 9163,//Lv. @1 / Exp @2%
    
    STR_ID_AGIT_GUESTBOOK_WRITE_FAIL8   = 9164,//낙서장 글쓰기 일일 제한수를 초과했습니다. 
    STR_ID_MATCH_NOT_MAP                = 9166,

    STR_ID_AGIT_MENU_PLACE              = 9170,//배치
    STR_ID_AGIT_MENU_SHOP               = 9171,//구매
    STR_ID_AGIT_MENU_INVITE_VISIT       = 9172,//초대/방문
    STR_ID_AGIT_MENU_PARTY              = 9173,//파티UI On/Off
    STR_ID_AGIT_MENU_MSN                = 9174,//메신저
    STR_ID_AGIT_MENE_ADD_FAVORITE       = 9175,//즐겨찾기 추가
    STR_ID_AGIT_MENU_RETURN             = 9176,//내 아지트 가기

    STR_ID_ENCHANT_SYSTEM_BREAK_BROKEN  = 9167,//해체실패 + 강화석 깨짐
    STR_ID_ENCHANT_SYSTEM_ALCHEMY_FAILED= 9168,//
    STR_ID_ENCHANT_SYSTEM_ALCHEMY_NORMAL= 9169,//일반연금성공

    STR_ID_ENCHANT_ATTRIBUTE_TITLE      = 9177,
    STR_ID_ENCHANT_ATTRIBUTE_DESC01     = 9178,
    STR_ID_ENCHANT_ATTRIBUTE_DESC02     = 9179,
    STR_ID_ENCHANT_ATTRIBUTE_DESC03     = 9180,
    STR_ID_ENCHANT_ATTRIBUTE_DESC04     = 9181,
    STR_ID_ENCHANT_ATTRIBUTE_DESC05     = 9182,
    STR_ID_ENCHANT_ATTRIBUTE_DESC06     = 9183,
    STR_ID_ENCHANT_ATTRIBUTE_DESC07     = 9184,
    STR_ID_ENCHANT_ATTRIBUTE_DESC08     = 9185,
    STR_ID_ENCHANT_ATTRIBUTE_DESC09     = 9186,
    STR_ID_ENCHANT_ATTRIBUTE_DESC10     = 9187,
    STR_ID_ENCHANT_ATTRIBUTE_DESC11     = 9188,
    STR_ID_ENCHANT_ATTRIBUTE_DESC12     = 9189,
    STR_ID_ENCHANT_ATTRIBUTE_DESC13     = 9190,
    STR_ID_ENCHANT_SYSTEM_ALCHEMY_EPIC  = 9192,//노말연금성공

    STR_ID_AGIT_CHARACTER_OPEN          = 9196, //별사탕 @1개로 캐릭터 배치를 오픈 하시겠습니까?
    STR_ID_AGIT_CHARACTER_OPEN_FAIL2    = 9197, //보유중인 캐릭터가 아닙니다. 
    STR_ID_AGIT_CHARACTER_OPEN_FAIL6    = 9198, //캐릭터 오픈 DB처리 실패
    STR_ID_AGIT_CHARACTER_OPEN_FAIL7    = 9199, //이미 오픈된 캐릭터입니다. 

    STR_ID_ENCHANT_SYSTEM_FAILED_ENCHANT2  = 9200,//노말연금성공
    STR_ID_ENCHANT_SYSTEM_FAILED_ENCHANT5  = 9201,//노말연금성공
    STR_ID_ENCHANT_SYSTEM_FAILED_ENCHANT3  = 9202,//노말연금성공

    STR_ID_ENCHANT_SYSTEM_TOTAL_GEM        = 9203,
    STR_ID_ENCHANT_SYSTEM_COST_GEM         = 9204,

    STR_ID_SHUTDOWN_MSG_1                = 9206,//고객님은 만 @1세 미만으로 셧다운제 대상이기 때문에 @2시 ~ @3시 동안 게임 이용이 제한되오니, 게임 이용에 참고 부탁 드립니다. 셧다운 1시간 전입니다.
    STR_ID_SHUTDOWN_MSG_2                = 9207,//고객님은 만 @1세 미만으로 셧다운제 대상이기 때문에 @2시 ~ @3시 동안 게임 이용이 제한되오니, 게임 이용에 참고 부탁 드립니다. 셧다운 30분 전입니다.
    STR_ID_SHUTDOWN_MSG_3                = 9208,//고객님은 만 16세 미만으로 셧다운제 대상이기 때문에 00시 ~ 06시 동안 게임 이용이 제한됩니다.\n
    STR_ID_SHUTDOWN_MSG_4                = 9209,//06시 이후부터 게임 이용이 가능하오니 이점 양해 부탁 드립니다.
    
    STR_ID_ENCHANT_SYSTEM_ENCHANT_DESC_01  = 9210, //0~4강화 단계까지
    STR_ID_ENCHANT_SYSTEM_ENCHANT_DESC_02  = 9211, //5~6강화 단계까지
    STR_ID_ENCHANT_SYSTEM_ENCHANT_DESC_03  = 9212, //7~8강화 단계까지
    STR_ID_ENCHANT_SYSTEM_ENCHANT_DESC_04  = 9213, //9강화 이상	

    STR_ID_AGIT_CHAR_SET_COORDI_FAIL8   = 9214,//장착시킬 코디 아이템을 인벤토리에서 찾을 수 없습니다. 
    STR_ID_AGIT_CHAR_SET_COORDI_FAIL9   = 9215,//장착 요청할 코디 아이템 정보가 비어있습니다. 
    STR_ID_AGIT_CHAR_SET_COORDI_FAIL13  = 9216,//오픈되어 있지 않은 캐릭터 타입입니다. 
    STR_ID_AGIT_CHAR_SET_COORDI_FAIL16  = 9217,//캐릭터 코디 저장 DB 처리 실패 

    STR_ID_MARI4_CLASS_NAME                = 9218,
    STR_ID_TUTORIAL_MARI_3                 = 9219,

    STR_ID_MARI4_MEGA_LAUNCHER             = 9220,
    STR_ID_MARI4_MEGA_LAUNCHER_SHOT        = 9221,
    STR_ID_MARI4_REPAIR                    = 9222,
    STR_ID_MARI4_DRAIN                     = 9223,
    STR_ID_MARI4_CANCELER                  = 9224,
    STR_ID_MARI4_BARRIER                   = 9225,
    STR_ID_MARI4_AIRDASH                   = 9226,
    STR_ID_MARI4_BOOSTER                   = 9227,
    STR_ID_MARI4_BOOSTER_BACK              = 9228,
    STR_ID_MARI4_SOULSPEAR                 = 9229,

    STR_ID_ENCHANT_SYSTEM_EQUIP_EMPTY      = 9230,
    STR_ID_ENCHANT_SYSTEM_ENCHANT_EMPTY    = 9231,
    STR_ID_ENCHANT_SYSTEM_ALCHEMY_EMPTY2   = 9232,

    STR_ID_ENCHANT_MENU_TOOL_TIP		= 9233,	//강화

    STR_ID_AGIT_CHAR_ACTION_SETTING        = 9234,//캐릭터 액션 설정 
    STR_ID_AGIT_CHAR_ACTION_SET_FAIL15     = 9245,//캐릭터 액션 설정 DB처리 실패 

    STR_ID_AGIT_CHAR_MENT_SETTING       = 9246,//캐릭터 대사 설정
    STR_ID_AGIT_CHAR_MENT_NORMAL        = 9247,//일반 대사
    STR_ID_AGIT_CHAR_MENT_CLOSE         = 9248,//근접 대사
    STR_ID_AGIT_CHAR_MENT_CLICK         = 9249,//클릭 대사 
    STR_ID_AGIT_CHAR_MENT_DEFAULT0      = 9250,//안녕~
    STR_ID_AGIT_CHAR_MENT_DEFAULT1      = 9251,//어서와!
    STR_ID_AGIT_CHAR_MENT_DEFAULT2      = 9252,//날씨가 참 좋지?
    STR_ID_AGIT_CHAR_MENT_SET_FAIL10    = 9253,//설정한 대사 정보가 비어있습니다. 
    STR_ID_AGIT_CHAR_MENT_SET_FAIL11    = 9254,//설정한 대사에 사용할 수 없는 단어가 있습니다. 
    STR_ID_AGIT_CHAR_MENT_SET_FAIL12    = 9255,//설장할 대사가 길이 제한을 초과합니다. 
    STR_ID_AGIT_CHAR_MENT_SET_FAIL17    = 9256,//캐릭터 대사 저장 DB처리 실패 


    STR_ID_ENCHANT_SYSTEM_ALCHEMY_EMPTY1   = 9257,
    STR_ID_ENCHANT_SYSTEM_BREAKUP_EMPTY    = 9258,

    STR_ID_AGIT_BROWNIE_QUESTION3          = 9260,//캐릭터 모션을 하고 싶어 
    STR_ID_RONAN_JUMP_DOWN_ATK             = 9262,

    STR_ID_AGIT_CHARACTER_SER_POS_FAIL14   = 9263,//캐릭터 배치 DB처리 실패 
    STR_ID_AGIT_CHARACTER_SER_POS_FAIL18   = 9264,//캐릭터 배치 최대 제한수를 초과합니다. 
    STR_ID_AGIT_CHARACTER_SER_POS_FAIL19   = 9265,//캐릭터를 배치할 맵 정보가 없습니다. 
    
    STR_ID_COORDI_RANDOM_BOX            = 9165, // 랜덤박스		

    STR_ID_USE_TRAINING_OBJ_FAIL3       = 9277,//훈련소 아이템이 인벤토리에 없습니다. 
    STR_ID_USE_TRAINING_OBJ_FAIL4       = 9278,//훈련소 아이템이 아닙니다. 
    STR_ID_USE_TRAINING_OBJ_FAIL5       = 9279,//재료 아이템이 인벤토리에 없습니다. 
    STR_ID_USE_TRAINING_OBJ_FAIL6       = 9280,//재료 아이템 수량이 부족합니다. 
    STR_ID_USE_TRAINING_OBJ_FAIL8       = 9281,//등록된 훈련소가 아닙니다. 
    STR_ID_USE_TRAINING_OBJ_FAIL9       = 9282,//훈련할 수 있는 횟수가 없습니다. 
    STR_ID_USE_TRAINING_OBJ_FAIL10      = 9283,//실제 훈련할 수 있는 횟수가 없습니다. 
    STR_ID_USE_TRAINING_OBJ_FAIL11      = 9284,//훈련소 사용 DB기록 실패.

    STR_ID_FAIRY_TREE_LEVEL             = 9285,//#cCCF43E요정의 나무#cX @1단계

    STR_ID_EVENT_DUNGEON                = 9286,//이벤트 던전
    STR_ID_GC_GM_QUEST49                = 9287,//카니발
    STR_ID_ERROR_DUNGEON_ENTER_TICKET   = 9297,//던전 입장에 필요한 입장권을 보유하고 있지 않습니다.
    STR_ID_VIP_NEW_USER_BENEFIT         = 9309, //신규 구매 고객 혜택
    STR_ID_SURVEY_INTRODUCE_MAIN        = 9320,
    STR_ID_SURVEY_INTRODUCE_SUB         = 9321,
    STR_ID_SURVEY_INTRODUCE_SURVEY_NUM  = 9322,
    
    STR_ID_ATTRIBUTE_INFO               = 9319, //속성정보  

    STR_ID_CHARACTER_DIFFICULT          = 9381, //난이도 : 
    STR_ID_SECOND_TIME                  = 9363, //@1초
    STR_ID_EVENT_DUNGEON_DESCRIPTION    = 9364,//이벤트던전 입장 설명

    STR_ID_GMC_HALLOWEEN_PUMPKIN       = 9404, // 할로윈 호박 깨기
    STR_ID_DESC_HALLOWEENPUMPKIN_1     = 9405, // 상대의 할로윈 호박을 먼저 파괴하는 팀이 승리합니다. 
    STR_ID_DESC_HALLOWEENPUMPKIN_2     = 9406, // 아레나된 플레이어는 5초 후 맵의 중간에서 다시 살아납니다. 
    STR_ID_HALLOWEEN_PUMPKIN           = 9407, // 할로윈 호박 



    STR_ID_NOT_EVENT_PERIOD             = 9412, //이벤트 기간이 아닙니다. 	
    STR_ID_ALREADY_WORKING              = 9413, //아직 처리 중인 작업 입니다
    STR_UNKWON_ERROR                    = 9414, //알수 없는 에러
    STR_ID_REWARD_ALREAY_RECEIVED       = 9415, //1일 1회 보상 아이템을 이미 받았음

    STR_ID_COORDI_SETITEM               = 9419, // 코디 세트
    STR_ID_COORDI_HELMET                = 9420, // 코디 투구
    STR_ID_COORDI_JACKET                = 9421, // 코디 상의
    STR_ID_COORDI_PANTS                 = 9422, // 코디 하의
    STR_ID_COORDI_GLOVES                = 9423, // 코디 장갑
    STR_ID_COORDI_SHOES                 = 9424, // 코디 신발
    STR_ID_COORDI_MANTLE                = 9425, // 코디 망토
    STR_ID_COORDI_A_UPPER_HELMET        = 9426, // 코디 머리위 액세사리
    STR_ID_COORDI_A_DOWN_HELMET         = 9427, // 코디 머리아래 액서사리
    STR_ID_COORDI_A_JACKET              = 9428, // 코디 상의 액세사리
    STR_ID_COORDI_A_PANTS               = 9429, // 코디 하의 액세사리
    STR_ID_COORDI_A_ARM                 = 9430, // 코디 팔 액세사리
    STR_ID_COORDI_WEAPON                = 9431, // 코디 무기	
    STR_ID_COORDI_NECKLET               = 9432,
    STR_ID_EVENT_TIME                   = 9433, // 이벤트 시간
    STR_ID_CLICK_ON_THE_CLOCK_TO_GET_THE_REWARD = 9434, // 시계를 클릭하여 보상을 받으세요!!
    STR_ID_ENCHANT_SYSTEM_ERROR_19      = 9435, //PC방 아이템은 강화할 수 없습니다. 
    STR_ID_ENCHANT_SYSTEM_ERROR_20      = 9436, //강화석 스킬 읽어오기에 실패했습니다. 
    STR_ID_ENCHANT_SYSTEM_ERROR_21      = 9437, //강화석을 장착할 수 없는 아이템입니다. 
    STR_ID_COORDI_A_SHOES               = 9438,
    STR_ID_ENCHANT_ATTRIBUTE_DESC14     = 9439,
    STR_ID_FAILED_INCORRECT_USERID      = 9442,//사용자 ID가 없어서 결제 실패.
    STR_ID_SYSTEM_ERROR                 = 9443,//시스템 에러.
    STR_ID_NOT_ENOUGH_CASH              = 9444,//금액부족.
    STR_ID_NO_PRODUCT_INFORMATION       = 9445,//상품정보 없음.
    STR_ID_PRICE_DIFFERENT_DB           = 9446,//DB에 등록된 가격과 상품 가격이 다름.
    STR_ID_NO_REDUCTION_CASH            = 9447,//캐쉬 차감 실패.
    
    STR_ID_COORDI_NORMAL_COMPOSE        = 9454, // 일반 합성
    STR_ID_COORDI_RANDOM_COMPOSE        = 9455, // 랜덤 합성
    STR_ID_COORDI_COMPOSE_GUIDE1        = 9456, // 외형 아바타의 디자인에 내형 아바타의 속성과 셋트 옵션을 가진 아바타로 합성이 됩니다.

    STR_ID_SYSTEM_POST_LETTER          = 9460, // #cD5D2AF시스템#cX
    STR_ID_NORMAL_POST_LETTER          = 9461, // #cD5D2AF일반#cX
    STR_ID_SELECT_ALL                  = 9462, // #cD5D2AF모두 선택#cX
    STR_ID_LETTER_TITLE                = 9463, // 제목 :  @1
    STR_ID_LETTER_GOODS                = 9464, // 첨부물품 : @1 
    STR_ID_READ_LETTER_DATE            = 9465, // #cD5D2AF날     짜#cX
    STR_ID_READ_LETTER_FROM            = 9466, // #cD5D2AF보낸 사람#cX
    STR_ID_READ_LETTER_GOODS           = 9467, // #cD5D2AF첨부된 물품#cX 

    STR_ID_GC_GM_QUEST50                = 9468, //이벤트 릴레이 던전
    STR_ID_ENCHANT_BUY_GEM              = 9469, //Gem 구입 하기
    
    STR_ID_NEED_DUNGEON_TICKET          = 9471, //	* #cFFFF00던전 입장권#cX 보유 시 입장 가능합니다.\n* 상점의 훈련소에서 #cFFFF00일일 3회#cX 입장권 구입\n  이 가능합니다.
    STR_ID_USE_EVENT_PEPERO             = 9474, //@1님이 수호의 빼빼로를 사용 하셨습니다.	

    STR_ID_DELETE_LETTER_NO_ITEM       = 9475, // 이 편지를 삭제하시겠습니까? 편지함에서 영구 삭제되어 다시 볼 수 없어 집니다. 
    STR_ID_DELETE_LETTER_WITH_ITEM     = 9476, // 첨부된 물품이 있습니다. 이 편지를 삭제하시겠습니까? 편지함에서 영구 삭제 되어, 첨부된 물품을 다시 찾을 수 없어 집니다. 
    STR_ID_DELETE_LETTER_CONFIRM       = 9477, // 편지가 삭제되었습니다. 

    STR_ID_AGIT_INVEN_FULL              = 9482, //아지트 인벤토리에 여유 공간이 없습니다. 
    STR_ID_GC_GM_QUEST51                = 9483, //이벤트 영웅던전
    STR_ID_CURRENT_QUANTITY_OF_RIBBON   = 9485, // 보유 리본 갯수
    STR_ID_CURRENT_MY_GOOD_JOB_POINT    = 9486, // 착한 일 포인트
    STR_ID_NOT_ENOUGH_RIBBON            = 9494, // 필요한 리본 갯수가 부족합니다.
    STR_ID_TYPE_YOUR_FRIENDS_NAME       = 9495, // 선물 받을 친구의 닉네임을 입력해 주세요.\n 같은 서버에 접속 해 있는 친구에게만 선물이 가능합니다.
    STR_ID_HOW_MANY_DONATE_YOUR_RIBBON  = 9496, // 몇 개의 리본을 기부하시겠습니까? (리본 10개당 착한 일 포인트 @1점이 올라갑니다.)
    STR_ID_NO_EXISTED_NICKNAME          = 9497, // 존재하지 않는 닉네임 입니다.
    STR_ID_SUCESS_TO_SEND_THE_GIFT      = 9498, // @1님에게 선물을 성공적으로 전달 하였습니다. \n 착한 일 포인트를 @2점 획득 하였습니다.
    STR_ID_YOU_CANNOT_SEND_THE_GIFT_TO_YOURSELF = 9499, // 자신에게는 선물할 수 없습니다.
    STR_ID_DONATION_RESULT              = 9500, // @1개의 리본을 기부하여 @2 포인트를 획득하였습니다.
    STR_ID_GIFT_MESSAGE                 = 9501, // @1님이 선물을 보냈습니다.

    STR_ID_PARTY_BREAK                  = 9502, //파티탈퇴
    STR_ID_PARTY_CHANGE_CAPTAIN         = 9503, //파티장 위임
    STR_ID_PARTY_OUT                    = 9504, //강퇴
    STR_ID_COORDI_NOTIFY                = 9515, //스타일리쉬 쿠폰을 사용하여 일반 아이템을 코디용 아이템으로 변환 할 수 있습니다.\n변환한 코디용 아이템은 다시 일반 아이템으로 해제 할 수 없습니다.
    STR_ID_BUY_COORDI_NOTIFY            = 9516, //새로운 기간제 코디를 구입합니다. 보유중인 기간제 코디에 기간이 더해지지 않습니다.	
    STR_ID_NOT_COORDI_NOTIFY            = 9530, //스타일리쉬 쿠폰을 통해 코디로 변환된 장비입니다. 일반 장비로 해제시 능력치가 다시 적용 됩니다.
    STR_ID_AGIT_FACVORITE_ADD           = 9514, //즐겨찾기로 등록 하시겠습니까?
    STR_ID_EXIT_AGIT                    = 9527, //아지트에 접속 중입니다. \n나가시겠습니까?
    STR_ID_AGIT_UNLIMITED_PERIOD        = 9529, //영구

    STR_ID_DONATE                       = 9526, // 기부하기
    STR_ID_DATE                         = 9531, // 날짜
    STR_ID_SEND_GIFT                    = 9532, // 선물하기



    STR_ID_TYPE_NICKNAME                = 9534, // 닉네임 입력
    STR_ID_THE_GIFT_HAS_ARRIVED         = 9535, // 선물 도착
    STR_ID_NO_USER_TO_RECIEVE_IN_THIS_SERVER   = 9537, // 선물 받을 유저가 같은 서버에 없습니다.
    STR_ID_GET_REWARD_LOWER_GRADE_FIRST = 9538, // 아래 단계의 보상을 먼저 받아야 합니다.

    STR_ID_LUPUS_NAME                   = 9539, // 루퍼스
    STR_ID_NEW_LETTER_RECV              = 9548, //새로운 편지가 도착했습니다!
    
    STR_ID_GC_GM_QUEST52                = 9549, // 몽환의 결계

    STR_ID_YOUR_POINT_IS_MAXIMUM        = 9550, // 7단계 보상을 받기 전에는 더 이상 기부할 수 없습니다.
    STR_ID_RECV_ITEM_FROM_LETTER_ERR0   = 9551, //첨부 아이템을 받을 우편이 없습니다. 
    STR_ID_RECV_ITEM_FROM_LETTER_ERR1   = 9552, //첨부 아이템을 이미 받았습니다. 
    STR_ID_RECV_ITEM_FROM_LETTER_ERR2   = 9553, //GP 소유 한도 초과로 더 이상 GP를 받을 수 없습니다. 
    STR_ID_RECV_ITEM_FROM_LETTER_ERR3   = 9554, //아지트 아이템 인벤토리가 부족하여 첨부 아이템을 받을 수 없습니다. 
    STR_ID_RECV_ITEM_FROM_LETTER_ERR4   = 9555, //코디 아이템 인벤토리가 부족하여 첨부 아이템을 받을 수 없습니다. 
    STR_ID_RECV_ITEM_FROM_LETTER_ERR5   = 9556, //인벤토리가 부톡하여 첨부 아이템을 받을 수 없습니다. 
    STR_ID_RECV_ITEM_FROM_LETTER_ERR6   = 9557, //해당 우편은 첨부 아이템이 없습니다. 
    STR_ID_RECV_ITEM_FROM_LETTER_ERR7   = 9558, //우편 아이템 리스트 받아오기 DB처리 실패. 

    STR_ID_USE_TRAINING_OBJ_FAIL12      = 9559, //최고레벨에 도달했습니다. 더 이상 훈련할 수 없습니다.
    STR_ID_SYSTEM_LETTER_FROM           = 9560, //그랜드체이스

    STR_ID_GET_POST_LETTER_LIST_ERR     = 9561, //잘못된 페이지 정보를 요청하였습니다. 
    STR_ID_READ_LETTER_ERR              = 9562, //읽을 편지가 없습니다. 
    STR_ID_DELETE_LETTER_ERR0           = 9563, //삭제할 우편이 없습니다. 
    STR_ID_DELETE_LETTER_ERR1           = 9564, //우편 삭제 DB처리 실패. 

    STR_ID_USE_TRAINING_OBJ_FAIL13      = 9566, //격파할 쌓여있는 나무가 없습니다. 

    STR_ID_LUPUS_INTRODUCE              = 9576,
    
    STR_ID_RELAY_HERO_DUNGEON_LEVEL     = 9626, // @1단계	

    STR_ID_SEND_PRESEND_DESC            = 9629, //선물 받을 친구의 닉네임을 입력해 주세요. \n 선물은 3~4분안에 우편으로 발송 됩니다.
    STR_ID_WELCOME_BACK_NEW             = 9630, //친구에게 추천을 받아 추천상을 받아 보세요.
    STR_ID_PARTY_GP_50_PER              = 9632, // 파티원 GP +50%
    STR_ID_BUFF_GP_50_PER               = 9633, // GP +50%

    STR_ID_JUST_IN_TIME_MESSAGE1        = 9634, // 보상을 요청하였습니다. 몇 분이 소요될 수 있으니 가급적 서버를 이동하지 말고 기다려 주시기 바랍니다.
    STR_ID_JUST_IN_TIME_MESSAGE2        = 9635, // 보상을 요청하였습니다. 잠시만 기다려 주세요. 게임을 종료해도 보상은 자동으로 받게 됩니다. 
    
    STR_ID_LIMIT_DUNGEON_LEVEL          = 9638, // 목표 단계 : @1
    STR_ID_WRONG_PASSWORD_LIMIT_EXCEED  = 9639, // 비밀번호를 5회 잘못 입력하여 프로그램이 종료됩니다. 다시 시도해 주십시오.

    STR_ID_PRESENT_FAIL_ERROR           = 9641, //선물할 수 없는 아이템 입니다.

    STR_ID_HERO_REWARD_DESC             = 9640, // 마지막 단계를 완료 시마다 아이템을 획득할 수 있습니다.	
    STR_ID_HERO_REWARD_ERROR3           = 9642, // 보상아이템을 선택할 수 있는 기회가 없습니다.
    STR_ID_HERO_DUNGEON_INVITE          = 9643, // 릴레이 영웅던전에서는 초대를 할 수 없습니다.
    STR_ID_HERO_DUNGEON_DIFF_LEVEL      = 9644, // 현재 게임모드 클리어 조건 단계가 유저의 클리어 단계랑 다릅니다.
    
    STR_ID_GET_NEW_CHARACTER13          = 9653, //당신을 죽음으로 인도해 드리지요. 	
    
    STR_ID_TUTORIAL_RIN1_REAR_EVASION   = 9859,
    STR_ID_TUTORIAL_RIN1_FRONT_EVASION  = 9860,
    STR_ID_TUTORIAL_RIN1_X_DESC1        = 9861,
    STR_ID_TUTORIAL_RIN1_PROMOTION_DESC = 9862,
    STR_ID_RIN_NAME                     = 9863,
    STR_ID_TUTORIAL_RIN1_JOB_NAME       = 9864,
    STR_ID_TUTORIAL_RIN1_SHOW_X_SYSTEM  = 9865,
    STR_ID_TUTORIAL_RIN1_X_COMBO        = 9866,
    STR_ID_TUTORIAL_RIN1_X_JUMP_SKILL   = 9867,
    STR_ID_TUTORIAL_RIN1_X_DASH_SKILL   = 9868,
    STR_ID_TUTORIAL_RIN1_X_EVASION_SKILL= 9869,
    STR_ID_TUTORIAL_RIN1_X_SPECIAL_DESC1= 9870,
    STR_ID_TUTORIAL_RIN1_X_SPECIAL_DESC2= 9871,
    STR_ID_TUTORIAL_RIN1_X_SPECIAL_DESC3= 9872,
    STR_ID_TUTORIAL_RIN1_X_DESC2        = 9873,
    
          
    STR_ID_STAY_SERVER_TIMEOUT          = 9691, //서버가오랜시간 반응이 없습니다. 접속을 종료 합니다.

    STR_ID_COORDI_GACHA_SYSTEM_MESSAGE01   = 9645,
    STR_ID_COORDI_GACHA_SYSTEM_MESSAGE02   = 9646,
    STR_ID_COORDI_GACHA_SYSTEM_MESSAGE03   = 9647,
    STR_ID_COORDI_GACHA_SYSTEM_MESSAGE04   = 9648,

    STR_ID_PROMOTION_0                     = 9649,
    STR_ID_PROMOTION_1                     = 9650,
    STR_ID_PROMOTION_2                     = 9651,
    STR_ID_PROMOTION_3                     = 9652,
    STR_ID_PROMOTION_4                     = 9656,

    STR_ID_COORDI_GACHA_TOOLTIP01          = 9654,
    STR_ID_COORDI_GACHA_TOOLTIP02          = 9655,
    STR_ID_COORDI_GACHA_SYSTEM_MESSAGE05   = 9660,

    STR_ID_GACHA_GUIDE_DESC             = 9661,     // 신가차 설명1
    STR_ID_GACHA_GUIDE_DESC2            = 9662,     // 신가차 설명2


    STR_ID_ESCORT_MERRY_HP_75               = 9657,
    STR_ID_ESCORT_MERRY_HP_50               = 9658,
    STR_ID_ESCORT_MERRY_HP_25               = 9659,

    STR_ID_RELAY_HERO_DUNGEON_DESC          = 9679,
    STR_ID_RELAY_HEOR_REWARD1               = 9680,
    STR_ID_LEVEL_DOWN_NUMBER                = 9681, // (-@1)

    STR_ID_LEVEL_DOWN_OK                    = 9682, //  아이템 장착 레벨 다운에 성공하였습니다.
    STR_ID_LEVEL_DOWN_ERROR3                = 9683, //  필요한 재료 아이템이 부족합니다.
    STR_ID_LEVEL_DOWN_ERROR4                = 9684, //  장착 레벨을 낮출 수 없는 아이템입니다.
    STR_ID_LEVEL_DOWN_ERROR5                = 9685, //  더이상 장착 레벨을 낮출 수 없습니다.
    
    
    STR_ID_LEVEL_DOWN_DLG_DESC              = 9686, // 아이템의 필수레벨이 1씩 감소하며, 모든 능력치는 그대로 유지됩니다
    STR_ID_LEVEL_DOWN_DLG_HERO_COIN         = 9687, // 영웅 주화 남은 개수
    STR_ID_LEVEL_DOWN_DLG_REMAIN_DESC       = 9688, // 레벨 다운 남은 회수

    STR_ID_LEVEL_DOWN_DLG_HERO_COIN_NUM     = 9689, // @1개
    STR_ID_LEVEL_DOWN_DLG_REMAIN            = 9690, // @1회
    
    STR_ID_SET_NOT_PETSKILL					= 9692, // 펫스킬구매스트링 - 해당 스킬은 판매하지 않습니다.
    STR_ID_NOT_FIND_PETSKILL				= 9693, // 펫스킬구매스트링 - 착용가능한 스킬이 없습니다. 스킬을 구입하세요		

    STR_ID_GS_FOREST_OF_ELF_REVERS          = 9807,
    STR_ID_GS_VALLEY_OF_OATH_REVERS         = 9808,

   STR_ID_ECLIPSE_PLOT_EVENT_1         = 9809, // 남은 시간
    STR_ID_ECLIPSE_PLOT_EVENT_2         = 9810, // 보유 아이템
    STR_ID_ECLIPSE_PLOT_EVENT_3         = 9811, // 개기일식 이벤트가 발동하면, 보상을 획득할 수 있습니다.
    STR_ID_ECLIPSE_PLOT_EVENT_4         = 9812, // 던전에서 "개기일식 암흑사제"를 물리치고 "정화된 암흑사제"를 모으면 보상을 획득할 수 있습니다.
    STR_ID_ECLIPSE_PLOT_EVENT_5         = 9813, // 개기일식의 음모를 막고, "Jr.암흑사제 펫"을 얻으세요!\n("개기일식 상자"를 획득할 때 마다, 최종 보상 게이지가 올라갑니다.)

    STR_ID_EVENT_BOOK_DESC              = 9827,
    STR_ID_ECLIPSE_TIME_TITLE           = 9857,
    STR_ID_NOT_EVENT_TIME               = 9858, //보상 받을 수 있는 시간이 아닙니다.
    
    STR_ID_NOBREAK_LEVEL_CHECK              = 9825, // 현재 단계에서 사용할 수 없는 강화 보호주문서 입니다.	

    STR_ID_HOT_TIME_TITLE                   = 9831, // 이벤트 시간
    STR_ID_HOT_TIME_SEARCH_DESC             = 9832, // 해당 시간에 환영하기를 클릭하면 보상 아이템 획득과 함께 린을 많은 친구들에게 알릴 수 있습니다.
    
    STR_ID_ENCHANT_DISPLAY					= 9834, // +@1 강화	

    STR_ID_MONSTER_CARD_MIX_ERROR1			= 9835, // 해당 몬스터카드가 인벤토리에 존재하지 않습니다.
    STR_ID_MONSTER_CARD_MIX_ERROR2			= 9836, // 몬스터카드 아이템이 올바르지 않습니다.

    STR_ID_MONSTER_MENU_TOOL_TIP			= 9838,	// 몬스터카드 조합

    STR_ID_MONSTER_CARD_MIX_MAIN_DESC		= 9851,	// 두개 이상의 카드를 조합하여 새로운 카드를 만들수 있습니다.\n고등급 카드가 많을 수록 고등급 카드를 획득 할 확률이 높아 집니다.
    STR_ID_MONSTER_CARD_MIX_SUB_DESC		= 9852,	// 보유 중인 카드를 선택하여 주세요.
    STR_ID_MONSTER_CARD_MIX_PAGE_NUM		= 9853,	// @1 / @2
    STR_ID_DEPOT_EXTEND_FULL_SLOT		= 9874,//	보관소 공간을 더 이상 확장할 수 없습니다.
    STR_ID_DEPOT_EXTEND_SLOT_NO_ITEM    = 9875,//	보관소의 공간을 확장할 수 있는 아이템이 없습니다. 지금 구매 하시겠습니까?
    STR_ID_DEPOT_SUCCESS_EXTEND_SLOT	= 9876,//	보관소 슬롯이 @1 칸 만큼 추가되었습니다.
    STR_ID_DEPOT_EXTEND_FULL_SAFE       = 9877,//	보관소의 금고를 더 이상 확장할 수 없습니다.
    STR_ID_DEPOT_EXTEND_SAFE_NO_ITEM	= 9878,//	보관소의 금고를 추가할 수 있는 아이템이 없습니다. 지금 구매 하시겠습니까?
    STR_ID_DEPOT_SUCCESS_EXTEND_SAFE    = 9879,//	보관소의 금고가 추가되었습니다.
    STR_ID_DEPOT_CAN_EXTEND				= 9880,//      금고 또는 슬롯 확장이 가능합니다.

    STR_ID_NECK_ENCHANT_MATERIAL_TYPE1		= 9883, // 젬	
    STR_ID_NECK_ENCHANT_MATERIAL_TYPE2		= 9884, // 초콜릿
    STR_ID_NECK_ENCHANT_MATERIAL_TYPE3		= 9885, // 고급젬

    STR_ID_NECK_ENCAHNT_COST_MATERIAL_TITLE = 9886, // 소요 개수
    STR_ID_NECK_ENCAHNT_HAVE_MATERIAL_TITLE = 9887, // 보유 개수
    STR_ID_DEPOT_EXTEND_CONSUME_ITEM	= 9888,//	[ @1 ] 아이템이 @2 개 소모됩니다. 확장하시겠습니까?	

    STR_ID_TRANSFER_MISSION_SCROLL_GUIDE_0 = 9889, // 미션 조건을 클리어 하면 전직을 완료할 수 있습니다.
    STR_ID_TRANSFER_MISSION_SCROLL_GUIDE_1 = 9890, // 젬 1개로 즉시 전직을 완료할 수 있습니다.

    STR_ID_ECLIPSE_PLOT_REWARD_DESC1        = 9891,
    STR_ID_ECLIPSE_PLOT_REWARD_DESC2        = 9892,	

    STR_ID_DEPOT_ERR_DEPOT_00 = 9904,					// 아이템이 인벤토리에 없음. )
    STR_ID_DEPOT_ERR_DEPOT_01 = 9905,					// 창고 아이템을 창고로 옮기려고 시도함. )
    STR_ID_DEPOT_ERR_DEPOT_02 = 9906,					// 창고 슬롯이 비어있지 않음. )
    STR_ID_DEPOT_ERR_DEPOT_03 = 9907,					// 인벤토리에서 창고로 옮기기 실패 )
    STR_ID_DEPOT_ERR_DEPOT_04 = 9908,					// 영구 아닌 기간제 아이템을 창고로 보관 시도함. )
    STR_ID_DEPOT_ERR_DEPOT_05 = 9909,					// 창고에 없는 아이템. )
    STR_ID_DEPOT_ERR_DEPOT_06 = 9910,					// 창고에서 인벤토리로 옮기기 실패 )
    STR_ID_DEPOT_ERR_DEPOT_07 = 9911,					// 창고 아이템 타입이 아닌데 창고에서 삭제 시도 )
    STR_ID_DEPOT_ERR_DEPOT_08 = 9912,					// 창고에서 슬롯 위치 이동 실패. )
    STR_ID_DEPOT_ERR_DEPOT_09 = 9913,					// 창고 정보 반복 요청 옴. )
    STR_ID_DEPOT_ERR_DEPOT_10 = 9914,					// 확장할 창고 정보가 없음. )
    STR_ID_DEPOT_ERR_DEPOT_11 = 9915,					// 창고 최대 크기를 초과하여 확장할 수 없음. )
    STR_ID_DEPOT_ERR_DEPOT_12 = 9916,					// 창고 확장 아이템 DB 수량 동기화 실패. )
    STR_ID_DEPOT_ERR_DEPOT_13 = 9917,					// 창고 정보를 아직 DB에서 불러오지 않았음 )
    STR_ID_TRANSFER_MISSION_SCROLL_GUIDE_2 = 9918, // 새로운 경지에 도전할 자격을 갖추었군. \n지금 바로 도전해보길 바라네.
    STR_ID_RIN_INTRODUCE = 9919,

    STR_ID_DEPOT_NPC = 9920,					// 9920	보관 하고 싶은 아이템을 마우스로 끌어서 보관소로 옮겨주세요~ 보관은 공짜랍니다!!
    STR_ID_DEPOT_NPC_1 = 9921, //보관소의 공간을 늘이거나 금고를 추가하시려면 구입을 하셔야 해요~

    STR_ID_WEEKLY_PERFECT_ATTENDANCE_DESC   = 9924, // @1주 개근을 축하합니다!\n멋진 선물들이 준비되어 있습니다.\n아래의 아이템 중 마음에 드는것을 선택해 주세요^-^
    STR_ID_DEPOT_TOOLTIP = 9931,
    STR_ID_TUTORIAL_RIN1_X_SPECIAL_DESC1_2 = 9932,
    STR_ID_TUTORIAL_RIN1_X_SPECIAL_DESC1_3 = 9933,
    STR_ID_USE_EXP_POTION_OK                = 9947, // 	
    STR_ID_USE_LIMITE_LEVEL                 = 9948, // 사용 가능한 레벨이 아닙니다.	

    
    STR_ID_USE_CHAPION_STAGE_DESC = 10144, //	-보스게이트를 파괴 시 대륙 보스가 등장합니다. 공략 시 최고의 보상 아이템들을 획득 할 수 있습니다.-
    STR_ID_USE_CHAPION_STAGE_ESC = 10145, //	#cFFFF00(즉시종료 : ESC)#cX
    STR_ID_USE_CHAPION_INFO_TOOLTIP_1 = 10146, //	보스게이트를 파괴하면 강력한 대륙 보스를 만날 수 있습니다.\n챔피언 모드 입장권 획득 방법 : #cFFFF00매일 출석시#cX(우측상단 출석 메뉴)
    STR_ID_USE_CHAPION_INFO_TOOLTIP_2 = 10147, //	클릭 시 #cFFFF00더 높은 난이도와 보상#cX을 가진 챔피언 난이도 던전으로 입장이 가능 합니다.\n챔피언 모드 입장 방법 : 일반 난이도 클리어, 챔피언 모드 입장권 소지
    STR_ID_DEPOT_ADD_SLOT                   = 10060, // [ @1 ] 아이템이 @2 개 소모됩니다. #cff0000[ @3 ]#cX 슬롯을 확장하시겠습니까?
    STR_ID_DEPOT_ADD_SAFE                   = 10061, // [ @1 ] 아이템이 @2 개 소모됩니다. #cff0000[ @3 ]#cX 금고를 확장하시겠습니까?
    STR_ID_DEPOT_ERR_DEPOT_14               = 10163, //코디 인벤토리가 가득 찼습니다.
    STR_ID_NOT_USE_RESOLUTION               = 10195, //	이 해상도는 선택 할 수 없습니다.

    STR_ID_ENCHANT_SCROLL_ERROR01           = 10058,
    STR_ID_LIMITED_BUY_ONCE_PER_ACCOUNT     = 10148,   // 계정당 한번 구매 가능 This item may only be purchased once per account.
    STR_ID_LIMITED_BUY_3TIME_PER_DAY        = 10149,   // 하루에 3번 구매 가능 This item is restricted to 3 purchases a day.

    STR_ID_OPEN_CHANGE_SKILL_SLOT_QEUSTION  = 10164,
    STR_ID_OPENED_CHANGE_SKILL_SLOT_MESSAGE = 10165,
    STR_ID_OPEN_CHANGE_SKILL_SLOT_ERROR_03  = 10166,
    STR_ID_OPEN_CHANGE_SKILL_SLOT_ERROR_04  = 10167,
    STR_ID_OPEN_CHANGE_SKILL_SLOT_SUCCESS   = 10168,
    
    STR_ID_CONTRIBUTE_REWARD_DESC_1         = 10174,  //	Housing Item ( Apple )
    STR_ID_CONTRIBUTE_REWARD_DESC_2         = 10175,  //	Housing Item ( Sweet Potato )
    STR_ID_CONTRIBUTE_REWARD_DESC_3         = 10176,  //	Housing Item ( Fluffy Bread )
    STR_ID_CONTRIBUTE_REWARD_DESC_4         = 10177,  //	Housing Item ( Bottled Milk )
    STR_ID_CONTRIBUTE_REWARD_DESC_5         = 10178,  //	Housing Item ( Hamburger )

    STR_ID_GAWIBAWIBO_ERR_9 				= 10173,	//재도전 포인트가 부족합니다.

    STR_ID_ASIN_NAME 				        = 10179,	//16th 캐릭터 "아신"

    STR_ID_JIT_ERROR                        = 10180,    //JIT 이벤트 대상 정보 오류 
    STR_ID_JIT_ONLY_PCBANG                  = 10181,    //PC방 유저 대상 이벤트 입니다. 
    STR_ID_JIT_ONLY_NORMAL                  = 10182,    //일반 유저 대상 이벤트 입니다. 

    STR_ID_ADDTIONGUIDE1					= 10183,
    STR_ID_ADDTIONGUIDE2					= 10184,
    STR_ID_ADDTIONGUIDE3					= 10185,
    STR_ID_ADDTIONGUIDE4					= 10186,
    
    STR_ID_GET_WATER_BOMB					= 10187, // 물폭탄 @1개를 받았습니다.
    STR_ID_NOT_ACQUIRE_WATER_BOMB			= 10188, // 오늘은 물폭탄을 더 이상 받을 수 없습니다.
    
    STR_ID_SONGKRAN_REWARD_DESC				= 10189, // 점수 높으면 또 준다 많이 해!
    STR_ID_SONGKRAN_REWARD_POINT			= 10190, // @1

    STR_ID_SONGKRAN_POINT_TOOLTIP			= 10191, // @1 / @2

    STR_ID_SONGKRAN_EVENT_ERROR01			= 10192, // 물폭탄 아이템이 부족합니다.
    STR_ID_SONGKRAN_EVENT_ERROR02			= 10193, // 보상을 받고나서 계속 던질 수 있습니다.	
    
    STR_ID_CHRISTMAS_ERR_GRADE_REWARD       = 10206,    // 획득할 수 없는 등급의 보상을 선택하셨습니다.
    STR_ID_ADDTIONGUIDE5					= 10220,
    STR_ID_SONGKRAN_EVENT_ERROR03			= 10206, // 획득할 수 없는 등급의 보상을 선택하셨습니다.
    STR_ID_SONGKRAN_EVENT_ERROR04			= 10208, // 물폭탄 던지기 이벤트 기간이 아닙니다.
    STR_ID_SONGKRAN_EVENT_ERROR05			= 10209, // 물폭탄 던지기 정보가 로드 되지 않았습니다.
    STR_ID_SONGKRAN_EVENT_ERROR06			= 10210, // 물폭탄 아이템이 인벤토리에 없습니다.
    STR_ID_SONGKRAN_CHANGE_ITEM				= 10207, // 물폭탄이 성공적으로 교환되었습니다.

    STR_ID_MONSTER_ID_MOTION_ERROR          = 10233,	//@1번 몬스터 이상\nMonstertemplate, AI파일, 몬스터 모션 체크요망

    STR_ID_COUPLE_ITEM_EXPIRED              = 10255, // 커플 아이템 기간이 만료되어 삭제됩니다.    


    STR_ID_ASIN_HIDDEN_SKILL_TITLE          = 10261,

    STR_ID_DUNGEON_LEVEL_1					= 10262,
    STR_ID_DUNGEON_LEVEL_2					= 10263,
    STR_ID_DUNGEON_LEVEL_3					= 10264,
    STR_ID_DUNGEON_LEVEL_HELL				= 10265,

    STR_ID_HERO_DUNGEON_CLEAR_COUNT			= 10266,
    STR_ID_HERO_DUNGEON_TIME				= 10267,		
    
    STR_ID_GIFT_ITEM_FAIL                   = 10280,

    STR_ID_ITEM_CLEARDEBUFF                 = 10282,
    STR_ID_ITEM_ATKUP                       = 10283,
    STR_ID_ITEM_MOVESPEEDUP                 = 10284,
    STR_ID_ITEM_TURRET                      = 10322,

    STR_ID_ASIN1_PROMOTION_DESC             = 10289,

    STR_ID_GET_NEW_CHARACTER15              = 10294,
    
    STR_ID_ADDTIONGUIDE6		            = 10361,
    STR_ID_SHANGHAI_INVEN_NOTY              = 10363,    // 레벨 달성 보상이 지급되었습니다. 인벤토리를 확인해주세요~!
    STR_ID_SHANGHAI_POST_NOTY               = 10364,    // 레벨 달성 보상이 지급되었습니다. 우편함을 확인해주세요~!
    STR_ID_CONFIRM_BTN_ERROR                = 10365, // 해당 캐릭터가 아니거나, 레벨이 맞지 않습니다.
    
    STR_ID_PVP_BALANCE_TOOLTIP              = 10367, // 세르딘 여왕배 기사단 대회(대전 능력치가 평준화 됩니다.)

    STR_ID_ELESIS_CLASS_NIGHT	            = 10368,
    STR_ID_ELESIS_CLASS_SPEARMAN            = 10369,
    STR_ID_ELESIS_CLASS_SWORDMASTER         = 10370,
    STR_ID_ELESIS_CLASS_SAVIOR	            = 10371,

    STR_ID_AWAKENING_SKILL_CLOSE            = 10373,
    
    STR_ID_ACCEPT_GUILD_MAX_NUM             = 10374, // 길드 가입 최대 인원수가 초과 하였습니다.
    STR_ID_ITEM_BUFF_JUMPUP                 = 10375,
    STR_ID_ITEM_MONSPEED                    = 10376,
    STR_ID_ITEM_DAMAGE_ICE                  = 10377,

    STR_ID_JOPNAME_0_0         = 10368,
    STR_ID_JOPNAME_0_1         = 10369,
    STR_ID_JOPNAME_0_2         = 10370,
    STR_ID_JOPNAME_0_3         = 10371,
    STR_ID_LEVER_RESET_ITEM_USE             = 10402, // @1 레벨을 초기화 하시겠습니까?
    STR_ID_SKILL_TREE_CHANGE_MESSAGE        = 10404,
    STR_ID_LEVER_RESET_ITEM_USE_OK          = 10405, // 현재 캐릭터의 레벨이 초기화 되었습니다.
    STR_ID_LEVER_RESET_REWARD_OK            = 10406, // 축하합니다~! 영광의 도전 목표를 달성한 진정한 영웅입니다~!
    
    STR_ID_LEVER_RESET_ITEM_USE_ERROR01     = 10408, // 레벨 초기화 아이템이 존재하지 않습니다.
    STR_ID_LEVER_RESET_ITEM_USE_ERROR02     = 10409, // 방에서는 레벨 초기화 아이템을 사용할 수 없습니다.
    


    STR_ID_SKILL_DESC_BOX_LEVEL             = 10410,
    STR_ID_SKILL_DESC_BOX_COOLTIME          = 10411,
    STR_ID_SKILL_DESC_BOX_SLOT_SKILL        = 10412,
    STR_ID_SKILL_DESC_BOX_COMMAND_SKILL     = 10413,

    STR_ID_JOPNAME_1_0         = 10414,
    STR_ID_JOPNAME_1_1         = 10415,
    STR_ID_JOPNAME_1_2         = 10416,
    STR_ID_JOPNAME_1_3         = 10417,

    STR_ID_JOPNAME_2_0         = 10418,
    STR_ID_JOPNAME_2_1         = 10419,
    STR_ID_JOPNAME_2_2         = 10420,
    STR_ID_JOPNAME_2_3         = 10421,

    STR_ID_JOPNAME_3_0         = 10422,
    STR_ID_JOPNAME_3_1         = 10423,
    STR_ID_JOPNAME_3_2         = 10424,
    STR_ID_JOPNAME_3_3         = 10425,

    STR_ID_JOPNAME_4_0         = 10426,
    STR_ID_JOPNAME_4_1         = 10427,
    STR_ID_JOPNAME_4_2         = 10428,
    STR_ID_JOPNAME_4_3         = 10429,

    STR_ID_JOPNAME_5_0         = 10430,
    STR_ID_JOPNAME_5_1         = 10431,
    STR_ID_JOPNAME_5_2         = 10432,
    STR_ID_JOPNAME_5_3         = 10433,

    STR_ID_JOPNAME_6_0         = 10434,
    STR_ID_JOPNAME_6_1         = 10435,
    STR_ID_JOPNAME_6_2         = 10436,
    STR_ID_JOPNAME_6_3         = 10437,

    STR_ID_JOPNAME_7_0         = 10438,
    STR_ID_JOPNAME_7_1         = 10439,
    STR_ID_JOPNAME_7_2         = 10440,
    STR_ID_JOPNAME_7_3         = 10441,

    STR_ID_JOPNAME_8_0         = 10442,
    STR_ID_JOPNAME_8_1         = 10443,
    STR_ID_JOPNAME_8_2         = 10444,
    STR_ID_JOPNAME_8_3         = 10445,

    STR_ID_JOPNAME_9_0         = 10446,
    STR_ID_JOPNAME_9_1         = 10447,
    STR_ID_JOPNAME_9_2         = 10448,
    STR_ID_JOPNAME_9_3         = 10449,

    STR_ID_JOPNAME_10_0         = 10450,
    STR_ID_JOPNAME_10_1         = 10451,
    STR_ID_JOPNAME_10_2         = 10452,

    STR_ID_JOPNAME_11_0         = 10453,
    STR_ID_JOPNAME_11_1         = 10454,

    STR_ID_JOPNAME_12_0         = 10455,
    STR_ID_JOPNAME_12_1         = 10456,

    STR_ID_JOPNAME_13_0         = 10457,
    STR_ID_JOPNAME_13_1         = 10458,
    STR_ID_JOPNAME_13_2         = 10459,
    STR_ID_JOPNAME_13_3         = 10460,

    STR_ID_JOPNAME_14_0         = 10461,

    STR_ID_JOPNAME_15_0         = 10462,

    STR_ID_GET_NEW_CHARACTER14              = 10464,  // 더이상 소중한 사람들이 사라지는 건 싫어.
    STR_ID_DIFFICULT_HIGH         = 10465,
    STR_ID_DIFFICULT_NORMAL         = 10466,
    STR_ID_DIFFICULT_LOW         = 10467,

    STR_ID_GC_ADVENTURE_BALOON_DESC          = 10479, //모든 대륙 보상을 획득하고 최종 보상에 도전 해보세요
    STR_ID_GC_ADVENTURE_ASK_FINAL_REWARD     = 10480, //최종 보상 획득시, 지금까지 획득한 전리품 정보가 초기화 되며 다시 도전해서 전리품 획득이 가능합니다. 획득 하시겠습니까?
    STR_ID_GC_ADVENTURE_DATA_ERR             = 10481, //전리품 관련 정보를 제대로 가져오지 못했습니다.
    STR_ID_GC_ADVENTURE_REWARD_INFO_ERR      = 10482, //이벤트 보상 정보가 없습니다.
    STR_ID_GC_ADVENTURE_ALREADY_OBTAIN       = 10483, //요청한 대륙의 보상 아이템을 이미 받았습니다.
    STR_ID_GC_ADVENTURE_CONTINENT_REWARD_ERR = 10484, //모험 이벤트 대륙 보상 정보가 없습니다.
    STR_ID_GC_ADVENTURE_REWARD_CONDITION_ERR = 10485, //해당 대륙의 전리품을 다 획득하지 않았습니다.
    STR_ID_GC_ADVENTURE_FINAL_REWARD_ERR     = 10486, //최종 보상을 받을 수 없습니다. (각 대륙의 보상 획득 조건을 먼저 수행하세요)
    
    STR_ID_LIME_FINAL_AURA                  = 10489,

    STR_ID_CHAR0_WEAPON			= 10490,
    STR_ID_CHAR0_DESC			= 10491,
    STR_ID_CHAR1_WEAPON			= 10492,
    STR_ID_CHAR1_DESC			= 10493,
    STR_ID_CHAR2_WEAPON			= 10494,
    STR_ID_CHAR2_DESC			= 10495,
    STR_ID_CHAR3_WEAPON			= 10496,
    STR_ID_CHAR3_DESC			= 10497,
    STR_ID_CHAR4_WEAPON			= 10498,
    STR_ID_CHAR4_DESC			= 10499,
    STR_ID_CHAR5_WEAPON			= 10500,
    STR_ID_CHAR5_DESC			= 10501,
    STR_ID_CHAR6_WEAPON			= 10502,
    STR_ID_CHAR6_DESC			= 10503,
    STR_ID_CHAR7_WEAPON			= 10504,
    STR_ID_CHAR7_DESC			= 10505,
    STR_ID_CHAR8_WEAPON			= 10506,
    STR_ID_CHAR8_DESC			= 10507,
    STR_ID_CHAR9_WEAPON			= 10508,
    STR_ID_CHAR9_DESC			= 10509,
    STR_ID_CHAR10_WEAPON		= 10510,
    STR_ID_CHAR10_DESC			= 10511,
    STR_ID_CHAR11_WEAPON		= 10512,
    STR_ID_CHAR11_DESC			= 10513,
    STR_ID_CHAR12_WEAPON		= 10514,
    STR_ID_CHAR12_DESC			= 10515,
    STR_ID_CHAR13_WEAPON		= 10516,
    STR_ID_CHAR13_DESC			= 10517,
    STR_ID_CHAR14_WEAPON		= 10518,
    STR_ID_CHAR14_DESC			= 10519,
    STR_ID_CHAR15_WEAPON		= 10520,
    STR_ID_CHAR15_DESC			= 10521,
    STR_ID_CHAR16_WEAPON		= 10522,
    STR_ID_CHAR16_DESC			= 10523,

    STR_ID_FORCE_SHUTDOWN_MSG_60MIN         = 10533, // 고객님께서는 청소년보호법 제23조의 3제①에 근거하여 오전0시부터 오전6시까지 게임 이용을 하실 수 없습니다. 게임 이용에 참고 부탁드립니다. 강제종료 1시간 전입니다.
    STR_ID_FORCE_SHUTDOWN_MSG_30MIN         = 10534, // 고객님께서는 청소년보호법 제23조의 제3①에 근거하여 오전0시부터 오전6시까지 게임 이용을 하실 수 없습니다. 게임 이용에 참고 부탁드립니다. 강제종료 30분 전입니다.
    STR_ID_FORCE_SHUTDOWN_MSG_NOW           = 10535, // 고객님께서는 청소년보호법 제23조의 3제①에 근거하여 오전0시부터 오전6시까지 게임 이용을 하실 수 없습니다.\n6시 이후부터 게임이 이용이 가능하오니 이점 참고해 주시기 바랍니다. 관련하여 궁금하신 사항은 넷마블 고객센터로 문의해 주시기 바랍니다.
    STR_ID_SELECTIVE_SHUTDOWN_MSG_60MIN     = 10536, // 고객님께서는 자녀사랑에 설정된 게임이용시간에 따라 게임 이용이 제한되며, 게임이용이 허용된 시간에 한해 게임을 이용하실 수 있습니다. 강제종요 1시간 전이니 게임이용에 참고 부탁드립니다. 
    STR_ID_SELECTIVE_SHUTDOWN_MSG_30MIN     = 10537, // 고객님께서는 자녀사랑에 설정된 게임이용시간에 따라 게임 이용이 제한되며, 게임이용이 허용된 시간에 한해 게임을 이용하실 수 있습니다. 강제종료 30분 전이니 게임이용에 참고 부탁드립니다. 
    STR_ID_SELECTIVE_SHUTDOWN_MSG_NOW       = 10538, // 고객님께서는 자녀사랑에 설정된 게임이용시간에 따라 게임 이용이 제한되며, 게임이용이 허용된 시간에 한해 게임을 이용하실 수 있으니 참고 부탁드립니다. 관련하여 궁금하신 사항은 넷마블 고객센터>자녀사랑 페이지를 확인하시거나, 고객센터로 문의해 주시기 바랍니다.
  
    STR_ID_GET_NEW_CHARACTER16              = 10552, //얼른 달려가야겠어요! 역시... 정의는 승리하는 법이에요!
    
    STR_ID_SKILLTREE_WAIT_MSG               = 10558, // 스킬트리를 준비 중인 캐릭터 입니다.
    STR_ID_LIME_NAME                        = 10561,

    STR_ID_LIME_HIDDEN_SKILL_TITLE          = 10569,
    
    STR_ID_CHAR_SEAL_MSG_1                  = 10570,
    STR_ID_CHAR_SEAL_MSG_2                  = 10571,

    STR_ID_LIME_INTRODUCE                   = 10585, //	-사용무기: 블레싱해머 / 거대한 해머를 사용하는 캐릭터로 스킬을 통한 성장의 재미와 공격, 방어, 보조 오오라를 이용하여 자신 및 파티원들에게 도움을 줄 수 있는 캐릭터이다.

    STR_ID_OLYMPIC_EVENT                    = 10586, // 올림픽 이벤트
    STR_ID_OLYMPIC_EVENT_DESC               = 10587, // 올림픽 이벤트 설명
    STR_ID_OLYMPIC_MENU_TOOL_TIP            = 10588, // 올림픽 이벤트

    STR_ID_CHAT_OPTION_ALL       = 10589,//	전체
    STR_ID_CHAT_OPTION_GUILD       = 10590,//	길드
    STR_ID_CHAT_OPTION_WHISPER       = 10591,//	귓속말
    STR_ID_CHAT_OPTION_NORMAL       = 10592,//	일반

    STR_ID_GC_GS_CASTLE_OF_RULE              = 10593, //지배의 성 
        
    STR_ID_OLYMPIC_GAME_DATE                = 10594, //	@1월 @1일
    STR_ID_OLYMPIC_GAME_BALLOON	            = 10595, //	풍선 터트리기
    STR_ID_OLYMPIC_GAME_TREEDROP            = 10596, //	통나무 깨기
    STR_ID_OLYMPIC_GAME_WATERBOMB           = 10597, //	물풍선 던지기
    STR_ID_OLYMPIC_GAME_GAWIBAWIBO          = 10598, //	가위 바위 보
    STR_ID_OLYMPIC_GAME_DICEPLAY            = 10599, //	주사위 던지기
    STR_ID_OLYMPIC_GAME_BOSSGATE            = 10600, //	보스게이트 클리어 
    STR_ID_OLYMPIC_GAME_SOLO                = 10601, //	(개인전)
    STR_ID_OLYMPIC_GAME_TEAM                = 10602, //	(단체전)
    STR_ID_USERAGREEMENT_11                 = 10603,
    STR_ID_USERAGREEMENT_12                 = 10604,
    STR_ID_USERAGREEMENT_13                 = 10605,
    STR_ID_USERAGREEMENT_14                 = 10606,
    STR_ID_USERAGREEMENT_15                 = 10607,
    STR_ID_USERAGREEMENT_16                 = 10608,
    STR_ID_USERAGREEMENT_17                 = 10609,
    STR_ID_OLYMPIC_GAME_WIN                 = 10610, // #cD4D4D6우승#cX

    STR_ID_OLYMPIC_GAME_ALRAM               = 10612, //	현재 종목 플레이 시간이 종료 되었습니다. 다음 도전으로 바로 참여해 주세요.
    STR_ID_OLYMPIC_GAME_MAX_POINT_PER_GAME  = 10613, //	한 판당 획득할 수 있는 최대 점수는 10000점 입니다.
    STR_ID_OLYMPIC_GAME_UPDATE_DESC         = 10614, //	매일 5시 각 국가의 승점이 갱신 됩니다.
    STR_ID_OLYMPIC_GAME_RANKING             = 10615, //	순위
    STR_ID_OLYMPIC_GAME_NATION              = 10616, //	국가
    STR_ID_OLYMPIC_GAME_TOTAL_SCORE         = 10617, //	총점수
    STR_ID_OLYMPIC_GAME_QUIT_NOTICE         = 10619, //	지금 종료 하시면 현재까지의 점수만 기록 됩니다.
    STR_ID_OLYMPIC_GAME_BOSS_GATE           = 10620, //	보스게이트 종목은 던전 플레이를 통해 출현하는 보스를 사냥하고 금메달을 획득하는 방식 입니다. 
    
    STR_ID_LOTTERY_NAME			            = 10772, //	가챠복권
    STR_ID_LOTTERY_GUIDE			        = 10773, 
    STR_ID_LOTTERY_EXPLAIN0			        = 10774, 
    STR_ID_LOTTERY_EXPLAIN1			        = 10775, 
    STR_ID_LOTTERY_EXPLAIN2			        = 10776, 

    STR_ID_OLYMPIC_GAME_GAWIBAWIBO_WIN      = 10637, // 원소대결에서 승리하였습니다. 
    STR_ID_OLYMPIC_GAME_GAWIBAWIBO_RETRY    = 10638, // 확인:다시시작
    STR_ID_OLYMPIC_GAME_GAWIBAWIBO_NO_POINT = 10639, // 도전 포인트가 부족합니다. 

    STR_ID_CHAR_CASH_DESC                   = 10640, // 캐쉬 캐릭터 획득 방법\n\n즉시 획득 방법\n아래 [구입] 버튼을 누르시면 캐쉬 결제 후, 즉시 획득이 가능합니다.\n\n미션 획득 방법\n[상점]에서 GP로 [캐릭터 획득 미션]을 구입 후, 미션을 완료하면 획득이 가능합니다.
    STR_ID_CHAR_EVENT_DESC                  = 10641, // 이벤트 캐릭터 획득 방법\n\n이벤트 캐릭터는 정해진 기간동안 특정 조건을 만족하면 획득할 수 있습니다.

    STR_ID_JOPNAME_16_0						= 10669, //홀리나이트
    STR_ID_NO_USE_ITEM					    = 10670,//보유하지 않은 캐릭터의 아이템

    STR_ID_NOT_JOIN_GUILD                   = 10764 , //길드 없음

    STR_ID_PUSH_Z_FOR_SKIP                  = 10765 , //Z 키를 입력하면 결과창을 빠르게 넘길 수 있습니다.

    STR_UNKWON_ERROR_CODE                   = 10766, //( ErrorCode : @1 - @2 )

    STR_ID_SKILL_TRAIN_ERROR_NOITEM         = 10767, // 스킬 오픈 아이템이 없습니다.
    STR_ID_SKILL_TRAIN_IMPOSSIBLE_SKILL     = 10768, // 스킬 오픈 아이템으로 획득 가능한 스킬이 아닙니다.
    STR_ID_SKILL_TRAIN_CONDITION_ERROR      = 10769, // 스킬을 배울수 있는 조건을 만족하지 못했습니다.
    STR_ID_9TH_EVENT_COUNT                  = 10781, //	/ 100 개
    STR_ID_9TH_EVENT_DESC0                  = 10782, //	50개 이상 응모권을 모아야 응모 자격이 생깁니다.
    STR_ID_9TH_EVENT_LINK0                  = 10783, //	응모권 순위는
    STR_ID_9TH_EVENT_LINK1                  = 10784, //	에서 확인이 가능합니다. 
	STR_FATIGUE_SYSTEM_MESSAGE				= 10786,
	STR_FATIGUE_SYSTEM_MESSAGE_1			= 10787,
	STR_FATIGUE_SYSTEM_MESSAGE_2			= 10788,
	STR_FATIGUE_SYSTEM_MESSAGE_3			= 10789,
    STR_ID_BASE_COMBO_ZZ                    = 10791, //	기본 콤보      : Z Z
    STR_ID_9TH_EVENT_DESC1                  = 10792, //	응모권 순위가 5등 이상 되어야 100% 당첨 됩니다.
    STR_ID_9TH_EVENT_DESC2                  = 10793, //	응모권이 많을 수록 당첨 확률이 높이집니다.

    STR_ID_LEVEL_ITEM_ALRAM0                = 10803, //	#cFEC601Lv.@1 달성을 완료하였습니다.#cX
    STR_ID_LEVEL_ITEM_ALRAM1                = 10804, //	위의 캐릭터들에게 아이템이 지급되었습니다.\바로 확인 해주세요!

    STR_ID_JOPNAME_10_3                     = 10887,
    
    STR_ID_ITEM_COMBINATION_BALOON_STR1     = 10888, 
    STR_ID_ITEM_COMBINATION_BALOON_STR2     = 10889, 
    STR_ID_ITEM_COMBINATION_BALOON_STR3     = 10890, 
    STR_ID_ITEM_COMBINATION_REWARD_DESC     = 10891, //위의 보상 중 한가지를 랜덤하게 받을 수 있습니다! 
    STR_ID_ITEM_COMBINATION_ERROR1          = 10893, //재료 갯수가 서버에서 설정된 값과 다릅니다.
    STR_ID_ITEM_COMBINATION_ERROR2          = 10894, //서버에서 설정된 재료 아이템이 아닙니다.
    
    STR_ID_GP_RANDOM_ATTRIBUTE_ITEM         = 10910, // GP아이템 속성랜덤 주문서 @1장(보유:@2) 
    STR_ID_GP_INIT_ATTRIBUTE_ITEM           = 10911, // GP아이템 속성초기화 주문서 @1장(보유:@2) 
    STR_ID_GP_INIT_ATTRBUTE_DESC            = 10912, // 선택창에서 @1개의 속성을 직접 선택할 수 있습니다.  
    STR_ID_GP_INIT_ATTRBUTE_ITEM_ERR        = 10913, // 속성 초기화 주문서가 없습니다. 
    
    STR_ID_RAID_CLEAR_COUNT 				= 10924, //오늘 클리어 횟수 : @1/@2회 
    STR_ID_RAID_CLEAR_COMPLETE 				= 10925, //클리어 횟수 달성 
    STR_ID_RAID_CONDITION 					= 10926, //Lv.@1 / 장비 등급 @2이상 
    STR_ID_RAID_JOIN_FAIL 					= 10927, //방에 입장할 수 없습니다. 	
    
    STR_ID_NIGHTMARKET_FORGE                = 10929, // 빛나는 야명주가 제작되었습니다.
    STR_ID_RAID_JOIN_TIME					= 10931, //	입장 가능 시간이 아닙니다.  
    STR_ID_RAID_NON_OPTION					= 10938, //( 옵션 없음 ) 	
    
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8    = 10941, // 선택한 영웅 던전에 입장하기 위한 조건에 미달하였습니다. 
    STR_ID_HERO_DUNGEON_ENTERANCE_ERROR9    = 10942, // 능력치 등급이 모자라서 게임을 진행할 수 없습니다.
    STR_ID_ROOM_BONUS_PCBANG                = 10943, // PC방
    STR_ID_ROOM_BONUS_GCCLUB                = 10944, // GC Club
    STR_ID_ROOM_BONUS_GCBLOG                = 10945, // GC Blog	
    
    STR_ID_DIO_DEVIL_SKILL_DESC             = 10946,
    STR_ID_DIO_WEAPON_SKILL_DESC            = 10947,
    STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC9 = 11028,
    STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR0 = 11035, // 친구 리스트를 얻지 못하였습니다.\n다시 시도해 주시길 바랍니다. 
    STR_ID_AUTO_MATCH_INVITE_FRIEND = 11041, // 초대 해 주세요 
    STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR1 = 11042, // 파티장이 사용할 수 있는 기능입니다. 
	STR_ID_RECOM_GET_REWARD_ORDER_ERR       = 11085, // 낮은 단계 보상을 먼저 받아야 합니다.
    STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR2 = 11086, // 파티원이 준비되지 않았습니다. 
    STR_ID_AUTO_MATCH_ERR0 = 11087, // 던전 서버에서 대전 플레이를 하실 수 없습니다. 
    STR_ID_AUTO_MATCH_ERR1 = 11088, // 대전 서버에서 던전 플레이를 하실 수 없습니다. 
    STR_ID_AUTO_MATCH_ERR2 = 11089, // 존재하지 않는 캐릭터로 매칭 요청 하였습니다. 
    STR_ID_AUTO_MATCH_ERR3 = 11090, // 파티 정보가 이상합니다. 
    STR_ID_AUTO_MATCH_ERR4 = 11091, // 서버 연결 상태가 좋지 않습니다. 
    STR_ID_AUTO_MATCH_ERR5 = 11092, // 파티장만 매칭을 시작할 수 있습니다.
    STR_ID_AUTO_MATCH_ERR6 = 11093, // 파티원들이 준비상태가 아닙니다. 

    STR_ID_JOPNAME_14_1                     = 11113,
    STR_ID_AUTO_MATCH_ERR7 = 11114, // 매칭이 취소 되었습니다.
    STR_ID_AUTO_MATCH_MAP_DEFAULT = 11116, // 선택사항 없음 
    STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR3 = 11117, // 이미 초대된 대상입니다. 
    STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR4 = 11118, // 현재 같은 파티원입니다. 
    STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR5 = 11119, // 파티 초대 인원이 초과되었습니다. 
    STR_ID_AUTO_MATCH_OPTION = 11120, // 자동 수락
    STR_ID_JOPNAME_14_2                     = 11122,
    STR_ID_JOPNAME_14_3                     = 11123,
    STR_ID_AUTO_MATCH_OVER_TIME_DESC = 11125, // 현재 해당모드를 신청한 유저가 많지 않아\n상대찾기가 수월하지 않습니다.\n잠시만 기다려 주세요. 
    STR_ID_MATCH_INFO_0 = 11126,	//남은 판수
    STR_ID_MATCH_INFO_1 = 11127,	//랭크
    STR_ID_MATCH_INFO_2 = 11128,	//배치고사 진행 중
    STR_ID_MATCH_INFO_3 = 11129,	//@1판
    STR_ID_MATCH_INFO_4 = 11130,	//입장 가능한 시간이 아닙니다.\n@1시 @2분 부터 @3시 @4분까지 매칭이 가능합니다. 
    STR_ID_MATCH_INFO_5 = 11131,	//승 @1
    STR_ID_MATCH_INFO_6 = 11132,	//패 @1
    STR_ID_MATCH_INFO_7 = 11133,	//승률 @1%
    
    STR_ID_AUTO_MATCH_NOT_USE_4SPECIAL = 11134, // 자동 매칭모드에서는 4단 필살기를 사용 할 수 없습니다.
    STR_ID_MATCH_INFO_8 = 11135,	//대전 자동 매칭이 @5분 후 오픈 됩니다. @1시 @2분 부터 @3시 @4분까지 매칭이 가능합니다.  
    STR_ID_MATCH_INFO_9 = 11136,	//대전 자동 매칭이 @5분 후 종료 됩니다. @1시 @2분 부터 @3시 @4분까지 매칭이 가능합니다.
    STR_ID_MATCH_INFO_10 = 11137,	//오픈시간  
    STR_ID_MATCH_INFO_11 = 11138,	//오전
    STR_ID_MATCH_INFO_12 = 11139,	//오후
    STR_ID_MATCH_INFO_13 = 11140,	//@1 @2시 @3분 ~ @4 @5시 @6분

    STR_ID_RITASCHRISTMAS_OUTOFRANK			= 11212,    // 아직도 순위 밖이군요!/n 좀 더 분발해 주세요!
    STR_ID_RITASCHRISTMAS_BESTSCORE		= 11213,    // 오늘의 최고 점수
    STR_ID_RITASCHRISTMAS_PRIMIUM_ERR	= 11214,    // 프리미엄박스 오픈 에러
    STR_ID_RITASCHRISTMAS_PRIMIUMBOX		= 11215,    // 프리미엄 상자
    STR_ID_RITASCHRISTMAS_SPECIALKEY		= 11216,    // 스페셜 열쇠
    STR_ID_RITASCHRISTMAS_HELP_0			= 11217,    // #c000000울먹이는 라임을\n 이동시켜 보세요!#cx
    STR_ID_RITASCHRISTMAS_HELP_1			= 11218,    // #c000000재료를 개수만큼 모으면\n 피버타임이 시작됩니다.#cx
    STR_ID_RITASCHRISTMAS_HELP_2			= 11219,    // #c000000바구니를 비워 피버타임을 완성시켜 보세요!#cx
    STR_ID_RITASCHRISTMAS_OPENEXPLAIN		= 11220,    // 상자를 열면\n아이템이 쏟아져요!
    STR_ID_RITASCHRISTMAS_STAGE				= 11221,	//스테이지 @1
    STR_ID_RITASCHRISTMAS_GAIN_SPECIALBOX	= 11222,	//스페셜상자 @1 획득

    STR_ID_HERO_ITEM_UPGRADE_CONFIRM = 11224, // 업그레이드를 하게 되면 몬스터 카드가 사라지고 속성이 초기화 됩니다.\n업그레이드를 진행 하시겠습니까?
    STR_ID_HERO_ITEM_UPGRADE_SUCCESS = 11225, // @1 아이템으로\n 업그레이드를 성공하였습니다
	STR_FATIGUE_SYSTEM_MESSAGE_4			= 11226, 
	STR_FATIGUE_SYSTEM_MESSAGE_5			= 11227, //
	STR_FATIGUE_SYSTEM_MESSAGE_6			= 11229, //

	STR_ID_RITASCHRISTMAS_LEFT_COIN  = 11231,
	STR_ID_RITASCHRISTMAS_LEFT_BOX  = 11232,

	STR_ID_RITASCHRISTMAS_RANK_GUIDE  = 11234,
	STR_ID_RITASCHRISTMAS_ITEM_GUIDE  = 11241,

    STR_ID_AUTOMATCH_RANKING_DESC = 11242,    // 매일 새벽 9:00에 업데이트 되며 자동 매칭 시스템의 결과에 따라 순위가 결정 됩니다.
    STR_ID_AUTOMATCH_RANKING_RANK1 = 11243,   // 자동 매칭 순위
    STR_ID_AUTOMATCH_RANKING_RANK2 = 11244,   // 다승 순위

    STR_ID_AUTOMATCH_RANKING_INFO = 11257,    // 캐릭터 순위 : @1위
    STR_ID_AUTOMATCH_REWARD_DESC = 11258,     // 지난 시즌의 매칭 순위에 따라 지급 되며, 시즌이 변경 될 때 마다 아이템의 주인이 바뀝니다.
    STR_ID_JOPNAME_11_2 = 11260,
	
	STR_FATIGUE_SYSTEM_MESSAGE_7			= 11261, //
	STR_FATIGUE_SYSTEM_MESSAGE_8			= 11272, //
	
    STR_ID_TOOLTIP_YOU_NEED_HELL_TICKET3    = 11358, // A Champion Mode Ticket is required to play Event dungeon's Hell Mode.	
    STR_ID_JOPNAME_11_3 = 11360,// 어드밴서 

    STR_ID_ATTENDANCE_STAMP_STR_1 = 11391,              // @ 일차
    STR_ID_ENCHANT_SYSTEM_PERIODITEM_ERROR = 11405,	//기간제 아이템은 사용할 수 없습니다.

    STR_ID_HAUNT_CURSE_EMERGENCY_MSG = 11407,
	
	STR_AGREEMENT_CHECK			= 11457, //

    STR_ID_BUFF_DONATION_STR_1 = 11470,              // 초콜릿 없습니다.	
    STR_ID_BUFF_DONATION_STR_2 = 11471,              // 한번에 최대 1000개를 기부 할 수 있습니다.	
    
    STR_ID_LEY3_CLASS_NAME = 11477,
    STR_ID_JOPNAME_12_2 = 11479,

    STR_ID_JOPNAME_12_3 = 11561,
	
	STR_BILLING_CN_MESSAGE_1	=	11562	,
	STR_BILLING_CN_MESSAGE_2	=	11563	,
	STR_BILLING_CN_MESSAGE_3	=	11564	,
	STR_BILLING_CN_MESSAGE_4	=	11565	,
	STR_BILLING_CN_MESSAGE_5	=	11566	,
	STR_BILLING_CN_MESSAGE_6	=	11567	,
	STR_BILLING_CN_MESSAGE_7	=	11568	,
	STR_BILLING_CN_MESSAGE_8	=	11569	,
	STR_BILLING_CN_MESSAGE_9	=	11570	,
	STR_BILLING_CN_MESSAGE_10	=	11571	,
	STR_BILLING_CN_MESSAGE_11	=	11572	,
	STR_BILLING_CN_MESSAGE_12	=	11573	,
	STR_BILLING_CN_MESSAGE_13	=	11574	,
	STR_BILLING_CN_MESSAGE_14	=	11575	,
	STR_BILLING_CN_MESSAGE_15	=	11576	,
	STR_BILLING_CN_MESSAGE_16	=	11577	,
	STR_BILLING_CN_MESSAGE_17	=	11578	,
	STR_BILLING_CN_MESSAGE_18	=	11579	,
	STR_BILLING_CN_MESSAGE_19	=	11580	,
	STR_BILLING_CN_MESSAGE_20	=	11581	,
	STR_BILLING_CN_MESSAGE_21	=	11582	,
	STR_BILLING_CN_MESSAGE_22	=	11583	,
	STR_BILLING_CN_MESSAGE_23	=	11584	,
	STR_BILLING_CN_MESSAGE_24	=	11585	,
	STR_BILLING_CN_MESSAGE_25	=	11586	,
	STR_BILLING_CN_MESSAGE_26	=	11587	,
	STR_BILLING_CN_MESSAGE_27	=	11588	,
	STR_BILLING_CN_MESSAGE_28	=	11589	,
	STR_BILLING_CN_MESSAGE_29	=	11590	,
	STR_BILLING_CN_MESSAGE_30	=	11591	,
	STR_BILLING_CN_MESSAGE_31	=	11592	,
	STR_BILLING_CN_MESSAGE_32	=	11764	,
	STR_BILLING_CN_MESSAGE_33	=	11771	,
	
	STR_ADD_SLOT_CHECK_ERROR1           = 11765, //보유한 슬롯이 있습니다. 캐릭터 생성을 해주시기 바랍니다. 
    STR_ADD_SLOT_CHECK_ERROR2           = 11766, //캐릭터 슬롯이 최대 한계치에 도달했습니다. 추가로 슬롯을 확장할 수 없습니다. 
    STR_ADD_SLOT_CHECK_ERROR3           = 11767, //슬롯 추가 아이템이 인벤토리에 없습니다. 
    STR_ADD_SLOT_CHECK_ERROR4           = 11768, //슬롯 추가 동작이 지연되어 '@1' 캐릭터의 인벤토리로 아이템이 지급 되었습니다. 

    STR_ADD_SLOT_CHECK_OK           = 11769, //캐릭터 획득 슬롯이 @1개 추가 되었습니다.
    
    STR_NO_MATCH_COUNT              = 11772, //전적없음
	
	STR_PANHO_MESSAGE_0 = 11779,
	STR_PANHO_MESSAGE_1 = 11780,

	STR_GET_INVEN_ERROR             = 11788, //인벤토리 정보를 불러올수 없습니다.
	
	STR_ID_DUNGEION_ERR1 = 11789,
	STR_ID_DUNGEION_ERR2 = 11790,
	
	STR_CREATE_CHAR_NO_SLOT_ERROR   = 11792, //캐릭터 생성할 슬롯이 부족합니다.

    STR_ID_LAS4_CLASS_NAME      = 11795,	// 라스 (스트라이퍼)
    STR_ID_DIO4_CLASS_NAME      = 11796,	// 디오 (트와일라잇)
    STR_ID_LUPUS1_CLASS_NAME    = 11797,	// 루퍼스 (바운티헌터)
    STR_ID_LUPUS2_CLASS_NAME    = 11798,	// 루퍼스 (서쳐스)
    STR_ID_LUPUS3_CLASS_NAME    = 11799,    // 루퍼스 (익스큐터)
    STR_ID_LUPUS4_CLASS_NAME    = 11800,    // 루퍼스 (프로미넌스)
    STR_ID_RIN1_CLASS_NAME      = 11801,    // 린 (나래)
    STR_ID_RIN2_CLASS_NAME      = 11802,    // 린 (가람)
    STR_ID_RIN3_CLASS_NAME      = 11803,    // 린 (아라)
    STR_ID_RIN4_CLASS_NAME      = 11804,    // 린 (미리내)
    STR_ID_ASIN1_CLASS_NAME     = 11805,    // 아신 (무진)
    STR_ID_LIME1_CLASS_NAME     = 11806,    // 라임 (홀리나이트)

    STR_ID_BIND_ITEM_TO         = 11808,    // @1에게
	
	STR_BILLING_CN_MESSAGE_34	=	11809,	
	STR_CN_DISCONNECTED_SERVER = 11810,
	
	STR_CN_NON_CASH = 11815,
	
	STR_ID_DEPOT_ERR_DEPOT_15       = 11816, //창고에 넣을수 없는 아이템입니다.
	STR_ID_DEPOT_ERR_DEPOT_16       = 11817, //창고에 아이템을 넣을 공간이 부족합니다.
    STR_ID_SELL_ITEM_QUETION        = 11818, //@1 아이템을 판매 하시겠습니까?
	
	STR_ID_LOGIN_ID_SAVE = 11819,

    STR_ID_BUY_CASH_ITEM_S6_STR_1 = 11820,          // 금액 : @1원
    STR_ID_BUY_CASH_ITEM_S6_STR_2 = 11821,          // GC적립 : @1원(@2%)
    STR_ID_BUY_CASH_ITEM_S6_STR_3 = 11822,          // GC적립은 캐쉬결제시에만 해당됩니다.
    STR_ID_BUY_CASH_ITEM_S6_STR_4 = 11823,          // GC
    STR_ID_BUY_CASH_ITEM_S6_STR_5 = 11824,          // 필요한 GC
    STR_ID_BUY_CASH_ITEM_S6_STR_6 = 11825,          // 현재 GC
    STR_ID_BUY_CASH_ITEM_S6_STR_7 = 11826,          // 부족한 GC
    STR_ID_BUY_CASH_ITEM_S6_STR_8 = 11827,          // 적립된 GC
    STR_ID_BUY_CASH_ITEM_S6_STR_9 = 11828,          // 사용된 GC
    STR_ID_BUY_CASH_ITEM_S6_STR_10 = 11829,         // GC가 부족합니다.
    STR_ID_BUY_CASH_ITEM_S6_STR_11 = 11842,         // 구매하시겠습니까? 

    STR_ID_ERRAND_GUIDE_1   = 11858,                    //10분에 한번 구매해야 할 아이템의\n목록과 개수를 알려드립니다.\n잘 기억해야 해요
    STR_ID_ERRAND_GUIDE_2   = 11859,                    //각 상점에서 SPACE 키를 누르면\n아이템을 1회씩 구매합니다.\n구매가 완료되면 집으로 돌아오세요.
    STR_ID_ERRAND_GUIDE_3   = 11860,                    //갑자기 나타나는 메리를 조심하세요!\n여러분의 장바구니에 있는 물건 중\n한개를 뺏어간답니다.
    STR_ID_ERRAND_GUIDE_4   = 11861,                    //심부름에 성공하면 칭찬을 1회 받아요. 칭찬 횟수만큼 보상을 받을 수 있습니다. 한 게임에 9번의 칭찬을 모으면 VP를 덤으로!
    STR_ID_PLAY_LIST_GUIDE  = 11862,                    //이번 심부름 목록이야. 잊지말라고.\n'SPACE' 키로 닫을 수 있어. 지금도 시간은 흐르고 있으니 빨리 외우는게 좋을거야.
    STR_ID_ERRAND_EXIT      = 11863,                    //게임 중에 종료하면 지금까지 받은 칭찬과 진행사항이 사라집니다. 종료하시겠습니까?\n보상을 받지 않고 종료 하면 칭찬 횟수만큼의 보상이 인벤토리로 자동 지급됩니다.

    STR_ID_JOPNAME_15_1 = 11875,    //  무량
    STR_ID_ASIN2_CLASS_NAME = 11876, //  아신 (무량)
    STR_ID_ROOM_INFO_REV_ERROR  = 11880, //방 정보를 받지 못했습니다.

    STR_ID_ERRAND_EXIT_COMPLATE = 11881,                 //보상은 인벤토리로 지급되었습니다. 10분 후에 다시 만나요!
	
	STR_ID_MISSION_COMPLETE_ERR = 11882,
	
	STR_ID_DEPOT_ADD_SLOT_EX        = 11878, // [ @1 ] 아이템이 @2 개 소모됩니다. 슬롯을 확장하시겠습니까?
    STR_ID_DEPOT_ADD_SAFE_EX        = 11879, // [ @1 ] 아이템이 @2 개 소모됩니다. 금고를 확장하시겠습니까?

    STR_ID_FROM_DUNGEON_TO_EVENT_DUNGEON     = 11945,
    STR_ID_FROM_EVENT_DUNGEON_TO_DIFFDUNGEON = 11947,
	
    STR_ID_SUBJECT = 11970,	

    STR_ID_EDEL_NAME            = 11972,    
    STR_ID_CHAR17_DESC          = 11973,
    STR_ID_CHAR17_WEAPON        = 11974,    
    STR_ID_GET_NEW_CHARACTER17  = 11975,
    STR_ID_JOPNAME_17_0         = 11976,
	
    STR_ID_FROM_NORMAL_TO_PRIVATE_DUNGEON = 11977,      // 일반 던전에서 캐릭터 전용 던전으로 이동할 수 없습니다.
    STR_ID_NO_CLEAR_PRIVATE_DUNGEON = 11978,            // 전용던전을 모두 오픈하지 않았습니다.\n 전용던전을 모두 오픈한 후에 사용할 수 있습니다.
    STR_ID_NOT_MOVE_PRIVATE_DUNGEON = 11979,            // 캐릭터 전용던전으로 따라가기를 할 수 없습니다.	

    STR_ID_JOPNAME_16_1 = 11980,  // 세인트 
    STR_ID_LIME2_CLASS_NAME = 11981,
	
    STR_ID_NO_CLEAR_PRIVATE_DNGEON2 = 11982,            // 전용던전을 모두 오픈하지 않았습니다.\n 전용던전을 모두 오픈한 후에 대전 서버에 입장이 가능합니다.	
    
    STR_ID_CONTINENT_FLOSLAND = 12057,
    STR_TUTORIAL_ENTER_CONFIRM_MSG = 12028,  //신규 유저를 위한 튜토리얼이 있습니다. 진행하시겠습니까?\n취소하면 다시 할 수 없습니다.
    STR_TUTORIAL_CANCEL_CONFIRM_MSG = 12029, //튜토리얼을 취소하시겠습니까?\n튜토리얼은 다시 할 수 없습니다.
    STR_TUTORIAL_GUIDE_SYSTEM_MSG_1 = 12031, //좌우 방향키를 이용하면 움직일 수 있습니다.\n다음 스테이지로 갈 수 있는 맨 오른쪽의 포탈까지 이동하세요.
    STR_TUTORIAL_GUIDE_SYSTEM_MSG_2 = 12032, //아래 방향키를 이용하면 포탈을 타고 다음 스테이지로 이동합니다.
    STR_TUTORIAL_FUNCTION_PROHIBIT = 12039, //튜토리얼 진행 중에는 다른 기능을 사용하실 수 없습니다. 
	
	STR_BILLING_CN_MESSAGE_35	=	12043,
	STR_BILLING_CN_MESSAGE_36	=	12044,

    STR_ID_USE_LOOK_INVEN_EXTEND    = 12045, // '코디 인벤토리 확장권' 아이템을 사용하시겠습니까? 
	STR_TOTAL_ATTACK        = 12046, //종합공격력
    STR_CRITICAL_DAMAGE     = 12047, //치명타 데미지

    STR_ID_CAPSLOCK_ON = 12048,  //CapsLock이 켜져 있습니다.
    STR_ID_VIRTUALKEY_BACKSPACE = 12049,    //지우기
    STR_ID_VIRTUALKEY_UPPERLOWER = 12050,  //대소문자

    STR_ELYOS_RESISTANCE     = 12059,

    STR_VITALITY_STR_1 = 12062, // 활력 10 소모 시 충전 포인트가 1씩 상승 합니다. 포인트가 1 이상일 때 충전이 가능합니다.
    STR_VITALITY_STR_2 = 12063, // 보유 충전 포인트 : @1/@2
    STR_VITALITY_STR_3 = 12064, // 활력 및 활력 충전 포인트는 아침 6시를 기준으로  초기화 됩니다.
    STR_VITALITY_STR_4 = 12065, // 가용 가능한 활력 충전 포인트 : @1
	STR_VITALITY_STR_5 = 12066, // 사용 하시겠습니까?
    STR_VITALITY_STR_6 = 12067, // 활력 충전이 완료 되었습니다.
    STR_VITALITY_STR_7 = 12068, // 활력 : @1
	
    STR_ID_RYAN3_JUMP_UP_ATK    = 12069,
    STR_ID_RYAN3_JUMP_UP_ATK2   = 12070,
    STR_ID_RONAN2_DRAGON_DIVE2  = 12071,
    STR_ID_RYAN3_Z_DOWN_ATK     = 12072,

    STR_ID_LOOK_INVEN_EXTEND_FAIL0  = 12109, //	코디 옷장은 @1칸 까지 늘릴 수 있습니다.\n더 이상 사용 할 수 없습니다.
    STR_ID_GACHA_ERROR10        = 12155, // 12155	이미 모든 구슬 봉인 해제를 성공하여 더 이상 봉인 해제를 할 수 없습니다.
	
    STR_ID_ITEM_GRADE_MYSTIC    = 12156,
    STR_ID_ITEM_GRADE_LEGEND    = 12156,
    STR_ID_ITEM_GRADE_EPIC      = 12156,
    STR_ID_ITEM_GRADE_MAGIC     = 12156,
    STR_ID_ITEM_GRADE_NORMAL    = 12156,

    STR_BILLING_CN_MESSAGE_38	=	12161,
    STR_BILLING_CN_MESSAGE_39	=	12162,
    STR_BILLING_CN_MESSAGE_40	=	12163,
	
    STR_ID_ITEM_GRADE_STRING6   = 12164,
    STR_ID_ENCHANT_ATTRIBUTE_DESC15 = 12165,

    STR_UNLIMITED_REBIRTH_SYSTEM_MSG = 12178,  //부활완료. 시련의 숲에서는 무제한 부활이 가능합니다.
	STR_ID_ENCHANT_ATTRIBUTE_MARK_DESC07     = 12179, //크리티컬 데미지 @1 @2% 상승
    STR_ID_NO_DELETE_MISSION = 12183,                   // 삭제할 수 없는 미션입니다.
	
	STR_CHATTING_RESTRICT_MSG1 = 12184, //당신은 영구적으로 채팅이 금지된 상태 입니다.
	STR_CHATTING_RESTRICT_MSG2 = 12185, //@1년 @2월 @3일까지 채팅이 금지된 상태 입니다.

    STR_ID_EDEL_TUTORIAL_NAME = 12345,
    STR_ID_FROM_PRIVATE_TO_NORMAL_DUNGEON = 12346,  // 캐릭터 전용 던전에서 일반 던전으로 이동할 수 없습니다.   


    STR_ID_ATTRIBUTE_MIGRATION_TOOLTIP = 12058, // 속성 변경 이벤트 기간입니다. 공격 관련 속성만 재선택 할 수 있습니다.

 	STR_ID_JOIN_GUILD_MAX_NUM = 12355,	//길드 가입 최대 인원수를 초과하여 길드에 가입할 수 없습니다.
    STR_ID_GUILD_JOIN_SETTING_MAX_NUM = 12356,	//길드 최대 인원수를 초과하여 길드 가입 승인 방법을 변경할 수 없습니다. 
    STR_CHANGE_CYOU_POINT_MAX_OVER = 12364, //매번 최대 9999를 전환할 수 있습니다.
    STR_CHANGE_CYOU_POINT_SUCCESS = 12365, //전환 완료 되었습니다.
	
    STR_ID_MATCH_LOADINGSCENE_TIP_TITLE = 12366,//대전 플레이 Tip
    STR_ID_MATCH_LOADINGSCENE_TIP_CONTENTS = 12367, //'전 모드에서는 캐릭터의 능력치가 일정 부분 조정됩니다.

    STR_ID_LOW_QULITY = 12368,  //저사양
    STR_ID_HIGH_QULITY = 12369, //고사양
    STR_ID_NUM = 12374, //@1
    STR_ID_GUILD_BATTLE_POINT = 12385, //BPoint

    STR_ID_ITEM_CREATION_ERROR6 = 12370, // 수량을 지정하여 제작 할 수 없는 아이템입니다.

    STR_ID_JUMPING_CHAR_REWARD_OK   = 12371, // 아이템 보상을 획득하였습니다.
    STR_ID_JUMPING_CHAR_QUESTION    = 12372, // 해당 캐릭터를 점핑 캐릭터로 선택 하시겠습니까?
    STR_ID_JUMPING_CHAR_OK          = 12373, // 점핑에 성공하였습니다.


    STR_ID_JUMPING_CHAR_DESC0       = 12375, // 점핑 캐릭터 혹은 아이템 보상을 선택 해 주세요. 
    STR_ID_JUMPING_CHAR_DESC1       = 12376, // 원하는 캐릭터를 선택하면 @1 레벨의 전직 및 캐쉬 스킬, 4단 필살기, 엠피 4칸, 적정레벨의 장비등이 지급된 캐릭터를 지급 받게 됩니다. 
    STR_ID_JUMPING_CHAR_DESC2       = 12377, // 단, 이미 보유하고 있는 캐릭터의 경우도 @1레벨과 함께 위와 같은 상태로 변경 됩니다. 
    STR_ID_JUMPING_CHAR_DESC3       = 12378, // 아이템 보상을 선택 할 경우 점핑 캐릭터는 지급 받을 수 없는 대신 영웅주화와 데몬코어 아이템을 바상으로 지급 받습니다. 

    STR_ID_JUMPING_CHAR_REWARD_QUESTION  = 12379, // 아이템 보상을 받으면 점핑 캐릭터를 획득 할 수 없습니다.

    STR_ID_JUMPING_CHAR_ERROR1      = 12380,    // 점핑 캐릭터 이벤트를 진행할 수 없는 상태입니다.
    STR_ID_JUMPING_CHAR_ERROR2      = 12381,    // 점핑 캐릭터를 할 수 없는 캐릭터입니다.
    STR_ID_JUMPING_CHAR_ERROR3      = 12382,    // 보유하신 캐릭터 레벨이 점핑 레벨 보다 높아서 진행할 수 없습니다.
    STR_ID_COUNTER_RESIST_IN_PVP = 12383, //브라질만 쓰는 스트링 대전 카운터 저항
    STR_ID_MP_RECOVERY_IN_PVP = 12384, //브라질만 쓰는 스트링 대전 MP 회복률

    STR_ID_TOURNAMENT_NOT_USE_4SPECIAL =  12388, // 12388	대회 서버에서는 4단 필살기를 사용할 수 없습니다.

    STR_ID_BEIGAS_NAME = 12389,
    STR_ID_BEIGAS_TUTORIAL_NAME = 12390,
    STR_ID_JOPNAME_18_0 = 12391,
    STR_ID_CHAR18_WEAPON = 12392,
    STR_ID_CHAR18_DESC = 12393,

    STR_ID_SOCKET_NEED_COUNT = 12394,	//소모 @1
    STR_ID_SOCKET_OWN_COUNT = 12395,	//현재 @1

    STR_ID_COORDI_COMPOSE_DESC  = 12397,    //  코디 합성 : @1
    STR_ID_COORDI_COMPOSE_ERROR0    = 12398, // 코디 아이템 정보가 올바르지 않습니다.
    STR_ID_COORDI_COMPOSE_ERROR1    = 12399, // 코디 아이템만 합성이 가능합니다.
    STR_ID_COORDI_COMPOSE_ERROR2    = 12400, // 능력치가 있는 코디 아이템만 지정할 수 있습니다.
    STR_ID_COORDI_COMPOSE_ERROR3    = 12401, // 기간제 코디는 합성을 진행할 수 없습니다.
    STR_ID_COORDI_COMPOSE_ERROR4    = 12402, // 공용 아이템은 합성을 진행할 수 없습니다.
    STR_ID_COORDI_COMPOSE_ERROR5    = 12403, // 장착중인 아이템은 합성을 진행할 수 없습니다.
    STR_ID_COORDI_COMPOSE_ERROR6    = 12404, // 장착 슬롯이 다른 아이템은 합성을 진행할 수 없습니다.
    STR_ID_COORDI_COMPOSE_ERROR7    = 12405, // 전직이 다른 아이템은 합성을 진행할 수 없습니다.
    STR_ID_COORDI_COMPOSE_ERROR8    = 12406, // 코디 합성 쿠폰이 부족합니다.
    STR_ID_COORDI_COMPOSE_OK        = 12407, // 코디 합성에 성공했습니다.
	
    STR_ID_CONTINENT_DEVILDOM = 12431,
    

    STR_ID_FULLMOONEVENT           			= 12668, //[ 보름달 이벤트!! 오늘은 [ @1 ] 진행중입니다!! 자세한 내용은 홈페이지를 참고해주세요~! ]
    STR_ID_FULLMOON7						= 12676, //[보름달이 떴다!!!!] 이벤트가 진행중입니다.

    STR_ID_FULLMOON_JACKPOT					= 12678, // 운영자공지 : [무지개가 떴다!!!] 이벤트가 발동되었습니다!! 7가지 이벤트가 모두 적용됩니다!!
    STR_ID_FULLMOON_SQUARE					= 12679, // [무지개가 떴다!!] 이벤트가 발동하면 [소망의 무지개] 아이템을 지급합니다. [소망의 무지개] 아이템이 많을 수록 더 많은 추가 보상이 지급됩니다.
	
    STR_ID_TCP_CONNECT_FAIL = 12710,    // 릴레이 서버 연결에 실패하였습니다.
	
    STR_GIFTBOX_REWARD_WAIT = 12712, //서버처리중
    STR_ERR_CONNECTION_GIFTBOX_ERROR_01 = 12713,//신규 유저 대상이 아닙니다.
    STR_ERR_CONNECTION_GIFTBOX_ERROR_02 = 12765,//현재 등급의 보상 정보가 없습니다.
    STR_ERR_CONNECTION_GIFTBOX_ERROR_03 = 12766,//유저 정보 갱신 중, DB 기록 실패 
    STR_ERR_CONNECTION_GIFTBOX_ERROR_05 = 12767,//누적 시간 정보가 일치 하지 않습니다.
	
	STR_ID_TAEGEUKGI_RED = 12768,
    STR_ID_TAEGEUKGI_BLUE = 12769,
    STR_ID_TAEGEUKGI_YELLOW = 12770,
		
	STR_ID_VIRTUAL_DEPOT_MSG1 =  12771,  // 아이템 이동이 완료 되었습니다.
    STR_ID_VIRTUAL_DEPOT_MSG2 =  12772,  // 아이템 이동이 완료 되었습니다.
    STR_ID_VIRTUAL_DEPOT_MSG3 =  12773,  // 임시 보관 창고를 모두 비우기 전에는 창고 시스템을 이용할 수 없습니다. 캐릭터 선택창 화면의 임시 보관 창고에 들어가서 아이템을 모두 캐릭터 인벤토리로 이동시켜주세요. 
    STR_ID_VIRTUAL_DEPOT_MSG4 =  12774,  // 가상 창고에 아이템이 남아 있습니다. 캐릭터 인벤토리로 모두 이동 시키지 않으면 창고 사용에 제한이 있을 수 있습니다. 
    STR_ID_VIRTUAL_DEPOT_MSG5 =  12775,  // 공용 아이템 탭에 있는 아이템은 추 후 창고를 통해 캐릭터간 아이템 이동을 자유롭게 할 수 있습니다. 
    STR_ID_VIRTUAL_DEPOT_MSG6 =  12776,  // 캐릭터 귀속 아이템 탭에 있는 아이템은 추 후 캐릭터간 아이템 이동이 불가능 합니다. 신중하게 선택 해주세요. 
    STR_ID_VIRTUAL_DEPOT_MSG7 =  12777,  // 이대로 결정 하시겠습니까? 
	
    STR_ID_GUILDMARK_CHANGEITEM_NOTICE = 12785, //\n‘@1’ 캐릭터의 인벤토리로 길드 마크 변경권 아이템이 지급 되었습니다. 확인해주세요.

    STR_ID_SOCIAL_COMMERCE_1 = 12818,       //	금액
    STR_ID_SOCIAL_COMMERCE_2 = 12819,       //	목표 인원
    STR_ID_SOCIAL_COMMERCE_3 = 12820,       //	@1년 @2월 @3일부터 ~ @4년 @5월 @6일까지
    STR_ID_SOCIAL_COMMERCE_4 = 12821,       //	판매 종료와 동시에 일괄지급
    STR_ID_SOCIAL_COMMERCE_5 = 12822,       //	기간 내에 구매 수량이 확보되지 못할 경우 보상을 받을 수 없습니다

	STR_ID_DONATION_EVENT_DESC = 12825,	// @1개 : @2
	
    STR_ID_CHAR_NICK_STR1 = 12859,      //캐릭터마다 각각의 닉네임을 설정 할수 있게 되었습니다.
    STR_ID_CHAR_NICK_STR2 = 12860,      //캐릭터의 닉네임을 변경해 보세요!

	STR_ID_COORDI_UPGRADE_ERROR0 = 12865,   // 아이템 정보가 올바르지 않습니다.
    STR_ID_COORDI_UPGRADE_ERROR1 = 12866,   // 코디 아이템만 등급을 업그레이드 할 수 있습니다.
    STR_ID_COORDI_UPGRADE_ERROR2 = 12867,   // 능력치가 있는 코디만 업그레이드 할 수 있습니다.
    STR_ID_COORDI_UPGRADE_ERROR3 = 12868,   // 장착중인 코디는 업그레이드 할 수 없습니다.
    STR_ID_COORDI_UPGRADE_ERROR4 = 12869,   // 요청 하신 아이템은 이미 최고 등급입니다.
    STR_ID_COORDI_UPGRADE_ERROR5 = 12870,   // 등급 업그레이드가 불가능 한 아이템 입니다.
    STR_ID_COORDI_UPGRADE_ERROR6 = 12871,   // 업그레이드 재료가 부족합니다.
    STR_ID_COORDI_UPGRADE_OK     = 12872,   // 등급 업그레이드에 성공하였습니다.
    
    STR_ID_VIRTUAL_DEPOT_MOVE_STR = 12873,  //가상창고에서 인벤토리로 한번에 100개의 아이템을 옮길 수 있습니다.
    STR_ID_VIRTUAL_DEPOT_NEW_STR1 = 12874,  //이 후에도 창고를 통해 캐릭터간 이동이 자유롭습니다. 편하게 옮기세요.
    STR_ID_VIRTUAL_DEPOT_NEW_STR2 = 12875,  //이 후 창고를 통해 이동할 수 없습니다. 신중하게 옮기세요.
    STR_ID_NUM_REPLACE = 12891, //@1

    STR_ID_WEB_BUY_ITEM = 12889,  //구매 아이템 가상창고로 이동 하였습니다.
    STR_ID_VIRTUAL_DEPOT_TIP = 12890, // 창고 위쪽 아이템 부터 100개씩 한 번에 옮길 수 있습니다.
	
    STR_ID_FROM_DUNGEON_TO_DIFFDUNGEON = 12892, // 해당 던전에서는 다른 던전을 선택할 수 없습니다. 
    STR_ID_NOT_MOVE_INFINITY_DUNGEON = 12893, // 해당 던전으로 따라가기를 할 수 없습니다. 
    STR_ID_INFINITY_DUNGEON_INVITE = 12894, // 해당 던전에서는 초대를 할 수 없습니다. 	

    STR_ID_ENABLE_DEPOT_MOVE = 12895, // 창고 이동 가능
    STR_ID_DISABLE_DEPOT_MOVE = 12896, // 창고 이동 불가능
	STR_ID_PRESENT_OWN_CASH = 12897,	// 보유 캐시
    STR_ID_FOLLOW_OTHER_SERCER = 12898, //	다른 서버로는 이동할 수 없습니다.	

    STR_ID_EQUIP_NEW_SR_SLOT         = 12885,

    STR_ID_JOPNAME_17_1         = 12899,
    STR_ID_EDEL2_TUTORIAL_NAME  = 12901,
    STR_ID_GMC_FATAL_DEATHMATCH = 12902, // 한방

    STD_ID_MAX_COMBO_NUM = 12903, //최대 콤보 수
    STD_ID_MAX_COMBO_DAMAGE = 12904, //최대 콤보 데미지
    STD_ID_CONTINOUS_KILL_NUM = 12905,//콤보 중 연속 킬 수
    STD_ID_BACK_AERIAL_ATTACK_NUM = 12906,//Back, Aerial 공격 수
    STD_ID_HITTED_BY_MONSTER = 12907, //몬스터 피격 데미지
    STD_ID_DUNGEON_DAMAGE_CONTRIBUTION = 12908, //던전 데미지 기여도
    STR_ID_TOOLTIP_GPSHOP_PURCHASE_GUIDE = 12909, //구입 시 레어나 에픽등급의 아이템을 랜덤하게 획득할 수 있습니다.
    STR_ID_TOOLTIP_HERODUNGEON_REMAIN_ENDTIME = 12910,//	입장 종료까지 @1분 남았습니다. ( @2 / @3 )
    STR_ID_TOOLTIP_HERODUNGEON_REMAIN_STARTTIME = 12911,//	다음 입장까지 @1분 남았습니다. ( @2 / @3 )
    
    STR_ID_TOOLTIP_HEROSHOP_LIMIT_LEVEL = 12912, //	30레벨 이하는 영웅상점에 입장할 수 없습니다.

    STR_ID_INFINITY_GUIDE_DLG = 12913, // 사냥터 가이드 

	STR_ID_PRESENT_DEFAULT_MESSAGE = 12927,				// 님이 보내신 선물입니다.
	STR_ID_PRESENT_ALREADY_CHECKED_NICKNAME = 12928,	// 이미 확인된 닉네임입니다.
	STR_ID_PRESENT_NICKNAME_IS_CHECKED_NOW = 12929,		// 닉네임이 확인되었습니다.
	STR_ID_PRESENT_CANNOT_SEND_TO_MYSELF = 12930,		// 자신에게 선물을 보낼 수 없습니다.
	STR_ID_PRESENT_PLEASE_CHECK_NICKNAME = 12931,		// 닉네임 확인을 해 주세요.
	STR_ID_PRESENT_TIP = 12932,							// Tip : 선물 받는 유저는 재접속 후\n 우편함을 통해 선물을 받으실 수 있습니다.
	STR_ID_PRESENT_OWN_VP = 12933,						// 보유 VP

	STR_ID_IS_EXCLUSIVE = 12961,						// 전용
    STR_ID_KAIRO_ONESHOT_MSG = 12965, //한번에 100개를 여시겠습니까? (아니오는 한개만 오픈)
	STR_ID_DIO_CHAR_CREATE_ERROR = 12966, //30레벨 이상 캐릭터 최소 1개 이상 보유 시에만 캐릭터 생성이 가능합니다
    STR_ID_WELCOME_NEW_NICKNAME_MESSAGE = 12967, //	그랜드체이스에 돌아오신 것을 환영합니다. 새로운 닉네임과 함께 즐거운 그랜드체이스  되주세요.

    STR_ID_COUPLE_SYSTE_ERROR_MSG01 = 12968,            //커플 요청을 할수 있는 상태가 아닙니다.
    STR_ID_COUPLE_SYSTE_ERROR_MSG02 = 12969,            //커플 요청 받을수 있는 상태가 아닙니다.
    STR_ID_COUPLE_SYSTE_ERROR_MSG03 = 12970,            //자신에게 커플 신청 할수 없습니다.
    STR_ID_COUPLE_SYSTE_ERROR_MSG04 = 12971,            //대상 유저에게 신청 패킷을 보내지 못했습니다.
    STR_ID_COUPLE_SYSTE_ERROR_MSG05 = 12972,            //커플 신청 정보가 없습니다.


    STR_ID_ENCHANT_GP           = 12976,        // 소모GP/전체보유GP
    STR_ID_ENCHANT_GP_COST      = 12977,        // @1/@2
    STR_ID_ENCHANT_ENCHANT_DESC = 12980,        // 강화  설명 
    STR_ID_ENCHANT_BREAKUP_DESC = 12981,        // 해체  설명 
    STR_ID_ENCHANT_ALCHEMY_DESC = 12982,        // 연금  설명 
    STR_ID_ENCHANT_MAX_LEVEL_ERROR = 13006,     // 아이템의 강화레벨이 이미 최대 수치입니다.
    STR_ID_ENCHANT_COST_GP_ERROR = 13007,       // 강화에 필요한 GP가 모자릅니다.

    STR_ID_NO_NICKNAME = 13032,                 // Unknown

    STR_ID_DUNGEON_RANK_TOOLTIP0 = 13036,       // 클리어 기록을\n확인 할 수 있습니다!
    STR_ID_DUNGEON_RANK_TOOLTIP1 = 13037,       // 나와 전체 유저의 현재 랭킹을\n볼 수 있습니다.
    STR_ID_DUNGEON_RANK_TOOLTIP2 = 13038,       // 나와 전체 유저의 지난 랭킹을\n볼 수 있습니다.
    STR_ID_DUNGEON_RANK_TOOLTIP3 = 13039,       // 한번 더 클릭하면\n도움말을 닫습니다.
    STR_ID_DUNGEON_RANK_STAGE    = 13040,       // @1층
	
    STR_ID_EMOTICON_COUPLE = 13041,             //커플 구해요
	
    STR_ID_RANK_TEXT     = 13042,               // @1랭크

    STR_ID_SINGLE_RANDOM_ATTRIBUTE_DESC  = 13048, // 선택창에서 1개의 속성을 랜덤으로 다시 선택 할 수 있습니다.
    STR_ID_SINGLE_RANDOM_ATTRIBUTE_ITEM  = 13049, // 개별 속성 주문서 @1장(보유:@2) 
	  
    STR_ID_SINGLE_RANDOM_ATTRIBUTE_01 = 13051,      // 속성을 변경할 아이템이 인벤토리에 없습니다. 
    STR_ID_SINGLE_RANDOM_ATTRIBUTE_02 = 13052,      // 개별 랜덤 속성 주문서 아이템이 인벤토리에 없습니다. 
    STR_ID_SINGLE_RANDOM_ATTRIBUTE_03 = 13053,      // 해당 주문서를 사용할 수 있는 레벨의 아이템이 아닙니다. 
    STR_ID_SINGLE_RANDOM_ATTRIBUTE_04 = 13054,      //  해당 주문서를 사용할 수 있는 아이템이 아닙니다.  

	STR_ID_ATTENDANCE_CALENDAR_TIME = 13145,
	STR_ID_ATTENDANCE_CALENDAR_WEEK = 13146,
	STR_ID_ATTENDANCE_CALENDAR_PRIZE = 13153,

	STR_ID_HERO_DUNGEON_REWARD_INFO = 13176,
	STR_ID_HERO_DUNGEON_REWARD_CATCH = 13177,

	//////////////////////////////////////////////
	//Pet Glyph
	STR_ID_PET_MAGIC_STONE_DAMAGE_EFFECT = 13221,
	//Goes until 13226
	STR_ID_PET_GLYPH_SEAL_REMOVE = 13231,

	STR_ID_PVP_MATCH_POINT_RANK = 13249,
	STR_ID_PVP_MATCH_COUNT_INFO = 13250,
	STR_ID_PVP_MATCH_COUNT = 13251,

	STR_ID_WORLDHERO_DUNGEON_CANT_ENTER = 13253,
	STR_ID_WORLDHERO_DUNGEON_WAIT = 13254,
	STR_ID_WORLDHERO_DUNGEON_NAME = 13255,
	STR_ID_WORLDHERO_DUNGEON_BOSS_NAME = 13256,

	///////////////////////////////////////////////////////////////
	STR_ID_START_PVP_ROOM_WITH_BLOCKED_CHAR_HOST = 13258,
	STR_ID_START_PVP_ROOM_WITH_BLOCKED_CHAR_FELLOWS = 13259,

	STR_ID_START_PVP_ROOM_WITH_BLOCKED_RANK_HOST = 13260,
	STR_ID_START_PVP_ROOM_WITH_BLOCKED_RANK_FELLOWS = 13261,

	STR_ID_SELECT_PVP_ROOM_BLOCKED_CHAR = 13262,
	STR_ID_SELECT_PVP_ROOM_BLOCKED_RANK = 13263,

	STR_ID_CREATE_PVP_ROOM_DISABLED_PET_OPTION = 13264,
	STR_ID_CREATE_PVP_ROOM_DISABLED_SPECIAL4_OPTION = 13265,
	STR_ID_CREATE_PVP_ROOM_ATTR_CORRECTION_OPTION = 13266,
	STR_ID_CREATE_PVP_ROOM_NOT_ALLOWED_CHARS_OPTION = 13267,
	STR_ID_CREATE_PVP_ROOM_ALLOWED_RANKS_OPTION = 13268,

	STR_ID_CHANGE_PVP_ROOM_DISABLED_PET_OPTION = 13269,
	STR_ID_CHANGE_PVP_ROOM_ENABLED_PET_OPTION = 13270,
	STR_ID_CHANGE_PVP_ROOM_DISABLED_SPECIAL4_OPTION = 13271,
	STR_ID_CHANGE_PVP_ROOM_ENABLED_SPECIAL4_OPTION = 13272,
	STR_ID_CHANGE_PVP_ROOM_DISABLED_ATTR_CORRECTION_OPTION = 13273,
	STR_ID_CHANGE_PVP_ROOM_ENABLED_ATTR_CORRECTION_OPTION = 13274,
	STR_ID_CHANGE_PVP_ROOM_NOT_ALLOWED_CHARS_OPTION = 13275,
	STR_ID_CHANGE_PVP_ROOM_ALLOWED_CHARS_OPTION = 13276,
	STR_ID_CHANGE_PVP_ROOM_NOT_ALLOWED_RANKS_OPTION = 13277,
	STR_ID_CHANGE_PVP_ROOM_ALLOWED_RANKS_OPTION = 13278,
	STR_ID_CHANGE_PVP_ROOM_OPTION_ERROR = 13279,

	////////////////////////////////////////////////////////////////
	STR_ID_WORLDHERO_DUNGEON_REWARD_DESC = 13280,
	////////////////////////////////////////////////////////////////

	STR_ID_CANNOT_CREATE_PVP_ROOM_BLOCKED_CHAR = 13281,
	STR_ID_CANNOT_CREATE_PVP_ROOM_BLOCKED_RANK = 13282,
	STR_ID_CANNOT_CHANGE_PVP_ROOM_BLOCKED_CHAR = 13283,
	STR_ID_CANNOT_CHANGE_PVP_ROOM_BLOCKED_RANK = 13284,
	////////////////////////////////////////////////////////////////

	

	STR_ID_ROOM_EXTRA_OPTION_BLOCKED_PET = 13290,

	STR_ID_ROOM_EXTRA_OPTION_BUTTON_LIST = 13308,
	
	STR_ID_UNO_NAME = 13411,
	STR_ID_JOPNAME_19_0 = 13412,

	STR_ID_CHAR19_WEAPON = 13448,
	STR_ID_CHAR19_DESC = 13449,

	STR_ID_BILLBOARD_CHAT_TEMP_TEXT1 = 13450,
	STR_ID_BILLBOARD_CHAT_TEMP_TEXT2 = 13451,
	STR_ID_BILLBOARD_CHAT_TEXT = 13464,
	STR_ID_BILLBOARD_CHAT_WAIT_LIST = 13465,

	STR_ID_KEY_SETTING_SAVING = 13506,
	STR_ID_KEY_SETTING_NEW_USEITEM = 13507,

	STR_ID_PET_SEAL_CONFIRMATION = 13514,
	STR_ID_PET_SEAL_STATE = 13515,
	STR_ID_PET_SEAL_REMOVE = 13516,
	STR_ID_PET_DOESNT_EXIST = 13518,
	STR_ID_PET_UNSEAL_IMPOSSIBLE = 13520,

	STR_ID_CANNOT_SEAL_PET = 13532,
	STR_ID_PETVIEW_HIDING = 13533, 
	STR_ID_PET_SEAL_RARITY = 13534,
	STR_ID_PET_SEAL_QUANTITY = 13536,
	STR_ID_PET_SEAL_INVALID_PET = 13537,
	STR_ID_PET_SEAL_INVALID_SEAL = 13538,
	STR_ID_PET_SEAL_SEAL_DIFF = 13539,


	STR_ID_COORDI_GACHA_SYSTEM_MESSAGE10 = 13558,
	STR_ID_CHARACTER_LV = 13559,
	STR_ID_MAX_ATTACK = 13560,

	STR_ID_CANT_EXPAND_COORDI_INVENTORY = 13376,
	STR_ID_FAILED_TO_GET_COORDI_INFO = 13377,
	STR_ID_COORDI_MAX_SLOT_INFO = 13380,
	STR_ID_FAILED_TO_EXPAND_COORDI_INVENTORY = 13381,
	STR_ID_COORDI_INVENTORY_SPACE = 13382,
	STR_ID_COORDI_EXPAND_QUESTION = 13383,
	STR_ID_COORDI_EXPANSION_SUCCESS1 = 13384,
	STR_ID_COORDI_EXPANSION_SUCCESS2 = 13385,

	STR_ID_GACHA_PONG_NAME = 13404,
	STR_ID_GACHA_PONG_INFO_MISMATCH = 13405,
	STR_ID_GACHA_PONG_INSUFFICIENT_COIN = 13406,
	STR_ID_GACHA_PONG_INFO_ERROR = 13407,
	STR_ID_GACHA_PONG_TURN_LEVER = 13447,
};

#endif//_KGCSTRINGID3_H_   