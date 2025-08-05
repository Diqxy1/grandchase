#include "stdafx.h"
#include ".\GCItemImgWnd.h"

IMPLEMENT_CLASSNAME( KGCItemImgWnd );
IMPLEMENT_WND_FACTORY( KGCItemImgWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCItemImgWnd, "gc_item_img_wnd" );

KGCItemImgWnd::KGCItemImgWnd()
: m_pkEquipEnchantItemLv(NULL)
{
    m_bShow = true;
    m_pTexture = NULL;
    m_dwItemID = 0;
    m_iSetID = -1;
    m_bLikeBtn = false;
    m_dwColor = 0xffffffff;
    m_pkCount = NULL;
    m_DesignCoordiID = 0;
	
	LINK_CONTROL("number_strong",m_pkEquipEnchantItemLv);
    LINK_CONTROL("number_count", m_pkCount );
}
KGCItemImgWnd::~KGCItemImgWnd()
{
    SAFE_RELEASE( m_pTexture );
}

void KGCItemImgWnd::OnCreate( void )
{
	if( m_pkEquipEnchantItemLv != NULL ) {
		m_pkEquipEnchantItemLv->InitState( true );
		m_pkEquipEnchantItemLv->SetStringType( "normal_num_enchant" );
		m_pkEquipEnchantItemLv->SetWriteToRight(false);
	}

    if ( m_pkCount ) 
    {
        m_pkCount->InitState( true );
        m_pkCount->SetWriteToRight(false);        
    }
}
void KGCItemImgWnd::FrameMoveInEnabledState( void )
{
}
void KGCItemImgWnd::PostDraw( void )
{	
    if ( m_bShow )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
        float fWidth = static_cast< float >( GetWidth() ) * m_fWindowScaleX;
        float fHeight = static_cast< float >( GetHeight() ) * m_fWindowScaleY;
        DWORD dwColor = m_dwColor;

        if( m_bLikeBtn )
        {
            if( D3DWM_HOVER == m_eMode )
            {
                dwColor = 0xffd0d0d0 & m_dwColor;
            }
            else if( D3DWM_SELECT == m_eMode )
            {
                dwColor = 0xffd0d0d0 & m_dwColor;
                vPos.x += (fWidth * 0.015f);
                vPos.y += (fHeight * 0.015f);
            }
        }

        //  세트 이미지이면 조금 작게 그린다.
        if ( m_iSetID != -1 ) 
        {
            float fGapX = fWidth * 0.2f;
            float fGapY = fHeight * 0.2f;

            vPos.x += fGapX * 0.5f;
            vPos.y += fGapY * 0.8f;

            fWidth -= fGapX;
            fHeight -= fGapY;
        }

        GCITEMID itemID = m_dwItemID;
        if ( m_DesignCoordiID != 0 )
        {
            itemID = m_DesignCoordiID;
        }


        KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( itemID );
        if ( NULL != kTmpEmoticon && !g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( itemID ) )
        {
            // 이모티콘 데이터라면!!!!
            kTmpEmoticon->RenderForShop( vPos.x, vPos.y, vPos.x + fWidth, vPos.y + fHeight );
        }
        else if( m_pTexture != NULL )
        {
            g_pItemMgr->Draw_ShopImage( vPos, fWidth, fHeight, m_pTexture, m_iSetID != -1, m_iSetID, dwColor, itemID );
        }
    }
}

void KGCItemImgWnd::SetItemTexture( GCDeviceTexture* pTex )
{
    m_pTexture = pTex;
}

