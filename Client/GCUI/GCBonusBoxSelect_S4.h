#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCD3DWndEffect;
class KGCBonusBoxSelect_S4_Row;

class KGCBonusBoxSelect_S4 : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBonusBoxSelect_S4 );
    /// Default constructor
    KGCBonusBoxSelect_S4( void );
    /// Default destructor
    virtual ~KGCBonusBoxSelect_S4( void );
    /// Copy constructor
    KGCBonusBoxSelect_S4( const KGCBonusBoxSelect_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBonusBoxSelect_S4& operator=( const KGCBonusBoxSelect_S4& )
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
        SELECT_BONUS_BOX_TIME_LIMIT = 6 ,
        SELECT_BONUS_BOX_WAIT_TIME = 3 ,
    };

    std::map<DWORD, std::set<USHORT>> m_mapUserSelect;

    KD3DWnd*            m_pkGeneralTitle;
    KD3DWnd*            m_pkSpecialTitle;
    KD3DWnd*            m_pkRewardTitle;
    KGCBonusBoxSelect_S4_Row* m_pkBonusBoxRow[OLD_MAX_DUNGEON_PLAYER_NUM];

    KD3DWnd*            m_pkCountNum[SELECT_BONUS_BOX_TIME_LIMIT];
private:
    void ClickBonusBox();
    bool m_bAllProcessComplete;
    bool m_bSendBoxIndex;
    bool m_bSpecialReward;
    bool m_bStopCount;
    D3DXVECTOR2         m_vTileInitPos;

public:
    void InitUIs();
    void SetKeyNum( int iKey_ );
    void StopCountDown() { m_bStopCount = true; }

	bool	m_bOpenItemImg;

private:
    typedef void (KGCBonusBoxSelect_S4::*InitFunc)();
    typedef void (KGCBonusBoxSelect_S4::*FrameMoveFunc)();

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

    BONUS_BOX_ANIM_STATE m_eAnimState;
    DWORD               m_dwTimeCheck;

    KGCD3DWndEffect m_EffectBoxTtitle;

    void InitAnimation( void );

public:
    void SetAnimState( BONUS_BOX_ANIM_STATE eAnimState_ );
    void SetUserSelectBox( DWORD dwUserUID_, USHORT usBoxIndex_ );
    void SetEachUserBoxInfo( KRewardInfoList kRewardInfoList_ );
    bool IsAllProcessComplete() {return m_bAllProcessComplete; }
    void TitleSetting( bool IsSpecial_ );

	void SetEnd();
};

DEFINE_WND_FACTORY( KGCBonusBoxSelect_S4 );
DECLARE_WND_FACTORY( KGCBonusBoxSelect_S4 );
DECLARE_WND_FACTORY_NAME( KGCBonusBoxSelect_S4 );

