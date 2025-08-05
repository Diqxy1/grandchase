#pragma once
#include "stdafx.h"
#include "resource.h"
#include "struct.h"
#include "xSkinButton.h"

// CDlgTileInfo 대화 상자입니다.
struct Stage;

class CDlgTileInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgTileInfo)

public:
	CDlgTileInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTileInfo();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TILE_INFO };

	TileMap m_TileMap;
	void SetValue(TileMap &tilemap) { m_TileMap = tilemap; }
	TileMap GetValue() { return m_TileMap; }
	bool CheckGateFlag(Stage *pStage, int iFlag);
	void Refresh(void);

	void ComboBoxReset(void);

	CxSkinButton        m_btSkinButton;
	CSpinButtonCtrl     m_spinType;

	enum ARROW_COLOR { NORMAL, START, BOSS, HIDEN, PLATFORMER, NOSTAGE };
	CString GetArrowPath(ARROW_COLOR arrow_color, int iIndex);

	int                 m_iCurIndex;
	std::map<int, CString>   m_mapArrow_Black;
	std::map<int, CString>   m_mapArrow_Red;
	std::map<int, CString>   m_mapArrow_Green;
	std::map<int, CString>   m_mapArrow_Silver;
	std::map<int, CString>   m_mapArrow_Blue;

	void NextIndex(void);
	void PrevIndex(void);
	void SetButtonImage(void);
	void LoadDirectionDefScript(int iDirection, Stage *pStage);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox   m_cbStage;
	CComboBox   m_cbProperty;
	CButton     m_chNewStage;
	CComboBox   m_cbStageMap;

	CString     m_strOriStageMap;
	void SetDefaultStageMap(void);

	void RefreshMapDriectory(void);
	int GetStageIndexInComboBox(int iIndex);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1Init();
	afx_msg void OnBnClickedButtonTgEditor();
	afx_msg void OnBnClickedCheckNew();
	afx_msg void OnCbnSelchangeComboStagemap();
	afx_msg void OnCbnSelchangeComboStage();
};
