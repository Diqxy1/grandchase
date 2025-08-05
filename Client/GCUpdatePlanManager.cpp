#include "GCUpdatePlanManager.h"

#include "../GCUI/KGCWorldMapContainer.h"
#include "../GCUI/GCSkillTreeDlg.h"
#include "../GCUI/KGCMiniGameDlg.h"
#include "../GCUI/GCSceneMenuS6.h"
#include "../GCUI/KGCNewTutorialOption.h"

#include "../GCUI/GCCharGainScene.h"
#include "../GCUI/GCCharIntroFrame.h"
#include "../GCUI/GCEventMinigameList.h"
#include "../GCUI/GCServerScene.h"

#include "../GCUI/GCCashShopScene.h"
#include "../GCUI/GCGPShopScene.h"
#include "../GCUI/GCCoordiShopScene.h"
#include "../GCUI/KGCNewForgeDlg.h"

#include "../CommonFiles/UpdatePlanScriptID.h"



ImplementSingleton(GCUpdatePlanManager);

GCUpdatePlanManager::GCUpdatePlanManager(void)
{
    
}

GCUpdatePlanManager::~GCUpdatePlanManager(void)
{
}

void GCUpdatePlanManager::Update_EVENT_CLIENT_CONTENTS_OPEN_INFO( IN KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kRecv_ )
{
    if ( kRecv_.m_nSendType != KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT::ECCOI_DATA_CHANGED )
        return;

    if ( false == kRecv_.m_prGameCategoryInfo.first.empty() )
    {
        SiKGCRoomManager()->LoadGameModeFromPacket( kRecv_ );
        SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType()  );
     
        if ( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
        {
            g_pkUIScene->m_pkWorldMapContainer->RefreshOpenedContinent();
        }
    }

    if ( false == kRecv_.m_prGameCharInfo.first.empty() )
    {
        std::vector<int>::iterator vitGChar = kRecv_.m_prGameCharInfo.first.begin();
        for ( ; vitGChar!=kRecv_.m_prGameCharInfo.first.end() ; ++vitGChar )
        {
            switch( *vitGChar )
            {
            case KCCGameCharInfo::EV_MAXLEVEL:
                g_kGlobalValue.SetMaxLevel(kRecv_.m_prGameCharInfo.second.m_nMaxLevel);
                break;
            case KCCGameCharInfo::EV_CHARACTES:
                g_kGlobalValue.SetOpenedCharacterInfo( kRecv_.m_prGameCharInfo.second.m_mapCharactes );
                break;
            case KCCGameCharInfo::EV_CHARSP4OPENINFO:
                 g_kGlobalValue.SetCharSp4OpenInfo( kRecv_.m_prGameCharInfo.second.m_vecCharSP4OpenInfo );
                break;
            case KCCGameCharInfo::EV_CHAROPENTYPE:
                g_kGlobalValue.SetCharOpenType(kRecv_.m_prGameCharInfo.second.m_mapCharOpenType);
                break;
            case KCCGameCharInfo::EV_CHARCASHGOODS:
                 g_kGlobalValue.m_mapCharCashGoods = kRecv_.m_prGameCharInfo.second.m_mapCharCashGoods;
                break;
            case KCCGameCharInfo::EV_CHARSKILLENABLE:
                g_kGlobalValue.SetCharSkillEnable(kRecv_.m_prGameCharInfo.second.m_setCharSkillEnable);
                break;
            }
        }

        if ( g_pkUIScene->m_pkNewTutorialOption->IsRenderOn() )
        {
            g_pkUIScene->m_pkNewTutorialOption->GetCharSelectBox()->OnCreateComplete();
        }
        else if ( g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_GAIN && g_pkUIScene->m_pkCharGainScene->GetCharIntroFrame()->IsRenderOn() )
        {
            g_pkUIScene->m_pkCharGainScene->GetCharIntroFrame()->OnCreate();
        }

        if ( g_pkUIScene->m_pkSkillTree->IsRenderOn() )
        {
            g_pkUIScene->m_pkSkillTree->OnCreate();
        }
    }

    if ( false == kRecv_.m_prMonsterInfo.first.empty() )
    {
        std::vector<int>::iterator vitMon = kRecv_.m_prMonsterInfo.first.begin();
        for ( ; vitMon!=kRecv_.m_prMonsterInfo.first.end() ; ++vitMon )
        {
            switch( *vitMon )
            {
            case KCCMonsterInfo::EV_CHAMPIONRATIO:
                g_kGlobalValue.SetChampionAbility(kRecv_.m_prMonsterInfo.second.m_mapChampionRatio);
                break;
            case KCCMonsterInfo::EV_MONSTERABILITY:
                g_kGlobalValue.SetEventMonsterList(kRecv_.m_prMonsterInfo.second.m_mapMonsterAbility);
                break;
            }
        }
    }

    if (false == kRecv_.m_prMiniEventInfo.first.empty() )
    {
        std::vector<int>::iterator vitMini = kRecv_.m_prMiniEventInfo.first.begin();
        for ( ; vitMini!=kRecv_.m_prMiniEventInfo.first.end() ; ++vitMini )
        {
            switch( *vitMini )
            {
            case KCCMiniEventnInfo::EV_DISABLEMENULIST:
                g_kGlobalValue.SetDisableUIBtnList( kRecv_.m_prMiniEventInfo.second.m_vecDisableMenuList );
                break;
            case KCCMiniEventnInfo::EV_EVENTOPENINFO:
                g_kGlobalValue.m_vecEventOpenInfo = kRecv_.m_prMiniEventInfo.second.m_vecEventOpenInfo;
                break;
            }
        }

        if ( g_pkUIScene->m_pkEventMinigameList->IsRenderOn() )
        {
            g_pkUIScene->m_pkEventMinigameList->OnCreate();
        }
        
        //월드맵 파티클이 미니게임 리스트에 영향을 받는 케이스가 있어서..
        if ( g_pkUIScene->m_pkWorldMapContainer->IsRenderOn() )
        {
            g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap();
        }

        if ( g_pkUIScene->m_pkSceneMenuBtnS6->IsRenderOn() )
        {
            //OnCreate 함수 안에 openMenu 변수를 false 로 만들어 주는 부분이 있음..그래서 결국 if문으로 나눔
            if ( g_pkUIScene->m_pkSceneMenuBtnS6->GetOpenMenu() )
            {
                g_pkUIScene->m_pkSceneMenuBtnS6->OnCreate();
                g_pkUIScene->m_pkSceneMenuBtnS6->OnOpenBtnClick();
            }
            else
            {
                g_pkUIScene->m_pkSceneMenuBtnS6->OnCreate();
            }

        }
    }
}

