// KTDGFrustum.h: interface for the KTDGFrustum class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KTDGFRUSTUM_H__A2BA8923_D271_4E11_887E_C4E8C2BB5D1D__INCLUDED_)
//#define AFX_KTDGFRUSTUM_H__A2BA8923_D271_4E11_887E_C4E8C2BB5D1D__INCLUDED_

#ifndef _KTDGFRUSTUM_H_
#define _KTDGFRUSTUM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKTDGFrustum  
{
	public:
		CKTDGFrustum( LPDIRECT3DDEVICE9 pd3dDevice );
		virtual ~CKTDGFrustum();
		
		bool Construct( float ZDistance = 0.0f );
		bool CheckPoint( float XPos, float YPos, float ZPos );
		bool CheckCube( float XCenter, float YCenter, float ZCenter, float Size );
		bool CheckRectangle( float XCenter, float YCenter, float ZCenter, float XSize, float YSize, float ZSize );
		bool CheckSphere( float XCenter, float YCenter, float ZCenter, float Radius );
        bool CheckSphere( const D3DXVECTOR3& center, float Radius ){ return CheckSphere( center.x, center.y, center.z, Radius ); }
		
	private:
		LPDIRECT3DDEVICE9	m_pd3dDevice;
	    D3DXPLANE			m_Planes[6];
};

#endif // _KTDGFRUSTUM_H_
