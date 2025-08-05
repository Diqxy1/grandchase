#include "stdafx.h"
//
#include "MyD3D.h"
#include "Message.h"
#include "KDInput.h"
#include "Procedure.h"





#include "GCCameraOrtho.h"
//
#include "KGCRoomManager.h"

// 요청한 슬롯을 비운다.
void CMyD3DApplication::ClearSlot( int iIndex_, bool bInitUserInfo_ )
{    
	MyPlayer[iIndex_]->ClearPlayerData( bInitUserInfo_ );
    m_akPet[iIndex_]->EndPet();
    SiKGCRoomManager()->GetRoomInfo().bOpenSlot[iIndex_] = true;
}

HRESULT CMyD3DApplication::ViewMode_FrameMove()
{
    // 특정 시간이 지나면 메시지를 발사함
    // 왜냐하면 호스트가 몬스터나 아이템의 관리를 하기 때문...
    static int pLatency=0;
    pLatency++;   
    if(pLatency>LATENCY-1)
    {           
        SendNetworkData( GC_PID_PLAYERLATENCY_FRAME_INFO, 0);
        pLatency=0;
    }

    g_kCamera.ViewModeFrameMove();    

    return TRUE;
}   

void CMyD3DApplication::Select_FrameMove()
{
    // bSlotFrameRestart 값은 주로 게임이 끝나고 다시 대기실로 올 때 true로 되며
    // 모든 유저들의 슬롯에 모션을 제로로 세팅한다.
    if (bSlotFrameRestart)
    {
        for (int j = 0; j < MAX_PLAYER_NUM; j++)
        {
            MyPlayer[j]->cFrame = 0;
            MyPlayer[j]->vPos.x = 0;
            MyPlayer[j]->y_Ex = 0;
        }
        bSlotFrameRestart = false;
    }
    //====================================================================================
    // 2006.07.14 : Asirion
    // 운영자 권한이 있는 경우 방장 권한을 가질 수 있게 함
    // 한프레임에 하나의 명령만.
    if (g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN) // 최고권한이 있음
    {
        //====================================================================================
        // 2006.07.14 : Asirion
        // 인원수 조절 (넘버패드+/-)
        bool bChangeRoomInfo = false;
        if (diks[DIK_NUMPADMINUS] && !Old_diks[DIK_NUMPADMINUS]) // 인원수 줄일 것
        {
            int iIndex = SiKGCRoomManager()->GetEmptySlot();
            if (iIndex == -1)
                return;
            g_KDSound.PlayWithLoad("74");

            std::vector<std::pair<char, bool> > vecChangeSlot;
            vecChangeSlot.push_back(std::make_pair((char)iIndex, false));

            KP2P::GetInstance()->Send_ChangeRoomInfoReq(vecChangeSlot);
            return;
        }
        else if (diks[DIK_NUMPADPLUS] && !Old_diks[DIK_NUMPADPLUS]) // 인원수 늘일 것
        {
            int iIndex = SiKGCRoomManager()->GetClosedSlot();
            if (iIndex == -1)
                return;
            g_KDSound.PlayWithLoad("74");

            std::vector<std::pair<char, bool> > vecChangeSlot;
            vecChangeSlot.push_back(std::make_pair((char)iIndex, true));

            KP2P::GetInstance()->Send_ChangeRoomInfoReq(vecChangeSlot);
            return;
        }
        if (diks[DIK_LCONTROL] && diks[DIK_M] && !Old_diks[DIK_M]) // 게임 모드 카테고리 바꾸기
        {// 게임 모드 카테고리 변경 (좌측 컨트롤 버튼 + M)

            if (g_pkUIScene->m_pkRoomSceneS6->m_pkRoomMatchPanel->m_pkMatchInfo != NULL)
            {
                g_pkUIScene->m_pkRoomSceneS6->m_pkRoomMatchPanel->m_pkMatchInfo->ChangeGameModeCategory(true);
                bChangeRoomInfo = true;
            }
        }
        else if (diks[DIK_LCONTROL] && diks[DIK_N] && !Old_diks[DIK_N]) // 게임 맵 바꾸기
        {// 게임 맵 변경 (좌측 컨트롤 버튼 + N)

            if (g_pkUIScene)
            {
                if (g_pkUIScene->m_pkRoomSceneS6)
                {
                    if (g_pkUIScene->m_pkRoomSceneS6->m_pkRoomMatchPanel->m_pkMatchMapBox)
                    {
                        g_pkUIScene->m_pkRoomSceneS6->m_pkRoomMatchPanel->m_pkMatchMapBox->ChangeMap(true);
                        bChangeRoomInfo = true;
                        g_KDSound.Play("31");
                    }
                }
            }
        }
        else if (diks[DIK_LMENU] && diks[DIK_M] && !Old_diks[DIK_M]) // 서브 게임 모드 바꾸기
        {// 서브 게임 모드 변경 (좌측 알트 버튼 + M)

            if (g_pkUIScene->m_pkRoomSceneS6->m_pkRoomMatchPanel->m_pkMatchInfo != NULL)
            {
                g_pkUIScene->m_pkRoomSceneS6->m_pkRoomMatchPanel->m_pkMatchInfo->ChangeSubGameMode(true);
                bChangeRoomInfo = true;
            }

        }
        if (bChangeRoomInfo)
        {
            KP2P::GetInstance()->Send_ChangeRoomInfoReq(&SiKGCRoomManager()->GetRoomInfo());
            g_pkUIScene->RoomSceneUpdateData();
            return;
        }

        // 팀 변경 (좌측 알트 버튼 + 번호)
        bool bChangeTeam = (diks[DIK_LMENU] != 0);
        bool bTerminateUser = (diks[DIK_LCONTROL] != 0);
        int  iIndex = -1;
        for (int i = (int)DIK_1; i <= (int)DIK_6; i++)
        {
            if (diks[i] && !Old_diks[i])
            {
                iIndex = i - DIK_1;
                break;
            }
        }
        //====================================================================================
        // 2006.07.14 : Asirion
        // 팀바꾸기. 코드가 좀 더럽지만..옛날에 비하면 너무나도 아름다워졌다...오늘은 힘들어서 이만...
        if (GC_GMT_TEAM_FIGHT == SiKGCRoomManager()->GetGameModeType() && bChangeTeam == true && iIndex != -1 &&
            SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AUTOMATCH)
        {
            if (g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.bLive)
            {
                //언젠가 팀을 확장할 날이 오리라 보면서..코드를 조금 수정합니다.
                int iTeam = g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.iTeam;
                switch (iTeam)
                {
                case ETBlue:	iTeam = ETRed;		break;
                case ETRed:		iTeam = ETBlue;		break;
                default: //일로 들어오면 에러인데..사람이 몬스터 팀일리는 없잖아.
                    iTeam = !iTeam;
                    break;
                }
                g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.iTeam = iTeam;

                KP2P::GetInstance()->Send_ChangeRoomUserInfoReq(g_MyD3D->MyPlayer[iIndex]->m_kUserInfo,
                    KChangeRoomUserInfo::RUI_TEAM, iIndex);
                return;
            }
        }
        //====================================================================================
        // 2006.07.14 : Asirion
        // 강퇴
        if (bTerminateUser == true && iIndex != -1)
        {
            if (g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.bHost == false)
            {
                SiKGCRoomManager()->Kick_Player(iIndex);
                return;
            }
        }
        // 강제 시작 (좌측 컨트롤 버튼 + S)
        if (diks[DIK_LCONTROL] && diks[DIK_S] && !Old_diks[DIK_S]) // 강제 시작하기
        {
            bool bStartFlag = true;

            if (g_MyD3D->IsLocalPlayerObserver())
            {
                //옵저버는 대전 종류만 관전이 가능합니다.
                if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH ||
                    SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_GUILD_BATTLE ||
                    SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH ||
                    SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO ||
                    SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
                    SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
                {
                    bStartFlag = true;
                }
                else
                {
                    bStartFlag = false;
                }

            }


            if (bStartFlag)
            {
                Result_StartGame = 0;
                std::vector< std::pair<char, char> > vecPromotionCount;
                g_MyD3D->GetStatPromotionCount(vecPromotionCount);
                KP2P::GetInstance()->Send_StartGameReq(vecPromotionCount);
                WaitForServerAck(Result_StartGame, 0, 3000, 2);

                std::wstringstream stm;
                std::wstringstream strm;
                std::wstring strColorKey = L"#cff0000";
                std::wstring strColorKeyEnd = L"#cX";
                stm << strColorKey << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_START_GAME_ERROR) << strColorKeyEnd;

                // 051102. jseop. 서버에서 온 응답에 따른 처리.
                switch (Result_StartGame)
                {
                case 1:
                    break;
                case 2:
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetString(STR_ID_START_GAME_ERROR20),
                        g_pkStrLoader->GetString(STR_ID_START_GAME_ERROR21));
                    break;
                case 11:
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                        g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR5), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                    break;
                case 13:
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                        g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR1), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                    break;
                case 14:
                    strColorKey = L"#cffff00";
                    strm << strColorKey << L"\n"
                        << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3)
                        << g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime(SiKGCRoomManager()->GetGameMode())
                        << strColorKeyEnd;
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, stm.str(),
                        g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR7) + strm.str(), KGCUIScene::GC_MBOX_USE_EXIT, 0, 0, false, true);
                    break;
                case 15:
                    strColorKey = L"#cffff00";
                    strm << strColorKey << L"\n"
                        << g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR3)
                        << g_kGlobalValue.m_kHeroDungeonInfo.GetNextEntranceTime(SiKGCRoomManager()->GetGameMode())
                        << strColorKeyEnd;
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                        g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR6) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                    break;
                case 16:
                    strColorKey = L"#cffff00";
                    strm << strColorKey << L"\n"
                        << g_kGlobalValue.m_kHeroDungeonInfo.GetInitTime()
                        << strColorKeyEnd;
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, stm.str(),
                        g_pkStrLoader->GetString(STR_ID_HERO_DUNGEON_ENTERANCE_ERROR4) + strm.str(), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                    break;
                case 17:
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ERR_GAME_START_TIME), L"");
                    break;
                default:
                    std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_START_GAME_ACK, Result_StartGame);
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_START_GAME_ERROR30), strTemp);
                    break;
                }
            }
        }
    }
}

