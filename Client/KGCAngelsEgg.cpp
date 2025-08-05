#include "StdAfx.h"
#include "Message.h"


#include "Headup Display.h"

#include "KMci.h"
#include "KGCGameBoard.h"
#include "KGCAngelsEgg.h"

#include "KGCRoomManager.h"

#include "GCCameraOrtho.h"
#include "Message.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"
#include "Particle/KGCParticleManager.h"

KGCEggObject::KGCEggObject()
: KGC2DObject()
{
	m_fHP = 0.0f;
	m_iTeam = -1;
	m_bBreak = false;
	m_bNeedP2PUpdate = false;
	m_pHPBarTex = NULL;
	m_pHPBarBackTex = NULL;
}

KGCAngelsEgg::KGCAngelsEgg(void)
{
	lua_tinker::class_add<KGCAngelsEgg>( KGCLuabinder::getInstance().GetLuaState(), "KGCAngelsEggClass" );
		lua_tinker::class_def<KGCAngelsEgg>( KGCLuabinder::getInstance().GetLuaState(),  "GetEgg", 		&KGCAngelsEgg::GetEggObject  );;

	lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "KGCAngelsEgg", this );

	lua_tinker::class_add<KGCEggObject>( KGCLuabinder::getInstance().GetLuaState(), "KGCEggObjectClass" );
		lua_tinker::class_def<KGCEggObject>( KGCLuabinder::getInstance().GetLuaState(),  "SetHP", 		&KGCEggObject::SetHP  );
		lua_tinker::class_def<KGCEggObject>( KGCLuabinder::getInstance().GetLuaState(),  "GetHP", 		&KGCEggObject::GetHP  );
		lua_tinker::class_def<KGCEggObject>( KGCLuabinder::getInstance().GetLuaState(),  "SetTeam", 	&KGCEggObject::SetTeam  );
		lua_tinker::class_def<KGCEggObject>( KGCLuabinder::getInstance().GetLuaState(),  "GetTeam", 	&KGCEggObject::GetTeam  );
		lua_tinker::class_def<KGCEggObject>( KGCLuabinder::getInstance().GetLuaState(),  "GetParent", 	&KGCEggObject::GetParent  );
		lua_tinker::class_def<KGCEggObject>( KGCLuabinder::getInstance().GetLuaState(),  "SetBreak", 	&KGCEggObject::SetBreak  );;

	lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "KGCEggObject", this );

}

KGCAngelsEgg::~KGCAngelsEgg(void)
{
	
}

bool KGCEggObject::SendEggInfo()
{
	if( g_kGlobalValue.m_kUserInfo.bHost == false )
		return false;

	if( m_bNeedP2PUpdate == false )
		return false;

	g_kAngelsEggInfo->Type = GC_PID_ANGELS_EGG_INFO;
	g_kAngelsEggInfo->m_ucTeam = m_iTeam;
	g_kAngelsEggInfo->m_ucState = GetCurrTexIndex();
	g_kAngelsEggInfo->m_fHP = GetHP();
	g_kAngelsEggInfo->m_fMaxHP = GetMaxHP();
	g_kAngelsEggInfo->m_bBreak = IsBreak();

	SendNetworkData( GC_PID_ANGELS_EGG_INFO, 0 );

	m_bNeedP2PUpdate = false;

	return true;
}

void KGCEggObject::LoadHPBar()
{
	SAFE_RELEASE(m_pHPBarTex);
	SAFE_RELEASE(m_pHPBarBackTex);
	m_pHPBarTex     = g_pGCDeviceManager2->CreateTexture( "Energybar.dds" );
	m_pHPBarBackTex = g_pGCDeviceManager2->CreateTexture( "EnergyBarBack.dds" );
}

