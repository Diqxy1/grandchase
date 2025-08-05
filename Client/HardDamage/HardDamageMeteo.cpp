#include "stdafx.h"
#include ".\HardDamageMeteo.h"
#include "Stage.h"
#include "DamageManager.h"
#include "Spark.h"

#include "MapSystem/GCFantasticMap.h"

#include "Monster.h"
#include "KGCRoomManager.h"

CHardDamageMeteo::CHardDamageMeteo(void)
{
}

CHardDamageMeteo::~CHardDamageMeteo(void)
{
}


void CHardDamageMeteo::Begin ( CDamageInstance* pDamageInstance ) 
{
	CDamage::Begin(pDamageInstance);

	if (  pDamageInstance->m_What == DT_WIND_BLADE1 
		||pDamageInstance->m_What == DT_WIND_BLADE2 
		||pDamageInstance->m_What == DT_WIND_BLADE3 )
		return;

    if( pDamageInstance->m_IsMonsterDamage )
    {
		MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
		if( pMonster != NULL ) {
			g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x = pMonster->m_afX[ LATENCY_INDEX ];
			g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y = pMonster->m_afY[ LATENCY_INDEX ];
			g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight = pMonster->m_abIsRight[ LATENCY_INDEX ];
			g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->cFrame = pMonster->m_aiFrame[ LATENCY_INDEX ] + 50;
		}
    }
	
	// 운석일 때...
	if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame < 70 )
	{
		if( pDamageInstance->m_IsRight )
			pDamageInstance->m_X -= (float)( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - 64 ) * 0.6f;
		else 
			pDamageInstance->m_X += (float)( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - 64 ) * 0.6f;
		pDamageInstance->m_Y += (float)( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - 64 ) * 0.2f;
	}
	else
	{
		if( pDamageInstance->m_IsRight) 
			pDamageInstance->m_X -= (float)( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - 70 ) * 0.5f;
		else 
			pDamageInstance->m_X += (float)( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - 70 ) * 0.5f;
		pDamageInstance->m_Y += (float)( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - 70 ) * 0.25f;
	}
}


void CHardDamageMeteo::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::NotAngleFrameMove(pDamageInstance);
	// 운석일 때는 Contact 체크한다.
	// 운석이 땅에 부딪힘...
	if( pDamageInstance->m_Life < 200 )
    {
        float fPosY;
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
            D3DXVECTOR2 dxvCrashPos;
            SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y - 0.2f, dxvCrashPos );
            fPosY = dxvCrashPos.y;
        }
        else
        {
            fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y - 0.2f, 0 ).y;
        }
		if( fPosY != -1.0f )
	    {
		    g_KDSound.Play( "43" );
		    //아이스 메테오
		    if( pDamageInstance->m_What == DT_METEO_ICE )
		    {
			    g_MyD3D->MyStg->Color_r = 0;
			    g_MyD3D->MyStg->Color_g = 0;
			    g_MyD3D->MyStg->Color_b = 130;
		    }
            else if ( pDamageInstance->m_What == DT_METEO_SHINING )
            {
                g_MyD3D->MyStg->Color_r = 130;
                g_MyD3D->MyStg->Color_g = 30;
                g_MyD3D->MyStg->Color_b = 0;
            }
			else if ( pDamageInstance->m_What == DT_METEO_LOVE )
			{
				g_MyD3D->MyStg->Color_r = 130;
				g_MyD3D->MyStg->Color_g = 30;
				g_MyD3D->MyStg->Color_b = 0;
			}
            else if ( pDamageInstance->m_What == DT_METEO_LIFE )
            {
                g_MyD3D->MyStg->Color_r = 0;
                g_MyD3D->MyStg->Color_g = 130;
                g_MyD3D->MyStg->Color_b = 0;
            }
			else if ( pDamageInstance->m_What == DT_METEO_ELIA )
            {
                g_MyD3D->MyStg->Color_r = 130;
                g_MyD3D->MyStg->Color_g = 30;
                g_MyD3D->MyStg->Color_b = 0;
            }
			else if (pDamageInstance->m_What == DT_METEO_C_MASTER)
			{
				g_MyD3D->MyStg->Color_r = 179;
				g_MyD3D->MyStg->Color_g = 136;
				g_MyD3D->MyStg->Color_b = 255;
			}
			else if (pDamageInstance->m_What == DT_METEO_COLISEU)
			{
				g_MyD3D->MyStg->Color_r = 255;
				g_MyD3D->MyStg->Color_g = 255;
				g_MyD3D->MyStg->Color_b = 141;
			}
			else
			{
				g_MyD3D->MyStg->Color_r = 130;
				g_MyD3D->MyStg->Color_g = 0;
				g_MyD3D->MyStg->Color_b = 0;
			}

		    if( pDamageInstance->m_Life > -m_Life_Change ) 
			    pDamageInstance->m_Life = -m_Life_Change;

		    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y > 0 
			    && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x > 0 
			    && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x < g_MyD3D->MyStg->GetXLimit() )
		    {
			    //아이스 메테오
			    if( pDamageInstance->m_What == DT_METEO_ICE )
			    {
				    g_MyD3D->MySparks->IceFires( pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f );
				    g_MyD3D->m_pDamageManager->AddWithLocate( DT_METEO_CRASH_WAVE_ICE, pDamageInstance->m_Who, pDamageInstance->m_X , pDamageInstance->m_Y , false , pDamageInstance->m_ExtraMonsterNum );
			    }
                //빛 메테오
                else if( pDamageInstance->m_What == DT_METEO_SHINING )
                {
                    g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f );
                    g_MyD3D->m_pDamageManager->AddWithLocate( DT_METEO_CRASH_WAVE_SHINING, pDamageInstance->m_Who, pDamageInstance->m_X , pDamageInstance->m_Y , false , pDamageInstance->m_ExtraMonsterNum );
                }
				//러브 메테오
				else if( pDamageInstance->m_What == DT_METEO_LOVE )
				{
					g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f );
					g_MyD3D->m_pDamageManager->AddWithLocate( DT_METEO_CRASH_WAVE_LOVE, pDamageInstance->m_Who, pDamageInstance->m_X , pDamageInstance->m_Y , false , pDamageInstance->m_ExtraMonsterNum );
				}
                // 생명 메테오 
                else if( pDamageInstance->m_What == DT_METEO_LIFE )
                {
                    g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f );
                    g_MyD3D->m_pDamageManager->AddWithLocate( DT_METEO_CRASH_WAVE_LIFE, pDamageInstance->m_Who, pDamageInstance->m_X , pDamageInstance->m_Y , false , pDamageInstance->m_ExtraMonsterNum );
                }
				 //엘리아 메테오
                else if( pDamageInstance->m_What == DT_METEO_ELIA )
                {
                    g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f );
                    g_MyD3D->m_pDamageManager->AddWithLocate( DT_METEO_CRASH_WAVE_ELIA, pDamageInstance->m_Who, pDamageInstance->m_X , pDamageInstance->m_Y , false , pDamageInstance->m_ExtraMonsterNum );
                }
				else if ( pDamageInstance->m_What == DT_METEO_C_MASTER )
				{
					g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f);
					g_MyD3D->m_pDamageManager->AddWithLocate(DT_METEO_CRASH_WAVE_C_MASTER, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, pDamageInstance->m_ExtraMonsterNum );
				}
				else if ( pDamageInstance->m_What == DT_METEO_COLISEU )
				{
					g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f);
					g_MyD3D->m_pDamageManager->AddWithLocate(DT_METEO_CRASH_WAVE_COLISEU, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, pDamageInstance->m_ExtraMonsterNum );
				}
			    else
			    {
				    g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, fPosY - 0.2f, 0, 0.6f );
				    g_MyD3D->m_pDamageManager->AddWithLocate( DT_METEO_CRASH_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X , pDamageInstance->m_Y , false , pDamageInstance->m_ExtraMonsterNum );				
			    }
		    }
	    }
    }
}


