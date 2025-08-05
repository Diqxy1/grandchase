#pragma once

//#include "../uifx/D3DWnd.h"
//#include <string>
//#include <boost/noncopyable.hpp>

class KD3DStatic;
class KD3DEdit;

class GCNateOnAddFriend : public KD3DDialog,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME(GCNateOnAddFriend);
	GCNateOnAddFriend(void);
	virtual ~GCNateOnAddFriend(void);

	enum E_USE
	{
		INIT = -1,
		ADD = 0,
		REQUEST,
	};

	virtual void ActionPerformed(const KActionEvent& event);
	virtual bool CheckCondition(IN const KDialogInfo& kInfo_);
	void SetUse(GCNateOnAddFriend::E_USE eUse);
	GCNateOnAddFriend::E_USE GetUse() const { return m_eUse; }
	void SetReserveAction(bool bReserve) { m_bReserveAction = bReserve; }
	bool IsReserveAction() const { return m_bReserveAction; }
	void SetRequesterUID(const DWORD dwRequestUID) { m_dwRequestUID = dwRequestUID; }
	DWORD GetRequesterUID(void) { return m_dwRequestUID; }

	std::wstring GetNickName(void) { return m_wstrNickName; }
	std::wstring GetMessage(void) { return m_wstrMessage; }
	std::wstring GetAccountNickName(void) { return m_strAccountNickName; }
	void SetNickName(std::wstring wstrNickName) { m_wstrNickName = wstrNickName; }
	void SetMessage(std::wstring wstrMessage) { m_wstrMessage = wstrMessage; }
	void SetAccountNickName(std::wstring wstrAccountNickName) { m_strAccountNickName = wstrAccountNickName; }

protected:
	virtual void OnCreate();
	virtual void OnCreateComplete();
	virtual void OnDestroyComplete();
	virtual void FrameMoveInEnabledState();

	void Transformation();
	void OnOK();
	void OnCancel();
	void AddModeFrameMove();

private:
	KD3DStatic* m_pkTitle;
	KD3DStatic* m_pkNickNameStatic;
	KD3DStatic* m_pkNickNameGuide;
	KD3DEdit* m_pkNickNameEdit;
	KD3DStatic* m_pkMessageStatic;
	KD3DStatic* m_pkMessageGuide;
	KD3DEdit* m_pkMessageEdit;
	KD3DWnd* m_pkOK;
	KD3DWnd* m_pkCancel;
	GCNateOnAddFriend::E_USE m_eUse;
	bool m_bReserveAction;
	std::wstring m_wstrNickName;
	std::wstring m_wstrMessage;
	std::wstring m_strAccountNickName;
	DWORD m_dwRequestUID;
};

DEFINE_WND_FACTORY(GCNateOnAddFriend);
DECLARE_WND_FACTORY(GCNateOnAddFriend);
DECLARE_WND_FACTORY_NAME(GCNateOnAddFriend);
