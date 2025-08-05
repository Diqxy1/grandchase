// GCMissionToolDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CGCMissionToolDlg 대화 상자
class CGCMissionToolDlg : public CDialog
{
// 생성
public:
	CGCMissionToolDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_GCMISSIONTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    void InitListControl();
    void RefreshList();
    void InitComboBox();
    void AddMissions( std::string strFileName );
    void AddSubMissions( std::string strFileName );

public:
    CComboBox sortCharKind;
    CComboBox sortMissionKind;
    CEdit sortStatic;
    std::wstring m_strFindText;
    afx_msg void OnBnClickedButton2();
    CListCtrl kMissionList;
    afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnSave();
    afx_msg void OnEnChangeEdit1();
//    afx_msg void OnCbnEditchangeCombo1();
//    afx_msg void OnCbnEditupdateCombo1();
    afx_msg void OnCbnSelchangeCombo1();
    afx_msg void OnBnClickedLoadExcel();
};
