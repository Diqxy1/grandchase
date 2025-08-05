/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2012년 04월 22일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 오브젝트 싱크 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "KGCSyncObject.h"

ImplementSingleton( KGCSyncObjectManager )


////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스크립트 파일모니터 등록( SyncObjectInfo.lua )
////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( __PATH__ )

#include "../GCUtil/GCFileMoniter.h"
static bool bReLoadSyncObject = false;
void ReLoadSyncObjectInfo()
{
    bReLoadSyncObject	= true;
}
void KGCSyncObjectManager::CheckFileMonitor()
{
    if( bReLoadSyncObject )
    {
        LoadScript();
        bReLoadSyncObject = false;
    }
}
void KGCSyncObjectManager::RegisterFileMoniter()
{
    SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( "SyncObjectInfo.lua") , ReLoadSyncObjectInfo );
}

#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////
// 인스턴스 클래스 멤버정의
////////////////////////////////////////////////////////////////////////////////////////////////////////
SyncObjectInstance::SyncObjectInstance( DWORD dwUID, int iObjectID, float fX, float fY, SyncObjectOriginal* pOriginalObject )
{
    m_dwUID = dwUID;
    m_iObjectID = iObjectID;
    m_pOriginalObject = pOriginalObject;
    m_vPos.x = fX;
    m_vPos.y = fY;
    m_vPos.z = 0.5f;
    SettingAttribute();
    CreateParticle();
}

SyncObjectInstance::SyncObjectInstance( const SyncObjectInstance &sObjectInstance )
{
    m_dwUID = sObjectInstance.m_dwUID;
    m_iObjectID = sObjectInstance.m_iObjectID;
    m_pOriginalObject = sObjectInstance.m_pOriginalObject;
    m_vPos.x = sObjectInstance.m_vPos.x;
    m_vPos.y = sObjectInstance.m_vPos.y;
    SettingAttribute();
    CreateParticle();
}

SyncObjectInstance::~SyncObjectInstance()
{
    ClearParticle();
}

void SyncObjectInstance::CreateParticle()
{
    if( m_pOriginalObject )
    {
        ClearParticle();
        std::vector<SyncParticleInfo>::const_iterator vecIter = m_pOriginalObject->m_vecParticle.begin();
        for(; vecIter != m_pOriginalObject->m_vecParticle.end(); ++vecIter)
        {
            CParticleEventSeqPTR pSeq = g_ParticleManager->CreateSequence(vecIter->m_strName.c_str(), 
                (m_vPos.x + vecIter->m_fXOffset), (m_vPos.y + vecIter->m_fYOffset), 0.5f );
            if( pSeq )
            {
                if( m_pOriginalObject->m_bTraceParticle )
                {
                    TRACE_INFO stTrace;
                    stTrace.m_pvPos = &m_vPos;
                    stTrace.m_fXOffset = vecIter->m_fXOffset;
                    stTrace.m_fYOffset = vecIter->m_fYOffset;
                    stTrace.m_fParticleTraceTime = vecIter->m_fTraceTime;
                    pSeq->SetTrace( &stTrace );
                    pSeq->SetShow( true );
                }

                m_vecParticleSeq.push_back( pSeq );
            }
        }
    }
}

void SyncObjectInstance::ClearParticle()
{
    if( false == m_vecParticleSeq.empty() )
        g_ParticleManager->DeleteSequence( m_vecParticleSeq );
    m_vecParticleSeq.clear();
}

