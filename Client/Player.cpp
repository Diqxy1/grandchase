#include "stdafx.h"
//


#include "KDInput.h"

#include "KGCPlayerCommon.h"
#include "KGCPlayerElesis.h"
#include "KGCPlayerElesis2.h"
#include "KGCPlayerLire.h"
#include "KGCPlayerLire2.h"
#include "KGCPlayerLire3.h"
#include "KGCPlayerArme.h"
#include "KGCPlayerArme2.h"
#include "DamageManager.h"
#include "Spark.h"
#include "Message.h"


#include "Controls.h"
#include "Headup Display.h"
#include "QuestGameManager.h"

#include "Item.h"
#include "Monster.h"
//
////
#include "Stage.h"
#include "./KncP2PLib/KncP2P.h"

//
#include "PetAI.h"
#include "KGCGameBoard.h"

#include "GCCameraOrtho.h"
#include "Replay.h"
//
//

//



#include "./gcui/GCGameOverlayUI.h"

#include "GCRenderManager.h"
//
#include "MapSystem/GCFantasticMap.h"

#include "MapSystem/KGCLayerPart.h"
//
#include "KGCRoomManager.h"

#include "KGCEmoticon.h"

#include "KGCMinigameManager.h"
#include "buff/KGCBuffManager.h"

#include "GCEventManager.h"
#include "GCCoupleSystem.h"
#include "../KGCValentineDayManager.h"
#include "ClientErr.h"

#include "lua_tinker.h"
#include "GCObjectAndMotion.h"
#include "KStats.h"
#include "GCFileMoniter.h"
#include "GCObjectAndMotion.h"
#include "GCAchieveManager.h"

#include "GCSkill.h"
#include "KGCObjectEgo.h"
#include "GCUI/GCOnGameExpBar.h"

#include "KGCAgitGameManager.h"
#include "GCEclipseEventManager.h"

#include "../GCUI/KGCCharDependentGauge.h"
#include "../GCUI/KGCSavingGauge.h"

#include "KGCWorldMapManager.h"
#include "KGCWeatherSystem.h"
#include "KGCFaceLift.h"

#include "KGCSocialMotionManager.h"
#include "KGCChannelManager.h"
#include "GCAutoMatchManager.h"

#include "KGCEmbarkManager.h"



#include "GCBuffDonationManager.h"
#include "GCUI/KGCInGameMessage.h"
#include "KGCSubjectManager.h"
#include "GCGrowingSkillManager.h"
#include "mission/GCMissionManager.h"
#include <GCRoomAnimationManager.h>

#define EnumToString(s) #s
#define LIRE3SPECIAL2DURING ((DWORD)55*2.5)

static unsigned short Item_Time[GC_GAME_ITEM_NUM] = { 100, 550, 800, 550, 550, 100, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, };
static int iHistoryTime;

std::map<MOTIONID, PLAYERSTATEFUNCTION> PLAYER::ms_mapStateFunction;
bool PLAYER::m_bAttackSwoosh = false;
bool bReload = false;
void InitPlayerFileMoniter()
{
    bReload = true;
}

void PLAYER::RegisterLuabind()
{
    PLAYER::SetStateFunctions();

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

        // 클래스 함수 바인딩
        lua_tinker::class_add<kEmbarkArena>(L, "kEmbarkArena");
    lua_tinker::class_con<kEmbarkArena>(L, lua_tinker::constructor<>());
    lua_tinker::class_mem<kEmbarkArena>(L, "m_iEmbarkType", &kEmbarkArena::m_iEmbarkType);
    lua_tinker::class_mem<kEmbarkArena>(L, "m_bRight", &kEmbarkArena::m_bRight);
    lua_tinker::class_mem<kEmbarkArena>(L, "m_fPosX", &kEmbarkArena::m_fPosX);
    lua_tinker::class_mem<kEmbarkArena>(L, "m_fPosY", &kEmbarkArena::m_fPosY);
    lua_tinker::class_mem<kEmbarkArena>(L, "m_bUseArenaZoom", &kEmbarkArena::m_bUseArenaZoom);
    lua_tinker::class_def<kEmbarkArena>(L, "Init", &kEmbarkArena::Init);
    lua_tinker::class_def<kEmbarkArena>(L, "IsSetting", &kEmbarkArena::IsSetting);

    lua_tinker::class_add<PLAYER>(L, "PLAYER");
    lua_tinker::class_inh<PLAYER, KGCObj>(L);
    lua_tinker::class_mem<PLAYER>(L, "m_bRender", &PLAYER::m_bRender);
    lua_tinker::class_mem<PLAYER>(L, "m_bCrashCheck", &PLAYER::m_bCrashCheck);
    lua_tinker::class_mem<PLAYER>(L, "Next_Attack_Count", &PLAYER::Next_Attack_Count);
    lua_tinker::class_mem<PLAYER>(L, "Super", &PLAYER::Super);
    lua_tinker::class_mem<PLAYER>(L, "x_Speed", &PLAYER::x_Speed);
    lua_tinker::class_mem<PLAYER>(L, "y_Speed", &PLAYER::y_Speed);
    lua_tinker::class_mem<PLAYER>(L, "Fall_Speed", &PLAYER::m_fFall_Speed);
    lua_tinker::class_mem<PLAYER>(L, "m_bRenderLire3Arrow", &PLAYER::m_bRenderLire3Arrow);
    lua_tinker::class_mem<PLAYER>(L, "StoneCurseType", &PLAYER::m_iStoneCurseType);
    lua_tinker::class_mem<PLAYER>(L, "MetamorphosisForm", &PLAYER::MetamorphosisForm);

    lua_tinker::class_mem<PLAYER>(L, "m_bDrawSlashRight", &PLAYER::m_bDrawSlashRight);
    lua_tinker::class_mem<PLAYER>(L, "m_bDrawSlashLeft", &PLAYER::m_bDrawSlashLeft);
    lua_tinker::class_mem<PLAYER>(L, "m_bDrawSlashLegRight", &PLAYER::m_bDrawSlashLegRight);
    lua_tinker::class_mem<PLAYER>(L, "m_bDrawSlashLegLeft", &PLAYER::m_bDrawSlashLegLeft);

    lua_tinker::class_mem<PLAYER>(L, "NoCheckContact", &PLAYER::NoCheckContact);
    lua_tinker::class_mem<PLAYER>(L, "IsContact", &PLAYER::IsContact);
    lua_tinker::class_mem<PLAYER>(L, "NoSafePosition", &PLAYER::m_bNoSafePosition);
    lua_tinker::class_mem<PLAYER>(L, "m_bNoCrashCheck", &PLAYER::m_bNoCrashCheck);
    lua_tinker::class_mem<PLAYER>(L, "bIsPushRightKey", &PLAYER::m_bIsPushRightKey);
    lua_tinker::class_mem<PLAYER>(L, "bIsPushLeftKey", &PLAYER::m_bIsPushLeftKey);
    lua_tinker::class_mem<PLAYER>(L, "Slow_Count", &PLAYER::Slow_Count);
    lua_tinker::class_mem<PLAYER>(L, "bChargeMonoVolt", &PLAYER::m_bChargeMonoVolt);
    lua_tinker::class_mem<PLAYER>(L, "bIsCountAttackShield", &PLAYER::m_bIsCountAttackShield);
    lua_tinker::class_mem<PLAYER>(L, "m_iBranchSkill", &PLAYER::m_iBranchSkill);
    lua_tinker::class_mem<PLAYER>(L, "m_fBurnningElapsedTime", &PLAYER::m_fBurnningElapsedTime);
    lua_tinker::class_mem<PLAYER>(L, "m_bIsBurnningMode", &PLAYER::m_bIsBurnningMode);
    lua_tinker::class_mem<PLAYER>(L, "m_bSuperArmor", &PLAYER::m_bSuperArmor);
    lua_tinker::class_mem<PLAYER>(L, "m_bHyperArmor", &PLAYER::m_bHyperArmor);
    lua_tinker::class_mem<PLAYER>(L, "Body_Angle", &PLAYER::Body_Angle);
    lua_tinker::class_mem<PLAYER>(L, "m_bUnstop", &PLAYER::m_bUnstoppableMotion);
    lua_tinker::class_mem<PLAYER>(L, "m_bIsCatchableMotion", &PLAYER::m_bIsCatchableMotion);
    lua_tinker::class_mem<PLAYER>(L, "m_bShowEffect", &PLAYER::m_bShowEffect);
    lua_tinker::class_mem<PLAYER>(L, "m_bCheckGround", &PLAYER::m_bCheckGround);
    lua_tinker::class_mem<PLAYER>(L, "m_bFrameLock", &PLAYER::m_bFrameLock);
    lua_tinker::class_mem<PLAYER>(L, "m_bSpecialSkill", &PLAYER::m_bSpecialSkill);
    lua_tinker::class_mem<PLAYER>(L, "Next_Motion", &PLAYER::Next_Motion);
    lua_tinker::class_mem<PLAYER>(L, "m_iPlayerIndex", &PLAYER::m_iPlayerIndex);
    lua_tinker::class_mem<PLAYER>(L, "m_fDamageToMPAttack", &PLAYER::m_fDamageToMPAttack);
    lua_tinker::class_mem<PLAYER>(L, "m_fStaticToMPAttack", &PLAYER::m_fStaticToMPAttack);
    lua_tinker::class_mem<PLAYER>(L, "m_fMPDecreaseRatio", &PLAYER::m_fMPDecreaseRatio);
    lua_tinker::class_mem<PLAYER>(L, "m_bIsAgitBreaking", &PLAYER::m_bIsAgitBreaking);
    lua_tinker::class_mem<PLAYER>(L, "m_bOffAllEffect", &PLAYER::m_bOffAllEffect);
    lua_tinker::class_mem<PLAYER>(L, "m_bNoUseSpecial", &PLAYER::m_bNoUseSpecial);
    lua_tinker::class_mem<PLAYER>(L, "m_iChangeFrame", &PLAYER::m_iChangeFrame);
    lua_tinker::class_mem<PLAYER>(L, "m_iJumpPossibleFrame", &PLAYER::m_iJumpPossibleFrame);
    lua_tinker::class_mem<PLAYER>(L, "m_bIgnoreStopMotion", &PLAYER::m_bIgnoreStopMotion);
    lua_tinker::class_mem<PLAYER>(L, "Before_Motion", &PLAYER::Before_Motion);
    lua_tinker::class_mem<PLAYER>(L, "m_bMotionChanged", &PLAYER::m_bMotionChanged);
    lua_tinker::class_mem<PLAYER>(L, "m_iSelectTextrue", &PLAYER::m_iSelectTextrue);
    lua_tinker::class_mem<PLAYER>(L, "m_bNoRecoveryMpMotion", &PLAYER::m_bNoRecoveryMpMotion);
    lua_tinker::class_mem<PLAYER>(L, "m_bReserveWake", &PLAYER::m_bReserveWake);
    lua_tinker::class_mem<PLAYER>(L, "m_bNoUseSkill", &PLAYER::m_bNoUseSkill);
    lua_tinker::class_mem<PLAYER>(L, "m_bSavingMotion", &PLAYER::m_bSavingMotion);


    lua_tinker::class_def<PLAYER>(L, "GetScreenPos", &PLAYER::GetScreenPos);
    lua_tinker::class_def<PLAYER>(L, "PlaySound", &PLAYER::Sound);
    lua_tinker::class_def<PLAYER>(L, "PlaySoundForCharIndex", &PLAYER::SoundForCharIndex);
    lua_tinker::class_def<PLAYER>(L, "StartAttack", &PLAYER::StartAttack);
    lua_tinker::class_def<PLAYER>(L, "SetAttackInfo", &PLAYER::SetAttackInfo);
    lua_tinker::class_def<PLAYER>(L, "SetAttackProperty", &PLAYER::SetAttackProperty);
    lua_tinker::class_def<PLAYER>(L, "EndAttack", &PLAYER::EndAttack);
    lua_tinker::class_def<PLAYER>(L, "AddDamage", &PLAYER::AddDamage);
    lua_tinker::class_def<PLAYER>(L, "AddDamageAngleAndPos", &PLAYER::AddDamageAngleAndPos);
    lua_tinker::class_def<PLAYER>(L, "AddDamageWithLocate", &PLAYER::AddDamageWithLocate);
    lua_tinker::class_def<PLAYER>(L, "AddDamageOnP2P", &PLAYER::AddDamageOnP2P);
    lua_tinker::class_def<PLAYER>(L, "AddDamageWithStaticLocate", &PLAYER::AddDamageWithStaticLocate);

    lua_tinker::class_def<PLAYER>(L, "CheckDie", &PLAYER::CheckDie);
    lua_tinker::class_def<PLAYER>(L, "Catched", &PLAYER::Catched);
    lua_tinker::class_def<PLAYER>(L, "Direct_Motion_Input", &PLAYER::Direct_Motion_Input);
    lua_tinker::class_def<PLAYER>(L, "EffectDust", &PLAYER::EffectDust);
    lua_tinker::class_def<PLAYER>(L, "OnReverseGravity", &PLAYER::OnReverseGravity);
    lua_tinker::class_def<PLAYER>(L, "Change_HP", &PLAYER::ChangeHP);
    lua_tinker::class_def<PLAYER>(L, "CatchDamage", &PLAYER::SetCatchDamage);
    lua_tinker::class_def<PLAYER>(L, "CheckDie2End", &PLAYER::CheckDie2End);
    lua_tinker::class_def<PLAYER>(L, "Ah_Sound", &PLAYER::Ah_Sound);
    lua_tinker::class_def<PLAYER>(L, "Heal_To_Player", &PLAYER::Heal_To_Player);
    lua_tinker::class_def<PLAYER>(L, "ChangeMotion", &PLAYER::ChangeMotion);
    lua_tinker::class_def<PLAYER>(L, "Change_MP", &PLAYER::Change_MP);
    lua_tinker::class_def<PLAYER>(L, "ChangeAllCharacterMP", &PLAYER::ChangeAllCharacterMP);
    lua_tinker::class_def<PLAYER>(L, "AddItem", &PLAYER::AddItem);
    lua_tinker::class_def<PLAYER>(L, "Jump", &PLAYER::Jump);
    lua_tinker::class_def<PLAYER>(L, "JumpMotion", &PLAYER::JumpMotion);
    lua_tinker::class_def<PLAYER>(L, "Run", &PLAYER::Run);
    lua_tinker::class_def<PLAYER>(L, "StartSpecial", &PLAYER::StartSpecial);
    lua_tinker::class_def<PLAYER>(L, "StartSpecialNoZoom", &PLAYER::StartSpecialNoZoom);
    lua_tinker::class_def<PLAYER>(L, "Stop", &PLAYER::Stop);
    lua_tinker::class_def<PLAYER>(L, "Scream", &PLAYER::Scream);
    lua_tinker::class_def<PLAYER>(L, "GetPos", &PLAYER::GetPosition);
    lua_tinker::class_def<PLAYER>(L, "OnLire2Atk", &PLAYER::OnLire2Atk);
    lua_tinker::class_def<PLAYER>(L, "AddParticle", &PLAYER::AddParticle);
    lua_tinker::class_def<PLAYER>(L, "AddParticlePos", &PLAYER::AddParticlePos);
    lua_tinker::class_def<PLAYER>(L, "AddParticleXOffset", &PLAYER::AddParticleXOffset);
    lua_tinker::class_def<PLAYER>(L, "AddParticleXOffsetWithTrace", &PLAYER::AddParticleXOffsetWithTrace);
    lua_tinker::class_def<PLAYER>(L, "AddParticleWithAngle", &PLAYER::AddParticleWithAngle);
    lua_tinker::class_def<PLAYER>(L, "AddParticleWithBlackhole", &PLAYER::AddParticleWithBlackhole);
    lua_tinker::class_def<PLAYER>(L, "AddParticleWithTrace", &PLAYER::AddParticleWithTrace);
    lua_tinker::class_def<PLAYER>(L, "AddParticlePosWithTrace", &PLAYER::AddParticlePosWithTrace);
    lua_tinker::class_def<PLAYER>(L, "AddParticleXOffsetNoMotion", &PLAYER::AddParticleXOffsetNoMotion);
    lua_tinker::class_def<PLAYER>(L, "IsLocalPlayer", &PLAYER::IsLocalPlayer);
    lua_tinker::class_def<PLAYER>(L, "AddParticleNoDirection", &PLAYER::AddParticleNoDirection);
    lua_tinker::class_def<PLAYER>(L, "AddParticleNoDirectionPos", &PLAYER::AddParticleNoDirectionPos);
    lua_tinker::class_def<PLAYER>(L, "AddParticleNoDirectionWithTrace", &PLAYER::AddParticleNoDirectionWithTrace);
    lua_tinker::class_def<PLAYER>(L, "AddParticleNoDirectionPosWithTrace", &PLAYER::AddParticleNoDirectionPosWithTrace);
    lua_tinker::class_def<PLAYER>(L, "SetShield", &PLAYER::SetShield);
    lua_tinker::class_def<PLAYER>(L, "Metamorphosis", &PLAYER::ProcessMetamorphosisFormChange);
    lua_tinker::class_def<PLAYER>(L, "CheckEquipItem", &PLAYER::CheckEquipItem);
    lua_tinker::class_def<PLAYER>(L, "invincibility", &PLAYER::SetSuper);
    lua_tinker::class_def<PLAYER>(L, "IsOldMotion", &PLAYER::IsOldMotion);
    lua_tinker::class_def<PLAYER>(L, "AddBlurAlpha", &PLAYER::AddBlurAlpha);
    lua_tinker::class_def<PLAYER>(L, "EndBlur", &PLAYER::EndBlur);
    lua_tinker::class_def<PLAYER>(L, "SetIsRight", &PLAYER::SetIsRight);
    lua_tinker::class_def<PLAYER>(L, "GetIsRight", &PLAYER::GetIsRight);
    lua_tinker::class_def<PLAYER>(L, "ShowLas4GreatSword", &PLAYER::ShowLas4GreatSword);
    lua_tinker::class_def<PLAYER>(L, "SetWhiteCount", &PLAYER::SetWhiteCount);
    lua_tinker::class_def<PLAYER>(L, "GetElemental", &PLAYER::GetElemental);
    lua_tinker::class_def<PLAYER>(L, "AddDamageNoLatency", &PLAYER::AddDamageNoLatency);
    lua_tinker::class_def<PLAYER>(L, "SwapWeapon", &PLAYER::ProcessSwapWeapon);
    lua_tinker::class_def<PLAYER>(L, "ChangeForceNum", &PLAYER::ChangeForceNum);
    lua_tinker::class_def<PLAYER>(L, "InitForceNum", &PLAYER::InitForceNum);
    lua_tinker::class_def<PLAYER>(L, "GetForceNum", &PLAYER::GetForceNum);
    lua_tinker::class_def<PLAYER>(L, "ConfirmUseSheild", &PLAYER::ConfirmUseSheild);
    lua_tinker::class_def<PLAYER>(L, "IsSpecialMotion", &PLAYER::IsSpecialSkillMotion);

    lua_tinker::class_def<PLAYER>(L, "IsFatal", &PLAYER::IsFatal);
    lua_tinker::class_def<PLAYER>(L, "AddDamageWithLocateAbsoultPos", &PLAYER::AddDamageWithLocateAbsolutePos);
    lua_tinker::class_def<PLAYER>(L, "SetMagicEffect", &PLAYER::SetMagicEffect);
    lua_tinker::class_def<PLAYER>(L, "ClearMagicEffect", &PLAYER::ClearMagicEffect);
    lua_tinker::class_def<PLAYER>(L, "ClearMagicEffectByLocal", &PLAYER::ClearMagicEffectByLocal);
    lua_tinker::class_def<PLAYER>(L, "ClearAllPlayerMagicEffect", &PLAYER::ClearAllPlayerMagicEffect);
    lua_tinker::class_def<PLAYER>(L, "SetMagicEffect_NotMeBroad", &PLAYER::SetMagicEffect_NotMeBroad);
    lua_tinker::class_def<PLAYER>(L, "ClearMagicEffect_NotMeBroad", &PLAYER::ClearMagicEffect_NotMeBroad);

    lua_tinker::class_def<PLAYER>(L, "CheckSkill", &PLAYER::CheckSkill);
    lua_tinker::class_def<PLAYER>(L, "CheckSkillNoLocal", &PLAYER::IsEquipSkill);
    lua_tinker::class_def<PLAYER>(L, "BeginAnim", &PLAYER::BeginAnim);
    lua_tinker::class_def<PLAYER>(L, "EndAnim", &PLAYER::EndAnim);
    lua_tinker::class_def<PLAYER>(L, "SetChargeMonoVolt", &PLAYER::SetChargeMonoVolt);
    lua_tinker::class_def<PLAYER>(L, "GetMyCharString", &PLAYER::GetMyCharString);
    lua_tinker::class_def<PLAYER>(L, "IsLive", &PLAYER::IsLive);
    lua_tinker::class_def<PLAYER>(L, "GetCharType", &PLAYER::GetCharType);
    lua_tinker::class_def<PLAYER>(L, "InitMonoVolt", &PLAYER::InitMonoVolt);
    lua_tinker::class_def<PLAYER>(L, "InitAbyssHandEffectToScript", &PLAYER::InitAbyssHandEffectToScript);
    lua_tinker::class_def<PLAYER>(L, "ChangeHpToMp", &PLAYER::ChangeHpToMp);
    lua_tinker::class_def<PLAYER>(L, "DecreaseMP", &PLAYER::DecreaseMP);
    lua_tinker::class_def<PLAYER>(L, "Change_Motion", &PLAYER::Change_Motion);
    lua_tinker::class_def<PLAYER>(L, "DeleteMagicShield", &PLAYER::DeleteMagicShield);
    lua_tinker::class_def<PLAYER>(L, "SetChargedShield", &PLAYER::SetChargedShield);
    lua_tinker::class_def<PLAYER>(L, "GetPlayerMaxHP", &PLAYER::GetPlayerMaxHP);
    lua_tinker::class_def<PLAYER>(L, "CountBP", &PLAYER::CountBP);
    lua_tinker::class_def<PLAYER>(L, "IsMagicEffect", &PLAYER::IsMagicEffect);

    lua_tinker::class_def<PLAYER>(L, "GetFrameNum", &PLAYER::GetFrameNum);
    lua_tinker::class_def<PLAYER>(L, "GetFrame", &PLAYER::GetFrameIndex);
    lua_tinker::class_def<PLAYER>(L, "SetFrame", &PLAYER::SetFrameIndex);
    lua_tinker::class_def<PLAYER>(L, "SetMotion", &PLAYER::SetPlayerState);
    lua_tinker::class_def<PLAYER>(L, "StartAfterImage", &PLAYER::StartAfterImage);
    lua_tinker::class_def<PLAYER>(L, "EndAfterImage", &PLAYER::EndAfterImage);
    lua_tinker::class_def<PLAYER>(L, "SetBurnningPoint", &PLAYER::SetBurnningPoint);
    lua_tinker::class_def<PLAYER>(L, "GetBurnningPoint", &PLAYER::GetBurnningPoint);
    lua_tinker::class_def<PLAYER>(L, "GetOldMotion", &PLAYER::GetOldMotion);
    lua_tinker::class_def<PLAYER>(L, "ChangeBurnningTemplate", &PLAYER::ChangeBurnningTemplate);
    lua_tinker::class_def<PLAYER>(L, "SetUISuperPoint", &PLAYER::SetUISuperPoint);
    lua_tinker::class_def<PLAYER>(L, "UseSkillNotCheck", &PLAYER::UseSkillNotCheck);
    lua_tinker::class_def<PLAYER>(L, "ClearDeSableSkill", &PLAYER::ClearDeSableSkill);
    lua_tinker::class_def<PLAYER>(L, "RegDeSableSkill", &PLAYER::RegDeSableSkill);

    lua_tinker::class_def<PLAYER>(L, "SetMeshToggleState", &PLAYER::SetMeshToggleState);
    lua_tinker::class_def<PLAYER>(L, "GetMeshToggleState", &PLAYER::GetMeshToggleState);

    lua_tinker::class_def<PLAYER>(L, "IsToggleExtraMesh", &PLAYER::IsToggleExtraMesh);
    lua_tinker::class_def<PLAYER>(L, "ToggleExtraMesh", &PLAYER::ToggleExtraMesh);
    lua_tinker::class_def<PLAYER>(L, "ToggleExtraMeshAll", &PLAYER::ToggleExtraMeshAll);
    lua_tinker::class_def<PLAYER>(L, "ToggleBaseMesh", &PLAYER::ToggleBaseMesh);
    lua_tinker::class_def<PLAYER>(L, "ToggleBaseMeshStatic", &PLAYER::ToggleBaseMeshStatic);
    lua_tinker::class_def<PLAYER>(L, "IsRenderBaseMeshStatic", &PLAYER::IsRenderBaseMeshStatic);
    lua_tinker::class_def<PLAYER>(L, "ToggleSecondBaseMesh", &PLAYER::ToggleSecondBaseMesh);
    lua_tinker::class_def<PLAYER>(L, "ToggleSecondBaseMeshAll", &PLAYER::ToggleSecondBaseMeshAll);
    lua_tinker::class_def<PLAYER>(L, "ToggleMeshAll", &PLAYER::ToggleMeshAll);
    lua_tinker::class_def<PLAYER>(L, "RenderOffExtraAllWeapon", &PLAYER::RenderOffExtraAllWeapon);

    lua_tinker::class_def<PLAYER>(L, "SetChildMeshMotion", &PLAYER::SetChildMeshMotion);
    lua_tinker::class_def<PLAYER>(L, "SetChildMeshMotion_Front", &PLAYER::SetChildMeshMotion_Front);
    lua_tinker::class_def<PLAYER>(L, "SetChildMeshFrame", &PLAYER::SetChildMeshFrame);
    lua_tinker::class_def<PLAYER>(L, "SetChildMeshCurrentFrame", &PLAYER::SetChildMeshCurrentFrame);
    lua_tinker::class_def<PLAYER>(L, "GetChildMeshCurrentFrame", &PLAYER::GetChildMeshCurrentFrame);
    lua_tinker::class_def<PLAYER>(L, "ClearChildMeshOneTimeMotion", &PLAYER::ClearChildMeshOneTimeMotion);

    lua_tinker::class_def<PLAYER>(L, "CountHittingPoint", &PLAYER::CountHittingPoint);
    lua_tinker::class_def<PLAYER>(L, "GetHittingState", &PLAYER::GetHittingState);
    lua_tinker::class_def<PLAYER>(L, "SetHGDecreaseState", &PLAYER::SetHGDecreaseState);
    lua_tinker::class_def<PLAYER>(L, "GetHGDecreaseState", &PLAYER::GetHGDecreaseState);
    lua_tinker::class_def<PLAYER>(L, "GetHGPCumulativeNum", &PLAYER::GetHGPCumulativeNum);
    lua_tinker::class_def<PLAYER>(L, "SetMariSummonState", &PLAYER::SetMariSummonState);
    lua_tinker::class_def<PLAYER>(L, "GetMariSummonState", &PLAYER::GetMariSummonState);
    lua_tinker::class_def<PLAYER>(L, "IsDie", &PLAYER::IsDie);
    lua_tinker::class_def<PLAYER>(L, "SendBurnningPoint", &PLAYER::SendBurnningPoint);
    lua_tinker::class_def<PLAYER>(L, "GetMP", &PLAYER::GetMP);
    lua_tinker::class_def<PLAYER>(L, "GetHP", &PLAYER::GetHP);
    lua_tinker::class_def<PLAYER>(L, "GetCurrentMotion", &PLAYER::GetCurrentMotion);
    lua_tinker::class_def<PLAYER>(L, "SetPlayerHP", &PLAYER::SetPlayerHP);
    lua_tinker::class_def<PLAYER>(L, "OnEarthQuake", &PLAYER::OnEarthQuake);
    lua_tinker::class_def<PLAYER>(L, "IsLiveCharacter", &PLAYER::IsLiveCharacter);
    lua_tinker::class_def<PLAYER>(L, "SetSuperArmorFrame", &PLAYER::SetSuperArmorFrame);

    lua_tinker::class_def<PLAYER>(L, "SummonMonster", &PLAYER::SummonMonster);
    lua_tinker::class_def<PLAYER>(L, "SummonMonsterUseBuff", &PLAYER::SummonMonsterUseBuff);
    lua_tinker::class_def<PLAYER>(L, "SetSummonMonsterHP", &PLAYER::SetSummonMonsterHP);
    lua_tinker::class_def<PLAYER>(L, "CheckStandBlock", &PLAYER::CheckStandBlock);
    lua_tinker::class_def<PLAYER>(L, "AppointUseSkill", &PLAYER::AppointUseSkill);

    lua_tinker::class_def<PLAYER>(L, "SetTexture", &PLAYER::SetTexture);
    lua_tinker::class_def<PLAYER>(L, "ChangeSlashColor", &PLAYER::ChangeSlashColor);
    lua_tinker::class_def<PLAYER>(L, "GetTargetPos", &PLAYER::GetTargetPos);
    lua_tinker::class_def<PLAYER>(L, "AddDamageTarget", &PLAYER::AddDamageTarget);
    lua_tinker::class_def<PLAYER>(L, "GetPosFromNearBlock", &PLAYER::GetPosFromNearBlock);
    lua_tinker::class_def<PLAYER>(L, "GetDamageLife", &PLAYER::GetDamageLife);
    lua_tinker::class_def<PLAYER>(L, "IsDamageLife", &PLAYER::IsDamageLife);
    lua_tinker::class_def<PLAYER>(L, "SendStateToMonster", &PLAYER::SendStateToMonster);
    lua_tinker::class_def<PLAYER>(L, "GetStateToMonster", &PLAYER::GetStateToMonster);
    lua_tinker::class_def<PLAYER>(L, "GetNowCharPromotion", &PLAYER::GetNowCharPromotion);
    lua_tinker::class_def<PLAYER>(L, "SetCameraZoom", &PLAYER::SetCameraZoom);
    lua_tinker::class_def<PLAYER>(L, "IsMonsterGameMode", &PLAYER::IsMonsterGameMode);
    lua_tinker::class_def<PLAYER>(L, "IsFatalGameMode", &PLAYER::IsFatalGameMode);

    lua_tinker::class_def<PLAYER>(L, "SetDirectMove", &PLAYER::SetDirectMove);
    lua_tinker::class_def<PLAYER>(L, "CheckDirectMove", &PLAYER::CheckDirectMove);

    lua_tinker::class_def<PLAYER>(L, "GetWaitMotion", &PLAYER::GetWaitMotion);
    lua_tinker::class_def<PLAYER>(L, "SetWaitMotion", &PLAYER::SetWaitMotion);
    lua_tinker::class_def<PLAYER>(L, "GetWalkMotion", &PLAYER::GetWalkMotion);
    lua_tinker::class_def<PLAYER>(L, "SetWalkMotion", &PLAYER::SetWalkMotion);
    lua_tinker::class_def<PLAYER>(L, "SendAgitWateringFairyTree", &PLAYER::SendAgitWateringFairyTree);
    lua_tinker::class_def<PLAYER>(L, "SetShowBaseEffect", &PLAYER::SetShowBaseEffect);
    lua_tinker::class_def<PLAYER>(L, "SetRecoveryHpByRadio", &PLAYER::SetRecoveryHpByRadio);

    lua_tinker::class_def<PLAYER>(L, "SetWeaponRenderState", &PLAYER::SetWeaponRenderState);
    lua_tinker::class_def<PLAYER>(L, "SetMergeWeaponRenderState", &PLAYER::SetMergeWeaponRenderState);
    lua_tinker::class_def<PLAYER>(L, "GetWeaponRenderState", &PLAYER::GetWeaponRenderState);
    lua_tinker::class_def<PLAYER>(L, "GetMergeWeaponRenderState", &PLAYER::GetMergeWeaponRenderState);
    lua_tinker::class_def<PLAYER>(L, "SetWindSpeed", &PLAYER::SetWindSpeed);

    lua_tinker::class_def<PLAYER>(L, "ChargeQuantityPoint", &PLAYER::ChargeQuantityPoint);
    lua_tinker::class_def<PLAYER>(L, "CountQuantityPoint", &PLAYER::CountQuantityPoint);
    lua_tinker::class_def<PLAYER>(L, "GetQuantityPoint", &PLAYER::GetQuantityPoint);
    lua_tinker::class_def<PLAYER>(L, "GetMaxQuantityPoint", &PLAYER::GetMaxQuantityPoint);
    lua_tinker::class_def<PLAYER>(L, "CheckValidPuzzleKey", &PLAYER::CheckValidPuzzleKey);
    lua_tinker::class_def<PLAYER>(L, "CreateQuestionPuzzleKey", &PLAYER::CreateQuestionPuzzleKey);
    lua_tinker::class_def<PLAYER>(L, "SetFadeInOut", &PLAYER::SetFadeInOut);
    lua_tinker::class_def<PLAYER>(L, "SetOPChargeWeight", &PLAYER::SetOPChargeWeight);
    lua_tinker::class_def<PLAYER>(L, "ClearPuzzleyKeyInfo", &PLAYER::ClearPuzzleyKeyInfo);
    lua_tinker::class_def<PLAYER>(L, "IsShowDialogue", &PLAYER::IsShowDialogue);
    lua_tinker::class_def<PLAYER>(L, "SetSpecialBuff", &PLAYER::SetSpecialBuff);
    lua_tinker::class_def<PLAYER>(L, "ClearSpecialBuff", &PLAYER::ClearSpecialBuff);
    lua_tinker::class_def<PLAYER>(L, "SetRangeCheckSize", &PLAYER::SetRangeCheckSize);
    lua_tinker::class_def<PLAYER>(L, "SetRangeBuffEffect", &PLAYER::SetRangeBuffEffect);
    lua_tinker::class_def<PLAYER>(L, "RunCombinationSkill", &PLAYER::RunCombinationSkill);
    lua_tinker::class_def<PLAYER>(L, "CreateReflectionDamage", &PLAYER::CreateReflectionDamage);
    lua_tinker::class_def<PLAYER>(L, "SetAllObjectBuff", &PLAYER::SetAllObjectBuff);
    lua_tinker::class_def<PLAYER>(L, "SetRenderWeather", &PLAYER::SetRenderWeather);
    lua_tinker::class_def<PLAYER>(L, "SetRenderFaceLift", &PLAYER::SetRenderFaceLift);

    lua_tinker::class_def<PLAYER>(L, "SetWaitMotion", &PLAYER::SetWaitMotion);
    lua_tinker::class_def<PLAYER>(L, "IsWaitMotion", &PLAYER::IsWaitMotion);
    lua_tinker::class_def<PLAYER>(L, "IsFatalWaitMotion", &PLAYER::IsFatalWaitMotion);
    lua_tinker::class_def<PLAYER>(L, "IsWalkMotion", &PLAYER::IsWalkMotion);
    lua_tinker::class_def<PLAYER>(L, "IsRunMotion", &PLAYER::IsRunMotion);
    lua_tinker::class_def<PLAYER>(L, "IsDashMotion", &PLAYER::IsDashMotion);
    lua_tinker::class_def<PLAYER>(L, "IsWinMotion", &PLAYER::IsWinMotion);
    lua_tinker::class_def<PLAYER>(L, "IsLoseMotion", &PLAYER::IsLoseMotion);
    lua_tinker::class_def<PLAYER>(L, "IsStartMotion", &PLAYER::IsStartMotion);
    lua_tinker::class_def<PLAYER>(L, "IsJokeMotion", &PLAYER::IsJokeMotion);
    lua_tinker::class_def<PLAYER>(L, "IsLearnedSkill", &PLAYER::IsLearnedSkill);
    lua_tinker::class_def<PLAYER>(L, "IsLearnedSkillNoLocal", &PLAYER::IsTrainedSkill);
    lua_tinker::class_def<PLAYER>(L, "SwapSlotSkillType", &PLAYER::SwapSlotSkillType);
    lua_tinker::class_def<PLAYER>(L, "SetNotCheckSideFH", &PLAYER::SetNotCheckSideFH);
    lua_tinker::class_def<PLAYER>(L, "GetNotCheckSideFH", &PLAYER::GetNotCheckSideFH);
    lua_tinker::class_def<PLAYER>(L, "GetSafePosition", &PLAYER::GetSafePosition);
    lua_tinker::class_def<PLAYER>(L, "SetQPChangeFrame", &PLAYER::SetQPChangeFrame);
    lua_tinker::class_def<PLAYER>(L, "SetRageModeHpPercent", &PLAYER::SetRageModeHpPercent);
    lua_tinker::class_def<PLAYER>(L, "SetManaShieldRatio", &PLAYER::SetManaShieldRatio);
    lua_tinker::class_def<PLAYER>(L, "MoveToNearTargetPos", &PLAYER::MoveToNearTargetPos);
    lua_tinker::class_def<PLAYER>(L, "GetPlayerName", &PLAYER::GetPlayerName);
    lua_tinker::class_def<PLAYER>(L, "SetCharOutLineColor", &PLAYER::SetCharOutLineColor);
    lua_tinker::class_def<PLAYER>(L, "ProcessMetamorphosisNormalForm", &PLAYER::ProcessMetamorphosisNormalForm);
    lua_tinker::class_def<PLAYER>(L, "SetMagicEffectOnlyLocal", &PLAYER::SetMagicEffectOnlyLocal);
    lua_tinker::class_def<PLAYER>(L, "SetCurMarbleNum", &PLAYER::SetCurMarbleNum);
    lua_tinker::class_def<PLAYER>(L, "ChangeSkillCoolTime", &PLAYER::ChangeSkillCoolTime);
    lua_tinker::class_def<PLAYER>(L, "SetDamageEff", &PLAYER::SetDamageEff);
    lua_tinker::class_def<PLAYER>(L, "SetMagicEffectRandom", &PLAYER::SetMagicEffectRandom);
    lua_tinker::class_def<PLAYER>(L, "SetOverlayTimer", &PLAYER::SetOverlayTimer);
    lua_tinker::class_def<PLAYER>(L, "SetInGameMessage", &PLAYER::SetInGameMessage);

    lua_tinker::class_def<PLAYER>(L, "GetMyEmbarkInstance", &PLAYER::GetMyEmbarkInstance);
    lua_tinker::class_def<PLAYER>(L, "IsEmbarkationKey", &PLAYER::IsEmbarkationKey);
    lua_tinker::class_def<PLAYER>(L, "SetEmbarkAdaptBoneID", &PLAYER::SetEmbarkAdaptBoneID);
    lua_tinker::class_def<PLAYER>(L, "GetEmbarkAdaptBoneID", &PLAYER::GetEmbarkAdaptBoneID);
    lua_tinker::class_def<PLAYER>(L, "SetEmbarkUID", &PLAYER::SetEmbarkUID);
    lua_tinker::class_def<PLAYER>(L, "GetEmbarkUID", &PLAYER::GetEmbarkUID);

    lua_tinker::class_def<PLAYER>(L, "GetPlayerUID", &PLAYER::GetPlayerUID);

    lua_tinker::class_def<PLAYER>(L, "AddChildLiveModel", &PLAYER::AddChildLiveModel);
    lua_tinker::class_def<PLAYER>(L, "RemoveChildLiveModel", &PLAYER::RemoveChildLiveModel);
    lua_tinker::class_def<PLAYER>(L, "IsEndGamePlay", &PLAYER::IsEndGamePlay);
    lua_tinker::class_def<PLAYER>(L, "GetEmbarkSendCool", &PLAYER::GetEmbarkSendCool);
    lua_tinker::class_def<PLAYER>(L, "DirectKillPlayer", &PLAYER::DirectKillPlayer);
    lua_tinker::class_def<PLAYER>(L, "UseGameItem_Super", &PLAYER::UseGameItem_Super);
    lua_tinker::class_def<PLAYER>(L, "SetUnConditionToSiegFury", &PLAYER::SetUnConditionToSiegFury);
    lua_tinker::class_def<PLAYER>(L, "GetPrevDamageHitWho", &PLAYER::GetPrevDamageHitWho);
    lua_tinker::class_def<PLAYER>(L, "SetEnemyAtkGuardMotion", &PLAYER::SetEnemyAtkGuardMotion);
    lua_tinker::class_def<PLAYER>(L, "StartText", &PLAYER::StartText);
    lua_tinker::class_def<PLAYER>(L, "SetEmbarkArena", &PLAYER::SetEmbarkArena);
    lua_tinker::class_def<PLAYER>(L, "IsSetEmbarkArena", &PLAYER::IsSetEmbarkArena);
    lua_tinker::class_def<PLAYER>(L, "Take_Item", &PLAYER::Take_Item);
    lua_tinker::class_def<PLAYER>(L, "UseGameItem", &PLAYER::UseGameItem);
    lua_tinker::class_def<PLAYER>(L, "GetNumGameItem", &PLAYER::GetNumGameItem);
    lua_tinker::class_def<PLAYER>(L, "AddLife", &PLAYER::AddLife);
    lua_tinker::class_def<PLAYER>(L, "SetLock", &PLAYER::SetLock);
    lua_tinker::class_def<PLAYER>(L, "IsInLength", &PLAYER::IsInLength);
    lua_tinker::class_def<PLAYER>(L, "AddParticleToChildMeshBone", &PLAYER::AddParticleToChildMeshBone);
    lua_tinker::class_def<PLAYER>(L, "RenderChildMeshAllParticle", &PLAYER::RenderChildMeshAllParticle);
    lua_tinker::class_def<PLAYER>(L, "DeleteDamage", &PLAYER::DeleteDamage);
    lua_tinker::class_def<PLAYER>(L, "InitDangerSque", &PLAYER::InitDangerSque);
    lua_tinker::class_def<PLAYER>(L, "RenderChildMeshParticle", &PLAYER::RenderChildMeshParticle);
    lua_tinker::class_def<PLAYER>(L, "IsRenderChildMeshParticle", &PLAYER::IsRenderChildMeshParticle);
    lua_tinker::class_def<PLAYER>(L, "IsLiveChildMeshParticle", &PLAYER::IsLiveChildMeshParticle);
    lua_tinker::class_def<PLAYER>(L, "GetChildMeshCurrentMotionName", &PLAYER::GetChildMeshCurrentMotionName);
    lua_tinker::class_def<PLAYER>(L, "CheckTransformPetSkill", &PLAYER::CheckTransformPetSkill);

    lua_tinker::class_mem<PLAYER>(L, "m_vDamageStartPos", &PLAYER::m_vDamageStartPos);
    lua_tinker::class_mem<PLAYER>(L, "m_vDamageWhoPos", &PLAYER::m_vDamageWhoPos);

    lua_tinker::class_def<PLAYER>(L, "CheckEquipItem", &PLAYER::CheckEquipItem);
    lua_tinker::class_def<PLAYER>(L, "SetSavingGaugeAmount", &PLAYER::SetSavingGaugeAmount);
    lua_tinker::class_def<PLAYER>(L, "CheckSetEquipItem", &PLAYER::CheckSetEquipItem);

    lua_tinker::class_def<PLAYER>(L, "SetCartoon", &PLAYER::SetCartoon);

    //BloodSystem GC_CHAR_UNO
    lua_tinker::class_mem<PLAYER>(L, "m_bOnceChargeBloodGuage", &PLAYER::m_bOnceChargeBloodGuage);
    lua_tinker::class_def<PLAYER>(L, "CalcBloodPoint", &PLAYER::CalcBloodPoint);
    lua_tinker::class_def<PLAYER>(L, "GetBloodPoint", &PLAYER::GetBloodPoint);

    // 2ª 4 Skills
    lua_tinker::class_def<PLAYER>(L, "IsTutorialMode", &PLAYER::IsTutorialMode);
    lua_tinker::class_def<PLAYER>(L, "CheckEquipSkill", &PLAYER::IsEquipSkill);
    lua_tinker::class_def<PLAYER>(L, "DamageToSameTeam", &PLAYER::DamageToSameTeam);

#ifdef DECANEE
    lua_tinker::class_def<PLAYER>(L, "StartDecaneeBar", &PLAYER::CheckRunMarbleMode);
    lua_tinker::class_def<PLAYER>(L, "GetDecaneeMarble", &PLAYER::GetCurMarbleNum);
    lua_tinker::class_def<PLAYER>(L, "IsDecaneeBarRunning", &PLAYER::GetIsMarbleRun);
    lua_tinker::class_def<PLAYER>(L, "GetMaxMarbleNum", &PLAYER::GetMaxMarbleNum);
#endif

    lua_tinker::class_def<PLAYER>(L, "SetSuper", &PLAYER::SetSuper_);
    if (GCFUNC::LoadLuaScript(KGCLuabinder::getInstance(), "ActionScript.stg") == false)
    {
        MessageBoxA(NULL, "ActionScript.stg Load Failed!", NULL, MB_OK);
        exit(0);
    }

    {
        KLuaManager luaMgr;
        if (GCFUNC::LoadLuaScript(luaMgr, "ActionFiles.stg") == false)
        {
            MessageBoxA(NULL, "ActionFiles.stg Load Failed!", NULL, MB_OK);
            exit(0);
        }
        else
        {
            if (SUCCEEDED(luaMgr.BeginTable("ActionFiles")))
            {
                std::string strFileName;
                for (int i = 1; SUCCEEDED(luaMgr.GetValue(i, strFileName)); i++)
                {
                    SiKGCFileMoniter()->RegisterFile(g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath(strFileName), InitPlayerFileMoniter);

                    if (GCFUNC::LoadLuaScript(KGCLuabinder::getInstance(), strFileName) == false)
                    {
                        std::string str = KGCLuabinder::getInstance().GetLastError();
                        return;
                    }
                }
                luaMgr.EndTable();
            }
        }
    }
}


PLAYER::PLAYER(void)
{
    m_bIsEquipCoordiSetItem = false;
    m_iContactX = 0;
    m_iContactY = 0;

    m_kUserInfo.Init();
    m_strNation = L"";
    for (int i = 0; i < FORM_NUM; ++i)
        m_FormTemplate[i].Init();
    m_bRenderLire3Arrow = false;
    m_pLire3Arrow = NULL;
    m_pNonChangeFaceLift = NULL;
    m_bLoadDownHelmetNFL = false;


    Body_Angle = 0;
    m_pTexBerserk1 = NULL;
    m_pTexBerserk2 = NULL;
    m_pTexFatal = NULL;

    m_pTexSlash = NULL;
    //    V_Slash                     = NULL;
    //    V_Slash_Left                = NULL;
    m_pPlayerSp = NULL;
    m_pSerdinSign = NULL;
    m_pCanabanSign = NULL;

    m_pRonan3Force = NULL;
    m_pRonan3Force1 = NULL;
    m_pRonan3Force2 = NULL;
    m_pRuneShield = NULL;
    m_pRuneShield1 = NULL;
    m_pLasFireWingLeft = NULL;
    m_pLasFireWingRight = NULL;
    m_pLasMotalBlowReady = NULL;
    m_pLasMotalBlowReadyAnim = NULL;
    m_pDashEff = NULL;

    m_pAmy4Action1 = NULL;
    m_pAmy4Action2 = NULL;
    m_pAmy4Action3 = NULL;
    for (int i = 0; i < 4; i++)
        m_pHoldEffect[i] = NULL;

    m_pBurnningCharge[0] = NULL; //진 버닝차지일때 이펙트
    m_pBurnningCharge[1] = NULL;
    m_pHideParticle = NULL;
    m_Elemental = 0;
    m_StrongLevel = 0;
    m_fGetExpGpRatio = 0.0f;
    m_iJumpActionLimitCount = 0;
    m_iSummoning = 0;
    m_bJumpZoomed = false;
    m_bJumpDashed = false;

    m_iStoneCurseType = -1;
    Silk = 0;
    MetamorphosisForm = FORM_NORMAL;
    //m_pFormShadow = NULL;
    Next_Motion = -1;
    Next_Direction = -1;
    //m_vecFormItemModel.clear();

    Before_Motion = -1;
    m_bKamikiDefence = false;
    m_bPumpkinDefence = false;
    m_bKazeazeDefence = false;
    m_iJumpPossibleFrame = 0;
    m_iInvEffFrame = 0;
    m_iFlashEffectFrame = 0;
    m_iShakeEffectFrame = 0;
    m_bRenderQuestDrama = true;
    m_iDownCount = 0;
    m_dwDownBugTime = -1;

    m_vecTagCharNum.clear();
    m_vecTagCharNum.reserve(2);
    m_mapTagCharInfo.clear();

    vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    Target_Score = 0;
    m_mapEndExp.clear();

    Las4GreatSword.clear();
    m_bDrawSlashLeft = false;
    m_bDrawSlashRight = false;
    m_bDrawSlashLegRight = false;
    m_bDrawSlashLegLeft = false;
    m_pObject = NULL;

    for (int i = 0; i < FORM_NUM; ++i)
        m_pObjMetaForm[i] = NULL;

    m_eAttackType = ATTACKTYPE_NORMAL;
    m_eAttackDirection = ATTACKDIR_MIDDLE;
    m_fPushTargetX = 0.0f;
    m_fPushTargetY = 0.0f;
    m_bSpecialAttack = false;

    m_bSuperMode = 0;


    m_fHP_RecoverVal = 0.0;
    m_fMP_RecoverVal = 0.0;

#if !defined( __PATH__ )
    m_iTutorialPlayerAction = -1;
#endif

    m_iFreezeCount = 0;
    m_fFreezeDamage = 0.0f;
    m_iPlusFreezeCount = 10;

    m_dwBackAttackTime = 0;
    m_dwAerialAttackTime = 0;
    m_dwMissAttackTime = 0;

    g_kMonsterManager.CreateMonsterBoolState(m_mapS1AttackState);
    g_kMonsterManager.CreateMonsterBoolState(m_mapS2AttackState);
    g_kMonsterManager.CreateMonsterBoolState(m_mapS3AttackState);

    m_iCannotCatchFrame = 0;
    m_bDamageSetNoCheckContact = false;
    m_bNoCrashCheck = false;
    m_pEmoticon = NULL;

    m_fAutoHpRegenLimit = 0.0f;
    m_nForceNum = 0;
    m_cUseShield = CID_MAX;

    m_iArenaCoolTime = 0;

    m_bIsPushRightKey = false;
    m_bIsPushLeftKey = false;

    m_mapPlayAnimation.clear();

    m_bChargeMonoVolt = false;
    m_iAbyssHandEff = 0;

    m_bIsContinousDamage = false;
    m_fContinousDamage = 0.0f;

    m_ePhysicAtkType = (ePhysicAtkType)0;
    m_bIsChargedShield = false;

    m_fPhysicAtkPower = 0.0f;
    m_iPrevShield = 0;
    //m_iAttackPlayerIndex = -1;

    m_iShieldTime = 0;
    m_iShieldLimitTime = 0;
    m_fEffectDamage = 0.0f;

    m_bIsCountAttackShield = false;
    m_fRegenRate = 0.0f;

    m_iShockSturnCount = 0;
    m_iBranchSkill = -1;
    m_bBurnningSpecial = false;

    m_bIsBurnningMode = false;

#ifdef DECANEE
    m_iDecaneeChargeSpeed = 30;
    m_iDecaneeConsumeSpeed = 15;
#endif

    m_bUnstoppableMotion = false;
    m_bIsCatchableMotion = false;
    m_bIgnoreStopMotion = false;
    m_bOffAllEffect = false;
    m_bCheckGround = false;
    m_bNoUseSpecial = false;
    for (int i = 0; i < (int)m_pMonoVolt.size(); ++i)
    {
        m_pMonoVolt[i] = NULL;
    }
    for (int i = 0; i < (int)m_pAbyssHandEff.size(); ++i)
    {
        m_pAbyssHandEff[i] = NULL;
    }

    m_fEff_x = 0.0f;
    m_fEff_y = 0.0f;

    m_strGuildMarkName = L"";

    m_bIsComboState = false;

    vParticleToDamageMagicNumber = D3DXVECTOR2(1.0f, 0.48f);

    InitHittingGauge();

    m_bMariSummonState = false;

    m_bIdle = false;
    m_bNotTargeting = false;

    m_bSpecialShield = false;

    m_pGuildMarkTexture = NULL;
    m_bWeaponChanging = false;
    m_bOngameItemChange = false;

    m_iSuperArmorFrame = -1;

    m_fLimitDistance = 10000.0f;
    m_fFall_Speed = DEAULT_FALL_SPEED;
    m_iSelectTextrue = -1;

    m_fDamageToMPAttack = 0.0f;
    m_fStaticToMPAttack = 0.0f;
    m_fReflectionDamageRatio = 0.0f;
    m_fMPDecreaseRatio = 1.0f;
    m_fMPDecreaseRatioForSpecial = 1.0f;

    m_bMeshToggleState = false;
    m_iDotaPoint = 0;
    m_iMaxDotaPoint = 0;
    m_bUseNotAP = false;
    m_uiNotSpecialSuper = 0;

    m_bWeaponRenderState = true;
    m_bMergeWeaponRenderState = true;

    m_bSetSelectMotion = false;
    m_bResultTower = false;

    m_iQPChargeFrame = 0;
    m_iQuantityPoint = GetCurFormTemplate().iMaxQP;

    SetCommonMontion();

    m_iComboSkillHotKeyIdx = -1;
    m_iComboSkillComboIdx = -1;
    m_iComboSkillNowComboIdx = -1;

    m_dwPrevTagTime = 0;

    m_pEclipseBack = NULL;

    x_WindSpeed = 0.0f;
    y_WindSpeed = 0.0f;
    m_fJump_WindRadio = 1.0f;
    m_iDamageSummonMonSlot = -1;

    m_fMinusAPCoolTime = 0.f;

    m_iDefaultFaceLiftType = FACE_LIFT_NORMAL;
    m_mapFaceLiftDelayInfo.clear();

    m_fManaShieldRatio = 0.f;

    m_bIsRenderBaseMesh = true;
    m_bIsRenderFace = true;

    m_vecCharAbilityState.clear();

    m_iShieldTexNum = -1;
    m_bShieldTypeCounting = false;

    m_bMPRecoveryFix = false;
    m_fMPRecoveryFix = 0.0f;
    m_iEmbarkTypeOfInPortal = INT_MAX;
    m_dwEmbarkUIDOfInPortal = INT_MAX;
    m_cSelectedPromotion = -1;

    m_pSqueDanger = NULL;

    m_fFatigueRatio = 100.f;
    m_bNoRecoveryMpMotion = false;

    m_bRenderChildMeshParticle = true;
    m_bSavingMotion = false;

    m_bNoUseSkill = false;

    ClearControlInfo();
}

PLAYER::~PLAYER()
{
    // 댕글링. 삭제할 필요가 없다 
    /*   SAFE_RELEASE(m_pTexBerserk1);
    SAFE_RELEASE(m_pTexBerserk2);
    SAFE_RELEASE(m_pTexFatal);
    SAFE_RELEASE(m_pTexBasicSp);
    SAFE_RELEASE(m_pTexProgressSp);
    SAFE_RELEASE(m_pElesis3Sp);
    SAFE_RELEASE(m_pLire3Sp);
    SAFE_RELEASE(m_pWarLockDAssassinSp);
    SAFE_RELEASE(m_pTexLasSp);
    SAFE_RELEASE(m_pTexSlash);
    SAFE_RELEASE(m_pRyanSp);
    SAFE_RELEASE(m_pCentinelSp);
    SAFE_RELEASE(m_pNephillimSp);
    SAFE_RELEASE(m_pMDanSp);
    SAFE_RELEASE(m_pElesis4Sp);
    SAFE_RELEASE(m_pPlayerSp);
    SAFE_RELEASE(m_pSerdinSign);
    SAFE_RELEASE(m_pCanabanSign);*/
}

bool PLAYER::Take_Item(char What, bool bNoMessage /*= false*/) // 현 플레이어의 아이템 목록에 아이템이 추가됨
{
    if (NUM_TAKE_ITEM <= GetNumGameItem())
    {
        if (false == bNoMessage)
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DOTA_GAMEITEM_BUY_FAIL3), L"", KGCUIScene::GC_MBOX_USE_DOTA_GAME, 0, 0, false, true);
        return false;
    }

    // Game Item Enum의 순서때문에 하드 코딩을 감행 합니다.-_-;
    if (What == GC_GAME_ITEM_SANTA_GIFT || What == GC_GAME_ITEM_RICECAKE)
    {
        // GP를 제외한 다른 게임 아이템들이 나와야함
        int iItemKinds = rand() % GC_GAME_ITEM_HASTE;
        if (iItemKinds == GC_GAME_ITEM_GP)
        {
            // 퀘스트이면 헤이스트 안나옴
            if (SiKGCRoomManager()->GetGameModeType() == GC_GMT_UNION_FIGHT)
                iItemKinds = rand() % GC_GAME_ITEM_GP; // 6
            else
                iItemKinds = GC_GAME_ITEM_HASTE;
        }

        //정말 말로 표현할수 없을만큼 거지 같군-_-;
        //산타일때 반사템 안나오도록 해준다.
        if (What == GC_GAME_ITEM_SANTA_GIFT)
        {
            while (iItemKinds == GC_GAME_ITEM_REFLECTION)
            {
                iItemKinds = rand() % GC_GAME_ITEM_HASTE;
                if (iItemKinds == GC_GAME_ITEM_GP)
                {
                    // 퀘스트이면 헤이스트 안나옴
                    if (SiKGCRoomManager()->GetGameModeType() == GC_GMT_UNION_FIGHT)
                        iItemKinds = rand() % GC_GAME_ITEM_GP; // 6
                    else
                        iItemKinds = GC_GAME_ITEM_HASTE;
                }
            }

        }
        What = iItemKinds;
    }
    m_vecGameItem.push_back(What);

    if (SiKGCRoomManager()->GetGameMode() == GC_GM_DOTA)
    {
        if (m_iPlayerIndex == g_MyD3D->Get_MyPlayer())
        {
            g_MyD3D->MyItms->SetItemCursorPos(GetNumGameItem() - 1);
            g_MyD3D->MyItms->SetSelectedItem(What);
        }
    }
    else
    {
        if (m_iPlayerIndex == g_MyD3D->Get_MyPlayer() && 1 == GetNumGameItem())
            g_MyD3D->MyItms->SetItemCursorPos(0);
    }
    return true;
}

void PLAYER::Eff_Item(EFFITEM EffItem) // 현 플레이어에 적용되는 아이템이 영향을 미치게 함(Count를 시작함)
{
    if (EffItem.cWhat == -1)
        return;

    // 헤이스트 영향을 받지 않음.
    if (EffItem.cWhat == GC_GAME_ITEM_HASTE && 0.0f != m_fAbility[ABILITY_IGNORE_HASTE])
        return;

    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_BROADCASTING) && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN)
        return;

    switch (EffItem.cWhat)
    {
    case GC_GAME_ITEM_SUPER:
        m_pObject->MeshStateOn(MS_IMMORTAL);
        break;
    case GC_GAME_ITEM_HALLOW:
        SetMagicEffect(EGC_EFFECT_INVISIBLE, EffItem.sTime);
        break;
    default:
        break;
    }
    if (EffItem.sTime == -1)
        Item_Remain_Time[EffItem.cWhat] = Item_Time[EffItem.cWhat];
    else
        Item_Remain_Time[EffItem.cWhat] = EffItem.sTime;

    if (GC_GM_DOTA == SiKGCRoomManager()->GetGameMode())
    {
        // 점령전일때는 ?이미지를 띄워주도록 하자!!
        if (IsLocalPlayer() && g_pkUIScene->m_pkGameOverlayUI)
            g_pkUIScene->m_pkGameOverlayUI->m_pkDotaItemImg->ShowItemImg(EffItem.cWhat);

        // 사용한 아이템에 대한 통계를 남기자!!
        std::map<GCITEMID, int>::iterator mapIter = m_sDotaStats.m_mapItemUseCount.find(EffItem.cWhat);
        if (mapIter != m_sDotaStats.m_mapItemUseCount.end())
            ++mapIter->second;
        else
            m_sDotaStats.m_mapItemUseCount[EffItem.cWhat] = 1;
    }
}


//void PLAYER::OnResetDevice()
void PLAYER::InitDevice()
{
    /*SLVERTEX v[10 + ( LATENCY + SHOWLATENCY )*2];
    for( int i = 0; i < 10 + ( LATENCY + SHOWLATENCY )*2; i++ )
    {
    v[i].SetSLVER(0,0,0,0,0);
    }
    V_Slash = g_pGCDeviceManager2->CreateVertexBuffer( (5+(LATENCY+SHOWLATENCY))*2*sizeof(SLVERTEX), D3DUSAGE_WRITEONLY, FVF_LVERTEX, D3DPOOL_DEFAULT, NULL);
    memcpy( V_Slash->Lock(), v, (5+(LATENCY+SHOWLATENCY))*2*sizeof(SLVERTEX) );
    V_Slash->Unlock();*/

    //SLVERTEX v[10 + ( LATENCY + SHOWLATENCY ) * 2 ];
    //for( int i = 0; i < 10 + ( LATENCY + SHOWLATENCY ) * 2; i++ )
    //{
    //    v[i].SetSLVER( 0, 0, 0, 0, 0 );
    //}
    //    V_Slash = g_pGCDeviceManager2->CreateVertexBuffer( ( 5 + ( LATENCY + SHOWLATENCY ) ) * 2 * sizeof( SLVERTEX ), D3DUSAGE_WRITEONLY, FVF_LVERTEX, D3DPOOL_DEFAULT, ( void* )v );
    //    V_Slash_Left = g_pGCDeviceManager2->CreateVertexBuffer( ( 5 + ( LATENCY + SHOWLATENCY ) ) * 2 * sizeof( SLVERTEX ), D3DUSAGE_WRITEONLY, FVF_LVERTEX, D3DPOOL_DEFAULT, ( void* )v );

    Slash[0].Init();
    Slash[1].Init();
    Slash[2].Init();
    Slash[3].Init();

    for (int i = 0; i < PAT_NUM; i++)
    {

        AttackSlash[i].SetLength(200);
        AttackSlash[i].Init();

        AttackSlash[i].SetColor_(255,
            i * (255 / PAT_NUM),
            (i + (PAT_NUM / 3)) * (255 / PAT_NUM),
            (i + (PAT_NUM / 3) * 2) * (255 / PAT_NUM)
        );

    }

    return;
}


void PLAYER::MoveXInSky()
{
    //공중에 있어야함
    if (IsContact || uiMotion == GetOwnMotion(MID_COMMON_ARENA))
        return;
    if (uiMotion == MID_ELESIS_RUN && cFrame < 30) //옛날꺼랑 맞춰야함
        return;

    //둘중 하나는 눌려져야함
    if (!(g_MyD3D->MyCtrl->k_Right || g_MyD3D->MyCtrl->k_Left) || !g_MyD3D->EnableKeyInput())
        return;

    //아래의 모션에서는 움직이지 못함
    if (uiMotion == GetOwnMotion(MID_COMMON_DOWN_IN_SKY) ||
        uiMotion == GetOwnMotion(MID_COMMON_DAMAGED_BY_FLYATK) ||
        uiMotion == MID_LIRE_LEGATK ||
        uiMotion == MID_ELESIS3_JUMPATK_DOWN ||
        uiMotion == MID_LIRE4_DOUBLEJUMP_ATK)
        return;

    //아니라면 움직일 수 있음 공중에서 조금씩 빨라지게 만드는 부분
    //수치는 우선 하드코딩으로 해둡니다.
    if (g_MyD3D->MyCtrl->k_Right)
    {
        if (x_Speed < 0.012f)
            x_Speed += 0.0015f;
        return;
    }
    if (g_MyD3D->MyCtrl->k_Left)
    {
        if (x_Speed > -0.012f)
            x_Speed -= 0.0015f;
        return;
    }
}

void PLAYER::StartAttack(float fDamage, int iPhysicAtkType, int iDamageType)
{
    m_fLastPower = m_fPhysicAtkPower = fDamage;
    m_ePhysicAtkType = (ePhysicAtkType)iPhysicAtkType;
    m_ePhysicAtkDamageAction = (ePhysicAtkReaction)iDamageType;

    m_iCameraCrash = 0;
    m_iPlusDelay = 0;
    //Mexe nessa porra aqui
    //Delay dos ataques
    m_iHitDelay = 8;

    m_eAttackType = ATTACKTYPE_NORMAL;
    m_eAttackDirection = ATTACKDIR_MIDDLE;
    m_fPushTargetX = 0.0f;
    m_fPushTargetY = 0.0f;
    m_bSpecialAttack = false;

    if (IsSpecialSkillMotion())
        m_bSpecialAttack = true;
}

void PLAYER::SetAttackProperty(int iAttackType /*= (int)ATTACKTYPE_NORMAL*/, int iAttackDirection /*= (int)ATTACKDIR_MIDDLE*/,
    float fPushTargetX /*= 0.0f*/, float fPushTargetY /*= 0.0f*/, bool bSpecialAttack /*= false*/, float fPushPlayerX/*=0.0f*/, float fPushPlayerY/*=0.0f*/)
{
    // 몬스터에게만 해당되는 속성들 입니다
    m_eAttackType = (EGCAttackType)iAttackType;
    m_eAttackDirection = (EGCAttackDirection)iAttackDirection;
    m_fPushTargetX = fPushTargetX;
    m_fPushTargetY = fPushTargetY;
    m_bSpecialAttack = bSpecialAttack;

    // 플레이어에게만 해당되는 속성
    m_fPlayerPhysicPushX = fPushPlayerX;
    m_fPlayerPhysicPushY = fPushPlayerY;

    if (IsSpecialSkillMotion())
        m_bSpecialAttack = true;
}

void PLAYER::EndAttack()
{
    m_fPhysicAtkPower = 0.0f;
    m_fPlayerPhysicPushX = 0.0f;
    m_fPlayerPhysicPushY = 0.0f;
}

CDamageInstance* PLAYER::AddDamage(unsigned short Reserved1, unsigned short Reserved2, bool bAlwaysRight /* = false */, bool bSpecialAttack /*= false*/)
{
    PROFILE_SET("PLAYER::AddDamage");

    float fTemp;
    bool bTemp;

    if (Reserved2)
    {
        fTemp = Body_Angle;
        bTemp = bIsRight;

        short Angle = Reserved2;

        if (Extra_Char_Num != CID_LIRE3 && Extra_Char_Num != CID_LAS3)
        {
            if (Reserved2 > 90 && Reserved2 < 270)
            {
                bIsRight = !bIsRight;
                Angle = 180 - (short)Reserved2;
            }
        }
        else
        {
            Angle = (short)Reserved2;
        }

        Body_Angle = (float)Angle / 180.0f * D3DX_PI;
    }

    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->Add((int)Reserved1, m_iPlayerIndex, false, -1, NULL, bAlwaysRight);

    if (bSpecialAttack && pkDamage)
    {
        pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
        //      pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
    }

    if (Reserved2)
    {
        Body_Angle = fTemp;
        bIsRight = bTemp;
    }

    AddDamageRelation(pkDamage);
    return pkDamage;
}

void PLAYER::AddDamageAngleAndPos(unsigned short Reserved1, unsigned short Reserved2, float fPosX, float fPosY)
{
    CDamageInstance* pkDamage = AddDamage(Reserved1, Reserved2);

    if (pkDamage)
    {
        pkDamage->m_X = (vPos.x + fPosX);
        pkDamage->m_Y = (vPos.y + fPosY);
    }
}

bool PLAYER::AddDamageTarget(unsigned short Reserved1, D3DXVECTOR2* vpTargetPos)
{
    bool bResult = false;
    if (vpTargetPos)
    {
        for (int iLoop = 0; iLoop < MAX_TARGET_DAMAGE; ++iLoop)
        {
            if (vpTargetPos[iLoop].x || vpTargetPos[iLoop].y)
            {
                CDamageInstance* pkDamage = AddDamage(Reserved1, 0);
                if (pkDamage)
                {
                    pkDamage->m_X = vpTargetPos[iLoop].x + pkDamage->m_pOriginalDamage->m_Start_Locate_x;
                    pkDamage->m_Y = vpTargetPos[iLoop].y + pkDamage->m_pOriginalDamage->m_Start_Locate_y;
                    bResult = true;
                }
            }
        }
    }
    return bResult;
}

void PLAYER::AddDamageNoLatency(int iWhat, bool bSpecialAttack /*= false*/)
{
    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->Add(iWhat, m_iPlayerIndex, true, -1, NULL, false);

    if (bSpecialAttack && pkDamage)
    {
        pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
        //      pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
    }

    AddDamageRelation(pkDamage);
}

void PLAYER::AddDamageWithLocate(int iWhat, float fXLoc, float fYLoc, bool bSpecialAttack /*= false*/, bool bNoLatency)
{
    if (bIsRight == false)
        fXLoc = -fXLoc;

    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->AddWithLocate(iWhat, m_iPlayerIndex, vPos.x + fXLoc, vPos.y + fYLoc);

    if (pkDamage)
    {
        if (bNoLatency)
            pkDamage->m_SlowCount = 0;

        if (bSpecialAttack)
        {
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
            //          pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
        }
    }

    AddDamageRelation(pkDamage);
}

void PLAYER::AddDamageWithLocateAbsolutePos(int iWhat, float fXLoc, float fYLoc, bool bSpecialAttack /*= false */)
{
    if (bIsRight == false)
        fXLoc = -fXLoc;

    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->AddWithLocate(iWhat, m_iPlayerIndex, fXLoc, fYLoc);

    if (bSpecialAttack && pkDamage)
    {
        pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
        //      pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
    }

    AddDamageRelation(pkDamage);
}
void PLAYER::AddDamageWithNoDirectionLocateAbsolutePos(int iWhat, float fXLoc, float fYLoc, bool bSpecialAttack /*= false */)
{
    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->AddWithLocate(iWhat, m_iPlayerIndex, fXLoc, fYLoc);

    if (bSpecialAttack && pkDamage)
    {
        pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
        //      pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
    }

    AddDamageRelation(pkDamage);
}
void PLAYER::AddDamageOnP2P(int What)
{
    if (IsLocalPlayer())
    {
        LTP_DAMAGE packet;
        packet.bAbsoluePos = false;
        packet.iCharIndex = m_iPlayerIndex;
        packet.iDamage = What;
        packet.bMonsterDamage = false;

        g_LatencyTransfer.PushPacket(&packet);
    }

}

void PLAYER::AddDamageWithStaticLocate(int iWhat, float fStartX, float fStartY, bool bSpecialAttack /*= false*/, bool bNoLatency /*= false*/)
{
    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->AddWithLocate(iWhat, m_iPlayerIndex, fStartX, fStartY);

    if (pkDamage)
    {
        if (bNoLatency)
            pkDamage->m_SlowCount = 0;

        if (bSpecialAttack)
            pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;

        pkDamage->m_X = fStartX;
        pkDamage->m_Y = fStartY;
    }

    AddDamageRelation(pkDamage);
}

void PLAYER::CheckDie()
{
    // 넘어졌다 일어서는 모션일때 죽음을 체크합니다.
    if (GetHP() <= 0)
    {
        Slow_Count = 100;
        g_MyD3D->m_bNormalEgg = true;

        if (IsLocalPlayer() || GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
        {
            ProcessMetamorphosisNormalForm(true);

            if (Extra_Char_Num == CID_RONAN3)
                InitForceNum();

            InitMonoVolt();
            DeleteMagicShield();

            //SetNormalTpye();
            if (m_bIsBurnningMode)
                ChangeBurnningTemplate();

            if (0 <= GetLastKillmePlayer() && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
            {
                if (GetLastKillmePlayer() < MAX_PLAYER_NUM)//Player한테 죽음
                {
                    float fGetExpGpRatio = GetExpGpRatio(GetLastKillmePlayer(), m_iPlayerIndex);
                    KGC_PID_BROAD_PLAYER_DEATH_INFO kPacket(GC_PID_BROAD_PLAYER_DEATH_INFO,
                        g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.dwUID,/////g_kGlobalValue.m_kUserInfo.dwUID,
                        g_MyD3D->MyPlayer[GetLastKillmePlayer()]->m_kUserInfo.dwUID,
                        fGetExpGpRatio, m_iPlayerIndex == GetLastKillmePlayer(), false);
                    SERIALIZE_P2P_PACKET(kPacket);
                    SendP2PPacket((void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST, _RELIABLE);
                    KGCPC_MEMO("GC_PID_BROAD_PLAYER_DEATH_INFO");
                }
                else
                {
                    KGC_PID_BROAD_PLAYER_DEATH_INFO kPacket(GC_PID_BROAD_PLAYER_DEATH_INFO,
                        g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.dwUID,//g_kGlobalValue.m_kUserInfo.dwUID,
                        GetLastKillmePlayer() - MAX_PLAYER_NUM,
                        0.0f, false, false, true);
                    SERIALIZE_P2P_PACKET(kPacket);
                    SendP2PPacket((void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST, _RELIABLE);
                    KGCPC_MEMO("GC_PID_BROAD_PLAYER_DEATH_INFO");
                }
            }

            m_bDirectKill = false;
        }
        x_Speed = 0;
    }
}

void PLAYER::Catched(float fOffsetX)
{
    if (fOffsetX == 0.0f)
        fOffsetX = 0.1f;

    int iMyPlayerNo = g_MyD3D->Get_MyPlayer();

    if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
    {
        iMyPlayerNo = 1;
        m_iCatchPlayerIndex = 1;
    }

    //내가 잡힌것이 아니면 리턴        
    if (iMyPlayerNo != m_iCatchPlayerIndex)
        return;
    //서로서로 잡았을 때의 처리
    //나도 잡기 동작인지 알아봄
    if (IsCatchMotion(g_MyD3D->MyPlayer[iMyPlayerNo]->uiMotion)) {
        return;
    }

    m_fPhysicAtkPower = 0.0f;


    // [2/4/2008] breadceo. 엘숲을 위한 하드코딩
    if (false == SiKGCRoomManager()->IsMonsterGameMode())
    {
        unsigned char ucStage = SiKGCRoomManager()->GetRoomInfo().ucStage;

        switch (ucStage) {
        case GC_GS_FOREST_OF_ELF:
#if defined ( NATION_BRAZIL ) || defined ( NATION_KOREA ) || defined ( NATION_USA )
        case GC_GS_AMAZON_FOREST:
#endif
        case GC_GS_FOREST_OF_ELF_REVERS:
            if (g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x >= 0.45f && g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x <= 0.51f)
            {
                g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = 0.51f;
            }
            break;

        default:
            break;
        }
    }

    // [2/4/2008] breadceo. 잡기전에 안전한 위치를 계산하자
    D3DXVECTOR2 vSafePos = g_MyD3D->MyStg->Check_SafePostionX(g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x, vPos.y);

    //잡혀주기
    g_MyD3D->MyPlayer[iMyPlayerNo]->bIsRight = !bIsRight;

    //앞으로는 이거 쓰지말고 actionScript에서 처리할 수 있도록 합시다.
    if (bIsRight)
    {
        if (uiMotion == MID_LAS3_CATCH || uiMotion == MID_FORM_WOLF_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.01f;
        else if (uiMotion == MID_RYAN_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.03f;
        else if (uiMotion == MID_RYAN2_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.05f;
        else if (uiMotion == MID_AMY1_CATCH || uiMotion == MID_AMY2_PERFORMENCE_CATCH || uiMotion == MID_AMY2_CATCH ||
            uiMotion == MID_AMY3_CATCH || uiMotion == MID_AMY3_DANCE_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.14f;
        else if (uiMotion == MID_RONAN_CATCH || uiMotion == MID_RONAN2_CATCH || uiMotion == MID_RYAN4_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.175f;
        else if (uiMotion == MID_LIRE4_CATCH || uiMotion == MID_RYAN4_MDAN_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.2f;
        else if (uiMotion == MID_RONAN3_CATCH || uiMotion == MID_RONAN4_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.22f;
        else if (uiMotion == MID_ELESIS4_CATCH || uiMotion == MID_RYAN3_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.25f;
        else if (uiMotion == MID_ARME4_CATCH || uiMotion == MID_JIN1_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.3f;
        else if (uiMotion == MID_JIN_CATCH || uiMotion == MID_JIN2_2_CATCH || uiMotion == MID_JIN2_CATCH) // 진 잡기 설정
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + 0.185f;
        else
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x + fOffsetX;
    }
    else
    {
        if (uiMotion == MID_LAS3_CATCH || uiMotion == MID_FORM_WOLF_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.01f;
        else if (uiMotion == MID_RYAN_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.03f;
        else if (uiMotion == MID_RYAN2_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.05f;
        else if (uiMotion == MID_AMY1_CATCH || uiMotion == MID_AMY2_PERFORMENCE_CATCH || uiMotion == MID_AMY2_CATCH ||
            uiMotion == MID_AMY3_CATCH || uiMotion == MID_AMY3_DANCE_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.14f;
        else if (uiMotion == MID_RONAN_CATCH || uiMotion == MID_RONAN2_CATCH || uiMotion == MID_RYAN4_CATCH || uiMotion == MID_RONAN4_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.175f;
        else if (uiMotion == MID_LIRE4_CATCH || uiMotion == MID_RYAN4_MDAN_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.2f;
        else if (uiMotion == MID_RONAN3_CATCH || uiMotion == MID_RONAN4_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.22f;
        else if (uiMotion == MID_ELESIS4_CATCH || uiMotion == MID_RYAN3_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.25f;
        else if (uiMotion == MID_ARME4_CATCH || uiMotion == MID_JIN1_CATCH)
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.3f;
        else if (uiMotion == MID_JIN_CATCH || uiMotion == MID_JIN2_2_CATCH || uiMotion == MID_JIN2_CATCH) // 진 잡기 설정
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - 0.185f;
        else
            g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.x = vPos.x - fOffsetX;
    }

    if (vSafePos.y > vPos.y)
    {
        g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.y = vSafePos.y;
    }
    else
    {
        g_MyD3D->MyPlayer[iMyPlayerNo]->vPos.y = vPos.y;
    }
    int slowcount = 0;
    if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
        slowcount = LATENCY;

    // 키 커맨드 입력제어 초기화
    g_MyD3D->SetEnableKeyInput(true);
    g_MyD3D->MyPlayer[iMyPlayerNo]->Slow_Count = slowcount;
    g_MyD3D->MyPlayer[iMyPlayerNo]->cFrame = cFrame;
    g_MyD3D->MyPlayer[iMyPlayerNo]->SetPlayerState(uiMotion + 1);
    g_MyD3D->MyPlayer[iMyPlayerNo]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 0;
    g_MyD3D->MyPlayer[iMyPlayerNo]->SetLastKillmePlayer(m_iPlayerIndex);
}

void PLAYER::OnReverseGravity(float fRangeX, float fDamage, bool bFlyCheck, float fHeight, float fRangeY, float fOffSetX, float fOffSetY, bool bSpecialDamage)
{
    if (fHeight == 0.0f)
        fHeight = 0.06f;

    D3DXVECTOR2 vecPos(vPos.x + fOffSetX, vPos.y + fOffSetY);
    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i) == false)
        {
            if (0.0f < fRangeY)
            {
                if ((g_MyD3D->MyPlayer[i]->vPos.x > vecPos.x - fRangeX && g_MyD3D->MyPlayer[i]->vPos.x < vecPos.x + fRangeX) &&
                    (g_MyD3D->MyPlayer[i]->vPos.y > vecPos.y - fRangeY && g_MyD3D->MyPlayer[i]->vPos.y < vecPos.y + fRangeY))
                {
                    g_MyD3D->MyPlayer[i]->ReverseGravity(m_iPlayerIndex, fHeight, fDamage, !bFlyCheck, bSpecialDamage);
                }
            }
            else
            {
                if ((g_MyD3D->MyPlayer[i]->vPos.x > vecPos.x - fRangeX && g_MyD3D->MyPlayer[i]->vPos.x < vecPos.x + fRangeX))
                {
                    g_MyD3D->MyPlayer[i]->ReverseGravity(m_iPlayerIndex, fHeight, fDamage, !bFlyCheck, bSpecialDamage);
                }
            }
        }
    }

    if (g_kGlobalValue.m_kUserInfo.bHost)
    {
        fDamage *= 500;
        float fRangeToMonsterX = fRangeX * 1.0f;
        float fRangeToMonsterY = fRangeY * 1.0f;
        float fDamageToLarge = fDamage;
        //if( uiMotion == MID_AMY4_SPECIAL3 )
        //    fDamageToLarge =  fDamage / 17.0f;
        // 몬스터도 리버스그라비티!!
        for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
            MONSTER* pMonster = mit->second;
            if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
                continue;

            if (0.0f < fRangeY)
            {
                if ((pMonster->GetX() > vecPos.x - fRangeToMonsterX && pMonster->GetX() < vecPos.x + fRangeToMonsterX) &&
                    (pMonster->GetY() > vecPos.y - fRangeToMonsterY && pMonster->GetY() < vecPos.y + fRangeToMonsterY) &&
                    pMonster->IsLive())
                {
                    if (bFlyCheck || (!bFlyCheck && pMonster->GetIsContact()))
                    {
                        float fRealDamage = fDamage;
                        //if( g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].bNoDamageReaction )
                        //{
                        //	fRealDamage = fDamageToLarge;
                        //}

                        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
                        pMonster->m_vDamageStartPos = D3DXVECTOR2(vPos.x, vPos.y);
                        pMonster->m_vDamageWhoPos = D3DXVECTOR2(vPos.x, vPos.y);

                        pMonster->OnDamage(m_iPlayerIndex, DT_REVERSE_GRAVITY, fRealDamage, ATTACKTYPE_DOWN, ATTACKDIR_UP, 0.0f, fHeight, true, 0, true);
                    }
                }
            }
            else
            {
                if (pMonster->GetX() > vecPos.x - fRangeToMonsterX && pMonster->GetX() < vecPos.x + fRangeToMonsterX &&
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
                        pMonster->m_vDamageStartPos = D3DXVECTOR2(vPos.x, vPos.y);
                        pMonster->m_vDamageWhoPos = D3DXVECTOR2(vPos.x, vPos.y);
                        pMonster->OnDamage(m_iPlayerIndex, DT_REVERSE_GRAVITY, fRealDamage, ATTACKTYPE_DOWN, ATTACKDIR_UP, 0.0f, fHeight, true, 0, true);
                    }
                }
            }
        }
    }
}

void PLAYER::CheckDie2End()
{
    // Is_IfDie2End 검사(보통 아레나의 등장시 검사함)
    // 이 플레이어가 죽는다면 승부는 끝나는가? 검사
    Is_Aleady_Scream = false;

    // 승부 계산
    if (GC_GM_SURVIVAL_MODE == SiKGCRoomManager()->GetGameMode())
    {
        unsigned char Num_Player = 0;
        for (unsigned char i = 0; i < 6; i++)
        {
            if (g_MyD3D->MyPlayer[i]->m_cLife > 0)
                Num_Player++;
        }
        if (Num_Player < 3)
        {
            // 싸우는 사람이 2명 이하일 때...
            if (m_cLife < 2)
                Is_IfDie2End = true;
        }
    }
    else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
    {
        unsigned char Num_Red = 0, Num_Blue = 0;
        for (unsigned char i = 0; i < MAX_PLAYER_NUM; i++)
        {
            if (g_MyD3D->MyPlayer[i]->m_cLife > 0)
            {
                if (g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam())
                    Num_Blue++;
                else
                    Num_Red++;
            }
        }
        bool bNext = (GetNextTagChar() == GC_CHAR_INVALID);
        if (Num_Blue < 2 && bNext && m_kUserInfo.IsBlueTeam())
            Is_IfDie2End = true;
        if (Num_Red < 2 && bNext && !m_kUserInfo.IsBlueTeam())
            Is_IfDie2End = true;
    }
    else if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
    {
        unsigned char Num_Red = 0, Num_Blue = 0;
        for (unsigned char i = 0; i < MAX_PLAYER_NUM; i++)
        {
            if (g_MyD3D->MyPlayer[i]->m_cLife > 0)
            {
                if (g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam())
                    Num_Blue++;
                else
                    Num_Red++;
            }
        }
        if (Num_Blue < 2 && m_cLife < 2 && m_kUserInfo.IsBlueTeam())
            Is_IfDie2End = true;
        if (Num_Red < 2 && m_cLife < 2 && !m_kUserInfo.IsBlueTeam())
            Is_IfDie2End = true;
    }
}

void PLAYER::ChangeMotion(int iMotion)
{
    if (!IsLocalPlayer())
        return;

#if defined( USE_ONGAME_INVENTORY )
    if ((iMotion == MID_WOLF_FORM_START || iMotion == MID_NEPHILLIM_FORM_START || iMotion == MID_RYAN4_MDAN_FORM_START) &&
        KD3DWnd::D3DWS_ENABLED == g_pkUIScene->GetStateUI(KGCUIScene::GC_MBOX_GAME_INVENTORY)) {
        g_pkUIScene->CloseGameInventory();
    }
#endif

    if (false == VerifyRealMotion(iMotion))
    {
        if (IsContact)
            iMotion = GetWaitMotion();
        else
            iMotion = GetCurFormTemplate().JUMP;
    }

    // 탑승시 탑승물에 등록된 모션만 체인지 가능
    if (UINT_MAX > GetEmbarkUID())
    {
        bool bReturn = true;
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark)
        {
            std::vector<int> vecEmbarkMotion = SiKGCEmbarkManager()->GetCharMotionList();
            std::vector<int>::iterator vecIter = vecEmbarkMotion.begin();
            for (; vecIter != vecEmbarkMotion.end(); ++vecIter)
            {
                if ((*vecIter) == iMotion)
                {
                    bReturn = false;
                    break;
                }
            }
        }
        if (bReturn)
            return;
    }

    Reserve_Motion = iMotion;
    Reserve_IsRight = bIsRight;
    Next_Attack_Count = GetCurFormTemplate().iNextAttackCount;
    Next_Motion = -1;
    Next_Direction = -1;

    // 모션이 바뀌기 전 해당 모션에서 사용한 파티클 리스트를 제거한다.
    ClearMotionParticleList();

    m_bDrawSlashRight = false;
    m_bDrawSlashLeft = false;
    m_bDrawSlashLegRight = false;
    m_bDrawSlashLegLeft = false;
    m_bNoCrashCheck = false;
    m_bNoRecoveryMpMotion = false;
    m_bRenderChildMeshParticle = true;
    m_bSavingMotion = false;

    m_bNoUseSkill = false;
}

void PLAYER::Change_MP(float fChange)
{
    if (IsLocalPlayer() == false)
        return;


    g_MyD3D->MyHead->Level += fChange;
    if (g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP())
    {
        g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
    }

    if (g_MyD3D->MyHead->Level < 0)
        g_MyD3D->MyHead->Level = 0;


}

void PLAYER::ChangeAllCharacterMP(float fChange)
{
    if (fChange < 0)
    {
        g_MyD3D->MyHead->Level += fChange;
    }
    else
    {
        if (g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP())
            g_MyD3D->MyHead->Level += fChange;
    }

    if (g_MyD3D->MyHead->Level < 0)
        g_MyD3D->MyHead->Level = 0;

}

void PLAYER::DecreaseMP(float fChange)
{
    if (IsLocalPlayer() == false)
        return;

    g_MyD3D->MyHead->Level += fChange;

    if (g_MyD3D->MyHead->Level < 0.0f)
        g_MyD3D->MyHead->Level = 0.0f;

    if (g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP())
        g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();


}

void PLAYER::AddItem()
{
    if (!g_kGlobalValue.m_kUserInfo.bHost)
        return;

    D3DXVECTOR2 Vec;
    if (bIsRight)
        Vec.x = vPos.x - 0.05f + 0.135f;
    else
        Vec.x = vPos.x - 0.05f - 0.135f;

    Vec.y = vPos.y + 0.2f;
    g_MyD3D->MyItms->Add(rand() % 8, Vec, false, 3000);
}

void PLAYER::Jump()
{

    y_Speed = GetCurFormTemplate().fJumpYSpeed;

    SetPlayerState(GetCurFormTemplate().JUMP);
    cFrame = 0;
    IsContact = false;

    if (Item_Remain_Time[GC_GAME_ITEM_WING])
        y_Speed = 0.05f;

    SetWindSpeed();
}

void PLAYER::JumpMotion(float fJumpSpeed, int iMotionIndex)
{

    y_Speed = fJumpSpeed;

    SetPlayerState(iMotionIndex);
    cFrame = 0;
    IsContact = false;
}

void PLAYER::Run()
{
    if (IsLocalPlayer() == false)
        return;

    if (g_MyD3D->MyCtrl->k_Left && g_MyD3D->MyCtrl->k_Right)
        return;

    if ((bIsRight && g_MyD3D->MyCtrl->k_Right) || (!bIsRight && g_MyD3D->MyCtrl->k_Left))
    {
        cFrame = 0;
        SetPlayerState(GetCurFormTemplate().RUN);
    }
}

void PLAYER::StartSpecial(BYTE r, BYTE g, BYTE b, bool bNoSound, float fZoom, bool bNoFace, bool bNoLightBim)
{
    if (0.0f == fZoom) fZoom = 2.0f;

    //파티클 전체 화면 특수무기 효과
    //파티클 시스템 업데이트
    for (int i = 0; i < (int)m_vecSkillEff.size(); ++i)
    {
        ITEM_EFFECT& stEffect = m_vecSkillEff[i];
        CreateItemEffectParam kParam(m_bShowEffect, bIsRight, stEffect, m_posDesc);
        kParam.x = m_posDesc.body.x;
        kParam.y = m_posDesc.body.y;
        kParam.z = m_posDesc.body.z;


        CreateItemEffect(kParam);
    }

    for (std::vector<KGCAnimManager::SkillAnim*>::iterator j = m_SkillAnim.begin(); j != m_SkillAnim.end(); j++)
    {
        g_AnimManager.FrameUpdateSkillAnim((*j), &m_posDesc);
    }

    if (m_Elemental == ITEM_ELEMENTAL_FIRE && m_StrongLevel >= 8)
    {
        g_KDSound.Play("FireSkill");
    }

    if (IsLocalPlayer())
    {
        if (NULL == g_pGraphicsHelper->GetPlayerActionMotion(uiMotion))
            return;

        float fydelta = 0.0f;
        fydelta = g_pGraphicsHelper->GetPlayerActionMotion(uiMotion)->GetFrameData(cFrame).Pos_y * CHAR_SCALE_RATIO;
#if !defined( __PATH__ )
        if (g_MyD3D->IsGifRecordMode() == false)
#endif
            if (0.0f < fZoom && g_kCamera.CheckSpecialZoom())
                g_kCamera.SetZoom(fZoom, g_MyD3D->Get_MyPlayer());

        g_MyD3D->MyStg->Color_Stop_Count = 50 + (LATENCY + SHOWLATENCY);

        // 2007/5/18. daeuk. 조선컴들..
        if (g_MyD3D->m_KGCOption.IsSkillEffect() && !bNoLightBim)
        {
            g_MyD3D->m_pDamageManager->AddWithLocate(DT_SKILL_READY_CIRCLE, m_iPlayerIndex, vPos.x, vPos.y + fydelta);
            g_MyD3D->MySparks->LightBim(vPos.x - 1.0f, vPos.y + y_Ex - .2f, LATENCY + SHOWLATENCY);
        }
    }
    else
    {
        g_MyD3D->MyStg->Color_Stop_Count = 50;
        // 2007/5/18. daeuk. 조선컴들..
        if (g_MyD3D->m_KGCOption.IsSkillEffect() && !bNoLightBim)
        {
            g_MyD3D->m_pDamageManager->Add(DT_SKILL_READY_CIRCLE, m_iPlayerIndex);
            g_MyD3D->MySparks->LightBim(vPos.x - 1.0f, vPos.y + y_Ex - .2f, 0);
        }
    }

    bool bFade = false;
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA)
    {
        int iMyPlayer = g_MyD3D->Get_MyPlayer();
        if (g_MyD3D->IsPlayerIndex(iMyPlayer) && (g_MyD3D->MyPlayer[iMyPlayer]->GetDotaStage() == GetDotaStage()))
            bFade = true;
    }
    else
        bFade = true;

    if (bFade)
    {
        g_MyD3D->MyStg->Color_r = r;
        g_MyD3D->MyStg->Color_g = g;
        g_MyD3D->MyStg->Color_b = b;
    }

    if (cFrame < 20 || Extra_Char_Num == CID_ARME2 || Extra_Char_Num == CID_ARME1 ||
        uiMotion != MID_ELESIS_SPECIAL3)
    {
        if (Extra_Char_Num != CID_ELESIS2 && Extra_Char_Num != CID_ARME2)
            if (!bNoSound)
                Ah_Sound(5);
    }

    KGCPC_MEMO("Player Special Skill");
    if (!bNoFace && g_kGlobalValue.m_bSpecialFaceShow)
    {
        Add_Face();

#if defined( DUNGEONPLAYSTATE )
        if (g_kGlobalValue.IsQuickSkillPlayer(GetCharType()))
        {
            // 던전플레이통계( 필살기 : AP는 스탑걸리면 무조건 필살기로 판단 함 )
            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
            {
                g_kGlobalValue.AddDungeonPlayState(KGCGlobal::DUNPLAY_STATE_SPECIAL);
                SendDungeonPlayState(KGCGlobal::DUNPLAY_STATE_SPECIAL);
            }
        }
#endif
    }
}

void PLAYER::StartSpecialNoZoom(BYTE r, BYTE g, BYTE b, bool bNoSound)
{
    StartSpecial(r, g, b, bNoSound, 1.0f);
}

void PLAYER::Scream()
{
    if (GetHP() > 0)
    {
        Ah_Sound(0);
    }
    else if (!Is_Aleady_Scream)
    {
        Is_Aleady_Scream = true;
        Ah_Sound(1);
        // 스톱은 각 클라이언트가 각각 수행한다.
        if (Is_IfDie2End && !g_MyD3D->Next2Stop)
        {
            g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Slow_Count = 30;
            g_MyD3D->MyHead->White_Count = 2 + SHOWLATENCY;
            g_MyD3D->Next2Stop = true;
        }
    }
}

// 없어도 되는건지 없애보고 테스트 해봅시다
void PLAYER::OnLire2Atk()
{
    //====================================================================
    // 특별한 경우를 제외하곤 석궁이외에 쓰지말것
    // Next_Attack_Count가 255가 아니면 동작이 지속되지 않음
    // 중간에 각도 변경이 가능
    // MID_LIRE2_JUMPPING_DOWN_ARROW_ATK은 연속으로 계속 쏠수 있게 만들어준다
    //====================================================================
    if (!IsLocalPlayer())
        return;

    //if(Now_Motion == MID_LIRE2_JUMPPING_DOWN_ARROW_ATK)
    //이하 한 단락은 없어도 상관이 없을 듯...
    if (uiMotion == MID_LIRE2_JUMPPING_DOWN_ARROW_ATK)
    {
        if (Next_Attack_Count != 255)
        {
            cFrame = 0;
            SetPlayerState(MID_LIRE2_JUMP);
        }
        else
        {
            Next_Attack_Count = 10;
        }
    }
    else
    {
        if (Next_Attack_Count != 255)
        {
            // Z키를 누르지 않아서 중간에 콤보 멈춤.
            cFrame = 0;
            if (uiMotion >= (UINT)(MID_LIRE2_ATK1_1) && uiMotion <= (UINT)(MID_LIRE2_ATK1_5))
                SetPlayerState(MID_LIRE2_ATK1_STOP);
            if (uiMotion >= (UINT)(MID_LIRE2_ATK2_1) && uiMotion <= (UINT)(MID_LIRE2_ATK2_5))
                SetPlayerState(MID_LIRE2_ATK2_STOP);
            if (uiMotion >= (UINT)(MID_LIRE2_ATK3_1) && uiMotion <= (UINT)(MID_LIRE2_ATK3_5))
                SetPlayerState(MID_LIRE2_ATK3_STOP);
            Next_Attack_Count = 0;
        }
        else
        {
            // Z키를 눌렀기 때문에 콤보 계속
            // Next_Attack_Count=GetCurFormTemplate().iNextAttackCount;
            g_MyD3D->MyCtrl->k_Up = false;
            g_MyD3D->MyCtrl->k_Down = false;
            int Plus = 0;
            if (uiMotion == (UINT)(MID_LIRE2_ATK1_2) || uiMotion == (UINT)(MID_LIRE2_ATK2_2) || uiMotion == (UINT)(MID_LIRE2_ATK3_2))
                Plus = 1;
            if (uiMotion == (UINT)(MID_LIRE2_ATK1_3) || uiMotion == (UINT)(MID_LIRE2_ATK2_3) || uiMotion == (UINT)(MID_LIRE2_ATK3_3))
                Plus = 2;
            if (uiMotion == (UINT)(MID_LIRE2_ATK1_4) || uiMotion == (UINT)(MID_LIRE2_ATK2_4) || uiMotion == (UINT)(MID_LIRE2_ATK3_4))
                Plus = 3;
            //if(diks[DIK_UP])
            if (diks[g_MyD3D->MyCtrl->m_keyUp])
            {
                SetPlayerState(MID_LIRE2_ATK2_1 + Plus);
            }
            //else if(diks[DIK_DOWN])
            else if (diks[g_MyD3D->MyCtrl->m_keyDown])
            {
                SetPlayerState(MID_LIRE2_ATK3_1 + Plus);
            }
            else if (g_MyD3D->MyCtrl->k_Right || g_MyD3D->MyCtrl->k_Left)
            {
                SetPlayerState(MID_LIRE2_ATK1_1 + Plus);
            }
            else
            {
                if (uiMotion >= (UINT)(MID_LIRE2_ATK1_1) && uiMotion < (UINT)(MID_LIRE2_ATK1_5))
                    SetPlayerState(MID_LIRE2_ATK1_1 + Plus);
                else if (uiMotion >= (UINT)(MID_LIRE2_ATK2_1) && uiMotion < (UINT)(MID_LIRE2_ATK2_5))
                    SetPlayerState(MID_LIRE2_ATK2_1 + Plus);
                else if (uiMotion >= (UINT)(MID_LIRE2_ATK3_1) && uiMotion < (UINT)(MID_LIRE2_ATK3_5))
                    SetPlayerState(MID_LIRE2_ATK3_1 + Plus);
            }
            g_MyD3D->MyCtrl->k_Up = false;
            g_MyD3D->MyCtrl->k_Down = false;
        }
    }
}

//--------------------------------------ActionScript.stg파일의 함수를 호출합니다--------------------------------------//
void PLAYER::FrameInfoProc(UINT uiMotion_, unsigned char Info, unsigned short Reserved1, unsigned short Reserved2, int iFrame_)
{
    PROFILE_SET("PLAYER::FrameInfoProc");

    // [1/20/2008] breadceo. 내 모션 아니면 돌릴 생각도 말어 + 잡히기는 내가 늠모션 하는걸 수도 있다..
    if (false == IsCatchMotion(uiMotion_) && false == g_pGraphicsHelper->Verify(Extra_Char_Num, uiMotion))
    {
        ASSERT(!"내 모션이 아닌데 돌릴려고?");
        return;
    }

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();

    lua_checkstack(L, 10);					// 스택 사이즈를 체크 합니다. ( 20보다 작으면 여유공간을 확보 합니다. )
    lua_pushstring(L, "Player_Action");		// Player_Action 테이블을 불러오기 위해 이름을 Push 합니다.
    lua_rawget(L, LUA_GLOBALSINDEX);			// 글로벌 테이블에서 Player_Action 키를 찾습니다.
    lua_pushinteger(L, uiMotion_);			// Player_Action의 키값을 넣습니다.
    lua_rawget(L, -2);
    if (lua_isfunction(L, -1))	// 찾은값이 Function이 아니면 뛰어 넘습니다.
    {
        lua_pushinteger(L, m_iPlayerIndex);	// 함수의 첫번째 파라메터를 Push 합니다.
        lua_pushinteger(L, iFrame_);			// 함수의 두번째 파라메터를 Push 합니다.
        if (0 != lua_pcall(L, 2, 0, 0))
        {
#if !defined(__PATH__)
            printf("%s\n", lua_tostring(L, -1));
#endif
            lua_pop(L, 1);						// 함수와 관계없는 Player_Action 테이블을 Pop 합니다.
        }
    }
    else
    {
        lua_pop(L, 1);		// 키로 찾은 값을 Pop 합니다.
        lua_pop(L, 1);		// Player_Action 테이블을 Pop 합니다.
    }


    // 얼굴표정 처리
    SiKGCFaceLift()->SetFaceLiftByMotionEnum(this, uiMotion_, iFrame_);

}

bool PLAYER::SetPlayerState(int iStateID)
{
    PROFILE_SET("PLAYER::SetPlayerState");

    // 모션 인덱스 검사
    if (iStateID < 0 || iStateID >= g_pGraphicsHelper->GetPlayerMotionNum())
        return false;
    if (iStateID == -1)
        return false;
    if (m_bBuffFrameLock)
        return false;

    if (false == VerifyRealMotion(iStateID))
    {
        if (IsContact)
            iStateID = GetWaitMotion();
        else
            iStateID = GetCurFormTemplate().JUMP;
    }

    // 탑승시 탑승물에 등록된 모션만 체인지 가능
    if (UINT_MAX > GetEmbarkUID())
    {
        bool bReturn = true;
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark)
        {
            std::vector<int> vecEmbarkMotion = SiKGCEmbarkManager()->GetCharMotionList();
            std::vector<int>::iterator vecIter = vecEmbarkMotion.begin();
            for (; vecIter != vecEmbarkMotion.end(); ++vecIter)
            {
                if ((*vecIter) == iStateID)
                {
                    bReturn = false;
                    break;
                }
            }
        }
        if (bReturn)
            return false;
    }

    // 리스트에 있는 스킬모션 사용 횟수를 체크 한다.
    if (IsLocalPlayer() && !m_mapStatsSkillMotionList.empty()) {
        std::map< int, int >::iterator mit = m_mapStatsSkillMotionList.find(iStateID);
        if (mit != m_mapStatsSkillMotionList.end()) {
            mit->second += 1;
        }
    }

    // 현재 모션이 -1인지 검사
    // assert( uiMotion != -1 );

    // 예외모션 처리
    if (m_iPlayerIndex == g_MyD3D->Get_MyPlayer() &&
        SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL /*&&
                                                                     g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN*/)
    {
        if (g_kGlobalValue.IsExceptMotion(iStateID) == true)
        {
#if !defined(__PATH__)
            MessageBoxA(NULL, "Blocked Character by Server Script!", NULL, MB_OK);
#endif
            exit(0);
        }
    }

    // 바뀌는 모션이 다음 콤보스킬모션이 아니면 콤보 초기화
    if (IsSetComboSkill())
    {
        GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, m_vecHotKeySkill[m_iComboSkillHotKeyIdx].m_eSkill)->m_pHotKeySkill;
        if (info && (false == info->m_vecComboSkill.empty()))
        {
            info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, EGCSkillTree(info->m_vecComboSkill[m_iComboSkillComboIdx].m_iSID))->m_pHotKeySkill;
            if (info && iStateID != info->m_iMotion)
            {
                m_iComboSkillHotKeyIdx = -1;
                m_iComboSkillComboIdx = -1;
            }
        }
    }

    // 이전모션이 아이스 픽시 모션이면 
    // 파티클과 데미지 붙혀주는 하드코드
    CheckIcePixie();

    // 데스매치에서 이전모션이 시작이거나 아레나면 무적걸기
    if (GetCurFormTemplate().START == Old_Motion || MID_COMMON_ARENA == Old_Motion)
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
        {
            SetSuper("", 0, 0);
        }
    }

    if (IsMagicEffect(EGC_EFFECT_CHAR_SKILL_SUPER_ARMOR)) {
        ClearMagicEffect(EGC_EFFECT_CHAR_SKILL_SUPER_ARMOR);
    }

    // 슈퍼아머와 하이퍼아머 검사( 모션체인지 없음 )
    if (CheckOption(iStateID))
        return false;

    // 모션이 바뀔때 마다 Render가 계속 true가 되버려서 
    // 아지트 격파처리시 문제가 생겼어요..ㅠㅠ 그래 아래와 같은 범죄를..ㅠㅠ
    if (false == m_bIsAgitBreaking)
        m_bRender = true;

    // 모션이 바뀌기 전 해당 모션에서 사용한 파티클 리스트를 제거한다.
    ClearMotionParticleList();

    // 프레임무브 돌릴 StateFunction변경 및 변수 초기화
    m_pCurStateFunction = ms_mapStateFunction[(MOTIONID)iStateID];
    m_bCrashCheck = true;
    m_bRenderLire3Arrow = false;
    g_MyD3D->m_KGCBlurMgr.BlurEnd();
    m_fPhysicAtkPower = 0.0f;
    Next_Motion = -1;
    Next_Direction = -1;
    m_bDrawSlashRight = false;
    m_bDrawSlashLeft = false;
    m_bDrawSlashLegRight = false;
    m_bDrawSlashLegLeft = false;
    m_bNoCrashCheck = false;
    m_bNoRecoveryMpMotion = false;
    m_bRenderChildMeshParticle = true;
    m_bSavingMotion = false;
    m_bNoUseSkill = false;
    m_iCannotCatchFrame = 0;

    if (iStateID == GetWaitMotion() || iStateID == GetFatalWaitMotion())
        SetLastKillmePlayer(MAX_PLAYER_NUM), SetLastMademeGravityPlayer(MAX_PLAYER_NUM);
    if (GetOwnMotion(MID_COMMON_INTO_THE_TELEPORT) == GetOwnMotion(iStateID) && ContactBlock > 4 && ContactBlock < 9)
        m_cTargetPortalBlock = ContactBlock - 5;
    if (IsCatchMotion(iStateID))
        Body_Angle = 0.0f;

    //공통 모션을 사용하지 않는 케릭터일 경우에 자신만의 모션을 찾아봅니다.
    unsigned int uiTempMotion = uiMotion;
    {
        uiMotion = GetSwitchMotionByDirection(GetOwnMotion(iStateID), bIsRight);
        m_bMotionChanged = true;

        if (uiMotion != uiTempMotion)
        {
            m_bUnstoppableMotion = false;
            m_bIgnoreStopMotion = false;
            m_bCheckGround = false;
            m_bSpecialSkill = false;
            m_bOffAllEffect = false;
            m_bNoUseSpecial = false;
            m_bIsCatchableMotion = false;
        }
    }

    // 모션에 따라 무기도 변경되어야하는 경우 하드코드
    if (m_bMotionChanged)
        ChangeWeaponFromMotion();

    if (IsLocalPlayer())
        g_kCamera.Follow(true);

    if (this->IsSpecialSkillMotion())
    {
        g_kMonsterManager.CreateMonsterBoolState(m_mapS1AttackState);
        g_kMonsterManager.CreateMonsterBoolState(m_mapS2AttackState);
        g_kMonsterManager.CreateMonsterBoolState(m_mapS3AttackState);
    }

    std::map< std::string, std::vector<KGCAnimManager::ItemAnim> >::iterator mit = m_mapPlayAnimation.begin();
    while (mit != m_mapPlayAnimation.end())
    {
        std::vector<KGCAnimManager::ItemAnim>& vecAnim = mit->second;
        std::vector<KGCAnimManager::ItemAnim>::iterator vit = vecAnim.begin();
        while (vit != vecAnim.end())
        {
            vit->animInst->SetShow(false);
            ++vit;
        }
        ++mit;
    }

    if (IsLocalPlayer()) {
        if (uiMotion == GetCurFormTemplate().SPECIAL1_B ||
            uiMotion == GetCurFormTemplate().SPECIAL1_W) {
            SiKGCAchieveManager()->OccurAction(SAchieve::SPECIAL_SKILL, GetCharType(), 1);
        }
        else if (uiMotion == GetCurFormTemplate().SPECIAL2_B ||
            uiMotion == GetCurFormTemplate().SPECIAL2_W) {
            SiKGCAchieveManager()->OccurAction(SAchieve::SPECIAL_SKILL, GetCharType(), 2);
        }
        else if (uiMotion == GetCurFormTemplate().SPECIAL3_B ||
            uiMotion == GetCurFormTemplate().SPECIAL3_W) {
            SiKGCAchieveManager()->OccurAction(SAchieve::SPECIAL_SKILL, GetCharType(), 3);
        }
    }

    // 제로 버프 하드코드
    if (uiMotion != MID_ZERO1_DEFENCE_WAIT &&
        uiMotion != MID_ZERO1_DEFENCE_WALK_FRONT &&
        uiMotion != MID_ZERO1_DEFENCE_WALK_BACK &&
        g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_ZERO2_DS_REFLECTION))
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_ZERO2_DS_REFLECTION);
    }

    // 에프터이미지 종료 : 잔상효과
    EndAfterImage();

    // MID통계작업
    SetStatsMID(iStateID);

    // 콤보 스킬 쿨타임 처리
    for (int iLoop = 0; iLoop < int(m_vecHotKeySkill.size()); ++iLoop)
    {
        m_vecHotKeySkill[iLoop].iStartKeyTime = 0;
        m_vecHotKeySkill[iLoop].iEndKeyTime = 0;
    }
    m_iComboSkillNowComboIdx = -1;

    CheckCombinationSkill(iStateID);
    SetNotCheckSideFH(false);

    return true;
}

void PLAYER::UpdateItemTime()
{
    PROFILE_SET("PLAYER::UpdateItemTime");

    //리플레이 중에는 리플레이 속도에 맞춰서 시간을 적당히 조절하자.
    if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
    {
        for (int i = 0; i < GC_GAME_ITEM_NUM; i++)
        {
            if (Item_Remain_Time[i] > 0)
            {
                switch (g_MyD3D->MyReplay->m_eReplay_Speed)
                {
                case RS_STOP: break;
                case RS_SLOW:
                {
                    static bool bTemp = false;
                    bTemp = !bTemp;
                    if (bTemp)
                        Item_Remain_Time[i]--;
                }
                break;
                case RS_NORMAL: Item_Remain_Time[i]--; break;
                case RS_FAST: Item_Remain_Time[i] -= 2; break;
                case RS_TOP: Item_Remain_Time[i] -= 4; break;
                }

                if (Item_Remain_Time[i] <= 0)
                {
                    switch (i)
                    {
                    case GC_GAME_ITEM_SUPER:
                        m_pObject->MeshStateOff(MS_IMMORTAL);
                        break;
                    case GC_GAME_ITEM_HALLOW:
                        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_INVISIBLE);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        return;
    }

    for (int i = 0; i < GC_GAME_ITEM_NUM; i++)
    {
        if (Item_Remain_Time[i] > 0)
        {
            Item_Remain_Time[i]--;
            if (Item_Remain_Time[i] <= 0)
            {
                switch (i)
                {
                case GC_GAME_ITEM_SUPER:
                    m_pObject->MeshStateOff(MS_IMMORTAL);
                    break;
                case GC_GAME_ITEM_HALLOW:
                    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_INVISIBLE);
                    break;
                default:
                    break;
                }
            }
        }
    }
}
void PLAYER::RecoveryHP_MP()
{
    PROFILE_SET("PLAYER::RecoveryHP_MP");

    if (!IsLocalPlayer())
        return;

    //============================================================ End HP 회복
    if (Item_Remain_Time[GC_GAME_ITEM_MPUP])
    {
        if (g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP() / 2.0f)
        {
            g_MyD3D->ChargeLocalPlayerMana(0.03f);
        }
    }

#if !defined( __PATH__ )
    if (g_kGlobalValue.m_bToggleRecoverHP)
        return;
#endif

    ////============================================================ Start HP 회복
    if (m_bIncHP == false)
        return;

    float fEncHP = 0.0f;
    if (GetHP() > 0.0f)
    {
        // 대회섭에서 자동 HP증가치를 감소시킨다.
        if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO) ||
            SiKGCChannelManager()->IsPVPBallanceServer() ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN)
        {
            fEncHP = m_fIncHPRatio * (GetCurFormTemplate().fIncHP * 0.5f);
        }
        else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
        {
            fEncHP = m_fIncHPRatio * (GetCurFormTemplate().fIncHP * 0.33f);
        }
        else
        {
            fEncHP = m_fIncHPRatio * GetCurFormTemplate().fIncHP;
        }

        float fRecoveryLimit = m_fAbility[ABILITY_AUTO_RECOVERY_HP_LIMIT_IN_DUNGEON] + m_fAbility[ABILITY_HP_RECOVERY_RATE];
        if (fRecoveryLimit > 1.0f)
            fRecoveryLimit = 1.0f;

        // 퀘스트에서 HP 증가 없음 던전이고 부스트힐이 업고 체력이 40%이상이면이다!!!
        if (SiKGCRoomManager()->IsMonsterGameMode() &&
            //Recuperação de HP DEBUFF
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_MURING_HEAL) == false &&
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_BOOST_HEAL) == false &&
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_LIME_SPECIAL2_BUFF) == false &&
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_HEAL) == false &&
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_AMY1_GODDESS_BUFF) == false &&
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_AMY3_DANCEMODE_SPECIAL3_BUFF) == false &&
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_AMY4_MELOMODE_SPECIAL3_BUFF) == false &&
            g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_AMY_ABRACADABRA_6) == false &&
            GetHP() > m_fAbility[ABILITY_HP] * (m_fAutoHpRegenLimit + fRecoveryLimit))
        {
            fEncHP = 0.0f;
        }

        // 속성 능력치 적용!
        // HP 리젠 속도 증가~  대전에서는 ABILITY_HP_RECOVERY_RATE를 회복 속도로 쓰고... 던전에서는 회복 한계치로 쓴다.. -_-;; ㅁ;ㅈㄷ룸ㅈ더ㅏㅗㄹ
#if defined( MATCH_BALANCE )
        {
            float fLevel = GetCurrentChar().iLevel;
            fEncHP = (fEncHP * (((fLevel ? fLevel : 1.f) + 10.f) / 15.f) * (1.0f + (SiKGCRoomManager()->IsMonsterGameMode() ? 0.0f : m_fAbility[ABILITY_HP_RECOVERY_RATE])));
        }
#else
        fEncHP = fEncHP * (1.0f + (SiKGCRoomManager()->IsMonsterGameMode() ? 0.0f : m_fAbility[ABILITY_HP_RECOVERY_RATE]));
#endif
    }


    if (Shield == 0)//Shield가 없을 때
    {
        if (GetHP() > 0.0f && !o10)
        {
            DWORD dwRealCheckTime = StartGameTime - 400;
            if (dwRealCheckTime > 0 && static_cast<float>(dwRealCheckTime / GC_FPS_LIMIT) < m_fAbility[ABILITY_FAST_HP_RECOVERY_DURING_LIMIT_TIME])
            {
                //#if defined(NATION_BRAZIL)  //  레나스 효과가 미비하다고 브라질 애들이 테클 걸어서 수정
                //                fEncHP += 0.4f;
                //#else
                fEncHP += 0.08f;
                //#endif
            }
        }
        // HP증가 아이템 효과
        if (Item_Remain_Time[GC_GAME_ITEM_HPUP] > 0)
        {
#ifdef LEVEL_DESIGN_STAT_CALC
            fEncHP += 8.f;

#else
            fEncHP += 0.008f;
#endif		
        }
    }
    else//Shield 상태에서의 회복
    {
        if (Item_Remain_Time[GC_GAME_ITEM_HPUP] > 0)
#ifdef LEVEL_DESIGN_STAT_CALC
            fEncHP += 8.f;

#else
            fEncHP += 0.008f;
#endif		
    }

    if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_ARME_SKILLTREE))
        fEncHP += (fEncHP * 0.02f);

    float fTempVal = 1.0f;
    if (SiGCAutoMatchManager()->IsAutoMatchBalance())
    {
        SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_HP_RECORVERY, fTempVal);
    }


    fEncHP *= fTempVal;

    SetHP(GetHP() + fEncHP);
}

void PLAYER::IncHP_inFrameMove(float _recovery /* =0 */)
{
#if !defined( __PATH__ )
    if (g_kGlobalValue.m_bToggleRecoverHP)
        return;
#endif

    float recover = 0.0f;
    float rate = (m_fAbility[ABILITY_HP] * 1.2f) / 100.f;
    m_fHP_RecoverVal += _recovery;

    if (m_fHP_RecoverVal <= 0.0f)
    {
        m_fHP_RecoverVal = 0.0f;
        return;
    }
    else if (m_fHP_RecoverVal >= rate)
        recover = rate;
    else
        recover = m_fHP_RecoverVal;

    if (GetHP() == m_fAbility[ABILITY_HP])
    {
        m_fHP_RecoverVal -= rate;
    }
    else
    {
        SetHP(GetHP() + recover);
        m_fHP_RecoverVal -= recover;
    }
}

void PLAYER::IncMP_inFrameMove(float _recovery /* =0 */)
{
    float recover = 0.0f;
    float rate = 0.036f;
    m_fMP_RecoverVal += _recovery;

    if (m_fMP_RecoverVal <= 0.0f)
    {
        m_fMP_RecoverVal = 0.0f;
        return;
    }
    else if (m_fMP_RecoverVal >= rate)
        recover = rate;
    else
        recover = m_fMP_RecoverVal;

    if (g_MyD3D->MyHead->Level >= g_kGlobalValue.GetMaxMP())
    {
        g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
        m_fMP_RecoverVal -= rate;
    }
    else
    {
        g_MyD3D->MyHead->Level += recover;
        m_fMP_RecoverVal -= recover;
    }
}

bool PLAYER::IsSkipFrameTime()
{
    // 3프레임 2번정도쉰다.
    // 조건 - 아이템 효과(슬로우모션) or
    // - 게임중에 HP없이 죽는 모션을 취하거나,
    // - 승리포즈 취하는 타이밍에 다른 포즈를 취하고 있으면
    if ((Item_Remain_Time[GC_GAME_ITEM_HASTE] > 0 ||
        (!GetHP() &&
            (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)) && !IsDetermineWin) ||
        (uiMotion != GetCurFormTemplate().WIN && IsDetermineWin)) && o3)
        return true;

    if (0 < m_iSkipFrame)
        return true;

    return false;
}

bool PLAYER::CheckStandBlock(float fX, float fY)
{
    return (IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(fX, fY) || fY <= -0.5f);
}

bool PLAYER::CheckContact()
{
    if (m_bNoSafePosition) {
        return false;
    }

    // 탑승물과 본이 연결되었다면 발판체크 하지 않는다.
    if (UINT_MAX > GetEmbarkUID())
    {
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark && pEmbark->m_pObject &&
            (-1 < GetEmbarkAdaptBoneID() && GetEmbarkAdaptBoneID() < static_cast<int>(pEmbark->m_pObject->GetNumAngleBone())))
        {
            return false;
        }
    }

    // 잡기 중일 때는 일체의 컨텍트 처리를 하지 않음
    if (IsCatchMotion(uiMotion) && cFrame < 45)
    {
        x_Speed = 0.0f;
        y_Speed = 0.0f;
        //return false; // 요거 없에서 컨택처리 하도록 하겠다!!
    }

    //Contact 검사
    Old_IsContact = IsContact;
    IsContact = false;

    D3DXVECTOR3 staticV;
    if (SiKGCRoomManager()->IsMonsterGameMode())
    {
        GCCollisionRect<float> Rect = GetCollisionRect();
        KGCContectInfo kInfo(vPos.x, vPos.y, x_Speed, y_Speed, NoCheckContact, IsTagMotion(), true, (vPos.y + m_posDesc.HeadPos.y), (y_Ex * 2.0f));
        kInfo.m_bNotCheckSideFH = m_bNotCheckSideFH;
        if (true == g_MyD3D->MyStg->Check_Contact(kInfo))
        {
            if (kInfo.m_ySpeed <= 0.0f)
            {
                if (Gravity + 0.1f > kInfo.m_y)
                {
                    IsContact = true;
                    vPos.y = kInfo.m_y;
                    kInfo.m_ySpeed = 0;
                    Gravity = 100.0f;
                }
                else if (SiKGCFantasticMap()->IsLastBottomPosition(vPos.x, vPos.y) == true)
                {
                    Gravity = 100.0f;
                }
            }
            else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME && true == SiKGCMinigameManager()->CheckContact(kInfo))
            {
                IsContact = true;
                Gravity = 100.0f;
                vPos.y = kInfo.m_y;
            }
        }
        else if (kInfo.m_cCheck)
        {
            vPos.y = kInfo.m_y;
        }

        vPos.x = kInfo.m_x;
        x_Speed = kInfo.m_xSpeed;
        y_Speed = kInfo.m_ySpeed;
    }
    else
    {
        staticV = g_MyD3D->MyStg->Check_Contact(vPos.x, vPos.y, NoCheckContact, IsTagMotion(), &m_iContactX, &m_iContactY, m_bNotCheckSideFH);
        if (staticV.y != -1.0f)
        {
            if (Gravity + 0.1f > vPos.y)
            {
                if (y_Speed <= 0 && !g_MyD3D->MyStg->m_bDeathScrollY)
                {
                    IsContact = true;
                    vPos.y = staticV.y;
                    y_Speed = 0;
                    ContactBlock = (char)staticV.z;
                    Gravity = 100.0f;
                }
            }
            else
            {
                if (g_MyD3D->MyStg->IsLowStandBlock(vPos.x, vPos.y))
                    Gravity = 100.0f;
            }
        }
    }
    //방금 땅에서 발이 떨어졌음. 적어도 10프레임은 유지할것임
    // 데미지에서 nocheckcontact 간섭을 했으면 이거 건드리면 안됨
    if (!IsContact && Old_IsContact && m_bDamageSetNoCheckContact == false)
    {
        NoCheckContact = 10;
    }
    else if (IsContact && !Old_IsContact)//막 착지 했음. 캐릭터별로 적절한 포즈를 취해줌
    {
        // 공중에 떠있는 경우, 먼지가 생기면 이상하기 때문에...요런 처리를...
        // + 현재 모션이 WAIT 계열이면 먼지 안하기
        if (y_Ex < 0.35f && (Old_IsContact == false && IsContact == true) &&
            (uiMotion != GetWaitMotion() && uiMotion != GetFatalWaitMotion() && uiMotion != GetOwnMotion(MID_COMMON_INTO_THE_TELEPORT)))
        {
            EffectDust();
            Sound(0);
        }

        //착지 모션들
        if (uiMotion == GetOwnMotion(MID_COMMON_DAMAGED_BY_FLYATK))
        {
            // 공중에 떳다가 떨어졌을 때 실드가 있고.. 그 실드를 깰만큼 데미지를 받지 않으면
            // 실드로 감싸진 채로 착지 되지 않고 그냥 멈춰버린다.. 그래서 실드가 있는 상태라면..
            // 실드를 최소값으로 설정해 무조건 깨지게 해주겠다..
            if (Shield > 0)
                Shield = 1;

            //로난도 쉴드있당..... 이거 뭔가.. 안좋은 방법 같은데.... 데미지 맞기 전 연산에만 있어서.... 이럴수 밖에 ㅠ,.ㅠ 시간나면 로난 쉴드 자체를 수정 해야겠삼.
            if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_HOLY_LV3_SHIELD))
            {
                g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_HOLY_LV3_SHIELD);
            }
            // 펫도 실드 있다 
            if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_RENCIA_SHIELD))
            {
                g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RENCIA_SHIELD);
            }
            else if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_CHOUCRING_SHIELD))
            {
                g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_CHOUCRING_SHIELD);
            }
            else if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_DOKKAEBIE_SHIELD))
            {
                g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DOKKAEBIE_SHIELD);
            }
            else if (GetLastMademeGravityPlayer() >= 0 && GetLastMademeGravityPlayer() < MAX_PLAYER_NUM) {
                DamageInfo damage;
#ifdef LEVEL_DESIGN_STAT_CALC
                damage.SetDamage(-Last_Gravity * 1000.f);
#else
                damage.SetDamage(-Last_Gravity * 10.f);
#endif		
                damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, GetLastMademeGravityPlayer());
                Change_HP(damage);
            }
            else {                                                                                        // 캐릭별 데미지 factor 적용X
                DamageInfo damage;
#ifdef LEVEL_DESIGN_STAT_CALC
                damage.SetDamage(-Last_Gravity * 1000.f);
#else
                damage.SetDamage(-Last_Gravity * 10.f);
#endif			
                damage.SetAttacker(DamageInfo::DAMAGE_MONSTER, GetLastMademeGravityPlayer() - MAX_PLAYER_NUM);
                damage.SetDamageFactor(false);
                Change_HP(damage);
            }

            Last_Gravity = 0.0f;

            Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP);
            //Super = 25; //떨어지면서 공격 받았을때..맞지 않도록함
        }
        else if (uiMotion == GetOwnMotion(MID_COMMON_DOWN_IN_SKY))
        {
            cFrame = 0;
            SetPlayerState(MID_COMMON_DOWN_AND_STANDUP);
            Sound(49);
            //Super = 25; //떨어지면서 공격 받았을때..맞지 않도록함
        }
        else if (uiMotion == GetOwnMotion(MID_COMMON_ARENA))
        {
            SetPlayerState(GetOwnMotion(MID_COMMON_DROP));
            cFrame = 0;
            Super = 55;
            g_kCamera.SetZoom(.75f, g_MyD3D->Get_MyPlayer());
        }
        else if (uiMotion == MID_LIRE2_JUMPPING_DOWN_ARROW_ATK)
        {
            SetPlayerState(MID_LIRE2_LAND_IN_DOWN_ARROW_ATK);
            cFrame = 0;
            Sound(0);
        }
        else if (uiMotion == MID_LAS_DOUBLEJUMP)
        {
            SetPlayerState(MID_LAS_DOUBLEJUMP_LAND);
            cFrame = 0;
        }
        else if (uiMotion == MID_ELESIS3_JUMPATK_DOWN && cFrame > 15)
        {
            SetPlayerState(MID_ELESIS3_JUMPATK_DOWN_LAND);
            cFrame = 0;
        }
        else if (uiMotion == MID_LAS3_UP_JUMPDASH ||
            uiMotion == MID_LAS3_FRONT_JUMPDASH ||
            uiMotion == MID_LAS3_DOWN_JUMPDASH)
        {
            SetPlayerState(MID_LAS3_LANDING);
            cFrame = 0;
        }
        else if (uiMotion == MID_RYAN_JUMP_ATK2)
        {
            SetPlayerState(MID_RYAN_JUMP_ATK2_LAND);
            cFrame = 0;
        }
        else if (uiMotion == MID_FORM_WOLF_JUMP_ATK2)
        {
            SetPlayerState(MID_FORM_WOLF_JUMP_ATK2_LAND);
            cFrame = 0;
        }
        else if (uiMotion == MID_FORM_WOLF_JUMP || uiMotion == MID_FORM_WOLF_JUMP_ATK2 || uiMotion == MID_FORM_WOLF_JUMP_ATK1)
        {
            // Player Template 값이 바껴버리면 끼인다.
            switch (MetamorphosisForm)
            {
            case FORM_NORMAL:
                SetPlayerState(MID_RYAN_WAIT);
                break;
            case FORM_WOLF:
                SetPlayerState(MID_FORM_WOLF_WAIT);
                break;
            }

            cFrame = 0;
        }
        else if (uiMotion == MID_NEPHILLIM_JUMP_ATK1 ||
            uiMotion == MID_NEPHILLIM_JUMP_ATK2 ||
            uiMotion == MID_NEPHILLIM_JUMP_ATK4 ||
            uiMotion == MID_NEPHILLIM_JUMP_DASH_ATK)
        {
            // Player Template 값이 바껴버리면 끼인다.
            switch (MetamorphosisForm)
            {
            case FORM_NORMAL:
                SetPlayerState(MID_RYAN2_WAIT);
                break;
            case FORM_NEPHILIM:
                SetPlayerState(MID_NEPHILLIM_WAIT);
                break;
            }

            cFrame = 0;
        }
        else if (uiMotion == MID_RYAN2_JUMP_ATK2 && cFrame < 41)
        {
            cFrame = 41;
            x_Speed = 0.0f;
        }
        else if (uiMotion == MID_NEPHILLIM_JUMP)
        {
            SetPlayerState(MID_NEPHILLIM_LAND);
            cFrame = 0;
        }
        else if (uiMotion == MID_NEPHILLIM_JUMP_ATK3)
        {
            // Player Template 값이 바껴버리면 끼인다.
            switch (MetamorphosisForm)
            {
            case FORM_NORMAL:
                SetPlayerState(MID_RYAN2_WAIT);
                break;
            case FORM_NEPHILIM:
                cFrame = 24;
                break;
            }
        }
        else if (uiMotion == MID_ELESIS4_JUMP_ATK3)
        {
            SetPlayerState(MID_ELESIS4_JUMP_ATK_LAND);
            cFrame = 0;
        }
        else if (uiMotion == MID_RYAN4_MDAN_JUMP_ATK)
        {
            // Player Template 값이 바껴버리면 끼인다.
            switch (MetamorphosisForm)
            {
            case FORM_NORMAL:
                SetPlayerState(MID_RYAN2_WAIT);
                break;
            case FORM_MDAN:
                SetPlayerState(MID_RYAN4_MDAN_WAIT);
                break;
            }

            cFrame = 0;
        }
        else if (uiMotion == MID_JIN2_JUMP_DOWN_ATK)
        {
            SetPlayerState(MID_JIN2_JUMP_DOWN_ATK_TO_WAIT);
            cFrame = 0;
        }
        //착지했으므로 기본자세로 돌려야 하는 모션들
        else if (IsReturnToWaitMotion())
        {
            if (GetHP() > 0.f)
                SetPlayerState(GetWaitMotion());
            else
                SetPlayerState(GetFatalWaitMotion());
            cFrame = 0;
            x_Speed = 0.0f;
        }

        if (iContact_to_Stone != -1)
        {
            g_Push_Count = 0;
            SetPlayerState(MID_COMMON_STONECURSE);
            SetLastKillmePlayer(iContact_to_Stone);
            cFrame = 0;
            bIsRight = true;
            x_Speed = 0;
            iContact_to_Stone = -1;
        }

        m_fPhysicAtkPower = 0.0f;
        m_iJumpActionLimitCount = 0;
        m_fFall_Speed = DEAULT_FALL_SPEED;

        if (IsLocalPlayer())
        {
            g_Hot_Count = 0;
        }
    }

    if (NoCheckContact > 0)
        NoCheckContact--;

    m_bDamageSetNoCheckContact = false;

    if (SiKGCRoomManager()->IsMonsterGameMode() == false)
    {
        // 안에서 자동으로 됨
        // 벽과의 충돌검사를 수행한다.
        if (ContactBlock == 3)// 벽에 걸렸음...
        {
            if (IsContact)
            {
                if (vPos.x < staticV.x + 0.0245f)
                {
                    vPos.x = staticV.x + 0.025f;
                    x_Speed = 0;
                }
            }
        }
        else if (ContactBlock == 2)
        {
            if (IsContact)
            {
                if (vPos.x > staticV.x + 0.0255f)
                {
                    vPos.x = staticV.x + 0.025f;
                    x_Speed = 0;
                }
            }
        }
    }

    // 쓰러지는 동작인데...IsContact는 false라면...
    if (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) && !IsContact)
    {
        Slow_Count = 0;
        SetPlayerState(MID_COMMON_DOWN_IN_SKY);
        cFrame = 0;
    }
    return true;
}

bool PLAYER::ProcessSlowCount()
{
    //==============================================================================
    // 죽음과 아레나 처리 - Slow Count가 적용되면 더이상의 처리는 없다.
    //==============================================================================
    if (g_kGlobalValue.m_bFrameLock && o3 != 0)
        return false;

    // 모든 캐릭터에게 Slow Count가 적용된다.
    if (Slow_Count > 0)
    {
        // 필살기 스탑모션일 때 커맨드 입력받는 함수 호출하자.
        InputSpecialSkillPuzzleKey();

        Slow_Count--;
        // 1마리가 죽었음...

        // 연습모드일때는 마릿수에 변화가 없음.
        if (Is_IfDie2End)
        {
            if (Slow_Count > 90 && (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)) && !GetHP())
            {
                if (m_cLife > 0)
                {
                    m_cLife--;
                    UpdateTagCharInfo();
                }
            }
        }
        else
        {
            if (Slow_Count == 30 && (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)) && !GetHP())
            {
                if (m_cLife > 0)
                {
                    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON || g_kGlobalValue.m_bMUHANLife == false)
                        m_cLife--;
                    UpdateTagCharInfo();
                }

                if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
                {
                    if (GetNextTagChar() != GC_CHAR_INVALID && !IsDetermineWin)
                    {
                        Arena();
                    }
                }
                else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH)
                {
                    //타이밍 봐서 아레나 해야 한다.
                    if (!IsDetermineWin)
                    {
                        m_iArenaCoolTime = ARENA_COOL_TIME_DEATH;
                        return false;
                    }
                    else
                    {
                        m_cLife = 0;
                    }
                }
                else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
                {
                    //타이밍 봐서 아레나 해야 한다.
                    if (!IsDetermineWin)
                    {
                        m_iArenaCoolTime = ARENA_COOL_TIME_FATAL_DEATH;
                        return false;
                    }
                    else
                    {
                        m_cLife = 0;
                    }
                }
                else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG
                    || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN)
                {
                    //타이밍 봐서 아레나 해야 한다.
                    if (!IsDetermineWin)
                    {
                        m_iArenaCoolTime = ARENA_COOL_TIME_EX;
                        return false;
                    }
                    else
                    {
                        m_cLife = 0;
                    }
                }
                else
                {
                    if (m_cLife > 0 && !IsDetermineWin)
                    {
                        Arena();
                    }
                }
            }
        }
        return false;
    }
    return true;
}

void PLAYER::ArmeChageBlackNWhite()
{
    g_MyD3D->MyHead->IsBlackMagic = !g_MyD3D->MyHead->IsBlackMagic;
}

void PLAYER::UseHealingBall()
{
    int MyPlayerNo = g_MyD3D->Get_MyPlayer();

    bool bNeedHeal = false;

    if (true == g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[MyPlayerNo]->GetCurrentChar(), ITEM_ID_HEALBALL))
    {
        bNeedHeal = true;
    }
    if (true == g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[MyPlayerNo]->GetCurrentChar(), ITEM_ID_HEALBALL2))
    {
        bNeedHeal = true;
    }
    if (false == bNeedHeal && g_pItemMgr->DecDurationItemForItemID(&g_MyD3D->MyPlayer[MyPlayerNo]->GetCurrentChar(), ITEM_ID_HEALBALL_QUESTPACKAGE))
    {
        bNeedHeal = true;
    }
    if (true == g_pItemMgr->CheckEquipItemForItemID(&g_MyD3D->MyPlayer[MyPlayerNo]->GetCurrentChar(), ITEM_ID_USEFUL_WATCH))
    {
        bNeedHeal = true;
    }

    if (true == bNeedHeal && false == m_bIsOnceHeal)
    {
        m_bIsOnceHeal = true;
        DamageInfo damage;
        damage.SetDamage(g_MyD3D->MyPlayer[MyPlayerNo]->m_fAbility[ABILITY_HP] * 0.3f);
        damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, MyPlayerNo);
        damage.SetAbilityEffect(false, false);
        Change_HP(damage);

        LTP_MAGICEFF_SYNCH EffPacket;
        ZeroMemory(&EffPacket, sizeof(EffPacket));
        EffPacket.packetType = ELTP_MAGICEFF;
        EffPacket.size = sizeof(EffPacket);
        EffPacket.latencyTime = GC_ELAPSED_TIME * 10.0f;
        EffPacket.iMagicEffType = 0;
        SETVECTOR3(EffPacket.vecPos, vPos.x, vPos.y, 0.0f);
        g_LatencyTransfer.PushPacket(&EffPacket);
    }
}

bool PLAYER::IsCanUseHealingBall(int iWhatWho)
{
#ifndef NATION_TAIWAN
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
        return false;
#endif

    if (GetHP() != 0.0f)
        return false;

    //카제아제로 부터 온 힐링도 한번만 적용 되도록 한다(맨티스0322272), 그래서 아래 if( HEALING_DEFAULT == iWhatWho )는 주석처리함
    //if( HEALING_DEFAULT == iWhatWho )
    //{
    // Healing Pet은 한번만 사용되도록
    if (m_bIsOnceHeal)
        return false;
    //}

    if (true == g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEM_ID_HEALBALL))
        return true;
    if (true == g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEM_ID_HEALBALL2))
        return true;
    if (true == g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEM_ID_HEALBALL_QUESTPACKAGE))
        return true;
    if (true == g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEM_ID_USEFUL_WATCH))
        return true;

    return false;
}

void PLAYER::GetProtectRingInfo(int& iItemID, int& iDuration)
{
    KItem* kInvenItem = NULL;

    // PC방 아이템을 가장 먼저 소모하게 만든다.
    if (!g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEMID_PROTECT_RING_PC, kInvenItem))
    {
        if (!g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEMID_PROTECT_RING_10, kInvenItem))
        {
            if (!g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEMID_PROTECT_RING, kInvenItem))
            {
                if (!g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEMID_PROTECT_RING_QUESTPACKAGE, kInvenItem))
                {
                    if (!g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEMID_PROTECT_RING_SINGLE, kInvenItem))
                    {

                        iItemID = -1;
                        iDuration = 0;
                        return;
                    }
                }
            }
        }
    }

    iItemID = kInvenItem->m_ItemID;
    iDuration = kInvenItem->m_nCount;
}

void PLAYER::GetHealingBallInfo(int& iItemID, int& iDuration)
{
    KItem* kInvenItem = NULL;
    if (!g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEM_ID_HEALBALL, kInvenItem)
        && !g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEM_ID_HEALBALL2, kInvenItem)
        && !g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ITEM_ID_HEALBALL_QUESTPACKAGE, kInvenItem))
    {
        iItemID = -1;
        iDuration = 0;
        return;
    }
    iItemID = kInvenItem->m_ItemID;
    iDuration = kInvenItem->m_nCount;
}

int PLAYER::GetSpecialMotionID(int iSpecial, int* outiDuration)
{
    int MotionID = 0;
    int iUseSkillIndex = -1;

    if (g_kGlobalValue.IsCharUseSpecial4(GetCurrentChar().iCharType)) {
        if (m_iSkillType != 1) {
            *outiDuration = -1;
            return MotionID;
        }
        for (std::vector< GCHotKeySkillInfo >::iterator vit = m_vecPossibleChargingSkillInfo.begin(); vit != m_vecPossibleChargingSkillInfo.end(); ++vit) {
            if (vit->m_iChargingSkillLV == iSpecial) {
                iUseSkillIndex = vit->m_iChargingSkillLV;
                if (g_MyD3D->MyHead->IsBlackMagic == false) {
                    iUseSkillIndex += 3;
                }
                MotionID = vit->m_iMotion;
                break;
            }
        }
    }
    else {
        if (iSpecial == 3)
        {
            if (g_MyD3D->MyHead->IsBlackMagic && GetCurFormTemplate().SPECIAL3_B != -1)
            {
                iUseSkillIndex = 5;
                MotionID = GetCurFormTemplate().SPECIAL3_B;
            }
            else
            {
                iUseSkillIndex = 2;
                MotionID = GetCurFormTemplate().SPECIAL3_W;
            }
        }

        if (iSpecial == 2)
        {
            if (g_MyD3D->MyHead->IsBlackMagic && GetCurFormTemplate().SPECIAL2_B != -1)
            {
                iUseSkillIndex = 4;
                MotionID = GetCurFormTemplate().SPECIAL2_B;
            }
            else
            {
                iUseSkillIndex = 1;
                MotionID = GetCurFormTemplate().SPECIAL2_W;
            }
        }

        if (iSpecial == 1)
        {
            if (g_MyD3D->MyHead->IsBlackMagic && GetCurFormTemplate().SPECIAL1_B != -1)
            {
                iUseSkillIndex = 3;
                MotionID = GetCurFormTemplate().SPECIAL1_B;
            }
            else
            {
                iUseSkillIndex = 0;
                MotionID = GetCurFormTemplate().SPECIAL1_W;
            }
        }
    }

    if (outiDuration != NULL)
    {
        if (iUseSkillIndex != -1)
        {
            KItem* kInvenItem = NULL;

            if (0 == m_mapwSkillItem.count(std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, (DWORD)iUseSkillIndex)))
            {
                *outiDuration = -1;
            }
            else
            {
                if (m_mapwSkillItem.count(std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, (DWORD)iUseSkillIndex)))
                {
                    if (g_pItemMgr->FindInventory(m_mapwSkillItem[std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, iUseSkillIndex)], &kInvenItem))
                    {
                        *outiDuration = kInvenItem->m_nCount;
                    }
                }
            }
        }
    }

    return MotionID;
}


bool PLAYER::IsSpecialWhite(int iLevel_)
{
    // 버닝 필살기를 썼니? ( 버닝 게이지 모아서 쓰는것.. 버닝 모드일때 아니고 ㅠㅠ )
    m_bBurnningSpecial = false;
    bool bWhite = true;
    int iBlackSpecial = -1;


    switch (iLevel_)
    {
    case 1:
        iBlackSpecial = GetCurFormTemplate().SPECIAL1_B;
        break;
    case 2:
        iBlackSpecial = GetCurFormTemplate().SPECIAL2_B;
        break;
    case 3:
        iBlackSpecial = GetCurFormTemplate().SPECIAL3_B;
        break;
    default:
        iBlackSpecial = -1;
        break;
    }

    if (g_MyD3D->MyHead->IsBlackMagic && iBlackSpecial != -1)
    {
        bWhite = false;
        // 버닝이 있는 아이다.
        if ( GetCurFormTemplate().BURNNING_ACTIVE != -1 )
        {
            m_bBurnningSpecial = true;
            if (m_fBurnningPoint < GetCurFormTemplate().BurningCostForSpecial || m_bIsBurnningMode)
            {
                bWhite = true;
                m_bBurnningSpecial = false;
            }
        }
    }

    return bWhite;
}

bool PLAYER::UseSpecialForHybridCharacter(int& iSkillIndex_, int iShortCutLv_)
{
    if (m_bNoUseSkill) {
        return false;
    }

    if (m_vecPossibleChargingSkillInfo.empty()) {
        return false;
    }

    if (uiMotion == GetCurFormTemplate().SAVING_STANCE ||
        uiMotion == GetCurFormTemplate().SAVING_ATTACK ||
        uiMotion == GetCurFormTemplate().SAVING_EVASION) {
        return false;
    }

    if (iShortCutLv_ != -1) {
        int iSkillID = GetCurFormTemplate().SID_SPECIAL4;
        const  GCSkill* pSkill = SiGCSKT()->GetSkill(static_cast<EGCSkillTree>(iSkillID));

        if (pSkill && pSkill->m_pHotKeySkill) {
            if (SetPlayerState(pSkill->m_pHotKeySkill->m_iMotion)) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    else {
        for (std::vector< GCHotKeySkillInfo >::iterator vit = m_vecPossibleChargingSkillInfo.begin(); vit != m_vecPossibleChargingSkillInfo.end(); ++vit) {
            float fMana = 0.f;
            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
                fMana = vit->m_fMonsterModeMana;
            }
            else {
                fMana = vit->m_fMana;
            }

            if (vit->m_bSpecialSkill) {
                fMana *= m_fMPDecreaseRatioForSpecial;
            }

            if (g_MyD3D->MyCtrl->Energy > fMana) {
                // 버닝 체크해서 조건에 만족되면 버닝포인트 깎아주고, 모션도 바꿔주자.

                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                // 진짜 각성 들가면 이 부분 버린다!!
                if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL) {
                    std::map< std::pair< int, int >, std::vector< SCashSkillInfo > >::iterator mitCashSkill;
                    mitCashSkill = m_mapCashSkillInfo.find(std::make_pair(Extra_Char_Num % GC_CHAR_NUM, Extra_Char_Num / GC_CHAR_NUM));
                    if (mitCashSkill != m_mapCashSkillInfo.end()) {
                        for (int i = 0; i < static_cast<int>(mitCashSkill->second.size()); i++) {
                            const GCSkill* pSkill = SiGCSKT()->GetSkill((EGCSkillTree)mitCashSkill->second[i].m_iSkillID);
                            if (pSkill && pSkill->m_pHotKeySkill) {
                                if (pSkill->m_pHotKeySkill->m_iMotion == vit->m_iMotion) {
                                    int iValidCheck = -1;
                                    GetSpecialMotionID(vit->m_iChargingSkillLV, &iValidCheck);
                                    if (iValidCheck < 1) {
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }

                iSkillIndex_ = vit->m_iChargingSkillLV;
                if (g_MyD3D->MyHead->IsBlackMagic == false) {
                    iSkillIndex_ += 3;
                }
                /////////////////////////////////////////////////////////////////////////////////////////////////////////

                int iMotion = vit->m_iMotion;
                if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
                    iMotion = vit->m_iMonsterModeMotion;
                }
                if (GetCurFormTemplate().BURNNING_ACTIVE != -1 && GetCurFormTemplate().BURNNING_CHARGE != -1) {
                    if (m_fBurnningPoint >= GetCurFormTemplate().BurningCostForSpecial || m_bIsBurnningMode) {
                        iMotion = vit->m_iAddMotion;
                    }

                    if (m_bIsBurnningMode == false) {
                        m_fBurnningPoint -= GetCurFormTemplate().BurningCostForSpecial;

                        if (m_fBurnningPoint < 0.0f) {
                            m_fBurnningPoint = 0.0f;
                        }
                    }
                }
                else if (GetCurFormTemplate().BURNNING_ACTIVE != -1 && GetCurFormTemplate().BURNNING_CHARGE == -1) {
                    if (m_bIsBurnningMode) {
                        iMotion = vit->m_iAddMotion;
                    }
                }

                if (uiMotion == GetOwnMotion(MID_SKILL_ARME1_B1_BLOODPEST))
                    return false;

                if (SetPlayerState(iMotion)) {
                    g_MyD3D->MyHead->Level -= fMana;
                    // 차징으로 필살기 사용 카운트 저장
                    g_kGlobalValue.m_prHybridSpecialUseCount.second++;
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    }

    return false;
}

bool PLAYER::IsUsableStateSpecial(int iUseShortCutLv, float fLimit_)
{
    if (iUseShortCutLv == -1) {
        if (g_MyD3D->MyCtrl->Energy > fLimit_) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (g_MyD3D->MyHead->Level > fLimit_) {
            if (iUseShortCutLv == static_cast<int>(round(fLimit_))) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
}

void PLAYER::UseSpecial(int iUseShortCutLv)
{
    if (m_bNoUseSkill) {
        return;
    }

    if (!IsSpecialUsable())
        return;

    int iUseSkillIndex = -1; //m_kSkillItem[]에서의 인덱스

    bool bIsUsedSpecial = false;

    int iUsedSpecialLevel = 0;

    if (g_kGlobalValue.IsHybridPlayer(Extra_Char_Num) ||
        g_kGlobalValue.IsCharUseSpecial4(Extra_Char_Num)) {
        bIsUsedSpecial = UseSpecialForHybridCharacter(iUseSkillIndex, iUseShortCutLv);
    }
    else {
        if (IsUsableStateSpecial(iUseShortCutLv, 2.7f)) {
            if (!IsSpecialWhite(3)) {
                m_fBurnningPoint -= GetCurFormTemplate().BurningCostForSpecial;
                bIsUsedSpecial = SetPlayerState(GetCurFormTemplate().SPECIAL3_B);
                iUseSkillIndex = 5;
            }
            else {
                bIsUsedSpecial = SetPlayerState(GetCurFormTemplate().SPECIAL3_W);
                iUseSkillIndex = 2;
            }

            if (bIsUsedSpecial) {
                if (m_fMPDecreaseRatioForSpecial < 1.f) {
                    g_MyD3D->MyHead->Level -= 3.f * m_fMPDecreaseRatioForSpecial;
                }
                else {
                    g_MyD3D->MyHead->Level = 0;
                }
            }

            iUsedSpecialLevel = 3;
        }

        if (IsUsableStateSpecial(iUseShortCutLv, 1.9f) && !bIsUsedSpecial)
        {
            if (!IsSpecialWhite(2))
            {
                m_fBurnningPoint -= GetCurFormTemplate().BurningCostForSpecial;
                bIsUsedSpecial = SetPlayerState(GetCurFormTemplate().SPECIAL2_B);
                iUseSkillIndex = 4;
            }
            else
            {
                if (GetCurFormTemplate().SPECIAL2_W == MID_LAS_SPECIAL2)
                {
                    if ((rand() % 2) == 0)
                        bIsUsedSpecial = SetPlayerState(MID_LAS_SPECIAL2_FAIL);
                    else
                        bIsUsedSpecial = SetPlayerState(MID_LAS_SPECIAL2);
                }
                else
                {
                    bIsUsedSpecial = SetPlayerState(GetCurFormTemplate().SPECIAL2_W);
                }

                iUseSkillIndex = 1;
            }

            if (bIsUsedSpecial)
                g_MyD3D->MyHead->Level -= 1.9f * m_fMPDecreaseRatioForSpecial;

            iUsedSpecialLevel = 2;
        }

        if (IsUsableStateSpecial(iUseShortCutLv, 0.9f) && !bIsUsedSpecial)
        {
            if (uiMotion == GetOwnMotion(MID_SKILL_ARME1_B1_BLOODPEST))
                return;

            if (!IsSpecialWhite(1))
            {
                m_fBurnningPoint -= GetCurFormTemplate().BurningCostForSpecial;
                bIsUsedSpecial = SetPlayerState(GetCurFormTemplate().SPECIAL1_B);
                iUseSkillIndex = 3;
            }
            else
            {
                bIsUsedSpecial = SetPlayerState(GetCurFormTemplate().SPECIAL1_W);
                iUseSkillIndex = 0;
            }

            if (bIsUsedSpecial)
                g_MyD3D->MyHead->Level -= 0.9f * m_fMPDecreaseRatioForSpecial;

            //특수효과 추가........ Info로 빼내야 할듯... 우선은 기본 기술 1단계일 때만 화살표 보이게 한다.
            if (Extra_Char_Num == CID_LIRE2 && uiMotion == MID_LIRE2_SPECIAL1_DOWN)
            {
                //g_MyD3D->MySparks->AddArrow(x-1.0f, y-.2f);
                g_MyD3D->MySparks->Add(ARROW, vPos.x - 1.0f, vPos.y - 0.2f, 0, 1.0f);
            }

            iUsedSpecialLevel = 1;
        }
    }

    if (m_fBurnningPoint < 0.0f)
        m_fBurnningPoint = 0.0f;



    if (bIsUsedSpecial)
    {
        Reserve_Motion = 0;
        Next_Attack_Count = 0;
        Old_Frame = cFrame = 0;
        SiKGCSubjectManager()->AddSpecial();

        // 몸 방향을 바꾸어 쓰는 필살기들은 여기서 Body_Angle을 0로 만들면 안된다.
        if (uiMotion != MID_LIRE_SPECIAL1 && uiMotion != MID_LIRE_EXPLOSION_ARROW
            && uiMotion != MID_SKILL_LIRE_METALSHOT_LV3
            && uiMotion != MID_SKILL_LIRE_FIRE_ARROW_LV1 && uiMotion != MID_SKILL_LIRE_FROZEN_ARROW
            && uiMotion != MID_SKILL_LIRE_POISON_ARROW && uiMotion != MID_SKILL_LIRE_HEADSHOT_LV1
            && uiMotion != MID_SKILL_LIRE_HEADSHOT_LV2 && uiMotion != MID_SKILL_LIRE_CONTINUOUS_ATK_LV1
            && uiMotion != MID_SKILL_LIRE_CONTINUOUS_ATK_LV2 && uiMotion != MID_SKILL_LIRE_FIRE_ARROW_LV2
            && uiMotion != MID_SKILL_LIRE_METALSHOT_LV2 && uiMotion != MID_SKILL_LIRE_PIERCESHOT_LV1
            && uiMotion != MID_SKILL_LIRE_PIERCESHOT_LV2)
            Body_Angle = 0;

        if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
        {
            if (m_iSkillType == 1 && m_mapwSkillItem.count(std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, (DWORD)iUseSkillIndex)))//수량성 기술을 사용한것임
            {
                g_pItemMgr->DecDurationItem(m_mapwSkillItem[std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, iUseSkillIndex)]);
            }

            if (g_kGlobalValue.IsCharUseSpecial4(GetCurrentChar().iCharType) == false) {
                SetSkillType(m_iSkillType);
            }
        }

#if defined( DUNGEONPLAYSTATE )
        // 던전플레이통계( 필살기 )
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        {
            g_kGlobalValue.AddDungeonPlayState(KGCGlobal::DUNPLAY_STATE_SPECIAL);
            SendDungeonPlayState(KGCGlobal::DUNPLAY_STATE_SPECIAL);
        }
#endif

#if defined( COLLECT_SKILL_HOTKEY_USED_INFO )
        if (iUseShortCutLv == -1) {
            g_kGlobalValue.SetSSkillHotKeyUsedInfo(false, iUsedSpecialLevel);
        }
        else {
            g_kGlobalValue.SetSSkillHotKeyUsedInfo(true, iUsedSpecialLevel);
        }
#endif

        // 화령의 축복 버프 기간이라면 회복버프
        if (SiGCBuffDonationManager()->IsBuffOn())
            SiGCBuffDonationManager()->FullHPMP();
    }

    g_MyD3D->MyCtrl->Energy = 0;

    return;
}

int PLAYER::CheckComboSkill(EGCSkillTree eSkill)
{
    int iResult = -1;

    // 핫키 정보얻기
    GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, eSkill)->m_pHotKeySkill;

    // 콤보가 있는지 확인
    if (info && (false == info->m_vecComboSkill.empty()))
    {
        // 어떤 스킬인지 찾기
        int iKeyCount = g_MyD3D->MyCtrl->m_vecKeyCnt[(g_MyD3D->MyCtrl->k_FreshSkillNum - 1)];
        if (iKeyCount < int(info->m_vecComboSkill.size()))
        {
            iResult = iKeyCount;
        }
        else
        {
            iResult = int(info->m_vecComboSkill.size()) - 1;
        }
    }
    return iResult;
}

void PLAYER::UseGameItem(bool bNoSound)
{
    if (g_MyD3D->MyItms->GetSelectedItem() > -1)
    {
        if (false == bNoSound)
            Sound(10); // 아이템 소리
        g_MyD3D->MyItms->Use_Item(m_iPlayerIndex, g_MyD3D->MyItms->GetSelectedItem());
        g_MyD3D->MyItms->Delete_Item();
    }
}

void PLAYER::CommonKeyInput()
{
    if (g_MyD3D->MyHead->IsCheatMode)
        return;

    //죄송...나도 이럴수 밖에 없더라구..
    g_pkGameOverlayUI->EnableHealMessage(IsCanUseHealingBall());
    g_MyD3D->MyCtrl->CheckChange();

    if (CheckItemKey())
        return;

    CheckSkillKey();
    CheckPetKey();
    CheckGameItemKey();
    CheckJokeKey();
    CheckShortCutItemKey();
    CheckCameraZoomKey();
}

bool PLAYER::CheckItemKey()
{
    if (false == SiKGCWorldMapManager()->IsUseGameItem(SiKGCRoomManager()->GetGameMode()))
        return false;

    if (g_MyD3D->MyCtrl->k_Use_Item)
    {
        if (GC_GMC_TAG_MATCH == SiKGCRoomManager()->GetGameModeCategory())
        {
            SetTagMotion();
            //TagCharacter();
        }
        else if (GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory())
        {
            if (false == IsDie() && g_MyD3D->MyItms->GetSelectedItem() > -1)
            {
                Sound(10); // 아이템 소리
                g_MyD3D->MyItms->Use_Item(m_iPlayerIndex, g_MyD3D->MyItms->GetSelectedItem());
                g_MyD3D->MyItms->Delete_Item();
            }
        }
        else
        {
            UseGameItem();
        }
        return true;
    }

    return false;
}

void PLAYER::CheckSkillKey()
{
    if (UINT_MAX > GetEmbarkUID())
        return;

    if (g_MyD3D->MyCtrl->k_FreshSkillNum && g_MyD3D->MyCtrl->k_FreshSkillNum <= (int)m_vecHotKeySkill.size())
    {
        bool bUseSkill = true;
        GCHotKeySkill skill = m_vecHotKeySkill[g_MyD3D->MyCtrl->k_FreshSkillNum - 1];
        int iComboIdx = CheckComboSkill(skill.m_eSkill);
        if (0 < iComboIdx)
        {
            bUseSkill = false;
            if ((m_iComboSkillNowComboIdx != iComboIdx) &&
                (skill.iStartKeyTime <= cFrame && cFrame <= skill.iEndKeyTime) &&
                (-1 == m_iComboSkillHotKeyIdx && -1 == m_iComboSkillComboIdx))
            {
                m_iComboSkillHotKeyIdx = g_MyD3D->MyCtrl->k_FreshSkillNum - 1;
                m_iComboSkillComboIdx = iComboIdx;
            }
        }

        if (bUseSkill)
        {
            UseSkill(&m_vecHotKeySkill[g_MyD3D->MyCtrl->k_FreshSkillNum - 1], true, iComboIdx);
        }
        g_MyD3D->MyCtrl->k_Use_PetSkill = false;
        g_MyD3D->MyCtrl->k_Fresh_Joke = false;
    }

    // 특수 기술
    if (g_MyD3D->MyCtrl->k_Fresh_Fire)
    {
        // 던전이면 게임 클리어 시나 로딩화면 표시 시 필살기 못쓰게 한다.
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        {
            if (!g_pkQuestManager->m_bClearStage && !g_pkGameOverlayUI->IsShowQuestTitle())
                UseSpecial();
        }
        // 아지트에서 필살기 못쓰게 
        else if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT)
        {
            UseSpecial();
        }
    }

    if (g_MyD3D->MyCtrl->k_Change_SkillType)
    {
        if (g_kGlobalValue.IsCharUseSpecial4(GetCurrentChar().iCharType)) {
            if (MetamorphosisForm == FORM_NORMAL) {
                if (g_MyD3D->MyHead->IsBlackMagic == false) {
                    if (m_mapHotKeySkill[SKILL_TYPE_CASH_B].empty() == false) {
                        if (m_iSkillType == 0) {
                            m_iSkillType = 1;
                        }
                        else {
                            m_iSkillType = 0;
                        }

                        if (m_iSkillType == 0) {
                            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_B];
                            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_B];
                        }
                        else {
                            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_B];
                            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_B];
                        }
                    }
                }
                else {
                    if (m_mapHotKeySkill[SKILL_TYPE_CASH_A].empty() == false) {
                        if (m_iSkillType == 0) {
                            m_iSkillType = 1;
                        }
                        else {
                            m_iSkillType = 0;
                        }

                        if (m_iSkillType == 0) {
                            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_A];
                            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_A];
                        }
                        else {
                            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_A];
                            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_A];
                        }
                    }
                }
            }
        }
        else {
            SetSkillType(1 - m_iSkillType); //Toggle
        }

#if defined( USE_AP_SLOT_CHANGE )
        ChangeQuickSlotSkill();
#endif
    }
}

void PLAYER::CheckPetKey()
{
    if (g_MyD3D->MyCtrl->k_Use_PetSkill)
    {
        if (false == SiKGCWorldMapManager()->IsUsePetSkill(SiKGCRoomManager()->GetGameMode()))
            return;

        // 아지트에서 못쓰게 
        if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_INDIGO && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT)
        {
#if defined( DUNGEONPLAYSTATE )
            // 던전플레이통계( PetSkill )
            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
            {
                g_kGlobalValue.AddDungeonPlayState(KGCGlobal::DUNPLAY_STATE_PET);
                SendDungeonPlayState(KGCGlobal::DUNPLAY_STATE_PET);
            }
#endif
            if (CheckTransformPetSkill()) {
                //UseTransformPetSkill();
                g_MyD3D->m_akPet[m_iPlayerIndex]->UseTransformSkill();
            }
            else {
                g_MyD3D->m_akPet[m_iPlayerIndex]->UseSkill();
            }
        }
    }
}

void PLAYER::CheckGameItemKey()
{
    // ¾ÆÀÌÅÛ ¼±ÅÃ
    if (g_MyD3D->MyCtrl->k_Select_Item)
    {
        if (false == SiKGCWorldMapManager()->IsUseGameItem(SiKGCRoomManager()->GetGameMode()))
            return;

        // [3/18/2008] breadceo. ¶óÀÌ¾ð 3Â÷ ¼±ÇàÀÛ¾÷
        //if( GetCurrentChar().iCharType == GC_CHAR_RYAN && MetamorphosisForm == FORM_NEPHILIM )
        //{
        //    ProcessMetamorphosisNormalForm( true );
        //}
        g_KDSound.Play("73");
        g_MyD3D->MyItms->MoveNextItemCursorPos(GetNumGameItem());
        // ¿ö·ÏÀº ¹é/Èæ¸¶¹ý ÀüÈ¯ ¾øÀ½
        if ((GetCurrentChar().iCharType == GC_CHAR_ARME && Extra_Char_Num != CID_ARME3) || (GetCurrentChar().iCharType == GC_CHAR_RONAN))
        {
            if (MetamorphosisForm == FORM_NORMAL) {
                ArmeChageBlackNWhite();
                if (g_kGlobalValue.IsCharUseSpecial4(GetCurrentChar().iCharType)) {
                    if (g_MyD3D->MyHead->IsBlackMagic) {
                        if (m_iSkillType == 0) {
                            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_A];
                            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_A];
                        }
                        else {
                            if (m_mapHotKeySkill[SKILL_TYPE_CASH_A].empty() == false) {
                                m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_A];
                                m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_A];
                            }
                        }
                    }
                    else {
                        if (m_iSkillType == 0) {
                            if (m_mapHotKeySkill[SKILL_TYPE_NORMAL_B].empty() == false) {
                                m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_B];
                                m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_B];
                            }
                        }
                        else {
                            if (m_mapHotKeySkill[SKILL_TYPE_CASH_B].empty() == false) {
                                m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_B];
                                m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_B];
                            }
                        }
                    }
                }
            }
        }
    }

    // °ÔÀÓ Áß¿¡¸¸ ¾ÆÀÌÅÛ »ç¿ëµÇµµ·Ï
    if (false == IsDetermineWin)
    {
        // ´ÜÃà½½·Ô º¸ÀÌ±â||°¨Ãß±â ¹öÆ°À» ´­·¶´Â°¡
        if (g_MyD3D->MyCtrl->k_ShowShortCut == true)
        {
            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA)
                g_pkGameOverlayUI->ShowGuideMsg(!g_pkGameOverlayUI->GetShowGuide());
            else
                g_pkGameOverlayUI->ChangeShortCutSlotState();
        }

        // Äù½ºÆ® ¸ðµåÀÎ °æ¿ì¿¡¸¸ ´ÜÃà½½·ÔÀ» È°¼ºÈ­
        if (g_MyD3D->GetGameCategoryKind(SiKGCRoomManager()->GetGameModeCategory()) == GC_GMK_MONSTER && IsDie() == false)
        {
            if (false == NotUseShortCutItem())
            {
                // ¾ÆÀÌÅÛ ´ÜÃà½½·Ô Å°¸¦ ´­·¶´Â°¡	:	 KEY_UPÀº -1
                if (g_MyD3D->MyCtrl->k_ShortCut_Item != KEY_UP)
                {
                    //Luma 가 제작한 슬롯에서 투명 물약 사용을 비활성화합니다.
                    if (true == g_MyD3D->GetMyShortCutSlot()->CanUseItem(static_cast<int>(g_MyD3D->MyCtrl->k_ShortCut_Item)))
                        g_MyD3D->GetMyShortCutSlot()->UseItem(static_cast<int>(g_MyD3D->MyCtrl->k_ShortCut_Item));

                    g_pkGameOverlayUI->ToggleShortCutBox(true);
                }
            }
            else
                g_pkGameOverlayUI->ToggleShortCutBox(false);
        }

        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA)
        {
            if (g_MyD3D->MyCtrl->k_Game_Item != KEY_UP)
            {
                // °ÔÀÓ ¾ÆÀÌÅÛ »ç¿ë ´ÜÃàÅ°
                int iNumber = static_cast<INT>(g_MyD3D->MyCtrl->k_Game_Item);
                if (0 <= iNumber && iNumber < int(m_vecGameItem.size()))
                {
                    g_MyD3D->MyItms->SetItemCursorPos(iNumber);
                    g_MyD3D->MyItms->Use_Item(m_iPlayerIndex, GetGameItem(iNumber));
                    g_MyD3D->MyItms->Delete_Item();
                    Sound(10);
                }
            }
        }
    }
}

void PLAYER::CheckJokeKey()
{
    if (UINT_MAX > GetEmbarkUID())
        return;

    // 도발
    if (g_MyD3D->MyCtrl->k_Fresh_Joke)
    {
        if (uiMotion == GetWaitMotion() || uiMotion == GetFatalWaitMotion())
        {
            if (!IsCanUseHealingBall(HEALING_FRESH_JOKE))
            {
                cFrame = 0;
                SetPlayerState(GetJokeMotion());
            }
            else
            {
                UseHealingBall();
            }
        }
    }
}

void PLAYER::CheckShortCutItemKey()
{
#if defined( EMOTICON_SYSTEM )
    if (false == NotUseShortCutItem(true))
    {
        bool bEmot = true;
        std::map<int, SMacroCommunity> mapMacroCommunity = SiKGCWorldMapManager()->GetMacroCommunity(SiKGCRoomManager()->GetGameMode());
        if (false == mapMacroCommunity.empty())
            bEmot = false;

        if (bEmot)
        {
            if (g_MyD3D->MyCtrl->k_ShortCut_Emot != KEY_UP)
            {
                int iEmotIndex = static_cast<INT>(g_MyD3D->MyCtrl->k_ShortCut_Emot);
                if (g_MyD3D->GetMyShortCutSlot()->IsEquipSpecialSkillItem(iEmotIndex))
                    UseSpecial(iEmotIndex + 1);
                else
                    g_MyD3D->GetMyShortCutSlot()->UseEmot(iEmotIndex);
                g_pkGameOverlayUI->ToggleShortCutBox(true);
            }
        }
        else
        {
            if (g_MyD3D->MyCtrl->k_ShortCut_MacroCommunity != KEY_UP)
            {
                KGCShortCutBox* pShortCutBox = g_pkGameOverlayUI->GetShourtCutBoxInstance();
                if (pShortCutBox)
                {
                    int iEmotIndex = static_cast<INT>(g_MyD3D->MyCtrl->k_ShortCut_Emot);
                    std::map<int, SMacroCommunity>::iterator mapIter = mapMacroCommunity.find((iEmotIndex + 1));
                    if (mapIter != mapMacroCommunity.end())
                    {
                        pShortCutBox->SetInputCommunityKey(iEmotIndex, mapIter->second.m_iReEntryCool);
                        bEmot = false;
                        g_pkGameOverlayUI->ToggleShortCutBox(true);
                    }
                }
            }
        }
    }

#endif 
}

void PLAYER::CheckCameraZoomKey()
{
    SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo(SiKGCRoomManager()->GetGameMode());
    if (g_MyD3D->MyCtrl->k_keyCameraZoom && false == g_kGlobalValue.m_bNoCameraZoom)
    {
        if (pCameraZoom && 0 < int(pCameraZoom->m_vecZoomStep.size()))
        {
            int iStep = 0;
            std::vector<float>::iterator vecIter = pCameraZoom->m_vecZoomStep.begin();
            for (; vecIter != pCameraZoom->m_vecZoomStep.end(); ++vecIter)
            {
                ++iStep;
                if (g_kCamera.m_fWideMode == *vecIter)
                {
                    ++vecIter;
                    break;
                }
            }

            if (vecIter == pCameraZoom->m_vecZoomStep.end())
            {
                vecIter = pCameraZoom->m_vecZoomStep.begin();
                iStep = 0;
            }

#if defined( USE_CAMERA_ZOOM_STATE )
            AddCameraZoomState(g_kCamera.m_fWideMode, *vecIter);
#endif

            g_kCamera.m_fWideMode = *vecIter;
            g_kCamera.SavePrevState();
            g_pkGameOverlayUI->SetZoomScrollPos(iStep);
        }
    }

    if (pCameraZoom && 0 < int(pCameraZoom->m_vecZoomStep.size()))
    {
        g_pkGameOverlayUI->ShowZoomUI((false == g_kGlobalValue.m_bNoCameraZoom));
    }

    g_MyD3D->MyCtrl->k_keyCameraZoom = false;
}

void PLAYER::ReadyToRender(KRenderData* pkRenderData_)
{
    PROFILE_SET("PLAYER::ReadyToRender");

    if (m_pObject == NULL)
        return;

    KRenderData kRenderDataNowDrawing;
    if (pkRenderData_ == NULL)
    {
        if (IsLocalPlayer())
        {
            kRenderDataNowDrawing = g_kLocalPlayerLatency->kRenderData[0];
        }
        else
        {
            kRenderDataNowDrawing = *this;
        }
    }
    else
    {
        kRenderDataNowDrawing = *pkRenderData_;
    }

    vRenderPos = kRenderDataNowDrawing.vPos - (D3DXVECTOR3)vParticleToDamageMagicNumber;

    GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion(kRenderDataNowDrawing.usMotion);
    //D3DXMATRIX matScale, matRot, matTrans, matWorld;

    if (!pMotion ||
        pMotion->GetNum_Frame() == 0)
        return;

    y_Ex = (pMotion->GetFrameData(kRenderDataNowDrawing.cFrame).Pos_y + GetCurFormTemplate().fCharHeight) * CHAR_SCALE_RATIO;

    unsigned short ucFrame = kRenderDataNowDrawing.cFrame;
    if (ucFrame > pMotion->GetNum_Frame() - 1)
    {
        ucFrame = (unsigned char)pMotion->GetNum_Frame() - 1;
    }

    m_pObject->SetFrame(&pMotion->GetFrameData(ucFrame));


    float fCharScaleRatio = CHAR_SCALE_RATIO;
    if (GetCurFormTemplate().fPlayingScale != 0.f) {
        fCharScaleRatio = CHAR_SCALE_RATIO * GetCurFormTemplate().fPlayingScale;
    }
    D3DXMatrixScaling(&g_TempMatScale, fCharScaleRatio, fCharScaleRatio, fCharScaleRatio);

    if (true == m_bOffAllEffect)
    {
        if (ucFrame == pMotion->GetNum_Frame() - 2)
        {
            ShowAllItemEffect(true);
        }
        else
        {
            ShowAllItemEffect(false);
        }
    }

    // 캐쉬 방어 드루이드 발찌 아이템이펙트가 데미지랑 같이 뜨는데...
    // 강화 9짜리만 그것도 잡기 데미지에만 발찌 이펙트가 뜨는거...
    // 막는 코드 죄송ㅠㅠ 하드코드...
    if (MID_RYAN4_CATCH == kRenderDataNowDrawing.usMotion)
    {
        for (std::vector<ITEM_EFFECT>::iterator i = m_vecItemEff.begin(); i != m_vecItemEff.end(); i++)
        {
            if (((*i).sequenceName[0] == "RuneShield3") && ucFrame > 61)
            {
                ShowAllItemEffect(true);
            }
        }
    }

    D3DXMATRIX matEmbark;
    D3DXMatrixIdentity(&matEmbark);
    if (UINT_MAX > GetEmbarkUID())
    {
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark && pEmbark->m_pObject &&
            (-1 < GetEmbarkAdaptBoneID() && GetEmbarkAdaptBoneID() < static_cast<int>(pEmbark->m_pObject->GetNumAngleBone())))
        {
            kRenderDataNowDrawing.vPos.x = pEmbark->m_vRealRenderPos.x;
            kRenderDataNowDrawing.vPos.y = pEmbark->m_vRealRenderPos.y;

            pEmbark->GetBoneMatrix(GetEmbarkAdaptBoneID(), &matEmbark);
        }
    }

    if (true == IsPlusMotionX(kRenderDataNowDrawing.usMotion))
    {
        float fPlusX = 0.0f;
        for (int i = 0; i <= ucFrame; ++i)
        {
            fPlusX += pMotion->GetFrameData(i).Plus_x * CHAR_SCALE_RATIO;
        }
        if (bIsRight == false)
        {
            fPlusX = -fPlusX;
        }
        D3DXMatrixTranslation(&g_TempMatTranslate, kRenderDataNowDrawing.vPos.x - 1.0f + fPlusX, kRenderDataNowDrawing.vPos.y - 0.48f + y_Ex, 1.0f);
    }
    else
    {
        D3DXMatrixTranslation(&g_TempMatTranslate, kRenderDataNowDrawing.vPos.x - 1.0f, kRenderDataNowDrawing.vPos.y - 0.48f + y_Ex, 1.0f);
    }

    if (kRenderDataNowDrawing.bIsRight)
    {
        D3DXMatrixRotationY(&g_TempMatRotate, 0.0f);
    }
    else
    {
        D3DXMatrixRotationY(&g_TempMatRotate, 3.14159f);
    }

    g_TempMatWorld = g_TempMatScale * g_TempMatRotate * g_TempMatTranslate;
    g_TempMatWorld = matEmbark * g_TempMatWorld;

    m_pObject->SetWorldMat(&g_TempMatWorld);

    if (Extra_Char_Num == CID_LIRE3 && m_pLire3Arrow)
    {
        m_pLire3Arrow->m_bRender = m_bRenderLire3Arrow;
    }


    if (m_iFlashEffectFrame > 0)
    {
        if (g_MyD3D->m_KGCOption.IsCartoon())
        {
            if (kRenderDataNowDrawing.bIsRight)
                m_pObject->SetCartoon(CARTOON_DAMAGE_REVERSE);
            else
                m_pObject->SetCartoon(CARTOON_DAMAGE);
        }
        else // 카툰을 사용할수 없을때는 그냥 흰색으로 만들어 주더라...
        {
            // 2008.01.07 wony
            // 카툰이 아닐때는 그냥 텍스쳐가 흰색으로 번쩍번쩍 거려서 눈아플듯해서
            // 카툰이 아닐때는 적용시키지 않겠음
            //m_pObject->SetTexture( 0 );
        }
        m_iFlashEffectFrame--;
    }
    else
    {
        m_pObject->SetCartoon(g_pMagicEffect->GetCartoon(m_iPlayerIndex));
        m_pObject->SetTexture(-1);
    }

    D3DXMATRIX matbasic, matinv;
    Get_BasicMatrix(&matbasic);

    D3DXMatrixInverse(&matinv, NULL, &g_kCamera.m_matCamera);
    D3DXMatrixMultiply(&matbasic, &matbasic, &matinv);

    m_posDesc.right = kRenderDataNowDrawing.bIsRight;

    m_posDesc.body.x = matbasic._41;
    m_posDesc.body.y = matbasic._42;
    m_posDesc.body.z = matbasic._43;


    if (IsLocalPlayer())
    {
        for (unsigned char i = 0; i < 4 + (LATENCY + SHOWLATENCY); i++)
        {
            m_posDesc.rightHandLatancy[i * 2] = m_posDesc.rightHandLatancy[(i + 1) * 2];
            m_posDesc.rightHandLatancy[i * 2 + 1] = m_posDesc.rightHandLatancy[(i + 1) * 2 + 1];

            m_posDesc.leftHandLatancy[i * 2] = m_posDesc.leftHandLatancy[(i + 1) * 2];
            m_posDesc.leftHandLatancy[i * 2 + 1] = m_posDesc.leftHandLatancy[(i + 1) * 2 + 1];

            m_posDesc.headLatancy[i * 2] = m_posDesc.headLatancy[(i + 1) * 2];
            m_posDesc.headLatancy[i * 2 + 1] = m_posDesc.headLatancy[(i + 1) * 2 + 1];

            m_posDesc.rightShoulderLatancy[i * 2] = m_posDesc.rightShoulderLatancy[(i + 1) * 2];
            m_posDesc.rightShoulderLatancy[i * 2 + 1] = m_posDesc.rightShoulderLatancy[(i + 1) * 2 + 1];

            m_posDesc.leftShoulderLatancy[i * 2] = m_posDesc.leftShoulderLatancy[(i + 1) * 2];
            m_posDesc.leftShoulderLatancy[i * 2 + 1] = m_posDesc.leftShoulderLatancy[(i + 1) * 2 + 1];
        }

        SETVECTOR3(m_posDesc.rightHandLatancy[8 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.rightHandTip.x, m_posDesc.rightHandTip.y, m_posDesc.rightHandTip.z);
        SETVECTOR3(m_posDesc.rightHandLatancy[9 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.rightHandBody.x, m_posDesc.rightHandBody.y, m_posDesc.rightHandBody.z);

        SETVECTOR3(m_posDesc.leftHandLatancy[8 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.leftHandTip.x, m_posDesc.leftHandTip.y, m_posDesc.leftHandTip.z);
        SETVECTOR3(m_posDesc.leftHandLatancy[9 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.leftHandBody.x, m_posDesc.leftHandBody.y, m_posDesc.leftHandBody.z);

        SETVECTOR3(m_posDesc.headLatancy[8 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.HeadBombSpark.x, m_posDesc.HeadBombSpark.y, m_posDesc.HeadBombSpark.z);
        SETVECTOR3(m_posDesc.headLatancy[9 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.HeadBombSpark.x, m_posDesc.HeadBombSpark.y, m_posDesc.HeadBombSpark.z);
        SETVECTOR3(m_posDesc.rightShoulderLatancy[8 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.rightShoulder.x, m_posDesc.rightShoulder.y, m_posDesc.rightShoulder.z);
        SETVECTOR3(m_posDesc.rightShoulderLatancy[9 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.rightShoulder.x, m_posDesc.rightShoulder.y, m_posDesc.rightShoulder.z);

        SETVECTOR3(m_posDesc.leftShoulderLatancy[8 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.leftShoulder.x, m_posDesc.leftShoulder.y, m_posDesc.leftShoulder.z);
        SETVECTOR3(m_posDesc.leftShoulderLatancy[9 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.leftShoulder.x, m_posDesc.leftShoulder.y, m_posDesc.leftShoulder.z);

        SETVECTOR3(m_posDesc.rightShoulderLatancy[8 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.rightShoulder.x, m_posDesc.rightShoulder.y, m_posDesc.rightShoulder.z);
        SETVECTOR3(m_posDesc.rightShoulderLatancy[9 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.rightShoulder.x, m_posDesc.rightShoulder.y, m_posDesc.rightShoulder.z);

        SETVECTOR3(m_posDesc.leftShoulderLatancy[8 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.leftShoulder.x, m_posDesc.leftShoulder.y, m_posDesc.leftShoulder.z);
        SETVECTOR3(m_posDesc.leftShoulderLatancy[9 + (LATENCY + SHOWLATENCY) * 2], m_posDesc.leftShoulder.x, m_posDesc.leftShoulder.y, m_posDesc.leftShoulder.z);

    }
    else
    {
        // [2/5/2008] breadceo. 응?
        //if( CheckHiding() )
        //    return;
        for (unsigned char i = 0; i < 5; i++)
        {
            m_posDesc.rightHandLatancy[i * 2] = m_posDesc.rightHandLatancy[(i + 1) * 2];
            m_posDesc.rightHandLatancy[i * 2 + 1] = m_posDesc.rightHandLatancy[(i + 1) * 2 + 1];

            m_posDesc.leftHandLatancy[i * 2] = m_posDesc.leftHandLatancy[(i + 1) * 2];
            m_posDesc.leftHandLatancy[i * 2 + 1] = m_posDesc.leftHandLatancy[(i + 1) * 2 + 1];

            m_posDesc.headLatancy[i * 2] = m_posDesc.headLatancy[(i + 1) * 2];
            m_posDesc.headLatancy[i * 2 + 1] = m_posDesc.headLatancy[(i + 1) * 2 + 1];

            m_posDesc.rightShoulderLatancy[i * 2] = m_posDesc.rightShoulderLatancy[(i + 1) * 2];
            m_posDesc.rightShoulderLatancy[i * 2 + 1] = m_posDesc.rightShoulderLatancy[(i + 1) * 2 + 1];

            m_posDesc.leftShoulderLatancy[i * 2] = m_posDesc.leftShoulderLatancy[(i + 1) * 2];
            m_posDesc.leftShoulderLatancy[i * 2 + 1] = m_posDesc.leftShoulderLatancy[(i + 1) * 2 + 1];
        }

        SETVECTOR3(m_posDesc.rightHandLatancy[10], m_posDesc.rightHandTip.x, m_posDesc.rightHandTip.y, m_posDesc.rightHandTip.z);
        SETVECTOR3(m_posDesc.rightHandLatancy[11], m_posDesc.rightHandBody.x, m_posDesc.rightHandBody.y, m_posDesc.rightHandBody.z);

        SETVECTOR3(m_posDesc.leftHandLatancy[10], m_posDesc.leftHandTip.x, m_posDesc.leftHandTip.y, m_posDesc.leftHandTip.z);
        SETVECTOR3(m_posDesc.leftHandLatancy[11], m_posDesc.leftHandBody.x, m_posDesc.leftHandBody.y, m_posDesc.leftHandBody.z);

        SETVECTOR3(m_posDesc.headLatancy[10], m_posDesc.HeadBombSpark.x, m_posDesc.HeadBombSpark.y, m_posDesc.HeadBombSpark.z);
        SETVECTOR3(m_posDesc.headLatancy[11], m_posDesc.HeadBombSpark.x, m_posDesc.HeadBombSpark.y, m_posDesc.HeadBombSpark.z);

        SETVECTOR3(m_posDesc.rightShoulderLatancy[10], m_posDesc.rightShoulder.x, m_posDesc.rightShoulder.y, m_posDesc.rightShoulder.z);
        SETVECTOR3(m_posDesc.rightShoulderLatancy[11], m_posDesc.rightShoulder.x, m_posDesc.rightShoulder.y, m_posDesc.rightShoulder.z);

        SETVECTOR3(m_posDesc.leftShoulderLatancy[10], m_posDesc.leftShoulder.x, m_posDesc.leftShoulder.y, m_posDesc.leftShoulder.z);
        SETVECTOR3(m_posDesc.leftShoulderLatancy[11], m_posDesc.leftShoulder.x, m_posDesc.leftShoulder.y, m_posDesc.leftShoulder.z);
    }

    if (m_posDesc.charID == CID_RONAN4 && MetamorphosisForm == FORM_NORMAL && GetHP() > 0.f)
    {
        // 로난 3차 포스위치 계산m
        if (g_MyD3D->MyPlayer[m_iPlayerIndex]->GetIsRight())
            m_vecMonoVoltPos.x = m_posDesc.effectrightShoulder.x - 0.05f;
        else
            m_vecMonoVoltPos.x = m_posDesc.effectrightShoulder.x + 0.05f;
        m_vecMonoVoltPos.y = m_posDesc.effectrightShoulder.y + 0.1f;
        m_vecMonoVoltPos.z = m_posDesc.effectrightShoulder.z;

        // 로난 3차(어비스나이트) 왼손이펙트 위치 계산
        m_vecAbyssHandEffPos.x = m_posDesc.leftHandLatancy[11].x;
        m_posDesc.leftHandLatancy[11].y -= 0.23f;
        m_vecAbyssHandEffPos.y = m_posDesc.leftHandLatancy[11].y;
        m_vecAbyssHandEffPos.z = 1.0f;//m_posDesc.leftHandLatancy[11].z;				
    }
    else
    {
        m_vecMonoVoltPos.x = m_vecMonoVoltPos.y = -9999.0f;
        m_vecAbyssHandEffPos.x = m_vecAbyssHandEffPos.y = -9999.0f;

    }

    // 아르메 쇼크스턴 lv2 용 하드코딩 - 일정 땜에 ㅈㅅ ㅡ.,ㅡ
    if (m_posDesc.charID == CID_ARME1 && g_MyD3D->MyPlayer[m_iPlayerIndex]->uiMotion == MID_SKILL_ARME1_B2_SHOCKSTURN_LV2)
    {
        if (g_MyD3D->MyPlayer[m_iPlayerIndex]->cFrame >= (int)GetNowMotionFRM()->GetNum_Frame() - 1)
        {
            if (SiKGCRoomManager()->IsMonsterGameMode())
            {
                for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
                    mit->second->m_iShockSturnCount = 0;
                }

            }
            else
            {
                for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
                    mit->second->m_iShockSturnCount = 0;
                }
                for (int i = 0; i < MAX_PLAYER_NUM; i++)
                {
                    g_MyD3D->MyPlayer[i]->m_iShockSturnCount = 0;
                }
            }
        }
    }

    if (m_posDesc.charID == CID_LIRE1 || m_posDesc.charID == CID_LIRE2 || m_posDesc.charID == CID_LIRE3 || m_posDesc.charID == CID_LIRE4)
    {
        m_posDesc.bow = m_posDesc.leftHandLatancy[11];
        m_posDesc.bow.y -= 0.25f;
        m_posDesc.bowAngle = D3DXToRadian(90) + atan2f(m_posDesc.leftHandLatancy[10].y - m_posDesc.leftHandLatancy[11].y, m_posDesc.leftHandLatancy[10].x - m_posDesc.leftHandLatancy[11].x);

        m_posDesc.swordCenter = m_posDesc.bow;

        if (m_posDesc.charID == CID_LIRE2)
        {
            m_posDesc.crossBowLeft = m_posDesc.bow;
            m_posDesc.crossBowLeftAngle = m_posDesc.bowAngle;

            m_posDesc.crossBowRight = m_posDesc.rightHandLatancy[11];
            m_posDesc.crossBowRight.y -= 0.25f;

            m_posDesc.swordCenter = m_posDesc.crossBowRight;

            m_posDesc.crossBowRightAngle = D3DXToRadian(90) + atan2f(m_posDesc.rightHandLatancy[10].y - m_posDesc.rightHandLatancy[11].y, m_posDesc.rightHandLatancy[10].x - m_posDesc.rightHandLatancy[11].x);
        }
        else if (m_posDesc.charID == CID_LIRE4) // 노바..
        {
            m_posDesc.bow = m_posDesc.leftHandLatancy[10];
            m_posDesc.bow.y -= 0.24f;
            m_posDesc.bowAngle = D3DXToRadian(90) + atan2f(m_posDesc.leftHandLatancy[11].y - m_posDesc.leftHandLatancy[10].y, m_posDesc.leftHandLatancy[11].x - m_posDesc.leftHandLatancy[10].x);

            m_posDesc.swordCenter = m_posDesc.bow;
        }

    }
    else if (m_posDesc.charID == CID_LAS1 || m_posDesc.charID == CID_LAS3)
    {
        const int iStart = 11;
        const int iEnd = 10;
        m_posDesc.LasKnifeOne = (m_posDesc.rightHandLatancy[iStart] + m_posDesc.rightHandLatancy[iEnd]) * 0.5f; //Center좌표 사용
        m_posDesc.LasKnifeOne.y -= 0.25f;

        m_posDesc.LasKnifeTwo = (m_posDesc.leftHandLatancy[iStart] + m_posDesc.leftHandLatancy[iEnd]) * 0.5f; //Center좌표 사용
        m_posDesc.LasKnifeTwo.y -= 0.25f;

        if (m_posDesc.charID == CID_LAS3)
        {
            m_posDesc.LasKnifeOne.y += 0.065f;
            m_posDesc.LasKnifeTwo.y += 0.065f;
        }

        m_posDesc.swordCenter = m_posDesc.LasKnifeOne;

        //여기서부터는 단검을 쥐고 있는 각도를 구한다. 손 끝 좌표와 칼 끝 좌표를 이용한 것이다.!
        m_posDesc.LasKnifeOneAngle = atan2f(m_posDesc.rightHandLatancy[iEnd].y - m_posDesc.rightHandLatancy[iStart].y, m_posDesc.rightHandLatancy[iEnd].x - m_posDesc.rightHandLatancy[iStart].x);
        m_posDesc.LasKnifeTwoAngle = atan2f(m_posDesc.leftHandLatancy[iEnd].y - m_posDesc.leftHandLatancy[iStart].y, m_posDesc.leftHandLatancy[iEnd].x - m_posDesc.leftHandLatancy[iStart].x);
    }
    // 라이언 도끼
    else if (m_posDesc.charID == CID_RYAN1)
    {
        m_posDesc.RyanAxe = m_posDesc.leftHandLatancy[10];
        m_posDesc.RyanAxe.y -= 0.26f;

        m_posDesc.RyanAxe.x -= ((m_posDesc.leftHandLatancy[10].x - m_posDesc.leftHandLatancy[11].x)) * 0.30f;
        m_posDesc.RyanAxe.y -= ((m_posDesc.leftHandLatancy[10].y - m_posDesc.leftHandLatancy[11].y)) * 0.30f;

        m_posDesc.swordCenter = m_posDesc.RyanAxe;

        m_posDesc.RyanAxeAngle = D3DXToRadian(270)
            + atan2f(m_posDesc.leftHandLatancy[10].y - m_posDesc.leftHandLatancy[11].y,
                m_posDesc.leftHandLatancy[10].x - m_posDesc.leftHandLatancy[11].x);
    }
    // 센티넬 크리센트
    else if (m_posDesc.charID == CID_RYAN2)
    {
        // 크리센트
        if (MetamorphosisForm == FORM_NORMAL)
        {
            m_posDesc.SentinelCricent = m_posDesc.rightHandLatancy[11];
            m_posDesc.SentinelCricent.y -= 0.245f;

            m_posDesc.swordCenter = m_posDesc.SentinelCricent;

            bool bIsLocalPlayer = IsLocalPlayer();
            if ((bIsLocalPlayer && g_kLocalPlayerLatency[0].kRenderData[0].bIsRight) || (!bIsLocalPlayer && kRenderDataNowDrawing.bIsRight))
                m_posDesc.SentinelCricentAngle = atan2f(m_posDesc.rightHandLatancy[10].y - m_posDesc.leftHandLatancy[10].y, m_posDesc.rightHandLatancy[10].x - m_posDesc.leftHandLatancy[10].x);
            else
                m_posDesc.SentinelCricentAngle = atan2f(m_posDesc.leftHandLatancy[10].y - m_posDesc.rightHandLatancy[10].y, m_posDesc.leftHandLatancy[10].x - m_posDesc.rightHandLatancy[10].x);
        }
        else if (MetamorphosisForm == FORM_NEPHILIM)
        {
            m_posDesc.NephillimLeftHand = m_posDesc.leftHandLatancy[10];
            m_posDesc.NephillimRightHand = m_posDesc.rightHandLatancy[10];

            m_posDesc.NephillimLeftHand.y -= 0.25f;
            m_posDesc.NephillimRightHand.y -= 0.25f;

            m_posDesc.swordCenter = m_posDesc.NephillimRightHand;
        }
    }
    else
    {
        m_posDesc.swordTip = m_posDesc.rightHandLatancy[10];
        m_posDesc.swordTip.y -= 0.25f;
        m_posDesc.swordBody = m_posDesc.rightHandLatancy[11];
        m_posDesc.swordBody.y -= 0.25f;

        m_posDesc.leftSwordTip = m_posDesc.leftHandLatancy[10];
        m_posDesc.leftSwordTip.y -= 0.25f;
        m_posDesc.leftSwordBody = m_posDesc.leftHandLatancy[11];
        m_posDesc.leftSwordBody.y -= 0.25f;

        m_posDesc.swordAngle = atan2f(m_posDesc.swordTip.y - m_posDesc.swordBody.y, m_posDesc.swordTip.x - m_posDesc.swordBody.x);
        m_posDesc.leftSwordAngle = atan2f(m_posDesc.leftSwordTip.y - m_posDesc.leftSwordBody.y, m_posDesc.leftSwordTip.x - m_posDesc.leftSwordBody.x);

        if (m_posDesc.charID == CID_ARME1 || m_posDesc.charID == CID_ARME2)
        {
            m_posDesc.staffTip = m_posDesc.swordTip;
            m_posDesc.staffBody = m_posDesc.swordBody;
            m_posDesc.staffAngle = m_posDesc.swordAngle;
        }

        m_posDesc.swordCenter.x = (m_posDesc.swordBody.x + m_posDesc.swordTip.x) / 2.0f;
        m_posDesc.swordCenter.y = (m_posDesc.swordBody.y + m_posDesc.swordTip.y) / 2.0f;

        m_posDesc.leftSwordCenter.x = (m_posDesc.leftSwordBody.x + m_posDesc.leftSwordTip.x) / 2.0f;
        m_posDesc.leftSwordCenter.y = (m_posDesc.leftSwordBody.y + m_posDesc.leftSwordTip.y) / 2.0f;

        if (m_posDesc.charID == CID_ARME3)
        {
            m_posDesc.swordCenter.y -= 0.05f;
        }

        if (m_posDesc.charID == CID_ELESIS3)
        {
            m_posDesc.swordCenter.y -= 0.02f;
            m_posDesc.leftSwordCenter.y -= 0.02f;
        }

        if (m_posDesc.charID == CID_ELESIS4)
        {
            m_posDesc.swordCenter.y -= 0.01f;
            m_posDesc.leftSwordCenter.y -= 0.01f;
        }

        if ((m_posDesc.charID == CID_RYAN3 && MetamorphosisForm == FORM_NEPHILIM) ||
            (m_posDesc.charID == CID_RYAN4 && MetamorphosisForm == FORM_NEPHILIM))
        {
            m_posDesc.NephillimLeftHand = m_posDesc.leftHandLatancy[10];
            m_posDesc.NephillimRightHand = m_posDesc.rightHandLatancy[10];

            m_posDesc.NephillimLeftHand.y -= 0.25f;
            m_posDesc.NephillimRightHand.y -= 0.25f;
        }

        //if ( m_posDesc.charID == CID_RONAN3 )
        //{
        //    m_posDesc.rightHandTip = m_posDesc.rightHandLatancy[11];
        //    //m_posDesc.rightHandTip.x -= 0.1f;
        //    m_posDesc.rightHandTip.y -= 0.25f;
        //}
    }

    m_posDesc.effectrightShoulder = m_posDesc.rightShoulderLatancy[11];
    m_posDesc.effectrightShoulder.y -= 0.25f;
    m_posDesc.effectleftShoulder = m_posDesc.leftShoulderLatancy[11];
    m_posDesc.effectleftShoulder.y -= 0.25f;

    if (m_pEmoticon)
    {
        m_pEmoticon->SetPos(&D3DXVECTOR2(m_pObject->GetWorldMat()->_41, m_pObject->GetWorldMat()->_42));
        //m_pEmoticon->FrameMove();

        if (m_pEmoticon->IsEnd())
        {
            m_pEmoticon = NULL;
        }
    }

    if (MetamorphosisForm == FORM_MDAN)
    {
        m_pObject->GetAngleBone(4, &g_TempMatTranslate);
        D3DXMatrixMultiply(&g_TempMatTranslate, &g_TempMatTranslate, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&g_TempMatScale, 0.192664f, 0.7529f, -0.075852f);
    }
    else if (MetamorphosisForm == FORM_NEPHILIM)
    {
        m_pObject->GetAngleBone(12, &g_TempMatTranslate);
        D3DXMatrixMultiply(&g_TempMatTranslate, &g_TempMatTranslate, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&g_TempMatScale, 0.192664f, 0.7529f, -0.075852f);
    }
    else if (MetamorphosisForm == FORM_WOLF)
    {
        m_pObject->GetAngleBone(8, &g_TempMatTranslate);
        D3DXMatrixMultiply(&g_TempMatTranslate, &g_TempMatTranslate, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&g_TempMatScale, 0.146518f, 0.61405f, -0.035105f);
    }
    else
    {
        m_pObject->GetAngleBone(8, &g_TempMatTranslate);
        D3DXMatrixMultiply(&g_TempMatTranslate, &g_TempMatTranslate, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&g_TempMatScale, 0.154994f, 0.785072f, -0.213087f);
    }
    D3DXMatrixMultiply(&g_TempMatTranslate, &g_TempMatScale, &g_TempMatTranslate);
    SETVECTOR3(m_posDesc.HeadBombSpark, g_TempMatTranslate._41, g_TempMatTranslate._42, g_TempMatTranslate._43);


    //zstaiji - ParticleEffect.stg 스크립트에 머리중심 오프셋 이펙트 설정하기 위해서 추가
    D3DXMATRIX MatTranslateHead;
    D3DXMATRIX MatScaleHead;

    if (MetamorphosisForm == FORM_MDAN)
    {
        m_pObject->GetAngleBone(4, &MatTranslateHead);
        D3DXMatrixMultiply(&MatTranslateHead, &MatTranslateHead, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&MatScaleHead, 0.192664f, 0.7529f, -0.075852f);
    }
    else if (MetamorphosisForm == FORM_NEPHILIM)
    {
        m_pObject->GetAngleBone(12, &MatTranslateHead);
        D3DXMatrixMultiply(&MatTranslateHead, &MatTranslateHead, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&MatScaleHead, 0.192664f, 0.7529f, -0.075852f);
    }
    else if (MetamorphosisForm == FORM_WOLF)
    {
        m_pObject->GetAngleBone(8, &MatTranslateHead);
        D3DXMatrixMultiply(&MatTranslateHead, &MatTranslateHead, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&MatScaleHead, 0.146518f, 0.61405f, -0.035105f);
    }
    else
    {
        m_pObject->GetAngleBone(8, &MatTranslateHead);
        D3DXMatrixMultiply(&MatTranslateHead, &MatTranslateHead, m_pObject->GetWorldMat());
        D3DXMatrixTranslation(&MatScaleHead, 0.0f, 0.15f, 0.0f);
    }
    D3DXMatrixMultiply(&MatTranslateHead, &MatScaleHead, &MatTranslateHead);
    SETVECTOR3(m_posDesc.HeadPos, MatTranslateHead._41, MatTranslateHead._42, MatTranslateHead._43);



    // [3/17/2008] breadceo. 이 밑에는 애니메이션, 이펙트 렌더링에 관련된 코드만 넣을것

    for (std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
    {
        g_AnimManager.FrameUpdateItemAnim(&(*j), &m_posDesc, m_cLife);
    }

    if (m_bShowEffect == false)
        return;

    const int iShowTime = 255 * 2;
    //ZeroMemory( aiItemPosIndex, sizeof(aiItemPosIndex) );
    //ZeroMemory( aiMaxItemPosIndex, sizeof(aiMaxItemPosIndex) );


    std::map< int, std::vector<std::pair< std::set<int>, std::set<int> > > >::iterator mit;
    for (mit = m_mapOverlappedPair.begin(); m_mapOverlappedPair.end() != mit; mit++)
    {
        if (mit->first == 0)
        {
            continue;
        }

        std::vector<std::pair< std::set<int>, std::set<int> > >::iterator vit;
        int iVitIndex = 0;
        for (vit = mit->second.begin(); mit->second.end() != vit; vit++)
        {

            if (iVitIndex == (StartGameTime / iShowTime) % mit->second.size())
            {
                BYTE alpha = (255 - abs((int)StartGameTime % iShowTime - 255));

                if (mit->second.size() == 1)
                    alpha = 255;
                else
                    alpha = alpha > 85 ? 255 : alpha * 3;

                std::set<int>::iterator sit;
                for (sit = (*vit).second.begin(); sit != (*vit).second.end(); sit++)
                {
                    m_ItemAnim[(*sit)].animInst->SetDiffuse(D3DCOLOR_ARGB(alpha, 255, 255, 255));
                }

                for (sit = (*vit).first.begin(); sit != (*vit).first.end(); sit++)
                {
                    for (unsigned int i = 0; i < m_vecItemEff[(*sit)].pSeq.size(); i++)
                    {
                        m_vecItemEff[(*sit)].pSeq[i]->m_bRenderForImageTag = true;
                    }

                }


            }
            else
            {
                std::set<int>::iterator sit;
                for (sit = (*vit).second.begin(); sit != (*vit).second.end(); sit++)
                {
                    m_ItemAnim[(*sit)].animInst->SetDiffuse(0x00000000);
                }

                for (sit = (*vit).first.begin(); sit != (*vit).first.end(); sit++)
                {
                    for (unsigned int i = 0; i < m_vecItemEff[(*sit)].pSeq.size(); i++)
                    {
                        m_vecItemEff[(*sit)].pSeq[i]->m_bRenderForImageTag = false;
                    }
                }
            }
            iVitIndex++;
        }
    }


    // 완전 하드코딩 잘한다.. ㅠㅠ
    if (Extra_Char_Num == CID_LAS4)
    {
        if ((kRenderDataNowDrawing.usMotion == MID_LAS4_SPECIAL3 && ucFrame > 31) || kRenderDataNowDrawing.usMotion == MID_LAS4_SPECIAL3_FINISH ||
            (kRenderDataNowDrawing.usMotion == MID_LAS4_SPECIAL3_FINISH_LAST && ucFrame < 10))
        {
            for (std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
            {
                KGCAnimManager::ItemAnim& itemAnim = (*j);
                itemAnim.animInst->SetShow(false);
            }

            for (int i = 0; i < (int)Las4GreatSword.size(); ++i)
            {
                Las4GreatSword[i].animInst->SetShow(true);
                g_AnimManager.FrameUpdateItemAnim(&Las4GreatSword[i], &m_posDesc, m_cLife);
            }
        }
        else
        {
            for (std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
            {
                KGCAnimManager::ItemAnim& itemAnim = (*j);
                itemAnim.animInst->SetShow(true);
            }

            for (int i = 0; i < (int)Las4GreatSword.size(); ++i)
            {
                Las4GreatSword[i].animInst->SetShow(false);
            }
        }
    }

    std::map< std::string, std::vector<KGCAnimManager::ItemAnim> >::iterator mitAnim = m_mapPlayAnimation.begin();
    while (mitAnim != m_mapPlayAnimation.end())
    {
        std::vector<KGCAnimManager::ItemAnim>& vecAnim = mitAnim->second;
        std::vector<KGCAnimManager::ItemAnim>::iterator vit = vecAnim.begin();
        while (vit != vecAnim.end())
        {
            KGCAnimManager::ItemAnim& kAnim = (*vit);
            g_AnimManager.FrameUpdateItemAnim(&kAnim, &m_posDesc, m_cLife);
            ++vit;
        }
        ++mitAnim;
    }

    m_pObject->SetTexture(m_iSelectTextrue);

    std::vector<CParticleEventSeqPTR>::iterator vecIter = m_vecChildMeshParticle.begin();
    for (; vecIter != m_vecChildMeshParticle.end(); )
    {
        if (false == g_ParticleManager->IsLiveInstance(*vecIter))
        {
            vecIter = m_vecChildMeshParticle.erase(vecIter);
            continue;
        }

        (*vecIter)->SetShow(m_bRender && m_bRenderChildMeshParticle);
        ++vecIter;
    }

    m_pObject->AdjustParticlePos();
}

void PLAYER::OnDamagedProcess(void)
{
    bool bTrue = false;
    //잡기모션인데 
    if (IsCatchMotion(uiMotion))
    {
        //잡는모션이 아니면
        if (!IsCatchMotion(uiMotion, true))
        {
            //잡힌거임
            bTrue = true;
        }

    }
    else
    {
        bTrue = IsDamagedMotion();

    }


    if (this->uiMotion == GetCurFormTemplate().ARROW_REFLECTION ||
        this->uiMotion == GetCurFormTemplate().ARROWDEF_GROUND1 ||
        this->uiMotion == GetCurFormTemplate().ARROWDEF_GROUND2 ||
        this->uiMotion == GetCurFormTemplate().ARROWDEF_SKY)
        bTrue = true;

    if (bTrue)
    {
        //////////////////////////////////////////////////////////////////////////
        if (g_MyD3D->MyPlayer[m_iPlayerIndex]->IsMagicEffect(EGC_SKILL_HIDE))
        {
            g_MyD3D->MyPlayer[m_iPlayerIndex]->Item_Remain_Time[GC_GAME_ITEM_HALLOW] = 0;

            g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_SKILL_HIDE, false, true);
        }
        if (g_MyD3D->MyPlayer[m_iPlayerIndex]->IsMagicEffect(EGC_BUFF_CLONE_ATK))
        {
            g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_BUFF_CLONE_ATK);
        }
        //////////////////////////////////////////////////////////////////////////


        ClearChildMeshOneTimeMotion("WEAPON");
    }
}

//portal
void PLAYER::Frame_Move()
{
    PROFILE_SET("PLAYER::Frame_Move");

    // 오오라 발동시간 체크
    if (m_sRangeCheckInfo.m_kStopWatch.IsStop()) {
        InitRangeInfo();
    }

#if defined( USE_ONGAME_INVENTORY )
    if (IsLocalPlayer()) {
        if (GetHP() <= 0.0f && KD3DWnd::D3DWS_ENABLED == g_pkUIScene->GetStateUI(KGCUIScene::GC_MBOX_GAME_INVENTORY)) {
            g_pkUIScene->CloseGameInventory();
            return;
        }
        if (MetamorphosisForm != FORM_NORMAL ||
            uiMotion == MID_WOLF_FORM_START ||
            uiMotion == MID_NEPHILLIM_FORM_START ||
            uiMotion == MID_RYAN4_MDAN_FORM_START) {
            g_MyD3D->MyCtrl->k_Game_Inventory = false;
        }
        if (m_bBuffFrameLock)
        {
            g_MyD3D->MyCtrl->k_Game_Inventory = false;
        }
    }
#endif

#if !defined(__PATH__)
    if (g_kGlobalValue.RunPlayerAllMotion()) {
        return;
    }
#endif

    OnFrameMove();

    FrameMoveEmbarkation();
    FrameMoveItemEffect();

#if !defined(__PATH__)
    if (bReload)
    {
        g_kStatsManager.OnActionScript();
        g_kStatsManager.OnReloadFireInfo();
        bReload = false;
    }
#endif
    m_bShowBaseEffect = true;
    m_iProcessedFrame = 0;

    // AP캐릭터 스킬처리
    if (IsLocalPlayer())
    {
        // 콤보스킬 처리
        if (IsSetComboSkill())
        {
            GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, m_vecHotKeySkill[m_iComboSkillHotKeyIdx].m_eSkill)->m_pHotKeySkill;

            // 콤보가 있는지 확인
            if (info && (false == info->m_vecComboSkill.empty()))
            {
                if ((cFrame == info->m_vecComboSkill[(m_iComboSkillComboIdx - 1)].m_iMotionChangeFrame) ||
                    (cFrame == (GetFrameNum() - 1)))
                {
                    UseSkill(&m_vecHotKeySkill[m_iComboSkillHotKeyIdx], true, m_iComboSkillComboIdx);
                    g_MyD3D->MyCtrl->k_Use_PetSkill = false;
                    g_MyD3D->MyCtrl->k_Fresh_Joke = false;
                    m_iComboSkillHotKeyIdx = -1;
                    m_iComboSkillComboIdx = -1;
                }
            }
        }

        // 스킬 쿨타임 처리
        int iKeyNumber = 0;
        for (std::vector< GCHotKeySkill >::iterator vit = m_vecHotKeySkill.begin(); vit != m_vecHotKeySkill.end(); vit++)
        {
            // 콤보키 쿨타임 구간 확인
            if (cFrame > vit->iEndKeyTime)
            {
                // 스킬키 쿨타임 확인
                if (0 >= vit->fCoolTime)
                {
                    if (iKeyNumber < int(g_MyD3D->MyCtrl->m_vecKeyCnt.size()))
                        g_MyD3D->MyCtrl->m_vecKeyCnt[iKeyNumber] = 0;
                    vit->iStartKeyTime = 0;
                    vit->iEndKeyTime = 0;
                }
                else
                {
                    vit->fCoolTime -= GC_ELAPSED_TIME;
                }
            }

            if (iKeyNumber < int(g_MyD3D->MyCtrl->m_vecKeyCnt.size()))
                ++iKeyNumber;
        }

#if defined( USE_AP_SLOT_CHANGE )
        for (std::map< int, vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkillBySlotType.begin(); mit != m_mapHotKeySkillBySlotType.end(); ++mit) {
            if (mit->first == m_iCurQuickSlotType) {
                continue;
            }

            for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
                if (0 >= vit->fCoolTime) {
                    vit->iStartKeyTime = 0;
                    vit->iEndKeyTime = 0;
                }
                else {
                    vit->fCoolTime -= GC_ELAPSED_TIME;
                }
            }
        }
#endif

        //여기도 진짜 각성 들어가면 들어내야 하는 부분임....
        if (g_kGlobalValue.IsCharUseSpecial4(Extra_Char_Num % GC_CHAR_NUM)) {
            CountSkillCoolTime();
        }
    }

    if (m_kUserInfo.nUserState != GC_RUS_PLAYING)
    {
        if (m_pObject)
        {
            m_pObject->SetRender(false);
        }
        return;
    }

#if !defined( __PATH__ )
    if (uiMotion == GetWaitMotion() &&
        m_iTutorialPlayerAction != -1 &&
        true == VerifyMotion(m_iTutorialPlayerAction) &&
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
    {
        SetPlayerState(m_iTutorialPlayerAction);
        cFrame = 0;
    }
#endif

#if defined( _DEBUG ) || !defined( __PATH__ )
    m_vPos_inQuest.x = vPos.x * 400.0f;
    m_vPos_inQuest.y = vPos.y * 400.0f;
#endif
    ShieldTime();

    EGCGameModeCategory eGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();

    if (CheckInPortal())
    {
        m_bRender = false;
        m_bCrashCheck = false;
    }

    //////////////////////////////////////////////////////////////////////////
    // 오전 10:05 2008-03-18    by jemitgge
    // 죽었을때도 이모티콘 쓸수 있도록 죽었는지 체크 하고 리턴하는 코드 앞에 
    // 이모티콘 프레임 검사한다.
    if (NULL != m_pEmoticon && m_pEmoticon->IsEnd())
        m_pEmoticon = NULL;
    //////////////////////////////////////////////////////////////////////////

    SiKGCCoupleSystem()->NearCoupleEffect(this);
    SiKGCValentineDayManager()->NearChocolateEffect(this);

    // 무적모드 유지 코드..
    if (m_bSuperMode) Super = 255;
    if (0 <= m_iSuperArmorFrame)
    {
        m_bSuperArmor = (0 < m_iSuperArmorFrame);
        --m_iSuperArmorFrame;
    }

    // 필살기 제외한 무적모드
    if (m_uiNotSpecialSuper > 0)
        --m_uiNotSpecialSuper;
    else
        m_uiNotSpecialSuper = 0;

    if (m_kUserInfo.cCharIndex == -1 || m_cLife < 1)
    {
        if (m_pObject)
        {
            m_pObject->SetRender(false);
            ToggleMeshAll(false);
        }
        if (m_bShowEffect == true)
            ShowAllItemEffect(false);
        return;
    }

    bool bInvinsible = m_pObject != NULL ? m_pObject->IsInvisible() : false;

    if (m_bShowEffect && bInvinsible)
    {
        ShowAllItemEffect(false);
    }
    else if (m_bShowEffect && !bInvinsible && !CheckHiding())
    {
        ShowAllItemEffect(true);
    }

    m_bFrameLock = false;

    if (Fatal_Sound_Delay > 0)
        Fatal_Sound_Delay--;

    if (m_iJumpPossibleFrame > 0)
        m_iJumpPossibleFrame--;

    if (m_iShakeEffectFrame > 0)
        m_iShakeEffectFrame--;

    if (m_iCannotCatchFrame > 0)
    {
        m_iCannotCatchFrame--;
    }

    // 변신이 풀려야하면 풀어준다.
    // 대전일 경우는 승패가 결정되었을 경우에, 퀘스트의 경우는 스테이지가 넘어갈 때..
    if (IsDetermineWin)
    {
        // [3/18/2008] breadceo. 던전이 아니면 여기서 풀려야한다
        ProcessMetamorphosisNormalForm(SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON);
    }

    //Item 유효 시간을 줄임
    UpdateItemTime();

    //공용-HP/MP 증가 : 쉴드 부분을 따로 새로운 변수만 단독으로 사용하도록 만들어야 될듯
    RecoveryHP_MP();

    // 진 전용 - 버닝게이지 업데이트
    UpdateBP();
    UpdateHittingPoint();
    DecreaseMarble();

    // 수량 포인트 계산
    ChargeQuantityPoint();

    // 레이지 모드 체크 : 다른 캐릭들도 쓸려면 플레이어 템플릿에서 iRageModeHpPercent 값 셋팅 하도록 !!!!
    CheckRageMode();

    // 단축슬롯 쿨타임 감소
    if (g_MyD3D->GetGameCategoryKind(eGameModeCategory) == GC_GMK_MONSTER && IsDie() == false
        && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MINIGAME)
    {
        IncHP_inFrameMove();
        IncMP_inFrameMove();
    }

    EffectFrameMove();

    RenderFadeInOut();

    CheckTargetInRange();

    // 2007/2/20. iridology. 손에 붙은 파티클 FrameMove. 좌우에 따라서 Layer만 변경합니다.
    HandFireFrameMove();
    // 해당모션에서만 보여줄 파티클 리스를 지워야 되는지 확인 후 지운다.
    CheckMotionParticleList();

    EgoItemFrameMove(cFrame);

    ReadyToRender();

    // 신강화 이펙트 그려주자!!
    FrameMove_EnchantWeaponEffect();

    //====================================================================================
    // 2009.03.31 : Jemitgge
    // Comment : 이 플레이어가 데미지를 입었나 확인하고 입었다면 해야하는 일들을 한다..
    OnDamagedProcess();

#ifdef DECANEE
    if (m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_DECANEE)
    {
        IncreaseMarble();
        UpdateSubjulgate();
    }
#endif

    if (Shield <= 0) {
        m_bShieldTypeCounting = false;
    }

    ProcessFreezeMotion();
    if (IsLocalPlayer() || eGameModeCategory == GC_GMC_TUTORIAL)// 내 것만 계산한다.
    {
        --m_dwPrevTagTime;
        if (m_dwPrevTagTime > (UINT)(GC_FPS_LIMIT * 5))
            m_dwPrevTagTime = 0;



        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN ||
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
        {
            if (m_iArenaCoolTime > 0)
            {
                m_iArenaCoolTime--;
                if (m_iArenaCoolTime == 0)
                {
                    Arena();
                }
                goto FrameSkipped;
            }

        }

        // 변신 지속시간이 다되고 필살기 도중이 아닐 때
        if (m_bNormalForm == true && !IsSpecialSkillMotion())
        {
            ProcessMetamorphosisNormalForm();
            m_bNormalForm = false;
        }

        bool bEnableKeyInput = g_MyD3D->EnableKeyInput();

        if (!IsLocalPlayer())
            bEnableKeyInput = false;
        // NOTE : 퀘스트 모드의 스토리 대화 상태인 경우는 제외한다.
        //Receive_Key_in_Game();
        if (bEnableKeyInput)
            CommonKeyInput();


        //슬로우 모션을 만들어 주는 부분
        if (IsSkipFrameTime())
            goto FrameSkipped;

        //무거워졌다는 느낌이
        //한프레임에 한가지 인풋에 대해서만
        //반응하기 때문이라고 생각해서 아래 함수 호출을 루프로 수정함



        if (CheckInputSavingSystem() == false) {
#if defined(__PATH__)
            if (bEnableKeyInput)
#else
            if (bEnableKeyInput && !g_kGlobalValue.m_bStartCollectDamageList)
#endif
            {
                unsigned int iPrevMotion;
                do
                {
                    iPrevMotion = uiMotion;
                    (*m_pCurStateFunction)(this, true); //KeyInput처리로 Frame이 0으로 갈수 있음
                    KGCPlayerCommon::OnEmpty(this, true); //KeyInput처리로 Frame이 0으로 갈수 있음
                } while (iPrevMotion != uiMotion);

                MoveXInSky();
                g_MyD3D->MyCtrl->k_ShortCut_Item = KEY_UP;
                g_MyD3D->MyCtrl->k_ShortCut_Emot = KEY_UP;

            }

#if !defined(__PATH__)
            if (!g_kGlobalValue.m_bStartCollectDamageList) {
                (*m_pCurStateFunction)(this, false);
            }
#else
            //프레임 루프 관련
            //현재 프레임에 대한 적적한 처리를 해줌
            (*m_pCurStateFunction)(this, false);
#endif
            if (bEnableKeyInput && CheckChangeSwap())
            {
                ProcessSwapWeapon();
                //            g_KDSound.Play( "673" );
            }
        }

        //공중에서 막 착지 했을때 처리를 해줌. Frame이 0이 될수 있음
        if (CheckContact())
        {
            if (!ProcessSlowCount())
                goto FrameSkipped;
        }

        if (IsTagMotion())
            y_Speed = 0.0f;

        //-------------------------------------y축 이동-------------------------------------
        //====================================================================================
        // 2009.02.03 : Jemitgge
        // Comment : 모션으로 y값을 바꾸었다면, 다음 모션도 이전 모션의 y값에 따른다.

        //SetBeforeMotionPosY();
        //====================================================================================

        if (0 < m_iStopSpeedFrame)
            y_Speed = 0.0f;

        // 버프 프레임 락 상태에서는 그대로 굳어버린다... 당연히 움직이면 안되겠지??
        if (m_bBuffFrameLock)
            y_Speed = 0.0f;

        vPos.y += y_Speed;// + Gravity;

        if (vPos.y < -0.2f)
        {
            if (vPos.y > -0.23f)
            {
                Ah_Sound(1);

                if (m_bDeathFromFall == false)
                {
                    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
                    {
                        // 진짜 번지인지 아니면 자살인지 Check해보자.
                        float fGetExpGpRatio = 0.0f;
                        if (GetLastKillmePlayer() != MAX_PLAYER_NUM)
                        {
                            fGetExpGpRatio = GetExpGpRatio(GetLastKillmePlayer(), m_iPlayerIndex);

                            KGC_PID_BROAD_PLAYER_DEATH_INFO kPacket(GC_PID_BROAD_PLAYER_DEATH_INFO,
                                g_kGlobalValue.m_kUserInfo.dwUID,
                                g_MyD3D->MyPlayer[GetLastKillmePlayer()]->m_kUserInfo.dwUID,
                                fGetExpGpRatio, true, (GetLastKillmePlayer() == MAX_PLAYER_NUM), false);
                            SERIALIZE_P2P_PACKET(kPacket);
                            SendP2PPacket((void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST, _RELIABLE);
                        }
                        else
                        {
                            fGetExpGpRatio = GetExpGpRatio(m_iPlayerIndex, m_iPlayerIndex); //자살로 처리한다.

                            KGC_PID_BROAD_PLAYER_DEATH_INFO kPacket(GC_PID_BROAD_PLAYER_DEATH_INFO,
                                g_kGlobalValue.m_kUserInfo.dwUID,
                                g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.dwUID,
                                fGetExpGpRatio, true, (GetLastKillmePlayer() == MAX_PLAYER_NUM), false);
                            SERIALIZE_P2P_PACKET(kPacket);
                            SendP2PPacket((void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST, _RELIABLE);
                        }

                        m_bDeathFromFall = true;
                        KGCPC_MEMO("GC_PID_BROAD_PLAYER_DEATH_INFO");
                    }
                }
            }
            if (vPos.y < -4.f) {

                if (IsMonsterGameMode()) {
                    float fFallDamage = GetPlayerMaxHP() * 0.3f;
                    SetHP(GetHP() - fFallDamage);

                    if (IsFatal() == false)
                    {
                        // 탑승물 아레나가 셋팅되어 있다면
                        if (m_kEmbarkArena.IsSetting())
                        {
                            if (IsLocalPlayer() && false == m_kEmbarkArena.m_bSendPacket)
                            {
                                SiKGCEmbarkManager()->AddEmbark(m_kEmbarkArena.m_iEmbarkType, m_kEmbarkArena.m_fPosX, m_kEmbarkArena.m_fPosY, m_kEmbarkArena.m_bRight, true, m_kUserInfo.dwUID);
                                m_kEmbarkArena.m_bSendPacket = true;
                            }
                            vPos.y = -3.0f;
                        }
                        else if (IsFallArenaGameMode())
                        {
                            Calculate_Arena_Pos();
                        }
                        // 떨어졌을때 가장 가까운 안전한 위치로 이동
                        else
                        {
                            D3DXVECTOR2 vSafePos = g_MyD3D->MyStg->Check_SafePostionX(vPos.x, vPos.y);
                            SiKGCFantasticMap()->CheckPierceBlockXPos(vPos.x, vSafePos.x, vPos.y);
                            for (int i = 0; i < LATENCY + SHOWLATENCY; ++i)
                            {
                                g_MyD3D->MyPlayer[m_iPlayerIndex]->L_x[i] = vSafePos.x;
                                g_MyD3D->MyPlayer[m_iPlayerIndex]->L_y[i] = vSafePos.y;
                            }
                            vPos.x = vSafePos.x;
                            vPos.y = vSafePos.y;
                        }
                    }
                }
            }
            if (vPos.y < -5.0f)
            {
                // 죽을 때 처리해 줘야함
                ProcessMetamorphosisNormalForm(true);
            }
            if (vPos.y < -7.0f)
            {

                Kill();
                m_bDeathFromFall = false;
                goto FrameSkipped;
            }
        }

        // 낙하속도를 Fall_Speed로 조절가능하도록...
        // Fall_Speed는 컨택되었을때 기본값으로 초기화된다.
        // 원래코드 : y_Speed -= 0.0015f;
        if (y_Speed > 0.0)            y_Speed -= DEAULT_FALL_SPEED;
        else                           y_Speed -= m_fFall_Speed;

        if (y_Speed < -0.03f)
        {
            if (uiMotion != MID_LAS3_DOWN_JUMPDASH)
                y_Speed = -0.03f;
        }
        //-------------------------------------y축 이동 끝----------------------------------

        //-------------------------------------x축 이동-------------------------------------
        float fBeforeX = vPos.x;
        GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion(uiMotion);

        // 모션의 X축이 이동했을 경우
        if (NULL != pMotion && !m_bBuffFrameLock)
        {
            if (pMotion->GetNum_Frame() > cFrame)
            {
                float fMotionMove = (pMotion->GetFrameData(cFrame).Plus_x * CHAR_SCALE_RATIO);
                if (0.0f != fMotionMove)
                {
                    if (SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL)
                    {
                        if (bIsRight)    vPos.x += fMotionMove;
                        else            vPos.x -= fMotionMove;
                    }
                    else
                    {
                        if (bIsRight)    SetDirectMove((vPos.x + fMotionMove), vPos.y);
                        else            SetDirectMove((vPos.x - fMotionMove), vPos.y);
                    }
                }
            }
        }

        if (0 < m_iStopSpeedFrame)
            x_Speed = 0.0f;

        // 버프 프레임 락 상태에서는 그대로 굳어버린다... 당연히 움직이면 안되겠지??
        if (m_bBuffFrameLock)
            x_Speed = 0.0f;

        // 좌,우 발판 충돌확인
        if (0.0f != x_Speed && SiKGCRoomManager()->IsMonsterGameMode())
        {
            float fAfterX = vPos.x + x_Speed;

            // 여분의 오프셋 처리
            if (fBeforeX < fAfterX)
            {
                fBeforeX -= FOOTHOLDSIZE;
                fAfterX += FOOTHOLDSIZE;
            }
            else
            {
                fBeforeX += FOOTHOLDSIZE;
                fAfterX -= FOOTHOLDSIZE;
            }

            if (SiKGCFantasticMap()->CheckPierceBlockXPos(fBeforeX, fAfterX, vPos.y))
            {
                x_Speed = 0.0f;
            }
        }

        // Speed적용( 가속도 처리포함 )
        if (uiMotion != GetCurFormTemplate().JUMP_READY)
        {
            vPos.x += x_Speed;
            float f_Accelation = 0.0005f;
            if (ContactBlock == 4 && IsContact)
            {
                f_Accelation = 0.0001f;
            }
            else
            {
                f_Accelation = 0.0005f;
            }
            if (x_Speed > f_Accelation - 0.0001f)
                x_Speed -= f_Accelation;
            else if (x_Speed < -(f_Accelation - 0.0001f))
                x_Speed += f_Accelation;
            if (x_Speed < f_Accelation && x_Speed > -f_Accelation)
                x_Speed = 0.0f;
        }

        if (0 < m_iStopSpeedFrame)
            --m_iStopSpeedFrame;

        //-------------------------------------x축 이동 끝----------------------------------
        if (!m_bFrameLock && !m_bBuffFrameLock) {
            ++cFrame;
        }

        //==================================================================================
        //Frame++아래에서 모션이 바뀌게 되면 Frame = 0을 뛰어넘어 버리는 현상이 필연적이다
        //따라서 Reserve_Motion과 Reserve_IsRight라는 변수를 사용해서 이 현상을 해결한다
        if (Reserve_Motion != 0)
        {
            SetPlayerState(Reserve_Motion);
            bIsRight = Reserve_IsRight;
            cFrame = 0;
            Reserve_Motion = 0;
        }
        //==================================================================================

        if (0 < Next_Attack_Count && Next_Attack_Count < 255)
            --Next_Attack_Count;

        //===================================================================================
        // 모션의 후 처리 부분
        //===================================================================================

        //1. 불가능한 모션들...
        if (!IsContact &&
            (uiMotion == GetWaitMotion() || uiMotion == GetCurFormTemplate().WALK || uiMotion == GetFatalWaitMotion()))
        {
            SetPlayerState(GetCurFormTemplate().JUMP);
            cFrame = 0;
        }

        // 쓰러지는 동작인데...IsContact는 false라면...
        if (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) && !IsContact)
        {
            Slow_Count = 0;
            SetPlayerState(MID_COMMON_DOWN_IN_SKY);
            cFrame = 0;
        }

        /////////////////////////////////////////////////////////////////////
        // DATE: 06-12-28 milennium9@kogstudios.com
        /////////////////////////////////////////////////////////////////////
        // 케릭터가 미친듯이 쓰러지면 계속 쓰러지는 버그일 수 있다.
        // 버그를 수정하기 위해서 3초 이내에 5번 넘게 쓰러지면 다른 위치로 옮겨버린다.
        /////////////////////////////////////////////////////////////////////
        if (uiMotion == GetOwnMotion(MID_COMMON_DOWN_IN_SKY) && cFrame == 10)
        {
            ++m_iDownCount;
            if (m_dwDownBugTime == -1)
            {
                m_iDownCount = 0;
                m_dwDownBugTime = timeGetTime();
            }

            //3초가 지나기 전에 4번 넘어진 경우. 계속 쓰러지고 있다면 적어도 6초이내에 이 조건을 만족할 것이다.
            if (timeGetTime() - m_dwDownBugTime < 3000 && m_iDownCount == 4)
            {
                //초기화 시켜둔다.
                m_iDownCount = 0;
                m_dwDownBugTime = -1;

                SetPlayerState(MID_COMMON_SMALLDAMAGE);
                Direct_Motion_Input(MID_COMMON_SMALLDAMAGE);
            }

            if (m_iDownCount == 5)
            {
                m_iDownCount = 0;
                m_dwDownBugTime = -1;
            }
        }

        pMotion = g_pGraphicsHelper->GetPlayerActionMotion(uiMotion);
        if (pMotion)
        {
            //2. Frame이 끝났을때...
            if (cFrame >= pMotion->GetNum_Frame())//프레임 인덱스가 넘어섰음
            {
                if (IsContact)
                {
                    if (GetHP() <= 0.f)
                        SetPlayerState(GetFatalWaitMotion());
                    else SetPlayerState(GetWaitMotion());


                    x_Speed = 0.0f;
                }
                else SetPlayerState(GetCurFormTemplate().JUMP);

                m_fPhysicAtkPower = 0.0f;
                cFrame = 0;
                Body_Angle = 0;
            }
        }

        // 던전은 실패했을때 WIN모션 안취하도록 쌩깐다.
        bool bSetWinPos = true;
        if (GC_GMC_DUNGEON == eGameModeCategory && false == g_pGameBoard->m_pStartingMember[m_iPlayerIndex].IsWin)
            bSetWinPos = false;

        //3. 승리조건을 만족했다면 WIN 포즈를 취한다.
        if (uiMotion != GetCurFormTemplate().WIN && bSetWinPos)
        {
            // [1/5/2008] breadceo. 변신중에는 그냥 풀어!
            if (uiMotion == GetFatalWaitMotion() || uiMotion == GetWaitMotion() ||
                uiMotion == GetCurFormTemplate().WALK || uiMotion == GetCurFormTemplate().RUN ||
                MetamorphosisForm != FORM_NORMAL)
            {
                // 게임이 끝났거나 던전에서 스테이지를 클리어 했을 때
                if (IsDetermineWin || (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() && g_pkQuestManager->m_bClearStage))
                {
                    if (GetCurFormTemplate().WIN == GetWaitMotion() && IsContact &&
                        uiMotion != GetCurFormTemplate().CATCH && uiMotion != (GetCurFormTemplate().CATCH + 1))
                    {
                        int iForm = MetamorphosisForm;
                        ProcessMetamorphosisNormalForm();
                        m_FormTemplate[iForm].WIN = m_FormTemplate[iForm].WAIT;
                    }
                    else if (GetCurFormTemplate().WIN != GetWaitMotion())
                    {
                        SetPlayerState(GetCurFormTemplate().WIN);

                        if (m_kUserInfo.GetCurrentChar().kPetInfo.m_dwUID != 0)
                        {
                            if (g_pGameBoard->m_pStartingMember[m_iPlayerIndex].IsWin ||
                                ((GC_GMC_DUNGEON == eGameModeCategory) && g_pkQuestManager->m_bClearStage))
                                g_MyD3D->m_akPet[m_iPlayerIndex]->m_bIsWin = true;
                            else
                                g_MyD3D->m_akPet[m_iPlayerIndex]->m_bIsLose = true;
                        }

                        x_Speed = 0;
                        cFrame = 0;
                    }
                }
            }
        }
    }
    else
    {
        if (Slow_Count > 0)
            --Slow_Count; // 여러번 맞는 일을 막기 위해 자체적으로 Slow_Count를 쓴다.

        if (pLatency < LATENCY)//버퍼를 이용함
        {
            //-----------------버퍼로 부터 값을 취함---------------------------------------------------
#ifdef LEVEL_DESIGN_STAT_CALC
            SetHP(100000.0f);
            //SetHP(100.0f);
#else
            SetHP(100000.0f);
#endif		
            Shield = 5000;
            for (unsigned char i = 0; i < LATENCY; i++)//버퍼에있는 HP,Shield중에 가장 최소값을 취함
            {
                if (GetHP() > L_HP[i])
                    SetHP(L_HP[i]);
                if (Shield > L_Shield[i])
                {
                    Shield = L_Shield[i];
                }
            }

            bIsRight = L_IsRight[pLatency];
            x_Speed = L_x[pLatency] - vPos.x;
            y_Speed = L_y[pLatency] - vPos.y;
            vPos.x = L_x[pLatency];
            vPos.y = L_y[pLatency];
            //x=L_x[pLatency];
            //y=L_y[pLatency];
            //x_Speed=L_x_Speed[pLatency];
            //y_Speed=L_y_Speed[pLatency];
            cFrame = L_Frame[pLatency];
            //-----------------버퍼로 부터 값을 취함 끝-------------------------------------------------


            //if( IsContact && ( Now_Motion == MID_COMMON_DOWN_AND_STANDUP || Now_Motion == GetCurFormTemplate().WAKE ) )
            //{
            // if( rand() % 30 == 0 )
            // EffectDust();
            //}

            //현재 모션을 업데이트하고 Frame이 넘어가면.. 정지시켜버림
            //(전송받을때 까지.. 대기한다)
            if (uiMotion != L_Now_Motion[pLatency])
            {
                SetPlayerState(L_Now_Motion[pLatency]);
            }

            GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion(uiMotion);
            if (pMotion)
            {
                if (pMotion->GetNum_Frame() > cFrame)
                {
                    if (cFrame > (int)pMotion->GetNum_Frame() - 1)
                        cFrame = (int)pMotion->GetNum_Frame() - 1;
                }
            }

            // 뒤집기 중일 때는 일체의 컨텍트 처리를 하지 않음
            if (IsCatchMotion(uiMotion) && cFrame <= 40)
            {
                x_Speed = 0.0f;
                y_Speed = 0.0f;
            }
            else
            {
                bool bNoCheckContact = false;

                // 탑승물과 본이 연결되었다면 발판체크 하지 않는다.
                if (UINT_MAX > GetEmbarkUID())
                {
                    KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
                    if (pEmbark && pEmbark->m_pObject &&
                        (-1 < GetEmbarkAdaptBoneID() && GetEmbarkAdaptBoneID() < static_cast<int>(pEmbark->m_pObject->GetNumAngleBone())))
                    {
                        bNoCheckContact = true;
                    }
                }

                if (false == bNoCheckContact)
                {
                    Old_IsContact = IsContact;
                    IsContact = false;
                    if (SiKGCRoomManager()->IsMonsterGameMode())
                    {
                        KGCContectInfo kInfo(vPos.x, vPos.y, x_Speed, y_Speed, 0, IsTagMotion(), true, (vPos.y + m_posDesc.HeadPos.y));
                        if (true == g_MyD3D->MyStg->Check_Contact(kInfo))
                        {
                            if (kInfo.m_ySpeed <= 0.0f)
                            {
                                if (!Old_IsContact)
                                    Sound(0);
                                IsContact = true;
                            }
                        }
                    }
                    else
                    {
                        D3DXVECTOR3 staticV = g_MyD3D->MyStg->Check_Contact(vPos.x, vPos.y, 0, IsTagMotion(), &m_iContactX, &m_iContactY);
                        if (staticV.y != -1.0f)
                        {
                            if (!Old_IsContact)
                                Sound(0);
                            IsContact = true;
                        }
                    }

                    //방금 착지했음
                    if (IsContact && !Old_IsContact)
                    {
                        if (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_DOWN_IN_SKY))
                        {
                            Sound(49);
                        }

                        // 현재 모션이 WAIT 계열이면 먼지 안하기
                        if (Old_Motion != GetWaitMotion() && Old_Motion != GetFatalWaitMotion() &&
                            uiMotion != GetOwnMotion(MID_COMMON_INTO_THE_TELEPORT) && GetOwnMotion(MID_COMMON_INTO_TELEPORT) != uiMotion)
                        {
                            EffectDust();
                        }
                        m_fPhysicAtkPower = 0.0f;
                    }
                    if ((Old_Motion != GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) && Old_Motion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) && Old_Motion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)) &&
                        (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) || uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)))
                    {
                        Sound(49);

                        // 현재 모션이 WAIT 계열이면 먼지 안하기
                        if (Old_Motion != GetWaitMotion() && Old_Motion != GetFatalWaitMotion() &&
                            uiMotion != GetOwnMotion(MID_COMMON_INTO_THE_TELEPORT) && GetOwnMotion(MID_COMMON_INTO_TELEPORT) != uiMotion)
                        {
                            EffectDust();
                        }
                        m_fPhysicAtkPower = 0.0f;
                    }
                }
            }
            pLatency++;
        }
        else// 나 이외의 다른 이의 데이터가 늦게와서 받았던 버퍼를 다 써버렸음. 임의로 보간한다.
        {
            // 2007.10.16 wony
            // 보간하는 중에 프레임을 그냥 알아서 증가시키는것 때문에 렉필이 발생한다..
            // 필살기 도중엔 보간 하지 않아!!
            if (!IsSpecialSkillMotion())
            {
                if (IsSkipFrameTime())
                    goto FrameSkipped;

                /**************** 보간하는 부분 !!! 중요 *******************/
                if (Slow_Count > 0)
                    goto FrameSkipped;// 여러번 맞는 일을 막기 위해 자체적으로 Slow_Count를 쓴다.
                // 뒤집기 중일 때는 일체의 컨텍트 처리를 하지 않음
                if (IsCatchMotion(uiMotion) && cFrame < 45)
                {
                    x_Speed = 0.0f;
                    y_Speed = 0.0f;
                }
                else
                {
                    // 컨텍트 처리
                    IsContact = false;
                    if (SiKGCRoomManager()->IsMonsterGameMode())
                    {
                        KGCContectInfo kInfo(vPos.x, vPos.y, x_Speed, y_Speed, 0, IsTagMotion());
                        if (true == g_MyD3D->MyStg->Check_Contact(kInfo))
                        {
                            if (kInfo.m_ySpeed <= 0.0f)
                            {
                                if (y_Speed <= 0)//&& !Gravity)
                                {
                                    IsContact = true;
                                    m_fPhysicAtkPower = 0.0f;
                                    vPos.y = kInfo.m_y;
                                    kInfo.m_ySpeed = 0;
                                }
                            }
                        }
                        vPos.x = kInfo.m_x;
                        x_Speed = kInfo.m_xSpeed;
                        y_Speed = kInfo.m_ySpeed;
                    }
                    else
                    {
                        D3DXVECTOR3 staticV = g_MyD3D->MyStg->Check_Contact(vPos.x, vPos.y, 0, IsTagMotion(), &m_iContactX, &m_iContactY);
                        if (staticV.y != -1.0f)
                        {
                            if (y_Speed <= 0)//&& !Gravity)
                            {
                                IsContact = true;
                                m_fPhysicAtkPower = 0.0f;
                                vPos.y = staticV.y;
                                y_Speed = 0;
                                ContactBlock = (char)staticV.z;
                            }
                        }
                    }

                    // 조금 다르므로 주의할 것...
                    if (!IsContact)
                    {
                        vPos.y += y_Speed;//+Gravity;
                        if (vPos.y < -0.7f)             vPos.y = -0.7f;
                        if (y_Speed > 0.0)            y_Speed -= DEAULT_FALL_SPEED;
                        else                           y_Speed -= m_fFall_Speed;
                        if (y_Speed < -0.03f)           y_Speed = -0.03f;
                    }
                    vPos.x += x_Speed;

                    // 도약 중에는 x_Speed가 조정되지 않음....
                    if (uiMotion != GetCurFormTemplate().JUMP_READY)
                    {
                        vPos.x += x_Speed;

                        if (SiKGCRoomManager()->IsMonsterGameMode())
                        {
                            if (x_Speed > 0.0004f)
                                x_Speed -= 0.0005f;
                            else if (x_Speed < -0.0004f)
                                x_Speed += 0.0005f;
                            if (x_Speed < 0.0005f && x_Speed > -0.0005f)
                                x_Speed = 0;
                            if (uiMotion == GetWaitMotion() || uiMotion == GetFatalWaitMotion())
                                x_Speed = 0;
                        }
                        else
                        {
                            if (ContactBlock == 4 && IsContact)
                            {
                                if (x_Speed > 0.00009f)
                                    x_Speed -= 0.0001f;
                                else if (x_Speed < -0.00009f)
                                    x_Speed += 0.0001f;
                                if (x_Speed < 0.0001f && x_Speed > -0.0001f)
                                    x_Speed = 0;
                            }
                            else
                            {
                                if (x_Speed > 0.0004f)
                                    x_Speed -= 0.0005f;
                                else if (x_Speed < -0.0004f)
                                    x_Speed += 0.0005f;
                                if (x_Speed < 0.0005f && x_Speed > -0.0005f)
                                    x_Speed = 0;
                                if (uiMotion == GetWaitMotion() || uiMotion == GetFatalWaitMotion())
                                    x_Speed = 0;
                            }
                        }
                    }
                }

                ++cFrame;
                GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion(uiMotion);

                if (pMotion)
                {
                    if (uiMotion == GetCurFormTemplate().WIN || uiMotion == GetCurFormTemplate().RESULT_LOSE) // 승리 포즈 중이라면...다음 동작으로 넘어가지 않게...
                        if ((int)pMotion->GetNum_Frame() - 1 < cFrame)
                            cFrame = (int)pMotion->GetNum_Frame() - 1;

                    if ((int)pMotion->GetNum_Frame() - 1 < cFrame)
                    {
                        if (IsContact)
                        {
                            if (GetHP() <= 0.f)
                                uiMotion = GetFatalWaitMotion();
                            else uiMotion = GetWaitMotion();
                        }
                        else
                            uiMotion = GetCurFormTemplate().JUMP;
                        m_fPhysicAtkPower = 0.0f;
                        cFrame = 0;
                        Body_Angle = 0;

                        assert(uiMotion != -1);
                    }
                }

                pMotion = g_pGraphicsHelper->GetPlayerActionMotion(uiMotion);

                if (pMotion)
                {
                    if (pMotion->GetNum_Frame() > cFrame)
                    {
                        if (bIsRight)
                            vPos.x += pMotion->GetFrameData(cFrame).Plus_x * CHAR_SCALE_RATIO;
                        else
                            vPos.x -= pMotion->GetFrameData(cFrame).Plus_x * CHAR_SCALE_RATIO;
                    }
                }
                /**************** 보간하는 부분 !!! 끝 *******************/
            }
        }

        if (vPos.y<-.2f && vPos.y>-.23f)
            Ah_Sound(1);
    }

    // 그래비티값이 0가 될때까지...
    /*
    if(Gravity>0)
    {
    Gravity -= 0.001f;
    if(Gravity < 0)
    Gravity = 0;
    }
    */

    // 지금 Motion이 이전 프레임과 다르며 Super값이 255라면...?
    // 석궁의 1단계 필살기 방향 전환시 무적이 풀리는 현상을 막기위하여 하드코딩 추가
    if (Old_Motion != uiMotion && Super == 255 && uiMotion != MID_LIRE2_SPECIAL1_UP)
        Super = 0;
    if (Super > 0 && Super < 255)
        Super--;

    //=================Frame Data 처리부분=================================//
    // 이전 Motion과 현재 Motion이 같다면...
    int iStartFrame = 0;
    int iEndFrame = 0;
    if (Old_Motion == uiMotion)
    {
        m_bMotionChanged = false;
        if (Old_Frame < cFrame)
        {
            iStartFrame = Old_Frame + 1;
            iEndFrame = cFrame + 1;
            m_pObject->ChildFrameMove();
        }
    }
    else
    {
        // 같은 모션으로 Critical, Double 표시하려고
        Before_Motion = Old_Motion;

        iStartFrame = 0;
        if (uiMotion == (UINT)(MID_LIRE2_DASHATK))
        {
            if (cFrame > 22)
            {
                iStartFrame = cFrame;
            }
        }
        else if (uiMotion == (UINT)(MID_LIRE2_SPECIAL1_UP))
        {
            iStartFrame = cFrame;
        }
        else if (uiMotion >= (UINT)(MID_LIRE2_ATK1_1) && uiMotion <= (UINT)(MID_LIRE2_ATK3_5))//석궁의 콤보
        {
            if (cFrame > 10)
            {
                iStartFrame = cFrame - 10;
            }
        }
        iEndFrame = cFrame + 1;
        m_pObject->ChildFrameMove();

#if defined( USE_FACE_LIFT )
        // 표정변화 딜레이타임 초기화
        ResetFaceLiftDelay();
#endif
    }

    Old_Motion = uiMotion;
    Old_Frame = cFrame;

#if !defined(__PATH__)
    m_iFrame = static_cast<int>(cFrame);
#endif

    //공통 모션을 사용하지 않는 케릭터일 경우에 모션은 다르더라도 공통모션의 FrameInfoProc을 사용합니다.
    UINT iOriginalMotion = (UINT)GetOwnMotion(uiMotion, true);
    GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion(iOriginalMotion);

    unsigned char  cInfo = 0;
    unsigned short Reserved1 = 0;
    unsigned short Reserved2 = 0;

#if defined( USE_FACE_LIFT )
    if (m_bIsBurnningMode && GetCurrentChar().iCharType == GC_CHAR_RONAN)
        SetRenderFaceLift(FACE_LIFT_BURNMODE);
    else
        SetRenderFaceLift(m_iDefaultFaceLiftType);
#endif

    // 이걸로 한번 모션에 대한 검증을 해 주자.. 별로 효과가 있을것 같진 않지만..
    if (pMotion != NULL && pMotion->IsLoaded())
    {
        for (int i = iStartFrame; i < iEndFrame; i++)
        {
            SFrame& kFrame = pMotion->GetFrameData(i);
            cInfo = kFrame.Option;
            Reserved1 = kFrame.Reserved1;
            Reserved2 = kFrame.Reserved2;
            FrameInfoProc(iOriginalMotion, cInfo, Reserved1, Reserved2, i);
            m_iProcessedFrame++;
        }

        // iStartFrame와 iEndFrame가 같으면 FrameInfoProc()함수가 호출안되서 
        // 표정갱신이 안되기 때문에 여기서 한번 호출하도록 할께요
        if (iStartFrame == iEndFrame)
            SiKGCFaceLift()->SetFaceLiftByMotionEnum(this, uiMotion, cFrame);
    }

    if (m_pObject)
    {
        m_pObject->SetRender(m_bRender);
        if (m_bShowEffect == true)
            ShowAllItemEffect(m_bRender);
    }

    // 엑스트라 메시 프레임무브
    g_kGlobalValue.m_ExtraMesh.FrameMove(m_iPlayerIndex);

    if (IsFatal())
        SetEPlayerState(PS_FATAL);

    if ((m_ePlayerState == PS_DEAD || m_ePlayerState == PS_FATAL) && GetHP() > 0.0f)
        SetEPlayerState(PS_ALIVE);

FrameSkipped:
    m_QueueParticleList.push(m_ParticleMsg);
    m_ParticleMsg.clear();

    if (m_QueueParticleList.size() > LATENCY - 5)
    {
        ParticleQueueProcess(m_QueueParticleList.front());
        m_QueueParticleList.pop();
    }

    // 스킵프레임 처리
    if (0 < m_iSkipFrame)          m_iSkipFrame--;
    else                            m_iSkipFrame = m_iResetSkipFrame;
    if (0 < m_iContinueSkipFrame)  m_iContinueSkipFrame--;
    else                            m_iResetSkipFrame = 0;


}

void PLAYER::FrameMoveItemEffect()
{
    PROFILE_SET("PLAYER::FrameMoveItemEffect");

    //  1초마다 정리한다.
    static DWORD dwLastTick = 0;
    const int nTICK_TIME = 1000 * 1;    //  1초

    DWORD dwCurTick = GetTickCount();

    if (dwCurTick - dwLastTick < nTICK_TIME)
        return;

    dwLastTick = dwCurTick;

    //  ItemEffect
    {
        size_t nVecSize = m_vecItemEff.size();

        for (size_t i = 0; i < nVecSize; ++i)
        {
            std::vector<CParticleEventSeqPTR> vecSeq;

            size_t nSeqSize = m_vecItemEff[i].pSeq.size();
            vecSeq.reserve(nSeqSize);

            for (size_t j = 0; j < nSeqSize; ++j)
            {
                if (m_vecItemEff[i].pSeq[j])
                {
                    vecSeq.push_back(m_vecItemEff[i].pSeq[j]);
                }
            }

            m_vecItemEff[i].pSeq.swap(vecSeq);
        }
    }

    //  DamageEffect
    {
        size_t nVecSize = m_vecDamageEff.size();

        for (size_t i = 0; i < nVecSize; ++i)
        {
            std::vector<CParticleEventSeqPTR> vecSeq;

            size_t nSeqSize = m_vecDamageEff[i].pSeq.size();
            vecSeq.reserve(nSeqSize);

            for (size_t j = 0; j < nSeqSize; ++j)
            {
                if (m_vecDamageEff[i].pSeq[j])
                {
                    vecSeq.push_back(m_vecDamageEff[i].pSeq[j]);
                }
            }


            m_vecDamageEff[i].pSeq.swap(vecSeq);
        }
    }


    //  SkillEffect
    {
        size_t nVecSize = m_vecSkillEff.size();

        for (size_t i = 0; i < nVecSize; ++i)
        {
            std::vector<CParticleEventSeqPTR> vecSeq;

            size_t nSeqSize = m_vecSkillEff[i].pSeq.size();
            vecSeq.reserve(nSeqSize);

            for (size_t j = 0; j < nSeqSize; ++j)
            {
                if (m_vecSkillEff[i].pSeq[j])
                {
                    vecSeq.push_back(m_vecSkillEff[i].pSeq[j]);
                }
            }

            m_vecSkillEff[i].pSeq.swap(vecSeq);
        }
    }
}

void PLAYER::SetEPlayerState(EPlayerState eState)
{
    PROFILE_SET("PLAYER::SetEPlayerState");

    {
        static int iFatalFrameCount = 0;
        if (IsLocalPlayer() && eState == PS_FATAL && iFatalFrameCount == 3) {
            SiKGCAchieveManager()->OccurAction(SAchieve::FATAL, GetCharType());
        }

        if (eState == PS_FATAL) {
            ++iFatalFrameCount;
        }
        else {
            iFatalFrameCount = 0;
        }
    }

    //  State 변경시 처리해야 하는 작업
    if (m_ePlayerState != eState)
    {
        switch (eState)
        {
        case PS_ALIVE:
            ReSetFatalAttriButeEffect();
            break;
        case PS_FATAL:
            SetFatalAttriButeEffect();
            break;
        case PS_DEAD:
            //죽을 때 오오라 다 없애주자.
            InitRangeInfo();
            SiKGCSubjectManager()->AddDieCnt();
            break;
        }
    }

    m_ePlayerState = eState;

    if (m_ePlayerState == PS_DEAD)
    {
        if (m_pObject)
            m_pObject->MeshStateOn(MS_DEAD);
        if (g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject)
            g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject->MeshStateOn(MS_DEAD);
    }
    else if (m_ePlayerState == PS_ALIVE || m_ePlayerState == PS_FATAL)
    {
        if (m_pObject)
            m_pObject->MeshStateOff(MS_DEAD);
        if (g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject)
            g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject->MeshStateOff(MS_DEAD);
    }
}
bool PLAYER::IsCatchMotion(UINT Motion, bool bOnlyCatch/* = false*/)
{
    // CATCH의 초기값이 0 이라서 ㅡㅅ- 0이면 엘리시스 대기모션이기도 하고 해서 그냥 검사안하고 false 반환
    if (Motion == 0 || Motion == 1)
        return false;

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->GetPlayer(i)->GetCurFormTemplate().CATCH == Motion || MID_SIEG4_CATCH_FURY == Motion)
            return true;
        if (g_MyD3D->GetPlayer(i)->GetCurFormTemplate().CATCH + 1 == Motion || MID_SIEG4_CATCHED_FURY == Motion)
            return !bOnlyCatch;
    }

    return false;
}

bool PLAYER::IsNoCrashCheckMotion(int iMotion)
{
    //이제 이거 쓰지 말고 m_bCrashCheck 변수 셋팅하세요~~~
    return(iMotion == MID_LIRE2_DASH ||
        iMotion == MID_LIRE2_DASHATK ||
        iMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
        iMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
        iMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) ||
        iMotion == MID_LIRE_JUMPREADY ||
        iMotion == MID_LIRE_JUMP ||
        iMotion == GetOwnMotion(MID_COMMON_STONECURSE) ||
        iMotion == MID_LAS_EVASION ||
        iMotion == GetOwnMotion(MID_COMMON_ARENA) ||
        iMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
        iMotion == MID_LIRE3_SPECIAL3 ||
        // TagMotion
        IsTagMotion() ||
        IsCatchMotion(iMotion) ||
        iMotion == MID_SKILL_ARME1_W2_BASE_SPECIAL_LV2_READY ||
        iMotion == MID_SKILL_ARME1_WHITE2_BASE_SPECIAL_LV2);
    //이제 이거 쓰지 말고 m_bCrashCheck 변수 셋팅하세요~~~
}

void PLAYER::PushCheckWithDamage()
{
    if (m_bNoCrashCheck)
        return;

    g_MyD3D->m_pDamageManager->PushCheckWithPlayer(m_iPlayerIndex);
}
//상대방에게 밀고 밀림처리. 잡기도 처리함
void PLAYER::PushCheckWithOtherPlayers()
{
    //사람에게 밀리지 않는 모션들을 등록하세요
    if (m_bNoCrashCheck || !m_bCrashCheck || IsNoCrashCheckMotion(uiMotion) || m_bNoCrashCheckByBuff)
        return;

    // 내가 탑승물 승차시 충돌처리 하지말자!
    if (UINT_MAX > GetEmbarkUID())
        return;

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (g_MyD3D->IsSameTeam(i, m_iPlayerIndex))
            continue;

        // 탑승물 탑승자는 충돌처리 하지말자!
        if (UINT_MAX > g_MyD3D->MyPlayer[i]->GetEmbarkUID())
            continue;

        //게임 종료상태이거나 연결이 끈어졌다면
        if (!g_MyD3D->MyPlayer[i]->m_cLife || g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING)
            continue;

        //상대방 모션이 나를 밀 수 없는 모션일때
        if (!g_MyD3D->MyPlayer[i]->m_bCrashCheck || g_MyD3D->MyPlayer[i]->IsNoCrashCheckMotion(g_MyD3D->MyPlayer[i]->uiMotion))
            continue;

        if (vPos.y + 0.1f > g_MyD3D->MyPlayer[i]->vPos.y && vPos.y - 0.1f < g_MyD3D->MyPlayer[i]->vPos.y)
            //&& !Item_Remain_Time[GC_GAME_ITEM_SUPER]) // 무적일때 잡기 안되는 컨셉일거 같은데..
        {
            if (vPos.x + 0.15f > g_MyD3D->MyPlayer[i]->vPos.x &&
                vPos.x - 0.15f < g_MyD3D->MyPlayer[i]->vPos.x)
            {
                if (vPos.x > g_MyD3D->MyPlayer[i]->vPos.x)
                {
                    // 적의 오른쪽 침범 - 왼쪽으로 보고 있어야함
                    if (vPos.x < Old_x)
                        vPos.x = g_MyD3D->MyPlayer[i]->vPos.x + 0.16f;

                    if (bIsRight)
                        continue;

                    if (!diks[g_MyD3D->MyCtrl->m_keyLeft])
                        continue;
                }
                else
                {
                    // 적의 왼쪽 침범 - 오른쪽으로 보고 있어야함
                    if (vPos.x > Old_x)
                        vPos.x = g_MyD3D->MyPlayer[i]->vPos.x - 0.16f;

                    if (!bIsRight)
                        continue;

                    if (!g_MyD3D->MyCtrl->k_Right)
                        continue;
                }

                if (m_iCannotCatchFrame != 0)
                    continue;

                // 2007/2/16. iridology. 네피림 잡히기 안됨
                if (g_MyD3D->MyPlayer[i]->MetamorphosisForm == FORM_NEPHILIM)
                    continue;

                // 2008/04/03. Jaeho. 매그너스 단 잡히기 안됨
                if (g_MyD3D->MyPlayer[i]->MetamorphosisForm == FORM_MDAN)
                    continue;

                if (g_MyD3D->MyPlayer[i]->MetamorphosisForm == FORM_RIN_GODDESS || g_MyD3D->MyPlayer[i]->MetamorphosisForm == FORM_RIN_EVIL)
                    continue;

                // 2007.03.26 wony
                // 무적이면 잡기 안되도록 합시다
                if (g_MyD3D->MyPlayer[i]->Item_Remain_Time[GC_GAME_ITEM_SUPER])
                    continue;

                // 뒤집기 동작 들어가기
                if (g_MyD3D->MyPlayer[i]->Super != 0 || cFrame >= 3)
                    continue;

                if (uiMotion != GetCurFormTemplate().ATK1 &&
                    uiMotion != GetCurFormTemplate().DASH_ATK &&
                    !IsCatchableMotion(uiMotion) &&
                    !m_bIsCatchableMotion)
                    continue;

                g_MyD3D->MyPlayer[i]->SetLastKillmePlayer(m_iPlayerIndex);
                // 잡기할때,, HASTE푼다.. 잡혔는데 억울하쟈나;; 
                g_MyD3D->MyPlayer[i]->Item_Remain_Time[GC_GAME_ITEM_HASTE] = 0;

                if (g_MyD3D->MyPlayer[i]->uiMotion == MID_SIEG4_CATCHED_FURY)
                    SetPlayerState(MID_SIEG4_CATCHED_FURY);
                else
                    SetPlayerState(GetCurFormTemplate().CATCH);
                Body_Angle = 0.001f * i; //이수치로 잡은 플레이어 정보를 전송
                cFrame = 0;
            }
        }
    }
}

void PLAYER::PushCheckWithMonsters()
{
    //아레나 모션일때 밀리지 마라
    if (uiMotion == GetOwnMotion(MID_COMMON_ARENA))
        return;

    //죽으려고 넘어질때는 충돌처리 하지말것!
    if (GetHP() == 0.0f &&
        (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) ||
            uiMotion == GetOwnMotion(MID_COMMON_DOWN_IN_SKY)))
        return;

    KGCEmbarkObject* pEmbark = NULL;
    if (UINT_MAX > GetEmbarkUID())
    {
        pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark && false == pEmbark->IsPushCheck())
            return;
    }

    std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin();
    for (; mit != g_kMonsterManager.m_mapMonster.end(); ++mit)
    {
        MONSTER* pMonster = mit->second;
        if (!pMonster->m_bLive)
            continue;

        if (pMonster->m_fHP <= 0.0f)
            continue;

        const SMonsterTemplate& sMonsterTable = g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType];

        if (m_bNoCrashCheckByBuff && sMonsterTable.bNoPassMonster == false) {
            continue;
        }

        // PushCheck를 하지않도록 셋팅된 몬스터는 PushCheck를 하지 않는다.
        if (sMonsterTable.bNoPushCheck && pMonster->m_bCollisionCheck == false)
            continue;

        if ((!m_bCrashCheck || m_bNoCrashCheck) && false == sMonsterTable.bNoPassMonster)
        {
            if (pMonster->m_iMonsterType != MON_SWAMP_NESSI &&
                pMonster->m_iMonsterType != MON_PATUPLAT_GOLEM)
                continue;
        }

        // 우리팀이 소환한 몬스터는 충돌체크를 하지 않는다.
        if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
            continue;

        float fMonsterWidth = pMonster->m_fWidth;
        float fMonsterHeight = pMonster->m_fHeight;

        for (int iLoop = 0; iLoop < pMonster->GetPushRectNum(); ++iLoop)
        {
            if (false == pMonster->GetPushCheck(iLoop))
                continue;

            GCCollisionRect<float> kPlayerBody, kColl;
            GCCollisionRect<float> kMonRect = pMonster->GetPushBoundRect(iLoop);
            kMonRect.m_Bottom -= 0.1f;

            // 무한대값이 되서 아무것도 없는데 Push가됨. 아래는 체크해보는 코드임.
            // 루아에서 점프시 0으로 나누어지는 경우가 있어 Speed 설정하면서 문제발생함
            //if( 0 == _finite( kMonRect.m_Left ) || 0 == _finite( kMonRect.m_Right ) || 
            //    0 == _finite( kMonRect.m_Top ) || 0 == _finite( kMonRect.m_Bottom ) )
            //{
            //    continue;
            //}

            if (NULL == pEmbark)
                kPlayerBody.SetRect(vPos.x - 1.0f, vPos.y - 0.25f + y_Ex, vPos.x - 1.0f, vPos.y - 0.25f + y_Ex);
            else
            {
                kPlayerBody = pEmbark->GetPushRect();
                kPlayerBody.m_Top += 0.25f;   kPlayerBody.m_Bottom += 0.25f;
            }

            if (kPlayerBody.CheckCollision(kMonRect, &kColl))
            {
                float fPlayerHalfWidth = (kPlayerBody.m_Right - kPlayerBody.m_Left) / 2.0f;
                float fMonsterX = kMonRect.GetCenterX() + 1.0f;

                // 몬스터를 절대 지나갈 수 없도록 되어있을땐 아래 처리를 하지 않는다.
                if (false == sMonsterTable.bNoPassMonster)
                {
                    if (uiMotion == MID_ARME_ARME2_DASH)
                    {
                        uiMotion = MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER;
                        continue;
                    }
                    else if (uiMotion == MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER ||
                        uiMotion == MID_LAS_EVASION)
                    {
                        //아르메가 순간이동시에 가도센에게 밀리는것은 약하게 조금씩 밀릴뿐....
                        //완전히 밀리지는 않는다.
                        if (fMonsterX > vPos.x)
                            vPos.x -= 0.03f;
                        else
                            vPos.x += 0.03f;
                        break;
                    }
                    else if (pMonster->m_iMonsterType == MON_KING_CRAB && pMonster->GetState() != "ATTACK02")
                    {
                        if (pEmbark)
                        {
                            D3DXVECTOR3 vEmbarkPos(pEmbark->GetPosX(), pEmbark->GetPosY(), pEmbark->GetPosZ());
                            if (!pMonster->GetIsRight())
                                vEmbarkPos.x = kMonRect.m_Left + 1.0f;
                            else
                                vEmbarkPos.x = kMonRect.m_Right + 1.0f;
                            pEmbark->SetPos(vEmbarkPos.x, vEmbarkPos.y, vEmbarkPos.z);
                        }
                        else
                        {
                            if (!pMonster->GetIsRight())
                                vPos.x = kMonRect.m_Left + 1.0f;
                            else
                                vPos.x = kMonRect.m_Right + 1.0f;
                        }
                        break;
                    }
                    else if (pMonster->m_iMonsterType == MON_DRAKE_WORM)
                    {
                        if (pEmbark)
                        {
                            D3DXVECTOR3 vEmbarkPos(pEmbark->GetPosX(), pEmbark->GetPosY(), pEmbark->GetPosZ());
                            if (!pMonster->GetIsRight())
                                vEmbarkPos.x = kMonRect.m_Left + 1.0f;
                            else
                                vEmbarkPos.x = kMonRect.m_Right + 1.0f;
                            pEmbark->SetPos(vEmbarkPos.x, vEmbarkPos.y, vEmbarkPos.z);
                        }
                        else
                        {
                            if (!pMonster->GetIsRight())
                                vPos.x = kMonRect.m_Left + 1.0f;
                            else
                                vPos.x = kMonRect.m_Right + 1.0f;
                        }
                        break;
                    }
                }

                float fBefore, fAfter, fY;
                if (pEmbark)
                {
                    fAfter = fBefore = pEmbark->GetPosX();
                    fY = pEmbark->GetPosY();
                    if (kMonRect.GetCenterX() > kPlayerBody.GetCenterX())
                        fAfter = kMonRect.m_Left + 1.0f - fPlayerHalfWidth;
                    else
                        fAfter = kMonRect.m_Right + 1.0f + fPlayerHalfWidth;
                }
                else
                {
                    fAfter = fBefore = vPos.x;
                    fY = vPos.y;
                    if (kMonRect.GetCenterX() > kPlayerBody.GetCenterX())
                        fAfter = kMonRect.m_Left + 1.0f - fPlayerHalfWidth;
                    else
                        fAfter = kMonRect.m_Right + 1.0f + fPlayerHalfWidth;
                }

                // 사이벽 벽처리
                float fAfterWall = fAfter;
                if (fBefore < fAfterWall)
                {
                    fBefore -= FOOTHOLDSIZE;
                    fAfterWall += FOOTHOLDSIZE;
                }
                else
                {
                    fBefore += FOOTHOLDSIZE;
                    fAfterWall -= FOOTHOLDSIZE;
                }
                if (false == SiKGCFantasticMap()->CheckPierceBlockXPos(fBefore, fAfterWall, fY))
                    fAfterWall = fAfter;

                if (pEmbark)
                    pEmbark->SetPos(fAfterWall, pEmbark->GetPosY(), pEmbark->GetPosZ());
                else
                    vPos.x = fAfterWall;

                break;
            }
        }

    }

    //}
}

void PLAYER::PushCheckWithObjects()
{
    //아레나 모션일때 밀리지 마라
    if (m_bNoCrashCheck || uiMotion == GetOwnMotion(MID_COMMON_ARENA))
        return;

    //죽으려고 넘어질때는 충돌처리 하지말것!
    if (GetHP() == 0.0f &&
        (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) ||
            uiMotion == GetOwnMotion(MID_COMMON_DOWN_IN_SKY)))
        return;

    if (m_bCrashCheck == false)
        return;

    for (Itor2DObject itor = g_MyD3D->Begin2DObject(); itor != g_MyD3D->End2DObejct(); ++itor)
    {
        (*itor)->PushCheck(vPos.x, vPos.y + y_Ex, IsNoCrashCheckMotion(uiMotion));
    }

    for (int i = 0; i < MAX_OBJECT; ++i)
    {
        if (!g_MyD3D->m_pObject[i]) continue;
        KGCGameObject* pObject = g_MyD3D->m_pObject[i];
        if (!pObject->IsLive()
            || !pObject->IsCrashCheck()
            || !m_bCrashCheck
            )
            continue;

        // Jaeho 2007.12.06
        // 마법의 숫자가 들어간다... CKTDGAnim::CAnimInstance 이라 좌표가 마음대로
        // 움직이지 않아 이렇게 코딩하기로 결심하였다 --;
        // 나중에 Object가 텍스쳐.. 혹은 뭘로 구현될지 모르겠는데 그때 다시 정정해야할 듯...
        // -0.2f를 해준건 문 높이가 높아서 플레이어들이 뛰어넘기 힘들다... 그래서 높이 체크를 조금 낮췄음.
        if ((pObject->GetPosY() + pObject->GetHeight() > vPos.y + y_Ex)
            && (pObject->GetPosY() < vPos.y + y_Ex)
            )
        {
            float fObjectLeft = pObject->GetPosX() - pObject->GetWidth() * 0.5f - 0.1f;
            float fObjectRight = pObject->GetPosX() + pObject->GetWidth() * 0.5f - 0.2f;
            if ((fObjectRight > vPos.x)
                && (fObjectLeft < vPos.x))
            {
                if ((fObjectLeft + fObjectRight) / 2.0f > vPos.x)
                    vPos.x = fObjectLeft;
                else
                    vPos.x = fObjectRight;
            }
        }
    }
}

void PLAYER::PushCehckWithMinigameObj()
{
    if (m_bNoCrashCheck) return;
    SiKGCMinigameManager()->PushCheck(!m_bCrashCheck);
}

void PLAYER::Crash_Check()
{
    PROFILE_SET("PLAYER::Crash_Check");

    // 파워가 있다면 적과의 충돌을 검사한다.
    GCCollisionRect<float> rtWeapon;
    if (m_kUserInfo.nUserState != GC_RUS_PLAYING)
        return;

    if (IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()))
    {
        if (!Slow_Count && !IsDetermineWin && !g_pkQuestManager->m_bClearStage)
        {
            PushCheckWithOtherPlayers();
            PushCheckWithMonsters();
            PushCheckWithObjects();
            PushCehckWithMinigameObj();
            PushCheckWithDamage();
        }
    }

    //=======================================화면 좌우로 못나가게 함
    if (!IsTagMotion())
    {
        if (vPos.x > g_MyD3D->MyStg->GetXLimit() - 0.1f)
        {
            vPos.x = g_MyD3D->MyStg->GetXLimit() - 0.1f;
            x_Speed = 0;
        }
        else if (vPos.x < 0.05f)
        {
            vPos.x = 0.05f;
            x_Speed = 0;
        }
    }
    Old_x = vPos.x;
    //=======================================화면 좌우로 못나가게 함---끝


    if (m_ePhysicAtkType == PAT_SWORD)
    {
        rtWeapon.SetRect(MyArmor.x, MyArmor.y, MyArmor_Ex.x, MyArmor_Ex.y);
    }
    else if (m_ePhysicAtkType == PAT_LEG_LEFT)
    {
        rtWeapon.SetRect(m_posDesc.m_BoneLegLeftTip.x, m_posDesc.m_BoneLegLeftTip.y, m_posDesc.m_BoneLegLeftTip.x, m_posDesc.m_BoneLegLeftTip.y);
    }
    else if (m_ePhysicAtkType == PAT_LEG_RIGHT)
    {
        rtWeapon.SetRect(m_posDesc.m_BoneLegRightTip.x, m_posDesc.m_BoneLegRightTip.y, m_posDesc.m_BoneLegRightTip.x, m_posDesc.m_BoneLegRightTip.y);
    }
    else if (m_ePhysicAtkType == PAT_SWORD_LEFT)
    {
        rtWeapon.SetRect(MyLeftArmor.x, MyLeftArmor.y, MyLeftArmor_Ex.x, MyLeftArmor_Ex.y);
    }
    else if (m_ePhysicAtkType == PAT_FIST_RIGHT)
    {
        rtWeapon.SetRect(m_posDesc.rightHandBody.x, m_posDesc.rightHandBody.y, m_posDesc.rightHandTip.x, m_posDesc.rightHandTip.y);
    }
    else if (m_ePhysicAtkType == PAT_FIST_LEFT)
    {
        rtWeapon.SetRect(m_posDesc.leftHandBody.x, m_posDesc.leftHandBody.y, m_posDesc.leftHandTip.x, m_posDesc.leftHandTip.y);
    }

    m_bUsePower = false;

    if (m_fPhysicAtkPower)
    {
        if (Attack_Damage(&rtWeapon))
            m_fPhysicAtkPower = 0.0f;

        bool UsePower1 = Attack_Monster(&rtWeapon);
        bool UsePower2 = Attack_Player(&rtWeapon);
        bool UsePower3 = Attack_Object(&rtWeapon);
        bool UsePower4 = Attack_MinigameObj(&rtWeapon);


        if (UsePower1 || UsePower2 || UsePower3 || UsePower4)
        {
            m_fPhysicAtkPower = 0.0f;
        }
    }

    //// 최신 정보를 레이턴시로 보낸다.(내부적으로 MyID인 경우만 동작하게 되어있음)
    UpdateData();

    Process_Face();
}

void PLAYER::ProcessBackStepBuff()
{
    bool isBackStep = g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP) != 0;
    bool isBackStepDun = g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP_DUN) != 0;
    bool isBackStepAppear = g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP_APPEAR_DUN) != 0;

    if (isBackStepDun == true)
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP_DUN);
        g_MyD3D->MyPlayer[m_iPlayerIndex]->Item_Remain_Time[GC_GAME_ITEM_HALLOW] = 0;

        g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP_APPEAR_DUN, 5.0f);
    }

    if (isBackStepAppear == true)
    {
        g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP_APPEAR_DUN, 3.0f);
    }

    if (isBackStep == true)
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP);
        g_MyD3D->MyPlayer[m_iPlayerIndex]->Item_Remain_Time[GC_GAME_ITEM_HALLOW] = 0;

        // 3초간 공격력 증가!
        g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_BACKSTEP_APPEAR, 3.0f);
    }
}

GCCollisionRect<float>& PLAYER::GetCollisionRect()
{
    m_kCollisionRect.SetRect(MyBody.x, MyBody.w, MyBody.y, MyBody.z);
    //m_kCollisionRect.AdjustRect();
    return m_kCollisionRect;
}


bool PLAYER::OnDamagePreProcess(int who, bool bPhyAtk, bool bMonsterDamage, bool bSpecial, int iWhat, int iHitWho)
{
    PLAYER* pPlayer = g_MyD3D->MyPlayer[m_iPlayerIndex];

    if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_HOLY_LV3_SHIELD))
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_HOLY_LV3_SHIELD);
        pPlayer->Super = 10;
        pPlayer->Slow_Count = 7;

        return false;
    }

    if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_RENCIA_SHIELD))
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RENCIA_SHIELD);
        pPlayer->Super = 10;
        pPlayer->Slow_Count = 7;

        return false;
    }
    else if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_CHOUCRING_SHIELD))
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_CHOUCRING_SHIELD);
        pPlayer->Super = 10;
        pPlayer->Slow_Count = 7;

        return false;
    }

    else if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_DOKKAEBIE_SHIELD))
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DOKKAEBIE_SHIELD);
        pPlayer->Super = 10;
        pPlayer->Slow_Count = 7;
        g_MyD3D->m_pDamageManager->SetDamageLife(DT_PET_DOKKAEBI_ATTACK3, 1);
        return false;
    }
    //====================================================================================
    // 2009.03.09 : Jemitgge
    // Comment : 라스.. 분신술... 아 좀.. ㅠ 분신술 상태에서 맞았을때는,, 데미지 처리 안한다. 
    //			 펑 하고 사라병鳴?대각선 상단에서 나타난다! 
    if (iHitWho > -1)
    {
        if (g_MyD3D->MyPlayer[iHitWho]->IsMagicEffect(EGC_BUFF_CLONE_ATK) || g_MyD3D->MyPlayer[iHitWho]->IsMagicEffect(EGC_EFFECT_EDEL2_SPECIAL_C_TELEPORT))
        {
            if (false == g_MyD3D->MyPlayer[iHitWho]->m_bJumpDashed)
            {
                D3DXVECTOR3 vTempPos = g_MyD3D->MyPlayer[iHitWho]->GetPosition();
                if (false == g_MyD3D->MyPlayer[iHitWho]->bIsRight)
                {
                    vTempPos.x -= 0.35f;
                }
                else if (true == g_MyD3D->MyPlayer[iHitWho]->bIsRight)
                {
                    vTempPos.x += 0.35f;
                }
                vTempPos.y += 0.55f;
                g_MyD3D->MyPlayer[iHitWho]->SetDirectMove(vTempPos.x, vTempPos.y);
                g_MyD3D->MyPlayer[iHitWho]->NoCheckContact = 35;
                g_MyD3D->MyPlayer[iHitWho]->m_bJumpDashed = true;
            }

            if (g_MyD3D->MyPlayer[iHitWho]->cFrame == (int)g_MyD3D->MyPlayer[iHitWho]->GetNowMotionFRM()->GetNum_Frame() - 1)
            {
                g_MyD3D->MyPlayer[iHitWho]->m_bJumpDashed = false;
            }

            g_MyD3D->MyPlayer[iHitWho]->bIsRight = !g_MyD3D->MyPlayer[iHitWho]->bIsRight;
            g_MyD3D->MyPlayer[iHitWho]->Change_Motion(g_MyD3D->MyPlayer[iHitWho]->GetCurFormTemplate().JUMP);

            if (g_MyD3D->MyPlayer[iHitWho]->IsMagicEffect(EGC_BUFF_CLONE_ATK)) {
                g_pMagicEffect->ClearMagicEffect(iHitWho, EGC_BUFF_CLONE_ATK);
            }

            if (g_MyD3D->MyPlayer[iHitWho]->IsMagicEffect(EGC_EFFECT_EDEL2_SPECIAL_C_TELEPORT)) {
                g_pMagicEffect->ClearMagicEffect(iHitWho, EGC_EFFECT_EDEL2_SPECIAL_C_TELEPORT);
            }
            return false;
        }
        else if (g_MyD3D->MyPlayer[iHitWho]->IsMagicEffect(EGC_EFFECT_EDEL2_SPECIAL_C_MP_RECOVERY) && iHitWho == m_iPlayerIndex && IsLocalPlayer()) {
            if (IsMonsterGameMode()) {
                g_MyD3D->MyHead->Level += 0.3f;
            }
            else {
                g_MyD3D->MyHead->Level += 0.1f;
            }

            if (g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP())
            {
                g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
            }

        }
        else if (g_MyD3D->MyPlayer[iHitWho]->IsMagicEffect(EGC_EFFECT_EDEL2_SPECIAL_C_DECREASE_COOLTIME) && iHitWho == m_iPlayerIndex) {
            for (std::vector< GCHotKeySkill >::iterator vit = m_vecHotKeySkill.begin(); vit != m_vecHotKeySkill.end(); vit++)
            {
                // 스킬키 쿨타임 확인
                if (1.f <= vit->fCoolTime) {
                    vit->fCoolTime -= 1.f;
                }
            }
        }
    }



    // 저주의조율 오른쪽! 보스가 플레이어 타격 시 1hit당 5%의 피를 채움!
    // 방장이 주관한다.
    if (pPlayer->IsMagicEffect(EGC_EFFECT_JURIOR_CURSE_R) && m_kUserInfo.bHost)
    {
        if (who == -1)return false;
        MONSTER* pMonster = g_kMonsterManager.GetMonster(who);
        if (pMonster != NULL) {
            if (pMonster->m_iMonsterType == MON_JURIOR && iWhat != DT_XENIA5_MAP_DOTDAMAGE)
            {
                pMonster->m_fHP += (pMonster->m_fHPPoint * 0.05f);
                if ((pMonster->m_fHPPoint) < pMonster->m_fHP)
                    pMonster->m_fHP = pMonster->m_fHPPoint;
            }
        }
    }

    if (IsLocalPlayer() && g_MyD3D->MyCtrl->k_Fresh_Double_C && pPlayer->IsContact && pPlayer->uiMotion != MID_COMMON_CATCH_BY_RYAN)
    {
        if (g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_RYAN1_RYAN_NATURESHIELD)
            && false == g_pMagicEffect->IsMagicEffect(pPlayer->m_iPlayerIndex, EGC_EFFECT_RYAN_NATURESHIELD))
        {
            pPlayer->Direct_Motion_Input(MID_SKILL_RYAN_NATURESHIELD);
            pPlayer->cFrame = 0;
            pPlayer->Next_Attack_Count = 0;
            g_MyD3D->MyCtrl->k_Skill_Key = false;
            g_MyD3D->MyCtrl->k_Fresh_Left = false;
            g_MyD3D->MyCtrl->k_Fresh_Right = false;
            KGCPlayerRyan::m_dwSuperTime = timeGetTime();
            pPlayer->ConfirmUseSheild(CID_RYAN1);
            pPlayer->Super = 220;
            pPlayer->x_Speed = 0.0f;
            pPlayer->y_Speed = 0.0f;

            return false;
        }
    }
    if (bPhyAtk && (IsLocalPlayer() || GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())) {

        if (bSpecial) {
            g_pMagicEffect->HitCreateBuffDamage(who, m_iPlayerIndex, NULL, CAHR_ATTACK_TYPE_SPECIAL_DAMAGE);
        }
        else {
            g_pMagicEffect->HitCreateBuffDamage(who, m_iPlayerIndex, NULL, CAHR_ATTACK_TYPE_PHYSIC);
        }
    }

    if (pPlayer->IsLocalPlayer())
    {
#ifdef MATCH_BALANCE_2011_8
        if (pPlayer->uiMotion == MID_JIN4_SPECIAL_STANCE_DEFENCE && pPlayer->GetFrameIndex() <= 40)
#else
        if (pPlayer->uiMotion == MID_JIN4_SPECIAL_STANCE_DEFENCE && pPlayer->GetFrameIndex() < 20)
#endif		
        {
            float fX;
            bool bBack = false;
            if (bMonsterDamage)
            {
                MONSTER* pMonster = g_kMonsterManager.GetMonster(who);
                if (pMonster != NULL) {
                    fX = pMonster->GetX();
                }
            }
            else
            {
                fX = g_MyD3D->MyPlayer[who]->vPos.x;
            }
            if (GetIsRight()) // 오른쪽을 바라보고 있는데
            {
                if (vPos.x > fX) // 날 때린놈이 왼쪽에 있으면 백어택
                    bBack = true;
            }
            else // 왼쪽을 바라보고 있는데
            {
                if (vPos.x < fX) // 날 때린놈이 오른쪽에 있으면 백어택
                    bBack = true;
            }

            if (!bBack)
            {
                pPlayer->SetPlayerState(MID_JIN4_SPECIAL_STANCE_COUNTER);
                pPlayer->cFrame = 0;
                g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_JIN4_COUNTER, 1);
                Super = 3;
                pPlayer->x_Speed = 0.0f;
                pPlayer->y_Speed = 0.0f;
                return false;
            }
        }
    }
    if (bMonsterDamage) {

    }
    return true;
}

bool PLAYER::Attack_Player(GCCollisionRect<float>* rtWeapon)
{
    bool UsePower = false;
    GCCollisionRect<float> rtCollision;

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if (pPlayer->m_kUserInfo.nUserState != GC_RUS_PLAYING)
            continue;

        if (g_MyD3D->IsSameTeam(i, m_iPlayerIndex))
            continue;

        if (g_MyD3D->MyPlayer[i]->IsInvinsible(m_bSpecialAttack) == true)
            continue;

        if (g_MyD3D->MyPlayer[i]->IsSuperNotSpecial(m_bSpecialAttack))
            continue;

        if (pPlayer->IsSpecialSkillMotion()) {
            continue;
        }

        GCCollisionRect<float>& rtBody = pPlayer->GetCollisionRect();

        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(pPlayer->GetEmbarkUID());
        if (pEmbark)
        {
            rtBody = pEmbark->GetAttackRect();
            rtBody.m_Top += 0.25f;    rtBody.m_Bottom += 0.25f;
        }

        if (!rtBody.CheckCollision(*rtWeapon, &rtCollision))
            continue;

        if (false == pPlayer->GetPermissionDamage().empty())
        {
            if ((timeGetTime() - m_dwMissAttackTime) > MISS_ATTACK_TERM_TIME)
            {
                m_dwMissAttackTime = timeGetTime();
                D3DXVECTOR2 hitPos = D3DXVECTOR2(rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f);
                pPlayer->ShowMissAttack(hitPos.x, hitPos.y);
            }
            continue;
        }

        if (pEmbark)
        {
            pEmbark->SetWhatDamage(0);
            pEmbark->CallLuaFunc_Callback(KGCEmbarkTemplate::CALLBACK_CRASH_DAMAGE);
        }

        if (IsLocalPlayer())// 때린 사람이 나라면...
        {
            Next_Attack_Count = 15;
#ifdef LEVEL_DESIGN_STAT_CALC
            g_MyD3D->MyHead->Event_Combo((!pPlayer->IsContact), m_fPhysicAtkPower * 3.f);
#else
            g_MyD3D->MyHead->Event_Combo((!pPlayer->IsContact), m_fPhysicAtkPower * 0.03f);
#endif
            g_pGameBoard->AddAttackCount();
            Slow_Count = m_iHitDelay;

            pPlayer->SetFlashEffectFrame(FLASH_EFFECT_FRAME);
            pPlayer->SetShakeEffectFrame(10);

            if (GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_FURY_MODE && !m_bIsBurnningMode && !IsSpecialSkillMotion())
            {
                CountBP(GetCurFormTemplate().fBPPerAttackPlayer * 3.0f, false);
            }
        }
        if (!pPlayer->OnDamagePreProcess(m_iPlayerIndex, true, false, m_bSpecialAttack, -1, i))
            return false;

        pPlayer->m_vDamageWhoPos = D3DXVECTOR2(GetX(), GetY());

        // 마리2차 매직구슬 버프가 있을 때 타격을 입으면 해당 버프를 삭제한다.
        if (pPlayer->IsMagicEffect(EGC_MARI3_MAGIC_BEAD_MP_RECOVERY))
        {
            pPlayer->ClearMagicEffect(EGC_MARI3_MAGIC_BEAD_MP_RECOVERY);

            if (pPlayer->IsLocalPlayer())
            {
                g_MyD3D->MyHead->Level += 0.5f;
                if (g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP())
                    g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
            }
        }

        // 마리3차 리페어 버프가 있을 때 타격을 입으면 해당 버프를 삭제한다.
        if (pPlayer->IsMagicEffect(EGC_EFFECT_MARI4_REPAIR) ||
            pPlayer->IsMagicEffect(EGC_EFFECT_MARI4_REPAIR_R))
        {
            pPlayer->ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR);
            pPlayer->ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR_R);

            if (pPlayer->IsLocalPlayer())
            {
                g_MyD3D->MyHead->Level += 0.5f;
                if (g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP())
                    g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
            }
        }

        if (pPlayer->IsLocalPlayer() ||
            ((GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())))// 맞은 사람이 나라면...
        {
            // 론한2촤 블로킹!!
            if (pPlayer->MetamorphosisForm == FORM_NORMAL && pPlayer->m_bSpecialAttack == false &&
                (pPlayer->Extra_Char_Num == CID_RONAN3 || pPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME ||
                    (pPlayer->GetCurrentChar().iCharType == GC_CHAR_SIEG && pPlayer->CheckSkill(SID_SIEG1_FURY_TYPE_B_LV3))) &&
                pPlayer->IsDamagedMotion() == false)
            {
                static int b = g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME;
                if (this->bIsRight == false)
                {
                    for (b = 0; b < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++b)
                    {
                        if (g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_RIGHT][b]) break;
                    }
                }
                else
                {
                    for (b = 0; b < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++b)
                    {
                        if (g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_LEFT][b]) break;
                    }
                }

                if (b != g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME)
                {
                    if (pPlayer->GetCurrentChar().iCharType == GC_CHAR_SIEG) {
                        pPlayer->SendBurnningPoint(pPlayer->m_iPlayerIndex);
                        if (pPlayer->GetBurnningPoint() >= 0.5f) {
                            if (pPlayer->Extra_Char_Num == CID_SIEG4) {
                                pPlayer->Direct_Motion_Input(MID_SIEG4_SKILL_GUARD);
                            }
                            else if (pPlayer->Extra_Char_Num == CID_SIEG3) {
                                pPlayer->Direct_Motion_Input(MID_SIEG3_SKILL_GUARD);
                            }
                            else if (pPlayer->Extra_Char_Num == CID_SIEG2) {
                                pPlayer->Direct_Motion_Input(MID_SIEG2_SKILL_GUARD);
                            }
                            else {
                                pPlayer->Direct_Motion_Input(MID_SIEG1_SKILL_GUARD);
                            }
                        }
                    }
                    else if (pPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME) {
                        if (pPlayer->IsMagicEffect(EGC_EFFECT_LIME2_SELF_AURA_DEFFENCE) && pPlayer->CheckSkill(SID_LIME2_SELF_AURA_DEFFENSE_PLUS_LV3)) {
                            if (pPlayer->GetMP() >= 0.3f) {
                                pPlayer->Direct_Motion_Input(MID_LIME2_COUNTER_ATK);
                            }
                        }
                    }
                    else {
                        pPlayer->Direct_Motion_Input(MID_RONAN3_BLOCKING);
                    }
                    return false;
                }
            }

            int iEnemyAtkGuardMotion = pPlayer->GetEnemyAtkGuardMotion(pPlayer->GetCurrentMotion());
            if (iEnemyAtkGuardMotion != -1 &&
                pPlayer->m_bSpecialAttack == false) {
                pPlayer->Direct_Motion_Input(iEnemyAtkGuardMotion);
                return false;
            }

            if (pPlayer->IsLocalPlayer())
            {
                g_kCamera.SetShakeCameraFrame(rand() % 2 ? 5 : 10);
                pPlayer->SetFlashEffectFrame(FLASH_EFFECT_FRAME);
                pPlayer->SetShakeEffectFrame(10);

                if (m_iCameraCrash > g_kCamera.m_iEarthQuake)
                {
                    g_kCamera.SetEarthQuake(m_iCameraCrash);

                }
            }

            float fIsRight;
            NoCheckContact = 0;
            if (!pPlayer->Shield)
            {
                pPlayer->bIsRight = !bIsRight;
            }

            if (pPlayer->bIsRight)
                fIsRight = -1.0f;
            else
                fIsRight = 1.0f;

            //피격자가 홀드 모션이면 피격모션을 취하지 않고, 피만 깎는다.
            if (pPlayer->uiMotion == GetOwnMotion(MID_COMMON_HOLD)
                || pPlayer->uiMotion == GetOwnMotion(MID_COMMON_HOLD_NOEFFECT))
            {

                if (pPlayer->Slow_Count < pPlayer->GetCurFormTemplate().dwStopOnDamage + m_iPlusDelay)
                    pPlayer->Slow_Count = (UCHAR)pPlayer->GetCurFormTemplate().dwStopOnDamage + m_iPlusDelay;

                DamageInfo damage;
#ifdef LEVEL_DESIGN_STAT_CALC
                damage.SetDamage(-m_fPhysicAtkPower * 3.f);
#else
                damage.SetDamage(-m_fPhysicAtkPower * 0.03f);
#endif		
                damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, m_iPlayerIndex);
                damage.SetSpecialAttack(m_bSpecialAttack, GetSpecialGrade());
                pPlayer->Change_HP(damage);

                HitEffect(*rtWeapon, rtBody);

                Sound(6);
                UsePower = true;
                m_bUsePower = true;
                continue;
            }

            //칼에 맞았을때만 밀리도록...
            if (m_fPhysicAtkPower > 4.0f && m_fPhysicAtkPower < 8.0f && (m_ePhysicAtkType == PAT_SWORD || m_ePhysicAtkType == PAT_SWORD_LEFT || m_ePhysicAtkType == PAT_FIST_LEFT))
            {
                float fPushFactor = 0.01f;
                if (MetamorphosisForm == FORM_MDAN && uiMotion == MID_RYAN4_MDAN_SPECIAL3)
                {
                    fPushFactor = 0.016f;
                }

                if (uiMotion != MID_ELESIS_GIGA_SLASH)
                {
                    pPlayer->x_Speed = fPushFactor * fIsRight;
                }
            }

            if (m_ePhysicAtkDamageAction == PAR_SMALL_DAMAGE_NODOWN)
            {
                pPlayer->Direct_Motion_Input(MID_COMMON_SMALLDAMAGE);
            }
            else if (m_ePhysicAtkDamageAction == PAR_BIG_DAMAGE_NODOWN)
            {
                pPlayer->Direct_Motion_Input(MID_COMMON_BIGDAMAGE);
            }
            else if (m_ePhysicAtkDamageAction == PAR_SMALL_DAMAGE)
            {
                pPlayer->Direct_Motion_Input(MID_COMMON_SMALLDAMAGE);
            }
            else if (m_ePhysicAtkDamageAction == PAR_FOOT_ACHE)
            {
                pPlayer->Direct_Motion_Input(MID_COMMON_DAMAGED_SPIKE_TRAP);
            }
            else if (m_ePhysicAtkDamageAction == PAR_MID_SPECIAL)
            {
                pPlayer->Direct_Motion_Input(MID_COMMON_DAMAGED_MID_SPECIAL);
            }
            else if (m_ePhysicAtkDamageAction == PAR_UPPER_SPECIAL)
            {
                pPlayer->Direct_Motion_Input(MID_COMMON_DAMAGED_UPPER_SPECIAL);
            }
            else
                pPlayer->Direct_Motion_Input(MID_COMMON_BIGDAMAGE);
            //}

            if ((pPlayer->uiMotion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) || pPlayer->uiMotion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)) &&
                (m_ePhysicAtkDamageAction == PAR_DOWN || !g_MyD3D->MyPlayer[i]->IsContact))
            {
                if (m_ePhysicAtkDamageAction != PAR_SMALL_DAMAGE_NODOWN && m_ePhysicAtkDamageAction != PAR_BIG_DAMAGE_NODOWN) {
                    pPlayer->x_Speed = 0.02f * fIsRight;
                    pPlayer->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP);
                }
            }
            else if (m_ePhysicAtkDamageAction == PAR_REVERSE_DOWN || !g_MyD3D->MyPlayer[i]->IsContact)
            {
                if (m_ePhysicAtkDamageAction != PAR_SMALL_DAMAGE_NODOWN && m_ePhysicAtkDamageAction != PAR_BIG_DAMAGE_NODOWN) {
                    pPlayer->bIsRight = !pPlayer->bIsRight;
                    pPlayer->x_Speed = -0.02f * fIsRight;
                    pPlayer->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP);
                }
            }
            else if (m_ePhysicAtkDamageAction == PAR_DOWN_IN_SKY)
            {
                pPlayer->Gravity = pPlayer->vPos.y;
                pPlayer->y_Speed = 0.01f + 0.025f;
                pPlayer->x_Speed = 0.02f * fIsRight;
                pPlayer->Direct_Motion_Input(MID_COMMON_DOWN_IN_SKY);
                pPlayer->cFrame = 0;
            }
            else if (PAR_FRONTDOWN_0 <= m_ePhysicAtkDamageAction && m_ePhysicAtkDamageAction <= PAR_FRONTDOWN_DROPDAMAGE_LAST)//띄우기 공격
            {
                //뜨기만 하고 데미지 없음
                int iGravity = 0;
                int iDropDamage = 0;
                if (m_ePhysicAtkDamageAction <= PAR_FRONTDOWN_LAST)
                {
                    iGravity = m_ePhysicAtkDamageAction - PAR_FRONTDOWN_0;
                }
                else
                {
                    iGravity = m_ePhysicAtkDamageAction - PAR_FRONTDOWN_DROPDAMAGE_0;
                    iDropDamage = iGravity / 2;
                }

                // 상대편 뛰우기 공격
                //pPlayer->Gravity=(float)( iGravity ) * 0.01f + 0.03f;
                pPlayer->Gravity = pPlayer->vPos.y;
                pPlayer->y_Speed = (float)(iGravity) * 0.005f + 0.025f;
                pPlayer->Last_Gravity = 0.01f * iDropDamage;
                pPlayer->Direct_Motion_Input(MID_COMMON_DAMAGED_BY_FLYATK);

                if (Extra_Char_Num == CID_LIRE3 && (uiMotion == MID_LIRE3_DASHATK2 || uiMotion == MID_LIRE3_CRITICAL_DASHATK2))
                {
                    if (bIsRight)
                        pPlayer->x_Speed = 0.015f;
                    else
                        pPlayer->x_Speed = -0.015f;
                }
            }

            if (pPlayer->y_Speed > 0 && (pPlayer->uiMotion != pPlayer->GetOwnMotion(MID_COMMON_DAMAGED_BY_FLYATK) &&
                pPlayer->uiMotion != pPlayer->GetOwnMotion(MID_COMMON_DOWN_IN_SKY)))
                pPlayer->y_Speed = 0;

            if (pPlayer->Slow_Count < pPlayer->GetCurFormTemplate().dwStopOnDamage + m_iPlusDelay)
                pPlayer->Slow_Count = (UCHAR)pPlayer->GetCurFormTemplate().dwStopOnDamage + m_iPlusDelay;

            if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
                pPlayer->Slow_Count += LATENCY + SHOWLATENCY;
        }

        // 피격자 속도 조절처리
        pPlayer->x_Speed += m_fPlayerPhysicPushX;
        pPlayer->y_Speed += m_fPlayerPhysicPushY;

        DamageInfo damage;
#ifdef LEVEL_DESIGN_STAT_CALC
        damage.SetDamage(-m_fPhysicAtkPower * 3.f);
#else
        damage.SetDamage(-m_fPhysicAtkPower * 0.03f);
#endif		
        damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, m_iPlayerIndex);
        damage.SetSpecialAttack(m_bSpecialAttack, GetSpecialGrade());
#ifdef MATCH_BALLENCE_121217
        damage.SetShieldCountEnable(true);
        damage.SetReflectionEnable(true);
        float fDamage = pPlayer->Change_HP(damage);
#else
        pPlayer->Change_HP(damage);
#endif		

        UsePower = true;
        m_bUsePower = true;
        HitEffect(*rtWeapon, rtBody);

        // 점령전에서는 플레이어를 때렸을때도 MP찬다.(취소되었다.)
        //if( IsLocalPlayer() &&
        //    (GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory()) )
        //{
        //    float f_fAtkPoint = GetAttackPoint( damage );
        //    float fIncMP = (f_fAtkPoint * 0.0125f) * ( 1.0f + m_fAbility[ABILITY_MP_RECOVERY_RATE]);
        //        g_MyD3D->ChargeLocalPlayerMana( fIncMP );
        //}

        // 진은 주먹으로 공격하기 때문에 무기류에 맞을 때와 다른 피격 사운드가 필요해서 예외처리함.
        if (Extra_Char_Num == CID_JIN1)
            Sound(1140);
        else if (Extra_Char_Num == CID_JIN3)
            Sound(1507 + rand() % 2);
        else if (Extra_Char_Num == CID_MARI3)
            Sound(92025);
        else
            Sound(6);

        CheckChargeMarblePhysicAtk();
        //#ifndef MATCH_BALLENCE_121217
        pPlayer->ShieldCountAttack(pPlayer->m_iPlayerIndex, m_iPlayerIndex, m_fPhysicAtkPower * 0.03f);
        pPlayer->ReflectionDamage(pPlayer->m_iPlayerIndex, m_iPlayerIndex, m_fPhysicAtkPower * 0.03f);
        //#endif
                // 스페셜 버프 적용
        if (g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.empty() == false) {
            std::map< int, std::map< int, std::pair< int, float > > >::iterator mit = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.find(SPECIAL_BUFF_SUCCESS_ATK);
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
    return UsePower;
}

bool PLAYER::Attack_Monster(GCCollisionRect<float>* rtWeapon)
{
    bool UsePower = false;
    GCCollisionRect<float> rtMonster;
    GCCollisionRect<float> rtCollision;

    for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        MONSTER* pMonster = mit->second;
        if (!pMonster->m_bLive)
            continue;

        if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
            continue;

        if (pMonster->GetNoCrashDamage())
            continue;

        for (int j = 0; j < pMonster->GetAttackBoundRectNum(); j++)
        {
            rtMonster = pMonster->GetAttackBoundRect(j);

            if (pMonster->IsInvincible() || !rtMonster.CheckCollision(*rtWeapon, &rtCollision))
                continue;

            if (IsAttackMiss())
            {
                if (pMonster->GetAttackCheck() && 0 <= pMonster->GetHP())
                {
                    if ((timeGetTime() - m_dwMissAttackTime) > MISS_ATTACK_TERM_TIME)
                    {
                        m_dwMissAttackTime = timeGetTime();
                        D3DXVECTOR2 hitPos = D3DXVECTOR2(rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f);
                        pMonster->ShowMissAttack(hitPos.x, hitPos.y);
                    }
                }
                continue;
            }

            if (false == pMonster->GetPermissionDamage().empty())
            {
                if ((timeGetTime() - m_dwMissAttackTime) > MISS_ATTACK_TERM_TIME)
                {
                    m_dwMissAttackTime = timeGetTime();
                    D3DXVECTOR2 hitPos = D3DXVECTOR2(rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f);
                    pMonster->ShowMissAttack(hitPos.x, hitPos.y);
                }
                continue;
            }

            pMonster->SetLastAttackedBoxIndex(j);

            // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
            pMonster->m_vDamageStartPos = D3DXVECTOR2(GetX(), GetY());
            pMonster->m_vDamageWhoPos = D3DXVECTOR2(GetX(), GetY());
            bool bCheckHit = pMonster->OnDamage(m_iPlayerIndex, -1,
                m_fPhysicAtkPower,
                m_eAttackType, m_eAttackDirection,
                m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack, GetSpecialGrade());
            Slow_Count = m_iHitDelay;
            Next_Attack_Count = 15;
            UsePower = true;
            m_bUsePower = true;

            if (IsLocalPlayer() && GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_FURY_MODE && !m_bIsBurnningMode && !IsSpecialSkillMotion() ) {
                CountBP( GetCurFormTemplate().fBPPerAttackMonster * 3.0f, false );
            }

            HitEffect(*rtWeapon, rtMonster);
            D3DXVECTOR2 hitPos = D3DXVECTOR2(rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f);
            if (m_bSpecialAttack) {
                g_pMagicEffect->HitCreateBuffDamage(m_iPlayerIndex, mit->first, &hitPos, CAHR_ATTACK_TYPE_SPECIAL_DAMAGE);
            }
            else {
                g_pMagicEffect->HitCreateBuffDamage(m_iPlayerIndex, mit->first, &hitPos, CAHR_ATTACK_TYPE_PHYSIC);
            }
            pMonster->SetHitPos(hitPos);
            CheckChargeMarblePhysicAtk();
            // 스페셜 버프 적용
            if (g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.empty() == false) {
                std::map< int, std::map< int, std::pair< int, float > > >::iterator mit = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.find(SPECIAL_BUFF_SUCCESS_ATK);
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
            break;
        }
        pMonster->SetAttackedMonster(true);

    }

    if (UsePower)
    {
        if (Extra_Char_Num == CID_JIN1)
            Sound(1140);
        else if (Extra_Char_Num == CID_JIN3)
            Sound(1507 + rand() % 2);
        else
            Sound(6);
    }

    g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);

    return UsePower;
}

bool PLAYER::Attack_Object(GCCollisionRect<float>* rtWeapon)
{
    bool UsePower = false;
    GCCollisionRect<float> rtObject;
    GCCollisionRect<float> rtCollision;

    for (Itor2DObject itor = g_MyD3D->Begin2DObject(); itor != g_MyD3D->End2DObejct(); ++itor)
    {
        rtObject = (*itor)->GetCollisionRect();

        if (!rtObject.CheckCollision(*rtWeapon, &rtCollision))
            continue;

#ifdef LEVEL_DESIGN_STAT_CALC
        if ((*itor)->Damaged(*rtWeapon, m_fPhysicAtkPower * 3.f, -1, m_iPlayerIndex, false) == false)
            continue;
#else
        if ((*itor)->Damaged(*rtWeapon, m_fPhysicAtkPower * 0.03f, -1, m_iPlayerIndex, false) == false)
            continue;
#endif

        Slow_Count = m_iHitDelay;
        Next_Attack_Count = 15;
        UsePower = true;
        m_bUsePower = true;

        HitEffect(*rtWeapon, (*itor)->GetCollisionRect());
    }

    for (int i = 0; i < MAX_OBJECT; i++)
    {
        if (!g_MyD3D->m_pObject[i]) continue;
        KGCGameObject* pObject = (KGCGameObject*)g_MyD3D->m_pObject[i];
        if (!pObject) continue;
        if (!pObject->IsLive() || !pObject->IsCrashCheck())
            continue;

        rtObject = pObject->GetCollisionRect();

        if (!rtObject.CheckCollision(*rtWeapon, &rtCollision))
            continue;

        bool bCheckHit = pObject->OnDamage(m_iPlayerIndex, m_fPhysicAtkPower,
            m_eAttackType, m_eAttackDirection,
            m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack);

        Slow_Count = m_iHitDelay;
        Next_Attack_Count = 15;
        UsePower = true;

        HitEffect(*rtWeapon, rtObject);
        D3DXVECTOR2 hitPos = D3DXVECTOR2(rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f);
        g_pMagicEffect->HitCreateBuffDamage(m_iPlayerIndex, i, &hitPos);
        CheckChargeMarblePhysicAtk();

    }
    if (UsePower)
    {
        if (Extra_Char_Num == CID_JIN1)
            Sound(1140);
        else if (Extra_Char_Num == CID_JIN3)
            Sound(1507 + rand() % 2);
        else
            Sound(6);
    }

    return UsePower;
}

bool PLAYER::Attack_MinigameObj(GCCollisionRect<float>* rtWeapon)
{
    std::pair<bool, GCCollisionRect<float> > pairCollision = SiKGCMinigameManager()->HitObject(rtWeapon, false);
    if (pairCollision.first == true)
    {
        Slow_Count = m_iHitDelay;
        Next_Attack_Count = 15;

        HitEffect(*rtWeapon, pairCollision.second);

        CheckChargeMarblePhysicAtk();
    }
    return pairCollision.first;
}

bool PLAYER::Attack_Damage(GCCollisionRect<float>* rtWeapon)
{
    return g_MyD3D->m_pDamageManager->CrashWithWeapon(m_iPlayerIndex, rtWeapon);
}

void PLAYER::All_Latency_Equal()
{
    for (int i = 0; i < (LATENCY + SHOWLATENCY); i++)
    {
        g_kLocalPlayerLatency->fHP[i] = GetHP();
        g_kLocalPlayerLatency->usShield[i] = Shield;
        g_kLocalPlayerLatency->kRenderData[i].bIsRight = bIsRight;
        g_kLocalPlayerLatency->kRenderData[i].vPos.x = vPos.x;
        g_kLocalPlayerLatency->kRenderData[i].vPos.y = vPos.y;
        g_kLocalPlayerLatency->vSpeed[i].x = x_Speed;
        g_kLocalPlayerLatency->vSpeed[i].y = y_Speed;
        g_kLocalPlayerLatency->kRenderData[i].cFrame = cFrame;
        g_kLocalPlayerLatency->kRenderData[i].usMotion = uiMotion;
    }

}

// 가끔씩은 레이턴시를 무시하고 바로 동작을 전환해야하는 경우가 있음.
void PLAYER::Direct_Motion_Input(UINT Motion, bool bSpecialDamage)
{
    if (false == VerifyRealMotion(Motion))
    {
        if (IsContact)
            Motion = GetWaitMotion();
        else
            Motion = GetCurFormTemplate().JUMP;
    }

    // 탑승시 탑승물에 등록된 모션만 체인지 가능
    if (UINT_MAX > GetEmbarkUID())
    {
        bool bReturn = true;
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark)
        {
            std::vector<int> vecEmbarkMotion = SiKGCEmbarkManager()->GetCharMotionList();
            std::vector<int>::iterator vecIter = vecEmbarkMotion.begin();
            for (; vecIter != vecEmbarkMotion.end(); ++vecIter)
            {
                if ((*vecIter) == Motion)
                {
                    bReturn = false;
                    break;
                }
            }
        }
        if (bReturn)
            return;
    }

    //====================================================================================
    // 2009.02.03 : Jemitgge
    // Comment : Direct_Motion_Input으로 모션을 바꾼다면, Latency 문제를 꼭 체크해보시길... Slow_Count가 걸려있지 않은 모션에서는
    //			 cFrame이 LATENCY + SHOWLATENCY 만큼 증가하여 모션의 첫 Frame 부터 시작하지 않을 수 있음,,

    Before_Motion = Old_Motion;
    //////////////////////////////////////////////////////////////////////////
    if (IsLocalPlayer() == false && GC_GM_TUTORIAL != SiKGCRoomManager()->GetGameMode())
        return;

    unsigned short pFrame = 0;

    //승리의 포즈시 데미지 입지 않게...
    if (uiMotion == GetCurFormTemplate().WIN ||
        uiMotion == GetCurFormTemplate().START
        )
        return;

    // Shield가 있을때는
    // 맞는 모션으로 바꾸라고 하면 바꾸지 않음
    // 무적일때도 맞는모션으로 바꾸라고 하면 안바꾼다
    // 넘어진상태에서는 무적이든 쉴드든 일어나야한다.
    if ((Motion != MID_COMMON_DOWN_AND_STANDUP) &&
        (Shield || IsInvinsible(bSpecialDamage)))
    {
        if (IsDamagedMotion(Motion))
            return;
    }

    if (uiMotion == MID_COMMON_HOLD || uiMotion == MID_COMMON_HOLD_NOEFFECT)
    {
        return;
    }

    //네피림의 경우만 피격 모션이 없음. 그러나 밀리는 것만 있음.
    if (MetamorphosisForm == FORM_NEPHILIM)
    {
        //맞은 방향으로 밀린다. x좌표를 이동.
        if (bIsRight)
        {
            for (int i = 0; i < (LATENCY + SHOWLATENCY); i++)
            {
                g_kLocalPlayerLatency->kRenderData[i].vPos.x = g_kLocalPlayerLatency->kRenderData[i].vPos.x - 0.01f;
            }
        }
        else
        {
            for (int i = 0; i < (LATENCY + SHOWLATENCY); i++)
            {
                g_kLocalPlayerLatency->kRenderData[i].vPos.x = g_kLocalPlayerLatency->kRenderData[i].vPos.x + 0.01f;
            }
        }
        All_Latency_Equal();
    }

    // 2007/2/16. iridology. SuperArmor 상태일때에는 맞는 모션으로 가지 않음.
    // 네피림의 경우에만 사용할수도 있지만 MagicEffect로 빼기 위해 작업.
    if (m_bSuperArmor)
    {
        if (Motion == MID_COMMON_BIGDAMAGE || Motion == MID_COMMON_SMALLDAMAGE)
            return;
    }

    if (uiMotion == MID_NEPHILLIM_DOWN_AND_STAND_UP && Motion == GetWaitMotion())
    {
        pFrame = cFrame;
        uiMotion = GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP);
        return;
    }
    if (uiMotion == MID_NEPHILLIM_COUNTER_DAMAGE && Motion == GetWaitMotion())
    {
        pFrame = cFrame;
        uiMotion = GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP);
        return;
    }

    if (uiMotion == MID_RYAN4_MDAN_DOWN_AND_STAND_UP && Motion == GetWaitMotion())
    {
        pFrame = cFrame;
        uiMotion = GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP);
        return;
    }
    if (uiMotion == MID_RYAN4_MDAN_COUNTER_DAMAGE && Motion == GetWaitMotion())
    {
        pFrame = cFrame;
        uiMotion = GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP);
        return;
    }
    if ((uiMotion == MID_RIN4_FORM_GODDESS_DOWN_AND_STAND_UP && Motion == GetWaitMotion()) ||
        (uiMotion == MID_RIN4_FORM_GODDESS_FRONTDOWN_AND_STANDUP && Motion == GetWaitMotion()))
    {
        pFrame = cFrame;
        uiMotion = GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP);
        return;
    }
    if ((uiMotion == MID_RIN4_FORM_EVIL_DOWN_AND_STAND_UP && Motion == GetWaitMotion()) ||
        (uiMotion == MID_RIN4_FORM_EVIL_FRONTDOWN_AND_STANDUP && Motion == GetWaitMotion()))
    {
        pFrame = cFrame;
        uiMotion = GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP);
        return;
    }

    if (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
        return;

    // 2007/5/18. daeuk. CounterTest
    //#define COUNTER_ATK_TEST
#ifdef COUNTER_ATK_TEST
#ifdef _DEBUG
    if (uiMotion == GetWaitMotion())
        g_MyD3D->MyCtrl->Energy = 1.1f;
#endif
#endif

    //Counter Attack맞음
    if (IsLocalPlayer() && g_MyD3D->MyCtrl->Energy > 1.0f)
    {
        if (IsPossibleCounterMotion(Motion))
        {
            float fDecreaseEnergyRate = 1.0f;

#if defined( USE_MATCH_STATUS_RATE ) 
            if (SiKGCRoomManager()->IsMonsterGameMode() == false) {
                float fCounterDef = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_COUNTER_DEF] * COUNTER_DEF_PERCENT_MATCH;
                if (fCounterDef > COUNTER_DEF_MAX_VALUE_MATCH) {
                    fCounterDef = COUNTER_DEF_MAX_VALUE_MATCH;
                }
                fDecreaseEnergyRate -= fCounterDef;
            }
            else {
                fDecreaseEnergyRate -= m_fAbility[ABILITY_COUNTER_DEF];
            }
#else
            fDecreaseEnergyRate -= m_fAbility[ABILITY_COUNTER_DEF];
#endif

            if (IsMagicEffect(EGC_AMY_ABRACADABRA_3) || IsMagicEffect(EGC_AMY_ABRACADABRA_DUN_3))
                fDecreaseEnergyRate = 0.0f;

            if (fDecreaseEnergyRate < 0.0f)
                fDecreaseEnergyRate = 0.0f;

            g_MyD3D->MyHead->Level -= g_MyD3D->MyCtrl->Energy * fDecreaseEnergyRate;


            for (unsigned int i = 0; i < m_vecHotKeySkill.size(); i++)
            {
                float fCoolTime = 10.0f;

                std::map< int, float >::iterator mit = GetCurFormTemplate().mapControlCountCoolTimeSkillList.begin();
                for (mit; mit != GetCurFormTemplate().mapControlCountCoolTimeSkillList.end(); ++mit) {
                    if (CheckSkill(mit->first)) {
                        fCoolTime = mit->second;
                    }
                }

                if (m_vecHotKeySkill[i].fCoolTime < fCoolTime)
                {
                    m_vecHotKeySkill[i].fCoolTime = fCoolTime;
                }

            }

            // 진짜 각성 들어가면 들어낼 부분
            if (g_kGlobalValue.IsCharUseSpecial4(GetCurrentChar().iCharType)) {
                for (std::map< int, std::vector< GCHotKeySkill > >::iterator mitSkill = m_mapHotKeySkill.begin(); mitSkill != m_mapHotKeySkill.end(); ++mitSkill) {
                    for (unsigned int i = 0; i < mitSkill->second.size(); i++)
                    {
                        float fCoolTime = 10.0f;

                        std::map< int, float >::iterator mit = GetCurFormTemplate().mapControlCountCoolTimeSkillList.begin();
                        for (mit; mit != GetCurFormTemplate().mapControlCountCoolTimeSkillList.end(); ++mit) {
                            if (CheckSkill(mit->first)) {
                                fCoolTime = mit->second;
                            }
                        }

                        if (mitSkill->second[i].fCoolTime < fCoolTime) {
                            mitSkill->second[i].fCoolTime = fCoolTime;
                        }
                    }
                }
            }

            if (g_MyD3D->MyHead->Level < 0)
                g_MyD3D->MyHead->Level = 0.0f;

            g_MyD3D->MyCtrl->Energy = 0;
            Motion = MID_COMMON_FRONTDOWN_AND_STANDUP;

#if defined (MATCH_BALANCE_2011_8)
            Super = 100;
#endif

            SiKGCAchieveManager()->OccurAction(SAchieve::COUNTER_ATTACK, GetCharType());

            Ah_Sound(6);
        }
    }

    if (IsLocalPlayer() && g_MyD3D->MyCtrl->Skill_Key_Count > 0)
    {
        if (IsPossibleCounterMotion(Motion))
        {
            g_MyD3D->MyCtrl->Skill_Key_Count = 0;
            KGCPlayerRyan::m_fChargeElapsedTime = 0.0f;
        }
    }

    //공통 모션을 사용하지 않는 케릭터들일 경우 공통 모션을 다른 걸로 바꿔줍니다.
    int iChangedMotion = GetOwnMotion(Motion);

    if (SetPlayerState(iChangedMotion))
    {
        //====================================================================================
        // 2009.02.03 : Jemitgge
        // Comment : peer들과 모션 싱크를 맞추기 위해서 latency 에 따라 pFrame을 증가 시켜줍니다.

        if (IsLocalPlayer() || GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
        {

            for (int i = 0; i < (LATENCY + SHOWLATENCY); i++)
            {
                if (IsLocalPlayer())
                {
                    g_kLocalPlayerLatency->kRenderData[i].bIsRight = bIsRight;
                    g_kLocalPlayerLatency->kRenderData[i].usMotion = iChangedMotion;
                    g_kLocalPlayerLatency->kRenderData[i].cFrame = pFrame;
                }

                if (Slow_Count > 0)
                    Slow_Count--;
                else
                    pFrame++;
            }

        }
        cFrame = pFrame;

        // 죽음 처리로 바로 들어가는 것을 막기위함
        if ((Motion == MID_COMMON_DOWN_AND_STANDUP || Motion == MID_COMMON_FRONTDOWN_AND_STANDUP || Motion == MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) && GetHP() <= 0.0f)
        {
            if (Slow_Count > 29)
                Slow_Count = 29;
        }

        Body_Angle = 0.0f;

        // 엎어지면 소환카운트를 0으로 하자, 가지고 있는게 불리함.
        if ((Motion == MID_COMMON_DOWN_AND_STANDUP || Motion == MID_COMMON_FRONTDOWN_AND_STANDUP || Motion == MID_COMMON_DAMAGED_BY_FLYATK || Motion == MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER))
            m_iSummoning = 0;

    }



}

bool PLAYER::CheckHiding()
{
    // 숨는 매직 이펙트를 넣어주자
    std::vector<EGCPlayerMagicEffect>HideEffect;
    HideEffect.push_back(EGC_EFFECT_BACKSTEP);
    HideEffect.push_back(EGC_SKILL_HIDE);
    HideEffect.push_back(EGC_EFFECT_INVISIBLE);
    HideEffect.push_back(EGC_EFFECT_GHOST_HIDE);

    for (int i = 0; i < (int)HideEffect.size(); ++i)
    {
        bool isHiding = g_MyD3D->MyPlayer[m_iPlayerIndex]->IsMagicEffect(HideEffect[i]);

        if (isHiding == true)
            return true;
    }

    return false;
}

void PLAYER::ShowAllItemEffect(bool bShow)
{
    for (std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
    {
        if (MetamorphosisForm == FORM_NORMAL)
        {
            (*j).animInst->SetShow(NoForceEffect((*j).positionType) && bShow);
        }
        else
            (*j).animInst->SetShow(false);
    }

    for (std::vector<ITEM_EFFECT>::iterator i = m_vecItemEff.begin(); i != m_vecItemEff.end(); i++)
    {
        for (unsigned int j = 0; j < (*i).pSeq.size(); j++)
        {
            if (MetamorphosisForm == FORM_NORMAL)
            {
                if ((*i).pSeq[j])
                    (*i).pSeq[j]->SetShow(NoForceEffect((*i).emitPositionType[j / (*i).sequenceName.size()]) && bShow);
            }
            else
            {
                if ((*i).pSeq[j])
                    (*i).pSeq[j]->SetShow(false);
            }
        }
    }

    m_bShowEffect = bShow;

    g_pkPetAI->Show(m_vecPetAiID, bShow);
    if (m_iPlayerIndex != -1 && m_iPlayerIndex != 7)
        g_MyD3D->m_akPet[m_iPlayerIndex]->ShowEffect(bShow);

    // 특정 모션 때 꺼줘야 할 이펙트들을 설정. 모션에서 y값 조절해버리면 이펙트들 안따라 다니는 문제때문에 이렇게라도 하드코딩 함...
    if (m_pRonan3Force)
        m_pRonan3Force->SetShow(bShow);
    if (m_pRonan3Force1)
        m_pRonan3Force1->SetShow(bShow);
    if (m_pRonan3Force2)
        m_pRonan3Force2->SetShow(bShow);
    if (m_pRuneShield)
        m_pRuneShield->SetShow(bShow);
    if (m_pRuneShield1)
        m_pRuneShield1->SetShow(bShow);
    for (int i = 0; i < 4; i++)
    {
        if (m_pHoldEffect[i])
            m_pHoldEffect[i]->SetShow(bShow);
        if (m_pMonoVolt[i])
            m_pMonoVolt[i]->SetShow(bShow);
    }

    if (m_pAbyssHandEff[0])
        m_pAbyssHandEff[0]->SetShow(bShow);
    if (m_pAbyssHandEff[1])
        m_pAbyssHandEff[1]->SetShow(bShow);
    // 진 - 버닝관련 이펙트
    if (m_pBurnningCharge[0])
        m_pBurnningCharge[0]->SetShow(bShow);
    if (m_pBurnningCharge[1])
        m_pBurnningCharge[1]->SetShow(bShow);

    ShowBaseEffect((bShow && m_bShowBaseEffect));
}

void PLAYER::ShowBaseEffect(bool bShow)
{
    std::vector< CharBaseParticle >::iterator vecParticle = GetCurFormTemplate().BaseEffect.vecParticleInfo.begin();
    for (; vecParticle != GetCurFormTemplate().BaseEffect.vecParticleInfo.end(); ++vecParticle)
    {
        if (vecParticle->pParticle)
        {
            vecParticle->pParticle->SetShow(((GetIsRight() == vecParticle->bIsRight) && bShow));
        }
    }
}

void PLAYER::Shadow_Render()
{
    if (m_kUserInfo.nUserState != GC_RUS_PLAYING)
        return;

    if (!m_cLife)

        return;

    //if( g_MyD3D->MyReplay->m_iCurrState != RP_VIEW && SiKGCRoomManager()->GetGameMode() != GC_GM_TUTORIAL )
    //    return;

    if (m_kUserInfo.cCharIndex == -1)
        return;

    if (m_bRenderQuestDrama == false)
        return;

    if (m_bRender == false)
        return;

    unsigned short i_Frame;
    UINT i_Motion;
    float fX;

    //모델을 그린다.
    if (IsLocalPlayer())
    {
        i_Frame = g_kLocalPlayerLatency->kRenderData[0].cFrame;
        i_Motion = g_kLocalPlayerLatency->kRenderData[0].usMotion;
        fX = g_kLocalPlayerLatency->kRenderData[0].vPos.x;
    }
    else
    {
        i_Frame = cFrame;
        i_Motion = uiMotion;
        fX = vPos.x;
    }


    GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion(i_Motion);
    if (NULL == pMotion)
        return;

    if (i_Frame > pMotion->GetNum_Frame() - 1)
        i_Frame = (unsigned char)pMotion->GetNum_Frame() - 1;
    if (GetCurrentChar().iCharType == GC_CHAR_ARME &&
        (i_Motion == MID_ARME_ARME2_DASH || i_Motion == MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER) && i_Frame < 40)
        return;
    //if( GetCurrentChar().iCharType == GC_CHAR_LAS
    //    && i_Motion == MID_LAS_EVASION
    //    && (i_Frame > 5 && i_Frame < 22) )
    //    return;
    // 제일 낮은 위치를 빼온다.
    D3DXVECTOR2 vShadowPos;
    if (IsLocalPlayer())
    {
        vShadowPos.x = g_kLocalPlayerLatency->kRenderData[0].vPos.x;
        vShadowPos.y = g_kLocalPlayerLatency->kRenderData[0].vPos.y;
    }
    else
    {
        vShadowPos.x = vPos.x;
        vShadowPos.y = vPos.y;
    }
    Shadow_y = g_MyD3D->MyStg->Get_LowHeight(vShadowPos.x, vShadowPos.y);

    float fDistance = vShadowPos.y - Shadow_y + (pMotion->GetFrameData(i_Frame).Pos_y + GetCurFormTemplate().fCharHeight) * CHAR_SCALE_RATIO;
    if (fDistance < -0.18f)
        fDistance = -0.18f;

    float fZ = 1.5f;
    if (SiKGCRoomManager()->IsMonsterGameMode())
    {
        fZ = KGCLayerPart::ms_fZOrder[6];
    }

    D3DXMATRIX mat1, mat2, mat3;
    D3DXMatrixScaling(&mat1, CHAR_SCALE_RATIO, CHAR_SCALE_RATIO, 0.0001f);
    D3DXMatrixTranslation(&mat2, fX - 1.0f, Shadow_y - 0.5f, fZ);
    D3DXMatrixMultiply(&mat3, &mat1, &mat2);
    D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

    D3DXVECTOR4 vResult;
    D3DXVec3Transform(&vResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mat1);

    float fShadowSize;
    switch (MetamorphosisForm)
    {
    case FORM_NORMAL:
        fShadowSize = 0.150f;
        break;
    case FORM_WOLF:
        fShadowSize = 0.160f;
        break;
    case FORM_NEPHILIM:
        fShadowSize = 0.170f;
        break;
        // For_Thanatos    
    case FORM_MDAN:
    case FORM_RIN_GODDESS:
    case FORM_RIN_EVIL:
        fShadowSize = 0.170f;
        break;
    default:
        fShadowSize = 0.150f;
        break;
    }

    if (Item_Remain_Time[GC_GAME_ITEM_HALLOW] || CheckHiding())
    {
        if (IsLocalPlayer())
        {
            g_pGraphicsHelper->SimpleShadowRender(vResult.x, vResult.y, fDistance, fShadowSize, fZ);
        }

        if (IsMagicEffect(EGC_SKILL_HIDE))
        {
            if (m_pHideParticle == NULL)
            {
                m_pHideParticle = g_ParticleManager->CreateSequence("Las01_Shadow_Hide_01", 0, 0, 0.0f);

                TRACE_INFO stTrace;
                if (m_pHideParticle) {
                    stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
                    stTrace.m_fYOffset = -0.05f;
                    stTrace.m_fParticleTraceTime = 5.0f;
                    m_pHideParticle->SetTrace(&stTrace);
                }

            }
        }
        else
        {
            if (m_pHideParticle != NULL)
            {
                g_ParticleManager->DeleteSequence(m_pHideParticle);
                m_pHideParticle = NULL;
            }
        }
    }
    else if (GetHP() > 0.f ||
        !((uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)) && cFrame > 20))
    {
        g_pGraphicsHelper->SimpleShadowRender(vResult.x, vResult.y, fDistance, fShadowSize, fZ);
    }
}

void PLAYER::Get_BasicMatrix(D3DXMATRIX* pMat, bool bLatency, bool bCamera, bool bForNickName)
{
    D3DXMATRIX mat2, mat3;
    float f_y_Ex = 0.0f;
    KPlayerRenderData kRenderData;
    if (IsLocalPlayer() && bLatency)
    {
        kRenderData = g_kLocalPlayerLatency->kRenderData[0];
    }
    else
    {
        kRenderData = *this;
    }

    float fCharScaleRatio = CHAR_SCALE_RATIO;
    if (GetCurFormTemplate().fPlayingScale != 0.f) {
        fCharScaleRatio = CHAR_SCALE_RATIO * GetCurFormTemplate().fPlayingScale;
    }

    if (kRenderData.bIsRight)
    {
        D3DXMatrixScaling(pMat, fCharScaleRatio, fCharScaleRatio, fCharScaleRatio);
    }
    else
    {
        D3DXMatrixScaling(&mat2, fCharScaleRatio, fCharScaleRatio, fCharScaleRatio);
        D3DXMatrixRotationY(&mat3, 3.1415f);
        D3DXMatrixMultiply(pMat, &mat2, &mat3);
    }

    if (NULL == g_pGraphicsHelper->GetPlayerActionMotion(kRenderData.usMotion))
        return;

    if (!bForNickName) //닉네임일 경우, 닉네임이 흔들락 거리지 않도록 막는다.
    {
        f_y_Ex = (g_pGraphicsHelper->GetPlayerActionMotion(kRenderData.usMotion)->GetFrameData((int)kRenderData.cFrame).Pos_y + GetCurFormTemplate().fCharHeight) * CHAR_SCALE_RATIO;
    }

    // [12/11/2007] breadceo.잡기 모션일 때 y 좌표를 모션꺼로 업데이트 하지 말자. 렌더링은 잘될꺼고, 이거때문에 맹계문제가 생긴다
    if (IsCatchMotion(uiMotion))
    {
        f_y_Ex = 0.14f;
    }

    float fShakeTargetX = 0.0f;
    if (m_iShakeEffectFrame && bForNickName == false)
        fShakeTargetX = (float)(rand() % 2 ? 1 : -1) * 0.010f;

    if (bCamera)
        D3DXMatrixTranslation(&mat2, kRenderData.vPos.x - 1.0f + fShakeTargetX, kRenderData.vPos.y - 0.48f + f_y_Ex, 1.0f);
    else
        D3DXMatrixTranslation(&mat2, kRenderData.vPos.x - 1.0f + fShakeTargetX, kRenderData.vPos.y - 0.23f + f_y_Ex, 1.0f);

    D3DXMatrixMultiply(&mat3, pMat, &mat2);

    if (bCamera)
    {
        D3DXMatrixMultiply(pMat, &mat3, &g_kCamera.m_matCamera);
    }
    else
    {
        *pMat = mat3;
    }

}

// 순차적으로 본을 하나씩 그려나간다.
// 캐릭터스튜디오의 본 인덱스와 실제 코드상의 인덱스 번호 차이가 있음.
// 캐릭터스튜디오의 본 인덱스에서 +1한 값이 이 함수상에서 쓰인 번호와 일치함에 주의할 것!!!
void PLAYER::RecordBonePosition()
{
    D3DXVECTOR3 vecTemp;
    D3DXMATRIX matTemp;
    D3DXMATRIX matWorld;
    D3DXVECTOR3 posSword;

    // 변신 때문에 체크
    ASSERT(m_pObject != NULL);

    // 변신하는데 죽어서 추가...
    if (NULL == m_pObject)
        return;

    if (NULL == g_pGraphicsHelper->GetPlayerActionMotion(uiMotion))
        return;

    // 2007/2/15. iridology. cFrame Bound Check
    if (cFrame < 0)
        cFrame = 0;

    else if (cFrame > g_pGraphicsHelper->GetPlayerActionMotion(uiMotion)->GetNum_Frame() - 1)
        cFrame = g_pGraphicsHelper->GetPlayerActionMotion(uiMotion)->GetNum_Frame() - 1;

    Get_BasicMatrix(&matWorld, false, false);

    SFrame* pframe = m_pObject->GetFrame();

    m_pObject->SetFrame(&g_pGraphicsHelper->GetPlayerActionMotion(uiMotion)->GetFrameData(cFrame));

    m_pObject->SetMotionMatrix(false, NULL, false);

    // 몸 영역을 계산합니다
    MyBody.x = 50.0f; //left
    MyBody.y = -50.0f; //right
    MyBody.z = 50.0f; //bottom
    MyBody.w = -50.0f; //top

    for (DWORD i = 0; i < 14; i++)
    {
        m_pObject->GetAngleBone(i, &matTemp);

        D3DXMatrixMultiply(&matTemp, &matTemp, &matWorld);

        vecTemp.x = matTemp._41;
        vecTemp.y = matTemp._42;
        vecTemp.z = matTemp._43;

        if (vecTemp.x < MyBody.x)
            MyBody.x = vecTemp.x;
        if (vecTemp.x > MyBody.y)
            MyBody.y = vecTemp.x;
        if (vecTemp.y < MyBody.z)
            MyBody.z = vecTemp.y;
        if (vecTemp.y > MyBody.w)
            MyBody.w = vecTemp.y;
    }
    //잘 안맞는다고 생각... 이런 후처리를...
    MyBody.x -= 0.1f;
    MyBody.y += 0.1f;
    MyBody.z -= 0.1f;
    MyBody.w += 0.1f;

    // 라스 3차 전직 3단필살기 하드코딩. 미안요
    if (Extra_Char_Num == CID_LAS4)
    {
        if (uiMotion == MID_LAS4_SPECIAL3 ||
            (uiMotion == MID_LAS4_SPECIAL3_FINISH && cFrame < GC_FPS_LIMIT) ||
            (uiMotion == MID_LAS4_SPECIAL3_FINISH_LAST && cFrame < 20))
        {
            GetCurFormTemplate().vWeaponBase.z = 0.08f;
            GetCurFormTemplate().vWeaponEnd.z = -1.7f;
        }
        else
        {
            GetCurFormTemplate().vWeaponBase.z = -0.2f;
            GetCurFormTemplate().vWeaponEnd.z = -1.0f;
        }
    }

    ProcessPositionDescParam processPositionDescParam(this);
    processPositionDescParam.matWorld = matWorld;

    ProcessPositionDesc(processPositionDescParam);

    MyArmor = m_posDesc.rightHandTip;
    MyArmor_Ex = m_posDesc.rightHandBody;

    MyLeftArmor = m_posDesc.leftHandTip;
    MyLeftArmor_Ex = m_posDesc.leftHandBody;

    m_pObject->SetFrame(pframe);
}


// LATENCY 와 관련된 작업을 하는곳 MyID일때만 해당됨
void PLAYER::UpdateData()
{
    if (IsLocalPlayer())
    {
        int i;
        for (i = 0; i < (LATENCY + SHOWLATENCY) - 1; i++)
        {
            g_kLocalPlayerLatency->fHP[i] = g_kLocalPlayerLatency->fHP[i + 1];
            g_kLocalPlayerLatency->usShield[i] = g_kLocalPlayerLatency->usShield[i + 1];
            g_kLocalPlayerLatency->kRenderData[i].bIsRight = g_kLocalPlayerLatency->kRenderData[i + 1].bIsRight;
            g_kLocalPlayerLatency->kRenderData[i].vPos.x = g_kLocalPlayerLatency->kRenderData[i + 1].vPos.x;
            g_kLocalPlayerLatency->kRenderData[i].vPos.y = g_kLocalPlayerLatency->kRenderData[i + 1].vPos.y;
            g_kLocalPlayerLatency->vSpeed[i] = g_kLocalPlayerLatency->vSpeed[i + 1];
            g_kLocalPlayerLatency->kRenderData[i].cFrame = g_kLocalPlayerLatency->kRenderData[i + 1].cFrame;
            g_kLocalPlayerLatency->kRenderData[i].usMotion = g_kLocalPlayerLatency->kRenderData[i + 1].usMotion;
        }
        g_kLocalPlayerLatency->fHP[i] = GetHP();
        g_kLocalPlayerLatency->usShield[i] = Shield;
        g_kLocalPlayerLatency->kRenderData[i] = *this;
        g_kLocalPlayerLatency->vSpeed[i].x = x_Speed;
        g_kLocalPlayerLatency->vSpeed[i].y = y_Speed;


        pLatency++;
        if (pLatency > LATENCY - 1)// 현재 LATENCY번만큼 지나면 정보를 Send함...
        {
            SendNetworkData(GC_PID_PLAYERLATENCY_FRAME_INFO, 0);
            pLatency = 0;
        }
    }
}

void PLAYER::SoundForCharIndex(const char* ID, unsigned char delay, bool bNoOverlap)
{
    PROFILE_SET("PLAYER::SoundForCharIndex");

    /************************************************************************/
    /* 06/03/2007. daeuk. 몬스터는 소리 나야될꺼 아니삼..
    /************************************************************************/
    if (m_iPlayerIndex != MAX_PLAYER_NUM)
    {
        //승리 포즈일 때는 한 번만 이겼다는 소리를 내자.
        if (uiMotion == GetCurFormTemplate().WIN)
        {
            if (Before_Motion == uiMotion)
                return;
            else
                Before_Motion = uiMotion;
        } //계속 이겼다는 소리지르는 버그 때문에;;
    }

    if (IsLocalPlayer())
    {
        if (bNoOverlap)
            g_KDSound.PlayNoOverlapping(ID, 0, LATENCY + SHOWLATENCY + delay, m_iDotaStage);
        else
            g_KDSound.Play(ID, 0, LATENCY + SHOWLATENCY + delay, m_iDotaStage);
    }
    else
    {
        if (bNoOverlap)
            g_KDSound.PlayNoOverlapping(ID, 0, delay, m_iDotaStage);
        else
            g_KDSound.Play(ID, 0, delay, m_iDotaStage);
    }
}

void PLAYER::Sound(int Num, unsigned char delay, bool bNoOverlap)
{
    PROFILE_SET("PLAYER::Sound");

    if (Num == -1)
        return;

    if (g_pkGameOverlayUI->IsRenderCartoonDrama()) {
        return;
    }

    // 시작모션이 스킵인데 시작모션 중이면 쌩까지
    if (SiKGCWorldMapManager()->IsSkipStartMotion(SiKGCRoomManager()->GetGameMode()) &&
        uiMotion == GetCurFormTemplate().START)
    {
        return;
    }

    //Fatal Sound Delay를 위해
    if (Num == 117 || Num == 118)
    {
        if (Fatal_Sound_Delay)
            return;

        Fatal_Sound_Delay = 149;
    }
    std::ostringstream strmSoundNum;
    strmSoundNum << Num;

    SoundForCharIndex(strmSoundNum.str().c_str(), delay, bNoOverlap);

}

int PLAYER::GetEncoachSoundSwitch(int iOriginalSound)
{
#ifdef DECANEE
    if (m_kUserInfo.GetCurrentChar().iCharType != GC_CHAR_DECANEE)
        return iOriginalSound;
    if (IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV1) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV2) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_DUN) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV1_DUN) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV2))
    {
        switch (iOriginalSound)
        {
        case 2000013:   return 2000031;
        case 2000014:   return 2000032;
        case 2000020:   return 2000035;
        case 2000022:   return 2000034;
        case 2000023:   return 2000035;
        case 2000026:   return 2000037;
        case 2000028:   return 2000040;
        case 2000029:   return 2000041;
        case 2000030:   return 2000044;
        default:        return iOriginalSound;
        }
    }
    else
#endif
        return iOriginalSound;
}

// 플레이어 고유의 소리와 관련됨
void PLAYER::Ah_Sound(char What, char Special_Delay)
{
    if (g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.y < -0.25f)
    {
        return;
    }
    int iCharType = g_MyD3D->MyPlayer[m_iPlayerIndex]->GetCurrentChar().iCharType;
    switch (What)
    {
    case 0:// 일반적인 억~
        Sound(GetEncoachSoundSwitch(GetCurFormTemplate().sSound.AH[rand() % 4]), 0);
        break;
    case 1:// 고통스러운 으아~
        if (g_MyD3D->MyPlayer[m_iPlayerIndex]->Is_IfDie2End || g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.y < -0.2f)
            Sound(GetEncoachSoundSwitch(GetCurFormTemplate().sSound.AHHH), 0);
        else
            Sound(GetEncoachSoundSwitch(GetCurFormTemplate().sSound.AHH), 0);
        break;
    case 5:// 필살기 준비
    {
        if (m_bBurnningSpecial)
            Sound(GetEncoachSoundSwitch(GetCurBurnningTemplate().sSound.ATK[rand() % 5]), 0);
        else
            Sound(GetEncoachSoundSwitch(GetCurFormTemplate().sSound.ATK[rand() % 5]), 0);
    }
    break;
    case 6:// 카운트 맞았을시
        if (GetCurFormTemplate().sSound.STAND[1] == -1)
            Sound(GetEncoachSoundSwitch(GetCurFormTemplate().sSound.STAND[0]), 0);
        else
            Sound(GetEncoachSoundSwitch(GetCurFormTemplate().sSound.STAND[rand() % 2]), 0);
        break;
    case 7:
        Sound(GetEncoachSoundSwitch(GetCurFormTemplate().sSound.AHHH), 0);
        break;
    }
}

void PLAYER::SetDamageEff(float x, float y, float z)
{
    for (int i = 0; i < (int)m_vecDamageEff.size(); ++i)
    {
        ITEM_EFFECT& stEffect = m_vecDamageEff[i];
        CreateItemEffectParam kParam(m_bShowEffect, bIsRight, stEffect, m_posDesc);
        kParam.x = x;
        kParam.y = y;
        kParam.z = z;
        CreateItemEffect(kParam);
    }

    for (std::vector<KGCAnimManager::DamageAnim>::iterator j = m_DamageAnim.begin(); j != m_DamageAnim.end(); j++)
    {
        g_AnimManager.FrameUpdateDamageAnim(&(*j), D3DXVECTOR3(x, y, z), bIsRight);
    }
}

void PLAYER::Rebirth(char cLife /* = 1 */)
{
    if (m_cLife == 1 && GetHP() <= 0.0f &&
        (uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
            uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER)))
    {
        // 부활 해야되!!!
    }
    else if (m_cLife > 0)
    {
        return;
    }

    // 부활하게 되면 피격 점수를 0로 초기화 시켜준다.
    // 부활하게 되면 피격 점수를 0로 초기화 시켜준다.
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
    {
        // 라이프가 0 일때 부활하는 것은 수반이나, 보너스로 추가로 살아난 것이므로 점수 초기화
        if (0 == m_cLife && IsLocalPlayer() && g_MyD3D->IsPlayerIndex(m_iPlayerIndex))
        {
            for (int j = 0; j < (int)g_pkQuestManager->m_alQuestPoint[m_iPlayerIndex].size(); ++j)
            {
                if (g_pkQuestManager->m_alQuestPoint[m_iPlayerIndex].count(j))
                    g_pkQuestManager->m_alQuestPoint[m_iPlayerIndex][j].Attacked = 0;

                g_pkQuestManager->m_PlayerQuestPoint[m_iPlayerIndex].Attacked = 0;
                SendNetworkData(GC_PID_QUEST_STAGE_POINT, j);
            }
        }
    }

    m_bBuffFrameLock = false;
    m_cLife = cLife;
    Arena();

    g_kCamera.m_bZoomViewMode = false;
    g_kCamera.SetTargetPlayer(m_iPlayerIndex);
    g_MyD3D->m_bLastFrameSend = false;
    m_iRegenCount++;
}

void PLAYER::ReverseGravity(int Attaker, float fGravity, float fLastGravity, bool bCheckContact, bool bSpecialDamage)
{
    // IsContact 체크 여부
    // 법사 ReverseGravity는 공중에 떠있어도 맞는다.
    if (uiMotion == GetOwnMotion(MID_COMMON_INTO_TELEPORT) ||
        CheckInPortal()) // 포탈안에 있으면 데미지는 쌩깐다 
        return;

    // 로난 방어막 생성된 상태면 쌩까자
    if (IsMagicEffect(EGC_EFFECT_RONAN_HOLY_LV3_SHIELD)) {
        return;
    }

    if (bCheckContact == true)
    {
        if (IsContact == false)
            return;
    }

    if (IsInvinsible(bSpecialDamage) == true)
        return;

    SetLastKillmePlayer(Attaker);
    SetLastMademeGravityPlayer(Attaker);
    Last_Gravity = fLastGravity;
    y_Speed = fGravity * 0.8f;
    Gravity = vPos.y;
    //Gravity = fGravity;
    cFrame = 0;

    SetPlayerState(MID_COMMON_DAMAGED_BY_FLYATK);
}

bool PLAYER::IsInvinsible(bool bSpecialDamage)
{
    bool bResult = true;
    if (m_kUserInfo.nUserState == GC_RUS_PLAYING &&
        m_cLife > 0 &&
        Super == 0 &&
        Item_Remain_Time[GC_GAME_ITEM_SUPER] == 0)
    {
        bResult = false;
    }

    // 필살기 데미지가 아닌 데미지만 맞도록 하는 처리
    // 기존 Super나 무적아이템으로 슈퍼가 된상태일때는 체크하지 않는다.
    if (false == bResult)
        bResult = IsSuperNotSpecial(bSpecialDamage);

    return bResult;
}

bool PLAYER::IsSuperNotSpecial(bool bSpecialDamage)
{
    bool bResult = false;
    if (false == bSpecialDamage && 0 < m_uiNotSpecialSuper)
    {
        bResult = true;
    }
    return bResult;
}

PLAYER& PLAYER::operator =(const PLAYER& right_)
{
    m_kUserInfo.strNickName = right_.m_kUserInfo.strNickName;
    m_kUserInfo.strNickColor = right_.m_kUserInfo.strNickColor;
    vPos.x = right_.vPos.x;
    vPos.y = right_.vPos.y;
    uiMotion = right_.uiMotion;
    Extra_Char_Num = right_.Extra_Char_Num;
    y_Ex = right_.y_Ex;
    m_cLife = right_.m_cLife;
    m_kUserInfo = right_.m_kUserInfo;
    cFrame = right_.cFrame;


    m_pLire3Arrow = right_.m_pLire3Arrow;

    for (int i = 0; i < ABILITY_MAX; ++i)
    {
        m_fAbility[i] = right_.m_fAbility[i];
    }

    return *this;
}

void PLAYER::SetCollectionData()
{
    SetCharacterCollectionAttributeList();
    SetVIPCollectionAttributeList();
}

void PLAYER::SetMyPlayer(int iSelChar, bool bNoAllocation_)
{
    if (!bNoAllocation_)
        m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    if (iSelChar != -1)
        m_kUserInfo.cCharIndex = iSelChar;

    //SetCollectionData();

    Calcualte_Point();

    m_kUserInfo.bLive = true;

    SetPlayerName(g_kGlobalValue.m_kUserInfo.strNickName.c_str(), g_kGlobalValue.m_kUserInfo.strNickColor.c_str());

    m_cLife = 0;

    //Character Item정보를 Player Item정보를 담는 Array로 복사합니다.
    //Adjust_Equip();

    vPos.x = 0;
    y_Ex = 0;

    cFrame = 0;

    Calcualte_Point();
    LoadPlayerTemplate();
}

void PLAYER::UpdateGlobalUserInfo()
{
    g_kGlobalValue.m_kUserInfo = m_kUserInfo;
}

void PLAYER::UpdateTempPlayerUserInfo()
{
    g_MyD3D->m_TempPlayer.m_kUserInfo = m_kUserInfo;
}

void PLAYER::BeforeStartGame()
{
    m_bDeathFromFall = false;
    m_cLife = 0;

    m_bRender = true;
    //m_bIsRenderBaseMesh = true;
    //m_bIsRenderBaseMeshStatic = false;

    if (m_kUserInfo.cCharIndex == -1)
    {
        m_kUserInfo.bLive = false;
        m_kUserInfo.nUserState = GC_RUS_NONE;
    }

    m_iSkillType = 0;
    Slot_x = 0;
    Slot_y = 0;
    Slot_Frame = 0;
    Item_Score = 0; // 점수 아이템을 얼마나 먹었나?
    m_vecKillCoinInfo.clear();
    m_bThrowMatchGP = false; //대전 GP뱉었나?
    Is_IfDie2End = false;
    First_Receive = false;
    Old_x = vPos.x;
    y_Speed = 0.0f;
    m_fFall_Speed = DEAULT_FALL_SPEED;
    Super = 0;
    m_bRenderQuestDrama = true;
    m_iWeaponType = 0;
    g_MyD3D->MyHead->IsBlackMagic = true;
    m_iDotaPoint = 0;
    m_bIsAgitBreaking = false;
    m_bUseNotAP = false;
    m_uiNotSpecialSuper = 0;
    m_bResultTower = false;
    m_iComboSkillHotKeyIdx = -1;
    m_iComboSkillComboIdx = -1;
    m_iComboSkillNowComboIdx = -1;
    x_WindSpeed = 0.0f;
    y_WindSpeed = 0.0f;
    m_fJump_WindRadio = 1.0f;
    m_iDamageSummonMonSlot = -1;
    m_iStopSpeedFrame = 0;
    m_bNotCheckSideFH = false;
    m_iSkipFrame = 0;
    m_iResetSkipFrame = 0;
    m_iContinueSkipFrame = 0;
    m_bIsAttackMiss = false;
    m_fMPDecreaseRatio = 1.0f;
    m_dwMyEmbarkUID = UINT_MAX;
    m_iEmbarkAdaptBoneID = -1;
    m_dwEmbarkSendCool = 0;
    m_vecPermissionDamage.clear();
    m_bDirectKill = false;
    m_bBuffFrameLock = false;
    m_iEmbarkTypeOfInPortal = INT_MAX;
    m_dwEmbarkUIDOfInPortal = INT_MAX;
    m_vDamageStartPos = D3DXVECTOR2(0.0f, 0.0f);
    m_vDamageWhoPos = D3DXVECTOR2(0.0f, 0.0f);

    m_bCheckDieSummons = false;
    m_bNotTargeting = false;
    m_iOriginalTeam = m_kUserInfo.iTeam;
    m_nBloodPoint = 6;

    g_MyD3D->SetEnableKeyInput(true);

    if (g_MyD3D->MyReplay->m_iCurrState == RP_NONE && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO)
    {
        for (int i = 0; i < 3; ++i)
            m_vecCharNum.push_back(m_kUserInfo.aiTagSlot[i]);

        int iCharNum = m_vecCharNum[0];
        m_vecCharNum.erase(m_vecCharNum.begin());
        m_kUserInfo.SetCurrentChar(iCharNum);
    }
    else if (GC_GMC_TAG_MATCH == SiKGCRoomManager()->GetGameModeCategory())
    {
        m_kUserInfo.SetCurrentChar(m_kUserInfo.aiTagSlot[0]);

        std::vector< int >::iterator vit;
        for (int i = TAG_CHAR_NUM - 1; i >= 0; i--)
        {
            vit = std::find(m_vecTagCharNum.begin(), m_vecTagCharNum.end(), m_kUserInfo.aiTagSlot[i]);
            if (vit == m_vecTagCharNum.end())
                m_vecTagCharNum.push_back(m_kUserInfo.aiTagSlot[i]);
        }
    }

    Calcualte_Point(true, true);

#if defined( CHAR_ABILITY_STATE )
    CalcAbilityState();
#endif

    if (m_mapTutorialDefaultEquipSkillA.empty() ||
        m_mapTutorialDefaultEquipSkillB.empty() ||
        m_mapCashSkillInfo.empty()) {
        LoadBaseSkillInfo();
    }

    LoadPlayerTemplate();

    std::vector<int>::iterator vecIter = GetCurFormTemplate().vecSummonMonsterID.begin();
    for (; vecIter != GetCurFormTemplate().vecSummonMonsterID.end(); ++vecIter)
    {
        g_MyD3D->CreateMeshAnimationForPrevLoad(*vecIter);
    }

    if (SiKGCRoomManager()->IsMonsterGameMode())
    {
        if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DOTA)
        {
            m_kUserInfo.iTeam = ETBlue;
            g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.iTeam = ETBlue;
        }
        else if (IsLocalPlayer())
        {
            m_kUserInfo.iTeam = g_kGlobalValue.m_kUserInfo.iTeam;
        }

        g_kGlobalValue.m_MonsterMake = 0;
    }

    m_cLife = 3;
    char cEventPlus = 2;

    if (GC_GM_INDIGO_TEAM != SiKGCRoomManager()->GetGameMode())
    {
        if (g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH ||
            g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS ||
            g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH_REVERS)
        {
            m_cLife = 5;
            cEventPlus = 4;
        }
    }

    if (GC_GMC_MATCH == SiKGCRoomManager()->GetGameModeCategory() && SiKGCEventManager()->IsActiveEvent(EVENT_ID_FATAL_MATCH))
    {
        m_cLife += cEventPlus;
    }

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME)
        m_cLife = 1;

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG)
        m_cLife = 120;

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
    {
        if (SiKGCRoomManager()->IsBreakMode() && SiKGCRoomManager()->IsHellMode())
        {
            m_cLife = 1;
        }
        else
        {
            m_cLife = SiKGCWorldMapManager()->GetDungeonCharBaseLife(SiKGCRoomManager()->GetGameMode());

            if (SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST64 &&
                SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST81 &&
                SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST82 &&
                SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST99 &&
                SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST102 &&
                SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST98)
            {
                if (g_kGlobalValue.CheckPremium(KPremiumInfo::TYPE_GCPREMIUM))
                {
                    m_cLife++;
                }
            }
        }
    }

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH)
        m_cLife = 120;

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
        m_cLife = 120;

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA)
        m_cLife = 120;

    if (GC_GMC_TAG_MATCH == SiKGCRoomManager()->GetGameModeCategory())
        TagMatchInit();

    SetCommonMontion();

    m_bDecChangeWeapon = false;

    if (IsLocalPlayer())
    {
        if (g_pkUIScene->m_pkGameOverlayUI)
            g_pkUIScene->m_pkGameOverlayUI->ResetComboSkill();
    }

    g_kGlobalValue.m_mapCharacterStats.clear();

#if defined( DUNGEONPLAYSTATE )
    g_kGlobalValue.m_mapDungeonPlayState.clear();
#endif

#if defined( USE_CAMERA_ZOOM_STATE )
    m_mapCameraZoomState.clear();
#endif    
}

void PLAYER::SetCommonMontion()
{
    m_vecCommonMotionList.clear();
    m_vecCommonMotionList.push_back(MID_COMMON_SMALLDAMAGE);
    m_vecCommonMotionList.push_back(MID_COMMON_BIGDAMAGE);
    m_vecCommonMotionList.push_back(MID_COMMON_DOWN_AND_STANDUP);
    m_vecCommonMotionList.push_back(MID_COMMON_DOWN_IN_SKY);
    m_vecCommonMotionList.push_back(MID_COMMON_ARENA);
    m_vecCommonMotionList.push_back(MID_COMMON_DROP);
    m_vecCommonMotionList.push_back(MID_COMMON_FRONTDOWN_AND_STANDUP);
    m_vecCommonMotionList.push_back(MID_COMMON_STONECURSE);
    m_vecCommonMotionList.push_back(MID_COMMON_STONECURSE_BY_MEDUSA);
    m_vecCommonMotionList.push_back(MID_COMMON_DAMAGED_BY_FLYATK);
    m_vecCommonMotionList.push_back(MID_COMMON_SHOPPOS);
    m_vecCommonMotionList.push_back(MID_COMMON_INTO_THE_TELEPORT);
    m_vecCommonMotionList.push_back(MID_COMMON_INTROPOS);
    m_vecCommonMotionList.push_back(MID_COMMON_TIGER_ROLLING_ACTION);
    m_vecCommonMotionList.push_back(MID_COMMON_INTO_BASILISK);
    m_vecCommonMotionList.push_back(MID_COMMON_ENT_TRUNK);
    m_vecCommonMotionList.push_back(MID_COMMON_FREEZED_BY_PIXIE);
    m_vecCommonMotionList.push_back(MID_COMMON_FREEZED_BY_LICH);
    m_vecCommonMotionList.push_back(MID_COMMON_INFECT_BOMB);
    m_vecCommonMotionList.push_back(MID_COMMON_DAMAGED_BY_GUARD);
    m_vecCommonMotionList.push_back(MID_COMMON_DAMAGED_BY_GUARD_ENABLE_COUNTER);
    m_vecCommonMotionList.push_back(MID_COMMON_HOLD);
    m_vecCommonMotionList.push_back(MID_COMMON_DAMAGED_SPIKE_TRAP);
    m_vecCommonMotionList.push_back(MID_COMMON_HOLD_NOEFFECT);
    m_vecCommonMotionList.push_back(MID_COMMON_STUN);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_ELESIS);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LIRE);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_ARME);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_ELESIS2);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LIRE2);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_ARME2);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LAS);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LAS2);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_ELE3);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LIRE3);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LAS3);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RYAN);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_FORM_WOLF);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RYAN2);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_NEPHILLIM);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_ELESIS4);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_ARME4);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LIRE4);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_LAS4);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RONAN);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RYAN3);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RONAN2);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RYAN4);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_MDAN);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_AMY1);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RONAN3);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_AMY2);
    m_vecCommonMotionList.push_back(MID_FIGHT_CATCH_BY_AMY2);
    m_vecCommonMotionList.push_back(MID_COMMON_CATCH_BY_RONAN4);
    m_vecCommonMotionList.push_back(MID_AMY3_DANCE_CATCHED);
    m_vecCommonMotionList.push_back(MID_AMY3_CATCHED);
    m_vecCommonMotionList.push_back(MID_JIN_CATCHED);
    m_vecCommonMotionList.push_back(MID_JIN1_CATCHED);
    m_vecCommonMotionList.push_back(MID_SIEG1_CATCHED);
    m_vecCommonMotionList.push_back(MID_JIN2_CATCHED);
    m_vecCommonMotionList.push_back(MID_JIN2_2_CATCHED);
    m_vecCommonMotionList.push_back(MID_AMY4_MELO_CATCHED);
    m_vecCommonMotionList.push_back(MID_AMY4_CATCHED);
    m_vecCommonMotionList.push_back(MID_SIEG2_CATCHED);
    m_vecCommonMotionList.push_back(MID_JIN4_CATCHED);
    m_vecCommonMotionList.push_back(MID_SIEG3_CATCHED);
    m_vecCommonMotionList.push_back(MID_MARI1_CATCHED);
    m_vecCommonMotionList.push_back(MID_MARI2_CATCHED);
    m_vecCommonMotionList.push_back(MID_SIEG4_CATCHED);
    m_vecCommonMotionList.push_back(MID_SIEG4_CATCHED_FURY);
    m_vecCommonMotionList.push_back(MID_MARI3_CATCHED);
    m_vecCommonMotionList.push_back(MID_DIO1_CATCHED);
    m_vecCommonMotionList.push_back(MID_ZERO1_CATCHED);
    m_vecCommonMotionList.push_back(MID_LEY1_CATCHED);
    m_vecCommonMotionList.push_back(MID_MARI4_CATCHED);
    m_vecCommonMotionList.push_back(MID_RIN1_CATCHED);
    m_vecCommonMotionList.push_back(MID_ASIN1_CATCHED);
    m_vecCommonMotionList.push_back(MID_ASIN1_SENDED);
    m_vecCommonMotionList.push_back(MID_COMMON_SMALLDAMAGE_NO_COUNTER);
    m_vecCommonMotionList.push_back(MID_COMMON_BIGDAMAGE_NO_COUNTER);
    m_vecCommonMotionList.push_back(MID_COMMON_DOWN_AND_STANDUP_NO_COUNTER);
    m_vecCommonMotionList.push_back(MID_COMMON_DIO_SPECIAL4_HITTED);
    m_vecCommonMotionList.push_back(MID_COMMON_AMY1_SPECIAL4_HITTED);
    m_vecCommonMotionList.push_back(MID_RIN2_EVIL_CATCHED);
    m_vecCommonMotionList.push_back(MID_COMMON_DAMAGED_COUNTER_SPIKE_TRAP);
    m_vecCommonMotionList.push_back(MID_COMMON_FRONT_FALL_DOWN);
    m_vecCommonMotionList.push_back(MID_RIN3_COMMON_GODDESS_UNIQUE_SKILL_1);
    m_vecCommonMotionList.push_back(MID_RIN4_COMMON_SPEICLA2_HOLD);
    m_vecCommonMotionList.push_back(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_DIE);
    m_vecCommonMotionList.push_back(MID_COMMON_DIO_SPECIAL4_HITTED);
    m_vecCommonMotionList.push_back(MID_NEPHILIM_DIO_SPECIAL4_HITTED);
    m_vecCommonMotionList.push_back(MID_MDAN_DIO_SPECIAL4_HITTED);
    m_vecCommonMotionList.push_back(MID_COMMON_ZERO3_OS_UNIQUE_HOLD);
    m_vecCommonMotionList.push_back(MID_ZERO4_COMMON_DAMAGED_BLACKHALL);
    m_vecCommonMotionList.push_back(MID_COMMON_SLIDING);
    m_vecCommonMotionList.push_back(MID_COMMON_DAMAGED_DIO3_TENACIOUS_HOLD);
    m_vecCommonMotionList.push_back(MID_COMMON_LEY3_GAROUILLE_ICE_SLIDING);
    m_vecCommonMotionList.push_back(MID_COMMON_LEY3_HAUNT_CURSE4_DAMAGE);
    m_vecCommonMotionList.push_back(MID_COMMON_LEONA_TRANSFORM);


    // 이모티콘 모션 담기 
    std::vector< int > vecEmoticonMotion;
    SiKGCSocialMotionManager()->GetEmoticonMotionList(vecEmoticonMotion);
    std::vector< int >::const_iterator vitMotion = vecEmoticonMotion.begin();
    for (; vitMotion != vecEmoticonMotion.end(); ++vitMotion) {
        m_vecCommonMotionList.push_back(*vitMotion);
    }

    // 탑승물 관련 모션
    std::vector<int> vecEmbarkMotionList = SiKGCEmbarkManager()->GetCharMotionList();
    std::vector<int>::iterator vecEmbarkMotionIter = vecEmbarkMotionList.begin();
    for (; vecEmbarkMotionIter != vecEmbarkMotionList.end(); ++vecEmbarkMotionIter)
    {
        m_vecCommonMotionList.push_back(*vecEmbarkMotionIter);
    }
}

void PLAYER::PresetTutorialChars(std::string strEquipScrip /*= "TutorialEquip.lua" */)
{
    ClearPlayerData();

    SCharInfo kInfo;
    SUserItem _Item;
    _Item.dwItemUID = 0;

    kInfo.iLevel = 0;
    kInfo.biExp = 100;

    m_kUserInfo.ClearChar();

    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, strEquipScrip) == false)
        return;

    int CharCount;
    LUA_BEGIN_TABLE("Tutorial", return);
    {
        LUA_GET_VALUE("CharNum", CharCount, return);

        if (CharCount > GC_CHAR_NUM + 1)
        {
            assert(0);
            return;
        }

        // 이제 부터 진짜!
        LUA_BEGIN_TABLE("Char", return);
        {
            for (int i = 0; i < CharCount; i++)
            {
                const char* TableName;
                std::wstringstream WTableName;
                WTableName << L"Char" << i;
                TableName = GCUTIL_STR::GCStrWideToChar(WTableName.str().c_str());

                LUA_BEGIN_TABLE(TableName, continue);
                {
                    kInfo.iCharType = i;

                    const char* JobLevel;
                    for (int j = 0; j < NUM_JOB_LEVEL; j++)
                    {
                        kInfo.setPromotion.insert(j);
                        kInfo.iPromotionLevel = j;

                        std::wstringstream WTableName;

                        WTableName << L"JobLevel" << j << L"_SelectEquipOnSkill";
                        JobLevel = GCUTIL_STR::GCStrWideToChar(WTableName.str().c_str());
                        if (luaMgr.BeginTable(JobLevel) == S_OK) {
                            for (int k = 1; ; k++) {
                                WTableName.str(L"");
                                WTableName << L"Equip" << k;
                                JobLevel = GCUTIL_STR::GCStrWideToChar(WTableName.str().c_str());
                                if (luaMgr.BeginTable(JobLevel) == S_OK)
                                {
                                    int eSkillID;
                                    LUA_GET_VALUE_DEF("SkillID", eSkillID, -1);
                                    if (IsLearnedSkill(static_cast<EGCSkillTree>(eSkillID))) {
                                        kInfo.vecItemInfo.clear();
                                        if (luaMgr.BeginTable("ItemID") == S_OK)
                                        {
                                            for (int l = 1; ; ++l)
                                            {
                                                int nItemID = 0;
                                                LUA_GET_VALUE_NOASSERT(l, nItemID, break);

                                                // 그냥 세트 무조건 입히자 답없다 ㅡ.ㅡ;;
                                                if (l == 1)
                                                {
                                                    if (nItemID == -1)
                                                    {
                                                        g_MyD3D->m_kItemMgr.EquipBaseItem(&kInfo);
                                                    }
                                                    else
                                                    {
                                                        g_MyD3D->m_kItemMgr.EquipSetItem(&kInfo, nItemID);
                                                    }
                                                }
                                                else
                                                {
                                                    _Item.iItemID = nItemID;
                                                    GCItem* iTem = g_MyD3D->m_kItemMgr.GetItemData(nItemID);
                                                    CONTINUE_NIL(iTem);
                                                    g_MyD3D->m_kItemMgr.EquipItem(&kInfo, iTem);
                                                }
                                            }
                                            luaMgr.EndTable();
                                        }
                                    }
                                    luaMgr.EndTable();
                                }
                                else {
                                    break;
                                }
                            }
                            luaMgr.EndTable();
                        }
                        else {
                            WTableName.str(L"");
                            WTableName << L"JobLevel" << j;
                            JobLevel = GCUTIL_STR::GCStrWideToChar(WTableName.str().c_str());
                            if (SUCCEEDED(luaMgr.BeginTable(JobLevel)))
                            {
                                kInfo.vecItemInfo.clear();
                                for (int k = 1; ; ++k)
                                {
                                    int nItemID = 0;
                                    LUA_GET_VALUE_NOASSERT(k, nItemID, break);

                                    // 그냥 세트 무조건 입히자 답없다 ㅡ.ㅡ;;
                                    if (k == 1)
                                    {
                                        if (nItemID == -1)
                                        {
                                            g_MyD3D->m_kItemMgr.EquipBaseItem(&kInfo);
                                        }
                                        else
                                        {
                                            g_MyD3D->m_kItemMgr.EquipSetItem(&kInfo, nItemID);
                                        }
                                    }
                                    else
                                    {
                                        _Item.iItemID = nItemID;
                                        GCItem* iTem = g_MyD3D->m_kItemMgr.GetItemData(nItemID);
                                        CONTINUE_NIL(iTem);
                                        g_MyD3D->m_kItemMgr.EquipItem(&kInfo, iTem);
                                    }
                                }
                                LUA_END_TABLE(return);
                            }
                        }

                        // 만렙으로 설정.
                        kInfo.iLevel = MAX_LEVEL;

                        if (kInfo.iCharType == GC_CHAR_DIO || kInfo.iCharType == GC_CHAR_LEY || kInfo.iCharType == GC_CHAR_ZERO)
                            kInfo.kSkillInfo.m_nLvSPPoint = 120;
                        else
                            kInfo.kSkillInfo.m_nLvSPPoint = 160;

                        // 추가
                        m_kUserInfo.AddChar(kInfo);
                    }
                }
                LUA_END_TABLE(continue);
            }
        }
        LUA_END_TABLE(return);
        //====================캐릭터 추가 끝=====================================//
    }
    LUA_END_TABLE(return);

    if (m_mapTutorialDefaultEquipSkillA.empty() ||
        m_mapTutorialDefaultEquipSkillB.empty() ||
        m_mapCashSkillInfo.empty()) {
        LoadBaseSkillInfo();
    }
}

void PLAYER::LoadBaseSkillInfo()
{
    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, "TutorialEquip.lua") == false)
        return;

    m_mapTutorialDefaultEquipSkillA.clear();
    m_mapTutorialDefaultEquipSkillB.clear();
    LUA_BEGIN_TABLE("TutorialDefaultSkill", return);
    {
        // 이제 부터 진짜!
        LUA_BEGIN_TABLE("Char", return);
        {
            for (int i = 0; i < GC_CHAR_NUM; i++)
            {
                const char* TableName;
                std::wstringstream WTableName;
                WTableName << L"Char" << i;
                TableName = GCUTIL_STR::GCStrWideToChar(WTableName.str().c_str());
                LUA_BEGIN_TABLE(TableName, continue);
                {
                    const char* JobLevel;
                    // 필살기 타입 A
                    for (int j = 0; j < NUM_JOB_LEVEL; j++)
                    {
                        std::vector< EGCSkillTree > vecSkill;
                        std::wstringstream WTableName;
                        WTableName << L"TypeA_JobLevel" << j;
                        JobLevel = GCUTIL_STR::GCStrWideToChar(WTableName.str().c_str());
                        if (SUCCEEDED(luaMgr.BeginTable(JobLevel)))
                        {
                            for (int k = 1; ; ++k)
                            {
                                int nItemID = 0;
                                LUA_GET_VALUE_NOASSERT(k, nItemID, break);

                                vecSkill.push_back(static_cast<EGCSkillTree>(nItemID));
                            }
                            LUA_END_TABLE(continue);
                        }
                        if (vecSkill.empty() == false) {
                            m_mapTutorialDefaultEquipSkillA.insert(std::make_pair(std::make_pair(i, j), vecSkill));
                        }
                    }

                    // 필살기 타입 B
                    for (int j = 0; j < NUM_JOB_LEVEL; j++)
                    {
                        std::vector< EGCSkillTree > vecSkill;
                        std::wstringstream WTableName;
                        WTableName << L"TypeB_JobLevel" << j;
                        JobLevel = GCUTIL_STR::GCStrWideToChar(WTableName.str().c_str());
                        if (SUCCEEDED(luaMgr.BeginTable(JobLevel)))
                        {
                            for (int k = 1; ; ++k)
                            {
                                int nItemID = 0;
                                LUA_GET_VALUE_NOASSERT(k, nItemID, break);

                                vecSkill.push_back(static_cast<EGCSkillTree>(nItemID));
                            }
                            LUA_END_TABLE(continue);
                        }
                        if (vecSkill.empty() == false) {
                            m_mapTutorialDefaultEquipSkillB.insert(std::make_pair(std::make_pair(i, j), vecSkill));
                        }
                    }
                }
                LUA_END_TABLE(continue);
            }
        }
        LUA_END_TABLE(return);
        //====================캐릭터 추가 끝=====================================//
    }
    LUA_END_TABLE(return);

    m_mapCashSkillInfo.clear();

    if (luaMgr.BeginTable("CashSkill") == S_OK)
    {
        for (int i = 1; luaMgr.BeginTable(i) == S_OK; ++i)
        {
            SCashSkillInfo sCashSkillInfo;

            int iChar = -1;
            int iPromotion = -1;
            luaMgr.GetValue("CharNum", iChar);
            luaMgr.GetValue("Promotion", iPromotion);
            luaMgr.GetValue("SkillType", sCashSkillInfo.m_iSkillType);
            luaMgr.GetValue("SkillLv", sCashSkillInfo.m_iSkillLv);
            luaMgr.GetValue("SkillID", sCashSkillInfo.m_iSkillID);

            if (luaMgr.BeginTable("SkillGoodsID") == S_OK)
            {
                for (int j = 1; ; ++j)
                {
                    GCITEMID dwItemID = -1;
                    if (luaMgr.GetValue(j, dwItemID) == E_FAIL)
                        break;
                    sCashSkillInfo.m_vecGoodsID.push_back(dwItemID);
                }
                luaMgr.EndTable();
            }

            if (iChar != -1 && iPromotion != -1) {
                m_mapCashSkillInfo[std::make_pair(iChar, iPromotion)].push_back(sCashSkillInfo);
            }
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }
}

void PLAYER::InitTutorialChar(int iChar_, int iExtraNum_)
{
#if defined( COLLECT_FRAME_DATA )
    g_kGlobalValue.StartCollectFrameData();
#endif
    // 2008.01.07 wony
    // 게임 서버에 접속하지 않고 바로 연습모드에 들어가면..
    // 옵져버 구조체와 유져 정보 구조체의 초기값이 모두 초기값인 0이기 때문에 옵져버로 표시된다
    // 그 문제 해결을 위해 넣은 코드임..
    if (g_kGlobalValue.m_kUserInfo.dwUID == 0) // 초기값이면 아직 한번도 게임서버에 접속하지 않은 상태이다
        g_kGlobalValue.m_kUserInfo.dwUID = 1;   // 대충 1로 해놓자..-ㅅ -

    //아이템이 안나오는 버그를 없애기 위해...
    g_kGlobalValue.m_kUserInfo.bHost = true;
    m_kUserInfo.bHost = true;
    m_kUserInfo.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
    m_kUserInfo.cCharIndex = iChar_;
    m_kUserInfo.bLive = true;
    m_kUserInfo.nUserState = GC_RUS_PLAYING;
    bIsRight = true; // m_kUserInfo.MyID사용하고, 살아있고, 오른쪽 보고있고
    vPos.x = (float)g_MyD3D->MyStg->Num_Width * 0.5f; // Player의 초기위치가 Map의 정보를 참조 하기 때문에 항상 Map을 먼저 Load
    vPos.y = 0.8f;
    Old_x = vPos.x;
    m_cLife = 1;
    pLatency = LATENCY;
    g_MyD3D->MyHead->Old_Life[0] = g_MyD3D->MyHead->Change_Life[0] = m_cLife;
    y_Speed = 0.0f;
    m_fFall_Speed = DEAULT_FALL_SPEED;
    Super = 0;
    m_iWeaponType = 0;
    Calcualte_Point();
    if (iExtraNum_ != -1) {
        Extra_Char_Num = iExtraNum_;
    }
    LoadPlayerTemplate();
    //↓이 함수 잘못짜여져 있어 수정했음 아래꺼 쓸일 없을것임 wony
    //SetSkillType(0);
    SetPlayerState(GetWaitMotion());
    CreatePlayer();

    //소환몬스터 미리 로드( 안하면 소환할때 렉걸림 )
    std::vector<int>::iterator vecIter = GetCurFormTemplate().vecSummonMonsterID.begin();
    for (; vecIter != GetCurFormTemplate().vecSummonMonsterID.end(); ++vecIter)
    {
        g_MyD3D->CreateMeshAnimationForPrevLoad(*vecIter);
    }

    //if( Extra_Char_Num == CID_ARME1 )
    //{
    //    Shield = 50;
    //    m_cUseShield = CID_ARME1;
    //}

    for (int i = 0; i < FORM_NUM; ++i)
    {
        if (m_pObjMetaForm[i])
            m_pObjMetaForm[i]->SetShellMatrixScale(DEFAULT_PLAYER_SCALE_SIZE);
    }

    KGCPlayerCommon::MakeSoundLoadList();
    MakeSoundLoadList();
    MakeFireInfo3DObjectLoadList();

}

void PLAYER::EndGame()
{
    InitRangeInfo();
    // 게임에서 나올때~ 무기채인지로 바꾼 아이템은 고이접어두고 원래 무기를 차야할것만 같자나!!!!! 그렇지?
    InitPromotionTemplate();

    // Ryan4 게임 끝나고 무기 해제 상태로 변경시켜주는 부분
    if (Extra_Char_Num == CID_RYAN4)
    {
        ToggleExtraMesh("WEAPON", true);
        ToggleExtraMesh("MERGE_WEAPON", false);
    }

    Body_Angle = 0.0f;

    vPos.x = 0.0f;
    y_Ex = 0.0f;

    cFrame = 0;
    g_MyD3D->MyHead->IsBlackMagic = true;
    m_kUserInfo.nUserState = GC_RUS_NONE;
    m_bResultTower = false;

    m_vecCharNum.clear();
    m_vecTagCharNum.clear();
    m_vecTagCharNum.reserve(2);

    // 키 커맨드 입력제어 초기화
    g_MyD3D->SetEnableKeyInput(true);

    // 2007/4/5. iridology. 
    // UpdatePlayerResource()에서 만든 놈을 여기서 지워버리면 대략 낭패이기 때문에 위로 옮김
    ReleaseTagPlayerResource();

    // 게임 시작전 플레이어로 되돌린다.
    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL && g_MyD3D->MyReplay->m_iCurrState != RP_VIEW)
    {
        if (m_kUserInfo.bLive && (m_iPlayerIndex >= 0 && m_iPlayerIndex < MAX_PLAYER_NUM))
        {
            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH &&
                g_pGameBoard->m_pStartingMember[m_iPlayerIndex].dwUID == m_kUserInfo.dwUID &&
                m_kUserInfo.GetCurrentChar().iCharType != g_pGameBoard->m_pStartingMember[m_iPlayerIndex].m_iChar)
            {
                m_kUserInfo.SetCurrentChar(g_pGameBoard->m_pStartingMember[m_iPlayerIndex].m_iChar);
            }
            Calcualte_Point();
            RemovePlayerObject();
            UpdatePlayerResource();

            if (IsLocalPlayer() == true)
                UpdateGlobalUserInfo();
        }
    }

    // 게임에서 사용했던 리소스 해제
    ReleasePlayerResourceOnGame();

    m_iFlashEffectFrame = 0;
    m_iShakeEffectFrame = 0;

    //g_MyD3D->MyBuff->CleanBuff( true );
    //g_pMagicEffect->ClearMagicEffect();
    g_pMagicEffect->ClearBuffInstace();

    //로딩중이던 플레이어 일지도?!
    m_LoadingState.Init();

    InitMonoVolt();
    InitAbyssHandEffect(); // 어비스나이트 왼손 이펙트 꺼주자
    DeleteMagicShield();

    // 진 버닝모드 관련한 것들을 초기화 시키자
    //SetNormalTpye();

    // 캐릭터 통계데이터 초기화
    g_kGlobalValue.m_mapCharacterStats.clear();

#if defined( DUNGEONPLAYSTATE )
    // 던전 플레이 통계데이터 초기화
    g_kGlobalValue.m_mapDungeonPlayState.clear();
#endif

}

void PLAYER::EffectInit()
{
    //if( !m_kUserInfo.bLive )
    //    return;

    if (m_kUserInfo.cCharIndex == -1)
        return;

    InitAbyssHandEffect(); // 파티클 중복 생성될 우려를 감안해서 로난3차(어비스나이트) 왼손 이펙트 켜기 전 초기화 먼저 해주자F

    //스페셜 이펙트
    switch (Extra_Char_Num)
    {
    case CID_LAS1:
    {
        m_pLasFireWingLeft = g_ParticleManager->CreateSequence("LasFireWingLeft", 0, 0, 0.5f);
        m_pLasFireWingRight = g_ParticleManager->CreateSequence("LasFireWingRight", 0, 0, 0.5f);
        m_pLasMotalBlowReady = g_ParticleManager->CreateSequence("LasMotalBlowReady", 0, 0, 0.5f);

        CKTDGAnim::CAnimInstance* tempAnim = g_MyD3D->m_pMapAnim->CreateInst(g_MyD3D->m_pMapAnim->GetSequence("MotalBlow"));
        g_MyD3D->m_pMapAnim->DeleteInst(tempAnim);
        m_pLasMotalBlowReadyAnim = g_MyD3D->m_pMapAnim->CreateInst(g_MyD3D->m_pMapAnim->GetSequence("MotalReady"));
        m_pLasMotalBlowReadyAnim->GetMatrix()->Scale(0.0013f, 0.0013f, 1.0f);
    }
    break;
    }
    m_pDashEff = g_ParticleManager->CreateSequence("DashEff", 0, 0, 0.0f);

    // 강화 아이템 착용 여부를 확인하고 이펙트를 생성한다.
    GCItem* itemData;
    CHAR_IDENTY charID;
    char cCharNum = Extra_Char_Num;

    bool bProtectRing = true;	// 수반 Effect 표시

    charID = (CHAR_IDENTY)cCharNum;
    m_posDesc.charID = charID;
    SCharInfo* pCharInfo = &GetCurrentChar();
    if (pCharInfo == NULL)
        g_kGlobalValue.m_strNullFunc = __FUNCTION__;

    // [12/13/2007] breadceo. 초기화
    m_bKamikiDefence = false;
    m_bPumpkinDefence = false;
    m_bKazeazeDefence = false;

    //  히어로 아이템 버프 리셋
    g_pItemMgr->InitHeroItemSpecialAbility(m_iPlayerIndex);

    ITEM_EFFECT stEffect;
    KItemIDVector vecEquipItemInfo = pCharInfo->GetEquipmentForLoadModel(m_iPlayerIndex);
    for (int j = 0; j < (int)vecEquipItemInfo.size(); j++)
    {
        int iItemNID = vecEquipItemInfo[j];

        if (iItemNID == 5357)
            continue;

        itemData = g_MyD3D->m_kItemMgr.GetItemData(iItemNID);
        CONTINUE_NIL(itemData);

        // 무기의 경우 코디에 이펙트가 있으면 코디를, 
        // 코디에 이펙트 없고 장착 무기에 이펙트 없으면 무기를 
        if (itemData->dwSlotPosition & ESP_WEAPON) {
            char str[15] = "";
            std::string strItemID;
            strItemID = itoa(iItemNID, str, 10);

            int iEnchant = 0;;
            int iStrong = itemData->iStrongLevel;
            if (iStrong < 5) {
                iEnchant = 0;
            }
            else if (iStrong >= 5 && iStrong < 8) {
                iEnchant = 1;
            }
            else if (iStrong == 8) {
                iEnchant = 2;
            }
            else if (iStrong == 9) {
                iEnchant = 3;
            }

            if (0 == g_ParticleManager->GetItemEffect(EGCDamage, strItemID, charID, iEnchant, m_vecDamageEff)) {
                GCITEMID weaponItemID = pCharInfo->GetEquipWeaponItemID();
                if (iItemNID != weaponItemID) {
                    iItemNID = weaponItemID;
                    itemData = g_MyD3D->m_kItemMgr.GetItemData(iItemNID);
                    CONTINUE_NIL(itemData);
                }
            }
        }

        char str[15] = "";
        std::string strItemNID;
        strItemNID = itoa(iItemNID, str, 10);

        int enchantLevel;
        int iStrongLevel = itemData->iStrongLevel;
        EITEMATTRIBUTE eAttribute = itemData->eAttribute;

        if (iItemNID == 14915)
        {
            iStrongLevel = 9;
            if (g_kGlobalValue.m_vecNecklace.size() > 0) {
                std::random_shuffle(g_kGlobalValue.m_vecNecklace.begin(), g_kGlobalValue.m_vecNecklace.end());
                std::vector<int>::iterator mit = g_kGlobalValue.m_vecNecklace.begin();
                eAttribute = static_cast<EITEMATTRIBUTE>(*mit);
            }
            else
            {
                eAttribute = EIATTR_FIRE;
            }
        }

        if (eAttribute != EIATTR_NONE)
        {
            //  발찌면
            if (itemData->dwSlotPosition & ESP_A_SHOES)
            {
                if (m_Elemental == ITEM_ELEMENTAL_NONE)
                    m_Elemental = g_MyD3D->m_kItemMgr.GetElementalType(eAttribute);
            }
            else
            {
                m_Elemental = g_MyD3D->m_kItemMgr.GetElementalType(eAttribute);
                m_iAbyssHandEff = eAttribute;
            }

            strItemNID = g_MyD3D->m_kItemMgr.EnumElementalTypetoStr(g_MyD3D->m_kItemMgr.GetElementalType(eAttribute));
        }

        if (iStrongLevel < 5)
        {
            enchantLevel = 0;
        }
        else if (iStrongLevel >= 5 && iStrongLevel < 8)
        {
            enchantLevel = 1;
        }
        else if (iStrongLevel == 8)
        {
            enchantLevel = 2;
        }
        else if (iStrongLevel == 9)
        {
            enchantLevel = 3;
        }

        //발찌는 자신의 강화레벨이 아니므로 - 목걸이만 강화레벨이다.
        if (eAttribute != EIATTR_NONE && m_StrongLevel < iStrongLevel)
        {
            //  발찌가 아니면
            if (!(itemData->dwSlotPosition & ESP_A_SHOES))
                m_StrongLevel = iStrongLevel;
        }


        int iVecSize = (int)(m_vecItemEff.size() + m_ItemAnim.size()); // 일단 여기서 이전 사이즈를 저장하고

        // 아이템 이펙트 정보를 얻어온다.
        int iAddedItemParticleNum = g_ParticleManager->GetItemEffect(EGCItem, strItemNID, charID, enchantLevel, m_vecItemEff);
        g_ParticleManager->GetItemEffect(EGCDamage, strItemNID, charID, enchantLevel, m_vecDamageEff);
        g_ParticleManager->GetItemEffect(EGCSkill, strItemNID, charID, enchantLevel, m_vecSkillEff);

        int iAddedItemAnimNum = g_AnimManager.GetNewItemAnim(strItemNID, charID, m_ItemAnim, enchantLevel);
        g_AnimManager.GetNewDamageAnim(strItemNID, charID, m_DamageAnim, enchantLevel);
        g_AnimManager.GetNewSkillAnim(strItemNID, charID, m_SkillAnim, enchantLevel);

        //새로 추가된 anim 중에 겹쳐서는 안되는 아이가 잇는지 찾는다.
        for (int i = 0; i < iAddedItemAnimNum; i++)
        {
            //겹쳐서는 안되는 아이가 잇따!!!
            if (m_ItemAnim[m_ItemAnim.size() - 1 - i].positionIndex)
            {
                std::set<int> setAddedItemParticle;
                std::set<int> setAddedItemAnim;

                for (int j = 0; j < iAddedItemParticleNum; j++)
                {
                    //if ( m_vecItemEff.size() - 1 - j < 0 || m_vecItemEff.size() < m_vecItemEff.size() - 1 - j )
                    //{
                        // 뭔지는 몰라도 잘못되었따.
                    //    return;
                    //}

                    setAddedItemParticle.insert(m_vecItemEff.size() - 1 - j);
                }

                for (int j = 0; j < iAddedItemAnimNum; j++)
                {
                    //if ( m_ItemAnim.size() - 1 - j < 0 || m_ItemAnim.size() < m_ItemAnim.size() - 1 - j )
                    //{
                        // 뭔지는 몰라도 잘못되었따.
                    //    return;
                    //}

                    setAddedItemAnim.insert(m_ItemAnim.size() - 1 - j);
                }

                if (m_mapOverlappedPair.find(m_ItemAnim[m_ItemAnim.size() - 1 - i].positionIndex) == m_mapOverlappedPair.end())
                {
                    std::vector<std::pair< std::set<int>, std::set<int> > > vec;
                    m_mapOverlappedPair[m_ItemAnim[m_ItemAnim.size() - 1 - i].positionIndex] = vec;
                }

                m_mapOverlappedPair[m_ItemAnim[m_ItemAnim.size() - 1 - i].positionIndex].push_back(std::make_pair(setAddedItemParticle, setAddedItemAnim));

                break;
            }
        }

        // 일반 반지와 수호의 반지 둘 다 낄 수 있으므로..
        if (itemData->dwSlotPosition == ESP_A_FINGER &&
            iVecSize < (int)(m_vecItemEff.size() + m_ItemAnim.size())) // 현재 사이즈와 비교해서 성공 여부를 가늠하자 ㅠ
            bProtectRing = false;

        std::vector< int > vecRet = g_pkPetAI->CreatePetinItem(GCUTIL_STR::GCStrCharToWide((char*)strItemNID.c_str()), 0, m_iPlayerIndex, false, iItemNID);
        if (!vecRet.empty())
            m_vecPetAiID.insert(m_vecPetAiID.end(), vecRet.begin(), vecRet.end());

        // [12/13/2007] breadceo. 그냥 착용만 하면 되는거라 이렇게 함
        if (itemData->dwGoodsID == ITEM_ID_GAIA_SHIELD || itemData->dwGoodsID == ITEM_ID_GAIA_SHIELD_ONCE)
        {
            m_bKamikiDefence = true;
        }
        if (itemData->dwGoodsID == ITEM_ID_PET_GHOST)
        {
            m_bPumpkinDefence = true;
        }
        if (itemData->dwGoodsID == ITEM_ID_KAZEAZE_SHIELD)
        {
            m_bKazeazeDefence = true;
        }

        // 히어로 아이템의 버프를 셋팅한다.
        g_pItemMgr->SettingHeroItemSpecialAbility(iItemNID, m_iPlayerIndex);
    }



    if (bProtectRing == true)
    {
        for (std::vector<KSlotData>::iterator it = m_kUserInfo.kQuickSlot.m_vecItemSlot.begin();
            it != m_kUserInfo.kQuickSlot.m_vecItemSlot.end(); it++)
        {
            if (g_MyD3D->IsProtectRing(((*it).m_kItem.m_dwID) / 10) == true)
            {
                char str[MAX_PATH] = "";
                std::string strItemNID;
                strItemNID = itoa(static_cast<int>(((*it).m_kItem.m_dwID) / 10), str, 10);

                g_ParticleManager->GetItemEffect(EGCItem, strItemNID, charID, 0, m_vecItemEff);
                g_AnimManager.GetNewItemAnim(strItemNID, charID, m_ItemAnim, 0);
            }
        }
    }

    // [9/13/2007] breadceo.옛날에 영향 안미치는게 내 컨셉
    // 스페셜 이펙트
    //switch( Extra_Char_Num )
    //{
    //case CID_LAS4:
    //    {
    if (m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_LAS) {
        std::string strBlade;
        std::string strBladeDeco;

        if (m_Elemental == ITEM_ELEMENTAL_FIRE)
            strBlade = "Las4GreatBlade_Fire";
        else if (m_Elemental == ITEM_ELEMENTAL_DARK)
            strBlade = "Las4GreatBlade_Dark";
        else if (m_Elemental == ITEM_ELEMENTAL_SHINING)
            strBlade = "Las4GreatBlade_Light";
        else if (m_Elemental == ITEM_ELEMENTAL_ICE)
            strBlade = "Las4GreatBlade_Ice";
        else if (m_Elemental == ITEM_ELEMENTAL_LOVE ||
            m_Elemental == ITEM_ELEMENTAL_ROSE)
            strBlade = "Las4GreatBlade_Love";
        else
            strBlade = "Las4GreatBlade";

        if (m_Elemental == ITEM_ELEMENTAL_LIGHTNING)
            strBladeDeco = "Las4GreatBlade_Lightning_deco";
        //else if( m_Elemental == ITEM_ELEMENTAL_DARK )
        //    Las4GreatBlade = "Las4GreatBlade_Dark";
        else
            strBladeDeco = "Las4GreatBlade_deco";

        g_AnimManager.GetNewItemAnim(strBlade.c_str(), CID_LAS4, Las4GreatSword, 0);
        g_AnimManager.GetNewItemAnim(strBladeDeco.c_str(), CID_LAS4, Las4GreatSword, 0);

        for (int i = 0; i < (int)Las4GreatSword.size(); ++i)
        {
            Las4GreatSword[i].animInst->SetShow(false);
        }
    }
    //    }
    //    break;
    //}

    // 젠장..
    g_MyD3D->m_akPet[m_iPlayerIndex]->m_iPlayerNumber = m_iPlayerIndex;
    // 암흑9단계는 펫(?)도 생성한다
    if (m_Elemental == ITEM_ELEMENTAL_DARK && m_StrongLevel == 9)
    {
        std::vector< int > vecRet = g_pkPetAI->CreatePetinItem(g_pkStrLoader->GetString(STR_ID_DARK_ENCHANT_LEVEL9).c_str(), 0, m_iPlayerIndex);
        if (!vecRet.empty())
            m_vecPetAiID.insert(m_vecPetAiID.end(), vecRet.begin(), vecRet.end());
    }

    char strTemp[30];
    sprintf(strTemp, "%d", g_MyD3D->m_kItemMgr.GetEquipSetItemName(this, true));
    g_ParticleManager->GetItemEffect(EGCItem, strTemp, charID, 0, m_vecItemEff);
    g_ParticleManager->GetItemEffect(EGCDamage, strTemp, charID, 0, m_vecDamageEff);
    g_ParticleManager->GetItemEffect(EGCSkill, strTemp, charID, 0, m_vecSkillEff);
    g_AnimManager.GetNewItemAnim(strTemp, charID, m_ItemAnim, 0);
    g_AnimManager.GetNewDamageAnim(strTemp, charID, m_DamageAnim, 0);
    g_AnimManager.GetNewSkillAnim(strTemp, charID, m_SkillAnim, 0);


    g_AnimManager.GetManager()->Sort();

    // 아이템 이펙트는 생성된 상태에서 시작합니다..
    for (int i = 0; i < (int)m_vecItemEff.size(); ++i)
    {
        ITEM_EFFECT& stEffect = m_vecItemEff[i];
        CreateItemEffectParam kParam(m_bShowEffect, bIsRight, stEffect, m_posDesc);
        CreateItemEffect(kParam);
    }

    g_pkPetAI->Show(m_vecPetAiID, true);

    ShowAllItemEffect(false);

    InitMonoVolt();
    if (Extra_Char_Num == CID_RONAN4)
        SetAbyssHandEffect(m_iAbyssHandEff); //로난3차(어비스나이트) 왼쪽 손 이펙트 켜기\

    // 진 버닝관련멤버들을 초기화한다.
    //SetNormalTpye();
    CreateDangerSque();
}

void PLAYER::EffectInit(TagCharacterInfo& tagInfo)
{
    //if( !m_kUserInfo.bLive )
    //    return;

    if (m_kUserInfo.cCharIndex == -1)
        return;

    InitAbyssHandEffect(&tagInfo); // 파티클 중복 생성될 우려를 감안해서 로난3차(어비스나이트) 왼손 이펙트 켜기 전 초기화 먼저 해주자

    //스페셜 이펙트
    switch (tagInfo.Extra_Char_Num)
    {
    case CID_LAS1:
    {
        tagInfo.m_pLasFireWingLeft = g_ParticleManager->CreateSequence("LasFireWingLeft", 0, 0, 0.5f);
        tagInfo.m_pLasFireWingRight = g_ParticleManager->CreateSequence("LasFireWingRight", 0, 0, 0.5f);
        tagInfo.m_pLasMotalBlowReady = g_ParticleManager->CreateSequence("LasMotalBlowReady", 0, 0, 0.5f);

        CKTDGAnim::CAnimInstance* tempAnim = g_MyD3D->m_pMapAnim->CreateInst(g_MyD3D->m_pMapAnim->GetSequence("MotalBlow"));
        g_MyD3D->m_pMapAnim->DeleteInst(tempAnim);
        tagInfo.m_pLasMotalBlowReadyAnim = g_MyD3D->m_pMapAnim->CreateInst(g_MyD3D->m_pMapAnim->GetSequence("MotalReady"));
        tagInfo.m_pLasMotalBlowReadyAnim->GetMatrix()->Scale(0.0013f, 0.0013f, 1.0f);
    }
    break;
    }
    tagInfo.m_pDashEff = g_ParticleManager->CreateSequence("DashEff", 0, 0, 0.0f);

    // 강화 아이템 착용 여부를 확인하고 이펙트를 생성한다.
    GCItem* itemData;
    CHAR_IDENTY charID;
    char cCharNum = tagInfo.Extra_Char_Num;

    bool bProtectRing = true;	// 수반 Effect 표시

    charID = (CHAR_IDENTY)cCharNum;

    SCharInfo* pCharInfo = &m_kUserInfo.GetCurrentChar(tagInfo.m_iCharType);
    if (pCharInfo == NULL)
        g_kGlobalValue.m_strNullFunc = __FUNCTION__;


    //  히어로 아이템 버프 리셋
    g_pItemMgr->InitHeroItemSpecialAbility(m_iPlayerIndex);

    ITEM_EFFECT stEffect;
    KItemIDVector& vecEquipItemInfo = pCharInfo->GetEquipmentForLoadModel(m_iPlayerIndex);
    for (int j = 0; j < (int)vecEquipItemInfo.size(); j++)
    {
        int iItemNID = vecEquipItemInfo[j];

        if (iItemNID == 5357)
            continue;

        itemData = g_MyD3D->m_kItemMgr.GetItemData(iItemNID);
        CONTINUE_NIL(itemData);

        if (itemData->dwSlotPosition & ESP_WEAPON) {   // 무기 코디의 경우 실 장착 무기 이펙트가 있는지 체크하기 위해 실 제 무기로 바꾸겠음 
            GCITEMID weaponItemID = pCharInfo->GetEquipWeaponItemID();
            if (iItemNID != weaponItemID) {
                iItemNID = weaponItemID;
                itemData = g_MyD3D->m_kItemMgr.GetItemData(iItemNID);
                CONTINUE_NIL(itemData);
            }
        }

        char str[15] = "";
        std::string strItemNID;
        strItemNID = itoa(iItemNID, str, 10);
        int enchantLevel;
        int iStrongLevel = itemData->iStrongLevel;
        EITEMATTRIBUTE eAttribute = itemData->eAttribute;

        //그랜드 체이스 목걸이 하드 코딩입니다.
        if (iItemNID == 14915)
        {
            iStrongLevel = 9;
            if (g_kGlobalValue.m_vecNecklace.size() > 0) {
                std::random_shuffle(g_kGlobalValue.m_vecNecklace.begin(), g_kGlobalValue.m_vecNecklace.end());
                std::vector<int>::iterator mit = g_kGlobalValue.m_vecNecklace.begin();
                eAttribute = static_cast<EITEMATTRIBUTE>(*mit);
            }
            else
            {
                eAttribute = EIATTR_FIRE;
            }
        }

        if (eAttribute != EIATTR_NONE)
        {
            //  발찌면
            if (itemData->dwSlotPosition & ESP_A_SHOES)
            {
                if (tagInfo.m_Elemental == ITEM_ELEMENTAL_NONE)
                    tagInfo.m_Elemental = g_MyD3D->m_kItemMgr.GetElementalType(eAttribute);
            }
            else
            {
                tagInfo.m_Elemental = g_MyD3D->m_kItemMgr.GetElementalType(eAttribute);
                m_iAbyssHandEff = eAttribute;
            }

            strItemNID = g_MyD3D->m_kItemMgr.EnumElementalTypetoStr(g_MyD3D->m_kItemMgr.GetElementalType(eAttribute));
        }

        //발찌는 자신의 강화레벨이 아니므로 - 목걸이만 강화레벨이다.
        if (eAttribute != EIATTR_NONE && tagInfo.m_StrongLevel < iStrongLevel)
        {
            //  발찌면
            if (!(itemData->dwSlotPosition & ESP_A_SHOES))
                tagInfo.m_StrongLevel = iStrongLevel;
        }

        if (iStrongLevel < 5)
        {
            enchantLevel = 0;
        }
        else if (iStrongLevel >= 5 && iStrongLevel < 8)
        {
            enchantLevel = 1;
        }
        else if (iStrongLevel == 8)
        {
            enchantLevel = 2;
        }
        else if (iStrongLevel == 9)
        {
            enchantLevel = 3;
        }

        int iVecSize = (int)(m_vecItemEff.size() + m_ItemAnim.size()); // 일단 여기서 이전 사이즈를 저장하고

        // 아이템 이펙트 정보를 얻어온다.
        int iAddedItemParticleNum = g_ParticleManager->GetItemEffect(EGCItem, strItemNID, charID, enchantLevel, tagInfo.m_vecItemEff);
        g_ParticleManager->GetItemEffect(EGCDamage, strItemNID, charID, enchantLevel, tagInfo.m_vecDamageEff);
        g_ParticleManager->GetItemEffect(EGCSkill, strItemNID, charID, enchantLevel, tagInfo.m_vecSkillEff);

        int iAddedItemAnimNum = g_AnimManager.GetNewItemAnim(strItemNID, charID, tagInfo.m_ItemAnim, enchantLevel);
        g_AnimManager.GetNewDamageAnim(strItemNID, charID, tagInfo.m_DamageAnim, enchantLevel);
        g_AnimManager.GetNewSkillAnim(strItemNID, charID, tagInfo.m_SkillAnim, enchantLevel);

        //새로 추가된 anim 중에 겹쳐서는 안되는 아이가 잇는지 찾는다.
        for (int i = 0; i < iAddedItemAnimNum; i++)
        {
            //겹쳐서는 안되는 아이가 잇따!!!
            if (tagInfo.m_ItemAnim[tagInfo.m_ItemAnim.size() - 1 - i].positionIndex)
            {
                std::set<int> setAddedItemParticle;
                std::set<int> setAddedItemAnim;

                for (int j = 0; j < iAddedItemAnimNum; j++)
                {
                    setAddedItemAnim.insert(tagInfo.m_ItemAnim.size() - 1 - j);
                }

                for (int j = 0; j < iAddedItemParticleNum; j++)
                {
                    setAddedItemParticle.insert(tagInfo.m_vecItemEff.size() - 1 - j);
                }

                if (tagInfo.m_mapOverlappedPair.find(tagInfo.m_ItemAnim[tagInfo.m_ItemAnim.size() - 1 - i].positionIndex) == tagInfo.m_mapOverlappedPair.end())
                {
                    std::vector<std::pair< std::set<int>, std::set<int> > > vec;
                    tagInfo.m_mapOverlappedPair[tagInfo.m_ItemAnim[tagInfo.m_ItemAnim.size() - 1 - i].positionIndex] = vec;
                }

                tagInfo.m_mapOverlappedPair[tagInfo.m_ItemAnim[tagInfo.m_ItemAnim.size() - 1 - i].positionIndex].push_back(std::make_pair(setAddedItemParticle, setAddedItemAnim));

                break;
            }
        }

        if (itemData->dwSlotPosition == ESP_A_FINGER &&
            iVecSize < (int)(m_vecItemEff.size() + m_ItemAnim.size())) // 현재 사이즈와 비교해서 성공 여부를 가늠하자 ㅠ
            bProtectRing = false;

        std::vector< int > vecRet = g_pkPetAI->CreatePetinItem(GCUTIL_STR::GCStrCharToWide((char*)strItemNID.c_str()), 0, m_iPlayerIndex, false, iItemNID);
        if (!vecRet.empty())
            tagInfo.m_vecPetAiID.insert(tagInfo.m_vecPetAiID.end(), vecRet.begin(), vecRet.end());

        // 히어로 아이템의 버프를 셋팅한다.
        g_pItemMgr->SettingHeroItemSpecialAbility(iItemNID, m_iPlayerIndex);
    }

    if (bProtectRing == true)
    {
        for (std::vector<KSlotData>::iterator it = m_kUserInfo.kQuickSlot.m_vecItemSlot.begin();
            it != m_kUserInfo.kQuickSlot.m_vecItemSlot.end(); it++)
        {
            if (g_MyD3D->IsProtectRing(((*it).m_kItem.m_dwID) / 10) == true)
            {
                char str[15] = "";
                std::string strItemNID;
                strItemNID = itoa(static_cast<int>(((*it).m_kItem.m_dwID) / 10), str, 10);

                g_ParticleManager->GetItemEffect(EGCItem, strItemNID, charID, 0, tagInfo.m_vecItemEff);
                g_AnimManager.GetNewItemAnim(strItemNID, charID, tagInfo.m_ItemAnim, 0);
            }
        }
    }

    // [9/13/2007] breadceo.옛날에 영향 안미치는게 내 컨셉
    // 스페셜 이펙트
    switch (tagInfo.Extra_Char_Num)
    {
    case CID_LAS4:
    {
        std::string strBlade;
        std::string strBladeDeco;

        if (tagInfo.m_Elemental == ITEM_ELEMENTAL_FIRE)
            strBlade = "Las4GreatBlade_Fire";
        else if (tagInfo.m_Elemental == ITEM_ELEMENTAL_DARK)
            strBlade = "Las4GreatBlade_Dark";
        else if (tagInfo.m_Elemental == ITEM_ELEMENTAL_SHINING)
            strBlade = "Las4GreatBlade_Light";
        else if (tagInfo.m_Elemental == ITEM_ELEMENTAL_ICE)
            strBlade = "Las4GreatBlade_Ice";
        else if (tagInfo.m_Elemental == ITEM_ELEMENTAL_LOVE ||
            tagInfo.m_Elemental == ITEM_ELEMENTAL_ROSE)
            strBlade = "Las4GreatBlade_Love";
        else
            strBlade = "Las4GreatBlade";

        if (tagInfo.m_Elemental == ITEM_ELEMENTAL_LIGHTNING)
            strBladeDeco = "Las4GreatBlade_Lightning_deco";
        //else if( m_Elemental == ITEM_ELEMENTAL_DARK )
        //    Las4GreatBlade = "Las4GreatBlade_Dark";
        else
            strBladeDeco = "Las4GreatBlade_deco";

        g_AnimManager.GetNewItemAnim(strBlade.c_str(), CID_LAS4, tagInfo.Las4GreatSword, 0);
        g_AnimManager.GetNewItemAnim(strBladeDeco.c_str(), CID_LAS4, tagInfo.Las4GreatSword, 0);

        for (int i = 0; i < (int)tagInfo.Las4GreatSword.size(); ++i)
        {
            tagInfo.Las4GreatSword[i].animInst->SetShow(false);
        }
    }
    break;
    case CID_RONAN4:
        SetAbyssHandEffect(m_iAbyssHandEff, &tagInfo); //로난3차(어비스나이트) 왼쪽 손 이펙트 켜기
        break;
    }

    // 암흑9단계는 펫(?)도 생성한다
    if (tagInfo.m_Elemental == ITEM_ELEMENTAL_DARK && tagInfo.m_StrongLevel == 9)
    {
        std::vector< int > vecRet = g_pkPetAI->CreatePetinItem(g_pkStrLoader->GetString(STR_ID_DARK_ENCHANT_LEVEL9).c_str(), 0, m_iPlayerIndex);
        if (!vecRet.empty())
            tagInfo.m_vecPetAiID.insert(tagInfo.m_vecPetAiID.end(), vecRet.begin(), vecRet.end());
    }

    char strTemp[30];
    sprintf(strTemp, "%d", g_MyD3D->m_kItemMgr.GetEquipSetItemName(this, &tagInfo));
    g_ParticleManager->GetItemEffect(EGCItem, strTemp, charID, 0, tagInfo.m_vecItemEff);
    g_ParticleManager->GetItemEffect(EGCDamage, strTemp, charID, 0, tagInfo.m_vecDamageEff);
    g_ParticleManager->GetItemEffect(EGCSkill, strTemp, charID, 0, tagInfo.m_vecSkillEff);

    g_AnimManager.GetNewItemAnim(strTemp, charID, tagInfo.m_ItemAnim, 0);
    g_AnimManager.GetNewDamageAnim(strTemp, charID, tagInfo.m_DamageAnim, 0);
    g_AnimManager.GetNewSkillAnim(strTemp, charID, tagInfo.m_SkillAnim, 0);

    g_AnimManager.GetManager()->Sort();

    // 아이템 이펙트는 생성된 상태에서 시작합니다..
    for (int i = 0; i < (int)tagInfo.m_vecItemEff.size(); ++i)
    {
        ITEM_EFFECT& stEffect = tagInfo.m_vecItemEff[i];
        CreateItemEffectParam kParam(m_bShowEffect, bIsRight, stEffect, m_posDesc);
        CreateItemEffect(kParam);
    }

    InitMonoVolt();

    CreateDangerSque();
}

void PLAYER::EffectFrameMove()
{
    PROFILE_SET("PLAYER::EffectFrameMove");

    BaseEffectFrameMove();

    //이펙트 처리
    switch (uiMotion)
    {
    case MID_LAS_SPECIAL3:
        if (m_pLasFireWingLeft != NULL && m_pLasFireWingRight != NULL
            && g_ParticleManager->IsLiveInstance(m_pLasFireWingLeft) == true
            && g_ParticleManager->IsLiveInstance(m_pLasFireWingRight) == true)
        {
            m_pLasFireWingLeft->SetEmitRate(CMinMax<float>(100, 100));
            m_pLasFireWingRight->SetEmitRate(CMinMax<float>(100, 100));
            if (bIsRight == true)
            {
                m_pLasFireWingLeft->SetPosition(m_posDesc.body.x + 0.06f, m_posDesc.body.y, m_posDesc.body.z);
                m_pLasFireWingRight->SetPosition(m_posDesc.body.x + 0.06f, m_posDesc.body.y, m_posDesc.body.z);
            }
            else
            {
                m_pLasFireWingLeft->SetPosition(m_posDesc.body.x - 0.05f, m_posDesc.body.y, m_posDesc.body.z);
                m_pLasFireWingRight->SetPosition(m_posDesc.body.x - 0.05f, m_posDesc.body.y, m_posDesc.body.z);
            }
        }
        break;
    case MID_LAS2_DASH_2:
    case MID_LAS2_JUMPDASH:
    {
        if (m_pDashEff != NULL && g_ParticleManager->IsLiveInstance(m_pDashEff) == true)
        {
            m_pDashEff->SetEmitRate(CMinMax<float>(10, 15));
            if (bIsRight == true)
            {
                m_pDashEff->SetPosition(m_posDesc.body.x - 0.06f, m_posDesc.body.y - 0.12f, m_posDesc.body.z);
            }
            else
            {
                m_pDashEff->SetPosition(m_posDesc.body.x + 0.06f, m_posDesc.body.y - 0.12f, m_posDesc.body.z);
            }
        }
        break;
    }
    case MID_LAS_SPECIAL2:
    case MID_LAS_SPECIAL2_FAIL:
    case MID_LAS_BLOOD_BASH:
        if (m_pLasMotalBlowReady != NULL
            && g_ParticleManager->IsLiveInstance(m_pLasMotalBlowReady) == true
            && g_MyD3D->m_pMapAnim->IsAliveInst(m_pLasMotalBlowReadyAnim) == true)
        {
            if (cFrame < 18)
            {
                m_pLasMotalBlowReadyAnim->GetMatrix()->Move(m_posDesc.body.x, m_posDesc.body.y, m_posDesc.body.z);
                m_pLasMotalBlowReadyAnim->Start(CKTDGAnim::PT_LOOP);
            }
            else
            {
                m_pLasMotalBlowReadyAnim->Stop();
            }
        }
        break;
    case MID_LIRE3_SPECIAL2:
        if (cFrame == 10)
            m_dwLire3Sp2FrameSum = 0;
        else if (cFrame == 31)
        {
            if (m_dwLire3Sp2FrameSum > LIRE3SPECIAL2DURING)
                cFrame = 32;
            else
                cFrame = 20;
        }
        else
            m_dwLire3Sp2FrameSum++;
        break;
    case MID_COMMON_HOLD: //홀드 모션 이펙트
    {
        KSafeArray<std::string, 4> strParticleName;
        strParticleName[0] = "Arme01_Hold_Beam_effect_01";
        strParticleName[1] = "Arme01_Hold_Beam_effect_02";
        strParticleName[2] = "Arme01_Hold_Beam_effect_03";
        strParticleName[3] = "Arme01_Hold_Beam_effect_04";

        for (int i = 0; i < 4; i++)
        {
            if (m_pHoldEffect[i] == NULL || g_ParticleManager->IsLiveInstance(m_pHoldEffect[i]) == false)
            {
                m_pHoldEffect[i] = g_ParticleManager->CreateSequence(strParticleName[i], 0, 0, 0.5f);
                TRACE_INFO stTrace;
                if (m_pHoldEffect[i]) {
                    stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
                    stTrace.m_fParticleTraceTime = 5.0f;
                    m_pHoldEffect[i]->SetTrace(&stTrace);

                }

            }
        }
    }
    break;
    default:
        if (m_pDashEff != NULL && g_ParticleManager->IsLiveInstance(m_pDashEff) == true)
            m_pDashEff->SetEmitRate(CMinMax<float>(0, 0));
        if (m_pLasFireWingLeft != NULL && g_ParticleManager->IsLiveInstance(m_pLasFireWingLeft) == true)
            m_pLasFireWingLeft->SetEmitRate(CMinMax<float>(0, 0));
        if (m_pLasFireWingRight != NULL && g_ParticleManager->IsLiveInstance(m_pLasFireWingRight) == true)
            m_pLasFireWingRight->SetEmitRate(CMinMax<float>(0, 0));
        if (m_pLasMotalBlowReady != NULL && g_ParticleManager->IsLiveInstance(m_pLasMotalBlowReady) == true)
            m_pLasMotalBlowReady->SetEmitRate(CMinMax<float>(0, 0));
        if (m_pLasMotalBlowReadyAnim != NULL && g_MyD3D->m_pMapAnim->IsAliveInst(m_pLasMotalBlowReadyAnim) == true)
            m_pLasMotalBlowReadyAnim->Stop();
        for (int i = 0; i < 4; i++)
        {
            if (m_pHoldEffect[i] != NULL && g_ParticleManager->IsLiveInstance(m_pHoldEffect[i]) == true)
            {
                g_ParticleManager->DeleteSequence(m_pHoldEffect[i]);
                m_pHoldEffect[i] = NULL;
            }
        }

        break;
    }
}

void PLAYER::BaseEffectFrameMove()
{
    PROFILE_SET("PLAYER::BaseEffectFrameMove");

    int iCondition = GetCurFormTemplate().BaseEffect.iCondition;
    std::vector< CharBaseParticle >::iterator vecParticle = GetCurFormTemplate().BaseEffect.vecParticleInfo.begin();
    for (; vecParticle != GetCurFormTemplate().BaseEffect.vecParticleInfo.end(); ++vecParticle)
    {
        if ((NULL == vecParticle->pParticle || false == g_ParticleManager->IsLiveInstance(vecParticle->pParticle)))
        {
            bool bCreate = false;
            switch (iCondition)
            {
            case BECT_CONDITION_NONE:
            {
                bCreate = true;
            }
            break;
            case BECT_CONDITION_LEVEL:
            {
                SCharInfo& sCharInfo = m_kUserInfo.GetCurrentChar();
                if ((-1 != sCharInfo.iCharType) &&
                    (vecParticle->iMinLevel <= sCharInfo.iLevel && sCharInfo.iLevel <= vecParticle->iMaxLevel))
                {
                    bCreate = true;
                }
            }
            break;
            }
            if (bCreate)
                CreateBaseEffect(*vecParticle);
        }
    }
}

void PLAYER::CreateBaseEffect(CharBaseParticle& BaseParticle)
{
    if (NULL == BaseParticle.pParticle)
    {
        BaseParticle.pParticle = g_ParticleManager->CreateSequence(BaseParticle.strSequenceName.c_str(), BaseParticle.fOffsetX, BaseParticle.fOffsetY);

        if (BaseParticle.pParticle)
        {
            TRACE_INFO stTrace;
            stTrace.m_pvPos = ComputePosDesc(m_posDesc, (EMIT_POSITION_TYPE)BaseParticle.iPositionType);
            stTrace.m_fXOffset = BaseParticle.fOffsetX;
            stTrace.m_fYOffset = BaseParticle.fOffsetY;
            stTrace.m_fParticleTraceTime = BaseParticle.fTraceTime;

            BaseParticle.pParticle->SetTrace(&stTrace);
        }
    }
}

void PLAYER::EffectClear()
{
    EffectClear(true);
}

void PLAYER::EffectClear(bool bClearBuf)
{
    m_bShowEffect = false;

    // 파티클 삭제
    for (int i = 0; i < (int)m_vecItemEff.size(); ++i)
        g_ParticleManager->DeleteSequence(m_vecItemEff[i].pSeq);
    for (int i = 0; i < (int)m_vecDamageEff.size(); ++i)
        g_ParticleManager->DeleteSequence(m_vecDamageEff[i].pSeq);
    for (int i = 0; i < (int)m_vecSkillEff.size(); ++i)
        g_ParticleManager->DeleteSequence(m_vecSkillEff[i].pSeq);

    m_vecItemEff.clear();
    m_vecDamageEff.clear();
    m_vecSkillEff.clear();

    g_ParticleManager->DeleteSequence(m_pRonan3Force);
    g_ParticleManager->DeleteSequence(m_pRonan3Force1);
    g_ParticleManager->DeleteSequence(m_pRonan3Force2);
    g_ParticleManager->DeleteSequence(m_pRuneShield);
    g_ParticleManager->DeleteSequence(m_pRuneShield1);
    g_ParticleManager->DeleteSequence(m_pLasFireWingLeft);
    g_ParticleManager->DeleteSequence(m_pLasFireWingRight);
    g_ParticleManager->DeleteSequence(m_pLasMotalBlowReady);
    g_ParticleManager->DeleteSequence(m_pDashEff);

    // 진 버닝이펙트
    g_ParticleManager->DeleteSequence(m_pBurnningCharge[0]);
    g_ParticleManager->DeleteSequence(m_pBurnningCharge[1]);
    g_ParticleManager->DeleteSequence(m_pHideParticle);
    //g_ParticleManager->DeleteSequence( m_pLire3Wing );
    g_ParticleManager->DeleteSequence(m_pEclipseBack);

    // 애니메이션 삭제
    for (int i = 0; i < (int)m_ItemAnim.size(); ++i)
        g_AnimManager.DeleteInst(m_ItemAnim[i].animInst);

    for (int i = 0; i < (int)m_SkillAnim.size(); ++i)
        g_AnimManager.DeleteSkillInst(m_SkillAnim[i]);

    if (m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_LAS)
    {
        for (int i = 0; i < (int)Las4GreatSword.size(); ++i)
            g_AnimManager.DeleteInst(Las4GreatSword[i].animInst);

        Las4GreatSword.clear();
    }

    m_ItemAnim.clear();
    m_DamageAnim.clear();
    m_SkillAnim.clear();

    g_MyD3D->m_pMapAnim->DeleteInst(m_pLasMotalBlowReadyAnim);

    // 펫 삭제
    g_pkPetAI->ClearAIObject(m_iPlayerIndex);

    m_pRonan3Force = NULL;
    m_pRonan3Force1 = NULL;
    m_pRonan3Force2 = NULL;
    m_pRuneShield = NULL;
    m_pRuneShield1 = NULL;
    m_pLasFireWingLeft = NULL;
    m_pLasFireWingRight = NULL;
    m_pLasMotalBlowReady = NULL;
    m_pLasMotalBlowReadyAnim = NULL;
    // 진 버닝이펙트
    m_pBurnningCharge[0] = NULL;
    m_pBurnningCharge[1] = NULL;
    m_pHideParticle = NULL;
    //m_pLire3Wing = NULL;
    m_pDashEff = NULL;

    m_pEclipseBack = NULL;

    m_Elemental = 0;
    m_StrongLevel = 0;

    g_ParticleManager->DeleteSequence(m_pFireLeftTip);
    g_ParticleManager->DeleteSequence(m_pFireRightTip);

    m_pFireLeftTip = NULL;
    m_pFireRightTip = NULL;

    m_pEmoticon = NULL;

    m_mapOverlappedPair.clear();

    std::map< std::string, std::vector<KGCAnimManager::ItemAnim> >::iterator mit = m_mapPlayAnimation.begin();
    while (mit != m_mapPlayAnimation.end())
    {
        std::vector<KGCAnimManager::ItemAnim>& vecAnim = mit->second;
        for (int i = 0; i < (int)vecAnim.size(); ++i)
        {
            g_AnimManager.DeleteInst(vecAnim[i].animInst);
        }
        ++mit;
    }
    m_mapPlayAnimation.clear();

    if (SiKGCRoomManager()->IsMonsterGameMode() && bClearBuf)
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DARKBALL);

    InitAbyssHandEffect();
    InitMonoVolt();
    m_ParticleMsg.clear(); //대기중인 파티클 초기화 안되는 문제 수정.
    m_vecChildMeshParticle.clear();

    // BaseEffect제거
    BaseEffectClear();
    InitDangerSque();
}
void PLAYER::BaseEffectClear()
{
    std::vector< CharBaseParticle >::iterator vecParticle = GetCurFormTemplate().BaseEffect.vecParticleInfo.begin();
    for (; vecParticle != GetCurFormTemplate().BaseEffect.vecParticleInfo.end(); ++vecParticle)
    {
        if (vecParticle->pParticle)
        {
            g_ParticleManager->DeleteSequence(vecParticle->pParticle);
            vecParticle->pParticle = NULL;
        }
    }
}

D3DXMATRIX* PLAYER::ComputePositionBone(D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& vecPositionBone)
{
    return IGCComputePaletteP3M::ComputePositionBone(matOut, Bone_Num, matBasic, vecPositionBone);
}

D3DXMATRIX* PLAYER::ComputeAngleBone(D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic_)
{

    if (Bone_Num == 7)
    {
        D3DXMATRIX matBasic;
        D3DXMATRIX Next;
        D3DXMATRIX rotZ;

        int iFrame = 0;
        int iMotion = 0;

        if (IsLocalPlayer())
        {
            iFrame = g_kLocalPlayerLatency->kRenderData[0].cFrame;
            iMotion = g_kLocalPlayerLatency->kRenderData[0].usMotion;
        }
        else
        {
            iFrame = cFrame;
            iMotion = uiMotion;
        }

        if (((iMotion == MID_LIRE3_CRITICALATK ||
            iMotion == MID_LIRE3_DOUBLEATK ||
            iMotion == MID_LIRE_DASH) && iFrame > 3) ||
            iMotion == MID_LIRE_DASHATK ||
            iMotion == MID_LIRE4_CRITICAL_ATK ||
            iMotion == MID_LIRE4_DOUBLE_ATK1 ||
            iMotion == MID_SKILL_LIRE_DOUBLE_DASH_SHOT)
        {
            D3DXMatrixRotationZ(&rotZ, -Body_Angle);
        }
        else
            D3DXMatrixRotationZ(&rotZ, Body_Angle);

        D3DXMatrixMultiply(&matBasic, &rotZ, &matBasic_);
        D3DXMatrixMultiply(matOut, &pFrame->A_Bone[Bone_Num], &matBasic);
        return matOut;
    }
    else if (Bone_Num == 12) // 오른쪽 어깨
    {
        // 리르 3차 (노바) 일반 콤보 1~3 모션에서..
        // 한손으로 활쏘는데 반대편 팔을 BodyAngle에 관계없이 고정시켜주기 위해서..

        D3DXMATRIX matBasic;
        D3DXMATRIX Next;
        D3DXMATRIX rotZ;

        int iFrame = 0;
        int iMotion = 0;

        if (IsLocalPlayer())
        {
            iFrame = g_kLocalPlayerLatency->kRenderData[0].cFrame;
            iMotion = g_kLocalPlayerLatency->kRenderData[0].usMotion;
        }
        else
        {
            iFrame = cFrame;
            iMotion = uiMotion;
        }

        if (Body_Angle > 0.0f &&
            (iMotion == MID_LIRE4_ATK1_1 ||
                iMotion == MID_LIRE4_ATK1_2 ||
                iMotion == MID_LIRE4_ATK1_3))
        {
            D3DXMatrixRotationZ(&rotZ, -Body_Angle);
            D3DXMatrixMultiply(&matBasic, &rotZ, &matBasic_);
            D3DXMatrixMultiply(matOut, &pFrame->A_Bone[Bone_Num], &matBasic);
            return matOut;
        }
        else
            return IGCComputePaletteP3M::ComputeAngleBone(matOut, pFrame, Bone_Num, matBasic_);
    }
    else
        return IGCComputePaletteP3M::ComputeAngleBone(matOut, pFrame, Bone_Num, matBasic_);

}

void PLAYER::ReleasePlayerResource()
{
    EndAfterImage();

    for (int i = 0; i < FORM_NUM; ++i) {
        if (m_pObjMetaForm[i])
        {
            SAFE_REMOVE_DELETE(m_pObjMetaForm[i]);
        }
    }
    m_pObject = NULL;
    m_pLire3Arrow = NULL;
    m_pNonChangeFaceLift = NULL;
    m_mapFaceLiftMeshObject.clear();

    // 사실 지금은 필요없는 거지만.. 스킬 리소스도 게임이 끝때 다 지워져버리기 때문에 나중에 관리할때 사용하시던지 지우시던지..
    SiGCSKT()->ReleaseResource(m_kUserInfo.dwUID);
    SiGCSKT()->RemoveUserSkillSet(m_kUserInfo.dwUID);
    m_pObjectEgo.clear();
}

void PLAYER::UpdatePlayerResource(TagCharacterInfo* pTagInfo)
{
    // TODO : Asirion Tag매치 정리 필요.
    AutoSetExtra(*pTagInfo);
    SCharInfo& CharInfo = m_kUserInfo.GetCurrentChar(pTagInfo->m_iCharType);

    if (pTagInfo->m_pObjMetaForm[FORM_NORMAL] == NULL)
    {
        pTagInfo->m_pObjMetaForm[FORM_NORMAL] = new GCObject();
        pTagInfo->m_pObjMetaForm[FORM_NORMAL]->SetMeshComputePaletteInterface((IGCComputePaletteP3M*)this);
    }
    else
    {
        pTagInfo->GetMetaForm(FORM_NORMAL)->RemoveMesh(NULL);
        pTagInfo->m_pLire3Arrow = NULL;
    }

    GCObject* pObj = pTagInfo->GetMetaForm(FORM_NORMAL);
    CreateItemModel(pObj, CharInfo, pTagInfo->Extra_Char_Num, false);

    CreatePlayerName(pObj);

    if (pTagInfo->Extra_Char_Num == CID_LIRE3)
    {
        pTagInfo->m_pLire3Arrow = pObj->AddMesh(g_pGCDeviceManager2->CreateMeshP3M("Lire3Arrow.p3m"), g_pGCDeviceManager2->CreateTexture("Lire3Arrow.dds"));
        pTagInfo->m_pLire3Arrow->m_bRender = false;
    }
}

void PLAYER::CreateItemModel(GCObject* pObject_, const SCharInfo& sCharInfo_, char cExtraCharNum_, bool bTempPlayer_)
{
    int iCharType = sCharInfo_.iCharType;
    int iPromotion = (cExtraCharNum_ - sCharInfo_.iCharType) / GC_CHAR_NUM;
    m_bEquipBigHead = false;
    m_bLoadDownHelmetNFL = false;
    m_bEquipFaceOffHead = false;

    PLAYER_TEMPLATE kPlayerTemplate;
    GetPlayerTemplate(cExtraCharNum_, kPlayerTemplate);

    KItemIDVector vecItemSorted = sCharInfo_.GetEquipmentForLoadModel(m_iPlayerIndex);
    KItemIDVector::iterator vecIter = vecItemSorted.begin();

    for (; vecIter != vecItemSorted.end(); ++vecIter)
    {
        GCItem* pItem = g_pItemMgr->GetItemData(*vecIter);
        CONTINUE_NIL(pItem);

        // 표정 시스템을 적용해야 할 아이템이 있는지
        if (SiGCFaceOffManager()->IsFaceOffItem(*vecIter)) {
            m_bEquipFaceOffHead = true;
            break;
        }
    }

    for (vecIter = vecItemSorted.begin(); vecIter != vecItemSorted.end(); ++vecIter)
    {
        GCItem* pItem = g_pItemMgr->GetItemData(*vecIter);
        CONTINUE_NIL(pItem);

        if (m_bEquipFaceOffHead == true && SiGCFaceOffManager()->IsEmotionItem(*vecIter)) {
            continue;
        }
        else
        {
            if (iCharType == GC_CHAR_ZERO && g_kGlobalValue.GetCurrEquipedZeroMaskID(&this->GetCurrentChar()) > 0) //Zero Mask
            {
                if (((pItem->dwSlotPosition & ESP_A_DOWN_HELMET) == ESP_A_DOWN_HELMET))
                    continue;
            }
            if (((pItem->dwSlotPosition & ESP_WEAPON) == ESP_WEAPON) && g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
            {
                RoomAnimationInfo RoomAnimationModel = g_pRoomAnimationMgr->GetRoomAnimation(m_iPlayerIndex);

                if (RoomAnimationModel.m_iModel < 1)
                {
                    if (RoomAnimationModel.m_iMotion < 0)
                        LoadItemModel(pObject_, kPlayerTemplate, pItem, iCharType, iPromotion, bTempPlayer_);
                }
                else
                {
                    char strTemp[MAX_PATH] = { 0, };
                    sprintf_s(strTemp, MAX_PATH, "Room_%d_%d", RoomAnimationModel.m_iModel, this->GetCurrentChar().iCharType);
                    LoadRoomAnimationModel(pObject_, kPlayerTemplate, strTemp, bTempPlayer_, true);
                }
                continue;
            }
            LoadItemModel(pObject_, kPlayerTemplate, pItem, iCharType, iPromotion, bTempPlayer_);
        }
    }

    LoadAddtionModel(pObject_, kPlayerTemplate, bTempPlayer_);
    LoadETCModel(pObject_, cExtraCharNum_, bTempPlayer_);
}

void PLAYER::CreateItemModelClean(GCObject* pObject_, const SCharInfo& sCharInfo_, char cExtraCharNum_, bool bTempPlayer_)
{
    int iCharType = sCharInfo_.iCharType;
    int iPromotion = (cExtraCharNum_ - sCharInfo_.iCharType) / GC_CHAR_NUM;
    m_bEquipBigHead = false;
    m_bLoadDownHelmetNFL = false;
    m_bEquipFaceOffHead = false;

    PLAYER_TEMPLATE kPlayerTemplate;
    GetPlayerTemplate(cExtraCharNum_, kPlayerTemplate);

    KItemIDVector vecItemSorted = sCharInfo_.GetEquipmentForLoadModel(m_iPlayerIndex);
    KItemIDVector::iterator vecIter = vecItemSorted.begin();

    for (; vecIter != vecItemSorted.end(); ++vecIter)
    {
        GCItem* pItem = g_pItemMgr->GetItemData(*vecIter);
        CONTINUE_NIL(pItem);

        // 표정 시스템을 적용해야 할 아이템이 있는지
        if (SiGCFaceOffManager()->IsFaceOffItem(*vecIter)) {
            m_bEquipFaceOffHead = true;
            break;
        }
    }

    for (vecIter = vecItemSorted.begin(); vecIter != vecItemSorted.end(); ++vecIter)
    {
        GCItem* pItem = g_pItemMgr->GetItemData(*vecIter);
        CONTINUE_NIL(pItem);

        // 표정아이템(출석포인트로 사는)은 개편된 투구에서는 그리지 말자
        if (m_bEquipFaceOffHead == true && SiGCFaceOffManager()->IsEmotionItem(*vecIter)) {
            continue;
        }
        else {
            if (iCharType == GC_CHAR_ZERO && g_kGlobalValue.GetCurrEquipedZeroMaskID(&this->GetCurrentChar()) > 0) //Zero Mask
            {
                if (((pItem->dwSlotPosition & ESP_A_DOWN_HELMET) == ESP_A_DOWN_HELMET))
                    continue;
            }
            LoadItemModel(pObject_, kPlayerTemplate, pItem, iCharType, iPromotion, bTempPlayer_);
        }
    }

    // 추가 모델링 로드
    LoadAddtionModel(pObject_, kPlayerTemplate, bTempPlayer_);

    // 기타 모델링 로드
    LoadETCModel(pObject_, cExtraCharNum_, bTempPlayer_);
}

void PLAYER::LoadItemModel(GCObject* pObject_, PLAYER_TEMPLATE& kPlayerTemplate_, GCItem* pItem_, int iCharType_, int iPromotion_, bool bTempPlayer_)
{
    // 빅헤드를 끼고있으면 빅헤드용 앞면 처리를 해줘야한다.
    DWORD dwBigHeadPos = (ESP_A_DOWN_HELMET + ESP_A_UPPER_HELMET);
    if (dwBigHeadPos == (pItem_->dwSlotPosition & dwBigHeadPos))
        m_bEquipBigHead = true;

    // 게임 중인지 판단
    bool bGamePlaying = !(false == m_bResultTower && (GC_RUS_PLAYING != m_kUserInfo.nUserState || bTempPlayer_));

    // 조건별 옵션을 적용할 것인가 결정
    ITEM_MESH_OPTION kOption;
    std::vector< ITEM_MESH_OPTION >::iterator vecMeshOptionIter = kPlayerTemplate_.vecItemMeshOption.begin();
    for (; vecMeshOptionIter != kPlayerTemplate_.vecItemMeshOption.end(); ++vecMeshOptionIter)
    {
        // 장착 포지션 확인
        if (pItem_->dwSlotPosition != vecMeshOptionIter->dwConSlotPosition)
            continue;

        // 게임 플레이 중이 아닐때만 적용하는 옵션
        if (0 == vecMeshOptionIter->iConPlaying && bGamePlaying)
            continue;

        // 게임 플레이 중일때만 적용하는 옵션
        if (1 == vecMeshOptionIter->iConPlaying && (false == bGamePlaying))
            continue;

        // 광장에서만 적용하는 옵션
        if (1 == vecMeshOptionIter->iConSquare)
            continue;

        // 옵션적용
        kOption = (*vecMeshOptionIter);
        break;
    }

    if (kOption.bIsNotLoad)
        return;

    if (kOption.strChildName.empty())
        kOption.strChildName = GetDefaultMeshName(pItem_->dwSlotPosition);

    // 라이브 모델 로드
    std::vector<GCObject*> vecLiveModel = g_pItemMgr->CreateLiveModel(pItem_->dwGoodsID, iCharType_, iPromotion_, pObject_, kOption.vPos, kOption.bIsOnlyTraceParentPos, const_cast<char*>(kOption.strLiveMotionName.c_str()));

    // 라이브 모델이 없으면 일반모델 로드
    if (vecLiveModel.empty())
    {
        bool bBuild = !bTempPlayer_;
        if (pItem_->dwSlotPosition & ESP_WEAPON)
            bBuild = false;

        GCMeshObject* pMeshObject = g_pItemMgr->CreateAbtaModel(pItem_->dwGoodsID, iCharType_, iPromotion_, pObject_, bBuild, static_cast<char>(kOption.iBoneID));
        if (pMeshObject)
        {
            pMeshObject->m_bRender = kOption.bIsRender;
            pMeshObject->m_strName = kOption.strChildName;
            if (kOption.iTechnique != -1)
            {
                pMeshObject->m_iTechnique = kOption.iTechnique;
            }
        }

        LoadFaceTexture(pObject_, iCharType_, iPromotion_, pItem_->dwGoodsID, pItem_->dwSlotPosition);

        // 이 부분은 정리 안할꺼임. 파일명에 _m 붙은 아이템이 뭐가있는지 누가쓰는지 확인도 안해봄.
        if (pItem_->dwSlotPosition & ESP_WEAPON)
        {
            GCMeshObject* pMergeMeshObj = g_pItemMgr->CreateLas4MergeAbtaModel(pItem_->dwGoodsID, iCharType_, GetMetaForm(FORM_NORMAL));
            if (pMergeMeshObj != NULL)
            {
                if (kOption.bIsRender == false) {
                    pMergeMeshObj->m_bRender = true;
                }
                else {
                    pMergeMeshObj->m_bRender = false;
                }
                pMergeMeshObj->m_strName = "MERGE_WEAPON";
            }
        }
    }
    else
    {
        // 라이브모델 옵션적용
        std::vector<GCObject*>::iterator vecLiveIter = vecLiveModel.begin();
        for (int iLoopCnt = 0; vecLiveIter != vecLiveModel.end(); ++vecLiveIter, ++iLoopCnt)
        {
            char szChildMeshName[1024];
            strcpy(szChildMeshName, kOption.strChildName.c_str());
            if (kOption.bIsAutoChildNameAsLiveLoopCnt)
                sprintf(szChildMeshName, "%s%d", kOption.strChildName.c_str(), iLoopCnt);

            if (0 < static_cast<int>(strlen(szChildMeshName)))
                (*vecLiveIter)->m_strName = szChildMeshName;

            (*vecLiveIter)->SetPosition(kOption.vPos);
            (*vecLiveIter)->SetScale(kOption.vScale);
            (*vecLiveIter)->SetRender(kOption.bIsRender);
            if (kOption.iTechnique != -1)
            {
                (*vecLiveIter)->SetTechnique(kOption.iTechnique);
            }

            std::vector<ADD_ITEM_LIVEMODEL>::iterator vecAddLiveIter = kOption.vecAddItemLiveModel.begin();
            for (; vecAddLiveIter != kOption.vecAddItemLiveModel.end(); ++vecAddLiveIter)
            {
                std::vector<GCObject*> vecAddLiveModel = g_pItemMgr->CreateLiveModel(pItem_->dwGoodsID, iCharType_, iPromotion_, pObject_, vecAddLiveIter->vPos, vecAddLiveIter->bOnlyTraceParentPos, const_cast<char*>(vecAddLiveIter->strMotion.c_str()), vecAddLiveIter->iBoneID, vecAddLiveIter->bUseBoneIDFromArg);
                std::vector<GCObject*>::iterator vecAddLiveModelIter = vecAddLiveModel.begin();
                for (; vecAddLiveModelIter != vecAddLiveModel.end(); ++vecAddLiveModelIter)
                {
                    (*vecAddLiveModelIter)->m_strName = vecAddLiveIter->strChildName;
                    (*vecAddLiveModelIter)->SetTechnique(vecAddLiveIter->iTechnique);
                    (*vecAddLiveModelIter)->SetRender(vecAddLiveIter->bRender);
                }
            }
        }
    }
}

void PLAYER::LoadRoomAnimationModel(GCObject* pObject_, PLAYER_TEMPLATE& kPlayerTemplate_, std::string RoomAnimationModel, bool bTempPlayer_, int isWeapon)
{
    DWORD currentSlot = 0x00000000;
    if (isWeapon > 0)
        currentSlot = 0x00000080;

    bool bGamePlaying = !(false == m_bResultTower && (GC_RUS_PLAYING != m_kUserInfo.nUserState || bTempPlayer_));

    ITEM_MESH_OPTION kOption;
    std::vector< ITEM_MESH_OPTION >::iterator vecMeshOptionIter = kPlayerTemplate_.vecItemMeshOption.begin();
    for (; vecMeshOptionIter != kPlayerTemplate_.vecItemMeshOption.end(); ++vecMeshOptionIter)
    {
        if (currentSlot != vecMeshOptionIter->dwConSlotPosition)
            continue;
        if (0 == vecMeshOptionIter->iConPlaying && bGamePlaying)
            continue;
        if (1 == vecMeshOptionIter->iConPlaying && (false == bGamePlaying))
            continue;
        if (1 == vecMeshOptionIter->iConSquare)
            continue;
        kOption = (*vecMeshOptionIter);
        break;
    }

    if (kOption.bIsNotLoad)
        return;

    if (kOption.strChildName.empty())
        kOption.strChildName = GetDefaultMeshName(currentSlot);

    bool bBuild = !bTempPlayer_;
    if (currentSlot & ESP_WEAPON)
        bBuild = false;

    GCMeshObject* pMeshObject = g_pItemMgr->CreateRoomAnimationModel(RoomAnimationModel, pObject_, bBuild, static_cast<char>(kOption.iBoneID));
    if (pMeshObject)
    {
        if (GetCurrentChar().iCharType == GC_CHAR_ZERO)
            pMeshObject->m_bRender = true;
        else
            pMeshObject->m_bRender = kOption.bIsRender;
        pMeshObject->m_strName = kOption.strChildName;
        if (kOption.iTechnique != -1)
            pMeshObject->m_iTechnique = kOption.iTechnique;
    }
}

std::string PLAYER::GetDefaultMeshName(DWORD dwSlotPosition)
{
    if (dwSlotPosition & ESP_WEAPON)
        return "WEAPON";
    return "";
}

void PLAYER::LoadAddtionModel(GCObject* pObject, PLAYER_TEMPLATE& kPlayerTemplate, bool bTempPlayer_)
{
    if (NULL == pObject)
        return;

    if (IsRemovePartsItem(HEAD))
    {
        if (DontAppearParts(GetCurrentChar().iCharType) == HEAD)
            return;
        return;
    }

    std::vector< ADD_MESH >::iterator vecAddMeshIter = kPlayerTemplate.vecAddMesh.begin();
    for (; vecAddMeshIter != kPlayerTemplate.vecAddMesh.end(); ++vecAddMeshIter)
    {
        // 모든 조건을 통과해야만 추가 메시를 로드할 수 있닷!!
        bool bConBigHead = true, bConEquipItem = true, bConNonEquipItem = true, bConGameModeCategory = true, bConPlaying = true, bConEquipSetItem = true, bConNonEquipSetItem = true;

        // 빅헤드 조건확인( 0이면 빅헤드가 아닐때 로드, 1이면 빅헤드 일때 로드 )
        if (-1 < vecAddMeshIter->iConBigHead)
        {
            if (m_bEquipBigHead)
                bConBigHead = !(0 == vecAddMeshIter->iConBigHead);
            else
                bConBigHead = !(1 == vecAddMeshIter->iConBigHead);
        }

        // 장착한 장비 조건 확인
        if (-1 < vecAddMeshIter->iConEquipItem)
        {
            bConEquipItem = g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&m_kUserInfo.GetCurrentChar(), static_cast<GCITEMID>(vecAddMeshIter->iConEquipItem), vecAddMeshIter->bIsCoordi);
        }

        // 장착한 안한 장비 조건 확인
        if (-1 < vecAddMeshIter->iConNonEquipItem)
        {
            bConNonEquipItem = (false == g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&m_kUserInfo.GetCurrentChar(), static_cast<GCITEMID>(vecAddMeshIter->iConNonEquipItem), vecAddMeshIter->bIsCoordi));
        }

        // 장착한 세트 장비 조건 확인
        if (-1 < vecAddMeshIter->iConEquipSetItem)
        {
            bConEquipSetItem = CheckSetEquipItem(vecAddMeshIter->iConEquipSetItem);
        }

        // 장착 안한  세트 장비 조건 확인
        if (-1 < vecAddMeshIter->iConNonEquipSetItem)
        {
            bConNonEquipSetItem = (CheckSetEquipItem(vecAddMeshIter->iConNonEquipSetItem) == true) ? false : true;
        }

        // 게임모드카테고리 조건 확인
        if (-1 < vecAddMeshIter->iConGameModeCategory)
        {
            bConGameModeCategory = (vecAddMeshIter->iConGameModeCategory == static_cast<int>(SiKGCRoomManager()->GetGameModeCategory()));
        }

        // 플레이 중인지 조건 확인
        if (-1 < vecAddMeshIter->iConPlaying)
        {
            // 게임 중인지 판단
            bool bGamePlaying = !(false == m_bResultTower && (GC_RUS_PLAYING != m_kUserInfo.nUserState || bTempPlayer_));

            // 게임 플레이 중이 아닌데 0이 셋팅되어있으면
            if (0 == vecAddMeshIter->iConPlaying && bGamePlaying)
                bConPlaying = false;

            // 게임 플레이 중인데 1이 셋팅되어있으면
            if (1 == vecAddMeshIter->iConPlaying && (false == bGamePlaying))
                bConPlaying = false;
        }

        // 종착지
        if (bConBigHead && bConEquipItem && bConNonEquipItem && bConGameModeCategory && bConPlaying && bConEquipSetItem && bConNonEquipSetItem)
        {
            if ("" == vecAddMeshIter->piLiveInfo.first)
            {
                GCMeshObject* pAddMesh = pObject->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(vecAddMeshIter->strMeshName), g_pGCDeviceManager2->CreateTexture(vecAddMeshIter->strTextureName));
                if (pAddMesh != NULL)
                {
                    pAddMesh->m_bExtraMeshObject = vecAddMeshIter->bIsExtraMesh;
                    pAddMesh->m_iTechnique = vecAddMeshIter->iTechnique;
                    pAddMesh->m_bRender = vecAddMeshIter->bIsRender;
                    pAddMesh->m_bSecondBaseMesh = vecAddMeshIter->bIsSecondBaseMesh;
                    pAddMesh->m_strName = vecAddMeshIter->strID;
                    if (false == vecAddMeshIter->strChildName.empty())
                        pAddMesh->m_strName = vecAddMeshIter->strChildName;
                }
            }
            else
            {
                GCObject* pAddMesh = g_pGCDeviceManager2->CreateLiveModel(vecAddMeshIter->strMeshName.c_str(), vecAddMeshIter->strTextureName.c_str(), vecAddMeshIter->piLiveInfo.first.c_str(), vecAddMeshIter->piLiveInfo.second, pObject);
                if (pAddMesh != NULL)
                {
                    pAddMesh->m_bIsExtraObject = vecAddMeshIter->bIsExtraMesh;
                    pAddMesh->m_strName = vecAddMeshIter->strID;
                    pAddMesh->SetTechnique(vecAddMeshIter->iTechnique);
                    pAddMesh->SetRender(vecAddMeshIter->bIsRender);
                    if (false == vecAddMeshIter->strChildName.empty())
                        pAddMesh->m_strName = vecAddMeshIter->strChildName;
                }
            }
        }
    }
}

void PLAYER::LoadETCModel(GCObject* pObject_, char cExtraCharNum_, bool bTempPlayer_)
{
    // 얼굴표정 리소스 로드
    if (NULL == m_pNonChangeFaceLift)
        LoadFaceLift(pObject_);

    // 아지트 캐릭터 셋팅UI에서 필요한 라이브메시 로드
    if (bTempPlayer_ && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
    {
        GCObject* pChild = g_pGCDeviceManager2->CreateLiveModel("broom.p3m", "broom.dds", "NPC_broom_wait.frm", 14, pObject_);
        if (pChild)
        {
            pChild->m_strName = "AGIT_BROOM";
            pChild->SetRender(false);
        }
    }

    // 대기룸이나 결과타워에서 사용될 무기모션 하드코드
    if (m_bSetSelectMotion && (0 <= cExtraCharNum_ && cExtraCharNum_ < CID_MAX))
    {
        m_bSetSelectMotion = false;
        CreateShowRoomETCMotionFile(g_kGlobalValue.GetCharacterSelectMotionID(cExtraCharNum_));
    }
}

void PLAYER::CreateShowRoomETCMotionFile(int iCharMotionIndex)
{
    switch (iCharMotionIndex)
    {
    case MID_MARI4_WIN:
    case MID_MARI4_RESULT_WIN:
        SetChildMeshMotion_Front("WEAPON", "SoulTaker_3256");
        break;
    case MID_MARI4_LOSE:
    case MID_MARI4_RESULT_LOSE:
        SetChildMeshMotion_Front("WEAPON", "SoulTaker_3259");
        break;
    case MID_MARI4_SELECT:
        SetChildMeshMotion_Front("WEAPON", "SoulTaker_3260");
        break;
    case MID_LUPUS1_SELECT:
        SetChildMeshMotion_Front("WEAPON_0", "Lupus_Gun_3318_15");
        SetChildMeshMotion_Front("WEAPON_1", "Lupus_Gun_3318_16");
        break;
    }
}

// 얼굴 표정변환 시스템에 어울리지 않는 특수복장과 악세사리 아이템때문에
// 어울리는 표정 하나만 로드하기 위한 코드입니다.
void PLAYER::LoadFaceTexture(GCObject* pObject, int iCharType, int iPromotion, int iItemID, DWORD dwSlotPosition)
{
    // 일반 머리와 빅헤드, 헤드하위장식이 아니면 나가
    DWORD dwBigHead = ESP_A_UPPER_HELMET + ESP_A_DOWN_HELMET;
    if ((ESP_HELMET != (dwSlotPosition & ESP_HELMET)) &&
        (ESP_A_DOWN_HELMET != (dwSlotPosition & ESP_A_DOWN_HELMET)) &&
        (dwBigHead != (dwSlotPosition & dwBigHead)))
        return;

    // 하위장식이 로드되었는데 헬멧로드하려고 하면 나가
    if (m_bLoadDownHelmetNFL && (dwSlotPosition == (dwSlotPosition & ESP_HELMET)))
        return;

    // 리소스 이름을 만들어보자
    std::string strTextureName;
    const char* strAbtaName = g_pGCDeviceManager2->GetAbtaName(g_pItemMgr->GetSameImageIndex(iItemID));
    strTextureName = g_pGCDeviceManager2->GetLupusResourceFileName(strAbtaName, iCharType, iPromotion, "_NFL.dds");
    if ("" == strTextureName)
        strTextureName = g_pGCDeviceManager2->GetResourceFileName(strAbtaName, iCharType, iPromotion, "_NFL.dds");

    // 텍스쳐 파일이름기록이 안됫으면 없는 리소스니깐 나가
    if (strTextureName.empty() || "" == strTextureName)
        return;

    int iFaceLiftType;
    if (m_bEquipBigHead) {
        iFaceLiftType = FACE_LIFT_BIG_HEAD;
    }
    else {
        iFaceLiftType = FACE_LIFT_NORMAL;
    }

    if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&m_kUserInfo.GetCurrentChar(), ITEMID_ASIN_SHAADOW_SET, true)) {
        iFaceLiftType = FACE_LIFT_SHADOW;
    }

    PLAYER_TEMPLATE kPlayerTemplate;
    GetPlayerTemplate(Extra_Char_Num, kPlayerTemplate);
    std::map< int, FACE_LIFT_MESH >::iterator mit = kPlayerTemplate.mapFaceLiftMesh.find(iFaceLiftType);
    if (mit != kPlayerTemplate.mapFaceLiftMesh.end())
    {
        bool bDownHelmet = false;
        if ((dwSlotPosition == (dwSlotPosition & ESP_A_DOWN_HELMET)))
            bDownHelmet = true;

        // 하위장식 로드하려고 하는데 다른 헬멧로드된 상태면 그 헬멧 지워
        if (bDownHelmet && m_pNonChangeFaceLift)
        {
            pObject->RemoveMesh(m_pNonChangeFaceLift->m_pMesh);
            m_pNonChangeFaceLift = NULL;
        }

        GCMeshObject* pMeshObj = pObject->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(mit->second.strMeshName), g_pGCDeviceManager2->CreateTexture(strTextureName.c_str()));
        if (pMeshObj != NULL)
        {
            pMeshObj->m_bExtraMeshObject = true;
            pMeshObj->m_bRender = true;
            m_pNonChangeFaceLift = pMeshObj;

            // 하위장식 로드했으면 로드했다고 표시
            if (bDownHelmet)
                m_bLoadDownHelmetNFL = true;
        }
    }
}

void PLAYER::LoadFaceLift(GCObject* pObject)
{
    m_mapFaceLiftMeshObject.clear();

    PLAYER_TEMPLATE kPlayerTemplate;
    GetPlayerTemplate(Extra_Char_Num, kPlayerTemplate);

    if (IsRemovePartsItem(HEAD))
        return;

    if (kPlayerTemplate.mapFaceLiftMesh.empty() == false) {
        if (m_bEquipBigHead) {
            m_iDefaultFaceLiftType = FACE_LIFT_BIG_HEAD;
        }
        else {
            m_iDefaultFaceLiftType = FACE_LIFT_NORMAL;
        }

        if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&m_kUserInfo.GetCurrentChar(), ITEMID_ASIN_SHAADOW_SET, true)) {
            m_iDefaultFaceLiftType = FACE_LIFT_SHADOW;
        }

        std::map< int, FACE_LIFT_MESH >::iterator mit = kPlayerTemplate.mapFaceLiftMesh.begin();
        for (mit; mit != kPlayerTemplate.mapFaceLiftMesh.end(); ++mit) {
            GCMeshObject* pMeshObj = NULL;
            pMeshObj = pObject->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(mit->second.strMeshName), g_pGCDeviceManager2->CreateTexture(mit->second.strTextureName));
            if (pMeshObj != NULL) {
                pMeshObj->m_bExtraMeshObject = true;
                pMeshObj->m_bRender = false;
                m_mapFaceLiftMeshObject.insert(std::make_pair(mit->first, pMeshObj));
            }
        }

        // 최초 얼굴표정은 빅대가리 면상 아니면 일반 면상
        std::map< int, GCMeshObject* >::iterator mitFace = m_mapFaceLiftMeshObject.find(m_iDefaultFaceLiftType);
        if (mitFace != m_mapFaceLiftMeshObject.end() && mitFace->second) {
            mitFace->second->m_bExtraMeshObject = false;
            mitFace->second->m_bRender = true;
        }
    }
}

std::pair<std::wstring, D3DCOLOR> PLAYER::CreatePlayerName(GCObject* pObject_)
{
    std::wstringstream strmPlayerName;
    SAbilityRevision* pAbilityRevision = SiKGCWorldMapManager()->GetAbilityRevisionInfo(SiKGCRoomManager()->GetGameMode());
    bool bAbilityRevision = (pAbilityRevision && pAbilityRevision->bLoadTable);
    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL &&
        false == g_MyD3D->m_pkQuestGameMgr->GetReviseCharLevel() &&
        false == bAbilityRevision)
        strmPlayerName << g_pkStrLoader->GetString(STR_ID_LEVEL).c_str() << L" " << GetCurrentChar().iLevel << L". ";

    strmPlayerName << GetStrUserName();

    D3DCOLOR Color;
    if (pObject_ && strmPlayerName.rdbuf()->in_avail() > 0)
    {
        if (!g_MyD3D->IsMyTeam(m_iPlayerIndex)) {
            Color = 0xFFFF0000;
        }
        else {
            Color = SiGCTextColorManager()->StringToColor(GetStrUserColor());
        }

        char tmp[128];
        sprintf(tmp, "character_%d.dds", GetCurrentChar().iCharType);
        pObject_->SetText(0, strmPlayerName.str(), Color, 0xFF000000);
        pObject_->SetText(1, strmPlayerName.str(), Color, 0xFF000000, tmp);
        pObject_->Build();
    }

    return std::pair<std::wstring, D3DCOLOR>(strmPlayerName.str(), Color);
}

//필요없는 Resource를 모두 반환하고 필요한것들을 모두 로딩한다.
void PLAYER::UpdatePlayerResource(bool bIsTempPlayer_ /* = false */, bool bInitEgoItem /*true*/)
{
    if (m_kUserInfo.cCharIndex == -1)
    {
        // 안골랐으면 다 지움.
        ReleasePlayerResource();
        return;
    }

    AutoSetExtra();
    SCharInfo& CharInfo = GetCurrentChar();

    // TODO : Asirion m_pObject Release를 아주 조심해야 한다.
    // 메모리 조낸 샐 수 있다.
    if (m_pObject == NULL)
    {
        // temp player 는 manager에 등록 하지 않는다.
        if (bIsTempPlayer_)
        {
            m_pObject = new GCObject();
        }
        else
        {
            m_pObject = g_RenderManager->CreateObject();
        }
#ifdef _DEBUG   
        m_pObject->m_strName = "플레이어 변신 전";
#endif
        m_pObjMetaForm[FORM_NORMAL] = m_pObject;
        m_pObject->SetMeshComputePaletteInterface((IGCComputePaletteP3M*)this);
    }
    m_mapFaceLiftMeshObject.clear();
    m_pObject->RemoveMesh(NULL);
    m_pLire3Arrow = NULL;
    m_pNonChangeFaceLift = NULL;

    bool isOptionEnabled = ( !g_MyD3D->m_KGCOption.GetRoomAnimationEnable() && m_iPlayerIndex != g_MyD3D->Get_MyPlayer() ) || m_iPlayerIndex == g_MyD3D->Get_MyPlayer();

    if ( m_pObject )
    {
        if( isOptionEnabled )
            CreateItemModel( m_pObject, CharInfo, Extra_Char_Num, bIsTempPlayer_ );
        else
            CreateItemModelClean( m_pObject, CharInfo, Extra_Char_Num, bIsTempPlayer_ );
    }

    if (!bIsTempPlayer_)
    {
        m_prPlayerNameWithColor = CreatePlayerName(m_pObject);
    }

    // ------------------------------------- 리르3의 화살 특수 처리 -------------------------------------
    if (Extra_Char_Num == CID_LIRE3)
    {
        m_pLire3Arrow = GetMetaForm(FORM_NORMAL)->AddMesh(g_pGCDeviceManager2->CreateMeshP3M("Lire3Arrow.p3m"), g_pGCDeviceManager2->CreateTexture("Lire3Arrow.dds"));
        m_pLire3Arrow->m_bRender = false;
    }

    InitHittingGauge();
    SetMariSummonState(false);
    if (bInitEgoItem)
        EgoItemInitialize();

    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_CHANGE_CHAR_INFO, m_iPlayerIndex));

    if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM) {
        GetCurFormTemplate().vecExtraMesh.clear();
    }
    else {
        LoadExtraMesh(GetCurFormTemplate().vecExtraMesh);
    }
}

void PLAYER::UpdatePlayerResourceClean(bool bIsTempPlayer_ /* = false */, bool bInitEgoItem /*true*/)
{
    if (m_kUserInfo.cCharIndex == -1)
    {
        // 안골랐으면 다 지움.
        ReleasePlayerResource();
        return;
    }

    AutoSetExtra();
    SCharInfo& CharInfo = GetCurrentChar();

    // TODO : Asirion m_pObject Release를 아주 조심해야 한다.
    // 메모리 조낸 샐 수 있다.
    if (m_pObject == NULL)
    {
        // temp player 는 manager에 등록 하지 않는다.
        if (bIsTempPlayer_)
        {
            m_pObject = new GCObject();
        }
        else
        {
            m_pObject = g_RenderManager->CreateObject();
        }
#ifdef _DEBUG   
        m_pObject->m_strName = "플레이어 변신 전";
#endif
        m_pObjMetaForm[FORM_NORMAL] = m_pObject;
        m_pObject->SetMeshComputePaletteInterface((IGCComputePaletteP3M*)this);
    }
    m_mapFaceLiftMeshObject.clear();
    m_pObject->RemoveMesh(NULL);
    m_pLire3Arrow = NULL;
    m_pNonChangeFaceLift = NULL;

    if (m_pObject)
        CreateItemModelClean(m_pObject, CharInfo, Extra_Char_Num, bIsTempPlayer_);

    if (!bIsTempPlayer_)
    {
        m_prPlayerNameWithColor = CreatePlayerName(m_pObject);
    }

    // ------------------------------------- 리르3의 화살 특수 처리 -------------------------------------
    if (Extra_Char_Num == CID_LIRE3)
    {
        m_pLire3Arrow = GetMetaForm(FORM_NORMAL)->AddMesh(g_pGCDeviceManager2->CreateMeshP3M("Lire3Arrow.p3m"), g_pGCDeviceManager2->CreateTexture("Lire3Arrow.dds"));
        m_pLire3Arrow->m_bRender = false;
    }

    InitHittingGauge();
    SetMariSummonState(false);
    if (bInitEgoItem)
        EgoItemInitialize();

    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_CHANGE_CHAR_INFO, m_iPlayerIndex));

    if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM) {
        GetCurFormTemplate().vecExtraMesh.clear();
    }
    else {
        LoadExtraMesh(GetCurFormTemplate().vecExtraMesh);
    }
}


GCMeshObject* PLAYER::AddMeshObject(std::string strMesh_, std::string strTexture_, EGCCartoonTexture eCartoon_)
{
    return m_pObject->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(strMesh_), g_pGCDeviceManager2->CreateTexture(strTexture_), eCartoon_);
}

void PLAYER::SetTexture(char* szTex)
{
    m_iSelectTextrue = -1;
    if (szTex)
    {
        if (!(m_iSelectTextrue = m_pObject->IsLoadTexture(szTex)))
            m_iSelectTextrue = m_pObject->AddTexture(g_pGCDeviceManager2->CreateTexture(szTex));
    }
}

void PLAYER::RemoveMeshObject(std::string strMesh_)
{
    GCDeviceMeshP3M* pP3M = g_pGCDeviceManager2->CreateMeshP3M(strMesh_);
    m_pObject->RemoveMesh(pP3M);
    pP3M->Release();
}

void PLAYER::AddChildLiveModel(char* strName, char* strMesh, char* stdTexture, char* strMotion, int iBone, float fZPos)
{
    D3DXVECTOR3 vOptionalPos(0.0f, 0.0f, fZPos);
    GCObject* pChild = g_pGCDeviceManager2->CreateLiveModel(strMesh, stdTexture, strMotion, iBone, m_pObject, vOptionalPos);
    if (pChild)
    {
        pChild->m_strName = strName;
        pChild->SetRender(true);
    }
}

void PLAYER::RemoveChildLiveModel(char* strName)
{
    m_pObject->RemoveMesh(strName);
}

bool PLAYER::CheckEquipItem(int ItemID, bool bCoordi)
{
    return g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), ItemID, bCoordi);
}

bool PLAYER::CheckSetEquipItem(int ItemID)
{
    return g_pItemMgr->IsChangeMeshCoordi(ItemID);
}


// 변신 상태에 따라서 변신!
void PLAYER::ProcessMetamorphosisFormChange(int iFormID)
{
    if (IsDetermineWin)
        return;

    // 펫 변신중이면 다음에 걸어줘야 할 버프 있는지 확인
    std::pair< int, float > prNextTransformPet = GetNextTransformPetBuff();
    SetNextTransformPetBuff(prNextTransformPet.first, prNextTransformPet.second);

    if (GetCurrentChar().iCharType == GC_CHAR_RYAN) {
        std::map< int, std::vector< GCHotKeySkill > >::iterator mitHotKey = m_mapHotKeySkill.find(m_FormTemplate[iFormID].sMetamorphosisTemplate.iFormID);
        std::map< int, std::vector< GCHotKeySkillInfo > >::iterator mitCharging = m_mapPossibleChargingSkillInfo.find(m_FormTemplate[iFormID].sMetamorphosisTemplate.iFormID);
        if (mitHotKey != m_mapHotKeySkill.end() && mitCharging != m_mapPossibleChargingSkillInfo.end()) {
            m_vecHotKeySkill = mitHotKey->second;
            m_vecPossibleChargingSkillInfo = mitCharging->second;
        }
    }
    else {
        SetNormalTpye();

        if (m_FormTemplate[iFormID].vecQuickSlotSkill.empty() == false) {
            m_vecHotKeySkill.clear();
            m_vecPossibleChargingSkillInfo.clear();

            std::vector< EGCSkillTree > vecEquipSkillID;
            for (int i = 0; i < static_cast<int>(m_FormTemplate[iFormID].vecQuickSlotSkill.size()); i++) {
                vecEquipSkillID.push_back(static_cast<EGCSkillTree>(m_FormTemplate[iFormID].vecQuickSlotSkill[i]));
            }

            const GCSkill* pSkill = NULL;
            int iSkillCount = 0;
            for (int i = 0; i < static_cast<int>(vecEquipSkillID.size()); i++) {

                if (CheckSkill(vecEquipSkillID[i])) {
                    pSkill = SiGCSKT()->GetSkill(vecEquipSkillID[i]);
                    if (pSkill && pSkill->m_pHotKeySkill)
                    {
                        GCHotKeySkill skill;
                        skill.m_eSkill = static_cast<EGCSkillTree>(m_FormTemplate[iFormID].vecQuickSlotSkill[i]);
                        GCHotKeySkillInfo* info = pSkill->m_pHotKeySkill;
                        skill.pTexture = g_pGCDeviceManager2->CreateTexture(pSkill->GetTextureFileName());
                        skill.pAddMotionTexture = g_pGCDeviceManager2->CreateTexture(pSkill->GetAddMotionTextureFileName());
                        m_vecHotKeySkill.push_back(skill);
                        if (info->m_iChargingSkillLV > 0) {
                            m_vecPossibleChargingSkillInfo.push_back(*info);
                        }

                        iSkillCount++;
                    }
                }

                if (iSkillCount >= MAX_SLOT_COUNT) {
                    break;
                }
            }

            if (!m_vecPossibleChargingSkillInfo.empty()) {
                std::sort(m_vecPossibleChargingSkillInfo.begin(), m_vecPossibleChargingSkillInfo.end(), KChargingSkillLevelHigh());
            }
        }
    }

    // 피통 늘려야 된다면 늘려줘야지!!
    float fPrevMaxHP = m_fAbility[ABILITY_HP];
    m_fAbility[ABILITY_HP] = m_fAbility[ABILITY_HP] * m_FormTemplate[iFormID].sMetamorphosisTemplate.StartHPRatio;

    //피통이 늘어날 경우에 피도 적절히 늘려주기위한 비율
    float fNowHPRatio;
    fNowHPRatio = GetHP() / m_fAbility[ABILITY_HP];

    // 데미지 효과 적용
    if (GetHP() > m_fAbility[ABILITY_HP] * m_FormTemplate[iFormID].sMetamorphosisTemplate.fDamageRatio)
    {
        SetHP(GetHP() - m_fAbility[ABILITY_HP] * m_FormTemplate[iFormID].sMetamorphosisTemplate.fDamageRatio);
    }

    if (iFormID == FORM_NEPHILIM || iFormID == FORM_MDAN) {
        // 피통이 실제로 늘어났을 때만 피 적절히 올려주자.
        if (fPrevMaxHP < m_fAbility[ABILITY_HP]) {
            SetHP(GetHP() + (GetHP() * fNowHPRatio));
        }
    }

    // 아이템 효과 적용
    if (m_FormTemplate[iFormID].sMetamorphosisTemplate.iUseItem != -1)
    {
        g_MyD3D->MyItms->Use_Item(m_iPlayerIndex, m_FormTemplate[iFormID].sMetamorphosisTemplate.iUseItem);
    }

    TransformStartParticle(m_FormTemplate[iFormID].sMetamorphosisTemplate.iFormID);

    // 변신을 알아서 한다
    MetamorphosisForm = m_FormTemplate[iFormID].sMetamorphosisTemplate.iFormID;

    // 순서
    TransformStartBuff(m_FormTemplate[iFormID].sMetamorphosisTemplate.iFormID);

    // 변신 할 때 강화 목걸이 무기 이펙트를 보이지 않게 한다
    SetShowEquipEffect(EPT_AXE, false);
    SetShowEquipEffect(EPT_CRICENT, false);

    // 네피림이면 슈퍼 아머 && 주먹불
    if (MetamorphosisForm == FORM_NEPHILIM)
        SuperArmor(true);

    UINT uiChaneMotion = 0;
    if (m_FormTemplate[MetamorphosisForm].sMetamorphosisTemplate.FormChangeCompleteMotion == -1)
    {
        Direct_Motion_Input(m_FormTemplate[iFormID].WAIT);
        uiChaneMotion = m_FormTemplate[iFormID].WAIT;
    }
    else
    {
        Direct_Motion_Input(m_FormTemplate[MetamorphosisForm].sMetamorphosisTemplate.FormChangeCompleteMotion);
        uiChaneMotion = m_FormTemplate[MetamorphosisForm].sMetamorphosisTemplate.FormChangeCompleteMotion;
    }

    // 마나 회복 바보로 만듬. ( 30초시 1칸반 45초시 2칸 )
    SetIncMP(0.0008f);
    g_MyD3D->m_fIncMPShield = 0.00006f;

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
    {
        // Tag CharacterInfo도 Setting
        std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.find(GetCurrentChar().iCharType);
        if (mit == m_mapTagCharInfo.end())
        {
            ASSERT(!"TagCharInfo에 현재 케릭터 정보가 없음");
        }
        TagCharacterInfo& tagInfo = mit->second;
    }
    if (GetMetaForm(iFormID) && GetMetaForm(FORM_NORMAL))
    {
        g_RenderManager->RemoveObject(GetMetaForm(FORM_NORMAL));
        g_RenderManager->AddObject(GetMetaForm(iFormID));
        GetMetaForm(iFormID)->SetMeshState(GetMetaForm(FORM_NORMAL)->GetMeshState());
    }
#ifdef _DEBUG
    m_pObjMetaForm[iFormID]->m_strName = "플레이어 변신후";
#endif
    m_pObject = m_pObjMetaForm[iFormID];

    if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_BOMB_INFECT))
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_BOMB_INFECT);
    }

    LoadSlash();
    SyncTransform(uiChaneMotion);
}

void PLAYER::LoadSlash()
{
    SetSlashV();
    if (GetCurFormTemplate().sPlayerSlash.strSlashName != "")
        m_pTexSlash = g_pGCDeviceManager2->CreateTexture(GetCurFormTemplate().sPlayerSlash.strSlashName);

    if (GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName.compare("NULL") != 0)
        SetOtherSpecialFaceTexture();
    m_pPlayerSp = g_pGCDeviceManager2->CreateTexture(GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName);

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN)
    {
        m_pSerdinSign = g_pGCDeviceManager2->CreateTexture("Captain_Serdin.dds");
        m_pCanabanSign = g_pGCDeviceManager2->CreateTexture("Captain_Canaban.dds");
    }
}

void PLAYER::TransformEndParticle(int iFormID)
{
    // 시간이 다된 거 or 게임 종료라면 정상적인 이펙트
    for (int i = 0; i < (int)m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect.size(); ++i)
    {
        switch (m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].iEffectKind)
        {
        case 0: //PP_ADD_PARTICLE = 0;
            AddParticle(const_cast<char*>(m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].strEffectName.c_str()),
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].iParticleNum,
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].fOffset);
            break;
        case 1: //PP_ADD_PARTICLE_TRACE = 1;
            AddParticleWithTrace(const_cast<char*>(m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].strEffectName.c_str()),
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].iParticleNum,
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].fOffset);
            break;
        case 2: //PP_ADD_PARTICLE_RANDOM_DIR = 2;
            AddParticleNoDirection(const_cast<char*>(m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].strEffectName.c_str()),
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].iParticleNum,
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecEndEffect[i].fOffset);
            break;
        }
    }
}

void PLAYER::TransformStartParticle(int iFormID)
{
    for (int i = 0; i < (int)m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect.size(); ++i)
    {
        switch (m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].iEffectKind)
        {
        case 0: //PP_ADD_PARTICLE = 0;
            AddParticle(const_cast<char*>(m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].strEffectName.c_str()),
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].iParticleNum,
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].fOffset);
            break;
        case 1: //PP_ADD_PARTICLE_TRACE = 1;
            AddParticleWithTrace(const_cast<char*>(m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].strEffectName.c_str()),
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].iParticleNum,
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].fOffset);
            break;
        case 2: //PP_ADD_PARTICLE_RANDOM_DIR = 2;
            AddParticleNoDirection(const_cast<char*>(m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].strEffectName.c_str()),
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].iParticleNum,
                m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartEffect[i].fOffset);
            break;
        }
    }
}

void PLAYER::TransformStartBuff(int iFormID)
{
    // 2007/2/21. iridology. GetCurFormTemplate()이 바뀌기 전
    // 장착하고 있는 아이템에 따른 지속시간을 구함.
    if (m_FormTemplate[iFormID].sMetamorphosisTemplate.iFormID != -1)
    {
        bool IsItemExist = false;
        int iVectorID = -1;
        for (int i = 0; i < (int)m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartBuffEffect.size(); ++i)
        {
            if (m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartBuffEffect[i].iGoodsID == -1 && !IsItemExist)
            {
                iVectorID = i;
            }
            if (g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartBuffEffect[i].iGoodsID))
            {
                IsItemExist = true;
                iVectorID = i;
                break;
            }
        }

        GetMetaFormBuffTimeIdx(iFormID) = iVectorID;

        // 로컬 버프 착용
        // StartBuffEffect에 Local만 적용되어야 하는지 전체다 적용되어야 하는지 없으므로 필요할때 넣으셈..
        if (IsLocalPlayer() && -1 != GetMetaFormBuffTimeIdx(iFormID))
        {

            EGCPlayerMagicEffect eMagicEffect = (EGCPlayerMagicEffect)m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartBuffEffect[GetMetaFormBuffTimeIdx(iFormID)].iEffectID;
            float fCurrTime = m_FormTemplate[iFormID].sMetamorphosisTemplate.vecStartBuffEffect[GetMetaFormBuffTimeIdx(iFormID)].fBuffTime;

            if (g_kGlobalValue.CheckTrainedSkill(this, SID_RYAN1_WOLF_TIME_EXTENSION) && iFormID == FORM_WOLF)
            {
                fCurrTime = 45.0f;

                if (g_MyD3D->GetMyPlayer()->CheckEquipItem(ITEM_ID_MOONLIGHT_RING))
                {
                    fCurrTime += 15.0f;
                }

                if (g_MyD3D->GetMyPlayer()->CheckEquipItem(ITEM_ID_MOONLIGHT_RING))
                {
                    fCurrTime += 15.0f;
                }
            }

            if (iFormID == FORM_WOLF)
                InitHittingGauge();

            g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, eMagicEffect, fCurrTime);
        }
    }
}

void PLAYER::ProcessMetamorphosisNormalForm(bool b3rdTransformClear/* = true*/, int iChangeMotion_/* = -1*/)
{
    // [3/18/2008] breadceo. 라이언 4차 선행작업
    if (MetamorphosisForm == FORM_MDAN && false == b3rdTransformClear)
    {
        return;
    }

    if (GetCurrentChar().iCharType == GC_CHAR_RYAN || GetCurFormTemplate().vecQuickSlotSkill.empty() == false) {
        if (g_MyD3D->MyHead->IsBlackMagic == true && m_iSkillType == 0) {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_A];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_A];
        }
        else if (g_MyD3D->MyHead->IsBlackMagic == false && m_iSkillType == 0) {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_B];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_B];
        }
        else if (g_MyD3D->MyHead->IsBlackMagic == true && m_iSkillType == 1) {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_A];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_A];
        }
        else if (g_MyD3D->MyHead->IsBlackMagic == false && m_iSkillType == 1) {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_B];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_B];
        }
    }

    const int iBeforeMetamorphosisFromID = MetamorphosisForm;
    bool bTagCharTranslation = false;

    if (MetamorphosisForm != FORM_NORMAL)
    {
        TransformEndParticle(iBeforeMetamorphosisFromID);

        // 네피림이면 슈퍼 아머
        if (MetamorphosisForm == FORM_NEPHILIM)
        {
            SuperArmor(false);
            HandFireShow(false);
        }

        //  늑대면 HayperArmor 종료
        //  모션 중간에 풀리면 정상적으로 해제 안되서 이렇게 처리 ( 점프 공격 같은거 )
        if (MetamorphosisForm == FORM_WOLF)
        {
            if (m_bHyperArmor)
                m_bHyperArmor = false;
        }

        // 종료 버프가 있으면 종료 버프들 걸어주자.
        if (GetFormTemplate(iBeforeMetamorphosisFromID).sMetamorphosisTemplate.vecEndBuffEffect.empty() == false) {
            std::vector< METAMORPHOSIS_TEMPLATE::S_EndBuffEffect > vecEndBuffEffect = GetFormTemplate(iBeforeMetamorphosisFromID).sMetamorphosisTemplate.vecEndBuffEffect;
            for (int i = 0; i < static_cast<int>(vecEndBuffEffect.size()); i++) {
                SetMagicEffectOnlyLocal(vecEndBuffEffect[i].iEffectID, vecEndBuffEffect[i].fBuffTime);
            }
        }

        // 변신 풀릴 시 지워야 할 버프들이 있으면 지워주자.
        if (GetFormTemplate(iBeforeMetamorphosisFromID).sMetamorphosisTemplate.setEndClearBuffList.empty() == false) {
            std::set< int > setEndClearBuffList = GetFormTemplate(iBeforeMetamorphosisFromID).sMetamorphosisTemplate.setEndClearBuffList;
            for (std::set< int >::iterator sit = setEndClearBuffList.begin(); sit != setEndClearBuffList.end(); ++sit) {
                ClearMagicEffect(*sit);
            }
        }

        // 변신에서 돌아 올 때 강화 목걸이 무기 이펙트를 보이게 한다
        SetShowEquipEffect(EPT_AXE, m_bRender);
        SetShowEquipEffect(EPT_CRICENT, m_bRender);

        // 변신 풀릴때 
        //if( IsLocalPlayer() && -1 != GetMetaFormBuffTimeIdx(iBeforeMetamorphosisFromID) )
        //g_MyD3D->MyBuff->CleanBuff( false, GetFormTemplate(iBeforeMetamorphosisFromID).sMetamorphosisTemplate.vecStartBuffEffect[GetMetaFormBuffTimeIdx(iBeforeMetamorphosisFromID)].iEffectID );

        m_fAbility[ABILITY_HP] = m_fAbility[ABILITY_HP] * GetFormTemplate(iBeforeMetamorphosisFromID).sMetamorphosisTemplate.EndHPRatio;

    }

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
    {
        // 쉬고있는 놈중에 변신한 놈이 있을수 있다 찾자
        std::map< int, TagCharacterInfo >::iterator mit;

        for (mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit)
        {
            if (GetCurrentChar().iCharType != (*mit).first && FORM_NORMAL != (*mit).second.MetamorphosisForm)
            {
                TagCharacterInfo& tagInfo = mit->second;
                tagInfo.m_fAbility[ABILITY_HP] = tagInfo.m_fAbility[ABILITY_HP] * tagInfo.GetFormTemplate(iBeforeMetamorphosisFromID).sMetamorphosisTemplate.EndHPRatio;

                // 네피림이면 슈퍼 아머
                if (tagInfo.MetamorphosisForm == FORM_NEPHILIM)
                    tagInfo.m_bSuperArmor = false;

                tagInfo.MetamorphosisForm = FORM_NORMAL;

                bTagCharTranslation = true;
            }
        }
    }


    // 2007/2/21. iridology. 변신 풀리는게 게임이 끝나서라면 모든 마법효과를 제거	
    // 포탈 들어갈때도 제거 
    if (IsDetermineWin || uiMotion == GetOwnMotion(MID_COMMON_INTO_TELEPORT))
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, true);

    // 마나 원래대로
    SetIncMP(0.0015f);
    g_MyD3D->m_fIncMPShield = 0.001f;

    if (GetMetaForm(iBeforeMetamorphosisFromID))
    {
        if (g_RenderManager->RemoveObject(GetMetaForm(iBeforeMetamorphosisFromID)))
            g_RenderManager->AddObject(GetMetaForm(FORM_NORMAL));
        GetMetaForm(FORM_NORMAL)->SetMeshState(GetMetaForm(iBeforeMetamorphosisFromID)->GetMeshState());
    }
    m_pObject = GetMetaForm(FORM_NORMAL);
    MetamorphosisForm = FORM_NORMAL;

    //if ( g_pMagicEffect->IsMagicEffect( m_iPlayerIndex, EGC_EFFECT_BOMB_INFECT ) )
    {
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_BOMB_INFECT);
    }

    LoadSlash();

    // Now Motion이 -1인 경우가 있어서 함 고쳐봅니다.
    assert(uiMotion != -1);

    if (uiMotion == GetCurFormTemplate().WIN) {
        return;
    }

    if (iChangeMotion_ == -1) {
        if (uiMotion == -1)
            uiMotion = GetWaitMotion();

        // 변신이 풀렸으니 모션도 라이언의 모션으로 바꿔줘야 한다
        if (false == CheckInPortal())
        {
            if (GetHP() > 0.0f || uiMotion != MID_COMMON_FRONTDOWN_AND_STANDUP)
                Direct_Motion_Input(GetWaitMotion());
        }
    }
    else {
        uiMotion = iChangeMotion_;
        Direct_Motion_Input(uiMotion);
    }

    SyncTransform(uiMotion);
}

void PLAYER::SyncTransform(UINT uiChangeMotion_)
{
    if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
        return;

    if (IsLocalPlayer())
    {
        All_Latency_Equal();
        pLatency = LATENCY;
        g_kTransform->ucPlayerIndex = (UCHAR)g_MyD3D->Get_MyPlayer();
        g_kTransform->cTransformType = (CHAR)MetamorphosisForm;
    }
    else
    {
        for (int i = 0; i < LATENCY; i++)
        {
            L_Now_Motion[i] = uiChangeMotion_;
            L_Frame[i] = 0;
        }
        uiMotion = uiChangeMotion_;
        cFrame = 0;
        pLatency = 0;
    }
}

void PLAYER::GetCatchGPSum(int& nGPSum) const
{
    nGPSum = 0;
    std::vector< KKillCoinInfo >::const_iterator vit;
    for (vit = m_vecKillCoinInfo.begin(); vit != m_vecKillCoinInfo.end(); ++vit)
    {
        nGPSum += vit->m_nCoin;
    }
}

void PLAYER::SetEmoticon(KGCEmoticonInstance* pEmoticon)
{
    m_pEmoticon = pEmoticon;
    if (m_pEmoticon)
    {
        m_pEmoticon->SetIsRight(bIsRight);
        if (m_pObject)
            m_pEmoticon->SetPos(&D3DXVECTOR2(m_pObject->GetWorldMat()->_41, m_pObject->GetWorldMat()->_42));
    }
}

void PLAYER::RenderEmoticon(float left, float top, float right, float bottom)
{
    m_pEmoticon->RenderForResult(left, top, right, bottom);
}

void PLAYER::SetOriginaSpecial(PLAYER_TEMPLATE& PlayerTemplate)

{
    PlayerTemplate.SPECIAL1_W = PlayerTemplate.ORIGINAL1_W;
    PlayerTemplate.SPECIAL2_W = PlayerTemplate.ORIGINAL2_W;
    PlayerTemplate.SPECIAL3_W = PlayerTemplate.ORIGINAL3_W;
    PlayerTemplate.SPECIAL1_B = PlayerTemplate.ORIGINAL1_B;
    PlayerTemplate.SPECIAL2_B = PlayerTemplate.ORIGINAL2_B;
    PlayerTemplate.SPECIAL3_B = PlayerTemplate.ORIGINAL3_B;
}

//어비스나이트 손 이펙트 붙이는 함수
void PLAYER::SetAbyssHandEffect(int iAbyssHandEff, TagCharacterInfo* pTagInfo)
{
    KSafeArray<CParticleEventSeqPTR*, 2> pAbyssHandEff;

    if (pTagInfo)
    {
        pAbyssHandEff[0] = &pTagInfo->m_pAbyssHandEff[0];
        pAbyssHandEff[1] = &pTagInfo->m_pAbyssHandEff[1];
    }
    else
    {
        pAbyssHandEff[0] = &m_pAbyssHandEff[0];
        pAbyssHandEff[1] = &m_pAbyssHandEff[1];
    }

    TRACE_INFO stTrace;
    stTrace.m_pvPos = &m_vecAbyssHandEffPos;
    stTrace.m_fParticleTraceTime = 5.0f;

    if (iAbyssHandEff == 0)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_FIRE)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Fire_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Fire_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_ICE)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_ice_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_ice_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_LIGHTNING)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Lightning_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Lightning_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_DARKNESS)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_dark_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_dark_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_SHINING)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Light_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Light_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_LOVE)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Love_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Love_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_ROSE)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Rozen_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Rozen_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_DEATH)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Death_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Death_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_TIME)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Atropos_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Atropos_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_LIFE)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Leaf_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Leaf_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_DARK_DRAGON_RED)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_DarkDragon_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_DarkDragon_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_DARK_DRAGON_BLUE)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_DarkDragon_B_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_DarkDragon_B_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_FOOLS_DAY)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_Ddong_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_Ddong_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);

    }
    else if (iAbyssHandEff == EIATTR_HALLOWEEN)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Neck_Halloween_Ronan_Hand_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Neck_Halloween_Ronan_Hand_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_ELIA)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_ELIA_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_ELIA_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else if (iAbyssHandEff == EIATTR_PEGASUS_NECK)
    {
        *pAbyssHandEff[0] = g_ParticleManager->CreateSequence("Ronan4_hand_DarkDragon_B_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        *pAbyssHandEff[1] = g_ParticleManager->CreateSequence("Ronan4_hand_DarkDragon_B_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
    }
    else
    {
        return;
    }

    if ((*pAbyssHandEff[0]))
        (*pAbyssHandEff[0])->SetTrace(&stTrace);
    if (iAbyssHandEff != 0)
        if ((*pAbyssHandEff[1]))
            (*pAbyssHandEff[1])->SetTrace(&stTrace);
}

void PLAYER::InitAbyssHandEffect(TagCharacterInfo* pTagInfo)
{
    KSafeArray<CParticleEventSeqPTR*, 2> pAbyssHandEff;

    if (pTagInfo)
    {
        pAbyssHandEff[0] = &pTagInfo->m_pAbyssHandEff[0];
        pAbyssHandEff[1] = &pTagInfo->m_pAbyssHandEff[1];
    }
    else
    {
        pAbyssHandEff[0] = &m_pAbyssHandEff[0];
        pAbyssHandEff[1] = &m_pAbyssHandEff[1];
    }

    m_iAbyssHandEff = 0;
    if (*pAbyssHandEff[0])
        g_ParticleManager->DeleteSequence(*pAbyssHandEff[0]);
    if (*pAbyssHandEff[1])
        g_ParticleManager->DeleteSequence(*pAbyssHandEff[1]);
    *pAbyssHandEff[0] = NULL;
    *pAbyssHandEff[1] = NULL;
}

void PLAYER::InitAbyssHandEffectToScript()
{
    if (m_pAbyssHandEff[0])
        g_ParticleManager->DeleteSequence(m_pAbyssHandEff[0]);
    if (m_pAbyssHandEff[1])
        g_ParticleManager->DeleteSequence(m_pAbyssHandEff[1]);
    m_pAbyssHandEff[0] = NULL;
    m_pAbyssHandEff[1] = NULL;
}

void PLAYER::SetChargeMonoVolt(bool isChargeMonoVolt)
{
    if (isChargeMonoVolt)
    {
        for (int i = 0; i < 4; i++)
        {
            if (m_pMonoVolt[i])
                g_ParticleManager->DeleteSequence(m_pMonoVolt[i]);
            m_pMonoVolt[i] = NULL;
        }

        TRACE_INFO stTrace;
        stTrace.m_pvPos = &m_vecMonoVoltPos;
        m_pMonoVolt[0] = g_ParticleManager->CreateSequence("Ronan4_Monoball_01", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        m_pMonoVolt[1] = g_ParticleManager->CreateSequence("Ronan4_Monoball_02", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        m_pMonoVolt[2] = g_ParticleManager->CreateSequence("Ronan4_Monoball_03", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);
        m_pMonoVolt[3] = g_ParticleManager->CreateSequence("Ronan4_Monoball_04", stTrace.m_pvPos->x, stTrace.m_pvPos->y, stTrace.m_pvPos->z);

        stTrace.m_fParticleTraceTime = 5.0f;

        for (int i = 0; i < 4; i++)
        {
            m_pMonoVolt[i]->SetTrace(&stTrace);
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            if (m_pMonoVolt[i])
                g_ParticleManager->DeleteSequence(m_pMonoVolt[i]);
            m_pMonoVolt[i] = NULL;
        }
    }
}

void PLAYER::InitMonoVolt()
{
    m_bChargeMonoVolt = false;
    for (int i = 0; i < 4; i++)
    {
        if (m_pMonoVolt[i])
            g_ParticleManager->DeleteSequence(m_pMonoVolt[i]);
        m_pMonoVolt[i] = NULL;
    }
}

void PLAYER::ChangeForceNum(bool IsIncrease)
{
    if (true == IsIncrease)
    {
        //g_pMagicEffect->SetMagicEffect( m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF1 , 10.0f );
        if (++m_nForceNum > 3)
            m_nForceNum = 3;
    }
    else
    {
        if (--m_nForceNum < 0)
            m_nForceNum = 0;
    }

    g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF, 10.0f, 1, -1, true);

    //// 이펙트 생성하고, 필요 없는 이펙트는 지우쟈;;
    //if ( m_pRonan3Force != NULL && g_ParticleManager->IsLiveInstance( m_pRonan3Force ) == true )
    //    g_ParticleManager->DeleteSequence( m_pRonan3Force );
    //if ( m_pRonan3Force1 != NULL && g_ParticleManager->IsLiveInstance( m_pRonan3Force1 ) == true )
    //    g_ParticleManager->DeleteSequence( m_pRonan3Force1 );
    //if ( m_pRonan3Force2 != NULL && g_ParticleManager->IsLiveInstance( m_pRonan3Force2 ) == true )
    //    g_ParticleManager->DeleteSequence( m_pRonan3Force2 );

    //int iMagicIndex;
    //if ( g_pMagicEffect->IsMagicEffect( m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF1, &iMagicIndex ) )
    //    g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, iMagicIndex );
    //if ( g_pMagicEffect->IsMagicEffect( m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF2, &iMagicIndex ) )
    //    g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, iMagicIndex );
    //if ( g_pMagicEffect->IsMagicEffect( m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF3, &iMagicIndex ) )
    //    g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, iMagicIndex );

    //MAGIC_EFFECT stEffect;
    //switch ( m_nForceNum )
    //{
    //case 0:
    //    m_pRonan3Force  = NULL;
    //    m_pRonan3Force1 = NULL;
    //    m_pRonan3Force2 = NULL;
    //    break;
    //case 1:
    //    m_pRonan3Force  = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff01", 0,0,0.5f );
    //    m_pRonan3Force1 = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff01_01", 0,0,0.5f );
    //    m_pRonan3Force2 = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff01_02", 0,0,0.5f );

    //    stEffect.m_eMagicEffect = EGC_EFFECT_RONAN3_FORCE_BUFF1;
    //    g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, stEffect);
    //	break;
    //case 2:
    //    m_pRonan3Force  = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff02", 0,0,0.5f );
    //    m_pRonan3Force1 = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff02_01", 0,0,0.5f );
    //    m_pRonan3Force2 = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff02_02", 0,0,0.5f );

    //    stEffect.m_eMagicEffect = EGC_EFFECT_RONAN3_FORCE_BUFF2;
    //    g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, stEffect);
    //    break;
    //case 3:
    //    m_pRonan3Force  = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff03", 0,0,0.5f );
    //    m_pRonan3Force1 = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff03_01", 0,0,0.5f );
    //    m_pRonan3Force2 = g_ParticleManager->CreateSequence( "Ronan3_SPECIAL2-1_buff03_02", 0,0,0.5f );

    //    stEffect.m_eMagicEffect = EGC_EFFECT_RONAN3_FORCE_BUFF3;
    //    g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, stEffect);
    //    break;
    //default:
    //    break;
    //}

    //if ( m_nForceNum > 0 )
    //{
    //    TRACE_INFO stTrace;
    //    stTrace.m_pvPos = ComputePosDesc( EPT_BODY_CENTER );
    //    stTrace.m_fParticleTraceTime = 5.0f;
    //    if ( m_pRonan3Force != NULL && g_ParticleManager->IsLiveInstance( m_pRonan3Force ) == true )
    //        m_pRonan3Force->SetTrace( &stTrace );
    //    if ( m_pRonan3Force1 != NULL && g_ParticleManager->IsLiveInstance( m_pRonan3Force1 ) == true )
    //        m_pRonan3Force1->SetTrace( &stTrace );
    //    if ( m_pRonan3Force2 != NULL && g_ParticleManager->IsLiveInstance( m_pRonan3Force2 ) == true )
    //        m_pRonan3Force2->SetTrace( &stTrace );
    //}
}

void PLAYER::InitForceNum(void)
{
    //포스 이펙트 지우쟈;;
    if (m_pRonan3Force != NULL && g_ParticleManager->IsLiveInstance(m_pRonan3Force) == true)
        g_ParticleManager->DeleteSequence(m_pRonan3Force);
    if (m_pRonan3Force1 != NULL && g_ParticleManager->IsLiveInstance(m_pRonan3Force1) == true)
        g_ParticleManager->DeleteSequence(m_pRonan3Force1);
    if (m_pRonan3Force2 != NULL && g_ParticleManager->IsLiveInstance(m_pRonan3Force2) == true)
        g_ParticleManager->DeleteSequence(m_pRonan3Force2);

    m_pRonan3Force = NULL;
    m_pRonan3Force1 = NULL;
    m_pRonan3Force2 = NULL;

    //int iMagicIndex;
    //if ( g_pMagicEffect->IsMagicEffect( m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF1, &iMagicIndex ) )
    //    g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, iMagicIndex );
    //if ( g_pMagicEffect->IsMagicEffect( m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF2, &iMagicIndex ) )
    //    g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, iMagicIndex );
    //if ( g_pMagicEffect->IsMagicEffect( m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF3, &iMagicIndex ) )
    //    g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, iMagicIndex );

    m_nForceNum = 0;

    g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN3_FORCE_BUFF, 10.0f);
}

void PLAYER::ConfirmUseSheild(char cUseSheild)
{
    m_cUseShield = cUseSheild;
}

bool PLAYER::IsFatal()
{
    if (m_ePlayerState != PS_DEAD && GetHP() <= 0)
        return true;

    return false;
}
void PLAYER::SetMagicEffect(int iID, float ftime, int iLv, bool bNoLocal_)
{
    if (IsLocalPlayer() || bNoLocal_)
    {
        LTP_BUFF buff;
        buff.iBuffIndex = iID;
        buff.iTargetIndex = m_iPlayerIndex;
        buff.fBuffTime = ftime;
        buff.iBuffLv = iLv + 1;
        buff.bClearBuff = false;
        buff.iWho = m_iPlayerIndex;
        buff.dwAttackPlayerUnit = m_kUserInfo.dwUID;
        g_LatencyTransfer.PushPacket(&buff, 1);
    }
}

void PLAYER::SetMagicEffect_NotMeBroad(int iBuffIndex_, float fBuffTime_, int iBuffLevel_)
{
    if (IsLocalPlayer())
    {
        LTP_BUFF buff;
        buff.iBuffIndex = iBuffIndex_;
        buff.iTargetIndex = m_iPlayerIndex;
        buff.fBuffTime = fBuffTime_;
        buff.iBuffLv = iBuffLevel_ + 1;
        buff.bClearBuff = false;
        buff.iWho = m_iPlayerIndex;
        g_LatencyTransfer.PushPacket(&buff, 0);
    }
}

void PLAYER::ClearMagicEffect(int iID, bool bNoLocal_)
{
    if (IsLocalPlayer() || bNoLocal_)
    {
        LTP_BUFF buff;
        buff.iBuffIndex = iID;
        buff.iTargetIndex = m_iPlayerIndex;
        buff.bClearBuff = true;
        g_LatencyTransfer.PushPacket(&buff, 1);
    }
}

void PLAYER::ClearMagicEffect_NotMeBroad(int iBuffIndex_)
{
    if (IsLocalPlayer() && IsMagicEffect(iBuffIndex_))
    {
        LTP_BUFF buff;
        buff.iBuffIndex = iBuffIndex_;
        buff.iTargetIndex = m_iPlayerIndex;
        buff.bClearBuff = true;
        g_LatencyTransfer.PushPacket(&buff, 0);
    }
}

bool PLAYER::IsMagicEffect(int iID, bool CooltimeSkip)
{
    // Comment : 특정 모션안에서 이루어지는 buff체크는 쿨타임 중이 아니고 진짜 버프만 걸린 상태여야 한다 ㅎ 
    if (!CooltimeSkip)
        return 0 != (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, iID) && !g_pMagicEffect->IsCoolTime(m_iPlayerIndex, iID));
    else
        return g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, iID) != 0;
}

void PLAYER::HiddenBuffParticle(int iBuffIndex, bool bIsBufftimeStop, bool bIsHiddenParticle)
{
    if (!g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, iBuffIndex))
        return;
    g_pMagicEffect->HiddenBuffParticle(iBuffIndex, bIsBufftimeStop, bIsBufftimeStop);
}

void PLAYER::SetPlayerName(const WCHAR* strNickName_, wstring strNickColor_)
{
    m_kUserInfo.strNickName = strNickName_;
    m_kUserInfo.strNickColor = strNickColor_;

    D3DCOLOR Color = SiGCTextColorManager()->StringToColor(strNickColor_);

    // draw shadow
    std::wstringstream strmPlayerName;

    if (!g_MyD3D->IsMyTeam(m_iPlayerIndex)) {
        Color = 0xFFFF0000;
    }
    else {
        Color = SiGCTextColorManager()->StringToColor(GetStrUserColor());
    }

    strmPlayerName << GetStrUserName();

    if (m_pObject)
    {
        m_pObject->SetText(0, strmPlayerName.str(), Color);
    }

    m_prPlayerNameWithColor.first = strmPlayerName.str();
    m_prPlayerNameWithColor.second = Color;
}

bool PLAYER::IsLive()
{
    return m_kUserInfo.bLive;
}

int PLAYER::GetCharType(void)
{
    return (int)g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Extra_Char_Num;
}

void PLAYER::ChangeHpToMp(float fHpPercent, float fMpPercent)
{
    if (IsLocalPlayer())
    {
        float fChangeHpToMp = m_fAbility[ABILITY_HP] * fHpPercent;
        SetHP(GetHP() - fChangeHpToMp);
        DecreaseMP(3 * fMpPercent);
    }
}

void PLAYER::SetPlayerHP(float fHP)
{
    if (IsLocalPlayer())
    {
        SetHP(GetHP() + fHP);
    }
}

void PLAYER::Change_Motion(int iMotion) //로컬플레이어 인것에 관계없이 모션 체이지 할려고...
{
    if (false == VerifyRealMotion(iMotion))
    {
        if (IsContact)
            iMotion = GetWaitMotion();
        else
            iMotion = GetCurFormTemplate().JUMP;
    }

    // 탑승시 탑승물에 등록된 모션만 체인지 가능
    if (UINT_MAX > GetEmbarkUID())
    {
        bool bReturn = true;
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark)
        {
            std::vector<int> vecEmbarkMotion = SiKGCEmbarkManager()->GetCharMotionList();
            std::vector<int>::iterator vecIter = vecEmbarkMotion.begin();
            for (; vecIter != vecEmbarkMotion.end(); ++vecIter)
            {
                if ((*vecIter) == iMotion)
                {
                    bReturn = false;
                    break;
                }
            }
        }
        if (bReturn)
            return;
    }

    Reserve_Motion = iMotion;
    Reserve_IsRight = bIsRight;
    Next_Attack_Count = GetCurFormTemplate().iNextAttackCount;
    Next_Motion = -1;
    Next_Direction = -1;

    ClearMotionParticleList();

    m_bDrawSlashRight = false;
    m_bDrawSlashLeft = false;
    m_bDrawSlashLegRight = false;
    m_bDrawSlashLegLeft = false;
    m_bNoCrashCheck = false;
    m_bNoRecoveryMpMotion = false;
    m_bRenderChildMeshParticle = true;
    m_bSavingMotion = false;
    m_bNoUseSkill = false;
}

// 데미지 시간조절
BOOL PLAYER::SetDamageLife(int iDamageType, int iLife)
{
    return g_MyD3D->m_pDamageManager->SetDamageLife(iDamageType, iLife);
}

void PLAYER::DeleteMagicShield()
{
    if (m_pRuneShield)
    {
        g_ParticleManager->DeleteSequence(m_pRuneShield);
        m_pRuneShield = NULL;
    }

    if (m_pRuneShield1)
    {
        g_ParticleManager->DeleteSequence(m_pRuneShield1);
        m_pRuneShield1 = NULL;
    }

    m_iShieldTime = 0;
    m_bIsCountAttackShield = false;
    m_bSpecialShield = false;
    m_fIncHPRatio -= m_fRegenRate;
    m_fRegenRate = 0.0f;
    m_iShieldLimitTime = 0;

    Shield = 0;
}

void PLAYER::SetChargedShield(bool bIsChargedShield)
{
    m_bIsChargedShield = bIsChargedShield;
}

void PLAYER::ShieldTime()
{
    if (Shield <= 0)
        return;

    if (!m_bIsCountAttackShield && !m_bSpecialShield)
        return;

    if (m_iShieldTime == m_iShieldLimitTime)
    {
        DeleteMagicShield();
    }
    m_iShieldTime++;
}

void PLAYER::ShieldCountAttack(int iPlayerIndex, int iAttacker, float fDamage, bool bPetDamage, bool bMonsterDamage)
{
    if (!g_MyD3D->MyPlayer[iPlayerIndex]->m_bIsCountAttackShield)
        return;

    if (g_MyD3D->MyPlayer[iPlayerIndex]->Shield <= 0)
        return;

    if (g_MyD3D->MyPlayer[iPlayerIndex]->m_iShieldTime == m_iShieldLimitTime)
    {
        DeleteMagicShield();
        g_MyD3D->MyPlayer[iPlayerIndex]->m_iShieldTime = 0;
        g_MyD3D->MyPlayer[iPlayerIndex]->m_bIsCountAttackShield = false;
    }

    //쉴드 카운터 일단 안쓴다고 함
    return;

    //     int randomNum = rand()%10;
    // 
    // 	if( randomNum >= 0 && randomNum < 3 )
    // 	{
    // 		float fHalfDamage;		
    // 		
    // 		if( SiKGCRoomManager()->IsMonsterGameMode() || bMonsterDamage )
    // 		{	
    // 			fHalfDamage = ASMfabs(fDamage) / 2.f;
    // 			MONSTER* pMonster = g_kMonsterManager.GetMonster(iAttacker);
    // 
    // 			if( pMonster != NULL){
    // 
    // 			fHalfDamage = fabs(fDamage) * 0.2f * pMonster->GetAttackPoint();	
    // 				pMonster->m_fHP -= fHalfDamage;
    // 
    // 				if( pMonster->m_fHP < 0.0f )
    // 					pMonster->m_fHP = 0.0f;
    // 
    // 				pMonster->SetAttackedMonster(true);
    // 			}
    //         }
    //         else
    //         {
    // #if defined ( LEVEL_DESIGN_STAT_CALC )
    //             fHalfDamage = ASMfabs(fDamage) * 0.35f * g_MyD3D->MyPlayer[iAttacker]->m_fAbility[ABILITY_ATK]*0.01f;
    // #else
    //             fHalfDamage = ASMfabs(fDamage) * 0.35f * g_MyD3D->MyPlayer[iAttacker]->m_fAbility[ABILITY_ATK];
    // #endif
    // 
    // 
    //             LTP_CHANGE_PLAYER_HP kPacket;
    //             kPacket.m_dwUserUID = g_MyD3D->MyPlayer[iAttacker]->m_kUserInfo.dwUID;
    //             kPacket.m_fChangeHP = g_MyD3D->MyPlayer[iAttacker]->GetHP() - fHalfDamage;
    //             g_LatencyTransfer.PushPacket( &kPacket );
    //             g_MyD3D->MyHead->Add_HP_Changer( g_MyD3D->MyPlayer[iAttacker]->m_iPlayerIndex, fHalfDamage, false, "",0);
    //         }
    //     }
}

void PLAYER::ReflectionDamage(int iPlayerIndex, int iAttacker, float fDamage, bool bPetDamage, bool bMonsterDamage)
{
    if (!g_MyD3D->IsPlayerIndex(iPlayerIndex) && !g_MyD3D->MyPlayer[iPlayerIndex]->m_bIsCountAttackShield)
        return;

    if (g_MyD3D->MyPlayer[iPlayerIndex]->m_fReflectionDamageRatio == 0.f)
        return;

    float fHalfDamage;
    if (SiKGCRoomManager()->IsMonsterGameMode() || bMonsterDamage)
    {
        if (g_kMonsterManager.IsMonsterIndex(iAttacker))
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(iAttacker);
            fHalfDamage = ASMfabs(fDamage) * g_MyD3D->MyPlayer[iPlayerIndex]->m_fReflectionDamageRatio;

            if (fHalfDamage > MAX_MONSTER_REFLECTION_DAMAGE)
                fHalfDamage = MAX_MONSTER_REFLECTION_DAMAGE;


            pMonster->SetDamageDiff(fHalfDamage);
            pMonster->ActionCallBack("DamageBeforeChange");
            fHalfDamage = pMonster->GetDamageDiff();
            if (pMonster != NULL) {
                pMonster->m_fHP -= fHalfDamage;

                if (pMonster->m_fHP < 0.0f)
                    pMonster->m_fHP = 0.0f;

                pMonster->SetAttackedMonster(true);
            }
        }
    }
    else
    {
        if (g_MyD3D->IsPlayerIndex(iAttacker))
        {
            if (bPetDamage)
                fHalfDamage = ASMfabs(fDamage) * g_MyD3D->MyPlayer[iPlayerIndex]->m_fReflectionDamageRatio;
            else
                fHalfDamage = ASMfabs(fDamage) * g_MyD3D->MyPlayer[iPlayerIndex]->m_fReflectionDamageRatio * g_MyD3D->MyPlayer[iAttacker]->m_fAbility[ABILITY_ATK] * 0.01f;

            LTP_CHANGE_PLAYER_HP kPacket;
            kPacket.m_dwUserUID = g_MyD3D->MyPlayer[iAttacker]->m_kUserInfo.dwUID;
            kPacket.m_fChangeHP = g_MyD3D->MyPlayer[iAttacker]->GetHP() - fHalfDamage;
            g_LatencyTransfer.PushPacket(&kPacket);
#if defined( LEVEL_DESIGN_STAT_CALC )
            g_MyD3D->MyHead->Add_HP_Changer(g_MyD3D->MyPlayer[iAttacker]->m_iPlayerIndex, fHalfDamage, false, "", 0);
            //g_MyD3D->MyHead->Add_HP_Changer(g_MyD3D->MyPlayer[iAttacker]->m_iPlayerIndex, fHalfDamage * 100.f, false, "", 0);
#else
            g_MyD3D->MyHead->Add_HP_Changer(g_MyD3D->MyPlayer[iAttacker]->m_iPlayerIndex, g_MyD3D->MyPlayer[iAttacker]->m_iPlayerIndex, fHalfDamage * 100.f, false, "", 0);
#endif
        }
    }
}

void PLAYER::AddDamageRelation(CDamageInstance* damage)
{
#if defined(RESOURCE_GRAPH)
    if (damage)
    {
        char tmp1[256];
        char tmp2[256];

        sprintf(tmp1, "DAMAGE_%d", damage->m_What);
        sprintf(tmp2, "CHARACTER_%d", (char)Extra_Char_Num);
        g_pGCDeviceManager2->AddRelation(tmp1, tmp2);
    }
#endif
}

void PLAYER::AddParticleRelation(CParticleEventSeqPTR particle)
{
#if defined(RESOURCE_GRAPH)
    if (particle)
    {
        char tmp1[256];
        char tmp2[256];
        sprintf(tmp2, "CHARACTER_%d", (char)Extra_Char_Num);
        g_pGCDeviceManager2->AddRelation("PARTICLE_" + particle->GetName(), tmp2);
    }
#endif
}

// 진 - 버닝게이지 관련 변수들 초기화
void PLAYER::InitBurnningGauge()
{
    m_fBurnningElapsedTime = 0.0f;
    m_fBurnningPoint = 0.0f;
    m_bIsMovedAfterCharge = false;

    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_SIEG_FURY);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_SIEG4_FURY);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_SIEG_FURY_DUN);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_SIEG4_FURY_DUN);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_JIN_BURNNING_MODE_EFFECT);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_MAGICSWORD_MODE);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN2_SPEAR_MODE);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN3_SHIELD_MODE);
    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN4_BURNING_MODE);
}

// 진 전용 - 버닝게이지 업데이트
void PLAYER::UpdateBP()
{
    // 버닝모드일 때 진 주위에 붙어다니는 이펙트 생성		
    DrawBurnningModeEffect();

    if (!IsLocalPlayer())
        return;

    //버닝게이지를 모을 때의 사운드를 정지시킨다.( 진 전직 공용 )
    //if( GetCurBurnningTemplate().BURNNING_CHARGE != uiMotion )
    //	g_KDSound.Stop("1160");		

    if ( IsSpecialSkillMotion() || IsSwapSpecialSkillMotion() )
    {
        m_fBurnningElapsedTime = 0.0f;
        return;
    }

    float fDecreaseTime = 3.0f / (GetCurFormTemplate().fBPDecreaseTime * float(GC_FPS_LIMIT));

    if (!m_bIsBurnningMode) //버닝모드가 아닐때의 BP 계산
    {
        // 지그하트 BP자동회복, 진은 fBPHitIncrease가 0이기 때문에 오토차징이 되지 않는다. 
        // 오토차징을 위해서는 플레이어템플릿의 fBPHitIncrease은 0이면 안된다.
        AutoChargeBurnningPoint();

        if (!g_MyD3D->MyCtrl->k_Push_Burnning && m_fBurnningPoint >= 0.0f)
        {
            m_fBurnningElapsedTime += GC_ELAPSED_TIME;

            if (m_fBurnningElapsedTime >= GetCurFormTemplate().fBPStopDelayTime)
            {
                m_fBurnningPoint -= fDecreaseTime;
                if (m_fBurnningPoint <= 0.0f)
                {
                    m_fBurnningPoint = 0.0f;
                    m_fBurnningElapsedTime = 0.0f;
                }
            }

            // x키가 눌려지고 있지 않으면 m_bIsMovedAfterCharge를 원상태로 되돌린다.
            if (!g_MyD3D->MyCtrl->k_Push_Burnning)
                m_bIsMovedAfterCharge = false;
        }
        else if (g_MyD3D->MyCtrl->k_Push_Burnning && m_fBurnningPoint == 3.0f) //버닝모드 발동
        {
            m_fBurnningElapsedTime += GC_ELAPSED_TIME;

            if (m_fBurnningElapsedTime >= 0.7f && IsContact)
            {

                //ChangeWeaponTemplate();
                if (uiMotion == GetCurFormTemplate().BURNNING_CHARGE)
                {
                    SetPlayerState(GetCurFormTemplate().BURNNING_ACTIVE);
                    cFrame = 0;
                    ChangeBurnningTemplate();
                }
            }
        }
        else
        {
            if (m_bIsMovedAfterCharge && m_fBurnningPoint > 0.0f)
            {
                m_fBurnningElapsedTime += GC_ELAPSED_TIME;

                if (m_fBurnningElapsedTime >= GetCurFormTemplate().fBPStopDelayTime)
                {
                    m_fBurnningPoint -= fDecreaseTime;
                    if (m_fBurnningPoint <= 0.0f)
                    {
                        m_fBurnningPoint = 0.0f;
                        m_fBurnningElapsedTime = 0.0f;
                    }
                }
            }
            else
                m_fBurnningElapsedTime = 0.0f;

            // x키가 눌려지고 있지 않으면 m_bIsMovedAfterCharge를 원상태로 되돌린다.
            if (!g_MyD3D->MyCtrl->k_Push_Burnning)
                m_bIsMovedAfterCharge = false;
        }
    }
    else //버닝모드 이면 BP를 감소만 시킨다.
    {
        m_fBurnningPoint -= fDecreaseTime;

        if (m_fBurnningPoint <= 0.0f)
        {
            InitBurnningGauge();
            ChangeBurnningTemplate();

            if (uiMotion == MID_SIEG1_FURY_WAIT)
            {
                SetPlayerState(MID_SIEG1_FURY_WAIT_TO_WAIT);
                cFrame = 0;
            }
        }
    }
}

void PLAYER::AutoChargeBurnningPoint()
{
    float fBP = 0.0f;
    if ( GetCurFormTemplate().fBPHitIncrease != 0.0f )
        fBP = ( 3.0f / (GetCurFormTemplate().fBPChargeTime * (float)GC_FPS_LIMIT ) ) / GetCurFormTemplate().fBPHitIncrease;

    CountBP( fBP, true );
}

// 진 - 버닝모드관련 이펙트는 여기에다 추가하자.
// 버프로 이펙트를 붙인 이유는 태그모드에서 진 몸에만 파티클을 붙이기 위해서이고,
// 진이 태그로 교체 되었어도 버닝모드일 시의 파티클은 유지 시켜줄 필요가 있어서 버프로 파티클 붙임
void PLAYER::DrawBurnningModeEffect()
{
    // 진 - 버닝모으기 이펙트
    if (uiMotion == GetCurBurnningTemplate().BURNNING_CHARGE && uiMotion != 0)
    {
        if (cFrame == 10)
        {
            float fOffsetY = 0.0f;

            for (int i = 0; i < 2; ++i)
            {
                if (m_pBurnningCharge[i] == NULL || g_ParticleManager->IsLiveInstance(m_pBurnningCharge[i]) == false)
                {
                    switch (Extra_Char_Num)
                    {
                    case CID_JIN1:
                    case CID_JIN4:
                    {
                        fOffsetY = -0.05f;
                    }
                    break;
                    case CID_JIN2:
                    {
                        fOffsetY = -0.03f;
                    }
                    break;
                    case CID_JIN3:
                    {
                        if (m_iWeaponType == 0)
                            fOffsetY = -0.03f;
                        else
                            fOffsetY = -0.05f;
                    }
                    break;
                    }

                    char tmp1[20];
                    sprintf(tmp1, "Jin1_Gage_0%d", i + 1);
                    m_pBurnningCharge[i] = g_ParticleManager->CreateSequence(tmp1, 0, 0, 0.0f);

                    TRACE_INFO stTrace;
                    if (m_pBurnningCharge[i]) {
                        stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
                        stTrace.m_fYOffset = fOffsetY;
                        stTrace.m_fParticleTraceTime = 5.0f;
                        m_pBurnningCharge[i]->SetTrace(&stTrace);

                    }

                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 2; ++i)
        {
            if (m_pBurnningCharge[i])
            {
                g_ParticleManager->DeleteSequence(m_pBurnningCharge[i]);
                m_pBurnningCharge[i] = NULL;
            }
        }
    }

    // 필살기 사용중에는 버닝게이지가 감소 않기 때문에 버닝이펙트도 정지시켜준다.
    if ((GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_FURY_MODE || GetCurFormTemplate().iSubAbilityType == SUB_ABILITY_BURNNING_MODE) &&
        uiMotion != GetCurBurnningTemplate().sTagTemplate.m_uiAtkMotion && uiMotion != GetCurBurnningTemplate().sTagTemplate.m_uiStartMotion &&
        uiMotion != GetCurBurnningTemplate().sTagTemplate.m_uiEndMotion)
    {
        if (IsSpecialSkillMotion() || IsSwapSpecialSkillMotion())
        {
            HiddenBuffParticle(EGC_JIN_BURNNING_MODE_EFFECT, true, false);
            HiddenBuffParticle(EGC_SIEG_FURY, true, true);
            HiddenBuffParticle(EGC_SIEG4_FURY, true, true);
            HiddenBuffParticle(EGC_SIEG_FURY_DUN, true, true);
            HiddenBuffParticle(EGC_SIEG4_FURY_DUN, true, true);
        }
        else
        {
            HiddenBuffParticle(EGC_JIN_BURNNING_MODE_EFFECT, false, false);
            HiddenBuffParticle(EGC_SIEG_FURY, false, true);
            HiddenBuffParticle(EGC_SIEG4_FURY, false, true);
            HiddenBuffParticle(EGC_SIEG_FURY_DUN, false, true);
            HiddenBuffParticle(EGC_SIEG4_FURY_DUN, false, true);
        }

    }
}

void PLAYER::CountBP(float fBP, bool bRatio, bool bForceDecrease)
{
    if (!IsLocalPlayer())
        return;

    if (m_bIsBurnningMode && bForceDecrease == false)
        return;

    if (bRatio)
    {
        m_fBurnningPoint += fBP * GetCurFormTemplate().fBPHitIncrease;
    }
    else
    {
        m_fBurnningPoint += fBP;
    }

    if (m_fBurnningPoint < 0.0f)
        m_fBurnningPoint = 0.0f;

    if (m_fBurnningPoint > 3.0f)
        m_fBurnningPoint = 3.0f;
}

void PLAYER::SetBeforeMotionPosY(void)
{
    static float fSavePos_Y = 0;
    static bool bFirstMotioin = true;
    DWORD tmpMotion = 0;

    //====================================================================================
    // 2009.02.03 : Jemitgge
    // Comment : 모션 프레임의 y값을 구해서 저장해 놓는다.
    if (uiMotion == MID_SKILL_LIRE_METEO_STRIKE_LV1_JUMP && GetNowMotionFRM()->GetNum_Frame() - 2 <= cFrame && cFrame <= GetNowMotionFRM()->GetNum_Frame() - 1)
    {
        fSavePos_Y = (vPos.y + (GetNowMotionFRM()->GetFrameData(cFrame).Pos_y + GetCurFormTemplate().fCharHeight) * CHAR_SCALE_RATIO) - 0.14f;
        bFirstMotioin = false;
    }

    //====================================================================================
    // 2009.02.03 : Jemitgge
    // Comment : 저장해 놓은 y값을, 그 다음 모션의 첫번째 y값으로 정한뒤, 
    //			 모든 Latency를 맞춘다.
    if (Before_Motion == MID_SKILL_LIRE_METEO_STRIKE_LV1_JUMP
        /*&& cFrame <= LATENCY + SHOWLATENCY*/ && !bFirstMotioin)
    {
        vPos.y = fSavePos_Y;
        bFirstMotioin = true;
        All_Latency_Equal();
    }
}

void PLAYER::SetNormalTpye(void)
{
    // 버닝 모드도 풀고, 스왑웨폰도 되돌려 놓는다!
    if (m_bIsBurnningMode)
    {
        ChangeBurnningTemplate();
        SetSkillType(m_iSkillType);
    }

    //InitBurnningGauge();

    if (m_iWeaponType == 1)
    {
        m_iWeaponType = !m_iWeaponType;
        ChangeWeaponTemplate();
    }

    ClearTransformPetBuffFull();
}

float PLAYER::GetY_Ex()
{
    KPlayerRenderData kRenderDataNowDrawing;
    if (IsLocalPlayer())
    {
        kRenderDataNowDrawing = g_kLocalPlayerLatency->kRenderData[0];
    }
    else
    {
        kRenderDataNowDrawing = *this;
    }

    GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion(kRenderDataNowDrawing.usMotion);

    if (!pMotion ||
        pMotion->GetNum_Frame() == 0)
        return 0.0f;

    return (pMotion->GetFrameData(kRenderDataNowDrawing.cFrame).Pos_y + GetCurFormTemplate().fCharHeight) * CHAR_SCALE_RATIO;

}

int PLAYER::GetFrameNum()
{
    return GetNowMotionFRM()->GetNum_Frame();
}

int PLAYER::GetFrameIndex()
{
    return cFrame;
}

void PLAYER::SetFrameIndex(int iFrame_)
{
    cBeforeFrame = cFrame;
    cFrame = iFrame_;
}


bool PLAYER::RenderWeapon() const
{
    if (!m_pObject)
        return false;
    return m_pObject->GetMeshRender("WEAPON");
}

void PLAYER::RenderWeapon(bool val)
{
    if (!m_pObject)
        return;
    m_pObject->ToggleExtraMesh("WEAPON", val);
    m_pObject->ToggleExtraMesh("WEAPON_0", val);
    m_pObject->ToggleExtraMesh("WEAPON_1", val);
}

bool PLAYER::RenderMergeWeapon() const
{
    if (!m_pObject)
        return false;
    return m_pObject->GetMeshRender("MERGE_WEAPON");
}

void PLAYER::RenderMergeWeapon(bool bRenderMergeWeapon)
{
    if (!m_pObject)
        return;
    m_pObject->ToggleExtraMesh("MERGE_WEAPON", bRenderMergeWeapon);
}

void PLAYER::StartAfterImage()
{
    if (m_pObject)
    {
        g_MyD3D->m_kAfterImageRenderer.RegisterObject(m_pObject);
    }
}

void PLAYER::EndAfterImage()
{
    if (m_pObject)
    {
        if (g_MyD3D->m_kAfterImageRenderer.IsRegisterObject(m_pObject))
            g_MyD3D->m_kAfterImageRenderer.UnRegisterObject(m_pObject);
    }
}


void PLAYER::SetUISuperPoint(float fSuper)
{
    if (IsLocalPlayer())
        g_MyD3D->MyHead->SuperPoint = fSuper;
}

void PLAYER::DrainHPMP(const int iDrainType/*= 0*/)
{
    // 속성 능력치 적용!%
    // HP, MP 흡수~
    // iDrainType 
    // 1 == HP만 흡수
    // 2 == MP만 흡수 
    // 0 == HP MP  흡수 
    float fDrainHPPow = m_fAbility[ABILITY_HP_DRAIN_RATE];
    float fDrainMPPow = m_fAbility[ABILITY_MP_DRAIN_RATE];

    static DWORD dwLastDrainHP = 0;
    static DWORD dwLastDrainMP = 0;
    int iLevel = this->m_kUserInfo.GetCurrentChar().iLevel;

    if (timeGetTime() - dwLastDrainHP > 1000 * 5/*초*/ && fDrainHPPow > 0.0f && iDrainType != 2)
    {
        // 캐릭터 HP 흡수량 = HP흡수력 / (40 + ChrLv * ChrLv) * 0.05 * MaxHP
        float fDrainHP = fDrainHPPow / (40 + iLevel * iLevel) * 0.05f * this->m_fAbility[ABILITY_HP];

        // 0.01 이 1이네 -_-; 1은 채워주쟈 
#ifdef LEVEL_DESIGN_STAT_CALC
        if (fDrainHP < 1.f)
            fDrainHP = 1.f;
#else
        if (fDrainHP < 0.01f)
            fDrainHP = 0.01f;
#endif
        else if (fDrainHP > this->m_fAbility[ABILITY_HP] * 0.1f)
            fDrainHP = this->m_fAbility[ABILITY_HP] * 0.1f;

        //         DamageInfo damage;
        //         damage.SetDamage( fDrainHP );
        //         damage.SetAttacker( DamageInfo::DAMAGE_PLAYER, m_iPlayerIndex );
        //         damage.SetAbilityEffect( true, false );
        //         damage.SetIgnoreDefence( true );
        //         damage.SetDamageFactor( false );
        //         Change_HP( damage );

        SetHP(GetHP() + fDrainHP);

        dwLastDrainHP = timeGetTime();
    }

    if (timeGetTime() - dwLastDrainMP > 1000 * 3/*초*/ && fDrainMPPow > 0.0f && iDrainType != 1)
    {
        // 캐릭터 MP 흡수량 = MP흡수력 / (40 + ChrLv * ChrLv) * 0.5
        float fDrainMP = fDrainMPPow / (40 + iLevel * iLevel) * 0.5f;

        if (fDrainMP > 1.0f)
            fDrainMP = 1.0f;

        float fMPBuffRatio = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE];
        g_MyD3D->ChargeLocalPlayerMana(fDrainMP, fMPBuffRatio);

        dwLastDrainMP = timeGetTime();
    }
}

bool PLAYER::IsToggleExtraMesh(const char* strID)
{
    return KGCObj::IsToggleExtraMesh(strID);
}

void PLAYER::ToggleMeshAll(bool bToggle_)
{
    if (false == m_bMeshToggleState)
        KGCObj::ToggleMeshAll(bToggle_);
    else
        KGCObj::ToggleMeshAll(false);
}

void PLAYER::ToggleExtraMesh(const char* strID, bool bToggle_, int iShaderTech)
{
    if (false == m_bMeshToggleState)
        KGCObj::ToggleExtraMesh(strID, bToggle_, iShaderTech);
    else
        KGCObj::ToggleExtraMesh(strID, false, iShaderTech);
}

void PLAYER::ToggleExtraMeshAll(bool bToggle_)
{
    if (false == m_bMeshToggleState)
        KGCObj::ToggleExtraMeshAll(bToggle_);
    else
        KGCObj::ToggleExtraMeshAll(false);
}

void PLAYER::ToggleBaseMesh(bool bToggle_)
{
    if (false == m_bMeshToggleState) {
        KGCObj::ToggleBaseMesh(bToggle_);
        m_bIsRenderBaseMesh = bToggle_;
    }
    else {
        KGCObj::ToggleBaseMesh(false);
        m_bIsRenderBaseMesh = false;
    }
}

void PLAYER::ToggleBaseMeshStatic(bool bToggle_)
{
    ToggleBaseMesh(bToggle_);
    m_bIsRenderBaseMeshStatic = !bToggle_;
}

void PLAYER::ToggleSecondBaseMesh(const char* strID, bool bToggle_)
{
    KGCObj::ToggleSecondBaseMesh(strID, bToggle_);
}

void PLAYER::ToggleSecondBaseMeshAll(bool bToggle_)
{
    KGCObj::ToggleSecondBaseMeshAll(bToggle_);
}

void PLAYER::RenderOffExtraAllWeapon()
{
    ToggleExtraMesh("WEAPON", false);
    ToggleExtraMesh("WEAPON_0", false);
    ToggleExtraMesh("WEAPON_1", false);
    ToggleExtraMesh("MERGE_WEAPON", false);
    ToggleExtraMesh("GRANDARK_2ND", false);
    ToggleExtraMesh("GRANDARK_2ND_1", false);
    ToggleExtraMesh("GRANDARK_2ND_2", false);
    ToggleExtraMesh("DOG_GRANDARK", false);
    ToggleExtraMesh("EDEL_PISTOL", false);
}

void PLAYER::SetChildMeshMotion(char* strChlid, char* strMotion, bool bReserveNext, bool bForce, bool bResetFrame/* = false*/, bool bNotOneTime/* = false*/, int iLoopMode/* = 0*/)
{
    return KGCObj::SetChildMeshMotion(strChlid, strMotion, bReserveNext, bForce, bResetFrame, bNotOneTime, iLoopMode);
}

void PLAYER::SetChildMeshMotion_Front(char* strChlid, char* strMotion)
{
    return KGCObj::SetChildMeshMotion_Front(strChlid, strMotion);
}

void PLAYER::SetChildMeshFrame(char* strChlid, char* strMotion, int iFrame)
{
    KGCObj::SetChildMeshFrame(strChlid, strMotion, iFrame);
}

void PLAYER::SetChildMeshCurrentFrame(char* strChlid, int iFrame)
{
    KGCObj::SetChildMeshCurrentFrame(strChlid, iFrame);
}

int PLAYER::GetChildMeshCurrentFrame(char* strChlid)
{
    return KGCObj::GetChildMeshCurrentFrame(strChlid);
}

const char* PLAYER::GetChildMeshCurrentMotionName(char* strChild)
{
    return KGCObj::GetChildMeshCurrentMotionName(strChild);
}

void PLAYER::ClearChildMeshOneTimeMotion(char* strChlid)
{
    return KGCObj::ClearChildMeshOneTimeMotion(strChlid);
}
void PLAYER::InitHittingGauge()
{
    m_fHittingPoint = 0.0f;
    m_bIsFullHittingState = false;
    m_bHGDecreaseState = true;

}

void PLAYER::CountHittingPoint(float fHittingPoint)
{
    if (!IsLocalPlayer())
        return;

    m_fHittingPoint += fHittingPoint;

    if (m_fHittingPoint <= 0.0f)
    {
        m_fHittingPoint = 0.0f;
        m_bIsFullHittingState = false;
    }
    else if (m_fHittingPoint >= 1.0f)
    {
        m_fHittingPoint = 1.0f;
        m_bIsFullHittingState = true;
        m_bHGDecreaseState = true;
    }
}

void PLAYER::UpdateHittingPoint()
{
    if (!IsLocalPlayer())
        return;

    if ((m_bHGDecreaseState || m_bIsFullHittingState) && m_fHittingPoint != 0.0f)
    {
        float fDecreaseFrame = GC_FPS_LIMIT * (GetCurFormTemplate().fHGPDecreaseTime / (1.0f / GetCurFormTemplate().fHGPCumulativeNum));
        CountHittingPoint(-(GetCurFormTemplate().fHGPCumulativeNum / fDecreaseFrame));
    }
}

void PLAYER::SendBurnningPoint(int iPlayerIndex)
{
    if (iPlayerIndex != g_MyD3D->Get_MyPlayer())
        return;

    KGC_PID_BURNNING_POINT kPacket;
    kPacket.dwUID = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.dwUID;
    kPacket.fBP = g_MyD3D->MyPlayer[iPlayerIndex]->GetBurnningPoint();

    SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
    KGCPC_MEMO("KGC_PID_BURNNING_POINT");
}

float PLAYER::GetMP()
{
    return g_MyD3D->MyHead->Level;
}

float PLAYER::GetHP() const
{
    return m_fHP;
}

void PLAYER::SetHP(float fHP_)
{
    // 죽어라 케도 땅에 떨어지기전에 HP 회복되면 살아난다.ㅠ
    // 죽으면 false가 되어서 풀린다.
    if (m_bDirectKill)
        return;

    // 이 함수에다가 방어력, 버프 이런거 집어넣으면 혼납니다.
    // 그런 용도라면 Change_HP 함수를 쓰세요.
    // 이 함수는 값을 셋팅할 때 보장되어야 하는 부분만 추가합니다.

    // HP변화에 따른 탑승물 콜백호출 및 탑승물HP 변경
    if (UINT_MAX > GetEmbarkUID() && IsLocalPlayer())
    {
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark)
        {
            if ((static_cast<int>(m_fHP) > static_cast<int>(fHP_)) &&
                (static_cast<int>(fHP_) != m_fAbility[ABILITY_HP]))
            {
                pEmbark->SetHP(pEmbark->GetHP() + (fHP_ - m_fHP), true);
                pEmbark->CallLuaFunc_Callback(KGCEmbarkTemplate::CALLBACK_CHANGE_CHAR_HP);
            }

            if ((0.0f < m_fHP) && (fHP_ <= 0.0f))
                pEmbark->CallLuaFunc_Callback(KGCEmbarkTemplate::CALLBACK_EMBARKATOR_DIE);
        }
    }

    // 달성과제 피격체크
#if defined(LEVEL_DESIGN_STAT_CALC)
    if ((static_cast<int>(m_fHP) > static_cast<int>(fHP_)) &&
        (static_cast<int>(fHP_) != static_cast<int>(m_fAbility[ABILITY_HP])))
#else
    if ((m_fHP > fHP_) &&
        (fHP_ != m_fAbility[ABILITY_HP]))
#endif
    {
        SiKGCSubjectManager()->AddDamageHitCnt();
        SiKGCSubjectManager()->AddDamageVolume((m_fHP - fHP_));

        //미션조건용 피격횟수
        if (vPos.y >= -4.f) // 플랫폼맵에서 떨었졌을 때는 제외
            AddControlInfoBeAttackedCount();
    }

    m_fHP = fHP_;
    if (m_fHP > m_fAbility[ABILITY_HP]) {
        SetHPFull();
    }
    else if (m_fHP < 0.f) {
        m_fHP = 0.f;
    }
#if defined(NATION_BRAZIL)
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        DangerEffect();
#else
    DangerEffect();
#endif


}

void PLAYER::SetHPFull()
{
    SetHP(m_fAbility[ABILITY_HP]);
}

void PLAYER::SetAttack(float fAtk_)
{
    m_fAbility[ABILITY_ATK] = fAtk_;
}


void PLAYER::SetLastKillmePlayer(unsigned char val)
{
    Last_Killme_Player = val;
}

void PLAYER::SetLastMademeGravityPlayer(unsigned char val)
{
    Last_MademeGravity_Player = val;
}

void PLAYER::ComboBreak()
{
    if (g_MyD3D->MyHead->ShowCombo)
        SiKGCAchieveManager()->OccurAction(SAchieve::COMBO_BREAK, GetCharType());

    SiKGCAchieveManager()->OccurAction(SAchieve::DAMAGED, GetCharType());

    g_pkQuestManager->SetComboKillPoint(true);
    g_pkQuestManager->ComboBreakCheck(true);
    g_pGameBoard->SetComboBreak(true);
    g_MyD3D->MyHead->ComboClear();

    // 던젼에서 콤보 관련 점수 계산 위한 패킷 전송
    if (g_MyD3D->GetMyPlayer()->GetcomboState() && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
    {
        g_MyD3D->GetMyPlayer()->SetComboState(false);

        if (g_kGlobalValue.m_kUserInfo.bHost == false)
        {
            KGC_PID_QUEST_RANK_COMBO_STATE kPacket;
            kPacket.dwUID = g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID;
            kPacket.bIsComboState = false;
            kPacket.bAttacked = true;

            SendP2PPacket(&kPacket, sizeof(kPacket), SiKGCRoomManager()->GetHostUID(), _RELIABLE);
            KGCPC_MEMO("KGC_PID_QUEST_RANK_COMBO_STATE");
        }
        else
            g_pkQuestManager->CountComboBreak(); //감점을 해주시겠다!
    }
}

void PLAYER::DamageInfo::SetDamageInstanceAttacker(const CDamageInstance* damage_)
{
    if (damage_->m_IsMonsterDamage) {
        SetAttacker(PLAYER::DamageInfo::DAMAGE_MONSTER, damage_->m_ExtraMonsterNum);
    }
    else if (damage_->m_bPetDamage) {
        SetAttacker(PLAYER::DamageInfo::DAMAGE_PET, damage_->m_Who);
    }
    else {
        SetAttacker(PLAYER::DamageInfo::DAMAGE_PLAYER, damage_->m_Who);
    }
}

void PLAYER::DamageInfo::SetDamageInstance(const CDamageInstance* damage_)
{
    SetDamageInstanceAttacker(damage_);
    m_iDamageKind = damage_->m_What;
    m_dwDamageAtt = damage_->m_pOriginalDamage->m_dwDMAttribute;
    m_bSpecialAttack = damage_->m_pOriginalDamage->m_bSpecialAttack;
    m_nSpecialGrade = damage_->m_pOriginalDamage->m_nSpecialGrade;
}

bool PLAYER::IsSpecialUsable(bool bForceUse_, bool bNoCheckContact_)
{
    if (!IsContact && !bNoCheckContact_)
        return false;

    if (IsCatchMotion(uiMotion))
        return false;

    if (IsMagicEffect(EGC_SIEG1_SKILL_CASH_SPECIAL3_BUFF, true))
        return false;

    if (!g_pMagicEffect->IsSpecialUsable(m_iPlayerIndex))
    {
        return false;
    }


    if (uiMotion == GetJokeMotion() ||
        uiMotion == GetOwnMotion(MID_DIO4_DEVIL_JOKE) ||      // 스크립트에는 데빌만 있어서 둘다...
        uiMotion == GetOwnMotion(MID_DIO4_WEAPON_JOKE) ||
        uiMotion == GetOwnMotion(MID_COMMON_INTO_TELEPORT) ||
        CheckInPortal() ||
        uiMotion == GetCurFormTemplate().START ||
        uiMotion == GetCurFormTemplate().WIN ||
        uiMotion == GetCurFormTemplate().RESULT_LOSE ||
        uiMotion == GetCurFormTemplate().RESULT_WIN ||
        uiMotion == GetOwnMotion(MID_AMY_WAIT_EXTRAMOTION) ||
        uiMotion == GetOwnMotion(MID_AMY_WAIT_EXTRAMOTION_LEFT)
        ) {
        return false;
    }

    if (bForceUse_) {
        if (IsNoAllowForceUseMotion()) {
            return false;
        }
    }
    else {
        if (IsDamagedMotion()) {
            return false;
        }
    }

    if (m_bNoUseSpecial) {
        return false;
    }

    if (uiMotion == GetCurFormTemplate().SAVING_STANCE ||
        uiMotion == GetCurFormTemplate().SAVING_ATTACK ||
        uiMotion == GetCurFormTemplate().SAVING_EVASION) {
        return false;
    }

    return true;
}

void PLAYER::UseSkill(GCHotKeySkill* skill, bool bCheckUsable, int iComboIdx)
{
    if (m_bNoUseSkill) {
        return;
    }

    if (uiMotion == GetCurFormTemplate().SAVING_STANCE ||
        uiMotion == GetCurFormTemplate().SAVING_ATTACK ||
        uiMotion == GetCurFormTemplate().SAVING_EVASION ||
        uiMotion == GetCurFormTemplate().START ||
        uiMotion == MID_DIO4_WEAPON_START ||
        uiMotion == MID_RIN2_EVIL_START) {
        return;
    }

    // 무기 뺏기고 카운터 가능하게 하려면 어쩔 수 없네...
    if (IsMagicEffect(EGC_EFFECT_EDEL1_UNIQUE_SKILL1_STEAL_WEAPON)) {
        return;
    }

    EGCGameModeCategory eCategory = SiKGCRoomManager()->GetGameModeCategory();
    if (eCategory == GC_GMC_AGIT)
        return;


    // 4필 사용을 막는다. ( 평준화 x ) 
    if (SiKGCRoomManager()->GetRoomInfo().m_bDisableSP4SkillsPvP)
    {
        int iSkillID = GetCurFormTemplate().SID_SPECIAL4;
        if (skill && iSkillID == skill->m_eSkill)
        {
            g_pkChatManager->AddChatMsg(g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_NOT_USE_4SPECIAL));
            g_MyD3D->MyCtrl->k_FreshSkillNum = 0;
            return;
        }
        iSkillID = GetCurFormTemplate().SID_SPECIAL4_1;
        if (skill && iSkillID == skill->m_eSkill)
        {
            g_pkChatManager->AddChatMsg(g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_NOT_USE_4SPECIAL));
            g_MyD3D->MyCtrl->k_FreshSkillNum = 0;
            return;
        }
    }

    // 사용할 스킬 정보 얻어오기
    int iStartComboKeyCoolTime = 0;
    int iEndComboKeyCoolTime = 0;
    GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, skill->m_eSkill)->m_pHotKeySkill;

    bool bForceUseSkill = false;
    bool bNoCheckContact = false;
    if (info) {
        // 현재 모션이 필살기이면 필살기만 단축키를 이용해서 중복 사용할 수 있다.
        // 필살기 사용 중 단축키 특수기 사용 못하게 하기 위함이다.
        if (IsSpecialSkillMotion(uiMotion) && IsSpecialSkillMotion(info->m_iMotion) == false) {
            return;
        }

        // 페이탈 시 사용불가 옵션이 있고 페이탈 상태이면 스킬 사용 못한다.
        if (info->m_bNoUseFatal && IsFatal()) {
            return;
        }

        bForceUseSkill = info->m_bForceUseSkill;

        if (info->m_kPossibleUseJump.m_bPrecedingSID == -1) {
            bNoCheckContact = info->m_kPossibleUseJump.m_bPossible;
        }
        else {
            if (info->m_kPossibleUseJump.m_bPossible && CheckSkill(info->m_kPossibleUseJump.m_bPrecedingSID)) {
                bNoCheckContact = true;
            }
            else {
                bNoCheckContact = false;
            }
        }

        if (info->m_iCheckCoolTimeBuff != -1 && IsMagicEffect(info->m_iCheckCoolTimeBuff)) {
            return;
        }

        // 필살기 모션 체크 옵션이 true고 필살기 모션이면 스킬 쓰지 말도록 하자.
        if (info->m_bCheckSpecialMotion) {
            if (IsSpecialSkillMotion()) {
                return;
            }
        }
    }

    if (bCheckUsable && !IsSpecialUsable(bForceUseSkill, bNoCheckContact))
        return;

    if (m_bUseNotAP)
        return;


    if (info && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL) {
        // 진짜 각성 들가면 이 부분 버린다!!
        std::map< std::pair< int, int >, std::vector< SCashSkillInfo > >::iterator mitCashSkill;
        mitCashSkill = m_mapCashSkillInfo.find(std::make_pair(Extra_Char_Num % GC_CHAR_NUM, Extra_Char_Num / GC_CHAR_NUM));
        if (mitCashSkill != m_mapCashSkillInfo.end()) {
            for (int i = 0; i < static_cast<int>(mitCashSkill->second.size()); i++) {
                const GCSkill* pSkill = SiGCSKT()->GetSkill((EGCSkillTree)mitCashSkill->second[i].m_iSkillID);
                if (pSkill && pSkill->m_pHotKeySkill) {
                    if (pSkill->m_pHotKeySkill->m_iMotion == info->m_iMotion) {
                        int iValidCheck = -1;
                        GetSpecialMotionID(info->m_iChargingSkillLV, &iValidCheck);
                        if (iValidCheck < 1) {
                            return;
                        }
                    }
                }
            }
        }
    }

    // 오오라 스킬 적용
    if (info && (info->m_vecAuraBuff.empty() == false || info->m_vecAuraDeBuff.empty() == false)) {
        InitRangeInfo();
        SetRangeCheckSize(info->m_fAuraSizeWidth, info->m_fAuraSizeHeight);

        for (std::vector< int >::iterator vit = info->m_vecAuraBuff.begin(); vit != info->m_vecAuraBuff.end(); ++vit) {
            SetRangeBuffEffect(*vit, false, true);
        }
        for (std::vector< int >::iterator vit = info->m_vecAuraDeBuff.begin(); vit != info->m_vecAuraDeBuff.end(); ++vit) {
            SetRangeBuffEffect(*vit, true, true);
        }
        return;
    }

    // 쿨 타임 시 모션이 있다면 키 입력이 들어 올 때 해당 모션으로 모션 체이지 시켜주자.
    // 연속 사용을 막기 위해서 해당 모션을 취하고 있을 때 무시 시키도록 하자.
    if (skill->fCoolTime > 0.0f &&
        uiMotion != info->m_iMotionWhenCooltime &&
        uiMotion != info->m_iMotion &&
        info->m_iMotionWhenCooltime != -1 &&
        g_MyD3D->MyHead->Level >= info->m_fManaMotionWhenCooltime) {
        if (SetPlayerState(info->m_iMotionWhenCooltime)) {
            g_MyD3D->MyHead->Level -= info->m_fManaMotionWhenCooltime * m_fMPDecreaseRatio;
            Reserve_Motion = 0;
            Next_Attack_Count = 0;
            Old_Frame = cFrame = 0;
            return;
        }
    }

    // 오오라 스킬에 쿨타임이 돌고있다면 다른 오오라로 교체 못하게 해주자.
    if (skill->m_eSkill == SID_LIME_NORMAL_AURA_OFFENCE ||
        skill->m_eSkill == SID_LIME_NORMAL_AURA_DEFENSE ||
        skill->m_eSkill == SID_LIME_NORMAL_AURA_ASSISTANT ||
        skill->m_eSkill == SID_LIME2_SELF_AURA_ATTACK ||
        skill->m_eSkill == SID_LIME2_SELF_AURA_MOVE ||
        skill->m_eSkill == SID_LIME2_SELF_AURA_DEFFENSE) {
        for (std::vector< GCHotKeySkill >::iterator vitLime = m_vecHotKeySkill.begin(); vitLime != m_vecHotKeySkill.end(); ++vitLime) {
            if (vitLime->m_eSkill == SID_LIME_NORMAL_AURA_OFFENCE ||
                vitLime->m_eSkill == SID_LIME_NORMAL_AURA_DEFENSE ||
                vitLime->m_eSkill == SID_LIME_NORMAL_AURA_ASSISTANT ||
                vitLime->m_eSkill == SID_LIME2_SELF_AURA_ATTACK ||
                vitLime->m_eSkill == SID_LIME2_SELF_AURA_MOVE ||
                vitLime->m_eSkill == SID_LIME2_SELF_AURA_DEFFENSE) {
                if (vitLime->fCoolTime > 0.5f) {
                    return;
                }
            }
        }
    }

    if (skill->iStartKeyTime == 0 &&
        skill->iEndKeyTime == 0 &&
        skill->fCoolTime > 0.0f)
        return;

    // 콤보 정보 얻어오기
    if (0 <= iComboIdx && iComboIdx < int(info->m_vecComboSkill.size()))
    {
        iStartComboKeyCoolTime = info->m_vecComboSkill[iComboIdx].m_iStartKeyCoolTime;
        iEndComboKeyCoolTime = info->m_vecComboSkill[iComboIdx].m_iEndKeyCoolTime;
        info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, EGCSkillTree(info->m_vecComboSkill[iComboIdx].m_iSID))->m_pHotKeySkill;
    }

    float fMana = info->m_fMana;

#if defined( MATCH_BALANCE_2011_8 )
    if (IsMonsterGameMode())
    {
        if (0.0f < info->m_fMonsterModeMana)
            fMana = info->m_fMonsterModeMana;
    }
#endif

    if (fMana > g_MyD3D->MyHead->Level)
    {
        return;
    }

    // 버닝 체크해서 조건에 만족되면 버닝포인트 깎아주고, 모션도 바꿔주자.
    int iMotion = info->m_iMotion;
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
        iMotion = info->m_iMonsterModeMotion;
    }
    if (GetCurFormTemplate().BURNNING_ACTIVE != -1 && GetCurFormTemplate().BURNNING_CHARGE != -1) {
        if (m_fBurnningPoint >= GetCurFormTemplate().BurningCostForSpecial || m_bIsBurnningMode) {
            iMotion = info->m_iAddMotion;
        }

        if (m_bIsBurnningMode == false) {
            m_fBurnningPoint -= GetCurFormTemplate().BurningCostForSpecial;

            if (m_fBurnningPoint < 0.0f) {
                m_fBurnningPoint = 0.0f;
            }
        }

    }
    else if (GetCurFormTemplate().BURNNING_ACTIVE != -1 && GetCurFormTemplate().BURNNING_CHARGE == -1) {
        if (m_bIsBurnningMode) {
            iMotion = info->m_iAddMotion;
        }
    }

    if (uiMotion == GetOwnMotion(MID_SKILL_ARME1_B1_BLOODPEST))
        return;

    if (SetPlayerState(iMotion) ||
        info->m_iBuffIndex != -1) {
        if (info->m_iBuffIndex == -1) {
            Reserve_Motion = 0;
            Next_Attack_Count = 0;
            Old_Frame = cFrame = 0;
        }

        float fCoolTime = info->m_fCoolTime;
#if defined( MATCH_BALANCE_2011_8 )
        if (SiKGCRoomManager()->IsMonsterGameMode())
        {
            if (0.0f < info->m_fMonsterModeCoolTime)
                fCoolTime = info->m_fMonsterModeCoolTime;
        }
#endif               

        //////////// 시작 - 성장 스킬 마나, 쿨타임, hp 소모 계산 ////////////////
        // 4필은 제외 시키자.
        if (info->m_iChargingSkillLV < 4) {
            fMana = SiGCGrowingSkillManager()->CountSkillUseMana(this, fMana);
            fCoolTime = SiGCGrowingSkillManager()->CountSkillCoolTime(this, fCoolTime);
            SiGCGrowingSkillManager()->CountSkillUseHP(this);

            // 이 스킬은 어떻게 할 수가 없다 조건이 너무 하드하다...
            if (info->m_bSpecialSkill == false && CheckSkill(SID_BEIGAS1_GROWING_SKILL_B_TYPE1)) {
                std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
                SiGCGrowingSkillManager()->GetGrowingSkillInfo(GetCurrentChar().iCharType, vecGrowingSkillInfo);
                for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
                    if (SID_BEIGAS1_GROWING_SKILL_B_TYPE1 == vit->m_iSkillID) {
                        fCoolTime += (fCoolTime * vit->m_fUniqueCoolTime);

                        for (std::vector< GCHotKeySkill >::iterator vitHotkey = m_vecHotKeySkill.begin(); vitHotkey != m_vecHotKeySkill.end(); ++vitHotkey) {
                            GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, vitHotkey->m_eSkill)->m_pHotKeySkill;
                            if (info && info->m_bSpecialSkill && vitHotkey->fCoolTime > 0.f) {
                                vitHotkey->fCoolTime += (vitHotkey->fCoolTime * vit->m_fSpecialCoolTime);
                                if (vitHotkey->fCoolTime < 0.f) {
                                    vitHotkey->fCoolTime = 0.f;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
        //////////// 끝 - 성장 스킬 마나, 쿨타임, hp 소모 계산 //////////////////

 /*       if (fCoolTime > 0.0f)
        {
            //  최소 1.0초 미만으로 내려가지 않게 한다.
            if (fCoolTime - m_fMinusAPCoolTime < 1.f) {
                skill->fCoolTime = min(fCoolTime, 1.0f);
            }
            else {
                skill->fCoolTime = fCoolTime - m_fMinusAPCoolTime;
            }
        }*/

        if (fCoolTime > 0.0f)
        {
            if (info->m_iChargingSkillLV == 4)
            {
                skill->fCoolTime = fCoolTime;
            }
            else if (fCoolTime - m_fMinusAPCoolTime < 1.f)
            {
                skill->fCoolTime = min(fCoolTime, 1.0f);
            }
            else
            {
                skill->fCoolTime = fCoolTime - m_fMinusAPCoolTime;
            }
        }

        if (info->m_bSpecialSkill) {
            g_MyD3D->MyHead->Level -= (fMana * m_fMPDecreaseRatio * m_fMPDecreaseRatioForSpecial);
            g_kGlobalValue.m_prHybridSpecialUseCount.first++; // 하이브리드 캐릭터 단축키로 필살기 사용 카운트 저장

            // 진짜 각성 들가면 빠져야 하는 부분
            if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
            {
                int iSkillIndex = info->m_iChargingSkillLV;
                if (g_MyD3D->MyHead->IsBlackMagic == false) {
                    iSkillIndex += 3;
                }

                if (m_iSkillType == 1 && m_mapwSkillItem.count(std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, (DWORD)iSkillIndex)))//수량성 기술을 사용한것임
                {
                    g_pItemMgr->DecDurationItem(m_mapwSkillItem[std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, iSkillIndex)]);
                }
            }
        }
        else {
            g_MyD3D->MyHead->Level -= (fMana * m_fMPDecreaseRatio);
        }

        g_MyD3D->MyCtrl->k_FreshSkillNum = 0;

        skill->iStartKeyTime = iStartComboKeyCoolTime;
        skill->iEndKeyTime = iEndComboKeyCoolTime;
        m_iComboSkillNowComboIdx = iComboIdx;
        SiKGCSubjectManager()->AddSpecial();

        //미션에서 체크해야하는 스킬이 있는지 확인
        std::map< int, int >::iterator mit = m_mapControlInfoSkill.find(skill->m_eSkill);
        if (mit != m_mapControlInfoSkill.end())
            mit->second += 1;

        if (g_kGlobalValue.IsCharUseSpecial4(Extra_Char_Num % GC_CHAR_NUM)) {
            SetSkillCoolTime(skill);
        }

        if (info->m_iBuffIndex != -1) {
            SetMagicEffect(info->m_iBuffIndex, info->m_fBuffTime);
        }
    }

    // 화령의 축복 버프 기간이라면 회복버프
    if (SiGCBuffDonationManager()->IsBuffOn())
        SiGCBuffDonationManager()->FullHPMP();
}

bool PLAYER::AppointUseSkill(EGCSkillTree eSkillID, bool bCheckUsable)
{
    if (g_MyD3D->MyCtrl->k_FreshSkillNum && g_MyD3D->MyCtrl->k_FreshSkillNum <= (int)m_vecHotKeySkill.size())
    {
        GCHotKeySkill HotKeySkill = m_vecHotKeySkill[g_MyD3D->MyCtrl->k_FreshSkillNum - 1];
        if ((eSkillID == HotKeySkill.m_eSkill) && (m_vecHotKeySkill[g_MyD3D->MyCtrl->k_FreshSkillNum - 1].fCoolTime <= 0))
        {
            UseSkill(&m_vecHotKeySkill[g_MyD3D->MyCtrl->k_FreshSkillNum - 1], bCheckUsable);
            return true;
        }
    }
    return false;
}

void PLAYER::ClearDeSableSkill()
{
    m_vecDesableSkillMotion.clear();
}

void PLAYER::RegDeSableSkill(int iMotionID)
{
    m_vecDesableSkillMotion.push_back(iMotionID);
}

bool PLAYER::UseSkillNotCheck()
{
    EGCGameModeCategory eCategory = SiKGCRoomManager()->GetGameModeCategory();
    if (eCategory == GC_GMC_AGIT)
        return false;

    if (m_bUseNotAP || m_bNoUseSkill)
        return false;

    int iSkillNum = g_MyD3D->MyCtrl->k_FreshSkillNum - 1;
    if (0 > iSkillNum || iSkillNum >= int(m_vecHotKeySkill.size()))
        return false;

    GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill(GetCurrentChar().iCharType, m_vecHotKeySkill[iSkillNum].m_eSkill)->m_pHotKeySkill;
    std::vector<int>::iterator vecIter = m_vecDesableSkillMotion.begin();
    for (; vecIter != m_vecDesableSkillMotion.end(); ++vecIter)
    {
        if (info->m_iMotion == *vecIter)
            return false;
    }

    UseSkill(&m_vecHotKeySkill[iSkillNum], false);
    g_MyD3D->MyCtrl->k_Use_PetSkill = false;
    g_MyD3D->MyCtrl->k_Fresh_Joke = false;
    return true;
}

bool PLAYER::IsDamagedMotion(int iMotion_ /*= -1 */)
{
    if (iMotion_ == -1)
        iMotion_ = uiMotion;

    if (iMotion_ == GetOwnMotion(MID_COMMON_BIGDAMAGE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_BY_FLYATK) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_BY_GUARD) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_BY_GUARD_ENABLE_COUNTER) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_MID_SPECIAL) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_SPIKE_TRAP) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_UPPER_SPECIAL) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DOWN_IN_SKY) ||
        iMotion_ == GetOwnMotion(MID_COMMON_ENT_TRUNK) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FREEZED_BY_ENT) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FREEZED_BY_LICH) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FREEZED_BY_PIXIE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) ||
        iMotion_ == GetOwnMotion(MID_COMMON_HOLD) ||
        iMotion_ == GetOwnMotion(MID_COMMON_HOLD_NOEFFECT) ||
        iMotion_ == GetOwnMotion(MID_COMMON_HOLD_UNTIL_ATTACKED) ||
        iMotion_ == GetOwnMotion(MID_COMMON_PARALYSE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_PIERCING_DAMAGED) ||
        iMotion_ == GetOwnMotion(MID_COMMON_SMALLDAMAGE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_STONECURSE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_STONECURSE_BY_MEDUSA) ||
        iMotion_ == GetOwnMotion(MID_COMMON_STUN) ||
        iMotion_ == GetOwnMotion(MID_DAMAGED_BY_HELL_MOUTH) ||
        iMotion_ == GetOwnMotion(MID_SKILL_ARME1_ICESTRIKE_FREEZING) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DIO_SPECIAL4_HITTED) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_COUNTER_SPIKE_TRAP) ||
        iMotion_ == GetOwnMotion(MID_COMMON_AMY1_SPECIAL4_HITTED) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FRONT_FALL_DOWN) ||
        iMotion_ == GetOwnMotion(MID_RIN3_COMMON_GODDESS_UNIQUE_SKILL_1) ||
        iMotion_ == GetOwnMotion(MID_RIN4_COMMON_SPEICLA2_HOLD) ||
        iMotion_ == GetOwnMotion(MID_COMMON_ZERO3_OS_UNIQUE_HOLD) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_DIE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DIO_SPECIAL4_HITTED) ||
        iMotion_ == GetOwnMotion(MID_NEPHILIM_DIO_SPECIAL4_HITTED) ||
        iMotion_ == GetOwnMotion(MID_MDAN_DIO_SPECIAL4_HITTED) ||
        iMotion_ == GetOwnMotion(MID_ZERO4_COMMON_DAMAGED_BLACKHALL) ||
        iMotion_ == GetOwnMotion(MID_COMMON_LEY3_GAROUILLE_ICE_SLIDING) ||
        iMotion_ == GetOwnMotion(MID_COMMON_LEY3_HAUNT_CURSE4_DAMAGE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_DIO3_TENACIOUS_HOLD))
        return true;
    return false;

}

bool PLAYER::IsNoAllowForceUseMotion(int iMotion_)
{
    if (iMotion_ == -1)
        iMotion_ = uiMotion;

    if (iMotion_ == GetOwnMotion(MID_COMMON_FRONT_FALL_DOWN) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_BY_FLYATK) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_BY_GUARD) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_MID_SPECIAL) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_SPIKE_TRAP) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DAMAGED_UPPER_SPECIAL) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) ||
        iMotion_ == GetOwnMotion(MID_COMMON_DOWN_IN_SKY) ||
        iMotion_ == GetOwnMotion(MID_COMMON_ENT_TRUNK) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FREEZED_BY_ENT) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FREEZED_BY_LICH) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FREEZED_BY_PIXIE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) ||
        iMotion_ == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER) ||
        iMotion_ == GetOwnMotion(MID_COMMON_HOLD) ||
        iMotion_ == GetOwnMotion(MID_COMMON_HOLD_NOEFFECT) ||
        iMotion_ == GetOwnMotion(MID_COMMON_HOLD_UNTIL_ATTACKED) ||
        iMotion_ == GetOwnMotion(MID_COMMON_PARALYSE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_PIERCING_DAMAGED) ||
        iMotion_ == GetOwnMotion(MID_COMMON_STONECURSE) ||
        iMotion_ == GetOwnMotion(MID_COMMON_STONECURSE_BY_MEDUSA) ||
        iMotion_ == GetOwnMotion(MID_COMMON_STUN) ||
        iMotion_ == GetOwnMotion(MID_DAMAGED_BY_HELL_MOUTH) ||
        iMotion_ == GetOwnMotion(MID_SKILL_ARME1_ICESTRIKE_FREEZING))
        return true;
    return false;
}

void PLAYER::OnEarthQuake(float fRange, float fDamage, float fPosX, float fPosY, float fHeight /*= 0.0f */, bool bSpecialDamage /*= false*/)
{
#define DISTANCE( x1, y1, x2, y2 ) ASMsqrt( pow( ( x1 - x2 ), 2 ) + pow( ( y1 - y2 ), 2 ) )
    if (fHeight == 0.0f)
        fHeight = 0.06f;
    D3DXVECTOR2 vQuakePoint = vPos;
    if (GetIsRight())
    {
        vQuakePoint.x += fPosX;
    }
    else
        vQuakePoint.x -= fPosX;

    vQuakePoint.y += fPosY;

    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i) == false)
        {
            float fDist = DISTANCE(g_MyD3D->MyPlayer[i]->vPos.x, g_MyD3D->MyPlayer[i]->vPos.y, vQuakePoint.x, vQuakePoint.y);

            // 범위 안의 녀석만 적용된다!
            if (fDist > fRange)
                continue;
            {
                g_MyD3D->MyPlayer[i]->ReverseGravity(m_iPlayerIndex, fHeight, fDamage, true, bSpecialDamage);
                CheckChargeMarblePhysicAtk();
            }
        }
    }

    if (g_kGlobalValue.m_kUserInfo.bHost) {
        fDamage *= 500;
        // 몬스터도 리버스그라비티!!
        for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
            MONSTER* pMonster = mit->second;

            if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
                continue;

            float fDist = DISTANCE(pMonster->GetX(), pMonster->GetY(), vQuakePoint.x, vQuakePoint.y);

            // 범위 안의 녀석만 적용된다!
            if (fDist > fRange)
                continue;

            if (pMonster->IsLive())
            {
                if (pMonster->GetIsContact())
                {
                    // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
                    pMonster->m_vDamageStartPos = D3DXVECTOR2(vPos.x, vPos.y);
                    pMonster->m_vDamageWhoPos = D3DXVECTOR2(vPos.x, vPos.y);
                    pMonster->OnDamage(m_iPlayerIndex, DT_REVERSE_GRAVITY, fDamage, ATTACKTYPE_DOWN, ATTACKDIR_UP, 0.0f, fHeight, true, 0, true);
                    pMonster->SetHitPos(D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()));
                    CheckChargeMarblePhysicAtk();
                }
            }

        }
    }
    else {
        for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
            MONSTER* pMonster = mit->second;
            float fDist = DISTANCE(pMonster->GetX(), pMonster->GetY(), vQuakePoint.x, vQuakePoint.y);

            // 범위 안의 녀석만 적용된다!
            if (fDist > fRange)
                continue;

            if (pMonster->IsLive()) {
                if (pMonster->GetIsContact()) {
                    CheckChargeMarblePhysicAtk();
                }
            }
        }
    }
#undef DISTANCE
}

void PLAYER::SetStartMana()
{
    float fStartMana = 0.0f;
#ifdef MATCH_BALLENCE_121217
    if (CheckSkill(SID_DIO2_AP_START_LV4) || CheckSkill(SID_LEY01_START_AP_LV4))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            fStartMana = 1.2f;
        }
        else {
            fStartMana = 0.8f;
        }
    }
    else if (CheckSkill(SID_DIO2_AP_START_LV5) || CheckSkill(SID_LEY01_START_AP_LV5))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            fStartMana = 1.5f;
        }
        else {
            fStartMana = 1.0f;
        }
    }
    else if (CheckSkill(SID_DIO1_AP_START_LV1) || CheckSkill(SID_LEY01_START_AP_LV1))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            fStartMana = 0.3f;
        }
        else {
            fStartMana = 0.2f;
        }
    }
    else if (CheckSkill(SID_DIO1_AP_START_LV2) || CheckSkill(SID_LEY01_START_AP_LV2))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            fStartMana = 0.6f;
        }
        else {
            fStartMana = 0.4f;
        }
    }
    else if (CheckSkill(SID_DIO1_AP_START_LV3) || CheckSkill(SID_LEY01_START_AP_LV3))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            fStartMana = 0.9f;
        }
        else {
            fStartMana = 0.6f;
        }
    }
#else
    if (CheckSkill(SID_DIO2_AP_START_LV4) || CheckSkill(SID_LEY01_START_AP_LV4))
    {
        fStartMana = 1.2f;
    }
    else if (CheckSkill(SID_DIO2_AP_START_LV5) || CheckSkill(SID_LEY01_START_AP_LV5))
    {
        fStartMana = 1.5f;
    }
    else if (CheckSkill(SID_DIO1_AP_START_LV1) || CheckSkill(SID_LEY01_START_AP_LV1))
    {
        fStartMana = 0.3f;
    }
    else if (CheckSkill(SID_DIO1_AP_START_LV2) || CheckSkill(SID_LEY01_START_AP_LV2))
    {
        fStartMana = 0.6f;
    }
    else if (CheckSkill(SID_DIO1_AP_START_LV3) || CheckSkill(SID_LEY01_START_AP_LV3))
    {
        fStartMana = 0.9f;
    }
#endif

    if (GetCurrentChar().iCharType == GC_CHAR_ARME)
        fStartMana = 1.5f;

    g_MyD3D->MyHead->Level = fStartMana;
    g_MyD3D->MyCtrl->Energy = 0.0f;
}

bool PLAYER::IsLiveCharacter()
{
    if (!IsLive())
    {
        return false;
    }
    if (m_kUserInfo.nUserState != GC_RUS_PLAYING)
    {
        return false;
    }
    if (m_cLife < 1)
    {
        return false;
    }

    if (m_fHP <= 0.0f && uiMotion == GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
        return false;

    return true;
}
void PLAYER::HideNDisAbleKey(bool bToggle_)
{

}

void PLAYER::SetIncMP(float fIncMP)
{
    if (!IsLocalPlayer())
    {
        return;
    }

    g_MyD3D->m_fIncMP = fIncMP;

#ifdef MATCH_BALLENCE_121217

    if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO2_AP_SPEED_LV5))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            g_MyD3D->m_fIncMP *= 1.5f;
        }
        else {
            g_MyD3D->m_fIncMP *= 1.15f;
        }
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO2_AP_SPEED_LV4) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_4))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            g_MyD3D->m_fIncMP *= 1.4f;
        }
        else {
            g_MyD3D->m_fIncMP *= 1.12f;
        }
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO2_AP_SPEED_LV3) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LEY02_AP_UP_LV3) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_3))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            g_MyD3D->m_fIncMP *= 1.3f;
        }
        else {
            g_MyD3D->m_fIncMP *= 1.09f;
        }
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO1_AP_SPEED_LV2) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LEY02_AP_UP_LV2) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_2))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            g_MyD3D->m_fIncMP *= 1.2f;
        }
        else {
            g_MyD3D->m_fIncMP *= 1.06f;
        }
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO1_AP_SPEED_LV1) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LEY02_AP_UP_LV1) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_1))
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
            g_MyD3D->m_fIncMP *= 1.1f;
        }
        else {
            g_MyD3D->m_fIncMP *= 1.03f;
        }
    }
#else
    if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO2_AP_SPEED_LV5))
    {
        g_MyD3D->m_fIncMP *= 1.5f;
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO2_AP_SPEED_LV4) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_4))
    {
        g_MyD3D->m_fIncMP *= 1.4f;
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO2_AP_SPEED_LV3) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LEY02_AP_UP_LV3) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_3))
    {
        g_MyD3D->m_fIncMP *= 1.3f;
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO1_AP_SPEED_LV2) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LEY02_AP_UP_LV2) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_2))
    {
        g_MyD3D->m_fIncMP *= 1.2f;
    }
    else if (g_MyD3D->GetMyPlayer()->CheckSkill(SID_DIO1_AP_SPEED_LV1) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LEY02_AP_UP_LV1) || g_MyD3D->GetMyPlayer()->CheckSkill(SID_LUPUS_MP_1))
    {
        g_MyD3D->m_fIncMP *= 1.1f;
    }
#endif

}

// 2010, 12, 20, shmhlove : 움직이는 아이템
void PLAYER::EgoItemInitialize()
{
    if (NULL == m_pObject)
        return;

    m_pObjectEgo.clear();
    KItemIDVector vecEquiped = m_kUserInfo.GetCurrentChar().GetEquipmentForLoadModel(m_iPlayerIndex);
    BOOST_FOREACH(GCITEMID & itemID, vecEquiped) {
        std::list<KGCObjectEgo*> listEgo = SiKGCObjectEgoMgr()->GetObjectEgo(itemID);
        if (listEgo.empty() == true)
            continue;

        BOOST_FOREACH(KGCObjectEgo * pEgo, listEgo) {
            pEgo->AddMesh(m_pObject, itemID, m_iPlayerIndex, &m_pObjectEgo);
        }
    }
}

void PLAYER::EgoItemFrameMove(USHORT ucFrame)
{
    PROFILE_SET("PLAYER::EgoItemFrameMove");

    if (m_pObjectEgo.empty()) {
        return;
    }

    int iIndex = 0;
    const KGCObjectEgoState* state = NULL;
    for (KGCObjectAndMotion* pChild = NULL; pChild = m_pObject->GetChildObject(iIndex); iIndex++)
    {
        std::vector< KGCEgoItemInfo >::iterator vit = m_pObjectEgo.begin();
        for (; vit != m_pObjectEgo.end(); ++vit)
        {
            // UID컨버팅필요(플레이어ID, ItemID, 모델ID)
            if (vit->m_strUniqueID == pChild->m_strName)
                break;
        }
        if (vit == m_pObjectEgo.end())
            continue;

        pChild->ToggleMeshAll(m_bRender);

        KGCObjectEgo* pEgo = vit->m_pEgoObj;
        if (pEgo == NULL) {
            ASSERT(!"List 안에서 Null이 됨 AddMesh를 디버깅해보셈~");
            continue;
        }

        std::string strCurState = SiKGCObjectEgoMgr()->GetStateInfo(pChild->m_strName);
        if (strCurState.empty())
            continue;

        state = NULL;
        int iConditionIdx = -1;
        if (IsLocalPlayer() || g_MyD3D->m_pStateMachine->GetState() != GS_GAME)
        {
            state = pEgo->GetEgoState(strCurState, this, ucFrame, iConditionIdx);
        }
        else if (vit->m_bCheckCondition)
        {
            vit->m_bCheckCondition = false;
            state = pEgo->GetEgoStateNotCheckCondition(vit->m_iConditionID);
        }

        if (state == NULL) {
            if (pChild->IsEmpty()) {
                state = pEgo->GetEgoStateFromID(strCurState);
                if (state == NULL)
                    continue;

                state = pEgo->GetEgoStateFromID(state->m_strAfterState);
                if (state == NULL)
                    continue;
            }
            else
                continue;
        }

        if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME &&
            0 <= iConditionIdx)
        {
            KGC_PID_BROAD_EGOITEM_STATE kPacket;
            kPacket.dwUID = m_kUserInfo.dwUID;
            kPacket.iPlayerID = m_iPlayerIndex;
            kPacket.dwItemID = pChild->dwGoodsID;
            kPacket.iModelID = vit->m_dwUModelniqueID;
            kPacket.iConditionID = iConditionIdx;

            SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE);
            KGCPC_MEMO("KGC_PID_BROAD_EGOITEM_STATE");
        }

        // 모션변경
        SiKGCObjectEgoMgr()->AddStateInfo(pChild->m_strName, state->m_strID);
        pChild->ClearMotion();
        if (state->m_iRepeat <= 0) {
            pChild->AddMotion(state->m_strMotion, false);
        }
        else {
            for (int i = 0; i < state->m_iRepeat; ++i) {
                pChild->AddMotion(state->m_strMotion, true);
            }
        }

        // 모션이 바뀔때 해야할 일들...
        if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME) {

            // 데미지 생성
            if (0 <= state->m_iDamageID) {
                AddDamage(state->m_iDamageID, 0);
            }

            // 사운드 재생
            if (0 <= state->m_iSoundID) {
                Sound(state->m_iSoundID, 0);
            }
        }
    }
}

void PLAYER::SetEgoItemOrderPlayer(int iPlayerID, DWORD dwItemID, int iModelID, int iConditionID)
{
    if (!IsLocalPlayer())
    {
        std::vector< KGCEgoItemInfo >::iterator vecIter = m_pObjectEgo.begin();
        for (; vecIter != m_pObjectEgo.end(); ++vecIter)
        {
            if (vecIter->m_dwUModelniqueID == iModelID)
            {
                vecIter->m_bCheckCondition = true;
                vecIter->m_iConditionID = iConditionID;
                return;
            }
        }
    }
}

void PLAYER::ShowExpEffect(int iGhostIndex, float fExp_)
{
    PLAYER* pKiller = this;
    PLAYER* pGhost = g_MyD3D->MyPlayer[iGhostIndex];

    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->Add(DT_EXP_EFFECT, m_iPlayerIndex, false, -1, NULL, true);

    if (pkDamage == NULL)
        return;

    float fOffset_X = 0.0f;
    float fOffset_Y = 0.0f;
    float fKillerPos_X = pKiller->GetPos()->x;
    float fKillerPos_Y = pKiller->GetPos()->y;
    float fGhostPos_X = pGhost->GetPos()->x;
    float fGhostPos_Y = pGhost->GetPos()->y;

    D3DXVECTOR2 vecPos = g_pkGameOverlayUI->GetHeadUp()->m_pkExpBar->m_pkExpPercent->GetFixedWindowPos();
    D3DXVECTOR3& CameraPos = D3DXVECTOR3(g_kCamera.m_fLookX + 1.0f, g_kCamera.m_fLookY + 0.75f, 0.0f);
    // 카메라가 볼수 있는 영역 알아내자. 줌이 적용되어 있다.
    float fWidth = g_kCamera.m_rtCamera.right - g_kCamera.m_rtCamera.left;//2.0f;//800.0f;
    float fHeight = g_kCamera.m_rtCamera.top - g_kCamera.m_rtCamera.bottom;//1.5f;//600.0f;
    // cw = 카메라가 카메라 영역에서 얼마나 움직였는가
    float cwx = CameraPos.x - fWidth * 0.5f;
    float cwy = CameraPos.y - fHeight * 0.5f;

    float fRealPosX = cwx + (vecPos.x / 400.0f);
    float fRealPosY = cwy + (1.5f - vecPos.y / 400.0f);

    //m_iLastAttackedMe
    pkDamage->m_pOriginalDamage->SetMileStonePos(fGhostPos_X, fGhostPos_Y);   // 입사각~ 야호 
    pkDamage->m_pOriginalDamage->SetMileStonePos(fGhostPos_X - randf() / 15.0f, fGhostPos_Y + randf() / 15.0f);   // 시작 
    pkDamage->m_pOriginalDamage->SetMileStonePos(fGhostPos_X, fGhostPos_Y - randf() / 15.0f);   // 시작 머뭇..머뭇..
    pkDamage->m_pOriginalDamage->SetMileStonePos(fRealPosX, fRealPosY);   // 마지막
    pkDamage->m_pOriginalDamage->SetMileStonePos(fRealPosX, fRealPosY);   // 마지막


    // 이딴데 담아도 될까?
    pkDamage->m_pOriginalDamage->m_fDamageToPlayer = fExp_;
}

// 2010, 2, 15, shmhlove : 가까운 거리에 있는 적 위치 찾기
D3DXVECTOR2* PLAYER::GetTargetPos(int iTargetNum, float fLimitDist, bool bCheckBoss_ /* = false */, bool bCheckRight_ /* = false */, bool bCheckLeft_ /* = false */, float fLimitRangeY_)
{
    struct DistData
    {
        float fDist;
        D3DXVECTOR2 vPos;
        DistData(float fDist_, D3DXVECTOR2 vPos_) : fDist(fDist_), vPos(vPos_) {}
    };
#define DISTANCE( x1, y1, x2, y2 ) ASMsqrt( pow( ( x1 - x2 ), 2 ) + pow( ( y1 - y2 ), 2 ) )

    // 데이터 초기화
    std::list<DistData> listMinDist;
    memset(m_vTargetPos, 0, sizeof(D3DXVECTOR2) * MAX_TARGET_DAMAGE);

    // 예외처리
    if (iTargetNum > MAX_TARGET_DAMAGE)    iTargetNum = MAX_TARGET_DAMAGE;
    if (iTargetNum < 0)                    return NULL;

    // 플레이어 거리파악
    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if (pPlayer->m_kUserInfo.bLive == false)
            continue;
        if (pPlayer->m_cLife == 0)
            continue;
        if (pPlayer->Item_Remain_Time[GC_GAME_ITEM_HALLOW])
            continue;
        if (pPlayer->CheckHiding())
            continue;
        if (pPlayer->m_ePlayerState == PS_DEAD)
            continue;
        if (pPlayer->uiMotion == pPlayer->GetCurFormTemplate().START)
            continue;
        if (pPlayer->m_bNotTargeting == true)
            continue;
        if (g_MyD3D->IsSameTeam(i, m_iPlayerIndex))
            continue;

        if (bCheckRight_ && pPlayer->vPos.x < vPos.x) {
            continue;
        }

        if (bCheckLeft_ && pPlayer->vPos.x > vPos.x) {
            continue;
        }

        if (fLimitRangeY_ > 0.f) {
            if (vPos.y + fLimitRangeY_ < pPlayer->vPos.y || pPlayer->vPos.y < vPos.y - fLimitRangeY_) {
                continue;
            }
        }

        // 거리구하기
        float fDist = DISTANCE(pPlayer->vPos.x, pPlayer->vPos.y, vPos.x, vPos.y);

        // 제한거리 검사
        if (fLimitDist && (fDist > fLimitDist))
            continue;

        // 삽입될 지점찾기
        std::list<DistData>::iterator listIter = listMinDist.begin();
        for (; listIter != listMinDist.end(); ++listIter)
        {
            if (fDist < listIter->fDist)
                break;
        }
        listMinDist.insert(listIter, DistData(fDist, D3DXVECTOR2(pPlayer->vPos.x, pPlayer->vPos.y)));
    }

    // 몬스터 거리파악
    for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        MONSTER* pMonster = mit->second;
        if (!pMonster->IsLive())
            continue;

        if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
            continue;

        if (bCheckBoss_) {
            if (MONSTER::MON_BOSS != pMonster->m_iBossMonster && NPC_NONE != pMonster->m_eNPC) {
                continue;
            }
        }
        else {
            if (NPC_NONE != pMonster->m_eNPC)
                continue;
        }

        GCCollisionRect<float> boundBox = pMonster->GetAttackBoundRect(0);
        D3DXVECTOR2 dxvPos(0.0f, 0.0f);

        dxvPos.x = boundBox.m_Left + ((boundBox.m_Right - boundBox.m_Left) / 2.0f);
        dxvPos.y = boundBox.m_Bottom;

        dxvPos.x += 1.0f;
        dxvPos.y += 0.25f;

        if (bCheckRight_ && dxvPos.x < vPos.x) {
            continue;
        }

        if (bCheckLeft_ && dxvPos.x > vPos.x) {
            continue;
        }

        if (fLimitRangeY_ > 0.f) {
            if (vPos.y + fLimitRangeY_ < dxvPos.y || dxvPos.y < vPos.y - fLimitRangeY_) {
                continue;
            }
        }

        // 거리구하기
        //float fDist = DISTANCE( pMonster->m_afX[ LATENCY_INDEX ], pMonster->m_afY[ LATENCY_INDEX ], vPos.x, vPos.y );
        float fDist = DISTANCE(dxvPos.x, dxvPos.y, vPos.x, vPos.y);

        // 제한거리 검사
        if (fLimitDist && (fDist > fLimitDist))
            continue;

        // 삽입될 지점찾기
        std::list<DistData>::iterator listIter = listMinDist.begin();
        for (; listIter != listMinDist.end(); ++listIter)
        {
            if (fDist < listIter->fDist)
                break;
        }

        //listMinDist.insert(listIter, DistData(fDist, D3DXVECTOR2(pMonster->m_afX[ LATENCY_INDEX ], pMonster->m_afY[ LATENCY_INDEX ])));
        listMinDist.insert(listIter, DistData(fDist, dxvPos));

    }

    //위치 값 복사
    std::list<DistData>::iterator listIter = listMinDist.begin();
    for (int iLoop = 0; (iLoop < iTargetNum && (listIter != listMinDist.end())); ++iLoop, ++listIter)
    {
        m_vTargetPos[iLoop] = listIter->vPos;
    }

    return m_vTargetPos;
}

// 2011, 3, 7, shmhlove : 가까운 거리에 발판과의 거리찾기
float PLAYER::GetPosFromNearBlock()
{
    return ASMfabs(vPos.y - g_MyD3D->MyStg->Get_LowHeight(vPos.x, vPos.y));
}

// 2011, 3, 25, shmhlove : 특정 데미지가 살아 있는제 검사
int PLAYER::GetDamageLife(int iDamageTypeiLife)
{
    int iLife = g_MyD3D->m_pDamageManager->GetDamageLife(iDamageTypeiLife);
    if (-1 == iLife)
        iLife = 0;
    return iLife;
}

BOOL PLAYER::IsDamageLife(DAMAGE_TYPE eDamageTypeiLife)
{
    return -1 < g_MyD3D->m_pDamageManager->GetDamageLife(eDamageTypeiLife);
}

void PLAYER::SetCameraZoom(float fZoom)
{
    g_kCamera.SetZoom(fZoom, g_MyD3D->Get_MyPlayer());
}

bool PLAYER::IsMonsterGameMode()
{
    return SiKGCRoomManager()->IsMonsterGameMode();
}

bool PLAYER::IsFatalGameMode()
{
    return SiKGCRoomManager()->IsFatalGameMode();
}

bool PLAYER::SetWeaponChangeItem(int iCharType, bool bChange, KEquipItemInfo itemInfo, bool bInGame)
{
    //  인게임에서 보조무기는 바뀌지 않는다.( m_mapSwapWeapon )
    if (!bInGame)
    {
        m_mapSwapWeapon[iCharType] = std::make_pair/*<bool, std::pair<GCITEMID,GCITEMUID>>*/(bChange, std::pair<GCITEMID, GCITEMUID>(itemInfo.m_dwID, itemInfo.m_dwUID));
    }

    m_mapChangeWeaponItem[iCharType] = itemInfo;
    SCharInfo sUser = m_kUserInfo.GetCurrentChar(iCharType);


    for (KUserItemVector::iterator vit = sUser.vecItemInfo.begin(); vit != sUser.vecItemInfo.end(); vit++)
    {
        GCItem* pGCItem = g_MyD3D->m_kItemMgr.GetItemData(vit->iItemID);
        if (pGCItem)
        {
            if (pGCItem->dwSlotPosition & ESP_WEAPON) {
                m_mapBaseChangeWeaponItem[iCharType] = *vit;
                m_mapBaseChangeWeaponItem[iCharType].iItemID *= 10;
                return true;
            }
        }
    }

    return false;
}

void PLAYER::ClearWeaponChengeItem()
{
    m_mapSwapWeapon.clear();
    m_mapChangeWeaponItem.clear();
}

void PLAYER::RemovePlayerObject()
{
    EndAfterImage();

    if (m_pObject == NULL) return;
    for (int i = FORM_NORMAL + 1; i < FORM_NUM; i++)
    {
        if (m_pObject == m_pObjMetaForm[i]) {
            m_pObject = NULL;
        }
    }
    if (m_pObject == m_pObjMetaForm[FORM_NORMAL])
        m_pObjMetaForm[FORM_NORMAL] = NULL;

    SAFE_REMOVE_DELETE(m_pObject);

    m_mapFaceLiftMeshObject.clear();
    m_pNonChangeFaceLift = NULL;
}

bool PLAYER::SetDirectMove(float fX, float fY, bool bNoPassPlayer, bool bNoPassMonster, float fOffsetX_, float fOffsetY_, bool bDirReverse_)
{
    bool bResult = true;

    // 충돌체크 예외처리
    if (bNoPassPlayer)
        bNoPassPlayer = !(m_bNoCrashCheck || !m_bCrashCheck || IsNoCrashCheckMotion(uiMotion));
    if (bNoPassMonster)
        bNoPassMonster = !(m_bNoCrashCheck || !m_bCrashCheck || IsNoCrashCheckMotion(uiMotion));

    // 탑승물 탑승시 지정한 모션이 아니면 강제 위치이동을 막는다.
    if (UINT_MAX > GetEmbarkUID())
    {
        bool bReturn = true;
        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
        if (pEmbark)
        {
            if (bNoPassPlayer)
                bNoPassPlayer = pEmbark->IsPushCheck();
            if (bNoPassMonster)
                bNoPassMonster = pEmbark->IsPushCheck();

            std::vector<int> vecEmbarkMotion = SiKGCEmbarkManager()->GetCharMotionList();
            std::vector<int>::iterator vecIter = vecEmbarkMotion.begin();
            for (; vecIter != vecEmbarkMotion.end(); ++vecIter)
            {
                if ((*vecIter) == uiMotion)
                {
                    bReturn = false;
                    break;
                }
            }
        }
        if (bReturn)
            return false;
    }

    // 밑으로 갈수 있는지 확인
    if (vPos.y > fY)
    {
        if (CheckStandBlock(vPos.x, vPos.y))
            fY = vPos.y;
    }
    // 위로 갈수 있는지 확인
    else if (vPos.y < fY)
    {
        float fAfterHead = fY + (y_Ex * 2.0f);
        float fBeforeHead = vPos.y + (y_Ex * 2.0f) - FOOTHOLDSIZE;
        if (SiKGCFantasticMap()->CheckPierceBlockYPos(vPos.x, fBeforeHead, fAfterHead))
            fY = fAfterHead - (y_Ex * 2.0f);
    }
    vPos.y = fY + fOffsetY_;

    if (IsLocalPlayer())
        g_kLocalPlayerLatency->kRenderData[(LATENCY + SHOWLATENCY) - 1].vPos.y = fY;

    // 스테이지 끝처리
    if (fX < 0.0f)
        fX = 0.0f;
    else if (fX > SiKGCFantasticMap()->GetPixelWidth())
    {
        GCCollisionRect<float> fRect = GetCollisionRect();
        fX = SiKGCFantasticMap()->GetPixelWidth()/* - (fRect.m_Right-fRect.m_Left)*/;
    }

    float fAfter = fX;
    float fBefore = vPos.x;

    // 플레이어 충돌 처리
    if (bNoPassPlayer)
    {
        for (int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop)
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

            // 몬스터가 살아 있는지 확인
            if (!pPlayer->IsLive())
                continue;

            if (pPlayer->m_fHP <= 0.0f)
                continue;

            // 우리팀끼리는 충돌처리 하지 않는다.
            if (g_MyD3D->IsSameTeam(m_iPlayerIndex, iLoop))
                continue;

            // 적 플레이어 키
            GCCollisionRect<float> Rect = pPlayer->GetCollisionRect();
            float fHeight = (Rect.m_Top - Rect.m_Bottom) + pPlayer->vPos.y;

            // 내 플레이어 키
            GCCollisionRect<float> MyRect = GetCollisionRect();
            float fMyHeight = (MyRect.m_Top - MyRect.m_Bottom) + vPos.y;

            // vPos와 fAfter사이에 플레이어가 있는지 확인
            if (((pPlayer->vPos.y < fMyHeight && fMyHeight < fHeight) ||
                (pPlayer->vPos.y < vPos.y && vPos.y < fHeight)) ||
                ((vPos.y < fHeight && fHeight < fMyHeight) ||
                    (vPos.y < pPlayer->vPos.y && pPlayer->vPos.y < fMyHeight)))
            {
                if (bIsRight)
                {
                    if (((vPos.x - 0.05f) <= pPlayer->vPos.x) && (pPlayer->vPos.x <= fAfter))
                    {
                        if (bDirReverse_) {
                            fAfter = pPlayer->vPos.x + (Rect.GetWidth() / 2.0f);
                        }
                        else {
                            fAfter = pPlayer->vPos.x - (Rect.GetWidth() / 2.0f);
                        }

                        bResult = false;
                        break;
                    }
                }
                else
                {
                    if (((vPos.x - 0.05f) >= pPlayer->vPos.x) && (pPlayer->vPos.x >= fAfter))
                    {
                        if (bDirReverse_) {
                            fAfter = pPlayer->vPos.x - (Rect.GetWidth() / 2.0f);
                        }
                        else {
                            fAfter = pPlayer->vPos.x + (Rect.GetWidth() / 2.0f);
                        }
                        bResult = false;
                        break;
                    }
                }
            }
        }
    }

    // 몬스터 처리( 플레이어가 절대 넘어가지 못하는 몬스터가 존재한다. )
    for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        MONSTER* pMonster = mit->second;
        // 몬스터가 살아 있는지 확인
        if (!pMonster->m_bLive)
            continue;

        if (pMonster->m_fHP <= 0.0f)
            continue;

        // 우리팀이 소환한 몬스터는 충돌체크를 하지 않는다.
        if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
            continue;

        // 몬스터 템플릿얻기
        const SMonsterTemplate& sMonsterTable = g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType];

        // 몬스터 Pass가 셋팅되있거나 함수호출시 NoPassMonster가 셋팅되었는지 확인
        if (sMonsterTable.bNoPassMonster || bNoPassMonster)
        {
            float fMonsterPosY = pMonster->GetY();
            float fMonsterHeight = pMonster->m_fAttackBoundBoxHeight + fMonsterPosY;

            // vPos.x와 fAfter사이에 몬스터가 있는지 확인
            GCCollisionRect<float> Rect = GetCollisionRect();
            float fCharacterHeight = (Rect.m_Top - Rect.m_Bottom) + vPos.y;
            if (((fMonsterPosY < fCharacterHeight && fCharacterHeight < fMonsterHeight) ||
                (fMonsterPosY < vPos.y && vPos.y < fMonsterHeight)) ||
                ((vPos.y < fMonsterHeight && fMonsterHeight < fCharacterHeight) ||
                    (vPos.y < fMonsterPosY && fMonsterPosY < fCharacterHeight)))
            {
                if (bIsRight)
                {
                    if (((vPos.x - 0.05f) < pMonster->GetX()) && (pMonster->GetX() < fAfter))
                    {
                        if (bDirReverse_) {
                            fAfter = pMonster->GetX() + (pMonster->m_fAttackBoundBoxWidth / 2.0f);
                        }
                        else {
                            fAfter = pMonster->GetX() - (pMonster->m_fAttackBoundBoxWidth / 2.0f);
                        }
                        bResult = false;
                        break;
                    }
                }
                else
                {
                    if (((vPos.x - 0.05f) > pMonster->GetX()) && (pMonster->GetX() > fAfter))
                    {
                        if (bDirReverse_) {
                            fAfter = pMonster->GetX() - (pMonster->m_fAttackBoundBoxWidth / 2.0f);
                        }
                        else {
                            fAfter = pMonster->GetX() + (pMonster->m_fAttackBoundBoxWidth / 2.0f);
                        }
                        bResult = false;
                        break;
                    }
                }
            }
        }

    }

    // 벽처리
    float fAfterWall = fAfter;

    // 여분의 오프셋처리
    if (fBefore < fAfterWall)
    {
        fBefore -= FOOTHOLDSIZE;
        fAfterWall += FOOTHOLDSIZE;
    }
    else
    {
        fBefore += FOOTHOLDSIZE;
        fAfterWall -= FOOTHOLDSIZE;
    }

    if (false == SiKGCFantasticMap()->CheckPierceBlockXPos(fBefore, fAfterWall, vPos.y))
    {
        fAfterWall = fAfter;
    }

    fAfter = fAfterWall;

    vPos.x = fAfter + fOffsetX_;
    if (IsLocalPlayer())
        g_kLocalPlayerLatency->kRenderData[(LATENCY + SHOWLATENCY) - 1].vPos.x = fAfter + fOffsetX_;

    return bResult;
}

bool PLAYER::CheckDirectMove(float fX, float fY, bool bNoPassPlayer, bool bNoPassMonster)
{
    bool bResult = true;

    // 밑으로 내려갈때 내려갈수 있는지 확인
    if (vPos.y > fY)
    {
        if (CheckStandBlock(vPos.x, vPos.y))
            fY = vPos.y;
    }

    // 스테이지 끝처리
    if (fX < 0.0f)
        fX = 0.0f;
    else if (fX > SiKGCFantasticMap()->GetPixelWidth())
    {
        GCCollisionRect<float> fRect = GetCollisionRect();
        fX = SiKGCFantasticMap()->GetPixelWidth()/* - (fRect.m_Right-fRect.m_Left)*/;
    }

    float fAfter = fX;
    float fBefore = vPos.x;

    // 플레이어 충돌 처리
    if (bNoPassPlayer)
    {
        for (int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop)
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

            // 몬스터가 살아 있는지 확인
            if (!pPlayer->IsLive())
                continue;

            if (pPlayer->m_fHP <= 0.0f)
                continue;

            // 우리팀끼리는 충돌처리 하지 않는다.
            if (g_MyD3D->IsSameTeam(m_iPlayerIndex, iLoop))
                continue;

            // 적 플레이어 키
            GCCollisionRect<float> Rect = pPlayer->GetCollisionRect();
            float fHeight = (Rect.m_Top - Rect.m_Bottom) + pPlayer->vPos.y;

            // 내 플레이어 키
            GCCollisionRect<float> MyRect = GetCollisionRect();
            float fMyHeight = (MyRect.m_Top - MyRect.m_Bottom) + vPos.y;

            // vPos와 fAfter사이에 플레이어가 있는지 확인
            if (((pPlayer->vPos.y < fMyHeight && fMyHeight < fHeight) ||
                (pPlayer->vPos.y < vPos.y && vPos.y < fHeight)) ||
                ((vPos.y < fHeight && fHeight < fMyHeight) ||
                    (vPos.y < pPlayer->vPos.y && pPlayer->vPos.y < fMyHeight)))
            {
                if (bIsRight)
                {
                    if (((vPos.x - 0.05f) <= pPlayer->vPos.x) && (pPlayer->vPos.x <= fAfter))
                    {
                        fAfter = pPlayer->vPos.x - (Rect.GetWidth() / 2.0f);
                        bResult = false;
                        break;
                    }
                }
                else
                {
                    if (((vPos.x - 0.05f) >= pPlayer->vPos.x) && (pPlayer->vPos.x >= fAfter))
                    {
                        fAfter = pPlayer->vPos.x + (Rect.GetWidth() / 2.0f);
                        bResult = false;
                        break;
                    }
                }
            }
        }
    }

    // 몬스터 처리( 플레이어가 절대 넘어가지 못하는 몬스터가 존재한다. )
    for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        MONSTER* pMonster = mit->second;

        // 몬스터가 살아 있는지 확인
        if (!pMonster->m_bLive)
            continue;

        if (pMonster->m_fHP <= 0.0f)
            continue;

        // 우리팀이 소환한 몬스터는 충돌체크를 하지 않는다.
        if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
            continue;

        // 몬스터 템플릿얻기
        const SMonsterTemplate& sMonsterTable = g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType];

        // 몬스터 Pass가 셋팅되있거나 함수호출시 NoPassMonster가 셋팅되었는지 확인
        if (sMonsterTable.bNoPassMonster || bNoPassMonster)
        {
            float fMonsterPosY = pMonster->GetY();
            float fMonsterHeight = pMonster->m_fAttackBoundBoxHeight + fMonsterPosY;

            // vPos.x와 fAfter사이에 몬스터가 있는지 확인
            GCCollisionRect<float> Rect = GetCollisionRect();
            float fCharacterHeight = (Rect.m_Top - Rect.m_Bottom) + vPos.y;
            if (((fMonsterPosY < fCharacterHeight && fCharacterHeight < fMonsterHeight) ||
                (fMonsterPosY < vPos.y && vPos.y < fMonsterHeight)) ||
                ((vPos.y < fMonsterHeight && fMonsterHeight < fCharacterHeight) ||
                    (vPos.y < fMonsterPosY && fMonsterPosY < fCharacterHeight)))
            {
                if (bIsRight)
                {
                    if (((vPos.x - 0.05f) < pMonster->GetX()) && (pMonster->GetX() < fAfter))
                    {
                        fAfter = pMonster->GetX() - (pMonster->m_fAttackBoundBoxWidth / 2.0f);
                        bResult = false;
                        break;
                    }
                }
                else
                {
                    if (((vPos.x - 0.05f) > pMonster->GetX()) && (pMonster->GetX() > fAfter))
                    {
                        fAfter = pMonster->GetX() + (pMonster->m_fAttackBoundBoxWidth / 2.0f);
                        bResult = false;
                        break;
                    }
                }
            }
        }
    }


    return bResult;
}

void PLAYER::SetDotaStage(int iState)
{
    m_iDotaStage = iState;
    if (IsLocalPlayer())
    {
        g_MyD3D->m_TempPlayer.m_iDotaStage = iState;
    }
}

bool PLAYER::DamageInfo::IsPlayerDamage() const
{
    return (GetAttackerType() == DAMAGE_PLAYER && g_MyD3D->IsPlayerIndex(GetAttacker()));
}

bool PLAYER::DamageInfo::IsMonsterDamage() const
{
    return (GetAttackerType() == DAMAGE_MONSTER && g_kMonsterManager.IsMonsterIndex(GetAttacker()));
}

int PLAYER::GetWaitMotion()
{
#ifdef DECANEE
    if (GetCurrentChar().iCharType == GC_CHAR_DECANEE && m_bCheckRunMarble)
        return MID_DECANEE1_SPECIAL_TRANSFORMATION_WAIT;
#endif
    return GetCurFormTemplate().WAIT;
}

void PLAYER::SetWaitMotion()
{
    Direct_Motion_Input(GetWaitMotion());
}

bool PLAYER::IsWaitMotion()
{
    return (GetWaitMotion() == uiMotion);
}

int PLAYER::GetFatalWaitMotion()
{
#ifdef DECANEE
    if (GetCurrentChar().iCharType == GC_CHAR_DECANEE && m_bCheckRunMarble)
        return MID_DECANEE1_SPECIAL_TRANSFORMATION_FATAL;
#endif
    return GetCurFormTemplate().FATAL_WAIT;
}

bool PLAYER::IsFatalWaitMotion()
{
    return (GetFatalWaitMotion() == uiMotion);
}

int PLAYER::GetWalkMotion()
{
    return GetCurFormTemplate().WALK;
}

void PLAYER::SetWalkMotion()
{
    Direct_Motion_Input(GetCurFormTemplate().WALK);
}

bool PLAYER::IsWalkMotion()
{
    return (GetCurFormTemplate().WALK == uiMotion);
}

bool PLAYER::IsRunMotion()
{
    return (GetCurFormTemplate().RUN == uiMotion);
}

bool PLAYER::IsDashMotion()
{
    return (GetCurFormTemplate().DASH == uiMotion);
}

bool PLAYER::IsWinMotion()
{
    return (GetCurFormTemplate().WIN == uiMotion) || (GetCurFormTemplate().RESULT_WIN == uiMotion);
}

bool PLAYER::IsLoseMotion()
{
    return (GetCurFormTemplate().RESULT_LOSE == uiMotion);
}

bool PLAYER::IsStartMotion()
{
    return (GetCurFormTemplate().START == uiMotion);
}

int PLAYER::GetJokeMotion()
{
#ifdef DECANEE
    if (GetCurrentChar().iCharType == GC_CHAR_DECANEE && m_bCheckRunMarble)
        return MID_DECANEE1_SPECIAL_TRANSFORMATION_JOKE;
#endif
    return GetCurFormTemplate().JOKE;
}

bool PLAYER::IsJokeMotion()
{
    return (GetJokeMotion() == uiMotion);
}

void PLAYER::SendAgitWateringFairyTree()
{
    KP2P::GetInstance()->Send_WateringFairyTreeReq();
}

void PLAYER::SetShowBaseEffect(bool bShow)
{
    m_bShowBaseEffect = bShow;
    ShowBaseEffect(m_bShowBaseEffect);
}

void PLAYER::SetRecoveryHpByRadio(float fRadio)
{
    if (0.0f > fRadio) fRadio = 0.0f;
    if (1.0f < fRadio) fRadio = 1.0f;

    float fHp = m_fAbility[ABILITY_HP] * fRadio;
    SetHP(GetHP() + fHp);
}

void PLAYER::CheckIcePixie()
{
    if (GetOwnMotion(MID_COMMON_FREEZED_BY_PIXIE) == Old_Motion ||
        GetOwnMotion(MID_COMMON_FREEZED_BY_ENT) == Old_Motion ||
        GetOwnMotion(MID_COMMON_FREEZED_BY_LICH) == Old_Motion ||
        GetOwnMotion(MID_SKILL_ARME1_ICESTRIKE_FREEZING) == Old_Motion)
    {
        if (GetOwnMotion(MID_COMMON_FREEZED_BY_PIXIE) == Old_Motion)
        {
            AddParticle("PIXIE_ice_tick", 0, 0.0f);
        }
        else if (GetOwnMotion(MID_COMMON_FREEZED_BY_ENT) == Old_Motion)
        {
            AddDamage(DT_ENT_TRUNK_END, 0);
        }
        else if (GetOwnMotion(MID_COMMON_FREEZED_BY_LICH) == Old_Motion)
        {
            AddParticle("tomb_hand_down_01", 0, 0.0f);
            AddParticle("tomb_hand_down_02", 0, 0.0f);
        }
        else if (GetOwnMotion(MID_SKILL_ARME1_ICESTRIKE_FREEZING) == Old_Motion)
        {
            AddParticle("PIXIE_ice_tick", 0, 0.0f);
            //AddDamageWithLocate(DT_ARME_SKILL_ICECIRCULAR_LV2_BOOM, 0.0f, 0.0f)
        }

        m_iFreezeCount = 0;
        m_fFreezeDamage = 0.0f;
        m_iPlusFreezeCount = 10;
    }
}

bool PLAYER::CheckOption(int iStateID)
{
    // 2007/2/20. milennium9. SuperArmor 상태일때에는 맞는 모션으로 가지 않음.
    // 네피림의 경우에만 사용할수도 있지만 MagicEffect로 빼기 위해 작업.
    if (m_bSuperArmor)
    {
        if (iStateID == MID_COMMON_BIGDAMAGE || iStateID == MID_COMMON_SMALLDAMAGE)
            return true;
    }

    // 하이퍼 아머일때도 슈퍼아머와 마찬가지로 모션변화 없음
    // 예외 : 현재 모션이 잡기모션이거나 지정해둔 데미지 모션일 때
    if (m_bHyperArmor && !m_bDirectKill)
    {
        if (!IsCatchMotion(uiMotion) && IsDamagedMotion(iStateID))
            return true;
    }

    return false;
}

void PLAYER::SetStatsMID(int iStateID)
{
#if defined( CHARACTER_STATS )
    std::set< int >::iterator setIter = g_kGlobalValue.m_setMIDList.find(iStateID);
    if (setIter != g_kGlobalValue.m_setMIDList.end())
    {
        SCharInfo sCharInfo = m_kUserInfo.GetCurrentChar(m_kUserInfo.cCharIndex);
        EGCGameModeCategory eGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();
        if ((-1 < sCharInfo.iCharType) && ((GC_GMC_MATCH == eGameModeCategory) || (GC_GMC_DUNGEON == eGameModeCategory)))
        {
            std::pair<int, int> prKey(iStateID, sCharInfo.iLevel);
            std::map< std::pair<int, int>, KGCGlobal::CHARACTERSTATS >::iterator mapIter = g_kGlobalValue.m_mapCharacterStats.find(prKey);
            if (mapIter == g_kGlobalValue.m_mapCharacterStats.end())
            {
                KGCGlobal::CHARACTERSTATS StatsData;
                switch (eGameModeCategory)
                {
                case GC_GMC_DUNGEON:            // 퀘스트
                    StatsData.iGameMode = 0;
                    break;
                case GC_GMC_MATCH:              // 대전
                    if (SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT)
                        StatsData.iGameMode = 2;
                    else
                        StatsData.iGameMode = 1;
                    break;
                }
                StatsData.iAccCount = 1;
                g_kGlobalValue.m_mapCharacterStats.insert(make_pair(prKey, StatsData));
            }
            else
            {
                ++mapIter->second.iAccCount;
            }
        }
    }
#endif
}

bool PLAYER::CheckNotDrawWeapon()
{
    if (IsToggleExtraMesh("MAGIC_WEAPON") ||
        IsToggleExtraMesh("MAGIC_SPEAR") ||
        IsToggleExtraMesh("MAGIC_SHIELD") ||
        IsToggleExtraMesh("MAGIC_GLOVE") ||
        IsToggleExtraMesh("UNO1_SPECIAL4_WEAPON"))
    {
        return true;
    }

    return false;
}

// 하드코딩 완전 잘한다...
void PLAYER::ChangeWeaponFromMotion()
{
    m_bRenderWeapon = GetCurFormTemplate().WeaponRenderBasically;
    m_bRenderMergeWeapon = GetCurFormTemplate().LeftWeaponRenderBasically;

    //아우 진짜 이 아래 코드들은 깝깝해서 눈물날 지경 밑에다 추가하지말고 WeaponRenderBasically 변수에 셋팅하도록
    if (CheckInPortal())
    {
        ToggleMeshAll(false);
        return;
    }
    if (CheckNotDrawWeapon())
    {
        m_bRenderWeapon = false;
    }
    else if (Extra_Char_Num == CID_LAS4)
    {
        m_bRenderWeapon = false;
        m_bRenderMergeWeapon = true;
        ShowLas4GreatSword(false);
    }
    else if (Extra_Char_Num == CID_RYAN4)
    {
        if (m_SwapTemplate.WAIT == MID_RYAN4_ONE_HAND_WAIT)
        {
            m_bRenderWeapon = true;
            m_bRenderMergeWeapon = false;
        }
        else
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = true;
        }
    }
    else if (Extra_Char_Num == CID_AMY1)
    {
        if (uiMotion == MID_AMY1_SPECIAL3
            || uiMotion == MID_AMY1_DANCE_SPECIAL1
            || uiMotion == MID_AMY1_DANCE_SPECIAL2
            || uiMotion == MID_AMY1_DANCE_SPECIAL3
            || uiMotion == MID_AMY1_DANCE_SPECIAL2_LEFT
            || uiMotion == MID_AMY1_DANCE_SPECIAL2_LEFT
            || uiMotion == MID_AMY1_DANCE_JOKE
            )
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = false;
        }
        else
        {
            m_bRenderWeapon = true;
            m_bRenderMergeWeapon = false;
        }
    }
    else if (Extra_Char_Num == CID_AMY3)
    {
        if (GetWaitMotion() == MID_AMY3_DANCE_WAIT)
        {
            m_bRenderWeapon = true;
            m_bRenderMergeWeapon = false;
        }
        else if (GetWaitMotion() == MID_AMY3_WAIT)
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = true;
        }

        if (uiMotion == MID_AMY3_SWAP_TO_DANCE_MODE)
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = true;
        }
        else if (uiMotion == MID_AMY3_SWAP_TO_FIGHT_MODE)
        {
            m_bRenderWeapon = true;
            m_bRenderMergeWeapon = false;
        }
        else if (uiMotion == MID_AMY3_SWAP_TO_DANCEMODE_WAIT || uiMotion == MID_AMY3_SWAP_TO_DANCEMODE_JUMP)
        {
            m_bRenderWeapon = true;
            m_bRenderMergeWeapon = false;
        }
        else if (uiMotion == MID_AMY3_SWAP_TO_FIGHTMODE_WAIT || uiMotion == MID_AMY3_SWAP_TO_FIGHTMODE_JUMP)
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = true;
        }
        else if (uiMotion == MID_AMY3_SPECIAL1 || uiMotion == MID_AMY3_SPECIAL2 || uiMotion == MID_AMY3_SPECIAL3 || uiMotion == MID_AMY3_SPECIAL3_LEFT)
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = true;
        }
        else if (uiMotion == MID_AMY3_DANCE_SPECIAL1 || uiMotion == MID_AMY3_DANCE_SPECIAL2 ||
            uiMotion == MID_AMY3_DANCE_SPECIAL3 || uiMotion == MID_AMY3_DANCE_SPECIAL3_LEFT)
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = false;
        }
    }
    else if (Extra_Char_Num == CID_RONAN4 && IsToggleExtraMesh("MAGIC_GLOVE"))
    {
        m_bRenderWeapon = false;
    }
    else if (Extra_Char_Num == CID_JIN3)
    {
        if (m_SwapTemplate.WAIT == MID_JIN2_WAIT)
        {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = false;
        }
        else
        {
            m_bRenderWeapon = true;
            m_bRenderMergeWeapon = false;
        }
    }
    else if (Extra_Char_Num == CID_MARI4)
    {
        std::vector<int>::iterator vecIter = m_vecCommonMotionList.begin();
        for (; vecIter != m_vecCommonMotionList.end(); ++vecIter)
        {
            if (uiMotion == *vecIter)
            {
                m_bRenderWeapon = false;
                m_bRenderMergeWeapon = true;
                SetChildMeshMotion("MERGE_WEAPON", "SoulTaker_common", true, true, false, true, EXTRA_MESH_LOOPMODE_SINGLE);
                break;
            }
        }
    }
    else if (uiMotion == MID_LIME1_WAIT || uiMotion == MID_LIME1_WALK || uiMotion == MID_LIME1_RUN) { // 반복 시키는 모션 답 없어서 하드코딩 함..... 이 ㅈㅇ.,,
        if (IsToggleExtraMesh("WEAPON")) {
            m_bRenderWeapon = true;
            m_bRenderMergeWeapon = false;
        }
        else {
            m_bRenderWeapon = false;
            m_bRenderMergeWeapon = true;
        }
    }

    if (m_pObject)
    {
        ToggleExtraMeshAll(false);
        if (m_bIsRenderBaseMeshStatic == false) {
            ToggleBaseMesh(true);
        }
        ToggleExtraMesh("WEAPON", m_bRenderWeapon);
        ToggleExtraMesh("WEAPON_0", m_bRenderWeapon);
        ToggleExtraMesh("WEAPON_1", m_bRenderWeapon);
        ToggleExtraMesh("MERGE_WEAPON", m_bRenderMergeWeapon);
        ToggleExtraMesh("GRANDARK_2ND", false);
        ToggleExtraMesh("GRANDARK_2ND_1", false);
        ToggleExtraMesh("GRANDARK_2ND_2", false);
        ToggleExtraMesh("DOG_GRANDARK", false);

        if (m_pLire3Arrow)
            m_pLire3Arrow->m_bRender = m_bRenderLire3Arrow;

        if (m_pObject->IsInvisible() == false && CheckHiding() == false)
            ShowAllItemEffect(true);
    }
}

void PLAYER::RenderEnchantWeaponEffect(int iLevel_)
{
    EGCEnchantColorGroup eColor = GetEnchnatColorGroup(GetCurrentChar().iCharType, iLevel_);

#if !defined( LOAD_ENCHANT_EFFECT_FROM_SCRIPT )
    if (eColor == ENCHANT_COLOR_GREEN)
    {
        if (iLevel_ >= 9)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV01_01", 0, 0.0f, -0.15f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV01_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV01_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 10)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV02_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV02_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV02_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 11)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV03_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV03_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 13)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV04_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV04_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 15)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV05_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV05_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Green_LV05_03", 0, 0.0f, -0.2f, 5.f);
        }
    }
    else if (eColor == ENCHANT_COLOR_VIOLET)
    {
        if (iLevel_ >= 9)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV01_01", 0, 0.0f, -0.15f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV01_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV01_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 10)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV02_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV02_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV02_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 11)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV03_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV03_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 13)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV04_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV04_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 15)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV05_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV05_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Purple_LV05_03", 0, 0.0f, -0.2f, 5.f);
        }
    }
    else if (eColor == ENCHANT_COLOR_BLUE)
    {
        if (iLevel_ >= 9)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV01_01", 0, 0.0f, -0.15f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV01_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV01_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 10)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV02_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV02_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV02_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 11)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV03_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV03_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 13)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV04_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV04_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 15)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV05_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV05_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Blue_LV05_03", 0, 0.0f, -0.2f, 5.f);
        }
    }
    else
    {
        if (iLevel_ >= 9)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV01_01", 0, 0.0f, -0.15f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV01_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV01_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 10)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV02_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV02_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV02_03", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 11)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV03_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV03_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 13)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV04_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV04_02", 0, 0.0f, -0.2f, 5.f);
        }

        if (iLevel_ >= 15)
        {
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV05_01", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV05_02", 0, 0.0f, -0.2f, 5.f);
            AddParticleNoDirectionPosWithTrace("UpgradeEF_Crimson_LV05_03", 0, 0.0f, -0.2f, 5.f);
        }
    }
#else
    if (iLevel_ < 17) {
        if (iLevel_ >= 9) {
            g_kGlobalValue.AddEnchantEffect(eColor, ENCHANT_EFFECT_LEVEL_9, this);
        }

        if (iLevel_ >= 10) {
            g_kGlobalValue.AddEnchantEffect(eColor, ENCHANT_EFFECT_LEVEL_10, this);
        }

        if (iLevel_ >= 11) {
            g_kGlobalValue.AddEnchantEffect(eColor, ENCHANT_EFFECT_LEVEL_11, this);
        }

        if (iLevel_ >= 13) {
            g_kGlobalValue.AddEnchantEffect(eColor, ENCHANT_EFFECT_LEVEL_13, this);
        }

        if (iLevel_ >= 15) {
            g_kGlobalValue.AddEnchantEffect(eColor, ENCHANT_EFFECT_LEVEL_15, this);
        }
    }
    else if (iLevel_ >= 17) {
        g_kGlobalValue.AddEnchantEffect(eColor, ENCHANT_EFFECT_LEVEL_17, this);
    }
#endif
}

EGCEnchantColorGroup PLAYER::GetEnchnatColorGroup(int iCharType, int iEnchantLevel)
{
    if (iEnchantLevel < 17) {
        switch (iCharType)
        {
        case GC_CHAR_ELESIS:
        case GC_CHAR_AMY:
        case GC_CHAR_JIN:
        case GC_CHAR_LUPUS:
        case GC_CHAR_RYAN:
        case GC_CHAR_ALICE:
            return ENCHANT_COLOR_RED;
        case GC_CHAR_LIRE:
        case GC_CHAR_ZERO:
            return ENCHANT_COLOR_GREEN;
        case GC_CHAR_ARME:
        case GC_CHAR_SIEG:
        case GC_CHAR_DIO:
        case GC_CHAR_LEY:
        case GC_CHAR_BEIGAS:
        case GC_CHAR_PERYTON:
#ifdef DECANEE
        case GC_CHAR_DECANEE:
#endif
            return ENCHANT_COLOR_VIOLET;
        case GC_CHAR_LAS:
        case GC_CHAR_RONAN:
        case GC_CHAR_MARI:
        case GC_CHAR_RIN:
        case GC_CHAR_ASIN:
        case GC_CHAR_LIME:
        case GC_CHAR_EDEL:
        case GC_CHAR_UNO:
            return ENCHANT_COLOR_BLUE;
        default:
            return ENCHANT_COLOR_RED;
        }
    }
    else {
        switch (iCharType) {
        case GC_CHAR_ELESIS:    return ENCHANT_COLOR_17_ELESIS;
        case GC_CHAR_LIRE:      return ENCHANT_COLOR_17_LIRE;
        case GC_CHAR_ARME:      return ENCHANT_COLOR_17_ARME;
        case GC_CHAR_LAS:       return ENCHANT_COLOR_17_LAS;
        case GC_CHAR_RYAN:      return ENCHANT_COLOR_17_RYAN;
        case GC_CHAR_RONAN:     return ENCHANT_COLOR_17_RONAN;
        case GC_CHAR_AMY:       return ENCHANT_COLOR_17_AMY;
        case GC_CHAR_JIN:       return ENCHANT_COLOR_17_JIN;
        case GC_CHAR_SIEG:      return ENCHANT_COLOR_17_SIEG;
        case GC_CHAR_MARI:      return ENCHANT_COLOR_17_MARI;
        case GC_CHAR_DIO:       return ENCHANT_COLOR_17_DIO;
        case GC_CHAR_ZERO:      return ENCHANT_COLOR_17_ZERO;
        case GC_CHAR_LEY:       return ENCHANT_COLOR_17_LEY;
        case GC_CHAR_LUPUS:     return ENCHANT_COLOR_17_LUPUS;
        case GC_CHAR_RIN:       return ENCHANT_COLOR_17_RIN;
        case GC_CHAR_ASIN:      return ENCHANT_COLOR_17_ASIN;
        case GC_CHAR_LIME:      return ENCHANT_COLOR_17_LIME;
        case GC_CHAR_EDEL:      return ENCHANT_COLOR_17_EDEL;
        case GC_CHAR_BEIGAS:    return ENCHANT_COLOR_17_BEIGAS;
        case GC_CHAR_UNO:       return ENCHANT_COLOR_17_UNO;
#ifdef PERYTON
        case GC_CHAR_PERYTON:	return  ENCHANT_COLOR_17_PERYTON;
#endif
#ifdef ALICE
        case GC_CHAR_ALICE:	return  ENCHANT_COLOR_17_ALICE;
#endif
#ifdef DECANEE
        case GC_CHAR_DECANEE:	return  ENCHANT_COLOR_17_DECANEE;
#endif
        default:                return ENCHANT_COLOR_17_ELESIS;
        }
    }
}

void PLAYER::FrameMove_EnchantWeaponEffect()
{
    if (uiMotion == GetCurFormTemplate().START || uiMotion == GetJokeMotion() || uiMotion == MID_DIO4_WEAPON_JOKE || uiMotion == MID_DIO4_WEAPON_START)
    {
        UCHAR cCheckFrame = 10;
        if (uiMotion == GetCurFormTemplate().START)
            cCheckFrame = 20;

        if (cFrame == cCheckFrame)
        {
            std::pair<bool, int> prEnchantWeaponInfo;

            if (IsLocalPlayer()) {
                prEnchantWeaponInfo = g_pItemMgr->GetEquipedEnchantWeaponInfo(&GetCurrentChar());
            }
            else {
                prEnchantWeaponInfo = g_pItemMgr->GetOtherUserEquipedEnchantWeaponInfo(&GetCurrentChar());
            }

            if (prEnchantWeaponInfo.first && prEnchantWeaponInfo.second >= 9)
            {
                RenderEnchantWeaponEffect(prEnchantWeaponInfo.second);
            }
        }
    }
}

void PLAYER::SendDungeonPlayState(int iState)
{
    KGC_PID_BROAD_DUNGEON_PLAYSTATE kPacket;
    kPacket.iState = iState;
    SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE);
    KGCPC_MEMO("KGC_PID_BROAD_DUNGEON_PLAYSTATE");
}

void PLAYER::ReloadResource()
{
    if (m_bIsBurnningMode)
        ChangeBurnningTemplate();

    // 전직갱신 : 모션초기화, 리소스업데이트, 플레이어템플릿변경, 이펙트초기화, 스킬트리초기화, 펫초기화, 변신 모델 리로드 
    UpdatePlayerResource();

    if (false == m_bDecChangeWeapon) {
        Calcualte_Point(!m_bDecChangeWeapon, true, false);
        m_bDecChangeWeapon = true;
    }
    else {
        Calcualte_Point(!m_bDecChangeWeapon, true, false);
    }

    g_pGraphicsHelper->LoadPlayerActionMotionByPlayerIndex(m_iPlayerIndex, Extra_Char_Num);

    //InitBonusBuff();
    ReloadBuff();
    LoadPlayerTemplate(Extra_Char_Num);
    SetPlayerState(GetWaitMotion());
    SetFrameIndex(GetFrameNum());
    UpdatePlayerResourceOnGame();
    SetSkillType(0);
    EffectClear(false);
    ClearMotionParticleList();
    EffectInit();
    RecordBonePosition();
    RenderEclipseBack();
    for (int k = 0; k < FORM_NUM; ++k)
    {
        if (m_FormTemplate[k].sMetamorphosisTemplate.iFormID != FORM_NORMAL)
            UpdateFormResource(k /*m_vecFormItemModel*/);
    }

    //LoadPet(); 의 이유가 이름때문이냐?!
    KPetInfo kPet;
    kPet = m_kUserInfo.GetCurrentChar().kPetInfo;
    if (kPet.m_dwID != 0 && kPet.m_dwUID != 0)
    {
        std::wstringstream strTemp;
        strTemp << L"LV " << kPet.m_iLevel << L". " << kPet.m_strName;
        SetPetName(strTemp.str());
    }

    LoadSlash();

    // 버프제거
    if (IsMagicEffect(EGC_EFFECT_MARI4_REPAIR))
        ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR);
    if (IsMagicEffect(EGC_EFFECT_MARI4_REPAIR_R))
        ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR_R);
}

GCITEMUID PLAYER::GetSwapWeaponUID(void)
{
    std::map<int, KEquipItemInfo>::iterator mit = m_mapChangeWeaponItem.find(GetCurrentChar().iCharType);
    if (mit != m_mapChangeWeaponItem.end()) {
        return mit->second.m_dwUID;
    }
    else {
        return 0;
    }
}

int PLAYER::GetWeaponJobLevel()
{
    //  무기가 두개일 경우는 없겠지...
    KUserItemVector& vecItemInfo = m_kUserInfo.GetCurrentChar().vecItemInfo;

    for (int i = 0; i < (int)vecItemInfo.size(); ++i)
    {
        GCItem* pItem = g_pItemMgr->GetItemData(vecItemInfo[i].iItemID);
        if (!pItem)
            continue;

        if (pItem->dwSlotPosition & ESP_WEAPON)
        {
            return pItem->iNeedChangeJobLevelForEquippingItem;
        }
    }

    return -1;
}

void PLAYER::ReloadBuff(void)
{
    int iTotalBuff = g_pMagicEffect->GetNumTotalBuff();

    for (int i = 0; i < iTotalBuff; ++i) {
        if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, i) != 0) {     // 해당 버프 걸려 있음

            if (true == g_pMagicEffect->IsCoolTime(m_iPlayerIndex, i)) { // 쿨타임이면 지나갑시다
                continue;
            }

            // 레이의 소환 스킬(버프)들을 무기체인지로 재 소환되지 않도록.
            if (i == EGC_EFFECT_LEY2_GAROUILLE

                || i == EGC_EFFECT_LEY2_MERRY_FOR_DUNGEON
                || i == EGC_EFFECT_LEY2_MERRY_HP_LV1_FOR_DUNGEON
                || i == EGC_EFFECT_LEY2_MERRY_HP_LV2_FOR_DUNGEON

                || i == EGC_EFFECT_LEY3_SUMMON_HAUNT_FOR_DUNGEON
                || i == EGC_EFFECT_LEY3_SUMMON_HAUNT_HP_UP_LV1_FOR_DUNGEON
                || i == EGC_EFFECT_LEY3_SUMMON_HAUNT_HP_UP_LV2_FOR_DUNGEON

                || i == EGC_EFFECT_LEY4_SUMMON_JEEVES_FOR_DUNGEON) {
                continue;
            }

            float fRemainTime = g_pMagicEffect->IsMagicEffectTimeRet(m_iPlayerIndex, i);
            g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, i, fRemainTime);
        }
    }
}

bool PLAYER::IsSetComboSkill()
{
    return((-1 < m_iComboSkillHotKeyIdx) && (-1 < m_iComboSkillComboIdx));
}

void PLAYER::RenderEclipseBack()
{
    if (SiGCEclipseEventManager()->IsEventTime()) {
        if (SiGCEclipseEventManager()->IsActiveEvent() && SiGCEclipseEventManager()->IsValidDungeon(SiKGCRoomManager()->GetGameMode())) {
            if (m_pEclipseBack == NULL) {
                g_pkGameOverlayUI->SetEclipseTextureColor(D3DCOLOR_ARGB(0, 0, 0, 0));
                m_pEclipseBack = g_ParticleManager->CreateSequence("event_eclipse_background", 0.f, 0.f, 0.5f);
                g_ParticleManager->SetLayer(m_pEclipseBack, GC_LAYER_UNDER_UI);
            }
        }
    }
}

void PLAYER::ChargeQuantityPoint()
{
    if (!IsLocalPlayer() ||
        GetCurFormTemplate().iSubAbilityType != SUB_ABILITY_QUANTITY_POINT ||
        (IsLearnedSkill(SID_RIN2_SELECT_EVIL) && GetCurrentChar().iPromotionLevel > 0)) {
        return;
    }

    if (m_iQuantityPoint >= GetCurFormTemplate().iMaxQP) {
        m_iQPChargeFrame = 0;
        return;
    }

    m_iQPChargeFrame++;
    if (m_iQPChargeFrame > GetCurFormTemplate().iQPChargeFrame) {
        ClearSpecialBuff(m_iQuantityPoint);
        m_iQuantityPoint++;
        SetSpecialBuff(m_iQuantityPoint);
        m_iQPChargeFrame = 0;
    }
}

void PLAYER::SetOPChargeWeight(int iWeight_)
{
    m_iQPChargeFrame += iWeight_;

    if (m_iQPChargeFrame > GetCurFormTemplate().iQPChargeFrame) {
        m_iQPChargeFrame = GetCurFormTemplate().iQPChargeFrame;
    }
}

int PLAYER::GetQuantityPoint()
{
    // 린이고 악 스킬라인 선택했으면 부적 못쓴다.
    if ((IsLearnedSkill(SID_RIN2_SELECT_EVIL) && GetCurrentChar().iPromotionLevel > 0)) {
        return 0;
    }

    return m_iQuantityPoint;
}

void PLAYER::InitQuantityPoint()
{
    if (GetCurFormTemplate().iSubAbilityType != SUB_ABILITY_QUANTITY_POINT ||
        (IsLearnedSkill(SID_RIN2_SELECT_EVIL) && GetCurrentChar().iPromotionLevel > 0)) {
        return;
    }

    m_iQPChargeFrame = 0;

    ClearSpecialBuff(m_iQuantityPoint);
    m_iQuantityPoint = GetCurFormTemplate().iMaxQP;
    SetSpecialBuff(m_iQuantityPoint);
    if (g_pkUIScene) {
        g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->m_pkSpGauge->InitQPGauge();
    }
}

void PLAYER::CountQuantityPoint(int iAmount_)
{
    if (m_iQuantityPoint == 0) {
        return;
    }

    ClearSpecialBuff(m_iQuantityPoint);

    m_iQuantityPoint += iAmount_;

    if (m_iQuantityPoint > GetMaxQuantityPoint()) {
        m_iQuantityPoint = GetMaxQuantityPoint();
    }

    SetSpecialBuff(m_iQuantityPoint);
}

int PLAYER::GetMaxQuantityPoint()
{
    int iMax = GetCurFormTemplate().iMaxQP;
    return iMax;
}

void PLAYER::SetSpecialBuff(int iIndex_, int iType_)
{
    if (iType_ >= SPECIAL_BUFF_MAX) {
        iType_ = SPECIAL_BUFF_NORMAL;
    }

    std::map< int, std::map< int, std::pair< int, float > > >::iterator mit = GetCurFormTemplate().mapSpecialBuff.find(iType_);
    if (mit != GetCurFormTemplate().mapSpecialBuff.end()) {
        std::map< int, std::pair< int, float > >::iterator mitSB = mit->second.find(iIndex_);
        if (mitSB != mit->second.end()) {
            SetMagicEffect(mitSB->second.first, mitSB->second.second);
            g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, mitSB->second.first, mitSB->second.second);
        }
    }
}

void PLAYER::ClearSpecialBuff(int iIndex_)
{
    std::map< int, std::map< int, std::pair< int, float > > >::iterator mit = GetCurFormTemplate().mapSpecialBuff.find(SPECIAL_BUFF_NORMAL);
    if (mit != GetCurFormTemplate().mapSpecialBuff.end()) {
        std::map< int, std::pair< int, float > >::iterator mitSB = mit->second.find(iIndex_);
        if (mitSB != mit->second.end()) {
            if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, mitSB->second.first)) {
                ClearMagicEffect(mitSB->second.first);
                g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, mitSB->second.first);
            }
        }
    }
}

void PLAYER::InputSpecialSkillPuzzleKey()
{
    if (!IsLocalPlayer() ||
        m_vecInputPuzzleKey.size() >= m_vecQuestionPuzzleKey.size() ||
        GetCurFormTemplate().setPuzzleKeyList.empty()) {
        return;
    }

    int iOrder = 0;
    int iKeyValue = -1;
    if (g_MyD3D->m_KGCOption.m_bEnableKeySetting) {
        if (diks[g_MyD3D->MyCtrl->m_keyUp] && !Old_diks[g_MyD3D->MyCtrl->m_keyUp]) {
            m_vecInputPuzzleKey.push_back(DIK_UP);
            iOrder = static_cast<int>(m_vecInputPuzzleKey.size()) - 1;
            iKeyValue = DIK_UP;
        }
        else if (diks[g_MyD3D->MyCtrl->m_keyDown] && !Old_diks[g_MyD3D->MyCtrl->m_keyDown]) {
            m_vecInputPuzzleKey.push_back(DIK_DOWN);
            iOrder = static_cast<int>(m_vecInputPuzzleKey.size()) - 1;
            iKeyValue = DIK_DOWN;
        }
        else if (diks[g_MyD3D->MyCtrl->m_keyLeft] && !Old_diks[g_MyD3D->MyCtrl->m_keyLeft]) {
            m_vecInputPuzzleKey.push_back(DIK_LEFT);
            iOrder = static_cast<int>(m_vecInputPuzzleKey.size()) - 1;
            iKeyValue = DIK_LEFT;
        }
        else if (diks[g_MyD3D->MyCtrl->m_keyRight] && !Old_diks[g_MyD3D->MyCtrl->m_keyRight]) {
            m_vecInputPuzzleKey.push_back(DIK_RIGHT);
            iOrder = static_cast<int>(m_vecInputPuzzleKey.size()) - 1;
            iKeyValue = DIK_RIGHT;
        }
    }
    else {
        std::set< int >::iterator sit = GetCurFormTemplate().setPuzzleKeyList.begin();
        for (; sit != GetCurFormTemplate().setPuzzleKeyList.end(); ++sit) {
            if (g_pkInput->IsDown(*sit)) {
                m_vecInputPuzzleKey.push_back(*sit);
                iOrder = static_cast<int>(m_vecInputPuzzleKey.size()) - 1;
                iKeyValue = *sit;
                break;
            }
        }
    }

    if (iKeyValue != -1) {
        if (IsPushValidPuzzleKey(iKeyValue, iOrder)) {
            g_MyD3D->MyHead->RenderPuzzleKeyPushEffect(iOrder);
        }
        else {
            g_MyD3D->MyHead->RenderPuzzleKeyFailEffect(iOrder);
        }
    }
}

void PLAYER::ClearPuzzleyKeyInfo()
{
    m_vecInputPuzzleKey.clear();
    m_vecQuestionPuzzleKey.clear();
}

bool PLAYER::CheckValidPuzzleKey()
{
    if (!IsLocalPlayer() ||
        m_vecInputPuzzleKey.empty() ||
        m_vecQuestionPuzzleKey.empty()) {
        return false;
    }
    return m_vecInputPuzzleKey == m_vecQuestionPuzzleKey;
}

bool PLAYER::IsPushValidPuzzleKey(int iKeyValue_, int iOrder_)
{
    if (iOrder_ >= static_cast<int>(m_vecQuestionPuzzleKey.size())) {
        return false;
    }

    if (iKeyValue_ == m_vecQuestionPuzzleKey[iOrder_]) {
        return true;
    }

    return false;
}

void PLAYER::CreateQuestionPuzzleKey(float fSafePosX_, float fOffsetY_)
{
    if (!IsLocalPlayer() ||
        GetCurFormTemplate().mapPuzzleKeyPattern.empty()) {
        return;
    }

    srand((unsigned)time(NULL));
    int randomNum = rand() % GetCurFormTemplate().mapPuzzleKeyPattern.size();

    m_vecQuestionPuzzleKey.clear();
    m_vecInputPuzzleKey.clear();
    g_MyD3D->MyHead->InitPuzzleKeyInfo();

    std::map< int, std::vector< int > >::iterator mit = GetCurFormTemplate().mapPuzzleKeyPattern.find(randomNum);
    if (mit != GetCurFormTemplate().mapPuzzleKeyPattern.end()) {
        m_vecQuestionPuzzleKey = mit->second;
    }


    D3DXVECTOR2 vCharPos(vPos.x, vPos.y);

    //왼쪽 벽 체크 - 매직넘버 좀 ㎲嗤맑도.... Check_SafePostionX에서 0.05라는 매직넘버를 사용하고 있어서... 어쩔 수 없이...
    D3DXVECTOR2 vSafePos = g_MyD3D->MyStg->Check_SafePostionX(vPos.x - fSafePosX_, vPos.y);
    if (vSafePos.x >= vPos.x - fSafePosX_ + 0.05f) {
        vCharPos.x += fSafePosX_;
    }

    //오른쪽 벽 체크
    vSafePos = g_MyD3D->MyStg->Check_SafePostionX(vPos.x + fSafePosX_, vPos.y);
    if (vSafePos.x <= vPos.x + fSafePosX_ - 0.05f) {
        vCharPos.x -= fSafePosX_;
    }

    if (fOffsetY_ != 0.f) {
        vCharPos.y += fOffsetY_;
    }

    for (int i = 0; i < static_cast<int>(m_vecQuestionPuzzleKey.size()); i++) {
        g_MyD3D->MyHead->RenderPuzzleKey(m_vecQuestionPuzzleKey[i], vCharPos);
    }
}

void PLAYER::SetFadeInOut(int iRed_, int iGreen_, int iBlue_, int iAlpha_ /*= 255*/, int iWeight_ /*= 0 */)
{
    m_sFadeInOutInfo.m_iRed = iRed_;
    m_sFadeInOutInfo.m_iGreen = iGreen_;
    m_sFadeInOutInfo.m_iBlue = iBlue_;
    if (iAlpha_ >= 0) {
        m_sFadeInOutInfo.m_iAlpha = iAlpha_;
    }
    m_sFadeInOutInfo.m_iWeight = iWeight_;
}

void PLAYER::RenderFadeInOut()
{
    if (m_sFadeInOutInfo.m_iWeight != 0) {
        m_sFadeInOutInfo.m_iAlpha += m_sFadeInOutInfo.m_iWeight;

        if (m_sFadeInOutInfo.m_iAlpha <= 0) {
            m_sFadeInOutInfo.m_iAlpha = 0;
            m_sFadeInOutInfo.m_iWeight = 0;
        }
        else if (m_sFadeInOutInfo.m_iAlpha >= 255) {
            m_sFadeInOutInfo.m_iAlpha = 255;
            m_sFadeInOutInfo.m_iWeight = 0;
        }

        g_pkGameOverlayUI->SetLoadingTextureColor(D3DCOLOR_ARGB(m_sFadeInOutInfo.m_iAlpha, m_sFadeInOutInfo.m_iRed,
            m_sFadeInOutInfo.m_iGreen, m_sFadeInOutInfo.m_iBlue));
    }
}

void PLAYER::CheckRageMode()
{
    if (!IsLocalPlayer()) {
        return;
    }

    SiGCGrowingSkillManager()->SetMagicEffectWhenRage(this);

    if (GetCurFormTemplate().iRageModeHpPercent == -1 || CheckSkill(SID_RIN_COMMON_RAGE_START) == false) {
        return;
    }

    if (m_fAbility[ABILITY_HP] * (static_cast<float>(GetCurFormTemplate().iRageModeHpPercent) / 100.f) >= m_fHP) {
        if (IsMagicEffect(EGC_EFFECT_RIN_RAGE_MODE_BUFF) == false) {
            SetMagicEffect(EGC_EFFECT_RIN_RAGE_MODE_BUFF, 999999.f);
        }
    }
    else {
        if (IsMagicEffect(EGC_EFFECT_RIN_RAGE_MODE_BUFF)) {
            ClearMagicEffect(EGC_EFFECT_RIN_RAGE_MODE_BUFF);
        }
    }
}

bool PLAYER::IsShowDialogue()
{
    return g_pkGameOverlayUI->IsShowDialogue();
}

void PLAYER::SetWindSpeed(float fWeightX, float fWeightY)
{
    // X축 바람속도 적용
    if (0.0f != x_WindSpeed)
    {
        if (IsContact)
            x_Speed += (x_WindSpeed * fWeightX);
        else
        {
            // 제자리 점프 시 Radio가 곱해지면 거의 티가 안난다.
            // 좀더 자연스럽게 하기위해 1.5f라는 체험넘버를 넣었다.
            if (false == g_MyD3D->MyCtrl->k_Left && false == g_MyD3D->MyCtrl->k_Right)
                x_Speed += x_WindSpeed * (m_fJump_WindRadio * 1.5f) * fWeightX;
            else
                x_Speed += x_WindSpeed * m_fJump_WindRadio * fWeightX;
        }

        x_WindSpeed = 0.0f;
        m_fJump_WindRadio = 1.0f;
    }

    // Y축 바람속도 적용
    if (0.0f != y_WindSpeed)
    {
        y_Speed += (y_WindSpeed * fWeightY);
        y_WindSpeed = 0.0f;
    }
}

void PLAYER::SetLock(bool bLock)
{
    ToggleRenderPlayer(bLock);
    m_bRender = !bLock;
    m_vecChildMeshParticle.clear();
    m_bRenderQuestDrama = !bLock;
    m_bCrashCheck = !bLock;
    m_bNoCrashCheck = bLock;
    m_bCrashCheck = !bLock;
    Item_Remain_Time[GC_GAME_ITEM_SUPER] = bLock ? INT_MAX : 0;
    m_bNotTargeting = bLock;

    x_Speed = 0.0f;
    y_Speed = 0.001f;

    if (IsLocalPlayer())
    {
        g_MyD3D->SetEnableKeyInput(!bLock);
        m_bFrameLock = bLock;
    }
}

void PLAYER::SetMotionLock(bool bLock)
{
    m_bBuffFrameLock = bLock;
    if (false == m_bBuffFrameLock)
    {
        IsContact = false;
    }
}

bool PLAYER::IsFallArenaGameMode()
{
    switch (SiKGCRoomManager()->GetGameMode())
    {
    case GC_GM_QUEST53:
    case GC_GM_QUEST46:
        return true;
    }
    return false;
}

// 포션 사용을 막는다.
bool PLAYER::NotUseShortCutItem(bool bCheckGameMode /*= false*/)
{
#pragma message("[NotUseShortCutItem] Descomentar para que os administradores possam usar rebirth mesmo em fases que nao pode")
    //if ( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
    //return false;

    switch (SiKGCRoomManager()->GetGameMode())
    {
    case GC_GM_DOTA:
        return true;
    }

    // DungeonInfo.lua¿¡¼­ Á¤ÀÇÇØ¼­ »ç¿ëÇÏ¼¼¿ä.
    if (SiKGCWorldMapManager()->IsNotUseShortCutItem(SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetDungeonLevel()))
    {
        if (bCheckGameMode == false)
        {
            if (g_MyD3D->GetMyShortCutSlot()->HasAllowedItem(SiKGCRoomManager()->GetGameMode()))
            {
                return false;
            }

            return true;
        }
    }

    return false;
}

// ºÎÈ° »ç¿ëÀ» ¸·´Â´Ù.(¼ö¹ÝÆ÷ÇÔ)
bool PLAYER::IsNotRebirth()
{
    if (SiKGCWorldMapManager()->IsNotRebirth(SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetDungeonLevel()))
        return true;

    return false;
}

#if defined( USE_AP_SLOT_CHANGE )
void PLAYER::ChangeQuickSlotSkill()
{
    if ((!g_kGlobalValue.IsQuickSkillPlayer(m_kUserInfo.GetCurrentChar().iCharType) &&
        !g_kGlobalValue.IsHybridPlayer(m_kUserInfo.GetCurrentChar().iCharType)) ||
        g_kGlobalValue.GetOpenSkillSlotIndex(m_kUserInfo.GetCurrentChar().iCharType) <= KSkillSlot::INDEX_A) {
        return;
    }

    std::map< int, vector< GCHotKeySkill > >::iterator mitPrev = m_mapHotKeySkillBySlotType.find(m_iCurQuickSlotType);

    m_iCurQuickSlotType++;
    if (m_iCurQuickSlotType >= KSkillSlot::INDEX_MAX) {
        m_iCurQuickSlotType = KSkillSlot::INDEX_A;
    }

    std::map< int, vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkillBySlotType.find(m_iCurQuickSlotType);

    if (mit != m_mapHotKeySkillBySlotType.end()) {
        if (mitPrev != m_mapHotKeySkillBySlotType.end()) {
            mitPrev->second = m_vecHotKeySkill;
        }
        m_vecHotKeySkill = mit->second;
    }
}
#endif

void PLAYER::CheckTargetInRange()
{
    PROFILE_SET("PLAYER::CheckTargetInRange");

    if (m_sRangeCheckInfo.m_vRangeCheckSize == D3DXVECTOR2(0.f, 0.f) ||
        GetPlayerState() == PS_DEAD) {
        return;
    }

    FRect rtRange, rtRangeOut;
    rtRange.left = g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.x - m_sRangeCheckInfo.m_vRangeCheckSize.x - 1.f;
    rtRange.right = g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.x + m_sRangeCheckInfo.m_vRangeCheckSize.x - 1.f;
    rtRange.top = g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.y + m_sRangeCheckInfo.m_vRangeCheckSize.y - 0.32f;
    rtRange.bottom = g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.y - m_sRangeCheckInfo.m_vRangeCheckSize.y - 0.32f;


    float fOutBoundCheck = 0.1f;

    rtRangeOut.left = rtRange.left - fOutBoundCheck;
    rtRangeOut.right = rtRange.right - fOutBoundCheck;
    rtRangeOut.top = rtRange.top + fOutBoundCheck;
    rtRangeOut.bottom = rtRange.bottom - fOutBoundCheck;

    // 플레이어가 범위안에 있는지 체크하자.
    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if (pPlayer == NULL || pPlayer->m_kUserInfo.bLive == false) {
            continue;
        }

        // 나는 체크하지 말자.
        if (pPlayer->m_kUserInfo.dwUID == m_kUserInfo.dwUID) {
            continue;
        }

        GCCollisionRect<float>& rtBody = pPlayer->GetCollisionRect();
        if (rtBody.CheckCollision(rtRange)) {
            if (g_MyD3D->IsSameTeam(i, m_iPlayerIndex)) {
                // 같은 편이면 좋은 버프 걸어주자.
                if (pPlayer->m_sRangeCheckInfo.m_bRangeInSameTeam == false) {
                    for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeBuff.end(); ++sit) {
                        // 같은 버프가 이미 걸려 있지 않다면 버프 걸어주자
                        if (pPlayer->IsMagicEffect(*sit) == false) {
                            g_pMagicEffect->SetMagicEffect(pPlayer->m_iPlayerIndex, *sit, 99999.f);
                            m_sRangeCheckInfo.m_mapAppliedBuff[pPlayer->m_kUserInfo.dwUID].insert(*sit);
                        }
                    }
                    pPlayer->m_sRangeCheckInfo.m_bRangeInSameTeam = true;
                }
            }
            else {
                // 다른 편이고 디버프가 있으면 해당 버프 걸어주자.
                if (pPlayer->m_sRangeCheckInfo.m_bRangeInEnemy == false) {
                    for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeDeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeDeBuff.end(); ++sit) {
                        if (pPlayer->IsMagicEffect(*sit) == false) {
                            g_pMagicEffect->SetMagicEffect(pPlayer->m_iPlayerIndex, *sit, 99999.f);
                            m_sRangeCheckInfo.m_mapAppliedDeBuff[pPlayer->m_kUserInfo.dwUID].insert(*sit);
                        }
                    }
                    pPlayer->m_sRangeCheckInfo.m_bRangeInEnemy = true;
                }
            }
        }
        else if (!rtBody.CheckCollision(rtRangeOut)) {
            // 범위 안에 있다가 밖으로 빠져 나가면 걸려있던 버프들은 모두 지워주자.
            if (g_MyD3D->IsSameTeam(i, m_iPlayerIndex)) {
                if (pPlayer->m_sRangeCheckInfo.m_bRangeInSameTeam == true) {
                    for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeBuff.end(); ++sit) {
                        std::map< DWORD, std::set< int > >::iterator mit = m_sRangeCheckInfo.m_mapAppliedBuff.find(pPlayer->m_kUserInfo.dwUID);
                        if (mit != m_sRangeCheckInfo.m_mapAppliedBuff.end()) {
                            std::set< int >::iterator sitBuff = mit->second.find(*sit);
                            if (sitBuff != mit->second.end()) {
                                if (pPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME) {
                                    SRangeCheckInfo sRangeCheck = pPlayer->GetRangeCheckInfo();
                                    std::set< int >::iterator sitRangeBuff = sRangeCheck.m_setRangeBuff.find(*sit);
                                    if (sitRangeBuff == sRangeCheck.m_setRangeBuff.end()) {
                                        if (g_pMagicEffect->IsMagicEffect(pPlayer->m_iPlayerIndex, *sit))
                                            g_pMagicEffect->ClearMagicEffect(pPlayer->m_iPlayerIndex, *sit);
                                    }
                                }
                                else {
                                    g_pMagicEffect->ClearMagicEffect(pPlayer->m_iPlayerIndex, *sit);
                                }
                                mit->second.erase(*sit);
                            }
                        }
                    }
                    pPlayer->m_sRangeCheckInfo.m_bRangeInSameTeam = false;
                }
            }
            else {
                if (pPlayer->m_sRangeCheckInfo.m_bRangeInEnemy == true) {
                    for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeDeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeDeBuff.end(); ++sit) {
                        std::map< DWORD, std::set< int > >::iterator mit = m_sRangeCheckInfo.m_mapAppliedDeBuff.find(pPlayer->m_kUserInfo.dwUID);
                        if (mit != m_sRangeCheckInfo.m_mapAppliedDeBuff.end()) {
                            std::set< int >::iterator sitBuff = mit->second.find(*sit);
                            if (sitBuff != mit->second.end()) {
                                g_pMagicEffect->ClearMagicEffect(pPlayer->m_iPlayerIndex, *sit);
                                mit->second.erase(*sit);
                            }
                        }
                    }
                    pPlayer->m_sRangeCheckInfo.m_bRangeInEnemy = false;
                }
            }
        }
    }

    GCCollisionRect<float> rtMonster;
    // 몬스터가 범위안에 있는지 체크하자.

    for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        MONSTER* pMonster = mit->second;
        if (!pMonster->m_bLive)
            continue;

        if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first))
            continue;

        bool bMonsterCollision = false;
        for (int j = 0; j < pMonster->GetAttackBoundRectNum(); j++)
        {
            rtMonster = pMonster->GetAttackBoundRect(j);

            if (rtMonster.CheckCollision(rtRange)) {
                bMonsterCollision = true;
                break;
            }
        }

        if (bMonsterCollision) {
            if (pMonster->GetInPlayerRangeCheck() == false) {
                for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeDeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeDeBuff.end(); ++sit) {
                    pMonster->SetMagicEffect(mit->first, *sit, 99999.0f, 1);
                }
                pMonster->SetInPlayerRangeCheck(true);
            }
        }
        else {
            if (pMonster->GetInPlayerRangeCheck() == true) {
                for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeDeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeDeBuff.end(); ++sit) {
                    pMonster->ClearMagicEffect(mit->first, *sit);
                }
                pMonster->SetInPlayerRangeCheck(false);
            }
        }
    }

}

void PLAYER::RenderRangeCheckBoundingBox()
{
    if (g_kGlobalValue.m_bRenderBoundBox &&
        m_sRangeCheckInfo.m_vRangeCheckSize != D3DXVECTOR2(0.f, 0.f))
    {
        FRect rtRange;
        rtRange.left = vPos.x - m_sRangeCheckInfo.m_vRangeCheckSize.x - 1.f;
        rtRange.right = vPos.x + m_sRangeCheckInfo.m_vRangeCheckSize.x - 1.f;
        rtRange.top = vPos.y + m_sRangeCheckInfo.m_vRangeCheckSize.y - 0.32f;
        rtRange.bottom = vPos.y - m_sRangeCheckInfo.m_vRangeCheckSize.y - 0.32f;

        D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
        SETVECTOR3(lefttop, rtRange.left, rtRange.top, 0.4f);
        SETVECTOR3(righttop, rtRange.right, rtRange.top, 0.4f);
        SETVECTOR3(leftbottom, rtRange.left, rtRange.bottom, 0.4f);
        SETVECTOR3(rightbottom, rtRange.right, rtRange.bottom, 0.4f);

        g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
            lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
            D3DCOLOR_ARGB(180, 0, 200, 100));
    }
}

void PLAYER::SetRangeCheckSize(float fRangeX_, float fRangeY_, bool bInit_, int iTime_)
{
    if (bInit_) {
        InitRangeInfo();
    }

    m_sRangeCheckInfo.m_vRangeCheckSize = D3DXVECTOR2(fRangeX_, fRangeY_);
    if (iTime_ == 0) {
        m_sRangeCheckInfo.m_kStopWatch.SetStopCheck(true);
    }
    else {
        m_sRangeCheckInfo.m_kStopWatch.SetStopWatch(iTime_ * 1000);
    }
}

void PLAYER::SetRangeBuffEffect(int iBuffIndex_, bool bDeBuff_, bool bClearPrevBuff /*= false */)
{
    if (bClearPrevBuff) {
        m_sRangeCheckInfo.m_setRangeBuff.clear();
        m_sRangeCheckInfo.m_setRangeDeBuff.clear();
    }

    if (bDeBuff_) {
        m_sRangeCheckInfo.m_setRangeDeBuff.insert(iBuffIndex_);
    }
    else {
        m_sRangeCheckInfo.m_setRangeBuff.insert(iBuffIndex_);
        // 좋은 버프는 자신도 받자. 나쁜 버프는 받을 필요 없지!!
        SetMagicEffect(iBuffIndex_, 99999.f, 0, true);
    }
}

void PLAYER::InitRangeInfo()
{
    if (m_sRangeCheckInfo.m_vRangeCheckSize == D3DXVECTOR2(0.f, 0.f)) {
        return;
    }

    for (int i = 0; i < MAX_PLAYER_NUM; i++) {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if (pPlayer == NULL ||
            pPlayer->m_kUserInfo.bLive == false) {
            continue;
        }

        if (pPlayer->m_kUserInfo.dwUID == m_kUserInfo.dwUID) {
            for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeBuff.end(); ++sit) {
                g_pMagicEffect->ClearMagicEffect(pPlayer->m_iPlayerIndex, *sit);
            }

            for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeDeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeDeBuff.end(); ++sit) {
                g_pMagicEffect->ClearMagicEffect(pPlayer->m_iPlayerIndex, *sit);
            }
        }
        else {
            std::map< DWORD, std::set< int > >::iterator mitBuff = m_sRangeCheckInfo.m_mapAppliedBuff.find(pPlayer->m_kUserInfo.dwUID);
            if (mitBuff != m_sRangeCheckInfo.m_mapAppliedBuff.end()) {
                for (std::set< int >::iterator sit = mitBuff->second.begin(); sit != mitBuff->second.end(); ++sit) {
                    g_pMagicEffect->ClearMagicEffect(pPlayer->m_iPlayerIndex, *sit);
                }
            }

            std::map< DWORD, std::set< int > >::iterator mitDeBuff = m_sRangeCheckInfo.m_mapAppliedDeBuff.find(pPlayer->m_kUserInfo.dwUID);
            if (mitDeBuff != m_sRangeCheckInfo.m_mapAppliedDeBuff.end()) {
                for (std::set< int >::iterator sit = mitDeBuff->second.begin(); sit != mitDeBuff->second.end(); ++sit) {
                    g_pMagicEffect->ClearMagicEffect(pPlayer->m_iPlayerIndex, *sit);
                }
            }
        }

        pPlayer->m_sRangeCheckInfo.m_bRangeInSameTeam = false;
        pPlayer->m_sRangeCheckInfo.m_bRangeInEnemy = false;
    }

    for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        MONSTER* pMonster = mit->second;
        for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeBuff.end(); ++sit) {
            pMonster->ClearMagicEffect(mit->first, *sit);
        }

        for (std::set< int >::iterator sit = m_sRangeCheckInfo.m_setRangeDeBuff.begin(); sit != m_sRangeCheckInfo.m_setRangeDeBuff.end(); ++sit) {
            pMonster->ClearMagicEffect(mit->first, *sit);
        }

        pMonster->SetInPlayerRangeCheck(false);
    }

    m_sRangeCheckInfo.Init();
}

void PLAYER::InitMarbleInfo()
{
    m_fCurMarbleNum = 0.f;

    if (GetCurFormTemplate().fMaxMarbleAmount == 0.0f) {
        return;
    }

#ifdef DECANEE
    if (GetCurrentChar().iCharType == GC_CHAR_DECANEE)
    {
        m_iDecaneeConsumeSpeed = 15.f;
        m_iDecaneeChargeSpeed = 45.f;

        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM);
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_DUN);
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV1);
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV1_DUN);
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV2);
        g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV2_DUN);

        if (IsTutorialMode())
        {
            if (CheckSkill(SID_DECANEE2_PASSIVE_ADD_02))
            {
                m_iDecaneeConsumeSpeed = 20.f;
                m_iDecaneeChargeSpeed = 60.f;
            }
            else if (CheckSkill(SID_DECANEE2_PASSIVE_ADD_03))
            {
                m_iDecaneeConsumeSpeed = 10.f;
                m_iDecaneeChargeSpeed = 30.f;
            }
        }
        else
        {
            if (IsEquipSkill(SID_DECANEE2_PASSIVE_ADD_02))
            {
                m_iDecaneeConsumeSpeed = 20.f;
                m_iDecaneeChargeSpeed = 60.f;
            }
            else if (IsEquipSkill(SID_DECANEE2_PASSIVE_ADD_03))
            {
                m_iDecaneeConsumeSpeed = 10.f;
                m_iDecaneeChargeSpeed = 30.f;
            }
        }
        return;
    }
#endif

    m_bCheckRunMarble = false;

    ////버프 걸어주자
    int iEquipMarbleSkill = -1;

    std::map< int, std::map< int, std::pair< int, float > > >::iterator mit = GetCurFormTemplate().mapSpecialBuff.find(SPECIAL_BUFF_MARBLE);
    if (mit != GetCurFormTemplate().mapSpecialBuff.end()) {
        for (std::map< int, std::pair< int, float > >::iterator mitSkill = mit->second.begin(); mitSkill != mit->second.end(); ++mitSkill) {
            const GCSkill* pSkill = SiGCSKT()->GetSkill(static_cast<EGCSkillTree>(mitSkill->first));
            if (pSkill && pSkill->m_bPassive) {
                if (IsLearnedSkill(mitSkill->first)) {
                    iEquipMarbleSkill = mitSkill->first;
                }
            }
            else {
                if (CheckSkill(mitSkill->first)) {
                    iEquipMarbleSkill = mitSkill->first;
                    break;
                }
            }
        }

        if (iEquipMarbleSkill != -1) {
            std::map< int, std::pair< int, float > >::iterator mitSB = mit->second.find(iEquipMarbleSkill);

            if (mitSB != mit->second.end()) {
                m_prSelectedMarbleBuff = mitSB->second;
                GetCurFormTemplate().fHGPDecreaseTime = mitSB->second.second;
            }
        }
    }
}

void PLAYER::CheckChargeMarbleDamageAtk(int iDamageIndex, bool bCheckOnlyOne)
{
    if (GetCurFormTemplate().fMaxMarbleAmount == 0.f ||
        IsUsableMarbleState() == false ||
        IsLocalPlayer() == false ||
        m_bCheckRunMarble) {
        return;
    }

    // 마블 차징이 가능한 데미지 인지 검사한다.
    if (iDamageIndex != -1) {
        std::map< int, std::pair< bool, float > >::iterator mit = m_mapCurEquippedMarbleChargeDamage.find(iDamageIndex);
        if (mit != m_mapCurEquippedMarbleChargeDamage.end()) {
            if ((mit->second.first == true && bCheckOnlyOne == false) ||
                (mit->second.first == false && bCheckOnlyOne == true)) {
                return;
            }

            m_fCurMarbleNum += mit->second.second;

            if (m_fCurMarbleNum >= GetCurFormTemplate().fMaxMarbleAmount) {
                m_fCurMarbleNum = GetCurFormTemplate().fMaxMarbleAmount;
#ifdef DECANEE
                if (GetCurrentChar().iCharType != GC_CHAR_DECANEE)
                    SetMagicEffect(m_prSelectedMarbleBuff.first, m_prSelectedMarbleBuff.second);
#else
                SetMagicEffect(m_prSelectedMarbleBuff.first, m_prSelectedMarbleBuff.second);
#endif
            }
        }
    }
}
void PLAYER::CheckChargeBurnDamageAtk(int iDamageIndex, bool bCheckOnlyOne)
{
    if (GetCurFormTemplate().bUseBurnCharge == false ||
        IsLocalPlayer() == false) {
        return;
    }

    // 버닝 차징이 가능한 데미지 인지 검사한다.
    if (iDamageIndex != -1) {
        std::map< int, std::pair< bool, float > >::iterator mit = m_mapCurEquippedBurnChargeDamage.find(iDamageIndex);
        if (mit != m_mapCurEquippedBurnChargeDamage.end()) {
            if ((mit->second.first == true && bCheckOnlyOne == false) ||
                (mit->second.first == false && bCheckOnlyOne == true)) {
                return;
            }
            float fTemp = m_fBurnningPoint;
            fTemp += mit->second.second;

            if (fTemp >= 3.0f)
                m_fBurnningPoint = 3.0f;
            else
                m_fBurnningPoint = fTemp;
        }
    }
}

void PLAYER::CheckChargeMarblePhysicAtk()
{
    if (GetCurFormTemplate().fMaxMarbleAmount == 0.f ||
        IsUsableMarbleState() == false ||
        IsLocalPlayer() == false ||
        m_bCheckRunMarble) {
        return;
    }

    std::map< int, float >::iterator mit = m_mapCurEquippedMarbleChargeMotion.find(uiMotion);

    // 마블 차징이 가능한 모션인지 검사한다.
    if (mit != m_mapCurEquippedMarbleChargeMotion.end()) {
        m_fCurMarbleNum += mit->second;
        if (m_fCurMarbleNum >= GetCurFormTemplate().fMaxMarbleAmount) {
            m_fCurMarbleNum = GetCurFormTemplate().fMaxMarbleAmount;
#ifdef DECANEEvoid PLAYER::CheckChargeMarbleDamageAtk(int iDamageIndex, bool bCheckOnlyOne)
            if (GetCurrentChar().iCharType != GC_CHAR_DECANEE)
                SetMagicEffect(m_prSelectedMarbleBuff.first, m_prSelectedMarbleBuff.second);
#else
            SetMagicEffect(m_prSelectedMarbleBuff.first, m_prSelectedMarbleBuff.second);
#endif
        }
    }
}

void PLAYER::InitCurEquippedMarbleChargeMotion()
{
    if (GetCurFormTemplate().fMaxMarbleAmount == 0.f) {
        return;
    }

    m_mapCurEquippedMarbleChargeMotion.clear();
    m_mapCurEquippedMarbleChargeDamage.clear();
    std::vector< SMarbleSytemInfo > vecMarbleSytemInfo = GetCurFormTemplate().vecMarbleSytemInfo;

    for (int i = 0; i < static_cast<int>(vecMarbleSytemInfo.size()); i++) {
        std::vector< int > vecMotionList = vecMarbleSytemInfo[i].m_vecMotionList;
        std::vector< int > vecDamageList = vecMarbleSytemInfo[i].m_vecDamageList;

        if (vecMotionList.size() == 1) {
            m_mapCurEquippedMarbleChargeMotion.insert(std::make_pair(vecMotionList[0], vecMarbleSytemInfo[i].m_fMarbleChargeNum));
        }
        else {
            for (int j = 0; j < static_cast<int>(vecMotionList.size()); j++) {
                m_mapCurEquippedMarbleChargeMotion.insert(std::make_pair(vecMotionList[j], vecMarbleSytemInfo[i].m_fMarbleChargeNum));
            }
        }

        for (int j = 0; j < static_cast<int>(vecDamageList.size()); j++) {
            m_mapCurEquippedMarbleChargeDamage.insert(std::make_pair(vecDamageList[j],
                std::make_pair(vecMarbleSytemInfo[i].m_bCheckOnlyOneUnit, vecMarbleSytemInfo[i].m_fMarbleChargeNum)));
        }
    }
}

void PLAYER::InitCurEquippedBurnChargeMotion()
{
    m_mapCurEquippedBurnChargeDamage.clear();
    std::vector< SBurnSytemInfo > vecBurnSytemInfo = GetCurFormTemplate().vecBurnSytemInfo;

    for (int i = 0; i < static_cast<int>(vecBurnSytemInfo.size()); i++) {
        std::vector< int > vecDamageList = vecBurnSytemInfo[i].m_vecDamageList;

        for (int j = 0; j < static_cast<int>(vecDamageList.size()); j++) {
            m_mapCurEquippedBurnChargeDamage.insert(std::make_pair(vecDamageList[j],
                std::make_pair(vecBurnSytemInfo[i].m_bCheckOnlyOneUnit, vecBurnSytemInfo[i].m_fBurnChargeNum)));
        }
    }
}

bool PLAYER::CheckRunMarbleMode()
{
    if (m_fCurMarbleNum >= GetCurFormTemplate().fMaxMarbleAmount && m_bCheckRunMarble == false && IsLocalPlayer()) {
        m_bCheckRunMarble = true;
    }

    return m_bCheckRunMarble;
}

#ifdef DECANEE
void PLAYER::IncreaseMarble()
{
    if (!IsLocalPlayer() || m_bCheckRunMarble == true)
        return;

    if (m_fCurMarbleNum >= GetCurFormTemplate().fMaxMarbleAmount) {
        m_fCurMarbleNum = GetCurFormTemplate().fMaxMarbleAmount;
        return;
    }

    if (GetCurFormTemplate().fBPHitIncrease != 0.0f)
        m_fCurMarbleNum += ((GetCurFormTemplate().fMaxMarbleAmount / (m_iDecaneeChargeSpeed * (float)GC_FPS_LIMIT)) / GetCurFormTemplate().fBPHitIncrease) * GetCurFormTemplate().fBPHitIncrease;
}
#endif

void PLAYER::DecreaseMarble()
{
    if (!IsLocalPlayer() || m_bCheckRunMarble == false) {
        return;
    }

#ifdef DECANEE
    if ( GetCurrentChar().iCharType == GC_CHAR_DECANEE )
    {
        m_fCurMarbleNum -= (GetCurFormTemplate().fMaxMarbleAmount / (GC_FPS_LIMIT * m_iDecaneeConsumeSpeed));
    }
    else
#endif
    {
        m_fCurMarbleNum -= (GetCurFormTemplate().fMaxMarbleAmount / (GC_FPS_LIMIT * GetCurFormTemplate().fHGPDecreaseTime));
    }

    if (m_fCurMarbleNum <= 0.f) {
        m_fCurMarbleNum = 0.f;
        m_bCheckRunMarble = false;
#ifdef DECANEE
        if ( uiMotion == MID_DECANEE1_SPECIAL_TRANSFORMATION_WAIT )
        {
            SetPlayerState(MID_DECANEE1_SPECIAL_TRANSFORMATION_TO_NORMAL_WAIT);
            cFrame = 0;
        }
#endif
    }
}

bool PLAYER::IsUsableMarbleState()
{
#ifdef DECANEE
    if ( g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_DECANEE )
    {
        if ( CheckSkill( SID_DECANEE1_COMBO_ADD_05 ) )
            return true;
        return false;
    }
#endif
    if (!m_mapCurEquippedMarbleChargeMotion.empty()) {
        bool bLearn = false;
        std::map< int, std::map< int, std::pair< int, float > > >::iterator mit = GetCurFormTemplate().mapSpecialBuff.find(SPECIAL_BUFF_MARBLE);
        if (mit != GetCurFormTemplate().mapSpecialBuff.end()) {
            for (std::map< int, std::pair< int, float > >::iterator mitSkill = mit->second.begin(); mitSkill != mit->second.end(); ++mitSkill) {
                const GCSkill* pSkill = SiGCSKT()->GetSkill(static_cast<EGCSkillTree>(mitSkill->first));
                if (pSkill && pSkill->m_bPassive) {
                    if (IsLearnedSkill(mitSkill->first)) {
                        bLearn = true;
                    }
                }
                else {
                    if (CheckSkill(mitSkill->first)) {
                        bLearn = true;
                        break;
                    }
                }
            }
        }
        return bLearn;
    }
    return false;
}


void PLAYER::InitCombinationSkill()
{
    if (IsLocalPlayer() == false ||
        GetCurFormTemplate().mapCombinationSkillList.empty()) {
        return;
    }

    std::map< int, SCombinationSkillSetInfo >::iterator mit = GetCurFormTemplate().mapCombinationSkillList.begin();

    for (mit; mit != GetCurFormTemplate().mapCombinationSkillList.end(); ++mit) {
        for (std::vector< std::pair< int, bool > >::iterator vit = mit->second.vecSkillList.begin(); vit != mit->second.vecSkillList.end(); ++vit) {
            vit->second = false;
        }
    }
}

bool PLAYER::RunCombinationSkill(int iCombinationIdex_)
{
    if (IsLocalPlayer() == false ||
        GetCurFormTemplate().mapCombinationSkillList.empty()) {
        return false;
    }

    std::map< int, SCombinationSkillSetInfo >::iterator mit = GetCurFormTemplate().mapCombinationSkillList.find(iCombinationIdex_);

    bool bUsable = true;
    if (mit != GetCurFormTemplate().mapCombinationSkillList.end()) {
        // 연계 콤보가 가능한 상태인지를 체크한다ㅏ.
        if (static_cast<int>(mit->second.vecSkillList.size()) == 0) {
            bUsable = false;
        }
        else {
            for (int i = 0; i < static_cast<int>(mit->second.vecSkillList.size()); i++) {
                if (mit->second.vecSkillList[i].second == false) {
                    bUsable = false;
                    break;
                }
            }
        }

        if (bUsable) {
            if (mit->second.prAutoRunSkillInfo.second > 0.f &&
                mit->second.prAutoRunSkillInfo.second <= g_MyD3D->MyHead->Level) {
                g_MyD3D->MyHead->Level -= mit->second.prAutoRunSkillInfo.second;
                SetPlayerState(mit->second.prAutoRunSkillInfo.first);
                cFrame = 0;
            }
            else if (mit->second.prAutoRunSkillInfo.second == 0.f) {
                SetPlayerState(mit->second.prAutoRunSkillInfo.first);
                cFrame = 0;
            }

            std::map< int, SCombinationSkillSetInfo >::iterator mitInit = GetCurFormTemplate().mapCombinationSkillList.begin();
            for (mitInit; mitInit != GetCurFormTemplate().mapCombinationSkillList.end(); ++mitInit) {
                for (int i = 0; i < static_cast<int>(mitInit->second.vecSkillList.size()); i++) {
                    mitInit->second.vecSkillList[i].second = false;
                }
            }
        }
    }

    return bUsable;
}

void PLAYER::CheckCombinationSkill(int iMotion_)
{
    if (IsLocalPlayer() == false ||
        GetCurFormTemplate().mapCombinationSkillList.empty()) {
        return;
    }

    std::map< int, SCombinationSkillSetInfo >::iterator mit = GetCurFormTemplate().mapCombinationSkillList.begin();

    for (; mit != GetCurFormTemplate().mapCombinationSkillList.end(); ++mit) {
        for (std::vector< std::pair< int, bool > >::iterator vit = mit->second.vecSkillList.begin(); vit != mit->second.vecSkillList.end(); ++vit) {
            if (vit->first == iMotion_ && vit->second == false) {
                vit->second = true;
                break;
            }
        }
    }
}

void PLAYER::CreateReflectionDamage()
{
    if (m_prReflectionDamage.first == -1) {
        return;
    }

    CDamageInstance* pkDamage = AddDamage(m_prReflectionDamage.first, 0);
    pkDamage->m_Angle = m_prReflectionDamage.second;
    m_prReflectionDamage = std::make_pair(-1, 0.f);
}

void PLAYER::SetAllObjectBuff(int iBuffIndex_, float fBuffTime_, bool bDeBuff, int iLv /*= 0*/)
{
    if (!IsLocalPlayer()) {
        return;
    }

    for (int i = 0; i < MAX_PLAYER_NUM; i++) {
        if (g_MyD3D->MyPlayer[i] && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive) {
            if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i) && bDeBuff == true) { // 아군이면서 디버프면 무시
                continue;
            }
            else if (!g_MyD3D->IsSameTeam(m_iPlayerIndex, i) && bDeBuff == false) { // 적이면서 디버프가 아니면 무시
                continue;
            }
            else if (!g_MyD3D->IsSameTeam(m_iPlayerIndex, i) && bDeBuff == true && g_MyD3D->MyPlayer[i]->Super != 0) { // 적이면서 디버프고 super이면 무시
                continue;
            }


            g_MyD3D->MyPlayer[i]->SetMagicEffect(iBuffIndex_, fBuffTime_, 0, true);
        }
    }

    for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        if (mit->second->IsLive()) {
            if (g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first) && bDeBuff == true) {
                continue;
            }
            else if (!g_MyD3D->IsSameTeam_PlayerAndMonster(m_iPlayerIndex, mit->first) && bDeBuff == false) {
                continue;
            }
            mit->second->SetMagicEffectNoLocal(mit->first, iBuffIndex_, fBuffTime_, 1, m_iPlayerIndex);
            g_pMagicEffect->SetMagicEffect(mit->first, iBuffIndex_, fBuffTime_, 2, m_iPlayerIndex, false);
        }
    }
}

void PLAYER::SetRenderWeather(int iWeatherIndex_, bool bRender_, DWORD dwTime_)
{
    SiKGCWeatherSystem()->SetRender(iWeatherIndex_, bRender_, dwTime_);
}

int PLAYER::GetEncoachFaceSwitch(int iOriginalFaceType)
{
#ifdef DECANEE
    if (m_kUserInfo.GetCurrentChar().iCharType != GC_CHAR_DECANEE)
        return iOriginalFaceType;
    if (IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV1) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV2) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_DUN) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV1_DUN) || IsMagicEffect(EGC_EFFECT_DECANEE1_SPECIAL_TRANSFORM_LV2))
    {
        switch (iOriginalFaceType)
        {
        case FACE_LIFT_NORMAL:          return FACE_LIFT_ENCOACH_00;
        case FACE_LIFT_EXPRESSION_07:   return FACE_LIFT_ENCOACH_01;
        case FACE_LIFT_JOKE:            return FACE_LIFT_ENCOACH_02;
        case FACE_LIFT_EXPRESSION_01:   return FACE_LIFT_ENCOACH_03;
        case FACE_LIFT_SPECIAL:         return FACE_LIFT_ENCOACH_04;
        case FACE_LIFT_EXPRESSION_04:   return FACE_LIFT_ENCOACH_04;
        case FACE_LIFT_JOKE2:           return FACE_LIFT_ENCOACH_02;
        case FACE_LIFT_EXPRESSION_06:   return FACE_LIFT_ENCOACH_02;
        default:                        return iOriginalFaceType;
        }
    }
    else
#endif
        return iOriginalFaceType;
}

void PLAYER::SetRenderFaceLift(int iFaceLiftType_)
{
    // 표정변화없는 모델이 로드되어 있다면(NFL)
    if (NULL != m_pNonChangeFaceLift)
    {
        m_pNonChangeFaceLift->m_bExtraMeshObject = false;
        m_pNonChangeFaceLift->m_bRender = true;
        return;
    }

    if (m_mapFaceLiftMeshObject.empty()) {
        return;
    }

    // 면상 모두 off
    std::map< int, GCMeshObject* >::iterator mit = m_mapFaceLiftMeshObject.begin();
    for (; mit != m_mapFaceLiftMeshObject.end(); ++mit) {
        mit->second->m_bExtraMeshObject = true;
        mit->second->m_bRender = false;
    }

    if (m_bIsRenderBaseMesh == false && m_bIsRenderFace == false) {
        return;
    }

    if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&m_kUserInfo.GetCurrentChar(), ITEMID_ASIN_SHAADOW_SET, true)) {
        iFaceLiftType_ = FACE_LIFT_SHADOW;
    }

    if (m_iDefaultFaceLiftType == FACE_LIFT_BIG_HEAD) {
        iFaceLiftType_ = m_iDefaultFaceLiftType;
    }

    if (CheckTransformPetSkill()) {
        iFaceLiftType_ = FACE_LIFT_NORMAL;
    }

    // 얼굴 없는 아이템인지
    if (m_bEquipFaceOffHead == false) {
        if (GetCurrentChar().iCharType != GC_CHAR_LUPUS &&
            GetCurrentChar().iCharType != GC_CHAR_RIN &&
            GetCurrentChar().iCharType != GC_CHAR_ASIN &&
            GetCurrentChar().iCharType != GC_CHAR_LIME)
            return;
        else
            iFaceLiftType_ = FACE_LIFT_NORMAL;
    }

    int iNewFace = GetEncoachFaceSwitch(iFaceLiftType_);

    std::map< int, GCMeshObject* >::iterator mitFace = m_mapFaceLiftMeshObject.find(iNewFace);
    if (mitFace != m_mapFaceLiftMeshObject.end() && mitFace->second) {
        mitFace->second->m_bExtraMeshObject = false;
        mitFace->second->m_bRender = true;
        mitFace->second->m_bRender = (GetCurrentChar().iCharType == GC_CHAR_UNO && IsMagicEffect(EGC_EFFECT_UNO1_SPECIAL4_TRANSFORM, true)) ? false : true;
    }
}

bool PLAYER::IsTrainedSkill(EGCSkillTree eSkillID_)
{
    if (m_kUserInfo.GetCurrentChar().m_vecSPInfo.empty()) {
        return false;
    }

    for (int i = 0; i < static_cast<int>(m_kUserInfo.GetCurrentChar().m_vecSPInfo.size()); i++) {
        for (int j = 0; j < static_cast<int>(m_kUserInfo.GetCurrentChar().m_vecSPInfo[i].m_vecSkills.size()); j++) {
            if (static_cast<EGCSkillTree>(m_kUserInfo.GetCurrentChar().m_vecSPInfo[i].m_vecSkills[j]) == eSkillID_) {
                return true;
            }
        }
    }

    return false;
}

bool PLAYER::IsEquipSkill(EGCSkillTree eSkillID_)
{
    std::map<char, std::vector<KSkillSlot> >::iterator mit = m_kUserInfo.GetCurrentChar().m_mapEquipSkill.find(GetCurrentChar().iPromotionLevel);

    if (mit != m_kUserInfo.GetCurrentChar().m_mapEquipSkill.end()) {
        std::vector<KSkillSlot>::iterator vit = mit->second.begin();

        for (vit; vit != mit->second.end(); ++vit) {
            if (vit->m_nSkillID == eSkillID_) {
                return true;
            }
        }
    }

    return false;
}

D3DXVECTOR2 PLAYER::GetSafePosition(float fX, float fY)
{
    D3DXVECTOR2 vSafePos = g_MyD3D->MyStg->Check_SafePostionX(fX, fY);
    SiKGCFantasticMap()->CheckPierceBlockXPos(fX, vSafePos.x, fY);
    return vSafePos;
}

#if defined( USE_CAMERA_ZOOM_STATE )
void PLAYER::AddCameraZoomState(float fBefore, float fAfter)
{
    // 게임종료시에는 StartTime이 셋팅된 모든 단계시간을 누적함.
    if (0.0f == fBefore && 0.0f == fAfter)
    {
        std::map<float, std::pair<DWORD, DWORD> >::iterator mapIter = m_mapCameraZoomState.begin();
        for (; mapIter != m_mapCameraZoomState.end(); ++mapIter)
        {
            if (0 < mapIter->second.first)
            {
                mapIter->second.second += (timeGetTime() - mapIter->second.first) / 1000;
                mapIter->second.first = 0;
            }
        }
    }
    else
    {
        std::map<float, std::pair<DWORD, DWORD> >::iterator mapIter = m_mapCameraZoomState.find(fBefore);
        if (mapIter != m_mapCameraZoomState.end())
        {
            mapIter->second.second += (timeGetTime() - mapIter->second.first) / 1000;
            mapIter->second.first = 0;
        }

        mapIter = m_mapCameraZoomState.find(fAfter);
        if (mapIter != m_mapCameraZoomState.end())
        {
            mapIter->second.first = timeGetTime();
        }
        else
        {
            m_mapCameraZoomState.insert(std::make_pair(fAfter, std::make_pair(timeGetTime(), 0)));
        }

        // 패킷발싸!!~
        if (IsLocalPlayer())
        {
            KGC_PID_BROAD_CAMERAZOOM_STATE kPacket;
            kPacket.dwUID = m_kUserInfo.dwUID;
            kPacket.fBefore = fBefore;
            kPacket.fAfter = fAfter;
            SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE);
            KGCPC_MEMO("KGC_PID_BROAD_CAMERAZOOM_STATE");
        }
    }
}
#endif

// 진짜 각성 들가면 삭제되야하는 함수.
void PLAYER::SwapSlotSkillType(bool bIsSkillTypeB /*= false */)
{
    if (IsLocalPlayer() == false) {
        return;
    }

    if (bIsSkillTypeB) {
        if (m_iSkillType == 0) {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_B];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_B];
        }
        else {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_B];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_B];
        }
    }
    else {
        if (m_iSkillType == 0) {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_A];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_A];
        }
        else {
            m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_CASH_A];
            m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_CASH_A];
        }
    }
}

// 진짜 각성 들가면 삭제되야하는 함수.
void PLAYER::CountSkillCoolTime()
{
    if (IsLocalPlayer() == false) {
        return;
    }

    for (std::map< int, std::vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkill.begin(); mit != m_mapHotKeySkill.end(); ++mit) {
        if (mit->second.empty() == false) {
            for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
                if (cFrame > vit->iEndKeyTime)
                {
                    if (0 >= vit->fCoolTime) {
                        vit->iStartKeyTime = 0;
                        vit->iEndKeyTime = 0;
                    }
                    else {
                        vit->fCoolTime -= GC_ELAPSED_TIME;
                    }
                }
            }
        }
    }

    // 무기 체인지 됐을 때 보존하고 있던 쿨타임도 실시간으로 계산해준다.
    if (m_mapPrevHotKeyCoolTime.empty() == false) {
        for (std::map< int, std::vector< KHotKeyCoolTimeInfo > >::iterator mit = m_mapPrevHotKeyCoolTime.begin(); mit != m_mapPrevHotKeyCoolTime.end(); ++mit) {
            if (mit->second.empty() == false) {
                for (std::vector< KHotKeyCoolTimeInfo >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
                    if (cFrame > vit->iEndKeyTime)
                    {
                        if (0 >= vit->fCoolTime) {
                            vit->iStartKeyTime = 0;
                            vit->iEndKeyTime = 0;
                        }
                        else {
                            vit->fCoolTime -= GC_ELAPSED_TIME;
                        }
                    }
                }
            }
        }
    }
}

void PLAYER::ClearSkillCoolTime()
{
    for (std::map< int, std::vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkill.begin(); mit != m_mapHotKeySkill.end(); ++mit) {
        if (mit->second.empty() == false) {
            for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
                vit->iStartKeyTime = 0;
                vit->iEndKeyTime = 0;
                vit->fCoolTime = 0.f;
            }
        }
    }

    // 무기 체인지 됐을 때 보존하고 있던 쿨타임도 실시간으로 계산해준다.
    if (m_mapPrevHotKeyCoolTime.empty() == false) {
        for (std::map< int, std::vector< KHotKeyCoolTimeInfo > >::iterator mit = m_mapPrevHotKeyCoolTime.begin(); mit != m_mapPrevHotKeyCoolTime.end(); ++mit) {
            if (mit->second.empty() == false) {
                for (std::vector< KHotKeyCoolTimeInfo >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
                    vit->iStartKeyTime = 0;
                    vit->iEndKeyTime = 0;
                    vit->fCoolTime = 0.f;
                }
            }
        }
    }
    for (unsigned int i = 0; i < m_vecHotKeySkill.size(); i++)
    {
        m_vecHotKeySkill[i].fCoolTime = 0.0f;
        m_vecHotKeySkill[i].iStartKeyTime = 0;
        m_vecHotKeySkill[i].iEndKeyTime = 0;
    }
}

void PLAYER::SetSkillCoolTime(GCHotKeySkill* skill)
{
    if (IsLocalPlayer() == false) {
        return;
    }

    if (skill) {
        for (std::map< int, std::vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkill.begin(); mit != m_mapHotKeySkill.end(); ++mit) {
            if (mit->second.empty() == false) {
                for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
                    if (vit->m_eSkill == skill->m_eSkill) {
                        vit->fCoolTime = skill->fCoolTime;
                        vit->iStartKeyTime = skill->iStartKeyTime;
                        vit->iEndKeyTime = skill->iEndKeyTime;
                    }
                }
            }
        }
    }
}

void PLAYER::UpdateChangedSkillSlot()
{
    if (IsLocalPlayer() == false || g_MyD3D->m_pStateMachine->GetState() != GS_GAME) {
        return;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////        
    // 이전 쿨 타임을 저장한다.    
    KHotKeyCoolTimeInfo kCoolTimeSp4;
    //for( std::vector< GCHotKeySkill >::iterator vit = m_vecHotKeySkill.begin(); vit != m_vecHotKeySkill.end(); ++vit ) {            
    //    const GCSkill* pSkill = SiGCSKT()->GetSkill( vit->m_eSkill );
    //    if( pSkill && pSkill->m_pHotKeySkill && pSkill->m_pHotKeySkill->m_iChargingSkillLV == 4 && vit->fCoolTime > 0.f ) {
    //        // 4단 필살기 쿨타임은 공용으로 사용하기 때문에 따로 저장한다.
    //        kCoolTimeSp4.m_eSkill = vit->m_eSkill;
    //        kCoolTimeSp4.fCoolTime = vit->fCoolTime;
    //        kCoolTimeSp4.iEndKeyTime = vit->iEndKeyTime;
    //        kCoolTimeSp4.iStartKeyTime = vit->iStartKeyTime;
    //    }
    //}

    std::map< int, std::vector< KHotKeyCoolTimeInfo > > mapPrevHotKeyCoolTime = m_mapPrevHotKeyCoolTime;
    bool bFirst = false;
    if (m_mapPrevHotKeyCoolTime.empty()) {
        bFirst = true;
    }
    m_mapPrevHotKeyCoolTime.clear();

    for (std::map< int, std::vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkill.begin(); mit != m_mapHotKeySkill.end(); ++mit) {
        std::vector< KHotKeyCoolTimeInfo > vecPrevCoolTime;
        for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
            KHotKeyCoolTimeInfo kCoolTime;
            kCoolTime.m_eSkill = vit->m_eSkill;
            kCoolTime.fCoolTime = vit->fCoolTime;
            kCoolTime.iEndKeyTime = vit->iEndKeyTime;
            kCoolTime.iStartKeyTime = vit->iStartKeyTime;
            vecPrevCoolTime.push_back(kCoolTime);

            const GCSkill* pSkill = SiGCSKT()->GetSkill(vit->m_eSkill);
            if (pSkill && pSkill->m_pHotKeySkill && pSkill->m_pHotKeySkill->m_iChargingSkillLV == 4 && vit->fCoolTime > 0.f) {
                // 4단 필살기 쿨타임은 공용으로 사용하기 때문에 따로 저장한다.
                kCoolTimeSp4.m_eSkill = vit->m_eSkill;
                kCoolTimeSp4.fCoolTime = vit->fCoolTime;
                kCoolTimeSp4.iEndKeyTime = vit->iEndKeyTime;
                kCoolTimeSp4.iStartKeyTime = vit->iStartKeyTime;
            }
        }
        m_mapPrevHotKeyCoolTime.insert(std::make_pair(mit->first, vecPrevCoolTime));
    }
    if (bFirst) {
        mapPrevHotKeyCoolTime = m_mapPrevHotKeyCoolTime;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    g_MyD3D->MyHead->IsBlackMagic = true;
    m_iSkillType = 0;
    ReSettingSpecial4Skill(GetCurrentChar().iCharType, static_cast<int>(Extra_Char_Num / GC_CHAR_NUM));

    // 이전 쿨타임을 재 셋팅한다.
    if (mapPrevHotKeyCoolTime.empty() == false) {
        for (std::map< int, std::vector< KHotKeyCoolTimeInfo > >::iterator mit = mapPrevHotKeyCoolTime.begin(); mit != mapPrevHotKeyCoolTime.end(); ++mit) {
            std::map< int, std::vector< GCHotKeySkill > >::iterator mitSkill = m_mapHotKeySkill.find(mit->first);
            if (mitSkill != m_mapHotKeySkill.end()) {
                for (int i = 0; i < static_cast<int>(mit->second.size()); i++) {
                    if (i < static_cast<int>(mitSkill->second.size())) {
                        const GCSkill* pSkill = SiGCSKT()->GetSkill(mitSkill->second[i].m_eSkill);
                        // 4단필은 쿨타임 공유하기때문에 제외 시킴
                        if (pSkill && pSkill->m_pHotKeySkill &&
                            pSkill->m_pHotKeySkill->m_iChargingSkillLV == 4 && kCoolTimeSp4.fCoolTime > 0.f) {
                            mitSkill->second[i].fCoolTime = kCoolTimeSp4.fCoolTime;
                            mitSkill->second[i].iEndKeyTime = kCoolTimeSp4.iEndKeyTime;
                            mitSkill->second[i].iStartKeyTime = kCoolTimeSp4.iStartKeyTime;
                        }
                        else {
                            if (mitSkill->second[i].m_eSkill == mit->second[i].m_eSkill) {
                                mitSkill->second[i].fCoolTime = mit->second[i].fCoolTime;
                                mitSkill->second[i].iEndKeyTime = mit->second[i].iEndKeyTime;
                                mitSkill->second[i].iStartKeyTime = mit->second[i].iStartKeyTime;
                            }
                        }
                    }
                }
            }
        }
    }

    m_vecHotKeySkill = m_mapHotKeySkill[SKILL_TYPE_NORMAL_A];
    m_vecPossibleChargingSkillInfo = m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_A];

    if (g_pkUIScene->m_pkGameOverlayUI) {
        g_pkUIScene->m_pkGameOverlayUI->ResetComboSkill();
    }
}

void PLAYER::SetQPChangeFrame(int iQP_)
{
    GetCurFormTemplate().iQPChargeFrame = iQP_;
}

void PLAYER::SetRageModeHpPercent(int iPercent_)
{
    GetCurFormTemplate().iRageModeHpPercent = iPercent_;
}

void PLAYER::ResetFaceLiftDelay()
{
    std::map< int, std::pair<int, int> >::iterator mapIter = m_mapFaceLiftDelayInfo.begin();
    for (; mapIter != m_mapFaceLiftDelayInfo.end(); ++mapIter)
    {
        mapIter->second.first = 0;
        mapIter->second.second = 0;
    }
}

// 플레이어 정보 초기화 시 등에서 하드하게 지워야 하는 버프들은 여기다 넣자.
void PLAYER::ForcedClearBuffList()
{
    // 지워져야 할 버프들 지우자.
    std::vector< int > vecClearBuffList;
    vecClearBuffList.push_back(EGC_EFFECT_JIN1_SPECIAL4_BUFF);
    vecClearBuffList.push_back(EGC_EFFECT_JIN4_SPECIAL4_BUFF);
    vecClearBuffList.push_back(EGC_EFFECT_ELESIS1_SPECIAL4_CHANGE_MODE);
    vecClearBuffList.push_back(EGC_EFFECT_RONAN3_SPECIAL4_TEMPEST);
    vecClearBuffList.push_back(EGC_EFFECT_RONAN4_SPECIAL4_BUFF);
    vecClearBuffList.push_back(EGC_EFFECT_SIEG3_SPECIAL4_BUFF);
    vecClearBuffList.push_back(EGC_EFFECT_MARI1_SPECIAL4_BUFF);
    vecClearBuffList.push_back(EGC_EFFECT_MARI2_SPECIAL4_BUFF);

    for (std::vector< int >::iterator vit = vecClearBuffList.begin(); vit != vecClearBuffList.end(); ++vit) {
        if (IsMagicEffect(*vit)) {
            ClearMagicEffect(*vit);

            if (EGC_EFFECT_RONAN4_SPECIAL4_BUFF == *vit) {
                std::map< int, std::vector<CDamageInstance*> >::iterator itor;
                for (itor = g_MyD3D->m_pDamageManager->GetDamageInstancePool().begin(); itor != g_MyD3D->m_pDamageManager->GetDamageInstancePool().end(); ++itor)
                {
                    std::vector<CDamageInstance*>::iterator vit;
                    for (vit = itor->second.begin(); vit != itor->second.end(); vit++)
                    {
                        CDamageInstance* pDamageOld = *vit;

                        if (pDamageOld->m_HitWho == m_iPlayerIndex)
                        {
                            if (pDamageOld->m_What == DT_RONAN4_SPECIAL4_SWORD_MESH || DT_RONAN4_SPECIAL4_ERUDON_SWORD_MESH ||
                                pDamageOld->m_What == DT_RONAN4_SPECIAL4_SWORD_DMG
                                )
                            {
                                pDamageOld->m_Life = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool PLAYER::MoveToNearTargetPos(float fLimitDist_, float fPosX_, float fPosY_, bool bCheckBoss_, bool bCheckRight_ /* = false */, bool bCheckLeft_ /* = false */, bool bDirReverse_, float fLimitRangeY_)
{
    D3DXVECTOR2* vPos = GetTargetPos(1, fLimitDist_, bCheckBoss_, bCheckRight_, bCheckLeft_, fLimitRangeY_);

    if (vPos) {
        if (vPos[0].x != 0.f && vPos[0].y != 0.f) {
            SetDirectMove(vPos[0].x, vPos[0].y, true, true, fPosX_, fPosY_, bDirReverse_);
            return true;
        }
    }

    return false;
}

D3DXVECTOR2 PLAYER::GetScreenPos()
{
    D3DXMATRIX mat1, mat2, matBasic;
    D3DXVECTOR3 vPlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    Get_BasicMatrix(&matBasic, true, true, true);
    D3DXMatrixTranslation(&mat2, -0.15f, -0.1f, 0);
    D3DXMatrixMultiply(&mat1, &matBasic, &mat2);

    ::D3DXVec3TransformCoord(&vPlayerPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mat1);

    float fScreenX = int((vPlayerPos.x + 1.0f) * (GC_SCREEN_FLOAT_WIDTH / 2.0f)) + 0.5f;
    float fScreenY = int((-vPlayerPos.y + 0.75f) * (GC_SCREEN_FLOAT_WIDTH / 2.0f)) + 0.5f;
    fScreenX *= g_pGCDeviceManager->GetWindowScaleX();
    fScreenY *= g_pGCDeviceManager->GetWindowScaleY();

    return D3DXVECTOR2(fScreenX, fScreenY);

}

bool PLAYER::IsPossibleCounterMotion(UINT Motion)
{
    if (GC_GLOBAL_DEFINE::bMatchBalance2014 == FALSE) {
        // 린 변신 시는 카운터 불가능
        if (MetamorphosisForm == FORM_RIN_EVIL || MetamorphosisForm == FORM_RIN_GODDESS) {
            return false;
        }
    }

    switch (Motion)
    {
    case MID_COMMON_SMALLDAMAGE:
    case MID_COMMON_BIGDAMAGE:
    case MID_COMMON_DOWN_AND_STANDUP:
    case MID_COMMON_DAMAGED_COUNTER_SPIKE_TRAP:
    case MID_COMMON_DAMAGED_SPIKE_TRAP:
    case MID_COMMON_DIO_SPECIAL4_HITTED:
    case MID_NEPHILIM_DIO_SPECIAL4_HITTED:
    case MID_MDAN_DIO_SPECIAL4_HITTED:
    case MID_COMMON_DOWN_IN_SKY_NO_STOP:
    case MID_COMMON_DAMAGED_BY_FLYATK:
    case MID_COMMON_DAMAGED_MID_SPECIAL:
    case MID_COMMON_DAMAGED_UPPER_SPECIAL:
    case MID_COMMON_DOWN_IN_SKY:
    case MID_COMMON_FRONT_FALL_DOWN:
    case MID_COMMON_DAMAGED_DIO3_TENACIOUS_HOLD:
    case MID_RIN3_COMMON_GODDESS_UNIQUE_SKILL_1:
    case MID_COMMON_FRONTDOWN_AND_STANDUP:
    case MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER:
    case MID_COMMON_SMALLDAMAGE_NO_COUNTER:
    case MID_COMMON_BIGDAMAGE_NO_COUNTER:
    case MID_COMMON_DOWN_AND_STANDUP_NO_COUNTER:
    case MID_COMMON_FRONTDOWN_AND_STANDUP_NO_DIE:
    case MID_ZERO4_COMMON_DAMAGED_BLACKHALL:
    case MID_COMMON_AMY1_SPECIAL4_HITTED:
    case MID_COMMON_PIERCING_DAMAGED:
    case MID_DAMAGED_BY_HELL_MOUTH:
    case MID_COMMON_LEY3_HAUNT_CURSE4_DAMAGE:
    case MID_RIN3_NEPHILIM_GODDESS_UNIQUE_SKILL_1:
    case MID_RIN3_MDAN_GODDESS_UNIQUE_SKILL_1:
    case MID_COMMON_DAMAGED_BY_GUARD_ENABLE_COUNTER:
        return true;
    default:
        return false;
    }
}

void PLAYER::SetCharOutLineColor(float r_, float g_, float b_)
{
    m_pObject->SetOutlineColor(D3DXVECTOR4(r_, g_, b_, 1));
}

int PLAYER::CountLiveTeamPlayer()
{
    int iLiveCnt = 0;
    int iMaxPlayerNum = MAX_PLAYER_NUM;
    if (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory()) {
        iMaxPlayerNum = MAX_DUNGEON_PLAYER_NUM;
    }

    for (int i = 0; i < iMaxPlayerNum; i++) {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == m_kUserInfo.dwUID) {
            continue;
        }

        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->IsSameTeam(m_iPlayerIndex, i)) {
            iLiveCnt++;
        }
    }

    return iLiveCnt;
}

void PLAYER::SetOtherSpecialFaceTexture()
{
    if (GetCurFormTemplate().iOtherFaceHowToCheck == -1) {
        return;
    }

    switch (GetCurFormTemplate().iOtherFaceHowToCheck)
    {
    case SPECIAL_FACE_TRAINED_SKILL:
    {
        const GCSkill* pSkill = SiGCSKT()->GetSkill(static_cast<EGCSkillTree>(GetCurFormTemplate().iOtherFaceCheckValue));
        if (pSkill) {
            if (IsLocalPlayer()) {
                if (SiGCSKT()->IsLearnedSkill(pSkill->GetID()) && GetCurrentChar().iCharType == pSkill->GetCharType()) {
                    GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName = GetCurFormTemplate().strOtherFaceTextureName;
                }
            }
            else {
                if (IsTrainedSkill(pSkill->GetID()) && GetCurrentChar().iCharType == pSkill->GetCharType()) {
                    GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName = GetCurFormTemplate().strOtherFaceTextureName;
                }
            }
        }
    }
    default:
        break;
    }
}

void PLAYER::SetMagicEffectOnlyLocal(int iID, float ftime, int iLv /*= 1 */)
{
    if (IsLocalPlayer()) {
        g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, iID, ftime, iLv);
    }
}

void PLAYER::ChangeSkillCoolTime(int iSkillID_, float fCoolTime_)
{
    if (IsLocalPlayer() == false) {
        return;
    }

    for (std::vector< GCHotKeySkill >::iterator vit = m_vecHotKeySkill.begin(); vit != m_vecHotKeySkill.end(); ++vit) {
        if (vit->m_eSkill == static_cast<int>(iSkillID_)) {
            vit->fCoolTime = fCoolTime_;
        }
    }
}


void PLAYER::FrameMoveEmbarkation()
{
    if (false == IsLocalPlayer())
        return;

    if (0 < m_dwEmbarkSendCool)
        --m_dwEmbarkSendCool;

    if (UINT_MAX == GetEmbarkUID())
        return;

    KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
    if (NULL == pEmbark)
        return;

    if (NULL == pEmbark->m_pObject)
        return;

    // 본 어댑팅이 된 상태에서만 탑승물의 움직임을 적용한다.
    if (-1 == GetEmbarkAdaptBoneID() || GetEmbarkAdaptBoneID() >= static_cast<int>(pEmbark->m_pObject->GetNumAngleBone()))
        return;

    vPos.x = pEmbark->GetPosX();
    vPos.y = pEmbark->GetPosY();
    //printf("X : %f.2, Y : %f.2\n", vPos.x, vPos.y);
    SetIsRight(pEmbark->GetIsRight());
    x_Speed = 0.0f;
    y_Speed = 0.0f;
    m_fFall_Speed = 0.0f;
}

bool PLAYER::IsEmbarkationKey()
{
    if (false == IsLocalPlayer())
        return false;

    return g_MyD3D->MyCtrl->k_keyEmbarkation;
}

KGCEmbarkObject* PLAYER::GetMyEmbarkInstance()
{
    if (UINT_MAX > GetEmbarkUID())
        return SiKGCEmbarkManager()->GetEmbarkInstance(GetEmbarkUID());
    return NULL;
};

void PLAYER::SetPermissionDamage(const std::vector<int>& vecArg)
{
    m_vecPermissionDamage.clear();
    m_vecPermissionDamage = vecArg;
}

void PLAYER::ShowMissAttack(float fHitPosX, float fHitPosY)
{
    float fOffsetX = fHitPosX - vPos.x;
    float fOffsetY = fHitPosY - vPos.y;
    AddParticleNoDirectionPosWithTrace("Miss_01", 0, fOffsetX, fOffsetY, 0.5f);
    AddParticleNoDirectionPosWithTrace("Miss_02", 0, fOffsetX, fOffsetY, 0.5f);
}

bool PLAYER::IsEndGamePlay()
{
    return (IsDetermineWin || (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() && g_pkQuestManager->m_bClearStage));
}

void PLAYER::DirectKillPlayer()
{
    SetHP(0.0f);
    m_bDirectKill = true;
    Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP);
}

void PLAYER::UseGameItem_Super(int iFrame)
{
    Item_Remain_Time[GC_GAME_ITEM_SUPER] = iFrame;
}

// 한명만 랜덤확율로 버프를 걸어주는 함수..... 쓰일 곳 있겠지...
void PLAYER::SetMagicEffectRandom(int iID, float ftime, bool bEnemyBuff /*= false*/, bool bOverlap_ /*= false*/)
{
    if (IsLocalPlayer() == false) {
        return;
    }

    std::vector< int > vecPlayerIdx;
    if (bEnemyBuff) {
        for (int i = 0; i < MAX_PLAYER_NUM; i++) {
            if (g_MyD3D->MyPlayer[i]->IsLive() && g_MyD3D->IsSameTeam(m_iPlayerIndex, g_MyD3D->MyPlayer[i]->m_iPlayerIndex) == false) {
                vecPlayerIdx.push_back(g_MyD3D->MyPlayer[i]->m_iPlayerIndex);
            }
        }
    }
    else {
        for (int i = 0; i < MAX_PLAYER_NUM; i++) {
            if (g_MyD3D->MyPlayer[i]->IsLive() && g_MyD3D->IsSameTeam(m_iPlayerIndex, g_MyD3D->MyPlayer[i]->m_iPlayerIndex)) {
                vecPlayerIdx.push_back(g_MyD3D->MyPlayer[i]->m_iPlayerIndex);
            }
        }
    }

    int iPlayerCnt = static_cast<int>(vecPlayerIdx.size());

    if (iPlayerCnt <= 0) {
        return;
    }

    int iSelectedIdx = rand() % iPlayerCnt;

    // 인덱스 넘으면 그냥 리턴하자.
    if (iSelectedIdx >= static_cast<int>(vecPlayerIdx.size())) {
        return;
    }

    // 중복 허용 아니면 리턴하자.
    if (bOverlap_ == false && g_MyD3D->MyPlayer[vecPlayerIdx[iSelectedIdx]]->IsMagicEffect(iID)) {
        return;
    }

    LTP_BUFF buff;
    buff.iBuffIndex = iID;
    buff.iTargetIndex = vecPlayerIdx[iSelectedIdx];
    buff.fBuffTime = ftime;
    buff.iBuffLv = 1;
    buff.bClearBuff = false;
    buff.iWho = m_iPlayerIndex;
    buff.dwAttackPlayerUnit = m_kUserInfo.dwUID;
    g_LatencyTransfer.PushPacket(&buff, 1);
}

void PLAYER::SetOverlayTimer(bool bEnable, float fLimitedTime /*= 0.0f*/, bool bLocal_ /*= false */)
{
    g_pkGameOverlayUI->SetLocalTimer(fLimitedTime);
}

void PLAYER::SetInGameMessage(int iStringID, int iTime_)
{
    KGCInGameMessage* pMessage = g_pkGameOverlayUI->GetInGameMessage();
    KInGameMessageAttribute kMessage;
    kMessage.iStringID = iStringID;
    kMessage.iShowFrame = static_cast<int>(iTime_) * GC_FPS_LIMIT;
    pMessage->SetMessage(&kMessage);
}

void PLAYER::ClearAllPlayerMagicEffect(int iBuffID_, bool bLocal_)
{
    if (bLocal_ == false) {
        if (IsLocalPlayer()) {
            for (int i = 0; i < MAX_PLAYER_NUM; i++) {
                g_MyD3D->MyPlayer[i]->ClearMagicEffect(iBuffID_);
            }
        }
    }
    else {
        for (int i = 0; i < MAX_PLAYER_NUM; i++) {
            g_pMagicEffect->ClearMagicEffect(i, iBuffID_);
        }
    }
}

void PLAYER::ClearMagicEffectByLocal(int iBuffID_)
{
    if (IsLocalPlayer() == false) {
        return;
    }

    g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, iBuffID_);
}

bool PLAYER::CheckInPortal()
{
    if (uiMotion == GetOwnMotion(MID_COMMON_WAIT_TELEPORT))
        return true;

    if (INT_MAX > m_iEmbarkTypeOfInPortal)
        return true;

    return false;
}

bool PLAYER::IsRemovePartsItem(REMOVEPARTS removeParts)
{
    SCharInfo& CharInfo = GetCurrentChar();
    KItemIDVector vecItemSorted = CharInfo.GetEquipmentForLoadModel(m_iPlayerIndex);

    KItemIDVector::iterator vit = vecItemSorted.begin();
    for (; vit != vecItemSorted.end(); vit++)
    {
        if (g_pItemMgr->IsRemovePartsItem(*vit, removeParts))
        {
            return true;
        }
    }

    return false;
}

REMOVEPARTS PLAYER::DontAppearParts(char charType)
{
    switch (charType)
    {
    case GC_CHAR_ZERO:
        return HEAD;
    }

    return MAX_PARTS;
}

void PLAYER::SetUnConditionToSiegFury()
{
    SCharInfo& kCharInfo = GetCurrentChar();
    if (kCharInfo.iCharType != GC_CHAR_SIEG)
        return;

    int iFuryMotionID;
    switch (((Extra_Char_Num - kCharInfo.iCharType) / GC_CHAR_NUM))
    {
    case JOB_LEVEL_0:
        if (CheckSkill(SID_SIEG1_FURY_TYPE_A_LV1) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV2) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV3))
            iFuryMotionID = MID_SIEG1_SKILL_CRAZY_FURY;
        else
            iFuryMotionID = MID_SIEG1_FURY;
        break;
    case JOB_LEVEL_1:
        if (CheckSkill(SID_SIEG1_FURY_TYPE_A_LV1) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV2) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV3))
            iFuryMotionID = MID_SIEG2_SKILL_CRAZY_FURY;
        else
            iFuryMotionID = MID_SIEG2_FURY;
        break;
    case JOB_LEVEL_2:
        if (CheckSkill(SID_SIEG1_FURY_TYPE_A_LV1) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV2) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV3))
            iFuryMotionID = MID_SIEG3_SKILL_CRAZY_FURY;
        else
            iFuryMotionID = MID_SIEG3_FURY;
        break;
    case JOB_LEVEL_3:
        if (CheckSkill(SID_SIEG1_FURY_TYPE_A_LV1) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV2) ||
            CheckSkill(SID_SIEG1_FURY_TYPE_A_LV3))
            iFuryMotionID = MID_SIEG4_SKILL_CRAZY_FURY;
        else
            iFuryMotionID = MID_SIEG4_FURY;
        break;
    default:
        return;
    }

    SetBurnningPoint(3.0f);
    ChangeMotion(iFuryMotionID);
    ChangeBurnningTemplate();
}

int PLAYER::GetPrevDamageHitWho(int iDamage_)
{
    int iTemp = -1;
    if (iDamage_ == m_iHittedPrevDamage && m_iPrevDamageHitWho != -1) {
        iTemp = m_iPrevDamageHitWho;
    }

    m_iHittedPrevDamage = -1;
    m_iPrevDamageHitWho = -1;

    return iTemp;
}

void PLAYER::SetHittedPrevDamageInfo(int iHittedPrevDamage_, int iPrevDamageHitWho_)
{
    m_iHittedPrevDamage = iHittedPrevDamage_;
    m_iPrevDamageHitWho = iPrevDamageHitWho_;
}

int PLAYER::GetEnemyAtkGuardMotion(int iMotion_)
{
    std::map< int, int >::iterator mit = GetCurFormTemplate().mapEnemyAtkGuardMotion.find(iMotion_);
    if (mit != GetCurFormTemplate().mapEnemyAtkGuardMotion.end()) {
        return mit->second;
    }

    return -1;
}

void PLAYER::SetEnemyAtkGuardMotion(int iReadyMotion_, int iPlayMotion_)
{
    GetCurFormTemplate().mapEnemyAtkGuardMotion.insert(std::make_pair(iReadyMotion_, iPlayMotion_));
}

void PLAYER::StartText(char* strText)
{
    g_MyD3D->MyHead->Start_Text(strText);
}

void PLAYER::SetEmbarkArena(kEmbarkArena* pInfo)
{
    if (NULL == pInfo)
    {
        m_kEmbarkArena.Init();
        return;
    }
    m_kEmbarkArena.m_iEmbarkType = pInfo->m_iEmbarkType;
    m_kEmbarkArena.m_bRight = pInfo->m_bRight;
    m_kEmbarkArena.m_fPosX = pInfo->m_fPosX;
    m_kEmbarkArena.m_fPosY = pInfo->m_fPosY;
    m_kEmbarkArena.m_bUseArenaZoom = pInfo->m_bUseArenaZoom;
}

bool PLAYER::IsSetEmbarkArena()
{
    return m_kEmbarkArena.IsSetting();
}

void PLAYER::AddLife()
{
    m_cLife++;
}

void PLAYER::InitDangerSque()
{
    if (m_pSqueDanger)
    {
        g_ParticleManager->DeleteSequence(m_pSqueDanger);
        m_pSqueDanger = NULL;
    }
}

void PLAYER::CreateDangerSque()
{
    InitDangerSque();
    m_pSqueDanger = g_ParticleManager->CreateSequence("UI_Danger_Red_02");
    if (m_pSqueDanger)
    {
        //m_pSqueDanger->SetLayer( GC_LAYER_CUSTOM );
        m_pSqueDanger->SetShow(false);
        TRACE_INFO stTrace;
        stTrace.m_pvPos = &g_MyD3D->MyHead->m_vCameraLookPos;
        stTrace.m_fParticleTraceTime = 5.0f;
        m_pSqueDanger->SetTrace(&stTrace);
    }
}

void PLAYER::DangerEffect()
{

    if (IsLocalPlayer())
    {
        if (m_fAbility[ABILITY_HP] * (GC_GLOBAL_DEFINE::nDangerHPPercent / 100.0f) >= m_fHP)
        {
            if (m_pSqueDanger)
            {
                if (false == m_pSqueDanger->GetShow())
                {
                    CParticleEventSeqPTR pTmp = g_ParticleManager->CreateSequence("UI_Danger_Red_01");
                    if (pTmp)
                    {
                        TRACE_INFO stTrace;
                        stTrace.m_pvPos = &g_MyD3D->MyHead->m_vCameraLookPos;
                        stTrace.m_fParticleTraceTime = 5.0f;
                        pTmp->SetTrace(&stTrace);
                        pTmp->SetShow(true);
                    }
                    m_pSqueDanger->SetShow(true);
                }
            }
        }
        else
        {
            if (m_pSqueDanger)
                m_pSqueDanger->SetShow(false);
        }
    }

}

bool PLAYER::IsInLength(int iPlayerIndex_, float fLength_)
{
    if (iPlayerIndex_ < 0 || iPlayerIndex_ >= MAX_PLAYER_NUM) {
        return false;
    }

    float fDstX = vPos.x - g_MyD3D->MyPlayer[iPlayerIndex_]->vPos.x;
    float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[iPlayerIndex_]->vPos.y));
    if (Length < fLength_) {
        return true;
    }
    else {
        return false;
    }
}

void PLAYER::DeleteDamage(int iDamageEnum_, int iPlayerIndex_)
{
    g_MyD3D->m_pDamageManager->DeleteInstance(iDamageEnum_, iPlayerIndex_);
}
CParticleEventSeqPTR PLAYER::AddParticleToChildMeshBone(char* strChlid, char* strSeqeunce, int iBoneIndex_, float fTraceTime /*= 0.5f*/, float offset_x /*= 0.0f*/, float offset_y /*= 0.0f*/, bool bNoDirection /*= false */)
{
    CParticleEventSeqPTR pParticle = KGCObj::AddParticleToChildMeshBone(strChlid, strSeqeunce, iBoneIndex_, fTraceTime, offset_x, offset_y, bNoDirection);
    if (pParticle)
    {
        m_vecChildMeshParticle.push_back(pParticle);
    }

    return pParticle;
}

void PLAYER::RenderChildMeshAllParticle(bool bRender_)
{
    m_bRenderChildMeshParticle = bRender_;

    std::vector<CParticleEventSeqPTR>::iterator vecIter = m_vecChildMeshParticle.begin();
    for (; vecIter != m_vecChildMeshParticle.end(); )
    {
        if (false == g_ParticleManager->IsLiveInstance(*vecIter)) {
            vecIter = m_vecChildMeshParticle.erase(vecIter);
            continue;
        }

        (*vecIter)->SetShow(bRender_);
        ++vecIter;
    }
}

void PLAYER::RenderChildMeshParticle(const char* strID, bool bRender_)
{
    std::vector<CParticleEventSeqPTR>::iterator vecIter = m_vecChildMeshParticle.begin();
    for (; vecIter != m_vecChildMeshParticle.end(); )
    {
        if (false == g_ParticleManager->IsLiveInstance(*vecIter)) {
            vecIter = m_vecChildMeshParticle.erase(vecIter);
            continue;
        }

        if ((*vecIter)->GetName() == strID) {
            (*vecIter)->SetShow(bRender_);
            break;
        }
        ++vecIter;
    }
}

bool PLAYER::IsRenderChildMeshParticle(const char* strID)
{
    std::vector<CParticleEventSeqPTR>::iterator vecIter = m_vecChildMeshParticle.begin();
    for (; vecIter != m_vecChildMeshParticle.end(); ++vecIter)
    {
        if ((*vecIter)->GetName() == strID) {
            if ((*vecIter)->GetShow() == true) {
                return true;
            }
            else {
                return false;
            }
        }
    }

    return false;
}

bool PLAYER::IsLiveChildMeshParticle(const char* strID)
{
    std::vector<CParticleEventSeqPTR>::iterator vecIter = m_vecChildMeshParticle.begin();
    for (; vecIter != m_vecChildMeshParticle.end(); ++vecIter)
    {
        if ((*vecIter)->GetName() == strID) {
            return g_ParticleManager->IsLiveInstance(*vecIter);
        }
    }

    return false;
}

// 세이빙 시스템 입력 체크
bool PLAYER::CheckInputSavingSystem()
{
    // 미칠듯한 조건 체크네....
    if (IsLocalPlayer() == false ||
        GetCurFormTemplate().SAVING_STANCE == uiMotion ||
        GetCurFormTemplate().SAVING_STANCE == -1 ||
        GetCurFormTemplate().SAVING_ATTACK == uiMotion ||
        GetCurFormTemplate().SAVING_EVASION == uiMotion ||
        GetCurFormTemplate().WIN == uiMotion ||
        GetCurFormTemplate().RESULT_WIN == uiMotion ||
        GetCurFormTemplate().RESULT_LOSE == uiMotion ||
        uiMotion == GetOwnMotion(MID_COMMON_INTO_TELEPORT) ||
        IsSpecialSkillMotion(uiMotion) ||
        IsCatchMotion(uiMotion) ||
        IsDamagedMotion(uiMotion) ||
        m_bSavingMotion ||
        CheckInPortal() ||
        IsLearnedSkill(GetCurFormTemplate().SAVING_SKILL_ID) == false ||
        SiKGCRoomManager()->IsMonsterGameMode() == false ||
        SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST64 ||
        g_MyD3D->m_pStateMachine->GetState() != GS_GAME ||
        g_MyD3D->EnableKeyInput() == false) {
        return false;
    }


    if (g_MyD3D->MyCtrl->k_Use_Item) {
        g_MyD3D->MyCtrl->k_Use_Item = false;
        if (g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->m_pkSavingGauge->GetRealGaugeAmount() > 0.f) {
            ChangeMotion(GetCurFormTemplate().SAVING_STANCE);
            g_KDSound.Play("60209");
        }
        else {
            g_KDSound.Play("GACHA_FAILED");
        }
        return true;
    }

    return false;
}

void PLAYER::SetSavingGaugeAmount(float fAmount_)
{
    if (g_pkUIScene) {
        g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->m_pkSavingGauge->SetGauge(fAmount_);
    }
}

void PLAYER::SetSavingStateFunc()
{
    int iSavingID = GetCurFormTemplate().SAVING_STANCE;
    if (iSavingID > 0 && iSavingID < MID_TOTAL) {
        ms_mapStateFunction[static_cast<MOTIONID>(iSavingID)] = KGCPlayerCommon::OnSavingStance;
    }

    iSavingID = m_SwapTemplate.SAVING_STANCE;
    if (iSavingID > 0 && iSavingID < MID_TOTAL) {
        ms_mapStateFunction[static_cast<MOTIONID>(iSavingID)] = KGCPlayerCommon::OnSavingStance;
    }
}

bool PLAYER::CheckTransformPetSkill()
{
    if (IsMagicEffect(EGC_EFFECT_PET_LEONA_FULL) || IsMagicEffect(EGC_EFFECT_PET_LEONA_WEAPON)) {
        return true;
    }

    return false;
}

std::pair< int, float > PLAYER::GetNextTransformPetBuff(void)
{
    if (IsMagicEffect(EGC_EFFECT_PET_LEONA_FULL)) {
        return std::make_pair(EGC_EFFECT_PET_LEONA_WEAPON, g_pMagicEffect->IsMagicEffectTimeRet(m_iPlayerIndex, EGC_EFFECT_PET_LEONA_FULL));
    }

    return std::make_pair(-1, -1.0f);
}

void PLAYER::ClearTransformPetBuffFull(void)
{
    if (IsMagicEffect(EGC_EFFECT_PET_LEONA_FULL)) {
        ClearMagicEffect(EGC_EFFECT_PET_LEONA_FULL);
    }
}

void PLAYER::SetNextTransformPetBuff(int iBuff_, float fTime_)
{
    if (iBuff_ >= 0 && fTime_ > 0.0f) {
        ClearTransformPetBuffFull();
        SetMagicEffect_NotMeBroad(iBuff_, fTime_, 0);
    }
}

bool PLAYER::IsToggleSecondBaseMesh(const char* strID_)
{
    return KGCObj::IsToggleSecondBaseMesh(strID_);
}

bool PLAYER::CheckPetTransformMotion(void)
{
    if (uiMotion == MID_COMMON_LEONA_TRANSFORM) {
        return true;
    }

    return false;
}


void PLAYER::SetCartoon(int iCartoon_)
{
    if (iCartoon_ == -1) {
        m_pObject->SetCartoon(CARTOON_DEFAULT);
    }
    else {
        m_pObject->SetCartoon(static_cast<EGCCartoonTexture>(iCartoon_));
    }
}
void PLAYER::ClearControlInfo()
{
    m_iControlInfoComboCount = 0;
    m_iControlInfoBackAttackCount = 0;
    m_iControlInfoBeAttackedCount = 0;
    m_iControlInfoRank = 0;
    m_mapControlInfoSkill.clear();
    m_mapControlInfoPotion.clear();
}

void PLAYER::AddControlInfoPotionCount(int iType_)
{
    std::map< int, int >::iterator mit = m_mapControlInfoPotion.find(iType_);
    if (mit != m_mapControlInfoPotion.end())
        mit->second += 1;
}

int PLAYER::GetControlInfoSkillCount(int iType_)
{
    std::map< int, int >::iterator mit = m_mapControlInfoSkill.find(iType_);
    if (mit != m_mapControlInfoSkill.end())
        return mit->second;
    else
        return -1;
}

int PLAYER::GetControlInfoPotionCount(int iType_)
{
    std::map< int, int >::iterator mit = m_mapControlInfoPotion.find(iType_);
    if (mit != m_mapControlInfoPotion.end())
        return mit->second;
    else
        return -1;
}

void PLAYER::SetControlInfo()
{
    const std::vector< std::pair< int, int > >& vecVirtualCount = g_kGlobalValue.m_kUserMission.GetVirtualCount();
    std::vector< std::pair< int, int > >::const_iterator vecVirCntIter = vecVirtualCount.begin();
    for (; vecVirCntIter != vecVirtualCount.end(); ++vecVirCntIter)
    {
        const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission(vecVirCntIter->first);

        if (sSubMissionInfo == NULL)
            continue;

        for (int j = 0; j < (int)sSubMissionInfo->m_vecConditionID.size(); j++)
        {
            const SConditionInfo* sCondition = SiKGCMissionManager()->GetConditionInfo(sSubMissionInfo->m_vecConditionID[j]);
            if (!sCondition)
                continue;

            if ((sCondition->m_pairMonLevelRange.first >= 0) || (sCondition->m_pairMonLevelRange.second >= 0))
            {
                int iLowLevel, iHighLevel;
                if ((sCondition->m_pairMonLevelRange.first >= 0))
                    iLowLevel = GetCurrentChar().iLevel - sCondition->m_pairMonLevelRange.first;
                else
                    iLowLevel = 0;

                if ((sCondition->m_pairMonLevelRange.second >= 0))
                    iHighLevel = GetCurrentChar().iLevel + sCondition->m_pairMonLevelRange.second;
                else
                    iHighLevel = MAX_LEVEL;

                if (!g_MyD3D->m_pkQuestGameMgr->IsQuestLevelFromInputLevel(iLowLevel, iHighLevel, SiKGCRoomManager()->GetGameMode(), SiKGCRoomManager()->GetDungeonLevel()))
                    continue;
            }
            else if (!(sCondition->m_iGameMode == SiKGCRoomManager()->GetGameMode() &&
                (sCondition->m_pairDifficultRange.first == -1 || sCondition->m_pairDifficultRange.first <= SiKGCRoomManager()->GetDungeonLevel())))
                continue;

            if (sCondition->m_iType == KGCMissionManager::TYPE_USE_POTION)
            {
                m_mapControlInfoPotion.insert(std::make_pair(sCondition->m_pairUseCount.first, 0));
            }
            else if (sCondition->m_iType == KGCMissionManager::TYPE_SKILL_COUNT)
            {
                m_mapControlInfoSkill.insert(std::make_pair(sCondition->m_pairUseCount.first, 0));
            }
            break;
        }
    }
}

void PLAYER::AddDiceParticle()
{
    AddParticleNoDirectionPosWithTrace("Dice1_01", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice1_02", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice1_03", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice1_04", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice2_01", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice2_02", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice2_03", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice2_04", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice_Effect_01", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice_Effect_02", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
    AddParticleNoDirectionPosWithTrace("Dice_Effect_03", 0, 0.0f, 0.0f, 5.0f, static_cast<int>(EPT_HEAD));
}


// 캐릭터 능력치이다. UI에서 보이는 능력치를 계산해서 준다. ( 보정 처리 및 기타 기능은 제외된다. )
void PLAYER::GetCharAbility(OUT EAbilityData& stAbilityData_)
{
    // 레벨 캐릭터 기본능력치 얻기
    EAbilityData stCharacterAbility;
    GetCharBaseAbility(stCharacterAbility);

    // 아이템 능력치를 캐릭터 능력치로 변환해서 가져온다.
    EAbilityData stEquipItemAbility;
    g_MyD3D->m_kItemMgr.GetItemRealAbility(this, GetCurrentChar().iCharType, false, stEquipItemAbility);

    // 불량유저 PC방 효과등 처리
    SetSpecialAbility(stCharacterAbility);
    SetSpecialAbility(stEquipItemAbility);

    stAbilityData_ += stCharacterAbility;
    stAbilityData_ += stEquipItemAbility;

    // 전직 레벨에 따른 보너스 능력치 표시 
    int iPromotion = static_cast<int>(GetCurrentChar().iPromotionLevel);
    int iAtk = 0;
    int iDef = 0;
    int iHP = 0;
    float fCritical = 0.0f;

    g_kGlobalValue.GetPromotionBonusStatus(iPromotion, iAtk, iDef, iHP, fCritical, this);

    stAbilityData_.fEquipItemAbility[ABILITY_ATK] += iAtk;
    stAbilityData_.fEquipItemAbility[ABILITY_DEF] += iDef;
    stAbilityData_.fEquipItemAbility[ABILITY_HP] += iHP;
    stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE] += fCritical;
}

float PLAYER::GetCharTotalAtk(const EAbilityData& stAbilityData_)
{

    int iDef = static_cast<int>(stAbilityData_.fEquipItemAbility[ABILITY_HP]);
    int iHP = static_cast<int>(stAbilityData_.fEquipItemAbility[ABILITY_DEF]);
    float fCriticalDamageRate = stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_UP] / 100.0f;
    float fCriticalRate = stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE];
    if ( fCriticalRate > 1 )
        fCriticalRate = 1;
    int iAtk = static_cast<int>(stAbilityData_.fEquipItemAbility[ABILITY_ATK]);
    int iSPAtk = static_cast<int>(stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK]);
    float fMPRecovery = stAbilityData_.fEquipItemAbility[ABILITY_MP_RECOVERY_RATE];
    float fTotalAck = (((2 - fCriticalRate) + (fCriticalRate) * (1.2f + fCriticalDamageRate)) * ((iAtk * 27) + (iDef * 25) + (iHP * 30) + ((iAtk + iSPAtk) * 20.0f) * (1 + fMPRecovery))) / 33.75f;
    return fTotalAck;
}

void PLAYER::CalcAbilityState()
{
    EAbilityData stAbilityData;
    GetCharAbility(stAbilityData);

    KDungeonCharInfo kCharAbilityState;
    kCharAbilityState.m_dwNormalAttack = static_cast<DWORD>(stAbilityData.fEquipItemAbility[ABILITY_ATK]);
    kCharAbilityState.m_dwDefence = static_cast<DWORD>(stAbilityData.fEquipItemAbility[ABILITY_DEF]);
    kCharAbilityState.m_dwVitality = static_cast<DWORD>(stAbilityData.fEquipItemAbility[ABILITY_HP]);
    kCharAbilityState.m_dwSpecialAttack = static_cast<DWORD>(stAbilityData.fEquipItemAbility[ABILITY_SPECIAL_ATK]);
    kCharAbilityState.m_dwMPDrain = static_cast<DWORD>(stAbilityData.fEquipItemAbility[ABILITY_MP_DRAIN_RATE]);
    kCharAbilityState.m_fMPRecoveryRate = stAbilityData.fEquipItemAbility[ABILITY_MP_RECOVERY_RATE];
    kCharAbilityState.m_fCounterResistRate = stAbilityData.fEquipItemAbility[ABILITY_COUNTER_DEF];
    kCharAbilityState.m_fCriticalAttackRate = stAbilityData.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE];
    kCharAbilityState.m_fCriticalDamageRate = stAbilityData.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_UP] / 100.0f;
    kCharAbilityState.m_dwElyosResist = static_cast<DWORD>(stAbilityData.fEquipItemAbility[ABILITY_ELYOS_RESIST_BUFF]);
    kCharAbilityState.m_dwTotalAttack = static_cast<DWORD>(GetCharTotalAtk(stAbilityData));
    m_vecCharAbilityState.push_back(kCharAbilityState);
}

int PLAYER::getTotalDamage() {
    return totalDamage;
}

void PLAYER::setTotalDamage(int damage) {
    this->totalDamage += damage;
}

void PLAYER::InitCurEquippedBloodChargeMotion()
{
    m_mapCurEquippedBloodChargeDamage.clear();
    std::vector< SBloodSytemInfo > vecBloodSytemInfo = GetCurFormTemplate().vecBloodSytemInfo;

    for (int i = 0; i < static_cast<int>(vecBloodSytemInfo.size()); i++) {
        std::vector< int > vecDamageList = vecBloodSytemInfo[i].m_vecDamageList;

        for (int j = 0; j < static_cast<int>(vecDamageList.size()); j++) {
            m_mapCurEquippedBloodChargeDamage.insert(std::make_pair(vecDamageList[j],
                std::make_pair(vecBloodSytemInfo[i].m_bCheckOnlyOneUnit, vecBloodSytemInfo[i].m_fBloodChargeNum)));
        }
    }
}

void PLAYER::CheckChargeBloodDamageAtk(int iDamageIndex, bool bCheckOnlyOne)
{
    if (iDamageIndex != -1) {
        std::map< int, std::pair< bool, float > >::iterator mit = m_mapCurEquippedBloodChargeDamage.find(iDamageIndex);
        if (mit != m_mapCurEquippedBloodChargeDamage.end()) {
            float fTemp = GetBloodPoint();
            fTemp += mit->second.second;
            CalcBloodPoint(fTemp);
        }
    }
}

void PLAYER::CalcBloodPoint(int bloodPoint) {
    this->m_nBloodPoint += bloodPoint;
    if (this->m_nBloodPoint > 6)
    {
        this->m_nBloodPoint = 6;
    }
}

bool PLAYER::IsTutorialMode() {
    return g_kGlobalValue.IsTutorialMode();
}

void PLAYER::DamageToSameTeam(int iDamage) {
    for (int i = 0; i < MAX_PLAYER_NUM; i++) {
        if (!g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive || g_MyD3D->MyPlayer[i]->m_cLife < 1)
            continue;

        if (g_MyD3D->IsSameTeam(i, m_iPlayerIndex)) {
            g_MyD3D->MyPlayer[i]->AddDamage(iDamage, 0, false, true);
        }
    }
}

float PLAYER::GetCorruptedBuffNumber()
{
    if (!g_MyD3D->m_pkQuestGameMgr->GetIsCorrupted())
        return 0;
    float BaseMissionCorruption = g_MyD3D->m_pkQuestGameMgr->GetCorruptedIndex();
    if ( SiKGCRoomManager()->IsHellMode() )
        BaseMissionCorruption *= 2;
    float BaseResistManip = m_fAbility[ABILITY_ELYOS_RESIST_BUFF] * 0.3f;
    float FinalManip = (BaseMissionCorruption - BaseResistManip);
    if (FinalManip < 0)
        return 0;
    return FinalManip;
}

void PLAYER::SetRenderUnoFaceLift()
{
    std::map< int, GCMeshObject* >::iterator mit = m_mapFaceLiftMeshObject.begin();
    for (; mit != m_mapFaceLiftMeshObject.end(); ++mit) {
        mit->second->m_bExtraMeshObject = true;
        mit->second->m_bRender = false;
    }
}

void PLAYER::SetCharacterCollectionAttributeList()
{
    m_vecCharacterCollectionAttributeList.clear();
    std::vector< CollectionAttribute >::iterator it;
    KCharInfoVector tempChar = m_kUserInfo.vecCharInfo;
    for ( auto element : g_kGlobalValue.m_vecCharacterCollectionList )
    {
        KCharInfoVector::iterator itemIT = find( tempChar.begin(), tempChar.end(), element.m_iCharType );
        if ( itemIT != tempChar.end() )
        {
            for ( int i = itemIT->iPromotionLevel; i >= 0; i-- )
            {
                if ( i > (int)element.m_pkPromotionArray.size() )
                    continue;
                for ( auto element2 : element.m_pkPromotionArray[i].m_vecPromotionAttributes )
                {
                    it = std::find_if( m_vecCharacterCollectionAttributeList.begin(), m_vecCharacterCollectionAttributeList.end(), [&]( const CollectionAttribute& o ) { return o.m_iAttribute == element2.m_iAttribute; } );
                    if ( it != m_vecCharacterCollectionAttributeList.end() )
                        it->m_fAttributeValue += element2.m_fAttributeValue;
                    else
                        m_vecCharacterCollectionAttributeList.push_back( element2 );
                }
            }
        }
    }
}

std::wstring PLAYER::GetCharCollectionAllStringStatus()
{
    std::wstringstream finalCollectionString;
    finalCollectionString.str(std::wstring());
    finalCollectionString.clear();
    for (auto element : m_vecCharacterCollectionAttributeList )
    {
        if ( finalCollectionString.str() != L"" )
            finalCollectionString << L"\n\n";
        float attTemp = element.m_fAttributeValue;
        if (attTemp < 1)
            attTemp *= 100;
        finalCollectionString << g_pItemMgr->GetAtrributeString( element.m_iAttribute, attTemp );
    }
    return finalCollectionString.str();
}

std::wstring PLAYER::GetCharacterCollectionStringByPromotion( int iCharType, int iPromotion )
{
    std::wstringstream promotionCollectionString;
    promotionCollectionString.str( std::wstring() );
    promotionCollectionString.clear();
    for ( auto element : g_kGlobalValue.m_vecCharacterCollectionList )
    {
        if ( element.m_iCharType == iCharType )
        {
            for ( auto element2 : element.m_pkPromotionArray[iPromotion].m_vecPromotionAttributes )
            {
                if (promotionCollectionString.str() != L"")
                    promotionCollectionString << L"\n";
                float attTemp = element2.m_fAttributeValue;
                if (attTemp < 1)
                    attTemp *= 100;
                promotionCollectionString << g_pItemMgr->GetAtrributeString( element2.m_iAttribute, attTemp );
            }
        }
    }
    return promotionCollectionString.str();
}

int PLAYER::GetTotalVIPIndex()
{
    return m_kUserInfo.m_vecFontVector.size();
}

void PLAYER::SetVIPCollectionAttributeList()
{
    int m_iVipSize = GetTotalVIPIndex();
    m_vecVIPCollectionAttributeList.clear();
    std::vector< VIPAttribute >::iterator it;
    VIPAttribute tempVIPAttr;
    std::vector<GCITEMID> tempFontVector = m_kUserInfo.m_vecFontVector;
    for (auto element : g_kGlobalValue.m_vecVIPCollectionList)
    {
        std::vector<GCITEMID>::const_iterator itemIT = find(tempFontVector.begin(), tempFontVector.end(), element.m_iFontID);
        if (itemIT != tempFontVector.end())
        {
            it = std::find_if(m_vecVIPCollectionAttributeList.begin(), m_vecVIPCollectionAttributeList.end(), [&](const VIPAttribute& o) { return o.m_iAttribute == element.m_pkCollectionAttribute.m_iAttribute; });
            if (it == m_vecVIPCollectionAttributeList.end())
            {
                tempVIPAttr.m_fAttributeValue = element.m_pkCollectionAttribute.m_fAttributeValue;
                tempVIPAttr.m_iAttribute = element.m_pkCollectionAttribute.m_iAttribute;
                tempVIPAttr.m_iFontID = element.m_iFontID;
                m_vecVIPCollectionAttributeList.push_back(tempVIPAttr);
            }
            else
                it->m_fAttributeValue += element.m_pkCollectionAttribute.m_fAttributeValue;
        }
    }
    if (m_iVipSize == VIP_COLLECTION_SIZE)
    {
        tempVIPAttr.m_fAttributeValue = 2000;
        tempVIPAttr.m_iAttribute = ATTRTYPE_DARK_DAMAGE_ATK_POINT;
        tempVIPAttr.m_iFontID = -1;
        m_vecVIPCollectionAttributeList.push_back(tempVIPAttr);
    }
}

std::wstring PLAYER::GetVIPCollectionAllStringStatus()
{
    std::wstringstream finalCollectionString;
    finalCollectionString.str(std::wstring());
    finalCollectionString.clear();
    for (int i = 0; i < (int)m_vecVIPCollectionAttributeList.size(); i++)
    {
        if (m_vecVIPCollectionAttributeList[i].m_iFontID == -1)
            continue;
        if (i > 0)
            finalCollectionString << L"\n\n";
        float attTemp = m_vecVIPCollectionAttributeList[i].m_fAttributeValue;
        if (attTemp < 1)
            attTemp *= 100;
        finalCollectionString << g_pItemMgr->GetAtrributeString(m_vecVIPCollectionAttributeList[i].m_iAttribute, attTemp);
    }
    return finalCollectionString.str();
}

std::wstring PLAYER::GetVIPCollectionExStringStatus()
{
    std::wstringstream finalCollectionString;
    std::wstring m_pkColor = L"3b3b3b";
    if (GetTotalVIPIndex() == VIP_COLLECTION_SIZE)
        m_pkColor = L"f7ca4d";
    finalCollectionString << L"#c" + m_pkColor + g_pkStrLoader->GetReplacedString( STR_ID_SET_ITEM_ATTRIBUTE_EQUIP_COUNT, "i", VIP_COLLECTION_SIZE ) + L"#cX" << L"\n";
    finalCollectionString << L"#c" + m_pkColor + g_pItemMgr->GetAtrributeString( ATTRTYPE_DARK_DAMAGE_ATK_POINT, 2000 ) + L"#cX";
    return finalCollectionString.str();
}

std::wstring PLAYER::GetVIPCollectionStringByFontID(int iFontID)
{
    std::wstringstream fontCollectionString;
    fontCollectionString.str(std::wstring());
    fontCollectionString.clear();
    for (auto element : g_kGlobalValue.m_vecVIPCollectionList)
    {
        if (element.m_iFontID == iFontID)
        {
            if (fontCollectionString.str() != L"")
                fontCollectionString << L"\n";
            float attTemp = element.m_pkCollectionAttribute.m_fAttributeValue;
            if (attTemp < 1)
                attTemp *= 100;
            fontCollectionString << g_pItemMgr->GetAtrributeString(element.m_pkCollectionAttribute.m_iAttribute, attTemp);
        }
    }
    return fontCollectionString.str();
}

#ifdef DECANEE
void PLAYER::UpdateSubjulgate()
{
    int iHPPercentage = 0;
    bool bIsSubjulgateActive = false;
    if (IsTutorialMode())
    {
        if (CheckSkill(SID_DECANEE1_PASSIVE_ADD_03))
        {
            iHPPercentage = 90;
            bIsSubjulgateActive = true;
        }
        else if (CheckSkill(SID_DECANEE1_PASSIVE_ADD_02))
        {
            iHPPercentage = 95;
            bIsSubjulgateActive = true;
        }
        else if (CheckSkill(SID_DECANEE1_PASSIVE_ADD_01))
        {
            iHPPercentage = 100;
            bIsSubjulgateActive = true;
        }
    }
    else
    {
        if (IsEquipSkill(SID_DECANEE1_PASSIVE_ADD_03))
        {
            iHPPercentage = 90;
            bIsSubjulgateActive = true;
        }
        else if (IsEquipSkill(SID_DECANEE1_PASSIVE_ADD_02))
        {
            iHPPercentage = 95;
            bIsSubjulgateActive = true;
        }
        else if (IsEquipSkill(SID_DECANEE1_PASSIVE_ADD_01))
        {
            iHPPercentage = 100;
            bIsSubjulgateActive = true;
        }
    }

    if ( bIsSubjulgateActive )
    {
        if( static_cast<int>( ( GetHP() * 100) / m_fAbility[ABILITY_HP] ) >= iHPPercentage )
        {
            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
            {
                SetMagicEffect(EGC_EFFECT_DECANEE1_SUBJECT, 999999.f);
                ClearMagicEffect(EGC_EFFECT_DECANEE1_SUBJECT_PVP);
                return;
            }
            else
            {
                SetMagicEffect(EGC_EFFECT_DECANEE1_SUBJECT_PVP, 999999.f);
                ClearMagicEffect(EGC_EFFECT_DECANEE1_SUBJECT);
                return;
            }
        }
        else
        {
            ClearMagicEffect(EGC_EFFECT_DECANEE1_SUBJECT);
            ClearMagicEffect(EGC_EFFECT_DECANEE1_SUBJECT_PVP);
        }
    }
}
#endif