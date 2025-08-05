#pragma once

class KGCItemImgWnd;
class KGCCharacterUI;
class KGCDescBox;
class KLuaManager;

class KGCCouponNPCEventWnd :	public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCouponNPCEventWnd );
	/// Default constructor
	KGCCouponNPCEventWnd( void );
	/// Default destructor
	virtual ~KGCCouponNPCEventWnd( void );
	/// Copy constructor
	KGCCouponNPCEventWnd( const KGCCouponNPCEventWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCouponNPCEventWnd& operator=( const KGCCouponNPCEventWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

protected:
	KD3DSizingBox*	m_pkBackground;
	KD3DSizingBox*	m_pkBackgroundHover;
	KD3DSizingBox*	m_pkBackgroundSelect;

	KD3DSizingBox*	m_pkFrameItem;
	KD3DSizingBox*	m_pkFrameItemHover;
	KD3DSizingBox*	m_pkFrameItemSelect;

	KD3DSizingBox*	m_pkFrameTitle;
	KD3DSizingBox*	m_pkFrameTitleHover;
	KD3DSizingBox*	m_pkFrameTitleSelect;

	KD3DSizingBox*	m_pkFrameContents;
	KD3DSizingBox*	m_pkFrameContentsHover;
	KD3DSizingBox*	m_pkFrameContentsSelect;

	KD3DStatic*		m_pkTitleText;	
	KD3DStatic*		m_pkContentsText;		
	KD3DWnd*		m_pkNewEvent;
	KD3DWnd*		m_pkEndEvent;	

	bool			m_bActive;
	bool			m_bEmpty;

	GCDeviceTexture* m_pTextureItem;

public:
	KD3DWnd*		m_pkBtnWebLink;
	bool			m_bSelectedWnd;
	std::string		m_strWebAddress;

public:
	void SetText( std::wstring& strEventTitle_,  std::wstring& strEventContents_);
	void SetEventTexture( int iEventUID_ );
	void SetActiveWnd( bool bActive_ );
	bool GetActiveWnd() { return m_bActive; }	
	void RenderHoverWnd(bool bRender_);
	void RenderSelectWnd(bool bRender_);	
	void RenderEventState(bool bRender_);
	void SetEmptyWnd( bool bRender_);
	bool IsEmptyEvent();
    void SetWebButtonOff();
};


DEFINE_WND_FACTORY( KGCCouponNPCEventWnd );
DECLARE_WND_FACTORY( KGCCouponNPCEventWnd );
DECLARE_WND_FACTORY_NAME( KGCCouponNPCEventWnd );