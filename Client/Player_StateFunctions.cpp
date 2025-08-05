#include "stdafx.h"
#include "MyD3D.h"
#include "KGCPlayerCommon.h"
#include "KGCPlayerElesis.h"
#include "KGCPlayerElesis2.h"
#include "KGCPlayerElesis3.h"
#include "KGCPlayerElesis4.h"
#include "KGCPlayerLire.h"
#include "KGCPlayerLire2.h"
#include "KGCPlayerLire3.h"
#include "KGCPlayerLire4.h"
#include "KGCPlayerArme.h"
#include "KGCPlayerArme2.h"
#include "KGCPlayerArme3.h"
#include "KGCPlayerArme4.h"
#include "KGCPlayerLas.h"
#include "KGCPlayerLas2.h"
#include "KGCPlayerLas3.h"
#include "KGCPlayerLas4.h"
#include "KGCPlayerRyan.h"
#include "KGCFormWolf.h"
#include "KGCPlayerRyan2.h"
#include "KGCFormNephilim.h"
#include "KGCPlayerRyan3.h"
#include "KGCPlayerRyan4.h"
#include "KGCPlayerRyan4_2.h"
#include "KGCFormMDan.h"
#include "KGCTagPlayer.h"
#include "KGCPlayerRonan.h"
#include "KGCPlayerRonan2.h"
#include "KGCPlayerAmy.h"
#include "KGCPlayerAmy_2.h"
#include "KGCPlayerRonan3.h"
#include "KGCPlayerAmy2.h"
#include "KGCPlayerAmy2_2.h"
#include "KGCPlayerRonan4.h"
#include "KGCPlayerAmy3.h"
#include "KGCPlayerAmy3_2.h"
#include "KGCPlayerJin.h"
#include "KGCPlayerJinBurnning.h"
#include "KGCPlayerJin1.h"
#include "KGCPlayerJin1Burnning.h"
#include "KGCPlayerJin2.h"
#include "KGCPlayerJin2Burnning.h"
#include "KGCPlayerJin2_2.h"
#include "KGCPlayerJin2_2Burnning.h"
#include "KGCPlayerSieg.h"

