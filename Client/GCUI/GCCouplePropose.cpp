#include "StdAfx.h"

#include "KGC2DObject.h"
#include "GCCouplePropose.h"
#include "GCCoupleSystem.h"
#include "GCItemInformationBox.h"
#include "GCItemManager.h"

IMPLEMENT_CLASSNAME( KGCCouplePropose );
IMPLEMENT_WND_FACTORY( KGCCouplePropose );
IMPLEMENT_WND_FACTORY_NAME( KGCCouplePropose, "gc_couple_propose" );

KGCCouplePropose::KGCCouplePropose(void)
: m_eCoupleState( CP_PROPOSE )
, m_dwPrePartnerUID( 0 )
, m_iCurrentRing( 0 )
{
	m_pkRingSlot = NULL;

	m_pkCoupleString = NULL;
	m_pkMessage = NULL;

	m_pkProposeMessage = NULL;

	m_pkOk = NULL;
	m_pkCancle = NULL;
	m_pkRingRight = NULL;
	m_pkRingLeft = NULL;

	m_pkTitle[CP_PROPOSE] = NULL;
	m_pkTitle[CP_RINGCHANGE] = NULL;


	LINK_CONTROL( "ring_slot", m_pkRingSlot );

	LINK_CONTROL( "static", m_pkCoupleString );
	LINK_CONTROL( "static_message", m_pkMessage );

	LINK_CONTROL( "edit_message", m_pkProposeMessage );

	LINK_CONTROL( "btn_ok", m_pkOk );
	LINK_CONTROL( "btn_cancle", m_pkCancle );
	LINK_CONTROL( "btn_right", m_pkRingRight );
	LINK_CONTROL( "btn_left", m_pkRingLeft );

	LINK_CONTROL( "title_propose", m_pkTitle[CP_PROPOSE] );
	LINK_CONTROL( "title_ringchange", m_pkTitle[CP_RINGCHANGE] );
}

KGCCouplePropose::~KGCCouplePropose(void)
{
}

void KGCCouplePropose::OnCreate( void )
{
	m_pkRingSlot->InitState( true );

	m_pkCoupleString->InitState( true );
	m_pkCoupleString->SetText( g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM10 ) );
	m_pkCoupleString->SetAlign( DT_CENTER );
	m_pkMessage->InitState( true );
	m_pkMessage->SetText( g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM6 ) );

	m_pkProposeMessage->InitState( true );
	m_pkProposeMessage->SetText( L"" );

	m_pkOk->InitState( true, true, this );
	m_pkCancle->InitState( true, true, this );
	m_pkRingRight->InitState( true, true, this );
	m_pkRingLeft->InitState( true, true, this );

	m_pkTitle[CP_PROPOSE]->InitState( true );
	m_pkTitle[CP_RINGCHANGE]->InitState( false );

	m_kRingImage.SetScale( 0.3f, 0.3f );

	m_iCurrentRing = 0;

#if defined( _DEBUG )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

	float x = 0.0f, y = 1.0f;
	LUA_GET_VALUE_DEF( "Scalex", x, 1.0f );
	LUA_GET_VALUE_DEF( "Scaley", y, 1.0f );

	float px, py;
	LUA_GET_VALUE_DEF( "Px", px, 1.0f );
	LUA_GET_VALUE_DEF( "Py", py, 1.0f );
#endif
	m_kRingImage.SetPosition( ( m_pkRingSlot->GetFixedWindowPos().x + 3.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -( m_pkRingSlot->GetFixedWindowPos().y + 36.0f ) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f );
	m_kRingImage.SetScale( 0.23f, 0.23f );
	m_kRingImage.CameraMatrixOn();
}

void KGCCouplePropose::ActionPerformed( const KActionEvent& event )
{
	_GCWND_MSG_MAP( m_pkOk,		KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOk );
	_GCWND_MSG_MAP( m_pkCancle, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancle );

	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK &&
		( event.m_pWnd == m_pkRingRight || event.m_pWnd == m_pkRingLeft ) )
	{
		OnClickRingChange( event.m_pWnd == m_pkRingRight );
	}
}

void KGCCouplePropose::OnClickRingChange( bool bRight )
{
	if( m_vecRingList.empty() )
		return;

	bRight?--m_iCurrentRing:--m_iCurrentRing;

	if( m_iCurrentRing >= (int)m_vecRingList.size() )
		m_iCurrentRing = 0;

	if( m_iCurrentRing < 0 )
		m_iCurrentRing = (int)m_vecRingList.size() - 1;

	m_kRingImage.RemoveAllTexture();
	int iItemID = g_pItemMgr->GetSameImageIndex(m_vecRingList[m_iCurrentRing].m_dwID);

	char strItemID[MAX_PATH];
	sprintf( strItemID, "Sbta%03d.dds", iItemID );
	m_kRingImage.AddTexture( strItemID );
}

