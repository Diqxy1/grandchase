#include "stdafx.h"
#include ".\KGCRepetitionEventBox.h"

//

//
//
//

#include "MyD3D.h"
#include "KGCEmoticon.h"

IMPLEMENT_CLASSNAME( KGCRepetitionEventBox );
IMPLEMENT_WND_FACTORY( KGCRepetitionEventBox );
IMPLEMENT_WND_FACTORY_NAME( KGCRepetitionEventBox, "gc_repetition_event_box" );

KGCRepetitionEventBox::KGCRepetitionEventBox(void)
{
    m_pkOk = NULL;
    m_pkBar = NULL;

    m_pkEventTitle = NULL;
    m_pkItemName = NULL;
    m_pkItemDesc = NULL;
    m_pkMessage = NULL;
    m_pkBackground = NULL;

    m_pTextureItem = NULL;

    LINK_CONTROL( "btn_ok", m_pkOk );
    LINK_CONTROL( "bar2", m_pkBar );
    LINK_CONTROL( "event_static", m_pkEventTitle );
    LINK_CONTROL( "item_info_name", m_pkItemName );
    LINK_CONTROL( "item_info_desc", m_pkItemDesc );
    LINK_CONTROL( "event_desc", m_pkMessage );
    LINK_CONTROL( "background", m_pkBackground );

    m_ItemID = 0;
	m_ItemUID = 0;
    m_iSetNumber = -1;
	m_cEnchantEquipGradeID = 0;
	m_cEnchantLevel = 0;
}

KGCRepetitionEventBox::~KGCRepetitionEventBox(void)
{
    SAFE_RELEASE( m_pTextureItem );
}

void KGCRepetitionEventBox::OnCreate( void )
{
    m_pkOk->InitState( true, true, this );
    m_pkEventTitle->InitState( true );
    m_pkItemName->InitState( true );
    m_pkItemDesc->InitState( true );
    m_pkMessage->InitState( true );

    m_pkEventTitle->SetAlign( DT_CENTER );
    
    m_pkItemName->SetFontColor( 0xFFFFFF00 );

    m_pkItemDesc->SetMultiLine( true );
    m_pkMessage->SetMultiLine( true );

    m_pkItemDesc->SetAutoMultiLineLimit(4);
    m_pkItemDesc->SetLineSpace( 1.1f );
    m_pkMessage->SetLineSpace( 1.1f );
    m_pkItemDesc->SetMultiLine( true );

    m_pkOk->SetHotKey( DIK_RETURN );
    m_pkOk->SetHotKey( DIK_ESCAPE );
	m_pkOk->SetHotKey( DIK_SPACE );

    this->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}

void KGCRepetitionEventBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkOk )
        {
            g_KDSound.Play( "31" );
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_OK ) );
        }
    }
}

void KGCRepetitionEventBox::PostChildDraw( void )
{
    // 샵 아이템 이미지를 그린다.
    if ( IsRenderOn() && m_ItemID != 0 )
    {		
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        {
            float fX = 17.0f * m_fWindowScaleX;
            float fY = 47.0f * m_fWindowScaleY;
            float fWidth = 76.0f * m_fWindowScaleX;
            float fHeight = 76.0f * m_fWindowScaleY;

            if( m_iSetNumber != -1 )
            {
                D3DXVECTOR4 vTexCoords( GetTextureCoords( m_iSetNumber ) );

				g_pGCDeviceManager2->DrawInScreen(m_pTextureItem,
					vPos.x + fX + fWidth * 0.1f, vPos.y + fY - fHeight * 0.2f,
					vPos.x + fX + fWidth * 0.9f, vPos.y + fY + fHeight,
					vTexCoords.x, vTexCoords.y, vTexCoords.z, vTexCoords.w, false,10.0f,0xffffffff);
                /*g_MyD3D->Draw_TL( vPos.x + fX + fWidth * 0.1f, vPos.y + fY - fHeight * 0.2f,
                    vPos.x + fX + fWidth * 0.9f, vPos.y + fY + fHeight,
                    vTexCoords.x, vTexCoords.y, vTexCoords.z, vTexCoords.w );*/
            }
            else
            {
                KGCEmoticon* kTmpEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_ItemID );
                if ( NULL != kTmpEmoticon && !g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem( m_ItemID ) )
                {
                    // 이모티콘 데이터라면!!!!
                    kTmpEmoticon->RenderForShop( vPos.x + fX, vPos.y + fY, vPos.x + fX + fWidth, vPos.y + fY + fHeight );
                }
                else
				{
					g_pGCDeviceManager2->DrawInScreen(m_pTextureItem,
						vPos.x + fX, vPos.y + fY,
						vPos.x + fX + fWidth, vPos.y + fY + fHeight,
						0.0f, 0.0f, 1.0f, 1.0f, false,10.0f,0xffffffff);
                    /*g_MyD3D->Draw_TL( vPos.x + fX, vPos.y + fY,
                                vPos.x + fX + fWidth, vPos.y + fY + fHeight,
                                0.0f, 0.0f, 1.0f, 1.0f );*/
				}
            }
        }


		if ( m_cEnchantLevel > 0 ) 
		{
			std::string strType = "normal_num_enchant";
			if( m_cEnchantEquipGradeID == KItem::GRADE_NORMAL )
				strType = "normal_num_enchant";
			else if( m_cEnchantEquipGradeID == KItem::GRADE_EPIC )
				strType = "epic_num_enchant";
			
			g_pkUIMgr->RenderNumber( strType, vPos + D3DXVECTOR2( 67 * m_fWindowScaleX, 84 * m_fWindowScaleY),
				m_cEnchantLevel, false, false ,-1.0f,true);
		}
    }
}

