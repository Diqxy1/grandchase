#pragma once

#include <vector>
#include <map>
#include <ToString.h>

#include "dbg/dbg.hpp"

#define DEFAULT_INVALID_ACTION -1

class KActionUnit
{
	DeclToStringW;
public:
	KActionUnit() : fTotalProb(0.0f) {}
	KActionUnit(const KActionUnit& t) { *this = t; }
	KActionUnit& operator=(const KActionUnit& t)
	{
		vecSet = t.vecSet;
		fTotalProb = t.fTotalProb;
		return *this;
	}

	// 동일한 ActionID를 두번 add하지 않음을 전제해야 한다. (별도로 체크하지 않음.)
	void AddActionProbability(int nActionID, float fProbability);
	//void ReplaceActionProbability( int nActionID, float fProbability );
	float GetTotalProb(bool bCalc = false);
	bool Decision(IN float fRoulette, OUT int& nActionID);

protected:
	std::vector< std::pair<int, float> > vecSet; // pair( int : sub-action id, float : probality )
	float fTotalProb;
};

class KActionDecider
{
	DeclToStringW;

public:
	KActionDecider(int iInvalidAction = DEFAULT_INVALID_ACTION);
	virtual ~KActionDecider();
	KActionDecider(const KActionDecider& t) { *this = t; }
	KActionDecider& operator=(const KActionDecider& t)
	{
		m_mapActionTransitionTable = t.m_mapActionTransitionTable;
		m_iInvalidAction = t.m_iInvalidAction;

		return *this;
	}

	//int GetInvalidAction() { return m_iInvalidAction; }
	void AddAction(int iState, int iAction, float fProb);
	//void ReplaceProbability( int iState, int iAction, float fProb );
	//void ReplaceProbability( int iAction, float fProb );
	void MakeDecision(std::map<int, int>& mapResult);
	int MakeDecision(int iState);
	int MakeDecision(int iState, float fRoulette);    // for debugging & unit testing.

protected:
	std::map<int, KActionUnit> m_mapActionTransitionTable;
	int m_iInvalidAction;
};

DeclOstmOperatorA(KActionDecider);
DeclOstmOperatorW(KActionDecider);