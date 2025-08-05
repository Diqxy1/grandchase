#include "StdAfx.h"
#include "KGCHeroDungeonOpentimeBox.h"

IMPLEMENT_CLASSNAME( KGCHeroDungeonOpentimeBox );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonOpentimeBox );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonOpentimeBox, "gc_hero_dungeon_opentime_box" );

KGCHeroDungeonOpentimeBox::KGCHeroDungeonOpentimeBox(void)
: m_pkScrollBar( NULL )
, m_pkBtnClose( NULL )
, m_pkStaticDungeonName( NULL )
, m_pkBackgroundInner( NULL )
{
    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < EMAX_OPEN_TIME_LIST_NUM; ++i )
    {
        m_pkStaticOpentimeList[i] = NULL;
        sprintf_s( str, "static_opentime%d", i );
        LINK_CONTROL( str, m_pkStaticOpentimeList[i] );
    }

    LINK_CONTROL( "scroll",                 m_pkScrollBar );
    LINK_CONTROL( "background",             m_pkBackgroundInner );
    LINK_CONTROL( "btn_close",              m_pkBtnClose );
    LINK_CONTROL( "static_dungeon_name",    m_pkStaticDungeonName );
}

KGCHeroDungeonOpentimeBox::~KGCHeroDungeonOpentimeBox(void)
{
}

void KGCHeroDungeonOpentimeBox::OnCreate( void )
{
    for( int i = 0; i < EMAX_OPEN_TIME_LIST_NUM; ++i )
    {
        m_pkStaticOpentimeList[i]->InitState(true);
        m_pkStaticOpentimeList[i]->SetAlign(DT_CENTER);
        m_pkStaticOpentimeList[i]->SetText(L"");
    }

    m_pkScrollBar->InitState(true, true, this );
    m_pkBackgroundInner->InitState(true);
    m_pkBtnClose->InitState(true, true, this);
    m_pkStaticDungeonName->InitState(true);
    m_pkStaticDungeonName->SetText(L"");
    m_pkStaticDungeonName->SetAlign(DT_CENTER);
    InitScroll();
}

void KGCHeroDungeonOpentimeBox::OnCreateComplete( void )
{
}

void KGCHeroDungeonOpentimeBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MAP( m_pkScrollBar, OnScroll );
    GCWND_MSG_MAP( m_pkBtnClose,	    		    KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCHeroDungeonOpentimeBox::OnDestroyComplete( void )
{
}

void KGCHeroDungeonOpentimeBox::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

    if( m_pkBackgroundInner->CheckPosInWindowBound(vMousePos) )
    {
        if( g_pkInput->ISWheelUp() )
        {
            if( m_pkScrollBar->GetScrollPos() > 0 )
            {
                m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos());
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }
        else if( g_pkInput->ISWheelDown() )
        {
            if( m_pkScrollBar->GetScrollPos() < m_pkScrollBar->GetScrollRangeMax() )
            {
                m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos());
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }	
    }

    if ( g_pkInput->IsUp( DIK_ESCAPE ) )
    {
        OnClose();
    }
}

void KGCHeroDungeonOpentimeBox::PostChildDraw( void )
{
}

void KGCHeroDungeonOpentimeBox::OnScroll()
{
    int iRealindex = m_pkScrollBar->GetScrollPos();
    int iMaxListView = GetListViewCount( iRealindex );

    for(int i = 0; i < iMaxListView; ++i )
    {
        int iIndex = i + iRealindex;

        if( 0 <= iIndex && iIndex < static_cast<int>(m_prAllOpentime.second.size()) )
        {		
            m_pkStaticOpentimeList[i]->ToggleRender( true );
            if( static_cast<int>(m_prAllOpentime.second.size()) > iIndex)
            {
                m_pkStaticOpentimeList[i]->SetText(m_prAllOpentime.second[iIndex]);
            }
            else
            {
                m_pkStaticOpentimeList[i]->SetText(L"");
            }
        }
    }

    for(int i = iMaxListView; i < EMAX_OPEN_TIME_LIST_NUM; ++i)
    {
        m_pkStaticOpentimeList[i]->ToggleRender( false );
    }	
}

int KGCHeroDungeonOpentimeBox::GetListViewCount(int iIndex_)
{
    int iListViewCount = static_cast<int>(m_prAllOpentime.second.size() ) - iIndex_;

    if(iListViewCount < EMAX_OPEN_TIME_LIST_NUM)
        return iListViewCount;

    return EMAX_OPEN_TIME_LIST_NUM;
}

void KGCHeroDungeonOpentimeBox::InitScroll()
{
    m_pkScrollBar->SetScrollPos( 0 );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollPageSize( 1 );

    int iFileCount = static_cast<int>( m_prAllOpentime.second.size() );    
    int iScrollMax = 0;
    int iCount =  iFileCount - EMAX_OPEN_TIME_LIST_NUM;

    if(iCount > 0)
    {
        iScrollMax = iCount;
    }

    m_pkScrollBar->SetScrollRangeMax( iScrollMax + 1 );

    OnScroll();
}

void KGCHeroDungeonOpentimeBox::OnClose()
{
    this->ToggleRender(false);
}

void KGCHeroDungeonOpentimeBox::SetAllOpentime( std::pair<std::wstring, std::vector<std::wstring>> prOpentime_ )
{
    this->ToggleRender(true);    
    m_pkStaticDungeonName->SetText(prOpentime_.first);
    m_prAllOpentime = prOpentime_;

    InitScroll();
}