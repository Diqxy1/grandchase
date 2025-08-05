#include "stdafx.h"
#include "KGCAttendanceItemWnd.h"

//

#include "MyD3D.h"
//

IMPLEMENT_CLASSNAME( KGCAttendanceItemWnd );
IMPLEMENT_WND_FACTORY( KGCAttendanceItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCAttendanceItemWnd, "gc_attendance_item_wnd" );

KGCAttendanceItemWnd::KGCAttendanceItemWnd( void ) : m_ItemInfo(NULL)
{
    //m_pTextureItem  = NULL;
    m_iItemID       = -1;
    m_bShow         = false;

    m_pkItemName        = NULL;
    //m_pkGetItemButton   = NULL;
    m_pkActiveFrame     = NULL;
    //m_pkItemLimit       = NULL;
    m_pkDuration        = NULL;

    LINK_CONTROL( "item_name_static",		m_pkItemName );
    //LINK_CONTROL( "get_item_button",        m_pkGetItemButton );
    LINK_CONTROL( "active_frame",			m_pkActiveFrame );
    //LINK_CONTROL( "item_limit_static",      m_pkItemLimit );
	LINK_CONTROL("item_img",					m_pkItemImg);
    LINK_CONTROL( "duration_static",			m_pkDuration );
}

KGCAttendanceItemWnd::~KGCAttendanceItemWnd( void )
{
    //SAFE_RELEASE( m_pTextureItem );
}

void KGCAttendanceItemWnd::OnCreate( void )
{
    m_pkItemName->InitState( true );
    m_pkActiveFrame->InitState( false );
    m_pkItemImg->InitState( true );
    m_pkDuration->InitState( true );

    m_pkDuration->SetAlign( DT_RIGHT );
    m_pkDuration->SetFontColor( 0xFFFFFF00 );

    m_bIsGetButtonClick = false;
}

void KGCAttendanceItemWnd::FrameMoveInEnabledState( void )
{
    //POINT   pt = g_pkInput->GetMousePos();
    //bool    bMouseHover = CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) );

    m_pkActiveFrame->ToggleRender( m_bIsGetButtonClick );
}

bool KGCAttendanceItemWnd::GetHoveredItem( GCItem*& pItem, D3DXVECTOR2 &vecPos )
{
    if( m_iItemID == -1 )
        return false;

    pItem = m_ItemInfo;

    vecPos = GetCurrentWindowPos();
    vecPos.x += GetWidth();

    return true;
}

void KGCAttendanceItemWnd::PostDraw( void )
{
//     if ( m_bShow && m_iItemID > -1 )
//     {		
// 
//         D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
// 
//         // 샵 아이템 이미지를 그린다.
//      /*   m_pTextureItem->SetDeviceTexture();
//         g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//         g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//         {
//             g_MyD3D->Draw_TL( vPos.x + 7.7f, vPos.y + 3.3f, vPos.x + 109.8f, vPos.y + 106.1f,
//                 0.0f, 0.0f, 1.0f, 1.0f );
//         }
//         KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MAGFILTER );
//         KD3DDeviceState::PopSamplerState( 0, D3DSAMP_MINFILTER );*/
// 
// 		m_pkItemImg->ShowItem();
// 
// // 		g_pGCDeviceManager2->DrawInScreen(m_pTextureItem,
// // 			vPos.x + 7.7f * m_fWindowScaleX, vPos.y + 3.3f *  m_fWindowScaleY, vPos.x + 109.8f * m_fWindowScaleX, vPos.y + 106.1f * m_fWindowScaleY,
// // 			0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);
//     }
}

void KGCAttendanceItemWnd::ActionPerformed( const KActionEvent& event )
{
    //if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        //g_KDSound.Play( "30" );

	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == this )
		{
			m_bIsGetButtonClick = !m_bIsGetButtonClick;
			g_KDSound.Play( "31" );
		}
	}

//     if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
//     {
//         if( event.m_pWnd == m_pkGetItemButton )
//         {
//             m_bIsGetButtonClick = true;
//             g_KDSound.Play( "31" );
//         }
//     }
}

