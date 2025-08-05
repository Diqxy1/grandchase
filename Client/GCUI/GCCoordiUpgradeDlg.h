#pragma once

class KGCCommonItemWnd;

class KGCCoordiUpgradeDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCoordiUpgradeDlg );
    /// Default constructor
    KGCCoordiUpgradeDlg( void );
    /// Default destructor
    virtual ~KGCCoordiUpgradeDlg( void );
    /// Copy constructor
    KGCCoordiUpgradeDlg( const KGCCoordiUpgradeDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiUpgradeDlg& operator=( const KGCCoordiUpgradeDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_INVEN_COLUMN = 4,
        EM_INVEN_ROW = 6,
        EM_INVEN_WIDH_SIZE = 64,
        EM_INVEN_HEIGHT_SIZE = 64,
    };

    enum
    {
        EM_COMMON_STATE = 0,
        EM_RESULT_STATE = 1,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );

public:
    void CreateInvenSlot();
    void InitUIState();
    void InitResultState();

    void UpdateCoordiList();
    void UpdateInvenItemList();
    void UpdateItemCount();

    void CheckItemInformation();
    void DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos);
    KItem GetResultItem();

public:
    void OnOK();
    void OnClose();
    void OnAddItem();

    void OnCoordiSlot();
    void OnInvenItemList( int index );
    void OnScrollPos();

public:
    GCITEMUID   m_ItemUID;
    int         m_iScrollpos;
    int         m_nState;

public:
    KD3DStatic*     m_pkStaticItemNum;
    KD3DStatic*     m_pkStaticCostNum;

    KD3DWnd*        m_pkBtnOk;
    KD3DWnd*        m_pkBtnClose;
    KD3DWnd*        m_pkBtnAdd;

    KGCCommonItemWnd*   m_pkCoordiItem;
    KGCCommonItemWnd*   m_pkResultItem;

    std::vector<KGCCommonItemWnd*> m_vecInvenItemImgList;
    std::vector<GCITEMUID>      m_vecItemUIDList;

    KD3DScrollbar* m_pkScrollbar;

};

DEFINE_WND_FACTORY( KGCCoordiUpgradeDlg );
DECLARE_WND_FACTORY( KGCCoordiUpgradeDlg );
DECLARE_WND_FACTORY_NAME( KGCCoordiUpgradeDlg );
