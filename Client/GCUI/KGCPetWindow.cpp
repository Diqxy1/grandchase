#include "stdafx.h"
#include ".\KGCPetWindow.h"



#include "../MyD3D.h"

#include "../gcui/GCItemInfoBox.h"
#include "../gcui/KGCPetPreview.h"
#include "../gcui/KGCPetMessage.h"


IMPLEMENT_CLASSNAME( KGCPetWindow );
IMPLEMENT_WND_FACTORY( KGCPetWindow );
IMPLEMENT_WND_FACTORY_NAME( KGCPetWindow, "gc_pet_window" );

KGCPetWindow::KGCPetWindow(void)
{
    char str[256] = "";
    for ( int i = 0; i < NUM_OF_BOX; ++i )
    {
        m_pkPetBox[i]       = NULL;
        m_pkQuestionMark[i] = NULL;

        sprintf( str, "slot%d", i+1 );
        LINK_CONTROL( str,               m_pkPetBox[i] );

        sprintf( str, "question_mark%d", i+1 );
        LINK_CONTROL( str,               m_pkQuestionMark[i] );

        m_enState[i] = CTS_CLOSE;

		m_pTextureItem[i] = NULL;
    }

    for(int i=0;i<NUM_OF_ROW;++i)
    {
        for(int j=0;j<NUM_OF_COL-1;++j)
        {
            sprintf( str, "evolution_arrow%d_%d", i+1, j+1 );
            LINK_CONTROL( str,               m_pkEvolutionArrow[i][j] );
        }
    }

    m_pkClose   = NULL;
    LINK_CONTROL( "close_button",               m_pkClose );

    m_pkScrollBar       = NULL;
    LINK_CONTROL( "replay_box_scroll",               m_pkScrollBar );

    m_pkEmoticonBtn     = NULL;
    m_pkTitleBtn        = NULL;
    m_pkSelPetBtn       = NULL;
    m_pkMonsterCardBtn  = NULL;

    LINK_CONTROL( "emoticon_button",       m_pkEmoticonBtn );
    LINK_CONTROL( "title_button",          m_pkTitleBtn );
    LINK_CONTROL( "pet_button_big",        m_pkSelPetBtn );
    LINK_CONTROL( "monster_card_button",   m_pkMonsterCardBtn );

    m_iRow = 0;

    LoadingItemList();
}

KGCPetWindow::~KGCPetWindow(void)
{
	DestroyPetShopImg();
}

void KGCPetWindow::OnCreate( void )
{
    for ( int i = 0; i < NUM_OF_BOX; ++i )
    {
        m_pkPetBox[i]->InitState( true, true, this );
        m_pkQuestionMark[i]->InitState( true, true, this );
        m_pTextureItem[i] = NULL;
    }

    for(int i=0;i<NUM_OF_ROW;++i)
    {
        for(int j=0;j<NUM_OF_COL-1;++j)
        {
            m_pkEvolutionArrow[i][j]->InitState( true, true, this );
        }
    }

    m_pkClose->InitState( true, true, this );
    m_pkClose->SetHotKey( DIK_ESCAPE );
    m_pkScrollBar->InitState( true, true, this );

    m_pkEmoticonBtn->InitState(true, true, this);
    m_pkTitleBtn->InitState( true, true, this );
    m_pkSelPetBtn->InitState( true, true, this );

	if (m_pkMonsterCardBtn)
		m_pkMonsterCardBtn->InitState( true, true, this );

    m_pkScrollBar->SetScrollPos( 0 );
    m_iRow = 0;


    if ( !GC_GLOBAL_DEFINE::bRealTimeLoadEtcTex ) 
    {
	    CreatePetShopImg();
    }

    RefreshPetSlot();

}

void KGCPetWindow::OnDestroy( void )
{
    for(int i=0;i<NUM_OF_BOX;++i)
		m_pTextureItem[i] = NULL;
}

