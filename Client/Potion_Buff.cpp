#include "stdafx.h"
#include "MyD3D.h"
#include "Message.h"
#include "Potion_Buff.h"
#include "buff/KGCBuffManager.h"
#include "GCSKT.h"

#define PLAYPARK_BONUS_BUFF_RATE 1.2f

KPotion_Buff::KPotion_Buff(KItem* _kInven, PLAYER_INFO* _pMyPlayer, SHORTCUT_ITEM* _pStruct)
{
    kItem = (*_kInven);
    nowCoolTime = 0;
    m_MyPlayer = _pMyPlayer;
    m_ItemStruct = _pStruct;
    bRenderDamage = true;
}
KPotion_Buff::~KPotion_Buff()
{}

bool KPotion_Buff::Do()
{
    if (m_ItemStruct->m_setGameModes.find(SiKGCRoomManager()->GetGameMode()) == m_ItemStruct->m_setGameModes.end())
    {
        if (SiKGCWorldMapManager()->IsNotUseShortCutItem(SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetDungeonLevel()))
        {
            return false;
        }
    }

    bool bHP = HP();
    bool bMP = MP();
    bool bAttack = Attack();
    bool bDefense = Defense();
    bool bCancelation = Cancelation();
    bool bSetBuff = SetBuff();
    bool bUseSkill = UseSkill();
    bool bPetMp = PetMP();
    bool bUseSP4Skill = Use4SPSkill();
    bool bUseDamage = UseDamagePotion();
    bool bUseSummon = UseSummonPotion();

    if (bHP == false &&
        bMP == false &&
        bAttack == false &&
        bDefense == false &&
        bCancelation == false &&
        bSetBuff == false &&
        bUseSkill == false &&
        bPetMp == false &&
        bUseSP4Skill == false &&
        bUseDamage == false &&
        bUseSummon == false)
        return false;

    PlaySound();
    PlaySoundAll();
    SetCoolTime();
    DecreaseQuantity();

    return true;
}

bool KPotion_Buff::HP()
{
    if (m_ItemStruct->fHP_RecoveryRate == 0.0)
        return false;
    if (m_MyPlayer->fHP_Point == *(m_MyPlayer->pfHP))
        return false;
    if (g_MyD3D->GetMyPlayer()->GetHP() == 0.0f &&
        (g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
            g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
            g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) ||
            g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_DOWN_IN_SKY)))
        return false;

    float fRecoveryRate = 0.0f;

    if (m_ItemStruct->bPercentageRecovery)
    {
        fRecoveryRate = m_MyPlayer->fHP_Point * m_ItemStruct->fHP_RecoveryRate;
    }
    else if (m_ItemStruct->bLevelDesignRecovery)
    {
#if defined (LEVEL_DESIGN_STAT_CALC)
        fRecoveryRate = m_ItemStruct->fHP_RecoveryRate;
#else
        fRecoveryRate = m_ItemStruct->fHP_RecoveryRate / 100.0f;
#endif
    }
    else
    {
        fRecoveryRate = static_cast<float>((m_ItemStruct->fHP_RecoveryRate - ((static_cast<float>(m_MyPlayer->iLevel) * 1.5) / 100)) * m_MyPlayer->fHP_Point);
    }

    if (fRecoveryRate > m_MyPlayer->fHP_Point)
        fRecoveryRate = m_MyPlayer->fHP_Point;
  
    if (!m_ItemStruct->bLevelDesignRecovery)
    {
        const float fFivePerfect = 5.0f * m_MyPlayer->fHP_Point / 100.0f;
        if (fRecoveryRate < fFivePerfect)
            fRecoveryRate = fFivePerfect;
    }

    if (g_pMagicEffect->IsMagicEffect(g_MyD3D->Get_MyPlayer(), EGC_PLAYPARK_BONUS_BUFF))
    {
        fRecoveryRate *= PLAYPARK_BONUS_BUFF_RATE;
    }

    float fRecoveryRateUp = 1.f;
    if (g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_RECOVERY_UP_POTION] > 0.f)
    {
        fRecoveryRateUp = 1.f + g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_RECOVERY_UP_POTION] * 0.01f;
    }
    else if (g_pMagicEffect->IsMagicEffect(g_MyD3D->GetMyPlayer()->m_iPlayerIndex, EGC_EFFECT_ATTRIBUTE_POTION_UP))
    {
        fRecoveryRateUp = 1.f + g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_FATAL_RECOVERY_POTION_UP] * 0.01f;
    }

    if (GC_GLOBAL_DEFINE::bGuildLevel7)
    {
        if (g_kGlobalValue.GetGuildLevel() >= KGCGlobal::GUILDCL_LEVEL_RECOVERY_UP_POTION)
        {
            fRecoveryRateUp += 0.1f;
        }
    }

    fRecoveryRate *= fRecoveryRateUp;

    g_MyD3D->GetMyPlayer()->IncHP_inFrameMove(fRecoveryRate);

    return true;
}

