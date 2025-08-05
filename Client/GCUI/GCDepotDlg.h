#pragma once

class KGCDepotChar;
class KGCCommonItemWnd;

class KGCDepotDlg : public KD3DDialog    // extends
	, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME(KGCDepotDlg);
	/// Default constructor
	KGCDepotDlg(void);
	/// Default destructor
	virtual ~KGCDepotDlg(void);
	/// Copy constructor
	KGCDepotDlg(const KGCDepotDlg&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCDepotDlg& operator=(const KGCDepotDlg&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

public:
	virtual void ActionPerformed(const KActionEvent& event);
	virtual void OnCreate(void);
	virtual void OnCreateComplete(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void PostChildDraw();

	void UpdateCharacterSelect(void);
	void UpdateDepotData(void);

	void CreateUI(void);
	void SetBoxPosition(std::vector<KD3DWnd*> &vecBox, int With, D3DXVECTOR2 pos_);
	void SetTargetBoxList(void);
	void SetDepotBackBox(void);
	void SetSafeBoxPosition(std::vector<KD3DWnd*> &vecBox, int SafeType, bool bIsBtn, KActionListener* pListerner);
	void SetInvenItemList();
	void SetShowInvenItemList();
	void SetSafeList();
	void SetSafeList(int iSelSafe);
	void ClearDepotItemImageList();
	void SetDepotItemList();

	void CheckDrowItemInfo();
	void DrawItemInfo(KItem* pItem, D3DXVECTOR2 vPos);

	void SetNPCFailFace() { m_iFaceTime = 100; m_pkNpcFailFace->ToggleRender(true); }
	void SetItemImg(KItem* pInventoryItem, KGCCommonItemWnd* pkItemImg);

	void ProgressStartHoverBox();
	int ProgressStartInvenDrag(bool btnUp);
	int ProgressStartDepotDrag(bool btnUp);
	void EndlDrag(void);

	void OnScrollPos(void);
	void OnClickSafe(const KActionEvent& event);
	void OnClickSafeAdd(void);
	void OnClickSlotAdd(void);
	void OnClickClose(void);

	void ActionDepotAddItemRbtnClick(GCITEMUID dwItemUID);
	void ActionDepotDelItemRbtnClick(GCITEMUID dwItemUID);
	void ActionInvenDrag(GCITEMUID dwItemUID, KGCCommonItemWnd* pImage);
	void ActionDepotDrag(GCITEMUID dwItemUID, KGCCommonItemWnd* pImage);
	void UpdateItemSearch(std::wstring SearchName);

	bool isDepotSearchActive = false;

private:
	KGCDepotChar* m_pkCharacterInfo;
	KD3DWnd*	  m_pkBottomCloseBtn;
	KD3DWnd*	  m_pkCloseBtn;
	KD3DStatic*   m_pkDescStatic;
	KD3DStatic*   m_pkNpcStatic;
	KD3DWnd*	  m_pkClipRange;
	KD3DWnd*	  m_pkSafeClipRange;
	KD3DWnd*	  m_pkBtnSafeAdd;
	KD3DWnd*	  m_pkBtnSlotAdd;
	KD3DWnd*	  m_pkNpcFailFace;
	KD3DScrollbar* m_pkScrollbar;
	KD3DSizingBox* m_pkInven_Back;

	std::vector<KD3DWnd*> m_vecDepotBackBox;
	std::vector<KD3DWnd*> m_vecInvenBackBox;
	std::vector<KD3DWnd*> m_vecDepotLockBackBox;
	std::vector<KD3DWnd*> m_vecDepotSafeBtn;
	std::vector<KD3DWnd*> m_vecDepotSafeLock;

	std::vector<KD3DWnd*> m_vecDepotTargetBox;

	std::vector<std::pair<GCITEMUID, KGCCommonItemWnd*>> m_vecInvenItemImgList;
	std::vector<std::pair<GCITEMUID, KGCCommonItemWnd*>> m_vecDepotItemImgList;

	std::vector<GCITEMUID> m_vecShowInvenItemList;
	std::vector<GCITEMUID> m_vecTempItemList;

	int m_iScrollpos;

	KGCCommonItemWnd* m_pkCurrentDragImage;
	KItem* m_pkCurrentDragItem;

	int m_iFaceTime;
	int m_iMentTime;
	int m_iCurrentMent;

	std::vector<wstring> m_vecMentList;
	std::wstring DepotSearch;
	GAME_STATE GetCurrentGameState;
};

DEFINE_WND_FACTORY(KGCDepotDlg);
DECLARE_WND_FACTORY(KGCDepotDlg);
DECLARE_WND_FACTORY_NAME(KGCDepotDlg);
