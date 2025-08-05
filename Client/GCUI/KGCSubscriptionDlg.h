#pragma once

#include "GCUI/GCItemImgWnd.h"
#include "GCItemInfoBox.h"

class KGCSubscriptionDlg :	public KD3DDialog,
							public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSubscriptionDlg );
	/// Default constructor
	KGCSubscriptionDlg( void );
	/// Default destructor
	virtual ~KGCSubscriptionDlg( void );
	/// Copy constructor
	KGCSubscriptionDlg( const KGCSubscriptionDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSubscriptionDlg& operator=( const KGCSubscriptionDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

public:
	void InitSubscriptionInfo();
	void SetCurrentAmount( int iCurAmount_ ) { m_iCurrentAmount = iCurAmount_; }
	void SetAcquiredAmount( int iAcquiredAmount_ ) { m_iAcquiredAmount = iAcquiredAmount_; }
	void SetHoldingAmount( int iHoldingAmount_ ){ m_iHoldingAmount = iHoldingAmount_;}
    int GetNeedAmount(){ return m_iNeedAmount; }

private:
	int 	m_iCurEventUID; //현재이벤트 uid
	int 	m_iAcquiredAmount; //획득수량
	int 	m_iExchangedAmount; //교환한수량
	int 	m_iHoldingAmount;	//보유수량
	int 	m_iCurrentAmount; //남은경품수량
	int		m_iNeedAmount;
	int		m_iGiftItemID;
	DWORD	m_dwTicketID;	//경품 아이템 ID

	KD3DStatic* m_pkStaticDesc1;
	KD3DStatic* m_pkStaticDesc2;


public:
	enum eBagType
	{ BLUE_BAG =9, RED_BAG=10, GOLD_BAG=11 };

	KD3DWnd* m_pkSubscriptionBtn; // 교환버튼
	KD3DWnd* m_pkCloseBnt; // 종료버튼
	
	KGCItemImgWnd* m_pkItemView;
	KGCItemInfoBox* m_pkItemInfoBox;

	KD3DWnd* m_pkBlueBagTitle;
	KD3DWnd* m_pkBlueBagDesc;
	KD3DWnd* m_pkRedBagTitle;
	KD3DWnd* m_pkRedBagDesc;
	KD3DWnd* m_pkGoldBagTitle;
	KD3DWnd* m_pkGoldBagDesc;

	eBagType	m_eBagType;
	std::map<DWORD,int>	m_mapNPCEVENTNUM;// DWORD = NPC_ID, int = EVENT_ID
	
public:
	void SetRenderEventActive( int iIndex, bool bRender );
	void OnExit();
	void OnClickExchangeBtn();
	void Update();
	void SaveEventID();
	void SetBagType( int eType );
	
};


DEFINE_WND_FACTORY( KGCSubscriptionDlg );
DECLARE_WND_FACTORY( KGCSubscriptionDlg );
DECLARE_WND_FACTORY_NAME( KGCSubscriptionDlg );