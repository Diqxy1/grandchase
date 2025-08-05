#if !defined(AFX_MYTREECTRL_H__E490FBBC_0C30_11D8_9063_00409533E092__INCLUDED_)
#define AFX_MYTREECTRL_H__E490FBBC_0C30_11D8_9063_00409533E092__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTreeCtrl.h : header file
//
#include "TreeType.h"
//#include "TreeEditorDoc.h"

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#pragma warning(disable:4786)
//#include <vector>
//#include <algorithm>
#include <functional>

typedef struct
{
	int iLevel;
	CString str;
} TREE_RECORD;

typedef struct
{
	HTREEITEM hti;
	HTREEITEM *pChildrenOfParent;
	HTREEITEM htiLoadedItem;
	int iSizeChildrenOfParent;
} LOAD_ABOVE;

int SearchLoadAboveItem(LOAD_ABOVE &la, HTREEITEM hti);
int CALLBACK CompareForLoadAbove(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

typedef std::vector<TREE_RECORD> TREE_RECORD_VECTOR;
typedef std::vector<CString> CSTRING_VECTOR;

/////////////////////////////////////////////////////////////////////////////
// CEditTreeCtrl window

// Redifine TVI_ROOT and TVI_FIRST as constants for clarity (when we use these
// constants with a different meaning).
static const HTREEITEM HTI_ENTIRE_TREE = TVI_ROOT;
static const HTREEITEM HTI_SUBTREE = TVI_FIRST;

static const int SCROLL_SPEED = 100;
static const int MAX_ASCII_RECORD = 256;
static const int MAX_LEVELS = 40;

enum SORT_TYPE
{
	ST_ALPHABETIC,
	ST_NUMERIC,
};

enum OUTSIDE_PARAM
{
	OP_NONE,
	OP_ENTIRE_TREE,
	OP_SUBTREE,
};

enum
{
	CUR_COPY_ROOT,
	CUR_COPY_SON,
	CUR_MOVE_ABOVE,
	CUR_COPY_ABOVE,
	CUR_MOVE_ROOT,
	CUR_MOVE_SON,
	CUR_NODROP,
	CUR_COUNT,
};

class CEditTreeCtrl : public CTreeCtrl
{
	// Construction
public:
	CEditTreeCtrl();

	typedef void (CEditTreeCtrl::*PARSING_FUNCTION)(HTREEITEM hti, void *pvParam);

	// Attributes
public:
	//	CTreeEditorDoc *m_pted;

	SORT_TYPE m_SortType;
	UINT m_uLimitText;
	BOOL m_bDragging;
	WORD m_wOldKey;
	HTREEITEM m_hitemDrag;
	HTREEITEM m_hitemDrop;
	HCURSOR m_hCursor;
	HCURSOR m_ahCursor[CUR_COUNT];
	CTreeType m_ttTransfer;
	CImageList *m_pimagelist;
	BOOL m_bTimerABOVE_Active;
	BOOL m_bTimerBELOW_Active;
	UINT m_uOldFlags;

	// Operations
public:

	CString m_strBeforeLabel;
	void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

	void InsertLoadItemBeforeHti(LOAD_ABOVE &la);
	void SortSubtree();
	void OnLoadAbove(HTREEITEM hti);
	int GetLevelTree(HTREEITEM hti);
	void GetFromClipboard(CString &fromClipboard);
	void Add2Clipboard(CString *pstrData, int iSize);
	void SaveCsvTabTreeToClipboard(CTreeType &tt);
	BOOL PasteEntireTreeFromCliboard(CTreeType &tt);
	void pfCollapseAndSort(HTREEITEM hti, void *);
	void CEditTreeCtrl::GenerateFirstDirectoryRecords(const char *pszFirstRecord,
		CSTRING_VECTOR &cvFirstDirRecords);
	//	BOOL LoadCsvTreeFromMemory(TREE_RECORD_VECTOR &vtr, CTreeType &tt, int iMaxLevel,
	//			FILE_FORMAT ff);
	void FindPath(const char *lpszPath, CSTRING_VECTOR &cv, BOOL bCalledFromOutside = false);
	BOOL LoadTreeFromDirectory(const char *pszFileName, CTreeType &tt);
	int CalculateMaxLevel(CTreeType &tt);
	BOOL SaveCsvTreeToFile(const char *pszFileName, CTreeType &tt);
	BOOL ValidCsvRecord(char *psz, int &iMaxLevel, char *pszDelimiter, int iLevel);
	BOOL LoadCsvTreeFromFile(const char *pszFileName, CTreeType &tt);
	BOOL DeleteItemM(HTREEITEM hItem);
	BOOL ExpandM(HTREEITEM hItem, UINT nCode);
	BOOL LoadTreeDataR(CTreeType &tt, HTREEITEM hti, OUTSIDE_PARAM op = OP_NONE);
	void RestoreStates(CTreeType &tt);
	int CountSubtree(HTREEITEM hti, OUTSIDE_PARAM op = OP_NONE);
	void CutSubtree();
	BOOL InitializeImageList();
	BOOL SelectVisibleItem(HTREEITEM hti);
	HTREEITEM SelectDropCursor(CPoint *pPoint = NULL);
	int CalculateIndentLevel(char *psz, char ch);
	void ParseTree(PARSING_FUNCTION pf, HTREEITEM hti = TVI_ROOT,
		void *pvParam = NULL);
	void OnButtonUp();
	void pfCollapse(HTREEITEM hti, void *);
	void pfExpand(HTREEITEM hti, void *);
	BOOL SaveTreeData(CTreeType &tt, HTREEITEM hti, OUTSIDE_PARAM op = OP_NONE);
	void SetLimitText(UINT uLimitText);
	void SaveTreeToFile(const char *pszFileName, CTreeType &tt);
	BOOL SaveIndentTreeToFile(const char *pszFileName, CTreeType &tt);
	BOOL LoadIndentTreeFromFile(const char *pszFileName, CTreeType &tt);
	BOOL LoadTreeFromFile(const char *pszFileName, CTreeType &tt);
	BOOL LoadTreeData(CTreeType &tt, HTREEITEM hti, OUTSIDE_PARAM op = OP_NONE);
	void OnContextMenu(CWnd* pWnd, CPoint point);
	void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnRename();
	void OnAddChild();
	void OnAddSibling();
	void OnDeleteItem();
	void OnDeleteAllItems();
	void OnCollapse();
	//	void OnLoadEntireTree(FILE_FORMAT ff, CTreeType &tt);
	//	void OnSaveEntireTree(FILE_FORMAT ff, CTreeType &tt);
	void OnLoadSubtree(HTREEITEM hti);
	void OnSaveSubtree();
	void OnExpandAllLevelsSubtree();
	void OnExpandAllTree();
	void OnExpandOneLevelSubtree();
	void OnExpandOneLevelTree();
	BOOL IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent);
	HTREEITEM CEditTreeCtrl::InsertItemAndSetImage(LPCTSTR lpszItem,
		HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEditTreeCtrl)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bRenameMode;
	HTREEITEM m_hReNameItem;
	bool m_bMoveToOtherLevel;
	CString m_strOriginalName;
	std::vector<HTREEITEM> m_vtSelectedItem;
	HTREEITEM AddChild(CString str, HTREEITEM hDest = TVI_LAST);
	HTREEITEM AddSibling(CString str, HTREEITEM hDest = TVI_LAST);
	void AllowMoveToOtherLevel(void);
	void AllowMoveToSameLevel(void);
	void SetAllowMoveToOtherLevel(bool bOtherLevel);
	bool GetAllowMoveToOtherLevel(void);
	BOOL Select(HTREEITEM hItem, UINT nCode, bool bRefresh = 1);

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditTreeCtrl)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCountEntireTree();
	afx_msg void OnCountSubtree();
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCalculateLevel();
	afx_msg void OnSortSubtreeAlphabetic();
	afx_msg void OnSortSubtreeNumeric();
	afx_msg void OnAddchildQ();
	afx_msg void OnSiblingchildQ();
	afx_msg void OnRemarkQ();
	afx_msg void OnDeletesubtreeQ();
	afx_msg void OnDeleteentiretreeQ();
	//}}AFX_MSG
	//BOOL m_bInsertionMode;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCollapseAllEntireTree();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMRdblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTREECTRL_H__E490FBBC_0C30_11D8_9063_00409533E092__INCLUDED_)
