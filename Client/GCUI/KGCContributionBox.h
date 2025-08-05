#ifndef _GCCONTRIBUTIONBOX_H
#define _GCCONTRIBUTIONBOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DEdit;

class KGCContributionBox : public KD3DDialog,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCContributionBox );
	/// Default constructor
	KGCContributionBox( void );
	/// Default destructor
	virtual ~KGCContributionBox( void );
	/// Copy constructor
	KGCContributionBox( const KGCContributionBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCContributionBox& operator=( const KGCContributionBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum EContributionBoxResult
	{
		CBR_OK,
		CBR_CANCEL,
		CBR_CREATED,
		CBR_DESTROYED,
	};

public:
	void	OnClickOKBtn();
	void	OnExit();

	virtual void ActionPerformed( const KActionEvent& event );

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	//virtual void OnDestroyComplete( void );
	//virtual void FrameMoveInEnabledState( void );
	//virtual void PostChildDraw( void );

    bool IsNumberValue( IN std::wstring strInput );


protected:
	KD3DWnd*    m_pkOK;
	KD3DWnd*    m_pkCancel;
	KD3DEdit*   m_pkInputGP;
	KD3DStatic* m_pkMyCount;
	KGCItemImgWnd*		m_pkItemView;

    int m_iInputCount;

};

DEFINE_WND_FACTORY( KGCContributionBox );
DECLARE_WND_FACTORY( KGCContributionBox );
DECLARE_WND_FACTORY_NAME( KGCContributionBox );

#endif // _GCCONTRIBUTIONBOX_H_