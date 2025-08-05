#pragma once

class KGCAgitUserListCombo;

class KGCAgitSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum ICON_TYPE { ICON_CLOSE = 0, ICON_OPEN = 1, ICON_FRIEND = 2, ICON_CURSEL = 3, NUM_ICON = 4 };

public:
    DECLARE_CLASSNAME( KGCAgitSlot );
    /// Default constructor
    KGCAgitSlot( void );
    /// Default destructor
    virtual ~KGCAgitSlot( void );
    /// Copy constructor
    KGCAgitSlot( const KGCAgitSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitSlot& operator=( const KGCAgitSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DSizingBox*  m_pkDefaultFrame;
    KD3DSizingBox*  m_pkActiveFrame;
    KD3DStatic*     m_pkStaticAgitOwner;
    KD3DStatic*     m_pkStaticAgitName;
    KGCAgitUserListCombo*                m_pkUserListCombo;
    KSafeArray< KD3DWnd*, NUM_ICON >     m_apkIcon;

	bool	m_bIsValid;

public:
	KAgitPageInfo   m_kAgitInfo;

public:
    void SetAgitSlotInfo( KAgitPageInfo& info_ );
    void SetCurSel( bool bSet_ );
    void Clear( void );

    void SetActive( bool bActive_ );

	bool IsValid() { return m_bIsValid; }
    void VisibleUserComboList( bool bIsVisible );

private:
    void SetOpenType( int iType_ );
    void OnClickCombo( void );    
};

DEFINE_WND_FACTORY( KGCAgitSlot );
DECLARE_WND_FACTORY( KGCAgitSlot );
DECLARE_WND_FACTORY_NAME( KGCAgitSlot );
