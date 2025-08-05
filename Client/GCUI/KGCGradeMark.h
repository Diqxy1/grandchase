#ifndef _KGCGRADEMARK_H_
#define _KGCGRADEMARK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "uifx/D3DWnd.h"

class KGCGradeMark : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCGradeMark );
    /// Default constructor
    KGCGradeMark( void );
    /// Default destructor
    virtual ~KGCGradeMark( void );
    /// Copy constructor
    KGCGradeMark( const KGCGradeMark& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGradeMark& operator=( const KGCGradeMark& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate();
    virtual void FrameMoveInEnabledState( void );

private:
    KSafeArray<KD3DWnd*,GC_GRADE_NUM * 3> m_apkGradeMark;

public:
    void SetGrade( int iGrade );
    // 그래이드 초기화!
    void SetGrade( void );
};

DEFINE_WND_FACTORY( KGCGradeMark );
DECLARE_WND_FACTORY( KGCGradeMark );
DECLARE_WND_FACTORY_NAME( KGCGradeMark );

#endif // _KGCGRADEMARK_H_
