#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <UserPacket.h>

#define SQUARE_USERINFO_ELEMENT_NUM     7

struct USERLIST_INFO 
{
    std::wstring strNick;
    std::wstring strNickColor;
    int iGradeMark;
    DWORD dwPeopleUID;
};

class KGCSquareUserList_Element;
class KD3DScrollbar;
class KD3DStatic;
class KGCMultiMenu;

class KGCSquareUserList : public KD3DWnd,
	                      public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSquareUserList );
	/// Default constructor
	KGCSquareUserList( void );
	/// Default destructor
	virtual ~KGCSquareUserList( void );
	/// Copy constructor
	KGCSquareUserList( const KGCSquareUserList& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSquareUserList& operator=( const KGCSquareUserList& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetUserList( int iIndex, std::wstring strUserNick, int iGradeMark, DWORD dwUID );
    void PushUserListVector( const KSquareUserInfo &userInfo );
    void PopUserListVector( DWORD dwPeopleUID );
    void PrintUserList();
    void ClearUserListVector();
    bool GetRenderUserList() const { return m_bRenderUserList; }
    void SetRenderUserList( bool bRenderUserList );
    bool IsLocalPlayer( DWORD dwUID ) const;
    bool IsLClickUserList( void ) const { return m_bIsLClickUserList; }
    void SetLClickUserlist( bool isClick );
    void SetPopupBoxRender( bool isRender );
    bool CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos);
    std::wstring GetClickedUserName( void ) const { return m_strClickedUserName; }

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

	void SetMenu( DWORD dwUID );

private:
    KSafeArray<KGCSquareUserList_Element*,SQUARE_USERINFO_ELEMENT_NUM> m_pkElement;
    KD3DScrollbar*      m_pkScrollbar;
    KD3DWnd*            m_pkScrollBG;
    KD3DWnd*            m_pkCloseBtn;
    KD3DWnd*            m_pkBackGround;
    KGCMultiMenu*       m_pkPopupBox;
    KD3DStatic*         m_pkBoxName;
    int                 m_iTopIndex;
    bool                m_bRenderUserList;
    bool                m_bIsLClickUserList;
    std::vector<USERLIST_INFO> m_vecUserInfo;
    std::wstring        m_strClickedUserName;
    std::wstring        m_strClickedUserAccountName;
	std::wstring		m_strNickName;
    DWORD               m_dwSelectedUID;
};

DEFINE_WND_FACTORY( KGCSquareUserList );
DECLARE_WND_FACTORY( KGCSquareUserList );
DECLARE_WND_FACTORY_NAME( KGCSquareUserList );