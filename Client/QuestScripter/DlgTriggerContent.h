#pragma once
#include "SplitterControl.h"
#include "MyTreeCtrl.h"
#include "ReadOnlyEdit.h"
#include "BaseTrigger.h"
#include "DlgTGChildGenner.h"
// CdlgTriggerContent 대화 상자입니다.
class CBaseEventAction;
class CBaseCondition;

class CdlgTriggerContent : public CDialog
{
	DECLARE_DYNAMIC(CdlgTriggerContent)

public:
	CdlgTriggerContent(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgTriggerContent();

	// 대화 상자 데이터입니다.
	CSplitterControl m_wndSplitter1;
	enum { IDD = IDD_DIALOG_TRIGGER_CONTENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:

	bool                                    m_bCreated;
	CStatic                                 m_stTGComment;
	CStatic                                 m_stTGContent;
	CReadOnlyEdit                           m_ebTG_Comment;
	CEditTreeCtrl                           m_treeTG;
	CBaseTrigger                            *m_pCurrentTrigger;
	CButton                                 m_btUse;
	CButton                                 m_btActive;
	HACCEL                                  m_hAccelTable;      // 단축키 사용을 위해 사용하는 Accel 핸들
	CDlgTGChildGenner                       m_dlgTGChildGenner; // 트리거 함수 추가용 다이얼로그

	TG_TYPE                                 m_enType_ClipBoard;
	std::vector<CBaseEventAction*>               m_vtEAction_ClipBoard;
	std::vector<CBaseCondition*>                 m_vtCondition_ClipBoard;
	void ClearEActionClipBoard(void);
	void ClearConditionClipBoard(void);
	void CheckMonsterGenToKillMaster(void); // 몬스터 생성 EAction을 Condition에 붙여넣으면 KillMaster가 생성시켜주는 함수

	HTREEITEM m_hCurSelItem;
	HTREEITEM m_hEvent;
	HTREEITEM m_hCondition;
	HTREEITEM m_hAction;
	bool FindSelItem(HTREEITEM hFItem);
	void RemoveSelItem(HTREEITEM hDelItem);
	void ClearSelectItem(void);
	int GetBlankMonsterSlotInTrigger(void);     // m_pCurrentTrigger 안에 있는 비어있는 몬스터 슬롯 리턴.
	int GetBlankObjSlotInTrigger(void);         // m_pCurrentTrigger 안에 있는 비어있는 오브젝트 슬롯 리턴.
	int GetBlankGateObjSlotInTrigger(void);     // m_pCurrentTrigger 안에 있는 비어있는 게이트오브젝트 슬롯 리턴.
	// Event, Condition, Action 중 어떤 타입인지 확인하고 해당타입의 값을 리턴(m_hEvent... 중 하나를..)
	HTREEITEM FindItemType(const HTREEITEM hItem);
	CBaseEventAction *FindEventItem(HTREEITEM hItem);
	CBaseCondition *FindConditionItem(HTREEITEM hItem);
	CBaseEventAction *FindActionItem(HTREEITEM hItem);
	void RefreshTreeTitle(HTREEITEM hItem, std::wstring strTitle);

	bool DelEventItem(HTREEITEM hItem);
	bool DelConditionItem(HTREEITEM hItem);
	bool DelActionItem(HTREEITEM hItem);

	void DoResize1(int delta);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnAddchildQ_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddSiblingQ_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShiftTreeSelect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteSubTree_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnableMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectTrigger(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeItemDClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTgctAddEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTgctAddCondition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTgctAddAction(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTgctAddPopupEvent();
	afx_msg void OnTgctAddPopupCondition();
	afx_msg void OnTgctAddPopupAction();
	afx_msg LRESULT OnTgctModifyEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTgctModifyCondition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTgctModifyAction(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTgctCut();
	afx_msg void OnTgctCopy();
	afx_msg void OnTgctPaste();
	afx_msg void OnTgctDelete();
	afx_msg void OnTgctUse();
	afx_msg void OnTgctStartWithUse();
	afx_msg void OnEnChangeEditTgComment();
	afx_msg void OnTreeCut(void);
	afx_msg void OnTreeCopy(void);
	afx_msg void OnTreePaste(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedCheckUse();
	afx_msg void OnBnClickedCheckActive();
	afx_msg LRESULT OnNMTreeClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeCustomDraw(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnSelchangedTreeTg(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void MoveItem(WORD wKey);
	void TreeCopy(HTREEITEM hSelItem = NULL);
	HTREEITEM TreePaste(HTREEITEM hSelItem = NULL, HTREEITEM hParent = NULL, HTREEITEM hDest = NULL, bool bNoAddName = false);
	HTREEITEM TreeCopyAndPaste(HTREEITEM hSelItem, HTREEITEM hParent, HTREEITEM hDest);
};
