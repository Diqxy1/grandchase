
#include "stdafx.h"
#include "Pet.h"
#include "Stage.h"
#include "GCCameraOrtho.h"
#include "Replay.h"
#include "DamageManager.h"
#include "gcui/GCPetWnd.h"
#include "gcui/GCGameOverlayUI.h"
#include "gcui/GCHeadupDisplay.h"
#include "GCRenderManager.h"
#include "MapSystem/KGCLayerPart.h"
#include "gcui/GCMyHeadup.h"
#include "./Square/GCSquare.h"
#include "./Square/GCSquarePeople.h"
#include "QuestGameManager.h"
#include "GCAchieveManager.h"
#include "GCPetMagicStone.h"

using namespace std;

#define PETDELAY 12
#define MAXPETMP 1.0f

KPet::KPet()
    : m_pOwner(NULL)
    , m_fAtkRatio(1.0f)
    , m_dwSkillGoodsID(0)
{
    m_pObject = NULL;
    clear();
    m_iPlayerNumber = -1;
    m_mapTagPet.clear();

    m_bRenderShadow = true;
    m_bRenderName = true;
}

KPet::~KPet()
{
    m_mParticlePos.clear();
    m_pParticleEventSequence.clear();
    m_pParticleEventSequence2.clear();
    m_mapTagPet.clear();
}

KPet& KPet::operator =(const KPet& right_)
{
    lstrcpy(m_strPetName, right_.m_strPetName);
    m_iPlayerNumber = right_.m_iPlayerNumber;
    m_iPetID = right_.m_iPetID;
    m_iLevel = right_.m_iLevel;
    m_iEvolutionLevel = right_.m_iEvolutionLevel;
    m_iResultFrame = right_.m_iResultFrame;
    usMotion = right_.usMotion;
    bIsRight = right_.bIsRight;
    cFrame = right_.cFrame;

    return *this;
}
void KPet::clear()
{
    EndAfterImage();
    SAFE_REMOVE_DELETE(m_pObject);

    KRenderData::Init();

    usMotion = EGCPETMOTION::INVALID;
    m_iPetID = -1;
    m_iEvolutionLevel = 0;
    m_iLevel = 0;

    m_iResultFrame = 0;
    m_fScale = 1.0f;

    m_bIsJump = false;
    m_bIsTeleport = false;

    m_iRenderDelay = 15;

    m_fIncMP = 0.0f;
    m_fCurrentMP = 0.0f;

    m_bEnableUseSkill = false;

    m_vSlotPos.x = 0;
    m_vSlotPos.y = 0;

    m_bIsWin = false;
    m_bIsLose = false;

    //m_bStart            = false;

    m_iRemainSpecial = 0;

    m_bPetWaitForTag = false;

    m_bExistMirrorMotion = false;

    m_mapMotion.clear();
    ClearParticleEffect();

    m_bRenderShadow = true;
    m_bRenderName = true;
}

void KPet::BindLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

        lua_tinker::class_add<KPet>(L, "KPet");
    lua_tinker::class_mem<KPet>(L, "m_iPetID", &KPet::m_iPetID);
    lua_tinker::class_mem<KPet>(L, "m_iEvolutionLevel", &KPet::m_iEvolutionLevel);
    lua_tinker::class_mem<KPet>(L, "IsRight", &KPet::bIsRight);
    lua_tinker::class_mem<KPet>(L, "iFrame", &KPet::cFrame);
    lua_tinker::class_mem<KPet>(L, "iMotion", &KPet::usMotion);
    lua_tinker::class_mem<KPet>(L, "bIsContact", &KPet::m_bIsContact);
    lua_tinker::class_mem<KPet>(L, "bIsJump", &KPet::m_bIsJump);
    lua_tinker::class_mem<KPet>(L, "bIsTeleport", &KPet::m_bIsTeleport);
    lua_tinker::class_mem<KPet>(L, "iRenderDelay", &KPet::m_iRenderDelay);
    lua_tinker::class_mem<KPet>(L, "fIncMP", &KPet::m_fIncMP);
    lua_tinker::class_mem<KPet>(L, "bIsWin", &KPet::m_bIsWin);
    lua_tinker::class_mem<KPet>(L, "bIsLose", &KPet::m_bIsLose);
    lua_tinker::class_mem<KPet>(L, "fMotionX", &KPet::m_fMotionX);
    lua_tinker::class_def<KPet>(L, "AddDamage", &KPet::AddDamage);
    lua_tinker::class_def<KPet>(L, "AddDamageWithAngle", &KPet::AddDamageWithAngle);

    lua_tinker::class_def<KPet>(L, "AddActionParticle", &KPet::AddActionParticle);
    lua_tinker::class_def<KPet>(L, "GetPos", &KPet::GetPos);
    lua_tinker::class_def<KPet>(L, "GetDelayPos", &KPet::GetDelayPos);
    lua_tinker::class_def<KPet>(L, "GetPrePos", &KPet::GetPrePos);
    lua_tinker::class_def<KPet>(L, "PlayerTrace", &KPet::PlayerTrace);
    lua_tinker::class_def<KPet>(L, "SetPetMotion", &KPet::SetPetMotion);
    lua_tinker::class_def<KPet>(L, "GetNumFrame", &KPet::GetNumFrame);
    lua_tinker::class_def<KPet>(L, "FloatAbs", &KPet::FloatAbs);
    lua_tinker::class_def<KPet>(L, "AddParticle", &KPet::AddParticle);
    lua_tinker::class_def<KPet>(L, "AddParticleWithTrace", &KPet::AddParticleWithTrace);
    lua_tinker::class_def<KPet>(L, "AddParticleNoDirectionPosWithTrace", &KPet::AddParticleNoDirectionPosWithTrace);
    lua_tinker::class_def<KPet>(L, "AddParticleXOffset", &KPet::AddParticleXOffset);
    lua_tinker::class_def<KPet>(L, "SetIncMPPoint", &KPet::SetIncMP);
    lua_tinker::class_def<KPet>(L, "PlaySound", &KPet::PlaySound);
    lua_tinker::class_def<KPet>(L, "StartAfterImage", &KPet::StartAfterImage);
    lua_tinker::class_def<KPet>(L, "EndAfterImage", &KPet::EndAfterImage);
    lua_tinker::class_def<KPet>(L, "AddBuff", &KPet::AddBuff);
    lua_tinker::class_def<KPet>(L, "OnReverseGravity", &KPet::OnReverseGravity);
    lua_tinker::class_def<KPet>(L, "LoadPetSingleMotion", &KPet::LoadPetSingleMotion);

    lua_tinker::class_def<KPet>(L, "ToggleMesh", &KPet::ToggleMesh);
    lua_tinker::class_def<KPet>(L, "IsRenderOnPet", &KPet::IsRenderOnPet);
    lua_tinker::class_def<KPet>(L, "TogglePetRender", &KPet::TogglePetRender);

    if (GCFUNC::LoadLuaScript(KGCLuabinder::getInstance(), "PetAction.lua") == false)
    {
        MessageBoxA(NULL, "PetAction.lua Load Failed!", NULL, MB_OK);
        exit(0);
    }

    if (GCFUNC::LoadLuaScript(KGCLuabinder::getInstance(), "PetSkill.lua") == false)
    {
        MessageBoxA(NULL, "PetSkill.lua Load Failed!", NULL, MB_OK);
        exit(0);
    }
}

void KPet::StartPet(int iPlayerNumber, int iPetType, int iLevel, int iEvolutionLevel,
    int iLocation, std::wstring strPetName, bool bAddToRenderMgr_/* = false*/, DWORD dwCostumeID /*= 0*/, bool bUseShopTexture_ /*= false*/)
{
    EndPet();
    SetBaseInformation(iPetType, iLevel, iEvolutionLevel, strPetName);
    CreateObject(bAddToRenderMgr_);
    LoadPetModel(bUseShopTexture_, dwCostumeID);
    LoadPetMotion(iLocation, iPetType, iEvolutionLevel);
    SetOwnerInfo(iPlayerNumber);

    SetCostumeID(dwCostumeID);
    LoadPetSound(m_iPetID, m_iEvolutionLevel);

    if (g_MyD3D->IsPlayerIndex(iPlayerNumber)) {
        KPetInfo& kPet = g_MyD3D->MyPlayer[iPlayerNumber]->GetCurrentChar().kPetInfo;
        for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++)
        {
            GCItem* pkItem = g_pItemMgr->GetItemData(kPet.m_vecEquipItem[i].m_dwID);
            if (pkItem == NULL)
                continue;


            if (g_MyD3D->MyPlayer[iPlayerNumber]->IsLocalPlayer()) {

                if (g_pItemMgr->FindInventory(kPet.m_vecEquipItem[i].m_dwUID, &pkItem) == false)
                    continue;
            }

            if (pkItem->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_SKILL)
            {
                // 현재 펫이 장착하고있는 스킬의 GoodsID를 저장해 둔다
                m_dwSkillGoodsID = pkItem->dwGoodsID;
            }
        }
    }


    LoadPetActionMotion(iPetType, iEvolutionLevel, m_dwSkillGoodsID);

    KGCPC_MEMO("Start Pet : " << KncUtil::toNarrowString(strPetName));
}

void KPet::InitBeforeGameStart()
{
    if (m_iPlayerNumber < 0)
        return;

    if (m_iPetID == EGCPETID::INVALID)
        return;

    //cFrame = 0;
    usMotion = EGCPETMOTION::INVALID;
    KRenderData::Init();
    m_iResultFrame = 0;

    m_bIsTeleport = false;

    m_bIsWin = false;
    m_bIsLose = false;
    m_bStart = false;

    m_fMotionX = 0.0f;

    //스킬을 확인해서 그 스킬에 해당하는 MP회복량을 할당합니다.
    GetIncMPFromLua();

    if (KPet::IsPetAvailableState() && g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer())
    {
        CalcRemainSpecial(g_MyD3D->MyPlayer[m_iPlayerNumber]->GetCurrentChar().kPetInfo);
    }
}

void KPet::FrameInfoProc(UINT uiMotion)
{
    if (KPet::IsPetAvailableState() && g_MyD3D->MyPlayer[m_iPlayerNumber]->IsSkipFrameTime() &&
        uiMotion != EGCPETMOTION::ATTACK && uiMotion != EGCPETMOTION::MAGIC)
        return;

    KGCPC_BEGIN("Pet_Action");
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L);
    lua_checkstack(L, 10);

    lua_getglobal(L, "Pet_FrameMove");
    if (lua_isfunction(L, -1)) {
        lua_pushnumber(L, m_iPlayerNumber);
        lua_pushnumber(L, cFrame);
        lua_pushnumber(L, m_iPetID);
        lua_pushnumber(L, m_iEvolutionLevel);
        lua_pushnumber(L, (int)uiMotion);
        lua_pushnumber(L, m_dwSkillGoodsID);

        if (lua_pcall(L, 6, 0, 0) != 0) {
            lua_pop(L, 1);
        }
    }
    else {
        lua_pop(L, 1);
    }

    KGCPC_END();
}

void KPet::FrameInfoProc_Common(UINT uiMotion)
{

    if ((uiMotion != EGCPETMOTION::TELEPORT_READY) && (uiMotion != EGCPETMOTION::ATTACK) && (uiMotion != EGCPETMOTION::MAGIC))
    {
        PlayerTrace();

        if ((uiMotion != EGCPETMOTION::WIN) && (uiMotion != EGCPETMOTION::LOSE) && (uiMotion != EGCPETMOTION::TELEPORT_ARRIVE) && (uiMotion != EGCPETMOTION::START))
        {
            if ((m_bIsContact == false))
            {
                if ((m_bIsJump == false))
                {
                    SetPetMotion(EGCPETMOTION::JUMP_READY);
                }
                m_bIsJump = true;

            }
            else if ((uiMotion != EGCPETMOTION::JUMP_LAND))
            {

                if ((uiMotion != EGCPETMOTION::START) && (ASMfabs(vPos.x - m_avPos[0].x) < 0.0025f))
                {
                    if ((m_bIsJump == true))
                    {
                        SetPetMotion(EGCPETMOTION::JUMP_LAND);
                    }
                    else if ((uiMotion == EGCPETMOTION::MOVE))
                    {
                        SetPetMotion(EGCPETMOTION::MOVE_END);
                    }
                    else if ((uiMotion != EGCPETMOTION::MOVE_END))
                    {
                        if ((uiMotion != EGCPETMOTION::WAIT))
                        {
                            SetPetMotion(EGCPETMOTION::WAIT);
                        }

                        if (true == m_bIsWin)
                        {
                            SetPetMotion(EGCPETMOTION::WIN);
                            m_bIsWin = false;
                        }
                        else if ((m_bIsLose))
                        {
                            SetPetMotion(EGCPETMOTION::LOSE);
                            m_bIsLose = false;
                        }
                    }

                    m_bIsJump = false;

                }
                else
                {
                    if ((m_bIsJump == true))
                    {
                        SetPetMotion(EGCPETMOTION::JUMP_LAND);
                    }
                    else if ((uiMotion != EGCPETMOTION::MOVE))
                    {
                        SetPetMotion(EGCPETMOTION::MOVE);
                    }

                    m_bIsJump = false;
                }
            }

            if (((uiMotion == EGCPETMOTION::JUMP_UP) || ((uiMotion == EGCPETMOTION::JUMP_READY) && (cFrame >= 5))) && (m_vPrePos.y > vPos.y))
            {
                SetPetMotion(EGCPETMOTION::JUMP_DOWN);
            }

            if ((uiMotion == EGCPETMOTION::JUMP_DOWN && m_vPrePos.y < vPos.y))
            {
                SetPetMotion(EGCPETMOTION::JUMP_UP);
            }
        }
    }

    if ((m_iRenderDelay == 0))
    {
        // 프레임 조절
        int iMaxFrame = GetNumFrame(uiMotion);

        // 모션의 마지막 프레임에서 멈춘다
        if (((uiMotion == EGCPETMOTION::JUMP_UP) || (uiMotion == EGCPETMOTION::JUMP_DOWN) || (uiMotion == EGCPETMOTION::WIN) || (uiMotion == EGCPETMOTION::LOSE)) && (cFrame >= iMaxFrame - 1))
        {
            if ((uiMotion == EGCPETMOTION::WIN) || (uiMotion == EGCPETMOTION::LOSE))
            {
                m_fMotionX = 0;
            }
            cFrame = iMaxFrame - 1;
        }
        else
        {
            if (((uiMotion == EGCPETMOTION::START) || (uiMotion == EGCPETMOTION::JUMP_LAND) || (uiMotion == EGCPETMOTION::MOVE_END)) && (cFrame >= iMaxFrame - 1))
            {
                SetPetMotion(EGCPETMOTION::WAIT);
                // 공격 후에는 순간이동 한다
            }
            else if (((uiMotion == EGCPETMOTION::ATTACK) || (uiMotion == EGCPETMOTION::MAGIC)) && (cFrame >= iMaxFrame - 1))
            {
                SetPetMotion(EGCPETMOTION::TELEPORT_READY);
            }
            else if ((uiMotion == EGCPETMOTION::TELEPORT_READY) && (cFrame >= iMaxFrame - 1))
            {
                SetPetMotion(EGCPETMOTION::TELEPORT_ARRIVE);
            }
            else if ((uiMotion == EGCPETMOTION::TELEPORT_ARRIVE) && (cFrame >= iMaxFrame - 1))
            {
                SetPetMotion(EGCPETMOTION::WAIT);
                m_bIsTeleport = false;
                m_bIsJump = false;
            }
            else if ((uiMotion == EGCPETMOTION::JUMP_READY) && (cFrame >= iMaxFrame - 1))
            {

                if ((m_vPrePos.y > vPos.y))
                {
                    SetPetMotion(EGCPETMOTION::JUMP_DOWN);
                }
                else
                {
                    SetPetMotion(EGCPETMOTION::JUMP_UP);
                }
            }
            else if (uiMotion == EGCPETMOTION::WAIT)
            {
                // 스타트 모션좀 보쟈 ㅠ 
                if (false == m_bStart && g_pkQuestManager->m_iCurStage == 0)
                {
                    SetPetMotion(EGCPETMOTION::START);
                    m_bStart = true;
                    EnableGlyphEffect(false);
                }
                else
                {
                    if (iMaxFrame == 0)
                        return;

                    cFrame = (++cFrame) % iMaxFrame;
                }
            }
            else
            {
                if (iMaxFrame == 0)
                    return;

                cFrame = (++cFrame) % iMaxFrame;
            }
        }
    }
    else
    {
        --m_iRenderDelay;
    }
}

