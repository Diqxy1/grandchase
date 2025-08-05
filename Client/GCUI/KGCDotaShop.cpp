#include "stdafx.h"
#include "KGCDotaShop.h"

IMPLEMENT_CLASSNAME( KGCDotaShop );
IMPLEMENT_WND_FACTORY( KGCDotaShop );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaShop, "gc_dota_shop" );

KGCDotaShop::KGCDotaShop( void )
: m_pkBack( NULL )
, m_pkImgSelectItem( NULL )
, m_pkScroll( NULL )
, m_pkBtnClose( NULL )
, m_pkBtnBuy( NULL )
, m_pkStaticMyPoint( NULL )
, m_pkStaticItemPoint( NULL )
, m_pkStaticPoint( NULL )
, m_pkStaticItem( NULL )
, m_pkStaticItemName( NULL )
, m_pkShowBuyItem( NULL )
, m_pkShowBuyItemDesc( NULL )
, m_pkShopDesc( NULL )
, m_iScrollIdx( 0 )
{
    LINK_CONTROL( "background2", m_pkBack );
    LINK_CONTROL( "img_selectitem", m_pkImgSelectItem );
    LINK_CONTROL( "scroll", m_pkScroll );
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_buy", m_pkBtnBuy );
    LINK_CONTROL( "static_mydpoint", m_pkStaticMyPoint );
    LINK_CONTROL( "static_itemdpoint", m_pkStaticItemPoint );
    LINK_CONTROL( "static_descpoint", m_pkStaticPoint );
    LINK_CONTROL( "static_descitemname", m_pkStaticItemName );
    LINK_CONTROL( "static_descitem", m_pkStaticItem );
    LINK_CONTROL( "img", m_pkShowBuyItem );
    LINK_CONTROL( "static_desc", m_pkShowBuyItemDesc );
    LINK_CONTROL( "static_shopdesc", m_pkShopDesc );
}

KGCDotaShop::~KGCDotaShop( void )
{
    std::vector<GCDeviceTexture*>::iterator vecIter = m_vecpItemImg.begin();
    for(; vecIter != m_vecpItemImg.end(); ++vecIter )
    {
        SAFE_RELEASE( (*vecIter) );
    }
    m_vecpItemImg.clear();
}

void KGCDotaShop::InitData() 
{
    std::vector<KGCDPointItemBox*>::iterator vecBoxIter = m_vecItemBox.begin();
    for(; vecBoxIter != m_vecItemBox.end(); ++vecBoxIter)
        (*vecBoxIter)->SetItemTexture( NULL );

    m_iSelectItem = 0;

    // 키락 풀기
    g_MyD3D->SetEnableKeyInput( true );
}

void KGCDotaShop::OnDestroy( void )
{
    InitData();
}

