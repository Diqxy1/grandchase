#ifndef _GCTUTORIALJOB_H_
#define _GCTUTORIALJOB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCTutorialJob : public KD3DWnd, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCTutorialJob );
    /// Default constructor
    KGCTutorialJob( void );
    /// Default destructor
    virtual ~KGCTutorialJob( void );
    /// Copy constructor
    KGCTutorialJob( const KGCTutorialJob& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTutorialJob& operator=( const KGCTutorialJob& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreateComplete( void );

public:
    void SelectJop(int iMaxLevel, int iJobLevel, bool bPlayerTap, bool bUseSpecialClass = false );
    void SetEnableButton(bool bSwitch, bool bUseSpecialClass = false );
    void SetEnableComButton(bool bSwitch, bool bUseSpecialClass = false );
    void SetAllEnable(bool bSwitch, bool bPlayerTab, bool bUseSpecialClass = false );
    void SetJobBtnLock( bool bLock, int iOpendPromotion );
    void SetComJobBtnLock( bool bLock, int iOpendPromotion );

protected:
    KSafeArray<KD3DWnd*, NUM_JOB_LEVEL+1> m_pkWndJobButton;
    KSafeArray<KD3DWnd*, NUM_JOB_LEVEL+1> m_pkWndComJobButton;
    bool m_bFirst;
};

DEFINE_WND_FACTORY( KGCTutorialJob );
DECLARE_WND_FACTORY( KGCTutorialJob );
DECLARE_WND_FACTORY_NAME( KGCTutorialJob );

#endif  // _GCTUTORIALJOB_H_