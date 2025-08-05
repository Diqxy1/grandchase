#ifndef _KGCSERVERLIST_H_
#define _KGCSERVERLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DScrollbar;
class KGCServerBar;
class KGCHighLight;

class KGCServerList: public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCServerList );
    /// Default constructor
    KGCServerList( void );
    /// Default destructor
    virtual ~KGCServerList( void );
    /// Copy constructor
    KGCServerList( const KGCServerList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCServerList& operator=( const KGCServerList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void ActionPerformed( const KActionEvent& event );
    virtual void OnCreateComplete( void );
    virtual void OnCreate( void );
    void FrameMoveInEnabledState();
	virtual void OnInitialize();

	enum
	{
        MAX_SERVERLIST_ON_ONE_PAGE = 5
		//COLUMN_GAP_TIMES = 48,			//서버 바 한개의 넓이가 Gap의 몇배인가
		//ROW_GAP_TIMES = 36				//서버 바 한개의 높이가 Gap의 몇배인가
	};

	virtual void PostChildDraw( void );

    

    KGCServerBar* FindServerBar(DWORD dwServerUID);
    std::string GetChildFile();
    std::string GetChildPath();
    void SetServerList();
    void SetViewServerList();

	void SetFocusingHilight();

private:

	std::vector<KD3DWnd*> m_vecServerBarBack;
    KGCServerBar*   m_pkServerBar;
    std::vector<KGCServerBar*> m_vecServerBar;
	
    std::vector<KGCServerBar*> m_vecViewServerBar;

	KGCServerBar*	m_pkServerFocus;
    KD3DWnd*            m_pkClipRange;
	KD3DWnd*            m_pkBar_draw_rect;

	KD3DWnd*            m_pkBtnConnection;
    KD3DWnd*            m_pkBtnLeftPage;
    KD3DWnd*            m_pkBtnRightPage;

	KGCHighLight*	m_pkHighLight;

    int m_iTopIndex;

    int m_iViewMaxList;
	int m_iColumn;
	int m_iRow;

    int m_iCurrentPage;
    int i_delay;

    std::vector<KServerInfo> m_ServerList;
};

DEFINE_WND_FACTORY( KGCServerList );
DECLARE_WND_FACTORY( KGCServerList );
DECLARE_WND_FACTORY_NAME( KGCServerList );

#endif // _KGCSERVERLIST_H_