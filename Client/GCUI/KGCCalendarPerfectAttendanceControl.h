#ifndef _KGCCALENDARPERFECTATTENDANCEBOXCONTROL_H_
#define _KGCCALENDARPERFECTATTENDANCEBOXCONTROL_H_

class KGCCalendarPerfectAttendanceControl: public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCalendarPerfectAttendanceControl );

	KGCCalendarPerfectAttendanceControl(void);
	virtual ~KGCCalendarPerfectAttendanceControl(void);
	/// Copy constructor
	KGCCalendarPerfectAttendanceControl( const KGCCalendarPerfectAttendanceControl& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCalendarPerfectAttendanceControl& operator=( const KGCCalendarPerfectAttendanceControl& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

	void AllRenderOff();

	KD3DWnd*            m_pkPerfectAttendanceBack;
	KD3DStatic*			m_pkPerfectAttendanceTitle;

	KD3DWnd*            m_pkPerfectAttendanceMark;
	KD3DWnd*            m_pkPerfectAttendanceCheck;

	bool m_bChecked;
	int m_iWeek;

public:
	void SetWeekDay( int iNum );
	void SetStamp();
	void Check();
	bool IsChecked( void ) { return m_bChecked; }
};

DEFINE_WND_FACTORY( KGCCalendarPerfectAttendanceControl );
DECLARE_WND_FACTORY( KGCCalendarPerfectAttendanceControl );
DECLARE_WND_FACTORY_NAME( KGCCalendarPerfectAttendanceControl );

#endif