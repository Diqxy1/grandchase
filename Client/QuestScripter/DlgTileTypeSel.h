#pragma once
#include "xSkinButton.h"
//#include <map>
//#include <vector>

// CDlgTileTypeSel 대화 상자입니다.

class CDlgTileTypeSel : public CDialog
{
	DECLARE_DYNAMIC(CDlgTileTypeSel)

public:
	CDlgTileTypeSel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTileTypeSel();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TILE_TYPE_SELECT };

	CxSkinButton        m_btSkinButton;
	CSpinButtonCtrl     m_spinType;

	int                 m_iCurIndex;
	std::map<int, CString>   m_mapBmpFilePath;
	std::vector<int>         m_vtSequence;

	void NextIndex(void);
	void PrevIndex(void);
	void SetButtonImage(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
