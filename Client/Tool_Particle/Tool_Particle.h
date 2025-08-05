//-----------------------------------------------------------------------------
// File: Tool_Particle.h
//
// Desc: Header file Tool_Particle sample app
//-----------------------------------------------------------------------------
#ifndef AFX_TOOL_PARTICLE_H__E2508A34_AE0C_49D9_A875_2D52EBA75A72__INCLUDED_
#define AFX_TOOL_PARTICLE_H__E2508A34_AE0C_49D9_A875_2D52EBA75A72__INCLUDED_
#include "afxcmn.h"
#include "afxwin.h"
#include "TimeLineEditor.h"
#include "ListParticle.h"

#if _MSC_VER >= 1000
#pragma once
#endif
#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file
#endif

//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
// TODO: change "DirectX AppWizard Apps" to your name or the company name
#define DXAPP_KEY        TEXT("Software\\DirectX AppWizard Apps\\Tool_Particle")

// Struct to store the current input state
struct UserInput
{
    // TODO: change as needed
    BOOL bRotateUp;
    BOOL bRotateDown;
    BOOL bRotateLeft;
    BOOL bRotateRight;
};

struct BKVertex
{
	D3DXVECTOR4 m_vPos; // x, y, z, 1/w
	D3DCOLOR    m_diff; // diffuse color
	D3DXVECTOR2 m_vTex; // tu, tv

	BKVertex()
	{
		m_diff = 0xffffffff;
	}

	enum _FVF
	{
		FVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1,
	};
};

//-----------------------------------------------------------------------------
// Name: class CAppForm
// Desc: CFormView-based class which allows the UI to be created with a form
//       (dialog) resource. This class manages all the controls on the form.
//-----------------------------------------------------------------------------
class CAppForm : public CFormView, public CD3DApplication
{
public:
    BOOL                    m_bLoadingApp;          // TRUE, if the app is loading
    //ID3DXFont*              m_pD3DXFont;            // D3DX font    

    UserInput               m_UserInput;            // Struct for storing user input 

    FLOAT                   m_fWorldRotX;           // World rotation state X-axis
    FLOAT                   m_fWorldRotY;           // World rotation state Y-axis

	CString					m_strOpenFile;
	CString					m_strOpenTextureFile;
private:
    HWND    m_hwndRenderWindow;
    HWND    m_hwndRenderFullScreen;
    HWND    m_hWndTopLevelParent;

    HRESULT ConfirmDevice( D3DCAPS9*,DWORD,D3DFORMAT );
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT FrameMove();
    HRESULT Render();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT FinalCleanup();
    virtual HRESULT AdjustWindowForChange();

    HRESULT RenderText();

    void    UpdateInput( UserInput* pUserInput );

    VOID    ReadSettings();
    VOID    WriteSettings();

    VOID    UpdateUIForDeviceCapabilites();
	void RenderBKImg();

protected:
    DECLARE_DYNCREATE(CAppForm)

             CAppForm();
    virtual  ~CAppForm();

public:
    BOOL IsReady() { return m_bReady; }
    TCHAR* PstrFrameStats() { return m_strFrameStats; }
    VOID RenderScene() { Render3DEnvironment(); }
    HRESULT CheckForLostFullscreen();

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
	CListParticle m_ListParticle;
	CComboBox m_BlendSrc;
	CComboBox m_BlendDest;
	CString m_strName;
	UINT m_uiMaxParticle;
	float m_fKillTime;
	CString m_strLifeTime;
	CComboBox m_ComboLayer;
	CString m_strTexture;
	CString m_strGravity;
	CString m_strEmitRate;
	CString m_strEmitRadius;
	UINT m_uiKillNum;

	CTimeLineEditor m_GridTimeLine;

	D3DCOLOR m_ColorBK;

	LPDIRECT3DTEXTURE9 m_pBKImg;
public:
    //{{AFX_MSG(CAppForm)
    afx_msg void OnToggleFullScreen();
    afx_msg void OnChangeDevice();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
	afx_msg void OnStnClickedRenderview();
	afx_msg void OnFileOpen();	
	afx_msg void OnBnClickedOpenfile();
//	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedApply();
	afx_msg void OnParticleNew();
	afx_msg void OnColor();
	afx_msg void OnBKImg();
	afx_msg void OnCbnSelchangeComboSrcBlend();
	afx_msg void OnBnClickedBtGravity();
	afx_msg void OnBnClickedBtEmitradius();
	afx_msg void OnFileSave();
	afx_msg void OnBnClickedScriptedit();
	afx_msg void OnBnClickedBtEmitrate();
	afx_msg void OnParticleFind();
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
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
    virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CAppFrameWnd)
    afx_msg void OnChangeDevice();
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
    virtual BOOL OnIdle( LONG );
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
extern CAppForm*       g_AppFormView;

#endif



