#include "stdafx.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Message.h"

KGCMonsterManager g_kMonsterManager;

KGCMonsterManager::KGCMonsterManager( void )
: m_iMaxSpace(15)
{
    m_mapMonster.clear();
    m_vecMonsterTable.clear();
    m_mapMonsterAction.clear();
    mapActionNULL.clear();
    mapStateNULL.clear();
    mapCallbackNULL.clear();
    strNULL.clear();
}

KGCMonsterManager::~KGCMonsterManager( void )
{

}

void KGCMonsterManager::FrameMove()
{
    // 몬스터에 대한 프레임 업데이트
    for ( std::map<int, MONSTER*>::iterator mit = m_mapMonster.begin() ; mit != m_mapMonster.end() ; ++mit ) 
    {
        mit->second->FrameMove();
    }
}

void KGCMonsterManager::DeleteAllMonster()
{
	for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin(); mit != m_mapMonster.end() ; ++mit ) {
		SAFE_DELETE(mit->second);
	}

	m_mapMonster.clear();
	m_setPreCreateBroadMonsterIndex.clear();
}

void KGCMonsterManager::DeleteMonsterAtNumber(int index)
{
	std::map<int,MONSTER*>::iterator mit = m_mapMonster.find(index);
	if ( mit == m_mapMonster.end() ) {
		return;
	}

	SAFE_DELETE (mit->second);
	m_mapMonster.erase(mit);
}

void KGCMonsterManager::AllKillMonster()
{
    for( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) 
    {
        MONSTER *pMonster = mit->second;
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
        if( NULL == pPlayer )
            pMonster->SetHP( 0.0f );
    }
}

void KGCMonsterManager::AllKillInfinityNormalMonster( void )
{
    for( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) 
    {
        if( mit->second == NULL ) 
            continue;

        if( mit->second->m_iMonsterType == MON_INFINITY_DUNGEON_GATE )
            continue;

        if( mit->second->m_iBossMonster == MONSTER::MON_BOSS ) 
            continue;

        if( mit->second->m_iSummoned == MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER ) 
            continue;

        mit->second->SetHP( 0.0f );
    }
}

void KGCMonsterManager::CreateMonsterBoolState( OUT std::map<int, bool> &mapSaAttackState )
{
	mapSaAttackState.clear();
	for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin(); mit != m_mapMonster.end() ;mit++ ) {
		if ( mapSaAttackState.insert(std::pair<int, bool>(mit->first,false)).second == false )
			mapSaAttackState[mit->first] = false;
	}

}


void KGCMonsterManager::CreateMonsterAttackIndex( OUT std::map<int, int> &mapAttackIndex )
{
	mapAttackIndex.clear();
	for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin(); mit != m_mapMonster.end() ;mit++ ) {
		if ( mapAttackIndex.insert(std::pair<int, int>(mit->first,false)).second == false )
			mapAttackIndex[mit->first] = -1;
	}

}


void KGCMonsterManager::CheckAllMonster()
{
	std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin();
	while (   mit != m_mapMonster.end() ) {
		if(mit->second == NULL) {
			mit = m_mapMonster.erase(mit);
		}
		else
			++mit;
	}

	m_setPreCreateBroadMonsterIndex.clear();
}

void KGCMonsterManager::CreateAllMonster(int iNum)
{
	for( int i = 0; i < iNum; i++ )
	{
		CreateMonster(i);
	}
}

int KGCMonsterManager::CreateMonsters( int iNum )
{
	int iStartIndex = 0; 
	std::map<int,MONSTER*>::reverse_iterator mit = m_mapMonster.rbegin();
	if ( mit != m_mapMonster.rend() ) {
		iStartIndex = mit->first + 1;
	}

	for ( int i = iStartIndex ; i < iStartIndex + iNum ; ++i ) {
		CreateMonster(i);
	}

	return iStartIndex;

}

void KGCMonsterManager::CreateMonster( int iIndex )
{
	if ( m_mapMonster.insert(std::pair<int,MONSTER*>(iIndex,NULL)).second ) {
		m_mapMonster[iIndex] = new MONSTER();
		m_mapMonster[iIndex]->m_iMonsterIndex = iIndex;
	}
	else {
		if(m_mapMonster[iIndex] == NULL) {
			m_mapMonster[iIndex] = new MONSTER();
			m_mapMonster[iIndex]->m_iMonsterIndex = iIndex;
		}
	}

	m_setPreCreateBroadMonsterIndex.erase( iIndex );
}

int KGCMonsterManager::CreateMonster()
{
	return CreateMonsters(1);
}


