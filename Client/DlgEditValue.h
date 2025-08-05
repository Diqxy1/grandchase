#pragma once
#include "afxwin.h"
#include "MinMax.h"
#include "KTDGParticleSystem.h"

class CDlgEditValue : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditValue)

public:
	CDlgEditValue(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgEditValue();

	void SetValue( CKTDGParticleSystem::EventType eType, CString strValue, BOOL bFade );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VALUEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void SetHelpText();	

private:
	CKTDGParticleSystem::EventType m_eType;
public:
	CString m_strValue;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	BOOL m_bFade;

	CString m_strTitle[4];	
	CEdit m_editMin[4];
	CEdit m_editMax[4];
	float m_fMin[4];
	float m_fMax[4];
	
	
	afx_msg void OnBnClickedMin();
	afx_msg void OnBnClickedMax();
};

extern char* EDIT_VALUE_TYPE_DEFAULT[CKTDGParticleSystem::EVENT_TYPE_NUM+1];