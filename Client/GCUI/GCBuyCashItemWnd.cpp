#include "stdafx.h"
#include ".\gcbuycashitemwnd.h"
//
//
//
//
//
//
//
//
//
#include "../MyD3D.h"

//

#include "../Procedure.h"


#include "../GCUI/GCItemImgWnd.h"
#include "../GCUI/GCSetItemImgWnd.h"


//
//

IMPLEMENT_CLASSNAME( KGCBuyCashItemWnd );
IMPLEMENT_WND_FACTORY( KGCBuyCashItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCBuyCashItemWnd, "gc_buy_cashitem_window" );

KGCBuyCashItemWnd::KGCBuyCashItemWnd(void) : m_kItem(NULL)
{
    LINK_CONTROL( "nickname_section_bar",   m_pNickNameSectionBar );

    LINK_CONTROL( "buy_wnd",                m_pBuyWnd );
    LINK_CONTROL( "present_wnd",            m_pPresentWnd );

    // 선물, 구매 버튼.
    LINK_CONTROL( "buy_button",             m_pBuyBtn );
    LINK_CONTROL( "present_button",         m_pPresentBtn );
    LINK_CONTROL( "cancel_button",          m_pCancelBtn );
    LINK_CONTROL( "slash",                  m_pSlash );

    LINK_CONTROL( "price_static",           m_pStaticPrice );

    LINK_CONTROL( "static_remain_cash",     m_pStaticRemainCash );
    LINK_CONTROL( "value_remain_cash",      m_pValueRemainCash );

    // 받을사람
    LINK_CONTROL( "static_take_user",       m_pStaticTakeUser );
    LINK_CONTROL( "edit_take_user",         m_pEditTakeUser );
    LINK_CONTROL( "back_take_user",         m_pBackTakeUser );

    // 메세지
    LINK_CONTROL( "static_comment",         m_pStaticComment );
    LINK_CONTROL( "edit_comment",           m_pEditComment );
    LINK_CONTROL( "back_comment",           m_pBackComment );

    LINK_CONTROL( "price_combobox",         m_pComboBox );
    LINK_CONTROL( "background",             m_pSizingBox );
    LINK_CONTROL( "name_static",            m_pStaticItemName );
    LINK_CONTROL( "item_img_wnd",           m_pItemImgWnd );
    LINK_CONTROL( "setitem_img_wnd",        m_pSetItemImgWnd );
    LINK_CONTROL( "static_atk",             m_pStaticAtk );
    LINK_CONTROL( "static_def",             m_pStaticDef );
    LINK_CONTROL( "static_vit",             m_pStaticVit );
    LINK_CONTROL( "static_lv",              m_pStaticLv );
    LINK_CONTROL( "static_item_desc",       m_pStaticDesc );


    LINK_CONTROL( "cash_img",               m_pkCash_img );

    LINK_CONTROL( "exit_button",            m_pExitBtn );

    LINK_CONTROL( "checkbox_use_virtualcash",       m_pkUseVCashCheckBox );
    LINK_CONTROL( "static_virtual_cash",            m_pkStaticVCash );
}

KGCBuyCashItemWnd::~KGCBuyCashItemWnd(void)
{

}



