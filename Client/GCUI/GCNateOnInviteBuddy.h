#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>

class GCInviteBuddyTree;

class GCNateOnInviteBuddy : public KD3DDialog,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME(GCNateOnInviteBuddy);
	GCNateOnInviteBuddy(void);
	virtual ~GCNateOnInviteBuddy(void);

	void OnOK();
	void OnCancel();

protected:
	virtual void ActionPerformed(const KActionEvent& event);
	virtual void OnCreate(void);

	KD3DWnd*					m_pkInvite;
	KD3DWnd*					m_pkClose;
	GCInviteBuddyTree*			m_pkBuddyTree;
};

DEFINE_WND_FACTORY(GCNateOnInviteBuddy);
DECLARE_WND_FACTORY(GCNateOnInviteBuddy);
DECLARE_WND_FACTORY_NAME(GCNateOnInviteBuddy);
