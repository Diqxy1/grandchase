#pragma once

class KGCRitasPanel;
class KGCRitasRankingDlg;
class KGCRitasResultDlg;

class KGCRitasChristmas : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasChristmas );
    /// Default constructor
    KGCRitasChristmas( void );
    /// Default destructor
    virtual ~KGCRitasChristmas( void );
    /// Copy constructor
    KGCRitasChristmas( const KGCRitasChristmas& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasChristmas& operator=( const KGCRitasChristmas& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete();

	void OnClose();
	KGCRitasPanel* GetRitasPanel()	{ return m_pkRitasPanel; }
	KGCRitasResultDlg* GetResultDlg() { return m_pkRitasReslutDlg; }
	KGCRitasRankingDlg*	GetRankingDlg() { return m_pkRitasRankingDlg; }
private:
	KGCRitasPanel*			m_pkRitasPanel;
	KGCRitasRankingDlg*		m_pkRitasRankingDlg;
	KGCRitasResultDlg*		m_pkRitasReslutDlg;

	KD3DWnd*	m_pkBtnClose;

	EGCBackgroundMusic m_backMusic;
};

DEFINE_WND_FACTORY( KGCRitasChristmas );
DECLARE_WND_FACTORY( KGCRitasChristmas );
DECLARE_WND_FACTORY_NAME( KGCRitasChristmas );
