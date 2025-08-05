#pragma once
//#include "QuestScripter.h"
//#include "stdafx.h"
#include "resource.h"
#include "struct.h"
//#include <vector>
//#include <map>

class CxSkinButton;
// CDlgTileBG 대화 상자입니다.

class CDlgTileBG : public CDialog
{
	DECLARE_DYNAMIC(CDlgTileBG)

public:
	CDlgTileBG(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTileBG();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TILE_BG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	bool                    m_bDirectionMode;       // 버튼에 방향을 표시해주는 모드 인가?
	int                     m_iRow;
	int                     m_iCol;
	std::vector<CxSkinButton*>   m_vtButton;
	std::vector<TileMap>         m_vtTileMap;
	void SetRowColTileMap(int iRow, int iCol, TileMap tilemap);
	void SetTooltipTileMap(CxSkinButton *pButton, TileMap tilemap);
	void CreateButtons(int iRow, int iCol);
	bool CopyToQuest(Quest *quest);
	bool CopyFromQuest(Quest *quest);
	void DestroyButtons(void);
	void ReSizeButton(void);
	void SetButtonImage(CxSkinButton *pButton, CString strFileName);
	bool ExistenceStage(int iStage);
	bool ExistenceStageInTileMap(int iStage);
	void DeleteStage(int iStage);
	void SetDirectionMode(void);
	void SetStageMode(void);
	bool IsSideArrowToHere(int iDirection, int iRow, int iCol);     // 주변의 화살이 나를 향해 있나 확인
	bool IsHaveStartStage(void);
	TileMap *GetTileMap(int iStage);
	TileMap *GetTileMap(int iRow, int iCol);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HRESULT OnClickTileButton(WPARAM wParam, LPARAM lParam);
};
