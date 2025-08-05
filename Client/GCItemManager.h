/// @file   GCItemManager.cpp
/// @author pch413@kogstudios.com
/// @since  2004-09-07 ���� 4:39:13

#ifndef _GCITEMMANAGER_H_
#define _GCITEMMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#ifdef __ORIGIN_DEV__
#define CONTINUE_NIL(p) if( p == NULL )\
                        {\
                            assert(p != NULL);\
                            continue;\
                        }
#else
#define CONTINUE_NIL(p) if( p == NULL )\
                            continue;
#endif

#ifdef __ORIGIN_DEV__
#define BREAK_NIL(p) if( p == NULL )\
                     {\
                        assert(p != NULL);\
                        break;\
                     }
#else
#define BREAK_NIL(p) if( p == NULL )\
                        break;
#endif

#ifdef __ORIGIN_DEV__
#define _RETURN_NIL(p, q) if( p == NULL )\
                          {\
                            assert(p != NULL);\
                            return q;\
                          }
#else
#define _RETURN_NIL(p, q) if( p == NULL )\
                            return q;
#endif

#define RETURN_NIL(p) _RETURN_NIL( p, (void)0 );

#include "GCUTIL/CRC/CRC_32.h"
#include "GCItemEnum.h"
#include "ProtectedType.h"
#include <atltime.h>
#include "KGCStringID.h"
#include "ItemContainer.h"
#include "GCAttributeDecider.h"
#include "GCGradeDecider.h"
#include "GCSocketDecider.h"
#include "GCCoupleSystem.h"
#include "GCMonsterCardManager.h"

struct SUserInfo;
struct SCharInfo;


enum COLLECTION_TITLE_ABILITY_TYPE
{
    CAT_HP = 0,
    CAT_STATECONFUSE,
    CAT_ARENA
};

// �ڵ� ������ Ÿ�� 
enum ECoordiShopItemType
{ 
    COORDI_SHOP_ORIGINAL    = 0,    // ������ ������
    COORDI_SHOP_7PACKAGE    = 1,    // 7�� ��Ű�� 
    NUM_COORDI_SHOP         = 2,
};

enum EM_HOT_ITEM_TYPE
{
    EM_HOT_ITEM_NONE_TYPE   = 0x0000,
    EM_HOT_ITEM_WEAPON_TYPE = 0x0001,
    EM_HOT_ITEM_ARMOR_TYPE  = 0x0002,
    EM_HOT_ITEM_BOX_TYPE    = 0x0004,
    EM_HOT_ITEM_ALL_TYPE    = EM_HOT_ITEM_WEAPON_TYPE + EM_HOT_ITEM_ARMOR_TYPE + EM_HOT_ITEM_BOX_TYPE,
};

struct KInventoryItem
{
    DWORD   m_dwUID;
    KProtectedType<int> m_iItemID;
    int     m_iPattern;
    KProtectedType<int> m_iDuration;	//���� �Ⱓ�� ��Ÿ��

    int     m_iItemType;				// �Ϲ�,����?? �Ⱓ��??
    time_t  m_tRegDate;
    time_t  m_tStartDate;
    time_t  m_tEndDate;
    int     m_nPeriod;					//ItemID�� �Բ� Ű�� �Ǵ� �κ��ε�..
    KProtectedType<CHAR> m_cEnchantLevel;

    std::vector< KSocketInfo > m_vecSocketInfo;
    std::vector< KAttributeInfo > m_vecAttribute;
    char m_cItemGrade;

    std::wstring GetRemainTime();

    KInventoryItem( void )
    {
        m_dwUID     = 0;	
        m_iItemID   = -1;
        m_iPattern  = 0;
        m_iDuration = -1;

        m_iItemType = 0;				// �Ϲ�,����?? �Ⱓ��??
        m_tRegDate  = 0;
        m_tStartDate = 0;
        m_tEndDate = 0;
        m_nPeriod = -1;
        m_cEnchantLevel = -1;
        m_cItemGrade = 0;
    }
    ~KInventoryItem( void )
    {
        // empty
    }
    KInventoryItem( const KInventoryItem& src )
    {
        *this = src;
    }
    const KInventoryItem& operator=( const KInventoryItem& src )
    {
        m_dwUID     = src.m_dwUID;
        m_iItemID   = src.m_iItemID;
        m_iPattern  = src.m_iPattern;
        m_iDuration = src.m_iDuration;
        m_iItemType	 = src.m_iItemType;
        m_tRegDate	 = src.m_tRegDate;
        m_tStartDate = src.m_tStartDate;
        m_tEndDate	 = src.m_tEndDate;
        m_nPeriod	 = src.m_nPeriod;
        m_cEnchantLevel = src.m_cEnchantLevel;
        m_vecSocketInfo = src.m_vecSocketInfo;
        m_vecAttribute = src.m_vecAttribute;
        m_cItemGrade = src.m_cItemGrade;

        return *this;
    }
};

// 2007.04.26 wony
// ����Ʈ ������ ������ �����۵�� �׿����� �ּ����� �������Դϴ�
// ���߿� sort ���Ͽ� ����Ʈ��� ȭ�󰳳��� ����ٸ� �׶��� �̳༮�� �ʿ����������..
struct PointItemList
{
    int         iGoodsID;
    int         iPointPrice;
    int         iItemTypeValue;

    PointItemList( void )
    {
        iGoodsID    = -1;
        iPointPrice = 0;
        iItemTypeValue   = 0;
    }

    ~PointItemList( void )
    {
        // empty
    }

    const PointItemList& operator=( const PointItemList& src )
    {
        iGoodsID    = src.iGoodsID;
        iPointPrice = src.iPointPrice;
        iItemTypeValue   = src.iItemTypeValue;
        return *this;
    }
};


// �ڵ� ������ ������ ������ ����ü 
struct KCoordiShopItem
{
    GCItem* pOriginal;  // ���� ������ 
    GCItem* pPackage7;  // 7�� ��Ű�� ������

    KCoordiShopItem( void )
    {
        pOriginal = NULL;
        pPackage7 = NULL;
    }

    KCoordiShopItem( GCItem* pOriginal, GCItem* pPackage7 )
    {
        this->pOriginal = pOriginal;
        this->pPackage7 = pPackage7;
    }

    void InsertItem( GCItem* pItem )
    {
        if( pItem->eItemKind == GIK_COORDI_ITEM )
        {
            if( pOriginal == NULL )
                pOriginal = pItem;
            else
            {
                if( pOriginal->dwPrice < pItem->dwPrice )
                {
                    pPackage7 = pOriginal;
                    pOriginal = pItem;
                }
                else
                    pPackage7 = pItem;
            }
        }
    }
};

struct kRemovePartsCoordiItem
{
    int                      iRemoveParts;
    std::vector<GCITEMID>    vecItemID;

    void Init()
    {
        iRemoveParts = -1;
        vecItemID.clear();
    }
};

struct SCoordiUpgradeInfo
{
    GCITEMID                m_GradeUpgradeItemID; // ��� ���׷��̵� ������.
    std::map<char,int>      m_mapGradeUseCount;   // ��� ���׷��̵�� ������ ��� ����
    int                     m_nEnableItemType;    // ��� ItemType.
    std::set<GCITEMID>      m_setUpgradeBanItem;  // ���׷��̵� �Ұ� ������.
    bool m_bGetInfo;

    SCoordiUpgradeInfo()
    {
        m_bGetInfo = false;
        m_GradeUpgradeItemID = 0;
        m_mapGradeUseCount.clear();
        m_nEnableItemType = 1;  // �ڵ�
        m_setUpgradeBanItem.clear();
    }
};



class PLAYER;
struct TagCharacterInfo;

typedef std::unordered_map<DWORD,DWORD> map_same_image_index_type_type;
typedef std::map<int,std::map<DWORD,DWORD>> MAP_CHARID_MAP_RECOM;

typedef std::vector<KInvenBuffItem>		VEC_INVEN_BUFF;
typedef VEC_INVEN_BUFF::iterator		VEC_INVEN_BUFF_ITER;
typedef std::map<GCITEMID, KInvenBuffItem>	MAP_INVEN_BUFF;
typedef MAP_INVEN_BUFF::iterator			MAP_INVEN_BUFF_ITER;
typedef std::map<int,VEC_INVEN_BUFF>		MAP_INVEN_BUFF_GROUP;
typedef MAP_INVEN_BUFF_GROUP::iterator		MAP_INVEN_BUFF_GROUP_ITER;


class KGCItemManager
{
public:
    KGCItemManager( void );
    ~KGCItemManager( void );

public:
    // �� �������� ��� Ÿ��
    enum EShopItemType
    {
        SIT_ARMOR,
        SIT_SET,
        SIT_WEAPON,
        SIT_ACCESSORY,
        SIT_QUEST,
        SIT_PET,
#if defined( COUPLE_SYSTEM )
        SIT_COUPLE_ITEM,
#else
        SIT_PETITEM,
#endif
        SIT_POINTITEM,
        SIT_ACCESSORY2,		//����Ʈ ���� ���� ������ �ִ� ��ư
        NUM_SHOP_ITEM_TYPE,
    };

    enum EShopItemSort
    {
        SIS_DEFAULT = -1,
        SIS_NEW,
        SIS_RECOM,
        SIS_HIGH_LV,
        SIS_LOW_LV,
        SIS_NAME,
        SIS_KIND,
    };

    // �������� ĳ���� Ÿ��
    enum EItemCharType
    {
        ICT_KNIGHT, // ���
        ICT_ARCHER, // �ü�
        ICT_WIZARD, // ����
        ICT_THIEF,  // ����
    };

    enum ECertItemID
    {
        ITEM_CERT_BROKEN_DURATION   = 7392, // ������ �ڰ��� ������
    };

    enum
    {
        ITEM_CRYSTAL_DISCOUNT_CARD          = 1251,
        ITEM_CHAR_LAS_CARD		            = 1750,
        ITEM_CHANGE_NICK_NAME               = 2372,
        ITEM_CHAR_LAS_CARD_CASH             = 2380,
        ITEM_CHAR_SWORDMASTER_CARD          = 2587,

        ITEM_ROOM_NAME_COLORING_GP          = 2667,
        ITEM_ROOM_NAME_COLORING_CASH        = 2668,

        ITEM_CHANGE_PET_NICKNAME_CARD       = 3897, // �� �г��� ���� ī��
        ITEM_PETBOX                         = 3898, // ����λ���
        ITEM_EVENT_PETBOX                   = 3899, // ����λ���
        ITEM_EXTEND_INVENTORY_ITEM_60       = 4222, // +60 �κ��丮 �ø��� ������
        ITEM_EXTEND_INVENTORY_ITEM_30       = 4350, // +30 �κ��丮 �ø��� ������
        ITEM_EXTEND_INVENTORY_ITEM_10       = 5978, // +10 �κ��丮 �ø��� ������
        ITEM_EXTEND_INVENTORY_ITEM_10_2     = 7682, // +10 �κ��丮 �ø��� ������ ���ǰ��� �γ� �̹����� �����־ ������ -��-;
        ITEM_EXTEND_INVENTORY_ITEM_10_3     = 9877, // +10 �κ��丮 �ø��� ������ �̰� ���̹� �� �����ֵ�...
        ITEM_EXTEND_INVENTORY_ITEM_5        = 16129,
        
