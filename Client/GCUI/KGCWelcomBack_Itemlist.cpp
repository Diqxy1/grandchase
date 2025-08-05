#include "stdafx.h"
#include "KGCWelcomBack_Itemlist.h"
//

IMPLEMENT_CLASSNAME( KGCWelcomeBackItemList );
IMPLEMENT_WND_FACTORY( KGCWelcomeBackItemList );
IMPLEMENT_WND_FACTORY_NAME( KGCWelcomeBackItemList, "gc_welcome_back_itemlist" );

KGCWelcomeBackItemList::KGCWelcomeBackItemList(void)
: m_pkBack( NULL )
{
    m_strItemList = L"";
    LINK_CONTROL( "back",                m_pkBack );

    m_vecItemID.clear();
    m_pkItemInfoBox = NULL;
    m_CuriItemID = 0;

    LINK_CONTROL( "item_info_box",      m_pkItemInfoBox);
}

KGCWelcomeBackItemList::~KGCWelcomeBackItemList(void)
{
}

void KGCWelcomeBackItemList::OnCreate( void )
{
    m_pkBack->InitState( true );
    m_pkItemInfoBox->InitState( false, true, this );
    m_CuriItemID = 0;
}

void KGCWelcomeBackItemList::SetItemList( DWORD iItemID, int iItemNum )
{
    WCHAR wszItemNumtemp[MAX_PATH] = {0, };
    std::wstring strItemInfo = g_pItemMgr->GetItemName(iItemID/10 )+ L"  (" + ((iItemNum == -1) ? L"1" : _itow( iItemNum, wszItemNumtemp, 10 )) + L")";
    KD3DStatic* pStatic = new KD3DStatic;

    pStatic->InitState( true );
    pStatic->SetAlign( DT_LEFT );
    m_pkBack->AddChildWithRegistToLua( pStatic );
    m_vecItemID.push_back( iItemID );

    if( (int)m_pkBack->GetWidth() < g_pkFontMgr->GetWidth( strItemInfo.c_str() ) + 15 )
    {
        m_pkBack->SetWidth( g_pkFontMgr->GetWidth( strItemInfo.c_str() ) + 15 );
    }

    D3DXVECTOR2 vStaticOffset(0, (float)(m_pkBack->GetNumChild() - 10) * 15 + 5);

    ListHeightChange();
    pStatic->SetOffset( D3DXVECTOR2(10, 0) );
    pStatic->SetWindowPosDirect( vStaticOffset );
    pStatic->SetWndState( D3DWS_ENABLED );
    pStatic->SetWidth( m_pkBack->GetWidth() );
    pStatic->SetHeight( 15 );
    pStatic->SetText( strItemInfo );

}

void KGCWelcomeBackItemList::ListHeightChange( void )
{
    const int iFrameThick = 4;
    int iHeight = (m_pkBack->GetNumChild() - 10) * 16;
    m_pkBack->SetHeight( iHeight + iFrameThick * 2 );

    for(int i = 3; i < 9; i++)
    {
        if( i == 3 || i == 4 || i == 5 )
        {
            m_pkBack->GetChild(i)->SetHeight(iHeight);
        }
        else
        {
            D3DXVECTOR2 wndpos = m_pkBack->GetChild(i)->GetFixedWindowLocalPos();
            wndpos.y = (float)( iHeight + m_pkBack->GetChild(i)->GetHeight() );
            m_pkBack->GetChild(i)->SetWindowPosDirect( wndpos );
        }
    }
}

void KGCWelcomeBackItemList::FrameMoveInEnabledState( void )
{
    /*
    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );
    if( m_pkBack->CheckPosInWindowBound( vMousePos ) )
    {
        bool bCheck = false;
        if( 0 < m_pkBack->GetNumChild() )
        {
            for( int j = 0; j < m_pkBack->GetNumChild(); ++j)
            {
                if( m_pkBack->GetChild(j)->CheckPosInWindowBound( vMousePos ) )
                {
                    // 아이템 정보 표시창 위치 보정.
                    D3DXVECTOR2 vecPos = m_pkBack->GetChild(j)->GetCurrentWindowPos();
                    vecPos.x += 80;
                    vecPos.y -= ( m_pkItemInfoBox->GetHeight() / 2.0f ) - (m_pkBack->GetHeight() / 2.0f);

                    if ( vecPos.y + m_pkItemInfoBox->GetHeight() > GC_SCREEN_HEIGHT )
                        vecPos.y = (float)(GC_SCREEN_HEIGHT - m_pkItemInfoBox->GetHeight());

                    m_pkItemInfoBox->SetWindowPosDirect(vecPos);

                    if( m_CuriItemID != m_pkItemInfoBox->GetItemID() )
                    {
                        m_pkItemInfoBox->SetItem(g_pItemMgr->GetItemData(m_vecItemID[j]));
                        m_CuriItemID = m_vecItemID[j];
                    }

                    m_pkItemInfoBox->ToggleRender(true);
                    bCheck = true;
                    break;
                }
            }
        }

        if( !bCheck )
            m_pkItemInfoBox->ToggleRender(false);
    }
    else
    {
        m_pkItemInfoBox->ToggleRender(false);
    }
    */
}

void KGCWelcomeBackItemList::OnDestroyComplete()
{
    m_pkItemInfoBox->InitState( false );
}

void KGCWelcomeBackItemList::ActionPerformed( const KActionEvent& event )
{
    // 오늘은 집에 갈 수 있을까..
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {
        if( 0 < m_pkBack->GetNumChild() )
        {
            for( unsigned int i = 0; i < m_pkBack->GetNumChild(); ++i)
            {
                if( strcmp( m_pkBack->GetChild(i)->GetClassName(), "KD3DStatic" ) == 0 )
                {
                    if( event.m_pWnd == m_pkBack->GetChild(i) )
                    {
//                        OnClickSubSkillBox( (KGCSkillBoxSub*)event.m_pWnd );
                    }
                }
            }
        }
    }
}

void KGCWelcomeBackItemList::InitItemList()
{
	for(int i = m_pkBack->GetNumChild() - 1 ; i >= 0; --i)
	{
		if(m_pkBack->GetChild(i)->GetWindowName().empty() )
			m_pkBack->DeleteChild(i);
	}
	
	m_vecItemID.clear();
}
