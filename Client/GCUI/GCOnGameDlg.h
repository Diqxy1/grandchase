#ifndef _GCONGAMEDLG_H_
#define _GCONGAMEDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../UIFX/D3DListBox.h"
//#include "GCTutorialDlg.h"
//#include "GCArenaBattleDlg.h"

class KD3DStatic;
class KGCTutorialDlg;
class KGCArenaBattleDlg;

class KGCOnGameDlg : public KD3DWnd 
{
public:
    DECLARE_CLASSNAME( KGCOnGameDlg );
    /// Default constructor
    KGCOnGameDlg( void );
    /// Default destructor
    virtual ~KGCOnGameDlg( void );
    /// Copy constructor
    KGCOnGameDlg( const KGCOnGameDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOnGameDlg& operator=( const KGCOnGameDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
// 변수
    KGCTutorialDlg*     m_pkTutorialDlg;
    KGCArenaBattleDlg*  m_pkArenaBattleDlg;
// 함수
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );    
    void OnInitialize();
};

DEFINE_WND_FACTORY( KGCOnGameDlg );
DECLARE_WND_FACTORY( KGCOnGameDlg );
DECLARE_WND_FACTORY_NAME( KGCOnGameDlg );

extern KGCOnGameDlg* g_pkOnGameDlg;
#endif // _GCONGAMEDLG_H_