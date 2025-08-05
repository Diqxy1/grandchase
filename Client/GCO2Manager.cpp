#include "stdafx.h"
//
//

#include "Monster.h"
#include "GCO2Manager.h"

#include "GCCameraOrtho.h"
#include "KGCRoomManager.h"

KGCO2Region::KGCO2Region()
{
    m_eObjType          = GC_OBJ_O2ZONE;
	m_fPosX = m_fPosY = m_fRadius = 0.0f;
	m_pO2Zone       = NULL;
    m_Tex_O2Zone    = NULL;
    m_fWidth = 0.0f;
    m_fHeight = 0.0f;
}

KGCO2Region::~KGCO2Region()
{
    SAFE_RELEASE(m_Tex_O2Zone);
}

void KGCO2Region::Init(float PosX, float PosY, float Radius)
{
	m_fPosX = PosX;
	m_fPosY = PosY;
	m_fRadius = Radius;

	m_pO2Zone = g_ParticleManager->CreateSequence( "O2Zone", m_fPosX - 1.0f, m_fPosY - 0.4f, 0.5f );
    if( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST25 )
        m_Tex_O2Zone = g_pGCDeviceManager2->CreateTexture( "O2_Zone2.dds" );
    else
        m_Tex_O2Zone = g_pGCDeviceManager2->CreateTexture( "O2_Zone.dds" );
}


bool KGCO2Region::IsIn(int iPlayerIdx)
{
	if(!g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.bLive)
		return false;

	if( ASMsqrt( pow( g_MyD3D->MyPlayer[iPlayerIdx]->vPos.x - m_fPosX,2) 
			+ pow( g_MyD3D->MyPlayer[iPlayerIdx]->vPos.y - m_fPosY,2) ) < m_fRadius * 2.0f )
        return true;
    return false;	
}

void KGCO2Region::Create( void )
{
}

void KGCO2Region::Destroy( void )
{
}

void KGCO2Region::Enable( void )
{

}

void KGCO2Region::FrameMove( float fElapsedTime )
{

}

void KGCO2Region::Render( void )
{
    /*if( m_Tex_O2Zone ) m_Tex_O2Zone->SetDeviceTexture();

    g_pGCDeviceManager2->SetRenderStateSet();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_kCamera.m_matCamera );
        g_MyD3D->Draw_Billboard3( D3DXVECTOR3(m_fPosX - 1.45f, m_fPosY - 0.5f, 3.0f)
                                , D3DXVECTOR3(m_fPosX - 1.45f, m_fPosY+m_fHeight - 0.5f, 3.0f)
                                , D3DXVECTOR3(m_fPosX+m_fWidth - 1.44f, m_fPosY - 0.5f, 3.0f)
                                , D3DXVECTOR3(m_fPosX+m_fWidth - 1.44f, m_fPosY+m_fHeight - 0.5f, 3.0f)
                                 );
        D3DXMATRIX matWorld;
        D3DXMatrixIdentity( &matWorld );
        g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    }
    KD3DDeviceState::PopRenderStateSet();*/

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


	g_pGCDeviceManager2->DrawInWorld(m_Tex_O2Zone,&g_kCamera.m_matCamera,
		D3DXVECTOR3(m_fPosX - 1.45f, m_fPosY - 0.5f, 3.0f)
		, D3DXVECTOR3(m_fPosX - 1.45f, m_fPosY+m_fHeight - 0.5f, 3.0f)
		, D3DXVECTOR3(m_fPosX+m_fWidth - 1.44f, m_fPosY - 0.5f, 3.0f)
		, D3DXVECTOR3(m_fPosX+m_fWidth - 1.44f, m_fPosY+m_fHeight - 0.5f, 3.0f));
   g_pGCDeviceManager2->PopState();
}

