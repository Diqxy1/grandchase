#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"

struct SGCHighLightRegion
{
	D3DXVECTOR2	m_vPos;
	DWORD		m_dwWidth;
	DWORD		m_dwHeight;


	float					m_fDeltaForMovingHightLight;
	UINT					m_iCntFrame;

	SGCHighLightRegion()
	{
		m_vPos = D3DXVECTOR2( 0.0f, 0.0f );
		m_dwHeight = 0;
		m_dwWidth = 0;

		m_fDeltaForMovingHightLight = 100.0f;
		m_iCntFrame = 0;
	}

	bool operator==( SGCHighLightRegion& arg )
	{
		return ( (m_vPos==arg.m_vPos)&&
			(m_dwWidth==arg.m_dwWidth)&&
			(m_dwHeight==arg.m_dwHeight) );
	}

	bool operator<<( SGCHighLightRegion& arg );
	void SettingDelta();
};

class KGCHighLight:		public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCHighLight );
	/// Default constructor
	KGCHighLight( void );
	/// Default destructor
	virtual ~KGCHighLight( void );
	/// Copy constructor
	KGCHighLight( const KGCHighLight& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCHighLight& operator=( const KGCHighLight& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		HL_LEFT_TOP = 0,
		HL_TOP,
		HL_RIGHT_TOP,
		HL_LEFT,
		HL_CENTER,
		HL_RIGHT,
		HL_LEFT_BOTTOM,
		HL_BOTTOM,
		HL_RIGHT_BOTTOM,

		HL_NUM,
	};

public:
	void SetHighLightRegion( float fx, float fy, DWORD dwHeight, DWORD dwWidth );

	void SetDarkReginon(DWORD dwWidth_ , DWORD dwHeight_ );
	void SetDarkReginonStartPos(D3DXVECTOR2 vPos_)		{ m_vDarkRegionStartPos = vPos_; } 

protected:
	virtual void OnCreate();
	virtual void FrameMoveInEnabledState();
	void RefreshRegion( SGCHighLightRegion sRegion );

	

protected:
	KSafeArray<KD3DWnd*,HL_NUM> m_pkDarkRegion;
	KD3DSizingBox* m_pkCenterRegion;
	KD3DSizingBox* m_pkOutLine;

private:
	D3DCOLOR		m_dcARGB;

	SGCHighLightRegion		m_sTargetRegion;
	SGCHighLightRegion		m_sCurrentRegion;

	

	int	m_iAnimationFrame;
	int	m_iFrameCount;

	DWORD m_dwDarkRegionWidth;
	DWORD m_dwDarkRegionHeight;

	D3DXVECTOR2	m_vDarkRegionStartPos;

};

DEFINE_WND_FACTORY( KGCHighLight );
DECLARE_WND_FACTORY( KGCHighLight );
DECLARE_WND_FACTORY_NAME( KGCHighLight );
