#include "stdafx.h"
#include "GCRewardItemListBox.h"

IMPLEMENT_CLASSNAME( KGCRewardItemListBox );
IMPLEMENT_WND_FACTORY( KGCRewardItemListBox );
IMPLEMENT_WND_FACTORY_NAME( KGCRewardItemListBox, "gc_reward_item_list_box" );


KGCRewardItemListBox::KGCRewardItemListBox( void )
: m_pkClose(NULL)
, m_pkImgTitleVipUser(NULL)
, m_pkImgTitleNewBuyUser(NULL)
, m_pkimgItemBackground(NULL)
, m_pkScroll(NULL)
, m_pkStaticGuide(NULL)
, m_pkSboxMainBackground(NULL)
, m_pkSboxInnerBak(NULL)
, m_pkSboxTitleBackground(NULL)
, m_iRows( 3 )
, m_iCols( 5 )
{
    LINK_CONTROL( "btn_close", m_pkClose );
    LINK_CONTROL( "img_title_vip_user", m_pkImgTitleVipUser );
    LINK_CONTROL( "img_title_new_buy_user", m_pkImgTitleNewBuyUser);

    LINK_CONTROL( "img_item_box", m_pkimgItemBackground);
    LINK_CONTROL( "scroll", m_pkScroll );
    LINK_CONTROL( "sb_main_background", m_pkSboxMainBackground );  
    LINK_CONTROL( "sb_item_background", m_pkSboxInnerBak );    
    LINK_CONTROL( "static_guide", m_pkStaticGuide );    
    LINK_CONTROL( "sb_title_background", m_pkSboxTitleBackground );

}

KGCRewardItemListBox::~KGCRewardItemListBox( void )
{

}

void KGCRewardItemListBox::OnCreateComplete( void )
{
    InitVariable();
    RefreshPage();
}

void KGCRewardItemListBox::AddItem( GCITEMID ItemID_, int iDuration /*= 0 */ )
{
    FindItem findId;
    findId.SetItemID( ItemID_ );

    std::vector< std::pair< GCITEMID, int > >::iterator vit;
    vit = std::find_if( m_vecItem.begin(), m_vecItem.end(), findId );

    if( vit == m_vecItem.end() )
    {
        m_vecItem.push_back( std::make_pair(ItemID_, iDuration) );		
        //m_vecItemSelected.push_back(false);
    }
    else
        vit->second += iDuration;		

}


void KGCRewardItemListBox::OnCreate( void )
{
    m_pkClose->InitState( true, true, this );
    m_pkImgTitleVipUser->InitState( true );
    m_pkImgTitleNewBuyUser->InitState( false );
    m_pkimgItemBackground->InitState(false);

    m_pkScroll->InitState( true, true, this );

    m_pkClose->SetHotKey( DIK_ESCAPE );

    float fFontSpace = 1.0f;
#if defined(NATION_IDN) || defined(NATION_USA)
    fFontSpace = 0.0f;
#endif

    m_pkStaticGuide->SetFontSpace( fFontSpace );
    m_pkStaticGuide->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

    m_pkStaticGuide->SetText( L"여기에 안내문구 넣기" );


    if ( m_vecItemWnd.empty() )
    {
        D3DXVECTOR2 vLeftTopPos = m_pkSboxInnerBak->GetFixedWindowLocalPos();
        vLeftTopPos.x += 6;
        vLeftTopPos.y += 5;

        int iBoxCount = m_iCols * m_iRows;

        for ( int i = 0; i < iBoxCount ; i++ )
        {			
            KGCUserItemWnd* wnd = (KGCUserItemWnd*)g_pkUIMgr->CreateTemplateUI( this, "gc_user_item_wnd", "RewardItemListBox.stg", "box_item", true, true );   
            D3DXVECTOR2 vPos = vLeftTopPos;
            vPos.x = vLeftTopPos.x + (float)( wnd->GetWidth() * ( i % m_iCols ) );
            vPos.y = vLeftTopPos.y + (float)( wnd->GetHeight() * ( i / m_iCols ) );
            wnd->SetFixedWindowLocalPos( vPos );
            wnd->InitState( true , true, this );
            m_vecItemWnd.push_back( wnd );
        }
    }



    static bool bInit = true;
    if( bInit ) {
        bInit = false;

        m_vtClosePos    = m_pkClose->GetFixedWindowLocalPos();
        m_vtStaticGuidePos = m_pkStaticGuide->GetFixedWindowLocalPos();
  

        m_dwScrollHeight   = m_pkScroll->GetHeight();

        m_ptSboxMainBackgroundWH.x = m_pkSboxMainBackground->GetWidth();
        m_ptSboxMainBackgroundWH.y = m_pkSboxMainBackground->GetHeight();

        m_ptSboxInnerBakWH.x       = m_pkSboxInnerBak->GetWidth();
        m_ptSboxInnerBakWH.y       = m_pkSboxInnerBak->GetHeight();

        m_ptimgItemBackgroundWH.x = m_pkimgItemBackground->GetWidth();
        m_ptimgItemBackgroundWH.y = m_pkimgItemBackground->GetHeight();
    }

    InitVariable();


    RefreshPage();

}

void KGCRewardItemListBox::InitVariable( void )
{   
    //빈 상자 출력
    std::vector< KGCUserItemWnd* >::iterator vitUI = m_vecItemWnd.begin();
    while( vitUI != m_vecItemWnd.end() ) 
    {
        (*vitUI)->SetItem();
        vitUI++;
    }

}
void KGCRewardItemListBox::ClearItemList( void )
{
    m_vecItem.clear();
    //m_vecItemSelected.clear();
}

