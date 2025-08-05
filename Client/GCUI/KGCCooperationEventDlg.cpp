#include "stdafx.h"
#include "KGCCooperationEventDlg.h"
#include "GCItemImgWnd.h"
#include "Procedure.h"

IMPLEMENT_CLASSNAME( KGCCooperationEventDlg );
IMPLEMENT_WND_FACTORY( KGCCooperationEventDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCooperationEventDlg, "gc_cooperation_event_dlg" );

KGCCooperationEventDlg::KGCCooperationEventDlg(void)
: m_pkOkBtn( NULL )
, m_pkCloseBtn( NULL )
, m_pkWebLinkBtn( NULL )
, m_pkCouponCode( NULL )
, m_pkScrollBarBtn( NULL )
, m_pkEditCouponInput( NULL )
, m_pkStaticCouponDesc( NULL )
, m_pkStaticEventTitle( NULL )
, m_pkStaticEventDesc( NULL )
, m_pkStaticMyPointString( NULL )
, m_pkEventLogImg( NULL )
, m_pkScrollBar( NULL )
, m_pkItemInfoBox( NULL )
, m_bScriptLoadComplete( false )
, m_iEventID( 0 )
, m_iTitleStringID( 0 )
, m_iDescStringID( 0 )
, m_iDescToolTipStringID( 0 )
, m_pLogoTexture( NULL )
{
    for(int i = 0 ; i < MAX_ITEM_VIEW; ++i)
    {
        m_pkItemPointBar[i] = NULL;
        m_pkBuyBtn[i] = NULL;
        m_pkItemImgView[i] = NULL;
        
        char szTemp[MAX_PATH] = { 0, };
        sprintf(szTemp, "item_point_bar_back%d", i + 1);
        LINK_CONTROL( szTemp, m_pkItemPointBar[i]);

        sprintf(szTemp, "btn_buy%d", i + 1);
        LINK_CONTROL( szTemp, m_pkBuyBtn[i]);

        sprintf(szTemp, "item_img_view%d", i + 1);
        LINK_CONTROL( szTemp, m_pkItemImgView[i]);

    }

    LINK_CONTROL( "btn_ok", m_pkOkBtn );
    LINK_CONTROL( "btn_web_link", m_pkWebLinkBtn );
    LINK_CONTROL( "btn_close", m_pkCloseBtn );

    LINK_CONTROL( "coupon_code", m_pkCouponCode );

    LINK_CONTROL( "scroll_bar_back", m_pkScrollBarBtn );

    LINK_CONTROL("edit_coupon_input", m_pkEditCouponInput );
    LINK_CONTROL("static_coupon_desc", m_pkStaticCouponDesc );
    LINK_CONTROL("static_event_title", m_pkStaticEventTitle );
    LINK_CONTROL("static_event_desc", m_pkStaticEventDesc );
    LINK_CONTROL("my_point_string", m_pkStaticMyPointString );
    LINK_CONTROL("event_log_img_view", m_pkEventLogImg );
    LINK_CONTROL("scroll_bar", m_pkScrollBar );
    LINK_CONTROL("item_info_box", m_pkItemInfoBox);
}   

KGCCooperationEventDlg::~KGCCooperationEventDlg(void)
{
    SAFE_RELEASE( m_pLogoTexture );
}

void KGCCooperationEventDlg::OnCreate()
{
    if(false == m_bScriptLoadComplete)
    {
        LoadCooperationEventInfo();
        SAFE_RELEASE(m_pLogoTexture);
        m_pLogoTexture = g_pGCDeviceManager2->CreateTexture( m_strLogoImgFileName );
    }

    for(int i = 0 ; i < MAX_ITEM_VIEW; ++i)
    {
        m_pkItemPointBar[i]->InitState(false,true, this);
        m_pkBuyBtn[i]->InitState(false, true, this);
        m_pkItemImgView[i]->InitState(false, true, this);
    }
    
    m_pkOkBtn->InitState(true, true, this);
    m_pkWebLinkBtn->InitState( true, true, this);
    m_pkCloseBtn->InitState(true, true, this);
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    m_pkCouponCode->InitState(true ,true ,this);
    m_pkScrollBarBtn->InitState(true, true, this);
    m_pkEditCouponInput->InitState(true, true, this);
    m_pkStaticCouponDesc->InitState(false, true, this);
    m_pkStaticEventTitle->InitState(true, true, this);
    m_pkStaticEventTitle->SetText( g_pkStrLoader->GetString(m_iTitleStringID) );
    m_pkStaticEventDesc->InitState(true, true, this);
    m_pkStaticEventDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( m_iDescStringID ) );
    m_pkStaticEventDesc->SetToolTip( g_pkStrLoader->GetString( m_iDescToolTipStringID ) );
    m_pkStaticMyPointString->InitState( true, true, this );
    m_pkScrollBar->InitState( true, false, this);
    m_pkItemInfoBox->InitState(false);
}


