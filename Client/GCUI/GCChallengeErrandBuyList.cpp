#include "stdafx.h"

#include "GCChallengeErrandBuyList.h"

IMPLEMENT_CLASSNAME( KGCChallengeErrandBuyList );
IMPLEMENT_WND_FACTORY( KGCChallengeErrandBuyList );
IMPLEMENT_WND_FACTORY_NAME( KGCChallengeErrandBuyList, "gc_errand_buy_list" );


KGCChallengeErrandBuyList::KGCChallengeErrandBuyList( void ):
m_pkStaticAdvice( NULL ),
m_pkCloseBtn( NULL )
{
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apStaticBuyNum[i] = NULL;
        char strBuyNum[128];
        sprintf_s( strBuyNum, 127, "static_buy_num%d", i );
        LINK_CONTROL( strBuyNum, m_apStaticBuyNum[i] );
    }

    LINK_CONTROL( "static_advice", m_pkStaticAdvice );
    LINK_CONTROL( "btn_close", m_pkCloseBtn );
}

KGCChallengeErrandBuyList::~KGCChallengeErrandBuyList( void )
{
}

void KGCChallengeErrandBuyList::OnCreate( void )
{
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apStaticBuyNum[i]->InitState( true );
        m_apStaticBuyNum[i]->SetAlign(DT_CENTER);
        m_apStaticBuyNum[i]->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
        m_apStaticBuyNum[i]->SetText( 0 );
    }

    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_SPACE );
    m_pkStaticAdvice->InitState( true );
    m_pkStaticAdvice->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_PLAY_LIST_GUIDE).c_str());
}

void KGCChallengeErrandBuyList::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCChallengeErrandBuyList::FrameMoveInEnabledState( void )
{

}

void KGCChallengeErrandBuyList::SetBuyList( void )
{
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apStaticBuyNum[i]->InitState( true );
        m_apStaticBuyNum[i]->SetAlign(DT_CENTER);
        m_apStaticBuyNum[i]->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
        m_apStaticBuyNum[i]->SetText( 0 );
    }

    std::vector<ErrandItemBuyList> vecBuyList = SiGCChallengeErrandManager()->GetBuyInfo();

    if( vecBuyList.empty() )
        return;

    ErrandItemBuyList eBuyList;
    eBuyList.m_nType = 0;
    eBuyList.m_nItemNum = 0;

    std::vector<ErrandItemBuyList>::iterator itor;
    for( itor = vecBuyList.begin(); itor != vecBuyList.end(); itor++ )
    {
        eBuyList = *itor;

        if( eBuyList.m_nType < KGCChallengeErrandModule::NAME_HOME )
        {
            m_apStaticBuyNum[eBuyList.m_nType]->SetText( eBuyList.m_nItemNum );
            m_apStaticBuyNum[eBuyList.m_nType]->ToggleRender(true);
        }
    }

}

void KGCChallengeErrandBuyList::OnClose( void )
{
    ToggleRender( false );
}


