#pragma once

class KGCEnchantSubEnchantPanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEnchantSubEnchantPanel );
    /// Default constructor
    KGCEnchantSubEnchantPanel( void );
    /// Default destructor
    virtual ~KGCEnchantSubEnchantPanel( void );
    /// Copy constructor
    KGCEnchantSubEnchantPanel( const KGCEnchantSubEnchantPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEnchantSubEnchantPanel& operator=( const KGCEnchantSubEnchantPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_STATE_NONE = 0,
        EM_STATE_EQUIP_SLOT = 1,
        EM_STATE_ENCHANT_SLOT = 2,
        EM_STATE_ASSIST_SLOT = 3,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnEnchant();
    void OnEquip();
    
    void OnEquipSlot();
    void OnEnchantSlot();
    void OnAssistSlot();

    void OnEquipItemImg();
    void OnEnchantItemImg();
    void OnAssistItemImg();

    void OnInvenItemList( GCITEMUID _itemUID );

public:
    void InitUIState();
    void UpdateUIState();
    
    void LockUI( bool bLock );
    void SetResultEquip();
    void SetResult();

    bool CheckItemInformation();
    void DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos);

public:
    GCITEMUID GetEquipItemUID() { return m_EquipitemUID; }
    GCITEMUID GetEnchantItemUID() { return m_EnchantitemUID; }
    GCITEMUID GetAssistItemUID() { return m_AssistitemUID; }

public:
    GCITEMUID m_EquipitemUID;
    GCITEMUID m_EnchantitemUID;
    GCITEMUID m_AssistitemUID;

    int       m_nState;

public:
    KD3DWnd*        m_pkBtnEnchant;
    KD3DWnd*        m_pkBtnEquip;

    KD3DWnd*        m_pkBtnEquipSlot;
    KD3DWnd*        m_pkBtnEnchatSlot;
    KD3DWnd*        m_pkBtnAssistSlot;

    KD3DWnd*        m_pkSelEquipSlot;
    KD3DWnd*        m_pkSelEnchatSlot;
    KD3DWnd*        m_pkSelAssistSlot;


    KGCCommonItemWnd*   m_pkEquipItemImg;
    KGCCommonItemWnd*   m_pkEnchantItemImg;
    KGCCommonItemWnd*   m_pkAssistItemImg;
   
};

DEFINE_WND_FACTORY( KGCEnchantSubEnchantPanel );
DECLARE_WND_FACTORY( KGCEnchantSubEnchantPanel );
DECLARE_WND_FACTORY_NAME( KGCEnchantSubEnchantPanel );
