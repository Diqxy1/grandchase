#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KD3DStatic;
class KGCMonsterCard;
class KGCItemImgWnd;
class KGCMonsterCardContents;

class KGCMonsterCardBook : public KD3DDialog,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMonsterCardBook );
    /// Default constructor
    KGCMonsterCardBook( void );
    /// Default destructor
    virtual ~KGCMonsterCardBook( void );
    /// Copy constructor
    KGCMonsterCardBook( const KGCMonsterCardBook& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMonsterCardBook& operator=( const KGCMonsterCardBook& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostChildDraw( void );
    virtual void OnDestroyComplete( void );

public:
    enum
    {   
        MONSTER_CARD_SIMPLE_INFO_HEIGHT = 3,
        MONSTER_CARD_SIMPLE_INFO_WIDTH = 5,
        MAX_MONSTER_CARD_SIMPLE_INFO = 15,
        CLICK_MONSTER_CARD_SIMPLE_INFO  = 200002,

    };
private:
    struct FilterBtnBySlot
    {
        KD3DWnd*    pkSlotFilterBtn;
        DWORD       dwSlotFilter;
    };

    struct FilterBtnByGrade
    {
        KD3DWnd*    pkGradeFilterBtn;
        char        cGradeFilter;
    };

    struct FilterBtnByOwn
    {
        KD3DWnd*    pkOwnFilterBtn;
        bool        bOwnFilter;
    };

    enum ESLOTFILTERBTNTYPE
    {
        ESFBT_ALL,
        ESFBT_HELMET,
        ESFBT_JACKET_MANTLE,
        ESFBT_GLOVES_WEAPON,
        ESFBT_PANTS_SHOES,
        ESFBT_ACCESSORIES,
        ESFBT_MAX,
    };

    enum EGRADEFILTERBTNTYPE
    {
        EGFBT_ALL,
        EGFBT_NORMAL,
        EGFBT_RARE,
        EGFBT_EPIC,
        EGFBT_LEGEND,
        EGFBT_MYSTIC,
        EGFBT_MAX,
    };

    enum EOWNFILTERBTNTYPE
    {
        EOFBT_ALL,
        EOFBT_OWN,
        EOFBT_MAX,
    };

    KGCMonsterCardContents*     pkMonsterCardContents[ MAX_MONSTER_CARD_SIMPLE_INFO ];
    KGCMonsterCardContents*     pkMonsterCardContentsDummy;

    KGCMonsterCardContents*     pkMonsterCardContentsBig;
    KGCMonsterCardContents*     pkMonsterCardContentsBigDummy;
   
    KD3DWnd*        m_pkPrePageBtn;
    KD3DWnd*        m_pkNextPageBtn;
    KD3DWnd*        m_pkCloseBtn;

    KD3DStatic*     m_pkOwnCradCount;
    KD3DStatic*     m_pkCurrentPage;

    DWORD           m_dwClickedSlotFilterIndex;
    DWORD           m_dwClickedGradeFilterIndex;
    DWORD           m_dwClickedOwnFilterIndex;

    std::vector< GCITEMID > m_vecShowMonsterCardList;

    FilterBtnBySlot         m_stSlotFilterBtn[ ESFBT_MAX ];
    FilterBtnByGrade        m_stGradeFilterBtn[ EGFBT_MAX ];
    FilterBtnByOwn          m_stOwnFilterBtn[ EOFBT_MAX ];
    int                     m_iCurrentPage;

    KActionListener*    m_pkReceiveActionListener;
    KD3DWnd*            m_pkActionEventTransfer;

    bool                m_bRate;
    int                 m_iOrderComma;
    int                 m_iMaxPage;
private:
    void SetMonsterCardSimpleInfoList();
    void MakeMonsterCardShowList();
    void OnClose();
    void ClickMovePageBtn( int iMoveCount_ );

public:
    void SetReceiveActionListener( KActionListener* pkReceiveActionEvent_ ) { m_pkReceiveActionListener = pkReceiveActionEvent_; }
    void SetActionEventTransfer( KD3DWnd* pkActionEventTransfer_ ) { m_pkActionEventTransfer = pkActionEventTransfer_; }
    void SetClickedSlotFilterBtn( DWORD dwSlotPosition );
    
    KGCAttributeTable GetAttributebyMonsterCardID(DWORD dMonsterCardID);
    int  GetValuebyMonsterCardID(DWORD dMonsterCardID);
    D3DXVECTOR4  GetTexCoordbyMonsterCardID(DWORD dMonsterCardID);
    bool MakeItemOnFirstSlot( IN DWORD dwItemID );
    int GetMonsterCardGrade(DWORD dwItemID);

};

DEFINE_WND_FACTORY( KGCMonsterCardBook );
DECLARE_WND_FACTORY( KGCMonsterCardBook );
DECLARE_WND_FACTORY_NAME( KGCMonsterCardBook );

