#pragma once

class KGCCoordiItemInformationBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCoordiItemInformationBox );
    /// Default constructor
    KGCCoordiItemInformationBox( void );
    /// Default destructor
    virtual ~KGCCoordiItemInformationBox( void );
    /// Copy constructor
    KGCCoordiItemInformationBox( const KGCCoordiItemInformationBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiItemInformationBox& operator=( const KGCCoordiItemInformationBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void OncreateComplete( void );
    virtual void PostChildDraw();

private:
    enum EITEMINFOGROUP
    {
        EIIG_NAME = 0,
        EIIG_DEFAULT,
        EIIG_SET,
        EIIG_DESC,
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
    KD3DListBox* m_pkSetItem; //
    KD3DStatic* m_pkDesc;


    GCItem*                     m_pkItem;

    char                        m_cGrade;
    GCITEMID                    m_ItemID;
    GCITEMUID                   m_ItemUID;
    GCITEMUID                   m_ItemBoxUID;
    int                         m_iCoordiSeasonNum;
    
    std::vector< KAttributeInfo >   m_vecAttribute;  
    
    std::wstring                m_strItemName;
    int							m_iChar;
    int                         m_iAlpha;
    int                         m_iLevel;
    int                         m_iReqEquipLevel;

private:

    int PlusPosY( KD3DStatic* pkStatic_ );
    int PlusPosY( KD3DListBox* pkStatic_ );
    void SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DStatic* psStanDardStatic_);
    void SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DStatic* psStanDardStatic1_, KD3DStatic* psStanDardStatic2_);
    
    void SetHeightGroup( EITEMINFOGROUP eItemInfoGroup_, KD3DListBox* psStanDardStatic_);
    void SetPositionGroup( EITEMINFOGROUP eItemInfoGroup_, D3DXVECTOR2 vNewPos_);    
    void ToggleRenderItemInfoGroup( EITEMINFOGROUP eItemInfoGroup_, bool bShow_ );
    
    std::wstring MakeDiffString( int iDiff );
    std::wstring MakeAtrributeInfoString( const std::vector< KAttributeInfo >& vecAttributeInfo_ );

    std::wstring MakeDefault1String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, int iLevel_ = -1 );
    std::wstring MakeDefault2String( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, int iLevel_ = -1 );
    std::wstring MakePriceString( GCItem* pItem_);
    std::wstring MakeEquipLimitString( GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, int iLevel_ = -1 );
    std::wstring MakeGradeString( GCItem* pItem_, KItem* pInventoryItem_, SUserItem* pUserItem_ );
    std::vector<std::wstring> MakeSetItemListString( GCITEMID ItemID_ );
    std::wstring MakeItemNameString();
    std::wstring MakeDescString( GCItem* pItem_, KItem* pInven_, char cPetPromotion_ );

public:
    void SetBoxState();
    void SetUIPosition();
    void SetItemInformation( GCItem* pItem_, char cPetPromotion_ = -1, KItem* pInven_ = NULL,  SUserItem* pUserItem_ = NULL, KPetInfo* pPetInfo_ = NULL, char cItemGrade_ = -1, int iLevel_ = -1 );

    GCITEMID   GetItemID() { return m_ItemID; }
    GCITEMUID   GetItemUID() {return m_ItemUID; }
    GCITEMUID   GetItemBoxUID() {return m_ItemBoxUID; }

    GCItem* GetItem() { return m_pkItem;   }
    void SetAlpha( int iAlpha ) {   m_iAlpha = iAlpha;  };
};

DEFINE_WND_FACTORY( KGCCoordiItemInformationBox );
DECLARE_WND_FACTORY( KGCCoordiItemInformationBox );
DECLARE_WND_FACTORY_NAME( KGCCoordiItemInformationBox );
