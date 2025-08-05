#ifndef _KGCGiftBoxState_H_
#define _KGCGiftBoxState_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KGCGiftBoxState : public KD3DWnd // extends
    , public KActionListener 
{
public:
    DECLARE_CLASSNAME( KGCGiftBoxState );
    /// Default constructor
    KGCGiftBoxState( void );
    /// Default destructor
    virtual ~KGCGiftBoxState( void );
    /// Copy constructor
    KGCGiftBoxState( const KGCGiftBoxState& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGiftBoxState& operator=( const KGCGiftBoxState& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EGIFTBOX_STATE {
        STATE_NORMAL = 0,
        STATE_REWARD_READY = 1,
        STATE_SIZE = 2,
    };

    enum EGIFTBOX_EVENT_TYPE {
        NEW_USER = 0, 
        EVENT_REWARD = 1, 
        EVENT_TYPE_SIZE = 2,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnInitialize(void);

public:
    void ClickNewUserReward(void);
    void SetGiftBoxInfo( int iEventType , bool IsRewardReady );
    void UpdateTime(int iEventType);
    void SetPlayTime();
    void ClickEventReward(void);
    void SetNextEventReward(bool bNextEvent);
    void SetNewUserToolTip( GCITEMID gcItemID );
    void CheckParticleTimeForBtn();
    void ShowConfirmBtnParticle();
    void ShowRewardParticle();

protected:
    KD3DWnd* m_apkBoxShape[STATE_SIZE];
    KD3DStatic* m_apkWnd[EVENT_TYPE_SIZE];
    KD3DStatic* m_apkStatic[EVENT_TYPE_SIZE];
    bool m_bBlinkOn;
    bool m_bNewUserTimeOut;
    bool m_bNextEventReward;
    int m_nNewUserRemainTime;
    int m_dwAccTimeNot;
    int m_nPrevTime;
    int m_iParticleTime;
    int m_iEventtype;
};

DEFINE_WND_FACTORY( KGCGiftBoxState );
DECLARE_WND_FACTORY( KGCGiftBoxState );
DECLARE_WND_FACTORY_NAME( KGCGiftBoxState );

#endif // _KGCGiftBoxState_H_