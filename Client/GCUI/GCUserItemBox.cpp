#include "stdafx.h"
//



//
//
//
#include "../MyD3D.h"

#include "GCUserItemBox.h"
#include "GCUserItemWnd.h"
//
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/KGCPetPreview.h"
#include "GCUI/GCShopTab.h"

#include "Procedure.h"
//

#include "GCStateGame.h"
#include "GCUI/KGCPetMessage.h"
//


#include "GCSKT.h"
#include "GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( KGCUserItemBox );
IMPLEMENT_WND_FACTORY( KGCUserItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCUserItemBox, "gc_user_item_box" );

KGCUserItemBox::KGCUserItemBox( void )
{
	m_pkScrollbar       = NULL;
    m_pkClipRange       = NULL;
	m_iScrollPos	    = 0;
    m_iScrollTarget     = 0;
    m_fScrolled         = 0.f;
    m_iTotalPage        = 0;
    m_iCurrentPage      = 0;
    m_bAnimScroll       = false;
    m_bSorting          = true;
    m_HoveredItemUID    = KItem::INVALID_ITEM;

    m_iRows = 4;
    m_iCols = 8;

    m_fXGap = 0.f;
    m_fYGap = 0.f;
    m_fXOffset = 0.f;
    m_fYOffset = 0.f;
    m_fScollGap = 10.f;
    m_iScrollFrame = 10;
    m_fLineHeight = 74.f;

	m_bShowBtnState = true;

    for( int i = 0; i < NUM_ITEM_VIEW_WND; ++i )
    {
        m_apItemWnd[i] = NULL;
        LINK_CONTROL_STM( "box_item" << i, m_apItemWnd[i] );
    }

    LINK_CONTROL( "scroll", m_pkScrollbar );
    LINK_CONTROL( "clip_range", m_pkClipRange );

    m_vecHotEquipParticle.clear();

}

KGCUserItemBox::~KGCUserItemBox( void )
{
    if ( g_ParticleManager ) 
    {
        g_ParticleManager->DeleteSequence( m_vecHotEquipParticle );
    }

    m_vecHotEquipParticle.clear();
}

void KGCUserItemBox::OnCreate( void )
{
    for ( UINT i = 0; i < NUM_ITEM_VIEW_WND; ++i )
    {
        if( m_apItemWnd[i] == NULL )
            continue;

        m_apItemWnd[i]->InitState( false );
    }

	m_bShowBtnState = true;

    if( m_pkScrollbar != NULL ) {
        m_pkScrollbar->SetSelfInputCheck(true);
        m_pkScrollbar->AddActionListener(this);
        m_pkScrollbar->SetScrollPos(0);
        m_pkScrollbar->SetScrollRangeMin(0);
        m_pkScrollbar->SetScrollRangeMax(0);
        m_pkScrollbar->SetScrollGap( 1 );
        m_pkScrollbar->SetScrollPageSize( 2 );
    }

    if( m_pkClipRange == NULL ) {
        m_pkClipRange = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "ClipRange", "others\\invisible" );
        m_pkClipRange->SetHeight( GetHeight() );
        m_pkClipRange->SetWidth( GetWidth() );
    }
    m_pkClipRange->InitState( true, true );
    m_pkClipRange->SetClipChild( true );

    CreateChildWndList();
}

void KGCUserItemBox::OnInitialize()
{
}

void KGCUserItemBox::OnDestroy()
{
}



void KGCUserItemBox::FrameMoveInEnabledState( void )
{
	if( !o10 )
	{
		return;
	}	

    if( m_iScrollTarget < m_iScrollPos ) {
        m_fScrolled += m_fScollGap;
    }
    else if( m_iScrollTarget > m_iScrollPos ) {
        m_fScrolled -= m_fScollGap;
    }

    if( m_fScrolled > 0.f ) {
        --m_iScrollPos;
        if( m_iScrollTarget < m_iScrollPos ) {
            m_fScrolled = -1 * m_fLineHeight;
            RefreshItemWndCustom(m_iScrollPos - 1);
        }
        else {
            m_fScrolled = 0.f;
            RefreshItemWnd();
            RefreshWndPosition();
        }
    }
    else if( m_fScrolled < -1 * m_fLineHeight ) {
        m_fScrolled = 0.f;
        ++m_iScrollPos;
        RefreshItemWnd();
        RefreshWndPosition();
    }

    if( m_fScrolled != 0.f ) {
        RefreshWndPosition();
    }
}