void KGCDotaShop::OnCreateComplete( void )
{
    // 도타템플릿에서 상점에 대한 현재 정보 얻어오기
    SDotaTemplate *pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
    if( pDotaTemplate )
    {
        m_sDSItemInfo = pDotaTemplate->sDPointShopItemList;

        if( m_pkImgSelectItem )
            m_pkImgSelectItem->InitState( true );
        if( m_pkScroll )
        {
            m_iScrollIdx = 0;
            m_pkScroll->InitState( true, true, this );
            m_pkScroll->SetHeight( 187 );
            m_pkScroll->SetScrollPos( m_iScrollIdx );
            m_pkScroll->SetScrollGap( 1 );
            m_pkScroll->SetThumbYGab( 0 );
            m_pkScroll->SetScrollPageSize( 1 );
            m_pkScroll->SetScrollRangeMin( 0 );
            m_pkScroll->SetScrollRangeMax( ((m_sDSItemInfo.vecItemList.size() - 9) / 4) + 2 );
            m_pkScroll->ResizeBtn();
        }
        if( m_pkBtnClose )
        {
            m_pkBtnClose->SetHotKey( DIK_ESCAPE );
            m_pkBtnClose->InitState( true, true, this );
            m_pkBtnClose->DisconnectAllEventProcedure();
            m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCDotaShop::OnClickClsoe );
            m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCDotaShop::OnEnterCursor );
        }
        if( m_pkBtnBuy )
        {
            m_pkBtnBuy->SetHotKey( DIK_RETURN );
            m_pkBtnBuy->InitState( true, true, this );
            m_pkBtnBuy->DisconnectAllEventProcedure();
            m_pkBtnBuy->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCDotaShop::OnClickBuy );
            m_pkBtnBuy->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCDotaShop::OnEnterCursor );
        }
        if( m_pkStaticMyPoint )
            m_pkStaticMyPoint->InitState( true );
        if( m_pkStaticItemPoint )
            m_pkStaticItemPoint->InitState( true );
        if( m_pkStaticPoint )
        {
            m_pkStaticPoint->SetFontSpace( 2.5f );
            m_pkStaticPoint->SetFontColor( D3DCOLOR_ARGB(255, 213, 210, 174) );
            m_pkStaticPoint->SetFontOutline( true, D3DCOLOR_ARGB(255, 17, 28, 44) );
            m_pkStaticPoint->SetText( g_pkStrLoader->GetString( 8732 ) );
            m_pkStaticPoint->InitState( true );
        }
        if( m_pkStaticItem )
        {
#if defined ( NATION_USA ) || defined ( NATION_PHILIPPINE )
            m_pkStaticItem->SetFontSpace( 0 );
#else
            m_pkStaticItem->SetFontSpace( 1.5f );
#endif
            m_pkStaticItem->SetMultiLine( true );
            m_pkStaticItem->InitState( true );
        }
        if( m_pkStaticItemName )
        {
#if defined ( NATION_USA ) || defined ( NATION_PHILIPPINE )
            m_pkStaticItemName->SetFontSpace( 0 );
#else
            m_pkStaticItemName->SetFontSpace( 2.5f );
#endif
            m_pkStaticItemName->SetFontColor( D3DCOLOR_ARGB(255, 177, 212, 229) );
            m_pkStaticItemName->SetFontOutline( true, D3DCOLOR_ARGB(255, 17, 28, 44) );
            m_pkStaticItemName->InitState( true );
        }
        if( m_pkShowBuyItem )
            m_pkShowBuyItem->InitState( false );
        if( m_pkShowBuyItemDesc )
        {
            m_pkShowBuyItemDesc->SetAlign( DT_CENTER );
            m_pkShowBuyItemDesc->InitState( false );
        }
        if( m_pkShopDesc )
        {
            m_pkShopDesc->InitState( true );
            m_pkShopDesc->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_APOINTSHOP_DESC) );
        }

        InitData();         // 데이터 초기화
        CreateItemBox();    // 아이템 박스 생성
        CreateItemImg();    // 아이템 이미지 로드
        RefreshMyPoint();   // 보유 DPoint처리
        RefreshSelectItem();// 선택된 아이템 처리
        RefreshItemBox();   // 아이템 박스 셋팅

        // 키락 걸기
        g_MyD3D->SetEnableKeyInput( false );
    }
}

void KGCDotaShop::CreateItemImg()
{
    if( m_vecpItemImg.empty() )
    {
        char szTemp[128];
        GCDeviceTexture* pTex;
        std::vector<pair<int, int>>::iterator vecIter = m_sDSItemInfo.vecItemList.begin();
        for(; vecIter != m_sDSItemInfo.vecItemList.end(); ++vecIter)
        {
            sprintf(szTemp, "item%02d.dds", vecIter->first);
            pTex = g_pGCDeviceManager2->CreateTexture( szTemp );
            m_vecpItemImg.push_back( pTex );
        }
    }
}

