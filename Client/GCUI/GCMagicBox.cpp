
#include "stdafx.h"
#include "GCMagicBox.h"

IMPLEMENT_CLASSNAME( KGCMagicBox );
IMPLEMENT_WND_FACTORY( KGCMagicBox );
IMPLEMENT_WND_FACTORY_NAME( KGCMagicBox, "gc_magic_box" );

KGCMagicBox::KGCMagicBox( void )
: m_pkBtnClose(NULL)
, m_pkBtnBuyAcientStone(NULL)
, m_pkBtnbuyKounatStone(NULL)
, m_pkBtnUseAncientStone(NULL)
, m_pkStaticAcientStone(NULL)
, m_pkStaticKounatStone(NULL)
, m_pkStaticCurrentHaveCount(NULL)
, m_pkStaticEventGuide(NULL)
, m_pkStaticAcientCount(NULL)
, m_pkStaticKounatcount(NULL)
, m_pkBtnRightPage(NULL)
, m_pkBtnLeftPage(NULL)
, m_pkSelectItemWnd(NULL)
, m_bCheckCanBuy( true )
, m_bCheckInventory( true )
{
    InitVariable();

    for(int iLoop = 0; iLoop < GC_CHAR_NUM; ++iLoop) {
        m_apkCharName[iLoop] = NULL;
        m_apkCharFace[iLoop] = NULL;
    }

    for(int iLoop = 0; iLoop < NUM_MAX_ITEM; ++iLoop) {
        m_apkMagicBoxItemWnd[iLoop] = NULL; 
    }

    for(int iLoop = 0; iLoop < NUM_MAX_PAGE_INDEX; ++iLoop) {
        m_apkStaticPageIndex[iLoop] = NULL; 
    }    
    //초기화 끝

    char szTemp[256];
    for(int iLoop = 0; iLoop < GC_CHAR_NUM; ++iLoop) {
        sprintf(szTemp, "char_name_%02d", iLoop);            LINK_CONTROL( szTemp, m_apkCharName[iLoop]          );
        sprintf(szTemp, "char_face_%02d", iLoop);            LINK_CONTROL( szTemp, m_apkCharFace[iLoop]          );
    }

    for(int iLoop = 0; iLoop < NUM_MAX_ITEM; ++iLoop) {
        sprintf(szTemp, "item_box_%02d", iLoop);    LINK_CONTROL( szTemp, m_apkMagicBoxItemWnd[iLoop]   );
    }
    LINK_CONTROL( "red_box", m_pkSelectItemWnd );

    for(int iLoop = 0; iLoop < NUM_MAX_PAGE_INDEX; ++iLoop) {
        sprintf(szTemp, "static_item_list_index_%02d", iLoop);    LINK_CONTROL( szTemp, m_apkStaticPageIndex[iLoop]   );
    }

    LINK_CONTROL( "btn_close",		            m_pkBtnClose             );
    LINK_CONTROL( "btn_buy_acient_stone",	    m_pkBtnBuyAcientStone             );
    LINK_CONTROL( "btn_buy_kounat_stone",	    m_pkBtnbuyKounatStone             );
    LINK_CONTROL( "btn_use_acient_stone",		    m_pkBtnUseAncientStone             );
    LINK_CONTROL( "btn_use_kounat_stone",		    m_pkBtnUseKounatStone             );
    LINK_CONTROL( "btn_use_select_acient_stone",		    m_pkBtnUseSelectAcientStone             );
    LINK_CONTROL( "btn_use_select_kounat_stone",		    m_pkBtnUseSelectKounatStone             );



    LINK_CONTROL( "btn_right_page",		        m_pkBtnRightPage             );
    LINK_CONTROL( "btn_left_page",		        m_pkBtnLeftPage             );
    LINK_CONTROL( "static_acient_stone",        m_pkStaticAcientStone             );
    LINK_CONTROL( "static_kounat_stone",        m_pkStaticKounatStone             );
    LINK_CONTROL( "static_current_have_count", m_pkStaticCurrentHaveCount      );


    LINK_CONTROL( "static_event_guide",		    m_pkStaticEventGuide             );
    LINK_CONTROL( "static_acient_count",	    m_pkStaticAcientCount             );
    LINK_CONTROL( "static_kounat_count",	    m_pkStaticKounatcount             );


}

