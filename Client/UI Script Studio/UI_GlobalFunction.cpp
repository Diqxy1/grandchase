#include "stdafx.h"
#include ".\ui_globalfunction.h"

KD3DWndClass* FindWndWithName( const std::vector< KD3DWndClass* >& vecChild, std::string strWndName )
{
	std::vector< KD3DWndClass* >::const_iterator vit;

	for( vit = vecChild.begin() ; vit != vecChild.end() ; ++vit )
	{
		if( (*vit)->GetWndName().compare( strWndName ) == 0 )
			return *vit;
	}

	return NULL;
}

