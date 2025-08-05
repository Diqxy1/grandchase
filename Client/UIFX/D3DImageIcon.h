#ifndef _D3DIMAGEICON_H_
#define _D3DIMAGEICON_H_
#pragma once

class KD3DImageIcon : public KD3DWnd
{
public:

	DECLARE_CLASSNAME( KD3DImageIcon );
	/// Default constructor
	KD3DImageIcon( void );
	/// Default destructor
	virtual ~KD3DImageIcon( void );
	/// Copy constructor
	KD3DImageIcon( const KD3DImageIcon& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DImageIcon& operator=( const KD3DImageIcon& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	virtual void PostDraw( void );
    virtual void Destroy( void );

	void Create( bool bAuto = false );
	//GCDeviceTexture*        m_pTexture;
	void SetTextureName(std::wstring strFileName);
    void SetTexNameUseMassFile( const std::wstring& strFileName_ );

    std::wstring m_strTextureName;

    GCDeviceTexture*    m_pTexture;
};

DEFINE_WND_FACTORY( KD3DImageIcon );

#endif // _D3DIMAGEICON_H_