void KPet::EndPet()
{
    EndAfterImage();
    SAFE_REMOVE_DELETE(m_pObject);

    std::map<int, GCDeviceMotionFRM*>::iterator mit;
    for (mit = m_mapMotion.begin(); mit != m_mapMotion.end(); ++mit) {
        SAFE_RELEASE(mit->second);
    }
    m_mapMotion.clear();

    ClearParticleEffect();

    m_iPlayerNumber = -1;
    m_iPetID = -1;
    m_fAtkRatio = 1.0f;

    // 초기화
    KRenderData::Init();

    m_bIsJump = false;
    m_bIsTeleport = false;

    m_iRenderDelay = 15;

    m_fCurrentMP = m_fCurrentMP + (m_fCurrentMP / 2);

    if (m_fCurrentMP > 1.0f)
        m_fCurrentMP = 1.0f;

    m_bEnableUseSkill = false;
    m_bPetWaitForTag = false;

    SAFE_DELETE(m_pOwner);
    KGCPC_MEMO("End Pet : " << m_strPetName);
}

void KPet::UpdateResource()
{
    if (m_iPetID == -1)
    {
        EndPet();
        return;
    }
    StartPet(m_iPlayerNumber, m_iPetID, m_iLevel, m_iEvolutionLevel, EGCPETLOCATION::ROOM, m_strPetName, false, 0, true);
};

void KPet::FrameMove()
{

    if (m_pObject == NULL)
        return;

    if (g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE)
    {
        if (false == g_MyD3D->IsPlayerIndex(m_iPlayerNumber))
            return;
    }

    if (KPet::IsPetAvailableState())
    {
        if (!g_MyD3D->MyPlayer[m_iPlayerNumber]->m_kUserInfo.bLive ||
            g_MyD3D->MyPlayer[m_iPlayerNumber]->m_cLife == 0 || m_iPetID == -1 ||
            g_MyD3D->MyPlayer[m_iPlayerNumber]->CheckInPortal())
        {
            if (m_pObject)
            {
                m_pObject->SetRender(false);
            }
            return;
        }
    }

    FrameMove_Motion();

    FrameInfoProc(usMotion);
    FrameInfoProc_Common(usMotion);
    FrameMoveEffect();
    m_pObject->SetRender(m_bPetWaitForTag);

    float fDungeonRate = 1.0f;

    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST81 )
        fDungeonRate = 0.5f;
    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST92 )
        fDungeonRate = 0.5f;
    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST99 )
        fDungeonRate = 0.5f;
    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST100 )
        fDungeonRate = 0.5f;
    if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST102)
        fDungeonRate = 0.5f;

    auto fPetGlyphMPRecovery = SiGCPetMagicStone()->GetGlyphEffectAmmount( PET_MAGIC_STONE_EFFECT_TYPE::PMSET_PET_MP_RECOVERY );
    if ( fPetGlyphMPRecovery > 0.f && m_iPlayerNumber == g_MyD3D->Get_MyPlayer() )
        m_fCurrentMP += m_fIncMP * ( fDungeonRate + fPetGlyphMPRecovery );
    else
        m_fCurrentMP += m_fIncMP * fDungeonRate;

    if ( MAXPETMP < m_fCurrentMP )
        m_fCurrentMP = MAXPETMP;
}

void KPet::FrameMoveEffect()
{
    if (!m_bPetWaitForTag)
        return;

    if (m_iRenderDelay == 0 && !IsHallow())
    {
        std::map<std::string, CParticleEventSeqPTR>::iterator iter;

        for (iter = m_pParticleEventSequence.begin(); iter != m_pParticleEventSequence.end(); iter++)
        {
            if (iter->second == NULL)
                continue;

            iter->second->SetShow(true);

            if (bIsRight)
                iter->second->SetPosition(vPos.x - 1.0f + m_mParticlePos[iter->first].first.x, vPos.y - 0.5f + m_mParticlePos[iter->first].first.y, 0.5f);
            else
                iter->second->SetPosition(vPos.x - 1.0f + m_mParticlePos[iter->first].second.x, vPos.y - 0.5f + m_mParticlePos[iter->first].second.y, 0.5f);
        }

        int i;
        for (iter = m_pParticleEventSequence2.begin(), i = 0; iter != m_pParticleEventSequence2.end(); iter++, i++)
        {
            if (iter->second == NULL)
                continue;

            iter->second->SetShow(true);

            D3DXMATRIX matBone;

            m_pObject->SetMotionMatrix(true, NULL, false, true);
            m_pObject->GetAngleBone(iter->second.m_iBone, &matBone);

            matBone *= *(m_pObject->GetWorldMat());

            const SPetTemplate& sPetTemplate = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel);
            if (i < static_cast<int>(m_vBoneTracePos.size()) && i < static_cast<int> (sPetTemplate.vecBoneEffect.size()))
            {
                m_vBoneTracePos[i].x = matBone._41 + sPetTemplate.vecBoneEffect[i].m_vOffsetPos->x;
                m_vBoneTracePos[i].y = matBone._42 + sPetTemplate.vecBoneEffect[i].m_vOffsetPos->y;
                //                BOOST_FOREACH( const SPetBoneEffectInfo& val, sPetTemplate.vecBoneEffect ) {
                                    //m_vBoneTracePos[i].x = matBone._41 + val.m_vOffsetPos[i].x;
                                    //m_vBoneTracePos[i].y = matBone._42 + val.m_vOffsetPos[i].y;
                                //}
            }
        }


        // 펫은 기본이 플레이어 기반으로 움직여서 필살기 시전 시 등에서 트레이스가 되지 않는 문제가 있어
        // 센터에 따라 다닐 수 있도록 0번 본의 위치만 구해준다.
        D3DXMATRIX matBone;
        m_pObject->SetMotionMatrix(true, NULL, false, true);
        m_pObject->GetAngleBone(0, &matBone);
        matBone *= *(m_pObject->GetWorldMat());
        m_vTraceCenterPos.x = matBone._41;
        m_vTraceCenterPos.y = matBone._42;
    }
    else
    {
        std::map<std::string, CParticleEventSeqPTR>::iterator iter;

        for (iter = m_pParticleEventSequence.begin(); iter != m_pParticleEventSequence.end(); iter++)
        {
            if (iter->second)
                iter->second->SetShow(false);
        }


        for (iter = m_pParticleEventSequence2.begin(); iter != m_pParticleEventSequence2.end(); iter++)
        {
            if (iter->second)
                iter->second->SetShow(false);
        }
    }
     
    //여기서 부터는 
    switch (m_iPetID)
    {
    case EGCPETID::BLUEFAIRY:
        // 페어리 기 모으는 동작
        if (usMotion == EGCPETMOTION::ATTACK && cFrame == 10)
        {
            CParticleEventSeqPTR pTemp;
            AddParticle("pet-fairy_Icearrow", 0, 0);
            pTemp = AddParticle("pet-fairy_Icearrow1", 0, 0);
            pTemp->SetBlackHolePosition(&m_vTracePos);
        }
        break;

    default:
        break;
    }
}