void KGCEggObject::ParticleEffect( int iState )
{
	D3DXVECTOR2 vPos;
	vPos.x = GetParent()->GetPos().x - 0.74f;
	vPos.y = GetParent()->GetPos().y - 0.3f;

	switch ( iState )
	{
	case 0:
		g_ParticleManager->CreateSequence( "snowman_small"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_big" , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 01"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 02" , vPos.x, vPos.y, 0.5f );
		break;
	case 1:
		g_ParticleManager->CreateSequence( "snowman_small"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_small"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_big" , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 01"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 02" , vPos.x, vPos.y, 0.5f );
		break;
	case 2:
		g_ParticleManager->CreateSequence( "snowman_small"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_small"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_big" , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_big" , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 01"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 02" , vPos.x, vPos.y, 0.5f );
		break;
	case 3:
		g_ParticleManager->CreateSequence( "snowman_small"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_big" , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 01"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 02" , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_small"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_big" , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 01"   , vPos.x, vPos.y, 0.5f );
		g_ParticleManager->CreateSequence( "snowman_attack 02" , vPos.x, vPos.y, 0.5f );
        //g_ParticleManager->CreateSequence( "Box_Boom_01" , vPos.x, vPos.y, 0.5f );
        //g_ParticleManager->CreateSequence( "Box_Boom_02" , vPos.x, vPos.y, 0.5f );
        //g_ParticleManager->CreateSequence( "Box_Boom_03" , vPos.x, vPos.y, 0.5f );
        //g_ParticleManager->CreateSequence( "Box_Boom_04" , vPos.x, vPos.y, 0.5f );
		break;
	}
}

void KGCEggObject::HPRender()
{
	if( m_bBreak || !m_pHPBarTex || !m_pHPBarBackTex )
		return;

	D3DXVECTOR3 pos1;
	D3DXVECTOR3 pos2;
	D3DXVECTOR3 pos3;
	D3DXVECTOR3 pos4;

	// back
	{
		float fX = GetPos().x - 0.73f;
		float fY = GetPos().y - 0.5f;

		float fWidth = GetWidth();
		float fHeight = 0.15f;

		pos1.x = fX - fWidth * 0.5f;
		pos1.y = fY - fHeight * 0.5f;
		pos1.z = 0.4f;

		pos2.x = fX - fWidth * 0.5f;
		pos2.y = fY + fHeight * 0.5f; 
		pos2.z = 0.4f;

		pos3.x = fX + fWidth * 0.5f;
		pos3.y = fY - fHeight * 0.5f;
		pos3.z = 0.4f;

		pos4.x = fX + fWidth * 0.5f;
		pos4.y = fY + fHeight * 0.5f;
		pos4.z = 0.4f;		

		fY -= 0.005f; // 2픽셀정도..

		g_pGCDeviceManager2->DrawInWorld(m_pHPBarBackTex, &g_kCamera.m_matCamera,
			pos1, pos2, pos3, pos4);		
	}

	// front
	{
		float fHP       = m_fHP;
		float fHPPoint  = m_fHP_max;

		float fX = GetPos().x - 0.73f;
		float fY = GetPos().y - 0.5f;

		float fWidth = GetWidth();
		float fHeight = 0.15f;

		pos1.x = fX - fWidth * 0.5f;
		pos1.y = fY - fHeight * 0.5f;
		pos1.z = 0.35f;

		pos2.x = fX - fWidth * 0.5f;
		pos2.y = fY + fHeight * 0.5f;
		pos2.z = 0.35f;

		pos3.x = pos1.x + ( fWidth * ( fHP / fHPPoint ) );
		pos3.y = fY - fHeight * 0.5f;
		pos3.z = 0.35f;

		pos4.x = pos1.x + ( fWidth * ( fHP / fHPPoint ) );
		pos4.y = fY + fHeight * 0.5f;
		pos4.z = 0.35f;

		g_pGCDeviceManager2->DrawInWorld(m_pHPBarTex, &g_kCamera.m_matCamera,
			pos1, pos2, pos3, pos4);
	}
}

KGCEggObject* KGCAngelsEgg::GetEggObject( int iTeam )
{
	if( iTeam < 0 || iTeam >= TEAM_NUM )
	{
		ASSERT(!"Invalid Team index in Angels Egg Object" );
		return NULL;
	}

	if( m_apkAngelsEggObject[iTeam] == NULL )
	{
		ASSERT(!"NULL object in array!!!" );
	}

	return m_apkAngelsEggObject[iTeam];
}

HRESULT KGCAngelsEgg::InitAtGame()
{
#if defined( ANGELS_EGG_REFORM )
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        // 알을 몬스터로 만들자

        // 알깨기 
        //SummonAngelsEgg( TEAM_SERDIN, MON_ANGELSEGG, v2AngelsEggPos[TEAM_SERDIN], true );
        //SummonAngelsEgg( TEAM_CANABAN, MON_ANGELSEGG, v2AngelsEggPos[TEAM_CANABAN], true );

        // 할로윈 호박 
        SummonAngelsEgg( TEAM_SERDIN, MON_HALLOWEEN_PUMPKIN, v2AngelsEggPos[TEAM_SERDIN], true, TEAM_SERDIN );
        SummonAngelsEgg( TEAM_CANABAN, MON_HALLOWEEN_PUMPKIN, v2AngelsEggPos[TEAM_CANABAN], true, TEAM_CANABAN );
    }
#endif
	return S_OK;
}

HRESULT KGCAngelsEgg::InitAtLoading()
{
#if defined( ANGELS_EGG_REFORM )
    if( LoadReformScript() == false )
    {
        ASSERT( !"AngelsEgg Script Load Error!!!" );
        return E_FAIL;
    }
#else
    // 알을 2D오브젝트로 만들자
	m_apkAngelsEggObject[TEAM_CANABAN] = (KGCEggObject*)g_MyD3D->Add2DObject( new KGCEggObject() ).second;
	m_apkAngelsEggObject[TEAM_SERDIN] = (KGCEggObject*)g_MyD3D->Add2DObject( new KGCEggObject() ).second;

	// 알 스크립트를 읽어 오자!!
	if( LoadScript() == false )
	{
		ASSERT( !"AngelsEgg Script Load Error!!!" );
		return E_FAIL;
	}

    m_bCheckSetEggHP = false;

	for(int i=0;i<TEAM_NUM;++i)
	{
		m_apkAngelsEggObject[i]->LoadHPBar();
		m_apkAngelsEggObject[i]->SendEggInfo();
	}
#endif
    for ( int i = 0; i < TEAM_NUM; ++i ) 
    {
        m_arrayParticle[i] = NULL;
    }

    for ( int i = 0; i < TEAM_NUM; ++i ) 
    {
        m_arrayParticle[i] = NULL;
    }

    m_bLoadingComplate[TEAM_SERDIN] = false;
    m_bLoadingComplate[TEAM_CANABAN] = false;
    m_fBeforeHp[TEAM_SERDIN] = 0.0f;
    m_fBeforeHp[TEAM_CANABAN] = 0.0f;

	return S_OK;
}

void KGCAngelsEgg::SummonAngelsEgg(int iTeam, int iMonsterType, D3DXVECTOR2 vPos, bool bIsRight, int iSlotNum)
{
    int iLiveCnt=0;
    float fHP=0.0f;
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( g_MyD3D->MyPlayer[iLoop]->IsLive() )
            ++iLiveCnt;
    }
    fHP = 2000.0f * (iLiveCnt * 0.5f);

    KGC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER kPacket;
    kPacket.Type = GC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER;
    kPacket.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
    kPacket.iSlot = iSlotNum;
    kPacket.iMonType = iMonsterType;
    kPacket.iTeam = iTeam;
    kPacket.fHP = fHP;
    kPacket.bIsRight = bIsRight;
    kPacket.vPos = vPos;
    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER" );
}