void KGCPetWindow::ActionPerformed( const KActionEvent& event )
{
    if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
    {
        g_KDSound.Play( "31" ); 

        if( event.m_pWnd == m_pkClose )
        {
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
        }
        else if ( event.m_pWnd == m_pkEmoticonBtn )
        {
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_EMOTICON_COLLECTION ) );
        }
        else if ( event.m_pWnd == m_pkTitleBtn )
        {
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_TITLE_COLLECTION ) );
        }
        else if( event.m_pWnd == m_pkMonsterCardBtn )
        {
			if (m_pkMonsterCardBtn)
			{
				SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MONSTER_CARD_BOOK, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
			}           
        }

        D3DXVECTOR2 vInputPos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
        for( int i=0; i < (int)NUM_OF_BOX; ++i )
        {
            if( (event.m_pWnd == m_pkPetBox[i]) && (CTS_CLOSE != m_enState[i]) )
            {
                const int iIndex = GetFirstPetIndex()+i;
                if( 0 <= iIndex && iIndex < (int)m_vecPetList.size() )
                {
                    if( g_pkUIScene->m_pkPetPreview )
                    {
                        // 진화한 녀석의 프리뷰를 보여준다
                        g_pkUIScene->MessageBox( KGCUIScene::GC_PET_PREVIEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
                        g_pkUIScene->m_pkPetPreview->SetMyInfoPreview();
                        g_pkUIScene->m_pkPetPreview->SetDescState( 2 );
                        g_pkUIScene->m_pkPetPreview->SetNowPet( g_kGlobalValue.GetPetIDFromItemID( m_vecPetList[ iIndex ].first.first ), m_vecPetList[ iIndex ].first.second );
                        g_pkUIScene->m_pkPetPreview->m_pkPetMsg->SetMentState( PET_MENT_FIRST_MEETING );
                    }
                }
            }
        }        
    }

    if( event.m_pWnd == m_pkScrollBar )
    {
        if( m_pkScrollBar->GetScrollPos() != m_iRow )
        {
            RefreshPetSlot();
            m_iRow = m_pkScrollBar->GetScrollPos();
            
            if( g_pkUIScene->m_pkItemInformationBox )
                g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
        }
    }
}

void KGCPetWindow::PostChildDraw( void )
{
    for( int i=0; i < (int)NUM_OF_BOX; ++i )
    {
        if( CTS_CLOSE != m_enState[i] )
        {
            BYTE iAlpha = PCT_NO_BLEND;
            if( CTS_OPEN == m_enState[i] )
                iAlpha = PCT_NO_BLEND;
            else if( CTS_HALF_OPEN == m_enState[i] )
                iAlpha = PCT_HALF_BLEND;

            D3DXVECTOR2 vPetPox = D3DXVECTOR2( m_pkPetBox[i]->GetRelocatedWindowPos().x - 6.0f * GC_SCREEN_DIV_WIDTH * m_fWindowScaleX, m_pkPetBox[i]->GetRelocatedWindowPos().y );
            if( m_pTextureItem[i] ) 
            {
                g_pItemMgr->Draw_ShopImage( vPetPox, (float)m_pkPetBox[i]->GetWidth() * m_fWindowScaleX, (float)m_pkPetBox[i]->GetHeight() * m_fWindowScaleY, m_pTextureItem[i], false, -1,D3DCOLOR_ARGB(iAlpha,255,255,255)  ); 
            }
        }
    }
}

void KGCPetWindow::FrameMoveInEnabledState( void )
{
    if( g_pkUIScene )
    {
        if( g_pkUIScene->m_pkItemInformationBox )
        {
            D3DXVECTOR2 vPos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

            for(int i=0;i<NUM_OF_BOX;++i)
            {
                if( NULL == m_pTextureItem[i] )
                    continue;

                if( m_pkPetBox[i]->CheckPosInWindowBound( vPos ) )
                {
                    D3DXVECTOR2 vecPos = m_pkPetBox[i]->GetCurrentWindowPos();
                    vecPos.x += 70;
                    vecPos.y += ( m_pkPetBox[i]->GetHeight() / 2.0f );
                    vecPos.x *= m_pkPetBox[i]->GetWindowScaleX();
                    vecPos.y *= m_pkPetBox[i]->GetWindowScaleY();

                    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vecPos);

                    const int iItemID = m_vecPetList[ GetFirstPetIndex()+i ].first.first;
                    const char cPromotion = m_vecPetList[ GetFirstPetIndex()+i ].first.second;
                    GCItem* pItem = g_pItemMgr->GetItemData( iItemID );

                    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pItem, cPromotion);
                    g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
                    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

                    return;
                }
            }

            g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
        }
    }
}