void KPet::PlayerTrace()
{
    if (!m_pOwner)
        return;


    UpdateMasterInfo();
    // 나 자신일때는 MyLatency의 값을 참조한다
    // 리플레이 때는 밑에 g_MyD3D->MyPlayer[m_iMyNumber] 를 참조
    if ((g_MyD3D->MyReplay->m_iCurrState != RP_VIEW) &&
        (KPet::IsPetAvailableState() && m_iPlayerNumber == g_MyD3D->GetMyPlayer()->m_iPlayerIndex))
    {
        if (
            ((usMotion == EGCPETMOTION::TELEPORT_ARRIVE && m_bIsTeleport == false) || usMotion == EGCPETMOTION::INVALID) &&
            (false == KPet::IsPetAvailableState() || g_MyD3D->MyPlayer[m_iPlayerNumber]->GetPlayerState() != PS_DEAD))
        {
            if (usMotion == EGCPETMOTION::TELEPORT_ARRIVE)
            {
                m_iRenderDelay = 15;
                m_bIsTeleport = true;
            }

            if (usMotion == EGCPETMOTION::INVALID)
            {
                SetPetMotion(EGCPETMOTION::START);
                m_iRenderDelay = 20;
            }

            for (int i = 0; i < PETDELAY; i++)
            {
                if (m_pOwner->OwerPlayer.bIsRight)
                    m_avPos[i].x = m_pOwner->OwerPlayer.vPos.x - m_fDistX;
                else
                    m_avPos[i].x = m_pOwner->OwerPlayer.vPos.x + m_fDistX;
                m_avPos[i].y = m_pOwner->OwerPlayer.vPos.y;

                m_abIsRight[i] = m_pOwner->OwerPlayer.bIsRight;
                m_abContact[i] = m_pOwner->bIsContact;

                CheckPetContact(i, true);
            }
        }

        m_vPrePos = (D3DXVECTOR2)vPos;
        vPos = (D3DXVECTOR3)m_avPos[0];
        SETVECTOR3(m_vTracePos, vPos.x - 1.0f, vPos.y - 0.42f + m_fDistY, 0.5f);
        bIsRight = m_abIsRight[0];
        m_bIsContact = m_abContact[0];

        if (m_iRenderDelay == 0)
            vPos.x += m_fMotionX;

        for (int i = 0; i + 1 < PETDELAY; i++)
        {
            m_avPos[i] = m_avPos[i + 1];
            m_abIsRight[i] = m_abIsRight[i + 1];
            m_abContact[i] = m_abContact[i + 1];
        }

        if (m_pOwner->OwerPlayer.bIsRight)
            m_avPos[PETDELAY - 1].x = m_pOwner->OwerPlayer.vPos.x - m_fDistX;
        else
            m_avPos[PETDELAY - 1].x = m_pOwner->OwerPlayer.vPos.x + m_fDistX;
        m_avPos[PETDELAY - 1].y = m_pOwner->OwerPlayer.vPos.y;

        m_abIsRight[PETDELAY - 1] = m_pOwner->OwerPlayer.bIsRight;
        m_abContact[PETDELAY - 1] = m_pOwner->bIsContact;

        CheckPetContact(PETDELAY - 1, true);
    }
    // 내가 아닐때 g_MyD3D->MyPlayer[m_iMyNumber] 를 참조한다 + 광장
    else
    {
        if (((usMotion == EGCPETMOTION::TELEPORT_ARRIVE && m_bIsTeleport == false) || usMotion == EGCPETMOTION::INVALID) &&
            (false == KPet::IsPetAvailableState() || g_MyD3D->MyPlayer[m_iPlayerNumber]->GetPlayerState() != PS_DEAD))
        {
            if (usMotion == EGCPETMOTION::TELEPORT_ARRIVE)
            {
                m_iRenderDelay = 15;
                m_bIsTeleport = true;
            }

            if (usMotion == EGCPETMOTION::INVALID)
            {
                SetPetMotion(EGCPETMOTION::START);
                m_iRenderDelay = 20;
            }

            if (KPet::IsPetAvailableState())
            {
                for (int i = 0; i < PETDELAY; i++)
                {
                    if (m_pOwner->OwerPlayer.bIsRight)
                        m_avPos[i].x = m_pOwner->OwerPlayer.vPos.x - m_fDistX;
                    else
                        m_avPos[i].x = m_pOwner->OwerPlayer.vPos.x + m_fDistX;
                    m_avPos[i].y = m_pOwner->OwerPlayer.vPos.y;

                    m_abIsRight[i] = m_pOwner->OwerPlayer.bIsRight;
                    m_abContact[i] = m_pOwner->bIsContact;

                    CheckPetContact(i, false);
                }
            }
            else
            {
                for (int i = 0; i < PETDELAY; i++)
                {
                    if (m_pOwner->pOwnerRenderData->bIsRight)
                        m_avPos[i].x = m_pOwner->pOwnerRenderData->vPos.x - m_fDistX;
                    else
                        m_avPos[i].x = m_pOwner->pOwnerRenderData->vPos.x + m_fDistX;
                    m_avPos[i].y = m_pOwner->pOwnerRenderData->vPos.y;

                    m_abIsRight[i] = m_pOwner->pOwnerRenderData->bIsRight;
                    m_abContact[i] = m_pOwner->bIsContact;

                    CheckPetContact(i, false);
                }

            }

        }

        m_vPrePos = (D3DXVECTOR2)vPos;
        vPos = (D3DXVECTOR3)m_avPos[0];
        SETVECTOR3(m_vTracePos, vPos.x - 1.0f, vPos.y - 0.42f + m_fDistY, 0.5f);
        bIsRight = m_abIsRight[0];
        m_bIsContact = m_abContact[0];

        if (m_iRenderDelay == 0)
            vPos.x += m_fMotionX;

        for (int i = 0; i + 1 < PETDELAY; i++)
        {
            m_avPos[i] = m_avPos[i + 1];
            m_abIsRight[i] = m_abIsRight[i + 1];
            m_abContact[i] = m_abContact[i + 1];
        }

        if (m_pOwner)
        {
            if (KPet::IsPetAvailableState())
            {
                if (m_pOwner->OwerPlayer.bIsRight)
                    m_avPos[PETDELAY - 1].x = m_pOwner->OwerPlayer.vPos.x - m_fDistX;
                else
                    m_avPos[PETDELAY - 1].x = m_pOwner->OwerPlayer.vPos.x + m_fDistX;
                m_avPos[PETDELAY - 1].y = m_pOwner->OwerPlayer.vPos.y;

                m_abIsRight[PETDELAY - 1] = m_pOwner->OwerPlayer.bIsRight;
                m_abContact[PETDELAY - 1] = m_pOwner->bIsContact;
            }
            else
            {
                if (m_pOwner->pOwnerRenderData->bIsRight)
                    m_avPos[PETDELAY - 1].x = m_pOwner->pOwnerRenderData->vPos.x - m_fDistX;
                else
                    m_avPos[PETDELAY - 1].x = m_pOwner->pOwnerRenderData->vPos.x + m_fDistX;
                m_avPos[PETDELAY - 1].y = m_pOwner->pOwnerRenderData->vPos.y;

                m_abIsRight[PETDELAY - 1] = m_pOwner->pOwnerRenderData->bIsRight;
                m_abContact[PETDELAY - 1] = m_pOwner->bIsContact;
            }
        }

        CheckPetContact(PETDELAY - 1, false);
    }

    if (g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE)
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[m_iPlayerNumber];
        static DWORD ElapsedTime = ::timeGetTime();

        //====================================================================================
        // 2009.02.18 : DD
        // Comment : 펫이 wait상태이고 미러모션들이 존재할 때만 주인공 행동에 따른 펫모션을 
        //			 처리한다. m_bExistMirrorMotion은 펫 템플릿에서 받아온다.
        if (usMotion == EGCPETMOTION::WAIT && g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).bExistMirrorMotion)
        {
            // 공격 상태
            if (pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1 ||
                pPlayer->uiMotion == pPlayer->GetCurFormTemplate().CRITICAL_ATK ||
                pPlayer->uiMotion == pPlayer->GetCurFormTemplate().DOUBLE_ATK)
            {
                SetPetMotion(EGCPETMOTION::ATTACK_MIRROR);
            }
            // wait에서 일정시간~
            if (pPlayer->uiMotion == pPlayer->GetWaitMotion())
            {
                if (ElapsedTime + 1000 * 6/*초*/ <= ::timeGetTime())
                {
                    SetPetMotion(EGCPETMOTION::BORING);
                    ElapsedTime = ::timeGetTime();
                }
            }

            // 도발~
            if (pPlayer->uiMotion == pPlayer->GetJokeMotion())
            {
                SetPetMotion(EGCPETMOTION::JOKE_MIRROR);
            }

            // 피격 상태~
            if (pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_SMALLDAMAGE) ||
                pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_BIGDAMAGE) ||
                pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
                pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP))
            {
                SetPetMotion(EGCPETMOTION::DAMAGE_MIRROR);
            }
        }
        else
        {
            ElapsedTime = ::timeGetTime();
        }
    }
    //////////////////////////////////////////////////////////////////////////

    // 대쉬나 기타 갑자기 빨리 움직이는 경우에 펫도 함께 빨라졌다 느려졌다 하기 때문에
    // 어색합니다 보정해 주는 부분
    float fDistX = m_avPos[PETDELAY - 1].x - m_avPos[0].x;
    float fDistY = m_avPos[PETDELAY - 1].y - m_avPos[0].y;

    float fGap;
    if (KPet::IsPetAvailableState() && g_MyD3D->MyPlayer[m_iPlayerNumber]->IsTagMotion())
        fGap = 1.5f;
    else
        fGap = 0.8f;

    // 일정거리 이상 떨어지면 그냥 사라졌다가 나타나게 한다.
    // 포탈을 탄다든지.. 펫이 기술을 쓰고 돌아온다든지..
    if (fDistX * fDistX + fDistY * fDistY > fGap)
    {
        SetPetMotion(EGCPETMOTION::TELEPORT_READY);
        return;
    }

    // 10프레임동안 부드럽게 이동하도록 펫의 위치 조절해 준다.
    for (int i = 1; i + 1 < PETDELAY; i++)
    {
        m_avPos[i].x = m_avPos[0].x + fDistX / (float)((PETDELAY - 1) * i);

        if (m_abContact[i])
        {
            m_avPos[i].y = (m_avPos[i - 1].y + m_avPos[i + 1].y) / 2.0f;
        }
    }
}

void KPet::RenderPetName()
{
    if (false == m_bRenderName) {
        return;
    }

    if (m_iPetID == -1)
        return;

    if (m_iRenderDelay > 0)
        return;

    if (m_iPlayerNumber < 0)
        return;

    if (KPet::IsPetAvailableState())
    {
        KGCEmbarkObject* pEmbark = g_MyD3D->MyPlayer[m_iPlayerNumber]->GetMyEmbarkInstance();
        if (pEmbark)
        {
            if (pEmbark->IsNotDrawPetName())
                return;
        }


        if (!m_bPetWaitForTag || usMotion == EGCPETMOTION::INVALID ||
            !g_MyD3D->MyPlayer[m_iPlayerNumber]->m_kUserInfo.bLive ||
            g_MyD3D->MyPlayer[m_iPlayerNumber]->m_cLife == 0 ||
            g_MyD3D->MyPlayer[m_iPlayerNumber]->CheckInPortal() ||
            g_MyD3D->MyPlayer[m_iPlayerNumber]->m_pObject->IsInvisible())
            return;
    }
    else
    {
        if (m_pOwner)
            if (m_pOwner->bRender == false)
                return;
    }

    D3DXVECTOR3 pos_scr;
    D3DXVECTOR3 pos_world = vPos;
    D3DVIEWPORT9  viewport;
    D3DXMATRIX   projection, view, world;
    D3DXMatrixIdentity(&world);
    g_pd3dDevice->GetViewport(&viewport);
    g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &projection);
    g_pd3dDevice->GetTransform(D3DTS_VIEW, &view);

    pos_world.y += -0.5f;
    pos_world.x += -1.0f;
    D3DXVec3Project(&pos_scr, &pos_world, &viewport, &projection, &view, &g_kCamera.m_matCamera);

    if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE)
    {
        SiGCSquare()->GetSquarePeople(m_iPlayerNumber)->DrawPetName((int)pos_scr.x, (int)pos_scr.y);
    }
    else
    {
        if (KPet::IsPetAvailableState())
        {
            g_MyD3D->MyPlayer[m_iPlayerNumber]->Draw_PetName((int)pos_scr.x, (int)pos_scr.y);
        }
    }
}

void KPet::RenderShadow()
{
    if (false == m_bRenderShadow) {
        return;
    }

    if (m_iPetID == -1)
        return;
    if (m_iPlayerNumber < 0)
        return;

    //if( (int)m_vecPetModel.size() == 0 )
    //    return;

    if (KPet::IsPetAvailableState())
    {
        if (!m_bPetWaitForTag || usMotion == EGCPETMOTION::INVALID ||
            usMotion >= EGCPETMOTION::NUM ||
            g_MyD3D->m_KGCOption.GetQualityCount() <= 3 ||
            !g_MyD3D->MyPlayer[m_iPlayerNumber]->m_kUserInfo.bLive ||
            g_MyD3D->MyPlayer[m_iPlayerNumber]->m_cLife == 0)
            return;
    }
    else
    {
        if (m_pOwner)
            if (m_pOwner->bRender == false)
                return;
    }

    if (m_iRenderDelay > 0)
        return;

    if (IsHallow())
        return;

    D3DXMATRIX mat1, mat2, mat3;
    float fZ = 5.0f;
    m_fShadowY = g_MyD3D->MyStg->Get_LowHeight(vPos.x, vPos.y);
    float fDistance = vPos.y + m_fDistY - m_fShadowY + GetPetMotionFrm(usMotion)->GetFrameData(cFrame).Pos_y * m_fScale;
    if (fDistance < 0.0f)
        fDistance = 0.0f;

    D3DXMatrixScaling(&mat1, m_fScale, m_fScale, 0.0001f);
    D3DXMatrixTranslation(&mat2, vPos.x - 1.0f, m_fShadowY - 0.52f, 3.0f);

    D3DXMatrixMultiply(&mat3, &mat1, &mat2);
    D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

    D3DXVECTOR4 vResult;
    D3DXVec3Transform(&vResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mat1);

    g_pGraphicsHelper->SimpleShadowRender(vResult.x, vResult.y, fDistance, 0.095f, KGCLayerPart::ms_fZOrder[7]);
}

void KPet::SetMatrixPalette(int iMeshIndex, D3DXMATRIX& matBasic, bool bFrameMove, bool bEndLock)
{
    if (m_iPetID == -1)
        return;

    //if( !m_bRender || (int)m_vecPetModel.size() == 0 )
    //{
    //    cFrame = 0;
    //    return;
    //}

    if (bFrameMove)
    {
        if (EGCPETMOTION::INVALID < usMotion && usMotion < EGCPETMOTION::NUM)
        {
            if (bEndLock)
            {
                if (cFrame < static_cast<int>(GetPetMotionFrm(usMotion)->GetNum_Frame()) - 1)
                    cFrame++;
            }
            else
            {
                cFrame = (cFrame + 1) % GetPetMotionFrm(usMotion)->GetNum_Frame();
            }
        }
    }

    //    m_vecPetModel[iMeshIndex]->GetMeshObject().m_pMesh->SetMatrixPalette( &GetPetMotionFrm(  uiMotion  )->GetFrameData( cFrame ), &matBasic, true, false, true );
    //m_vecPetModel[iMeshIndex]->Render();
}

void KPet::SetPetMotion(int iMotion)
{
    if (m_pObject == NULL || m_iPetID == -1)
        return;

    usMotion = iMotion;
    cFrame = 0;

    m_fMotionX = 0.0f;
}

void KPet::AddPetTemplate(int iPetID_, int iEvolutionLevel_)
{

}

void KPet::LoadPetTemplate()
{
    KLuaManager luaMgr(KGCLuabinder::getInstance().GetLuaState());
    char strTemp[MAX_PATH] = { 0, };

    GCFUNC::LoadLuaScript(luaMgr, "Enum.stg");
    if (GCFUNC::LoadLuaScript(luaMgr, "PetTemplate.stg") == false)
    {
        MessageBoxA(NULL, "PetTemplate Load Failed!", NULL, MB_OK);
        exit(0);
    }

    g_kGlobalValue.GetPetTemplate(74, 0);

    //{
    //	KLuaManager luaMgr2;

    //	std::string strGachaPetFile = "GachaPetStat_";
    //	strGachaPetFile += GCUTIL_STR::GCStrWideToChar(NATION_CODE);
    //	strGachaPetFile += ".stg";

    //	if( GCFUNC::LoadLuaScript( luaMgr2, strGachaPetFile ) == false )
    //	{
    //		return;
    //	}

    //	for( int i = 0; i < (int)g_kGlobalValue.m_vecPetTemplate.size(); ++i )
    //	{
    //		for( int j = 0; j < (int)g_kGlobalValue.m_vecPetTemplate[i].size(); ++j )
    //		{
    //			sprintf( strTemp, "Pet%d_%d", i, j );
    //			if ( SUCCEEDED( hr = luaMgr2.BeginTable( strTemp ) ) )
    //			{
    //				luaMgr2.GetValue( "AtkConst", g_kGlobalValue.m_vecPetTemplate[i][j].fAtkConstant );
    //				luaMgr2.GetValue( "DefConst", g_kGlobalValue.m_vecPetTemplate[i][j].fDefConstant );
    //				luaMgr2.GetValue( "HPConst",  g_kGlobalValue.m_vecPetTemplate[i][j].fHPConstant );

    //				luaMgr2.GetValue( "AtkDefault", g_kGlobalValue.m_vecPetTemplate[i][j].fAtkDefault );
    //				luaMgr2.GetValue( "DefDefault", g_kGlobalValue.m_vecPetTemplate[i][j].fDefDefault );
    //				luaMgr2.GetValue( "HPDefault",  g_kGlobalValue.m_vecPetTemplate[i][j].fHPDefault );

    //				luaMgr2.GetValue( "EvolutionConst", g_kGlobalValue.m_vecPetTemplate[i][j].fEvolutionConst );

    //				luaMgr2.EndTable();
    //			}
    //		}
    //	}
    //}
}

void KPet::LoadPetCostume()
{
    KLuaManager luaMgr;

    GCFUNC::LoadLuaScript(luaMgr, "Enum.stg");
    if (GCFUNC::LoadLuaScript(luaMgr, "PetCostume.stg") == false)
    {
        MessageBoxA(NULL, "PetCostume Load Failed!", NULL, MB_OK);
        exit(0);
    }

    LUA_BEGIN_TABLE("PetCostumeList", return)
    {
        for (int i = 1; ; ++i)
        {
            DWORD dwItemID = 0;
            PetMeshInfo PetMesh;
            LUA_BEGIN_TABLE(i, break)
            {
                LUA_GET_VALUE(1, dwItemID, break);
                LUA_GET_VALUE(2, PetMesh.strMeshName, break);
                LUA_GET_VALUE(3, PetMesh.strTextureName, break);
                LUA_GET_VALUE(4, PetMesh.strShopTextureName, break);
                LUA_GET_VALUE(5, PetMesh.iCartoonTexture, break);
            }
            LUA_END_TABLE(break);

            g_kGlobalValue.AddPetCostumeMeshInfo(dwItemID, PetMesh);
        }
    }
    LUA_END_TABLE(return);
}

