#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"
//#include "../uifx/ActionListener.h"
//#include <UserPacket.h>

class KD3DStatic;
class KGCMultiMenu;

class KGCSquarePickUserList : public KD3DWnd,
	                          public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSquarePickUserList );
	/// Default constructor
	KGCSquarePickUserList( void );
	/// Default destructor
	virtual ~KGCSquarePickUserList( void );
	/// Copy constructor
	KGCSquarePickUserList( const KGCSquarePickUserList& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSquarePickUserList& operator=( const KGCSquarePickUserList& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void ListHeightChange( void );
    float GetListHeight( void ) const;
    float GetListWidth( void ) const;
    void AddPeople( std::wstring strName, DWORD dwUID_, DWORD dwAlign_ );
    void DeletePeople( void );

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

	void SetMenu( DWORD dwUID );

private:
    KD3DSizingBox*        m_pkBG;
    KGCMultiMenu*   m_pkMultiMenu;

	DWORD			m_dwPickedUserUID;
    std::wstring	m_strNickName;              // 캐릭터 닉네임
    std::wstring    m_strAccountNickName;       // 계정 닉네임
	std::map<int,DWORD>	m_mapUserUID;
};

DEFINE_WND_FACTORY( KGCSquarePickUserList );
DECLARE_WND_FACTORY( KGCSquarePickUserList );
DECLARE_WND_FACTORY_NAME( KGCSquarePickUserList );
