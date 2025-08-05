#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "Loading.h"
#include "ACEdit.h"
//#include <map>
// CDlgTGLoading 대화 상자입니다.

class CDlgTGLoading : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGLoading)

public:
	CDlgTGLoading(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGLoading();

    CLoading m_Loading;
    void InitValue(void) { m_Loading.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CLoading*)pValue = m_Loading; }
    void SetValue(CBaseTGFunction *pValue) { m_Loading = *(CLoading*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    std::map<CString, int> m_mapQuestEnum;
    
    // 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_LOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    CComboBox m_cbOnOff;
    //CEdit m_ebLoadingTitleFileName;
    CACEdit m_cbLoad_Enum;
    CStatic m_stLoading1;
    CStatic m_stLoading2;
    CStatic m_stLoading3;
};
