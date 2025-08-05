#ifndef _KGCEVENTBOX_H_
#define _KGCEVENTBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DSizingBox.h"
//#include "../UIFX/D3DCheckBox.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DEdit.h"

class KGCEventBox : public KD3DDialog,          // extends
	public KActionListener   // implements
{
protected:
	DWORD m_dwUse;
	DWORD m_dwCurrentBoxSetting;

	std::wstring m_strRecommendNickName;

public:
	DECLARE_CLASSNAME( KGCEventBox );
	KGCEventBox(void);
	~KGCEventBox(void);	
	enum EMessageBoxResult
	{
		MBR_OK,
		MBR_YES,
		MBR_NO,
		MBR_CREATED,
		MBR_DESTROYED,
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState();

	void SetUse( DWORD dwUse_ )	{ m_dwUse = dwUse_; }
	DWORD GetUse()	{ return m_dwUse; }
	void SetBox( DWORD dwUse_ );

	void SetTitle( std::wstring strTitle_ );
	void SetMessage( std::wstring strMsg_ );
	void SetRecommandMsg( std::wstring strMsg_ );
	void SetChackBoxMsg( std::wstring strMsg_ );

	void SetEventMessageBox();
	void SetEventRecommandBox();

	void OnCheck();
	void OnCancel();
	void OnConfirm();
	void OnClickEditRecommend();
	void OnConfirmRecommendNickName();

protected:
	KD3DSizingBox*  m_pkBackGround; 
	KD3DStatic*     m_pkStaticTitle;
	KD3DStatic*     m_pkStaticRecommend;
	KD3DStatic*     m_pkStaticMessage;
	KD3DStatic*		m_pkStaticNoRecommend;

	KD3DWnd*	    m_pkBtnOk;
	KD3DWnd*	    m_pkBtnCancel;
	KD3DWnd*		m_pkBackRecommend;
	KD3DWnd*		m_pkBar;
	KD3DWnd*		m_pkBar2;

	KD3DEdit*       m_pkEditNick;
	KD3DEdit*       m_pkEditRecommend;

	KD3DCheckBox*   m_pkCheckBoxNoRecommend;
};


DEFINE_WND_FACTORY( KGCEventBox );
DECLARE_WND_FACTORY( KGCEventBox );
DECLARE_WND_FACTORY_NAME( KGCEventBox );

#endif // _KGCEVENTBOX_H_