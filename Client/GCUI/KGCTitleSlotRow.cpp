#include "stdafx.h"
#include ".\kgctitleslotrow.h"
//

//
#include "../gcui/GCMyInfoScene.h"
#include "../gcui/KGCCTConditionWindow.h"

#include "../MyD3D.h"
#include "../gcui/GCItemInfoBox.h"


IMPLEMENT_CLASSNAME( KGCTitleSlotRow );
IMPLEMENT_WND_FACTORY( KGCTitleSlotRow );
IMPLEMENT_WND_FACTORY_NAME( KGCTitleSlotRow, "gc_title_slot_row" );

KGCTitleSlotRow::KGCTitleSlotRow(void)
{
    char str[256] = "";
    for ( int i = 0; i < NUM_OF_TITLE_COL; ++i )
    {
        m_pkBGSlot[i]       = NULL;
        m_pkBGClicked[i]    = NULL;
        m_pkOn[i]           = NULL;
        m_pkOff[i]          = NULL;
        m_pkGet[i]          = NULL;
        m_pkCondition[i]    = NULL;
        m_pTextureItem[i]   = NULL;
        m_pkTimed[i]        = NULL;
        m_iMissionID[i]     = -1;
        m_iItemID[i]        = -1;

        sprintf( str, "slot%d", i+1 );
        LINK_CONTROL( str,               m_pkBGSlot[i] );
        sprintf( str, "clicked%d", i+1 );
        LINK_CONTROL( str,               m_pkBGClicked[i] );
        sprintf( str, "use_on%d", i+1 );
        LINK_CONTROL( str,               m_pkOn[i] );
        sprintf( str, "use_off%d", i+1 );
        LINK_CONTROL( str,               m_pkOff[i] );
        sprintf( str, "get%d", i+1 );
        LINK_CONTROL( str,               m_pkGet[i] );
        sprintf( str, "condition%d", i+1 );
        LINK_CONTROL( str,               m_pkCondition[i] );     
        sprintf( str, "time%d", i+1 );
        LINK_CONTROL( str,               m_pkTimed[i] );
    }
}

KGCTitleSlotRow::~KGCTitleSlotRow(void)
{
}

void KGCTitleSlotRow::OnCreate( void )
{
    for ( int i = 0; i < NUM_OF_TITLE_COL; ++i )
    {
        m_pkBGSlot[i]->InitState( true, true, this );
        m_pkBGClicked[i]->InitState( true, true, this );
        m_pkOn[i]->InitState( true, true, this );
        m_pkOff[i]->InitState( true, true, this );
        m_pkGet[i]->InitState( true, true, this );
        m_pkCondition[i]->InitState( true, true, this );
        m_pkTimed[i]->InitState( false, true, this );
    }

}

void KGCTitleSlotRow::OnDestroy( void )
{
	for(int i=0;i<NUM_OF_TITLE_COL;++i)
		m_pTextureItem[i]	= NULL;
}

void KGCTitleSlotRow::ActionPerformed( const KActionEvent& event )
{
    if( D3DWE_BUTTON_CLICK == event.m_dwCode )
    {
        for(int i=0;i<NUM_OF_TITLE_COL;++i)
        {
            g_KDSound.Play( "31" ); 
            if( event.m_pWnd == m_pkOn[i] )
            {
                // 장착정보를 바꾼다.;
                if( g_pItemMgr->FindInventoryForItemID( m_iItemID[i] ) )
                {
                    SCharInfo &SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();    
                    if( g_pItemMgr->EquipTitleItem( &SChar, m_iItemID[i] ) )
                    {
#ifndef NATION_BRAZIL 
                        if ( g_pkUIScene != NULL )
                        {                            
                            std::wostringstream strmText;
                            strmText << g_pkStrLoader->GetReplacedString( STR_ID_EQUIP_TITLE_ITEM, "L"
                                , g_pItemMgr->GetItemData( m_iItemID[i] )->strItemName );
							g_pkUIScene->EnableMessageBox( false );
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                strmText.str(), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
							
                        }
#endif
                    }
                    g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();

                    SpeakToActionListener( KActionEvent( this, -1, REFRESH_TITLE_LIST, 0 ) );          // -1 이면 모두 갱신
                }
                else
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CANT_FIND_ITEM ), L""
                                            , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
                }
            }
            else if( event.m_pWnd == m_pkOff[i] )
            {
                // 장착정보를 끈다.;
                SCharInfo &SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
                g_pItemMgr->UnEquipTitleItem( &SChar );
                g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
                g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd( -1 );
                SpeakToActionListener( KActionEvent( this, i, REFRESH_TITLE_LIST, 0 ) );           // Row + i 인덱스만 갱신
            }
            else if( event.m_pWnd == m_pkGet[i] )
            {
                // 장비를 얻는다.;
                KP2P::GetInstance()->Send_CollectionComplete( m_iMissionID[i] );
            }
            else if( event.m_pWnd == m_pkCondition[i] )
            {
                g_pkUIScene->m_pkCTConditionWindow->RefreshAll();
                g_pkUIScene->m_pkCTConditionWindow->SetMission( GetMissionID(i) );
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_CONDITION_WINDOW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
            }
        }
    }
}

