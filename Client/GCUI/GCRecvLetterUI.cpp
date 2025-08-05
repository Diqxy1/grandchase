#include "stdafx.h"
#include "GCRecvLetterUI.h"

IMPLEMENT_CLASSNAME( KGCRecvLetterUI );
IMPLEMENT_WND_FACTORY( KGCRecvLetterUI );
IMPLEMENT_WND_FACTORY_NAME( KGCRecvLetterUI, "gc_recvletterui" );

#define TEXT_OUTLINE_COLOR D3DCOLOR_ARGB( 255, 12, 29, 47 )

KGCRecvLetterUI::KGCRecvLetterUI( void )
{
    m_pkStaticDate = NULL;
    m_pkStaticFrom = NULL;
    m_pkStaticTitle = NULL;
    m_pkStaticGoods = NULL;
    m_pkStaticGP = NULL;
    LINK_CONTROL( "staticDate",     m_pkStaticDate );
    LINK_CONTROL( "staticFrom",     m_pkStaticFrom);
    LINK_CONTROL( "staticTitle",    m_pkStaticTitle );
    LINK_CONTROL( "staticGoods",    m_pkStaticGoods );
    LINK_CONTROL( "staticGP",       m_pkStaticGP );

    m_pkBtnClose = NULL;
    m_pkBtnReceive = NULL;
    m_pkBtnSend = NULL;
    m_pkBtnDelete = NULL;
    LINK_CONTROL( "btnClose",   m_pkBtnClose );
    LINK_CONTROL( "btnRecv",    m_pkBtnReceive );
    LINK_CONTROL( "btnSend",    m_pkBtnSend );
    LINK_CONTROL( "btnDelete",  m_pkBtnDelete );

    m_pkListLetter = NULL;
    LINK_CONTROL( "listLetter",  m_pkListLetter );

    for( int i=0 ; i<NUM_GOODS_SLOT ; ++i ) { 
        m_apkGoods[i] = NULL;

        char szTemp[128] = { 0, };
        sprintf_s( szTemp, 127, "itemImg%d", i );
        LINK_CONTROL( szTemp, m_apkGoods[i] );
    }

    m_pkScroll = NULL;
    LINK_CONTROL( "scroll",      m_pkScroll );
    m_iScrollTopIndex = 0;

    m_bMouseBound = false;
    m_iBoundIndex = -1;
}

KGCRecvLetterUI::~KGCRecvLetterUI( void )
{
}

void KGCRecvLetterUI::OnCreate( void )
{
    m_pkStaticDate->InitState( true );  
    m_pkStaticFrom->InitState( true );
    m_pkStaticTitle->InitState( true );
    m_pkStaticGoods->InitState( true );
    m_pkStaticGP->InitState( true );
    m_pkStaticGP->SetAlign( DT_RIGHT );

    m_pkListLetter->InitState( true, false, this );
    m_pkListLetter->Clear();
    m_pkListLetter->SetLineSpace( 1.0f );

    m_pkStaticDate->SetFontOutline( true, TEXT_OUTLINE_COLOR );
    m_pkStaticFrom->SetFontOutline( true, TEXT_OUTLINE_COLOR );
    m_pkStaticGoods->SetFontOutline( true, TEXT_OUTLINE_COLOR );

    m_pkScroll->InitState( true, true, this );
    
    m_pkBtnSend->InitState( false, true, this );
    
    m_pkBtnDelete->InitState( true, true, this );
    m_pkBtnDelete->DisconnectAllEventProcedure();
    m_pkBtnDelete->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRecvLetterUI::OnClickDelete );

    m_pkBtnReceive->InitState( true, true, this );
    m_pkBtnReceive->DisconnectAllEventProcedure();
    m_pkBtnReceive->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRecvLetterUI::OnClickGoodsRecv );

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRecvLetterUI::OnClickClose );

    for( int i=0 ; i<NUM_GOODS_SLOT ; ++i ) { 
        m_apkGoods[ i ]->InitState( true, true, this );
    }
}

void KGCRecvLetterUI::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkScroll || event.m_pWnd == m_pkListLetter ) { 
        OnScroll();
    }

    for( int i=0 ; i<NUM_GOODS_SLOT ; ++i ) { 
        if( event.m_pWnd == m_apkGoods[ i ] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ) { 
            SetMouseBound( true, i );
            break;
        }

        if( event.m_pWnd == m_apkGoods[ i ] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE ) { 
            SetMouseBound( false );
        }
    }
}

