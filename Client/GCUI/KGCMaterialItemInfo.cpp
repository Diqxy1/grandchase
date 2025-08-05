#include "stdafx.h"
#include "KGCMaterialItemInfo.h"

IMPLEMENT_CLASSNAME( KGCMaterialItemInfo );
IMPLEMENT_WND_FACTORY( KGCMaterialItemInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCMaterialItemInfo, "gc_material_item_info" );

KGCMaterialItemInfo::KGCMaterialItemInfo()
: m_pkItemImg( NULL )
, m_pkItemCount( NULL )
, m_pkItemSelBtn( NULL )
, m_dwItemID( 0 )
, m_uItemUID( 0 )
, m_nCount( 0 )
, m_nInvCount( 0 )
, m_bEnableSelection( false )
, m_nLevel ( 0 )
{
	LINK_CONTROL( "item_img", m_pkItemImg );
	LINK_CONTROL( "static_count", m_pkItemCount );
	LINK_CONTROL( "item_select_btn", m_pkItemSelBtn );
}

KGCMaterialItemInfo::~KGCMaterialItemInfo()
{
}

void KGCMaterialItemInfo::ActionPerformed( const KActionEvent& e)
{
	if(e.m_pWnd == this)
	{
		if( e.m_dwCode == EWNDMESSAGE_CLOSE )
		{
			return;
		}
	}
}

void KGCMaterialItemInfo::OnClickItemSelect( const KActionEvent& event )
{
	D3DXVECTOR2 dxvPos = event.m_pWnd->GetCurrentWindowPos();
	dxvPos.y += 63.0f;

	g_pkUIScene->m_pkItemCreationSelectBox->SetPos( dxvPos );
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ITEM_CREATION_SELECT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL , m_dwItemID, m_nLevel, true, true, false, false);
}

void KGCMaterialItemInfo::SetItem(GCITEMID dwItemID, DWORD dwImgColor, DWORD dwCostColor)
{
	this->m_dwItemID = dwItemID;
	
	if( dwItemID > 0 )
	{
		m_pkItemImg->SetItemInfo(dwItemID, dwImgColor);
		m_pkItemImg->ShowItem( true );
		m_pkItemCount->SetFontColor( dwCostColor );
		m_pkItemCount->ToggleRender( true );
		m_dwImageColor = dwImgColor;
		m_dwCostColor = dwCostColor;
	}
	else
	{
		m_pkItemImg->ShowItem( false );
		m_pkItemCount->ToggleRender( false );
		m_pkItemSelBtn->ToggleRender( false );
		ItemSetCount( -1 );
	}
}

void KGCMaterialItemInfo::ItemSetCount(int iCount)
{
	m_nCount = iCount;

	if( m_nCount == -1 )
	{
		m_pkItemCount->ToggleRender( false );
		return;
	}

	int nInvCount = g_pItemMgr->GetInventoryItemDuration(m_dwItemID);
	
	if(SiGCForgeManager()->IsEnableBreakUp() == false)
	{
		std::wstringstream str;
		str << nInvCount << L"/" << iCount;	
		m_pkItemCount->SetText(str.str());
	}
	else
	{
		m_pkItemCount->SetNumber(nInvCount);
	}
}

void KGCMaterialItemInfo::EnableSelection(bool bEnable)
{
	if(m_pkItemSelBtn)
	{
		m_pkItemSelBtn->ToggleRender( bEnable );
	}
}

void KGCMaterialItemInfo::OnCreate()
{
	if(m_pkItemImg)
	{
		m_pkItemImg->InitState( true );
	}

	if( m_pkItemCount )
	{
		m_pkItemCount->InitState( true );
		m_pkItemCount->SetAlign( DT_RIGHT );
	}

	if( m_pkItemSelBtn )
	{
		m_pkItemSelBtn->InitState( false, true, this );
		m_pkItemSelBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMaterialItemInfo::OnClickItemSelect);
	}
}

void KGCMaterialItemInfo::FrameMoveInEnabledState()
{
	if( m_dwItemID == 0 )
	{
		return;
	}

	if( CheckMousePosInWindowBound() )
	{
		KItem* pInvItem = NULL;
		GCItem* pItem = NULL;
		
		if( g_pItemMgr->FindInventory(m_uItemUID, &pInvItem, &pItem) == false )
		{
			pItem = g_pItemMgr->GetItemData( GetItemID() );
		}
		
		if( !pItem && !pInvItem )
		{
			return;
		}

		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, pInvItem, NULL, NULL, -1, m_nLevel );
		g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

		D3DXVECTOR2 vecPos( GetCurrentWindowPos() );
		vecPos.x += GetWidth();
		vecPos.x *= m_fWindowScaleX;
		vecPos.y *= m_fWindowScaleY;

		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );            
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	}
}