#include "stdafx.h"
#include "GCItemSocketManagementDlg.h"
#include "GCMonsterCardBook.h"
#include "GCItemImgWnd.h"
#include "GCItemManager.h"
#include "GCSocketIcon.h"


IMPLEMENT_CLASSNAME( KGCItemSocketManagementDlg );
IMPLEMENT_WND_FACTORY( KGCItemSocketManagementDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCItemSocketManagementDlg, "gc_item_socket_management_dlg" );

KGCItemSocketManagementDlg::KGCItemSocketManagementDlg( void )
: m_pkItemImg( NULL )
, m_pkDescription( NULL )
, m_iSelectedIndex( EISI_UNKNOWN )
, m_ItemUID( 0 )
, m_eSelectedTab( KSocketInfo::STS_CLOSED )
, m_pkCloseBtn( NULL )
, m_SelectedMonsterCardID( 0 )
, m_pkScrollBar( NULL )
, m_UseCashItemID ( 0 )
, m_eUseMaterialKind( UMK_CASHITEM )
, m_pkTextNeed(NULL)
, m_pkTextOwn(NULL)
, m_pkNeedCount(NULL)
, m_pkOwnCount(NULL)
{
    LINK_CONTROL( "socket_item_img",        m_pkItemImg );
    LINK_CONTROL( "static_description",     m_pkDescription );
    LINK_CONTROL( "close_btn",              m_pkCloseBtn );
    LINK_CONTROL( "scroll_bar",              m_pkScrollBar );

    LINK_CONTROL( "static_text_need",       m_pkTextNeed );
    LINK_CONTROL( "static_text_own",        m_pkTextOwn );
    LINK_CONTROL( "static_need_count",      m_pkNeedCount );
    LINK_CONTROL( "static_own_count",       m_pkOwnCount );

    for( int i= 0 ; i < KSocketInfo::STS_NUM; ++i)
    {
        char strTemp[ MAX_PATH ] = "";

        m_pkSocketManageMentTab[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "socket_management_tab%d", i );
        LINK_CONTROL( strTemp, m_pkSocketManageMentTab[i] );

        m_pkActionBtn[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "action_btn%d", i );
        LINK_CONTROL( strTemp, m_pkActionBtn[i] );

        m_pkSocketManageMentSelectedTab[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "socket_management_selected_tab%d", i );
        LINK_CONTROL( strTemp, m_pkSocketManageMentSelectedTab[i] );

        m_iMaterialUnit[i] = 0;
    }

    for( int i = 0; i < EISI_MAX; ++i )
    {
        char strTemp[ MAX_PATH ] = "";

        m_pkSocketActiveEdge[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "socket_active_edge%d", i );
        LINK_CONTROL( strTemp, m_pkSocketActiveEdge[i] );

        m_pkMonsterCardImg[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "monster_card_img%d", i );
        LINK_CONTROL( strTemp, m_pkMonsterCardImg[i] );

        m_pkSocketBtn[i] = NULL;
            sprintf_s( strTemp, MAX_PATH, "socket_btn%d", i );
            LINK_CONTROL( strTemp, m_pkSocketBtn[i] );
    }
}

KGCItemSocketManagementDlg::~KGCItemSocketManagementDlg( void )
{
}

void KGCItemSocketManagementDlg::OnCreate( void )
{
    for( int i= 0 ; i < KSocketInfo::STS_NUM; ++i)
    {
        m_pkSocketManageMentTab[i]->InitState( true, true, this );
        m_pkActionBtn[i]->InitState( false, true, this );
        m_pkActionBtn[i]->Lock( true );
        m_pkSocketManageMentSelectedTab[i]->ToggleRender(false);
    }

    for( int i = 0 ; i < EISI_MAX; ++i )
    {
        m_pkMonsterCardImg[i]->InitState( false, true, this );
        m_pkMonsterCardImg[i]->SetLikeBtn( true );
        m_pkSocketBtn[i]->InitState( false, true, this );
    }

    m_pkCloseBtn->InitState( true, true, this );

    //깨진 소켓을 살리는 아이템은 추후에 추가될 예정!! 이거만 지우면 동작한다 ㅋㅋ
    m_pkSocketManageMentTab[KSocketInfo::STS_INIT]->ToggleRender( false );
    m_pkSocketManageMentTab[KSocketInfo::STS_BREAK]->ToggleRender( false );

#if defined(NATION_THAILAND) || defined(NATION_IDN) || defined(NATION_KOREA) || defined(NATION_LATINAMERICA) || defined(NATION_CHINA)
	//넌 기능 없다면서 왜 있는거냐 - 소켓 봉인 해제
	m_pkSocketManageMentTab[KSocketInfo::STS_CLOSED]->ToggleRender( false );
#endif

    m_pkScrollBar->InitState( false, true, this );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollPageSize( EISI_MAX / EISI_ONE_LINE_COUNT );

	m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    m_pkTextNeed->SetAlign(DT_LEFT);
    m_pkTextOwn->SetAlign(DT_LEFT);
    m_pkNeedCount->SetAlign(DT_RIGHT);
    m_pkOwnCount->SetAlign(DT_RIGHT);
}

