#ifndef _GCITEM_H_
#define _GCITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <utility>
#include "ProtectedType.h"

#pragma pack( push, 1 )

#include "GCItemAttribute.h"
#include <vector>

enum ECHARTYPE
{
    ECT_ELESIS  = 0x00000001,
    ECT_LIRE    = 0x00000002,
    ECT_ARME    = 0x00000004,
    ECT_LAS     = 0x00000008,
    ECT_RYAN    = 0x00000010,
    ECT_RONAN   = 0x00000020,
    ECT_AMY     = 0x00000040,
    ECT_JIN     = 0x00000080,
    ECT_SIEG    = 0x00000100,
    ECT_MARI    = 0x00000200,
    ECT_DIO     = 0x00000400,
    ECT_ZERO    = 0x00000800,
    ECT_LEY     = 0x00001000,
    ECT_LUPUS   = 0x00002000,
    ECT_RIN     = 0x00004000,
    ECT_ASIN    = 0x00008000,
    ECT_LIME    = 0x00010000,
    ECT_EDEL    = 0x00020000,
    ECT_BEIGAS  = 0x00040000,
    ECT_UNO     = 0x00080000,
    ECT_PERYTON = 0x00100000,
    ECT_ALICE 	= 0x00200000,
    ECT_DECANEE	= 0x00400000,
    ECT_CAELLUM = 0x00800000,
    ECT_LUMIEL  = 0x01000000,
    ECT_KIARA   = 0x02000000,
    ECT_IO      = 0x04000000,
    ECT_ALL     = 0xFFFFFF
};

enum EMONEYTYPE
{
	EMT_GAMEPOINT,
	EMT_CASH,
	EMT_CRYSTAL,
	EMT_GEM,
};

enum ESLOTPOSITION
{
    ESP_NONE			 = 0x00000000,
    ESP_SET_ITEM		 = 0x00000001,
    ESP_HELMET      	 = 0x00000002,
    ESP_JACKET			 = 0x00000004,
    ESP_PANTS			 = 0x00000008,
    ESP_GLOVES           = 0x00000010,
    ESP_SHOES			 = 0x00000020,
    ESP_MANTLE			 = 0x00000040,
    ESP_WEAPON	         = 0x00000080,
    ESP_A_UPPER_HELMET	 = 0x00000100,
    ESP_A_DOWN_HELMET	 = 0x00000200,
    ESP_A_NECKLET		 = 0x00000400,
    ESP_A_JACKET		 = 0x00000800,
    ESP_A_PANTS			 = 0x00001000,
    ESP_A_ARM			 = 0x00002000,
    ESP_A_FINGER         = 0x00004000,
    ESP_A_SHOES          = 0x00008000,
    ESP_PET				 = 0x00010000,
    ESP_SLOT_ITEM		 = 0x00020000,
    ESP_HELMET_SOCKET	 = 0x00040000,
    ESP_WEAPON_SOCKET	 = 0x00080000,
    ESP_JACKET_SOCKET    = 0x00100000,
    ESP_SLOT_EMOT		 = 0x00200000,
    ESP_A_HELMET_SOCKET  = 0x00400000,
    ESP_TITLE			 = 0x00800000,	    
    ESP_A_EARRING		 = 0x01000000,	    
    ESP_CRIT_FONT		 = 0x02000000,
    ESP_ROOM_ANIM		 = 0x04000000,
    ESP_CUTIN    		 = 0x08000000,
    ESP_UNIQUE_CHAR_ITEM = 0x10000000,
    ESP_ALL				 = 0xFFFFFFFF,
};

