#pragma once

//#include <string>
//#include <vector>
#include "StdAfx.h"
#include "CRenderEyeInfo.h"
#include "LayerPart.h"

class KGridGuide;

class CStaticRenderView
	: public CStatic
	, public CRenderEyeInfo
{
	DECLARE_DYNAMIC(CStaticRenderView)

private:
	bool IsShiftPushed;
	bool IsLDownShiftPushed;

	int m_iMIndexX;
	int m_iMIndexY;

	int m_iDragStartIndexX;
	int m_iDragStartIndexY;

	CPoint m_Point;
	CPoint m_StartPoint;
	CLayerPart *m_pCursorPart;
	CLayerPart *m_pDragPart;

	KGridGuide *m_pCurGridGuide;

	int m_iDragStartPosX;
	int m_iDragStartPosY;
	int m_iDragObjectPosX;
	int m_iDragObjectPosY;
	int m_iDragObjectWidth;
	int m_iDragObjectHeight;

	CMenu m_NewMenu;
	std::vector< CLayerPart* > m_vecPart;

	bool IsAltPushed;
	bool m_bNeedReload;
	bool m_bToolObjectAdd;

	KLayerRect<float>      m_rtStart;
	KLayerRect<float>      m_rtEnd;

private:
	void AddAndReload_(IN CLayerPart* pTargetObj, IN CLayerPart* pCloneObj);
	void AddObject_(IN CLayerPart* pTargetObj, IN CLayerPart* pCloneObj);
	void ReloadLayer_(IN POSITION& pos);

public:
	CStaticRenderView();
	virtual ~CStaticRenderView();

	bool EnableHand() const { return IsShiftPushed; }
	void SetEnableHand(bool Hand) { IsShiftPushed = Hand; }

	bool GetAltPushed() const { return IsAltPushed; }
	void SetAltPushed(bool bAlt) { IsAltPushed = bAlt; }

	bool GetToolObjectAdd() const { return m_bToolObjectAdd; }
	void SetToolObjectAdd(bool bAdd) { m_bToolObjectAdd = bAdd; }

	void QuaterZoom(bool Zoom);

	void RestoreView();

	void CreateCursorPart(std::string PartName);
	CLayerPart* GetCursor() { return m_pCursorPart; }
	CLayerPart* GetDragPart() { return m_pDragPart; }
	void ClearCursor();
	void Clear();
	void ClearDragPart() { m_pDragPart = NULL; }
	void ClearCurGridGuide() { m_pCurGridGuide = NULL; }
	void SetCurGridGuide(KGridGuide* pGuide) { m_pCurGridGuide = pGuide; }
	void DeleteCurGridGuide();
	void RenderCurGridGuide();
	void SetCursorPos();

	void OnFHDrag();

	void ResetView();

	void Get3DWorldPos(D3DVECTOR* PosOut);
	void UpdateMouseIndex();
	void AddToolObject(int iIndexX, int iIndexY);

	int GetMouseIndexX() const { return m_iMIndexX; }
	int GetMouseIndexY() const { return m_iMIndexY; }

	void MakeFootHoldImagePrepare(float fX, float fY);
	void MakeFootHoldImage(const char* strName);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
