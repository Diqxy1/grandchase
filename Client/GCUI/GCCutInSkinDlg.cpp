#include "stdafx.h"
#include "GCCutInSkinDlg.h"
#include "GCCoordiViewItemWnd.h"

IMPLEMENT_CLASSNAME( KGCCutInSkinDlg );
IMPLEMENT_WND_FACTORY( KGCCutInSkinDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCutInSkinDlg, "gc_cutindlg" );

KGCCutInSkinDlg::KGCCutInSkinDlg( void )
: m_pkStaticExplanation( NULL )
, m_pkScrollbar( NULL )
{
	LINK_CONTROL( "scroll", m_pkScrollbar );
	LINK_CONTROL( "explanation", m_pkStaticExplanation );
}

KGCCutInSkinDlg::~KGCCutInSkinDlg( void )
{
}

void KGCCutInSkinDlg::OnCreate( void )
{
	if (m_pkScrollbar)
	{
		m_pkScrollbar->InitState(true, true, this);
		m_pkScrollbar->InitScroll(0, 1, 0, 1, 0, 0, 1, true);
	}
	CreateCutInSkinSlot();
}

void KGCCutInSkinDlg::FilterRemoveOtherChar()
{
	m_vecCutInSkinInfo.clear();
	m_vecCutInSkinInfo = g_pCutInSkinMgr->GetCutInSkinList();
	for (auto it = m_vecCutInSkinInfo.begin(); it != m_vecCutInSkinInfo.end();)
	{
		GCItem* fInvenItem = g_pItemMgr->GetItemData( it->m_iItemID / 10 );
		if (!(fInvenItem->dwCharType & g_pItemMgr->ItemCharTypeToItemFlag(m_pCharType))) {
			it = m_vecCutInSkinInfo.erase(it);
		}
		else {
			++it;
		}
	}
}

void KGCCutInSkinDlg::OnCreateComplete( void )
{
	m_iScrollpos = 0;
	UpdateSlotCutInSkinList();
	m_pkStaticExplanation->InitState( true );
	m_pkStaticExplanation->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_CUTINSKIN_INTRO ) );
	m_pkStaticExplanation->SetAlign( DT_CENTER );
}

void KGCCutInSkinDlg::FrameMoveInEnabledState( void )
{
	CheckItemInformation();
}

void KGCCutInSkinDlg::ActionPerformed(const KActionEvent& event)
{
	GCWND_MAP( m_pkScrollbar, OnScrollPos );
}

void KGCCutInSkinDlg::CreateCutInSkinSlot()
{
	float fBasePosX = 32.0f;
	float fBasePosY = 179.0f;
	if (m_vecCutInSkinList.empty())
	{
		std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_newcustomization_dlg.stg", "", "customization_dlg\\cutin_dlg\\box_item", true, EM_INVEN_COLUMN * EM_INVEN_ROW );
		for ( int i = 0; i < (int)vecTemp.size(); ++i )
		{
			float fPosX = fBasePosX + ( ( i % EM_INVEN_COLUMN ) * EM_INVEN_WIDH_SIZE );
			float fPosY = fBasePosY + ( ( i / EM_INVEN_COLUMN ) * EM_INVEN_HEIGHT_SIZE );
			vecTemp[i]->SetWindowPosDirect( D3DXVECTOR2( fPosX, fPosY ) );
			vecTemp[i]->ToggleRender( true );
			vecTemp[i]->InitState( true, true, this );
			m_vecCutInSkinList.push_back( (KGCCoordiViewItemWnd*)vecTemp[i] );
		}
	}
}

void KGCCutInSkinDlg::OnScrollPos( void )
{
	int iPos = m_pkScrollbar->GetScrollPos();
	if ( m_iScrollpos != iPos )
	{
		m_iScrollpos = iPos;
		g_KDSound.Play( "73" );
		UpdateSlotCutInSkinList();
	}
}

void KGCCutInSkinDlg::UpdateSlotCutInSkinList( void )
{
	if ( m_pCharType != g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType || m_vecCutInSkinInfo.empty() )
	{
		m_pCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;
		m_vecCutInSkinInfo.clear();
		FilterRemoveOtherChar();
	}
	int nItemCount = (int)m_vecCutInSkinInfo.size();
	int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;
	m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
	m_pkScrollbar->SetScrollRangeMax( nScrollMax );
	m_pkScrollbar->SetScrollPos( m_iScrollpos );
	UpdateCutInSkinItemList();
}

void KGCCutInSkinDlg::UpdateCutInSkinItemList( void )
{
	for ( int i = 0; i < (int)m_vecCutInSkinList.size(); ++i )
		m_vecCutInSkinList[i]->ResetItem();
	int nCount = 0;
	for (int i = 0; i < (int)m_vecCutInSkinInfo.size(); ++i)
	{
		if (i < (m_iScrollpos * EM_INVEN_COLUMN))
			continue;
		if (nCount >= EM_INVEN_COLUMN * EM_INVEN_ROW)
			return;
		m_vecCutInSkinList[nCount]->SetItemCustom( m_vecCutInSkinInfo[i].m_iItemID, m_vecCutInSkinInfo[i].m_iType, m_vecCutInSkinInfo[i].m_bIsBuy );
		nCount++;
	}
}

void KGCCutInSkinDlg::CheckItemInformation()
{
	for ( int i = 0; i < (int)m_vecCutInSkinList.size(); ++i )
		if ( m_vecCutInSkinList[i]->GetItemID() != 0 && m_vecCutInSkinList[i]->CheckMousePosInWindowBound() )
		{
			GCItem* pItem = g_pItemMgr->GetItemData( m_vecCutInSkinList[i]->GetItemID() / 10 );
			DrawItemInfo( pItem, m_vecCutInSkinList[i]->GetCurrentWindowPos() );
			D3DXVECTOR2 vFocusingPos = m_vecCutInSkinList[i]->GetFixedWindowLocalPos();
			vFocusingPos.x -= 2.0f;
			vFocusingPos.y -= 3.0f;
			return;
		}
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}

void KGCCutInSkinDlg::DrawItemInfo( GCItem* pItem, D3DXVECTOR2 vPos )
{
	if ( pItem != NULL && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false )
	{
		vPos.x += EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH;
		vPos.y += ( EM_INVEN_HEIGHT_SIZE / 2 ) * GC_SCREEN_DIV_WIDTH;
		vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
		vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
		if ( vPos.x + g_pkUIScene->m_pkItemInformationBox->GetWidth() > static_cast<float>( GC_SCREEN_WIDTH ) )
		{
			vPos.x -= ( static_cast<float>( EM_INVEN_WIDH_SIZE * GC_SCREEN_DIV_WIDTH ) * m_fWindowScaleX );
			vPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth();
		}
		GCItem* pGCItem = g_pItemMgr->GetItemData( pItem->dwGoodsID );
		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pGCItem, -1 );
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos );
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
	}
	else
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}