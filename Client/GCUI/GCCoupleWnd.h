#pragma once
#include "KGC2DObject.h"

class KD3DView;
class KD3DStatic;
class KGCCheckBox;
class KGCCoupleMark;
class KGCCoupleCharSelect;
class KD3DSizingBoxBtn;
class KGCSayBalloonSquare;

class KGCCoupleWnd : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME(KGCCoupleWnd);

	KGCCoupleWnd(void);
	virtual ~KGCCoupleWnd(void);
	/// Copy constructor
	KGCCoupleWnd(const KGCCoupleWnd&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCCoupleWnd& operator=(const KGCCoupleWnd&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

	enum {
		GCCW_LEFT_CHAR = 0,
		GCCW_RIGHT_CHAR = 1,

		GCCW_CHAR_NUM,
	};

	enum {
		GCCW_MOTION_NUM = 8,

		GCCW_SLOT_HELMET = 0,
		GCCW_SLOT_SUIT = 1,
		GCCW_SLOT_PANTS = 2,
		GCCW_SLOT_SHOES = 3,
		GCCW_SLOT_ITEM = 4,
		GCCW_SLOT_BG = 5,
		GCCW_SLOT_NUM = 6,
	};

public:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void OnDestroyComplete(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void PostChildDraw(void);
	virtual void ActionPerformed(const KActionEvent& event);

	void FrameMove_Marks();

	void Update();

	void SetMyCoupleWnd();
	void SetOtherCoupleWnd();
	void UpdateCharacterSelect();

private:
	void ToggleChatList();
	void InitCoupleView();
	void UpdateEquipSlot();
	void SetDefaultMotion();
	void OnClickMotionBtn(int i);
	void OnClickSlot(int i);
	void AddMotionToCouple(int iMotionID, bool bOnce, bool bClear);
	void OnEdit(DWORD dwCode);
	void AddChat(bool bMyChat, bool bRight, std::wstring& strMsg);
	void AddSystemMessage(const std::wstring& strMsg);
	void AddItemParts(int iItemID);

	void ChatEnable(bool bEnable);

	const char* GetCharSex(int iCharType);
	std::string GetMotionID(int iMotionNum, int iCharType, int iPartnerCharType, bool bRight);
private:
	bool		m_bMyCouple;
	int			m_iCurrMotion;
	KD3DView* m_pkCoupleView;
	KSafeArray<KD3DStatic*, GCCW_CHAR_NUM> m_pkNickName;
	KSafeArray<KGCCoupleCharSelect*, GCCW_CHAR_NUM> m_pkCharSelect;

	KD3DWnd* m_pkRingSlot;
	KSafeArray<KD3DSizingBoxBtn*, GCCW_SLOT_NUM> m_pkSlot;
	std::map<int, DWORD>	m_mapSlotInfo;
	KSafeArray<KD3DStaticBtn*, GCCW_MOTION_NUM> m_pkMotion;

	KD3DStatic* m_pkMotionText;
	KGC2DObject				m_kRingImage;
	KSafeArray<KGC2DObject, GCCW_SLOT_NUM> m_akItemImage;

	//chatting
	KD3DWnd* m_pkChatBg;
	KD3DWnd* m_pkChatUngerBg;
	KD3DWnd* m_pkChatEnter;

	KGCCheckBox* m_pkChatListOnOff;
	KD3DWnd* m_pkChatListBg;
	KD3DListBox* m_pkChatList;
	KD3DWnd* m_pkChatListClose;

	KD3DEdit* m_pkChatEdit;
	KGCSayBalloonSquare* m_pkSayBalloon;
	KD3DStatic* m_pkCoupleChatStatic;

	int						m_iSystemMsgAlpha;
	int						m_iSystemMsgViewFrm;
	KD3DStatic* m_pkSystemMessage;
	KD3DWnd* m_pkSystemMsgBg;
	float					m_fOutlineWidthOrigin;

	KSafeArray<KGCCoupleMark*, GCCW_CHAR_NUM> m_pkMark;
};

DEFINE_WND_FACTORY(KGCCoupleWnd);
DECLARE_WND_FACTORY(KGCCoupleWnd);
DECLARE_WND_FACTORY_NAME(KGCCoupleWnd);
