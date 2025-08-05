#include "stdafx.h"
//
#include "KGCRoomManager.h"
#include "VersionDef.h"
//
//
#include "MyD3D.h"
#include "QuestGameManager.h"
#include "Stage.h"

#include "KGCChannelManager.h"

#include "GCStateMachine.h"
//
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/KGCRoomList.h"
#include "KGCDeathMatch.h"
#include "GCStateGame.h"

#include "GCUI/KGCHurryUPDlg.h"
#include "KGCRoomListManager.h"
#include "KGCMatchManager.h"
#include "KGCInfinityDungeonManager.h"

ImplementSingleton(KGCRoomManager)


std::vector< std::wstring > KGCRoomManager::m_vecDungeonRoomNameList;
std::vector< std::wstring > KGCRoomManager::m_vecRandomRoomNameList;

KGCRoomManager::KGCRoomManager(void)
{

    m_iSelectedModeCategoryIndex = 0;
    m_iSelectedModeIndex = 0;
    m_iSelectedStageIndex = 0;
    m_iSelectedSubMode = GC_SGM_NO_ITEM;
    m_iDropItemGettingRule = DRR_TURN_RANDOM;
    m_bRandomMap = false;
    m_bChannelRoomList = false;
    m_bCreateToJoin = false;
    m_bIsPlayedHeroDungeon = false;

    m_kRoomInfo.eGameModeCategory = GC_GMC_MATCH;
    m_kRoomInfo.eGameMode = GC_GM_TEAM_MODE;
    m_kRoomInfo.nSubGameMode = GC_SGM_NO_ITEM;
    m_kRoomInfo.m_cRoutingMethod = DRR_TURN_RANDOM;
    m_kRoomInfo.bDeathMatchBalancing = true;
    m_kRoomInfo.bDeathMatchIntrudable = false;
    m_tmPreQuestionStartAutoGame = 0;

    m_kPrevRoomInfo = m_kRoomInfo;

    m_prRcvRoomInfo.first = false;

    vecValidCategory.clear();

    m_cvBanInfo.set_capacity(SBanInfo::MAX_BANINFO_NUM);
    m_cvBanInfo.clear();

    // 던젼에서 랜덤으로 사용될 방이름 초기화
    m_vecDungeonRoomNameList.clear();
    m_vecDungeonRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_CREATE_DUNGEON_ROOM_TITLE1));
    m_vecDungeonRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_CREATE_DUNGEON_ROOM_TITLE2));
    m_vecDungeonRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_CREATE_DUNGEON_ROOM_TITLE3));
    m_vecDungeonRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_CREATE_DUNGEON_ROOM_TITLE4));

    // 랜덤으로 사용될 방이름 초기화
    m_vecRandomRoomNameList.clear();
    m_vecRandomRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_RANDOM_ROOM_NAME1));
    m_vecRandomRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_RANDOM_ROOM_NAME2));
    m_vecRandomRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_RANDOM_ROOM_NAME3));
    m_vecRandomRoomNameList.push_back(g_pkStrLoader->GetString(STR_ID_RANDOM_ROOM_NAME4));


    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCRoomManager>(L, "KGCRoomManager");
    lua_tinker::class_def<KGCRoomManager>(L, "GetGameModeCategory", &KGCRoomManager::GetGameModeCategory);
    lua_tinker::class_def<KGCRoomManager>(L, "GetGameMode", &KGCRoomManager::GetGameMode);
    lua_tinker::class_def<KGCRoomManager>(L, "GetSubGameMode", &KGCRoomManager::GetSubGameMode);
    lua_tinker::class_def<KGCRoomManager>(L, "GetDungeonLevel", &KGCRoomManager::GetDungeonLevel);
    lua_tinker::class_def<KGCRoomManager>(L, "GetGameModeCategoryCStr", &KGCRoomManager::GetGameModeCategoryCStr);
    lua_tinker::class_def<KGCRoomManager>(L, "GetGameModeCStr", &KGCRoomManager::GetGameModeCStr);
    lua_tinker::class_def<KGCRoomManager>(L, "GetSubGameModeCStr", &KGCRoomManager::GetSubGameModeCStr);
    lua_tinker::decl(KGCLuabinder::getInstance().GetLuaState(), "RoomMgr", this);
}

KGCRoomManager::~KGCRoomManager(void)
{
    m_vecGameModeCategory.clear();
    m_vecShowGameModeCategory.clear();
    m_mapChannelCategory.clear();
}

// -------------------------------------------------------------------------------------

std::vector<KGCGameModeCategory> KGCRoomManager::GetGameModeCategoryVector()
{
    ASSERT(m_vecShowGameModeCategory.size() != 0);
    return m_vecShowGameModeCategory;
}

std::vector<KGCGameMode> KGCRoomManager::GetGameModeVector()
{
    std::vector<KGCGameModeCategory> vecGameModeCategory = GetGameModeCategoryVector();
    if (m_iSelectedModeCategoryIndex >= (int)vecGameModeCategory.size())
    {
        if (vecGameModeCategory.empty()) {
            UpdateShowGameModeCategory(false, SiKGCChannelManager()->GetCurrentChannelType());
            vecGameModeCategory = GetGameModeCategoryVector();
            m_iSelectedModeCategoryIndex = 0;
        }
        else
        {
            m_iSelectedModeCategoryIndex = (int)vecGameModeCategory.size() - 1;
        }
    }

    ASSERT(vecGameModeCategory.size() != 0);
    return m_vecShowGameModeCategory[m_iSelectedModeCategoryIndex].m_vecGameMode;
}

std::vector<KGCGameStage> KGCRoomManager::GetGameStageVector()
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_ENTER_LIMIT) || g_kGlobalValue.ServerInfo.CheckServerType(ST_CREATE_LIMIT))
    {
        std::vector<KGCGameModeCategory>::iterator itor;

        for (itor = m_vecGameModeCategory.begin(); itor != m_vecGameModeCategory.end(); ++itor)
        {
            if (GC_GMC_INDIGO == (*itor).m_eGameModeCategory && false == (*itor).m_vecGameMode.empty())
                return (*itor).m_vecGameMode[0].m_vecGameStage;
        }
    }

    std::vector<KGCGameMode> vecGameMode = GetGameModeVector();
    if (m_iSelectedModeIndex >= (int)vecGameMode.size()) {
        m_iSelectedModeIndex = 0;
        if (vecGameMode.empty()) {
            UpdateShowGameModeCategory(false, SiKGCChannelManager()->GetCurrentChannelType());
            vecGameMode = GetGameModeVector();
        }
    }
    ASSERT(vecGameMode.size() != 0);
    return m_vecShowGameModeCategory[m_iSelectedModeCategoryIndex].m_vecGameMode[m_iSelectedModeIndex].m_vecGameStage;
}

void KGCRoomManager::LoadStageSpecification()
{
    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, "Enum.stg") == false)
        return;

    if (SUCCEEDED(luaMgr.BeginTable("StageSpecification")))
    {
        m_mapStageSpecification.clear();
        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                int iMapID = -1;
                KGCStageSpecification sStageSpecification;
                LUA_GET_VALUE("MapID", iMapID, break; );
                if (SUCCEEDED(luaMgr.BeginTable("GameMode")))
                {
                    for (int j = 1; ; ++j)
                    {
                        int iGameMode = GC_GM_INVALID;
                        LUA_GET_VALUE_NOASSERT(j, iGameMode, break; );
                        sStageSpecification.m_setGameMode.insert((EGCGameMode)iGameMode);
                    }
                    luaMgr.EndTable();
                }

                if (FAILED(luaMgr.GetValue("FootHold", sStageSpecification.m_strFootHoldPostString)))
                {
                    sStageSpecification.m_strFootHoldPostString.clear();
                }
                if (FAILED(luaMgr.GetValue("Sky", sStageSpecification.m_strSkyPostString)))
                {
                    sStageSpecification.m_strSkyPostString.clear();
                }
                m_mapStageSpecification[iMapID].push_back(sStageSpecification);
            }
            LUA_END_TABLE(break);
        }
        luaMgr.EndTable();
    }

}

void KGCRoomManager::LoadGameMode()
{
    int i = 0;

    // 초기화
    m_vecGameModeCategory.clear();
    m_vecCheckDungeonGameMode.clear();

    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, "Enum.stg") == false)
        return;

    if (GCFUNC::LoadLuaScript(luaMgr, "GameModeTable.lua") == false)
        return;

    //	사내에서만 동작, 게임모드 스크립트에서 가져올수 있다.
#if !defined( __PATH__ )
    if (!GC_GLOBAL_DEFINE::bLoadGameModeScript)
        return;

    //우선 모든 카테고리를 다 읽는다.
    LUA_BEGIN_TABLE("FullCategory", assert(0))
    {
        m_vecGameModeCategory.clear();
        int iTemp = 0;
        for (int j = 1; ; ++j)
        {
            LUA_GET_VALUE_NOASSERT(j, iTemp, break)
                KGCGameModeCategory kGameModeCategory;
            kGameModeCategory.m_vecGameMode.clear();
            kGameModeCategory.m_bRandomMap = false;
            kGameModeCategory.m_eGameModeCategory = (EGCGameModeCategory)iTemp;

            m_vecGameModeCategory.push_back(kGameModeCategory);
        }
    }
    LUA_END_TABLE(assert(0));

    //채널별로 해당되는 카테고리를 읽는다.
    LUA_BEGIN_TABLE("ChannelCategory", assert(0))
    {
        m_mapChannelCategory.clear();
        //채널별로 어떠한 카테고리를 담아야 할지를 추려낸다.
        for (i = 0; i < (int)CT_MAX; ++i)
        {
            LUA_BEGIN_TABLE(i, continue)
            {
                std::vector<EGCGameModeCategory> vecCategory;
                vecCategory.clear();
                for (int j = 1; ; ++j)
                {
                    int iTemp = 0;
                    LUA_GET_VALUE_NOASSERT(j, iTemp, break)
                        vecCategory.push_back((EGCGameModeCategory)iTemp);
                }
                // 2007/3/13. iridology. 사내이면 용사의섬 넣음
                //#if defined( _DEBUG ) || !defined( __PATH__ )
                //{
                //    if( i == CT_UNLIMITED )
                //        vecCategory.push_back( GC_GMC_INDIGO );
                //}
                //#endif

                m_mapChannelCategory[(EChannelType)i] = vecCategory;
            }
            LUA_END_TABLE(assert(0))
        }
    }
    LUA_END_TABLE(assert(0));

    //카테고리에 해당하는 모드를 읽는다.
    LUA_BEGIN_TABLE("CategoryMode", assert(0))
    {
        m_vecCheckDungeonGameMode.clear();
        for (i = 0; i < (int)m_vecGameModeCategory.size(); ++i)
        {
            LUA_BEGIN_TABLE((int)m_vecGameModeCategory[i].m_eGameModeCategory, continue)
            {
                for (int j = 1; ; ++j)
                {
                    int iTemp = 0;
                    LUA_GET_VALUE_NOASSERT(j, iTemp, break)
                        KGCGameMode kGameMode;
                    kGameMode.m_eGameMode = (EGCGameMode)iTemp;
                    kGameMode.m_vecGameStage.clear();
                    m_vecGameModeCategory[i].m_vecGameMode.push_back(kGameMode);

                    if (GC_GMC_DUNGEON == m_vecGameModeCategory[i].m_eGameModeCategory)
                        m_vecCheckDungeonGameMode.push_back(kGameMode.m_eGameMode);
                }
            }
            LUA_END_TABLE(assert(0));
        }
    }
    LUA_END_TABLE(assert(0))

        //카테고리에 해당하는 Stage들을 읽는다. - 여기서 Quest랑 몬스터 원정대(hunt)는 모드별로 Stage가 다르기 때문에 여기서 읽어두지 않는다.
        //근데 그 전에 Stage에 몬스터들을 셋팅해 주자.
        KSafeArray<KGCGameStage, NUM_TOTAL_STAGE> akGameStage;
    for (i = 0; i < NUM_TOTAL_STAGE; ++i)
    {
        akGameStage[i].m_eGameStage = (EGCGameStage)i;
        akGameStage[i].m_nSubGameMode = GC_SGM_NORMAL;
    }

    //본격적으로 Category에 Stage들을 읽어보자. --퀘스트랑 몬스터 hunt는 제외
    LUA_BEGIN_TABLE("CategoryStage", assert(0))
    {
        for (int i = 0; i < (int)m_vecGameModeCategory.size(); ++i)
        {
            LUA_BEGIN_TABLE((int)m_vecGameModeCategory[i].m_eGameModeCategory, continue)
            {
                int iTemp = 0;
                for (int j = 1; ; ++j)
                {
                    LUA_GET_VALUE_NOASSERT(j, iTemp, break)
                        assert(iTemp < NUM_TOTAL_STAGE&& iTemp >= 0);
                    for (int k = 0; k < (int)m_vecGameModeCategory[i].m_vecGameMode.size(); ++k)
                    {
                        m_vecGameModeCategory[i].m_vecGameMode[k].m_vecGameStage.push_back(akGameStage[iTemp]);
                    }
                }
            }
            LUA_END_TABLE(assert(0))
        }
    }
    LUA_END_TABLE(assert(0))

        //Random Map possible한지 읽자.
        LUA_BEGIN_TABLE("RandomMapPossibleCategory", return)
    {
        std::vector<EGCGameModeCategory> vecTemp;
        vecTemp.clear();

        int iTemp = 0;
        for (int j = 1; ; ++j)
        {
            LUA_GET_VALUE_NOASSERT(j, iTemp, break)
                assert(iTemp < GC_GMC_COUNT&& iTemp > GC_GMC_INVALID);
            vecTemp.push_back((EGCGameModeCategory)iTemp);
        }

        for (int k = 0; k < (int)vecTemp.size(); ++k)
        {
            for (int l = 0; l < (int)m_vecGameModeCategory.size(); ++l)
            {
                if (m_vecGameModeCategory[l].m_eGameModeCategory == vecTemp[k])
                {
                    m_vecGameModeCategory[l].m_bRandomMap = true;
                }
            }
        }
    }
    LUA_END_TABLE(return);
#endif


}

// 채널에 따라서 해당 채널에서 플레이 할 수 있는 게임모드를 업데이트 한다!
void KGCRoomManager::UpdateShowGameModeCategory(bool bCategoryFullMode, EChannelType eChannelType)
{
    m_vecShowGameModeCategory.clear();

    if (bCategoryFullMode)
    {
        for (int i = 0; i < (int)m_vecGameModeCategory.size(); ++i)
            m_vecShowGameModeCategory.push_back(m_vecGameModeCategory[i]);

        return;
    }

    // 채널에서 사용할 수 있는 카테고리를 얻어온다
    std::map< EChannelType, std::vector< EGCGameModeCategory > >::iterator itor = m_mapChannelCategory.find(eChannelType);
    std::vector<EGCGameModeCategory> vecGameModeCategory;

    if (itor != m_mapChannelCategory.end())
        vecGameModeCategory = itor->second;
    else
        vecGameModeCategory = m_mapChannelCategory[CT_UNLIMITED];

    // 전체 게임모드 중에서 해당 채널에서 사용 할 수 있는 녀석만 걸러서 세팅해 준다
    for (int i = 0; i < (int)vecGameModeCategory.size(); ++i)
    {
        if (vecGameModeCategory[i] == GC_GMC_COUNT) continue;

        for (int j = 0; j < (int)m_vecGameModeCategory.size(); ++j)
        {
            if (vecGameModeCategory[i] == (int)m_vecGameModeCategory[j].m_eGameModeCategory)
            {
                m_vecShowGameModeCategory.push_back(m_vecGameModeCategory[j]);
            }
        }
    }
}

// 현재 선택된 카테고리에 따라서 게임모드를 초기화 해 준다
// 카테고리 + 게임모드 + 스테이지 까지 세팅한다
void KGCRoomManager::ResetGameMode()
{

    // 현재 채널에서 플레이 가능한 모드들의 정보를 얻어온다
    std::vector<KGCGameModeCategory>  vecGameModeCategory = GetGameModeCategoryVector();
    std::vector<KGCGameMode>          vecGameMode = GetGameModeVector();
    std::vector<KGCGameStage>        vecGameStage = GetGameStageVector();

    // 얻어온 정보를 룸에 적용시켜 준다
    m_kRoomInfo.eGameModeCategory = vecGameModeCategory[m_iSelectedModeCategoryIndex].m_eGameModeCategory;
    m_kRoomInfo.eGameMode = vecGameMode[m_iSelectedModeIndex].m_eGameMode;
    m_bRandomMap = vecGameModeCategory[m_iSelectedModeCategoryIndex].m_bRandomMap;

    if (GetGameModeCategory() == GC_GMC_DUNGEON)
        m_kRoomInfo.ucStage = (UCHAR)g_MyD3D->m_pkQuestGameMgr->GetQuestIndexFromGameMode(m_kRoomInfo.eGameMode);
    else if (GetGameModeCategory() == GC_GMC_AGIT)
        m_kRoomInfo.ucStage = 1;
    else if (GetGameModeCategory() != GC_GMC_MONSTER_HUNT &&
        GetGameModeCategory() != GC_GMC_MONSTER_CRUSADER &&
        GetGameModeCategory() != GC_GMC_MINIGAME &&
        GetGameModeCategory() != GC_GMC_DOTA)
        m_kRoomInfo.ucStage = (UCHAR)vecGameStage[m_iSelectedStageIndex].m_eGameStage;

    m_kRoomInfo.iMonsterID = MON_VS_ENT;
    m_kRoomInfo.iMonsterCount = 1;
}

void KGCRoomManager::ResetSubGameMode()
{
    switch (GetGameModeCategory())
    {
    case GC_GMC_DUNGEON:
    {
        // 이건 따로 다른데서 컨트롤 하므로.. 여기서 건들지 말자..
        break;
    }
    case GC_GMC_MONSTER_CRUSADER:
    case GC_GMC_MONSTER_HUNT:
    case GC_GMC_INDIGO:         //
    case GC_GMC_TAG_MATCH:      // 던전 레벨을 0으로 해야 통계에 남는다
    case GC_GMC_MATCH:          //
    case GC_GMC_GUILD_BATTLE:   //
    case GC_GMC_TUTORIAL:       //
    case GC_GMC_DEATHMATCH:
    case GC_GMC_DOTA:
    case GC_GMC_AUTOMATCH:
    case GC_GMC_FATAL_DEATHMATCH:
    {
        // 던전레벨이 몬스터 원정대에서는 단계로 쓰인다
        SetDungeonLevel(0);
        // 몬스터 나오면 무조건 노템임..
        m_iSelectedSubMode = GC_SGM_NO_ITEM;
        break;
    }
    default:
    {
        m_iSelectedSubMode = GC_SGM_NO_ITEM;
        break;
    }
    }

    SetSubGameMode(m_iSelectedSubMode);
}

bool KGCRoomManager::SetGameModeCategory(EGCGameModeCategory eGameModeCategory)
{
    std::vector<KGCGameModeCategory> vecGameModeCategory = GetGameModeCategoryVector();

    for (int i = 0; i < (int)vecGameModeCategory.size(); ++i)
    {
        // 플레이 할 수 있는 카테고리인지 체크해 본다
        if (eGameModeCategory == vecGameModeCategory[i].m_eGameModeCategory)
        {
            m_kRoomInfo.eGameModeCategory = eGameModeCategory;
            m_bRandomMap = vecGameModeCategory[i].m_bRandomMap;
            m_iSelectedModeCategoryIndex = i;
            //g_MyD3D->m_kGuideMgr.SetCurrentMode( eGameModeCategory );

            return true;
        }
    }
    return false;
}

bool KGCRoomManager::SetGameMode(EGCGameMode eGameMode)
{
    std::vector<KGCGameMode> vecGameMode = GetGameModeVector();

    for (int i = 0; i < (int)vecGameMode.size(); ++i)
    {
        if (eGameMode == vecGameMode[i].m_eGameMode)
        {
            m_kRoomInfo.eGameMode = eGameMode;
            m_iSelectedModeIndex = i;
            return true;
        }
    }
    return false;
}

bool KGCRoomManager::SetGameModeStage(EGCGameStage eStage)
{
    std::vector<KGCGameStage> vecGameStage = GetGameStageVector();

    for (int i = 0; i < (int)vecGameStage.size(); ++i)
    {
        if (vecGameStage[i].m_eGameStage == eStage)
        {
            g_MyD3D->MyStg->SetStage(eStage);

            m_iSelectedStageIndex = i;
            return true;
        }
    }

    return false;
}

bool KGCRoomManager::SetSubGameMode(int nSubGameMode)
{
    if (nSubGameMode >= GC_SGM_NORMAL)
    {
        m_kRoomInfo.nSubGameMode = nSubGameMode;
        m_iSelectedSubMode = nSubGameMode;

        if (GetGameModeCategory() == GC_GMC_DUNGEON ||
            g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
        {
            int iLevel = 0;
            iLevel = nSubGameMode - GC_SGM_DUNGEON_NORMAL;

            m_kRoomInfo.nDungeonLevel = iLevel;
            SiKGCRoomListManager()->SetCurrentLevel(iLevel);
        }

        return true;
    }
    return false;
}

void KGCRoomManager::SetSubDungeonGameMode(int nSubGameMode)
{
    m_kRoomInfo.nSubGameMode = nSubGameMode + GC_SGM_DUNGEON_NORMAL;
    m_iSelectedSubMode = nSubGameMode + GC_SGM_DUNGEON_NORMAL;
    m_kRoomInfo.nDungeonLevel = nSubGameMode;
}

void KGCRoomManager::InitGameModeDirect(const EGCGameModeCategory eGameModeCategory, const EGCGameMode eGameMode)
{
    // 인덱스 초기화
    m_iSelectedModeCategoryIndex = 0;
    m_iSelectedModeIndex = 0;
    m_iSelectedStageIndex = 0;
    m_kRoomInfo.bRandomMapMode = false;

    if (eGameModeCategory == GC_GMC_DUNGEON)
    {
        SetGameMode(eGameMode);
    }
    else
    {

    }

    //인덱스를 따라서 모드를 바꿉시다.
    ResetGameMode();
    ResetSubGameMode();

}

void KGCRoomManager::InitGameMode()
{
    // 인덱스 초기화
    m_iSelectedModeCategoryIndex = 0;
    m_iSelectedModeIndex = 0;
    m_iSelectedStageIndex = 0;
    m_kRoomInfo.bRandomMapMode = false;

    if (GetGameModeCategory() == GC_GMC_DUNGEON ||
        g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
    {
        SetGameMode(SiKGCWorldMapManager()->GetGameMode());
    }

    if (g_MyD3D->m_pStateMachine->GetState() == GS_MATCH)
    {
        if (SiKGCRoomListManager()->GetRoomFilter() == GC_GMC_MATCH)
            SetGameMode(GC_GM_TEAM_MODE);
        else if (SiKGCRoomListManager()->GetRoomFilter() == GC_GMC_TAG_MATCH)
            SetGameMode(GC_GM_TAG_TEAM);
        else if (SiKGCRoomListManager()->GetRoomFilter() == GC_GMC_DEATHMATCH)
            SetGameMode(GC_GM_DEATH_TEAM);
        else if (SiKGCRoomListManager()->GetRoomFilter() == GC_GMC_GUILD_BATTLE)
            SetGameMode(GC_GM_GUILD_BATTLE);
        else if (SiKGCRoomListManager()->GetRoomFilter() == GC_GMC_DOTA)
            SetGameMode(GC_GM_DOTA);
        else if (SiKGCRoomListManager()->GetRoomFilter() == GC_GMC_FATAL_DEATHMATCH)
            SetGameMode(GC_GM_FATAL_DEATH_TEAM);

    }

    // 2007.03.14 wony
    // 용사의 섬이면 이놈을 true로 해야할듯?
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
    {
        m_kRoomInfo.bRandomMapMode = true;
        RefreshRandomMap();
    }

    //인덱스를 따라서 모드를 바꿉시다.
    ResetGameMode();
    ResetSubGameMode();
}

void KGCRoomManager::InitGameModeParty(EGCGameModeCategory eGameModeCategory, EGCGameMode eGameMode, EGCGameStage eGameStage, int nSubGameMode, bool IsDungeon)
{
    // 인덱스 초기화
    m_iSelectedModeCategoryIndex = 0;
    m_iSelectedModeIndex = 0;
    m_iSelectedStageIndex = 0;
    m_kRoomInfo.bRandomMapMode = false;


    if (IsDungeon)
        SetGameMode(SiKGCWorldMapManager()->GetGameMode());
    else
    {
        SiKGCRoomListManager()->SetRoomFilter(eGameModeCategory);
        m_kRoomInfo.nSubGameMode = nSubGameMode;

        if (eGameModeCategory == GC_GMC_DEATHMATCH ||
            eGameModeCategory == GC_GMC_FATAL_DEATHMATCH)
        {
            m_kRoomInfo.nDungeonLevel = 0;
            m_kRoomInfo.nSubGameMode = GC_SGM_NO_ITEM;
        }

    }

    //인덱스를 따라서 모드를 바꿉시다.
    ResetGameModeParty(eGameModeCategory, eGameMode, eGameStage, IsDungeon);
}

void KGCRoomManager::ResetGameModeParty(EGCGameModeCategory eGamemodeCategory, EGCGameMode eGameMode, EGCGameStage eGameStage, bool IsDungeon)
{
    // 얻어온 정보를 룸에 적용시켜 준다
    m_kRoomInfo.eGameModeCategory = eGamemodeCategory;
    m_kRoomInfo.eGameMode = eGameMode;

    if (IsDungeon)
        m_kRoomInfo.ucStage = (UCHAR)g_MyD3D->m_pkQuestGameMgr->GetQuestIndexFromGameMode(m_kRoomInfo.eGameMode);
    else
        m_kRoomInfo.ucStage = (UCHAR)eGameStage;

    m_kRoomInfo.iMonsterID = MON_VS_ENT;
    m_kRoomInfo.iMonsterCount = 1;
}

inline bool IsCharOptionEqual(KSafeArray<bool, GC_CHAR_NUM>& old_, KSafeArray<bool, GC_CHAR_NUM>& new_)
{
    for (int i = 0; i < GC_CHAR_NUM; i++)
    {
        if (old_[i] != new_[i])
        {
            return false;
        }
    }

    return true;
}

inline bool IsRankOptionEqual(KSafeArray<bool, GC_BG_GRADE_MAX>& old_, KSafeArray<bool, GC_BG_GRADE_MAX>& new_)
{
    for (int i = 0; i < GC_BG_GRADE_MAX; i++)
    {
        if (old_[i] != new_[i])
        {
            return false;
        }
    }

    return true;
}

void KGCRoomManager::WritePvPRoomOptions()
{
    if (GetGameModeCategory() == GC_GMC_DUNGEON)
        return;

    if (m_kRoomInfo.m_bDisablePetAttackPvP)
    {
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(STR_ID_CREATE_PVP_ROOM_DISABLED_PET_OPTION), KGCChatManager::CHAT_CMD_ADMIN);
    }

    if (m_kRoomInfo.m_bDisableSP4SkillsPvP)
    {
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(STR_ID_CREATE_PVP_ROOM_DISABLED_SPECIAL4_OPTION), KGCChatManager::CHAT_CMD_ADMIN);
    }

    if (m_kRoomInfo.m_bEnablePvPBallance)
    {
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(STR_ID_CREATE_PVP_ROOM_ATTR_CORRECTION_OPTION), KGCChatManager::CHAT_CMD_ADMIN);
    }

    std::wstring strBlockedChars = L"";
    for (int i = 0; i < GC_CHAR_NUM; i++)
    {
        if (m_kRoomInfo.m_saAllowedChars[i] == false)
        {
            strBlockedChars += L"/";

            strBlockedChars += g_kGlobalValue.GetCharName(i);

            strBlockedChars += L"/";
        }
    }

    if (!strBlockedChars.empty())
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetReplacedString(STR_ID_CREATE_PVP_ROOM_NOT_ALLOWED_CHARS_OPTION, "L", strBlockedChars), KGCChatManager::CHAT_CMD_BROADCAST);

    std::wstring strAllowedRanks = L"";

    for (int i = 0; i < GC_BG_GRADE_MAX; i++)
    {
        if (m_kRoomInfo.m_saAllowedRankings[i] == true)
        {
            strAllowedRanks += L"/";

            strAllowedRanks += g_kGlobalValue.GetRankScoreName((i + 1));

            strAllowedRanks += L"/";
        }
    }

    if (!strAllowedRanks.empty())
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetReplacedString(STR_ID_CREATE_PVP_ROOM_ALLOWED_RANKS_OPTION, "L", strAllowedRanks), KGCChatManager::CHAT_CMD_BROADCAST);
}

void KGCRoomManager::UpdateRoomInfo(KRoomInfo* pkRoomInfo)
{
    if (pkRoomInfo != NULL)
        m_kRoomInfo = *pkRoomInfo;

    KChangeRoomInfo kRoomInfo;
    m_kRoomInfo.SetChangeRoomInfo(&kRoomInfo);
    UpdateRoomInfo(kRoomInfo);

    KGCDeathMatch* pGamemode = (KGCDeathMatch*)g_pStateGame->GetGameModePtr(GC_GMC_DEATHMATCH);
    pGamemode->m_fTime = (float)m_kRoomInfo.nDeathMatchTime;
    pGamemode->m_iGoal = m_kRoomInfo.nDeathKillCount;

    pGamemode = (KGCDeathMatch*)g_pStateGame->GetGameModePtr(GC_GMC_FATAL_DEATHMATCH);
    pGamemode->m_fTime = (float)m_kRoomInfo.nDeathMatchTime;
    pGamemode->m_iGoal = m_kRoomInfo.nDeathKillCount;
}

void KGCRoomManager::UpdateRoomInfo(KChangeRoomInfo kRoomInfo)
{
    SetGameModeCategory((EGCGameModeCategory)kRoomInfo.m_cGameCategory);
    SetGameMode((EGCGameMode)kRoomInfo.m_iGameMode);
    SetGameModeStage((EGCGameStage)kRoomInfo.m_iMapID);
    SetSubGameMode(kRoomInfo.m_iSubGameMode);
    SetDropItemGettingRule(kRoomInfo.m_cRoutingMethod);

    m_kRoomInfo = kRoomInfo;
}

