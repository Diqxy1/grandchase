#pragma once
#include <d3d9.h>
#include <d3dx9tex.h>
//#include <vector>

struct KD3DWndClass;

// KViewTextureDialog 대화 상자입니다.

enum EUIState
{
    EUISTATE_DEFAULT = 0,
    EUISTATE_HOVER,
    EUISTATE_SELECT,
    EUISTATE_ACTIVE,
    EUISTATE_LOCK,
};

struct RectInfo
{
    int iTexture;
    EUIState eState;

    int x1;
    int y1;
    int x2;
    int y2;
};



// line 그리기용
struct LineVertex
{
	float x, y, z;
	float rhw;
	DWORD color;
};
// fvf
#define D3DFVF_LINEVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)





class KViewTextureDialog : public CDialog
{
	DECLARE_DYNAMIC(KViewTextureDialog)

public:
	KViewTextureDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KViewTextureDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW_TEXTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnSelectChange();

    virtual BOOL OnInitDialog();

    CString m_strText;
	CString m_strFolderPath;	// export 2011. 08. 19

    CListBox* m_pkUseTextureList;

#define MAX_TEXTURE 300
    int m_iMaxTexture;
    bool m_abUseTexture[MAX_TEXTURE];
    int m_iCountUseTexture;
    int m_aiUseTextureList[MAX_TEXTURE];

    int m_iSelect;

    void Render();
    void RenderWindow( int nIndex );
    void LoadTexture();
    void DrawRectangle( long x1, long y1, long x2, long y2 );
    void AddRectInfo( KD3DWndClass* kWnd );

    std::vector< RectInfo > m_vecRect;

    LPDIRECT3D9                     m_pD3D;
    LPDIRECT3DDEVICE9               m_pd3dDevice;
    LPD3DXSPRITE                    m_pSprite;
    std::map<int,LPDIRECT3DTEXTURE9> m_mapTexture;  // Our texture

    HRESULT                 InitD3D( HWND hWnd );
    VOID                    Cleanup();

	CStatic                 m_ctlTexture;


	//////////// 2011. 08. 19  export 기능 추가
	LPDIRECT3DSURFACE9 m_pOldBackBuffer;		// 기존 표면
	LPDIRECT3DSURFACE9 m_pSurface;				// 새로 렌더링 할 표면

	afx_msg void OnBnClickedButtonExport();
	bool GetSaveFolerPath();
	void ExportMaskedTexture();
	void RenderSurface( int nIndex );
	void RenderLine( int nIndex );
	void SaveSurfaceToFile( int nIndex );
	void CopyOrginalFile();
};