KGCO2Manager::KGCO2Manager()
{
	m_bEqipO2Mask = false;
	m_pWaterImpact = NULL;
	m_fWaterHeight = 0.0f;
    for( int i = 0 ; i < (int)m_bInWaterNow.size() ; ++i )
    {
        m_bInWaterNow[i] = false;
    }
    for( int i = 0 ; i < (int)m_bInWaterOld.size() ; ++i )
    {
        m_bInWaterOld[i] = false;
    }

	g_kMonsterManager.CreateMonsterBoolState(m_mapInWaterMonsterNow);
	g_kMonsterManager.CreateMonsterBoolState(m_mapInWaterMonsterOld);

}

KGCO2Manager::~KGCO2Manager()
{
	Destroy();
}

void KGCO2Manager::Create()
{
	if( g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE && g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE_LOADING && g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE_END)
	{
		m_pWaterImpact = g_ParticleManager->CreateSequence( "WaterImpact", 0,0,0.5f );
	
		int iMyPlayerIdx = g_MyD3D->Get_MyPlayer();
		if( g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[iMyPlayerIdx]->m_kUserInfo.GetCurrentChar(), O2MASK_ITEM_ID, true ) == true ||
            g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[iMyPlayerIdx]->m_kUserInfo.GetCurrentChar(), O2MASK_ITEM_ID_DISCOUNT, true ) == true ||
            g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[iMyPlayerIdx]->m_kUserInfo.GetCurrentChar(), O2MASK_ITEM_ID_10, true ) == true ||
            g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[iMyPlayerIdx]->m_kUserInfo.GetCurrentChar(), O2MASK_ITEM_ID_SEASON4, true ) == true 
            )		        //산소 마스크가 있으면 O2가 줄지 않음
			m_bEqipO2Mask = true;
		else
			m_bEqipO2Mask = false;
	}
}

void KGCO2Manager::SetWaterHeight( float fHeight )
{
	m_fWaterHeight = fHeight;
}

void KGCO2Manager::Destroy()
{

}

void KGCO2Manager::FrameMove(float fElapsedTime)
{
	UpdatePlayerPosition();

	int iMyPlayerIdx = g_MyD3D->Get_MyPlayer();

    PLAYER* pPlayer = g_MyD3D->MyPlayer[iMyPlayerIdx];
	if( m_bEqipO2Mask == false && pPlayer->CheckInPortal() == false )		//산소 마스크가 있으면 O2가 줄지 않음, 포탈 안에 있어도 안줄어
	{
		if( pPlayer->m_cLife != 0 && 
			(pPlayer->GetHP() >= 0.0f && pPlayer->uiMotion != pPlayer->GetOwnMotion( MID_COMMON_DOWN_AND_STANDUP ) ) )
		{
			if(m_bInWaterNow[iMyPlayerIdx])	//물속
			{
				if(IsInO2Region(iMyPlayerIdx))		//산소 발생 지역안이면.. 산소를 조금씩 회복
				{
#if defined(NATION_PHILIPPINE)
					pPlayer->m_fO2 += fElapsedTime*10.0f;
#else
					pPlayer->m_fO2 += fElapsedTime*3.0f;
#endif
				}
				else					//아니면 산소 감소
				{
#if defined(NATION_PHILIPPINE)
					pPlayer->m_fO2 -= fElapsedTime/2.0f;
#else
					pPlayer->m_fO2 -= fElapsedTime;
#endif
				}
			}
			else						//물 밖이면 매우 빠른 속도로 회복
			{
				pPlayer->m_fO2 += fElapsedTime*10.0f;
			}

			if( pPlayer->m_fO2 > MAX_PLAYER_O2 )
				pPlayer->m_fO2 = MAX_PLAYER_O2;
			else if(pPlayer->m_fO2 <= 0.0f)	//0보다 작으면 죽여버린다
			{
                pPlayer->m_fO2 = 0.0f;

				PLAYER::DamageInfo damage;
				if (pPlayer->m_bHyperArmor==false)
				{
#if defined( LEVEL_DESIGN_STAT_CALC )
					damage.SetDamage(-3.f);
#else
					damage.SetDamage(-0.03f);
#endif
				
					damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_PLAYER, MAX_PLAYER_NUM );
					damage.SetAbilityEffect( false, true );



					pPlayer->Change_HP( damage );

					if(pPlayer->IsContact && pPlayer->GetHP() <= 0.0f)
					{
						pPlayer->cFrame = 0;
						pPlayer->SetPlayerState( MID_COMMON_DOWN_AND_STANDUP );

						if( pPlayer->IsLocalPlayer() )
							SiKGCAchieveManager()->OccurAction( SAchieve::DIE_O2, pPlayer->GetCharType() );
					}
				}                              
			}
		}
	}


	//물 튀김 효과 추가...
	WaterEff();	
}

