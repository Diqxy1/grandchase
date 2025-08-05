#pragma once
#include "GCForgeManager.h"

class KGCRecipeInfoBar;
struct RecipeSimpleInfo;

class KGCNewForgeItemCreation : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCNewForgeItemCreation );

	const static int mc_iMaxRecipeViewRealHeight = GC_SCREEN_DIV_SIZE_INT(380);
	const static int mc_iMaxRecipeViewIntervalHeight = GC_SCREEN_DIV_SIZE_INT(15);
	const static int mc_iDefaultRecipeViewHeightCount = 15;
	const static int mc_iMaxRecipeViewHeightCount = 15;

	/// Default constructor
	KGCNewForgeItemCreation( void );
	/// Default destructor
	virtual ~KGCNewForgeItemCreation( void );
	/// Copy constructor
	KGCNewForgeItemCreation( const KGCNewForgeItemCreation& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCNewForgeItemCreation& operator=( const KGCNewForgeItemCreation& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void FrameMoveInEnabledState( void );

	void OnSelectSlotBtnList( const KActionEvent& event );
	void SelectSlotBtn( int iIndex );

	void InitItemCreation( void );
	void InitSlotBtnList( void );

	void OnScrollPos( );

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

	void FilterInfoBar(const std::wstring& strFilter);

	bool IsEnabledRecipeBar( GCITEMID itemID_ );

	std::wstring GetGradeString( char cGradeID );
private:

	KSafeArray<KD3DWnd*,GCForgeManager::ESFB_MAX>   m_saSlotFilterBtnList;
	KSafeArray<KD3DWnd*,GCForgeManager::ESFB_MAX>   m_saSlotFilterSelectedBtnList;

	KSafeArray<KD3DWnd*,GCForgeManager::ERSB_MAX>   m_saDownSortList;
	KSafeArray<KD3DWnd*,GCForgeManager::ERSB_MAX>   m_saUpSortList;
	KSafeArray<KD3DWnd*,GCForgeManager::ERSB_MAX>   m_saSortBtnList;

	KD3DScrollbar*									m_pkScrollbar;

	std::vector< KGCRecipeInfoBar* >                m_vecRecipeInfoBar;
	int                                             m_iMaxRecipeViewCount;

	int                                             m_iSelectIndex;
	int												m_iScrollpos;
	std::wstring									m_strFilter;
};

DEFINE_WND_FACTORY( KGCNewForgeItemCreation );
DECLARE_WND_FACTORY( KGCNewForgeItemCreation );
DECLARE_WND_FACTORY_NAME( KGCNewForgeItemCreation );