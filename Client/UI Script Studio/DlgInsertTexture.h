#pragma once

#include "../MassFileLib/GCMassFile.h"
// DlgInsertTexture 대화 상자입니다.

class DlgInsertTexture : public CDialog
{
	DECLARE_DYNAMIC(DlgInsertTexture)

public:
	DlgInsertTexture(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgInsertTexture();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INSERT_TEXTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CComboBox*		m_pKomFileList;
	GCMassFile		m_MassFile;

	afx_msg void OnBnClickedOk();
};
