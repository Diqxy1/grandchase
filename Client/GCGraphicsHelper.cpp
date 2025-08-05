#include "stdafx.h"
#include ".\gcgraphicshelper.h"

////
#include "MyD3D.h"

//
//
#include "KGCRoomManager.h"

#include "GCSKT.h"
#include "KGCSocialMotionManager.h"
#include "KGCEmbarkManager.h"

GCGraphicsHelper* g_pGraphicsHelper = NULL;

GCGraphicsHelper::GCGraphicsHelper(LPDIRECT3DDEVICE9 d3ddevice)
{
	g_pGraphicsHelper = this;
	m_pd3dDevice = d3ddevice;

	for (int i = 0; i < (int)m_pTexCartoon.size(); ++i)
	{
		m_pTexCartoon[i] = NULL;
	}
	for (int i = 0; i < (int)m_vecMotionPlayerAction.size(); ++i)
	{
		m_vecMotionPlayerAction[i] = NULL;
	}

	m_mapCharMotion.clear();

	m_TexShadow = NULL;
	memset(&m_rtShadow, 0, sizeof(SHADOWRECT));
	memset(&m_Vertex, 0, sizeof(SLVERTEX) * 4);

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		m_pObject[i] = NULL;

	for (int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; ++i)
	{
		m_pMotionPlayerSelect[i] = NULL;
	}
}

GCGraphicsHelper::~GCGraphicsHelper(void)
{
	//m_pTexCartoon[ ] 의 Release는 하지 않음.. 알아서 지워지겠지..
	m_mapCharMotion.clear();

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		SAFE_DELETE(m_pObject[i]);
}

void GCGraphicsHelper::OnInitDevice()
{
	//LoadCartoonTexture( true );
	if (!GC_GLOBAL_DEFINE::bRealTimeLoadMotion)
	{
		LoadPlayerSelectMotion();
	}

	LoadExtraTexture();

	OnReloadActionScript();
}

void GCGraphicsHelper::LoadExtraTexture()
{
	char strTemp[MAX_PATH];

	for (int i = 0; i < TEX_EXTRA_MAX; i++)
	{
		sprintf(strTemp, "Extra%d.dds", i);
		m_pTexExtra[i] = g_pGCDeviceManager2->CreateTexture(strTemp);
	}

	if (m_TexShadow == NULL)
	{
		m_TexShadow = g_pGCDeviceManager2->CreateTexture("shadow.tga");
	}
}

void GCGraphicsHelper::SetExtraTexture(GCExtraTexture eExtraTexture)
{
	m_pTexExtra[eExtraTexture]->SetDeviceTexture();
}

void GCGraphicsHelper::LoadCartoonTexture(bool bUsingVS_)
{
	KGCLuaManager LuaMgr;
	HRESULT hr;

	for (int i = 0; i < (int)CARTOON_NUM; ++i)
	{
		if (m_pTexCartoon[i])
		{
			m_pTexCartoon[i]->Release();
		}
	}

	if (GCFUNC::LoadLuaScript(LuaMgr, "CartoonList.lua") == false)
	{
		MessageBoxA(NULL, "CartoonList.lua Load Failed!", NULL, MB_OK);
		exit(0);
	}

	if (bUsingVS_)
	{
		hr = LuaMgr.BeginTable("VS_CartoonTexture");
	}
	else
	{
		hr = LuaMgr.BeginTable("FP_CartoonTexture");
	}
	std::string strTextureName;
	for (int i = 0; i < (int)CARTOON_NUM; ++i)
	{
		LuaMgr.GetValue(i + 1, strTextureName);
		m_pTexCartoon[i] = g_pGCDeviceManager2->CreateTexture(strTextureName);
	}
	LuaMgr.EndTable();
}

void GCGraphicsHelper::SetCartoonTexture(EGCCartoonTexture eCartoonType)
{
	m_pTexCartoon[eCartoonType]->SetDeviceTexture(1);
}

void GCGraphicsHelper::LoadPlayerSelectMotion()
{
	char strTemp[MAX_PATH] = { 0, };
	for (int i = 0; i < CID_MAX; i++) {
		int iMotionNum = g_kGlobalValue.GetCharacterSelectMotionID(i);
		if (iMotionNum != -1)
		{
			if (iMotionNum < 1000)
				sprintf_s(strTemp, MAX_PATH, "%03d.FRM", iMotionNum);
			else
				sprintf_s(strTemp, MAX_PATH, "%d.FRM", iMotionNum);
		}

		m_pMotionPlayerSelect[i] = g_pGCDeviceManager2->CreateMotionFRM(strTemp);
	}
}

void GCGraphicsHelper::LoadPlayerSelectMotion(CHAR_IDENTY eCharType)
{
	if (eCharType < 0 || eCharType >= CID_MAX)
		return;

	char strTemp[MAX_PATH] = { 0, };

	int iMotionNum = g_kGlobalValue.GetCharacterSelectMotionID(eCharType);
	if (iMotionNum != -1)
	{
		if (iMotionNum < 1000)
			sprintf_s(strTemp, MAX_PATH, "%03d.FRM", iMotionNum);
		else
			sprintf_s(strTemp, MAX_PATH, "%d.FRM", iMotionNum);
	}

	m_pMotionPlayerSelect[eCharType] = g_pGCDeviceManager2->CreateMotionFRM(strTemp);
}

GCDeviceMotionFRM* GCGraphicsHelper::GetPlayerSelectMotion(CHAR_IDENTY eCharType)
{
	if ((int)eCharType < 0 || (GC_CHAR_NUM * NUM_JOB_LEVEL) <= (int)eCharType)
		return NULL;

	if (!m_pMotionPlayerSelect[eCharType])
	{
		LoadPlayerSelectMotion(eCharType);
	}

	return m_pMotionPlayerSelect[eCharType];
}

GCDeviceMotionFRM* GCGraphicsHelper::GetPlayerSelectMotionRoom(CHAR_IDENTY eCharType, int iMotionNum)
{
	if ((int)eCharType < 0 || (GC_CHAR_NUM * NUM_JOB_LEVEL) <= (int)eCharType)
		return NULL;

	if (!m_pMotionPlayerSelect[eCharType])
	{
		LoadPlayerSelectMotion(eCharType);
	}

	if ( ( !g_MyD3D->m_KGCOption.GetRoomAnimationEnable() && g_MyD3D->GetMyPlayer()->m_iPlayerIndex != g_pRoomAnimationMgr->iPlayerID) || g_MyD3D->GetMyPlayer()->m_iPlayerIndex == g_pRoomAnimationMgr->iPlayerID)
	{
		char strTemp[MAX_PATH] = { 0, };
		if (iMotionNum != -1)
		{
			sprintf_s(strTemp, MAX_PATH, "Room_%d_%d.FRM", iMotionNum, g_MyD3D->MyPlayer[ g_pRoomAnimationMgr->iPlayerID ]->GetCurrentChar().iCharType);
			return g_pGCDeviceManager2->CreateMotionFRM(strTemp);
		}
	}

	return m_pMotionPlayerSelect[eCharType];
}

GCDeviceMotionFRM* GCGraphicsHelper::GetPlayerActionMotion(int iMotionID)
{
	assert(m_vecMotionPlayerAction[iMotionID] != NULL);
	if ((int)m_vecMotionPlayerAction.size() <= iMotionID || iMotionID < 0)
		goto ErrorProc;

	if (m_vecMotionPlayerAction[iMotionID] == NULL)
	{
		goto ErrorProc;
	}

	if (m_vecMotionPlayerAction[iMotionID]->GetDeviceState() != GCDS_LOADED)
		goto ErrorProc;

	return m_vecMotionPlayerAction[iMotionID];
ErrorProc:
	return m_vecMotionPlayerAction[MID_COMMON_STONECURSE];
}

void GCGraphicsHelper::LoadPlayerActionMotionFull()
{
	for (int i = 0; i < GC_CHAR_NUM * NUM_JOB_LEVEL; i++)
	{
		ReservePlayerActionMotionByCharNum(i);
	}
	ReserveFormMotionWolf(CID_RYAN1);
	ReserveFormNephilim(CID_RYAN2);
	ReserveFormNephilim(CID_RYAN3);
	ReserveFormMDAN(CID_RYAN4);

	LoadReservedList();
}

void GCGraphicsHelper::ReservePlayerActionMotionByCharNum(char cCharNum, DWORD dwFlag)
{
	ReservePlayerActionOnScript(cCharNum, dwFlag);
}

void GCGraphicsHelper::LoadPlayerActionMotionByPlayerIndex(int iPlayerIndex, char cCharType, DWORD dwFlag)
{
	ReservePlayerActionMotionByPlayerIndex(iPlayerIndex, cCharType, dwFlag);
	LoadReservedList();
}

void GCGraphicsHelper::ReserveMetamorphosisMotion(int iPlayerIndex, char cCharType)
{
	// 매그너스 단은 아이템에 관계없이 사용할 수 있으므로...
	if (cCharType == CID_RYAN4)
		ReserveFormMDAN(cCharType);

	KCharInfoVector& vecChar = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.vecCharInfo;
	KCharInfoVector::iterator vit;
	for (vit = vecChar.begin(); vit != vecChar.end(); ++vit)
	{
		SCharInfo& charInfo = *vit;
		for (int i = 0; i <= charInfo.iPromotionLevel; ++i) {
			char Extra_Char_Num = charInfo.iCharType + i * GC_CHAR_NUM;
			if (Extra_Char_Num == cCharType) {
				ReserveMetamorphosisByItem(charInfo);
			}
		}
	}
}

