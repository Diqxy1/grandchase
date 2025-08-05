#ifndef _KGCALERTWND_H_
#define _KGCALERTWND_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

#define ALERT_TOGGLE_FRAME 25

class KGCAlertWnd : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCAlertWnd );
    /// Default constructor
    KGCAlertWnd( void );
    /// Default destructor
    virtual ~KGCAlertWnd( void );
    /// Copy constructor
    KGCAlertWnd( const KGCAlertWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAlertWnd& operator=( const KGCAlertWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum ALERT_TYPE
    {
        DEFAULT = 0,
        MISSION,
        MSN,
        CALENDAR,
		MYINFOSCENE,
		SKILLTREE,
		COUPLE,
        RECOMMEND,
        NUM_ALERT,
    };

public:
    virtual void FrameMoveInEnabledState();

protected:
    virtual void OnCreate();
    virtual void ActionPerformed( const KActionEvent& event );

private:
    KD3DWnd* m_pkAlert;
    int m_iToggleFrame;
    DWORD m_dwTime;

    ALERT_TYPE m_eAlertButtonType;

    bool ( KGCAlertWnd::*AlertCheckFunc[NUM_ALERT] )();

    bool AlertCheck_True();
    bool AlertCheck_Mission();
    bool AlertCheck_MSN();
    bool AlertCheck_Calendar();
	bool AlertCheck_MyInfoScene();
	bool AlertCheck_SkillTree();
	bool AlertCheck_CoupleWnd();
    bool AlertCheck_Recommend();

public:
    void SetAlertButtonType( ALERT_TYPE eType ){ m_eAlertButtonType = eType; }
};

DEFINE_WND_FACTORY( KGCAlertWnd );
DECLARE_WND_FACTORY( KGCAlertWnd );
DECLARE_WND_FACTORY_NAME( KGCAlertWnd );

#endif //_KGCALERTWND_H_
