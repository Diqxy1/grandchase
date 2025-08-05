#pragma section("GCE",read,write,execute)

#include "KGCBuffUnoEarring.h"
#include "stdafx.h"
#include "Controls.h"
#include "Damage.h"
#include "KGCRoomManager.h"

#pragma code_seg("GCE")
KGCBuffUnoEarring::KGCBuffUnoEarring() {

}

#pragma code_seg("GCE")
KGCBuffUnoEarring::~KGCBuffUnoEarring() {

}

#pragma code_seg("GCE")
bool KGCBuffUnoEarring::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
    if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID) && pBuffInst_)
    {
        m_mapStopWatch.insert(std::pair<int, KncStopWatch>(iPlayerIndex_, KncStopWatch()));
        m_mapStopWatch[iPlayerIndex_].SetStopWatch(m_iPeriodTime);

        MAP_HEROITEM_ITER mit = g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.find(pBuffInst_->m_iBuffEnum);

        if (mit != g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.end())
        {
            std::vector<float> vecCondition;
            if (!g_pItemMgr->GetHeroItemCondition(mit->second, &vecCondition))
                return false;

            if (vecCondition.size() > 4)
            {
                m_iPeriodTime = static_cast<int>(vecCondition[0]);
                m_iShieldTime = static_cast<int>(vecCondition[1]);
                m_iDungeonShieldHP = static_cast<int>(vecCondition[2]);
                m_iOtherShieldHP = static_cast<int>(vecCondition[3]);
                m_iProbability = static_cast<int>(vecCondition[4]);
            }
        }
        return true;
    }
    return false;
}

#pragma code_seg("GCE")
bool KGCBuffUnoEarring::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
        return false;

    if (!g_MyD3D->IsPlayerIndex(iPlayerIndex_))
        return false;

    //  버프는 각자 계산해서 알려준ㄴ다.
    if (!g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
        return false;

    // 캐릭터 우노 만
    if (g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType != GC_CHAR_UNO)
        return false;

    if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
    {
        if (m_mapStopWatch[iPlayerIndex_].IsStop())
        {
            srand((unsigned)time(NULL));
            int randomNum = rand() % 100;
            if (randomNum < m_iProbability)
            {
                // 적용하기 전에 효과가 있는지 확인하십시오 ^-^
                if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsMagicEffect(EGC_EFFECT_UNO1_EARRING_BUFF)) {
                    return false;
                }

                // 먼저 효과를 적용하십시오.
                g_MyD3D->MyPlayer[iPlayerIndex_]->SetMagicEffect(EGC_EFFECT_UNO1_EARRING_BUFF, 5);
            }
            m_mapStopWatch[iPlayerIndex_].SetStopWatch(m_iPeriodTime * 1000);
        }
    }
    return true;
}

#pragma code_seg("GCE")
bool KGCBuffUnoEarring::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
    m_mapStopWatch.erase(iPlayerIndex_);
    if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
        return true;
    return false;
}