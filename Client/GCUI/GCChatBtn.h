#ifndef _KGCCHATBTN_H_
#define _KGCCHATBTN_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCChatBtn : public KD3DWnd,
				   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCChatBtn );
	/// Default constructor
	KGCChatBtn( void );
	/// Default destructor
	virtual ~KGCChatBtn( void );
	/// Copy constructor
	KGCChatBtn( const KGCChatBtn& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCChatBtn& operator=( const KGCChatBtn& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	void SetNickName( std::wstring& strNickName );
	void SetAlert( bool bAlert ){ m_pkAlert->ToggleRender( bAlert ); }
    void SetUID( DWORD dwUID ) { m_dwUID = dwUID; }
    DWORD GetUID() const { return m_dwUID; }

protected:
	virtual void OnCreate( void );

private:
	KD3DStatic* m_pkNickName;
	KD3DWnd*	m_pkAlert;
    DWORD       m_dwUID;
};

DEFINE_WND_FACTORY( KGCChatBtn );
DECLARE_WND_FACTORY( KGCChatBtn );
DECLARE_WND_FACTORY_NAME( KGCChatBtn );

#endif //_KGCCHATBTN_H_