bool KGCAngelsEgg::LoadScript()
{
	KLuaManager luaMgr;

	GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" );
	GCFUNC::LoadLuaScript( luaMgr, "AngelsEgg.lua" );

	if( FAILED( luaMgr.BeginTable( "AngelsEgg" ) ) )
		return false;

	if( FAILED( luaMgr.BeginTable( SiKGCRoomManager()->GetGameModeStage() )) )
		return false;

	// 천사의 알의 위치
	if( SUCCEEDED( luaMgr.BeginTable( "EggPosition" ) ) )
	{
		if( FAILED( luaMgr.BeginTable( "Canaban" ) ) )
			return false;
		else
		{
			float x = 0.0f, y = 0.0f;
			LUA_GET_VALUE( 1, x, return false );
			LUA_GET_VALUE( 2, y, return false );

			GetEggObject(TEAM_CANABAN)->SetPosition( x, y );
			GetEggObject(TEAM_CANABAN)->SetTeam( TEAM_CANABAN );
			luaMgr.EndTable();
		}


		if( FAILED( luaMgr.BeginTable( "Serdin" ) ) )
			return false;
		else
		{
			float x = 0.0f, y = 0.0f;
			LUA_GET_VALUE( 1, x, return false );
			LUA_GET_VALUE( 2, y, return false );

			GetEggObject(TEAM_SERDIN)->SetPosition( x, y );
			GetEggObject(TEAM_SERDIN)->SetTeam( TEAM_SERDIN );
			luaMgr.EndTable();
		}

		luaMgr.EndTable();
	}
	else
	{
		ASSERT( !"Angels Egg Script Egg Position Error" );
		return false;
	}

	// 플레이어가 시작할 위치 ( 이 좌표 부근에서 시작한다. )
	if( SUCCEEDED( luaMgr.BeginTable( "PlayerPosition" ) ) )
	{

		luaMgr.EndTable();
	}
	else
	{
		ASSERT( !"AngelsEgg Script Player Position Error" );
		return false;
	}

	luaMgr.EndTable();
	luaMgr.EndTable();


	GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "AngelsEggInit.lua" );
	GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(),
		"state, msg = pcall( InitAngelsEggAttribute ); "
		"if state == false then"
		"	print( msg ) "
		"end" );

	return true;
}

bool KGCAngelsEgg::LoadReformScript()
{
    KLuaManager luaMgr;

    GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" );
    GCFUNC::LoadLuaScript( luaMgr, "AngelsEgg.lua" );

    if( FAILED( luaMgr.BeginTable( "AngelsEgg" ) ) )
        return false;

    if( FAILED( luaMgr.BeginTable( SiKGCRoomManager()->GetGameModeStage() )) )
        return false;

    // 천사의 알의 위치
    if( SUCCEEDED( luaMgr.BeginTable( "EggPosition" ) ) )
    {
        if( FAILED( luaMgr.BeginTable( "Canaban" ) ) )
            return false;
        else
        {
            LUA_GET_VALUE( 1, v2AngelsEggPos[TEAM_CANABAN].x, return false );
            LUA_GET_VALUE( 2, v2AngelsEggPos[TEAM_CANABAN].y, return false );
            luaMgr.EndTable();
        }


        if( FAILED( luaMgr.BeginTable( "Serdin" ) ) )
            return false;
        else
        {
            LUA_GET_VALUE( 1, v2AngelsEggPos[TEAM_SERDIN].x, return false );
            LUA_GET_VALUE( 2, v2AngelsEggPos[TEAM_SERDIN].y, return false );
            luaMgr.EndTable();
        }

        luaMgr.EndTable();
    }
    else
    {
        ASSERT( !"Angels Egg Script Egg Position Error" );
        return false;
    }

    // 플레이어가 시작할 위치 ( 이 좌표 부근에서 시작한다. )
    if( SUCCEEDED( luaMgr.BeginTable( "PlayerPosition" ) ) )
    {

        luaMgr.EndTable();
    }
    else
    {
        ASSERT( !"AngelsEgg Script Player Position Error" );
        return false;
    }
    luaMgr.EndTable();
    luaMgr.EndTable();

    // 알 파티클 정보
    if( SUCCEEDED( luaMgr.BeginTable( "AngelsEggParticle" ) ) )
    {
        int iLoop=1;
        sParticle.clear();
        while(true)
        {
            if( FAILED( luaMgr.BeginTable( iLoop++ ) ) )
                break;

            ParticleInfo sParticleInfo;
            LUA_GET_VALUE_DEF( 1, sParticleInfo.szSequence, "" );
            LUA_GET_VALUE_DEF( 2, sParticleInfo.iTeam, 0 );
            LUA_GET_VALUE_DEF( 3, sParticleInfo.vOffset.x, 0.0f );
            LUA_GET_VALUE_DEF( 4, sParticleInfo.vOffset.y, 0.0f );
            LUA_GET_VALUE_DEF( 5, sParticleInfo.bIsRight, false );
            LUA_GET_VALUE_DEF( 6, sParticleInfo.fHpMaxRidio, 0.0f );
            LUA_GET_VALUE_DEF( 7, sParticleInfo.fHpMinRidio, 0.0f );

            sParticle.push_back( sParticleInfo );
            luaMgr.EndTable();
        }

        luaMgr.EndTable();
    }
    else
        return false;

    return true;
}