void SyncObjectInstance::SettingAttribute()
{
    if( m_pOriginalObject )
    {
        m_iLife = m_pOriginalObject->m_iStart_Life;
        m_vPos.x += m_pOriginalObject->m_fOffsetX;
        m_vPos.y += m_pOriginalObject->m_fOffsetY;
        m_iNoCheckGroundFrame = m_pOriginalObject->m_iNoCheckGroundFrame;
    }
    m_bIsGroundCrash = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// 매니져 클래스 멤버정의
////////////////////////////////////////////////////////////////////////////////////////////////////////
KGCSyncObjectManager::KGCSyncObjectManager(void)
{
    BindLua();
    InitCalss();
#if !defined( __PATH__ )
    RegisterFileMoniter();
#endif
}

KGCSyncObjectManager::~KGCSyncObjectManager(void)
{
    DestroyCalss();
}

void KGCSyncObjectManager::InitCalss()
{
    m_dwUIDCount = 1;
    m_mapObjectInstanceCount.clear();
    ClearObject_Private( m_mapObjectInstance );
}

void KGCSyncObjectManager::DestroyCalss()
{
    m_dwUIDCount = 1;
    m_mapObjectInstanceCount.clear();
    ClearObject_Private( m_mapObjectInstance );
    ClearObject_Private( m_mapObjectInfo );
}

template<typename T>
void KGCSyncObjectManager::ClearObject_Private( T &mapObject )
{
    T::iterator mapIter = mapObject.begin();
    for(; mapIter != mapObject.end(); ++mapIter)
    {
        SAFE_DELETE( mapIter->second );
    }
    mapObject.clear();
}

void KGCSyncObjectManager::FrameMove()
{
#if !defined( __PATH__ )
    CheckFileMonitor();
#endif

    std::map<DWORD, SyncObjectInstance*> mapDeleteList;
    std::map<DWORD, SyncObjectInstance*>::iterator mapInstanceIter = m_mapObjectInstance.begin();
    for(; mapInstanceIter != m_mapObjectInstance.end(); ++mapInstanceIter)
    {
        SyncObjectInstance* pInstance = mapInstanceIter->second;
        if( pInstance && pInstance->m_pOriginalObject )
        {
            pInstance->m_iLife += pInstance->m_pOriginalObject->m_iChange_Life;

            if( false == pInstance->m_bIsGroundCrash )
            {
                float fSpeedX = 0.0f, fSpeedY = 0.0f;
                if( 0.0f != pInstance->m_pOriginalObject->m_fGravity )
                {
                    pInstance->m_vPos.y += fSpeedY = GetGravityPos( pInstance );
                }

                if( 0 < pInstance->m_iNoCheckGroundFrame )
                {
                    --pInstance->m_iNoCheckGroundFrame;
                }
                else if( pInstance->m_pOriginalObject->m_bCheckGround )
                {
                    CheckCrashGround( pInstance, fSpeedX, fSpeedY );
                }
            }

            if( g_kGlobalValue.m_kUserInfo.bHost )
            {
                if( false == pInstance->m_pOriginalObject->m_bNoCrashPlayer )
                {
                    bool bCollision = CheckCrashPlayer_Private( pInstance );
                    if( bCollision && pInstance->m_pOriginalObject->m_bClearCrash )
                        pInstance->m_iLife = 0;
                }

                if( false == pInstance->m_pOriginalObject->m_bNoCrashMonster )
                {
                    bool bCollision = CheckCrashMonster_Private( pInstance );
                    if( bCollision && pInstance->m_pOriginalObject->m_bClearCrash )
                        pInstance->m_iLife = 0;
                }

                if( 0 >= pInstance->m_iLife )
                {
                    mapDeleteList.insert( std::make_pair(mapInstanceIter->first, mapInstanceIter->second) );   
                }
            }
        }
    }

    std::map<DWORD, SyncObjectInstance*>::iterator mapDeleteIter = mapDeleteList.begin();
    for(; mapDeleteIter != mapDeleteList.end(); ++mapDeleteIter)
    {
        mapInstanceIter = m_mapObjectInstance.find( mapDeleteIter->first );
        if( mapInstanceIter != m_mapObjectInstance.end() )
            DelSyncObject_NotMeCast( mapInstanceIter->first );
    }
}


void KGCSyncObjectManager::Render()
{
#if !defined( __PATH__ )
    if( g_kGlobalValue.m_bRenderBoundBox )
    {
        std::map<DWORD, SyncObjectInstance*>::iterator mapInstanceIter = m_mapObjectInstance.begin();
        for(; mapInstanceIter != m_mapObjectInstance.end(); ++mapInstanceIter)
        {
            SyncObjectInstance *pInstance = mapInstanceIter->second;
            if( pInstance && pInstance->m_pOriginalObject )
            {
                D3DXVECTOR4 vRange; 
                vRange.x = pInstance->m_vPos.x - pInstance->m_pOriginalObject->m_fHalfWidth;
                vRange.y = pInstance->m_vPos.y - pInstance->m_pOriginalObject->m_fHalfHeight;
                vRange.z = pInstance->m_vPos.x + pInstance->m_pOriginalObject->m_fHalfWidth;
                vRange.w = pInstance->m_vPos.y + pInstance->m_pOriginalObject->m_fHalfHeight;
                
                GCCollisionRect<float> fRect;
                fRect.SetRect( vRange.x, vRange.y, vRange.z, vRange.w );
                fRect.AdjustRect();
                
                D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
                SETVECTOR3( lefttop, fRect.m_Left, fRect.m_Top, 0.4f );
                SETVECTOR3( righttop, fRect.m_Right, fRect.m_Top, 0.4f );
                SETVECTOR3( leftbottom, fRect.m_Left, fRect.m_Bottom, 0.4f );
                SETVECTOR3( rightbottom, fRect.m_Right, fRect.m_Bottom, 0.4f );

                g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
                    lefttop, righttop, leftbottom, rightbottom,0,0,0,
                    D3DCOLOR_ARGB(76, 231, 158, 231));
            }
        }
    }
#endif
}

