#include "stdafx.h"
#include "../MyD3D.h"
#include "KGCMacroCommunitySlot.h"

IMPLEMENT_CLASSNAME( KGCMacroCommunitySlot );
IMPLEMENT_WND_FACTORY( KGCMacroCommunitySlot );
IMPLEMENT_WND_FACTORY_NAME( KGCMacroCommunitySlot, "macro_community_slot" );

KGCMacroCommunitySlot::KGCMacroCommunitySlot()
: m_pkFrame( NULL )
, m_pkImage( NULL )
, m_pkCooltime_Pannel( NULL )
, m_MyIndex( 0 )
, m_iItemAlpha( 255 )
, m_iRealCoolTime( 0 )
, m_iMaxCoolTime( 0 )
{
    LINK_CONTROL( "frame", m_pkFrame );
}

KGCMacroCommunitySlot::~KGCMacroCommunitySlot()
{
    SAFE_RELEASE(m_pkImage);
    SAFE_RELEASE(m_pkCooltime_Pannel);
}

void KGCMacroCommunitySlot::OnCreate()
{
}

void KGCMacroCommunitySlot::OnDestroy()
{
}

void KGCMacroCommunitySlot::PostChildDraw()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

        // 커뮤니티 텍스쳐 출력
        if( m_pkImage )
        {
            m_pImageVertex = m_pkFrame->GetReLocatedWindowRect();
            DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
            g_pGCDeviceManager2->DrawInScreen(m_pkImage,
                m_pImageVertex[0].m_vPos.x+2.0f, m_pImageVertex[0].m_vPos.y+2.0f, m_pImageVertex[3].m_vPos.x, m_pImageVertex[3].m_vPos.y,
                0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, dwColor); 
        }

        // 쿨타임이 있을때만 출력
        if( 0 < m_iMaxCoolTime )
        {
            float fCoolTimeRatio = static_cast<float>(m_iRealCoolTime) / static_cast<float>(m_iMaxCoolTime);
            if( 0.0f < fCoolTimeRatio )
            {
                g_pGCDeviceManager2->DrawInScreen(m_pkCooltime_Pannel,
                    m_pImageVertex[3].m_vPos.x, m_pImageVertex[3].m_vPos.y - 1, m_pImageVertex[0].m_vPos.x + 2,
                    ( m_pImageVertex[0].m_vPos.y - 1 )+ (m_pImageVertex[3].m_vPos.y - m_pImageVertex[0].m_vPos.y - 1 ) * ( 1.0f - fCoolTimeRatio ) + 1 ,
                    0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f,D3DCOLOR_ARGB(200,0,0,0));	  
            }
        }
    }
    g_pGCDeviceManager2->PopState();
}

void KGCMacroCommunitySlot::ActionPerformed(const KActionEvent& event )
{
}

void KGCMacroCommunitySlot::FrameMoveInEnabledState()
{
}

void KGCMacroCommunitySlot::CoolTimeFrameMove()
{
    if( 0 < m_iRealCoolTime )
        --m_iRealCoolTime;
    else
        m_iMaxCoolTime = 0;
}

void KGCMacroCommunitySlot::InitData()
{
    SAFE_RELEASE(m_pkImage);
    SAFE_RELEASE(m_pkCooltime_Pannel);

    if( 0 < static_cast<int>(strlen(m_sMacroCommunityInfo.m_strTexture.c_str())) )
    {
        m_pkImage = g_pGCDeviceManager2->CreateTexture( m_sMacroCommunityInfo.m_strTexture.c_str() );
    }
    
    m_pkCooltime_Pannel = g_pGCDeviceManager2->GetNullTexture();
}

bool KGCMacroCommunitySlot::SendCommunityMsg()
{
    if( 0 < m_iRealCoolTime )
        return false;

    KGC_PID_BROAD_MACRO_COMMUNITY kPacket;
    kPacket.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
    kPacket.iCommunitySlot = m_MyIndex;
    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_MACRO_COMMUNITY" );

    return true;
}

void KGCMacroCommunitySlot::SetCoolTime( int iCoolTime )
{
    m_iMaxCoolTime = iCoolTime;
    m_iRealCoolTime = iCoolTime;
}