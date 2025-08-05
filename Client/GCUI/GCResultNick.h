#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"
//#include "../uifx/D3DStatic.h"

class KGCResultNick: public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCResultNick );
	/// Default constructor
	KGCResultNick( void );
	/// Default destructor
	virtual ~KGCResultNick( void );
	/// Copy constructor
	KGCResultNick( const KGCResultNick& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCResultNick& operator=( const KGCResultNick& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	void ActionPerformed(const KActionEvent& event ) {};
	void OnCreate();

public:
	void SetTeam( int iTeam );
	void SetNick( std::wstring& strNick );
	void Init();

protected:
	KSafeArray<KD3DSizingBox*,TEAM_NUM> m_pkColorFrame;
	KD3DStatic*		m_pkNick;
};

DEFINE_WND_FACTORY( KGCResultNick );
DECLARE_WND_FACTORY( KGCResultNick );
DECLARE_WND_FACTORY_NAME( KGCResultNick );