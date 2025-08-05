#pragma once

class KGCEnchantSubAlchemyPanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEnchantSubAlchemyPanel );
    /// Default constructor
    KGCEnchantSubAlchemyPanel( void );
    /// Default destructor
    virtual ~KGCEnchantSubAlchemyPanel( void );
    /// Copy constructor
    KGCEnchantSubAlchemyPanel( const KGCEnchantSubAlchemyPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEnchantSubAlchemyPanel& operator=( const KGCEnchantSubAlchemyPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_STATE_NONE = 0,
        EM_STATE_EQUIP_SLOT1 = 1,
        EM_STATE_EQUIP_SLOT2 = 2,
        EM_STATE_ASSIST_SLOT = 3,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnAlchemy();

    void OnEquipSlot1();
    void OnEquipSlot2();
    void OnAssistSlot();

    void OnEquipItemImg1();
    void OnEquipItemImg2();
    void OnAssistItemImg();

    void OnInvenItemList( GCITEMUID _itemUID );

public:
    void InitUIState();
    void UpdateUIState();
    
    void LockUI( bool bLock );
    void SetResult();

    bool CheckItemInformation();
    void DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos);

public:
    GCITEMUID GetEquipItemUID1() { return m_EquipitemUID1; }
    GCITEMUID GetEquipItemUID2() { return m_EquipitemUID2; }
    GCITEMUID GetAssistItemUID() { return m_AssistitemUID; }
    void SetAlchemyResultItem( GCITEMUID _itemUID ) { m_ResultItemUID = _itemUID; }

public:
    bool IsEquipItem( GCITEMUID _EquipitemUID ); 

public:
    GCITEMUID m_EquipitemUID1;
    GCITEMUID m_EquipitemUID2;
    GCITEMUID m_AssistitemUID;
    GCITEMUID m_ResultItemUID;

    int       m_nState;

public:
    KD3DWnd*        m_pkBtnAlchemy;

    KD3DWnd*        m_pkBtnEquipSlot1;
    KD3DWnd*        m_pkBtnEquipSlot2;
    KD3DWnd*        m_pkBtnAssistSlot;

    KD3DWnd*        m_pkSelEquipSlot1;
    KD3DWnd*        m_pkSelEquipSlot2;
    KD3DWnd*        m_pkSelAssistSlot;


    KGCCommonItemWnd*   m_pkEquipItemImg1;
    KGCCommonItemWnd*   m_pkEquipItemImg2;
    KGCCommonItemWnd*   m_pkAssistItemImg;

};

DEFINE_WND_FACTORY( KGCEnchantSubAlchemyPanel );
DECLARE_WND_FACTORY( KGCEnchantSubAlchemyPanel );
DECLARE_WND_FACTORY_NAME( KGCEnchantSubAlchemyPanel );