void KGCMonsterManager::RegisterLuabind()
{
	// 바인딩
	lua_State* L = KGCLuabinder::getInstance().GetLuaState();
	CHECK_STACK(L)
	lua_tinker::class_add<KGCMonsterManager>( L, "KGCMonsterManager" );
	lua_tinker::class_def<KGCMonsterManager>( L, "GetMonster",  &KGCMonsterManager::GetMonster );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetMonsterType",  &KGCMonsterManager::GetMonsterType );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetMonsterTypeToLive",  &KGCMonsterManager::GetMonsterTypeToLive );
	lua_tinker::class_def<KGCMonsterManager>( L, "GetSummonMonsterNum",  &KGCMonsterManager::GetSummonMonsterNum );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetLiveMonsterNum",  &KGCMonsterManager::GetLiveMonsterNum );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetMonsterHighHPNum",  &KGCMonsterManager::GetMonsterHighHPNum );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetLiveSameTeamMonsterNum",  &KGCMonsterManager::GetLiveSameTeamMonsterNum );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetLiveMonsterToType",  &KGCMonsterManager::GetLiveMonsterToType );
    lua_tinker::class_def<KGCMonsterManager>( L, "IsLiveMonsterToType",  &KGCMonsterManager::IsLiveMonsterToType );
    lua_tinker::class_def<KGCMonsterManager>( L, "SetMagicEffectToType",  &KGCMonsterManager::SetMagicEffectToType );
    lua_tinker::class_def<KGCMonsterManager>( L, "ClearMagicEffectToType",  &KGCMonsterManager::ClearMagicEffectToType );
    lua_tinker::class_def<KGCMonsterManager>( L, "SetMonsterStateToType",  &KGCMonsterManager::SetMonsterStateToType );
    lua_tinker::class_def<KGCMonsterManager>( L, "SummonMonster",  &KGCMonsterManager::SummonMonster );
    lua_tinker::class_def<KGCMonsterManager>( L, "AllKillMonster",  &KGCMonsterManager::AllKillMonster );
    lua_tinker::class_def<KGCMonsterManager>( L, "IsLiveBossMonster",  &KGCMonsterManager::IsLiveBossMonster );
    lua_tinker::class_def<KGCMonsterManager>( L, "IsSummonMonsterByTrigger",  &KGCMonsterManager::IsSummonMonsterByTrigger );

    lua_tinker::class_def<KGCMonsterManager>( L, "GetLiveInfinityMonsterTotalNum",  &KGCMonsterManager::GetLiveInfinityMonsterTotalNum );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetLiveInfinityMonsterNormalNum",  &KGCMonsterManager::GetLiveInfinityMonsterNormalNum );
    lua_tinker::class_def<KGCMonsterManager>( L, "GetLiveInfinityMonsterBossNum",  &KGCMonsterManager::GetLiveInfinityMonsterBossNum );

    lua_tinker::decl( L, "MonsterManager", this );
}

void KGCMonsterManager::LoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "BuffEnum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "_AICommonFileInfo.stg" ) == false )
        return;

    if( SUCCEEDED(luaMgr.BeginTable("AICommonFileList")) )
    {
        std::string strFileName;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue(iLoop, strFileName)); ++iLoop )
        {
            if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), strFileName.c_str() ) == false ||
                GCFUNC::LoadLuaScript( luaMgr, strFileName.c_str() ) == false )
            {
#if !defined(__PATH__)
                strFileName += " 파일 로드 실패";
                MessageBoxA( NULL, strFileName.c_str(), "AI 로드실패!!", MB_OK);
#endif
            }
        }
        luaMgr.EndTable();
    }    

    m_vecMonsterTable.clear();
    LoadMonsterTemplate( luaMgr );

    m_mapMonsterAction.clear();
    std::vector< SMonsterTemplate >::iterator vecIter = m_vecMonsterTable.begin();
    for(; vecIter != m_vecMonsterTable.end(); ++vecIter)
    {
        LoadActionScript( luaMgr, (*vecIter) );
    }
}

void KGCMonsterManager::LoadOneMonsterActionScript( const int iMonsterType_ )
{
    if( static_cast<int>(m_vecMonsterTable.size()) <= iMonsterType_ ) { 
        return;
    }

    SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ iMonsterType_ ];

    if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), kMonsterTemplate.strAIFileName ) == false )
    {
        return;
    }    
}

void KGCMonsterManager::ReLoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "BuffEnum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "_AICommonFileInfo.stg" ) == false )
        return;

    if( SUCCEEDED(luaMgr.BeginTable("AICommonFileList")) )
    {
        std::string strFileName;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue(iLoop, strFileName)); ++iLoop )
        {
            if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), strFileName.c_str() ) == false ||
                GCFUNC::LoadLuaScript( luaMgr, strFileName.c_str() ) == false )
            {
#if !defined(__PATH__)
                strFileName += " 파일 로드 실패";
                MessageBoxA( NULL, strFileName.c_str(), "AI 로드실패!!", MB_OK);
#endif
            }
        }
        luaMgr.EndTable();
    }

    m_vecMonsterTable.clear();
    LoadMonsterTemplate( luaMgr );

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) 
    {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        LoadActionScript( luaMgr, m_vecMonsterTable[mit->second->m_iMonsterType] );
    }
}