void GCGraphicsHelper::ReserveMetamorphosisByItem(SCharInfo& charInfo_)
{
	KUserItemVector& vecItem = charInfo_.vecItemInfo;
	KUserItemVector::iterator vitItem;
	for (vitItem = vecItem.begin(); vitItem != vecItem.end(); ++vitItem)
	{
		SUserItem& item = *vitItem;

		switch (item.iItemID)
		{
		case KGCItemManager::ITEM_FULL_MOON_RING:
		case 3225:
		case 3227:
		case 3253:
		case 13713:
		case 13714:
			ReserveFormMotionWolf(charInfo_.iCharType);
			break;
		case 3953:
		case 3954:
		case 13715:
			ReserveFormNephilim(charInfo_.iCharType);
			break;
		}
	}
}

void GCGraphicsHelper::UnLoadPlayerActionMotion()
{
	for (int i = 0; i < (int)m_vecMotionPlayerAction.size(); i++)
	{
		if (m_vecMotionPlayerAction[i])
		{
			// 스킬 텍스쳐도 한번 읽어보시자
			char strTemp[MAX_PATH];
			sprintf(strTemp, "SkillImage%d.dds", i);
			g_pGCDeviceManager2->ReleaseDevice(strTemp);

			m_vecMotionPlayerAction[i]->Release();
			m_vecMotionPlayerAction[i] = NULL;
		}
	}
}

void GCGraphicsHelper::LoadPlayerActionMotion(char cExtraCharNum, int iMotionID)
{
	std::vector< int >& vecMotion = m_mapCharMotion[cExtraCharNum];
	std::vector< int >::iterator vit = std::find(vecMotion.begin(), vecMotion.end(), iMotionID);
	if (vit == vecMotion.end())
		m_mapCharMotion[cExtraCharNum].push_back(iMotionID);

	if (m_vecMotionPlayerAction[iMotionID] != NULL) //이미 로딩됨
		return;

	// 스킬 텍스쳐도 한번 읽어보시자
	char strTemp[MAX_PATH];
	sprintf(strTemp, "SkillImage%d.dds", iMotionID);
	if (g_pGCDeviceManager->GetMassFileManager()->IsIn(strTemp))
		g_pGCDeviceManager2->CreateTexture(strTemp);

	// 아이스픽시에게 얼려진 모션을 엔트 줄기 잡기 모션과 동일하게 쓰기 때문에.. (실제 파일을 복사해서 늘려줘도 되겠지..)
	int iLoadMotion = iMotionID;
	// 격투가꺼 그대로 가따씀 =+=;
	if (iMotionID >= MID_JIN2_2_BURNNING_CHARGE && iMotionID <= MID_JIN2_2_UNIQUE_HUJIN)
	{
		iLoadMotion -= 373;
	}

	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "MotionEnum.stg") == false)
		return;

	if (GCFUNC::LoadLuaScript(luaMgr, "SamePlayerMotion.stg") == false)
		return;

	std::map<int, int> mapSameMotionID;
	mapSameMotionID.clear();

	LUA_BEGIN_TABLE("SamePlayerMotion", return)
	{
		for (int i = 1; ; ++i)
		{
			int iSameMotion = -1;
			int iLoadMotion = -1;
			LUA_BEGIN_TABLE(i, break)
			{
				luaMgr.GetValue(1, iSameMotion);
				luaMgr.GetValue(2, iLoadMotion);
			}
			LUA_END_TABLE(break);
			mapSameMotionID[iSameMotion] = iLoadMotion;
		}
	}

	std::map<int, int>::iterator mitSameMotionID = mapSameMotionID.find(iMotionID);

	if (mitSameMotionID != mapSameMotionID.end())
		iLoadMotion = mitSameMotionID->second;
	else
	{
		switch (iMotionID)
		{
		case MID_SKILL_ELESIS1_MAGIC_DEFENSE: 
			iLoadMotion = MID_ELESIS_ATK1_1;
			break;
		}
	}

	char strFileName[MAX_PATH];
	sprintf(strFileName, "%03d.FRM", iLoadMotion);
	m_vecMotionPlayerAction[iMotionID] = g_pGCDeviceManager2->CreateMotionFRM(strFileName);

	// 이거 모션 이상한데~?
	if (m_vecMotionPlayerAction[iMotionID]->IsDisable())
	{
#if defined( _DEBUG )
		{
			std::wstringstream strText;
			std::wstringstream strCaption;
			strText << L"iMotionID = " << iMotionID << L" 모션이 이상함(파일이 없다?)";
			strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
			MessageBoxW(NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK);
		}
#endif
	}
}

bool GCGraphicsHelper::Verify(char cCharType, int iMotion)
{
	std::map< char, std::vector< int > >::iterator mit = m_mapCharMotion.find(cCharType);
	if (mit == m_mapCharMotion.end())
		return false;

	if (iMotion == MID_COMMON_INTROPOS)
	{
		return true;
	}

	std::vector< int >& vecCharMotion = mit->second;
	std::vector< int >::iterator vit;
	vit = std::find(vecCharMotion.begin(), vecCharMotion.end(), iMotion);
	return (vit != vecCharMotion.end());
}

bool GCGraphicsHelper::Verify(int iMotion)
{
	assert(m_vecMotionPlayerAction[iMotion] != NULL);
	if ((int)m_vecMotionPlayerAction.size() <= iMotion || iMotion < 0)
		return false;

	if (m_vecMotionPlayerAction[iMotion] == NULL)
		return false;

	if (m_vecMotionPlayerAction[iMotion]->GetDeviceState() != GCDS_LOADED)
		return false;

	return true;
}

void GCGraphicsHelper::OnReloadActionScript()
{
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "MotionEnum.stg") == false)
		return;

	int iMotionNum;
	luaMgr.GetValue("MID_TOTAL", iMotionNum);

	m_vecMotionPlayerAction.resize(iMotionNum, NULL);

	PLAYER::SetStateFunctions();
	g_MyD3D->GetMyPlayer()->SetSavingStateFunc();
}

int GCGraphicsHelper::GetFaceLiftTypeByPlayerHittedMotion(int iMotion_)
{
	std::map< int, int >::iterator mit = m_mapHittedCommonMotion.find(iMotion_);
	if (mit != m_mapHittedCommonMotion.end()) {
		return mit->second;
	}
	else {
		return -1;
	}
}

void GCGraphicsHelper::ReservePlayerHittedMotionByFaceLiftType()
{
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DOWN_IN_SKY, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_SMALLDAMAGE, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_BIGDAMAGE, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DOWN_AND_STANDUP, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_FRONTDOWN_AND_STANDUP, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_STONECURSE, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_STONECURSE_BY_MEDUSA, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DAMAGED_BY_FLYATK, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_FREEZED_BY_PIXIE, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_FREEZED_BY_ENT, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_FREEZED_BY_LICH, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_SKILL_ARME1_ICESTRIKE_FREEZING, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DAMAGED_BY_GUARD, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DAMAGED_BY_GUARD_ENABLE_COUNTER, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_HOLD, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_HOLD_NOEFFECT, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_PIERCING_DAMAGED, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_STUN, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_PIERCING_DAMAGED, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_PARALYSE, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_HOLD_UNTIL_ATTACKED, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_THROW_BUBBLE_GUM_BOMB, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DAMAGED_MID_SPECIAL, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DAMAGED_UPPER_SPECIAL, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_DAMAGED_BY_HELL_MOUTH, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_DAMAGED_BY_HELL_MOUTH_IGNORE_HYPERARMOR, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DOWN_IN_SKY_NO_CONTACT, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DOWN_IN_SKY_NO_STOP, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_ASIN1_SENDED, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_DAMAGED_COUNTER_SPIKE_TRAP, FACE_LIFT_HITTED));
	m_mapHittedCommonMotion.insert(std::make_pair(MID_COMMON_FRONT_FALL_DOWN, FACE_LIFT_HITTED));
}