enum EITEMSLOTPOSITION
{
    EISP_SET_ITEM           =  0,
    EISP_HELMET      	    =  1,
    EISP_JACKET             =  2,
    EISP_PANTS              =  3,
    EISP_GLOVES             =  4,
    EISP_SHOES	            =  5,
    EISP_MANTLE             =  6,
    EISP_WEAPON	            =  7,
    EISP_A_UPPER_HELMET     =  8,
    EISP_A_DOWN_HELMET      =  9,
    EISP_A_NECKLET          = 10,
    EISP_A_JACKET           = 11,
    EISP_A_PANTS            = 12,
    EISP_A_ARM              = 13,
    EISP_A_FINGER           = 14,
    EISP_A_SHOES            = 15,
    EISP_PET				= 16,
    EISP_SLOT_ITEM			= 17,
    EISP_HELMET_SOCKET		= 18,
    EISP_WEAPON_SOCKET		= 19,
    EISP_JACKET_SOCKET      = 20,
    EISP_SLOT_EMOT			= 21,
    EISP_A_HELMET_SOCKET    = 22,
    EISP_TITLE              = 23,	    
    EISP_A_EARRING          = 24,	
    EISP_CRIT_FONT          = 25,
    EISP_ROOM_ANIM          = 26,
    EISP_CUTIN              = 27,
    EISP_UNIQUE_CHAR_ITEM   = 28,
    EISP_NUM,
};

enum COORDI_SLOT_POSION
{
    COORDI_PARTS_HELMET			=  0,
    COORDI_PARTS_JACKET         =  1,
    COORDI_PARTS_PANTS          =  2,
    COORDI_PARTS_GLOVES         =  3,
    COORDI_PARTS_SHOES	        =  4,
    COORDI_PARTS_MANTLE         =  5,
    COORDI_PARTS_A_UPPER_HELMET =  6,
    COORDI_PARTS_A_DOWN_HELMET  =  7,
    COORDI_PARTS_A_JACKET       =  8,
    COORDI_PARTS_A_PANTS        =  9,
    COORDI_PARTS_A_ARM          = 10,
    COORDI_PARTS_A_NECKLET      = 11,
    COORDI_PARTS_A_SHOES        = 12,
    COORDI_PARTS_TITLE          = 13,
    COORDI_PARTS_WEAPON         = 14,
    COORDI_PARTS_SIZE           = 15,
};


enum ESKILLPOSITION
{
	ESP_SKC_INVALID					= 0x00000000,
	ESP_SKC_WHITE_ONE               = 0x00000001,
	ESP_SKC_WHITE_TWO               = 0x00000002,
	ESP_SKC_WHITE_THREE             = 0x00000004,
	ESP_SKC_DARK_ONE                = 0x00000008,
	ESP_SKC_DARK_TWO                = 0x00000010,
	ESP_SKC_DARK_THREE              = 0x00000020,
	ESP_SKC_WHITE_CHANGEJOB_ONE     = 0x00000040,
	ESP_SKC_WHITE_CHANGEJOB_TWO     = 0x00000080,
	ESP_SKC_WHITE_CHANGEJOB_THREE   = 0x00000100,
	ESP_SKC_DARK_CHANGEJOB_ONE      = 0x00000200,
	ESP_SKC_DARK_CHANGEJOB_TWO      = 0x00000400,
	ESP_SKC_DARK_CHANGEJOB_THREE    = 0x00000800,
	ESP_SKC_WHITE_CHANGEJOB2_ONE    = 0x00001000,
	ESP_SKC_WHITE_CHANGEJOB2_TWO	= 0x00002000,
	ESP_SKC_WHITE_CHANGEJOB2_THREE	= 0x00004000,
	ESP_SKC_DARK_CHANGEJOB2_ONE		= 0x00008000,
	ESP_SKC_DARK_CHANGEJOB2_TWO		= 0x00010000,
	ESP_SKC_DARK_CHANGEJOB2_THREE	= 0x00020000,
};

#define ESP_SKILL_MAX	19
#define ESP_SLOT_MAX	22

