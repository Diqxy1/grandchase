#include "stdafx.h"
#include "GCItemReceiveBox.h"
#include "GCItemInfoBox.h"
#include "GCItemImgWnd.h"
#include "../MyD3D.h"
#include "KGCEmoticon.h"
#include "GCItemInformationBoxPair.h"
#include "GCShortCutItem.h"
#include "gcui/GCUserItemWnd.h"
#include "KGCQALogManager.h"
#include "gcui/KGCDungeonDetailWnd.h"
#include "QuestGameManager.h"

KGCItemReceiveBox::KGCItemReceiveBox( void )
{
    m_bEnableSell = true;
}

IMPLEMENT_CLASSNAME( KGCItemReceiveBoxNew );
IMPLEMENT_WND_FACTORY( KGCItemReceiveBoxNew );
IMPLEMENT_WND_FACTORY_NAME( KGCItemReceiveBoxNew, "gc_item_receive_box2" );

void KGCItemReceiveBoxNew::AddItem( GCITEMUID ItemUID_, int iDuration /*= 0 */ )
{
	FindItem findId;
	findId.SetItemUID( ItemUID_ );

	std::vector< std::pair< GCITEMUID, int > >::iterator vit;
	vit = std::find_if( m_vecItem.begin(), m_vecItem.end(), findId );

	if( vit == m_vecItem.end() )
	{
		m_vecItem.push_back( std::make_pair(ItemUID_, iDuration) );		
	}
	else
		vit->second += iDuration;		

    std::vector< std::pair< GCITEMUID, int > >::iterator vecIter = m_vecItem.begin();
    for(; vecIter != m_vecItem.end();) {
        if( !g_MyD3D->m_kItemMgr.FindInventory( vecIter->first ) ) {
            vecIter = m_vecItem.erase( vecIter );
        }
        else {
            KItem* pKitem = g_MyD3D->m_kItemMgr.GetInventoryItem( vecIter->first );

            if( pKitem != NULL )
            {
                GCItem* pGCitem = g_pItemMgr->GetItemData( pKitem->m_ItemID );
                if ( !pGCitem )
                {
                    vecIter = m_vecItem.erase( vecIter );
                    continue;
                }

                // 영구 아이템은 수량 -1로 
                if( pGCitem->eItemType == GCIT_PERIOD && pGCitem->iItemTypeValue == -1 ) { 
                    vecIter->second = -1;
                }

                // 칭호는 표시하지 않음 
                if( pGCitem != NULL && pGCitem->eItemKind == GIK_TITLE_ITEM ) {
                    vecIter = m_vecItem.erase( vecIter );
                    continue;
                }

            }

            ++vecIter;
        }
    }

	class ItemGradeCmp
	{
	public:
		bool operator()(const std::pair< GCITEMUID, int >& p, const std::pair< GCITEMUID, int >& q) const
		{
			KItem* itemp = g_MyD3D->m_kItemMgr.GetInventoryItem( p.first );
			KItem* itemq = g_MyD3D->m_kItemMgr.GetInventoryItem( q.first );
			if( itemp->m_cGradeID == itemq->m_cGradeID )
			{
				return itemp->m_nCount > itemq->m_nCount;
			}
			else
			{
				return itemp->m_cGradeID > itemq->m_cGradeID;
			}
		}
	};

	sort(m_vecItem.begin(), m_vecItem.end(), ItemGradeCmp());
    
}

bool KGCItemReceiveBoxNew::CheckCondition(  IN const KDialogInfo& kInfo_ )
{
    if( IsEmptyReceiveItem() )
        return false;
    return true;
}