void GCGraphicsHelper::ReservePlayerActionCOMMON(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_COMMON_DOWN_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_COMMON_SMALLDAMAGE);
	ReservePlayerActionMotion(cCharType, MID_COMMON_BIGDAMAGE);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DOWN_AND_STANDUP);
	ReservePlayerActionMotion(cCharType, MID_COMMON_ARENA);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DROP);
	ReservePlayerActionMotion(cCharType, MID_COMMON_FRONTDOWN_AND_STANDUP);
	ReservePlayerActionMotion(cCharType, MID_COMMON_STONECURSE);
	ReservePlayerActionMotion(cCharType, MID_COMMON_STONECURSE_BY_MEDUSA);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_BY_FLYATK);
	ReservePlayerActionMotion(cCharType, MID_COMMON_INTROPOS);
	ReservePlayerActionMotion(cCharType, MID_COMMON_TIGER_ROLLING_ACTION);
	ReservePlayerActionMotion(cCharType, MID_COMMON_INTO_THE_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_COMMON_FREEZED_BY_PIXIE);
	ReservePlayerActionMotion(cCharType, MID_COMMON_FREEZED_BY_ENT);
	ReservePlayerActionMotion(cCharType, MID_COMMON_FREEZED_BY_LICH);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_ICESTRIKE_FREEZING);
	ReservePlayerActionMotion(cCharType, MID_COMMON_WAIT_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_COMMON_INTO_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_BY_GUARD);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_BY_GUARD_ENABLE_COUNTER);
	ReservePlayerActionMotion(cCharType, MID_COMMON_HOLD);
	ReservePlayerActionMotion(cCharType, MID_COMMON_HOLD_NOEFFECT);
	ReservePlayerActionMotion(cCharType, MID_COMMON_PIERCING_DAMAGED);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_SPIKE_TRAP);
	ReservePlayerActionMotion(cCharType, MID_COMMON_STUN);
	ReservePlayerActionMotion(cCharType, MID_COMMON_PARALYSE);
	ReservePlayerActionMotion(cCharType, MID_COMMON_HOLD_UNTIL_ATTACKED);
	ReservePlayerActionMotion(cCharType, MID_COMMON_THROW_BUBBLE_GUM_BOMB);
	ReservePlayerActionMotion(cCharType, MID_COMMON_BUBBLE_GUM_SHIELD);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_MID_SPECIAL);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_UPPER_SPECIAL);
	ReservePlayerActionMotion(cCharType, MID_DAMAGED_BY_HELL_MOUTH);
	ReservePlayerActionMotion(cCharType, MID_DAMAGED_BY_HELL_MOUTH_IGNORE_HYPERARMOR);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DOWN_IN_SKY_NO_CONTACT);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DOWN_IN_SKY_NO_STOP);
	ReservePlayerActionMotion(cCharType, MID_ASIN1_SENDED);
	ReservePlayerActionMotion(cCharType, MID_DIO1_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_COMMON_SMALLDAMAGE_NO_COUNTER);
	ReservePlayerActionMotion(cCharType, MID_COMMON_BIGDAMAGE_NO_COUNTER);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DOWN_AND_STANDUP_NO_COUNTER);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DIO_SPECIAL4_HITTED);
	ReservePlayerActionMotion(cCharType, MID_NEPHILIM_DIO_SPECIAL4_HITTED);
	ReservePlayerActionMotion(cCharType, MID_MDAN_DIO_SPECIAL4_HITTED);
	ReservePlayerActionMotion(cCharType, MID_COMMON_AMY1_SPECIAL4_HITTED);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_COUNTER_SPIKE_TRAP);
	ReservePlayerActionMotion(cCharType, MID_COMMON_FRONT_FALL_DOWN);
	ReservePlayerActionMotion(cCharType, MID_RIN2_EVIL_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_RIN3_COMMON_GODDESS_UNIQUE_SKILL_1);
	ReservePlayerActionMotion(cCharType, MID_RIN4_COMMON_SPEICLA2_HOLD);
	ReservePlayerActionMotion(cCharType, MID_COMMON_FRONTDOWN_AND_STANDUP_NO_DIE);
	ReservePlayerActionMotion(cCharType, MID_COMMON_ZERO3_OS_UNIQUE_HOLD);
	ReservePlayerActionMotion(cCharType, MID_ZERO4_COMMON_DAMAGED_BLACKHALL);
	ReservePlayerActionMotion(cCharType, MID_COMMON_SLIDING);
	ReservePlayerActionMotion(cCharType, MID_COMMON_DAMAGED_DIO3_TENACIOUS_HOLD);
	ReservePlayerActionMotion(cCharType, MID_COMMON_LEY3_GAROUILLE_ICE_SLIDING);
	ReservePlayerActionMotion(cCharType, MID_COMMON_LEY3_HAUNT_CURSE4_DAMAGE);
	ReservePlayerActionMotion(cCharType, MID_COMMON_LEONA_TRANSFORM);
	ReservePlayerActionMotion( cCharType, MID_COMMON_MARI_NANO_DOWN );
	ReservePlayerActionMotion( cCharType, MID_COMMON_SLOW_BIGDAMAGE );

	std::set<int> setSocialMotion;
	if (SiKGCSocialMotionManager()->GetSocialMotion(setSocialMotion)) {
		std::set<int>::iterator sit = setSocialMotion.begin();
		for (sit; sit != setSocialMotion.end(); ++sit) {
			ReservePlayerActionMotion(cCharType, *sit);
		}
	}

	std::vector< int > vecEmoticonMotion;
	SiKGCSocialMotionManager()->GetEmoticonMotionList(vecEmoticonMotion);
	std::vector< int >::const_iterator vitMotion = vecEmoticonMotion.begin();
	for (; vitMotion != vecEmoticonMotion.end(); ++vitMotion) {
		ReservePlayerActionMotion(cCharType, *vitMotion);
	}

	std::vector<int> vecEmbarkMotionList = SiKGCEmbarkManager()->GetCharMotionList();
	std::vector<int>::iterator vecEmbarkMotionIter = vecEmbarkMotionList.begin();
	for (; vecEmbarkMotionIter != vecEmbarkMotionList.end(); ++vecEmbarkMotionIter)
	{
		ReservePlayerActionMotion(cCharType, *vecEmbarkMotionIter);
	}
}