void PLAYER::SetStateFunctions()
{
	for (int i = 0; i < g_pGraphicsHelper->GetPlayerMotionNum(); i++)
	{
		ms_mapStateFunction[(MOTIONID)i] = KGCPlayerCommon::OnEmpty;
	}

	//================================================================================================
	// Common motion
	//================================================================================================
	ms_mapStateFunction[MID_COMMON_DOWN_AND_STANDUP] = KGCPlayerCommon::OnDownAndStandUp;
	ms_mapStateFunction[MID_RYAN2_DOWN_AND_STAND_UP] = KGCPlayerCommon::OnDownAndStandUp;
	ms_mapStateFunction[MID_COMMON_DOWN_IN_SKY] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_COMMON_ARENA] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_COMMON_FRONTDOWN_AND_STANDUP] = KGCPlayerCommon::OnFrontDownAndStandUp;
	ms_mapStateFunction[MID_COMMON_STONECURSE] = KGCPlayerCommon::OnStoneCurse;
	ms_mapStateFunction[MID_COMMON_STONECURSE_BY_MEDUSA] = KGCPlayerCommon::OnStoneCurse;
	ms_mapStateFunction[MID_COMMON_HOLD] = KGCPlayerCommon::OnHold;
	ms_mapStateFunction[MID_COMMON_HOLD_NOEFFECT] = KGCPlayerCommon::OnHold;
	ms_mapStateFunction[MID_COMMON_HOLD_UNTIL_ATTACKED] = KGCPlayerCommon::OnHold;
	ms_mapStateFunction[MID_COMMON_DAMAGED_BY_FLYATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_COMMON_ENT_TRUNK] = KGCPlayerCommon::OnEmptyLoop;
	ms_mapStateFunction[MID_COMMON_INTO_THE_TELEPORT] = KGCPlayerCommon::OnPortal;

	ms_mapStateFunction[MID_COMMON_FREEZED_BY_PIXIE] = KGCPlayerCommon::OnFreezedByPixie;
	ms_mapStateFunction[MID_NEPHILLIM_FREEZED_BY_PIXIE] = KGCPlayerCommon::OnFreezedByPixie;
	ms_mapStateFunction[MID_COMMON_FREEZED_BY_ENT] = KGCPlayerCommon::OnFreezedByPixie;
	ms_mapStateFunction[MID_NEPHILLIM_FREEZED_BY_ENT] = KGCPlayerCommon::OnFreezedByPixie;
	ms_mapStateFunction[MID_COMMON_FREEZED_BY_LICH] = KGCPlayerCommon::OnFreezedByPixie;
	ms_mapStateFunction[MID_NEPHILLIM_FREEZED_BY_LICH] = KGCPlayerCommon::OnFreezedByPixie;
	//================================================================================================
	// ELESIS 1 시작
	//================================================================================================
	ms_mapStateFunction[MID_ELESIS_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_ELESIS_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_ELESIS_JUMP] = KGCPlayerCommon::OnJump;
	ms_mapStateFunction[MID_ELESIS_ATK1_1] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS_ATK1_2] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS_ATK1_3] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS_ATK1_4] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS_ATK1_5] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_1] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_2] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_3] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_4] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_MAGIC_DEFENSE] = KGCPlayerElesis::OnComboAtk;

	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP_ATK2] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP_ATK3] = KGCPlayerElesis::OnSkillComboAttack;

	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_DOWN] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_DOWN_ATK2] = KGCPlayerElesis::OnSkillComboAttack;

	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_UP_2] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_UP_2_ATK2] = KGCPlayerElesis::OnSkillComboAttack;

	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_DOWN_2] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_DOWN_2_ATK2] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_DOWN_2_ATK3] = KGCPlayerElesis::OnSkillComboAttack;

	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK2] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK3] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK4] = KGCPlayerElesis::OnSkillComboAttack;
	ms_mapStateFunction[MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK5] = KGCPlayerElesis::OnSkillComboAttack;

	ms_mapStateFunction[MID_ELESIS_DASH] = KGCPlayerElesis::OnDash;
	ms_mapStateFunction[MID_ELESIS_JUMPATK] = KGCPlayerElesis::OnJumpAtk;
	ms_mapStateFunction[MID_ELESIS_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS_RUN] = KGCPlayerElesis::OnRun;
	ms_mapStateFunction[MID_ELESIS_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_ELE_ELE2_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELE_ELE2_RESULT_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELE_ELE2_FATAL] = KGCPlayerCommon::OnWait;

	//================================================================================================
	// ELESIS 1 끝
	//================================================================================================
	//================================================================================================
	// LIRE 1 시작
	//================================================================================================
	ms_mapStateFunction[MID_LIRE_WAIT] = KGCPlayerLire::OnWait;
	ms_mapStateFunction[MID_LIRE_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_LIRE_DASH] = KGCPlayerLire::OnDash;
	ms_mapStateFunction[MID_LIRE_JUMP] = KGCPlayerLire::OnJump;
	ms_mapStateFunction[MID_LIRE_DOUBLEJUMP] = KGCPlayerLire::OnDoubleJump;
	ms_mapStateFunction[MID_LIRE_ATK1_1] = KGCPlayerLire::OnComboAtk;
	ms_mapStateFunction[MID_LIRE_ATK1_2] = KGCPlayerLire::OnComboAtk;
	ms_mapStateFunction[MID_LIRE_ATK1_3] = KGCPlayerLire::OnComboAtk;
	ms_mapStateFunction[MID_LIRE_ATK1_4] = KGCPlayerLire::OnComboAtk;
	ms_mapStateFunction[MID_LIRE_ATK1_5] = KGCPlayerLire::OnComboAtk;
	ms_mapStateFunction[MID_LIRE_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_LIRE_LIRE2_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE_LIRE2_RESULT_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE_LIRE2_FATAL] = KGCPlayerCommon::OnWait;

	ms_mapStateFunction[MID_SKILL_LIRE_SIEGE_WAIT] = KGCPlayerLire::OnSiegeMode;
	ms_mapStateFunction[MID_SKILL_LIRE_SIEGE_ATK] = KGCPlayerLire::OnSiegeModeAtk;
	ms_mapStateFunction[MID_SKILL_LIRE_DOUBLE_SIEGE_SHOT] = KGCPlayerLire::OnSiegeModeAtk;

	ms_mapStateFunction[MID_SKILL_LIRE_METALSHOT_LV3] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_SPECIAL2_ATK] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_CONTINUOUS_ATK_LV1] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_CONTINUOUS_ATK_LV2] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_FROZEN_ARROW] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_POISON_ARROW] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_FIRE_ARROW_LV1] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_FIRE_ARROW_LV2] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_HEADSHOT_LV1] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_HEADSHOT_LV2] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_PIERCESHOT_LV1] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_PIERCESHOT_LV2] = KGCPlayerCommon::OnMotionToChangeBodyAngle;
	ms_mapStateFunction[MID_SKILL_LIRE_DODGE_START] = KGCPlayerLire::OnDodge;
	ms_mapStateFunction[MID_SKILL_LIRE1_DASH] = KGCPlayerCommon::OnDash;
	ms_mapStateFunction[MID_LIRE_DASHATK] = KGCPlayerLire::OnDashAtk;
	ms_mapStateFunction[MID_SKILL_LIRE_JUMP_DASH] = KGCPlayerLire::OnJumpDash;
	ms_mapStateFunction[MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE] = KGCPlayerLire::OnShotMagicCircleInit;
	ms_mapStateFunction[MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_90] = KGCPlayerLire::OnShotMagicCircle;
	ms_mapStateFunction[MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_45] = KGCPlayerLire::OnShotMagicCircle;
	ms_mapStateFunction[MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_0] = KGCPlayerLire::OnShotMagicCircle;
	ms_mapStateFunction[MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_315] = KGCPlayerLire::OnShotMagicCircle;
	ms_mapStateFunction[MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_270] = KGCPlayerLire::OnShotMagicCircle;
	ms_mapStateFunction[MID_SKILL_LIRE_DAMAGE_AMPLIFIER] = KGCPlayerLire::OnDamageAmplifier;
	ms_mapStateFunction[MID_LIRE_LIRE2_FATAL] = KGCPlayerLire::OnWait;
	ms_mapStateFunction[MID_SKILL_LIRE_CONTINUOUS_COMBO_SHOT] = KGCPlayerLire::OnContinuousCombo;
	//================================================================================================
	// ARME 1 시작
	//================================================================================================
	ms_mapStateFunction[MID_ARME_WAIT_AND_FATAL] = KGCPlayerArme::OnWait;
	ms_mapStateFunction[MID_ARME_WALK] = KGCPlayerArme::OnWalk;
	ms_mapStateFunction[MID_ARME_ATK1_1] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_ARME_ATK1_2] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_ARME_ATK1_3] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_ARME_ATK1_4] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_ARME_ARME2_DASH] = KGCPlayerArme::OnDash;
	ms_mapStateFunction[MID_ARME_JUMP] = KGCPlayerArme::OnJump;
	ms_mapStateFunction[MID_ARME_JUMPATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME_SPECIAL2_BLACK] = KGCPlayerArme::OnSpecial2Black;
	ms_mapStateFunction[MID_ARME_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME_ARME2_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME_ARME2_RESULT_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;

	//  MID_ARME_SPECIAL1_BLACK = OnEmtpy;
	//  MID_ARME_SPECIAL3_BLACK = OnEmtpy;
	//  MID_ARME_SPECIAL1 = OnEmtpy;
	//  MID_ARME_SPECIAL2 = OnEmtpy;
	//  MID_ARME_SPECIAL3 = OnEmtpy;
	//  MID_ARME_DASHATK = OnEmtpy;
	//  MID_COMMON_CATCH_BY_ELESIS = OnEmtpy;
	//  MID_COMMON_CATCH_BY_LIRE = OnEmtpy;
	//  MID_COMMON_CATCH_BY_ARME = OnEmtpy;
	//  ms_mapStateFunction[MID_ELESIS_STANDUP_AND_ATK] = OnEmtpy;
	//  ms_mapStateFunction[MID_LIRE_ARME_ARME2_STANDUP_AND_ATK] = OnEmtpy;
	//  ms_mapStateFunction[MID_ELESIS_FLYATK] = OnEmpty;
	//  MID_NOUSE_81,
	//  MID_NOUSE_82,
	//  MID_NOUSE_83,
	//  MID_COMMON_SHOPPOS,                 //90//공용//乍?나오는 캐릭터 장면
	//  ms_mapStateFunction[MID_ELESIS_ARROW_REFLECTION] = OnEmtpy;

	//================================================================================================
	// ARME 1 끝
	//================================================================================================
	//================================================================================================
	// ELESIS 2 시작
	//================================================================================================
	ms_mapStateFunction[MID_ELESIS2_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS2_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_ELESIS2_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_ELESIS2_DASH] = KGCPlayerCommon::OnDash;
	ms_mapStateFunction[MID_ELESIS2_JUMP] = KGCPlayerCommon::OnJump;
	ms_mapStateFunction[MID_ELESIS2_ATK1_1] = KGCPlayerElesis2::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS2_ATK1_2] = KGCPlayerElesis2::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS2_ATK1_3] = KGCPlayerElesis2::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS2_ATK1_4] = KGCPlayerElesis2::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS2_ATK1_5] = KGCPlayerElesis2::OnComboAtk;
	ms_mapStateFunction[MID_ELESIS2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;

	//  MID_NOUSE_108,                      //108//사용안함(SLOT)//XXXXXXXXXXXXXXX
	//  MID_COMMON_CATCH_BY_ELESIS2 = OnEmtpy;
	//  ms_mapStateFunction[MID_ELE_ELE2_JOKE] = OnEmpty;
	//  MID_LIRE_LIRE2_JOKE = OnEmpty;
	//  MID_ARME_ARME2_JOKE = OnEmpty;
	//  MID_NOUSE_124,                      //124//사용안함(SLOT)//XXXXXXXXXXXXXXX

	//================================================================================================
	// ELESIS 2 끝
	//================================================================================================
	//================================================================================================
	// LIRE2 시작
	//================================================================================================
	ms_mapStateFunction[MID_LIRE2_WAIT] = KGCPlayerLire2::OnWait;
	ms_mapStateFunction[MID_LIRE2_WALK] = KGCPlayerCommon::OnWalk;

	ms_mapStateFunction[MID_LIRE2_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE2_JUMPREADY] = KGCPlayerLire2::OnJumpReady;
	ms_mapStateFunction[MID_LIRE2_JUMP] = KGCPlayerLire2::OnJump;
	ms_mapStateFunction[MID_LIRE2_SPECIAL1_DOWN] = KGCPlayerLire2::OnSpecial1;
	ms_mapStateFunction[MID_LIRE2_SPECIAL1_UP] = KGCPlayerLire2::OnSpecial1;
	ms_mapStateFunction[MID_LIRE2_JUMPPING_DOWN_ARROW_ATK] = KGCPlayerLire2::OnJumppingDownArrowAtk;
	ms_mapStateFunction[MID_LIRE2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_SKILL_LIRE2_DASH] = KGCPlayerLire2::OnDash;

	//================================================================================================
	// LIRE2 끝
	//================================================================================================
	//================================================================================================
	// ARME 2 시작
	//================================================================================================
	ms_mapStateFunction[MID_ARME2_WAIT_AND_FATAL] = KGCPlayerArme2::OnWait;
	ms_mapStateFunction[MID_ARME2_WALK] = KGCPlayerArme2::OnWalk;
	ms_mapStateFunction[MID_ARME2_ATK1_1] = KGCPlayerArme2::OnComboAtk;
	ms_mapStateFunction[MID_ARME2_ATK1_2] = KGCPlayerArme2::OnComboAtk;
	ms_mapStateFunction[MID_ARME2_ATK1_3] = KGCPlayerArme2::OnComboAtk;
	ms_mapStateFunction[MID_ARME2_ATK1_4] = KGCPlayerArme2::OnComboAtk;
	ms_mapStateFunction[MID_ARME2_ATK1_5] = KGCPlayerArme2::OnComboAtk;
	ms_mapStateFunction[MID_ARME2_JUMP] = KGCPlayerArme::OnJump;
	ms_mapStateFunction[MID_ARME2_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;

	//  MID_NOUSE_179,                      //179//사용안함(SLOT)//XXXXXXXXXXXXXXX
	//  MID_COMMON_INTROPOS,                //184//공용//게임 스타시에 나오는 캐릭터 장면

	//================================================================================================
	// ARME 2 끝
	//================================================================================================
	//================================================================================================
	// LAS 시작
	//================================================================================================
	ms_mapStateFunction[MID_LAS_WAIT] = KGCPlayerLas::OnWait;
	ms_mapStateFunction[MID_LAS_FATAL] = KGCPlayerLas::OnWait;
	ms_mapStateFunction[MID_LAS_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_LAS_DASH] = KGCPlayerLas::OnDash;
	ms_mapStateFunction[MID_LAS_RUN] = KGCPlayerLas::OnRun;
	ms_mapStateFunction[MID_SKILL_LAS1_DASH] = KGCPlayerLas::OnDash;
	ms_mapStateFunction[MID_SKILL_LAS1_RUN] = KGCPlayerLas::OnRun;
	ms_mapStateFunction[MID_LAS_JUMP] = KGCPlayerCommon::OnJump;
	ms_mapStateFunction[MID_LAS_JUMPATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LAS_DOUBLEJUMP] = KGCPlayerLas::OnDoubleJump;
	ms_mapStateFunction[MID_LAS_DOUBLEJUMP_ATK] = KGCPlayerLas::OnJumpAtk;
	ms_mapStateFunction[MID_LAS_ATK1_1] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_LAS_ATK1_2] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_LAS_ATK1_3] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_LAS_DASHATK1] = KGCPlayerLas::OnDashCombo1;
	ms_mapStateFunction[MID_LAS_DASHATK2] = KGCPlayerLas::OnDashCombo1;
	ms_mapStateFunction[MID_LAS_DASHATK3] = KGCPlayerLas::OnDashCombo1;
	ms_mapStateFunction[MID_LAS_DASHATK4] = KGCPlayerLas::OnDashCombo1;
	ms_mapStateFunction[MID_LAS_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;

	//ms_mapStateFunction[MID_LAS_SPECIAL3]                        = KGCPlayerCommon::OnEmptyNoContactCheck;
	ms_mapStateFunction[MID_LAS_EVASION] = KGCPlayerLas::OnEvasion;
	ms_mapStateFunction[MID_LAS_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LAS_RESULT_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_SKILL_LAS_SPININGSLASH_LV1] = KGCPlayerLas::OnSpiningSlash;
	ms_mapStateFunction[MID_SKILL_LAS_SPININGSLASH_LV2] = KGCPlayerLas::OnSpiningSlash;
	//ms_mapStateFunction[MID_SKILL_LAS_THUNDER_SLASH_START]		 = KGCPlayerLas::OnThunderSlashInit;
	//ms_mapStateFunction[MID_SKILL_LAS_THUNDER_SLASH_UP]			 = KGCPlayerLas::OnThunderSlash;
	//ms_mapStateFunction[MID_SKILL_LAS_THUNDER_SLASH_MIDDLE]		 = KGCPlayerLas::OnThunderSlash;
	//ms_mapStateFunction[MID_SKILL_LAS_THUNDER_SLASH_DOWN]		 = KGCPlayerLas::OnThunderSlash;
	ms_mapStateFunction[MID_SKILL_LAS_DASHATK_LV3_START] = KGCPlayerLas::OnNewDashAtk;
	ms_mapStateFunction[MID_SKILL_LAS_JUMP_ATK_LV2] = KGCPlayerLas::OnJumpAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK1_1] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK1_2] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK1_3] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK1_4] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK1_5] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_DROPPING] = KGCPlayerLas::OnDropping;
	ms_mapStateFunction[MID_SKILL_LAS_ATK2_1] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK2_2] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK2_3] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK2_4] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_ATK2_5] = KGCPlayerLas::OnNewComboAtk;
	ms_mapStateFunction[MID_SKILL_LAS_FLYATK_LV1] = KGCPlayerCommon::OnContinueousCombo;
	ms_mapStateFunction[MID_SKILL_LAS_FLYATK_LV2_1] = KGCPlayerCommon::OnContinueousCombo;
	ms_mapStateFunction[MID_SKILL_LAS_FLYATK_LV2_2] = KGCPlayerCommon::OnContinueousCombo;
	ms_mapStateFunction[MID_SKILL_LAS_EVASION_LV1] = KGCPlayerLas::OnDodge;
	ms_mapStateFunction[MID_SKILL_LAS_EVASION_LV2] = KGCPlayerLas::OnDodge;
	ms_mapStateFunction[MID_SKILL_LAS_TRADWARM_ATK] = KGCPlayerLas::OnTradWarm;
	ms_mapStateFunction[MID_SKILL_LAS_SUPER_JUMP] = KGCPlayerLas::OnSuperJump;
	//======================================================================================
	//                                  라스 전직
	//======================================================================================
	ms_mapStateFunction[MID_LAS2_WAIT] = KGCPlayerLas2::OnWait;    //완료
	ms_mapStateFunction[MID_LAS2_FATAL] = KGCPlayerLas2::OnWait;    //완료
	ms_mapStateFunction[MID_LAS2_WALK] = KGCPlayerCommon::OnWalk;    //완료
	ms_mapStateFunction[MID_LAS2_DASH_1] = KGCPlayerLas2::OnDash;      //완료
	ms_mapStateFunction[MID_LAS2_DASH_2] = KGCPlayerLas2::OnDash;      //완료
	ms_mapStateFunction[MID_SKILL_LAS2_DASH] = KGCPlayerLas2::OnDash; //완료
	ms_mapStateFunction[MID_SKILL_LAS2_JUMPDASH] = KGCPlayerLas2::OnDash; //완료
	ms_mapStateFunction[MID_SKILL_LAS2_DOUBLEDASH] = KGCPlayerLas2::OnDash; //완료
	ms_mapStateFunction[MID_LAS2_JUMP] = KGCPlayerCommon::OnJump;    //완료
	ms_mapStateFunction[MID_LAS2_JUMPATK] = KGCPlayerCommon::OnEmptyEndFrameLock;//완료
	ms_mapStateFunction[MID_LAS2_ATK1_1] = KGCPlayerLas2::OnComboAtk;  //완료
	ms_mapStateFunction[MID_LAS2_ATK1_2] = KGCPlayerLas2::OnComboAtk;  //완료
	ms_mapStateFunction[MID_LAS2_ATK1_3] = KGCPlayerLas2::OnComboAtk;  //완료
	ms_mapStateFunction[MID_LAS2_ATK1_4] = KGCPlayerLas2::OnComboAtk;  //완료
	ms_mapStateFunction[MID_LAS2_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;//완료
	ms_mapStateFunction[MID_LAS2_RESULT_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;//완료
	ms_mapStateFunction[MID_LAS2_JUMPDASH] = KGCPlayerLas2::OnDash;      //완료
	ms_mapStateFunction[MID_LAS2_ARROW_EVASION_SKY] = KGCPlayerLas2::OnArrowEvasion;
	ms_mapStateFunction[MID_LAS2_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;//완료
	ms_mapStateFunction[MID_LAS2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;//완료
	ms_mapStateFunction[MID_LAS2_ARROW_EVASION_GROUND] = KGCPlayerLas2::OnArrowEvasion; //완료
	ms_mapStateFunction[MID_SKILL_LAS2_SUPER_JUMP] = KGCPlayerLas::OnSuperJump;
	//======================================================================================
	//                                  엘리시스 2차 전직
	//======================================================================================
	ms_mapStateFunction[MID_ELESIS3_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_ELESIS3_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_ELESIS3_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_ELESIS3_DASH] = KGCPlayerCommon::OnDash;
	ms_mapStateFunction[MID_ELESIS3_RUN] = KGCPlayerElesis3::OnRun;//*
	ms_mapStateFunction[MID_ELESIS3_JUMP] = KGCPlayerElesis3::OnJump;//*
	ms_mapStateFunction[MID_ELESIS3_JUMPATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS3_JUMPATK_UP] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS3_JUMPATK_DOWN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS3_ATK1_1] = KGCPlayerElesis3::OnComboAtk;//*
	ms_mapStateFunction[MID_ELESIS3_ATK1_2] = KGCPlayerElesis3::OnComboAtk;//*
	ms_mapStateFunction[MID_ELESIS3_ATK1_3] = KGCPlayerElesis3::OnComboAtk;//*
	ms_mapStateFunction[MID_ELESIS3_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS3_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	//======================================================================================
	//                                  리르 2차 전직
	//======================================================================================
	ms_mapStateFunction[MID_LIRE3_WAIT] = KGCPlayerLire3::OnWait;
	ms_mapStateFunction[MID_LIRE3_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_LIRE3_DASH] = KGCPlayerLire3::OnDash;
	ms_mapStateFunction[MID_LIRE3_RUN] = KGCPlayerLire3::OnRun;
	ms_mapStateFunction[MID_LIRE3_DASHATK] = KGCPlayerLire3::OnDashAtk1;
	ms_mapStateFunction[MID_LIRE3_JUMP] = KGCPlayerLire3::OnJump;
	ms_mapStateFunction[MID_LIRE3_ATK1_1] = KGCPlayerLire3::OnComboAtk;
	ms_mapStateFunction[MID_LIRE3_ATK1_2] = KGCPlayerLire3::OnComboAtk;
	ms_mapStateFunction[MID_LIRE3_ATK1_3] = KGCPlayerLire3::OnComboAtk;
	ms_mapStateFunction[MID_LIRE3_JUMPATK1] = KGCPlayerLire3::OnJumpAtk;
	ms_mapStateFunction[MID_LIRE3_JUMPATK2] = KGCPlayerLire3::OnJumpAtk;
	ms_mapStateFunction[MID_LIRE3_SPECIAL2] = KGCPlayerLire3::OnSpecial2;
	ms_mapStateFunction[MID_LIRE3_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE3_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE3_FATAL] = KGCPlayerLire3::OnWait;
	ms_mapStateFunction[MID_LIRE3_JUMPATK3] = KGCPlayerLire3::OnJumpAtk;
	ms_mapStateFunction[MID_LIRE3_CRITICALATK] = KGCPlayerLire3::OnDashAtk1;
	ms_mapStateFunction[MID_LIRE3_DOUBLEATK] = KGCPlayerLire3::OnDashAtk1;
	ms_mapStateFunction[MID_LIRE3_JUMP_DASH] = KGCPlayerLire3::OnJumpDash;

	ms_mapStateFunction[MID_SKILL_LIRE3_DASH] = KGCPlayerLire3::OnDash;
	//======================================================================================
	//                                  리르 2차 전직 끝
	//======================================================================================
	//======================================================================================
	//                                  아르메 2차 전직
	//======================================================================================
	ms_mapStateFunction[MID_ARME3_WAIT] = KGCPlayerArme3::OnWait;
	ms_mapStateFunction[MID_ARME3_FATAL] = KGCPlayerArme3::OnWait;
	ms_mapStateFunction[MID_ARME3_WALK] = KGCPlayerArme3::OnWalk;
	ms_mapStateFunction[MID_ARME3_JUMPREADY] = KGCPlayerArme3::OnJumpReady;
	ms_mapStateFunction[MID_ARME3_JUMP] = KGCPlayerArme3::OnJump;
	ms_mapStateFunction[MID_ARME3_SUMMON_MOVE] = KGCPlayerArme3::OnWalk;
	ms_mapStateFunction[MID_ARME3_SUMMON_WAIT] = KGCPlayerArme3::OnWait;
	ms_mapStateFunction[MID_ARME3_UNSUMMON] = KGCPlayerArme3::OnUnSummon;
	ms_mapStateFunction[MID_ARME3_DASHATK1] = KGCPlayerArme3::OnDashCombo1;
	ms_mapStateFunction[MID_ARME3_DASHATK2] = KGCPlayerArme3::OnDashCombo2;
	ms_mapStateFunction[MID_ARME3_ATK1READY] = KGCPlayerArme3::OnAtkReady;
	ms_mapStateFunction[MID_ARME3_ATK2READY] = KGCPlayerArme3::OnAtkReady;
	ms_mapStateFunction[MID_ARME3_SPECIAL1] = KGCPlayerArme3::OnSpecial1;
	ms_mapStateFunction[MID_ARME3_SPECIAL2] = KGCPlayerArme3::OnSpecial2;
	ms_mapStateFunction[MID_ARME3_SPECIAL3] = KGCPlayerArme3::OnSpecial3;
	ms_mapStateFunction[MID_ARME3_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME3_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME3_SUMMON_JUMP] = KGCPlayerArme3::OnJump;
	KGCPlayerArme3::m_pSummonfunc[0] = KGCPlayerArme3::OnNormal;
	KGCPlayerArme3::m_pSummonfunc[1] = KGCPlayerArme3::OnLowGradeSummon;
	KGCPlayerArme3::m_pSummonfunc[2] = KGCPlayerArme3::OnMiddleGradeSummon;

	//ms_mapStateFunction[MID_ARME3_DASH] = KGCPlayerArme3::OnDash;

	//======================================================================================
	//                                  아르메 2차 전직 끝
	//======================================================================================
	//======================================================================================
	//                                  라스 2차 전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_LAS3_WAIT] = KGCPlayerLas3::OnWait;
	ms_mapStateFunction[MID_LAS3_FATAL] = KGCPlayerLas3::OnWait;
	ms_mapStateFunction[MID_LAS3_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_LAS3_DASH] = KGCPlayerCommon::OnDash;
	ms_mapStateFunction[MID_LAS3_DASH_ATK2] = KGCPlayerLas3::OnDashCombo2;
	ms_mapStateFunction[MID_LAS3_JUMP] = KGCPlayerLas3::OnJump;
	ms_mapStateFunction[MID_LAS3_UP_JUMPDASH] = KGCPlayerLas3::OnJumpDash;
	ms_mapStateFunction[MID_LAS3_FRONT_JUMPDASH] = KGCPlayerLas3::OnJumpDash;
	ms_mapStateFunction[MID_LAS3_DOWN_JUMPDASH] = KGCPlayerLas3::OnJumpDash;
	ms_mapStateFunction[MID_LAS3_LANDING] = KGCPlayerLas3::OnLanding;
	ms_mapStateFunction[MID_LAS3_ATK1] = KGCPlayerLas3::OnCombo1;
	ms_mapStateFunction[MID_LAS3_ATK2] = KGCPlayerLas3::OnCombo2;
	ms_mapStateFunction[MID_LAS3_ATK3] = KGCPlayerLas3::OnCombo3;
	ms_mapStateFunction[MID_LAS3_DASH_ATK] = KGCPlayerLas3::OnDashCombo1;
	ms_mapStateFunction[MID_LAS3_JUMP_ATK] = KGCPlayerLas3::OnJumpAtk;
	ms_mapStateFunction[MID_LAS3_SPECIAL1] = KGCPlayerLas3::OnSpecial1;
	ms_mapStateFunction[MID_LAS3_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_SKILL_LAS3_SUPER_JUMP] = KGCPlayerLas::OnSuperJump;

	//======================================================================================
	//                                  라스 2차 전직 끝
	//======================================================================================
	//======================================================================================
	//                                    라이언 시작
	//======================================================================================
	ms_mapStateFunction[MID_RYAN_WAIT] = KGCPlayerRyan::OnWait;
	ms_mapStateFunction[MID_RYAN_FATAL] = KGCPlayerRyan::OnWait;
	ms_mapStateFunction[MID_RYAN_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_RYAN_DASH] = KGCPlayerRyan::OnDash;
	ms_mapStateFunction[MID_RYAN_JUMP] = KGCPlayerRyan::OnJump;
	ms_mapStateFunction[MID_RYAN_ATK1] = KGCPlayerRyan::OnCombo1;
	ms_mapStateFunction[MID_RYAN_ATK2] = KGCPlayerRyan::OnCombo1;
	ms_mapStateFunction[MID_RYAN_ATK3] = KGCPlayerRyan::OnCombo1;
	ms_mapStateFunction[MID_RYAN_ATK4] = KGCPlayerRyan::OnCombo1;
	ms_mapStateFunction[MID_SKILL_RYAN_COMBO_UP_1] = KGCPlayerRyan::OnDashAtk;
	ms_mapStateFunction[MID_RYAN_DASH_ATK1] = KGCPlayerRyan::OnDashAtk;
	ms_mapStateFunction[MID_RYAN_JUMP_ATK1] = KGCPlayerRyan::OnJumpAtk1;
	ms_mapStateFunction[MID_RYAN_JUMP_ATK2] = KGCPlayerRyan::OnJumpAtk2;
	ms_mapStateFunction[MID_RYAN_JUMP_ATK2_LAND] = KGCPlayerRyan::OnJumpAtk2Land;
	ms_mapStateFunction[MID_RYAN_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_WOLF_FORM_START] = KGCPlayerRyan::OnFormChange;
	ms_mapStateFunction[MID_SKILL_RYAN_JUMP_DASH] = KGCPlayerRyan::OnDash;
	ms_mapStateFunction[MID_SKILL_RYAN_JUMP_DASH_TO_WAIT] = KGCPlayerRyan::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_SKILL_RYAN_JUMP_DASH_TO_JUMP] = KGCPlayerRyan::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_SKILL_RYAN_DASHATK_CANCEL] = KGCPlayerRyan::OnDashAtkCancel;
	ms_mapStateFunction[MID_SKILL_RYAN_CHARGING_ING] = KGCPlayerRyan::OnChargeX;
	ms_mapStateFunction[MID_SKILL_RYAN2_CHARGING_ING] = KGCPlayerRyan::OnChargeX;
	ms_mapStateFunction[MID_SKILL_RYAN3_CHARGING_ING] = KGCPlayerRyan::OnChargeX;
	ms_mapStateFunction[MID_SKILL_RYAN4_1_CHARGING_ING] = KGCPlayerRyan::OnChargeX;
	ms_mapStateFunction[MID_SKILL_RYAN4_2_CHARGING_ING] = KGCPlayerRyan::OnChargeX;
	ms_mapStateFunction[MID_SKILL_RYAN_NATURESHIELD] = KGCPlayerRyan::OnNatureShield;
	ms_mapStateFunction[MID_SKILL_RYAN_NATURESHIELD_STANDUP] = KGCPlayerRyan::OnNatureShield;
	ms_mapStateFunction[MID_SKILL_RYAN2_NATURESHIELD_STANDUP] = KGCPlayerRyan::OnNatureShield;
	ms_mapStateFunction[MID_SKILL_RYAN3_NATURESHIELD_STANDUP] = KGCPlayerRyan::OnNatureShield;
	ms_mapStateFunction[MID_SKILL_RYAN4_NATURESHIELD_STANDUP] = KGCPlayerRyan::OnNatureShield;
	ms_mapStateFunction[MID_SKILL_RYAN4_2_NATURESHIELD_STANDUP] = KGCPlayerRyan::OnNatureShield;
	ms_mapStateFunction[MID_SKILL_RYAN_COMBO_1] = KGCPlayerRyan::OnCombo1;
	ms_mapStateFunction[MID_SKILL_RYAN_COMBO_2] = KGCPlayerRyan::OnCombo1;
	ms_mapStateFunction[MID_SKILL_RYAN_COMBO_3] = KGCPlayerRyan::OnCombo1;
	ms_mapStateFunction[MID_SKILL_RYAN_RUN] = KGCPlayerRyan::OnRun;
	ms_mapStateFunction[MID_SKILL_RYAN_DASH] = KGCPlayerRyan::OnDash;

	//======================================================================================
	//                                     라이언 끝
	//======================================================================================
	//======================================================================================
	//                                     라이언 늑대 시작
	//======================================================================================
	ms_mapStateFunction[MID_WOLF_FORM_END] = KGCFormWolf::OnWolfFormEnd;
	ms_mapStateFunction[MID_FORM_WOLF_WAIT] = KGCFormWolf::OnWait;
	ms_mapStateFunction[MID_FORM_WOLF_FATAL] = KGCFormWolf::OnWait;
	ms_mapStateFunction[MID_FORM_WOLF_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_FORM_WOLF_DASH] = KGCFormWolf::OnDash;
	ms_mapStateFunction[MID_FORM_WOLF_JUMP] = KGCFormWolf::OnJump;
	ms_mapStateFunction[MID_FORM_WOLF_ATK1] = KGCFormWolf::OnCombo1;
	ms_mapStateFunction[MID_FORM_WOLF_ATK2] = KGCFormWolf::OnCombo1;
	ms_mapStateFunction[MID_FORM_WOLF_ATK3] = KGCFormWolf::OnCombo1;
	ms_mapStateFunction[MID_FORM_WOLF_ATK4] = KGCFormWolf::OnCombo1;
	ms_mapStateFunction[MID_FORM_WOLF_JUMP_ATK2] = KGCFormWolf::OnJumpAtk2;
	//======================================================================================
	//                                     라이언 늑대 끝
	//======================================================================================
	//======================================================================================
	//                                     라이언 1차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_RYAN2_WAIT] = KGCPlayerRyan2::OnWait;
	ms_mapStateFunction[MID_RYAN2_FATAL] = KGCPlayerRyan2::OnWait;
	ms_mapStateFunction[MID_RYAN2_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_RYAN2_DASH] = KGCPlayerRyan2::OnDash;
	ms_mapStateFunction[MID_RYAN2_JUMP] = KGCPlayerRyan2::OnJump;
	ms_mapStateFunction[MID_RYAN2_ATK1] = KGCPlayerRyan2::OnCombo;
	ms_mapStateFunction[MID_RYAN2_ATK2] = KGCPlayerRyan2::OnCombo;
	ms_mapStateFunction[MID_RYAN2_ATK3] = KGCPlayerRyan2::OnCombo;
	ms_mapStateFunction[MID_RYAN2_ATK4] = KGCPlayerRyan2::OnCombo;
	ms_mapStateFunction[MID_RYAN2_ATK5] = KGCPlayerRyan2::OnCombo;
	ms_mapStateFunction[MID_RYAN2_ATK6] = KGCPlayerRyan2::OnCombo;
	ms_mapStateFunction[MID_RYAN2_DASH_ATK] = KGCPlayerRyan2::OnDashAtk;
	ms_mapStateFunction[MID_RYAN2_JUMP_ATK1] = KGCPlayerRyan2::OnJumpAtk1;
	ms_mapStateFunction[MID_RYAN2_JUMP_ATK2] = KGCPlayerRyan2::OnJumpAtk2;
	//ms_mapStateFunction[ MID_RYAN2_JUMP_DASH_ATK ]               = KGCPlayerRyan2::OnJumpDashAtk;
	ms_mapStateFunction[MID_RYAN2_SPECIAL3] = KGCPlayerRyan2::OnSpecial3;
	ms_mapStateFunction[MID_RYAN2_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_NEPHILLIM_FORM_START] = KGCPlayerRyan2::OnFormChange;
	ms_mapStateFunction[MID_RYAN2_RUN] = KGCPlayerRyan2::OnRun;
	ms_mapStateFunction[MID_RYAN2_JUMP_DASH] = KGCPlayerRyan2::OnJumpDash;
	//======================================================================================
//                                     라이언 1차전직 끝
//======================================================================================
//======================================================================================
//                                     네피림 변신 시작
//======================================================================================
	ms_mapStateFunction[MID_NEPHILLIM_WAIT] = KGCFormNephilim::OnWait;
	ms_mapStateFunction[MID_NEPHILLIM_FATAL] = KGCFormNephilim::OnWait;
	ms_mapStateFunction[MID_NEPHILLIM_WALK] = KGCFormNephilim::OnWalk;
	ms_mapStateFunction[MID_NEPHILLIM_DASH1] = KGCFormNephilim::OnDash;
	ms_mapStateFunction[MID_NEPHILLIM_JUMP] = KGCFormNephilim::OnJump;
	ms_mapStateFunction[MID_NEPHILLIM_LAND] = KGCFormNephilim::OnLand;
	ms_mapStateFunction[MID_NEPHILLIM_JUMP_ATK1] = KGCFormNephilim::OnJumpAtk1;
	ms_mapStateFunction[MID_NEPHILLIM_JUMP_ATK2] = KGCFormNephilim::OnJumpAtk2;
	ms_mapStateFunction[MID_NEPHILLIM_JUMP_ATK3] = KGCFormNephilim::OnJumpAtk3;
	ms_mapStateFunction[MID_NEPHILLIM_JUMP_ATK4] = KGCFormNephilim::OnJumpAtk4;
	ms_mapStateFunction[MID_NEPHILLIM_JUMP_DASH] = KGCFormNephilim::OnJumpDash;
	ms_mapStateFunction[MID_NEPHILLIM_JUMP_DASH_ATK] = KGCFormNephilim::OnJumpDashAtk;
	ms_mapStateFunction[MID_NEPHILLIM_ATK1] = KGCFormNephilim::OnCombo;
	ms_mapStateFunction[MID_NEPHILLIM_ATK2] = KGCFormNephilim::OnCombo;
	ms_mapStateFunction[MID_NEPHILLIM_ATK3] = KGCFormNephilim::OnCombo;
	ms_mapStateFunction[MID_NEPHILLIM_ATK4] = KGCFormNephilim::OnCombo;
	ms_mapStateFunction[MID_NEPHILLIM_ATK5] = KGCFormNephilim::OnCombo;
	ms_mapStateFunction[MID_NEPHILLIM_FORM_END] = KGCFormNephilim::OnNephilimFormEnd;
	ms_mapStateFunction[MID_NEPHILLIM_RUN] = KGCFormNephilim::OnRun;
	ms_mapStateFunction[MID_NEPHILLIM_STONE_CURSE] = KGCPlayerCommon::OnStoneCurse;
	//ms_mapStateFunction[ MID_NEPHILLIM_TELEPORT ]                = KGCPlayerCommon::OnPortal;
	//ms_mapStateFunction[ MID_NEPHILLIM_INTO_BASILISK ]           = KGCPlayerCommon::OnIntoTheBasilisk;
	//======================================================================================
	//                                     네피림 변신 끝
	//======================================================================================
	//======================================================================================
	//                                     태그매치 모션들
	//======================================================================================
	ms_mapStateFunction[MID_TAG_ELESIS_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_ELESIS_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_LIRE_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_LIRE_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_ARME_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_ARME_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_LAS_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_LAS_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_RYAN_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_RYAN_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_WOLF_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_WOLF_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_NEPHILLIM_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_NEPHILLIM_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_RYAN4_MDAN_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_RYAN4_MDAN_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_AMY1_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_AMY1_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_AMY1_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_TAG_AMY2_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_AMY2_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_RIN1_TAG_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_RIN1_TAG_START] = KGCTagPlayer::OnTagStart;
	//======================================================================================
	//                                     태그매치 모션들 끝
	//======================================================================================

	//======================================================================================
	//                                     엘리시스 3차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_ELESIS4_WAIT] = KGCPlayerElesis4::OnWait;
	ms_mapStateFunction[MID_ELESIS4_FATAL] = KGCPlayerElesis4::OnWait;
	ms_mapStateFunction[MID_ELESIS4_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_ELESIS4_RUN] = KGCPlayerElesis4::OnRun;
	ms_mapStateFunction[MID_ELESIS4_DASH] = KGCPlayerElesis4::OnDash;
	ms_mapStateFunction[MID_ELESIS4_JUMP] = KGCPlayerElesis4::OnJump;
	ms_mapStateFunction[MID_ELESIS4_JUMP_DASH] = KGCPlayerElesis4::OnJumpDash;
	ms_mapStateFunction[MID_ELESIS4_ATK1] = KGCPlayerElesis4::OnCombo1;
	ms_mapStateFunction[MID_ELESIS4_ATK2] = KGCPlayerElesis4::OnCombo;
	ms_mapStateFunction[MID_ELESIS4_ATK3] = KGCPlayerElesis4::OnCombo;
	ms_mapStateFunction[MID_ELESIS4_ATK4] = KGCPlayerElesis4::OnCombo;
	ms_mapStateFunction[MID_ELESIS4_ATK5] = KGCPlayerElesis4::OnCombo;
	ms_mapStateFunction[MID_ELESIS4_BACK_STEP] = KGCPlayerElesis4::OnBackStep;
	ms_mapStateFunction[MID_ELESIS4_JUMP_ATK1] = KGCPlayerElesis4::OnJumpAtk1;
	ms_mapStateFunction[MID_ELESIS4_JUMP_ATK2] = KGCPlayerElesis4::OnJumpAtk2;
	ms_mapStateFunction[MID_ELESIS4_JUMP_ATK3] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS4_JUMP_ATK5] = KGCPlayerElesis4::OnJumpAtk5;
	ms_mapStateFunction[MID_ELESIS4_JUMP_ATK6] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS4_SPECIAL2] = KGCPlayerElesis4::OnSpecial2;
	ms_mapStateFunction[MID_ELESIS4_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ELESIS4_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;

	//ms_mapStateFunction[ MID_ELESIS4_DASH_ATK ]                  = KGCPlayerElesis4::OnDashAtk;

	//======================================================================================
	//                                     엘리시스 3차전직 끝
	//======================================================================================
	//======================================================================================
	//                                     아르메 3차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_ARME4_WAIT] = KGCPlayerArme4::OnWait;
	ms_mapStateFunction[MID_ARME4_WALK] = KGCPlayerArme4::OnWalk;
	ms_mapStateFunction[MID_ARME4_FATAL] = KGCPlayerArme4::OnWait;
	ms_mapStateFunction[MID_ARME4_ATK1] = KGCPlayerArme4::OnCombo;
	ms_mapStateFunction[MID_ARME4_ATK2] = KGCPlayerArme4::OnCombo;
	ms_mapStateFunction[MID_ARME4_ATK3] = KGCPlayerArme4::OnCombo;
	ms_mapStateFunction[MID_ARME4_JUMP] = KGCPlayerArme4::OnJump;
	ms_mapStateFunction[MID_ARME4_JUMP_ATK1] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME4_JUMP_ATK2] = KGCPlayerArme4::OnJumpAtk2;
	ms_mapStateFunction[MID_ARME4_DASH] = KGCPlayerArme4::OnDash;
	ms_mapStateFunction[MID_ARME4_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_ARME4_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	//======================================================================================
	//                                     아르메 3차전직 끝
	//======================================================================================
	//======================================================================================
	//                                     리르 3차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_LIRE4_WAIT] = KGCPlayerLire4::OnWait;
	ms_mapStateFunction[MID_LIRE4_FATAL] = KGCPlayerLire4::OnWait;
	ms_mapStateFunction[MID_LIRE4_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_LIRE4_DASH] = KGCPlayerLire4::OnDash;
	ms_mapStateFunction[MID_LIRE4_RUN] = KGCPlayerLire4::OnRun;
	ms_mapStateFunction[MID_LIRE4_JUMP] = KGCPlayerLire4::OnJump;
	ms_mapStateFunction[MID_LIRE4_DOUBLEJUMP] = KGCPlayerLire4::OnDoubleJump;
	ms_mapStateFunction[MID_LIRE4_ATK1_1] = KGCPlayerLire4::OnComboAtk;
	ms_mapStateFunction[MID_LIRE4_ATK1_2] = KGCPlayerLire4::OnComboAtk;
	ms_mapStateFunction[MID_LIRE4_ATK1_3] = KGCPlayerLire4::OnComboAtk;
	ms_mapStateFunction[MID_LIRE4_ATK1_4] = KGCPlayerLire4::OnComboAtk;
	ms_mapStateFunction[MID_LIRE4_ATK1_5] = KGCPlayerLire4::OnComboAtk;
	ms_mapStateFunction[MID_LIRE4_DOUBLE_ATK2] = KGCPlayerLire4::OnDoubleAttack2;
	ms_mapStateFunction[MID_LIRE4_SPECIAL3] = KGCPlayerLire4::OnSpecial3;
	ms_mapStateFunction[MID_LIRE4_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE4_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE4_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE4_RESULT_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LIRE4_DASH_BACK] = KGCPlayerLire4::OnDashBack;

	ms_mapStateFunction[MID_SKILL_LIRE4_DASH] = KGCPlayerLire4::OnDash;
	ms_mapStateFunction[MID_SKILL_LIRE4_DASH_TO_BACK] = KGCPlayerLire4::OnDashBack;
	//======================================================================================
	//                                     리르 3차전직 끝
	//======================================================================================
	//======================================================================================
	//                                     라스 3차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_LAS4_WAIT] = KGCPlayerLas4::OnWait;
	ms_mapStateFunction[MID_LAS4_FATAL] = KGCPlayerLas4::OnWait;
	ms_mapStateFunction[MID_LAS4_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_LAS4_DASH] = KGCPlayerLas4::OnDash;
	ms_mapStateFunction[MID_LAS4_DASH_END_WAIT] = KGCPlayerLas4::OnDashEnd;
	ms_mapStateFunction[MID_LAS4_DASH_END_JUMP] = KGCPlayerLas4::OnDashEndJump;
	ms_mapStateFunction[MID_LAS4_DOUBLE_DASH] = KGCPlayerLas4::OnDoubleDash;
	ms_mapStateFunction[MID_LAS4_DOUBLE_DASH_END_WAIT] = KGCPlayerLas4::OnDoubleDashEnd;
	ms_mapStateFunction[MID_LAS4_DOUBLE_DASH_END_JUMP] = KGCPlayerLas4::OnDashEndJump;
	ms_mapStateFunction[MID_LAS4_JUMP] = KGCPlayerCommon::OnJump;
	ms_mapStateFunction[MID_LAS4_ATK1] = KGCPlayerLas4::OnCombo1;
	ms_mapStateFunction[MID_LAS4_ATK2] = KGCPlayerLas4::OnCombo2;
	ms_mapStateFunction[MID_LAS4_ATK3] = KGCPlayerLas4::OnCombo3;
	ms_mapStateFunction[MID_LAS4_ATK4] = KGCPlayerLas4::OnCombo4;
	ms_mapStateFunction[MID_LAS4_DASH_ATK] = KGCPlayerLas4::OnDashAtk;
	ms_mapStateFunction[MID_LAS4_DOUBLE_DASH_ATK] = KGCPlayerLas4::OnDoubleDashAtk;
	ms_mapStateFunction[MID_LAS4_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_LAS4_CHANGE_BODY_SKILL] = KGCPlayerLas4::OnChangeBodytoLog;
	ms_mapStateFunction[MID_SKILL_LAS4_SUPER_JUMP] = KGCPlayerLas::OnSuperJump;
	//======================================================================================
	//                                     라스 3차전직 끝
	//======================================================================================
	//======================================================================================
	//                                     로난 시작
	//======================================================================================
	ms_mapStateFunction[MID_RONAN_WAIT] = KGCPlayerRonan::OnWait;
	ms_mapStateFunction[MID_RONAN_FATAL] = KGCPlayerRonan::OnWait;
	ms_mapStateFunction[MID_RONAN_WALK] = KGCPlayerRonan::OnWalk;
	ms_mapStateFunction[MID_RONAN_RUN] = KGCPlayerRonan::OnRun;
	ms_mapStateFunction[MID_RONAN_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_TAG_RONAN_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_TAG_RONAN_START] = KGCTagPlayer::OnTagStart;
	//======================================================================================
	//                                     로난 끝
	//======================================================================================
	//======================================================================================
	//                                     라이언 2차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_RYAN3_WAIT] = KGCPlayerRyan3::OnWait;
	ms_mapStateFunction[MID_RYAN3_FATAL] = KGCPlayerRyan3::OnWait;
	ms_mapStateFunction[MID_RYAN3_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_RYAN3_RUN] = KGCPlayerRyan3::OnRun;
	ms_mapStateFunction[MID_RYAN3_DASH] = KGCPlayerRyan3::OnDash;
	ms_mapStateFunction[MID_RYAN3_DASH_END_JUMP] = KGCPlayerRyan3::OnDashEnd;
	ms_mapStateFunction[MID_RYAN3_JUMP] = KGCPlayerRyan3::OnJump;
	ms_mapStateFunction[MID_RYAN3_JUMP_DASH] = KGCPlayerRyan3::OnDash;
	ms_mapStateFunction[MID_RYAN3_JUMP_DASH_ATK] = KGCPlayerRyan3::OnJumpDashAtk;
	ms_mapStateFunction[MID_RYAN3_ATK1] = KGCPlayerRyan3::OnCombo;
	ms_mapStateFunction[MID_RYAN3_ATK2] = KGCPlayerRyan3::OnCombo;
	ms_mapStateFunction[MID_RYAN3_ATK3] = KGCPlayerRyan3::OnCombo;
	ms_mapStateFunction[MID_RYAN3_ATK4] = KGCPlayerRyan3::OnCombo;
	ms_mapStateFunction[MID_RYAN3_LEVEL_ATK1] = KGCPlayerRyan3::OnCombo;
	ms_mapStateFunction[MID_RYAN3_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN3_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN3_RESULT_WIN] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN3_RESULT_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN3_DASH_END_WAIT] = KGCPlayerRyan3::OnDashEnd;
	//======================================================================================
	//                                     라이언 2차전직 끝
	//======================================================================================
	//======================================================================================
	//                                     로난 1차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_RONAN2_WAIT] = KGCPlayerRonan2::OnWait;
	ms_mapStateFunction[MID_RONAN2_FATAL] = KGCPlayerRonan2::OnWait;
	ms_mapStateFunction[MID_RONAN2_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_RONAN2_RUN] = KGCPlayerRonan2::OnRun;
	ms_mapStateFunction[MID_RONAN2_ATK1] = KGCPlayerRonan2::OnCombo;
	ms_mapStateFunction[MID_RONAN2_ATK2] = KGCPlayerRonan2::OnCombo;
	ms_mapStateFunction[MID_RONAN2_ATK3] = KGCPlayerRonan2::OnCombo;
	ms_mapStateFunction[MID_RONAN2_ATK4] = KGCPlayerRonan2::OnCombo;
	ms_mapStateFunction[MID_RONAN2_ATK5] = KGCPlayerRonan2::OnCombo;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK1_END_TO_WAIT] = KGCPlayerCommon::OnEmptyCancel;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK2_END_TO_WAIT] = KGCPlayerCommon::OnEmptyCancel;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK3_END_TO_WAIT] = KGCPlayerCommon::OnEmptyCancel;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK1_END_TO_JUMP] = KGCPlayerCommon::OnEmptyCancel;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK2_END_TO_JUMP] = KGCPlayerCommon::OnEmptyCancel;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK3_END_TO_JUMP] = KGCPlayerCommon::OnEmptyCancel;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK1] = KGCPlayerRonan2::OnDashAtk;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK2] = KGCPlayerRonan2::OnDashAtk;
	ms_mapStateFunction[MID_RONAN2_DASH_ATK3] = KGCPlayerRonan2::OnDashAtk;
	ms_mapStateFunction[MID_RONAN2_BACK_STEP] = KGCPlayerRonan2::OnBackStep;
	ms_mapStateFunction[MID_RONAN2_BACK_STEP_ATK] = KGCPlayerRonan2::OnBackStepAtk;
	ms_mapStateFunction[MID_RONAN2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	//======================================================================================
	//                                     로난 1차전직 끝
	//======================================================================================
	ms_mapStateFunction[MID_COMMON_WAIT_TELEPORT] = KGCPlayerCommon::OnWaitPortal;
	ms_mapStateFunction[MID_COMMON_INTO_TELEPORT] = KGCPlayerCommon::OnIntoPortal;
	ms_mapStateFunction[MID_NEPHILLIM_WAIT_TELEPORT] = KGCPlayerCommon::OnWaitPortal;
	ms_mapStateFunction[MID_NEPHILLIM_INTO_TELEPORT] = KGCPlayerCommon::OnIntoPortal;
	ms_mapStateFunction[MID_RYAN4_MDAN_WAIT_TELEPORT] = KGCPlayerCommon::OnWaitPortal;
	ms_mapStateFunction[MID_RYAN4_MDAN_INTO_TELEPORT] = KGCPlayerCommon::OnIntoPortal;
	//======================================================================================
	//                                     라이언 3차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_RYAN4_WAIT] = KGCPlayerRyan4::OnWait;
	ms_mapStateFunction[MID_RYAN4_FATAL] = KGCPlayerRyan4::OnWait;
	ms_mapStateFunction[MID_RYAN4_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_RYAN4_RUN] = KGCPlayerRyan4::OnRun;
	ms_mapStateFunction[MID_RYAN4_DASH] = KGCPlayerRyan4::OnDash;
	ms_mapStateFunction[MID_RYAN4_JUMP] = KGCPlayerRyan4::OnJump;
	ms_mapStateFunction[MID_RYAN4_ATK1] = KGCPlayerRyan4::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ATK2] = KGCPlayerRyan4::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ATK3] = KGCPlayerRyan4::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ATK4] = KGCPlayerRyan4::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ATK5] = KGCPlayerRyan4::OnCombo;
	ms_mapStateFunction[MID_RYAN4_DASH_ATK1] = KGCPlayerRyan4::OnDashAtk1;
	ms_mapStateFunction[MID_RYAN4_DASH_ATK2] = KGCPlayerRyan4::OnDashAtk2;
	ms_mapStateFunction[MID_RYAN4_DASH_ATK3] = KGCPlayerRyan4::OnDashAtk3;
	ms_mapStateFunction[MID_RYAN4_RUN_TO_WAIT] = KGCPlayerRyan4::OnRun;
	ms_mapStateFunction[MID_RYAN4_RUN_TO_JUMP] = KGCPlayerRyan4::OnRun;
	ms_mapStateFunction[MID_RYAN4_DASH_TO_WAIT] = KGCPlayerRyan4::OnDash;
	ms_mapStateFunction[MID_RYAN4_DASH_TO_JUMP] = KGCPlayerRyan4::OnDash;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_WAIT] = KGCPlayerRyan4_2::OnWait;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_FATAL] = KGCPlayerRyan4_2::OnWait;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_RUN] = KGCPlayerRyan4_2::OnRun;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_DASH] = KGCPlayerRyan4_2::OnDash;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_DASH_ATK] = KGCPlayerRyan4_2::OnDashAtk;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_JUMP] = KGCPlayerRyan4_2::OnJump;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_ATK1] = KGCPlayerRyan4_2::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_ATK2] = KGCPlayerRyan4_2::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_ATK3] = KGCPlayerRyan4_2::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_ATK4] = KGCPlayerRyan4_2::OnCombo;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_RUN_END_WAIT] = KGCPlayerRyan4_2::OnRun;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_RUN_END_JUMP] = KGCPlayerRyan4_2::OnRun;
	ms_mapStateFunction[MID_RYAN4_FORM_START] = KGCPlayerRyan4::OnFormChange;
	ms_mapStateFunction[MID_RYAN4_FORM_END] = KGCPlayerRyan4::OnFormChangeComplete;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_FORM_START] = KGCPlayerRyan4_2::OnFormChange;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_FORM_END] = KGCPlayerRyan4_2::OnFormChangeComplete;
	ms_mapStateFunction[MID_RYAN4_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN4_ONE_HAND_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_RYAN4_MDAN_WAIT] = KGCFormMDan::OnWait;
	ms_mapStateFunction[MID_RYAN4_MDAN_FATAL] = KGCFormMDan::OnWait;
	ms_mapStateFunction[MID_RYAN4_MDAN_RUN] = KGCFormMDan::OnRun;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH] = KGCFormMDan::OnDash;
	ms_mapStateFunction[MID_RYAN4_MDAN_JUMP] = KGCFormMDan::OnJump;
	ms_mapStateFunction[MID_RYAN4_MDAN_ATK1] = KGCFormMDan::OnCombo;
	ms_mapStateFunction[MID_RYAN4_MDAN_ATK2] = KGCFormMDan::OnCombo;
	ms_mapStateFunction[MID_RYAN4_MDAN_ATK3] = KGCFormMDan::OnCombo;
	ms_mapStateFunction[MID_RYAN4_MDAN_ATK4] = KGCFormMDan::OnCombo;
	ms_mapStateFunction[MID_RYAN4_MDAN_FORM_END] = KGCPlayerCommon::OnEmpty;
	ms_mapStateFunction[MID_RYAN4_MDAN_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_TO_WAIT] = KGCFormMDan::OnDash;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_TO_JUMP] = KGCFormMDan::OnDashToJump;
	ms_mapStateFunction[MID_RYAN4_MDAN_RUN_TO_WAIT] = KGCFormMDan::OnDashToWait;
	ms_mapStateFunction[MID_RYAN4_MDAN_RUN_TO_JUMP] = KGCFormMDan::OnRunToJump;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_ATK1_TO_JUMP] = KGCFormMDan::OnDashAtk1ToJump;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_JUMP] = KGCFormMDan::OnDashAtk2ToJump;
	ms_mapStateFunction[MID_RYAN4_MDAN_JUMP_ATK] = KGCFormMDan::OnJumpAtk;
	ms_mapStateFunction[MID_RYAN4_MDAN_JUMP_ATK] = KGCFormMDan::OnRunToJump;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_ATK1] = KGCFormMDan::OnDashAtk;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_ATK2] = KGCFormMDan::OnDashAtk;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_ATK2_BACK] = KGCFormMDan::OnDashAtk;
	ms_mapStateFunction[MID_RYAN4_MDAN_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_JUMP] = KGCFormMDan::OnDashAtk2BackToJump;
	ms_mapStateFunction[MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_WAIT] = KGCFormMDan::OnDashAtk2BackToWait;
	ms_mapStateFunction[MID_RYAN4_MDAN_DOWN_AND_STAND_UP] = KGCFormMDan::OnDownAndStandUp;

	//======================================================================================
	//                                     라이언 3차전직 끝
	//======================================================================================

	//======================================================================================
	//                                     에이미 1 시작
	//======================================================================================
	ms_mapStateFunction[MID_AMY1_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY1_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY1_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY1_DASH] = KGCPlayerAmy::OnDash;
	ms_mapStateFunction[MID_AMY1_DASH_ATK1] = KGCPlayerAmy::OnDashAtk;
	ms_mapStateFunction[MID_AMY1_DASH_ATK1_TO_WAIT] = KGCPlayerAmy::OnDashAtk1ToWaitJump;
	ms_mapStateFunction[MID_AMY1_DASH_ATK1_TO_JUMP] = KGCPlayerAmy::OnDashAtk1ToWaitJump;
	ms_mapStateFunction[MID_AMY1_JUMP_DASH] = KGCPlayerAmy::OnJumpDash;
	ms_mapStateFunction[MID_AMY1_DOUBLE_DASH] = KGCPlayerAmy::OnDoubleDash;
	ms_mapStateFunction[MID_AMY1_RUN] = KGCPlayerAmy::OnRun;
	ms_mapStateFunction[MID_AMY1_JUMP] = KGCPlayerAmy::OnJump;
	ms_mapStateFunction[MID_AMY1_DOUBLE_JUMP] = KGCPlayerAmy::OnDoubleJump;
	ms_mapStateFunction[MID_AMY1_DASH_TO_WAIT] = KGCPlayerAmy::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY1_DASH_TO_JUMP] = KGCPlayerAmy::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY1_RUN_TO_WAIT] = KGCPlayerAmy::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY1_RUN_TO_JUMP] = KGCPlayerAmy::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY1_DOUBLE_DASH_TO_WAIT] = KGCPlayerAmy::OnDoubleDashToWaitJump;
	ms_mapStateFunction[MID_AMY1_DOUBLE_DASH_TO_JUMP] = KGCPlayerAmy::OnDoubleDashToWaitJump;
	ms_mapStateFunction[MID_AMY1_ATK1] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_ATK2] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_ATK3] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_ATK4] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_ATK5] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_ATK6] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_ATK7] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_RUN_TO_DOUBLE_JUMP] = KGCPlayerAmy::OnRunToDoubleJump;
	ms_mapStateFunction[MID_AMY1_UNIQUE_ATK1] = KGCPlayerAmy::OnSpecial1Atk;
	ms_mapStateFunction[MID_AMY1_UNIQUE_ATK2] = KGCPlayerAmy::OnSpecial2Atk;
	ms_mapStateFunction[MID_AMY1_UNIQUE_ATK2_TO_WAIT] = KGCPlayerAmy::OnSpecialAtk2ToWaitJump;
	ms_mapStateFunction[MID_AMY1_UNIQUE_ATK2_TO_JUMP] = KGCPlayerAmy::OnSpecialAtk2ToWaitJump;
	ms_mapStateFunction[MID_AMY1_DASH_ATK2] = KGCPlayerAmy::OnDashAtk;
	ms_mapStateFunction[MID_AMY1_DASH_ATK2_TO_WAIT] = KGCPlayerAmy::OnDashAtk2ToWaitJump;
	ms_mapStateFunction[MID_AMY1_DASH_ATK2_TO_JUMP] = KGCPlayerAmy::OnDashAtk2ToWaitJump;
	ms_mapStateFunction[MID_AMY1_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_AMY1_DANCE_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY1_DANCE_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY1_DANCE_DASH_ATK1_TO_WAIT] = KGCPlayerAmy_2::OnDashAtk1ToWaitJump;
	ms_mapStateFunction[MID_AMY1_DANCE_DASH_TO_WAIT] = KGCPlayerAmy_2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY1_DANCE_RUN_TO_WAIT] = KGCPlayerAmy_2::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY1_DANCE_DOUBLE_DASH_TO_WAIT] = KGCPlayerAmy_2::OnDoubleDashToWaitJump;
	ms_mapStateFunction[MID_AMY1_DANCE_ATK1] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_DANCE_ATK2] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_DANCE_ATK3] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_DANCE_UNIQUE_ATK2_TO_WAIT] = KGCPlayerAmy_2::OnSpecialAtk2ToWaitJump;
	ms_mapStateFunction[MID_AMY1_DANCE_DASH_ATK2_TO_WAIT] = KGCPlayerAmy_2::OnDashAtk2ToWaitJump;
	ms_mapStateFunction[MID_AMY1_DANCE_SPECIAL1] = KGCPlayerAmy_2::OnSpecial1;
	ms_mapStateFunction[MID_AMY1_SWAP_TO_DANCE_MODE] = KGCPlayerAmy_2::OnModeChange;
	ms_mapStateFunction[MID_AMY1_SWAP_TO_FIGHT_MODE] = KGCPlayerAmy_2::OnModeChange;
	ms_mapStateFunction[MID_AMY1_SWAP_TO_DANCEMODE_WAIT] = KGCPlayerAmy_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY1_SWAP_TO_FIGHTMODE_WAIT] = KGCPlayerAmy_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY1_SWAP_TO_JUMP] = KGCPlayerAmy_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY1_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY1_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY1_DANCE_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY1_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;
	//======================================================================================
	//                                     에이미 1 끝
	//======================================================================================
	//======ㅋ================================================================================
	//                                     로난 2차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_RONAN3_WAIT] = KGCPlayerRonan3::OnWait;
	ms_mapStateFunction[MID_RONAN3_FATAL] = KGCPlayerRonan3::OnWait;
	ms_mapStateFunction[MID_RONAN3_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_RONAN3_RUN] = KGCPlayerRonan3::OnRun;
	ms_mapStateFunction[MID_RONAN3_DASH] = KGCPlayerRonan3::OnDash;
	ms_mapStateFunction[MID_RONAN3_DASH_TO_JUMP] = KGCPlayerRonan3::OnDashToJump;
	ms_mapStateFunction[MID_RONAN3_JUMP] = KGCPlayerRonan3::OnJump;
	ms_mapStateFunction[MID_RONAN3_ATK1] = KGCPlayerRonan3::OnCombo;
	ms_mapStateFunction[MID_RONAN3_ATK2] = KGCPlayerRonan3::OnCombo;
	ms_mapStateFunction[MID_RONAN3_ATK3] = KGCPlayerRonan3::OnCombo;
	ms_mapStateFunction[MID_RONAN3_ATK4] = KGCPlayerRonan3::OnCombo;
	ms_mapStateFunction[MID_RONAN3_ATK5] = KGCPlayerRonan3::OnCombo;
	ms_mapStateFunction[MID_RONAN3_ATK6] = KGCPlayerRonan3::OnCombo;
	ms_mapStateFunction[MID_RONAN3_DASH_ATK1] = KGCPlayerRonan3::OnDashAtk;
	ms_mapStateFunction[MID_RONAN3_DASH_ATK2] = KGCPlayerRonan3::OnDashAtk;
	ms_mapStateFunction[MID_RONAN3_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RONAN3_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RONAN3_DASH_ATK1_TO_WAIT] = KGCPlayerRonan3::OnDashAtkToWait;
	ms_mapStateFunction[MID_RONAN3_DASH_ATK2_TO_WAIT] = KGCPlayerRonan3::OnDashAtkToWait;
	ms_mapStateFunction[MID_RONAN3_GUARD] = KGCPlayerRonan3::OnGuard;
	ms_mapStateFunction[MID_RONAN3_JUMP_GUARD] = KGCPlayerRonan3::OnGuard;
	ms_mapStateFunction[MID_RONAN3_PSYCHIC_FORCE_JUMP_UP] = KGCPlayerRonan3::OnPsychicForce;
	ms_mapStateFunction[MID_RONAN3_PSYCHIC_FORCE_JUMP_DOWN] = KGCPlayerRonan3::OnPsychicForce;
	ms_mapStateFunction[MID_RONAN3_PSYCHIC_FORCE_WAIT] = KGCPlayerRonan3::OnPsychicForce;
	ms_mapStateFunction[MID_RONAN3_CRITICAL_ATK] = KGCPlayerRonan3::OnCriticalAtk;
	ms_mapStateFunction[MID_RONAN3_DOUBLE_ATK] = KGCPlayerRonan3::OnCriticalAtk;
	ms_mapStateFunction[MID_RONAN3_SWORD_SPECIAL1_POST_ATK] = KGCPlayerRonan3::OnSpecialAtk;
	ms_mapStateFunction[MID_RONAN3_SWORD_SPECIAL2] = KGCPlayerRonan3::OnSpecialAtk;
	ms_mapStateFunction[MID_RONAN3_SWORD_SPECIAL3] = KGCPlayerRonan3::OnSpecialAtk;
	//======================================================================================
	//                                     로난 2차전직 끝
	//======================================================================================

	//======================================================================================
	//                                     에이미 1차 전직 시작
	//======================================================================================

	ms_mapStateFunction[MID_AMY2_PERFORMENCE_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY2_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_DASH] = KGCPlayerAmy2_2::OnDash;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_JUMP] = KGCPlayerAmy2_2::OnJump;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_ATK1] = KGCPlayerAmy2_2::OnCombo;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_ATK2] = KGCPlayerAmy2_2::OnCombo;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_ATK3] = KGCPlayerAmy2_2::OnCombo;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_ATK4] = KGCPlayerAmy2_2::OnCombo;
	ms_mapStateFunction[MID_AMY2_SWAP_TO_PERFORMENCE_MODE] = KGCPlayerAmy2_2::OnModeChange;
	ms_mapStateFunction[MID_AMY2_SWAP_TO_FIGHT_MODE] = KGCPlayerAmy2_2::OnModeChange;
	ms_mapStateFunction[MID_AMY2_SWAP_TO_PERFORMENCEMODE_WAIT] = KGCPlayerAmy2_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY2_SWAP_TO_FIGHTMODE_WAIT] = KGCPlayerAmy2_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY2_SWAP_TO_FIGHTMODE_JUMP] = KGCPlayerAmy2_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY2_SWAP_TO_PERFORMENCE_JUMP] = KGCPlayerAmy2_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_SPECIAL3_READY] = KGCPlayerAmy2_2::OnSpecial3Atk;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_AMY2_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY2_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY2_JUMP] = KGCPlayerAmy2::OnJump;
	ms_mapStateFunction[MID_AMY2_JUMP_DASH] = KGCPlayerAmy2::OnJumpDash;
	ms_mapStateFunction[MID_AMY2_ATK1] = KGCPlayerAmy2::OnCombo;
	ms_mapStateFunction[MID_AMY2_ATK2] = KGCPlayerAmy2::OnCombo;
	ms_mapStateFunction[MID_AMY2_ATK3] = KGCPlayerAmy2::OnCombo;
	ms_mapStateFunction[MID_AMY2_ATK4] = KGCPlayerAmy2::OnCombo;
	ms_mapStateFunction[MID_AMY2_RUN] = KGCPlayerAmy2::OnRun;
	ms_mapStateFunction[MID_AMY2_RUN_TO_WAIT] = KGCPlayerAmy2::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY2_RUN_TO_JUMP] = KGCPlayerAmy2::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY2_DASH] = KGCPlayerAmy2::OnDash;
	ms_mapStateFunction[MID_AMY2_DOUBLE_JUMP] = KGCPlayerAmy2::OnDoubleJump;
	ms_mapStateFunction[MID_AMY2_DASH_TO_WAIT] = KGCPlayerAmy2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY2_DASH_TO_JUMP] = KGCPlayerAmy2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY2_RUN_TO_DOUBLE_JUMP] = KGCPlayerAmy2::OnRunToDoubleJump;
	ms_mapStateFunction[MID_AMY2_SPECIAL_ATK1] = KGCPlayerAmy2::OnUniqueSkill;
	ms_mapStateFunction[MID_AMY2_SPECIAL_ATK1_TO_WAIT] = KGCPlayerAmy2::OnUniqueSkillAtk;
	ms_mapStateFunction[MID_AMY2_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY2_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY2_SPECIAL_ATK1_SKY_TO_JUMP] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY2_DASH_ATK1] = KGCPlayerAmy2::OnDashAtk;
	ms_mapStateFunction[MID_AMY2_DASH_ATK2] = KGCPlayerAmy2::OnDashAtk;
	ms_mapStateFunction[MID_AMY2_DASH_ATK1_TO_JUMP] = KGCPlayerAmy2::OnDashAtk1ToWaitJump;
	ms_mapStateFunction[MID_AMY2_DASH_ATK1_TO_WAIT] = KGCPlayerAmy2::OnDashAtk1ToWaitJump;

	ms_mapStateFunction[MID_AMY2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY2_PERFORMENCE_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY2_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;

	//======================================================================================
	//                                     에이미 1차 전직 끝
	//======================================================================================

	//======================================================================================
	//                                     엘리시스 스킬 시작
	//======================================================================================
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_1] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_2] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_3] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_COMBO_A_4] = KGCPlayerElesis::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS1_SUPER_DASH] = KGCPlayerElesis::OnDash;
	ms_mapStateFunction[MID_SKILL_ELESIS1_3HIT_DASH_ATK] = KGCPlayerElesis::OnSkillDashAtk1;
	ms_mapStateFunction[MID_SKILL_ELESIS1_FAST_ATK] = KGCPlayerElesis::OnSkillFastInput;
	ms_mapStateFunction[MID_SKILL_ELESIS1_SPECIAL_ATK2_1] = KGCPlayerElesis::OnSkillSpecialAtk2DashCombo;
	ms_mapStateFunction[MID_SKILL_ELESIS1_SPECIAL_ATK2_2] = KGCPlayerElesis::OnSkillSpecialAtk2DashCombo;
	ms_mapStateFunction[MID_SKILL_ELESIS1_SPECIAL_ATK2_0_1_DASH] = KGCPlayerElesis::OnSkillSpecialAtk2Dash;
	ms_mapStateFunction[MID_SKILL_ELESIS1_SPECIAL_ATK2_1_2_DASH] = KGCPlayerElesis::OnSkillSpecialAtk2Dash;
	ms_mapStateFunction[MID_SKILL_ELESIS1_SPECIAL_ATK2_2_3_DASH] = KGCPlayerElesis::OnSkillSpecialAtk2Dash;
	ms_mapStateFunction[MID_SKILL_ELESIS1_SPECIAL2_LV3] = KGCPlayerElesis::OnSkillSpecial2_Lv2;
	ms_mapStateFunction[MID_SKILL_ELESIS1_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_SKILL_ELESIS2_MAGIC_DEFENSE] = KGCPlayerElesis2::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS3_MAGIC_DEFENSE] = KGCPlayerElesis3::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ELESIS4_MAGIC_DEFENSE] = KGCPlayerElesis4::OnCombo;
	//======================================================================================
	//                                     기사 스킬트리 끝
	//======================================================================================

	//======================================================================================
	//                                     로난3차전직 시작
	//======================================================================================
	ms_mapStateFunction[MID_RONAN4_WAIT] = KGCPlayerRonan4::OnWait;
	ms_mapStateFunction[MID_RONAN4_FATAL] = KGCPlayerRonan4::OnWait;
	ms_mapStateFunction[MID_RONAN4_RUN] = KGCPlayerRonan4::OnRun;
	ms_mapStateFunction[MID_RONAN4_DASH] = KGCPlayerRonan4::OnDash;
	ms_mapStateFunction[MID_RONAN4_DASH_TO_JUMP] = KGCPlayerRonan4::OnDashToJump;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_A2_TO_JUMP] = KGCPlayerRonan4::OnDashAtkToJump;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_A3_TO_JUMP] = KGCPlayerRonan4::OnDashAtkToJump;
	ms_mapStateFunction[MID_RONAN4_JUMP] = KGCPlayerRonan4::OnJump;
	ms_mapStateFunction[MID_RONAN4_ATK1] = KGCPlayerRonan4::OnCombo;
	ms_mapStateFunction[MID_RONAN4_ATK2] = KGCPlayerRonan4::OnCombo;
	ms_mapStateFunction[MID_RONAN4_ATK3] = KGCPlayerRonan4::OnCombo;
	ms_mapStateFunction[MID_RONAN4_ATK4] = KGCPlayerRonan4::OnCombo;
	ms_mapStateFunction[MID_RONAN4_JUMP_ATK_A1] = KGCPlayerRonan4::OnJumpAtk;
	ms_mapStateFunction[MID_RONAN4_JUMP_ATK_B] = KGCPlayerRonan4::OnJumpAtk;
	ms_mapStateFunction[MID_RONAN4_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_A1] = KGCPlayerRonan4::OnDashAtk;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_A3] = KGCPlayerRonan4::OnDashAtk;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_B1] = KGCPlayerRonan4::OnDashAtkCombo;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_B2] = KGCPlayerRonan4::OnDashAtkCombo;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_A1_TO_WAIT] = KGCPlayerRonan4::OnDashAtkToWait;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_A2_TO_WAIT] = KGCPlayerRonan4::OnDashAtkToWait;
	ms_mapStateFunction[MID_RONAN4_DASH_ATK_A3_TO_WAIT] = KGCPlayerRonan4::OnDashAtkToWait;
	ms_mapStateFunction[MID_RONAN4_UNIQUE_ATK_A1] = KGCPlayerRonan4::OnUniqueAtk;
	ms_mapStateFunction[MID_RONAN4_AERIAL_COMBO_A1] = KGCPlayerRonan4::OnSpecialCombo;
	ms_mapStateFunction[MID_RONAN4_AERIAL_COMBO_A2] = KGCPlayerRonan4::OnSpecialCombo;
	ms_mapStateFunction[MID_RONAN4_MAGIC_COMBO1] = KGCPlayerRonan4::OnSpecialCombo;
	ms_mapStateFunction[MID_RONAN4_CRITICAL_ATK] = KGCPlayerRonan4::OnSpecialCombo;
	ms_mapStateFunction[MID_RONAN4_DOUBLE_ATK] = KGCPlayerRonan4::OnSpecialCombo;
	ms_mapStateFunction[MID_RONAN4_MONOVOLT_READY] = KGCPlayerRonan4::OnMonoVolt;

	//======================================================================================
	//                                     로난3차전직 끝
	//======================================================================================

	//======================================================================================
	//                                     법사 스킬 시작
	//======================================================================================
	ms_mapStateFunction[MID_SKILL_COMMON_ARME_TELEPORT_SPEED_UP] = KGCPlayerArme::OnDash;
	ms_mapStateFunction[MID_SKILL_ARME4_TELEPORT_SPEED_UP] = KGCPlayerArme4::OnDash;
	ms_mapStateFunction[MID_SKILL_COMMON_ARME_TELEPORT_UP] = KGCPlayerCommon::OnVerticalDashToJump;
	ms_mapStateFunction[MID_SKILL_COMMON_ARME_TELEPORT_DOWN] = KGCPlayerCommon::OnVerticalDashToJump;
	ms_mapStateFunction[MID_SKILL_COMMON_ARME_TELEPORT_BACK] = KGCPlayerCommon::OnBackTeleport;
	ms_mapStateFunction[MID_SKILL_ARME1_NEWCOMBO_ATK1] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ARME1_NEWCOMBO_ATK2] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ARME1_NEWCOMBO_ATK3] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ARME1_NEWCOMBO_ATK4] = KGCPlayerArme::OnComboAtk;
	ms_mapStateFunction[MID_SKILL_ARME1_B1_FEAR_LV1] = KGCPlayerArme::OnBlackMagicFear;
	ms_mapStateFunction[MID_SKILL_ARME1_B1_FEAR_LV2] = KGCPlayerArme::OnBlackMagicFear;
	ms_mapStateFunction[MID_SKILL_ARME1_WHITE2_ICETRAP_LV1] = KGCPlayerArme::OnSpecialW2IceTrap;
	ms_mapStateFunction[MID_SKILL_ARME1_B2_SHOCKSTURN_LV2] = KGCPlayerArme::OnSpecial2Black;
	//======================================================================================
	//                                     법사 스킬 끝
	//======================================================================================

	//======================================================================================
	//                                     에이미 2차전직 시작
	//======================================================================================

	// 연주모드
	ms_mapStateFunction[MID_AMY3_DANCE_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY3_DANCE_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY3_DANCE_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY3_DANCE_DASH] = KGCPlayerAmy3_2::OnDash;
	ms_mapStateFunction[MID_AMY3_DANCE_DASH_ATK] = KGCPlayerAmy3_2::OnDashAtk;
	ms_mapStateFunction[MID_AMY3_DANCE_DASH_ATK_TO_WAIT] = KGCPlayerAmy3_2::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_DASH_ATK_TO_JUMP] = KGCPlayerAmy3_2::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_JUMP_DASH] = KGCPlayerAmy3_2::OnJumpDash;
	ms_mapStateFunction[MID_AMY3_DANCE_DASH_TO_WAIT] = KGCPlayerAmy3_2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_DASH_TO_JUMP] = KGCPlayerAmy3_2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_JUMP_DASH_TO_JUMP] = KGCPlayerAmy3_2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_RUN] = KGCPlayerAmy3_2::OnRun;
	ms_mapStateFunction[MID_AMY3_DANCE_RUN_TO_WAIT] = KGCPlayerAmy3_2::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_RUN_TO_JUMP] = KGCPlayerAmy3_2::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_JUMP] = KGCPlayerAmy3_2::OnJump;
	ms_mapStateFunction[MID_AMY3_DANCE_DOUBLE_JUMP] = KGCPlayerAmy3_2::OnDoubleJump;
	ms_mapStateFunction[MID_AMY3_DANCE_RUN_TO_DOUBLE_JUMP] = KGCPlayerAmy3_2::OnRunToDoubleJump;
	ms_mapStateFunction[MID_AMY3_DANCE_ATK1] = KGCPlayerAmy3_2::OnCombo;
	ms_mapStateFunction[MID_AMY3_DANCE_ATK2] = KGCPlayerAmy3_2::OnCombo;
	ms_mapStateFunction[MID_AMY3_DANCE_ATK3] = KGCPlayerAmy3_2::OnCombo;
	ms_mapStateFunction[MID_AMY3_DANCE_UNIQUE_ATK] = KGCPlayerAmy3_2::OnCombo;
	ms_mapStateFunction[MID_AMY3_DANCE_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_AMY3_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY3_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY3_JUMP] = KGCPlayerAmy3::OnJump;
	ms_mapStateFunction[MID_AMY3_JUMP_DASH] = KGCPlayerAmy3::OnJumpDash;
	ms_mapStateFunction[MID_AMY3_ATK1] = KGCPlayerAmy3::OnCombo;
	ms_mapStateFunction[MID_AMY3_ATK2] = KGCPlayerAmy3::OnCombo;
	ms_mapStateFunction[MID_AMY3_ATK3] = KGCPlayerAmy3::OnCombo;
	ms_mapStateFunction[MID_AMY3_ATK4] = KGCPlayerAmy3::OnCombo;
	ms_mapStateFunction[MID_AMY3_ATK5] = KGCPlayerAmy3::OnCombo;
	ms_mapStateFunction[MID_AMY3_ATK6] = KGCPlayerAmy3::OnCombo;
	ms_mapStateFunction[MID_AMY3_RUN] = KGCPlayerAmy3::OnRun;
	ms_mapStateFunction[MID_AMY3_RUN_TO_WAIT] = KGCPlayerAmy3::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY3_RUN_TO_JUMP] = KGCPlayerAmy3::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY3_DASH] = KGCPlayerAmy3::OnDash;
	ms_mapStateFunction[MID_AMY3_DOUBLE_JUMP] = KGCPlayerAmy3::OnDoubleJump;
	ms_mapStateFunction[MID_AMY3_DASH_TO_WAIT] = KGCPlayerAmy3::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY3_DASH_TO_JUMP] = KGCPlayerAmy3::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY3_JUMP_DASH_TO_JUMP] = KGCPlayerAmy3::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY3_RUN_TO_DOUBLE_JUMP] = KGCPlayerAmy3::OnRunToDoubleJump;
	ms_mapStateFunction[MID_AMY3_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY3_DASH_ATK1] = KGCPlayerAmy3::OnDashAtk;
	ms_mapStateFunction[MID_AMY3_DASH_ATK2] = KGCPlayerAmy3::OnDashAtk;
	ms_mapStateFunction[MID_AMY3_DASH_ATK1_TO_JUMP] = KGCPlayerAmy3::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_DASH_ATK1_TO_WAIT] = KGCPlayerAmy3::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_DASH_ATK2_TO_JUMP] = KGCPlayerAmy3::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_DASH_ATK2_TO_WAIT] = KGCPlayerAmy3::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_FIGHT_JUMP_UNIQUE2_TO_JUMP] = KGCPlayerAmy3::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_DANCE_JUMP_UNIQUE2_TO_JUMP] = KGCPlayerAmy3::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY3_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY3_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY3_DANCE_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY3_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY3_FATAL] = KGCPlayerCommon::OnWait;

	ms_mapStateFunction[MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL1] = KGCPlayerAmy3::OnJumpUniqueSkill;
	ms_mapStateFunction[MID_AMY3_DANCE_JUMP_UNIQUE_SKILL1] = KGCPlayerAmy3::OnJumpUniqueSkill;
	ms_mapStateFunction[MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL2] = KGCPlayerAmy3::OnJumpUniqueSkill2;
	ms_mapStateFunction[MID_AMY3_DANCE_JUMP_UNIQUE_SKILL2] = KGCPlayerAmy3::OnJumpUniqueSkill2;
	ms_mapStateFunction[MID_AMY3_SWAP_TO_DANCE_MODE] = KGCPlayerAmy3_2::OnModeChange;
	ms_mapStateFunction[MID_AMY3_SWAP_TO_FIGHT_MODE] = KGCPlayerAmy3_2::OnModeChange;
	ms_mapStateFunction[MID_AMY3_SWAP_TO_DANCEMODE_WAIT] = KGCPlayerAmy3_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY3_SWAP_TO_DANCEMODE_JUMP] = KGCPlayerAmy3_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY3_SWAP_TO_FIGHTMODE_WAIT] = KGCPlayerAmy3_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY3_SWAP_TO_FIGHTMODE_JUMP] = KGCPlayerAmy3_2::OnModeChangeToWaitJump;
	//======================================================================================
	//                                     에이미 2차 전직 끝
	//======================================================================================

	//======================================================================================
	//                                     진 시작
	//======================================================================================
	ms_mapStateFunction[MID_JIN_BURNNING_CHARGE] = KGCPlayerCommon::OnBurnningCharge;
	ms_mapStateFunction[MID_JIN_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_JIN_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_JIN_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN_WALK_BURNNING_MODE] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN_RUN] = KGCPlayerJin::OnRun;
	ms_mapStateFunction[MID_JIN_DASH] = KGCPlayerJin::OnDash;
	ms_mapStateFunction[MID_JIN_JUMP_DASH] = KGCPlayerJin::OnDash;
	ms_mapStateFunction[MID_JIN_JUMP] = KGCPlayerJin::OnJump;
	ms_mapStateFunction[MID_JIN_ATK1] = KGCPlayerJin::OnCombo;
	ms_mapStateFunction[MID_JIN_ATK2] = KGCPlayerJin::OnCombo;
	ms_mapStateFunction[MID_JIN_ATK3] = KGCPlayerJin::OnCombo;
	ms_mapStateFunction[MID_JIN_ATK4] = KGCPlayerJin::OnCombo;
	ms_mapStateFunction[MID_JIN_ATK5] = KGCPlayerJin::OnCombo;
	ms_mapStateFunction[MID_JIN_DASH_ATK] = KGCPlayerJin::OnDashAtk;
	ms_mapStateFunction[MID_JIN_DASH_ATK_1] = KGCPlayerJin::OnDashAtkCombo;
	ms_mapStateFunction[MID_JIN_DASH_ATK_2] = KGCPlayerJin::OnDashAtkCombo;
	ms_mapStateFunction[MID_JIN_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_JIN_TAG_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_JIN_TAG_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_JIN_RUN_BURNNING_MODE] = KGCPlayerJinBurnning::OnRun;
	ms_mapStateFunction[MID_JIN_DASH_BURNNING_MODE] = KGCPlayerJinBurnning::OnDash;
	ms_mapStateFunction[MID_JIN_WAIT_BURNNING_MODE] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_JIN_RUN_TO_JUMP] = KGCPlayerJin::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN_DASH_TO_JUMP] = KGCPlayerJin::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN_DASH_ATK_TO_JUMP] = KGCPlayerJin::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN_DASH_ATK_1_TO_JUMP] = KGCPlayerJin::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN_DASH_ATK_2_TO_JUMP] = KGCPlayerJin::OnReturnToJumpOrWait;
	//======================================================================================
	//                                     진  끝
	//======================================================================================

	//======================================================================================
	//                                     진 1차 시작
	//======================================================================================
	ms_mapStateFunction[MID_JIN1_BURNNING_CHARGE] = KGCPlayerCommon::OnBurnningCharge;
	ms_mapStateFunction[MID_JIN1_RUN_BURNNING_MODE] = KGCPlayerJin1Burnning::OnRun;
	ms_mapStateFunction[MID_JIN1_DASH_BURNNING_MODE] = KGCPlayerJin1Burnning::OnDash;
	ms_mapStateFunction[MID_JIN1_WALK_BURNNING_MODE] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN1_WAIT] = KGCPlayerJin1::OnWait;
	ms_mapStateFunction[MID_JIN1_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_JIN1_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN1_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_JIN1_RUN] = KGCPlayerJin1::OnRun;
	ms_mapStateFunction[MID_JIN1_DASH] = KGCPlayerJin1::OnDash;
	ms_mapStateFunction[MID_JIN1_JUMP_DASH] = KGCPlayerJin1::OnDash;
	ms_mapStateFunction[MID_JIN1_JUMP] = KGCPlayerJin1::OnJump;
	ms_mapStateFunction[MID_JIN1_AERIAL_STEP_ATTACKED] = KGCPlayerJin1::OnJump;
	ms_mapStateFunction[MID_JIN1_AERIAL_STEP_ATK] = KGCPlayerJin1::OnAerialStep;
	ms_mapStateFunction[MID_JIN1_RUN_TO_JUMP] = KGCPlayerJin1::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN1_DASH_TO_JUMP] = KGCPlayerJin1::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN1_DASH_ATK_TO_JUMP] = KGCPlayerJin1::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN1_DASH_ATK_1_TO_JUMP] = KGCPlayerJin1::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN1_TUMBLING_TO_JUMP] = KGCPlayerJin1::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN1_DASH_TUMBLING_JUMP] = KGCPlayerJin1::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN1_TUMBLING] = KGCPlayerJin1::OnUniqueSkillTumbling;
	ms_mapStateFunction[MID_JIN1_DASH_TUMBLING] = KGCPlayerJin1::OnUniqueSkillTumbling;
	ms_mapStateFunction[MID_JIN1_DASH_ATK] = KGCPlayerJin1::OnDashAtk;
	ms_mapStateFunction[MID_JIN1_DASH_ATK_1] = KGCPlayerJin1::OnDashAtkCombo;
	ms_mapStateFunction[MID_JIN1_ATK1] = KGCPlayerJin1::OnCombo;
	ms_mapStateFunction[MID_JIN1_ATK2] = KGCPlayerJin1::OnCombo;
	ms_mapStateFunction[MID_JIN1_ATK3] = KGCPlayerJin1::OnCombo;
	ms_mapStateFunction[MID_JIN1_ATK4] = KGCPlayerJin1::OnCombo;
	ms_mapStateFunction[MID_JIN1_ATK5] = KGCPlayerJin1::OnCombo;
	ms_mapStateFunction[MID_JIN1_TUMBLING_TO_WAIT] = KGCPlayerJin1::OnTumblingToAction;
	ms_mapStateFunction[MID_JIN1_DASH_TUMBLING_WAIT] = KGCPlayerJin1::OnTumblingToAction;
	ms_mapStateFunction[MID_JIN1_BURNNING_HUJIN] = KGCPlayerJin1::OnHujin;
	ms_mapStateFunction[MID_JIN1_AERIAL_COMBO_ATK1_EX] = KGCPlayerJin1::OnAerialCombo;
	ms_mapStateFunction[MID_JIN1_AERIAL_COMBO_ATK1] = KGCPlayerJin1::OnAerialCombo;
	ms_mapStateFunction[MID_JIN1_AERIAL_COMBO_ATK2] = KGCPlayerJin1::OnAerialCombo;

	//======================================================================================
	//                                     진 1차 끝
	//======================================================================================

	//======================================================================================
	//                                     진 2차 시작
	//======================================================================================
	ms_mapStateFunction[MID_JIN2_WAIT] = KGCPlayerJin2::OnWait;
	ms_mapStateFunction[MID_JIN2_FATAL] = KGCPlayerJin2::OnWait;
	ms_mapStateFunction[MID_JIN2_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN2_RUN] = KGCPlayerJin2::OnRun;
	ms_mapStateFunction[MID_JIN2_JUMP] = KGCPlayerJin2::OnJump;
	ms_mapStateFunction[MID_JIN2_DASH] = KGCPlayerJin2::OnDash;
	ms_mapStateFunction[MID_JIN2_JUMP_DASH] = KGCPlayerJin2::OnDash;
	ms_mapStateFunction[MID_JIN2_BURNNING_CHARGE] = KGCPlayerCommon::OnBurnningCharge;
	ms_mapStateFunction[MID_JIN2_RUN_BURNNING_MODE] = KGCPlayerJin2Burnning::OnRun;
	ms_mapStateFunction[MID_JIN2_DASH_BURNNING_MODE] = KGCPlayerJin2Burnning::OnDash;
	ms_mapStateFunction[MID_JIN2_WALK_BURNNING_MODE] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN2_RUN_TO_JUMP] = KGCPlayerJin2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_DASH_TO_JUMP] = KGCPlayerJin2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_ATK1] = KGCPlayerJin2::OnCombo;
	ms_mapStateFunction[MID_JIN2_ATK2] = KGCPlayerJin2::OnCombo;
	ms_mapStateFunction[MID_JIN2_ATK3] = KGCPlayerJin2::OnCombo;
	ms_mapStateFunction[MID_JIN2_ATK4] = KGCPlayerJin2::OnCombo;
	ms_mapStateFunction[MID_JIN2_ATK5] = KGCPlayerJin2::OnCombo;
	ms_mapStateFunction[MID_JIN2_DASH_TUMBLING_JUMP] = KGCPlayerJin2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_DASH_TUMBLING] = KGCPlayerJin2::OnUniqueSkillTumbling;
	ms_mapStateFunction[MID_JIN2_DASH_TUMBLING_WAIT] = KGCPlayerJin2::OnTumblingToAction;
	ms_mapStateFunction[MID_JIN2_DASH_ATK] = KGCPlayerJin2::OnDashAtk;
	ms_mapStateFunction[MID_JIN2_JUMP_ATK] = KGCPlayerJin2::OnJumpAtk;
	ms_mapStateFunction[MID_JIN2_JUMP_ATK_1] = KGCPlayerJin2::OnJumpAtk;
	ms_mapStateFunction[MID_JIN2_JUMP_ATK_1_TO_JUMP] = KGCPlayerJin2::OnJumpAtk;
	ms_mapStateFunction[MID_JIN2_JUMP_ATK_1_TO_WAIT] = KGCPlayerJin2::OnTumblingToAction;
	ms_mapStateFunction[MID_JIN2_JUMP_DOWN_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_JIN2_AERIAL_COMBO_ATK1] = KGCPlayerJin2::OnAerialCombo;
	ms_mapStateFunction[MID_JIN2_AERIAL_COMBO_ATK2] = KGCPlayerJin2::OnAerialCombo;
	ms_mapStateFunction[MID_JIN2_AERIAL_COMBO_ATK3] = KGCPlayerJin2::OnAerialCombo;
	ms_mapStateFunction[MID_JIN2_DASH_ATK_TO_JUMP] = KGCPlayerJin2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_JIN2_THROWWEAPON] = KGCPlayerJin2::OnThrowBong;
	ms_mapStateFunction[MID_JIN_TAG_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_JIN_TAG_START] = KGCTagPlayer::OnTagStart;

	//////////////////////////////////////////////////////////////////////////
	// 맨손 격투
	//////////////////////////////////////////////////////////////////////////

	ms_mapStateFunction[MID_JIN2_2_BURNNING_CHARGE] = KGCPlayerCommon::OnBurnningCharge;
	ms_mapStateFunction[MID_JIN2_2_WAIT] = KGCPlayerJin2_2::OnWait;
	ms_mapStateFunction[MID_JIN2_2_FATAL] = KGCPlayerJin2_2::OnWait;
	ms_mapStateFunction[MID_JIN2_2_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN2_2_WALK_BURNNING_MODE] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_JIN2_2_RUN] = KGCPlayerJin2_2::OnRun;
	ms_mapStateFunction[MID_JIN2_2_DASH] = KGCPlayerJin2_2::OnDash;
	ms_mapStateFunction[MID_JIN2_2_JUMP_DASH] = KGCPlayerJin2_2::OnDash;
	ms_mapStateFunction[MID_JIN2_2_JUMP] = KGCPlayerJin2_2::OnJump;
	ms_mapStateFunction[MID_JIN2_2_ATK1] = KGCPlayerJin2_2::OnCombo;
	ms_mapStateFunction[MID_JIN2_2_ATK2] = KGCPlayerJin2_2::OnCombo;
	ms_mapStateFunction[MID_JIN2_2_ATK3] = KGCPlayerJin2_2::OnCombo;
	ms_mapStateFunction[MID_JIN2_2_ATK4] = KGCPlayerJin2_2::OnCombo;
	ms_mapStateFunction[MID_JIN2_2_ATK5] = KGCPlayerJin2_2::OnCombo;
	ms_mapStateFunction[MID_JIN2_2_DASH_ATK] = KGCPlayerJin2_2::OnDashAtk;
	ms_mapStateFunction[MID_JIN2_2_DASH_ATK_1] = KGCPlayerJin2_2::OnDashAtkCombo;
	ms_mapStateFunction[MID_JIN2_2_DASH_ATK_2] = KGCPlayerJin2_2::OnDashAtkCombo;
	ms_mapStateFunction[MID_JIN2_2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_JIN2_2_TAG_END] = KGCTagPlayer::OnTagEnd;
	ms_mapStateFunction[MID_JIN2_2_TAG_START] = KGCTagPlayer::OnTagStart;
	ms_mapStateFunction[MID_JIN2_2_RUN_BURNNING_MODE] = KGCPlayerJin2_2Burnning::OnRun;
	ms_mapStateFunction[MID_JIN2_2_DASH_BURNNING_MODE] = KGCPlayerJin2_2Burnning::OnDash;
	ms_mapStateFunction[MID_JIN2_2_WAIT_BURNNING_MODE] = KGCPlayerJin2_2::OnWait;
	ms_mapStateFunction[MID_JIN2_2_RUN_TO_JUMP] = KGCPlayerJin2_2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_2_DASH_TO_JUMP] = KGCPlayerJin2_2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_2_DASH_ATK_TO_JUMP] = KGCPlayerJin2_2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_2_DASH_ATK_1_TO_JUMP] = KGCPlayerJin2_2::OnReturnToJumpOrWait;
	ms_mapStateFunction[MID_JIN2_2_DASH_ATK_2_TO_JUMP] = KGCPlayerJin2_2::OnReturnToJumpOrWait;

	//======================================================================================
	//                                     진 2차 끝
	//======================================================================================
	
	ms_mapStateFunction[MID_SIEG1_DASH_TO_WAIT] = KGCPlayerSieg1::OnDash;
	ms_mapStateFunction[MID_SIEG1_DASH_TO_JUMP] = KGCPlayerCommon::OnJump;
	ms_mapStateFunction[MID_SIEG1_DASH_ATK1] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_DASH_ATK1_F] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_JUMP_DASH] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_SIEG1_RUN] = KGCPlayerSieg1::OnRun;
	ms_mapStateFunction[MID_SIEG1_RUN_TO_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_SIEG1_RUN_TO_JUMP] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_SIEG1_ATK1] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK2] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK3] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK4] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK5] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK1_F] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK2_F] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK3_F] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK4_F] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_ATK5_F] = KGCPlayerSieg1::OnCombo;
	ms_mapStateFunction[MID_SIEG1_JUMP_ATK] = KGCPlayerSieg1::OnJumpAtk;
	ms_mapStateFunction[MID_SIEG1_DASH_TO_WAIT] = KGCPlayerSieg1::OnWait;
	ms_mapStateFunction[MID_SIEG1_DASH_TO_JUMP] = KGCPlayerCommon::OnJump;
	ms_mapStateFunction[MID_SIEG1_RUN_TO_WAIT] = KGCPlayerSieg1::OnWait;
	ms_mapStateFunction[MID_SIEG1_RUN_TO_JUMP] = KGCPlayerCommon::OnJump;
	ms_mapStateFunction[MID_SIEG1_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_SIEG1_SPECIAL3] = KGCPlayerSieg1::OnSpecial3;
	ms_mapStateFunction[MID_SIEG1_SPECIAL3_F] = KGCPlayerSieg1::OnSpecial3;

	// 연주모드
	ms_mapStateFunction[MID_AMY4_MELO_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY4_MELO_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY4_MELO_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY4_MELO_DASH] = KGCPlayerAmy4_2::OnDash;
	ms_mapStateFunction[MID_AMY4_MELO_DASH_ATK] = KGCPlayerAmy4_2::OnDashAtk;
	ms_mapStateFunction[MID_AMY4_MELO_DASH_ATK_TO_WAIT] = KGCPlayerAmy4_2::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_DASH_ATK_TO_JUMP] = KGCPlayerAmy4_2::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_JUMP_DASH] = KGCPlayerAmy4_2::OnJumpDash;
	ms_mapStateFunction[MID_AMY4_MELO_DASH_TO_WAIT] = KGCPlayerAmy4_2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_DASH_TO_JUMP] = KGCPlayerAmy4_2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_JUMP_DASH_TO_JUMP] = KGCPlayerAmy4_2::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_RUN] = KGCPlayerAmy4_2::OnRun;
	ms_mapStateFunction[MID_AMY4_MELO_RUN_TO_WAIT] = KGCPlayerAmy4_2::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_RUN_TO_JUMP] = KGCPlayerAmy4_2::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_JUMP] = KGCPlayerAmy4_2::OnJump;
	ms_mapStateFunction[MID_AMY4_MELO_DOUBLE_JUMP] = KGCPlayerAmy4_2::OnDoubleJump;
	ms_mapStateFunction[MID_AMY4_MELO_RUN_TO_DOUBLE_JUMP] = KGCPlayerAmy4_2::OnRunToDoubleJump;
	ms_mapStateFunction[MID_AMY4_MELO_ATK1] = KGCPlayerAmy4_2::OnCombo;
	ms_mapStateFunction[MID_AMY4_MELO_ATK2] = KGCPlayerAmy4_2::OnCombo;
	ms_mapStateFunction[MID_AMY4_MELO_ATK3] = KGCPlayerAmy4_2::OnCombo;
	ms_mapStateFunction[MID_AMY4_MELO_ATK4] = KGCPlayerAmy4_2::OnCombo;
	ms_mapStateFunction[MID_AMY4_MELO_ATK5] = KGCPlayerAmy4_2::OnCombo;
	ms_mapStateFunction[MID_AMY4_MELO_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;

	ms_mapStateFunction[MID_AMY4_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_AMY4_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_AMY4_JUMP] = KGCPlayerAmy4::OnJump;
	ms_mapStateFunction[MID_AMY4_JUMP_DASH] = KGCPlayerAmy4::OnJumpDash;
	ms_mapStateFunction[MID_AMY4_ATK1] = KGCPlayerAmy4::OnCombo;
	ms_mapStateFunction[MID_AMY4_ATK2] = KGCPlayerAmy4::OnCombo;
	ms_mapStateFunction[MID_AMY4_ATK3] = KGCPlayerAmy4::OnCombo;
	ms_mapStateFunction[MID_AMY4_ATK4] = KGCPlayerAmy4::OnCombo;
	ms_mapStateFunction[MID_AMY4_ATK5] = KGCPlayerAmy4::OnCombo;
	ms_mapStateFunction[MID_AMY4_ATK6] = KGCPlayerAmy4::OnCombo;
	ms_mapStateFunction[MID_AMY4_RUN] = KGCPlayerAmy4::OnRun;
	ms_mapStateFunction[MID_AMY4_RUN_TO_WAIT] = KGCPlayerAmy4::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY4_RUN_TO_JUMP] = KGCPlayerAmy4::OnRunToWaitJump;
	ms_mapStateFunction[MID_AMY4_DASH] = KGCPlayerAmy4::OnDash;
	ms_mapStateFunction[MID_AMY4_DOUBLE_JUMP] = KGCPlayerAmy4::OnDoubleJump;
	ms_mapStateFunction[MID_AMY4_DASH_TO_WAIT] = KGCPlayerAmy4::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY4_DASH_TO_JUMP] = KGCPlayerAmy4::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY4_JUMP_DASH_TO_JUMP] = KGCPlayerAmy4::OnDashToWaitJump;
	ms_mapStateFunction[MID_AMY4_RUN_TO_DOUBLE_JUMP] = KGCPlayerAmy4_2::OnRunToDoubleJump;
	ms_mapStateFunction[MID_AMY4_JUMP_ATK] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY4_DASH_ATK1] = KGCPlayerAmy4::OnDashAtk;
	ms_mapStateFunction[MID_AMY4_DASH_ATK2] = KGCPlayerAmy4::OnDashAtk;
	ms_mapStateFunction[MID_AMY4_DASH_ATK3] = KGCPlayerAmy4::OnDashAtk;
	ms_mapStateFunction[MID_AMY4_DASH_ATK1_TO_JUMP] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_DASH_ATK1_TO_WAIT] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_DASH_ATK2_TO_JUMP] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_DASH_ATK2_TO_WAIT] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_DASH_ATK3_TO_JUMP] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_DASH_ATK3_TO_WAIT] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_FIGHT_JUMP_UNIQUE2_TO_JUMP] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_MELO_JUMP_UNIQUE2_TO_JUMP] = KGCPlayerAmy4::OnDashAtkToWaitJump;
	ms_mapStateFunction[MID_AMY4_SELECT] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY4_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY4_MELO_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY4_LOSE] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_AMY4_FATAL] = KGCPlayerCommon::OnWait;

	ms_mapStateFunction[MID_AMY4_FIGHT_JUMP_UNIQUE_SKILL1] = KGCPlayerAmy4::OnJumpUniqueSkill;
	ms_mapStateFunction[MID_AMY4_MELO_JUMP_UNIQUE_SKILL1] = KGCPlayerAmy4::OnJumpUniqueSkill;
	ms_mapStateFunction[MID_AMY4_FIGHT_JUMP_UNIQUE_SKILL2] = KGCPlayerAmy4::OnJumpUniqueSkill2;
	ms_mapStateFunction[MID_AMY4_MELO_JUMP_UNIQUE_SKILL2] = KGCPlayerAmy4::OnJumpUniqueSkill2;
	ms_mapStateFunction[MID_AMY4_SWAP_TO_MELO_MODE] = KGCPlayerAmy4_2::OnModeChange;
	ms_mapStateFunction[MID_AMY4_SWAP_TO_FIGHT_MODE] = KGCPlayerAmy4_2::OnModeChange;
	ms_mapStateFunction[MID_AMY4_SWAP_TO_MELOMODE_WAIT] = KGCPlayerAmy4_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY4_SWAP_TO_MELOMODE_JUMP] = KGCPlayerAmy4_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY4_SWAP_TO_FIGHTMODE_WAIT] = KGCPlayerAmy4_2::OnModeChangeToWaitJump;
	ms_mapStateFunction[MID_AMY4_SWAP_TO_FIGHTMODE_JUMP] = KGCPlayerAmy4_2::OnModeChangeToWaitJump;

	ms_mapStateFunction[MID_AMY4_SPECIAL2] = KGCPlayerCommon::OnPositionFix;
	ms_mapStateFunction[MID_AMY4_MELO_SPECIAL3] = KGCPlayerCommon::OnPositionFix;
	ms_mapStateFunction[MID_AMY4_MELO_SPECIAL2] = KGCPlayerCommon::OnPositionFix;

	ms_mapStateFunction[MID_SIEG2_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_SIEG2_WALK] = KGCPlayerCommon::OnWalk;
	ms_mapStateFunction[MID_SIEG2_DASH] = KGCPlayerSieg2::OnDash;
	ms_mapStateFunction[MID_SIEG2_DASH_TO_WAIT] = KGCPlayerSieg2::OnDash;
	ms_mapStateFunction[MID_SIEG2_DASH_TO_JUMP] = KGCPlayerSieg2::OnDash;
	ms_mapStateFunction[MID_SIEG2_DASH_ATK1] = KGCPlayerSieg2::OnDashAtk;
	ms_mapStateFunction[MID_SIEG2_DASH_ATK1_F] = KGCPlayerSieg2::OnDashAtk;
	ms_mapStateFunction[MID_SIEG2_JUMP] = KGCPlayerSieg2::OnJump;
	ms_mapStateFunction[MID_SIEG2_JUMP_DASH] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_SIEG2_RUN] = KGCPlayerSieg2::OnRun;
	ms_mapStateFunction[MID_SIEG2_ATK1] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK2] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK3] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK4] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK5] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK1_F] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK2_F] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK3_F] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK4_F] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_ATK5_F] = KGCPlayerSieg2::OnCombo;
	ms_mapStateFunction[MID_SIEG2_JUMP_ATK] = KGCPlayerSieg2::OnJumpAtk;
	ms_mapStateFunction[MID_SIEG2_FURY_WAIT] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_SIEG2_FATAL] = KGCPlayerCommon::OnWait;
	ms_mapStateFunction[MID_SIEG2_WINPOS] = KGCPlayerCommon::OnEmptyEndFrameLock;
	ms_mapStateFunction[MID_SIEG2_DASH_POLE_JUMP] = KGCPlayerSieg2::OnPole;
	ms_mapStateFunction[MID_SIEG2_DASH_POLE_JUMP_ATK1] = KGCPlayerSieg2::OnPole;
	ms_mapStateFunction[MID_SIEG2_DASH_POLE_JUMP_ATK2] = KGCPlayerSieg2::OnPole;
	ms_mapStateFunction[MID_SIEG2_JUMP_ATK2] = KGCPlayerSieg2::OnJumpDownAtk;

	ms_mapStateFunction[MID_COMMON_PARALYSE] = KGCPlayerCommon::OnHold;

	ms_mapStateFunction[MID_AMY1_SKILL_COMBO1] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_SKILL_COMBO2] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_SKILL_COMBO3] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_SKILL_COMBO4] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_SKILL_COMBO_ADD1] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_SKILL_COMBO_ADD2] = KGCPlayerAmy::OnCombo;
	ms_mapStateFunction[MID_AMY1_SKILL_DANCE_COMBO4] = KGCPlayerAmy::OnCombo;

	ms_mapStateFunction[MID_JIN4_BURNNING_CHARGE] = KGCPlayerCommon::OnBurnningCharge;
}