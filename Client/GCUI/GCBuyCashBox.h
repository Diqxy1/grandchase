#pragma once

class KD3DWnd;
class KGCItemImgWnd;
class KGCBuyCashBox : public KD3DDialog
	, public KActionListener
{
public:
	DECLARE_CLASSNAME(KGCBuyCashBox);
	/// Default constructor
	KGCBuyCashBox(void);
	/// Default destructor
	virtual ~KGCBuyCashBox(void);
	/// Copy constructor
	KGCBuyCashBox(const KGCBuyCashBox&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCBuyCashBox& operator=(const KGCBuyCashBox&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

	enum {
#ifdef NATION_KOREA
		PRICE_X = 515,
		PRICE_Y = 226,
		CURRENT_CASH_X = 517,
		CURRENT_CASH_Y = 250,
		CURRENT_VCASH_X = 517,
		CURRENT_VCASH_Y = 270,

		CALC_ORIGINAL_PRICE_X = 500,
		CALC_ORIGINAL_PRICE_Y = 336,
		CALC_DISCOUNT_X = 500,
		CALC_DISCOUNT_Y = 354,
		CALC_RESULT_X = 490,

		CALC_RESULT_Y = 369,
		CALC_RESULT_ONLY_X = 490,
		CALC_RESULT_ONLY_Y = 335,
#else
		PRICE_X = 619,
		PRICE_Y = 263,
		CURRENT_CASH_X = 621,
		CURRENT_CASH_Y = 286,
		CURRENT_VCASH_X = 621,
		CURRENT_VCASH_Y = 306,

		CALC_ORIGINAL_PRICE_X = 604,
		CALC_ORIGINAL_PRICE_Y = 370,
		CALC_DISCOUNT_X = 604,
		CALC_DISCOUNT_Y = 390,
		CALC_RESULT_X = 594,

		CALC_RESULT_Y = 405,
		CALC_RESULT_ONLY_X = 594,
		CALC_RESULT_ONLY_Y = 371,
#endif
		SMALL_MAINFRAME = 250,
		SMALL_INNERBG = 166,
		SMALL_CALCBG = 45,

		BIG_MAINFRAME = 280,
		BIG_INNERBG = 196,
		BIG_CALCBG = 75,

		CALC_RESULT_LABLE_X = 25,
		CALC_RESULT_LABLE_Y_AT_SMALL = 175,
		CALC_RESULT_LABLE_X_AT_BIG = 210,

		OK_GAP_X = 65,
		OK_GAP_Y = 5,
		CANCLE_GAP_X = 150,
		CANCLE_GAP_Y = 5,
	};

public:
	virtual void OnCreate(void);
	virtual void FrameMoveInEnabledState(void);
	virtual void ActionPerformed(const KActionEvent& event);
	virtual void PostChildDraw(void);
	virtual bool CheckCondition(IN const KDialogInfo& kInfo_);
	virtual void OnCreateComplete(void);

	void SetItemID(DWORD dwItemID);
	void ResizeWnd();
	void BuyCashItemUseCashAndVirtualCash();


private:
	void OnCheckUseVirtualCash();
	void OnCheckMixVirtualCash();
	void OnCheckUseGift();
	void OnClickOK();
	void OnClickCancle();
	void OnChangeItemPeriod();

	void CalcResultPrice();

private:
	KD3DStatic* m_pkItemPriceLabel;
	KD3DStatic* m_pkCurrentCashLabel;
	KD3DStatic* m_pkVirtualCashLabel;
	KD3DStatic* m_pkUseVirtualCash;
	KD3DStatic* m_pkMixVirtualCash;
	KD3DStatic* m_pk_use_Gift;

	KD3DStatic* m_pkCalcItemPriceLabel;
	KD3DStatic* m_pkCalcVirtualCashLabel;
	KD3DStatic* m_pkCalcPercentCashLabel;
	KD3DStatic* m_pkCalcResultCashLabel;

	KGCItemImgWnd* m_pkItemImg;

	KD3DCheckBox* m_pkCheckUseVirtualCash;
	KD3DCheckBox* m_pkCheckMixVirtualCash;
	KD3DCheckBox* m_pkCheckUseGift;

	KD3DComboBox* m_pkComboItemPeriod;

	KD3DWnd* m_pkOK;
	KD3DWnd* m_pkCancle;

	KD3DSizingBox* m_pkMainframe;
	KD3DSizingBox* m_pkCalcBG;
	KD3DSizingBox* m_pkGiftBG;
	KD3DSizingBox* m_pkInnerBG;

	KD3DSizingBox* m_pkGiftNicknameBG;
	KD3DSizingBox* m_pkGiftCommentBG;

	KD3DEdit* m_pEditGiftID;
	KD3DEdit* m_pEditGiftComment;

	KD3DWnd* m_pkFontImgNickname;

private:
	DWORD			m_dwItemID;
	int				m_iOriginalPrice;
	int				m_iDiscount;
	int				m_iResultPrice;
	GCItem* m_pkItem;
};

DEFINE_WND_FACTORY(KGCBuyCashBox);
DECLARE_WND_FACTORY(KGCBuyCashBox);
DECLARE_WND_FACTORY_NAME(KGCBuyCashBox);