void KGCItemSocketManagementDlg::OnCreateComplete( void )
{
	InitTabInfo( KSocketInfo::STS_OPENED );
}

void KGCItemSocketManagementDlg::InitTabInfo( KSocketInfo::SOCKET_STATE eSelectTab_ )
{
    m_pkActionBtn[ m_eSelectedTab ]->Lock( true );
    m_pkActionBtn[ m_eSelectedTab ]->SetWndMode( D3DWM_LOCK );
    m_pkActionBtn[ m_eSelectedTab ]->ToggleRender( false );
    m_pkSocketManageMentTab[ m_eSelectedTab]->Lock( false );
    m_pkSocketManageMentSelectedTab[ m_eSelectedTab]->ToggleRender( false );
    m_eSelectedTab = eSelectTab_;

    SetSelectedItem();

    //나중에 체크박스가 생길 것이오.
    if( KSocketInfo::STS_CLOSED == eSelectTab_ )
        m_eUseMaterialKind = UMK_CASHITEM;
    else if( KSocketInfo::STS_OPENED == eSelectTab_ )
        m_eUseMaterialKind = UMK_GAMEPOINT;
    else if( KSocketInfo::STS_USING == eSelectTab_ )
        m_eUseMaterialKind = UMK_CASHITEM;
    else
        m_eUseMaterialKind = UMK_CASHITEM;

    m_pkActionBtn[ m_eSelectedTab ]->ToggleRender( true );
    m_pkSocketManageMentTab[ m_eSelectedTab]->Lock( true );
    m_pkSocketManageMentSelectedTab[m_eSelectedTab]->ToggleRender( true );

    m_SelectedMonsterCardID = 0;
    m_UseCashItemID = 0;
    m_iSelectedIndex = EISI_UNKNOWN;
    RefreshSocketActiveEdge();
    SetCondition();
}

void KGCItemSocketManagementDlg::PostChildDraw( void )
{

}

