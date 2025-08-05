#pragma once
//#include "..\uifx\d3dwnd.h"

class KGCSizingThreeBox : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCSizingThreeBox );
    /// Default constructor
    KGCSizingThreeBox( void );
    /// Default destructor
    virtual ~KGCSizingThreeBox( void );
    /// Copy constructor
    KGCSizingThreeBox( const KGCSizingThreeBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSizingThreeBox& operator=( const KGCSizingThreeBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    KD3DWnd*    m_pkLeft;
    KD3DWnd*    m_pkMiddle;
    KD3DWnd*    m_pkRight;

public:
    void SetSize( DWORD dwWidth, DWORD dwHeight );
    void OnSetHeight( DWORD dwHeight );
	void OnSetWidth( DWORD dwWidth );

    DWORD   GetHeight() { return m_pkMiddle->GetHeight(); }
    DWORD   GetWidth()  { return (m_pkLeft->GetWidth() + m_pkMiddle->GetWidth() + m_pkRight->GetWidth()); }
};

DEFINE_WND_FACTORY( KGCSizingThreeBox );
DECLARE_WND_FACTORY( KGCSizingThreeBox );
DECLARE_WND_FACTORY_NAME( KGCSizingThreeBox );