void KGCPetWindow::SetCollectionState( int iIndex, COLLECTION_STATE enState )
{
    if( (int)m_vecPetList.size() <= iIndex )
        return;

    const int ciSlotIndex = GetSlotIndexByIndex( iIndex );

    m_pkQuestionMark[ciSlotIndex]->ToggleRender( false );

    switch( enState )
    {
        case CTS_CLOSE:
            m_pkQuestionMark[ciSlotIndex]->ToggleRender( true );
            break;
        case CTS_OPEN:
            break;
        case CTS_HALF_OPEN:
            break;
    }

    m_enState[iIndex] = enState;
}

void KGCPetWindow::SetItem( int iIndex, int iItemID, char cPromotion )
{
    if( (int)m_vecPetList.size() <= iIndex )
        return;

    const int ciSlotIndex = GetSlotIndexByIndex( iIndex );
    const int ciItemID = m_vecPetList[iIndex].first.first;

    if( -1 == ciSlotIndex )
        return;

	if( m_mapPetShopImg.count( std::make_pair( iItemID, cPromotion ) ) )
    {
		m_pTextureItem[ciSlotIndex] = m_mapPetShopImg[ std::make_pair( iItemID, cPromotion ) ];
    }
	else
    {
        m_mapPetShopImg.insert( std::make_pair( std::make_pair(iItemID, cPromotion)
            , g_pItemMgr->CreatePetItemTexture( iItemID, cPromotion ) ) );
		
        m_pTextureItem[ciSlotIndex] = m_mapPetShopImg[ std::make_pair( iItemID, cPromotion ) ];
    }
}

int KGCPetWindow::GetFirstPetIndex(void)
{
    return (m_pkScrollBar->GetScrollPos() * NUM_OF_COL);
}

int KGCPetWindow::GetSlotIndexByIndex(int iIndex)
{
    const int ciFirst_SIndex   = GetFirstPetIndex();
    const int ciEnd_SIndex     = ciFirst_SIndex + (NUM_OF_BOX);
    
    if( iIndex < ciFirst_SIndex || iIndex >= ciEnd_SIndex )
        return -1;

    return iIndex - GetFirstPetIndex();
}

void KGCPetWindow::LoadingItemList( void )
{
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr , "collection_missionlist.lua" ) == false )
        return ;

    m_vecPetList.clear();

    LUA_BEGIN_TABLE( "COLLECTION_MISSIONLIST", return )
    {
        char strTable[MAX_PATH];
        strcpy( strTable, "PET" );
        LUA_BEGIN_TABLE( strTable, return )
        {
            for( int i = 1; ; ++i )
            {
                LUA_BEGIN_TABLE( i, break )
                {
                    for( int j = 1; ; ++j )
                    {
                        LUA_BEGIN_TABLE( j, break )
                        {
                            int iID         = -1;
                            int iPromotion  = 0;
                            LUA_GET_VALUE_DEF( "ID", iID, -1 );
                            LUA_GET_VALUE_DEF( "Promotion", iPromotion, 0 );

                            m_vecPetList.push_back( std::make_pair( std::make_pair(iID, (char)iPromotion), false ) ); 
                        }
                        LUA_END_TABLE( break )
                    }
                }
                LUA_END_TABLE( break )
            }
        }
        LUA_END_TABLE( return )
    }
    LUA_END_TABLE( return );
}