void KPet::LoadPetSkillInfo()
{
    KLuaManager luaMgr(KGCLuabinder::getInstance().GetLuaState());
    char strTemp[MAX_PATH] = { 0, };

    if (GCFUNC::LoadLuaScript(luaMgr, "PetEquipSkill.stg") == false)
    {
        MessageBoxA(NULL, "PetEquipSkill Load Failed!", NULL, MB_OK);
        exit(0);
    }

    g_kGlobalValue.m_mapPetSkillItemInfo.clear();

    LUA_BEGIN_TABLE("PetItemInfo", return)
    {
        for (int iPetItemList = 1; ; ++iPetItemList)
        {
            LUA_BEGIN_TABLE(iPetItemList, break)
            {
                int iItemID = -1;
                std::vector<int> vTemp;

                LUA_GET_VALUE_DEF("ItemID", iItemID, -1)

                    LUA_BEGIN_TABLE("UsePetList", break)
                {
                    for (int iPetList = 1; ; ++iPetList)
                    {
                        int iPetIDList = -1;
                        if (FAILED(luaMgr.GetValue(iPetList, iPetIDList)))
                            break;
                        vTemp.push_back(iPetIDList);
                    }

                }
                LUA_END_TABLE(break)

                    g_kGlobalValue.m_mapPetSkillItemInfo.insert(std::make_pair(iItemID, vTemp));
            }
            LUA_END_TABLE(break)
        }
    }
    LUA_END_TABLE(return)
}

void KPet::CheckPetContact(int index, bool bIsLocalPlayer)
{
    if (m_abContact[index])
    {
        float fPlayerPosX, fPlayerPosY;
        float fPetContactY;
        if (m_pOwner)
        {
            if (KPet::IsPetAvailableState())
            {
                if (m_pOwner->OwerPlayer.bIsRight)
                    fPlayerPosX = m_pOwner->OwerPlayer.vPos.x - m_fDistX;
                else
                    fPlayerPosX = m_pOwner->OwerPlayer.vPos.x + m_fDistX;
                fPlayerPosY = m_pOwner->OwerPlayer.vPos.y + m_fDistY;
            }
            else
            {
                if (m_pOwner->pOwnerRenderData->bIsRight)
                    fPlayerPosX = m_pOwner->pOwnerRenderData->vPos.x - m_fDistX;
                else
                    fPlayerPosX = m_pOwner->pOwnerRenderData->vPos.x + m_fDistX;
                fPlayerPosY = m_pOwner->pOwnerRenderData->vPos.y + m_fDistY;
            }
        }

        // 펫이 있는 X좌표에서 밑의 발판의 Y좌표를 구한다
        fPetContactY = g_MyD3D->MyStg->Get_LowHeight(m_avPos[index].x, m_avPos[index].y + 0.2f);

        // 펫의 X좌표에서 땅에 붙어있는 위치가 일정 거리이상 떨어졌다면
        // 캐릭터에게 좀 더 붙어서 허공에 뜬것처럼 보이는것을 해결해 준다
        if (ASMfabs(fPlayerPosY - fPetContactY) < 0.2f)
        {
            m_avPos[index].y = fPetContactY;
        }
        else
        {
            m_avPos[index].x = fPlayerPosX;
        }
    }
}

bool KPet::IsEnableMotionToUseSkill()
{
    if (!m_bPetWaitForTag || usMotion == EGCPETMOTION::WIN || usMotion == EGCPETMOTION::LOSE)
        return false;
    return true;
}

void KPet::CalcRemainSpecial(IN KPetInfo& kPet)
{
    std::vector<KSimpleItem>::iterator vit = kPet.m_vecEquipItem.begin();
    while (vit != kPet.m_vecEquipItem.end())
    {
        GCItem* kItem = NULL;
        if (!g_pItemMgr->FindInventory((*vit).m_dwUID, &kItem))
        {
            m_iRemainSpecial = 0;
            m_bEnableUseSkill = false;

            vit = kPet.m_vecEquipItem.erase(vit);
            continue;
        }

        if (kItem->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_SKILL)
        {
            KItem* kInvenItem = NULL;
            if (g_pItemMgr->FindInventory((*vit).m_dwUID, &kInvenItem) == true)
            {
                m_iRemainSpecial = kInvenItem->m_nCount;
            }
            else
            {
                m_iRemainSpecial = 0;
                m_bEnableUseSkill = false;

                vit = kPet.m_vecEquipItem.erase(vit);
                continue;
            }
        }
        ++vit;
    }

    // 괜히한거 같군..
    if (g_pkUIScene->m_pkGameOverlayUI->GetHeadUp())
        g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->SetRemainString(GetRemainSpecialStr());
}

void KPet::UseSkill()
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_CREATE_LIMIT))
        return;

    if (false == KPet::IsPetAvailableState())
        return;

    if (m_iPlayerNumber < 0)
        return;

    // 플레이어가 죽었으면
    if (g_MyD3D->MyPlayer[m_iPlayerNumber]->GetPlayerState() == PS_DEAD)
        return;

    if (m_fCurrentMP < MAXPETMP)
        return;

    if (IsEnableMotionToUseSkill() == false)
        return;

    if (usMotion == EGCPETMOTION::ATTACK
        || usMotion == EGCPETMOTION::MAGIC
        || usMotion == EGCPETMOTION::TELEPORT_READY
        || usMotion == EGCPETMOTION::TELEPORT_ARRIVE
        || usMotion == EGCPETMOTION::WIN
        || usMotion == EGCPETMOTION::LOSE)
        return;

    if (m_bEnableUseSkill == false)
    {
        m_fIncMP = 0.0f;
        return;
    }

    m_bEnableUseSkill = false;
    KPetInfo& kPet = g_MyD3D->MyPlayer[m_iPlayerNumber]->GetCurrentChar().kPetInfo;
    for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++)
    {
        GCItem* kItem = NULL;
        if (!g_pItemMgr->FindInventory(kPet.m_vecEquipItem[i].m_dwUID, &kItem))
            continue;

        // 기술 사용 시 스킬 아이템 수량을 하나씩 깍아준다
        if (kItem->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_SKILL)
        {
            if (KPet::IsPetAvailableState() && g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer())
            {
                CalcRemainSpecial(kPet);
            }

            m_bEnableUseSkill = g_pItemMgr->DecDurationItem(kPet.m_vecEquipItem[i].m_dwUID);
        }
    }

    // [8/1/2007 breadceo] 다 썼으면 못써야지
    if (KPet::IsPetAvailableState() && IsLocalPet())
    {
        CalcRemainSpecial(kPet);
    }

    bIsRight = g_kLocalPlayerLatency->kRenderData[0].bIsRight;

    // 모션을 바꿔주고 패킷을 세팅해 둔다.
    // TODO : 여러종류의 스킬이 존재하므로 스킬의 정보도 담아서 보내야 한다.
    usMotion = EGCPETMOTION::ATTACK;
    //m_iMotion = EGCPETMOTION::MAGIC;
    cFrame = 0;

    m_fCurrentMP = 0.0f;

    EnableGlyphEffect();

    g_kUsePetSkill->bUsePetSkill = true;
    g_kUsePetSkill->cPlayerIndex = (char)m_iPlayerNumber;
    g_kUsePetSkill->cPetMotion = (char)usMotion;
    g_kUsePetSkill->PetSkill.bIsRight = bIsRight;
    g_kUsePetSkill->PetSkill.fX = vPos.x;
    g_kUsePetSkill->PetSkill.fY = vPos.y;

    // 리플레이에 펫 스킬 사용하는것 남기기
    g_MyD3D->MyReplay->Set_UsePetSkill(g_kUsePetSkill, g_kUsePetSkill->cPlayerIndex);

    SiKGCAchieveManager()->OccurAction(SAchieve::USE_PETSKILL, g_MyD3D->GetMyPlayer()->GetCharType(), m_iPetID);
}

void KPet::CreateParticleEffect()
{
    ClearParticleEffect();
    LoadPetEffect();
}

void KPet::ClearParticleEffect()
{
    if (m_pParticleEventSequence.size() > 0)
    {
        std::map< std::string, CParticleEventSeqPTR >::iterator Iter;

        for (Iter = m_pParticleEventSequence.begin(); Iter != m_pParticleEventSequence.end(); Iter++)
        {
            g_ParticleManager->DeleteSequence(Iter->second);
        }

        m_pParticleEventSequence.clear();
    }

    if (m_pParticleEventSequence2.empty() == false)
    {
        std::map< std::string, CParticleEventSeqPTR >::iterator Iter;

        for (Iter = m_pParticleEventSequence2.begin(); Iter != m_pParticleEventSequence2.end(); Iter++)
        {
            g_ParticleManager->DeleteSequence(Iter->second);
        }

        m_pParticleEventSequence2.clear();
    }

    m_mActionParticle.clear();
    m_mParticlePos.clear();
}

CParticleEventSeqPTR KPet::AddParticle(char* strSeqeunce, float fDirPosX, int iParticleNum, bool bReverseTex, float fOffsetX, float fOffsetY)
{
    CParticleEventSeqPTR pTemp;
    pTemp = g_ParticleManager->CreateSequence(strSeqeunce, vPos.x - 1.0f + fOffsetX, vPos.y - 0.42f + m_fDistY + fOffsetY, 0.5f);
#if defined(RESOURCE_GRAPH)		
    char tmp1[256];
    char tmp2[256];
    sprintf(tmp1, "PARTICLE_%s", strSeqeunce);
    sprintf(tmp2, "PET_%d", GetPetNameID());
    g_pGCDeviceManager2->AddRelation(tmp1, tmp2);
#endif

    if (pTemp != NULL)
    {
        // 방향성 세팅
        DIRECTION_INFO stDirect;
        stDirect.m_fDirPosX = -fDirPosX;
        stDirect.m_bDirVelocity = true;
        stDirect.m_pbIsRight = &bIsRight;
        stDirect.m_bReverseTexture = bReverseTex;
        pTemp->SetDirectInfo(&stDirect);

        pTemp->SetShow(true);

        if (iParticleNum != 0)
        {
            CMinMax<float> emitRate(1000.0f, 1000.0f);
            pTemp->SetEmitRate(emitRate);
            pTemp->SetKillNum(iParticleNum);
        }
    }

    return pTemp;
}

CParticleEventSeqPTR KPet::AddParticleNoDirectionPosWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY, float fTraceTime, bool bTracePet_)
{
    CParticleEventSeqPTR pTemp;
    pTemp = g_ParticleManager->CreateSequence(strSeqeunce, vPos.x - 1.0f, vPos.y - 0.42f + m_fDistY, 0.5f);

    if (pTemp != NULL)
    {
        pTemp->SetShow(true);
        TRACE_INFO stTrace;
        if (bTracePet_) {
            stTrace.m_pvPos = &m_vTraceCenterPos;
        }
        else {
            stTrace.m_pvPos = &m_vTracePos;
        }
        if (fTraceTime == 0.0f)
            stTrace.m_fParticleTraceTime = pTemp->GetKillTime();
        else
            stTrace.m_fParticleTraceTime = fTraceTime;
        stTrace.m_fXOffset = fOffsetX;
        stTrace.m_fYOffset = fOffsetY;
        pTemp->SetTrace(&stTrace);

        if (iParticleNum != 0)
        {
            CMinMax<float> emitRate(1000.0f, 1000.0f);
            pTemp->SetEmitRate(emitRate);
            pTemp->SetKillNum(iParticleNum);
        }
    }
    return pTemp;
}

CParticleEventSeqPTR KPet::AddParticleXOffset(char* strSeqeunce, int iParticleNum, float fOffsetX)
{
    CParticleEventSeqPTR pTemp;
    pTemp = g_ParticleManager->CreateSequence(strSeqeunce, vPos.x - 1.0f + fOffsetX,
        vPos.y - 0.42f,
        0.5f);
    if (pTemp != NULL)
    {
        // 방향성 세팅
        DIRECTION_INFO stDirect;
        stDirect.m_fDirPosX = 0.0f;
        stDirect.m_bDirVelocity = true;
        stDirect.m_pbIsRight = &bIsRight;
        pTemp->SetDirectInfo(&stDirect);
        pTemp->SetShow(true);

        if (iParticleNum != 0)
        {
            CMinMax<float> emitRate(1000.0f, 1000.0f);
            pTemp->SetEmitRate(emitRate);
            pTemp->SetKillNum(iParticleNum);
        }
    }

    return pTemp;
}

bool KPet::IsHallow()
{
    if (KPet::IsPetAvailableState())
    {
        if (g_MyD3D->MyPlayer[m_iPlayerNumber]->Item_Remain_Time[GC_GAME_ITEM_HALLOW] ||
            g_MyD3D->MyPlayer[m_iPlayerNumber]->CheckHiding())
        {
            if (g_MyD3D->IsMyTeam(m_iPlayerNumber) || g_MyD3D->IsLocalPlayerObserver() || (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW))
            {
                if (o5 < 3)
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }

        if (g_MyD3D->MyPlayer[m_iPlayerNumber]->GetPlayerState() == PS_DEAD)
        {
            if (o5 < 3)
            {
                return true;
            }
        }
    }

    return false;
}

int KPet::GetPetNameID()
{
    return g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).iNameID;
}

int KPet::GetPetDesc(int iDescState_)
{
    return g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).GetDescID(iDescState_);
}

GCDeviceMotionFRM* KPet::GetPetMotionFrm(int iMotion_)
{
    std::map<int, GCDeviceMotionFRM*>::iterator mit = m_mapMotion.find(iMotion_);
    if (mit != m_mapMotion.end())
    {
        return mit->second;
    }
#if defined( __PATH__ )
    char strTempMotion[MAX_PATH];
    sprintf(strTempMotion, "PET%03d_%d_TEMP.FRM", m_iPetID, m_iEvolutionLevel);
    return g_pGCDeviceManager2->CreateMotionFRM(strTempMotion);
#else
    return NULL;
#endif
}

float KPet::GetPetResultScale()
{
    return g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fResultScale;
}

D3DXVECTOR3 KPet::GetPetAxisCorrection()
{
    return g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).vAxisCorrection;
}

