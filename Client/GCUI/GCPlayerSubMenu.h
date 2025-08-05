#ifndef _KGCPLAYERSUBMENU_H_
#define _KGCPLAYERSUBMENU_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCPlayerSubMenu : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME(KGCPlayerSubMenu);
	/// Default constructor
	KGCPlayerSubMenu(void);
	/// Default destructor
	virtual ~KGCPlayerSubMenu(void);
	/// Copy constructor
	KGCPlayerSubMenu(const KGCPlayerSubMenu&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCPlayerSubMenu& operator=(const KGCPlayerSubMenu&)
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

DEFINE_WND_FACTORY(KGCPlayerSubMenu);
DECLARE_WND_FACTORY(KGCPlayerSubMenu);
DECLARE_WND_FACTORY_NAME(KGCPlayerSubMenu);

extern KGCPlayerSubMenu* g_pkSubMenu;

#endif //_KGCPLAYERSUBMENU_H_