#pragma once
//====================================================================================
// 2006.09.27 : Asirion
// DISABLE이 젤 마지막에 오도록.
#define ID_BTNUP		0
#define ID_BTNDOWN		1
#define ID_BTNHOVER		2
#define ID_BTNDISABLE	3

// CNBitmapButton
#include <string>
#include "../kom/kom.h"
#include "ColorStatic.h"


//-----------------------------------------------------------------------------
// CNBitmapButton
//-----------------------------------------------------------------------------
// 설명 : 비트맵 버튼과는 달리 마우스 커서가 버튼 위에 올라갔을 경우
//		  설정한 비트맵을 뿌려 줄 수 있다.
//		  또한 버튼을 클릭했을 경우 SetPush() 로 Push 상태를 변경할때 까지
//		  눌린 상태로 비트맵을 뿌려준다.
//		  툴팁도 지원한다.
//-----------------------------------------------------------------------------
// 변경일 : 2002/09/10
//-----------------------------------------------------------------------------
// 작성자 : 이장원
//-----------------------------------------------------------------------------
class CNBitmapButton : public CButton
{
	DECLARE_DYNAMIC(CNBitmapButton)

public:
	CNBitmapButton();
	virtual ~CNBitmapButton();

protected:
	BOOL m_bHover;	// 마우스 커서가 버튼 위에 있는지 안 있는지
	BOOL m_bPush;	// 눌렸는지 안 눌렸는지
	BOOL m_bToolTip;// 툴팁이 눌렸는지 안 눌렸는지
    BOOL m_bDisable;// 버튼을 비활성화 시킬지 유무
	CBitmap* m_pBitmap[ID_BTNDISABLE + 1];
	CToolTipCtrl m_MyToolTipCtrl;
	BOOL m_bCapture;
	bool m_bNoPress;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
    //UINT nIDUp = 0, UINT nIDDown = 0, UINT nIDFocus = 0, UINT nIDHover = 0, UINT nIDDisable = 0)
	void LoadBitmaps( Komfile * massfile,std::string strBtnName_ );
	void SetPush(bool bPush);
    void SetDisable(bool bDisable);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetToolTipText(CString strMessage);
	void DrawBitmap(UINT nID, CRect rcBtn, CDC* pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void SetNoPress(bool bNoPress){ m_bNoPress = bNoPress;}
	afx_msg void OnCaptureChanged(CWnd *pWnd);
    BOOL GetDisable() { return m_bDisable; }
};

class CNBitmapDecoButton : public CButton
{
	DECLARE_DYNAMIC(CNBitmapDecoButton)

public:
	CNBitmapDecoButton();
	virtual ~CNBitmapDecoButton();

protected:
	CBitmap* m_pBitmap[3];
	CColorStatic m_stName;
	CFont    m_kFont;
	std::map<int, CBitmap*> m_mapDeco;
	std::map<int,std::pair<bool, CRect>> m_mapDecoRenderPos;

//	std::pair<CRect, std::wstring> m_pairText;

	BOOL m_bPush;
	BOOL m_bCapture;
	BOOL m_bHover;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void LoadBitmaps( Komfile * massfile,std::string strBackName, std::map<int,std::pair<std::string,CRect>> mapDecoName_ );
	BOOL OnEraseBkgnd(CDC* pDC);
	void DrawBitmap(UINT nID, CRect rcBtn, CDC* pDC);
	void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClicked();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/);

	void SetDecoRender( int iID, bool bRender );
	void SetName( CRect rt, std::wstring strName );
	void OnBtOff();
	void OnBtOn();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	void OnCaptureChanged(CWnd *pWnd);
};



extern void LoadBitmapFromMassFile( Komfile * massfile,std::string strFileName_, CBitmap** ppBitmap_ );