#pragma once
#include "../uifx/D3DWnd.h"
#include "../uifx/D3DStatic.h"
#include "../uifx/D3DEdit.h"
//#include "../uifx/D3DStaticLink.h"

class KGCMultiMenu;
class KD3DImageIcon;
class KGCItemImgWnd;
class KGCGuildPublicMbox;

//#include <map>
class KGCGuildSettingChangeDlg	:	public KD3DWnd,
									public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildSettingChangeDlg );
	/// Default constructor
	KGCGuildSettingChangeDlg( void );
	/// Default destructor
	virtual ~KGCGuildSettingChangeDlg( void );
	/// Copy constructor
	KGCGuildSettingChangeDlg( const KGCGuildSettingChangeDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildSettingChangeDlg& operator=( const KGCGuildSettingChangeDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum EGuildJoinUsage
	{
		EG_JOIN_DIRECTLY	= 1,
		EG_JOIN_AFTER		= 2,
		EG_JOIN_STOP		= 3,
	};

	struct KSettingChangeItem
	{
		std::set<int> m_setNameChangeItemID;
		std::set<int> m_setMarkChangeItemID;

		void Init()
		{
			m_setNameChangeItemID.clear();
			m_setMarkChangeItemID.clear();
		}
	};
protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	virtual void PostChildDraw( void );
	//virtual void OnDestroyComplete( void );

protected:
	KD3DStatic*		m_pkGuildName;
	KD3DStatic*		m_pkStaticJoinUsage;
	KD3DStatic*		m_pkStaticGuildURL;
	KD3DStatic*		m_pkGuildNameChangeDesc;
	KD3DStatic*		m_pkGuildMarkChangeDesc;
	KD3DStatic*		m_pkGuildDestroyDesc;

	KD3DWnd*		m_pkGuildNameChangeBtn;
	KD3DWnd*		m_pkGuildNameChangeCancelBtn;

	KD3DWnd*		m_pkGuildMarkChangeBtn;
	KD3DWnd*		m_pkGuildMarkChangeCancelBtn;

	KD3DWnd*		m_pkGuildURLChangeBtn;
	KD3DWnd*		m_pkGuildURLChangeCancelBtn;

	KD3DWnd*		m_pkBtnNameChangeOK;
	KD3DWnd*		m_pkBtnURLChangeOK;

	KD3DWnd*		m_pkGuildHowToJoinInfoPopupBtn;
	KGCMultiMenu*	m_pkGuildHowToJoinInfoMultiMenu;

	KD3DWnd*		m_pkGuildDestroyBtn;
	KD3DEdit*		m_pkGuildURLEdit;

	KD3DEdit*		m_pkEditGuildName;

	KD3DImageIcon*	m_pkGuildMarkIcon;

	KSettingChangeItem m_sSettingChangeItem;

	KGCItemImgWnd*	m_pkTexNameChangeItem;
	KGCItemImgWnd*	m_pkTexMarkChangeItem;

	KGCItemInfoBox* m_pkItemInfoBox;	

	KD3DSizingBox*	m_pkMarkChangeItemBlind;
	KD3DSizingBox*	m_pkNameChangeItemBlind;

public:
	void SetInfoText( std::wstring& rText, const int index, bool bClear = true );
	void SetEditModeGuildName( bool bEditMode );
	void SetEditModeGuildURL( bool bEditMode );
	void InitGuildSettingInfo();
	void LoadSettingChangeItem();	
};

DEFINE_WND_FACTORY( KGCGuildSettingChangeDlg );
DECLARE_WND_FACTORY( KGCGuildSettingChangeDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildSettingChangeDlg );
