#pragma once
#include "stdafx.h"

#include "resource.h"

class CDownLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownLoadDlg)

public:
	CDownLoadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDownLoadDlg();

	void SetText(CString str);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DOWNLOAD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();            
	DECLARE_MESSAGE_MAP()
public:
    
    CStatic StrState;
	
	void OnError( std::string url,int curlcode,int httpcode );

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCancel();	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
