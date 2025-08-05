#pragma once
#include "stdafx.h"
#include "struct.h"
//#include <vector>
using namespace std;

// CDlgStruct 대화 상자입니다.

class CDlgStruct : public CDialog
{
	DECLARE_DYNAMIC(CDlgStruct)

public:

    enum UsingType { ADD_CHILD_TYPE, ADD_SIBLING_TYPE };

	CDlgStruct(UsingType type, NamesOfLevel NOL, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgStruct();

    UsingType m_type;
    NamesOfLevel m_ParentNOL;


    CString m_strResult;
    NamesOfLevel m_NamesOfLevel;
    void Clear();
    void AddStruct(CString str, NamesOfLevel NOL);
    void DelStruct(CString str);
    void AddChildType(void);
    void AddSiblingType(void);
    bool SetComboBoxByTreeLevel_Child(int iLevel);


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STRUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_cbStruct;
    vector<NamesOfLevel> m_vtStruct;
    afx_msg void OnBnClickedOk();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL OnInitDialog();
};
