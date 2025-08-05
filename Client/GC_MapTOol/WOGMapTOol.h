#pragma once

//-----------------------------------------------------------------------------
// File: WOGMapTOol.h
//
// Desc: Header file WOGMapTOol sample app
//-----------------------------------------------------------------------------
#ifndef AFX_WOGMAPTOOL_H__3B87B67F_F46D_4F50_B112_7952156D9CCB__INCLUDED_
#define AFX_WOGMAPTOOL_H__3B87B67F_F46D_4F50_B112_7952156D9CCB__INCLUDED_
//#include "StdAfx.h"
//#include "afxwin.h"
#include "staticrenderview.h"
//#include "afxcmn.h"
//#include <vector>

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file
#endif

//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
// TODO: change "DirectX AppWizard Apps" to your name or the company name
#define DXAPP_KEY        TEXT("Software\\DirectX AppWizard Apps\\WOGMapTOol")

// This GUID must be unique for every game, and the same for
// every instance of this app.  // {BB55E26D-67CE-4C31-95ED-C2D810E01CC1}
// The GUID allows DirectInput to remember input settings
extern GUID g_guidApp;

class GCDeviceTexture;
class GCDeviceFontBase;
class GCDeviceShader;
class CToolObject;

// Property Wnd
class CUserProp;
class CPartList;
class CLayerList;
class CEffectProp;
class CLayerProp;
class CMoveProp;
class CFootHoldValue;
class CCommandRecv;

#define SETSTR(p,q) str << p; \
                    q = str.str().c_str(); \
                    str.str(std::string())
//-----------------------------------------------------------------------------
// Name: class CAppForm
// Desc: CFormView-based class which allows the UI to be created with a form
//       (dialog) resource. This class manages all the controls on the form.
//-----------------------------------------------------------------------------
class CAppForm : public CFormView, public CD3DApplication
{
public:
	enum WOGMAPTOOLMODE
	{
		MODE_FH = 0,
		MODE_LAYER = 1,
	};
	struct WorldVertex
	{
		D3DXVECTOR3 Pos;
		DWORD Diff;
		FLOAT       tu, tv; // Vertex texture coordinates

		void Set(float X, float Y, float Z, float ftu = 0.0f, float ftv = 0.0f, DWORD dwDiff = 0xffffff00)
		{
			Pos.x = X;
			Pos.y = Y;
			Pos.z = Z;

			tu = ftu;
			tv = ftv;

			Diff = dwDiff;
		}

		enum _FVF
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		};

		const WorldVertex& operator = (const WorldVertex& rvar)
		{
			this->Pos.x = rvar.Pos.x;
			this->Pos.y = rvar.Pos.y;
			this->Pos.z = rvar.Pos.z;
			this->Diff = rvar.Diff;

			return *this;
		}
	};
public:
	int                     ResizedX;
	int                     ResizedY;
	void                    SetWorldVertex();
	void                    SetCurLayerVertex(float left, float top, float right, float bottom);
	void                    SetMonitorVertex(float left, float top, float right, float bottom);
	void                    SetCurObjVertex(float left, float top, float right, float bottom);

	WorldVertex m_Vertex[8];
	WorldVertex m_CurLayerVertex[8];
	WorldVertex m_CurObjVertex[8];
	WorldVertex m_MonitorVertex[8];

	WOGMAPTOOLMODE          GetCurMode() { return m_eMode; }
	void                    SetCurMode(WOGMAPTOOLMODE eMode) { m_eMode = eMode; }

	WOGMAPTOOLMODE          m_eMode;
	BOOL                    m_bLoadingApp;          // TRUE, if the app is loading
	CD3DFont*               m_pFont;                // Font for drawing text

	FLOAT                   m_fWorldRotX;           // World rotation state X-axis
	FLOAT                   m_fWorldRotY;           // World rotation state Y-axis

	LPDIRECT3DSWAPCHAIN9    m_pSwapChain1;
	LPDIRECT3DSWAPCHAIN9    m_pSwapChain2;
	CRITICAL_SECTION        m_csSwap;

	CStaticRenderView*      GetRenderView() { return &m_RenderView; }

	void                    SetListLoaded(bool Load) { m_bListLoaded = Load; }

	void                    SetBoxObj(CLayerPart* pObj);

	int                     GetLayerIndex() { if (m_iLayerIndex > 0) return m_iLayerIndex; else m_iLayerIndex = 0; return m_iLayerIndex; }
	void                    SetLayerIndex(int iIndex) { m_iLayerIndex = iIndex; }

	bool                    GetScriptLoaded() { return m_bScriptLoaded; }
	std::string             GetScriptName() { return m_strFileName; }

	CToolObject**           m_ppTObj;
	std::vector< CToolObject* > m_vecCurObj;
	void                    ClearCurObj(bool bClearAll = true);

	void                    InitDungeon(int Width, int Height);
	void                    DeleteDungeon();

	CToolObject**           GetppObj() { return m_ppTObj; }

	bool                    IsNeedtoSave();

	void                    SetControlValue(CLayerPart* pPart);

	void                    SetChangeLayerIndex(int iIndex) { m_iChangeLayerIndex = iIndex; }
	int                     GetChangeLayerIndex() { return m_iChangeLayerIndex; }

	void                    FileSave(std::string strFielName);
	void                    FileLoad(std::string strFielName);

	CUserProp*              GetUserProp() const { return m_pUserProp; }
	CPartList*              GetPartList() const { return m_pPartList; }
	CLayerList*             GetLayerList() const { return m_pLayerList; }
	CLayerProp*             GetLayerProp() const { return m_pLayerProp; }
	CMoveProp*              GetMoveProp() const { return m_pMoveProp; }
	CFootHoldValue*         GetFootHoldValue() const { return m_pFHValue; }

	void                    CreateModaless();
	void                    DestroyModeless();
	void                    ArrangeModaless(CRect& rt);

	void                    DisableAllProperty(BOOL bDisable);
	std::string             GetFileName() { return m_strFileName; }

	void                    UpdateControlValue();
	void                    UpdateSelectPart();

	void                    ShowChildWindow(bool bLayer);
	void                    ToggleLayerWindow(bool bShow);
	void                    ToggleFHWindow(bool bShow);

	void                    ReloadLayer();

	void                    AddLog(const std::string& str);
	void                    ClearLog();

	CCommandRecv*           GetCommandRecv() { return m_pCommandRecv; }
	static DWORD            dwLastFrame;

	GCDeviceFontBase*           m_pkFont;
	void                    RenderCursorIndex();

	void                    FootHoldImageingPrepare();
	void                    FootHoldImageingRestore();

