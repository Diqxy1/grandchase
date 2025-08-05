#include "stdafx.h"
#include "KGCMissionCharSelector_S4.h"

IMPLEMENT_CLASSNAME( KGCMissionCharSelector_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionCharSelector_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionCharSelector_S4, "gc_mission_char_selecter" );

KGCMissionCharSelector_S4::KGCMissionCharSelector_S4( void )
: m_FuncCallBack( NULL )
, m_FuncCallBackParm1( NULL )
, m_iSelectIdx( 0 )
{
    char szTemp[128];
    memset(m_pkChar, 0, sizeof(KD3DWnd*)*GC_CHAR_NUM*NUM_CHAR_UI);
    for(int iLoop=0; iLoop<GC_CHAR_NUM; ++iLoop)
    {
        sprintf(szTemp, "charname%d", iLoop);
        LINK_CONTROL( szTemp, m_pkChar[ iLoop ][ CHAR_UI_NAME ] );
        sprintf(szTemp, "charface%d", iLoop);
        LINK_CONTROL( szTemp, m_pkChar[ iLoop ][ CHAR_UI_FACE ] );
    }
}

KGCMissionCharSelector_S4::~KGCMissionCharSelector_S4( void ){}

void KGCMissionCharSelector_S4::OnCreateComplete( void )
{
    m_iSelectIdx = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
    if( 0 > m_iSelectIdx && m_iSelectIdx > GC_CHAR_NUM )
        m_iSelectIdx = 0;

    for(int iLoop=0; iLoop<GC_CHAR_NUM; ++iLoop)
    {
        if( m_pkChar[ iLoop ][ CHAR_UI_NAME ] && m_pkChar[ iLoop ][ CHAR_UI_FACE ] )
        {
            if( iLoop == m_iSelectIdx )
            {
                m_pkChar[ iLoop ][ CHAR_UI_NAME ]->InitState(true, false);
                m_pkChar[ iLoop ][ CHAR_UI_FACE ]->InitState(true, false);
            }
            else
            {
                m_pkChar[ iLoop ][ CHAR_UI_NAME ]->InitState(false, false);
                m_pkChar[ iLoop ][ CHAR_UI_FACE ]->InitState(false, false);
            }
        }
    }
}

void KGCMissionCharSelector_S4::ActionPerformed( const KActionEvent& event ){}

void KGCMissionCharSelector_S4::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}