void KGCMagicBox::InitVariable()
{
    m_iRealPageIndex = 1;
    m_iItemListPageLimit = 100;
    m_iSelectCharacter = -1;
    m_dwMyKonautStoneCount = -1;
    m_dwMyAncientStoneCount = -1;

    m_nViewMaxIndex = 10;   //페이지의 리얼 인덱스(실제로 보이는 인덱스)
    m_nViewMinIndex = 0;   //페이지의 리얼 인덱스(실제로 보이는 인덱스)
    if( m_pkSelectItemWnd != NULL ) {
        m_pkSelectItemWnd->SetBlank();
    }
}

void KGCMagicBox::OnInitialize( void )
{ 

}

void KGCMagicBox::CalculateLimitPageByCharacter( OUT int& _itemPageLimit )
{
    std::map<int, std::vector<SMagicBoxInfo>>::iterator mit = m_mapItemList.find( m_iSelectCharacter );

    if( mit == m_mapItemList.end() )
    {
        _itemPageLimit = 0;
        return;
    }

    std::vector<SMagicBoxInfo>::size_type nSize = mit->second.size();
    if( nSize == 0 )
    {
        _itemPageLimit = 0;
        return;
    }

    _itemPageLimit = nSize/10 + (nSize%10?1:0);
    return;

}

void KGCMagicBox::SetSelectableList( std::map<GCITEMID, std::pair<int,int> >& _mapItemInfo )
{
    m_mapItemList.clear();
    std::map<GCITEMID, std::pair<int,int> >::iterator itor = _mapItemInfo.begin();
    for ( ; itor != _mapItemInfo.end() ; ++itor ) {

        GCITEMID gcItemID = itor->first/10;

        GCItem* pItem = g_pItemMgr->GetItemData( gcItemID );
        if( pItem == NULL )
            continue;

        SMagicBoxInfo sMagicItemInfo;
        sMagicItemInfo.m_gcItemID = gcItemID;
        sMagicItemInfo.m_iExtraKeyCount = itor->second.first;
        sMagicItemInfo.m_iGetItemCount = itor->second.second;

        for( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
            DWORD dwCharFlag = g_pItemMgr->GetCharFlag( i );

            if( dwCharFlag & pItem->dwCharType ) {
                m_mapItemList[i].push_back(sMagicItemInfo);
            }
        }

    }


    //받은 리스트를 박스에 뿌려준다.    
    InitVariable();
    SetChar( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType );
}

void KGCMagicBox::ClickItemList( GCITEMID _selectItemID )
{
    std::map<int, std::vector<SMagicBoxInfo>>::iterator mit = m_mapItemList.find( m_iSelectCharacter );
    if( mit == m_mapItemList.end() )
        return;

    std::vector<SMagicBoxInfo>::iterator vit = std::find_if( mit->second.begin(), mit->second.end(), boost::bind( &SMagicBoxInfo::m_gcItemID, _1 ) == _selectItemID );
    if( vit == mit->second.end() )
        return;

    SMagicBoxInfo& sItemInfo = (*vit);
    m_pkSelectItemWnd->SetCount( sItemInfo.m_iExtraKeyCount );
    m_pkSelectItemWnd->SetImage( _selectItemID );

    // 구매 제한 아이템인지 체크 
    Result_CanBuyCashItem = INT_MAX;
    KP2P::GetInstance()->Send_CanBuyCashItem( _selectItemID * 10 );
    g_MyD3D->WaitForServerAck( Result_CanBuyCashItem, INT_MAX, 3000, TIME_OUT_VALUE );

    switch( Result_CanBuyCashItem  )
    {
    case 0:
    case 1:
        m_bCheckCanBuy = true;
        break;
    case 2:
        m_bCheckCanBuy = false;
        break;
    case 3:
    case 4:                 
        m_bCheckInventory = false;
        break;
    default:
        m_bCheckCanBuy = true;
        m_bCheckInventory = true;
        break;
    }
    
}

