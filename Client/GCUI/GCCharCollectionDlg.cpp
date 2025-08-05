#include "stdafx.h"
#include "GCCharCollectionDlg.h"
#include "GCCharCollection.h"
#include "GCCharCollectionImage.h"

IMPLEMENT_CLASSNAME( KGCCharCollectionDlg );
IMPLEMENT_WND_FACTORY( KGCCharCollectionDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCharCollectionDlg, "gc_char_collection_dlg" );

KGCCharCollectionDlg::KGCCharCollectionDlg( void )
: m_pkScrollbar( NULL )
, m_pkStatusList( NULL )
, m_iScrollpos( 0 )
{
	LINK_CONTROL( "static_stat_list", m_pkStatusList );
	LINK_CONTROL( "scrollbar_char", m_pkScrollbar );
}

KGCCharCollectionDlg::~KGCCharCollectionDlg( void )
{
}

void KGCCharCollectionDlg::OnCreate( void )
{
	if ( m_pkStatusList )
	{
		m_pkStatusList->InitState( true, true, this );
	}
	if ( m_pkScrollbar )
	{
		m_pkScrollbar->InitState( true, true, this );
		m_pkScrollbar->InitScroll( 0, 1, 0, 1, 0, 0, 1, true );
	}
	CreateCharSlot();
}

void KGCCharCollectionDlg::OnCreateComplete( void )
{
	SetAttrString();
	UpdateSlotCollectionViewList();
}

void KGCCharCollectionDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MAP( m_pkScrollbar, OnScrollPos );
}

void KGCCharCollectionDlg::CreateCharSlot()
{
	if ( m_vecCharPlate.empty() )
	{
		std::vector< KD3DWnd* > vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_char_collection_dlg.stg", "", "all_collection_dlg\\char_collection_dlg\\collection_char", true, EM_INVEN_COLUMN * EM_INVEN_ROW );
		for ( int i = 0; i < (int)vecTemp.size(); ++i )
		{
			int moreX = 0;
			int moreY = 0;
			if ( i > 1 )
				moreY = 4;
			if ( i > 3 )
				moreY = 8;
			if ( i % 2 != 0 )
				moreX = 4;
			float fPosX = EM_STARTER_X + ( ( i % EM_INVEN_COLUMN ) * EM_INVEN_WIDH_SIZE ) + moreX;
			float fPosY = EM_STARTER_Y + ( ( i / EM_INVEN_COLUMN ) * EM_INVEN_HEIGHT_SIZE ) + moreY;
			vecTemp[i]->SetWindowPosDirect( D3DXVECTOR2( fPosX, fPosY ) );
			vecTemp[i]->ToggleRender( true );
			vecTemp[i]->InitState( false, true, this );
			m_vecCharPlate.push_back( (KGCCharCollection*)vecTemp[i] );
		}
	}
}

void KGCCharCollectionDlg::PopulateCharPlate( int iIndex, int iCharType )
{
	for ( auto element : g_kGlobalValue.m_vecCharacterCollectionList )
	{
		if ( element.m_iCharType == iCharType )
		{
			if( m_vecCharPlate[iIndex] )
			{
				m_vecCharPlate[ iIndex ]->ToggleRender( true );
				m_vecCharPlate[ iIndex ]->SetCharType( element.m_iCharType, -1 );
			}
		}
	}
	for( auto element : g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo )
		if ( element.iCharType == iCharType )
			if( m_vecCharPlate[iIndex] )
				m_vecCharPlate[ iIndex ]->SetCharType( element.iCharType, element.iPromotionLevel );
}

void KGCCharCollectionDlg::OnScrollPos()
{
	int iPos = m_pkScrollbar->GetScrollPos();
	if ( m_iScrollpos != iPos )
	{
		m_iScrollpos = iPos;
		g_KDSound.Play( "73" );
		UpdateSlotCollectionViewList();
	}
}

void KGCCharCollectionDlg::UpdateSlotCollectionViewList()
{
	int nItemCount = (int)g_kGlobalValue.m_vecCharacterCollectionList.size();
	if ( m_iScrollpos > 0 && nItemCount <= EM_INVEN_COLUMN * EM_INVEN_ROW )
		m_iScrollpos = 0;
	int nScrollMax = ( EM_INVEN_COLUMN * EM_INVEN_ROW ) + 1;
	m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
	m_pkScrollbar->SetScrollRangeMax( nScrollMax );
	m_pkScrollbar->SetScrollPos( m_iScrollpos );
	UpdateCollectionViewItemList();
}

void KGCCharCollectionDlg::UpdateCollectionViewItemList()
{
	for ( int i = 0; i < (int)m_vecCharPlate.size(); ++i )
		m_vecCharPlate[i]->ResetCharPlates();
	int nCurrentChar = m_iScrollpos * 5;
	int nCount = 0;
	for ( int i = 0; i < (int)g_kGlobalValue.m_vecCharacterCollectionList.size(); ++i )
	{
		if ( i < ( m_iScrollpos * EM_INVEN_COLUMN ) )
			continue;
		if ( nCount >= EM_INVEN_COLUMN * EM_INVEN_ROW )
			return;
		if ( nCurrentChar >= GC_CHAR_NUM )
			return;
		PopulateCharPlate( nCount, nCurrentChar );
		nCurrentChar++;
		nCount++;
	}
}

void KGCCharCollectionDlg::SetAttrString()
{
	if ( m_pkStatusList )
	{
		m_pkStatusList->SetTextAutoMultiline( g_MyD3D->m_TempPlayer.GetCharCollectionAllStringStatus() );
	}
}