        ITEM_PET_EVOLUTION_ITEM_CASH5000    = 4237, // ��ȭ������ - ĳ�� 5000 ( ����������� ��ȭ )
        ITEM_PET_EVOLUTION_ITEM_CASH3000    = 4348, // ��ȭ������ - ĳ�� 3000 ( ���� 20�� ��ȭ )
        ITEM_PET_EVOLUTION_ITEM_POINT       = 4239, // ��ȭ������ - ����Ʈ
        ITEM_PET_EVOLUTION_ITEM_GP          = 4349, // ��ȭ������ - GP ( ���� 40�� ��ȭ )

        ITEM_PET_DEVOLUTION_ITEM_POINT      = 4240, // ��ȭ������ - ����Ʈ
        ITEM_PET_DEVOLUTION_ITEM_GP         = 4238, // ��ȭ������ - GP

        ITEM_PET_HATCH_ITEM_GP              = 4355, // ��ȭ������ - GP

        ITEM_RESET_WIN_LOSE_DATA            = 4798, // �·� �ʱ�ȭ ������

        ITEM_PET_PACKEGE1                   = 4811, // ����Ű�� ������
        ITEM_PET_PACKEGE2                   = 4812, // ����Ű�� ������
        ITEM_PET_PACKEGE3                   = 4813, // ����Ű�� ������

        ITEM_BONUS_CHARGING_ITEM_POINT      = 7020, // ���ʽ� ���� ������ �⼮ ����Ʈ
        ITEM_BONUS_CHARGING_ITEM_CASH       = 7144, // ���ʽ� ���� ������ ĳ��

        ITEM_CHAR_RYAN_CARD_CASH            = 5312, // ���̾� ĳ���� ī��
        ITEM_CHAR_RONAN_CARD_CASH           = 5520, // �γ� ĳ���� ī��
        ITEM_CHAR_SIEG_CARD_CASH            = 44110,    //  ��ũ��Ʈ ĳ���� ī��
        ITEM_HELLMODE_TICKET                = 8205,  // ���� �����
        
        ITEM_FREEPATH_TICKET		   = 6262,

        ITEM_GC_CLUB                        = 4234, // GCŬ��

#if defined(NATION_USA)
        ITEM_GC_BLOG                        = 2202, // GC����
#else
        ITEM_GC_BLOG                        = 32808, // GC����
#endif

        ITEM_EASTER_EVENT_TREASURE_BOX		= 7851, // ��Ȱ�� �̺�Ʈ ���� ����
        ITEM_FORTUNE_CAPSULE_ELA            = 9692, // ��� ĸ��( �����ý�, ����, �Ƹ��� )
        ITEM_FULL_MOON_RING					= 10157, // ������ �� ����
        
        ITEM_CASH_CLEAR_SPELL_ITEM			= 10203,// ���� ���� ������ (�ѱ���í��)
        ITEM_DIAMOND_CLEAR_SPELL_ITEM		= 167628,// ���� ���� ������ (�ѱ���í��)

        ITEM_QUEENS_BOX						= 9623, // ������ ����
        ITEM_GACHA_VIP                      = 9991, // ��í �ս� ����
        ITEM_PH_CBT_BOX						= 10795, // �ʸ��� ����Ƽ ����
        ITEM_WINTER_VACATION_BOMB_BOX1		= 11920, // �ܿ� ���� ��ź ����1
        ITEM_WINTER_VACATION_BOMB_BOX2		= 11921, // �ܿ� ���� ��ź ����2
        ITEM_WENDY_SECRET_BOX				= 12410, // ������ ��� ����
        ITEM_TREASURE_MINGMING_BOX			= 8544,
        ITEM_SEASON2_CONGRATULATION_BOX		= 8970,
        ITEM_FORTUNE_CAPSULE_LRR            = 9693,
        ITEM_SEVEN_CIRCLE_TREASURE_BOX      = 9837,
        ITEM_TREASURE_ITEM_BOX				= 7684,
        ITEM_CHAR_JIN_CARD_CASH             = 13050,

        ITEM_SEAL_BEAD_LEVEL1_JIN			= 13124, //���ε� ���� ������ ���� 
        ITEM_SEAL_BEAD_LEVEL2_JIN			= 13125, //���ε� ��� ������ ���� 
        ITEM_SEAL_BEAD_LEVEL3_JIN			= 13126, //���ε� �ְ�� ������ ����

        ITEM_NEW_YEAR_MONEY_1000GP			= 13367, //��� 1000GP
        ITEM_NEW_YEAR_MONEY_5000GP			= 13368, //��� 5000GP
        ITEM_NEW_YEAR_MONEY_10000GP			= 13369, //��� 10000GP
        ITEM_NEW_YEAR_MONEY_50000GP			= 13370, //��� 50000GP
        ITEM_5000_GP_COUPON                 = 15898, //5000GP����
        ITEM_1000000_GP_COUPON              = 37837, //100��GP����
        ITEM_1000000_GP_COUPON2              = 102573, //100��GP����
        ITEM_100_GP_COUPON                  = 102569, //100GP����
        ITEM_1000_GP_COUPON2                = 102570, //1000GP����
        ITEM_10000_GP_COUPON2               = 102571, //1��GP����
        ITEM_100000_GP_COUPON               = 102572, //10��GP����

        ITEM_EASTER_EVENT_BROKEN_EGG1           = 14985,    // ���� ��Ȱ�� �ް�1
        ITEM_EASTER_EVENT_BROKEN_EGG2           = 14990,    // ���� ��Ȱ�� �ް�2
        ITEM_EASTER_EVENT_BROKEN_EGG3           = 14991,    // ���� ��Ȱ�� �ް�3
        ITEM_EASTER_EVENT_BROKEN_EGG4           = 14992,    // ���� ��Ȱ�� �ް�4
        ITEM_500_GP_COUPON					= 11573, // 500 GP ����
        ITEM_10000_GP_COUPON				= 10095, // 1��GP ����
        ITEM_20000_GP_COUPON				= 743, // 20000 GP ����
        ITEM_1000_GP_COUPON				        = 15140, // 1000GP ����
        ITEM_2000_GP_COUPON				        = 15141, // 2000GP ����

        ITEM_VIRTUAL_CASH_COUPON0               = 13119, // ���� ĳ�� ����
        ITEM_VIRTUAL_CASH_COUPON1               = 13120, // ���� ĳ�� ����
        ITEM_VIRTUAL_CASH_COUPON2               = 17104, //���� GASH ����(20 GASH)
        ITEM_VIRTUAL_CASH_COUPON3               = 17578, //���� GASH ����(10 GASH)
        ITEM_VIRTUAL_CASH_COUPON4               = 20393, //���� GASH ����(80 GASH)

        ITEM_RING_OF_BEST                       = 17166, // ����Ʈ ����
        ITEM_RING_OF_FRIEND                     = 17167, // ģ�� ����

        ITEM_ARIES									= 18831,//���ڸ� ���κ���
        ITEM_TAURUS								= 18832,//Ȳ���ڸ� ���κ���
        ITEM_GEMINI								= 18833,//�ֵ����ڸ� ���κ���
        ITEM_CANCER								= 18834,//���ڸ� ���κ���
        ITEM_LEO										= 18835,//�����ڸ� ���κ���
        ITEM_VIRGO									= 18836,//ó���ڸ� ���κ���
        ITEM_LIBRA									= 18837,//õĪ�ڸ� ���κ���
        ITEM_SCORPIO								= 18838,//�����ڸ� ���κ���
        ITEM_SAGITTARIUS							= 18839,//����ڸ� ���κ���
        ITEM_CAPRICORN							= 18840,//�����ڸ� ���κ���
        ITEM_AQUARIUS							= 18841,//�����ڸ� ���κ���
        ITEM_PISCES									= 18842,//������ڸ� ���κ���

        ITEM_HALLOWEEN_BOARD				= 32942,
        ITEM_HALLOWEEN_DICE					= 32943,	// �������� 0 �ٿ��� �޶�ż�..

        ITEM_BINGO_BOARD                    = 20676,
        ITEM_BINGO_COIN                     = 20677,
        ITEM_BINGO_BONUS                    = 20678,


#ifdef NATION_KOREA
        ITEM_GAWIBAWIBO_PACKEGE				= 59320,	// ���� ������ 
        ITEM_GAWIBAWIBO_BOARD				= 59322,	// ���� ������ 
        ITEM_GAWIBAWIBO_COIN				= 59318,	// ���� ������ 
        ITEM_GAWIBAWIBO_RETRY				= 59319,	// ���� ������ 
#elif defined (NATION_BRAZIL)
        ITEM_GAWIBAWIBO_PACKEGE				= 43658,	// ���� ������ 
        ITEM_GAWIBAWIBO_BOARD				= 41342,	// ���� ������ 
        ITEM_GAWIBAWIBO_COIN				= 41343,	// ���� ������ 
        ITEM_GAWIBAWIBO_RETRY				= 41344,	// ���� ������ 
#else
        ITEM_GAWIBAWIBO_PACKEGE				= 41345,	// ���� ������ 
        ITEM_GAWIBAWIBO_BOARD				= 41342,	// ���� ������ 
        ITEM_GAWIBAWIBO_COIN				= 41343,	// ���� ������ 
        ITEM_GAWIBAWIBO_RETRY				= 41344,	// ���� ������ 
#endif
        
        ITEM_GWC_GLOVE						= 21883,	//	�������弼Ʈ �Ծ������ �տ� �պ�

        ITEM_PUZZLE_PLATE   				= 30174,

        ITEM_GACHA_COIN						= 14299,
        ITEM_STYLISH_COUPON                 = 35998,
        ITEM_PROMOTION_MAGIC_1              = 36668,    //1������������
        ITEM_PROMOTION_MAGIC_2              = 56494,    //2������������
        ITEM_PROMOTION_MAGIC_3              = 56495,    //3������������

        ITEM_PLANTTREE_BOARD = 54189,//		-- �����ɱ��� ItemID
        ITEM_PLANTTREE_GOLDTREE = 54191,//  		-- Ȳ�� ���� ItemID
        ITEM_PLANTTREE_WATER = 54192,//     		-- ���ֱ� ItemID
        ITEM_PLANTTREE_MANURE = 54193,// 		-- �Ÿ��ֱ� ItemID
        ITEM_RESET_SKILL                    = 54905,  // ��ų ��ü ȯ��ī��

        ITEM_GP_COUPON_S4_1                 = 51831,
        ITEM_GP_COUPON_S4_2                 = 51832,
        ITEM_MAGICBOX_GAME_BOARD            = 59281, //�ڿ쳪Ʈ �������� ������ 

