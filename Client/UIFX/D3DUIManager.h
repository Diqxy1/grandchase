#ifndef _D3DUIMANAGER_H_
#define _D3DUIMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#define	DEFAULT_UI_TEXTURE	16
#define TEXEL_BOUND(t) if(t<0)t=0;if(t>1)t=1;

//#include "../KAssert.h"
//#include "D3DUIObject.h"
//#include "D3DUIScene.h"
//#include "KInput.h"
//#include "FontManager.h"
//#include <map>
//#include <vector>
//#include <list>
//#include <string>
//#include <queue>
#include "GCDeviceBuildTexture.h"

class KLuaManager;
class KOldFontManager;

class GCUITextureCache:public INotUseCountable
{
public:
	GCUITextureCache(int index,D3DXVECTOR4 *uv);	
	~GCUITextureCache();
	
	inline bool IsContain(int itex,D3DXVECTOR4 *vec);	
	inline bool IsContained(int itex,D3DXVECTOR4 *vec);		
	inline bool IsContained(GCUITextureCache * section){return IsContained(section->m_iTexIndex,&section->m_uv);}		
	
	inline bool SetTexture(GCBuildTexture * tex,GCTmpTexture * tmptex);
	inline void DrawInScreen(KD3DUIVertex* vert);
	void Clear();

	inline int GetIndex(){return m_iTexIndex;}

protected:
	int			m_iTexIndex;
	D3DXVECTOR4	m_uv;
	GCBuildTexture * m_BuildTexture;
	STextureSection m_BuildSection;
};


struct KD3DUINumInfo
{
    std::string strType;
    DWORD       dwWidth;
    DWORD       dwHeight;
    int         iTexIdx;
    D3DXVECTOR4 vTexCoords;
    float       fOffset;

    KD3DUINumInfo( void ) : vTexCoords( 0.0f, 0.0f, 1.0f, 1.0f )
    {
        strType.erase();
        dwWidth  = 0;
        dwHeight = 0;
        iTexIdx  = -1;
        fOffset  = 0;
    }
    ~KD3DUINumInfo( void )
    {
        // empty
    }
    KD3DUINumInfo( const KD3DUINumInfo& src )
    {
        strType    = src.strType;
        dwWidth    = src.dwWidth;
        dwHeight   = src.dwHeight;
        iTexIdx    = src.iTexIdx;
        vTexCoords = src.vTexCoords;
        fOffset    = src.fOffset;
    }
    const KD3DUINumInfo& operator=( const KD3DUINumInfo& src )
    {
        strType    = src.strType;
        dwWidth    = src.dwWidth;
        dwHeight   = src.dwHeight;
        iTexIdx    = src.iTexIdx;
        vTexCoords = src.vTexCoords;
        fOffset    = src.fOffset;
        return *this;
    }
};