void KGCUserItemBox::ActionPerformed( const KActionEvent& event )
{
    if( m_pkScrollbar == NULL )
        return;

	if ( event.m_pWnd == m_pkScrollbar )
	{
		int iPos = m_pkScrollbar->GetScrollPos();
		
		if ( m_iScrollPos != iPos )
		{
			g_KDSound.Play( "73" );
			OnScrollPos(iPos);
		}
		return;
	}
}

void KGCUserItemBox::RefreshScrollMax()
{
	//Create에서 부르니깐... 초반에 여러번 들어온다..ㅡ.ㅡ;
	//m_ItemCategory = eCategory;
	m_iTotalPage = ( m_vecItemList.size() + GetCols() - 1 ) / GetCols();
    if( m_iTotalPage > GetRows() )
        m_iTotalPage -= GetRows() - 2;
    else
        m_iTotalPage = 1;

    if( m_pkScrollbar != NULL ) {
        m_pkScrollbar->SetScrollRangeMax(m_iTotalPage);
        OnScrollPos(m_pkScrollbar->GetScrollPos());
    }
    m_iScrollPos = m_iScrollTarget;
    RefreshItemWndCustom( m_iScrollPos );
}

void KGCUserItemBox::OnScrollPos(int iPos)
{
    if( iPos >= m_iTotalPage )
        iPos = m_iTotalPage - GetRows();
    else if( iPos < 0 ) {
        iPos = 0;
    }

    m_iCurrentPage = iPos;
    if( m_iScrollTarget == m_iScrollPos && m_iScrollPos > iPos ) {
        //RefreshItemWndCustom( m_iScrollPos - 1 );
        m_fScrolled = -74.f;
    }

    m_iScrollTarget = iPos;

    if( m_bAnimScroll == false ) {
        m_iScrollPos = m_iScrollTarget;
        m_fScrolled = 0.f;
        RefreshItemWnd();
    }
}

void KGCUserItemBox::RefreshItemWndCustom( int iScrollPos_ )
{
    if( iScrollPos_ < 0 )
        iScrollPos_ = 0;

    int iStartIndex = iScrollPos_ * GetCols();
    int iSkipCnt = 0;

    m_mapWndItemInfo.clear();
    int iItemIndex = 0;
    for(int i=0;i< static_cast<int>( m_vecItemWnd.size() );++i)
    {
        const int iCurItemIndex = iStartIndex + iItemIndex;
        KD3DWnd* pWnd = m_vecItemWnd[i];

        if( pWnd->IsRenderOn() == false )
            continue;

        if( (iCurItemIndex) >= (int)m_vecItemList.size() )
        {
            m_vecItemWnd[i]->SetItem();
            
#if defined( USE_ONGAME_INVENTORY )
            m_vecItemWnd[i]->OnSetItemState();
#endif
            continue;
        }

        KItem& pInvenItem = m_vecItemList[iCurItemIndex];
        m_mapWndItemInfo[pWnd->GetUID()] = pInvenItem.m_ItemUID;
        m_vecItemWnd[i]->SetItem( &pInvenItem );
		if ( m_bShowBtnState == false )
			m_vecItemWnd[i]->OffItemBtn( );
        
#if defined( USE_ONGAME_INVENTORY ) 
        if ( iCurItemIndex < static_cast<int>(m_vecItemState.size())  ) 
        {
            m_vecItemWnd[i]->OnSetItemState( m_vecItemState[ iCurItemIndex ].first, m_vecItemState[ iCurItemIndex ].second );
        }
        else
        {
            m_vecItemWnd[i]->OnSetItemState( false, false );
        }
#endif

        pWnd->SetHoverPrev( false );
        ++iItemIndex;
    }
}

void KGCUserItemBox::RefreshItemWnd()
{
    RefreshItemWndCustom( m_iScrollPos );
}

