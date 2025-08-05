#include "stdafx.h"
#include "GCMonsterCard.h"
#include "GCMonsterCardBook.h"
#include "GCItemManager.h"
#include "GCItemImgWnd.h"


IMPLEMENT_CLASSNAME( KGCMonsterCardBook );
IMPLEMENT_WND_FACTORY( KGCMonsterCardBook );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterCardBook, "gc_monster_card_book" );

KGCMonsterCardBook::KGCMonsterCardBook( void )
: m_dwClickedOwnFilterIndex( EOFBT_OWN )
, m_dwClickedSlotFilterIndex( ESFBT_ALL )
, m_dwClickedGradeFilterIndex( EGFBT_ALL )
, m_iCurrentPage( 0 )
, m_pkActionEventTransfer( NULL )
, m_pkPrePageBtn( NULL )
, m_pkNextPageBtn( NULL )
, m_pkOwnCradCount( NULL )
, m_pkCurrentPage( NULL )
, m_iMaxPage( 0 )
, m_bRate(false)
, m_iOrderComma(1)
{
    for( int i = 0 ; i < MAX_MONSTER_CARD_SIMPLE_INFO; ++i )
    {
        char szTemp[MAX_PATH] = "";
        
       pkMonsterCardContents[i] = NULL;
        sprintf_s( szTemp, MAX_PATH, "monster_card_contents%d", i );
        LINK_CONTROL( szTemp, pkMonsterCardContents[i] );
    }
    
 
    for( int i = 0 ; i < ESFBT_MAX; ++i )
    {
        m_stSlotFilterBtn[i].pkSlotFilterBtn = NULL;
        char strTemp[ MAX_PATH ] = "";
        sprintf_s( strTemp, MAX_PATH, "slot_filter_btn%d", i);
        LINK_CONTROL( strTemp, m_stSlotFilterBtn[i].pkSlotFilterBtn );
    }


    m_stSlotFilterBtn[ESFBT_ALL].dwSlotFilter = ESP_SET_ITEM;
    m_stSlotFilterBtn[ESFBT_HELMET].dwSlotFilter = ESP_HELMET;
    m_stSlotFilterBtn[ESFBT_JACKET_MANTLE].dwSlotFilter = ESP_JACKET | ESP_MANTLE;
    m_stSlotFilterBtn[ESFBT_PANTS_SHOES].dwSlotFilter = ESP_PANTS | ESP_SHOES;
    m_stSlotFilterBtn[ESFBT_GLOVES_WEAPON].dwSlotFilter = ESP_GLOVES | ESP_WEAPON;
    m_stSlotFilterBtn[ESFBT_ACCESSORIES].dwSlotFilter = ESP_A_UPPER_HELMET | ESP_A_DOWN_HELMET | ESP_A_PANTS | ESP_A_ARM | ESP_A_FINGER | ESP_A_SHOES | ESP_A_JACKET | ESP_A_NECKLET | ESP_A_FINGER | ESP_A_SHOES;

    for( int i = 0 ; i < EGFBT_MAX; ++i )
    {
        m_stGradeFilterBtn[i].pkGradeFilterBtn = NULL;
        char strTemp[ MAX_PATH ] = "";
        sprintf_s( strTemp, MAX_PATH, "grade_filter_btn%d", i);
        LINK_CONTROL( strTemp, m_stGradeFilterBtn[i].pkGradeFilterBtn );
    }

    m_stGradeFilterBtn[ EGFBT_ALL ].cGradeFilter = KItem::GRADE_NUM;
    m_stGradeFilterBtn[ EGFBT_NORMAL ].cGradeFilter = KItem::GRADE_NORMAL;
    m_stGradeFilterBtn[ EGFBT_RARE ].cGradeFilter = KItem::GRADE_RARE;
    m_stGradeFilterBtn[ EGFBT_EPIC ].cGradeFilter = KItem::GRADE_EPIC;
    m_stGradeFilterBtn[ EGFBT_LEGEND ].cGradeFilter = KItem::GRADE_LEGEND;
    m_stGradeFilterBtn[ EGFBT_MYSTIC ].cGradeFilter = KItem::GRADE_MYSTIC;

    for( int i = 0 ; i < EOFBT_MAX; ++i )
    {
        m_stOwnFilterBtn[i].pkOwnFilterBtn = NULL;
        char strTemp[ MAX_PATH ] = "";
        sprintf_s( strTemp, MAX_PATH, "own_filter_btn%d", i);
        LINK_CONTROL( strTemp, m_stOwnFilterBtn[i].pkOwnFilterBtn );
    }

    m_stOwnFilterBtn[ EOFBT_ALL ].bOwnFilter = false;
    m_stOwnFilterBtn[ EOFBT_OWN ].bOwnFilter = true;

    LINK_CONTROL( "pre_page_btn", m_pkPrePageBtn );
    LINK_CONTROL( "next_page_btn", m_pkNextPageBtn );
    LINK_CONTROL( "close_btn", m_pkCloseBtn);
    LINK_CONTROL( "my_monster_card_count", m_pkOwnCradCount );
    LINK_CONTROL( "current_monster_card_book_page", m_pkCurrentPage );

    LINK_CONTROL( "monster_card_contents_big", pkMonsterCardContentsBig );
    LINK_CONTROL( "monster_card_contents_big_dummy", pkMonsterCardContentsBigDummy );
    
    LINK_CONTROL( "monster_card_contents_dummy", pkMonsterCardContentsDummy );


    m_pkReceiveActionListener = this;
}

