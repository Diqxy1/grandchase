#pragma once

struct RecipeSimpleInfo
{
    RecipeSimpleInfo() : iItemLevel( 0 ), iPrice( 0 ), ItemID( 0 ), usIndex(0), strItemName( L""), cGrade(-1) , cFactor(0), bHasDiscount( false )
    {}
    void Clear(){
        iItemLevel = 0;
        iPrice = 0;
        pairIdIndex.first = 0;
        pairIdIndex.second = 0;
        ItemID = 0;
        usIndex = 0;
        strItemName.clear();
        cGrade = -1;
        cFactor = 0;
        bHasDiscount = false;
    }
    int             iItemLevel;
    int             iPrice;
    PAIR_USHORT_DWORD pairIdIndex;
    GCITEMID        ItemID;
    USHORT          usIndex;
    std::wstring    strItemName;
    char            cGrade;
    char            cFactor;
    bool            bHasDiscount;

	RecipeSimpleInfo(const RecipeSimpleInfo& other)
	{
		this->iItemLevel = other.iItemLevel;
		this->iPrice = other.iPrice;
		this->pairIdIndex = other.pairIdIndex;
		this->ItemID = other.ItemID;
		this->usIndex = other.usIndex;
		this->strItemName = other.strItemName;
		this->cGrade = other.cGrade;
		this->cFactor = other.cFactor;
        this->bHasDiscount = other.bHasDiscount;
	}

	RecipeSimpleInfo& operator=(const RecipeSimpleInfo& other)
	{
		this->iItemLevel = other.iItemLevel;
		this->iPrice = other.iPrice;
		this->pairIdIndex = other.pairIdIndex;
		this->usIndex = other.usIndex;
		this->ItemID = other.ItemID;
		this->strItemName = other.strItemName;
		this->cGrade = other.cGrade;
		this->cFactor = other.cFactor;
        this->bHasDiscount = other.bHasDiscount;
		return *this;
	}
};

struct SMaterialInfo {
    GCITEMID    m_ItemID;
    int         m_nDuration;
    int         m_nPeriod;
	int			m_nLevel;
    bool        m_bExist;

    SMaterialInfo()
        :m_ItemID(0)
        ,m_nDuration(-1) //KItem::UNLIMITED_ITEM
        ,m_nPeriod(-1) //KItem::UNLIMITED_ITEM
		,m_nLevel(-1)
        ,m_bExist(false)
    {}

};

class GCForgeManager
{
    DeclareSingleton(GCForgeManager);

public:
    typedef bool (*RecipeSortFunc)( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_);
    typedef std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> > MAP_FM_MATERIAL_LIST;

    enum ESLOTFILTERBTN
    {
		ESFB_ALL = 0,
        ESFB_CARD,
        ESFB_EQUIPMENT,
        ESFB_ACCESSORY,
        ESFB_ETC,
        ESFB_EVENT,
        ESFB_VIP,
        ESFB_MAX,
    };

    enum EGRADEFILTERBTN
    {
        EGFB_NORMAL = 0,
        EGFB_RARE,
        EGFB_EPIC,
        EGFB_LEGEND,
        EGFB_MYSTIC,
        EGFB_MAX,
    };

    enum ERECIPESORTBTN
    {
        ERSB_ITEMLEVEL = 0,
        ERSB_ITEMNAME,
		ERSB_ITEMGRADE,
        ERSB_PRICE,
        ERSB_MAX,
    };

    GCForgeManager(void);
    ~GCForgeManager(void);

    static bool SortByItemLevel( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ );
    static bool SortByItemName( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ );
    static bool SortByPrice( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ );
	static bool SortByGrade( const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_ );

    void SetCharacter(int iChar){m_iCurrentCharacter = iChar;}
    int GetCharacter(){return m_iCurrentCharacter;}

    void SetSlot(int iIndex){m_iCurrentSlot = iIndex;}
    int GetSlotFrag( );

    void SetResultSendMaterial(){m_Result_ManufactureS3_Material_Get = 0;}

    const std::vector<RecipeSimpleInfo>& GetShowRecipeList(){return m_vecShowRecipeList;}
    void ClearRecipeAndBreakInfo();

    const MAP_FM_MATERIAL_LIST& GetMeterialList(){return m_mapAllMeterialList;}

    void SetRecipeAllList( std::vector<KManufactureItem> &vecRecipeAllList_ );
    void SetShowRecipeList();
    void SetSort(int iSortType, bool Ascending_);
    void InitSort( );
    void AddMaterialList( MAP_FM_MATERIAL_LIST& mapAddMaterialList_ );

    void Send_GetItemCatalog();
    void Send_GetMeterials(std::set< PAIR_USHORT_DWORD >& setItemList);

    void ClearMeterialList(){m_mapAllMeterialList.clear();}

