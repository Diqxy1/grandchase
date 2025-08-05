#pragma once
class KGCCommonItemWnd;
class KGCForgeItemBreakUp : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCForgeItemBreakUp );

    const static int m_iMAX_ITEM_VIEW_WIDTH_COUNT = 5;
    const static int m_iMAX_ITEM_VIEW_HEGHIT_COUNT = 9;


    /// Default constructor
    KGCForgeItemBreakUp( void );
    /// Default destructor
    virtual ~KGCForgeItemBreakUp( void );
    /// Copy constructor
    KGCForgeItemBreakUp( const KGCForgeItemBreakUp& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCForgeItemBreakUp& operator=( const KGCForgeItemBreakUp& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
enum
{
    EM_INVEN_COLUMN = 9,
    EM_INVEN_ROW = 5,
    EM_INVEN_WIDH_SIZE = 45,
    EM_INVEN_HEIGHT_SIZE = 45,
};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void OnCreateItemBackList( void );
    void OnInitItemBackList( void );
    void OnCreateItemImgList( void );
    void OnInitItemImgList( void );
    void OnCreateItemImgEdge( void );
    void OnClickItem( const KActionEvent& event );
    void ClickItem( GCITEMUID giItem, KGCItemImgWnd* pWnd );
    void FrameMoveInEnabledState( void );
    void RefreshItemList( void );
    void RefreshPage( void );
    void OnBoundItem( const KActionEvent& event );
    void OnScrollPos();
    void UpdateInvenItemList();
    KD3DScrollbar* m_pkScrollbar;
    int m_iScrollpos;
    std::vector<KGCCommonItemWnd*>  m_vecInvenItemImgList;
    std::vector<GCITEMUID>          m_vecItemUIDList;
    KncStopWatch        m_kStopWatch;

private:
    
    std::vector<KD3DWnd*> m_vecItemImgBackList;
    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>> m_vecItemImgList;

    KD3DWnd* m_pkItemImgEdge;

    bool m_bCheckBox;
};

DEFINE_WND_FACTORY( KGCForgeItemBreakUp );
DECLARE_WND_FACTORY( KGCForgeItemBreakUp );
DECLARE_WND_FACTORY_NAME( KGCForgeItemBreakUp );