        ITEM_SPHINX_BOARD                   = 59981,
        ITEM_SPHINX_COIN                   = 59686,
         
        ITEM_AGIT_FLOWER_POT_SEED           = 60526,    // ���� 
        ITEM_AGIT_MONEY_STAR                = 60527,    // ������ 
        ITEM_AGIT_FLOWER_POT_NORMAL         = 60528,    // ����Ʈ ȭ�� �Ϲ� 
        ITEM_AGIT_FLOWER_POT_CASH           = 60529,    // ����Ʈ ȭ�� ĳ�� 
        ITEM_AGIT_TRAINING                  = 60530,    // ����Ʈ �Ʒü� 
        ITEM_AGIT_GUEST_BOOK                = 60531,    // ����Ʈ ������ 
        ITEM_AGIT_TRAINING_TILE             = 60532,    // ����Ʈ �Ʒü� �����丷 
        ITEM_AGIT_EVENT_OBJECT_PACKAGE      = 65783,    // ����Ʈ �߼� ������Ʈ ��Ű�� 
        ITEM_ENCHANT_STONE                  = 62727,    // ��ȭ��
        ITEM_SONGKRAN_WATER_BOMB			= 87610,	// ����ź ��Ű��
		
        ITEM_SUMMON_ARME_RING               = 99265,    // �Ƹ��� NPC ��ȯ���� 

        ITEM_NIGHTMARKET                    = 101833,    // �߸���
        ITEM_NIGHTMARKET_LIGHT              = 101834,    // ������ �߸���

        ITEM_OPEN_CHANGE_SKILL_SLOT         = 999999,   // ��ų���� ������ �ѹ� ������ ���⿡ �����ϸ� ��.

        ITEM_RITAS_CHRISTMAS_CONTINUE_COIN_EXCHANGE_ITEMID  = 104673,  // ��Ÿ�� ũ�������� �̺�Ʈ�� �絵�� ���� ���� ������

        ITEM_RED_SPEAR_MONSTER_CARD         = 111570,  // ���� �� ���� ���Ǿ� �ߵ��ϴ� ���� ī�� 

        ITEM_VP1_COUPON         = 112908,	    // 1 VP ����
        ITEM_VP10_COUPON        = 112909,       // 10 VP ����
        ITEM_VP100_COUPON       = 112910,	    // 100 VP ����
        ITEM_VP500_COUPON      = 127738,	    // 500 VP ����
        ITEM_VP1000_COUPON      = 112911,	    // 1000 VP ����
        ITEM_VP10000_COUPON     = 112912,	    // 10000 VP ����
        ITEM_VP1000000_COUPON     = 123739,	    // 1000000 VP ����
        ITEM_WATER_FESTIVAL_VP1000 = 123751,    // ���� ���� 1000VP���� 

        ITEM_EXTEND_INVENTORY_EVENT_ITEM_10       = 115213, // �̹�Ʈ +10 ����
        ITEM_EXTEND_INVENTORY_EVENT_ITEM_60       = 115214, // �̺�Ʈ +60 ����

        ITEM_ANGRYBOSS_TICKET               = 74606,

        ITEM_LOOK_INVEN_EXTEND_100  = 149584, // �ڵ� �κ� Ȯ�� ������
		ITEM_LOOK_INVEN_EXTEND_50  = 149583, // �ڵ� �κ� Ȯ�� ������
		ITEM_LOOK_INVEN_EXTEND_10  = 149582, // �ڵ� �κ� Ȯ�� ������

        ITEM_ADD_SLOT_ITEM                  = 121396, // ĳ���� ���� Ȯ�� ������

        ITEM_COORDI_COMPOSE_COUPON  = 129020,

		ITEM_GC_PREMIUM = 152230,
    };

    enum
    {
        MAXIMUM_PCBANG_UID  = 10000,   //050711. microcat.
        EMAX_HERO_EARRING_SLOT = 2, //����� �������� ���� �� �ִ� ��ű� �������� �ִ� ���� ���� ����
    };

    enum EMONEYTYPE_SERVER
    {
        EMTS_INVALID =0,
        EMTS_CASH,
        EMTS_GAMEPOINT,
        EMTS_VIRTUALCASH,
        EMTS_CRYSTAL,
        EMTS_GCPOINT,
        EMTS_GEM,
    };

    enum
    {
        EMCOORDI_COMPOSE_OK = 0,
        EMCOORDI_COMPOSE_ERR0,
        EMCOORDI_COMPOSE_ERR1,
        EMCOORDI_COMPOSE_ERR2,
        EMCOORDI_COMPOSE_ERR3,
        EMCOORDI_COMPOSE_ERR4,
        EMCOORDI_COMPOSE_ERR5,
        EMCOORDI_COMPOSE_ERR6,
        EMCOORDI_COMPOSE_ERR7,
        EMCOORDI_COMPOSE_ERR8,
    };

    enum
    {
        EMCOORDI_UPGRADE_OK = 0,
        EMCOORDI_UPGRADE_ERR0,  // ������ ������ �ùٸ��� ����.
        EMCOORDI_UPGRADE_ERR1,  // �ڵ� �������� �ƴմϴ�.
        EMCOORDI_UPGRADE_ERR2,  // �ɷ�ġ�� ���� �ڵ��Դϴ�.
        EMCOORDI_UPGRADE_ERR3,  // �������� �ڵ� �Դϴ�.
        EMCOORDI_UPGRADE_ERR4,  // ������ �������Դϴ�.
        EMCOORDI_UPGRADE_ERR5,  // ��� ���׷��̵尡 �Ұ����� �������Դϴ�.
        EMCOORDI_UPGRADE_ERR6,  // ���׷��̵� ������ �����մϴ�.
        EMCOORDI_UPGRADE_ERR7,
        EMCOORDI_UPGRADE_ERR8,
    };

    struct SetItemAttribute
    {
        EAttributeType  eAttributeType;
        float           fAbility;
    };

    struct SetItemAttributeList
    {
        int iEquipCount;
        std::vector< SetItemAttribute > vecSetItemAttribute;
    };

    struct SetTemplet
    {
        int			                iSetNum;        
        std::vector<int>			vecParts;
        std::vector<int>			vecAccessorys;
        std::vector<int>            vecWeaponNum;
        std::vector<int>			vecItemEffEnum;
        std::vector<int>            vecBuffNum;
        bool                        bGachaSetItem;
        bool						m_bSquareMotion;
        std::vector< SetItemAttributeList > vecSetItemAtrributeList;
        bool						m_bCheerUpMotion;
    };

    struct CoordiSeasonTemplet
    {
        int                         iSeasonNum;
        int                         iSeasonText;
        int                         iSeasonGrade;
        std::set<int>               setPartsList;      //   ����Ǵ� ���� ����Ʈ
        std::set<int>               setItemList;       //   ��ü ������ ����Ʈ
        std::vector< SetItemAttributeList > vecSetItemAtrributeList;

        CoordiSeasonTemplet()
        {
            iSeasonGrade = -1;
            iSeasonNum = UINT_MAX;
            iSeasonText = 0;
            setPartsList.clear();
            setItemList.clear();
            vecSetItemAtrributeList.clear();
        }
    };

    struct SpecialAbility //zstaiji - ������ Ư���ɷ� ��ũ��Ʈ�� ����
    {
        int			                iItemNum;   //�������� ��ȣ     
        int							iTotalNum;	//Ư���ɷ� �� ����
        std::vector<int>			vecAbilityNum; //������ ��ȣ�� ��Ī �� �ɷ��� ��ȣ
    };

    struct KHeroItemSpecialAbility
    {
        KHeroItemSpecialAbility()
        {
            dwGoodsID = -1;
            iBuffIndex = -1;
            fBuffTime = 0.f;
            nPriority = 0;
            
            vecConditions.clear();
        }

        DWORD               dwGoodsID;          //������ID
        int                 iBuffIndex;         //�������� ������ �ִ� ���� �ε���
        float               fBuffTime;
        int                 nPriority;

        vector<float>       vecConditions;      //��� ���ǵ�... ������ Ʋ�� ���� �ʿ��� �������� �������� ��Ƶα����� �����̳�
    };

    KGCAttributeDecider m_kAttributeDecider;
    KGCGradeDecider     m_kGradeDecider;
    KGCSocketDecider    m_kSocketDecider;
public: // ������ ������ �����´�.   
    
    GCItem* GetItemData( GCITEMID dwItemID );

    void LoadItemData();
    void BindToLua();

    inline DWORD GetNumItem( void )
    {
        return m_dwNumItem;
    }

public: //�κ��丮 �������� ������ �����´�.
    
    inline int GetNumInventory() 
    {
        return (int)m_vecInventory.size(); 
    }
    inline std::vector<GCITEMUID> GetInventoryList() { return m_vecInventory; }

    inline int GetNumInventoryWithoutTitle() {
        int iTitleCnt = 0;
        for(int i=0;i< GetNumInventory();++i)
        {
            GCItem *pItem = GetItemData( GetInventoryItem(i)->m_ItemID );

            if( pItem == NULL ||
                GIK_TITLE_ITEM == pItem->eItemKind || GIK_MONSTER_CARD == pItem->eItemKind ||
                SiKGCCoupleSystem()->IsInCoupleInven( GetInventoryItem(i)->m_ItemUID ) )
            {
                ++iTitleCnt;
            }
        }
        return GetNumInventory() - iTitleCnt;
    }

    void FilterRemoveOtherChar(IN OUT std::vector<GCITEMUID>& vecItemList_, int iCharType);
    KItem* GetInventoryItemIndex( int iIndexList );
    KItem* GetInventoryItem( GCITEMUID dwUID );
    KItem* GetInventoryItemFromID( GCITEMID dwItemID );
    std::vector<KItem> GetInventoryItemList( 
                        GCITEM_KIND eItemKind
                        , int eMoneyType = -1
                        , DWORD eItemPosition = ESP_SET_ITEM|ESP_HELMET|ESP_JACKET|ESP_PANTS|ESP_GLOVES|ESP_SHOES|
                                                ESP_MANTLE|ESP_WEAPON|ESP_A_UPPER_HELMET|ESP_A_DOWN_HELMET|ESP_A_NECKLET|      
                                                ESP_A_JACKET|ESP_A_PANTS|ESP_A_ARM|ESP_A_FINGER|ESP_A_SHOES|ESP_PET|	
                                                ESP_SLOT_ITEM|ESP_HELMET_SOCKET|ESP_WEAPON_SOCKET|ESP_JACKET_SOCKET|
                                                ESP_SLOT_EMOT|ESP_A_HELMET_SOCKET|ESP_A_EARRING
                                                , DWORD eCharType = ECT_ALL );
    std::vector<GCITEMUID> GetInventoryItemUIDList( DWORD dwCharType_ , DWORD dwItemPosition_, DWORD dwSkillPosition_ );
    std::vector<GCITEMUID> GetInventoryItemUIDAllList( DWORD dwItemPosition_, DWORD dwSkillPosition_ );
    std::vector<GCITEMUID> GetInventoryCoordiItemUIDList( DWORD dwCharType_ = ECT_ALL, DWORD dwItemPosition_ = 0 );
    std::vector<GCITEMUID> GetInventoryItemUIDListByKind( GCITEM_KIND eKind_ );

