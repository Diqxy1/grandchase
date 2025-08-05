#ifndef _GCQUESTGAUGE_H_
#define _GCQUESTGAUGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCQuestGauge : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCQuestGauge );
    /// Default constructor
    KGCQuestGauge( void );
    /// Default destructor
    virtual ~KGCQuestGauge( void );
    /// Copy constructor
    KGCQuestGauge( const KGCQuestGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQuestGauge& operator=( const KGCQuestGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetChar( int iChar );
    void SetCurrentVal( DWORD dwCur );
    void SetMaximumVal( DWORD dwMax );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );

protected:
    KD3DWnd*    m_pkGuageBar;
    KD3DWnd*    m_pkType;
    DWORD       m_dwCur;
    DWORD       m_dwMax;
};

DEFINE_WND_FACTORY( KGCQuestGauge );
DECLARE_WND_FACTORY( KGCQuestGauge );
DECLARE_WND_FACTORY_NAME( KGCQuestGauge );

#endif // _GCQUESTGAUGE_H_