#include "stdafx.h"
#include ".\harddamagegas.h"
#include "KGCRoomManager.h"

static float g_fGasRandom[][3] = // 가스 난수표
{
	{   0.353343f,  0.386029f,  0.399091f,  },
	{   0.427381f,  0.016419f,  0.98645f,   },
	{   0.929319f,  0.175756f,  0.41142f,   },
	{   0.593707f,  0.89935f,   0.597644f,  },
	{   0.975768f,  0.252907f,  0.262978f,  },
	{   0.952788f,  0.412061f,  0.833308f,  },
	{   0.0585345f, 0.663503f,  0.170354f,  },
	{   0.774438f,  0.399274f,  0.548235f,  },
	{   0.54207f,   0.983917f,  0.196936f,  },
	{   0.315561f,  0.747734f,  0.408155f,  },
};

CHardDamageGas::CHardDamageGas(void)
{
	m_fOrgAngle = 0.0f;
}

CHardDamageGas::~CHardDamageGas(void)
{
}


void CHardDamageGas::BeginWithLocate(CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	CDamage::BeginWithLocate(pDamageInstance,fStartX,fStartY);

    if( pDamageInstance->m_What != DT_CASTUL_TRAP_GAS2 && pDamageInstance->m_What != DT_ANTOINETTE_POISON_GAS )
    {
	    static int cnt = 0;
	    float ang = 2.0f * D3DX_PI * g_fGasRandom[cnt][0];
	    float len = 0.4f * g_fGasRandom[cnt][1];
	    pDamageInstance->m_X += 0.6f * len * ::cosf( ang );
	    pDamageInstance->m_Y += 0.3f * len * ::sinf( ang );
	    pDamageInstance->m_Scale_x += 0.25f * g_fGasRandom[cnt][2];
	    pDamageInstance->m_Angle += ang;
	    m_fOrgAngle = ang; //변수에 초기각을 저장해둔다.
	    if ( ++cnt == 10 ) 
		    cnt = 0;
    }
}

void CHardDamageGas::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::NotAngleFrameMove(pDamageInstance);

    if( pDamageInstance->m_What != DT_CASTUL_TRAP_GAS2 && pDamageInstance->m_What != DT_ANTOINETTE_POISON_GAS )
    {
	    pDamageInstance->m_Angle += 0.3f * ( pDamageInstance->m_Scale_x - 0.15f );
	    pDamageInstance->m_X += 0.001f * cosf( m_fOrgAngle ) * pDamageInstance->m_Scale_x;
	    pDamageInstance->m_Y += 0.001f * sinf( m_fOrgAngle ) * pDamageInstance->m_Scale_x;
    }
}

bool CHardDamageGas::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || ( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() ) ||pDamageInstance->m_What == DT_PET_KUMI_ATTACK_1 )
    {
        if( pDamageInstance->m_Life < pDamageInstance->m_pOriginalDamage->m_Start_Life - 50||pDamageInstance->m_What == DT_PET_KUMI_ATTACK_1 ) 
        {
            // 연금술사 가스만 슬로우 걸린다
            if( pDamageInstance->m_What == DT_GAS || pDamageInstance->m_What == DT_ANTOINETTE_POISON_GAS ||pDamageInstance->m_What == DT_PET_KUMI_ATTACK_1 )
            {
                PLAYER::DamageInfo damage;
                damage.SetSpecialAttack( pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
                damage.SetDamage( m_fDamageToPlayer );
                damage.SetDamageInstanceAttacker( pDamageInstance );
                g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );

				if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ABILITY_IGNORE_HASTE] <= 0.0f )
					g_MyD3D->MyPlayer[hitWho]->Item_Remain_Time[GC_GAME_ITEM_HASTE] = 20;
            }
            else
            {
                // 방독면 착용 여부..
                if( g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[hitWho]->m_kUserInfo.GetCurrentChar(), GASMASK_ITEM_ID, true ) == false &&
                    g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[hitWho]->m_kUserInfo.GetCurrentChar(), GASMASK_ITEM_ID_SEASON4, true ) == false )
                {
                    PLAYER::DamageInfo damage;
                    damage.SetDamage( m_fDamageToPlayer );
                    damage.SetDamageInstance( pDamageInstance );                
                    
                    g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );

                    // 바로 죽이기 -_-
                    if( ( m_dwDMAttribute & DA_KILLING_DAMAGE ) && g_MyD3D->MyPlayer[hitWho]->GetHP() == 0 )
                    {
                        g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                    }
                }
            }
        }
    }
    return true;
}
