#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class KGCSocketIcon : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSocketIcon );
    /// Default constructor
    KGCSocketIcon( void );
    /// Default destructor
    virtual ~KGCSocketIcon( void );
    /// Copy constructor
    KGCSocketIcon( const KGCSocketIcon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSocketIcon& operator=( const KGCSocketIcon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event_ );

private:
    KD3DWnd*        m_pkSocketIcon[ KSocketInfo::STS_NUM ];
    KSocketInfo::SOCKET_STATE        m_eItemSocketState;
public:
    void SetSocketState( KSocketInfo::SOCKET_STATE  eItemSocketState_ );
    KSocketInfo::SOCKET_STATE GetSocketState() { return m_eItemSocketState; }
    void AllSelfInputCheck( bool bUpdate_);
};

DEFINE_WND_FACTORY( KGCSocketIcon );
DECLARE_WND_FACTORY( KGCSocketIcon );
DECLARE_WND_FACTORY_NAME( KGCSocketIcon );