bool KGCSyncObjectManager::CheckCrashPlayer_Private( SyncObjectInstance *pInstance )
{
    bool bResult = false;

    if( NULL == pInstance )
        return bResult;

    D3DXVECTOR4 vRange;
    GCCollisionRect<float> kObjectRect, kCollision;
    vRange.x = pInstance->m_vPos.x - pInstance->m_pOriginalObject->m_fHalfWidth;
    vRange.y = pInstance->m_vPos.y - pInstance->m_pOriginalObject->m_fHalfHeight;
    vRange.z = pInstance->m_vPos.x + pInstance->m_pOriginalObject->m_fHalfWidth;
    vRange.w = pInstance->m_vPos.y + pInstance->m_pOriginalObject->m_fHalfHeight;
    
    kObjectRect.SetRect( vRange.x, vRange.y, vRange.z, vRange.w );
    kObjectRect.AdjustRect();
    
    for( int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

        if( pPlayer->m_kUserInfo.bLive == false )
            continue;

        if( pPlayer->m_cLife == 0 )
            continue;

        if( pPlayer->m_ePlayerState == PS_DEAD )
            continue;

        if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().START )
            continue;

        std::vector<int>::const_iterator vecIter = pInstance->m_pOriginalObject->m_vecNoCrashBuff.begin();
        for(; vecIter != pInstance->m_pOriginalObject->m_vecNoCrashBuff.end(); ++vecIter)
        {
            if( pPlayer->IsMagicEffect( *vecIter ) )
                break;
        }
        if( vecIter != pInstance->m_pOriginalObject->m_vecNoCrashBuff.end() )
            continue;

        GCCollisionRect<float> kPlayerRect = pPlayer->GetCollisionRect();
        kPlayerRect.m_Top -= 0.2f;  kPlayerRect.m_Bottom -= 0.2f;
        kPlayerRect.AdjustRect();
        if( kPlayerRect.CheckCollision( kObjectRect, &kCollision ) )
        {
            bResult = true;

            if( -1 < pInstance->m_pOriginalObject->m_iPlayerMagicEff )
            {
                pPlayer->SetMagicEffect( pInstance->m_pOriginalObject->m_iPlayerMagicEff, pInstance->m_pOriginalObject->m_fMagicEffTime, pInstance->m_pOriginalObject->m_iMagicEffLevel, true );
            }

            if( pInstance->m_pOriginalObject->m_bOnlyOneCrash )
                break;
        }
    }

    return bResult;
}

