#pragma once
#include "afxcmn.h"
#include "KWndManager.h"


// KUIBringingDlg 대화 상자입니다.

class KUIBringingDlg : public CDialog
{
	DECLARE_DYNAMIC(KUIBringingDlg)

public:
	KUIBringingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KUIBringingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BRINGING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	// 엑셀레이터
	HACCEL			m_hAccel;
	KD3DWndClass*	m_pClipBoard;	// 공용 클립보드

	// 왼쪽
	KWndManager *m_pOrgWndMgr;		// 상위(UI Script StudioDlg) g_WndMgr와 같은 포인터
	KWndManager *m_pLeftWndMgr;		// 좌측 윈도우메니져	
	CTreeCtrl m_CtrlLeftTreeView;	// 왼쪽 트리 컨트롤러
	CString m_strLeftFilePath;		// 왼쪽 파일(경로포함)

	// 오른쪽
	KWndManager	*m_pRightWndMgr;	// 우측 윈도우메니져
	CTreeCtrl m_CtrlRightTreeView;	// 오른쪽 트리 컨트롤러
	CString m_strRightFilePath;		// 오른족 파일(경로포함)


	void InsertChild(KD3DWndClass* pkWnd, HTREEITEM hParent, CTreeCtrl *pCtrlTreeView, HTREEITEM hInsertAfter = TVI_LAST);
	void Release();
	void LoadWndData(KD3DWndClass* pSrcWnd, KD3DWndClass* pCopyWnd);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonFileOpen();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnNMRclickCompareLeftTreeview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickCompareRightTreeview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeCopy();
	afx_msg void OnTreePaste();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnRightTreeCopy();
	afx_msg void OnTreeDelete();
	afx_msg void OnTreeInsert();
};