bool KGCUserItemBox::GetCurrentHoveredItem( GCItem*& pItem, KItem*& pkInventoryItem, D3DXVECTOR2 &vecPos )
{
    if( m_HoveredItemUID == KItem::INVALID_ITEM )
        return false;

    POINT   pt = g_pkInput->GetMousePos();
    if( false == CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) ) )
        return false;

    pItem = m_kHoveredItemInfo;
    pkInventoryItem = &m_kHoveredInvenItem;
    vecPos = m_vHoveredItemPos;
    
    return true;
}

void KGCUserItemBox::UpdateData()
{
    RefreshItemWnd();

#if defined( USE_ONGAME_INVENTORY )
    SetItemState();
#endif
}

bool KGCUserItemBox::OnEnterCursor( const KActionEvent& kEvent_ )
{
    g_KDSound.Play( "30" );

    if( kEvent_.m_pWnd == NULL )
        return true;

    KGCUserItemWndInterface* pWnd = static_cast<KGCUserItemWndInterface*>(kEvent_.m_pWnd);
    m_HoveredItemUID = pWnd->GetInvenItem().m_ItemUID;
    m_kHoveredInvenItem = pWnd->GetInvenItem();
    m_kHoveredItemInfo = pWnd->GetItemData();

    m_vHoveredItemPos = kEvent_.m_pWnd->GetCurrentWindowPos();
    m_vHoveredItemPos.x += ( kEvent_.m_pWnd->GetWidth() );
    m_vHoveredItemPos.y += ( kEvent_.m_pWnd->GetHeight() / 2.0f );
    m_vHoveredItemPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
    m_vHoveredItemPos.y *= g_pGCDeviceManager2->GetWindowScaleY();

    if( m_vHoveredItemPos.x > static_cast<float>(GC_SCREEN_WIDTH/2) )
    {
        m_vHoveredItemPos.x -= ( static_cast<float>( kEvent_.m_pWnd->GetWidth() ) * m_fWindowScaleX );
        m_vHoveredItemPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth();
    }

    return true;
}

void KGCUserItemBox::SetRows( int val )
{
    m_iRows = val;
    CreateChildWndList();
}

void KGCUserItemBox::SetCols( int val )
{
    m_iCols = val;
    CreateChildWndList();
}

void KGCUserItemBox::SetXGap( float val )
{
    m_fXGap = val;
    CalcuateWndPosition();
}

void KGCUserItemBox::SetYGap( float val )
{
    m_fYGap = val;
    CalcuateWndPosition();
}

void KGCUserItemBox::SetXOffset( float val )
{
    m_fXOffset = val;
    CalcuateWndPosition();
}

void KGCUserItemBox::SetYOffset( float val )
{
    m_fYOffset = val;
    CalcuateWndPosition();
}

KGCUserItemWndInterface* KGCUserItemBox::CreateOneChildWnd()
{
    KD3DWnd* pWnd = g_pkUIMgr->CreateTemplateUIByFindName( m_pkClipRange, GetChildFile().c_str(), "", GetChildPath().c_str(), true );
    if( pWnd == NULL ) {
        ASSERT( pWnd );
        return NULL;
    }
    pWnd->InitState( true, true, this );
    return static_cast<KGCUserItemWndInterface*>(pWnd);
}

