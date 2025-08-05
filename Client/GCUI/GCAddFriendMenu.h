#ifndef _KGCADDFRIENDMENU_H_
#define _KGCADDFRIENDMENU_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCAddFriendMenu : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME(KGCAddFriendMenu);
	/// Default constructor
	KGCAddFriendMenu(void);
	/// Default destructor
	virtual ~KGCAddFriendMenu(void);
	/// Copy constructor
	KGCAddFriendMenu(const KGCAddFriendMenu&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCAddFriendMenu& operator=(const KGCAddFriendMenu&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

public:
	virtual void ActionPerformed(const KActionEvent& event);
	void SetFriendNickName(std::wstring strNickName) { m_strFriendNickName = strNickName; }

protected:
	virtual void OnCreate(void);

private:
	KD3DStatic* m_pkAddFriend;
	std::wstring m_strFriendNickName;
};

DEFINE_WND_FACTORY(KGCAddFriendMenu);
DECLARE_WND_FACTORY(KGCAddFriendMenu);
DECLARE_WND_FACTORY_NAME(KGCAddFriendMenu);

extern KGCAddFriendMenu* g_pkAddFriendMenu;

#endif //_KGCADDFRIENDMENU_H_
