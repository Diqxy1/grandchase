#ifndef _D3DUIVERTEX_H_
#define _D3DUIVERTEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
/*
struct KD3DUIVertex
{
    D3DXVECTOR4 m_vPos; // x, y, z, 1/w
    D3DCOLOR    m_diff; // diffuse color
    D3DXVECTOR2 m_vTex; // tu, tv

    KD3DUIVertex( void )
    {
        m_vPos = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
        m_diff = D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff );
        m_vTex = D3DXVECTOR2( 0.0f, 0.0f );
    }
    KD3DUIVertex( D3DXVECTOR4 vPos, D3DCOLOR diff, D3DXVECTOR2 vTex )
    {
        m_vPos = vPos;
        m_diff = diff;
        m_vTex = vTex;
    }
    KD3DUIVertex( const KD3DUIVertex& src )
    {
        m_vPos = src.m_vPos;
        m_diff = src.m_diff;
        m_vTex = src.m_vTex;
    }
    const KD3DUIVertex& operator=( const KD3DUIVertex& src )
    {
        m_vPos = src.m_vPos;
        m_diff = src.m_diff;
        m_vTex = src.m_vTex;
        return *this;
    }
};

#define D3DFVF_D3DUIVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
*/
#endif // _D3DUIVERTEX_H_