void KGCRoomManager::UpdateRoomInfo(KRoomOptions kOptions)
{
    m_kRoomInfo = kOptions;

    SRoomInfo oldOptions = SiKGCRoomManager()->GetPrevRoomInfo();

    if (g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost == true)
    {
        g_pkUIScene->RoomButtonSetStartDelay(219);
    }

    SiKGCRoomManager()->GetPrevRoomInfo() = SiKGCRoomManager()->GetRoomInfo();
    SiKGCRoomManager()->UpdateRoomUI();
    int iStrID = 0;

    if (oldOptions.m_bDisablePetAttackPvP != m_kRoomInfo.m_bDisablePetAttackPvP)
    {
        iStrID = m_kRoomInfo.m_bDisablePetAttackPvP ? STR_ID_CHANGE_PVP_ROOM_DISABLED_PET_OPTION : STR_ID_CHANGE_PVP_ROOM_ENABLED_PET_OPTION;
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(iStrID), KGCChatManager::CHAT_CMD_ADMIN);
    }

    if (oldOptions.m_bDisableSP4SkillsPvP != m_kRoomInfo.m_bDisableSP4SkillsPvP)
    {
        iStrID = m_kRoomInfo.m_bDisableSP4SkillsPvP ? STR_ID_CHANGE_PVP_ROOM_DISABLED_SPECIAL4_OPTION : STR_ID_CHANGE_PVP_ROOM_ENABLED_SPECIAL4_OPTION;
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(iStrID), KGCChatManager::CHAT_CMD_ADMIN);
    }

    if (oldOptions.m_bEnablePvPBallance != m_kRoomInfo.m_bEnablePvPBallance)
    {
        iStrID = m_kRoomInfo.m_bEnablePvPBallance ? STR_ID_CHANGE_PVP_ROOM_DISABLED_ATTR_CORRECTION_OPTION : STR_ID_CHANGE_PVP_ROOM_ENABLED_ATTR_CORRECTION_OPTION;
        g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(iStrID), KGCChatManager::CHAT_CMD_ADMIN);
    }

    if (!IsCharOptionEqual(oldOptions.m_saAllowedChars, m_kRoomInfo.m_saAllowedChars))
    {
        if (kOptions.m_dwDisabledChars > 0)
        {
            std::wstring strBlockedChars = L"";
            for (int i = 0; i < GC_CHAR_NUM; i++)
            {
                if (m_kRoomInfo.m_saAllowedChars[i] == false)
                {
                    strBlockedChars += L"/";

                    strBlockedChars += g_kGlobalValue.GetCharName(i);

                    strBlockedChars += L"/";
                }
            }

            g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetReplacedString(STR_ID_CHANGE_PVP_ROOM_NOT_ALLOWED_CHARS_OPTION, "L", strBlockedChars), KGCChatManager::CHAT_CMD_BROADCAST);
        }
        else
        {
            g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(STR_ID_CHANGE_PVP_ROOM_ALLOWED_CHARS_OPTION), KGCChatManager::CHAT_CMD_ADMIN);
        }
    }

    if (!IsRankOptionEqual(oldOptions.m_saAllowedRankings, m_kRoomInfo.m_saAllowedRankings))
    {
        if (kOptions.m_dwAllowedRankings != 0xFFFFFFFF)
        {
            std::wstring strAllowedRanks = L"";
            for (int i = 0; i < GC_BG_GRADE_MAX; i++)
            {
                if (m_kRoomInfo.m_saAllowedRankings[i] == true)
                {
                    strAllowedRanks += L"/";
                    strAllowedRanks += g_kGlobalValue.GetRankScoreName((i + 1));
                    strAllowedRanks += L"/";
                }
            }

            g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetReplacedString(STR_ID_CHANGE_PVP_ROOM_NOT_ALLOWED_RANKS_OPTION, "L", strAllowedRanks), KGCChatManager::CHAT_CMD_BROADCAST);
        }
        else
        {
            g_pkChatManager->AddChatMsgTo(g_MyD3D->m_pStateMachine->GetState(), g_pkStrLoader->GetString(STR_ID_CHANGE_PVP_ROOM_ALLOWED_RANKS_OPTION), KGCChatManager::CHAT_CMD_ADMIN);
        }
    }
}

void KGCRoomManager::ChangeGameModeCategory(bool bRightMove)
{
    std::vector<KGCGameModeCategory> vecCategory = GetGameModeCategoryVector();

    ASSERT(vecCategory.size() != 0);

    if (bRightMove)// 오른쪽 방향으로 바꾸기
    {
        if (++m_iSelectedModeCategoryIndex >= (int)vecCategory.size())
        {
            m_iSelectedModeCategoryIndex = 0;
        }
    }
    else// 왼쪽 방향으로 바꾸기
    {
        if (--m_iSelectedModeCategoryIndex < 0)
        {
            m_iSelectedModeCategoryIndex = (int)vecCategory.size() - 1;
        }
    }

    // 2007.03.14 wony
    // 카테고리를 변경했음에도 불구하고 랜덤맵으로 선택되어 있다
    // 카테고리를 변경할 때 랜덤맵을 false로 바꿔주자
    // 용섬은 true로 설정해야..
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
    {
        m_kRoomInfo.bRandomMapMode = true;
        RefreshRandomMap();
    }
    else
    {
        m_kRoomInfo.bRandomMapMode = false;
    }

    m_iSelectedModeIndex = 0;
    m_iSelectedStageIndex = 0;

    ResetGameMode();
    ResetSubGameMode();
}

void KGCRoomManager::ChangeGameMode(bool bRightMove)
{
    std::vector<KGCGameMode> vecMode = GetGameModeVector();

    ASSERT(vecMode.size() != 0);

    if (bRightMove)// 오른쪽 방향으로 바꾸기
    {
        if (++m_iSelectedModeIndex >= (int)vecMode.size())
        {
            m_iSelectedModeIndex = 0;
        }
    }
    else// 왼쪽 방향으로 바꾸기
    {
        if (--m_iSelectedModeIndex < 0)
        {
            m_iSelectedModeIndex = (int)vecMode.size() - 1;
        }
    }

    m_iSelectedStageIndex = 0;
    ResetGameMode();
    ResetSubGameMode();
}

void KGCRoomManager::ChangeDotaStage(bool bRightMove)
{
    //용사의 섬이면 불가하다.
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
        return;

    // 도타맵 정보는 글로벌에 있다.
    if (bRightMove)
    {
        if (int(g_kGlobalValue.m_vecDotaTemplate.size()) < (++g_kGlobalValue.m_iCurDotaMapID))
            g_kGlobalValue.m_iCurDotaMapID = 1;
    }
    else
    {
        if (1 > (--g_kGlobalValue.m_iCurDotaMapID))
            g_kGlobalValue.m_iCurDotaMapID = int(g_kGlobalValue.m_vecDotaTemplate.size());
    }
}

void KGCRoomManager::ChangeStage(bool bRightMove)
{
    //용사의 섬이면 불가하다.
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
        return;

    std::vector<KGCGameStage> vecStage = GetGameStageVector();

    ASSERT(vecStage.size() != 0);

    if (bRightMove)// 오른쪽 방향으로 바꾸기
    {
        if (m_bRandomMap)// 랜덤맵이 가능한 모드일 경우
        {
            if (m_kRoomInfo.bRandomMapMode)// 랜덤맵이었다면 첫번째 맵으로
            {
                m_kRoomInfo.bRandomMapMode = false;
                m_iSelectedStageIndex = 0;
            }
            else
            {
                if (++m_iSelectedStageIndex >= (int)vecStage.size())
                {
                    m_kRoomInfo.bRandomMapMode = true;
                    m_iSelectedStageIndex = ::rand() % (int)vecStage.size();
                }
            }
        }
        else// 랜덤맵이 불가능한 모드일 경우
        {
            if (++m_iSelectedStageIndex >= (int)vecStage.size())
            {
                m_iSelectedStageIndex = 0;
            }
        }
    }
    else// 왼쪽 방향으로 바꾸기
    {
        if (m_bRandomMap)// 랜덤맵이 가능한 모드일 경우
        {
            if (m_kRoomInfo.bRandomMapMode)// 랜덤맵이었다면 마지막 맵으로
            {
                m_kRoomInfo.bRandomMapMode = false;
                m_iSelectedStageIndex = (int)vecStage.size() - 1;
            }
            else
            {
                if (--m_iSelectedStageIndex < 0)
                {
                    m_kRoomInfo.bRandomMapMode = true;
                    m_iSelectedStageIndex = ::rand() % (int)vecStage.size();
                }
            }
        }
        else// 랜덤맵이 불가능한 모드일 경우
        {
            if (--m_iSelectedStageIndex < 0)
            {
                m_iSelectedStageIndex = (int)vecStage.size() - 1;
            }
        }
    }

    ResetGameMode();
}

void KGCRoomManager::RefreshRandomMap(void)
{
    if (m_kRoomInfo.bRandomMapMode)
    {
        std::vector<KGCGameStage> vecStage = GetGameStageVector();

        ASSERT(vecStage.size() != 0);
        m_iSelectedStageIndex = ::rand() % (int)vecStage.size();
        g_MyD3D->MyStg->SetStage(vecStage[m_iSelectedStageIndex].m_eGameStage);
    }
}

EGCGameMode KGCRoomManager::GetStagePlayable(std::map<int, KDungeonUserInfo>& mapStagePlayable)
{
    EGCGameMode eGameMode;
    SUserInfo* pHostUserInfo = NULL;

    mapStagePlayable.clear();

    // 방안에서는 방장껄로..
    if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
    {
        pHostUserInfo = &g_MyD3D->MyPlayer[g_MyD3D->Get_HostPlayer()]->m_kUserInfo;
        if (g_pkUIScene->m_pkWorldMapContainer->IsRenderOn())
            eGameMode = SiKGCWorldMapManager()->GetGameMode();
        else
            eGameMode = SiKGCRoomManager()->GetGameMode();
    }
    else // 그게 아니면 내껄로..
    {
        pHostUserInfo = &g_kGlobalValue.m_kUserInfo;
        eGameMode = SiKGCWorldMapManager()->GetGameMode();
    }

    mapStagePlayable = pHostUserInfo->mapStagePlayable;

    return eGameMode;
}

void KGCRoomManager::ChangeDungeonGameMode(int nGameLevel)
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
        return;

    if (GetGameModeCategory() != GC_GMC_DUNGEON)
        return;

    int iEnableLevel = 0;
    int iLimitLevel = 0;

    std::map<int, KDungeonUserInfo> mapStagePlayable;

    EGCGameMode eGameMode = GetStagePlayable(mapStagePlayable);

    iLimitLevel = SiKGCWorldMapManager()->GetLimiteOpenLevel(eGameMode);
    iEnableLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel(eGameMode, mapStagePlayable);

    if (g_kGlobalValue.IsUnlock() || g_kGlobalValue.IsDungeonFreePass())
        iEnableLevel = iLimitLevel - 1;

    // 바운드 처리~
    if (iEnableLevel >= iLimitLevel)
    {
        iEnableLevel = iLimitLevel - 1;
    }

    nGameLevel = (GC_SGM_DUNGEON_NORMAL + nGameLevel);
    iEnableLevel = (GC_SGM_DUNGEON_NORMAL + iEnableLevel);

    m_iSelectedSubMode = nGameLevel;

    if (nGameLevel < GC_SGM_DUNGEON_NORMAL)
    {
        m_iSelectedSubMode = GC_SGM_DUNGEON_NORMAL;
    }
    else if (nGameLevel > iEnableLevel)
    {
        m_iSelectedSubMode = iEnableLevel;
    }


    // 서브 게임 모드를 설정한다.
    SetSubGameMode(m_iSelectedSubMode);

}

void KGCRoomManager::ChangeSubGameModeTower(bool bRightMove)
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
        return;

    if (GetGameModeCategory() == GC_GMC_DUNGEON ||
        g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
    {
        int iEnableLevel = 0;
        int iLimitLevel = 0;

        std::map<int, KDungeonUserInfo> mapStagePlayable;

        EGCGameMode eGameMode = GetStagePlayable(mapStagePlayable);

        iLimitLevel = SiKGCWorldMapManager()->GetLimiteOpenLevel(eGameMode);
        iEnableLevel = SiKGCWorldMapManager()->GetPlayEnableTowerLevel(eGameMode, mapStagePlayable);

        if (iEnableLevel >= iLimitLevel)
        {
            iEnableLevel = iLimitLevel - 1;
        }

        iEnableLevel = (GC_SGM_DUNGEON_NORMAL + iEnableLevel);

        if (bRightMove)
        {
            if (++m_iSelectedSubMode > iEnableLevel)
            {
                m_iSelectedSubMode = GC_SGM_DUNGEON_NORMAL;
            }
        }
        else
        {
            if (--m_iSelectedSubMode < GC_SGM_DUNGEON_NORMAL)
            {
                m_iSelectedSubMode = iEnableLevel;
            }
        }
    }
    else
        return;

    SetSubGameMode(m_iSelectedSubMode);
}

void KGCRoomManager::ChangeSubGameMode(bool bRightMove)
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
        return;

    // 던전일때..
    if (GetGameModeCategory() == GC_GMC_DUNGEON ||
        g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
    {

        int iEnableLevel = 0;
        int iLimitLevel = 0;

        std::map<int, KDungeonUserInfo> mapStagePlayable;

        EGCGameMode eGameMode = GetStagePlayable(mapStagePlayable);

        iLimitLevel = SiKGCWorldMapManager()->GetLimiteOpenLevel(eGameMode);
        iEnableLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel(eGameMode, mapStagePlayable);


        if (g_kGlobalValue.IsUnlock() || g_kGlobalValue.IsDungeonFreePass())
            iEnableLevel = iLimitLevel - 1;

        // 바운드 처리~
        if (iEnableLevel >= iLimitLevel)
        {
            iEnableLevel = iLimitLevel - 1;
        }

        iEnableLevel = (GC_SGM_DUNGEON_NORMAL + iEnableLevel);

        if (bRightMove)
        {
            if (++m_iSelectedSubMode > iEnableLevel)
            {
                m_iSelectedSubMode = GC_SGM_DUNGEON_NORMAL;
            }
        }
        else
        {
            if (--m_iSelectedSubMode < GC_SGM_DUNGEON_NORMAL)
            {
                m_iSelectedSubMode = iEnableLevel;
            }
        }
    }
    else // 던전이 아닐때..
    {
        if (GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER ||
            GetGameModeCategory() == GC_GMC_MONSTER_HUNT ||
            GetGameModeCategory() == GC_GMC_DOTA)
            return;

        if (bRightMove)
        {
            if (++m_iSelectedSubMode > GC_SGM_NO_ITEM)
            {
                m_iSelectedSubMode = 0;
            }
        }
        else
        {
            if (--m_iSelectedSubMode < 0)
            {
                m_iSelectedSubMode = GC_SGM_NO_ITEM;
            }
        }
    }

    // 서브 게임 모드를 설정한다.
    SetSubGameMode(m_iSelectedSubMode);
}

std::string KGCRoomManager::GetGameModeCategoryStr()
{
#define caseStateToString(state) case state: str = #state; break;
    std::string str;
    switch (m_kRoomInfo.eGameModeCategory)
    {
        caseStateToString(GC_GMC_INVALID);

        caseStateToString(GC_GMC_MATCH)
            caseStateToString(GC_GMC_GUILD_BATTLE)
            caseStateToString(GC_GMC_DUNGEON)
            caseStateToString(GC_GMC_INDIGO)
            caseStateToString(GC_GMC_TUTORIAL)
            caseStateToString(GC_GMC_TAG_MATCH)
            caseStateToString(GC_GMC_MONSTER_CRUSADER)
            caseStateToString(GC_GMC_MONSTER_HUNT)
            caseStateToString(GC_GMC_DEATHMATCH)
            caseStateToString(GC_GMC_ANGELS_EGG)
            caseStateToString(GC_GMC_CAPTAIN)
            caseStateToString(GC_GMC_DOTA);
        caseStateToString(GC_GMC_COUNT);
        caseStateToString(GC_GMC_FATAL_DEATHMATCH)

    default:
        str = "GameModeCategory Error";
        break;
    }
    return str;
}

