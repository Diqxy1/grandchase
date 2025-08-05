#include "ActionDecider.h"

void KActionUnit::AddActionProbability(int nActionID, float fProbability)
{
	vecSet.push_back(std::make_pair(nActionID, fProbability));
	fTotalProb = GetTotalProb() + fProbability;

	if (fTotalProb > 1.0f)
	{
		float fFactor = 1.0f / fTotalProb;

		std::vector< std::pair<int, float> >::iterator vit;
		for (vit = vecSet.begin(); vit != vecSet.end(); vit++)
		{
			vit->second *= fFactor;
		}

		fTotalProb = 1.0f;
	}
}

//void KActionUnit::ReplaceActionProbability( int nActionID, float fProbability )
//{
//    std::vector< std::pair<int,float> >::iterator vit;
//
//    vit = vecSet.begin();
//    while( vit != vecSet.end() )
//    {
//        if( vit->first == nActionID )
//        {
//            fTotalProb -= vit->second;
//            vit = vecSet.erase( vit );
//
//            continue;
//        }
//
//        vit++;
//    }
//
//    AddActionProbability( nActionID, fProbability );
//}

float KActionUnit::GetTotalProb(bool bCalc /*= false*/)
{
	if (bCalc)
	{
		fTotalProb = 0.0f;
		std::vector< std::pair<int, float> >::iterator vit;
		for (vit = vecSet.begin(); vit != vecSet.end(); vit++)
		{
			fTotalProb += vit->second;
		}
	}

	return fTotalProb;
}

//////////////////////////////////////////////////////////////////////////

KActionDecider::KActionDecider(int iInvalidAction /* = DEFAULT_INVALID_ACTION  */) : m_iInvalidAction(iInvalidAction)
{
}

KActionDecider::~KActionDecider()
{
}

void KActionDecider::AddAction(int iState, int iAction, float fProb)
{
	std::map<int, KActionUnit>::iterator atTableIt;
	atTableIt = m_mapActionTransitionTable.find(iState);

	if (atTableIt != m_mapActionTransitionTable.end())
	{
		atTableIt->second.AddActionProbability(iAction, fProb);
	}
	else
	{
		KActionUnit kSet;
		kSet.AddActionProbability(iAction, fProb);
		m_mapActionTransitionTable[iState] = kSet;
	}
}

//void KActionDecider::ReplaceProbability( int iState, int iAction, float fProb )
//{
//    std::map<int, KActionUnit>::iterator atTableIt;
//    atTableIt = m_mapActionTransitionTable.find( iState );
//
//    if( atTableIt != m_mapActionTransitionTable.end() )
//    {
//        atTableIt->second.ReplaceActionProbability( iAction, fProb );
//    }
//    else
//    {
//        return;
//        /*
//        KActionUnit *pkSet=new KActionUnit;
//        pkSet->AddActionProbability( kActionProb );
//        m_mapActionTransitionTable[iState] = pkSet;
//        */
//    }
//}
//
//void KActionDecider::ReplaceProbability( int iAction, float fProb )
//{
//    std::map<int, KActionUnit>::iterator atTableIt;
//
//    atTableIt = m_mapActionTransitionTable.begin();
//    while( atTableIt != m_mapActionTransitionTable.end() )
//    {
//        ReplaceProbability( atTableIt->first, iAction, fProb );
//
//        atTableIt++;
//    }
//}

void KActionDecider::MakeDecision(std::map<int, int>& mapResult)
{
	mapResult.clear();

	std::map<int, KActionUnit>::iterator mit;

	for (mit = m_mapActionTransitionTable.begin(); mit != m_mapActionTransitionTable.end(); mit++)
	{
		int iAction = MakeDecision(mit->first);

		if (iAction != m_iInvalidAction)
		{
			mapResult[mit->first] = iAction;
		}
	}
}

int KActionDecider::MakeDecision(int iState)
{
	float fRoulette = (float)rand() / (float)RAND_MAX;

	return MakeDecision(iState, fRoulette);
}

int KActionDecider::MakeDecision(int iState, float fRoulette)
{
	int nReturn = m_iInvalidAction;

	std::map<int, KActionUnit>::iterator mit;
	mit = m_mapActionTransitionTable.find(iState);
	if (mit != m_mapActionTransitionTable.end())
	{
		mit->second.Decision(fRoulette, nReturn);
	}

	return nReturn;
}

bool KActionUnit::Decision(IN float fRoulette, OUT int& nActionID)
{
	if (fRoulette < 0.0f)
	{
		fRoulette = 0.0f;
	}
	else if (fRoulette > 1.0f)
	{
		fRoulette = 1.0f;
	}

	if (fRoulette > GetTotalProb())    return false;
	if (vecSet.empty())                return false;

	std::vector<float> vecAccProb(vecSet.size());

	vecAccProb[0] = vecSet[0].second;

	for (size_t i = 1; i < vecSet.size(); i++)
	{
		vecAccProb[i] = vecSet[i].second + vecAccProb[i - 1];
	}

	int iSTART, iMIDDLE, iEND;
	iSTART = 0;
	iEND = vecSet.size() - 1;

	while (iSTART != iEND)
	{
		iMIDDLE = (iSTART + iEND) / 2;
		if (fRoulette > vecAccProb[iMIDDLE])
		{
			iSTART = iMIDDLE + 1;
		}
		else
		{
			iEND = iMIDDLE;
		}
	}

	nActionID = vecSet[iSTART].first;
	return true;
}

ImplToStringW(KActionUnit)
{
	std::vector< std::pair<int, float> >::const_iterator vit;
	for (vit = vecSet.begin(); vit != vecSet.end(); vit++)
	{
		stm_ << vit->first << L"( " << vit->second << L" ) ";
	}

	return stm_ << std::endl;
}

ImplToStringW(KActionDecider)
{
	stm_ << L"Number of States : " << m_mapActionTransitionTable.size() << std::endl;

	std::map<int, KActionUnit>::const_iterator mit;
	for (mit = m_mapActionTransitionTable.begin(); mit != m_mapActionTransitionTable.end(); mit++)
	{
		stm_ << L"State " << mit->first << L" : ";

		mit->second.ToString(stm_);
	}

	return stm_;
}

ImplOstmOperatorW2A(KActionDecider);
ImplOstmOperatorW(KActionDecider);