#pragma once

//class KGCItemImgWnd;
//class KGCSetItemImgWnd;

class KGCPresentCashItemWnd : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPresentCashItemWnd );
    /// Default constructor
    KGCPresentCashItemWnd( void );
    /// Default destructor
    virtual ~KGCPresentCashItemWnd( void );
    /// Copy constructor
    KGCPresentCashItemWnd( const KGCPresentCashItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPresentCashItemWnd& operator=( const KGCPresentCashItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		EDIT_LIMIT_LINE = 11,	// 입력 줄 수 제한
		EDIT_LIMIT_TEXT = 128,	// 글자 수 제한 (128이 edit최대)
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	void OnClose( void );

	void SetGoodsInfo( void );
	void OnClickFriendListBtn( void );
	
	void OnClickCashBtn( void );
	void OnClickVpBtn( void );
	void OnClickEdit( void );
	void OnClickCheckNickBtn( void );

	void SetNickname( std::wstring& wsNickName );
	bool CheckReadyToBuy( void );
	void SetCheckedNickname( bool bCheckedNickname );
	std::wstring GetItemName( void );

	inline bool IsPresent( void )
	{
		return m_bPresent;
	}
	inline std::wstring GetOwnerNickname( void )
	{
		return m_pkEditReceiver->GetText();
	}
	std::wstring GetComment( void );

private:
	KD3DWnd*            m_pFriendListBtn;
	KD3DWnd*            m_pCloseBtn;
	KD3DWnd*            m_pCashBtn;
	KD3DWnd*            m_pVpBtn;
	KD3DWnd*            m_pCheckNickBtn;

	KD3DStatic*         m_pkStaticItemName;
	KD3DStatic*			m_pkStaticTitlePrice;
	KD3DStatic*			m_pkStaticTitleMyCash;
	KD3DStatic*			m_pkStaticTitleMyVp;
	KD3DStatic*			m_pkStaticPrice;
	KD3DStatic*			m_pkStaticMyCash;
	KD3DStatic*			m_pkStaticMyVp;
	KD3DStatic*			m_pkStaticLength;
	KD3DStatic*			m_pkStaticTip;

	KD3DEdit*           m_pkEditReceiver;
	KD3DEdit*           m_pkEditMsg;

	KGCItemImgWnd*      m_pkItemImgWnd;
	KGCSetItemImgWnd*   m_pkSetItemImgWnd;

	GCItem              m_kItem;

	bool				m_bPresent;

	bool				m_bCheckedNickname;
	WCHAR				m_wcCheckNicknameTemp[256];

};

DEFINE_WND_FACTORY( KGCPresentCashItemWnd );
DECLARE_WND_FACTORY( KGCPresentCashItemWnd );
DECLARE_WND_FACTORY_NAME( KGCPresentCashItemWnd );
