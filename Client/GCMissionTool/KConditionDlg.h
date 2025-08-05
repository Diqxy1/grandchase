#pragma once
#include "afxwin.h"


// KConditionDlg 대화 상자입니다.

class KConditionDlg : public CDialog
{
	DECLARE_DYNAMIC(KConditionDlg)

public:
	KConditionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KConditionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();
    void InitCombo_Monster();
    void FillControls( int iConditionID );

	DECLARE_MESSAGE_MAP()
protected:
    CString m_Stage;
    CString m_DifficultRange;
    CString m_MapID;
    CString m_ScoreRange;
    CString m_ClearTime;
    CString m_PlayerKill;
    CString m_Die;
    CString m_ExLife;
    BOOL m_bWin;
    CString m_Ratio;
    int m_iConditionID;
    CComboBox m_combo_Monster;
    CComboBox m_combo_GameMode;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    void SetContidionID( int iConditionID );
    int GetConditionID()    { return m_iConditionID; }
};