void KGCUserItemBox::CreateChildWndList()
{
    ASSERT( !GetChildFile().empty() );
    ASSERT( !GetChildPath().empty() );

    //만들어야 하는 Wnd의 갯수
    int iCreationNum = ( GetRows() + 1 ) * GetCols();

    //현재 만들어 진 Wnd 갯수와의 차이
    int iDelta = iCreationNum - m_vecItemWnd.size();

	if( iDelta == 0 ) {
		for( int i = 0 ; i < static_cast<int>(m_vecItemWnd.size() ) ; ++i ) {
			m_vecItemWnd[i]->InitState( true, true, this );
		}
	}
	else if( iDelta < 0 ) {
		for( DWORD i = m_vecItemWnd.size() + iDelta ; i < m_vecItemWnd.size() ; ++i ) {
			m_vecItemWnd[i]->InitState( false );
		}
	}
	else
	{
		std::vector<KD3DWnd*> vecCreatedList = g_pkUIMgr->CreateTemplateUIListByFindName( m_pkClipRange, GetChildFile().c_str(), "", GetChildPath().c_str(), true, iDelta );
		std::vector<KD3DWnd*>::iterator vit;
		for( vit = vecCreatedList.begin() ; vit != vecCreatedList.end() ; ++vit ) {
			(*vit)->InitState( true, true, this );
			m_vecItemWnd.push_back( static_cast<KGCUserItemWndInterface*>(*vit) );
			m_vecOriginPos.push_back( (*vit)->GetFixedWindowLocalPos() );
		}
	}

	std::vector<KGCUserItemWndInterface*>::iterator vitItemWnd = m_vecItemWnd.begin();
	for ( ;vitItemWnd != m_vecItemWnd.end() ; ++vitItemWnd )
	{
		(*vitItemWnd)->DisconnectAllEventProcedure();
		(*vitItemWnd)->ConnectEventProcedure( D3DWE_CURSOR_ENTER, this, &KGCUserItemBox::OnEnterCursor );
		(*vitItemWnd)->ConnectEventProcedure( D3DWE_CURSOR_LEAVE, this, &KGCUserItemBox::OnLeaveCursor );
		(*vitItemWnd)->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCUserItemBox::OnClickItem );
		(*vitItemWnd)->ConnectEventProcedure( D3DWE_RBUTTON_CLICK, this, &KGCUserItemBox::OnRClickItem );
	}

    CalcuateWndPosition();
    RefreshItemWnd();

}

void KGCUserItemBox::CalcuateWndPosition()
{
    if( m_pkClipRange == NULL ) {
        m_pkClipRange = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "ClipRange", "others\\invisible" );
        m_pkClipRange->SetHeight( GetHeight() );
        m_pkClipRange->SetWidth( GetWidth() );
    }

    D3DXVECTOR2 vStartPos = D3DXVECTOR2( GetXOffset(), GetYOffset() );

    for( int row = 0 ; row < GetRows() + 1 ; ++row ) {
        for( int col = 0 ; col < GetCols() ; ++col ) {
            int iIndex = row * GetCols() + col;

            if( iIndex >= static_cast<int>( m_vecItemWnd.size() ) )
                continue;

            DWORD dwWidth = m_vecItemWnd[iIndex]->GetWidth();
            DWORD dwHeight = m_vecItemWnd[iIndex]->GetHeight();

            if( iIndex == 0 ) {
                m_fLineHeight = static_cast<float>( dwHeight );
                m_fScollGap = static_cast<float>( dwHeight ) / static_cast<float>( m_iScrollFrame );
            }

            D3DXVECTOR2 vPos;
            vPos.x = ( dwWidth + GetXGap() ) * col;
            vPos.y = ( dwHeight + GetYGap() ) * row;
            m_vecOriginPos[iIndex] = vStartPos + vPos;
            m_vecItemWnd[iIndex]->InitState( true, true );
        }
    }

    if( m_vecItemWnd.empty() == false ) {
        float fWidth = m_vecItemWnd[0]->GetWidth() + GetXGap() + 1.f;
        float fHeight = m_vecItemWnd[0]->GetHeight() + GetYGap();

        m_pkClipRange->SetWidth( static_cast<DWORD>(fWidth * GetCols()) );
        m_pkClipRange->SetHeight( static_cast<DWORD>(fHeight * GetRows()) );
    }
    RefreshWndPosition();
}

void KGCUserItemBox::RefreshItemList( const std::vector<GCITEMUID>& vecItemList_ , bool bShowBtn )
{
	m_bShowBtnState = bShowBtn;

    m_vecItemList.clear();
    for( DWORD i = 0; i < vecItemList_.size() ; ++i ) {
        KItem* pkItem = g_pItemMgr->GetInventoryItem( vecItemList_[i] );
        CONTINUE_NIL( pkItem );

        m_vecItemList.push_back( *pkItem );
    }

#if defined( USE_ONGAME_INVENTORY ) 
    SetItemState();
#endif

    RefreshScrollMax();
    m_fScrolled = 0.f;
    m_HoveredItemUID = KItem::INVALID_ITEM;
}

void KGCUserItemBox::RefreshItemList( const std::vector<KItem>& vecItemList_ , bool bShowBtn  )
{
	m_bShowBtnState = bShowBtn;

    m_vecItemList = vecItemList_;

#if defined( USE_ONGAME_INVENTORY ) 
    SetItemState();
#endif

    RefreshScrollMax();
    m_fScrolled = 0.f;
    m_HoveredItemUID = KItem::INVALID_ITEM;
}