    /*�ڵ� �����ۿ� �� �� ���� PC�� ������ ���� ���� �ɷ� ���ϴ�.*/
    void FilterBadCoordiItemList( IN OUT std::vector<GCITEMUID>& vecItemList );
    void FilterRemoveCoordiItem( IN OUT std::vector<GCITEMUID>& vecItemList_ );
    void FilterRemainStuffOnly( IN OUT std::vector<GCITEMUID>& vecItemList_ );
    void FilterEquipedItem( const SCharInfo* pCharInfo_, std::vector<GCITEMUID>& vecItemList_, bool bCoordi_ = false );
    void FilterBadPCBANGItemList( IN OUT std::vector<GCITEMUID>& vecItemList );

    void RemoveInventoryItemList(GCITEMUID dwUID);

    // ���� ĳ���͸� ������ �ڵ� ������ ����Ʈ - ������ > �̻������� �ǿ��� ���� ĳ���� ������ �ڵ� ������ �����ֱ� ����
    void FilterUnuseCoordiItemList( IN OUT std::vector< GCITEMUID >& vecItemList_ );

private:
    std::set<GCITEMID> m_setNonCountItemList;
    std::set<GCITEMID> m_setNonVisibleItemList;

public: // �κ��丮 �������� ������ �����´�.
    void InitNonCountItemSize();
    void Server_SetNonCountItem( std::set<DWORD>& setItemList );
    void Server_SetNonVisibleItem( std::set<DWORD>& setItemList );
    bool IsNonCountItem( DWORD dwItemID );
    bool IsNonVisibleItem( DWORD dwItemID );
    inline int GetTotalInventorySize( void )
    {
        return m_kInventory.GetSize( m_setNonCountItemList )+1;
    }
    inline int GetInvenSize( void )
    {
        return m_kInventory.GetInvenItemSize( m_setNonCountItemList );
    }
    inline int GetCoordiInventorySize( void )
    {
        return (int)GetInventoryCoordiItemUIDList().size();
    }

    void LogInventory()
    {
        return m_kInventory.LogInventory( m_setNonCountItemList );
    }

    bool CheckExtraInventoryExist();
    bool CheckExtraCoordiExist();
    bool CheckItemCountLimit();
    int GetVolumeInventoryItem( int iItemID );
    

public:
    bool CheckItemContinentType( GCItem* gcItem, int  eCurrentMapType );
    void MakeShopItemList( EGCCharType CharType, int ItemType, int iCategory, int iSortType, int iMoneyType, const WCHAR* strName_ );
    void UpdateShopItemList();
    void CategoryGrouping( std::vector<DWORD>& vecItemList, int ItemType, int iCategory );
    void SortTypeGrouping( std::vector<DWORD>& vecItemList, int iSortType );
    bool GetShopItem(int iShopItem, GCItem*& pItem );
    inline int	GetNumShopItem() { return (int)m_vecShopItemList.size(); }
    DWORD GetCharFlag( int iCharType_ );

public:
    void MakeCoordiShopItemList( EGCCharType eCharType_, int iItemType_, int iCategoly_, int iSortType_ , const WCHAR* strName_ = NULL );
    void MakeHotCoordiShopItemList( EGCCharType eCharType_, std::vector< KCoordiShopItem >& vecItemList_ );
    int  GetNumCoordiItem( void ) { return static_cast<int>( m_vecCoordishopItemList.size() ); }
    bool GetCoordiItem( ECoordiShopItemType eType, int iShopItem, GCItem*& pItem );
    bool GetCoordiItem( int iCoordiSetID, KCoordiShopItem& itemInfo );
    void LoadBaseCoordiItem( void );
    bool IsBaseCoordiItem( GCITEMID itemID_ );
    bool IsTransparentCoordiItem( GCITEMID itemID_ );
    
public:
    void MakeAgitCashItemList( std::vector<DWORD>& vecAgitCashItem );   // ����Ʈ �� ĳ�� ������ ����Ʈ�� �����. 
    
public:
    int GetVipNum();
    int GetGemNum();
    int GetCrystalNum();	
    int GetHeroCoin();
    bool IsExistCrystalDiscountCard( void );
    int GetItemType( DWORD dwItemGoodsID );
    bool IsItemToInvenByItemID( DWORD dwItemGoodsID );
    bool IsBreakUpItem( DWORD dwItemGoodsID );
    bool IsGPCoupon(GCITEMID dwGoodsID);
    bool IsVPCoupon(GCITEMID dwGoodsID);

public:
    //================================================Inventory Start===========================================//
    KItemContainer m_kInventory;

    //Clear
    //void ClearInventory( void );

    void SetDurationItem( IN OUT std::vector<KItem>& vecDurationItemInfo );
    bool DecDurationItem( GCITEMUID dwUID, std::vector<GCITEMUID>* pVector = NULL );
    bool DecDurationItemForItemID( SCharInfo* pCharInfo, int iItemID, std::vector<GCITEMUID>* pVector = NULL);		//�����ϰ� �ִ� �������� ������ ���Դϴ�.
    bool DecInventoryDurationItemForItemID( int iItemID, std::vector<GCITEMUID>* pVector = NULL, int iCount = 1);
    void DecInventoryDurationItemForItemIdWithOutAck( int iItemID );

    bool FindInventory( GCITEMUID itemUID );
    bool FindInventory( GCITEMUID itemUID, KItem** pInventoryItem );
    bool FindInventory( GCITEMUID itemUID, GCItem** pGCItem );
    bool FindInventory( GCITEMUID itemUID, KItem** pInventoryItem, GCItem** pGCItem );
    KItem* FindEquipedSkill( SCharInfo* pCharInfo, ESKILLPOSITION eSkillSlot );

    bool FindInventoryForItemID( DWORD dwItemID );
    bool FindInventoryForItemIDCoordi( DWORD dwItemID );
    bool FindInventoryForItemID( DWORD dwItemID, KItem** pInventoryItem );
    bool FindInventoryForItemID( DWORD dwItemID, GCItem** pGCItem );
    bool FindInventoryForItemID( DWORD dwItemID, KItem** pInventoryItem, GCItem** pGCItem );
    bool FindInventoryForItemIdAndLevel( DWORD dwItemID, int iLevel_ );

    int  GetInventoryItemDuration( int iItemID );

    void GetNewReceiveItemNum( const std::vector<KItem>& vecItemInfo, std::vector<KItem>& newVecItemInfo );

    //================================================Inventory End============================================//
    //bool ClearConsumptionItemFromEquip( void );
    //void ClearEquip( int iChar );
    //void SetEquip( int iChar, const KInventoryItem& kEquip );
    //void RemoveEquip( int iChar, int iIndex );
    
    bool CheckEquipItem( SUserInfo* pUserInfo, GCITEMUID dwUID );
    bool CheckEquipItem( GCITEMUID dwUID );
    bool CheckEquipItemForItemID( SCharInfo* pCharInfo, GCITEMID itemID, bool bCoordi = false );
    bool CheckEquipItemIDForEffect( SCharInfo* pCharInfo, GCITEMID itemID, bool bCoordi = false );
    bool CheckEquipItemForItemID( SCharInfo* pCharInfo, GCITEMID itemID, KItem*& pInventoryItem );
    bool CheckEquipItemForItemID( SCharInfo* pCharInfo, GCITEMID itemID, GCItem*& pGCItem );
    bool CheckEquipItemForItemID( SCharInfo* pCharInfo, GCITEMID itemID, KItem*& pInventoryItem, GCItem*& pGCItem );

    bool CheckEquipItemByItemUID( SCharInfo* pCharInfo, GCITEMUID dwItemUID_ );
    bool GetEquipItemUIDByItemID( SCharInfo* pCharInfo, DWORD dwitemID_, OUT GCITEMUID& dwItemUID_ );
    
    //SetItem
    void MakeSetItemList( std::vector<GCItem*>& pVecSetItem, int iSetID );
    void EquipSetItem(SCharInfo* pCharInfo, int iSetID);		//Set�� ��� �����ϴ�.
    void UnequipSetItem(SCharInfo* pCharInfo, int iSetID);		//Set�� ��� ����ϴ�.
    bool CheckEquipSetItem( SCharInfo* pCharInfo, int iSetID );
    bool CheckInventorySetItem( int iSetID );
    
    //Equip
    bool EquipInventoryItem(SCharInfo* pCharInfo, GCITEMUID dwUID, bool bCoordi_ = false, bool checkClass = true );

    void EquipShopItem( SCharInfo* pCharInfo, DWORD _ItemKey );
    void EquipItem( SCharInfo* pCharInfo, GCItem*& pItemInfo, bool bEquipBase_ = false, GCITEMUID itemUID_ = 0, bool bCoordi_ = false );

private:
    bool NormalEquipInventoryItem( SCharInfo* pCharInfo, GCItem*& pItemInfo, KItem*& pInvenItemInfo );
    bool CoordiEquipInventoryItem( SCharInfo* pCharInfo, GCItem*& pItemInfo, KItem*& pInvenItemInfo );

public:
    //Base Item
    bool IsBaseItem( int iItemID );
    void EquipBaseItem( SCharInfo* pCharInfo );
    void UnEquipBaseItem( SCharInfo* pCharInfo );

    //Unequip
    void UnEquipAllItem( SCharInfo* pCharInfo );    //  ��� �� �ڵ� ������ ��� �����.
    void UnEquipItem( SCharInfo* pCharInfo );       //  ��� ��� �����.
    
    void UnequipItemAll( SCharInfo* pCharInfo);     //  �� �Լ��� �����ѵ� �Ϻ� ���̰� �־ ���Ӵϴ�. ���߿� ��������.
    void UnequipLookItemAll( SCharInfo* pCharInfo ); //  �� �Լ��� �����ѵ� �Ϻ� ���̰� �־ ���Ӵϴ�. ���߿� ��������.
    
    void UnequipItemForItemUID( SCharInfo* pCharInfo, GCITEMUID itemUID );
    void UnequipItemForItemID( SCharInfo* pCharInfo, int iItemID );
    void UnEquipWeapon( SCharInfo* pCharInfo );

    //Remove & Unequip
    bool RemoveEmptyItem( SUserInfo* pUserInfo , std::vector< KSimpleItem >* pvecOut = NULL);

    void BuyItem( int iItemID, int iPeriod, bool bMessageBox = true);

    bool CheckItemImage( GCItem* pItem_ );
    bool CheckShopItemImage( GCItem* pItem_ );
    bool CheckSetItemImage( GCItem* pItem_ );
    bool CheckEquipItemMeshTex( GCItem* pItem_ );
    bool CheckItemImageFull();

    std::vector<std::string> m_vecStrNullItemImage;

