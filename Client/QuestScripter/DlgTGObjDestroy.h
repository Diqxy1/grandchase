#pragma once
#include "stdafx.h"
#include "ACEdit.h"
#include "ObjDestory.h"
#include "DlgTGBase.h"

// CDlgTGObjDestroy 대화 상자입니다.

class CDlgTGObjDestroy : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGObjDestroy)

public:
	CDlgTGObjDestroy(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGObjDestroy();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_IS_DESTROYED_OBJECT };
    CObjDestory m_ObjDestroy;
    void InitValue(void) { m_ObjDestroy.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CObjDestory*)pValue = m_ObjDestroy; }
    void SetValue(CBaseTGFunction *pValue) { m_ObjDestroy = *(CObjDestory*)pValue; }
    void Refresh(void);
    void CalcResult(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    CComboBox m_cbSlot;
    CACEdit m_cbType;
    CStatic m_stObjDestroy;
    CStatic m_stObjDestroy2;
};
