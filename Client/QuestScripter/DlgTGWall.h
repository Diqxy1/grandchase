#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "Wall.h"


// CDlgTGWall 대화 상자입니다.

class CDlgTGWall : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGWall)

public:
	CDlgTGWall(CWnd* pParent = NULL);
	virtual ~CDlgTGWall();

	enum { IDD = IDD_DIALOG_TG_WALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
    CWall m_TGWall;

public:
    void InitValue(void) { m_TGWall.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CWall*)pValue = m_TGWall; }
    void SetValue(CBaseTGFunction *pValue) { m_TGWall = *(CWall*)pValue; }
    void Refresh(void);
    void CalcResult(void);

public:
    CEdit m_ebWallType;
    CEdit m_ebWallSlot;
    CEdit m_ebPosX;
    CComboBox m_cbIsDestroy;
    CComboBox m_cbIsCheckInBound;
};
