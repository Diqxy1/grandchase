#pragma once
#include "afxwin.h"
#include <d3d9.h>
//#include <strsafe.h>
#include <d3dx9tex.h>
#include "afxcoll.h"
//#include <vector>
// CTextureViewDlg 대화 상자입니다.

struct KD3DWndClass;

class CTextureViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CTextureViewDlg)

public:
	CTextureViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextureViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEXTUREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic			m_ctlTexture;
	CStatic			m_TextureFileName;
	CStringArray	m_strArrayName;
	CString			m_strTexturePath;
	CComboBox*		m_pCombo;

	LPDIRECT3DTEXTURE9		m_pTexture;	
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9       m_pd3dDevice; // Our rendering device
	std::map<int,LPDIRECT3DTEXTURE9>		m_mapTexture;	  // Our texture
	std::vector<CString>				m_vecString;
	LPD3DXSPRITE			m_pSprite;
	LPD3DXSPRITE			m_pSprite2;

	D3DXVECTOR2				m_vStart, m_vEnd;

	int						m_width, m_height;
	double					m_Magnification_X, m_Magnification_Y;
	CPoint					m_nowPoint;
	CPoint					m_RectSize;

	CPoint					m_BtnDownOnRviewPoint; // 20091013 이동석
	CPoint					m_RviewAddPoint;  // 20091013 이동석

	bool	m_bDrag;		// 마우스를 클릭하고 있는 상태

	bool	m_bRDrag;		// 오른쪽 작업영역을 클릭한 상태 20091013 이동석
	
	bool	m_bDraw;		// Rectangle을 그리고 있는 상태
	bool	m_bRect;		// Rectangle을 다 그린 상태
	
	bool	m_bOnMove;		// 마우스가 Rectangle 좌측 상단 영역에 들어가 있는 경우	(클릭하면 위치 이동 가능)
	bool	m_bOnSize;		// 마우스가 Rectangle 우측 하단 영역에 들어가 있는 경우	(클릭하면 크기 조절 가능)
	bool	m_bDoMove;		// 마우스가 Rectangle 좌측 상단 영역을 클릭한 경우 (위치 이동 가능)
	bool	m_bDoSize;		// 마우스가 Rectangle 우측 하단 영역을 클릭한 경우 (크기 조절 가능)
	
	bool	m_KeyPress;		// 윗쪽 방향키를 계속 누르고 있는 상태

	bool	m_bCtrlState;	// Ctrl 를 누르고 있는 상태
	bool	m_bShiftState;	// Shift 를 누르고 있는 상태

	CRect	m_RectDef;
	CRect	m_RectHov;
	CRect	m_RectSel;
	CRect	m_RectAct;
	CRect	m_RectLoc;

	int		m_iDefNum;	
	int		m_iHovNum;
	int		m_iSelNum;
	int		m_iActNum;
	int		m_iLocNum;

	int		m_iRvalue1;
	int		m_iGvalue1;
	int     m_iBvalue1;
	int		m_iRvalue2;
	int		m_iGvalue2;
	int     m_iBvalue2;
	
	//------------------------------------------------------------------------
	//	이동석
	float	m_fMagnification;
	
	int		m_iAdd_x;
	int		m_iAdd_y;
	//////////////////////////////////////////////////////////////////////////
	
	HRESULT			InitD3D( HWND hWnd );
	void			Render(void);
	VOID			RenderWindow();
	BOOL			OnInitDialog();
	void			AddFileName(CString);
	void			GetTextureFilePath(CString);
	void			GetTextureViewRect(CRect &rect);

	afx_msg void	OnPaint();
	afx_msg void	OnDestroy();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnEnChangeStartX();
	afx_msg void	OnEnChangeStartY();
	afx_msg void	OnEnChangeEndX();
	afx_msg void	OnEnChangeEndY();
	afx_msg void	OnBnClickedGomain();
	afx_msg void	OnEnChangeWidth();
	afx_msg void	OnEnChangeHeight();
	afx_msg void	OnBnClickedSetsize();
	afx_msg void	OnBnClickedOriginalSize();
	afx_msg void	OnCbnSelchangeState();
	afx_msg void	OnBnClickedButton2();
	afx_msg void	OnBnClickedChangecolor();
	afx_msg void	OnBnClickedChangecolor2();
	afx_msg void	OnComboSelectChange();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

    void            SetSelectedWindowTextrueRect( KD3DWndClass *pSelectedWnd );
	void			LoadTexture();
	void			UpdataRect(int iState, int iTextureNum, CPoint Start, CPoint End);

private:
	VOID			Cleanup();
	bool			Cursor_InTextureRect();
	bool			Cursor_OnRectMove();
	bool			Cursor_OnRectSize();
	void			GetMagnification();
	void			MouseMove();
	void			SetEditBox();
	void			DrawMoveRect();
	void			DrawSizeRect();
	void			DrawCoordinateAxis();
	void			DrawRectangle(long tx, long ty, long bx, long by);
	void			KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	double			Magnificator(int, long, long);
public:
    CComboBox m_cbTexState;

//추가, 이동석
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    CComboBox m_ctrlComboFindString;
    CString m_strFindString;
    afx_msg void OnCbnEditchangeComboFindText();
    afx_msg void OnCbnSelchangeCombo1();
};