enum EITEMATTRIBUTE
{
	EIATTR_NONE			         =  0,
	EIATTR_FIRE			         =  1,
	EIATTR_ICE			         =  2,
	EIATTR_LIGHTNING	         =  3,
	EIATTR_DARKNESS		         =  4,
	EIATTR_WIND			         =  5,
	EIATTR_GP_SHOES_SHIELD       =  6,
	EIATTR_CASH_SHOES_SHIELD     =  7,
    EIATTR_SHINING               =  8,
	EIATTR_LOVE					 =  9,
	EIATTR_ROSE					 = 10,
    EIATTR_DEATH                 = 11,
    EIATTR_TIME                  = 12,
    EIATTR_LIFE                  = 13,
	EIATTR_DARK_DRAGON_RED		 = 14,
	EIATTR_DARK_DRAGON_BLUE		 = 15,
	EIATTR_FOOLS_DAY			 = 16,
	EIATTR_ARIES_ANKLE           = 17,
	EIATTR_TAURUS_ANKLE          = 18,
	EIATTR_ZORRO_ANKLE           = 19,
    EIATTR_GEMINI_ANKLE          = 20,
    EIATTR_CANCER_ANKLE          = 21,
    EIATTR_LEO_ANKLE             = 22,
    EIATTR_VIRGO_ANKLE           = 23,
    EIATTR_LIBRA_ANKLE           = 24,
    EIATTR_SCORPIO_ANKLE         = 25,
    EIATTR_SAGITTARIUS_ANKLE     = 26,
    EIATTR_CAPRICORN_ANKLE       = 27,
    EIATTR_AQUARIUS_ANKLE        = 28,
    EIATTR_PISCES_ANKLE          = 29,
    EIATTR_HALLOWEEN             = 30,
    EIATTR_ELIA                  = 31,
    EIATTR_ELIA_ANKLE            = 32,
    EIATTR_ARIES                 = 33,
    EIATTR_TAURUS                = 34,
    EIATTR_GEMINI                = 35,
    EIATTR_CANCER                = 36,
    EIATTR_LEO                   = 37,
    EIATTR_VIRGO                 = 38,
    EIATTR_LIBRA                 = 39,
    EIATTR_SCORPIO               = 40,
    EIATTR_SAGITTARIUS           = 41,
    EIATTR_CAPRICORN             = 42,
    EIATTR_AQUARIUS              = 43,
    EIATTR_PISCES                = 44,
    EIATTR_10TH                  = 45,
    EIATTR_DIMENSION             = 46,
    EIATTR_PEGASUS_ANKLE         = 47,
    EIATTR_PEGASUS_NECK          = 48,
    EIATTR_AUTUMN_NECK           = 49,
    EIATTR_CELESTIAL_MASTER_NECK = 50,
    EIATTR_COLISEU_NECK          = 51,
    EIATTR_FAIRY_NECK            = 52,

};

enum GCENCHANT_GRADE
{
	GEG_OLD_BRAVE                   = 0,    
	GEG_OLD_HOLY                    = 1,    
	GEG_OLD_GLORY                   = 2,
	GEG_BRAVE                       = 3,    
	GEG_HOLY                        = 4,    
	GEG_GLORY                       = 5,
	GEG_NUM
};

enum GCGAMEMODE
{
	GCGM_TEAM		            = 0x00000001,
	GCGM_SURVIVAL	        	= 0x00000002,
	GCGM_GUILD_BATTLE           = 0x00000004,
	GCGM_CHANGEJOB_KNIGHT      	= 0x00000008,
	GCGM_CHANGEJOB_ARCHER      	= 0x00000010,
	GCGM_CHANGEJOB_MAGICIAN    	= 0x00000020,
	GCGM_MONSTER_CRUSADERS      = 0x00000040,
	GCGM_MONSTER_HUNT	        = 0x00000080,
    GCGM_QUEST_1		        = 0x00000100,
    GCGM_QUEST_2		        = 0x00000200,
    GCGM_QUEST_3		        = 0x00000400,
    GCGM_QUEST_4		        = 0x00000800,
    GCGM_QUEST_5		        = 0x00001000,
    GCGM_QUEST_6		        = 0x00002000,
    GCGM_QUEST_7				= 0x00004000,
    GCGM_QUEST_8			    = 0x00008000,
    GCGM_QUEST_9                = 0x00010000,
    GCGM_QUEST_10               = 0x00020000,
    GCGM_QUEST_11               = 0x00040000,
    GCGM_QUEST_12   	        = 0x00080000,
    GCGM_QUEST_13   	        = 0x00100000,
    GCGM_QUEST_14   	        = 0x00200000,
    GCGM_QUEST_15   	        = 0x00400000,
    GCGM_QUEST_16   	        = 0x00800000,
    GCGM_QUEST_17   	        = 0x01000000,
    GCGM_QUEST_18   	        = 0x02000000,
    GCGM_QUEST_19   	        = 0x04000000,
    GCGM_QUEST_20   	        = 0x08000000,
    GCGM_QUEST_21               = 0x10000000,
	GCGM_QUEST_22				= 0x20000000,
};