void CMyD3DApplication::MovePlayer(int Target, int Source)
{
    if(MyPlayer[Source]->m_kUserInfo.bLive && Source != Target )
	{
		*((PLAYER*)MyPlayer[Target]) = *((PLAYER*)MyPlayer[Source]);

		MyPlayer[Target]->RemovePlayerObject();
        MyPlayer[Target]->UpdatePlayerResource();
        SiKGCRoomManager()->GetRoomInfo().bOpenSlot[Target] = false;
		ClearSlot(Source);
    }
}

void CMyD3DApplication::Set_ShellMatrix(float Scale)
{
	// 쉐이딩 메트릭스 설정
	m_mat._11 = 0.5f*Scale* g_kCamera.m_fZoom; m_mat._12 = 0.0f; m_mat._13 = 0.0f; m_mat._14 = 0.0f; 
    m_mat._21 = 0.0f; m_mat._22 =-0.5f*Scale * g_kCamera.m_fZoom; m_mat._23 = 0.0f; m_mat._24 = 0.0f; 
    m_mat._31 = 0.0f; m_mat._32 = 0.0f; m_mat._33 = 1.0f; m_mat._34 = 0.0f; 
    m_mat._41 = 0.5f; m_mat._42 = 0.5f; m_mat._43 = 0.0f; m_mat._44 = 1.0f; 
	m_pd3dDevice->SetTransform( D3DTS_TEXTURE1, &m_mat );
}

