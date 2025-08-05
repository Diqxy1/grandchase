//#include "stdafx.h"
//#include ".\harddamagebombwave.h"
//#include "KGCRoomManager.h"
//
//CHardDamageBombWave::CHardDamageBombWave(void)
//{
//}
//
//CHardDamageBombWave::~CHardDamageBombWave(void)
//{
//}
//
//bool CHardDamageBombWave::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
//{
//	// 우리팀이 맞았을 때...                                                        
//	if( (g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())) && pDamageInstance->m_Life > 200 )
//	{
//		// 나의 적이 맞았음...
//		if( (GC_GMT_ALONE_FIGHT == SiKGCRoomManager()->GetGameModeType() && pDamageInstance->m_Who != hitWho) || pDamageInstance->m_iTeam != g_MyD3D->MyPlayer[hitWho]->m_kUserInfo.iTeam )
//        {
//            PLAYER::DamageInfo damage;
//            damage.SetDamage( m_fDamageToPlayer );
//            damage.SetDamageInstanceAttacker( pDamageInstance );
//			g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
//			g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
//		}
//
//		if( pDamageInstance->m_IsRight )
//			g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.01f;
//		else 
//			g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.01f;
//
//		g_MyD3D->MyPlayer[hitWho]->y_Speed = 0.035f;
//
//	}
//
//	return true;
//}
//