enum GCITEM_KIND
{
	GIK_NORMAL		            =  0,
	GIK_CERTIFICATE             =  1,
	GIK_MAGIC_PAPER		        =  2,
	GIK_STRONG_ITEM             =  3,
	GIK_STRONG_ITEM_NOT_STRONG  =  4,
	GIK_PRINT_UID               =  5,
	GIK_MISSION_SCROLL			=  6,
	GIK_PET						=  7,
    GIK_POINT_ITEM				=  8,
    GIK_ENCHANT_ASIST_ITEM      =  9,
    GIK_ENCHANT_ENABLE_ITEM     = 10,
    GIK_ENCHANT_CORE            = 11,
    GIK_RECIPE                  = 12,
    GIK_STUFF                   = 13,
    GIK_BOSS_RARE               = 14,
    GIK_EQUIP                   = 15,
    GIK_EQUIP_RARE              = 16,
    GIK_MISSION_STUFF           = 17,
    GIK_SHORTCUT_SLOT			= 18,
	GIK_TITLE_ITEM			    = 19,
	GIK_COUPLE_ITEM				= 20,
	GIK_OPEN_UP_ITEM			= 21,
	GIK_TREASUREBOX_ITEM		= 22,
    GIK_MONSTER_CARD            = 23,
    GIK_SELECT_LEVEL_ITEM       = 24,
    GIK_COORDI_ITEM             = 25,
    GIK_COORDI_PACKAGE7         = 26,
    GIK_COORDI_PACKAGE15        = 27, 
    GIK_COORDIBOX               = 28,
	GIK_AGIT_ITEM				= 29,
    GIK_ITEM_KIND_NUM,
};

enum GCGAMEITEM
{
	GCGI_NOTING = 0,
	GCGI_SLOW,
	GCGI_HP,
	GCGI_MP,
	GCGI_HIGHJUMP,
	GCGI_TRANSPARENCY,
	GCGI_SUPER,
	GCGI_REFLECTION,
};

enum USER_EQUIP_CATEGORY
{
	UEC_EQUIP1 = 0,
	UEC_EQUIP2,
	UEC_SHORTCUT_SLOT,
	UEC_PET,
	UEC_SKILL,
	UEC_SKILL_WHITE,
	UEC_SKILL_BLACK,
    UEC_LOOK,
	UEC_NUM,
};

enum USER_INVEN_CATEGORY
{
    UIC_EQUIP = 0,
    UIC_ACCESSORY,
	UIC_SHORTCUT_SLOT,
    UIC_PET,
    UIC_NUM,
};

enum GCITEM_TYPE
{
	GCIT_PERIOD = 0,
	GCIT_COUNT = 1,
};

struct GCItemAbility
{
	int iAtk;
	int iDef;
	int iVit;
    GCItemAbility() :iAtk( 0 ), iDef( 0 ), iVit( 0 )
    {}
};

