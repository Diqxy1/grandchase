#pragma once
#include "GCUI/GCDungeonTitleColorS6.h"

extern int              Result_ReportNickColor;

class KGCNicknameColorBox : public KD3DDialog    // extends
	, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME(KGCNicknameColorBox);
	KGCNicknameColorBox(void);
	virtual ~KGCNicknameColorBox(void);
	KGCNicknameColorBox(const KGCNicknameColorBox&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCNicknameColorBox& operator=(const KGCNicknameColorBox&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}
public:
	KD3DWnd* m_pkBtnConfirm;
	KD3DWnd* m_pkClose;

	KD3DComboBox* m_pkColorCombo;


	KD3DStatic* m_pkDefaultMessage;
	std::wstring currentColor;
	std::vector<wstring> m_vecstrNickColorList;
	void OnClose();
	void OnConfirm();
	void SetNicknameColor();
protected:
	virtual void OnCreate(void);
	virtual void ActionPerformed(const KActionEvent& event);
private:
};
DEFINE_WND_FACTORY(KGCNicknameColorBox);
DECLARE_WND_FACTORY(KGCNicknameColorBox);
DECLARE_WND_FACTORY_NAME(KGCNicknameColorBox);