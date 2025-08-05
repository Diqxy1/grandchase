#pragma once

class KGCDungeonRankSlot;

class KGCDungeonRankingPanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonRankingPanel );
    /// Default constructor
    KGCDungeonRankingPanel( void );
    /// Default destructor
    virtual ~KGCDungeonRankingPanel( void );
    /// Copy constructor
    KGCDungeonRankingPanel( const KGCDungeonRankingPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonRankingPanel& operator=( const KGCDungeonRankingPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_SLOT_NUM = 8,
        EM_MAX_PAGE = 13,
    };

    enum
    {
        EM_CURRENT_RANK = 0,
        EM_OLD_RANK = 1,
        EM_SIZE_RANK = 2,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );


    void TestFunction( bool bCurrent, int nPage );

public:
    void SetRankingState( int nState );
    void SetPage( int nPage ) { m_nPage = nPage; }
    bool EnableGetData();

    bool IsMyRank( const KDungeonRankInfo& kRank );
    
    void ReqRankingData();

public:
    void ChangeRankingPage( int nPage );
    void UpdateRankingData();
    void UpdateUI();
    void UpdateMyRankinUI( bool bRank );

    void LockUI( bool bLock );



public:
    void OnLeftBtn();
    void OnRightBtn();
    void OnRefrashBtn();


public:
    int m_nState;
    int m_nPage;

    DWORD m_dwLastReqTick;


public:
    KD3DWnd*    m_pkLeftBtn;
    KD3DWnd*    m_pkRightBtn;
    KD3DWnd*    m_pkRefrashBtn;

    KD3DWnd*    m_pkNoRank;
    KD3DWnd*    m_pkMyCurRank;
    KD3DWnd*    m_pkMyOldRank;

    KGCUINumber*    m_pkNumMyRank;       

    KSafeArray<KGCDungeonRankSlot*,EM_SLOT_NUM>     m_pkRankSlot;
};

DEFINE_WND_FACTORY( KGCDungeonRankingPanel );
DECLARE_WND_FACTORY( KGCDungeonRankingPanel );
DECLARE_WND_FACTORY_NAME( KGCDungeonRankingPanel );
