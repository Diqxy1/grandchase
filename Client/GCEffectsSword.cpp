#include "StdAfx.h"
#include ".\gceffectssword.h"
//
//
//

//
//
#define SPLINE_GAP_PER_VERTEX	(3)		//이게 커지면 더 부드럽게 될지도 모른다
#define LINE_VERTEX             (2)

GCEffectsSword::GCEffectsSword(void)
{
	m_pVertex = NULL;
//    m_pVertexEdge = NULL;	
	m_Color = 0xffffffff;
	m_dwListLength = 7;
	//m_dwListLength = 40;
	m_dwListCount = 0;
	//m_iLayer = 0;
    m_fLineWidth = 1.00f;
    m_fHilightWidth = 0.00f;

	
    m_iTextureIndex = 0;

    m_bRender = true;
    m_bFullTexture = false;
}

GCEffectsSword::~GCEffectsSword(void)
{
	SAFE_DELETE_ARRAY(m_pVertex);

} 

//void GCEffectsSword::RegisterLuabind()
//{
//	lua_tinker::class_add<GCEffectsSword>( KGCLuabinder::getInstance().GetLuaState(), "GCEffectsSword" );		
//		lua_tinker::class_def<GCEffectsSword>( KGCLuabinder::getInstance().GetLuaState(), "Init",  GCEffectsSword::Init );
//		lua_tinker::class_def<GCEffectsSword>( KGCLuabinder::getInstance().GetLuaState(), "SetColor",  GCEffectsSword::SetColor_ );
//        lua_tinker::class_def<GCEffectsSword>( KGCLuabinder::getInstance().GetLuaState(), "SetLineWidth",  GCEffectsSword::SetLineWidth  );
//		;
//}
 
void GCEffectsSword::Init()
{		

	// Create Vertex
	SAFE_DELETE_ARRAY(m_pVertex);	
	m_pVertex = new SwordVertex[ m_dwListLength * SPLINE_GAP_PER_VERTEX * LINE_VERTEX ];

    
}

void GCEffectsSword::SetColor_(int a, int r, int g, int b)
{	
	SetColor(D3DCOLOR_ARGB(a,r,g,b));
}

void GCEffectsSword::Add(D3DXVECTOR3& pt1, D3DXVECTOR3& pt2, bool color)
{	
	static int iCount = 0; 
	iCount ++;

	int iAddIndex = (int)m_dwListCount;

	if( m_dwListCount == m_dwListLength )
	{
		// 당겨주자		
		memmove(m_pVertex, m_pVertex + SPLINE_GAP_PER_VERTEX*LINE_VERTEX,sizeof(SwordVertex) * ((m_dwListLength-1) * SPLINE_GAP_PER_VERTEX*LINE_VERTEX) );
		iAddIndex--;         
	}

	D3DXVECTOR3 v[3];

	v[0] = pt2;
	v[1] = pt1 + (pt2 - pt1) * ( 1.0f - m_fHilightWidth );
	v[2] = pt1 + (v[1] - pt1) * ( 1.0f - m_fLineWidth );
	SETVECTOR3( m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX].Pos, v[1].x, v[1].y, v[1].z );    
	SETVECTOR3( m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX + 1].Pos, v[2].x, v[2].y, v[2].z );

	if ( color )
	{
		m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX].Diff = m_Color;
		m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX + 1].Diff = m_Color;// & 0x00ffffff;
	}
	else
	{
		m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX].Diff = 0;
		m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX + 1].Diff = 0;// & 0x00ffffff;
	}
	
	
    if( m_bFullTexture )
    {
	    m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX].tv = 0;
	    m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX+1].tv = 1;
	    m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX].tu = m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX+1].tu = (float)m_iTextureIndex / ( m_dwListLength - 1);
    }
    else
    {
        m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX].tu = m_aiTextureUV[TEX_TU_1];
        m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX+1].tu = m_aiTextureUV[TEX_TU_2];
        m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX].tv = m_aiTextureUV[TEX_TV_1];
        m_pVertex[iAddIndex * SPLINE_GAP_PER_VERTEX * LINE_VERTEX+1].tv = m_aiTextureUV[TEX_TV_2];
    }

	m_iTextureIndex++;
	m_iTextureIndex %= m_dwListLength;

	m_dwListCount = iAddIndex +1;

	if( m_dwListCount < 4 )		// 보간할꺼 없삼
		return;

	// 보간하자    
	for( int iAdd = 0; iAdd < LINE_VERTEX; iAdd++ )
	{
        {
		    D3DXVECTOR3 vec[4];
		    vec[0] = m_pVertex[(iAddIndex-3)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].Pos;
		    vec[1] = m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].Pos;
		    vec[2] = m_pVertex[(iAddIndex-1)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].Pos;
		    vec[3] = m_pVertex[(iAddIndex-0)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].Pos;

		    float fS = 0.0f;
		    for( int i = 0; i < SPLINE_GAP_PER_VERTEX; i++ )
		    {   
			    m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX + i*LINE_VERTEX + iAdd ].Diff = 
				    m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].Diff;

			    D3DXVec3CatmullRom( &m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX + i*LINE_VERTEX + iAdd ].Pos, 
				    &vec[0], &vec[1], &vec[2], &vec[3],
				    fS );            
			    fS += (1.0f) / (SPLINE_GAP_PER_VERTEX);
		    }
        }
        {
            D3DXVECTOR2 vec[4];
            vec[0].x = m_pVertex[(iAddIndex-3)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tu;
            vec[1].x = m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tu;
            vec[2].x = m_pVertex[(iAddIndex-1)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tu;
            vec[3].x = m_pVertex[(iAddIndex-0)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tu;

            vec[0].y = m_pVertex[(iAddIndex-3)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tv;
            vec[1].y = m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tv;
            vec[2].y = m_pVertex[(iAddIndex-1)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tv;
            vec[3].y = m_pVertex[(iAddIndex-0)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].tv;

            float fS = 0.0f;
            for( int i = 0; i < SPLINE_GAP_PER_VERTEX; i++ )
            {   
                m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX + i*LINE_VERTEX + iAdd ].Diff = 
                    m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX+iAdd].Diff;

                D3DXVECTOR2 vTextureCoord;
                D3DXVec2CatmullRom( &vTextureCoord, 
                    &vec[0], &vec[1], &vec[2], &vec[3],
                    fS );            
                fS += (1.0f) / (SPLINE_GAP_PER_VERTEX);

                m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX + i*LINE_VERTEX + iAdd ].tu = vTextureCoord.x;
                m_pVertex[(iAddIndex-2)*SPLINE_GAP_PER_VERTEX*LINE_VERTEX + i*LINE_VERTEX + iAdd ].tv = vTextureCoord.y;
            }
        }

	} 

	// Alpha
}

