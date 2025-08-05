#include "stdafx.h"
#include ".\HardDamageOctusWarcry.h"
#include "Stage.h"
#include "DamageManager.h"



#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"

// NeoGenesis Quest1 16 Stage만 나오는거.
static float s_fContactY[] = { 1.29f, 0.94f, 0.55f, 0.19f, 1.0f, 1.39f, 0.65f };
static int iDustCnt = 2;

CHardDamageOctusWarcry::CHardDamageOctusWarcry(void)
{
    iDustCnt = 2;
}

CHardDamageOctusWarcry::~CHardDamageOctusWarcry(void)
{
}

void CHardDamageOctusWarcry::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
    CDamage::NotAngleFrameMove(pDamageInstance);

    if( ++iDustCnt == 3 )
    {
        float fPosY;
        for( int i = 0; i < sizeof(s_fContactY)/sizeof(s_fContactY[0]); ++i )
        {
            if( SiKGCRoomManager()->IsMonsterGameMode() )
            {
                D3DXVECTOR2 dxvCrashPos;
                SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, s_fContactY[i], dxvCrashPos );
                fPosY = dxvCrashPos.y;
            }
            else
            {
                fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, s_fContactY[i], 0 ).y;
            }

            if( fPosY != -1.0f )
            {
                g_MyD3D->m_pDamageManager->EffectDust( pDamageInstance->m_X - 1.0f, s_fContactY[i] - 0.2f, pDamageInstance );
            }
        }
        iDustCnt = 0;
	}
}
