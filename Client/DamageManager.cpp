#include "stdafx.h"
#include "DamageManager.h"
#include "Spark.h"
#include "KGCPlayerCommon.h"
#include "Monster.h"
#include "Headup Display.h"
#include "Stage.h"
#include "Buff/KGCBuffManager.h"
#include "GCCameraOrtho.h"
#include "Replay.h"
#include "GCO2Manager.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGC3DObject.h"
#include "KGC3DObjectMgr.h"
#include "KGCMinigameManager.h"
#include "KGC3DObjectMgr.h"
#include "KGCState.h"
#include "HardDamage/HardDamageGorgosFire.h"
#include "HardDamage/HardDamageArrow.h"
#include "HardDamage/HardDamageCriticalArrow.h"
#include "HardDamage/HardDamageStoneCurse.h"
#include "HardDamage/HardDamageWaterPole.h"
#include "HardDamage/HardDamageDrillRock.h"
#include "HardDamage/HardDamageMeteo.h"
#include "HardDamage/HardDamageBomb.h"
#include "HardDamage/HardDamagebomb2.h"
#include "HardDamage/HardDamageFireLance.h"
#include "HardDamage/HardDamageSinFly.h"
#include "HardDamage/HardDamageMine.h"
#include "HardDamage/HardDamageFlingShield.h"
#include "HardDamage/HardDamageBlockBuster.h"
#include "HardDamage/HardDamageGas.h"
#include "HardDamage/HardDamageLightningBolt.h"
#include "HardDamage/HardDamageLightningGenerate1.h"
#include "HardDamage/HardDamageLightningGenerate2.h"
#include "HardDamage/HardDamageGuardianStrike.h"
#include "HardDamage/HardDamageGuardianBall.h"
#include "HardDamage/HardDamageRonan4SummonValkyrie.h"
#include "HardDamage/HardDamageManyFruit.h"
#include "HardDamage/HardDamageKazeazeDarkBall.h"
#include "HardDamage/HardDamageMagicStar.h"
#include "HardDamage/HardDamageBombWave.h"
#include "HardDamage/HardDamageBasiliskFire.h"
#include "HardDamage/HardDamagePhoenixArrow.h"
#include "HardDamage/HardDamageCannon.h"
#include "HardDamage/HardDamageIceBolt.h"
#include "HardDamage/HardDamageFireSpot.h"
#include "HardDamage/HardDamageBreeding.h"
#include "HardDamage/HardDamageFire.h"
//
#include "HardDamage/HardDamageWholeStageGas.h"
//
#include "HardDamage/HardDamageElectronBall.h"
#include "HardDamage/HardDamageIcePixieIceStorm.h"
#include "HardDamage/KHardDamagePepeGas.h"
#include "HardDamage/HardDamageBermesiaFinalEarthQuake.h"
#include "HardDamage/HardDamageElesis4Special2.h"
#include "HardDamage/HardDamageElesis4DownAtk.h"
#include "HardDamage/HardDamageBigMeteo.h"
#include "HardDamage/HardDamageElesis4Special2Shoot.h"
#include "HardDamage/HardDamageBigMeteoReady.h"

#include "HardDamage/HardDamageBigLightningReady.h"
#include "HardDamage/HardDamageKamikiLightning.h"
#include "HardDamage/HardDamageLas4DownAtk.h"
#include "HardDamage/HardDamageLas4BounceAtk.h"
#include "HardDamage/HardDamageLas4Special3Sword.h"
#include "HardDamage/KGCHardDamagePoisonBreath.h"
#include "HardDamage/HardDamageOctusWarcry.h"
#include "HardDamage/HardDamageHommingMissle.h"
#include "HardDamage/HardDamageRonanMagicSpecial1.h"
#include "HardDamage/HardDamageRonanMeteo.h"

#include "HardDamage/HardDamageSummonDragon.h"

#include "HardDamage/harddamagebigmeteosolo.h"
#include "HardDamage/HardDamageSummonTempest.h"
#include "HardDamage/HardDamageAmyNoteSinFly.h"
#include "HardDamage/HardDamageSummonInstrument.h"
#include "HardDamage/HardDamageRonan4SwordSpecial3.h"
#include "HardDamage/HardDamageRonan4MonoVolt.h"

#include "HardDamage/HardDamageMonsterMine.h"
// HardDamage �����۾� ����
//#include "HardDamage/HardDamageAssassinRanger.h"
// HardDamage �����۾� ��

#include "HardDamage/HardDamageArmeSkillTest.h"
#include "HardDamage/HardDamageArme1IceStrike.h"
#include "HardDamage/HardDamageArmeSummonGorgos.h"
#include "HardDamage/HardDamageArmeWallOfIce.h"
#include "HardDamage/HardDamagehold.h"
#include "HardDamage/HardDamageSaintBreeding.h"
// HardDamage �����۾� ����
//#include "HardDamage/HardDamageArmeBigBoom.h"
// HardDamage �����۾� ��

#include "HardDamage/HardDamageAmy3UniqueSkill1.h"
#include "HardDamage/HardDamageAmy3Special3.h"

#include "HardDamage/HardDamageJinSpecial3Burnning.h"
#include "HardDamage/HardDamageChargedBolt.h"
#include "HardDamage/HardDamageCrabStone.h"

#include "HardDamage/HardDamageStarDustRain.h"
#include "HardDamage/HardDamageShotMagicCircle.h"
#include "HardDamage/HardDamageMagicMissile.h"
#include "HardDamage/HardDamageSiegeDeco.h"

#include "HardDamage/HardDamageLasTradWarmPot.h"
#include "HardDamage/HardDamageTradWarmTrap.h"

#include "harddamage/HardDamageRyanTotem.h"
#include "harddamage/HardDamageRyanPoisonSpore.h"
#include "harddamage/HardDamageRyanBloody.h"
#include "HardDamage/HardDamageAmy4.h"
#include "HardDamage/HardDamageXenia5MapDamage.h"
#include "HardDamage/HardDamagePeriot.h"
#include "HardDamage/HardDamageXenia7.h"
#include "HardDamage/HardDamageXenia8.h"
#include "HardDamage/HardDamageJin.h"
#include "HardDamage/HardDamageMermairing.h"
#include "HardDamage/HardDamageAmy1SkillTree.h"
#include "HardDamage/HardDamagePet.h"
#include "HardDamage/HardDamageDesert4.h"
#include "HardDamage/HardDamageChaos2.h"
#include "HardDamage/HardDamageChaos3.h"
#include "HardDamage/HardDamageHero1.h"

#include "HardDamage/HardDamageAstarot.h"
#include "HardDamage/HardDamageBakunawa.h"
#include "HardDamage/HardDamageDevilion.h"
#include "HardDamage/HardDamageMari.h"
#include "HardDamage/HardDamageMari2BuildLV2.h"
#include "HardDamageKindPet.h"
#include "HardDamage/HardDamageSelion.h"
#include "HardDamage/HardDamageBlitchen.h"
#include "HardDamage/HardDamageBakunawa.h"
#include "HardDamage/HardDamageAstarot.h"
#include "HardDamage/HardDamageTradWarmTrap.h"
#include "HardDamage/HardDamageLasTradWarmPot.h"
#include "HardDamage/HardDamageShotMagicCircleDun.h"
#include "HardDamage/HardDamageRoseArrow.h"
#include "dbg/dbg.hpp"
//#include <boost/bind.hpp>
#include "KGCRoomManager.h"
#include "KStats.h"
#include "GCUtil/GCFileMoniter.h"
#include "KGCGameBoard.h"
#include "KGCEmbarkManager.h"

#include "Message.h"
#include "Stage.h"
#include "GCGrowingSkillManager.h"
#include "QuestGameManager.h"

#include "GCPetMagicStone.h"

bool bReloadDamage = false;
void InitDamageFileMoniter()
{
    bReloadDamage = true;
}


CDamageManager::CDamageManager(void)
{
    m_apTexture = NULL;
    m_vecInfo.clear();

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
        m_iPlayerIndex[i] = -1;
    g_kMonsterManager.CreateMonsterAttackIndex(m_mapMonsterIndex);
    m_bIsSP75BTexture = false;

    m_mapEraseMonDamageList.clear();
    m_pLuaMgr = new KLuaManager();
}

CDamageManager::~CDamageManager(void)
{
    RemoveAll();

    delete m_pLuaMgr;
}

bool CDamageManager::CheckEnableCreate()
{
    int m_iTotalInstance = 0;
    std::map< int, std::vector<CDamageInstance*> >::iterator itor;
    for (itor = m_mapDamageInstance.begin(); itor != m_mapDamageInstance.end(); ++itor)
    {
        m_iTotalInstance += (int)itor->second.size();
    }

    if (m_iTotalInstance >= MAX_DAMAGE_INSTANCE)
    {
        return false;
    }

    return true;
}

CDamageInstance* CDamageManager::PetAdd(int What, int Who, bool NoSlowCount/* = false*/, int ExtraMonsterNum/* = -1*/, float fAtkRatio /*= 1.0f*/, bool bPetTrace /*=false*/, bool bPlayerTrace /*=false*/)
{
    CDamageInstance* pDamageInstance = NULL;
    pDamageInstance = new CDamageInstance();
    pDamageInstance->m_bPetDamage = true;
    pDamageInstance->m_fRatio = fAtkRatio;
    pDamageInstance->m_bPetTrace = bPetTrace;
    pDamageInstance->m_bPlayerTrace = bPlayerTrace;

    pDamageInstance = Add(What, Who, NoSlowCount, ExtraMonsterNum, pDamageInstance);
    return pDamageInstance;
}

CDamageInstance* CDamageManager::PetAddWithAngle(int What, int Who, float fStartAngle, bool NoSlowCount/* = false*/, int ExtraMonsterNum/* = -1*/, float fAtkRatio /*= 1.0f*/)
{
    CDamageInstance* pDamageInstance = NULL;
    pDamageInstance = new CDamageInstance();
    pDamageInstance->m_bPetDamage = true;
    pDamageInstance->m_fRatio = fAtkRatio;

    pDamageInstance = AddWithAngle(What, Who, fStartAngle, NoSlowCount, ExtraMonsterNum, pDamageInstance);
    return pDamageInstance;
}

CDamageInstance* CDamageManager::PetAddWithLocate(int What, int Who, float fStartX, float fStartY, bool NoSlowCount/* = false*/, int ExtraMonsterNum/* = -1*/, float fAtkRatio /*= 1.0f*/)
{
    CDamageInstance* pDamageInstance = NULL;
    pDamageInstance = new CDamageInstance();
    pDamageInstance->m_bPetDamage = true;

    pDamageInstance = AddWithLocate(What, Who, fStartX, fStartY, NoSlowCount, ExtraMonsterNum, pDamageInstance);
    return pDamageInstance;
}

void CDamageManager::DamagedFireHardcordingPreProcess(int What, const OwnerData& owner, CDamageInstance* pDamageInstance)
{
    // ����Ʈ �����̸� Ư�� ���� �� ����Ʈ�� ��� �Ѵ�.
    if (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory())
    {
        if (pDamageInstance->m_Who < MAX_PLAYER_NUM)
            g_MyD3D->CalcQuestPointByDamageType(pDamageInstance->m_Who, What);
    }

    //�������� ���̾� �ϵ��ڵ� ��ó��
    switch (What)
    {
    case DT_HARPY_FEATHER_ATTACK:
        if (owner.Frame < 40)
            pDamageInstance->m_pOriginalDamage->BeginWithAngle(pDamageInstance, -0.8f + (float)(owner.Frame - 29) * 0.16f);
        else
            pDamageInstance->m_pOriginalDamage->BeginWithAngle(pDamageInstance, -0.8f + (float)(owner.Frame - 54) * 0.16f);
        break;
    case DT_CURE_HEART1:
    case DT_ARME_SKILL_CURE_LV2_HEART:
    {
        for (float Angle = owner.Body_Angle; Angle < 5.783f; Angle += 0.5f)
        {
            AddWithAngle(DT_CURE_HEART1, owner.Who, Angle);
        }
        break;
    }

    case DT_CURE_HEART1_DUN:
    {
        for (float Angle = owner.Body_Angle; Angle < 5.783f; Angle += 0.5f)
        {
            AddWithAngle(DT_CURE_HEART1_DUN, owner.Who, Angle);
        }
        break;
    }

    case DT_ARME_SKILL_CURE_LV2_HEART_DUN:
    {
        for (float Angle = owner.Body_Angle; Angle < 5.783f; Angle += 0.5f)
        {
            AddWithAngle(DT_ARME_SKILL_CURE_LV2_HEART_DUN, owner.Who, Angle);
        }
        break;
    }

    case DT_DOWN_ARROW:
    case DT_DOWN_ARROW_LIGHTNING:
    case DT_DOWN_ARROW_SHINING:
    case DT_DOWN_ARROW_LOVE:
    case DT_DOWN_ARROW_TIME:
    case DT_DOWN_ARROW_LIFE:
    case DT_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_DOWN_ARROW_HALLOWEEN:
    case DT_DOWN_ARROW_ELIA:
    case DT_DOWN_ARROW_PEGASUS:
    case DT_DOWN_ARROW_C_MASTER:
    case DT_DOWN_ARROW_COLISEU:
    case DT_DOWN_ARROW_DUN:
    case DT_DOWN_ARROW_LIGHTNING_DUN:
    case DT_DOWN_ARROW_SHINING_DUN:
    case DT_DOWN_ARROW_LOVE_DUN:
    case DT_DOWN_ARROW_TIME_DUN:
    case DT_DOWN_ARROW_LIFE_DUN:
    case DT_DOWN_ARROW_DARK_DRAGON_RED_DUN:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE_DUN:
    case DT_DOWN_ARROW_HALLOWEEN_DUN:
    case DT_DOWN_ARROW_ELIA_DUN:
    case DT_DOWN_ARROW_PEGASUS_DUN:
    case DT_DOWN_ARROW_C_MASTER_DUN:
    case DT_DOWN_ARROW_COLISEU_DUN:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIGHTNING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_SHINING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LOVE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_TIME:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIFE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_HALLOWEEN:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_ELIA:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_PEGASUS:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_C_MASTER:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_COLISEU:
    {

        float x = owner.x;
        float y = owner.y;

        for (int i = 0; i < 7; i++)
        {
            AddWithLocate(What, pDamageInstance->m_Who, x + (float)i * 0.2f, y + 1.5f + (float)i * 0.3f);
            AddWithLocate(What, pDamageInstance->m_Who, x - (float)i * 0.2f, y + 1.65f + (float)i * 0.3f);
        }
        break;
    }

    case DT_STORM_ARROW:
    case DT_STORM_ARROW_DUN:
    case DT_STORM_ARROW_LIGHTNING:
    case DT_STORM_ARROW_SHINING:
    case DT_STORM_ARROW_LOVE:
    case DT_STORM_ARROW_TIME:
    case DT_STORM_ARROW_LIFE:
    case DT_STORM_ARROW_DARK_DRAGON_RED:
    case DT_STORM_ARROW_DARK_DRAGON_BLUE:
    case DT_STORM_ARROW_HALLOWEEN:
    case DT_STORM_ARROW_ELIA:
    case DT_STORM_ARROW_PEGASUS:
    case DT_STORM_ARROW_COLISEU:
    case DT_STORM_ARROW_C_MASTER:
    case DT_STORM_ARROW_DUN_LIGHTNING:
    case DT_STORM_ARROW_DUN_SHINING:
    case DT_STORM_ARROW_DUN_LOVE:
    case DT_STORM_ARROW_DUN_TIME:
    case DT_STORM_ARROW_DUN_LIFE:
    case DT_STORM_ARROW_DUN_DARK_DRAGON_RED:
    case DT_STORM_ARROW_DUN_DARK_DRAGON_BLUE:
    case DT_STORM_ARROW_DUN_HALLOWEEN:
    case DT_STORM_ARROW_DUN_ELIA:
    case DT_STORM_ARROW_DUN_PEGASUS:
    case DT_STORM_ARROW_DUN_COLISEU:
    case DT_STORM_ARROW_DUN_C_MASTER:

    case DT_LIRE1_SPECIAL4_STORM_ARROW:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_LIGHTNING:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_SHINING:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_LOVE:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_TIME:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_LIFE:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_DARK_DRAGON_RED:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_DARK_DRAGON_BLUE:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_HALLOWEEN:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_ELIA:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_PEGASUS:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_COLISEU:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_C_MASTER:

        pDamageInstance->m_pOriginalDamage->BeginWithAngle(pDamageInstance, (float)(owner.Frame - 9) * 0.10472f - 0.78f);
        break;

    case DT_CRITICAL_ARROW:
    case DT_CRITICAL_ARROW_LIGHTNING:
    case DT_CRITICAL_ARROW_SHINING:
    case DT_CRITICAL_ARROW_LOVE:
    case DT_NOVA_CRITICAL_ARROW:
    case DT_NOVA_CRITICAL_BOLT:
    case DT_NOVA_CRITICAL_ARROW_LIGHTNING:
    case DT_NOVA_CRITICAL_BOLT_LIGHTNING:
    case DT_NOVA_CRITICAL_ARROW_SHINING:
    case DT_NOVA_CRITICAL_BOLT_SHINING:
    case DT_NOVA_CRITICAL_ARROW_DARK:
    case DT_NOVA_CRITICAL_BOLT_DARK:
    case DT_NOVA_CRITICAL_ARROW_LOVE:
    case DT_NOVA_CRITICAL_BOLT_LOVE:
    case DT_NOVA_CRITICAL_ARROW_DEATH:
    case DT_NOVA_CRITICAL_BOLT_DEATH:
    case DT_CRITICAL_ARROW_TIME:
    case DT_NOVA_CRITICAL_ARROW_TIME:
    case DT_NOVA_CRITICAL_BOLT_TIME:
    case DT_CRITICAL_ARROW_LIFE:
    case DT_NOVA_CRITICAL_ARROW_LIFE:
    case DT_NOVA_CRITICAL_BOLT_LIFE:

    case DT_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_RED:

    case DT_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_BLUE:

    case DT_CRITICAL_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_BOLT_HALLOWEEN:

    case DT_CRITICAL_ARROW_ELIA:
    case DT_NOVA_CRITICAL_ARROW_ELIA:
    case DT_NOVA_CRITICAL_BOLT_ELIA:

    case DT_CRITICAL_ARROW_PEGASUS:
    case DT_NOVA_CRITICAL_ARROW_PEGASUS:
    case DT_NOVA_CRITICAL_BOLT_PEGASUS:

    case DT_CRITICAL_ARROW_C_MASTER:
    case DT_CRITICAL_ARROW_COLISEU:
//    case DT_NOVA_CRITICAL_C_MASTER:
  //  case DT_NOVA_CRITICAL_C_MASTER:

        //�������鼭 ��⿡ �ݴ�������� ���.
        if (owner.Now_Motion == MID_LIRE_DASHATK ||
            owner.Now_Motion == MID_LIRE4_DASH_BACK_ATK ||
            owner.Now_Motion == MID_LIRE4_CRITICAL_ATK ||
            owner.Now_Motion == MID_LIRE4_DOUBLE_ATK1 ||
            owner.Now_Motion == MID_LIRE4_SHADOW_ATTACK ||
            owner.Now_Motion == MID_SKILL_LIRE_DOUBLE_DASH_SHOT)
            pDamageInstance->m_IsRight = !pDamageInstance->m_IsRight;
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        break;

    case DT_WITCH_GROUND:
    case DT_WITCH_GROUND_DUN:
        if (owner.Frame < 25)
        {
            float x = owner.x - 1.0f;
            float y = owner.y - 0.2f;

            g_MyD3D->MySparks->Particles(x, y, LATENCY + SHOWLATENCY, 20, 15);
        }
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        break;

    case DT_REVERSE_GRAVITY:
    {
        // ������ �׷���Ƽ�� ��ƼŬ
        float x = owner.x - 1.0f;
        float y = owner.y - 0.2f;
        char MyLatency;
        if (g_MyD3D->MyPlayer[owner.Who]->IsLocalPlayer())
            MyLatency = LATENCY + SHOWLATENCY;
        else
            MyLatency = 0;
        for (int i = 0; i < 5; i++)
        {
            pDamageInstance->Add(DT_DEFENCE, x, y, MyLatency, 4.0f - (float)i * 0.5f, x, y + (float)i * 0.25f, 0.0001f);
        }
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
    }
    break;

    case DT_COUNTER:
        g_MyD3D->MyHead->White_Count = 2 + SHOWLATENCY;
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        pDamageInstance->m_IsRight = true;
        break;
    case DT_DEFENCE:
    case DT_CRITICAL:
    case DT_DOUBLE:
    case DT_AERIAL:
    case DT_BACKATTACK:
    case DT_BLOCKING:
    case DT_GUARD:
    case DT_STUN:
    case DT_JOYSTICK_LEFT:
    case DT_JOYSTICK_RIGHT:
    case DT_JOYSTICK_LEFT_HIGH:
    case DT_JOYSTICK_RIGHT_HIGH:
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        pDamageInstance->m_IsRight = true;
        break;
    case DT_PUMPKINHEAD_MISSILE:
    case DT_SOCCERBALL_PARADOM_MISSILE:
    case DT_PARADOM_MISSILE:
    case DT_KAKIRU_HEAD_MISSILE:
    case DT_LOBSTER_MISSILE:
    case DT_KAZEAZE_DARKBALL:
    case DT_KAZEAZE_DARKBALL_CHILD:
    case DT_KAMIKI_DARK_MAGIC:
    case DT_MONSTER_NEO_LOBSTER_MISSILE:
        if (owner.Frame > 0 && owner.Frame <= 4)
            pDamageInstance->m_Angle = 3.1416f * 2.0f * ((float)(owner.Frame - 1)) / 4.0f + 0.77f;
        else if (owner.Frame > 33 && owner.Frame < 38)
            pDamageInstance->m_Angle = 3.1416f * 2.0f * ((float)(owner.Frame - 34)) / 4.0f;
        else
        {
            pDamageInstance->m_Angle = (float)(owner.Frame - 29) * -0.035f - 3.1416f / 6.0f;
            if (What == DT_PUMPKINHEAD_MISSILE && (owner.Frame % 2 == 0))
            {
                pDamageInstance->m_Angle = (float)(owner.Frame - 28) * -0.035f - 3.1416f / 6.0f + 3.1416f;
            }
        }

        pDamageInstance->m_X = owner.x - cosf(pDamageInstance->m_Angle) * -0.12f;
        pDamageInstance->m_Y = owner.y - sinf(pDamageInstance->m_Angle) * -0.12f + 0.16f;
        break;
    case DT_WITCH_TELEPORT:
    case DT_WITCH_EVASION:
    case DT_LAS_TELEPORT:
    case DT_ARME_SKILL_BACK_TELEPORT:
    case DT_ARME_SKILL_VERTICAL_TELEPORT:
        if (owner.Frame < 15)
            pDamageInstance->m_Life = 100 + 12 * (12 - owner.Frame);
        else if (owner.Frame > 24)
            pDamageInstance->m_Life = 100 + 12 * (owner.Frame - 25);
        else
            pDamageInstance->m_Life = 50;
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        break;
    case DT_THROW_FRUIT:
    case DT_THROW_MANY_FRUIT1:
    case DT_THROW_MANY_FRUIT2:
    case DT_THROW_MANY_FRUIT3:
    case DT_THROW_MANY_FRUIT4:
        pDamageInstance->m_X = owner.x - cosf(pDamageInstance->m_Angle) * -0.12f;
        pDamageInstance->m_Y = owner.y - sinf(pDamageInstance->m_Angle) * -0.12f + 0.16f;
        break;

    case DT_BLOCK_BUSTER:
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        if (pDamageInstance->m_IsRight)
        {
            EffectDust(pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.30f, pDamageInstance, (LATENCY + SHOWLATENCY), true);
        }
        else
        {
            EffectDust(pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.30f, pDamageInstance, (LATENCY + SHOWLATENCY), true);
        }
        break;

    case DT_EAGLE_SITE:
    {
        if (false == g_MyD3D->IsPlayerIndex(owner.Who))
            break;

        //synch�� ������ �ʽ��ϴ�.            
        g_pMagicEffect->SetMagicEffect(owner.Who, EGC_EFFECT_EAGLE_SITE, 30.0f);


        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
    }
    break;

    case DT_BOOST_HEAL2:
    {
        for (int i = 0; i < MAX_PLAYER_NUM; i++)
        {
            //����ִ� ��� �߿�
            if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING) && g_MyD3D->MyPlayer[i]->m_cLife)
            {
                if (g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i) == true)
                {
                    g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_BOOST_HEAL, 30.0f);
                    Add(DT_CURED_BY_BOOST_HEAL1, i);
                }
            }
        }
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
    }
    break;

    case DT_ATHENS_MAKE_RED:
    {
        for (int i = 0; i < MAX_PLAYER_NUM; i++)
        {
            //����ִ� ��� �߿�
            if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING) && g_MyD3D->MyPlayer[i]->m_cLife)
            {
                if (g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i) == true)
                {
                    g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_ATHENS_SWORD, 30.0f);
                    Add(DT_ATHENS_SHOW_SWORD, i);
                }
            }
        }
    }
    break;
    case DT_ATHENS_MAKE_RED_DUN:
    {
        for (int i = 0; i < MAX_PLAYER_NUM; i++)
        {
            //����ִ� ��� �߿�
            if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING) && g_MyD3D->MyPlayer[i]->m_cLife)
            {
                if (g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i) == true)
                {
                    g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_ATHENS_SWORD_DUN, 30.0f);
                    Add(DT_ATHENS_SHOW_SWORD, i);
                }
            }
        }
    }
    break;
    case DT_ARME3_FIRE_SHIELD:
    {
        if (false == g_MyD3D->IsPlayerIndex(owner.Who))
            break;

        g_pMagicEffect->SetMagicEffect(owner.Who, EGC_EFFECT_FIRE_SHIELD, 15.0f);
    }
    break;

    case DT_LAS3_SPECIAL1:
    {
        if (false == g_MyD3D->IsPlayerIndex(owner.Who))
            break;

        g_pMagicEffect->SetMagicEffect(owner.Who, EGC_EFFECT_BACKSTEP, 15.0f);
    }
    break;

    case DT_ELESIS4_SPECIAL2_R:
    case DT_ELESIS4_SPECIAL2_L:
    {
        if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.CheckState(GC_RUS_PLAYING) &&
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_ELESIS4_SPECIAL2)
        {
            pDamageInstance->m_ExtraMonsterNum = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame;
        }
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
    }
    break;

    case DT_ELESIS4_SPECIAL3_PARTICLE_L:
    case DT_ELESIS4_SPECIAL3_PARTICLE_R:
    case DT_ELESIS4_SPECIAL3_SWORD_L:
    case DT_ELESIS4_SPECIAL3_SWORD_R:
    {
        if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.CheckState(GC_RUS_PLAYING) &&
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_ELESIS4_SPECIAL3)
        {
            pDamageInstance->m_ExtraMonsterNum = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame;
        }
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
    }
    break;
    case DT_SKILL_LAS_RAVEN_THROWKNIFE:
        for (int i = 0; i < 5; i++)
        {
            CDamageInstance* pDamage = AddWithAngle(DT_SKILL_LAS_RAVEN_THROWKNIFE, pDamageInstance->m_Who, 0.261f + 0.654498f * i);
            pDamage->m_pOriginalDamage->m_Start_Angle = 0.261f + 0.654498f * i;
        }
        break;
    case DT_SKILL_LAS_RAVEN_THROWKNIFE_2:
        for (int i = 0; i < 5; i++)
        {
            CDamageInstance* pDamage = AddWithAngle(DT_SKILL_LAS_RAVEN_THROWKNIFE_1, pDamageInstance->m_Who, 0.261f + 0.654498f * i);
            pDamage->m_pOriginalDamage->m_Start_Angle = 0.261f + 0.654498f * i;
        }
        break;
    case DT_SKILL_LAS_RAVEN_THROWKNIFE_1:
    {
        for (int i = 0; i < 4; i++)
        {
            CDamageInstance* pDamage = AddWithAngle(DT_SKILL_LAS_RAVEN_THROWKNIFE_1, pDamageInstance->m_Who, 0.589f + 0.654498f * i);
            pDamage->m_pOriginalDamage->m_Start_Angle = 0.589f + 0.654498f * i;
        }

        CDamageInstance* pDamage1 = AddWithAngle(DT_SKILL_LAS_RAVEN_THROWKNIFE_1, pDamageInstance->m_Who, -0.0872f);
        pDamage1->m_pOriginalDamage->m_Start_Angle = -0.0872f;

        CDamageInstance* pDamage2 = AddWithAngle(DT_SKILL_LAS_RAVEN_THROWKNIFE_1, pDamageInstance->m_Who, 3.2288f);
        pDamage2->m_pOriginalDamage->m_Start_Angle = 3.2288f;
    }
    break;
    case DT_SKILL_RYAN_POISON_SPORE_START:
    {
        float x = owner.x;
        float y = owner.y;

        AddWithLocate(DT_SKILL_RYAN_POISON_SPORE, pDamageInstance->m_Who, x, y);
        CDamageInstance* pDamage = AddWithLocate(DT_SKILL_RYAN_POISON_SPORE, pDamageInstance->m_Who, x, y);
        pDamage->m_pOriginalDamage->m_Angle_Speed = -0.1f;
        pDamage = AddWithLocate(DT_SKILL_RYAN_POISON_SPORE, pDamageInstance->m_Who, x, y);
        pDamage->m_pOriginalDamage->m_Angle_Speed = -0.25f;
    }
    break;

    case DT_SKILL_RYAN_POISON_SPORE_START_DUN:
    {
        float x = owner.x;
        float y = owner.y;

        AddWithLocate(DT_SKILL_RYAN_POISON_SPORE, pDamageInstance->m_Who, x, y);
        CDamageInstance* pDamage = AddWithLocate(DT_SKILL_RYAN_POISON_SPORE_DUN, pDamageInstance->m_Who, x, y);
        pDamage->m_pOriginalDamage->m_Angle_Speed = -0.1f;
        pDamage = AddWithLocate(DT_SKILL_RYAN_POISON_SPORE_DUN, pDamageInstance->m_Who, x, y);
        pDamage->m_pOriginalDamage->m_Angle_Speed = -0.25f;
    }
    break;

    case DT_TEMPLEOFDESTROY_MAP_BUFF:
    {
        for (int i = 0; i < MAX_PLAYER_NUM; i++)
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
            if (pPlayer->m_kUserInfo.nUserState != GC_RUS_PLAYING || pPlayer->m_cLife < 1)
                continue;

            if (!g_pMagicEffect->IsMagicEffect(i, EGC_EFFECT_COMMON_ATK_UP) &&
                !g_pMagicEffect->IsMagicEffect(i, EGC_EFFECT_COMMON_DEF_UP) &&
                !g_pMagicEffect->IsMagicEffect(i, EGC_EFFECT_COMMON_VIT_UP))
            {
                g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_COMMON_ATK_UP + rand() % 3, 999999.0f);
            }
        }
    }
    break;
    case DT_WEAPONCHANGE_EFFECT:
    {
        if (!g_MyD3D->IsPlayerIndex(owner.Who))
            break;

        PLAYER* pPlayer = g_MyD3D->MyPlayer[owner.Who];

        switch (pPlayer->Extra_Char_Num)
        {
        case CID_LIRE1:
        case CID_LIRE2:
        case CID_LIRE3:
        case CID_LIRE4:
        case CID_LAS4:
        case CID_RONAN3:
        case CID_AMY2:
        case CID_MARI1:
        {
            pDamageInstance->m_pOriginalDamage->m_iTracePosType = EPT_EFFECT_LHAND;
            for (unsigned int i = 0; i < pDamageInstance->m_pOriginalDamage->m_vecChildParticle.size(); ++i)
            {
                // ���� �� �ֹ̳��̷�!
                pDamageInstance->m_pOriginalDamage->m_vecChildParticle[i].m_fYOffset = 0.23f;
            }
        }
        break;
        default:
            pDamageInstance->m_pOriginalDamage->m_iTracePosType = EPT_EFFECT_RHAND;
            break;

            pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        }
    }
    break;
    default:
        pDamageInstance->m_pOriginalDamage->Begin(pDamageInstance);
        break;
    }
}

OUT CDamageInstance* CDamageManager::Add(int What, int Who, bool NoSlowCount, int ExtraMonsterNum, IN CDamageInstance* pDamageInstance_/* = NULL*/, bool bAlwaysRight /* = false */)
{
    CDamageInstance* pDamageInstance = CreateDamage(What, Who, NoSlowCount, ExtraMonsterNum, pDamageInstance_);

    if (pDamageInstance)
    {
        OwnerData owner;
        owner.Who = Who;
        owner.x = g_MyD3D->MyPlayer[Who]->vPos.x;
        owner.y = g_MyD3D->MyPlayer[Who]->vPos.y;
        owner.Body_Angle = g_MyD3D->MyPlayer[Who]->Body_Angle;
        owner.Now_Motion = g_MyD3D->MyPlayer[Who]->uiMotion;
        owner.Frame = g_MyD3D->MyPlayer[Who]->cFrame;
        DamagedFireHardcordingPreProcess(What, owner, pDamageInstance);
    }

    return pDamageInstance;
}

OUT CDamageInstance* CDamageManager::AddWithAngle(int What, int Who, float fStartAngle, bool NoSlowCount, int ExtraMonsterNum, IN CDamageInstance* pDamageInstance_/* = NULL*/)
{
    CDamageInstance* pDamageInstance = CreateDamage(What, Who, NoSlowCount, ExtraMonsterNum, pDamageInstance_);

    if (pDamageInstance)
        pDamageInstance->m_pOriginalDamage->BeginWithAngle(pDamageInstance, fStartAngle);

    return pDamageInstance;
}

OUT CDamageInstance* CDamageManager::AddWithLocate(int What, int Who, float fStartX, float fStartY, bool NoSlowCount,
    int ExtraMonsterNum, IN CDamageInstance* pDamageInstance_/* = NULL*/,
    int HitWho, DIRECTION_INFO* stDirect, OBJ_TYPE emWhoType,
    OBJ_TYPE emHitWhoType)
{
    CDamageInstance* pDamageInstance = CreateDamage(What, Who, NoSlowCount, ExtraMonsterNum, pDamageInstance_, stDirect);

    if (pDamageInstance)
    {
        pDamageInstance->m_emWhoType = emWhoType;
        pDamageInstance->m_emHitWhoType = emHitWhoType;

        if (HitWho != -1)
        {
            pDamageInstance->m_HitWho = HitWho;
        }
        pDamageInstance->m_pOriginalDamage->BeginWithLocate(pDamageInstance, fStartX, fStartY);

        if (What == DT_TEMPLEOFDESTROY_MAP_BUFF)
        {
            OwnerData owner;
            owner.Who = MAX_PLAYER_NUM;
            DamagedFireHardcordingPreProcess(What, owner, pDamageInstance);
        }
    }

    return pDamageInstance;
}

CDamageInstance* CDamageManager::MonsterAddLastPlayers(int What, int LastPlayeri, int Monsteri, bool NoSlowCount)
{
    CDamageInstance* pDamageInstance = MonsterAdd(What, Monsteri, NoSlowCount);

    if (pDamageInstance)
    {
        // [10/4/2007] breadceo.begin �Ŀ� Player ���� ������
        pDamageInstance->m_Who = LastPlayeri; // Player ��
        pDamageInstance->m_ExtraMonsterNum = Monsteri;
        pDamageInstance->m_IsMonsterDamage = false;
    }

    return pDamageInstance;
}

CDamageInstance* CDamageManager::MonsterAdd(int What, int Monsteri, bool NoSlowCount)
{
    CDamageInstance* pDamageInstance = CreateDamage(What, MAX_PLAYER_NUM, NoSlowCount, Monsteri, NULL);

    MONSTER* pMonster = g_kMonsterManager.GetMonster(Monsteri);
    if (pMonster == NULL) return pDamageInstance;

    if (pDamageInstance)
    {
        OwnerData owner;
        owner.Who = MAX_PLAYER_NUM;
        owner.x = pMonster->m_afX[0];
        owner.y = pMonster->m_afY[0];
        owner.Body_Angle = pMonster->Body_Angle;
        owner.Now_Motion = pMonster->m_aiMotion[0];
        owner.Frame = pMonster->m_aiFrame[0];
        DamagedFireHardcordingPreProcess(What, owner, pDamageInstance);
    }
    return pDamageInstance;
}

CDamageInstance* CDamageManager::MonsterAddWithAngle(int What, int Monsteri, float fStartAngle, bool NoSlowCount)
{
    CDamageInstance* pDamageInstance = CreateDamage(What, MAX_PLAYER_NUM, NoSlowCount, Monsteri, NULL);

    if (pDamageInstance)
        pDamageInstance->m_pOriginalDamage->BeginWithAngle(pDamageInstance, fStartAngle);

    return pDamageInstance;
}

CDamageInstance* CDamageManager::MonsterAddWithLocate(int What, int Monsteri, float fStartX, float fStartY, bool NoSlowCount)
{
    CDamageInstance* pDamageInstance = CreateDamage(What, MAX_PLAYER_NUM, NoSlowCount, Monsteri, NULL);

    if (pDamageInstance)
        pDamageInstance->m_pOriginalDamage->BeginWithLocate(pDamageInstance, fStartX, fStartY);

    return pDamageInstance;
}

CDamageInstance* CDamageManager::MonsterAddWithTargetPos(int What, int Monsteri, float fStartX, float fStartY, float fTartgetPosX, float fTartgetPosY, std::string strAction /*= "" */)
{
    CDamageInstance* pDamageInstance = CreateDamage(What, MAX_PLAYER_NUM, false, Monsteri, NULL);

    if (pDamageInstance)
        pDamageInstance->m_pOriginalDamage->BeginWithLocate(pDamageInstance, fStartX, fStartY);

    float fX = fTartgetPosX - pDamageInstance->m_X;
    float fY = (fTartgetPosY + 0.1f) - pDamageInstance->m_Y;

    // �� �����Ӹ��� �����ұ�?
    int interval = abs((int)(fX / pDamageInstance->m_pOriginalDamage->m_x_Speed));

    // �� �����ӿ� �󸶳� ��������?
    if (pDamageInstance->m_pOriginalDamage->m_fGravity != 0.0f)
    {
        int age;
        float fDropY;

        if (interval % 2 == 0)
            age = (interval + 1) * (interval / 2);
        else
            age = ((interval + 1) * (interval / 2)) + ((interval / 2) + 1);

        fDropY = (age * pDamageInstance->m_pOriginalDamage->m_fGravity) + fY;
        pDamageInstance->m_pOriginalDamage->m_y_Speed = fDropY / interval;
    }
    else
        pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;

    // ��ǥ ������ ������ ������ ������ �浹�� üũ���� �ʰڴ�.
    pDamageInstance->m_pOriginalDamage->m_iNoCheckContactDamage = pDamageInstance->m_pOriginalDamage->m_Start_Life - interval - 1;

    if (strAction != "")
    {
        DAMAGE_TRIGGER kTemp;
        kTemp.TriggerType = 1; //TRIGGER_TIME
        kTemp.val1 = pDamageInstance->m_pOriginalDamage->m_Start_Life - interval;
        kTemp.strTrigger = strAction;
        pDamageInstance->m_pOriginalDamage->m_vecTrigger.push_back(kTemp);
    }

    return pDamageInstance;
}

void CDamageManager::Remove(CDamageInstance* pDamageInstance)
{
    if ( NULL == pDamageInstance )
        return;

    if (-1 < pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_iAfterImage && pDamageInstance->m_p3DObject != NULL )
        g_MyD3D->m_kAfterImageRenderer.UnRegisterObject(pDamageInstance->m_p3DObject->GetGCObject());

    // ������ ������ ���� �޽ø� ���ִ� ����̴�
    if (pDamageInstance->m_pOriginalDamage->m_bToggleMesh)
    {

        if (true == pDamageInstance->m_IsMonsterDamage)
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
            if (pMonster != NULL) {
                pMonster->ToggleExtraMeshAll(true);
                pMonster->ToggleBaseMesh(true);
                pMonster->ToggleMonsterInfo(true);
                pMonster->SetNoTargettingFrame(0);
                pDamageInstance->m_pOriginalDamage->OutToggleMeshDamageEffect(pDamageInstance, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()));
            }
        }
        else if (false == pDamageInstance->m_IsMonsterDamage)
        {
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetMeshToggleState(false);
            D3DXVECTOR3 vPos = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition();
            if (vPos)
                pDamageInstance->m_pOriginalDamage->OutToggleMeshDamageEffect(pDamageInstance, D3DXVECTOR2(vPos.x, vPos.y));
        }
    }

    // ������ ������ ���� ��ȯ���� �޽ø� ���ִ� ����̴�.
    if (pDamageInstance->m_pOriginalDamage->m_iToggleSummonMonsterID && false == pDamageInstance->m_IsMonsterDamage)
    {
        std::vector<MONSTER*> vecMonster = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetMySummonMonster(EGCMonster(pDamageInstance->m_pOriginalDamage->m_iToggleSummonMonsterID));
        std::vector<MONSTER*>::iterator vecIter = vecMonster.begin();
        for (; vecIter != vecMonster.end(); ++vecIter)
        {
            (*vecIter)->ToggleExtraMeshAll(true);
            (*vecIter)->ToggleBaseMesh(true);
            (*vecIter)->ToggleMonsterInfo(true);
            (*vecIter)->SetNoTargettingFrame(0);
            pDamageInstance->m_pOriginalDamage->OutToggleSummonMonsterMeshDamageEffect(pDamageInstance, D3DXVECTOR2((*vecIter)->GetX(), (*vecIter)->GetY()));
        }
    }

    // ���Ͱ� ��ȯ�ǰ� Lock�̵� �÷��̾��϶� ó��
    if (g_kGlobalValue.m_kUserInfo.bHost)
    {
        std::vector<int>::iterator vecSummonIter = pDamageInstance->m_vecSummonMonHitWho.begin();
        for (; vecSummonIter != pDamageInstance->m_vecSummonMonHitWho.end(); ++vecSummonIter)
        {
            if (g_MyD3D->IsPlayerIndex((*vecSummonIter)) &&
                pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_bIsLock)
            {
                PLAYER* pPlayer = g_MyD3D->MyPlayer[(*vecSummonIter)];
                if (pPlayer)
                {
                    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_END kPacket;
                    kPacket.iHitPlayerIdx = (*vecSummonIter);
                    SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
                    KGCPC_MEMO("KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_END");
                }
            }
        }
    }

    // IsCharacterLock���� Lock�ɸ� �÷��̾ �ִٸ� Ǯ���ش�.
    std::vector< DWORD >::iterator vecLockIter = pDamageInstance->m_vecLockChar.begin();
    for (; vecLockIter != pDamageInstance->m_vecLockChar.end(); ++vecLockIter)
    {
        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(*vecLockIter);
        if (pPlayer && pDamageInstance->m_pOriginalDamage->m_bIsCharLock)
        {
            pPlayer->SetLock(false);
        }
    }

    if (pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos.m_bIsSync && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON) {
        for (int i = 0; i < MAX_PLAYER_NUM; i++) {
            if (g_MyD3D->MyPlayer[i]->m_bNoSafePosition == true) {
                g_MyD3D->MyPlayer[i]->m_bNoSafePosition = false;
                g_MyD3D->MyPlayer[i]->m_fFall_Speed = DEAULT_FALL_SPEED;
            }
        }
    }

    if ((0 != pDamageInstance->m_dwIdentifie) && (pDamageInstance->m_pOriginalDamage->m_bDeleteSync))
    {
        KGC_PID_BROAD_DELETE_DAMAGE kPacket;
        kPacket.iWhat = pDamageInstance->m_What;
        if (pDamageInstance->m_IsMonsterDamage)
            kPacket.iWho = pDamageInstance->m_ExtraMonsterNum;
        else
            kPacket.iWho = pDamageInstance->m_Who;
        kPacket.dwIdentifie = pDamageInstance->m_dwIdentifie;
        kPacket.bIsMonsterDamage = pDamageInstance->m_IsMonsterDamage;
        SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE);
        KGCPC_MEMO("KGC_PID_BROAD_DELETE_DAMAGE");
    }

    // �������� �浹���� �ʾ����� ������ �����ش�.
    if (-1 < pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect)
    {
        for (int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop)
        {
            if (g_MyD3D->MyPlayer[iLoop]->IsMagicEffect(pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect, true))
                g_MyD3D->MyPlayer[iLoop]->ClearMagicEffect(pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect, false);
        }

        MAP_INT_PMONSTER::iterator mapIter = g_kMonsterManager.m_mapMonster.begin();
        for (; mapIter != g_kMonsterManager.m_mapMonster.end(); ++mapIter)
        {
            if (mapIter->second->IsMagicEffect(mapIter->first, pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect))
                mapIter->second->ClearMagicEffect(mapIter->first, pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect);
        }
    }

    pDamageInstance->m_pOriginalDamage->End(pDamageInstance);
    SAFE_DELETE(pDamageInstance->m_pOriginalDamage);
    SAFE_DELETE(pDamageInstance);
}

void CDamageManager::RemoveAll()
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor;
    for (itor = m_mapDamageInstance.begin(); itor != m_mapDamageInstance.end(); ++itor)
    {
        for (int j = 0; j < (int)itor->second.size(); ++j)
        {
            Remove(itor->second[j]);
        }

        itor->second.clear();
    }
    m_mapDamageInstance.clear();
}

void CDamageManager::FrameMove()
{
    if (bReloadDamage)
    {
#ifdef __ORIGIN_DEV__
        g_kStatsManager.OnActionScript();
        g_kStatsManager.OnReloadFireInfo();
#endif
        bReloadDamage = false;
    }

    if (g_kGlobalValue.m_bFrameLock && o3 != 0)
        return;

    CDamageInstance* pDamageInstance = NULL;

    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.begin();
    std::list< int > erase_list;

    while (itor != m_mapDamageInstance.end())
    {
        for (int j = (int)itor->second.size() - 1; j >= 0; --j)
        {
            pDamageInstance = itor->second[j];

            // ������ ������ġ ����( Begin ���Ŀ� ��ġ�� �ٲٴ� ��찡 ����.. )
            if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life)
            {
                pDamageInstance->m_vDamageStartPos = D3DXVECTOR2(pDamageInstance->m_X, pDamageInstance->m_Y);
                if (pDamageInstance->m_IsMonsterDamage)
                {
                    MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                    if (pMonster)
                        pDamageInstance->m_vDamageWhoPos = D3DXVECTOR2(pMonster->GetX(), pMonster->GetY());
                }
                else
                    pDamageInstance->m_vDamageWhoPos = D3DXVECTOR2(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetX(), g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetY());
            }

            //���÷��� �ӵ� ������ �� ����.
            ControlVelocityForReplay(pDamageInstance->m_pOriginalDamage, pDamageInstance->m_What);
            int iLifeChange = pDamageInstance->m_pOriginalDamage->m_Life_Change;
            if ((0 == iLifeChange) || // LifeŸ���� ������ �ʴ� ���� ������
                (pDamageInstance->m_Life >= -iLifeChange && iLifeChange < 0) ||
                (pDamageInstance->m_Life <= 255 - iLifeChange && iLifeChange > 0))
            {
                // Hit_per_frame ���ҽ� ó��
                pDamageInstance->CountHitPerFrame();

                // ���� ��ȯüũ
                pDamageInstance->CheckSummonMonster();

                // Ÿ�� ���� ������ Ȯ��
                CheckTargetAutoDamage(pDamageInstance);

                // �������������� ��ũ��ġüũ(��ġ����ó��)
                CheckSyncPosDamageAtCrash(pDamageInstance);

                if (!pDamageInstance->m_SlowCount)
                {
                    if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                        pDamageInstance->m_IsShow = true;
                    else
                        pDamageInstance->m_IsShow = false;

                    //���÷��� ���� ȭ�� ���� �ڵ�.
                    if (g_MyD3D->MyReplay->m_eReplay_Speed == RS_STOP && g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
                        pDamageInstance->m_Life -= pDamageInstance->m_pOriginalDamage->m_Life_Change; //�������� �׻� �����ϱ� ����. 
                    else   //���� ���� �ƴ϶�� ������.
                    {
                        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life) {
                            pDamageInstance->m_pOriginalDamage->m_iWhoStartMotion = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion;
                        }

                        if (pDamageInstance->m_pOriginalDamage->m_bUserFrameDependant &&
                            pDamageInstance->m_pOriginalDamage->m_iWhoStartMotion == g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion) {
                            int iLoopCount = 0;
                            if (pDamageInstance->m_IsMonsterDamage)
                            {
                                MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);
                                if (pMonster != NULL) {
                                    iLoopCount = pMonster->m_iProcessedFrame;
                                }
                            }
                            else
                            {
                                iLoopCount = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_iProcessedFrame;
                            }

                            if (pDamageInstance->m_p3DObject != NULL)
                                pDamageInstance->m_p3DObject->SetSlowCount(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Slow_Count);

                            TRACE(L"cFrame = %d  Slow_Count = %c  LoopCount = %d\n", g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame, g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Slow_Count, iLoopCount);

                            if (iLoopCount <= 0) {
                                pDamageInstance->m_Life -= pDamageInstance->m_pOriginalDamage->m_Life_Change;
                            }

                            for (pDamageInstance->m_iLoopNum = 0; pDamageInstance->m_iLoopNum < iLoopCount; pDamageInstance->m_iLoopNum++) {
                                pDamageInstance->m_pOriginalDamage->FrameMove(pDamageInstance);
                            }
                        }
                        else {
                            pDamageInstance->m_pOriginalDamage->FrameMove(pDamageInstance);
                        }

                        // ������ ��ǿ����� �����Ǵ� ������
                        if (0 < int(pDamageInstance->m_pOriginalDamage->m_kOnlyMotionDamage.m_vecCondition.size()))
                        {
                            pDamageInstance->m_pOriginalDamage->CheckOnlyMotionDamage(pDamageInstance);
                        }

                    }

                    if (pDamageInstance->m_pOriginalDamage->m_IsEnableDamage &&
                        (!o5 || CheckDamagePerFrame(pDamageInstance) || IsCrashCheckPerFrame(pDamageInstance)) &&
                        !IsDetermineWin)
                    {
                        pDamageInstance->SetCrashCheck(false);

                        CrashWithPlayer(pDamageInstance);

                        CrashWith2DObject(pDamageInstance);
                        CrashWithMinigameObj(pDamageInstance);

                        //CrashWithWeapon( pDamageInstance );						

                        CrashWithMonsterAttack(pDamageInstance);

                        // ���ʹ� �⺻������ -1 ���̱� ������ �÷��̾�ʹ� �ٸ� ���̴�.
                        // �׷��� ���͵� ���� ����� ���� ���� �� �ִ�.
                        CrashWithMonster(pDamageInstance);

                        if (pDamageInstance->m_Who < MAX_PLAYER_NUM)
                        {
                            CrashWithObject(pDamageInstance);     // Object�� ���Ͱ� ���� �� ����.
                        }
                        else if (pDamageInstance->m_What == DT_PRIEST_CURE ||
                            pDamageInstance->m_What == DT_WOODEN_HEALING_RAIN)
                            CrashWithMonster(pDamageInstance);
                    }

                    // Ȥ ���������� �𸣴� o3���� üũ����
                    if ((g_MyD3D->MyReplay->m_eReplay_Speed != RS_STOP || g_MyD3D->MyReplay->m_iCurrState != RP_VIEW) && !o3) //���÷��� ���� �߿��� eff�θ� �ʿ� ����.
                        CrashWithEffect(pDamageInstance);

                    CallEffAtTime(pDamageInstance);
                    CallEffAtGroundCrash(pDamageInstance);

                    // ��Ÿ��忡�� ������ ��� ���������� ���� �̴� ������ ������� ���Ѵ�.
                    if (GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory())
                    {
                        CrashWithUpGround(pDamageInstance, 0.0f, 0.5f);
                    }

                    pDamageInstance->m_Life += pDamageInstance->m_pOriginalDamage->m_Life_Change;
                }
                else
                    pDamageInstance->m_SlowCount--;

                // �÷��̾ ������Ų �͸� ��������.
                if (pDamageInstance->m_IsMonsterDamage == false &&
                    pDamageInstance->m_Who < MAX_PLAYER_NUM &&
                    pDamageInstance->m_pOriginalDamage->m_bTogetherDie) {

                    PLAYER* pPlayer = g_MyD3D->GetPlayer(pDamageInstance->m_Who);
                    if (pPlayer && pPlayer->IsFatal() == false && pPlayer->GetHP() == 0.f) {
                        pDamageInstance->m_Life = 0;
                    }
                }
                else if (pDamageInstance->m_IsMonsterDamage && pDamageInstance->m_pOriginalDamage->m_bTogetherDie) {
                    MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);

                    if (pMonster && pMonster->GetHP() <= 0.f) {
                        pDamageInstance->m_Life = 0;
                    }
                }

                // ���� �׿��� �ϴ� �������� �ִٸ� �˻� �� ���� �׿�����.
                if (pDamageInstance->m_Life <= 0 && pDamageInstance->m_pOriginalDamage->m_vecTogetherDieDamage.empty() == false) {
                    for (int i = 0; i < static_cast<int>(pDamageInstance->m_pOriginalDamage->m_vecTogetherDieDamage.size()); i++) {
                        for (std::map< int, std::vector<CDamageInstance*> >::iterator mit = m_mapDamageInstance.begin(); mit != m_mapDamageInstance.end(); ++mit) {
                            for (std::vector<CDamageInstance*>::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
                                if (pDamageInstance->m_Who == (*vit)->m_Who && pDamageInstance->m_pOriginalDamage->m_vecTogetherDieDamage[i] == (*vit)->m_What) {
                                    (*vit)->m_Life = 0;
                                    continue;
                                }
                            }
                        }
                    }
                }

                if (pDamageInstance->m_Life <= 0 && pDamageInstance->m_pOriginalDamage->m_vecTogetherDieBuff.empty() == false &&
                    g_MyD3D->GetPlayer(pDamageInstance->m_Who)) {
                    if (g_MyD3D->GetPlayer(pDamageInstance->m_Who)->IsLocalPlayer()) {
                        std::vector< int >::iterator vit = pDamageInstance->m_pOriginalDamage->m_vecTogetherDieBuff.begin();
                        for (vit; vit != pDamageInstance->m_pOriginalDamage->m_vecTogetherDieBuff.end(); ++vit) {
                            g_MyD3D->GetPlayer(pDamageInstance->m_Who)->ClearMagicEffect(*vit);
                        }
                    }
                }
            }
            else
            {
                static int iDeleted = 0;

                if (pDamageInstance->m_What == DT_FIRE)
                {
                    iDeleted++;

                    printf("DT_FIRE Removed!!!! :----------- %d ------ Times!!!!!!!!!!!!!!!!!\n", iDeleted);
                }
                itor->second.erase(itor->second.begin() + j);
                Remove(pDamageInstance);
            }
        } //end of for        

        if (itor->second.empty())
            erase_list.push_back(itor->first);

        ++itor;
    } //end of for

    while (!erase_list.empty())
    {
        m_mapDamageInstance.erase(erase_list.back());
        erase_list.pop_back();
    }

    // ���� ������ ��������
    std::map< std::pair<int, int>, int >::iterator mapIter = m_mapEraseMonDamageList.begin();
    for (; mapIter != m_mapEraseMonDamageList.end(); )
    {
        --mapIter->second;
        if (0 >= mapIter->second)
        {
            DeleteMonsterDamage(mapIter->first.first, mapIter->first.second);
            mapIter = m_mapEraseMonDamageList.erase(mapIter);
        }
        else
        {
            ++mapIter;
        }
    }


}


int CDamageManager::SetBuffer(SLVERTEX* v, std::map< int, std::vector<CDamageInstance*> >::iterator itor)
{
    CDamageInstance* pDamageInstance = NULL;

    // ����Ʈ ��������
    static float fAngle = 0.0f;
    int rendersize = 0;
    int iVertexOffset = 0;
    D3DXMATRIX  mat1, mat2, mat3;

    for (int j = 0; j < (int)itor->second.size(); ++j)
    {
        pDamageInstance = itor->second[j];

        if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
            pDamageInstance->m_IsShow = true;
        else
            pDamageInstance->m_IsShow = false;

        if (!pDamageInstance->m_IsShow)
            continue;

        if (0 < pDamageInstance->m_TextureNoRenderCnt)
        {
            --pDamageInstance->m_TextureNoRenderCnt;
            continue;
        }

        // �ʱ�ȭ
        v[iVertexOffset].SetSLVER(-1.0f, 1.0f, 0, 0.01f, 0.01f);
        v[iVertexOffset + 1].SetSLVER(1.0f, 1.0f, 0, 0.99f, 0.01f);
        v[iVertexOffset + 2].SetSLVER(-1.0f, -1.0f, 0, 0.01f, 0.99f);
        v[iVertexOffset + 3].SetSLVER(1.0f, -1.0f, 0, 0.99f, 0.99f);

        //tu, tv����
        if (pDamageInstance->m_IsRight || pDamageInstance->m_pOriginalDamage->m_bAddNoDirection)
        {
            v[iVertexOffset].tu = 0.01f;
            v[iVertexOffset + 1].tu = 0.99f;
            v[iVertexOffset + 2].tu = 0.01f;
            v[iVertexOffset + 3].tu = 0.99f;
        }
        else
        {
            v[iVertexOffset].tu = 0.99f;
            v[iVertexOffset + 1].tu = 0.01f;
            v[iVertexOffset + 2].tu = 0.99f;
            v[iVertexOffset + 3].tu = 0.01f;
        }

        int iLife = pDamageInstance->m_Life;

        if (static_cast<int>(m_vecInfo.size()) <= pDamageInstance->m_What)
            continue;

        CDamage* pDamage = pDamageInstance->m_pOriginalDamage;//m_vecInfo[pDamageInstance->m_What];

        switch (pDamageInstance->m_What)
        {
        case DT_THROW_MANY_FRUIT1:
        case DT_THROW_MANY_FRUIT2:
        case DT_THROW_MANY_FRUIT3:
        case DT_THROW_MANY_FRUIT4:
        case DT_THROW_FRUIT:
        case DT_MONSTER_ENT03:
        {
            switch (pDamageInstance->m_What)
            {
            case DT_THROW_MANY_FRUIT1:
            case DT_THROW_FRUIT:
            {
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.5f;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.5f;
                break;
            }
            case DT_THROW_MANY_FRUIT2:
            {
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.5f;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.5f;
                break;
            }
            case DT_THROW_MANY_FRUIT3:
            {
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.5f;
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.5f;
                break;
            }
            case DT_THROW_MANY_FRUIT4:
            {
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.5f;
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.5f;
                break;
            }
            case DT_MONSTER_ENT03:
            {
                if (pDamageInstance->m_IsRight)
                {
                    v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.5f;
                    v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.5f;
                }
                else
                {
                    v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.5f;
                    v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.5f;
                }
            }
            }

            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        }
        case DT_STONE_BODY:
        case DT_STONE_BODY_ICE:
        case DT_WOLF_STONE_BODY_ICE:
        case DT_WOLF_STONE_BODY:
        case DT_NEPHILIM_STONE_BODY:
        case DT_NEPHILIM_STONE_BODY_ICE:
        case DT_SPIDERQUEEN_WEB_BODY:
            v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.01f + (float)iLife * 0.00653f;

            //������ ����
            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x * (float)(150 - pDamageInstance->m_Life) / 150.0f, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.003f, pDamageInstance->m_Y - 0.52f + (float)(150 - pDamageInstance->m_Life) * 0.0006f - 0.09f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_LIGHTNING_BOLT:
        case DT_LIGHTNING_BOLT_BLUE:
        case DT_ARME_SKILL_SHOCKSTURN_LV2_ATK1:
        case DT_ARME_SKILL_SHOCKSTURN_LV1_ATK1_DUN:
        case DT_HATSHEPSUT_LIGHTNING_BOLT:
        case DT_ARME_SKILL_SHOCKSTURN_LV2_EFFECT:
        case DT_LICHBOSS_LIGHTNINGBOLT:
        case DT_LICH_LIGHTNING_BOLT:
            v[iVertexOffset].tv = v[iVertexOffset + 1].tv =
                ((float)(unsigned char)(RANDOM_NUMBER * 4.0f)) * 0.25f;

            v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = v[iVertexOffset].tv + 0.25f;

            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_METEO_CRASH_WAVE:
        case DT_METEO_CRASH_WAVE_ICE:
        case DT_METEO_CRASH_WAVE_SHINING:
        case DT_METEO_CRASH_WAVE_LOVE:
        case DT_METEO_CRASH_WAVE_LIFE:
        case DT_BOMB_WAVE:
        case DT_CHEROKI_BOMB_WAVE:
        case DT_MONSTER_BOMB_WAVE:
        case DT_CANNON_WAVE:
        case DT_ARCHIMIST_BUSTER_WAVE:
        case DT_KAZEAZE_LIGHTNING_WAVE:
        case DT_ELESIS4_SPECIAL3_CRASH_WAVE:
        case DT_WIND_BLADE_CRASH_WAVE_ICE:
        case DT_METEO_CRASH_WAVE_ELIA:
        case DT_METEO_CRASH_WAVE_C_MASTER:
        case DT_METEO_CRASH_WAVE_COLISEU:
        case DT_WIND_BLADE_CRASH_WAVE:
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, iLife, iLife, iLife);

            v[iVertexOffset].tv = v[iVertexOffset + 1].tv = (float)o3 / 3.0f;
            v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = v[iVertexOffset].tv + 0.33f;

            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * 0.33f, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_CONFUSION:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        case DT_FIREBALL_DARK:
            if (iLife <= 25)
            {
                iLife *= 10;
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(255, iLife, iLife, iLife);
            }
            v[iVertexOffset].tv = v[iVertexOffset + 1].tv = (float)(o10 / 4) * 0.2f;
            v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = v[iVertexOffset].tv + 0.2f;

            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * 0.4f, 1.0f);

            // ������ ���� �ؽ��� ȸ����Ų��
            if (pDamageInstance->m_IsRight)
            {
                D3DXMatrixRotationZ(&mat2, pDamageInstance->m_Angle + 3.14f);
            }
            else
            {
                D3DXMatrixRotationZ(&mat2, pDamageInstance->m_Angle - 3.14f);
            }

            D3DXMatrixMultiply(&mat1, &mat1, &mat2);

            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_GAIKOZSLASH:
            if (iLife <= 25)
            {
                iLife *= 10;
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(255, iLife, iLife, iLife);
            }
            v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.0f;
            v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 1.0f;

            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * 1.2f, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;

        case DT_BERMESIA_FINAL_FIRE_BALL_LV1:
        case DT_BERMESIA_FINAL_FIRE_BALL_LV2:
        case DT_BERMESIA_FINAL_FIRE_BALL_LV3:
        case DT_BERMESIA_FINAL_FIRE_BALL_LV4:
        case DT_BERMESIA_FINAL_FIRE_BALL_LV5:
        case DT_FIREBALL:
        case DT_KAMIKI_DEFENCE_CIRCLE:
        case DT_ARME4_FIREBALL:
        case DT_ABORINGINELORD_DARKBALL:
        case DT_FIRESPIDER_FIREBALL:
            if (iLife <= 25)
            {
                iLife *= 10;
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(255, iLife, iLife, iLife);
            }
            v[iVertexOffset].tv = v[iVertexOffset + 1].tv = (float)(o10 / 4) * 0.2f;
            v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = v[iVertexOffset].tv + 0.2f;


            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * 0.4f, 1.0f);
            if (pDamageInstance->m_What == DT_ARME4_FIREBALL)
            {
                if (pDamageInstance->m_IsRight)
                {
                    D3DXMatrixRotationZ(&mat2, 3.14159f / 4.0f * 3.0f);
                }
                else
                {
                    D3DXMatrixRotationZ(&mat2, -3.14159f / 4.0f * 3.0f);
                }

                D3DXMatrixMultiply(&mat1, &mat1, &mat2);
            }

            if (pDamageInstance->m_What == DT_ABORINGINELORD_DARKBALL) {
                D3DXMatrixRotationZ(&mat2, 3.14159f);
                D3DXMatrixMultiply(&mat1, &mat1, &mat2);
            }

            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);

            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;

        case DT_POTAL_CIRCLE:
        {
            int State;
            State = iLife / 29;

            if (State > 5)
            {
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.666f;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.999f;
            }
            else if (State > 2)
            {
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.333f;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.666f;
            }
            else
            {
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.333f;
            }

            switch (State)
            {
            case 6:
            case 3:
            case 0:
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0;
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.333f;
                break;
            case 7:
            case 4:
            case 1:
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.333f;
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.666f;
                break;
            case 8:
            case 5:
            case 2:
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.666f;
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.999f;
            }

        }
        D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
        D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
        D3DXMatrixMultiply(&mat1, &mat2, &mat3);
        D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        D3DXMatrixMultiply(&mat3, &mat1, &mat2);
        D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

        break;
        case DT_WATER_POLE:

            if (iLife % (-pDamage->m_Life_Change * 4) < 20)
            {
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.01f;
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.49f;
            }
            else
            {
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.51f;
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.99f;
            }
            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * 3.0f, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_KAZEAZE_FIRE:
            iLife = iLife % 12;
            if (iLife < 3 || (iLife > 5 && iLife < 9))
            {
                if (!pDamageInstance->m_IsRight)
                {
                    v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.5f;
                    v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0;
                }
                else
                {
                    v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0;
                    v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.5f;
                }
            }
            else
            {
                if (!pDamageInstance->m_IsRight)
                {
                    v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 1.0f;
                    v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.5f;
                }
                else
                {
                    v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.5f;
                    v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 1.0f;
                }
            }

            if (iLife < 6)
            {
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.5f;
            }
            else
            {
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.5f;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 1.0f;
            }

            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - .5f, .5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;

        case DT_PUMPKINHEAD_MISSILE:
        case DT_SOCCERBALL_PARADOM_MISSILE:
        case DT_PARADOM_MISSILE:
        case DT_KAZEAZE_DARKBALL:
        case DT_KAZEAZE_DARKBALL_CHILD:
        case DT_KAMIKI_DARK_MAGIC:
        case DT_KAKIRU_HEAD_MISSILE:
        case DT_LOBSTER_MISSILE:
        case DT_MONSTER_NEO_LOBSTER_MISSILE:

            iLife = iLife % 12;
            if (iLife < 3 || (iLife > 5 && iLife < 9))
            {
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0;
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 0.5f;
            }
            else
            {
                v[iVertexOffset].tu = v[iVertexOffset + 2].tu = 0.5f;
                v[iVertexOffset + 1].tu = v[iVertexOffset + 3].tu = 1.0f;
            }

            if (iLife < 6)
            {
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 0.5f;
            }
            else
            {
                v[iVertexOffset].tv = v[iVertexOffset + 1].tv = 0.5f;
                v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = 1.0f;
            }

            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - .5f, .5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_MAGIC_STAR:
        case DT_MAGIC_STAR_DUN:
        {
            int iAlpha = LifeToAlpha(iLife, 50);
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, iAlpha, iAlpha, iAlpha);

            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
        }
        break;
        case DT_DEADLY_SHARP_CHAR:
        {
            int iAlpha = LifeToAlpha(iLife, 50);
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, iAlpha, iAlpha, iAlpha);
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        }
        case DT_TORNADO_SPIN:
        {
            int iAlpha = LifeToAlpha(iLife, 50);
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, iAlpha, iAlpha, iAlpha);
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        }
        case DT_BOMB:
        case DT_BOMB2:
        case DT_MONSTER_MINE:
        case DT_CHEROKI_BOMB:
        case DT_AMY4_CANNON_BALL:
        case DT_AMY4_CANNON_BALL_GROUND:
            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_MINE:
        case DT_ARME2_MINE:
        case DT_ARME2_SPECIAL_MINE:
            if (!((g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsLocalPlayer()) ||
                (o3 < 2 && GC_GMT_ALONE_FIGHT != SiKGCRoomManager()->GetGameModeType() &&
                    pDamageInstance->m_iTeam == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.iTeam)))
            {
                pDamageInstance->m_IsShow = false;
                continue;
            }

            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        case DT_AGG_HERE:
        case DT_HARPY_DOWN:
        case DT_FLASK:
        case DT_FLING_SHILED:
        case DT_SOUL:
            //case DT_RONAN3_RUNE_SHEILD:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        case DT_DARK_POISON_GAS:
            if (iLife <= 25)
            {
                iLife *= 4;
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(iLife, 255, 255, 255);
            }
            else
            {
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(100, 255, 255, 255);
            }
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);

            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        case DT_PHOENIX_ARROW:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        case DT_PHOENIX_ARROW_FIRE:
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, 127, 127, 127);

            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x
                , (pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x) + (255 - (pDamageInstance->m_Life / 2)) * 0.003f
                , 1.0f);

            fAngle += 0.9f;
            if (fAngle >= 2 * D3DX_PI) fAngle = 2 * D3DX_PI - fAngle;
            D3DXMatrixRotationY(&mat3, fAngle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixRotationZ(&mat2, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat1, &mat2);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.75f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        case DT_PHOENIX_ARROW_WAVE2:
        case DT_PHOENIX_ARROW_WAVE1:
            if (iLife >= (int)(255 + pDamageInstance->m_pOriginalDamage->m_Life_Change * 5.5f * 2.0f))
            {
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(128, 255, 255, 255);
            }
            else
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(0, 255, 255, 255);

            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);

            // 0.1s ����
            if (iLife >= (int)(255 + pDamageInstance->m_pOriginalDamage->m_Life_Change * 5.5f)/*255 - 12*/)
                pDamageInstance->m_Y += 0.01f;

            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        case DT_ARME3_ICEBALL:
            if (iLife <= 25)
            {
                iLife *= 10;
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(255, iLife, iLife, iLife);
            }
            v[iVertexOffset].tv = v[iVertexOffset + 1].tv = (float)(o10 / 4) * 0.2f;
            v[iVertexOffset + 2].tv = v[iVertexOffset + 3].tv = v[iVertexOffset].tv + 0.2f;


            D3DXMatrixScaling(&mat1, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * 0.4f, 1.0f);

            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        case DT_INSECTIVOROUS_BREEDING:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, 255, 255, 255);
            break;
        case DT_LAS4_SPECIAL3_SWORD_R:
        case DT_LAS4_SPECIAL3_SWORD_L:
        case DT_LAS4_SPECIAL3_FIRE_SWORD_L:
        case DT_LAS4_SPECIAL3_FIRE_SWORD_R:
        case DT_LAS4_SPECIAL3_DARK_SWORD_L:
        case DT_LAS4_SPECIAL3_DARK_SWORD_R:
        case DT_LAS4_SPECIAL3_LIGHT_SWORD_R:
        case DT_LAS4_SPECIAL3_LIGHT_SWORD_L:
        case DT_LAS4_SPECIAL3_ICE_SWORD_R:
        case DT_LAS4_SPECIAL3_ICE_SWORD_L:
        case DT_LAS4_SPECIAL3_LOVE_SWORD_R:
        case DT_LAS4_SPECIAL3_LOVE_SWORD_L:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, 0.045f, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, 255, 255, 255);
            break;
        case DT_PIXIE_ICE1:
        case DT_PIXIE_ICE2:
        case DT_PIXIE_ICE3:
        case DT_PIXIE_ICE4:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(50, 255, 255, 255);
            break;
        case DT_ELESIS4_SPECIAL2_R:
        case DT_ELESIS4_SPECIAL2_L:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            if (iLife <= 8)
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(iLife * 31, iLife * 31, iLife * 31, iLife * 31);
            else
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                D3DCOLOR_ARGB(255, 255, 255, 255);
            break;
            //case DT_ELESIS4_DOWN_ATK:
            //    D3DXMatrixScaling ( &mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f );
            //    D3DXMatrixRotationZ( &mat3, pDamageInstance->m_Angle );
            //    D3DXMatrixMultiply( &mat1, &mat2, &mat3 );
            //    D3DXMatrixTranslation( &mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f );
            //    D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
            //    D3DXMatrixMultiply( &mat1, &mat3, &g_kCamera.m_matCamera );

            //    v[iVertexOffset].Diff = v[iVertexOffset+1].Diff = v[iVertexOffset+2].Diff = v[iVertexOffset+3].Diff = 
            //        D3DCOLOR_ARGB(153, 255, 255, 255);
            //    break;
        case DT_RONAN_SWORD_SPECIAL3_SWORD:
        case DT_RONAN_SWORD_SPECIAL3_SWORD_DUN:
            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);

            //// ���鼭 �ö󰡵��� �Ѵ�
            //D3DXMatrixRotationY( &mat2, ( pDamageInstance->m_Life % 20 ) * 3.14f / 20.0f );
            //D3DXMatrixMultiply( &mat1, &mat1, &mat2 );

            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            if (iLife >= 18)
            {
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(255 - (20 * iLife), 255, 255, 255);
            }
            else
            {
                v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                    D3DCOLOR_ARGB(255, 255, 255, 255);
            }
            break;
        case DT_SKILL_LAS_HONET_STING:
        case DT_SKILL_LAS_BOMBDART_DART:
        case DT_SKILL_LAS_THUNDER_SLASH_SURIKEN:
        case DT_SKILL_LAS_RAVEN_THROWKNIFE:
        case DT_SKILL_LAS_RAVEN_THROWKNIFE_1:
        case DT_SKILL_LAS_RAVEN_THROWKNIFE_2:
        case DT_SKILL_RYAN_GREAT_DIVIDE_3:
        case DT_SKILL_SIEGE_MISSILE:
        case DT_SKILL_SIEGE_MISSILE_DUN:
        case DT_MARI4_MEGA_LAUNCHER:
        case DT_MARI4_MEGA_LAUNCHER_DUN:
        case DT_MARI4_SP_MEGA_LAUNCHER_DUN:
        case DT_MARI4_MEGA_LAUNCHER_PARTICLE:
            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff = D3DCOLOR_ARGB(255, 255, 255, 255);

            D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);
            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);
            break;
        default:
            if (pDamageInstance->m_pOriginalDamage->m_SRCBlendMode == D3DBLEND_ONE
                && pDamageInstance->m_pOriginalDamage->m_DESTBlendMode == D3DBLEND_ONE)
            {
                if (pDamage->m_Life_Change == -1)
                {
                    if (iLife <= 25)
                    {
                        iLife *= 10;
                        v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                            D3DCOLOR_ARGB(255, iLife, iLife, iLife);
                    }
                }
                else
                {
                    v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                        D3DCOLOR_ARGB(255, iLife, iLife, iLife);
                }
            }
            else
            {
                if (pDamage->m_bNoAlphaChange)
                {
                    v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                        D3DCOLOR_ARGB(255, 255, 255, 255);
                }
                else
                {
                    if (pDamage->m_Life_Change == -1)
                    {
                        if (iLife <= 25)
                            v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                            D3DCOLOR_ARGB(iLife * 10, 255, 255, 255);
                    }
                    else
                    {
                        v[iVertexOffset].Diff = v[iVertexOffset + 1].Diff = v[iVertexOffset + 2].Diff = v[iVertexOffset + 3].Diff =
                            D3DCOLOR_ARGB(iLife, 255, 255, 255);
                    }
                }

            }

            if (pDamageInstance->m_What == DT_LIGHTNING_GENERATE1
                || pDamageInstance->m_What == DT_LIGHTNING_GENERATE1_BLUE
                || pDamageInstance->m_What == DT_LIGHTNING_GENERATE2
                || pDamageInstance->m_What == DT_LIGHTNING_GENERATE2_BLUE)
                D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x * 0.167f, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x * 0.167f, 1.0f);
            else
                D3DXMatrixScaling(&mat2, pDamageInstance->m_Scale_x, pDamageInstance->m_Scale_x * pDamage->m_Y_per_x, 1.0f);

            D3DXMatrixRotationZ(&mat3, pDamageInstance->m_Angle);
            D3DXMatrixMultiply(&mat1, &mat2, &mat3);
            D3DXMatrixTranslation(&mat2, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            D3DXMatrixMultiply(&mat3, &mat1, &mat2);
            D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

            break;
        } //end of switch(pDamageInstance->m_What)

        D3DXVec3TransformCoord(&v[iVertexOffset].Pos, &v[iVertexOffset].Pos, &mat1);
        D3DXVec3TransformCoord(&v[iVertexOffset + 1].Pos, &v[iVertexOffset + 1].Pos, &mat1);
        D3DXVec3TransformCoord(&v[iVertexOffset + 2].Pos, &v[iVertexOffset + 2].Pos, &mat1);
        D3DXVec3TransformCoord(&v[iVertexOffset + 3].Pos, &v[iVertexOffset + 3].Pos, &mat1);
        pDamageInstance->m_pOriginalDamage->UVOperation(pDamageInstance, iVertexOffset, v);

        iVertexOffset += 4;
        rendersize++;
    }
    return rendersize;
}

void CDamageManager::Render()
{
    static SLVERTEX V[MAX_DAMAGE_VERTEX];
    CDamageInstance* pDamageInstance = NULL;

    std::map< int, std::vector<CDamageInstance*> >::iterator itor;
    for (itor = m_mapDamageInstance.begin(); itor != m_mapDamageInstance.end(); ++itor)
    {
        //���� ������ �ؾ��� ������
        int rendersize = SetBuffer(V, itor);
        if (rendersize > 0)
        {
            pDamageInstance = itor->second[0];

            g_pGCDeviceManager2->PushState();
            g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, pDamageInstance->m_pOriginalDamage->m_SRCBlendMode);
            g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, pDamageInstance->m_pOriginalDamage->m_DESTBlendMode);

            g_pGCDeviceManager2->DrawInWorld(m_apTexture[pDamageInstance->m_pOriginalDamage->m_Texture], V, rendersize);

            g_pGCDeviceManager2->PopState();
        }
    }

}

void CDamageManager::CrashWithUpGround(CDamageInstance* pDamageInstance, float fOffsetX, float fOffsetY)
{
    if (false == SiKGCFantasticMap()->IsAvailableGoDown(pDamageInstance->m_X + fOffsetX, pDamageInstance->m_Y + fOffsetY))
    {
        pDamageInstance->m_Life = 1;
    }
}

void CDamageManager::CrashWithPlayer(CDamageInstance* pDamageInstance)
{
    bool Combo_Counted = false;
    bool bCheckCombination = false;

    if (pDamageInstance->m_What == DT_PET_MEDUSA_ATTACT2)
        return;

    // ������ �����ϰ��� �浹�ϴ� �������� ������
    if (false == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.empty())
        return;

    // �÷��̾�� �浹ó�� ���� �ʴ´�.
    if (pDamageInstance->m_pOriginalDamage->m_bNoCrashPlayer)
        return;

    // hit�� HP�� ����ϱ� ���� ������.
    float fAbsorbHPPerHit = 0.f;

    // �� ���ָ� üũ���� ����
    bool bOnlyOneCollision = false;
    int iOnlyOneHitWho = -1;
    int iCountCollision = 0;

    // ������ �浹�� �˻��Ѵ�.    
    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.cCharIndex == -1)
            continue;

        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false)
            continue;

        if (!(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) &&
            !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS) &&
            g_MyD3D->MyPlayer[i]->IsSpecialSkillMotion()) {
            continue;
        }

        if (pDamageInstance->m_pOriginalDamage->m_bNoCrashFatal)
        {
            if (0.0f >= g_MyD3D->MyPlayer[i]->GetHP() || g_MyD3D->MyPlayer[i]->IsFatal())
                continue;
        }

        //�巹��ũ ���� ���� �������� �������α��� ���� ���� ��� �÷��̾� ���̴� ������������ �÷��̾ ���������̸� 
        //�����ʵ��� �����Ѵ�.
        if (pDamageInstance->m_What == DT_LAVA_CONTINENT_MAP_DAMAGE && g_MyD3D->MyPlayer[i]->uiMotion == MID_COMMON_ARENA)
        {
            continue;
        }

        // ������ �������� �̹� �°� �ִٸ� �� �������� ���� �ʰ� �ϰڴ�.
        if (-1 < pDamageInstance->m_pOriginalDamage->m_iUniqueDamage)
        {
            if (IsAlreadyCrashDamage(pDamageInstance, i)) {
                continue;
            }
        }

        // Ư�� ���� �ɷ������� �� �������� ���� �ʰ� �Ѵ�. 
        if (pDamageInstance->m_pOriginalDamage->m_vecPassBuff.empty() == false) {

            bool bContinue = false;
            std::vector< int >::const_iterator it = pDamageInstance->m_pOriginalDamage->m_vecPassBuff.begin();

            for (; it != pDamageInstance->m_pOriginalDamage->m_vecPassBuff.end(); ++it) {
                if (g_MyD3D->MyPlayer[i]->IsMagicEffect(*it)) {
                    bContinue = true;
                    break;
                }
            }

            if (bContinue) {
                continue;
            }
        }

        // Ư�� ������ �� �ɷ������� �� �������� ���� �ʰ� �Ѵ�.
        if (pDamageInstance->m_pOriginalDamage->m_vecPassNoBuff.empty() == false) {

            bool bContinue = false;
            std::vector< int >::const_iterator it = pDamageInstance->m_pOriginalDamage->m_vecPassNoBuff.begin();

            for (; it != pDamageInstance->m_pOriginalDamage->m_vecPassNoBuff.end(); ++it) {
                if (false == g_MyD3D->MyPlayer[i]->IsMagicEffect(*it)) {
                    bContinue = true;
                    break;
                }
            }

            if (bContinue) {
                continue;
            }
        }

        // 2007.08.22 wony ȭ���̶�� �ǵ尡 �־ ���� �ʰ� ����ϰ��� �����̸�..
        if (pDamageInstance->m_pOriginalDamage->m_bIgnoreSuperDamage == false) {
            if (!((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_IGNORE_INVINSIBLE) &&
                g_MyD3D->MyPlayer[i]->IsInvinsible(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack)))
            {
                if ((g_MyD3D->MyPlayer[i]->Shield == 0 || pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) &&
                    g_MyD3D->MyPlayer[i]->IsInvinsible(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack) == true &&
                    !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS))
                    continue;
            }
        }

        if (SiKGCRoomManager()->IsMonsterGameMode())
        {
            if (g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_INTO_TELEPORT)) // ��Ż�ȿ� ������ �������� �߱�� 
                continue;
        }

        bool bSameTeam;
        if (!pDamageInstance->m_IsMonsterDamage && g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i))
            bSameTeam = true;
        else
            bSameTeam = false;

        //���͵� ���� ���� �ִ�!!!
        if (pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_ExtraMonsterNum))
        {
            bSameTeam = g_MyD3D->IsSameTeam_PlayerAndMonster(i, pDamageInstance->m_ExtraMonsterNum);
        }

        if (pDamageInstance->m_pOriginalDamage->m_bAlwaysEnemy)
        {
            bSameTeam = false;
        }


        bool bCurPlayerRight = g_MyD3D->MyPlayer[i]->bIsRight;    // CrashPlayer �Լ��� ȣ��Ǹ� �ٶ󺸴� ������ �ٲ�Ƿ� �̸� �޾Ƶδ� ��.

        // �� üũ�ؼ� �������� ���� ���� ����
        if (pDamageInstance->m_pOriginalDamage->m_bAttackToSameTeam)
        {
            if (false == bSameTeam)
                continue;
        }
        else
        {
            if (bSameTeam == true)
            {
                //CURE �Ӽ��̰� ����� ���� ����
                if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE)
                    && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() && i == g_MyD3D->Get_MyPlayer())
                    continue;

                // ���� ���̶�� CURE �׸��̶� ���� ��ź WAVE�� �¾ƾ� �ȴ�( ��� )
                if (!(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) &&
                    !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS) &&
                    (pDamageInstance->m_What != DT_BOMB_WAVE
                        && pDamageInstance->m_What != DT_WATER_POLE
                        && pDamageInstance->m_What != DT_FIRE_POLE))
                    continue;
            }
            else
            {
                // �ٸ� ���̶� Cure�׸��� ���� ���ƾ� �ȴ�
                if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE || pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS)
                    continue;
            }
        }

        //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
        if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
            && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
            continue;

        if (pDamageInstance->m_pOriginalDamage->m_bNoPassDamage)
        {
            //}			
            continue;
            //if( (g_MyD3D->MyPlayer[i]->vPos.y + g_MyD3D->MyPlayer[i]->y_Ex < pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.4f - 0.25f )&&
            //	(g_MyD3D->MyPlayer[i]->vPos.y + g_MyD3D->MyPlayer[i]->y_Ex > pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.4f - 0.35f))
            //{
            //	float fObjectLeft = pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f;
            //	float fObjectRight = pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f;
            //	if( (fObjectRight > g_MyD3D->MyPlayer[i]->vPos.x ) && 
            //		(fObjectLeft < g_MyD3D->MyPlayer[i]->vPos.x) )
            //	{
            //		if( (fObjectLeft + fObjectRight)/2.0f > g_MyD3D->MyPlayer[i]->vPos.x )
            //			g_MyD3D->MyPlayer[i]->vPos.x = fObjectLeft;
            //		else
            //			g_MyD3D->MyPlayer[i]->vPos.x = fObjectRight;
            //		continue;
            //	}
            //}
        }

        if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) &&
            pDamageInstance->m_pOriginalDamage->m_bSpecialAttack == false) {
            g_MyD3D->MyPlayer[i]->m_prReflectionDamage = std::make_pair(pDamageInstance->m_What, pDamageInstance->m_Angle);
        }

        float fRangeX = 0.0f;
        float fRangeY = 0.0f;
        if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
        {
            fRangeX = 1.0f;
            fRangeY = 1.0f;
        }
        else
        {
            fRangeX = 0.5f;
            fRangeY = 0.4f;
        }

        // ���� ��Ȯ�� �浹 �˻縦 �Ѵ�.
        bool bCollision = false;
        bool bCollisionEmark = false;
        if (UINT_MAX > g_MyD3D->MyPlayer[i]->GetEmbarkUID())
        {
            KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(g_MyD3D->MyPlayer[i]->GetEmbarkUID());
            if (pEmbark)
            {
                GCCollisionRect<float> kEmbarkRect = pEmbark->GetAttackRect();
                GCCollisionRect<float> kDamageRect, kColl;

                kEmbarkRect.m_Left += 1.0f;
                kEmbarkRect.m_Right += 1.0f;
                kEmbarkRect.m_Top += 0.15f;
                kEmbarkRect.m_Bottom += 0.15f;

                kDamageRect.SetRect(pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX,
                    pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.25f,
                    pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX,
                    pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.35f);
                if (kEmbarkRect.CheckCollision(kDamageRect, &kColl))
                {
                    bCollisionEmark = true;
                    pEmbark->SetWhatDamage(pDamageInstance->m_What);
                    pEmbark->CallLuaFunc_Callback(KGCEmbarkTemplate::CALLBACK_CRASH_DAMAGE);
                }
            }
        }
        else
        {
            if (g_MyD3D->MyPlayer[i]->MyBody.x + 1.0f <= pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX
                && g_MyD3D->MyPlayer[i]->MyBody.y + 1.0f > pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX
                && g_MyD3D->MyPlayer[i]->MyBody.z < pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.25f
                && g_MyD3D->MyPlayer[i]->MyBody.w > pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.35f)
                bCollision = true;
        }

        if (bCollision || bCollisionEmark)
        {
            //�ݻ�
            if (g_MyD3D->MyPlayer[i]->Item_Remain_Time[GC_GAME_ITEM_REFLECTION]
                && (!(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE)
                    || !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS))
                && (!(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_PHYSICAL)))

            {
                if (!(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_DO_NOT_REFLECTION))
                {
                    pDamageInstance->m_Who = i;
                    pDamageInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_Start_Life;
                    pDamageInstance->m_Scale_x = pDamageInstance->m_pOriginalDamage->m_Start_Scale_x;
                    pDamageInstance->m_IsRight = !pDamageInstance->m_IsRight;
                    pDamageInstance->m_IsMonsterDamage = false;    //����� �ݻ��ϸ� ���� �������� �ƴ�����.
                    pDamageInstance->m_iTeam = g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam;
                }
                continue;
            }

            if (pDamageInstance->IsHitPerFrame() == false)
                continue;

            // ���� ����
            if (0 < pDamageInstance->m_pOriginalDamage->m_uiNotSpecialSuper)
            {
                if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) ||
                    (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS))
                {
                    g_MyD3D->MyPlayer[i]->SetNotSpecialSuper(pDamageInstance->m_pOriginalDamage->m_uiNotSpecialSuper);
                    continue;
                }
            }

            // [12/11/2007] breadceo. ��Ʈ�����۴� �������� ���� üũ�� �ؼ� �޺��� �̻��ϰ� ���´� �׷��� �̷��� �� �� �ٵ� �߸��ߴ�!
            bool bBeforeDamage = pDamageInstance->CheckAlreadyDamaged(i);
            pDamageInstance->m_rtCollision = g_MyD3D->MyPlayer[i]->GetCollisionRect();

            // ���ҵ����� ������ ���Ѵ�. by. dolki811
            if (pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_iNumDivision > 0 && bCollision) {
                int iNumDivision = pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_iNumDivision;
                std::vector< float > vecRangeDamageRatio = pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_vecDamageRatio;

                for (int idx = 0; idx < static_cast<int>(vecRangeDamageRatio.size()); idx++) {
                    float fDivisionRangeX, fDivisionRangeY;

                    if (pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_bCheckRangeX) {
                        fDivisionRangeX = fRangeX * (static_cast<float>(idx + 1) / static_cast<float>(iNumDivision));
                    }
                    else {
                        fDivisionRangeX = fRangeX;
                    }

                    if (pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_bCheckRangeY) {
                        fDivisionRangeY = fRangeY * (static_cast<float>(idx + 1) / static_cast<float>(iNumDivision));
                    }
                    else {
                        fDivisionRangeY = fRangeY;
                    }

                    if (g_MyD3D->MyPlayer[i]->MyBody.x + 1.0f <= pDamageInstance->m_X + pDamageInstance->m_Scale_x * fDivisionRangeX
                        && g_MyD3D->MyPlayer[i]->MyBody.y + 1.0f > pDamageInstance->m_X - pDamageInstance->m_Scale_x * fDivisionRangeX
                        && g_MyD3D->MyPlayer[i]->MyBody.z < pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fDivisionRangeY - 0.25f
                        && g_MyD3D->MyPlayer[i]->MyBody.w > pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fDivisionRangeY - 0.35f) {
                        pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_fRangeDamageRatio = vecRangeDamageRatio[idx];
                        break;
                    }
                }
            }

            if (pDamageInstance->m_pOriginalDamage->CrashPlayer(pDamageInstance, i, 1.f))
            {
                iCountCollision++;
                if (bOnlyOneCollision == false) {
                    iOnlyOneHitWho = i;
                }
                bOnlyOneCollision = true;
                if (pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos.m_bIsSync && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON) {
                    g_MyD3D->MyPlayer[i]->m_bNoSafePosition = true;
                }

                CallEffAtCrash(pDamageInstance, i, true);

                if (g_MyD3D->GetMyPlayer() && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer()) {
                    g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk(pDamageInstance->m_What);
                    // ���� ������ ����
                    g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk(pDamageInstance->m_What);
                    // �� ������ ����
                    g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk(pDamageInstance->m_What);

                    // ����� ���� ����
                    if (g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.empty() == false) {
                        std::map< int, std::map< int, std::pair< int, float > > >::iterator mit =
                            g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.find(SPECIAL_BUFF_SUCCESS_ATK);
                        if (mit != g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.end()) {
                            std::map< int, std::pair< int, float > >::iterator mitSB = mit->second.begin();
                            PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
                            for (mitSB; mitSB != mit->second.end(); ++mitSB) {
                                if (g_MyD3D->GetMyPlayer()->CheckSkill(mitSB->first) &&
                                    g_MyD3D->GetMyPlayer()->SkillAloowMotion(pPlayer->GetCurrentChar().iCharType, pPlayer->uiMotion)
                                    ) {
                                    //g_pMagicEffect->SetMagicEffect( g_MyD3D->Get_MyPlayer(), mitSB->second.first, mitSB->second.second );
                                    g_MyD3D->GetMyPlayer()->SetMagicEffect(mitSB->second.first, mitSB->second.second);
                                }
                            }
                        }
                    }
                }
                bCheckCombination = true;

                // m_fAbsorbHPToMaxHP�� �ְ� ������ŭ HP�� �����ϰ�, �߰��� Hit ���� ���� m_iAddAbsorbHPPerHit �������ŭ�� HP�� �߰��� �����Ѵ�.
                if (pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP && pDamageInstance->m_pOriginalDamage->m_iAddAbsorbHPPerHit != 0)
                {
                    float fRatio = pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP * static_cast<float>(pDamageInstance->m_pOriginalDamage->m_iAddAbsorbHPPerHit / 100);
                    if (pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_Who))
                    {
                        MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);
                        if (pMonster) {
                            if (fAbsorbHPPerHit == 0.f) {
                                fAbsorbHPPerHit = pMonster->GetHP() + (pMonster->GetMaxHP() * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP);
                            }
                            else {
                                fAbsorbHPPerHit += (pMonster->GetMaxHP() * fRatio);
                            }
                        }
                    }
                    else if (g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who))
                    {
                        SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
                        if (-1 != sCharInfo.iCharType)
                        {
                            if (fAbsorbHPPerHit == 0.f) {
                                fAbsorbHPPerHit = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() +
                                    (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPlayerMaxHP() * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP);
                            }
                            else {
                                fAbsorbHPPerHit += (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPlayerMaxHP() * fRatio);
                            }
                        }
                    }
                }

                // �ڽ� �������� �������� ������Ű�� ������ ���������.
                //D3DXVECTOR2 hitPos = D3DXVECTOR2( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f );
                if (pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_BUFF_CREATE_DAMAGE) == false &&
                    pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_NO_CREATE_BUFF_DAMAGE) == false) {
                    if (pDamageInstance->m_bPetDamage) {
                        g_pMagicEffect->HitCreateBuffDamage(pDamageInstance->m_Who, i, NULL, CAHR_ATTACK_TYPE_PET_DAMAGE);
                    }
                    else if (pDamageInstance->m_pOriginalDamage->m_bSpecialAttack) {
                        g_pMagicEffect->HitCreateBuffDamage(pDamageInstance->m_Who, i, NULL, CAHR_ATTACK_TYPE_SPECIAL_DAMAGE);
                    }
                    else {
                        g_pMagicEffect->HitCreateBuffDamage(pDamageInstance->m_Who, i, NULL, CAHR_ATTACK_TYPE_BOX_DAMAGE);
                    }
                }
            }

            bool bDamaged = bBeforeDamage == true && pDamageInstance->CheckAlreadyDamaged(i) && (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE);

            if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_NOT_COMBO_COUNT)
                Combo_Counted = true;
            else
                Combo_Counted = false;

            bool bCombo = !Combo_Counted && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() && !pDamageInstance->m_IsMonsterDamage;
            bool bCrashCheckPerFrame = IsCrashCheckPerFrame(pDamageInstance);

            if (bCombo && ((false == bCrashCheckPerFrame && false == bDamaged) ||
                (true == bCrashCheckPerFrame && bBeforeDamage == false && pDamageInstance->CheckAlreadyDamaged(i) == true) ||
                (Combo_Counted == false && bCrashCheckPerFrame == true)))
            {
                if (pDamageInstance->m_pOriginalDamage)
                {
                    if (pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer < 0.0f)
                    {
                        g_MyD3D->MyHead->Event_Combo(false, 0.05f);
                        g_pGameBoard->AddAttackCount();
                    }
                }

                Combo_Counted = true;

                if (bSameTeam == false)
                {
                    g_MyD3D->MyPlayer[i]->SetFlashEffectFrame(FLASH_EFFECT_FRAME);
                    g_MyD3D->MyPlayer[i]->SetShakeEffectFrame(10);
                }
            }

            // ���Ͱ� ������Ų �������̰�.. ó�� �¾Ҵٸ�?
            if (pDamageInstance->m_IsMonsterDamage && bBeforeDamage == false && pDamageInstance->CheckAlreadyDamaged(i) == true)
            {
                //����!!!!!!!!!!!!!!!!!!!!
                if ((pDamageInstance->m_What != DT_XENIA5_MAP_DOTDAMAGE) &&
                    (pDamageInstance->m_What != DT_POISONTHORN_BOMB))// �������϶��� ����
                {
                    MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                    if (pMonster != NULL && !pDamageInstance->m_pOriginalDamage->m_bIgnoreChampionProperty) {
                        pMonster->ChampionMonsterAttack(i);
                    }

                }
            }

            if (false == ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) ||
                (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS)))
            {

                // ����2�� �������� ������ ���� �� Ÿ���� ������ �ش� ������ �����Ѵ�.
                if (g_MyD3D->MyPlayer[i]->IsMagicEffect(EGC_MARI3_MAGIC_BEAD_MP_RECOVERY) &&
                    !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS) &&
                    pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer < 0.0f)
                {
                    g_MyD3D->MyPlayer[i]->ClearMagicEffect(EGC_MARI3_MAGIC_BEAD_MP_RECOVERY);


                    if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
                    {
                        g_MyD3D->MyHead->Level += 0.5f;
                        if (g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP())
                            g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
                    }
                }

                if (pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_MAP_DAMAGE) == false) {
                    // ����3�� ����� ������ ���� �� Ÿ���� ������ �ش� ������ �����Ѵ�.
                    if ((g_MyD3D->MyPlayer[i]->IsMagicEffect(EGC_EFFECT_MARI4_REPAIR) || g_MyD3D->MyPlayer[i]->IsMagicEffect(EGC_EFFECT_MARI4_REPAIR_R) || g_MyD3D->MyPlayer[i]->IsMagicEffect(EGC_EFFECT_MARI4_SP_REPAIR) || g_MyD3D->MyPlayer[i]->IsMagicEffect(EGC_EFFECT_MARI4_SP_REPAIR_R)) &&
                        !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS) && pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer < 0.0f)
                    {
                        g_MyD3D->MyPlayer[i]->ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR);
                        g_MyD3D->MyPlayer[i]->ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR_R);
                        g_MyD3D->MyPlayer[i]->ClearMagicEffect(EGC_EFFECT_MARI4_SP_REPAIR);
                        g_MyD3D->MyPlayer[i]->ClearMagicEffect(EGC_EFFECT_MARI4_SP_REPAIR_R);

                        if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
                        {
                            g_MyD3D->MyHead->Level += 0.5f;
                            if (g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP())
                                g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
                        }
                    }
                }
            }

            // ������������ �÷��̾ ���������� MP����.(��ҵǾ���.)
            //if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) && 
            //    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsLocalPlayer() &&
            //    (GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory()) )
            //{
            //    float fIncMP = (pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f * pDamageInstance->m_fRatio) * 0.0125f;
            //    fIncMP = fIncMP * ( 1.0f + g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_fAbility[ABILITY_MP_RECOVERY_RATE]);
            //    g_MyD3D->ChargeLocalPlayerMana( fIncMP );
            //}

            if (g_MyD3D->MyPlayer[i]->IsMagicEffect(EGC_BUFF_CLONE_ATK, true))
            {
                g_pMagicEffect->ClearMagicEffect(i, EGC_BUFF_CLONE_ATK);
                if (pDamageInstance->m_What == DT_SKILL_LAS_TRADWARM_LV2_SCROOL)
                {
                    pDamageInstance->m_Life = 1;
                    pDamageInstance->m_pOriginalDamage->m_vecAutoDamage.clear();
                }
            }

            pDamageInstance->SetCrashCheck(true);

            if (pDamageInstance->m_pOriginalDamage->m_bOnlyOneUnit)
                break;
        }
        else
        {
            // �������� �浹���� �ʾ����� ������ �����ش�.
            if (-1 < pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect)
            {
                if (g_MyD3D->MyPlayer[i]->IsMagicEffect(pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect, true))
                    g_MyD3D->MyPlayer[i]->ClearMagicEffect(pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect, false);
            }
        }
    }

    if (bOnlyOneCollision && g_MyD3D->GetMyPlayer() && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer()) {
        g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->SetHittedPrevDamageInfo(pDamageInstance->m_What, iOnlyOneHitWho);

        //////////////////////////// ���� - ���尳�� ���� ���� //////////////////////////////////////
        SiGCGrowingSkillManager()->SetMagicEffectWhenAttack(g_MyD3D->MyPlayer[pDamageInstance->m_Who], iCountCollision);
        ////////////////////////////  ��  - ���尳�� ���� ���� //////////////////////////////////////
    }

    if (pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP &&
        pDamageInstance->m_pOriginalDamage->m_iAddAbsorbHPPerHit != 0 &&
        fAbsorbHPPerHit != 0.f) {
        if (pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_Who)) {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);
            if (pMonster) {
                pMonster->SetHP(fAbsorbHPPerHit);
            }

        }
        else if (g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who)) {
            SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
            if (-1 != sCharInfo.iCharType) {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetHP(fAbsorbHPPerHit);
            }
        }
    }

}

void CDamageManager::CrashWithMonster(CDamageInstance* pDamageInstance)
{
    if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) || (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS))
        return;

    // ���Ϳ� �浹ó�� ���� �ʴ´�.
    if (pDamageInstance->m_pOriginalDamage->m_bNoCrashMonster)
        return;

    //���Ϳ� �÷��̾ �ٸ� ����Ʈ�� �ٸ�ȿ���� �ʿ��Ҷ� �������.
    if (pDamageInstance->m_What == DT_PET_MASHIMARO_THROW_PLAYER ||
        pDamageInstance->m_What == DT_PET_SMALL_MAID_THROW_PLAYER ||
        pDamageInstance->m_What == DT_PET_MAID_THROW_PLAYER ||
        pDamageInstance->m_What == DT_PET_CHRISTMAS_MAID_THROW_PLAYER ||
        pDamageInstance->m_What == DT_KERORO_LIGHTNING_BOOM)
        return;

    if (pDamageInstance->IsHitPerFrame() == false)
        return;

    float fAbsorbHPPerHit = 0.f;
    bool bOnlyOneCollision = false; // �� ������ �浹�Ǿ������� üũ�Ѵ�.
    int iCountCollision = 0; // �ǰݵ� �� �� üũ
    int iHitMonsterIndex = -1;
    for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        if (pDamageInstance->m_What == DT_PERIOT_EAT_MONSTER)
            goto IgnoreSameTeam;

        // ������ �����ϰ��� �浹�ϴ� �������� üũ����
        if (false == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.empty())
        {
            std::vector<int>::iterator vecIter = pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.begin();
            for (; vecIter != pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.end(); ++vecIter)
            {
                if ((*vecIter) == mit->second->m_iMonsterType)
                    break;
            }

            if (vecIter == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.end())
                continue;
        }

        // NoCrashDamage�Ӽ��� �ִ� ���ʹ� ����
        if (0 <= mit->second->GetMonsterType() && mit->second->GetMonsterType() < MONSTER_MODEL_NUM)
        {
            if (mit->second->GetNoCrashDamage())
                continue;
        }

        // �� üũ�ؼ� �������� ���� ���� ����
        if (pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_CURE_FOR_MONSTER) == false)
        {
            bool bSameTeam = false;
            if (pDamageInstance->m_IsMonsterDamage)
            {
                if (g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, mit->first))
                    bSameTeam = true;
                if (pDamageInstance->m_pOriginalDamage->m_bAlwaysEnemy)
                    bSameTeam = false;
            }
            else
            {
                if (g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first))
                    bSameTeam = true;
            }

            if (pDamageInstance->m_pOriginalDamage->m_bAttackToSameTeam)
            {
                if (false == bSameTeam)
                    continue;
            }
            else
            {
                if (bSameTeam)
                    continue;
            }
        }

    IgnoreSameTeam:
        if (!mit->second->m_bLive)
            continue;

        //if( pDamageInstance->m_Who == Monsteri )
        //	continue;

        // ������ �������� �̹� �°� �ִٸ� �� �������� ���� �ʰ� �ϰڴ�.
        if (-1 < pDamageInstance->m_pOriginalDamage->m_iUniqueDamage)
        {
            if (IsAlreadyCrashDamage(pDamageInstance, mit->first)) {
                continue;
            }
        }

        // Ư�� ���� �ɷ������� �� �������� ���� �ʰ� �Ѵ�. 
        if (pDamageInstance->m_pOriginalDamage->m_vecPassBuff.empty() == false) {

            bool bContinue = false;
            std::vector< int >::const_iterator it = pDamageInstance->m_pOriginalDamage->m_vecPassBuff.begin();

            for (; it != pDamageInstance->m_pOriginalDamage->m_vecPassBuff.end(); ++it) {
                if (mit->second->IsMagicEffect(mit->first, *it)) {
                    bContinue = true;
                    break;
                }
            }

            if (bContinue) {
                continue;
            }
        }

        // Ư�� ������ �� �ɷ������� �� �������� ���� �ʰ� �Ѵ�.
        if (pDamageInstance->m_pOriginalDamage->m_vecPassNoBuff.empty() == false) {

            bool bContinue = false;
            std::vector< int >::const_iterator it = pDamageInstance->m_pOriginalDamage->m_vecPassNoBuff.begin();

            for (; it != pDamageInstance->m_pOriginalDamage->m_vecPassNoBuff.end(); ++it) {
                if (false == mit->second->IsMagicEffect(mit->first, *it)) {
                    bContinue = true;
                    break;
                }
            }

            if (bContinue) {
                continue;
            }
        }

        //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
        if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
            && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
            continue;

        bool bCurMonsterRight = mit->second->m_abIsRight[LATENCY_INDEX];    // CrashPlayer �Լ��� ȣ��Ǹ� �ٶ󺸴� ������ �ٲ�Ƿ� �̸� �޾Ƶδ� ��.

        float fRangeX = 0.0f;
        float fRangeY = 0.0f;
        if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
        {
            fRangeX = 1.0f;
            fRangeY = 1.0f;
        }
        else
        {
            fRangeX = 0.5f;
            fRangeY = 0.4f;
        }

        GCCollisionRect<float> rtFire;
        GCCollisionRect<float> rtCollision;
        rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX - 1.0f;
        rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX - 1.0f;
        rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
        rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;

        bool bSuccessCollision = false;
        for (int i = 0; i < mit->second->GetAttackBoundRectNum(); i++)
        {
            GCCollisionRect<float> rtMonster = mit->second->GetAttackBoundRect(i);

            if (!((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_IGNORE_INVINSIBLE) && mit->second->IsInvincible()))
            {
                if (mit->second->IsInvincible())
                    continue;
            }

            if (rtMonster.CheckCollision(rtFire, &rtCollision) == false)
                continue;

            // ���ҵ����� ������ ���Ѵ�. by. dolki811
            if (pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_iNumDivision > 0) {
                int iNumDivision = pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_iNumDivision;
                std::vector< float > vecRangeDamageRatio = pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_vecDamageRatio;

                for (int idx = 0; idx < static_cast<int>(vecRangeDamageRatio.size()); idx++) {
                    float fDivisionRangeX, fDivisionRangeY;

                    if (pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_bCheckRangeX) {
                        fDivisionRangeX = fRangeX * (static_cast<float>(idx + 1) / static_cast<float>(iNumDivision));
                    }
                    else {
                        fDivisionRangeX = fRangeX;
                    }

                    if (pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_bCheckRangeY) {
                        fDivisionRangeY = fRangeY * (static_cast<float>(idx + 1) / static_cast<float>(iNumDivision));
                    }
                    else {
                        fDivisionRangeY = fRangeY;
                    }

                    GCCollisionRect<float> rtRangeFire;
                    GCCollisionRect<float> rtRangeCollision;
                    rtRangeFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * fDivisionRangeX - 1.0f;
                    rtRangeFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * fDivisionRangeX - 1.0f;
                    rtRangeFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fDivisionRangeY - 0.26f;
                    rtRangeFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fDivisionRangeY - 0.26f;

                    if (rtMonster.CheckCollision(rtRangeFire, &rtRangeCollision) == true) {
                        pDamageInstance->m_pOriginalDamage->m_kDivisionDamage.m_fRangeDamageRatio = vecRangeDamageRatio[idx];
                        break;
                    }
                }
            }

            pDamageInstance->m_rtCollision = rtCollision;

            if (rtFire.m_Left == rtCollision.m_Left)
                mit->second->SetDamageDirection(MONSTER::DR_RIGHT);
            else if (rtFire.m_Right == rtCollision.m_Right)
                mit->second->SetDamageDirection(MONSTER::DR_LEFT);

            mit->second->SetLastAttackedBoxIndex(i);
            D3DXVECTOR2 hitPos = D3DXVECTOR2(rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f);
            mit->second->SetHitPos(hitPos);

            CallEffAtCrash(pDamageInstance, mit->first, false);
            g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);
            pDamageInstance->m_pOriginalDamage->CrashMonster(pDamageInstance, mit->first);
            mit->second->SetAttackedMonster(true);
            pDamageInstance->SetCrashCheck(true);
            bSuccessCollision = true;

            break;
        }

        if (bSuccessCollision) {
            iCountCollision++;
            if (bOnlyOneCollision == false) {
                iHitMonsterIndex = mit->second->GetMonsterIndex();
            }
            bOnlyOneCollision = true;
            if (pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP && pDamageInstance->m_pOriginalDamage->m_iAddAbsorbHPPerHit != 0)
            {
                float fRatio = pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP * static_cast<float>(pDamageInstance->m_pOriginalDamage->m_iAddAbsorbHPPerHit / 100);
                if (pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_Who))
                {
                    MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);
                    if (pMonster) {
                        if (fAbsorbHPPerHit == 0.f) {
                            fAbsorbHPPerHit = pMonster->GetHP() + (pMonster->GetMaxHP() * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP);
                        }
                        else {
                            fAbsorbHPPerHit += (pMonster->GetMaxHP() * fRatio);
                        }
                    }
                }
                else if (g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who))
                {
                    SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
                    if (-1 != sCharInfo.iCharType)
                    {
                        if (fAbsorbHPPerHit == 0.f) {
                            fAbsorbHPPerHit = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() +
                                (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPlayerMaxHP() * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP);
                        }
                        else {
                            fAbsorbHPPerHit += (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPlayerMaxHP() * fRatio);
                        }
                    }
                }
            }

            // �ڽ� �������� �������� ������Ű�� ������ ���������.
            if (pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_BUFF_CREATE_DAMAGE) == false &&
                pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_NO_CREATE_BUFF_DAMAGE) == false) {
                D3DXVECTOR2 hitPos = D3DXVECTOR2(rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f);
                if (pDamageInstance->m_bPetDamage) {
                    g_pMagicEffect->HitCreateBuffDamage(pDamageInstance->m_Who, mit->first, &hitPos, CAHR_ATTACK_TYPE_PET_DAMAGE);
                }
                else if (pDamageInstance->m_pOriginalDamage->m_bSpecialAttack) {
                    g_pMagicEffect->HitCreateBuffDamage(pDamageInstance->m_Who, mit->first, &hitPos, CAHR_ATTACK_TYPE_SPECIAL_DAMAGE);
                }
                else {
                    g_pMagicEffect->HitCreateBuffDamage(pDamageInstance->m_Who, mit->first, &hitPos, CAHR_ATTACK_TYPE_BOX_DAMAGE);
                }
            }

            if (pDamageInstance->m_pOriginalDamage->m_iIgnoreGroundCheckHittedObject != -1) {
                mit->second->m_iNoCheckContact = pDamageInstance->m_pOriginalDamage->m_iIgnoreGroundCheckHittedObject;
            }
        }

        // �������� �浹���� �ʾ����� ������ �����ش�.
        if (false == bSuccessCollision && -1 < pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect)
        {
            if (mit->second->IsMagicEffect(mit->first, pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect))
                mit->second->ClearMagicEffect(mit->first, pDamageInstance->m_pOriginalDamage->m_iNoCrashUnMagicEffect);
        }

        if (bSuccessCollision && pDamageInstance->m_pOriginalDamage->m_bOnlyOneUnit)
            break;
    }

    if (pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToMaxHP &&
        pDamageInstance->m_pOriginalDamage->m_iAddAbsorbHPPerHit != 0 &&
        fAbsorbHPPerHit != 0.f) {
        if (pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_Who)) {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);
            if (pMonster) {
                pMonster->SetHP(fAbsorbHPPerHit);
            }

        }
        else if (g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who)) {
            SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
            if (-1 != sCharInfo.iCharType) {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetHP(fAbsorbHPPerHit);
            }
        }
    }

    if (bOnlyOneCollision && g_MyD3D->GetMyPlayer() && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer()) {
        // ���� ������ ����
        g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->SetHittedPrevDamageInfo(pDamageInstance->m_What, iHitMonsterIndex);

        //////////////////////////// ���� - ���尳�� ���� ���� //////////////////////////////////////
        SiGCGrowingSkillManager()->SetMagicEffectWhenAttack(g_MyD3D->MyPlayer[pDamageInstance->m_Who], iCountCollision);
        ////////////////////////////  ��  - ���尳�� ���� ���� //////////////////////////////////////
    }
}

// ���Ͱ� �������� �����ϴ°� üũ�ϴ� �Լ�( ���Ͱ������� ������ HP���ҽ�Ű�� �� )
void CDamageManager::CrashWithMonsterAttack(CDamageInstance* pDamageInstance)
{
    GCCollisionRect<float> rtFire, rtMonAtkRange, rtCollision;
    int iIndex = LATENCY_INDEX;
    if (!g_kGlobalValue.m_kUserInfo.bHost)
        iIndex = 0;

    if (false == pDamageInstance->m_pOriginalDamage->m_vecPermissionDamageHP.empty())
        return;

    if (!pDamageInstance->m_pOriginalDamage->m_iDamageHP)
        return;

    // �������� �浹ó�� ���� �ʴ´�.
    if (pDamageInstance->m_pOriginalDamage->m_bNoCrashDamage)
        return;

    // ������ �����ϰ��� �浹�ϴ� �������� ������
    if (false == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.empty())
        return;

    float fRangeX = 0.0f;
    float fRangeY = 0.0f;
    if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
    {
        fRangeX = 1.0f;
        fRangeY = 1.0f;
    }
    else
    {
        fRangeX = 0.5f;
        fRangeY = 0.4f;
    }

    for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        //���� ���̶��.
        if (pDamageInstance->m_IsMonsterDamage)
        {
            if (g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, mit->first))
                continue;
        }
        else
        {
            if (g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first))
                continue;
        }

        if (!mit->second->m_bLive)
            continue;

        //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
        if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
            && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
            continue;

        //�����Ѱ� �ƴ϶�� �н�
        if (mit->second->m_vecPhsAtk.empty())
            continue;

        rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX - 1.0f;
        rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX - 1.0f;
        rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
        rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
        rtCollision = rtFire;

        for (std::vector<KMonsterPhysicAttack>::iterator it = mit->second->m_vecPhsAtk.begin();
            it != mit->second->m_vecPhsAtk.end(); it++)
        {
            if ((*it).m_iBaseBone == 0 && (*it).m_iEndBone == 0)
            {
                // ���� �����ȿ� ������ üũ�Ѵ�.. ���ݹ����� �¿� ���� �ٿ���ڽ� ũ���� �� 
                if (!mit->second->m_abIsRight[iIndex])	// �������� ���� ���� ��
                {
                    rtMonAtkRange.m_Left = mit->second->m_afX[iIndex];
                    rtMonAtkRange.m_Right = mit->second->m_afX[iIndex] + mit->second->m_fAttackBoundBoxWidth + (*it).m_fRange;
                }
                else	// ������ ���� ���� ��
                {
                    rtMonAtkRange.m_Left = mit->second->m_afX[iIndex] - mit->second->m_fAttackBoundBoxWidth - (*it).m_fRange;
                    rtMonAtkRange.m_Right = mit->second->m_afX[iIndex];
                }
                rtMonAtkRange.m_Top = mit->second->m_afY[iIndex] + mit->second->m_fAttackBoundBoxHeight;
                rtMonAtkRange.m_Bottom = mit->second->m_afY[iIndex];
            }
            else
            {
                D3DXVECTOR3 vecBase, vecEnd;
                D3DXMATRIX matBone;
                const D3DXMATRIX* pWorld = mit->second->m_pObject->GetWorldMat();

                mit->second->m_pObject->GetAngleBone((*it).m_iBaseBone, &matBone);
                matBone = matBone * *pWorld;
                SETVECTOR3(vecBase, matBone._41, matBone._42, 1.0f);

                mit->second->m_pObject->GetAngleBone((*it).m_iEndBone, &matBone);
                matBone = matBone * *pWorld;
                SETVECTOR3(vecEnd, matBone._41, matBone._42, 1.0f);

                rtMonAtkRange.SetRect(vecBase.x, vecBase.y, vecEnd.x, vecEnd.y);
            }

            if (rtFire.CheckCollision(rtMonAtkRange, &rtCollision))
            {
                pDamageInstance->m_pOriginalDamage->m_iDamageHP--;
                mit->second->m_iSlowCount = 20;
                (*it).m_iLifeTime = 0;

                //CallEffAtCrash( pDamageInstance, -1, false, rtWeapon.m_Left + 1.0f, rtWeapon.m_Top + 0.25f );

                if (pDamageInstance->m_pOriginalDamage->m_iDamageHP <= 0)
                {
                    pDamageInstance->m_Life = 1;

                    if (pDamageInstance->m_What == DT_MARI3_BUILD_MANA_DEVICE)
                    {
                        bool bTeamBuffEnd = false;
                        bool bEnemyBuffEnd = false;

                        std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(pDamageInstance->m_What);

                        if (itor != m_mapDamageInstance.end()) {
                            for (int i = 0; i < (int)itor->second.size(); i++)
                            {
                                if (itor->second[i]->m_Who == pDamageInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_TEAM_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bTeamBuffEnd = true;
                                }

                                if (itor->second[i]->m_Who == pDamageInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_ENEMY_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bEnemyBuffEnd = true;
                                }
                            }
                        }
                    }
                }

                mit->second->SetAttackedMonster(true);
                pDamageInstance->m_pOriginalDamage->CrashEffect(pDamageInstance);
                pDamageInstance->m_pOriginalDamage->DamageHpEffect(pDamageInstance);
            }

        }

    }
}

bool CDamageManager::CrashWithMonsterPhsAtk(int iMonsterIndex, GCCollisionRect<float>* rtWeapon)
{
    GCCollisionRect<float> rtFire, rtCollision;
    int iIndex = LATENCY_INDEX;
    if (!g_kGlobalValue.m_kUserInfo.bHost)
        iIndex = 0;

    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.begin();
    CDamageInstance* pDamageInstance;



    while (itor != m_mapDamageInstance.end())
    {
        for (int j = (int)itor->second.size() - 1; j >= 0; --j)
        {
            pDamageInstance = itor->second[j];

            if (false == pDamageInstance->m_pOriginalDamage->m_vecPermissionDamageHP.empty())
                continue;

            if (!pDamageInstance->m_pOriginalDamage->m_iDamageHP)
                continue;

            //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
            if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
                && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
                continue;

            MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.find(iMonsterIndex);

            if (mit == g_kMonsterManager.m_mapMonster.end() || !mit->second->m_bLive)
                return false;


            //�����Ѱ� �ƴ϶�� �н�
            if (mit->second->m_vecPhsAtk.empty())
                continue;

            float fRangeX = 1.0f;
            float fRangeY = 1.0f;

            rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX - 1.0f;
            rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX - 1.0f;
            rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
            rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
            rtCollision = rtFire;

            if (rtFire.CheckCollision(*rtWeapon, &rtCollision))
            {
                pDamageInstance->m_pOriginalDamage->m_iDamageHP--;
                mit->second->m_iSlowCount = 20;

                if (pDamageInstance->m_pOriginalDamage->m_iDamageHP <= 0)
                {
                    pDamageInstance->m_Life = 1;

                    if (pDamageInstance->m_What == DT_MARI3_BUILD_MANA_DEVICE)
                    {
                        bool bTeamBuffEnd = false;
                        bool bEnemyBuffEnd = false;

                        std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(pDamageInstance->m_What);

                        if (itor != m_mapDamageInstance.end()) {
                            for (int i = 0; i < (int)itor->second.size(); i++)
                            {
                                if (itor->second[i]->m_Who == pDamageInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_TEAM_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bTeamBuffEnd = true;
                                }

                                if (itor->second[i]->m_Who == pDamageInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_ENEMY_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bEnemyBuffEnd = true;
                                }
                            }
                        }
                    }
                }
                pDamageInstance->m_pOriginalDamage->CrashEffect(pDamageInstance);
                pDamageInstance->m_pOriginalDamage->DamageHpEffect(pDamageInstance);
            }
        }
        ++itor;
    }
    return false;
}

void CDamageManager::CrashWithObject(CDamageInstance* pDamageInstance)
{
    //ġ���ϴ� �͵��� ���Ϳʹ� ����� ����.
    if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) || (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS))
        return;

    // ���� ������Ʈ�� �浹ó�� ���� �ʴ´�.
    if (pDamageInstance->m_pOriginalDamage->m_bNoCrashGameObject)
        return;

    // ������ �����ϰ��� �浹�ϴ� �������� ������
    if (false == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.empty())
        return;

    bool bOnlyOneCollision = false;
    for (int i = 0; i < MAX_OBJECT; i++)
    {
        if (!g_MyD3D->m_pObject[i]) continue;
        if (!g_MyD3D->m_pObject[i]->IsLive())
            continue;

        if (pDamageInstance->m_IsMonsterDamage && g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i))
            continue;

        //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
        if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
            && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
            continue;

        //bool bCurMonsterRight = g_MyD3D->m_pObject[i]->m_abIsRight[LATENCY_INDEX];    // CrashPlayer �Լ��� ȣ��Ǹ� �ٶ󺸴� ������ �ٲ�Ƿ� �̸� �޾Ƶδ� ��.

        float fRangeX = 0.0f;
        float fRangeY = 0.0f;
        if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
        {
            fRangeX = 1.0f;
            fRangeY = 1.0f;
        }
        else
        {
            fRangeX = 0.5f;
            fRangeY = 0.4f;
        }

        GCCollisionRect<float> rtFire;
        rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX - 1.0f;
        rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX - 1.0f;
        rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
        rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;

        GCCollisionRect<float> rtObject = g_MyD3D->m_pObject[i]->GetCollisionRect();
        if (!g_MyD3D->m_pObject[i]->IsCrashCheck() || rtObject.CheckCollision(rtFire, NULL) == false)
            continue;

        CallEffAtCrash(pDamageInstance, i, false);
        pDamageInstance->m_pOriginalDamage->CrashObject(pDamageInstance, i);
        pDamageInstance->SetCrashCheck(true);

        bOnlyOneCollision = true;

        if (g_MyD3D->GetMyPlayer() && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer()) {
            g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk(pDamageInstance->m_What);
            g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk(pDamageInstance->m_What);
            g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk(pDamageInstance->m_What);
        }

        if (pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer()
            && !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_NOT_COMBO_COUNT))
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(i);
            if (pMonster != NULL) {
                pMonster->SetFlashEffectFrame(FLASH_EFFECT_FRAME);
                pMonster->SetShakeEffectFrame(10);
            }
        }
    }

    if (bOnlyOneCollision) {
        g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk(pDamageInstance->m_What, true);
    }
}

bool CDamageManager::CrashWithWeapon(int iPlayerIndex, GCCollisionRect<float>* rtWeapon)
{
    GCCollisionRect<float> rtFire, rtCollision; //, rtWeapon, ;
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.begin();
    CDamageInstance* pDamageInstance;



    while (itor != m_mapDamageInstance.end())
    {
        for (int j = (int)itor->second.size() - 1; j >= 0; --j)
        {
            pDamageInstance = itor->second[j];
            int i = iPlayerIndex;

            if (false == pDamageInstance->m_pOriginalDamage->m_vecPermissionDamageHP.empty())
                continue;

            if (!pDamageInstance->m_pOriginalDamage->m_iDamageHP)
                continue;

            if (g_MyD3D->MyPlayer[i]->m_kUserInfo.cCharIndex == -1)
                continue;

            if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false)
                continue;

            if (!pDamageInstance->m_IsMonsterDamage &&
                pDamageInstance->m_What != DT_SKILL_LAS_TRADWARM_POT &&
                g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i))
                continue;

            if (pDamageInstance->m_What != DT_SKILL_LAS_TRADWARM_POT
                && pDamageInstance->m_Who == i)
                continue;

            //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
            if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
                && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
                continue;

            //if( g_MyD3D->GetMyPlayer()->m_kUserInfo.iTeam == g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.iTeam )
            //	continue;

            rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x - 1.0f;
            rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x - 1.0f;
            rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x - 0.26f;
            rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x - 0.26f;
            rtCollision = rtFire;

            if (rtFire.CheckCollision(*rtWeapon, &rtCollision))
            {
                pDamageInstance->m_pOriginalDamage->m_iDamageHP--;

                if (pDamageInstance->m_What == DT_SKILL_RYAN_STONE_WALL)
                {
                    // ���� �Ǵ� 4! ������ ���������� ������ 2�� ������ ������
                    pDamageInstance->m_pOriginalDamage->m_iDamageHP--;
                }
                else if (pDamageInstance->m_What == DT_TALIN_SUPERFRAME_LAUNCHER ||
                    pDamageInstance->m_What == DT_TALIN_SUPERFRAME_LAUNCHER_BOTTOM)
                {
                    g_MyD3D->MySparks->Particles(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.3f, 0, 15, 15);
                    g_MyD3D->MySparks->Add(HIT_LIGHT, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.3f, 0, 0.7f);
                }
                else if (pDamageInstance->m_What == DT_DEBRIS_MAGE_HEAL_TOTEM)
                {
                    g_MyD3D->MySparks->Particles(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.3f, 0, 15, 15);
                    g_MyD3D->MySparks->Add(HIT_LIGHT, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.3f, 0, 0.7f);
                }

                g_MyD3D->MyPlayer[i]->Slow_Count = 8;
                g_MyD3D->MyPlayer[i]->Next_Attack_Count = 15;

                CallEffAtCrash(pDamageInstance, -1, false, rtWeapon->m_Left + 1.0f, rtWeapon->m_Top + 0.25f);

                if (pDamageInstance->m_pOriginalDamage->m_iDamageHP <= 0)
                {
                    pDamageInstance->m_Life = 1;

                    if (pDamageInstance->m_What == DT_MARI3_BUILD_MANA_DEVICE)
                    {
                        bool bTeamBuffEnd = false;
                        bool bEnemyBuffEnd = false;

                        std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(pDamageInstance->m_What);

                        if (itor != m_mapDamageInstance.end()) {
                            for (int i = 0; i < (int)itor->second.size(); i++)
                            {
                                if (itor->second[i]->m_Who == pDamageInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_TEAM_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bTeamBuffEnd = true;
                                }

                                if (itor->second[i]->m_Who == pDamageInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_ENEMY_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bEnemyBuffEnd = true;
                                }
                                if (bTeamBuffEnd && bEnemyBuffEnd)
                                    break;
                            }
                        }
                    }
                }

                pDamageInstance->m_pOriginalDamage->CrashEffect(pDamageInstance);
                pDamageInstance->m_pOriginalDamage->DamageHpEffect(pDamageInstance);
                return true;
            }
        }
        ++itor;
    }
    return false;
}

void CDamageManager::CrashWithMinigameObj(CDamageInstance* pDamageInstance)
{
    //ġ���ϴ� �͵��� ����� ����.
    if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) || (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS))
        return;

    //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
    if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
        && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
        return;

    // ������ �����ϰ��� �浹�ϴ� �������� ������
    if (false == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.empty())
        return;

    //bool bCurMonsterRight = g_MyD3D->m_pObject[i]->m_abIsRight[LATENCY_INDEX];    // CrashPlayer �Լ��� ȣ��Ǹ� �ٶ󺸴� ������ �ٲ�Ƿ� �̸� �޾Ƶδ� ��.
    float fRangeX = 0.0f;
    float fRangeY = 0.0f;
    if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
    {
        fRangeX = 1.0f;
        fRangeY = 1.0f;
    }
    else
    {
        fRangeX = 0.5f;
        fRangeY = 0.4f;
    }

    GCCollisionRect<float> rtFire;
    rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX - 1.0f;
    rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX - 1.0f;
    rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
    rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;

    bool bOneHit;
    if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE)
        bOneHit = true;
    else
        bOneHit = false;

    std::pair<bool, GCCollisionRect<float> >& HitInfo = SiKGCMinigameManager()->HitObject(&rtFire, bOneHit, pDamageInstance->m_What);

    if (HitInfo.first == true)
    {
        CallEffAtCrash(pDamageInstance, -1, false, HitInfo.second.m_Left + 1.0f, HitInfo.second.m_Top + 0.25f);

        if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONETIME_DAMAGE) && pDamageInstance->m_Life > 1)
        {
            pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
            pDamageInstance->m_Life = 1;
        }

        pDamageInstance->m_pOriginalDamage->CrashEffect(pDamageInstance);
    }
}

void CDamageManager::CrashWithEffect(CDamageInstance* pDamageInstance)
{
    CDamageInstance* pShieldInstance = NULL;

    // ������ �����ϰ��� �浹�ϴ� �������� ������
    if (false == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.empty())
        return;

    // �������� �浹ó�� ���� �ʴ´�
    if (pDamageInstance->m_pOriginalDamage->m_bNoCrashDamage)
        return;

    int itestWhat = pDamageInstance->m_What;
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.begin();
    while (itor != m_mapDamageInstance.end()) {
        for (int j = (int)itor->second.size() - 1; j >= 0; --j)
        {
            pShieldInstance = itor->second[j];

            float fRangeX = 0.0f;
            float fRangeY = 0.0f;
            float fShieldRangeX = 0.0f;
            float fShieldRangeY = 0.0f;

            // ������ HP�� �ִ� �������� ������ �� �µ��� ����. 
            if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
            {
                fRangeX = 1.0f;
                fRangeY = 1.0f;
            }
            else
            {
                fRangeX = 0.5f;
                fRangeY = 0.4f;
            }

            if (pShieldInstance->m_pOriginalDamage->m_bIsTotalRange)
            {
                fShieldRangeX = 1.0f;
                fShieldRangeY = 1.0f;
            }
            else
            {
                fShieldRangeX = 0.5f;
                fShieldRangeY = 0.8f;
            }

            // ���� �ȵ���
            if (pDamageInstance == pShieldInstance)
                continue;

            // üũ�� ������ ���� ��� �ϵ��ϰ� ó����.
            if (DT_RIN2_GODDESS_SPECIAL2_SHIELD == pShieldInstance->m_What ||
                DT_RIN2_GODDESS_SPECIAL2_SHIELD_FORCED == pShieldInstance->m_What ||
                DT_RIN2_GODDESS_SPECIAL2_SHIELD_EFFECT == pShieldInstance->m_What) {
                if (pDamageInstance->m_pOriginalDamage->m_iDamageHP != 0) {
                    continue;
                }
            }
            else if (DT_RIN2_GODDESS_SPECIAL2_SHIELD == pDamageInstance->m_What ||
                DT_RIN2_GODDESS_SPECIAL2_SHIELD_FORCED == pDamageInstance->m_What ||
                DT_RIN2_GODDESS_SPECIAL2_SHIELD_EFFECT == pDamageInstance->m_What) {
                if (pShieldInstance->m_pOriginalDamage->m_iDamageHP != 0) {
                    continue;
                }
            }

            // ���� ���ΰ�?
            if ((pDamageInstance->m_iTeam == pShieldInstance->m_iTeam) &&
                // ������HP�Ӽ��� �ְų� NoPassDamage�Ӽ��� �ִ°�?
                (pShieldInstance->m_pOriginalDamage->m_iDamageHP || pDamageInstance->m_pOriginalDamage->m_bNoPassDamage) &&
                // �� ��ų�������ΰ�?
                (DT_SKILL_LAS_TRADWARM_POT != pShieldInstance->m_What) &&
                // ������ �������� �浹�㰡�Ǿ��°�?
                (false == pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_bNoCheckSameTeam))
                continue;

            if (DT_SKILL_MAGIC_MISSILE_READY != pShieldInstance->m_What
                && DT_SKILL_LAS_BOMBDART_DART != pShieldInstance->m_What
                && DT_SKILL_LAS_BOMBDART_SCROLL_TIMER != pShieldInstance->m_What
                && DT_SKILL_LAS_TRADWARM_POT != pShieldInstance->m_What)
            {
                // ������ �ݻ��ϸ� �ȵǰ���?
                if (pDamageInstance->m_IsMonsterDamage == false)
                {
                    if (pDamageInstance->m_Who == pShieldInstance->m_Who &&
                        (false == pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_bNoCheckSameTeam))
                        continue;
                }
                else
                {
                    //if ( pShieldInstance->m_IsMonsterDamage && pShieldInstance->m_iTeam == -1 )	// �ٳ׻�NPC�� �츮 ���̴� �ݻ��� �� �־� ����
                        //continue;
                }
            }

            // Right : pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f
            // Left : pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f
            // Top : pDamageInstance->m_Y + pDamageInstance->m_Scale_x * m_Info[pDamageInstance->m_What].m_Y_per_x * 0.5f
            // Bottom : pDamageInstance->m_Y - pDamageInstance->m_Scale_x * m_Info[pDamageInstance->m_What].m_Y_per_x * 0.5f

            //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
            if (!(pShieldInstance->m_Life <= pShieldInstance->m_pOriginalDamage->m_iCrashStart
                && pShieldInstance->m_Life >= pShieldInstance->m_pOriginalDamage->m_iCrashEnd))
                continue;

            // Right to Left ��
            if (pShieldInstance->m_X + pShieldInstance->m_Scale_x * fShieldRangeX < pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX)
                continue;

            // Left to Right ��
            if (pShieldInstance->m_X - pShieldInstance->m_Scale_x * fShieldRangeX > pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX)
                continue;

            // 0.6 -> 0.8�� �� �÷����� --;

            // Top to Bottom ��
            if (pShieldInstance->m_Y + pShieldInstance->m_Scale_x * pShieldInstance->m_pOriginalDamage->m_Y_per_x * fShieldRangeY
                < pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY)
                continue;

            // Bottom to Top ��
            if (pShieldInstance->m_Y - pShieldInstance->m_Scale_x * pShieldInstance->m_pOriginalDamage->m_Y_per_x * fShieldRangeY
                > pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY)
                continue;

            // �浹 �Ǿ�3

            if (pShieldInstance->m_What == DT_SKILL_LAS_SPIKE_TRAP ||
                pShieldInstance->m_What == DT_SKILL_LAS_CANON_TRAP ||
                pShieldInstance->m_What == DT_SKILL_RYAN_HP_TOTEM ||
                pShieldInstance->m_What == DT_SKILL_RYAN_DP_TOTEM ||
                pShieldInstance->m_What == DT_SKILL_RYAN_MP_TOTEM ||
                pShieldInstance->m_What == DT_SKILL_RYAN_HP_TOTEM_DUN ||
                pShieldInstance->m_What == DT_SKILL_RYAN_DP_TOTEM_DUN ||
                pShieldInstance->m_What == DT_SKILL_RYAN_MP_TOTEM_DUN ||
                pShieldInstance->m_What == DT_SKILL_RYAN_AP_TOTEM_DUN ||
                pShieldInstance->m_What == DT_PERIOT_HOMMING_MISSILE_HP ||
                pShieldInstance->m_What == DT_SUPERMON_PERIOT_HOMMING_MISSILE_HP ||
                pShieldInstance->m_What == DT_PERIOT_HOMMING_MISSILE_DESTROY ||
                pDamageInstance->m_What == DT_DARKTEMPLAR_SOUL_DRAIN ||
                pDamageInstance->m_What == DT_MARI2_BUILDING_LV1_JUMP_TRAP ||
                pDamageInstance->m_What == DT_HERO_GADOSEN_LIGHTOFHELL ||
                pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE1_LIGHTOFHELL ||
                pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE2_LIGHTOFHELL)
            {
                if (pDamageInstance->m_iTeam == pShieldInstance->m_iTeam)
                    continue;
                if (pDamageInstance->m_What == DT_XENIA5_MAP_DAMAGE || pDamageInstance->m_What == DT_XENIA5_MAP_DOTDAMAGE)
                    continue;

                goto IgnoreNoPassDamage;
            }

            // �������� ������ �浹�� ó��
            if (pDamageInstance->m_pOriginalDamage->m_fDamageCrashSpeedX || pDamageInstance->m_pOriginalDamage->m_fDamageCrashSpeedY)
                pShieldInstance->SetDamageCrashSpeed(pDamageInstance->m_pOriginalDamage->m_fDamageCrashSpeedX, pDamageInstance->m_pOriginalDamage->m_fDamageCrashSpeedY);

            // ������ �������� �������� �����ϱ����� ��
            if (pDamageInstance->m_pOriginalDamage->m_IsStaticDamageCleaner &&
                pShieldInstance->m_pOriginalDamage->m_IsStaticDamage)
            {
                pShieldInstance->m_Life = 1;
                goto IgnoreNoPassDamage;
            }

            // ���� �������� �������� �浹������ �Ӽ��߰��ϰ� ���⼭ ó�� �սô�
            if (pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_bSetStruct)
            {
                // �������̸� �������� ������ �浹ó�� ����
                if ((false == pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_bNoCheckSameTeam) &&
                    (pDamageInstance->m_iTeam == pShieldInstance->m_iTeam))
                {
                    goto GOTO_DAMAGE_TO_DAMAGE_END;
                }

                if (false == pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecExceptDamageEnum.empty())
                {
                    std::vector<int>::iterator vecExceptDamageEnumIter = pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecExceptDamageEnum.begin();
                    for (; vecExceptDamageEnumIter != pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecExceptDamageEnum.end(); ++vecExceptDamageEnumIter)
                    {
                        if ((*vecExceptDamageEnumIter) == pShieldInstance->m_What)
                            break;
                    }
                    if (vecExceptDamageEnumIter != pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecExceptDamageEnum.end())
                        continue;
                }

                if (false == pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecApointDamageEnum.empty())
                {
                    std::vector<int>::iterator vecApointDamageEnumIter = pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecApointDamageEnum.begin();
                    for (; vecApointDamageEnumIter != pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecApointDamageEnum.end(); ++vecApointDamageEnumIter)
                    {
                        if ((*vecApointDamageEnumIter) == pShieldInstance->m_What)
                            break;
                    }
                    if (vecApointDamageEnumIter == pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_vecApointDamageEnum.end())
                        continue;
                }

                if (-1 != pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_iMyLife)
                    pDamageInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_iMyLife;
                if (-1 != pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_iYouLife)
                    pShieldInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_iYouLife;

                // ������ ���� ó��
                if (pDamageInstance->m_pOriginalDamage->m_kDamageToDamage.m_bSummonMonster)
                {
                    if (-1 < pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_iMonsterID)
                    {
                        if (-1 == pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_iSummonLifeTime)
                        {
                            pDamageInstance->m_pOriginalDamage->SummonMonster(pDamageInstance);
                        }
                    }
                }
            }
        GOTO_DAMAGE_TO_DAMAGE_END:

            // ������ HPó���ϱ� ���� ��
            if (pDamageInstance->m_pOriginalDamage->m_IsEnableDamage &&
                (pShieldInstance->m_pOriginalDamage->m_bNoPassDamage || pShieldInstance->m_pOriginalDamage->m_iDamageHP))
            {

                if (false == pShieldInstance->m_pOriginalDamage->m_vecPermissionDamageHP.empty())
                {
                    std::vector<int>::iterator vecPermissionIter = pShieldInstance->m_pOriginalDamage->m_vecPermissionDamageHP.begin();
                    for (; vecPermissionIter != pShieldInstance->m_pOriginalDamage->m_vecPermissionDamageHP.end(); ++vecPermissionIter)
                    {
                        if ((*vecPermissionIter) == pDamageInstance->m_What)
                            break;
                    }
                    if (vecPermissionIter == pShieldInstance->m_pOriginalDamage->m_vecPermissionDamageHP.end())
                        continue;
                }

            IgnoreNoPassDamage:
                if (pDamageInstance->m_What == DT_TALIN_SUPERFRAME_LAUNCHER ||
                    pDamageInstance->m_What == DT_TALIN_SUPERFRAME_LAUNCHER_BOTTOM)
                {
                    pDamageInstance->CreateSequence("Talin_Launcher_Spark_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                }

                // ���������� �ִ� ���ݿ��� �ڽ��������� hp�� �������.
                if (g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who)) {
                    if (pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer != 0.f) {
                        pShieldInstance->m_pOriginalDamage->m_iDamageHP--;
                    }
                }
                else {
                    if (pDamageInstance->m_pOriginalDamage->m_fDamageToMonster != 0.f) {
                        pShieldInstance->m_pOriginalDamage->m_iDamageHP--;
                    }
                }

                if (pShieldInstance->m_pOriginalDamage->m_iDamageHP <= 0)
                {
                    pShieldInstance->m_Life = 1;

                    if (pShieldInstance->m_What == DT_MARI3_BUILD_MANA_DEVICE)
                    {
                        bool bTeamBuffEnd = false;
                        bool bEnemyBuffEnd = false;

                        std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(pDamageInstance->m_What);

                        if (itor != m_mapDamageInstance.end()) {
                            for (int i = 0; i < (int)itor->second.size(); i++)
                            {
                                if (itor->second[i]->m_Who == pShieldInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_TEAM_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bTeamBuffEnd = true;
                                }

                                if (itor->second[i]->m_Who == pShieldInstance->m_Who &&
                                    itor->second[i]->m_What == DT_MARI3_BUILD_MANA_DEVICE_ENEMY_BUFF)
                                {
                                    itor->second[i]->m_Life = 1;
                                    bEnemyBuffEnd = true;
                                }
                            }
                        }
                    }
                }

                pShieldInstance->m_pOriginalDamage->DamageHpEffect(pShieldInstance);

                if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONETIME_DAMAGE)
                {
                    pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
                    pDamageInstance->m_Life = 1;
                }

                if (pShieldInstance->m_pOriginalDamage->m_bNoPassDamage)
                    if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE)
                        pDamageInstance->m_Life = 1;
            }

            // �ݻ� ���� �Ӽ��� ������ �ݻ� �� �� ����
            if (!(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_DO_NOT_REFLECTION)
                && !(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_PHYSICAL))
            {
                CallEffAtEffectCrash(pDamageInstance, pShieldInstance);
            }

            // �ݻ� �Ӽ��� �����ϰ� ����Ʈ���� �浹ó��
            CallEffAtEffectCrashForce(pDamageInstance, pShieldInstance);
        }
        // Next!!
        ++itor;
    }
}

void CDamageManager::CallEffAtEffectCrash(CDamageInstance* pDamageInstance, CDamageInstance* pShieldInstance)
{
    bool bSameTeam = g_MyD3D->IsSameTeam(pDamageInstance->m_Who, pShieldInstance->m_Who);

    //���͵� ���� ���� �ִ�!!!
    if (pDamageInstance->m_IsMonsterDamage)
    {
        bSameTeam = g_MyD3D->IsSameTeam_PlayerAndMonster(pShieldInstance->m_Who, pDamageInstance->m_ExtraMonsterNum);
    }

    for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecAutoDamage.size(); ++i)
    {
        AUTO_DAMAGE& autoDamage = pDamageInstance->m_pOriginalDamage->m_vecAutoDamage[i];

        if (autoDamage.m_iDamageIndex == -1)
            break;

        if (false == (autoDamage.m_dwAutoDamage & EFFECT_CRASH_AUTO_DAMAGE))
            continue;

        if (autoDamage.m_iCrashStartTime != -1 && pDamageInstance->m_Life > autoDamage.m_iCrashStartTime)
            continue;

        if (pDamageInstance->CheckAlreadyDamaged(pDamageInstance->m_HitWho)
            && (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE))
        {
            continue;
        }

        D3DXVECTOR2 offset;
        offset.y = autoDamage.m_fOffSetY;
        if (pDamageInstance->m_IsRight)
        {
            offset.x = autoDamage.m_fOffSetX;
        }
        else
        {
            offset.x = -autoDamage.m_fOffSetX;
        }

        CDamageInstance* pAuto;
        if (false == pDamageInstance->m_IsMonsterDamage)
        {
            pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, pDamageInstance->m_X + offset.x,
                pDamageInstance->m_Y + offset.y, autoDamage.m_bNoSlowCount, -1, NULL, pDamageInstance->m_HitWho);
        }
        else
        {
            pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, pDamageInstance->m_X + offset.x,
                pDamageInstance->m_Y + offset.y, autoDamage.m_bNoSlowCount, pDamageInstance->m_ExtraMonsterNum, NULL, pDamageInstance->m_HitWho);
        }

        if (pAuto)
        {
            pAuto->m_bPetDamage = pDamageInstance->m_bPetDamage;
            pAuto->m_IsRight = pDamageInstance->m_IsRight;
            pAuto->m_SlowCount = 0;
        }

        if (autoDamage.m_bNoClear == false)
            autoDamage.m_dwAutoDamage = 0;
    }

    switch (pShieldInstance->m_What)
    {

    case DT_ARME3_MAGIC_SHIELD2:
    case DT_ARME4_MAGIC_SHIELD:
    case DT_SHIELDMAN_3_SHIELD:
    case DT_RONAN1_SKILL_AREA_REF_DEF_BUFF:
    case DT_RONAN1_SKILL_AREA_REF_DEF_BUFF_DUN:
    case DT_ZERO2_DEFENCE_REFLECTION:
    case DT_ZERO3_DEFENCE_REFLECTION:
    case DT_MARI1_MINI_OBSTACLE_DMG_SP4:
        if (bSameTeam == false)
        {
            // ������ �ƴ� ȭ�츸 �ݻ� --+ ������ �ƴ� ���̾�� �ݻ�
            if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) || pDamageInstance->m_What == DT_FIREBALL || pDamageInstance->m_What == DT_ARME4_FIREBALL)
            {
                pDamageInstance->m_Who = pShieldInstance->m_Who;
                pDamageInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_Start_Life;
                pDamageInstance->m_Scale_x = pDamageInstance->m_pOriginalDamage->m_Start_Scale_x;
                pDamageInstance->m_IsRight = !pDamageInstance->m_IsRight;
                pDamageInstance->m_iTeam = pShieldInstance->m_iTeam;

                if (pShieldInstance->m_What == DT_SHIELDMAN_3_SHIELD)
                {
                    if (pShieldInstance->m_IsRight)
                    {
                        pDamageInstance->CreateSequence("mon_defense01", pShieldInstance->m_X - 1.0f, pShieldInstance->m_Y - 0.5f, 0.5f);
                    }
                    else
                    {
                        pDamageInstance->CreateSequence("mon_defense01_R", pShieldInstance->m_X - 1.0f, pShieldInstance->m_Y - 0.5f, 0.5f);
                    }
                }
            }
        }
        break;
    case DT_SKILL_ELESIS1_FAST_ATK:
    case DT_SKILL_ELESIS1_MAGIC_DEFENSE:
        if (bSameTeam == false)
        {
            if (!pDamageInstance->m_pOriginalDamage->m_bSpecialAttack)
            {
                if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) /*|| ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONETIME_DAMAGE )*/ ||
                    pDamageInstance->m_What == DT_FIREBALL || pDamageInstance->m_What == DT_ARME4_FIREBALL)
                {
                    if (pDamageInstance->m_Life > 1)
                    {
                        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                        {
                            pDamageInstance->CreateSequence("ELESIS1_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                            pDamageInstance->CreateSequence("ELESIS1_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                            pDamageInstance->CreateSequence("ELESIS1_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                        }
                        g_KDSound.Play("855");
                        pDamageInstance->m_Life = 0;
                        g_MyD3D->MyPlayer[pShieldInstance->m_Who]->AddDamage(DT_DEFENCE, 0);

                        for (unsigned int i = 0; i < pShieldInstance->m_pOriginalDamage->m_vecCrashParticle.size(); i++)
                        {
                            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                            {
                                pShieldInstance->CreateSequence(pShieldInstance->m_pOriginalDamage->m_vecCrashParticle[i].m_strParticle
                                    , pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                            }

                        }

                    }
                }
            }
        }
        break;
    case DT_RONAN3_GUARD_CANCEL_ATK:
    case DT_RONAN3_BLOCK_CANCEL_ATK:
    case DT_RIN4_FORM_DEFFENCE_PLUS_DELAY:
    case DT_RIN4_FORM_DEFFENCE_REFLECT_DAMAGE:
    case DT_ELESIS_BLOCK_CANCEL_ATK:
        if (bSameTeam == false)
        {
            // �ڿ��� ���� ���� �����Ѵ�. �Ƴ� �ĺ��� �̻��� �� �켱 �� ����;;
            if (pDamageInstance->m_IsRight != pShieldInstance->m_IsRight && pDamageInstance->m_What != DT_FIREBALL)
            {
                pShieldInstance->m_Life = 0;
                return;
            }

            // �γ��� ���� �ϸ� �������� �������������������������� ������
            if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) || pDamageInstance->m_What == DT_FIREBALL
                || pDamageInstance->m_What == DT_ARME4_FIREBALL || pDamageInstance->m_What == DT_ARME3_ICEBALL)
            {
                pDamageInstance->m_Life = 0;
                // ���� ���� ������ ���� �޺� �����;;;
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count = 0;

                if (pShieldInstance->m_What == DT_RONAN3_GUARD_CANCEL_ATK)
                {
                    g_MyD3D->MyPlayer[pShieldInstance->m_Who]->AddDamage(DT_GUARD, 0);
                }

                int iRandNum = (rand() % 2) + 753;
                std::stringstream str;
                str << iRandNum;
                g_KDSound.Play(str.str().c_str());
            }
        }
        break;
    case DT_MONSTER_PARADOM_ATFIELD:
        if (pDamageInstance->m_pOriginalDamage->m_IsEnableDamage)
        {
            // DA_TRACE_CREATOR �Ӽ��� �༮�� �������� ������ �ٲ������ �� �۵��� ���ؼ� �������� �ȹް� �������..
            // ex) DT_TORNADO_SPIN
            if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_TRACE_CREATOR)
            {
                // ��ȣ�� �����̴� ����� �������� ����������..
                pDamageInstance->m_pOriginalDamage->m_fDamageToMonster = 0.0f;
            }
            else
            {
                pDamageInstance->m_Who = pShieldInstance->m_Who;
                pDamageInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_Start_Life;
                pDamageInstance->m_Scale_x = pDamageInstance->m_pOriginalDamage->m_Start_Scale_x;
                pDamageInstance->m_IsRight = !pDamageInstance->m_IsRight;
                pDamageInstance->m_iTeam = pShieldInstance->m_iTeam;
                pDamageInstance->m_ExtraMonsterNum = pShieldInstance->m_ExtraMonsterNum;
            }
        }
        break;
    case DT_RONAN4_UNIQUE_ATK_SWORD:
        if (bSameTeam == false)
        {
            if (pDamageInstance->m_IsRight != pShieldInstance->m_IsRight && pDamageInstance->m_What != DT_FIREBALL)
            {
                pShieldInstance->m_Life = 0;
                return;
            }

            if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) || pDamageInstance->m_What == DT_FIREBALL
                || pDamageInstance->m_What == DT_ARME4_FIREBALL || pDamageInstance->m_What == DT_ARME3_ICEBALL)
            {
                pDamageInstance->m_Life = 0;
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count = 0;

                if (pShieldInstance->m_What == DT_RONAN4_UNIQUE_ATK_SWORD)
                {
                    g_MyD3D->MyPlayer[pShieldInstance->m_Who]->AddDamage(DT_GUARD, 0);
                }

                //g_KDSound.Play();
            }
        }
        break;
    case DT_VANESSA_SHIELD:
        // �ƽ�Ÿ���� ���̾�θ� �ݻ�
        if (pDamageInstance->m_What == DT_METEO)
        {
            pDamageInstance->CreateSequence("ELESIS1_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            g_KDSound.Play("855");
            pDamageInstance->m_Life = 0;
            g_MyD3D->MyPlayer[pShieldInstance->m_Who]->AddDamage(DT_DEFENCE, 0);
        }
        break;
    case DT_FREEWAY_ARROW:
    case DT_FREEWAY_ARROW_LIGHTNING:
    case DT_FREEWAY_ARROW_SHINING:
    case DT_FREEWAY_ARROW_LOVE:
    case DT_FREEWAY_ARROW_DUN:
    case DT_FREEWAY_ARROW_LIGHTNING_DUN:
    case DT_FREEWAY_ARROW_SHINING_DUN:
    case DT_FREEWAY_ARROW_LOVE_DUN:
    case DT_ARROW:
    case DT_ARROW_LIGHTNING:
    case DT_ARROW_SHINING:
    case DT_ARROW_LOVE:
    case DT_BOLT:
    case DT_BOLT_LIGHTNING:
    case DT_BOLT_SHINING:
    case DT_BOLT_LOVE:
    case DT_LAS_JUMP2_ATTACK:
    case DT_HUGE_ARROW:
    case DT_HUGE_ARROW_LIGHTNING:
    case DT_HUGE_ARROW_SHINING:
    case DT_HUGE_ARROW_LOVE:
    case DT_HUGE_JUMPDOWN_ARROW:
    case DT_HUGE_JUMPDOWN_ARROW_LIGHTNING:
    case DT_HUGE_JUMPDOWN_ARROW_SHINING:
    case DT_HUGE_JUMPDOWN_ARROW_LOVE:
    case DT_NOVA_BOLT:
    case DT_NOVA_FREEWAY_BOLT:
    case DT_NOVA_FREEWAY_BOLT_DUN:
    case DT_NOVA_ARROW:
    case DT_NOVA_SPECIAL1_BOLT:
    case DT_NOVA_BOLT_LIGHTNING:
    case DT_NOVA_FREEWAY_BOLT_LIGHTNING:
    case DT_NOVA_FREEWAY_BOLT_LIGHTNING_DUN:
    case DT_NOVA_ARROW_LIGHTNING:
    case DT_NOVA_SPECIAL1_BOLT_LIGHTNING:
    case DT_NOVA_BOLT_SHINING:
    case DT_NOVA_FREEWAY_BOLT_SHINING:
    case DT_NOVA_FREEWAY_BOLT_SHINING_DUN:
    case DT_NOVA_ARROW_SHINING:
    case DT_NOVA_SPECIAL1_BOLT_SHINING:
    case DT_NOVA_BOLT_DARK:
    case DT_NOVA_FREEWAY_BOLT_DARK:
    case DT_NOVA_FREEWAY_BOLT_DARK_DUN:
    case DT_NOVA_ARROW_DARK:
    case DT_NOVA_SPECIAL1_BOLT_DARK:
    case DT_NOVA_BOLT_LOVE:
    case DT_NOVA_FREEWAY_BOLT_LOVE:
    case DT_NOVA_FREEWAY_BOLT_LOVE_DUN:
    case DT_NOVA_ARROW_LOVE:
    case DT_NOVA_SPECIAL1_BOLT_LOVE:
    case DT_NOVA_DOWN_ARROW:
    case DT_NOVA_DOWN_ARROW_LIGHTNING:
    case DT_NOVA_DOWN_ARROW_SHINING:
    case DT_NOVA_DOWN_ARROW_DARK:
    case DT_NOVA_DOWN_ARROW_LOVE:
    case DT_CRITICAL_ARROW:
    case DT_CRITICAL_ARROW_LIGHTNING:
    case DT_CRITICAL_ARROW_SHINING:
    case DT_CRITICAL_ARROW_LOVE:
    case DT_DOWN_ARROW:
    case DT_DOWN_ARROW_LIGHTNING:
    case DT_DOWN_ARROW_SHINING:
    case DT_DOWN_ARROW_LOVE:
    case DT_DOWN_ARROW_DUN:
    case DT_DOWN_ARROW_LIGHTNING_DUN:
    case DT_DOWN_ARROW_SHINING_DUN:
    case DT_DOWN_ARROW_LOVE_DUN:
    case DT_CRITICAL_BOLT:
    case DT_CRITICAL_BOLT_LIGHTNING:
    case DT_CRITICAL_BOLT_SHINING:
    case DT_CRITICAL_BOLT_LOVE:
    case DT_UP_ARROW:
    case DT_UP_ARROW_LIGHTNING:
    case DT_UP_ARROW_SHINING:
    case DT_UP_ARROW_LOVE:
    case DT_HUGE_CRITICAL_ARROW:
    case DT_HUGE_CRITICAL_ARROW_LIGHTNING:
    case DT_HUGE_CRITICAL_ARROW_SHINING:
    case DT_HUGE_CRITICAL_ARROW_LOVE:
    case DT_HUGE_RUNATK_ARROW:
    case DT_HUGE_RUNATK_ARROW_LIGHTNING:
    case DT_HUGE_RUNATK_ARROW_SHINING:
    case DT_HUGE_RUNATK_ARROW_LOVE:
    case DT_HUGE_COMBO3_ARROW:
    case DT_HUGE_COMBO3_ARROW_LIGHTNING:
    case DT_HUGE_COMBO3_ARROW_SHINING:
    case DT_HUGE_COMBO3_ARROW_LOVE:
    case DT_NOVA_CRITICAL_ARROW:
    case DT_NOVA_CRITICAL_BOLT:
    case DT_NOVA_SPECIAL1_ARROW:
    case DT_NOVA_CRITICAL_ARROW_LIGHTNING:
    case DT_NOVA_CRITICAL_BOLT_LIGHTNING:
    case DT_NOVA_SPECIAL1_ARROW_LIGHTNING:
    case DT_NOVA_CRITICAL_ARROW_SHINING:
    case DT_NOVA_CRITICAL_BOLT_SHINING:
    case DT_NOVA_SPECIAL1_ARROW_SHINING:
    case DT_NOVA_CRITICAL_ARROW_DARK:
    case DT_NOVA_CRITICAL_BOLT_DARK:
    case DT_NOVA_SPECIAL1_ARROW_DARK:
    case DT_NOVA_CRITICAL_ARROW_LOVE:
    case DT_NOVA_CRITICAL_BOLT_LOVE:
    case DT_NOVA_SPECIAL1_ARROW_LOVE:

    case DT_NOVA_BOLT_DEATH:
    case DT_NOVA_FREEWAY_BOLT_DEATH:
    case DT_NOVA_FREEWAY_BOLT_DEATH_DUN:
    case DT_NOVA_ARROW_DEATH:
    case DT_NOVA_DOWN_ARROW_DEATH:
    case DT_NOVA_CRITICAL_BOLT_DEATH:
    case DT_NOVA_CRITICAL_ARROW_DEATH:
    case DT_NOVA_SPECIAL1_BOLT_DEATH:
    case DT_NOVA_SPECIAL1_ARROW_DEATH:

    case DT_ARROW_TIME:
    case DT_CRITICAL_ARROW_TIME:
    case DT_FREEWAY_ARROW_TIME:
    case DT_FREEWAY_ARROW_TIME_DUN:
    case DT_UP_ARROW_TIME:
    case DT_DOWN_ARROW_TIME:
    case DT_DOWN_ARROW_TIME_DUN:
    case DT_CRITICAL_BOLT_TIME:
    case DT_BOLT_TIME:
    case DT_HUGE_ARROW_TIME:
    case DT_HUGE_CRITICAL_ARROW_TIME:
    case DT_HUGE_RUNATK_ARROW_TIME:
    case DT_HUGE_COMBO3_ARROW_TIME:
    case DT_HUGE_JUMPDOWN_ARROW_TIME:
    case DT_NOVA_BOLT_TIME:
    case DT_NOVA_ARROW_TIME:
    case DT_NOVA_CRITICAL_ARROW_TIME:
    case DT_NOVA_FREEWAY_BOLT_TIME:
    case DT_NOVA_FREEWAY_BOLT_TIME_DUN:
    case DT_NOVA_CRITICAL_BOLT_TIME:
    case DT_NOVA_SPECIAL1_BOLT_TIME:
    case DT_NOVA_SPECIAL1_ARROW_TIME:

    case DT_ARROW_LIFE:
    case DT_CRITICAL_ARROW_LIFE:
    case DT_FREEWAY_ARROW_LIFE:
    case DT_FREEWAY_ARROW_LIFE_DUN:
    case DT_UP_ARROW_LIFE:
    case DT_DOWN_ARROW_LIFE:
    case DT_DOWN_ARROW_LIFE_DUN:
    case DT_CRITICAL_BOLT_LIFE:
    case DT_BOLT_LIFE:
    case DT_HUGE_ARROW_LIFE:
    case DT_HUGE_CRITICAL_ARROW_LIFE:
    case DT_HUGE_RUNATK_ARROW_LIFE:
    case DT_HUGE_COMBO3_ARROW_LIFE:
    case DT_HUGE_JUMPDOWN_ARROW_LIFE:
    case DT_NOVA_BOLT_LIFE:
    case DT_NOVA_ARROW_LIFE:
    case DT_NOVA_CRITICAL_ARROW_LIFE:
    case DT_NOVA_FREEWAY_BOLT_LIFE:
    case DT_NOVA_FREEWAY_BOLT_LIFE_DUN:
    case DT_NOVA_CRITICAL_BOLT_LIFE:
    case DT_NOVA_SPECIAL1_BOLT_LIFE:
    case DT_NOVA_SPECIAL1_ARROW_LIFE:

    case DT_ARROW_DARK_DRAGON_RED:
    case DT_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_FREEWAY_ARROW_DARK_DRAGON_RED:
    case DT_FREEWAY_ARROW_DARK_DRAGON_RED_DUN:
    case DT_UP_ARROW_DARK_DRAGON_RED:
    case DT_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_DOWN_ARROW_DARK_DRAGON_RED_DUN:
    case DT_CRITICAL_BOLT_DARK_DRAGON_RED:
    case DT_BOLT_DARK_DRAGON_RED:
    case DT_HUGE_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_COMBO3_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED_DUN:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_RED:

    case DT_ARROW_DARK_DRAGON_BLUE:
    case DT_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_FREEWAY_ARROW_DARK_DRAGON_BLUE:
    case DT_FREEWAY_ARROW_DARK_DRAGON_BLUE_DUN:
    case DT_UP_ARROW_DARK_DRAGON_BLUE:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE_DUN:
    case DT_CRITICAL_BOLT_DARK_DRAGON_BLUE:
    case DT_BOLT_DARK_DRAGON_BLUE:
    case DT_HUGE_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_COMBO3_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE_DUN:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_BLUE:

    case DT_ARROW_HALLOWEEN:
    case DT_CRITICAL_ARROW_HALLOWEEN:
    case DT_FREEWAY_ARROW_HALLOWEEN:
    case DT_FREEWAY_ARROW_HALLOWEEN_DUN:
    case DT_UP_ARROW_HALLOWEEN:
    case DT_DOWN_ARROW_HALLOWEEN:
    case DT_DOWN_ARROW_HALLOWEEN_DUN:
    case DT_CRITICAL_BOLT_HALLOWEEN:
    case DT_BOLT_HALLOWEEN:
    case DT_HUGE_ARROW_HALLOWEEN:
    case DT_HUGE_CRITICAL_ARROW_HALLOWEEN:
    case DT_HUGE_RUNATK_ARROW_HALLOWEEN:
    case DT_HUGE_COMBO3_ARROW_HALLOWEEN:
    case DT_HUGE_JUMPDOWN_ARROW_HALLOWEEN:
    case DT_NOVA_BOLT_HALLOWEEN:
    case DT_NOVA_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_ARROW_HALLOWEEN:
    case DT_NOVA_FREEWAY_BOLT_HALLOWEEN:
    case DT_NOVA_FREEWAY_BOLT_HALLOWEEN_DUN:
    case DT_NOVA_CRITICAL_BOLT_HALLOWEEN:
    case DT_NOVA_SPECIAL1_BOLT_HALLOWEEN:
    case DT_NOVA_SPECIAL1_ARROW_HALLOWEEN:

    case DT_ARROW_ELIA:
    case DT_CRITICAL_ARROW_ELIA:
    case DT_FREEWAY_ARROW_ELIA:
    case DT_FREEWAY_ARROW_ELIA_DUN:
    case DT_UP_ARROW_ELIA:
    case DT_DOWN_ARROW_ELIA:
    case DT_DOWN_ARROW_ELIA_DUN:
    case DT_CRITICAL_BOLT_ELIA:
    case DT_BOLT_ELIA:
    case DT_HUGE_ARROW_ELIA:
    case DT_HUGE_CRITICAL_ARROW_ELIA:
    case DT_HUGE_RUNATK_ARROW_ELIA:
    case DT_HUGE_COMBO3_ARROW_ELIA:
    case DT_HUGE_JUMPDOWN_ARROW_ELIA:
    case DT_NOVA_BOLT_ELIA:
    case DT_NOVA_ARROW_ELIA:
    case DT_NOVA_CRITICAL_ARROW_ELIA:
    case DT_NOVA_FREEWAY_BOLT_ELIA:
    case DT_NOVA_FREEWAY_BOLT_ELIA_DUN:
    case DT_NOVA_CRITICAL_BOLT_ELIA:
    case DT_NOVA_SPECIAL1_BOLT_ELIA:
    case DT_NOVA_SPECIAL1_ARROW_ELIA:

    case DT_ARROW_PEGASUS:
    case DT_CRITICAL_ARROW_PEGASUS:
    case DT_FREEWAY_ARROW_PEGASUS:
    case DT_FREEWAY_ARROW_PEGASUS_DUN:
    case DT_UP_ARROW_PEGASUS:
    case DT_DOWN_ARROW_PEGASUS:
    case DT_DOWN_ARROW_PEGASUS_DUN:
    case DT_CRITICAL_BOLT_PEGASUS:
    case DT_BOLT_PEGASUS:
    case DT_HUGE_ARROW_PEGASUS:
    case DT_HUGE_CRITICAL_ARROW_PEGASUS:
    case DT_HUGE_RUNATK_ARROW_PEGASUS:
    case DT_HUGE_COMBO3_ARROW_PEGASUS:
    case DT_HUGE_JUMPDOWN_ARROW_PEGASUS:
    case DT_NOVA_BOLT_PEGASUS:
    case DT_NOVA_ARROW_PEGASUS:
    case DT_NOVA_CRITICAL_ARROW_PEGASUS:
    case DT_NOVA_FREEWAY_BOLT_PEGASUS:
    case DT_NOVA_FREEWAY_BOLT_PEGASUS_DUN:
    case DT_NOVA_CRITICAL_BOLT_PEGASUS:
    case DT_NOVA_SPECIAL1_BOLT_PEGASUS:
    case DT_NOVA_SPECIAL1_ARROW_PEGASUS:

    case DT_ARROW_COLISEU:
    case DT_CRITICAL_ARROW_COLISEU:
    case DT_FREEWAY_ARROW_COLISEU:
    case DT_FREEWAY_ARROW_COLISEU_DUN:
    case DT_UP_ARROW_COLISEU:
    case DT_DOWN_ARROW_COLISEU:
    case DT_DOWN_ARROW_COLISEU_DUN:
    case DT_CRITICAL_BOLT_COLISEU:
    case DT_BOLT_COLISEU:
    case DT_HUGE_ARROW_COLISEU:
    case DT_HUGE_CRITICAL_ARROW_COLISEU:
    case DT_HUGE_RUNATK_ARROW_COLISEU:
    case DT_HUGE_COMBO3_ARROW_COLISEU:
    case DT_HUGE_JUMPDOWN_ARROW_COLISEU:
    case DT_NOVA_BOLT_COLISEU:
    case DT_NOVA_ARROW_COLISEU:
    case DT_NOVA_CRITICAL_ARROW_COLISEU:
    case DT_NOVA_FREEWAY_BOLT_COLISEU:
    case DT_NOVA_FREEWAY_BOLT_COLISEU_DUN:
    case DT_NOVA_CRITICAL_BOLT_COLISEU:
    case DT_NOVA_SPECIAL1_BOLT_COLISEU:
    case DT_NOVA_SPECIAL1_ARROW_COLISEU:

    case DT_ARROW_C_MASTER:
    case DT_CRITICAL_ARROW_C_MASTER:
    case DT_FREEWAY_ARROW_C_MASTER:
    case DT_FREEWAY_ARROW_C_MASTER_DUN:
    case DT_UP_ARROW_C_MASTER:
    case DT_DOWN_ARROW_C_MASTER:
    case DT_DOWN_ARROW_C_MASTER_DUN:
    case DT_CRITICAL_BOLT_C_MASTER:
    case DT_BOLT_C_MASTER:
    case DT_HUGE_ARROW_C_MASTER:
    case DT_HUGE_CRITICAL_ARROW_C_MASTER:
    case DT_HUGE_RUNATK_ARROW_C_MASTER:
    case DT_HUGE_COMBO3_ARROW_C_MASTER:
    case DT_HUGE_JUMPDOWN_ARROW_C_MASTER:
    case DT_NOVA_BOLT_C_MASTER:
    case DT_NOVA_ARROW_C_MASTER:
    case DT_NOVA_CRITICAL_ARROW_C_MASTER:
    case DT_NOVA_FREEWAY_BOLT_C_MASTER:
    case DT_NOVA_FREEWAY_BOLT_C_MASTER_DUN:
    case DT_NOVA_CRITICAL_BOLT_C_MASTER:
    case DT_NOVA_SPECIAL1_BOLT_C_MASTER:
    case DT_NOVA_SPECIAL1_ARROW_C_MASTER:

    case DT_LIRE1_SPECIAL4_DOWN_ARROW:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIGHTNING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_SHINING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LOVE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_TIME:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIFE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_HALLOWEEN:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_ELIA:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_PEGASUS:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_C_MASTER:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_COLISEU:
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[pShieldInstance->m_Who];
        if (false == bSameTeam && false == pDamageInstance->m_pOriginalDamage->m_bSpecialAttack
            && g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_LIRE1_HUNGRY_ARROW))
        {
            //====================================================================================
            // 2009.02.19 : Jemitgge
            // Comment : ����� ȭ���ϰ� ������;; ȭ���̳� �ĺ��̳� �������!!
            //			 ���� �������...
            if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE) || pDamageInstance->m_What == DT_FIREBALL || pDamageInstance->m_What == DT_ARME4_FIREBALL)
            {
                pDamageInstance->m_Life = 0;
                pShieldInstance->m_Life = 0;

                if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                {
                    pDamageInstance->CreateSequence("ELESIS1_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                    pDamageInstance->CreateSequence("ELESIS1_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                    pDamageInstance->CreateSequence("ELESIS1_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                }
            }
        }
    }
    break;
    case DT_SKILL_MAGIC_MISSILE_READY:
    {
        if (pDamageInstance->m_What == DT_SKILL_MAGIC_MISSILE && pDamageInstance->m_Who == pShieldInstance->m_Who)
        {
            AddWithLocate(DT_SKILL_MAGIC_MISSILE_BOMB, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            pShieldInstance->m_Life = 1;
            pDamageInstance->m_Life = 1;
        }
    }
    break;
    case DT_SKILL_LAS_BOMBDART_DART:
    case DT_SKILL_LAS_BOMBDART_SCROLL_TIMER:
    {
        if ((pDamageInstance->m_What == DT_SKILL_LAS_BOMBDART_SCROLL_FIXED
            || pDamageInstance->m_What == DT_SKILL_LAS_BOMBDART_SCROLL_FIXED_L
            || pDamageInstance->m_What == DT_SKILL_LAS_BOMBDART_SCROLL
            || pDamageInstance->m_What == DT_SKILL_LAS_BOMBDART_SCROLL_L)
            && pDamageInstance->m_Who == pShieldInstance->m_Who)
        {
            if (pShieldInstance->m_What == DT_SKILL_LAS_BOMBDART_SCROLL_TIMER)
            {
                AddWithLocate(DT_SKILL_LAS_BOMBDART_BOMB, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
                g_KDSound.Play("1452");
                pDamageInstance->m_Life = 1;
                pShieldInstance->m_Life = 1;
            }

            if (pShieldInstance->m_What == DT_SKILL_LAS_BOMBDART_DART)
            {
                if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                {
                    pDamageInstance->CreateSequence("Las01_Brind_Spark_01", pShieldInstance->m_X - 1.0f, pShieldInstance->m_Y - 0.45f, 0.5f);
                    pDamageInstance->CreateSequence("Las01_Brind_Spark_02", pShieldInstance->m_X - 1.0f, pShieldInstance->m_Y - 0.45f, 0.5f);
                }
                pShieldInstance->m_Life = 1;
            }
        }
    }
    break;

    //	case DT_CRYSTAL_BLIZZARD_GLASS_ATK3:
    //	case DT_CRYSTAL_BLIZZARD_GLASS_ATK3_R:
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[pShieldInstance->m_Who];
        pPlayer->Direct_Motion_Input(MID_COMMON_FREEZED_BY_PIXIE);

#ifdef LEVEL_DESIGN_STAT_CALC
        pPlayer->m_fFreezeDamage = -1.f;
#else
        pPlayer->m_fFreezeDamage = -0.01f;
#endif

        pPlayer->m_iFreezeCount = 20;

        pPlayer->m_fFreezeDamage -= pPlayer->m_fFreezeDamage * pPlayer->m_fAbility[ABILITY_ABNORMAL_CONDITION_RESISTANCE];
    }
    break;

    default:
        break;
    }
}

void CDamageManager::CallEffAtEffectCrashForce(CDamageInstance* pDamageInstance, CDamageInstance* pTargetInstance)
{
    bool bSameTeam = g_MyD3D->IsSameTeam(pDamageInstance->m_Who, pTargetInstance->m_Who);

    //���͵� ���� ���� �ִ�!!!
    if (pDamageInstance->m_IsMonsterDamage)
    {
        bSameTeam = g_MyD3D->IsSameTeam_PlayerAndMonster(pTargetInstance->m_Who, pDamageInstance->m_ExtraMonsterNum);
    }

    switch (pTargetInstance->m_What)
    {
    case DT_VANESSA_SHIELD:
        // �ƽ�Ÿ���� ���̾�θ� �ݻ�
        if (pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK ||
            pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK5)
        {
            pDamageInstance->m_Life = 1;
        }
        else if (pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK2 ||
            pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK3 ||
            pDamageInstance->m_What == DT_ARME4_SPECIAL3_BLACK4)
        {
            pDamageInstance->m_Life = 1;
        }
        break;
    case DT_SKILL_RYAN_HP_TOTEM:
    case DT_SKILL_RYAN_MP_TOTEM:
    case DT_SKILL_RYAN_DP_TOTEM:
    {
        if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS || pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE ||
            g_MyD3D->IsSameTeam(pDamageInstance->m_Who, pTargetInstance->m_Who))
        {
            pTargetInstance->m_pOriginalDamage->m_iDamageHP++;
            break;
        }
    }
    break;
    case DT_SKILL_RYAN_STONE_WALL:
    {
        if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS || pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE ||
            g_MyD3D->IsSameTeam(pDamageInstance->m_Who, pTargetInstance->m_Who))
        {
            pTargetInstance->m_pOriginalDamage->m_iDamageHP++;
            break;
        }

        // ���� �Ǵ� 4! ������ ���������� ������ 2�� ������ ������
        if (bSameTeam == false && false == (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE))
        {
            pTargetInstance->m_pOriginalDamage->m_iDamageHP--;

            if (pTargetInstance->m_pOriginalDamage->m_iDamageHP <= 0)
                pTargetInstance->m_Life = 1;

            pTargetInstance->m_pOriginalDamage->DamageHpEffect(pTargetInstance);
        }
    }
    break;
    case DT_SENTRYKEEPER2_ATK1_DESTINATION:
    {
        if (pDamageInstance->m_What == DT_SENTRYKEEPER2_ATK1)
        {
            // ���缭 15������ �ִٰ� ������.
            pDamageInstance->m_Life = 15;
            pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.001f;
            pTargetInstance->m_Life = 1;
        }
    }
    break;
    case DT_SENTRYKEEPER1_ATK2_DESTINATION:
    {
        if (pDamageInstance->m_What == DT_SENTRYKEEPER1_ATK2)
        {
            // ���缭 15������ �ִٰ� ������.
            pDamageInstance->m_Life = 1;
            pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.001f;
            pTargetInstance->m_Life = 1;
        }
    }
    break;
    default:
        break;
    }
}

void CDamageManager::Load(int iLoadLevel/* = -1*/)
{
    if (iLoadLevel == 0 || iLoadLevel == -1)
    {
        UnLoad();
        LoadFireInfo();
        m_apTexture = new GCDeviceTexture * [m_i_MAX_DAMAGE_TEX_NUM];
        memset(m_apTexture, 0, sizeof(GCDeviceTexture*) * m_i_MAX_DAMAGE_TEX_NUM);
    }

    if (iLoadLevel == -1)
    {
        // Full
        char TextureFileName[30];
        for (int i = 0; i < m_i_MAX_DAMAGE_TEX_NUM; i++)
        {
            sprintf(TextureFileName, "Special%02d.dds", i);
            m_apTexture[i] = g_pGCDeviceManager2->CreateTexture(TextureFileName);
        }
    }
    else
    {
        static int s_iThisLoad = 0;

        char TextureFileName[30];
        for (int i = 0; i < m_i_MAX_DAMAGE_TEX_NUM / 3; ++i)
        {
            sprintf(TextureFileName, "Special%02d.dds", s_iThisLoad);
            m_apTexture[s_iThisLoad] = g_pGCDeviceManager2->CreateTexture(TextureFileName);

            if (++s_iThisLoad == m_i_MAX_DAMAGE_TEX_NUM)
            {
                s_iThisLoad = 0;
                break;
            }
        }
    }

    m_mapEraseMonDamageList.clear();
}

void CDamageManager::LoadFireInfo()
{
    KLuaManager& luaMgr = *m_pLuaMgr;

    if (GCFUNC::LoadLuaScript(luaMgr, "Enum.stg") == false)
        goto load_err;

    if (GCFUNC::LoadLuaScript(luaMgr, "MotionEnum.stg") == false)
        goto load_err;

    if (GCFUNC::LoadLuaScript(luaMgr, "DamageEnum.stg") == false)
        goto load_err;

    if (GCFUNC::LoadLuaScript(luaMgr, "BuffEnum.stg") == false)
        goto load_err;

    if (GCFUNC::LoadLuaScript(luaMgr, "FireInfo.stg") == false)
        goto load_err;

    SiKGCFileMoniter()->RegisterFile(g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath("FireInfo.stg"), InitDamageFileMoniter);



    luaMgr.GetValue("MAX_DAMAGE_TEX_NUM", m_i_MAX_DAMAGE_TEX_NUM);
    luaMgr.EndTable();


    if (LoadFireInfoTable(luaMgr, m_vecInfo) == false)
    {
        goto load_err;
    }

    return;

load_err:
    MessageBoxA(NULL, "FireInfo Load Failed!", "", MB_OK);
    //exit(0);
    return;
}

void CDamageManager::MakeSoundLoadList()
{
    UnLoad();
    LoadFireInfo();

    std::vector<int> vecLoadList;
    for (int i = 0; i < static_cast<int>(m_vecInfo.size()); ++i)
    {
        CDamage* pDamage = m_vecInfo[i];
        vecLoadList.push_back(pDamage->m_iCrashSound);
    }

    std::for_each(vecLoadList.begin(), vecLoadList.end(),
        boost::bind(&KDSound::PreparetoLoad, boost::ref(g_KDSound), _1));
}

void CDamageManager::MakeSoundLoadList(std::vector<CDamage*>& vecInfo)
{
    std::vector<int> vecLoadList;
    for (int i = 0; i < static_cast<int>(m_vecInfo.size()); ++i)
    {
        CDamage* pDamage = vecInfo[i];
        vecLoadList.push_back(pDamage->m_iCrashSound);
    }

    std::for_each(vecLoadList.begin(), vecLoadList.end(),
        boost::bind(&KDSound::PreparetoLoad, boost::ref(g_KDSound), _1));
}

void CDamageManager::ReloadSpecial75(bool bIsBlackTex)
{
    m_bIsSP75BTexture = bIsBlackTex;
    KLuaManager& luaMgr = *m_pLuaMgr;

    if (GCFUNC::LoadLuaScript(luaMgr, "DamageEnum.stg") == false)
        return;

    int iNewmaxDamageTexNum;
    luaMgr.GetValue("MAX_DAMAGE_TEX_NUM", iNewmaxDamageTexNum);
    luaMgr.EndTable();

    if (m_apTexture)
    {
        for (int i = 0; i < m_i_MAX_DAMAGE_TEX_NUM; i++)
        {
            SAFE_RELEASE(m_apTexture[i]);
        }
        SAFE_DELETE_ARRAY(m_apTexture);

        // �ؽ��Ļ��� �ε�
        char TextureFileName[30];

        m_apTexture = new GCDeviceTexture * [iNewmaxDamageTexNum];

        for (int i = 0; i < iNewmaxDamageTexNum; i++)
        {
            if (i == 75)
            {
                if (m_bIsSP75BTexture)
                    sprintf(TextureFileName, "Special75_B.dds");
                else
                    sprintf(TextureFileName, "Special75.dds");
            }
            else
            {
                sprintf(TextureFileName, "Special%02d.dds", i);
            }

            m_apTexture[i] = g_pGCDeviceManager2->CreateTexture(TextureFileName);
        }
    }
}

void CDamageManager::ReloadFireInfo()
{
    delete m_pLuaMgr;
    m_pLuaMgr = new KLuaManager();
    KLuaManager& luaMgr = *m_pLuaMgr;

    if (GCFUNC::LoadLuaScript(luaMgr, "Enum.stg") == false)
        return;

    if (GCFUNC::LoadLuaScript(luaMgr, "MotionEnum.stg") == false)
        return;

    if (GCFUNC::LoadLuaScript(luaMgr, "DamageEnum.stg") == false)
        return;

    if (GCFUNC::LoadLuaScript(luaMgr, "BuffEnum.stg") == false)
        return;

    if (GCFUNC::LoadLuaScript(luaMgr, "FireInfo.stg") == false)
        return;


    int iNewmaxDamageTexNum;

    luaMgr.GetValue("MAX_DAMAGE_TEX_NUM", iNewmaxDamageTexNum);


    std::vector<CDamage*> newInfo;
    if (LoadFireInfoTable(luaMgr, newInfo) == false)
        return;

    MakeSoundLoadList(newInfo);
    g_KDSound.FullLoadfromLoadList();

    std::vector<CDamage*> vecOldpInfo;
    vecOldpInfo.reserve(m_vecInfo.size());

    int* oldpiRenderSize = NULL;

    // ������ ����
    vecOldpInfo = m_vecInfo;
    // ���� ����
    m_vecInfo = newInfo;

    // ������ �����
    std::vector<CDamage*>::iterator vit;
    for (vit = vecOldpInfo.begin(); vit != vecOldpInfo.end(); ++vit)
    {
        SAFE_DELETE((*vit));
    }
    vecOldpInfo.clear();

    if (m_apTexture)
    {
        for (int i = 0; i < m_i_MAX_DAMAGE_TEX_NUM; i++)
        {
            SAFE_RELEASE(m_apTexture[i]);
        }
        SAFE_DELETE_ARRAY(m_apTexture);

        // �ؽ��Ļ��� �ε�
        char TextureFileName[30];

        m_apTexture = new GCDeviceTexture * [iNewmaxDamageTexNum];
        memset(m_apTexture, 0, sizeof(GCDeviceTexture*) * iNewmaxDamageTexNum);

        for (int i = 0; i < iNewmaxDamageTexNum; i++)
        {
            if (m_bIsSP75BTexture && i == 75)
                sprintf(TextureFileName, "Special75_B.dds");
            else
                sprintf(TextureFileName, "Special%02d.dds", i);

            m_apTexture[i] = g_pGCDeviceManager2->CreateTexture(TextureFileName);
        }
    }

    g_pkChatManager->AddSystemMsg(0, L"������ ���� ���ε� �Ϸ�");
}

bool CDamageManager::LoadFireInfoTable(KLuaManager& luaMgr, std::vector<CDamage*>& Info)
{
    if (SUCCEEDED(luaMgr.BeginTable("DamageFile", false)))
    {
        std::string strFileName;
        for (int i = 1; SUCCEEDED(luaMgr.GetValue(i, strFileName)); i++)
        {
            SiKGCFileMoniter()->RegisterFile(g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath(strFileName), InitDamageFileMoniter);

            if (GCFUNC::LoadLuaScript(luaMgr, strFileName) == false)
                return false;

        }
        luaMgr.EndTable();
    }
    int iMaxDamageNum;
    luaMgr.GetValue("MAX_DAMAGE_NUM", iMaxDamageNum);

    if (FAILED(luaMgr.BeginTable("Damage", false)))
        return false;

    for (int i = 0; iMaxDamageNum > i; ++i)
    {
        CDamage* pDamage = new CDamage();
        Info.push_back(pDamage);
        if (FAILED(luaMgr.BeginTable(i, false)))
        {
            continue;
        }
        else
        {
            LoadFireInfoSingleObject(luaMgr, pDamage);
        }

        luaMgr.EndTable();
    }
    luaMgr.EndTable();

    return true;
}

bool CDamageManager::LoadFireInfoSingleObject(KLuaManager& luaMgr, CDamage* pDamage)
{
    luaMgr.GetValue("PARENT", pDamage->m_iParent);

    int iParent = pDamage->m_iParent;

    if (pDamage->m_iParent != -1)   // �θ� �ִٸ�
    {
        if (pDamage->m_iParent >= (int)m_vecInfo.size())
        {
            MessageBoxA(NULL, "Parent Fail", "", MB_OK);
        }

        *pDamage = *(m_vecInfo[pDamage->m_iParent]);
        pDamage->m_iReserveParent = iParent;
    }
    luaMgr.GetValue("LIFE_CHANGE", pDamage->m_Life_Change);
    luaMgr.GetValue("REVERSE_LIFE", pDamage->m_Reverse_Life);
    luaMgr.GetValue("START_LIFE", pDamage->m_Start_Life);
    pDamage->m_iCrashStart = pDamage->m_Start_Life;
    luaMgr.GetValue("ENABLE_DAMAGE", pDamage->m_IsEnableDamage);
    luaMgr.GetValue("IS_STATICDAMAGE_CLEANER", pDamage->m_IsStaticDamageCleaner);
    luaMgr.GetValue("IS_STATICDAMAGE", pDamage->m_IsStaticDamage);
    luaMgr.GetValue("START_SCALE_X", pDamage->m_Start_Scale_x);
    luaMgr.GetValue("SCALE_Y_PER_X", pDamage->m_Y_per_x);
    luaMgr.GetValue("START_LOCATE_X", pDamage->m_Start_Locate_x);
    luaMgr.GetValue("START_LOCATE_Y", pDamage->m_Start_Locate_y);

    LUA_BEGIN_TABLE_RAII("START_LOCATE_RANDOM_X")
    {
        luaMgr.GetValue(1, pDamage->m_Start_Locate_Random_x[0]);
        luaMgr.GetValue(2, pDamage->m_Start_Locate_Random_x[1]);
    }
    LUA_BEGIN_TABLE_RAII("START_LOCATE_RANDOM_Y")
    {
        luaMgr.GetValue(1, pDamage->m_Start_Locate_Random_y[0]);
        luaMgr.GetValue(2, pDamage->m_Start_Locate_Random_y[1]);
    }

    LUA_BEGIN_TABLE_RAII("START_LOCATE_FRAME_X")
    {
        luaMgr.GetValue(1, pDamage->m_Start_Locate_Frame_x[0]);
        luaMgr.GetValue(2, pDamage->m_Start_Locate_Frame_x[1]);
    }
    LUA_BEGIN_TABLE_RAII("START_LOCATE_FRAME_Y")
    {
        luaMgr.GetValue(1, pDamage->m_Start_Locate_Frame_y[0]);
        luaMgr.GetValue(2, pDamage->m_Start_Locate_Frame_y[1]);
    }

    luaMgr.GetValue("SPEED_X", pDamage->m_x_Speed);
    luaMgr.GetValue("SPEED_Y", pDamage->m_y_Speed);
    luaMgr.GetValue("CRASH_SPEED_X", pDamage->m_x_CrashSpeed);
    luaMgr.GetValue("CRASH_SPEED_Y", pDamage->m_y_CrashSpeed);
    luaMgr.GetValue("DECREASE_SPEED_X", pDamage->m_x_DecSpeed);
    luaMgr.GetValue("DECREASE_SPEED_Y", pDamage->m_y_DecSpeed);

    luaMgr.GetValue("SPEED_SCALE", pDamage->m_Scale_Speed);
    luaMgr.GetValue("ISANGLE_MOVE", pDamage->m_IsAngleMove);
    luaMgr.GetValue("IS_REALTIME_ANGLE_MOVE", pDamage->m_IsRealTimeAngleMove);
    luaMgr.GetValue("IS_ANGLE_MOVE_WITH_OFFSET", pDamage->m_IsAngleMoveWithOffset);
    luaMgr.GetValue("IS_FIXED_ANGLE_MOVE", pDamage->m_IsFixedAngleMove);
    luaMgr.GetValue("START_ANGLE", pDamage->m_Start_Angle);
    luaMgr.GetValue("SPEED_ANGLE", pDamage->m_Angle_Speed);
    luaMgr.GetValue("LIMITE_ANGLE", pDamage->m_fLimite_Angle);
    luaMgr.GetValue("TEXTURE", pDamage->m_Texture);
    luaMgr.GetValue("TEXTURE_NO_RENDER_CNT", pDamage->m_TextureNoRenderCnt);
#if defined( LEVEL_DESIGN_STAT_CALC )
    luaMgr.GetValue("DAMAGETO_PLAYER", pDamage->m_fDamageToPlayer);
    luaMgr.GetValue("DAMAGETO_MONSTER", pDamage->m_fDamageToMonster);
#else
    luaMgr.GetValue( "ISDAMAGECORRUPTED", pDamage->m_bIsCorruptedDamage );
    if (SUCCEEDED(luaMgr.GetValue("DAMAGETO_PLAYER", pDamage->m_fDamageToPlayer)))
    {
        pDamage->m_fDamageToPlayer /= 100.0f;
    }
    if (SUCCEEDED(luaMgr.GetValue("DAMAGETO_MONSTER", pDamage->m_fDamageToMonster)))
    {
        pDamage->m_fDamageToMonster /= 1000.0f;
    }

#endif
    luaMgr.GetValue("IGNORE_DEF_DAMAGE", pDamage->m_bIgnoreDefDamage);
    luaMgr.GetValue("IGNORE_SUPER_DAMAGE", pDamage->m_bIgnoreSuperDamage);
    luaMgr.GetValue("MP_DAMAGE", pDamage->m_fMpChange);
    luaMgr.GetValue("TRACE_MONSTER_BOUND_BOX", pDamage->m_bTraceMonsterBoundBox);
    luaMgr.GetValue("DELETE_SYNC", pDamage->m_bDeleteSync);
    luaMgr.GetValue("NO_CRASH_FATAL", pDamage->m_bNoCrashFatal);
    luaMgr.GetValue("IGNORE_GROUND_CHECK_HITTED_OBJECT", pDamage->m_iIgnoreGroundCheckHittedObject);
    luaMgr.GetValue("IGNORE_CHAMPION_PROPERTY", pDamage->m_bIgnoreChampionProperty);
    luaMgr.GetValue("NOT_COMBO_BREAK", pDamage->m_bNotComboBreak);

    // Particle
    luaMgr.GetValue("PARTICLE_SEQNAME", pDamage->m_particleSeqName);
    luaMgr.GetValue("EMITMIN", pDamage->m_EmitMin);
    luaMgr.GetValue("EMITMAX", pDamage->m_EmitMax);

    LUA_BEGIN_TABLE("CHILD_PARTICLE", goto NoChildParticle)
    {
        pDamage->m_vecChildParticle.clear();

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                CHILD_PARTICLE stParticle;
                stParticle.m_iStartLife = pDamage->m_Start_Life;
                LUA_GET_VALUE_NOASSERT(1, stParticle.m_particleSeqName, break);
                //printf("m_particleSeqName --> %s\n", stParticle.m_particleSeqName.data());
                luaMgr.GetValue(2, stParticle.m_EmitMin);
                luaMgr.GetValue(3, stParticle.m_EmitMax);
                luaMgr.GetValue(4, stParticle.m_bDieTogether);
                luaMgr.GetValue(5, stParticle.m_fXOffset);
                luaMgr.GetValue(6, stParticle.m_fYOffset);
                luaMgr.GetValue(7, stParticle.m_iStartLife);
                luaMgr.GetValue(8, stParticle.m_fParticleTraceTime);
                luaMgr.GetValue(9, stParticle.m_bWithAngle);
                luaMgr.GetValue(10, stParticle.m_bDotParticle);
                luaMgr.GetValue(11, stParticle.m_bReverseTexture);
                luaMgr.GetValue(12, stParticle.m_bIsLocal);
                luaMgr.GetValue(13, stParticle.m_bSyncCreatorDir);
                pDamage->m_vecChildParticle.push_back(stParticle);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoChildParticle);

NoChildParticle:

    LUA_BEGIN_TABLE("DAMAGE_HP_PARTICLE", goto NoDamageHpParticle)
    {
        pDamage->m_vecDamageHpParticle.clear();

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                DAMAGE_PARTICLE stParticle;
                LUA_GET_VALUE_DEF(1, stParticle.m_particleSeqName, "");
                LUA_GET_VALUE_DEF(2, stParticle.m_fXOffset, 0.0f);
                LUA_GET_VALUE_DEF(3, stParticle.m_fYOffset, 0.0f);
                LUA_GET_VALUE_DEF(4, stParticle.m_fTraceTime, 0.5f);
                pDamage->m_vecDamageHpParticle.push_back(stParticle);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoDamageHpParticle);

NoDamageHpParticle:

    LUA_BEGIN_TABLE("DAMAGE_IN_TOGGLE_MESH_PARTICLE", goto NoDamageInToggleParticle)
    {
        pDamage->m_vecInToggleMeshParticle.clear();

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                DAMAGE_PARTICLE stParticle;
                LUA_GET_VALUE_DEF(1, stParticle.m_particleSeqName, "");
                LUA_GET_VALUE_DEF(2, stParticle.m_fXOffset, 0.0f);
                LUA_GET_VALUE_DEF(3, stParticle.m_fYOffset, 0.0f);
                LUA_GET_VALUE_DEF(4, stParticle.m_fTraceTime, 0.5f);
                pDamage->m_vecInToggleMeshParticle.push_back(stParticle);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoDamageInToggleParticle);

NoDamageInToggleParticle:

    LUA_BEGIN_TABLE("DAMAGE_OUT_TOGGLE_MESH_PARTICLE", goto NoDamageOutToggleParticle)
    {
        pDamage->m_vecOutToggleMeshParticle.clear();

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                DAMAGE_PARTICLE stParticle;
                LUA_GET_VALUE_DEF(1, stParticle.m_particleSeqName, "");
                LUA_GET_VALUE_DEF(2, stParticle.m_fXOffset, 0.0f);
                LUA_GET_VALUE_DEF(3, stParticle.m_fYOffset, 0.0f);
                LUA_GET_VALUE_DEF(4, stParticle.m_fTraceTime, 0.5f);
                pDamage->m_vecOutToggleMeshParticle.push_back(stParticle);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoDamageOutToggleParticle);

NoDamageOutToggleParticle:

    LUA_BEGIN_TABLE("DAMAGE_IN_TOGGLE_SUMMONMONSTER_MESH_PARTICLE", goto NoDamageInToggleSummonMonsterParticle)
    {
        pDamage->m_vecInToggleSummonMonsterMeshParticle.clear();

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                DAMAGE_PARTICLE stParticle;
                LUA_GET_VALUE_DEF(1, stParticle.m_particleSeqName, "");
                LUA_GET_VALUE_DEF(2, stParticle.m_fXOffset, 0.0f);
                LUA_GET_VALUE_DEF(3, stParticle.m_fYOffset, 0.0f);
                LUA_GET_VALUE_DEF(4, stParticle.m_fTraceTime, 0.5f);
                pDamage->m_vecInToggleSummonMonsterMeshParticle.push_back(stParticle);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoDamageInToggleSummonMonsterParticle);

NoDamageInToggleSummonMonsterParticle:

    LUA_BEGIN_TABLE("DAMAGE_OUT_TOGGLE_SUMMONMONSTER_MESH_PARTICLE", goto NoDamageOutToggleSummonMonsterParticle)
    {
        pDamage->m_vecOutToggleSummonMonsterMeshParticle.clear();

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                DAMAGE_PARTICLE stParticle;
                LUA_GET_VALUE_DEF(1, stParticle.m_particleSeqName, "");
                LUA_GET_VALUE_DEF(2, stParticle.m_fXOffset, 0.0f);
                LUA_GET_VALUE_DEF(3, stParticle.m_fYOffset, 0.0f);
                LUA_GET_VALUE_DEF(4, stParticle.m_fTraceTime, 0.5f);
                pDamage->m_vecOutToggleSummonMonsterMeshParticle.push_back(stParticle);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoDamageOutToggleSummonMonsterParticle);

NoDamageOutToggleSummonMonsterParticle:

    LUA_BEGIN_TABLE("MPDAMAGE_PARTICLE", goto NoMPDamageParticle)
    {
        pDamage->m_vecMPDamageParticle.clear();

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                DAMAGE_PARTICLE stParticle;
                LUA_GET_VALUE_DEF(1, stParticle.m_particleSeqName, "");
                LUA_GET_VALUE_DEF(2, stParticle.m_fXOffset, 0.0f);
                LUA_GET_VALUE_DEF(3, stParticle.m_fYOffset, 0.0f);
                LUA_GET_VALUE_DEF(4, stParticle.m_fTraceTime, 0.5f);
                pDamage->m_vecMPDamageParticle.push_back(stParticle);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoMPDamageParticle);

NoMPDamageParticle:

    LoadMeshInfo(luaMgr, pDamage->m_MeshInfo);

    LUA_BEGIN_TABLE("SPARK", goto NoSpark)
    {
        int iTempSparkKind, iTempSparkID;
        for (int skarkNum = 1; ; ++skarkNum)
        {
            LUA_BEGIN_TABLE(skarkNum, break)
            {
                std::pair<int, int> TempSpark;
                LUA_GET_VALUE_NOASSERT(1, iTempSparkKind, break);
                LUA_GET_VALUE_NOASSERT(2, iTempSparkID, break);
                TempSpark.first = iTempSparkKind;
                TempSpark.second = iTempSparkID;
                pDamage->m_vecSpark.push_back(TempSpark);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoSpark);
NoSpark:

    LUA_BEGIN_TABLE("VALKYRIE", goto NoValkyrie)
    {
        pDamage->m_vecMileStone.clear();
        luaMgr.GetValue("ON_UI", pDamage->m_bOnUI);
        for (int i = 1; ; ++i)
        {
            D3DXVECTOR3 vecTemp;

            LUA_BEGIN_TABLE(i, break)
            {
                LUA_GET_VALUE_DEF(1, vecTemp.x, 0.0f);
                LUA_GET_VALUE_DEF(2, vecTemp.y, 0.0f);
                pDamage->SetMileStonePos(vecTemp.x, vecTemp.y);
                // ������ ���� ������� �� �������� �ϱ� ���ؼ���... �� �ΰ��� �� �߰��� ����Ѵ�...
                // ù���� ������ ó���� ���� �ϳ��� �� ����.
                if (pDamage->m_vecMileStone.size() == 1)
                {
                    pDamage->SetMileStonePos(vecTemp.x, vecTemp.y);
                }
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(goto NoValkyrie);
NoValkyrie:

    LUA_BEGIN_TABLE("RESTRICT_HIT_COUNT", goto NoRestrictHitCount)
    {
        pDamage->m_kRestrictHitCnt.Init();
        luaMgr.GetValue("HIT_COUNT", pDamage->m_kRestrictHitCnt.iHitCnt);
        luaMgr.GetValue("CHANGE_MOTION", pDamage->m_kRestrictHitCnt.iChangeMotion);
        luaMgr.GetValue("ONLY_PLAYER", pDamage->m_kRestrictHitCnt.bOnlyPlayer);
    }
    LUA_END_TABLE(goto NoRestrictHitCount);
NoRestrictHitCount:

    LUA_BEGIN_TABLE("VARIOUS_INCREASE_RATIO", goto NoVariousIncreaseRatio)
    {
        pDamage->m_kVariousIncreaseRatio.Init();
        luaMgr.GetValue("Y_PER_X_RATIO", pDamage->m_kVariousIncreaseRatio.fY_Per_X_Ratio);
        luaMgr.GetValue("SCALE_X_RATIO", pDamage->m_kVariousIncreaseRatio.fScale_X_Ratio);
        luaMgr.GetValue("NO_FRAMEMOVE", pDamage->m_kVariousIncreaseRatio.bNoFrameMove);
    }
    LUA_END_TABLE(goto NoVariousIncreaseRatio);
NoVariousIncreaseRatio:

    LUA_BEGIN_TABLE("DAMAGE_FLAG_BY_SET", goto NoDamageFlag)
    {
        int iFlag = 0;
        for (int i = 1; ; i++) {
            LUA_GET_VALUE_NOASSERT(i, iFlag, break);
            pDamage->m_setDamageFlag.insert(iFlag);
        }
    }
    LUA_END_TABLE(goto NoDamageFlag);
NoDamageFlag:

    luaMgr.GetValue("ADDNODIRECTION", pDamage->m_bAddNoDirection);
    luaMgr.GetValue("ANIM_SEQNAME", pDamage->m_AnimSeqName);
    luaMgr.GetValue("ANIM_STARTSCALEX", pDamage->m_fAnim_Start_ScaleX);
    luaMgr.GetValue("ANIM_STARTSCALEY", pDamage->m_fAnim_Start_ScaleY);
    luaMgr.GetValue("ANIM_LIMITSCALEX", pDamage->m_fAnim_Limit_ScaleX);
    luaMgr.GetValue("ANIM_LIMITSCALEY", pDamage->m_fAnim_Limit_ScaleY);
    luaMgr.GetValue("ANIM_SCALE_SPEEDX", pDamage->m_fAnimScaleX_Speed);
    luaMgr.GetValue("ANIM_SCALE_SPEEDY", pDamage->m_fAnimScaleY_Speed);
    luaMgr.GetValue("ISANIMLOOP", pDamage->m_bAnimLoop);
    luaMgr.GetValue("ANIM_ANGLE", pDamage->m_fAnimAngle);
    luaMgr.GetValue("ANIM_STARTPOSX", pDamage->m_fAnimStartPosX);
    luaMgr.GetValue("ANIM_STARTPOSY", pDamage->m_fAnimStartPosY);
    luaMgr.GetValue("DAMAGE_MOTION", pDamage->m_iDamageMotion);
    luaMgr.GetValue("DAMAGE_XSPEED", pDamage->m_fDMXSpeed);
    luaMgr.GetValue("DAMAGE_YSPEED", pDamage->m_fDMYSpeed);
    luaMgr.GetValue("DAMAGE_FLAG", pDamage->m_dwDMAttribute);
    luaMgr.GetValue("TRACE_POS", pDamage->m_iTracePosType);
    luaMgr.GetValue("NO_LOCAL_MOTION", pDamage->m_bNoLocalMotion);

    luaMgr.GetValue("DAMAGE_SOUND", pDamage->m_iCrashSound);
    luaMgr.GetValue("AP_MAGIC_EFF", pDamage->m_iAPMagicEffect);
    luaMgr.GetValue("MAGIC_EFF", pDamage->m_iMagicEffect);
    luaMgr.GetValue("MAGIC_TIME", pDamage->m_fMagicEffTime);
    luaMgr.GetValue("MAGIC_LEVEL", pDamage->m_iMagicLevel);
    luaMgr.GetValue("MAGIC_EFF_TO_MONSTER", pDamage->m_iMonsterMagicEff);
    luaMgr.GetValue("UNMAGIC_EFF", pDamage->m_iUnMagicEffect);
    luaMgr.GetValue("NO_CRASH_UNMAGIC_EFF", pDamage->m_iNoCrashUnMagicEffect);
    luaMgr.GetValue("UNIQUE_MAGIC_EFF", pDamage->m_bUniqueMgicEff);
    luaMgr.GetValue("FASTHOST_MAGIC_EFF", pDamage->m_bFastHostMagicEff);

    LUA_BEGIN_TABLE_RAII("RAND_MAGIC_EFF")
    {
        float fBuffTime;
        int iBuffEnum, iLoop = 1;
        while (1)
        {
            LUA_BEGIN_TABLE_RAII(iLoop++)
            {
                iBuffEnum = 0;
                fBuffTime = 0.0f;
                luaMgr.GetValue(1, iBuffEnum);
                luaMgr.GetValue(2, fBuffTime);
                pDamage->m_vecRandMagicEffect.push_back(std::make_pair(iBuffEnum, fBuffTime));
            }
    else
        break;
        }
    }

    luaMgr.GetValue("EARTH_QUAKE", pDamage->m_iEarthQuake);
    luaMgr.GetValue("EARTH_QUAKE_Y", pDamage->m_iEarthQuakeY);
    luaMgr.GetValue("EARTH_QUAKE_RUN_FRAME", pDamage->m_iEarthQuakeRunFrame);
    luaMgr.GetValue("CRASH_START", pDamage->m_iCrashStart);
    luaMgr.GetValue("CRASH_END", pDamage->m_iCrashEnd);
    luaMgr.GetValue("DIRECTIONALITY", pDamage->m_bDirection);
    luaMgr.GetValue("SRC_BLENDMODE", pDamage->m_SRCBlendMode);
    luaMgr.GetValue("DEST_BLENDMODE", pDamage->m_DESTBlendMode);
    luaMgr.GetValue("JUMP_POSSIBLE", pDamage->m_iJumpPossibleFrame);

    luaMgr.GetValue("IS_CONTINOUS_STONE_CURSE_DAMAGE", pDamage->m_bIsContinousDamage);
    luaMgr.GetValue("STONE_CURSE_DECREASE_PER_PUSH", pDamage->m_fDecreasePerPush);
#if defined( LEVEL_DESIGN_STAT_CALC )
    luaMgr.GetValue("STONE_CURSE_MAX_DAMAGE", pDamage->m_fStoneMaxDmg);
    luaMgr.GetValue("STONE_CURSE_MIN_DAMAGE", pDamage->m_fStoneMinDmg);
    luaMgr.GetValue("CONTINOUS_STONE_CURSE_DAMAGE", pDamage->m_fContinousDamage);
#else
    if (SUCCEEDED(luaMgr.GetValue("STONE_CURSE_MAX_DAMAGE", pDamage->m_fStoneMaxDmg)))
    {
        pDamage->m_fStoneMaxDmg /= 100.0f;
    }
    if (SUCCEEDED(luaMgr.GetValue("STONE_CURSE_MIN_DAMAGE", pDamage->m_fStoneMinDmg)))
    {
        pDamage->m_fStoneMinDmg /= 100.0f;
    }
    if (SUCCEEDED(luaMgr.GetValue("CONTINOUS_STONE_CURSE_DAMAGE", pDamage->m_fContinousDamage)))
    {
        pDamage->m_fContinousDamage /= 100.0f;
    }
#endif
    luaMgr.GetValue("PARTICLE_TRACE", pDamage->m_bParticleTrace);
    luaMgr.GetValue("PARTICLE_ANGLEMOVE", pDamage->m_bParticleAngleMove);
    luaMgr.GetValue("MOTION_TRACE", pDamage->m_bMotionTrace);
    luaMgr.GetValue("MOTION_X_TRACE", pDamage->m_bMotionXTrace);
    luaMgr.GetValue("MOTION_Y_TRACE", pDamage->m_bMotionYTrace);

    luaMgr.GetValue("STOP_MOTION_TRACE_LIFE", pDamage->m_iStopMotionTraceLife);

    luaMgr.GetValue("PLAYER_X_TRACE", pDamage->m_bPlayerXTrace);
    luaMgr.GetValue("PLAYER_Y_TRACE", pDamage->m_bPlayerYTrace);

    luaMgr.GetValue("PLUS_DELAY", pDamage->m_iPlusDelay);

    // ���� ��ȯ
    LUA_BEGIN_TABLE_RAII("SUMMON_MONSTER")
    {
        luaMgr.GetValue("TYPE", pDamage->m_kSummonMonInfo.m_iMonsterID);

        //printf("Type --> %d\n",pDamage->m_kSummonMonInfo.m_iMonsterID);

        luaMgr.GetValue("STATE", pDamage->m_kSummonMonInfo.m_szState, sizeof(char) * 16);
        luaMgr.GetValue("SUMMON_LIFETIME", pDamage->m_kSummonMonInfo.m_iSummonLifeTime);
        luaMgr.GetValue("SUMMON_LEVEL", pDamage->m_kSummonMonInfo.m_iLevel);
        luaMgr.GetValue("OFFSET_X", pDamage->m_kSummonMonInfo.m_fOffsetX);
        luaMgr.GetValue("OFFSET_Y", pDamage->m_kSummonMonInfo.m_fOffsetY);
        luaMgr.GetValue("LIMITE_LIVE_MONSTER", pDamage->m_kSummonMonInfo.m_iLimiteLiveMonsterCnt);
        luaMgr.GetValue("IS_LOCK_CHARACTER", pDamage->m_kSummonMonInfo.m_bIsLock);
    }

    // ����ź
    LUA_BEGIN_TABLE_RAII("GUIDEDMISSILE")
    {
        LUA_GET_VALUE_DEF("TARGET_CONDITION", pDamage->m_GuidedMissile.m_iGMCondition, -1);
        LUA_GET_VALUE_DEF("END_GUIDED_TIME", pDamage->m_GuidedMissile.m_iEndGMTime, 0);
        LUA_GET_VALUE_DEF("NOT_GUIDED_TIME", pDamage->m_GuidedMissile.m_iNotGMTime, 0);
        LUA_GET_VALUE_DEF("NOT_GUIDED_TIME_X_SPEED", pDamage->m_GuidedMissile.m_fNotGMTimeSpeedX, 0.0f);
        LUA_GET_VALUE_DEF("NOT_GUIDED_TIME_Y_SPEED", pDamage->m_GuidedMissile.m_fNotGMTimeSpeedY, 0.0f);
        LUA_GET_VALUE_DEF("NOT_TARGET_DAMAGE_LIFE", pDamage->m_GuidedMissile.m_iNotTargetLife, -1);
        LUA_GET_VALUE_DEF("HOMING_ANGLE", pDamage->m_GuidedMissile.m_fHomingAngle, 5.0f);
        LUA_GET_VALUE_DEF("OFFSET_X", pDamage->m_GuidedMissile.m_vTargetOffset.x, 0.0f);
        LUA_GET_VALUE_DEF("OFFSET_Y", pDamage->m_GuidedMissile.m_vTargetOffset.y, 0.0f);
        LUA_GET_VALUE_DEF("SHOPTTING_DIST_X", pDamage->m_GuidedMissile.m_vGMShottingDist.x, 0.0f);
        LUA_GET_VALUE_DEF("SHOPTTING_DIST_Y", pDamage->m_GuidedMissile.m_vGMShottingDist.y, 0.0f);
        LUA_GET_VALUE_DEF("ACCELEATOR_X", pDamage->m_GuidedMissile.m_vGMAccelerator.x, 0.0f);
        LUA_GET_VALUE_DEF("ACCELEATOR_Y", pDamage->m_GuidedMissile.m_vGMAccelerator.y, 0.0f);

        LUA_GET_VALUE_DEF("RESET_TARGET", pDamage->m_GuidedMissile.m_bIsResetTarget, false);

        LUA_BEGIN_TABLE_RAII("GUIDEDRECT")
        {
            LUA_GET_VALUE_DEF(1, pDamage->m_GuidedMissile.m_fCollisionRect.m_Left, 0.0f);
            LUA_GET_VALUE_DEF(2, pDamage->m_GuidedMissile.m_fCollisionRect.m_Top, 0.0f);
            LUA_GET_VALUE_DEF(3, pDamage->m_GuidedMissile.m_fCollisionRect.m_Right, 0.0f);
            LUA_GET_VALUE_DEF(4, pDamage->m_GuidedMissile.m_fCollisionRect.m_Bottom, 0.0f);
        }
        LUA_GET_VALUE_DEF("GUIDED_STRAIGHT", pDamage->m_GuidedMissile.m_bIsStraight, false);
        LUA_GET_VALUE_DEF("USE_TARGET_DIRECTION", pDamage->m_GuidedMissile.m_bUseTargetDirection, false);
        LUA_GET_VALUE_DEF("TARGET_DAMAGE_TO_ENUM", pDamage->m_GuidedMissile.m_iTargetDamageToEnum, -1);
        LUA_GET_VALUE_DEF("TARGET_MONSTER_TO_TYPE", pDamage->m_GuidedMissile.m_iTargetMonsterToType, -1);
        LUA_GET_VALUE_DEF("TARGET_PLAYER_TO_ID", pDamage->m_GuidedMissile.m_iTargetPlayerToID, -1);
        LUA_GET_VALUE_DEF("TARGET_CHAR_TO_TYPE", pDamage->m_GuidedMissile.m_iTargetCharToType, -1);
        LUA_GET_VALUE_DEF("DIE_AT_TARGET_POS", pDamage->m_GuidedMissile.m_bDieAtTargetPos, false);
        LUA_GET_VALUE_DEF("DIE_OFFSET_X", pDamage->m_GuidedMissile.m_dxvDiePosOffset.x, 0.0f);
        LUA_GET_VALUE_DEF("DIE_OFFSET_Y", pDamage->m_GuidedMissile.m_dxvDiePosOffset.y, 0.0f);

        pDamage->m_GuidedMissile.m_bSetStruct = true;
    }

    // ���Ϳ��� �ش�Ǵ� ����
    luaMgr.GetValue("ATTACK_TYPE_TO_MONSTER", pDamage->m_iAttackType);
    pDamage->m_iAttackTypeOnAerial = pDamage->m_iAttackType;
    luaMgr.GetValue("ATTACK_TYPE_TO_AERIAL", pDamage->m_iAttackTypeOnAerial);
    luaMgr.GetValue("ATTACK_DIR_TO_MONSTER", pDamage->m_iAttackDirection);
    luaMgr.GetValue("PUSH_X_TO_MONSTER", pDamage->m_fPushTargetX);
    luaMgr.GetValue("PUSH_Y_TO_MONSTER", pDamage->m_fPushTargetY);
    luaMgr.GetValue("MONSTER_SUPER_DAMAGE", pDamage->m_bMonSuperDamage);
    luaMgr.GetValue("IS_SPECIAL_ATTACK", pDamage->m_bSpecialAttack);
    luaMgr.GetValue("IS_SLOT_UNIQUE_SKILL_DAMAGE", pDamage->m_bSlotUniqueSkillDmg);
    luaMgr.GetValue("IS_SPECIAL4_DAMAGE", pDamage->m_bSpecial4Damage);
    luaMgr.GetValue("SPECIAL_GRADE", pDamage->m_nSpecialGrade);

    luaMgr.GetValue("NO_CATCH_FRAME", pDamage->m_iCannotCatchFrame);
    luaMgr.GetValue("NO_CHECK_CONTACT", pDamage->m_iNoCheckContact);
    //����� �� ���� ���������� ����
    luaMgr.GetValue("NO_PASS_DAMAGE", pDamage->m_bNoPassDamage);
    //������ HP
    luaMgr.GetValue("DAMAGE_HP", pDamage->m_iDamageHP);
    //������ HP�� ���� �� �ִ� ����������
    LUA_BEGIN_TABLE_RAII("PERMISSION_DAMAGE_HP")
    {
        int iDamageWhat;
        pDamage->m_vecPermissionDamageHP.clear();
        for (int iLoop = 1; SUCCEEDED(luaMgr.GetValue(iLoop, iDamageWhat)); ++iLoop)
        {
            pDamage->m_vecPermissionDamageHP.push_back(iDamageWhat);
        }
    }

    // ������ ���͸� �浹�ϴ� ������
    LUA_BEGIN_TABLE_RAII("CRASHDAMAGE_TO_MONSTERTYPE")
    {
        int iMonsterType;
        pDamage->m_vecCrashDamageToMonsterType.clear();
        for (int iLoop = 1; SUCCEEDED(luaMgr.GetValue(iLoop, iMonsterType)); ++iLoop)
        {
            pDamage->m_vecCrashDamageToMonsterType.push_back(iMonsterType);
        }
    }

    //	�ڽ��� HitPerFrame�� ������ �θ�ɷ� �������ְ�
    //	�θ� ������ 0���� �ʱ�ȭ ���ش�.
    if (FAILED(luaMgr.GetValue("HIT_PER_FRAME", pDamage->m_iHitPerFrame)))
    {
        if (iParent != -1)
        {
            pDamage->m_iHitPerFrame = m_vecInfo[iParent]->m_iHitPerFrame;
        }
        else
            pDamage->m_iHitPerFrame = 0;
    }

    luaMgr.GetValue("IS_TOTAL_RANGE", pDamage->m_bIsTotalRange);
    luaMgr.GetValue("IS_GROUND_BOTTOMBOX", pDamage->m_bIsGroundBottomBox);

    luaMgr.GetValue("SLOW_COUNT", pDamage->m_iSlowCount);
    luaMgr.GetValue("NO_CHECK_CONTACT_DAMAGE", pDamage->m_iNoCheckContactDamage);

    luaMgr.GetValue("USER_FRAME_DEPENDENT", pDamage->m_bUserFrameDependant);
    luaMgr.GetValue("USER_MOTION_DEPENDENT", pDamage->m_bUserMotionDependant);

    luaMgr.GetValue("GRAVITY", pDamage->m_fGravity);
    luaMgr.GetValue("IS_ACCGRAVITY", pDamage->m_bAccGravity);

    luaMgr.GetValue("NO_ALPHA_CHANGE", pDamage->m_bNoAlphaChange);

    luaMgr.GetValue("DRAIN_HP", pDamage->m_fLeechHP);
    luaMgr.GetValue("DRAIN_MP", pDamage->m_fLeechMP);

    luaMgr.GetValue("ABSORB_HP", pDamage->m_fAbsorbHP);
    luaMgr.GetValue("ABSORB_HP_TO_MAXHP", pDamage->m_fAbsorbHPToMaxHP);
    luaMgr.GetValue("ABSORB_HP_TO_DAMAGE", pDamage->m_fAbsorbHPToDamage);

    luaMgr.GetValue("ADD_ABSORB_HP_PER_HIT", pDamage->m_iAddAbsorbHPPerHit);

    luaMgr.GetValue("ALWAYS_ENEMY", pDamage->m_bAlwaysEnemy);
    luaMgr.GetValue("ATTACK_TO_SAMETEAM", pDamage->m_bAttackToSameTeam);

    luaMgr.GetValue("TRACE_OWNER_DIRECTION", pDamage->m_bOwnerDirection);
    luaMgr.GetValue("MAX_INSTANCE_NUM", pDamage->m_iMaxInstanceNum);
    luaMgr.GetValue("STOP_AT_CONTACT", pDamage->m_bStopAtContact);
    luaMgr.GetValue("END_AT_CONTACT", pDamage->m_bEndAtContact);
    luaMgr.GetValue("CHECK_BOUNDARY", pDamage->m_bCheckBoundary);
    luaMgr.GetValue("CREATE_CHECK_BOUNDARY", pDamage->m_bCreateCheckBoundary);
    luaMgr.GetValue("END_AT_LASTBOTTOM_CONTACT", pDamage->m_bEndAtLastBottomContact);
    luaMgr.GetValue("STOP_AT_LASTBOTTOM_CONTACT", pDamage->m_bStopAtLastBottomContact);


    luaMgr.GetValue("DAMAGE_SPEED_BY_DAMAGE_DIRECTION", pDamage->m_bApplyDamageSpeedByDamageDirection);

    luaMgr.GetValue("HASTE", pDamage->m_iHaste);
    luaMgr.GetValue("FATAL_RATIO", pDamage->m_iFatalRatio);
    luaMgr.GetValue("TOGGLE_MESH", pDamage->m_bToggleMesh);
    luaMgr.GetValue("TOGGLE_SUMMONMONSTER_MESH_ID", pDamage->m_iToggleSummonMonsterID);
    luaMgr.GetValue("DAMAGE_TO_TELEPORT", pDamage->m_iDamageToTeleport);
    luaMgr.GetValue("IGNORE_SPECIAL_MON_DAMAGE_TO_TELEPORT", pDamage->m_bIgnoreSpecialMonDamageToTeleport);
    luaMgr.GetValue("SHIELD_BREAK", pDamage->m_bShieldBreak);
    luaMgr.GetValue("NOT_SPECIAL_SUPER", pDamage->m_uiNotSpecialSuper);
    luaMgr.GetValue("NEXT_ATTACK_COUNT", pDamage->m_iNextAttackCount);
    luaMgr.GetValue("PARTICLE_SLOW", pDamage->m_bParticleSlow);
    luaMgr.GetValue("OWNER_KILL_DAMAGE", pDamage->m_bOwnerKillDamage);
    luaMgr.GetValue("IS_GAS_DAMSGE", pDamage->m_bGasDamage);
    luaMgr.GetValue("IS_CHARACTER_LOCK", pDamage->m_bIsCharLock);
    luaMgr.GetValue("UNIQUE_HIT_DAMAGE", pDamage->m_iUniqueDamage);
    luaMgr.GetValue("CHECK_NO_PASS_FLOOR", pDamage->m_bCheckNoPassFloor);
    luaMgr.GetValue("MONSTER_TARGETTING_RELEASE", pDamage->m_bMonTargettingRelease);


    luaMgr.GetValue("TRACE_CREATE_OFFSET_X", pDamage->m_fTraceCreateOffsetX);
    luaMgr.GetValue("TRACE_CREATE_OFFSET_Y", pDamage->m_fTraceCreateOffsetY);

    luaMgr.GetValue("NO_CRASH_PLAYER", pDamage->m_bNoCrashPlayer);
    luaMgr.GetValue("NO_CRASH_MONSTER", pDamage->m_bNoCrashMonster);
    luaMgr.GetValue("NO_CRASH_DAMAGE", pDamage->m_bNoCrashDamage);
    luaMgr.GetValue("NO_CRASH_GAMEOBJECT", pDamage->m_bNoCrashGameObject);
    luaMgr.GetValue("NO_CRASH_2DOBJECT", pDamage->m_bNoCrash2DObject);


    LUA_BEGIN_TABLE_RAII("SKIP_INFO")
    {
        luaMgr.GetValue("CONTINUE_FRAME", pDamage->m_pairSkipInfo.first);
        luaMgr.GetValue("SKIP_FRAME", pDamage->m_pairSkipInfo.second);
    }

    LUA_BEGIN_TABLE_RAII("SYNC_DAMAGE_POS_AT_CRASH_POS")
    {
        pDamage->m_kSyncDamagePosAtCrashPos.m_bIsSync = true;
        luaMgr.GetValue("TYPE", pDamage->m_kSyncDamagePosAtCrashPos.m_iSyncType);
        luaMgr.GetValue("OFFSET_X", pDamage->m_kSyncDamagePosAtCrashPos.m_fOffsetX);
        luaMgr.GetValue("OFFSET_Y", pDamage->m_kSyncDamagePosAtCrashPos.m_fOffsetY);
        luaMgr.GetValue("IS_CONTINUE", pDamage->m_kSyncDamagePosAtCrashPos.m_bContinue);
    }

    LoadOnlyMotionDamage(luaMgr, pDamage->m_kOnlyMotionDamage);
    LUA_BEGIN_TABLE_RAII("MOVE_CREATER_TO_CRASHPOS")
    {
        pDamage->m_kMoveCreateToCrashPos.m_bIsMoving = true;
        luaMgr.GetValue("OFFSET_X", pDamage->m_kMoveCreateToCrashPos.m_fOffsetX);
        luaMgr.GetValue("OFFSET_Y", pDamage->m_kMoveCreateToCrashPos.m_fOffsetY);
        luaMgr.GetValue("DAMAGE_LIFE", pDamage->m_kMoveCreateToCrashPos.m_iDamageLife);
    }

    LUA_BEGIN_TABLE_RAII("MOVE_CREATER_TO_UNITPOS")
    {
        pDamage->m_kMoveCreateToUnitPos.m_bIsMoving = true;
        luaMgr.GetValue("OFFSET_X", pDamage->m_kMoveCreateToUnitPos.m_fOffsetX);
        luaMgr.GetValue("OFFSET_Y", pDamage->m_kMoveCreateToUnitPos.m_fOffsetY);
        luaMgr.GetValue("DAMAGE_LIFE", pDamage->m_kMoveCreateToUnitPos.m_iDamageLife);
    }

    LUA_BEGIN_TABLE_RAII("BOUNDARY_REFLECTION")
    {
        pDamage->m_kBoundaryReflection.Init();
        pDamage->m_kBoundaryReflection.m_bSetStruct = true;
        luaMgr.GetValue("LEFT_SIDE_OFFSET", pDamage->m_kBoundaryReflection.m_fLeftSideOffset);
        luaMgr.GetValue("RIGHT_SIDE_OFFSET", pDamage->m_kBoundaryReflection.m_fRightSideOffset);
        luaMgr.GetValue("BOTTOM_OFFSET", pDamage->m_kBoundaryReflection.m_fBottomOffset);
        luaMgr.GetValue("TOP", pDamage->m_kBoundaryReflection.m_fTop);
        luaMgr.GetValue("REFLECTION_DECREASE_RADIO", pDamage->m_kBoundaryReflection.m_fReflectionDecreaseRadio);
        luaMgr.GetValue("END_DAMAGE", pDamage->m_kBoundaryReflection.m_bEndDamage);
        luaMgr.GetValue("NO_CHECK_DIFFGROUND", pDamage->m_kBoundaryReflection.m_bNoCheckDiffGround);
    }

    LoadCrashParticle(luaMgr, pDamage->m_vecCrashParticle);
    LoadAutoDamage(luaMgr, pDamage->m_vecAutoDamage);
    LoadTargetAutoDamage(luaMgr, pDamage->m_vecTargetAutoDamage);
    LoadTimeSound(luaMgr, pDamage->m_vecTimeSound);
    LoadTrigger(luaMgr, pDamage->m_vecTrigger);


    if (SUCCEEDED(luaMgr.BeginTable("PASS_BUFF")))
    {
        pDamage->m_vecPassBuff.clear();
        for (int i = 1; ; ++i) {
            int iBuffEnum = 1;
            LUA_GET_VALUE(i, iBuffEnum, break);
            pDamage->m_vecPassBuff.push_back(iBuffEnum);
        }

        luaMgr.EndTable();
    }

    if (SUCCEEDED(luaMgr.BeginTable("PASS_NO_BUFF")))
    {
        pDamage->m_vecPassNoBuff.clear();
        for (int i = 1; ; ++i) {
            int iBuffEnum = 1;
            LUA_GET_VALUE(i, iBuffEnum, break);
            pDamage->m_vecPassNoBuff.push_back(iBuffEnum);
        }

        luaMgr.EndTable();
    }

    if (SUCCEEDED(luaMgr.BeginTable("MON_CRASH_REACTION")))
    {
        pDamage->m_kMonReaction.Init();
        luaMgr.GetValue("REACTION", pDamage->m_kMonReaction.m_strReaction);
        luaMgr.GetValue("PASS_BOSS", pDamage->m_kMonReaction.m_bPassBoss);
        std::string strTemp;
        if (SUCCEEDED(luaMgr.BeginTable("PASS_STATE")))
        {
            for (int i = 1; ; ++i)
            {
                LUA_GET_VALUE(i, strTemp, break);
                pDamage->m_kMonReaction.m_vecPassState.push_back(strTemp);
            }
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }

    pDamage->m_mapNoCreateExceptionGameMode.clear();
    if (SUCCEEDED(luaMgr.BeginTable("NO_CREATE_EXCEPTION_GAMEMODE")))
    {
        int iValue;
        for (int i = 1; ; ++i)
        {
            LUA_GET_VALUE(i, iValue, break);
            pDamage->m_mapNoCreateExceptionGameMode.insert(std::make_pair(iValue, iValue));
        }
        luaMgr.EndTable();
    }

    if (SUCCEEDED(luaMgr.BeginTable("CRASH_DAMAGE_TO_DAMAGE")))
    {
        pDamage->m_kDamageToDamage.Init();
        LUA_GET_VALUE("MY_LIFE", pDamage->m_kDamageToDamage.m_iMyLife, -1);
        LUA_GET_VALUE("YOU_LIFE", pDamage->m_kDamageToDamage.m_iYouLife, -1);
        LUA_GET_VALUE("SUMMON_MONSTER", pDamage->m_kDamageToDamage.m_bSummonMonster, false);
        LUA_GET_VALUE("NO_CHECK_SAMETEAM", pDamage->m_kDamageToDamage.m_bNoCheckSameTeam, false);

        pDamage->m_kDamageToDamage.m_vecApointDamageEnum.clear();
        if (SUCCEEDED(luaMgr.BeginTable("APOINT_DAMAGE")))
        {
            int iDamageEnum;
            for (int iLoop = 1; SUCCEEDED(luaMgr.GetValue(iLoop, iDamageEnum)); ++iLoop)
            {
                pDamage->m_kDamageToDamage.m_vecApointDamageEnum.push_back(iDamageEnum);
            }

            luaMgr.EndTable();
        }

        pDamage->m_kDamageToDamage.m_vecExceptDamageEnum.clear();
        if (SUCCEEDED(luaMgr.BeginTable("EXCEPT_DAMAGE")))
        {
            int iDamageEnum;
            for (int iLoop = 1; SUCCEEDED(luaMgr.GetValue(iLoop, iDamageEnum)); ++iLoop)
            {
                pDamage->m_kDamageToDamage.m_vecExceptDamageEnum.push_back(iDamageEnum);
            }

            luaMgr.EndTable();
        }

        pDamage->m_kDamageToDamage.m_bSetStruct = true;

        luaMgr.EndTable();
    }

    pDamage->m_vecTogetherDieDamage.clear();
    if (SUCCEEDED(luaMgr.BeginTable("TOGETHER_DIE_DAMAGE_LIST")))
    {
        for (int i = 1; ; ++i) {
            int iBuffEnum = 1;
            LUA_GET_VALUE(i, iBuffEnum, break);
            pDamage->m_vecTogetherDieDamage.push_back(iBuffEnum);
        }
        luaMgr.EndTable();
    }
    luaMgr.GetValue("GROUND_OFFSET_X", pDamage->m_fGround_Offset_X);
    luaMgr.GetValue("GROUND_OFFSET_Y", pDamage->m_fGround_Offset_Y);

    luaMgr.GetValue("ONLY_ONE_UNIT", pDamage->m_bOnlyOneUnit);
    luaMgr.GetValue("UNIT_STOP_FRAME", pDamage->m_iStopUnitFrame);

    luaMgr.GetValue("TOGETHER_DIE", pDamage->m_bTogetherDie);
    luaMgr.GetValue("NO_PUSHSPEED_TO_FATAL", pDamage->m_bNoPushSpeedToFatal);
    luaMgr.GetValue("SAMETEAM_DAMAGETO_MONSTER", pDamage->m_fSameTeamDamageToMonster);

    luaMgr.GetValue("AUTODAMAGE_TO_GROUNDCRASH_OF_NODIECURDAMAGE", pDamage->m_bAutoDamageToGroundCrashOfNoDieCurDamage);

    pDamage->m_kDivisionDamage.Init();
    if (SUCCEEDED(luaMgr.BeginTable("DIVISION_DAMAGE_RANGE")))
    {
        int iNum = 0;

        luaMgr.GetValue("NUM_DIVISION_RANGE", iNum);
        pDamage->m_kDivisionDamage.m_iNumDivision = iNum;
        luaMgr.GetValue("CHECK_X_RANGE", pDamage->m_kDivisionDamage.m_bCheckRangeX);
        luaMgr.GetValue("CHECK_Y_RANGE", pDamage->m_kDivisionDamage.m_bCheckRangeY);

        if (iNum > 0) {
            if (SUCCEEDED(luaMgr.BeginTable("DAMAGE_PER_RANGE")))
            {
                for (int i = 1; i <= iNum; i++) {
                    float fDamageRatio = 1.f;
                    luaMgr.GetValue(i, fDamageRatio);
                    pDamage->m_kDivisionDamage.m_vecDamageRatio.push_back(fDamageRatio);
                }
                luaMgr.EndTable();
            }
        }
        luaMgr.EndTable();
    }

    if (SUCCEEDED(luaMgr.BeginTable("TOGETHER_DIE_BUFF")))
    {
        pDamage->m_vecTogetherDieBuff.clear();
        for (int i = 1; ; ++i) {
            int iBuffEnum = 1;
            LUA_GET_VALUE(i, iBuffEnum, break);
            pDamage->m_vecTogetherDieBuff.push_back(iBuffEnum);
        }

        luaMgr.EndTable();
    }

    if (pDamage->m_dwDMAttribute & DA_UNIQUE_DAMAGE)
        pDamage->m_iMaxInstanceNum = 1;
    return true;
}

void CDamageManager::DeleteDevice()
{
    UnLoad();
}

void CDamageManager::InitDevice(void)
{

}

void CDamageManager::UnLoad(void)
{
    if (m_apTexture)
    {
        for (int i = 0; i < m_i_MAX_DAMAGE_TEX_NUM; i++)
        {
            SAFE_RELEASE(m_apTexture[i]);
        }
    }
    SAFE_DELETE_ARRAY(m_apTexture);
    m_i_MAX_DAMAGE_TEX_NUM = 0;

    std::vector<CDamage*>::iterator vit;
    for (vit = m_vecInfo.begin(); vit != m_vecInfo.end(); ++vit)
    {
        SAFE_DELETE((*vit));
    }
    m_vecInfo.clear();
}

void CDamageManager::CheckAndChange(int& What, int Who)
{
    if (g_MyD3D->IsPlayerIndex(Who) == false)
        return;

    if (g_MyD3D->MyPlayer[Who]->m_StrongLevel < 8)
        return;

    DWORD Elemental = g_MyD3D->MyPlayer[Who]->m_Elemental;

    switch (Elemental)
    {
    case ITEM_ELEMENTAL_LIGHTNING:
    {
        //���� 8�ܰ� �̻��� ���� �ٲ�� �͵��� �Ʒ��� ����ϼ���.
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_LIGHTNING;                      break;
        case DT_CONTINUOUS_ARROW:			What = DT_CONTINUOUS_ARROW_LIGHTNING;           break;
        case DT_CONTINUOUS_ARROW_LV1:		What = DT_CONTINUOUS_ARROW_LV1_LIGHTNING;       break;
        case DT_CONTINUOUS_ARROW_LV2:		What = DT_CONTINUOUS_ARROW_LV2_LIGHTNING;       break;
        case DT_EXPLOSION_ARROW:            What = DT_EXPLOSION_ARROW_LIGHTNING;            break;
        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_LIGHTNING;             break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_LIGHTNING;                break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_LIGHTNING;            break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_LIGHTNING; break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_LIGHTNING;              break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_LIGHTNING_DUN;          break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_LIGHTNING;                   break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_LIGHTNING;                 break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_LIGHTNING_DUN;             break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_LIGHTNING;  break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_LIGHTNING;                 break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_LIGHTNING;             break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_LIGHTNING;       break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_LIGHTNING;       break;
        case DT_EXPLOSION_ARROW_READY:      What = DT_EXPLOSION_ARROW_READY_LIGHTNING;      break;
        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_LIGHTNING;              break;
        case DT_BOLT:                       What = DT_BOLT_LIGHTNING;                       break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_LIGHTNING;         break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_LIGHTNING;                 break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_LIGHTNING;        break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_LIGHTNING;          break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_LIGHTNING;          break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_LIGHTNING;        break;
            // ��� ���� ��ȭ
        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_LIGHTNING;                  break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_LIGHTNING_SHOT;   break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_LIGHTNING_SHOT_DUN;   break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_LIGHTNING;                 break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_LIGHTNING;        break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_LIGHTNING;          break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_LIGHTNING_DUN;      break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_LIGHTNING;         break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_LIGHTNING;         break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_LIGHTNING;        break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:        What = DT_NOVA_SPECIAL3_ARROW_LIGHTNING_DUN;        break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_LIGHTNING;         break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_LIGHTNING;        break;
        case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_LIGHTNING;             break;
        case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_LIGHTNING;              break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_LIGHTNING_SHOT;   break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_LIGHTNING_SHOT_DUN;   break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_LIGHTNING;            break;
        }
    }
    break;

    case ITEM_ELEMENTAL_SHINING:
    {
        //�� 8�ܰ� �̻��� ���� �ٲ�� �͵��� �Ʒ��� ����ϼ���.
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_SHINING;                            break;
        case DT_CONTINUOUS_ARROW:		    What = DT_CONTINUOUS_ARROW_SHINNING;	            break;
        case DT_CONTINUOUS_ARROW_LV1:       What = DT_CONTINUOUS_ARROW_LV1_SHINING;             break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_SHINING;             break;
        case DT_EXPLOSION_ARROW:            What = DT_EXPLOSION_ARROW_SHINING;                  break;
        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_SHINING;                   break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_SHINING;                      break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_SHINING;                  break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_SHINING;       break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_SHINING;                    break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_SHINING_DUN;                break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_SHINING;                         break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_SHINING;                       break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_SHINING_DUN;                   break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_SHINING;        break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_SHINING;                       break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_SHINING;                   break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_SHINING;             break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_SHINING;             break;
        case DT_EXPLOSION_ARROW_READY:      What = DT_EXPLOSION_ARROW_READY_SHINING;            break;
        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_SHINING;                    break;
        case DT_BOLT:                       What = DT_BOLT_SHINING;                             break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_SHINING;               break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_SHINING;                       break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_SHINING;              break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_SHINING;                break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_SHINING;                break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_SHINING;              break;
            // ��� �� ��ȭ
        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_SHINING;                    break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_SHINING_SHOT;     break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_SHINING_SHOT_DUN;     break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_SHINING;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_SHINING;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_SHINING;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_SHINING_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_SHINING;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_SHINING;           break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_SHINING;          break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:        What = DT_NOVA_SPECIAL3_ARROW_SHINING_DUN;          break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_SHINING;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_SHINING;          break;
        case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_SHINING;               break;
        case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_SHINING;                break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_SHINING_SHOT;     break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_SHINING_SHOT_DUN;     break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_SHINING;              break;
        case DT_LAS4_SPECIAL3_SWORD_L:      What = DT_LAS4_SPECIAL3_LIGHT_SWORD_L;          break;
        case DT_LAS4_SPECIAL3_SWORD_R:      What = DT_LAS4_SPECIAL3_LIGHT_SWORD_R;          break;

        case DT_METEO:                      What = DT_METEO_SHINING;                        break;
        case DT_METEO_CRASH_WAVE:           What = DT_METEO_CRASH_WAVE_SHINING;             break;

            // ��Ʋ������ 3���� ���׿�
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_SHINING;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_SHINING;   break;
        }
    }
    break;

    case ITEM_ELEMENTAL_DARK:
    {
        switch (What)
        {
        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_DARK;                   break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_DARK_SHOT;    break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_DARK_SHOT_DUN;    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_DARK;                  break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_DARK;         break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_DARK;           break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_DARK_DUN;       break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_DARK;          break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_DARK;          break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_DARK;         break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:    What = DT_NOVA_SPECIAL3_ARROW_DARK_DUN;     break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_DARK;          break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_DARK;         break;
        case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_DARK;              break;
        case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_DARK;               break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_DARK_SHOT;    break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_DARK_SHOT_DUN;   break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_DARK;             break;
        case DT_LAS4_SPECIAL3_SWORD_L:      What = DT_LAS4_SPECIAL3_DARK_SWORD_L;       break;
        case DT_LAS4_SPECIAL3_SWORD_R:      What = DT_LAS4_SPECIAL3_DARK_SWORD_R;       break;
        }
    }
    break;

    case ITEM_ELEMENTAL_ICE:
    {
        switch (What)
        {
        case DT_METEO:                          What = DT_METEO_ICE;                        break;
        case DT_METEO_CRASH_WAVE:               What = DT_METEO_CRASH_WAVE_ICE;             break;

            // ��Ʋ������ 3���� ���׿�
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_ICE;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_ICE;   break;
        case DT_LAS4_SPECIAL3_SWORD_L:          What = DT_LAS4_SPECIAL3_ICE_SWORD_L;        break;
        case DT_LAS4_SPECIAL3_SWORD_R:          What = DT_LAS4_SPECIAL3_ICE_SWORD_R;        break;
        }
    }
    break;

    case ITEM_ELEMENTAL_FIRE:
    {
        switch (What)
        {
        case DT_LAS4_SPECIAL3_SWORD_L:          What = DT_LAS4_SPECIAL3_FIRE_SWORD_L;            break;
        case DT_LAS4_SPECIAL3_SWORD_R:          What = DT_LAS4_SPECIAL3_FIRE_SWORD_R;            break;
        }
    }
    break;

    case ITEM_ELEMENTAL_LOVE:
    case ITEM_ELEMENTAL_ROSE:
    {
        //�� 8�ܰ� �̻��� ���� �ٲ�� �͵��� �Ʒ��� ����ϼ���.
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_LOVE;                       break;
        case DT_CONTINUOUS_ARROW:           What = DT_CONTINUOUS_ARROW_LOVE;            break;
        case DT_CONTINUOUS_ARROW_LV1:       What = DT_CONTINUOUS_ARROW_LV1_LOVE;        break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_LOVE;        break;
        case DT_EXPLOSION_ARROW:            What = DT_EXPLOSION_ARROW_LOVE;             break;
        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_LOVE;              break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_LOVE;                 break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_LOVE;             break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_LOVE;  break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_LOVE;               break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_LOVE_DUN;           break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_LOVE;                    break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_LOVE;                  break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_LOVE_DUN;              break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_LOVE;   break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_LOVE;                  break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_LOVE;              break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_LOVE;        break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_LOVE;        break;
        case DT_EXPLOSION_ARROW_READY:      What = DT_EXPLOSION_ARROW_READY_LOVE;       break;
        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_LOVE;               break;
        case DT_BOLT:                       What = DT_BOLT_LOVE;                        break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_LOVE;          break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_LOVE;                  break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_LOVE;         break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_LOVE;           break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_LOVE;           break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_LOVE;         break;
            // ��� �� ��ȭ
        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_LOVE;                    break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_LOVE_SHOT;     break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_LOVE_SHOT_DUN;     break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_LOVE;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_LOVE;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_LOVE;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_LOVE_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_LOVE;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_LOVE;           break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_LOVE;          break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:        What = DT_NOVA_SPECIAL3_ARROW_LOVE_DUN;          break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_LOVE;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_LOVE;          break;
        case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_LOVE;               break;
        case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_LOVE;                break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_LOVE_SHOT;     break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_LOVE_SHOT_DUN;     break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_LOVE;              break;
        case DT_LAS4_SPECIAL3_SWORD_L:      What = DT_LAS4_SPECIAL3_LOVE_SWORD_L;          break;
        case DT_LAS4_SPECIAL3_SWORD_R:      What = DT_LAS4_SPECIAL3_LOVE_SWORD_R;          break;

        case DT_METEO:                  What = DT_METEO_LOVE;                        break;
        case DT_METEO_CRASH_WAVE:       What = DT_METEO_CRASH_WAVE_LOVE;             break;

            // ��Ʋ������ 3���� ���׿�
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_LOVE;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_LOVE;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_LOVE;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_LOVE;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_LOVE;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_LOVE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_LOVE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_LOVE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_LOVE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_LOVE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_LOVE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_LOVE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_LOVE;   break;
        }
    }
    break;
    case ITEM_ELEMENTAL_DEATH:
    {
        switch (What)
        {
        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_DEATH;                   break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_DEATH_SHOT;    break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_DEATH_SHOT_DUN;    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_DEATH;                  break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_DEATH;         break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_DEATH;           break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_DEATH_DUN;       break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_DEATH;          break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_DEATH;          break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_DEATH;         break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:        What = DT_NOVA_SPECIAL3_ARROW_DEATH_DUN;         break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_DEATH;          break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_DEATH;         break;
            //          case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_DEATH;              break;
            //          case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_DEATH;               break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_DEATH_SHOT;    break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_DEATH_SHOT_DUN;    break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_DEATH;             break;

        }
    }
    break;
    case ITEM_ELEMENTAL_TIME:
    {
        //  �⺻�� ������ �����ϰ� ȭ�� ��ƼŬ�� �����Ѵ�.
        switch (What)
        {
        case DT_ARROW:                  What = DT_ARROW_TIME;                  break;
        case DT_CONTINUOUS_ARROW:		What = DT_CONTINUOUS_ARROW_TIME;       break;
        case DT_CONTINUOUS_ARROW_LV1:	What = DT_CONTINUOUS_ARROW_LV1_TIME;   break;
        case DT_CONTINUOUS_ARROW_LV2:	What = DT_CONTINUOUS_ARROW_LV2_TIME;   break;

        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_TIME;              break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_TIME;                 break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_TIME;             break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_TIME;  break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_TIME;               break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_TIME_DUN;           break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_TIME;                    break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_TIME;                  break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_TIME_DUN;              break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_TIME;   break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_TIME;                  break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_TIME;              break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_TIME;        break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_TIME;        break;

        case DT_CRITICAL_BOLT:          What = DT_CRITICAL_BOLT_TIME;          break;
        case DT_BOLT:                   What = DT_BOLT_TIME;                   break;
        case DT_LIRE2_SPECIAL1_DMG:     What = DT_LIRE2_SPECIAL1_DMG_TIME;     break;
        case DT_HUGE_ARROW:             What = DT_HUGE_ARROW_TIME;             break;
        case DT_HUGE_CRITICAL_ARROW:    What = DT_HUGE_CRITICAL_ARROW_TIME;    break;
        case DT_HUGE_RUNATK_ARROW:      What = DT_HUGE_RUNATK_ARROW_TIME;      break;
        case DT_HUGE_COMBO3_ARROW:      What = DT_HUGE_COMBO3_ARROW_TIME;      break;
        case DT_HUGE_JUMPDOWN_ARROW:    What = DT_HUGE_JUMPDOWN_ARROW_TIME;    break;

        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_TIME;                    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_TIME;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_TIME;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_TIME;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_TIME_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_TIME;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_TIME;           break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_TIME;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_TIME;          break;



            //  ���⼭ ���ʹ� ���̶� �Ȱ����� ����.
        case DT_EXPLOSION_ARROW_READY:      What = DT_EXPLOSION_ARROW_READY_SHINING;  break;
        case DT_EXPLOSION_ARROW:            What = DT_EXPLOSION_ARROW_SHINING;        break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_SHINING_SHOT;     break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_SHINING_SHOT_DUN;     break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_SHINING;          break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:        What = DT_NOVA_SPECIAL3_ARROW_SHINING_DUN;          break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_SHINING_SHOT;     break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_SHINING_SHOT_DUN;     break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_SHINING;              break;
        case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_SHINING;               break;
        case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_SHINING;                break;

#if !defined (NATION_BRAZIL)
        case DT_METEO:                      What = DT_METEO_SHINING;                        break;
        case DT_METEO_CRASH_WAVE:           What = DT_METEO_CRASH_WAVE_SHINING;             break;
            // ��Ʋ������ 3���� ���׿�
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_SHINING;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_SHINING;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_SHINING;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_SHINING;   break;
#endif
        }
    }
    break;
    case ITEM_ELEMENTAL_LIFE:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_LIFE;                       break;
        case DT_CONTINUOUS_ARROW:		    What = DT_CONTINUOUS_ARROW_LIFE;            break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_LIFE;        break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_LIFE;        break;

        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_LIFE;              break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_LIFE;                 break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_LIFE;             break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_LIFE;  break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_LIFE;               break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_LIFE_DUN;           break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_LIFE;                    break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_LIFE;                  break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_LIFE_DUN;              break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_LIFE;   break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_LIFE;                  break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_LIFE;              break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_LIFE;        break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_LIFE;        break;

        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_LIFE;               break;
        case DT_BOLT:                       What = DT_BOLT_LIFE;                        break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_LIFE;          break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_LIFE;                  break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_LIFE;         break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_LIFE;           break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_LIFE;           break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_LIFE;         break;

        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_LIFE;                    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_LIFE;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_LIFE;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_LIFE;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_LIFE_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_LIFE;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_LIFE;           break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_LIFE;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_LIFE;          break;
            // �Ƹ��� ���׿�

#if !defined (NATION_BRAZIL) && !defined ( NATION_IDN )
                // ��Ʋ������ 3���� ���׿�
        case DT_METEO:                          What = DT_METEO_LIFE;                        break;
        case DT_METEO_CRASH_WAVE:               What = DT_METEO_CRASH_WAVE_LIFE;             break;
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_LIFE;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_LIFE;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_LIFE;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_LIFE;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_LIFE;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_LIFE;   break;
#else
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_LIFE;    break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_LIFE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8;   break;
#endif
        }
    }
    break;
    case ITEM_ELEMENTAL_DARK_DRAGON_RED:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_DARK_DRAGON_RED;                        break;
        case DT_CONTINUOUS_ARROW:		    What = DT_CONTINUOUS_ARROW_DARK_DRAGON_RED;             break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_DARK_DRAGON_RED;         break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_DARK_DRAGON_RED;         break;

        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_DARK_DRAGON_RED;               break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_DARK_DRAGON_RED;                  break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_DARK_DRAGON_RED;              break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_DARK_DRAGON_RED;   break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_DARK_DRAGON_RED;                break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_DARK_DRAGON_RED_DUN;            break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_DARK_DRAGON_RED;                     break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_DARK_DRAGON_RED;                   break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_DARK_DRAGON_RED_DUN;               break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_RED;    break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_DARK_DRAGON_RED;                   break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_DARK_DRAGON_RED;               break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_DARK_DRAGON_RED;         break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_DARK_DRAGON_RED;         break;

        case DT_CRITICAL_BOLT:          What = DT_CRITICAL_BOLT_DARK_DRAGON_RED;          break;
        case DT_BOLT:                   What = DT_BOLT_DARK_DRAGON_RED;                   break;
        case DT_LIRE2_SPECIAL1_DMG:     What = DT_LIRE2_SPECIAL1_DMG_DARK_DRAGON_RED;     break;
        case DT_HUGE_ARROW:             What = DT_HUGE_ARROW_DARK_DRAGON_RED;             break;
        case DT_HUGE_CRITICAL_ARROW:    What = DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_RED;    break;
        case DT_HUGE_RUNATK_ARROW:      What = DT_HUGE_RUNATK_ARROW_DARK_DRAGON_RED;      break;
        case DT_HUGE_COMBO3_ARROW:      What = DT_HUGE_COMBO3_ARROW_DARK_DRAGON_RED;      break;
        case DT_HUGE_JUMPDOWN_ARROW:    What = DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_RED;    break;

        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_DARK_DRAGON_RED;                    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_DARK_DRAGON_RED;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_RED;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_RED;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_DARK_DRAGON_RED;           break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_RED;          break;

            // �Ƹ��� ���׿�
        case DT_METEO:                  What = DT_METEO_LOVE;                        break;
        case DT_METEO_CRASH_WAVE:       What = DT_METEO_CRASH_WAVE_LOVE;             break;

            // �Ƹ��� ������Ʈ
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_DARK_DRAGON_RED;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_RED;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_RED;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_RED;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_DARK_DRAGON_RED;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_DARK_DRAGON_RED;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_DARK_DRAGON_RED;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_DARK_DRAGON_RED;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_DARK_DRAGON_RED;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_DARK_DRAGON_RED;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_DARK_DRAGON_RED;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_DARK_DRAGON_RED;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_DARK_DRAGON_RED;   break;
        }
    }
    break;
    case ITEM_ELEMENTAL_DARK_DRAGON_BLUE:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_DARK_DRAGON_BLUE;                       break;
        case DT_CONTINUOUS_ARROW:		    What = DT_CONTINUOUS_ARROW_DARK_DRAGON_BLUE;            break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_DARK_DRAGON_BLUE;        break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_DARK_DRAGON_BLUE;        break;

        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_DARK_DRAGON_BLUE;              break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_DARK_DRAGON_BLUE;                 break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_DARK_DRAGON_BLUE;             break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_DARK_DRAGON_BLUE;  break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_DARK_DRAGON_BLUE;               break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_DARK_DRAGON_BLUE_DUN;           break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_DARK_DRAGON_BLUE;                    break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_DARK_DRAGON_BLUE;                  break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_DARK_DRAGON_BLUE_DUN;              break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_BLUE;   break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_DARK_DRAGON_BLUE;                  break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_DARK_DRAGON_BLUE;              break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_DARK_DRAGON_BLUE;        break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_DARK_DRAGON_BLUE;        break;

        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_DARK_DRAGON_BLUE;               break;
        case DT_BOLT:                       What = DT_BOLT_DARK_DRAGON_BLUE;                        break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_DARK_DRAGON_BLUE;     break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_DARK_DRAGON_BLUE;                  break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_BLUE;         break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_DARK_DRAGON_BLUE;           break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_DARK_DRAGON_BLUE;           break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_BLUE;         break;

        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_DARK_DRAGON_BLUE;                   break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_DARK_DRAGON_BLUE;                  break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_BLUE;         break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE;           break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE_DUN;       break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_BLUE;          break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_DARK_DRAGON_BLUE;          break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE;          break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_BLUE;         break;


        case DT_METEO:                          What = DT_METEO_ICE;                        break;
        case DT_METEO_CRASH_WAVE:               What = DT_METEO_CRASH_WAVE_ICE;             break;

            // �Ƹ��� ������Ʈ
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_DARK_DRAGON_BLUE;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_BLUE;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_BLUE;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_BLUE;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_DARK_DRAGON_BLUE;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_DARK_DRAGON_BLUE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_DARK_DRAGON_BLUE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_DARK_DRAGON_BLUE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_DARK_DRAGON_BLUE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_DARK_DRAGON_BLUE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_DARK_DRAGON_BLUE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_DARK_DRAGON_BLUE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_DARK_DRAGON_BLUE;   break;



        }
    }
    break;
    case ITEM_ELEMENTAL_HALLOWEEN:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_HALLOWEEN;                      break;
        case DT_CONTINUOUS_ARROW:		    What = DT_CONTINUOUS_ARROW_HALLOWEEN;           break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_HALLOWEEN;       break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_HALLOWEEN;       break;

        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_HALLOWEEN;             break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_HALLOWEEN;                break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_HALLOWEEN;            break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_HALLOWEEN; break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_HALLOWEEN;              break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_HALLOWEEN_DUN;          break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_HALLOWEEN;                   break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_HALLOWEEN;                 break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_HALLOWEEN_DUN;             break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_HALLOWEEN;  break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_HALLOWEEN;                 break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_HALLOWEEN;             break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_HALLOWEEN;       break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_HALLOWEEN;       break;

        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_HALLOWEEN;              break;
        case DT_BOLT:                       What = DT_BOLT_HALLOWEEN;                       break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_HALLOWEEN;         break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_HALLOWEEN;                 break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_HALLOWEEN;        break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_HALLOWEEN;          break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_HALLOWEEN;          break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_HALLOWEEN;        break;

        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_HALLOWEEN;                    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_HALLOWEEN;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_HALLOWEEN;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_HALLOWEEN;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_HALLOWEEN_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_HALLOWEEN;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_HALLOWEEN;           break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_HALLOWEEN;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_HALLOWEEN;          break;

            // �Ƹ��� ������Ʈ
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_HALLOWEEN;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_HALLOWEEN;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_HALLOWEEN;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_HALLOWEEN;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_HALLOWEEN;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_HALLOWEEN;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_HALLOWEEN;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_HALLOWEEN;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_HALLOWEEN;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_HALLOWEEN;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_HALLOWEEN;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_HALLOWEEN;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_HALLOWEEN;   break;
        }
    }
    break;

    case ITEM_ELEMENTAL_ELIA:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_ELIA;                       break;
        case DT_CONTINUOUS_ARROW:		    What = DT_CONTINUOUS_ARROW_ELIA;            break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_ELIA;        break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_ELIA;        break;

        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_ELIA;              break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_ELIA;                 break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_ELIA;             break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_ELIA;  break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_ELIA;               break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_ELIA_DUN;           break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_ELIA;                    break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_ELIA;                  break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_ELIA_DUN;              break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_ELIA;   break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_ELIA;                  break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_ELIA;              break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_ELIA;        break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_ELIA;        break;

        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_ELIA;               break;
        case DT_BOLT:                       What = DT_BOLT_ELIA;                        break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_ELIA;          break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_ELIA;                  break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_ELIA;         break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_ELIA;           break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_ELIA;           break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_ELIA;         break;

        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_ELIA;                    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_ELIA;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_ELIA;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_ELIA;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_ELIA_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_ELIA;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_ELIA;           break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_ELIA;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_ELIA;          break;

        case DT_METEO:                          What = DT_METEO_ELIA;                        break;
        case DT_METEO_CRASH_WAVE:               What = DT_METEO_CRASH_WAVE_ELIA;             break;

            // �Ƹ��� ������Ʈ
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_ELIA;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_ELIA;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_ELIA;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_ELIA;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_ELIA;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_ELIA;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_ELIA;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_ELIA;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_ELIA;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_ELIA;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_ELIA;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_ELIA;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_ELIA;   break;
        }
    }
    break;
// Origin Novos
    case ITEM_ELEMENTAL_CELESTIAL_MASTER:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_C_MASTER;                       break;
        case DT_CONTINUOUS_ARROW:           What = DT_CONTINUOUS_ARROW_C_MASTER;            break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_C_MASTER;        break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_C_MASTER;        break;

        case DT_EXPLOSION_ARROW:            What = DT_EXPLOSION_ARROW_C_MASTER;             break;
        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_C_MASTER;              break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_C_MASTER;                 break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_C_MASTER;             break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_C_MASTER;  break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_C_MASTER;               break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_C_MASTER_DUN;           break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_C_MASTER;                    break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_C_MASTER_DUN;              break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_C_MASTER;   break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_C_MASTER;                  break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_C_MASTER;              break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_C_MASTER;        break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_C_MASTER;        break;

        case DT_EXPLOSION_ARROW_READY:      What = DT_EXPLOSION_ARROW_READY_C_MASTER;   break;
        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_C_MASTER;           break;
        case DT_BOLT:                       What = DT_BOLT_C_MASTER;                    break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_C_MASTER;      break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_C_MASTER;              break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_C_MASTER;     break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_C_MASTER;       break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_C_MASTER;       break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_C_MASTER;     break;
            // ��� �� ��ȭ
        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_C_MASTER;                    break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_C_MASTER_SHOT;     break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_C_MASTER_SHOT_DUN;     break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_C_MASTER;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_C_MASTER;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_C_MASTER;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_C_MASTER_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_C_MASTER;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_C_MASTER;           break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_C_MASTER;          break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:        What = DT_NOVA_SPECIAL3_ARROW_C_MASTER_DUN;          break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_C_MASTER;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_C_MASTER;          break;
        case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_C_MASTER;               break;
        case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_C_MASTER;                break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_C_MASTER_SHOT;     break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_C_MASTER_SHOT_DUN;     break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_C_MASTER;              break;

        case DT_METEO:                          What = DT_METEO_C_MASTER;                        break;
        case DT_METEO_CRASH_WAVE:               What = DT_METEO_CRASH_WAVE_C_MASTER;             break;

            // �Ƹ��� ������Ʈ
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_C_MASTER;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_C_MASTER;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_C_MASTER;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_C_MASTER;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_C_MASTER;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_C_MASTER;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_C_MASTER;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_C_MASTER;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_C_MASTER;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_C_MASTER;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_C_MASTER;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_C_MASTER;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_C_MASTER;   break;
        }
    }
    break;
    case ITEM_ELEMENTAL_COLISEU:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_COLISEU;                        break;
        case DT_CONTINUOUS_ARROW:           What = DT_CONTINUOUS_ARROW_COLISEU;             break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_COLISEU;         break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_COLISEU;         break;

        case DT_EXPLOSION_ARROW:            What = DT_EXPLOSION_ARROW_COLISEU;              break;
        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_COLISEU;               break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_COLISEU;                  break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_COLISEU;              break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_COLISEU;   break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_COLISEU;                break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_COLISEU_DUN;            break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_COLISEU;                     break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_COLISEU;                   break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_COLISEU_DUN;               break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_COLISEU;    break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_COLISEU;                   break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_COLISEU;               break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_COLISEU;         break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_COLISEU;         break;
        case DT_EXPLOSION_ARROW_READY:      What = DT_EXPLOSION_ARROW_READY_COLISEU;        break;
        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_COLISEU;                break;
        case DT_BOLT:                       What = DT_BOLT_COLISEU;                         break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_COLISEU;           break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_COLISEU;                   break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_COLISEU;          break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_COLISEU;            break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_COLISEU;            break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_COLISEU;          break;
            // ��� �� ��ȭ
        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_COLISEU;                    break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT:   What = DT_NOVA_SPECIAL3_ARROW_COLISEU_SHOT;     break;
        case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL3_ARROW_COLISEU_SHOT_DUN;     break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_COLISEU;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_COLISEU;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_COLISEU;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_COLISEU_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_COLISEU;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_COLISEU;           break;
        case DT_NOVA_SPECIAL3_ARROW:        What = DT_NOVA_SPECIAL3_ARROW_COLISEU;          break;
        case DT_NOVA_SPECIAL3_ARROW_DUN:    What = DT_NOVA_SPECIAL3_ARROW_COLISEU_DUN;      break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_COLISEU;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_COLISEU;          break;
        case DT_NOVA_ARROW_EFF:             What = DT_NOVA_ARROW_EFF_COLISEU;               break;
        case DT_NOVA_BOLT_EFF:              What = DT_NOVA_BOLT_EFF_COLISEU;                break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT:   What = DT_NOVA_SPECIAL1_ARROW_COLISEU_SHOT;     break;
        case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:   What = DT_NOVA_SPECIAL1_ARROW_COLISEU_SHOT_DUN;     break;
        case DT_NOVA_DOWN_ARROW:            What = DT_NOVA_DOWN_ARROW_COLISEU;              break;

        case DT_METEO:                          What = DT_METEO_COLISEU;                        break;
        case DT_METEO_CRASH_WAVE:               What = DT_METEO_CRASH_WAVE_COLISEU;             break;

            // �Ƹ��� ������Ʈ
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_COLISEU;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_COLISEU;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_COLISEU;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_COLISEU;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_COLISEU;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_COLISEU;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_COLISEU;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_COLISEU;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_COLISEU;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_COLISEU;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_COLISEU;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_COLISEU;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_COLISEU;   break;
        }
    }
    break;
    //--//
    case ITEM_ELEMENTAL_PEGASUS_NECK:
    {
        switch (What)
        {
        case DT_ARROW:                      What = DT_ARROW_PEGASUS;                        break;
        case DT_CONTINUOUS_ARROW:		    What = DT_CONTINUOUS_ARROW_PEGASUS;             break;
        case DT_CONTINUOUS_ARROW_LV1:	    What = DT_CONTINUOUS_ARROW_LV1_PEGASUS;         break;
        case DT_CONTINUOUS_ARROW_LV2:	    What = DT_CONTINUOUS_ARROW_LV2_PEGASUS;         break;

        case DT_CRITICAL_ARROW:             What = DT_CRITICAL_ARROW_PEGASUS;               break;
        case DT_STORM_ARROW:                What = DT_STORM_ARROW_PEGASUS;                  break;
        case DT_STORM_ARROW_DUN:            What = DT_STORM_ARROW_DUN_PEGASUS;              break;
        case DT_LIRE1_SPECIAL4_STORM_ARROW: What = DT_LIRE1_SPECIAL4_STORM_ARROW_PEGASUS;   break;
        case DT_FREEWAY_ARROW:              What = DT_FREEWAY_ARROW_PEGASUS;                break;
        case DT_FREEWAY_ARROW_DUN:          What = DT_FREEWAY_ARROW_PEGASUS_DUN;            break;
        case DT_UP_ARROW:                   What = DT_UP_ARROW_PEGASUS;                     break;
        case DT_DOWN_ARROW:                 What = DT_DOWN_ARROW_PEGASUS;                   break;
        case DT_DOWN_ARROW_DUN:             What = DT_DOWN_ARROW_PEGASUS_DUN;               break;
        case DT_LIRE1_SPECIAL4_DOWN_ARROW:  What = DT_LIRE1_SPECIAL4_DOWN_ARROW_PEGASUS;    break;
        case DT_METAL_SHOT:                 What = DT_METAL_SHOT_PEGASUS;                   break;
        case DT_METAL_SHOT_LV1:             What = DT_METAL_SHOT_LV1_PEGASUS;               break;
        case DT_METAL_SHOT_LV2_ARROW:       What = DT_METAL_SHOT_LV2_ARROW_PEGASUS;         break;
        case DT_METAL_SHOT_LV3_ARROW:       What = DT_METAL_SHOT_LV3_ARROW_PEGASUS;         break;
        case DT_CRITICAL_BOLT:              What = DT_CRITICAL_BOLT_PEGASUS;                break;
        case DT_BOLT:                       What = DT_BOLT_PEGASUS;                         break;
        case DT_LIRE2_SPECIAL1_DMG:         What = DT_LIRE2_SPECIAL1_DMG_PEGASUS;           break;
        case DT_HUGE_ARROW:                 What = DT_HUGE_ARROW_PEGASUS;                   break;
        case DT_HUGE_CRITICAL_ARROW:        What = DT_HUGE_CRITICAL_ARROW_PEGASUS;          break;
        case DT_HUGE_RUNATK_ARROW:          What = DT_HUGE_RUNATK_ARROW_PEGASUS;            break;
        case DT_HUGE_COMBO3_ARROW:          What = DT_HUGE_COMBO3_ARROW_PEGASUS;            break;
        case DT_HUGE_JUMPDOWN_ARROW:        What = DT_HUGE_JUMPDOWN_ARROW_PEGASUS;          break;

        case DT_NOVA_BOLT:                  What = DT_NOVA_BOLT_PEGASUS;                    break;
        case DT_NOVA_ARROW:                 What = DT_NOVA_ARROW_PEGASUS;                   break;
        case DT_NOVA_CRITICAL_ARROW:        What = DT_NOVA_CRITICAL_ARROW_PEGASUS;          break;
        case DT_NOVA_FREEWAY_BOLT:          What = DT_NOVA_FREEWAY_BOLT_PEGASUS;            break;
        case DT_NOVA_FREEWAY_BOLT_DUN:      What = DT_NOVA_FREEWAY_BOLT_PEGASUS_DUN;        break;
        case DT_NOVA_CRITICAL_BOLT:         What = DT_NOVA_CRITICAL_BOLT_PEGASUS;           break;
        case DT_NOVA_SPECIAL3_BOLT:         What = DT_NOVA_SPECIAL3_BOLT_PEGASUS;           break;
        case DT_NOVA_SPECIAL1_BOLT:         What = DT_NOVA_SPECIAL1_BOLT_PEGASUS;           break;
        case DT_NOVA_SPECIAL1_ARROW:        What = DT_NOVA_SPECIAL1_ARROW_PEGASUS;          break;

            //�Ƹ��� ���׿�
        case DT_METEO:                          What = DT_METEO_ICE;                        break;
        case DT_METEO_CRASH_WAVE:               What = DT_METEO_CRASH_WAVE_ICE;             break;

            // �Ƹ��� �� ����Ʈ
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_ICE;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_ICE;   break;
        }
    }
    break;
    default:
        break;
    }
}

void CDamageManager::CheckMonsterAndChange(int& What, int iMonsterType)
{
    switch (iMonsterType)
    {
    case MON_BLACKFAIRYQUEEN:
    case MON_BLACKFAIRYQUEEN_BOSS:
    {
        switch (What)
        {
            // ��Ʋ������ 3���� ���׿�
        case DT_ARME4_SPECIAL3_BLACK:           What = DT_ARME4_SPECIAL3_BLACK_ICE;         break;
        case DT_ARME4_SPECIAL3_BLACK2:          What = DT_ARME4_SPECIAL3_BLACK2_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK3:          What = DT_ARME4_SPECIAL3_BLACK3_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK4:          What = DT_ARME4_SPECIAL3_BLACK4_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK5:          What = DT_ARME4_SPECIAL3_BLACK5_ICE;        break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM1:     What = DT_ARME4_SPECIAL3_BLACK_BOOM1_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM2:     What = DT_ARME4_SPECIAL3_BLACK_BOOM2_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM3:     What = DT_ARME4_SPECIAL3_BLACK_BOOM3_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM4:     What = DT_ARME4_SPECIAL3_BLACK_BOOM4_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM5:     What = DT_ARME4_SPECIAL3_BLACK_BOOM5_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM6:     What = DT_ARME4_SPECIAL3_BLACK_BOOM6_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM7:     What = DT_ARME4_SPECIAL3_BLACK_BOOM7_ICE;   break;
        case DT_ARME4_SPECIAL3_BLACK_BOOM8:     What = DT_ARME4_SPECIAL3_BLACK_BOOM8_ICE;   break;
        }
        break;
    }
    }
}

void CDamageManager::CallEffAtCrash(CDamageInstance* pDamageInstance, int HitWho, bool IsCrashPlayer, float _fHitX, float _fHitY)
{
    // ���⼺ ������ �ӽ�
    DIRECTION_INFO stDirect;
    stDirect.m_fDirPosX = 0;
    stDirect.m_bDirVelocity = true;
    stDirect.m_pbIsRight = &pDamageInstance->m_IsRight;

    float fHitX;
    float fHitY;

    if (HitWho == -1)
    {
        fHitX = _fHitX;
        fHitY = _fHitY;
    }
    else
    {
        if (IsCrashPlayer)
        {
            fHitX = g_MyD3D->MyPlayer[HitWho]->vPos.x;
            fHitY = g_MyD3D->MyPlayer[HitWho]->vPos.y;
            pDamageInstance->m_emHitWhoType = OBJ_TYPE_PLAYER;

            // ��������ġ�� �ǰ�ü�� �������� ����ȭ ��Ų��.
            // ��ġ�� �� �� ���� �� ���������� DAMAGE_XSPEED, DAMAGE_YSPEED�� ���� ����.
            if (pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos.m_bIsSync) {
                D3DXVECTOR2 vPos = SyncDamagePosAtCrashPos(pDamageInstance, HitWho, true);
                g_MyD3D->MyPlayer[HitWho]->vPos.x = vPos.x;
                g_MyD3D->MyPlayer[HitWho]->vPos.y = vPos.y;

                if (pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos.m_bContinue)
                {
                    std::vector<int>::iterator veciIter = pDamageInstance->m_vecPlayerSyncDamagePosAtCrashPos.begin();
                    for (; veciIter != pDamageInstance->m_vecPlayerSyncDamagePosAtCrashPos.end(); ++veciIter)
                    {
                        if (HitWho == (*veciIter))
                            break;
                    }
                    if (veciIter == pDamageInstance->m_vecPlayerSyncDamagePosAtCrashPos.end())
                        pDamageInstance->m_vecPlayerSyncDamagePosAtCrashPos.push_back(HitWho);
                }
            }
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {

                const SMonsterTemplate& kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType];
                if (kMonsterTemplate.bCrashParticlePosToAttackBox)
                {
                    D3DXVECTOR2 vHitPos = pMonster->GetHitPos();
                    fHitX = vHitPos.x;
                    fHitY = vHitPos.y;
                }
                else
                {
                    fHitX = pMonster->m_afX[0];
                    fHitY = pMonster->m_afY[0];
                }

                pDamageInstance->m_emHitWhoType = OBJ_TYPE_MONSTER;

                if (pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos.m_bIsSync &&
                    pMonster->m_iBossMonster != MONSTER::MON_BOSS &&
                    pMonster->m_iBossMonster != MONSTER::MON_MIDBOSS &&
                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].fWalkSpeed != 0.0f &&
                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].eNPC == NPC_NONE &&
                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].iNameID != -1 &&
                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].iMonsterID != -1) {
                    D3DXVECTOR2 vPos = SyncDamagePosAtCrashPos(pDamageInstance, HitWho, false);
                    pMonster->SetX(vPos.x);
                    pMonster->SetY(vPos.y);

                    //m_vecPlayerSyncDamagePosAtCrashPos.clear();
                    //m_vecMonsterSyncDamagePosAtCrashPos.clear();
                    if (pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos.m_bContinue)
                    {
                        std::vector<int>::iterator veciIter = pDamageInstance->m_vecMonsterSyncDamagePosAtCrashPos.begin();
                        for (; veciIter != pDamageInstance->m_vecMonsterSyncDamagePosAtCrashPos.end(); ++veciIter)
                        {
                            if (pMonster->GetMonsterIndex() == (*veciIter))
                                break;
                        }
                        if (veciIter == pDamageInstance->m_vecMonsterSyncDamagePosAtCrashPos.end())
                            pDamageInstance->m_vecMonsterSyncDamagePosAtCrashPos.push_back(pMonster->GetMonsterIndex());
                    }
                }
            }
            // �켱...
            //if ( pDamageInstance->m_What == DT_SKILL_LAS_VENOM_FIND_VICTIM )
            //{
            //ONEHIT��������� �ѹ��� �°� �Ѵ�.
            if (pDamageInstance->CheckAlreadyDamaged(HitWho + MAX_PLAYER_NUM) &&
                (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE))
            {
                return;
            }
            //else
            //{
            //pDamageInstance->m_bAlreadyDamaged[ HitWho + MAX_PLAYER_NUM ] = true;
            //}
            //}
        }
    }


    // ���� DamageToTeleport�� ������ �ǰ��� �̵���Ű��!!
    if (-1 < pDamageInstance->m_pOriginalDamage->m_iDamageToTeleport)
    {
        bool bEndLoop = true;

        // ���Ͱ� �¾Ҵ��� �÷��̾ �¾Ҵ��� Ȯ��
        if (IsCrashPlayer)
        {
            if (g_MyD3D->IsPlayerIndex(HitWho) && g_MyD3D->MyPlayer[HitWho]->IsLive())
                bEndLoop = false;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                if (pMonster->IsLive())
                    bEndLoop = false;
            }
        }

        std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = m_mapDamageInstance.begin();
        for (; (false == bEndLoop) && (mapIter != m_mapDamageInstance.end()); ++mapIter)
        {
            std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
            for (; vecIter != mapIter->second.end(); ++vecIter)
            {
                if (NULL == (*vecIter))
                    continue;

                if (((*vecIter)->m_Who == pDamageInstance->m_Who) &&
                    ((*vecIter)->m_What == pDamageInstance->m_pOriginalDamage->m_iDamageToTeleport) &&
                    (0 < (*vecIter)->m_Life))
                {
                    if (IsCrashPlayer)
                    {
                        g_MyD3D->MyPlayer[HitWho]->vPos.x = (*vecIter)->m_X;
                        g_MyD3D->MyPlayer[HitWho]->vPos.y = (*vecIter)->m_Y;
                    }
                    else
                    {
                        MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
                        if (pMonster != NULL) {
                            if (pDamageInstance->m_pOriginalDamage->m_bIgnoreSpecialMonDamageToTeleport) {
                                if (pMonster->m_iBossMonster != MONSTER::MON_BOSS &&
                                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].fWalkSpeed != 0.0f &&
                                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].eNPC == NPC_NONE &&
                                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].iNameID != -1 &&
                                    g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].iMonsterID != -1) {
                                    pMonster->SetX((*vecIter)->m_X);
                                    pMonster->SetY((*vecIter)->m_Y);
                                }
                            }
                            else {
                                pMonster->SetX((*vecIter)->m_X);
                                pMonster->SetY((*vecIter)->m_Y);
                            }
                        }
                    }
                    bEndLoop = true;
                    break;
                }
            }
        }

    }

    switch (pDamageInstance->m_What)
    {
    case DT_CRITICAL_ARROW:
    case DT_CRITICAL_ARROW_LIGHTNING:
    case DT_CRITICAL_ARROW_SHINING:
    case DT_CRITICAL_ARROW_LOVE:
    case DT_DOWN_ARROW:
    case DT_DOWN_ARROW_LIGHTNING:
    case DT_DOWN_ARROW_SHINING:
    case DT_DOWN_ARROW_LOVE:
    case DT_DOWN_ARROW_DUN:
    case DT_DOWN_ARROW_LIGHTNING_DUN:
    case DT_DOWN_ARROW_SHINING_DUN:
    case DT_DOWN_ARROW_LOVE_DUN:
    case DT_CRITICAL_BOLT:
    case DT_CRITICAL_BOLT_LIGHTNING:
    case DT_CRITICAL_BOLT_SHINING:
    case DT_CRITICAL_BOLT_LOVE:
    case DT_HARPY_DOWN:
    case DT_SWORD_FIRE:
    case DT_CRITICALX_UP:
    case DT_HUGE_CRITICAL_ARROW:
    case DT_HUGE_CRITICAL_ARROW_LIGHTNING:
    case DT_HUGE_CRITICAL_ARROW_SHINING:
    case DT_HUGE_CRITICAL_ARROW_LOVE:
    case DT_HUGE_RUNATK_ARROW:
    case DT_HUGE_RUNATK_ARROW_LIGHTNING:
    case DT_HUGE_RUNATK_ARROW_SHINING:
    case DT_HUGE_RUNATK_ARROW_LOVE:
        // ��� ȭ���..
    case DT_NOVA_CRITICAL_BOLT:
    case DT_NOVA_CRITICAL_ARROW:
    case DT_NOVA_CRITICAL_BOLT_LIGHTNING:
    case DT_NOVA_CRITICAL_ARROW_LIGHTNING:
    case DT_NOVA_CRITICAL_BOLT_SHINING:
    case DT_NOVA_CRITICAL_ARROW_SHINING:
    case DT_NOVA_CRITICAL_BOLT_DARK:
    case DT_NOVA_CRITICAL_ARROW_DARK:
    case DT_NOVA_CRITICAL_BOLT_LOVE:
    case DT_NOVA_CRITICAL_ARROW_LOVE:
    case DT_NOVA_CRITICAL_BOLT_DEATH:
    case DT_NOVA_CRITICAL_ARROW_DEATH:

    case DT_CRITICAL_ARROW_TIME:
    case DT_DOWN_ARROW_TIME:
    case DT_DOWN_ARROW_TIME_DUN:
    case DT_CRITICAL_BOLT_TIME:
    case DT_HUGE_CRITICAL_ARROW_TIME:
    case DT_HUGE_RUNATK_ARROW_TIME:
    case DT_NOVA_CRITICAL_ARROW_TIME:
    case DT_NOVA_CRITICAL_BOLT_TIME:
    case DT_CRITICAL_ARROW_LIFE:
    case DT_DOWN_ARROW_LIFE:
    case DT_DOWN_ARROW_LIFE_DUN:
    case DT_CRITICAL_BOLT_LIFE:
    case DT_HUGE_CRITICAL_ARROW_LIFE:
    case DT_HUGE_RUNATK_ARROW_LIFE:
    case DT_NOVA_CRITICAL_ARROW_LIFE:
    case DT_NOVA_CRITICAL_BOLT_LIFE:

    case DT_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_DOWN_ARROW_DARK_DRAGON_RED_DUN:
    case DT_CRITICAL_BOLT_DARK_DRAGON_RED:
    case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_RED:

    case DT_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE_DUN:
    case DT_CRITICAL_BOLT_DARK_DRAGON_BLUE:
    case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_BLUE:

    case DT_CRITICAL_ARROW_HALLOWEEN:
    case DT_DOWN_ARROW_HALLOWEEN:
    case DT_DOWN_ARROW_HALLOWEEN_DUN:
    case DT_CRITICAL_BOLT_HALLOWEEN:
    case DT_HUGE_CRITICAL_ARROW_HALLOWEEN:
    case DT_HUGE_RUNATK_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_BOLT_HALLOWEEN:

    case DT_CRITICAL_ARROW_C_MASTER:
    case DT_DOWN_ARROW_C_MASTER:
    case DT_DOWN_ARROW_C_MASTER_DUN:
    case DT_CRITICAL_BOLT_C_MASTER:
    case DT_HUGE_CRITICAL_ARROW_C_MASTER:
    case DT_HUGE_RUNATK_ARROW_C_MASTER:
    case DT_NOVA_CRITICAL_ARROW_C_MASTER:
    case DT_NOVA_CRITICAL_BOLT_C_MASTER:

    case DT_CRITICAL_ARROW_COLISEU:
    case DT_DOWN_ARROW_COLISEU:
    case DT_DOWN_ARROW_COLISEU_DUN:
    case DT_CRITICAL_BOLT_COLISEU:
    case DT_HUGE_CRITICAL_ARROW_COLISEU:
    case DT_HUGE_RUNATK_ARROW_COLISEU:
    case DT_NOVA_CRITICAL_ARROW_COLISEU:
    case DT_NOVA_CRITICAL_BOLT_COLISEU:

    case DT_LIRE1_SPECIAL4_DOWN_ARROW:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIGHTNING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_SHINING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LOVE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_TIME:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIFE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_HALLOWEEN:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_ELIA:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_PEGASUS:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_C_MASTER:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_COLISEU:

        if (IsCrashPlayer)
        {
            Add(DT_WAVE1, HitWho);
            Add(DT_WAVE2, HitWho);
        }
        break;

    case DT_CONFUSION:
        AddWithLocate(DT_CONFUSION_SHOCK, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        break;

    case DT_EXPLOSION_ARROW:
    case DT_EXPLOSION_ARROW_LIGHTNING:
    case DT_EXPLOSION_ARROW_SHINING:
    case DT_EXPLOSION_ARROW_LOVE:
        AddWithLocate(DT_HIT_EXPLOSION_ARROW, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        AddWithLocate(DT_HIT_EXPLOSION_ARROW, pDamageInstance->m_Who, pDamageInstance->m_X + 0.15f, pDamageInstance->m_Y - 0.065f);
        AddWithLocate(DT_HIT_EXPLOSION_ARROW, pDamageInstance->m_Who, pDamageInstance->m_X - 0.15f, pDamageInstance->m_Y + 0.085f);
        break;

    case DT_BLOCK_BUSTER:
    {
        // ���� ����Ʈ�� �߻���Ų��.
        g_KDSound.Play("43");
        CDamageInstance* pkDamage = NULL;

        pkDamage = AddWithLocate(DT_BOMB_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
        if (pkDamage)
        {
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
        }

        pkDamage = AddWithLocate(DT_ARCHIMIST_BUSTER_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
        if (pkDamage)
        {
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
        }

        g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f);
        break;
    }
    case DT_DRILLMON_ROCK:
        EffectDust(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, pDamageInstance);
        EffectDust(pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.2f, pDamageInstance);
        EffectDust(pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.2f, pDamageInstance);
        break;

    case DT_FIREBALL_DARK:  //���� ���̾ 
        g_MyD3D->MySparks->DarkFires(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f);
        break;

    case DT_CURE_HEART1: // ���ƴٴϴ� ��Ʈ
    case DT_CURE_HEART1_DUN: // ���ƴٴϴ� ��Ʈ
    case DT_CURE_HEART2: // ġ�� ��Ʈ
    case DT_ARME_SKILL_CURE_LV2_HEART:
    case DT_ARME_SKILL_CURE_LV2_HEART_DUN:
        g_MyD3D->MySparks->Red_Particles(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y, 0, 10 + pDamageInstance->m_Life / 20, 10);
        break;

    case DT_RONAN_SWORD_SPECIAL2_ATK:
    case DT_RONAN2_SWORD_SPECIAL3_ATK1:
    case DT_RONAN2_SWORD_SPECIAL3_ATK2:
    case DT_RONAN2_SWORD_SPECIAL3_ATK3:
    case DT_RONAN2_SWORD_SPECIAL1_ATK1_L:
    case DT_RONAN2_SWORD_SPECIAL1_ATK2_L:
    case DT_RONAN2_SWORD_SPECIAL1_ATK3_L:
    case DT_VANESSA_GOD_ATK3_MISSILE:
    case DT_VANESSA_GOD_ATK5_DMG_MAIN:
    case DT_VANESSA_GOD_ATK5_DMG_SUB_1:
    case DT_VANESSA_GOD_ATK5_DMG_SUB_2:
    case DT_RONAN4_COMMON_JUMP_ATK_B1:
    case DT_RONAN4_COMMON_JUMP_ATK_B2:
    {
        if (g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who))
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_RONAN4_AERIAL_COMBO_A3)
                {
                    pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark01", fHitX - 1.0f, fHitY - 0.32f, 0.5f);
                    pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark02", fHitX - 1.0f, fHitY - 0.32f, 0.5f);
                    pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark03", fHitX - 1.0f, fHitY - 0.32f, 0.5f);
                }
                else
                {
                    pDamageInstance->CreateSequence("Ronan_voit_spark01", fHitX - 1.0f, fHitY - 0.32f, 0.5f);
                    pDamageInstance->CreateSequence("Ronan_voit_spark02", fHitX - 1.0f, fHitY - 0.32f, 0.5f);
                    pDamageInstance->CreateSequence("Ronan_voit_spark03", fHitX - 1.0f, fHitY - 0.32f, 0.5f);
                }
            }
        }
    }
    break;
    case DT_VANESSA_GOD_ATK4_BOOM:
    case DT_VANESSA_GOD_ATK6_EXPLOSION:
    case DT_VANESSA_GOD_ATK6_BALL:
    case DT_VANESSA_GOD_ATK6_READY:
    {
        g_MyD3D->MySparks->Fires(fHitX - 1.0f, fHitY - 0.2f, 0, 0.5f);
    }
    break;
    case DT_RAGNA_BOLT:
    case DT_AMY2_PERFORMENCE_ATK1_NOTE1:
    case DT_AMY2_PERFORMENCE_ATK1_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE1:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK3_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE1:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE2:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE1:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE2:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE1:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK1_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_ATK1_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_ATK3_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE2_DUN:
    case DT_JURIOR_LASER:
    case DT_MARI3_SPECIAL2_ATK:
    case DT_AMY2_SPECIAL4_BIG_NOTE1:
    case DT_AMY2_SPECIAL4_BIG_NOTE2:
    case DT_AMY2_SPECIAL4_BIG_NOTE3:
    case DT_AMY2_SPECIAL4_SMALL_NOTE1:
    case DT_AMY2_SPECIAL4_SMALL_NOTE2:
    case DT_AMY2_SPECIAL4_SMALL_NOTE3:
    case DT_AMY2_SPECIAL4_SMALL_NOTE1_L:
    case DT_AMY2_SPECIAL4_SMALL_NOTE2_L:
    case DT_AMY2_SPECIAL4_SMALL_NOTE3_L:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Ronan_voit_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Ronan_voit_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Ronan_voit_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        }
    }
    break;
    case DT_AMY2_PERFORMENCE_DASH_ATK_NOTE1:
    case DT_AMY2_PERFORMENCE_DASH_ATK_NOTE2:
    case DT_AMY2_PERFORMENCE_DASH_ATK_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_DASH_ATK_NOTE2_DUN:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            if (DT_AMY2_PERFORMENCE_DASH_ATK_NOTE1)
                pDamageInstance->CreateSequence("Amy02_Backattack_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            else
                pDamageInstance->CreateSequence("Amy02_Backattack_spark01_2", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Amy02_Backattack_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Amy02_Backattack_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Amy02_Backattack_spark04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        }

        for (int i = 0; i < MAX_PLAYER_NUM; ++i)
        {
            if (g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i) == false) //����� MP�� ���� ��Ų��.
            {
                if (g_MyD3D->MyPlayer[i]->IsLocalPlayer() && i == HitWho)
                {
                    g_MyD3D->MyPlayer[i]->DecreaseMP(-0.3f);
                }
                if (i == HitWho && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON
                    && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_CRUSADER
                    && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_HUNT)
                {
                    g_MyD3D->MyHead->Add_HP_Changer(pDamageInstance->m_Who, i, 3, true, "+MP");
                }
            }
        }

    }
    break;
    // �γ� ���˼�3���� �ö���� Į ������
    case DT_RONAN_SWORD_SPECIAL3_SWORD:
    case DT_RONAN_SWORD_SPECIAL3_SWORD_DUN:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        }
    }
    break;

    case DT_SHINING_ARROW:
        CallAnim(pDamageInstance, "HitShiningArrow", 0.004f, 0.004f);
        break;

    case DT_MOTAL_BLOW:
        CallAnim(pDamageInstance, "MotalBlow", 0.006f, 0.006f);
        break;

    case DT_BLOOD_BASH:
        CallAnim(pDamageInstance, "BloodBash", 0.006f, 0.006f);
        break;

    case DT_ARME3_FIRESPOT:
        // ���� ����Ʈ�� �߻���Ų��.
        g_KDSound.Play("43");
        pDamageInstance->m_Life = 1;
        AddWithLocate(DT_ARME3_FIRECRASH1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.1f);
        AddWithLocate(DT_ARME3_FIRECRASH2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.1f);
        AddWithLocate(DT_ARME3_BURSTFIRE3, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.02f);
        AddWithLocate(DT_ARME3_BURSTFIRE2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.02f);
        AddWithLocate(DT_ARME3_BURSTFIRE1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.02f);
        break;

    case DT_LAS3_SPECIAL2_ATK1:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            CParticleEventSeqPTR pTemp = pDamageInstance->CreateSequence("2ndlas_special2_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pTemp->SetDirectInfo(&stDirect);
        }
    }
    break;
    case DT_LAS3_SPECIAL2_ATK2:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            CParticleEventSeqPTR pTemp;
            pTemp = pDamageInstance->CreateSequence("2ndlas_special2_08", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pTemp->SetDirectInfo(&stDirect);

            pTemp = pDamageInstance->CreateSequence("2ndlas_special2_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pTemp->SetDirectInfo(&stDirect);
        }
    }
    break;
    case DT_LAS3_SPECIAL2_ATK3:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            CParticleEventSeqPTR pTemp = pDamageInstance->CreateSequence("2ndlas_special2_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pTemp->SetDirectInfo(&stDirect);
        }
    }
    break;
    case DT_LAS3_SPECIAL2_ATK4:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            CParticleEventSeqPTR pTemp = pDamageInstance->CreateSequence("2ndlas_special2_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pTemp->SetDirectInfo(&stDirect);
        }
    }
    break;

    case DT_LAS3_SPECIAL3_ATK1:
    case DT_LAS3_SPECIAL3_ATK3:
    case DT_LAS3_SPECIAL3_ATK5:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            if (pDamageInstance->m_IsRight == true)
            {
                pDamageInstance->CreateSequence("2ndlas_special3_06", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_01", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_02", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_03", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_00", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
            else
            {
                pDamageInstance->CreateSequence("2ndlas_special3_06", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_01", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_02", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_03", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_00", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
        }
    }
    break;
    case DT_LAS3_SPECIAL3_BACK_ATK1:
    case DT_LAS3_SPECIAL3_BACK_ATK3:
    case DT_LAS3_SPECIAL3_BACK_ATK5:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            if (pDamageInstance->m_IsRight == true)
            {
                pDamageInstance->CreateSequence("2ndlas_special3_06", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_01", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_02", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_03", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_00", pDamageInstance->m_X - 1.1f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
            else
            {
                pDamageInstance->CreateSequence("2ndlas_special3_06", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_01", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_02", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_03", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("2ndlas_special3_00", pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
        }
    }
    break;
    case DT_ELESIS4_SPECIAL3_SHOOT:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            CParticleEventSeqPTR pTemp = pDamageInstance->CreateSequence("ELESIS4_SPECIAL3_hit_effect01", pDamageInstance->m_X - 1.0f,
                pDamageInstance->m_Y - 0.5f, 0.5f);
            pTemp->SetDirectInfo(&stDirect);
            pTemp = pDamageInstance->CreateSequence("ELESIS4_SPECIAL3_hit_effect02", pDamageInstance->m_X - 1.0f,
                pDamageInstance->m_Y - 0.5f, 0.5f);
            pTemp->SetDirectInfo(&stDirect);
        }
    }
    break;
    // 2007.10.24 wony
    // �� ���� ���� �� ���ǿ� �������� �ƽ��ƽ��� Ÿ�ֿ̹� �����ϸ�..
    // PlayerTemplate�� ������ NoCheckContact������ �Ʒ��� ��������..
    // ���� ���ݿ� �����ߴٸ� �׳� �� ���ǿ� �ɸ����� ����
    case DT_PET_MASHIMARO_THROW_PLAYER:
        if (g_MyD3D->IsPlayerIndex(HitWho))
            g_MyD3D->MyPlayer[HitWho]->AddDamage(DT_PET_MASHIMARO_PLUNGER_PLAYER, 0);
        break;
    case DT_PET_SMALL_MAID_THROW_PLAYER:
    case DT_PET_MAID_THROW_PLAYER:
    case DT_PET_CHRISTMAS_MAID_THROW_PLAYER:
        if (g_MyD3D->IsPlayerIndex(HitWho))
            g_MyD3D->MyPlayer[HitWho]->AddDamage(DT_PET_SMALL_MAID_PLAYER, 0);
        break;
    case DT_LAS4_JUMP_ATK:
        if (g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who))
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetNoCheckContact(0);
        break;
    case DT_BOMBMAN_01:
        // ���� ����Ʈ�� �߻���Ų��.
        //g_KDSound.Play( "43" );
        AddWithLocate(DT_BOMBMAN_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true, pDamageInstance->m_ExtraMonsterNum);
        pDamageInstance->m_Life = 1;
        g_KDSound.Play("43");
        break;
    case DT_CUPID_ARROW:
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Cupid_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Cupid_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Cupid_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Cupid_spark04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        }

        pDamageInstance->m_Life = 1;
        g_KDSound.Play("666");
        break;
    case DT_RONAN3_GUARD_CANCEL_ATK:
    case DT_RONAN3_BLOCK_CANCEL_ATK:
    case DT_RIN4_FORM_DEFFENCE_PLUS_DELAY:
    case DT_RIN4_FORM_DEFFENCE_REFLECT_DAMAGE:
    case DT_ELESIS_BLOCK_CANCEL_ATK:
    {
        bool bSameTeam = true;
        if (IsCrashPlayer == true)
        {
            if (g_MyD3D->IsSameTeam(pDamageInstance->m_Who, HitWho))
                bSameTeam = true;
            else
                bSameTeam = false;
        }
        else
        {
            bSameTeam = false;
        }

        if (bSameTeam == false && pDamageInstance->m_Life >= 8)
        {
            // �γ��� ���� �ߴ�.. 
            if ((g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who)) && pDamageInstance->m_What == DT_RONAN3_GUARD_CANCEL_ATK)
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->AddDamage(DT_GUARD, 0);
            }

            int iRandNum = (rand() % 2) + 753;
            std::stringstream str;
            str << iRandNum;
            g_KDSound.Play(str.str().c_str());
        }
        break;
    }
    case DT_SKILL_ELESIS1_NEWSPECIAL2_A_ATK2:
    case DT_SKILL_ELESIS1_NEWSPECIAL2_A_ATK1_R:
    case DT_SKILL_ELESIS1_SPECIAL2_LV1:
    case DT_SKILL_ELESIS1_NEWSPECIAL2_LV3_CHARGE:
    case DT_SKILL_ELESIS1_SPECIAL2_LV2_KICKEFFECT:
    case DT_SKILL_ELESIS1_MEGASLASH_LV3:
    case DT_ELESIS_NEWSPECIAL3_A:
    case DT_SKILL_ELESIS1_SPECIAL1_LV1_R:
    case DT_SKILL_ELESIS1_SPECIAL1_LV1_L:
    case DT_SKILL_ELESIS1_SPECIAL1_LV2_KUNG:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("ELESIS1_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
        }
        break;
    }
    case DT_SKILL_ELESIS1_NEWSPECIAL2_LV3_KUNG:
    case DT_SKILL_ELESIS1_SPECIAL3_LV1_BOOM:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
        }
        break;
    }
    case DT_RONAN4_WHITE_MAGIC_SPECIAL1_DMG3:
    case DT_RONAN4_WHITE_MAGIC_SPECIAL1_DMG4:
    case DT_RONAN4_WHITE_MAGIC_SPECIAL1_DMG5:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Ronan_voit_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("Ronan_voit_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            pDamageInstance->CreateSequence("Ronan_voit_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
        }
        break;
    }
    case DT_RONAN4_MONOVOLT_ATK2_1:
    case DT_RONAN4_MONOVOLT_ATK2_2:
    case DT_RONAN4_MONOVOLT_ATK2_3:
    {
        AddWithLocate(DT_RONAN4_MONOVOLT_ATK2_BEFORE_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
        break;
    }
    case DT_ARME_SKILL_BLOODPEST_TRACE_OBJECT:
    {
        pDamageInstance->m_Life = 0;
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
            {
                pDamageInstance->m_Life = 0;
                break;
            }

            if (!g_MyD3D->MyPlayer[HitWho]->IsInvinsible(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack))
            {
                g_MyD3D->m_pDamageManager->AddWithLocate(DT_ARME_SKILL_BLOODPEST_CATCHED_MP, pDamageInstance->m_Who,
                    g_MyD3D->MyPlayer[HitWho]->GetPosition().x,
                    g_MyD3D->MyPlayer[HitWho]->GetPosition().y + 0.1f, false, -1, NULL, HitWho,
                    NULL, OBJ_TYPE_PLAYER, OBJ_TYPE_PLAYER);
            }
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                if (!pMonster->IsInvincible())
                {
                    pDamageInstance->m_Life = 0;
                    g_MyD3D->m_pDamageManager->AddWithLocate(DT_ARME_SKILL_BLOODPEST_CATCHED_MP, pDamageInstance->m_Who,
                        pMonster->GetX(),
                        pMonster->GetY() + 0.1f, false, -1, NULL, HitWho,
                        NULL, OBJ_TYPE_PLAYER, OBJ_TYPE_MONSTER);
                }
            }
            else {
                pDamageInstance->m_Life = 0;
                break;
            }

        }
        g_KDSound.Play("992");
        break;
    }
    case DT_ARME_SKILL_SHOCKSTURN_LV1_ATK:
    {
        AddWithLocate(DT_ARME_SKILL_SHOCKSTURN_LV1_LIGHTING, pDamageInstance->m_Who, fHitX - 0.17f, pDamageInstance->m_Y);
        AddWithLocate(DT_ARME_SKILL_SHOCKSTURN_LV1_LIGHTING, pDamageInstance->m_Who, fHitX - 0.02f, pDamageInstance->m_Y);
        AddWithLocate(DT_ARME_SKILL_SHOCKSTURN_LV1_LIGHTING, pDamageInstance->m_Who, fHitX + 0.13f, pDamageInstance->m_Y);

        float xPos = 0.0f;
        if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
            xPos = 0.9f;
        else
            xPos = 1.1f;

        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_01", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_02", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_03", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_04", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_05", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_06", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_07", fHitX - xPos, fHitY - 0.3f, 0.5f);
        }
        g_KDSound.Play("1008");
    }
    break;
    case DT_ARME_SKILL_SHOCKSTURN_LV2_ATK1:
    {
        bool bIsCount = false;

        if (SiKGCRoomManager()->IsMonsterGameMode())
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                if (pMonster->m_iShockSturnCount == 0 || pMonster->m_iShockSturnCount == 3 ||
                    pMonster->m_iShockSturnCount == 7)
                {
                    bIsCount = true;
                }

                pMonster->m_iShockSturnCount++;

                if (pMonster->m_iShockSturnCount >= 7)
                    pMonster->m_iShockSturnCount = 0;
            }

        }
        else
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;

            if (g_MyD3D->MyPlayer[HitWho]->m_iShockSturnCount == 0 || g_MyD3D->MyPlayer[HitWho]->m_iShockSturnCount == 3 ||
                g_MyD3D->MyPlayer[HitWho]->m_iShockSturnCount == 7)
            {
                bIsCount = true;
            }
            g_MyD3D->MyPlayer[HitWho]->m_iShockSturnCount++;

            if (g_MyD3D->MyPlayer[HitWho]->m_iShockSturnCount >= 7)
                g_MyD3D->MyPlayer[HitWho]->m_iShockSturnCount = 0;
        }

        if (bIsCount)
        {
            AddWithLocate(DT_ARME_SKILL_SHOCKSTURN_LV2_LIGHTING, pDamageInstance->m_Who, fHitX - 0.17f, pDamageInstance->m_Y);
            AddWithLocate(DT_ARME_SKILL_SHOCKSTURN_LV2_LIGHTING, pDamageInstance->m_Who, fHitX - 0.02f, pDamageInstance->m_Y);
            AddWithLocate(DT_ARME_SKILL_SHOCKSTURN_LV2_LIGHTING, pDamageInstance->m_Who, fHitX + 0.13f, pDamageInstance->m_Y);

            float xPos = 0.0f;
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
                xPos = 0.9f;
            else
                xPos = 1.1f;

            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_01", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_02", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_03", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_04", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_05", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_06", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Shock_Stern_LV1_07", fHitX - xPos, fHitY - 0.3f, 0.5f);
            }
            g_KDSound.Play("1008");
        }
    }
    break;
    case DT_ARME_SKILL_REVERSEGRAVITY_LV2_RIGHT:
    case DT_ARME_SKILL_REVERSEGRAVITY_LV2_LEFT:
    {
        if (!g_MyD3D->IsPlayerIndex(HitWho))
            break;
        if (!g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who))
            break;

        D3DXVECTOR3 vecPlayerPos = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition();
        D3DXVECTOR3 vecHitWhoPos = g_MyD3D->MyPlayer[HitWho]->GetPosition();

        if (vecPlayerPos.x > vecHitWhoPos.x)
        {
            if (vecPlayerPos.x - 0.2 < vecHitWhoPos.x)
                g_MyD3D->MyPlayer[HitWho]->x_Speed = 0;
        }
        else
        {
            if (vecPlayerPos.x + 0.2 > vecHitWhoPos.x)
                g_MyD3D->MyPlayer[HitWho]->x_Speed = 0;
        }
    }
    break;
    //case DT_ARME_SKILL_DEATHCLOUD_OBJECT:	
    //	{
    //		AddWithLocate( DT_ARME_SKILL_DEATHCLOUD_BUFF, pDamageInstance->m_Who , fHitX, fHitY + 0.1f);
    //		g_KDSound.Play( "1001" );		
    //	}
    //	break;
    //case DT_ARME_SKILL_DEATHCLOUD_BUFF:
    //	{
    //		if( IsCrashPlayer )
    //		{
    //			if( !g_MyD3D->IsPlayerIndex( HitWho ) )
    //				break;
    //		}
    //		else
    //		{
    //			if( !g_kMonsterManager.IsMonsterIndex( HitWho ) )
    //				break;
    //		}

    //		g_pMagicEffect->ClearMagicEffect( HitWho, EGC_EFFECT_SKILL_ARME_DEATHCLOUD_BUFF);
    //		g_pMagicEffect->SetMagicEffect( HitWho, EGC_EFFECT_SKILL_ARME_DEATHCLOUD_BUFF, 5.0f, 1, pDamageInstance->m_Who, IsCrashPlayer );						
    //	}
    break;
    case DT_ARME_SKILL_FEAR_LV1_ATK1:
    case DT_ARME_SKILL_FEAR_LV1_ATK2:
    case DT_ARME_SKILL_FEAR_LV1_ATK3:
    case DT_ARME_SKILL_FEAR_LV1_ATK1_REAR:
    case DT_ARME_SKILL_FEAR_LV1_ATK2_REAR:
    case DT_ARME_SKILL_FEAR_LV1_ATK3_REAR:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_EFFECT_SKILL_FEAR_LV1);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_EFFECT_SKILL_FEAR_LV1, 12.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);
        float xPos = 0.0f, yPos = 0.0f;
        if (IsCrashPlayer)
        {
            xPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.x;
            yPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.y;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                xPos = pMonster->vRenderPos.x;
                yPos = pMonster->vRenderPos.y + pMonster->m_fAttackBoundBoxHeight * 0.6f;
            }
        }
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_01", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_02", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_03", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_04", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_05", xPos, yPos, 0.5f);
        }
        g_KDSound.Play("994");
        g_KDSound.Play("995");
    }
    break;
    case DT_ARME_SKILL_FEAR_LV2_ATK1:
    case DT_ARME_SKILL_FEAR_LV2_ATK2:
    case DT_ARME_SKILL_FEAR_LV2_ATK3:
    case DT_ARME_SKILL_FEAR_LV2_ATK1_REAR:
    case DT_ARME_SKILL_FEAR_LV2_ATK2_REAR:
    case DT_ARME_SKILL_FEAR_LV2_ATK3_REAR:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_EFFECT_SKILL_FEAR_LV2);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_EFFECT_SKILL_FEAR_LV2, 12.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);
        float xPos = 0.0f, yPos = 0.0f;
        if (IsCrashPlayer)
        {
            xPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.x;
            yPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.y;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                xPos = pMonster->vRenderPos.x;
                yPos = pMonster->vRenderPos.y + pMonster->m_fAttackBoundBoxHeight * 0.6f;
            }
        }
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_01", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_02", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_03", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_04", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_05", xPos, yPos, 0.5f);
        }
        g_KDSound.Play("994");
        g_KDSound.Play("995");
    }
    break;

    case DT_ARME_SKILL_FEAR_LV1_ATK1_DUN:
    case DT_ARME_SKILL_FEAR_LV1_ATK2_DUN:
    case DT_ARME_SKILL_FEAR_LV1_ATK3_DUN:
    case DT_ARME_SKILL_FEAR_LV1_ATK1_REAR_DUN:
    case DT_ARME_SKILL_FEAR_LV1_ATK2_REAR_DUN:
    case DT_ARME_SKILL_FEAR_LV1_ATK3_REAR_DUN:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_EFFECT_SKILL_FEAR_LV1);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_EFFECT_SKILL_FEAR_LV1, 12.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);
        float xPos = 0.0f, yPos = 0.0f;
        if (IsCrashPlayer)
        {
            xPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.x;
            yPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.y;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                xPos = pMonster->vRenderPos.x;
                yPos = pMonster->vRenderPos.y + pMonster->m_fAttackBoundBoxHeight * 0.6f;
            }
        }
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_01", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_02", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_03", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_04", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV1_head_05", xPos, yPos, 0.5f);
        }
        g_KDSound.Play("994");
        g_KDSound.Play("995");
    }
    break;
    case DT_ARME_SKILL_FEAR_LV2_ATK1_DUN:
    case DT_ARME_SKILL_FEAR_LV2_ATK2_DUN:
    case DT_ARME_SKILL_FEAR_LV2_ATK3_DUN:
    case DT_ARME_SKILL_FEAR_LV2_ATK1_REAR_DUN:
    case DT_ARME_SKILL_FEAR_LV2_ATK2_REAR_DUN:
    case DT_ARME_SKILL_FEAR_LV2_ATK3_REAR_DUN:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_EFFECT_MON_DEFDOWN_LV1);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_EFFECT_MON_DEFDOWN_LV1, 12.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);
        float xPos = 0.0f, yPos = 0.0f;
        if (IsCrashPlayer)
        {
            xPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.x;
            yPos = g_MyD3D->MyPlayer[HitWho]->m_posDesc.body.y;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                xPos = pMonster->vRenderPos.x;
                yPos = pMonster->vRenderPos.y + pMonster->m_fAttackBoundBoxHeight * 0.6f;
            }
        }
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_01", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_02", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_03", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_04", xPos, yPos, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Fear_LV2_head_05", xPos, yPos, 0.5f);
        }
        g_KDSound.Play("994");
        g_KDSound.Play("995");
    }
    break;

    case DT_ARME_SKILL_FROSTRING_LV1_ATK1:
    case DT_ARME_SKILL_FROSTRING_LV1_ATK2:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_EFFECT_SKILL_ARME_FROSTRING_LV1);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_EFFECT_SKILL_ARME_FROSTRING_LV1, 15.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);
        AddWithLocate(DT_ARME_SKILL_FROSTRING_LV1_CRASH_EFFECT, pDamageInstance->m_Who, fHitX + 1.0f, fHitY + 0.5f, false, -1, NULL, HitWho);
    }
    break;
    case DT_ARME_SKILL_FROSTRING_LV2_ATK1:
    case DT_ARME_SKILL_FROSTRING_LV2_ATK2:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_EFFECT_SKILL_ARME_FROSTRING_LV2);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_EFFECT_SKILL_ARME_FROSTRING_LV2, 17.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);
        AddWithLocate(DT_ARME_SKILL_FROSTRING_LV2_CRASH_EFFECT, pDamageInstance->m_Who, fHitX + 1.0f, fHitY + 0.5f, false, -1, NULL, HitWho);
    }
    break;
    case DT_ARME_SKILL_SAINTBREEDING_FIRE:
    case DT_ARME_SKILL_SAINTBREEDING_GHOST:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_EFFECT_SKILL_ARME_SAINTBREEDING);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_EFFECT_SKILL_ARME_SAINTBREEDING, 7.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);


        if (SiKGCRoomManager()->IsMonsterGameMode())
        {
            MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.find(HitWho);
            if (mit != g_kMonsterManager.m_mapMonster.end()) {
                m_mapMonsterIndex[mit->first] = HitWho;
            }
        }
        else
        {
            for (int i = 0; i < MAX_PLAYER_NUM; i++)
            {
                if (i == HitWho)
                    m_iPlayerIndex[i] = HitWho;
            }
        }

        if (IsCrashPlayer)
        {
            AddWithLocate(DT_ARME_SKILL_SAINTBREEDING_SPACE, pDamageInstance->m_Who, fHitX + 1.0f,
                fHitY + 0.5f, false, -1, NULL, HitWho, NULL, OBJ_TYPE_PLAYER, OBJ_TYPE_PLAYER);
        }
        else
        {
            AddWithLocate(DT_ARME_SKILL_SAINTBREEDING_SPACE, pDamageInstance->m_Who, fHitX + 1.0f,
                fHitY + 0.5f, false, -1, NULL, HitWho, NULL, OBJ_TYPE_PLAYER, OBJ_TYPE_MONSTER);
        }

        float xPos = 0.0f;
        if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
            xPos = 0.9f;
        else
            xPos = 1.1f;
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Arme01_Hell_Breath_Spark_01", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Hell_Breath_Spark_02", fHitX - xPos, fHitY - 0.3f, 0.5f);
            pDamageInstance->CreateSequence("Arme01_Hell_Breath_Spark_03", fHitX - xPos, fHitY - 0.3f, 0.5f);
        }
    }
    break;
    case DT_ARME_SKILL_SAINTBREEDING_SPACE:
    {
        if (SiKGCRoomManager()->IsMonsterGameMode())
        {
            for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
                if (mit->first != pDamageInstance->m_HitWho) {
                    m_mapMonsterIndex[mit->first] = HitWho;
                }
                else
                    continue;

                if (m_mapMonsterIndex[mit->first] != -1 && m_mapMonsterIndex[mit->first] == HitWho && m_mapMonsterIndex[mit->first] != pDamageInstance->m_HitWho) {
                    pDamageInstance->m_Life = 1;
                    g_pMagicEffect->ClearMagicEffect(pDamageInstance->m_HitWho, EGC_EFFECT_SKILL_ARME_SAINTBREEDING);
                }
            }

        }
        else
        {
            for (int i = 0; i < MAX_PLAYER_NUM; i++)
            {
                if (i != pDamageInstance->m_HitWho)
                    m_iPlayerIndex[i] = HitWho;

                if (m_iPlayerIndex[i] != -1 && m_iPlayerIndex[i] == HitWho && m_iPlayerIndex[i] != pDamageInstance->m_HitWho)
                {
                    pDamageInstance->m_Life = 1;
                    g_pMagicEffect->ClearMagicEffect(pDamageInstance->m_HitWho, EGC_EFFECT_SKILL_ARME_SAINTBREEDING);
                }
            }
        }
    }
    break;
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET1:
    case DT_AMY3_DANCE_UNIQUE_SKILL_PET1:
    {
        int randomNum = rand() % 2;

        if (randomNum == 0)
        {
            AddWithLocate(DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET1_1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, -1, NULL);
        }
        else
        {
            AddWithLocate(DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET1_2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, -1, NULL);
        }
        break;
    }
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET2:
    case DT_AMY3_DANCE_UNIQUE_SKILL_PET2:
    {
        int randomNum = rand() % 2;

        if (randomNum == 0)
        {
            AddWithLocate(DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET2_1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, -1, NULL);
        }
        else
        {
            AddWithLocate(DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET2_2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, -1, NULL);
        }
        break;
    }
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET3:
    case DT_AMY3_DANCE_UNIQUE_SKILL_PET3:
    {
        int randomNum = rand() % 2;

        if (randomNum == 0)
        {
            AddWithLocate(DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET3_1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, -1, NULL);
        }
        else
        {
            AddWithLocate(DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET3_2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, -1, NULL);
        }
        break;
    }
    case DT_AMY3_DANCE_SPECIAL1_ATK:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            float xPos = 0.0f;
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
            {
                xPos = 0.9f;
                pDamageInstance->CreateSequence("Amy03_special2-1_Emoticon01", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Amy03_special2-1_Emoticon02", fHitX - xPos, fHitY - 0.3f, 0.5f);
                //pDamageInstance->CreateSequence( "Amy03_special2-1_Emoticon03", fHitX- xPos, fHitY	- 0.3f, 0.5f );
            }
            else
            {
                xPos = 1.1f;
                pDamageInstance->CreateSequence("Amy03_special2-1_Emoticon01_R", fHitX - xPos, fHitY - 0.3f, 0.5f);
                pDamageInstance->CreateSequence("Amy03_special2-1_Emoticon02_R", fHitX - xPos, fHitY - 0.3f, 0.5f);
                //pDamageInstance->CreateSequence( "Amy03_special2-1_Emoticon03", fHitX- xPos, fHitY	- 0.3f, 0.5f );
            }
        }
        //g_KDSound.Play("1101");
        break;
    }
    case DT_JIN_UNIQUE_HANDWIND_ATK1:
    case DT_JIN_UNIQUE_HANDWIND_ATK1_R:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Jin_Hadoken_Spark_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Jin_Hadoken_Spark_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Jin_Hadoken_Spark_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Jin_Hadoken_Spark_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        }
    }
    break;
    case DT_HEADSHOT_ARROW:
    case DT_HEADSHOT_ARROW_LV2:
    {
        //====================================================================================
        // 2009.02.09 : Jemitgge
        // Comment : ��弦�� ���� ���̰� �ڽ��� �Ǹ� ��´�.. 60%�� Ȯ���̴�.
        //			 ���� ����� ��°� �ֻ��� ��� ���� ��ũ ����

        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;

            if (HitWho == g_MyD3D->Get_MyPlayer())
            {
                int iRate = 3;
                if (GC_GLOBAL_DEFINE::bMatchBalance2014) {
                    iRate = 6;
                }
                if (rand() % iRate == 0)
                {
                    g_MyD3D->MyPlayer[HitWho]->SetHP(0.f);
                    // ����Ʈ ����~ 

                    if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                    {
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_01", fHitX - 1.0f, fHitY - 0.33f, 0.5f);
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_02", fHitX - 1.0f, fHitY - 0.33f, 0.5f);
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_03", fHitX - 1.0f, fHitY - 0.33f, 0.5f);
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_04", fHitX - 1.0f, fHitY - 0.33f, 0.5f);
                    }

                    g_KDSound.Play("1305");
                }
            }
        }
        else
        {
            if (pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer())
            {
                MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
                if (pMonster != NULL) {
                    fHitX = pMonster->vRenderPos.x;
                    fHitY = pMonster->vRenderPos.y + pMonster->m_fAttackBoundBoxHeight * 0.6f;
                    if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                    {
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_01", fHitX, fHitY, 0.5f);
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_02", fHitX, fHitY, 0.5f);
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_03", fHitX, fHitY, 0.5f);
                        pDamageInstance->CreateSequence("Lire_Headshot_Arrow_Effect_04", fHitX, fHitY, 0.5f);
                    }

                    g_KDSound.Play("1305");
                }
            }
        }
    }
    break;
    case DT_DAMAGE_AMPLIFIER_LV1:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_SKILLEFFECT_DAMAGE_AMPLIFIER_LV1);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_SKILLEFFECT_DAMAGE_AMPLIFIER_LV1, 7.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);

        g_KDSound.Play("1334");
    }
    break;
    case DT_POISON_ARROW:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_SKILLEFFECT_POISONARROW_DOTDAMAGE);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_SKILLEFFECT_POISONARROW_DOTDAMAGE, 7.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);

        g_KDSound.Play("1334");
    }
    break;
    case DT_SKILL_FROZEN:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;

            g_MyD3D->MyPlayer[HitWho]->SetPlayerState(MID_COMMON_FREEZED_BY_PIXIE);
            g_MyD3D->MyPlayer[HitWho]->cFrame = 0;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
                pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
                pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
                pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
                    10.0f, ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade);
            }
        }
    }
    break;
    case DT_PET_BAKUNAWA_BOOM:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;

            g_MyD3D->MyPlayer[HitWho]->SetPlayerState(MID_COMMON_FREEZED_BY_PIXIE);
            g_MyD3D->MyPlayer[HitWho]->cFrame = 0;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
                pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
                pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
                pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
                    10.0f, ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, false, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade);
            }
        }
    }
    break;
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN:
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN_LV1:
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN_LV2:
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN_LV3:
    case DT_SUMMON_GARGOYLE_ICEBREATH:
    case DT_MARI4_SPECIAL3_ICE:
    case DT_MARI4_SPECIAL3_ICE_EVENT_MON:
    case DT_ARME3_SPECIAL4_ICE_DMG:
    case DT_MARI4_SPECIAL4_LASER_ICE_ATK:
    case DT_MARI4_SPECIAL4_ICE_BOOM:
    case DT_COWNAT_MON_DARKSPELL_ATTACK02_LEFT:
    case DT_COWNAT_MON_DARKSPELL_ATTACK02_RIGHT:
    case DT_EDEL1_UPGRADE_SPECIAL1_TYPE2_ATK4:
    case DT_MON_JONJALLE_ATTACK03_FRAME_BLUE:
    case DT_EDEL2_SPECIAL_A_1_ICE_ATK:
    case DT_EDEL2_SPECIAL_A_2_ICE_ATK:
    {
        if (IsCrashPlayer)
        {
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
                pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
                pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
                pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
                    0.0f, ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade, true, false, false, false, 0, false, pDamageInstance->m_ExtraMonsterNum);
            }
        }
    }
    break;
    case DT_FIRE_ARROW:
    case DT_FIRE_ARROW_EX:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;
        }
        else
        {
            if (!g_kMonsterManager.IsMonsterIndex(HitWho))
                break;
        }

        g_pMagicEffect->ClearMagicEffect(HitWho, EGC_SKILLEFFECT_FIREARROW_DOTDAMAGE);
        g_pMagicEffect->SetMagicEffect(HitWho, EGC_SKILLEFFECT_FIREARROW_DOTDAMAGE, 7.0f, 1, pDamageInstance->m_Who, IsCrashPlayer);

        g_KDSound.Play("1334");
    }
    break;
    case DT_LEY1_FLAME_ROLL_FIRE:
    case DT_RONAN_MAGIC_SPECIAL2_FIRE:
    case DT_RONAN_MAGIC_SPECIAL2_FIRE_LV2:
    case DT_RONAN_MAGIC_SPECIAL2_FIRE_LV3:
    case DT_ARME4_SPECIAL4_HUGE_METEO_FIRE:
    case DT_ARME4_SPECIAL4_HUGE_METEO_FIRE_EVENT_MON:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;

            if (g_MyD3D->MyPlayer[HitWho]->Slow_Count > 0)
                break;
        }
    }
    break;
    case DT_DARKSHOT_BLACKHOLE_LEFT:
    case DT_DARKSHOT_BLACKHOLE_RIGHT:
    {
        if (o5 == 0 && g_MyD3D->Get_MyPlayer() == HitWho)
        {
            int iLeftRightEffect = DT_JOYSTICK_LEFT_HIGH + ((int)o5 % 3);
            g_MyD3D->MyPlayer[HitWho]->AddDamage(static_cast<unsigned short>(iLeftRightEffect), 0, true);
        }
    }
    break;
    case DT_MONSTER_NESSI_BREATH:
    case DT_MONSTER_NESSI_BREATH_BACK:
    case DT_MONSTER_SWAMP_GOLEM_BREATH:
        g_MyD3D->MySparks->IceFires(fHitX - 1.0f, fHitY, 0, 0.5f);
        break;
    case DT_SKILL_LAS_SPIKE_TRAP:
    {
        g_KDSound.Play("1486");
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            if (GC_GLOBAL_DEFINE::bMatchBalance2014) {
                pDamageInstance->CreateSequence("Las01_Spike_Trap_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.4f, 0.5f);
                pDamageInstance->CreateSequence("Las01_Spike_Trap_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.4f, 0.5f);
                pDamageInstance->CreateSequence("Las01_Spike_Trap_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.4f, 0.5f);
            }
            else {
                pDamageInstance->CreateSequence("Las01_Spike_Trap_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.37f, 0.5f);
                pDamageInstance->CreateSequence("Las01_Spike_Trap_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.37f, 0.5f);
                pDamageInstance->CreateSequence("Las01_Spike_Trap_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.37f, 0.5f);
            }
        }
    }
    break;
    case DT_SKILL_LAS_CANON_TRAP:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Las01_Canon_Trap_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Las01_Canon_Trap_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Las01_Canon_Trap_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Las01_Canon_Trap_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        }
    }
    break;
    case DT_SKILL_LAS_STUN_SURIKEN:
    case DT_SKILL_LAS_STUN_SURIKEN_1:
    {
        //====================================================================================
        // 2009.03.21 : Jemitgge
        // Comment : ���� ������ ������ �����ð��� �ִ�! �� 
        if (pDamageInstance->m_What == DT_SKILL_LAS_STUN_SURIKEN
            || pDamageInstance->m_What == DT_SKILL_LAS_STUN_SURIKEN_1)
        {
            if (IsCrashPlayer)
            {
#if !defined( MATCH_BALANCE_2011_8 )
                if (g_MyD3D->MyPlayer[HitWho]->Shield <= 0 || g_MyD3D->MyPlayer[HitWho]->IsInvinsible(pDamageInstance->m_pOriginalDamage->m_bSpecialAttack))
                    g_MyD3D->MyPlayer[HitWho]->Slow_Count = 20;
#endif
                g_MyD3D->m_pDamageManager->Add(DT_STUN, HitWho);
            }
            else
            {
                MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
                if (pMonster != NULL) {
#if !defined( MATCH_BALANCE_2011_8 )
                    pMonster->m_iSlowCount = 35;
#endif
                    g_MyD3D->m_pDamageManager->MonsterAdd(DT_STUN, HitWho);
                }
            }
        }
    }
    break;
    case DT_SKILL_RYAN_PHOENIX_1:
    {
        if (IsCrashPlayer)
        {
            if (false == g_MyD3D->IsSameTeam(pDamageInstance->m_Who, HitWho))
                g_MyD3D->MyPlayer[HitWho]->Slow_Count += 20;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                if (g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, HitWho))
                    pMonster->m_iSlowCount += 20;
            }

        }
    }
    break;
    case DT_AMY_STIGMA_CIRCLE:
    case DT_AMY_STIGMA_CIRCLE_LV2:
    {
        if (!IsCrashPlayer)
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                pMonster->m_fHP_Recover = 0.0f;
            }
        }

    }
    break;
    case DT_AMY_PHOTO_SHOT:
    case DT_AMY_PHOTO_SHOT_R:
        if (IsCrashPlayer)
        {
            pDamageInstance->m_Life = 2;
        }
        break;
    case DT_MARI2_BUILDING_LV1_JUMP_TRAP:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Mari2_Machine_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            pDamageInstance->CreateSequence("Mari2_Machine_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        }
    }
    break;
    case DT_SIEG1_SKILL_UNIQUE_JUMP:
    case DT_SIEG1_SKILL_UNIQUE_JUMP_SAME_TEAM:
    {
        if (pDamageInstance->m_Who == HitWho)
            break;

        if (!g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who))
            break;

        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            float xPos = 0.0f;
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
            {
                xPos = 1.15f;
                pDamageInstance->CreateSequence("Sieg01_Trample_Emoticon01_R", fHitX - xPos, fHitY - 0.16f, 0.5f);
                pDamageInstance->CreateSequence("Sieg01_Trample_Emoticon02_R", fHitX - xPos, fHitY - 0.16f, 0.5f);
                pDamageInstance->CreateSequence("Sieg01_Trample_Emoticon03_R", fHitX - xPos, fHitY - 0.16f, 0.5f);
            }
            else
            {
                xPos = 0.85f;
                pDamageInstance->CreateSequence("Sieg01_Trample_Emoticon01", fHitX - xPos, fHitY - 0.16f, 0.5f);
                pDamageInstance->CreateSequence("Sieg01_Trample_Emoticon02", fHitX - xPos, fHitY - 0.16f, 0.5f);
                pDamageInstance->CreateSequence("Sieg01_Trample_Emoticon03", fHitX - xPos, fHitY - 0.16f, 0.5f);
            }
        }
    }
    break;
    case DT_PET_RURU_ATTACK2:
    case DT_EXTINCTION_MON_CENTER_05:
    {
        if (IsCrashPlayer)
        {
            if (!g_MyD3D->IsPlayerIndex(HitWho))
                break;

            g_MyD3D->MyPlayer[HitWho]->SetPlayerState(MID_COMMON_FREEZED_BY_PIXIE);
            g_MyD3D->MyPlayer[HitWho]->cFrame = 0;
        }
        else
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
                pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
                pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
                pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
                    -0.01 * -1000.0f, ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, false, 0, false, false, true);
            }
        }
    }
    break;
    case DT_PET_DOKKAEBI_ATTACK3:
    {
        if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who, EGC_EFFECT_DOKKAEBIE_SHIELD) || (pDamageInstance->m_Life <= 1))
        {
            g_pMagicEffect->ClearMagicEffect(pDamageInstance->m_Who, EGC_EFFECT_DOKKAEBIE_SHIELD);
            pDamageInstance->m_Life = 1;
        }

    }
    break;

    default:
        break;
    }


    for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecCrashParticle.size(); ++i)
    {
        CRASH_PARTICLE& crashEff = pDamageInstance->m_pOriginalDamage->m_vecCrashParticle[i];

        if (crashEff.m_iCreateLifeTime != -1)
            continue;

        if (true == crashEff.m_bRenderToContactPosition)
        {
            fHitX = pDamageInstance->m_X;
            fHitY = pDamageInstance->m_Y;
        }

        CParticleEventSeqPTR pTemp;
        pTemp = pDamageInstance->CreateSequence(crashEff.m_strParticle, fHitX - 1.0f + crashEff.m_fXOffset, fHitY - 0.5f + crashEff.m_fYOffset, 0.5f);
        if (pTemp != NULL && pDamageInstance->m_pOriginalDamage->m_vecCrashParticle[i].m_bSelfDirection) {
            // ���⼺ ����
            DIRECTION_INFO stDirect;
            stDirect.m_fDirPosX = 0;
            stDirect.m_bDirVelocity = true;
            stDirect.m_pbIsRight = &pDamageInstance->m_IsRight;
            pTemp->SetDirectInfo(&stDirect);
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                pTemp->SetShow(true);
            else
                pTemp->SetShow(false);
        }
    }

    if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_HIT_EFFECT)
    {
        float fOffsetY = 0.0f;
        if (!IsCrashPlayer)
        {
            fHitX = pDamageInstance->m_X;
            fHitY = pDamageInstance->m_Y;
            fOffsetY = -0.5f;
        }
        else
        {
            fOffsetY = -0.32f;
        }
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            pDamageInstance->CreateSequence("Jin1_Hit_01", fHitX - 1.0f, fHitY + fOffsetY, 0.5f);
            pDamageInstance->CreateSequence("Jin1_Hit_02", fHitX - 1.0f, fHitY + fOffsetY, 0.5f);
            pDamageInstance->CreateSequence("Jin1_Hit_03", fHitX - 1.0f, fHitY + fOffsetY, 0.5f);
            pDamageInstance->CreateSequence("Jin1_Hit_04", fHitX - 1.0f, fHitY + fOffsetY, 0.5f);
            pDamageInstance->CreateSequence("Jin1_Hit_05", fHitX - 1.0f, fHitY + fOffsetY, 0.5f);
            pDamageInstance->CreateSequence("Jin1_Hit_06", fHitX - 1.0f, fHitY + fOffsetY, 0.5f);
        }
    }

    CheckTargetAutoDamage(pDamageInstance, HitWho, IsCrashPlayer);

    for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecAutoDamage.size(); ++i)
    {
        AUTO_DAMAGE& autoDamage = pDamageInstance->m_pOriginalDamage->m_vecAutoDamage[i];

        if (autoDamage.m_iDamageIndex == -1)
            break;

        if (false == (autoDamage.m_dwAutoDamage & CRASH_AUTO_DAMAGE))
        {
            continue;
        }

        // ��Ÿ�� �������̸� ���䵥���� �浹üũ���۽ð��� ���� ������Ÿ�Ӱ� üũ�Ѵ�.
        if (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONETIME_DAMAGE)
        {
            if (autoDamage.m_iCrashStartTime != -1 && pDamageInstance->m_PrevLife > autoDamage.m_iCrashStartTime)
                continue;
        }
        else
        {
            if (autoDamage.m_iCrashStartTime != -1 && pDamageInstance->m_Life > autoDamage.m_iCrashStartTime)
                continue;
        }

        if (true == autoDamage.m_bRenderToContactPos)
        {
            fHitX = pDamageInstance->m_X;
            fHitY = pDamageInstance->m_Y;
        }

        D3DXVECTOR2 offset;
        offset.y = autoDamage.m_fOffSetY;
        if (pDamageInstance->m_IsRight)
        {
            offset.x = autoDamage.m_fOffSetX;
        }
        else
        {
            offset.x = -autoDamage.m_fOffSetX;
        }

        CDamageInstance* pAuto;
        if (false == pDamageInstance->m_IsMonsterDamage)
        {
            pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, fHitX + offset.x,
                fHitY + offset.y, autoDamage.m_bNoSlowCount, -1, NULL, HitWho, NULL, OBJ_TYPE_PLAYER, pDamageInstance->m_emHitWhoType);
        }
        else
        {
            pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, fHitX + offset.x,
                fHitY + offset.y, autoDamage.m_bNoSlowCount, pDamageInstance->m_ExtraMonsterNum, NULL, HitWho
                , NULL, OBJ_TYPE_MONSTER, pDamageInstance->m_emHitWhoType);
        }

        if (pAuto)
        {
            pAuto->m_bPetDamage = pDamageInstance->m_bPetDamage;
            pAuto->m_IsRight = pDamageInstance->m_IsRight;
            pAuto->m_SlowCount = 0;

            if (autoDamage.m_bDieCurDamage)
                pDamageInstance->m_Life = 1;
        }

        //====================================================================================
        // 2009.02.20 : Jemitgge
        // Comment : Create one of the AUTO_DAMAGE then Other one's no more create damage...
        //			 ������.. ����ؼ� ���䵥������ �� �����ؾ� �ϴ� �� �ִ�..
        switch (pDamageInstance->m_What)
        {
        case DT_PIERCE_ARROW_EX:
        case DT_STARDUST_BOMB:
        case DT_SKILL_LAS_VENOM_FIND_VICTIM:
        case DT_SKILL_LAS_RAVEN_THROWKNIFE:
        {
            // ��ƶ� �� �׳� ��
            pDamageInstance->InsertAlreadyDamaged(HitWho + MAX_PLAYER_NUM);
        }
        break;
        case DT_BLOODYORC_UWEK_1:
        case DT_WOODEN_SUNKEN_AREA:
        case DT_HERA_CHAOS_MAGICCIRCLE_AREA:
        case DT_IBLIS_LASER_SHOOTER:
        case DT_SUPERMON_IBLIS_LASER_SHOOTER:
        case DT_THANATOS2_DARKSPEAR_SHOOTER:
        {
            // ��ƶ� �� �׳� ��
            pDamageInstance->InsertAlreadyDamaged(HitWho);
        }
        break;
        default:
            if (!(pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE) && autoDamage.m_bNoClear == false)
                autoDamage.m_dwAutoDamage = 0;
            break;
        }
    }
}

void CDamageManager::CallEffAtTime(CDamageInstance* pDamageInstance)
{
    float Angle = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;

    switch (pDamageInstance->m_What)
    {
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG2:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG2_R:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG3:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG3_R:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG4:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG4_R:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG5:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL2_DMG5_R:
        if (pDamageInstance->m_Life == 4)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ronan_SPECIAL1-3_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL1-2_Shockwave_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.55f, 0.5f);
            }
            g_KDSound.Play("914");
        }
        break;
    case DT_RONAN4_RONAN4_VALKYRIE_FINISH_R:
    case DT_RONAN4_RONAN4_VALKYRIE_FINISH_L:
        if (pDamageInstance->m_Life == 4)
        {
            AddWithLocate(DT_RONAN4_RONAN4_VALKYRIE_FINISH_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.50f);
            float fXPos;

            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
                fXPos = -1.0f;
            else
                fXPos = -1.0f;
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_01", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.55f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_02", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.550f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_03", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.550f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_04", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.550f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_05", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.550f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_06", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.550f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_07", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.550f, 0.5f);
                pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Boom_08", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.550f, 0.5f);
            }
            g_KDSound.Play("932");
        }
        break;
    case DT_LAVA_FLOW1:
        if (pDamageInstance->m_Life > 89 && pDamageInstance->m_Life < 91)
        {
            AddWithLocate(DT_THROW_STONE0, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            AddWithLocate(DT_THROW_STONE1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            AddWithLocate(DT_THROW_STONE2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            AddWithLocate(DT_THROW_STONE3, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            AddWithLocate(DT_THROW_STONE4, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            AddWithLocate(DT_THROW_STONE5, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            AddWithLocate(DT_THROW_STONE6, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;

    case DT_EXPLOSION_ARROW_READY:
    case DT_EXPLOSION_ARROW_READY_SHINING:
    case DT_EXPLOSION_ARROW_READY_LIGHTNING:
    case DT_EXPLOSION_ARROW_READY_LOVE:
        if (pDamageInstance->m_Life > 7 && pDamageInstance->m_Life < 12)
        {
            AddWithAngle(DT_EXPLOSION_ARROW, pDamageInstance->m_Who, Angle + 0.3f);
            AddWithAngle(DT_EXPLOSION_ARROW, pDamageInstance->m_Who, Angle + 0.15f);
            AddWithAngle(DT_EXPLOSION_ARROW, pDamageInstance->m_Who, Angle);
            AddWithAngle(DT_EXPLOSION_ARROW, pDamageInstance->m_Who, Angle - 0.15f);
            AddWithAngle(DT_EXPLOSION_ARROW, pDamageInstance->m_Who, Angle - 0.3f);
        }
        break;

        /*
            if( pDamageInstance->m_Life > 7 && pDamageInstance->m_Life < 12 )
            {
                AddWithAngle(DT_EXPLOSION_ARROW_LIGHTNING,pDamageInstance->m_Who, Angle + 0.3f );
                AddWithAngle(DT_EXPLOSION_ARROW_LIGHTNING,pDamageInstance->m_Who, Angle + 0.15f );
                AddWithAngle(DT_EXPLOSION_ARROW_LIGHTNING,pDamageInstance->m_Who, Angle );
                AddWithAngle(DT_EXPLOSION_ARROW_LIGHTNING,pDamageInstance->m_Who, Angle - 0.15f );
                AddWithAngle(DT_EXPLOSION_ARROW_LIGHTNING,pDamageInstance->m_Who, Angle - 0.3f );
            }
            break;

            if( pDamageInstance->m_Life > 7 && pDamageInstance->m_Life < 12 )
            {
                AddWithAngle(DT_EXPLOSION_ARROW_SHINING,pDamageInstance->m_Who, Angle + 0.3f );
                AddWithAngle(DT_EXPLOSION_ARROW_SHINING,pDamageInstance->m_Who, Angle + 0.15f );
                AddWithAngle(DT_EXPLOSION_ARROW_SHINING,pDamageInstance->m_Who, Angle );
                AddWithAngle(DT_EXPLOSION_ARROW_SHINING,pDamageInstance->m_Who, Angle - 0.15f );
                AddWithAngle(DT_EXPLOSION_ARROW_SHINING,pDamageInstance->m_Who, Angle - 0.3f );
            }
            break;*/

    case DT_METAL_SHOT:
    case DT_METAL_SHOT_LIGHTNING:
    case DT_METAL_SHOT_SHINING:
    case DT_METAL_SHOT_LOVE:
    case DT_METAL_SHOT_TIME:
    case DT_METAL_SHOT_LIFE:
    case DT_METAL_SHOT_DARK_DRAGON_RED:
    case DT_METAL_SHOT_DARK_DRAGON_BLUE:
    case DT_METAL_SHOT_HALLOWEEN:
    case DT_METAL_SHOT_ELIA:
    case DT_METAL_SHOT_PEGASUS:
    case DT_METAL_SHOT_C_MASTER:
    case DT_METAL_SHOT_COLISEU:
        if (pDamageInstance->m_Life > 7 && pDamageInstance->m_Life < 12)
        {
#if defined (MATCH_BALLENCE_121217)
            int iWhat = DT_METAL_SHOT_LV1;
#else
            int iWhat = DT_ARROW;
#endif
            if (g_kGlobalValue.CheckTrainedSkill(g_MyD3D->MyPlayer[pDamageInstance->m_Who], SID_LIRE1_METALSHOT_LV2))
                iWhat = DT_METAL_SHOT_LV2_ARROW;

            CDamageInstance* pkDamage = NULL;
            pkDamage = AddWithAngle(iWhat, pDamageInstance->m_Who, Angle - 0.2f);
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

            pkDamage = AddWithAngle(iWhat, pDamageInstance->m_Who, Angle - 0.1f);
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

            pkDamage = AddWithAngle(iWhat, pDamageInstance->m_Who, Angle);
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

            pkDamage = AddWithAngle(iWhat, pDamageInstance->m_Who, Angle + 0.1f);
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

            pkDamage = AddWithAngle(iWhat, pDamageInstance->m_Who, Angle + 0.2f);
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
        }
        break;

    case DT_METAL_SHOT_SECOND:
    {
        if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_SKILL_LIRE_METALSHOT_LV3)
        {
            if (pDamageInstance->m_Life > 242 && pDamageInstance->m_Life < 247)
            {
                CDamageInstance* pkDamage = NULL;
                pkDamage = AddWithAngle(DT_METAL_SHOT_LV3_ARROW, pDamageInstance->m_Who, Angle - 0.15f);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

                pkDamage = AddWithAngle(DT_METAL_SHOT_LV3_ARROW, pDamageInstance->m_Who, Angle);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

                pkDamage = AddWithAngle(DT_METAL_SHOT_LV3_ARROW, pDamageInstance->m_Who, Angle + 0.15f);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
            }
        }
        else if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_SKILL_LIRE_CONTINUOUS_ATK_LV2)
        {
            if (pDamageInstance->m_Life > 242 && pDamageInstance->m_Life < 247)
            {
                CDamageInstance* pkDamage = NULL;
                pkDamage = AddWithAngle(DT_CONTINUOUS_ARROW, pDamageInstance->m_Who, Angle - 0.2f);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_fDamageToPlayer *= 0.5f;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

                pkDamage = AddWithAngle(DT_CONTINUOUS_ARROW, pDamageInstance->m_Who, Angle - 0.1f);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_fDamageToPlayer *= 0.5f;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

                pkDamage = AddWithAngle(DT_CONTINUOUS_ARROW, pDamageInstance->m_Who, Angle);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_fDamageToPlayer *= 0.5f;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

                pkDamage = AddWithAngle(DT_CONTINUOUS_ARROW, pDamageInstance->m_Who, Angle + 0.1f);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_fDamageToPlayer *= 0.5f;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

                pkDamage = AddWithAngle(DT_CONTINUOUS_ARROW, pDamageInstance->m_Who, Angle + 0.2f);
                pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
                pkDamage->m_pOriginalDamage->m_fDamageToPlayer *= 0.5f;
                pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
            }
        }
    }
    break;
    case DT_DARK_SWORM_HOLE:
        if (pDamageInstance->m_Life > 180 && pDamageInstance->m_Life < 183)
        {
            AddWithLocate(DT_DARK_SWORM_FLY, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.232f);
            AddWithLocate(DT_DARK_SWORM_FLY, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.172f);
            AddWithLocate(DT_DARK_SWORM_FLY, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.122f);
            AddWithLocate(DT_DARK_SWORM_FLY, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.072f);
            AddWithLocate(DT_DARK_SWORM_FLY, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y - 0.022f);
            AddWithLocate(DT_DARK_SWORM_FLY, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.022f);
            AddWithLocate(DT_DARK_SWORM_FLY, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.072f);
        }
        break;

    case DT_ATHENS_HEAL_SWORD:
    case DT_ATHENS_HEAL_SWORD_DUN:
        //-3�� �������� ���ϴµ� DT_ATHENS_HEAL_SHOCK �ι� Add�ϱ� ���ؼ�.
        if (pDamageInstance->m_Life > 48 && pDamageInstance->m_Life < 52)
        {
            Add(DT_ATHENS_HEAL_SHOCK, pDamageInstance->m_Who);
        }

        if (pDamageInstance->m_Life < 4)
        {
            Add(DT_ATHENS_HEAL_SHOCK, pDamageInstance->m_Who);
        }
        break;
    case DT_ENT_LAND:
        if (pDamageInstance->m_Life == 255)
            Add(DT_ENT_LAND_SMOKE, pDamageInstance->m_Who);
        break;

    case DT_FLASK:
        if (pDamageInstance->m_Life == 75)
        {
            g_KDSound.Play("145");
            for (int i = 0; i < 10; i++)
                AddWithLocate(DT_GAS, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;

    case DT_DARK_SWORM_FLY:
        if (pDamageInstance->m_Life < 3)
        {
            CallAnim(pDamageInstance, "FlyBlowUp", 0.005f, 0.005f);
        }
        break;

    case DT_SOUL:
        if (pDamageInstance->m_Life < 7)
        {
            AddWithLocate(DT_SKILL_READY_CIRCLE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            pDamageInstance->Add(FLOAT_SOUL, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y, 0, 0.6f);
        }
        break;

    case DT_BOMB:
    case DT_BOMB2:
        if (pDamageInstance->m_Life < 5)
        {
            // ���� ����Ʈ�� �߻���Ų��.
            g_KDSound.Play("43");
            AddWithLocate(DT_BOMB_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
            AddWithLocate(DT_BOMB_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f);
        }
        break;
    case DT_CHEROKI_BOMB:
    {
        if (pDamageInstance->m_Life < 5)
        {
            g_KDSound.Play("43");
            g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f);
        }
    }
    break;


    case DT_TORNADO_VOLCANIC1:
    case DT_TORNADO_VOLCANIC2:
    case DT_TORNADO_VOLCANIC3:
        if (pDamageInstance->m_Life == 255)
        {
            g_KDSound.Play("34");
        }
        break;
    case DT_BLOCK_BUSTER:
        if (pDamageInstance->m_Life == 255)
            g_KDSound.Play("143");
        break;
    case DT_RED_CANNON:
    case DT_YELLOW_CANNON:
        if (pDamageInstance->m_Life == 255)
        {
            g_KDSound.Play("43");
            pDamageInstance->CreateSequence("ShotFire", pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;
    case DT_FIRE:
        if (pDamageInstance->m_Life <= 10)
        {
            //���⿡�� �� ���� ���� ����Ʈ�� �־��ش�. - ����
            pDamageInstance->CreateSequence("CrashSmoke", pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;
    case DT_ELESIS4_SPECIAL3_SHOOT:

        if (pDamageInstance->m_Life == 3 || pDamageInstance->m_Life == 6 || pDamageInstance->m_Life == 9)
        {
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight)
                g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 0.85f, pDamageInstance->m_Y - 0.4f, 0, 0.6f);
            else
                g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.15f, pDamageInstance->m_Y - 0.4f, 0, 0.6f);

            g_MyD3D->MyStg->Color_r = 128;
            g_MyD3D->MyStg->Color_g = 0;
            g_MyD3D->MyStg->Color_b = 0;
            g_KDSound.Play("43");
        }

        if (pDamageInstance->m_Life == 6 || pDamageInstance->m_Life == 9 || pDamageInstance->m_Life == 12)
        {
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight)
            {
                g_MyD3D->m_pDamageManager->AddWithLocate(DT_ELESIS4_SPECIAL3_CRASH_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X + 0.15f,
                    pDamageInstance->m_Y - 0.2f, false, pDamageInstance->m_ExtraMonsterNum);
            }
            else
            {
                g_MyD3D->m_pDamageManager->AddWithLocate(DT_ELESIS4_SPECIAL3_CRASH_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X - 0.15f,
                    pDamageInstance->m_Y - 0.2f, false, pDamageInstance->m_ExtraMonsterNum);
            }
        }
        break;
    case DT_NOVA_ARROW:
    case DT_NOVA_CRITICAL_ARROW:
    case DT_NOVA_SPECIAL3_ARROW:
    case DT_NOVA_SPECIAL1_ARROW:
    case DT_NOVA_ARROW_LIGHTNING:
    case DT_NOVA_CRITICAL_ARROW_LIGHTNING:
    case DT_NOVA_SPECIAL3_ARROW_LIGHTNING:
    case DT_NOVA_SPECIAL1_ARROW_LIGHTNING:
    case DT_NOVA_ARROW_SHINING:
    case DT_NOVA_CRITICAL_ARROW_SHINING:
    case DT_NOVA_SPECIAL3_ARROW_SHINING:
    case DT_NOVA_SPECIAL1_ARROW_SHINING:
    case DT_NOVA_ARROW_DARK:
    case DT_NOVA_CRITICAL_ARROW_DARK:
    case DT_NOVA_SPECIAL3_ARROW_DARK:
    case DT_NOVA_SPECIAL1_ARROW_DARK:
    case DT_NOVA_ARROW_LOVE:
    case DT_NOVA_CRITICAL_ARROW_LOVE:
    case DT_NOVA_SPECIAL3_ARROW_LOVE:
    case DT_NOVA_SPECIAL1_ARROW_LOVE:
    case DT_NOVA_ARROW_DEATH:
    case DT_NOVA_CRITICAL_ARROW_DEATH:
    case DT_NOVA_SPECIAL3_ARROW_DEATH:
    case DT_NOVA_SPECIAL1_ARROW_DEATH:
    case DT_NOVA_ARROW_TIME:
    case DT_NOVA_CRITICAL_ARROW_TIME:
    case DT_NOVA_SPECIAL1_ARROW_TIME:
    case DT_NOVA_ARROW_LIFE:
    case DT_NOVA_CRITICAL_ARROW_LIFE:
    case DT_NOVA_SPECIAL1_ARROW_LIFE:
    case DT_NOVA_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_BLUE:

    case DT_NOVA_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_ARROW_HALLOWEEN:
    case DT_NOVA_SPECIAL1_ARROW_HALLOWEEN:

    case DT_NOVA_ARROW_ELIA:
    case DT_NOVA_CRITICAL_ARROW_ELIA:
    case DT_NOVA_SPECIAL1_ARROW_ELIA:

    case DT_NOVA_ARROW_PEGASUS:
    case DT_NOVA_CRITICAL_ARROW_PEGASUS:
    case DT_NOVA_SPECIAL1_ARROW_PEGASUS:

    case DT_NOVA_ARROW_COLISEU:
    case DT_NOVA_CRITICAL_ARROW_COLISEU:
    case DT_NOVA_SPECIAL1_ARROW_COLISEU:

    case DT_NOVA_ARROW_C_MASTER:
    case DT_NOVA_CRITICAL_ARROW_C_MASTER:
    case DT_NOVA_SPECIAL1_ARROW_C_MASTER:

        if (pDamageInstance->m_Life == 254)
        {
            CDamageInstance* pkDamageInstance = Add(DT_NOVA_ARROW_EFF, pDamageInstance->m_Who);
            pkDamageInstance->m_pParticle->SetAdditinalZRot(pDamageInstance->m_Angle);
            pkDamageInstance->m_pParticle->SetBeginPos(pDamageInstance->m_pOriginalDamage->m_vPos);
        }
        break;
    case DT_NOVA_BOLT:
    case DT_NOVA_FREEWAY_BOLT:
    case DT_NOVA_FREEWAY_BOLT_DUN:
    case DT_NOVA_CRITICAL_BOLT:
    case DT_NOVA_SPECIAL3_BOLT:
    case DT_NOVA_SPECIAL1_BOLT:
    case DT_NOVA_BOLT_LIGHTNING:
    case DT_NOVA_FREEWAY_BOLT_LIGHTNING:
    case DT_NOVA_FREEWAY_BOLT_LIGHTNING_DUN:
    case DT_NOVA_CRITICAL_BOLT_LIGHTNING:
    case DT_NOVA_SPECIAL3_BOLT_LIGHTNING:
    case DT_NOVA_SPECIAL1_BOLT_LIGHTNING:
    case DT_NOVA_BOLT_SHINING:
    case DT_NOVA_FREEWAY_BOLT_SHINING:
    case DT_NOVA_FREEWAY_BOLT_SHINING_DUN:
    case DT_NOVA_CRITICAL_BOLT_SHINING:
    case DT_NOVA_SPECIAL3_BOLT_SHINING:
    case DT_NOVA_SPECIAL1_BOLT_SHINING:
    case DT_NOVA_BOLT_DARK:
    case DT_NOVA_FREEWAY_BOLT_DARK:
    case DT_NOVA_FREEWAY_BOLT_DARK_DUN:
    case DT_NOVA_CRITICAL_BOLT_DARK:
    case DT_NOVA_SPECIAL3_BOLT_DARK:
    case DT_NOVA_SPECIAL1_BOLT_DARK:
    case DT_NOVA_BOLT_LOVE:
    case DT_NOVA_FREEWAY_BOLT_LOVE:
    case DT_NOVA_FREEWAY_BOLT_LOVE_DUN:
    case DT_NOVA_CRITICAL_BOLT_LOVE:
    case DT_NOVA_SPECIAL3_BOLT_LOVE:
    case DT_NOVA_SPECIAL1_BOLT_LOVE:
    case DT_NOVA_DOWN_ARROW:
    case DT_NOVA_DOWN_ARROW_LIGHTNING:
    case DT_NOVA_DOWN_ARROW_SHINING:
    case DT_NOVA_DOWN_ARROW_DARK:
    case DT_NOVA_DOWN_ARROW_LOVE:
    case DT_NOVA_BOLT_DEATH:
    case DT_NOVA_FREEWAY_BOLT_DEATH:
    case DT_NOVA_FREEWAY_BOLT_DEATH_DUN:
    case DT_NOVA_CRITICAL_BOLT_DEATH:
    case DT_NOVA_SPECIAL3_BOLT_DEATH:
    case DT_NOVA_SPECIAL1_BOLT_DEATH:
    case DT_NOVA_DOWN_ARROW_DEATH:

    case DT_NOVA_BOLT_TIME:
    case DT_NOVA_FREEWAY_BOLT_TIME:
    case DT_NOVA_FREEWAY_BOLT_TIME_DUN:
    case DT_NOVA_CRITICAL_BOLT_TIME:
    case DT_NOVA_SPECIAL3_BOLT_TIME:
    case DT_NOVA_SPECIAL1_BOLT_TIME:

    case DT_NOVA_BOLT_LIFE:
    case DT_NOVA_FREEWAY_BOLT_LIFE:
    case DT_NOVA_FREEWAY_BOLT_LIFE_DUN:
    case DT_NOVA_CRITICAL_BOLT_LIFE:
    case DT_NOVA_SPECIAL3_BOLT_LIFE:
    case DT_NOVA_SPECIAL1_BOLT_LIFE:

    case DT_NOVA_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED_DUN:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_SPECIAL3_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED:

    case DT_NOVA_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE_DUN:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_SPECIAL3_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE:

    case DT_NOVA_BOLT_HALLOWEEN:
    case DT_NOVA_FREEWAY_BOLT_HALLOWEEN:
    case DT_NOVA_FREEWAY_BOLT_HALLOWEEN_DUN:
    case DT_NOVA_CRITICAL_BOLT_HALLOWEEN:
    case DT_NOVA_SPECIAL3_BOLT_HALLOWEEN:
    case DT_NOVA_SPECIAL1_BOLT_HALLOWEEN:

    case DT_NOVA_BOLT_ELIA:
    case DT_NOVA_FREEWAY_BOLT_ELIA:
    case DT_NOVA_FREEWAY_BOLT_ELIA_DUN:
    case DT_NOVA_CRITICAL_BOLT_ELIA:
    case DT_NOVA_SPECIAL3_BOLT_ELIA:
    case DT_NOVA_SPECIAL1_BOLT_ELIA:

    case DT_NOVA_BOLT_PEGASUS:
    case DT_NOVA_FREEWAY_BOLT_PEGASUS:
    case DT_NOVA_FREEWAY_BOLT_PEGASUS_DUN:
    case DT_NOVA_CRITICAL_BOLT_PEGASUS:
    case DT_NOVA_SPECIAL3_BOLT_PEGASUS:
    case DT_NOVA_SPECIAL1_BOLT_PEGASUS:

    case DT_NOVA_BOLT_C_MASTER:
    case DT_NOVA_BOLT_COLISEU:
    case DT_NOVA_FREEWAY_BOLT_COLISEU:
    case DT_NOVA_FREEWAY_BOLT_COLISEU_DUN:
    case DT_NOVA_CRITICAL_BOLT_COLISEU:
    case DT_NOVA_SPECIAL3_BOLT_COLISEU:
    case DT_NOVA_SPECIAL1_BOLT_COLISEU:

    case DT_NOVA_FREEWAY_BOLT_C_MASTER:
    case DT_NOVA_FREEWAY_BOLT_C_MASTER_DUN:
    case DT_NOVA_CRITICAL_BOLT_C_MASTER:
    case DT_NOVA_SPECIAL3_BOLT_C_MASTER:
    case DT_NOVA_SPECIAL1_BOLT_C_MASTER:

        if (pDamageInstance->m_Life == 252)
        {
            CDamageInstance* pkDamageInstance = Add(DT_NOVA_BOLT_EFF, pDamageInstance->m_Who);
            pkDamageInstance->m_pParticle->SetAdditinalZRot(pDamageInstance->m_Angle);
            pkDamageInstance->m_pParticle->SetBeginPos(pDamageInstance->m_pOriginalDamage->m_vPos);
        }
        break;

    case DT_NOVA_SPECIAL3_ARROW_SHOT:
    case DT_NOVA_SPECIAL3_ARROW_LIGHTNING_SHOT:
    case DT_NOVA_SPECIAL3_ARROW_SHINING_SHOT:
    case DT_NOVA_SPECIAL3_ARROW_DARK_SHOT:
    case DT_NOVA_SPECIAL3_ARROW_LOVE_SHOT:
    case DT_NOVA_SPECIAL3_ARROW_DEATH_SHOT:
    case DT_NOVA_SPECIAL3_ARROW_COLISEU_SHOT:
    case DT_NOVA_SPECIAL3_ARROW_C_MASTER_SHOT:
        if (pDamageInstance->m_Life > 7 && pDamageInstance->m_Life < 12)
        {
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW, pDamageInstance->m_Who, Angle + 0.3f);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW, pDamageInstance->m_Who, Angle + 0.15f);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW, pDamageInstance->m_Who, Angle);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW, pDamageInstance->m_Who, Angle - 0.15f);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW, pDamageInstance->m_Who, Angle - 0.3f);
        }
        break;
    case DT_NOVA_SPECIAL1_ARROW_SHOT_DUN:
    case DT_NOVA_SPECIAL1_ARROW_LIGHTNING_SHOT_DUN:
    case DT_NOVA_SPECIAL1_ARROW_SHINING_SHOT_DUN:
    case DT_NOVA_SPECIAL1_ARROW_DARK_SHOT_DUN:
    case DT_NOVA_SPECIAL1_ARROW_LOVE_SHOT_DUN:
    case DT_NOVA_SPECIAL1_ARROW_DEATH_SHOT_DUN:
    case DT_NOVA_SPECIAL1_ARROW_COLISEU_SHOT_DUN:
    case DT_NOVA_SPECIAL1_ARROW_C_MASTER_SHOT_DUN:
        if (pDamageInstance->m_Life > 7 && pDamageInstance->m_Life < 12)
        {
            AddWithAngle(DT_NOVA_SPECIAL1_ARROW, pDamageInstance->m_Who, Angle + 0.3f);
            AddWithAngle(DT_NOVA_SPECIAL1_ARROW, pDamageInstance->m_Who, Angle + 0.15f);
            AddWithAngle(DT_NOVA_SPECIAL1_ARROW, pDamageInstance->m_Who, Angle);
            AddWithAngle(DT_NOVA_SPECIAL1_ARROW, pDamageInstance->m_Who, Angle - 0.15f);
            AddWithAngle(DT_NOVA_SPECIAL1_ARROW, pDamageInstance->m_Who, Angle - 0.3f);
        }
        break;
    case DT_NOVA_SPECIAL3_ARROW_SHOT_DUN:
    case DT_NOVA_SPECIAL3_ARROW_LIGHTNING_SHOT_DUN:
    case DT_NOVA_SPECIAL3_ARROW_SHINING_SHOT_DUN:
    case DT_NOVA_SPECIAL3_ARROW_DARK_SHOT_DUN:
    case DT_NOVA_SPECIAL3_ARROW_LOVE_SHOT_DUN:
    case DT_NOVA_SPECIAL3_ARROW_DEATH_SHOT_DUN:
    case DT_NOVA_SPECIAL3_ARROW_COLISEU_SHOT_DUN:
    case DT_NOVA_SPECIAL3_ARROW_C_MASTER_SHOT_DUN:
        if (pDamageInstance->m_Life > 7 && pDamageInstance->m_Life < 12)
        {
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW_DUN, pDamageInstance->m_Who, Angle + 0.3f);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW_DUN, pDamageInstance->m_Who, Angle + 0.15f);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW_DUN, pDamageInstance->m_Who, Angle);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW_DUN, pDamageInstance->m_Who, Angle - 0.15f);
            AddWithAngle(DT_NOVA_SPECIAL3_ARROW_DUN, pDamageInstance->m_Who, Angle - 0.3f);
        }
        break;
    case DT_NOVA_SPECIAL2_BLOOD_CLOUD1:
    case DT_NOVA_SPECIAL2_BLOOD_CLOUD3:
    case DT_NOVA_SPECIAL2_BLOOD_CLOUD5:
    case DT_NOVA_SPECIAL2_BLOOD_CLOUD8:
        // ������ ����
        if (pDamageInstance->m_Life >= 15)
        {
            if ((90 - pDamageInstance->m_Life) % 24 == 0)
            {
                AddWithLocate(DT_NOVA_SPECIAL2_DOWN_ARROW, pDamageInstance->m_Who,
                    pDamageInstance->m_pOriginalDamage->m_vPos.x + 1.0f,
                    pDamageInstance->m_pOriginalDamage->m_vPos.y + 0.4f);
            }
        }
        break;

    case DT_NOVA_SPECIAL2_BLOOD_CLOUD_DUN:
        // ������ ����
        if (pDamageInstance->m_Life >= 15)
        {
            if ((90 - pDamageInstance->m_Life) % 24 == 0)
            {
                AddWithLocate(DT_NOVA_SPECIAL2_DOWN_ARROW_DUN, pDamageInstance->m_Who,
                    pDamageInstance->m_pOriginalDamage->m_vPos.x + 1.0f,
                    pDamageInstance->m_pOriginalDamage->m_vPos.y + 0.4f);
            }
        }
        // ���� �ϰ� �����°�
        break;

        // ���� �ϰ� �����°�
    case DT_RONAN_MAGIC_SPECIAL2_READY:
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 55)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Ronan_SPECIAL2-2-under04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.4f, 0.5f);
                pDamageInstance->CreateSequence("Ronan_SPECIAL2-2-under05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.4f, 0.5f);
            }

            CDamageInstance* pDmg = NULL;
            if (pDamageInstance->m_IsMonsterDamage)
            {
                MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                if (pMonster != NULL) {
                    float fX = pDamageInstance->m_X;
                    float fY = pMonster->m_afY[LATENCY_INDEX] + pDamageInstance->m_pOriginalDamage->m_Start_Locate_y;

                    pDmg = MonsterAddWithLocate(DT_RONAN_MAGIC_SPECIAL2_METEO_CREATE, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X, pDamageInstance->m_Y);
                }

            }
            else
            {
                pDmg = AddWithLocate(DT_RONAN_MAGIC_SPECIAL2_METEO_CREATE, pDamageInstance->m_Who, pDamageInstance->m_X,
                    pDamageInstance->m_Y);
            }
        }
        break;

    case DT_RONAN_MAGIC_SPECIAL2_METEO_CREATE:
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 1)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Ronan_SPECIAL2-2-01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ronan_SPECIAL2-2-02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ronan_SPECIAL2-2-03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ronan_SPECIAL2-2-04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
        }
        else if (pDamageInstance->m_Life == 1)
        {
            CDamageInstance* pDmg = NULL;
            if (pDamageInstance->m_IsMonsterDamage)
                pDmg = MonsterAddWithLocate(DT_RONAN_MAGIC_SPECIAL2_METEO, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X, pDamageInstance->m_Y);
            else
            {
                int iDamage = DT_RONAN_MAGIC_SPECIAL2_METEO;
                if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->CheckSkill(SID_RONAN1_WHITE2_BLASTBOMB_LV2))
                {
                    iDamage = DT_RONAN_METEO_LV2;
                }
                pDmg = AddWithLocate(iDamage, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            }

            // �������� x, y
            pDmg->m_fAnimScaleX = pDamageInstance->m_fAnimScaleX;
            pDmg->m_fAnimScaleY = pDamageInstance->m_fAnimScaleY;
        }
        break;

    case DT_RONAN_MAGIC_SPECIAL2_BOMB_WAVE:
        if (pDamageInstance->m_Life == 1)
        {
            if (pDamageInstance->m_IsMonsterDamage)
            {
                MonsterAddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X - 0.2f, pDamageInstance->m_Y);
                MonsterAddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X - 0.1f, pDamageInstance->m_Y);
                MonsterAddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X, pDamageInstance->m_Y);
                MonsterAddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X + 0.1f, pDamageInstance->m_Y);
                MonsterAddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X + 0.2f, pDamageInstance->m_Y);
            }
            else
            {
                AddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X - 0.2f, pDamageInstance->m_Y);
                AddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X - 0.1f, pDamageInstance->m_Y);
                AddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
                AddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X + 0.1f, pDamageInstance->m_Y);
                AddWithLocate(DT_RONAN_MAGIC_SPECIAL2_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X + 0.2f, pDamageInstance->m_Y);
            }
        }
        break;
    case DT_RONAN_SWORD_SPECIAL2_ATK:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 1)
        {
            g_KDSound.Play("529");
        }
        else if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 3)
        {
            g_KDSound.Play("494");
        }
    }
    break;
    case DT_RONAN_SWORD_SPECIAL1_ATK:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 1)
        {
            g_KDSound.Play("490");
        }
    }
    break;

    case DT_RYAN3_SPECIAL3_WING_START:
    case DT_RYAN3_SPECIAL3_WING_START_L:
    {
        if (pDamageInstance->m_Life == 15)
        {
            CDamageInstance* pDmg = NULL;
            if (pDamageInstance->m_What == DT_RYAN3_SPECIAL3_WING_START)
            {
                pDmg = Add(DT_RYAN3_SPECIAL3_WING_END, pDamageInstance->m_Who);
            }
            else
            {
                pDmg = Add(DT_RYAN3_SPECIAL3_WING_END_L, pDamageInstance->m_Who);
            }
            pDmg->m_IsRight = true;

            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Ryan02_special3_effetc2_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ryan02_special3_effetc2_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ryan02_special3_effetc2_07", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ryan02_special3_effetc02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ryan02_special3_effetc03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
        }
    }
    break;

    case DT_RYAN3_SPECIAL1_BAMBOO:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life)
        {
            g_KDSound.Play("534");
        }
    }
    break;

    case DT_RYAN3_SPECIAL2_HORIZONAL_CUT:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life)
        {
            g_KDSound.Play("536");
        }
    }
    break;

    case DT_RYAN3_SPECIAL2_VERTICAL_CUT_R:
    case DT_RYAN3_SPECIAL2_VERTICAL_CUT_L:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life)
        {
            g_KDSound.Play("537");
        }
    }
    break;

    case DT_RYAN3_SPECIAL3_WEAPON_R:
    case DT_RYAN3_SPECIAL3_WEAPON_L:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life)
        {
            g_KDSound.Play("540");
        }
        else if (pDamageInstance->m_Life == 20)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Ryan02_special3_effetc02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Ryan02_special3_effetc04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
        }
    }
    break;

    case DT_RYAN3_SEED_BLOOM:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life)
        {
            g_KDSound.Play("548");
            // [12/10/2007] breadceo. �а� ������ �ڶ� ���� �ɴ�~
            if (randf() <= 0.2f)
            {
                pDamageInstance->CreateSequence("Ryan02_flower_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
        }
    }
    break;

    case DT_VANESSA_GOD_ATK6_BALL:
    {
        if (!SiKGCRoomManager()->IsMonsterGameMode())
        {
            return;
        }
        //�ٴ�üũ���� �� callcrashground������ �������� �ʹ� Ŀ�� ��Ȯ�� Ÿ�̹��� �� ���� ����.
        if (0.3f >= pDamageInstance->m_Y)
        {
            pDamageInstance->m_Life = 1;
            MonsterAddWithLocate(DT_VANESSA_GOD_ATK6_EXPLOSION, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
    }
    break;
    case DT_CUPID_ARROW:
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 42)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Cupid_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Cupid_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Cupid_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Cupid_spark04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
            pDamageInstance->m_Life = 1;
            g_KDSound.Play("666");
        }
        break;
    case DT_AMY2_PERFORMENCE_SPECIAL1_ATK:
        if (pDamageInstance->m_Life == 1)
        {
            AddWithLocate(DT_AMY2_PERFORMENCE_SPECIAL_ATK_DAMAGE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Amy02_special2-1_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Amy02_special2-1_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Amy02_special2-1_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Amy02_special2-1_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Amy02_special2-1_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            }
            pDamageInstance->m_Life = 1;
            g_KDSound.Play("812");
        }
        break;
    case DT_RYAN4_ONE_HAND_SPECIAL3_SPEAR_R:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 1)
            {
                pDamageInstance->CreateSequence("Ryan04_specialt2-3_final_effect01_R", pDamageInstance->m_X - 0.8f, pDamageInstance->m_Y - 0.66f, 0.5f);
            }
            if (0 == o3)
            {
                pDamageInstance->CreateSequence("Ryan04_special2-3_final_effetct03", pDamageInstance->m_X - 0.92f, pDamageInstance->m_Y - 0.56f, 0.5f);
            }
        }
    }
    break;
    case DT_RYAN4_ONE_HAND_SPECIAL3_SPEAR_L:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 1)
            {
                pDamageInstance->CreateSequence("Ryan04_specialt2-3_final_effect01_L", pDamageInstance->m_X - 1.2f, pDamageInstance->m_Y - 0.66f, 0.5f);
            }
            if (0 == o3)
            {
                pDamageInstance->CreateSequence("Ryan04_special2-3_final_effetct03", pDamageInstance->m_X - 1.08f, pDamageInstance->m_Y - 0.56f, 0.5f);
            }
        }
    }
    break;

    case DT_RYAN4_SPECIAL2_KICK:
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
        {
            if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 1)
            {
                if (pDamageInstance->m_IsRight)
                {
                    pDamageInstance->CreateSequence("Ryan04_special1-2_swing_R", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                }
                else
                {
                    pDamageInstance->CreateSequence("Ryan04_special1-2_swing_L", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                }
            }
        }
    }
    break;

    case DT_RYAN4_ONE_HAND_SPECIAL2_DUMMY:
    {
        unsigned short ucFrame = pDamageInstance->m_p3DObject->GetFrame();
        if (ucFrame == 10 || ucFrame == 16 || ucFrame == 20 || ucFrame == 24)
        {
            AddWithLocate(DT_RYAN4_ONE_HAND_SPECIAL2_PUSH, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
        }
        else if (ucFrame == 30 || ucFrame == 35 || ucFrame == 40 || ucFrame == 45)
        {
            AddWithLocate(DT_RYAN4_ONE_HAND_SPECIAL2_PULL, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
        }
    }
    break;

    case DT_RYAN4_MDAN_SPECIAL3_EFREET_BEGIN:
    {
        if (pDamageInstance->m_Life == 4)
        {
            AddWithLocate(DT_RYAN4_MDAN_SPECIAL3_EFREET, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
            g_KDSound.Play("633");
        }
    }
    break;

    case DT_RYAN4_MDAN_SPECIAL3_EFREET:
    {
        unsigned short ucFrame = pDamageInstance->m_p3DObject->GetFrame();
        if (ucFrame == 60)
        {
            g_KDSound.Play("634");
        }
        else if (ucFrame == 85)
        {
            g_KDSound.Play("635");
        }
        else if (ucFrame == 89)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                if (pDamageInstance->m_IsRight)
                {
                    pDamageInstance->CreateSequence("Ryan04_specialt2-3_final_effect01_R", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                }
                else
                {
                    pDamageInstance->CreateSequence("Ryan04_specialt2-3_final_effect01_L", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                }
            }
        }
        else if (ucFrame == 92)
        {
            AddWithLocate(DT_RYAN4_MDAN_SPECIAL3_EFREET_BOMB, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
            g_KDSound.Play("595");
        }
        else if (ucFrame == 122)
        {
            g_KDSound.Play("664");
        }
    }
    break;
    case DT_RYAN4_SPECIAL3_DROP_WEAPON:
    {
        unsigned short ucFrame = pDamageInstance->m_p3DObject->GetFrame();
        if (ucFrame == 5)
        {
            g_KDSound.Play("669");
        }
    }
    break;
    case DT_RYAN4_SPECIAL3_SHINING_FINGER_DECO:
    {
        if (pDamageInstance->m_Life == pDamageInstance->m_pOriginalDamage->m_Start_Life - 1)
        {
            g_KDSound.Play("668");
        }
    }
    break;
    case DT_ELESIS_NEWSPECIAL3_LV3_BALL1:
    case DT_ELESIS_NEWSPECIAL3_LV3_BALL2:
    case DT_ELESIS_NEWSPECIAL3_LV3_BALL3:
    case DT_ELESIS_NEWSPECIAL3_LV3_BALL4:
    case DT_ELESIS_NEWSPECIAL3_LV3_BALL5:
        if (pDamageInstance->m_Life == 1)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV3_Boom05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            }
            g_KDSound.Play("855");
        }
        break;
    case DT_SKILL_ELESIS1_SPECIAL3_LV1_FIREBALL:
        if (pDamageInstance->m_Life == 1)
        {
            AddWithLocate(DT_SKILL_ELESIS1_SPECIAL3_LV1_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV1_Boom_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV1_Boom_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV1_Boom_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV1_Boom_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV1_Boom_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_SPECIAL3_LV1_Boom_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            }
            g_KDSound.Play("855");
        }
        break;
    case DT_SKILL_ELESIS1_CRITICALX_LV2_SHOT:
        if (pDamageInstance->m_Life == 1)
        {
            AddWithLocate(DT_SKILL_ELESIS1_CRITICALX_LV3_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_07", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_08", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_09", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            }
            g_KDSound.Play("855");
        }
        break;
    case DT_SKILL_ELESIS1_CRITICALX_LV2_SHOT_R:
        if (pDamageInstance->m_Life == 1)
        {
            AddWithLocate(DT_SKILL_ELESIS1_CRITICALX_LV3_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_01_R", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_02_R", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_07", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_08", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("ELESIS1_CRITICALX_LV2_After_09", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            }
            g_KDSound.Play("855");
        }
        break;
    case DT_RONAN4_MONOVOLT_ATK2_1:
    case DT_RONAN4_MONOVOLT_ATK2_2:
    case DT_RONAN4_MONOVOLT_ATK2_3:
        if (pDamageInstance->m_Life == 1)
        {
            AddWithLocate(DT_RONAN4_MONOVOLT_ATK2_BEFORE_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;
    case DT_RONAN4_MONOVOLT_ATK2_BEFORE_BOOM:
        if (pDamageInstance->m_Life == 1)
        {
            AddWithLocate(DT_RONAN4_MONOVOLT_ATK2_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            g_KDSound.Play("905");
        }
        break;
    case DT_ARME_SKILL_ICECIRCULAR_LV2_FREEZING:
        if (pDamageInstance->m_Life == 10)
        {
            AddWithLocate(DT_ARME_SKILL_ICECIRCULAR_LV2_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            g_KDSound.Play("958");
        }
        break;
    case DT_ARME_SKILL_FINISHTOUCH_LV1_BALL:
        if (pDamageInstance->m_Life == 16)
        {
            pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.015f;
        }
        break;
    case DT_ARME_SKILL_WIZARD_DOLL_OBJECT:
    case DT_ARME_SKILL_WIZARD_DOLL_OBJECT2:
    case DT_ARME_SKILL_WIZARD_DOLL_OBJECT3:
    case DT_ARME_SKILL_WIZARD_DOLL_OBJECT_L:
    case DT_ARME_SKILL_WIZARD_DOLL_OBJECT2_L:
    case DT_ARME_SKILL_WIZARD_DOLL_OBJECT3_L:
        if (pDamageInstance->m_Life == 1)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Arme01_Change_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.47f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Change_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.47f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Change_spark04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.47f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Change_spark05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.47f, 0.5f);
            }
            g_KDSound.Play("982");
        }
        break;
    case DT_ARME_SKILL_ICETRAP_LV1_ICE:
    case DT_ARME_SKILL_ICETRAP_LV1_DIRECTLY_ICE:
        if (pDamageInstance->m_Life == 20)
        {
            pDamageInstance->m_Life = 1;
            AddWithLocate(DT_ARME_SKILL_ICETRAP_LV1_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom07", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Icetrap_LV1_boom08", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f);
            }
            g_KDSound.Play("964");
        }
        break;
        //case DT_ARME_SKILL_ICETRAP_LV1_DIRECTLY_ICE:
        //	if( pDamageInstance->m_Life == 20)
        //	{
        //		pDamageInstance->m_Life = 1;
        //		AddWithLocate(DT_ARME_SKILL_ICETRAP_LV1_BOOM, pDamageInstance->m_Who,pDamageInstance->m_X,pDamageInstance->m_Y);	
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom07", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme01_Icetrap_LV1_boom08", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.35f, 0.5f );
        //		g_KDSound.Play( "964" );
        //}
        //break;
        //case DT_ARME_SKILL_WALLOFICE_EFFECT:
        //	if( pDamageInstance->m_Life <= 1)
        //	{			
        //		pDamageInstance->CreateSequence( "Arme_Wall_Of_Ice_Crash_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme_Wall_Of_Ice_Crash_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f );			
        //		pDamageInstance->CreateSequence( "Arme_Wall_Of_Ice_Crash_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme_Wall_Of_Ice_Crash_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme_Wall_Of_Ice_Crash_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f );
        //		pDamageInstance->CreateSequence( "Arme_Wall_Of_Ice_Crash_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f );						
        //	}
        //	break;
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL:
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL2:
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL_DUN:
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL2_DUN:
        if (pDamageInstance->m_Life <= 1)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_04", pDamageInstance->m_X - 1.2f, pDamageInstance->m_Y - 0.6f, 0.5f);
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_Crash_04", pDamageInstance->m_X - 1.2f, pDamageInstance->m_Y - 0.6f, 0.5f);
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_Crash_05", pDamageInstance->m_X - 1.2f, pDamageInstance->m_Y - 0.6f, 0.5f);
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_Crash_06", pDamageInstance->m_X - 1.2f, pDamageInstance->m_Y - 0.6f, 0.5f);
            }
        }
        break;
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL1_L:
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL2_L:
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL1_L_DUN:
    case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL2_L_DUN:
        if (pDamageInstance->m_Life <= 1)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_02", pDamageInstance->m_X - 0.7f, pDamageInstance->m_Y - 0.6f, 0.5f);
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_Crash_01", pDamageInstance->m_X - 0.7f, pDamageInstance->m_Y - 0.6f, 0.5f);
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_Crash_02", pDamageInstance->m_X - 0.7f, pDamageInstance->m_Y - 0.6f, 0.5f);
                pDamageInstance->CreateSequence("Arme_Wall_Of_Ice_Crash_03", pDamageInstance->m_X - 0.7f, pDamageInstance->m_Y - 0.6f, 0.5f);
            }
        }
        break;
    case DT_ARME_SKILL_FROSTRING_LV1_ATK1:
        if (pDamageInstance->m_Life == 20)
        {
            pDamageInstance->m_Life = 1;
            AddWithLocate(DT_ARME_SKILL_FROSTRING_LV1_ATK2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;
    case DT_ARME_SKILL_FROSTRING_LV2_ATK1:
        if (pDamageInstance->m_Life == 20)
        {
            pDamageInstance->m_Life = 1;
            AddWithLocate(DT_ARME_SKILL_FROSTRING_LV2_ATK2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;
    case DT_ARME_SKILL_SAINTBREEDING_SPACE:
    {
        if (pDamageInstance->m_Life == 1)
        {

            pDamageInstance->m_Life = 0;

            if (!SiKGCRoomManager()->IsMonsterGameMode())
            {
                //	��ȯ�� �ϰ��
                if (pDamageInstance->m_emHitWhoType == OBJ_TYPE_MONSTER)
                {
                    MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_HitWho);
                    if (pMonster != NULL) {
                        AddWithLocate(DT_ARME_SKILL_SAINTBREEDING_BOOM, pDamageInstance->m_Who,
                            pMonster->GetX(), pMonster->GetY() + 0.2f, false, -1, NULL, pDamageInstance->m_HitWho);
                    }
                }
                else
                {
                    AddWithLocate(DT_ARME_SKILL_SAINTBREEDING_BOOM, pDamageInstance->m_Who,
                        g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPosition().x, g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPosition().y + 0.2f, false, -1, NULL, pDamageInstance->m_HitWho);
                }

                for (int i = 0; i < MAX_PLAYER_NUM; i++)
                {
                    m_iPlayerIndex[i] = -1;
                }
            }
            else
            {
                MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_HitWho);
                if (pMonster != NULL) {

                    AddWithLocate(DT_ARME_SKILL_SAINTBREEDING_BOOM, pDamageInstance->m_Who,
                        pMonster->GetX(), pMonster->GetY() + 0.2f, false, -1, NULL, pDamageInstance->m_HitWho);

                }
                g_kMonsterManager.CreateMonsterAttackIndex(m_mapMonsterIndex);
            }
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Arme01_Hell_Breath_Boom_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Hell_Breath_Boom_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Hell_Breath_Boom_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Hell_Breath_Boom_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Hell_Breath_Boom_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.45f, 0.5f);
            }
            g_KDSound.Play("1013");
        }
    }
    break;
    case DT_ARME_SKILL_BIGBOOM_FIRE:
    case DT_ARME_SKILL_BIGBOOM_FIRE_L:
    {
        if (pDamageInstance->m_Life == 10)
        {
            AddWithLocate(DT_ARME_SKILL_BIGBOOM_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        else if (pDamageInstance->m_Life == 5)
        {
            g_KDSound.Play("1045");
            AddWithLocate(DT_ARME_SKILL_BIGBOOM_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);

            float xPos = 0.0f;
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
                xPos = 0.7f;
            else
                xPos = 1.3f;
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Arme01_Big_Boom_Boom_01", pDamageInstance->m_X - xPos, pDamageInstance->m_Y - 0.55f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Big_Boom_Boom_02", pDamageInstance->m_X - xPos, pDamageInstance->m_Y - 0.55f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Big_Boom_Boom_03", pDamageInstance->m_X - xPos, pDamageInstance->m_Y - 0.55f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Big_Boom_Boom_04", pDamageInstance->m_X - xPos, pDamageInstance->m_Y - 0.55f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Big_Boom_Boom_05", pDamageInstance->m_X - xPos, pDamageInstance->m_Y - 0.55f, 0.5f);
                pDamageInstance->CreateSequence("Arme01_Big_Boom_Boom_06", pDamageInstance->m_X - xPos, pDamageInstance->m_Y - 0.55f, 0.5f);
            }
        }
    }
    break;
    case DT_AMY3_COMMON_JUMP_UNIQUE_SKILL1_ATK:
    {
        if (pDamageInstance->m_Life == 0)
            g_KDSound.Play("1078");
    }
    break;
    case DT_AMY3_DANCE_SPECIAL1_ATK:
    {
        if (pDamageInstance->m_Life == 0)
            g_KDSound.Play("1100");
    }
    break;
    case DT_SKILL_LAS_SUMMON_KAZEAZE_LV1:
    {
        if (pDamageInstance->m_Life == 1)
        {
            g_KDSound.Play("1430");
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                g_ParticleManager->CreateSequence("Las01_Summon_Kazeaze_LV1_Off_01", pDamageInstance->m_X + pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vLocalPos.x - 1.0f,
                    pDamageInstance->m_Y - 0.48f, 0.5f);
                g_ParticleManager->CreateSequence("Las01_Summon_Kazeaze_LV1_Off_02", pDamageInstance->m_X + pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vLocalPos.x - 1.0f,
                    pDamageInstance->m_Y - 0.48f, 0.5f);
            }
        }
    }
    break;
    case DT_SKILL_LAS_SUMMON_KAZEAZE_LV2:
    {
        if (pDamageInstance->m_Life == 10)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                g_ParticleManager->CreateSequence("Las01_Summon_Kazeaze_LV2_Off_01", pDamageInstance->m_X + pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vLocalPos.x - 1.0f,
                    pDamageInstance->m_Y - 0.48f, 0.5f);
                g_ParticleManager->CreateSequence("Las01_Summon_Kazeaze_LV2_Off_02", pDamageInstance->m_X + pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vLocalPos.x - 1.0f,
                    pDamageInstance->m_Y - 0.48f, 0.5f);
                g_ParticleManager->CreateSequence("Las01_Summon_Kazeaze_LV2_Off_03", pDamageInstance->m_X + pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vLocalPos.x - 1.0f,
                    pDamageInstance->m_Y - 0.48f, 0.5f);
                g_ParticleManager->CreateSequence("Las01_Summon_Kazeaze_LV2_Off_04", pDamageInstance->m_X + pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vLocalPos.x - 1.0f,
                    pDamageInstance->m_Y - 0.48f, 0.5f);
            }
        }
    }
    break;
    case DT_SKILL_LAS_HONET_STING:
    {
        unsigned short ucFrame = pDamageInstance->m_p3DObject->GetFrame();
        if (ucFrame % 5 == 1)
        {
            CDamageInstance* pDamage = NULL;
            pDamage = AddWithLocate(DT_SKILL_LAS_HONET_STING_DAMAGE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);

            if (50 > pDamageInstance->m_Life)
            {
                pDamage->m_pOriginalDamage->m_fDMXSpeed = 0.0f;
                pDamage->m_pOriginalDamage->m_fPushTargetX = 0.0f;
            }
        }

        if (pDamageInstance->m_Life == 1)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Las01_Hornet_Spark_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.63f, 0.5f);
                pDamageInstance->CreateSequence("Las01_Hornet_Spark_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.63f, 0.5f);
                pDamageInstance->CreateSequence("Las01_Hornet_Spark_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.63f, 0.5f);
            }
        }
    }
    break;
    case DT_SKILL_RYAN_GREAT_DIVIDE_2:
    {
        if (pDamageInstance->m_Life == 75)
        {
            for (int i = 0; i < 5; i++)
            {
                CDamageInstance* pDamage = AddWithLocate(DT_SKILL_RYAN_GREAT_DIVIDE_3, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);

                switch (i)
                {
                case 1:
                    pDamage->m_Angle = 3.752f;
                    break;
                case 2:
                    pDamage->m_Angle = 0.349f;
                    break;
                case 3:
                    pDamage->m_Angle = 1.745f;
                    break;
                case 4:
                    pDamage->m_Angle = 2.967f;
                    break;
                default:
                    break;
                }
            }
        }
        break;
    }
    break;
    case DT_SKILL_RYAN_STONE_WALL:
        if (pDamageInstance->m_Life == 1)
        {
            if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            {
                pDamageInstance->CreateSequence("Ryan1_Stonewall_Broken_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
                pDamageInstance->CreateSequence("Ryan1_Stonewall_Broken_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
                pDamageInstance->CreateSequence("Ryan1_Stonewall_Broken_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
                pDamageInstance->CreateSequence("Ryan1_Stonewall_Broken_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
            }
            g_KDSound.Play("40062");
        }
        break;
    case DT_SKILL_RYAN_BLOODY_DOOR:
    case DT_SKILL_RYAN_BLOODY_DOOR_DUN:
        if (pDamageInstance->m_Life == 150)
        {
            D3DXVECTOR3 vDestPos(pDamageInstance->m_X - 0.75f, pDamageInstance->m_Y - 0.5f, 0.5f);
            CParticleEventSeqPTR pTemp = pDamageInstance->CreateSequence("Ryan1_Bloody_Funeral_Effect_03", vDestPos.x, vDestPos.y, vDestPos.z);
            if (pTemp)
            {
                pTemp->SetInternalBlackHolePos(vDestPos);
                // ���⼺ ����
                DIRECTION_INFO stDirect;
                stDirect.m_fDirPosX = -0;
                stDirect.m_bDirVelocity = true;
                stDirect.m_pbIsRight = &pDamageInstance->m_IsRight;
                pTemp->SetDirectInfo(&stDirect);
                if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                    pTemp->SetShow(true);
                else
                    pTemp->SetShow(false);
            }
        }
        break;
    case DT_SKILL_RYAN_BLOODY_DOOR_R:
    case DT_SKILL_RYAN_BLOODY_DOOR_DUN_R:
        if (pDamageInstance->m_Life == 150)
        {
            D3DXVECTOR3 vDestPos(pDamageInstance->m_X - 1.2f, pDamageInstance->m_Y - 0.5f, 0.5f);
            CParticleEventSeqPTR pTemp = pDamageInstance->CreateSequence("Ryan1_Bloody_Funeral_Effect_03_R", vDestPos.x, vDestPos.y, vDestPos.z);
            if (pTemp)
            {
                pTemp->SetInternalBlackHolePos(vDestPos);
                // ���⼺ ����
                DIRECTION_INFO stDirect;
                stDirect.m_fDirPosX = -0;
                stDirect.m_bDirVelocity = true;
                stDirect.m_pbIsRight = &pDamageInstance->m_IsRight;
                pTemp->SetDirectInfo(&stDirect);
                if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                    pTemp->SetShow(true);
                else
                    pTemp->SetShow(false);
            }
        }
        break;
    case DT_TALIN_SUPERFRAME_LAUNCHER:
    case DT_TALIN_SUPERFRAME_LAUNCHER_BOTTOM:
        if (pDamageInstance->m_Life == 1651)
        {
            pDamageInstance->m_pOriginalDamage->m_bNoPassDamage = false;
            pDamageInstance->m_pOriginalDamage->m_iDamageHP = 0;
        }
        break;
    case DT_LEVELUP_EFF:
        if (pDamageInstance->m_Life == 199)
        {
            int iMaxPlayerNum = MAX_PLAYER_NUM;

            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
                iMaxPlayerNum = MAX_DUNGEON_PLAYER_NUM;

            if (pDamageInstance->m_Who >= 0 && pDamageInstance->m_Who < iMaxPlayerNum)
            {
                PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
                pPlayer->CreatePlayerName(pPlayer->m_pObject);
                std::wstring& strLevelUpMessage = g_pkStrLoader->GetReplacedString(8210, "l", pPlayer->GetStrUserName());
                g_pkChatManager->AddChatMsgTo(GS_GAME, strLevelUpMessage, KGCChatManager::CHAT_TYPE_NORMAL);

                if (pPlayer == g_MyD3D->GetMyPlayer())
                    g_pkUIScene->OnLevelupInfo();
            }
        }
        break;

    case DT_HELL_HOWLING_GHOST_VULCANUS:
        if (pDamageInstance->m_Life == 1)
        {
            pDamageInstance->CreateSequence("Pain_Ghost_Boom", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
            pDamageInstance->CreateSequence("Dark_Templar_Cursefire_Hit_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
            pDamageInstance->CreateSequence("HellBigboy_Punch_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
        }
        break;
    case DT_HELL_DEATHWORM_STONE:
        if (pDamageInstance->m_Life == 1)
        {
            pDamageInstance->CreateSequence("Firegolem_Rock_Spark", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.48f, 0.5f);
        }
        break;
    default:
        break;
    }


    for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecCrashParticle.size(); ++i)
    {
        CRASH_PARTICLE& crashEff = pDamageInstance->m_pOriginalDamage->m_vecCrashParticle[i];

        if (crashEff.m_iCreateLifeTime == -1)
            continue;

        if (crashEff.m_iCreateLifeTime == pDamageInstance->m_Life) {
            CParticleEventSeqPTR pTemp;
            pTemp = pDamageInstance->CreateSequence(crashEff.m_strParticle, pDamageInstance->m_X - 1.0f + crashEff.m_fXOffset,
                pDamageInstance->m_Y - 0.5f + crashEff.m_fYOffset, 0.5f);

            if (pTemp != NULL && pDamageInstance->m_pOriginalDamage->m_vecCrashParticle[i].m_bSelfDirection) {
                // ���⼺ ����
                DIRECTION_INFO stDirect;
                stDirect.m_fDirPosX = 0;
                stDirect.m_bDirVelocity = true;
                stDirect.m_pbIsRight = &pDamageInstance->m_IsRight;
                pTemp->SetDirectInfo(&stDirect);
                if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                    pTemp->SetShow(true);
                else
                    pTemp->SetShow(false);
            }

        }
    }



    for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecAutoDamage.size(); ++i)
    {
        AUTO_DAMAGE& autoDamage = pDamageInstance->m_pOriginalDamage->m_vecAutoDamage[i];

        if (autoDamage.m_iDamageIndex == -1)
            break;

        bool bCreateDamage = false;

        if ((autoDamage.m_dwAutoDamage & TIME_AUTO_DAMAGE)) {
            if (autoDamage.m_dwAutoDamage != TIME_AUTO_DAMAGE)
            {
                if (pDamageInstance->CheckAlreadyDamaged(pDamageInstance->m_HitWho)
                    && (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE))
                {
                    continue;
                }
            }

            bCreateDamage = pDamageInstance->m_Life == autoDamage.m_iStartLifeTime;
        }

        if ((autoDamage.m_dwAutoDamage & BOUNDARY_CRASH_AUTO_DAMAGE) && CheckBoundary(pDamageInstance)) {
            bCreateDamage = true;
        }

        if (bCreateDamage)
        {
            D3DXVECTOR2 offset;
            offset.y = autoDamage.m_fOffSetY;
            if (pDamageInstance->m_IsRight)
            {
                offset.x = autoDamage.m_fOffSetX;
            }
            else
            {
                offset.x = -autoDamage.m_fOffSetX;
            }

            // ���䵥������ ������ ���� ������ �����̶� ���� �Ϸ��� (������ ǻ�ʷ�) 
            DIRECTION_INFO stDirect;
            stDirect.m_bIsRight = pDamageInstance->m_IsRight;

            CDamageInstance* pAuto;
            if (false == pDamageInstance->m_IsMonsterDamage)
            {
                pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, pDamageInstance->m_X + offset.x,
                    pDamageInstance->m_Y + offset.y, autoDamage.m_bNoSlowCount, -1, NULL, pDamageInstance->m_HitWho, &stDirect, pDamageInstance->m_emWhoType, pDamageInstance->m_emHitWhoType);
            }
            else
            {
                pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, pDamageInstance->m_X + offset.x,
                    pDamageInstance->m_Y + offset.y, autoDamage.m_bNoSlowCount, pDamageInstance->m_ExtraMonsterNum, NULL, pDamageInstance->m_HitWho, &stDirect);

                if (pAuto) {
                    pAuto->m_iTeam = pDamageInstance->m_iTeam;
                }
            }

            if (pAuto)
            {
                pAuto->m_bPetDamage = pDamageInstance->m_bPetDamage;
                pAuto->m_IsRight = pDamageInstance->m_IsRight;
                pAuto->m_SlowCount = 0;

                if (autoDamage.m_bDieCurDamage)
                    pDamageInstance->m_Life = 0;
            }

            if (autoDamage.m_bNoClear == false)
                autoDamage.m_dwAutoDamage = 0;
        }
    }

    for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecTimeSound.size(); ++i)
    {
        TIME_SOUND& timeSound = pDamageInstance->m_pOriginalDamage->m_vecTimeSound[i];

        if (timeSound.m_iSoundNumber == -1 || timeSound.m_iPlayTime == -1)
            continue;

        if (timeSound.m_iPlayTime == pDamageInstance->m_Life)
        {
            char strTemp[15];
            sprintf(strTemp, "%d", timeSound.m_iSoundNumber);
            g_KDSound.Play(strTemp, timeSound.m_bLoop);
        }
    }

    if (pDamageInstance->m_pOriginalDamage->m_iEarthQuakeRunFrame == pDamageInstance->m_Life) {
        if (pDamageInstance->m_pOriginalDamage->m_iEarthQuake != 0)
            g_kCamera.SetEarthQuake(pDamageInstance->m_pOriginalDamage->m_iEarthQuake);

        if (pDamageInstance->m_pOriginalDamage->m_iEarthQuakeY != 0)
            g_kCamera.SetEarthQuakeY(pDamageInstance->m_pOriginalDamage->m_iEarthQuakeY);
    }
}

void CDamageManager::CallEffAtGroundCrash(CDamageInstance* pDamageInstance)
{
    D3DXVECTOR3 vecCrashPos(0.0f, -1.0f, 0.0f);

    if (pDamageInstance->m_bGroundCrash)
    {
        if (0.0f != pDamageInstance->m_pOriginalDamage->m_fGround_Offset_X)
            pDamageInstance->m_X -= pDamageInstance->m_pOriginalDamage->m_fGround_Offset_X;
        if (0.0f != pDamageInstance->m_pOriginalDamage->m_fGround_Offset_Y)
            pDamageInstance->m_Y -= pDamageInstance->m_pOriginalDamage->m_fGround_Offset_Y;
    }

    if (pDamageInstance->m_pOriginalDamage->m_bCheckBoundary)
    {
        CheckBoundary(pDamageInstance);
    }

    if (pDamageInstance->m_pOriginalDamage->m_kBoundaryReflection.m_bSetStruct)
    {
        ReflectionBoundary(pDamageInstance);
        if (pDamageInstance->m_pOriginalDamage->m_kBoundaryReflection.m_bNoCheckDiffGround)
            return;
    }

    if (SiKGCRoomManager()->IsMonsterGameMode())
    {
        float fPosY;
        if (pDamageInstance->m_pOriginalDamage->m_bIsGroundBottomBox)
        {
            fPosY = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x;
        }
        else
        {
            fPosY = pDamageInstance->m_Y;
        }

        D3DXVECTOR2 dxvCrashPos;
        if (pDamageInstance->m_What == DT_PERIOT_HOMMING_MISSILE_HP ||
            pDamageInstance->m_What == DT_SUPERMON_PERIOT_HOMMING_MISSILE_HP ||
            pDamageInstance->m_What == DT_PERIOT_HOMMING_MISSILE_DESTROY)
        {
            SiKGCFantasticMap()->CrashCheck(pDamageInstance->m_X, fPosY, dxvCrashPos, 0x0000008 + 0x00000004);
        }
        else
            SiKGCFantasticMap()->CrashCheck(pDamageInstance->m_X, fPosY, dxvCrashPos);

        vecCrashPos.x = dxvCrashPos.x;
        vecCrashPos.y = dxvCrashPos.y;
    }
    else
    {
        float fPosY;
        if (pDamageInstance->m_pOriginalDamage->m_bIsGroundBottomBox)
        {
            fPosY = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x;
        }
        else
        {
            fPosY = pDamageInstance->m_Y;
        }

        vecCrashPos = g_MyD3D->MyStg->Check_Contact(pDamageInstance->m_X, fPosY, 0);
    }

    if (vecCrashPos.y != -1.0f)
    {
        bool bForceCrash = false;
        pDamageInstance->m_bGroundCrash = true;
        pDamageInstance->m_fAccGravity = 0.0f;

        if (SiKGCRoomManager()->IsMonsterGameMode())
        {
            bForceCrash = SiKGCFantasticMap()->IsLastBottomPosition(vecCrashPos.x, vecCrashPos.y);
        }
        else
        {
            bForceCrash = g_MyD3D->MyStg->IsLowStandBlock(vecCrashPos.x, vecCrashPos.y);
        }

        if (!bForceCrash)
        {
            if (pDamageInstance->m_pOriginalDamage->m_iNoCheckContactDamage != -1
                && pDamageInstance->m_pOriginalDamage->m_iNoCheckContactDamage < pDamageInstance->m_Life)
                return;
        }

        if (pDamageInstance->m_pOriginalDamage->m_bCheckNoPassFloor) {
            if (SiKGCRoomManager()->IsMonsterGameMode()) {
                if (SiKGCFantasticMap()->IsAvailableGoDown(vecCrashPos.x, vecCrashPos.y) ||
                    SiKGCFantasticMap()->IsAvailableJumpUp(vecCrashPos.x, vecCrashPos.y)) {
                    return;
                }
            }
            else {
                if (false == g_MyD3D->MyStg->Check_Wall(pDamageInstance->m_X, pDamageInstance->m_Y, 3))
                {
                    if (g_MyD3D->MyStg->IsEnableNoCheck(vecCrashPos.x, vecCrashPos.y) ||
                        g_MyD3D->MyStg->IsLowStandBlock(vecCrashPos.x, vecCrashPos.y)) {
                        return;
                    }
                }
            }
        }

        if (pDamageInstance->m_pOriginalDamage->m_bStopAtContact)
        {
            pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.0f;
            pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
            pDamageInstance->m_pOriginalDamage->m_fGravity = 0.0f;
        }

        if (pDamageInstance->m_pOriginalDamage->m_bStopAtLastBottomContact)
        {
            if (2 < pDamageInstance->m_Life && CheckLastBottom(vecCrashPos.x, vecCrashPos.y))
            {
                pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.0f;
                pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
                pDamageInstance->m_pOriginalDamage->m_fGravity = 0.0f;
            }
        }

        if (2 < pDamageInstance->m_Life && pDamageInstance->m_pOriginalDamage->m_bEndAtContact)
        {
            pDamageInstance->m_Life = 2;
        }

        bool bNoCheckParticleGroundCheck = false;
        if (pDamageInstance->m_pOriginalDamage->m_bEndAtLastBottomContact)
        {
            if (2 < pDamageInstance->m_Life && CheckLastBottom(vecCrashPos.x, vecCrashPos.y))
            {
                pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.0f;
                pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
                pDamageInstance->m_pOriginalDamage->m_fGravity = 0.0f;
                pDamageInstance->m_Life = 2;
                bNoCheckParticleGroundCheck = true;
            }
        }

        for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecCrashParticle.size(); ++i)
        {
            CRASH_PARTICLE& crashEff = pDamageInstance->m_pOriginalDamage->m_vecCrashParticle[i];

            if (false == bNoCheckParticleGroundCheck)
                if (!crashEff.m_bGroundCheck)
                    continue;

            if (true == crashEff.m_bRenderToContactPosition)
            {
                vecCrashPos.x = pDamageInstance->m_X;
                vecCrashPos.y = pDamageInstance->m_Y;
            }

            CParticleEventSeqPTR pTemp;
            pTemp = pDamageInstance->CreateSequence(crashEff.m_strParticle, vecCrashPos.x - 1.0f + crashEff.m_fXOffset,
                vecCrashPos.y - 0.5f + crashEff.m_fYOffset, 0.5f);

            if (pTemp != NULL && pDamageInstance->m_pOriginalDamage->m_vecCrashParticle[i].m_bSelfDirection) {
                // ���⼺ ����
                DIRECTION_INFO stDirect;
                stDirect.m_fDirPosX = 0;
                stDirect.m_bDirVelocity = true;
                stDirect.m_pbIsRight = &pDamageInstance->m_IsRight;
                pTemp->SetDirectInfo(&stDirect);
                if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                    pTemp->SetShow(true);
                else
                    pTemp->SetShow(false);
            }
        }

        for (int i = 0; i < (int)pDamageInstance->m_pOriginalDamage->m_vecAutoDamage.size(); ++i)
        {
            AUTO_DAMAGE& autoDamage = pDamageInstance->m_pOriginalDamage->m_vecAutoDamage[i];

            if (autoDamage.m_iDamageIndex == -1)
                break;

            if (false == (autoDamage.m_dwAutoDamage & GROUND_CRASH_AUTO_DAMAGE) &&
                false == (autoDamage.m_dwAutoDamage & END_GROUND_CRASH_AUTO_DAMAGE))
                continue;

            if ((autoDamage.m_dwAutoDamage & END_GROUND_CRASH_AUTO_DAMAGE) &&
                (false == CheckLastBottom(vecCrashPos.x, vecCrashPos.y)))
                continue;

            // ���� �������� ��üũ�� ������, �� ������ ��ü�� �� üũ ���� �������� ������ �ִٸ� �켱���� ���������� �ش�.
            if ((autoDamage.m_dwAutoDamage & GROUND_CRASH_AUTO_DAMAGE) &&
                (pDamageInstance->m_pOriginalDamage->m_iNoCheckContactDamage != -1 && pDamageInstance->m_pOriginalDamage->m_iNoCheckContactDamage < pDamageInstance->m_Life))
                continue;

            if (autoDamage.m_iCrashStartTime != -1 && pDamageInstance->m_Life > autoDamage.m_iCrashStartTime)
                continue;

            D3DXVECTOR2 offset;
            offset.y = autoDamage.m_fOffSetY;
            if (pDamageInstance->m_IsRight)
            {
                offset.x = autoDamage.m_fOffSetX;
            }
            else
            {
                offset.x = -autoDamage.m_fOffSetX;
            }

            CDamageInstance* pAuto;
            if (false == pDamageInstance->m_IsMonsterDamage)
            {
                pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, pDamageInstance->m_X + offset.x,
                    pDamageInstance->m_Y + offset.y, autoDamage.m_bNoSlowCount, -1, NULL, pDamageInstance->m_HitWho);
            }
            else
            {
                pAuto = AddWithLocate(autoDamage.m_iDamageIndex, pDamageInstance->m_Who, pDamageInstance->m_X + offset.x,
                    pDamageInstance->m_Y + offset.y, autoDamage.m_bNoSlowCount, pDamageInstance->m_ExtraMonsterNum, NULL, pDamageInstance->m_HitWho);
            }

            if (pAuto)
            {
                pAuto->m_bPetDamage = pDamageInstance->m_bPetDamage;
                pAuto->m_IsRight = pDamageInstance->m_IsRight;
                pAuto->m_SlowCount = 0;

            }

            //���ǿ�������� ������ ���� autoDamage.m_bDieCurDamage �̰� üũ ����.(To.. ��.... ������... ��...��!!!!!!!!!!!?????�Ф� ���ϴԾ�,.,.,.��,,,)				
            if (false == pDamageInstance->m_pOriginalDamage->m_bAutoDamageToGroundCrashOfNoDieCurDamage)
                pDamageInstance->m_Life = 1;

            if (autoDamage.m_bNoClear == false)
                autoDamage.m_dwAutoDamage = 0;
        }

        switch (pDamageInstance->m_What)
        {
        case DT_BLOCK_BUSTER:
            if (pDamageInstance->m_Life < 220)
            {
                // ���� ����Ʈ�� �߻���Ų��.
                g_KDSound.Play("43");
                AddWithLocate(DT_BOMB_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
                AddWithLocate(DT_ARCHIMIST_BUSTER_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
                g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f);

                pDamageInstance->m_Y = vecCrashPos.y;
                pDamageInstance->m_Life = 1;

            }
            break;
        case DT_RED_CANNON:
        case DT_YELLOW_CANNON:
        {
            //1�� �ȿ��� ���ǰ� ��Ƶ� �������� �ʴ´�.
            if (pDamageInstance->m_Life > 200)
                return;

            // ���� ����Ʈ�� �߻���Ų��.
            g_KDSound.Play("43");

            CDamageInstance* fire;

            //if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
            //{
            //    pDamageInstance->m_Who = MAX_PLAYER_NUM;
            //    pDamageInstance->m_iTeam = ETMonster;
            //}

            fire = AddWithLocate(DT_BOMB_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true, pDamageInstance->m_ExtraMonsterNum);
            fire->m_iTeam = pDamageInstance->m_iTeam;
            fire->m_IsRight = pDamageInstance->m_IsRight;
            fire = AddWithLocate(DT_CANNON_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true, pDamageInstance->m_ExtraMonsterNum);
            fire->m_iTeam = pDamageInstance->m_iTeam;
            fire->m_IsRight = pDamageInstance->m_IsRight;
            g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f);

            // ���� ����Ʈ�� ������ ���ϰ� �����.
            pDamageInstance->m_Life = 1;

        }
        break;
        case DT_MONGBAN_CANNON:
        {
            //1�� �ȿ��� ���ǰ� ��Ƶ� �������� �ʴ´�.
            if (pDamageInstance->m_Life > 200)
                return;

            // ���� ����Ʈ�� �߻���Ų��.
            g_KDSound.Play("43");
            AddWithLocate(DT_BOMB_FIRE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, pDamageInstance->m_ExtraMonsterNum);
            AddWithLocate(DT_BOMB_WAVE, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, false, pDamageInstance->m_ExtraMonsterNum);
            g_MyD3D->MySparks->Fires(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f);

            // ���� ����Ʈ�� ������ ���ϰ� �����.
            pDamageInstance->m_Life = 1;

        }
        break;
        case DT_BOMB:
        case DT_MONSTER_MINE:
        case DT_CHEROKI_BOMB:
            pDamageInstance->m_Y = vecCrashPos.y;
            break;
        case DT_BOMBMAN_01:
            // ���� ����Ʈ�� �߻���Ų��.
            //g_KDSound.Play( "43" );
            if (pDamageInstance->m_Life < 75)
            {
                AddWithLocate(DT_BOMBMAN_BOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true, pDamageInstance->m_ExtraMonsterNum);
                pDamageInstance->m_Life = 1;
                g_KDSound.Play("43");
            }


            break;
        case DT_DRILLMON_ROCK:
            g_KDSound.Play("43");
            EffectDust(pDamageInstance->m_X - 1.0f, vecCrashPos.y - 0.2f, pDamageInstance);
            EffectDust(pDamageInstance->m_X - 1.1f, vecCrashPos.y - 0.2f, pDamageInstance);
            EffectDust(pDamageInstance->m_X - 0.9f, vecCrashPos.y - 0.2f, pDamageInstance);
            pDamageInstance->m_Life = 1;
            break;
        case DT_ARME3_FIRESPOT:
            if (pDamageInstance->m_Life < 75)
            {
                // ���� ����Ʈ�� �߻���Ų��.
                g_KDSound.Play("43");
                pDamageInstance->m_Y = vecCrashPos.y;
                pDamageInstance->m_Life = 1;
                AddWithLocate(DT_ARME3_FIRECRASH1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.2f);
                AddWithLocate(DT_ARME3_FIRECRASH2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.2f);
                AddWithLocate(DT_ARME3_BURSTFIRE3, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.12f);
                AddWithLocate(DT_ARME3_BURSTFIRE2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.12f);
                AddWithLocate(DT_ARME3_BURSTFIRE1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y + 0.12f);
            }
            break;

        case DT_RYAN3_SEED:
        {
            if (pDamageInstance->m_Life < 16)
            {
                pDamageInstance->m_Y = vecCrashPos.y;
                pDamageInstance->m_Life = 1;
                AddWithLocate(DT_RYAN3_SEED_BLOOM, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
            }
        }
        break;
        case DT_VANESSA_ATK3:
        {
            if (pDamageInstance->m_Life < 16)
            {
                pDamageInstance->m_Y = vecCrashPos.y;
                pDamageInstance->m_Life = 1;
                MonsterAddWithLocate(DT_VANESSA_SEED_BLOOM, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X, pDamageInstance->m_Y);
            }
        }
        break;

        case DT_RYAN4_ONE_HAND_SPECIAL3_SPEAR_R:
        {
            if (pDamageInstance->m_Life < pDamageInstance->m_pOriginalDamage->m_Start_Life - 14)
            {
                pDamageInstance->m_Y = vecCrashPos.y;
                pDamageInstance->m_Life = 1;
                if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                {
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_07_R", pDamageInstance->m_X - 0.95f, pDamageInstance->m_Y - 0.5f, 0.5f);
                }
                AddWithLocate(DT_RYAN4_ONE_HAND_SPECIAL3_EXPLOSION, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
                g_KDSound.Play("628");
            }
        }
        break;
        case DT_RYAN4_ONE_HAND_SPECIAL3_SPEAR_L:
        {
            if (pDamageInstance->m_Life < pDamageInstance->m_pOriginalDamage->m_Start_Life - 14)
            {
                pDamageInstance->m_Y = vecCrashPos.y;
                pDamageInstance->m_Life = 1;
                if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                {
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                    pDamageInstance->CreateSequence("Ryan04_special2-3_boom_07_L", pDamageInstance->m_X - 1.05f, pDamageInstance->m_Y - 0.5f, 0.5f);
                }
                AddWithLocate(DT_RYAN4_ONE_HAND_SPECIAL3_EXPLOSION, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
                g_KDSound.Play("628");
            }
        }
        break;
        case DT_POISONSPIDER_POISON:
        case DT_BLOODYORC_UWEK_1:
        {
            // �׳� ���� �� �ϰ� �ؼ� ���� ó���� �Ѵ� 
            pDamageInstance->m_Y = vecCrashPos.y;
            pDamageInstance->m_Life = 1;
            MonsterAddWithLocate(DT_POISONSPIDER_POISON_GAS, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X, pDamageInstance->m_Y);
        }
        break;
        case DT_FIRETRAP:
        case DT_THORNTRAP:
        case DT_HOLYTRAP:
        case DT_RONAN_MAGIC_SPECIAL2_FIRE:
        case DT_RONAN_MAGIC_SPECIAL2_FIRE_LV2:
        case DT_RONAN_MAGIC_SPECIAL2_FIRE_LV3:
        case DT_MONSTER_MAGMON_BLAZE:
            //case DT_SKILL_LAS_SPIKE_TRAP:
        case DT_SKILL_LAS_CANON_TRAP:
        case DT_DRAKEWORM_FLAME_SEA:
        case DT_MARI2_BUILDING_LV1_JUMP_TRAP:
        case DT_LEY1_FLAME_ROLL_FIRE:
        case DT_ARME4_SPECIAL4_HUGE_METEO_FIRE:
        case DT_ARME4_SPECIAL4_HUGE_METEO_FIRE_EVENT_MON:
        {
            //====================================================================================
            // 2009.02.10 : Jemitgge
            // Comment : ���� ������ ���� ��������.

            pDamageInstance->m_Y = vecCrashPos.y;
            pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
        }
        break;
        case DT_SKILL_RYAN_HP_TOTEM:
        case DT_SKILL_RYAN_MP_TOTEM:
        case DT_SKILL_RYAN_DP_TOTEM:
        case DT_AMY_DIVINE_MESSAGE_HEAL:
        case DT_AMY_DIVINE_MESSAGE_FAKE:
        case DT_SPIDER_GUARD_BOMB:
        case DT_SPIDER_GUARD_BOMB_R:
        case DT_SPIDER_GUARD_BOMB1:
        case DT_SPIDER_GUARD_BOMB1_R:
        {
            //====================================================================================
            // 2009.02.10 : Jemitgge
            // Comment : ���� ������ ���� ��������.

            pDamageInstance->m_Y = vecCrashPos.y + 0.1f;
            pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
        }
        break;
        case DT_SKILL_LAS_TRADWARM_POT:
        case DT_SKILL_LAS_TRADWARM_LV2_SCROOL:
        case DT_SKILL_LAS_TRADWARM_LV2_SCROOL_DUN:
        case DT_TALIN_SUPERFRAME_LAUNCHER:
        case DT_TALIN_SUPERFRAME_LAUNCHER_BOTTOM:
        {
            pDamageInstance->m_Y = vecCrashPos.y + 0.17f;
            pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;

            if (pDamageInstance->m_What == DT_SKILL_LAS_TRADWARM_POT)
            {
                if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                {
                    g_ParticleManager->CreateSequence("Las01_Trad_Warm_LV1_Bottom_01", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
                }
                g_kCamera.SetEarthQuake(20);
                g_KDSound.Play("1446");
            }
        }
        break;
        case DT_DAMAGE_AMPLIFIER_LV1:
        case DT_DAMAGE_AMPLIFIER_LV1_DUN:
        {
            g_KDSound.Play("1335");
            pDamageInstance->m_Life = 1;
        }
        break;

        case DT_AMY4_CANNON_BALL:
        {
            CDamageInstance* pDmg = AddWithLocate(DT_AMY4_CANNON_BALL_GROUND, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true);
            pDmg->m_Life = pDamageInstance->m_Life;
            pDamageInstance->m_Life = 1;
        }
        break;
        case DT_PERIOT_HOMMING_MISSILE_HP:
        case DT_SUPERMON_PERIOT_HOMMING_MISSILE_HP:
        {
            g_ParticleManager->CreateSequence("Pariot_Lifecircle_Spark_01", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            g_ParticleManager->CreateSequence("Pariot_Lifecircle_Spark_02", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            g_ParticleManager->CreateSequence("Pariot_Lifecircle_Spark_03", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            pDamageInstance->m_Life = 1;
        }
        break;
        case DT_PERIOT_HOMMING_MISSILE_DESTROY:
        {
            g_ParticleManager->CreateSequence("Pariot_Destroycircle_Spark_01", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            g_ParticleManager->CreateSequence("Pariot_Destroycircle_Spark_02", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            g_ParticleManager->CreateSequence("Pariot_Destroycircle_Spark_03", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            g_ParticleManager->CreateSequence("Pariot_Destroycircle_Spark_04", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            pDamageInstance->m_Life = 1;
        }
        break;
        case DT_TECHNICAL_GUARD_ATK3:
        {
            pDamageInstance->m_Y = vecCrashPos.y;
            pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
            pDamageInstance->m_Life = 1;
            g_ParticleManager->CreateSequence("TGuard_Glue_shot", pDamageInstance->m_X - 1.0f, vecCrashPos.y + 0.05f - 0.48f, 0.5f);
            CDamageInstance* pDmg = AddWithLocate(DT_TECHNICAL_GUARD_ATK3_BOMB, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y, true, pDamageInstance->m_ExtraMonsterNum);
        }
        break;
        case DT_HELL_DEATHWORM_STONE:
        {
            g_KDSound.Play("985015");
        }
        break;
        default:
            pDamageInstance->m_pOriginalDamage->OnGroundCrash(pDamageInstance, true);
            break;
        }
    }
    else
    {
        pDamageInstance->m_pOriginalDamage->OnGroundCrash(pDamageInstance, false);
    }

    if (pDamageInstance->m_bGroundCrash)
    {
        if (0.0f != pDamageInstance->m_pOriginalDamage->m_fGround_Offset_X)
            pDamageInstance->m_X += pDamageInstance->m_pOriginalDamage->m_fGround_Offset_X;
        if (0.0f != pDamageInstance->m_pOriginalDamage->m_fGround_Offset_Y)
            pDamageInstance->m_Y += pDamageInstance->m_pOriginalDamage->m_fGround_Offset_Y;
    }
}

void CDamageManager::CallAnim(CDamageInstance* pDamageInstance, std::string strAnim, float fScaleX, float fScaleY)
{
    CKTDGAnim::CAnimInstance* pAnim;
    pAnim = g_MyD3D->m_pMapAnim->CreateInst(g_MyD3D->m_pMapAnim->GetSequence(strAnim));

    if (pAnim != NULL)
    {
        pAnim->GetMatrix()->Move(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.504f, 0.5f);
        if (pDamageInstance->m_IsRight)
            pAnim->GetMatrix()->Scale(fScaleX, fScaleY, 1.0f);
        else
            pAnim->GetMatrix()->Scale(-fScaleX, fScaleY, 1.0f);

        pAnim->Start(CKTDGAnim::PT_ONE_PASS_AUTO_DIE);
        if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
            pAnim->SetShow(true);
        else
            pAnim->SetShow(false);
    }
}


bool CDamageManager::MakeDamage(int What, CDamageInstance* pDamageInstance)
{
    if (What < 0 || static_cast<int>(m_vecInfo.size()) <= What)
        return false;

    switch (What)
    {
    case DT_PET_SMALL_MAID_THROW_MONSTER:
    case DT_PET_MAID_THROW_MONSTER:
    case DT_PET_SMALL_MAID_THROW_PLAYER:
    case DT_PET_MAID_THROW_PLAYER:
    case DT_PET_CHRISTMAS_MAID_THROW_PLAYER:
    case DT_PET_MASHIMARO_THROW_PLAYER:
    case DT_PET_MASHIMARO_THROW_MONSTER:
    case DT_ARME_SKILL_HOLDBEAM_FIRE:
    case DT_ARME_SKILL_HOLDBEAM_FIRE_LV2:
    case DT_SAKU_ATTACK:
    case DT_PET_RICA_LASER_SHOT:
    case DT_PET_RICA_LASER_SHOT_R:
    case DT_RONAN_PARALYZE_BEAM:
    case DT_BUZZY_BUBBLEGUM_THROW:
    case DT_BUZZY_BUBLEGUM_BOMB:
    case DT_BUBBLEGUM_BOMB_MISSILE:
    case DT_THANATOS2_VAMPIRE_R:
    case DT_THANATOS2_VAMPIRE_L:
    case DT_THANATOS3_TAKEUNDERCONTROL:
    case DT_PET_SEAMUS_HOLD:
        //    case DT_PET_KITTY_SKILL_1:
    case DT_HERO_ICEGOLEM_ATK2:
    case DT_HERO_KAMIKI_ATK2_HOLD:
    case DT_KARUEL_MON_GUARDIANGOLEM_ATK3_2:
    case DT_KARUEL_MON_GUARDIANGOLEM_ATK3_2_EFF:
    case DT_DIO4_DEVIL_EVILSHOCK_TYPE1_LV1_DMG:
    case DT_DIO4_DEVIL_EVILSHOCK_TYPE1_LV2_DMG:
    case DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV1_1_HOLD:
    case DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV1_2_HOLD:
    case DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV1_3_HOLD:
    case DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV2_1_HOLD:
    case DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV2_2_HOLD:
    case DT_DIO4_DEVIL_EVILEDDY_TYPE1_LV2_3_HOLD:
    case DT_BEIGAS1_SPECIAL_TYPE5_LV1_HOLD_RANGE:
    case DT_PET_TRICKY_PLAYER_ATK:
    case DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK2_HOLD:
    case DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK2_HOLD:
        //case DT_DIO_SPECIAL4_TYPE2_DMG:
        pDamageInstance->m_pOriginalDamage = new CHardDamageHold();
        break;
        //case DT_GORGOS_FIRE:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageGorgosFire();
        //    break;
    case DT_WATER_POLE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageWaterPole();
        break;
        //case DT_DRILLMON_ROCK:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageDrillRock();
        //    break;
    case DT_BOMB:
    case DT_CHEROKI_BOMB:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBomb();
        break;
    case DT_BOMB2:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBomb2();
        break;
    case DT_FIRE_LANCE:
    case DT_NAGASIMON03:
        pDamageInstance->m_pOriginalDamage = new CHardDamageFireLance();
        break;
        //case DT_MINE:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageMine();
        //    break;
    case DT_FLING_SHILED:
        pDamageInstance->m_pOriginalDamage = new CHardDamageFlingShield();
        break;
        //case DT_BLOCK_BUSTER:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageBlockBuster();
        //    break;
    case DT_GUARDIAN_STRIKE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageGuardianStrike();
        break;
    case DT_GUARDIAN_BALL:
    case DT_GUARDIAN_BALL_DUN:
        pDamageInstance->m_pOriginalDamage = new CHardDamageGuardianBall();
        break;
        //case DT_ENT_TRUNK:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageEntTrunk();
        break;
    case DT_FIRE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageFire();
        break;
        //case DT_GOLEM_FIRE:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageGolemFire();
        //    break;
        //case DT_GOLEM_BODY_FIRE:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageGolemBodyFire();
        //    break;

    case DT_THROW_MANY_FRUIT1:
    case DT_THROW_MANY_FRUIT2:
    case DT_THROW_MANY_FRUIT3:
    case DT_THROW_MANY_FRUIT4:
        pDamageInstance->m_pOriginalDamage = new CHardDamageManyFruit();
        break;
    case DT_KAZEAZE_DARKBALL:
        pDamageInstance->m_pOriginalDamage = new CHardDamageKazeazeDarkBall();
        break;
    case DT_MAGIC_STAR:
    case DT_MAGIC_STAR_DUN:
    case DT_ARME3_TORNADO3:
    case DT_STONE_GOLEM_TORNADO3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageMagicStar();
        break;
    case DT_BASILISK_FIRE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBasiliskFire();
        break;

        //case DT_BOMB_WAVE:
        //case DT_CANNON_WAVE:
        //case DT_ARCHIMIST_BUSTER_WAVE:    
        //case DT_CHEROKI_BOMB_WAVE:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageBombWave();
        //    break;
    case DT_GAS:
    case DT_DARK_POISON_GAS:
    case DT_CASTUL_TRAP_GAS:
    case DT_CASTUL_TRAP_GAS2:
    case DT_BERMESIA_FINAL_GAS_LV1:
    case DT_BERMESIA_FINAL_GAS_LV2:
    case DT_BERMESIA_FINAL_GAS_LV3:
    case DT_BERMESIA_FINAL_GAS_LV4:
    case DT_BERMESIA_FINAL_GAS_LV5:
    case DT_RENASIEN_GAS:
    case DT_ANTOINETTE_POISON_GAS:
    case DT_PLANT_MINI_DEAD_ATK:
    case DT_PET_KUMI_ATTACK_1:
        pDamageInstance->m_pOriginalDamage = new CHardDamageGas();
        break;
    case DT_CASTUL_TRAP_WHOLE_STAGE_GAS:
        pDamageInstance->m_pOriginalDamage = new CHardDamageWholeStageGas();
        break;
    case DT_LIGHTNING_BOLT:
    case DT_LIGHTNING_BOLT_BLUE:
    case DT_ARME_SKILL_SHOCKSTURN_LV2_ATK1:
    case DT_ARME_SKILL_SHOCKSTURN_LV1_ATK1_DUN:
    case DT_HATSHEPSUT_LIGHTNING_BOLT:
    case DT_ARME_SKILL_SHOCKSTURN_LV2_EFFECT:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLightningBolt();
        break;

    case DT_LIGHTNING_GENERATE1:
    case DT_LIGHTNING_GENERATE1_BLUE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLightningGenerate1();
        break;
    case DT_LIGHTNING_GENERATE2:
    case DT_LIGHTNING_GENERATE2_BLUE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLightningGenerate2();
        break;
    case DT_KAMIKI_LIGHTNING_WAVE:
    case DT_MONSTER_KAMIKI07:
    case DT_MONSTER_YONGWANG_BEAM:
    case DT_MONGBAN_LIGHTING_FIELD:
        pDamageInstance->m_pOriginalDamage = new KHardDamageKamikiLightning();
        break;

    case DT_DARK_STONECURSE:     // ����Ŀ��(��) - �������̰� ����Ѵ�.
    case DT_DARK_STONECURSE_ICE: //����Ŀ�� ����
    case DT_STONE_CURSE:
    case DT_ARME3_ICE_STORM:
    case DT_ICE_BREATH:
    case DT_ARME3_ICE_STONE_CURSE:
    case DT_ICE_GOLEM_BREATH:
    case DT_BEHOLDER_STONE_CURSE:
    case DT_ARME_SKILL_STONE_CURSE_LV2_DAMAGE:
    case DT_SPIDERQUEEN_WEB_BODY:
    case DT_GIANT_STONE_RING:
    case DT_GARGOYLE_STONEBREATH:
#if !defined( NATION_KOREA ) && !defined( NATION_LATINAMERICA )
    case DT_PET_MEDUSA_ATTACT1:
#endif
        pDamageInstance->m_pOriginalDamage = new CHardDamageStoneCurse();
        break;

    case DT_METEO:
    case DT_METEO_ICE:
    case DT_METEO_SHINING:
    case DT_METEO_LOVE:
    case DT_METEO_LIFE:
    case DT_WIND_BLADE1:
    case DT_WIND_BLADE2:
    case DT_WIND_BLADE3:
    case DT_METEO_ELIA:
    case DT_METEO_C_MASTER:
    case DT_METEO_COLISEU:
        pDamageInstance->m_pOriginalDamage = new CHardDamageMeteo();
        break;

    case DT_FREEWAY_ARROW:                      //�밢�� ȭ��
    case DT_FREEWAY_ARROW_LIGHTNING:            //�밢�� ȭ��    ���� �ü��� ��
    case DT_FREEWAY_ARROW_SHINING:              //�밢�� ȭ��    �� �ü��� ��
    case DT_FREEWAY_ARROW_LOVE:
    case DT_FREEWAY_ARROW_DUN:                      //�밢�� ȭ��
    case DT_FREEWAY_ARROW_LIGHTNING_DUN:            //�밢�� ȭ��    ���� �ü��� ��
    case DT_FREEWAY_ARROW_SHINING_DUN:              //�밢�� ȭ��    �� �ü��� ��
    case DT_FREEWAY_ARROW_LOVE_DUN:
    case DT_ARROW:                              //ȭ��
    case DT_ARROW_LIGHTNING:                    //ȭ��            ���� �ü� ��
    case DT_ARROW_SHINING:                      //ȭ��            �� �ü� ��
    case DT_ARROW_LOVE:
    case DT_LAS_JUMP2_ATTACK:                   //ȭ�� ȸ��
    case DT_HUGE_ARROW:
    case DT_HUGE_ARROW_LIGHTNING:
    case DT_HUGE_ARROW_SHINING:
    case DT_HUGE_ARROW_LOVE:
    case DT_HUGE_JUMPDOWN_ARROW:
    case DT_HUGE_JUMPDOWN_ARROW_LIGHTNING:
    case DT_HUGE_JUMPDOWN_ARROW_SHINING:
    case DT_HUGE_JUMPDOWN_ARROW_LOVE:
    case DT_SKULL_ARROW:
        // ��� ȭ���..
    case DT_NOVA_BOLT:
    case DT_NOVA_FREEWAY_BOLT:
    case DT_NOVA_FREEWAY_BOLT_DUN:
    case DT_NOVA_ARROW:
//    case DT_NOVA_SPECIAL1_BOLT:
    case DT_NOVA_BOLT_LIGHTNING:
    case DT_NOVA_FREEWAY_BOLT_LIGHTNING:
    case DT_NOVA_FREEWAY_BOLT_LIGHTNING_DUN:
    case DT_NOVA_ARROW_LIGHTNING:
//    case DT_NOVA_SPECIAL1_BOLT_LIGHTNING:
    case DT_NOVA_BOLT_SHINING:
    case DT_NOVA_FREEWAY_BOLT_SHINING:
    case DT_NOVA_FREEWAY_BOLT_SHINING_DUN:
    case DT_NOVA_ARROW_SHINING:
//    case DT_NOVA_SPECIAL1_BOLT_SHINING:
    case DT_NOVA_BOLT_DARK:
    case DT_NOVA_FREEWAY_BOLT_DARK:
    case DT_NOVA_FREEWAY_BOLT_DARK_DUN:
    case DT_NOVA_ARROW_DARK:
//    case DT_NOVA_SPECIAL1_BOLT_DARK:
    case DT_NOVA_BOLT_LOVE:
    case DT_NOVA_FREEWAY_BOLT_LOVE:
    case DT_NOVA_FREEWAY_BOLT_LOVE_DUN:
    case DT_NOVA_ARROW_LOVE:
//    case DT_NOVA_SPECIAL1_BOLT_LOVE:
    case DT_NOVA_DOWN_ARROW:
    case DT_NOVA_DOWN_ARROW_LIGHTNING:
    case DT_NOVA_DOWN_ARROW_SHINING:
    case DT_NOVA_DOWN_ARROW_DARK:
    case DT_NOVA_DOWN_ARROW_LOVE:
        // ��ųƮ�� - �ü�
 //   case DT_FIRE_ARROW:
//    case DT_POISON_ARROW:
//    case DT_FROZEN_ARROW:
//    case DT_HEADSHOT_ARROW:
//    case DT_HEADSHOT_ARROW_LV2:
//    case DT_CONTINUOUS_ARROW:
//    case DT_CONTINUOUS_ARROW_LOVE:
//    case DT_CONTINUOUS_ARROW_SHINNING:
//    case DT_CONTINUOUS_ARROW_LIGHTNING:

    case DT_NOVA_BOLT_DEATH:
    case DT_NOVA_FREEWAY_BOLT_DEATH:
    case DT_NOVA_FREEWAY_BOLT_DEATH_DUN:
    case DT_NOVA_ARROW_DEATH:
//    case DT_NOVA_SPECIAL1_BOLT_DEATH:
    case DT_NOVA_DOWN_ARROW_DEATH:

    case DT_ARROW_TIME:
    case DT_CONTINUOUS_ARROW_TIME:
    case DT_FREEWAY_ARROW_TIME:
    case DT_FREEWAY_ARROW_TIME_DUN:
    case DT_HUGE_ARROW_TIME:
    case DT_HUGE_JUMPDOWN_ARROW_TIME:
    case DT_NOVA_BOLT_TIME:
    case DT_NOVA_ARROW_TIME:
    case DT_NOVA_FREEWAY_BOLT_TIME:
    case DT_NOVA_FREEWAY_BOLT_TIME_DUN:
//    case DT_NOVA_SPECIAL1_BOLT_TIME:
    case DT_ARROW_LIFE:
    case DT_CONTINUOUS_ARROW_LIFE:
    case DT_FREEWAY_ARROW_LIFE:
    case DT_FREEWAY_ARROW_LIFE_DUN:
    case DT_HUGE_ARROW_LIFE:
    case DT_HUGE_JUMPDOWN_ARROW_LIFE:

    case DT_NOVA_BOLT_LIFE:
    case DT_NOVA_FREEWAY_BOLT_LIFE:
    case DT_NOVA_FREEWAY_BOLT_LIFE_DUN:
    case DT_NOVA_ARROW_LIFE:
//    case DT_NOVA_SPECIAL1_BOLT_LIFE:

    case DT_ARROW_DARK_DRAGON_RED:
    case DT_CONTINUOUS_ARROW_DARK_DRAGON_RED:
    case DT_FREEWAY_ARROW_DARK_DRAGON_RED:
    case DT_FREEWAY_ARROW_DARK_DRAGON_RED_DUN:
    case DT_HUGE_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_RED_DUN:
//    case DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED:

    case DT_ARROW_DARK_DRAGON_BLUE:
    case DT_CONTINUOUS_ARROW_DARK_DRAGON_BLUE:
    case DT_FREEWAY_ARROW_DARK_DRAGON_BLUE:
    case DT_FREEWAY_ARROW_DARK_DRAGON_BLUE_DUN:
    case DT_HUGE_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_JUMPDOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_FREEWAY_BOLT_DARK_DRAGON_BLUE_DUN:
//    case DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE:

    case DT_ARROW_HALLOWEEN:
    case DT_CONTINUOUS_ARROW_HALLOWEEN:
    case DT_FREEWAY_ARROW_HALLOWEEN:
    case DT_FREEWAY_ARROW_HALLOWEEN_DUN:
    case DT_HUGE_ARROW_HALLOWEEN:
    case DT_HUGE_JUMPDOWN_ARROW_HALLOWEEN:
    case DT_NOVA_BOLT_HALLOWEEN:
    case DT_NOVA_ARROW_HALLOWEEN:
    case DT_NOVA_FREEWAY_BOLT_HALLOWEEN:
    case DT_NOVA_FREEWAY_BOLT_HALLOWEEN_DUN:
//    case DT_NOVA_SPECIAL1_BOLT_HALLOWEEN:

    case DT_ARROW_ELIA:
    case DT_CONTINUOUS_ARROW_ELIA:
    case DT_FREEWAY_ARROW_ELIA:
    case DT_FREEWAY_ARROW_ELIA_DUN:
    case DT_HUGE_ARROW_ELIA:
    case DT_HUGE_JUMPDOWN_ARROW_ELIA:
    case DT_NOVA_BOLT_ELIA:
    case DT_NOVA_ARROW_ELIA:
    case DT_NOVA_FREEWAY_BOLT_ELIA:
    case DT_NOVA_FREEWAY_BOLT_ELIA_DUN:
//   case DT_NOVA_SPECIAL1_BOLT_ELIA:

    case DT_ARROW_PEGASUS:
    case DT_CONTINUOUS_ARROW_PEGASUS:
    case DT_FREEWAY_ARROW_PEGASUS:
    case DT_FREEWAY_ARROW_PEGASUS_DUN:
    case DT_HUGE_ARROW_PEGASUS:
    case DT_HUGE_JUMPDOWN_ARROW_PEGASUS:
    case DT_NOVA_BOLT_PEGASUS:
    case DT_NOVA_ARROW_PEGASUS:
    case DT_NOVA_FREEWAY_BOLT_PEGASUS:
    case DT_NOVA_FREEWAY_BOLT_PEGASUS_DUN:
//    case DT_NOVA_SPECIAL1_BOLT_PEGASUS:

    case DT_ARROW_C_MASTER:
    case DT_CONTINUOUS_ARROW_C_MASTER:
    case DT_FREEWAY_ARROW_C_MASTER:
    case DT_FREEWAY_ARROW_C_MASTER_DUN:
    case DT_HUGE_ARROW_C_MASTER:
    case DT_HUGE_JUMPDOWN_ARROW_C_MASTER:
    case DT_NOVA_BOLT_C_MASTER:
    case DT_NOVA_ARROW_C_MASTER:
    case DT_NOVA_FREEWAY_BOLT_C_MASTER:
    case DT_NOVA_FREEWAY_BOLT_C_MASTER_DUN:
//    case DT_NOVA_SPECIAL1_BOLT_C_MASTER:

    case DT_ARROW_COLISEU:
    case DT_CONTINUOUS_ARROW_COLISEU:
    case DT_FREEWAY_ARROW_COLISEU:
    case DT_FREEWAY_ARROW_COLISEU_DUN:
    case DT_HUGE_ARROW_COLISEU:
    case DT_HUGE_JUMPDOWN_ARROW_COLISEU:
    case DT_NOVA_BOLT_COLISEU:
    case DT_NOVA_ARROW_COLISEU:
    case DT_NOVA_FREEWAY_BOLT_COLISEU:
    case DT_NOVA_FREEWAY_BOLT_COLISEU_DUN:
//    case DT_NOVA_SPECIAL1_BOLT_COLISEU:

        pDamageInstance->m_pOriginalDamage = new CHardDamageArrow();
        break;

    case DT_STRADUST1:
    case DT_STRADUST2:
    case DT_STRADUST3:
    case DT_STRADUST4:
    case DT_STRADUST5:
    case DT_STRADUST6:
    case DT_STRADUST7:
    case DT_STRADUST8:
        pDamageInstance->m_pOriginalDamage = new CHardDamageStarDustRain();
        break;

    case DT_SHOTMAGICCIRCLE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageShotMagicCircle();
        break;

    case DT_SHOTMAGICCIRCLE_DUN:
        pDamageInstance->m_pOriginalDamage = new CHardDamageShotMagicCircleDun();
        break;

    case DT_MARI1_SPECIAL3_TYPE_C_READY:
    case DT_MARI1_SPECIAL3_TYPE_C_READY_DUN:
    case DT_SKILL_MAGIC_MISSILE_READY:
    case DT_BEIGAS1_SPECIAL_TYPE2_LV3_READY:
    case DT_BEIGAS1_SPECIAL_TYPE3_LV1_GUIDE_BALL:
        pDamageInstance->m_pOriginalDamage = new CHardDamageMagicMissile();
        break;

        //case DT_SIEGE_DECO:
        //	pDamageInstance->m_pOriginalDamage = new CHardDamageSiegeDeco();
        //	break;

    case DT_CRITICAL_ARROW:                     //�������� ȭ��
    case DT_CRITICAL_ARROW_LIGHTNING:           //�������� ȭ�� ���� �ü� ��
    case DT_CRITICAL_ARROW_SHINING:
    case DT_CRITICAL_ARROW_LOVE:
    case DT_DOWN_ARROW:                         //�������� ȭ��
    case DT_DOWN_ARROW_LIGHTNING:               //�������� ȭ�� ���� �ü� ��
    case DT_DOWN_ARROW_SHINING:
    case DT_DOWN_ARROW_LOVE:
    case DT_DOWN_ARROW_DUN:                         //�������� ȭ��
    case DT_DOWN_ARROW_LIGHTNING_DUN:               //�������� ȭ�� ���� �ü� ��
    case DT_DOWN_ARROW_SHINING_DUN:
    case DT_DOWN_ARROW_LOVE_DUN:
    case DT_CRITICAL_BOLT:                      //���� Critical ȭ��
    case DT_CRITICAL_BOLT_LIGHTNING:            //���� Critical ȭ�� ���� �ü� ��
    case DT_CRITICAL_BOLT_SHINING:
    case DT_CRITICAL_BOLT_LOVE:
    case DT_HARPY_DOWN:                         //���� ��ü
    case DT_UP_ARROW:
    case DT_UP_ARROW_LIGHTNING:
    case DT_UP_ARROW_SHINING:
    case DT_UP_ARROW_LOVE:
    case DT_HUGE_CRITICAL_ARROW:
    case DT_HUGE_CRITICAL_ARROW_LIGHTNING:
    case DT_HUGE_CRITICAL_ARROW_SHINING:
    case DT_HUGE_CRITICAL_ARROW_LOVE:
    case DT_HUGE_RUNATK_ARROW:
    case DT_HUGE_RUNATK_ARROW_LIGHTNING:
    case DT_HUGE_RUNATK_ARROW_SHINING:
    case DT_HUGE_RUNATK_ARROW_LOVE:
    case DT_HUGE_COMBO3_ARROW:
    case DT_HUGE_COMBO3_ARROW_LIGHTNING:
    case DT_HUGE_COMBO3_ARROW_SHINING:
    case DT_HUGE_COMBO3_ARROW_LOVE:
    case DT_SKULL_CRITICAL_ARROW:
        // ��� ȭ���..
    case DT_NOVA_CRITICAL_ARROW:
    case DT_NOVA_CRITICAL_BOLT:
    case DT_NOVA_SPECIAL1_ARROW:
    case DT_NOVA_CRITICAL_ARROW_LIGHTNING:
    case DT_NOVA_CRITICAL_BOLT_LIGHTNING:
    case DT_NOVA_SPECIAL1_ARROW_LIGHTNING:
    case DT_NOVA_CRITICAL_ARROW_SHINING:
    case DT_NOVA_CRITICAL_BOLT_SHINING:
    case DT_NOVA_SPECIAL1_ARROW_SHINING:
    case DT_NOVA_CRITICAL_ARROW_DARK:
    case DT_NOVA_CRITICAL_BOLT_DARK:
    case DT_NOVA_SPECIAL1_ARROW_DARK:
    case DT_NOVA_CRITICAL_ARROW_LOVE:
    case DT_NOVA_CRITICAL_BOLT_LOVE:
    case DT_NOVA_SPECIAL1_ARROW_LOVE:
    case DT_SKILL_LAS_DASHATK_LV3_4:
    case DT_NOVA_CRITICAL_ARROW_DEATH:
    case DT_NOVA_CRITICAL_BOLT_DEATH:
    case DT_NOVA_SPECIAL1_ARROW_DEATH:

    case DT_CRITICAL_ARROW_TIME:
    case DT_UP_ARROW_TIME:
    case DT_DOWN_ARROW_TIME:
    case DT_DOWN_ARROW_TIME_DUN:
    case DT_CRITICAL_BOLT_TIME:
    case DT_HUGE_CRITICAL_ARROW_TIME:
    case DT_HUGE_RUNATK_ARROW_TIME:
    case DT_HUGE_COMBO3_ARROW_TIME:
    case DT_NOVA_CRITICAL_ARROW_TIME:
    case DT_NOVA_CRITICAL_BOLT_TIME:
    case DT_NOVA_SPECIAL1_ARROW_TIME:
    case DT_CRITICAL_ARROW_LIFE:
    case DT_UP_ARROW_LIFE:
    case DT_DOWN_ARROW_LIFE:
    case DT_DOWN_ARROW_LIFE_DUN:
    case DT_CRITICAL_BOLT_LIFE:
    case DT_HUGE_CRITICAL_ARROW_LIFE:
    case DT_HUGE_RUNATK_ARROW_LIFE:
    case DT_HUGE_COMBO3_ARROW_LIFE:
    case DT_NOVA_CRITICAL_ARROW_LIFE:
    case DT_NOVA_CRITICAL_BOLT_LIFE:
    case DT_NOVA_SPECIAL1_ARROW_LIFE:

    case DT_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_UP_ARROW_DARK_DRAGON_RED:
    case DT_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_DOWN_ARROW_DARK_DRAGON_RED_DUN:
    case DT_CRITICAL_BOLT_DARK_DRAGON_RED:
    case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_RED:
    case DT_HUGE_COMBO3_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_RED:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_RED:
    case DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_RED:

    case DT_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_UP_ARROW_DARK_DRAGON_BLUE:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_DOWN_ARROW_DARK_DRAGON_BLUE_DUN:
    case DT_CRITICAL_BOLT_DARK_DRAGON_BLUE:
    case DT_HUGE_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_RUNATK_ARROW_DARK_DRAGON_BLUE:
    case DT_HUGE_COMBO3_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_ARROW_DARK_DRAGON_BLUE:
    case DT_NOVA_CRITICAL_BOLT_DARK_DRAGON_BLUE:
    case DT_NOVA_SPECIAL1_ARROW_DARK_DRAGON_BLUE:

    case DT_CRITICAL_ARROW_HALLOWEEN:
    case DT_UP_ARROW_HALLOWEEN:
    case DT_DOWN_ARROW_HALLOWEEN:
    case DT_DOWN_ARROW_HALLOWEEN_DUN:
    case DT_CRITICAL_BOLT_HALLOWEEN:
    case DT_HUGE_CRITICAL_ARROW_HALLOWEEN:
    case DT_HUGE_RUNATK_ARROW_HALLOWEEN:
    case DT_HUGE_COMBO3_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_ARROW_HALLOWEEN:
    case DT_NOVA_CRITICAL_BOLT_HALLOWEEN:
    case DT_NOVA_SPECIAL1_ARROW_HALLOWEEN:

    case DT_CRITICAL_ARROW_ELIA:
    case DT_UP_ARROW_ELIA:
    case DT_DOWN_ARROW_ELIA:
    case DT_DOWN_ARROW_ELIA_DUN:
    case DT_CRITICAL_BOLT_ELIA:
    case DT_HUGE_CRITICAL_ARROW_ELIA:
    case DT_HUGE_RUNATK_ARROW_ELIA:
    case DT_HUGE_COMBO3_ARROW_ELIA:
    case DT_NOVA_CRITICAL_ARROW_ELIA:
    case DT_NOVA_CRITICAL_BOLT_ELIA:
    case DT_NOVA_SPECIAL1_ARROW_ELIA:

    case DT_CRITICAL_ARROW_PEGASUS:
    case DT_UP_ARROW_PEGASUS:
    case DT_DOWN_ARROW_PEGASUS:
    case DT_DOWN_ARROW_PEGASUS_DUN:
    case DT_CRITICAL_BOLT_PEGASUS:
    case DT_HUGE_CRITICAL_ARROW_PEGASUS:
    case DT_HUGE_RUNATK_ARROW_PEGASUS:
    case DT_HUGE_COMBO3_ARROW_PEGASUS:
    case DT_NOVA_CRITICAL_ARROW_PEGASUS:
    case DT_NOVA_CRITICAL_BOLT_PEGASUS:
    case DT_NOVA_SPECIAL1_ARROW_PEGASUS:

    case DT_CRITICAL_ARROW_C_MASTER:
    case DT_UP_ARROW_C_MASTER:
    case DT_DOWN_ARROW_C_MASTER:
    case DT_DOWN_ARROW_C_MASTER_DUN:
    case DT_CRITICAL_BOLT_C_MASTER:
    case DT_HUGE_CRITICAL_ARROW_C_MASTER:
    case DT_HUGE_RUNATK_ARROW_C_MASTER:
    case DT_HUGE_COMBO3_ARROW_C_MASTER:
    case DT_NOVA_CRITICAL_ARROW_C_MASTER:
    case DT_NOVA_CRITICAL_BOLT_C_MASTER:
    case DT_NOVA_SPECIAL1_ARROW_C_MASTER:

    case DT_CRITICAL_ARROW_COLISEU:
    case DT_UP_ARROW_COLISEU:
    case DT_DOWN_ARROW_COLISEU:
    case DT_DOWN_ARROW_COLISEU_DUN:
    case DT_CRITICAL_BOLT_COLISEU:
    case DT_HUGE_CRITICAL_ARROW_COLISEU:
    case DT_HUGE_RUNATK_ARROW_COLISEU:
    case DT_HUGE_COMBO3_ARROW_COLISEU:
    case DT_NOVA_CRITICAL_ARROW_COLISEU:
    case DT_NOVA_CRITICAL_BOLT_COLISEU:
    case DT_NOVA_SPECIAL1_ARROW_COLISEU:

    case DT_LIRE1_SPECIAL4_DOWN_ARROW:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIGHTNING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_SHINING:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LOVE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_TIME:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_LIFE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_RED:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_DARK_DRAGON_BLUE:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_HALLOWEEN:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_ELIA:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_PEGASUS:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_C_MASTER:
    case DT_LIRE1_SPECIAL4_DOWN_ARROW_COLISEU:

        pDamageInstance->m_pOriginalDamage = new CHardDamageCriticalArrow();
        break;

    case DT_METAL_SHOT_LV1_LOVE:
    case DT_METAL_SHOT_LV2_ARROW_LOVE:
    case DT_METAL_SHOT_LV3_ARROW_LOVE:
    case DT_CONTINUOUS_ARROW_LOVE:
    case DT_CONTINUOUS_ARROW_LV1_LOVE:
    case DT_CONTINUOUS_ARROW_LV2_LOVE:
    case DT_STORM_ARROW_LOVE:
    case DT_STORM_ARROW_DUN_LOVE:
    case DT_LIRE1_SPECIAL4_STORM_ARROW_LOVE:
    case DT_BOLT_LOVE:
    case DT_LIRE2_SPECIAL1_DMG_LOVE:
    case DT_NOVA_SPECIAL1_BOLT_LOVE:
    case DT_NOVA_SPECIAL3_BOLT_LOVE:

        pDamageInstance->m_pOriginalDamage = new CHardDamageRoseArrow();
        break;

    case DT_CYCLOPSE_STORMP_STONE0:
    case DT_CYCLOPSE_STORMP_STONE1:
    case DT_CYCLOPSE_STORMP_STONE2:
    case DT_CYCLOPSE_STONE:
    case DT_CONFUSION:
    case DT_DARK_SWORM_FLY:
    case DT_THROW_STONE0:
    case DT_THROW_STONE1:
    case DT_THROW_STONE2:
    case DT_THROW_STONE3:
    case DT_THROW_STONE4:
    case DT_THROW_STONE5:
    case DT_THROW_STONE6:
        pDamageInstance->m_pOriginalDamage = new CHardDamageSinFly();
        break;
    case DT_PHOENIX_ARROW:
    case DT_PHOENIX_ARROW_FIRE:
        pDamageInstance->m_pOriginalDamage = new CHardDamagePhoenixArrow();
        break;
    case DT_RED_CANNON:  //���� ������ ��� ��.
    case DT_YELLOW_CANNON:
    case DT_MONGBAN_CANNON:
        pDamageInstance->m_pOriginalDamage = new CHardDamageCannon();
        break;
    case DT_ARME3_ICEBOLT1:
    case DT_ARME3_ICEBOLT2:
    case DT_ARME3_ICEBOLT3:
        pDamageInstance->m_pOriginalDamage = new HardDamageIceBolt();
        break;
    case DT_RYAN3_SEED:
    case DT_ARME3_FIRESPOT:
    case DT_VANESSA_ATK3:
        pDamageInstance->m_pOriginalDamage = new HardDamageFireSpot();
        break;
    case DT_INSECTIVOROUS_BREEDING:
    case DT_GOLEM_TOKEN:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBreeding();
        break;
    case DT_RYAN2_ELECTRON_BALL:
    case DT_DARK_AMON_SOUL_IMPACT:
        //case DT_MONSTER_KAMIKI06: // ī��Ű �������
        //case DT_VANESSA_ATK2:
    case DT_MONSTER_FAIRY_SOUL:
    case DT_SKILL_RYAN_WOLF_SPECIAL2_3:
    case DT_ANTOINETTE_VIB:
    case DT_BLOODYORC_WARRIOR_SHOULDER_ATK:
    case DT_PERIOT_DASH_ATK:
        pDamageInstance->m_pOriginalDamage = new CHardDamageElectronicBall();
        break;
    case DT_ICEPIXIE_ICEBREATH:
    case DT_BLACKFAIRYQUEEN_BREATH:
    case DT_ARME4_SPECIAL1_BLACK:
    case DT_MONSTER_KAMIKI05: // ī��Ű ��������
    case DT_MONSTER_WENDY03: // ���� ���̽� �극��
    case DT_BLACKFAIRYQUEEN_BREATH_SILVERKNIGHTS: //�ҷ����� �극�� 
    case DT_SABERTOOTH_HOWL_GAS:
    case DT_FREEZING_KENKRE_BREATH:
    case DT_MARI3_SPECIAL3_ATK_FROZEN_HORIZONTAL:
    case DT_MARI3_SPECIAL3_ATK_FROZEN_VERTICAL:
    case DT_HERO_KAMIKI_ICEFIELD:
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN:
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN_LV1:
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN_LV2:
    case DT_ZERO1_CLASSICALFORCE_ICE_FROZEN_LV3:
    case DT_SUMMON_GARGOYLE_ICEBREATH:
    case DT_MARI4_SPECIAL3_ICE:
    case DT_MARI4_SPECIAL3_ICE_EVENT_MON:
    case DT_ARME3_SPECIAL4_ICE_DMG:
    case DT_MARI4_SPECIAL4_LASER_ICE_ATK:
    case DT_MARI4_SPECIAL4_ICE_BOOM:
    case DT_PET_RURU_ATTACK2:
    case DT_PET_GWEN_CHAIN_ATK:
    case DT_COWNAT_MON_DARKSPELL_ATTACK02_LEFT:
    case DT_COWNAT_MON_DARKSPELL_ATTACK02_RIGHT:
    case DT_EDEL1_UPGRADE_SPECIAL1_TYPE2_ATK4:
    case DT_MON_JONJALLE_ATTACK03_FRAME_BLUE:
    case DT_EDEL2_SPECIAL_A_1_ICE_ATK:
    case DT_EDEL2_SPECIAL_A_2_ICE_ATK:
    case DT_PET_ICEDRAGON_ATTACK1:
    case DT_PET_ICEDRAGON_ATTACK2:
    case DT_PET_ICEDRAGON_ATTACK3:
        //case DT_ARME3_ICEBALL:
    case DT_EXTINCTION_MON_KAMIKI_PHASE1_ICEFIELD:
    case DT_EXTINCTION_MON_KAMIKI_PHASE2_ICEFIELD:
    case DT_ARME_SKILL_ICESTRIKE_ICEFIELD:
    case DT_EXTINCTION_MON_CENTER_05:
        pDamageInstance->m_pOriginalDamage = new KHardDamageIcePixieIceStorm();
        break;
    case DT_CHAOS_PEPE_GAS:
        pDamageInstance->m_pOriginalDamage = new KHardDamagePepeGas();
        break;
    case DT_BERMESIA_FINAL_EARTHQUAKE_LV1:
    case DT_BERMESIA_FINAL_EARTHQUAKE_LV2:
    case DT_BERMESIA_FINAL_EARTHQUAKE_LV3:
    case DT_BERMESIA_FINAL_EARTHQUAKE_LV4:
    case DT_BERMESIA_FINAL_EARTHQUAKE_LV5:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBermesiaFinalEarthQuake();
        break;
    case DT_ELESIS4_SPECIAL2_R:
    case DT_ELESIS4_SPECIAL2_L:
        pDamageInstance->m_pOriginalDamage = new CHardDamageElesis4Special2();
        break;
        //case DT_ELESIS4_DOWN_ATK:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageElesis4DownAtk();
        //    break;
        //case DT_ELESIS4_SPECIAL3_PARTICLE_L:
        //case DT_ELESIS4_SPECIAL3_PARTICLE_R:
        //case DT_ELESIS4_SPECIAL3_SWORD_L:
        //case DT_ELESIS4_SPECIAL3_SWORD_R:
        //case DT_RYAN3_SPECIAL3_WING_START:
        //case DT_RYAN3_SPECIAL3_WING_END:
        //case DT_RYAN3_SPECIAL3_WING_START_L:
        //case DT_RYAN3_SPECIAL3_WING_END_L:
        //case DT_RYAN4_SPECIAL3_SHINING_FINGER_DECO:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageParticleSlowCount();
        //    break;
    case DT_ARME4_SPECIAL3_BLACK:
    case DT_ARME4_SPECIAL3_BLACK2:
    case DT_ARME4_SPECIAL3_BLACK3:
    case DT_ARME4_SPECIAL3_BLACK4:
    case DT_ARME4_SPECIAL3_BLACK5:
    case DT_ARME4_SPECIAL3_BLACK_ICE:
    case DT_ARME4_SPECIAL3_BLACK2_ICE:
    case DT_ARME4_SPECIAL3_BLACK3_ICE:
    case DT_ARME4_SPECIAL3_BLACK4_ICE:
    case DT_ARME4_SPECIAL3_BLACK5_ICE:
    case DT_ARME4_SPECIAL3_BLACK_SHINING:
    case DT_ARME4_SPECIAL3_BLACK2_SHINING:
    case DT_ARME4_SPECIAL3_BLACK3_SHINING:
    case DT_ARME4_SPECIAL3_BLACK4_SHINING:
    case DT_ARME4_SPECIAL3_BLACK5_SHINING:
    case DT_ARME4_SPECIAL3_BLACK_LOVE:
    case DT_ARME4_SPECIAL3_BLACK2_LOVE:
    case DT_ARME4_SPECIAL3_BLACK3_LOVE:
    case DT_ARME4_SPECIAL3_BLACK4_LOVE:
    case DT_ARME4_SPECIAL3_BLACK5_LOVE:

    case DT_ARME4_SPECIAL3_BLACK_LIFE:
    case DT_ARME4_SPECIAL3_BLACK2_LIFE:
    case DT_ARME4_SPECIAL3_BLACK3_LIFE:
    case DT_ARME4_SPECIAL3_BLACK4_LIFE:
    case DT_ARME4_SPECIAL3_BLACK5_LIFE:

    case DT_ARME4_SPECIAL3_BLACK_DARK_DRAGON_RED:
    case DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_RED:
    case DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_RED:
    case DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_RED:
    case DT_ARME4_SPECIAL3_BLACK5_DARK_DRAGON_RED:

    case DT_ARME4_SPECIAL3_BLACK_DARK_DRAGON_BLUE:
    case DT_ARME4_SPECIAL3_BLACK2_DARK_DRAGON_BLUE:
    case DT_ARME4_SPECIAL3_BLACK3_DARK_DRAGON_BLUE:
    case DT_ARME4_SPECIAL3_BLACK4_DARK_DRAGON_BLUE:
    case DT_ARME4_SPECIAL3_BLACK5_DARK_DRAGON_BLUE:

    case DT_ARME4_SPECIAL3_BLACK_HALLOWEEN:
    case DT_ARME4_SPECIAL3_BLACK2_HALLOWEEN:
    case DT_ARME4_SPECIAL3_BLACK3_HALLOWEEN:
    case DT_ARME4_SPECIAL3_BLACK4_HALLOWEEN:
    case DT_ARME4_SPECIAL3_BLACK5_HALLOWEEN:

    case DT_ARME4_SPECIAL3_BLACK_ELIA:
    case DT_ARME4_SPECIAL3_BLACK2_ELIA:
    case DT_ARME4_SPECIAL3_BLACK3_ELIA:
    case DT_ARME4_SPECIAL3_BLACK4_ELIA:
    case DT_ARME4_SPECIAL3_BLACK5_ELIA:

    case DT_ARME4_SPECIAL3_BLACK_C_MASTER:
    case DT_ARME4_SPECIAL3_BLACK2_C_MASTER:
    case DT_ARME4_SPECIAL3_BLACK3_C_MASTER:
    case DT_ARME4_SPECIAL3_BLACK4_C_MASTER:
    case DT_ARME4_SPECIAL3_BLACK5_C_MASTER:

    case DT_ARME4_SPECIAL3_BLACK_COLISEU:
    case DT_ARME4_SPECIAL3_BLACK2_COLISEU:
    case DT_ARME4_SPECIAL3_BLACK3_COLISEU:
    case DT_ARME4_SPECIAL3_BLACK4_COLISEU:
    case DT_ARME4_SPECIAL3_BLACK5_COLISEU:

        //case DT_MONSTER_POTATO_METEOR_L:
    //case DT_MONSTER_POTATO_METEOR_R:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBigMeteo();
        break;
    case DT_ARME4_SPECIAL3_METEO_READY:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBigMeteoReady();
        break;
    case DT_ARME4_SPECIAL2_BLACK_READY:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBigLightningReady();
        break;
    case DT_ELESIS4_SPECIAL2_SHOOT_L:
    case DT_ELESIS4_SPECIAL2_SHOOT_R:
        pDamageInstance->m_pOriginalDamage = new CHardDamageElesis4Special2Shoot();
        break;
    case DT_LAS4_JUMP_ATK:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLas4DownAtk();
        break;
    case DT_LAS4_BOUNSE_ATK:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLas4BounceAtk();
        break;
    case DT_LAS4_SPECIAL3_SWORD_R:
    case DT_LAS4_SPECIAL3_SWORD_L:
    case DT_LAS4_SPECIAL3_FIRE_SWORD_L:
    case DT_LAS4_SPECIAL3_FIRE_SWORD_R:
    case DT_LAS4_SPECIAL3_DARK_SWORD_L:
    case DT_LAS4_SPECIAL3_DARK_SWORD_R:
    case DT_LAS4_SPECIAL3_LIGHT_SWORD_L:
    case DT_LAS4_SPECIAL3_LIGHT_SWORD_R:
    case DT_LAS4_SPECIAL3_ICE_SWORD_L:
    case DT_LAS4_SPECIAL3_ICE_SWORD_R:
    case DT_LAS4_SPECIAL3_LOVE_SWORD_L:
    case DT_LAS4_SPECIAL3_LOVE_SWORD_R:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLas4Special3Sword();
        break;
    case DT_RENASIEN_ATK3_R:
    case DT_RENASIEN_ATK3_L:
    case DT_RENASIEN_ATK4_R:
    case DT_RENASIEN_ATK4_L:
        pDamageInstance->m_pOriginalDamage = new KGCHardDamagePoisonBreath();
        break;
    case DT_OCTUS_ATK4:
        pDamageInstance->m_pOriginalDamage = new CHardDamageOctusWarcry();
        break;
        // HardDamage �����۾� ����
        //     case DT_RONAN_MAGIC_SPECIAL1_BUFF:
        //         pDamageInstance->m_pOriginalDamage = new CHardDamageRonanMagicSpecial1();
        //         break;
        // HardDamage �����۾� ��
    case DT_RONAN_SWORD_SPECIAL3_SWORD:
    case DT_RONAN_SWORD_SPECIAL3_SWORD_DUN:
        pDamageInstance->m_pOriginalDamage = new CHardDamageHommingMissle();
        break;
    case DT_RONAN_MAGIC_SPECIAL2_READY:
        //case DT_RONAN_MAGIC_SPECIAL2_METEO:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRonanMeteo();
        break;
        //case DT_RONAN2_MAGIC_SPECIAL1_BUFF:
        //    pDamageInstance->m_pOriginalDamage = new CHardDamageRonan2MagicSpecial1();
        break;
    case DT_RONAN2_MAGIC_SPECIAL3_SUMMON_DRAGON:
        pDamageInstance->m_pOriginalDamage = new CHardDamageSummonDragon();
        break;
    case DT_DEEP_IMPACT_SOLO:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBigMeteoSolo();
        break;
    case DT_TEMPEST_SLAYER:
        pDamageInstance->m_pOriginalDamage = new CHardDamageSummonTempest();
        break;
    case DT_AMY2_PERFORMENCE_ATK1_NOTE1: // ���̹� 1������ ���ָ����� - dolki
    case DT_AMY2_PERFORMENCE_ATK1_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE1:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK3_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE1:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE2:
    case DT_AMY2_PERFORMENCE_DASH_ATK_NOTE1:
    case DT_AMY2_PERFORMENCE_DASH_ATK_NOTE2:
    case DT_AMY2_PERFORMENCE_ATK1_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_ATK1_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_ATK2_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_ATK3_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_ATK4_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE1_DUN:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE2_DUN:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE1:
    case DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE2:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE1:
    case DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE2:
    case DT_AMY2_FIGHT_UNIQUE_EFFECT:
    case DT_AMY3_DANCE_SPECIAL1_ATK:
    case DT_AMY2_SPECIAL4_BIG_NOTE1:
    case DT_AMY2_SPECIAL4_BIG_NOTE2:
    case DT_AMY2_SPECIAL4_BIG_NOTE3:
    case DT_AMY2_SPECIAL4_SMALL_NOTE1:
    case DT_AMY2_SPECIAL4_SMALL_NOTE2:
    case DT_AMY2_SPECIAL4_SMALL_NOTE3:
    case DT_AMY2_SPECIAL4_SMALL_NOTE1_L:
    case DT_AMY2_SPECIAL4_SMALL_NOTE2_L:
    case DT_AMY2_SPECIAL4_SMALL_NOTE3_L:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAmyNoteSinFly();
        break;
    case DT_AMY2_PERFORMENCE_INSTRUMENT1_MODEL:
    case DT_AMY2_PERFORMENCE_INSTRUMENT2_MODEL:
    case DT_AMY2_PERFORMENCE_INSTRUMENT3_MODEL:
    case DT_AMY2_PERFORMENCE_INSTRUMENT4_MODEL:
    case DT_AMY2_PERFORMENCE_INSTRUMENT5_MODEL:
        //case DT_AMY2_PERFORMENCE_INSTRUMENT_FINISH:
        pDamageInstance->m_pOriginalDamage = new CHardDamageSummonInstrument();
        break;
    case DT_RONAN4_MAGIC_SWORD_SPECIAL3_1:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL3_1_L:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL3_2:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL3_2_L:
    case DT_RONAN4_MAGIC_SWORD_SPECIAL3_1_DMG1:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRonan4SwordSpecial3();
        break;
    case DT_RONAN4_MONOVOLT_ATK1_1:
    case DT_RONAN4_MONOVOLT_ATK1_2:
    case DT_RONAN4_MONOVOLT_ATK1_3:
    case DT_RONAN4_MONOVOLT_ATK2_1:
    case DT_RONAN4_MONOVOLT_ATK2_2:
    case DT_RONAN4_MONOVOLT_ATK2_3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRonan4MonoVolt();
        break;
    case DT_RONAN4_WHITE_MAGIC_SPECIAL3_VALKYRIE:
    case DT_RONAN4_RONAN4_VALKYRIE_ARROW1:
    case DT_RONAN4_RONAN4_VALKYRIE_ARROW2:
    case DT_RONAN4_RONAN4_VALKYRIE_ARROW3:
    case DT_RONAN4_RONAN4_VALKYRIE_ARROW4:
    case DT_RONAN4_RONAN4_VALKYRIE_ARROW5:
    case DT_RONAN4_RONAN4_VALKYRIE_ARROW6:
    case DT_RONAN4_RONAN4_VALKYRIE_FINISH_R:
    case DT_RONAN4_RONAN4_VALKYRIE_FINISH_L:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRonan4SummonValkyrie();
        break;
    case DT_ARME_SKILL_BLOODPEST_TRACE_OBJECT:
    case DT_ARME_SKILL_BLOODPEST_CATCHED_MP:
    case DT_PERIOT_HOMMING_MISSILE_HP:
    case DT_SUPERMON_PERIOT_HOMMING_MISSILE_HP:
    case DT_STATUE_HOMMING_MISSILE:
    case DT_DKMARK_II_TOYHOMMING:
    case DT_DARKTEMPLAR_SOUL_DRAIN:
    case DT_THANATOS2_GHOSTOFDEATH:
    case DT_TALIN_SMART_HOMMING:
    case DT_SANDRULER_ATK4:
    case DT_SUPERMON_SANDRULER_ATK4:
    case DT_THUNDER_HAMMER_DUEL_ATK2:
    case DT_HERO_GADOSEN_LIGHTOFHELL:
    case DT_HERO_KAMIKI_ATK1:
    case DT_HERO_NEMOPHILLA_MIND_STARTER:
    case DT_HELL_HOWLING_GHOST:
    case DT_HELL_HOWLING_GHOST_VULCANUS:
    case DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK1:
    case DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK1:
    case DT_EXTINCTION_MON_GADOSEN_PHASE1_LIGHTOFHELL:
    case DT_EXTINCTION_MON_GADOSEN_PHASE2_LIGHTOFHELL:
    case DT_UNO1_COMBO_UP_ATK2_MON:
    case DT_UNO1_BACKMOVE_ATK2_MON:
    case DT_UNO1_DOUBLE_ATK1_MON:
    case DT_UNO1_COMBO_ATK2_E_MON:
    case DT_UNO1_CRITICAL_ATK1_MON:
    case DT_DIO_SPECIAL4_TYPE2_DMG:
        pDamageInstance->m_pOriginalDamage = new CHardDamageArmeSkillTest();
        break;
    case DT_ARME_SKILL_ICESTRIKE_FREEING_SPACE:
    case DT_ARME_SKILL_ICECIRCULAR_LV2_FREEZING:
        pDamageInstance->m_pOriginalDamage = new CHardDamageArme1IceStrike();
        break;
    case DT_ARME_SKILL_W3_GORGOSFLAME_LV1:
    case DT_ARME_SKILL_W3_GORGOSFLAME_LV2:
        pDamageInstance->m_pOriginalDamage = new CHardDamageArmeSummonGorgos();
        break;
        //case DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL:
        //case DT_ARME_SKILL_WALLOFICE_HORIZON_WALL:
        //	pDamageInstance->m_pOriginalDamage = new CHardDamageArmeWallOfIce();			
        //	break;
    case DT_ARME_SKILL_SAINTBREEDING_SPACE:
    case DT_ARME_SKILL_FROSTRING_LV1_CRASH_EFFECT:
    case DT_ARME_SKILL_FROSTRING_LV2_CRASH_EFFECT:
        pDamageInstance->m_pOriginalDamage = new CHardDamageSaintBreeding();
        break;
        // HardDamage �����۾� ����
        //case DT_ARME_SKILL_BIGBOOM_STICK:
        //	pDamageInstance->m_pOriginalDamage = new CHardDamageArmeBigBoom();			
        //	break;
        // HardDamage �����۾� ��
    case DT_MONSTER_MINE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageMonsterMine();
        break;
        //HardDamage �����۾�
        //case DT_ASSASSIN_RANGER02:
        //	pDamageInstance->m_pOriginalDamage = new CHardDamageAssassinRanger();
        //	break;
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET1:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET2:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET3:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET1_1:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET1_2:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET2_1:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET2_2:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET3_1:
    case DT_AMY3_FIGHTING_UNIQUE_SKILL1_PET3_2:
    case DT_AMY3_DANCE_UNIQUE_SKILL_PET1:
    case DT_AMY3_DANCE_UNIQUE_SKILL_PET2:
    case DT_AMY3_DANCE_UNIQUE_SKILL_PET3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAmy3UniqueSkill1();
        break;
    case DT_AMY3_DANCE_SPECIAL3_BACK_COLOR:
    case DT_AMY3_DANCE_SPECIAL3_BACK_COLOR_DUN:
    case DT_AMY4_MELO_SPECIAL2_BACK_BLACK:
    case DT_JURIOR_BACKCOLOR1:
    case DT_JURIOR_BACKCOLOR2:
    case DT_RONAN_PENTACLE_STRIKE_1:
    case DT_RONAN_PENTACLE_STRIKE_2:
    case DT_RONAN_PENTACLE_STRIKE_3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAmy3Special3();
        break;
    case DT_STONE_BODY:
    case DT_STONE_BODY_ICE:
    case DT_WOLF_STONE_BODY_ICE:
    case DT_WOLF_STONE_BODY:
    case DT_NEPHILIM_STONE_BODY:
    case DT_NEPHILIM_STONE_BODY_ICE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageStoneBody();
        break;
    case DT_JIN_SPECIAL3_BURNNING_BALL_EFFECT:
    case DT_JIN_SPECIAL3_BURNNING_BALL_EFFECT_DRAMA:
    case DT_JIN1_SPECIAL3_FINAL_EFFECT:
        pDamageInstance->m_pOriginalDamage = new CHardDamageJinSpecial3Burnning();
        break;
    case DT_MONSTER_CHARGED_BOLT:
        pDamageInstance->m_pOriginalDamage = new HardDamageChargedBolt();
        break;
    case DT_MONSTER_CRAB_STONE:
        pDamageInstance->m_pOriginalDamage = new HardDamageCrabStone();
        break;
    case DT_ZIG_SUMMON_DRAGON:
        pDamageInstance->m_pOriginalDamage = new CHardDamageSummonBlackDragon();
        break;
    case DT_SKILL_LAS_TRADWARM_POT:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLasTradWarmPot();
        break;
    case DT_SKILL_LAS_TRADWARM_LV2_TRAP:
        pDamageInstance->m_pOriginalDamage = new CHardDamageTradWarmTrap();
        break;
    case DT_SKILL_RYAN_HP_TOTEM:
    case DT_SKILL_RYAN_MP_TOTEM:
    case DT_SKILL_RYAN_DP_TOTEM:
    case DT_SKILL_RYAN_HP_TOTEM_AREA:
    case DT_SKILL_RYAN_MP_TOTEM_AREA:
    case DT_SKILL_RYAN_DP_TOTEM_AREA:
    case DT_SKILL_RYAN_AP_TOTEM:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRyanTotem();
        break;
    case DT_SKILL_RYAN_POISON_SPORE:
    case DT_SKILL_RYAN_POISON_SPORE_DUN:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRyanPoisonSpore();
        break;
        //case DT_SKILL_RYAN_BLOODY_SMOKE:
        //case DT_SKILL_RYAN_BLOODY_SMOKE_R:
        //	pDamageInstance->m_pOriginalDamage = new CHardDamageRyanBloody();
        //	break;
    case DT_AMY4_CANNON_BALL:
    case DT_AMY4_CANNON_BALL_GROUND:

        pDamageInstance->m_pOriginalDamage = new CHardDamageAmy4CannonBall();
        break;
    case DT_AMY4_PARTNER:
    case DT_AMY4_MELO_SPECIAL3_PARTNER:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAmy4RandomBoy();
        break;
    case DT_XENIA5_MAP_DAMAGE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageXeniaMapDamage();
        break;
    case DT_PERIOT_MONSTER_BALL:
    case DT_HATSHEPSUT_COCCON_BALL:
        pDamageInstance->m_pOriginalDamage = new CHardDamagePeriotOrb();
        break;
    case DT_PERIOT_EAT_MONSTER:
        pDamageInstance->m_pOriginalDamage = new CHardDamagePeriotEat();
        break;
    case DT_PERIOT_FIRE_DESTROY_ORB:
    case DT_THANATOS2_DARKSPEAR_SHOOTER:
        pDamageInstance->m_pOriginalDamage = new CHardDamagePeriotDestroyOrb();
        break;
    case DT_RONAN_METEOR_LV3_RUNE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRonanMeteorLv3Rune();
        break;
        // HardDamage �����۾� ����
        //     case DT_RONAN_MAGIC_SPECIAL1_BUFF_LV2:
        //         pDamageInstance->m_pOriginalDamage = new CHardDamageRonanMagicSpecial1Lv2();
        //         break;
        //     case DT_RONAN_MAGIC_SPECIAL1_BUFF_LV3:
        //         pDamageInstance->m_pOriginalDamage = new CHardDamageRonanMagicSpecial1Lv3();
        //         break;
        // HardDamage �����۾� ��

        // HardDamage �����۾� ����
        //     case DT_RONAN_ENERGY_CIRCLE:
        //         pDamageInstance->m_pOriginalDamage = new CHardDamageRonanEnergyCircle();
        //         break;
        // HardDamage �����۾� ��
    case DT_RONAN_ENERGY_CIRCLE_CURSOR:
        pDamageInstance->m_pOriginalDamage = new CHardDamageRonanEnergyRune();
        break;
    case DT_DIVINE_TREE_HOLY_POLE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageHolyPole();
        break;
    case DT_DIVINE_TREE_POISON_BREATH:
    case DT_DIVINE_TREE_POISON_BREATH_L:
    case DT_DIVINE_TREE_POISON_BREATH_R:
        pDamageInstance->m_pOriginalDamage = new CHardDamageDivineTreeSonicBomb();
        break;
        // HardDamage �����۾� ����
        // 	case DT_DIVINE_TREE_ACID_RAIN:
        // 		pDamageInstance->m_pOriginalDamage = new CHardDamageDivineTreeAcidRain();
        // 		break;
        // HardDamage �����۾� ��
    case DT_DIVINE_TREE_SUNKEN_AREA:
    case DT_WOODEN_SUNKEN_AREA:
        pDamageInstance->m_pOriginalDamage = new CHardDamageSunkenArea();
        break;
    case DT_DARKTEMPLAR_DOUBLEFRAME:
    case DT_THANATOS3_LEFTHAND:
        pDamageInstance->m_pOriginalDamage = new CHardDamageDarkTemplarDoubleFrame();
        break;
    case DT_IBLIS_LASER_SHOOTER:
    case DT_SUPERMON_IBLIS_LASER_SHOOTER:
        pDamageInstance->m_pOriginalDamage = new CHardDamageIblisLaser();
        break;
    case DT_JIN4_ENERGY_BALL:
        pDamageInstance->m_pOriginalDamage = new CHardDamageJin4Ball();
        break;

    case DT_JIN4_SP3_DMG_1:
    case DT_JIN4_BURNNING_SP3_DMG_1:
    case DT_JIN4_BURNNING_SP3_DMG_3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageJin4Sucker();
        break;

    case DT_PET_MERMAIRING_TORNADO_1:
        pDamageInstance->m_pOriginalDamage = new CHardDamageMermairing();
        break;
    case DT_PET_QUMING_THROWGIFT_1:
    case DT_PET_QUMING_THROWGIFT_2:
    case DT_PET_QUMING_THROWGIFT_3:
    case DT_PET_QUMING_THROWGIFT_4:
    case DT_PET_QUMING_THROWGIFT_5:
    case DT_PET_QUMING_THROWGIFT_6:
    case DT_PET_QUMING_THROWGIFT_7:
    case DT_PET_QUMING_THROWGIFT_1_R:
    case DT_PET_QUMING_THROWGIFT_2_R:
    case DT_PET_QUMING_THROWGIFT_3_R:
    case DT_PET_QUMING_THROWGIFT_4_R:
    case DT_PET_QUMING_THROWGIFT_5_R:
    case DT_PET_QUMING_THROWGIFT_6_R:
    case DT_PET_QUMING_THROWGIFT_7_R:
        pDamageInstance->m_pOriginalDamage = new HardDamageIceBolt();
        break;
    case DT_AMY_DIVINE_MESSAGE_FAKE:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAmy1DivineMsg();
        break;
    case DT_AMY_ABRACADABRA:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAmy1Abracadabra();
        break;
    case DT_AMY_ABRACADABRA_DUN:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAmy1AbracadabraDun();
        break;
    case DT_PET_SEAMUS_BUFF:
        pDamageInstance->m_pOriginalDamage = new CHardDamagePetSeamusBuff();
        break;
    case DT_TELEPORTER:
        pDamageInstance->m_pOriginalDamage = new CHardDamageTeleporter();
        break;
    case DT_MARI1_PORTAL:
        pDamageInstance->m_pOriginalDamage = new CHardDamagePortal();
        break;
    case DT_HATSHEPSUT_ABSORPTION:
        pDamageInstance->m_pOriginalDamage = new CHardDamageHatshepsut();
        break;
    case DT_EXP_EFFECT:
        pDamageInstance->m_pOriginalDamage = new CHardDamageCatMullRomSpline();
        break;
    case DT_MARI2_BUILD_LV2_BIG_MISSILE:
#if defined( ANGELS_EGG_REFORM )
    case DT_ANGELS_EGG_TURRET_BIG_MISSILE:
#endif
        pDamageInstance->m_pOriginalDamage = new CHardDamageMari2BuildLV2();
        break;
    case DT_MARI2_CHARGE_2:
        pDamageInstance->m_pOriginalDamage = new CHardDamageMari2MagicBeadLv2();
        break;
    case DT_SANDRULER_ATK4_LAUNCHER:
    case DT_SUPERMON_SANDRULER_ATK4_LAUNCHER:
        pDamageInstance->m_pOriginalDamage = new CHardDamageDesert4DungFly();
        break;
    case DT_MAP_RAIL_SHORT:
    case DT_MAP_RAIL_LONG:
        pDamageInstance->m_pOriginalDamage = new CHardDamageChaosMapRail();
        break;
    case DT_MIRAGE_POT_HP:
    case DT_MIRAGE_POT_MP:
    case DT_MIRAGE_POT_DEC_MP:
        pDamageInstance->m_pOriginalDamage = new CHardDamageDesert4Pot();
        break;
    case DT_TECHNICAL_GUARD_ATK3_BOMB:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBindWeb();
        break;
    case DT_LAS1_SPECIAL4:
    case DT_LAS1_SPECIAL4_R:
        //
    case DT_TINY_SPEAR_LANCE_R:
    case DT_TINY_SPEAR_LANCE:
    case DT_PET_SOL_METEOR:
    case DT_PET_SOL_METEOR_R:
        //
    case DT_LIRE2_SPECIAL3_DMG_DUN:
    case DT_LIRE2_SPECIAL3_DMG_R_DUN:
    case DT_LIRE2_SPECIAL3_DMG:
    case DT_LIRE2_SPECIAL3_DMG_R:
    case DT_RIN1_SPECIAL3_UNIQUE_WONGIOKU_ATK:
    case DT_RIN1_SPECIAL3_UNIQUE_WONGIOKU_ATK_R:
    case DT_DIO3_EVIL_EDDY_BALL_LV3:
    case DT_DIO3_EVIL_EDDY_BALL_LV3_2:
    case DT_DIO3_EVIL_EDDY_BALL_LV3_3:
    case DT_DIO3_EVIL_EDDY_BALL_LV3_4:
    case DT_DIO3_EVIL_EDDY_BALL_LV3_5:
    case DT_RIN_SPECIAL4_BALL_FIRE_1:
    case DT_RIN_SPECIAL4_BALL_FIRE_2:
    case DT_RIN_SPECIAL4_BALL_FIRE_3:
    case DT_RIN_SPECIAL4_BALL_FIRE_4:
    case DT_RIN_SPECIAL4_BALL_FIRE_5:
    case DT_RIN_SPECIAL4_BALL_FIRE_6:
    case DT_RIN_SPECIAL4_BALL_FIRE_UP:
    case DT_RIN_SPECIAL4_BALL_FIRE_DOWN:
    case DT_RIN2_EVIL_BASE_SPECIAL3_UNIQUE_WONGIOKU_ATK:
    case DT_RIN2_EVIL_BASE_SPECIAL3_UNIQUE_WONGIOKU_ATK_R:
    case DT_RIN1_SPECIAL3_UNIQUE_WONGIOKU_ATK_DUN:
    case DT_RIN1_SPECIAL3_UNIQUE_WONGIOKU_ATK_R_DUN:
        pDamageInstance->m_pOriginalDamage = new CHardDamageLire2Drill();
        break;
    case DT_PET_ARON_ATTACK3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageKindPet();
        break;
    case DT_HERO_KAMIKI_ATK3_2:
    case DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK3_2:
    case DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK3_2:
        pDamageInstance->m_pOriginalDamage = new CHardDamageKamikiLightning();
        break;
    case DT_HERO_KAMIKI_ATK3:
    case DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK3:
    case DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageKamikiLightningStarter();
        break;
    case DT_PET_SELION_ATTACK:
    case DT_PET_SELION_ATTACK_R:
    case DT_PET_SELION_CRASH:
    case DT_PET_BLACKDRAGON_ATTACK1:
    case DT_PET_BLACKDRAGON_ATTACK2:
    case DT_PET_BLACKDRAGON_ATTACK3:
    case DT_PET_BERKAS_ATTACK1:
    case DT_PET_BERKAS_ATTACK2:
    case DT_PET_BERKAS_ATTACK3:
        //	case DT_PET_HARKYON_ATK_ETERNAL_EXTINCTION:
        //	case DT_PET_HARKYON_ATK_ETERNAL_EXTINCTION_BOMB:
        //        pDamageInstance->m_pOriginalDamage = new CHardDamageSelion();
        //        break;
    case DT_PET_ASTAROT_ATTACK1:
        pDamageInstance->m_pOriginalDamage = new CHardDamageAstarot();
        break;
    case DT_PET_BLITZCHEN_ATTACK_1:
    case DT_PET_BLITZCHEN_ATTACK_2:
    case DT_PET_BLITZCHEN_ATTACK_3:
    case DT_PET_BLACK_BLITZCHEN_ATTACK_1:
    case DT_PET_BLACK_BLITZCHEN_ATTACK_2:
    case DT_PET_BLACK_BLITZCHEN_ATTACK_3:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBlitchen();
        break;
    case DT_PET_BAKUNAWA_SPECIAL_PULL1:
    case DT_PET_BAKUNAWA_SPECIAL_PULL2:
    case DT_PET_BAKUNAWA_SPECIAL_PULL3:
    case DT_PET_BAKUNAWA_SPECIAL_PULL4:
    case DT_PET_BAKUNAWA_SPECIAL_BLACKHOLL:
    case DT_PET_BAKUNAWA_SPECIAL_BOOM:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBakunawa();
        break;

        //New Skills

    case DT_PET_DEVILION_ATK1:
    case DT_PET_DEVILION_ATK2:
        pDamageInstance->m_pOriginalDamage = new CHardDamageDevilion();
        break;

    case DT_BLOCK_BUSTER:
        pDamageInstance->m_pOriginalDamage = new CHardDamageBlockBuster();
        break;
    default:
        pDamageInstance->m_pOriginalDamage = new CDamage();
        break;
    }

    //������ ī���ؼ� �־��ش�.
    *(pDamageInstance->m_pOriginalDamage) = *m_vecInfo[What];

    return true;
}

bool CDamageManager::Add_LighteningBolt(int PosWho, CDamageInstance* pDamageInstance)
{

    // ���Ⱑ NULL�̸� �ȵ�..
    if (pDamageInstance == NULL)
        return NULL;

    if (CheckEnableCreate() == false)
        return false;

    if (pDamageInstance->m_ExceptPlayer[0] < 8)
        return false;

    for (int i = 0; i < 7; i++)
    {
        if (PosWho == pDamageInstance->m_ExceptPlayer[i])
            return false;
    }

    // ������ �´��� ���� ������ �߰��Ѵ�.
    float Angle;
    float Length;
    Length = D3DXVec2Length(&D3DXVECTOR2(g_MyD3D->MyPlayer[PosWho]->vPos.x - pDamageInstance->m_X, g_MyD3D->MyPlayer[PosWho]->vPos.y - pDamageInstance->m_Y));
    Angle = acosf((g_MyD3D->MyPlayer[PosWho]->vPos.x - pDamageInstance->m_X) / Length);
    if (g_MyD3D->MyPlayer[PosWho]->vPos.y - pDamageInstance->m_Y < 0)
        Angle = 6.2832f - Angle;

    // ���� ���ʷ������� ����
    float temp_Angle = pDamageInstance->m_Angle;

    // Angle���� ����� ��ġ�� �ٲ۴�.
    if (Angle > temp_Angle + 3.1416f)
        temp_Angle += 6.2832f;
    else if (Angle < temp_Angle - 3.1416f)
        temp_Angle -= 6.2832f;

    if (pDamageInstance->m_What == DT_LIGHTNING_GENERATE1 || pDamageInstance->m_What == DT_LIGHTNING_GENERATE1_BLUE)
        if (Angle < temp_Angle - 1.6f || Angle>temp_Angle + 1.6f)
            return false;
        else if (Angle < temp_Angle - 2.0f || Angle>temp_Angle + 2.0f)
            return false;


    CDamageInstance* pAddDamageInstance;
    pAddDamageInstance = new CDamageInstance();

    if (pDamageInstance->m_What == DT_LIGHTNING_GENERATE1_BLUE || pDamageInstance->m_What == DT_LIGHTNING_GENERATE2_BLUE)
    {
        if (!MakeDamage(DT_LIGHTNING_GENERATE2_BLUE, pAddDamageInstance))
        {
            SAFE_DELETE(pAddDamageInstance);
            return NULL;
        }
        pAddDamageInstance->m_What = DT_LIGHTNING_GENERATE2_BLUE;
    }
    else
    {
        if (!MakeDamage(DT_LIGHTNING_GENERATE2, pAddDamageInstance))
        {
            SAFE_DELETE(pAddDamageInstance);
            return NULL;
        }
        pAddDamageInstance->m_What = DT_LIGHTNING_GENERATE2;
    }

    CDamage* pDamage = m_vecInfo[DT_LIGHTNING_GENERATE2];

    pAddDamageInstance->m_Who = pDamageInstance->m_Who;
    pAddDamageInstance->m_Angle = Angle;

    pAddDamageInstance->m_iTeam = pDamageInstance->m_iTeam;
    pAddDamageInstance->m_IsRight = g_MyD3D->MyPlayer[PosWho]->bIsRight;
    pAddDamageInstance->m_Life = pDamage->m_Start_Life;
    pAddDamageInstance->m_Scale_x = pDamage->m_Start_Scale_x;

    pAddDamageInstance->m_ExtraMonsterNum = pDamageInstance->m_ExtraMonsterNum;
    pAddDamageInstance->m_IsMonsterDamage = pDamageInstance->m_IsMonsterDamage;

    pAddDamageInstance->m_X = g_MyD3D->MyPlayer[PosWho]->vPos.x;
    pAddDamageInstance->m_Y = g_MyD3D->MyPlayer[PosWho]->vPos.y + pDamage->m_Start_Locate_y;

    if (g_MyD3D->MyPlayer[PosWho]->IsLocalPlayer() || PosWho == 6)
        pAddDamageInstance->m_SlowCount = (LATENCY + SHOWLATENCY);
    else
        pAddDamageInstance->m_SlowCount = 0;

    pAddDamageInstance->m_IsShow = false;

    float X, Y;
    X = (pAddDamageInstance->m_X + pDamageInstance->m_X) / 2 - 1.0f;
    Y = (pAddDamageInstance->m_Y + pDamageInstance->m_Y) / 2 - 0.2f;

    float Rand = RANDOM_NUMBER;
    if (pDamageInstance->m_What == DT_LIGHTNING_GENERATE1_BLUE
        || pDamageInstance->m_What == DT_LIGHTNING_GENERATE2_BLUE)
    {
        if (Rand < 0.1f)
            pDamageInstance->Add(BLUE_LIGHTNING0, X, Y, pAddDamageInstance->m_SlowCount, .8f, X, Y, Angle - .7854f);
        else if (Rand < 0.2f)
            pDamageInstance->Add(BLUE_LIGHTNING4, X, Y, pAddDamageInstance->m_SlowCount, .8f, X, Y, Angle - .7854f);
    }
    else
    {
        if (Rand < 0.1f)
            pDamageInstance->Add(LIGHTNING0, X, Y, pAddDamageInstance->m_SlowCount, .8f, X, Y, Angle - .7854f);
        else if (Rand < 0.2f)
            pDamageInstance->Add(LIGHTNING4, X, Y, pAddDamageInstance->m_SlowCount, .8f, X, Y, Angle - .7854f);
    }

    if (pDamageInstance->m_What == DT_LIGHTNING_GENERATE1
        || pDamageInstance->m_What == DT_LIGHTNING_GENERATE1_BLUE)
    {
        pAddDamageInstance->m_ExceptPlayer[6] = PosWho;
        for (int i = 0; i < MAX_PLAYER_NUM; i++)
            pAddDamageInstance->m_ExceptPlayer[i] = 8;
    }
    else
    {
        for (int i = 0; i < MAX_PLAYER_NUM; i++)
            pAddDamageInstance->m_ExceptPlayer[i] = pDamageInstance->m_ExceptPlayer[i + 1];
        pAddDamageInstance->m_ExceptPlayer[6] = PosWho;
    }

    m_mapDamageInstance[pDamageInstance->m_What].push_back(pAddDamageInstance);

    return true;
}

void CDamageManager::ControlVelocityForReplay(CDamage* pDamage, int What)
{
    if (g_MyD3D->MyReplay->m_iCurrState != RP_VIEW)
        return;

    if (static_cast<int>(m_vecInfo.size()) <= What)
        return;

    CDamage* pOrigDamage = m_vecInfo[What];

    switch (g_MyD3D->MyReplay->m_eReplay_Speed)
    {
    case RS_STOP:
    {
        pDamage->m_Life_Change = pDamage->m_Life_Change; //�Ϻη� �̷���..
        //������ ���꿡�� �� ��쿡�� �������� �ٿ����� ���� ���̴�.
    }
    break;
    case RS_SLOW:
    {
        pDamage->m_x_Speed = pOrigDamage->m_x_Speed * 0.5f;
        pDamage->m_y_Speed = pOrigDamage->m_y_Speed * 0.5f;
        pDamage->m_Angle_Speed = pOrigDamage->m_Angle_Speed * 0.5f;
        pDamage->m_Life_Change = (pOrigDamage->m_Life_Change / 2 == 0) ? pOrigDamage->m_Life_Change : pOrigDamage->m_Life_Change / 2;
        pDamage->m_Scale_Speed = powf(pOrigDamage->m_Scale_Speed, 0.5f);
        pDamage->m_fAnimScaleX_Speed = powf(pOrigDamage->m_fAnimScaleX_Speed, 0.5f);
        pDamage->m_fAnimScaleY_Speed = powf(pOrigDamage->m_fAnimScaleY_Speed, 0.5f);
    }
    break;
    case RS_NORMAL:
    {
        pDamage->m_x_Speed = pOrigDamage->m_x_Speed;
        pDamage->m_y_Speed = pOrigDamage->m_y_Speed;
        pDamage->m_Angle_Speed = pOrigDamage->m_Angle_Speed;
        pDamage->m_Scale_Speed = pOrigDamage->m_Scale_Speed;
        pDamage->m_Life_Change = pOrigDamage->m_Life_Change;
        pDamage->m_fAnimScaleX_Speed = pOrigDamage->m_fAnimScaleX_Speed;
        pDamage->m_fAnimScaleY_Speed = pOrigDamage->m_fAnimScaleY_Speed;
    }
    break;
    case RS_FAST:
    {
        pDamage->m_x_Speed = pOrigDamage->m_x_Speed * 2.0f;
        pDamage->m_y_Speed = pOrigDamage->m_y_Speed * 2.0f;
        pDamage->m_Angle_Speed = pOrigDamage->m_Angle_Speed * 2.0f;
        pDamage->m_Life_Change = pOrigDamage->m_Life_Change * 2;
        pDamage->m_Scale_Speed = powf(pOrigDamage->m_Scale_Speed, 2.0f);
        pDamage->m_fAnimScaleX_Speed = powf(pOrigDamage->m_fAnimScaleX_Speed, 2.0f);
        pDamage->m_fAnimScaleY_Speed = powf(pOrigDamage->m_fAnimScaleY_Speed, 2.0f);
    }
    break;
    case RS_TOP:
    {
        pDamage->m_x_Speed = pOrigDamage->m_x_Speed * 4.0f;
        pDamage->m_y_Speed = pOrigDamage->m_y_Speed * 4.0f;
        pDamage->m_Angle_Speed = pOrigDamage->m_Angle_Speed * 4.0f;
        pDamage->m_Life_Change = pOrigDamage->m_Life_Change * 4;
        pDamage->m_Scale_Speed = powf(pOrigDamage->m_Scale_Speed, 4.0f);
        pDamage->m_fAnimScaleX_Speed = powf(pOrigDamage->m_fAnimScaleX_Speed, 4.0f);
        pDamage->m_fAnimScaleY_Speed = powf(pOrigDamage->m_fAnimScaleY_Speed, 4.0f);
    }
    break;
    }
}

BOOL CDamageManager::IsExistDamage(DAMAGE_TYPE eDamageType)
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor;
    for (itor = m_mapDamageInstance.begin(); itor != m_mapDamageInstance.end(); ++itor)
    {
        for (int i = 0; i < (int)itor->second.size(); i++)
        {
            if (itor->second[i]->m_What == (int)eDamageType)
                return TRUE;
        }
    }
    return FALSE;
}

BOOL CDamageManager::SetDamageLife(int iDamageType, int iLife)
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(iDamageType);
    if (itor != m_mapDamageInstance.end()) {
        for (int i = 0; i < (int)itor->second.size(); i++)
        {
            if (itor->second[i]->m_What == iDamageType)
            {
                itor->second[i]->m_Life = iLife;
                return TRUE;
            }
        }
    }
    return FALSE;
}

int CDamageManager::GetDamageLife(int iDamageType)
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(iDamageType);
    if (itor != m_mapDamageInstance.end()) {
        for (int i = 0; i < (int)itor->second.size(); i++)
        {
            if (itor->second[i]->m_What == iDamageType)
            {
                return itor->second[i]->m_Life;
            }
        }
    }
    return -1;
}

BOOL CDamageManager::IsExistDamage(CDamageInstance* pDamageInstance)
{
    if (pDamageInstance == NULL) {
        return FALSE;
    }

    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(pDamageInstance->m_What);
    if (itor != m_mapDamageInstance.end()) {
        for (int i = 0; i < (int)itor->second.size(); i++)
        {
            if (itor->second[i] == pDamageInstance)
                return TRUE;
        }
    }
    return FALSE;
}

void CDamageManager::DeleteInstance(CDamageInstance* pDamageInstance)
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(pDamageInstance->m_What);
    if (itor != m_mapDamageInstance.end()) {
        for (int i = 0; i < (int)itor->second.size(); i++)
        {
            if (itor->second[i] == pDamageInstance)
            {
                itor->second.erase(itor->second.begin() + i);
                Remove(pDamageInstance);

                if (itor->second.size() == 0)
                    m_mapDamageInstance.erase(itor);

                return;
            }
        }
    }
}

// �� �Լ��� ������ Life�� ���ش�.
void CDamageManager::DeleteInstance(int iDamageEnum_, int iPlayerIndex_)
{
    CDamageInstance* pDamageInstance = NULL;
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(iDamageEnum_);
    if (itor != m_mapDamageInstance.end()) {
        for (int i = 0; i < (int)itor->second.size(); i++) {
            pDamageInstance = itor->second[i];
            if (pDamageInstance->m_Who == iPlayerIndex_) {
                pDamageInstance->m_Life = 1;
                return;
            }
        }
    }
}

void CDamageManager::DeleteMonsterDamageToType(int iMonsterIdx, int iDamageType, int iEraseLife)
{
    m_mapEraseMonDamageList.insert(std::make_pair(std::make_pair(iMonsterIdx, iDamageType), iEraseLife));
}

void CDamageManager::DeleteMonsterDamage(int iMonsterIdx, int iDamageType)
{
    bool bMapIterAcc = false;

    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(iDamageType);

    if (itor != m_mapDamageInstance.end()) {
        bMapIterAcc = true;
        bool bVecIterAcc = false;
        std::vector<CDamageInstance*>::iterator vecIter = itor->second.begin();
        for (; vecIter < itor->second.end(); )
        {
            bVecIterAcc = true;
            if ((*vecIter)->m_IsMonsterDamage &&
                (*vecIter)->m_ExtraMonsterNum == iMonsterIdx &&
                (*vecIter)->m_What == iDamageType)
            {
                Remove((*vecIter));
                vecIter = itor->second.erase(vecIter);
                bVecIterAcc = false;

                if (itor->second.size() == 0)
                {
                    itor = m_mapDamageInstance.erase(itor);
                    bMapIterAcc = false;
                    break;
                }
            }

            if (bVecIterAcc)
                vecIter++;
        }
    }
}

bool CDamageManager::IsMonsterDamageToType(int iMonsterIdx, int iDamageType)
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.find(iDamageType);
    if (itor != m_mapDamageInstance.end()) {
        std::vector<CDamageInstance*>::iterator vecIter = itor->second.begin();
        for (; vecIter < itor->second.end(); ++vecIter)
        {
            if ((*vecIter)->m_IsMonsterDamage &&
                (*vecIter)->m_ExtraMonsterNum == iMonsterIdx &&
                (*vecIter)->m_What == iDamageType)
            {
                return true;
            }
        }
    }
    return false;
}

void CDamageManager::DeleteMonsterInstance()
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor;
    for (itor = m_mapDamageInstance.begin(); itor != m_mapDamageInstance.end(); ++itor)
    {
        for (int i = 0; i < (int)itor->second.size(); i++)
        {
            if (itor->second[i]->m_IsMonsterDamage == true)
            {
                Remove(itor->second[i]);
                itor->second.erase(itor->second.begin() + i);

                if (itor->second.size() == 0)
                    m_mapDamageInstance.erase(itor);

                return;
            }
        }
    }
}

void CDamageManager::EffectDust(float fX, float fY, CDamageInstance* pDamageInstance, int iLatency/* = 0*/, bool bRound/* = false*/)
{
    if (SiKGCFantasticMap()->IsWaterStage() && SiKGCFantasticMap()->GetO2Manager())
    {
        if (pDamageInstance->m_IsMonsterDamage)
        {
            if (SiKGCFantasticMap()->GetO2Manager()->IsInWaterMonster(pDamageInstance->m_ExtraMonsterNum))
            {
                g_MyD3D->MySparks->WaterSmogs(fX, fY, static_cast<unsigned char>(iLatency), bRound);
            }
        }
        else
        {
            if (SiKGCFantasticMap()->GetO2Manager()->IsInWater(pDamageInstance->m_Who))
                g_MyD3D->MySparks->WaterSmogs(fX, fY, iLatency, bRound);
        }
    }
    else if (g_MyD3D->MyStg->GetStage() >= GC_GS_QUEST_ZERO_A && g_MyD3D->MyStg->GetStage() <= GC_GS_QUEST_ZERO_C)
    {
        g_MyD3D->MySparks->LeafSmogs(fX, fY, iLatency, bRound);
    }
    else if (g_MyD3D->MyStg->GetStage() >= GC_GS_CASTUL_TEMPLE_UNDER_A && g_MyD3D->MyStg->GetStage() <= GC_GS_CASTUL_TEMPLE_UNDER_L)
    {
        g_MyD3D->MySparks->UndeadSmogs(fX, fY, iLatency, bRound);
    }
    else
    {
        g_MyD3D->MySparks->Smogs(fX, fY, iLatency, bRound);
    }
}

bool CDamageManager::IsCrashCheckPerFrame(CDamageInstance* pDamageInstance)
{
    // [9/17/2007] breadceo. �� ������ �浹üũ�� �������� ����ϼ���
    if ((GetOriginalDamage(pDamageInstance->m_What)->m_iHitPerFrame == 1) || pDamageInstance->IsHitFrame())
    {
        return true;
    }
    else
    {
        return false;
    }

}

void CDamageManager::LoadMeshInfo(KLuaManager& luaMgr, MESH_INFO& meshInfo)
{
    if (SUCCEEDED(luaMgr.BeginTable("MESHINFO")))
    {
        if (SUCCEEDED(luaMgr.BeginTable("MESHLIST")))
        {
            meshInfo.m_vecStrMesh.clear();
            meshInfo.m_vecStrTex.clear();
            meshInfo.m_vecStrName.clear();
            meshInfo.m_veciCartoon.clear();
            meshInfo.m_veciTechnique.clear();
            std::string strMesh;
            std::string strTex;
            std::string strID;
            int iCartoon;
            int iTechnique;
            for (int i = 1; ; ++i)
            {
                if (SUCCEEDED(luaMgr.BeginTable(i)))
                {
                    LUA_GET_VALUE_DEF(1, strMesh, "NULL");
                    LUA_GET_VALUE_DEF(2, strTex, "NULL");
                    LUA_GET_VALUE_DEF(3, strID, "");
                    LUA_GET_VALUE_DEF(4, iCartoon, 0);
                    LUA_GET_VALUE_DEF(5, iTechnique, -1);
                    meshInfo.m_vecStrMesh.push_back(strMesh);
                    meshInfo.m_vecStrTex.push_back(strTex);
                    meshInfo.m_vecStrName.push_back(strID);
                    meshInfo.m_veciCartoon.push_back(iCartoon);
                    meshInfo.m_veciTechnique.push_back(iTechnique);
                    luaMgr.EndTable();
                }
                else
                    break;

            }
            luaMgr.EndTable();
        }


        if (SUCCEEDED(luaMgr.BeginTable("MOTIONLIST")))
        {
            meshInfo.m_vecStrMotion.clear();
            std::string strMotion;
            for (int i = 1; ; ++i)
            {
                LUA_GET_VALUE_NOASSERT(i, strMotion, break);
                meshInfo.m_vecStrMotion.push_back(strMotion);
            }
            luaMgr.EndTable();
        }
        LUA_GET_VALUE_DEF("SCALE", meshInfo.m_fScale, CHAR_SCALE_RATIO);
        LUA_GET_VALUE_DEF("ISSTOPMOTION", meshInfo.m_bIsStopMotion, false);
        LUA_GET_VALUE_DEF("TECHNIQUE", meshInfo.m_iTechnique, -1);
        LUA_GET_VALUE_DEF("ADDNODIRECTION", meshInfo.m_bAlwaysRight, false);
        LUA_GET_VALUE_DEF("USE_AFTER_IMAGE", meshInfo.m_iAfterImage, -1);
        LUA_GET_VALUE_DEF("USE_FACE_LIFT", meshInfo.m_iFaceLift, -1);

        if (SUCCEEDED(luaMgr.BeginTable("LOCALPOS")))
        {
            LUA_GET_VALUE_DEF(1, meshInfo.m_vLocalPos.x, 0.0f);
            LUA_GET_VALUE_DEF(2, meshInfo.m_vLocalPos.y, 0.0f);
            LUA_GET_VALUE_DEF(3, meshInfo.m_vLocalPos.z, 0.5f);
            luaMgr.EndTable();
        }

        if (SUCCEEDED(luaMgr.BeginTable("MESH_CHILDPARTICLE")))
        {
            MESH_CHILD_PARTICLE kTemp;
            meshInfo.m_vecMeshChildParticle.clear();
            for (int i = 1; ; ++i)
            {
                if (SUCCEEDED(luaMgr.BeginTable(i)))
                {
                    kTemp.Init();
                    LUA_GET_VALUE(1, kTemp.m_strParticle, return);
                    LUA_GET_VALUE(2, kTemp.m_fOffSetX, return);
                    LUA_GET_VALUE(3, kTemp.m_fOffSetY, return);
                    LUA_GET_VALUE(4, kTemp.m_iCreateFrame, return);
                    LUA_GET_VALUE(5, kTemp.m_iMontionIndex, return);
                    LUA_GET_VALUE_DEF(6, kTemp.m_bDieTogether, false);
                    LUA_GET_VALUE_DEF(7, kTemp.m_iBoneIndex, -1);
                    LUA_GET_VALUE_DEF(8, kTemp.m_bDotParticle, false);
                    LUA_GET_VALUE_DEF(9, kTemp.m_fTraceTime, -1);
                    LUA_GET_VALUE_DEF(7, kTemp.m_iBoneIndex, -1);
                    LUA_GET_VALUE_DEF(8, kTemp.m_bDotParticle, false);

                    meshInfo.m_vecMeshChildParticle.push_back(kTemp);
                    luaMgr.EndTable();
                }
                else
                    break;

            }
            luaMgr.EndTable();
        }
        if (SUCCEEDED(luaMgr.BeginTable("MESH_SOUNDLIST")))
        {
            MESH_SOUNDLIST kTemp;
            meshInfo.m_vecMeshSoundList.clear();
            for (int i = 1; ; ++i)
            {
                if (SUCCEEDED(luaMgr.BeginTable(i)))
                {
                    kTemp.Init();
                    LUA_GET_VALUE(1, kTemp.m_strSound, return);
                    LUA_GET_VALUE(2, kTemp.m_iStartFrame, return);
                    LUA_GET_VALUE(3, kTemp.m_iMontionIndex, return);

                    meshInfo.m_vecMeshSoundList.push_back(kTemp);
                    luaMgr.EndTable();
                }
                else
                    break;
            }
            luaMgr.EndTable();
        }
        if (SUCCEEDED(luaMgr.BeginTable("MESH_CREATEDAMAGE")))
        {
            MESH_CREATEDAMAGE kTemp;
            meshInfo.m_vecMeshCreateDamage.clear();
            for (int i = 1; ; ++i)
            {
                if (SUCCEEDED(luaMgr.BeginTable(i)))
                {
                    kTemp.Init();
                    LUA_GET_VALUE(1, kTemp.m_iDamageIndex, return);
                    LUA_GET_VALUE(2, kTemp.m_fOffSetX, return);
                    LUA_GET_VALUE(3, kTemp.m_fOffSetY, return);
                    LUA_GET_VALUE(4, kTemp.m_iCreateFrame, return);
                    LUA_GET_VALUE(5, kTemp.m_iMontionIndex, return);
                    LUA_GET_VALUE_DEF(6, kTemp.m_bRightLeftOffset, false);

                    meshInfo.m_vecMeshCreateDamage.push_back(kTemp);
                    luaMgr.EndTable();
                }
                else
                    break;

            }
            luaMgr.EndTable();
        }

        if (SUCCEEDED(luaMgr.BeginTable("ANIMATION")))
        {
            int iLife;
            int iMotionIdx;
            meshInfo.m_vecAnimInfo.clear();
            for (int i = 1; ; ++i)
            {
                if (SUCCEEDED(luaMgr.BeginTable(i)))
                {
                    LUA_GET_VALUE_DEF(1, iLife, 0);
                    LUA_GET_VALUE_DEF(2, iMotionIdx, 0);

                    meshInfo.m_vecAnimInfo.push_back(std::make_pair(iLife, iMotionIdx));
                    luaMgr.EndTable();
                }
                else
                    break;


            }
            luaMgr.EndTable();
        }
        luaMgr.EndTable();

    }

}

void CDamageManager::CreateMeshAnimation(CDamageInstance* pDamage)
{
    if (pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh.empty())
        return;

    SAFE_DELETE(pDamage->m_p3DObject);
    pDamage->m_p3DObject = new KGC3DObject();
    pDamage->m_p3DObject->Init(pDamage->m_pOriginalDamage->m_MeshInfo.m_iTechnique);

    for (int i = 0; i < (int)pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh.size(); ++i)
    {
        if (pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh[i] == "WEAPON" ||
            pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh[i] == "M_WEAPON" ||
            pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh[i] == "FULL_EQUIP")
        {
            if (pDamage->m_IsMonsterDamage == true)
            {
                assert(!"do not set ""weapon"" to monster damage!");
                continue;
            }
            PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamage->m_Who];
            KItemIDVector& vecItem = pPlayer->GetCurrentChar().GetEquipmentForLoadModel();

            KItemIDVector::iterator vit;
            for (vit = vecItem.begin(); vit != vecItem.end(); ++vit)
            {
                GCItem* pItem = g_pItemMgr->GetItemData(*vit);

                SCharInfo& CharInfo = pPlayer->GetCurrentChar();
                int iPromotion = (pPlayer->Extra_Char_Num - CharInfo.iCharType) / GC_CHAR_NUM;

                int iItemID = g_pItemMgr->GetSameImageIndex(*vit);

                if (pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh[i] == "FULL_EQUIP")
                {
                    // SceondBaseMesh ���� ������ SecondeBaseMesh�� ó��
                    if (pPlayer->CheckTransformPetSkill()) {
                        std::vector< ADD_MESH >::const_iterator itAddMesh = pPlayer->GetCurFormTemplate().vecAddMesh.begin();
                        for (; itAddMesh != pPlayer->GetCurFormTemplate().vecAddMesh.end(); ++itAddMesh) {
                            if (itAddMesh->bIsSecondBaseMesh && pPlayer->IsToggleSecondBaseMesh(itAddMesh->strID.c_str())) {
                                pDamage->m_p3DObject->AddMesh(itAddMesh->strMeshName, itAddMesh->strTextureName, itAddMesh->strID);
                            }

                            // �� ó�� 
                            if (pPlayer->GetCurFormTemplate().mapFaceLiftMesh.empty() == false) {
                                int iFaceLiftType = FACE_LIFT_NORMAL;
                                if (pDamage->m_pOriginalDamage->m_MeshInfo.m_iFaceLift != -1) {
                                    iFaceLiftType = pDamage->m_pOriginalDamage->m_MeshInfo.m_iFaceLift;
                                }

                                std::map< int, FACE_LIFT_MESH >::iterator mit = pPlayer->GetCurFormTemplate().mapFaceLiftMesh.find(iFaceLiftType);
                                if (mit != pPlayer->GetCurFormTemplate().mapFaceLiftMesh.end() && !pPlayer->IsRemovePartsItem(HEAD)) {
                                    pDamage->m_p3DObject->AddMesh(mit->second.strMeshName, mit->second.strTextureName);
                                }
                            }
                        }
                        break;
                    }

                    std::string strLive = g_pGCDeviceManager2->GetLiveAbtaName(iItemID, CharInfo.iCharType, iPromotion, ".p3m");
                    if (!strLive.empty())
                    {
                        GCITEMID gcLiveTextureID = g_pItemMgr->GetSameTextureIndex(iItemID);
                        std::string strLiveTex = g_pGCDeviceManager2->GetLiveAbtaName(gcLiveTextureID, CharInfo.iCharType, iPromotion, ".dds");
                        pDamage->m_p3DObject->AddMesh(strLive, strLiveTex);
                    }

                    bool bBigHead = false;
                    if ((pItem->dwSlotPosition & ESP_A_DOWN_HELMET) && (pItem->dwSlotPosition & ESP_A_UPPER_HELMET)) {
                        bBigHead = true;
                    }

                    // �� ���� �޽��� �ִٸ� �ش� �޽��� ���� ��ü�Ѵ�.
                    if (pPlayer->GetCurFormTemplate().mapFaceLiftMesh.empty() == false) {
                        int iFaceLiftType = -1;
                        if (bBigHead) {
                            iFaceLiftType = FACE_LIFT_BIG_HEAD;
                        }
                        else if (pItem->dwSlotPosition & ESP_HELMET) {
                            iFaceLiftType = FACE_LIFT_NORMAL;
                            if (pDamage->m_pOriginalDamage->m_MeshInfo.m_iFaceLift != -1) {
                                iFaceLiftType = pDamage->m_pOriginalDamage->m_MeshInfo.m_iFaceLift;
                            }
                        }

                        if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&pPlayer->m_kUserInfo.GetCurrentChar(), ITEMID_ASIN_SHAADOW_SET, true))
                        {
                            iFaceLiftType = FACE_LIFT_SHADOW;
                        }

                        std::map< int, FACE_LIFT_MESH >::iterator mit = pPlayer->GetCurFormTemplate().mapFaceLiftMesh.find(iFaceLiftType);
                        if (mit != pPlayer->GetCurFormTemplate().mapFaceLiftMesh.end() && !pPlayer->IsRemovePartsItem(HEAD))
                        {
                            pDamage->m_p3DObject->AddMesh(mit->second.strMeshName, mit->second.strTextureName);
                        }
                    }

                    if (pItem->dwSlotPosition & ESP_WEAPON)
                    {
                        if (CharInfo.iCharType == GC_CHAR_ASIN)
                        {
                            if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&pPlayer->m_kUserInfo.GetCurrentChar(), ITEMID_ASIN_SHAADOW_SET, true))
                            {
                                pDamage->m_p3DObject->AddMesh("Asin_HandProtect.p3m", "Asin_HandProtect_Shadow.dds");
                            }
                            else
                            {
                                pDamage->m_p3DObject->AddMesh("Asin_HandProtect.p3m", "Asin_HandProtect.dds");
                            }
                        }
                    }

                    g_pItemMgr->CreateAbtaModel(iItemID, CharInfo.iCharType, iPromotion, pDamage->m_p3DObject->GetGCObject());
                }
                else {
                    if (pItem->dwSlotPosition & ESP_WEAPON)
                    {
                        if (pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh[i] == "WEAPON")
                        {
                            std::string strLive = g_pGCDeviceManager2->GetLiveAbtaName(iItemID, CharInfo.iCharType, iPromotion, ".p3m");
                            if (!strLive.empty())
                            {
                                GCITEMID gcLiveTextureID = g_pItemMgr->GetSameTextureIndex(iItemID);
                                std::string strLiveTex = g_pGCDeviceManager2->GetLiveAbtaName(gcLiveTextureID, CharInfo.iCharType, iPromotion, ".dds");
                                pDamage->m_p3DObject->AddMesh(strLive, strLiveTex);
                            }
                            g_pItemMgr->CreateAbtaModel(iItemID, CharInfo.iCharType, iPromotion, pDamage->m_p3DObject->GetGCObject());
                        }
                        else if (pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh[i] == "M_WEAPON")
                        {
                            std::string strLive = g_pGCDeviceManager2->GetLiveAbtaName(iItemID, CharInfo.iCharType, iPromotion, "_m.p3m");
                            if (!strLive.empty())
                            {
                                GCITEMID gcLiveTextureID = g_pItemMgr->GetSameTextureIndex(iItemID);
                                std::string strLiveTex = g_pGCDeviceManager2->GetLiveAbtaName(gcLiveTextureID, CharInfo.iCharType, iPromotion, "_m.dds");
                                pDamage->m_p3DObject->AddMesh(strLive, strLiveTex);
                            }
                            g_pItemMgr->CreateLas4MergeAbtaModel(iItemID, CharInfo.iCharType, pDamage->m_p3DObject->GetGCObject());
                        }
                        else {

                        }
                    }
                }
            }
            continue;
        }

        pDamage->m_p3DObject->SetCartoon(EGCCartoonTexture(pDamage->m_pOriginalDamage->m_MeshInfo.m_veciCartoon[i]));
        pDamage->m_p3DObject->AddMesh(pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMesh[i],
            pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrTex[i], pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrName[i], pDamage->m_pOriginalDamage->m_MeshInfo.m_veciTechnique[i]);
    }

    for (int i = 0; i < (int)pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMotion.size(); ++i)
    {
        pDamage->m_p3DObject->AddMotion(pDamage->m_pOriginalDamage->m_MeshInfo.m_vecStrMotion[i]);
    }

    pDamage->m_p3DObject->SetMotionFromIndex(0);
    pDamage->m_p3DObject->SetScale(pDamage->m_pOriginalDamage->m_MeshInfo.m_fScale);
    D3DXVECTOR3 vecLocalPos = pDamage->m_pOriginalDamage->m_MeshInfo.m_vLocalPos;
    if (false == pDamage->m_IsRight)
    {
        vecLocalPos.x *= -1.0f;
    }
    pDamage->m_p3DObject->SetLocalPos(vecLocalPos);
    pDamage->m_p3DObject->SetRender(false);
    pDamage->m_p3DObject->SetShellMatrixScale(pDamage->m_pOriginalDamage->m_MeshInfo.m_fScale);

    // �ܻ���
    if (-1 < pDamage->m_pOriginalDamage->m_MeshInfo.m_iAfterImage &&
        !g_MyD3D->m_kAfterImageRenderer.IsRegisterObject(pDamage->m_p3DObject->GetGCObject()))
        g_MyD3D->m_kAfterImageRenderer.RegisterObject(pDamage->m_p3DObject->GetGCObject(), pDamage->m_pOriginalDamage->m_MeshInfo.m_iAfterImage);

    SiKGC3DObjectMgr()->Add3DObject(pDamage->m_p3DObject);
}

void CDamageManager::CreateMeshAnimationForPrevLoad(int iWhat, int iPidx)
{
    CDamage* pDamage = GetOriginalDamage(iWhat);

    if (!pDamage)
        return;

    if (pDamage->m_MeshInfo.m_vecStrMesh.empty())
        return;

    KGC3DObject* p3DObject = new KGC3DObject();
    p3DObject->Init(pDamage->m_MeshInfo.m_iTechnique);

    std::stringstream stm1, stm2;
    for (int i = 0; i < (int)pDamage->m_MeshInfo.m_vecStrMesh.size(); ++i)
    {
        if (pDamage->m_MeshInfo.m_vecStrMesh[i] == "WEAPON" ||
            pDamage->m_MeshInfo.m_vecStrMesh[i] == "M_WEAPON" ||
            pDamage->m_MeshInfo.m_vecStrMesh[i] == "FULL_EQUIP")
        {
            //��? �̰� �̹� �����ϰ� �����ϱ� �����ε� ���ص� �ȴٰ�
        }
        else
        {
            p3DObject->SetCartoon(EGCCartoonTexture(pDamage->m_MeshInfo.m_veciCartoon[i]));
            p3DObject->AddMesh(pDamage->m_MeshInfo.m_vecStrMesh[i], pDamage->m_MeshInfo.m_vecStrTex[i], pDamage->m_MeshInfo.m_vecStrName[i], pDamage->m_MeshInfo.m_veciTechnique[i]);
        }
    }

    p3DObject->SetShellMatrixScale(pDamage->m_MeshInfo.m_fScale);


    for (int i = 0; i < (int)pDamage->m_MeshInfo.m_vecStrMotion.size(); ++i)
    {
        p3DObject->AddMotion(pDamage->m_MeshInfo.m_vecStrMotion[i]);
    }

    p3DObject->SetScale(pDamage->m_MeshInfo.m_fScale);
    p3DObject->SetLocalPos(pDamage->m_MeshInfo.m_vLocalPos);
    p3DObject->SetRender(false);

    SiKGC3DObjectMgr()->Add3DObjectForPrevLoad(p3DObject);
}

void CDamageManager::LoadTimeSound(KLuaManager& luaMgr, std::vector< TIME_SOUND >& vecTimesound)
{
    LUA_BEGIN_TABLE("TIME_SOUND", return)
    {
        vecTimesound.clear();
        TIME_SOUND kTemp;
        bool bExit;

        for (int i = 1; ; ++i)
        {
            bExit = false;
            LUA_BEGIN_TABLE(i, break)
            {
                kTemp.Init();
                LUA_GET_VALUE(1, kTemp.m_iSoundNumber, bExit = true);
                LUA_GET_VALUE(2, kTemp.m_iPlayTime, bExit = true);
                LUA_GET_VALUE_DEF(3, kTemp.m_bLoop, false);
                LUA_GET_VALUE_DEF(4, kTemp.m_bStop, false);

                if (bExit == false)
                    vecTimesound.push_back(kTemp);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(return);
}

void CDamageManager::LoadAutoDamage(KLuaManager& luaMgr, std::vector< AUTO_DAMAGE >& vecAutoDamageInfo)
{
    LUA_BEGIN_TABLE("AUTO_DAMAGE", return)
    {
        vecAutoDamageInfo.clear();
        AUTO_DAMAGE kTemp;
        bool bExit;
        for (int i = 1; ; ++i)
        {
            bExit = false;
            LUA_BEGIN_TABLE(i, break)
            {
                kTemp.Init();
                LUA_GET_VALUE(1, kTemp.m_iDamageIndex, bExit = true);
                LUA_GET_VALUE(2, kTemp.m_fOffSetX, bExit = true);
                LUA_GET_VALUE(3, kTemp.m_fOffSetY, bExit = true);
                LUA_GET_VALUE(4, kTemp.m_iStartLifeTime, bExit = true);
                LUA_GET_VALUE_DEF(5, kTemp.m_bRenderToContactPos, false);

                LUA_GET_VALUE(6, kTemp.m_dwAutoDamage, bExit = true);
                LUA_GET_VALUE_DEF(7, kTemp.m_iCrashStartTime, -1);
                LUA_GET_VALUE_DEF(8, kTemp.m_bDieCurDamage, false);
                LUA_GET_VALUE_DEF(9, kTemp.m_bNoSlowCount, false);
                LUA_GET_VALUE_DEF(10, kTemp.m_bNoClear, false);

                if (bExit == false)
                    vecAutoDamageInfo.push_back(kTemp);

            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(return);
}

void CDamageManager::LoadTargetAutoDamage(KLuaManager& luaMgr, std::vector< TARGET_AUTO_DAMAGE >& vecTargetAutoDamageInfo)
{
    LUA_BEGIN_TABLE("TARGET_AUTO_DAMAGE", return)
    {
        vecTargetAutoDamageInfo.clear();
        TARGET_AUTO_DAMAGE kTemp;
        bool bExit;
        for (int i = 1; ; ++i)
        {
            bExit = false;
            LUA_BEGIN_TABLE(i, break)
            {
                kTemp.Init();
                LUA_GET_VALUE_DEF(1, kTemp.m_iDamageIndex, -1);
                LUA_GET_VALUE_DEF(2, kTemp.m_fOffSetX, 0.0f);
                LUA_GET_VALUE_DEF(3, kTemp.m_fOffSetY, 0.0f);
                LUA_GET_VALUE_DEF(4, kTemp.m_iStartLifeTime, 0);
                LUA_GET_VALUE_DEF(5, kTemp.m_bDieCurDamage, false);
                LUA_GET_VALUE_DEF(6, kTemp.m_iCharIndex, -1);
                LUA_GET_VALUE_DEF(7, kTemp.m_bNoTargetMonster, false);

                vecTargetAutoDamageInfo.push_back(kTemp);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(return);
}

void CDamageManager::LoadCrashParticle(KLuaManager& luaMgr, std::vector< CRASH_PARTICLE >& vecCrashInfo)
{
    LUA_BEGIN_TABLE("CRASH_PARTICLE", return)
    {
        vecCrashInfo.clear();
        CRASH_PARTICLE kTemp;
        bool bExit;

        for (int i = 1; ; ++i)
        {
            bExit = false;
            LUA_BEGIN_TABLE(i, break)
            {
                kTemp.Init();
                LUA_GET_VALUE(1, kTemp.m_strParticle, bExit = true);
                LUA_GET_VALUE(2, kTemp.m_fXOffset, bExit = true);
                LUA_GET_VALUE(3, kTemp.m_fYOffset, bExit = true);
                LUA_GET_VALUE_DEF(4, kTemp.m_bRenderToContactPosition, false);
                LUA_GET_VALUE_DEF(5, kTemp.m_iCreateLifeTime, -1);
                LUA_GET_VALUE_DEF(6, kTemp.m_bGroundCheck, false);
                LUA_GET_VALUE_DEF(7, kTemp.m_bSelfDirection, false);
                if (bExit == false)
                    vecCrashInfo.push_back(kTemp);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(return);
}

void CDamageManager::LoadTrigger(KLuaManager& luaMgr, std::vector< DAMAGE_TRIGGER >& vecDamageTrigger)
{
    LUA_BEGIN_TABLE("TRIGGER", return)
    {
        vecDamageTrigger.clear();
        DAMAGE_TRIGGER kTemp;

        for (int i = 1; ; ++i)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                luaMgr.GetValue(1, kTemp.TriggerType);
                luaMgr.GetValue(2, kTemp.val1);
                luaMgr.GetValue(3, kTemp.val2);
                luaMgr.GetValue(4, kTemp.strTrigger);
                luaMgr.GetValue(5, kTemp.TriggerID);
                luaMgr.GetValue(6, kTemp.strVal1);
                luaMgr.GetValue(7, kTemp.bNoCheckActive);

                vecDamageTrigger.push_back(kTemp);
            }
            LUA_END_TABLE(break);
        }
    }
    LUA_END_TABLE(return);

}

void CDamageManager::LoadOnlyMotionDamage(KLuaManager& luaMgr, OnlyMotionDamage& kOnlyMotion)
{
    LUA_BEGIN_TABLE("ONLY_MOTION_DAMAGE", return)
    {
        std::pair<int, bool> piTemp;

        luaMgr.GetValue("TRUE_TO_LIFE", kOnlyMotion.m_iTrueToLife);
        luaMgr.GetValue("FALSE_TO_LIFE", kOnlyMotion.m_iFalseToLife);
        luaMgr.GetValue("OPERATION", kOnlyMotion.m_iOperation);

        LUA_BEGIN_TABLE("CONDITION", return)
        {
            for (int iLoop = 1; ; ++iLoop)
            {
                LUA_BEGIN_TABLE(iLoop, break)
                {
                    luaMgr.GetValue(1, piTemp.first);
                    luaMgr.GetValue(2, piTemp.second);

                    kOnlyMotion.m_vecCondition.push_back(piTemp);
                }
                LUA_END_TABLE(break);
            }
        }
        LUA_END_TABLE(return);
    }
    LUA_END_TABLE(return);
}

bool CDamageManager::PushCheckWithPlayer(int iPlayerIndex)
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.begin();
    CDamageInstance* pDamageInstance;

    while (itor != m_mapDamageInstance.end())
    {
        for (int j = (int)itor->second.size() - 1; j >= 0; --j)
        {
            pDamageInstance = itor->second[j];
            int i = iPlayerIndex;

            //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
            if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
                && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
                continue;

            if (!pDamageInstance->m_pOriginalDamage->m_bNoPassDamage)
                continue;

            if (pDamageInstance->m_Who == i)
                continue;

            // �÷��̾ ���� �浹 ���� ������ ���� �и��� �ʵ��� ��.
            if (g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_ARENA) ||
                g_MyD3D->MyPlayer[i]->IsNoCrashCheckMotion(g_MyD3D->MyPlayer[i]->uiMotion) ||
                g_MyD3D->MyPlayer[i]->m_bNoCrashCheck ||
                !g_MyD3D->MyPlayer[i]->m_bCrashCheck ||
                g_MyD3D->MyPlayer[i]->m_bNoCrashCheckByBuff) {
                continue;
            }


            //if( g_MyD3D->GetMyPlayer()->m_kUserInfo.iTeam == g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.iTeam )
            //	continue;
            if (g_MyD3D->IsSameTeam(pDamageInstance->m_Who, i))
                continue;
            //if( !g_MyD3D->MyPlayer[i]->m_bCrashCheck )
            //{

            float fRangeX = 0.5f;
            float fRangeY = 0.4f;

            GCCollisionRect<float> kPlayerRect, kDamageRect, kCollision;

            kPlayerRect.SetRect(g_MyD3D->MyPlayer[i]->vPos.x - 0.15f,
                g_MyD3D->MyPlayer[i]->vPos.y,
                g_MyD3D->MyPlayer[i]->vPos.x + 0.15f,
                g_MyD3D->MyPlayer[i]->vPos.y - 0.3f);

            // �Ʒ� �ڵ�� �޽ð� �����̴´�� �ڽ�ũ�Ⱑ ���Ͽ�(�޽ð� ������ ������ �ڽ��� ���� ������ ���´�) 
            // Push�� �ȵǾ��� ��Ȳ������ Push�� �Ǿ� ��ġ�� ���������� ������ �ֵ�.
            //kPlayerRect.SetRect( g_MyD3D->MyPlayer[i]->MyBody.x + 1.0f, // LEFT
            //                     g_MyD3D->MyPlayer[i]->MyBody.w,        // TOP(+)
            //                     g_MyD3D->MyPlayer[i]->MyBody.y + 1.0f, // RIGHT
            //                     g_MyD3D->MyPlayer[i]->MyBody.z );      // BOTTOM(-)

            kDamageRect.SetRect(pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX,
                pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.25f,
                pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX,
                pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.35f);

            if (kPlayerRect.CheckCollision(kDamageRect, &kCollision))
            {
                float fGap = 0.135f;
                if (kDamageRect.GetCenterX() > kPlayerRect.GetCenterX())
                {
                    if (false == g_MyD3D->MyPlayer[i]->GetIsRight())
                        fGap = (kPlayerRect.GetWidth() / 2.0f) + 0.015f;
                    g_MyD3D->MyPlayer[i]->vPos.x = kDamageRect.m_Left - fGap;
                }
                else
                {
                    if (g_MyD3D->MyPlayer[i]->GetIsRight())
                        fGap = (kPlayerRect.GetWidth() / 2.0f) + 0.015f;
                    g_MyD3D->MyPlayer[i]->vPos.x = kDamageRect.m_Right + fGap;
                }
                return true;
            }
        }
        ++itor;
    }
    return false;
}

bool CDamageManager::PushCheckWithMonster(int iMonsterIndex)
{
    std::map< int, std::vector<CDamageInstance*> >::iterator itor = m_mapDamageInstance.begin();
    CDamageInstance* pDamageInstance;

    while (itor != m_mapDamageInstance.end())
    {
        for (int j = (int)itor->second.size() - 1; j >= 0; --j)
        {
            pDamageInstance = itor->second[j];
            int i = iMonsterIndex;
            MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
            if (pMonster == NULL) return false;

            if (MONSTER::MON_BOSS == pMonster->m_iBossMonster)
                continue;

            if (pDamageInstance->m_IsMonsterDamage)
            {
                if (g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, i))
                    continue;
            }
            else
            {
                if (g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, i))
                    continue;
            }

            if (!pMonster->m_bLive)
                continue;

            if (!pDamageInstance->m_pOriginalDamage->m_bNoPassDamage)
                continue;

            const SMonsterTemplate& sMonsterTable = g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType];
            if (sMonsterTable.bNoPushCheck == false)
                continue;

            //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
            if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
                && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
                continue;


            GCCollisionRect<float> rtFire;
            rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f - 1.0f;
            rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f - 1.0f;
            rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.4f - 0.26f;
            rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.4f - 0.26f;

            for (int k = 0; k < pMonster->GetPushRectNum(); k++)
            {
                GCCollisionRect<float> rtMonster = pMonster->GetPushBoundRect(k);

                if (rtFire.m_Bottom < rtMonster.m_Top && rtFire.m_Top > rtMonster.m_Bottom)
                {
                    if (rtFire.m_Left < rtMonster.m_Right && rtFire.m_Right > rtMonster.m_Left)
                    {
                        float fObjectLeft = pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f;
                        float fObjectRight = pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f;

                        if ((fObjectLeft + fObjectRight) / 2.0f > pMonster->GetX())
                        {
                            pMonster->SetX(fObjectLeft - 0.16f);
                        }
                        else
                        {
                            pMonster->SetX(fObjectLeft + 0.16f);
                        }
                        return true;
                    }
                }
            }
        }
        ++itor;
    }
    return false;
}

void CDamageManager::CrashWith2DObject(CDamageInstance* pDamageInstance)
{
    //ġ���ϴ� �͵��� ����� ����.
    if ((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE) || (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS))
        return;

    // 2D Object�� �浹ó�� ���� �ʴ´�.
    if (pDamageInstance->m_pOriginalDamage->m_bNoCrash2DObject)
        return;

    //ũ���� ������ �ȿ� �־�� �浹�㰡�ȴ�.
    if (!(pDamageInstance->m_Life <= pDamageInstance->m_pOriginalDamage->m_iCrashStart
        && pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_iCrashEnd))
        return;

    if (pDamageInstance->IsHitPerFrame() == false)
        return;

    // ������ �����ϰ��� �浹�ϴ� �������� ������
    if (false == pDamageInstance->m_pOriginalDamage->m_vecCrashDamageToMonsterType.empty())
        return;

    //bool bCurMonsterRight = g_MyD3D->m_pObject[i]->m_abIsRight[LATENCY_INDEX];    // CrashPlayer �Լ��� ȣ��Ǹ� �ٶ󺸴� ������ �ٲ�Ƿ� �̸� �޾Ƶδ� ��.

    float fRangeX = 0.0f;
    float fRangeY = 0.0f;
    if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
    {
        fRangeX = 1.0f;
        fRangeY = 1.0f;
    }
    else
    {
        fRangeX = 0.5f;
        fRangeY = 0.4f;
    }

    GCCollisionRect<float> rtFire;
    rtFire.m_Left = pDamageInstance->m_X - pDamageInstance->m_Scale_x * fRangeX - 1.0f;
    rtFire.m_Right = pDamageInstance->m_X + pDamageInstance->m_Scale_x * fRangeX - 1.0f;
    rtFire.m_Top = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;
    rtFire.m_Bottom = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY - 0.26f;

    bool bCrash = false;
    GCCollisionRect<float> rtObjectRect;
    std::list< KGC2DObject* >::iterator lit;

    for (Itor2DObject itor = g_MyD3D->Begin2DObject(); itor != g_MyD3D->End2DObejct(); ++itor)
    {
        rtObjectRect = (*itor)->GetCollisionRect();

        if (!rtObjectRect.CheckCollision(rtFire, NULL))
            continue;

        if ((*itor)->Damaged(rtFire, -1.0f * pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer, pDamageInstance->m_What, pDamageInstance->m_Who, pDamageInstance->m_IsMonsterDamage))
        {
            rtObjectRect = (*itor)->GetCollisionRect();

            CallEffAtCrash(pDamageInstance, -1, false, rtObjectRect.m_Left + 1.0f, rtObjectRect.m_Top + 0.25f);

            if (((pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONETIME_DAMAGE) || (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE)) && pDamageInstance->m_Life > 1)
            {
                pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
                pDamageInstance->m_Life = 1;
            }

            pDamageInstance->m_pOriginalDamage->CrashEffect(pDamageInstance);
            bCrash = true;
            if (g_MyD3D->GetMyPlayer() && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer()) {
                g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk(pDamageInstance->m_What);
                g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk(pDamageInstance->m_What);
                g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk(pDamageInstance->m_What);
            }
        }
    }

    if (bCrash) {
        g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk(pDamageInstance->m_What, true);
        g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk(pDamageInstance->m_What, true);
    }
}

OUT CDamageInstance* CDamageManager::CreateDamage(int What, int Who, bool NoSlowCount, int ExtraMonsterNum, IN CDamageInstance* pDamageInstance_, DIRECTION_INFO* stDirect)
{
    if (CheckEnableCreate() == false)
        return NULL;

    if (What < 0 || static_cast<int>(m_vecInfo.size()) <= What)
        return false;

    std::map<int, int>::iterator mapIter = m_vecInfo[What]->m_mapNoCreateExceptionGameMode.find(SiKGCRoomManager()->GetGameMode());
    if (mapIter != m_vecInfo[What]->m_mapNoCreateExceptionGameMode.end())
        return false;

    //����̸� ���°�, �׷��� �ٲ�� �� �ʿ伺�� �ִ��� üũ�غ���.
    CheckAndChange(What, Who);
    MONSTER* pMonster = g_kMonsterManager.GetMonster(ExtraMonsterNum);
    if (pMonster != NULL) {
        CheckMonsterAndChange(What, pMonster->m_iMonsterType);
    }

    CDamageInstance* pDamageInstance = NULL;
    if (pDamageInstance_ == NULL)
    {
        pDamageInstance = new CDamageInstance();
    }
    else
    {
        pDamageInstance = pDamageInstance_;
    }

    // ���Ⱑ NULL�̸� �ȵ�..
    if (pDamageInstance == NULL)
        return NULL;

    if ( !MakeDamage( What, pDamageInstance ) )
    {
        if ( !pDamageInstance_ )
        {
            SAFE_DELETE( pDamageInstance );
        }
        return NULL;
    }

    pDamageInstance->m_pOriginalDamage->CustomizeBeforeCreate( pDamageInstance );

    //����Ʈ�� ��Ʈ�� �ϵ� �ڵ�
    for (int i = 0; i < 7; ++i)
    {
        pDamageInstance->m_ExceptPlayer[i] = 8;
    }

    pDamageInstance->m_What = What;
    pDamageInstance->m_Who = Who;
    pDamageInstance->m_ExtraMonsterNum = ExtraMonsterNum;

    pDamageInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_Start_Life;
    pDamageInstance->m_Scale_x = pDamageInstance->m_pOriginalDamage->m_Start_Scale_x;

    pDamageInstance->m_fAnimScaleX = pDamageInstance->m_pOriginalDamage->m_fAnim_Start_ScaleX;
    pDamageInstance->m_fAnimScaleY = pDamageInstance->m_pOriginalDamage->m_fAnim_Start_ScaleY;
    pDamageInstance->m_TextureNoRenderCnt = pDamageInstance->m_pOriginalDamage->m_TextureNoRenderCnt;

    if (Who == MAX_PLAYER_NUM)
    {
        if (pMonster)  // who�� max��� �� ���� �Ŵ� �ƴѰ� ����.
        {
            pDamageInstance->m_iTeam = pMonster->m_iTeam;
            pDamageInstance->m_IsRight = ((stDirect == NULL) ? pMonster->m_abIsRight[0] : stDirect->m_bIsRight);
            //pDamageInstance->m_IsRight      = g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight;
            pDamageInstance->m_IsMonsterDamage = true;

            PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(pMonster->m_dwOwnerUID);
            if (pPlayer)
            {
                SCharInfo charinfo = pPlayer->GetCurrentChar();
                pDamageInstance->m_pOriginalDamage->m_fDamageToMonster *= ((charinfo.iLevel + 1) * 0.5f);
                pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer *= ((charinfo.iLevel + 1) * 0.5f);
            }
        }

        //if ( What == DT_SANDRULER_ATK4_LAUNCHER || What == DT_SUPERMON_SANDRULER_ATK4_LAUNCHER  )
        //	pDamageInstance->m_ExtraMonsterNum = 0;

        // �޽ø� ���ִ� ����̴�
        if (pDamageInstance->m_pOriginalDamage->m_bToggleMesh)
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(ExtraMonsterNum);
            if (pMonster)
            {
                pMonster->ToggleExtraMeshAll(false);
                pMonster->ToggleBaseMesh(false);
                pMonster->ToggleMonsterInfo(false);
                pMonster->SetNoTargettingFrame(pDamageInstance->m_pOriginalDamage->m_Start_Life);
                pDamageInstance->m_pOriginalDamage->InToggleMeshDamageEffect(pDamageInstance, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()));
            }
        }
    }
    else
    {
        if (g_MyD3D->IsPlayerIndex(Who) == true)   // ������ �����ڵ� �������ֻ�..
        {
            pDamageInstance->m_iTeam = g_MyD3D->MyPlayer[Who]->m_kUserInfo.iTeam;
            pDamageInstance->m_IsRight = ((stDirect == NULL) ? g_MyD3D->MyPlayer[Who]->bIsRight : stDirect->m_bIsRight);

            // �޽ø� ���ִ� ����̴�
            if (pDamageInstance->m_pOriginalDamage->m_bToggleMesh)
            {
                g_MyD3D->MyPlayer[Who]->SetMeshToggleState(true);
                g_MyD3D->MyPlayer[Who]->ToggleExtraMeshAll(false);
                g_MyD3D->MyPlayer[Who]->ToggleBaseMesh(false);

                D3DXVECTOR3 vPos = g_MyD3D->MyPlayer[Who]->GetPosition();
                if (vPos)
                    pDamageInstance->m_pOriginalDamage->InToggleMeshDamageEffect(pDamageInstance, D3DXVECTOR2(vPos.x, vPos.y));
            }

            // ��ȯ������ �޽ø� ���ִ� ����̴�.
            if (pDamageInstance->m_pOriginalDamage->m_iToggleSummonMonsterID)
            {
                std::vector<MONSTER*> vecMonster = g_MyD3D->MyPlayer[Who]->GetMySummonMonster(EGCMonster(pDamageInstance->m_pOriginalDamage->m_iToggleSummonMonsterID));
                std::vector<MONSTER*>::iterator vecIter = vecMonster.begin();
                for (; vecIter != vecMonster.end(); ++vecIter)
                {
                    (*vecIter)->ToggleExtraMeshAll(false);
                    (*vecIter)->ToggleBaseMesh(false);
                    (*vecIter)->ToggleMonsterInfo(false);
                    (*vecIter)->SetNoTargettingFrame(pDamageInstance->m_pOriginalDamage->m_Start_Life);
                    pDamageInstance->m_pOriginalDamage->InToggleSummonMonsterMeshDamageEffect(pDamageInstance, D3DXVECTOR2((*vecIter)->GetX(), (*vecIter)->GetY()));
                }
            }
        }
    }

    if (!NoSlowCount
        && (Who == MAX_PLAYER_NUM || g_MyD3D->MyPlayer[Who]->IsLocalPlayer()) // ������ �ٲٸ� ũ���� �ȳ��� ������?
        ) //���͸� ���ο� ī��Ʈ�� �شٰ�?
        pDamageInstance->m_SlowCount = (LATENCY + SHOWLATENCY);
    else
        pDamageInstance->m_SlowCount = 0;

    pDamageInstance->m_IsShow = false;

    if (pDamageInstance->m_bPetDamage)
    {
        if (Who == g_MyD3D->Get_MyPlayer())
        {
            pDamageInstance->m_pOriginalDamage->m_fDamageToMonster += pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * SiGCPetMagicStone()->GetGlyphEffectAmmount(PET_MAGIC_STONE_EFFECT_TYPE::PMSET_INCREASE_PET_DAMAGE);
        }
    }

    if (pDamageInstance->m_pOriginalDamage->m_iMaxInstanceNum)
    {
        std::vector<CDamageInstance*>& vecFullDamageInstance = m_mapDamageInstance[pDamageInstance->m_What];
        std::vector<CDamageInstance*> vecTargetDamageInstance;
        CDamageInstance* pUniqueDamage = NULL;
        int counter;
        for (counter = (int)m_mapDamageInstance[pDamageInstance->m_What].size() - 1; counter >= 0; --counter)
        {
            pUniqueDamage = vecFullDamageInstance[counter];

            // ������� �ٸ��� �Ѿ��
            if (pDamageInstance->m_Who != pUniqueDamage->m_Who)
                continue;

            // �ٸ� ������ �Ѿ
            if (pDamageInstance->m_What != pUniqueDamage->m_What)
                continue;

            // ���� ������!              
            vecTargetDamageInstance.push_back(pUniqueDamage);
        }

        int iVictim = vecTargetDamageInstance.size() - pDamageInstance->m_pOriginalDamage->m_iMaxInstanceNum;

        ++iVictim;

        if (iVictim > 0)
        {
            for (int i = 1; i <= iVictim; ++i)
            {
                int iIdx = vecTargetDamageInstance.size() - i;
                if (iIdx >= 0) {
                    vecTargetDamageInstance[iIdx]->m_Life = 1;
                }
            }
        }
    }

    m_mapDamageInstance[pDamageInstance->m_What].push_back(pDamageInstance);

    CreateMeshAnimation(pDamageInstance);

#ifdef __ORIGIN_DEV__
    if (g_kGlobalValue.m_bStartCollectDamageList &&
        g_MyD3D->GetMyPlayer() &&
        g_MyD3D->GetMyPlayer()->m_iPlayerIndex == Who) {
        g_kGlobalValue.SavePlayerMotionDamageList(What);

        for (std::vector< AUTO_DAMAGE >::iterator vit = pDamageInstance->m_pOriginalDamage->m_vecAutoDamage.begin(); vit != pDamageInstance->m_pOriginalDamage->m_vecAutoDamage.end(); ++vit) {
            g_kGlobalValue.SavePlayerMotionDamageList(vit->m_iDamageIndex);
            g_MyD3D->GetMyPlayer()->AddDamage(vit->m_iDamageIndex, 0);
        }
    }
#endif

    return pDamageInstance;
}

void CDamageManager::ActivateTrigger(CDamageInstance* pDamageInstance, int iTriggerIndex)
{
    std::vector< DAMAGE_TRIGGER >& vecTrigger = pDamageInstance->m_pOriginalDamage->m_vecTrigger;

    if (vecTrigger[iTriggerIndex].TriggerID)
    {
        for (unsigned int i = 0; i < vecTrigger.size(); ++i)
        {
            if (false == vecTrigger[i].bNoCheckActive && vecTrigger[i].TriggerID == vecTrigger[iTriggerIndex].TriggerID)
            {
                vecTrigger[i].bActivated = true;
            }
        }
    }
    m_pLuaMgr->BeginTable(vecTrigger[iTriggerIndex].strTrigger.c_str());
    int iOldLife = pDamageInstance->m_pOriginalDamage->m_Start_Life;
    DWORD iOldFlag = pDamageInstance->m_pOriginalDamage->m_dwDMAttribute;

    LoadFireInfoSingleObject(*m_pLuaMgr, pDamageInstance->m_pOriginalDamage);

    if (iOldLife != pDamageInstance->m_pOriginalDamage->m_Start_Life)
    {
        pDamageInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_Start_Life;
    }

    if (0 < pDamageInstance->m_pOriginalDamage->m_Reverse_Life)
    {
        pDamageInstance->m_Life = pDamageInstance->m_pOriginalDamage->m_Reverse_Life;
    }

    if ((iOldFlag & DA_ONEHIT_DAMAGE) != (pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE))
    {
        pDamageInstance->EraseAlreadyDamaged();
    }
    m_pLuaMgr->EndTable();

}
bool CDamageManager::CheckBoundary(CDamageInstance* pDamageInstance)
{
    float fRangeX = 0.0f;
    float fRangeY = 0.0f;
    if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
    {
        fRangeX = 1.0f;
        fRangeY = 1.0f;
    }
    else
    {
        fRangeX = 0.5f;
        fRangeY = 0.4f;
    }

    float fMax_X = SiKGCRoomManager()->IsMonsterGameMode() ? SiKGCFantasticMap()->GetPixelWidth() : 1.5f * (float)g_MyD3D->MyStg->Num_Width;
    float fMax_Y = SiKGCRoomManager()->IsMonsterGameMode() ? SiKGCFantasticMap()->GetPixelHeight() : 1.5f * (float)g_MyD3D->MyStg->Num_Height;

    bool bStopX = false;
    if (pDamageInstance->m_X < 0.0f)
    {
        pDamageInstance->m_X = 0.0f + pDamageInstance->m_Scale_x * fRangeX;
        bStopX = true;
    }
    else if (pDamageInstance->m_X > (fMax_X - 0.1f))
    {
        pDamageInstance->m_X = fMax_X - pDamageInstance->m_Scale_x * fRangeX;
        bStopX = true;
    }

    if (bStopX)
    {
        pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.000001f;
    }

    bool bStopY = false;
    if (pDamageInstance->m_Y < 0.0f)
    {
        pDamageInstance->m_Y = 0.0f + pDamageInstance->m_pOriginalDamage->m_Y_per_x * fRangeY;
        bStopY = true;
    }

    if (bStopY)
    {
        pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.000001f;
        pDamageInstance->m_pOriginalDamage->m_fGravity = 0.0f;
    }

    if (bStopX || bStopY) {
        return true;
    }
    else {
        return false;
    }
}

void CDamageManager::ReflectionBoundary(CDamageInstance* pDamageInstance)
{
    if (2 >= pDamageInstance->m_Life)
        return;

    float fRangeX = 0.0f;
    float fRangeY = 0.0f;
    BoundaryReflection kRefInfo = pDamageInstance->m_pOriginalDamage->m_kBoundaryReflection;
    if (pDamageInstance->m_pOriginalDamage->m_bIsTotalRange)
    {
        fRangeX = 1.0f;
        fRangeY = 1.0f;
    }
    else
    {
        fRangeX = 0.5f;
        fRangeY = 0.4f;
    }

    float fMax_X = SiKGCRoomManager()->IsMonsterGameMode() ? SiKGCFantasticMap()->GetPixelWidth() : 1.5f * (float)g_MyD3D->MyStg->Num_Width;
    float fMax_Y = SiKGCRoomManager()->IsMonsterGameMode() ? SiKGCFantasticMap()->GetPixelHeight() : 1.5f * (float)g_MyD3D->MyStg->Num_Height;

    bool bXReflection = false, bYReflection = false;
    if (pDamageInstance->m_X < kRefInfo.m_fLeftSideOffset)
        bXReflection = true;
    else if (pDamageInstance->m_X > ((fMax_X - 0.1f) + kRefInfo.m_fRightSideOffset))
        bXReflection = true;
    if (pDamageInstance->m_Y < kRefInfo.m_fBottomOffset)
        bYReflection = true;
    else if (pDamageInstance->m_Y > kRefInfo.m_fTop)
        bYReflection = true;

    if (bXReflection)
    {
        if (kRefInfo.m_bEndDamage)
        {
            pDamageInstance->m_Life = 2;
        }
        else
        {
            pDamageInstance->m_pOriginalDamage->m_x_Speed *= -1.0f;
            pDamageInstance->m_pOriginalDamage->m_x_Speed *= kRefInfo.m_fReflectionDecreaseRadio;
        }
    }

    if (bYReflection)
    {
        if (kRefInfo.m_bEndDamage)
        {
            pDamageInstance->m_Life = 2;
        }
        else
        {
            pDamageInstance->m_pOriginalDamage->m_y_Speed *= -1.0f;
            pDamageInstance->m_pOriginalDamage->m_y_Speed *= kRefInfo.m_fReflectionDecreaseRadio;
        }
    }
}

bool CDamageManager::CheckLastBottom(float fPosX, float fPosY)
{
    if (SiKGCFantasticMap()->IsLastBottomPosition(fPosX, fPosY))
        return true;
    float fLastPos = SiKGCFantasticMap()->GetLastBottomPosition(fPosX, fPosY);
    if (fPosY < fLastPos)
        return true;
    return false;
}

D3DXVECTOR2 CDamageManager::SyncDamagePosAtCrashPos(CDamageInstance* pDamageInstance, int HitWho, bool bIsCrashPlayer)
{
    SYNC_DAMAGE_POS_AT_CRASH_POS kInfo = pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos;

    if (bIsCrashPlayer) {
        kInfo.m_fOffsetX = g_MyD3D->MyPlayer[HitWho]->bIsRight ? kInfo.m_fOffsetX : -kInfo.m_fOffsetX;
    }
    else {
        MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
        if (pMonster != NULL) {
            kInfo.m_fOffsetX = pMonster->GetIsRight() ? kInfo.m_fOffsetX : -kInfo.m_fOffsetX;
        }
    }

    D3DXVECTOR2 vPos(0.f, 0.f);
    D3DXVECTOR2 vWhoPos(0.f, 0.f);
    switch (kInfo.m_iSyncType)
    {
    case SYNC_POS_TOP:
        vPos.x = pDamageInstance->m_X + kInfo.m_fOffsetX;
        vPos.y = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.8f + kInfo.m_fOffsetY;
        break;
    case SYNC_POS_BOTTOM:
        vPos.x = pDamageInstance->m_X + kInfo.m_fOffsetX;
        vPos.y = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.8f + kInfo.m_fOffsetY;
        break;
    case SYNC_POS_LEFT:
        vPos.x = pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f + kInfo.m_fOffsetX;
        vPos.y = pDamageInstance->m_Y + kInfo.m_fOffsetY;
        break;
    case SYNC_POS_RIGHT:
        vPos.x = pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f + kInfo.m_fOffsetX;
        vPos.y = pDamageInstance->m_Y + kInfo.m_fOffsetY;
        break;
    case SYNC_POS_CENTER:
        vPos.x = pDamageInstance->m_X + kInfo.m_fOffsetX;
        vPos.y = pDamageInstance->m_Y + kInfo.m_fOffsetY;
    default:
        if (bIsCrashPlayer) {
            vWhoPos.x = g_MyD3D->MyPlayer[HitWho]->vPos.x;
            vWhoPos.y = g_MyD3D->MyPlayer[HitWho]->vPos.y;
        }
        else {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(HitWho);
            if (pMonster != NULL) {
                vWhoPos.x = pMonster->m_afX[0];
                vWhoPos.y = pMonster->m_afY[0];
            }
        }

        if (pDamageInstance->m_X > vWhoPos.x) {
            vPos.x = pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f + kInfo.m_fOffsetX;
        }
        else {
            vPos.x = pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f + kInfo.m_fOffsetX;
        }

        if (pDamageInstance->m_Y > vWhoPos.y) {
            vPos.y = pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.8f + kInfo.m_fOffsetY;
        }
        else {
            vPos.y = pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.8f + kInfo.m_fOffsetY;
        }
        break;
    }

    return vPos;
}

bool CDamageManager::IsAlreadyCrashDamage(CDamageInstance* pDamageInstance, int iHitWho)
{
    bool bContinue = false;

    std::map< int, std::vector< CDamageInstance* > >::iterator mapIter = m_mapDamageInstance.find(pDamageInstance->m_What);
    if (mapIter != m_mapDamageInstance.end()) {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for (; vecIter != mapIter->second.end(); ++vecIter)
        {
            if ((iHitWho == (*vecIter)->m_HitWho) && ((*vecIter)->m_What == pDamageInstance->m_pOriginalDamage->m_iUniqueDamage))
            {
                bContinue = true;
                break;
            }
        }
    }

    return bContinue;
}

void CDamageManager::GetKeyDamageInstance(CDamageInstance* pDamageInstance, std::pair< int, int >& prKeyDamageInstance)
{
    if (pDamageInstance->m_IsMonsterDamage) {
        prKeyDamageInstance = std::make_pair(pDamageInstance->m_What, pDamageInstance->m_Who + pDamageInstance->m_ExtraMonsterNum);
    }
    else {
        if (pDamageInstance->m_Who == MAX_PLAYER_NUM && pDamageInstance->m_ExtraMonsterNum == -1) {
            prKeyDamageInstance = std::make_pair(pDamageInstance->m_What, pDamageInstance->m_ExtraMonsterNum);
        }
        else {
            prKeyDamageInstance = std::make_pair(pDamageInstance->m_What, pDamageInstance->m_Who);
        }
    }
}

int CDamageManager::GetDamageInstanceCount(int iDamageEnum, int iLife)
{
    int iCount = 0;
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = m_mapDamageInstance.begin();
    for (; mapIter != m_mapDamageInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for (; vecIter != mapIter->second.end(); ++vecIter)
        {
            if ((*vecIter)->m_What == iDamageEnum && (*vecIter)->m_Life > iLife)
            {
                ++iCount;
            }
        }
    }

    return iCount;
}

D3DXVECTOR2 CDamageManager::GetDamageInstancePos(int iDamageEnum)
{
    int iCount = 0;
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = m_mapDamageInstance.begin();
    for (; mapIter != m_mapDamageInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for (; vecIter != mapIter->second.end(); ++vecIter)
        {
            if ((*vecIter)->m_What == iDamageEnum)
            {
                return D3DXVECTOR2((*vecIter)->m_X, (*vecIter)->m_Y);
            }
        }
    }

    return D3DXVECTOR2(0.0f, 0.0f);
}

void CDamageManager::CheckTargetAutoDamage(CDamageInstance* pDamageInstance, int iHitWho, bool IsCrashPlayer)
{
    if (false == pDamageInstance->m_pOriginalDamage->m_vecTargetAutoDamage.empty())
    {
        // ���ǵ� Ÿ�Ͽ��䵥���� �ݺ�
        std::vector< TARGET_AUTO_DAMAGE >::iterator vecIter = pDamageInstance->m_pOriginalDamage->m_vecTargetAutoDamage.begin();
        for (; vecIter != pDamageInstance->m_pOriginalDamage->m_vecTargetAutoDamage.end(); ++vecIter)
        {
            bool bAddDamage = false;
            CDamageInstance* pAuto = NULL;

            // StartLife�� -1�̸� ũ���� �Ǿ����� ����( ũ���� �� �༮�� )
            if (-1 == vecIter->m_iStartLifeTime)
            {
                if (IsCrashPlayer)
                {
                    if (g_MyD3D->IsPlayerIndex(iHitWho))
                    {
                        D3DXVECTOR3 vPos = g_MyD3D->MyPlayer[iHitWho]->GetPosition();
                        bAddDamage = AddTargetAutoDamage(pDamageInstance, &(*vecIter), vPos.x, vPos.y, iHitWho, false);
                    }
                }
                else
                {
                    if (g_kMonsterManager.IsMonsterIndex(iHitWho))
                    {
                        MONSTER* pMonster = g_kMonsterManager.GetMonster(iHitWho);
                        bAddDamage = AddTargetAutoDamage(pDamageInstance, &(*vecIter), pMonster->GetX(), pMonster->GetY(), iHitWho, true);
                    }
                }
            }
            // StartLife�� -1�� �ƴϸ� ������ Ÿ�ӿ� ���� ����( ��ü Ȥ�� ������ �༮�� )
            else if (pDamageInstance->m_Life == vecIter->m_iStartLifeTime)
            {
                if (-1 == vecIter->m_iCharIndex)
                {
                    for (int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop)
                    {
                        PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];
                        if (NULL == pPlayer)
                            continue;
                        if (pPlayer->m_kUserInfo.bLive == false)
                            continue;
                        if (pPlayer->m_cLife == 0)
                            continue;
                        if (pPlayer->CheckHiding())
                            continue;
                        if (pPlayer->m_ePlayerState == PS_DEAD)
                            continue;
                        if (false == pDamageInstance->m_IsMonsterDamage)
                        {
                            if (g_MyD3D->IsSameTeam_PlayerAndMonster(pPlayer->m_iPlayerIndex, pDamageInstance->m_ExtraMonsterNum))
                                continue;
                        }
                        else
                        {
                            if (g_MyD3D->IsSameTeam(pPlayer->m_iPlayerIndex, pDamageInstance->m_Who))
                                continue;
                        }

                        D3DXVECTOR3 vPos = pPlayer->GetPosition();
                        bAddDamage = AddTargetAutoDamage(pDamageInstance, &(*vecIter), vPos.x, vPos.y, iLoop, false);
                    }

                    if (false == vecIter->m_bNoTargetMonster)
                    {
                        MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin();
                        for (; mit != g_kMonsterManager.m_mapMonster.end(); ++mit)
                        {
                            MONSTER* pMonster = mit->second;
                            if (pMonster == NULL || !pMonster->IsLive())
                                continue;
                            if (pDamageInstance->m_IsMonsterDamage)
                            {
                                if (g_MyD3D->IsSameTeam_MonsterAndMonster(pMonster->m_iMonsterIndex, pDamageInstance->m_ExtraMonsterNum))
                                    continue;
                            }
                            else
                            {
                                if (g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, pMonster->m_iMonsterIndex))
                                    continue;
                            }
                            if (NPC_NONE != pMonster->m_eNPC)
                                continue;
                            if (pMonster->m_bMonsterTarget == false)
                                continue;

                            bAddDamage = AddTargetAutoDamage(pDamageInstance, &(*vecIter), pMonster->GetX(), pMonster->GetY(), pMonster->m_iMonsterIndex, true);
                        }
                    }
                }
                else
                {
                    if (g_MyD3D->IsPlayerIndex(vecIter->m_iCharIndex))
                    {
                        D3DXVECTOR3 vPos = g_MyD3D->MyPlayer[vecIter->m_iCharIndex]->GetPosition();
                        bAddDamage = AddTargetAutoDamage(pDamageInstance, &(*vecIter), vPos.x, vPos.y, vecIter->m_iCharIndex, false);
                    }
                }
            }

            if (bAddDamage && vecIter->m_bDieCurDamage)
            {
                pDamageInstance->m_Life = 2;
            }
        }
    }
}

bool CDamageManager::AddTargetAutoDamage(CDamageInstance* pDamageInstance, TARGET_AUTO_DAMAGE* pAutoInfo, float fPosX, float fPosY, int iHitWho, bool bHitMonster)
{
    if (NULL == pAutoInfo)
        return false;

    if (NULL == pDamageInstance)
        return false;

    CDamageInstance* pAuto = NULL;
    if (false == pDamageInstance->m_IsMonsterDamage)
    {
        pAuto = AddWithLocate(pAutoInfo->m_iDamageIndex, pDamageInstance->m_Who, fPosX + pAutoInfo->m_fOffSetX, fPosY + pAutoInfo->m_fOffSetY,
            false, -1, NULL, iHitWho, NULL, OBJ_TYPE_PLAYER, bHitMonster ? OBJ_TYPE_MONSTER : OBJ_TYPE_PLAYER);
    }
    else
    {
        pAuto = AddWithLocate(pAutoInfo->m_iDamageIndex, pDamageInstance->m_Who, fPosX + pAutoInfo->m_fOffSetX, fPosY + pAutoInfo->m_fOffSetY,
            false, pDamageInstance->m_ExtraMonsterNum, NULL, iHitWho, NULL, OBJ_TYPE_MONSTER, bHitMonster ? OBJ_TYPE_MONSTER : OBJ_TYPE_PLAYER);
    }

    if (pAuto)
    {
        pAuto->m_bPetDamage = pDamageInstance->m_bPetDamage;
        pAuto->m_IsRight = pDamageInstance->m_IsRight;
        pAuto->m_SlowCount = 0;
        return true;
    }
    return false;
}

void CDamageManager::CheckSyncPosDamageAtCrash(CDamageInstance* pDamageInstance)
{
    std::vector<int>::iterator vecMonsterIter = pDamageInstance->m_vecMonsterSyncDamagePosAtCrashPos.begin();
    for (; vecMonsterIter != pDamageInstance->m_vecMonsterSyncDamagePosAtCrashPos.end(); ++vecMonsterIter)
    {
        MONSTER* pMonster = g_kMonsterManager.GetMonster((*vecMonsterIter));
        if (NULL == pMonster)
            continue;

        if (false == pMonster->IsLive())
            continue;

        D3DXVECTOR2 vPos = SyncDamagePosAtCrashPos(pDamageInstance, (*vecMonsterIter), false);
        pMonster->SetX(vPos.x);
        pMonster->SetY(vPos.y);
    }

    std::vector<int>::iterator vecPlayerIter = pDamageInstance->m_vecPlayerSyncDamagePosAtCrashPos.begin();
    for (; vecPlayerIter != pDamageInstance->m_vecPlayerSyncDamagePosAtCrashPos.end(); ++vecPlayerIter)
    {
        if (false == g_MyD3D->IsPlayerIndex((*vecPlayerIter)))
            continue;

        PLAYER* pPlayer = g_MyD3D->MyPlayer[(*vecPlayerIter)];
        D3DXVECTOR2 vPos = SyncDamagePosAtCrashPos(pDamageInstance, (*vecPlayerIter), true);
        pPlayer->vPos.x = vPos.x;
        pPlayer->vPos.y = vPos.y;
    }
}

bool CDamageManager::CheckDamagePerFrame(CDamageInstance* pDamageInstance)
{
    switch (pDamageInstance->m_What)
    {
    case DT_LAS4_COMBO_ATK1:
    case DT_LAS4_COMBO_ATK2:
    case DT_LAS4_COMBO_ATK3:
    case DT_LAS4_COMBO_ATK4:
    case DT_LAS4_CRITICIAL_DOUBLE_ATK:
    case DT_LAS4_DASH_ATK:
    case DT_LAS4_DOUBLE_DASH_ATK1:
    case DT_LAS4_DOUBLE_DASH_ATK2:
    case DT_LAS4_BOUNSE_ATK:
    case DT_LAS4_JUMP_ATK:
    case DT_LAS4_STANDUP_ATK:
    case DT_LAS4_SPECIAL1_ATK:
    case DT_LAS4_SPECIAL1_DOWN:
    case DT_LAS4_SPECIAL2_NANDO:
    case DT_LAS4_SPECIAL2_FINISH:
    case DT_LAS4_SPECIAL3_ATK1:
    case DT_LAS4_SPECIAL3_ATK2:
    case DT_LAS4_SPECIAL3_FINISH:
        return true;
    }
    return false;
}