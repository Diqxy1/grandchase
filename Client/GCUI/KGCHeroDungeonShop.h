#pragma once

#include "GCHeroDungeonManager.h"

class KD3DStatic;
class KGCCommonEditBox;
class KGCHeroItemBox;
class KGCHeroItemPreview;

class KGCHeroDungeonShop :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonShop );
    /// Default constructor
    KGCHeroDungeonShop( void );
    /// Default destructor
    virtual ~KGCHeroDungeonShop( void );
    /// Copy constructor
    KGCHeroDungeonShop( const KGCHeroDungeonShop& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonShop& operator=( const KGCHeroDungeonShop& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroy( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

// 캐릭터 선택 관련
private:
    KSafeArray<KD3DWnd*, GC_CHAR_NUM>  m_apkCharacterName;

    DWORD   m_dwCurrentCharacter;
    int     m_iCurrentCharacterName;

// 아이템 정보 관련 
private:        
    void SendHeroItemCatalogReq( void );
    void SendHeroItemMaterialReq( void );
    void SetHeroItemInfo( void );

public:
    std::map<DWORD, std::vector<KHeroItemInfo> > m_mapItemInfo;     // 영웅 아이템 정보 < 캐릭터 타입, 아이템 정보 >
    bool IsRenderItemPreview(){ return m_bShowPreview; }
    void ReleaseLock( void );
    void SetMyHeroCoin( void );
    int GetItemCount( DWORD _dwChar );
    void UpdateCharacterInfo();
    void UpdateAllHeroShop(void);


// 영웅 주화, 주화 조각
private:
    KD3DWnd* m_pkHeroCoin;
    KD3DWnd* m_pkHeroVIP;
    
// 아이템 박스 관련
private:
    enum { NUM_HERO_ITEM_BOX = 8 };
    KSafeArray<KGCHeroItemBox*, NUM_HERO_ITEM_BOX>  m_apkItemBox;
    
    void SetHeroItemBox(DWORD dwCharType, std::map<DWORD, std::vector<KHeroItemInfo> >& mapItemInfo, int iCurPage);
    void DrawItemInfo( void );

// 미리보기 관련
private:
    int  m_iCurrentItemBox;
    bool m_bShowPreview;
    KGCHeroItemPreview* m_pkHeroItemPreview;
    void ShowItemPreview();

// 전체 페이지 관리 관련
private:
    KD3DWnd*    m_pkBtnPrevPage;
    KD3DWnd*    m_pkBtnNextPage;
    KD3DStatic* m_pkStaticPage;
    KD3DStatic* m_pkStaticHeroCoin;

    int     m_iCurrentPage;
    int     m_iTotalPage;

    void OnPrevPage( void );
    void OnNextPage( void );
    void ResetPage(int iVecItemInfoSize);
};

DEFINE_WND_FACTORY( KGCHeroDungeonShop );
DECLARE_WND_FACTORY( KGCHeroDungeonShop );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonShop );