#include "stdafx.h"
#include ".\harddamagebigmeteo.h"
#include "Stage.h"


#include "DamageManager.h"
#include "GCCameraOrtho.h"
#include "MapSystem/GCFantasticMap.h"
#include "Monster.h"
#include "KGCRoomManager.h"

CHardDamageBigMeteo::CHardDamageBigMeteo(void)
{
}

CHardDamageBigMeteo::~CHardDamageBigMeteo(void)
{
}

void CHardDamageBigMeteo::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );
}

void CHardDamageBigMeteo::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
    CDamage::NotAngleFrameMove(pDamageInstance);

	if( pDamageInstance->m_Life <= -m_Life_Change )
	{
		if( pDamageInstance->m_Life > -m_Life_Change ) 
			pDamageInstance->m_Life = -m_Life_Change;

		return;
	}

    // 운석일 때는 Contact 체크한다.
    // 운석이 땅에 부딪힘...
    if( pDamageInstance->m_Life < 155 )
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

        if ( fPosY != -1.0f )
        {
            g_KDSound.Play( "43" );
            //아이스 메테오
            if( pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK_ICE ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ICE || 
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ICE ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ICE ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK5_ICE )
            {
                g_MyD3D->MyStg->Color_r = 0;
                g_MyD3D->MyStg->Color_g = 0;
                g_MyD3D->MyStg->Color_b = 130;
            }
            else if( pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK_SHINING ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_SHINING ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_SHINING ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_SHINING ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK5_SHINING )
            {
                g_MyD3D->MyStg->Color_r = 130;
                g_MyD3D->MyStg->Color_g = 30;
                g_MyD3D->MyStg->Color_b = 0;
            }
			else if( pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK_LOVE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_LOVE||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_LOVE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_LOVE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK5_LOVE )
			{
				g_MyD3D->MyStg->Color_r = 130;
				g_MyD3D->MyStg->Color_g = 30;
				g_MyD3D->MyStg->Color_b = 0;
			}
            else if ( pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK_ELIA ||
                    pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ELIA ||
                    pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ELIA ||
                    pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ELIA ||
                    pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK5_ELIA )
            {
                g_MyD3D->MyStg->Color_r = 130;
                g_MyD3D->MyStg->Color_g = 30;
                g_MyD3D->MyStg->Color_b = 0;
            }
            else
            {
                g_MyD3D->MyStg->Color_r = 130;
                g_MyD3D->MyStg->Color_g = 0;
                g_MyD3D->MyStg->Color_b = 0;
            }

            if( pDamageInstance->m_Life > -m_Life_Change ) 
                pDamageInstance->m_Life = -m_Life_Change;

            if( pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2 ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3 ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4 ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ICE ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ICE ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ICE ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_SHINING ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_SHINING ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_SHINING ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_LOVE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_LOVE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_LOVE || 
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_LIFE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_LIFE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_LIFE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_RED ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_RED ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_RED ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_BLUE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_BLUE ||
				pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_BLUE ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_HALLOWEEN ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_HALLOWEEN ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ELIA ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ELIA ||
                pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ELIA )

                return;

            bool bBoundCheck = false;
            if( pDamageInstance->m_IsMonsterDamage )
            {
				MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
				if ( pMonster ) {
					if( pMonster->m_afY[LATENCY_INDEX] > 0.0f &&
						pMonster->m_afX[LATENCY_INDEX] > 0.0f &&
						pMonster->m_afX[LATENCY_INDEX] < g_MyD3D->MyStg->GetXLimit() )
						bBoundCheck = true;
				}
            }
            else
            {
                if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y > 0.0f &&
                    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x > 0.0f &&
                    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x < g_MyD3D->MyStg->GetXLimit() )
                    bBoundCheck = true;
            }

            if( bBoundCheck )
            {
                float fParticleY = 0.2f;
                float fDamageY = 0.0f;
                g_kCamera.SetEarthQuake( 5 );
                CDamageInstance* pBoom = NULL;
                if( pDamageInstance->m_IsMonsterDamage )
                    pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM1, pDamageInstance->m_ExtraMonsterNum, false );
                else
                    pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM1, pDamageInstance->m_Who, false );

				if( !pBoom ) return;

                pBoom->m_X = pDamageInstance->m_X;
                pBoom->m_Y = pDamageInstance->m_Y - fDamageY;
                pBoom->m_IsRight = pDamageInstance->m_IsRight;

                if( pDamageInstance->m_IsMonsterDamage )
                    pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM6, pDamageInstance->m_ExtraMonsterNum, false );
                else
                    pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM6, pDamageInstance->m_Who, false );

				if( !pBoom ) return;

                pBoom->m_X = pDamageInstance->m_X;
                pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
                pBoom->m_IsRight = pDamageInstance->m_IsRight;

                if( pDamageInstance->m_IsMonsterDamage )
                    pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM2, pDamageInstance->m_ExtraMonsterNum, false );
                else
                    pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM2, pDamageInstance->m_Who, false );

				if( !pBoom ) return;

                pBoom->m_X = pDamageInstance->m_X;
                pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
                pBoom->m_IsRight = pDamageInstance->m_IsRight;

                if( pDamageInstance->m_IsMonsterDamage )
                    pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM3, pDamageInstance->m_ExtraMonsterNum, false );
                else
                    pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM3, pDamageInstance->m_Who, false );

				if( !pBoom ) return;

                pBoom->m_X = pDamageInstance->m_X;
                pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
                pBoom->m_IsRight = pDamageInstance->m_IsRight;

                if( pDamageInstance->m_IsMonsterDamage )
                    pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM8, pDamageInstance->m_ExtraMonsterNum, false );
                else
                    pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM8, pDamageInstance->m_Who, false );

				if( !pBoom ) return;

                pBoom->m_X = pDamageInstance->m_X;
                pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
                pBoom->m_IsRight = pDamageInstance->m_IsRight;

                if( !pDamageInstance->m_IsRight )
                {
                    if( pDamageInstance->m_IsMonsterDamage )
                        pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM5, pDamageInstance->m_ExtraMonsterNum, false );
                    else
                        pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM5, pDamageInstance->m_Who, false );
                }
                else
                {
                    if( pDamageInstance->m_IsMonsterDamage )
                        pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM4, pDamageInstance->m_ExtraMonsterNum, false );
                    else
                        pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM4, pDamageInstance->m_Who, false );
                }

				if( !pBoom ) return;
                pBoom->m_X = pDamageInstance->m_X;
                pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
                pBoom->m_IsRight = pDamageInstance->m_IsRight;
            }
        }
    }
}

void CHardDamageBigMeteo::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    //pDamageInstance->m_pOriginalDamage->m_x_Speed *= 1.02f;
    //pDamageInstance->m_pOriginalDamage->m_y_Speed *= 1.02f;
}

void CHardDamageBigMeteo::BeginOthers( CDamageInstance* pDamageInstance )
{
    if( pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2 ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3 ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4 ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ICE ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ICE ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ICE ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_SHINING ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_SHINING ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_SHINING ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_LOVE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_LOVE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_LOVE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_LIFE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_LIFE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_LIFE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_RED ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_RED ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_RED ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_BLUE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_BLUE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_BLUE ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_HALLOWEEN ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_HALLOWEEN ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_HALLOWEEN ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ELIA ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ELIA ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ELIA )
        return;

    CDamageInstance* pTempDamageInstance1 = NULL;
    CDamageInstance* pTempDamageInstance2 = NULL;
    CDamageInstance* pTempDamageInstance3 = NULL;

    if( pDamageInstance->m_IsMonsterDamage )
	{
        pTempDamageInstance1 = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK2, pDamageInstance->m_ExtraMonsterNum, false );
        pTempDamageInstance2 = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK3, pDamageInstance->m_ExtraMonsterNum, false );
        pTempDamageInstance3 = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK4, pDamageInstance->m_ExtraMonsterNum, false );
    }
    else
	{
        pTempDamageInstance1 = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK2, pDamageInstance->m_Who, false );
        pTempDamageInstance2 = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK3, pDamageInstance->m_Who, false );
        pTempDamageInstance3 = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK4, pDamageInstance->m_Who, false );
    }

    pTempDamageInstance1->m_X = pDamageInstance->m_X;
    pTempDamageInstance1->m_Y = pDamageInstance->m_Y;
    pTempDamageInstance2->m_X = pDamageInstance->m_X;
    pTempDamageInstance2->m_Y = pDamageInstance->m_Y;
    pTempDamageInstance3->m_X = pDamageInstance->m_X;
    pTempDamageInstance3->m_Y = pDamageInstance->m_Y;

    if( pDamageInstance->m_IsRight != g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight )
    {
        pTempDamageInstance1->m_IsRight = pDamageInstance->m_IsRight;
        pTempDamageInstance2->m_IsRight = pDamageInstance->m_IsRight;
        pTempDamageInstance3->m_IsRight = pDamageInstance->m_IsRight;
    }
    else
    {
        pTempDamageInstance1->m_IsRight = pDamageInstance->m_IsRight;
        pTempDamageInstance2->m_IsRight = pDamageInstance->m_IsRight;
        pTempDamageInstance3->m_IsRight = pDamageInstance->m_IsRight;
    } 
}