void KGCPetWindow::RefreshPetSlot( void )
{
    for(int i=0;i<NUM_OF_BOX;++i)
    {
        m_enState[i] = CTS_CLOSE;
        m_pkPetBox[i]->ToggleRender( true );
        m_pkQuestionMark[i]->ToggleRender( true );
        m_pTextureItem[i] = NULL;
    }

    for(int i=0;i<NUM_OF_ROW;++i)
    {
        for(int j=0;j<NUM_OF_COL-1;++j)
        {
            m_pkEvolutionArrow[i][j]->ToggleRender( true );
        }
    }

    for(int i=0;i<(int)m_vecPetList.size();++i)
    {
        m_vecPetList[i].second = false;

        int iSlotID = GetSlotIndexByIndex( i );
        if( -1 != iSlotID )
        {
            const int iRow = iSlotID / NUM_OF_COL;
            const int iCol = (iSlotID % NUM_OF_COL) - 1;

            if( -1 == m_vecPetList[i].first.first )
            {
                m_pkPetBox[iSlotID]->ToggleRender( false );
                m_pkQuestionMark[iSlotID]->ToggleRender( false );
                
                if( iCol >= 0 )
                {
                    m_pkEvolutionArrow[iRow][iCol]->ToggleRender( false );
                }
            }
            else
            {
                if( -1 == iCol )
                {
                    m_pkQuestionMark[iSlotID]->ToggleRender( false );
                    SetItem( i, m_vecPetList[ i ].first.first, m_vecPetList[ i ].first.second );
                    m_enState[iSlotID] = CTS_HALF_OPEN;
                }
            }
        }
    }

    std::map< GCITEMUID, KPetInfo >::iterator mitPet;
    for( mitPet = g_MyD3D->m_mapPetInfo.begin(); mitPet != g_MyD3D->m_mapPetInfo.end(); ++mitPet )
    {
        KPetInfo kPet = mitPet->second;

        int iCurPetIndex = GetPetIndex( kPet.m_dwID, kPet.m_cPromotion );

        if( -1 != iCurPetIndex )
        {
            int iCurSlotIndex = GetSlotIndexByIndex( iCurPetIndex );
            m_vecPetList[ iCurPetIndex ].second = true;
            const int iCol = (iCurSlotIndex % NUM_OF_COL) + 1;

            for(int i=0;i<iCol;++i)
            {
                int iSlotIndex = iCurSlotIndex - i;
                int iPetIndex = iCurPetIndex - i;

                if( 0 <= iSlotIndex )
                {
                    m_pkQuestionMark[ iSlotIndex ]->ToggleRender( false );
                    SetItem( iPetIndex, m_vecPetList[ iPetIndex ].first.first, m_vecPetList[ iPetIndex ].first.second );
                    if( m_vecPetList[ iPetIndex ].second )
                        m_enState[ iSlotIndex ] = CTS_OPEN;
                    else
                        m_enState[ iSlotIndex ] = CTS_HALF_OPEN;
                }
            }
        }
    }

    m_pkScrollBar->SetScrollPageSize( 1 );
    m_pkScrollBar->SetScrollRangeMin( 0 );

    int iScrollSize = m_vecPetList.size() / NUM_OF_COL;
    if( (int)m_vecPetList.size() % NUM_OF_COL > 0 )
        ++iScrollSize;
    if( iScrollSize >= (NUM_OF_ROW-1) )
        iScrollSize -= (NUM_OF_ROW-1);
    m_pkScrollBar->SetScrollRangeMax( iScrollSize );
}

int KGCPetWindow::GetPetIndex( int iItemID, char cPromotion )
{
    for(int i=0;i<(int)m_vecPetList.size();++i)
    {
        if( iItemID == m_vecPetList[i].first.first
            && cPromotion == m_vecPetList[i].first.second
            )
            return i;
    }

    return -1;;
}

void KGCPetWindow::CreatePetShopImg(void)
{
	DestroyPetShopImg();

	std::vector< std::pair< std::pair<int, char>, bool> >::iterator it = m_vecPetList.begin();
	for(;it != m_vecPetList.end();it++)
	{
		const int iItemID		= it->first.first;
		const char cPromotion	= it->first.second;

		m_mapPetShopImg.insert( std::make_pair( std::make_pair(iItemID, cPromotion)
			, g_pItemMgr->CreatePetItemTexture( iItemID, cPromotion ) ) );
	}

}

void KGCPetWindow::DestroyPetShopImg(void)
{
	std::map< std::pair<int, char>, GCDeviceTexture* >::iterator it = m_mapPetShopImg.begin();
	for(;it != m_mapPetShopImg.end();it++)
		SAFE_RELEASE( it->second );

	m_mapPetShopImg.clear();
}

bool KGCPetWindow::IsEqualPet( int iItemID, char cPromotion )
{
    if( iItemID != g_pkUIScene->m_pkCollectionItemInfoBox->GetItemID() ||
        cPromotion != g_pkUIScene->m_pkCollectionItemInfoBox->GetPetEvolutionLevel() )
    {
        return false;
    }

    return true;
}
