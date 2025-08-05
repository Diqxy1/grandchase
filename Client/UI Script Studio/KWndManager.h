#pragma once

#include "D3DWndClass.h"
//#include "../GCLuaManager/KLuaManager.h"
//#include <vector>
//#include <string>
#include <d3d9.h>
#include "KWndDataPackage.h"
#include "../MassFileLib/GCMassFileManager.h"

// stg file struct
struct tagUIScriptFile{
    std::string strFileName;
    std::string strFileCaption;

    bool operator<( tagUIScriptFile rhs) { return strFileName < rhs.strFileName; }
};

//
class KWndManager
{
public:
	KWndManager(void);
	~KWndManager(void);
	void Init();
	
	void InsertWindow( KD3DWndClass* pkWnd, KD3DWndClass* pkParent );
	void InsertWindow( KD3DWndClass* pkWnd, KD3DWndClass* pkParent, UINT nIndex );

	
	void MoveWindow( KD3DWndClass* pkWnd, KD3DWndClass* pkParent );
	
	void CopyWindow( KD3DWndClass* pkWnd );

	void PasteWindow( KD3DWndClass* pkParent , int iIndex = -1 );
    void PasteWindowContent( KD3DWndClass* pkParent , int iIndex = -1 );
	
	void DeleteWindow( KD3DWndClass* pkWnd );
	
	bool LoadLua(std::string);
	

	HRESULT LoadUIWindowTable( KD3DWndClass* pkWnd, KD3DWndClass* pkParent );
	HRESULT LoadUINumberTable( KD3DNumClass* pkWnd );
	HRESULT LoadUIScript();
	HRESULT DataMapping();

	HRESULT SaveToFile(const std::string& strFileName, bool bSort = false);
    HRESULT SaveToFile(KD3DWndClass* pkWnd, const std::string& strFileName);
	std::string ToString(KD3DWndClass* pkWnd);
	KD3DWndClass* FindWnd( const std::string& strName ,KD3DWndClass *pkWnd , const UINT uiIndex );
	HRESULT	Z_PosForth(KD3DWndClass*);
	HRESULT	Z_PosBack(KD3DWndClass*);
	HRESULT	Z_PosTop(KD3DWndClass*);
	HRESULT	Z_PosBottom(KD3DWndClass*);
	HRESULT	Z_PosMove(KD3DWndClass*, int nCount);

	HRESULT DropTexture(UINT index);
	HRESULT ArrangeDeletedTextureIndex(KD3DWndClass* pkWnd , UINT index);
	std::vector<std::string>			m_vecStrTexFileName;
    std::vector<tagUIScriptFile>		m_vecUIScriptFile;       // 2011. 09. 01  stg파일과 캡션을 같이 쓰기 위해 수정

	std::map<int,LPDIRECT3DTEXTURE9>		m_mapTexture;
	
	KD3DWndClass*				m_pWndRoot;	

	GCMassFileManager*			GetMassFileManager() { return m_pMassFileManager; }
	std::string			m_KOMFileName;

	KD3DWndClass* GetClipboardPointer(){ return m_pClipBoard; }
	void SetClipboardPointer( KD3DWndClass* src ){ m_pClipBoard = src; }

private:
	std::string TextureTableToString();
	std::string NumberTableToString();
		
	void		  SwapWnd( KD3DWndClass** pkWnd1 , KD3DWndClass** pkWnd2 );
	
	UINT m_uiTab;
	BOOL m_bStart;
	KLuaManager					m_LuaMgr, m_LuaCopy;

    KD3DWndClass*				m_pClipBoard;
	KD3DWndClass*				m_pSourceWnd;

	std::vector<KD3DNumClass*>	m_vecNum;
	GCMassFileManager*			m_pMassFileManager;
    
public:
    std::vector<tagUIScriptFile> m_vecCaptionData;             // 캡션 정보 저장 백터
    BOOL LoadCaption();                                        // 2011. 09. 28
    HRESULT InsertCaptionData( tagUIScriptFile* pFileInfo );   // 2011. 09. 28
    HRESULT LoadUIScriptTable( tagUIScriptFile* pFileInfo );   // 2011. 09. 28

};
