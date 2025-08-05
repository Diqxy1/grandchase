#pragma once

#include "stdafx.h"

class CLayerPart;

// CLayerList 대화 상자입니다.

class CLayerList : public CDialog
{
	DECLARE_DYNAMIC(CLayerList)

public:
	CLayerList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLayerList();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LAYERLIST };

	void ReloadObjectList(POSITION CurSel);
	void ReloadObjectList(int iIndex);
	std::string GetPartName(CLayerPart* pPart);
	void ReloadLayerList();
	CListCtrl* GetLayerControl() { return &m_lcLayerList; }
	CListCtrl* GetObjectControl() { return &m_lcObjectList; }
	void DeleteAllItems();
	bool GetCurLayer(int& iLayer);
	POSITION GetCurLayerPos() { return m_lcLayerList.GetFirstSelectedItemPosition(); }
	POSITION GetCurObjectPos() { return m_lcObjectList.GetFirstSelectedItemPosition(); }
	void ReloadCurLayerListObjectList();
	void CopyObject();
	void PasteObject();
	void PasteCurObject();
	CLayerPart* GetCurItemData(OUT int* pIRet = NULL);

	void DisableAllProperty(BOOL bDisable);
	void Reset();

private:
	CLayerPart* m_pCopyObject;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_lcLayerList;
	CListCtrl m_lcObjectList;
	CButton m_btnResize;
	CButton m_btnLayerChange;
	CButton m_btnLayerAdd;
	CButton m_btnLayerDel;
	afx_msg void OnNMClickLayerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedObjectMove();
	afx_msg void OnBnClickedObjectDel();
	afx_msg void OnBnClickedLayerAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedLayerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedLayerDel();
	afx_msg void OnNMClickObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedLayerResize();
	afx_msg void OnBnClickedChangeLayer();
	afx_msg void OnBnClickedZOrderUp();
	afx_msg void OnBnClickedZOrderDown();
	afx_msg void OnBnClickedObjectClone();
	afx_msg void OnLvnItemchangedObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedReverseLeftright();
	afx_msg void OnBnClickedReverseUpdown();
	CButton m_btnObjectDel;
	CButton m_btnObjectClone;
	CButton m_btnObjectInverse1;
	CButton m_btnObjectInverse2;
	CButton m_btnObjectMove;
	CButton m_btnZUp;
	CButton m_btnZDown;

	void ChangeLayer(int iFirst, int iSecond);
};
