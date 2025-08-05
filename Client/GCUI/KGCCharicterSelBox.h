#ifndef _KGCCHARICTERSELBOX_H_
#define _KGCCHARICTERSELBOX_H_

//#include "../uifx/D3DWnd.h"

class KGCCharicterSelBox : public KD3DWnd,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharicterSelBox );
    /// Default constructor
    KGCCharicterSelBox( void );
    /// Default destructor
    virtual ~KGCCharicterSelBox( void );
    /// Copy constructor
    KGCCharicterSelBox( const KGCCharicterSelBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharicterSelBox& operator=( const KGCCharicterSelBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetOnlySelectedDraw(bool bDraw);
    void SelectCharicter(int index);
    int GetCurCharIndex(void) { return m_iCharIndex; }

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState();

protected:  
    int                 m_iCharIndex;
    KSafeArray<KD3DWnd*,GC_CHAR_NUM * NUM_JOB_LEVEL> m_pkChar;  
};

DEFINE_WND_FACTORY( KGCCharicterSelBox );
DECLARE_WND_FACTORY( KGCCharicterSelBox );
DECLARE_WND_FACTORY_NAME( KGCCharicterSelBox );

#endif //_KGCCHARICTERSELBOX_H_