#pragma once
#include "GCForgeManager.h"

class KGCRecipeInfoBar;
struct RecipeSimpleInfo;

class KGCForgeItemCreation : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCForgeItemCreation );

    const static int mc_iMaxRecipeViewRealHeight = GC_SCREEN_DIV_SIZE_INT(370);
    const static int mc_iMaxRecipeViewIntervalHeight = GC_SCREEN_DIV_SIZE_INT(2);
    const static int mc_iDefaultRecipeViewHeightCount = 13;
    const static int mc_iMaxRecipeViewHeightCount = 13;

    /// Default constructor
    KGCForgeItemCreation( void );
    /// Default destructor
    virtual ~KGCForgeItemCreation( void );
    /// Copy constructor
    KGCForgeItemCreation( const KGCForgeItemCreation& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCForgeItemCreation& operator=( const KGCForgeItemCreation& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    enum
    {
        EM_INVEN_COLUMN = 4,
        EM_INVEN_ROW = 4,
        EM_INVEN_WIDH_SIZE = 64,
        EM_INVEN_HEIGHT_SIZE = 64,
    };
public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void FrameMoveInEnabledState( void );

    void OnSelectGradeBtnList( const KActionEvent& event );
    void SelectGradeBtn( int iIndex );

    void OnSelectSloatBtnList( const KActionEvent& event );
    void SelectSloatBtn( int iIndex );

    void OnChangePage( const KActionEvent& event );
    void OnChangeCharacter( const KActionEvent& event );

    void InitItemCreation( void );
    void InitGradeBtnList( void );
    void InitSloatBtnList( void );

    void SetMaxPage( );

    void InitSortBtnList( void );
    void OnSelectSortBtnList( const KActionEvent& event );
    void SelectSortBtn( int iIndex );
    void InitInfobarList( void );
    void SetInfobarList( );
    void SetInfobarString(GCItem* pItem, KGCRecipeInfoBar* pBar,const RecipeSimpleInfo& csInfo );
    void SetInfobarStringColor(GCItem* pItem, KGCRecipeInfoBar* pBar,const RecipeSimpleInfo& csInfo );
    void OnSelectInfobar( const KActionEvent& event );
    void SelectInfoBar( unsigned short iIndex ,GCITEMID giItemID );
    void CreateInfobarList( void );
    void OnInitInfobar( void );
    void OnBoundItem( const KActionEvent& event );
    void RefreshAllRecipe();
    void RefreshAllIndex( void );
    void RefreshBarIndex( int iIndex );
    int  GetFirstBarIndex( void );

    bool IsEnabledRecipeBar( GCITEMID itemID_ );

    KD3DSizingBox*                  m_pkBackground;
    void OnScrollPos();
    void InitScroll();
    KncStopWatch        m_kStopWatch;
private:
    
    KSafeArray<KD3DWnd*,GCForgeManager::EGFB_MAX>   m_saGradeFilterBtnList;
    KSafeArray<KD3DWnd*,GCForgeManager::EGFB_MAX>   m_saGradeFilterSelectedBtnList;

    KSafeArray<KD3DWnd*,GCForgeManager::ESFB_MAX>   m_saSloatFilterBtnList;
    KSafeArray<KD3DWnd*,GCForgeManager::ESFB_MAX>   m_saSloatFilterSelectedBtnList;

    KSafeArray<KD3DWnd*,GCForgeManager::ERSB_MAX>   m_saDownSortList;
    KSafeArray<KD3DWnd*,GCForgeManager::ERSB_MAX>   m_saUpSortList;
    KSafeArray<KD3DWnd*,GCForgeManager::ERSB_MAX>   m_saSortBtnList;

    std::vector< KGCRecipeInfoBar* >                m_vecRecipeInfoBar;
    int                                             m_iMaxRecipeViewCount;

    int                                             m_iSelectIndex;
    KD3DScrollbar*  m_pkScrollbar;
    int m_iScrollpos;
    bool m_bCheckBox;

};

DEFINE_WND_FACTORY( KGCForgeItemCreation );
DECLARE_WND_FACTORY( KGCForgeItemCreation );
DECLARE_WND_FACTORY_NAME( KGCForgeItemCreation );
