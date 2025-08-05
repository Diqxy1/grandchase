#include "stdafx.h"
#include "P2P.h"
#include "../uifx/KInput.h"
#include "GCDescBox.h"
#include "GCSizingThreeBox.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
#include "KGCGPCapsuleItemList.h"
#include "KGCGPCapsuleDlg.h"
#include "./gcui/GCItemInfoBox.h"
#include "KDSound.h"

IMPLEMENT_CLASSNAME( KGCGPCapsuleItemList );
IMPLEMENT_WND_FACTORY( KGCGPCapsuleItemList );
IMPLEMENT_WND_FACTORY_NAME( KGCGPCapsuleItemList, "gc_gpcapsule_itemlist" );

KGCGPCapsuleItemList::KGCGPCapsuleItemList(void)
: m_iCharType( 0 )
, m_iItemKind( 0 )
, m_iCapsuleIndex( 0 )
, m_iPrice( 0 )
, m_iCurPage( 1 )
, m_iParticleTime( 0 )
, m_iRandomItem( 0 )
, m_iObtainItemID( 0 )
{
	m_pTextureItem = NULL;

	m_vecItemList.clear();
	m_pkOpenCapsuleBtn	= NULL;
	m_pkLine			= NULL;
	m_pkTitle			= NULL;
	m_pkTitleLevel		= NULL;
	m_pkTitleItemName	= NULL;
	m_pkPrevPage		= NULL;
	m_pkNextPage		= NULL;
	m_pkCapsuleDesc0	= NULL;
	m_pkSlash			= NULL;
	m_pkCurPage			= NULL;
	m_pkTotalPage		= NULL;

	LINK_CONTROL( "btn_open_capsule",	m_pkOpenCapsuleBtn );
	LINK_CONTROL( "vertical_line",		m_pkLine );
	LINK_CONTROL( "title_capsule_item",	m_pkTitle );
	LINK_CONTROL( "subtitle_level",		m_pkTitleLevel );
	LINK_CONTROL( "subtitle_itemname",	m_pkTitleItemName );
	LINK_CONTROL( "btn_prev_page",		m_pkPrevPage );
	LINK_CONTROL( "btn_prev_next",		m_pkNextPage );
	LINK_CONTROL( "static_desc0",		m_pkCapsuleDesc0 );
	LINK_CONTROL( "static_slash",		m_pkSlash );
	LINK_CONTROL( "current_page",		m_pkCurPage );
	LINK_CONTROL( "total_page",			m_pkTotalPage );

	m_pkItemInfoBox             = NULL;
	LINK_CONTROL( "item_info_box", m_pkItemInfoBox );

	for( int i = 0 ; i < MAX_ITEM_LINE ; i++ )
	{
		m_pkLevelText[i] = NULL;
		std::stringstream str;
		str<<"static_level"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkLevelText[i] );

		m_pkItemNameText[i] = NULL;
		str.str("");
		str<<"static_item_name"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkItemNameText[i] );

		m_pkObtainedItem[i] = NULL;
		str.str("");
		str<<"obtained_item"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkObtainedItem[i] );
		
		m_pkItemFactorText[i] = NULL;
		str.str("");
		str<<"static_item_factor"<<i;
		LINK_CONTROL( str.str().c_str(), m_pkItemFactorText[i] );
	}	

}

KGCGPCapsuleItemList::~KGCGPCapsuleItemList(void)
{
	m_vecItemList.clear();
	SAFE_RELEASE(m_pTextureItem);
}

void KGCGPCapsuleItemList::OnCreate( void )
{
	m_pkOpenCapsuleBtn->InitState( true, true, this );
	m_pkPrevPage->InitState( true, true, this );
	m_pkNextPage->InitState( true, true, this );
	m_pkLine->InitState( true );
	m_pkTitle->InitState( true );
	m_pkTitleLevel->InitState( true );
	m_pkTitleItemName->InitState( true );
	m_pkItemInfoBox->InitState( false );

	for( int i = 0 ; i < MAX_ITEM_LINE ; i++ )
	{
		m_pkLevelText[i]->InitState( true );
		m_pkLevelText[i]->SetAlign( DT_CENTER );
		m_pkItemNameText[i]->InitState( true );
		m_pkItemNameText[i]->SetAlign( DT_CENTER );

		m_pkObtainedItem[i]->InitState( false );
		m_pkItemFactorText[i]->InitState( true );
		m_pkItemFactorText[i]->SetAlign( DT_CENTER );
	}

	m_pkCapsuleDesc0->InitState( true );
	m_pkSlash->InitState( true );
	m_pkSlash->SetAlign( DT_CENTER );	
	m_pkSlash->SetText( L"/" );	
	m_pkCurPage->InitState( true );
	m_pkCurPage->SetAlign( DT_CENTER );	
	m_pkTotalPage->InitState( true );
	m_pkTotalPage->SetAlign( DT_CENTER );
	m_pkTotalPage->SetText( GetMaxPage() );	
}

