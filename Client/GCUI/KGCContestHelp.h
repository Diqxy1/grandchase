#ifndef _KGCCONTESTHELP_H_
#define _KGCCONTESTHELP_H_

//#include "../uifx/D3DWnd.h"
//#include "../GCLuaManager/KLuaManager.h"
//#include "../GCLuaManager/KLuabinder.h"

class KD3DStatic;

struct PAGE
{
	int m_iStrTitleID;
	int m_iStrID;
};

class KGCContestHelp : public KD3DWnd,
					   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCContestHelp );
	/// Default constructor
	KGCContestHelp( void );
	/// Default destructor
	virtual ~KGCContestHelp( void );
	/// Copy constructor
	KGCContestHelp( const KGCContestHelp& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCContestHelp& operator=( const KGCContestHelp& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnInitialize();
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void SetPage( int iPage );
	void AddPage( int iTitle, int iStr );
    void InitPage();

private:
	KD3DStatic* m_pkHelpStr;
	KD3DStatic* m_pkTitleStr;
	KD3DWnd*	m_pkLeft;
	KD3DWnd*	m_pkRight;

	std::vector<PAGE> m_vecPage;
	int m_iCurrPage;
};

DEFINE_WND_FACTORY( KGCContestHelp );
DECLARE_WND_FACTORY( KGCContestHelp );
DECLARE_WND_FACTORY_NAME( KGCContestHelp );

#endif //_KGCCONTESTHELP_H_