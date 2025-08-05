#pragma once
#include "stdafx.h"

// CParticleListDlg 대화 상자입니다.

class CLayerPart;

class CParticleListDlg : public CDialog
{
	DECLARE_DYNAMIC(CParticleListDlg)

public:
	CParticleListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CParticleListDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PARTICLE_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CLayerPart* m_pCloneLayerPart;

	DECLARE_MESSAGE_MAP()
public:
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
	virtual BOOL OnInitDialog();
	void SetControlValue(CLayerPart* pPart);
	CButton m_kAddButton;
	CButton m_kDeleteButton;
	CListCtrl m_kListParticle;
	afx_msg void OnBnClickedButtonAddParticle();
	afx_msg void OnBnClickedButtonDeleteParticle();
};
