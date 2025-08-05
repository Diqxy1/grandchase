#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// KMissionEditDlg 대화 상자입니다.

class KMissionEditDlg : public CDialog
{
	DECLARE_DYNAMIC(KMissionEditDlg)
public:
	KMissionEditDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMissionEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MISSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    virtual BOOL OnInitDialog();

public:
    // 미션에 대한 함수들 입니다.
    void SetMissionID( DWORD dwMissionID ) { m_dwMissionID = dwMissionID; }

protected:
    void SetMissionData( DWORD dwMissionID = 0);
    void InitSubMissionList();
    void InitRewardList( CListCtrl& kList );
    void RefreshSubMissionList();
    void AddRewardItem( CListCtrl& klist, std::vector< std::pair< DWORD, int > >& vecRewards );

private:
    bool    m_bAdd;
    DWORD   m_dwMissionID;
    std::vector<int> m_vecSubMissionID;
    std::vector< std::pair< DWORD, int > > m_vecReward;
    std::vector< std::pair< DWORD, int > > m_vecSelReward;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    CEdit m_edit_Inherit;
    CEdit m_edit_Level;
    CEdit m_edit_Promotion;
    CEdit m_edit_Time;
    CEdit m_edit_TargetMission;
    CEdit m_edit_Overlap;
    CEdit m_edit_Title;
    CEdit m_edit_Desc;
    CListCtrl m_list_Submission;
    CEdit m_edit_GP;
    CEdit m_edit_Exp;
    CListCtrl m_list_Reward;
    CListCtrl m_list_SelReward;
    CEdit m_edit_NewPromotion;
    CListBox m_MissionID;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnClose();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnBnClickedButtonAddsubmission();
    afx_msg void OnNMDblclkListSubmission(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonAdditem();
    afx_msg void OnBnClickedButtonAdditem2();
    afx_msg void OnLvnKeydownListReward(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnKeydownListSelectreward(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnKeydownListSubmission(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButton6();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedBtnCmission();
    afx_msg void OnBnClickedBtnImission();
    CComboBox m_combo_Character;
    CComboBox m_combo_NewChar;
};