void KPet::AddDamage(int eDamage, float fX, float fY, bool bPetTrace, bool bPlayerTrace)
{
    if (false == KPet::IsPetAvailableState())
        return;

    CDamageInstance* fire;
    fire = g_MyD3D->m_pDamageManager->PetAdd(eDamage, m_iPlayerNumber, false, -1, m_fAtkRatio, bPetTrace, bPlayerTrace);

    if (fire == NULL)
        return;
#if defined(RESOURCE_GRAPH)
    char tmp1[256];
    char tmp2[256];
    sprintf(tmp1, "DAMAGE_%d", eDamage);
    sprintf(tmp2, "PET_%d", GetPetNameID());
    g_pGCDeviceManager2->AddRelation(tmp1, tmp2);
#endif

    if (bIsRight)
    {
        fire->m_X = vPos.x - fX;
        fire->m_Y = vPos.y + fY + m_fDistY;
        fire->m_IsRight = true;
        fire->m_Angle = fire->m_pOriginalDamage->m_Start_Angle;
    }
    else
    {
        fire->m_X = vPos.x + fX;
        fire->m_Y = vPos.y + fY + m_fDistY;
        fire->m_IsRight = false;
        fire->m_Angle = -fire->m_pOriginalDamage->m_Start_Angle;
    }
}

void KPet::AddActionParticle(char* strParticle_)
{
    CParticleEventSeqPTR pTemp;

    pTemp = AddParticle(strParticle_,
        m_mActionParticle[strParticle_].fPosX, 0);
    // 2007/1/16. iridology. 2번째 인자는 ParticleNum... 0일 경우 알아서 스크립트대로
    // m_mActionParticle[strParticle_].fPosY );
#if defined(RESOURCE_GRAPH)		
    char tmp1[256];
    char tmp2[256];
    sprintf(tmp1, "PARTICLE_%s", strParticle_);
    sprintf(tmp2, "PET_%d", GetPetNameID());
    g_pGCDeviceManager2->AddRelation(tmp1, tmp2);
#endif

    if (bIsRight == true)
    {
        pTemp->SetPosition(vPos.x - 1.0f + m_mActionParticle[strParticle_].fCorrPosRX
            , vPos.y - 0.5f + m_mActionParticle[strParticle_].fCorrPosRY, 0.5f);
    }
    else
    {
        pTemp->SetPosition(vPos.x - 1.0f + m_mActionParticle[strParticle_].fCorrPosLX
            , vPos.y - 0.5f + m_mActionParticle[strParticle_].fCorrPosLY, 0.5f);
    }
}

void KPet::AddBuff(int iPlayerIndex_, int iBuffindex, float fbuffTime_)
{
    g_pMagicEffect->SetMagicEffect(iPlayerIndex_, iBuffindex, fbuffTime_);
}

void KPet::GetIncMPFromLua()
{
    if (false == KPet::IsPetAvailableState())
        return;

    if (m_iPlayerNumber < 0)
        return;

    m_fIncMP = 0.0f;

    if (m_iPlayerNumber < 0)
        return;

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L);
    lua_getglobal(L, "SetPetIncMPPoint");
    if (lua_isfunction(L, -1) == false) {
        lua_pop(L, 1);
        return;
    }

    lua_pushnumber(L, m_iPlayerNumber);
    lua_pushnumber(L, m_dwSkillGoodsID);
    if (lua_pcall(L, 2, 0, 0) != 0) {
        lua_pop(L, 1);
    }
}

bool KPet::IsItemEquip(int iItemKind_, int iGoodsID_)
{
    if (false == KPet::IsPetAvailableState())
        return false;
    if (m_iPlayerNumber < 0)
        return false;

    KPetInfo& kPet = g_MyD3D->MyPlayer[m_iPlayerNumber]->GetCurrentChar().kPetInfo;

    if (iItemKind_ == -1 || iGoodsID_ == -1)// 어떤 아이템이든 펫이 장착하고 있는지 알 고 싶을 때
    {
        if (kPet.m_vecEquipItem.size() > 0)
        {
            return true;
        }
    }
    else if (iItemKind_ != -1) // 해당하는 아이템 종류를 펫이 장착하고 있는지 알고 싶을 때
    {
        for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++)
        {
            GCItem* kItem = NULL;
            if (g_pItemMgr->FindInventory(kPet.m_vecEquipItem[i].m_dwUID, &kItem) == false)
                continue;

            if (kItem->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_SKILL)
                return true;
        }
    }
    else if (iItemKind_ == -1 && iGoodsID_ != -1) // GoodsID에 해당하는 아이템을 장착하고 있는지 알고 싶을 때
    {
        for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++)
        {
            GCItem* kItem = NULL;
            if (g_pItemMgr->FindInventory(kPet.m_vecEquipItem[i].m_dwUID, &kItem) == false)
                continue;

            if (kItem->dwGoodsID == iGoodsID_)
            {
                return true;
            }
        }
    }
    else
    {
        for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++)
        {
            GCItem* kItem = NULL;
            if (g_pItemMgr->FindInventory(kPet.m_vecEquipItem[i].m_dwUID, &kItem) == false)
                continue;

            if (kItem->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_SKILL && kItem->dwGoodsID == iGoodsID_)
            {
                return true;
            }
        }
    }

    return false;
}

void KPet::LoadPetEffect()
{
    // 이 펫을 가지고 있는 주인이 없으면 리턴시킨다.
    if (m_iPlayerNumber < 0)
        return;

    if (m_iPetID == -1)
        return;

    const SPetTemplate& sPetTemplate = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel);

    BOOST_FOREACH(const SPetEffectInfo & val, sPetTemplate.vecEffect) {
        m_pParticleEventSequence[val.m_strEffectName] = g_ParticleManager->CreateSequence(val.m_strEffectName, val.m_vCreatePos.x, val.m_vCreatePos.y, val.m_vCreatePos.z);
        m_mParticlePos[val.m_strEffectName].first = val.m_vOffsetPos[1];
        m_mParticlePos[val.m_strEffectName].second = val.m_vOffsetPos[0];
#if defined(RESOURCE_GRAPH)	
        g_pGCDeviceManager2->AddRelation("PARTICLE_" + strEffectName, "PET_" + GetPetNameID());
#endif
    }

    BOOST_FOREACH(const SPetEffectInfo & val, sPetTemplate.vecActionEffect) {
        SParticleInfo sPinfo;
        sPinfo.strParticleName = val.m_strEffectName;
        sPinfo.fPosX = val.m_vCreatePos.x;
        sPinfo.fPosY = val.m_vCreatePos.y;
        sPinfo.fPosZ = val.m_vCreatePos.z;
        sPinfo.fCorrPosRX = val.m_vOffsetPos[1].x;
        sPinfo.fCorrPosRY = val.m_vOffsetPos[1].y;
        sPinfo.fCorrPosLX = val.m_vOffsetPos[0].x;
        sPinfo.fCorrPosLY = val.m_vOffsetPos[0].y;

        m_mActionParticle[sPinfo.strParticleName] = sPinfo;
    }


    m_vBoneTracePos.clear();
    int itmp = 0;
    BOOST_FOREACH(const SPetBoneEffectInfo & val, sPetTemplate.vecBoneEffect) {


        if (val.m_iBoneID == -1)
            continue;

        D3DXMATRIX matBone;
        m_pObject->SetMotionMatrix(true, NULL, false, true);
        m_pObject->GetAngleBone(val.m_iBoneID, &matBone);

        matBone *= *(m_pObject->GetWorldMat());

        m_vBoneTracePos.push_back(D3DXVECTOR3());
        m_vBoneTracePos[itmp].x = matBone._41;
        m_vBoneTracePos[itmp].y = matBone._42;
        m_vBoneTracePos[itmp].z = 0.5f;

        m_pParticleEventSequence2[val.m_strEffectName] = g_ParticleManager->CreateSequence(val.m_strEffectName.c_str(),
            m_vBoneTracePos[itmp].x + val.m_vOffsetPos[1].x,
            m_vBoneTracePos[itmp].y + val.m_vOffsetPos[1].y, 0.5f);

        m_pParticleEventSequence2[val.m_strEffectName].m_iBone = val.m_iBoneID;


        TRACE_INFO stTrace;
        stTrace.m_pvPos = &m_vBoneTracePos[itmp];
        stTrace.m_fParticleTraceTime = val.m_fTraceTime;
        m_pParticleEventSequence2[val.m_strEffectName]->SetTrace(&stTrace);

        itmp++;
    }
}

void KPet::LoadPetEffect(TagPetInfo& tagPetInfo)
{
    const SPetTemplate& sPetTemplate = g_kGlobalValue.GetPetTemplate(tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel);

    BOOST_FOREACH(const SPetEffectInfo & val, sPetTemplate.vecEffect) {
        tagPetInfo.m_pParticleEventSequence[val.m_strEffectName] = g_ParticleManager->CreateSequence(val.m_strEffectName, val.m_vCreatePos.x, val.m_vCreatePos.y, val.m_vCreatePos.z);
        tagPetInfo.m_mParticlePos[val.m_strEffectName].first = val.m_vOffsetPos[1];
        tagPetInfo.m_mParticlePos[val.m_strEffectName].second = val.m_vOffsetPos[0];
    }

    BOOST_FOREACH(const SPetEffectInfo & val, sPetTemplate.vecActionEffect) {
        SParticleInfo sPinfo;
        sPinfo.strParticleName = val.m_strEffectName;
        sPinfo.fPosX = val.m_vCreatePos.x;
        sPinfo.fPosY = val.m_vCreatePos.y;
        sPinfo.fPosZ = val.m_vCreatePos.z;
        sPinfo.fCorrPosRX = val.m_vOffsetPos[1].x;
        sPinfo.fCorrPosRY = val.m_vOffsetPos[1].y;
        sPinfo.fCorrPosLX = val.m_vOffsetPos[0].x;
        sPinfo.fCorrPosLY = val.m_vOffsetPos[0].y;

        tagPetInfo.m_mActionParticle[sPinfo.strParticleName] = sPinfo;
    }
}

void KPet::PlaySound(int iSoundNum, unsigned char chDelay)
{
    std::ostringstream strmSoundNum;
    strmSoundNum << iSoundNum;

    if (KPet::IsPetAvailableState() && g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer() && (g_MyD3D->MyReplay->m_iCurrState != RP_VIEW))
        g_KDSound.Play(strmSoundNum.str().c_str(), 0, LATENCY + SHOWLATENCY + chDelay);
    else
        g_KDSound.Play(strmSoundNum.str().c_str(), 0, chDelay);
}

void KPet::InitTagPetData(IN int iPidx, IN int iCharType)
{
    m_mapTagPet.clear();

    if (IsPetAvailable(iPidx, iCharType) == true)
    {
        TagPetInfo tempPetInfo;
        tempPetInfo.InitTagPetData(this);
        tempPetInfo.m_bNeedDelete = FALSE;
        tempPetInfo.m_iCharType = iCharType;
        m_mapTagPet.insert(std::make_pair(iCharType, tempPetInfo));
    }
}

void KPet::AddTagPetInfo(IN int iPidx, IN int iCharType)
{
    if (IsPetAvailable(iPidx, iCharType) == true)
    {
        TagPetInfo tempPetInfo;
        tempPetInfo.Init();
        tempPetInfo.m_iCharType = iCharType;
        StartPet(iPidx, tempPetInfo, EGCPETLOCATION::GAME);
        m_mapTagPet.insert(std::make_pair(iCharType, tempPetInfo));
    }
}

bool KPet::IsPetAvailable(IN int iPidx, IN int iCharType)
{
    if (false == KPet::IsPetAvailableState())
        return false;

    if (iPidx == -1)
        return false;

    m_iPlayerNumber = iPidx;

    KPetInfo& kPetInfo = g_MyD3D->MyPlayer[m_iPlayerNumber]->m_kUserInfo.GetCurrentChar(iCharType).kPetInfo;

    int iPetID = g_kGlobalValue.GetPetIDFromItemID(kPetInfo.m_dwID);
    if (iPetID != -1)
    {
        if ((g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer() && g_MyD3D->MyReplay->m_iCurrState != RP_VIEW))
        {
            if (kPetInfo.m_dwUID == 0 || !g_MyD3D->IsExistPetInfo(kPetInfo.m_dwUID))
                return false;
        }
        else
        {
            if (kPetInfo.m_dwUID == 0)
                return false;
        }
    }
    else
        return false;

    return true;
}

void KPet::StartPet(IN int iPidx, OUT TagPetInfo& tagPetInfo, int iLocation, bool bUseShopTexture /*= false*/)
{
    if (false == KPet::IsPetAvailableState())
        return;

    EndPet(tagPetInfo);

    m_iPlayerNumber = iPidx;

    KPetInfo& kPetInfo = g_MyD3D->MyPlayer[m_iPlayerNumber]->m_kUserInfo.GetCurrentChar(tagPetInfo.m_iCharType).kPetInfo;

    // 펫 이름 설정
    swprintf(tagPetInfo.m_strPetName, PETNICKNAME, kPetInfo.m_strName.c_str());

    // 기본 정보들을 세팅한다
    tagPetInfo.m_iPetID = g_kGlobalValue.GetPetIDFromItemID(kPetInfo.m_dwID);
    tagPetInfo.m_iEvolutionLevel = kPetInfo.m_cPromotion;
    tagPetInfo.m_iLevel = kPetInfo.m_iLevel;

    tagPetInfo.m_fScale = g_kGlobalValue.GetPetTemplate(tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel).fScale;
    tagPetInfo.m_fDistX = g_kGlobalValue.GetPetTemplate(tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel).fDistX;
    tagPetInfo.m_fDistY = g_kGlobalValue.GetPetTemplate(tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel).fDistY;

    tagPetInfo.m_kCostume.m_dwID = kPetInfo.m_kCostume.m_dwID;
    tagPetInfo.m_kCostume.m_dwUID = kPetInfo.m_kCostume.m_dwUID;

    tagPetInfo.m_fScale *= GetPetScaleFromPetLevel(tagPetInfo.m_iLevel);

    SAFE_REMOVE_DELETE(tagPetInfo.m_pObject);

    tagPetInfo.m_pObject = new GCObject();
    tagPetInfo.m_pObject->SetShellMatrixScale(0.26f);
    // 펫 모델 로드
    //LoadPetModel( iLocation, tagPetInfo.m_kCostume.m_dwID, tagPetInfo.m_pObject, tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel );
    LoadPetModel(bUseShopTexture, tagPetInfo.m_kCostume.m_dwID, tagPetInfo.m_pObject, tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel);
    LoadPetMotion(iLocation, tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel, &tagPetInfo.m_mapMotion);

    for (int i = 0; i < (int)kPetInfo.m_vecEquipItem.size(); i++)
    {
        KItem* kInventory = NULL;
        GCItem* kItem = NULL;

        if (g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer()) {
            if (g_pItemMgr->FindInventory(kPetInfo.m_vecEquipItem[i].m_dwUID, &kInventory, &kItem) == false)
                continue;
        }
        else {
            kItem = g_pItemMgr->GetItemData(kPetInfo.m_vecEquipItem[i].m_dwID);
            if (NULL == kItem)
                continue;
        }



        if (kItem->dwPetEquipItemSlot & EPET_EQUIP_ITEM_SLOT_SKILL)
        {
            if (g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer()) {
                if (kInventory->m_nCount > 0) {
                    tagPetInfo.m_bEnableUseSkill = true;
                    tagPetInfo.m_dwSkillGoodsID = kItem->dwGoodsID;
                }
            }
            else {
                tagPetInfo.m_bEnableUseSkill = true;
                tagPetInfo.m_dwSkillGoodsID = kItem->dwGoodsID;
            }
        }
    }

    CreateParticleEffect(tagPetInfo);
    tagPetInfo.ShowEffect(false);
    LoadPetSound(tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel);
    LoadPetActionMotion(tagPetInfo.m_iPetID, tagPetInfo.m_iEvolutionLevel, tagPetInfo.m_dwSkillGoodsID, &tagPetInfo.m_mapMotion);
}

