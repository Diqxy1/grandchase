/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

★ 설계자 ☞ 이상호
★ 설계일 ☞ 2010년 11월 17일
★ E-Mail ☞ shmhlove@naver.com
★ 클래스 ☞ 윈도우 리전

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/

#if !defined(AFX_RGNDLG_H__INCLUDED_)
#define AFX_RGNDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// CRGNDlg dialog
class CRGNDlg : public CDialog
{
public:
	CRGNDlg(UINT nIDTemplate,CWnd* pParent, HINSTANCE hInstance);
	~CRGNDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
    float    m_xScale;
	float    m_yScale;

    HINSTANCE m_hInstance;
	HRGN     m_dialogrgn;
	CDC      m_dc;
	BOOL     m_bTrackMove;
	CPoint   m_ptMouse, 
		     m_ptLast;

	void InvertTracker(CPoint point);
	// Generated message map functions
	//{{AFX_MSG(CRGNDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif 
