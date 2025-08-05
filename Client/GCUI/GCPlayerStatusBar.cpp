#include "stdafx.h"
//
//
//
#include "../MyD3D.h"
#include "GCPlayerStatusBar.h"
#include "KncP2PLib/KncP2P.h"
#include "KSingleton.h"
#include "Replay.h"
#include "KGCRoomManager.h"
#include "GCSingleMenu.h"

IMPLEMENT_CLASSNAME(KGCPlayerStatusBar);
IMPLEMENT_WND_FACTORY(KGCPlayerStatusBar);
IMPLEMENT_WND_FACTORY_NAME(KGCPlayerStatusBar, "gc_player_status_bar");

KGCPlayerStatusBar::KGCPlayerStatusBar(void) : m_vBarPos(0.0f, 0.0f),
m_vHPGaugeTexCoords(0.0f, 0.0f, 0.0f, 0.0f),
m_ucFrmCount(0),
m_vNickNameBasePos(0.0f, 0.0f)
{
	m_iPlayerIdx = -1;

	m_pkHPGauge = NULL;
	m_pkName = NULL;
	m_pkFairPlayReport = NULL;

	m_pkPingAntenna = NULL;

	m_pkRank = NULL;

	m_pkDeathKill = NULL;
	m_pkDeathKillNumber = NULL;

	for (int i = 0; i < GC_CHAR_NUM; ++i)
	{
		m_pkChar[i] = NULL;
		m_pkTagChar[i] = NULL;

		char strCharName[MAX_PATH];

		sprintf(strCharName, "status_bar_char%d", i);
		LINK_CONTROL(strCharName, m_pkChar[i]);

		sprintf(strCharName, "status_bar_char%d_2", i);
		LINK_CONTROL(strCharName, m_pkTagChar[i]);
	}

	LINK_CONTROL("status_bar_hp_gauge", m_pkHPGauge);
	LINK_CONTROL("status_bar_player_name", m_pkName);

	//ping 관련
	LINK_CONTROL("ping_bg", m_pkPingBg);
	LINK_CONTROL("red_ping", m_pkRedPing);
	LINK_CONTROL("yellow_ping", m_pkYellowPing);
	LINK_CONTROL("green_ping", m_pkGreenPing);

	LINK_CONTROL("fairplay_report_menu", m_pkFairPlayReport);

	LINK_CONTROL("ping_antenna", m_pkPingAntenna);
	LINK_CONTROL("rank", m_pkRank);

	LINK_CONTROL("death_kill", m_pkDeathKill);
	LINK_CONTROL("death_kill_number", m_pkDeathKillNumber);
}

KGCPlayerStatusBar::~KGCPlayerStatusBar(void)
{
	// empty
}

void KGCPlayerStatusBar::OnCreate(void)
{
	m_iPlayerIdx = -1;
	m_vBarPos = GetFixedWindowPos();
	m_vHPGaugeTexCoords = m_pkHPGauge->GetWndTexCoords();

	for (int i = 0; i < GC_CHAR_NUM; i++)
	{
		m_pkChar[i]->SetSelfInputCheck(false);
		m_pkTagChar[i]->SetSelfInputCheck(false);
		m_pkTagChar[i]->ToggleRender(false);
	}

	m_pkHPGauge->SetSelfInputCheck(false);
	m_pkName->SetSelfInputCheck(false);

	if (m_vNickNameBasePos.x == 0.0f)
		m_vNickNameBasePos = m_pkName->GetFixedWindowLocalPos();

	m_pkPingBg->SetSelfInputCheck(false);
	m_pkRedPing->SetSelfInputCheck(false);
	m_pkYellowPing->SetSelfInputCheck(false);
	m_pkGreenPing->SetSelfInputCheck(false);

	//우선 다 꺼놓았다가 핑 함수에 따라서 킨다.
	m_pkPingBg->ToggleRender(false);
	m_pkRedPing->ToggleRender(false);
	m_pkYellowPing->ToggleRender(false);
	m_pkGreenPing->ToggleRender(false);

	m_pkFairPlayReport->InitState(false, true, this);
	m_pkRank->InitState(false);

	SetHP(1.0f);

#if defined(USE_PING_STATE_UI)
	m_pkPingAntenna->InitState(true);
#else
	m_pkPingAntenna->InitState(false);
#endif

	m_pkDeathKill->InitState(false);
	m_pkDeathKillNumber->InitState(false);
	m_pkDeathKillNumber->SetStringType("epic_num_enchant");
	m_pkDeathKillNumber->SetWriteToRight(false);
	m_pkDeathKillNumber->SetInterval(12.0f);
	m_pkDeathKillNumber->SetWriteAlign(ALIGN_RIGHT_SIDE);

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
	{
		m_pkDeathKill->ToggleRender(true);
		m_pkDeathKillNumber->ToggleRender(true);
	}
}

