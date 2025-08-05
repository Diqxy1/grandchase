#pragma once

class KGCEnchantSubEnchantPanel;
class KGCEnchantSubBreakupPanel;
class KGCEnchantSubAlchemyPanel;
class KGCEnchantSubShopPanel;

class KGCEnchantSubS6Dlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEnchantSubS6Dlg );
    /// Default constructor
    KGCEnchantSubS6Dlg( void );
    /// Default destructor
    virtual ~KGCEnchantSubS6Dlg( void );
    /// Copy constructor
    KGCEnchantSubS6Dlg( const KGCEnchantSubS6Dlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEnchantSubS6Dlg& operator=( const KGCEnchantSubS6Dlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        EM_INVEN_COLUMN = 4,
        EM_INVEN_ROW = 4,
        EM_INVEN_WIDH_SIZE = 64,
        EM_INVEN_HEIGHT_SIZE = 64,
    };

    enum EM_STATE
    {
        EM_STATE_ENCHANT = 0,
        EM_STATE_BREAKEUP = 1,
        EM_STATE_ALCHEMY = 2,
        EM_STATE_SIZE = 3,
    };

    enum ASSIST_TYPE
    {
        AT_NOUSE    = 0,
        AT_UPGRADE  = 1, // 강화률 상승 아이템 사용.
        AT_NOBREAK  = 2, // 파괴 방지 아이템 사용.
        AT_SPECIFIC = 3, // 특정 레벨 강화 아이템 사용.
        AT_RANDOM   = 4, // 랜덤 레벨 강화 아이템 사용.
        AT_UPGRADE_NOBREAK   = 5, // ( 강화률 상승 + 파괴 방지 ) 복합 아이템 
        RT_MAX,
    };

    enum
    {
        EM_ITEM_NONE    = 0,
        EM_ITEM_HEAD    = 1,
        EM_ITEM_BODY1   = 2,
        EM_ITEM_BODY2   = 3,
        EM_ITEM_HAND    = 4,
        EM_ITEM_SHOOSE  = 5,
        EM_ITEM_MANTLE  = 6,
        EM_ITEM_WEAPON  = 7,
    };

    enum
    {
        EM_PARTICLE_LV1 = 0,
        EM_PARTICLE_LV2 = 1,
        EM_PARTICLE_LV3 = 2,
        EM_PARTICLE_SMALL = 3,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
    virtual void FrameMoveInEnabledState();
    virtual void PostChildDraw();


public:
    void OnOK();
    void OnEquip();
    void OnClose();
    void OnComboSelChange();
    void OnScrollPos();

    void OnClearInven();
    void OnEquipSlot();
    void OnEnchantSlot();
    void OnAssistSlot();    

    void OnInvenItemList( int index );

public:
    void InitUIState();
    void UpdateUIState();
    void CreateInvenSlot();

    void UpdateInvenItemList();
    void UpdateEquipSlotItemList();
    void UpdateEnchantSlotItemList();
    void UpdateAssistSlotItemList();

    void UpdateGPCost();

    void GetEquipSlotItemList( bool bSort = true );
    void GetEnchantSlotItemList();
    void GetAssistSlotItemList();

    bool DoEnchantItem();
    bool DoBreakUPItem();
    bool DoAlchemyItem();
    bool DoEquipItem();

    void LockUI( bool bLock );
    
    void SetResultEquip();
    void SetAlchemyResultItem( GCITEMUID _itemUID );
    void SetResult();

public:
    void RenderParticle( bool bSucc );
    void CheckParticleTime();

    void CheckItemInformation();
    void DrawItemInfo(KItem* pItem ,D3DXVECTOR2 vPos);


    ESLOTPOSITION GetItemPartsPosition( int iIndex_ );
    
    static bool SortByEquipItem( const GCITEMUID& left_, const GCITEMUID& right_ );
    static bool SortByEnchantItem(const GCITEMUID& left_, const GCITEMUID& right_ );

public:
    void SetMaterialInfo( KEVENT_STRENGTH_MATERIAL_INFO& kMaterialInfo_ );
    bool CheckValidScroll( GCITEMID _itemID, DWORD& dwAssistType );
    bool IsNoBreakLevelCheck( KItem* kItem_, KItem* kItemAssist_, int iType_ );
    DWORD GetStrengthMaxLevel() { return m_dwStrengthMaxLevel; }

public:
    int m_nState;
    int m_iScrollpos;

    int m_nParticleType;
    int m_nParticleTime;
    bool m_bSuccess;

    // 강화 안되는 아이템들. 아이템 리스트 만들때 여기 있는 아이템은 걸러준다. 
    std::map< std::pair<DWORD, DWORD>, std::set< GCITEMID >> m_mapStrengthMaterialInfo;
    std::set<GCITEMID>                                       m_setStrengthShopItemList; // 강화 판매 아이템 정보.
    DWORD                                                    m_dwStrengthMaxLevel; // 최대 강화 레벨.
    std::set<GCITEMID>                                       m_setNonStrengthItem; // 강화석 장착되지 않는 아이템 리스트.
    std::map<GCITEMID, std::pair<int,int>>                   m_mapProtectionInfoList; // 강화 보호 주문서 정보.
    std::map<DWORD, std::pair<int,int>>                      m_mapStrengthGPCost;       // 강화 시 소모 되는 GP정보 <Level, <무기일 때 GP, 방어구일 때 GP>>

public:
    KD3DWnd*        m_pkBtnClose;

    KD3DStatic*     m_pkStaticGP;
    KD3DStatic*     m_pkStaticGPCost;
    KD3DStatic*     m_pkStaticDesc;

    KD3DScrollbar*  m_pkScrollbar;

    KD3DComboBox*	m_pkItemGroupCombo;


    KGCEnchantSubEnchantPanel*  m_pkEnchantPanel;
    KGCEnchantSubBreakupPanel*  m_pkBreakUPPanel;
    KGCEnchantSubAlchemyPanel*  m_pkAlchemyPanel;
    KGCEnchantSubShopPanel*     m_pkShopPanel;

    std::vector<KGCCommonItemWnd*>  m_vecInvenItemImgList;
    std::vector<GCITEMUID>          m_vecItemUIDList;
};

DEFINE_WND_FACTORY( KGCEnchantSubS6Dlg );
DECLARE_WND_FACTORY( KGCEnchantSubS6Dlg );
DECLARE_WND_FACTORY_NAME( KGCEnchantSubS6Dlg );
