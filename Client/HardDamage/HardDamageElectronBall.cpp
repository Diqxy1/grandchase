#include "stdafx.h"
#include "HardDamageElectronBall.h"


#include "Monster.h"

CHardDamageElectronicBall::CHardDamageElectronicBall(void)
{
    m_vecFirstHit.clear();
    m_vecTwiceHit.clear();
    m_iSoundOutput = 0;
}

CHardDamageElectronicBall::~CHardDamageElectronicBall(void)
{
    m_vecFirstHit.clear();
    m_vecTwiceHit.clear();
    m_iSoundOutput = 0;
}

void CHardDamageElectronicBall::Begin( CDamageInstance* pDamageInstance )
{
    m_vecFirstHit.clear();
    m_vecTwiceHit.clear();
    m_vecFirstHit.reserve( MAX_PLAYER_NUM + static_cast<int>(g_kMonsterManager.m_mapMonster.size()) );
    m_vecTwiceHit.reserve( MAX_PLAYER_NUM + static_cast<int>(g_kMonsterManager.m_mapMonster.size()) );
    m_iSoundOutput = 0;

    CDamage::Begin( pDamageInstance );
}

bool CHardDamageElectronicBall::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );

    // 2007/2/9. iridology. 이놈이 처음 맞았나?
    std::vector<int>::iterator vit1 = std::find( m_vecFirstHit.begin(), m_vecFirstHit.end(), hitWho );
    if( vit1 == m_vecFirstHit.end() )
    {
        m_vecFirstHit.push_back( hitWho );
        g_KDSound.Play( "8" );
    }
    else
    {
        // 2007/2/9. iridology. 두번째 맞은건가?
        std::vector<int>::iterator vit2 = std::find( m_vecTwiceHit.begin(), m_vecTwiceHit.end(), hitWho );
        if( vit2 == m_vecTwiceHit.end() )
        {
            m_vecTwiceHit.push_back( hitWho );
            g_KDSound.Play( "8" );
        }
        else
        {
            // 카미키 암흑 소울 임팩트!
            if( pDamageInstance->m_What == DT_MONSTER_KAMIKI06 ||
                pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE1_06 ||
                pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE2_06 )
            {
                // 맞은 위치에 따라서 날아가는걸 조절해 주겠다
				MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
				if ( pMonster ) {

					float fX = pMonster->m_afX[LATENCY_INDEX] - g_MyD3D->MyPlayer[hitWho]->vPos.x;
					float fY = pMonster->m_afY[LATENCY_INDEX] - g_MyD3D->MyPlayer[hitWho]->vPos.y;
					float fLength = ASMsqrt( powf( fX, 2.0f ) + powf( fY, 2.0f ) );
					float fAngle = asinf( fY / fLength );

					if( pDamageInstance->m_X + 0.05f < g_MyD3D->MyPlayer[hitWho]->vPos.x )
						g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.07f * cosf( fAngle );
					else
						g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.07f * cosf( fAngle );

					g_MyD3D->MyPlayer[hitWho]->y_Speed = -0.04f * sinf( fAngle ) + 0.03f;
				}
            }
            else
            {
				if ( pDamageInstance->m_What == DT_PERIOT_DASH_ATK ||
					 pDamageInstance->m_What == DT_BLOODYORC_WARRIOR_SHOULDER_ATK )
				{
					if( pDamageInstance->m_IsRight )
						g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.08f;
					else
						g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.08f;
				}
				else
				{
					// 2007/2/9. iridology. 세번째 맞은거다.. 날려주자..
					if( pDamageInstance->m_X + 0.05f < g_MyD3D->MyPlayer[hitWho]->vPos.x )
						g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.08f;
					else
						g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.08f;
				}

                g_MyD3D->MyPlayer[hitWho]->y_Speed = 0.04f;
            }

            g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_COMMON_DOWN_AND_STANDUP );
            g_KDSound.Play( "8" );
        }
    }

    // 카미키 암흑 소울 임팩트!
    //if( pDamageInstance->m_What == DT_MONSTER_KAMIKI06 )
    //{
    //    // 암흑 타격 이펙트를 뿌려주고 싶다
    //    float fEffectX = g_MyD3D->MyPlayer[hitWho]->vPos.x;
    //    float fEffectY = g_MyD3D->MyPlayer[hitWho]->vPos.y;

    //    g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum]->SetDamageEff( fEffectX - 1.0f, fEffectY - 0.5f, 0.7f );
    //}

    return true;
}

void CHardDamageElectronicBall::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
{
    CDamage::CrashMonster( pDamageInstance, Monsteri );

    if( m_iSoundOutput > 5 )
        return;

    m_iSoundOutput++;
    g_KDSound.Play( "8" );
}
