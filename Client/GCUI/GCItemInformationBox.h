#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../GCItem.h"

class KD3DWnd;
class KD3DSizingBox;
class KD3DStatic;
class KGCSocketIcon;

struct KInventoryItem;
class KGCItemInformationBox : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemInformationBox );
    /// Default constructor
    KGCItemInformationBox( void );
    /// Default destructor
    virtual ~KGCItemInformationBox( void );
    /// Copy constructor
    KGCItemInformationBox( const KGCItemInformationBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemInformationBox& operator=( const KGCItemInformationBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void OncreateComplete( void );
    virtual void PostChildDraw();

public:
    enum EITEMCOMPAREICON
    {
        EICI_UP,
        EICI_EQUAL,
        EICI_DOWN,
        EICI_MAX,
    };

    enum EITEMINFORMATIONBOXSTATE
    {
        EIIBS_ALONE,
        EIIBS_EQUIPPED,
        EIIBS_POINTED,
        EIIBS_MAX,
    };

private:
    enum EITEMINFOGROUP
    {
        EIIG_NAME = 0,
        EIIG_DEFAULT,
        EIIG_SOCKET,
        EIIG_ATTRIBUTE,
        EIIG_SET,
        EIIG_DESC,
        EIIG_EXTRADESC,
        EIIG_MAX,
    };

    struct ItemInfoGroup
    {
        KD3DWnd*    m_pkItemInfoGroup;
        KD3DSizingBox*    m_pkBackGround;
        bool        m_bExist;
        DWORD       m_dwExtraSize;
        DWORD       m_dwExtraPlus;

        ItemInfoGroup() : m_pkItemInfoGroup( NULL ), m_bExist( true ), m_dwExtraSize(0), m_dwExtraPlus(0)
        {}
        DWORD   GetHeight() { return m_pkItemInfoGroup->GetHeight(); }
    };

    ItemInfoGroup  m_stItemInfoGroup[ EIIG_MAX ];
    KD3DSizingBox* m_pkBackground;
    KD3DSizingBox* m_pkCenterBackground;

    KD3DStatic* m_pkName;
    KD3DStatic* m_pkDefault1;
    KD3DStatic* m_pkDefault2;
    KD3DStatic* m_pkSocket; //
    KD3DStatic* m_pkAttribute; //
    KD3DListBox* m_pkSetItem; //
    KD3DStatic* m_pkDesc;
    KD3DStatic* m_pkExtraDesc;

    std::map< int, KGCSocketIcon*> m_mapSocketIcon;
    KD3DWnd* m_pkCompareIcon[ EICI_MAX ];

    GCItem*                     m_pkItem;
    std::wstring m_strEquipState[ EIIBS_MAX ];
    EITEMINFORMATIONBOXSTATE    m_eItemInformationBoxState;
    char                        m_cGrade;
    GCITEMID                    m_ItemID;
    GCITEMUID                   m_ItemUID;
    GCITEMUID                   m_ItemBoxUID;
    std::vector< KAttributeInfo >   m_vecAttribute;
    std::vector< KSocketInfo >      m_vecSocket;
    bool                        m_bShowExtraInfo;
    std::wstring                m_strShowExtraInfo;
    std::wstring                m_strHideExtraInfo;;
    std::wstring                m_strExtraDescString;
    std::vector < EITEMINFOGROUP > m_vecExtraInfoGroup;
    std::wstring                m_strItemName;
    bool                        m_bEquippedItem;
    int							m_iChar;
    int                         m_iAlpha;
    int                         m_iLevel;
    int                         m_iNeedEquipLevel;

private:
    void HideItemInfoGroup( EITEMINFOGROUP eItemInfoGroup_ );
    int PlusPosY( KD3DStatic* pkStatic_ );
    int PlusPosY( KD3DListBox* pkStatic_ );
    void SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DStatic* psStanDardStatic_);
    void SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DListBox* psStanDardStatic_);
    void SetPositionGroup( EITEMINFOGROUP eItemInfoGroup_, D3DXVECTOR2 vNewPos_);
    void ToggleRenderItemInfoGroup( EITEMINFOGROUP eItemInfoGroup_, bool bShow_ );
    std::wstring MakeDiffString( int iDiff );
    std::wstring MakeSocketItemString( int iAbility );
    std::wstring MakeSocketInfoString( const std::vector< KSocketInfo >& vecSocketInfo_ );
    std::wstring MakeAtrributeInfoString( const std::vector< KAttributeInfo >& vecAttributeInfo_, const std::set<int>& setEnchantAtt );
    std::wstring MakeDefault1String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, int iLevel_ = -1 );
    std::wstring MakeDefault2String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, char cItemGrade_ = -1, int iLevel_ = -1 );
    std::wstring MakePriceString( GCItem* pItem_);
    std::wstring MakeEquipLimitString( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, int iLevel_ = -1 );
    std::wstring MakeAbilityString( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, int iLevel_ = -1 );
    std::wstring MakeGradeString( GCItem* pItem_, KItem* pInventoryItem_, SUserItem* pUserItem_, char cItemGrade_ = -1);
    std::vector<std::wstring> MakeSetItemListString( GCITEMID ItemID_ );
    std::wstring MakeItemNameString( GCItem* pItem_, char cPetPromotion_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, char cItemGrade_ = -1 );
    std::wstring MakeDescString( GCItem* pItem_, KItem* pInven_, char cPetPromotion_ );

public:
    void SetUIPosition();
    void SetItemInformation( GCItem* pItem_, char cPetPromotion_ = -1, KItem* pInven_ = NULL,  SUserItem* pUserItem_ = NULL, KPetInfo* pPetInfo_ = NULL, char cItemGrade_ = -1, int iLevel_ = -1 );
    void SetBoxState( EITEMINFORMATIONBOXSTATE eItemInformationboxState_ );
    void SetCompareIcon( EITEMCOMPAREICON eItemCompareIcon_ );
    GCITEMID   GetItemID() { return m_ItemID; }
    GCITEMUID   GetItemUID() {return m_ItemUID; }
    GCITEMUID   GetItemBoxUID() {return m_ItemBoxUID; }

    void SetShowExtraInfo( bool bShow_ );
    bool IsExistExtraInfoGroup();
    void ShowExtraInfo( bool bShow_);
    void SetKeyText( int iStringID );
    GCItem* GetItem() { return m_pkItem;   }
    void SetAlpha( int iAlpha ) {   m_iAlpha = iAlpha;  };
};

DEFINE_WND_FACTORY( KGCItemInformationBox );
DECLARE_WND_FACTORY( KGCItemInformationBox );
DECLARE_WND_FACTORY_NAME( KGCItemInformationBox );
