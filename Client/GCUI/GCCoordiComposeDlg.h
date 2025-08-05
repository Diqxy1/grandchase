#pragma once

class KGCCommonItemWnd;

class KGCCoordiComposeDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCoordiComposeDlg );
    /// Default constructor
    KGCCoordiComposeDlg( void );
    /// Default destructor
    virtual ~KGCCoordiComposeDlg( void );
    /// Copy constructor
    KGCCoordiComposeDlg( const KGCCoordiComposeDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiComposeDlg& operator=( const KGCCoordiComposeDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_STATE_NONE = 0,
        EM_STATE_DESIGN_SLOT = 1,
        EM_STATE_ABILITY_SLOT = 2,
    };

    enum
    {
        EM_INVEN_COLUMN = 4,
        EM_INVEN_ROW = 6,
        EM_INVEN_WIDH_SIZE = 64,
        EM_INVEN_HEIGHT_SIZE = 64,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

public:
    void CreateInvenSlot();
    void InitUIState();

    void UpdateDesignSlotCoordiList();
    void UpdateAbilitySlotCoordiList();
    void UpdateInvenItemList();
    void UpdateItemCount();

    void CheckItemInformation();
    void DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos);

public:
    void OnOK();
    
    void OnEnterCursor();
    void OnLeaveCursor();
    
    void OnClose();
    void OnAddItem();

    void OnDesignSlot();
    void OnAbilitySlot();
    void OnDesignItem();
    void OnAbilityItem();

    void OnInvenItemList( int index, bool bVerifBypass = false );
    void OnScrollPos();

public:
    int             m_nState;
    GCITEMUID       m_DesignItemUID;
    GCITEMUID       m_AbilityItemUID;
    int             m_iScrollpos;


public:
    KD3DStatic*     m_pkStaticItemNum;
    
    KD3DWnd*        m_pkBtnOk;
    KD3DWnd*        m_pkBtnClose;
    KD3DWnd*        m_pkBtnAdd;
    KD3DWnd*        m_pkBtnDesignSlot;
    KD3DWnd*        m_pkBtnAbilitySlot;
    
    KD3DWnd*        m_pkBtnDesignSlotItem;
    KD3DWnd*        m_pkBtnAbilitySlotItem;
    KD3DWnd*        m_pkBtnResultSlot;

    KGCItemImgWnd*  m_pkDesignItemImg;
    KGCItemImgWnd*  m_pkAbilityItemImg;
    KGCItemImgWnd*  m_pkResultItemImg;
    
    KD3DWnd*        m_pkComposeImg;
    KD3DWnd*        m_pkDescImg;

    std::vector<KGCCommonItemWnd*> m_vecInvenItemImgList;
    std::vector<GCITEMUID>      m_vecItemUIDList;

    KD3DScrollbar* m_pkScrollbar;

};

DEFINE_WND_FACTORY( KGCCoordiComposeDlg );
DECLARE_WND_FACTORY( KGCCoordiComposeDlg );
DECLARE_WND_FACTORY_NAME( KGCCoordiComposeDlg );