void KGCItemSocketManagementDlg::ActionPerformed( const KActionEvent& event_)
{
    for( int i = 0 ; i < EISI_MAX; ++i )
    {
        if( ( m_pkMonsterCardImg[i] == event_.m_pWnd || m_pkSocketBtn[i] == event_.m_pWnd )
            && m_pkSocketBtn[i]->GetSocketState() == m_eSelectedTab ) 
        {
            int iTopIndex = GetScrollTopIndex();
            if( KSocketInfo::STS_OPENED == m_eSelectedTab )
            {
                GCItem* pItem = NULL;
                DWORD dwSlotPosition = 0;
                if( g_pItemMgr->FindInventory( m_ItemUID, &pItem ) && pItem )
                    dwSlotPosition = pItem->dwSlotPosition;

                if( D3DWE_BUTTON_CLICK == event_.m_dwCode )
                {
                    g_pkUIScene->m_pkMonsterCardBook->SetClickedSlotFilterBtn( dwSlotPosition );
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MONSTER_CARD_BOOK, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0 ,false, true );
                    g_pkUIScene->m_pkMonsterCardBook->SetReceiveActionListener( this );
                    g_pkUIScene->m_pkMonsterCardBook->SetActionEventTransfer( m_pkSocketBtn[i] );
                    return;
                }
                else if( KGCMonsterCardBook::CLICK_MONSTER_CARD_SIMPLE_INFO == event_.m_dwCode )
                {
                    g_pkUIScene->m_pkMonsterCardBook->Destroy();
                    
                    GCITEMID ItemMonsterCard = static_cast< GCITEMID >(event_.m_dwlParam );
                    GCItem* pMonsterCardItem = g_pItemMgr->GetItemData( ItemMonsterCard );

                    if( NULL == pMonsterCardItem || 0 == (pMonsterCardItem->dwSlotPosition & dwSlotPosition) )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ITEM_CANT_EQUIP_MONSTER_CARD), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true );
                        return;
                    }
                    m_SelectedMonsterCardID = ItemMonsterCard;

                    int iSelectIndex = m_iSelectedIndex - iTopIndex;
                    if( IsEnableSocketIndex( iSelectIndex ) )
                        m_pkMonsterCardImg[iSelectIndex]->ToggleRender( false );

                    m_iSelectedIndex = i + iTopIndex;

                    RefreshSelectedMonsterCard();
                    RefreshSocketActiveEdge();
                    SetCondition();
                    
                    return;
                }
            }
            else
            {
                if( D3DWE_BUTTON_CLICK == event_.m_dwCode )
                {
                    m_iSelectedIndex = i + iTopIndex;
                    RefreshSocketActiveEdge();
                    SetCondition();
                    return;
                }
            }
        }
    }

    if( event_.m_pWnd == m_pkActionBtn[ m_eSelectedTab ] && D3DWE_BUTTON_CLICK == event_.m_dwCode )
    {
        OnClickActionButton( m_eSelectedTab );
        return;
    }

    for( int i = 0 ; i < KSocketInfo::STS_NUM; ++i)
    {
        if( event_.m_pWnd == m_pkSocketManageMentTab[i] && D3DWE_BUTTON_CLICK == event_.m_dwCode )
        {
            InitTabInfo( static_cast<KSocketInfo::SOCKET_STATE>(i) );
            return;
        }
    }

    if( event_.m_pWnd == m_pkCloseBtn && D3DWE_BUTTON_CLICK == event_.m_dwCode )
    {
        OnClose();
        return;
    }

    if( event_.m_pWnd == m_pkScrollBar )
    {
        OnScroll();
        return;
    }
    
}

void KGCItemSocketManagementDlg::SetSelectedItem()
{
    for( int i = 0 ; i < EISI_MAX; ++i)
    {
        m_pkMonsterCardImg[i]->ToggleRender( false );
        m_pkSocketBtn[i]->ToggleRender( false );
    }

    m_pkScrollBar->SetScrollPos( 0 );
    m_pkScrollBar->ToggleRender( false );

    if( 0 == m_ItemUID )
        return;

    GCItem* pItem = NULL;
    KItem* pInventoryItem = NULL;

    if( false == g_pItemMgr->FindInventory( m_ItemUID, &pInventoryItem, &pItem ) || NULL == pItem || NULL == pInventoryItem )
        return;

    m_pkItemImg->SetItemInfo( pItem->dwGoodsID );
    
    m_vecSocketInfo = pInventoryItem->m_vecSocket;

    if( EISI_MAX < static_cast< int >( m_vecSocketInfo.size() ) )
    {
        int iSocketCount = static_cast< int >( m_vecSocketInfo.size() );
        int iScrollMax =  static_cast< int >( iSocketCount / EISI_ONE_LINE_COUNT );

        if( iSocketCount > (iScrollMax * EISI_ONE_LINE_COUNT ) )
            ++iScrollMax;

        m_pkScrollBar->SetScrollRangeMax( iScrollMax );
        m_pkScrollBar->ToggleRender( true );
    }

   SetScoketView();
}

