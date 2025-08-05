#pragma once

class KGCFontSkinDlg;
class KGCRoomAnimationDlg;
class KGCCutInSkinDlg;

class KGCCustomizationDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCustomizationDlg );
    /// Default constructor
    KGCCustomizationDlg( void );
    /// Default destructor
    virtual ~KGCCustomizationDlg( void );
    /// Copy constructor
    KGCCustomizationDlg( const KGCCustomizationDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCustomizationDlg& operator=( const KGCCustomizationDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    void OnClose();
    void UpdateSubWindow( int iOption );
    void UpdateAllSubWindows();
    bool IsSubWindowActive( int iOption );
private:
    KD3DWnd* m_pkClose;
	KSafeArray< KD3DWnd*, 3 > m_vecBtn;
    KSafeArray< KD3DWnd*, 3 > m_vecBtnOn;
    KGCFontSkinDlg* m_pkFontSkinDlg;
    KGCRoomAnimationDlg* m_pkAnimationDlg;
    KGCCutInSkinDlg* m_pkCutinSkinDlg;
};

DEFINE_WND_FACTORY( KGCCustomizationDlg );
DECLARE_WND_FACTORY( KGCCustomizationDlg );
DECLARE_WND_FACTORY_NAME( KGCCustomizationDlg );
