#ifndef _GCGRAPHICQUALITY_H_
#define _GCGRAPHICQUALITY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCGraphicQuality : public KD3DWnd, // extends
	public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCGraphicQuality );
    /// Default constructor
    KGCGraphicQuality( void );
    /// Default destructor
    virtual ~KGCGraphicQuality( void );
    /// Copy constructor
    KGCGraphicQuality( const KGCGraphicQuality& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGraphicQuality& operator=( const KGCGraphicQuality& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        MAX_QUALITY     = 7,
    };

public:
	virtual void ActionPerformed( const KActionEvent& event );
    void SetQuality( int iQuality );
	int GetQuality( void ){ return m_iGraphicQuality; }

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

protected:
	int m_iGraphicQuality;
	KSafeArray<KD3DWnd*,MAX_QUALITY> m_ppkGage;
	KD3DWnd* m_pkThumb;
	
	KD3DWnd* m_pkBtnMinus;
	KD3DWnd* m_pkBtnMajor;

	bool m_bDrag;
};

extern KGCGraphicQuality* g_pGraphicQuality;

DEFINE_WND_FACTORY( KGCGraphicQuality );
DECLARE_WND_FACTORY( KGCGraphicQuality );
DECLARE_WND_FACTORY_NAME( KGCGraphicQuality );

#endif // _GCGRAPHICQUALITY_H_