void KPet::CreateParticleEffect(OUT TagPetInfo& tagPetInfo)
{
    ClearParticleEffect(tagPetInfo);
    LoadPetEffect(tagPetInfo);
}

void KPet::ClearParticleEffect(OUT TagPetInfo& tagPetInfo)
{
    if (tagPetInfo.m_pParticleEventSequence.size() > 0)
    {
        std::map< std::string, CParticleEventSeqPTR >::iterator Iter;

        for (Iter = tagPetInfo.m_pParticleEventSequence.begin(); Iter != tagPetInfo.m_pParticleEventSequence.end(); Iter++)
        {
            g_ParticleManager->DeleteSequence(Iter->second);
        }

        tagPetInfo.m_pParticleEventSequence.clear();
    }
    if (tagPetInfo.m_pParticleEventSequence2.size() > 0)
    {
        std::map< std::string, CParticleEventSeqPTR >::iterator Iter;

        for (Iter = tagPetInfo.m_pParticleEventSequence2.begin(); Iter != tagPetInfo.m_pParticleEventSequence2.end(); Iter++)
        {
            g_ParticleManager->DeleteSequence(Iter->second);
        }

        tagPetInfo.m_pParticleEventSequence2.clear();
    }

    tagPetInfo.m_mActionParticle.clear();
    tagPetInfo.m_mParticlePos.clear();
}

void KPet::EndPet(OUT TagPetInfo& tagPetInfo)
{
    if (tagPetInfo.m_bNeedDelete == FALSE)
        return;

    SAFE_REMOVE_DELETE(tagPetInfo.m_pObject);

    std::map<int, GCDeviceMotionFRM*>::iterator mit;
    for (mit = tagPetInfo.m_mapMotion.begin(); mit != tagPetInfo.m_mapMotion.end(); ++mit) {
        SAFE_RELEASE(mit->second);
    }
    tagPetInfo.m_mapMotion.clear();

    ClearParticleEffect(tagPetInfo);

    tagPetInfo.m_iPetID = -1;

    tagPetInfo.m_fScale = 0.0f;

    tagPetInfo.m_fCurrentMP = 0.0f;
    tagPetInfo.m_bEnableUseSkill = false;

    tagPetInfo.m_fAtkRatio = 1.0f;
}

void KPet::ReleaseTagPetResource()
{
    std::map< int, TagPetInfo >::iterator mit;
    for (mit = m_mapTagPet.begin(); mit != m_mapTagPet.end(); ++mit)
    {
        TagPetInfo& tagPetInfo = mit->second;
        if (tagPetInfo.m_bNeedDelete == FALSE)
            continue;

        EndPet(tagPetInfo);
    }

    m_mapTagPet.clear();
}

void KPet::TagPet(IN int iBeforeChar, IN int iCharType)
{
    if (false == KPet::IsPetAvailableState())
        return;

    std::map< int, TagPetInfo >::iterator mitBef = m_mapTagPet.find(iBeforeChar);
    std::map< int, TagPetInfo >::iterator mit = m_mapTagPet.find(iCharType);

    if (mitBef != m_mapTagPet.end())
    {
        // 이전 Pet 정보 저장
        TagPetInfo& tagBefPetInfo = mitBef->second;
        tagBefPetInfo.m_bNeedDelete = TRUE;
        tagBefPetInfo.InitTagPetData(this);
        tagBefPetInfo.ShowEffect(false);
    }

    // Tag한 케릭터가 장비한 펫이 없다면
    if (mit == m_mapTagPet.end())
    {
        // 2007/3/30. iridology. 3명 골라서 태그할 경우 문제가 생길 수도 있다.
        // Tag한 케릭터가 장비한 펫이 없고 이전 펫 정보가 있을 경우에는
        // 이전 펫은 지울 필요가 없다.
        if (mitBef != m_mapTagPet.end())
        {
            mitBef->second.m_bNeedDelete = FALSE;
        }

        ShowEffect(false);
        m_bPetWaitForTag = false;
        m_iRemainSpecial = 0;
        m_bEnableUseSkill = false;
        return;
    }

    // 장비한 펫이 있다면 펫정보 세팅
    TagPetInfo& tagPetInfo = mit->second;
    tagPetInfo.m_bNeedDelete = FALSE;
    m_iPetID = tagPetInfo.m_iPetID;
    m_iEvolutionLevel = tagPetInfo.m_iEvolutionLevel;
    m_iLevel = tagPetInfo.m_iLevel;
    //    m_vecPetModel = tagPetInfo.m_vecPetModel;
    m_bEnableUseSkill = tagPetInfo.m_bEnableUseSkill;
    m_dwSkillGoodsID = tagPetInfo.m_dwSkillGoodsID;
    GetIncMPFromLua();
    m_fCurrentMP = tagPetInfo.m_fCurrentMP;
    wcsncpy(m_strPetName, tagPetInfo.m_strPetName, PETNICKNAME);
    SetOwnerInfo(m_iPlayerNumber);
    m_fScale = tagPetInfo.m_fScale;
    m_fDistX = tagPetInfo.m_fDistX;
    m_fDistY = tagPetInfo.m_fDistY;
    m_mapMotion = tagPetInfo.m_mapMotion;
    m_mActionParticle = tagPetInfo.m_mActionParticle;
    m_pParticleEventSequence = tagPetInfo.m_pParticleEventSequence;
    m_pParticleEventSequence2 = tagPetInfo.m_pParticleEventSequence2;
    m_mParticlePos = tagPetInfo.m_mParticlePos;
    m_fNameHeight = tagPetInfo.m_fNameHeight;

    if (m_pObject)
    {
        EndAfterImage();
        g_RenderManager->RemoveObject(m_pObject);
        tagPetInfo.m_pObject->SetMeshState(m_pObject->GetMeshState());
    }
    m_pObject = tagPetInfo.m_pObject;
    g_RenderManager->AddObject(m_pObject);

    tagPetInfo.ShowEffect(true);

    SetPetMotion(EGCPETMOTION::WAIT);
    if (g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer())
    {
        CalcRemainSpecial(g_MyD3D->MyPlayer[m_iPlayerNumber]->GetCurrentChar().kPetInfo);
    }

    m_bPetWaitForTag = true;
}

#ifdef __EQUAL_OPERATOR__
#undef __EQUAL_OPERATOR__
#endif
#define __EQUAL_OPERATOR__(p) p = pPet->p
void TagPetInfo::InitTagPetData(KPet* pPet)
{
    __EQUAL_OPERATOR__(m_iPetID);
    __EQUAL_OPERATOR__(m_iEvolutionLevel);
    __EQUAL_OPERATOR__(m_iLevel);
    //    __EQUAL_OPERATOR__(m_vecPetModel);
    __EQUAL_OPERATOR__(m_pObject);
    __EQUAL_OPERATOR__(m_bEnableUseSkill);
    __EQUAL_OPERATOR__(m_dwSkillGoodsID);
    __EQUAL_OPERATOR__(m_fCurrentMP);
    __EQUAL_OPERATOR__(m_mapMotion);
    __EQUAL_OPERATOR__(m_mActionParticle);
    __EQUAL_OPERATOR__(m_pParticleEventSequence);
    __EQUAL_OPERATOR__(m_mParticlePos);
    wcsncpy(m_strPetName, pPet->m_strPetName, PETNICKNAME);

    __EQUAL_OPERATOR__(m_fScale);
    __EQUAL_OPERATOR__(m_fDistX);
    __EQUAL_OPERATOR__(m_fDistY);
}
#undef __EQUAL_OPERATOR__

std::string TagPetInfo::toStr()
{
    std::stringstream stm;

    stm << "NeedDelete : " << m_bNeedDelete << ", CharType : " << m_iCharType
        << ", PetID : " << m_iPetID << ", PetEvolutionLevel : " << m_iEvolutionLevel << ",\n"
        //<< ", PetModel : " << m_pPetModel->GetMesh()->IsDisable() << ",\n"
        << "\tbEnableUseSkill : " << m_bEnableUseSkill << ", SkillGoodsID : " << (int)m_dwSkillGoodsID
        << ", CurMP : " << m_fCurrentMP << ", MotionSize : " << (int)m_mapMotion.size() << ",\n"
        << "\tActParticleSize : " << (int)m_mActionParticle.size()
        << ", ParticleEventSeq : " << (int)m_pParticleEventSequence.size()
        << ", ParticlePos : " << (int)m_mParticlePos.size() << "\n";

    return stm.str();
}

void TagPetInfo::ShowEffect(bool bShow)
{
    std::map<std::string, CParticleEventSeqPTR>::iterator mit;
    for (mit = m_pParticleEventSequence.begin(); mit != m_pParticleEventSequence.end(); ++mit)
    {
        CParticleEventSeqPTR pSeq = mit->second;
        pSeq->SetShow(bShow);
    }
}

void TagPetInfo::Init()
{
    m_bNeedDelete = TRUE;
    m_iCharType = GC_CHAR_INVALID;
    m_iPetID = EGCPETID::INVALID;
    m_iEvolutionLevel = 0;
    m_iLevel = 0;

    SAFE_REMOVE_DELETE(m_pObject);

    m_bEnableUseSkill = false;
    m_dwSkillGoodsID = -1;
    m_fCurrentMP = 0.0f;

    std::map<int, GCDeviceMotionFRM*>::iterator mit;
    for (mit = m_mapMotion.begin(); mit != m_mapMotion.end(); ++mit) {
        SAFE_RELEASE(mit->second);
    }
    m_mapMotion.clear();
    m_mActionParticle.clear();
    m_pParticleEventSequence.clear();
    m_pParticleEventSequence2.clear();
    m_mParticlePos.clear();
    wcsncpy(m_strPetName, L"", PETNICKNAME);
    m_fScale = 0.0f;
    m_fDistX = 0.0f;
    m_fDistY = 0.0f;
}

int TagPetInfo::GetPetMotionID()
{
    return m_iPetID;
}


void KPet::ShowEffect(IN bool bShow)
{
    std::map<std::string, CParticleEventSeqPTR>::iterator mit;
    for (mit = m_pParticleEventSequence.begin(); mit != m_pParticleEventSequence.end(); ++mit)
    {
        CParticleEventSeqPTR pSeq = mit->second;
        pSeq->SetShow(bShow);
    }
}

bool KPet::IsUseMotion(int iLocation, int iMotion)
{
    switch (iLocation)
    {
    case EGCPETLOCATION::GAME:
        if (iMotion != EGCPETMOTION::ROOM &&
            iMotion != EGCPETMOTION::SHOP &&
            iMotion != EGCPETMOTION::SHOP_ACTION)
            return true;
        break;

    case EGCPETLOCATION::GAME_LOADING:
        if (iMotion == EGCPETMOTION::GAME_LOADING)
            return true;
        break;

    case EGCPETLOCATION::ROOM:
        if (iMotion == EGCPETMOTION::ROOM)
            return true;
        break;

    case EGCPETLOCATION::SHOP:
        if (iMotion == EGCPETMOTION::WAIT ||
            iMotion == EGCPETMOTION::SHOP ||
            iMotion == EGCPETMOTION::SHOP_ACTION)
            return true;
        break;
    }

    return false;
}

std::string KPet::toStr()
{
    std::stringstream stm;

    if (m_iPetID == -1)
        stm << "NoPet\n";
    else
    {
        stm << "Owner : " << m_iPlayerNumber << " PetID : " << m_iPetID << " EvolutionLv : " << m_iEvolutionLevel << "\n"
            << " Motion : " << GetMotionStr() << " uiMotionNum : " << (int)usMotion << "cFrame : " << (int)cFrame << "\n"
            << " RemainSpecial : " << m_iRemainSpecial << " SkillGoodsID : " << m_dwSkillGoodsID << " IncMP : " << m_fIncMP << "\n"
            << " CurMP : " << m_fCurrentMP << "\n";
    }

    return stm.str();
}

std::string KPet::toTagStr()
{
    std::stringstream stm;
    std::map< int, TagPetInfo >::iterator mit;
    int i = 0;
    for (mit = m_mapTagPet.begin(); mit != m_mapTagPet.end(); ++mit, ++i)
    {
        stm << "[" << m_iPlayerNumber << "TagPetInfo" << i << "] " << mit->second.toStr();
    }

    return stm.str();
}

