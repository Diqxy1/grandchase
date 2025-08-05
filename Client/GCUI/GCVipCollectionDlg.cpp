#include "stdafx.h"
#include "GCVipCollectionDlg.h"
#include "GCVipCollection.h"

IMPLEMENT_CLASSNAME( KGCVipCollectionDlg );
IMPLEMENT_WND_FACTORY( KGCVipCollectionDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCVipCollectionDlg, "vip_char_collection_dlg" );

KGCVipCollectionDlg::KGCVipCollectionDlg( void )
: m_pkScrollbar( NULL )
, m_pkStatusList( NULL )
, m_iScrollpos( 0 )
, m_pkStatusListComplete( 0 )
{
	LINK_CONTROL( "static_stat_complete_list", m_pkStatusListComplete );
	LINK_CONTROL( "static_stat_list", m_pkStatusList );
	LINK_CONTROL( "scrollbar_char", m_pkScrollbar );
}

KGCVipCollectionDlg::~KGCVipCollectionDlg( void )
{
}

void KGCVipCollectionDlg::OnCreate( void )
{
	if ( m_pkStatusList )
	{
		m_pkStatusList->InitState( true, true, this );
	}
	if ( m_pkStatusListComplete )
	{
		m_pkStatusListComplete->InitState( true, true, this );
	}
	if ( m_pkScrollbar )
	{
		m_pkScrollbar->InitState( true, true, this );
		m_pkScrollbar->InitScroll( 0, 1, 0, 1, 0, 0, 1, true );
	}
	CreateVIPSlot();
}

void KGCVipCollectionDlg::OnCreateComplete( void )
{
	SetAttrString();
	UpdateSlotCollectionViewList();
}

void KGCVipCollectionDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MAP( m_pkScrollbar, OnScrollPos );
}

void KGCVipCollectionDlg::CreateVIPSlot()
{
	if ( m_vecVipPlate.empty() )
	{
		std::vector< KD3DWnd* > vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_char_collection_dlg.stg", "", "all_collection_dlg\\vip_collection_dlg\\collection_vip", true, EM_INVEN_COLUMN * EM_INVEN_ROW );
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
			vecTemp[i]->InitState( true, true, this );
			m_vecVipPlate.push_back( (KGCVipCollection*)vecTemp[i] );
		}
	}
}

void KGCVipCollectionDlg::PopulateVIPPlate( int iIndex, int vipIndex, int iFontID )
{
	if( m_vecVipPlate[ iIndex ] )
		m_vecVipPlate[ iIndex ]->SetVIPImage( vipIndex, iFontID );
}

void KGCVipCollectionDlg::OnScrollPos()
{
	int iPos = m_pkScrollbar->GetScrollPos();
	if ( m_iScrollpos != iPos )
	{
		m_iScrollpos = iPos;
		g_KDSound.Play( "73" );
		UpdateSlotCollectionViewList();
	}
}

void KGCVipCollectionDlg::UpdateSlotCollectionViewList()
{
	int nItemCount = (int)g_kGlobalValue.m_vecVIPCollectionList.size();
	if ( m_iScrollpos > 0 && nItemCount <= EM_INVEN_COLUMN * EM_INVEN_ROW )
		m_iScrollpos = 0;
	int nScrollMax = ( ( nItemCount - 1 ) / EM_INVEN_COLUMN ) + 1;
	m_pkScrollbar->SetScrollPageSize( EM_INVEN_ROW );
	m_pkScrollbar->SetScrollRangeMax( nScrollMax );
	m_pkScrollbar->SetScrollPos( m_iScrollpos );
	UpdateCollectionViewItemList();
}

void KGCVipCollectionDlg::UpdateCollectionViewItemList()
{
	for ( int i = 0; i < (int)m_vecVipPlate.size(); ++i )
		m_vecVipPlate[i]->ResetCharPlates();
	int iFontSize = (int)g_kGlobalValue.m_vecVIPCollectionList.size();
	int nCurrentVIP = m_iScrollpos * 8;
	int nCount = 0;
	for ( int i = 0; i < (int)m_vecVipPlate.size(); ++i )
	{
		if ( i < ( m_iScrollpos * EM_INVEN_COLUMN ) )
			continue;
		if ( nCount >= EM_INVEN_COLUMN * EM_INVEN_ROW )
			return;
		if ( nCurrentVIP >= iFontSize )
			return;
		PopulateVIPPlate( nCount, nCurrentVIP, g_kGlobalValue.m_vecVIPCollectionList[ nCurrentVIP ].m_iFontID );
		nCurrentVIP++;
		nCount++;
	}
}

void KGCVipCollectionDlg::SetAttrString()
{
	if ( m_pkStatusList )
		m_pkStatusList->SetTextAutoMultiline( g_MyD3D->m_TempPlayer.GetVIPCollectionAllStringStatus() );
	if ( m_pkStatusListComplete )
		m_pkStatusListComplete->SetTextAutoMultiline( g_MyD3D->m_TempPlayer.GetVIPCollectionExStringStatus() );
}