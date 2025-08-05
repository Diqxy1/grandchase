#ifndef _KGCJINEVENTMSG_H_
#define _KGCJINEVENTMSG_H_

class KGCJinEventMsg : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCJinEventMsg );
    /// Default constructor
    KGCJinEventMsg( void );
    /// Default destructor
    virtual ~KGCJinEventMsg( void );
    /// Copy constructor
    KGCJinEventMsg( const KGCJinEventMsg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCJinEventMsg& operator=( const KGCJinEventMsg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState();

	void OnExit();

protected:
    virtual void OnCreate();
	virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void Initialize();

private:
	KD3DWnd*			m_pkOK;
	KD3DStatic*			m_pkStatic;
};

DEFINE_WND_FACTORY( KGCJinEventMsg );
DECLARE_WND_FACTORY( KGCJinEventMsg );
DECLARE_WND_FACTORY_NAME( KGCJinEventMsg );

#endif //_KGCJINEVENTMSG_H_