std::string KGCRoomManager::GetGameModeStr()
{
#define caseStateToString(state) case state: str = #state; break;
    std::string str;
    switch (m_kRoomInfo.eGameMode)
    {
        caseStateToString(GC_GM_INVALID);

        caseStateToString(GC_GM_TUTORIAL);
        caseStateToString(GC_GM_TEAM_MODE);
        caseStateToString(GC_GM_SURVIVAL_MODE);
        caseStateToString(GC_GM_TAG_TEAM);
        caseStateToString(GC_GM_TAG_SURVIVAL);
        caseStateToString(GC_GM_GUILD_BATTLE);
        caseStateToString(GC_GM_INDIGO_TEAM);
        caseStateToString(GC_GM_QUEST0);
        caseStateToString(GC_GM_QUEST1);
        caseStateToString(GC_GM_QUEST2);
        caseStateToString(GC_GM_QUEST3);
        caseStateToString(GC_GM_QUEST4);
        caseStateToString(GC_GM_QUEST5);
        caseStateToString(GC_GM_QUEST6);
        caseStateToString(GC_GM_QUEST7);
        caseStateToString(GC_GM_QUEST8);
        caseStateToString(GC_GM_QUEST9);
        caseStateToString(GC_GM_QUEST10);
        caseStateToString(GC_GM_QUEST11);
        caseStateToString(GC_GM_QUEST12);
        caseStateToString(GC_GM_QUEST13);
        caseStateToString(GC_GM_QUEST14);
        caseStateToString(GC_GM_QUEST15);
        caseStateToString(GC_GM_QUEST16);
        caseStateToString(GC_GM_QUEST17);
        caseStateToString(GC_GM_QUEST18);
        caseStateToString(GC_GM_QUEST19);
        caseStateToString(GC_GM_QUEST20);
        caseStateToString(GC_GM_MONSTER_CRUSADER);
        caseStateToString(GC_GM_MONSTER_HUNT);
        caseStateToString(GC_GM_QUEST21);
        caseStateToString(GC_GM_DEATH_TEAM);
        caseStateToString(GC_GM_DEATH_SURVIVAL);
        caseStateToString(GC_GM_MINIGAME_TREEDROP);
        caseStateToString(GC_GM_MINIGAME_BALLOON);
        caseStateToString(GC_GM_MINIGAME_BUTTERFRY);
        caseStateToString(GC_GM_QUEST22);
        caseStateToString(GC_GM_ANGELS_EGG);
        caseStateToString(GC_GM_CAPTAIN);
        caseStateToString(GC_GM_QUEST23);
        caseStateToString(GC_GM_QUEST24);
        caseStateToString(GC_GM_QUEST25);
        caseStateToString(GC_GM_QUEST26);
        caseStateToString(GC_GM_QUEST27);
        caseStateToString(GC_GM_QUEST28);
        caseStateToString(GC_GM_QUEST29);
        caseStateToString(GC_GM_QUEST30);
        caseStateToString(GC_GM_QUEST31);
        caseStateToString(GC_GM_QUEST32);
        caseStateToString(GC_GM_QUEST33);
        caseStateToString(GC_GM_QUEST34);
        caseStateToString(GC_GM_QUEST35);
        caseStateToString(GC_GM_QUEST36);
        caseStateToString(GC_GM_QUEST37);
        caseStateToString(GC_GM_QUEST38);
        caseStateToString(GC_GM_QUEST39);
        caseStateToString(GC_GM_QUEST40);
        caseStateToString(GC_GM_QUEST41);
        caseStateToString(GC_GM_QUEST42);
        caseStateToString(GC_GM_QUEST43);
        caseStateToString(GC_GM_QUEST44);
        caseStateToString(GC_GM_QUEST45);
        caseStateToString(GC_GM_QUEST46);
        caseStateToString(GC_GM_QUEST47);
        caseStateToString(GC_GM_QUEST48);
        caseStateToString(GC_GM_DOTA);
        caseStateToString(GC_GM_AGIT);
        caseStateToString(GC_GM_QUEST49);
        caseStateToString(GC_GM_QUEST50);
        caseStateToString(GC_GM_QUEST51);
        caseStateToString(GC_GM_QUEST52);
        caseStateToString(GC_GM_QUEST53);
        caseStateToString(GC_GM_QUEST54);
        caseStateToString(GC_GM_QUEST55);
        caseStateToString(GC_GM_QUEST56);
        caseStateToString(GC_GM_QUEST57);
        caseStateToString(GC_GM_QUEST58);
        caseStateToString(GC_GM_AUTOMATCH_TEAM);
        caseStateToString(GC_GM_QUEST59);
        caseStateToString(GC_GM_QUEST60);
        caseStateToString(GC_GM_QUEST61);
        caseStateToString(GC_GM_QUEST62);
        caseStateToString(GC_GM_QUEST63);
        caseStateToString(GC_GM_QUEST64);
        caseStateToString(GC_GM_QUEST65);
        caseStateToString(GC_GM_QUEST66);
        caseStateToString(GC_GM_QUEST67);
        caseStateToString(GC_GM_QUEST68);
        caseStateToString(GC_GM_QUEST69);
        caseStateToString(GC_GM_QUEST70);
        caseStateToString(GC_GM_QUEST71);
        caseStateToString(GC_GM_QUEST72);
        caseStateToString(GC_GM_QUEST73);
        caseStateToString(GC_GM_QUEST74);
        caseStateToString(GC_GM_QUEST75);
        caseStateToString(GC_GM_QUEST76);
        caseStateToString(GC_GM_FATAL_DEATH_TEAM);
        caseStateToString(GC_GM_FATAL_DEATH_SURVIVAL);
        caseStateToString(GC_GM_QUEST77);
        caseStateToString(GC_GM_QUEST78);
        caseStateToString(GC_GM_QUEST79);
        caseStateToString(GC_GM_QUEST80);
        caseStateToString(GC_GM_QUEST81);
        caseStateToString(GC_GM_QUEST82);
        caseStateToString(GC_GM_MONSTER_TEAM);
        caseStateToString(GC_GM_MONSTER_SURVIVAL);
        caseStateToString(GC_GM_QUEST83);
        caseStateToString(GC_GM_QUEST84);
        caseStateToString(GC_GM_ARCHER_CONTEST);
        caseStateToString(GC_GM_QUEST85);
        caseStateToString(GC_GM_QUEST86);
        caseStateToString(GC_GM_QUEST87);
        caseStateToString(GC_GM_QUEST88);
        caseStateToString(GC_GM_QUEST89);
        caseStateToString(GC_GM_QUEST90);
        caseStateToString(GC_GM_QUEST91);
        caseStateToString(GC_GM_QUEST92);
        caseStateToString(GC_GM_QUEST93);
        caseStateToString(GC_GM_QUEST94);
        caseStateToString(GC_GM_QUEST95);
        caseStateToString(GC_GM_QUEST96);
        caseStateToString(GC_GM_QUEST97);
        caseStateToString(GC_GM_QUEST98);
        caseStateToString(GC_GM_QUEST99);
        caseStateToString(GC_GM_QUEST100);
        caseStateToString(GC_GM_QUEST101);
        caseStateToString(GC_GM_QUEST102);
        caseStateToString(GC_GM_QUEST103);
        caseStateToString(NUM_GC_GAME_MODE);
    default:
        str = "GameMode Error";
        break;
    }
    return str;
}

std::string KGCRoomManager::GetSubGameModeStr()
{
    std::string str;

    if (m_kRoomInfo.nSubGameMode == GC_SGM_INVALID)
    {
        str = "GC_SGM_INVALID";
    }
    else if (m_kRoomInfo.nSubGameMode == GC_SGM_NORMAL)
    {
        str = "GC_SGM_NORMAL";
    }
    else if (m_kRoomInfo.nSubGameMode == GC_SGM_INVALID)
    {
        str = "GC_SGM_NO_ITEM";
    }
    else
    {
        str = "GC_SGM_QUEST_LEVEL_";
        str += boost::str(boost::format("%1") % (m_kRoomInfo.nSubGameMode - GC_SGM_DUNGEON_NORMAL));
    }
    return str;
}

void KGCRoomManager::PressureUser(DWORD dwUID)
{
    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwUID)
        {
            //넌 눌러 졌다. 
            //g_MyD3D->MyPlayer[i]->m_kPressureState.m_nState = KPressureState::PS_PRESSED;
            std::wstringstream strTemp;
#if defined(NATION_IDN) || defined(NATION_TAIWAN) || defined(NATION_THAILAND) || defined(NATION_BRAZIL) || defined(NATION_CHINA)
            strTemp << g_pkStrLoader->GetReplacedString(STR_ID_HURRYUP, "l", L"#c" + g_MyD3D->MyPlayer[i]->GetStrUserColor() + g_MyD3D->MyPlayer[i]->GetStrUserName().c_str() + L"#cx");
#else
            strTemp << g_MyD3D->MyPlayer[i]->GetStrUserName()
#if	defined(NATION_USA) || defined( NATION_EU )
                << L" "
#endif
                << g_pkStrLoader->GetString(STR_ID_HURRYUP);
#endif
            g_pkChatManager->AddChatMsg(strTemp.str(), KGCChatManager::CHAT_TYPE_ADMIN);

        }
    }

    //눌려진게 자신이면 창을 띄워라.
    if (g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.dwUID == dwUID)
    {
        g_pkUIScene->m_pkHurryUPDlg->ViewStart();

        //누르고 나면 게이지를 다시 채울수 있음.
        g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kPressureState.m_nState = KPressureState::PS_GAUGE;
        g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kPressureState.m_dwElapsedTime = ::timeGetTime();
        KP2P::GetInstance()->Send_SetMyPressState(KPressureState::PS_GAUGE);
    }

    return;
}

void KGCRoomManager::SetIdleState(DWORD dwUID, bool bIdle)
{
    if (g_MyD3D->m_pStateMachine->GetState() != GS_ROOM &&
        g_MyD3D->m_pStateMachine->GetState() != GS_GAME)
        return;

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwUID)
        {
            if (bIdle == true && g_MyD3D->MyPlayer[i]->m_bIdle != bIdle)
            {
                std::wstringstream strTemp;
#if defined(NATION_IDN) || defined(NATION_THAILAND) || defined(NATION_BRAZIL)
                strTemp << g_pkStrLoader->GetReplacedString(STR_ID_CHANGETO_IDLE, "l", L"#c" + g_MyD3D->MyPlayer[i]->GetStrUserColor() + g_MyD3D->MyPlayer[i]->GetStrUserName().c_str() + L"#cx");
#else
                strTemp << g_MyD3D->MyPlayer[i]->GetStrUserName()
#if	defined(NATION_USA)
                    << L" "
#endif
                    << g_pkStrLoader->GetString(STR_ID_CHANGETO_IDLE);
#endif
                g_pkChatManager->AddChatMsg(strTemp.str(), KGCChatManager::CHAT_TYPE_ADMIN);
            }
            g_MyD3D->MyPlayer[i]->m_bIdle = bIdle;
        }
    }

    UpdateRoomUI();
    return;
}

void KGCRoomManager::SetPressureState(DWORD dwUID, KPressureState kPrState)
{
    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwUID)
        {
            //상태 복사
            g_MyD3D->MyPlayer[i]->m_kPressureState.m_nState = kPrState.m_nState;

            //지속 시간이 넘어온다. 시작 시간으로 변경. - 패킷 주고 받는 시간 만큼 오차생기는건 무시한다.
            g_MyD3D->MyPlayer[i]->m_kPressureState.m_dwElapsedTime = ::timeGetTime() - kPrState.m_dwElapsedTime;
        }
    }

    UpdateRoomUI();
    return;
}


int KGCRoomManager::GetEmptySlot()
{
    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false && m_kRoomInfo.bOpenSlot[i] == true)
            return i;
    }
    return -1;
}

int KGCRoomManager::GetClosedSlot()
{
    for (int i = MAX_PLAYER_NUM - 1; i > -1; i--)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false && m_kRoomInfo.bOpenSlot[i] == false)
            return i;
    }
    return -1;
}
int KGCRoomManager::GetOpenSlotCount()
{
    int iCount = 0;
    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false && m_kRoomInfo.bOpenSlot[i] == true)
            iCount++;
    }
    return iCount;
}
EGCGameModeType KGCRoomManager::GetGameModeType(void)const
{
    switch (m_kRoomInfo.eGameMode)
    {
    case GC_GM_SURVIVAL_MODE:
    case GC_GM_TAG_SURVIVAL:
    case GC_GM_DEATH_SURVIVAL:
    case GC_GM_FATAL_DEATH_SURVIVAL:
    {
        return GC_GMT_ALONE_FIGHT;
    }
    case GC_GM_TEAM_MODE:
    case GC_GM_GUILD_BATTLE:
    case GC_GM_INDIGO_TEAM:
    case GC_GM_TAG_TEAM:
    case GC_GM_DEATH_TEAM:
    case GC_GM_ANGELS_EGG:
    case GC_GM_CAPTAIN:
    case GC_GM_DOTA:
    case GC_GM_AUTOMATCH_TEAM:
    case GC_GM_FATAL_DEATH_TEAM:
    {
        return GC_GMT_TEAM_FIGHT;
    }
    case GC_GM_QUEST0:
    case GC_GM_QUEST1:
    case GC_GM_QUEST2:
    case GC_GM_QUEST3:
    case GC_GM_QUEST4:
    case GC_GM_QUEST5:
    case GC_GM_QUEST6:
    case GC_GM_QUEST7:
    case GC_GM_QUEST8:
    case GC_GM_QUEST9:
    case GC_GM_QUEST10:
    case GC_GM_QUEST11:
    case GC_GM_QUEST12:
    case GC_GM_QUEST13:
    case GC_GM_QUEST14:
    case GC_GM_QUEST15:
    case GC_GM_QUEST16:
    case GC_GM_QUEST17:
    case GC_GM_QUEST18:
    case GC_GM_QUEST19:
    case GC_GM_QUEST20:
    case GC_GM_MONSTER_CRUSADER:
    case GC_GM_MONSTER_HUNT:
    case GC_GM_QUEST21:
    {
        return GC_GMT_UNION_FIGHT;
    }
    }

    return GC_GMT_UNION_FIGHT;
}

EGCGameStage KGCRoomManager::GetGameModeStage()
{
    std::vector<KGCGameStage> vecGameStage = GetGameStageVector();

    int iIndex = m_iSelectedStageIndex;
    if (iIndex >= (int)vecGameStage.size())
        iIndex = (int)vecGameStage.size() - 1;

    if (iIndex < 0)
        return GC_GS_FOREST_OF_ELF;

    ASSERT(vecGameStage.size() != 0);
    return vecGameStage[iIndex].m_eGameStage;
}

void KGCRoomManager::UpdateRoomUI()
{
    if (g_MyD3D->m_pStateMachine->IsStateRoom())
        g_pkUIScene->RoomSceneUpdateData();
}