HRESULT KGCAngelsEgg::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCAngelsEgg::CustomFrameMove");

#if !defined( ANGELS_EGG_REFORM )
    if( !m_bCheckSetEggHP )
    {
        SetEggHP();
        m_bCheckSetEggHP = true;
    }
#endif

	SendP2PPacket( GC_PID_RELAY_PING );
	g_MyD3D->Story++;

    SiKGCEmbarkManager()->FrameMove();

	// 플레이어에 대한 프레임 업데이트 및 충돌처리용 영역을 계산한다.
	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
        g_MyD3D->MyPlayer[i]->Calculate_Body_Rect();
		g_MyD3D->MyPlayer[i]->Frame_Move();
	}


    // 몬스터에 대한 프레임 업데이트
    g_kMonsterManager.FrameMove();	

	// 플레이어의 충돌을 체크한다.
	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		g_MyD3D->MyPlayer[i]->Crash_Check();
	}

	// 펫에 대한 프레임 업데이트
	for( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		g_MyD3D->m_akPet[i]->FrameMove();
	}

    for(int i=0;i<MAX_OBJECT;i++)
    {
        if( g_MyD3D->m_pObject[i] )
        {
            g_MyD3D->m_pObject[i]->FrameMove();			
        }
    }

#if defined( ANGELS_EGG_REFORM )
    // 몬스터 HP상태에 따라 텍스쳐 바꿔주기
    if( m_bLoadingComplate[TEAM_SERDIN] )
        ChangeAngelsEggTexture( TEAM_SERDIN );
    if( m_bLoadingComplate[TEAM_CANABAN] )
        ChangeAngelsEggTexture( TEAM_CANABAN );
    
#else
	for( int i = 0 ; i < TEAM_NUM ; ++i )
	{
		if( o10 < 2 && g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost == true )
			m_apkAngelsEggObject[i]->SendEggInfo();
	}
#endif

    SiKGCSubjectManager()->FrameMove();

	return S_OK;
}

void KGCAngelsEgg::InitAngelsEggTexture(int iTeam)
{
    m_iSelectTexture[TEAM_SERDIN] = -1;
    m_iSelectTexture[TEAM_CANABAN] = -1;

    std::vector<ParticleInfo>::iterator vecIter = sParticle.begin();
    for(int iLoop=0; vecIter != sParticle.end(); ++vecIter, ++iLoop)
    {
        // 인덱스 오버플로워시 스크립트 오류임.
        if( -1 == m_iSelectTexture[vecIter->iTeam] )
            m_iSelectTexture[vecIter->iTeam] = iLoop;
    }

    if( 0 <= m_iSelectTexture[iTeam] && m_iSelectTexture[iTeam] < int(sParticle.size()) )
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonSlotNum[iTeam]);
		if( pMonster  == NULL ) return;

        m_arrayParticle[iTeam] = pMonster->AddTraceParticleToBoneEx( (char*)sParticle[m_iSelectTexture[iTeam]].szSequence.c_str(), 
            1.0f, 0, 8, 8, sParticle[m_iSelectTexture[iTeam]].vOffset.x, sParticle[m_iSelectTexture[iTeam]].vOffset.y, sParticle[m_iSelectTexture[iTeam]].bIsRight );
    }
}