void KGCItemReceiveBoxNew::OnCreate( void )
{
	g_pItemMgr->ClearSellItemList();
	if ( m_vecItemWnd.empty() )
	{
        KD3DWnd*    pInnerBak = GetChildByName("inner_bak");
		D3DXVECTOR2 vLeftTopPos = pInnerBak->GetFixedWindowLocalPos();
		vLeftTopPos.x += 4;
		vLeftTopPos.y += 4;

		for ( int i = 0; i < 18; i++ )
		{			
			KGCUserItemWnd* wnd = (KGCUserItemWnd*)g_pkUIMgr->CreateTemplateUI( this, "gc_user_item_wnd", "itemboxTemplate.stg", "box_item", true, true );
			D3DXVECTOR2 vPos = vLeftTopPos;
			vPos.x = vLeftTopPos.x + (float)( wnd->GetWidth() * ( i % 6 ) );
			vPos.y = vLeftTopPos.y + (float)( wnd->GetHeight() * ( i / 6 ) );
			wnd->SetFixedWindowLocalPos( vPos );
			wnd->InitState( true , true, this );
			m_vecItemWnd.push_back( wnd );
		}
    }

	m_pkOk->InitState( true, true, this );

    m_pkSellEtcItem->InitState( false, false, this );;
    KD3DWnd* check = m_pkSellEtcItem->GetChildByName("check");
    check->ToggleRender( false );		

    GetChildByName("btn_lose")->InitState( true, true, this );
    GetChildByName("btn_lose")->SetHotKey( DIK_ESCAPE );

	m_pkScroll->InitState( true, true, this );
    m_pkOk->SetHotKey( DIK_RETURN );
#ifdef USE_JOYSTICK
    m_pkOk->SetHotJoystickKey(EJK_KEY_4);
#endif
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ) {
        m_pkDungeonDetailBtn->InitState(true, true, this);
        m_pkDungeonDetailCloseBtn->InitState(false, true, this);
    }
    else {
        m_pkDungeonDetailBtn->InitState(false, true, this);
        m_pkDungeonDetailCloseBtn->InitState(false, true, this);
    }

    m_pkDungeonDetailWnd->InitState(false, true, this );
}

void KGCItemReceiveBoxNew::ClearItemList( void )
{
	m_vecItem.clear();
}

void KGCItemReceiveBoxNew::FrameMoveInEnabledState( void )
{
	const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
	if( GetChildByName("inner_bak")->CheckPosInWindowBound( vMousePos ) )
	{
		for( std::vector< KGCUserItemWnd* >::iterator vit = m_vecItemWnd.begin();
			vit != m_vecItemWnd.end(); vit++ )		
		{
			if( (*vit)->CheckPosInWindowBound( vMousePos ) )
			{
				GCItem* pItem = NULL;
				KItem* pInventoryItem = NULL;				
				if( g_pItemMgr->FindInventory( (*vit)->m_dwUID, &pInventoryItem, &pItem) && pInventoryItem && pItem )
				{
 
					// 아이템 정보 표시창 위치 보정.
					D3DXVECTOR2 vecPos = (*vit)->GetCurrentWindowPos();
					vecPos.x += (*vit)->GetWidth();
					vecPos.x *= m_fWindowScaleX;
					vecPos.y *= m_fWindowScaleY;					
					g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
					g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, pInventoryItem );
					g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();						
					g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
                    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );
				}				
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

		//SetScrollPos( ( int( fPos ) / m_iGap ) * m_iGap );		
	}
}

void KGCItemReceiveBoxNew::ActionPerformed( const KActionEvent& event )
{
	if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
	{
		if ( event.m_pWnd == m_pkOk )
		{
			if( !g_pItemMgr->IsEmptySellItemList() )
			{
				g_pItemMgr->SellItemFromList();
			}			
			ClearItemList();
            g_pkQuestManager->ClearDungeonDetailInfo();
			SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
			return;
		}
        else if( event.m_pWnd == m_pkSellEtcItem && m_bEnableSell )
        {
            KD3DWnd* check = event.m_pWnd->GetChildByName("check");
            check->ToggleRender( !check->IsRenderOn() );
            SelectEtcItem( check->IsRenderOn() );
        }

        if ( event.m_pWnd == m_pkDungeonDetailBtn)
        {
            m_pkDungeonDetailBtn->ToggleRender(false);
            m_pkDungeonDetailCloseBtn->ToggleRender(true);

            if ( m_pkDungeonDetailWnd->IsRenderOn() )
                m_pkDungeonDetailWnd->ToggleRender( false);
            else 
                m_pkDungeonDetailWnd->ToggleRender( true);
        }

        if ( event.m_pWnd == m_pkDungeonDetailCloseBtn)
        {
            m_pkDungeonDetailBtn->ToggleRender(true);
            m_pkDungeonDetailCloseBtn->ToggleRender(false);

            if ( m_pkDungeonDetailWnd->IsRenderOn() )
                m_pkDungeonDetailWnd->ToggleRender( false);
            else 
                m_pkDungeonDetailWnd->ToggleRender( true);
        }

        if ( event.m_pWnd == GetChildByName("btn_lose") && m_bEnableSell )
        {
            g_pItemMgr->ClearSellItemList();
            ClearItemList();
            g_pkQuestManager->ClearDungeonDetailInfo();
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
            return;
        }

        for( std::vector< KGCUserItemWnd* >::iterator vit = m_vecItemWnd.begin();
            vit != m_vecItemWnd.end(); vit++ )		
        {
            if( *vit == event.m_pWnd )
            {
                GCItem* pItem = NULL;
                KItem* pInventoryItem = NULL;

                if( g_pItemMgr->FindInventory( (*vit)->m_dwUID, &pInventoryItem, &pItem) && pInventoryItem && pItem )
                {

                    if ( g_pItemMgr->IsContainSellItemList( pInventoryItem->m_ItemUID ) )
                    {
                        g_pItemMgr->RemoveSellItemList( pInventoryItem->m_ItemUID );
                        (*vit)->OnSellEnd();
                    }
                    else
                    {
                        KItem tempItem = *pInventoryItem;
                        if (pInventoryItem->m_nCount != -1 )
                        {
                            tempItem.m_nCount = (*vit)->GetItemDuration();
                        }
                        g_pItemMgr->AddSellItemToList( tempItem );
                        (*vit)->OnSellStart();
                    }
                }								
            }
        }

	}
	else if( KD3DWnd::D3DWE_BUTTON_DOWN == event.m_dwCode )
	{
		if ( event.m_pWnd == m_pkOk )
			g_KDSound.Play( "31" );
	}

}