class KD3DUIManager : public KD3DUIObject // extends
{
public:
    KD3DUIManager( void );
    virtual ~KD3DUIManager( void );
    KD3DUIManager( const KD3DUIManager& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    const KD3DUIManager& operator=( const KD3DUIManager& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	//void ResetDeviceFont();
    HRESULT OneTimeSceneInit( HWND hWnd, const std::string& strScriptFileName );
    HRESULT InitDeviceObjects();
    HRESULT DeleteDeviceObjects( void );
    HRESULT FinalCleanup( void );
    HRESULT FrameMove( float fTime );
    HRESULT Render( void );

	void RenderNumber(	const std::string& strType,
						const D3DXVECTOR2& vPos,
						const char* strChar,
						D3DXCOLOR argb = 0xffffffff,
						bool bWriteToRightSide = true,
						bool bShadow = true,
						float fInterval = -1.0f,
						bool bScale = true,
						bool bNotFrontZero = false,
						float fScale =1.0f );
    void RenderNumber( 	const std::string& strType,
                       	const D3DXVECTOR2& vPos,
						UINT64 dwNum,
						bool bWriteToRightSide = true,
						bool bShadow = true,
						float fInterval = -1.0f,
						bool bScale = true);

    void RenderNumber( 	const std::string& strType,
                        const D3DXVECTOR2& vPos,
                        DWORD dwNum, 
                        bool bWriteToRightSide,
                        bool bShadow,
                        float fInterval,
                        bool bScale,
                        int iPrecision,
                        bool bPrecision ); 

	void RenderNumberScale( 	const std::string& strType,
						const D3DXVECTOR2& vPos,
						DWORD dwNum,
						float fScale = 1.0f,
                        DWORD dwColor = 0xffffffff,
                        bool bShadow = true,
						bool bScale = true,
                        bool bWriteToRightSide = true,/* = true */
						float fInterval = -1.0f );

	void RenderNumber(	const std::string& strType,
						const D3DXVECTOR2& vPos,
						UINT64 dwNum,
						D3DXCOLOR argb,
						bool bWriteToRightSide = true,
						bool bShadow = true,
						float fInterval = -1.0f, 
						bool bScale = true);

	//이 넘버 타입의 높이와 넓이를 리턴해주는 함수
	void GetTypeNumberInfo( IN const std::string& strType , OUT DWORD& dwWidth_ , OUT DWORD& dwHeight_ );

    void MoveUIScene( const std::string& strSceneName );
	void CreateFonts();

    bool CheckInstallFont( IN char *szFontFileName = NULL, OUT char *szFontName = NULL );
    bool InstallFont( IN const char *szCopyWinFontPath, IN char *szFontName );
	void ResetFonts();
	std::vector<KD3DWndClass>		            m_vecUIWndClass;
	std::vector<int>							m_vecScene;

	void WriteUIData_Binary();
	void ReadUIData_Binary(std::string strUIFileName);

    inline void RegisterUIScene( const std::string& strSceneName, KD3DUIScene* pkScene )
    {
        m_mapScene[strSceneName] = pkScene;
    }
    inline void RegisterSceneFactory( const std::string& strType, KD3DUISceneFactory* pkFactory )
    {
        m_mapSceneFactory[strType] = pkFactory;
    }
    inline void RegisterWindowFactory( const std::string& strType, KD3DWndFactory* pkFactory )
    {
        m_mapWndFactory[strType] = pkFactory;
    }
    inline void RegisterSpline( const std::string& strType, KSpline* pkSpline )
    {
        m_mapSpline[strType] = pkSpline;
    }
    inline void AddActionListener( KActionListener* pActionListener )
    {
        m_pActionListener = pActionListener;
    }

    GCDeviceTexture *GetDeviceTexture( int iIndex_ ){return m_apDeviceTexture[iIndex_];};
    
	void ClearTexture( void ); 
	void ClearUnusedTexture(int i=1);	

	//reload script
	void ReloadNumTable( const char* strScriptFileName );

    //오브젝트 동적 생성을 위해서, Public으로 선언 바꿈.
	KD3DWnd* CreateTemplateUI( IN KD3DWnd* pkParent, IN std::string strType, IN std::string strScriptFile, IN std::string strWndName = "", IN bool bCreate = true, bool bPushTable = false );
	HRESULT LoadUIWindowTable_FromScriptOriginal( KLuaManager* pkLuaMgr, KD3DWnd* pkWnd, KD3DWnd* pkParent = NULL , std::string strWndName = "" );	
    HRESULT LoadUIWindowTable_FromScript( KLuaManager* pkLuaMgr, KD3DWnd* pkWnd, KD3DWnd* pkParent = NULL , std::string strWndName = "" );	
	HRESULT LoadUIWindowTable( std::string strFileName, KD3DUIScene* pkWnd );	
	
    GCUITextureCache* DrawUIInScreen(GCUITextureCache* cache,int iIndex,KD3DUIVertex* vertex);
    KD3DWnd*    CreateUI( const char* strType );
    D3DXVECTOR2 GetMousePos();
	//UI의 해상도 관련 값 재 구성.
	void ResetAllUIforResolution();

private:
    typedef boost::unordered_map<KD3DWnd::uid_type,boost::unordered_map<DWORD,std::vector<boost::shared_ptr<KD3DWndEventProcedure>>>> procedurelist_type;
    typedef boost::unordered_map<DWORD,std::vector<boost::shared_ptr<KD3DWndEventProcedure>>> procedurewnd_type;
    typedef std::vector<boost::shared_ptr<KD3DWndEventProcedure>> procedurefunc_type;

    procedurelist_type  m_mapProcedure;
    void CallWndProcedure( KD3DWnd::uid_type UID_, const KActionEvent& kEvent_ );
    void CallCodeProcedure( KD3DWnd::uid_type UID_, procedurewnd_type& procedure_wnd, DWORD dwCode_, const KActionEvent& kEvent_ );
    void SendEventToLua( const KActionEvent& kEvent_ );
public:
    KD3DWnd* GetWndFromID( KD3DWnd::uid_type UID_ );
    void RemoveUID( KD3DWnd::uid_type UID_ );
    void SendEvent( const KActionEvent& kEvent_, bool bSendLua_ = true );

    template< typename UI, typename R, typename P >
    int ConnectGlobalEventProcedure( DWORD dwEventCode_, UI* pWnd, R (UI::*pFunc_)(P) )
    {
        boost::shared_ptr<KD3DWndEventProcChild<UI,R,P>>    spCommand( new KD3DWndEventProcChild<UI,R,P>() );
        spCommand->m_ID = pWnd->GetUID();
        spCommand->m_pFunc = pFunc_;
        ConnectEventProcedure( -1, dwEventCode_, spCommand );
        return spCommand->GetUID();
    }

    template< typename UI, typename R >
    int ConnectGlobalEventProcedure( DWORD dwEventCode_, UI* pWnd, R (UI::*pFunc_)() )
    {
        boost::shared_ptr<KD3DWndEventProcChild<UI,R,void>>    spCommand( new KD3DWndEventProcChild<UI,R,void>() );
        spCommand->m_ID = pWnd->GetUID();
        spCommand->m_pFunc = pFunc_;
        g_pkUIMgr->ConnectEventProcedure( -1, dwEventCode_, spCommand );
        return spCommand->GetUID();
    }

    void ConnectEventProcedure( KD3DWnd::uid_type UID_, DWORD dwCode_, boost::shared_ptr<KD3DWndEventProcedure> func_ );
    void ConnectEventProcedure( DWORD dwCode_, boost::shared_ptr<KD3DWndEventProcedure> func_ );
    void DisconnectAllEventProcedure( KD3DWnd::uid_type UID_ );
    void DisconnectEventProcedure_Code( KD3DWnd::uid_type UID_, DWORD dwCode_ );
    void DisconnectEventProcedure_ProcID( KD3DWnd::uid_type UID_, int iProcedureUID_ );
    void DisconnectEventProcedure_Detail( KD3DWnd::uid_type UID_, DWORD dwCode_, int iProcedureUID_ );
    void DisconnectEventProcedure( int iProcedureUID_ );
    void DisconnectGlobalEventProcedure( KD3DWnd::uid_type UID_ );
    void DisconnectGlobalEventProcedure( KD3DWnd::uid_type UID_, DWORD dwCode_ );
    void DisconnectGlobalEventProcedure( KD3DWnd::uid_type UID_, procedurefunc_type& procedures_ );
	std::map<std::string, KD3DUINumInfo> GetNumInfor()			{ return m_mapNumInfo; }
protected:    

    KD3DWnd*    CreateWndFromFactory( std::string& strType_ );
	GCUITextureCache* GetUITextureCache(int iIndex,D3DXVECTOR4* vertex);

	void ClearUIScene( void );
    HRESULT LoadUIScript( KLuaManager* pkLuaMgr, const std::string& strScriptFileName );
	HRESULT LoadTextureTable( KLuaManager* pkLuaMgr );
	HRESULT LoadNumTable( KLuaManager* pkLuaMgr );
    HRESULT LoadLuaGlobal( KLuaManager* pkLuaMgr );
    HRESULT LoadLuaClass( KLuaManager* pkLuaMgr );
    HRESULT LoadUISceneTable( KLuaManager* pkLuaMgr, KD3DUIScene* pkUIScene ,std::string strFileName);

    GCDeviceTexture**                           m_apDeviceTexture;

    IDirect3DDevice9*                           m_pd3dDevice;
    std::vector<std::string>                    m_vecStrTexFileName;

    std::map<std::string, KD3DUISceneFactory*>  m_mapSceneFactory;
    std::map<std::string, KD3DUIScene*>         m_mapScene;

    typedef boost::unordered_map<int, KD3DWnd*> wndlist_type;
    boost::unordered_map<int, KD3DWnd*>         m_mapWndList;

    std::string                                 m_strCurrScene;
    std::string                                 m_strNextScene;

    std::map<std::string, KD3DWndFactory*>      m_mapWndFactory;
    std::map<std::string, KSpline*>             m_mapSpline;
    KActionListener*                            m_pActionListener;

    KInput                                      m_kInput;
#if defined( _THAI_LANG )	//태국은 Tahoma체를 사용합니다.
	GCDeviceFontTH*                             m_pkFontMgr;
	GCDeviceFontTH*                             m_pkFontMgrSmall;
	GCDeviceFontTH*                             m_pkFontMgr14;
	GCDeviceFontTH*                             m_pkFontMgr16;
	GCDeviceFontTH*                             m_pkFontMgrTitle;
	GCDeviceFontTH*                             m_pkFontMgrOutline;
	GCDeviceFontTH*                             m_pkFontMgrOutline14;
	GCDeviceFontTH*                             m_pkFontMgrOutline16;
	GCDeviceFontTH*                             m_pkFontMgrOutlineTitle;
#else
    GCDeviceFont*                               m_pkFontMgr;
	GCDeviceFont*								m_pkFontMgrSmall;
    GCDeviceFont*                               m_pkFontMgr14;
    GCDeviceFont*                               m_pkFontMgr16;
    GCDeviceFont*                               m_pkFontMgrTitle;
    GCDeviceFont*                               m_pkFontMgrOutline;
    GCDeviceFont*                               m_pkFontMgrOutline14;
    GCDeviceFont*                               m_pkFontMgrOutline16;
    GCDeviceFont*                               m_pkFontMgrOutlineTitle;
#endif

    KD3DUIVertex                                m_rectNumRender[4];
   

	std::set<GCUITextureCache*>				m_setUITextureSection;	
	std::list<GCBuildTexture *>					m_listBuildTexture;
    GCTmpTexture*								m_tmpTexture;
    bool                                    m_bCursorOnUI;

public:
	std::map<std::string, KD3DUINumInfo>        m_mapNumInfo;

    KD3DWnd* CreateDefaultUI( IN KD3DWnd* pkParent, IN std::string strType, IN std::string strWndName = "");
    KD3DWnd* CreateTemplateUIByFindName( IN KD3DWnd* pkParent_, IN const char* strScriptFile_, IN const char* strMakeWndName_, IN const char* strWndFindName_,  IN bool bCreate_ = true );
    std::vector<KD3DWnd*> CreateTemplateUIListByFindName( IN KD3DWnd* pkParent_, IN const char* strScriptFile_, IN const char* strMakeWndName_, IN const char* strWndFindName_,  IN bool bCreate_ = true, int iCount = 1);

    void SetCursorOnUI() { m_bCursorOnUI = true; }
    bool GetCursorOnUI() { return m_bCursorOnUI; }
private:
    HRESULT LoadUIScriptByFindName( KLuaManager* pkLuaMgr_, std::vector<KD3DWnd*>& vecWnd_, KD3DWnd* pkParent_, std::string strMakeWndName_, std::string strFindWndName_, int iCount );

};

extern KD3DUIManager*   g_pkUIMgr;
extern HWND             g_hUIMainWnd;

#endif // _D3DUIMANAGER_H_
