#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// KSubMissionDlg 대화 상자입니다.

class KSubMissionDlg : public CDialog
{
	DECLARE_DYNAMIC(KSubMissionDlg)

public:
	KSubMissionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSubMissionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SUBMISSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:    
    CListCtrl m_Conditions;
    CEdit m_Action_Count;
    CEdit m_Action_Item;
    CEdit m_Completion;
    CEdit m_C_Count;
    CButton m_bMonDrop;
    CEdit m_C_Item;


    int     m_iSubMissionID;
    std::vector< int > m_vecConditions;    

    void RefreshConditionList();
    void RefreshControls();
    void InitConditionList();
    bool SetSubMissionID( int iSubMissionID );

public:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedButton1();    
    afx_msg void OnLvnKeydownListCondition(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnAItem();
    afx_msg void OnBnClickedBtnCItem();
    CEdit m_edit_desc;
    afx_msg void OnNMDblclkListCondition(NMHDR *pNMHDR, LRESULT *pResult);
};
