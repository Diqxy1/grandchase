#include "stdafx.h"
#include "GCReplayRecordREC.h"

IMPLEMENT_CLASSNAME( KGCReplayRecordREC );
IMPLEMENT_WND_FACTORY( KGCReplayRecordREC );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayRecordREC, "gc_replay_record_rec" );

KGCReplayRecordREC::KGCReplayRecordREC( void )
{
    m_pkRec = NULL;
    LINK_CONTROL( "background", m_pkRec );

    m_dwTimer = 0;
    m_bShow  = false;
}

KGCReplayRecordREC::~KGCReplayRecordREC( void )
{
}

void KGCReplayRecordREC::OnCreate( void )
{
    m_pkRec->InitState(true, true, this);
    m_pkRec->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    m_dwTimer = 0;
    m_bShow = true;
}

void KGCReplayRecordREC::ActionPerformed( const KActionEvent& event )
{

}

void KGCReplayRecordREC::RECPlay( void )
{
    m_pkRec->ToggleRender( true );
    m_dwTimer = ::timeGetTime();;
    m_bShow = true;
}

void KGCReplayRecordREC::RECStop( void )
{
    m_pkRec->ToggleRender( false );
    m_dwTimer = 0;
    m_bShow = false;
}

void KGCReplayRecordREC::FrameMoveInEnabledState( void )
{
    DWORD dwNow = ::timeGetTime();

   if( m_bShow )
   {
       if( dwNow - m_dwTimer > ON_TIME )
       {
           m_bShow = false;
           m_dwTimer = dwNow;
       }
   }
   else if( !m_bShow )
   {
       if( dwNow - m_dwTimer > ON_TIME ) 
       {
           m_bShow = true;
           m_dwTimer = dwNow;
       }
   }

   m_pkRec->ToggleRender( m_bShow );
}