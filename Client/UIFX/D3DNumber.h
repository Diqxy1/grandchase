#pragma once

//#include "../uifx/D3DWnd.h"
//#include <map>

class KD3DNumber : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KD3DNumber );
	/// Default constructor
	KD3DNumber( void );
	/// Default destructor
	virtual ~KD3DNumber( void );
	/// Copy constructor
	KD3DNumber( const KD3DNumber& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DNumber& operator=( const KD3DNumber& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void SetNumber( UINT nNumber, ED3DWndMode eMode = D3DWM_DEFAULT );
    void SetNumberCoordi( UINT nNumber, const D3DXVECTOR4& vCoordi_ );

    std::map< UINT, D3DXVECTOR4 > GetTexCoordi() const { return m_mapTexCoordi; }
    void SetTexCoordi( const std::map< UINT, D3DXVECTOR4 >& mapCoordi ) { m_mapTexCoordi = mapCoordi; }

    // [2/16/2008] breadceo. 같은 텍스쳐에 있다는 가정 + 사이즈도 똑같다는 가정!
private:
    std::map< UINT, D3DXVECTOR4 > m_mapTexCoordi;
};

DEFINE_WND_FACTORY( KD3DNumber );
DECLARE_WND_FACTORY( KD3DNumber );
DECLARE_WND_FACTORY_NAME( KD3DNumber );