void KGCMonsterManager::LoadMonsterTemplate( KLuaManager &luaMgr )
{
    if( GCFUNC::LoadLuaScript( luaMgr, "MonsterTemplate.stg" ) == false )
        return;

    luaMgr.BeginTable( "MonsterList" );
    {
        int iMonsterIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iMonsterIndex ) ) ) break;
            {
                SMonsterTemplate kMonster;

                luaMgr.GetValue( "ID",                      kMonster.iMonsterID );
                luaMgr.GetValue( "strAIFileName",           kMonster.strAIFileName );
                luaMgr.GetValue( "strInitFunc",             kMonster.strInitFunc );
                luaMgr.GetValue( "iBaseLevel",              kMonster.iBaseLevel );

                // 아이템 개편으로 능력치 보정!
                luaMgr.GetValue( "iNameID",                 kMonster.iNameID );
                luaMgr.GetValue( "fScale",                  kMonster.fScale );
                luaMgr.GetValue( "fWeight",                 kMonster.fWeight );
                luaMgr.GetValue( "fWalkSpeed",              kMonster.fWalkSpeed );
                luaMgr.GetValue( "fRunSpeed",               kMonster.fRunSpeed );

                int iTemp; // 스크립트에는 픽셀 단위라서 게임내에 값으로 변형시켜준다
                luaMgr.GetValue( "fWidth",                  iTemp );
                kMonster.fWidth = (float)iTemp / 400.0f;
                luaMgr.GetValue( "fHeight",                 iTemp );
                kMonster.fHeight = (float)iTemp / 400.0f;

                luaMgr.GetValue( "fAttackBoundBoxWidth",    iTemp );
                kMonster.fAttackBoundBoxWidth = (float)iTemp / 400.0f;
                luaMgr.GetValue( "fAttackBoundBoxHeight",   iTemp );
                kMonster.fAttackBoundBoxHeight = (float)iTemp / 400.0f;

                iTemp = -1;
                luaMgr.GetValue( "fHeadPosY",   iTemp );
                kMonster.fHeadPosY = (float)iTemp/100.0f;
                iTemp = 0;
                luaMgr.GetValue( "fHeadPosX",   iTemp );
                kMonster.fHeadPosX = (float)iTemp/100.0f;

                luaMgr.GetValue( "eNameSize",               kMonster.eNameSize );
                luaMgr.GetValue( "fShadowRatio",            kMonster.fShadowRatio );
                luaMgr.GetValue( "eNPC",                    kMonster.eNPC );

                luaMgr.GetValue( "fVolume",                 iTemp );
                kMonster.fVolume = (float)iTemp / 400.0f;

                // [10/4/2007] breadceo.for tank
                luaMgr.GetValue( "bNoDamageReaction",       kMonster.bNoDamageReaction );

                luaMgr.GetValue( "fArrowDefence",           kMonster.fArrowDefence);

                luaMgr.GetValue( "fZPos",                   kMonster.fZPos);

                LUA_GET_VALUE_DEF( "iTechnique", kMonster.iTechnique, -1 );
                LUA_GET_VALUE_DEF( "iLayer", kMonster.iLayer, GC_LAYER_CHAR );
                LUA_GET_VALUE_DEF( "iCenterBone", kMonster.iCenterBone, 0 );
                LUA_GET_VALUE_DEF( "iHitHP", kMonster.iHitHP, 0 );
                LUA_GET_VALUE_DEF( "bNameRenderSkip", kMonster.bNameRenderSkip, false );
                LUA_GET_VALUE_DEF( "bHPrenderSkip", kMonster.bHPrenderSkip, false );
                LUA_GET_VALUE_DEF( "bMonsterTarget", kMonster.bMonsterTarget, true );
                LUA_GET_VALUE_DEF( "bNotMissionCount", kMonster.bNotMissionCount, false );
                LUA_GET_VALUE_DEF( "bNoPushCheck", kMonster.bNoPushCheck, false );
                LUA_GET_VALUE_DEF( "bNoPassMonster", kMonster.bNoPassMonster, false );
                LUA_GET_VALUE_DEF( "bNotBackAttack", kMonster.bNotBackAttack, false );

                LUA_GET_VALUE_DEF( "iCharType", kMonster.iCharType, GC_CHAR_INVALID );
                LUA_GET_VALUE_DEF( "bNotGuidedTarget", kMonster.bNotGuidedTarget, false );
                LUA_GET_VALUE_DEF( "bNotCheckStageWall", kMonster.bNotCheckStageWall, false );
                LUA_GET_VALUE_DEF( "bNoCrashDamage", kMonster.bNoCrashDamage, false );
                LUA_GET_VALUE_DEF( "bNoRealMiniMap", kMonster.bNoRealMiniMap, false );
                LUA_GET_VALUE_DEF( "bNoCheckStageRank", kMonster.bNoCheckStageRank, false );
                LUA_GET_VALUE_DEF( "bNoChampion", kMonster.bNoChampion, false );
                LUA_GET_VALUE_DEF( "bCrashParticlePosToAttackBox", kMonster.bCrashParticlePosToAttackBox, false );
                LUA_GET_VALUE_DEF( "bNoCrashToIncMP", kMonster.bNoCrashToIncMP, false );
                LUA_GET_VALUE_DEF( "bNoPushCheckToDamage", kMonster.bNoPushCheckToDamage, false );
                LUA_GET_VALUE_DEF( "bNoFlashEffect", kMonster.bNoFlashEffect, false );
				LUA_GET_VALUE_DEF( "bNoPetPlusDelay", kMonster.bNoPetPlusDelay, false );
				LUA_GET_VALUE_DEF( "bNoBuff", kMonster.bNoBuff, false );
				LUA_GET_VALUE_DEF( "bNoDamageBuff", kMonster.bNoDamageBuff, false );


                // 이름과 HP바가 나오는 위치를 
                if( SUCCEEDED( luaMgr.BeginTable( "iNameBone" ) ) )
                {
                    luaMgr.GetValue( 1, kMonster.iNameBone );
                    luaMgr.GetValue( 2, kMonster.fNameOffset_X );
                    luaMgr.GetValue( 3, kMonster.fNameOffset_Y );
                    luaMgr.EndTable();
                }

                // 몬스터의 메쉬, 텍스쳐, 카툰텍스쳐 정보를 읽어온다
                if( SUCCEEDED(luaMgr.BeginTable( "Mesh" )) )
                {
                    int iMeshIndex = 1;

                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iMeshIndex ) ) ) break;
                        {
                            EXTRA_MESH mesh;

                            LUA_GET_VALUE_DEF( 1, mesh.strMeshName, "" );
                            LUA_GET_VALUE_DEF( 2, mesh.strTextureName, "" );
                            LUA_GET_VALUE_DEF( 3, mesh.dwCartoon, 0 );
                            LUA_GET_VALUE_DEF( 4, mesh.dwTechnique, -1 );

                            LUA_GET_VALUE_DEF( "ID", mesh.strID, "" );
                            //모션달린 모델들
                            LUA_GET_VALUE_DEF( "BONEID", mesh.iBoneId, -1 );
                            LUA_GET_VALUE_DEF( "MOTION", mesh.strMotionName, "" );					
                            LUA_GET_VALUE_DEF( "POS_X", mesh.vOptionalPos.x, 0.0f );
                            LUA_GET_VALUE_DEF( "POS_Y", mesh.vOptionalPos.y, 0.0f );
                            LUA_GET_VALUE_DEF( "POS_Z", mesh.vOptionalPos.z, 0.0f );
                            kMonster.vecMeshInfo.push_back( mesh );
                            ++iMeshIndex;
                        }
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();          // "Mesh"
                }

                if( SUCCEEDED( luaMgr.BeginTable( "Sound" ) ) )
                {
                    for( int i = 1 ; ; ++i )
                    {
                        int iSoundID;
                        LUA_GET_VALUE_NOASSERT( i, iSoundID, break );
                        kMonster.m_vecSounds.push_back( iSoundID );
                    }
                    luaMgr.EndTable();
                }

                if( SUCCEEDED( luaMgr.BeginTable( "BuffList" ) ) )
                {
                    for( int i = 1 ; ; ++i )
                    {
                        int iBuffID;
                        LUA_GET_VALUE_NOASSERT( i, iBuffID, break );
                        kMonster.vecBuffList.push_back( iBuffID );
                    }
                    luaMgr.EndTable();
                }


                // 몬스터의 메쉬, 텍스쳐, 카툰텍스쳐 정보를 읽어온다
                if( SUCCEEDED(luaMgr.BeginTable( "Mesh_Dead" )) )
                {
                    int iMeshIndex = 1;

                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iMeshIndex ) ) ) break;
                        {
                            EXTRA_MESH mesh;

                            LUA_GET_VALUE_DEF( 1, mesh.strMeshName, "" );
                            LUA_GET_VALUE_DEF( 2, mesh.strTextureName, "" );
                            LUA_GET_VALUE_DEF( 3, mesh.dwCartoon, 0 );
                            LUA_GET_VALUE_DEF( 4, mesh.dwTechnique, -1 );

                            LUA_GET_VALUE_DEF( "ID", mesh.strID, "" );
                            //모션달린 모델들
                            LUA_GET_VALUE_DEF( "BONEID", mesh.iBoneId, -1 );
                            LUA_GET_VALUE_DEF( "MOTION", mesh.strMotionName, "" );					
                            LUA_GET_VALUE_DEF( "POS_X", mesh.vOptionalPos.x, 0.0f );
                            LUA_GET_VALUE_DEF( "POS_Y", mesh.vOptionalPos.y, 0.0f );
                            LUA_GET_VALUE_DEF( "POS_Z", mesh.vOptionalPos.z, 0.0f );
                            kMonster.vecMeshDeadInfo.push_back( mesh );
                            ++iMeshIndex;
                        }
                        luaMgr.EndTable();					
                    }
                    luaMgr.EndTable();          // "Mesh"
                }

                // 몬스터 궤적!
                if( SUCCEEDED(luaMgr.BeginTable( "Swoosh" )) )
                {
                    int iMeshIndex = 1;

                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iMeshIndex ) ) ) break;
                        {
                            SwooshInfo kSwooshInfo;

                            luaMgr.GetValue( 1, kSwooshInfo.iBaseBone );
                            luaMgr.GetValue( 2, kSwooshInfo.iEndBone );    
                            luaMgr.GetValue( 3, kSwooshInfo.strTex );
                            luaMgr.GetValue( 4, kSwooshInfo.fTu1 );
                            luaMgr.GetValue( 5, kSwooshInfo.fTu2 );
                            luaMgr.GetValue( 6, kSwooshInfo.fTv1 );
                            luaMgr.GetValue( 7, kSwooshInfo.fTv2 );

                            kMonster.vecSwooshInfo.push_back( kSwooshInfo );

                            ++iMeshIndex;
                        }
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();          // "Mesh"
                }



                if( SUCCEEDED(luaMgr.BeginTable( "PushRect" )) )
                {
                    int iRectIndex = 1;

                    PushRectInfo kRectInfo;

                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iRectIndex ) ) ) break;
                        {
                            luaMgr.GetValue( 1, kRectInfo.iBoneId );
                            luaMgr.GetValue( 2, kRectInfo.fWidth );    
                            luaMgr.GetValue( 3, kRectInfo.fHeight );
                            luaMgr.GetValue( 4, kRectInfo.fOffsetX );
                            luaMgr.GetValue( 5, kRectInfo.fOffsetY );

                            kMonster.vecPushRectInfo.push_back( kRectInfo );

                            ++iRectIndex;
                        }
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();
                }
                else
                {
                    PushRectInfo kRectInfo;
                    kRectInfo.iBoneId = -1;
                    kRectInfo.fWidth = kMonster.fWidth;
                    kRectInfo.fHeight = kMonster.fHeight;
                    kRectInfo.fOffsetX = 0.0f;
                    kRectInfo.fOffsetY = 0.0f;

                    kMonster.vecPushRectInfo.push_back( kRectInfo );
                }



                if( SUCCEEDED(luaMgr.BeginTable( "AttackRect" )) )
                {
                    int iRectIndex = 1;

                    while( true )
                    {
                        AttackRectInfo kRectInfo;
                        if( FAILED( luaMgr.BeginTable( iRectIndex ) ) ) break;
                        {


                            luaMgr.GetValue( 1, kRectInfo.iBoneId );
                            luaMgr.GetValue( 2, kRectInfo.fWidth );    
                            luaMgr.GetValue( 3, kRectInfo.fHeight );
                            luaMgr.GetValue( 4, kRectInfo.fDelta );
                            luaMgr.GetValue( 5, kRectInfo.OffsetX );
                            luaMgr.GetValue( 6, kRectInfo.OffsetY );

                            kMonster.vecAttackRectInfo.push_back( kRectInfo );

                            ++iRectIndex;
                        }
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();          // "Mesh"
                }
                else
                {

                    AttackRectInfo kRectInfo;
                    kRectInfo.iBoneId = -1;
                    kRectInfo.fWidth = kMonster.fAttackBoundBoxWidth;
                    kRectInfo.fHeight = kMonster.fAttackBoundBoxHeight;
                    kRectInfo.fDelta = 1.0f;

                    kMonster.vecAttackRectInfo.push_back( kRectInfo );
                }

                if( SUCCEEDED(luaMgr.BeginTable( "FootHold" )) )
                {
                    int iFootHoldIndex = 1;                   
                    MonsterFootHoldInfo kFootHoldInfo;

                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iFootHoldIndex ) ) ) break;
                        {
                            luaMgr.GetValue( 1, kFootHoldInfo.m_iBaseBone );
                            luaMgr.GetValue( 2, kFootHoldInfo.m_iEndBone );
                            luaMgr.GetValue( 3, kFootHoldInfo.m_dwProp );    
                            luaMgr.GetValue( 4, kFootHoldInfo.m_fRelativeX );    
                            luaMgr.GetValue( 5, kFootHoldInfo.m_fRelativeY );    

                            kFootHoldInfo.m_fRelativeX += 1.0f;
                            kFootHoldInfo.m_fRelativeY += 0.4f;

                            kMonster.vecFootHoldInfo.push_back( kFootHoldInfo );

                            ++iFootHoldIndex;
                        }
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();          // "Mesh"
                }

                if( SUCCEEDED(luaMgr.BeginTable( "TraceIcon" )) )
                {
                    luaMgr.GetValue( "IsMyExistScreen", kMonster.sTraceIcon.bIsMyExistScreen );
                    luaMgr.GetValue( "IsNonCheckInScreen", kMonster.sTraceIcon.bIsNonCheckInScreen );

                    luaMgr.GetValue( "ParticleSqu_1", kMonster.sTraceIcon.strSqu_1 );
                    luaMgr.GetValue( "ParticleSqu_2", kMonster.sTraceIcon.strSqu_2 );
                    luaMgr.EndTable();          // "Mesh"
                }

                m_vecMonsterTable.push_back( kMonster );
            }
            luaMgr.EndTable();

            ++iMonsterIndex;
        }
    }
    luaMgr.EndTable();
}

