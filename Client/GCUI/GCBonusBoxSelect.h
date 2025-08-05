#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GCD3DWndEffect.h"
class KD3DStatic;
class KGCItemImgWnd;
class KGCD3DWndEffect;

class KGCBonusBoxSelect : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBonusBoxSelect );
    /// Default constructor
    KGCBonusBoxSelect( void );
    /// Default destructor
    virtual ~KGCBonusBoxSelect( void );
    /// Copy constructor
    KGCBonusBoxSelect( const KGCBonusBoxSelect& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBonusBoxSelect& operator=( const KGCBonusBoxSelect& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& even );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void PostChildDraw( void );

    enum BONUS_BOX_ANIM_STATE
    {
        BBAS_SHOW_TITLE,
        BBAS_SHOW_BONUS_BOX,
        BBAS_BEFORE_TIME_COUNT,
        BBAS_TIME_COUNT,
        BBAS_BEFORE_BONUS_BOX_OPEN,
        BBAS_BONUS_BOX_OPEN,
        BBAS_ALL_PROCESS_COMPLETE,
        BBAS_MAX,
    };

private:
    enum 
    { 
        SELECT_BONUS_BOX_TIME_LIMIT = 5 ,
        SELECT_BONUS_BOX_WAIT_TIME = 8 ,
    };

    KD3DWnd*            m_pkBonusBox[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DWnd*            m_pkBonusBoxOpened[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DWnd*            m_pkOwnerBackGround[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DWnd*            m_pkItemImgBackground[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DWnd*            m_pkBonusGpBackground[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DWnd*            m_pkItemNameBackground[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DStatic*         m_pkStaticOwner[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DStatic*         m_pkStaticBonusGP[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DStatic*         m_pkStaticBonusItemName[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KGCItemImgWnd*      m_pkItemImg[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    bool                m_bSelected[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    DWORD               m_dwUserUID[ OLD_MAX_DUNGEON_PLAYER_NUM ];

    KD3DWnd*            m_pkBoxNumber[ OLD_MAX_DUNGEON_PLAYER_NUM ];
    KD3DWnd*            m_pkBoxNumberBack[ OLD_MAX_DUNGEON_PLAYER_NUM ];    
    KD3DWnd*            m_pkSelectBoxComment;

    KD3DWnd*            m_pkSelectBoxTitle;
    KD3DWnd*            m_pkLimitTimeTitle;
    DWORD               m_dwTimeCheck;
    DWORD               m_dwSelectLimitTime;
    bool                m_bAlreadySelectMine; //내가 선택을 했는지..
    bool                m_bSendedOpenBoxPacket;
    bool                m_bAllProcessComplete;
    bool                m_bSendReadyPacket;
    float               m_fBonusBoxRotateZ;
    bool                m_bSendStartBonusBoxSelect;

    std::vector< std::pair< int, DWORD> > m_vecBoxSelectInfo;
    std::vector< KBonusBox > m_vecBonusBoxInfo;
    std::set< DWORD >   m_setReadyUserUID;
    int                 m_dwCurrentBonusBoxAnimIndex;
    int                 m_dwCurrentNumberAnimIndex;
    int                 m_dwCurrentNumberBackAnimIndex;
    float               m_fTimeParameter;

    D3DXVECTOR2         m_vTileInitPos;
private:

    void ClickBonusBox( int iSelectIndex_ );
    
    void OpenBonusBox();
    void SendSelectBoxInfo();
    void MakeSelectBoxInfo();
    int GetSelectedIndexByUserUID( DWORD dwUserUID_ );
    int GetRandomIndex();
    bool CheckEnableSelect( int iSelectIndex_, DWORD dwUserUID_);
    bool CheckAllUserSelect();
    
    void InitAnimation();
    void OpenBonusBoxEffect();
    void RandomSelect();

public:
    bool IsAllProcessComplete() {return m_bAllProcessComplete; }
    void SetBonusBoxList( std::vector< KBonusBox > vecBonusBoxInfo_ );
    bool SelectBonusBox( int iSelectIndex_, DWORD dwUserUID_  );
    void SetSelectBoxInfo( std::vector< std::pair<int, DWORD > > vecSelectBoxInfo_ ) { m_vecBoxSelectInfo.swap( vecSelectBoxInfo_ ); }
    bool CheckAllUserReady();
    void AddReadyUser( DWORD dwUserUid_);

private:
    KGCD3DWndEffect m_EffectSelectBoxTtitle; //
    KGCD3DWndEffect m_EffectLimitTimeTitle; // 
    KGCD3DWndEffect m_EffectSelectCommentTitle;
    KGCD3DWndEffect m_EffectBonusBox[OLD_MAX_DUNGEON_PLAYER_NUM];
    KGCD3DWndEffect m_EffectBoxNumber[OLD_MAX_DUNGEON_PLAYER_NUM];
    KGCD3DWndEffect m_EffectBoxNumberBack[OLD_MAX_DUNGEON_PLAYER_NUM];

    BONUS_BOX_ANIM_STATE m_eAnimState;

private:
    typedef void (KGCBonusBoxSelect::*InitFunc)();
    typedef void (KGCBonusBoxSelect::*FrameMoveFunc)();

    InitFunc m_pInitAnimFunc[ BBAS_MAX ];
    FrameMoveFunc m_pFrameMove;
    FrameMoveFunc m_pFrameMoveAnimFunc[ BBAS_MAX ];

    void InitShowTitle();
    void InitShowBonusBox();
    void InitBeforeTimeCount();
    void InitTimeCount();
    void InitBeforeBonusBoxOpen();
    void InitBonusBoxOpen();
    void InitAllProcessComplete();

    void FrameMoveShowTitle();
    void FrameMoveShowBonusBox();
    void FrameMoveBeforeTimeCount();
    void FrameMoveTimeCount();
    void FrameMoveBeforeBonusBoxOpen();
    void FrameMoveBonusBoxOpen();
    void FrameMoveAllProcessComplete();

public:
    void SetAnimState( BONUS_BOX_ANIM_STATE eAnimState_ );
};

DEFINE_WND_FACTORY( KGCBonusBoxSelect );
DECLARE_WND_FACTORY( KGCBonusBoxSelect );
DECLARE_WND_FACTORY_NAME( KGCBonusBoxSelect );

