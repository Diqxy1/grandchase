#ifndef _KGCASTAR_H_
#define _KGCASTAR_H_

////#include <vector>

struct ROUTE{
	enum ACTION
	{ 
		NONE = -1,
		MOVE = 0,
		JUMP = 1,
		MOVE_JUMP = 2,
	};

	int m_iX;
	int m_iY;
	int m_iCost;		// m_iBeforeCost + m_iAfterCost
	int m_iBeforeCost;	// 현재까지 진행해온 Cost
	int m_iAfterCost;	// 앞으로 진행해갈 Cost
	ACTION m_eAction;

	ROUTE()
	{
		m_iX = 0;
		m_iY = 0;
		m_iCost = 0;
		m_iBeforeCost = 0;
		m_iAfterCost = 0;
		m_eAction = NONE;
	}
	ROUTE( int iX, int iY, int iBeforeCost, int iAfterCost )
	{
		m_iX = iX;
		m_iY = iY;
		m_iAfterCost = iAfterCost;
		m_iBeforeCost = iBeforeCost;
		m_iCost = iBeforeCost + iAfterCost;
		m_eAction = NONE;
	}
	bool operator == (const ROUTE& right) const
	{
		if( m_iX == right.m_iX && 
			m_iY == right.m_iY )
			return true;
		
		return false;
	}
};

class KGCRoute
{
public:
	KGCRoute();
	virtual ~KGCRoute();

	void Init();
	void SetMap( char* pMap, int iWidth, int iHeight );

	void SetPosition( int iStartX, int iStartY, int iEndX, int iEndY );
	void SetRange( int iRange ){ m_iRange = iRange; }

	std::vector< ROUTE > GetRoute(){ return m_vecOpenNode; }

	void Execute();

private:
	bool FindNeighborNode( ROUTE& node, std::vector< ROUTE >& vecOut );
	bool FindBestNode( std::vector< ROUTE >& vecTemp, ROUTE& out );
	bool FindNode( std::vector<ROUTE>& vec, int x, int y );
	void ComputeAction( ROUTE& start, ROUTE& end );
	void ActionSort();
	void ExecuteAlgo();

private:
	char* m_pMap;
	int m_iWidth;
	int m_iHeight;

	int m_iStartX;
	int m_iStartY;
	int m_iEndX;
	int m_iEndY;

	int m_iRange;

	std::vector< ROUTE > m_vecOpenNode;
	std::vector< ROUTE > m_vecCloseNode;
};

extern KGCRoute* g_pRoute;

#endif //_KGCASTAR_H_