#ifndef _GCSUBSCRIPTIONBOX_H
#define _GCSUBSCRIPTIONBOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../uifx/ActionListener.h"
#include "GCUI/GCItemImgWnd.h"
#include "GCItemInfoBox.h"

class KD3DEdit;

class KGCSubscriptionBox : public KD3DDialog,   
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCSubscriptionBox );
	/// Default constructor
	KGCSubscriptionBox( void );
	/// Default destructor
	virtual ~KGCSubscriptionBox( void );
	/// Copy constructor
	KGCSubscriptionBox( const KGCSubscriptionBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSubscriptionBox& operator=( const KGCSubscriptionBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void	OnClickOKBtn();
	void	OnExit();

	virtual void ActionPerformed( const KActionEvent& event );

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

protected:
    enum {LIMIT_INPUT_SIZE = 3};
	KD3DWnd*    m_pkOK;
	KD3DWnd*    m_pkCancel;
	KD3DEdit*   m_pkInputQuantity;

	KGCItemImgWnd*		m_pkItemView;
	KGCItemInfoBox*		m_pkItemInfoBox;

	int		m_iEventUID;
	int		m_iHoldingAmount;
	int		m_iPossibleAmount;
	int		m_iInput;
	int		m_iNeedAmount;

public:
	void SetSubscriptionInfo(int iEventUID_, int iHoldingAmount_, DWORD dwTicketID_, int iNeedAmount_, int iCurAmount_ );
	bool IsNumberValue( std::wstring strAmount_ );
	void SetHoldingAmount( int iHoldingAmount_ );
	void StopAll( bool bStop_ );
};

DEFINE_WND_FACTORY( KGCSubscriptionBox );
DECLARE_WND_FACTORY( KGCSubscriptionBox );
DECLARE_WND_FACTORY_NAME( KGCSubscriptionBox );

#endif // _GCSUBSCRIPTIONBOX_H
