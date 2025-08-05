#pragma once

class KGCCharacterLockBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharacterLockBox );
    /// Default constructor
    KGCCharacterLockBox( void );
    /// Default destructor
    virtual ~KGCCharacterLockBox( void );
    /// Copy constructor
    KGCCharacterLockBox( const KGCCharacterLockBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharacterLockBox& operator=( const KGCCharacterLockBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

	D3DXVECTOR2 InitBox(int es = 0,DWORD dwParam1=0, DWORD dwParam2=0);

	std::map<int,KD3DWnd*> m_mapLockedChar;

	KD3DSizingBox*		   m_pkBack;
};

DEFINE_WND_FACTORY( KGCCharacterLockBox );
DECLARE_WND_FACTORY( KGCCharacterLockBox );
DECLARE_WND_FACTORY_NAME( KGCCharacterLockBox );
