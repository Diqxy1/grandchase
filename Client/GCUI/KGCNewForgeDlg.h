#pragma once

#include "KGCNewForgeItemCreation.h"
#include "KGCNewForgeItemBreakUp.h"
#include "KGCNewForgeItemCraft.h"

class KGCSearchBar;

enum class EKGCNEWFORGE_STATE
{
	EKGCFOGE_CREATE = 0,
	EKGCFOGE_BREAKUP,
};

class KGCNewForgeDlg :
	public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCNewForgeDlg );
	/// Default constructor
	KGCNewForgeDlg( void );
	/// Default destructor
	virtual ~KGCNewForgeDlg( void );
	/// Copy constructor
	KGCNewForgeDlg( const KGCNewForgeDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCNewForgeDlg& operator=( const KGCNewForgeDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

public:
	void OnClickCreateBtn();
	void OnClickBrakeUpBtn();

	void OnClickCloseBtn();

	void ChangeState(EKGCNEWFORGE_STATE iState);

	bool IsEnabledRecipeBar( GCITEMID itemID );
	int  GetForgeCraftState( void );

	void SetSelectedMaterial( IN const GCITEMUID itemUID_ );

	void UpdateRecipeUI();
	void FindItemFromName(const WCHAR* strName_);

	KGCNewForgeItemCraftMachine* m_pkCraftMachine;
	KGCNewForgeItemBreakUp* m_pkItemBreakUp;

private:
	KD3DWnd     *m_pkCreateBtn;
	KD3DWnd     *m_pkSelectedCreateBtn;

	KD3DWnd     *m_pkBreakUpBtn;
	KD3DWnd     *m_pkSelectedBreakUpBtn;

	KD3DWnd     *m_pkCloseBtn;

	bool		m_bFirstOpen;

 	KGCNewForgeItemCreation*		m_pkItemCreation;


	EKGCNEWFORGE_STATE m_eCurState;

	KGCSearchBar*	m_pkSearchBar;
	std::wstring    m_strFilter;
};

DEFINE_WND_FACTORY( KGCNewForgeDlg );
DECLARE_WND_FACTORY( KGCNewForgeDlg );
DECLARE_WND_FACTORY_NAME( KGCNewForgeDlg );