void KGCPlayerStatusBar::OnDestroy(void)
{
	SetHP(1.0f);
}

void KGCPlayerStatusBar::FrameMoveInEnabledState(void)
{
	if (m_iPlayerIdx < 0 || MAX_PLAYER_NUM <= m_iPlayerIdx)
		return;

	if (g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.nUserState != GC_RUS_PLAYING)
		return;

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);

#if !defined( DISABLE_BADGUY_REPORT )
	{
		if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL &&
			g_MyD3D->MyReplay->m_iCurrState != RP_VIEW)
		{
			if (g_pkInput->BtnUp(KInput::MBRIGHT))
			{
				if (m_pkFairPlayReport->IsRenderOn() == false)
				{
					if (g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.bLive && this->CheckPosInWindowBound(vMousePos))
					{
						m_pkFairPlayReport->ToggleRender(true);
						m_pkFairPlayReport->SetBadGuy(g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.strNickName);
					}
				}
				else
				{
					if (m_pkFairPlayReport->CheckPosInWindowBound(vMousePos) == false)
					{
						m_pkFairPlayReport->ToggleRender(false);
					}
				}
			}
			else if (g_pkInput->BtnUp(KInput::MBLEFT))
			{
				if (m_pkFairPlayReport->CheckPosInWindowBound(vMousePos) == false)
				{
					m_pkFairPlayReport->ToggleRender(false);
				}
			}
		}

		//신고하기를 눌렀는데 유저가 나가버리는 안타까운 상황
		if (g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.bLive == false)
			m_pkFairPlayReport->ToggleRender(false);
	}
#endif // !defined( DISABLE_BADGUY_REPORT )

	// 캐릭터
	int iCurCharType = g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.GetCurrentChar().iCharType;
	SetChar(iCurCharType);

	// Next Character Set
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		int iNextCharType = g_MyD3D->MyPlayer[m_iPlayerIdx]->GetNextTagChar();
		SetTagChar(iNextCharType == iCurCharType ? GC_CHAR_INVALID : iNextCharType);
	}

	// HP
	float fHP = g_MyD3D->MyPlayer[m_iPlayerIdx]->GetHP();
	float fFulllHP = g_MyD3D->MyPlayer[m_iPlayerIdx]->m_fAbility[ABILITY_HP];
	PLAYER *pPlayer = g_MyD3D->MyPlayer[m_iPlayerIdx];

	SetHP(fHP / fFulllHP);

	++m_ucFrmCount;

	if (m_ucFrmCount > 150)
	{
		m_ucFrmCount = 0;
		SetPing(KSingleton<KncP2P>::GetInstance()->GetAvgPartPingTime(g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.dwUID));
	}

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
	{
		m_pkDeathKillNumber->SetNumber(g_pGameBoard->GetHowKill(m_iPlayerIdx));
	}
}

void KGCPlayerStatusBar::PostChildDraw(void)
{
	int iLife = g_MyD3D->MyPlayer[m_iPlayerIdx]->m_cLife;

	if (iLife >= 10)
		return;

	if (m_iPlayerIdx < 0 || MAX_PLAYER_NUM <= m_iPlayerIdx)
		return;

	if (g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.bLive == false)
		return;

	// 대장전 ( 대장의 목숨만 표시하쟈 ㅠ )
	if (iLife > 10)
		return;

	if (g_kGlobalValue.m_bMUHANLife == true) {
		return;
	}

	//g_pkUIMgr->RenderNumber( "orange_num", GetRelocatedWindowPos() + D3DXVECTOR2( 164.0f * g_pGCDeviceManager->m_windowScale, 6.0f * g_pGCDeviceManager->m_windowScale), iLife, true, false );
	g_pkUIMgr->RenderNumber("orange_num", GetRelocatedWindowPos() + D3DXVECTOR2(164.0f * m_fWindowScaleX, 6.0f * m_fWindowScaleY), iLife, true, false, -1.0f, false);
}

void KGCPlayerStatusBar::SetPlayerIndex(int iIndex)
{
	m_iPlayerIdx = iIndex;

	if (iIndex < 0 || iIndex >= MAX_PLAYER_NUM) return;

	std::wstring strPlayerName = L"#c" + g_MyD3D->MyPlayer[m_iPlayerIdx]->GetStrUserColor() + g_MyD3D->MyPlayer[m_iPlayerIdx]->GetStrUserName() + L"#cx";

	if (SiKGCRoomManager()->GetGameMode() == GC_GM_AUTOMATCH_TEAM && g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOType == KELOUserData::MT_NORMAL) {
		UpdateMark();
		SetName(strPlayerName.c_str(), true);
	}
	else {
		m_pkRank->ToggleRender(false);
		SetName(strPlayerName.c_str(), false);
	}
}

