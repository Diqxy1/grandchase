#pragma once

class KGCEnchantSubBreakupPanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEnchantSubBreakupPanel );
    /// Default constructor
    KGCEnchantSubBreakupPanel( void );
    /// Default destructor
    virtual ~KGCEnchantSubBreakupPanel( void );
    /// Copy constructor
    KGCEnchantSubBreakupPanel( const KGCEnchantSubBreakupPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEnchantSubBreakupPanel& operator=( const KGCEnchantSubBreakupPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_STATE_NONE = 0,
        EM_STATE_EQUIP_SLOT = 1,
        EM_STATE_ASSIST_SLOT = 3,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnBreakUP();

    void OnEquipSlot();
    void OnAssistSlot();

    void OnEquipItemImg();
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
    GCITEMUID GetEquipItemUID() { return m_EquipitemUID; }
    GCITEMUID GetAssistItemUID() { return m_AssistitemUID; }

public:
    GCITEMUID m_EquipitemUID;
    GCITEMUID m_AssistitemUID;

    int       m_nState;

public:
    KD3DWnd*        m_pkBtnBreakUP;

    KD3DWnd*        m_pkBtnEquipSlot;
    KD3DWnd*        m_pkBtnAssistSlot;

    KD3DWnd*        m_pkSelEquipSlot;
    KD3DWnd*        m_pkSelAssistSlot;

    KGCCommonItemWnd*   m_pkEquipItemImg;
    KGCCommonItemWnd*   m_pkAssistItemImg;

};

DEFINE_WND_FACTORY( KGCEnchantSubBreakupPanel );
DECLARE_WND_FACTORY( KGCEnchantSubBreakupPanel );
DECLARE_WND_FACTORY_NAME( KGCEnchantSubBreakupPanel );