bool KPotion_Buff::MP()
{
    if (m_ItemStruct->fMP_RecoveryRate == 0.0)
        return false;
    if (*(m_MyPlayer->pfMP) == 3.0f)
        return false;
    if (g_MyD3D->GetMyPlayer()->GetHP() == 0.0f &&
        (g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
            g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
            g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) ||
            g_MyD3D->GetMyPlayer()->uiMotion == g_MyD3D->GetMyPlayer()->GetOwnMotion(MID_COMMON_DOWN_IN_SKY)))
        return false;

    float fRecoveryRate = 0.0f;

    if (m_ItemStruct->bPercentageRecovery)
    {
#if defined( NATION_KOREA ) || defined (NATION_BRAZIL) || defined (NATION_TAIWAN) || defined(NATION_CHINA) || defined( NATION_PHILIPPINE ) 
        fRecoveryRate = g_kGlobalValue.GetMaxMP() * m_ItemStruct->fMP_RecoveryRate;
#else
        fRecoveryRate = 3.0f * m_ItemStruct->fMP_RecoveryRate;
#endif
    }
    else
    {
        fRecoveryRate = static_cast<float>((m_ItemStruct->fMP_RecoveryRate - ((static_cast<float>(m_MyPlayer->iLevel) * 1.5) / 100)) * 3.0f);
    }

#if defined( NATION_KOREA )|| defined (NATION_BRAZIL) || defined (NATION_TAIWAN) || defined(NATION_CHINA) || defined( NATION_PHILIPPINE )
    if (fRecoveryRate > 4.0f)
        fRecoveryRate = 4.0f;
#else
    if (fRecoveryRate > 3.0f)
        fRecoveryRate = 3.0f;
#endif

    if (fRecoveryRate < 0.15f)
        fRecoveryRate = 0.15f;

    if (g_pMagicEffect->IsMagicEffect(g_MyD3D->Get_MyPlayer(), EGC_PLAYPARK_BONUS_BUFF))
    {
        fRecoveryRate *= PLAYPARK_BONUS_BUFF_RATE;
    }

    g_MyD3D->GetMyPlayer()->IncMP_inFrameMove(fRecoveryRate);

    return true;
}

bool KPotion_Buff::PetMP()
{
    if (m_ItemStruct->fPetMp == 0.0f)
        return false;

    if (g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->m_dwSkillGoodsID == 0) {
        return false;
    }

    if (g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->m_fCurrentMP == g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->GetMaxMP())
        return false;

    float fSetPetMp = m_ItemStruct->fPetMp;
    if (g_pMagicEffect->IsMagicEffect(g_MyD3D->Get_MyPlayer(), EGC_PLAYPARK_BONUS_BUFF))
    {
        fSetPetMp *= PLAYPARK_BONUS_BUFF_RATE;
    }

    float fCurrentPetMp = g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->m_fCurrentMP;
    fCurrentPetMp += fSetPetMp;

    fCurrentPetMp = std::min<float>(fCurrentPetMp, g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->GetMaxMP());

    g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->SetMp(fCurrentPetMp);

    return true;

}
bool KPotion_Buff::Attack()
{
    if (m_ItemStruct->fAttack == 0.0)
        return false;

    return true;
}

bool KPotion_Buff::Defense()
{
    if (m_ItemStruct->fDefense == 0.0)
        return false;

    return true;
}

bool KPotion_Buff::Cancelation()
{
    bool bAbnormal = false;
    if (m_ItemStruct->bCancelation == false)
        return false;

    bAbnormal = g_pMagicEffect->SetMagicEffect(g_MyD3D->Get_MyPlayer(), EGC_CANCELATION, 1);

    if (bAbnormal == false) return false;

    return true;
}


void KPotion_Buff::SetCoolTime(INT _CoolTime)
{
    float coolTime = 0;
    if (_CoolTime == (-1))
        coolTime = static_cast<float>(m_ItemStruct->iCoolTime);
    else
        coolTime = static_cast<float>(_CoolTime);

    if (USERLEVEL_ADMIN == g_kGlobalValue.m_kUserInfo.eGCUserLevel)
    {
        coolTime = 0.3f;
    }

    nowCoolTime = static_cast<INT>(SECOND(coolTime));

}

void KPotion_Buff::DecreaseCoolTime(INT _Num)
{
    if (nowCoolTime > 0)
        nowCoolTime -= _Num;
}

bool KPotion_Buff::DecreaseQuantity()
{
    if (kItem.m_nCount != KItem::UNLIMITED_ITEM)
        kItem.m_nCount--;
    else
        return true;

    if (kItem.m_nCount >= 1)
    {
        return true;
    }
    else
        return false;
}

std::string KPotion_Buff::Dump() const
{
    std::stringstream stm;
    stm << "Potion_Buff " << KGCShortCutSlotItem::Dump();
    return stm.str();
}

bool KPotion_Buff::SetBuff()
{
    if (!(m_ItemStruct->iItemType == BUFF_POTION || m_ItemStruct->iItemType == EVENT_POTION))
        return false;

    if (m_ItemStruct->iBuffIndex == -1)
        return false;

    if (m_ItemStruct->iSystemMessage != -1) {
        std::wstring strDropMsg = g_pkStrLoader->GetReplacedString(m_ItemStruct->iSystemMessage, "s", L"#c" + g_MyD3D->GetMyPlayer()->GetStrUserColor() + g_MyD3D->GetMyPlayer()->GetStrUserName().c_str() + L"#cX");
        g_pkChatManager->AddChatMsg(strDropMsg, KGCChatManager::CHAT_TYPE_NORMAL);
    }

    g_pMagicEffect->SetMagicEffect(g_MyD3D->Get_MyPlayer(), m_ItemStruct->iBuffIndex, m_ItemStruct->fBuffTime);
    g_MyD3D->GetMyPlayer()->SetMagicEffect(m_ItemStruct->iBuffIndex, m_ItemStruct->fBuffTime);

    return true;
}

bool KPotion_Buff::UseSkill()
{
    if (SKILL_POTION != m_ItemStruct->iItemType && SKILL_POTION2 != m_ItemStruct->iItemType)
        return false;

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if (NULL == pPlayer || pPlayer->IsCatchMotion(pPlayer->uiMotion) || pPlayer->IsSpecialSkillMotion() ||
        (pPlayer->IsDamagedMotion() ||
            pPlayer->GetJokeMotion() == pPlayer->uiMotion ||
            MID_SKILL_RYAN_NATURESHIELD == pPlayer->uiMotion))
    {
        return false;
    }

    std::map<int, int >::iterator mit = m_ItemStruct->m_mapSkillMotion.find(pPlayer->MetamorphosisForm);

    if (m_ItemStruct->m_mapSkillMotion.end() == mit)
        return false;

    g_MyD3D->GetMyPlayer()->SetPlayerState(mit->second);
    g_MyD3D->GetMyPlayer()->cFrame = 0;

    return true;
}

