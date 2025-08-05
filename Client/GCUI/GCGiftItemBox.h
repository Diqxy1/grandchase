#ifndef _GCGiftItemBox_H_
#define _GCGiftItemBox_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCGiftItemBox : public KD3DDialog,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCGiftItemBox );
	KGCGiftItemBox(void);
	~KGCGiftItemBox(void);	

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete( void );

	void SetTitle( std::wstring strTitle_ );
	void SetContent( std::wstring strMsg_ );
	void SetContentAutoMultiLine( std::wstring strMsg_ );
    void SetMsgbox(int iItemID, std::wstring strMsg_, std::wstring strMsg2_);
	int  GetItemID() const { return m_iItemID; };

    void PostChildDraw( void );
	void OnConfirm();
    void OnDestroy();

protected:

	KD3DStatic*             m_pkTitle;
	KD3DStatic*             m_pkContent;
	KD3DStatic*				m_pkComment;

	KD3DWnd*	            m_pkBtnOk;
    KD3DWnd*	            m_pkPresent;

    int                     m_iItemID;
    GCDeviceTexture*        m_pTexItem;
	KD3DStatic*				m_pkCount;
};


DEFINE_WND_FACTORY( KGCGiftItemBox );
DECLARE_WND_FACTORY( KGCGiftItemBox );
DECLARE_WND_FACTORY_NAME( KGCGiftItemBox );

#endif // _GCGiftItemBox_H_