void KGCPlayerStatusBar::UpdateMark(void)
{
	m_pkRank->ToggleRender(true);
	SetGradeTex((g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.GetCurrentChar().kELOUserData.m_ucGrade - 1), m_pkRank);
}

void KGCPlayerStatusBar::SetGradeTex(int index, KD3DWnd *pWnd)
{
	if (pWnd == NULL) return;

	if (index > 10 || index < 0) {
		return;
	}

	int x_index = index % 5;
	int y_index = index / 5;

	float tu = static_cast<float>(x_index) * 17.f + 378.f;
	float tv = static_cast<float>(y_index) * 17.f + 157.f;
	float tu1 = static_cast<float>(x_index) * 17.f + 394.f;
	float tv1 = static_cast<float>(y_index) * 17.f + 173.f;
	D3DXVECTOR4 vec(tu, tv, tu1, tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT, vec);
}

void KGCPlayerStatusBar::ChangeUVtoTexel(D3DXVECTOR4& vUV)
{
	float fMagicNumber = 0.5f;
	if (vUV.x > vUV.z)
	{
		fMagicNumber *= -1.0f;
	}
	vUV.x = (vUV.x + fMagicNumber) / 512.0f;
	vUV.z = (vUV.z - fMagicNumber) / 512.0f;

	fMagicNumber = 0.5f;
	if (vUV.y > vUV.w)
	{
		fMagicNumber *= -1.0f;
	}
	vUV.y = (vUV.y + fMagicNumber) / 512.0f;
	vUV.w = (vUV.w - fMagicNumber) / 512.0f;

	TEXEL_BOUND(vUV.x);
	TEXEL_BOUND(vUV.y);
	TEXEL_BOUND(vUV.z);
	TEXEL_BOUND(vUV.w);
}

void KGCPlayerStatusBar::SetHP(float fHP)
{
	if (fHP > 1.0f)
		fHP = 1.0f;

	m_pkHPGauge->SetWndTexCoords(D3DXVECTOR4(m_vHPGaugeTexCoords.x, m_vHPGaugeTexCoords.y,
		m_vHPGaugeTexCoords.x + fHP * (m_vHPGaugeTexCoords.z - m_vHPGaugeTexCoords.x),
		m_vHPGaugeTexCoords.w));
	m_pkHPGauge->SetWidth(static_cast<int>(fHP * MAX_LEN_HP_GAUGE));
}

void KGCPlayerStatusBar::SetChar(int iChar)
{
	for (int i = 0; i < GC_CHAR_NUM; i++)
	{
		m_pkChar[i]->ToggleRender(i == iChar);
	}
}

void KGCPlayerStatusBar::SetTagChar(int iChar)
{
	for (int i = 0; i < GC_CHAR_NUM; i++)
	{
		m_pkTagChar[i]->ToggleRender(i == iChar);
	}
}

void KGCPlayerStatusBar::SetName(const WCHAR* szName, bool bPosChange)
{
	m_pkName->SetText(szName);

	if (bPosChange) {
		D3DXVECTOR2 vPos = m_vNickNameBasePos;
		vPos.x += 16;
		m_pkName->SetWindowPosDirect(vPos);
	}
	else {
		m_pkName->SetWindowPosDirect(m_vNickNameBasePos);
	}
}

void KGCPlayerStatusBar::SetPing(int iPing)
{
	m_pkPingBg->ToggleRender(true);

	if (0 <= iPing && iPing < 120)
	{
		m_pkRedPing->ToggleRender(false);
		m_pkYellowPing->ToggleRender(false);
		m_pkGreenPing->ToggleRender(true);
	}
	else if (iPing >= 120 && iPing <= 180)
	{
		m_pkRedPing->ToggleRender(false);
		m_pkYellowPing->ToggleRender(true);
		m_pkGreenPing->ToggleRender(false);
	}
	else
	{
		m_pkRedPing->ToggleRender(true);
		m_pkYellowPing->ToggleRender(false);
		m_pkGreenPing->ToggleRender(false);
	}
}

void KGCPlayerStatusBar::ActionPerformed(const KActionEvent& event)
{
}

void KGCPlayerStatusBar::SetRoomMemberPingInfo(std::vector<std::pair<DWORD, DWORD>>& vecRoomMemberPingInfo_)
{
	if (m_iPlayerIdx < 0)
		return;

	for (int i = 0; i < static_cast<int>(vecRoomMemberPingInfo_.size()); i++)
	{
		if (g_MyD3D->MyPlayer[m_iPlayerIdx]->m_kUserInfo.dwUID == vecRoomMemberPingInfo_[i].first)
			m_pkPingAntenna->SetPingState(vecRoomMemberPingInfo_[i].second);
	}
}