enum EAbilityType
{
    ABILITY_ATK,
    ABILITY_DEF,
    ABILITY_HP,
    ABILITY_SPECIAL_ATK,
    ABILITY_SPECIAL_DEF,
    ABILITY_HP_DRAIN_RATE,
    ABILITY_MP_DRAIN_RATE,
    ABILITY_HP_RECOVERY_RATE,
    ABILITY_MP_RECOVERY_RATE,
    ABILITY_CRITICAL_ATTACK_RATE,
    ABILITY_COUNTER_DEF,
    ABILITY_ROLLING,
    ABILITY_FAST_HP_RECOVERY_DURING_LIMIT_TIME,
    ABILITY_IGNORE_HASTE,
    ABILITY_AUTO_RECOVERY_HP_LIMIT_IN_DUNGEON,
    ABILITY_ABNORMAL_CONDITION_RESISTANCE,
    ABILITY_ARENA_ABSENCE_TIME,
	ABILITY_DARK_DAMAGE_ATK_POINT,
    ABILITY_CRITICAL_DAMAGE_RESIST,
    ABILITY_CRITICAL_DAMAGE_UP,
    ABILITY_CATCH_DAMAGE_RESIST,
    ABILITY_RECOVERY_UP_PET_MP,
    ABILITY_MOVEMENT_SPEED_UP,
    ABILITY_RECOVERY_UP_POTION,
    ABILITY_FATAL_SUPER_ARMOR_BUFF,
    ABILITY_FATAL_RECOVERY_MP_UP_BUFF,
    ABILITY_FATAL_ATTACK_UP_BUFF,
    ABILITY_FATAL_SPEEP_UP_BUFF,
    ABILITY_FATAL_RECOVERY_POTION_UP,
    ABILITY_FATAL_CRITICAL_DAMAGE_UP_BUFF,
    ABILITY_FATAL_ENCHANT_PROBABILITY_UP,
    ABILITY_FATAL_ENCHANT_DESTORY_RESIST,
    ABILITY_ATTACK_PERCENT,
    ABILITY_DEFENCE_PERCENT,
    ABILITY_MISSILE_DAMAGE_RESIST,
    ABILITY_BUFF_MP_RECORVERY_RATE,
    ABILITY_ELYOS_RESIST_BUFF,
    ABILITY_MAX,
};

enum ESpecialAbility
{
	SPECIAL_ABILITY_RECOVERY_UP_PET_MP,
	SPECIAL_ABILITY_MAX = 5,
};

struct VIPAttribute
{
    int    m_iFontID;
    int    m_iAttribute;
    float  m_fAttributeValue;
};

struct CollectionAttribute
{
    int    m_iAttribute;
    float  m_fAttributeValue;
};

struct CollectionPromotion
{
    int                                 m_iPromotionLevel;
    std::vector< CollectionAttribute >  m_vecPromotionAttributes;
};

struct CollectionCharacter
{
    int                                                 m_iCharType;
    KSafeArray< CollectionPromotion, 4 > m_pkPromotionArray;
};

struct CollectionVIP
{
    int                 m_iFontID;
    CollectionAttribute m_pkCollectionAttribute;
};

struct EAbilityData
{
    float fEquipItemAbility[ABILITY_MAX];

    EAbilityData() 
    {
        for( int i = 0; i < ABILITY_MAX; ++i)
        {
            fEquipItemAbility[i] = 0.0f;
        }
    }

    EAbilityData( const EAbilityData& right_) 
    {
        *this = right_;
    }

    EAbilityData& operator =( const EAbilityData& right_)
    {
        for(int i = 0 ; i < ABILITY_MAX; ++i)
        {
            fEquipItemAbility[i] = right_.fEquipItemAbility[i];
        }

        return *this;
    }

    const EAbilityData& operator +=( const EAbilityData& right_)
    {
        for(int i = 0 ; i < ABILITY_MAX; ++i)
        {
            fEquipItemAbility[i] += right_.fEquipItemAbility[i];
        }

        return *this;
    }

	const EAbilityData& operator -=( const EAbilityData& right_)
	{
		for(int i = 0 ; i < ABILITY_MAX; ++i)
		{
			fEquipItemAbility[i] -= right_.fEquipItemAbility[i];
		}

		return *this;
	}
};

struct EItemAbilityData
{
    float fAtk, fDef, fHP;
    float fItemAttribute[ATTRTYPE_MAX];

    EItemAbilityData()
    {
        fAtk    = 0.0f;
        fDef    = 0.0f;
        fHP     = 0.0f;
        for( int i = 0 ; i < ATTRTYPE_MAX; ++i )
        {
            fItemAttribute[i] = 0.0f;
        }
    }

    EItemAbilityData& operator =( const EItemAbilityData& right_)
    {
        fAtk    = right_.fAtk;
        fDef    = right_.fDef;
        fHP     = right_.fHP;
        for(int i = 0 ; i < ATTRTYPE_MAX; ++i)
        {
            fItemAttribute[i] = right_.fItemAttribute[i];
        }

         return *this;
    }

