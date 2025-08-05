#include "stdafx.h"
#include "GCSongkranRewardDlg.h"

IMPLEMENT_CLASSNAME( KGCSongkranRewardDlg );
IMPLEMENT_WND_FACTORY( KGCSongkranRewardDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSongkranRewardDlg, "gc_songkranrewarddlg" );

KGCSongkranRewardDlg::KGCSongkranRewardDlg( void )
: m_iItemID( 0 )
, m_iItemSize( 0 )
{
	m_pkBtnOK = NULL;
	m_pkRewardImg = NULL;
	m_pkRewardDesc = NULL;

	LINK_CONTROL( "btn_ok",	m_pkBtnOK );
	LINK_CONTROL( "reward_item", m_pkRewardImg );
	LINK_CONTROL( "static_reward_desc", m_pkRewardDesc );
}

KGCSongkranRewardDlg::~KGCSongkranRewardDlg( void )
{
}

void KGCSongkranRewardDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCSongkranRewardDlg::OnCreate( void )
{
	m_pkBtnOK->InitState( true, true, this );
	m_pkBtnOK->SetHotKey( DIK_ESCAPE );
	m_pkBtnOK->SetHotKey( DIK_RETURN );
	m_pkBtnOK->SetHotKey( DIK_SPACE );

	m_pkRewardDesc->InitState( true );

	m_pkRewardImg->InitState( true );
	m_pkRewardImg->ShowItem( false );
}

bool KGCSongkranRewardDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
//                     아이템아이디,          아이템사이즈,
 	SetText( (int)kInfo_.m_lParam, (int)kInfo_.m_lParam2 );
 	SetWindowZOrder( D3DWZ_TOPMOST );
	return true;
}

void KGCSongkranRewardDlg::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCSongkranRewardDlg::SetText( int iItemID, int iItemSize )
{
	m_iItemID = iItemID / 10;
	m_iItemSize = iItemSize;

	GCItem* pItem = g_pItemMgr->GetItemData( m_iItemID );
	std::wstring strMsg;

	if ( !pItem ) 
		return;

	strMsg = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_RECEIVE1, "l", pItem->strItemName );
	
    m_pkRewardDesc->SetForceWordWrapText( strMsg , true );
    m_pkRewardDesc->SetToolTip( strMsg );
	
	m_pkRewardImg->SetItemInfo( m_iItemID );
	m_pkRewardImg->ShowItem( true );
}

void KGCSongkranRewardDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID )
{
	vPos_.x *= m_fWindowScaleX;
	vPos_.y *= m_fWindowScaleY;
	vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

	GCItem* pItem = g_pItemMgr->GetItemData(_itemID);
	if ( !pItem ) 
		return;

	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

	g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}

void KGCSongkranRewardDlg::FrameMoveInEnabledState( void )
{

	const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	bool bCheck = false;
	if( m_pkRewardImg->CheckPosInWindowBound( vMousePos ) && m_pkRewardImg->IsRenderOn() )
	{
		// 아이템 정보 표시창 위치 보정.
		D3DXVECTOR2 vecPos = m_pkRewardImg->GetCurrentWindowPos();
		GCITEMID itemID = m_pkRewardImg->GetItemID();
		RenderItemInfoBox( vecPos, itemID );
		bCheck = true;
	}
}