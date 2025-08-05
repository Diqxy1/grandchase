#pragma once

class KGCObjectUI;
class KGCMaterialItemInfo;

class KGCForgeItemCraftMachine : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCForgeItemCraftMachine );
    /// Default constructor

    const static int m_iMAX_ITEM_LIST_WIDTH_COUNT = 4;
    const static int m_iMAX_ITEM_LIST_HEGHIT_COUNT = 2;

    const static int m_iMAX_RESULT_ITEM_LIST_WIDTH = 3;
    const static int m_iMAX_RESULT_ITEM_LIST_HEGHIT = 1;

    const static int m_iMACHINE_BASE_POS_X = GC_SCREEN_DIV_SIZE_INT(545);
    const static int m_iMACHINE_BASE_POS_Y = GC_SCREEN_DIV_SIZE_INT(105);


    
    const static int mc_iMaxRecipeViewRealHeight = GC_SCREEN_DIV_SIZE_INT(95);
    const static int mc_iMaxRecipeViewIntervalHeight = GC_SCREEN_DIV_SIZE_INT(103);
    const static int mc_iMaxRecipeViewHeightCount = 3;
    const static int mc_iDefaultRecipeViewHeightCount = 3;

    enum EFORGE_MACHINE_STATE {
        EFMS_NORMAL = 0 ,
        EFMS_START,
        EFMS_STOP,
        EFMS_CONTINUE,
    };

    enum EGRADECRAFTTABLE
    {
        EGFB_NORMAL = 0,
        EGFB_RARE,
        EGFB_EPIC,
        EGFB_LEGEND,
        EGFB_MYSTIC,
        EGFB_MAX,
    };

    enum { 
        NUM_MAX_MATERIAL = 8,
    };

    KGCForgeItemCraftMachine( void );
    /// Default destructor
    virtual ~KGCForgeItemCraftMachine( void );
    /// Copy constructor
    KGCForgeItemCraftMachine( const KGCForgeItemCraftMachine& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCForgeItemCraftMachine& operator=( const KGCForgeItemCraftMachine& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void ToggleStateImage( int nStateType );
    

    void SetForgeState(int iState);
    void InitCraftMachine();
    void OnCreateItemList( void );
    void InitItemList( void );
    void OnClickCreationItem( const KActionEvent& event );
    void OnClickStartBtn( const KActionEvent& event );
    void OnClickStartBtnCreate( void );
    void OnClickStartBtnBreakUp( void );
    void OnCompleteItem( const KActionEvent& event );
    void OnClickStopBtn( const KActionEvent& event );
    void SetItemHeaterAndBtnSetting( );
    void ClearResultItemList( void );
    void ClearItemList( void );
    void OnClickCountBtn( const KActionEvent& event );
    void SetCreationItem(std::vector<SMaterialInfo> vInfo, GCITEMID giItemID, int iIndex);
    void ClickCountBtn( int iIncCount );
    void OnClickBreakUpItem( const KActionEvent& event );
    void SetBreakItemList( );
    void OnClickWaitBreakUpItem( const KActionEvent& event );
    void ChangeCharacter();
    void OnCreateCraftMachine( void );
    void CreateCraftMachine2DObject( std::string strFileName, float sizeX, float sizeY );
    void PostChildDraw( );
    void FrameMoveInEnabledState( );
    bool CheckBoundItem(std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>>& vecItemList);
    void BoundItemList( );
    void MoveCraftMachine( );
    void RefreshGauge( );
    bool GetItemInfo( KItem** pInven, GCItem** pItem, std::pair<GCITEMUID,KGCItemImgWnd*> iter_);

    int GetForgeCraftState( void ) { return m_iMachineState; } 

    void OnCliclItemSelect( const KActionEvent& event );
    void MakeMaterialUIDList( void );
    void SetSelectedMaterial( IN const GCITEMUID itemUID_ );

    bool GetNotCoordiItemUID( const IN GCITEMID itemID_, OUT GCITEMUID& itemUID_ );
    bool IsSelectedMaterialItem( const IN GCITEMUID itemUID_ );
    void SetUserGPText( const int iCost_ = -1 );
    void OnCreateItemBackList(void);
    void OnInitItemBackList(void);

private:
    int m_iForgeState;
    int m_iMachineState;
    int m_iMachineDelayCnt;
    float m_fMachineSpeed;

    KD3DStatic *m_pkGPTitle;
    KD3DStatic *m_pkCostTitle;
    KD3DStatic *m_pkGP;
    KD3DStatic *m_pkCost;

    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>> m_vecItemList;
    std::vector<std::pair<GCITEMUID,KGCItemImgWnd*>> m_vecResultItemList;
    std::vector<KD3DStatic*> m_vecStaticItemNumList;
    std::vector<KD3DStatic*> m_vecStaticResultItemNumList;
    KD3DStatic* m_pkStaticPreResultItemNum;
    KGCItemImgWnd* m_pPrekResultItem;

    KD3DWnd* m_pkStartBtn;
    KD3DWnd* m_pkStopBtn;

    RecipeSimpleInfo m_kCreateItemInfo;

    //KD3DSizingBox *m_pkItemBack;
    KSafeArray<KD3DSizingBox*, 5> m_pkItemBack;
    KSafeArray<KD3DWnd*,3>			m_pkState;
    KD3DSizingBox *m_pkBtnBack;
    KD3DWnd* m_pkHeater;

    KD3DWnd* m_pkCountMinBtn;
    KD3DWnd* m_pkCountMaxBtn;
    KD3DWnd* m_pkCountPreBtn;
    KD3DWnd* m_pkCountNextBtn;

    KD3DWnd* m_pkGaugeCenter;
    KD3DWnd* m_pkGaugeRight;


    int m_iCreateCount;

    KGCObjectUI* m_pkCraftMachineObject;

    bool m_bIsRotation[7];
    bool m_bIsMove[7];
    float m_fMoveORROtationValue[7];
    float m_fPosXValue[7];

    KSafeArray< KD3DWnd*, NUM_MAX_MATERIAL > m_apkMetialSelect;
    std::vector< GCITEMUID > m_vecMatieralUIDList;
    std::vector< std::pair< GCITEMUID, KD3DWnd* > > m_vecItemSelectBtnList;
    std::vector< KD3DWnd* >  m_vecBoxImg;
    int m_iMaxRecipeViewCount;

public:
    bool m_bBypassForgeVerify;
};

DEFINE_WND_FACTORY( KGCForgeItemCraftMachine );
DECLARE_WND_FACTORY( KGCForgeItemCraftMachine );
DECLARE_WND_FACTORY_NAME( KGCForgeItemCraftMachine );
