#include "StdAfx.h"
#include "GCItem.h"


GCItem::GCItem()      
: dwGoodsID(0), eItemKind(GIK_NORMAL), dwCharType(0), iNeedChangeJobLevelForEquippingItem(0),
iNeedLevelForEquippingItem(0), iSetItemNumber(-1), dwSlotPosition(0), dwSkillPosition(0), iAttack(0),
iDefence(0), iHealth(0), iLimitedLevelOfLevelRatioAbility(-1), fLevelRatioAttackRate(0), fLevelRatioDefenceRate(0),
fLevelRatioHealthRate(0), 
dwTargetSkill(0), eMoneyType(EMT_GAMEPOINT), dwPrice(0), eItemType(GCIT_PERIOD), iItemTypeValue(-1), bAutoDecrease(0), eAttribute(EIATTR_NONE),
iStrongLevel(0), bNewItem(false), bNotBuyItem(false), bNotSellItem(false),
ePetItemKind(EPET_KIND_INVALID), dwPetEquipItemSlot(0),
dwPetGoodsIDOfOriginalPetEquipItemOfSkill(0),
shPetEquipItemPromotionLevel(0), cEnchantLevel( -1 ), cItemGrade(0), cEnchantGrade(0), iCoordiShopPackage(-1 )
, iGrade(-1), iAttribute(0), shSocket(-1), shLevelDown(0), shContinentType(0)
{
}

GCItem::GCItem( const GCItem& src )
{
    *this = src;
}

GCItem::~GCItem( void )
{
}

int	GCItem::GetCharType()
{
    int iCharType;
    switch( dwCharType )
    {
    case ECT_ELESIS:
        iCharType =GC_CHAR_ELESIS;
        break;
    case ECT_LIRE:
        iCharType =GC_CHAR_LIRE;
        break;
    case ECT_ARME:
        iCharType =GC_CHAR_ARME;
        break;
    case ECT_LAS:
        iCharType =GC_CHAR_LAS;
        break;
    case ECT_RYAN:
        iCharType =GC_CHAR_RYAN;
        break;
    case ECT_RONAN:
        iCharType =GC_CHAR_RONAN;
        break;
    case ECT_AMY:
        iCharType = GC_CHAR_AMY;
        break;
    case ECT_JIN:
        iCharType = GC_CHAR_JIN;
        break;
    case ECT_SIEG:
        iCharType = GC_CHAR_SIEG;
        break;
    case ECT_MARI:
        iCharType = GC_CHAR_MARI;
        break;		
    case ECT_DIO:
        iCharType = GC_CHAR_DIO;
        break;		
    case ECT_ZERO:
        iCharType = GC_CHAR_ZERO;
        break;
    case ECT_LEY:
        iCharType = GC_CHAR_LEY;
        break;
    case ECT_LUPUS:
        iCharType = GC_CHAR_LUPUS;
        break;
    case ECT_RIN:
        iCharType = GC_CHAR_RIN;
        break;
    case ECT_ASIN:
        iCharType = GC_CHAR_ASIN;
        break;
    case ECT_LIME:
        iCharType = GC_CHAR_LIME;
        break;
    case ECT_EDEL:
        iCharType = GC_CHAR_EDEL;
        break;
    case ECT_BEIGAS:
        iCharType = GC_CHAR_BEIGAS;
        break;
	case ECT_UNO:
        iCharType = GC_CHAR_UNO;
		break;
#ifdef PERYTON // Kawan>
    case ECT_PERYTON:
        iCharType = GC_CHAR_PERYTON;
        break;
#endif
#ifdef ALICE 
    case ECT_ALICE:
        iCharType = GC_CHAR_ALICE;
        break;
#endif
#ifdef DECANEE 
    case ECT_DECANEE:
        iCharType = GC_CHAR_DECANEE;
        break;
#endif
    default:
        // 공용 아이템이 다 이쪽으로 들어온다;;;
        //ASSERT( !"Invalid CharType" );    
        iCharType = GC_CHAR_ELESIS;
        break;
    }

    return iCharType;
}

