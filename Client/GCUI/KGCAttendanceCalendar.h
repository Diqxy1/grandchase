#ifndef _KGCATTENDANCECALENDAR_H_
#define _KGCATTENDANCECALENDAR_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class KGCNumberControl;
class KGCUINumber;
class KGCCalendarDayControl;
struct KDailyInfo;
class KGCAlertWnd;
class KGCCalendarPerfectAttendanceControl;

class KGCAttendanceCalendar: public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCAttendanceCalendar );

    KGCAttendanceCalendar(void);
    virtual ~KGCAttendanceCalendar(void);
    /// Copy constructor
    KGCAttendanceCalendar( const KGCAttendanceCalendar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAttendanceCalendar& operator=( const KGCAttendanceCalendar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EAttendanceCalendarResult
    {
        DESTROYED,
        EXIT,
        PERFECT_ATTENDANCE,
    };

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
protected:
    virtual void OnCreate( void );	
    virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete();
    virtual void OnCreateComplete();

    KD3DWnd*							m_pkExit;

    // 월개근상 버튼
    KGCAlertWnd*						m_pkMonthCheckButton;

    // 년 표시
    KGCUINumber*						m_pkNumYear;
    // 월 표시
	KGCUINumber*						m_pkNumMonth;

    KD3DStatic*							m_pkPlayTimeNum;
	KD3DStatic*							m_pkPlayTimeText;
    KD3DStatic*							m_pkPoint;
	KD3DStatic*							m_pkPerfectAttendanceTitle;

    KD3DWnd*							m_pkMonthCheckMark;
	KD3DWnd*							m_pkTextMonth;

	KSafeArray<KD3DWnd*,12>		m_pkMonthStatics;
    KSafeArray<KD3DStatic*,7>		m_apkWeekText;
	KSafeArray<KGCCalendarPerfectAttendanceControl*, 6> m_apkPerfectAttendanceWeek;

    KSafeArray<KSafeArray<KGCCalendarDayControl*,7>,6> m_pkDay;

    void SetStamp();
    void SetDay();
    void SetPlayTime();

    int                     m_iPlayTime;
    int                     m_iAlertToggle;
    int                     m_iGetGCPoint;
	bool					m_bCreated;
public:
    void SetCalendar();

    void CloseCalendar();
    void SetGCPoint( int _nPoint );
};

DEFINE_WND_FACTORY( KGCAttendanceCalendar );
DECLARE_WND_FACTORY( KGCAttendanceCalendar );
DECLARE_WND_FACTORY_NAME( KGCAttendanceCalendar );

#endif	//_KGCATTENDANCECALENDAR_H_
