#include "stdafx.h"
#include "./KD3DSizingBox.h"
#include "UI_GlobalFunction.h"
#include "D3DWndClass.h"

KD3DSizingBox::KD3DSizingBox()
{

}

KD3DSizingBox::~KD3DSizingBox()
{

}

void KD3DSizingBox::SetSize( KD3DWndClass* pkWnd, int iWidth, int iHeight )
{
	pkWnd->SetWidth( iWidth );
	pkWnd->SetHeight( iHeight );

	KD3DWndClass* pkLeft_Top		= FindWndWithName( pkWnd->GetChildContainer(), "left_top" );
	KD3DWndClass* pkTop				= FindWndWithName( pkWnd->GetChildContainer(), "top" );
	KD3DWndClass* pkRight_Top		= FindWndWithName( pkWnd->GetChildContainer(), "right_top" );
	KD3DWndClass* pkLeft			= FindWndWithName( pkWnd->GetChildContainer(), "left" );
	KD3DWndClass* pkCenter			= FindWndWithName( pkWnd->GetChildContainer(), "center" );
	KD3DWndClass* pkRight			= FindWndWithName( pkWnd->GetChildContainer(), "right" );
	KD3DWndClass* pkLeft_Bottom		= FindWndWithName( pkWnd->GetChildContainer(), "left_bottom" );
	KD3DWndClass* pkBottom			= FindWndWithName( pkWnd->GetChildContainer(), "bottom" );
	KD3DWndClass* pkRight_Bottom	= FindWndWithName( pkWnd->GetChildContainer(), "right_bottom" );

	if( pkLeft_Top == NULL ||		pkTop	 == NULL ||		
		pkRight_Top	 == NULL ||		pkLeft		 == NULL ||
		pkCenter		 == NULL ||		pkRight		 == NULL ||
		pkLeft_Bottom	 == NULL ||		pkBottom		 == NULL ||
		pkRight_Bottom == NULL
		)
		return;

	int iCenterWidth = iWidth - ( pkLeft_Top->GetWidth() + pkRight_Bottom->GetWidth() );
	int iCenterHeight = iHeight - ( pkLeft_Top->GetHeight() + pkRight_Bottom->GetHeight() );

	pkCenter->SetWidth( iCenterWidth );
	pkCenter->SetHeight(iCenterHeight);
	pkLeft->SetHeight(iCenterHeight);
	pkRight->SetHeight(iCenterHeight);
	pkTop->SetWidth(iCenterWidth);
	pkBottom->SetWidth(iCenterWidth);

    pkTop->SetWindowPos( D3DXVECTOR2(pkLeft_Top->GetWindowPos().x + pkLeft_Top->GetWidth(), pkLeft_Top->GetWindowPos().y) );
	pkLeft->SetWindowPos( D3DXVECTOR2( pkLeft_Top->GetWindowPos().x, pkLeft_Top->GetWindowPos().y + pkLeft_Top->GetHeight() ) );

    pkCenter->SetWindowPos( D3DXVECTOR2( pkTop->GetWindowPos().x, pkLeft->GetWindowPos().y) );
    pkBottom->SetWindowPos( D3DXVECTOR2( pkTop->GetWindowPos().x, pkBottom->GetWindowPos().y ) );
    pkRight->SetWindowPos( D3DXVECTOR2( pkRight->GetWindowPos().x, pkLeft->GetWindowPos().y ) );

    pkRight_Top->SetWindowPos( D3DXVECTOR2(pkTop->GetWindowPos().x + pkTop->GetWidth(), pkRight_Top->GetWindowPos().y ) );
    pkRight->SetWindowPos( D3DXVECTOR2(pkCenter->GetWindowPos().x + pkCenter->GetWidth(), pkRight->GetWindowPos().y ) );
    pkRight_Bottom->SetWindowPos( D3DXVECTOR2(pkBottom->GetWindowPos().x + pkCenter->GetWidth(), pkRight_Bottom->GetWindowPos().y ) );

    pkLeft_Bottom->SetWindowPos( D3DXVECTOR2(pkLeft_Bottom->GetWindowPos().x, pkLeft->GetWindowPos().y + pkLeft->GetHeight() ) );
    pkBottom->SetWindowPos( D3DXVECTOR2(pkBottom->GetWindowPos().x, pkCenter->GetWindowPos().y + pkCenter->GetHeight() ) );
    pkRight_Bottom->SetWindowPos( D3DXVECTOR2(pkRight_Bottom->GetWindowPos().x, pkRight->GetWindowPos().y + pkRight->GetHeight() ) );
}