void KGCMonsterManager::LoadActionScript( KLuaManager &luaMgr, SMonsterTemplate &kMonsterTemplate )
{
    KMonsterActionScript kMonActionInfo;

    if( GCFUNC::LoadLuaScript( luaMgr, kMonsterTemplate.strAIFileName ) == false ||
        GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), kMonsterTemplate.strAIFileName ) == false )
    {
        return;
    }

    luaMgr.BeginTable( "ActionList" );
    {
        int iActionIndex = 1;
        while( SUCCEEDED( luaMgr.BeginTable( iActionIndex ) ) )
        {
            KGCMonsterAction kMonsterAction;
            kMonsterAction.Init();

            luaMgr.GetValue( "ID",                  kMonsterAction.m_strID );
            luaMgr.GetValue( "Motion",              kMonsterAction.m_strMotion );
            luaMgr.GetValue( "Func",                kMonsterAction.m_strFunc );
            luaMgr.GetValue( "EnableMoveCheck",     kMonsterAction.m_bEnableMoveCheck );
            luaMgr.GetValue( "MoveFailFunc",        kMonsterAction.m_strMoveFailFunc );
            luaMgr.GetValue( "IsFallDownMotion",    kMonsterAction.m_bIsFallDownMotion );

            // 트리거
            if( SUCCEEDED( luaMgr.BeginTable( "Trigger" ) ) )
            {
                int iDungeonLevel = 0;
                if ( SiKGCWorldMapManager()->IsHeroDungeon(SiKGCRoomManager()->GetGameMode()) )
                    iDungeonLevel = g_pkQuestManager->GetHeroDungeonLevel();
                else
                    iDungeonLevel = SiKGCRoomManager()->GetRoomInfo().nDungeonLevel;

                int iTriggerIndex = 1;
                while( SUCCEEDED( luaMgr.BeginTable( iTriggerIndex ) ) )
                {
                    KGCActionTrigger kActionTrigger;

                    // 던전 레벨에 따라 다른 체크 프레임을 쓴ㄷㅏ!
                    char strTemp[MAX_PATH];
                    int iBaseAICheckFrame = 0;
                    int iBaseDelayFrame = 0;
                    sprintf_s( strTemp, MAX_PATH, "AICheckFrame%d", iDungeonLevel );
                    luaMgr.GetValue( "AICheckFrame",          iBaseAICheckFrame );
                    LUA_GET_VALUE_DEF( strTemp,               kActionTrigger.iAICheckFrame, iBaseAICheckFrame );

                    luaMgr.GetValue( "AIFunc",              kActionTrigger.strAIFunc );

                    sprintf_s( strTemp, MAX_PATH, "DelayFrame%d", iDungeonLevel );
                    luaMgr.GetValue( "DelayFrame",          iBaseDelayFrame );
                    LUA_GET_VALUE_DEF( strTemp,               kActionTrigger.iDelayFrame, iBaseDelayFrame );

                    // 다음 체크까지 남은 시간
                    kActionTrigger.iAICheckFrame -= 1;
                    kActionTrigger.iRemainAICheckFrame = kActionTrigger.iAICheckFrame;
                    kActionTrigger.iRemainDelayFrame = 0;

                    kMonsterAction.m_vecActionTrigger.push_back( kActionTrigger );

                    ++iTriggerIndex;

                    luaMgr.EndTable();
                }
                luaMgr.EndTable();
            }

            // 모션 리스트의 인덱스
            kMonsterAction.m_iMotionIndex = iActionIndex - 1;
            kMonActionInfo.m_mapActionList[ kMonsterAction.m_strID ] = kMonsterAction;
            kMonActionInfo.m_mapStateIndex[ iActionIndex - 1 ] = kMonsterAction.m_strID;
            ++iActionIndex;

            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();

    luaMgr.BeginTable( "CallBack" );
    {
        std::string strFuncName;
        luaMgr.GetValue( "Land",    strFuncName );
        kMonActionInfo.m_mapCallBack[ "Land" ] =   strFuncName;

        strFuncName = "";
        luaMgr.GetValue( "Die",     strFuncName );
        kMonActionInfo.m_mapCallBack[ "Die" ] =    strFuncName;

        strFuncName = "";
        luaMgr.GetValue( "Damage",  strFuncName );
        kMonActionInfo.m_mapCallBack[ "Damage" ] = strFuncName;

        strFuncName = "";
        luaMgr.GetValue( "DefenceArrow",  strFuncName );
        kMonActionInfo.m_mapCallBack[ "DefenceArrow" ] = strFuncName;

        strFuncName = "";
        luaMgr.GetValue( "End",  strFuncName );
        kMonActionInfo.m_mapCallBack[ "End" ] = strFuncName;

        strFuncName = "";
        luaMgr.GetValue( "ShieldDamage",  strFuncName );
        kMonActionInfo.m_mapCallBack[ "ShieldDamage" ] = strFuncName;

        strFuncName = "";
        luaMgr.GetValue( "DamageBeforeChange",  strFuncName );
        kMonActionInfo.m_mapCallBack[ "DamageBeforeChange" ] = strFuncName;

        strFuncName = "";
        luaMgr.GetValue( "SyncDamage",  strFuncName );
        kMonActionInfo.m_mapCallBack[ "SyncDamage" ] = strFuncName;
    }
    luaMgr.EndTable();

    std::map< int, KMonsterActionScript >::iterator mapIter = m_mapMonsterAction.find( kMonsterTemplate.iMonsterID );
    if( mapIter == m_mapMonsterAction.end() )
        m_mapMonsterAction.insert( std::make_pair(kMonsterTemplate.iMonsterID, kMonActionInfo) );
    else
        mapIter->second = kMonActionInfo;
}

MONSTER* KGCMonsterManager::GetMonster( int iMonsterIndex )
{ 
	MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iMonsterIndex);
	return (mit == m_mapMonster.end()) ? (NULL) : (mit->second); 
}

