#pragma once
//#include "uifx/D3DStatic.h"

struct SStaticLinkPos
{
    D3DXVECTOR2 m_vLeftTop;
    D3DXVECTOR2 m_vRightButtom;
};

class KGCLinkableStatic : public KD3DStatic
{
public:
    DECLARE_CLASSNAME( KGCLinkableStatic );
    /// Default constructor
    KGCLinkableStatic( void );
    /// Default destructor
    virtual ~KGCLinkableStatic( void );
    /// Copy constructor
    KGCLinkableStatic( const KGCLinkableStatic& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLinkableStatic& operator=( const KGCLinkableStatic& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
    virtual void AddLinkText( std::wstring strText );
    virtual void CalcStringPosition();
protected:
    std::multimap< std::wstring, std::vector< SStaticLinkPos > > m_mapLinkText;
};

DEFINE_WND_FACTORY( KGCLinkableStatic );
DECLARE_WND_FACTORY( KGCLinkableStatic );
DECLARE_WND_FACTORY_NAME( KGCLinkableStatic );