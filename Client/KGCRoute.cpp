#include "stdafx.h"
#include "KGCRoute.h"

KGCRoute* g_pRoute = NULL;

KGCRoute::KGCRoute()
{
	g_pRoute = this;
	Init();
}

KGCRoute::~KGCRoute()
{
}

void KGCRoute::Init()
{
	m_pMap = NULL;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iEndX = 0;
	m_iEndY = 0;
	m_iRange = 1;

	m_vecOpenNode.clear();
	m_vecCloseNode.clear();
}

void KGCRoute::SetMap( char* pMap, int iWidth, int iHeight )
{
	m_pMap = pMap;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

void KGCRoute::SetPosition( int iStartX, int iStartY, int iEndX, int iEndY )
{
	m_iStartX = iStartX;
	m_iStartY = iStartY;
	m_iEndX = iEndX;
	m_iEndY = iEndY;
}

void KGCRoute::Execute()
{
	ExecuteAlgo();
}

void KGCRoute::ExecuteAlgo()
{
	m_vecOpenNode.clear();
	m_vecCloseNode.clear();

	// 시작 노드 생성
	ROUTE node( m_iStartX, m_iStartY, 0, 0 );
	m_vecOpenNode.push_back( node );

	while(1)
	{
		// 열린 노드가 비었으면 경로가 없음
		if( m_vecOpenNode.empty() == true )
			break;

		std::vector< ROUTE > vecTemp;

		// 인접한 노드검색
		if( FindNeighborNode( node, vecTemp ) == false )
		{
			// 탐색하지 않은 새로운 노드가 없음
			int iEnd = (int)m_vecOpenNode.size()-1;
			node = m_vecOpenNode[iEnd];
			m_vecOpenNode.erase( m_vecOpenNode.begin() + iEnd );
		}
		else
		{
			ROUTE best;
			bool bEnd;

			// 최적의 노드 검색
			bEnd = FindBestNode( vecTemp, best );

			// 이전 노드에서 추가되는 노드로의 Action 부여
			ComputeAction( node, best );

			m_vecOpenNode.push_back( best );
			m_vecCloseNode.push_back( node );

			node = best;

			// 경로를 찾았음
			if( bEnd )
				break;
		}
	}

	ActionSort();
}

void KGCRoute::ComputeAction( ROUTE& start, ROUTE& end )
{
	if( start == end )
	{
		end.m_eAction = ROUTE::NONE;
		return;
	}

	end.m_eAction = ROUTE::MOVE;
	int iDiffX = abs(end.m_iX - start.m_iX);
	int iDiffY = abs(end.m_iY - start.m_iY);
	if( iDiffX >= 2 || iDiffY >= 2 )
	{
		if( iDiffX == 0 )
			end.m_eAction = ROUTE::JUMP;
		else
			end.m_eAction = ROUTE::MOVE_JUMP;
	}
}

void KGCRoute::ActionSort()
{
	int iBefore = (int)m_vecOpenNode.size()-1;
	for( int i = (int)m_vecOpenNode.size() - 2 ; i >= 0 ; --i, --iBefore )
	{
		// Move Action은 중복노드를 제거한다
		ROUTE::ACTION eAction = m_vecOpenNode[i].m_eAction;
		if( eAction == ROUTE::MOVE && m_vecOpenNode[iBefore].m_eAction == eAction )
		{
			m_vecOpenNode.erase( m_vecOpenNode.begin()+i );
		}
	}
}

bool KGCRoute::FindNeighborNode( ROUTE& node, std::vector< ROUTE >& vecOut )
{
	int iStartX = node.m_iX-m_iRange;
	int iStartY = node.m_iY-m_iRange;
	int iEndX = node.m_iX+m_iRange;
	int iEndY = node.m_iY+m_iRange;

	if( iStartX < 0 ) iStartX = 0;
	if( iStartY < 0 ) iStartY = 0;
	if( iEndX >= m_iWidth ) iEndX = m_iWidth-1;
	if( iEndY >= m_iHeight ) iEndY = m_iHeight-1;

	bool bFind = false;

	//node 주변의 이동가능한 노드를 검색합니다.
	for( int y=iStartY ; y<=iEndY ; ++y )
	{
		for( int x=iStartX ; x<=iEndX ; ++x )
		{
			// 자기자신은 포함하지 않는다.
			if( x == node.m_iX && y == node.m_iY )
				continue;

			// 이동가능한지 체크
			if( m_pMap[x+(y*m_iWidth)] == 1 )
			{
				// Close 목록에 포함되어 있는지 체크
				if( FindNode( m_vecCloseNode, x, y ) )
					continue;

				// 비용계산
				ROUTE last = m_vecOpenNode[m_vecOpenNode.size()-1];
				int iBeforeCost = last.m_iBeforeCost + abs(x-last.m_iX) + abs(y-last.m_iY);
				int iAfterCost = abs(x - m_iEndX) + abs(y - m_iEndY);

				ROUTE node(x, y, iBeforeCost, iAfterCost );
				vecOut.push_back(node);
				bFind = true;
			}
		}
	}

	return bFind;
}

bool KGCRoute::FindBestNode( std::vector< ROUTE >& vecTemp, ROUTE& out )
{
	out = vecTemp[0];

	// 가장 비용이 적은 노드를 검색한다
	for( int i=1 ; i<(int)vecTemp.size() ; ++i )
	{
		if( out.m_iCost == vecTemp[i].m_iCost )
		{
			//현재까지 진행해온 Cost가 작은넘을 우선으로 한다
			if( out.m_iBeforeCost > vecTemp[i].m_iBeforeCost )
				out = vecTemp[i];
		}
		else if( out.m_iCost > vecTemp[i].m_iCost )
		{
			out = vecTemp[i];
		}
	}

	// 목적위치를 찾았다.
	if( out.m_iX == m_iEndX && out.m_iY == m_iEndY )
		return true;

	return false;
}

bool KGCRoute::FindNode( std::vector<ROUTE>& vec, int x, int y )
{
	for( int i=0 ; i<(int)vec.size() ; ++i )
	{
		if( x == vec[i].m_iX && y == vec[i].m_iY )
			return true;
	}

	return false;
}