void KGCRoomManager::SendRoomInfo()
{
    if (g_kGlobalValue.m_kUserInfo.bHost && !(GetPrevRoomInfo() == GetRoomInfo()))
        KP2P::GetInstance()->Send_ChangeRoomInfoReq(&GetRoomInfo());
}

void KGCRoomManager::SetDropItemGettingRule(int val)
{
    m_iDropItemGettingRule = val;

    // 왜 그걸 여기서 보내???????
    // Update할때 보내면 2번 보내게 되는거 같은데...
    //방 정보 바뀐 패킷을 보애 줘야한다.
    //SendRoomInfo();
}

int KGCRoomManager::SetDropItemGettingRuleToNext()
{
    SRoomInfo sRoomInfo = GetRoomInfo();
    sRoomInfo.m_cRoutingMethod = (++m_iDropItemGettingRule) % DRR_NUM;
    KP2P::GetInstance()->Send_ChangeRoomInfoReq(&sRoomInfo);
    return sRoomInfo.m_cRoutingMethod;
}

// 이게.. 판타스틱 맵을 사용한다.. 라는 의미이긴 한데..
// 몬스터가 등장하면 판타스틱 맵을 사용하기 때문에..
// 몬스터 등장하는가 안하는가와 동일하다
bool KGCRoomManager::IsMonsterGameMode()
{
    return GetGameModeCategory() == GC_GMC_DUNGEON ||
        GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER ||
        GetGameModeCategory() == GC_GMC_MONSTER_HUNT ||
        GetGameModeCategory() == GC_GMC_MINIGAME ||
        GetGameModeCategory() == GC_GMC_DOTA ||
        g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE ||
        GetGameModeCategory() == GC_GMC_AGIT;
}

bool KGCRoomManager::IsFatalGameMode()
{
    return GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH;
}

bool KGCRoomManager::IsAvailableGameMode(EGCGameModeCategory eCategory, EGCGameMode eMode)
{
    for (int i = 0; i < (int)m_vecGameModeCategory.size(); ++i)
    {
        if (m_vecGameModeCategory[i].m_eGameModeCategory != eCategory)
            continue;

        std::vector<KGCGameMode>& vecGameMode = m_vecGameModeCategory[i].m_vecGameMode;
        for (int j = 0; j < (int)vecGameMode.size(); ++j)
        {
            if (vecGameMode[j].m_eGameMode == eMode)
                return true;
        }
    }
    return false;
}

bool KGCRoomManager::IsAvailableCategory(EChannelType eChannelType, EGCGameModeCategory eCategory)
{
    EChannelType eTargetChannelType = eChannelType;
    switch (eChannelType)
    {
        // 얘네들은 그냥 대전 채널과 동일하다
    case CT_BEGINNER:
    case CT_ROOKIE:
    case CT_EXPERT:
    case CT_MASTER:
    case CT_AUTOMATCH:
        eTargetChannelType = CT_UNLIMITED;
    case CT_GUILD_UNLIMITED:
    case CT_GUILD_BEGINNER:
    case CT_GUILD_ROOKIE:
    case CT_GUILD_EXPERT:
    case CT_GUILD_MASTER:
        break;
    }

    //std::map< EChannelType, std::vector<EGCGameModeCategory> > m_mapChannelCategory;
    std::map< EChannelType, std::vector<EGCGameModeCategory> >::iterator mit;

    mit = m_mapChannelCategory.find(eTargetChannelType);
    if (mit == m_mapChannelCategory.end())
        return false;

    for (int i = 0; i < (int)mit->second.size(); ++i)
    {
        if (mit->second[i] == eCategory)
            return true;
    }

    return false;
}

std::wstring KGCRoomManager::GetGameModeName(EGCGameMode eGameMode)
{
    switch (eGameMode)
    {
    case GC_GM_TAG_SURVIVAL:
    case GC_GM_SURVIVAL_MODE:
    case GC_GM_TEAM_MODE:
    case GC_GM_CAPTAIN:
        return g_pkStrLoader->GetString(STR_ID_GMC_MATCH);
    case GC_GM_TAG_TEAM:
        return g_pkStrLoader->GetString(STR_ID_GMC_TAG_MATCH);
    case GC_GM_GUILD_BATTLE:
        return g_pkStrLoader->GetString(STR_ID_GMC_GUILD_BATTLE);
    case GC_GM_MONSTER_CRUSADER:
        return g_pkStrLoader->GetString(STR_ID_GMC_MONSTER_CRUSADERS);
    case GC_GM_MONSTER_HUNT:
        return g_pkStrLoader->GetString(STR_ID_GMC_MONSTER_HUNT);
    case GC_GM_DEATH_TEAM:
    case GC_GM_DEATH_SURVIVAL:
        return g_pkStrLoader->GetString(STR_ID_GMC_DEATHMATCH);
    case GC_GM_ANGELS_EGG:
        //return g_pkStrLoader->GetString( STR_ID_GMC_ANGELS_EGG );
        return g_pkStrLoader->GetString(STR_ID_GMC_HALLOWEEN_PUMPKIN);
    case GC_GM_DOTA:
        return g_pkStrLoader->GetString(STR_ID_GMC_DOTA);
    case GC_GM_FATAL_DEATH_TEAM:
    case GC_GM_FATAL_DEATH_SURVIVAL:
        return g_pkStrLoader->GetString(STR_ID_GMC_FATAL_DEATHMATCH);
    default:
        return L"Invalid Mode";
    }
}

bool KGCRoomManager::All_Players_Ready(void)
{
    // 모든 플레이어가 레디했는가?
    for (unsigned char i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (!g_MyD3D->MyPlayer[i]->IsLocalPlayer() && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
        {
            if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_READY)
                return false;
        }
    }

    return true;
}

void KGCRoomManager::Kick_Player(char Num)
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_BROADCASTING))
    {
        int iAdminLevel = g_MyD3D->MyPlayer[Num]->m_kUserInfo.eGCUserLevel;
        if (iAdminLevel == USERLEVEL_ADMIN || iAdminLevel == USERLEVEL_HIGH)
            return;
    }

    KP2P::GetInstance()->Send_BanUserReq(g_MyD3D->MyPlayer[Num]->GetPlayerLoginID(), 3);
}


bool KGCRoomManager::CanPlayHellmode()
{
    return (g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_HELLMODE_TICKET) || 1 == g_kGlobalValue.m_iUseHellModeBtn || g_MyD3D->m_kItemMgr.FindInventoryForItemID(KGCItemManager::ITEM_FREEPATH_TICKET) ||
        (g_kGlobalValue.m_bIsRainbowEvent && (g_kGlobalValue.m_bJackPot || g_kGlobalValue.m_iRainbowEventID == STR_ID_RAINBOW4)));
}

std::string KGCRoomManager::GetStageFootHoldTextureName(IN int iFieldIndex_, IN bool bSpecification_)
{
    unsigned char ucStage = GetRoomInfo().ucStage;
    std::stringstream stmTextureName;
    stmTextureName << (int)ucStage << "_" << iFieldIndex_ + 1;

    bool bExist = m_mapStageSpecification.find(ucStage) != m_mapStageSpecification.end();
    if (bSpecification_ && bExist)
    {
        std::vector<KGCStageSpecification>::iterator vit;
        for (vit = m_mapStageSpecification[ucStage].begin(); vit != m_mapStageSpecification[ucStage].end(); ++vit)
        {
            bool bGameModeCategory = (*vit).m_setGameMode.empty() ||
                (*vit).m_setGameMode.find(GetGameMode()) != (*vit).m_setGameMode.end();

            if (!bGameModeCategory)
                continue;

            if ((*vit).m_strFootHoldPostString != "")
                stmTextureName << "_" << (*vit).m_strFootHoldPostString;

            break;
        }
    }
    stmTextureName << ".dds";

    return stmTextureName.str();
}

std::string KGCRoomManager::GetStageSkyTextureName(IN int iSkyID_, IN bool bSpecification_)
{
    unsigned char ucStage = GetRoomInfo().ucStage;
    std::stringstream stmTextureName;
    stmTextureName << "sky" << iSkyID_;

    bool bExist = m_mapStageSpecification.find(ucStage) != m_mapStageSpecification.end();
    if (bSpecification_ &&
        bExist)
    {
        std::vector<KGCStageSpecification>::iterator vit;
        for (vit = m_mapStageSpecification[ucStage].begin(); vit != m_mapStageSpecification[ucStage].end(); ++vit)
        {
            bool bGameModeCategory = (*vit).m_setGameMode.empty() ||
                (*vit).m_setGameMode.find(GetGameMode()) != (*vit).m_setGameMode.end();

            if (!bGameModeCategory)
                continue;

            if ((*vit).m_strSkyPostString != "")
                stmTextureName << "_" << (*vit).m_strSkyPostString;

            break;
        }
    }
    stmTextureName << ".dds";

    return stmTextureName.str();
}


void KGCRoomManager::LoadGameModeFromPacket(const KENU_CLIENT_CONTENTS_OPEN_NOT& kRecv)
{
    //DECL_PACKET( ENU_CLIENT_CONTENTS_OPEN_NOT )
    //{
    //    std::vector<std::pair<int, std::vector<int> > >                     m_vecChannelCategories; // Channel,<Category>
    //    std::vector<std::pair<int, std::vector<int> > >                     m_vecCategoryModes; // Category,<Modes>
    //    std::vector<std::pair<std::pair<int,bool>, std::vector<int> > >     m_vecModeInfo; // <Mode,Random>,<Maps>
    //    std::map<int,int>                                                   m_mapCharactes;
    //};

    m_vecGameModeCategory.clear();
    m_mapChannelCategory.clear();
    m_vecCheckDungeonGameMode.clear();

    for (int i = 0; i < (int)kRecv.m_vecChannelCategories.size(); ++i)
    {
        const std::vector<int>& vecCategories = kRecv.m_vecChannelCategories[i].second;

        for (int j = 0; j < (int)vecCategories.size(); ++j)
        {
            m_mapChannelCategory[(EChannelType)kRecv.m_vecChannelCategories[i].first].push_back((EGCGameModeCategory)vecCategories[j]);
        }
    }

    SetValidCategory();

    //카테고리에 해당하는 Stage들을 읽는다. - 여기서 Quest랑 몬스터 원정대(hunt)는 모드별로 Stage가 다르기 때문에 여기서 읽어두지 않는다.
    //근데 그 전에 Stage에 몬스터들을 셋팅해 주자.
    KSafeArray<KGCGameStage, NUM_TOTAL_STAGE> akGameStage;
    for (int i = 0; i < NUM_TOTAL_STAGE; ++i)
    {
        akGameStage[i].m_eGameStage = (EGCGameStage)i;
        akGameStage[i].m_nSubGameMode = GC_SGM_NORMAL;
    }

    std::vector<std::pair<int, std::vector<int> > >::const_iterator vitCM;
    for (vitCM = kRecv.m_vecCategoryModes.begin(); vitCM != kRecv.m_vecCategoryModes.end(); ++vitCM)
    {
        KGCGameModeCategory kGameModeCategory;
        kGameModeCategory.m_vecGameMode.clear();
        kGameModeCategory.m_bRandomMap = false;
        kGameModeCategory.m_eGameModeCategory = (EGCGameModeCategory)(*vitCM).first;

        const std::vector<int>& vecModes = (*vitCM).second;
        std::vector<int>::const_iterator vit;
        for (vit = vecModes.begin(); vit != vecModes.end(); ++vit)
        {
            KGCGameMode kGameMode;
            kGameMode.m_eGameMode = (EGCGameMode)(*vit);
            kGameMode.m_vecGameStage.clear();

            std::vector<std::pair<std::pair<int, bool>, std::vector<int> > >::const_iterator vitCI;
            for (vitCI = kRecv.m_vecCategoryInfo.begin(); vitCI != kRecv.m_vecCategoryInfo.end(); ++vitCI)
            {
                if ((*vitCI).first.first == (*vitCM).first)
                {
                    kGameModeCategory.m_bRandomMap = (*vitCI).first.second;
                    for (int i = 0; i < (int)(*vitCI).second.size(); ++i)
                    {
                        kGameMode.m_vecGameStage.push_back(akGameStage[(*vitCI).second[i]]);
                    }
                    break;
                }
            }

            kGameModeCategory.m_vecGameMode.push_back(kGameMode);

            int iCatecory = vitCM->first;
            if ((EGCGameMode)iCatecory == GC_GMC_DUNGEON)
                m_vecCheckDungeonGameMode.push_back((EGCGameMode)(*vit));
        }

        m_vecGameModeCategory.push_back(kGameModeCategory);
    }
}

