#ifndef _GCSHOPTAB_H_
#define _GCSHOPTAB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCShopTab : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCShopTab );
    /// Default constructor
    KGCShopTab( void );
    /// Default destructor
    virtual ~KGCShopTab( void );
    /// Copy constructor
    KGCShopTab( const KGCShopTab& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCShopTab& operator=( const KGCShopTab& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ChangeTextType(int iTextType);

protected:
    virtual void OnCreate( void );
    virtual void OnActivate( void );
    virtual void FrameMoveInEnabledState( void );

protected:
    bool        m_bToggleSize;
    KD3DWnd*    m_pkTitleWnd;
	KD3DWnd*    m_pkTitleWndToggle;
};

DEFINE_WND_FACTORY( KGCShopTab );
DECLARE_WND_FACTORY( KGCShopTab );
DECLARE_WND_FACTORY_NAME( KGCShopTab );

#endif // _GCSHOPTAB_H_