MONSTER* KGCMonsterManager::GetMonsterType( int iMonsterType )
{ 
    MAP_INT_PMONSTER::iterator mit = m_mapMonster.begin();
    for(; mit != m_mapMonster.end(); ++mit)
    {
        if( NULL == mit->second )
            continue;

        if( iMonsterType == mit->second->m_iMonsterType )
            return mit->second;
    }
    return NULL;
}

MONSTER* KGCMonsterManager::GetMonsterTypeToLive( int iMonsterType )
{ 
    MAP_INT_PMONSTER::iterator mit = m_mapMonster.begin();
    for(; mit != m_mapMonster.end(); ++mit)
    {
        if( NULL == mit->second )
            continue;

        if( iMonsterType != mit->second->m_iMonsterType )
            continue;

        if ( mit->second->IsLive() )
            return mit->second;

        break;
    }
    return NULL;
}

bool KGCMonsterManager::IsMonsterIndex( const int iMonster )
{
	MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iMonster);
	return mit != m_mapMonster.end() && mit->second != NULL;
}

bool KGCMonsterManager::IsMonsterType( const int iType )
{
	if( (MON_SLIME > iType) || (iType >= MONSTER_MODEL_NUM) )
		return false; 

	return true; 
}

bool KGCMonsterManager::IsSummonMonsterByPlayer( int iMonster )
{
	MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iMonster);
	if ( mit == m_mapMonster.end() ) 
		return false;

	if ( mit->second == NULL )
		return false;

	return mit->second->m_iSummoned == MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER;
}

