#include "stdafx.h"
////
//
//#include "GCMonsterLink.h"
//
//
//
//
//
//
//
//
//IMPLEMENT_GAME_OBJ_FACTORY( KGCMonsterLink );
//IMPLEMENT_GAME_OBJ_FACTORY_NAME( KGCMonsterLink, "monster_link" );
//
//KGCMonsterLink::KGCMonsterLink(void)
//{
//	m_pNormal = NULL;
//	m_pCrash = NULL;
//	m_pLockParticle = NULL;
//	m_pRebirth = NULL;
//
//	m_fRebirthTime = 0.0f;
//	m_fElapsedTime = 0.0f;
//	m_fMonPosX = 0.0f;
//	m_fMonPosY = 0.0f;
//	m_bLock = true;
//	m_bComplete = false;
//
//	m_strColor = "";
//
//	m_iDoor = -1;
//}
//
//KGCMonsterLink::~KGCMonsterLink(void)
//{
//}
//
//void KGCMonsterLink::Create( void )
//{
//	//애니메이션 설정
//	if( !m_strColor.compare("BGREEN") )
//	{
//		m_pNormal = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "MonsterLink1_Normal" ) );
//		m_pCrash  = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "MonsterLink1_Crash" ) );
//	}
//	else if( !m_strColor.compare("BLACK") )
//	{
//		m_pNormal = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "MonsterLink2_Normal" ) );
//		m_pCrash  = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "MonsterLink2_Crash" ) );
//	}
//	else if( !m_strColor.compare("VIOLET") )
//	{
//		m_pNormal = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "MonsterLink3_Normal" ) );
//		m_pCrash  = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "MonsterLink3_Crash" ) );
//	}
//
//	D3DXVECTOR2 pos = g_MyD3D->MyStg->Check_SafePostionX( g_MyD3D->MyStg->Num_Width * 1.5f * GetPosX(), true, GetPosY() );
//
//	//위치 / 애니메이션 상태 설정
//	m_pNormal->GetMatrix()->Move( pos.x - 1.0f, pos.y - 0.25f, 1.9f );
//	m_pNormal->GetMatrix()->Scale( 0.0015f, 0.0015f, 1.0f );
//
//	m_pCrash->GetMatrix()->Move( pos.x - 1.0f, pos.y - 0.25f, 1.9f );
//	m_pCrash->GetMatrix()->Scale( 0.0015f, 0.0015f, 1.0f );
//
//	m_pLockParticle = g_ParticleManager->CreateSequence( "DoorLock", pos.x - 1.0f, pos.y - 0.2f, 1.9f );
//
//	//초기값 설정
//	//SetIndex( g_MyD3D->AddMonster( MON_WOOD_DOOR, GetPosX(), GetPosY(), IsRight() ) );
//	SetIndex( g_MyD3D->AddMonster( "", MON_WOOD_DOOR, m_iLv, GetPosX(), GetPosY(), IsRight() ) );
//	SetMaxHP( g_kMonsterManager.m_mapMonster[GetIndex()]->m_fHP );
//	SetNowHP( g_kMonsterManager.m_mapMonster[GetIndex()]->m_fHP );
//
//	//초기에 Lock상태라면
//	if ( false == IsInitEnable() )
//	{
//		Lock( true );
//	}
//	else
//	{
//		Lock( false );
//	}
//}
//
//void KGCMonsterLink::Destroy( void )
//{
//	g_MyD3D->m_pMapAnim->DeleteInst( m_pNormal );
//	g_MyD3D->m_pMapAnim->DeleteInst( m_pCrash );
//}
//
//void KGCMonsterLink::FrameMove( float fElapsedTime )
//{
//	//완료된 상태
//	if( m_bComplete )
//		return;
//
//	//잠긴상태에서 연결된 몬스터가 죽었다면 UnLock
//	if( m_bLock && g_kMonsterManager.m_mapMonster[ m_iMonster ]->m_fHP <= 0 ){
//		Lock( false );
//		m_fMonPosX = g_kMonsterManager.m_mapMonster[ m_iMonster ]->m_afX[0];
//		m_fMonPosY = g_kMonsterManager.m_mapMonster[ m_iMonster ]->m_afY[0] + 0.3f;
//	}
//
//	//Lock상태라면 더이상 수행하지 않는다
//	if( m_bLock )
//		return;
//
//	//정해진 시간이 지날동안 장치를 파괴하지 못할경우 
//	//몬스터를 다시 재생하고 장치를 다시 Lock
//	m_fElapsedTime += fElapsedTime;
//
//	if( m_fElapsedTime > m_fRebirthTime - 0.4f )
//	{
//		m_pRebirth->SetShow( true );
//
//		g_KDSound.Play( "47" );
//
//		if( m_pRebirth->GetNumFreeParticles() > 0 ) 
//		{
//			//float fX, fY;
//			//if( g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster == MON_KAKIRU_HEAD )
//			//{
//			//	fX = 0.5f;
//			//	fY = 1.1f;
//			//}
//			//else if( g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster == MON_KAKIRU_LEFT )
//			//{
//			//	fX = 0.7f;
//			//	fY = 0.8f;
//			//}
//			//else if( g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster == MON_KAKIRU_RIGHT )
//			//{
//			//	fX = 0.3f;
//			//	fY = 0.8f;
//			//}
//
//			//g_ParticleManager->CreateParticle( m_pRebirth, 1, D3DXVECTOR3( fX, fY, 1.9f ) );
//		}
//	}
//
//	if( m_fElapsedTime > m_fRebirthTime && GetNowHP() > 0)
//	{
//		Lock( true );
//	}
//	else
//	{
//		SetNowHP( g_kMonsterManager.m_mapMonster[GetIndex()]->m_fHP );
//
//		//파괴된 상태
//		if ( GetNowHP() <= 0.0f )
//		{
//			m_pNormal->Stop();
//			m_pNormal->SetShow( false );
//			m_pCrash->Start( CKTDGAnim::PT_LOOP );
//			m_pCrash->SetShow( true );
//
//			if( g_kMonsterManager.m_mapMonster[ m_iMonster ]->m_fHP <= 0 )
//			{
//                // Jaeho.Ready
//				//m_bComplete = true;
//				//
//				////자신이 파괴되었으므로 성문에 불을 켠다
//				//KGCKakiruDoor* pDoor = (KGCKakiruDoor*)g_MyD3D->m_pkQuestGameMgr->GetGameObject( m_iDoor );
//				////pDoor->LightOn( g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster );
//			}
//		}
//	}
//}
//
//void KGCMonsterLink::Lock( bool bLock )
//{
//	if( bLock )
//	{
//		//몬스터 되살리기
//		//g_kMonsterManager.m_mapMonster[ m_iMonster ]->Wait_Room( g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster, m_fMonPosX, m_fMonPosY );
//		//g_MyD3D->m_AITemplet.ResetAIObject( m_iMonster );
//		g_kMonsterManager.m_mapMonster[ m_iMonster ]->m_bLive = true;
//
//		//m_fHP를 최대값으로 복구
//		g_kMonsterManager.m_mapMonster[ GetIndex() ]->m_fHP = GetMaxHP();
//		SetNowHP( GetMaxHP() );
//
//		//잠김상태로 설정
//		g_kMonsterManager.m_mapMonster[ GetIndex() ]->m_bLive = false;
//		m_pLockParticle->SetShow( true );
//		m_bLock = true;
//
//		m_pNormal->SetShow( true );
//		m_pNormal->Start( CKTDGAnim::PT_LOOP );
//		m_pCrash->Stop();
//		m_pCrash->SetShow( false );
//
//		m_fElapsedTime = 0.0f;
//	}
//	else
//	{
//		g_kMonsterManager.m_mapMonster[GetIndex()]->m_bLive = true;
//		m_pLockParticle->SetShow( false );
//		m_bLock = false;
//
//		g_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( STR_ID_GAMEMSG_KAKIRU_DESTROY ) );
//	}
//}
//
//void KGCMonsterLink::Render( void )
//{
//}
//
//void KGCMonsterLink::SetMonster( int iMonster )
//{
//	m_iMonster = iMonster;
//
//	//if(g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster == MON_KAKIRU_HEAD)
//	//	m_pRebirth = g_ParticleManager->CreateSequence( "KakiruHeadRebirth", 0.0f, 0.0f, 1.9f );
//	//else if(g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster == MON_KAKIRU_LEFT)
//	//	m_pRebirth = g_ParticleManager->CreateSequence( "KakiruLeftRebirth", 0.0f, 0.0f, 1.9f );
//	//else if(g_kMonsterManager.m_mapMonster[ m_iMonster ]->Now_Monster == MON_KAKIRU_RIGHT)
//	//	m_pRebirth = g_ParticleManager->CreateSequence( "KakiruRightRebirth", 0.0f, 0.0f, 1.9f );
//}