#define __EQUAL_OPERATOR__(p) this->p = src.p
const GCItem& GCItem::operator=( const GCItem& src )
{
    if ( this == &src )
        return *this;

    __EQUAL_OPERATOR__( strItemName );
    __EQUAL_OPERATOR__( strItemDesc );
    __EQUAL_OPERATOR__( dwGoodsID );
    __EQUAL_OPERATOR__( eItemKind );
    __EQUAL_OPERATOR__( dwCharType );
    __EQUAL_OPERATOR__( iNeedChangeJobLevelForEquippingItem );
    __EQUAL_OPERATOR__( iNeedLevelForEquippingItem );   
    __EQUAL_OPERATOR__( iSetItemNumber );		
    __EQUAL_OPERATOR__( dwSlotPosition );
    __EQUAL_OPERATOR__( dwSkillPosition );		
    __EQUAL_OPERATOR__( iAttack );
    __EQUAL_OPERATOR__( iDefence );
    __EQUAL_OPERATOR__( iHealth );
    __EQUAL_OPERATOR__( iLimitedLevelOfLevelRatioAbility );	    
    __EQUAL_OPERATOR__( fLevelRatioAttackRate );
    __EQUAL_OPERATOR__( fLevelRatioDefenceRate );
    __EQUAL_OPERATOR__( fLevelRatioHealthRate );	
    __EQUAL_OPERATOR__( dwTargetSkill );		
    __EQUAL_OPERATOR__( eMoneyType );
    __EQUAL_OPERATOR__( dwPrice );
    __EQUAL_OPERATOR__( eItemType );		
    __EQUAL_OPERATOR__( iItemTypeValue );
    
    __EQUAL_OPERATOR__( eAttribute );
    __EQUAL_OPERATOR__( iStrongLevel );     
    __EQUAL_OPERATOR__( ePetItemKind );		
    __EQUAL_OPERATOR__( dwPetEquipItemSlot );
    __EQUAL_OPERATOR__( dwPetGoodsIDOfOriginalPetEquipItemOfSkill );
    __EQUAL_OPERATOR__( iCoordiShopPackage );

    __EQUAL_OPERATOR__( iGrade );
    __EQUAL_OPERATOR__( iAttribute );
    __EQUAL_OPERATOR__( shSocket );
    __EQUAL_OPERATOR__( shLevelDown );

    __EQUAL_OPERATOR__( shPetEquipItemPromotionLevel );
    __EQUAL_OPERATOR__( cItemGrade );		

    __EQUAL_OPERATOR__( cEnchantLevel );
    __EQUAL_OPERATOR__( cEnchantGrade );

    __EQUAL_OPERATOR__( bAutoDecrease );		
    __EQUAL_OPERATOR__( bNewItem );
    __EQUAL_OPERATOR__( bNotBuyItem );
    __EQUAL_OPERATOR__( bNotSellItem ); 

    __EQUAL_OPERATOR__( m_vecSocketInfo );
    __EQUAL_OPERATOR__( m_vecAttribute );

    __EQUAL_OPERATOR__( shContinentType );
	__EQUAL_OPERATOR__( iRecomLevel );
    return *this;
}
const GCItem& GCItem::operator=( const GCItemData& src )
{
    __EQUAL_OPERATOR__( dwGoodsID );
    this->strItemName = src.szItemName;
    this->strItemDesc = src.szItemDesc;

    __EQUAL_OPERATOR__( eItemKind );
    __EQUAL_OPERATOR__( dwCharType );
    __EQUAL_OPERATOR__( iNeedChangeJobLevelForEquippingItem );
    __EQUAL_OPERATOR__( iNeedLevelForEquippingItem );
    __EQUAL_OPERATOR__( iSetItemNumber );
    __EQUAL_OPERATOR__( dwSlotPosition );
    __EQUAL_OPERATOR__( dwSkillPosition );
    __EQUAL_OPERATOR__( iAttack );
    __EQUAL_OPERATOR__( iDefence );
    __EQUAL_OPERATOR__( iHealth );  
    __EQUAL_OPERATOR__( iLimitedLevelOfLevelRatioAbility );
    __EQUAL_OPERATOR__( fLevelRatioAttackRate );
    __EQUAL_OPERATOR__( fLevelRatioDefenceRate );
    __EQUAL_OPERATOR__( fLevelRatioHealthRate );
    __EQUAL_OPERATOR__( dwTargetSkill );   
    __EQUAL_OPERATOR__( eMoneyType );
    __EQUAL_OPERATOR__( dwPrice );
    __EQUAL_OPERATOR__( eItemType );
    __EQUAL_OPERATOR__( bAutoDecrease );
    __EQUAL_OPERATOR__( iItemTypeValue );
    __EQUAL_OPERATOR__( eAttribute );
    __EQUAL_OPERATOR__( iStrongLevel );
    __EQUAL_OPERATOR__( bNewItem );
    __EQUAL_OPERATOR__( bNotBuyItem );
    __EQUAL_OPERATOR__( bNotSellItem );
    __EQUAL_OPERATOR__( ePetItemKind );
    __EQUAL_OPERATOR__( dwPetEquipItemSlot );
    __EQUAL_OPERATOR__( dwPetGoodsIDOfOriginalPetEquipItemOfSkill );
    __EQUAL_OPERATOR__( shPetEquipItemPromotionLevel );
    __EQUAL_OPERATOR__( cEnchantLevel );
    __EQUAL_OPERATOR__( cItemGrade );
    __EQUAL_OPERATOR__( cEnchantGrade );
	__EQUAL_OPERATOR__( iRecomLevel );
    return *this;
}
#undef __EQUAL_OPERATOR__