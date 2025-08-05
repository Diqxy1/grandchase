#include "stdafx.h"
#include "GCNeonsignItemList.h"
//
//
//
#include "KNeonSign.h"
//

#include "GCNeonsignSelectBox.h"

//
#include "MyD3D.h"

IMPLEMENT_CLASSNAME( KGCNeonsignItemList );
IMPLEMENT_WND_FACTORY( KGCNeonsignItemList );
IMPLEMENT_WND_FACTORY_NAME( KGCNeonsignItemList, "gc_neonsign_item_list" );

KGCNeonsignItemList::KGCNeonsignItemList(void)
: m_pkScroll(NULL)
, m_pkSelect(NULL)
, m_iSelectItemID(-1)
, m_iScrollPos(0)
, m_eType( KSignBoardData::SBT_SQUARE )
{
    char strName[MAX_PATH];
    char strNum[MAX_PATH];
    for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
    {
        m_pkNeonsignNum[i] = NULL;
        m_pkNeonsignItem[i] = NULL;
        m_pkItemTex[i] = NULL;
        m_aiItemID[i] = -1;

        sprintf( strName, "neonsign_item%d", i+1 );
        LINK_CONTROL( strName, m_pkNeonsignItem[i] );
        sprintf( strNum, "neonsign_num%d", i+1 );
        LINK_CONTROL( strNum, m_pkNeonsignNum[i] );
    }

    LINK_CONTROL( "scroll", m_pkScroll );
    LINK_CONTROL( "neonsign_item_select", m_pkSelect );

    m_vecItemID.clear();
}

KGCNeonsignItemList::~KGCNeonsignItemList(void)
{
    for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
    {
        SAFE_RELEASE( m_pkItemTex[i] );
    }
}

void KGCNeonsignItemList::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkScroll )
    {
        int iPos = m_pkScroll->GetScrollPos();

        if( m_iScrollPos != iPos )
        {
            g_KDSound.Play( "73" );
            OnScrollPos( iPos );
        }
    }
}

void KGCNeonsignItemList::OnCreate()
{
    for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
    {
        m_pkNeonsignItem[i]->InitState( true );
        m_pkNeonsignNum[i]->InitState( true );
        m_pkNeonsignNum[i]->SetText( L"0" );
    }
    m_pkSelect->InitState( true );

    m_pkScroll->InitState( true, true, this );
    m_pkScroll->SetScrollPos( 0 );
    m_pkScroll->SetScrollPageSize( NUM_NEONSIGN_ITEM );
    m_pkScroll->SetScrollRangeMin( 0 );
    m_pkScroll->SetScrollRangeMax( (int)m_vecItemID.size() );
    m_pkScroll->SetScrollGap( 1 );
}

void KGCNeonsignItemList::OnScrollPos( int iPos )
{
    m_iScrollPos = iPos;
    RefreshNeonsignItem();
}

void KGCNeonsignItemList::RefreshNeonsignItem()
{
    int iStartIndex = m_iScrollPos * NUM_NEONSIGN_ITEM;

    for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
    {
        if( iStartIndex + i >= (int)m_vecItemID.size() )
        {
            m_aiItemID[i] = -1;
            m_pkNeonsignNum[i]->SetText( L"" );
            continue;
        }

        m_aiItemID[i] = m_vecItemID[iStartIndex+i];
		if(KNeonSignItem::NSST_HIDE != SiKNeonSign()->GetNeonSignShowType(m_aiItemID[i]) )
		{
			m_pkNeonsignNum[i]->SetText( g_pItemMgr->GetInventoryItemDuration( m_aiItemID[i] / 10 ) );
		}else
		{
			m_pkNeonsignNum[i]->SetText( L"" );
		}
    }

    for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
    {
        GCDeviceTexture* pTempTex = m_pkItemTex[i];
        m_pkItemTex[i] = g_pItemMgr->CreateItemTexture( m_aiItemID[i] / 10, 0 );
        SAFE_RELEASE( pTempTex );
    }
}

void KGCNeonsignItemList::PostChildDraw()
{	

    for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
    {
        if( m_aiItemID[i] == -1 || KNeonSignItem::NSST_HIDE == SiKNeonSign()->GetNeonSignShowType(m_aiItemID[i]) ) 
            continue;

        D3DXVECTOR2 vPos = m_pkNeonsignItem[i]->GetRelocatedWindowPos();
        

		g_pGCDeviceManager2->PushState();
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        {
			g_pGCDeviceManager2->DrawInScreen(m_pkItemTex[i],
				vPos.x + 3.0f * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, vPos.y, vPos.x + 76.0f * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, vPos.y + 73.0f * m_fWindowScaleY * GC_SCREEN_DIV_WIDTH, 0.0f, 0.0f, 1.0f, 1.0f ,
				false,10.0f,0xffffffff);
        }
        g_pGCDeviceManager2->PopState();

		
    }
}

void KGCNeonsignItemList::FrameMoveInEnabledState()
{
    POINT       pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    if( true == this->CheckPosInWindowBound( vMousePos ) )
    {
        if( GetParent() == g_pkUIScene->GetCurMsgBox() )
        {
            if( g_pkInput->BtnUp( KInput::MBLEFT ) )
            {
                for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
                {
                    if( m_aiItemID[i] == -1 || KNeonSignItem::NSST_HIDE == SiKNeonSign()->GetNeonSignShowType(m_aiItemID[i]) )
                        continue;

                    if( true == m_pkNeonsignItem[i]->CheckPosInWindowBound( vMousePos ) )
                    {
                        m_iSelectItemID = m_aiItemID[i];
                        m_pkSelect->SetWindowPosDirect( m_pkNeonsignItem[i]->GetCurrentWindowLocalPos() );

                        g_pkUIScene->m_pkNeonsignSelectBox->SetItem( m_iSelectItemID );
                    }
                }
            }
        }
    }

    m_pkSelect->ToggleRender( false );
    for( int i = 0; i < NUM_NEONSIGN_ITEM; ++i )
    {
        if( m_aiItemID[i] != -1 && m_aiItemID[i] == m_iSelectItemID && KNeonSignItem::NSST_HIDE != SiKNeonSign()->GetNeonSignShowType(m_aiItemID[i]) )
        {
            m_pkSelect->ToggleRender( true );
        }
    }
}

void KGCNeonsignItemList::OnCreateComplete()
{
    m_pkScroll->InitState( true, true, this );
    m_pkScroll->SetScrollPos( 0 );
    m_pkScroll->SetScrollPageSize( NUM_NEONSIGN_ITEM );
    m_pkScroll->SetScrollRangeMin( 0 );
    m_pkScroll->SetScrollRangeMax( (int)m_vecItemID.size() );
    m_pkScroll->SetScrollGap( 1 );

    m_vecItemID = SiKNeonSign()->GetNeonsignItemList( m_eType );
    OnScrollPos(0);
    m_iSelectItemID = -1;
}