void KPotion_Buff::PlaySound()
{
    if (!m_ItemStruct->strSoundAll.empty())
        g_KDSound.Play(m_ItemStruct->strSound.c_str());
}

void KPotion_Buff::PlaySoundAll()
{
    if (!m_ItemStruct->strSoundAll.empty())
        g_KDSound.Play(m_ItemStruct->strSoundAll.c_str(), 0, LATENCY_BUFFER_SIZE);
}

bool KPotion_Buff::Use4SPSkill(void)
{
    if (LV4_SPECIAL_SKILL_POTION != m_ItemStruct->iItemType) {
        return false;
    }

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if (NULL == pPlayer || pPlayer->IsCatchMotion(pPlayer->uiMotion) || pPlayer->IsSpecialSkillMotion() ||
        (pPlayer->IsDamagedMotion() || pPlayer->GetJokeMotion() == pPlayer->uiMotion || MID_SKILL_RYAN_NATURESHIELD == pPlayer->uiMotion)) {
        return false;
    }

    int iSkillID = pPlayer->GetCurFormTemplate().SID_SPECIAL4;
    const  GCSkill* pSkill = SiGCSKT()->GetSkill(static_cast<EGCSkillTree>(iSkillID));

    if (NULL == pSkill) {
        return false;
    }

    if (pSkill->m_pHotKeySkill) {
        if (pPlayer->SetPlayerState(pSkill->m_pHotKeySkill->m_iMotion)) {
            return true;
        }
        else {
            return false;
        }
    }

    return false;
}
void KPotion_Buff::SetDamageOREmoticonAppearance(int iRenderState)
{
    switch (iRenderState)
    {
    case 0:
        bRenderDamage = true;
        break;
    case 1:
        bRenderDamage = false;
        break;
    default:
        bRenderDamage = true;
    }
}
bool KPotion_Buff::UseDamagePotion(void)
{
    if (DAMAGE_POTION != m_ItemStruct->iItemType) {
        return false;
    }

    int iPlayerIndex = g_MyD3D->Get_MyPlayer();
    if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex)) {
        return false;
    }

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if (NULL == pPlayer || pPlayer->IsCatchMotion(pPlayer->uiMotion) || pPlayer->IsSpecialSkillMotion() ||
        (pPlayer->IsDamagedMotion() || pPlayer->GetJokeMotion() == pPlayer->uiMotion || MID_SKILL_RYAN_NATURESHIELD == pPlayer->uiMotion)) {
        return false;
    }

    if (bRenderDamage)
    {
        if (pPlayer->bIsRight && m_ItemStruct->iDamageEnumRight != -1) {
            KGC_PID_BROAD_ADD_DAMAGE kPacket;
            kPacket.dwUID = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.dwUID;
            kPacket.iPlayerIndex = iPlayerIndex;
            kPacket.iDamageEnum = m_ItemStruct->iDamageEnumRight;
            SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
            KGCPC_MEMO("KGC_PID_BROAD_ADD_DAMAGE");
            return true;
        }
        else if (pPlayer->bIsRight == false && m_ItemStruct->iDamageEnumLeft != -1) {
            KGC_PID_BROAD_ADD_DAMAGE kPacket;
            kPacket.dwUID = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.dwUID;
            kPacket.iPlayerIndex = iPlayerIndex;
            kPacket.iDamageEnum = m_ItemStruct->iDamageEnumLeft;
            SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
            KGCPC_MEMO("KGC_PID_BROAD_ADD_DAMAGE");
            return true;
        }
    }
    else
        return true;

    return false;
}

bool KPotion_Buff::UseSummonPotion(void)
{
    if (SUMMON_POTION != m_ItemStruct->iItemType) {
        return false;
    }

    int iPlayerIndex = g_MyD3D->Get_MyPlayer();
    if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex)) {
        return false;
    }

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();

    pPlayer->SummonMonster(static_cast<EGCMonster>(m_ItemStruct->dwSummonMonsterID), 0.4f, 0.0f, 0.5f, 1, 99999.0f);

    return true;

}