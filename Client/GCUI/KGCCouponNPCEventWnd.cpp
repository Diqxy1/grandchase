#include "stdafx.h"
#include "P2P.h"
#include "../uifx/KInput.h"
#include "GCDescBox.h"
#include "GCSizingThreeBox.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
#include "KGCCouponNPCEventWnd.h"
#include "./gcui/GCItemInfoBox.h"
#include "KDSound.h"

IMPLEMENT_CLASSNAME( KGCCouponNPCEventWnd );
IMPLEMENT_WND_FACTORY( KGCCouponNPCEventWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCCouponNPCEventWnd, "gc_coupon_event_contents" );

KGCCouponNPCEventWnd::KGCCouponNPCEventWnd(void)
:m_bSelectedWnd(false)
,m_bActive(false)
,m_bEmpty(false)
{
	m_pTextureItem			=NULL;

	m_pkBackground			=NULL;
	m_pkBackgroundHover		=NULL;
	m_pkBackgroundSelect	=NULL;

	m_pkFrameItem			=NULL;
	m_pkFrameItemHover		=NULL;
	m_pkFrameItemSelect		=NULL;

	m_pkFrameTitle			=NULL;
	m_pkFrameTitleHover		=NULL;
	m_pkFrameTitleSelect	=NULL;

	m_pkFrameContents		=NULL;
	m_pkFrameContentsHover	=NULL;
	m_pkFrameContentsSelect	=NULL;

	m_pkTitleText			=NULL;	
	m_pkContentsText		=NULL;	
	m_pkBtnWebLink			=NULL;

	m_pkNewEvent			=NULL;
	m_pkEndEvent			=NULL;

	LINK_CONTROL( "background",					m_pkBackground );
	LINK_CONTROL( "background_hover",			m_pkBackgroundHover );
	LINK_CONTROL( "background_select",			m_pkBackgroundSelect );

	LINK_CONTROL( "frame_item",					m_pkFrameItem );
	LINK_CONTROL( "frame_item_hover",			m_pkFrameItemHover );
	LINK_CONTROL( "frame_item_select",			m_pkFrameItemSelect );

	LINK_CONTROL( "frame_title",				m_pkFrameTitle );
	LINK_CONTROL( "frame_title_hover",			m_pkFrameTitleHover );
	LINK_CONTROL( "frame_title_select",			m_pkFrameTitleSelect );

	LINK_CONTROL( "frame_contents",				m_pkFrameContents );
	LINK_CONTROL( "frame_contents_hover",		m_pkFrameContentsHover );
	LINK_CONTROL( "frame_contents_select",		m_pkFrameContentsSelect );

	LINK_CONTROL( "btn_web_link",				m_pkBtnWebLink );

	LINK_CONTROL( "static_event_title",			m_pkTitleText );
	LINK_CONTROL( "static_event_contents",		m_pkContentsText );

	LINK_CONTROL( "btn_new_event",				m_pkNewEvent );
	LINK_CONTROL( "btn_end_event",				m_pkEndEvent );

#if defined(NATION_PHILIPPINE)
	m_strWebAddress = "http://grandchase.levelupgames.ph/index.php";
#elif defined(NATION_THAILAND)
	m_strWebAddress = GCUTIL_STR::GCStrWideToChar(COUPON_EVENT_LINK);
#else
	m_strWebAddress = "";
#endif
}

KGCCouponNPCEventWnd::~KGCCouponNPCEventWnd(void)
{	
}

void KGCCouponNPCEventWnd::OnCreate( void )
{
	m_pkBackground->InitState(true);
	m_pkFrameItem->InitState(true);
	m_pkFrameTitle->InitState(true);
	m_pkFrameContents->InitState(true);

	m_pkBtnWebLink->InitState( false, true, this );

	m_pkBackgroundHover->InitState(false);
	m_pkFrameItemHover->InitState(false);	
	m_pkFrameTitleHover->InitState(false);
	m_pkFrameContentsHover->InitState(false);

	m_pkBackgroundSelect->InitState(false);
	m_pkFrameItemSelect->InitState(false);
	m_pkFrameTitleSelect->InitState(false);
	m_pkFrameContentsSelect->InitState(false);	

	m_pkTitleText->InitState( true, true );
	m_pkContentsText->InitState( true, true );

	m_pkNewEvent->InitState(false);
	m_pkEndEvent->InitState(false);

	m_bSelectedWnd = false;
	m_bActive = false;
	m_bEmpty = false;
}