void GCGraphicsHelper::ReservePlayerActionELE(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ELESIS_WAIT);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_WALK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_ATK1_5);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_DASH);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_START);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ELESIS);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_RUN);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_FLYATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_SPECIAL_MOONBLADE);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_GIGA_SLASH);
	ReservePlayerActionMotion(cCharType, MID_ELESIS_TORNADO_BLADE);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ELESIS123_BERSERK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP_ATK3);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_COMBO_A_DOWN);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_COMBO_A_DOWN_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_COMBO_A_UP_2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_COMBO_A_UP_2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_COMBO_A_DOWN_2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_COMBO_A_DOWN_2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_COMBO_A_DOWN_2_ATK3);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK3);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK4);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK5);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS1_MAGIC_DEFENSE);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionELE2(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_WALK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_DASH);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_ATK1_5);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_FASTATK_ON_COMBO1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_START);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ELESIS2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ELESIS2_SPECIAL_FLYING_IMPACT);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ELESIS123_BERSERK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS2_MAGIC_DEFENSE);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionELE3(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ELESIS123_BERSERK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_WAIT);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_WALK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_DASH);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_RUN);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_JUMPATK_UP);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_JUMPATK_DOWN);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_SHADOW_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ELE3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_START);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_ELESIS3_JUMPATK_DOWN_LAND);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS3_MAGIC_DEFENSE);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionELE4(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_WAIT);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_WALK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_RUN);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_DASH);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ELESIS4);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ATK1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ATK2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ATK3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ATK4);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ATK5);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_BACK_STEP);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_BACK_STEP_ATK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_ATK1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_ATK3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_ATK5);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_ATK6);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_JUMP_ATK_LAND);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_STANDUP_AND_ATTACK);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_START);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_ELESIS4_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ELESIS4_MAGIC_DEFENSE);
	ReservePlayerActionMotion(cCharType, MID_ELESIS123_BERSERK);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLIRE(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LIRE_LEGATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LIRE_WALK);
	ReservePlayerActionMotion(cCharType, MID_LIRE_DASH);
	ReservePlayerActionMotion(cCharType, MID_LIRE_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_LIRE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LIRE_DOUBLEJUMP);
	ReservePlayerActionMotion(cCharType, MID_LIRE_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_LIRE_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_LIRE_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_LIRE_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_LIRE_ATK1_5);
	ReservePlayerActionMotion(cCharType, MID_LIRE_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_LIRE_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LIRE_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_LIRE_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE_START);
	ReservePlayerActionMotion(cCharType, MID_LIRE_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LIRE_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LIRE);
	ReservePlayerActionMotion(cCharType, MID_LIRE_ARME_ARME2_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE_WIND_WARD);
	ReservePlayerActionMotion(cCharType, MID_LIRE_EAGLE_SITE);
	ReservePlayerActionMotion(cCharType, MID_LIRE_EXPLOSION_ARROW);
	ReservePlayerActionMotion(cCharType, MID_LIRE_LIRE2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LIRE_LIRE2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE_SIEGE_START);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE_SIEGE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE_SIEGE_ATK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE_SIEGE_END);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE1_DASH);
    ReservePlayerActionMotion( cCharType, MID_LIRE_LIRE2_FATAL );
    ReservePlayerActionMotion( cCharType, MID_LIRE_LIRE2_JOKE );

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLIRE2(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LIRE2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_WALK);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_DASH);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_START);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK1_5);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK2_1);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK2_2);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK2_3);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK2_4);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK2_5);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK3_1);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK3_2);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK3_3);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK3_4);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK3_5);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK1_STOP);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK2_STOP);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_ATK3_STOP);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LIRE2);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_SPECIAL1_DOWN);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_SPECIAL1_UP);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_JUMPPING_DOWN_ARROW_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_LAND_IN_DOWN_ARROW_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LIRE2_NARANTA);
	ReservePlayerActionMotion(cCharType, MID_LIRE_LIRE2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LIRE_LIRE2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE2_DASH);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLIRE3(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LIRE3_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_WALK);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_DASH);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_RUN);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_MELEEATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_JUMPATK1);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_JUMPATK2);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_LANDING);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LIRE3);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_JOKE);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_START);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LIRE_LIRE2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_JUMPATK3);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_DASHATK2);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_CRITICALATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_DOUBLEATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_CRITICAL_MELEEATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_CRITICAL_DASHATK2);
	ReservePlayerActionMotion(cCharType, MID_LIRE3_JUMP_DASH);

	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE3_DASH);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLIRE4(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LIRE4_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_JOKE);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_WALK);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DASH);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_RUN);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DOUBLEJUMP);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LIRE4);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_ATK1_5);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DOUBLE_ATK1);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DOUBLE_ATK2);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DASH_BACK_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_JUMP_ATK1);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DOUBLEJUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_START);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_DASH_BACK);
	ReservePlayerActionMotion(cCharType, MID_LIRE4_SHADOW_ATTACK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE4_DASH);
	ReservePlayerActionMotion(cCharType, MID_SKILL_LIRE4_DASH_TO_BACK);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LIRE_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionARME(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ARME_EARTHQUAKE);
	ReservePlayerActionMotion(cCharType, MID_ARME_WAIT_AND_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ARME_START);
	ReservePlayerActionMotion(cCharType, MID_ARME_WALK);
	ReservePlayerActionMotion(cCharType, MID_ARME_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_ARME_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_ARME_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_ARME_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_ARME_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_ARME_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ARME_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL1_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL2_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL3_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ARME_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_ARME_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ARME_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ARME);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL_SHINING_ARROW);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_DASH);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER);
	ReservePlayerActionMotion(cCharType, MID_ARME_SPECIAL_CONFUSION);
	ReservePlayerActionMotion(cCharType, MID_ARME_LAVA_FLOW);
	ReservePlayerActionMotion(cCharType, MID_ARME_BOOST_HEAL);
	ReservePlayerActionMotion(cCharType, MID_ARME_DARK_SWORM);
	ReservePlayerActionMotion(cCharType, MID_ARME_ATHENS_SWORD);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_WHITE2_ICESTRIKE);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_SPEED_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_DOWN);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_BACK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_B2_ENERGYBALL_LV1);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_B2_ENERGYBALL_LV2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_NEWCOMBO_ATK1);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_NEWCOMBO_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_NEWCOMBO_ATK3);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_NEWCOMBO_ATK4);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_WIZARD_DOLL);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_B1_FEAR_LV1);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_B1_FEAR_LV1_FOWARD);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_B1_FEAR_LV1_REAR);
	ReservePlayerActionMotion(cCharType, MID_SKILL_ARME1_STAND_UP_ATK);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionARME2(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ARME2_WAIT_AND_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ARME2_WALK);
	ReservePlayerActionMotion(cCharType, MID_ARME2_DASHATK);
	ReservePlayerActionMotion(cCharType, MID_ARME2_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_ARME2_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_ARME2_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_ARME2_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_ARME2_ATK1_5);
	ReservePlayerActionMotion(cCharType, MID_ARME2_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_ARME2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ARME2_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_ARME2_SPECIAL2_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME2_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ARME2_SPECIAL3_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME2_SPECIAL1_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ARME2);
	ReservePlayerActionMotion(cCharType, MID_ARME2_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ARME2_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ARME2_START);
	ReservePlayerActionMotion(cCharType, MID_ARME2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ARME2_SPECIAL_BIG_HAND);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_DASH);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_SPEED_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_DOWN);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_BACK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionARME3(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ARME3_WAIT);
	ReservePlayerActionMotion(cCharType, MID_ARME3_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ARME3_WALK);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_DASH);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER);
	ReservePlayerActionMotion(cCharType, MID_ARME3_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_ARME3_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SUMMON_MOVE);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SUMMON_WAIT);
	ReservePlayerActionMotion(cCharType, MID_ARME3_UNSUMMON);
	ReservePlayerActionMotion(cCharType, MID_ARME3_DASHATK1);
	ReservePlayerActionMotion(cCharType, MID_ARME3_DASHATK2);
	ReservePlayerActionMotion(cCharType, MID_ARME3_JUMPATK1);
	ReservePlayerActionMotion(cCharType, MID_ARME3_JUMPATK2);
	ReservePlayerActionMotion(cCharType, MID_ARME3_ATK1READY);
	ReservePlayerActionMotion(cCharType, MID_ARME3_ATK2READY);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SUMMON_ENDINE);
	ReservePlayerActionMotion(cCharType, MID_ARME3_DASHATK3);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SUMMON_SHYLPH);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SUMMON_UNDINE);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SUMMON_SALIST);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ARME3_START);
	ReservePlayerActionMotion(cCharType, MID_ARME3_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ARME_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ARME);
	ReservePlayerActionMotion(cCharType, MID_ARME3_SUMMON_JUMP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_SPEED_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_DOWN);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_BACK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionARME4(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_ARME4_WAIT);
	ReservePlayerActionMotion(cCharType, MID_ARME4_WALK);
	ReservePlayerActionMotion(cCharType, MID_ARME4_JOKE);
	ReservePlayerActionMotion(cCharType, MID_ARME4_FATAL);
	ReservePlayerActionMotion(cCharType, MID_ARME4_START);
	ReservePlayerActionMotion(cCharType, MID_ARME4_ATK1);
	ReservePlayerActionMotion(cCharType, MID_ARME4_ATK3);
	ReservePlayerActionMotion(cCharType, MID_ARME4_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_ARME4_JUMP);
	ReservePlayerActionMotion(cCharType, MID_ARME4_JUMP_ATK1);
	ReservePlayerActionMotion(cCharType, MID_ARME4_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_ARME4_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER);
	ReservePlayerActionMotion(cCharType, MID_ARME4_DASHATK1);
	ReservePlayerActionMotion(cCharType, MID_ARME4_DASHATK2);
	ReservePlayerActionMotion(cCharType, MID_ARME4_DASH);
	ReservePlayerActionMotion(cCharType, MID_ARME4_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_ARME4_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_ARME4_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_ARME4_SPECIAL1_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME4_SPECIAL2_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME4_SPECIAL3_BLACK);
	ReservePlayerActionMotion(cCharType, MID_ARME4_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_ARME4);
	ReservePlayerActionMotion(cCharType, MID_ARME4_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_SPEED_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_UP);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_DOWN);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME_TELEPORT_BACK);
	ReservePlayerActionMotion(cCharType, MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ARME_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLAS(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LAS_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LAS_WALK);
	ReservePlayerActionMotion(cCharType, MID_LAS_DASH);
	ReservePlayerActionMotion(cCharType, MID_LAS_RUN);
	ReservePlayerActionMotion(cCharType, MID_LAS_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_LAS_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_LAS_DOUBLEJUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS_DOUBLEJUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS_START);
	ReservePlayerActionMotion(cCharType, MID_LAS_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_LAS_FASTATK_ON_COMBO1);
	ReservePlayerActionMotion(cCharType, MID_LAS_DASHATK1);
	ReservePlayerActionMotion(cCharType, MID_LAS_DASHATK2);
	ReservePlayerActionMotion(cCharType, MID_LAS_DASHATK3);
	ReservePlayerActionMotion(cCharType, MID_LAS_DASHATK4);
	ReservePlayerActionMotion(cCharType, MID_LAS_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LAS_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS_ARROWDEF_ON_GROUND1);
	ReservePlayerActionMotion(cCharType, MID_LAS_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_LAS_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_LAS_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_LAS_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_LAS_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LAS_EVASION_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS_JOKE);
	ReservePlayerActionMotion(cCharType, MID_LAS_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LAS);
	ReservePlayerActionMotion(cCharType, MID_LAS_DOUBLEJUMP_LAND);
	ReservePlayerActionMotion(cCharType, MID_LAS_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS_SPECIAL2_FAIL);
	ReservePlayerActionMotion(cCharType, MID_LAS_EVASION);
	ReservePlayerActionMotion(cCharType, MID_LAS_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_LAS_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_LAS_DOUBLE_SLASH);
	ReservePlayerActionMotion(cCharType, MID_LAS_BLOOD_BASH);
	ReservePlayerActionMotion(cCharType, MID_LAS_GUARDIAN_BALL);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLAS2(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LAS2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LAS2_WALK);
	ReservePlayerActionMotion(cCharType, MID_LAS2_DASH_1);
	ReservePlayerActionMotion(cCharType, MID_LAS2_DASH_2);
	ReservePlayerActionMotion(cCharType, MID_LAS2_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_LAS2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS2_JUMPATK);
	ReservePlayerActionMotion(cCharType, MID_LAS2_START);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ATK1_1);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ATK1_2);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ATK1_3);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ATK1_4);
	ReservePlayerActionMotion(cCharType, MID_LAS2_JUMPDASH);
	ReservePlayerActionMotion(cCharType, MID_LAS2_JUMPDASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ARROW_EVASION_SKY);
	ReservePlayerActionMotion(cCharType, MID_LAS2_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS2_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_LAS2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LAS2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LAS2);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ARROWDEF_ON_GROUND1);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_LAS2_ARROW_EVASION_GROUND);
	ReservePlayerActionMotion(cCharType, MID_LAS2_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_LAS2_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_LAS2_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LAS2_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS2_DOUBLE_ATK);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLAS3(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LAS3_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS3_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LAS3_WALK);
	ReservePlayerActionMotion(cCharType, MID_LAS3_DASH);
	ReservePlayerActionMotion(cCharType, MID_LAS3_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_LAS3_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS3_UP_JUMPDASH);
	ReservePlayerActionMotion(cCharType, MID_LAS3_FRONT_JUMPDASH);
	ReservePlayerActionMotion(cCharType, MID_LAS3_DOWN_JUMPDASH);
	ReservePlayerActionMotion(cCharType, MID_LAS3_LANDING);
	ReservePlayerActionMotion(cCharType, MID_LAS3_ATK1);
	ReservePlayerActionMotion(cCharType, MID_LAS3_ATK2);
	ReservePlayerActionMotion(cCharType, MID_LAS3_ATK3);
	ReservePlayerActionMotion(cCharType, MID_LAS3_ATK4);
	ReservePlayerActionMotion(cCharType, MID_LAS3_ATK3_2);
	ReservePlayerActionMotion(cCharType, MID_LAS3_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS3_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_LAS3_DASH_ATK3);
	ReservePlayerActionMotion(cCharType, MID_LAS3_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS3_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LAS3);
	ReservePlayerActionMotion(cCharType, MID_LAS3_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_LAS3_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_LAS3_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LAS3_JOKE);
	ReservePlayerActionMotion(cCharType, MID_LAS_START);
	ReservePlayerActionMotion(cCharType, MID_LAS_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LAS_ARROWDEF_ON_GROUND1);
	ReservePlayerActionMotion(cCharType, MID_LAS_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_LAS_ARROWDEF_IN_SKY);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionLAS4(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_LAS4_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS4_FATAL);
	ReservePlayerActionMotion(cCharType, MID_LAS4_JOKE);
	ReservePlayerActionMotion(cCharType, MID_LAS4_WALK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DASH);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DASH_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DASH_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DOUBLE_DASH);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DOUBLE_DASH_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DOUBLE_DASH_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS4_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_LAS4_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS4_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_LAS4);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ATK1);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ATK2);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ATK3);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ATK4);
	ReservePlayerActionMotion(cCharType, MID_LAS4_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DASH_ATK_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DASH_ATK_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DOUBLE_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DOUBLE_DASH_ATK_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_LAS4_DOUBLE_DASH_ATK_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_LAS4_BOUNCE_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ARROWDEF_ON_GROUND1);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_LAS4_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_LAS4_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_LAS4_SPECIAL2_START);
	ReservePlayerActionMotion(cCharType, MID_LAS4_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_LAS4_START);
	ReservePlayerActionMotion(cCharType, MID_LAS4_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_LAS4_CHANGE_BODY_SKILL);
	ReservePlayerActionMotion(cCharType, MID_LAS4_SPECIAL2_NANDO);
	ReservePlayerActionMotion(cCharType, MID_LAS4_SPECIAL2_FINISH);
	ReservePlayerActionMotion(cCharType, MID_LAS4_SPECIAL3_FINISH);
	ReservePlayerActionMotion(cCharType, MID_LAS4_SPECIAL3_FINISH_LAST);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_LAS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionRyan(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_RYAN_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RYAN_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RYAN_WALK);
	ReservePlayerActionMotion(cCharType, MID_RYAN_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_RYAN_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RYAN);
	ReservePlayerActionMotion(cCharType, MID_RYAN_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RYAN_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RYAN_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN_JUMP_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN_JUMP_ATK2_LAND);
	ReservePlayerActionMotion(cCharType, MID_RYAN_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RYAN_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RYAN_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RYAN_START);
	ReservePlayerActionMotion(cCharType, MID_RYAN_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_ELE_ELE2_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_SKILL_RYAN_DASHATK_CANCEL);
	ReservePlayerActionMotion(cCharType, MID_RYAN_DOUBLE_ATK);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_START);
	}
}