void KGCDotaShop::CreateItemBox() 
{
    if( m_vecItemBox.empty() )
    {
        int iCount = 0;
        D3DXVECTOR2 vOriPos = m_pkBack->GetFixedWindowLocalPos();
        vOriPos.x += 6.0f;    vOriPos.y += 7.0f;
        D3DXVECTOR2 vPos = vOriPos;
        for(int iLoop=0; iLoop<MAX_ITEMBOX; ++iLoop)
        {
            if( iLoop < int(m_sDSItemInfo.vecItemList.size()) )
            {
                KGCDPointItemBox* pWnd = (KGCDPointItemBox*)g_pkUIMgr->CreateTemplateUI( this, "gc_dpoint_item_box", "ui_DPointItemBox.stg", "gc_dpoint_item_box", true, true );
                if( pWnd )
                {
                    pWnd->InitState( true, true, this );
                    pWnd->SetFixedWindowLocalPos( vPos );
                    pWnd->DisconnectAllEventProcedure();
                    pWnd->ConnectEventProcedure<KGCDotaShop, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCDotaShop::OnClickItemBox, _1, iLoop ) );
                    pWnd->ConnectEventProcedure<KGCDotaShop, void>( KD3DWnd::D3DWE_CURSOR_ENTER, this, boost::bind( &KGCDotaShop::OnRollOverItemBox, _1, iLoop ) );

                    vPos.x += pWnd->GetWidth() + 1;
                    if( 0 == (++iCount) % 4 )
                    {
                        vPos.x = vOriPos.x;
                        vPos.y += pWnd->GetHeight() + 1;
                    }

                    m_vecItemBox.push_back( pWnd );
                }
                continue;
            }
            break;
        }

        // 아이템 사용시 출력되는 이미지를 최상위로 올림
        m_pkShowBuyItem->SetOrder_Top();
        m_pkShowBuyItemDesc->SetOrder_Top();
    }
    else
    {
        std::vector<KGCDPointItemBox*>::iterator vecIter = m_vecItemBox.begin();
        for(int iLoop=0; vecIter!=m_vecItemBox.end(); ++vecIter, ++iLoop)
        {
            (*vecIter)->DisconnectAllEventProcedure();
            (*vecIter)->ConnectEventProcedure<KGCDotaShop, void>( KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind( &KGCDotaShop::OnClickItemBox, _1, iLoop ) );
            (*vecIter)->ConnectEventProcedure<KGCDotaShop, void>( KD3DWnd::D3DWE_CURSOR_ENTER, this, boost::bind( &KGCDotaShop::OnRollOverItemBox, _1, iLoop ) );
        }
    }
}

void KGCDotaShop::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkScroll )
    {
        int iTemp = m_pkScroll->GetScrollPos();
        if( iTemp != m_iScrollIdx )
        {
            g_KDSound.Play( "73" );
            m_iScrollIdx = iTemp;
            RefreshItemBox();
        }
    }
}

void KGCDotaShop::FrameMoveInEnabledState( void )
{
    // 스크롤 휠처리
    if ( g_pkInput->GetZMov() )
    {
        int iTemp = m_iScrollIdx;
        if( g_pkInput->GetZMov() > 0 )
            m_pkScroll->SetScrollPos( (m_pkScroll->GetScrollPos() - 1) );
        else if( g_pkInput->GetZMov() < 0 )
            m_pkScroll->SetScrollPos( (m_pkScroll->GetScrollPos() + 1) );

        iTemp = m_pkScroll->GetScrollPos();
        if( iTemp != m_iScrollIdx )
        {
            g_KDSound.Play( "73" );
            m_iScrollIdx = iTemp;
            RefreshItemBox();
            return;
        }
    }

    // 아이템 사용시 事譴訣?출력
    if( 0 < m_iAlpha )
    {
        if( 0 == m_iViewCoolTime )  m_iAlpha -= 10;
        else                        --m_iViewCoolTime;
        if( m_iAlpha < 0 ) m_iAlpha = 0;
        m_pkShowBuyItem->SetColor( D3DCOLOR_ARGB( m_iAlpha, 255, 255, 255 ) );
    }
    else
    {
        m_iAlpha = 0;
        m_iViewCoolTime = 0;
        m_pkShowBuyItem->SetItemTexture( NULL );
        m_pkShowBuyItem->ToggleRender( false );
        m_pkShowBuyItemDesc->SetText( L"" );
        m_pkShowBuyItemDesc->ToggleRender( false );
    }

    // Shop이 열린상태에서도 아이템을 사용할 수 있도록 처리
    UseItem();

    // 키로 아이템 선택처리 : 변경된 내용이 있다면 UI 갱신
    if( KeyControll() )
    {
        RefreshSelectItem();
        RefreshItemBox();
    }
}