void GCUpdatePlanManager::Update_EVENT_CLIENT_CONTENTS_FIRST_INIT_INFO( IN KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT& kRecv_ )
{
    if ( kRecv_.m_nSendType != KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT::ECCFII_DATA_CHANGED )
        return;

    if ( false == kRecv_.m_prUsingFilesInfo.first.empty() )
    {
        std::vector<int>::iterator vitUsing = kRecv_.m_prUsingFilesInfo.first.begin();
        for ( ; vitUsing!=kRecv_.m_prUsingFilesInfo.first.end() ; ++vitUsing )
        {
            switch( *vitUsing )
            {
            case KCCUsingFilesInfo::EV_LOADINGIMAGENAME:
                g_kGlobalValue.SetLoadingImage( kRecv_.m_prUsingFilesInfo.second.m_mapLoadingImageName );
                break;
            case KCCUsingFilesInfo::EV_NEWSNUMBER:
                g_kGlobalValue.SetNewFileNum( kRecv_.m_prUsingFilesInfo.second.m_vecNewsNumber );
                break;
            case KCCUsingFilesInfo::EV_PVPLOADINGIMAGENAME:
                g_kGlobalValue.SetPvPLoadingImageName(kRecv_.m_prUsingFilesInfo.second.m_mapPVPLoadingImageName);
                break;
            case KCCUsingFilesInfo::EV_EVENTBANNERINFO:
                g_kGlobalValue.SetEventBannerInfo(kRecv_.m_prUsingFilesInfo.second.m_vecEventBannerInfo);
                break;
            case KCCUsingFilesInfo::EV_SCRIPTNAME:
                g_kGlobalValue.SetSquareScriptName( kRecv_.m_prUsingFilesInfo.second.m_mapScriptName[KCCUsingFilesInfo::SCF_SQUARE],
                    kRecv_.m_prUsingFilesInfo.second.m_mapScriptName[KCCUsingFilesInfo::SCF_SQUAREOBJECT],
                    kRecv_.m_prUsingFilesInfo.second.m_mapScriptName[KCCUsingFilesInfo::SCF_SQUARE3DOBJECT]
                );
                break;
            }
        }

        if ( g_pkUIScene->m_pkServerScene->IsRenderOn() )
        {
            g_pkUIScene->m_pkServerScene->NewReLoad();
        }
    }

    if ( false == kRecv_.m_prHackCheckInfo.first.empty() )
    {
        std::vector<int>::iterator vitHack = kRecv_.m_prHackCheckInfo.first.begin();
        for ( ; vitHack!=kRecv_.m_prHackCheckInfo.first.end() ; ++vitHack )
        {
            switch( *vitHack )
            {
            case KCCHackCheckInfo::EV_EXCEPTIONMOTIONID:
                g_kGlobalValue.SetvecExceptionMotionID(kRecv_.m_prHackCheckInfo.second.m_vecExceptionMotionID);
                break;
            case KCCHackCheckInfo::EV_DLLBLACKLIST:
                g_kGlobalValue.m_setBlackDllList = kRecv_.m_prHackCheckInfo.second.m_setDLLBlackList;
                break;
            case KCCHackCheckInfo::EV_EXTENDSHALIST:
                break;
            }
        }
    }
}