void KGCMagicBox::OnCreate( void )
{

    //목록을 초기화 한다.
    InitVariable();

    for ( int i = 0 ; i < NUM_MAX_ITEM ; ++i ) {
        m_apkMagicBoxItemWnd[i]->SetBlank();
    }
    m_pkSelectItemWnd->SetBlank();

    SetSelectPageIndex( 1 );


    m_pkBtnClose->SetHotKey( DIK_ESCAPE );


    for( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
        m_apkCharName[i]->InitState( false );
        m_apkCharFace[i]->InitState( false );
    }

    for( int i = 0 ; i < NUM_MAX_ITEM ; ++i ) {
        if( m_apkMagicBoxItemWnd[i] != NULL) {
            m_apkMagicBoxItemWnd[i]->InitState( true, true, this );
        }
    }
    m_pkSelectItemWnd->InitState(true);

    int viewMaxIndex = 0;
    for( int i = 0 ; i < NUM_MAX_PAGE_INDEX ; ++i ) {
        if ( m_apkStaticPageIndex[i] != NULL ) {
            viewMaxIndex++;
            m_apkStaticPageIndex[i]->InitState( true, true, this );
            m_apkStaticPageIndex[i]->SetStringType( "very_small_num", true );
            m_apkStaticPageIndex[i]->SetNumber( i+1 );
        }
    }
    m_nViewIndexCount = viewMaxIndex;
    m_nViewMaxIndex = m_nViewIndexCount;

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnBuyAcientStone->InitState( true, true, this );
    m_pkBtnbuyKounatStone->InitState( true, true, this );
    m_pkBtnUseAncientStone->InitState( true, true, this ); 
    m_pkBtnUseKounatStone->InitState( true, true, this ); 
    m_pkBtnUseSelectAcientStone->InitState( false, true, this ); 
    m_pkBtnUseSelectKounatStone->InitState( false, true, this ); 

    m_pkBtnRightPage->InitState( true, true, this ); 
    m_pkBtnLeftPage->InitState( true, true, this ); 


    m_pkStaticAcientStone->InitState( true );
    m_pkStaticKounatStone->InitState( true );
    m_pkStaticCurrentHaveCount->InitState( true );
    m_pkStaticEventGuide->InitState( true );


    m_pkStaticAcientStone->SetText( g_pkStrLoader->GetString( STR_ID_ANCIENT_STONE ));
    m_pkStaticAcientStone->SetFontColor(  D3DCOLOR_ARGB(255, 255, 185, 0) );
    m_pkStaticKounatStone->SetText( g_pkStrLoader->GetString( STR_ID_KOUNAT_STONE ));
    m_pkStaticKounatStone->SetFontColor(  D3DCOLOR_ARGB(255, 255, 185, 0) );

    m_pkStaticCurrentHaveCount->SetFontSpace(1);
    m_pkStaticCurrentHaveCount->SetText( g_pkStrLoader->GetString( STR_ID_CURRNET_HAVE_COUNT ));
    m_pkStaticCurrentHaveCount->SetFontOutline( true, D3DCOLOR_ARGB(255, 25, 46, 67) );

#if !defined ( NATION_USA ) && !defined ( NATION_PHILIPPINE )
    m_pkStaticEventGuide->SetFontSpace(1);
#endif

    std::wstring wString;
    wString = g_pkStrLoader->GetString(STR_ID_MAGIC_BOX_DESC1) + L"\n";
    wString +=g_pkStrLoader->GetString(STR_ID_MAGIC_BOX_DESC2) + L"\n";
    wString +=g_pkStrLoader->GetString(STR_ID_MAGIC_BOX_DESC3) + L"\n";
    wString +=g_pkStrLoader->GetString(STR_ID_MAGIC_BOX_DESC4) + L"\n";
    wString +=g_pkStrLoader->GetString(STR_ID_MAGIC_BOX_DESC5);

    m_pkStaticEventGuide->SetTextAutoMultiline( wString );
    

    for( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
        m_apkCharName[i]->InitState( false );
        m_apkCharFace[i]->InitState( false );
    }  

    InitButtonPos();

    static bool bInit = true;
    if( bInit ) {
        bInit = false;

        D3DXVECTOR2 m_vtMagicBoxItemWndPos      = m_apkMagicBoxItemWnd[0]->GetFixedWindowLocalPos();
        POINT m_ptMagicBoxItemWndWH;
        m_ptMagicBoxItemWndWH.x = m_apkMagicBoxItemWnd[0]->GetWidth();
        m_ptMagicBoxItemWndWH.y = m_apkMagicBoxItemWnd[0]->GetHeight();

        const float fGapX = 3.0f;
        const float fGapY = 2.0f;

        for( int i = 0 ; i < NUM_MAX_ITEM ; ++i ) {
            if( m_apkMagicBoxItemWnd[i] != NULL) {
                D3DXVECTOR2 vtTemp( m_vtMagicBoxItemWndPos );
                vtTemp.x = m_vtMagicBoxItemWndPos.x + (fGapX + m_ptMagicBoxItemWndWH.x) * ( i % 5 );
                vtTemp.y = m_vtMagicBoxItemWndPos.y + (fGapY + m_ptMagicBoxItemWndWH.y) * ( i / 5 );


                m_apkMagicBoxItemWnd[i]->SetFixedWindowLocalPos( vtTemp );
            }
        }


        //숫자 정렬
        D3DXVECTOR2 m_vtPageIndexPos = m_apkStaticPageIndex[0]->GetFixedWindowLocalPos();
        POINT m_ptPageIndexWH;
        m_ptPageIndexWH.x = m_apkStaticPageIndex[0]->GetWidth();
        m_ptPageIndexWH.y = m_apkStaticPageIndex[0]->GetHeight();
        const float fPageIndexGapX = 0.0f;
        for(int iLoop = 0; iLoop < NUM_MAX_PAGE_INDEX; ++iLoop) {
            if ( m_apkStaticPageIndex[iLoop] ) {
                D3DXVECTOR2 vtTemp( m_vtPageIndexPos );
                vtTemp.x = m_vtPageIndexPos.x + ( fPageIndexGapX + m_ptPageIndexWH.x ) * iLoop;
                m_apkStaticPageIndex[iLoop]->SetFixedWindowLocalPos( vtTemp );
            }
        }
    }
}

