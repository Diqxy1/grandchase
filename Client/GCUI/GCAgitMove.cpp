#include "stdafx.h"
#include "GCAgitMove.h"
#include "GCAgitMapSlot.h"
#include "MapSystem\KGCLayerManager.h"

IMPLEMENT_CLASSNAME( KGCAgitMove );
IMPLEMENT_WND_FACTORY( KGCAgitMove );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitMove, "gc_agit_move" );

KGCAgitMove::KGCAgitMove( void )
{
    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i] = NULL;
    LINK_CONTROL( "btnPrev",    m_apkBtn[ BTN_PREV ] );
    LINK_CONTROL( "btnNext",    m_apkBtn[ BTN_NEXT ] );
    LINK_CONTROL( "btnClose",   m_apkBtn[ BTN_CLOSE ] );
    LINK_CONTROL( "btnWatch",   m_apkBtn[ BTN_WATCH ] );
    LINK_CONTROL( "btnMove",    m_apkBtn[ BTN_MOVE ] );

    m_pkStaticMapInfo = NULL;
    m_pkStaticPage = NULL;
    LINK_CONTROL( "staticMap",   m_pkStaticMapInfo );
    LINK_CONTROL( "staticPage",  m_pkStaticPage );

    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        m_apkSlot[i] = NULL;

        char szTemp[128] = { 0, };
        sprintf_s( szTemp, 127, "mapSlot%d", i );
        LINK_CONTROL( szTemp, m_apkSlot[i] );
    }

    for( int i=0 ; i<NUM_FLOWER_POT ; ++i )
    {
        m_apkIconFlowerPot[i] = NULL;

        char szTemp[128] = { 0, };
        sprintf_s( szTemp, "iconFlowerPot%d", i );
        LINK_CONTROL( szTemp, m_apkIconFlowerPot[i] );
    }

    m_pkSelectFrame = NULL;
    LINK_CONTROL( "curSel", m_pkSelectFrame );

    m_iCurPage = 0;
    m_iTotalPage = 0;
    m_iCurSlot = -1;
}

KGCAgitMove::~KGCAgitMove( void )
{
}

void KGCAgitMove::OnCreate( void )
{
    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i]->InitState( true, true, this );

    m_pkStaticMapInfo->InitState( true, false, NULL );
    m_pkStaticPage->InitState( true, false, NULL );
    m_pkStaticMapInfo->SetAlign( DT_LEFT );
    m_pkStaticPage->SetAlign( DT_CENTER );

    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_FLOWER_POT ; ++i )
        m_apkIconFlowerPot[i]->InitState( false );

    m_pkSelectFrame->InitState( false, false, NULL );
    
    m_apkBtn[ BTN_CLOSE ]->SetHotKey( DIK_ESCAPE );
}