bool KGCSyncObjectManager::CheckCrashMonster_Private( SyncObjectInstance *pInstance )
{
    bool bResult = false;

    if( NULL == pInstance )
        return bResult;

    D3DXVECTOR4 vRange;
    GCCollisionRect<float> kObjectRect, kCollision;
    vRange.x = pInstance->m_vPos.x - pInstance->m_pOriginalObject->m_fHalfWidth;
    vRange.y = pInstance->m_vPos.y - pInstance->m_pOriginalObject->m_fHalfHeight;
    vRange.z = pInstance->m_vPos.x + pInstance->m_pOriginalObject->m_fHalfWidth;
    vRange.w = pInstance->m_vPos.y + pInstance->m_pOriginalObject->m_fHalfHeight;
    
    kObjectRect.SetRect( vRange.x, vRange.y, vRange.z, vRange.w );
    kObjectRect.AdjustRect();

	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit )
    {
		if ( mit->second == NULL )
			continue;

		MONSTER* pMonster = mit->second;

        if( !pMonster->IsLive() )
            continue;

        if( NPC_NONE != pMonster->m_eNPC )
            continue;

        if( pMonster->IsInvincible() )
            continue;

        if( pMonster->GetNoCrashDamage() )
            continue;

        if( pInstance->m_pOriginalObject->m_bNoCrashCharSummonMonster )
        {
            PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
            if( pPlayer )
                continue;
        }

        std::vector<int>::const_iterator vecNoCrashMonsterIter = pInstance->m_pOriginalObject->m_vecNoCrashMonsterType.begin();
        for(; vecNoCrashMonsterIter != pInstance->m_pOriginalObject->m_vecNoCrashMonsterType.end(); ++vecNoCrashMonsterIter)
        {
            if( pMonster->m_iMonsterType == *vecNoCrashMonsterIter )
                break;
        }
        if( vecNoCrashMonsterIter != pInstance->m_pOriginalObject->m_vecNoCrashMonsterType.end() )
            continue;

        std::vector<int>::const_iterator vecNoCrashBuffIter = pInstance->m_pOriginalObject->m_vecNoCrashBuff.begin();
        for(; vecNoCrashBuffIter != pInstance->m_pOriginalObject->m_vecNoCrashBuff.end(); ++vecNoCrashBuffIter)
        {
            if( pMonster->IsMagicEffect( pMonster->m_iMonsterIndex, *vecNoCrashBuffIter ) )
                break;
        }
        if( vecNoCrashBuffIter != pInstance->m_pOriginalObject->m_vecNoCrashBuff.end() )
            continue;

        for(int iAttackRectLoop=0; iAttackRectLoop<pMonster->GetAttackBoundRectNum(); ++iAttackRectLoop )
        {
            GCCollisionRect<float> kMonsterRect = pMonster->GetAttackBoundRect( iAttackRectLoop );
            kMonsterRect.m_Top -= 0.25f;    kMonsterRect.m_Bottom -= 0.25f;
            if( kMonsterRect.CheckCollision( kObjectRect, &kCollision ) )
            {
                bResult = true;
                break;
            }
        }

        if( bResult )
        {
            if( pInstance->m_pOriginalObject->m_bMonsterTargettingRelease )
            {
                pMonster->ReleaseTarget( true );
                pMonster->ReleaseTargetDamage();
                pMonster->ReleaseTempTargetPos();
                pMonster->ReleaseTargetSyncObject();
            }

            if( -1 < pInstance->m_pOriginalObject->m_iMonsterMagicEff )
            {
                pMonster->SetMagicEffect_NotMeBroad( pMonster->m_iMonsterIndex, pInstance->m_pOriginalObject->m_iMonsterMagicEff, pInstance->m_pOriginalObject->m_fMagicEffTime, pInstance->m_pOriginalObject->m_iMagicEffLevel, 0 );
            }

            if( pInstance->m_pOriginalObject->m_bOnlyOneCrash )
                break;
        }
    }

    return bResult;
}