void KGCRoomManager::LoadGameModeFromPacket(KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kRecv)
{
    std::vector<int>::iterator vitGC = kRecv.m_prGameCategoryInfo.first.begin();

    for (; vitGC != kRecv.m_prGameCategoryInfo.first.end(); ++vitGC)
    {
        switch (*vitGC)
        {
        case KCCGameCategoryInfo::EV_CHANNELCATEGORIES:
        {    m_mapChannelCategory.clear();
        for (int i = 0; i < (int)kRecv.m_prGameCategoryInfo.second.m_vecChannelCategories.size(); ++i)
        {
            const std::vector<int>& vecCategories = kRecv.m_prGameCategoryInfo.second.m_vecChannelCategories[i].second;

            for (int j = 0; j < (int)vecCategories.size(); ++j)
            {
                m_mapChannelCategory[(EChannelType)kRecv.m_prGameCategoryInfo.second.m_vecChannelCategories[i].first].push_back((EGCGameModeCategory)vecCategories[j]);
            }
        }
        SetValidCategory();
        break;
        }
        case KCCGameCategoryInfo::EV_CATEGORYMODES:
        {
            g_kGlobalValue.SetOpenedEventDungeonInfo(kRecv.m_prGameCategoryInfo.second.m_vecCategoryModes);
            //카테고리에 해당하는 Stage들을 읽는다. - 여기서 Quest랑 몬스터 원정대(hunt)는 모드별로 Stage가 다르기 때문에 여기서 읽어두지 않는다.
            //근데 그 전에 Stage에 몬스터들을 셋팅해 주자.
            KSafeArray<KGCGameStage, NUM_TOTAL_STAGE> akGameStage;
            for (int i = 0; i < NUM_TOTAL_STAGE; ++i)
            {
                akGameStage[i].m_eGameStage = (EGCGameStage)i;
                akGameStage[i].m_nSubGameMode = GC_SGM_NORMAL;
            }
            m_vecGameModeCategory.clear();
            m_vecCheckDungeonGameMode.clear();

            std::vector<std::pair<int, std::vector<int> > >::const_iterator vitCM;
            for (vitCM = kRecv.m_prGameCategoryInfo.second.m_vecCategoryModes.begin(); vitCM != kRecv.m_prGameCategoryInfo.second.m_vecCategoryModes.end(); ++vitCM)
            {
                KGCGameModeCategory kGameModeCategory;
                kGameModeCategory.m_vecGameMode.clear();
                kGameModeCategory.m_bRandomMap = false;
                kGameModeCategory.m_eGameModeCategory = (EGCGameModeCategory)(*vitCM).first;

                const std::vector<int>& vecModes = (*vitCM).second;
                std::vector<int>::const_iterator vit;
                for (vit = vecModes.begin(); vit != vecModes.end(); ++vit)
                {
                    KGCGameMode kGameMode;
                    kGameMode.m_eGameMode = (EGCGameMode)(*vit);
                    kGameMode.m_vecGameStage.clear();

                    if (kRecv.m_prGameCategoryInfo.first.end() !=
                        std::find(kRecv.m_prGameCategoryInfo.first.begin(), kRecv.m_prGameCategoryInfo.first.end(), KCCGameCategoryInfo::EV_CATEGORYINFO))
                    {
                        std::vector<std::pair<std::pair<int, bool>, std::vector<int> > >::const_iterator vitCI;
                        for (vitCI = kRecv.m_prGameCategoryInfo.second.m_vecCategoryInfo.begin(); vitCI != kRecv.m_prGameCategoryInfo.second.m_vecCategoryInfo.end(); ++vitCI)
                        {
                            if ((*vitCI).first.first == (*vitCM).first)
                            {
                                kGameModeCategory.m_bRandomMap = (*vitCI).first.second;
                                for (int i = 0; i < (int)(*vitCI).second.size(); ++i)
                                {
                                    kGameMode.m_vecGameStage.push_back(akGameStage[(*vitCI).second[i]]);
                                }
                                break;
                            }
                        }
                    }
                    kGameModeCategory.m_vecGameMode.push_back(kGameMode);

                    int iCatecory = vitCM->first;
                    if ((EGCGameMode)iCatecory == GC_GMC_DUNGEON)
                        m_vecCheckDungeonGameMode.push_back((EGCGameMode)(*vit));
                }
                m_vecGameModeCategory.push_back(kGameModeCategory);
            }
            break;
        }
        case KCCGameCategoryInfo::EV_CATEGORYINFO:
        {
            m_vecStoreCategoryInfo.clear();
            KSafeArray<KGCGameStage, NUM_TOTAL_STAGE> GameStage;
            for (int i = 0; i < NUM_TOTAL_STAGE; ++i)
            {
                GameStage[i].m_eGameStage = (EGCGameStage)i;
                GameStage[i].m_nSubGameMode = GC_SGM_NORMAL;
            }

            std::vector<std::pair<std::pair<int, bool>, std::vector<int> > >::const_iterator vitCI;
            for (vitCI = kRecv.m_prGameCategoryInfo.second.m_vecCategoryInfo.begin(); vitCI != kRecv.m_prGameCategoryInfo.second.m_vecCategoryInfo.end(); ++vitCI)
            {
                KGCCategoryInfo kCategoryInfo;

                kCategoryInfo.m_eGameModeCategory = (EGCGameModeCategory)((*vitCI).first.first);
                for (int i = 0; i < (int)(*vitCI).second.size(); ++i)
                    kCategoryInfo.m_vecGameStage.push_back(GameStage[(*vitCI).second[i]]);
                kCategoryInfo.bRandomMap = (*vitCI).first.second;


                m_vecStoreCategoryInfo.push_back(kCategoryInfo);
            }
            break;
        }
        }
    }

    /* m_vecGameModeCategory에 EV_CATEGORYINFO에서 가져온 정보를 따로 담아주는 루틴 */
    std::vector<KGCGameModeCategory>::iterator itr_GameMode = m_vecGameModeCategory.begin();
    int iLoop = 0;
    //KCCGameCategoryInfo::EV_CATEGORYMODES에서 담아온 정보의 루프
    for (; itr_GameMode != m_vecGameModeCategory.end(); ++itr_GameMode, ++iLoop)//담아야 할 장소의 루프를 돈다.
    {
        KGCGameModeCategory kGameModeCategory;
        std::vector<KGCCategoryInfo>::iterator itr_Category = m_vecStoreCategoryInfo.begin();
        //KCCGameCategoryInfo::EV_CATEGORYINFO에서 담아온 정보의 루프
        for (; itr_Category != m_vecStoreCategoryInfo.end(); ++itr_Category)
        {
            //카테고리가 동일할때만 새로이 담아온 정보로 대체한다.
            if ((*itr_GameMode).m_eGameModeCategory == (*itr_Category).m_eGameModeCategory)
            {
                std::vector<KGCGameMode>::iterator itr_DetailGameMode = (*itr_GameMode).m_vecGameMode.begin();
                //팀전, 개인전등의 정보를 담은 루프. 스테이지는 팀전/개인전 모두 같으므로, 구분 없이 집어 넣는다.
                for (; itr_DetailGameMode != (*itr_GameMode).m_vecGameMode.end(); ++itr_DetailGameMode)
                {
                    KGCGameMode kGameMode;
                    kGameMode.m_eGameMode = (EGCGameMode)(*itr_DetailGameMode).m_eGameMode;
                    kGameMode.m_vecGameStage = (*itr_Category).m_vecGameStage;

                    kGameModeCategory.m_vecGameMode.push_back(kGameMode);
                }
                kGameModeCategory.m_bRandomMap = (*itr_Category).bRandomMap;
                kGameModeCategory.m_eGameModeCategory = (*itr_Category).m_eGameModeCategory;

                //카테고리 정보를 제대로 가져오지 못했다면 m_vecGameModeCategory에 바뀐 정보를 담아주지 않는다.
                if (GC_GMC_INVALID != kGameModeCategory.m_eGameModeCategory)
                    m_vecGameModeCategory.at(iLoop) = kGameModeCategory;
            }

        }

    }

}

HRESULT KGCRoomManager::CheckCreateRoomError()
{
    std::wstringstream stm;
    std::wstring strColorKey = L"#cff0000";
    std::wstring strColorKeyEnd = L"#cX";
    std::wstringstream strm;
    stm << strColorKey << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR0) << strColorKeyEnd;

    if (Result_Create != 0)
    {
        switch (Result_Create)
        {
        case -1:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1),
                g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 2:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CREATE_ROOM_FAIL1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 3:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVALID_GRADE_CHANNEL), g_pkStrLoader->GetString(STR_ID_INVALID_GRADE_CHANNEL2), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 8:
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_TOOLTIP_YOU_NEED_HELL_TICKET1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            break;
        case 9:
        case 10:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR5), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 11:
        {
            //입장제한 레벨에 미달된다는 메시지
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR1), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 12:
        {
            strColorKey = L"#cffff00";
            strm << strColorKey << L"\n"
                << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3)
                << g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime(GetGameMode())
                << strColorKeyEnd;
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR2) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 13:
        {
            strColorKey = L"#cffff00";
            strm << strColorKey << L"\n"
                << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3)
                << g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime(GetGameMode())
                << strColorKeyEnd;
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR6) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 14:
        {
            strColorKey = L"#cffff00";
            strm << strColorKey << L"\n"
                << g_kGlobalValue.m_kHeroDungeonInfo.GetInitTime()
                << strColorKeyEnd;
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        case 15:
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ERR_GAME_START_TIME), L"");
            break;
        case 16:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NEED_DUNGEON_TICKET), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        default:
        {
            std::wstring strTemp = g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CREATE_ROOM_FAIL2);
            strTemp += g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_CREATE_ROOM_ACK, Result_Create);
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_CREATE_ROOM_FAIL1),
                strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true, true);
            break;
        }
        }

        return E_FAIL;
    }

    return S_OK;

}

HRESULT KGCRoomManager::CreateRoomDirect(const EGCGameModeCategory cGameCategory, const EGCGameMode eGameMode)
{
    if (g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP && g_MyD3D->m_pStateMachine->GetState() != GS_MATCH)
        return E_FAIL;

    if (cGameCategory != GC_GMC_DUNGEON)
        return E_FAIL;

    InitGameModeDirect(cGameCategory, eGameMode);

    // 입장 티켓조건이 걸린 던전인지 확인
    if (cGameCategory == GC_GMC_DUNGEON)
    {
        if (ENTER_IMPOSSIBLE == CheckTicketToDungeonEnter(false, eGameMode))
            return E_FAIL;

        int nPlayType = SiKGCWorldMapManager()->GetHeroDungeonPlayType(eGameMode);

        switch (nPlayType)
        {
        case KGCWorldMapManager::GRPT_NOT_INFO:
        case KGCWorldMapManager::GRPT_ADMISSION:
            break;
        case KGCWorldMapManager::GRPT_IMPOSSIBLE_ADMISSION:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return E_FAIL;
        }
        case KGCWorldMapManager::GRPT_CONDITIONS_BELOW:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return E_FAIL;
        }
        case KGCWorldMapManager::GRPT_IMPOSSIBLE_TIME:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR2),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return E_FAIL;
        }
        default:
            return E_FAIL;
        }
    }

    if (SiKGCWorldMapManager()->IsInfinityDungeon(SiKGCRoomManager()->GetGameMode()))
    {
        if (SiKGCInfinityDungeonManager()->GetInfinityDungeon(SiKGCRoomManager()->GetGameMode())->GetTotalATK() > SiKGCWorldMapManager()->GetChangeUserGradeInfo(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType))
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return E_FAIL;
        }
    }

    KRoomInfo kRoomInfo = CreateRoomInfo(m_vecDungeonRoomNameList[rand() % m_vecDungeonRoomNameList.size()], L"", MAX_PLAYER_NUM);
    KInDoorUserInfo kIndoorUserInfo = CreateIndoorUserInfo();

    Result_Create = INT_MAX;
    KP2P::GetInstance()->Send_CreateRoomReq(kRoomInfo, kIndoorUserInfo);

    // 게임을 생성한다.
    GetRoomInfo().iRestBanCount = 3;
    g_MyD3D->WaitForServerAck(Result_Create, INT_MAX, 60000, -1);

    if (S_OK != CheckCreateRoomError())
        return E_FAIL;

    g_MyD3D->MyHead->Clear_All_Cheat();
    g_MyD3D->bSlotFrameRestart = true;
    //g_KMci.Play( GC_BGM_LETS_GO, true );
    return S_OK;
}

HRESULT KGCRoomManager::CheckHeroInfo(const EGCGameModeCategory cGameCategory, const EGCGameMode eGameMode)
{
    HRESULT FinalResult = S_OK;

    if (cGameCategory != GC_GMC_DUNGEON)
        FinalResult = E_FAIL;

    if (cGameCategory == GC_GMC_DUNGEON)
    {
        int nPlayType = SiKGCWorldMapManager()->GetHeroDungeonPlayType(eGameMode);
        switch (nPlayType)
        {
            case KGCWorldMapManager::GRPT_NOT_INFO:
            case KGCWorldMapManager::GRPT_ADMISSION:
                FinalResult = S_OK;
                break;
            case KGCWorldMapManager::GRPT_IMPOSSIBLE_ADMISSION:
                FinalResult = E_FAIL;
                break;
            case KGCWorldMapManager::GRPT_CONDITIONS_BELOW:
                FinalResult = E_FAIL;
                break;
            case KGCWorldMapManager::GRPT_IMPOSSIBLE_TIME:
                FinalResult = E_FAIL;
                break;
            default:
                FinalResult = E_FAIL;
                break;
        }
    }

    return FinalResult;
}


HRESULT KGCRoomManager::CreateRoom(const std::wstring& strRoomName,
    const std::wstring& strPassword,
    int iMaxPlayer,
    bool bIsQuickJoin, KRoomOptions kOptions)
{


    if (IsCorrectPassword(strPassword) == false)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVALID_STRING_00));
        return E_FAIL;
    }

    InitGameMode();

    // 입장 티켓조건이 걸린 던전인지 확인
    if (GS_WORLDMAP == g_MyD3D->m_pStateMachine->GetState())
    {
        if (ENTER_IMPOSSIBLE == CheckTicketToDungeonEnter((false == bIsQuickJoin)))
            return E_FAIL;
        int nPlayType = SiKGCWorldMapManager()->GetHeroDungeonPlayType();


        switch (nPlayType)
        {
        case KGCWorldMapManager::GRPT_NOT_INFO:
        case KGCWorldMapManager::GRPT_ADMISSION:
        case KGCWorldMapManager::GRPT_IMPOSSIBLE_TIME:
            break;
        case KGCWorldMapManager::GRPT_IMPOSSIBLE_ADMISSION:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return E_FAIL;
        }
        case KGCWorldMapManager::GRPT_CONDITIONS_BELOW:
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return E_FAIL;
        }
        default:
            return E_FAIL;
        }
    }

    if (SiKGCWorldMapManager()->IsInfinityDungeon(SiKGCRoomManager()->GetGameMode()))
    {
        if (SiKGCInfinityDungeonManager()->GetInfinityDungeon(SiKGCRoomManager()->GetGameMode())->GetTotalATK() > SiKGCWorldMapManager()->GetChangeUserGradeInfo(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType))
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8),
                L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return E_FAIL;
        }
    }

    if (!SiKSlangFilter()->FilterCheck(strRoomName, KSlangFilter::SLANG_FILTER_NICKNAME))
    {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME, L"", L"", (KGCUIScene::EGCMsgBoxUse)GetUse(), GetlParam1(), GetlParam2(), true );
        std::wstring wstrerr = g_pkStrLoader->GetString(STR_ID_INVALID_TEXT_INPUT);
        wstrerr += L"\n";
        wstrerr += g_pkStrLoader->GetString(STR_ID_NICK_NAME_ANOTHER_USE);

        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            wstrerr,
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        //Destroy();
        return E_FAIL;
    }

    KRoomInfo kRoomInfo = CreateRoomInfo(strRoomName, strPassword, iMaxPlayer);
    kRoomInfo.m_kOptions = kOptions;
    KInDoorUserInfo kIndoorUserInfo = CreateIndoorUserInfo();

    Result_Create = INT_MAX;
    if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE)
    {
        DWORD dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();
        KP2P::GetInstance()->Send_CreateRoomInSquareReq(dwChannelUID, kRoomInfo, kIndoorUserInfo);
    }
    else
    {
        if (g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP && g_MyD3D->m_pStateMachine->GetState() != GS_MATCH) {
            return E_FAIL;
        }
        else {
            KP2P::GetInstance()->Send_CreateRoomReq(kRoomInfo, kIndoorUserInfo);
        }
    }

    // 게임을 생성한다.
    GetRoomInfo().iRestBanCount = 3;
    g_MyD3D->WaitForServerAck(Result_Create, INT_MAX, 60000, -1);

    if (S_OK != CheckCreateRoomError())
        return E_FAIL;

    g_MyD3D->MyHead->Clear_All_Cheat();
    g_MyD3D->bSlotFrameRestart = true;
    //g_KMci.Play( GC_BGM_LETS_GO, true );
    return S_OK;
}


HRESULT KGCRoomManager::CreateRoomParty(const std::wstring& strRoomName,
    const std::wstring& strPassword,
    int iGameMode,
    int iDungeonLv,
    EGCGameModeCategory cGameCategory,
    int nSubGameMode,
    EGCGameStage gameStage,
    bool bIsDungeon)
{
#if defined(USE_ONLY_TCP_RELAY)
    KP2P::GetInstance()->Send_PingInfoNot(g_kGlobalValue.GetTRAvgPing());
    // 제한 핑을 넘어서는 평균 핑을 가지는 유저들에 대해서는 대전방을 만들 수 없도록 한다. 멀티플레이는 하지 못하게...ㅉㅉ
    if (g_kGlobalValue.m_dwJoinRoomLimitPing != -1 && g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP)
    {
        if (static_cast<int>(g_kGlobalValue.m_dwJoinRoomLimitPing) < g_kGlobalValue.GetTRAvgPing())
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_BAD_PING_USER_JOIN_LIMIT_MESSAGE1),
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return S_OK;
        }
    }
