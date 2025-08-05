#include "stdafx.h"
#include "GCCoordiComposeBox.h"

IMPLEMENT_CLASSNAME( KGCCoordiComposeBox );
IMPLEMENT_WND_FACTORY( KGCCoordiComposeBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiComposeBox, "gc_coordicomposebox" );

const GCITEMID COMPOSE_SCROLL_ITEMID = 45706;

KGCCoordiComposeBox::KGCCoordiComposeBox( void )
: m_OuterUID( 0 )
, m_InnerUID( 0 )
, m_nComposeType( NORMAL_COMPOSE )
, m_pkBtnClose( NULL )
, m_pkCheckBtnNormal( NULL ) 
, m_pkCheckBtnRandom( NULL ) 
, m_pkBtnCompose( NULL ) 
, m_pkScrollNum( NULL ) 
, m_pkNormalBtnStatic( NULL )
, m_pkRandomBtnStatic( NULL ) 
, m_pkGuideStatic( NULL ) 
, m_pOuterCoordi( NULL ) 
, m_pInnerCoordi( NULL ) 
, m_pResultCoordiNormal( NULL ) 
, m_pResultCoordiRandom( NULL ) 
{
    LINK_CONTROL( "btn_close",   m_pkBtnClose );
    LINK_CONTROL( "normal_compose_check",   m_pkCheckBtnNormal );
    LINK_CONTROL( "random_compose_check",   m_pkCheckBtnRandom );

    LINK_CONTROL( "compose_button",   m_pkBtnCompose );    

    LINK_CONTROL( "scroll_static",   m_pkScrollNum );
    LINK_CONTROL( "normal_compose_static",   m_pkNormalBtnStatic );
    LINK_CONTROL( "random_compose_static",   m_pkRandomBtnStatic );
    LINK_CONTROL( "normal_guide_static",   m_pkGuideStatic );    

    LINK_CONTROL( "outer_coordi_box", m_pOuterCoordi );
    LINK_CONTROL( "inner_coordi_box", m_pInnerCoordi );
    LINK_CONTROL( "result_coordi_box", m_pResultCoordiNormal );
    LINK_CONTROL( "result_coordi_box2", m_pResultCoordiRandom );    
}

KGCCoordiComposeBox::~KGCCoordiComposeBox( void )
{
}

void KGCCoordiComposeBox::OnCreate( void )
{
    if ( m_pkBtnClose ) 
    {
        m_pkBtnClose->InitState( true, true, this );
        m_pkBtnClose->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCCoordiComposeBox::OnClickBtnClose );
    }

    if ( m_pkCheckBtnNormal ) 
    {
        m_pkCheckBtnNormal->InitState( true, true, this );
        m_pkCheckBtnNormal->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCCoordiComposeBox::OnClickBtnNormalCompose );
    }

    if ( m_pkCheckBtnRandom )
    {
        m_pkCheckBtnRandom->InitState( true, true, this );
        m_pkCheckBtnRandom->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCCoordiComposeBox::OnClickBtnRandomCompose );
    }

    if ( m_pkBtnCompose ) 
    {
        m_pkBtnCompose->InitState( true, true, this );
        m_pkBtnCompose->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCCoordiComposeBox::OnClickBtnCompose );
    }
    
    if ( m_pkScrollNum ) 
    {
        m_pkScrollNum->InitState( true, false, this );
        m_pkScrollNum->SetAlign( DT_RIGHT );
    }
    
    if ( m_pkNormalBtnStatic ) 
    {
        m_pkNormalBtnStatic->InitState( true, false, this );
        m_pkNormalBtnStatic->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_NORMAL_COMPOSE ) );
        m_pkNormalBtnStatic->SetFontColor( 0xFF000000 );
        m_pkNormalBtnStatic->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    }

    if ( m_pkRandomBtnStatic ) 
    {
        m_pkRandomBtnStatic->InitState( true, false, this );
        m_pkRandomBtnStatic->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_RANDOM_COMPOSE ) );
        m_pkRandomBtnStatic->SetFontColor( 0xFF000000 );
        m_pkRandomBtnStatic->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    }

    if ( m_pkGuideStatic ) 
    {
        m_pkGuideStatic->InitState( true, false, this );
        m_pkGuideStatic->SetMultiLine( true );
        m_pkGuideStatic->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_COMPOSE_GUIDE1 ) );
        m_pkGuideStatic->SetFontColor( 0xFF000000 );
        m_pkGuideStatic->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    }

    if ( m_pOuterCoordi ) 
    {
        m_pOuterCoordi->InitState( true );
        m_pOuterCoordi->SetItem();
    }

    if ( m_pInnerCoordi ) 
    {
        m_pInnerCoordi->InitState( true );
        m_pInnerCoordi->SetItem();
    }

    if ( m_pResultCoordiNormal ) 
    {
        m_pResultCoordiNormal->InitState( true );
        m_pResultCoordiNormal->SetItem();
    }

    if ( m_pResultCoordiRandom ) 
    {
        m_pResultCoordiRandom->InitState( false );
        m_pResultCoordiRandom->SetItem();
    }
    
}

void KGCCoordiComposeBox::OnClickBtnClose()
{
    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_COORDI_COMPOSE ) );
}