bool KGCMonsterManager::IsSummonMonsterByMonster( int iMonster )
{
	MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iMonster);
	if ( mit == m_mapMonster.end() ) 
		return false;

	if ( mit->second == NULL )
		return false;

	return mit->second->m_iSummoned == MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
}

bool KGCMonsterManager::IsSummonMonsterByTrigger( int iMonster )
{
    MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iMonster);
    if ( mit == m_mapMonster.end() ) 
        return false;

    if ( mit->second == NULL )
        return false;

    return mit->second->m_iSummoned == MONSTER::MEMS_SUMMON_MONSTER_BY_TRIGGER;
}

bool KGCMonsterManager::IsSummonMonster( int iMonster )
{
    MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iMonster);
    if ( mit == m_mapMonster.end() ) 
        return false;

    if ( mit->second == NULL )
        return false;

    return mit->second->m_iSummoned != MONSTER::MEMS_NOT_SUMMON_MONSTER;
}

bool KGCMonsterManager::IsLiveMonsterToType(int iType)
{
    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iMonsterType == iType ) 
            return true;
    }

    return false;
}

int KGCMonsterManager::GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE eMonsterType )
{
	int iStartIndex = 0; 
	int iAddIndex = 1;
    bool bloop = true;

    switch( eMonsterType )
    {
    case ECREATE_MONSTER_TYPE_NORMAL:
        {
            MAP_INT_PMONSTER::iterator mit = m_mapMonster.begin();
            for ( ; mit != m_mapMonster.end() ; ++mit) {
                if( m_setPreCreateBroadMonsterIndex.find( mit->first ) == m_setPreCreateBroadMonsterIndex.end() ) {
                    if( mit->second == NULL){
                        iStartIndex = mit->first;
                        iAddIndex = 0;
                        break;
                    }
                    if( mit->second->IsLive() == false) {
                        iStartIndex = mit->first;
                        iAddIndex = 0;
                        break;
                    }
                }
                iStartIndex = mit->first;
            }
            break;
        }
    case ECREATE_MONSTER_TYPE_SUMMON:
        {
            iStartIndex = KMMTI_SUMMON_MONSTER_START;
            while( bloop ) {
                MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iStartIndex);
                if ( mit != m_mapMonster.end() && mit->second->IsLive() == true ) {
                    iStartIndex++;
                    continue;
                }

                if ( m_setPreCreateBroadMonsterIndex.find( iStartIndex ) == m_setPreCreateBroadMonsterIndex.end() ) {
                    iAddIndex = 0;
                    break;
                }

                iStartIndex++;
            }
            break;
        }
    case ECREATE_MONSTER_TYPE_EVENT:
        {
            iStartIndex = KMMTI_EVENT_MONSTER_START;
            while( bloop ) {
                MAP_INT_PMONSTER::iterator mit = m_mapMonster.find(iStartIndex);
                if ( mit != m_mapMonster.end() && mit->second->IsLive() == true ) {
                    iStartIndex++;
                    continue;
                }

                if ( m_setPreCreateBroadMonsterIndex.find( iStartIndex ) == m_setPreCreateBroadMonsterIndex.end() ) {
                    iAddIndex = 0;
                    break;
                }

                iStartIndex++;
            }
            break;
        }
    }

	while ( m_setPreCreateBroadMonsterIndex.insert(iStartIndex + iAddIndex).second == false )
		iAddIndex++;

	return iStartIndex + iAddIndex;

}