    const EItemAbilityData& operator +=( const EItemAbilityData& right_)
    {
        fAtk    += right_.fAtk;
        fDef    += right_.fDef;
        fHP     += right_.fHP;
        for(int i = 0 ; i < ATTRTYPE_MAX; ++i)
        {
            fItemAttribute[i] += right_.fItemAttribute[i];
        }

        return *this;
    }
};

enum EPET_ITEM_KIND
{
	EPET_KIND_INVALID			= 0x00000000,
	EPET_KIND_PET				= 0x00000001,
	EPET_KIND_FOOD				= 0x00000002,
	EPET_KIND_EQUIP_ITEM		= 0x00000004,
};

enum EPET_EQUIP_ITEM_SLOT
{
	EPET_EQUIP_ITEM_SLOT_INVALID			= 0x00000000,
	EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM		= 0x00000001,
	EPET_EQUIP_ITEM_SLOT_SKILL				= 0x00000002,
};

#define EPET_EQUIP_SLOT_MAX_COUNT 3

enum EPET_EQUIP_ITEM_ATTRIBUTE
{
	EPET_ATTR_INVALID		        = 0x00000000,
	EPET_ATTR_LIGHT			        = 0x00000001,
	EPET_ATTR_DARK			        = 0x00000002,
	EPET_ATTR_PHYSICAL				= 0x00000004,
	EPET_ATTR_MAGIC					= 0x00000008,
	EPET_ATTR_ICE					= 0x00000010,
	EPET_ATTR_TEMP1					= 0x00000020,
	EPET_ATTR_TEMP2					= 0x00000040,
	EPET_ATTR_TEMP3					= 0x00000080,
};

#define EPET_EQUIP_ITEM_ATTRIBUTE_MAX_COUNT  5
#define ITEM_NAME_LEN 50
#define ITEM_DESC_LEN 200
#define SORT_MARK_MASK	0x00ffffff
#define NEW_MARK_MASK	0x08000000
#define HOT_MARK_MASK	0x04000000
#define FIT_MARK_MASK	0x02000000
#define SALE_MARK_MASK	0x01000000

struct GCItem;

