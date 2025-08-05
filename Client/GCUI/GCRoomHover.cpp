#include "stdafx.h"
#include "GCRoomHover.h"
IMPLEMENT_CLASSNAME( KGCRoomHover );
IMPLEMENT_WND_FACTORY( KGCRoomHover );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomHover, "gc_room_hover" );

KGCRoomHover::KGCRoomHover( void )
{
    // empty
}

KGCRoomHover::~KGCRoomHover( void )
{
    // empty
}

void KGCRoomHover::PreDraw( void )
{
    g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
}

void KGCRoomHover::PostDraw( void )
{
    g_pGCDeviceManager2->PopState();    
}