#pragma once

class KGCRitasObj : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasObj );
    /// Default constructor
    KGCRitasObj( void );
    /// Default destructor
    virtual ~KGCRitasObj( void );
    /// Copy constructor
    KGCRitasObj( const KGCRitasObj& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasObj& operator=( const KGCRitasObj& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();

	void SetCanCatch( bool bCanCatch_ ) { m_bCanCatch = bCanCatch_; }
	void SetLive( bool bLive_ );			
	void SetOnBottom( bool bOnBottom_ )	{ m_bOnBottom = bOnBottom_; }
	void SetObjType( int iType_ )		{ m_iObjType = iType_; }

	bool GetCanCatch()		{ return m_bCanCatch; }
	bool GetLive()			{ return m_bLive; }
	bool GetOnBottom()		{ return m_bOnBottom; }
	int  GetObjType()		{ return m_iObjType;}

	void InitInfo(D3DXVECTOR2 vPos_ ,int iType = 0 , bool bLive_ = true );

private:

	int	m_iObjType;

	bool m_bLive;

	bool m_bCanCatch;

	bool m_bOnBottom;

	int m_iFrameCnt;

};

DEFINE_WND_FACTORY( KGCRitasObj );
DECLARE_WND_FACTORY( KGCRitasObj );
DECLARE_WND_FACTORY_NAME( KGCRitasObj );