    void LoadCoordiSetItemList( void );
    bool GetEquipCoordiSetItem( IN KCoordiSetTemplet& kCoordiSetTemplet, PLAYER* pPlayer );
    bool GetEquipCoordiSetItem( IN KCoordiSetTemplet& kCoordiSetTemplet, KSquareUserInfo kPlayer );

    void LoadSetItemList( void );
    void LoadCoordiSeasonList();
    int GetEquipSetItemName( PLAYER* pPlayer, bool bCoordi = false );
    int GetEquipSetItemName( PLAYER* pPlayer, TagCharacterInfo* pTagInfo, bool bCoordi = false );
    int GetEquipSetItemName( std::vector<int> vecItemInfo );

    void UpdateUserItem(SUserInfo* pUserInfo);

    std::vector<GCItem*> GetCashShopItem( DWORD dwGoodID_ );
    GCItemAbility GetItemAbility( DWORD dwGoodsID_ );
    GCItemAbility GetItemAbility( GCItem*& pItem_ );
    // NOTE : ȣȯ���� ���ؼ� ���ܵд�.

    //�⺻ ������������ üũ �Ѵ�. ������ ������ ������ �ö󰡸鼭 �ʿ伺�� ���ܼ� public���� ����
    bool IsBaseItem( int iCharType, DWORD ItemID );
    DWORD GetBaseItemID( int iCharType, ESLOTPOSITION eSlot );
   
    std::vector<int> GetSetItemAbility( PLAYER* pPlayer );
    void GetGaChaSetItemAbility( PLAYER* pPlayer);
    std::vector<int> GetSetItemAbility( PLAYER* pPlayer, TagCharacterInfo* pTagInfo );
    void GetGaChaSetItemAbility( PLAYER* pPlayer, TagCharacterInfo* pTagInfo );

    int GetSetItemNumber( SCharInfo* pCharInfo );
    

    // 20091026	[���� ��Ʈ ������ ��� �۾�] : �̵���
    int GetSetItemNumberByEquipVector( std::vector<DWORD>& rVecEquip );
    int IsSetItemMotion( const int iSetItemID_ );

    bool IsSetItemCheerUpMotion( const int iCheerUpItemID_ );

    bool IsEquipedItem( SCharInfo* pCharInfo, GCITEMUID itemUID_ );
    void MakeInventoryItemList( USER_EQUIP_CATEGORY eCategory, int iGroupType, bool bEtcItem,
        std::vector<GCITEMUID>* pvecInventory = NULL, bool bSortByLevel = true , bool bEquipAllChar = false);
private:
    SetTemplet*	GetEquipSetItemTemplet( std::vector<int>& vecSetItem, bool bDevide10 = false );
   
    //========================================================================
    //���� ĳ���Ϳ� �´� �κ��丮 ����Ʈ�� �����
    //bEtc�� true�̸� ItemCategory�� ���� ���� �����ִ°��̴�.

    bool IsCustomizationItem(GCITEMID iItem);

    void _MakeInventoryItemList( std::vector<GCITEMUID>& vecInventory, SCharInfo* pCharInfo, DWORD ItemCategory, DWORD SkillCategory, bool bEtc = false, int iGroupType = -1, USER_EQUIP_CATEGORY invencategory = UEC_EQUIP1, bool bSortByLevel = true, bool bNoClear = false , bool bEquipAllChar = false);
    //void MakeInventoryItemList(SCharInfo* pCharInfo, DWORD ItemCategory, DWORD SkillCategory, DWORD PetItemCategory, bool bEtc = false, int iGroupType = -1, USER_EQUIP_CATEGORY invencategory = UEC_EQUIP1 );
    void _GroupingInventory( USER_EQUIP_CATEGORY invencategory, int iGroupType, std::vector<GCITEMUID>& vecItemList );

public: // old methods
//  bool SaveItemDataFile( void );      // ���� �ȵ˴ϴ�. GCItem�� GCItemData�� ����ü�� �޶�����.

    //������ ���, �Ӽ�, ����, ����� Ÿ�� ���̺� ����.
    int GetGradeTableID( DWORD dwItemID );
    int GetAttributeTableID( DWORD dwItemID );
    int GetSocketTableID( DWORD dwItemID );
    int GetLevelDownTableID( DWORD dwItemID );
    int GetContinentTypeTableID( DWORD dwItemID );
    void SetGradeTableID( DWORD dwItemID, int iTableID );
    void SetAttributeTableID( DWORD dwItemID, int iTableID );
    void SetSocketTableID( DWORD dwItemID, int iTableID );
    void SetLevelDownTableID( DWORD dwItemID, int iTableID );
    void SetContinentTypeTableID( DWORD dwItemID, int iTableID );
    DWORD ItemCharTypeToItemFlag(int iCharType);
private: 
    // Load & UnLoad
    bool LoadItemDataFile( void );
    void UnloadItemDataFile( void );

    int _GetTableID( DWORD dwItemID, std::map<DWORD,int>& mapTable );
    void _SetTableID( DWORD dwItemID, int iTableID, std::map<DWORD,int>& mapTable );

    bool LoadTableScript();
    bool LoadGradeTable();
    bool LoadSocketTable();
    bool LoadAttributeTable();
    void LoadChannelItemList();         //  ä�κ� ���� �Ǹ� ����Ʈ ����
 
    DWORD GetItemGameModeFlag( DWORD gameMode );

    // NOTE : ȣȯ���� ���ؼ� ���ܵд�.
public: // old data field
    short   shMyAttack;
    short   shMyDefence;
    short   shMyHpInc;

    // NOTE : üũ���� �����ϴ� ������ �Բ� �����ȴ�.
private: // data field

    //�Ʒ� �ΰ����� ��� ������ ������
    std::hash_map< GCITEMID, GCItem* >			m_mapDB;
    std::hash_map< DWORD, std::vector<GCItem*> > m_mapCashShop;

    // 2007.02.20 wony
    // CheapItem �̳༮�� �ǹ̰� ��� �����մϴ�
    // �߱������� �Ⱓ���� ���������� �� �������� �����ִ°� �۾��� �Ǿ��ִµ�
    // ���߿� ��ġ�� �۾��� �Ѵٸ� ���ѵ� ���Ǹ�-��-;;
    // GCItem**								m_pCheapItem;		//���� �� �����۸� ��������
    //std::map<DWORD,KInventoryItem*>			m_mapInventory;

    std::vector<GCItem*>					m_vecAlreadyEquipSetItem;

    DWORD                                   m_dwNumItem;
    DWORD                                   m_dwTotalNumItem;

    std::vector<KCoordiSetTemplet>          m_vecCoordiSetList;     //  ������ �ڵ� ��Ʈ ������ ����Ʈ ( ����Ʈ ���� ó�� ) 
    std::vector<SetTemplet>                 m_SetList;
    std::vector<CoordiSeasonTemplet>        m_vecCoordiSensonList;  //  ������ �ڵ� ��Ʈ ������ ����Ʈ ( ���� �����ϰ� ���� )
    
    std::vector<DWORD>                      m_vecShopItemList;
    
    std::vector<GCITEMUID>                  m_vecInventory;
    
    // �ڵ� ������ ������ �����մϴ�. 
    std::map< int, KCoordiShopItem >                    m_mapCoordishopItem;        // �ڵ� ������ ���� < �ڵ� ������ ��Ʈ �ѹ�, �ڵ� ������ ID >
    std::map< GCITEMID, GCItem* >               m_mapCoordishopRandomBox;        // �ڵ� ������ ���� < �ڵ� ������ ��Ʈ ��

    std::vector<std::pair<GCITEMID, GCITEMID>>  m_vecCoordishopItemList;
    std::set< GCITEMID >                                m_setBaseCoordiItem;        // �⺻ �ڵ� ���� ( ���������� �����ش�. ) 
    std::set< GCITEMID >                                m_setTransparentCoordiItem;        // ���� �ڵ� ���� ( ������ ���� �����ش�. )
    std::vector<kRemovePartsCoordiItem>                 m_vecNoFaceItem; //�� �Ⱥ��̰� �ϴ� ������ 

    // 2007.02.16 wony
    // �߱����� ����մϴ�
    // ���� �� �������� ������ ���� �� �д�
    // m_mapDB���� ��� �������� ����ǰ�
    // ���� �ִ� m_pCheapItem�� m_mapDB�� �������ϹǷ� �̸��� cheap���� ������ expensive �̴�
    //      GoodsID  Price
private:
    std::unordered_map< DWORD, DWORD >	m_mapCheapItem;
public:
    DWORD GetCheapItemPrice( DWORD dwGoodsID );

    // �� �ǿ��� ���̴� ������ �ΰ�?
    bool IsPetTabItem( DWORD dwGoodsID );
    bool IsSelectCharacterPet( const char cCharType_, GCITEMUID itemUID_ );

private:
    // 2007.04.26 wony
    // ����Ʈ �������� ������ ����ִ�
    std::vector<PointItemList>              m_vecPointItemList;
    std::vector<PointItemList>              m_vecPointShop;

    // �� �������� ���� �� �ִ� ���� ������ ����Ʈ
    std::vector<KItem>                      m_vecPerfectMonthItemList;
    std::vector<KItem>                      m_vecPerfectMonthBoxItem; // �ڽ��� ���� ĳ������ �����۸� ���̵��� ����

    // �� �������� ���� �� �ִ� ���� ������ ����Ʈ
    std::vector<KItem>                      m_vecPerfectWeekItemList;

    // üũ�ϰ� �ִ� GCItem* Pool
    std::unordered_map< int, GCItem* >                m_mapPTypeCheckPool;
    typedef std::unordered_map< int, GCItem* >::iterator PTCheckIter;

    // ����ĳ�� ����
    GCITEMID m_dwCurrentGoodsID;        // ���� �������� ���� ������

public:
    void SendBuyForCashItem(GCItem* kItem, DWORD dwUseVirtualCash, bool bPresent, std::wstring strOwnName = L"", std::wstring strComment = L"" );
    void SendBuyForVirtualCashItem(GCItem* kItem, int iAmmount = 1 );
    void BuyCashItemS6( GCITEMID dwItemID_, bool bMsgBox = false );
    void BuyCashItemQuantity( GCITEMID dwItemID_, bool bForce = false );
	void PresentCashItemS6( GCITEMID dwItemID_, bool bMsgBox = false );
    GCITEMID GetCurrentGoodsID(){ return m_dwCurrentGoodsID; }
    
    bool m_bIsInPresentState; //���� Ÿ�� �޼��� �ڽ� ��ġ ���п�. (�����ϱ�/����)
public:
    // 2007.04.26 wony
    // ����Ʈ �������� ������ �ε��մϴ� PointItem.stg ����..=��=
    void LoadPointItemList();

    // ����Ʈ �� ����
    inline int GetPointShopSize() 
    { 
        return (int)m_vecPointShop.size(); 
    }

    PointItemList GetPointItem( int iIndexList )
    {
        return m_vecPointShop[ iIndexList ];
    }