int KGCMonsterManager::GetBaseMonsterLevel( int iMonsterType )
{
	if ( IsMonsterType(iMonsterType) == false ) return 0;
	const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ iMonsterType ];

	return kMonsterTemplate.iBaseLevel;
}

int KGCMonsterManager::CheckFreeSpaceMonsterNUM()
{
	int iFreeSpaceNum = 0;

	for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
		if(mit->second == NULL) {
			iFreeSpaceNum++;
			continue;
		}

		if(mit->second->IsLive() == false)
			iFreeSpaceNum++;
	}

	return iFreeSpaceNum;
}

int KGCMonsterManager::GetSummonMonsterNum ( int iMonType )
{
	int iSummonMonsterNum = 0;

	for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
		if ( mit->second == NULL )
			continue;

		if ( mit->second->IsLive() == false )
			continue;

		if ( mit->second->m_iSummoned != MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER )
			continue;

		if ( mit->second->m_iMonsterType != iMonType && iMonType != -1 )
			continue;

		++iSummonMonsterNum;
	}

	return iSummonMonsterNum;
}

int KGCMonsterManager::GetLiveMonsterNum()
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        ++iMonsterNum;
    }

    return iMonsterNum;
}

int KGCMonsterManager::GetLiveMonsterNumExceptSummonMonster()
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if( mit->second->m_iSummoned == MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER ) 
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        ++iMonsterNum;
    }

    return iMonsterNum;
}

int KGCMonsterManager::GetMonsterHighHPNum( float fHP )
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( false == mit->second->m_bLive )
            continue;

        if( -1 != mit->second->m_iTeam )
            continue;

        if ( fHP >= mit->second->m_fHP )
            continue;

        if( mit->second->m_bNotCheckStateWall == true ) {
            continue;
        }

        ++iMonsterNum;
    }

    return iMonsterNum;
}

int KGCMonsterManager::GetLiveSameTeamMonsterNum( int iTeam )
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iTeam != iTeam )
            continue;

        ++iMonsterNum;
    }

    return iMonsterNum;
}

int KGCMonsterManager::GetSummonMonsterIndex( DWORD dwPlayerUID_, EGCMonster eMonType_ )
{
    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if ( mit->second->m_iSummoned != MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER )
            continue;

        if ( mit->second->m_iMonsterType == eMonType_ && mit->second->GetSummonPlayerUID() == dwPlayerUID_ ) {
            return mit->second->GetMonsterIndex();
        }
    }

    return -1;
}

int KGCMonsterManager::GetLiveMonsterToType(int iType)
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iMonsterType == iType ) 
            ++iMonsterNum;
    }

    return iMonsterNum;
}

bool KGCMonsterManager::IsLiveBossMonster()
{
    MAP_INT_PMONSTER::iterator mit = m_mapMonster.begin();
    for(; mit != m_mapMonster.end(); ++mit)
    {
        if( NULL == mit->second )
            continue;

        if ( false == mit->second->IsLive() )
            continue;

        if( MONSTER::MON_BOSS == mit->second->m_iBossMonster )
            return true;
    }
    return false;
}

void KGCMonsterManager::SetMagicEffectToType(int iType, int iBuffID, float fBuffTime, int iBuffLevel)
{
    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iMonsterType != iType ) 
            continue;

        if( false == mit->second->IsMagicEffect( mit->first, iBuffID ) )
            mit->second->SetMagicEffect( mit->first, iBuffID, fBuffTime, iBuffLevel, -1, false );
    }
}

void KGCMonsterManager::ClearMagicEffectToType(int iType, int iBuffID)
{
    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iMonsterType != iType ) 
            continue;

        mit->second->ClearMagicEffect( mit->first, iBuffID );
    }
}

void KGCMonsterManager::SetMonsterStateToType(int iType, char *strState)
{
    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iMonsterType != iType ) 
            continue;

        mit->second->SetState( strState );
    }
}

std::map< std::string, KGCMonsterAction >& KGCMonsterManager::GetMonsterAction(int iMonsterType)
{
    std::map< int, KMonsterActionScript >::iterator mapIter = m_mapMonsterAction.find( iMonsterType );
    if( mapIter != m_mapMonsterAction.end() )
        return mapIter->second.m_mapActionList;
    return mapActionNULL;
}

std::map< int, std::string >& KGCMonsterManager::GetMonsterActionIndex(int iMonsterType)
{
    std::map< int, KMonsterActionScript >::iterator mapIter = m_mapMonsterAction.find( iMonsterType );
    if( mapIter != m_mapMonsterAction.end() )
        return mapIter->second.m_mapStateIndex;
    return mapStateNULL;
}

std::map< std::string, std::string >& KGCMonsterManager::GetMonsterCallback(int iMonsterType)
{
    std::map< std::string, std::string > mapNULL;
    std::map< int, KMonsterActionScript >::iterator mapIter = m_mapMonsterAction.find( iMonsterType );
    if( mapIter != m_mapMonsterAction.end() )
        return mapIter->second.m_mapCallBack;
    return mapCallbackNULL;
}

