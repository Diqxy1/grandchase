#pragma once

class KGCAgitListComboMain;
class KGCAgitListComboPopup;

class KGCAgitUserListCombo : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitUserListCombo );
    /// Default constructor
    KGCAgitUserListCombo( void );
    /// Default destructor
    virtual ~KGCAgitUserListCombo( void );
    /// Copy constructor
    KGCAgitUserListCombo( const KGCAgitUserListCombo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitUserListCombo& operator=( const KGCAgitUserListCombo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );

private:
    KGCAgitListComboMain*   m_pkComboListMain;
    KGCAgitListComboPopup*  m_pkComboListPopup;

public:
    void TogglePopup( void );
    void OnClickMain( void );
    void SetUserList( UCHAR ucMaxUser_, std::map< DWORD, std::wstring >& mapInfo_ );
    void Clear( void );

	bool CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos ); 
	void SetPopupBoxRender( bool bSet );
    void SetActive( bool bActive_ );
};

DEFINE_WND_FACTORY( KGCAgitUserListCombo );
DECLARE_WND_FACTORY( KGCAgitUserListCombo );
DECLARE_WND_FACTORY_NAME( KGCAgitUserListCombo );
