#include "StdAfx.h"
#include "D3DWndClass.h"
#include "KD3DSizingBox.h"

void KD3DWndClass::SetSize( int iWidth, int iHeight )
{
    if( GetWndType().compare( "gc_sizingbox" ) == 0 )
    {
        KD3DSizingBox::SetSize( this, iWidth, iHeight );
        return;
    }

    dwWidth = iWidth;
    dwHeight = iHeight;
}

void KD3DWndClass::SetDefTextureIDx( int iIDx, bool bInit )
{
    if( GetWndType().compare( "gc_sizingbox" ) == 0 )
    {
        if( bInit == false )
            KD3DSizingBox::SetDefTextureIDx( this, iIDx );
        return;
    }
    iDefTextureIDx = iIDx;
}

void KD3DWndClass::SetDefTexCoords( D3DXVECTOR4& coords, bool bInit )
{
    if( GetWndType().compare( "gc_sizingbox" ) == 0 )
    {
        if( bInit == false )
            KD3DSizingBox::SetDefTexCoords( this, coords );
        return;
    }
    vDefTexCoords = coords;
}

// 추가 2011. 08. 04
void KD3DWndClass::AddChildWnd( KD3DWndClass* pWnd, UINT nIndex )
{
//	std::vector< KD3DWndClass* >::iterator vit;
	
	vecChild.insert( vecChild.begin() + nIndex, pWnd);
}

void KD3DWndClass::CalcSortIndex()
{
    std::vector<KD3DWndClass*>::iterator pos = vecChild.begin();
    std::vector<KD3DWndClass*>::iterator pos_end = vecChild.end();

    DWORD dwIndex = 0;
    
    for ( ; pos != pos_end; ++pos )
    {
        (*pos)->SetSortIndex( dwIndex++ );
        (*pos)->CalcSortIndex();
    }
}

void KD3DWndClass::SortByName()
{
    std::sort( vecChild.begin(), vecChild.end(), SortByNameComp() ); 

    std::vector<KD3DWndClass*>::iterator pos = vecChild.begin();
    std::vector<KD3DWndClass*>::iterator pos_end = vecChild.end();

    for ( ; pos != pos_end; ++pos )
    {
        (*pos)->SortByName();
    }
}

void KD3DWndClass::SortByIndex()
{
    std::sort( vecChild.begin(), vecChild.end(), SortByIndexComp() ); 

    std::vector<KD3DWndClass*>::iterator pos = vecChild.begin();
    std::vector<KD3DWndClass*>::iterator pos_end = vecChild.end();

    for ( ; pos != pos_end; ++pos )
    {
        (*pos)->SortByIndex();
    }
}