    void MakePointShopItemList();
    bool IsPointShopItem( DWORD dwGoodsID );
    int GetPointItemPrice( DWORD dwGoodsID );
	int GetPointItemTypeValue( DWORD dwGoodsID );

    // ������ ���ݰ� ȭ�� ������ �ݴϴ�. pair<����, ȭ��Ÿ��>
    bool IsSellAvailableItem( IN GCITEMUID itemUID_ );
    bool IsRemoveAvailableItem( IN GCITEMUID itemUID_ );
    std::pair<int,EMONEYTYPE> GetItemPrice( IN GCITEMUID itemUID_, IN int iDuration_ = 0 );
    void BuyPointItem( DWORD dwGoodsID, IN int nCount = 1, IN int nMoneyType = KBuySellItemReq::EMT_GCPOINT  );

    // �� ���� ���� ������ ����
    void SetPerfectMonthItemList( IN const std::vector<KItem>& vecItemList )
    {
        m_vecPerfectMonthItemList = vecItemList;
    }

    void SetPerfectWeekItemList( IN const std::vector<KItem>& vecItemList )
    {        
        m_vecPerfectWeekItemList = vecItemList;
    }

    std::vector<KItem> GetPerfectWeekItemList()
    {
        return m_vecPerfectWeekItemList;
    }

    inline int GetPerfectMonthBoxItemSize()
    { 
        return (int)m_vecPerfectMonthBoxItem.size(); 
    }

    KItem GetPerfectMonthBoxItem( int iIndexList )
    {
        return m_vecPerfectMonthBoxItem[ iIndexList ];
    }

    KItem GetPerfectMonthItem( IN const GCITEMID iItemID );

    void MakePerfectMonthItemList();


    // �� ���� �ɸ��Ͱ� �����ϰ� �ִ� ����߿� Ư�� ������ġ�� �ִ�
    // �������� ã�´�.
    bool GetMyCharEquipByItemPosition( IN DWORD dwItemPosition, OUT GCItem** ppItem );
    bool GetMyCharEquipByItemPosition( IN DWORD dwItemPosition, OUT KItem** ppInvenItem );
    bool GetMyCharEquipByItemPosition( IN DWORD dwItemPosition, OUT KItem** ppInvenItem, OUT GCItem** pItem_);
    std::vector<KItem> GetInvenItemFromKind( GCITEM_KIND eItemKind_ );
    bool CheckRare( const DWORD dwItemID );
    bool CheckItemKind( const DWORD dwItemID, const GCITEM_KIND eItemKind );
    std::wstring GetItemName( DWORD dwGoodsID_ );
    std::wstring GetItemNameUID( GCITEMUID dwUID_ );

    DWORD GetPremiumType( int iPlayerIndex );
    bool IsMetamorphosisItem( DWORD dwItemID );
    
    DWORD GetItemStringColor( GCITEM_KIND eItemKind_ ) 
    {
        return m_aStringColorTable[eItemKind_];
    }	
    void LoadNormalTitleItem(void);

    // ��ȸ�� �Ǽ��縮 Īȣ
    bool IsRallyTitleItemListIndex( const int iIndex );
    bool IsRallyTitleItemID( const int iItemID );

    // Īȣ�� ���̵� �Ѱ��ָ�, �ش� Īȣ�� ���� �� %�� �г�Ƽ�� ����Ǿ� �ִ��� ������ݴϴ�.
    float GetTitleDecreasePercentByItemID(const int iTitleID);

    void UnEquipTitleItem( SCharInfo* pCharInfo );
    bool IsEquipTitleItem( SCharInfo* pCharInfo, int iItemID );
    int GetEquipTitleItem( SCharInfo* pCharInfo );
    bool EquipTitleItem( SCharInfo* pCharInfo, int iItemID );

private:
    KSafeArray<DWORD,GIK_ITEM_KIND_NUM> m_aStringColorTable;
    //====================================================================================
    // 2007-09-28 ���� 5:40:39 : Asirion 

    _inline void ExceptDuplicatedItem( DWORD dwUID_ );

    std::vector<int> m_vtRally_Collection_List;
    std::vector<float> m_vtRally_Collection_Hp;
    std::vector<float> m_vtRally_Collection_State;
    std::vector<float> m_vtRally_Collection_Arena;

public:
    std::wstring DumpInventory( GCITEM_KIND eKind = GIK_ITEM_KIND_NUM );
    
    DWORD   m_dwHotItemType;	
    
    DWORD GetHotItemType(){ return m_dwHotItemType; }
    void SetHotItemType( DWORD _dwType){ m_dwHotItemType = _dwType; }
    void ReSetNewItemType() { m_dwHotItemType = EM_HOT_ITEM_NONE_TYPE; }

    bool IsBetterItemAdd(GCItem*& pItem);
    bool IsEquipmentItem( DWORD dwItemID_ );  
    bool IsArmorEquipItem( DWORD dwItemID_ );   // �� �������̴�
    bool IsClosetItem(GCItem* pItem);


public:
    KSafeArray<DWORD,UEC_NUM> m_dwItemCategoryFlag;

private:
    std::map< std::pair<DWORD, int>, KDropItemInfo >  m_mapTradeItem;
    std::map< DWORD, KDropItemInfo > m_mapAllTradeItem;
public:
    void SetNotbuyInfoToAllItem( bool bNotbuy );
//	void SetTradeItem(std::map< std::pair<DWORD, int>, std::pair<DWORD, int>>& mapTradeItem_){ m_mapTradeItem = mapTradeItem_; }
    void SetTradeItem( std::map< std::pair<DWORD, int>, KDropItemInfo>& mapTradeItem_, std::map<DWORD,KDropItemInfo>& mapAllTradeItem_ );
    std::pair<DWORD, int> GetItemTradeInfo(DWORD dwItemID_);
    bool IsExistTradeItem(DWORD dwItemID_);
    DWORD GetTradeableItemID();

public:
    void UpdateInventory( const std::vector< KItemUnit >& vecItemUnit_ );
    bool DecreaseDuration( GCITEMUID dwItemUID_, bool bSendServer_);
    std::wstring GetItemTypeString(GCItem* pItem_ );
    std::wstring GetCoordiSlotString( ESLOTPOSITION emSlotPosition );
    
    std::wstring GetItemGradeString( char cItemGrade_);
    std::wstring GetItemGradeColorKey( char cItemGrade_ );
    DWORD GetItemGradeColorRGB( char cItemGrade_ );
    std::wstring GetItemAtrributeString( GCItem* pItem_ );
    std::wstring GetItemLevelString( GCItem* pItem_ );

    std::vector< GCItem* > GetMonsterCardList();
    std::vector< GCItem* > GetOwnMonsterCardList();
    void GetMonsterCardRewardList( const std::vector<GCITEMID>& _vecMaterial, std::vector<GCITEMID>& _vecReward );

    std::wstring GetAtrributeString( char cAtrributeType_, float fAbility_ );
    std::wstring GetSocketStateString( char eItemSocketState_ );
    std::wstring GetItemName( GCItem* pItem_, char cPetPromotion_, KItem* pInven_ , SUserItem* pUserItem_, KPetInfo* pPetInfo_ );
    std::wstring GetItemNameDesc( GCItem* pItem_, char cPetPromotion_ );
    std::wstring GetItemMoneyTypeString( GCItem* pItem_);

public:
    std::set<int> GetSetEffectItemName( GCITEMID ItemID_, std::map< int, std::vector< int > >* pMapPartsList_ = NULL );
   
    
    bool GetEquipSetItemAttributeBySetName( int iSetItemName_, OUT std::vector< SetItemAttributeList >& vecSetAttribute_ );
    bool GetCoordiSeasonAttribute( int iSeasonNum_, OUT std::vector< SetItemAttributeList >& vecSetAttribute_ );

    //  ���� �ڵ��� �ɷ�ġ�� ������ �ɷ�ġ�� �����´�.
    void GetItemAttribute( PLAYER *pPlayer, int iCharType_, bool CheckState, EItemAbilityData& stEquipItemAbility_); 

    //  ���� �ڵ��� �ɷ�ġ�� �����ͼ� ĳ���� �ɷ�ġ�� ��ȯ���ش�.
    void GetItemRealAbility( PLAYER *pPlayer, int iCharType_, bool CheckState, OUT EAbilityData& stRealAbility_);
    
    void GetEquipItemAttribute( PLAYER *pPlayer, int iCharType_, bool CheckState, EItemAbilityData& stEquipItemAbility_);
    void GetCoordiItemAttribute( PLAYER *pPlayer, int iCharType_, bool CheckState, EItemAbilityData& stEquipItemAbility_);
    
    EITEMSLOTPOSITION GetItemSlotPosion( COORDI_SLOT_POSION emPosition );
    ESLOTPOSITION GetSlotPosion( COORDI_SLOT_POSION emPosition );
    int GetSlotPostionCount( const GCItem* pItemInfo_ );
    int GetCoordiSlotSeasonNum( SCharInfo* pCharInfo, ESLOTPOSITION emPosition );
    
    int GetCoordiSeasonNum( GCITEMID ItemID_, int nGrade, std::set< int >* pVecPartsList_ = NULL );        
    CoordiSeasonTemplet* GetCoordiSeasonTemplet( GCITEMID _itemID, int nGrade, bool bDevide10 = false );
    std::wstring GetCoordiSeasonName( int _iSeasonNum );
    
    
    
    
    void ConvertAttributeToAbility( OUT EAbilityData& stAbilityData_, IN const EItemAbilityData& stAttributeData_);
    void MakeEquipAbilityUseful(IN EAbilityData& stAbilityData_, bool bStartGame_ = false );
    
    
    std::vector<int> GetSetItemList(int SetID );
    EItemAbilityData GetItemAbilityData( GCItem* pItem_, int iCharacterLevel_, KItem* pkInvenItem_ = NULL, SUserItem* pUserItem_ = NULL, KPetInfo* pPetInfo_ = NULL, int iLevel_ = -1);
    EItemAbilityData GetItemAttributeAbilityData( GCItem* pItem_, int iCharacterLevel_, KItem* pkInvenItem_ = NULL, SUserItem* pUserItem_ = NULL );

    GCItemAbility GetPetItemAbility( KPetInfo* pPetInfo_, int iCharacterLevel_);
    bool GetEquipItemOfSamePosition( PLAYER* pPlayer_, GCItem* pStandardItem_, OUT GCItem** ppItem_ = NULL, OUT SUserItem** ppUserItem_ = NULL, OUT KPetInfo** ppPetInfo_ = NULL);
    bool IsEquippedSameItem( PLAYER* pPlayer_, GCItem* pStandardItem_, GCITEMUID ItemUID_);
    std::wstring GetRemainTime( IN const KItem&  kItem );

    void SetMonsterCardScriptInfoFromServer( KEVENT_MONSTER_CARD_MIX_INFO_ACK kPacket_ );

private:
    void LoadMonsterCardManager();

    KGCMonsterCardManager	m_kMonsterCardManager;

