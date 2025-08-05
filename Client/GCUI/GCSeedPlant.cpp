#include "stdafx.h"
#include "GCSeedPlant.h"
#include "GCSeedSlot.h"

IMPLEMENT_CLASSNAME( KGCSeedPlant );
IMPLEMENT_WND_FACTORY( KGCSeedPlant );
IMPLEMENT_WND_FACTORY_NAME( KGCSeedPlant, "gc_seed_plant" );

KGCSeedPlant::KGCSeedPlant( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnOK = NULL;
    m_pkBtnCancle = NULL;
    LINK_CONTROL( "btnClose",   m_pkBtnClose );
    LINK_CONTROL( "btnOK",      m_pkBtnOK );
    LINK_CONTROL( "btnCancle",  m_pkBtnCancle );

    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        m_apkSeedSlot[i] = NULL;

        char szTemp[128] = {0, };
        sprintf_s( szTemp, 127, "seed_slot%d", i );
        LINK_CONTROL( szTemp, m_apkSeedSlot[i] );
    }

    m_iActiveSlot = -1;
}

KGCSeedPlant::~KGCSeedPlant( void )
{
}

void KGCSeedPlant::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );

    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSeedSlot[i]->InitState( true, true, this );

    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
}

void KGCSeedPlant::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
    GCWND_MSG_MAP( m_pkBtnOK,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnOK );
    GCWND_MSG_MAP( m_pkBtnCancle,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );

    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        if( event.m_pWnd == m_apkSeedSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
            OnSlot( i );
    }
}

void KGCSeedPlant::OnClose( void )
{
    this->ToggleRender( false );
}

void KGCSeedPlant::OnOK( void )
{
    if( 0 > m_iActiveSlot )
        return;

    if( false == SiKGCAgitGameManager()->CheckPlantEnable() )
        return;

    std::pair<GCITEMUID, GCITEMUID> PlantUID = SiKGCAgitGameManager()->GetNowPlantPotUID();

    if( PlantUID.first != 0 && PlantUID.second != 0 && g_pkUIScene->m_pkGuideAzit ){
        KP2P::GetInstance()->Send_SeedFlowerPotReq( PlantUID.first, PlantUID.second, m_iActiveSlot );
        g_pkUIScene->m_pkGuideAzit->SetCompleateActionNum(4);

    }
}

void KGCSeedPlant::OnSlot( int iIndex_ )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        if( i == iIndex_ )
        {
            m_apkSeedSlot[i]->SetActive( true );
            m_iActiveSlot = i;
        }
        else
            m_apkSeedSlot[i]->SetActive( false );
    }
}

void KGCSeedPlant::InitUI( void )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSeedSlot[i]->Clear();
    
    m_apkSeedSlot[0]->InitSeedSlot( 0, 1, 1, 1 );
    m_apkSeedSlot[1]->InitSeedSlot( 1, 1, 30, 5 );
    m_apkSeedSlot[2]->InitSeedSlot( 2, 1, 120, 10 );

    m_iActiveSlot = -1;
}