void KGCAngelsEgg::ChangeAngelsEggTexture(int iTeam)
{
    int iLoop=-1;
	MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonSlotNum[iTeam]);
	if( pMonster  == NULL ) return;

    if( m_fBeforeHp[iTeam] != pMonster->GetHP() )
    {
        m_fBeforeHp[iTeam] = pMonster->GetHP();
        float fPercent = pMonster->GetHP() / pMonster->GetMaxHP();

        std::vector<ParticleInfo>::iterator vecIter = sParticle.begin();
        for(iLoop=0; vecIter != sParticle.end(); ++vecIter, ++iLoop)
        {
            if( (vecIter->iTeam == iTeam) &&
                (vecIter->fHpMinRidio < fPercent) && (fPercent < vecIter->fHpMaxRidio) )
                break;
        }
    }

    if( (-1 == iLoop) || (iLoop == int(sParticle.size())) )
        iLoop = m_iSelectTexture[iTeam];

    if( iLoop != m_iSelectTexture[iTeam] )
    {
        if ( m_arrayParticle[iTeam] != NULL )
            g_ParticleManager->DeleteSequence( m_arrayParticle[iTeam] );

        m_iSelectTexture[iTeam] = iLoop;
        m_arrayParticle[iTeam] = pMonster->AddTraceParticleToBoneEx( (char*)sParticle[m_iSelectTexture[iTeam]].szSequence.c_str(),
            1.0f, 0, 8, 8, sParticle[m_iSelectTexture[iTeam]].vOffset.x, sParticle[m_iSelectTexture[iTeam]].vOffset.y, sParticle[m_iSelectTexture[iTeam]].bIsRight );
    }
}

HRESULT KGCAngelsEgg::CalcExpAndGP()
{
#if defined( ANGELS_EGG_REFORM )
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
			continue;

        g_pGameBoard->m_pStartingMember[i].IsWin = g_kMonsterManager.m_mapMonster[g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam]->IsLive();
    }
#else
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
			continue;

		ASSERT( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == 0 || g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == 1 );

		g_pGameBoard->m_pStartingMember[i].IsWin = !m_apkAngelsEggObject[g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam]->IsBreak();
	}
#endif
	return S_OK;
}

bool KGCAngelsEgg::CheckGameEnd()
{
	// 팀 모드일때는 다죽은 팀이 있으면 게임이 끝난다.
	int iRedTeam = 0;
	int iBlueTeam = 0;
	for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
	{
		if( ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) )
			&& g_MyD3D->MyPlayer[i]->m_cLife > 0 )
		{
			if( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() )
				++iBlueTeam;
			else
				++iRedTeam;
		}
	}

	if( iBlueTeam == 0 || iRedTeam == 0 )
	{
		IsBlueWin = ( iBlueTeam != 0 );

		return true;
	}

#if defined( ANGELS_EGG_REFORM )
    if( m_bLoadingComplate[TEAM_SERDIN] && m_bLoadingComplate[TEAM_CANABAN] )
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonSlotNum[TEAM_SERDIN]);
		if( pMonster && false == pMonster->IsLive() )
		{
			IsBlueWin = false;
			return true;
		}

		pMonster = g_kMonsterManager.GetMonster(m_iMonSlotNum[TEAM_CANABAN]);
        if( pMonster && false == pMonster->IsLive() )
        {
            IsBlueWin = true;
            return true;
        }
    }
#else
	if( m_apkAngelsEggObject[TEAM_SERDIN]->IsBreak() || m_apkAngelsEggObject[TEAM_CANABAN]->IsBreak() )
	{
		IsBlueWin = m_apkAngelsEggObject[TEAM_SERDIN]->IsBreak();
		return true;
	}
#endif
	return false;
}

