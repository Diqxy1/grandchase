#pragma once
#include "GCHeroDungeonManager.h"

class KD3DStatic;
class KGCCommonEditBox;
class KGCHeroItemBox;
class KGCHeroItemPreview;

class KGCHeroDungeonReward : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonReward );
    /// Default constructor
    KGCHeroDungeonReward( void );
    /// Default destructor
    virtual ~KGCHeroDungeonReward( void );
    /// Copy constructor
    KGCHeroDungeonReward( const KGCHeroDungeonReward& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonReward& operator=( const KGCHeroDungeonReward& )
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
    std::map<DWORD, std::vector<KHeroItemInfo> > m_mapItemInfo;     // 영웅 아이템 정보 < 캐릭터 타입, 아이템 정보 >
    void SetHeroItemInfo( void );

public:
    bool IsRenderItemPreview(){ return m_bShowPreview; }
    void ReleaseLock( void );
    void SetGameMode( EGCGameMode _emGameMode ) { m_emGameMode = _emGameMode; }
    void UpdateCharacterInfo();
    void UpdateRewardButton();
    int GetItemCount( DWORD _dwChar );

    // 아이템 박스 관련
private:
    enum { NUM_HERO_ITEM_BOX = 6 };
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
    KD3DStatic* m_pkStaticDesc;

    int     m_iCurrentPage;
    int     m_iTotalPage;

    EGCGameMode m_emGameMode;

    void OnPrevPage( void );
    void OnNextPage( void );
    void ResetPage(int iVecItemInfoSize);
};

DEFINE_WND_FACTORY( KGCHeroDungeonReward );
DECLARE_WND_FACTORY( KGCHeroDungeonReward );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonReward );
