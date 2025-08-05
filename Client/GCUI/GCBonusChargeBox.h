#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
struct KInventoryItem;

class KGCBonusChargeBox : public KD3DDialog,        // extends
	public KActionListener // implements
{
public:
	DECLARE_CLASSNAME(KGCBonusChargeBox);
	/// Default constructor
	KGCBonusChargeBox(void);
	/// Default destructor
	virtual ~KGCBonusChargeBox(void);
	/// Copy constructor
	KGCBonusChargeBox(const KGCBonusChargeBox&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCBonusChargeBox& operator=(const KGCBonusChargeBox&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

public:
	virtual void ActionPerformed(const KActionEvent& event);
	virtual bool InitDialog(IN const KDialogInfo& kInfo_);
	enum EGCBonusChargeBox
	{
		BCB_CREATED,
		BCB_DESTROYED,
		BCB_YES,
		BCB_NO,
	};

	inline UINT GetChargeFactor() const { return m_nChargeFactor; }
	GCITEMUID GetChargingItem() { return m_dwUID; }
	void SetChargingItem(GCITEMUID dwUID_);

protected:
	virtual void OnCreate(void);
	virtual void OnCreateComplete(void);
	virtual void OnDestroyComplete(void);

	void SetChargeFactor(UINT nChargeFactor);
	DWORD GetOKCode() const;

protected:
	KD3DStatic* m_pkChargeTitle;
	KD3DStatic* m_pkChargeError;
	KD3DStatic* m_pkCurrentNum;
	KD3DWnd* m_pkYes;
	KD3DWnd* m_pkNo;
	KD3DWnd* m_pkBonusNumUp;
	KD3DWnd* m_pkBonusNumDown;
	KD3DStatic* m_pkChargeNum;
	UINT        m_nChargeFactor;
	UINT        m_nMaxChargeNum;
	int         m_iPressCount;
	GCITEMUID         m_dwUID;
};

DEFINE_WND_FACTORY(KGCBonusChargeBox);
DECLARE_WND_FACTORY(KGCBonusChargeBox);
DECLARE_WND_FACTORY_NAME(KGCBonusChargeBox);