void KGCRecvLetterUI::FrameMoveInEnabledState( void )
{
    // 스크롤 처리 
    if( 0 != g_pkInput->GetZMov() ) { 
        if( g_pkInput->GetZMov() > 0 ) { 
            if( --m_iScrollTopIndex < 0 ) {
                ++m_iScrollTopIndex;
            }
            else {
                g_KDSound.Play( "73" );
            }
            m_pkScroll->SetScrollPos( m_iScrollTopIndex );
        }
        else { 
            if( ++m_iScrollTopIndex > m_pkScroll->GetScrollRangeMax() ) {
                --m_iScrollTopIndex;
            }
            else { 
                g_KDSound.Play( "73" );
            }
            m_pkScroll->SetScrollPos( m_iScrollTopIndex );
        }
        OnScroll();
    }
}

void KGCRecvLetterUI::PostChildDraw( void )
{
    // 첨부 아이템 정보 
    ShowItemInfo();
}

void KGCRecvLetterUI::OnClickClose( void )
{
    Clear();
    this->ToggleRender( false );
}

void KGCRecvLetterUI::OnClickGoodsRecv( void )
{
    if( m_LetterInfo.m_PostUID == 0 ) { 
        return;
    }

    if( m_LetterInfo.m_bGetItemGP ) { 
        return;
    }

    int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;
    Result_PostLetter = INT_MAX;

    // 인벤토리가 제한 수량을 초과하면 구매를 막는다.
    if ( g_MyD3D->m_kItemMgr.GetTotalInventorySize() > iInventoryCapacity )
    {
        //this->Destroy();
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL1 ) +
            g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_FULL2, "i", iInventoryCapacity ),
            g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL3 ), 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true
            );
    }
    else // 인벤토리 여유가 있다면
    {
        SiKGCPostLetterManager()->RecieveItemGP( m_LetterInfo.m_PostUID );
        g_MyD3D->WaitForServerAck( Result_PostLetter, INT_MAX, 1000, TIME_OUT_VALUE );
    }
    

    if( 0 == Result_PostLetter ) { 
        m_LetterInfo.m_bGetItemGP = true;
        m_LetterInfo.m_vecItem.clear();
        m_LetterInfo.m_nGamePoint = 0;
        for( int i=0 ; i<NUM_GOODS_SLOT ; ++i ) { 
            m_apkGoods[i]->SetItemInfo( KItem::INVALID_ITEM );
        }
        m_pkStaticGP->SetNumber( 0 );

		// 우편 UI 갱신
		g_pkUIScene->m_pkPostLetterDlg->SetItemRecvInfo( m_LetterInfo.m_PostUID );
     
    }
}

void KGCRecvLetterUI::OnClickSend( void )
{

}

void KGCRecvLetterUI::SetLetterInfo( POSTUID postUID_ )
{
    if( false == SiKGCPostLetterManager()->GetReadPostLetterInfo( postUID_, m_LetterInfo ) ) {
        Clear();
        OnClickClose();
    }

    ClearUI();

    std::wostringstream stmDate;
    struct tm* timeInfo = localtime( &m_LetterInfo.m_tmRegDate );
    if ( timeInfo ) 
    {
        stmDate << g_pkStrLoader->GetString( STR_ID_READ_LETTER_DATE );
        stmDate << L" " << timeInfo->tm_year + 1900 << L"." << timeInfo->tm_mon + 1 << L"." << timeInfo->tm_mday << L" ";   // 날짜 
    }

	std::wstring strFromNickName = L""; // 보낸사람 정보가 없으면, 그랜드체이스
	if( m_LetterInfo.m_strFromNickName.empty() )
		strFromNickName = g_pkStrLoader->GetString( STR_ID_SYSTEM_LETTER_FROM );//그랜드체이스
	else
		strFromNickName = m_LetterInfo.m_strFromNickName;

    std::wostringstream stmTitle;
    stmTitle << g_pkStrLoader->GetString( STR_ID_READ_LETTER_FROM ) << L" " << strFromNickName;

    m_pkStaticDate->SetText( stmDate.str() );
    m_pkStaticFrom->SetForceWordWrapText( stmTitle.str(), true );
    m_pkStaticTitle->SetForceWordWrapText( m_LetterInfo.m_strTitle, true );
    m_pkStaticGoods->SetText( g_pkStrLoader->GetString( STR_ID_READ_LETTER_GOODS ) );
    m_pkStaticGP->SetNumber( m_LetterInfo.m_nGamePoint );

    ConvertString( m_LetterInfo.m_strMessage );
    m_pkListLetter->AddString( m_LetterInfo.m_strMessage );
    m_pkListLetter->SetTopIndex( 0 );
    ResetScroll();

    for( int i=0 ; i<static_cast<int>( m_LetterInfo.m_vecItem.size() ) ; ++i ) { 
        if( i >= NUM_GOODS_SLOT ) { 
            break;
        }
        m_apkGoods[i]->SetItemInfo( m_LetterInfo.m_vecItem[i].m_ItemID / 10 );
        m_apkGoods[i]->ShowItem( true );
    }
}

