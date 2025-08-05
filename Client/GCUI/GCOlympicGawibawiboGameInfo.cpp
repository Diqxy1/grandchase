#include "stdafx.h"
#include "GCOlympicGawibawiboGameInfo.h"

IMPLEMENT_CLASSNAME( KGCOlympicGawibawiboGameInfo );
IMPLEMENT_WND_FACTORY( KGCOlympicGawibawiboGameInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicGawibawiboGameInfo, "gc_gawibawibo_game_info_box_olympic" );

KGCOlympicGawibawiboGameInfo::KGCOlympicGawibawiboGameInfo( void )
:	m_pkBtnChallenge(NULL)
,	m_iTryPoint(0)
,   m_tmTodayDate(0)
,   m_nGameType(0)
,   m_nTeamType(0)
{
    LINK_CONTROL("btn_challenge",m_pkBtnChallenge);

}

KGCOlympicGawibawiboGameInfo::~KGCOlympicGawibawiboGameInfo( void )
{
}

void KGCOlympicGawibawiboGameInfo::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkBtnChallenge )
        {
            if(m_iTryPoint > 0){
                m_ActionClickFunc();
            }
            else
            {
#if defined( NATION_KOREA )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_GAWIBAWIBO_NO_POINT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
#else
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_ERR_2 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
#endif
            }
        }
    }

}

void KGCOlympicGawibawiboGameInfo::OnCreate( void )
{
    m_pkBtnChallenge->InitState(true,true,this);
}

void KGCOlympicGawibawiboGameInfo::SetReqInfo( time_t  tmTodayDate, int nGameType, int nTeamType )
{
      m_tmTodayDate = tmTodayDate;                  
      m_nGameType   = nGameType;                    
      m_nTeamType   = nTeamType;                    
}

void KGCOlympicGawibawiboGameInfo::SetGameInfo( int iConinCount, int iAccumulateData )
{
    m_iTryPoint = iConinCount;
    m_iAccumulateData = iAccumulateData;
}


