#ifndef _VERTEX_H_
#define _VERTEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define FVF_VERTEX          (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define FVF_LVERTEX         (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_UVLVERTEX       (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX2|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1))
#define FVF_TRIANGLEVERTEX  (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define FVF_TLVERTEX        (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct SLVERTEX
{
    D3DXVECTOR3 Pos;      // Vertex position
    DWORD   	Diff;
	FLOAT       tu, tv; // Vertex texture coordinates

    SLVERTEX()
    {
        Diff = 0xffffffff;
    }
	void SetSLVER( float x, float y, float z, float fTu, float fTv )
	{
        SetSLVER( x, y, z, fTu, fTv, 0xffffffff );
	}
    void SetSLVER( float x, float y, float z, float fTu, float fTv, DWORD diff )
    {
        SETVECTOR3( Pos, x, y, z );
        Diff = diff;
        tu = fTu;
        tv = fTv;
    }
	void SetSLVERDiff( float x, float y, D3DCOLOR diff, float fTu, float fTv )
    {
		SETVECTOR3( Pos, x, y, 0.5f );
        Diff = diff;
        tu = fTu;
        tv = fTv;
    }
    enum _FVF
    {
        FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1,
    };
};

struct TLVERTEX
{
    D3DXVECTOR4 Pos;
    DWORD       Diff;
    FLOAT       tu, tv;
};

struct SUVLVERTEX
{
    D3DXVECTOR3 Pos;      // Vertex position
    unsigned long	Diff, Spec;
	FLOAT       tu1, tv1, tu2, tv2; // Vertex texture coordinates

	void SetSUVLVER( float x, float y, float z, FLOAT D, FLOAT E, FLOAT F, FLOAT G )
	{
		SETVECTOR3( Pos, x, y, z );
		Diff  = Spec = D3DCOLOR_ARGB( 255, 255, 255, 255 );
		tu1   = D;
		tv1   = E;
		tu2   = F;
		tv2   = G;
	}
};

struct TRIANGLEVERTEX
{
    float x, y, z;
    DWORD color;
};

struct BLURVERTEX
{
public:
    D3DXVECTOR3 pos;
    FLOAT       rhw;
    DWORD       color;
    FLOAT		tu, tv;

    BLURVERTEX(){}
    BLURVERTEX(D3DXVECTOR3 p, float w,DWORD c,float u,float v):pos(p),rhw(w),color(c),tu(u),tv(v) {}
    BLURVERTEX( BLURVERTEX& v )
    {
        pos = v.pos;
        rhw = v.rhw;
        color = v.color;
        tu = v.tu;
        tv = v.tv;
    }
    ~BLURVERTEX(){}
    enum _FVF
    {
        FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1,
    };
};

struct KD3DUIVertex
{
    D3DXVECTOR4 m_vPos; // x, y, z, 1/w
    D3DCOLOR    m_diff; // diffuse color
    D3DXVECTOR2 m_vTex; // tu, tv

    KD3DUIVertex( void )
    {
        SETVECTOR4( m_vPos, 0.0f, 0.0f, 0.0f, 1.0f );
        m_diff = D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff );
        SETVECTOR2( m_vTex, 0.0f, 0.0f );
    }
    KD3DUIVertex( D3DXVECTOR4& vPos, D3DCOLOR diff, D3DXVECTOR2& vTex )
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

    enum _FVF
    {
        FVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1,
    };

    void SetVertex( float x, float y, float z, float fTu, float fTv )
    {
        SETVECTOR4( m_vPos, x, y, z, 1.0f );
        m_diff = 0xffffffff;
        m_vTex.x = fTu;
        m_vTex.y = fTv;
    }

	void SetVertexDiff(float x, float y, D3DCOLOR diff, float fTu, float fTv )
	{
		SETVECTOR4( m_vPos, x, y, 0.0f, 1.0f );
        m_diff = diff;
        m_vTex.x = fTu;
        m_vTex.y = fTv;
    }
};

#endif // _VERTEX_H_
