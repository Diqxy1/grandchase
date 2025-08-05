#pragma once

#include "stdafx.h"
#include "Toolobject.h"
#include "CRenderEyeInfo.h"

class GCDeviceTexture;
class CLayerPart;
class CLayerManager;
class CCommandRecv;
class CEffectProp;

// KCreateLayerPartDlg 대화 상자입니다.

class KCreateLayerPartDlg
	: public CDialog
	, public CRenderEyeInfo
{
	DECLARE_DYNAMIC(KCreateLayerPartDlg)

public:
	KCreateLayerPartDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCreateLayerPartDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATELAYERPART };
private:
	LPDIRECT3DSWAPCHAIN9 m_pSwapChain;
	HWND    m_hwndRenderWindow;

	HRESULT FrameMove();

	CLayerPart* m_pPartClone;
	CRITICAL_SECTION m_csSwap;

	std::string m_strNewCategoryName;
	std::string m_strBeforePartName;

	CLayerManager* m_pManager;

	void EnableAllWindow(BOOL bEnable);

public:
	D3DVIEWPORT9 m_vpPrev;
	HRESULT Render();
	bool ThreadKill;
	void SetCategoryMember(std::string CategoryName);
	bool m_bCheckDuplicate;
	void UpdateLayerPart();
	std::string m_strFileName;
	int PartX;
	int PartY;
	int PartWidth;
	int PartHeight;
	int SrcBlendMode;
	int DestBlendMode;

	void ResetView();
	void QuaterZoom(bool Zoom);

	bool m_bIsShiftPushed;
	CPoint m_Point;
	CPoint m_StartPoint;

	int m_iMIndexX;
	int m_iMIndexY;
	int m_iDragStartIndexX;
	int m_iDragStartIndexY;

	void Get3DWorldPos(D3DVECTOR* PosOut);
	void UpdateMouseIndex();
	std::vector< CToolObject > m_vecCurObj;

	bool m_bToolObjectAdd;
	void AddToolObject(int iIndexX, int iIndexY);
	void ClearCurObj(bool bClearAll = true);

	void RenderFH();
	void StampObject();
	void UnStampObject();

	void AddNewCategory(const std::string& strCategoryName);
	void DeleteNewCategory(const std::string& strCategoryName);

	void SetClonePart(CLayerPart* pPart);
	void UpdateControlData();
	void InitControlData();

	CLayerManager* GetLayerManager() { return m_pManager; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
protected:
	HACCEL m_hAccel;
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CCommandRecv* m_pCommandRecv;
	CCommandRecv* GetCommandRecv() { return m_pCommandRecv; }

	void AddLog(const std::string& strLog);
	void ClearLog();

public:
	afx_msg void OnCbnSelchangeComboSrcblend();
	CComboBox m_cbSrcBlend;
	CComboBox m_cbDestBlend;
	afx_msg void OnCbnSelchangeComboDestblend();
	CListBox m_lbCategoryList;

	void SetSwapChain(LPDIRECT3DSWAPCHAIN9 Swap) { m_pSwapChain = Swap; }
	void SetNewCategoryName(std::string Name) { m_strNewCategoryName = Name; }
	CListBox m_lbPartsList;
	afx_msg void OnBnClickedCheckDuplicate();
	CButton m_btnOK;
	CEdit m_editPartName;
	CString m_strPartName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangePartName();
	afx_msg void OnBnClickedSelectTexture();
	CButton m_btnSelectTexture;
	afx_msg void OnEnChangePartX();
	afx_msg void OnEnChangePartY();
	afx_msg void OnEnChangePartWidth();
	afx_msg void OnEnChangeTexHeight();
	CString m_strPartX;
	CString m_strPartY;
	CString m_strPartWidth;
	CString m_strPartHeight;
	afx_msg void OnEnChangePartHeight();

	afx_msg void OnBnClickedAddPart();
	CString m_strCategoryName;
	CButton m_btnAdd;
	CButton m_btnDelPart;
	afx_msg void OnBnClickedDelPart();
	CButton m_btnDelCategory;
	afx_msg void OnBnClickedDelCategory();
	afx_msg void OnBnClickedAddCategory();
	afx_msg void OnLbnSelchangeCategoryList();
	afx_msg void OnLbnSelchangePartsList();
	CButton m_btnModify;
	afx_msg void OnBnClickedModifyPart();
	CEdit m_eLayerWidth;
	float m_fLayerWidth;
	CEdit m_eLayerHeight;
	float m_fLayerHeight;
	CString m_strLayerWidth;
	CString m_strLayerHeight;
	afx_msg void OnBnClickedReverseLeftright();
	afx_msg void OnBnClickedReverseUpdown();
	CButton m_btnReverseLeftRight;
	CButton m_btnReverseUpDown;
	CButton m_btnClone;
	afx_msg void OnBnClickedClone();
	CEdit m_eFHWidth;
	int m_iFHWidth;
	CString m_strFHWidth;
	CEdit m_eFHHeight;
	int m_iFHHeight;
	CString m_strFHHeight;
	CButton m_btnApply;
	afx_msg void OnBnClickedApply();
	CButton m_btnApplyTex;
	CButton m_btnApplyLayer;
	afx_msg void OnBnClickedApplyTexture();
	afx_msg void OnBnClickedApplyLayer();
	afx_msg void OnEnChangeFhWidth();
	afx_msg void OnEnChangeFhHeight();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnAccelReset();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CListCtrl m_listLog;
	afx_msg void OnAccelRedo();
	afx_msg void OnAccelUndo();
	afx_msg void OnEnChangeLayerWidth();
	afx_msg void OnEnChangeLayerHeight();
	afx_msg void OnBnClickedShowParticleInfo();
	CButton m_btnShowParticle;
	CStatic m_cStaticParticleNum;

private:
	bool m_bDrag;
	afx_msg void OnBnClickedBtnTexuv();

public:
	void DrawRectangle(long tx, long ty, long bx, long by);
public:
	//	afx_msg void OnStnClickedPartPreview();
};

extern KCreateLayerPartDlg* g_LayerToolDlg;