void KGCItemSocketManagementDlg::OnClickActionButton( KSocketInfo::SOCKET_STATE eItemSocketManagementTab_)
{
//     GCFUNC::CreateSequenceToUI( this, "Socket_01", 340.0f, 330.0f );
//     GCFUNC::CreateSequenceToUI( this, "Socket_02", 340.0f, 330.0f );

    int iUseMaterialKind = static_cast<int>( m_eUseMaterialKind );
    switch( eItemSocketManagementTab_ )
    {
    case KSocketInfo::STS_CLOSED:
        {
            KP2P::GetInstance()->Send_OpenSocketReq( m_ItemUID, m_UseCashItemID, m_iSelectedIndex, iUseMaterialKind );
        }
        break;
    case KSocketInfo::STS_OPENED:
        {
            KP2P::GetInstance()->Send_InsertItemSocketReq( m_ItemUID, m_UseCashItemID, m_iSelectedIndex , iUseMaterialKind, m_SelectedMonsterCardID );
        }
        break;
    case KSocketInfo::STS_USING:
        {
            KP2P::GetInstance()->Send_RemoveItemSocketReq( m_ItemUID, m_UseCashItemID, m_iSelectedIndex, iUseMaterialKind);
        }
        break;
    }
    Result_SocketManagement_Action = INT_MAX;
    g_MyD3D->WaitForServerAck( Result_SocketManagement_Action, INT_MAX, 3000, TIME_OUT_VALUE );

    if( 0 == Result_SocketManagement_Action )
    {
        InitTabInfo( m_eSelectedTab );
    }
}

void KGCItemSocketManagementDlg::SetCondition()
{
    //조건 체크
    int iNeedCount = 0;
    int iOwnCount = 0;
    m_pkActionBtn[m_eSelectedTab]->Lock( true );
    m_pkOwnCount->SetFontColor( D3DCOLOR_ARGB( 255,255,255,255 ) );

    if( UMK_GAMEPOINT == m_eUseMaterialKind )
    {
        m_UseCashItemID = 0;
        iOwnCount = g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP();
    }
    else if( UMK_CASHITEM == m_eUseMaterialKind )
    {
        m_UseCashItemID = g_pItemMgr->GetSocketChangeItemID();
        iOwnCount = g_pItemMgr->GetInventoryItemDuration( m_UseCashItemID );
    }

    int iSelectIndex = m_iSelectedIndex - GetScrollTopIndex();
    if( IsEnableSocketIndex( iSelectIndex ) )
    {
        std::map< DWORD, KSocketConsumeInfo > mapUseCountList;
        if( UMK_GAMEPOINT == m_eUseMaterialKind )
        {
            mapUseCountList.swap( g_pItemMgr->GetSocketManageConsumeGPList() );
        }
        else if( UMK_CASHITEM == m_eUseMaterialKind )
        {
            mapUseCountList.swap( g_pItemMgr->GetSocketManageCashItemUseCountList() );            
        }
        //실제조건 체크
        
        iNeedCount = iOwnCount + 1;
        KItem*  pInven = NULL;
        GCItem* pItem = NULL;
        if( g_pItemMgr->FindInventory( m_ItemUID, &pInven, &pItem ) && pInven && pItem )
        {
            std::map< DWORD, KSocketConsumeInfo >::iterator mitUseCount = mapUseCountList.find( g_pItemMgr->GetlItemEquipLevel(pItem, pInven) );
            if( mapUseCountList.end() != mitUseCount )
            {
                KItem::ITEM_GRADE eItemGrade = static_cast< KItem::ITEM_GRADE >( pInven->m_cGradeID );
                if( KItem::GRADE_NORMAL == eItemGrade )
                    iNeedCount = mitUseCount->second.m_dwNormal;
                else if( KItem::GRADE_RARE == eItemGrade )
                    iNeedCount = mitUseCount->second.m_dwRare;
                else if( KItem::GRADE_EPIC == eItemGrade )
                    iNeedCount = mitUseCount->second.m_dwEpic;
                else  if (KItem::GRADE_LEGEND == eItemGrade)
                    iNeedCount = mitUseCount->second.m_dwLegend;
                else  if (KItem::GRADE_MYSTIC == eItemGrade)
                    iNeedCount = mitUseCount->second.m_dwMystic;
            }
        }

        if( iNeedCount <= iOwnCount )
        {
            m_pkActionBtn[m_eSelectedTab]->Lock( false );
            m_pkOwnCount->SetFontColor( D3DCOLOR_ARGB( 255,255,255,255 ) );
        }
        else
            m_pkOwnCount->SetFontColor( D3DCOLOR_ARGB( 255,255,0,0 ) );
    }

    std::wstring strItemName;

    if( UMK_GAMEPOINT == m_eUseMaterialKind )
    {
        strItemName = L"GP";
        m_pkTextNeed->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SOCKET_NEED_COUNT, "l", strItemName ) );
        m_pkNeedCount->SetText(iNeedCount);

        m_pkTextOwn->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SOCKET_OWN_COUNT, "l", strItemName ) );
        m_pkOwnCount->SetText(iOwnCount);
    }
    else if( UMK_CASHITEM == m_eUseMaterialKind )
    {   
        GCItem* pItemCashItem = g_pItemMgr->GetItemData( g_pItemMgr->GetSocketChangeItemID() );
        if( pItemCashItem )
            strItemName = pItemCashItem->strItemName;

        m_pkTextNeed->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SOCKET_NEED_COUNT, "l", strItemName ) );
        m_pkNeedCount->SetText(iNeedCount);
        m_pkTextOwn->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SOCKET_OWN_COUNT, "l", strItemName ) );
        m_pkOwnCount->SetText(iOwnCount);
    }
    else
    {
        m_pkTextNeed->SetText(L"");
        m_pkNeedCount->SetText(L"");

        m_pkTextOwn->SetText(L"");
        m_pkOwnCount->SetText(L"");
    }
    
    m_pkDescription->SetTextAutoMultiline( GetDescString() );
}

void KGCItemSocketManagementDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCItemSocketManagementDlg::OnScroll()
{
    SetScoketView();
    RefreshSocketActiveEdge();
    RefreshSelectedMonsterCard();
}

KSocketInfo KGCItemSocketManagementDlg::GetSocketInfo(int iIndex_)
{
    if(m_vecSocketInfo[iIndex_].m_cSlotID == iIndex_) return m_vecSocketInfo[iIndex_];
    std::vector<KSocketInfo>::iterator vItor = m_vecSocketInfo.begin();

    for(;vItor != m_vecSocketInfo.end();vItor++)
    {
        if((*vItor).m_cSlotID == iIndex_)
            return (*vItor);
    }
    return *m_vecSocketInfo.begin();
}

void KGCItemSocketManagementDlg::SetScoketView()
{
    for( int i = 0; i < EISI_MAX; ++i )
    {
        m_pkMonsterCardImg[i]->ToggleRender( false );
        m_pkSocketBtn[i]->ToggleRender( false );
    }

    int iTopIndex = GetScrollTopIndex();
    int iViewSocketCount = static_cast<int>(m_vecSocketInfo.size() ) - iTopIndex;

    if(iViewSocketCount > EISI_MAX )
        iViewSocketCount = EISI_MAX;

    for(int i = 0 ; i < iViewSocketCount; ++i )
    {
        int iRealSocketIndex = iTopIndex + i;
        KSocketInfo socketinfo = GetSocketInfo(iRealSocketIndex);
        KSocketInfo::SOCKET_STATE eScoketState = (KSocketInfo::SOCKET_STATE)socketinfo.m_cState;
        m_pkSocketBtn[i]->SetSocketState( eScoketState );
        m_pkSocketBtn[i]->ToggleRender( true );

        if( KSocketInfo::STS_USING == eScoketState)
        {
            m_pkMonsterCardImg[i]->SetItemInfo( socketinfo.m_CardItemID / 10 );
            m_pkMonsterCardImg[i]->ToggleRender( true );
        }
    }

    for( int i = 0; i < EISI_MAX; ++i )
    {
        bool bSelfUpdate = ( m_pkSocketBtn[i]->GetSocketState() == m_eSelectedTab );
        m_pkMonsterCardImg[i]->SetSelfInputCheck( bSelfUpdate );
        m_pkSocketBtn[i]->SetSelfInputCheck( bSelfUpdate );
        m_pkSocketBtn[i]->AllSelfInputCheck( bSelfUpdate );
    }
}

void KGCItemSocketManagementDlg::RefreshSocketActiveEdge()
{
    for(int i = 0 ; i < EISI_MAX; ++i )
    {
        m_pkSocketActiveEdge[i]->ToggleRender( false );
    }

    int iSelectedIendx = m_iSelectedIndex - GetScrollTopIndex();
    if( IsEnableSocketIndex( iSelectedIendx ) )
    {
        m_pkSocketActiveEdge[iSelectedIendx]->ToggleRender( true );
    }
}


