#include "KMap.h"

// T1 : FH
template <class T1>
KMap<T1>::KMap(void) : m_fWidth(0), m_fHeight(0), m_iFHWidth(0), m_iFHHeight(0),
m_ppFHWorld(NULL)
{
}

template <class T1>
KMap<T1>::~KMap(void)
{
}

template <class T1>
void KMap<T1>::AddFootHold(T1* _iFootHold)
{
	int x = _iFootHold->GetIndexX();
	int y = _iFootHold->GetIndexY();
	BoundCheck(x, y);
	m_ppFHWorld[x][y] = *_iFootHold;
}

template <class T1>
void KMap<T1>::LoadFootHoldData(KLuaManager& luaMgr)
{
	LUA_BEGIN_TABLE("FOOTHOLD_DATA", return)
	{
		for (int i = 1; ; ++i)
		{
			LUA_BEGIN_TABLE(i, break)
			{
				AddDungeonFootHold(luaMgr);
			}
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(return);
}