void GCUpdatePlanManager::Update_EVENT_ITEM_BUY_INFO_NOT( IN KItemBuyInfo& kRecv_ )
{
    if ( false == g_pItemMgr->GetItemBuyInfo() )
    {//소트에서 아이템 데이터를 읽기 전에 이 패킷이 도착하였을때 데이터를 저장해놓는다
        g_pItemMgr->KeepItemBuyInfoData( kRecv_ );
        g_pItemMgr->SetItemBuyInfo( true );
    }
    else
    {//소트에서 이미 아이템데이터를 읽고난 다음 이 패킷이 왔다는 것
        g_pItemMgr->SetItemBuyInfoData( kRecv_ );
    }


    if ( g_pkUIScene->m_pkCashShopScene->IsRenderOn() )
    {
        g_pkUIScene->m_pkCashShopScene->OnInitialize();

    }
    else if ( g_pkUIScene->m_pkGPShopScene->IsRenderOn() )
    {
        g_pkUIScene->m_pkGPShopScene->OnInitialize();

    }
    else if ( g_pkUIScene->m_pkCoordiShopScene->IsRenderOn() )
    {
        g_pkUIScene->m_pkCoordiShopScene->OnInitialize();
    }

}

void GCUpdatePlanManager::Update_EVENT_UPDATE_EVENT_SCRIPT_NOT( IN std::vector< int >& kRecv_ )
{
    if ( true == kRecv_.empty() )
        return;

    std::vector< int >::iterator vit = kRecv_.begin();
    for ( ; vit!=kRecv_.end() ; ++vit )
    {
        EScriptID eScript = static_cast<EScriptID>(*vit);
        switch( eScript )
        {
        case SCI_INITDROP:
            break;
        case SCI_GAMEDROP:
            break;
        case SCI_GAWIBAWIBO:
            break;
        case SCI_MANUFACTURE:
            {
                //제작해지 정보를 다시 받기위해서는 기존 정보들을 Clear 해야 한다.
                SiGCForgeManager()->ClearRecipeAndBreakInfo();
//                 if ( g_pkUIScene->m_pkForgeDlg->IsRenderOn() )
//                 {
//                     if ( g_pkUIScene->m_pkForgeDlg->GetForgeCraftState() == KGCForgeItemCraftMachine::EFMS_NORMAL )
//                         g_pkUIScene->m_pkForgeDlg->OnCreate();
//                 }
            }
            break;
        }
    }
}
