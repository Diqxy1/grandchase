 /*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2012년 11월 15일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 탑승물 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "KGCEmbarkManager.h"

/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★액션 스크립트 파일모니터 등록★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#if !defined( __PATH__ )
#include "../GCUtil/GCFileMoniter.h"
static bool bReLoadEmbark = false;
void ReLoadEmbarkAction()
{
    bReLoadEmbark	= true;
}
void KGCEmbarkManager::CheckFileMonitor()
{
    if( bReLoadEmbark )
    {
        LoadScript();
        bReLoadEmbark = false;
    }
}
void KGCEmbarkManager::RegisterFileMoniter( const char *szFileName )
{
    SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( szFileName ) , ReLoadEmbarkAction );
}
#endif


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★★★탑승물 템플릿 클래스★★★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
void KGCEmbarkTemplate::Init()
{
    m_dwID = 0;
    m_iNameID = -1;
    m_fMaxHp = 0.0f;
    m_fScale = 1.0f;

    m_fAttackWidth = 0.0f;
    m_fAttackHeight = 0.0f;
    m_fOffsetAttackWidth = 0.0f;
    m_fOffsetAttackHeight = 0.0f;

    m_fPushWidth = 0.0f;
    m_fPushHeight = 0.0f;
    m_fOffsetPushWidth = 0.0f;
    m_fOffsetPushHeight = 0.0f;

    m_fNameOffsetX = 0.0f;
    m_fNameOffsetY = 0.0f;
    m_iDefaultCharUpBoneID = -1;

    m_bNoPushCheck = false;
    m_bOnlyPosBone = false;
    m_bCheckPortal = false;
    m_bNotDrawPetName = false;

    m_iUpCharMotion = -1;
    m_iDownCharMotion = -1;

    m_strActionScript.clear();
    m_vecMeshInfo.clear();
    m_vecActionList.clear();
    m_mapCallBackList.clear();
}

std::string KGCEmbarkTemplate::GetCallBackKeyName( int iIndex )
{
    switch( iIndex )
    {
    case CALLBACK_INIT:
        return "Init";
    case CALLBACK_UP_EMBARKATION:
        return "UpEmbarkation";
    case CALLBACK_DOWN_EMBARKATION:
        return "DownEmbarkation";
    case CALLBACK_LANDING:
        return "LandToOwner";
    case CALLBACK_CHANGE_HP:
        return "ChangeHP";
    case CALLBACK_DIE:
        return "DieOwner";
    case CALLBACK_EMBARKATOR_DIE:
        return "DieEmbarkator";
    case CALLBACK_CHANGE_CHAR_HP:
        return "ChangeCharHP";
    case CALLBACK_CRASH_DAMAGE:
        return "CrashDamage";
    case CALLBACK_SYNC:
        return "Sync";
    }
    return "";
}

std::string KGCEmbarkTemplate::GetCallBackFuncName( int iIndex ) const
{
    std::map<int, std::string>::const_iterator mapIter = m_mapCallBackList.find( iIndex );
    if( mapIter != m_mapCallBackList.end() )
        return mapIter->second;
    return "";
}


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★★★★탑승물 매니져 클래스★★★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
ImplementSingleton( KGCEmbarkManager )

KGCEmbarkManager::KGCEmbarkManager()
: m_dwUIDCount( 0 )
, m_iUIDLatencyCnt( 0 )
{
    m_mapEmbarkInstance.clear();
    m_mapEmbarkTemplate.clear();
    m_vecDeleteList.clear();
    m_vecAddList.clear();
    DestoryAllInstance( true );
}

KGCEmbarkManager::~KGCEmbarkManager()
{
    DestoryAllInstance( true );
}

void KGCEmbarkManager::DestoryAllInstance( bool bDirectDelete )
{
    std::map<DWORD, KGCEmbarkObject*>::iterator mapIter = m_mapEmbarkInstance.begin();
    if( mapIter == m_mapEmbarkInstance.end() )
        return;

    if( bDirectDelete )
    {
        for(; mapIter != m_mapEmbarkInstance.end(); ++mapIter)
        {
            SAFE_DELETE( mapIter->second );
        }
        m_mapEmbarkInstance.clear();
        m_dwUIDCount = 0;
    }
    else
    {
        for(; mapIter != m_mapEmbarkInstance.end(); ++mapIter)
        {
            m_vecDeleteList.push_back( mapIter->first );
        }
    }
}

void KGCEmbarkManager::ClearInstance( DWORD dwUID )
{
    if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
    {
        SiKGCEmbarkManager()->DestoryAllInstance();
    }
    else
    {
        KGC_PID_BROAD_EMBARK_DELETE kPacket;
        kPacket.dwEmbarkUID = dwUID;
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_DELETE" );
    }
}

void KGCEmbarkManager::ClearInstanceByLocal( DWORD dwUID, bool bDirectDelete )
{
    std::map<DWORD, KGCEmbarkObject*>::iterator mapIter = m_mapEmbarkInstance.find( dwUID );
    if( mapIter == m_mapEmbarkInstance.end() )
        return;

    if( bDirectDelete )
    {
        SAFE_DELETE( mapIter->second );
        m_mapEmbarkInstance.erase( mapIter );
    }
    else
    {
        m_vecDeleteList.push_back( mapIter->first );
    }
}

void KGCEmbarkManager::SetCountUID(DWORD dwUID, bool bNoCheckHost)
{
    if( false == g_kGlobalValue.m_kUserInfo.bHost &&
        false == bNoCheckHost )
        return;

    m_dwUIDCount = dwUID;
}

DWORD KGCEmbarkManager::GetCountUIDWithAutoCount()
{
    if( false == g_kGlobalValue.m_kUserInfo.bHost )
        return 0;

    m_dwUIDCount++;
    if( UINT_MAX == m_dwUIDCount )
        m_dwUIDCount = 1;

    return m_dwUIDCount;
}

KGCEmbarkObject* KGCEmbarkManager::CreateEmbarkObject( int iType, DWORD dwUID )
{
    std::map<DWORD, KGCEmbarkTemplate>::iterator mapIterOfTemplate = m_mapEmbarkTemplate.find( iType );
    if( mapIterOfTemplate == m_mapEmbarkTemplate.end() )
        return NULL;
        
    // UID에 해당하는 탑승물이 이미 있다면 AddList가 Instance에 등록되는 시점에 제거된다.
    KGCEmbarkObject *pEmbark = new KGCEmbarkObject( dwUID, &mapIterOfTemplate->second );
    m_vecAddList.push_back( pEmbark );
    return pEmbark;
}

void KGCEmbarkManager::SummonEmbark( KGC_PID_BROAD_EMBARK_SUMMON_ACK *pData )
{
    if( NULL == pData )
        return;

    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pData->dwPlayerUID );
    KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->CreateEmbarkObject( pData->iEmbarkType, pData->dwEmbarkUID );
    if( NULL == pEmbark )
        return;

    //pEmbark->LoadActionScript();

    bool bIsRight;
    float fPosX, fPosY;
    if( pPlayer && (0.0f == pData->fPosX) && (0.0f == pData->fPosY) )
    {
        fPosX = pPlayer->GetX();
        fPosY = pPlayer->GetY();
        bIsRight = pPlayer->GetIsRight();
    }
    else
    {
        fPosX = pData->fPosX;
        fPosY = pData->fPosY;
        bIsRight = pData->bIsRight;
    }

    float fHalfWidth = ((pEmbark->GetPushWidth()/400.0f)/2.0f);
    if( fPosX < fHalfWidth )
        fPosX = fHalfWidth;
    if( fPosX > (SiKGCFantasticMap()->GetPixelWidth() - fHalfWidth) )
        fPosX = SiKGCFantasticMap()->GetPixelWidth() - fHalfWidth;

    pEmbark->SetPos( fPosX, fPosY );
    pEmbark->SetIsRight( bIsRight );

    int iCopyLatency;
    if( pEmbark->IsMyOwnerOfEmbarkObject() )
        iCopyLatency = (LATENCY + SHOWLATENCY - 1);
    else
        iCopyLatency = pEmbark->GetLatency();
    pEmbark->LatencyAllEqual( iCopyLatency );
    pEmbark->StartEmbark();

    if( pPlayer && pData->bEmbakation )
    {
        pEmbark->SetUpEmbarkation( pData->dwPlayerUID, pEmbark->GetUpCharMotion(), pEmbark->GetUpCallback() );
        pPlayer->SetEmbarkAdaptBoneID( pEmbark->GetDefaultCharUpBoneID() );
        pPlayer->m_kEmbarkArena.m_bSendPacket = false;
    }

    if( pPlayer )
    {
        pPlayer->SetEmbarkTypeOfInPortal( INT_MAX );
        pPlayer->SetEmbarkUIDOfInPortal( INT_MAX );
    }
}

void KGCEmbarkManager::AddEmbark( int iType, float fPosX, float fPosY, bool bIsRight, bool bEmbakation, DWORD dwPlayerUID )
{
    KGC_PID_BROAD_EMBARK_SUMMON_REQ kPacket;
    kPacket.dwPlayerUID = dwPlayerUID;
    kPacket.dwEmbarkUID = 0;
    kPacket.iEmbarkType = iType;
    kPacket.fPosX = fPosX;
    kPacket.fPosY = fPosY;
    kPacket.bIsRight = bIsRight;
    kPacket.bEmbakation = bEmbakation;

    if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
    {
        KGC_PID_BROAD_EMBARK_SUMMON_ACK kAckPacket;
        kAckPacket.dwPlayerUID = dwPlayerUID;
        kAckPacket.dwEmbarkUID = GetCountUIDWithAutoCount();
        kAckPacket.iEmbarkType = iType;
        kAckPacket.fPosX = fPosX;
        kAckPacket.fPosY = fPosY;
        kAckPacket.bIsRight = bIsRight;
        kAckPacket.bEmbakation = bEmbakation;
        SummonEmbark( &kAckPacket );
    }
    else
    {
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_SUMMON_REQ" );
    }
}

void KGCEmbarkManager::StartEmbark()
{
    if( false == m_vecAddList.empty() )
    {
        std::vector<KGCEmbarkObject*>::iterator vecIter = m_vecAddList.begin();
        for(; vecIter != m_vecAddList.end(); ++vecIter)
        {
            (*vecIter)->StartEmbark();
        }
    }

    std::map<DWORD, KGCEmbarkObject*>::iterator mapIter = m_mapEmbarkInstance.begin();
    for(; mapIter != m_mapEmbarkInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->StartEmbark();
    }
}

void KGCEmbarkManager::FrameMove()
{
    std::map<DWORD, KGCEmbarkObject*>::iterator mapIter = m_mapEmbarkInstance.begin();
    for(; mapIter != m_mapEmbarkInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->FrameMove();
        if( mapIter->second->GetDeleteReservation() )
            ClearInstanceByLocal( mapIter->first );
    }

    FrameMove_DeleteList();
    FrameMove_AddList();

    if ( !m_mapEmbarkInstance.empty() )
    {
    if( 0 < m_iUIDLatencyCnt )
        {
        --m_iUIDLatencyCnt;
        }
    else
        {
        SendEmbarkUIDSync();
        }
    }

#if !defined( __PATH__ )
    CheckFileMonitor();
#endif
}

void KGCEmbarkManager::FrameMove_DeleteList()
{
    if( 0 == static_cast<int>(m_vecDeleteList.size()) )
        return;

    if( static_cast<int>(m_vecDeleteList.size()) == static_cast<int>(m_mapEmbarkInstance.size()) )
        m_dwUIDCount = 0;

    std::vector<DWORD>::iterator vecDeleteIter = m_vecDeleteList.begin();
    for(; vecDeleteIter != m_vecDeleteList.end(); ++vecDeleteIter)
    {
        ClearInstanceByLocal( (*vecDeleteIter), true );
    }
    m_vecDeleteList.clear();
}

void KGCEmbarkManager::FrameMove_AddList()
{
    std::vector<KGCEmbarkObject*>::iterator vecAddIter = m_vecAddList.begin();
    for(; vecAddIter != m_vecAddList.end(); ++vecAddIter)
    {
        (*vecAddIter)->LoadResource();
        ClearInstanceByLocal( (*vecAddIter)->GetEmbarkUID(), true );
        m_mapEmbarkInstance[(*vecAddIter)->GetEmbarkUID()] = (*vecAddIter);
    }
    m_vecAddList.clear();
}

void KGCEmbarkManager::SendEmbarkUIDSync()
{
    if( false == g_kGlobalValue.m_kUserInfo.bHost )
        return;


    KGC_PID_BROAD_EMBARK_SYNC_UID kPacket;
    kPacket.dwEmbarkUIDCnt = m_dwUIDCount;
    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _UNRELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_SYNC_UID" );

    m_iUIDLatencyCnt = LATENCY;
}

void KGCEmbarkManager::TextRender()
{
    std::map<DWORD, KGCEmbarkObject*>::iterator mapIter = m_mapEmbarkInstance.begin();
    for(; mapIter != m_mapEmbarkInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->TextRender();
    }
}

void KGCEmbarkManager::CheckStageMoveEmbarkation()
{
    std::map<DWORD, KGCEmbarkObject*>::iterator mapIter = m_mapEmbarkInstance.begin();
    for(; mapIter != m_mapEmbarkInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;
        
        if( false == mapIter->second->IsCheckPortal() )
            continue;
        
        PLAYER *pPlayer = mapIter->second->GetEmbarkationPlayer();
        if( NULL == pPlayer )
            continue;
        
        pPlayer->SetEmbarkTypeOfInPortal( mapIter->second->GetEmbarkType() );
        pPlayer->SetEmbarkUIDOfInPortal( mapIter->second->GetEmbarkUID() );
    }
}

const KGCEmbarkTemplate* KGCEmbarkManager::GetEmbarkTemplate( int iType )
{
    KGCEmbarkTemplate *pTemplate = NULL;
    std::map<DWORD, KGCEmbarkTemplate>::iterator mapIter = m_mapEmbarkTemplate.find( iType );
    if( mapIter != m_mapEmbarkTemplate.end() )
        pTemplate = &mapIter->second;
    return pTemplate;
}

KGCEmbarkObject* KGCEmbarkManager::GetEmbarkInstance( DWORD dwUID )
{
    if( false == m_vecAddList.empty() )
    {
        std::vector<KGCEmbarkObject*>::iterator vecIter = m_vecAddList.begin();
        for(; vecIter != m_vecAddList.end(); ++vecIter)
        {
            if( dwUID == (*vecIter)->GetEmbarkUID() )
            {
                return (*vecIter);
            }
        }
    }

    std::map<DWORD, KGCEmbarkObject*>::iterator mapIter = m_mapEmbarkInstance.find( dwUID );
    if( mapIter == m_mapEmbarkInstance.end() )
        return NULL;

    return mapIter->second;
}

void KGCEmbarkManager::SetCharMotion( int iMotionID )
{
    if( -1 == iMotionID )
        return;

    m_vecCharMotionList.push_back( iMotionID );
}

int KGCEmbarkManager::GetEmbarkationCount()
{
    int iCount = 0;
    std::map<DWORD, KGCEmbarkObject*>::iterator vecIter = m_mapEmbarkInstance.begin();
    for(; vecIter != m_mapEmbarkInstance.end(); ++vecIter)
    {
        if( NULL == vecIter->second )
            continue;

        if( vecIter->second->IsEmbarkation() )
            iCount++;
    }
    return iCount;
}

void KGCEmbarkManager::LoadScript()
{
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "MotionEnum.stg" ) == false )
        return;

    LoadEmbarkTemplate( luaMgr );

    std::map<DWORD, KGCEmbarkTemplate>::iterator mapTemplateIter = m_mapEmbarkTemplate.begin();
    for(; mapTemplateIter != m_mapEmbarkTemplate.end(); ++mapTemplateIter)
    {
        LoadActionScript( luaMgr, mapTemplateIter->second );
    }

#if !defined( __PATH__ )
    std::vector<KGC_PID_BROAD_EMBARK_SUMMON_ACK> vecReLoadList;
    std::map<DWORD, KGCEmbarkObject*>::iterator mapInstanceIter = m_mapEmbarkInstance.begin();
    for(; mapInstanceIter != m_mapEmbarkInstance.end(); ++mapInstanceIter)
    {
        if( false == mapInstanceIter->second->m_bIsReload )
            continue;

        KGC_PID_BROAD_EMBARK_SUMMON_ACK kEmbark;
        kEmbark.iEmbarkType = mapInstanceIter->second->GetEmbarkType();
        kEmbark.fPosX = mapInstanceIter->second->GetPosX();
        kEmbark.fPosY = mapInstanceIter->second->GetPosY();
        kEmbark.bIsRight = mapInstanceIter->second->GetIsRight();
        vecReLoadList.push_back( kEmbark );
    }

    DestoryAllInstance( true );

    std::vector<KGC_PID_BROAD_EMBARK_SUMMON_ACK>::iterator vecReloadIter = vecReLoadList.begin();
    for(; vecReloadIter != vecReLoadList.end(); ++vecReloadIter )
    {
        DWORD dwUID = GetCountUIDWithAutoCount();
        vecReloadIter->dwEmbarkUID = dwUID;
        SummonEmbark( &(*vecReloadIter) );
    }
#endif
}

void KGCEmbarkManager::LoadEmbarkTemplate( KLuaManager &luaMgr )
{
    if( GCFUNC::LoadLuaScript( luaMgr, "EmbarkTemplate.stg" ) == false )
        return;

    m_vecCharMotionList.clear();
    m_mapEmbarkTemplate.clear();
    if( SUCCEEDED(luaMgr.BeginTable( "EmbarkList" )) )
    {
        int iLoop = 1;
        KGCEmbarkTemplate kEmbarkTemplate;
        while( SUCCEEDED(luaMgr.BeginTable( iLoop++ )) )
        {
            kEmbarkTemplate.Init();

            LUA_GET_VALUE_DEF( "ID", kEmbarkTemplate.m_dwID, kEmbarkTemplate.m_dwID );
            LUA_GET_VALUE_DEF( "ACTION_SCRIPT", kEmbarkTemplate.m_strActionScript, kEmbarkTemplate.m_strActionScript.c_str() );
            LUA_GET_VALUE_DEF( "NAMEID", kEmbarkTemplate.m_iNameID, kEmbarkTemplate.m_iNameID );
            LUA_GET_VALUE_DEF( "MAXHP", kEmbarkTemplate.m_fMaxHp, kEmbarkTemplate.m_fMaxHp );
            LUA_GET_VALUE_DEF( "SCALE", kEmbarkTemplate.m_fScale, kEmbarkTemplate.m_fScale );

            LUA_GET_VALUE_DEF( "ATTACK_WIDTH", kEmbarkTemplate.m_fAttackWidth, kEmbarkTemplate.m_fAttackWidth );
            LUA_GET_VALUE_DEF( "ATTACK_HEIGHT", kEmbarkTemplate.m_fAttackHeight, kEmbarkTemplate.m_fAttackHeight );   
            LUA_GET_VALUE_DEF( "OFFSET_ATTACK_WIDTH", kEmbarkTemplate.m_fOffsetAttackWidth, kEmbarkTemplate.m_fOffsetAttackWidth );
            LUA_GET_VALUE_DEF( "OFFSET_ATTACK_HEIGHT", kEmbarkTemplate.m_fOffsetAttackHeight, kEmbarkTemplate.m_fOffsetAttackHeight );   

            LUA_GET_VALUE_DEF( "PUSH_WIDTH", kEmbarkTemplate.m_fPushWidth, kEmbarkTemplate.m_fPushWidth );
            LUA_GET_VALUE_DEF( "PUSH_HEIGHT", kEmbarkTemplate.m_fPushHeight, kEmbarkTemplate.m_fPushHeight );
            LUA_GET_VALUE_DEF( "OFFSET_PUSH_WIDTH", kEmbarkTemplate.m_fOffsetPushWidth, kEmbarkTemplate.m_fOffsetPushWidth );
            LUA_GET_VALUE_DEF( "OFFSET_PUSH_HEIGHT", kEmbarkTemplate.m_fOffsetPushHeight, kEmbarkTemplate.m_fOffsetPushHeight );

            LUA_GET_VALUE_DEF( "NAME_OFFSET_X", kEmbarkTemplate.m_fNameOffsetX, kEmbarkTemplate.m_fNameOffsetX );
            LUA_GET_VALUE_DEF( "NAME_OFFSET_Y", kEmbarkTemplate.m_fNameOffsetY, kEmbarkTemplate.m_fNameOffsetY );

            LUA_GET_VALUE_DEF( "NO_PUSH_CHECK", kEmbarkTemplate.m_bNoPushCheck, kEmbarkTemplate.m_bNoPushCheck );
            LUA_GET_VALUE_DEF( "ADAPT_ONLYPOS", kEmbarkTemplate.m_bOnlyPosBone, kEmbarkTemplate.m_bOnlyPosBone );
            LUA_GET_VALUE_DEF( "DEFAULT_CHARUP_BONE_ID", kEmbarkTemplate.m_iDefaultCharUpBoneID, kEmbarkTemplate.m_iDefaultCharUpBoneID );
            
            LUA_GET_VALUE_DEF( "CHECK_PORTAL", kEmbarkTemplate.m_bCheckPortal, kEmbarkTemplate.m_bCheckPortal );
            LUA_GET_VALUE_DEF( "NOT_DRAW_PETNAME", kEmbarkTemplate.m_bNotDrawPetName, kEmbarkTemplate.m_bNotDrawPetName );
            

            if( SUCCEEDED(luaMgr.BeginTable( "CharacterMotionList" )) )
            {
                LUA_GET_VALUE_DEF( "UP_MOTION", kEmbarkTemplate.m_iUpCharMotion, kEmbarkTemplate.m_iUpCharMotion );
                LUA_GET_VALUE_DEF( "DOWN_MOTION", kEmbarkTemplate.m_iDownCharMotion, kEmbarkTemplate.m_iDownCharMotion );
                SetCharMotion( kEmbarkTemplate.m_iUpCharMotion );
                SetCharMotion( kEmbarkTemplate.m_iDownCharMotion );

                if( SUCCEEDED(luaMgr.BeginTable( "ETC_MOTION" )) )
                {
                    int iMotionID=0;
                    int iMotionLoop=1;
                    std::vector<int> vecMotionID;
                    while( SUCCEEDED(luaMgr.GetValue(iMotionLoop++, iMotionID)) ) vecMotionID.push_back( iMotionID );

                    std::vector<int>::iterator vecIter = vecMotionID.begin();
                    for(; vecIter != vecMotionID.end(); ++vecIter)
                    {
                        SetCharMotion( *vecIter );
                    }
                    luaMgr.EndTable();
                }
                luaMgr.EndTable();
            }

            if( SUCCEEDED(luaMgr.BeginTable( "Mesh" )) )
            {
                int iMeshIndex = 1;
                while( SUCCEEDED(luaMgr.BeginTable( iMeshIndex++ )) )
                {
                    EXTRA_MESH mesh;
                    LUA_GET_VALUE_DEF( 1, mesh.strMeshName, "" );
                    LUA_GET_VALUE_DEF( 2, mesh.strTextureName, "" );
                    LUA_GET_VALUE_DEF( 3, mesh.dwCartoon, 0 );
                    LUA_GET_VALUE_DEF( 4, mesh.dwTechnique, 0 );
                    mesh.iBoneId = -1;
                    kEmbarkTemplate.m_vecMeshInfo.push_back( mesh );
                    luaMgr.EndTable();
                }
                luaMgr.EndTable();
            }

#if !defined( __PATH__ )
            RegisterFileMoniter( kEmbarkTemplate.m_strActionScript.c_str() );
#endif

            m_mapEmbarkTemplate.insert( std::make_pair( kEmbarkTemplate.m_dwID, kEmbarkTemplate ) );
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }
}

void KGCEmbarkManager::LoadActionScript( KLuaManager &luaMgr, KGCEmbarkTemplate &kEmbarkTemplate )
{
    if( GCFUNC::LoadLuaScript( luaMgr, "DamageEnum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "BuffEnum.stg" ) == false )
        return;

    if( false == GCFUNC::LoadLuaScript( luaMgr, kEmbarkTemplate.m_strActionScript ) ||
        false == GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), kEmbarkTemplate.m_strActionScript ) )
        return;

    luaMgr.BeginTable( "ActionList" );
    {
        int iLoop = 1;
        KGCEmbarkAction kActionInfo;
        while( SUCCEEDED(luaMgr.BeginTable( iLoop++ )) )
        {
            kActionInfo.Init();
            LUA_GET_VALUE_DEF( "ID",        kActionInfo.strStateID, "" );
            LUA_GET_VALUE_DEF( "Motion",    kActionInfo.strMotionName, "" );
            LUA_GET_VALUE_DEF( "Func",      kActionInfo.strFunctionName, "" );
            LUA_GET_VALUE_DEF( "OwnerFunc", kActionInfo.strOwnerFunctionName, "" );
            kActionInfo.m_iStateIndex = (iLoop - 2);

            kEmbarkTemplate.m_vecActionList.push_back( std::make_pair( kActionInfo.strStateID, kActionInfo) );
            luaMgr.EndTable();
        }
        
        luaMgr.EndTable();
    }
    
    luaMgr.BeginTable( "CallbackList" );
    {
        std::string strFunc;
        for(int iLoop=0; iLoop<KGCEmbarkTemplate::CALLBACK_MAX; ++iLoop)
        {
            LUA_GET_VALUE_DEF( kEmbarkTemplate.GetCallBackKeyName(iLoop).c_str(),   strFunc, "" );
            if( 0 < static_cast<int>(strlen(strFunc.c_str())) )
                kEmbarkTemplate.m_mapCallBackList.insert( std::make_pair(iLoop, strFunc) );
        }
        luaMgr.EndTable();
    }
}

void KGCEmbarkManager::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCEmbarkManager>( L, "KGCEmbarkManager" );
    lua_tinker::class_def<KGCEmbarkManager>( L, "GetEmbarkInstance",        &KGCEmbarkManager::GetEmbarkInstance );
    lua_tinker::class_def<KGCEmbarkManager>( L, "GetEmbarkationCount",      &KGCEmbarkManager::GetEmbarkationCount );
    lua_tinker::class_def<KGCEmbarkManager>( L, "AddEmbark",                &KGCEmbarkManager::AddEmbark );
    lua_tinker::class_def<KGCEmbarkManager>( L, "ClearInstance",            &KGCEmbarkManager::ClearInstance );
    lua_tinker::class_def<KGCEmbarkManager>( L, "ClearInstanceByLocal",     &KGCEmbarkManager::ClearInstanceByLocal );

    lua_tinker::decl( L, "KGCEmbarkManager", this );
}