void KGCItemSocketManagementDlg::RefreshSelectedMonsterCard()
{
    int iSelectedIendx = m_iSelectedIndex - GetScrollTopIndex();
    if( IsEnableSocketIndex( iSelectedIendx )
        && 0 != m_SelectedMonsterCardID )
    {
        m_pkMonsterCardImg[iSelectedIendx]->SetItemInfo( m_SelectedMonsterCardID );
        m_pkMonsterCardImg[iSelectedIendx]->ToggleRender( true );
    }
}

bool KGCItemSocketManagementDlg::IsEnableSocketIndex( int iSocketIndex_)
{
    return ( 0<= iSocketIndex_ && iSocketIndex_ < EISI_MAX );
}

std::wstring KGCItemSocketManagementDlg::GetDescString()
{
    std::wstring strDescString;
    if( KSocketInfo::STS_CLOSED == m_eSelectedTab )
    {
        if( -1 == m_iSelectedIndex )
            strDescString = g_pkStrLoader->GetString( STR_ID_ITEM_BEFORE_SELECT_CLOSED_SOCKET );
        else
            strDescString = g_pkStrLoader->GetString( STR_ID_ITEM_AFTER_SELECT_CLOSED_SOCKET );
    }
    else if( KSocketInfo::STS_OPENED == m_eSelectedTab )
    {
        if( -1 == m_iSelectedIndex )
        {
            strDescString = g_pkStrLoader->GetString( STR_ID_ITEM_BEFORE_SELECT_OPENED_SOCKET );
        }
        else
        {
            GCItem* pMonsterCardItem = g_pItemMgr->GetItemData( m_SelectedMonsterCardID );
            if( pMonsterCardItem )
            {
                std::wstring strGrade = g_pItemMgr->GetItemGradeString(pMonsterCardItem->cItemGrade);
                int iLevel = pMonsterCardItem->iNeedLevelForEquippingItem;
                strDescString = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_AFTER_SELECT_OPENED_SOCKET, "lil", pMonsterCardItem->strItemName, iLevel, strGrade );
            }
        }
    }
    else if( KSocketInfo::STS_USING == m_eSelectedTab )
    {
        if( -1 == m_iSelectedIndex )
        {
            strDescString = g_pkStrLoader->GetString( STR_ID_ITEM_BEFORE_SELECT_USING_SOCKET );
        }
        else
        {
            KItem* pInventoryItem = NULL;
            if( g_pItemMgr->FindInventory( m_ItemUID, &pInventoryItem ) && pInventoryItem )
            {
                std::vector< KSocketInfo>::iterator vitMonsterCard = std::find_if( pInventoryItem->m_vecSocket.begin(), pInventoryItem->m_vecSocket.end(), boost::bind( &KSocketInfo::m_cSlotID, _1) == static_cast<char>(m_iSelectedIndex ) );
                GCItem* pMonsterCardItem = NULL;
                if( pInventoryItem->m_vecSocket.end() != vitMonsterCard &&
                    (pMonsterCardItem = g_pItemMgr->GetItemData( (*vitMonsterCard).m_CardItemID / 10) ) )
                {
                    int iLevel = g_pItemMgr->GetlItemEquipLevel(pMonsterCardItem, pInventoryItem);
                    std::wstring strGrade = g_pItemMgr->GetItemGradeString(pMonsterCardItem->cItemGrade);
                    strDescString = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_AFTER_SELECT_USING_SOCKET, "lil", pMonsterCardItem->strItemName, iLevel, strGrade );
                }
            }
        }
    }
    else if( KSocketInfo::STS_BREAK == m_eSelectedTab )
    {
        if( -1 == m_iSelectedIndex )
            strDescString = g_pkStrLoader->GetString( STR_ID_ITEM_BEFORE_SELECT_BREAK_SOCKET );
        else
            strDescString = g_pkStrLoader->GetString( STR_ID_ITEM_AFTER_SELECT_BREAK_SOCKET );
    }

    return strDescString;
}

bool KGCItemSocketManagementDlg::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    this->SetSelectedItemUID( static_cast<GCITEMUID>( kInfo_.m_lParam ) );
    return true;
}
