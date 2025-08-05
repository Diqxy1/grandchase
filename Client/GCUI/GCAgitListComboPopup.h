#pragma once

class KGCAgitUserElement;

class KGCAgitListComboPopup : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { NUM_USER = 4 };

	enum EM_AGIT_USER_MENU
	{
		EM_AGIT_USER_MENU_INVITE_PARTY = 1,			// 파티 초대
		EM_AGIT_USER_MENU_AGIT_VISIT,			// 아지트 방문
		EM_AGIT_USER_MENU_ADD_FRIEND,			// 친구 추가
		EM_AGIT_USER_MENU_COUPLE_PROPOSE,		// 커플 맺기
		EM_AGIT_USER_MENU_BLOCK_FRIEND,			// 차단 하기 
		EM_AGIT_USER_MENU_AGIT_BAN,				// 아지트 추방
	};

public:
    DECLARE_CLASSNAME( KGCAgitListComboPopup );
    /// Default constructor
    KGCAgitListComboPopup( void );
    /// Default destructor
    virtual ~KGCAgitListComboPopup( void );
    /// Copy constructor
    KGCAgitListComboPopup( const KGCAgitListComboPopup& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitListComboPopup& operator=( const KGCAgitListComboPopup& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    std::vector< std::pair< DWORD, std::wstring > > m_vecUserList;

    KD3DScrollbar*  m_pkScroll;
    KD3DSizingBox*  m_pkBack;
    KSafeArray< KGCAgitUserElement*, NUM_USER > m_apkUserList;
    KGCMultiMenu*       m_pkPopupBox;


	std::wstring        m_strClickedUserName;
	std::wstring		m_strNickName;
	DWORD				m_dwSelectedUID;
	bool				m_bIsLClickUserList;

	//	우측마우스 팝업메뉴 비활성 ( 아지트 리스트에서 사용하지 않으므로 ) 
	bool				m_bPopUp;	
    

    int m_iScrollSpeed;
    int m_iScrollPos;
    int m_iBeforePos;
    int m_iScrollPage;

public:
    void SetUserList( std::map< DWORD, std::wstring >& mapInfo_ );
    void Clear( void );

private:
    void SetElementList( int iStarIndex_ );


public:
	bool IsLocalPlayer( DWORD dwUID ) const;
	void SetLClickUserlist( bool isClick );
	bool CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos );
	void SetPopupBoxRender( bool isRender );
	void SetMenu( DWORD dwUID );
};

DEFINE_WND_FACTORY( KGCAgitListComboPopup );
DECLARE_WND_FACTORY( KGCAgitListComboPopup );
DECLARE_WND_FACTORY_NAME( KGCAgitListComboPopup );
