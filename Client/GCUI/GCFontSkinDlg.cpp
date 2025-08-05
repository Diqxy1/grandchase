#include "stdafx.h"
#include "GCFontSkinDlg.h"
#include "GCCoordiViewItemWnd.h"

IMPLEMENT_CLASSNAME( KGCFontSkinDlg );
IMPLEMENT_WND_FACTORY( KGCFontSkinDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCFontSkinDlg, "gc_fontskindlg" );

KGCFontSkinDlg::KGCFontSkinDlg( void )
: m_pkStaticExplanation( NULL )
, m_pkScrollbar( NULL )
{
	LINK_CONTROL( "scroll", m_pkScrollbar );
	LINK_CONTROL( "explanation", m_pkStaticExplanation );
}

KGCFontSkinDlg::~KGCFontSkinDlg( void )
{
}

void KGCFontSkinDlg::OnCreate( void )
{
	if (m_pkScrollbar)
	{
		m_pkScrollbar->InitState(true, true, this);
		m_pkScrollbar->InitScroll(0, 1, 0, 1, 0, 0, 1, true);
	}
	CreateFontSkinSlot();
}

void KGCFontSkinDlg::OnCreateComplete( void )
{
	m_iScrollpos = 0;
	UpdateSlotFontSkinList();
	m_pkStaticExplanation->InitState( true );
	m_pkStaticExplanation->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_FONTSKIN_INTRO ) );
	m_pkStaticExplanation->SetAlign( DT_CENTER );
}

void KGCFontSkinDlg::FrameMoveInEnabledState( void )
{
	CheckItemInformation();
}

void KGCFontSkinDlg::CreateFontSkinSlot()
{
	float fBasePosX = 32.0f;
	float fBasePosY = 179.0f;
	if (m_vecFontSkinList.empty())
	{
		std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_newcustomization_dlg.stg", "", "customization_dlg\\fontskin_dlg\\box_item", true, EM_INVEN_COLUMN * EM_INVEN_ROW );
		for ( int i = 0; i < (int)vecTemp.size(); ++i )
		{
			float fPosX = fBasePosX + ( ( i % EM_INVEN_COLUMN ) * EM_INVEN_WIDH_SIZE );
			float fPosY = fBasePosY + ( ( i / EM_INVEN_COLUMN ) * EM_INVEN_HEIGHT_SIZE );
			vecTemp[i]->SetWindowPosDirect( D3DXVECTOR2( fPosX, fPosY ) );
			vecTemp[i]->ToggleRender( true );
			vecTemp[i]->InitState( true, true, this );
			m_vecFontSkinList.push_back( (KGCCoordiViewItemWnd*)vecTemp[i] );
		}
	}
}

void KGCFontSkinDlg::ActionPerformed(const KActionEvent& event)
{
	GCWND_MAP( m_pkScrollbar, OnScrollPos );
}

void KGCFontSkinDlg::OnScrollPos( void )
{
	int iPos = m_pkScrollbar->GetScrollPos();
	if ( m_iScrollpos != iPos )
	{
		m_iScrollpos = iPos;
		g_KDSound.Play( "73" );
		UpdateSlotFontSkinList();
	}
}

void KGCFontSkinDlg::UpdateSlotFontSkinList( void )
{
	int nItemCount = (int)g_pFontSkinMgr->GetFontSkinList().size();
	int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;
	m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
	m_pkScrollbar->SetScrollRangeMax( nScrollMax );
	m_pkScrollbar->SetScrollPos( m_iScrollpos );
	UpdateFontSkinItemList();
}

void KGCFontSkinDlg::UpdateFontSkinItemList( void )
{
	for ( int i = 0; i < (int)m_vecFontSkinList.size(); ++i )
		m_vecFontSkinList[i]->ResetItem();
	int nCount = 0;
	for (int i = 0; i < (int)g_pFontSkinMgr->GetFontSkinList().size(); ++i)
	{
		if ( i < ( m_iScrollpos * EM_INVEN_COLUMN ) )
			continue;
		if (nCount >= EM_INVEN_COLUMN * EM_INVEN_ROW)
			return;
		m_vecFontSkinList[nCount]->SetItem(g_pFontSkinMgr->GetFontSkinList()[i].m_iItemID, g_pFontSkinMgr->GetFontSkinList()[i].m_iType, g_pFontSkinMgr->GetFontSkinList()[i].m_bIsBuy );
		nCount++;
	}
}

void KGCFontSkinDlg::CheckItemInformation()
{
	for ( int i = 0; i < (int)m_vecFontSkinList.size(); ++i )
	{
		if (m_vecFontSkinList[i]->GetItemID() != 0 && m_vecFontSkinList[i]->CheckMousePosInWindowBound())
		{
			GCItem* pItem = g_pItemMgr->GetItemData( m_vecFontSkinList[i]->GetItemID() / 10 );
			DrawItemInfo(pItem, m_vecFontSkinList[i]->GetCurrentWindowPos());
			D3DXVECTOR2 vFocusingPos = m_vecFontSkinList[i]->GetFixedWindowLocalPos();
			vFocusingPos.x -= 2.0f;
			vFocusingPos.y -= 3.0f;
			return;
		}
	}
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
}

void KGCFontSkinDlg::DrawItemInfo( GCItem* pItem, D3DXVECTOR2 vPos )
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
	{
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}
}