void KD3DSizingBox::SetDefTextureIDx( KD3DWndClass* pkWnd, int iIDx )
{
    KD3DWndClass* pkLeft_Top		= FindWndWithName( pkWnd->GetChildContainer(), "left_top" );
    KD3DWndClass* pkTop				= FindWndWithName( pkWnd->GetChildContainer(), "top" );
    KD3DWndClass* pkRight_Top		= FindWndWithName( pkWnd->GetChildContainer(), "right_top" );
    KD3DWndClass* pkLeft			= FindWndWithName( pkWnd->GetChildContainer(), "left" );
    KD3DWndClass* pkCenter			= FindWndWithName( pkWnd->GetChildContainer(), "center" );
    KD3DWndClass* pkRight			= FindWndWithName( pkWnd->GetChildContainer(), "right" );
    KD3DWndClass* pkLeft_Bottom		= FindWndWithName( pkWnd->GetChildContainer(), "left_bottom" );
    KD3DWndClass* pkBottom			= FindWndWithName( pkWnd->GetChildContainer(), "bottom" );
    KD3DWndClass* pkRight_Bottom	= FindWndWithName( pkWnd->GetChildContainer(), "right_bottom" );

    pkLeft_Top->SetDefTextureIDx( iIDx );
    pkTop->SetDefTextureIDx( iIDx );
    pkRight_Top->SetDefTextureIDx( iIDx );
    pkLeft->SetDefTextureIDx( iIDx );
    pkCenter->SetDefTextureIDx( iIDx );
    pkRight->SetDefTextureIDx( iIDx );
    pkLeft_Bottom->SetDefTextureIDx( iIDx );
    pkBottom->SetDefTextureIDx( iIDx );
    pkRight_Bottom->SetDefTextureIDx( iIDx );
}

void KD3DSizingBox::SetDefTexCoords( KD3DWndClass* pkWnd, D3DXVECTOR4& coords )
{
    KD3DWndClass* pkLeft_Top		= FindWndWithName( pkWnd->GetChildContainer(), "left_top" );
    KD3DWndClass* pkTop				= FindWndWithName( pkWnd->GetChildContainer(), "top" );
    KD3DWndClass* pkRight_Top		= FindWndWithName( pkWnd->GetChildContainer(), "right_top" );
    KD3DWndClass* pkLeft			= FindWndWithName( pkWnd->GetChildContainer(), "left" );
    KD3DWndClass* pkCenter			= FindWndWithName( pkWnd->GetChildContainer(), "center" );
    KD3DWndClass* pkRight			= FindWndWithName( pkWnd->GetChildContainer(), "right" );
    KD3DWndClass* pkLeft_Bottom		= FindWndWithName( pkWnd->GetChildContainer(), "left_bottom" );
    KD3DWndClass* pkBottom			= FindWndWithName( pkWnd->GetChildContainer(), "bottom" );
    KD3DWndClass* pkRight_Bottom	= FindWndWithName( pkWnd->GetChildContainer(), "right_bottom" );

    int iEdgeWidth = (coords.z - coords.x) / 2 - 1;
    int iEdgeHeight = (coords.w - coords.y) / 2 - 1;

    pkLeft_Top->SetDefTexCoords(    D3DXVECTOR4( coords.x,                  coords.y, coords.x + iEdgeWidth,        coords.y + iEdgeHeight ) );
    pkLeft_Top->SetSize( iEdgeWidth, iEdgeHeight );
    pkTop->SetDefTexCoords(         D3DXVECTOR4( coords.x + iEdgeWidth,     coords.y, coords.x + iEdgeWidth + 2,    coords.y + iEdgeHeight ) );
    pkTop->SetSize( 1, iEdgeHeight );
    pkRight_Top->SetDefTexCoords(   D3DXVECTOR4( coords.z - iEdgeWidth,     coords.y, coords.z,                     coords.y + iEdgeHeight ) );
    pkRight_Top->SetSize( iEdgeWidth, iEdgeHeight );

    pkLeft->SetDefTexCoords(     D3DXVECTOR4( coords.x,                 coords.y + iEdgeHeight, coords.x + iEdgeWidth,     coords.y + iEdgeHeight+2 ) );
    pkLeft->SetSize( iEdgeWidth, 1 );
    pkCenter->SetDefTexCoords(  D3DXVECTOR4( coords.x + iEdgeWidth,     coords.y + iEdgeHeight, coords.x + iEdgeWidth+2,   coords.y + iEdgeHeight+2 ) );
    pkCenter->SetSize( 1, 1 );
    pkRight->SetDefTexCoords(   D3DXVECTOR4( coords.z - iEdgeWidth,     coords.y + iEdgeHeight, coords.z,                  coords.y + iEdgeHeight+2 ) );
    pkRight->SetSize( iEdgeWidth, 1 );

    pkLeft_Bottom->SetDefTexCoords( D3DXVECTOR4( coords.x,                  coords.w - iEdgeHeight,     coords.x + iEdgeWidth,      coords.w ) );
    pkLeft_Bottom->SetSize( iEdgeWidth, iEdgeHeight );
    pkBottom->SetDefTexCoords(      D3DXVECTOR4( coords.x + iEdgeWidth,     coords.w - iEdgeHeight,     coords.x + iEdgeWidth + 2,  coords.w ) );
    pkBottom->SetSize( 1, iEdgeHeight );
    pkRight_Bottom->SetDefTexCoords(D3DXVECTOR4( coords.z - iEdgeWidth,     coords.w - iEdgeHeight,     coords.z,                   coords.w ) );
    pkRight_Bottom->SetSize( iEdgeWidth, iEdgeHeight );

    KD3DSizingBox::SetSize( pkWnd, pkWnd->GetWidth(), pkWnd->GetHeight() );
}