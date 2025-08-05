#include "stdafx.h"
#include "KGCJumpingCharManager.h"

ImplementSingleton(KGCJumpingCharManager)

KGCJumpingCharManager::KGCJumpingCharManager()
	: m_bEnable(false)
	, m_nJumpingLevel(0)
{
	m_setCharList.clear();
}

KGCJumpingCharManager::~KGCJumpingCharManager()
{
}

bool KGCJumpingCharManager::CheckEnableJumping(char cChar)
{
	if (!m_bEnable)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_JUMPING_CHAR_ERROR1), L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return false;
	}

	if (m_setCharList.find(cChar) == m_setCharList.end())
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_JUMPING_CHAR_ERROR2), L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return false;
	}

	int nLevel = GetCharLevel(cChar);

	if (m_nJumpingLevel <= nLevel)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_JUMPING_CHAR_ERROR3), L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return false;
	}

	return true;
}

void KGCJumpingCharManager::DoJumpingCharReward()
{
	if (!m_bEnable)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_JUMPING_CHAR_ERROR1), L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, false);
		return;
	}

	KP2P::GetInstance()->Send_JumpingCharRewardReq();
}

void KGCJumpingCharManager::DoJumpingChar(char cChar)
{
	if (!CheckEnableJumping(cChar))
		return;

	KP2P::GetInstance()->Send_JumpingCharReq(cChar);
}

int KGCJumpingCharManager::GetCharLevel(char cChar)
{
	if (!g_kGlobalValue.m_kUserInfo.IsCharExist(cChar))
		return 0;

	return g_kGlobalValue.m_kUserInfo.GetCurrentChar(cChar).iLevel;
}