    bool IsChannelItemList( GCITEMID _itemID, DWORD dwChannel );
    std::map<GCITEMID,DWORD> m_mapChannelItemList;


    std::map< DWORD, KSocketConsumeInfo > m_mapCashItemUseCount;
    std::map< DWORD, KSocketConsumeInfo > m_mapConsumeGP;
    GCITEMID    m_SocketChangeItemID;
public:
    void SetSocketManagementCostTableInfo( std::map< DWORD, KSocketConsumeInfo > mapCashItemuseCount_, std::map< DWORD, KSocketConsumeInfo > mapConsumeGP_, GCITEMID SocketChangeItemID_);
    std::map< DWORD, KSocketConsumeInfo > GetSocketManageCashItemUseCountList() { return m_mapCashItemUseCount;}
    std::map< DWORD, KSocketConsumeInfo > GetSocketManageConsumeGPList() { return m_mapConsumeGP;}
    GCITEMID GetSocketChangeItemID() { return m_SocketChangeItemID; }

    struct ItemAbilityPercent
    {
        float fAtkCalcRate;
        float fDefCalcRate;
        float fVitCalcRate;

        ItemAbilityPercent() : fAtkCalcRate(0.0f), fDefCalcRate(0.0f), fVitCalcRate( 0.0f)
        {}
        ItemAbilityPercent( float fAtkRate_, float fDefRate_, float fVitRate_) : fAtkCalcRate( fAtkRate_), fDefCalcRate( fDefRate_), fVitCalcRate(fVitRate_)
        {}

    };

    struct ItemAbtilityPercentByCharType
    {
        ItemAbilityPercent stDefaultItemAbtilityPercent;
        std::map< int, ItemAbilityPercent > mapAbilityPercentByCharType;

        ItemAbtilityPercentByCharType()
        {}
    };

    struct ItemAbilityCalcRateByCharType
    {
        float fDefaultRate;
        std::map< int, float> mapAbilityCalcRateByCharType;

        ItemAbilityCalcRateByCharType() : fDefaultRate(0.0f)
        {}
    };

    std::map< int, ItemAbtilityPercentByCharType >   m_mapAbilityPercentByCharType;
    std::map< int, ItemAbilityCalcRateByCharType > m_mapCalcRateByCharType;
    std::map<int,std::vector<int>> m_mapSetItemList;
    void LoadItemAbilityCalcTable();
    GCItemAbility CalcItemAbility(GCItem* pItem_, KItem* pInven_ = NULL, char cItemGrade_ = 0, int iLevel_ = -1, SUserItem* pUserItem_ = NULL );
    int GetUniqueChartype(DWORD  dwCharType_ );
    int GetUniqueSlotPosition( DWORD dwSlotPosition_ );
    std::set<GCITEMID > m_setAbilityCalcExceptionList;
    void LoadItemAbilitCalcExceptionList();
    const KItemContainer::ItemList& GetFullInvenItemList() { return m_kInventory.GetFullItemList(); }

private:
    //�������� ����
    std::set< GCITEMID > m_setBuyLimitedItemList;
    void LoadBuyLimitItem();

    //ī��Ʈ ������ �� Ư���� ������..... Ư�� ���������� ���� (��Ҹ���ũ, �浶��)
typedef std::map< GCITEMID, std::set<int> >  MAP_EXCEPTION_COUNT_ITEM;
    MAP_EXCEPTION_COUNT_ITEM m_mapExceptionCountItem;
    void LoadExceptionConuntItem();
public:
    bool IsLimitItemID( GCITEMID ItemID_ );
    bool NotDecDurationItem(GCITEMID ItemID_ );

    GCItem* GetSameImageIndexItemInfo(GCITEMID iItemID_);
    INT GetSameImageIndex(GCITEMID iItemID_);
    INT GetSameTextureIndex(GCITEMID iItemID_);
    GCDeviceTexture* CreateShopItemTexture( int iItemID );
    GCDeviceTexture* CreateItemTexture( int iItemID, int iCharType = -1, int iCharPromotion = -1 );
    GCDeviceTexture* CreateItemTextureCustom(int iItemID, int iCharType = -1, int iCharPromotion = -1);
    GCDeviceTexture* CreateShopSetItemTexture( int iSetItemNum_, int iItemID );
    GCDeviceTexture* CreatePetItemTexture( int iPetGoodsID, int iEvolutionLevel = 0 );
    GCMeshObject* CreateLas4MergeAbtaModel( int iItemID_, int iCharType_, GCObject* pObj_ ,bool build = false);	
    GCMeshObject* CreateAbtaModel( int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, bool build=false, char cIgBoneIdx = -1 );
    GCMeshObject* CreateRoomAnimationModel(std::string RoomAnimationModel, GCObject* pObj_, bool build, char cIgBoneIdx = -1);
    void CreateTmpModel( int iItemID_, int iCharType_, int iPromotion, char cIgBoneIdx = -1 );
    std::vector<GCObject*> CreateLiveModel( int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, D3DXVECTOR3 vOptionalPos = D3DXVECTOR3(0.0f,0.0f,0.0f), bool bOnlyTraceParentPos = false, char *szMotionFileName = NULL, int iBoneIdx = -1, bool bUseBoneIDFromArg = false );
    void Draw_ShopImage( D3DXVECTOR2 vPos, float fWidth, float fHeight, GCDeviceTexture* tex, bool bSetItem, int iSetItemNumber = -1, D3DCOLOR col=0xffffffff, DWORD dwGoodsID_ =0 );
    void Draw_ShopImage_Origin( D3DXVECTOR2 vPos, float fWidth, float fHeight, GCDeviceTexture* tex, bool bSetItem, int iSetItemNumber = -1, D3DCOLOR col=0xffffffff, DWORD dwGoodsID_ =0 );

    void LoadTableScript(std::string strFileName_, map_same_image_index_type_type &mapBuf, bool div =false);
    void LoadSameImageIndexScript(){ LoadTableScript( "sameimageindex.lua",m_mapSameImageIndex, true ); }
    void LoadSameTextureIndexScript() { LoadTableScript( "sametextureindex.lua", m_mapSameTextureIndex, true ); }
    int GetItemCategory( GCITEMID itemID_ );

    void SellItem( IN const std::vector<KItem>& vecItemList );
    void AddSellItemToList( IN const KItem& kItem );
    void RemoveSellItemList( IN GCITEMUID itemUID_ );
    void ClearSellItemList() { m_vecSellList.clear(); }
    void SellItemFromList();
    bool IsContainSellItemList( IN GCITEMUID itemUID_ );
    bool IsEmptySellItemList() { return m_vecSellList.empty(); };
    const std::vector<KItem>& GetSellList() { return m_vecSellList; }
    std::vector<KItem>::size_type GetSellListSize() { return m_vecSellList.size(); }

    void AddSellPriceInfo( IN GCITEMID itemID_, IN const KSellInfo& kInfo_ );
    bool GetSellPriceInfo( IN GCITEMID itemID_, OUT KSellInfo& kInfo_ );
    bool GetCountItemPrice( IN GCITEMID itemID_, IN int iCount_, OUT int& iPrice_, OUT EMONEYTYPE& eMoneyType_ );

    // ���� ������ ���� ���ʽ� �ɷ�ġ ���
    void SetPromotionStatus( EAbilityData& stAbilityData_, int iPromotion_, PLAYER* pPlayer_ = NULL );
    void addStatusBonus(EAbilityData& stAbilityData_, float bonus);
    void addCriticalBonus(EAbilityData& stAbilityData_, float bonus);
private:
    std::vector<KItem> m_vecSellList;
    std::map<GCITEMID,KSellInfo> m_mapSellPrice;
    map_same_image_index_type_type m_mapSameImageIndex;
    map_same_image_index_type_type m_mapSameTextureIndex;

    std::vector<DWORD> m_vecBaseItem;


public:
    void LoadHeroItemSpecialAbility( void );

    void InitHeroItemSpecialAbility( int iPlayerIndex_ );
    void SettingHeroItemSpecialAbility( GCITEMID dwItemID_, int iPlayerIndex_ );
    bool GetHeroItemCondition( GCITEMID dwItemID_, std::vector<float>* pOutVector );
    int  GetHeroItemBuffGroup( int nBuffIndex );

    void SetRandomInitAttributeItemList(MAP_DWORD_SET arg_){m_mapRandomInitAttributeItemList.clear(); m_mapRandomInitAttributeItemList = arg_;}

    bool CheckRandomInitItemtoInventory(OUT DWORD& dwItemID, int iGrade);
    bool IsCashItem(DWORD dwItemID);
    bool GetAttributeTable( IN int iTableID, IN int iGrade, OUT KGCAttributeTable& kTable_ );
    bool CheckAndChangeEventItem(GCItem*& pItem);


    MAP_INVEN_BUFF_GROUP	m_mapInvenBuffItem;
    
    void SetInventBuffItemList( VEC_INVEN_BUFF& vecInvenBuff );

private:
    void LoadEventChangeItemList();             
    bool isEventChangeItem( DWORD _dwItemID );  

    std::vector<KHeroItemSpecialAbility> m_vecHeroItemAbility;
    std::map< int, int >                 m_mapHeroItemBuffGroup;

    




    FILE* pSort;

    MAP_DWORD_SET	m_mapRandomInitAttributeItemList;
    std::map<DWORD,INT> m_mapGachaList;
    std::vector<GCITEMID> m_vecExpItemList;
    std::map<GCITEMID, int> m_mapSkillScrool;

public:
    int GetlItemEquipLevel( GCItem* pItem_, KItem* pInven_ = NULL, SUserItem* pUserItem_ = NULL, bool bLevelDown = false );
    int GetlItemAbilityLimitLevel( GCItem* pItem_, KItem* pInven_ = NULL );

    //�̺�Ʈ ������ ��ȯ - Ư���ð����� �������� �������� �ٲ��.
    struct EVENTITEMCHANGE
    {
        DWORD dwChangeItem;
        CTime timeStart;
        CTime timeEnd;
    };
    std::map< DWORD, EVENTITEMCHANGE>   m_mapEventItemChange;

public:
    std::set<CTime> m_setApplyRefreshEventTimeChange;
    void LoadGachaList();
    bool IsGachaList(DWORD dwGoodsID){return m_mapGachaList.end() != m_mapGachaList.find(dwGoodsID); }
    std::map<DWORD,INT> GetGachaList(){return m_mapGachaList;}
    bool IsExpItemList(GCITEMID dwGoodsID);
    void SetExpItemList(std::vector<GCITEMID> &vecList);