void CHardDamageMeteo::AngleFrameMove(CDamageInstance* pDamageInstance)
{
	CDamage::AngleFrameMove(pDamageInstance);

	// 라스 3단필이 이쪽으로 들어오는듯..
	if( pDamageInstance->m_Life < 245 )
    {
        float fPosY;
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
            D3DXVECTOR2 dxvCrashPos;
            SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y - 0.2f, dxvCrashPos );
            fPosY = dxvCrashPos.y;
        }
        else
        {
            fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y - 0.2f, 0 ).y;
        }
		if( fPosY != -1.0f )
	    {
		    g_KDSound.Play( "43" );
		    g_MyD3D->MyStg->Color_r = 0;
		    g_MyD3D->MyStg->Color_g = 0;
		    g_MyD3D->MyStg->Color_b = 130;

		    pDamageInstance->m_Life = 1;

		    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y > 0 
			    && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x > 0 
                && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x < g_MyD3D->MyStg->GetXLimit() )
		    {
			    //아이스 메테오
			    CDamageInstance* pTempDamageInstance = g_MyD3D->m_pDamageManager->Add( DT_WIND_BLADE_CRASH_WAVE_ICE, pDamageInstance->m_Who, false, pDamageInstance->m_ExtraMonsterNum );
                if( pTempDamageInstance != NULL )
                {
			        pTempDamageInstance->m_X = pDamageInstance->m_X;
			        pTempDamageInstance->m_Y = pDamageInstance->m_Y - 0.2f;
			        if( pDamageInstance->m_IsRight == true )
			        {
				        pTempDamageInstance->m_X += 0.3f;
				        g_MyD3D->MySparks->IceFires( pDamageInstance->m_X - 0.7f, fPosY - 0.2f, 0, 0.6f );
			        }
			        else
			        {
				        pTempDamageInstance->m_X -= 0.3f;
				        g_MyD3D->MySparks->IceFires( pDamageInstance->m_X - 1.3f, fPosY - 0.2f, 0, 0.6f );
			        }
                }
		    }
			else
			if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y > 0
				&& g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x > 0
				&& g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x < g_MyD3D->MyStg->GetXLimit())
			{
				CDamageInstance* pTempDamageInstance = g_MyD3D->m_pDamageManager->Add(DT_WIND_BLADE_CRASH_WAVE, pDamageInstance->m_Who, false, pDamageInstance->m_ExtraMonsterNum);
				if (pTempDamageInstance != NULL)
				{
					pTempDamageInstance->m_X = pDamageInstance->m_X;
					pTempDamageInstance->m_Y = pDamageInstance->m_Y - 0.2f;
					if (pDamageInstance->m_IsRight == true)
					{
						pTempDamageInstance->m_X += 0.3f;
						g_MyD3D->MySparks->IceFires(pDamageInstance->m_X - 0.7f, fPosY - 0.2f, 0, 0.6f);
					}
					else
					{
						pTempDamageInstance->m_X -= 0.3f;
						g_MyD3D->MySparks->IceFires(pDamageInstance->m_X - 1.3f, fPosY - 0.2f, 0, 0.6f);
					}
				}
			}
	    }
    }
}

void CHardDamageMeteo::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    float pTempDamage = m_fDamageToMonster;

    // 이 아이 덩치가 너무 커서 -ㅅ-;;; 미리어 다 맞으면 엄청 딿네요 ㅋ
	MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
	if( pMonster != NULL ) {
		if ( pMonster->m_iMonsterType == MON_GODOFCIRCULATION ||
			pMonster->m_iMonsterType == MON_JURIOR ||
			pMonster->m_iMonsterType == MON_ANTOINETTE ||
			pMonster->m_iMonsterType == MON_DRAKE_WORM ||
			pMonster->m_iMonsterType == MON_GIANT ||
			pMonster->m_iMonsterType == MON_PERIOT  )
		{
			m_fDamageToMonster /= 8.0f;
		}
	}
    CDamage::CrashMonster( pDamageInstance , Monsteri );

    m_fDamageToMonster = pTempDamage;
    
}