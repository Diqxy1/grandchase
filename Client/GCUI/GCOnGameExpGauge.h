#ifndef _GCONGAMEEXPGAUGE_H_
#define _GCONGAMEEXPGAUGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../define.h"
//#include "../uifx/D3DWnd.h"

class GCOnGameExpGauge : public KD3DWnd 
{

    enum EGauge
    {
        E_LEFT = 0,
        E_BODY,
        E_RIGHT,
        E_NUM,
    };

public:
    DECLARE_CLASSNAME( GCOnGameExpGauge );
    /// Default constructor
    GCOnGameExpGauge( void );
    /// Default destructor
    virtual ~GCOnGameExpGauge( void );
    /// Copy constructor
    GCOnGameExpGauge( const GCOnGameExpGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const GCOnGameExpGauge& operator=( const GCOnGameExpGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
public:
    //함수
    void SetExp( float fExp_ );
    void AddExp( float fExp_ );
    void InitGauge();
    DWORD GetBodyWidth( float fGauge_ );
public:
    KSafeArray<KD3DWnd*,E_NUM> m_pkGauge;
    KD3DWnd*    m_pkLevelUp;
    // Gauge
    float       m_fGauge;
    DWORD       m_dwMaxGaugePixel;
    // Exp
    float       m_fCurExp;
    bool        m_bLevelUp;

    void SetVirtualExp( float fExp_ );
    void AddVirtualExp( float fExp_ );
    KSafeArray<KD3DWnd*,E_NUM> m_pkVirtualGauge;
    float       m_fVirtualGauge;
    float       m_fVirtualExp;
};

DEFINE_WND_FACTORY( GCOnGameExpGauge );
DECLARE_WND_FACTORY( GCOnGameExpGauge );
DECLARE_WND_FACTORY_NAME( GCOnGameExpGauge );
#endif//_GCONGAMEEXPGAUGE_H_
