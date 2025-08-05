#ifndef _GCLOADINGBOX_H_
#define _GCLOADINGBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCLoadingBox : public KD3DDialog,           // extends
                      public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KGCLoadingBox );
    /// Default constructor
    KGCLoadingBox( void );
    /// Default destructor
    virtual ~KGCLoadingBox( void );
    /// Copy constructor
    KGCLoadingBox( const KGCLoadingBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLoadingBox& operator=( const KGCLoadingBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        MAX_LOADING_TIME = 15,
        MAX_GAUGE_LEN = 215,
    };

    enum ELoadingBoxResult
    {
		LBR_SUCCESS,
		LBR_FAILED,
        LBR_CREATED,
        LBR_DESTROYED,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

protected:
	DWORD       m_dwLoadingTime;
    DWORD       m_dwOrgLen;
    KD3DWnd*    m_pkGauge;   
};

DEFINE_WND_FACTORY( KGCLoadingBox );
DECLARE_WND_FACTORY( KGCLoadingBox );
DECLARE_WND_FACTORY_NAME( KGCLoadingBox );

#endif // _GCLOADINGBOX_H_