void KGCSyncObjectManager::CheckCrashGround( SyncObjectInstance *pInstance, float fSpeedX, float fSpeedY )
{
    D3DXVECTOR4 fRange;
    fRange.x = pInstance->m_vPos.x + 1.0f;
    fRange.y = pInstance->m_vPos.y + 0.5f - pInstance->m_pOriginalObject->m_fHalfHeight;    // 아래
    fRange.z = pInstance->m_vPos.y + 0.5f + pInstance->m_pOriginalObject->m_fHalfHeight;    // 위
    fRange.w = pInstance->m_pOriginalObject->m_fHalfHeight * 2.0f;                          // 높이

    float fCollsionY = pInstance->m_vPos.y;
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        KGCContectInfo kInfo( fRange.x, fRange.y, fSpeedX, fSpeedY, false, false, true, fRange.z, fRange.w );
        if( true == g_MyD3D->MyStg->Check_Contact( kInfo ) )
        {
            fCollsionY = kInfo.m_y - 0.5f;
            pInstance->m_bIsGroundCrash = true;
        }
    }
    else
    {
        int iContactX, iContactY;
        D3DXVECTOR3 staticV;
        staticV = g_MyD3D->MyStg->Check_Contact( fRange.x, fRange.y, false, false, &iContactX, &iContactY);
        if( staticV.y != -1.0f )
        {
            fCollsionY = staticV.y - 0.5f;
            pInstance->m_bIsGroundCrash = true;
        }
    }

    if( pInstance->m_bIsGroundCrash )
    {
        pInstance->m_vPos.x += pInstance->m_pOriginalObject->m_fGroundOffsetX;
        pInstance->m_vPos.y = fCollsionY + pInstance->m_pOriginalObject->m_fGroundOffsetY;
    }
}

float KGCSyncObjectManager::GetGravityPos( SyncObjectInstance* pInstance ) 
{
    int iStartLife = pInstance->m_pOriginalObject->m_iStart_Life;
    int iChangeLife = pInstance->m_pOriginalObject->m_iChange_Life;
    if( 0 == iChangeLife ) iChangeLife = -1;
    if( pInstance->m_iLife == iStartLife ) iStartLife += 1;
    float fAge = (float)( (iStartLife - pInstance->m_iLife ) / iChangeLife );
    return (fAge * pInstance->m_pOriginalObject->m_fGravity);
}

int KGCSyncObjectManager::GetInstanceCountToID( int iID )
{
    std::map<int, int>::iterator mapIter = m_mapObjectInstanceCount.find( iID );
    if( mapIter != m_mapObjectInstanceCount.end() )
    {
        return mapIter->second;
    }
    return 0;
}

D3DXVECTOR2 KGCSyncObjectManager::GetInstancePos(DWORD dwUID)
{
    D3DXVECTOR2 vResult(0.0f, 0.0f);
    std::map<DWORD, SyncObjectInstance*>::iterator mapIter = m_mapObjectInstance.find( dwUID );
    if( mapIter != m_mapObjectInstance.end() && mapIter->second )
    {
        vResult.x = mapIter->second->m_vPos.x;
        vResult.y = mapIter->second->m_vPos.y;
    }

    if( 0.0f != vResult.x && 0.0f != vResult.y )
    {
        vResult.x += 1.0f;
        vResult.y += 0.5f;
    }

    return vResult;
}

SyncObjectOriginal* KGCSyncObjectManager::GetSyncObjectInfo_Private( int iObjectID )
{
    SyncObjectOriginal* pReturnValue = NULL;
    std::map<int, SyncObjectOriginal*>::iterator mapIter = m_mapObjectInfo.find( iObjectID );
    if( mapIter != m_mapObjectInfo.end() )
    {
        pReturnValue = mapIter->second;
    }
    else
    {
#if !defined(__PATH__)
        std::string strMessage;
        strMessage = iObjectID + "번 오브젝트 속성이 없습니다.(SyncObjectInfo.lua파일 확인)";
        MessageBoxA( NULL, strMessage.c_str(), "", MB_OK );
#endif
    }
    return pReturnValue;
}

