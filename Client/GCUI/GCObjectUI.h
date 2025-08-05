#pragma once
#include "KGC2DObject.h"

class KD3DWnd;
class KGCObjectUI : public KD3DWnd
                  //, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCObjectUI );
    /// Default constructor
    KGCObjectUI( void );
    /// Default destructor
    virtual ~KGCObjectUI( void );
    /// Copy constructor
    KGCObjectUI( const KGCObjectUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCObjectUI& operator=( const KGCObjectUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void BindToLua();
    virtual void PostDraw();

    KGC2DObject* Create2DObject();
    KGC2DObject* Get2DObject( int iIndex );
    bool Remove2DObject( int iIndex );
    void Clear2DObject();
private:
    std::vector<KGC2DObject*>   m_vecObjects;
};

DEFINE_WND_FACTORY( KGCObjectUI );
DECLARE_WND_FACTORY( KGCObjectUI );
DECLARE_WND_FACTORY_NAME( KGCObjectUI );