bool KGCItemReceiveBoxNew::IsEmptyReceiveItem(void)
{
    if ( g_MyD3D->m_kGuideMgr.CanUseMsgBox() == false )
        return true;

    std::vector< std::pair< GCITEMUID, int > >::iterator vitData = m_vecItem.begin();
    for( ; vitData < m_vecItem.end() ; ++vitData)
    {
        KItem* pItem = NULL;
        g_pItemMgr->FindInventory( (*vitData).first, &pItem );
        if( pItem && !g_pItemMgr->IsNonVisibleItem( pItem->m_ItemID ) )
            return false;
    }

    return true;
}

void KGCItemReceiveBoxNew::RefreshPage(void)
{
	m_pkScroll->SetScrollRangeMax( m_vecItem.size() / 6 );
	m_pkScroll->SetScrollPageSize( 2 );
	std::vector< std::pair< GCITEMUID, int > >::iterator vitData = m_vecItem.begin() + m_pkScroll->GetScrollPos() * 6;
	std::vector< KGCUserItemWnd* >::iterator vitUI = m_vecItemWnd.begin();
	for( ; vitUI != m_vecItemWnd.end() && vitData < m_vecItem.end() ; ++vitData) 
	{
		KItem* pItem = NULL;
		g_pItemMgr->FindInventory( (*vitData).first, &pItem );
        if( NULL == pItem )
            continue;

        GCItem* pItemData = g_pItemMgr->GetItemData( pItem->m_ItemID );
        if( pItemData == NULL ) { 
            continue;
        }

        if( g_pItemMgr->IsNonVisibleItem( pItem->m_ItemID ) && pItemData->eItemKind != GIK_MONSTER_CARD )
            continue;

		(*vitUI)->SetItem( pItem );
        (*vitUI)->SetItemDuration( (*vitData).second==0 ? -1 : (*vitData).second );
		if (g_pItemMgr->IsContainSellItemList( (*vitData).first ) )
		{
			(*vitUI)->OnSellStart();
		}

		vitUI++;
	}

	while( vitUI != m_vecItemWnd.end() ) 
	{
		(*vitUI)->SetItem();
		vitUI++;
	}
}

KGCItemReceiveBoxNew::KGCItemReceiveBoxNew( void )
: m_pkOk(NULL)
, m_pkSellEtcItem(NULL)
, m_pkScroll(NULL)
, m_pkDungeonDetailBtn(NULL)
,m_pkDungeonDetailCloseBtn(NULL)
, m_pkDungeonDetailWnd(NULL)
{
	LINK_CONTROL( "btn_ok", m_pkOk );
	LINK_CONTROL( "ch_selet_zaptem", m_pkSellEtcItem );
	LINK_CONTROL( "scroll", m_pkScroll );
    LINK_CONTROL( "dungeon_detail_btn" , m_pkDungeonDetailBtn );
    LINK_CONTROL( "dungeon_detail_close_btn" , m_pkDungeonDetailCloseBtn );
    LINK_CONTROL( "dungeon_datail_wnd", m_pkDungeonDetailWnd );
}