void KGCMagicBox::InitButtonPos()
{   

    D3DXVECTOR2 vtFacePos = m_apkCharFace[GC_CHAR_ELESIS]->GetFixedWindowLocalPos();
    D3DXVECTOR2 vtNamePos = m_apkCharName[GC_CHAR_ELESIS]->GetFixedWindowLocalPos();
   

    for( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
        m_apkCharFace[i]->SetWindowPos( vtFacePos );
        m_apkCharName[i]->SetWindowPos( vtNamePos );
    }

}
void KGCMagicBox::OnCreateComplete( void )
{
    KItem* pItem = NULL;
    int nTemp = 0;
    m_pkStaticAcientCount->InitState( true );
    m_pkStaticAcientCount->SetStringType( "very_small_num13", true );
    m_pkStaticAcientCount->SetWriteToRight( false ); 
    g_pItemMgr->FindInventoryForItemID( ITEM_ID_ANCIENT_STONE_USE, &pItem );
    pItem ? nTemp = pItem->m_nCount : nTemp = 0;
    m_pkStaticAcientCount->SetNumber( nTemp );

    pItem = NULL;
    m_pkStaticKounatcount->InitState( true );
    m_pkStaticKounatcount->SetStringType( "very_small_num13", true );
    m_pkStaticKounatcount->SetWriteToRight( false );
    g_pItemMgr->FindInventoryForItemID( ITEM_ID_KONAUT_STONE_USE, &pItem );
    pItem ? nTemp = pItem->m_nCount : nTemp = 0;
    m_pkStaticKounatcount->SetNumber( nTemp );

    m_pkSelectItemWnd->SetBlank(); 

    InitVariable();
    SetChar( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType );
}

KGCMagicBox::~KGCMagicBox( void )
{
}

void KGCMagicBox::SetSelectPageIndex( int _nIndex )
{
    if ( _nIndex < 0 || _nIndex > NUM_MAX_PAGE_INDEX ) {
        return;
    }
    for( int i = 0 ; i < NUM_MAX_PAGE_INDEX ; ++i ) {
        if( m_apkStaticPageIndex[i] == NULL )
            break;

        m_apkStaticPageIndex[i]->SetSelcted( false );
        m_apkStaticPageIndex[i]->SetStringType( "very_small_num" );
        
    }
    m_apkStaticPageIndex[_nIndex]->SetSelcted( true );
    m_apkStaticPageIndex[_nIndex]->SetStringType( "very_small_num_yellow" );
}

void KGCMagicBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose,                KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );

    GCWND_MSG_MAP( m_pkBtnRightPage,            KD3DWnd::D3DWE_BUTTON_CLICK,    ClickNextPage );
    GCWND_MSG_MAP( m_pkBtnLeftPage,             KD3DWnd::D3DWE_BUTTON_CLICK,    ClickPrePage );

    GCWND_MSG_MAP( m_pkBtnUseKounatStone,       KD3DWnd::D3DWE_BUTTON_CLICK,    ClickKounatStone );
    GCWND_MSG_MAP( m_pkBtnUseAncientStone,      KD3DWnd::D3DWE_BUTTON_CLICK,    ClickAncientStone );

    GCWND_MSG_MAP( m_pkBtnUseSelectKounatStone, KD3DWnd::D3DWE_BUTTON_CLICK,    ClickKounatStone );
    GCWND_MSG_MAP( m_pkBtnUseSelectAcientStone, KD3DWnd::D3DWE_BUTTON_CLICK,    ClickAncientStone );



    for( int i = 0 ; i < NUM_MAX_PAGE_INDEX ; ++i ) {
        if( m_apkStaticPageIndex[i] == NULL )
            continue;

        if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && m_apkStaticPageIndex[i] == event.m_pWnd ) {            
            SetItemPage( i + m_nViewMinIndex );   
        }
        else if (m_apkStaticPageIndex[i]->IsSelcted() == false ) {
            if ( event.m_pWnd == m_apkStaticPageIndex[i] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ){
                m_apkStaticPageIndex[i]->SetStringType( "very_small_num_yellow" );
            }
            else if ( event.m_pWnd == m_apkStaticPageIndex[i] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE ){
                m_apkStaticPageIndex[i]->SetStringType( "very_small_num" );
            }
        }
    }

                            
    if(event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        if(OnClickBuyBtn(event))
            return;


        for(int iLoop = 0; iLoop < NUM_MAX_ITEM; ++iLoop) {
            if ( m_apkMagicBoxItemWnd[iLoop] == event.m_pWnd ){
                if ( m_apkMagicBoxItemWnd[iLoop] != NULL ) {
                    GCITEMID itemID = m_apkMagicBoxItemWnd[iLoop]->GetItemID();
                    if( itemID == 0 )
                        break;
                    ClickItemList( itemID );
                }
            }
        }
    }
}

void KGCMagicBox::ClickKounatStone( )
{
    if( m_pkSelectItemWnd->GetItemID() == 0 ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ERROR_MAGICBOX5), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    // 구매 제한
    if( false == m_bCheckCanBuy ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LIMITED_BUY ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true, true );
        return;
    }

    // 인벤 부족 
    if( false == m_bCheckInventory ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL_ERROR1 ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true, true );
        return;
    }

    // 유저가 키 아이템 가지고 있나 검사하고 날린다. 
    if( g_pItemMgr->FindInventoryForItemID( ITEM_ID_KONAUT_STONE_USE ) )
        KP2P::GetInstance()->Send_UseMagicBoxReq( m_pkSelectItemWnd->GetItemID() * 10, ITEM_ID_KONAUT_STONE_USE * 10 );
    else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_ERROR_MAGICBOX3), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );

    m_pkBtnUseSelectKounatStone->ToggleRender( true );
    m_pkBtnUseSelectAcientStone->ToggleRender( false );
}

void KGCMagicBox::ClickAncientStone( )
{
    if( m_pkSelectItemWnd->GetItemID() == 0 ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ERROR_MAGICBOX5), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    // 구매 제한
    if( false == m_bCheckCanBuy ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LIMITED_BUY ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true, true );
        return;
    }

    // 인벤 부족 
    if( false == m_bCheckInventory ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL_ERROR1 ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true, true );
        return;
    }

    if( g_pItemMgr->FindInventoryForItemID( ITEM_ID_ANCIENT_STONE_USE ) )
        KP2P::GetInstance()->Send_UseMagicBoxReq( m_pkSelectItemWnd->GetItemID() * 10, ITEM_ID_ANCIENT_STONE_USE * 10 );
    else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_ERROR_MAGICBOX3), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );

    m_pkBtnUseSelectKounatStone->ToggleRender( false );
    m_pkBtnUseSelectAcientStone->ToggleRender( true );
}

void KGCMagicBox::ClickNextPage( )
{
    if ( m_iRealPageIndex < m_iItemListPageLimit ) {
        m_iRealPageIndex++;
        //페이지 새로고침
        SetItemPage( m_iRealPageIndex );
    }
}

void KGCMagicBox::ClickPrePage( )
{
    if ( m_iRealPageIndex > 0 ) {   
        m_iRealPageIndex--;
        //페이지 새로고침
        SetItemPage( m_iRealPageIndex );
    }
}