void KGCTitleSlotRow::PostDraw( void )
{
    DrawTitle();
}

void KGCTitleSlotRow::FrameMoveInEnabledState( void )
{
	FrameMove_Alpha();

    if( g_pkUIScene )
    {
        if( g_pkUIScene->m_pkItemInformationBox )
        {
            D3DXVECTOR2 vPos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

            for(int i=0;i<NUM_OF_TITLE_COL;++i)
            {
                if( -1 == m_iItemID[i] )
                    continue;

                if( m_pkBGSlot[i]->CheckPosInWindowBound( vPos ) )
                {
                    D3DXVECTOR2 vecPos = m_pkBGSlot[i]->GetCurrentWindowPos();
                    vecPos.x += m_pkBGSlot[i]->GetWidth(); /*70;*/
                    vecPos.y += (m_pkBGSlot[i]->GetHeight() / 2.0f );

                    vecPos.x *= m_pkBGSlot[i]->GetWindowScaleX();
                    vecPos.y *= m_pkBGSlot[i]->GetWindowScaleY();

                    GCItem* pItem = g_pItemMgr->GetItemData( m_iItemID[i] );

                    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vecPos);
                    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
					g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
                    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

					// 능력치 감소율 제거.
					//if( g_pItemMgr->FindInventoryForItemID( m_iItemID[i] ) )
					//{
					//	const float fDecreaseAbility = g_pItemMgr->GetTitleDecreasePercentByItemID( m_iItemID[i] );

					//	if( fDecreaseAbility > 0.0f )
					//	{
					//		
					//		char szTemp[1024];
					//		sprintf( szTemp, "%.2f", fDecreaseAbility );

					//		g_pkUIScene->m_pkCollectionItemInfoBox->SetTextColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					//		g_pkUIScene->m_pkToolTip->ShowToolTip( m_pkBGSlot[i]
					//									, g_pkStrLoader->GetReplacedString( STR_ID_TITLE_ITEM_DECEASE_ABILITY, "s", GCUTIL_STR::GCStrCharToWide(szTemp) )
					//									, D3DXVECTOR2( 0.0f, 0.0f ), false );															

					//		// 아이템 정보창의 위치를 조금 바꿔서 사용
					//		vecPos.x -= 15.0f;
					//		vecPos.y -= 40.0f;
					//		g_pkUIScene->m_pkToolTip->SetWindowPosDirect(vecPos);
					//	}
					//}
                    return;
                }
            }
        }
    }	
}

void KGCTitleSlotRow::FrameMove_Alpha(void)
{
	for(int i=0;i<NUM_OF_TITLE_COL;++i)
	{
		// Get 이 활성화 되어있을 때
		if( m_pkGet[i]->IsRenderOn() )
		{
			if( s_bAlphaDir )
				m_pkBGClicked[i]->SetAllWindowColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
			else
				m_pkBGClicked[i]->SetAllWindowColor( D3DCOLOR_ARGB(0, 255, 255, 255) );
		}
	}

	if( abs( (long int)(timeGetTime() - s_dwLastUpdateTime) ) > 25 )
	{
		if( 255 <= s_bAlpha )
			s_bAlphaDir = false;
		else if( 0 >= s_bAlpha )
			s_bAlphaDir = true;

		int iResultValue = (int)s_bAlpha;

		if( s_bAlphaDir )
			iResultValue += 20;
		else
			iResultValue -= 20;

		if( iResultValue > 255 )
			s_bAlpha = 255;
		else if( iResultValue < 0 )
			s_bAlpha = 0;
		else
			s_bAlpha = iResultValue;

		s_dwLastUpdateTime = timeGetTime();
	}
}

