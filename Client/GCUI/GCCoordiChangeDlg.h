#pragma once

class KD3DWnd;
class KD3DStatic;
class KGCUserItemBox;
class KGCItemImgWnd;
class KGCCharacterSelect;
class KGCCoordiChangeDlg : public KD3DDialog    // extends
{
public:
    DECLARE_CLASSNAME( KGCCoordiChangeDlg );
    /// Default constructor
    KGCCoordiChangeDlg( void );
    /// Default destructor
    virtual ~KGCCoordiChangeDlg( void );
    /// Copy constructor
    KGCCoordiChangeDlg( const KGCCoordiChangeDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiChangeDlg& operator=( const KGCCoordiChangeDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        NUM_BUY_ITEM = 4,
        NUM_INVEN_COLS = 4,
        NUM_INVEN_ROWS = 9,
        NUM_READY_INVEN_COLS = 4,
        NUM_READY_INVEN_ROWS = 2,
    };

public:
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState();
    virtual void OnDestroy( void );

private:
    void OnClickClose();
    bool OnClickInvenItem( const KActionEvent& event );
    bool OnClickCoordiItem( const KActionEvent& event );
    bool OnClickReadyInvenItem( const KActionEvent& event );
    bool OnClickReadyCoordiItem( const KActionEvent& event );

    void OnClickChangeToCoordi();
    void OnClickChangeToInven();

    void OnClickBuyItem( const KActionEvent& event );
    void OnCharacterChange( const KActionEvent& event );

    bool OnReceiveChangeToCoordi( const KActionEvent& event );
    bool OnReceiveChangeToInven( const KActionEvent& event );

    void DiconnectAllChildEventProcedure();
    void ConnectAllChildEventProcedure();
    void RefreshItemList();
    void RefreshCapacity();
    int GetChangeItemCount();

    inline int GetInvenFullSize() { return g_kGlobalValue.m_iInvenCapacity; };
    inline int GetInvenUseSize() { return g_pItemMgr->GetTotalInventorySize(); };
    inline int GetCoordiFullSize() {
        return g_kGlobalValue.m_iInvenCapacity;
    };
    inline int GetCoordiUseSize() { return g_pItemMgr->GetCoordiInventorySize(); };

private:
    KGCUserItemBox* m_pkInven;
    KGCUserItemBox* m_pkCoordi;
    KGCUserItemBox* m_pkReadyInven;
    KGCUserItemBox* m_pkReadyCoordi;
    KD3DWnd*        m_pkClose;
    KD3DWnd*        m_pkBtnChangeToCoordi;
    KD3DWnd*        m_pkBtnChangeToInven;
    KD3DWnd*        m_pkBtnInvenNext;
    KD3DWnd*        m_pkBtnInvenPrev;
    KD3DWnd*        m_pkBtnCoordiNext;
    KD3DWnd*        m_pkBtnCoordiPrev;

    KD3DStatic*     m_pkStaticInvenTitle;
    KD3DStatic*     m_pkStaticCoordiTitle;
    KD3DStatic*     m_pkStaticInvenCapacity;
    KD3DStatic*     m_pkStaticCoordiCapacity;
    KD3DStatic*     m_pkStaticInfo;
    KGCCharacterSelect*     m_pkCharSelect;

    KGCItemImgWnd*  m_pkItemImg;
    KGCUINumber*    m_pkItemCount;

    int         m_iCharType;
    std::vector<GCITEMID>   m_vecChangeItems;
    std::map<KD3DWnd::uid_type,GCITEMID> m_mapItem;

    KGCItemImgWnd*  m_pkBuyItemImg[NUM_BUY_ITEM];
    KD3DWnd*        m_pkBtnBuy[NUM_BUY_ITEM];
};

DEFINE_WND_FACTORY( KGCCoordiChangeDlg );
DECLARE_WND_FACTORY( KGCCoordiChangeDlg );
DECLARE_WND_FACTORY_NAME( KGCCoordiChangeDlg );