#if defined(USE_PING_STATE_UI)
    KP2P::GetInstance()->Send_RoomMemberPingInfoReq();
#endif
#endif
    if (!SiKSlangFilter()->FilterCheck(strRoomName, KSlangFilter::SLANG_FILTER_NICKNAME))
    {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME, L"", L"", (KGCUIScene::EGCMsgBoxUse)GetUse(), GetlParam1(), GetlParam2(), true );
        std::wstring wstrerr = g_pkStrLoader->GetString(STR_ID_INVALID_TEXT_INPUT);
        wstrerr += L"\n";
        wstrerr += g_pkStrLoader->GetString(STR_ID_NICK_NAME_ANOTHER_USE);

        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            wstrerr,
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
        //Destroy();
        return false;
    }


    InitGameModeParty(cGameCategory, static_cast<EGCGameMode> (iGameMode), gameStage, nSubGameMode, bIsDungeon);

    if (bIsDungeon) {
        KRoomInfo kRoomInfo = CreateRoomInfoPartyDungeon(strRoomName, strPassword, iGameMode, nSubGameMode, iDungeonLv);
        KInDoorUserInfo kIndoorUserInfo = CreateIndoorUserInfo();
        Result_Create = INT_MAX;
        KP2P::GetInstance()->Send_CreateRoomReq(kRoomInfo, kIndoorUserInfo);
    }
    else
    {
        KRoomInfo kRoomInfo = CreateRoomInfoPartyMatch(strRoomName, strPassword, iGameMode, cGameCategory, nSubGameMode, gameStage);
        KInDoorUserInfo kIndoorUserInfo = CreateIndoorUserInfo();
        Result_Create = INT_MAX;
        KP2P::GetInstance()->Send_CreateRoomReq(kRoomInfo, kIndoorUserInfo);
    }

    // 게임을 생성한다.
    GetRoomInfo().iRestBanCount = 3;
    g_MyD3D->WaitForServerAck(Result_Create, INT_MAX, 60000, -1);

    if (S_OK != CheckCreateRoomError())
        return E_FAIL;

    g_MyD3D->MyHead->Clear_All_Cheat();
    g_MyD3D->bSlotFrameRestart = true;
    return S_OK;
}

HRESULT KGCRoomManager::JoinRoom(const UINT uiJoinFlag, std::map<DWORD, DWORD> mapJoinOption, short shRoomID, const std::wstring& strPassword, bool bCenterRoom, bool bSendPacket/* = true*/)
{
    m_mapJoinOption = mapJoinOption;
    return JoinRoom(uiJoinFlag, shRoomID, strPassword, bCenterRoom, bSendPacket);
}

HRESULT KGCRoomManager::JoinRoom(const UINT uiJoinFlag, short shRoomID, const std::wstring& strPassword, bool bCenterRoom, bool bSendPacket/* = true*/)
{
    if (shRoomID != -1 && IsBannedRoom(shRoomID))
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_KICKED_ROOM1),
            g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_KICKED_ROOM2),
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        return E_FAIL;
    }
    else
    {
        //UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );
        InitGameMode();

        char cQuickJoinCategory = GC_GMC_INVALID;
        int iQuickJoinMode = GC_GM_INVALID;
        int nDifficult = 0;

        int iMsgID[2] = { 0, };
        switch (uiJoinFlag)
        {
            case KJoinRoomReqInfo::JOIN_NORMAL:
            {
                GetRoomInfo().bPublicRoom = strPassword.empty();
                break;
            }
            break;
            case KJoinRoomReqInfo::JOIN_QUICK:
            {
                // 던전일때..
                if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
                {
                    cQuickJoinCategory = (char)GC_GMC_DUNGEON;
                    iQuickJoinMode = (int)SiKGCWorldMapManager()->GetGameMode();
                    nDifficult = GetDungeonLevel();
                }
                else // 대전일때..
                {
                    cQuickJoinCategory = SiKGCRoomListManager()->GetRoomFilter();
                }

                GetRoomInfo().bPublicRoom = true;
                break;
            }
            case KJoinRoomReqInfo::JOIN_GUILD:
                GetRoomInfo().bPublicRoom = true;
                break;
            default:
            {
                GetRoomInfo().bPublicRoom = strPassword.empty();
                break;
            }
        }

        std::vector<USHORT> vecKickedRoomID;
        // 강퇴 당한지 30초가 안된 방이 있다면?
        GetBannedRoomID(vecKickedRoomID);

        // 입장 티켓조건이 걸린 던전인지 확인
        if (GC_GMC_DUNGEON == cQuickJoinCategory)
        {
            if (ENTER_IMPOSSIBLE == CheckTicketToDungeonEnter(true)) {
                Result_Join = EPWET_QUICK_JOIN_TIME_OVER;
                return E_FAIL;

            }
        }

        if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP) {
            int nPlayType = SiKGCWorldMapManager()->GetHeroDungeonPlayType(static_cast<EGCGameMode>(iQuickJoinMode));

            switch (nPlayType)
            {
            case KGCWorldMapManager::GRPT_NOT_INFO:
            case KGCWorldMapManager::GRPT_ADMISSION:
                break;
            case KGCWorldMapManager::GRPT_IMPOSSIBLE_ADMISSION:
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4),
                    L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return E_FAIL;
            }
            break;
            case KGCWorldMapManager::GRPT_CONDITIONS_BELOW:
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8),
                    L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return E_FAIL;
            }
            break;
            case KGCWorldMapManager::GRPT_IMPOSSIBLE_TIME:
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR2),
                    L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return E_FAIL;
            }
            break;
            }
        }

        if (SiKGCWorldMapManager()->IsInfinityDungeon(SiKGCRoomManager()->GetGameMode()))
        {
            if (SiKGCInfinityDungeonManager()->GetInfinityDungeon(SiKGCRoomManager()->GetGameMode())->GetTotalATK() > SiKGCWorldMapManager()->GetChangeUserGradeInfo(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType))
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR8),
                    L"",
                    KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return E_FAIL;
            }
        }

        Result_Join = -1;
        KInDoorUserInfo kUserInfo;
        g_kGlobalValue.m_kUserInfo.SetIndoorUserInfo(&kUserInfo);

        //길드 서버에서는 길드에 맞게 정렬을 하자
        //방장을 찾아라
        for (unsigned char i = 0; i < MAX_PLAYER_NUM; i++)
        {
            bool bHost = g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost;
            DWORD dwUID = g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID;
            int iGUildID = g_MyD3D->MyPlayer[i]->m_kUserInfo.iGuildID;
            std::wstring strLogin = g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin;

            if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost)
            {
                //bool bHost = g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost;
                //DWORD dwUID = g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID;
                //int iGUildID = g_MyD3D->MyPlayer[i]->m_kUserInfo.iGuildID;
                //std::wstring strLogin = g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin;
                //g_MyD3D->MyPlayer[i]->m_kUserInfo.m_iteam
            }
        }


        if (true == bSendPacket)
        {
            if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE)
            {
                DWORD dwChannelUID = SiKGCChannelManager()->GetCurrentChannel();
                KP2P::GetInstance()->Send_JoinRoomFromSquareReq(dwChannelUID, uiJoinFlag,
                    shRoomID, strPassword.c_str(), kUserInfo, vecKickedRoomID,
                    cQuickJoinCategory, iQuickJoinMode, nDifficult,
                    bCenterRoom);
            }
            else
            {
                KP2P::GetInstance()->Send_JoinRoomReq(uiJoinFlag, shRoomID, strPassword.c_str(),
                    kUserInfo, vecKickedRoomID,
                    cQuickJoinCategory, iQuickJoinMode, nDifficult,
                    bCenterRoom, m_mapJoinOption);
                m_mapJoinOption.clear();
            }
            g_MyD3D->WaitForServerAck(Result_Join, -1, 5000, 6);

            switch (Result_Join)
            {
            case 4:
            {
                iMsgID[0] = STR_ID_APPMSG_ERR_CANNOT_JOIN_ROOM1;
                iMsgID[1] = STR_ID_BLANK;
                break;
            }
            case 6:
            {
                iMsgID[0] = STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1;
                iMsgID[1] = STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2;
                break;
            }
            case 5:
            {
                //if ( g_pkRoomList->GetP2PNewVersion() <= (int)CLIENT_P2P_VER )
                {
                    iMsgID[0] = STR_ID_DIFF_P2P_VERSION1;
                    iMsgID[1] = STR_ID_DIFF_P2P_VERSION2;
                }
                //else
                {
                    iMsgID[0] = STR_ID_DIFF_P2P_VERSION3;
                    iMsgID[1] = STR_ID_DIFF_P2P_VERSION4;
                }
                break;
            }
            case 1:
            {
                iMsgID[0] = STR_ID_APPMSG_ERR_CANNOT_JOIN_ROOM1; // 현재 선택하신 유저의 방에
                iMsgID[1] = STR_ID_APPMSG_ERR_CANNOT_JOIN_ROOM2; // 참여할 수 없는 상황입니다.
                break;
            }
            case 2:
            {
                if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
                    return E_FAIL;

                iMsgID[0] = STR_ID_APPMSG_ERR_ENTER_ROOM1;
                iMsgID[1] = STR_ID_APPMSG_ERR_ENTER_ROOM2;
                break;
            }
            case 3:
            {
                iMsgID[0] = STR_ID_CAN_NOT_JOIN_GUILDROOM1;
                iMsgID[1] = STR_ID_CAN_NOT_JOIN_GUILDROOM2;
                break;
            }
            case 7:
            {
                iMsgID[0] = STR_ID_GUILD_BATTLE_ROOM_ENTER_ERROR;
                iMsgID[1] = STR_ID_BLANK;
                break;
            }
            case 100:
                return E_FAIL;
                break;
            }

            // 퀵조인

            if (Result_Join != 0)
            {
                if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO) ||
                    uiJoinFlag == KJoinRoomReqInfo::JOIN_QUICK)
                {
                    // 용사의 섬이나 퀵 조인 시 실패 했을 때는 메세지 띄워주지 않는다!!
                }
                else
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetString(iMsgID[0]),
                        g_pkStrLoader->GetString(iMsgID[1]),
                        KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                }
                m_bCreateToJoin = true;
                return E_FAIL;
            }
        }

        return S_OK;
    }
}

HRESULT KGCRoomManager::CreateRoomInSquare(const std::wstring& strRoomName, const std::wstring& strPassword, int iMaxPlayer)
{
    return S_OK;
}

KRoomInfo KGCRoomManager::CreateRoomInfo(const std::wstring& strRoomName, const std::wstring& strPassword, int iMaxPlayer)
{
    KRoomInfo kRoomInfo;
    kRoomInfo.m_strRoomName = strRoomName;
    kRoomInfo.m_bPublic = strPassword.empty();
    kRoomInfo.m_strRoomPasswd = strPassword;
    kRoomInfo.m_cGameCategory = GetGameModeCategory();
    kRoomInfo.m_iGameMode = GetGameMode();
    kRoomInfo.m_iSubGameMode = GetSubGameMode();
    kRoomInfo.m_iMapID = g_MyD3D->MyStg->GetStage();
    kRoomInfo.m_iP2PVersion = CLIENT_P2P_VER;
    kRoomInfo.m_nDifficulty = GetDungeonLevel();
    kRoomInfo.m_usMaxUsers = iMaxPlayer;
    kRoomInfo.m_bRandomableMap = GetRoomInfo().bRandomMapMode;
    kRoomInfo.m_cRoutingMethod = DRR_TURN_RANDOM;
    kRoomInfo.m_nMonsterID = GetRoomInfo().iMonsterID;
    kRoomInfo.m_nMonsterCount = GetRoomInfo().iMonsterCount;
    kRoomInfo.m_bDeathMatchBalancing = GetRoomInfo().bDeathMatchBalancing;
    kRoomInfo.m_bDeathMatchIntrudable = GetRoomInfo().bDeathMatchIntrudable;
    kRoomInfo.m_nDeathKillCount = 20;
    kRoomInfo.m_nDeathMatchTime = 300;
    kRoomInfo.m_bGuild = IsChannelRoomList();

    // 현재 설정된 던전의 maxUser를 가져온다.
    if (kRoomInfo.m_cGameCategory == GC_GMC_DUNGEON)
    {
        kRoomInfo.m_usMaxUsers = SiKGCWorldMapManager()->GetDungeonMaxPlayerNum(GetGameMode());
    }

    return kRoomInfo;
}

KRoomInfo KGCRoomManager::CreateRoomInfoPartyDungeon
(const std::wstring& strRoomName, const std::wstring& strPassword, int iGameMode, int nSubGameMode, int iDungeonLv)
{
    KRoomInfo kRoomInfo;
    kRoomInfo.m_strRoomName = strRoomName;
    kRoomInfo.m_bPublic = strPassword.empty();
    kRoomInfo.m_strRoomPasswd = strPassword;
    kRoomInfo.m_cGameCategory = GC_GMC_DUNGEON;
    kRoomInfo.m_iGameMode = iGameMode;
    kRoomInfo.m_iSubGameMode = nSubGameMode;
    kRoomInfo.m_iMapID = g_MyD3D->MyStg->GetStage();
    kRoomInfo.m_bRandomableMap = false;
    kRoomInfo.m_iP2PVersion = CLIENT_P2P_VER;

    ////몬스터 대전 사용
    kRoomInfo.m_nDifficulty = iDungeonLv;
    kRoomInfo.m_usMaxUsers = MAX_DUNGEON_PLAYER_NUM;
    kRoomInfo.m_bRandomableMap = GetRoomInfo().bRandomMapMode;
    kRoomInfo.m_cRoutingMethod = DRR_TURN_RANDOM;
    kRoomInfo.m_nMonsterID = GetRoomInfo().iMonsterID;
    kRoomInfo.m_nMonsterCount = GetRoomInfo().iMonsterCount;
    kRoomInfo.m_bDeathMatchBalancing = GetRoomInfo().bDeathMatchBalancing;
    kRoomInfo.m_bDeathMatchIntrudable = GetRoomInfo().bDeathMatchIntrudable;
    kRoomInfo.m_nDeathKillCount = 20;
    kRoomInfo.m_nDeathMatchTime = 300;
    kRoomInfo.m_bGuild = IsChannelRoomList();

    // 현재 설정된 던전의 maxUser를 가져온다.
    if (kRoomInfo.m_cGameCategory == GC_GMC_DUNGEON)
    {
        kRoomInfo.m_usMaxUsers = SiKGCWorldMapManager()->GetDungeonMaxPlayerNum(GetGameMode());
    }

    return kRoomInfo;
}