KGCMonsterCardBook::~KGCMonsterCardBook( void )
{
}

void KGCMonsterCardBook::OnCreate( void )
{
    for( int i = 0 ; i < ESFBT_MAX; ++ i )
    {
        m_stSlotFilterBtn[i].pkSlotFilterBtn->InitState( true, true, this );
    }

    for( int i = 0 ; i < EGFBT_MAX; ++ i )
    {
        m_stGradeFilterBtn[i].pkGradeFilterBtn->InitState( true, true, this );
    }

    for( int i = 0 ; i < EOFBT_MAX; ++ i )
    {
        m_stOwnFilterBtn[i].pkOwnFilterBtn->InitState( true, true, this );
    }

    m_pkPrePageBtn->InitState( true, true, this );
    m_pkNextPageBtn->InitState( true, true, this );
    m_pkCloseBtn->InitState( true, true, this );
	m_pkCloseBtn->SetHotKey( DIK_ESCAPE );


    for(int i = 0; i < MAX_MONSTER_CARD_SIMPLE_INFO; ++i)
    {
        pkMonsterCardContents[i]->InitState( true, true, this );        
        pkMonsterCardContents[i]->SetMonsterCardSignPos(pkMonsterCardContentsDummy->InitMonsterCardSignPos());
    }
    pkMonsterCardContentsBig->InitState( false, false );
    pkMonsterCardContentsBig->SetMonsterCardSignPos(pkMonsterCardContentsBigDummy->InitMonsterCardSignPos());
    
    

    m_pkOwnCradCount->SetAlign( DT_CENTER );
    m_pkCurrentPage->SetAlign( DT_CENTER );
}

void KGCMonsterCardBook::OnCreateComplete( void )
{
    m_dwClickedOwnFilterIndex = EOFBT_OWN;
    m_stSlotFilterBtn[ m_dwClickedSlotFilterIndex ].pkSlotFilterBtn->Lock( true );
    m_stGradeFilterBtn[ m_dwClickedGradeFilterIndex ].pkGradeFilterBtn->Lock( true );
    m_stOwnFilterBtn[ m_dwClickedOwnFilterIndex ].pkOwnFilterBtn->Lock( true );
    m_iCurrentPage = 0;

    MakeMonsterCardShowList();
}

void KGCMonsterCardBook::MakeMonsterCardShowList()
{
    std::vector< GCItem *> vecStanDardMonsterCardList = g_pItemMgr->GetMonsterCardList();
    std::vector< GCItem *> vecOwnMonsterCardList = g_pItemMgr->GetOwnMonsterCardList();

    m_pkOwnCradCount->SetText( g_pkStrLoader->GetReplacedString(STR_ID_MY_MONSTER_CARD_COUNT, "ii", static_cast<int>( vecOwnMonsterCardList.size() ), static_cast<int>( vecStanDardMonsterCardList.size() ) ) );

    if( EOFBT_OWN == m_dwClickedOwnFilterIndex )
        vecStanDardMonsterCardList.swap( vecOwnMonsterCardList );

    //정렬해주고

    m_vecShowMonsterCardList.clear();

    std::vector< GCItem* >::iterator vit = vecStanDardMonsterCardList.begin();
    for( ; vit != vecStanDardMonsterCardList.end(); ++vit )
    {
        if( ESFBT_ALL != m_dwClickedSlotFilterIndex && 
            !( m_stSlotFilterBtn[m_dwClickedSlotFilterIndex].dwSlotFilter & (*vit )->dwSlotPosition ) )
            continue;

        if( EGFBT_ALL != m_dwClickedGradeFilterIndex && 
            m_stGradeFilterBtn[m_dwClickedGradeFilterIndex].cGradeFilter != (*vit ) ->cItemGrade )
            continue;

        m_vecShowMonsterCardList.push_back( (*vit)->dwGoodsID );
    }

    m_iCurrentPage = 0;
    int iCardListSize = static_cast<int>( m_vecShowMonsterCardList.size() );
    m_iMaxPage = ( iCardListSize - 1 ) / MAX_MONSTER_CARD_SIMPLE_INFO;

    m_pkCurrentPage->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_BOOK_CURRENT_PAGE, "ii", m_iCurrentPage + 1, m_iMaxPage + 1) );

    SetMonsterCardSimpleInfoList();
}