void KGCCouponNPCEventWnd::FrameMoveInEnabledState( void )
{
	if( !m_bActive || m_bEmpty )		
		return;

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	if( m_pkBackground->CheckPosInWindowBound(vMousePos) && !m_bSelectedWnd)
	{
		RenderHoverWnd(true);

		if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			RenderSelectWnd(true);
	}
	else
	{
		RenderHoverWnd(false);
	}

	m_pkBtnWebLink->SetToolTip(g_pkStrLoader->GetString(STR_ID_COUPON_NPC_TOOLTIP));
}

void KGCCouponNPCEventWnd::ActionPerformed( const KActionEvent& event )
{
	if( !m_bActive || m_bEmpty )		
		return;

	if( event.m_pWnd == m_pkBtnWebLink && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		PopUpWebPage( GCUTIL_STR::GCStrCharToWide((char*)m_strWebAddress.c_str()) );
	}
}

void KGCCouponNPCEventWnd::OnCreateComplete()
{
}

void KGCCouponNPCEventWnd::OnDestroyComplete()
{
	SAFE_RELEASE(m_pTextureItem);
}

void KGCCouponNPCEventWnd::PostChildDraw()
{
	if( m_pTextureItem == NULL || m_bEmpty )
		return;

    //테스트코드
    /*KLuaManager luaMgr;
    GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    float x = 0;
    float y = 0;

    LUA_GET_VALUE_DEF( "x", x, 0 );
    LUA_GET_VALUE_DEF( "y", y, 0 );*/

	D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

    vPos.x -= 8.0f;
    vPos.y -= 6.0f;

	D3DXVECTOR2 vPos2( 18.f * GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, 2.0f * GC_SCREEN_DIV_WIDTH * m_fWindowScaleY);
	g_pItemMgr->Draw_ShopImage( vPos + vPos2, 40.f * GC_SCREEN_DIV_WIDTH * m_fWindowScaleX , 40.f * GC_SCREEN_DIV_WIDTH * m_fWindowScaleY, m_pTextureItem, false);
}

void KGCCouponNPCEventWnd::RenderHoverWnd(bool bRender_)
{
	m_pkBackgroundHover->ToggleRender(bRender_);
	m_pkFrameItemHover->ToggleRender(bRender_);
	m_pkFrameTitleHover->ToggleRender(bRender_);
	m_pkFrameContentsHover->ToggleRender(bRender_);
}

void KGCCouponNPCEventWnd::RenderSelectWnd(bool bRender_)
{
	m_pkBackgroundSelect->ToggleRender(bRender_);
	m_pkFrameItemSelect->ToggleRender(bRender_);
	m_pkFrameTitleSelect->ToggleRender(bRender_);
	m_pkFrameContentsSelect->ToggleRender(bRender_);
}

void KGCCouponNPCEventWnd::SetText( std::wstring& strEventTitle_,  std::wstring& strEventContents_ )
{
	m_pkTitleText->SetText(strEventTitle_);
	m_pkContentsText->SetText(strEventContents_);
}

void KGCCouponNPCEventWnd::SetActiveWnd( bool bActive_ ) 
{ 
	m_bActive = bActive_; 
}

void KGCCouponNPCEventWnd::SetEventTexture( int iEventUID_ )
{
	SAFE_RELEASE( m_pTextureItem );		

	char strEventCouponUID[MAX_PATH];

	sprintf( strEventCouponUID, "coupon%d.dds", iEventUID_ );

	m_pTextureItem = g_pGCDeviceManager2->CreateTexture(strEventCouponUID);
}

void KGCCouponNPCEventWnd::RenderEventState(bool bRender_)
{
	m_pkNewEvent->ToggleRender(bRender_);
}

void KGCCouponNPCEventWnd::SetEmptyWnd( bool bRender_)
{
	m_bEmpty = bRender_;
	m_pkBtnWebLink->ToggleRender( !bRender_ );
}

bool KGCCouponNPCEventWnd::IsEmptyEvent()
{
	if( !m_bActive || m_bEmpty )		
		return true;
	else
		return false;
}

void KGCCouponNPCEventWnd::SetWebButtonOff()
{
    m_pkBtnWebLink->ToggleRender( false );
}