void KGCItemReceiveBoxNew::SelectEtcItem( bool bSelect )
{
	for( std::vector< std::pair< GCITEMUID, int > >::iterator vit = m_vecItem.begin(); 
		vit != m_vecItem.end(); vit++ )		
	{		
		GCItem* pItem = NULL;
		KItem* pInventoryItem = NULL;
		if( g_pItemMgr->FindInventory( vit->first, &pInventoryItem, &pItem) && pInventoryItem && pItem )
		{
			if ( pInventoryItem->m_cGradeID == KItem::GRADE_NORMAL && pInventoryItem->m_nCount == -1 && pItem->eItemKind != GIK_TITLE_ITEM )
			{
				if ( bSelect )
				{
					g_pItemMgr->AddSellItemToList( *pInventoryItem );
				}
				else
				{
					g_pItemMgr->RemoveSellItemList( pInventoryItem->m_ItemUID );
				}
			}
		}
	}

	RefreshPage();
}

void KGCItemReceiveBoxNew::OnCreateComplete( void )
{
    RefreshPage();
#if defined( USE_QA_LOG_SYSTEM )
    SiKGCQALogManager()->RecvItemLog( m_vecItem );
#endif
}

void KGCItemReceiveBoxNew::OnDestroyComplete( void )
{
    m_bEnableSell = true;
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );    
}