KRoomInfo KGCRoomManager::CreateRoomInfoPartyMatch
(const std::wstring& strRoomName, const std::wstring& strPassword, int iGameMode, EGCGameModeCategory cGameCategory, int nSubGameMode, EGCGameStage gameStage)
{
    KRoomInfo kRoomInfo;
    kRoomInfo.m_strRoomName = strRoomName;
    kRoomInfo.m_bPublic = strPassword.empty();
    kRoomInfo.m_strRoomPasswd = strPassword;
    kRoomInfo.m_cGameCategory = cGameCategory;
    kRoomInfo.m_iSubGameMode = nSubGameMode;
    kRoomInfo.m_iGameMode = iGameMode;
    kRoomInfo.m_iMapID = gameStage;//
    kRoomInfo.m_nDifficulty = 0;
    kRoomInfo.m_iP2PVersion = CLIENT_P2P_VER;

    kRoomInfo.m_usMaxUsers = MAX_PLAYER_NUM;
    kRoomInfo.m_bRandomableMap = GetRoomInfo().bRandomMapMode;
    kRoomInfo.m_cRoutingMethod = DRR_TURN_RANDOM;
    kRoomInfo.m_nMonsterID = GetRoomInfo().iMonsterID;
    kRoomInfo.m_nMonsterCount = GetRoomInfo().iMonsterCount;
    kRoomInfo.m_bDeathMatchBalancing = GetRoomInfo().bDeathMatchBalancing;
    kRoomInfo.m_bDeathMatchIntrudable = GetRoomInfo().bDeathMatchIntrudable;
    kRoomInfo.m_nDeathKillCount = 20;
    kRoomInfo.m_nDeathMatchTime = 300;
    kRoomInfo.m_bGuild = IsChannelRoomList();

    return kRoomInfo;
}

KInDoorUserInfo KGCRoomManager::CreateIndoorUserInfo()
{
    KInDoorUserInfo kIndoorUserInfo;
    g_kGlobalValue.m_kUserInfo.SetIndoorUserInfo(&kIndoorUserInfo);
    kIndoorUserInfo.m_dwGuildUID = g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID;
    kIndoorUserInfo.m_strNickColor = g_kGlobalValue.m_kUserInfo.strNickColor;
    return kIndoorUserInfo;
}

bool KGCRoomManager::IsCorrectPassword(const std::wstring& strPassword)
{
    std::wstring strFilteringPasswd = strPassword;
    g_pkChatManager->MessageFilter(strFilteringPasswd);

    return strFilteringPasswd == strPassword;
}

// 서버에서 보내준 유효한 던전인지를 검사한다.
bool KGCRoomManager::CheckValidDungeon()
{
    if (GetGameModeCategory() != GC_GMC_DUNGEON)
        return true;

    std::vector<EGCGameMode> vecGameMode = GetCheckDungeonGameMode();

    EGCGameMode eGame;
    if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
        eGame = GetGameMode();
    else
        eGame = SiKGCWorldMapManager()->GetGameMode();

    if (eGame == GC_GM_INVALID)
        return true;


    std::vector<EGCGameMode>::iterator vit = std::find(vecGameMode.begin(), vecGameMode.end(), eGame);

    if (vit == vecGameMode.end())
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_OPEN_DUNGEON_ENTRANCE_ERROR),
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        return false;
    }
    else
        return true;
}

bool KGCRoomManager::MoveRoom(USHORT  usRoomID, bool bPublic, std::wstring wsRoomPass)
{

    if (g_MyD3D->m_pStateMachine->IsStateRoom())
    {
        if (GetRoomInfo().usRoomID == usRoomID)
            return true;

        bool bSubtractRP = false;
        if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO) && g_MyD3D->GetPlayerNum() > 1)
        {
            bSubtractRP = true;
        }

        // 2010-10-12, shmhlove, 레디 상태였다면 메시지박스와 유저상태를 변경합니다.
        if (g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY)
        {
            g_pkUIScene->ClearWaitMessageBox();
            g_pkUIScene->EnableMessageBox(true);
            g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_WAIT;
        }

        g_MyD3D->m_pStateMachine->ExitRoom(true, KFailRate::P2P_NO_ERROR, bSubtractRP);
        SiKGCRoomManager()->SetPlayedHeroDungeon(false);

    }

    if (bPublic == false && wsRoomPass != L"")
    {
        JoinRoom(KJoinRoomReqInfo::JOIN_NORMAL, usRoomID, wsRoomPass, IsChannelRoomList(), true);
    }
    else if (bPublic == false)
    {
        SiKGCRoomManager()->GetRoomInfo().usRoomID = usRoomID;
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_PASSWORD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);

    }
    else if (E_FAIL == JoinRoom(KJoinRoomReqInfo::JOIN_NORMAL, usRoomID, L"", IsChannelRoomList(), true))
        return false;

    return true;
}

bool KGCRoomManager::IsPlayerObserver(DWORD dwUserUID)
{
    std::vector<SUserInfo>& observer = GetRoomInfo().vecObserverUserInfo;

    for (std::vector<SUserInfo>::iterator vit = observer.begin(); vit != observer.end(); vit++)
    {
        if (vit->dwUID == dwUserUID)
            return true;
    }
    return false;
}

void KGCRoomManager::SetValidCategory()
{
    std::map< EChannelType, std::vector<EGCGameModeCategory> > ::iterator mapChannelCategory = m_mapChannelCategory.begin();

    for (; mapChannelCategory != m_mapChannelCategory.end(); mapChannelCategory++)
    {
        if (mapChannelCategory->first == CT_UNLIMITED)
        {
            int j = static_cast<int> (mapChannelCategory->second.size());
            for (int i = 0; i < j; ++i)
            {
                vecValidCategory.insert(std::make_pair(i, mapChannelCategory->second[i]));
            }
        }
    }
}
void KGCRoomManager::InitObserver(DWORD dwUserUID)
{
    std::vector<SUserInfo>& observer = GetRoomInfo().vecObserverUserInfo;

    for (std::vector<SUserInfo>::iterator vit = observer.begin(); vit != observer.end(); vit++)
    {
        if (vit->dwUID == dwUserUID || dwUserUID == -1)
            vit->Init();
    }
}
bool KGCRoomManager::IsObserverLive(DWORD dwUserUID)
{
    std::vector<SUserInfo>& observer = GetRoomInfo().vecObserverUserInfo;

    for (std::vector<SUserInfo>::iterator vit = observer.begin(); vit != observer.end(); vit++)
    {
        if (vit->dwUID == dwUserUID || dwUserUID == -1)
            return vit->bLive;
    }
    return false;
}

bool KGCRoomManager::IsAngelEggModeAbility()
{
    //	능력치 평준화 체크할때만 사용중이다.
    //	쓸때 잘보고 쓰세요.
    //	알깨기방에서만 적용되니깐...음..
    //	대전 관련 게임 스테이트 적용
    //	방, 방장비창, 게임중, 로딩중, 종료중
    //	위 스테이트는 던전 모드에서 사용되지만 GameMode가 틀리므로 상관없다.
    GAME_STATE emGameState = g_MyD3D->m_pStateMachine->GetState();
    if ((emGameState == GS_ROOM || emGameState == GS_MY_INFO_FROM_ROOM ||
        emGameState == GS_CHAR_SELECT_FROM_ROOM ||
        emGameState == GS_GAME || emGameState == GS_GAME_LOADING ||
        emGameState == GS_GAME_END)
        && GetGameModeCategory() == GC_GMC_ANGELS_EGG)
    {
        return true;
    }

    return false;
}


bool KGCRoomManager::IsDotaModeAbility()
{
    //	능력치 평준화 체크할때만 사용중이다.
    //	쓸때 잘보고 쓰세요.
    //	알깨기방에서만 적용되니깐...음..
    //	대전 관련 게임 스테이트 적용
    //	방, 방장비창, 게임중, 로딩중, 종료중
    //	위 스테이트는 던전 모드에서 사용되지만 GameMode가 틀리므로 상관없다.
    GAME_STATE emGameState = g_MyD3D->m_pStateMachine->GetState();
    if ((emGameState == GS_ROOM || emGameState == GS_MY_INFO_FROM_ROOM ||
        emGameState == GS_CHAR_SELECT_FROM_ROOM ||
        emGameState == GS_GAME || emGameState == GS_GAME_LOADING ||
        emGameState == GS_GAME_END)
        && GetGameModeCategory() == GC_GMC_DOTA)
    {
        return true;
    }

    return false;
}

int KGCRoomManager::CheckTicketToDungeonEnter(bool bErrorMsg, int iGameMode_, bool* pbGoldStage)
{
    int iResult = NOT_ENTER_CONDITION;
    int iGameMode = GetGameMode();

    if (-1 < iGameMode_)
        iGameMode = iGameMode_;

    std::map<int, KDungeonTicket>::iterator mapIter = m_mapDungeonTicket.find(iGameMode);
    if (mapIter != m_mapDungeonTicket.end())
    {
        KItem* pItem;
        std::map<GCITEMID, int>::iterator mapTicketIter = mapIter->second.m_mapEnterTicket.begin();
        for (; mapTicketIter != mapIter->second.m_mapEnterTicket.end(); ++mapTicketIter)
        {
            pItem = g_pItemMgr->GetInventoryItemFromID(mapTicketIter->first / 10);
            if (pItem && pItem->m_nCount >= mapTicketIter->second)
                break;
        }

        if (mapTicketIter == mapIter->second.m_mapEnterTicket.end())
        {
            if (bErrorMsg) {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ERROR_DUNGEON_ENTER_TICKET), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            }
            iResult = ENTER_IMPOSSIBLE;
        }
        else
        {
            iResult = ENTER_POSSIBLE;
            if (NULL != pbGoldStage)
            {
                std::map<GCITEMID, int>::iterator mapBossTicketIter = mapIter->second.m_mapBossTicket.begin();
                for (; mapBossTicketIter != mapIter->second.m_mapBossTicket.end(); ++mapBossTicketIter)
                {
                    pItem = g_pItemMgr->GetInventoryItemFromID(mapBossTicketIter->first / 10);
                    if (pItem && pItem->m_nCount >= mapBossTicketIter->second)
                        break;
                }

                *pbGoldStage = (mapBossTicketIter != mapIter->second.m_mapBossTicket.end());
            }
        }
    }
    return iResult;
}

void KGCRoomManager::AddBannedRoom(USHORT usBanRoomID, DWORD dwBanTime)
{
    SBanInfo sBanInfo;
    sBanInfo.usBannedRoomID = usBanRoomID;
    sBanInfo.dwBannedTime = dwBanTime;

    if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
    {
        sBanInfo.emGameCategory = GC_GMC_DUNGEON;
    }
    else
    {
        sBanInfo.emGameCategory = GC_GMC_MATCH;
    }

    m_cvBanInfo.push_back(sBanInfo);
}

bool KGCRoomManager::IsBannedRoom(USHORT usRoomID)
{
    EGCGameModeCategory emGameCategory;
    if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
    {
        emGameCategory = GC_GMC_DUNGEON;
    }
    else
    {
        emGameCategory = GC_GMC_MATCH;
    }


    for (int i = 0; i < (int)m_cvBanInfo.size(); ++i)
    {
        if (m_cvBanInfo[i].emGameCategory != emGameCategory)
            continue;

        if (m_cvBanInfo[i].usBannedRoomID != usRoomID)
            continue;

        if (m_cvBanInfo[i].dwBannedTime + 1000 * 30/*초*/ > ::timeGetTime())
            return true;
    }

    return false;
}

void KGCRoomManager::GetBannedRoomID(std::vector<USHORT>& vecKickedRoomID)
{
    vecKickedRoomID.clear();


    EGCGameModeCategory emGameCategory;
    if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
    {
        emGameCategory = GC_GMC_DUNGEON;
    }
    else
    {
        emGameCategory = GC_GMC_MATCH;
    }


    for (int i = 0; i < (int)m_cvBanInfo.size(); ++i)
    {
        if (m_cvBanInfo[i].emGameCategory != emGameCategory)
            continue;

        if (m_cvBanInfo[i].dwBannedTime + 1000 * 30/*초*/ < ::timeGetTime())
            continue;
        vecKickedRoomID.push_back(m_cvBanInfo[i].usBannedRoomID);
    }

    return;
}

bool KGCRoomManager::IsHellMode()
{
    return m_kRoomInfo.nDungeonLevel == KGCRoomListManager::KRLM_LEVEL_HELL;
}

bool KGCRoomManager::IsBreakMode()
{
    return m_kRoomInfo.nDungeonLevel == KGCRoomListManager::KRLM_LEVEL_BREAK;
}

std::set< EGCGameStage >  KGCRoomManager::GetGameModeListInChannel(EChannelType eChannelType)
{
    std::vector<KGCGameModeCategory> vecShowGameModeCategory;

    // 채널에서 사용할 수 있는 카테고리를 얻어온다
    std::map< EChannelType, std::vector< EGCGameModeCategory > >::iterator itor = m_mapChannelCategory.find(eChannelType);
    std::vector<EGCGameModeCategory> vecGameModeCategory;

    if (itor != m_mapChannelCategory.end())
        vecGameModeCategory = itor->second;
    else
        vecGameModeCategory = m_mapChannelCategory[CT_UNLIMITED];

    // 전체 게임모드 중에서 해당 채널에서 사용 할 수 있는 녀석만 걸러서 세팅해 준다
    for (int i = 0; i < (int)vecGameModeCategory.size(); ++i)
    {
        if (vecGameModeCategory[i] == GC_GMC_COUNT) continue;

        for (int j = 0; j < (int)m_vecGameModeCategory.size(); ++j)
        {
            if (vecGameModeCategory[i] == (int)m_vecGameModeCategory[j].m_eGameModeCategory)
            {
                vecShowGameModeCategory.push_back(m_vecGameModeCategory[j]);
            }
        }
    }

    std::set<EGCGameStage> setTemp;

    for (std::vector<KGCGameModeCategory>::iterator vit = vecShowGameModeCategory.begin(); vit != vecShowGameModeCategory.end(); ++vit) {
        for (std::vector<KGCGameMode>::iterator vitGameMode = vit->m_vecGameMode.begin(); vitGameMode != vit->m_vecGameMode.end(); ++vitGameMode) {
            for (std::vector<KGCGameStage>::iterator vitGameStage = vitGameMode->m_vecGameStage.begin(); vitGameStage != vitGameMode->m_vecGameStage.end(); ++vitGameStage) {
                setTemp.insert(vitGameStage->m_eGameStage);
            }
        }
    }

    return setTemp;
}

bool KGCRoomManager::CanStartAutoMachGame()
{
    time_t curTime = GCUTIL_TIME::GetCurTime();

    if (curTime > m_tmPreQuestionStartAutoGame) {
        Result_StartGamePossibleReq = INT_MAX;
        KP2P::GetInstance()->Send_StartGamePossibleReq();
        g_MyD3D->WaitForServerAck(Result_StartGamePossibleReq, -1, 3000, INT_MAX);

        m_tmPreQuestionStartAutoGame = curTime + 10;
    }

    if (Result_StartGamePossibleReq == 0)
        return true;

    int iUserCount = 0;
    for (unsigned char i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
        {
            iUserCount++;
        }
    }
    return iUserCount == SiKGCMatchManager()->GetRoomUserCnt();
}


void KGCRoomManager::SetRecvRoomInfo(bool bRecv, KRoomInfo kInfo)
{
    m_prRcvRoomInfo.first = bRecv;
    m_prRcvRoomInfo.second = kInfo;
}

bool KGCRoomManager::GetRecvRoomInfo(KRoomInfo& kInfo)
{
    if (m_prRcvRoomInfo.first) {
        kInfo = m_prRcvRoomInfo.second;
        return true;
    }
    return m_prRcvRoomInfo.first;
}

void KGCRoomManager::SetRecvRelayServerStatus(KEVENT_RELAY_SERVER_STATUS_ACK& kRecv)
{
    m_prRcvRoomInfo.second.m_dwTRelayServerIP = kRecv.m_dwTRelayServerIP;
    m_prRcvRoomInfo.second.m_usTRelayServerPort = kRecv.m_usTRelayServerPort;
}