void KGCRepetitionEventBox::SetText( GCITEMID iItemID, GCITEMUID _itemUID, std::wstring strEventDesc, std::wstring strEventTitle )
{
    SAFE_RELEASE( m_pTextureItem );

    m_ItemID = iItemID / 10;
    m_ItemUID = _itemUID;
	m_cEnchantLevel = 0;
	m_cEnchantEquipGradeID = 0;

    GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( m_ItemID );
    RETURN_NIL( kItem );


	KItem* pInvenItem = g_MyD3D->m_kItemMgr.GetInventoryItem( m_ItemUID );
	if ( pInvenItem )
	{
		m_pkItemName->SetFontColor( g_MyD3D->m_kItemMgr.GetItemGradeColorRGB( pInvenItem->m_cGradeID ) );

		m_cEnchantLevel = pInvenItem->m_cEnchantLevel;
        if ( m_ItemID == KGCItemManager::ITEM_ENCHANT_STONE )
            m_cEnchantEquipGradeID = pInvenItem->m_cGradeID;
        else
		    m_cEnchantEquipGradeID = pInvenItem->m_cEnchantEquipGradeID;
	}

    m_pkItemDesc->SetMultiLine( true );
    m_pkItemDesc->SetAutoMultiLineLimit(4);

        

    m_pkEventTitle->SetText( strEventTitle );
    m_pkItemName->SetForceWordWrapText( kItem->strItemName, true );
    m_pkItemDesc->SetTextAutoMultiline( kItem->strItemDesc );
    m_pkMessage->SetTextAutoMultiline( strEventDesc );

    // 라인처리(설명 디폴트 4라인, 메시지 디폴트 1라인)
    int fDescLine = m_pkItemDesc->GetLineNum() - 4;
    int fMessageLine = m_pkMessage->GetLineNum() - 1;
    if( fDescLine < 0 ) fDescLine = 0;
    if( fMessageLine < 0 ) fMessageLine = 0;

    // 윈도우 크기 설정
    m_pkBackground->SetHeight( DWORD(((fDescLine + (fMessageLine*1.4f)) * 10.0f) + 200) );

    // 바 위치
    m_pkBar->SetWindowPosY( (fDescLine * 10.0f) + 135.0f );

    // 메시지 위치
    m_pkMessage->SetWindowPosY( (fDescLine * 10.0f) + 140.0f );

    // 확인버튼위치
#if defined(NATION_KOREA)
    m_pkOk->SetWindowPosY( ((fDescLine + (fMessageLine*1.4f)) * 10.0f) + 164.0f );
#else
    m_pkOk->SetWindowPosY( ((fDescLine + (fMessageLine*1.4f)) * 10.0f) + 160.0f );
#endif
    // 세트 아이템이라면??
    if( kItem->dwSlotPosition & ESP_SET_ITEM )
    {
        m_pTextureItem = g_pItemMgr->CreateShopSetItemTexture( kItem->iSetItemNumber ,kItem->dwGoodsID);
        m_iSetNumber = kItem->iSetItemNumber;
    }
    else
    {
        m_pTextureItem = g_pItemMgr->CreateItemTexture( m_ItemID, 0 );
        m_iSetNumber = -1;
    }
}

D3DXVECTOR4 KGCRepetitionEventBox::GetTextureCoords( int iSetNum )
{
    D3DXVECTOR4 m_vTexCoords;

    if ( 0 == iSetNum % 2 )
    {
        m_vTexCoords.x = 0.0f;
        m_vTexCoords.y = 0.0f;
        m_vTexCoords.z = 0.5f;
        m_vTexCoords.w = 1.0f;
    }
    else
    {
        m_vTexCoords.x = 0.5f;
        m_vTexCoords.y = 0.0f;
        m_vTexCoords.z = 1.0f;
        m_vTexCoords.w = 1.0f;
    }

    return m_vTexCoords;
}

bool KGCRepetitionEventBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    //                     아이템아이디,          아이템사이즈,        이벤트설명,        이벤트제목
    this->SetText( static_cast<GCITEMID>( kInfo_.m_lParam ), static_cast<GCITEMUID>(kInfo_.m_lParam2), kInfo_.m_strText1, kInfo_.m_strText2 );
	this->SetWindowZOrder( D3DWZ_TOPMOST );
    return true;
}