std::string KGCMonsterManager::GetMonsterStateToIndex(int iMonsterType, int iIndex)
{
    std::map< int, KMonsterActionScript >::iterator mapIter = m_mapMonsterAction.find( iMonsterType );
    if( mapIter != m_mapMonsterAction.end() )
    {
        std::map< int, std::string >::iterator mapStateIter = mapIter->second.m_mapStateIndex.find( iIndex );
        if( mapStateIter != mapIter->second.m_mapStateIndex.end() )
        {
            return mapStateIter->second;
        }
    }
    return strNULL;
}

std::string KGCMonsterManager::GetMonsterCallbackToStringKey(int iMonsterType, std::string strKey)
{
    std::map< int, KMonsterActionScript >::iterator mapIter = m_mapMonsterAction.find( iMonsterType );
    if( mapIter != m_mapMonsterAction.end() )
    {
        std::map< std::string, std::string >::iterator mapCallbackIter = mapIter->second.m_mapCallBack.find( strKey );
        if( mapCallbackIter != mapIter->second.m_mapCallBack.end() )
        {
            return mapCallbackIter->second;
        }
    }
    return strNULL;
}

bool KGCMonsterManager::SummonMonster( KSummonInfo* pSummonInfo, int iSummonMonsterLevel )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return false;

    int iSlot = g_kMonsterManager.GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE_SUMMON );
    float fX = pSummonInfo->fX / SiKGCFantasticMap()->GetPixelWidth();
    float fY = pSummonInfo->fY;
    int iLevel = g_kMonsterManager.GetBaseMonsterLevel(pSummonInfo->iMonType);

    if( 0 < iSummonMonsterLevel )
        iLevel = iSummonMonsterLevel;

    g_kSummonMonster->Init();
    strncpy( g_kSummonMonster->m_szState, pSummonInfo->strState.c_str(), static_cast<int>(pSummonInfo->strState.size()) );
    g_kSummonMonster->m_usMonIndex = (USHORT)(iSlot);
    g_kSummonMonster->m_usMonType = (USHORT)( pSummonInfo->iMonType);
    g_kSummonMonster->m_usEntranceX = GCUTIL_MATH::floatToHalf( fX );
    g_kSummonMonster->m_usEntranceY = GCUTIL_MATH::floatToHalf( fY );
    g_kSummonMonster->m_iLevel = iLevel;
    g_kSummonMonster->m_bIsRight = true;
    g_kSummonMonster->m_iCountValue = pSummonInfo->iCountValue;
    g_kSummonMonster->m_bCheckSafePostion = pSummonInfo->bCheckSafePostion;
    SendNetworkData( GC_PID_SUMMON_MONSTER, KNC_BROADCAST );

    return false;
}

int KGCMonsterManager::GetLiveInfinityMonsterTotalNum()
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iSummoned != MONSTER::MEMS_SUMMON_MONSTER_BY_TRIGGER ) 
            continue;

        ++iMonsterNum;
    }

    return iMonsterNum;
}

int KGCMonsterManager::GetLiveInfinityMonsterNormalNum()
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iSummoned != MONSTER::MEMS_SUMMON_MONSTER_BY_TRIGGER ) 
            continue;

        if( mit->second->m_iBossMonster == MONSTER::MON_BOSS ) 
            continue;

        ++iMonsterNum;
    }

    return iMonsterNum;
}

int KGCMonsterManager::GetLiveInfinityMonsterBossNum()
{
    int iMonsterNum = 0;

    for ( std::map<int,MONSTER*>::iterator mit = m_mapMonster.begin() ;  mit != m_mapMonster.end() ; ++mit ) {
        if ( mit->second == NULL )
            continue;

        if ( mit->second->IsLive() == false )
            continue;

        if( mit->second->m_iSummoned != MONSTER::MEMS_SUMMON_MONSTER_BY_TRIGGER ) 
            continue;

        if( mit->second->m_iBossMonster != MONSTER::MON_BOSS ) 
            continue;

        ++iMonsterNum;
    }

    return iMonsterNum;
}

bool KGCMonsterManager::SummonInfinityMonster( KInfinitySummonInfo* pInfinitySummonInfo )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return false;

    int iSlot = g_kMonsterManager.GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE_NORMAL );
    float fX = pInfinitySummonInfo->fX / SiKGCFantasticMap()->GetPixelWidth();
    float fY = pInfinitySummonInfo->fY;

    g_kSummonInfinityMonster->Init();
    strncpy( g_kSummonInfinityMonster->m_szState, pInfinitySummonInfo->strState.c_str(), static_cast<int>(pInfinitySummonInfo->strState.size()) );
    g_kSummonInfinityMonster->m_usMonIndex = (USHORT)(iSlot);
    g_kSummonInfinityMonster->m_usMonType = (USHORT)( pInfinitySummonInfo->iMonType);
    g_kSummonInfinityMonster->m_usTGid = (USHORT)( pInfinitySummonInfo->iTGid );
    g_kSummonInfinityMonster->m_usEntranceX = GCUTIL_MATH::floatToHalf( fX );
    g_kSummonInfinityMonster->m_usEntranceY = GCUTIL_MATH::floatToHalf( fY );
    g_kSummonInfinityMonster->m_iLevel = pInfinitySummonInfo->iLevel;
    g_kSummonInfinityMonster->m_bIsRight = true;
    g_kSummonInfinityMonster->m_iCountValue = pInfinitySummonInfo->iCountValue;
    g_kSummonInfinityMonster->m_bCheckSafePostion = pInfinitySummonInfo->bCheckSafePostion;
    g_kSummonInfinityMonster->m_bBoss = pInfinitySummonInfo->bBoss;
    SendNetworkData( GC_PID_SUMMON_INFINITY_MONSTER, KNC_BROADCAST );

    return true;
}

