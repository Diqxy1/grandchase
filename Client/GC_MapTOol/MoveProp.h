#pragma once
#include "stdafx.h"

// CMoveProp 대화 상자입니다.

class CMoveProp : public CDialog
{
	DECLARE_DYNAMIC(CMoveProp)

public:
	CMoveProp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMoveProp();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOVE_PROP };

	void SetControlValue(CLayerPart* pPart);
	void UpdateControlValue();
	void UpdateSelectPart(CLayerPart* pPart);
	void DisableAllProperty(BOOL bDisable);

	void Enable(BOOL bEnable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL m_bMoveable;
	CButton m_btnMoveable;
	CEdit m_eXSpeed;
	CEdit m_eYSpeed;
	float m_fXSpeed;
	float m_fYSpeed;
	CString m_strXSpeed;
	CString m_strYSpeed;
	CButton m_btnPosResetX;
	CButton m_btnPosResetY;
	BOOL m_bPosResetX;
	BOOL m_bPosResetY;
	BOOL m_bScroolX;
	BOOL m_bScroolY;
	afx_msg void OnBnClickedMoveableTrue();
	afx_msg void OnBnClickedMoveableFalse();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApply();
public:
	//	afx_msg void OnBnClickedScroolx();
public:
	//	afx_msg void OnBnClickedScroolx();
public:
	//	afx_msg void OnBnClickedScrooly();
};