void KGCMonsterCardBook::FrameMoveInEnabledState( void )
{
    POINT ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)ptMouse.x, (float)ptMouse.y );
    
    pkMonsterCardContentsBig->AllOff();
    
    for( int i = 0 ; i < MAX_MONSTER_CARD_SIMPLE_INFO; ++i )
    {
        //m_stMonsterCardSimpleInfo[i].pkMonsterCardContents->m_pkMonsterCardImg->SetWndMode( m_stMonsterCardSimpleInfo[i].pkMonnsterCardBackGround->GetWndMode() );
        if( pkMonsterCardContents[i]->GetMonsterCardID() > 0 && 
            pkMonsterCardContents[i]->CheckPosInWindowBoundWithChild( vMousePos ) )
        {
            //수량과 관련
            D3DCOLOR dwColor = 0xffffffff;
            if( pkMonsterCardContents[i]->GetDuration() <= 0 )
                dwColor = 0xff555555;

            pkMonsterCardContentsBig->SetMonsterCardItemID(pkMonsterCardContents[i]->GetMonsterCardID());
            pkMonsterCardContentsBig->SetMonsterCardContents();
            pkMonsterCardContentsBig->SetMonsterCardDecimalSignOffSet(12.f);
            pkMonsterCardContentsBig->SetMonsterCardSignOffSet(24.f);
            pkMonsterCardContentsBig->SetMonsterCardLVNumScale(0.6f);
            pkMonsterCardContentsBig->SetMonsterCardNumScale(0.7f);
            pkMonsterCardContentsBig->SetMonsterCardPrimeNumScale(0.7f);
            pkMonsterCardContentsBig->SetMonsterCardAllNumShadow(false);
            pkMonsterCardContentsBig->ToggleRender( true );

            break;
        }
    }

}

void KGCMonsterCardBook::ActionPerformed( const KActionEvent& event )
{
    if( D3DWE_BUTTON_CLICK == event.m_dwCode)
    {
        for( int i = 0 ; i < ESFBT_MAX; ++i)
        {
            if( event.m_pWnd == m_stSlotFilterBtn[i].pkSlotFilterBtn)
            {
                m_stSlotFilterBtn[m_dwClickedSlotFilterIndex].pkSlotFilterBtn->Lock(false);
                m_stSlotFilterBtn[i].pkSlotFilterBtn->Lock(true);
                m_dwClickedSlotFilterIndex = i;
                MakeMonsterCardShowList();

                return;
            }
        }

        for( int i = 0 ; i < EGFBT_MAX; ++i)
        {
            if( event.m_pWnd == m_stGradeFilterBtn[i].pkGradeFilterBtn)
            {
                m_stGradeFilterBtn[m_dwClickedGradeFilterIndex].pkGradeFilterBtn->Lock( false );
                m_stGradeFilterBtn[i].pkGradeFilterBtn->Lock( true );
                m_dwClickedGradeFilterIndex = i;
                MakeMonsterCardShowList();
                return;
            }
        }

        for( int i = 0 ; i < EOFBT_MAX; ++i )
        {
            if( event.m_pWnd == m_stOwnFilterBtn[i].pkOwnFilterBtn)
            {
                m_stOwnFilterBtn[ m_dwClickedOwnFilterIndex ].pkOwnFilterBtn->Lock( false );
                m_stOwnFilterBtn[i].pkOwnFilterBtn->Lock( true );
                m_dwClickedOwnFilterIndex = i;
                MakeMonsterCardShowList();
                return;
            }
        }

        for( int i = 0 ; i < MAX_MONSTER_CARD_SIMPLE_INFO; ++i )
        {
            if( event.m_pWnd == pkMonsterCardContents[i])
            {
                if( pkMonsterCardContents[i]->GetDuration() > 0 )
                {
                   pkMonsterCardContents[i]->AddActionListener( m_pkReceiveActionListener );

                    if( m_pkActionEventTransfer )
                        pkMonsterCardContents[i]->SpeakToActionListener( KActionEvent( m_pkActionEventTransfer , CLICK_MONSTER_CARD_SIMPLE_INFO, pkMonsterCardContents[i]->GetMonsterCardID()) );

                }
                else
                {
                    if( m_pkActionEventTransfer )
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_DONT_HAVE_MONSTER_CARD ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                }

                return;
            }
        }

        if( event.m_pWnd == m_pkCloseBtn )
        {
            OnClose();
            return;
        }

        if( event.m_pWnd == m_pkPrePageBtn )
        {
            ClickMovePageBtn( -1 );
            return;
        }

        if( event.m_pWnd == m_pkNextPageBtn )
        {
            ClickMovePageBtn( 1 );
            return;
        }
    }
}