    void EquipChangeWeaponItem( SCharInfo* pCharInfo, GCItem*& pItemInfo, KItem* pInvenItemInfo);
    //�̺�Ʈ ������ �� - Ư���ð����� �������� ������ ���̰� �Ѵ�.
    struct EVENTITEMVIEW
    {
        DWORD dwViewItem;
        CTime timeStart;
        CTime timeEnd;
    };
    bool IsEventTimeViewItem( DWORD dwGoodID );
    void LoadEventViewItemList();
    bool AddEventTimeViewItemList( DWORD dwGoodID );
    void BuyGPItem( GCItem* kItem, int iItemID, int iPeriod );
    void BuyCashItemWithPoint( GCItem* kItem, int iPeriod );
    void LoadPointShopValidItemList( void );
    BOOL IsInVaildPointShopItem( GCITEMID _goodsID );
    std::vector< EVENTITEMVIEW >   m_vecEventItemView;
    std::set<CTime> m_setApplyRefreshEventTimeView; 

    struct LAST_SHOP_INFO
    {
        LAST_SHOP_INFO()
            :CharType(GC_CHAR_INVALID)
            ,ItemType(SIT_ARMOR)
            ,iCategory(0)
            ,iSortType(SIS_NEW)
            ,iMoneyType(0)
        {};
        EGCCharType CharType;
        int ItemType;
        int iCategory;
        int iSortType;
        int iMoneyType;
        std::wstring strName;
    };
    LAST_SHOP_INFO m_shopListLastLog;

private:
    // ��Ű�� ������ ����
    std::map<GCITEMID,std::vector<KDropItemInfo>> m_mapPackageInfo;
    bool                                          m_bRecievePackageInfo;

public:
    std::set< GCITEMID >                                m_setPointShopValidItemList;        // �⺻ �ڵ� ������ ID ���� 

    bool HaveEventItem();
private:
    std::map<int, vector<int>> m_mapEnchantEpicItemStatTable;
    std::map<int, vector<int>> m_mapEnchantNormalItemStatTable;
    int m_iMaxCoordiInvenSize;

public:
    void InitEnchantItemStatTable();

    GCItemAbility GetEnchantItemAbility( GCItem* pItem_, KItem* pInvenItem_ );
    GCItemAbility GetEnchantItemAbility( GCItem* pItem_, SUserItem* pUserItem_ );
    int GetEnchantItemStat( int iItemType, int iEnchantItemType_, int iEnchantLv_ );

    std::pair<bool,int> GetEquipedEnchantWeaponInfo(SCharInfo* pCharInfo_ );
    std::pair<bool,int> GetOtherUserEquipedEnchantWeaponInfo( SCharInfo* pCharInfo_ );
public:
    bool IsHotItem( GCITEMUID itemUID_, PLAYER* pPlayer_ );    
    bool IsNewItem( GCITEMUID itemUID_ );
    bool IsPutItemInDepot( GCItem* pItem, KItem* pkItem );
    
    int GetCoordiInventoryMaxSize();
    int SetCoordiInventoryMaxSize( int iMaxCoordiInvenSize);	
    
    void AddNewItem( GCITEMUID itemUID_ );
    void ClearNewItem( void );
    bool CheckEquipLevel( const int iCharacterLevel_, GCItem* pItem_, KItem* pInvenItem_ );
    bool CheckEquipLevel( const int iCharacterLevel_, GCITEMUID itemUID_ );
    bool GetItemUniqueSlotPosAndAbility( IN GCITEMUID itemUID_, IN SCharInfo& sCharInfo_, OUT int& iSlotPos_, OUT int& iAbility_ );
public:
    bool GetInventoryItemListFromItemID( IN GCITEMID itemID_, OUT std::vector< KItem* >& vecItemList );
    int GetLevelDownReqCoin( GCItem* pItem, KItem* pInvenItem );
    bool IsLevelDownItem( KItem* pInvenItem );



    std::map<GCITEMID,std::vector<KDropItemInfo>>& GetPackageInfo();
    void SetPackageInfo( std::map<GCITEMID,std::vector<KDropItemInfo>> _mapPackageInfo, bool bLastData );
    bool IsPackageItemID( IN GCITEMID dwItemID_ );
    void GetPackageElements( IN GCITEMID dwItemID_, OUT std::vector< KDropItemInfo >& vecItems_ );	
    void ClearPackageInfo();


    bool IsPaintItem();
    bool IsCashPaintItem();
    bool IsFreePaintItem();
    bool IsCurrentCharacterItem(GCITEMID giItemID);

// ���� �Ұ��� ������ ����Ʈ - �κ��丮���� ���� �Ⱥ��⿡ ó���Ѵ�. 
private:
    std::map< int, std::vector< GCITEMID > >    m_mapEquipDisableItem;  // < ĳ���� Ÿ��, ������ ����Ʈ >

public:
    void LoadEquipDisableItemList( void );
    bool IsEquipDisableItem( const IN int iCharType_, const IN GCITEMID itemID_ );
    
    void SetCharacterCardInfo( KEVENT_NEW_CHAR_CARD_INFO_NOT &mapInfo );
    bool GetCharacterCardInfo( int iChar, OUT KCharCardInfo &kInfo );
    bool GetCharacterCardInfo( int iChar, OUT GCITEMID &ItemIDCharCard, OUT GCITEMUID &ItemUIDCharCard );
    bool IsCharCard( GCITEMID ItemIDCharCard );
private:
    KEVENT_NEW_CHAR_CARD_INFO_NOT m_mapCharCardInfo;
    //CharType , MissionGoodsID
    std::map<int,GCITEMID> m_mapItemInfo;
private:
    std::set< GCITEMID >    m_setDpotNonvisibleItem;

public:
    void LoadDepotNonvisibelItem( void );
    bool IsDepotNonvisibleItem( const IN GCITEMID itemID_ );
    void LoadCashCharacterPackageItemScript();
    GCITEMID FindCashCharacterPackageItemID(int iChar);
    std::map<int,std::pair<GCITEMID, GCITEMUID>> GetUserCharCardList( );
    bool LoadRecomData(KLuaManager &luaMgr, std::map<DWORD, DWORD> &mapItemRecom);
    bool LoadRecomFile( std::string strFileName, std::map<DWORD, DWORD> &mapData  );
    bool LoadRecomFiles( );
    MAP_CHARID_MAP_RECOM m_mapItemRecomLevel;
    DWORD GetItemRecomData( GCITEMID dwItemID );

private:
    MAP_DWORD_SET	m_mapGPAttributeRandomItem;         // GP �Ӽ� ���� ������   
    MAP_DWORD_SET	m_mapGPAttributeInitItem;           // GP �Ӽ� �ʱ�ȭ ������ 
    MAP_DWORD_SET	m_mapAttributeSingleRandomItem;     // ���� �Ӽ� ���� �ֹ��� ����Ʈ

public:
    void SetGPAttributeRandomItemList( MAP_DWORD_SET mapList_ );
    void SetGPAttributeInitItemList( MAP_DWORD_SET mapList_ );
    void SetAttributeSingleRandomItemList( MAP_DWORD_SET mapList_ );

    bool CheckGPAttributeRandomItemFromInventory(OUT DWORD& dwItemID, int iGrade );
    bool CheckGPAttributeInitItemFromInventory(OUT DWORD& dwItemID, int iGrade );
    bool CheckAttributeSingleRandomItemFromInventory(OUT DWORD& dwItemID, int iLevel );

private:
        KItemBuyInfo    m_KItemBuyInfoData;
        bool            m_bItemBuyInfo;

        int             m_iPackageIndexMax;
        int             m_iPackageCurrendIndex;
        bool            m_bIsPackagInfo;

public:
    void SetPackageInfo( bool bPackage ) {  m_bIsPackagInfo = bPackage; }
    void SetPackageIndexMax( int iPackageIndexMax ) { m_iPackageIndexMax = iPackageIndexMax; }
    void SetPackageIndexCurrendIndex( int iPackageCurrendIndex ) { m_iPackageCurrendIndex = iPackageCurrendIndex; }

    int GetPackageIndexMax( ) { return m_iPackageIndexMax; }
    int GetPackageIndexCurrendIndex( ) { return m_iPackageCurrendIndex; }

    void SetItemBuyInfo( bool bItemBuyInfo_ ) { m_bItemBuyInfo = bItemBuyInfo_; }
    bool GetItemBuyInfo() { return m_bItemBuyInfo; }
    void KeepItemBuyInfoData(KItemBuyInfo& kItemBuyInfoData_ ) { m_KItemBuyInfoData = kItemBuyInfoData_; }
    void SetItemBuyInfoData( KItemBuyInfo& kItemBuyInfoData_ );
    void LoadRemovePartsItem( void );
    bool IsRemovePartsItem( GCITEMID itemID_ , int iRemoveParts );

    EItemElemental GetElementalType( EITEMATTRIBUTE eAttr );
    std::string EnumElementalTypetoStr( EItemElemental eElementalType );

public:
    bool IsEquipMonsterCard( const SCharInfo& charInfo_, const GCITEMID cardID_ );

public:
    std::vector<GCItem*> GetItemIDFromName(const WCHAR* strName_ , int chartype );
    void AddItemInInventory(std::vector<GCItem*> vectoritemlist, std::vector<const WCHAR*> );
    void VerifyCharCardInfo(void);

public:
    int GetEquipLevelFromMonsterCard(DWORD dwItemID);

    // �ڵ� �ռ� ����
    bool IsCoordiSlot( DWORD dwSlotPosition );
    int EnableAbilityCoordi( GCITEMUID _itemUID );
    int EnableDesignCoordi( GCITEMUID _itemUID );
    int DoCoordiCompose( GCITEMUID _AbilityItemUID, GCITEMUID _DesignItemUID );

    std::vector<GCITEMUID> GetEnableCoordiComposeList( bool bDesignCoordi, GCITEMUID _AbilityItemUID, GCITEMUID _DesignItemUID );
        

    // �ڵ� ��� ���׷��̵� ������
    SCoordiUpgradeInfo m_sCoordiUpgradeInfo;

    void CoordiGradeUpgradeInfoReq();
    void SetCoordiUpgradeInfo( KEVENT_COORDI_GRADE_UPGRADE_INFO_ACK& kRecv );
    int EnableCoordiUpgrade( GCITEMUID _itemUID );
    int DoCoordiUpgrade( GCITEMUID _itemUID );
    void GetEnableCoordiUpgradeList( std::vector<GCITEMUID>& vecItemList );
	bool IsChangeMeshCoordi( const int iSetItemID_ );
    void LoadSkillScroolList( void );
    bool IsSkillScroolList(DWORD dwGoodsID);
    int GetSkillID(GCITEMID dwGoodsID );

public:
	std::map<GCITEMID, int> m_mapLookExtendItemInfo;
	int m_nMaxLookInventorySlot;
	void SetLookExtendItemInfo(KEVENT_LOOK_INVENTORY_EXTEND_INFO_NOT& kRecv);
	bool IsLookExtendItem(GCITEMID dwGoodsID);
	int GetMaxLookExtendSlot() { return m_nMaxLookInventorySlot; };
	int GetLookExtendSize(GCITEMID dwGoodsID);

public:

	void GetInventoryPetGlyph(SCharInfo* pCharInfo, OUT std::vector<GCITEMUID>& vecGlyphs);
};

extern KGCItemManager* g_pItemMgr;

#endif // _GCITEMMANAGER_H_