float KPet::GetPetScaleFromPetLevel(int iLevel)
{
#if defined( ENABLE_PET_LEVEL_SCALE )
    {
        if (iLevel <= 10)
            return 0.7f;
        else if (iLevel <= 20)
            return 0.8f;
        else if (iLevel <= 30)
            return 0.9f;
        else if (iLevel < 40)
            return 1.0f;

        return 1.2f;
    }
#else // !defined( ENABLE_PET_LEVEL_SCALE )
    {
        return 1.0f;
    }
#endif // end defined( ENABLE_PET_LEVEL_SCALE )
}
void KPet::LoadPetSound(int iPetID_, int iPetEvolution_)
{
    int iSize = (int)g_kGlobalValue.GetPetTemplate(iPetID_, iPetEvolution_).vecSound.size();
    //이녀석이 사용하는 사운드도 함께 로드해야 되지 않겄소?
    for (int i = 0; i < iSize; ++i)
    {
        g_KDSound.PreparetoLoad(g_kGlobalValue.GetPetTemplate(iPetID_, iPetEvolution_).vecSound[i]);
        g_KDSound.LoadFromIndex(g_kGlobalValue.GetPetTemplate(iPetID_, iPetEvolution_).vecSound[i]);
    }
}

void KPet::AddDamageWithAngle(int eDamage, float fX, float fY, float fStartAngle, bool bMoveAngle)
{
    if (false == KPet::IsPetAvailableState())
        return;

    CDamageInstance* fire;
    fire = g_MyD3D->m_pDamageManager->PetAddWithAngle(eDamage, m_iPlayerNumber, fStartAngle, false, -1, m_fAtkRatio);

    if (fire == NULL)
        return;

    if (bIsRight)
    {
        fire->m_X = vPos.x - fX;
        fire->m_Y = vPos.y + fY + m_fDistY;
        fire->m_IsRight = true;
        fire->m_Angle = fire->m_pOriginalDamage->m_Start_Angle + fStartAngle;
        if (bMoveAngle)
        {
            float sx = fire->m_pOriginalDamage->m_x_Speed;
            float sy = fire->m_pOriginalDamage->m_y_Speed;
            sy = sinf(fire->m_Angle) * sx;
            sx = cosf(fire->m_Angle) * sx;
            fire->m_pOriginalDamage->m_x_Speed = sx;
            fire->m_pOriginalDamage->m_y_Speed = sy;
        }
    }
    else
    {
        fire->m_X = vPos.x + fX;
        fire->m_Y = vPos.y + fY + m_fDistY;
        fire->m_IsRight = false;
        fire->m_Angle = -fire->m_pOriginalDamage->m_Start_Angle - fStartAngle;
        if (bMoveAngle)
        {
            float sx = fire->m_pOriginalDamage->m_x_Speed;
            float sy = fire->m_pOriginalDamage->m_y_Speed;
            sy = sinf(fire->m_Angle) * sx * -1;
            sx = cosf(fire->m_Angle) * sx;
            fire->m_pOriginalDamage->m_x_Speed = sx;
            fire->m_pOriginalDamage->m_y_Speed = sy;
        }
    }
}
bool KPet::IsPetAvailableState()
{
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
        return false;

    if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME || g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING)
        return true;
    else
        return false;
}

void KPet::UpdateMasterInfo()
{
    if (!m_pOwner)
        return;

    if (KPet::IsPetAvailableState())
    {
        m_pOwner->Update(g_MyD3D->MyPlayer[m_iPlayerNumber]);
    }
    else
    {
        GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople(m_iPlayerNumber);
        if (pPeople != NULL)
        {
            m_pOwner->Update(pPeople);
        }
        else
        {
            GCSquarePeople* pReserve = SiGCSquare()->GetSquareReservePeople(m_iPlayerNumber);
            if (pReserve != NULL)
            {
                m_pOwner->Update(pReserve);
            }
        }
    }
}

KPetOwnerInfo::KPetOwnerInfo(PLAYER* pPlayer)
{
    Update(pPlayer);
}

KPetOwnerInfo::KPetOwnerInfo(GCSquarePeople* pPeople)
{
    Update(pPeople);
}

void KPetOwnerInfo::Update(PLAYER* pPlayer)
{
    if (pPlayer->IsLocalPlayer())
    {
        OwerPlayer = g_kLocalPlayerLatency->kRenderData[0];
    }
    else
    {
        OwerPlayer = *pPlayer;
    }

    bIsContact = pPlayer->IsContact;
    bRender = pPlayer->m_bRender;

    KGCEmbarkObject* pEmbark = pPlayer->GetMyEmbarkInstance();
    if (pEmbark)
    {
        bIsContact = pEmbark->GetIsContact();
    }
}

void KPetOwnerInfo::Update(GCSquarePeople* pPeople)
{
    pOwnerRenderData = pPeople->GetRenderData();
    bIsContact = pPeople->GetContact();
    bRender = pPeople->GetRender();
}

void KPet::CreateObject(bool bAddMgr)
{
    if (!m_pObject)
    {
        if (bAddMgr)
        {
            m_pObject = g_RenderManager->CreateObject(g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).iTechnique);
        }
        else
        {
            m_pObject = new GCObject();
        }

#ifdef _DEBUG
        m_pObject->m_strName = "펫";
#endif
        m_pObject->SetVolume(0.05f);
        m_pObject->SetTechnique(g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).iTechnique);
    }

    m_pObject->SetShellMatrixScale(0.26f);
}

//void KPet::LoadPetModel( int iLocation, DWORD dwCostumeID, GCObject* pObject_ /*= NULL*/, int iPetID /*= -1*/, int iEvolutionLevel /*= -1*/ )
void KPet::LoadPetModel(bool bUseShopTexture, DWORD dwCostumeID, GCObject* pObject_ /*= NULL*/, int iPetID /*= -1*/, int iEvolutionLevel /*= -1*/)
{
    if (pObject_ == NULL) {
        pObject_ = m_pObject;
    }
    // 펫 모델 로드
    std::vector<PetMeshInfo> MeshInfo;
    if (dwCostumeID == 0)
        if (iPetID == -1 && iEvolutionLevel == -1) {
            MeshInfo = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).vecMeshInfo;
        }
        else {
            MeshInfo = g_kGlobalValue.GetPetTemplate(iPetID, iEvolutionLevel).vecMeshInfo;
        }
    else
        MeshInfo = g_kGlobalValue.GetPetCostumeMeshList(dwCostumeID);

    int iMeshSize = (int)MeshInfo.size();
    for (int i = 0; i < iMeshSize; ++i)
    {
        std::string strMesh = MeshInfo[i].strMeshName;
        std::string strTexture;

        if (true == bUseShopTexture)
            strTexture = MeshInfo[i].strShopTextureName;
        else
            strTexture = MeshInfo[i].strTextureName;
        int iCartoon = MeshInfo[i].iCartoonTexture;
        assert(iCartoon > 0 && iCartoon < CARTOON_NUM);
        int iTechnique = MeshInfo[i].iTechnique;
        pObject_->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(strMesh), g_pGCDeviceManager2->CreateTexture(strTexture), (EGCCartoonTexture)iCartoon, "", iTechnique);
    }
}

int KPet::GetPetMotionID()
{
    return m_iPetID;
}

void KPet::LoadPetSingleMotion(int iMotionIndex, const char* strMotion, std::map<int, GCDeviceMotionFRM*>* pmapMotion /*= NULL*/)
{
    GCDeviceMotionFRM* pFrm = g_pGCDeviceManager2->CreateMotionFRM(std::string(strMotion));
    if (pFrm == NULL)
        return;

    if (pFrm->IsDisable()) {
        ASSERT(!"로드하려는 Pet의 모션 파일이 없음!!");
    }
    else {
        SetMotionFrm(iMotionIndex, pFrm, pmapMotion);
    }
}

void KPet::LoadPetMotion(int iLocation, int iPetID, int iEvolutionLevel, std::map<int, GCDeviceMotionFRM*>* pmapMotion)
{
    if (pmapMotion == NULL) {
        pmapMotion = &m_mapMotion;
    }
    std::map<int, GCDeviceMotionFRM*>& mapMotion = *pmapMotion;

    // 펫 모션 로드
    char strMotion[MAX_PATH];
    char strWaitMotion[MAX_PATH];
    char strTempMotion[MAX_PATH];
    char strShopWaitMotion[MAX_PATH];

    sprintf(strWaitMotion, "PET%03d_%d_%02d.FRM", iPetID, iEvolutionLevel, 0);
    sprintf(strTempMotion, "PET%03d_%d_TEMP.FRM", iPetID, iEvolutionLevel);
    sprintf(strShopWaitMotion, "PET%03d_%d_%02d.FRM", iPetID, iEvolutionLevel, 13);

    for (int i = 0; i < EGCPETMOTION::NUM; i++)
    {

        sprintf(strMotion, "PET%03d_%d_%02d.frm", iPetID, iEvolutionLevel, i);

        // 현재 위치에서 사용하는 모션인지 체크 한 뒤..
        // 사용하지 않는 모션이면 그냥 작은 템프모션을 로드해 둔다 ( 로드하는 메모리를 아끼자.. )
        if (IsUseMotion(iLocation, i))
        {
            GCDeviceMotionFRM* pFrm = g_pGCDeviceManager2->CreateMotionFRM(strMotion);

            if (pFrm->IsDisable())
            {
                SAFE_RELEASE(pFrm);
                if (i == EGCPETMOTION::SHOP_ACTION)
                {
                    pFrm = g_pGCDeviceManager2->CreateMotionFRM(strShopWaitMotion);

                    if (pFrm->IsDisable())
                    {
                        SAFE_RELEASE(pFrm);
                        pFrm = g_pGCDeviceManager2->CreateMotionFRM(strWaitMotion);
                    }
                }
                else
                {
                    pFrm = g_pGCDeviceManager2->CreateMotionFRM(strWaitMotion);
                }
            }

            SetMotionFrm(i, pFrm, &mapMotion);
        }
        else
        {
            GCDeviceMotionFRM* pTempFrm = g_pGCDeviceManager2->CreateMotionFRM(strTempMotion);
            SetMotionFrm(i, pTempFrm, &mapMotion);
        }
    }
}

void KPet::SetOwnerInfo(int iPlayerNumber)
{
    if (KPet::IsPetAvailableState())
    {
        KPetInfo kPet;
        if (iPlayerNumber == -1)
        {
            kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
            m_iPlayerNumber = 0;
        }
        else
        {
            kPet = g_MyD3D->MyPlayer[iPlayerNumber]->m_kUserInfo.GetCurrentChar().kPetInfo;
            m_iPlayerNumber = iPlayerNumber;

            std::wstringstream strTemp;
            strTemp << L"LV " << m_iLevel << L". " << m_strPetName;
            g_MyD3D->MyPlayer[iPlayerNumber]->SetPetName(strTemp.str());
        }

        for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++)
        {
            GCItem* kItem = NULL;
            KItem* kInventory = NULL;

            if (g_pItemMgr->FindInventory(kPet.m_vecEquipItem[i].m_dwUID, &kInventory, &kItem) == false)
                continue;

            if (kItem->dwPetEquipItemSlot & EPET_EQUIP_ITEM_SLOT_SKILL)
            {
                if (kInventory->m_nCount > 0)
                {
                    m_bEnableUseSkill = true;
                }
            }
        }
        SAFE_DELETE(m_pOwner);
        m_pOwner = new KPetOwnerInfo(g_MyD3D->MyPlayer[m_iPlayerNumber]);
    }
    else
    {
        m_iPlayerNumber = iPlayerNumber;

        GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople(m_iPlayerNumber);
        if (pPeople != NULL)
        {
            // 광장에서는 스킬쓸일 없다
            m_pOwner = new KPetOwnerInfo(pPeople);
            pPeople->SetPetName(m_strPetName);
        }
        else
        {
            GCSquarePeople* pReserve = SiGCSquare()->GetSquareReservePeople(m_iPlayerNumber);
            if (pReserve != NULL)
            {
                m_pOwner = new KPetOwnerInfo(pReserve);
                pReserve->SetPetName(m_strPetName);
            }
        }
    }
}

void KPet::SetBaseInformation(int iPetType, int iLevel, int iEvolutionLevel, std::wstring strPetName)
{
    // 펫 이름 설정
    swprintf(m_strPetName, PETNICKNAME, strPetName.c_str());

    // 기본 정보들을 세팅한다
    m_iPetID = iPetType;
    m_iEvolutionLevel = iEvolutionLevel;
    m_iLevel = iLevel;

    m_fScale = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fScale;
    m_fDistX = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fDistX;
    m_fDistY = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fDistY;
    m_fNameHeight = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fNameHeight;

    m_fScale *= GetPetScaleFromPetLevel(m_iLevel);
    m_bPetWaitForTag = true;
}


void KPet::SetBasicInfo(int iPlayerNumber, int iPetType, int iLevel, int iEvolutionLevel, int iLocation, std::wstring strPetName, bool bAddToRenderMgr_)
{
    // 펫 이름 설정
    swprintf(m_strPetName, PETNICKNAME, strPetName.c_str());

    // 기본 정보들을 세팅한다
    m_iPetID = iPetType;
    m_iEvolutionLevel = iEvolutionLevel;
    m_iLevel = iLevel;

    m_fScale = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fScale;
    m_fDistX = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fDistX;
    m_fDistY = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fDistY;
    m_fNameHeight = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).fNameHeight;

    m_fScale *= GetPetScaleFromPetLevel(m_iLevel);

    if (!m_pObject)
    {
        if (bAddToRenderMgr_)
        {
            m_pObject = g_RenderManager->CreateObject(g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).iTechnique);
        }
        else
        {
            m_pObject = new GCObject();
        }

#ifdef _DEBUG
        m_pObject->m_strName = "펫";
#endif
        m_pObject->SetVolume(0.05f);
        m_pObject->SetTechnique(g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel).iTechnique);
    }

    m_pObject->SetShellMatrixScale(0.26f);

    SetOwnerInfo(iPlayerNumber);

    m_bPetWaitForTag = true;


}

int KPet::GetMeshSize()
{
    return m_pObject->GetMeshObjectNum();
}

int	KPet::GetNumFrame(int iMotion_)
{
    std::map<int, GCDeviceMotionFRM*>::iterator mit;
    mit = m_mapMotion.find(iMotion_);

    if (mit == m_mapMotion.end())
        return 0;

    return mit->second->GetNum_Frame();
}

