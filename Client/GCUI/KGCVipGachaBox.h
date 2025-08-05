#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../UIFX/D3DWnd.h"
#include "../UIFX/D3DFrameWnd.h"
#include "../uifx/ActionListener.h"

#define VIP_ITEM_MAX 12
class KD3DStatic;
class KGCCharWnd;
class KGCItemInfoBox;
class KGCVipGachaItemBox;
class KGCVipGachaBox : public KD3DDialog,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCVipGachaBox );
	/// Default constructor
	KGCVipGachaBox( void );
	/// Default destructor
	virtual ~KGCVipGachaBox( void );
	/// Copy constructor
	KGCVipGachaBox( const KGCVipGachaBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCVipGachaBox& operator=( const KGCVipGachaBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void	OnExit();
    void    SetItemList( KEVENT_VIP_ITEM_LIST_NOT kRecv );
    void    UpdateBadgeNum( void );
    void    BuyItemProcedure( int index );
    int     GetCurCharType( void ) { return m_iCharType; }
    void    Dismount( void );

	virtual void ActionPerformed( const KActionEvent& event );

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );
    void NextPage( void );
    void PrevPage( void );
    void RefreshPage( void );
    void ToggleItemInfoBox( void );

protected:
 	KD3DWnd*    m_pkClose;
    KD3DWnd*    m_pkNextBtn;
    KD3DWnd*    m_pkPrevBtn;
    KD3DWnd*    m_pkBack;
    KGCCharWnd* m_pkCharWnd;
 	KD3DStatic* m_pkVipBadgeNumber;
    KD3DStatic* m_pkVipBadgeDesc;
    KD3DStatic* m_pkPage;
    KGCItemInfoBox* m_pkItemInfoBox;
    KGCVipGachaItemBox* m_pkItem[VIP_ITEM_MAX];
    KEVENT_VIP_ITEM_LIST_NOT    m_ItemList;

    int         m_iCharType;
    int         m_iCurPage;
    int         m_iTotalPage;
};

DEFINE_WND_FACTORY( KGCVipGachaBox );
DECLARE_WND_FACTORY( KGCVipGachaBox );
DECLARE_WND_FACTORY_NAME( KGCVipGachaBox );