void KGCGPCapsuleItemList::UpdatePage()
{
	MovePage(m_iCurPage);
}

void KGCGPCapsuleItemList::FrameMoveInEnabledState( void )
{
	if( m_iParticleTime > 0 )
	{
		m_iParticleTime--;

		D3DXVECTOR2 vPos( GetFixedWindowPos() );	
		D3DXVECTOR2 vPos2(0.0f, 0.0f);
		D3DXVECTOR2 vPos3( -53.0f, 159.0f );

		if( m_iParticleTime == 100 )
		{
			SAFE_RELEASE( m_pTextureItem );			
			m_pTextureItem = g_pItemMgr->CreateShopItemTexture(m_iObtainItemID);
		}
		else if( m_iParticleTime > 15 && m_iParticleTime <= 80 )
		{
			if( m_iParticleTime % 20 == 0 )
				g_KDSound.Play( "219" );
		}
		else if( m_iParticleTime == 0 )
		{						
			SAFE_RELEASE( m_pTextureItem );
			bool bIsSuccess = false;
			for( int i = 0 ; i < (int)m_vecItemList.size() ; i++ )
			{
				if( (int)m_vecItemList[i].first.m_ItemID / 10 == m_iObtainItemID )
				{
					bIsSuccess = true;
					break;
				}
			}
			if( bIsSuccess )
				g_KDSound.Play( "GPC_OBTAIN_RARE_ITEM" );				
			else
				g_KDSound.Play( "GPC_OBTAIN_GP_ITEM" );
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GP_CAPSULE_OBTAIN_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
		}
		else
		{
			if( m_iParticleTime == 199 )
				g_KDSound.Play( "GPC_SHAKE_CAPSULE" );

			if( !o5 && m_iParticleTime > 140 )
			{
				SAFE_RELEASE( m_pTextureItem );
				if( m_iRandomItem < (int)m_vecItemList.size() - 1 )
				{
					m_iRandomItem++;
				}
				else
				{
					m_iRandomItem = 0;
				}

				m_pTextureItem = g_pItemMgr->CreateShopItemTexture(m_vecItemList[m_iRandomItem].first.m_ItemID / 10);
			}		
			else if( !o10 && m_iParticleTime <= 140 && m_iParticleTime > 100 )
			{
				SAFE_RELEASE( m_pTextureItem );
				if( m_iRandomItem < (int)m_vecItemList.size() - 1 )
				{
					m_iRandomItem++;
				}
				else
				{
					m_iRandomItem = 0;
				}

				m_pTextureItem = g_pItemMgr->CreateShopItemTexture(m_vecItemList[m_iRandomItem].first.m_ItemID / 10);
			}
		}

		return;
	}
	else
	{
		SAFE_RELEASE( m_pTextureItem );
	}

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	for( int i = 0 ; i < MAX_CAPSULE_NUM ; i++ )
	{
		if( m_pkPrevPage->CheckPosInWindowBound(vMousePos) )
		{
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				g_KDSound.Play( "GPC_BUTTON_CLICK" );
				m_iCurPage--;
				if( m_iCurPage != 0 )
				{
					MovePage( m_iCurPage );
				}
				else
				{
					m_iCurPage = 1;
				}
				break;
			}
		}
		else if( m_pkNextPage->CheckPosInWindowBound(vMousePos) )
		{
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				g_KDSound.Play( "GPC_BUTTON_CLICK" );
				m_iCurPage++;
				if( m_iCurPage <= GetMaxPage() )
				{
					MovePage( m_iCurPage );
				}
				else
				{
					m_iCurPage = GetMaxPage();
				}
				break;
			}
		}
	}

	for( int i = 0 ; i < MAX_ITEM_LINE ; i++ )
	{
		if( m_pkItemNameText[i]->CheckPosInWindowBound(vMousePos) && m_pkItemNameText[i]->IsRenderOn() )
		{
			DrawItemInfoBox(i);
			break;
		}
		else
		{
			m_pkItemInfoBox->ToggleRender(false);
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
		}
	}
}

void KGCGPCapsuleItemList::ActionPerformed( const KActionEvent& event )
{
	if( m_iParticleTime != 0 )
		return;

	GCWND_MSG_MAP( m_pkOpenCapsuleBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnOpenCapsule );
}

void KGCGPCapsuleItemList::OnCreateComplete()
{
}

void KGCGPCapsuleItemList::OnDestroyComplete()
{
	m_pkItemInfoBox->ToggleRender(false);
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}


void KGCGPCapsuleItemList::PostChildDraw()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

		g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_0 );
		g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_1 );
		g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_2 );
		g_ParticleManager->Render( GC_LAYER_UPPER_SQUARE_UI_3 );
	}
	g_pGCDeviceManager2->PopState();

	if( m_pTextureItem )
	{
		D3DXVECTOR2 vPos( GetRelocatedWindowPos() );	
		D3DXVECTOR2 vPos2(0.0f, 0.0f);
		D3DXVECTOR2 vPos3( -53.0f * GC_SCREEN_DIV_WIDTH, 159.0f * GC_SCREEN_DIV_WIDTH );

		if( m_iParticleTime == 100 )
		{
			vPos2.x = 0.0f * GC_SCREEN_DIV_WIDTH;
			vPos2.y = 0.0f * GC_SCREEN_DIV_WIDTH;
		}
		else if( m_iParticleTime > 80 && m_iParticleTime < 100 )
		{
			vPos2.x = 0.0f * GC_SCREEN_DIV_WIDTH;
			vPos2.y = 0.0f * GC_SCREEN_DIV_WIDTH;
		}
		else if( m_iParticleTime > 15 && m_iParticleTime <= 80 )
		{	
			vPos2.x = 76.0f * GC_SCREEN_DIV_WIDTH;
			vPos2.y = 76.0f * GC_SCREEN_DIV_WIDTH;
			vPos3.x = -58.0f * GC_SCREEN_DIV_WIDTH;
			vPos3.y = 174.0f * GC_SCREEN_DIV_WIDTH;
		}
		else if( m_iParticleTime <= 15 && m_iParticleTime > 0)
		{
			vPos2.x = 0.0f;
			vPos2.y = 0.0f;
		}
		else
		{
			vPos2.x = 50.0f * GC_SCREEN_DIV_WIDTH;
			vPos2.y = 50.0f * GC_SCREEN_DIV_WIDTH;

			if( m_iParticleTime == 199 )
				g_KDSound.Play( "GPC_SHAKE_CAPSULE" );

			if( !o5 && m_iParticleTime > 140 )
			{
				SAFE_RELEASE( m_pTextureItem );
				if( m_iRandomItem < (int)m_vecItemList.size() - 1 )
				{
					m_iRandomItem++;
				}
				else
				{
					m_iRandomItem = 0;
				}

				vPos2.x *= m_fWindowScaleX;
				vPos2.y *= m_fWindowScaleY;

				vPos3.x *= m_fWindowScaleX;
				vPos3.y *= m_fWindowScaleY;

				g_pItemMgr->Draw_ShopImage( vPos + vPos3 , vPos2.x, vPos2.y, m_pTextureItem, false);
			}		
			else if( !o10 && m_iParticleTime <= 140 && m_iParticleTime > 100 )
			{
				SAFE_RELEASE( m_pTextureItem );
				if( m_iRandomItem < (int)m_vecItemList.size() - 1 )
				{
					m_iRandomItem++;
				}
				else
				{
					m_iRandomItem = 0;
				}

				m_pTextureItem = g_pItemMgr->CreateShopItemTexture(m_vecItemList[m_iRandomItem].first.m_ItemID / 10);
			}
		}
		g_pItemMgr->Draw_ShopImage( vPos + vPos3, vPos2.x, vPos2.y, m_pTextureItem, false);
	}
}

void KGCGPCapsuleItemList::SetCapsuleItemList( std::vector<std::pair<KDropItemInfo,int>>& vecItemList_, int iCharType_, int iItemKind_, int iCapsuleIndex_, int iPrice_)
{
	m_vecItemList.clear();
	std::vector<std::pair<KDropItemInfo,int>>::iterator itor;
	for( itor = vecItemList_.begin(); itor != vecItemList_.end(); ++itor )
	{
		if( itor->second != IG_BASE )
			m_vecItemList.push_back(*itor);
	}

	m_iCharType		= iCharType_;
	m_iItemKind		= iItemKind_;
	m_iCapsuleIndex	= iCapsuleIndex_;
	m_iPrice		= iPrice_;
}

void KGCGPCapsuleItemList::OnOpenCapsule()
{
	g_KDSound.Play( "GPC_BUTTON_CLICK" );
	int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;

	// 인벤토리가 제한 수량을 초과하면 캡슐열기를 막는다.
	if ( g_MyD3D->m_kItemMgr.GetTotalInventorySize() > iInventoryCapacity )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_GP_CAPSULE_FULL_INVENTORY), L"", 
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
		return;
	}

	KP2P::GetInstance()->Send_OpenCapsuleReq( m_iCharType, m_iItemKind, m_iCapsuleIndex, m_iPrice );

	g_pkUIScene->m_pkGPCapsuleDlg->LockAll(true);
	this->LockAll(true);
	m_pTextureItem = g_pItemMgr->CreateItemTexture(m_vecItemList[0].first.m_ItemID / 10);
}