bool KGCCooperationEventDlg::LoadCooperationEventInfo( )
{
    KLuaManager     luaMgr;

    if(false == GCFUNC::LoadLuaScript(luaMgr, "CooperationEventInfo.stg") )
        return false;

    LUA_BEGIN_TABLE("CooperationEventInfo", return false )
    {
        for( int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                std::string strWebLink;
                LUA_GET_VALUE_DEF( "TitleStringID", m_iTitleStringID, 0);
                LUA_GET_VALUE_DEF( "DescStringID", m_iDescStringID, 0);
                LUA_GET_VALUE_DEF( "DescToolTip", m_iDescToolTipStringID, 0);
                LUA_GET_VALUE_DEF( "LogoImageFileName", m_strLogoImgFileName, "");
                LUA_GET_VALUE_DEF( "WebLink", strWebLink, "");

                m_strWebLink = GCUTIL_STR::GCStrCharToWide( strWebLink.c_str() );

            }
            LUA_END_TABLE( return false )
        }
    }
    LUA_END_TABLE(return false )

    m_bScriptLoadComplete = true;

    return true;
}

void KGCCooperationEventDlg::OnCreateComplete( void )
{
    m_pkStaticMyPointString->SetText( g_pkStrLoader->GetReplacedString(STR_ID_COOPERATION_EVENT_MY_POINT, "i", GetMyMoneyPoint() ) );
    CodeInputEditWndInit();

    m_pkScrollBar->SetScrollPos( 0 );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollPageSize( 1 );
    InitSaleItemView();

    int iSaleItemSize = static_cast<int>( m_mapCoopEventItemInfo.size() );
    int iScrollMax = 0;
    int iCount =  iSaleItemSize - MAX_ITEM_VIEW;

    if(iCount > 0)
    {
        iScrollMax = iCount / ONE_LINE_ITEM_COUNT;
        if( (iCount % ONE_LINE_ITEM_COUNT) > 0)
            ++iScrollMax;
    }
    
    m_pkScrollBar->SetScrollRangeMax( iScrollMax + 1 );
}

void KGCCooperationEventDlg::FrameMoveInEnabledState( void )
{
    int iSaleItemIndex = m_pkScrollBar->GetScrollPos() * ONE_LINE_ITEM_COUNT;
    int iMaxItemView = GetItemViewCount( iSaleItemIndex) ;

    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    bool bCheck = false;
    for(int i = 0;i < iMaxItemView;++i)
    {
        std::map< int, KCoopEventItem>::iterator mit = m_mapCoopEventItemInfo.find( i + iSaleItemIndex);
        if( m_pkItemImgView[i]->CheckPosInWindowBound( vMousePos ) && 
            mit != m_mapCoopEventItemInfo.end() )
        {
            D3DXVECTOR2 vNewPos = m_pkItemImgView[i]->GetCurrentWindowPos();
            vNewPos.x *= m_fWindowScaleX;
            vNewPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();
            vNewPos.y *= m_fWindowScaleY;

            GCItem* pItem = g_pItemMgr->GetItemData(mit->second.m_dwItemID);
            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vNewPos );
            g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

            bCheck = true;
            break;
        }
    }

    if( !bCheck )
    {
        //m_pkItemInfoBox->ToggleRender(false);
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );

    }
}

void KGCCooperationEventDlg::ActionPerformed(const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn,            KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
    GCWND_MSG_MAP( m_pkEditCouponInput,     KD3DWnd::D3DWE_EDIT_RETURN, OnCodeInput );
    GCWND_MSG_MAP( m_pkOkBtn,               KD3DWnd::D3DWE_BUTTON_CLICK, OnCodeInput );
    GCWND_MSG_MAP( m_pkWebLinkBtn,          KD3DWnd::D3DWE_BUTTON_CLICK, OnWebLink );

    for(int i = 0 ; i < MAX_ITEM_VIEW; ++i)
    {
        if( event.m_pWnd == m_pkBuyBtn[i] && D3DWE_BUTTON_CLICK == event.m_dwCode )
        {
            OnBuy( i );
            return;
        }
    }


    GCWND_MAP( m_pkScrollBar, OnScroll );
}


void KGCCooperationEventDlg::OnExit()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCCooperationEventDlg::OnScroll()
{
    InitSaleItemView();
}

void KGCCooperationEventDlg::InitSaleItemView()
{
    int iTopIndex = m_pkScrollBar->GetScrollPos();
    int iSaleItemIndex = iTopIndex * ONE_LINE_ITEM_COUNT;
    int iMaxItemView = GetItemViewCount( iSaleItemIndex) ;

    for(int i = 0 ; i < iMaxItemView; ++i)
    {
        m_pkItemPointBar[i]->ToggleRender( true );
        m_pkBuyBtn[i]->ToggleRender( true );
        m_pkItemImgView[i]->ToggleRender( true );
        
        std::map< int, KCoopEventItem>::iterator mit = m_mapCoopEventItemInfo.find( i + iSaleItemIndex);
        if( mit != m_mapCoopEventItemInfo.end() )
            m_pkItemImgView[i]->SetItemInfo( mit->second.m_dwItemID );
    }

    for(int i = iMaxItemView; i < MAX_ITEM_VIEW; ++i)
    {
        m_pkItemPointBar[i]->ToggleRender( false );
        m_pkBuyBtn[i]->ToggleRender( false );
        m_pkItemImgView[i]->ToggleRender( false );
    }
}

