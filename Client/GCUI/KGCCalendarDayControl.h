#ifndef _KGCCALENDARDAYCONTROL_H_
#define _KGCCALENDARDAYCONTROL_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCNumberControl;
class KGCAlertWnd;

class KGCCalendarDayControl: public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCalendarDayControl );

    KGCCalendarDayControl(void);
    virtual ~KGCCalendarDayControl(void);
    /// Copy constructor
    KGCCalendarDayControl( const KGCCalendarDayControl& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCalendarDayControl& operator=( const KGCCalendarDayControl& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void AllRenderOff();

    KD3DWnd*            m_pkDayBack;
	KD3DStatic*			m_pkDayNum;

    KD3DWnd*            m_pkStampCheck;
    KD3DWnd*            m_pkStampTriangle;
    KD3DWnd*            m_pkStampNoCheck;
    KD3DWnd*            m_pkStampLucky;

    int							m_iToday;

	bool						m_bChecked;
    bool						m_bToday;

public:
    KGCAlertWnd*        m_pkCheckButton;
	
	enum {
		DAY_TODAY = 0,
		DAY_DEFAULT = 1,
		DAY_MONTH = 2
	};

public:
    void SetDay( int iNum, int state, bool bNoCheckDay );
    void SetStampType( char cSealType );
	bool IsChecked( void ) { return m_bChecked; }
    bool IsToday( void ) { return m_bToday; }
};

DEFINE_WND_FACTORY( KGCCalendarDayControl );
DECLARE_WND_FACTORY( KGCCalendarDayControl );
DECLARE_WND_FACTORY_NAME( KGCCalendarDayControl );

#endif	//_KGCCALENDARDAYCONTROL_H_