void KGCTitleSlotRow::SetClicked( int iIndex, bool bOn )
{
    if( !IsTitleSlotIndex(iIndex) )
        return;

    m_pkBGClicked[iIndex]->ToggleRender( bOn );
	m_pkBGClicked[iIndex]->SetAllWindowColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
}

void KGCTitleSlotRow::SetUseBtnType( int iIndex, TITLE_SLOT_BTN enType )
{
    if( !IsTitleSlotIndex(iIndex) )
        return;

    m_pkOn[iIndex]->ToggleRender( false );
    m_pkOff[iIndex]->ToggleRender( false );
    m_pkGet[iIndex]->ToggleRender( false );
    m_pkCondition[iIndex]->ToggleRender( false );
    SetClicked( iIndex, false );

    bool isNotCoordiTitle;
    GCItem* titleCheckTemp = g_pItemMgr->GetItemData(m_iItemID[iIndex]);
    if (titleCheckTemp != NULL)
        isNotCoordiTitle = ((titleCheckTemp->dwSlotPosition & ESP_TITLE) && (titleCheckTemp->eItemKind == GIK_TITLE_ITEM));
    else
        isNotCoordiTitle = true;

    switch( enType )
    {
        case TSB_GET:
            m_pkGet[iIndex]->ToggleRender( true );
			SetClicked( iIndex, true );
            break;
        case TSB_ON:
            m_pkOn[iIndex]->ToggleRender( isNotCoordiTitle );
            break;
        case TSB_OFF:
            m_pkOff[iIndex]->ToggleRender( isNotCoordiTitle );
            SetClicked( iIndex, true );
            break;
        case TSB_CONDITION:
            m_pkCondition[iIndex]->ToggleRender( true );
            break;
    }
}

void KGCTitleSlotRow::ReSet(void)
{
    for(int i=0;i<NUM_OF_TITLE_COL;++i)
    {
        SetUseBtnType( i, KGCTitleSlotRow::TSB_NONE );
        SetClicked( i, false );

        m_iMissionID[i]     = -1;
        m_iItemID[i]        = -1;
        m_pTextureItem[i]	= NULL;
        m_pkTimed[i]->ToggleRender( false );
        m_pkTimed[i]->SetToolTip( L"" );
    }
}

bool KGCTitleSlotRow::IsTitleSlotIndex( int iIndex )
{
    if( iIndex < NUM_OF_TITLE_COL && iIndex >= 0 )
        return true;
    
    return false;
}

void KGCTitleSlotRow::DrawTitle(void)
{
    for(int i=0;i<NUM_OF_TITLE_COL;++i)
    {
        if ( m_iItemID[i] != -1 )
        {
            D3DXVECTOR2 vPos( m_pkBGSlot[i]->GetRelocatedWindowPos() );
            if( m_pTextureItem[i] )
            {
                g_pItemMgr->Draw_ShopImage( vPos, (float)m_pkBGSlot[i]->GetWidth() * m_fWindowScaleX, (float)m_pkBGSlot[i]->GetHeight()* m_fWindowScaleY, m_pTextureItem[i], false );
            }
        }
    }
}

void KGCTitleSlotRow::SetItemInfo(int iIndex, int iItemID, GCDeviceTexture *pTextureItem)
{
    KItem* tempItem = g_pItemMgr->GetInventoryItemFromID( iItemID );
    if ( tempItem != NULL )
    {
        if ( tempItem->m_nPeriod != KItem::UNLIMITED_ITEM )
        {
            if ( m_pkTimed[iIndex] )
            {
                m_pkTimed[iIndex]->ToggleRender( true );
                m_pkTimed[iIndex]->SetToolTip( g_pItemMgr->GetRemainTime( *tempItem ) );
            }
        }
        else
        {
            m_pkTimed[iIndex]->ToggleRender( false );
            m_pkTimed[iIndex]->SetToolTip( L"" );
        }
    }
    m_iItemID[iIndex]		= iItemID;
	m_pTextureItem[iIndex]	= pTextureItem;
}