void CHardDamageBigMeteo::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    float pTempDamage = m_fDamageToMonster;
	MONSTER* pMonster = g_kMonsterManager.GetMonster(Monsteri);

	if ( pMonster ) {
    // 이 아이 덩치가 너무 커서 -ㅅ-;;; 미리어 다 맞으면 엄청 딿네요 ㅋ
		if ( pMonster->m_iMonsterType == MON_GODOFCIRCULATION ||
			 pMonster->m_iMonsterType == MON_JURIOR ||
			 pMonster->m_iMonsterType == MON_ANTOINETTE ||
			 pMonster->m_iMonsterType == MON_DRAKE_WORM ||
			 pMonster->m_iMonsterType == MON_GIANT ||
			 pMonster->m_iMonsterType == MON_PERIOT ||
			 pMonster->m_iMonsterType == MON_THANATOS_P1 ||
			 pMonster->m_iMonsterType == MON_THANATOS_P2 ||
			 pMonster->m_iMonsterType == MON_THANATOS_P3 ||
			 pMonster->m_iMonsterType == MON_KAZEAZE2 
			 )
		{
			m_fDamageToMonster /= 8.0f;
		}
	}
    CDamage::CrashMonster( pDamageInstance , Monsteri );

    m_fDamageToMonster = pTempDamage;

}

void CHardDamageBigMeteo::Boom( CDamageInstance* pDamageInstance )
{
	float fParticleY = 0.2f;
	float fDamageY = 0.0f;
	g_kCamera.SetEarthQuake( 5 );
	CDamageInstance* pBoom = NULL;
	if( pDamageInstance->m_IsMonsterDamage )
		pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM1, pDamageInstance->m_ExtraMonsterNum, false );
	else
		pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM1, pDamageInstance->m_Who, false );

	if( !pBoom ) return;

	pBoom->m_X = pDamageInstance->m_X;
	pBoom->m_Y = pDamageInstance->m_Y - fDamageY;
	pBoom->m_IsRight = pDamageInstance->m_IsRight;

	if( pDamageInstance->m_IsMonsterDamage )
		pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM6, pDamageInstance->m_ExtraMonsterNum, false );
	else
		pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM6, pDamageInstance->m_Who, false );

	if( !pBoom ) return;

	pBoom->m_X = pDamageInstance->m_X;
	pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
	pBoom->m_IsRight = pDamageInstance->m_IsRight;

	if( pDamageInstance->m_IsMonsterDamage )
		pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM2, pDamageInstance->m_ExtraMonsterNum, false );
	else
		pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM2, pDamageInstance->m_Who, false );

	if( !pBoom ) return;

	pBoom->m_X = pDamageInstance->m_X;
	pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
	pBoom->m_IsRight = pDamageInstance->m_IsRight;

	if( pDamageInstance->m_IsMonsterDamage )
		pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM3, pDamageInstance->m_ExtraMonsterNum, false );
	else
		pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM3, pDamageInstance->m_Who, false );

	if( !pBoom ) return;

	pBoom->m_X = pDamageInstance->m_X;
	pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
	pBoom->m_IsRight = pDamageInstance->m_IsRight;

	if( pDamageInstance->m_IsMonsterDamage )
		pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM8, pDamageInstance->m_ExtraMonsterNum, false );
	else
		pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM8, pDamageInstance->m_Who, false );

	if( !pBoom ) return;

	pBoom->m_X = pDamageInstance->m_X;
	pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
	pBoom->m_IsRight = pDamageInstance->m_IsRight;

	if( !pDamageInstance->m_IsRight )
	{
		if( pDamageInstance->m_IsMonsterDamage )
			pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM5, pDamageInstance->m_ExtraMonsterNum, false );
		else
			pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM5, pDamageInstance->m_Who, false );
	}
	else
	{
		if( pDamageInstance->m_IsMonsterDamage )
			pBoom = g_MyD3D->m_pDamageManager->MonsterAdd( DT_ARME4_SPECIAL3_BLACK_BOOM4, pDamageInstance->m_ExtraMonsterNum, false );
		else
			pBoom = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL3_BLACK_BOOM4, pDamageInstance->m_Who, false );
	}
	
	if( !pBoom ) return;

	pBoom->m_X = pDamageInstance->m_X;
	pBoom->m_Y = pDamageInstance->m_Y - fParticleY;
	pBoom->m_IsRight = pDamageInstance->m_IsRight;
}

void CHardDamageBigMeteo::End( CDamageInstance* pDamageInstance )
{
	CDamage::End( pDamageInstance );

	if( pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2 ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3 ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4 ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ICE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ICE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ICE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_SHINING ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_SHINING ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_SHINING ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_LOVE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_LOVE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_LOVE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_LIFE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_LIFE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_LIFE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_RED ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_RED ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_RED ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_BLUE ||
		pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_BLUE ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_BLUE ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_HALLOWEEN ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_HALLOWEEN ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_HALLOWEEN ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2_ELIA ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3_ELIA ||
        pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4_ELIA )
		return;

	Boom( pDamageInstance );
}
