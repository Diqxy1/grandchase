// 검궤적을 생성시키고 그려주는 함수 입니다.

#pragma once


#define SwordVertex     SLVERTEX


class GCDeviceTexture;

struct SwordEffectElement
{
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 vec2;
	bool color;
	SwordEffectElement()
        : color(true)
	{
	}

	SwordEffectElement& operator =( const SwordEffectElement& rval )
	{
		vec1 = rval.vec1;
		vec2 = rval.vec2;
		color = rval.color;
		return *this;
	}

	SwordEffectElement( const SwordEffectElement& rval )
	{
		vec1 = rval.vec1;
		vec2 = rval.vec2;
		color = rval.color;
	}
};

class GCEffectsSword
{
public:
    enum TextureUV
    {
        TEX_TU_1 = 0,
        TEX_TV_1,
        TEX_TU_2,
        TEX_TV_2,
    };
public:
	GCEffectsSword(void);
	~GCEffectsSword(void);

	virtual HRESULT Render(GCDeviceTexture* texture,D3DXMATRIX * matrix);

//	static void RegisterLuabind();

    void Clear() { m_dwListCount = 0; }
	void Init();
	void SetLength(DWORD dwListLength) { m_dwListLength = dwListLength; }
	void SetColor(D3DCOLOR color) { m_Color = color; }
	void SetColor_(int a, int r, int g, int b);
	void Add(D3DXVECTOR3& pt1, D3DXVECTOR3& pt2, bool color = true );	
	void AddWithLatency(D3DXVECTOR3& pt1, D3DXVECTOR3& pt2, bool color = true );	
    void Remove();
    
    void SetFullTexture( bool bFull_ ) { m_bFullTexture = bFull_; }

    void SetLineWidth( float fHilightWidth, float fWidth ) { m_fHilightWidth = fHilightWidth; m_fLineWidth = fWidth; }

	void Show(bool bShow) { m_bRender = bShow; }
	
	D3DXVECTOR3 GetVertexPosByIndex(int iindex_) { ASSERT( iindex_ >= 0 && iindex_ < (int)m_dwListLength);
                                                   return m_pVertex[iindex_].Pos; }
    void SetTextureUV( float fTu1_,float fTv1_, float fTu2_, float fTv2_ )
    { 
        m_aiTextureUV[TEX_TU_1] = fTu1_;
        m_aiTextureUV[TEX_TV_1] = fTv1_;
        m_aiTextureUV[TEX_TU_2] = fTu2_;
        m_aiTextureUV[TEX_TV_2] = fTv2_;
    }
private:
	void AlphaSmooth();

	DWORD				m_dwListLength;
	DWORD				m_dwListCount;

    float               m_fLineWidth;
    float               m_fHilightWidth;
	D3DCOLOR			m_Color;
	bool				m_bRender;	

	int					m_iLayer;
	int					m_iTextureIndex;
    KSafeArray<float,4> m_aiTextureUV;

    bool                m_bFullTexture;
	
    SwordVertex*    m_pVertex;    
	std::queue<SwordEffectElement> m_LatencyBuffer;
	//SLVERTEX*   m_pVertex;
};