void KGCBuyCashItemWnd::OnCreate( void )
{
    //일단 생성이 되는 순간은 구입 하는 Window로 세팅 한다.
#ifdef NATION_KOREA
    m_pSizingBox->SetSize( 290, 170 );
#else
    m_pSizingBox->SetSize( 371, 217 );
#endif
	
    m_pBuyBtn->InitState( true, true, this );
    m_pPresentBtn->InitState( false, true, this );
    m_pCancelBtn->InitState( true, true, this );

    m_pNickNameSectionBar->ToggleRender( false );
    m_pEditTakeUser->InitState( false, false, this );
    m_pBackTakeUser->InitState( false, false, this );
    m_pSlash->InitState( true, false, this );
    m_pComboBox->InitState( true, true, this );
    m_pExitBtn->InitState( true, true, this );
    m_pkCash_img->InitState( true, true, this );
#if defined( NATION_BRAZIL )
    m_pkCash_img->SetWindowPos( D3DXVECTOR2(210.0f, 189.0f) );
    m_pBuyWnd->SetWidth( (DWORD)46 );
    m_pBuyWnd->SetWndTexCoords( D3DXVECTOR4( 126.0f + 0.5f, 328.0f + 0.5f, 172.0f + 0.5f, 344.0f + 0.5f ) / 512.0f );
#endif

    m_pStaticPrice->SetText( std::wstring( g_pkStrLoader->GetString(STR_ID_ITEM_PRICE) ) );
    m_pStaticPrice->SetAlign( DT_RIGHT );
    m_pStaticRemainCash->SetText( std::wstring( g_pkStrLoader->GetString(STR_ID_REMAIN_CASH_POINT) ) );
    m_pStaticRemainCash->SetAlign( DT_RIGHT );
    m_pStaticTakeUser->SetText( std::wstring( g_pkStrLoader->GetString(STR_ID_RECEIVE_USER_NICK) ) );
    m_pStaticTakeUser->SetAlign( DT_RIGHT );
    m_pStaticComment->SetText( std::wstring( g_pkStrLoader->GetString(STR_ID_PRESENT_COMMENT) ) );
    m_pStaticComment->SetAlign( DT_RIGHT );

    m_pStaticItemName->SetAlign( DT_LEFT );
    m_pValueRemainCash->SetAlign( DT_CENTER );
    m_pComboBox->SetAlign( DT_CENTER );
    m_pStaticDesc->SetLineSpace( 1.2f );
    //m_pStaticAtk->SetAlign( DT_CENTER );
    //m_pStaticDef->SetAlign( DT_CENTER );
    //m_pStaticVit->SetAlign( DT_CENTER );
    m_vBuyBtnInitPos = m_pBuyBtn->GetFixedWindowLocalPos();

    m_pExitBtn->SetHotKey( DIK_ESCAPE );

    m_pkUseVCashCheckBox->InitState( true, true, this );
    m_pkStaticVCash->InitState( true, true, this );

    CheckCashType( (CASH_TYPE)(CT_GC_POINT == g_MyD3D->IsUseVirtualCash()) );
}