void KGCMonsterCardBook::SetMonsterCardSimpleInfoList()
{
    int iStartIndex = m_iCurrentPage * MAX_MONSTER_CARD_SIMPLE_INFO;
    int iSizeList = m_vecShowMonsterCardList.size();

     
    for( int i = 0 ; i < MAX_MONSTER_CARD_SIMPLE_INFO; ++i )
    {
        int iIndex = iStartIndex + i;
        if( iIndex < iSizeList )
        {
            
            pkMonsterCardContents[i]->AllOff();
            pkMonsterCardContents[i]->SetMonsterCardItemID( m_vecShowMonsterCardList[ iIndex ]);
            pkMonsterCardContents[i]->SetDuration(g_pItemMgr->GetInventoryItemDuration( m_vecShowMonsterCardList[ iIndex ]));
           // pkMonsterCardContents[i]->InitValue();
            pkMonsterCardContents[i]->SetMonsterCardDecimalSignOffSet(5.f);
            pkMonsterCardContents[i]->SetMonsterCardSignOffSet(13.f);
            

            D3DCOLOR dwColor = 0xffffffff;
            if( pkMonsterCardContents[i]->GetDuration() <= 0 )
                dwColor = 0xff808080;

            pkMonsterCardContents[i]->SetMonsterCardContents();   
            pkMonsterCardContents[i]->SetMonsterCardAllNumShadow( false );
         }
        else
        {
            pkMonsterCardContents[i]->AllOff();                    
        }
    }
}

void KGCMonsterCardBook::PostChildDraw( void )
{

    //만약 여기서 안그려지면 상위 컨트롤 자체를 Render하는것이 아니라, 
    //togglerender를 true로 바꿔주는 함수를 따로 만들어서 할 것.
    if( true == pkMonsterCardContentsBig->GetMonsterCardImgRenderOn() )
          pkMonsterCardContentsBig->Render();


}

void KGCMonsterCardBook::OnDestroyComplete( void )
{
    m_pkReceiveActionListener = this;
    m_pkActionEventTransfer = NULL;

    m_stSlotFilterBtn[m_dwClickedSlotFilterIndex].pkSlotFilterBtn->Lock( false );
    m_stGradeFilterBtn[m_dwClickedGradeFilterIndex].pkGradeFilterBtn->Lock( false );
    m_stOwnFilterBtn[m_dwClickedOwnFilterIndex].pkOwnFilterBtn->Lock( false );
    m_dwClickedSlotFilterIndex = ESFBT_ALL;
    m_dwClickedGradeFilterIndex = EGFBT_ALL;
    m_dwClickedOwnFilterIndex = EOFBT_ALL;

}

void KGCMonsterCardBook::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCMonsterCardBook::ClickMovePageBtn( int iMoveCount_ )
{
    int iResultPage = m_iCurrentPage + iMoveCount_;
    if(iResultPage < 0 )
        iResultPage = m_iMaxPage;

    if( iResultPage > m_iMaxPage )
        iResultPage = 0;

    m_iCurrentPage = iResultPage;

    m_pkCurrentPage->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_BOOK_CURRENT_PAGE, "ii", m_iCurrentPage + 1, m_iMaxPage + 1) );
    SetMonsterCardSimpleInfoList();
}

void KGCMonsterCardBook::SetClickedSlotFilterBtn( DWORD dwSlotPosition )
{
    for( int i = 0 ; i < ESFBT_MAX; ++i )
    {
        if( m_stSlotFilterBtn[i].dwSlotFilter & dwSlotPosition )
        {
            m_dwClickedSlotFilterIndex = static_cast<DWORD>(i);
            return;
        }
    }
    m_dwClickedSlotFilterIndex = ESFBT_ALL;
}

bool KGCMonsterCardBook::MakeItemOnFirstSlot( IN DWORD dwItemID )
{
    std::vector<GCITEMID>::iterator iter = m_vecShowMonsterCardList.begin();

    for( ; iter != m_vecShowMonsterCardList.end(); ++iter ){
        if( (*iter) == dwItemID ){
            std::iter_swap( iter, m_vecShowMonsterCardList.begin() );
            SetMonsterCardSimpleInfoList();
            return true;
        }
    }
    return false;
}
