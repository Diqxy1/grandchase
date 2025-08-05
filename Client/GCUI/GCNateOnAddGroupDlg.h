#pragma once

//#include "../uifx/D3DWnd.h"
//#include <boost/noncopyable.hpp>

class KD3DStatic;
class KD3DEdit;

class GCNateOnAddGroupDlg : public KD3DDialog,
	public KActionListener,
	public boost::noncopyable
{
public:
	DECLARE_CLASSNAME(GCNateOnAddGroupDlg);
	GCNateOnAddGroupDlg(void);
	virtual ~GCNateOnAddGroupDlg(void);

	enum WARN_TYPE
	{
		NO_WARN = 0,
		EQUAL,
		ILLEGAR,
	};

	enum DLG_USE
	{
		ADD = 0,
		RENAME,
		BLOCK_BUDDY,
	};

	virtual void ActionPerformed(const KActionEvent& event);
	virtual bool CheckCondition(IN const KDialogInfo& kInfo_);
	std::wstring GetInputStr();
	void SetUse(GCNateOnAddGroupDlg::DLG_USE eUse, int nGroupID) { m_eUse = eUse; m_nGroupID = nGroupID; }
	void Transformation();
	GCNateOnAddGroupDlg::DLG_USE GetUse() const { return m_eUse; }
	int GetGroupID() const { return m_nGroupID; }
	void SetReserveAction(bool bReserve) { m_bReserveAction = bReserve; }
	bool IsReserveAction() const { return m_bReserveAction; }
	void SetOrigGroupName(const std::wstring& strGroupName);

protected:
	virtual void OnCreate();
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState();
	virtual void OnDestroyComplete();
	void OnClose();
	void OnOK();
	void SetWarning(GCNateOnAddGroupDlg::WARN_TYPE eWarnType);
	void AddModeFrameMove();
	void RenameModeFrameMove();
	void BlockModeFrameMove();

private:
	KD3DWnd* m_pkOK;
	KD3DWnd* m_pkCancel;
	KD3DStatic* m_pkTitle;
	KD3DStatic* m_pkDesc;
	KD3DStatic* m_pkDesc2;
	KD3DEdit* m_pkEditGroup;
	GCNateOnAddGroupDlg::WARN_TYPE m_eWarn;
	std::wstring m_strPrevInput;
	GCNateOnAddGroupDlg::DLG_USE m_eUse;
	int m_nGroupID;
	bool m_bReserveAction;
	std::wstring m_strOriGroupName;
	std::wstring m_strNoWarn;
};

DEFINE_WND_FACTORY(GCNateOnAddGroupDlg);
DECLARE_WND_FACTORY(GCNateOnAddGroupDlg);
DECLARE_WND_FACTORY_NAME(GCNateOnAddGroupDlg);
