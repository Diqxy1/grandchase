#include "stdafx.h"
#include ".\kgcrandominlist.h"

namespace GCUTIL
{

	KGCRandomInList::KGCRandomInList(void)
		: m_bOverlapping( false )
		, m_bSelectableBoolean( false )
	{
	}

	KGCRandomInList::~KGCRandomInList(void)
	{
	}

	void KGCRandomInList::SetRange( int iStart, int iEnd )
	{
		ClearList();
		m_vecRangeValue.reserve( iEnd - iStart + 1 );

		for( int i = iStart ; i <= iEnd ; ++i )
		{
			m_vecRangeValue.push_back( std::vector<std::pair<int, bool> >::value_type(i, m_bSelectableBoolean ) );
		}

		m_kRandom.SetRange(0, (int)m_vecRangeValue.size() );
	}

	int KGCRandomInList::Rand()
	{
		int bRet = -1;
		int iCount = 0;
		int iSize = (int)m_vecRangeValue.size();
		while( true )
		{
			int iWhere = m_kRandom.rand() % iSize;
			if( m_vecRangeValue[iWhere].second == m_bSelectableBoolean)
			{
				m_vecRangeValue[iWhere].second = !m_bSelectableBoolean;
				m_setUsed.insert( iWhere );
				return m_vecRangeValue[iWhere].first;
			}

			if( m_setUsed.size() == m_vecRangeValue.size() )
			{
				m_setUsed.clear();
				m_bSelectableBoolean = !m_bSelectableBoolean;
			}
		}
	}

	void KGCRandomInList::ClearList()
	{
		m_vecRangeValue.clear();
		m_setUsed.clear();
		m_bSelectableBoolean = false;
		m_kRandom.SetRange(0, 1000 );
	}

	void KGCRandomInList::AddToList(int iValue )
	{
		m_vecRangeValue.push_back( std::vector<std::pair<int, bool> >::value_type(iValue, m_bSelectableBoolean ) );
	}

	void KGCRandomInList::RemoveFromList( int iValue )
	{
		for( int i = 0 ; i < (int)m_vecRangeValue.size() ; ++i )
		{
			if( m_vecRangeValue[i].first == iValue )
			{
				m_vecRangeValue.erase( m_vecRangeValue.begin() + i );
				break;
			}
		}
	}
}