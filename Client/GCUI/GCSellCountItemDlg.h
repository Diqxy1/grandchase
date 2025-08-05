#pragma once

class KGCSellCountItemDlg : public KD3DDialog    // extends
	, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME(KGCSellCountItemDlg);
	/// Default constructor
	KGCSellCountItemDlg(void);
	/// Default destructor
	virtual ~KGCSellCountItemDlg(void);
	/// Copy constructor
	KGCSellCountItemDlg(const KGCSellCountItemDlg&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCSellCountItemDlg& operator=(const KGCSellCountItemDlg&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

	enum {
		NUM_PAD = 10,
	};

	enum
	{
		EM_MODE_SELL_LIST,
		EM_MODE_SELL_INVEN,
		EM_MODE_INSERT_DEPOT_FROM_INVENTORY,
		EM_MODE_INSERT_DEPOT_FROM_DEPOT_UI,
		EM_MODE_DELETE_DEPOT,
		EM_MODE_MOVE_VDEPOT_TO_INVEN,
		EM_MODE_MOVE_INVEN_TO_VDEPOT,
	};

public:
	void SetUpdateUI();
	void OnClose();
	void OnClickClose();

private:
	virtual bool CheckCondition(IN const KDialogInfo& kInfo_);
	virtual bool InitDialog(IN const KDialogInfo& kInfo_);
	virtual void ActionPerformed(const KActionEvent& event) {};
	virtual void OnCreate(void);
	virtual void FrameMoveInEnabledState(void);

	void SetSellCount(int iCount_, bool bUpdateScroll_ = true);
	void SetScrollPos(float fPercent_);
	void UpdateModeState();
	void OnClickOK();
	void OnClickMax();
	void OnClickMin();
	void OnClickBackspace();
	void OnClickClear();
	void OnClickIncresase1() { SetSellCount(m_iSellCount + 1); };
	void OnClickIncresase10() { SetSellCount(m_iSellCount + 10); };
	void OnClickDecresase1() { SetSellCount(m_iSellCount - 1); };
	void OnClickDecresase10() { SetSellCount(m_iSellCount - 10); };
	void OnClickNumber(const KActionEvent& event);
	void OnStartScroll() { m_bScrollStart = true; };
	void OnEndScroll() { m_bScrollStart = false; };

private:
	KD3DWnd*            m_pkBtnOK;
	KD3DWnd*            m_pkBtnCancle;
	KD3DWnd*            m_pkBtnClose;
	KD3DWnd*            m_pkIncrease1;
	KD3DWnd*            m_pkDecrease1;
	KD3DWnd*            m_pkIncrease10;
	KD3DWnd*            m_pkDecrease10;
	KD3DWnd*            m_pkScrollLine;
	KD3DWnd*            m_pkBtnScroll;

	KD3DWnd*            m_pkSellTitle;
	KD3DWnd*            m_pkInsertDepotTitle;
	KD3DWnd*            m_pkDeleteDepotTitle;
	KD3DWnd*            m_pkSellCountTitle;
	KD3DWnd*            m_pkDepotCountTitle;
	KD3DWnd*            m_pkInsertDepotCountTitle;
	KD3DWnd*            m_pkDeleteDepotCountTitle;
	KD3DWnd*            m_pkVirtualDepotTitle;

	KGCItemImgWnd*      m_pkItemImg;

	KD3DStatic*         m_pkItemName;
	KD3DStatic*         m_pkDesc;
	KGCUINumber*        m_pkOwnCount;
	KGCUINumber*        m_pkSellCount;
	KGCUINumber*        m_pkReward;
	KGCItemBar*         m_pkMoneyType;

	KD3DSizingBoxBtn*   m_pkBtnNums[NUM_PAD];
	KD3DSizingBoxBtn*   m_pkBtnMax;
	KD3DSizingBoxBtn*   m_pkBtnMin;
	KD3DSizingBoxBtn*   m_pkBtnBackspace;
	KD3DSizingBoxBtn*   m_pkBtnClear;

	bool m_bScrollStart;

	std::map<KD3DWnd::uid_type, int> m_mapNumbers;
	int m_iMaxNum;
	GCITEMUID       m_ItemUID;
	int m_iSellCount;

	int m_emModeState;
};

DEFINE_WND_FACTORY(KGCSellCountItemDlg);
DECLARE_WND_FACTORY(KGCSellCountItemDlg);
DECLARE_WND_FACTORY_NAME(KGCSellCountItemDlg);