void KGCItemImgWnd::SetItemInfo( DWORD dwGoodsID_, DWORD dwColor, bool bSetItem, int iSetID, GCITEMUID dwUID, int nCount, GCITEMID dwCoordiID )
{
    SAFE_RELEASE( m_pTexture );

    //========ItemResorceLoading====================
    m_dwItemID = dwGoodsID_;
    if( 0 == dwGoodsID_ )
    {
        m_bShow = false;
        return;
    }

    GCITEMID itemID = m_dwItemID;
    m_DesignCoordiID = dwCoordiID;

    if ( dwCoordiID != 0 )
    {
        itemID = m_DesignCoordiID;
    }



    KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( itemID );
    if ( NULL == kTmpEmoticon || g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( itemID ) )
    {
        if( bSetItem )
        {
            m_iSetID = iSetID;
            m_pTexture = g_pItemMgr->CreateShopSetItemTexture( iSetID, itemID );
        }
        else
        {
            m_iSetID = -1;
            m_pTexture = g_pItemMgr->CreateItemTexture( itemID, 0 );
        }
    }
    //===============================================

	if ( dwUID != 0 ) {
		GCItem *pkItemInfo = g_MyD3D->m_kItemMgr.GetItemData( dwGoodsID_ );
		KItem *pkInvenItem = g_pItemMgr->GetInventoryItem( dwUID );

		if( m_pkEquipEnchantItemLv != NULL )        
		{
			bool bCheck = false;
			if( pkItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
				bCheck = true;
			else if( pkItemInfo->eItemKind != GIK_ENCHANT_ENABLE_ITEM && pkInvenItem->m_EnchantEquipItemUID != 0 )
				bCheck = true;

			if( bCheck )
			{
				m_pkEquipEnchantItemLv->ToggleRender(true);

				if( pkItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
				{
					if( pkInvenItem->m_cGradeID == KItem::GRADE_NORMAL )
						m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
					else if( pkInvenItem->m_cGradeID == KItem::GRADE_EPIC )
						m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
				}
				else
				{
					if( pkInvenItem->m_cEnchantEquipGradeID == KItem::GRADE_NORMAL )
						m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
					else if( pkInvenItem->m_cEnchantEquipGradeID == KItem::GRADE_EPIC )
						m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
				}

				m_pkEquipEnchantItemLv->SetNumber(pkInvenItem->m_cEnchantLevel);
			}
			else
				m_pkEquipEnchantItemLv->ToggleRender(false);
		}
	}
	else {
		if( m_pkEquipEnchantItemLv != NULL )        
		{
			m_pkEquipEnchantItemLv->ToggleRender(false);
		}
	}

    if ( m_pkCount )
    {
        if ( nCount > -1 )
        {
            m_pkCount->ToggleRender( true );
            m_pkCount->SetNumber( nCount );
        }
        else
        {
            m_pkCount->ToggleRender( false );
        }
    }

    m_dwColor = dwColor;
}

void KGCItemImgWnd::SetCustomItemInfo(DWORD dwGoodsID_, DWORD dwColor, bool bSetItem, int iSetID, GCITEMUID dwUID, int nCount, GCITEMID dwCoordiID)
{
    SAFE_RELEASE(m_pTexture);

    //========ItemResorceLoading====================
    m_dwItemID = dwGoodsID_;
    if (0 == dwGoodsID_)
    {
        m_bShow = false;
        return;
    }

    GCITEMID itemID = m_dwItemID;
    m_DesignCoordiID = dwCoordiID;

    if (dwCoordiID != 0)
    {
        itemID = m_DesignCoordiID;
    }

    m_pTexture = g_pItemMgr->CreateItemTextureCustom(itemID, 0);

    //===============================================

    if (dwUID != 0) {
        GCItem* pkItemInfo = g_MyD3D->m_kItemMgr.GetItemData(dwGoodsID_);
        KItem* pkInvenItem = g_pItemMgr->GetInventoryItem(dwUID);

        if (m_pkEquipEnchantItemLv != NULL)
        {
            bool bCheck = false;
            if (pkItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM)
                bCheck = true;
            else if (pkItemInfo->eItemKind != GIK_ENCHANT_ENABLE_ITEM && pkInvenItem->m_EnchantEquipItemUID != 0)
                bCheck = true;

            if (bCheck)
            {
                m_pkEquipEnchantItemLv->ToggleRender(true);

                if (pkItemInfo->eItemKind == GIK_ENCHANT_ENABLE_ITEM)
                {
                    if (pkInvenItem->m_cGradeID == KItem::GRADE_NORMAL)
                        m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
                    else if (pkInvenItem->m_cGradeID == KItem::GRADE_EPIC)
                        m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
                }
                else
                {
                    if (pkInvenItem->m_cEnchantEquipGradeID == KItem::GRADE_NORMAL)
                        m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
                    else if (pkInvenItem->m_cEnchantEquipGradeID == KItem::GRADE_EPIC)
                        m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
                }

                m_pkEquipEnchantItemLv->SetNumber(pkInvenItem->m_cEnchantLevel);
            }
            else
                m_pkEquipEnchantItemLv->ToggleRender(false);
        }
    }
    else {
        if (m_pkEquipEnchantItemLv != NULL)
        {
            m_pkEquipEnchantItemLv->ToggleRender(false);
        }
    }

    if (m_pkCount)
    {
        if (nCount > -1)
        {
            m_pkCount->ToggleRender(true);
            m_pkCount->SetNumber(nCount);
        }
        else
        {
            m_pkCount->ToggleRender(false);
        }
    }

    m_dwColor = dwColor;
}

void KGCItemImgWnd::SetItemInfoLua( DWORD dwGoodsID_, bool bSetItem, int iSetID )
{
    SAFE_RELEASE( m_pTexture );
    //========ItemResorceLoading====================
    m_dwItemID = dwGoodsID_;
    KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_dwItemID );
    if ( NULL == kTmpEmoticon || g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( m_dwItemID ) )
    {
        if( bSetItem )
        {
            m_iSetID = iSetID;
            m_pTexture = g_pItemMgr->CreateShopSetItemTexture( iSetID,dwGoodsID_ );
        }
        else
        {
            m_iSetID = -1;
            m_pTexture = g_pItemMgr->CreateItemTexture( dwGoodsID_, 0 );
        }
    }
}

void KGCItemImgWnd::ShowItem( bool bShow_ )
{
    m_bShow = bShow_;
}

void KGCItemImgWnd::SetPetItemInfo( DWORD dwGoodsID_, int iEvolutionLv, DWORD dwColor /*= -1 */ )
{
    SAFE_RELEASE( m_pTexture );
    //========ItemResorceLoading====================
    m_dwItemID = dwGoodsID_;
    m_pTexture = g_pItemMgr->CreatePetItemTexture( dwGoodsID_, iEvolutionLv );
    //g_pItemMgr->CreateItemTexture( dwGoodsID_, -1, iEvolutionLv );
    //===============================================
    m_dwColor = dwColor;

    if( m_pkCount )
        m_pkCount->ToggleRender( false );
}

void KGCItemImgWnd::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    lua_tinker::class_add<KGCItemImgWnd>( L, "KGCItemImgWnd" );
    lua_tinker::class_def<KGCItemImgWnd>( L, "SetItemInfo",         &KGCItemImgWnd::SetItemInfo );
    lua_tinker::class_def<KGCItemImgWnd>( L, "SetItemInfoLua",         &KGCItemImgWnd::SetItemInfoLua );
    lua_tinker::class_def<KGCItemImgWnd>( L, "SetPetItemInfo",      &KGCItemImgWnd::SetPetItemInfo );
    lua_tinker::class_def<KGCItemImgWnd>( L, "SetItemColor",        &KGCItemImgWnd::SetColor );
    lua_tinker::class_def<KGCItemImgWnd>( L, "GetItemColor",        &KGCItemImgWnd::GetColor );
    lua_tinker::class_inh<KGCItemImgWnd,KD3DWnd>( L );
}