void KGCMagicBox::SetItemPage( int _realPageIndex )
{
    //_pageIndex가 현재 보이고 있는 인덱스의 범위와 다르면 범위를 같도록 설정한다.
    if( _realPageIndex < 0 || _realPageIndex >= m_iItemListPageLimit )
        return;

    m_iRealPageIndex = _realPageIndex;
    int nBeginGap = m_iRealPageIndex - (NUM_MAX_PAGE_INDEX/2);
    int nEndGap = m_iItemListPageLimit - (m_iRealPageIndex + (NUM_MAX_PAGE_INDEX/2) );

    int nFirstPage = nBeginGap;
    int nLastPage = nEndGap;

    if( nBeginGap < 0 && nEndGap < 0 ) {
        nFirstPage = 0;
        nLastPage = m_iItemListPageLimit;
    }
    else if( nBeginGap < 0 && nEndGap > 0 ) {
        nFirstPage = 0;
        nLastPage = (m_iRealPageIndex + (NUM_MAX_PAGE_INDEX/2) ) - nBeginGap;
    }
    else if( nBeginGap > 0 && nEndGap < 0 ) {
        nFirstPage = nBeginGap + nEndGap;
        nLastPage = (m_iRealPageIndex + (NUM_MAX_PAGE_INDEX/2) );
    }
    else {
        nFirstPage = nBeginGap;
        nLastPage = (m_iRealPageIndex + (NUM_MAX_PAGE_INDEX/2) );
    }

    if( nFirstPage < 0 )
        nFirstPage = 0;

    if( nLastPage > m_iItemListPageLimit )
        nLastPage = m_iItemListPageLimit;

    m_nViewMinIndex = nFirstPage;
    m_nViewMaxIndex = nLastPage;

    for( int i = 0 ; i < NUM_MAX_PAGE_INDEX ; ++i ) {
        if( i >= nLastPage ) {
            m_apkStaticPageIndex[i]->ToggleRender(false);
            continue;
        }

        m_apkStaticPageIndex[i]->ToggleRender(true);
        m_apkStaticPageIndex[i]->SetNumber( i + nFirstPage + 1 );

        if( i+nFirstPage == m_iRealPageIndex )
            SetSelectPageIndex( i );
    }

    std::map<int, std::vector<SMagicBoxInfo>>::iterator mit = m_mapItemList.find( m_iSelectCharacter );
    if( mit == m_mapItemList.end() )
        return;

    std::vector<SMagicBoxInfo>& vecItemList = mit->second;
    if( vecItemList.empty() )
        return;

    if( m_iRealPageIndex * NUM_MAX_ITEM > static_cast<int>(vecItemList.size()) ) {
        return;
    }

    int nStartIndex = m_iRealPageIndex * NUM_MAX_ITEM;
    for( int i = 0 ; i < NUM_MAX_ITEM ; ++i ) {
        if( nStartIndex + i >= static_cast<int>(vecItemList.size()) ) {
            m_apkMagicBoxItemWnd[i]->SetBlank();
            continue;
        }
        
        SMagicBoxInfo& sInfo = vecItemList[nStartIndex+i];
        m_apkMagicBoxItemWnd[i]->SetImage( sInfo.m_gcItemID ); 
        m_apkMagicBoxItemWnd[i]->SetCount( sInfo.m_iExtraKeyCount ); 
    }
}

bool KGCMagicBox::OnClickBuyBtn( const KActionEvent &event )
{
    if( m_pkBtnBuyAcientStone == event.m_pWnd ) {
        DWORD dwItemID = ITEM_ID_ANCIENT_STONE_BUY;
        BuyItem( dwItemID );
        return true;
    }
    else if ( m_pkBtnbuyKounatStone == event.m_pWnd ) {
        DWORD dwItemID = ITEM_ID_KONAUT_STONE_BUY;
        BuyItem( dwItemID ); 
        return true; 
    }
    return false;
}