bool KGCItemReceiveBoxNew::InitDialog( IN const KDialogInfo& kInfo_ )
{

    return true;
    //////////////////////////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////
////////////////KGCChoiceDrop////////////////////////////////
/////////////////////////////////////////////////////////////


IMPLEMENT_CLASSNAME( KGCChoiceDrop );
IMPLEMENT_WND_FACTORY( KGCChoiceDrop );
IMPLEMENT_WND_FACTORY_NAME( KGCChoiceDrop, "gc_choice_drop" );


KGCChoiceDrop::KGCChoiceDrop( void )
: m_nSelectItemCount(0)
, m_nLimitItemCount(0)
, m_nLastSelection(-1)
{
    LINK_CONTROL( "btn_get", m_pkGet );
    LINK_CONTROL( "btn_get_lock", m_pkGetLock );
    LINK_CONTROL( "btn_get_all_lock", m_pkGetAllLock );


    LINK_CONTROL( "btn_get_all", m_pkGetAll );
    LINK_CONTROL( "btn_close", m_pkClose );
    LINK_CONTROL( "btn_cancel", m_pkCancel );
    LINK_CONTROL( "img_item_box", m_pkimgItemBackground);
    LINK_CONTROL( "img_tooltip_confirm_bg", m_pkImgTooltipConfirmBG);
    LINK_CONTROL( "img_tooltip_confirm_bg1", m_pkImgTooltipConfirmBG1);
    LINK_CONTROL( "static_tooltip_confirm", m_pkStaticTolltipConfirm);


    LINK_CONTROL( "game_option_boxcount_row_col", m_pkOptionRowCols);

    LINK_CONTROL( "scroll", m_pkScroll );

    LINK_CONTROL( "static_guide", m_pkStaticGuide );



    LINK_CONTROL( "sb_main_background", m_pkSboxMainBackground );
    LINK_CONTROL( "sb_item_background", m_pkSboxInnerBak );
    LINK_CONTROL( "sb_title_background", m_pkSboxTitleBackground );

}

void KGCChoiceDrop::OnCreateComplete( void )
{
    InitVariable();
    RefreshPage();
}

void KGCChoiceDrop::AddItem( KDropItemInfo kDropItemInfo )
{
    m_vecItem.push_back( kDropItemInfo );		
    m_vecItemSelected.push_back(false);
}


void KGCChoiceDrop::OnCreate( void )
{
    if( m_nLimitItemCount == 1 ) {
        m_pkGetLock->InitState( true, true, this );
    }
    else {
        m_pkGetAllLock->InitState( true, true, this );
    }

    m_pkGet->InitState( false, true, this );
    m_pkGetAll->InitState( false, true, this );



    m_pkClose->InitState( true, true, this );
    m_pkCancel->InitState( true, true, this );
    m_pkimgItemBackground->InitState(false);
    m_pkImgTooltipConfirmBG->InitState(false);
    m_pkImgTooltipConfirmBG1->InitState(false);
    m_pkStaticTolltipConfirm->InitState(false);


    m_pkOptionRowCols->InitState( false );
	
    m_pkScroll->InitState( true, true, this );

    m_pkClose->SetHotKey( DIK_ESCAPE );
    m_pkGet->SetHotKey( DIK_RETURN );

	float fFontSpace = 1.0f;
#if defined(NATION_IDN) || defined(NATION_USA)
	fFontSpace = 0.0f;
#endif
	m_pkStaticGuide->SetFontSpace( fFontSpace );

    m_pkStaticGuide->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

    std::wstring& strGuideMessage = g_pkStrLoader->GetReplacedString( STR_ID_CHOICE_DROP_GUIDE, "i", m_nLimitItemCount );
    m_pkStaticGuide->SetText( strGuideMessage.c_str() );

    m_pkStaticTolltipConfirm->SetFontSpace( 1 );
    m_pkStaticTolltipConfirm->SetText( g_pkStrLoader->GetString( STR_ID_SELECT_ALL_ITEM ) );


    m_iCols = m_pkOptionRowCols->GetWidth();
    m_iRows = m_pkOptionRowCols->GetHeight();

    if ( m_vecItemWnd.empty() )
    {
        D3DXVECTOR2 vLeftTopPos = m_pkSboxInnerBak->GetFixedWindowLocalPos();
        vLeftTopPos.x += 6;
        vLeftTopPos.y += 5;

        int iBoxCount = m_iCols * m_iRows;

        for ( int i = 0; i < iBoxCount ; i++ )
        {			
            KGCUserItemWnd* wnd = (KGCUserItemWnd*)g_pkUIMgr->CreateTemplateUI( this, "gc_user_item_wnd", "choiceItemboxTemplate.stg", "box_item", true, true );   
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
        m_vtGetPos      = m_pkGet->GetFixedWindowLocalPos();
        m_vtCancelPos   = m_pkCancel->GetFixedWindowLocalPos();
        m_vtClosePos    = m_pkClose->GetFixedWindowLocalPos();
        m_vtImgTooltipConfirmBGPos  = m_pkImgTooltipConfirmBG->GetFixedWindowLocalPos();
        m_vtStaticTolltipConfirmPos = m_pkStaticTolltipConfirm->GetFixedWindowLocalPos();
        m_pkImgTooltipConfirmBG->SetOrder_Top();
        m_pkImgTooltipConfirmBG1->SetOrder_Top();
        m_pkStaticTolltipConfirm->SetOrder_Top();




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

void KGCChoiceDrop::InitVariable( void )
{   
    m_nSelectItemCount = 0;

    for( size_t i = 0 ; i < m_vecItemSelected.size() ; i++ ){
        m_vecItemSelected[i] = false;
    }

    if( m_nLimitItemCount == (int)m_vecItem.size() ) {
        for( size_t i = 0 ; i < m_vecItemSelected.size() ; i++ ){
            m_vecItemSelected[i] = true;
            m_nSelectItemCount++;
        }
        m_pkStaticGuide->SetText( g_pkStrLoader->GetString( STR_ID_POSSIBLE_GET_ALL_ITEM ) );
    } else {
        m_pkStaticGuide->SetText( g_pkStrLoader->GetReplacedString( STR_ID_CHOICE_DROP_GUIDE, "i", m_nLimitItemCount ));
    }

    //빈 상자 출력
    std::vector< KGCUserItemWnd* >::iterator vitUI = m_vecItemWnd.begin();
    while( vitUI != m_vecItemWnd.end() ) 
    {
        (*vitUI)->SetItem();
        vitUI++;
    }

}
void KGCChoiceDrop::ClearItemList( void )
{
    m_vecItem.clear();
    m_vecItemSelected.clear();
    m_nLimitItemCount = -1;
}

void KGCChoiceDrop::FrameMoveInEnabledState( void )
{

    std::vector< KDropItemInfo >::iterator vitData = m_vecItem.begin() + m_pkScroll->GetScrollPos() * m_iCols;
    GCItem* pGCItem = NULL;


    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
    if( m_pkSboxInnerBak->CheckPosInWindowBound( vMousePos ) )
    {
        for( std::vector< KGCUserItemWnd* >::iterator vit = m_vecItemWnd.begin();
            vit != m_vecItemWnd.end() && vitData != m_vecItem.end() ; ++vit, ++vitData)		
        {
            if( (*vit)->CheckPosInWindowBound( vMousePos ) )
            {
                pGCItem = g_pItemMgr->GetItemData((*vitData).m_ItemID / 10 );
                if ( pGCItem == NULL )
                    continue;

                KItem kItem;
                kItem.m_ItemUID = KGCItemManager::MAXIMUM_PCBANG_UID+1;
                kItem.m_ItemID = (*vitData).m_ItemID / 10;
                kItem.m_nCount = (*vitData).m_nDuration;
                kItem.m_nPeriod = (*vitData).m_nPeriod;
                kItem.m_cEnchantLevel = (*vitData).m_cEnchantLevel;
                kItem.m_cGradeID = pGCItem->cItemGrade;
                kItem.m_vecSocket = pGCItem->m_vecSocketInfo;
                kItem.m_vecAttribute = pGCItem->m_vecAttribute;

                if ( kItem.m_nPeriod != KItem::UNLIMITED_ITEM )
                {
                    CTime cCurTime = CTime::GetCurrentTime();
                    CTimeSpan cSpanTime( kItem.m_nPeriod, 0, 0, 0 );
                    cCurTime += cSpanTime;
                    kItem.m_tEndDate = static_cast<time_t>(cCurTime.GetTime());
                }

                D3DXVECTOR2 vecPos = (*vit)->GetCurrentWindowPos();
                vecPos.x += (*vit)->GetWidth();
                vecPos.x *= m_fWindowScaleX;
                vecPos.y *= m_fWindowScaleY;					
                g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
                g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pGCItem, -1, &kItem );
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

void KGCChoiceDrop::ActionPerformed( const KActionEvent& event )
{

    GCWND_MSG_MAP( m_pkGet,         KD3DWnd::D3DWE_BUTTON_CLICK,    OnGet );
    GCWND_MSG_MAP( m_pkGetAll,      KD3DWnd::D3DWE_BUTTON_CLICK,    OnGet );
    GCWND_MSG_MAP( m_pkGetLock,     KD3DWnd::D3DWE_CURSOR_ENTER,    OnMouseEnterConfirm );
    GCWND_MSG_MAP( m_pkGetLock,     KD3DWnd::D3DWE_CURSOR_LEAVE,    OnMouseLeaveConfirm );
    GCWND_MSG_MAP( m_pkGetAllLock,  KD3DWnd::D3DWE_CURSOR_ENTER,    OnMouseEnterConfirm );
    GCWND_MSG_MAP( m_pkGetAllLock,  KD3DWnd::D3DWE_CURSOR_LEAVE,    OnMouseLeaveConfirm );

    GCWND_MSG_MAP( m_pkGetAll,      KD3DWnd::D3DWE_BUTTON_CLICK,    OnGetAll );
    GCWND_MSG_MAP( m_pkClose,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
    GCWND_MSG_MAP( m_pkCancel,      KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );


    if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
    {
        bool bPageRefresh = true;
        int i = m_pkScroll->GetScrollPos() * m_iCols;
        for( std::vector< KGCUserItemWnd* >::iterator vit = m_vecItemWnd.begin();
            vit != m_vecItemWnd.end(); ++vit )		
        {
            if( *vit == event.m_pWnd )
            {
                if( i >= (int)m_vecItem.size() ) {
                    bPageRefresh = false;
                    continue;
                }

                if( m_vecItemSelected[i] ) {                //선택된걸 취소
                    m_vecItemSelected[i] = false;
                    m_nSelectItemCount--;
                }
                else if (  m_nSelectItemCount < m_nLimitItemCount ) {   //선택안되었고, 더 선택가능한상태
                    m_vecItemSelected[i] = true;
                    m_nLastSelection = i;
                    m_nSelectItemCount++;
                }
                else {      //선택안되었고, 다 선택된 상태, 마지막에 선택한걸 취소하고 선택한다
                    m_vecItemSelected[m_nLastSelection] = false;
                    m_nLastSelection = i;
                    m_vecItemSelected[i] = true;
                }
            }
            ++i;
        }
        if ( bPageRefresh )
        RefreshPage();
    }
    if( KD3DWnd::D3DWE_BUTTON_DOWN == event.m_dwCode ){
        g_KDSound.Play( "31" );
    }
}


void KGCChoiceDrop::OnMouseEnterConfirm(void)
{
    if( m_nLimitItemCount != m_nSelectItemCount )
    {
        //m_pkImgTooltipConfirmBG->InitState(true);
        //m_pkImgTooltipConfirmBG1->InitState(true);
        m_pkStaticTolltipConfirm->ToggleRender(true);
    }
}

void KGCChoiceDrop::OnMouseLeaveConfirm(void)
{
    //m_pkImgTooltipConfirmBG->InitState(false);
    //m_pkImgTooltipConfirmBG1->InitState(false);
    m_pkStaticTolltipConfirm->ToggleRender(false);
}


void KGCChoiceDrop::SetLimitSelCount(USHORT _limitCount)
{
    m_nLimitItemCount = _limitCount;
}

bool KGCChoiceDrop::CheckCondition( const KDialogInfo& kInfo_ ) 
{
    if ( m_nLimitItemCount < 1 )
            return false;

    if ( m_vecItem.size() == 0/*IsEmptyReceiveItem()*/ )
        return false;

    return true;
}

void KGCChoiceDrop::RefreshPage(void)
{   
    if ( m_nLimitItemCount == m_nSelectItemCount ) {    //선택 다 했음
        if ( 1 == m_nLimitItemCount ) {                 //선택 가능 개수 1개
        m_pkGet->ToggleRender(true);
        m_pkGetLock->ToggleRender(false);
        m_pkGetAll->ToggleRender(false);
        m_pkGetAllLock->ToggleRender(false);
        } else {                                        //선택 가능 개수 1개 이상
            m_pkGet->ToggleRender(false);
            m_pkGetLock->ToggleRender(false);
            m_pkGetAll->ToggleRender(true);
            m_pkGetAllLock->ToggleRender(false);
        }
    } else {                                            //선택 덜 했음 (Lock버튼만 활성)
        if ( 1 == m_nLimitItemCount ) {                 //선택 가능 개수 1개
            m_pkGet->ToggleRender(false);
            m_pkGetLock->ToggleRender(true);
            m_pkGetAll->ToggleRender(false);
            m_pkGetAllLock->ToggleRender(false);
        } else {                                        //선택 가능 개수 1개 이상
            m_pkGet->ToggleRender(false);
            m_pkGetLock->ToggleRender(false);
            m_pkGetAll->ToggleRender(false);
            m_pkGetAllLock->ToggleRender(true);
        }
    }


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




    std::vector< KDropItemInfo >::iterator vitData = m_vecItem.begin() + m_pkScroll->GetScrollPos() * m_iCols;
    std::vector< KGCUserItemWnd* >::iterator vitUI = m_vecItemWnd.begin();

    //빈 상자 셋팅
    while( vitUI != m_vecItemWnd.end() ) 
    {
        (*vitUI)->SetItem();
        vitUI++;
    }

    vitUI = m_vecItemWnd.begin();
    int i = m_pkScroll->GetScrollPos() * m_iCols;
    for( ; vitUI != m_vecItemWnd.end() && vitData < m_vecItem.end() ; ++vitData ) 
    {
        GCItem* pGCItem = NULL;
        pGCItem = g_pItemMgr->GetItemData((*vitData).m_ItemID / 10 );

        (*vitUI)->SetItem( pGCItem );
        (*vitUI)->SetItemDuration( (*vitData).m_nDuration == 0 ? -1 : (*vitData).m_nDuration );

        if( m_vecItemSelected[i] )
            (*vitUI)->SetSelect( true );
        else
            (*vitUI)->SetSelect( false );

        (*vitUI)->UpdateSelectIcon();

        vitUI++;
        i++;
    }



}

void KGCChoiceDrop::ResizeWindow( int _row, bool _scroll )
{
    //높이 설정
    int iAddHeight = (_row - 1) * m_ptimgItemBackgroundWH.y;    //더할 높이이므로 (_row - 1)

    m_pkGet->SetWindowPosY(m_vtGetPos.y + (float)iAddHeight);
    m_pkGetLock->SetWindowPosY(m_vtGetPos.y + (float)iAddHeight);
    m_pkGetAll->SetWindowPosY(m_vtGetPos.y + (float)iAddHeight);
    m_pkGetAllLock->SetWindowPosY(m_vtGetPos.y + (float)iAddHeight);


    m_pkCancel->SetWindowPosY(m_vtCancelPos.y + (float)iAddHeight);

    m_pkImgTooltipConfirmBG->SetWindowPosY(m_vtImgTooltipConfirmBGPos.y + (float)iAddHeight);
    m_pkImgTooltipConfirmBG1->SetWindowPosY(m_vtImgTooltipConfirmBGPos.y + (float)iAddHeight);
    m_pkStaticTolltipConfirm->SetWindowPosY(m_vtStaticTolltipConfirmPos.y + (float)iAddHeight);
   

    m_pkSboxMainBackground->SetHeight(m_ptSboxMainBackgroundWH.y + iAddHeight);
    m_pkSboxInnerBak->SetHeight(m_ptSboxInnerBakWH.y + iAddHeight);

    m_pkScroll->SetHeight(m_dwScrollHeight + iAddHeight);

    //가로 설정 
    DWORD dwTemp;
    _scroll ? dwTemp = 0 : dwTemp = 1;
    DWORD dwMinusWidth = dwTemp * m_pkScroll->GetWidth();
    DWORD dwMinusHalfWidth = (dwMinusWidth)/2; 
    

    m_pkGet->SetWindowPosX(m_vtGetPos.x - (float)dwMinusHalfWidth);
    m_pkGetLock->SetWindowPosX(m_vtGetPos.x - (float)dwMinusHalfWidth);
    m_pkGetAll->SetWindowPosX(m_vtGetPos.x - (float)dwMinusHalfWidth);
    m_pkGetAllLock->SetWindowPosX(m_vtGetPos.x - (float)dwMinusHalfWidth);


    m_pkGetAll->SetWindowPosX(m_vtGetPos.x - (float)dwMinusHalfWidth);
    m_pkCancel->SetWindowPosX(m_vtCancelPos.x - (float)dwMinusHalfWidth);

    m_pkImgTooltipConfirmBG->SetWindowPosX(m_vtImgTooltipConfirmBGPos.x - (float)dwMinusHalfWidth);
    m_pkImgTooltipConfirmBG1->SetWindowPosX(m_vtImgTooltipConfirmBGPos.x - (float)dwMinusHalfWidth);
    m_pkStaticTolltipConfirm->SetWindowPosX(m_vtStaticTolltipConfirmPos.x - (float)dwMinusHalfWidth);

    m_pkClose->SetWindowPosX(m_vtClosePos.x - (float)dwMinusWidth);
    m_pkSboxMainBackground->SetWidth(m_ptSboxMainBackgroundWH.x - dwMinusWidth);
    m_pkSboxInnerBak->SetWidth(m_ptSboxInnerBakWH.x - dwMinusWidth);


}



void KGCChoiceDrop::OnGet()
{
    std::set<USHORT> setSelectRewardIndex;

    for( size_t i = 0 ; i < m_vecItemSelected.size() ; i++ ){
        if( m_vecItemSelected[i] )
            setSelectRewardIndex.insert((unsigned short)i);
    }

    m_vecItemSelected.clear();


    KP2P::GetInstance()->Send_ChoiceBoxOpenReq( GetBoxID() * 10, setSelectRewardIndex );
    OnClose();
}

GCITEMID& KGCChoiceDrop::GetBoxID()
{
    return m_ItemID;
}

void KGCChoiceDrop::SetBoxID( GCITEMID& _itemID )
{
    m_ItemID = _itemID;
}

void KGCChoiceDrop::OnClose()
{
    ClearItemList();
    Destroy();
}

void KGCChoiceDrop::OnGetAll()
{
    m_nSelectItemCount = 0;
    ClearItemList();
    RefreshPage();
}


void KGCChoiceDrop::ReceiveItemBoxList( std::set<GCITEMID>& _itemBoxList )
{
    if( _itemBoxList.empty() == false )
        m_setItemBoxList = _itemBoxList;
}

bool KGCChoiceDrop::IsChoiceDropItem( int _itemID )
{
    std::set<GCITEMID>::iterator itor = m_setItemBoxList.find( _itemID * 10 );
    if( itor == m_setItemBoxList.end() )
        return false;
    
    return true;
}
