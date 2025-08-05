#include "../GCItem.h"

//#pragma pack( push, 1 )
//typedef struct std::pair<int,bool> ItemKey;
//
//enum ECHARTYPE
//{
//    ECT_ELESIS  = 1,
//    ECT_LIRE	= 2,
//    ECT_ARME	= 4,
//    ECT_LAS		= 8,
//    ECT_RYAN	= 16,
//};
//
//enum EMONEYTYPE
//{
//    EMT_GAMEPOINT,
//    EMT_CASH,
//    EMT_CRYSTAL,
//    EMT_GEM,
//};
//
////장착위치
//enum ESLOTPOSITION
//{
//    ESP_SET_ITEM                    = 0x00000001,
//    ESP_HELMET      	            = 0x00000002,
//    ESP_JACKET                      = 0x00000004,
//    ESP_PANTS                       = 0x00000008,
//    ESP_GLOVES                      = 0x00000010,
//    ESP_SHOES	                    = 0x00000020,
//    ESP_MANTLE                      = 0x00000040,
//    ESP_WEAPON	                    = 0x00000080,
//    ESP_A_UPPER_HELMET              = 0x00000100,
//    ESP_A_DOWN_HELMET               = 0x00000200,
//    ESP_A_NECKLET                   = 0x00000400,
//    ESP_A_JACKET                    = 0x00000800,
//    ESP_A_PANTS                     = 0x00001000,
//    ESP_A_ARM                       = 0x00002000,
//    ESP_A_FINGER                    = 0x00004000,
//    ESP_A_SHOES                     = 0x00008000,
//    ESP_PET							= 0x00010000,
//};
//
//enum ESKILLPOSITION
//{
//    //skillPosiiton
//    ESP_SKC_INVALID					= 0x00000000,
//    ESP_SKC_WHITE_ONE               = 0x00000001,
//    ESP_SKC_WHITE_TWO               = 0x00000002,
//    ESP_SKC_WHITE_THREE             = 0x00000004,
//    ESP_SKC_DARK_ONE                = 0x00000008,
//    ESP_SKC_DARK_TWO                = 0x00000010,
//    ESP_SKC_DARK_THREE              = 0x00000020,
//
//    ESP_SKC_WHITE_CHANGEJOB_ONE     = 0x00000040,
//    ESP_SKC_WHITE_CHANGEJOB_TWO     = 0x00000080,
//    ESP_SKC_WHITE_CHANGEJOB_THREE   = 0x00000100,
//    ESP_SKC_DARK_CHANGEJOB_ONE      = 0x00000200,
//    ESP_SKC_DARK_CHANGEJOB_TWO      = 0x00000400,
//    ESP_SKC_DARK_CHANGEJOB_THREE    = 0x00000800,
//
//    ESP_SKC_WHITE_CHANGEJOB2_ONE    = 0x00001000,
//    ESP_SKC_WHITE_CHANGEJOB2_TWO	= 0x00002000,
//    ESP_SKC_WHITE_CHANGEJOB2_THREE	= 0x00004000,
//    ESP_SKC_DARK_CHANGEJOB2_ONE		= 0x00008000,
//    ESP_SKC_DARK_CHANGEJOB2_TWO		= 0x00010000,
//    ESP_SKC_DARK_CHANGEJOB2_THREE	= 0x00020000,
//};
//#define ESP_SKILL_MAX	19
//#define ESP_SLOT_MAX	17
//
//enum EITEMATTRIBUTE
//{
//    EIATTR_NONE			        = 0,
//    EIATTR_FIRE			        = 1,
//    EIATTR_ICE			        = 2,
//    EIATTR_LIGHTNING	        = 3,
//    EIATTR_DARKNESS		        = 4,
//    EIATTR_WIND			        = 5,
//    EIATTR_GP_SHOES_SHIELD      = 6,		// GP방어 발찌
//    EIATTR_CASH_SHOES_SHIELD    = 7,	// CASH방어 발찌
//    EIATTR_SHINING                = 8,
//};
//
////작명 요망
//enum GCGAMEMODE			// 실제 GC와 동일하게 사용할 수 없음
//{
//    GCGM_TEAM		            = 0x00000001,
//    GCGM_SURVIVAL	        	= 0x00000002,		//대전
//
//    GCGM_GUILD_BATTLE           = 0x00000004,
//
//    GCGM_CHANGEJOB_KNIGHT      	= 0x00000008,
//    GCGM_CHANGEJOB_ARCHER      	= 0x00000010,
//    GCGM_CHANGEJOB_MAGICIAN    	= 0x00000020,
//
//    GCGM_MONSTER_CRUSADERS      = 0x00000040,
//    GCGM_MONSTER_HUNT	        = 0x00000080,
//
//    GCGM_QUEST_1		        = 0x00000100,
//    GCGM_QUEST_2		        = 0x00000200,
//    GCGM_QUEST_3		        = 0x00000400,
//    GCGM_QUEST_4		        = 0x00000800,
//    GCGM_QUEST_5		        = 0x00001000,
//    GCGM_QUEST_6		        = 0x00002000,
//    GCGM_QUEST_7				= 0x00004000,
//    GCGM_QUEST_8			    = 0x00008000,
//    GCGM_QUEST_9                = 0x00010000,
//    GCGM_QUEST_10               = 0x00020000,
//    GCGM_QUEST_11               = 0x00040000,
//    GCGM_QUEST_12   	        = 0x00080000,
//    GCGM_QUEST_13   	        = 0x00100000,
//    GCGM_QUEST_14   	        = 0x00200000,
//    GCGM_QUEST_15   	        = 0x00400000,
//    GCGM_QUEST_16   	        = 0x00800000,
//    GCGM_QUEST_17   	        = 0x01000000,
//    GCGM_QUEST_18   	        = 0x02000000,
//    GCGM_QUEST_19   	        = 0x04000000,
//    GCGM_QUEST_20   	        = 0x08000000,
//};
//
////아이템 종류... 다양하게 확장합시다
//enum GCITEM_KIND
//{
//    GIK_NORMAL		            = 0,
//    GIK_CERTIFICATE             = 1,
//    GIK_MAGIC_PAPER		        = 2,
//    GIK_STRONG_ITEM             = 3,
//    GIK_STRONG_ITEM_NOT_STRONG  = 4,
//    GIK_PRINT_UID               = 5,
//    GIK_MISSION_SCROLL			= 6,
//    GIK_PET						= 7,
//};
//
//// CLIENT코드와 동일하게 맞춰줘야함
//enum GCGAMEITEM
//{
//    GCGI_NOTING = 0,
//    GCGI_SLOW,
//    GCGI_HP,
//    GCGI_MP,
//    GCGI_HIGHJUMP,
//    GCGI_TRANSPARENCY,
//    GCGI_SUPER,
//    GCGI_REFLECTION,
//};
//enum USERITEM_CATEGORY
//{
//    UIC_EQUIP1 = 0,
//    UIC_EQUIP2,
//    UIC_PET,
//    UIC_SKILL_WHITE,
//    UIC_SKILL_BLACK,
////	UIC_SHORTCUT_SLOT,
//    UIC_NUM,
//};
//
//enum GCITEM_TYPE
//{
//    GCIT_PERIOD = 0,		//기간제 아이템
//    GCIT_COUNT = 1,			//수량성 아이템
//};
//
//struct GCItemAbility
//{
//    int iAtk;
//    int iDef;
//    int iVit;
//};
//
//enum EPET_ITEM_KIND
//{
//    EPET_KIND_INVALID			= 0x00000000,       // PET관련 아이템이 아니다.
//    EPET_KIND_PET				= 0x00000001,
//    EPET_KIND_FOOD				= 0x00000002,
//    EPET_KIND_EQUIP_ITEM		= 0x00000004,
//};
//
//enum EPET_EQUIP_ITEM_SLOT
//{
//    EPET_EQUIP_ITEM_SLOT_INVALID			= 0x00000000,
//    EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM		= 0x00000001,
//    EPET_EQUIP_ITEM_SLOT_SKILL				= 0x00000002,
//};
//
//#define EPET_EQUIP_SLOT_MAX_COUNT 3
//enum EPET_EQUIP_ITEM_ATTRIBUTE
//{
//    EPET_ATTR_INVALID		        = 0x00000000,
//    EPET_ATTR_LIGHT			        = 0x00000001,
//    EPET_ATTR_DARK			        = 0x00000002,
//    EPET_ATTR_PHYSICAL				= 0x00000004,
//    EPET_ATTR_MAGIC					= 0x00000008,
//    EPET_ATTR_ICE					= 0x00000010,
//    EPET_ATTR_TEMP1					= 0x00000020,
//    EPET_ATTR_TEMP2					= 0x00000040,
//    EPET_ATTR_TEMP3					= 0x00000080,
//};
//#define EPET_EQUIP_ITEM_ATTRIBUTE_MAX_COUNT  5 //NOew은 default로 1개 있음.
//#define ITEM_NAME_LEN 50
//#define ITEM_DESC_LEN 200
//
//struct GCItemData
//{
//    /************************************************************************/
//    /* 아이템의 기본적인 변수                                               */
//    /************************************************************************/
//    //-------------------------------------------------------------------------------------------------------
//
//    // 고유번호
//    DWORD		    dwGoodsID;
//    //아이템 이름
//    //std::string 	strItemName;
//    WCHAR            szItemName[ITEM_NAME_LEN];
//    //아이템 설명
//    //std::string 	strItemDesc;
//    WCHAR            szItemDesc[ITEM_DESC_LEN];
//    //아이템 종류 (다양하게 확장)
//    GCITEM_KIND 	eItemKind;
//
//    // 사용할 수 있는 게임 모드
//    // GCGAMEMODE 를 |로 연결하여 사용
//    DWORD		    dwApplyMode;
//
//
//    /************************************************************************/
//    /* 장착관련 정보                                                        */
//    /************************************************************************/
//    //-------------------------------------------------------------------------------------------------------
//    //착용 조건
//    //캐릭터 종류 : ECHARTYPE을 |로 연결해서 사용
//    DWORD		    dwCharType;
//    //최소 전직 단계	
//    int		        iNeedChangeJobLevelForEquippingItem;
//    //최소 레벨
//    int		        iNeedLevelForEquippingItem;
//    //Set번호  -1이면 세트에 포함되지 않음
//    int		        iSetItemNumber;	
//    //장착 슬롯 위치 ESLOTPOSITION을 |해서 사용
//    DWORD		    dwSlotPosition;
//    DWORD			dwSkillPosition;
//    //-------------------------------------------------------------------------------------------------------
//
//
//
//    /************************************************************************/
//    /*                    아이템의 능력치                                   //
//    /************************************************************************/
//    //-------------------------------------------------------------------------------------------------------
//
//    //기본 능력치
//    int		        iAttack;	
//    int		        iDefence;	
//    int		        iHealth;
//
//    //%능력치
//    int             iLimitedLevelOfLevelRatioAbility;
//    float	    	fLevelRatioAttackRate;
//    float	    	fLevelRatioDefenceRate;
//    float	    	fLevelRatioHealthRate;
//
//    //==========================================================
//    //회복능력(레나스 아이템 관련)
//    // iIncHPFrame frame동안 회복 능력을 fIncHPRate만큼의 성능을 보이게함
//    // fIncHPRate가 1.0보다 작으면 회복 능력이 줄어듦
//    int		        iIncHPFrame;
//    float		    fIncHPRate;
//
//    int		        iIncMPFrame;
//    float		    fIncMPRate;
//
//    //게임아이템에 미치는 영향력
//    // ex> iUseItem = GCGI_SLOW
//    //     fUseItemRate = 2.0이면 내가 쓴 슬로우 시간은 두배가됨 ^_^
//    //	   물론 줄어들게 할 수도 있음
//    GCGAMEITEM	    eUseGameItem;
//    float 	 	    fUseGameItemRate;
//
//    // ex>  iReceiveItem = GCGI_SLOW
//    //		fReceiveItemRate = 0.5이면 슬로우 당했을 때 늦어지는 시간이 반감됨 ^_^
//    GCGAMEITEM 	    eReceiveGameItem;
//    float 		    fReceiveGameItemRate;
//
//
//    //경험치 받는 정도
//    float		        fObtainExpRate;		// *2
//    // 카운터 공격 방어확률 (0 ~ 100)
//    short		        shCountDefencePercent;	
//
//    //========================================================
//    //기술 관련
//    // ex> 스페셜1 ---> 200번 기술로 변경..
//    DWORD		    dwSourceSkill; //변경 기술 위치 ex>스페셜1, 스페셜2, 흑마법1,
//    DWORD		    dwTargetSkill;	 // 200
//
//    //=========================================================
//    //특수능력	
//    // dwApplyMode의 영향을 받음...
//    // 특수한 능력, string으로 처리. (하드코딩될수밖에 없음)
//
//    //std::string	    strSpecialSkill;		//ex> "SelfResurrection"
//    WCHAR           szSpecialSkill[100];
//    //-------------------------------------------------------------------------------------------------------
//
//
//    /************************************************************************/
//    /* 아이템의 구매 or 판매 조건  및 아이템 구별                                                                     */
//    /************************************************************************/
//    //-------------------------------------------------------------------------------------------------------
//    //구매 or 판매 조건
//    //돈 종류 및 가격
//    EMONEYTYPE	    eMoneyType;
//    DWORD		    dwPrice;
//
//    //아이템 타입 ( 기간 or 수량)
//    //0이면 기간, 1이면 수량
//    GCITEM_TYPE     eItemType;
//    //한판에 한개씩 자동으로 깎이는지??
//    bool    		bAutoDecrease;
//    //eItemtype이 기간이고, iItemTypeValue 가 -1이면 영구(일반)아이템, 
//    //eItemtype이 수량이고, iItemTypeValue가 -1이면 오류..
//    int		        iItemTypeValue;
//
//    //속성
//    EITEMATTRIBUTE	eAttribute;
//    int		        iStrongLevel;		//강화 레벨
//
//    //패턴 
//    DWORD 		    dwPatternA;   // 패턴A 가격 (ver 1.1)
//    DWORD 		    dwPatternB;   // 패턴B 가격 (ver 1.1)
//
//    //New표시
//    bool            bNewItem;
//    // 추천 아이템 레벨 
//    int	            iRecomLevel;
//    //현재 살수 없는가??
//    bool		    bNotBuyItem;
//    //되팔기 불가능한가?
//    bool		    bNotSellItem;
//
//    // 특수 구매 조건
//    //필요한 Blessing수.. 현재 Blessing = 300 이면 자격증임
//    int		        iNeedBlessing;
//    int 		    iNeedItemAND[5];		// 3 And 10 And 20 을 가지고 있어야 구매 가능!!!
//    int 		    iNeedItemOR[5];		    // 3 OR 10 을 가지고 있으면 구매 가능!!!
//    int 		    iNeedGuildLevelForBuyItem;			// 길드에서 특정 자격이 있어야 구매가능
//    int 		    iNeedUserLevelForBuyItem;			// 특별 레벨 이상만 구매 가능!!! ex) 왕립
//
//    EPET_ITEM_KIND	ePetItemKind;
//    DWORD			dwPetEquipItemSlot;
//    EPET_EQUIP_ITEM_ATTRIBUTE  ePetEquipItemAttribute;
//    DWORD			dwPetAttribute;
//    int				iSupplyPetFood;
//    DWORD			dwPetGoodsIDOfOriginalPetEquipItemOfSkill;
//    short			shPetEquipItemPromotionLevel;
//    int				iBillingNumber;
//
//    //-------------------------------------------------------------------------------------------------------
//
//
//public:
//    GCItemData( void )
//    {
//        ::ZeroMemory( this, sizeof( GCItemData ) );
//        //memset( this, 0, sizeof(KGCItemInfo));
//        iSetItemNumber = -1;
//        iNeedGuildLevelForBuyItem    = -1;
//        iLimitedLevelOfLevelRatioAbility = -1;
//        iItemTypeValue	 = -1;
//        iNeedItemAND[0]  = -1;
//        iNeedItemAND[1]  = -1;
//        iNeedItemAND[2]  = -1;
//        iNeedItemAND[3]  = -1;
//        iNeedItemAND[4]  = -1;
//        iNeedItemOR[0]  = -1;
//        iNeedItemOR[1]  = -1;
//        iNeedItemOR[2]  = -1;
//        iNeedItemOR[3]  = -1;
//        iNeedItemOR[4]  = -1;
//        iBillingNumber	= -1;
//    }
//    GCItemData( const GCItemData& src )
//    {
//        ::memcpy( this, &src, sizeof( GCItemData ) );
//    }
//    ~GCItemData( void )
//    {
//        //empty
//    }
//    const GCItemData& operator=( const GCItemData& src )
//    {
//        ::memcpy( this, &src, sizeof( GCItemData ) );
//        return *this;
//    }
//};
//
//
//#pragma pack( pop )
