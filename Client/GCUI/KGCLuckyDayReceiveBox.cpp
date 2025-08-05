#include "stdafx.h"
#include "KGCLuckyDayReceiveBox.h"


//
//

#include "MyD3D.h"
//

IMPLEMENT_CLASSNAME( KGCLuckyDayReceiveBox );
IMPLEMENT_WND_FACTORY( KGCLuckyDayReceiveBox );
IMPLEMENT_WND_FACTORY_NAME( KGCLuckyDayReceiveBox, "gc_lucky_day_receive_box" );

KGCLuckyDayReceiveBox::KGCLuckyDayReceiveBox(void)
{
    m_pkOk              = NULL;
    m_pkItemName        = NULL;

    LINK_CONTROL( "ok_button",          m_pkOk );
    LINK_CONTROL( "item_name_static",   m_pkItemName );

    m_iItemID           = -1;
    m_pTextureItem      = NULL;
    m_pTextureLuckyDay  = NULL;
}

KGCLuckyDayReceiveBox::~KGCLuckyDayReceiveBox(void)
{
    SAFE_RELEASE( m_pTextureItem );
    SAFE_RELEASE( m_pTextureLuckyDay );
}

void KGCLuckyDayReceiveBox::OnCreate( void )
{
    m_pkOk->InitState( true, true, this );
    m_pkItemName->InitState( true );

    m_pkOk->SetHotKey( DIK_RETURN );
    m_pkOk->SetHotKey( DIK_ESCAPE );
    m_pkItemName->SetAlign( DT_CENTER );

    SAFE_RELEASE( m_pTextureLuckyDay );
    m_pTextureLuckyDay = g_pGCDeviceManager2->CreateTexture( "LuckyDay.dds" );
}

void KGCLuckyDayReceiveBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        g_KDSound.Play( "30" );

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkOk )
        {
            g_KDSound.Play( "31" );
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
        }
    }
}

void KGCLuckyDayReceiveBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCLuckyDayReceiveBox::PostChildDraw()
{
	static const float  LEFT   = 173.0f;
	static const float  TOP    = 100.0f;
	static const float  WIDTH  = 105.0f;
	static const float  HEIGHT = 105.0f;
    if( m_iItemID > -1 )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        // 샵 아이템 이미지를 그린다.
        /*m_pTextureItem->SetDeviceTexture();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
            g_MyD3D->Draw_TL( vPos.x + LEFT, vPos.y + TOP, vPos.x + LEFT + WIDTH, vPos.y + TOP + HEIGHT,
                0.0f, 0.0f, 1.0f, 1.0f );
        }
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/

		g_pGCDeviceManager2->DrawInScreen(m_pTextureItem,
			vPos.x + LEFT * m_fWindowScaleX, vPos.y + TOP * m_fWindowScaleY, vPos.x + (LEFT + WIDTH) * m_fWindowScaleX, vPos.y + (TOP + HEIGHT) * m_fWindowScaleY,
			0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);
    }

    if( m_pTextureLuckyDay != NULL )
    {
        D3DXVECTOR2 vPos( GetRelocatedWindowPos() );

        /*m_pTextureLuckyDay->SetDeviceTexture();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
            g_MyD3D->Draw_TL( vPos.x + 130, vPos.y + 0, vPos.x + 130 + 125, vPos.y + 0 + 125,
                0.0f, 0.0f, 1.0f, 1.0f );
        }
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
        KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/

		g_pGCDeviceManager2->DrawInScreen(m_pTextureLuckyDay,
			vPos.x + 130 * m_fWindowScaleX, vPos.y + 0, vPos.x + (130 + 125) * m_fWindowScaleX, vPos.y + 0 + 125 * m_fWindowScaleY,
			0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);
    }
}

void KGCLuckyDayReceiveBox::FrameMoveInEnabledState()
{
}

void KGCLuckyDayReceiveBox::SetItem( DWORD dwItemID )
{
    m_iItemID = dwItemID;
    SAFE_RELEASE( m_pTextureItem );

    m_pTextureItem = g_pItemMgr->CreateItemTexture( m_iItemID, 0 );

    GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( m_iItemID);

    m_pkItemName->SetText( kItem->strItemName );
}

bool KGCLuckyDayReceiveBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    g_KDSound.Play( "Enchant_Success" );
    this->SetItem( static_cast<GCITEMID>(kInfo_.m_lParam) );
    return true;
}