void GCGraphicsHelper::ReserveFormMotionWolf(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_WOLF_FORM_START);
	ReservePlayerActionMotion(cCharType, MID_WOLF_FORM_END);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_WAIT);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_FATAL);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_JOKE);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_WALK);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_DASH);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_JUMP);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_FORM_WOLF);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ATK1);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ATK2);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ATK3);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ATK4);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_JUMP_ATK1);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_JUMP_ATK2_LAND);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_JUMP_ATK2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ARROWDEF_ON_GROUND2);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_WOLF_ARENA);
	ReservePlayerActionMotion(cCharType, MID_WOLF_DROP);
	ReservePlayerActionMotion(cCharType, MID_FORM_WOLF_CRITICAL);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_WOLF_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_WOLF_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_WOLF_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionRyan2(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_RYAN2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_WALK);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RYAN2);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ATK5);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ATK6);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_JUMP_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_JUMP_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_START);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_RUN);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_DOWN_AND_STAND_UP);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RYAN2_ARROW_REFLECTION);

	ReserveFormNephilim(cCharType);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_START);
	}
}

void GCGraphicsHelper::ReserveFormNephilim(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_FORM_START);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_WAIT);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_FATAL);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_WALK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_DASH1);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_LAND);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP_ATK1);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP_ATK3);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP_ATK4);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JUMP_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ATK1);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ATK2);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ATK3);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ATK4);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ATK5);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_JOKE);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_NEPHILLIM);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_SPECIAL);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_FORM_END);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_DOWN_AND_STAND_UP);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_DAMAGED_BY_FLY_ATK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_RUN);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_STONE_CURSE);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_COUNTER_DAMAGE);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_INTO_BASILISK);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_ARENA);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_DROP);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_FREEZED_BY_PIXIE);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_FREEZED_BY_ENT);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_FREEZED_BY_LICH);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_INTO_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_WAIT_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_THROW_BUBBLE_GUM_BOMB);
	ReservePlayerActionMotion(cCharType, MID_NEPHILLIM_BUBBLE_GUM_SHIELD);
	ReservePlayerActionMotion(cCharType, MID_NEPHILIM_HOLD);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_NEPHILLIM_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_NEPHILLIM_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_NEPHILLIM_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionRonan(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_RONAN_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RONAN_WALK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_RUN);
	ReservePlayerActionMotion(cCharType, MID_RONAN_DASH);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN_RESULT_LOSE);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ATK_UP1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ATK_UP2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RONAN_DASH_ATK_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RONAN);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ATK5);
	ReservePlayerActionMotion(cCharType, MID_RONAN_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_BACK_STEP);
	ReservePlayerActionMotion(cCharType, MID_RONAN_BACK_STEP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_SWORD_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_SWORD_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_SWORD_SPECIAL3_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_SWORD_SPECIAL3_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RONAN_START);
	ReservePlayerActionMotion(cCharType, MID_RONAN_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RONAN_RESULT_WIN);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RONAN_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP_DASH_ATK_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP_DASH_ATK_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN_DASH_ATK_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN_Z_COMBO_X_1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_Z_COMBO_X_2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_X_COMBO_Z);
	ReservePlayerActionMotion(cCharType, MID_RONAN_X_COMBO_1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_X_COMBO_2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_X_COMBO_3);
	ReservePlayerActionMotion(cCharType, MID_RONAN_X_COMBO_4);
	ReservePlayerActionMotion(cCharType, MID_RONAN_JUMP_X);
	ReservePlayerActionMotion(cCharType, MID_RONAN_DASH_X_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_CHANGE_MAGIC_SWORD);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_COMBO_1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_COMBO_2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_COMBO_3);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_COMBO_4);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_COMBO_BACKSTEP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_COMBO_DOUBLE);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN_MAGIC_SWORD_SPECIAL3);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionRyan3(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_RYAN3_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_WALK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_RUN);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_DASH_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_DASH_ATK_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_DASH_ATK_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP_DASH_ATK_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP_DASH_ATK_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_JUMP_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RYAN3);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_LEVEL_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_LEVEL_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_START);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_RESULT_WIN);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_RESULT_LOSE);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_FASTATK_ON_COMBO1);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_SPECIAL3_ATK_RIGHT);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_SPECIAL3_ATK_LEFT);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_DASH_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN3_BACKDASH_ATK);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_START);
	}
}
void GCGraphicsHelper::ReservePlayerActionRonan2(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_RONAN2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_WALK);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_RUN);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ATK5);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RONAN2);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK1_END_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK2_END_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK3_END_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK1_END_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK2_END_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_DASH_ATK3_END_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_JUMP_DOWN_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_JUMP_DOWN_ATK_END_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_JUMP_DOWN_ATK_END_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_BACK_STEP);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_BACK_STEP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_SWORD_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_SWORD_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_SWORD_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_MAGIC_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_MAGIC_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_MAGIC_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_WAIT_ROOM);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_START);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_RESULT_WIN);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_RESULT_LOSE);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RONAN2_ARROW_REFLECTION);
	ReservePlayerActionMotion( cCharType, MID_RONAN2_SPEAR_MODE);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_START);
	}
}

