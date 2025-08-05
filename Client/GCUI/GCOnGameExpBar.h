#ifndef _GCONGAMEEXPBAR_H_
#define _GCONGAMEEXPBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../define.h"
//#include "../uifx/D3DWnd.h"

class GCOnGameExpGauge;
class KD3DStatic;

class GCOnGameExpBar : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( GCOnGameExpBar );
    /// Default constructor
    GCOnGameExpBar( void );
    /// Default destructor
    virtual ~GCOnGameExpBar( void );
    /// Copy constructor
    GCOnGameExpBar( const GCOnGameExpBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const GCOnGameExpBar& operator=( const GCOnGameExpBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );

public:
    // 함수
    void SetExp( float fExp_ );
    void AddExp( float fExp_ );
    void InitGauge();
    void SetCurrentExpPercent();

    void SetVirtualExp( float fExp_ );
    void AddVirtualExp( float fExp_ );
    void SetVirtualExpPercent();

public:
    GCOnGameExpGauge*   m_pExpGauge;
    float               m_fCurExp;
    float               m_fVirtualExp;
    KD3DStatic*         m_pkExpPercent;
};


DEFINE_WND_FACTORY( GCOnGameExpBar );
DECLARE_WND_FACTORY( GCOnGameExpBar );
DECLARE_WND_FACTORY_NAME( GCOnGameExpBar );
#endif//_GCONGAMEEXPBAR_H_
