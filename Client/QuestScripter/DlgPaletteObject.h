#pragma once
#include <vector>
#include <map>
#include "afxwin.h"
#include "ACEdit.h"
#include "PaletteStruct.h"
using namespace std;

// CDlgPaletteObject 대화 상자입니다.
class CxSkinButton;

class CDlgPaletteObject : public CDialog
{
	DECLARE_DYNAMIC(CDlgPaletteObject)

public:
	CDlgPaletteObject(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPaletteObject();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PALETTE_OBJECT };

    map<CString, CString>   m_mapObjID;
    vector<TPObject>        m_vtTPObject;

    vector<CxSkinButton*> m_vtButton;
    int ReSizeButton(int iStartVerLine);
    void Refresh(void);
    void SetObjectID(map<CString, CString> &vtString);
    void CreateButtonWindow(vector<TPObject> &vtTPObject);
    void SetObjectImage(vector<TPObject> &vtTPObject);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg LRESULT OnClickToolButton(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCbnSelchangeComboId();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    CACEdit m_cbToolPalette;
    virtual BOOL OnInitDialog();
};
