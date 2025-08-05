#pragma once

class KGCCharIcon : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharIcon );
    /// Default constructor
    KGCCharIcon( void );
    /// Default destructor
    virtual ~KGCCharIcon( void );
    /// Copy constructor
    KGCCharIcon( const KGCCharIcon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharIcon& operator=( const KGCCharIcon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DWnd*    m_pkLock;
    KD3DWnd*    m_pkOpen;

	KD3DWnd*    m_pkNewIcon;
	KD3DWnd*    m_pkLockIcon;
	

public:
	KD3DWnd*    m_pkCashIcon;
	KD3DWnd*    m_pkSaleIcon;

    KD3DWnd*    m_pkSelIcon;
	KD3DWnd*    m_pkDisableIcon;

    void SetShow( bool bLock );
	void InitMark();

	void SetNewIcon (bool bNewIcon_ )	{ m_pkNewIcon->ToggleRender(bNewIcon_); }
	void SetLockIcon(bool bLockIcon_ )	{ m_pkLockIcon->ToggleRender(bLockIcon_); }
	void SetCashIcon(bool bLockIcon_ );
	bool IsLockChar()					{ return m_pkLockIcon->IsRenderOn(); }

	void SetSaleIcon(bool bSaleIcon_ )	{ m_pkSaleIcon->ToggleRender(bSaleIcon_); }

    void SetSelIcon(bool bSelIcon_ )	{ m_pkSelIcon->ToggleRender(bSelIcon_); }
	bool IsSelected() { return m_pkSelIcon->IsRenderOn(); }
	void SetDisableIcon(bool bDisableIcon_ )	{ m_pkDisableIcon->ToggleRender(bDisableIcon_); }
	bool IsDisabled() { return m_pkDisableIcon->IsRenderOn(); }
};

DEFINE_WND_FACTORY( KGCCharIcon );
DECLARE_WND_FACTORY( KGCCharIcon );
DECLARE_WND_FACTORY_NAME( KGCCharIcon );