void KGCCooperationEventDlg::PostChildDraw( void )
{
    int iSaleItemIndex = m_pkScrollBar->GetScrollPos() * ONE_LINE_ITEM_COUNT ;
    int iMaxItemView = GetItemViewCount( iSaleItemIndex );

    for(int i = 0 ; i < iMaxItemView; ++i)
    {
        std::map< int, KCoopEventItem>::iterator mit = m_mapCoopEventItemInfo.find( i + iSaleItemIndex);
        
        if( mit != m_mapCoopEventItemInfo.end() )
        {
            // 인벤토리에 있는 수량 제한 아이템이라면 남은 수량을 표시한다.
            g_pkUIMgr->RenderNumber( "blue_gradation_num", D3DXVECTOR2( (m_pkItemPointBar[i]->GetFixedWindowPos().x + 2.0f )* m_fWindowScaleX, (m_pkItemPointBar[i]->GetFixedWindowPos().y + 3.0f) * m_fWindowScaleY),mit->second.m_nPrice, true, false, -1.0f,true );
        }
    }

    D3DXVECTOR2 vPos = m_pkEventLogImg->GetFixedWindowPos();

    if( m_pLogoTexture != NULL )
    {
        g_pGCDeviceManager2->DrawInScreenResize( m_pLogoTexture, vPos.x, vPos.y, 
                                                vPos.x + (float)m_pkEventLogImg->GetWidth() , vPos.y + (float)m_pkEventLogImg->GetHeight() , 0.0f, 0.0f, 1.0f, 1.0f );
    }
}

void KGCCooperationEventDlg::OnBuy( int iIndex_ )
{
    int iSaleItemIndex = iIndex_ + ( m_pkScrollBar->GetScrollPos() * ONE_LINE_ITEM_COUNT );

    std::map< int, KCoopEventItem>::iterator mit = m_mapCoopEventItemInfo.find( iSaleItemIndex );
    if( mit == m_mapCoopEventItemInfo.end() )
        return;

    if(GetMyMoneyPoint() < mit->second.m_nPrice)
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COOPERATION_EVENT_ERROR3), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    Result_Buy_CooperationItem = INT_MAX;

    KP2P::GetInstance()->Send_CoOpItemBuyReq(mit->first );

    g_MyD3D->WaitForServerAck( Result_Buy_CooperationItem, INT_MAX, 5000, TIME_OUT_VALUE );

    m_pkStaticMyPointString->SetText( g_pkStrLoader->GetReplacedString(STR_ID_COOPERATION_EVENT_MY_POINT, "i", GetMyMoneyPoint() ) );
}

int KGCCooperationEventDlg::GetItemViewCount(int iItemIndex_)
{
    int iViewItemCount = static_cast<int>(m_mapCoopEventItemInfo.size() ) - iItemIndex_;
    
    if(iViewItemCount < MAX_ITEM_VIEW)
        return iViewItemCount;

    return MAX_ITEM_VIEW;
}

void KGCCooperationEventDlg::OnCodeInput()
{
    std::wstring strInputString = m_pkEditCouponInput->GetText() ;
    
    if(strInputString.empty())
        return;

    if(IsCorrectCode( GCUTIL_STR::GCStrWideToChar( strInputString.c_str() ) ) )
    {
        Result_CouponCode_Input = INT_MAX;
        KP2P::GetInstance()->Send_UseGameCouponReq( m_iEventID, strInputString , L"");
        g_MyD3D->WaitForServerAck( Result_CouponCode_Input, INT_MAX, 5000, TIME_OUT_VALUE );
    }
    else
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_WRONG_COUPON_CODE), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
    
    CodeInputEditWndInit();
}

bool KGCCooperationEventDlg::IsCorrectCode( std::string strInputCode_)
{
    for( int i = 0 ; i < (int)strInputCode_.size() ; ++i )
    {
        if( !isalnum(strInputCode_[i] ) )
        {
            return false;
        }
    }

    return true;
}

void KGCCooperationEventDlg::OnWebLink()
{   
    PopUpWebPage( m_strWebLink.c_str() );
}

int KGCCooperationEventDlg::GetMyMoneyPoint()
{
    KItem* pInven = NULL;
    if(false == g_pItemMgr->FindInventoryForItemID(m_dwMoneyItemID, &pInven) )
        return 0;
    
    return pInven->m_nCount;
}

void KGCCooperationEventDlg::CodeInputEditWndInit()
{   
    m_pkEditCouponInput->SetFocus(); 
    m_pkEditCouponInput->SetText(L"");
}

bool KGCCooperationEventDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    this->CodeInputEditWndInit();
    return true;
}