void KGCCoordiComposeBox::OnClickBtnNormalCompose()
{
    m_nComposeType = NORMAL_COMPOSE;
    UpdateCheckButton();
    UpdateResultCoordi();
}

void KGCCoordiComposeBox::OnClickBtnRandomCompose()
{
    m_nComposeType = RANDOM_COMPOSE;
    UpdateCheckButton();
    UpdateResultCoordi();
}

void KGCCoordiComposeBox::OnClickBtnCompose()
{
    MessageBoxA( NULL, "합성", "", MB_OK );
//     m_pkBtnCompose->Lock( true );
//     m_pkBtnClose->Lock( true );

    //KP2P::GetInstance()->Send_CoordiCompositeReq();
}

void KGCCoordiComposeBox::ActionPerformed( const KActionEvent& event )
{
}

void KGCCoordiComposeBox::ToggleRender( bool bRender )
{
    KD3DWnd::ToggleRender( bRender );
}

void KGCCoordiComposeBox::Init()
{
    m_OuterUID = 0;
    m_InnerUID = 0;
    m_nComposeType = NORMAL_COMPOSE;

    m_pkBtnCompose->Lock( false );
    m_pkBtnClose->Lock( false );

    m_pOuterCoordi->SetItem();
    m_pInnerCoordi->SetItem();
    m_pResultCoordiNormal->SetItem();
    m_pResultCoordiRandom->SetItem();


    UpdateCheckButton();
    UpdateResultCoordi();
    UpdateScrollNum();
    UpdateGuideString();  
}

void KGCCoordiComposeBox::Reset()
{
    m_OuterUID = 0;
    m_InnerUID = 0;
    //  m_nComposeType = NORMAL_COMPOSE;
}


void KGCCoordiComposeBox::UpdateGuideString()
{
    m_pkGuideStatic->SetText( g_pkStrLoader->GetString( STR_ID_COORDI_COMPOSE_GUIDE1 ) );
}

void KGCCoordiComposeBox::UpdateScrollNum()
{
    int nItemNum = 0;
    KItem * pItem = NULL;
    if(true == g_pItemMgr->FindInventoryForItemID(COMPOSE_SCROLL_ITEMID, &pItem))
        nItemNum = pItem->m_nCount;

    std::wstring strNum = boost::str(boost::wformat(L"%1%")%nItemNum);
    m_pkScrollNum->SetText( strNum );
}

void KGCCoordiComposeBox::UpdateResultCoordi()
{
    switch( m_nComposeType )
    {
    case NORMAL_COMPOSE:
        m_pResultCoordiNormal->ToggleRender( true );
        m_pResultCoordiRandom->ToggleRender( false );
        break;
    case RANDOM_COMPOSE:
        m_pResultCoordiNormal->ToggleRender( false );
        m_pResultCoordiRandom->ToggleRender( true );
        break;    
    }
}

void KGCCoordiComposeBox::UpdateCheckButton()
{
    switch( m_nComposeType )
    {
    case NORMAL_COMPOSE:
        m_pkCheckBtnNormal->SetCheck( true );
        m_pkCheckBtnRandom->SetCheck( false );  

        break;
    case RANDOM_COMPOSE:
        m_pkCheckBtnNormal->SetCheck( false );
        m_pkCheckBtnRandom->SetCheck( true );
        break;
    }
}

bool KGCCoordiComposeBox::SetItemUID( GCITEMUID _itemUID )
{
    if ( _itemUID == 0 ) 
        return false;

    if ( _itemUID == m_OuterUID || _itemUID == m_InnerUID ) 
        return false;

    // 외부 -> 내부 -> 내부순으로 장착
    KItem* pInvenItem = NULL;
    GCItem* pItem = NULL;
    if ( !g_pItemMgr->FindInventory( _itemUID, &pInvenItem, &pItem ) )
        return false;

    if ( !pInvenItem || !pItem ) 
        return false;

    if ( pInvenItem->IsExpired() ) 
        return false;

    bool m_bOuter = false;

    if ( m_OuterUID == 0 ) 
    {
        m_bOuter = true;
    }

    GCITEMUID _CheckItemUID = 0;

    //  외부 슬롯에 장착( 내부랑 비교 ) 
    if ( m_bOuter ) 
    {
        _CheckItemUID = m_InnerUID;
    }
    else 
    {   
        _CheckItemUID = m_OuterUID;
    }

    //  아이템 체크
    if ( _CheckItemUID != 0 ) 
    {
        KItem* pInvenInner = NULL;
        GCItem* pItemInner = NULL;

        if ( !g_pItemMgr->FindInventory( _CheckItemUID, &pInvenInner, &pItemInner ) ) 
            return false;

        if ( !pInvenInner || !pItemInner ) 
            return false;

        if ( pInvenInner->IsPeriod() != pInvenItem->IsPeriod() )
        {
            //  기간제 틀림
            return false;
        }

        if ( pItemInner->dwSlotPosition != pItem->dwSlotPosition )
        {
            // 장착 위치 틀림
            return false;
        }
    }

    
    if( m_bOuter ) 
    {
        m_OuterUID = _itemUID;
        m_pOuterCoordi->SetItem( pInvenItem );
    }
    else
    {
        m_InnerUID = _itemUID;
        m_pInnerCoordi->SetItem( pInvenItem );
    }

    return true;
}