#pragma once

#include "SplitterControl.h"
#include "DlgTriggerContent.h"
#include "MyTreeCtrl.h"
#include "Category.h"
// CDlgTrigger 대화 상자입니다.
class CDlgStage;

class CDlgTrigger : public CDialog
{
	DECLARE_DYNAMIC(CDlgTrigger)

public:
	CDlgTrigger(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTrigger();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TRIGGER };

	enum LEVEL_TYPE { LEVEL_ROOT = 0, LEVEL_CATEGORY = 1, LEVEL_TG = 2 };

	bool m_bCreated;
	CSplitterControl m_wndSplitter1;
	CdlgTriggerContent m_dlgTriggerContent;

	HACCEL              m_hAccelTable;      // 단축키 사용을 위해 사용하는 Accel 핸들
	bool                m_bClipBoard;
	int                 m_iCopyCnt;         // 클립보드에 있는 트리거 수번 사용시에 쓰는 번호
	CCategory           m_Category_ClipBoard;
	CBaseTrigger        m_BaseTrigger_ClipBoard;

	std::vector<CCategory*>    *m_pvtCategory;
	void RefreshTriggerTree(std::wstring strSelect = L"");
	CCategory *FindCurrentCategory(HTREEITEM hFItem);
	CBaseTrigger *FindCurrentTrigger(HTREEITEM hFItem);
	void DeleteTrigger(HTREEITEM hItem);
	void DeleteCategory(HTREEITEM hItem);

	CDlgStage *m_pDlgStage;
	void SetCallWindowPointer(CDlgStage *pDlgStage);     // 트리거창을 호출한 창의 포인터를 받는다.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void DoResize1(int delta);
	void DoResize2(int delta);
public:
	virtual BOOL OnInitDialog();
	CEditTreeCtrl m_TriggerTree;
	CStatic m_stInfo;
	void FindTGEqualCnt(HTREEITEM hItem, int &iCnt, CString strDefaultName);
	CString GetCheckTGDuplicateTitle(CString strTitle);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnAddchild_Enter_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddSibling_SpaceBar_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShiftTreeSelect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteSubTree_ForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnableMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangedTreeLabel(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTgAddCategory();
	afx_msg void OnTgAddTrigger();
	afx_msg void OnTgCut();
	afx_msg void OnTgCopy();
	afx_msg void OnTgPaste();
	afx_msg void OnTgDelete();
	afx_msg void OnTgUse();
	afx_msg void OnTgStartWithUse();
	afx_msg void OnTreeCut();
	afx_msg void OnTreeCopy();
	afx_msg void OnTreePaste();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnTreeCustomDraw(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeReDrawTextToSelItem(WPARAM wParam, LPARAM lParam);
	BOOL ShowWindow(int nCmdShow);

public:
	std::wstring MoveItem(WORD wKey);

public:
	void TreeCopy(HTREEITEM hSelItem = NULL);
	HTREEITEM TreePaste(HTREEITEM hSelItem = NULL, HTREEITEM hParent = NULL, HTREEITEM hDest = NULL, bool bNoAddName = false);
	HTREEITEM TreeCopyAndPaste(HTREEITEM hSelItem, HTREEITEM hParent, HTREEITEM hDest);
};