void KGCRewardItemListBox::FrameMoveInEnabledState( void )
{

    std::vector< std::pair< GCITEMID, int > >::iterator vitData = m_vecItem.begin() + m_pkScroll->GetScrollPos() * m_iCols;
    GCItem* pGCItem = NULL;


    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
    if( m_pkSboxInnerBak->CheckPosInWindowBound( vMousePos ) )
    {
        for( std::vector< KGCUserItemWnd* >::iterator vit = m_vecItemWnd.begin();
            vit != m_vecItemWnd.end() && vitData != m_vecItem.end() ; ++vit, ++vitData)		
        {
            if( (*vit)->CheckPosInWindowBound( vMousePos ) )
            {
                pGCItem = g_pItemMgr->GetItemData((*vitData).first);
                if ( pGCItem == NULL )
                    continue;

                D3DXVECTOR2 vecPos = (*vit)->GetCurrentWindowPos();
                vecPos.x += (*vit)->GetWidth();
                vecPos.x *= m_fWindowScaleX;
                vecPos.y *= m_fWindowScaleY;					
                g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
                g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pGCItem, -1 );
                g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();						
                g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
                g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );       			
            }
        }
    }

    static int oldPage = 0;
    if ( oldPage != m_pkScroll->GetScrollPos() )
    {
        RefreshPage();
        oldPage = m_pkScroll->GetScrollPos();
    }

    if ( g_pkInput->GetZMov() ) // mouse wheel
    {
        if( g_pkInput->GetZMov() > 0 )
        {
            m_pkScroll->SetScrollPos( m_pkScroll->GetScrollPos() - 1 );			
        }
        else 
        {			
            m_pkScroll->SetScrollPos( m_pkScroll->GetScrollPos() + 1 );			
        }	
    }
}

void KGCRewardItemListBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkClose,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
}


void KGCRewardItemListBox::RefreshPage(void)
{   
    int iItemNum = m_vecItem.size();
    int iViewRows = 1;
    bool bScroll = false;


    for ( int i = 0 ; i < (int)m_vecItemWnd.size() ; i++ ) {
        m_vecItemWnd[i]->ToggleRender( true );
    }

    if( iItemNum <= m_iCols ) { //m_icols의 변화에따라 코드 변환없이돌아가도록 수정 되면 좋겠다
        for ( int i = m_iCols ; i < (int)m_vecItemWnd.size() ; i++ ) {
            m_vecItemWnd[i]->ToggleRender( false );
        }
    }else if ( iItemNum <= (m_iCols*2) ) {
        for ( int i = (m_iCols*2) ; i < (int)m_vecItemWnd.size() ; i++ ) {
            m_vecItemWnd[i]->ToggleRender( false );
            iViewRows = 2;
        }
    }
    else if( iItemNum <= (m_iCols * 3) ){
        iViewRows = 3;
    }
    else {
        bScroll = true;
        iViewRows = 3;
    }
    m_pkScroll->ToggleRender( bScroll );
    ResizeWindow( iViewRows, bScroll );


    m_pkScroll->SetScrollRangeMax( (m_vecItem.size() - 1) / m_iCols );
    m_pkScroll->SetScrollPageSize( 2 );




    std::vector< std::pair< GCITEMID, int > >::iterator vitData = m_vecItem.begin() + m_pkScroll->GetScrollPos() * m_iCols;
    std::vector< KGCUserItemWnd* >::iterator vitUI = m_vecItemWnd.begin();

    //빈 상자 셋팅
    while( vitUI != m_vecItemWnd.end() ) 
    {
        (*vitUI)->SetItem();
        vitUI++;
    }

    vitUI = m_vecItemWnd.begin();
    
    for( int i = m_pkScroll->GetScrollPos() * m_iCols ; vitUI != m_vecItemWnd.end() && vitData < m_vecItem.end() ; ++vitData ) 
    {
        GCItem* pGCItem = NULL;
        pGCItem = g_pItemMgr->GetItemData((*vitData).first);

        (*vitUI)->SetItem( pGCItem );
        (*vitUI)->SetItemDuration( (*vitData).second==0?-1:(*vitData).second );

        vitUI++;
        i++;
    }
}

void KGCRewardItemListBox::ResizeWindow( int _row, bool _scroll )
{
    //높이 설정
    int iAddHeight = (_row - 1) * m_ptimgItemBackgroundWH.y;    //더할 높이이므로 (_row - 1)

    m_pkSboxMainBackground->SetHeight(m_ptSboxMainBackgroundWH.y + iAddHeight);
    m_pkSboxInnerBak->SetHeight(m_ptSboxInnerBakWH.y + iAddHeight);

    m_pkScroll->SetHeight(m_dwScrollHeight + iAddHeight);

    //가로 설정 
    DWORD dwTemp;
    _scroll ? dwTemp = 0 : dwTemp = 1;
    DWORD dwMinusWidth = dwTemp * m_pkScroll->GetWidth();
    DWORD dwMinusHalfWidth = (dwMinusWidth)/2; 

    m_pkClose->SetWindowPosX(m_vtClosePos.x - (float)dwMinusWidth);
    m_pkStaticGuide->SetWindowPosX(m_vtStaticGuidePos.x - (float)dwMinusWidth);
    m_pkSboxMainBackground->SetWidth(m_ptSboxMainBackgroundWH.x - dwMinusWidth);
    m_pkSboxInnerBak->SetWidth(m_ptSboxInnerBakWH.x - dwMinusWidth);

}


void KGCRewardItemListBox::OnClose()
{
    ClearItemList();
    this->ToggleRender(false );
}

void KGCRewardItemListBox::SetDesc( const std::wstring& strText )
{
    m_pkStaticGuide->SetTextAutoMultiline( strText.c_str() );
}