void KGCAngelsEgg::GameEnd()
{
	// 옵져버일때
	if( g_kGlobalValue.ServerInfo.CheckServerType( ST_BROADCASTING ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
	{                            
		g_MyD3D->MyHead->Start_Text("GAME OVER");
		return;
	}

	if( g_pGameBoard->m_pStartingMember[g_MyD3D->Get_MyPlayer()].IsWin )
	{
		g_KDSound.Play( "89" );
		g_KMci.Play( GC_BGM_GAME_WIN, false );
		g_MyD3D->MyHead->Start_Text("WIN");
	}
	else
	{
		g_KDSound.Play( "90" );
		g_KMci.Play( GC_BGM_GAME_LOSE, false );
		g_MyD3D->MyHead->Start_Text("LOSE");
	}
}

HRESULT KGCAngelsEgg::ON_RENDER_GC_LAYER_BETWEEN_BLOCK_CHAR()
{
#if !defined( ANGELS_EGG_REFORM )
	Render_AngelsEgg();
#endif
	KGCGameModeInterface::ON_RENDER_GC_LAYER_BETWEEN_BLOCK_CHAR();
	return S_OK;
}

HRESULT KGCAngelsEgg::Render_AngelsEgg()
{
	int i = 0; 
	for( i = 0; i < TEAM_NUM ; ++i )
	{
		GetEggObject(i)->Render();
	}
	return S_OK;
}

void KGCAngelsEgg::SetEggHP()
{
	//if( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost == false )
	//	return;

	float CanabanEggHP = 0.0f;
	int   CanabanNum = 0;
	float SerdinEggHP = 0.0f;
	int   SerdinNum = 0;

    std::vector<PLAYER*> vecPlayer;
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{        
        vecPlayer.push_back(g_MyD3D->MyPlayer[i]);
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
			continue;       

		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == TEAM_CANABAN )
		{
			CanabanEggHP += (float)g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_ATK];
			++CanabanNum;
		}
		else
		{
			SerdinEggHP += (float)g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_ATK];
			++SerdinNum;
		}
	}

    float fSerdinHP = (CanabanEggHP/(float)CanabanNum)* 20.0f * powf( 1.8f, (float)(CanabanNum - 1) );
    float fCanabanHP = (SerdinEggHP/(float)SerdinNum)* 20.0f * powf( 1.8f, (float)(SerdinNum - 1) );

    if ( SerdinEggHP >= CanabanEggHP )
    {
        fCanabanHP = fCanabanHP + ( fCanabanHP * ( 1.0f - ( CanabanEggHP / SerdinEggHP ) ) );

        if( fCanabanHP > fSerdinHP )
            fCanabanHP = fSerdinHP;
    }
    else
    {
        fSerdinHP = fSerdinHP + ( fSerdinHP * ( 1.0f - ( SerdinEggHP / CanabanEggHP ) ) );

        if( fSerdinHP > fCanabanHP )
            fSerdinHP = fCanabanHP;
    }

	m_apkAngelsEggObject[TEAM_SERDIN]->SetHP( fSerdinHP );
	m_apkAngelsEggObject[TEAM_SERDIN]->SetMaxHP( fSerdinHP );
	m_apkAngelsEggObject[TEAM_CANABAN]->SetHP( fCanabanHP );
	m_apkAngelsEggObject[TEAM_CANABAN]->SetMaxHP( fCanabanHP );
}

void KGCAngelsEgg::SetEggState( int iTeam, int iState, float fHp, float fMaxHP, bool bBreak )
{
	if( NULL == m_apkAngelsEggObject[iTeam] )
		return;

	m_apkAngelsEggObject[iTeam]->SetCurrTexIndex( iState );
	m_apkAngelsEggObject[iTeam]->SetBreak( bBreak );

	if ( m_apkAngelsEggObject[iTeam]->GetHP() > fHp )
	{
		// ㅊ 맞았네 -_-;
		m_apkAngelsEggObject[iTeam]->ParticleEffect( iState );
	}

	// 파티클 뿌리고 하쟈; 
	m_apkAngelsEggObject[iTeam]->SetHP( fHp );
	m_apkAngelsEggObject[iTeam]->SetMaxHP( fMaxHP );
}

void KGCAngelsEgg::HpRender(void)
{
	int i = 0; 
	for( i = 0; i < TEAM_NUM ; ++i )
	{
		GetEggObject(i)->HPRender();
	}
}