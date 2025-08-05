#ifndef _KGCPERFECTATTENDANCEBOX_H_
#define _KGCPERFECTATTENDANCEBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "GCItem.h"

class KGCAttendanceItemWnd;
class KD3DScrollbar;
class KGCItemInfoBox;
class KD3DStatic;

class KGCPerfectAttendanceBox: public KD3DDialog,
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPerfectAttendanceBox );

    KGCPerfectAttendanceBox(void);
    virtual ~KGCPerfectAttendanceBox(void);
    /// Copy constructor
    KGCPerfectAttendanceBox( const KGCPerfectAttendanceBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPerfectAttendanceBox& operator=( const KGCPerfectAttendanceBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EPerfectAttendanceResult
    {
        DESTROYED,
        EXIT,
        SELECT_REWARD_ITEM,
    };

    enum
    {
        NUM_ITEM_WND = 6,
        NUM_ITEM_SCROLL = 3,
    };

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
protected:
    virtual void OnCreate( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState();
    virtual void PostChildDraw( void );

    KD3DWnd*											m_pkExit;
    KD3DWnd*											m_pkPerfectAttendanceMark;
    KD3DWnd*											m_pkReceiveBtn;

    KD3DScrollbar*										m_pkScrollbar;
    int															m_iScrollPos;
    KSafeArray<KGCAttendanceItemWnd*,6>	m_apkItem;
    
    int                         m_iRewardType;
	int							m_iSelectedReward;

public:
    void OnScrollPos( int iPos );
    void SetRewardList( int iWeek_ );
    int GetRewardType() { return m_iRewardType; };

    void ShowItemInfo( void );

    GCItem*                     m_iGetItem;
};

DEFINE_WND_FACTORY( KGCPerfectAttendanceBox );
DECLARE_WND_FACTORY( KGCPerfectAttendanceBox );
DECLARE_WND_FACTORY_NAME( KGCPerfectAttendanceBox );

#endif	//_KGCPERFECTATTENDANCEBOX_H_