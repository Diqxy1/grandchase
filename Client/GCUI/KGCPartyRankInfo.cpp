#include "stdafx.h"
#include "KGCPartyRankInfo.h"
#include "KGCRankPlayerInfoBar.h"

IMPLEMENT_CLASSNAME( KGCPartyRankInfo );
IMPLEMENT_WND_FACTORY( KGCPartyRankInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCPartyRankInfo, "gc_party_rank_info" );

KGCPartyRankInfo::KGCPartyRankInfo( void )
: m_pPlayerInfoBarWnd(NULL)
{
    LINK_CONTROL("player_info_bar", m_pPlayerInfoBarWnd);

    for( int i = 0 ; i < (int)m_pPlayerInfoBar.size() ; ++i )
    {
        m_pPlayerInfoBar[i] = NULL;
    }

}

KGCPartyRankInfo::~KGCPartyRankInfo( void )
{
}

void KGCPartyRankInfo::OnCreate()
{
    m_pPlayerInfoBarWnd->InitState(true);
    
    float fStartPosX = m_pPlayerInfoBarWnd->GetFixedWindowLocalPos().x;
    float fStartPosY = m_pPlayerInfoBarWnd->GetFixedWindowLocalPos().y;
    
    //길드 정보
    std::vector<KD3DWnd*> vecSlotList 
        = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_game_overlay_ui.stg", "", 
        "game_overlay_ui\\minimap\\party_rank_info\\player_info_bar", true, MAX_PLAYER_NUM );

    if (!vecSlotList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSlot = vecSlotList.begin();
        DWORD dwHeight = (*iterSlot)->GetHeight();
        for ( int i = 0; iterSlot != vecSlotList.end(); ++iterSlot, ++i )
        {
            m_pPlayerInfoBar[i] = static_cast<KGCRankPlayerInfoBar*>(*iterSlot);
            m_pPlayerInfoBar[i]->InitState( false, true, this );

            D3DXVECTOR2 vPos;
            vPos.x = fStartPosX;
            vPos.y = fStartPosY+ static_cast<float>( (i)*(dwHeight+PARTY_RANK_BAR_GAP) );;
            m_pPlayerInfoBar[i]->SetFixedWindowLocalPos(vPos);
        }
    }
}

void KGCPartyRankInfo::ActionPerformed(const KActionEvent& event )
{

}

void KGCPartyRankInfo::FrameMoveInEnabledState()
{

}

void KGCPartyRankInfo::SetPlayerRankInfo()
{
    for( int i = 0 ; i < (int)m_pPlayerInfoBar.size() ; ++i ) {
        bool bRender = false;
        m_pPlayerInfoBar[i]->SetPlayerRankInfo( i , bRender);
        m_pPlayerInfoBar[i]->ToggleRender(bRender);
    }
}