void CMyD3DApplication::SetLight()
{
	//빛 설정
	D3DLIGHT9 Light;
	ZeroMemory( &Light, sizeof(D3DLIGHT9) );
    Light.Type        = D3DLIGHT_DIRECTIONAL;
    Light.Diffuse.r   = l_r;
    Light.Diffuse.g   = l_g;
    Light.Diffuse.b   = l_b;
    D3DXVec3Normalize( (D3DXVECTOR3*)&Light.Direction, &D3DXVECTOR3(-1.0f, -.7f, -.2f) );
    	
	m_pd3dDevice->SetLight(0,&Light);
	m_pd3dDevice->LightEnable(0, TRUE);
    g_pGCDeviceManager2->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_ARGB(0,AMBIENT,AMBIENT,AMBIENT));
}

void CMyD3DApplication::SendEquipItemList(IN PLAYER* pPlayer_, IN SUserInfo* pBeforeEquip_ /*= NULL*/, bool bOnGame)
{
    if (NULL == pPlayer_) {
        return;
    }

    static KEquipUser kEquip;
    kEquip.m_mapEquipInfo.clear();
    kEquip.m_strLogin = pPlayer_->m_kUserInfo.strLogin;
    kEquip.m_cCharType = pPlayer_->m_kUserInfo.GetCurrentChar().iCharType;

    bool bChange = false;
    std::vector<KSimpleItem> vecEquipList;

    if (NULL == pBeforeEquip_) {
        if (m_OriginalEquip.vecCharInfo != pPlayer_->m_kUserInfo.vecCharInfo ||
            m_OriginalEquip.cCharIndex != pPlayer_->m_kUserInfo.cCharIndex ||
            m_mapOriginalSwapWeapon != pPlayer_->m_mapSwapWeapon) {
            bChange = true;
        }
    }
    else {
        if (pBeforeEquip_->vecCharInfo != pPlayer_->m_kUserInfo.vecCharInfo ||
            pBeforeEquip_->cCharIndex != pPlayer_->m_kUserInfo.cCharIndex ||
            m_mapOriginalSwapWeapon != pPlayer_->m_mapSwapWeapon) {
            bChange = true;
        }
    }

    if (bChange == true)
    {
        SCharInfo& kCurrentChar = pPlayer_->m_kUserInfo.GetCurrentChar();
        KChangeEquipment kChangeEquipment;
        kEquip.m_mapEquipInfo.insert(std::make_pair(kCurrentChar.iCharType, kChangeEquipment));

#if defined( WEAPONCHANGE )
        if (bOnGame) {
            KItem* pKItem = NULL;
            if (pKItem = g_pItemMgr->m_kInventory.FindItemByItemUID(pPlayer_->GetSwapWeaponUID())) {
                kEquip.m_mapEquipInfo[kCurrentChar.iCharType].m_kChangeWeaponItem = g_kGlobalValue.ConvertKItemtoKEquipItemInfo(pKItem);
            }
        }
        else {
            std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mit = pPlayer_->m_mapSwapWeapon.find(kCurrentChar.iCharType);
            if (pPlayer_->m_mapSwapWeapon.end() != mit) {
                KItem* pKItem = NULL;
                if (pKItem = g_pItemMgr->m_kInventory.FindItemByItemUID((*mit).second.second.second)) {
                    kEquip.m_mapEquipInfo[kCurrentChar.iCharType].m_kChangeWeaponItem = g_kGlobalValue.ConvertKItemtoKEquipItemInfo(pKItem);
                }
            }
        }
#endif
        for (int j = 0; j < (int)kCurrentChar.vecItemInfo.size(); ++j)
        {
            KEquipItemInfo kItemInfo;
            kCurrentChar.vecItemInfo[j].Copy(kItemInfo);
            kEquip.m_mapEquipInfo[kCurrentChar.iCharType].m_vecEquipItems.push_back(kItemInfo);
        }

        if (NULL == pBeforeEquip_) {
            for (int i = 0; i < static_cast<int>(m_OriginalEquip.vecCharInfo.size()); ++i)
            {
                SCharInfo& sCharInfo = m_OriginalEquip.vecCharInfo[i];
                KChangeEquipment kChangeEquipment;
                kEquip.m_mapEquipInfo.insert(std::make_pair(sCharInfo.iCharType, kChangeEquipment));
                for (int j = 0; j < (int)sCharInfo.vecItemInfo.size(); ++j)
                {
                    KEquipItemInfo kItemInfo;
                    sCharInfo.vecItemInfo[j].Copy(kItemInfo);
                    kEquip.m_mapEquipInfo[sCharInfo.iCharType].m_vecReleaseItems.push_back(kItemInfo);
                }
            }
        }
        else {
            for (int i = 0; i < static_cast<int>(pBeforeEquip_->vecCharInfo.size()); ++i)
            {
                SCharInfo& sCharInfo = pBeforeEquip_->vecCharInfo[i];
                KChangeEquipment kChangeEquipment;
                kEquip.m_mapEquipInfo.insert(std::make_pair(sCharInfo.iCharType, kChangeEquipment));
                for (int j = 0; j < (int)sCharInfo.vecItemInfo.size(); ++j)
                {
                    KEquipItemInfo kItemInfo;
                    sCharInfo.vecItemInfo[j].Copy(kItemInfo);
                    kEquip.m_mapEquipInfo[sCharInfo.iCharType].m_vecReleaseItems.push_back(kItemInfo);
                }
            }
        }

        for (std::map<char, KChangeEquipment >::iterator mit = kEquip.m_mapEquipInfo.begin(); mit != kEquip.m_mapEquipInfo.end(); ++mit)
        {
            std::vector< KEquipItemInfo >& vecEquip = (mit->second).m_vecEquipItems;
            std::vector< KEquipItemInfo >& vecRelease = (mit->second).m_vecReleaseItems;
            std::vector< KEquipItemInfo >::iterator itrEquip = vecEquip.begin();
            for (; itrEquip != vecEquip.end(); ) {
                std::vector< KEquipItemInfo >::iterator itrRelease = vecRelease.begin();
                bool bErase = false;
                for (; itrRelease != vecRelease.end(); ) {

                    if ((*itrEquip) == (*itrRelease))
                    {
                        itrEquip = vecEquip.erase(itrEquip);
                        itrRelease = vecRelease.erase(itrRelease);
                        bErase = true;
                        break;
                    }
                    else {
                        ++itrRelease;
                    }
                }
                if (false == bErase) {
                    ++itrEquip;
                }
            }
            for (std::vector< KEquipItemInfo >::iterator itr = vecEquip.begin(); itr != vecEquip.end(); ) {
                if (g_pItemMgr->IsBaseItem(mit->first, itr->m_dwID / 10) == true)
                {
                    itr = vecEquip.erase(itr);
                }
                else {
                    ++itr;
                }
            }

            if (bOnGame) {
                for (std::vector< KEquipItemInfo >::iterator itr = vecEquip.begin(); itr != vecEquip.end(); ++itr) {
                    if (itr->m_dwUID == pPlayer_->GetSwapWeaponUID())
                    {
                        vecEquip.erase(itr);
                        break;
                    }
                }
            }

            for (std::vector< KEquipItemInfo >::iterator itr = vecRelease.begin(); itr != vecRelease.end(); ) {
                if (g_pItemMgr->IsBaseItem(mit->first, itr->m_dwID / 10) == true)
                {
                    itr = vecRelease.erase(itr);
                }
                else {
                    ++itr;
                }
            }

            if (bOnGame) {
                for (std::vector< KEquipItemInfo >::iterator itr = vecRelease.begin(); itr != vecRelease.end(); ++itr) {
                    if (itr->m_dwUID == pPlayer_->GetSwapWeaponUID())
                    {
                        vecRelease.erase(itr);
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < static_cast<int>(pPlayer_->m_kUserInfo.vecCharInfo.size()); ++i)
        {
            SCharInfo& sCharInfo = pPlayer_->m_kUserInfo.vecCharInfo[i];
            KChangeEquipment kChangeEquipment;
            std::map<char, KChangeEquipment >::iterator mit = kEquip.m_mapEquipInfo.insert(std::make_pair(sCharInfo.iCharType, kChangeEquipment)).first;;

            (mit->second).m_equipPetInfo = sCharInfo.kPetInfo;
            GCITEMUID dwPetUID = (mit->second).m_equipPetInfo.m_dwUID;
            DWORD dwPetID = (mit->second).m_equipPetInfo.m_dwID * 10;
            DWORD dwPetCostumeID = (mit->second).m_equipPetInfo.m_kCostume.m_dwID * 10;
            GCITEMUID dwPetCostumeUID = (mit->second).m_equipPetInfo.m_kCostume.m_dwUID;

            (mit->second).m_equipPetInfo.m_dwUID = dwPetUID;
            (mit->second).m_equipPetInfo.m_dwID = dwPetID;
            (mit->second).m_equipPetInfo.m_kCostume.m_dwID = dwPetCostumeID;
            (mit->second).m_equipPetInfo.m_kCostume.m_dwUID = dwPetCostumeUID;

            for (int j = 0; j < (int)(mit->second).m_equipPetInfo.m_vecEquipItem.size(); ++j)
            {
                (mit->second).m_equipPetInfo.m_vecEquipItem[j].m_dwUID = (mit->second).m_equipPetInfo.m_vecEquipItem[j].m_dwUID;
                (mit->second).m_equipPetInfo.m_vecEquipItem[j].m_dwID *= 10;
            }
        }

        if (!bOnGame)
            pPlayer_->UpdateGlobalUserInfo();

        KEVENT_CHANGE_LOOK_EQUIP_REQ kCharCoordi;

        for (int i = 0; i < static_cast<int>(pPlayer_->m_kUserInfo.vecCharInfo.size()); ++i)
        {
            SCharInfo& sCharInfo = pPlayer_->m_kUserInfo.vecCharInfo[i];
            for (int j = 0; j < (int)sCharInfo.vecLookInfo.size(); ++j)
            {
                KCoordiDiffInfo& kCoordiDiff = kCharCoordi.m_mapChangeLookEquip[sCharInfo.iCharType];
                kCoordiDiff.m_setEquip.insert(sCharInfo.vecLookInfo[j].dwItemUID);
            }
        }

        if (NULL == pBeforeEquip_) {
            for (int i = 0; i < static_cast<int>(m_OriginalEquip.vecCharInfo.size()); ++i)
            {
                SCharInfo& sCharInfo = m_OriginalEquip.vecCharInfo[i];
                for (int j = 0; j < (int)sCharInfo.vecLookInfo.size(); ++j)
                {
                    KCoordiDiffInfo& kCoordiDiff = kCharCoordi.m_mapChangeLookEquip[sCharInfo.iCharType];
                    kCoordiDiff.m_setRelease.insert(sCharInfo.vecLookInfo[j].dwItemUID);
                }
            }
        }
        else {
            for (int i = 0; i < static_cast<int>(pBeforeEquip_->vecCharInfo.size()); ++i)
            {
                SCharInfo& sCharInfo = pBeforeEquip_->vecCharInfo[i];
                for (int j = 0; j < (int)sCharInfo.vecLookInfo.size(); ++j)
                {
                    KCoordiDiffInfo& kCoordiDiff = kCharCoordi.m_mapChangeLookEquip[sCharInfo.iCharType];
                    kCoordiDiff.m_setRelease.insert(sCharInfo.vecLookInfo[j].dwItemUID);
                }
            }
        }

        std::map<char, KCoordiDiffInfo>::iterator mitPacket;
        for (mitPacket = kCharCoordi.m_mapChangeLookEquip.begin(); mitPacket != kCharCoordi.m_mapChangeLookEquip.end(); ++mitPacket) {
            std::set<GCITEMUID> setAfter;
            setAfter.swap(mitPacket->second.m_setEquip);
            std::set<GCITEMUID> setBefore;
            setBefore.swap(mitPacket->second.m_setRelease);

            std::set<GCITEMUID>::iterator sit;
            for (sit = setAfter.begin(); sit != setAfter.end(); ++sit) {
                if (setBefore.find(*sit) == setBefore.end()) {
                    mitPacket->second.m_setEquip.insert(*sit);
                }
            }

            for (sit = setBefore.begin(); sit != setBefore.end(); ++sit) {
                if (setAfter.find(*sit) == setAfter.end()) {
                    mitPacket->second.m_setRelease.insert(*sit);
                }
            }
        }
        Result_CoordiEquip = INT_MAX;
        KP2P::GetInstance()->Send_ChangeLookEqipReq(kCharCoordi);
        WaitForServerAck(Result_CoordiEquip, INT_MAX, 1000, -1);

        Result_ItemEquip = INT_MAX;
        KP2P::GetInstance()->Send_EquipItemReq(kEquip);
        WaitForServerAck(Result_ItemEquip, INT_MAX, 60000, -1);

    }

    bChange = false;

    if (!bOnGame) {
        pPlayer_->UpdateGlobalUserInfo();
        pPlayer_->UpdateTempPlayerUserInfo();
    }
}