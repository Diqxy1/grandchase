#pragma once



class KGCHeroDungeonSeason5Icon :	public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonSeason5Icon );
    /// Default constructor
    KGCHeroDungeonSeason5Icon( void );
    /// Default destructor
    virtual ~KGCHeroDungeonSeason5Icon( void );
    /// Copy constructor
    KGCHeroDungeonSeason5Icon( const KGCHeroDungeonSeason5Icon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonSeason5Icon& operator=( const KGCHeroDungeonSeason5Icon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    

    KD3DWnd*                m_pkDunDummyBound;
    void SetDungeonNameText( const std::wstring& strText );
    void SetRenderDungeonLimitLevel( bool bRender);
    void SetWndHover();
    void SetWndLock();
    void SetWndDefault();
    KD3DWnd::ED3DWndMode GetIconMode( void );
    void SetAvaliableTime(bool dun);
    bool GetAvaliableTime();
    void SetAvaliableLevel(bool dun);
    bool GetAvaliableLevel();
private:

    
    KD3DWnd*                m_pkDunback;
    KD3DWnd*                m_pkDunIcon;
    KD3DWnd*                m_pkDunLimitLevel;
    KD3DStatic*             m_pkDunName;
    int                     m_iModeID;
    bool                    m_bAvailableTime;
    bool                    m_bAvailableLevel;

public:
};

DEFINE_WND_FACTORY( KGCHeroDungeonSeason5Icon );
DECLARE_WND_FACTORY( KGCHeroDungeonSeason5Icon );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonSeason5Icon );




class KGCHeroDungeonSeason5 :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonSeason5 );
    /// Default constructor
    KGCHeroDungeonSeason5( void );
    /// Default destructor
    virtual ~KGCHeroDungeonSeason5( void );
    /// Copy constructor
    KGCHeroDungeonSeason5( const KGCHeroDungeonSeason5& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonSeason5& operator=( const KGCHeroDungeonSeason5& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnEnterScene( void );
    virtual void OnInitialize();

public:
  

    enum GC_HERO_DUNGEON
    {
        HERO_FURNACE_OF_HELL = 0,       //지옥의 용광로
        HERO_SANCTUM_OF_DESTRUCTION,    //파멸의 성소
        HERO_PHANTASMICSECTOR,          //환영의 미궁
        HERO_VERKAS,                    //베르카스의 요새
        HERO_EXTINCTION,                //소멸의 탑
		HERO_HENIR,
        HER0_BERCO_DA_ORIGEM,
        HERO_TIME_DISPLACEMENT,
        NUM_HERO_DUNGEON,
    };
    
    void SetHeroDungeonListState();
    void SetHeroDungeonMouseHoverState(GC_HERO_DUNGEON index);
    void SetEnterInfoToolTip(EGCGameMode _eGameMode);
    bool IsOpenTimeHeroDungeon(EGCGameMode _eGameMode );
    int GetNextEnteranceRemainTime();
    int GetCloseHeroDungeonRemainTime();
    void SetLimitInfo(EGCGameMode _eGameMode);
    void SetHeroDungeonID();
    void SetEnterSelectedHeroDungeon();
    bool IsAvailableHeroDungeon();
    void OnExit();

private:
    KD3DWnd*                m_pkCloseBtn;
    
    std::vector<std::pair<GC_HERO_DUNGEON,EGCGameMode> > m_vecHeroDunID;
    KGCHeroDungeonSeason5Icon* m_pkDungeonIconList[NUM_HERO_DUNGEON];

private:
    int m_iRemainNextEnteranceTime;
    int m_iRemainCloseTime;
    bool m_bOpenTime;
    bool m_bAvailableTime;
    bool m_bAvailableLevel;
    GC_HERO_DUNGEON m_eSelectedHeroDungeon;
    

public:
};

DEFINE_WND_FACTORY( KGCHeroDungeonSeason5 );
DECLARE_WND_FACTORY( KGCHeroDungeonSeason5 );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonSeason5 );

