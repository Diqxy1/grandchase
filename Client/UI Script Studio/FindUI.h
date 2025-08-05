#pragma once


// CFindUI 대화 상자입니다.

class CFindUI : public CDialog
{
	DECLARE_DYNAMIC(CFindUI)

public:
	CFindUI(CWnd* pParent = NULL);   // 표준 생성자입니다.
    CFindUI(CWnd* pParent = NULL, bool IsTreeViewWindow = true );
	virtual ~CFindUI();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FINDUI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedFind();

private:
    CTreeCtrl           *m_pCtlTreeView;
    CListCtrl           *m_pListCtrlView;
    bool                m_bTreeviewWindow;

public:
    void SetFocuse();
    void SetTreeView( CTreeCtrl *CtlTreeView )    { m_pCtlTreeView = CtlTreeView; }
    void SetListView( CListCtrl *CtListView  )    { m_pListCtrlView = CtListView; }
    void FindTreeViewWindows( void );
    void FindListViewWindows( void );
};