DWORD KGCSyncObjectManager::AddSyncObject_BroadCast( int iObjectID, float fX, float fY )
{
    DWORD dwReturnValue = 0;
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        KGC_PID_BROAD_SYNCOBJECT kPacket;
        kPacket.dwUID = dwReturnValue = m_dwUIDCount;
        kPacket.iObjectID = iObjectID;
        kPacket.iSyncMode = SYNC_MODE_ADD;
        kPacket.fX = fX - 1.0f;
        kPacket.fY = fY - 0.5f;
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_SYNCOBJECT" );

        m_dwUIDCount = m_dwUIDCount + 1;
    }
    return dwReturnValue;
}

DWORD KGCSyncObjectManager::DelSyncObject_BroadCast(DWORD dwUID)
{
    DWORD dwReturnValue = false;
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        std::map<DWORD, SyncObjectInstance*>::iterator mapIter = m_mapObjectInstance.find( dwUID );
        if( mapIter != m_mapObjectInstance.end() )
        {
            KGC_PID_BROAD_SYNCOBJECT kPacket;
            kPacket.dwUID = dwUID;
            kPacket.iSyncMode = SYNC_MODE_DEL;
            SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
            KGCPC_MEMO( "KGC_PID_BROAD_SYNCOBJECT" );

            dwReturnValue = dwUID;
        }
    }
    return dwReturnValue;
}

DWORD KGCSyncObjectManager::AddSyncObject_NotMeBroad( int iObjectID, float fX, float fY )
{
    DWORD dwReturnValue = 0;
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        KGC_PID_BROAD_SYNCOBJECT kPacket;
        kPacket.dwUID = dwReturnValue = m_dwUIDCount;
        kPacket.iObjectID = iObjectID;
        kPacket.iSyncMode = SYNC_MODE_ADD;
        kPacket.fX = fX - 1.0f;
        kPacket.fY = fY - 0.5f;
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_SYNCOBJECT" );

        m_dwUIDCount = m_dwUIDCount + 1;

        AddSyncObject_Packet_Private( kPacket.dwUID, kPacket.iObjectID, kPacket.fX, kPacket.fY);
    }
    return dwReturnValue;
}

DWORD KGCSyncObjectManager::DelSyncObject_NotMeCast(DWORD dwUID)
{
    DWORD dwReturnValue = false;
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        std::map<DWORD, SyncObjectInstance*>::iterator mapIter = m_mapObjectInstance.find( dwUID );
        if( mapIter != m_mapObjectInstance.end() )
        {
            KGC_PID_BROAD_SYNCOBJECT kPacket;
            kPacket.dwUID = dwUID;
            kPacket.iSyncMode = SYNC_MODE_DEL;
            SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
            KGCPC_MEMO( "KGC_PID_BROAD_SYNCOBJECT" );

            dwReturnValue = dwUID;

            DelSyncObject_Packet_Private(kPacket.dwUID);
        }
    }
    return dwReturnValue;
}

DWORD KGCSyncObjectManager::AddSyncObject_Packet_Private(DWORD dwUID, int iObjectID, float fX, float fY)
{
    DWORD dwReturnValue = 0;
    std::map<DWORD, SyncObjectInstance*>::iterator mapIter = m_mapObjectInstance.find( dwUID );
    if( mapIter == m_mapObjectInstance.end() )
    {
        SyncObjectOriginal* pOriginalObject = GetSyncObjectInfo_Private( iObjectID );
        if( pOriginalObject )
        {
            SyncObjectInstance* pInstance = new SyncObjectInstance( dwUID, iObjectID, fX, fY, pOriginalObject );
            if( pInstance )
            {
                dwReturnValue = dwUID;
                int iCount = m_mapObjectInstanceCount[ pInstance->m_iObjectID ] + 1;
                m_mapObjectInstanceCount[ pInstance->m_iObjectID ] = iCount;
                m_mapObjectInstance.insert( std::make_pair(dwUID, pInstance) );
            }
        }
    }

    return dwReturnValue;
}

DWORD KGCSyncObjectManager::DelSyncObject_Packet_Private(DWORD dwUID)
{
    DWORD dwReturnValue = 0;
    std::map<DWORD, SyncObjectInstance*>::iterator mapIter = m_mapObjectInstance.find( dwUID );
    if( mapIter != m_mapObjectInstance.end() )
    {
        int iCount = m_mapObjectInstanceCount[ mapIter->second->m_iObjectID ] -1;
        m_mapObjectInstanceCount[ mapIter->second->m_iObjectID ] = iCount;
        SAFE_DELETE( mapIter->second );
        m_mapObjectInstance.erase( mapIter );
        dwReturnValue = dwUID;
    }
    return dwReturnValue;
}

