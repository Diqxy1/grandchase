#pragma once

//#include "../uifx/D3DWnd.h"
//#include <boost/noncopyable.hpp>
#include "GCNateOn.h"
#include "GCBuddyTree.h"

class KD3DStatic;
class KD3DSizingBox;
class GCNateOnGroupMultiMenu;

#define MAX_POPUP_NUM 6

class GCNateOnMultiMenu : public KD3DWnd,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME(GCNateOnMultiMenu);
	GCNateOnMultiMenu(void);
	virtual ~GCNateOnMultiMenu(void);

	virtual void ActionPerformed(const KActionEvent& event);
	void SetMode(GCNateOn::E_NATEON_MODE eMode, const FRIEND_UI_DATA& kData);
	void AdjustChildPosition();

	typedef void (GCNateOnMultiMenu::*EXECEFUNC)(void);
	void UnBlockFriendAck();
	void BlockFriendAck();

protected:
	virtual void OnCreate();
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState();

	void AdjustMenuSize();
	void OnCursorEnter(KD3DWnd* pkWnd);
	void OnCursorLeave(KD3DWnd* pkWnd);
	void OnClick(KD3DWnd* pkWnd);
	void SetFunc();

	void RenameGroup();
	void DeleteGroup();
	void StartChat();
	void SendMail();
	void Block();
	void DeleteBuddy();
	void RegisterBuddy();
	void UnBlock();
	void MoveBuddy();

	void DeleteBuddyAck();
	void FollowFriend();
	void OnClose(const KActionEvent& event);

private:
	KD3DSizingBox* m_pkBG;
	KSafeArray<KD3DStatic*, MAX_POPUP_NUM> m_pkMenu;
	GCNateOnGroupMultiMenu* m_pkGroupMultiMenu;
	GCNateOn::E_NATEON_MODE m_eMode;
	FRIEND_UI_DATA m_kUIData;
	std::map< std::wstring, EXECEFUNC > m_mapExecuteFunc;
	int m_iCanFollowFriend;
};

DEFINE_WND_FACTORY(GCNateOnMultiMenu);
DECLARE_WND_FACTORY(GCNateOnMultiMenu);
DECLARE_WND_FACTORY_NAME(GCNateOnMultiMenu);