void KGCBuyCashItemWnd::FrameMoveInEnabledState( void )
{
}
void KGCBuyCashItemWnd::ClearEditControl()
{
    m_pEditComment->SetText( L"" );
    m_pEditTakeUser->SetText( L"" );
}
void KGCBuyCashItemWnd::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pCancelBtn ) 
    {
        switch( event.m_dwCode )
        {
        case D3DWE_BUTTON_CLICK:
            {
                g_KDSound.Play( "30" );
				SiGCSKT()->SetBuyingReqGroup(-1);
                SpeakToActionListener( KActionEvent( this, ID_CLOSE ) );
                ClearEditControl();
                break;
            }
        }
    }
    else if ( event.m_pWnd == m_pExitBtn ) 
    {
        switch( event.m_dwCode )
        {
        case D3DWE_BUTTON_CLICK:
            {
                g_KDSound.Play( "30" );
				SiGCSKT()->SetBuyingReqGroup(-1);
                SpeakToActionListener( KActionEvent( this, ID_CLOSE ) );
                ClearEditControl();
                break;
            }
        }
    }
    else if ( event.m_pWnd == m_pBuyBtn || event.m_pWnd == m_pPresentBtn ) 
    {
        switch( event.m_dwCode )
        {
        case D3DWE_BUTTON_CLICK:
            {
                std::vector<GCItem*>& vecItem = g_pItemMgr->GetCashShopItem( m_kItem->dwGoodsID );

                if (vecItem.empty())
                    return;

                int iSelectItem = m_pComboBox->GetCurSel();
                if ( iSelectItem == -1 )
                    iSelectItem = 0;

                /////////////////////////////////////////////////////////////////////
                // Date :  06-12-21 
                // milennium9@kogstudios.com 권상구
                //*******************************************************************
                // 보이는 순서를 뒤집었기 때문에 선택한 순서도 뒤집어 주어야 합니다.
                /////////////////////////////////////////////////////////////////////
                iSelectItem = (vecItem.size() - 1) - iSelectItem;
                /////////////////////////////////////////////////////////////////////


                GCItem* kItem = vecItem[iSelectItem];
                BREAK_NIL(kItem);
                bool bPresent = event.m_pWnd == m_pPresentBtn;

                g_MyD3D->SetUseVirtualCash( m_pkUseVCashCheckBox->GetCheck() );

                if( g_MyD3D->IsUseVirtualCash() )
				{
					if( kItem->dwPrice <= g_kGlobalValue.m_dwVirtualCash )
					{
						SpeakToActionListener( KActionEvent( this, ID_CLOSE ) );
						g_pItemMgr->SendBuyForVirtualCashItem( kItem );
					}
					else
					{
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOT_ENOUGH_CASH_POINT) 
								, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
					}
				}
                else
				{
					SpeakToActionListener( KActionEvent( this, ID_CLOSE ) );
					g_pItemMgr->SendBuyForCashItem( kItem, 0, 0, L"", L"" );
				}

                ClearEditControl();
            }
        }
    }
    #if defined( VIRTUAL_CASH ) && defined( GAME_CASH )
    else if( m_pkUseVCashCheckBox == event.m_pWnd
            || m_pkStaticVCash == event.m_pWnd )
    {
        if( event.m_dwCode == D3DWE_BUTTON_CLICK )
        {
            CheckCashType( (CASH_TYPE)(CT_GC_POINT == (!m_pkUseVCashCheckBox->GetCheck())) );
        }
    }
    #endif
}
void KGCBuyCashItemWnd::EnableBuyCashItemWnd( GCItem*& pkItem_, bool bPresent_ )
{
    CheckCashType( (CASH_TYPE)(CT_GC_POINT == g_MyD3D->IsUseVirtualCash()) );

    int m_iHeightInterval = 0;
    #if !defined( VIRTUAL_CASH )
        m_pkUseVCashCheckBox->ToggleRender( false );
        m_pkStaticVCash->ToggleRender( false );
        m_iHeightInterval = 15;
    #endif

    if ( bPresent_ == true )
    {
#ifdef NATION_KOREA
        m_pSizingBox->SetSize( 430, 390 - m_iHeightInterval );
#else
        m_pSizingBox->SetSize( 550, 499 - m_iHeightInterval );
#endif
        D3DXVECTOR2 wndpos = m_pPresentBtn->GetFixedWindowLocalPos();
        m_pPresentBtn->SetFixedWindowLocalPos( D3DXVECTOR2( wndpos.x, m_vBuyBtnInitPos.y - 5 - m_iHeightInterval ) );
        wndpos = m_pCancelBtn->GetFixedWindowLocalPos();
        m_pCancelBtn->SetFixedWindowLocalPos( D3DXVECTOR2( wndpos.x, m_vBuyBtnInitPos.y - 5 - m_iHeightInterval ) );
        wndpos = m_pSlash->GetFixedWindowLocalPos();
        m_pSlash->SetFixedWindowLocalPos( D3DXVECTOR2( wndpos.x, m_vBuyBtnInitPos.y - 5 - m_iHeightInterval ) );

        m_pkUseVCashCheckBox->SetFixedWindowLocalPos( D3DXVECTOR2( 20, m_vBuyBtnInitPos.y - 25 ) );
        m_pkStaticVCash->SetFixedWindowLocalPos( D3DXVECTOR2( 40, m_vBuyBtnInitPos.y - 23 ) );
    }
    else
    {
#ifdef NATION_KOREA
        m_pSizingBox->SetSize( 430, 310 );
#else
        m_pSizingBox->SetSize( 550, 397 );
#endif

        D3DXVECTOR2 wndpos = m_pCancelBtn->GetFixedWindowLocalPos();
        m_pCancelBtn->SetFixedWindowLocalPos( D3DXVECTOR2( wndpos.x, m_vBuyBtnInitPos.y - 65 - m_iHeightInterval ) );
        wndpos = m_pBuyBtn->GetFixedWindowLocalPos();
        m_pBuyBtn->SetFixedWindowLocalPos( D3DXVECTOR2( wndpos.x, m_vBuyBtnInitPos.y - 65 - m_iHeightInterval ) );
        wndpos = m_pSlash->GetFixedWindowLocalPos();
        m_pSlash->SetFixedWindowLocalPos( D3DXVECTOR2( wndpos.x, m_vBuyBtnInitPos.y - 65 - m_iHeightInterval ) );

        m_pkUseVCashCheckBox->SetFixedWindowLocalPos( D3DXVECTOR2( 20, m_vBuyBtnInitPos.y - 85 ) );
        m_pkStaticVCash->SetFixedWindowLocalPos( D3DXVECTOR2( 40, m_vBuyBtnInitPos.y - 83 ) );
    }

    if ( pkItem_->dwSlotPosition & ESP_SET_ITEM )
    {
        m_pSetItemImgWnd->ToggleRender( true );
        m_pSetItemImgWnd->SetItemInfo( pkItem_->iSetItemNumber, pkItem_->dwGoodsID);
        m_pSetItemImgWnd->ShowItem( true );
        m_pItemImgWnd->ToggleRender( false );
        m_pItemImgWnd->ShowItem( true );
    }
    else
    {
        m_pItemImgWnd->ToggleRender( true );
        m_pItemImgWnd->SetItemInfo( pkItem_->dwGoodsID );
        m_pItemImgWnd->ShowItem( true );
        m_pSetItemImgWnd->ToggleRender( false );
        m_pSetItemImgWnd->ShowItem( true );
    }

    m_pEditTakeUser->ToggleRender( bPresent_ );
    m_pBackTakeUser->ToggleRender( bPresent_ );
    m_pStaticTakeUser->ToggleRender( bPresent_ );
    m_pPresentBtn->ToggleRender( bPresent_ );
    m_pBuyBtn->ToggleRender( !bPresent_ );
    m_pStaticComment->ToggleRender( bPresent_ );
    m_pEditComment->ToggleRender( bPresent_ );
    m_pBackComment->ToggleRender( bPresent_ );
    m_pNickNameSectionBar->ToggleRender( bPresent_ );
    m_pBuyWnd->ToggleRender( !bPresent_ );
    m_pPresentWnd->ToggleRender( bPresent_ );
    m_pCancelBtn->ToggleRender( true );
    m_pComboBox->DeleteAllItem();

    // 이름
    std::wostringstream strstream;
    strstream << L"- " << pkItem_->strItemName;
    m_pStaticItemName->SetText( strstream.str() );
    strstream.str( L"" );
    GCItemAbility kItemAbility = g_pItemMgr->GetItemAbility( pkItem_ );
    // 능력치 및 설명
    strstream << g_pkStrLoader->GetString( STR_ID_INFO_ATTACK ) << L"  " << kItemAbility.iAtk;
    m_pStaticAtk->SetText( strstream.str() );
    strstream.str( L"" );
    strstream << g_pkStrLoader->GetString( STR_ID_INFO_DEFENCE ) << L"  " << kItemAbility.iDef;
    m_pStaticDef->SetText( strstream.str() ); 
    strstream.str( L"" );
    strstream << g_pkStrLoader->GetString( STR_ID_INFO_HP_INC ) << L"  " << kItemAbility.iVit;
    m_pStaticVit->SetText( strstream.str() ); 
    strstream.str( L"" );
    m_pStaticDesc->SetTextAutoMultiline( pkItem_->strItemDesc );

    // 장착 레벨.
    strstream << g_pkStrLoader->GetString( STR_ID_INFO_NEED_LEVEL ) << L"  " << pkItem_->iNeedLevelForEquippingItem;
    m_pStaticLv->SetText( strstream.str() );
    if ( pkItem_->iNeedLevelForEquippingItem <= g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel )        
        m_pStaticLv->SetFontColor( D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
    else
        m_pStaticLv->SetFontColor( D3DCOLOR_RGBA( 255, 0, 0, 255 ) );
    strstream.str( L"" );

    // 남은 캐쉬량.
    if( m_pkUseVCashCheckBox->GetCheck() )
        m_pValueRemainCash->SetText( (int)g_kGlobalValue.m_dwVirtualCash );
    else
        m_pValueRemainCash->SetText( (int)g_kGlobalValue.m_dwCash );

    std::vector<GCItem*>& vecItem = g_pItemMgr->GetCashShopItem( pkItem_->dwGoodsID );
    for ( int i = (int)vecItem.size() - 1; i >= 0; i-- )    
    {
        if ( vecItem[i]->iItemTypeValue != -1)
        {
            strstream << vecItem[i]->dwPrice << L"(" << vecItem[i]->iItemTypeValue << 
                g_pkStrLoader->GetString( pkItem_->eItemType == GCIT_PERIOD ? STR_ID_PERIOD_ITEM_DATE : STR_ID_COUNT_ITEM_COUNT ) << L")";
        }
        else
        {
            strstream << vecItem[i]->dwPrice << g_pkStrLoader->GetString( STR_ID_NOT_PERIOD_ITEM );
        }

        if ( i == (int)vecItem.size() - 1 )
        {
            m_pComboBox->SetComboStatic( strstream.str(), DT_CENTER );
        }
        m_pComboBox->AddString( strstream.str() , DT_CENTER );
        strstream.str( L"" );
    }
    m_kItem = pkItem_;
}

void KGCBuyCashItemWnd::CheckCashType(CASH_TYPE enCashType)
{
    m_pkUseVCashCheckBox->SetCheck( false );
    m_pStaticRemainCash->SetText( g_pkStrLoader->GetString(STR_ID_REMAIN_CASH_POINT) );
    
    switch( enCashType )
    {
        case CT_CASH:
            m_pkStaticVCash->SetText( g_pkStrLoader->GetString(STR_ID_CASH_CHECKBOX) );
            m_pValueRemainCash->SetText( (int)g_kGlobalValue.m_dwCash );
            break;
        case CT_GC_POINT:
            m_pkUseVCashCheckBox->SetCheck( true );
            m_pValueRemainCash->SetText( (int)g_kGlobalValue.m_dwVirtualCash );
            m_pkStaticVCash->SetText( g_pkStrLoader->GetString(STR_ID_VIRTUAL_CASH_CHECKBOX) );
            m_pStaticRemainCash->SetText( g_pkStrLoader->GetString(STR_ID_REMAIN_VIRTUAL_CASH_POINT) );    
            break;
        default:
            m_pkStaticVCash->SetText( g_pkStrLoader->GetString(STR_ID_CASH_CHECKBOX) );
            m_pValueRemainCash->SetText( (int)g_kGlobalValue.m_dwCash );
            break;
    }
}


IMPLEMENT_CLASSNAME( KGCChangeCash );
IMPLEMENT_WND_FACTORY( KGCChangeCash );
IMPLEMENT_WND_FACTORY_NAME( KGCChangeCash, "gc_change_cash_dlg" );

KGCChangeCash::KGCChangeCash(void)
: m_pkBtnMaxWebCash(NULL)
, m_pkBtnChangeCash(NULL)
, m_pkBtnCancel(NULL)
, m_pEditWebCash(NULL)
, m_pkBtnExit(NULL)
, m_pkCash(NULL)
, m_pkNeedCash(NULL)
{
    LINK_CONTROL("max_cash_button", m_pkBtnMaxWebCash);
    LINK_CONTROL("ok_button", m_pkBtnChangeCash);
    LINK_CONTROL("cancel_button", m_pkBtnCancel);
    LINK_CONTROL("exit_button", m_pkBtnExit);

    LINK_CONTROL("edit_cash", m_pEditWebCash);
    LINK_CONTROL("static_web_cash", m_pkCash);
    LINK_CONTROL("static_need_cash", m_pkNeedCash);


}

KGCChangeCash::~KGCChangeCash(void)
{

}


void KGCChangeCash::OnCreate( void )
{
    m_pkBtnMaxWebCash->InitState(true,true,this);
    m_pkBtnChangeCash->InitState(true,true,this);
    m_pkBtnCancel->InitState(true,true,this);
    m_pkBtnExit->InitState(true,true,this);
    m_pEditWebCash->InitState(true,true,this);

    m_pkCash->InitState(true);
    m_pkCash->SetAlign( DT_RIGHT );

    m_pkNeedCash->InitState(true);
    m_pkNeedCash->SetAlign( DT_RIGHT );

    m_pEditWebCash->SetText(L"");
    m_pEditWebCash->SetEditType( KD3DEdit::EDIT_NUMBER );
    m_pEditWebCash->SetLimitText(13);

}

void KGCChangeCash::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancel );
    GCWND_MSG_MAP( m_pkBtnExit, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancel );
    GCWND_MSG_MAP( m_pkBtnMaxWebCash, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickMaxCash );
    GCWND_MSG_MAP( m_pkBtnChangeCash, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickChangeCash );

    std::wstring strTemp = m_pEditWebCash->GetText();

    int iConvertCash = _wtoi(strTemp.c_str());

    DOUBLE dwWebCash = static_cast<float>(iConvertCash) /CYOU_POINT_PER_CASH_RATE;
    WCHAR szWebCashTemp[128];
    memset(szWebCashTemp, 0, sizeof(WCHAR) * 128);
    swprintf_s( szWebCashTemp, 128, L"%.1lf", dwWebCash);
    std::wstring wstrWebCash = szWebCashTemp;
    m_pkNeedCash->SetText(wstrWebCash);

    if ( IsRenderOn() )
        m_pEditWebCash->SetFocus();
}