void KGCGPCapsuleItemList::MovePage( int iPage_ )
{
	m_iCurPage = iPage_;
	m_pkCurPage->SetText( iPage_ );
	m_pkTotalPage->SetText( GetMaxPage() );

	if( iPage_ == GetMaxPage() )
	{
		if( (int)m_vecItemList.size() <= MAX_ITEM_LINE )
			DrawItemList(iPage_, (int)m_vecItemList.size() );			
		else if( (int)m_vecItemList.size() % MAX_ITEM_LINE == 0 ) 
			DrawItemList(iPage_, MAX_ITEM_LINE );			
		else
			DrawItemList(iPage_, (int)m_vecItemList.size() % MAX_ITEM_LINE );			
	}
	else
		DrawItemList(iPage_, MAX_ITEM_LINE );
}

void KGCGPCapsuleItemList::DrawItemList( int iPage_, int iCurPageItemListNum_ )
{
	int iStartIndex = iPage_ * MAX_ITEM_LINE - MAX_ITEM_LINE;

	if( iStartIndex > (int)m_vecItemList.size() )
		return;

	WCHAR strText[20];

	for( int i = 0 ; i < iCurPageItemListNum_ ; i++ )
	{
		GCItem* kItem = g_pItemMgr->GetItemData( m_vecItemList[i + iStartIndex ].first.m_ItemID / 10 );
		m_pkLevelText[i]->SetText( kItem->iNeedLevelForEquippingItem );
		m_pkLevelText[i]->ToggleRender(true);

		//보유하고 있는 아이템인지 검사하고 가진 아이템이면 표시해준다.
		if( g_pItemMgr->FindInventoryForItemID( m_vecItemList[i + iStartIndex ].first.m_ItemID / 10 ))
		{
			m_pkObtainedItem[i]->ToggleRender(true);
		}
		else
		{
			m_pkObtainedItem[i]->ToggleRender(false);
		}

		//수량제 아이템이면 표시를 해준다.
		if( m_vecItemList[i + iStartIndex ].first.m_nDuration != -1 )
		{
			swprintf(strText, 20, L"(%d)",m_vecItemList[i + iStartIndex ].first.m_nDuration );
			m_pkItemFactorText[i]->SetText(strText);				
		}
		else
		{
			m_pkItemFactorText[i]->SetText();				
		}


		if( m_vecItemList[i + iStartIndex ].second == IG_RARE )
		{
			m_pkItemNameText[i]->SetFontColor(0xFFFFFF00);
		}
		else if( m_vecItemList[i + iStartIndex ].second == IG_EVENT )
		{
			m_pkItemNameText[i]->SetFontColor(0xFFFF8C00);
		}
		
		m_pkItemNameText[i]->SetText( kItem->strItemName );
		m_pkItemNameText[i]->ToggleRender( true );
	}

	//라인에 출력될 아이템이 없는 텍스트는 그리지 않는다.
	if( iCurPageItemListNum_ < MAX_ITEM_LINE )
	{
		int iNotRenderLine = MAX_ITEM_LINE - iCurPageItemListNum_;

		for( int i = 1 ; i <= iNotRenderLine ; i++ )
		{
			m_pkItemNameText[ MAX_ITEM_LINE - i ]->ToggleRender(false);
			m_pkLevelText[ MAX_ITEM_LINE - i ]->ToggleRender(false);
			m_pkObtainedItem[ MAX_ITEM_LINE - i ]->ToggleRender(false);
			m_pkItemFactorText[MAX_ITEM_LINE - i ]->SetText();				
		}
	}
	m_pkCapsuleDesc0->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_GP_CAPSULE_DESC, "i", m_iPrice ) ) ;
}

int KGCGPCapsuleItemList::GetMaxPage()
{
	if( (int)m_vecItemList.size() % MAX_ITEM_LINE == 0 )
	{
		if( m_vecItemList.size() <= MAX_ITEM_LINE )
			return 1;
		else
			return m_vecItemList.size() / MAX_ITEM_LINE;
	}
	else
	{
		return (int)((int)m_vecItemList.size() / MAX_ITEM_LINE + 1);
	}
}

void KGCGPCapsuleItemList::DrawItemInfoBox( int iLineIndex_ )
{
	int iStartIndex = m_iCurPage * MAX_ITEM_LINE - MAX_ITEM_LINE;	

	GCItem* kItem = g_pItemMgr->GetItemData(m_vecItemList[iLineIndex_ + iStartIndex ].first.m_ItemID / 10);	

	D3DXVECTOR2 vecPos;

    vecPos.x = 300.0f * m_fWindowScaleX;
    vecPos.y = ( 100.0f + iLineIndex_ * 20.0f ) * m_fWindowScaleY;

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );

    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
}
