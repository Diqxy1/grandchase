#pragma once

class KGCFileFindDialog;

class KGCReplayExplain : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCReplayExplain );
    /// Default constructor
    KGCReplayExplain( void );
    /// Default destructor
    virtual ~KGCReplayExplain( void );
    /// Copy constructor
    KGCReplayExplain( const KGCReplayExplain& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReplayExplain& operator=( const KGCReplayExplain& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    enum { MAX_PAGE = 1, MAX_EXPLAIN = 6 };

    KD3DWnd*    m_pkPrevBtn;
    KD3DWnd*    m_pkNextBtn;

    KSafeArray<KD3DStatic*,  MAX_EXPLAIN>   m_apkExplain;

    int m_iPage;

    void SetExplain( void );
    void OnPrevPage( void );
    void OnNextPage( void );
    void SetPage( void );
};

DEFINE_WND_FACTORY( KGCReplayExplain );
DECLARE_WND_FACTORY( KGCReplayExplain );
DECLARE_WND_FACTORY_NAME( KGCReplayExplain );
