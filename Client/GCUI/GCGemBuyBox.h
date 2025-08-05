#ifndef _GCGEMBUYBOX_H_
#define _GCGEMBUYBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../define.h"
#include "ProtectedType.h"

class KD3DStatic;

class KGCGemBuyBox : public KD3DDialog,        // extends
	public KActionListener // implements
{
public:
	DECLARE_CLASSNAME(KGCGemBuyBox);
	/// Default constructor
	KGCGemBuyBox(void);
	/// Default destructor
	virtual ~KGCGemBuyBox(void);
	/// Copy constructor
	KGCGemBuyBox(const KGCGemBuyBox&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCGemBuyBox& operator=(const KGCGemBuyBox&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

public:
	enum EGemBuyBoxResult
	{
		GBB_CREATED,
		GBB_DESTROYED,
		GBB_YES,
		GBB_NO,
	};

public:
	inline int GetGemNum(void) const { return m_iGemNum; }
	bool CheckAvailableNum(int iNum);
	int GetAvilableNum();
	virtual void ActionPerformed(const KActionEvent& event);

protected:
	virtual void OnCreate(void);
	virtual void OnCreateComplete(void);
	virtual void OnDestroyComplete(void);
	virtual void FrameMoveInEnabledState(void);

	void SetGemNum(int iGemNum);

protected:
	KD3DStatic* m_pkTxt;
	KD3DWnd*    m_pkYes;
	KD3DWnd*    m_pkNo;
	KD3DWnd*    m_pkGemNumUp;
	KD3DWnd*    m_pkGemNumDown;
	KD3DStatic* m_pkGemNum;
	KD3DStatic* m_pkGPPrice;
	KProtectedType<int> m_iGemNum;
	int         m_iMaxGemNum;
	int         m_iPressCount;
};

DEFINE_WND_FACTORY(KGCGemBuyBox);
DECLARE_WND_FACTORY(KGCGemBuyBox);
DECLARE_WND_FACTORY_NAME(KGCGemBuyBox);

#endif // _GCGEMBUYBOX_H_