    bool GetMaterialList(int iIndex,GCITEMID giItemID, OUT std::vector<SMaterialInfo>& vInfo);
    bool GetMaterialColor( GCITEMID giItemID ,int nItemDuration, int nLevel, DWORD& imgColor, DWORD& fontcolor);
    bool GetMaterialColorByUID( GCITEMUID itemmUID_ ,int nItemDuration, int nLevel, DWORD& imgColor, DWORD& fontcolor);
    bool CreateItem( RecipeSimpleInfo &info ,int iFactor );
    bool CreateItem( RecipeSimpleInfo &info ,int iFactor, const IN vector<GCITEMUID>& vecMaterialList_ );
    void SetResultItem(std::vector< KItem >& vecItem);
    const std::vector< KItem >& GetResultItem( void ){return m_kResultItemList;}
    int GetCurrentSlot(){return m_iCurrentSlot;}
    int GetMaxCreateCount(int iIndex,GCITEMID giItemID );
    ////////////////////////////////////////////////////////////////////////////////////////위는 아이템 제작
////////////////////////////////////////////////////////////////////////////////////////아래는 아이템 해제

    void MakeAllBreakUpItemList();
    void SetShowBreakUpItemList();
    const std::vector<GCITEMUID>& GetShowBreakUpItemList(){return m_vecShowBreakUpItemList;}
    bool AddWaitItem( GCITEMUID guItemUID, GCITEMID giItemID );
    std::vector<std::pair<GCITEMUID, GCITEMID>>& GetWaitItemList(){return m_vecWaitItemList;}
    void EraseWaitItem(GCITEMUID guItemUID);
    bool BreakUpItem( GCITEMUID guItemUID );
    RecipeSimpleInfo GetCraftRecipeInfo( DWORD dwItemID );

    void SetEnableBreakUp(bool bEnable);
    bool IsEnableBreakUp( ){return m_bEnableBreakUp;}
    void InitWaitItem( );
    void Send_GetItemBreakupInfo();
    void SetItemBreakUpInfo( KEVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK& kRecv_ );
    bool IsSpecificBreakupItem( IN PAIR_INT& paitItemInfo );
    void GetSpecificBreakupItem( IN PAIR_INT& pairItemInfo, OUT std::vector< KDropItemInfo >& vecItemInfo );
    void GetBreakupItem( IN PAIR_INT& pairItemInfo, OUT std::map< GCITEMID, PAIR_INT >& mapResultItemInfo );
    bool MakeItemOnFirstSlot( IN DWORD dwItemID );
    bool MakeRecipeOnFirstBar( IN DWORD dwItemID );

    bool CheckRecipeContinentType( IN std::vector<KManufactureItem>::iterator vitRecipe );

    std::set<GCITEMUID> m_setWaitItemList;
    std::vector<std::pair<GCITEMUID, GCITEMID>> m_vecWaitItemList;
    std::set<GCITEMUID> m_setRemovedItemList;
private:
    int m_iCurrentCharacter;
    int m_iCurrentSlot;

    std::vector<KManufactureItem> m_vecRecipeAllList;
    std::vector< RecipeSimpleInfo > m_vecShowRecipeList; //디스플레이용

    RecipeSortFunc m_funcRecipeSort;
    bool           m_Ascending;

    MAP_FM_MATERIAL_LIST m_mapAllMeterialList;
    int m_Result_ManufactureS3_Material_Get;

    std::vector< KItem > m_kResultItemList;

/////////////////////////////////////////////////////////////////////////////////////////위는 아이템 제작
/////////////////////////////////////////////////////////////////////////////////////////아래는 아이템 해제
    std::vector<GCITEMUID> m_vecAllBreakUpItemList;
    std::vector<GCITEMUID> m_vecShowBreakUpItemList;

    bool m_bEnableBreakUp;

    // 2012-10-19 해체 정보도 서버에서 받음, 아이템 별로 해체 결과를 다르게 하는 SpecificBreakupInfo 추가
    float                                               m_fBreakUpMaterialRatioMin;        // 재료아이템수 연산시 최소값 (MaterialRatioMin)
    float                                               m_fBreakUpMaterialRatioMax;        // 재료아이템수 연산시 최대값 (MaterialRatioMax)
    std::map< int, GCITEMID >                           m_mapBreakUpMaterialItemID;        // 재료아이템 [Index,ItemID] (MaterialItemID)
    std::map< PAIR_INT, std::map< int, int > >          m_mapBreakUpMaterialInfo;          // [장착Lv,Grade], [재료Index,Count] (MaterialCountRatio)
    std::map< PAIR_INT, std::vector< KDropItemInfo > >  m_mapBreakUpSpecificBreakupInfo;   // [입력 아이템ID, Grade], [결과 아이템들] (SpecificBreakup)

};
DefSingletonInline(GCForgeManager);
