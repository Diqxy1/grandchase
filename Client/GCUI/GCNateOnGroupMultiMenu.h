#pragma once

//#include "../uifx/D3DWnd.h"
//#include <boost/noncopyable.hpp>

class KD3DStatic;
class KD3DSizingBox;

#define MAX_GROUP_POPUP_NUM 10

class GCNateOnGroupMultiMenu : public KD3DWnd,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME(GCNateOnGroupMultiMenu);
	GCNateOnGroupMultiMenu(void);
	virtual ~GCNateOnGroupMultiMenu(void);

	virtual void ActionPerformed(const KActionEvent& event);
	void SetGroups(DWORD dwUID);
	int GetGroupCount(void) { return (int)m_vecGroupID.size(); }

protected:
	virtual void OnCreate();
	virtual void OnCreateComplete();
	void AdjustMenuSize();
	void OnCursorEnter(KD3DWnd* pkWnd);
	void OnCursorLeave(KD3DWnd* pkWnd);
	void OnClick(KD3DWnd* pkWnd);
	void OnMoving();

private:
	KD3DSizingBox* m_pkBG;
	KSafeArray<KD3DStatic*, MAX_GROUP_POPUP_NUM> m_pkMenu;
	std::vector< int > m_vecGroupID;
	int m_nFromGroupID;
	DWORD m_dwUserUID;
};

DEFINE_WND_FACTORY(GCNateOnGroupMultiMenu);
DECLARE_WND_FACTORY(GCNateOnGroupMultiMenu);
DECLARE_WND_FACTORY_NAME(GCNateOnGroupMultiMenu);
