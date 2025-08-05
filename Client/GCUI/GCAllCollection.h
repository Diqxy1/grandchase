#pragma once

class KGCCharCollectionDlg;
class KGCVipCollectionDlg;

class KGCAllCollection : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAllCollection );
    /// Default constructor
    KGCAllCollection( void );
    /// Default destructor
    virtual ~KGCAllCollection( void );
    /// Copy constructor
    KGCAllCollection( const KGCAllCollection& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAllCollection& operator=( const KGCAllCollection& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    void OnClose();

private:
    KD3DWnd* m_pkClose;
	KSafeArray< KD3DWnd*, 2 > m_vecBtn;
    KSafeArray< KD3DWnd*, 2 > m_vecBtnOn;
    KGCCharCollectionDlg* m_pkCharCollection;
    KGCVipCollectionDlg* m_pkVipCollection;

};

DEFINE_WND_FACTORY( KGCAllCollection );
DECLARE_WND_FACTORY( KGCAllCollection );
DECLARE_WND_FACTORY_NAME( KGCAllCollection );
