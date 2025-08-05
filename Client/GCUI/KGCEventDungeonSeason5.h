#pragma once

struct sEventDungeonTexInfo
{
    int             iTexNum;
    D3DXVECTOR4     dxVecTexCoord;

    void clear()
    {
        iTexNum = -1;
        dxVecTexCoord = D3DXVECTOR4(0,0,0,0);
    }
};

class KGCEventDungeonSeason5Icon :	public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCEventDungeonSeason5Icon );
    /// Default constructor
    KGCEventDungeonSeason5Icon( void );
    /// Default destructor
    virtual ~KGCEventDungeonSeason5Icon( void );
    /// Copy constructor
    KGCEventDungeonSeason5Icon( const KGCEventDungeonSeason5Icon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEventDungeonSeason5Icon& operator=( const KGCEventDungeonSeason5Icon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

public:
    void SetAvaliableDay( bool dun );
    bool GetAvaliableDay();
    sEventDungeonTexInfo GetEventDunIconTexInfo( ED3DWndMode eWndMode );
    sEventDungeonTexInfo GetEventDunNameTexInfo( ED3DWndMode eWndMode );
    sEventDungeonTexInfo GetEventDunBackTexInfo( ED3DWndMode eWndMode );
    void SetEventDungeonIconTexInfo( ED3DWndMode eWndMode, sEventDungeonTexInfo sTexinfo );
    void SetEventDungeonNameTexInfo( ED3DWndMode eWndMode, sEventDungeonTexInfo sTexinfo );
    void SetEventDungeonBackTexInfo( ED3DWndMode eWndMode, sEventDungeonTexInfo sTexinfo );
    void SetEventDungeonTexInfo();
    void SetWndHover();
	void SetLock( bool bLock ) { m_bLock = bLock; }
	bool IsLock() { return m_bLock; }
    void SetModeID( int imodeid );
    int  GetModeID();

private:


    KD3DWnd*                m_pkDunback;
    KD3DWnd*                m_pkDunIcon;
    KD3DWnd*                m_pkDunName;
    bool                    m_bAvailableDay;
    int						m_iModeID;
	bool					m_bLock;

public:
    std::vector<std::pair<int,sEventDungeonTexInfo>> m_sEventDunIconTexInfo;
    std::vector<std::pair<int,sEventDungeonTexInfo>> m_sEventDunNameTexInfo;
    std::vector<std::pair<int,sEventDungeonTexInfo>> m_sEventDunBackTexInfo;
};

DEFINE_WND_FACTORY( KGCEventDungeonSeason5Icon );
DECLARE_WND_FACTORY( KGCEventDungeonSeason5Icon );
DECLARE_WND_FACTORY_NAME( KGCEventDungeonSeason5Icon );




class KGCEventDungeonSeason5 :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCEventDungeonSeason5 );
    /// Default constructor
    KGCEventDungeonSeason5( void );
    /// Default destructor
    virtual ~KGCEventDungeonSeason5( void );
    /// Copy constructor
    KGCEventDungeonSeason5( const KGCEventDungeonSeason5& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEventDungeonSeason5& operator=( const KGCEventDungeonSeason5& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnEnterScene( void );
    virtual void OnInitialize();
    

public:
    enum GC_EVENT_DUNGEON
    {
        EVENT_NIGHTMARECIRCUS = 0,          //�Ǹ��� ��Ŀ�� 
        EVENT_GHOSTTRAIN,                    //���迭��
        EVENT_HUNGRYPET,                    //����� ����� ���� 
        EVENT_MOONLIGHTYOKAIVILLAGE,        //�߽���
        EVENT_ECLIPSESOLAR,
		EVENT_REVENGE_OF_TALIN,             //Ż���� ����
        EVENT_JUDGEMENT_OF_KING_SLIME,
		EVENT_OLDDUNGEON,
		EVENT_ATONEMENT,
		EVENT_HALLOWEENMANSION,
		EVENT_PROJECTMARK,
		EVENT_PIXELWORLD,
        EVENT_COLISEU,
        EVENT_MEMORY,
        NUM_EVENT_DUNGEON,
        NUM_EVENT_DUNGEON_DUMMY = NUM_EVENT_DUNGEON,
    };
    void SetEventDungeonMouseHoverState( GC_EVENT_DUNGEON index );
    void SetEnterInfoToolTip( EGCGameMode _eGameMode );
    void SetEventDungeonID();
    void SetEventDungeonTexCoord();
    void InitEventDungeonTexCoord();
    void GetEventDungeonTexInfo(ED3DWndMode eWndMode, int pos);
    void SetEnterSelectedEventDungeon();
    void OnExit();
    void OnInformation();
private:
    KD3DWnd*                m_pkCloseBtn;
    KD3DWnd*				m_pkHeroInfoBtn;
    std::vector<std::pair<EGCGameMode,KGCEventDungeonSeason5Icon* > > m_vecEventDunID;
    KGCEventDungeonSeason5Icon* m_pkDungeonIconList[NUM_EVENT_DUNGEON];
    KGCEventDungeonSeason5Icon* m_pkDungeonDummyIconList[NUM_EVENT_DUNGEON_DUMMY];
    GC_EVENT_DUNGEON m_eSelectedEventDungeon;
private:
 //   std::map<EGCGameMode, sEventDungeonTexInfo>    m_mapEventDungeonTexInfo;
    bool m_bAtOnce
;
public:
};

DEFINE_WND_FACTORY( KGCEventDungeonSeason5 );
DECLARE_WND_FACTORY( KGCEventDungeonSeason5 );
DECLARE_WND_FACTORY_NAME( KGCEventDungeonSeason5 );