void KGCDotaShop::RefreshMyPoint()
{
    // 보유한 DPoint String처리
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex(iPlayerIdx) )
    {
        WCHAR wszTemp[128];
        wsprintf( wszTemp, L"%d", g_MyD3D->MyPlayer[ iPlayerIdx ]->Get_D_Point() );
        m_pkStaticMyPoint->SetText( wszTemp );
    }
}

void KGCDotaShop::RefreshSelectItem()
{
    // 선택된 아이템 String 처리
    if( IsSelectItem() )
    {
        // 아이템ID
        int iGameItemIdx = m_sDSItemInfo.vecItemList[m_iSelectItem].first;

        // 아이템가격
        int iGameItemCost = m_sDSItemInfo.vecItemList[m_iSelectItem].second;
        WCHAR wszTemp[128];
        wsprintf( wszTemp, L"%d", iGameItemCost );
        m_pkStaticItemPoint->SetText( wszTemp );

        // 아이템 이름
        int iItemName = m_sDSItemInfo.mapItemDesc[iGameItemIdx].first;
        m_pkStaticItemName->SetText( g_pkStrLoader->GetString( iItemName ) );

        // 아이템 설명
        int iItemDesc = m_sDSItemInfo.mapItemDesc[iGameItemIdx].second;
        m_pkStaticItem->SetTextAutoMultiline( g_pkStrLoader->GetString( iItemDesc ) );
    }

    // 선택된 아이템 박스를 상세아이템박스에게도 알려주자!
    if( m_iSelectItem < int(m_vecpItemImg.size()) )
        m_pkImgSelectItem->SetItemTexture( m_vecpItemImg[m_iSelectItem] );
    else
        m_pkImgSelectItem->SetItemTexture( NULL );
}

void KGCDotaShop::RefreshItemBox()
{
    int iStartItemIdx = (m_iScrollIdx * 4);

    // 아이템 박스 텍스쳐 셋팅 및 선택된 아이템 박스 Toggle처리
    std::vector<KGCDPointItemBox*>::iterator vecIter = m_vecItemBox.begin();
    for(int iLoop=iStartItemIdx; vecIter != m_vecItemBox.end(); ++vecIter, ++iLoop)
    {
        // 채워넣을 아이템이 존재함
        if( iLoop < int(m_sDSItemInfo.vecItemList.size()) )
        {
            int iGameItemID = m_sDSItemInfo.vecItemList[iLoop].first;
            (*vecIter)->SetItemID( iGameItemID );
            (*vecIter)->SetItemTexture( m_vecpItemImg[iLoop] );

            WCHAR wszTemp[128];
            wsprintf( wszTemp, L"%d", m_sDSItemInfo.vecItemList[iLoop].second );            
            (*vecIter)->SetPriceText( wszTemp );
        }
        // 빈박스로 두자 채워넣을 아이템이 없い빱?        else
        {
            (*vecIter)->SetItemID( -1 );
            (*vecIter)->SetItemTexture( NULL );
            (*vecIter)->SetPriceText( L"" );
        }

        // 박스가 선택된 박스인지 아닌지 Toggle처리
        (*vecIter)->SetSelectBox( (iLoop == m_iSelectItem) );
    }
}

void KGCDotaShop::UseItem() 
{
    int iMyPlayer = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iMyPlayer ) )
    {
        if( g_MyD3D->MyCtrl->k_Use_Item )
        {
            if( false == g_MyD3D->MyPlayer[iMyPlayer]->IsDie() && g_MyD3D->MyItms->GetSelectedItem() > -1)
            {
                g_MyD3D->MyPlayer[iMyPlayer]->Sound( 10 );
                g_MyD3D->MyItms->Use_Item(iMyPlayer, g_MyD3D->MyItms->GetSelectedItem());
                g_MyD3D->MyItms->Delete_Item();
            }
        }

        if( g_MyD3D->MyCtrl->k_Select_Item )
        {
            g_KDSound.Play( "73" );
            g_MyD3D->MyItms->MoveNextItemCursorPos( g_MyD3D->MyPlayer[iMyPlayer]->GetNumGameItem() );
        }
    }
}

