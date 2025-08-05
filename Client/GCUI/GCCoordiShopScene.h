#ifndef _GCCOORDISHOPSCENE_H_
#define _GCCOORDISHOPSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SCoordiShopNPCInfo
{
    std::vector< std::pair<int, int> >    m_vecGreetingString;  // < 스트링번호, 발동 확률 >
    std::vector< std::pair<int, int> >    m_vecRandomString;    // < 스트링번호, 발동 확률 >
    std::vector< std::pair<int, int> >    m_vecClickString;     // < 스트링번호, 발동 확률 >
};

class KGCSearchBar;
class KD3DComboBox;
class KGCCoordiItemBox;
class KGCCoordiCharView;

//Alto-Falante
class KGCBillBoardChatBoard;

class KGCCoordiShopScene : public KGCScene // extends
{
public:
    DECLARE_CLASSNAME( KGCCoordiShopScene );
    /// Default constructor
    KGCCoordiShopScene( void );
    /// Default destructor
    virtual ~KGCCoordiShopScene( void );
    /// Copy constructor
    KGCCoordiShopScene( const KGCCoordiShopScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiShopScene& operator=( const KGCCoordiShopScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void ActionPerformed( const KActionEvent& event );
    void OnInitialize( void );
    void OnLeaveScene( void );
    void OnEnterScene( void );
    void OnCharacterChange( void );

    void OnCreate( void );
protected:
    
    void OnDestroy( void );
    void FrameMoveInEnabledState( void );
    void OnCreateComplete();

private:
    enum { MIN_NPC_MSG_WIDTH = 1, MAX_NPC_MSG_WIDTH = 25, MAX_EQUIP = 6, MAX_COORDI_BOX = 6 };
    enum ECoordiShopTab { COORDI_TAB_ARMOR = 0, COORDI_TAB_ACCESSARY, COORDI_TAB_PACKAGE, NUM_COORDI_TAB };
    enum EPageType { PAGE_FIRST = 0, PAGE_PREV, PAGE_NEXT, PAGE_END };

    std::vector< KCoordiShopItem >              m_vecHotCoordiItem;
    KGCCoordiItemBox*                           m_pkHotCoordiItemBox;
    std::vector< std::string >                  m_vecCatalogTexture;
    bool                                        m_bEquipHotCoordi;
    DWORD                                       m_dwCurHotCoordiID;

    KGCSearchBar*   m_pkSearchBar;
    KD3DComboBox*   m_pkItemCombo;
    KD3DComboBox*   m_pkSortCombo;
    KSafeArray<KGCCoordiItemBox*, MAX_COORDI_BOX>   m_apkCoordiItemBox;

    ECoordiShopTab      m_eCurrentTab;
    KD3DWnd*            m_pkBtnArmorDefault;
    KD3DWnd*            m_pkBtnAccessaryDefault;
    KD3DWnd*            m_pkBtnPackageDefault;
    KD3DWnd*            m_pkBtnArmorActive;
    KD3DWnd*            m_pkBtnAccessaryActive;
    KD3DWnd*            m_pkBtnPackageActive;
    std::vector< GCItem* > m_vecEquipItem;

    KGCCoordiCharView*  m_pkCoordiView;
    KD3DWnd*            m_pkBtnCatalog;
    
    int                 m_iTotalPage;
    int                 m_iCurrentPage;
    int                 m_iTotalHotItem;
    int                 m_iCurrentHotItem;
    KD3DStatic*         m_pkStaticCurrentPage;
    KD3DWnd*            m_pkBtnPrevPage;
    KD3DWnd*            m_pkBtnNextPage;
    KD3DWnd*            m_pkBtnFirstPage;
    KD3DWnd*            m_pkBtnEndPage;
    KD3DWnd*            m_pkBtnHotCoordiPrev;
    KD3DWnd*            m_pkBtnHotCoordiNext;
    KD3DWnd*            m_pkBtnViewPackageList;

    int                             m_iNPCIndex;
    bool                            m_bNowEnter;
    bool                            m_bUserItemClick;
    std::vector<SCoordiShopNPCInfo> m_vecNPCInfo;
    KD3DStatic*                     m_pkStaticNPCSay;

private:
    void OnFashionCatalog( void );
    void ExitShop( void );
    void SetComboList( bool bResetCategory, bool bResetSortType );
    void ChangeSortType(int iCategory, int iSortType, const WCHAR* strName = NULL );
    void DrawCoordiItemInfo( void );
    void EquipCoordiItem( int iIndex_ );
    bool SetEquipData( GCItem* pItem );
    bool IsEquipItem( GCItem* pItem );
    bool IsHotCoordiPackageItem( GCItem* pItem );

    void InitPage( void );
    void SetPage( EPageType ePageType_ );
    void SetHotCoordiView( EPageType ePageType_ );

    void LoadCoordiShopInfo( void );
    void LoadNPCInfo( KLuaManager& luaMgr );
    void LoadCatalogInfo( KLuaManager& luaMgr );
    void NPCFrameMove( void );

    void SetHotCoordiItemList( void );
    void EquipHotCooriItem( void );
    void ViewHotCoordiPackageList( void );

public:
    void SetActiveTab( ECoordiShopTab eTabType, bool bInit );
    ECoordiShopTab GetActiveTab( void ) { return m_eCurrentTab; }

    void FindItemFromName( const WCHAR* strName );
    void UpdateItemBox( void );
    void ClearEquipItemList( void ); 

    void SetChangeCharacter( int iChar_ );
    void EquipCooriItemBox( int iIndex_ );
    void EquipCoordiItemParts( int iIndex_ );

    //Alto-falante
    KGCBillBoardChatBoard* m_pkBillboardChatBoard;
};

DEFINE_WND_FACTORY( KGCCoordiShopScene );
DECLARE_WND_FACTORY( KGCCoordiShopScene );
DECLARE_WND_FACTORY_NAME( KGCCoordiShopScene );

#endif // _GCSHOPSCENE_H_