private:
	bool                    m_bScriptLoaded;
	HACCEL  m_hKey;
	DWORD   m_dwTotalFrame;
	HWND    m_hwndRenderWindow;
	HWND    m_hWndTopLevelParent;
	int     m_iChangeLayerIndex;

	void    TakeFootHoldImage();

	HRESULT ConfirmDevice(D3DCAPS9*, DWORD, D3DFORMAT);
	HRESULT OneTimeSceneInit();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT FinalCleanup();
	VOID    Pause(BOOL bPause);

	void    RenderFH();
	HRESULT RenderText();

	VOID    ReadSettings();
	VOID    WriteSettings();

	VOID    UpdateUIForDeviceCapabilites();

	int m_iLayerIndex;
	bool m_bListLoaded;
	bool m_bRenderFH;

	std::string m_strFileName;

	void StampObject();
	void UnStampObject();

	// Child dlgs
	CUserProp* m_pUserProp;
	CPartList* m_pPartList;
	CLayerList* m_pLayerList;
	CLayerProp* m_pLayerProp;
	CMoveProp* m_pMoveProp;
	CFootHoldValue* m_pFHValue;

	CCommandRecv*   m_pCommandRecv;

#ifdef _DEBUG
	FILE* m_fpStream;
#endif

	void    AddMassFile(const std::string& strFileName_);

	bool        m_bFootHoldImaging;
	CRect       m_rtRenderView;

	HANDLE      m_hFootHoldImageThread;

protected:
	DECLARE_DYNCREATE(CAppForm)

	CAppForm();
	virtual  ~CAppForm();

public:
	TCHAR* PstrFrameStats() { return m_strFrameStats; }
	VOID RenderScene() { Render3DEnvironment(); }

	//{{AFX_DATA(CAppForm)
	enum { IDD = IDD_FORMVIEW };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAppForm)
	virtual void OnInitialUpdate();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CAppForm)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLayermanagerCreatelayerpart();
	afx_msg void OnLayermanagerEditlayer();
	CStaticRenderView m_RenderView;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnAppExit();
	afx_msg void OnAccelFileLoad();
	afx_msg void OnAccelExit();
	afx_msg void OnFileLoad();
	afx_msg void OnAccelResetview();
	afx_msg void OnFileSave();
	afx_msg void OnAccelFileSave();
	afx_msg void OnFileClose();
	afx_msg void OnFileNew();
	afx_msg void OnAccelFileNew();
	afx_msg void OnModeLayer();
	afx_msg void OnModeFh();
	afx_msg void OnAccelModeFh();
	afx_msg void OnAccelModeLayer();
	afx_msg void OnAccelFileClose();
	afx_msg void OnWorldresize();
	afx_msg void OnAccelWorldResize();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDebugZordersorting();
	afx_msg void OnAccelFileSaveAs();
	afx_msg void OnFileSaveAs();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	CListCtrl   m_lcLog;
	afx_msg void OnAccelUndo();
	afx_msg void OnAccelRedo();
	afx_msg void OnGridguidelineChangeTerm();
	afx_msg void OnGridAddVerticalGuide();
	afx_msg void OnGridAddHorizonGuide();
	afx_msg void OnBnClickedHGuideline();
	afx_msg void OnBnClickedVGuideline();
	afx_msg void OnForcedebugMakefootholdimage();

	CButton m_btnHorizonGuide;
	CButton m_btnVerticalGuide;
	afx_msg void OnAccelFootHold();
};

//-----------------------------------------------------------------------------
// Name: class CAppDoc
// Desc: Overridden CDocument class needed for the CFormView
//-----------------------------------------------------------------------------
class CAppDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE(CAppDoc)

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CAppDoc)
public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAppDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------------
// Name: class CAppFrameWnd
// Desc: CFrameWnd-based class needed to override the CFormView's window style
//-----------------------------------------------------------------------------
class CAppFrameWnd : public CFrameWnd
{
protected:
	DECLARE_DYNCREATE(CAppFrameWnd)
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppFrameWnd)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAppFrameWnd)
	afx_msg void OnChangeDevice();
	afx_msg void OnConfigInput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------------
// Name: class CApp
// Desc: Main MFC application class derived from CWinApp.
//-----------------------------------------------------------------------------
class CApp : public CWinApp
{
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CApp)
public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

extern LPDIRECT3DDEVICE9       g_pd3dDevice;
extern char g_Path[MAX_PATH];
extern CAppForm*       g_AppFormView;
extern GCDeviceShader*   g_pRenderState;