void KGCSyncObjectManager::BindLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCSyncObjectManager>( L, "KGCSyncObjectManager" );
    lua_tinker::class_def<KGCSyncObjectManager>( L,  "InitCalss",               &KGCSyncObjectManager::InitCalss  );
    lua_tinker::class_def<KGCSyncObjectManager>( L,  "AddSyncObject_BroadCast", &KGCSyncObjectManager::AddSyncObject_BroadCast  );
    lua_tinker::class_def<KGCSyncObjectManager>( L,  "DelSyncObject_BroadCast", &KGCSyncObjectManager::DelSyncObject_BroadCast  );

    lua_tinker::class_def<KGCSyncObjectManager>( L,  "AddSyncObject_NotMeBroad",&KGCSyncObjectManager::AddSyncObject_NotMeBroad  );
    lua_tinker::class_def<KGCSyncObjectManager>( L,  "DelSyncObject_NotMeCast", &KGCSyncObjectManager::DelSyncObject_NotMeCast  );
    
    lua_tinker::class_def<KGCSyncObjectManager>( L,  "GetInstanceCount",        &KGCSyncObjectManager::GetInstanceCount  );
    lua_tinker::class_def<KGCSyncObjectManager>( L,  "GetInstanceCountToID",    &KGCSyncObjectManager::GetInstanceCountToID  );

    lua_tinker::decl( L, "KGCSyncObjectManager", this );
}