bool KGCUserItemBox::OnClickItem( const KActionEvent& kEvent_ )
{
    wnditeminfo_type::iterator mit = m_mapWndItemInfo.find( kEvent_.m_pWnd->GetUID() );
    if( mit == m_mapWndItemInfo.end() )
        return true;

    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_CLICK_INVEN_ITEM, mit->second ) );
    OnEnterCursor(kEvent_);
    return true;
}

bool KGCUserItemBox::OnRClickItem( const KActionEvent& kEvent_ )
{
    wnditeminfo_type::iterator mit = m_mapWndItemInfo.find( kEvent_.m_pWnd->GetUID() );
    if( mit == m_mapWndItemInfo.end() )
        return true;


    SpeakToActionListener( KActionEvent( this, KGCUIScene::D3DWE_RCLICK_INVEN_ITEM, mit->second ) );
    return true;
}

void KGCUserItemBox::SetWndType( int iType_ )
{
    if( iType_ >= NUM_ITEM_VIEW_WND )
        return;
    if( m_apItemWnd[iType_] == NULL )
        return;

    SetChildFile( m_apItemWnd[iType_]->GetFileName() );
    SetChildPath( m_apItemWnd[iType_]->GetPath() );
    m_vecItemWnd.clear();
    m_vecOriginPos.clear();
    m_pkClipRange->RemoveAllChild();
    CreateChildWndList();
}

void KGCUserItemBox::SetChildFile( std::string val )
{
    m_strChildFile = val;
}

void KGCUserItemBox::SetChildPath( std::string val )
{
    m_strChildPath = val;
}

std::string KGCUserItemBox::GetChildFile()
{
    if( m_strChildFile.empty() ) {
        for ( UINT i = 0; i < NUM_ITEM_VIEW_WND; ++i )
        {
            if( m_apItemWnd[i] == NULL )
                continue;

            SetChildFile( m_apItemWnd[i]->GetFileName() );
            break;
        }
    }
    return m_strChildFile;
}

std::string KGCUserItemBox::GetChildPath()
{
    if( m_strChildPath.empty() ) {
        for ( UINT i = 0; i < NUM_ITEM_VIEW_WND; ++i )
        {
            if( m_apItemWnd[i] == NULL )
                continue;

            SetChildPath( m_apItemWnd[i]->GetPath() );
            break;
        }
    }
    return m_strChildPath;
}

void KGCUserItemBox::AddItemList( GCITEMUID itemUID_ )
{
    KItem* pkItem = g_pItemMgr->GetInventoryItem( itemUID_ );
    RETURN_NIL( pkItem );

    for( DWORD i = 0 ; i < m_vecItemList.size() ; ++i ) {
        if( m_vecItemList[i].m_ItemUID == itemUID_ ){
            m_vecItemList.erase( m_vecItemList.begin() + i );
            break;
        }
    }
    m_vecItemList.push_back( *pkItem );
    //if( m_bSorting )
    //    std::sort( m_vecItemList.begin(), m_vecItemList.end(), SSortbyUID() );

    RefreshScrollMax();
}

void KGCUserItemBox::AddItemList( const KItem& kItem_ )
{
    for( DWORD i = 0 ; i < m_vecItemList.size() ; ++i ) {
        if( m_vecItemList[i].m_ItemUID == kItem_.m_ItemUID ){
            m_vecItemList.erase( m_vecItemList.begin() + i );
            break;
        }
    }
    m_vecItemList.push_back( kItem_ );
    //if( m_bSorting )
    //    std::sort( m_vecItemList.begin(), m_vecItemList.end(), SSortbyUID() );

    RefreshScrollMax();
}

void KGCUserItemBox::RemoveItemList( GCITEMUID itemUID_ )
{
    for( DWORD i = 0 ; i < m_vecItemList.size() ; ++i ) {
        if( m_vecItemList[i].m_ItemUID == itemUID_ ){
            m_vecItemList.erase( m_vecItemList.begin() + i );
            break;
        }
    }
    //if( m_bSorting )
    //    std::sort( m_vecItemList.begin(), m_vecItemList.end(), SSortbyUID() );
    RefreshScrollMax();
}