void GCEffectsSword::AddWithLatency(D3DXVECTOR3& pt1, D3DXVECTOR3& pt2, bool color )
{

	SwordEffectElement eff;
	eff.vec1 = pt1;
	eff.vec2 = pt2;
	eff.color = color;
	m_LatencyBuffer.push(eff);	

	if ( m_LatencyBuffer.size() > LATENCY + SHOWLATENCY )
	{
		Add( m_LatencyBuffer.front().vec1, m_LatencyBuffer.front().vec2, m_LatencyBuffer.front().color );
		m_LatencyBuffer.pop();
		
	}
}

void GCEffectsSword::AlphaSmooth()
{
	float fAlphaSmoothGap = 3.0f;
	//int iAlpha;

//	fAlphaSmoothGap *= ((float) ((m_Color & 0xff000000) >> 24 )) / 255.0f;

	for( int i = 0; i < (int)((m_dwListLength-1) * SPLINE_GAP_PER_VERTEX + 1); i++ )
	{
		//iAlpha = ((int)(i*fAlphaSmoothGap)) << 24;

		//m_pVertex[i*2].Diff = (m_Color & 0x00ffffff) | iAlpha;
		//m_pVertex[i*2+1].Diff = (m_Color & 0x00ffffff)	| (255 << 24);

		//if( (i*fAlphaSmoothGap) >= 255 )
		//	return;
		//m_pVertex[i*2].tv = 0;
		//m_pVertex[i*2+1].tv = 1;

		//m_pVertex[i*2].tu = m_pVertex[i*2+1].tu = (float)i / ((m_dwListLength-1) * SPLINE_GAP_PER_VERTEX + 1);

		//m_pVertex[i*2].tu=0;
		//m_pVertex[i*2+1].tu=1.0f;

		////m_pVertex[i*2].tv=m_pVertex[i*2+1].tv=(float)4.0f*1.0f/64.0f+.02f;

		//m_pVertex[i*2].tv=m_pVertex[i*2+1].tv=(float)4.0f*0/64.0f+.02f;

		if( i < 6 * SPLINE_GAP_PER_VERTEX )
		{
			if ( m_pVertex[i*2].Diff )
			{
				m_pVertex[i*2].Diff = m_pVertex[i*2+1].Diff = D3DCOLOR_ARGB( i * 17, i * 17,i * 17,i * 17 );
			}						
		}

		//iAlpha = (float)( i / ((m_dwListLength-1) * SPLINE_GAP_PER_VERTEX + 1) )   * 255.0f;

		//m_pVertex[i*2].Diff = m_pVertex[i*2+1].Diff = D3DCOLOR_ARGB(  255, iAlpha,iAlpha,iAlpha);
	}
}

HRESULT GCEffectsSword::Render(GCDeviceTexture* texture,D3DXMATRIX * matrix)
{
    if( !m_bRender ) 
        return E_FAIL;
	if( m_dwListCount < 4 )
		return E_FAIL;	
	if( m_dwListCount-1 <= 0 )
		return E_FAIL;

	AlphaSmooth();	
	g_pGCDeviceManager2->DrawInWorldWithStrip(texture,matrix,m_pVertex + 2*LINE_VERTEX*SPLINE_GAP_PER_VERTEX,(m_dwListCount-4)*SPLINE_GAP_PER_VERTEX);    
	return S_OK;
}

void GCEffectsSword::Remove()
{
    if( !m_LatencyBuffer.empty() )
    {
        Add( m_LatencyBuffer.front().vec1, m_LatencyBuffer.front().vec2, m_LatencyBuffer.front().color );
        m_LatencyBuffer.pop();

        return;
    }
    if( m_dwListCount > 0 )
    {
        // 당겨주자		
        memmove(m_pVertex, m_pVertex + SPLINE_GAP_PER_VERTEX*LINE_VERTEX,sizeof(SwordVertex) * ((m_dwListLength-1) * SPLINE_GAP_PER_VERTEX*LINE_VERTEX) );
        m_dwListCount--;         
    }


}