bool KGCO2Manager::IsInWater(int iPlayerIdx)
{
	// 원시의 물높이가 낮아서 여기 체크해봐야 false리턴한다.
	// 할수 없이 하드코딩...
 	float fOffsetY = 1.0f;
	switch( SiKGCRoomManager()->GetGameMode() )
	{
	case GC_GM_QUEST28:
		fOffsetY = 1.25f;
		break;
	default:
		fOffsetY = 1.0f;
	}

	if( g_MyD3D->MyPlayer[iPlayerIdx]->vPos.y/fOffsetY < m_fWaterHeight - 0.18f )
	{
		return true;
	}
	return false;
}

bool KGCO2Manager::IsInWaterMonster(int iMonsterIdx)
{
	MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.find(iMonsterIdx);

	if(mit == g_kMonsterManager.m_mapMonster.end())
		return false;

	if( mit->second->m_afY[0] < m_fWaterHeight )
	{
		return true;
	}
	return false;
}

bool KGCO2Manager::IsInO2Region(int iPlayerIdx)
{
    for(int i=0;i<MAX_OBJECT;++i)
    {
        if( !g_MyD3D->m_pObject[i] ) continue;
        
        if( GC_OBJ_O2ZONE == g_MyD3D->m_pObject[i]->GetObjType() )
        {
            if( ((KGCO2Region*)g_MyD3D->m_pObject[i])->IsIn( iPlayerIdx ) )
                return true;
        }
    }

    return false;
}

void KGCO2Manager::UpdatePlayerPosition()
{
    m_bInWaterOld = m_bInWaterNow;

	for(int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		if( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
		{		
			continue;
		}
		m_bInWaterNow[i] = IsInWater(i);
	}


	m_mapInWaterMonsterOld = m_mapInWaterMonsterNow;
	m_mapInWaterMonsterNow.clear();
	for( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if( !(mit->second->m_bLive) )
		{
			continue;
		}
		m_mapInWaterMonsterNow[mit->first] = IsInWaterMonster(mit->first);

	}
}

void KGCO2Manager::WaterEff()
{
	D3DXVECTOR3 pos;
	for(int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		if( m_bInWaterNow[i] == m_bInWaterOld[i] )
			continue;

		//////////////////////////////////////////////////////////
		//물 튀기는 이펙트 추가 위치							//
		//////////////////////////////////////////////////////////
		pos.x = g_MyD3D->MyPlayer[i]->vPos.x - 1.0f;
		pos.y = m_fWaterHeight - 0.45f;
		pos.z = 0.5f;

		g_ParticleManager->CreateParticle( m_pWaterImpact, 1, pos );
	}

	for ( std::map<int, bool>::iterator mit = m_mapInWaterMonsterNow.begin(); mit != m_mapInWaterMonsterNow.end() ; ++mit ) {
		std::map<int, bool>::iterator mitOld = m_mapInWaterMonsterOld.find(mit->first);
		if(mitOld == m_mapInWaterMonsterOld.end())
			continue;
		if(mit->second == mitOld->second)
			continue;
		MONSTER* pMonster = g_kMonsterManager.GetMonster( mit->first );
		if(pMonster == NULL ) continue;

		pos.x = pMonster->m_afX[0] - 1.0f;
		pos.y = m_fWaterHeight - 0.45f;
		pos.z = 0.49f;

		g_ParticleManager->CreateParticle( m_pWaterImpact, 1, pos );

	}
}


bool KGCO2Manager::IsInSquareWater(float fy_)
{
	if( fy_ < m_fWaterHeight)
	{
		return true;
	}
	return false;
}