void KGCMagicBox::BuyItem( DWORD& dwBuyItemID )
{
    GCItem* pItem = g_pItemMgr->GetItemData( dwBuyItemID );
    if( pItem->eMoneyType == EMT_CASH )
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( dwBuyItemID );
#else
    #ifdef GAME_CASH
        g_pkUIScene->EnableBuyCashItemWnd( dwBuyItemID, false, true );
    #else
        g_MyD3D->m_kItemMgr.BuyItem( dwBuyItemID, pItem->iItemTypeValue  );
    #endif
#endif
    }
    else
    {
        std::wostringstream strmText1;
        std::wostringstream strmText2;
#if defined ( _PORTUGUESE ) || defined( _SPANISH )
        strmText1 << pItem->strItemName;
        strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", (int)pItem->dwPrice );
#elif defined( _ENGLISH )
        strmText1<<g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "li", pItem->strItemName, (int)pItem->dwPrice );
#else
        strmText1 << pItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
        strmText2 << (int)pItem->dwPrice << ' ' << g_pkStrLoader->GetString( STR_ID_BUY_PRICE );
#endif
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
            strmText1.str(), strmText2.str(),
            KGCUIScene::GC_MBOX_USE_BUY, dwBuyItemID, pItem->iItemTypeValue, true, true );
    }
}


void KGCMagicBox::OnClose()
{
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowZOrder(D3DWZ_NOTOPMOST);
    g_pkUIScene->m_pkCharacterSelectBox->OnClose();
    
    m_bCheckCanBuy = true;
    m_bCheckInventory = true;

    Destroy();
}

void KGCMagicBox::SetChar(int _iCharType)
{
    if( GC_CHAR_INVALID >= _iCharType || _iCharType >= GC_CHAR_NUM )
        return;


    m_iSelectCharacter = _iCharType;
    CalculateLimitPageByCharacter( m_iItemListPageLimit );

    for( int i = 0; i < GC_CHAR_NUM; ++i )
    {
        m_apkCharName[i]->ToggleRender( i == _iCharType );
        m_apkCharFace[i]->ToggleRender( i == _iCharType );
    }


    m_pkBtnUseSelectAcientStone->ToggleRender( false );
    m_pkBtnUseSelectKounatStone->ToggleRender( false );
    m_pkSelectItemWnd->SetBlank();
    SetItemPage(0);

	//	선택된 캐릭터로 UI를 갱신한다.
	g_pkUIScene->m_pkMyInfoScene->UpdateCharacterSelect( _iCharType );
}

void KGCMagicBox::FrameMoveInEnabledState( void )
{
#if defined ( NATION_USA )
    if ( m_mapItemList.empty() ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_WC_STRING9 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        OnClose();
        return;
    }
#else
    if( g_MyD3D->m_pStateMachine && g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_SERVER ){
        OnClose();
    }
#endif

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );


    if ( m_pkSelectItemWnd->GetItemID() != 0 && m_pkSelectItemWnd->CheckPosInWindowBound( vMousePos ) ) {
        g_pkUIScene->m_pkItemInformationBox->DrawItemInfoBox(m_pkSelectItemWnd->GetItemID(), vMousePos);
    }
    else {
        for ( int i = 0 ; i < NUM_MAX_ITEM ; i++) {
            if( m_apkMagicBoxItemWnd[i] == NULL )
                continue;

            if( m_apkMagicBoxItemWnd[i]->GetItemID() == 0 )
                continue;

            if( m_apkMagicBoxItemWnd[i]->CheckPosInWindowBound( vMousePos ) )
            {
                g_pkUIScene->m_pkItemInformationBox->DrawItemInfoBox(m_apkMagicBoxItemWnd[i]->GetItemID(), vMousePos);
                break;
            }
            else {
                g_pkUIScene->m_pkItemInformationBox->DrawItemInfoBox( -1, vMousePos );           
            }
        }         
    }
}

