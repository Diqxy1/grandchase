#pragma once
#include "afxwin.h"
#include <d3d9.h>
//#include <strsafe.h>
#include <d3dx9tex.h>
#include "afxcoll.h"
//#include <vector>



struct KD3DWndClass;

class CMagnifiCationDlg: public CDialog
{
	DECLARE_DYNAMIC(CMagnifiCationDlg)

public:
	CMagnifiCationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMagnifiCationDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_Magnification};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
    
	BOOL			OnInitDialog();	
	afx_msg void	OnPaint();
    afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
    void SetRenderState(bool brender);
    bool GetRenderState();
    void SetEdgeValue(int top, int left);
    void CaptureScreen();
    void SetScale(int scale);
    void SetZoom(int zoom);
    const int GetScale();

private:
    CPoint* m_MousePoint;
	CPoint m_nowPoint;
    CDC* m_pDc;
    CRect m_DrawScaleRect;

    bool m_bOnRender;
    int m_iMagnification;
    int m_iLeftEdge;
    int m_iTopEdge;
};
