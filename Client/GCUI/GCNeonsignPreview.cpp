#include "stdafx.h"
#include "GCNeonsignPreview.h"

#include "GCNeonsignSelectBox.h"
#include "GCItemManager.h"
#include "KGCAnnounceMgr.h"

IMPLEMENT_CLASSNAME( KGCNeonsignPreview );
IMPLEMENT_WND_FACTORY( KGCNeonsignPreview );
IMPLEMENT_WND_FACTORY_NAME( KGCNeonsignPreview, "gc_neonsign_preview" );

KGCNeonsignPreview::KGCNeonsignPreview(void)
: m_pkUpWindow(NULL)
, m_pkDownWindow(NULL)
, m_pkTitle(NULL)
, m_pkReserveBtn(NULL)
, m_pkCloseBtn(NULL)
, m_bReserve(false)
, m_bWaitForServer(false)
{
    LINK_CONTROL( "preview_up",     m_pkUpWindow );
    LINK_CONTROL( "preview_down",   m_pkDownWindow );

    SimpleAnnounce::InitAnnounceData( m_kAnnounceData );
}

KGCNeonsignPreview::~KGCNeonsignPreview(void)
{
}

void KGCNeonsignPreview::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn, D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkReserveBtn, D3DWE_BUTTON_CLICK, OnReserve );
}

void KGCNeonsignPreview::OnCreate()
{
    SiKGCAnnounceMgr()->SetPreview( false );
    m_pkUpWindow->InitState( true, true, this );
    m_pkDownWindow->InitState( true, true, this );

    m_pkTitle = static_cast<KD3DStatic*>( m_pkUpWindow->GetChildByName("title") );
    ASSERT( m_pkTitle != NULL );
    m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_NEONSIGN_PREVIEW ) );
    m_pkTitle->SetAlign( DT_CENTER );

    m_pkReserveBtn = static_cast<KD3DStatic*>( m_pkDownWindow->GetChildByName("btn_reserve") );
    ASSERT( m_pkReserveBtn != NULL );
    m_pkReserveBtn->InitState( true, true, this );

    m_pkCloseBtn = static_cast<KD3DStatic*>( m_pkDownWindow->GetChildByName("btn_close") );
    ASSERT( m_pkCloseBtn != NULL );
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    WaitForServer( false );
}

void KGCNeonsignPreview::OnClose()
{
    m_bReserve = false;
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    g_pkUIScene->m_pkNeonsignSelectBox->SetTitleText( false );
}

void KGCNeonsignPreview::OnDestroyComplete()
{
    SiKGCAnnounceMgr()->SetPreview( false );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );

    if( m_bReserve )
    {
        g_pkUIScene->m_pkNeonsignSelectBox->OnClose();
        if( true == g_pkUIScene->IsWaitMsgBox( KGCUIScene::GC_MBOX_NEONSIGN_SELECT ) )
        {
            g_pkUIScene->DeleteWaitMsgBox( KGCUIScene::GC_MBOX_NEONSIGN_SELECT );
        }
    }
}

void KGCNeonsignPreview::OnReserve()
{
    if( WaitForServer() )
        return;

    if( 0 == g_pItemMgr->GetInventoryItemDuration( m_kAnnounceData.m_ItemID / 10 ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NEONSIGN_ERROR2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }
    KP2P::GetInstance()->Send_GetSignBoardRegNum();
    WaitForServer( true );
}

void KGCNeonsignPreview::PostChildDraw()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_LIGHTING, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        SiKGCAnnounceMgr()->PreviewRender();
    }
    g_pGCDeviceManager2->PopState();
}

void KGCNeonsignPreview::FrameMoveInEnabledState()
{
    SiKGCAnnounceMgr()->PreviewFrameMove();
}

void KGCNeonsignPreview::SetPreviewData( const KSignBoardData& kData )
{
    m_kAnnounceData = kData;
}

void KGCNeonsignPreview::OnCreateComplete()
{
    SimpleAnnounce kPreview;
    kPreview.SetAnnounceData( m_kAnnounceData );
    SiKGCAnnounceMgr()->Preprocess( kPreview, 0.0f );
    D3DXVECTOR2 kRect = m_pkUpWindow->GetFixedWindowLocalPos();
    D3DXVECTOR2 kLeftBottom, kLeftTop;
    kPreview.GetNeonsignData().GetRect( KNeonSignItem::NSP_LEFTBOTTOM, kLeftBottom );
    kPreview.GetNeonsignData().GetRect( KNeonSignItem::NSP_LEFTTOP, kLeftTop );
    float fImsi = kPreview.GetTextTerm().y * 2.0f*GC_SCREEN_DIV_HEIGHT + kLeftBottom.y*GC_SCREEN_DIV_HEIGHT + kLeftTop.y*GC_SCREEN_DIV_HEIGHT ;
    kRect.y += static_cast<float>( m_pkUpWindow->GetHeight() ) + fImsi + kPreview.GetTextRect().y;
    m_pkUpWindow->GetChildByName( "center" )->SetHeight( static_cast<DWORD>(15.0f*GC_SCREEN_DIV_HEIGHT + fImsi + kPreview.GetTextRect().y) );
    m_pkDownWindow->SetFixedWindowLocalPos( kRect );
    D3DXVECTOR2 vPos = kPreview.GetTextPos();
    vPos.y = GetFixedWindowPos().y + static_cast<float>( m_pkUpWindow->GetHeight() ) + fImsi * 0.5f * GC_SCREEN_DIV_HEIGHT;
    kPreview.SetTextPos( vPos );
    SiKGCAnnounceMgr()->SetPreviewData( kPreview );
    SiKGCAnnounceMgr()->SetPreview( true );
    m_kAnnounceData = kPreview.GetAnnounceData();
}

void KGCNeonsignPreview::OnReserveClose()
{
    m_bReserve = true;
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCNeonsignPreview::WaitForServer( bool bWait )
{
    m_bWaitForServer = bWait;

    if( false == m_bWaitForServer )
    {
        m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_NEONSIGN_PREVIEW ) );
    }
    else
    {
        m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_WAITING_FOR_SERVER ) );
    }
}