bool KGCSyncObjectManager::LoadScript()
{
    KLuaManager luaMgr( KGCLuabinder::getInstance().GetLuaState() );

    if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
    {
#if !defined( __PATH__ )
        MessageBoxA( NULL, "Enum.stg Load Failed!", "", MB_OK );
#endif
        return false;
    }

    if( GCFUNC::LoadLuaScript( luaMgr , "BuffEnum.stg" ) == false )
    {
#if !defined( __PATH__ )
        MessageBoxA( NULL, "BuffEnum.stg Load Failed!", "", MB_OK );
#endif
        return false;
    }

    if( GCFUNC::LoadLuaScript( luaMgr, "SyncObjectInfo.lua" ) == false )
    {
#if !defined( __PATH__ )
        MessageBoxA( NULL, "SyncObjectInfo.lua Load Failed!", "", MB_OK );
#endif
        return false;
    }

    if( FAILED( luaMgr.BeginTable( "SyncObject", false ) ) ) 
        return false;

    std::map<DWORD, SyncObjectInstance*>::iterator mapIter = m_mapObjectInstance.begin();
    for(; mapIter != m_mapObjectInstance.end(); ++mapIter)
    {
        DelSyncObject_BroadCast( mapIter->first );
    }
    DestroyCalss();

    int iSyncObjectLoop = 1;
    SyncObjectOriginal sSyncObject;
    while( SUCCEEDED( luaMgr.BeginTable( iSyncObjectLoop ) ) )
    {
        sSyncObject.Init();
        sSyncObject.m_iObjectID = iSyncObjectLoop;
        
        LUA_GET_VALUE_DEF( "CHANGE_LIFE", sSyncObject.m_iChange_Life, -1 );
        LUA_GET_VALUE_DEF( "START_LIFE", sSyncObject.m_iStart_Life, 0 );
        LUA_GET_VALUE_DEF( "MAGIC_EFF_TO_PLAYER", sSyncObject.m_iPlayerMagicEff, -1 );
        LUA_GET_VALUE_DEF( "MAGIC_EFF_TO_MONSTER", sSyncObject.m_iMonsterMagicEff, -1);
        LUA_GET_VALUE_DEF( "MAGIC_EFF_LEVEL", sSyncObject.m_iMagicEffLevel, 0);
        LUA_GET_VALUE_DEF( "MAGIC_EFF_TIME", sSyncObject.m_fMagicEffTime, 0.0f);
        LUA_GET_VALUE_DEF( "OFFSET_X", sSyncObject.m_fOffsetX, 0.0f );
        LUA_GET_VALUE_DEF( "OFFSET_Y", sSyncObject.m_fOffsetY, 0.0f );
        LUA_GET_VALUE_DEF( "HALF_WIDTH", sSyncObject.m_fHalfWidth, 0.0f );
        LUA_GET_VALUE_DEF( "HALF_HEIGHT", sSyncObject.m_fHalfHeight, 0.0f );
        LUA_GET_VALUE_DEF( "GRAVITY", sSyncObject.m_fGravity, 0.0f );
        LUA_GET_VALUE_DEF( "GROUND_OFFSET_X", sSyncObject.m_fGroundOffsetX, 0.0f );
        LUA_GET_VALUE_DEF( "GROUND_OFFSET_Y", sSyncObject.m_fGroundOffsetY, 0.0f );
        LUA_GET_VALUE_DEF( "CHECK_GROUND", sSyncObject.m_bCheckGround, false );
        LUA_GET_VALUE_DEF( "NOCHECK_GROUND_FRAME", sSyncObject.m_iNoCheckGroundFrame, false );
        LUA_GET_VALUE_DEF( "NO_CRASH_PLAYER", sSyncObject.m_bNoCrashPlayer, false );
        LUA_GET_VALUE_DEF( "NO_CRASH_MONSTER", sSyncObject.m_bNoCrashMonster, false );
        LUA_GET_VALUE_DEF( "MONSTER_TARGETTING_RELEASE", sSyncObject.m_bMonsterTargettingRelease, false );
        LUA_GET_VALUE_DEF( "CLEAR_CRASH", sSyncObject.m_bClearCrash, false );
        LUA_GET_VALUE_DEF( "ONLY_ONE_CRASH", sSyncObject.m_bOnlyOneCrash, false );
        LUA_GET_VALUE_DEF( "PARTICLE_TRACE", sSyncObject.m_bTraceParticle, false );
        LUA_GET_VALUE_DEF( "NO_CRASH_CHAR_SUMMONMONSTER", sSyncObject.m_bNoCrashCharSummonMonster, false );
        
        if( SUCCEEDED( luaMgr.BeginTable( "NO_CRASH_BUFF" ) ) )
        {
            int iLoop = 1, iTemp;
            while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
            {
                sSyncObject.m_vecNoCrashBuff.push_back( iTemp );
            }
            luaMgr.EndTable();
        }

        if( SUCCEEDED( luaMgr.BeginTable( "NO_CARASH_MONSTER_TYPE" ) ) )
        {
            int iLoop = 1, iTemp;
            while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
            {
                sSyncObject.m_vecNoCrashMonsterType.push_back( iTemp );
            }
            luaMgr.EndTable();
        }

        if( SUCCEEDED( luaMgr.BeginTable( "OBJECT_PARTICLE" ) ) )
        {
            int iParticleLoop = 1;
            SyncParticleInfo sParticle;
            while( SUCCEEDED( luaMgr.BeginTable( iParticleLoop++ ) ) )
            {
                sParticle.Init();
                LUA_GET_VALUE_DEF( 1, sParticle.m_strName, "" );
                LUA_GET_VALUE_DEF( 2, sParticle.m_fXOffset, 0.0f );
                LUA_GET_VALUE_DEF( 3, sParticle.m_fYOffset, 0.0f );
                LUA_GET_VALUE_DEF( 4, sParticle.m_fTraceTime, 0.5f );
                sSyncObject.m_vecParticle.push_back( sParticle );
                luaMgr.EndTable();
            }
            luaMgr.EndTable();
        }

        SyncObjectOriginal *pSyncObject = new SyncObjectOriginal( sSyncObject );
        m_mapObjectInfo.insert( std::make_pair(iSyncObjectLoop++, pSyncObject) );
        luaMgr.EndTable();
    }
    luaMgr.EndTable();

    return true;
}