#pragma once

class KGCEventAlarmDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEventAlarmDlg );
    /// Default constructor
    KGCEventAlarmDlg( void );
    /// Default destructor
    virtual ~KGCEventAlarmDlg( void );
    /// Copy constructor
    KGCEventAlarmDlg( const KGCEventAlarmDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEventAlarmDlg& operator=( const KGCEventAlarmDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
    virtual void PreDraw( void );

public:
	void SetEventAlarm();
	void OnClose();
    void SetEventData( std::string strParticle, int nTitleString, int nDescString );

public:
	DWORD			m_dwFrameCount;

public:
	KD3DStatic*		m_pkEventTitle;
	KD3DStatic*		m_pkEventDesc;
};

DEFINE_WND_FACTORY( KGCEventAlarmDlg );
DECLARE_WND_FACTORY( KGCEventAlarmDlg );
DECLARE_WND_FACTORY_NAME( KGCEventAlarmDlg );
