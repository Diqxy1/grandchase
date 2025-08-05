#pragma once
//#include <map>
//#include <vector>
#include "stdafx.h"
#include "DlgPaletteContent.h"
#include "BaseTrigger.h"

// CToolPalette 대화 상자입니다.

class CToolPalette : public CDialog
{
	DECLARE_DYNAMIC(CToolPalette)

public:
	CToolPalette(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CToolPalette();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TOOL_PALETTE };

    TG_TYPE                 m_Type;
    bool                    m_bObject;
    bool                    m_bPlayer;
    bool                    m_bMonster;
    bool                    m_bDoor;
    bool                    m_bDamage;
    CDlgPaletteContent      m_dlgPaletteMon;
    CDlgPaletteContent      m_dlgPalettePlayer;
    CDlgPaletteContent      m_dlgPaletteObject;
    CDlgPaletteContent      m_dlgPaletteDoor;
    CDlgPaletteContent      m_dlgPaletteDamage;
    CACEdit                 m_cbTypeEAction;
    CACEdit                 m_cbTypeCondition;
    CButton                 m_ckObject;
    CButton                 m_ckPlayer;
    CButton                 m_ckMonster;
    CButton m_ckDamage;
    void SetType(TG_TYPE enType, int iFuncType);
    void RefreshCheckBoxs(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnCbnSelchangeComboType();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnClickToolButton(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedCheckObject();
    afx_msg void OnBnClickedCheckPlayer();
    afx_msg void OnBnClickedCheckMonster();
    BOOL ShowWindow(int nCmdShow);
    afx_msg void OnBnClickedCheckDamage();
};
