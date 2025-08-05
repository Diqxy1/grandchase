#pragma once

#include "KGCMaterialItemInfo.h"

class KGCObjectUI;
enum class EKGCNEWFORGE_STATE;

class KGCNewForgeItemCraftMachine : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCNewForgeItemCraftMachine );
	/// Default constructor

	const static int m_iMAX_ITEM_LIST_WIDTH_COUNT = 4;
	const static int m_iMAX_ITEM_LIST_HEGHIT_COUNT = 2;

	const static int m_iMAX_RESULT_ITEM_LIST_WIDTH = 3;
	const static int m_iMAX_RESULT_ITEM_LIST_HEGHIT = 1;

	const static int m_iMACHINE_BASE_POS_X = GC_SCREEN_DIV_SIZE_INT(545);
	const static int m_iMACHINE_BASE_POS_Y = GC_SCREEN_DIV_SIZE_INT(105);

	enum EFORGE_MACHINE_STATE 
	{
		EFMS_NORMAL = 0,
		EFMS_START,
		EFMS_STOP,
		EFMS_CONTINUE
	};

	enum EFORGE_MACHINE_CONSTRUCTION_STATE
	{
		EMCS_CREATING,
		EMCS_BREAKING,
		EMCS_SUCCESS,
		EMCS_MAX
	};

	enum 
	{ 
		NUM_MAX_MATERIAL = 8,
	};

	KGCNewForgeItemCraftMachine( void );
	/// Default destructor
	virtual ~KGCNewForgeItemCraftMachine( void );
	/// Copy constructor
	KGCNewForgeItemCraftMachine( const KGCNewForgeItemCraftMachine& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCNewForgeItemCraftMachine& operator=( const KGCNewForgeItemCraftMachine& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

	void SetForgeState(EKGCNEWFORGE_STATE iState);
	void InitCraftMachine();
	void OnCreateItemList( void );
	void InitItemList( void );
	void OnClickCreationItem( const KActionEvent& event );
	void OnClickStartBtn( const KActionEvent& event );
	void OnClickStartBtnCreate( void );
	void OnClickStartBtnBreakUp( void );
	void OnCompleteItem( const KActionEvent& event );
	void OnClickStopBtn( const KActionEvent& event );
	void ClearResultItemList( void );
	void ClearItemList( void );
	void OnClickCountBtn( const KActionEvent& event );
	void SetCreationItem(std::vector<SMaterialInfo> vInfo, GCITEMID giItemID, int iIndex);
	void ClickCountBtn( int iIncCount, bool isFreeType = false );
	void OnClickBreakUpItem( const KActionEvent& event );
	void SetBreakItemList( );
	void OnClickWaitBreakUpItem( const KActionEvent& event );

	void FrameMoveInEnabledState( );
	bool CheckBoundItem(std::vector<KGCMaterialItemInfo*>& vecItemList);
	void BoundItemList( );
	void MoveCraftMachine( );

	bool GetItemInfo( KItem** pInven, GCItem** pItem, KGCMaterialItemInfo* iter_);

	int GetForgeCraftState( void ) { return m_iMachineState; } 

	void OnCliclItemSelect( const KActionEvent& event );
	void MakeMaterialUIDList( void );
	void SetSelectedMaterial( IN const GCITEMUID itemUID_ );

	bool GetNotCoordiItemUID( const IN GCITEMID itemID_, OUT GCITEMUID& itemUID_ );
	bool IsSelectedMaterialItem( const IN GCITEMUID itemUID_ );
	void SetUserGPText( const int iCost_ = -1 );
	void OnTextEnter();
	void SetUseNum( int iNum );

	void InitCountBtn();

	void ShowPrizeRarity( const IN char cGradeID );

	std::vector<GCITEMUID>				m_vecMaterialUIDList;
	bool m_bBypassForgeVerify;
private:
	//Forge Money Information
	KD3DStatic *m_pkGPTitle;
	KD3DStatic *m_pkGP;
	KD3DStatic *m_pkCostTitle;
	KD3DStatic *m_pkCost;

	//Material Information
	KSafeArray< KGCMaterialItemInfo*, NUM_MAX_MATERIAL >	m_apkMaterialInfoSelect;
	std::vector<KGCMaterialItemInfo*>	m_vecMaterialItemList;

	//Forge operation
	KD3DWnd*	m_pkStartBtn;
	KD3DWnd*	m_pkStopBtn;

	//Prize information
	KD3DWnd*									m_pkCountMinBtn;
	KD3DWnd*									m_pkCountMaxBtn;
	KD3DWnd*									m_pkCountPreBtn;
	KD3DWnd*									m_pkCountNextBtn;
	KD3DWnd*									m_pkCountItemBack;
	KD3DWnd*									m_pkCountEditBG;
	KD3DEdit*									m_pkCountEdit;
	KD3DStatic*									m_pkStaticPreResultItemNum;
	KGCItemImgWnd*								m_pPrekResultItem;
	RecipeSimpleInfo							m_kCreateItemInfo;
	int											m_iCreateCount;
	KSafeArray< KD3DWnd*, KItem::GRADE_NUM >	m_apkItemGradeBack;
	
	std::vector<KGCMaterialItemInfo*>	m_vecResultItemList;


	//Forge state
	EKGCNEWFORGE_STATE					m_iForgeState;
	int									m_iMachineState;
	int									m_iMachineDelayCnt;
	KSafeArray< KD3DWnd*, EMCS_MAX >	m_apkForgeState;
};

DEFINE_WND_FACTORY( KGCNewForgeItemCraftMachine );
DECLARE_WND_FACTORY( KGCNewForgeItemCraftMachine );
DECLARE_WND_FACTORY_NAME( KGCNewForgeItemCraftMachine );
