#include "stdafx.h"
#include "EnchantStrong.h"
#include "GCItemManager.h"


KEnchantStrong::KEnchantStrong(void)
{
    m_dwCoreItemID = 0;
	m_fRatio = 1.0f;
}

KEnchantStrong::~KEnchantStrong(void)
{
	ClearEnchantAssist();
	ClearEnchantLimitLv();
}

void KEnchantStrong::RegisterLuabind()
{
    lua_tinker::class_add<KEnchantStrong>( KGCLuabinder::getInstance().GetLuaState(), "KEnchantStrong" );
        lua_tinker::class_def<KEnchantStrong>( KGCLuabinder::getInstance().GetLuaState(),  "SetEnchantFailResult",    &KEnchantStrong::SetEnchantFailResult  );
        lua_tinker::class_def<KEnchantStrong>( KGCLuabinder::getInstance().GetLuaState(),  "SetCoreItemID",           &KEnchantStrong::SetCoreItemID  );
        lua_tinker::class_def<KEnchantStrong>( KGCLuabinder::getInstance().GetLuaState(),  "ClearEnchantLimitLv",     &KEnchantStrong::ClearEnchantLimitLv  );
        lua_tinker::class_def<KEnchantStrong>( KGCLuabinder::getInstance().GetLuaState(),  "ClearEnchantAssist",      &KEnchantStrong::ClearEnchantAssist  );
        lua_tinker::class_def<KEnchantStrong>( KGCLuabinder::getInstance().GetLuaState(),  "AddEnchantAssistItem",    &KEnchantStrong::AddEnchantAssistItem  );;

    lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "EnchantSystem", this );
}

void KEnchantStrong::ClearEnchantAssist()
{
    m_mapEnchantAssist.clear();
}

void KEnchantStrong::ClearEnchantLimitLv()
{
    m_mapEnchantLimitLv.clear();
}


void KEnchantStrong::SetEnchantFailResult( const UINT uiLv_, const UINT uiResult_ )
{
    if ( m_mapEnchantLimitLv.find( uiLv_ ) == m_mapEnchantLimitLv.end() )
    {
        m_mapEnchantLimitLv[uiLv_] = uiResult_;
    }
}
//====================================================================================
// 2007-10-15 오후 7:28:03 : Asirion 
// 이 레벨에서 실패 했을경우 어떠한 결과를 나타내는가?
// EA_SUCCESS의 경우 실패했을때도 결과는 EA_SUCCESS이다.
KEnchantStrong::ENCHANT_ACTION KEnchantStrong::GetEnchantFailResult( const UINT uiEnchantLv_ )
{
    static std::map< UINT, UINT >::iterator s_mit;
    s_mit = m_mapEnchantLimitLv.find( uiEnchantLv_ );
    if ( s_mit == m_mapEnchantLimitLv.end() )
        return KEnchantStrong::EA_BREAK;
    return (KEnchantStrong::ENCHANT_ACTION)s_mit->second;
}

void KEnchantStrong::SetCoreItemID( const DWORD dwItemID_ )
{
    m_dwCoreItemID = dwItemID_;
}

void KEnchantStrong::AddEnchantAssistItem( const DWORD dwItemID_, const float fIncreaseRate_ )
{
    assert( m_mapEnchantAssist.find( dwItemID_ ) == m_mapEnchantAssist.end() );
    m_mapEnchantAssist.insert( std::make_pair( dwItemID_, fIncreaseRate_ ) );
}

bool KEnchantStrong::IsEnchantAssistItem( const DWORD dwItemID_ )
{
    return m_mapEnchantAssist.find( dwItemID_ ) != m_mapEnchantAssist.end();
} 

GCItemAbility KEnchantStrong::GetAbility( KItem* pInvenItem_ )
{
    GCItemAbility kAbility;
    if( NULL == pInvenItem_ )
        return kAbility;

	GCItem* pItem = g_pItemMgr->GetItemData( pInvenItem_->m_ItemID);
	if ( pItem == NULL ) //잘못된 값이 들어오면 0을 리턴.
		return kAbility;

    char cEnchantLv = pInvenItem_->m_cEnchantLevel;
    kAbility = GetAbility( pItem, cEnchantLv );

    return kAbility;
}

GCItemAbility KEnchantStrong::GetAbility( SUserItem* pUserItem_ )
{
	KItem kInvenItem;
	kInvenItem.m_cEnchantLevel = pUserItem_->cEnchantLevel;
	kInvenItem.m_ItemUID = pUserItem_->dwItemUID;
	kInvenItem.m_ItemID = pUserItem_->iItemID;
    kInvenItem.m_sEquipLevel = pUserItem_->nEquipLevel;
	return GetAbility( &kInvenItem );
}

