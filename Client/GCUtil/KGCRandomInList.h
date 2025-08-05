#pragma once
#include "GCRandomObj.h"
//#include <vector>
//#include <set>

namespace GCUTIL
{

	class KGCRandomInList	// 네트워크의 여러 클라이언트들의 싱크를 맞추려면 모든 함수를 동일한 타이밍에 호출해야 합니다.
	{
	public:
		KGCRandomInList(void);
		~KGCRandomInList(void);

		void SetSeed( DWORD dwSeed ) { m_kRandom.SetSeed( dwSeed ); }
		void SetRange( int iStart, int iEnd );
		void ClearList();
		void AddToList( int iValue );
		void RemoveFromList( int iValue );
		int	Rand();
	private:
		GCRand_Int	m_kRandom;
		std::vector<std::pair<int, bool> >	m_vecRangeValue;
		std::set<int>	m_setUsed;
		bool	m_bOverlapping;
		bool	m_bSelectableBoolean;//선택이 가능한 요소들을 의미한다. true이면 m_vecRangeValue의 second 값이 true인 것들 중에 선택한다.
	};

}