bool KGCDotaShop::KeyControll() 
{
    int iChange;
    bool bRefresh = false;

    //g_MyD3D->MyCtrl->k_Left : 현재-1, 4로 모드연산결과가 0이면 처리안함
    if( g_MyD3D->MyCtrl->k_Left )
    {
        iChange = m_iSelectItem - 1;
        if( (m_iSelectItem % 4 != 0) && (0 <= iChange) )
        {
            m_iSelectItem = iChange;
            bRefresh = true;
        }
        g_MyD3D->MyCtrl->k_Left = false;
    }
    //g_MyD3D->MyCtrl->k_Right : 현재+1 4로 모드연산결과가 1이면 처리안함
    else if( g_MyD3D->MyCtrl->k_Right )
    {
        iChange = m_iSelectItem + 1;
        if( (m_iSelectItem % 4 != 3) && iChange < int(m_sDSItemInfo.vecItemList.size()) )
        {
            m_iSelectItem = iChange;
            bRefresh = true;
        }
        g_MyD3D->MyCtrl->k_Right = false;
    }
    //g_MyD3D->MyCtrl->k_Up(스크롤처리필요) : 현재-4, 0보다커야 함, 위로 올라가야하면 스크롤 --
    else if( g_MyD3D->MyCtrl->k_Up )
    {
        iChange = m_iSelectItem - 4;
        if( iChange >= 0 )
        {
            // 스크롤바가 움직여 줘야하는지 확인
            if( (m_iScrollIdx * 4) > iChange )
            {
                m_pkScroll->SetScrollPos( (m_pkScroll->GetScrollPos() - 1) );
                int iTemp = m_pkScroll->GetScrollPos();
                if( iTemp != m_iScrollIdx )
                {
                    g_KDSound.Play( "73" );
                    m_iScrollIdx = iTemp;
                }
            }

            m_iSelectItem = iChange;
            bRefresh = true;
        }
        g_MyD3D->MyCtrl->k_Up = false;
    }
    //g_MyD3D->MyCtrl->k_Down(스크롤처리필요) : 현재+4, size보다 작아야함, 아래로 내려가야하면 스크롤 ++
    else if( g_MyD3D->MyCtrl->k_Down )
    {
        iChange = m_iSelectItem + 4;
        if( iChange < int(m_sDSItemInfo.vecItemList.size()) )
        {
            // 스크롤바가 움직여 줘야하는지 확인
            if( ((m_iScrollIdx * 4) + MAX_ITEMBOX) <= iChange )
            {
                m_pkScroll->SetScrollPos( (m_pkScroll->GetScrollPos() + 1) );
                int iTemp = m_pkScroll->GetScrollPos();
                if( iTemp != m_iScrollIdx )
                {
                    g_KDSound.Play( "73" );
                    m_iScrollIdx = iTemp;
                }
            }

            m_iSelectItem = iChange;
            bRefresh = true;
        }
        g_MyD3D->MyCtrl->k_Down = false;
    }

    return bRefresh;
}

void KGCDotaShop::OnClickItemBox( int iBoxIdx )
{
    // 박스인덱스넘겨받도록하고 스크롤만큼 더해주므로써 아이템 인덱스를 구한다.
    int iSelect = iBoxIdx + (m_iScrollIdx*4);

    // 선택된인덱스가 범위를 넘어가지 않도록 처리
    if( 0 > iSelect || iSelect >= int(m_sDSItemInfo.vecItemList.size()) ) 
        return;

    // 박스와 스트링을 리플레쉬
    m_iSelectItem = iSelect;
    RefreshItemBox();
    RefreshSelectItem();
}

void KGCDotaShop::OnRollOverItemBox( int iBoxIdx )
{
    if( 0 <= iBoxIdx && iBoxIdx < int(m_vecItemBox.size()) )
    {
        std::vector<KGCDPointItemBox*>::iterator vecIter = m_vecItemBox.begin();
        for(int iLoop=0; vecIter != m_vecItemBox.end(); ++vecIter, ++iLoop)
        {
            (*vecIter)->SetRollOverBox( iBoxIdx == iLoop );
        }
    }
}

void KGCDotaShop::CloseUI()
{
    OnEnterCursor();
    Destroy();
    g_pkUIScene->EndMessageBox();
}

