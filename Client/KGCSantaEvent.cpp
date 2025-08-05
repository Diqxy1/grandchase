#include "stdafx.h"
#include ".\kgcsantaevent.h"
//
#include "MyD3D.h"


#include "Item.h"
#include "Message.h"
#include "gcui/GCGameOverlayUI.h"
#include "QuestGameManager.h"
#include "KGCRoomManager.h"
#define         SANTA_GIFT_DROP_TIME            1.5f

KGCSantaEvent::KGCSantaEvent(void)
{
    Init();
}

KGCSantaEvent::~KGCSantaEvent(void)
{
    Delete();
}
void KGCSantaEvent::Init()
{
    m_pSantaAnim    = NULL;
    m_bLive = false;
#ifndef _DEBUG
	m_fEventTime        = 30.0f;
#else
	m_fEventTime        = 3.0f;
#endif
    m_fAccumulateTime   = 0.0f;
    m_fGiftDropTime     = 0.0f;
    m_cAppearCount      = 0;
	memset( &m_vGap, 0, sizeof(D3DXVECTOR2) );
    memset( &m_vCurPos, 0, sizeof(D3DXVECTOR2) );
    memset( &m_vEndPos, 0, sizeof(D3DXVECTOR2) );
}
void KGCSantaEvent::Delete()
{
    if ( g_MyD3D != NULL && g_MyD3D->m_pMapAnim != NULL )
    {
        g_MyD3D->m_pUIAnim->DeleteInst( m_pSantaAnim );
        m_pSantaAnim = NULL;
        m_bLive = false;
        m_fAccumulateTime = 0.0f;
        g_ParticleManager->DeleteSequence( m_pSantaEff );
    }
}

void KGCSantaEvent::Create( D3DXVECTOR2 vStartPos_, D3DXVECTOR2 vEndPos_ )
{
    if ( g_MyD3D == NULL && g_MyD3D->m_pMapAnim == NULL || m_bLive == true )
        return;
    m_vCurPos = vStartPos_;
    m_vEndPos = vEndPos_;
	m_vGap.x = ( m_vEndPos.x - m_vCurPos.x )/200.0f;
	m_vGap.y = ( m_vEndPos.y - m_vCurPos.y )/200.0f;
    m_pSantaAnim = g_MyD3D->m_pUIAnim->CreateInst( g_MyD3D->m_pUIAnim->GetSequence( "SantaEvent" ) );
    float fScaleFactor = m_vCurPos.x < m_vEndPos.x ? -1.0f : 1.0f;
    float fScale = 0.003f;
    m_pSantaAnim->GetMatrix()->Scale( fScale*fScaleFactor, fScale, 1.0f );
    if ( g_kGlobalValue.m_kUserInfo.bHost == false )
    {
        m_pSantaAnim->SetShow( true );
        m_pSantaAnim->Start( CKTDGAnim::PT_ONE_PASS );
        m_iLatency = -1;
    }
    else
    {
        m_pSantaAnim->SetShow( false );
        m_iLatency = LATENCY;
    }
    m_fGiftDropTime = 0.0f;
    m_bLive = true; 
    m_pSantaEff = g_ParticleManager->CreateSequence( "SantaEff", 0.0f, 0.0f, 0.0f );
}

void KGCSantaEvent::FrameMove( float fElapsedTime_ )
{
	// 로딩중이 아니고, 게임이 끝난게 아니고, 플레이어들이 움직일 수 있을때 ( 드라마, 대사 ) 만 프레임 무브 돌리쟈~!
	if ( false == g_pkGameOverlayUI->IsShowQuestTitle() &&
		false == g_pkQuestManager->m_bEndGame &&
		true == g_pkQuestManager->IsEnableMovement() &&
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
	{
		m_fAccumulateTime += fElapsedTime_;
		if (  m_fEventTime < m_fAccumulateTime )
		{
			SendP2PPacket( GC_PID_BROAD_SANTA_EVENT );
			m_fAccumulateTime = 0.0f;
		}
		if ( m_bLive == false )
			return;
		if ( m_iLatency > 0 )
		{
			m_iLatency--;
			return;
		}
		else if ( m_iLatency == 0 )
		{
			m_pSantaAnim->SetShow( true );
			m_pSantaAnim->Start( CKTDGAnim::PT_ONE_PASS );
			m_iLatency = -1;
		}

		if ( m_pSantaAnim == NULL )
			return;
		if ( g_kGlobalValue.m_kUserInfo.bHost == true )
		{
			m_fGiftDropTime += fElapsedTime_;
			if ( SANTA_GIFT_DROP_TIME <= m_fGiftDropTime )
			{
				m_fGiftDropTime = 0.0f;
				g_MyD3D->MyItms->Add( GC_GAME_ITEM_SANTA_GIFT, m_vCurPos, false, 3000 );
			}
		}
		m_pSantaAnim->GetMatrix()->Move( m_vCurPos.x - 1.0f , m_vCurPos.y - 0.5f , 0.5f );
		m_vCurPos += m_vGap;

		if ( m_pSantaEff != NULL && g_ParticleManager->IsLiveInstance(m_pSantaEff ) == true )
		{
			m_pSantaEff->SetEmitRate( CMinMax<float>(10, 15) );
			m_pSantaEff->SetPosition( D3DXVECTOR3( m_vCurPos.x - 1.0f ,m_vCurPos.y - 0.5f - 0.05f ,0.5f ) );
			m_pSantaEff->SetShow( true );
		}

		if ( abs( m_vEndPos.x - ( m_vCurPos.x ) ) < abs( m_vGap.x ) )
		{
			Delete();
		}
	}
	else
	{
		if ( m_pSantaEff != NULL && g_ParticleManager->IsLiveInstance(m_pSantaEff ) == true )
		{
			m_pSantaEff->SetShow( false );
		}

		if( m_pSantaAnim )
		{
			m_pSantaAnim->SetShow( false );
		}
	}
}