CParticleEventSeqPTR KPet::AddParticleWithTrace(const char* strSeqeunce, float fOffsetX, int iParticleNum, bool bReverseTex /*= true */)
{
    CParticleEventSeqPTR pTemp;
    pTemp = g_ParticleManager->CreateSequence(strSeqeunce, vPos.x - 1.0f, vPos.y - 0.48f, 0.5f);

    if (pTemp != NULL)
    {
        // 방향성 세팅
        DIRECTION_INFO stDirect;
        stDirect.m_fDirPosX = -fOffsetX;
        stDirect.m_bDirVelocity = true;
        stDirect.m_pbIsRight = &bIsRight;
        stDirect.m_bReverseTexture = bReverseTex;
        pTemp->SetDirectInfo(&stDirect);
        pTemp->SetShow(true);

        TRACE_INFO stTrace;
        stTrace.m_pvPos = &m_vTracePos;
        //if( fTraceTime == 0.0f )
        stTrace.m_fParticleTraceTime = pTemp->GetKillTime();
        //else
        //	stTrace.m_fParticleTraceTime = fTraceTime;
        pTemp->SetTrace(&stTrace);

        if (iParticleNum != 0)
        {
            CMinMax<float> emitRate(1000.0f, 1000.0f);
            pTemp->SetEmitRate(emitRate);
            pTemp->SetKillNum(iParticleNum);
        }
    }
    return pTemp;
}

void KPet::StartAfterImage()
{
    if (m_pObject)
    {
        g_MyD3D->m_kAfterImageRenderer.RegisterObject(m_pObject);
    }
}

void KPet::EndAfterImage()
{
    if (m_pObject)
    {
        if (g_MyD3D->m_kAfterImageRenderer.IsRegisterObject(m_pObject))
            g_MyD3D->m_kAfterImageRenderer.UnRegisterObject(m_pObject);
    }
}

void KPet::OnReverseGravity(float fRangeX, float fRangeY, float fPosX, float fPosY, float fDamage, bool bFlyCheck, float fHeight)
{
    if (fHeight == 0.0f)
        fHeight = 0.06f;

    float fPetPosX = vPos.x + fPosX;
    float fPetPosY = vPos.y + fPosY;
    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        if (g_MyD3D->IsSameTeam(m_iPlayerNumber, i) == false)
        {
            if (g_MyD3D->MyPlayer[i]->vPos.x > (fPetPosX)-fRangeX && g_MyD3D->MyPlayer[i]->vPos.x<fPetPosX + fRangeX &&
                g_MyD3D->MyPlayer[i]->vPos.y>fPetPosY - fRangeY && g_MyD3D->MyPlayer[i]->vPos.y < fPetPosY + fRangeY
                )
            {
                g_MyD3D->MyPlayer[i]->ReverseGravity(m_iPlayerNumber, fHeight, fDamage / 10.0f, !bFlyCheck);
            }
        }
    }

    if (g_kGlobalValue.m_kUserInfo.bHost)
    {
        fDamage *= 500;
        float fDamageToLarge = fDamage;
        if (usMotion == MID_AMY4_SPECIAL3)
            fDamageToLarge = fDamage;
        // 몬스터도 리버스그라비티!!

        for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
            MONSTER* pMonster = mit->second;

            if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerNumber, mit->first))
                continue;

            if (pMonster->GetX() > fPetPosX - fRangeX && pMonster->GetX() < fPetPosX + fRangeX &&
                pMonster->GetY() > fPetPosY - fRangeY && pMonster->GetY() < fPetPosY + fRangeY &&
                pMonster->IsLive())
            {
                if (bFlyCheck || (!bFlyCheck && pMonster->GetIsContact()))
                {
                    float fRealDamage = fDamage;
                    if (g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].bNoDamageReaction)
                    {
                        fRealDamage = fDamageToLarge;
                    }
                    // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
                    pMonster->m_vDamageStartPos = D3DXVECTOR2(fPetPosX, fPetPosY);
                    pMonster->m_vDamageWhoPos = D3DXVECTOR2(fPetPosX, fPetPosY);
                    //pMonster->OnDamage( m_iPlayerNumber, DT_REVERSE_GRAVITY, fRealDamage, ATTACKTYPE_DOWN, ATTACKDIR_UP, 0.0f, fHeight, true, 0, true );
                    pMonster->OnDamage(m_iPlayerNumber, DT_REVERSE_GRAVITY, fRealDamage, ATTACKTYPE_DOWN, ATTACKDIR_UP, 0.0f, fHeight, false, 0);
                }
            }
        }
    }
}

bool KPet::IsLocalPet()
{
    return g_MyD3D->MyPlayer[m_iPlayerNumber]->IsLocalPlayer();
}

bool KPet::SetMp(float fMp)
{
    if (fMp < 0.0f)
        return false;

    //펫의 MP가 Full이면 리턴
    if (m_fCurrentMP > MAXPETMP)   //MAXPETMP == 1.0f
        m_fCurrentMP = MAXPETMP;

    m_fCurrentMP = fMp;

    return true;
}

float KPet::GetMaxMP()
{
    return MAXPETMP;
}

void KPet::SetIncMP(float fIncMP_)
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    float fIncRate = 1.f;

    if (pPlayer != NULL)
    {
        fIncRate = fIncRate + (fIncRate * (m_iLevel / 100));

        if (pPlayer->m_fAbility[ABILITY_RECOVERY_UP_PET_MP] > 0.f)
        {
            fIncRate = fIncRate + (pPlayer->m_fAbility[ABILITY_RECOVERY_UP_PET_MP] / 100.f);
        }
    }

    m_fIncMP = fIncMP_ * fIncRate;
}

void KPet::LoadPetActionMotion(int iPetID, int iPromotion, GCITEMID skillID /*= 0*/, std::map<int, GCDeviceMotionFRM*>* pmapMotion /*= NULL*/)
{
    if (pmapMotion == NULL) {
        pmapMotion = &m_mapMotion;
    }

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L);

    for (int i = 0; i < EGCPETMOTION::NUM; i++) {

        lua_getglobal(L, "GetPetActionMotion");
        if (lua_isfunction(L, -1) == false) {
            lua_pop(L, 1);
            break;
        }

        lua_pushnumber(L, iPetID);
        lua_pushnumber(L, iPromotion);
        lua_pushnumber(L, skillID);
        lua_pushnumber(L, i);
        if (lua_pcall(L, 4, 1, 0) != 0) {
            lua_pop(L, 1);
        }
        else {
            if (lua_isstring(L, -1) != 0) {
                std::string strFileName = lua_tostring(L, -1);
                LoadPetSingleMotion(i, strFileName.c_str(), pmapMotion);
            }
            lua_pop(L, 1);
        }
    }
}

bool KPet::IsAvailableMotion(int iMotion_)
{
    std::map<int, GCDeviceMotionFRM*>::iterator mit = m_mapMotion.find(iMotion_);
    return mit != m_mapMotion.end();
}

void KPet::SetMotionFrm(int iMotion_, GCDeviceMotionFRM* pFrm_, std::map<int, GCDeviceMotionFRM*>* pmapMotion /*= NULL*/)
{
    if (NULL == pmapMotion) {
        pmapMotion = &m_mapMotion;
    }

    std::map<int, GCDeviceMotionFRM*>::iterator mit = pmapMotion->find(iMotion_);
    if (mit != pmapMotion->end()) {
        SAFE_RELEASE(mit->second);
        mit->second = pFrm_;
    }
    else {
        (*pmapMotion)[iMotion_] = pFrm_;
    }
}

void KPet::ToggleMesh(bool bToggle_)
{
    if (m_pObject) {
        m_pObject->ToggleBaseMeshAll(bToggle_);
        m_pObject->ToggleExtraMeshAll(bToggle_);
    }
}

void KPet::FrameMove_Motion()
{
    if (m_bExistMirrorMotion) {
        if (EGCPETMOTION::INVALID >= usMotion || usMotion >= EGCPETMOTION::NUM)
            return;
    }
    else {
        if (EGCPETMOTION::INVALID >= usMotion || usMotion > EGCPETMOTION::MOVE_END)
            return;
    }

    if (KPet::IsPetAvailableState()) {
        if (g_MyD3D->MyPlayer[m_iPlayerNumber]->IsSkipFrameTime()) {
            return;
        }
    }

    // 모션에서 움직이는 x축 적용
    D3DXMatrixScaling(&g_TempMatScale, m_fScale, m_fScale, m_fScale);
    if (!bIsRight)
    {
        D3DXMatrixRotationY(&g_TempMatRotate, 3.14159f);
        vPos.x -= GetPetMotionFrm(usMotion)->GetFrameData(cFrame).Plus_x * m_fScale;
        m_fMotionX -= GetPetMotionFrm(usMotion)->GetFrameData(cFrame).Plus_x * m_fScale;
    }
    else
    {
        D3DXMatrixRotationY(&g_TempMatRotate, 0.0f);
        vPos.x += GetPetMotionFrm(usMotion)->GetFrameData(cFrame).Plus_x * m_fScale;
        m_fMotionX += GetPetMotionFrm(usMotion)->GetFrameData(cFrame).Plus_x * m_fScale;
    }
    //m_pObject->Scale( );
    D3DXMatrixTranslation(&g_TempMatTranslate, vPos.x - 1.0f, vPos.y + m_fDistY + GetPetMotionFrm(usMotion)->GetFrameData(cFrame).Pos_y * m_fScale - 0.50f, 0.5f);
    g_TempMatWorld = g_TempMatScale * g_TempMatRotate * g_TempMatTranslate;
    m_pObject->SetWorldMat(&g_TempMatWorld);
    m_pObject->SetFrame(&GetPetMotionFrm(usMotion)->GetFrameData(cFrame));
}

void KPet::TogglePetRender(bool bToggle_)
{
    if (IsRenderOnPet() == bToggle_) {
        return;
    }

    PLAYER* pOwner = g_MyD3D->GetPlayer(m_iPlayerNumber);
    if (NULL == pOwner) {
        return;
    }

    bool bTransformEnd = (IsRenderOnPet() == false && bToggle_ == true);
    const SPetTemplate& sPetTemplate = g_kGlobalValue.GetPetTemplate(m_iPetID, m_iEvolutionLevel);
    if (!pOwner->CheckTransformPetSkill() && !bTransformEnd && !sPetTemplate.vecRenderOffEffect.empty()) {
        std::vector< SPetEffectInfo >::const_iterator it = sPetTemplate.vecRenderOffEffect.begin();
        for (; it != sPetTemplate.vecRenderOffEffect.end(); ++it) {
            AddParticle(const_cast<char*>(it->m_strEffectName.c_str()), 0.0, 0, false, it->m_vOffsetPos[0].x, it->m_vOffsetPos[0].y);
        }
    }

    if (!pOwner->CheckTransformPetSkill() && bTransformEnd && !sPetTemplate.vecTransformEndEffect.empty()) {
        std::vector< SPetEffectInfo >::const_iterator it = sPetTemplate.vecTransformEndEffect.begin();
        for (; it != sPetTemplate.vecTransformEndEffect.end(); ++it) {
            pOwner->AddParticleNoDirectionPosWithTrace(const_cast<char*>(it->m_strEffectName.c_str()), 0, it->m_vOffsetPos[0].x, it->m_vOffsetPos[0].y, 1.0f);
        }
    }

    if (bTransformEnd && sPetTemplate.iTransformEndSound != -1) {
        g_KDSound.Play(sPetTemplate.iTransformEndSound, 0);
    }

    ToggleMesh(bToggle_);
    m_bRenderShadow = bToggle_;
    m_bRenderName = bToggle_;
}

void KPet::UseTransformSkill()
{
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_CREATE_LIMIT))
        return;

    if (false == KPet::IsPetAvailableState())
        return;

    if (m_iPlayerNumber < 0)
        return;

    // 플레이어가 죽었으면
    if (g_MyD3D->MyPlayer[m_iPlayerNumber]->GetPlayerState() == PS_DEAD)
        return;


    bIsRight = g_kLocalPlayerLatency->kRenderData[0].bIsRight;

    // 모션을 바꿔주고 패킷을 세팅해 둔다.
    // TODO : 여러종류의 스킬이 존재하므로 스킬의 정보도 담아서 보내야 한다.
    usMotion = EGCPETMOTION::ATTACK;
    cFrame = GetPetMotionFrm(EGCPETMOTION::ATTACK)->GetNum_Frame();
    m_fCurrentMP = m_fCurrentMP;

    g_kUsePetSkill->bUsePetSkill = true;
    g_kUsePetSkill->cPlayerIndex = (char)m_iPlayerNumber;
    g_kUsePetSkill->cPetMotion = (char)usMotion;
    g_kUsePetSkill->PetSkill.bIsRight = bIsRight;
    g_kUsePetSkill->PetSkill.fX = vPos.x;
    g_kUsePetSkill->PetSkill.fY = vPos.y;

    // 리플레이에 펫 스킬 사용하는것 남기기
    g_MyD3D->MyReplay->Set_UsePetSkill(g_kUsePetSkill, g_kUsePetSkill->cPlayerIndex);
}

bool KPet::IsRenderOnPet(void)
{
    return (m_pObject->GetRender() == true) && (m_bRenderShadow == true) && (m_bRenderName == true);
}

void KPet::EnableGlyphEffect(bool bAttack /* = true */)
{
    if (g_MyD3D->m_pStateMachine->GetState() != GS_GAME && m_iPlayerNumber > (MAX_PLAYER_NUM - 1))
    {
        return;
    }

    if (g_MyD3D->MyPlayer[m_iPlayerNumber]->m_kUserInfo.GetCurrentChar().vecPetGlyphInfo.size() == 6)
    {
        if (bAttack)
        {
            AddParticle("UI_Petcircle_Skill_01", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_02", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_03", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_04", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_05", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_06", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_07", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_08", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_09", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_10", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_11", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_12", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_13", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_14", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_15", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_16", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_17", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_18", 0.0f, 0, true, 0.0f, 0.1f);
            AddParticle("UI_Petcircle_Skill_19", 0.0f, 0, true, 0.0f, 0.1f);
        }
        else
        {
            AddParticle("UI_Petcircle_Start_01", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_02", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_03", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_04", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_05", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_06", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_07", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_08", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_09", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_10", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_11", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_12", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_13", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_14", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Petcircle_Start_15", 0.0f, 0, true, 0.0f, 0.3f);
            AddParticle("UI_Pet_5star_start_01", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_02", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_03", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_04", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_05", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_06", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_07", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_08", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_09", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_10", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_11", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_12", 0.0f, 0, true, 0.0f, -0.15f);
            AddParticle("UI_Pet_5star_start_13", 0.0f, 0, true, 0.0f, -0.15f);
        }
    }
}