void KGCDotaShop::OnClickClsoe()
{
    OnEnterCursor();
    Destroy();
    g_pkUIScene->EndMessageBox();
    g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_SHOP_CLOSE );
}

void KGCDotaShop::OnClickBuy()
{
    // 중복 윈도우 생성 금지처리
    if( false == g_pkUIScene->IsCurMsgBox_Use( KGCUIScene::GC_MBOX_USE_DOTA_GAME ) )
    {
        int iPlayerIdx = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex(iPlayerIdx) && IsSelectItem() )
        {
            // 중복구매불가
            if( g_MyD3D->MyPlayer[ iPlayerIdx ]->IsGameItem( m_sDSItemInfo.vecItemList[ m_iSelectItem ].first ) )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_DOTA_GAMEITEM_BUY_FAIL1 ), L"", KGCUIScene::GC_MBOX_USE_DOTA_GAME, 0, 0, false, true);
                return;
            }

            // DPoint부족
            if( g_MyD3D->MyPlayer[ iPlayerIdx ]->Get_D_Point() < m_sDSItemInfo.vecItemList[ m_iSelectItem ].second )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_DOTA_GAMEITEM_BUY_FAIL2 ), L"", KGCUIScene::GC_MBOX_USE_DOTA_GAME, 0, 0, false, true);
                return;
            }

            // 슬롯용량확인( Take_Item 에서 처리 )
            //if( 3 <= g_MyD3D->MyPlayer[ iPlayerIdx ]->GetNumGameItem() )
            //{
            //    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_DOTA_GAMEITEM_BUY_FAIL3 ), L"", KGCUIScene::GC_MBOX_USE_DOTA_GAME, 0, 0, false, true);
            //    return;
            //}

            // Send보내기 방장은 받아서 DPoint감소시키고 그 정보 그대로 ACK로 보내주자
            // ACK 패킷받았을때 UID가 같으면 구매처리를 하자
            KGC_PID_BUY_DOTA_GAMEITEM kPacket( GC_PID_BUY_DOTA_GAMEITEM_REQ, g_MyD3D->MyPlayer[ iPlayerIdx ]->m_kUserInfo.dwUID, m_sDSItemInfo.vecItemList[ m_iSelectItem ].first, -1 );
            SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
            KGCPC_MEMO( "GC_PID_BUY_DOTA_GAMEITEM_REQ" );

            // 임시 아이템 구입
            //g_MyD3D->MyPlayer[ iPlayerIdx ]->Set_D_Point( (g_MyD3D->MyPlayer[ iPlayerIdx ]->Get_D_Point() - m_sDSItemInfo.vecItemList[ m_iSelectItem ].second) );
            //g_MyD3D->MyPlayer[ iPlayerIdx ]->Take_Item( m_sDSItemInfo.vecItemList[ m_iSelectItem ].first );
        }
    }
}

void KGCDotaShop::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}

void KGCDotaShop::SetBuyItem( int iGameItemIdx, GCDeviceTexture* pTexture, int iAlpha, int iViewCoolTime )
{
    if( 0 > iGameItemIdx || iGameItemIdx > GC_GAME_ITEM_NUM )
        return;
    
    m_iAlpha = iAlpha;
    m_iViewCoolTime = iViewCoolTime;
    m_pkShowBuyItem->SetItemTexture( pTexture );
    m_pkShowBuyItem->SetColor( D3DCOLOR_ARGB( m_iAlpha, 255, 255, 255 ) );
    m_pkShowBuyItem->ToggleRender( true );

    m_pkShowBuyItemDesc->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );
    m_pkShowBuyItemDesc->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
    m_pkShowBuyItemDesc->SetText( g_pkStrLoader->GetReplacedString(STR_USE_DOTA_GAMEITEM, "I", m_sDSItemInfo.mapItemDesc[iGameItemIdx].first) );
    m_pkShowBuyItemDesc->ToggleRender( true );
}

int KGCDotaShop::GetItemName( int iGameItemIdx )
{
    if( 0 > iGameItemIdx || iGameItemIdx > GC_GAME_ITEM_NUM )
        return 0;
    return m_sDSItemInfo.mapItemDesc[iGameItemIdx].first;
}