GCItemAbility KEnchantStrong::GetAbility( GCItem* pItem_, char cEnchantLevel_)
{
    GCItemAbility kAbility;
    float fEnchantLv = static_cast<float>(cEnchantLevel_);
    if ( pItem_->dwSlotPosition & ESP_HELMET_SOCKET || pItem_->dwSlotPosition & ESP_JACKET_SOCKET )
    {
        switch( (int)pItem_->cEnchantGrade )
        {
        case GEG_OLD_BRAVE: kAbility.iDef = (int)( powf( fEnchantLv, 2.0f )/9.0f + fEnchantLv*5.0f/9.0f ) + pItem_->iDefence; break;
        case GEG_OLD_HOLY: kAbility.iDef = (int)( powf( fEnchantLv, 2.0f )/5.0f + fEnchantLv*2.0f/5.0f ) + pItem_->iDefence; break;
        case GEG_OLD_GLORY: kAbility.iDef = (int)( powf( fEnchantLv, 2.0f )/3.0f + fEnchantLv/5.0f ) + pItem_->iDefence; break;
        case GEG_BRAVE: kAbility.iDef = (int)( powf( fEnchantLv, 2.0f )/9.0f + fEnchantLv*8.0f/9.0f ) + pItem_->iDefence; break;
        case GEG_HOLY: kAbility.iDef = (int)( powf( fEnchantLv, 2.0f )/5.0f + fEnchantLv*4.0f/5.0f ) + pItem_->iDefence; break;
        case GEG_GLORY: kAbility.iDef = (int)(powf(fEnchantLv, 2.0f) / 3.0f + fEnchantLv) + pItem_->iDefence; break;
        }
    }
    else if ( pItem_->dwSlotPosition & ESP_WEAPON_SOCKET )
    {
        switch( (int)pItem_->cEnchantGrade )
        {
        case GEG_OLD_BRAVE: kAbility.iAtk = (int)( powf( fEnchantLv, 2.0f )*4.0f/7.0f + (fEnchantLv-0.1f)/10.0f + pItem_->iAttack ); break;
        case GEG_OLD_HOLY: kAbility.iAtk = (int)( powf( fEnchantLv, 2.0f )*2.0f/3.0f + (fEnchantLv-0.1f)/10.0f + pItem_->iAttack ); break;
        case GEG_OLD_GLORY: kAbility.iAtk = (int)( powf( fEnchantLv, 2.0f ) + fEnchantLv + pItem_->iAttack ); break;
        case GEG_BRAVE: kAbility.iAtk = (int)( powf( fEnchantLv, 2.0f )*3.0f/5.0f + (fEnchantLv-0.1f)/4.0f + pItem_->iAttack ); break;
        case GEG_HOLY: kAbility.iAtk = (int)( powf( fEnchantLv, 2.0f ) + (fEnchantLv-0.1f)/10.0f + pItem_->iAttack ); break;
        case GEG_GLORY: kAbility.iAtk = (int)(powf(fEnchantLv, 2.0f) * 3.0f / 2.0f + fEnchantLv + pItem_->iAttack); break;
        }
        kAbility.iAtk = (int)ceilf( (float)kAbility.iAtk/2.0f );
    }

    //아이템 능력치 개편하면서 룬의 능력치를 3배 올려준다.
    kAbility.iAtk *= 6;
    kAbility.iDef *= 6;
    kAbility.iVit *= 6;

    return kAbility;
}

UINT KEnchantStrong::GetNeedCoreCount( const DWORD dwItemID_, const UINT uiEnchantLv_ )
{
	GCItem* pItem = g_pItemMgr->GetItemData( dwItemID_);
	int iItemGrade = (int)pItem->cEnchantGrade < GEG_BRAVE ? (int)pItem->cEnchantGrade : (int)pItem->cEnchantGrade - GEG_BRAVE;	
	float iNeedCoreCount = (iItemGrade + 1.0f)*( uiEnchantLv_ + 1.0f );
	iNeedCoreCount *= m_fRatio;	// 레인보우 이벤트 때문
	iNeedCoreCount += 0.5f;
	
	return static_cast<UINT>(iNeedCoreCount);
}

void KEnchantStrong::SetSecondaryRatio( const float fRatio_ )
{
	m_fRatio = fRatio_;
}
