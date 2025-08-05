#pragma once

#include "GCChallengeErrandManager.h"

#define NUM_ERRAND_SHOP                 9


class KGCChallengeErrandPlayer;
class KGCChallengeErrandModule;
class KGCChallengeErrandBuyList;

class KGCChallengeErrandPanel : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChallengeErrandPanel );
    /// Default constructor
    KGCChallengeErrandPanel( void );
    /// Default destructor
    virtual ~KGCChallengeErrandPanel( void );
    /// Copy constructor
    KGCChallengeErrandPanel( const KGCChallengeErrandPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChallengeErrandPanel& operator=( const KGCChallengeErrandPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum eErrandGameState{
        ERRAND_CHECK_GAME                     = 0,
        ERRAND_READY_GAME                     = 1,
        ERRAND_PLAYING_GAME                   = 2,
        ERRAND_PLAYER_MOVE                    = 3,
        ERRAND_END_GAME                       = 4,
        ERRAND_WAIT_REMAIN                    = 5,
    };

    enum eErrrandMoveState{
        PLAYER_MOVE_NONE    = -1,
        PLAYER_MOVE_LEFT    = 0,
        PLAYER_MOVE_RIGHT   = 1,
        PLAYER_MOVE_UP      = 2,
        PLAYER_MOVE_DOWN    = 3,
    };

    enum 
    {
        NUM_TIME_MIT_ONE    = 0,
        NUM_TIME_MIT_TWO    = 1,
        NUM_TIME_SEC_ONE    = 2,
        NUM_TIME_SEC_TWO    = 3,
        NUM_TIME_CIPHER     = 4,
    };

    enum
    {
        NUM_SUCCES_COUNT    = 0,
        NUM_SUCCES_MAX      = 1,
        NUM_MAX_COUNT       = 2,
    };
public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

public:
    void OnStart( void );
    void OnReward( void );

    void CheckFrameMove( void );
    void ReadyFrameMove( void );
    void PlayFrameMove( void );
    void UpdateKey( void );
    void PlayEndCheck( void );
    void UpdateErrandObj( void );
    void MovePlayer( void );

    void PlayerMoveRight( void );
    void PlayerMoveLeft( void );
    void PlayerMoveUp( void );
    void PlayerMoveDown( void );
    void PlayerInputSpace( void );
    void MerryAppear( void );

    bool MerryMeetCheck( void );
    bool CompareItemList( void );
    void BuyErrandItem( void );
    void RendomShop( void );

    void ResetStageClear( void );
    void ResetGame( void );

    void SetPlayTime( void );
    void RemainTime( void );
    void GameEndReq( void );
    void GameRewardReq( void );

    void ClearShop( void );

    int GetPanelState( void ) { return m_eGameNowState; }
    bool GetReward( void ) { return m_bReciveReward; }

private:
    int m_iStage;
    float m_fDetPos;
    eErrandGameState  m_eGameNowState;
    eErrrandMoveState m_eMoveState;

    KGCChallengeErrandModule*       m_pkErrandHome;
    KGCChallengeErrandPlayer*       m_pkErrandPlayer;
    KGCChallengeErrandBuyList*      m_pkErrandBuyList;
    std::vector< D3DXVECTOR2 >      m_vecShopPos;
    KSafeArray<KGCChallengeErrandModule*,NUM_ERRAND_SHOP>   m_apErrandShop;
    KSafeArray<KD3DStatic*,NUM_ERRAND_SHOP>                 m_apStaticHaveNum;

    KGCUINumber*    m_apTime[NUM_TIME_CIPHER];
    KGCUINumber*    m_apSuccesCount[NUM_MAX_COUNT];

    KD3DWnd*        m_pkImgGameStart;
    KD3DWnd*        m_pkImgGmaeEnd;
    KD3DWnd*        m_pkImgGmaeSucces;

    DWORD m_dwAccTimeNot;
    DWORD m_dwPlayTime;
    DWORD m_dwStageDlayTime;
    int m_nPrevTime;

    DWORD m_dwAccMerryApperTime;

    int m_nRandomMerry;
    int m_nMarryAppearInterval;
    int m_nMarryKeepAliveTime;

    bool m_bReciveReward;
};

DEFINE_WND_FACTORY( KGCChallengeErrandPanel );
DECLARE_WND_FACTORY( KGCChallengeErrandPanel );
DECLARE_WND_FACTORY_NAME( KGCChallengeErrandPanel );
