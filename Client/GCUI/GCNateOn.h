#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>
//#include "GCEnum.h"
//#include "define.h"

class KD3DStatic;
class GCBuddyTree;
class GCNateOnMultiMenu;
class GCNateOnChat;
class GCTalkList;
struct FRIEND_UI_DATA;
class GCBuddyChild;
class KGCMultiMenu;
class KGCAlertWnd;
class KD3DImageIcon;

class GCNateOn : public KD3DDialog,
	public KActionListener,
	public boost::noncopyable
{
public:
	enum E_NATEON_MODE
	{
		INIT_MODE = -1,
		CHAT_MODE = 0,
		FRIEND_MODE,
		GUILD_MODE,
		BLOCK_MODE,
		NUM_MODE,
	};

	enum
	{
		NATEON_TALK_BOX_SELECT = 100,
		NATEON_TALK_USER_SELECT = 101,
	};

	DECLARE_CLASSNAME(GCNateOn);
	GCNateOn(void);
	virtual ~GCNateOn(void);

	virtual void ActionPerformed(const KActionEvent& event);
	virtual bool InitDialog(IN const KDialogInfo& kInfo_);
	void UpdateTreeData();
	void OnTreeRButtonClick(const FRIEND_UI_DATA& kData);
	bool IsPopupOn() const;
	void SetNateonMode(E_NATEON_MODE eMode);
	GCNateOnChat* GetNateOnChatWindow(void) { return m_pkChat; }
	GCTalkList* GetTalkListWindow(void) { return m_pkTalkList; }
	GCNateOnMultiMenu* GetMultiMenu(void) { return m_pkMultiMenu; }
	void CheckTalkList(void);
	bool IsNew(void) { return m_bNew; }
	void SetNew(bool bNew) { m_bNew = bNew; }
	void SetOnlineCount();
	void SetNickName();
	void SetGrade();
	void DeleteAllTalkList();
	void DeleteTalkBox();

protected:
	virtual void OnCreate();
	virtual void OnCreateComplete();
	virtual void OnDestroy();
	virtual void OnDestroyComplete();
	virtual void FrameMoveInEnabledState();
	virtual void PostChildDraw();

private:
	void SetTooltips();
	void SetTreeMode(E_NATEON_MODE eMode);
	void OnBuddyList();
	void OnGuildList();
	void OnBlockList();
	void OnAddGroup();
	void OnBlockBuddy();
	void OnChangeEmoticon();
	void OnAddBuddy();
	void OnSelectTalkBox();
	void OnRClickCouple();
	void OnClickCouplePopup(const KActionEvent& event);

	void OnRecommend();

private:
	KSafeArray<KD3DWnd*, GC_GRADE_NUM> m_ppkGrade;
	KD3DStatic* m_pkNickName;
	KD3DWnd* m_pkClose;
	KD3DWnd* m_pkSendMailBtn;
	KD3DWnd* m_pkInboxBtn;
	KD3DWnd* m_pkDraftsBtn;
	KD3DStatic* m_pkOnline;
	KD3DWnd* m_pkAddBuddyBtn;
	KD3DWnd* m_pkBlockBuddyBtn;
	KD3DWnd* m_pkAddGroupBtn;
	KD3DWnd* m_pkBuddyListBtn;
	KD3DWnd* m_pkGuildListBtn;
	KD3DWnd* m_pkBlockListBtn;
	KD3DWnd* m_pkModeBuddyList;
	KD3DWnd* m_pkModeGuildList;
	KD3DWnd* m_pkModeBlockList;
	E_NATEON_MODE m_eMode;
	GCBuddyTree* m_pkTree;
	GCNateOnChat* m_pkChat;
	std::map< GCNateOn::E_NATEON_MODE, int > m_mapTopIndex;
	GCNateOnMultiMenu* m_pkMultiMenu;
	KD3DWnd* m_pkChangeEmoticon;
	KD3DWnd* m_pkEmoticon;
	KD3DWnd* m_pkEmoticonBG;
	GCTalkList* m_pkTalkList;
	KGCMultiMenu*	m_pkCouplePopup;
	GCBuddyChild* m_pkSpecialFriend;
	KD3DImageIcon* m_pkTexGuildMark;

	bool		m_bNew;

	KGCAlertWnd* m_pkRecommend;

	bool		m_bViewRecommendBtn;

	KD3DWnd* m_pkWaitImage;

public:
	void SetViewRecommendBtn(bool bEnable) { m_bViewRecommendBtn = bEnable; }
	bool GetViewRecommendBtn() { return m_bViewRecommendBtn; }

	void OnViewWaitImage() {
		if (m_pkWaitImage != NULL) {
			m_pkWaitImage->ToggleRender(true);    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_HIDE_GUID_BOOK));
		}
	}
	void OffViewWaitImage() {
		if (m_pkWaitImage != NULL) {
			m_pkWaitImage->ToggleRender(false);    g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SHOW_GUID_BOOK));
		}
	}

	bool IsWaitImageRender() { return (m_pkWaitImage->IsRenderOn() ? true : false); }

	// 길드 메신저용 추가 ui
private:
	KD3DWnd* m_pkGuildOfflineCheckBox;
	KD3DWnd* m_pkGuildOfflineCheckSign;
	KD3DWnd* m_pkGuildBlockCheckBox;
	KD3DWnd* m_pkGuildBlockCheckSign;

	KD3DStatic* m_pkGuildName;
	KD3DStatic* m_pkGuildOnline;
	KD3DStatic* m_pkGuildOfflineCheck;
	KD3DStatic* m_pkGuildBlockCheck;

public:
	void SetGuildBlockCheckBox(bool bCheck_);
	void SetGuildOfflineCheckBox(bool bCheck_);
	void OnClose();
};

extern GCNateOn* g_pNateOn;

DEFINE_WND_FACTORY(GCNateOn);
DECLARE_WND_FACTORY(GCNateOn);
DECLARE_WND_FACTORY_NAME(GCNateOn);
