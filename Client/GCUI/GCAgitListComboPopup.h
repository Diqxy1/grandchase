#pragma once

class KGCAgitUserElement;

class KGCAgitListComboPopup : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { NUM_USER = 4 };

	enum EM_AGIT_USER_MENU
	{
		EM_AGIT_USER_MENU_INVITE_PARTY = 1,			// ��Ƽ �ʴ�
		EM_AGIT_USER_MENU_AGIT_VISIT,			// ����Ʈ �湮
		EM_AGIT_USER_MENU_ADD_FRIEND,			// ģ�� �߰�
		EM_AGIT_USER_MENU_COUPLE_PROPOSE,		// Ŀ�� �α�
		EM_AGIT_USER_MENU_BLOCK_FRIEND,			// ���� �ϱ� 
		EM_AGIT_USER_MENU_AGIT_BAN,				// ����Ʈ �߹�
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

	//	�������콺 �˾��޴� ��Ȱ�� ( ����Ʈ ����Ʈ���� ������� �����Ƿ� ) 
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
