#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>
#include "GCNateOn.h"
//#include <set>

#define MAX_INVITE_CHILD_NUM 9

class GCInviteBuddyChild;
class KD3DScrollbar;

class GCInviteBuddyTree : public KD3DWnd,
						  public KActionListener,
						  public boost::noncopyable
{
public:
	DECLARE_CLASSNAME( GCInviteBuddyTree );
	GCInviteBuddyTree(void);
	virtual ~GCInviteBuddyTree(void);

	void UpdateData();
	void SetMode( int iIdx, GCNateOn::E_NATEON_MODE eMode );
	void OnChild( KD3DWnd* pkWnd );
	void OnChildRClick( KD3DWnd* pkWnd );
	int GetTopIndex() const { return m_iTopIndex; }
	void GetInviteList( std::set<DWORD>& vecInviteList );

protected:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate();
	virtual void OnCreateComplete();
	void OnScroll( DWORD dwEventCode );

private:
	KSafeArray<GCInviteBuddyChild*,MAX_INVITE_CHILD_NUM> m_pkChild;
	int m_iTopIndex;
	GCNateOn::E_NATEON_MODE m_eMode;
	KD3DScrollbar* m_pkScroll;
	std::map< DWORD, bool > m_mapClickList;
};

DEFINE_WND_FACTORY( GCInviteBuddyTree );
DECLARE_WND_FACTORY( GCInviteBuddyTree );
DECLARE_WND_FACTORY_NAME( GCInviteBuddyTree );