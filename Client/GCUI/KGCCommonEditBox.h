#pragma once

class KD3DStatic;
class KD3DListBox;
class KD3DEdit;
class KD3DScrollbar;
class KGCCommonEditBox :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCommonEditBox );
    /// Default constructor
    KGCCommonEditBox( void );
    /// Default destructor
    virtual ~KGCCommonEditBox( void );
    /// Copy constructor
    KGCCommonEditBox( const KGCCommonEditBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCommonEditBox& operator=( const KGCCommonEditBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum ECommonEditBoxKind
	{
		ECEB_COMMENT		= 1,
		ECEB_NOTICE1		= 2,
		ECEB_NOTICE2		= 3,
		ECEB_MY_INTRODUCE	= 4,
	};

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DScrollbar*      m_pkScrollBar;
    KD3DWnd*            m_pkEditBtn;
    KD3DWnd*            m_pkOKBtn;
	KD3DWnd*            m_pkBtnCancel;
	KD3DWnd*            m_pkBtnClose;
    KD3DStatic*         m_pkLength;
	KD3DStatic*         m_pkStaticTitle;
    KD3DListBox*        m_pkContents;
    KD3DEdit*           m_pkContentsEdit;

	D3DCOLOR			m_dwColor;
	int					m_iLimitText;
	int					m_iEditType;
	bool				m_bUseScrollBar;

	std::wstring		m_strPreData;

public:
    void OnOK( void );
	void OnCancel( void );
	void OnClose( void );
    void OnEdit( void );
    void OnScroll( void );
    void OnEditPushEnter( void );
    void SetEditMode( bool bEdit_ );
	void ToggleRenderScrollBar( bool bRender_ );
	void SetEditBoxLimitLine( int iLimitLine_ );
	void SetEditBoxLimitText( int iLimitText_ );
	void SetTextColor( D3DCOLOR dwColor_ = D3DCOLOR_RGBA(0,0,0,255));
	void SetDefaultText( std::wstring str_ );
	void SetUseScrollBar( bool bUse_ ) { m_bUseScrollBar = bUse_; }
	void SetTitleText( std::wstring	str_, DWORD dwAlign_ = DT_LEFT,  D3DCOLOR dwCololr_ = D3DCOLOR_RGBA(0,0,0,255) );
	void SetCommonEditType( int iType_ ) { m_iEditType = iType_; }
	void SendPacket( int iPacketType_ ); 
	void RenderEditBtn( bool bRender_ );
};

DEFINE_WND_FACTORY( KGCCommonEditBox );
DECLARE_WND_FACTORY( KGCCommonEditBox );
DECLARE_WND_FACTORY_NAME( KGCCommonEditBox );