void GCGraphicsHelper::LoadPlayerModel(int iPlayerIndex, int iCharType)
{
	PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex];
	if (pPlayer->m_kUserInfo.bLive == false)
		return;

	SAFE_DELETE(m_pObject[iPlayerIndex]);
	m_pObject[iPlayerIndex] = new GCObject();
	m_pObject[iPlayerIndex]->SetMeshComputePaletteInterface((IGCComputePaletteP3M*)pPlayer);
	m_pObject[iPlayerIndex]->RemoveMesh(NULL);

	KItemIDVector vecItemSorted;

	SCharInfo& CharInfo = pPlayer->m_kUserInfo.GetCurrentChar(iCharType);
	for (DWORD i = 0x00010000; i > 1; i /= 2)
	{
		int j;
		KItemIDVector vecEquipList = CharInfo.GetEquipmentForLoadModel();
		for (j = 0; j < (int)vecEquipList.size(); j++)
		{
			if (g_pItemMgr->GetItemData(vecEquipList[j])->dwSlotPosition & i)
				break;

			if (j != (int)vecEquipList.size())
			{
				int k;
				for (k = 0; k < (int)vecItemSorted.size(); k++)
				{
					if (vecEquipList[j] == vecItemSorted[k])
						break;
				}

				if (k == (int)vecItemSorted.size())
					vecItemSorted.push_back(vecEquipList[j]);
			}
		}
	}

	int iPromotion = (pPlayer->Extra_Char_Num - CharInfo.iCharType) / GC_CHAR_NUM;
	for (int i = 0; i < (int)vecItemSorted.size(); i++)
	{
		char cIgBoneIdx = -1;
		DWORD dwItemID = vecItemSorted[i];
		if (g_pItemMgr->GetItemData(dwItemID)->dwSlotPosition & ESP_WEAPON)
		{
			g_pItemMgr->CreateAbtaModel(dwItemID, CharInfo.iCharType, iPromotion, m_pObject[iPlayerIndex]);
			if (CID_LAS4 == pPlayer->Extra_Char_Num || CID_RYAN4 == pPlayer->Extra_Char_Num || CID_RONAN3 == pPlayer->Extra_Char_Num || CID_AMY3 == pPlayer->Extra_Char_Num || CID_JIN3 == pPlayer->Extra_Char_Num)
			{
				g_pItemMgr->CreateLas4MergeAbtaModel(dwItemID, CharInfo.iCharType, m_pObject[iPlayerIndex]);
			}
		}
		else
		{
			// 진 장갑 한쪽 끄는 처리 해야 된다면 이쪽에서 예외처리 해주자!!!!
			if (CID_RONAN4 == pPlayer->Extra_Char_Num && g_pItemMgr->GetItemData(dwItemID)->dwSlotPosition & ESP_GLOVES)
			{
				cIgBoneIdx = 10;
			}

			if (CID_JIN1 == pPlayer->Extra_Char_Num && g_pItemMgr->GetItemData(dwItemID)->dwSlotPosition & ESP_GLOVES)
			{
				cIgBoneIdx = 13;
			}
			else if ((CID_LEY1 == pPlayer->Extra_Char_Num || CID_LEY2 == pPlayer->Extra_Char_Num) &&
				g_pItemMgr->GetItemData(dwItemID)->dwSlotPosition & ESP_GLOVES)
			{
				cIgBoneIdx = 10;
			}

			g_pItemMgr->CreateTmpModel(dwItemID, CharInfo.iCharType, iPromotion, cIgBoneIdx);
		}
	}

	if (pPlayer->Extra_Char_Num == CID_LIRE3)
		m_pObject[iPlayerIndex]->AddMesh(g_pGCDeviceManager2->CreateMeshP3M("Lire3Arrow.p3m"), g_pGCDeviceManager2->CreateTexture("Lire3Arrow.dds"));

	for (int k = 0; k < FORM_NUM; ++k)
	{
		if (pPlayer->m_FormTemplate[k].sMetamorphosisTemplate.iFormID != FORM_NORMAL)
			pPlayer->UpdateFormResource(k, m_pObject[iPlayerIndex]);
	}
}

void GCGraphicsHelper::LoadPlayerModelFull()
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
		if (pPlayer->m_kUserInfo.bLive == false)
			return;

		for (int j = 0; j < (int)pPlayer->m_kUserInfo.vecCharInfo.size(); ++j)
			LoadPlayerModel(i, pPlayer->m_kUserInfo.vecCharInfo[j].iCharType);
	}
}

void GCGraphicsHelper::UnLoadPlayerModel()
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		SAFE_DELETE(m_pObject[i]);
}

void GCGraphicsHelper::SimpleShadowRender(IN float fX, IN float fY, IN float fDistance, IN float fdefHalfWidth, IN float fZ)
{
	m_rtShadow.fX = fX;
	m_rtShadow.fY = fY;
	// 그림자 크기가 0이면 안그려도 된다
	if (fdefHalfWidth == 0.0f)
		return;

	// 펫 그림자 크기..
	m_rtShadow.fHalfWidth = max(0.0f, fdefHalfWidth - fDistance * 0.25f);
	m_rtShadow.fHalfHeight = m_rtShadow.fHalfWidth * 0.5f;

	float fLeft = m_rtShadow.fX - m_rtShadow.fHalfWidth;
	float fTop = m_rtShadow.fY + m_rtShadow.fHalfHeight;
	float fRight = m_rtShadow.fX + m_rtShadow.fHalfWidth;
	float fBottom = m_rtShadow.fY - m_rtShadow.fHalfHeight;

	if (m_TexShadow != NULL)
	{
		m_Vertex[0].SetSLVER(fLeft, fTop, fZ, 0.0f, 0.0f);
		m_Vertex[1].SetSLVER(fRight, fTop, fZ, 1.0f, 0.0f);
		m_Vertex[2].SetSLVER(fLeft, fBottom, fZ, 0.0f, 1.0f);
		m_Vertex[3].SetSLVER(fRight, fBottom, fZ, 1.0f, 1.0f);

		g_pGCDeviceManager2->DrawInWorld(m_TexShadow, m_Vertex);
	}
}

void GCGraphicsHelper::ReservePlayerActionRyan4(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_RYAN4_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_WALK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_RUN);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ATK5);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_BACK_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_BACK_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK_UP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK_DOWN);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RYAN4);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK3_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_DASH_ATK3_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_SWAP_TO_ONE_HAND);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_SWAP_TO_TWO_HAND);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_RUN);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_DASH_ATK_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_DASH_ATK_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_RUN_END_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_RUN_END_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_SPECIAL1_1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_SPECIAL1_2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_SPECIAL1_3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_SPECIAL2_1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_SPECIAL2_2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_START);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_RESULT_WIN);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_WINPOS);

	ReserveFormMDAN(cCharType);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN_START);
	}
}

void GCGraphicsHelper::ReserveFormMDAN(char cCharType)
{
	if (cCharType != CID_RYAN4)
		return;

	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_RUN);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_STANDUP_AND_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_MDAN);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_FREEZED_BY_LICH);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_FREEZED_BY_ENT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_FREEZED_BY_PIXIE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_INTO_BASILISK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_STONE_CURSE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_COUNTER_DAMAGE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_INTO_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_WAIT_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_TELEPORT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_ARENA);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DROP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DOWN_AND_STAND_UP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DAMAGED_BY_FLY_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_WALK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_FORM_START);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_FORM_END);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_FORM_START);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_ONE_HAND_FORM_END);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_BACK_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_SMALLDAMAGE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_BIGDAMAGE);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DOWN_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK2_BACK);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_DASH_ATK2_BACK_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_THROW_BUBBLE_GUM_BOMB);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_BUBBLE_GUM_SHIELD);
	ReservePlayerActionMotion(cCharType, MID_RYAN4_MDAN_HOLD);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN4_MDAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN4_MDAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RYAN4_MDAN_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionAmy(char cCharType)
{
	// Fighting Mode
	ReservePlayerActionMotion(cCharType, MID_AMY1_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY1_RUN);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY1_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_JUMPREADY);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ATK4);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ATK5);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ATK6);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ATK7);
	ReservePlayerActionMotion(cCharType, MID_AMY1_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DOUBLE_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY1_UNIQUE_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY1_UNIQUE_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_ATK1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_ATK1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DASH_ATK2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_FATAL);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DOUBLE_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DOUBLE_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY1_RUN_TO_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_UNIQUE_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_UNIQUE_ATK2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_AMY1_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_AMY1);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_AMY1_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_AMY1_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY1_STANDUP_AND_ATK);

	// Dancing Mode
	ReservePlayerActionMotion(cCharType, MID_AMY1_SWAP_TO_DANCE_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SWAP_TO_FIGHT_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_DASH_ATK1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_DASH_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_DOUBLE_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_UNIQUE_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_AMY1_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SWAP_TO_DANCEMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SWAP_TO_FIGHTMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SWAP_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY1_JOKE);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_JOKE);
	ReservePlayerActionMotion(cCharType, MID_AMY1_START);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY1_LOSE);
	ReservePlayerActionMotion(cCharType, MID_AMY1_SELECT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_SPECIAL2_LEFT);
	ReservePlayerActionMotion(cCharType, MID_AMY1_DANCE_SPECIAL3_LEFT);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY1_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY1_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY1_START);
	}
}

// 에이미 1차 준비 - 캐릭터의 모션정보를 로드하는 함수 - dolki
void GCGraphicsHelper::ReservePlayerActionAmy2(char cCharType)
{
	// 연주 모드
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_CATCH);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL2_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL3_BACK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL3_FORWORD);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL3_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_ATK4);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_AMY2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_SPECIAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_DASH_CMD_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_JUMPDOWN_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_JOKE);

	// 파이팅 모드
	ReservePlayerActionMotion(cCharType, MID_AMY2_RUN);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_FIGHT_CATCH_BY_AMY2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_AMY2_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY2_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SWAP_TO_PERFORMENCE_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SWAP_TO_FIGHT_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SWAP_TO_PERFORMENCEMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SWAP_TO_FIGHTMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SWAP_TO_FIGHTMODE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SWAP_TO_PERFORMENCE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY2_ATK4);
	ReservePlayerActionMotion(cCharType, MID_AMY2_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_RUN_TO_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_AMY2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_AMY2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_AMY2_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SPECIAL_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SPECIAL_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SPECIAL_ATK1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SPECIAL_ATK1_SKY_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DASH_ATK1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY2_DASH_ATK1_TO_WAIT);

	//임시 모션
	ReservePlayerActionMotion(cCharType, MID_AMY2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY2_START);
	ReservePlayerActionMotion(cCharType, MID_AMY2_SELECT);
	ReservePlayerActionMotion(cCharType, MID_AMY2_PERFORMENCE_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY2_LOSE);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionAmy3(char cCharType)
{
	// 댄스 모드
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_RUN);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_CATCH);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_SPECIAL3_LEFT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_RUN_TO_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_UNIQUE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DASH_ATK_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_DASH_ATK_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JOKE);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_FATAL);

	ReservePlayerActionMotion(cCharType, MID_AMY3_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_RUN);
	ReservePlayerActionMotion(cCharType, MID_AMY3_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_RUN_TO_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_ATK1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_ATK1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_ATK2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DASH_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY3_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY3_JUMP_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_CATCH);
	ReservePlayerActionMotion(cCharType, MID_AMY3_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_AMY3_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SPECIAL3_LEFT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ATK4);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ATK5);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ATK6);
	ReservePlayerActionMotion(cCharType, MID_AMY3_UNIQUE_SKILL1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_UNIQUE_SKILL1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_AMY3_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_AMY3_JOKE);
	ReservePlayerActionMotion(cCharType, MID_AMY3_FATAL);

	ReservePlayerActionMotion(cCharType, MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL1);
	ReservePlayerActionMotion(cCharType, MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL2);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_UNIQUE_SKILL2);
	ReservePlayerActionMotion(cCharType, MID_AMY3_FIGHT_JUMP_UNIQUE2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_FIGHT_JUMP_UNIQUE2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_UNIQUE2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_JUMP_UNIQUE2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SWAP_TO_DANCE_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SWAP_TO_FIGHT_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SWAP_TO_DANCEMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SWAP_TO_DANCEMODE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SWAP_TO_FIGHTMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SWAP_TO_FIGHTMODE_JUMP);

	ReservePlayerActionMotion(cCharType, MID_AMY3_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY3_DANCE_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY3_START);
	ReservePlayerActionMotion(cCharType, MID_AMY3_SELECT);
	ReservePlayerActionMotion(cCharType, MID_AMY3_LOSE);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionRonan3(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_RONAN3_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_WALK);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_RUN);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RONAN3);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_WAIT_ROOM);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_START);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_RESULT_WIN);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_RESULT_LOSE);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ATK5);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_ATK6);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_GUARD);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_SWORD_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_SWORD_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_SWORD_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_MAGIC_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_MAGIC_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_MAGIC_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_SWORD_SPECIAL3_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_SWORD_SPECIAL2_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_BLOCKING);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_SWORD_SPECIAL1_STOP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_SWORD_SPECIAL1_POST_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_JUMP_DOWN_ATK_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_MAGIC_SPECIAL2_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_ATK1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_ATK1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_ATK2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_DASH_ATK2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_JUMP_GUARD);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_PSYCHIC_FORCE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_PSYCHIC_FORCE_JUMP_UP);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_PSYCHIC_FORCE_JUMP_DOWN);
	ReservePlayerActionMotion(cCharType, MID_RONAN3_MAGIC_SPECIAL3_READY);
	ReservePlayerActionMotion( cCharType, MID_RONAN3_SHIELD_MODE_1);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionRonan4(char cCharType) //로난3차전직
{
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_FATAL);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WALK);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_RUN);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_ARROW_REFLECTION);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_CATCH);
	ReservePlayerActionMotion(cCharType, MID_COMMON_CATCH_BY_RONAN4);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WAIT_ROOM);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_START);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_RESULT_WIN);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_RESULT_LOSE);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_JOKE);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_ATK3);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_ATK4);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_JUMP_ATK_A1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_JUMP_ATK_A2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_CRITICAL_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DOUBLE_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A3);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_B1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_B2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MAGIC_SWORD_SPECIAL1_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MAGIC_SWORD_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MAGIC_SWORD_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MAGIC_SWORD_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WHITE_MAGIC_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WHITE_MAGIC_SPECIAL2_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WHITE_MAGIC_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WHITE_MAGIC_SPECIAL3_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_WHITE_MAGIC_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A3_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_A3_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_B1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_DASH_ATK_B2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_JUMP_ATK_B);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_UNIQUE_ATK_A1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_UNIQUE_ATK_A2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_AERIAL_COMBO_A1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_AERIAL_COMBO_A2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_AERIAL_COMBO_A3);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MAGIC_COMBO1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MAGIC_COMBO2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MONOVOLT_READY);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MONOVOLT_ATK1);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_MONOVOLT_ATK2);
	ReservePlayerActionMotion(cCharType, MID_RONAN4_SELECT);
	ReservePlayerActionMotion( cCharType, MID_RONAN4_BURNING_MODE );

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_RONAN_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionJin(char cCharType) // 진
{
	//일반모션
	ReservePlayerActionMotion(cCharType, MID_JIN_BURNNING_CHARGE);
	ReservePlayerActionMotion(cCharType, MID_JIN_START_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN_WALK);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_1);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_2);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_ATK_2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_JIN_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN_RUN);
	ReservePlayerActionMotion(cCharType, MID_JIN_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL1_FORCED);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL2_FORCED);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL3_FORCED);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL3_FORCED2);
	ReservePlayerActionMotion(cCharType, MID_JIN_SPECIAL3_FORCED3);
	ReservePlayerActionMotion(cCharType, MID_JIN_CATCH);
	ReservePlayerActionMotion(cCharType, MID_JIN_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_JIN_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN_FATAL);
	ReservePlayerActionMotion(cCharType, MID_JIN_START);
	ReservePlayerActionMotion(cCharType, MID_JIN_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_JIN_LOSE);
	ReservePlayerActionMotion(cCharType, MID_JIN_JOKE);
	ReservePlayerActionMotion(cCharType, MID_JIN_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_JIN_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_JIN_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN_ATK3);
	ReservePlayerActionMotion(cCharType, MID_JIN_ATK4);
	ReservePlayerActionMotion(cCharType, MID_JIN_ATK5);
	ReservePlayerActionMotion(cCharType, MID_JIN_UNIQUE_HANDWIND);
	ReservePlayerActionMotion(cCharType, MID_JIN_UNIQUE_HANDWIND_BP_EMPTY);
	ReservePlayerActionMotion(cCharType, MID_JIN_UNIQUE_SPINKICK);
	ReservePlayerActionMotion(cCharType, MID_JIN_UNIQUE_EVASION);
	ReservePlayerActionMotion(cCharType, MID_JIN_UNIQUE_HUJIN);

	//버닝모드 모션
	ReservePlayerActionMotion(cCharType, MID_JIN_RUN_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN_WALK_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN_DASH_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN_WAIT_BURNNING_MODE);

	//태그모션
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_ATK);
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_END);
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionJin1(char cCharType) // 진 1차전직
{
	//일반모션
	ReservePlayerActionMotion(cCharType, MID_JIN1_BURNNING_CHARGE);
	ReservePlayerActionMotion(cCharType, MID_JIN1_START_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN1_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_WALK);
	ReservePlayerActionMotion(cCharType, MID_JIN1_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN1_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_JIN1_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN1_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN1_RUN);
	ReservePlayerActionMotion(cCharType, MID_JIN1_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN1_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_ATK_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_ATK_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_ATK_1);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_ATK_1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_ATK_1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_CATCH);
	ReservePlayerActionMotion(cCharType, MID_JIN1_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_JIN1_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN1_FATAL);
	ReservePlayerActionMotion(cCharType, MID_JIN1_START);
	ReservePlayerActionMotion(cCharType, MID_JIN1_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_JIN1_LOSE);
	ReservePlayerActionMotion(cCharType, MID_JIN1_JOKE);
	ReservePlayerActionMotion(cCharType, MID_JIN1_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_JIN1_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL3_READY);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL1_FORCED);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL1_FORCED_FINAL);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL2_FORCED);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL3_FORCED);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SPECIAL3_FORCED_READY);
	ReservePlayerActionMotion(cCharType, MID_JIN1_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN1_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN1_ATK3);
	ReservePlayerActionMotion(cCharType, MID_JIN1_ATK4);
	ReservePlayerActionMotion(cCharType, MID_JIN1_ATK5);
	ReservePlayerActionMotion(cCharType, MID_JIN1_TUMBLING);
	ReservePlayerActionMotion(cCharType, MID_JIN1_TUMBLING_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN1_TUMBLING_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_AERIAL_STEP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN1_AERIAL_STEP_ATTACKED);
	ReservePlayerActionMotion(cCharType, MID_JIN1_AERIAL_STEP_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_TUMBLING);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_TUMBLING_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_TUMBLING_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_BURNNING_CHULSANGO);
	ReservePlayerActionMotion(cCharType, MID_JIN1_BURNNING_CHULSANGO_EMPTY);
	ReservePlayerActionMotion(cCharType, MID_JIN1_BURNNING_HUJIN);
	ReservePlayerActionMotion(cCharType, MID_JIN1_BURNNING_BISANGKWON);
	ReservePlayerActionMotion(cCharType, MID_JIN1_AERIAL_COMBO_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN1_AERIAL_COMBO_ATK1_EX);
	ReservePlayerActionMotion(cCharType, MID_JIN1_AERIAL_COMBO_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN1_AERIAL_COMBO_ATK3);
	ReservePlayerActionMotion(cCharType, MID_JIN1_SELECT);
	ReservePlayerActionMotion(cCharType, MID_JIN1_UNIQUE_EVASION);

	//버닝모드 모션
	ReservePlayerActionMotion(cCharType, MID_JIN1_WALK_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN1_DASH_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN1_RUN_BURNNING_MODE);

	//태그모션
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_ATK);
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_END);
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionSieg1(char cCharType)
{
	ReservePlayerActionMotion(cCharType, MID_SIEG1_WAIT);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_WALK);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_DASH);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_JUMP);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_RUN);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_SPECIAL3);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_CATCH);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK1);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK3);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK4);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK5);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_JUMP_ATK2);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_FURY_WAIT);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_FURY_WAIT_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_FATAL);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_FURY);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_SPECIAL1_F);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_SPECIAL2_F);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_SPECIAL3_F);

	ReservePlayerActionMotion(cCharType, MID_SIEG1_AERIAL);

	ReservePlayerActionMotion(cCharType, MID_SIEG1_JOKE);

	ReservePlayerActionMotion(cCharType, MID_SIEG1_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ARROWDEF_ON_GROUND2);

	ReservePlayerActionMotion(cCharType, MID_SIEG1_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_LOSE);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_RESULT_WIN);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_RESULT_LOSE);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_START);

	ReservePlayerActionMotion(cCharType, MID_SIEG1_DASH_ATK1_F);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK1_F);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK2_F);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK3_F);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK4_F);
	ReservePlayerActionMotion(cCharType, MID_SIEG1_ATK5_F);

	ReservePlayerActionMotion(cCharType, MID_SIEG1_ARROW_REFLECT);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_ELESIS_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionJin2(char cCharType)
{
	//일반모션
	ReservePlayerActionMotion(cCharType, MID_JIN2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_WALK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_JIN2_RUN);
	ReservePlayerActionMotion(cCharType, MID_JIN2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_JIN2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN2_BURNNING_CHARGE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_START_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_UNIQUE_EVASION);
	ReservePlayerActionMotion(cCharType, MID_JIN2_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_ATK3);
	ReservePlayerActionMotion(cCharType, MID_JIN2_ATK4);
	ReservePlayerActionMotion(cCharType, MID_JIN2_ATK5);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL1_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL1_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL1_FORCED_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL1_FORCED_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL2_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL2_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL2_FORCED_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL2_FORCED_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_START2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_FORCED_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_FORCED_START2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_FORCED_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SPECIAL3_FORCED_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_TUMBLING);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_TUMBLING_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_TUMBLING_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_ATK_1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_ATK_1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_ATK_1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DAMA);
	ReservePlayerActionMotion(cCharType, MID_JIN2_BURNNING_DAMA);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_ATK_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_ATK_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_DOWN_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JUMP_DOWN_ATK_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_ATK_1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_AERIAL_COMBO_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_AERIAL_COMBO_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_AERIAL_COMBO_ATK3);
	ReservePlayerActionMotion(cCharType, MID_JIN2_AERIAL_COMBO_ATK4);
	ReservePlayerActionMotion(cCharType, MID_JIN2_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_SELECT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_JIN2_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_JIN2_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_JIN2_LOSE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_BURNNING_HUJIN);
	ReservePlayerActionMotion(cCharType, MID_JIN2_THROWWEAPON);
	ReservePlayerActionMotion(cCharType, MID_JIN2_AFTERTHROW_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_TO_JIN2_WAIT);

	//버닝모드 모션
	ReservePlayerActionMotion(cCharType, MID_JIN2_WALK_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_DASH_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_RUN_BURNNING_MODE);

	//일반모션
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_BURNNING_CHARGE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_START_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_WALK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_1_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_1_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_ATK_2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_RUN);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_SPECIAL1_FORCED);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_CATCH);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_FATAL);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_START);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_LOSE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_JOKE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_ATK1);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_ATK2);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_ATK3);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_ATK4);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_ATK5);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_UNIQUE_HANDWIND);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_UNIQUE_HANDWIND_BP_EMPTY);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_UNIQUE_SPINKICK);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_UNIQUE_EVASION);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_UNIQUE_HUJIN);

	//버닝모드 모션
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_RUN_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_WALK_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_DASH_BURNNING_MODE);
	ReservePlayerActionMotion(cCharType, MID_JIN2_2_WAIT_BURNNING_MODE);

	//태그모션
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_JIN2_2_TAG_ATK);
		ReservePlayerActionMotion(cCharType, MID_JIN2_2_TAG_END);
		ReservePlayerActionMotion(cCharType, MID_JIN2_2_TAG_START);

		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_ATK);
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_END);
		ReservePlayerActionMotion(cCharType, MID_JIN_TAG_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionMotion(char cCharType, int iMotionID)
{
	m_vecReserveForLoad.push_back(std::make_pair(cCharType, iMotionID));
}

void GCGraphicsHelper::LoadReservedList()
{
	for (std::vector< std::pair < int, int > >::iterator vit = m_vecReserveForLoad.begin(); vit != m_vecReserveForLoad.end(); vit++)
	{
		LoadPlayerActionMotion(vit->first, vit->second);
	}
}

void GCGraphicsHelper::ReservePlayerActionMotionByPlayerIndex(int iPlayerIndex, char cCharType, DWORD dwFlag)
{
	m_vecReserveForLoad.clear();
	ReservePlayerActionCOMMON(cCharType);
	ReservePlayerActionMotionByCharNum(cCharType, dwFlag);

	m_mapHittedCommonMotion.clear();
	ReservePlayerHittedMotionByFaceLiftType();

	if (cCharType == CID_RYAN1 ||
		cCharType == CID_RYAN2 ||
		cCharType == CID_RYAN3 ||
		cCharType == CID_RYAN4 ||
		g_MyD3D->MyPlayer[iPlayerIndex]->CheckEquipItem(KGCItemManager::ITEM_FULL_MOON_RING))
		ReserveMetamorphosisMotion(iPlayerIndex, cCharType);
}

void GCGraphicsHelper::ReservePlayerActionAmy4(char cCharType)
{
	// 댄스 모드
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_DASH_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_RUN);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_CATCH);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_SPECIAL3);

	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_RUN_TO_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_ATK4);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_ATK5);

	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JOKE);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_FATAL);

	ReservePlayerActionMotion(cCharType, MID_AMY4_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_WALK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_RUN);
	ReservePlayerActionMotion(cCharType, MID_AMY4_RUN_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_RUN_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_RUN_TO_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY4_DASH_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_DASH_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_DASH_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY4_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_JUMP_READY);
	ReservePlayerActionMotion(cCharType, MID_AMY4_JUMP_DASH);
	ReservePlayerActionMotion(cCharType, MID_AMY4_JUMP_DASH_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_JUMP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_DOUBLE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_CATCH);
	ReservePlayerActionMotion(cCharType, MID_AMY4_CATCHED);
	ReservePlayerActionMotion(cCharType, MID_AMY4_CRITICAL_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_DOUBLE_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_STAND_UP_ATK);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SPECIAL1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SPECIAL2);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SPECIAL3);

	ReservePlayerActionMotion(cCharType, MID_AMY4_ATK1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_ATK2);
	ReservePlayerActionMotion(cCharType, MID_AMY4_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY4_ATK4);
	ReservePlayerActionMotion(cCharType, MID_AMY4_ATK5);

	ReservePlayerActionMotion(cCharType, MID_AMY4_UNIQUE_SKILL1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP_UNIQUE_SKILL1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_ARROWDEF_ON_GROUND);
	ReservePlayerActionMotion(cCharType, MID_AMY4_ARROWDEF_IN_SKY);
	ReservePlayerActionMotion(cCharType, MID_AMY4_JOKE);
	ReservePlayerActionMotion(cCharType, MID_AMY4_FATAL);

	ReservePlayerActionMotion(cCharType, MID_AMY4_FIGHT_JUMP_UNIQUE_SKILL1);
	ReservePlayerActionMotion(cCharType, MID_AMY4_FIGHT_JUMP_UNIQUE_SKILL2);

	ReservePlayerActionMotion(cCharType, MID_AMY4_FIGHT_JUMP_UNIQUE2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_FIGHT_JUMP_UNIQUE2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP_UNIQUE2_TO_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_JUMP_UNIQUE2_TO_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SWAP_TO_MELO_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SWAP_TO_FIGHT_MODE);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SWAP_TO_MELOMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SWAP_TO_MELOMODE_JUMP);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SWAP_TO_FIGHTMODE_WAIT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SWAP_TO_FIGHTMODE_JUMP);

	ReservePlayerActionMotion(cCharType, MID_AMY4_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_WINPOS);
	ReservePlayerActionMotion(cCharType, MID_AMY4_START);
	ReservePlayerActionMotion(cCharType, MID_AMY4_SELECT);
	ReservePlayerActionMotion(cCharType, MID_AMY4_LOSE);

	ReservePlayerActionMotion(cCharType, MID_AMY4_DASH_ATK3);
	ReservePlayerActionMotion(cCharType, MID_AMY4_MELO_DASH_ATK2);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_ATK);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_END);
		ReservePlayerActionMotion(cCharType, MID_TAG_AMY2_START);
	}
}

void GCGraphicsHelper::ReservePlayerActionOnScript(char cCharType, DWORD dwFlag)
{
	PLAYER_TEMPLATE pt;
	std::vector< EGCSkillTree > dummy;
	SetPlayerTemplate(dummy, cCharType, pt);

	if (0 == dwFlag || (dwFlag & LOADMOTION_STANDARD))
	{
		std::for_each(pt.setStandardCharMotion.begin(), pt.setStandardCharMotion.end(),
			boost::bind(&GCGraphicsHelper::ReservePlayerActionMotion, boost::ref(*g_pGraphicsHelper), cCharType, _1));
	}

	if (0 == dwFlag || (dwFlag & LOADMOTION_EXTEND))
	{
		std::for_each(pt.setExtendCharMotion.begin(), pt.setExtendCharMotion.end(),
			boost::bind(&GCGraphicsHelper::ReservePlayerActionMotion, boost::ref(*g_pGraphicsHelper), cCharType, _1));
	}
}

int GCGraphicsHelper::GetPlayerMotionNum()
{
	return m_vecMotionPlayerAction.size();
}