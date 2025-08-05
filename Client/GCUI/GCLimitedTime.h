#ifndef _GCLIMITEDTIME_H_
#define _GCLIMITEDTIME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CLOCK_ANI_STOP_RATE 2
#define CLOCK_ANI_COOL_TIME  55
#define CLOCK_ANI_DELAY_TIME 2

class KGCLimitedTime : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCLimitedTime );
    /// Default constructor
    KGCLimitedTime( void );
    /// Default destructor
    virtual ~KGCLimitedTime( void );
    /// Copy constructor
    KGCLimitedTime( const KGCLimitedTime& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLimitedTime& operator=( const KGCLimitedTime& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event ){};

public:
    void HiddenNumber();
    void SetTime( float fTime );

private:
    KD3DWnd *m_pkClock;
    KD3DWnd *m_pkMitNumber1[10];
    KD3DWnd *m_pkMitNumber10[6];
    KD3DWnd *m_pkSecNumber1[10];
    KD3DWnd *m_pkSecNumber10[6];

private:
    int m_iTremorFrame;
    bool m_bShowClockAni;
};

DEFINE_WND_FACTORY( KGCLimitedTime );
DECLARE_WND_FACTORY( KGCLimitedTime );
DECLARE_WND_FACTORY_NAME( KGCLimitedTime );

#endif  // _GCLIMITEDTIME_H_