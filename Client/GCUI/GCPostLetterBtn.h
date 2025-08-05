#pragma once

class KGCPostLetterBtn : public KD3DWnd    // extends
{
public:
    DECLARE_CLASSNAME( KGCPostLetterBtn );
    /// Default constructor
    KGCPostLetterBtn( void );
    /// Default destructor
    virtual ~KGCPostLetterBtn( void );
    /// Copy constructor
    KGCPostLetterBtn( const KGCPostLetterBtn& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPostLetterBtn& operator=( const KGCPostLetterBtn& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );

public:
    static void SetReceiveOn( void );
    static void SetReceiveOff( void );
    static bool IsNewLetterRecv( void ) { return m_bReceiveState; }

private:
    void OnClickBtn( void );
    void ShowNewLetterMsg( bool bShow_ );

private:
    KD3DWnd*    m_pkNewMark;
    KD3DWnd*    m_pkNewLetterBalloon;
    KD3DStatic* m_pkStaticNewLetter;

    static bool m_bReceiveState;
    static bool m_bRecvMsgState;
    static int  m_iMsgViewTime;
};

DEFINE_WND_FACTORY( KGCPostLetterBtn );
DECLARE_WND_FACTORY( KGCPostLetterBtn );
DECLARE_WND_FACTORY_NAME( KGCPostLetterBtn );
