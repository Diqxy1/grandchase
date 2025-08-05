#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCNewClearSealJobBtn : public KD3DWnd, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCNewClearSealJobBtn );
    /// Default constructor
    KGCNewClearSealJobBtn( void );
    /// Default destructor
    virtual ~KGCNewClearSealJobBtn( void );
    /// Copy constructor
    KGCNewClearSealJobBtn( const KGCNewClearSealJobBtn& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNewClearSealJobBtn& operator=( const KGCNewClearSealJobBtn& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreateComplete( void );

public:
    void SelectJop(int iMaxLevel, int iJobLevel, bool bPlayerTap);
    void SetJopBnt( int iHavePromotion_ );
    void SetEnableButton(bool bSwitch);

protected:
    KSafeArray<KD3DWnd*, NUM_JOB_LEVEL> m_pkWndJobButton;
};

DEFINE_WND_FACTORY( KGCNewClearSealJobBtn );
DECLARE_WND_FACTORY( KGCNewClearSealJobBtn );
DECLARE_WND_FACTORY_NAME( KGCNewClearSealJobBtn );