void KGCRecvLetterUI::Clear( void )
{
    ClearUI();
    ClearInfo();
}

void KGCRecvLetterUI::OnScroll( void )
{
    int iTop = m_pkScroll->GetScrollPos();
    if( m_pkListLetter->GetTopIndex() != iTop ) { 
        g_KDSound.Play( "73" );
        m_pkListLetter->SetTopIndex( iTop );
    }
}

void KGCRecvLetterUI::ResetScroll( void )
{
    m_pkScroll->SetScrollPageSize( m_pkListLetter->GetDisplayLimitLine() );
    m_pkScroll->SetScrollRangeMax( m_pkListLetter->GetCount() );
    m_iScrollTopIndex = m_pkListLetter->GetTopIndex();
    m_pkScroll->SetScrollPos( m_iScrollTopIndex );
}

void KGCRecvLetterUI::SetMouseBound( bool bSet_, int iIndex_ /*= -1 */ )
{
    m_bMouseBound = bSet_;
    m_iBoundIndex = iIndex_;
}

void KGCRecvLetterUI::ShowItemInfo( void )
{
    if( m_bMouseBound && m_iBoundIndex != -1 ) {
        GCItem* pItem = g_pItemMgr->GetItemData( m_apkGoods[ m_iBoundIndex ]->GetItemID() );

        if( NULL == pItem ) { 
            return;
        }

        if( pItem->dwGoodsID == KItem::INVALID_ITEM ) { 
            return;
        }

        D3DXVECTOR2 dxvPos = m_apkGoods[ m_iBoundIndex ]->GetFixedWindowLocalPos();
        dxvPos.x += ( 135.0f * g_pGCDeviceManager->GetWindowScaleX() );
        dxvPos.y += ( 50.0f * g_pGCDeviceManager->GetWindowScaleY() );

        KItem kItem = m_LetterInfo.m_vecItem[m_iBoundIndex];
        kItem.m_ItemUID = KGCItemManager::MAXIMUM_PCBANG_UID+1;
        kItem.m_cGradeID = pItem->cItemGrade;
        kItem.m_vecSocket = pItem->m_vecSocketInfo;
        kItem.m_vecAttribute = pItem->m_vecAttribute;
        
        if ( kItem.m_nPeriod != KItem::UNLIMITED_ITEM )
        {
            CTime cCurTime = CTime::GetCurrentTime();
            CTimeSpan cSpanTime( kItem.m_nPeriod, 0, 0, 0 );
            cCurTime += cSpanTime;
            kItem.m_tEndDate = static_cast<time_t>(cCurTime.GetTime());
        }

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, &kItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    }
}

void KGCRecvLetterUI::OnClickDelete( void )
{
    if( m_LetterInfo.m_PostUID == 0 ) { 
        return;
    }

    if( m_LetterInfo.m_vecItem.empty() ) {  // 아이템 없읍
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_DELETE_LETTER_NO_ITEM ), L"", 
            KGCUIScene::GC_MBOX_USE_DELETE_LETTER, 1, 0, true, true );
    }
    else {                                   // 아이템 있음 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_DELETE_LETTER_WITH_ITEM ), L"", 
            KGCUIScene::GC_MBOX_USE_DELETE_LETTER, 1, 0, true, true );
    }
}

void KGCRecvLetterUI::OnDelete( void )
{
    std::vector< POSTUID > vecDeleteList;
    vecDeleteList.push_back( m_LetterInfo.m_PostUID );

    Result_PostLetter = INT_MAX;
    KP2P::GetInstance()->Send_DeleteLetterReq( vecDeleteList );
    g_MyD3D->WaitForServerAck( Result_PostLetter, INT_MAX, 1000, TIME_OUT_VALUE );

    if( 0 == Result_PostLetter ) { 
        OnClickClose();
        g_pkUIScene->m_pkPostLetterDlg->RefreshPageInfo();
    }
}

void KGCRecvLetterUI::ClearUI( void )
{
    m_pkStaticDate->SetText( L"" );
    m_pkStaticFrom->SetText( L"" );
    m_pkStaticTitle->SetText( L"" );
    m_pkStaticGoods->SetText( L"" );
    m_pkStaticGP->SetText( L"" );

    m_pkListLetter->Clear();

    for( int i=0 ; i<NUM_GOODS_SLOT ; ++i ) { 
        m_apkGoods[i]->SetItemInfo( KItem::INVALID_ITEM );
    }

    m_bMouseBound = false;
    m_iBoundIndex = -1;
}

void KGCRecvLetterUI::ClearInfo( void )
{
    m_LetterInfo = KPostItemInfo();
}

void KGCRecvLetterUI::ConvertString( std::wstring& strMsg_ )
{
    g_pkStrLoader->Replacer( strMsg_,  L"\\n", L"\r\n" );
}