#pragma once

enum AttendanceSlotState {
    ASS_LOCK        = 0,
    ASS_OPEN        = 1,
    ASS_STAMPED     = 2,
    ASS_CLAIMABLE   = 3,
};

class KGCAttendanceStampSlot : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAttendanceStampSlot );
    /// Default constructor
    KGCAttendanceStampSlot( void );
    /// Default destructor
    virtual ~KGCAttendanceStampSlot( void );
    /// Copy constructor
    KGCAttendanceStampSlot( const KGCAttendanceStampSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAttendanceStampSlot& operator=( const KGCAttendanceStampSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkSlotActive;
    KD3DWnd*                    m_pkSlotPassive;
    KD3DWnd*                    m_pkTimed;
    KD3DWnd*                    m_pkStamped;
    KD3DStatic*                 m_pkStaticDays;
    KD3DStatic*                 m_pkStaticDuration;
    KGCItemImgWnd*              m_pkRewardItemImgWnd;

    int m_nIndex;
    VEC_REWARD m_vecReward;
    AttendanceSlotState m_eState;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void SetSlotInfo( int nIndex );
    void InitWndState();
    void OnClickClaimReward();
    void DrawInfoWindow();
    void UpdateWndState();


};

DEFINE_WND_FACTORY( KGCAttendanceStampSlot );
DECLARE_WND_FACTORY( KGCAttendanceStampSlot );
DECLARE_WND_FACTORY_NAME( KGCAttendanceStampSlot );
