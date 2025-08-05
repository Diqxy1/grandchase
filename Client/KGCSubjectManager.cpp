 /*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2013년 05월 23일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 달성과제 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "KGCSubjectManager.h"
#include "DamageManager.h"
#include "GCUI\GCGameOverlayUI.h"
#include "GCUI\GCHeadupDisplay.h"
#include "GCUI\GCOnGameExpBar.h"
#include "KGCMultipleLanguages.h"


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★액션 스크립트 파일모니터 등록★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#if !defined( __PATH__ )
#include "../GCUtil/GCFileMoniter.h"
static bool bReLoadSubject = false;
void ReLoadSubjectAction()
{
    bReLoadSubject	= true;
}
void KGCSubjectManager::CheckFileMonitor()
{
    if( bReLoadSubject )
    {
        LoadScript();
        bReLoadSubject = false;
    }
}
void KGCSubjectManager::RegisterFileMoniter( const char *szFileName )
{
    SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( szFileName ) , ReLoadSubjectAction );
}
#endif


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★★★★달성과제 매니져 클래스★★★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
ImplementSingleton( KGCSubjectManager )

KGCSubjectManager::KGCSubjectManager()
: m_kStrLoader( false )
{
    m_dwUIDCounter = 0;
    m_dwBeforeSubject = 0;
    m_mapSubjectInstance.clear();
    m_mapSubjectTemplate.clear();
    InitStats();
    DestoryAllInstance();
}

KGCSubjectManager::~KGCSubjectManager()
{
    DestoryAllInstance();
}

void KGCSubjectManager::InitStats()
{
    m_kSubjectStats.m_dwLoginUID = 0;
    m_kSubjectStats.m_nGameMode = 0;
    m_kSubjectStats.m_nDifficulty = 0;
    m_kSubjectStats.m_nCharType = 0;
    m_kSubjectStats.m_nCharLevel = 0;
    m_kSubjectStats.m_nCharPromotion = 0;
    m_kSubjectStats.m_mapSubjectClearInfo.clear();
}

void KGCSubjectManager::DestoryAllInstance()
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        SAFE_DELETE( mapIter->second );
    }
    m_mapSubjectInstance.clear();
    m_dwUIDCounter = 0;
}

void KGCSubjectManager::ClearInstance( DWORD dwUID )
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.find( dwUID );
    if( mapIter == m_mapSubjectInstance.end() )
        return;

    KGCSubject *pSubject = mapIter->second;
    if( pSubject )
    {
        std::map< int, std::vector< std::pair< int, bool > > >::iterator mapIter = m_kSubjectStats.m_mapSubjectClearInfo.find( pSubject->GetStageID() );
        if( mapIter != m_kSubjectStats.m_mapSubjectClearInfo.end() )
            mapIter->second.push_back( std::make_pair(pSubject->GetSubjectType(), (1 == pSubject->GetSucdcess()?true:false)) );
        else
        {
            std::vector< std::pair< int, bool > > vecStats;
            vecStats.push_back( std::make_pair(pSubject->GetSubjectType(), (1 == pSubject->GetSucdcess()?true:false)) );
            m_kSubjectStats.m_mapSubjectClearInfo.insert( make_pair(pSubject->GetStageID(), vecStats) );
        }
    }

    SAFE_DELETE( mapIter->second );
    m_mapSubjectInstance.erase( mapIter );
}

void KGCSubjectManager::FrameMove()
{
    std::vector<DWORD> vecDeleteList;
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        KGCSubject* pSubject = mapIter->second;
        if( NULL == pSubject )
            continue;

        pSubject->FrameMove();

        if( pSubject->m_bDeleteSubject )
            vecDeleteList.push_back( mapIter->first );
    }

    std::vector<DWORD>::iterator vecIter = vecDeleteList.begin();
    for(; vecIter != vecDeleteList.end(); ++vecIter)
    {
        ClearInstance( (*vecIter) );
    }

#if !defined( __PATH__ )
    CheckFileMonitor();
#endif
}

void KGCSubjectManager::TextRender()
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->TextRender();
    }
}

const KGCSubjectTemplate* KGCSubjectManager::GetSubjectTemplate( DWORD dwType )
{
    KGCSubjectTemplate *pTemplate = NULL;
    std::map<DWORD, KGCSubjectTemplate>::iterator mapIter = m_mapSubjectTemplate.find( dwType );
    if( mapIter != m_mapSubjectTemplate.end() )
        pTemplate = &mapIter->second;
    return pTemplate;
}

const std::map<DWORD, KGCSubject*>& KGCSubjectManager::GetAllSubjectInstance()
{
    return m_mapSubjectInstance;
}

KGCSubject* KGCSubjectManager::GetSubjectInstance( DWORD dwUID )
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.find( dwUID );
    if( mapIter == m_mapSubjectInstance.end() )
        return NULL;

    return mapIter->second;
}

KGCSubject* KGCSubjectManager::GetSubjectInstanceFromType( DWORD dwSubjectType )
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter )
    {
        KGCSubject* pSubject = mapIter->second;
        if( NULL == pSubject )
            continue;

        if( pSubject->GetSubjectType() == dwSubjectType )
            return pSubject;
    }

    return NULL;
}


int KGCSubjectManager::GetMaxSubject()
{
    return static_cast<int>(m_mapSubjectTemplate.size());
}

KGCSubject* KGCSubjectManager::CreateSubject( DWORD dwType, int iUserIndex )
{
    if( -1 != iUserIndex )
    {
        int iPlayer = g_MyD3D->Get_MyPlayer();
        if( iPlayer != iUserIndex )
            return NULL;
    }

    std::map<DWORD, KGCSubjectTemplate>::iterator mapTemplateIter = m_mapSubjectTemplate.find( dwType );
    if( mapTemplateIter == m_mapSubjectTemplate.end() )
        return NULL;

    m_dwUIDCounter = m_dwUIDCounter + 1;
    KGCSubject* pSubject = new KGCSubject( m_dwUIDCounter, &mapTemplateIter->second );
    if( pSubject )
    {
        if( NULL == pSubject->m_pSubjectTemplate )
        {
            SAFE_DELETE( pSubject );
            return NULL;
        }

        pSubject->m_pStrLoader = &m_kStrLoader;
        ClearInstance( m_dwUIDCounter );
        m_mapSubjectInstance.insert( std::make_pair(m_dwUIDCounter, pSubject) );
        pSubject->CallLuaFunc_Callback( KGCSubjectTemplate::CALLBACK_INIT );
    }

    return pSubject;
}

int KGCSubjectManager::GetSubjectNum()
{
    return static_cast<int>(m_mapSubjectInstance.size());
}

void KGCSubjectManager::SetReward( KEVENT_DUNGEON_SUBJECT_REWARD_ACK& kRecv )
{
    //DWORD                   m_dwLoginUID;       // 보상 요청한 유저
    //DWORD                   m_dwTriggerID;      // 보상받은 과제의 트리거 ID
    //DWORD                   m_dwSubjectID;      // 보상받은 과제 ID
    //int                     m_dwGainExp;        // 보상으로 받은 exp(받지않으면 0)
    //int                     m_dwGainGp;         // 보상으로 받은 gp(받지않으면 0)
    //std::vector< KItem >    m_vecRewards;       // 보상으로 받은 아이템들(받지않으면 비어있음)

    if( 0 != kRecv.m_dwGainExp )
        SetRewardExp( kRecv.m_dwLoginUID, kRecv.m_dwGainExp );

    if( false == kRecv.m_vecRewards.empty() )
        SetRewardItem( kRecv.m_vecRewards );
}

void KGCSubjectManager::SetRewardExp( DWORD dwPlayerUID, int iExp )
{
    PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwPlayerUID );
    if( NULL == pPlayer )
        return;

    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->Add(DT_EXP_EFFECT, pPlayer->m_iPlayerIndex, false , -1 , NULL , true);
    if( pkDamage )
    {
        float fOffset_X = 0.0f;
        float fOffset_Y = 0.0f;
        float fPlayerPos_X = g_kLocalPlayerLatency->kRenderData[0].vPos.x + 0.15f;
        float fPlayerPos_Y = g_kLocalPlayerLatency->kRenderData[0].vPos.y;
        float fShowPos_X = fPlayerPos_X;
        float fShowPos_Y = fPlayerPos_Y + 1.0f;

        D3DXVECTOR2 vecPos = g_pkGameOverlayUI->GetHeadUp()->m_pkExpBar->m_pkExpPercent->GetFixedWindowPos();
        D3DXVECTOR3& CameraPos = D3DXVECTOR3( g_kCamera.m_fLookX + 1.0f, g_kCamera.m_fLookY + 0.75f, 0.0f );
        
        float fWidth = g_kCamera.m_rtCamera.right - g_kCamera.m_rtCamera.left;
        float fHeight = g_kCamera.m_rtCamera.top - g_kCamera.m_rtCamera.bottom;
        
        float cwx = CameraPos.x - fWidth * 0.5f;
        float cwy = CameraPos.y - fHeight * 0.5f;

        float fRealPosX = cwx + (vecPos.x / 400.0f);
        float fRealPosY = cwy + (1.5f - vecPos.y / 400.0f);
        
        pkDamage->m_pOriginalDamage->SetMileStonePos( fShowPos_X, fShowPos_Y );
        pkDamage->m_pOriginalDamage->SetMileStonePos( fShowPos_X - randf() / 15.0f, fShowPos_Y + randf() / 15.0f );
        pkDamage->m_pOriginalDamage->SetMileStonePos( fShowPos_X , fShowPos_Y - randf() / 15.0f );
        pkDamage->m_pOriginalDamage->SetMileStonePos( fRealPosX, fRealPosY );
        pkDamage->m_pOriginalDamage->SetMileStonePos( fRealPosX, fRealPosY );

        pkDamage->m_pOriginalDamage->m_fDamageToPlayer = static_cast<float>(iExp);
    }
}

void KGCSubjectManager::SetRewardItem( std::vector< KItem >& kItem )
{
    std::vector< KItem >::iterator vecIter = kItem.begin();
    for(; vecIter != kItem.end(); ++vecIter)
    {
        GCItem* pItem = g_pItemMgr->GetItemData( vecIter->m_ItemID / 10 );
        if( NULL == pItem )
            continue;
        
        g_pItemMgr->m_kInventory.AddItem( *vecIter, true );
    }
}

void KGCSubjectManager::AddBackAttack()
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->AddBackAttack();
    }
}

void KGCSubjectManager::AddSpecial()
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->AddSpecial();
    }
}

void KGCSubjectManager::AddDamageHitCnt()
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->AddDamageHitCnt();
    }
}

void KGCSubjectManager::AddDamageVolume( float fDamage )
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->AddDamageVolume( fDamage );
    }
}

void KGCSubjectManager::AddDieCnt()
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->AddDieCnt();
    }
}

void KGCSubjectManager::SetComboBreak()
{
    std::map<DWORD, KGCSubject*>::iterator mapIter = m_mapSubjectInstance.begin();
    for(; mapIter != m_mapSubjectInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->SetComboBreak();
    }
}

void KGCSubjectManager::LoadScript()
{
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "MotionEnum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "DamageEnum.stg" ) == false )
        return;

    LoadSubjectTemplate( luaMgr );

    std::map<DWORD, KGCSubjectTemplate>::iterator mapTemplateIter = m_mapSubjectTemplate.begin();
    for(; mapTemplateIter != m_mapSubjectTemplate.end(); ++mapTemplateIter)
    {
        LoadActionScript( luaMgr, mapTemplateIter->second );
    }

    LoadString();

    DestoryAllInstance();
}

void KGCSubjectManager::LoadSubjectTemplate( KLuaManager &luaMgr )
{
    if( GCFUNC::LoadLuaScript( luaMgr, "BuffEnum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "SubjectTemplate.stg" ) == false )
        return;

    m_mapSubjectTemplate.clear();
    if( SUCCEEDED(luaMgr.BeginTable( "SubjectList" )) )
    {
        int iLoop = 1;
        KGCSubjectTemplate kSubjectTemplate;
        while( SUCCEEDED(luaMgr.BeginTable( iLoop++ )) )
        {
            kSubjectTemplate.Init();

            LUA_GET_VALUE_DEF( "ID", kSubjectTemplate.m_dwTypeID, kSubjectTemplate.m_dwTypeID );
            LUA_GET_VALUE_DEF( "ACTION_SCRIPT", kSubjectTemplate.m_strActionScript, kSubjectTemplate.m_strActionScript.c_str() );
            LUA_GET_VALUE_DEF( "ID_SIMPLE_DESC", kSubjectTemplate.m_dwSimpleDescStringID, kSubjectTemplate.m_dwSimpleDescStringID );

            LUA_GET_VALUE_DEF( "Exp", kSubjectTemplate.m_kRewardInfo.m_iExp, kSubjectTemplate.m_kRewardInfo.m_iExp );
            LUA_GET_VALUE_DEF( "GP", kSubjectTemplate.m_kRewardInfo.m_iGp, kSubjectTemplate.m_kRewardInfo.m_iGp );
            if( SUCCEEDED(luaMgr.BeginTable( "Buff" )) )
            {
                int iBuffLoop = 1;
                kSubjectTemplate.m_kRewardInfo.m_vecBuffList.clear();
                while( SUCCEEDED(luaMgr.BeginTable( iBuffLoop++ )) )
                {
                    KGCSubjectReward_Buff kBuff;
                    LUA_GET_VALUE_DEF( 1, kBuff.iBuffID, kBuff.iBuffID );
                    LUA_GET_VALUE_DEF( 2, kBuff.fBuffTime, kBuff.fBuffTime );
                    LUA_GET_VALUE_DEF( 3, kBuff.iBuffLevel, kBuff.iBuffLevel );
                    LUA_GET_VALUE_DEF( 4, kBuff.bIsLocalBuff, kBuff.bIsLocalBuff );
                    kSubjectTemplate.m_kRewardInfo.m_vecBuffList.push_back( kBuff );
                    luaMgr.EndTable();
                }
                luaMgr.EndTable();
            }

            if( SUCCEEDED(luaMgr.BeginTable( "Drop" )) )
            {
                int iItemLoop = 1;
                kSubjectTemplate.m_kRewardInfo.m_vecItemList.clear();
                while( SUCCEEDED(luaMgr.BeginTable( iItemLoop++ )) )
                {
                    KGCSubjectReward_Item kItem;
                    LUA_GET_VALUE_DEF( 1, kItem.dwItemID, kItem.dwItemID );
                    LUA_GET_VALUE_DEF( 2, kItem.iGrade, kItem.iGrade );
                    LUA_GET_VALUE_DEF( 3, kItem.iPeriod, kItem.iPeriod );
                    LUA_GET_VALUE_DEF( 4, kItem.iCount, kItem.iCount );
                    kSubjectTemplate.m_kRewardInfo.m_vecItemList.push_back( kItem );
                    luaMgr.EndTable();
                }
                luaMgr.EndTable();
            }
#if !defined( __PATH__ )
            RegisterFileMoniter( kSubjectTemplate.m_strActionScript.c_str() );
#endif

            m_mapSubjectTemplate.insert( std::make_pair( kSubjectTemplate.m_dwTypeID, kSubjectTemplate ) );
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }

#if !defined( __PATH__ )
    RegisterFileMoniter( "SubjectTemplate.stg" );
#endif
}

void KGCSubjectManager::LoadActionScript( KLuaManager &luaMgr, KGCSubjectTemplate &kSubjectTemplate )
{
    if( false == GCFUNC::LoadLuaScript( luaMgr, kSubjectTemplate.m_strActionScript ) ||
        false == GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), kSubjectTemplate.m_strActionScript ) )
        return;

    luaMgr.BeginTable( "ActionList" );
    {
        int iLoop = 1;
        KGCSubjectAction kActionInfo;
        while( SUCCEEDED(luaMgr.BeginTable( iLoop++ )) )
        {
            kActionInfo.Init();
            LUA_GET_VALUE_DEF( "ID",        kActionInfo.strStateID, "" );
            LUA_GET_VALUE_DEF( "Func",      kActionInfo.strFunctionName, "" );
            kActionInfo.m_iStateIndex = (iLoop - 2);

            kSubjectTemplate.m_vecActionList.push_back( std::make_pair( kActionInfo.strStateID, kActionInfo) );
            luaMgr.EndTable();
        }
        
        luaMgr.EndTable();
    }

    luaMgr.BeginTable( "CallbackList" );
    {
        std::string strFunc;
        for(int iLoop=0; iLoop<KGCSubjectTemplate::CALLBACK_MAX; ++iLoop)
        {
            LUA_GET_VALUE_DEF( kSubjectTemplate.GetCallBackKeyName(iLoop).c_str(),   strFunc, "" );
            if( 0 < static_cast<int>(strlen(strFunc.c_str())) )
                kSubjectTemplate.m_mapCallBackList.insert( std::make_pair(iLoop, strFunc) );
        }
        luaMgr.EndTable();
    }
}

void KGCSubjectManager::LoadString()
{
    if (SiKGCMultipleLanguages()->UseMultipleLanguages() == false)
    {
        m_kStrLoader.LoadFromKom("strSubject.stg");
    }
    else
    {
        //std::string strErrFileName = ".\\Stage\\err_strSubject";
        std::string strErrFileName = "strSubject_";
        strErrFileName += SiKGCMultipleLanguages()->GetLanguageTypeNUM();
        strErrFileName += ".stg";

        m_kStrLoader.LoadFromKom(strErrFileName.c_str());
    }
}

void KGCSubjectManager::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

    lua_tinker::class_add<KGCSubjectManager>( L, "KGCSubjectManager" );
    lua_tinker::class_def<KGCSubjectManager>( L, "GetSubjectInstance",              &KGCSubjectManager::GetSubjectInstance );
    lua_tinker::class_def<KGCSubjectManager>( L, "GetSubjectInstanceFromType",      &KGCSubjectManager::GetSubjectInstanceFromType );
    lua_tinker::class_def<KGCSubjectManager>( L, "GetMaxSubject",                   &KGCSubjectManager::GetMaxSubject );

    lua_tinker::decl( L, "KGCSubjectManager", this );
}