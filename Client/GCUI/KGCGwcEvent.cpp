#include "stdafx.h"
#include "GCUI/KGCGwcEvent.h"
#include "GCUI/KGCGwcCountry.h"

IMPLEMENT_CLASSNAME( KGCGwcEvent );
IMPLEMENT_WND_FACTORY( KGCGwcEvent );
IMPLEMENT_WND_FACTORY_NAME( KGCGwcEvent, "gc_gwc" );

KGCGwcEvent::KGCGwcEvent( void )
: m_pkCloseBtn  (NULL ),
m_pkOkBtn  (NULL )
{
    m_mapCountryInfo.clear();
    m_mapCountryBar.clear();

    LINK_CONTROL( "btn_close",m_pkCloseBtn );
    LINK_CONTROL( "btn_ok",m_pkOkBtn );

    m_mapCountryBar[1] = NULL;
    m_mapCountryBar[3] = NULL;
    m_mapCountryBar[4] = NULL;
    m_mapCountryBar[8] = NULL;
    m_mapCountryBar[9] = NULL;
    m_mapCountryBar[11] = NULL;
    m_mapCountryBar[12] = NULL;

    LINK_CONTROL( "country_1",m_mapCountryBar[1] );
    LINK_CONTROL( "country_3",m_mapCountryBar[3] );
    LINK_CONTROL( "country_4",m_mapCountryBar[4] );
    LINK_CONTROL( "country_8",m_mapCountryBar[8] );
    LINK_CONTROL( "country_9",m_mapCountryBar[9] );
    LINK_CONTROL( "country_11",m_mapCountryBar[11] );
    LINK_CONTROL( "country_12",m_mapCountryBar[12] );

}

KGCGwcEvent::~KGCGwcEvent( void )
{
}

void KGCGwcEvent::OnCreate( void  )
{
    m_pkCloseBtn->InitState(true, true, this );
    m_pkCloseBtn->DisconnectAllEventProcedure();
    m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
    m_pkCloseBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGwcEvent::OnClickClose );

    m_pkOkBtn->InitState(true, true, this );
    m_pkOkBtn->DisconnectAllEventProcedure();
    m_pkOkBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGwcEvent::OnClickOK );
    
    for (std::map<DWORD, KGCGwcCountry*>::iterator mit = m_mapCountryBar.begin() ; mit != m_mapCountryBar.end() ; mit++ )
    {
        if(mit->second != NULL )
            mit->second->InitState(true,true,this);
    }
}

void KGCGwcEvent::ActionPerformed( const KActionEvent& event )
{
    
}

void KGCGwcEvent::OnCreateComplete( void )
{
    UpdateCountryInfo();
}

void KGCGwcEvent::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCGwcEvent::OnClickOK()
{
    ToggleRender( false );
    g_pkUIScene->EndMessageBox();

    Result_GwcRakingRewardReq = INT_MAX;
    KP2P::GetInstance()->Send_GWCRakingRewardReq();
    g_MyD3D->WaitForServerAck( Result_GwcRakingRewardReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

void KGCGwcEvent::SetCountryInfo( std::map<DWORD, KGWCCountryInfo>& mapCountryInfo_ )
{
    m_mapCountryInfo.clear();
    m_mapCountryInfo.swap( mapCountryInfo_ );
    bool temp = IsRenderOn();
    
    ToggleRender(true);
    UpdateCountryInfo();
}

void KGCGwcEvent::UpdateCountryInfo()
{
    int iIndexCnt = 0;
    std::map<DWORD, KGCGwcCountry*>::iterator mItWindow;

    for( std::map<DWORD, KGWCCountryInfo >::iterator mIt = m_mapCountryInfo.begin() ; mIt != m_mapCountryInfo.end();mIt++){
        int temp = mIt->first;
        if(COUNTRY_LA == temp ) continue;

        std::map<DWORD, KGCGwcCountry*>::iterator mItWindow = m_mapCountryBar.find( temp );
        if( mItWindow->second == NULL || mItWindow == m_mapCountryBar.end() ) continue;

        mItWindow->second->SetInformation( mIt->second );
        mItWindow->second->SetWindowPos( D3DXVECTOR2(58.0f,93.0f+(38.0f*( iIndexCnt ))));
        iIndexCnt++;
    }
}