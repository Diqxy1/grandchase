#ifndef _GCPLAYERO2BAR_H_
#define _GCPLAYERO2BAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCPlayerO2Bar : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCPlayerO2Bar );
    /// Default constructor
    KGCPlayerO2Bar( void );
    /// Default destructor
    virtual ~KGCPlayerO2Bar( void );
    /// Copy constructor
    KGCPlayerO2Bar( const KGCPlayerO2Bar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPlayerO2Bar& operator=( const KGCPlayerO2Bar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void EnableO2Bar( bool bEnable );
    void SetNowO2( float fNowO2 );
    void SetMaxO2( float fMaxO2 );

    inline float GetNowO2( void ) const { return m_fNowO2; }
    inline float GetMaxO2( void ) const { return m_fMaxO2; }

protected:
    enum
    {
        MAX_GAUGE_LENGTH = 482,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void ModulateGauge( void );

protected:
    float       m_fNowO2;
    float       m_fMaxO2;
    bool        m_bEnable;
    float       m_fTimeParam;
    KSafeArray<KD3DWnd*,10> m_pkGaugeBar;

	int			m_iBlinkGauge;
	int			m_iBlinkGap;
};

DEFINE_WND_FACTORY( KGCPlayerO2Bar );
DECLARE_WND_FACTORY( KGCPlayerO2Bar );
DECLARE_WND_FACTORY_NAME( KGCPlayerO2Bar );

#endif // _GCPLAYERO2BAR_H_