#include "stdafx.h"
#include "GCAgitDiceAction.h"

#include "LatencyTransfer.h"

const float F_DICE_ONE_TIME = 1.0f;


const int N_DICE_ACTION_EFF_NUM = 13;
const int N_DICE_RESULT_1 = 15;
const int N_DICE_RESULT_2 = 16;


KGCAgitDiceAction::KGCAgitDiceAction(void)
{
    m_eObjType = EM_AGIT_OBJECT_DICE_ACTION;
    m_nSound = -1;
    m_nDiceNum1 = 0;
    m_nDiceNum2 = 0;
    m_bActionDice = false;
    m_fDiceTime = 0.0f;
}

KGCAgitDiceAction::~KGCAgitDiceAction(void)
{
    Destroy();
}

void KGCAgitDiceAction::Create( void )
{
    if ( m_nSound != -1 )
    {
        g_KDSound.LoadFromIndex(m_nSound);
    }

    m_nDiceNum1 = 0;
    m_nDiceNum2 = 0;

}

void KGCAgitDiceAction::Destroy( void )
{	
    if ( m_nSound != -1 )
    {
        g_KDSound.Free(m_nSound);
    }

    m_vecEffect.clear();

    for ( size_t i = 0; i < m_vecBasicParticle.size(); ++i ) 
    {
        CParticleEventSeqPTR pParticle = m_vecBasicParticle[i];
        g_ParticleManager->DeleteSequence( pParticle );        
    }

    m_vecBasicParticle.clear();
}

void KGCAgitDiceAction::FrameMove( float fElapsedTime /*= GC_ELAPSED_TIME */ )
{

    if ( !IsLive() )
        return;

    //  주사위 굴리기 진행중이면 더이상 진행 안한다.
    if ( m_bActionDice ) 
    {
        m_fDiceTime += fElapsedTime;

        if ( m_fDiceTime > F_DICE_ONE_TIME ) 
        {
            m_bActionDice = false;
            m_fDiceTime = 0.0f;
        }

        return;
    }
    

    // 아지트 내에서 스페이스키 체크
    if( g_pkInput->IsDown( DIK_SPACE ) && g_pkAgitOverlayUI->GetCursorFocus() != KGCAgitOverlayUI::ECF_CHATBOX )
    {
        for(int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            if ( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive || !g_MyD3D->MyPlayer[i]->IsLocalPlayer() ) 
                continue;

            GCCollisionRect<float> rect;
            if(g_MyD3D->MyPlayer[i]->IsContact &&
                g_MyD3D->MyPlayer[i]->GetCollisionRect().CheckCollision(GetCollisionRect(),&rect))
            {

                m_nDiceNum1 = (rand()%6)+1;
                m_nDiceNum2 = (rand()%6)+1;

                m_bActionDice = true;

                //  Packet 전송
                LTP_AGIT_DICE_OBJECT_STATE kPacket;
                kPacket.m_dwItemUID = m_dwItemUID;
                kPacket.m_dwUserUID = g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID;
                kPacket.m_nDiceNum1 = m_nDiceNum1;
                kPacket.m_nDiceNum2 = m_nDiceNum2;

                g_LatencyTransfer.PushPacket( &kPacket );
            }
        }
    }
}

GCCollisionRect<float> KGCAgitDiceAction::GetCollisionRect( void )
{
    GCCollisionRect<float> kCollisionRect;

    //  Size
    float fHeight = GetHeight();
    float fWidth = GetWidth();

    //  Left 좌표 
    float fX = GetPosX();
    float fY = GetPosY();   

    //  Width를 변경하는 이유는 캐릭터 바운드가 너무 크다.
    if ( fWidth > 0.15f ) 
        fWidth -= 0.15f;
    else
        fWidth = 0.02f;

    fX += fWidth / 2.0f;    


    kCollisionRect.SetRect( fX - 1.0f, fY - 0.25f, fX + fWidth - 1.0f, fY + fHeight * 0.95f - 0.25f );

    return kCollisionRect;
}

void KGCAgitDiceAction::SetEffectName( const std::vector<std::string>& vecEffect )
{
    m_vecEffect = vecEffect;
}

void KGCAgitDiceAction::EnableLive( bool bLive )
{
    if ( bLive )
    {
        ResetBasicEffect();
        SetBasicEffect();
    }
    else
    {
        ResetBasicEffect();
    }
}

void KGCAgitDiceAction::SetBasicEffect()
{
    m_vecBasicParticle.clear();

    float fPosX = GetPosX() - 1.0f + ( GetWidth() / 2.0f );
    float fPosY = GetPosY() - 0.35f;

    for ( int i = 0; i < m_nBasicEffectNum; ++i ) 
    {
        if ( i >= static_cast<int>(m_vecEffect.size()) ) 
            break;

        CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( m_vecEffect[i], fPosX, fPosY, 0.5f ); 
        m_vecBasicParticle.push_back( pParticle );
    }   
}

void KGCAgitDiceAction::ResetBasicEffect()
{
    for ( size_t i = 0; i < m_vecBasicParticle.size(); ++i ) 
    {
        CParticleEventSeqPTR pParticle = m_vecBasicParticle[i];
        g_ParticleManager->DeleteSequence( pParticle );        
    }

    m_vecBasicParticle.clear();
}

void KGCAgitDiceAction::DoAction( const std::vector<DWORD>& vecParam )
{
    if ( vecParam.size() < 3 ) 
        return;

    DWORD dwUserUID = vecParam[0];
    int nDiceNum1 = vecParam[1];
    int nDiceNum2 = vecParam[2];

    PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwUserUID );
    if ( !pPlayer ) 
        return;

    for ( int i = m_nBasicEffectNum; i < m_nBasicEffectNum + N_DICE_ACTION_EFF_NUM; i++ )
    {
        if ( i >= static_cast<int>(m_vecEffect.size()) ) 
            break;

//        pPlayer->AddParticleNoDirectionPosWithTrace( m_vecEffect[i], 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_HEAD ) );
        pPlayer->AddParticleNoDirectionPosWithTrace( const_cast<char*>(m_vecEffect[i].c_str()), 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_HEAD ) );
    }

    if ( m_vecEffect.size() < N_DICE_RESULT_2+1 ) 
        return;


    std::string strTemp1 = boost::str(boost::format("%s%02d")%m_vecEffect[N_DICE_RESULT_1]%nDiceNum1);
    std::string strTemp2 = boost::str(boost::format("%s%02d")%m_vecEffect[N_DICE_RESULT_2]%nDiceNum2);

//    pPlayer->AddParticleNoDirectionPosWithTrace( strTemp1, 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_HEAD ) );
//    pPlayer->AddParticleNoDirectionPosWithTrace( strTemp2, 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_HEAD ) );

    pPlayer->AddParticleNoDirectionPosWithTrace( const_cast<char*>(strTemp1.c_str()), 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_HEAD ) );
    pPlayer->AddParticleNoDirectionPosWithTrace( const_cast<char*>(strTemp2.c_str()), 0, 0.0f, 0.0f, 5.0f, static_cast<int>( EPT_HEAD ) );

    // Sound & Effect 
    if ( m_nSound != -1 )
    {
        g_KDSound.Play( m_nSound, 0  );
    }

}