void KGCChangeCash::OnClickCancel( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
    m_pEditWebCash->SetText(L"");
    ToggleRender(false);

}

void KGCChangeCash::OnClickMaxCash( void )
{
    WCHAR szTemp[50];
    memset(szTemp, 0, sizeof(WCHAR) * 50);

    float fWebCash = static_cast<float>(( g_kGlobalValue.m_dwChinaWebCash)/1000.) /CYOU_POINT_PER_CASH_RATE;
    if ( fWebCash > MAX_CHAGE_CYOU_POINT )
    {
        fWebCash = MAX_CHAGE_CYOU_POINT*CYOU_POINT_PER_CASH_RATE;
    }
    _itow(static_cast<int>(fWebCash),szTemp,10);

    m_pEditWebCash->SetText(szTemp);
    m_pEditWebCash->SetFocus();

}

void KGCChangeCash::OnClickChangeCash( void )
{
    int iChangeCash = _wtoi(m_pkNeedCash->GetText().c_str());
    if ( MAX_CHAGE_CYOU_POINT < (float)iChangeCash )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_CHANGE_CYOU_POINT_MAX_OVER), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        return;
    }

    int iCash = _wtoi(m_pEditWebCash->GetText());
    KP2P::GetInstance()->Send_CYOU_CashChangeReq(iCash);
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
    m_pEditWebCash->SetText(L"");

}

void KGCChangeCash::OnCreateComplete( void )
{
    DOUBLE dbWebCash = static_cast<DOUBLE>(g_kGlobalValue.m_dwChinaWebCash) /1000.;
    int iChinaWebCash = static_cast<int>(g_kGlobalValue.m_dwChinaWebCash);
    if ( iChinaWebCash < 0 )
        dbWebCash = 0;

    WCHAR szWebCashTemp[128];
    memset(szWebCashTemp, 0, sizeof(WCHAR) * 128);
    swprintf_s( szWebCashTemp, 128, L"%.1lf", dbWebCash);
    std::wstring wstrWebCash = szWebCashTemp;
    m_pkCash->SetText(wstrWebCash);
}