#ifndef _KGCJUMP_CHAR_MANAGER_H_
#define _KGCJUMP_CHAR_MANAGER_H_
#pragma once

#include "KNCSingleton.h"

class KGCJumpingCharManager
{
	DeclareSingleton(KGCJumpingCharManager);

public:
	bool                m_bEnable;
	int                 m_nJumpingLevel;
	std::set<char>      m_setCharList;

public:
	KGCJumpingCharManager();
	~KGCJumpingCharManager();

	void SetEnableEvent(bool bEnable) { m_bEnable = bEnable; }
	bool IsEnableEvent() { return m_bEnable; }

	void SetJumpingLevel(int nJumpingLevel) { m_nJumpingLevel = nJumpingLevel; }
	int  GetJumpingLevel() { return m_nJumpingLevel; }

	void SetCharList(const std::set<char>& setCharList) { m_setCharList = setCharList; }
	std::set<char>& GetCharList() { return m_setCharList; }

	bool CheckEnableJumping(char cChar);

	void DoJumpingCharReward();
	void DoJumpingChar(char cChar);

	int GetCharLevel(char cChar);
};

DefSingletonInline(KGCJumpingCharManager);

#endif