void KGCAgitMove::ActionPerformed( const KActionEvent& event )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        if( event.m_pWnd == m_apkSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
            OnClickSlot( i );
    }

    if( event.m_pWnd == m_apkBtn[ BTN_PREV ] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnClickPage( BTN_PREV );
    if( event.m_pWnd == m_apkBtn[ BTN_NEXT ] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnClickPage( BTN_NEXT );

    GCWND_MSG_MAP( m_apkBtn[ BTN_WATCH ],       KD3DWnd::D3DWE_BUTTON_CLICK,    OnWatch );
    GCWND_MSG_MAP( m_apkBtn[ BTN_MOVE ],        KD3DWnd::D3DWE_BUTTON_CLICK,    OnMove );
    GCWND_MSG_MAP( m_apkBtn[ BTN_CLOSE ],       KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
}

void KGCAgitMove::PostChildDraw( void )
{
    // 현재 선택한 맵 화분 갯수 
    if( m_iCurSlot == -1 )
        return;


}

void KGCAgitMove::InitUI( KEVENT_AGIT_MAP_CATALOGUE_ACK& catalog_ )
{
    // 초기화 시키고 
    m_MaterialID = 0;
    m_vecMapInfo.clear();
    m_pkSelectFrame->ToggleRender( false );

    // 정보 저장 
    m_MaterialID = catalog_.m_PayItemID;
    for( std::map< DWORD, KAgitMap >::iterator mit = catalog_.m_mapMapList.begin() ; mit != catalog_.m_mapMapList.end() ; ++mit )
        m_vecMapInfo.push_back( mit->second );

    // 페이지 초기화 
    InitPage();
}

void KGCAgitMove::InitPage( void )
{
    if( m_vecMapInfo.empty() )
        return;

    int iTotalCount = static_cast<int>( m_vecMapInfo.size() );

    m_iTotalPage = iTotalCount / NUM_SLOT;
    if( iTotalCount % NUM_SLOT != 0 )
        ++m_iTotalPage;
    
    m_iCurPage = 1;
    SetPageStatic();
    SetMapSlot( m_iCurPage );
}

void KGCAgitMove::SetPageStatic( void )
{
    std::wostringstream stm;
    stm << m_iCurPage << L" / " << m_iTotalPage;
    m_pkStaticPage->SetText( stm.str() );
}

void KGCAgitMove::SetMapSlot( int iPage_ )
{
    if( iPage_ < 0 || m_iTotalPage < iPage_ )
        return;

    int iStartIndex = ( iPage_ - 1 ) * NUM_SLOT;
    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        m_apkSlot[i]->Clear();
        
        if( iStartIndex + i < static_cast<int>( m_vecMapInfo.size() ) )
            m_apkSlot[i]->SetMapInfo( m_vecMapInfo[ iStartIndex + i ] );
    }
}

void KGCAgitMove::OnClickSlot( int iIndex_ )
{
    if( iIndex_ < 0 || NUM_SLOT <= iIndex_ )
        return;

    if( m_apkSlot[iIndex_]->GetMapInfo().m_usMaxFlowerPot == 0 )
        return;

    // 선택 표시 프레임 
    D3DXVECTOR2 dxvPos = m_apkSlot[ iIndex_ ]->GetFixedWindowLocalPos();
    m_pkSelectFrame->SetWindowPos( D3DXVECTOR2(dxvPos.x + 1.0f, dxvPos.y + 2.0f) );
    m_pkSelectFrame->ToggleRender( true );

    // 이름도 표시해주세영
    m_pkStaticMapInfo->SetText( g_pkStrLoader->GetString( SiKGCAgitGameManager()->GetMapNameString( m_apkSlot[ iIndex_ ]->GetMapInfo().m_dwMapID ) ) );
    m_iCurSlot = iIndex_;

    // 화분도 표시해 주세영
    SetFlowerPotIcon();
}

void KGCAgitMove::OnClickPage( int iBtnType_ )
{
    if( iBtnType_ != BTN_PREV && iBtnType_ != BTN_NEXT )
        return;

    switch( iBtnType_ )
    {
    case BTN_PREV:
        if( m_iCurPage > 1 )
            --m_iCurPage;
        break;

    case BTN_NEXT:
        if( m_iCurPage < m_iTotalPage )
            ++m_iCurPage;
        break;
    }

    SetPageStatic();
    g_KDSound.Play( "984040" );
    SetMapSlot( m_iCurPage );
}

void KGCAgitMove::OnWatch( void )
{
    // 누가 있으면 미리보기 안됨
    if( SiKGCAgitGameManager()->GetAgitUserNum() != 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AGIT_MOVE_FAIL2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    if( m_iCurSlot < 0 || m_iCurSlot >= static_cast<int>(m_apkSlot.size() ) )
        return;

    KAgitMap kCurMap = m_apkSlot[ m_iCurSlot ]->GetMapInfo();

    if( kCurMap.m_nPrice > 0 )
    {
        std::string strMap = SiKGCAgitGameManager()->GetMapFileName( kCurMap.m_dwMapID );
        if( !strMap.empty()  )
        {
            // 배치된것 안보이게 하고 
            SiKGCAgitGameManager()->BeforeWatchAgit();
            SiKGCLayerManager()->LoadScript(strMap);
        }
    }
}

void KGCAgitMove::OnMove( void )
{
    // 누가 있으면 이사 안됨
    if( SiKGCAgitGameManager()->GetAgitUserNum() != 0 )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AGIT_MOVE_FAIL2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    // 돈 있나??
    KItem* pItem = NULL;
    g_pItemMgr->FindInventoryForItemID( m_MaterialID / 10, &pItem );

    if( pItem == NULL )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AGIT_MOVE_FAIL1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    if( m_iCurSlot < 0 || m_iCurSlot >= static_cast<int>(m_apkSlot.size() ) )
        return;

    KAgitMap kCurMap = m_apkSlot[ m_iCurSlot ]->GetMapInfo();
    if( pItem->m_nCount < kCurMap.m_nPrice )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AGIT_MOVE_FAIL1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    Result_BuyAgitShop = INT_MAX;
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_AGIT_MOVE,  "i", kCurMap.m_nPrice ), L"",
        KGCUIScene::GC_MBOX_USE_AGIT_MOVE, kCurMap.m_dwMapID, 0, true, true );
}

void KGCAgitMove::OnClose( void )
{
    this->ToggleRender( false );
}

void KGCAgitMove::SetFlowerPotIcon( void )
{
    if( m_iCurSlot == -1 )
        return;

    for( int i=0 ; i<NUM_FLOWER_POT ; ++i )
        m_apkIconFlowerPot[i]->ToggleRender( false );

    int iflowerPotNum = static_cast<int>( m_apkSlot[m_iCurSlot]->GetMapInfo().m_usMaxFlowerPot );
    if( iflowerPotNum > NUM_FLOWER_POT )
        iflowerPotNum = NUM_FLOWER_POT;

    for( int i=0 ; i<iflowerPotNum ; ++i )
        m_apkIconFlowerPot[i]->ToggleRender( true );
}