void KGCMagicBox::ResultEvent( KEVENT_USE_MAGIC_BOX_ACK result_ )
{
    //int                 m_nOK; // 결과값
    //GCITEMID            m_TargetItemID; // 키가 사용된 아이템ID
    //int                 m_nLeftKeyCount; // 아이템 획득까지 남은 키 개수
    //KItem               m_kKeyItem; // 사용한 키 아이템(수량)
    //bool                m_bClear; // 아이템 획득 여부
    //int                 m_nClearCount; // 아이템 획득 회수
    //std::vector<KItem>  m_vecItem; // 획득한 아이템
    //
    if ( result_.m_nOK != 0 )
        return;

    m_pkSelectItemWnd->SetCount(result_.m_nLeftKeyCount);

    for ( int i = 0 ; i < NUM_MAX_ITEM ; i ++ ) {
        if (m_apkMagicBoxItemWnd[i]->GetItemID() == result_.m_TargetItemID/10) {
            m_apkMagicBoxItemWnd[i]->SetCount( result_.m_nLeftKeyCount );
            break;
        }
    }

    GCITEMID dwUseItem = result_.m_kKeyItem.m_ItemID / 10;
    if ( dwUseItem == ITEM_ID_ANCIENT_STONE_USE ) {
        m_pkStaticAcientCount->SetNumber( result_.m_kKeyItem.m_nCount );
        m_pkBtnUseSelectAcientStone->ToggleRender(true);
        m_pkBtnUseSelectKounatStone->ToggleRender(false);

    }
    else if ( dwUseItem == ITEM_ID_KONAUT_STONE_USE ) {
        m_pkStaticKounatcount->SetNumber( result_.m_kKeyItem.m_nCount );
        m_pkBtnUseSelectAcientStone->ToggleRender(false);
        m_pkBtnUseSelectKounatStone->ToggleRender(true);
    }

    //1.마력석 소모 이펙트
    //    Bigkung_Parm_01
    //    Bigkung_Parm_02
    //    Get_GP_Effect_02


    //    2.아이템획득 이펙트
    //    Box_Open_01
    //    Box_Open_02
    //    Box_Open_03
    //    Box_Open_04
    //    Box_Open_05
    //    BoxCrash_01

    if ( result_.m_bClear ) {
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Box_Open_01", 0.03f, 0.04f, 0.5f ), GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Box_Open_02", 0.03f, 0.04f, 0.5f ), GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Box_Open_03", 0.03f, 0.04f, 0.5f ), GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Box_Open_04", 0.03f, 0.04f, 0.5f ), GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Box_Open_05", 0.03f, 0.04f, 0.5f ), GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "BoxCrash_01", 0.03f, 0.04f, 0.5f ), GC_LAYER_UI);
        g_KDSound.Play( "magicbox_get_item" );
    }
    else {        
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Kungji_Punch_01",  -0.29f, 0.43f, 0.5f ), GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Kungji_Punch_02",  -0.29f, 0.43f, 0.5f ), GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Get_GP_Effect_02", -0.29f, 0.43f, 0.5f ), GC_LAYER_UI);
        g_KDSound.Play( "magicbox_use_stone" );
    }

    //m_TargetItemID    //을 아이템 윈도우에서 찾아서 수량을 갱신한다.
    //컨테이너의 수량을 갱신한다. //윈도우에 보이는 수량을 갱신한다.

    GCITEMID targetItemID = result_.m_TargetItemID/10;
    std::map<int, std::vector<SMagicBoxInfo>>::iterator mit = m_mapItemList.begin();
    for( ; mit != m_mapItemList.end() ; ++mit ) {
        std::vector<SMagicBoxInfo>::iterator vit = std::find_if( mit->second.begin(), mit->second.end(), boost::bind( &SMagicBoxInfo::m_gcItemID, _1 ) == targetItemID );

        if( vit == mit->second.end() )
            continue;

        SMagicBoxInfo& sTargetInfo = (*vit);
        sTargetInfo.m_iExtraKeyCount = result_.m_nLeftKeyCount;
    }


    //사용한 아이템을 깎는다.m_kKeyItem
    //g_pItemMgr->DecInventoryDurationItemForItemIdWithOutAck( result_.m_TargetItemID );
    //g_pItemMgr->DecInventoryDurationItemForItemIdWithOutAck( result_.m_kKeyItem.m_ItemID ); 
    g_pItemMgr->DecInventoryDurationItemForItemIdWithOutAck( result_.m_kKeyItem.m_ItemID / 10 ); 

    //받은 아이템을 인벤토리에 넣어준다.
    std::vector<KItem>::iterator vit = result_.m_vecItem.begin();
    for ( ; vit != result_.m_vecItem.end(); ++vit )
    {
        g_pItemMgr->m_kInventory.AddItem( (*vit), true );
        g_pkUIScene->m_pkTreaureItemBox->SetItem( (*vit).m_ItemID );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, (*vit).m_ItemID/10, 0, false, true );
    }
    //인벤토리를 리프레시 한다.
    g_pkUIScene->m_pkMyInfoScene->UpdateData();
    g_pkUIScene->m_pkMyInfoScene->UpdateInventoryCapacity();

}