void KGCUserItemBox::ClearItemList()
{
    m_vecItemList.clear();
    RefreshScrollMax();
}

bool KGCUserItemBox::OnLeaveCursor( const KActionEvent& kEvent_ )
{
    if( kEvent_.m_pWnd == NULL )
        return true;

    wnditeminfo_type::iterator mit = m_mapWndItemInfo.find( kEvent_.m_pWnd->GetUID() );
    if( mit == m_mapWndItemInfo.end() )
        return true;

	if( m_HoveredItemUID != mit->second ){
        return true;
	}
    m_HoveredItemUID = KItem::INVALID_ITEM;
    return true;
}

void KGCUserItemBox::RefreshWndPosition()
{
    for( int i = 0 ; i < static_cast<int>(m_vecItemWnd.size()) ; ++i ) {
        if( m_vecItemWnd[i]->IsRenderOn() == false )
            continue;
		// 내부적으로만 쓰는거 빼기 싫어서 이렇게 만듬.
		// 첫번째 박스의 위치가 이전 위치랑 같으면 갱신이 안되기에 현재 설정하는 위치와
		// 틀리게 설정하여 무조건 갱신되도록 한다.
		m_vecItemWnd[i]->SetWindowPosDirect( m_vecOriginPos[i] + D3DXVECTOR2( 0.f, m_fScrolled ) );
    }

}

#if defined( USE_ONGAME_INVENTORY )
void KGCUserItemBox::SetItemState( void )
{
    m_vecItemState.clear();
    if( m_vecItemList.empty() ) { 
        return;
    }

    // Hot 아이템 리스트 만들고 
    std::map< int, GCITEMUID > mapHotItem;    // < 장착위치, < 능력치, UID > >
    for( std::vector< KItem >::iterator it = m_vecItemList.begin() ; it != m_vecItemList.end() ; ++it ) { 
        int iSlotPos = -1;
        int iAbility = -1;

        if( it->m_ItemUID <= KGCItemManager::MAXIMUM_PCBANG_UID ) {     // 피방 아이템 걸르고 
            continue;
        }

        if( it->CheckItemType( KItem::TYPE_LOOK ) ) {   // 코디 거르고 
            continue;
        }
        
        if( false == g_pItemMgr->GetItemUniqueSlotPosAndAbility( it->m_ItemUID, g_MyD3D->m_TempPlayer.GetCurrentChar(), iSlotPos, iAbility ) ) {
            continue;
        }

        std::map< int, GCITEMUID >::iterator mit = mapHotItem.find( iSlotPos );
        if( mit == mapHotItem.end() ) { 
            mapHotItem[ iSlotPos ] = it->m_ItemUID;
            continue;
        }
        else {
            int iSavedSlotPos = -1;
            int iSavedAbility = -1;

            g_pItemMgr->GetItemUniqueSlotPosAndAbility( mit->second, g_MyD3D->m_TempPlayer.GetCurrentChar(), iSavedSlotPos, iSavedAbility );
            if( iAbility > iSavedAbility ) { 
                mapHotItem.erase( mit );
                mapHotItem[ iSlotPos ] = it->m_ItemUID;
                continue;
            }
        }
    }

    std::set< GCITEMUID > setHotItemUID;
    for( std::map< int, GCITEMUID >::iterator mit = mapHotItem.begin() ; mit != mapHotItem.end() ; ++mit ) { 
        if( g_pItemMgr->IsHotItem( mit->second, &g_MyD3D->m_TempPlayer ) ) { 
            setHotItemUID.insert( mit->second );
        }
    }

    // 정보 세팅 
    for( int i=0 ; i<static_cast<int>( m_vecItemList.size() ) ; ++i ) { 
        std::set< GCITEMUID >::iterator sit = setHotItemUID.find( m_vecItemList[i].m_ItemUID );
        if( sit == setHotItemUID.end() ) { 
            if( true == g_pItemMgr->IsNewItem( m_vecItemList[i].m_ItemUID ) ) { 
                if( true == g_pItemMgr->CheckEquipLevel( g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel, m_vecItemList[i].m_ItemUID ) ) { 
                    m_vecItemState.push_back( std::make_pair( false, true ) );
                }
                else {
                    m_vecItemState.push_back( std::make_pair( false, false ) );
                }
            }
            else { 
                m_vecItemState.push_back( std::make_pair( false, false ) );
            }
            continue;
        }

        m_vecItemState.push_back( std::make_pair( true, false ) );
    }

    // Hot>New>일반 순으로 정렬 시키기 
    SortItemListByItemState();
}