struct GCItemData
{
    DWORD		    dwGoodsID;
    WCHAR            szItemName[ITEM_NAME_LEN];
    WCHAR            szItemDesc[ITEM_DESC_LEN];
    GCITEM_KIND 	eItemKind;
    DWORD		    dwApplyMode;
    DWORD		    dwCharType;
    int		        iNeedChangeJobLevelForEquippingItem;
    int		        iNeedLevelForEquippingItem;
    int		        iSetItemNumber;	
    DWORD		    dwSlotPosition;
    DWORD			dwSkillPosition;
    int		        iAttack;	
    int		        iDefence;	
    int		        iHealth;
    int             iLimitedLevelOfLevelRatioAbility;
    float	    	fLevelRatioAttackRate;
    float	    	fLevelRatioDefenceRate;
    float	    	fLevelRatioHealthRate;
    int		        iIncHPFrame;
    float		    fIncHPRate;
    int		        iIncMPFrame;
    float		    fIncMPRate;
    GCGAMEITEM	    eUseGameItem;
    float 	 	    fUseGameItemRate;
    GCGAMEITEM 	    eReceiveGameItem;
    float 		    fReceiveGameItemRate;
    float		    fObtainExpRate;
    short		    shCountDefencePercent;	
    DWORD		    dwSourceSkill;
    DWORD		    dwTargetSkill;
    WCHAR           szSpecialSkill[100];
    EMONEYTYPE	    eMoneyType;
    DWORD		    dwPrice;
    GCITEM_TYPE     eItemType;
    bool    		bAutoDecrease;
    int		        iItemTypeValue;
    EITEMATTRIBUTE	eAttribute;
    int		        iStrongLevel;
    DWORD 		    dwPatternA;
    DWORD 		    dwPatternB;
    bool            bNewItem;
    int	            iRecomLevel;
    bool		    bNotBuyItem;
    bool		    bNotSellItem;
    int		        iNeedBlessing;
    KSafeArray<int,5> iNeedItemAND;
    KSafeArray<int,5> iNeedItemOR;
    int 		    iNeedGuildLevelForBuyItem;
    int 		    iNeedUserLevelForBuyItem;
    EPET_ITEM_KIND	ePetItemKind;
    DWORD			dwPetEquipItemSlot;
    EPET_EQUIP_ITEM_ATTRIBUTE  ePetEquipItemAttribute;
    DWORD			dwPetAttribute;
    int				iSupplyPetFood;
    DWORD			dwPetGoodsIDOfOriginalPetEquipItemOfSkill;
    short			shPetEquipItemPromotionLevel;
    int				iBillingNumber;
    CHAR            cEnchantLevel;
    CHAR            cItemGrade;
	CHAR			cEnchantGrade;

public:
    GCItemData( void )
    {
        ::ZeroMemory( this, sizeof( GCItemData ) );
        iSetItemNumber = -1;
        iNeedGuildLevelForBuyItem    = -1;
        iLimitedLevelOfLevelRatioAbility = -1;
        iItemTypeValue	 = -1;
        iNeedItemAND[0]  = -1;
        iNeedItemAND[1]  = -1;
        iNeedItemAND[2]  = -1;
        iNeedItemAND[3]  = -1;
        iNeedItemAND[4]  = -1;
        iNeedItemOR[0]  = -1;
        iNeedItemOR[1]  = -1;
        iNeedItemOR[2]  = -1;
        iNeedItemOR[3]  = -1;
        iNeedItemOR[4]  = -1;
        iBillingNumber	= -1;
    }
    GCItemData( const GCItemData& src )
    {
        ::memcpy( this, &src, sizeof( GCItemData ) );
    }
    ~GCItemData( void )
    {
    }
    const GCItemData& operator=( const GCItemData& src )
    {
        ::memcpy( this, &src, sizeof( GCItemData ) );
        return *this;
    }    
};

#pragma pack( pop )

struct GCItem
{
    std::wstring     strItemName;
    std::wstring     strItemDesc;
	DWORD		    dwGoodsID;
	GCITEM_KIND 	eItemKind;
	DWORD		    dwCharType;
	int		        iNeedChangeJobLevelForEquippingItem;
	int		        iNeedLevelForEquippingItem;
	int		        iSetItemNumber;	
	DWORD		    dwSlotPosition;
	DWORD			dwSkillPosition;
	int		        iAttack;	
	int		        iDefence;	
	int		        iHealth;
	int             iLimitedLevelOfLevelRatioAbility;
	float	    	fLevelRatioAttackRate;
	float	    	fLevelRatioDefenceRate;
	float	    	fLevelRatioHealthRate;
	DWORD		    dwTargetSkill;
	EMONEYTYPE	    eMoneyType;
	DWORD		    dwPrice;
	GCITEM_TYPE     eItemType;
	int		        iItemTypeValue;
	EITEMATTRIBUTE	eAttribute;
	int		        iStrongLevel;
	EPET_ITEM_KIND	ePetItemKind;
	DWORD			dwPetEquipItemSlot;
	DWORD			dwPetGoodsIDOfOriginalPetEquipItemOfSkill;
    int             iCoordiShopPackage;
    int             iGrade;
    int             iAttribute;
    short           shSocket;
    short           shLevelDown;
    short           shContinentType;
    short			shPetEquipItemPromotionLevel;
    CHAR            cEnchantLevel;
	CHAR            cEnchantGrade;
    int            cItemGrade;
    bool    		bAutoDecrease;
    bool            bNewItem;
    bool		    bNotBuyItem;
    bool		    bNotSellItem;
    std::vector< KSocketInfo > m_vecSocketInfo;
    std::vector< KAttributeInfo > m_vecAttribute;
	int				iRecomLevel;

public:
    GCItem( void );
	GCItem( const GCItem& src );
	~GCItem( void );

	int	GetCharType();

    const GCItem& operator=( const GCItem& src );
	const GCItem& operator=( const GCItemData& src );
};

#endif