void KGCAttendanceItemWnd::SetItem( bool bShow, const KItem kItem )
{
    //SAFE_RELEASE( m_pTextureItem );
    
    if( !bShow )
    {
        m_iItemID = -1;
        ShowItem( false );
        return;
    }

    m_iItemID = kItem.m_ItemID;
    m_ItemInfo = g_MyD3D->m_kItemMgr.GetItemData( kItem.m_ItemID);
    ASSERT( m_ItemInfo != NULL );
    m_iDuration = kItem.m_nCount;
    m_kItemUnit = kItem;

    //Texture Loading
    //m_pTextureItem = g_pItemMgr->CreateItemTexture( m_iItemID, 0 );
	m_pkItemImg->SetItemInfo(m_iItemID, -1, false, -1, 0, m_iDuration);

    WCHAR szItemName[MAX_PATH] = { 0, };
    std::size_t len = std::wcslen( m_ItemInfo->strItemName.c_str() );
    for ( std::size_t i = 0; i < len; ++i )
    {
        szItemName[i] = m_ItemInfo->strItemName[i];
        if ( g_pkFontMgr->GetWidth( szItemName ) > (int)( GetWidth() * m_fWindowScaleX) && i > 0 )
        {
            szItemName[i-1] = szItemName[i] = L'\0';
            break;
        }
    }
    m_pkItemName->SetText( szItemName );

    // 속성 능력치 적용!
    ///////////////////////////////////////////////////////////////////////////////////////
    std::vector< KAttributeInfo > vecAttribute = kItem.m_vecAttribute;
    std::vector< KAttributeInfo >::iterator vitAttribute = vecAttribute.begin();

    char cEquipDecLvValue = 0;
    while ( vecAttribute.end() != vitAttribute )
    {
        if ( (*vitAttribute).m_cType == ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN )
        {
            cEquipDecLvValue = static_cast<char>((*vitAttribute).m_fValue);
        }
        ++vitAttribute;
    }

    
//     KItem kItemTemp = kItem;
//     int iNeedLevelForEquippingItem = g_pItemMgr->GetlItemEquipLevel(m_ItemInfo, &kItemTemp, NULL, true);
// 
//     // 제한 레벨 표시
//     std::vector<SCharInfo> vecCharInfo = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo;
//     for( int i = 0; i < (int)vecCharInfo.size(); ++i )
//     {
//         DWORD dwFlag = g_pItemMgr->GetCharFlag( vecCharInfo[i].iCharType );
// 
//         if( m_ItemInfo->dwCharType & dwFlag )
//         {
//             if( vecCharInfo[i].iLevel < iNeedLevelForEquippingItem - cEquipDecLvValue )
//             {
//                 std::wostringstream strmText;
//                 strmText << g_pkStrLoader->GetString( STR_ID_NOT_EQUIP_LEVEL ) << iNeedLevelForEquippingItem << ')';
//                 m_pkItemLimit->SetText( strmText.str() );
//             }
//             else
//             {
//                 m_pkItemLimit->SetText( L"" );
//             }
//         }
//     }

    // 기간제 이고 영구가 아닐때.. 기간이 얼마짜린지 보여준다
    if(  kItem.m_nPeriod != KItem::UNLIMITED_ITEM )
    {
        m_pkDuration->SetText( g_pkStrLoader->GetReplacedString( STR_ID_DURATION_PERIOD, "i", kItem.m_nPeriod ) );
    }
    else if( kItem.m_nCount != KItem::UNLIMITED_ITEM )
    {
        m_pkDuration->SetText( g_pkStrLoader->GetReplacedString( STR_ID_DURATION_COUNT, "i", kItem.m_nCount ) );
    }
    else
    {
        m_pkDuration->SetText( L"" );
    }

    ShowItem( true );

}

void KGCAttendanceItemWnd::ShowItem( bool bShow )
{
    m_bShow = bShow;

    m_pkItemName->ToggleRender( bShow );
    m_pkItemImg->ToggleRender( bShow );
    m_pkDuration->ToggleRender( bShow );

    if ( !bShow )
    {
        m_pkItemName->SetText( L"" );
        //m_pkItemLimit->SetText( L"" );
        m_pkDuration->SetText( L"" );
    }

    //m_pkGetItemButton->ToggleRender( bShow );
}