void KGCUserItemBox::SortItemListByItemState( void )
{
    if( m_vecItemList.size() != m_vecItemState.size() ) { 
        return;
    }

    std::vector< KItem > vecHotItem;
    std::vector< KItem > vecNewItem;
    std::vector< KItem > vecNormalItem;

    for( int i=0 ; i<static_cast<int>( m_vecItemState.size() ) ; ++i ) { 
        if( true == m_vecItemState[i].first ) {     // hot
            vecHotItem.push_back( m_vecItemList[ i ] );
            continue;
        }

        if( true == m_vecItemState[i].second ) {    // new
            vecNewItem.push_back( m_vecItemList[ i ] );
            continue;
        }

        vecNormalItem.push_back( m_vecItemList[ i ] );  // 아무것도 아님 
    }

    m_vecItemList.clear();
    m_vecItemState.clear();

    MakeItemListAndStateList( vecHotItem, true, false );
    MakeItemListAndStateList( vecNewItem, false, true );
    MakeItemListAndStateList( vecNormalItem, false, false );
}

void KGCUserItemBox::MakeItemListAndStateList( IN const std::vector< KItem >& vecItem, bool bHot_, bool bNew_ )
{
    for( std::vector< KItem >::const_iterator it = vecItem.begin() ; it != vecItem.end() ; ++it ) { 
        m_vecItemList.push_back( *it );
        m_vecItemState.push_back( std::make_pair( bHot_, bNew_ ) );
    }
}
#endif

void KGCUserItemBox::ShowItemEffect( bool _bShow )
{
#if defined( USE_ONGAME_INVENTORY ) 

    std::string strParticle = "UI_NewEquip_03";

    if ( _bShow )
    {
        g_ParticleManager->DeleteSequence( m_vecHotEquipParticle );
        m_vecHotEquipParticle.clear();

        for(int i=0;i< static_cast<int>( m_vecItemWnd.size() );++i)
        {
            if ( m_vecItemWnd[i]->IsHotItem() )
            {
                D3DXVECTOR2 vPos = m_vecItemWnd[i]->GetFixedWindowPos();

                vPos.x += ( m_vecItemWnd[i]->GetWidth() / 2 ) + 1;
                vPos.y += ( m_vecItemWnd[i]->GetHeight() / 2 ) + 1;

                vPos.x = (vPos.x)/ (400.0f * GC_SCREEN_DIV_WIDTH) - 1.002f;
                vPos.y = (-vPos.y)/ (400.0f * GC_SCREEN_DIV_WIDTH) + 0.752f;

                CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( strParticle, vPos.x, vPos.y, 0.5f );
                g_ParticleManager->SetLayer ( pParticle, GC_LAYER_UI );
                m_vecHotEquipParticle.push_back( pParticle );
            }
        }
    }
    else
    {
        g_ParticleManager->DeleteSequence( m_vecHotEquipParticle );
        m_vecHotEquipParticle.clear();
    }
#endif
}

UINT KGCUserItemBox::GetEnabledStateWndNum()
{
	return GetRows()*GetCols();
}

bool KGCUserItemBox::MakeItemOnFirstSlot( IN GCITEMID dwItemID )
{
    std::vector<KItem>::iterator iter = m_vecItemList.begin();

    // 처음 위치에 있다면 패스~
    if( (*iter).m_ItemID == dwItemID ){
        return true;
    }

    // 아니라면 위치 조정
    for( ; iter < m_vecItemList.end(); ++iter ){
        if( iter != m_vecItemList.end() ){
            if( (*iter).m_ItemID == dwItemID ){
                std::iter_swap( m_vecItemList.begin(), iter );
                UpdateData();
                return true;
            }
        } else {
            return false;
        }
    }

    return false;
}