void KGCCouplePropose::OnClickOk()
{
	if( m_vecRingList.empty() == false )
	{
		if( m_eCoupleState == CP_PROPOSE )
		{
			SiKGCCoupleSystem()->SendPropose( m_dwPrePartnerUID, m_vecRingList[m_iCurrentRing].m_dwID, m_vecRingList[m_iCurrentRing].m_dwUID, m_pkProposeMessage->GetText() );
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_WAIT );
		}
		else
		{
			SiKGCCoupleSystem()->SendRingChange( m_vecRingList[m_iCurrentRing].m_dwID, m_vecRingList[m_iCurrentRing].m_dwUID, m_pkProposeMessage->GetText() );
		}
	}
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCCouplePropose::OnClickCancle()
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCCouplePropose::SetUse( GCCoupleProposeState eState )
{
	if( eState < 0 || eState >= CP_NUM )
	{
		ASSERT( !"상태 초기화 값이 이상함" );
		eState = CP_PROPOSE;
	}
	m_eCoupleState = eState;
	m_iCurrentRing = 0;
	m_vecRingList = SiKGCCoupleSystem()->GetRingList( true, m_eCoupleState != CP_PROPOSE, true );
	m_pkRingRight->Lock( m_vecRingList.size() <= 1 );
	m_pkRingLeft->Lock( m_vecRingList.size() <= 1 );

	m_pkTitle[CP_PROPOSE]->ToggleRender( eState == CP_PROPOSE );
	m_pkTitle[CP_RINGCHANGE]->ToggleRender( eState == CP_RINGCHANGE );

	if( m_vecRingList.empty() == false )
	{
		m_kRingImage.RemoveAllTexture();
		int iItemID = g_pItemMgr->GetSameImageIndex(m_vecRingList[m_iCurrentRing].m_dwID);

		char strItemID[MAX_PATH];
		sprintf( strItemID, "Sbta%03d.dds", iItemID );
		m_kRingImage.AddTexture( strItemID );
	}
}

void KGCCouplePropose::SetProposeUserUID( DWORD dwUID, std::wstring strNickName )
{
	m_dwPrePartnerUID = dwUID;
}

void KGCCouplePropose::PostChildDraw( void )
{
	m_kRingImage.Render();
}

void KGCCouplePropose::DrawItemInfo()
{
    if( m_vecRingList.empty() )
        return;

    ASSERT( g_pkInput != NULL );
    D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

	if( m_pkRingSlot->CheckPosInWindowBound( vMousePos ) == false )
	{
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
		return;
	}

	GCItem* pItem = g_pItemMgr->GetItemData( m_vecRingList[m_iCurrentRing].m_dwID );
    KItem* pInventoryItem = NULL;
    if( g_pItemMgr->FindInventory( m_vecRingList[m_iCurrentRing].m_dwUID, &pInventoryItem, &pItem ) && pItem && pInventoryItem)
    {
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, pInventoryItem);
        D3DXVECTOR2 vNewPos = m_pkRingSlot->GetCurrentWindowPos();
        vNewPos.x = (vNewPos.x + m_pkRingSlot->GetWidth() ) * m_fWindowScaleX;
        vNewPos.y *= m_fWindowScaleY;
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vNewPos );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    }
}


void KGCCouplePropose::FrameMoveInEnabledState( void )
{
	DrawItemInfo();
}

void KGCCouplePropose::OnDestroyComplete()
{
	m_pkProposeMessage->Activate( false );
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_DESTROYED ) );
}

bool KGCCouplePropose::InitDialog( IN const KDialogInfo& kInfo_ )
{
    switch( kInfo_.m_nMsgBoxUse )
    {
    case KGCUIScene::GC_MBOX_USE_NORMAL:
        {
            this->SetUse( KGCCouplePropose::CP_PROPOSE );
            this->SetProposeUserUID( static_cast<DWORD>(kInfo_.m_lParam), kInfo_.m_strText1 );
        }
        break;
    case KGCUIScene::GC_MBOX_USE_COUPLE_RING_CHANGE:
        {
            this->SetUse( KGCCouplePropose::CP_RINGCHANGE );
        }
        break;
    }
    return true;
}