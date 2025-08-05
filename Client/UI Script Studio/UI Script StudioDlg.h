// UI Script StudioDlg.h : header file
//

#include "afxwin.h"
#include "afxcmn.h"
#include "TextureViewDlg.h"
#include "FindUI.h"
#include "KWndManager.h"
#include "KWndDataPackage.h"
#include "DlgInsertTexture.h"
#include "LoadSettingWindow.h"
#include "SceneFileSelectDlg.h"
#include "SettingListDlg.h"
#include "KViewTextureDialog.h"
#include "MagnificationDlg.h"
//#include <string>
//#include <deque>
//#include <algorithm>

#if !defined(AFX_UISCRIPTSTUDIODLG_H__984D5A3F_158A_427C_AE07_55279939D616__INCLUDED_)
#define AFX_UISCRIPTSTUDIODLG_H__984D5A3F_158A_427C_AE07_55279939D616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUIScriptStudioDlg dialog

class CUIScriptStudioDlg : public CDialog
{
// Construction
public:
	CUIScriptStudioDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUIScriptStudioDlg)
	enum { IDD = IDD_UISCRIPTSTUDIO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIScriptStudioDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUIScriptStudioDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	bool				m_bNowRendering;
	bool				m_bNowLoading;
	HACCEL				m_hAccel;
public:
	CStatic				m_ImageRect; 
	CListCtrl			m_CtrlWindowData;
	CListCtrl			m_ListCtrl;
	CTreeCtrl			m_CtlTreeView;

	CTextureViewDlg*	m_pTextureDlg;
	//CMagnifiCationDlg*  m_pMagnificationDlg;
	CString				m_strFilePath;
	CString				m_strFileNameWithPath;
	KWndDataPackage*	m_pDataPackage;
	DlgInsertTexture*	m_pInsertTextureDlg;
    CFindUI*            m_pFindUIDlg;
    CSettingListDlg    m_SettingListDlg;

    CLoadSettingWindow  m_LoadFileInfoWindow;
    CSceneFileSelectDlg m_SceneFileDlg;

	bool				m_bShiftState;
	bool				m_bCtrl;
	bool				m_bDrag;
    bool                m_bCtrlWindowDrag;
	bool				m_KeyPress;
	bool				m_bIsLoaded;
	bool				m_bCaps;

	D3DXMATRIX			m_matTrans;
	CPoint				m_pointStart;
	CRect				m_rectSelect;
	bool				m_bModeTest;

	KD3DWndClass*		m_pSelectedWnd;

	KD3DWndClass*		m_pTestMode;
	int					m_iState;
	int					m_iPos_x;
	int					m_iPos_y;

	int		m_iRvalue;
	int		m_iGvalue;
	int     m_iBvalue;

	LPDIRECT3DTEXTURE9		m_TextureEditStatic;

	CToolBar			*m_toolbar;

	afx_msg void OnDestroy();
	afx_msg void OnFileopen();
	afx_msg void OnFilesave();
	afx_msg void OnUpdateChkview(CCmdUI *pCmdUI);
    afx_msg void OnUpdateMagnification(CCmdUI *pCmdUI);
    afx_msg void OnMagnification();
	void KeyDown(UINT nChar);
	void SettingView(void);
	void InsertChild(KD3DWndClass* pkWnd, HTREEITEM hParent);
	void Render(void);
	
	//	이동석, 반투명 렌더를 위해 bSelect 추가
	void RenderWindow(KD3DWndClass* pkWndClass, int x = 0, int y = 0, bool bSelect = false );
	//-> void RenderWindow(KD3DWndClass* pkWndClass, int x = 0, int y = 0 ); // 기존함수
	
	void DrawRectangle();
	bool ChkSelectImageRect(KD3DWndClass* pkWndClass,CPoint, int x, int y);
	void DrawSelectRect();
	void RenderModeTest(KD3DWndClass *pkWndClass, int state, int pos_x, int pos_y);
	void UpdateCtrl();
	void ChkModeTest(int iState, CPoint point);

	afx_msg void OnLvnItemchangedListchild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeview(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnHdnItemdblclickWindowData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateWindowData(NMHDR *pNMHDR, LRESULT *pResult);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMRclickTreeview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeAdd();
	afx_msg void OnTreeDelete();
	afx_msg void OnTreeFind();
	afx_msg void OnTreeCopy();
	afx_msg void OnTreePaste();
    afx_msg void OnTreeViewTexture();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBack();
	afx_msg void OnForth();
	afx_msg void OnLast();
	afx_msg void OnFirst();
	afx_msg void OnInserttexture();
    afx_msg void OnAllViewTexture();
    afx_msg void OnAllFileSave();
	afx_msg void OnModetest();
	afx_msg void OnNMRclickListchild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListviewSetVisible();
	afx_msg void OnListviewSetInvisible();
	afx_msg void OnListviewSelectAll();
    afx_msg void OnListViewSetFind();

	afx_msg void OnLvnItemActivateListchild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnd();
	afx_msg void OnListview32819();
    afx_msg void OnListviewBack();
    afx_msg void OnListviewForth();
	afx_msg void OnListviewBottom();
	afx_msg void OnListviewTop();
	afx_msg void OnChange();

	afx_msg void OnSameposition();
    afx_msg void OnSamePosition_All();
    afx_msg void OnStnClickedImagerect();


    void SetSamePosition( KD3DWndClass *kWndClass );
    void GetTextureList( KD3DWndClass* pkWnd, bool pbUseTexture[] );

private:
	// 편집창에서 Ctrl-Z & Ctrl-Y 기능 구현
	#pragma pack(1)
	struct HISTORY
	{
		KD3DWndClass*	WndID;		// 식별자
		KD3DWndClass	properties;	// 실제 속성값을 저장
		CRect			SelRect;	// 선택 사각형 내용
	};
	#pragma pack()
	
	std::deque<HISTORY> m_dqHistory;
	std::deque<HISTORY>::iterator m_dqitNow;

	void Undo();
	void Redo();
	void recordHistory( KD3DWndClass* _WndID, bool _btDown=false );
	void clearHistory();

	
	// 마우스로 크기 조절 구현
	bool m_bSizeControl;
	bool m_bSetSize;

	void chckSizeControl(CPoint);
	void drawSizeControl();
	void SetNewSize(CPoint);

	
	// P4에서 상태 체크 & 변경 구현
	bool CheckP4MissionScriptState();
	bool CheckOpenedfile();
	void ForceSyncP4Files();
	void OpenForEditP4Files();
public:
    afx_msg void OnPreregFile();
    afx_msg void OnTextureDef();
    afx_msg void OnTextureHov();
    afx_msg void OnTextureSel();
    afx_msg void OnTextureAct();
    afx_msg void OnTextureLock();

	//------------------------------------------------------------------------
	//	20091007 부터 추가 내용, 이동석
	//	-> CEdit를 CEditSubItem으로 바꿔야 할듯. 그럼 h, cpp 파일이 한쌍씩 늘어나는데... 어쩔수 없으려나.
public:

	//	좌하단 리스트컨트롤 에디트 기능
	CEdit	m_EditSubItem;
	bool	m_bEditSubItemActive;
	bool	m_bViewOption;

	CString	m_strSelectItemName;
	CString m_strSelectItemValue;
	CString	m_strClipEdit;

	int		m_iSelectItem;
	int		m_iCurSelectedItem;

	int		m_iEditSubItemSelPos;

	void SelectedWndListUpdate();

	bool OnEditSubItemBegin( LPNMITEMACTIVATE pNMIA, LRESULT *pResult );
	bool OnEditSubItemEnd();

	bool OnPreTransMouseWheel();
	bool OnPrevSelectEditSubItem();
	bool OnNextSelectEditSubItem();

	bool OnPreTransEditSubItem( MSG* pMsg );

	std::vector< KD3DWndClass* >	m_vecSelectedWnd;

private:	
	bool OnMoveSelectEditSubItem( const int limit, bool bNext = true );
	bool UpdateEditSubItem();

public:
	afx_msg void OnNMClickWindowData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnZViewoption();
	afx_msg void OnNMClickListchild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeview(NMHDR *pNMHDR, LRESULT *pResult);


	/////////////////////////////////////////////////////////////////////////// 2011. 08. 04

	afx_msg void OnBringing();
	afx_msg void OnUpdateBringing(CCmdUI *pCmdUI);



	afx_msg void OnSaveScene();
    afx_msg void OnFileSaveAs();
    afx_msg void OnFileSaveAsSort();
    
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//////////////////////////////////////////////////////////////////////////  2013. 07. 11
	afx_msg void OnLvnBegindragListchild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg INT GetHitIndex(CPoint point);
	afx_msg void MoveListItem(const INT _nSource, const INT _nDest);
	CListCtrl*		m_pDragList;//from
	CImageList*		m_pDragImage;//drag_image
	BOOL			m_bDragging;
	int				m_nDragIndex;
	int				m_nOldTarget;
	CWnd*			m_pDropWnd;
	BOOL			m_bCheck;
	afx_msg void OnTreeUi32898();
	afx_msg void OnSetCurrentAndFirstSubSetSize(KD3DWndClass *fframe, D3DXVECTOR2 point);
	afx_msg void OnTreeUi32896();
	afx_msg bool OnSearchOneSubSet(KD3DWndClass *fframe);
	afx_msg void OnSetCurrentTexturePositionUp();
	afx_msg void OnSetCurrentTexturePositionDown();
	afx_msg void OnSetCurrentTexturePositionLeft();
	afx_msg void OnSetCurrentTexturePositionRight();
	afx_msg void OnSetCurrentTexturePositionUDMiddle();
	afx_msg void OnSetCurrentTexturePositionLRMiddle();
	afx_msg void OnChildListItemMoveByKey(UINT nChar);
	afx_msg void OnChildListItemSelectByKey(UINT nChar);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UISCRIPTSTUDIODLG_H__984D5A3F_158A_427C_AE07_55279939D616__INCLUDED_)
