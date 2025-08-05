#include "StdAfx.h"
#include ".\kWndManager.h"
#include "KD3DSizingBox.h"
//#include <vector>

static int m_iID = 0;
#define MAX_NAME 100

#if defined( SAFE_DELETE )
#undef SAFE_DELETE
#endif
#define SAFE_DELETE(p)		{ if(p) delete p; p = NULL; }

KWndManager::KWndManager(void)
{

	m_mapTexture.clear();

	m_bStart = false;
	m_uiTab = 0;
	m_pSourceWnd = NULL;
    m_pClipBoard = NULL;

	m_pMassFileManager = new GCMassFileManager();
	m_pMassFileManager->SetRealFileFirst( true );

	m_pWndRoot = new KD3DWndClass();
}

KWndManager::~KWndManager(void)	
{
    

	for(std::map<int,LPDIRECT3DTEXTURE9>::iterator mIt = m_mapTexture.begin(); mIt != m_mapTexture.end();mIt++)
    {
        if( mIt->second != NULL )
            mIt->second->Release();		

    }
	
	for(UINT j = 0 ; j < m_vecNum.size() ; j ++ )
	{
		delete m_vecNum[j];
	}

	
	m_pWndRoot->DeleteChildren();
	SAFE_DELETE( m_pWndRoot );
	SAFE_DELETE( m_pMassFileManager );

	//	클립보드 삭제 : 이동석
	if( m_pClipBoard ) m_pClipBoard->DeleteChildren();	//	자식들 먼저 파괴
	SAFE_DELETE( m_pClipBoard );						//	그리고 포인터 delete
}


void KWndManager::Init()
{
    m_pWndRoot->SetWndName( "scene_part" );
	m_pWndRoot->ClearChildContainer();
	m_mapTexture.clear();
}

bool KWndManager::LoadLua(std::string strFilename)
{
	if(FAILED( m_LuaMgr.DoFile( strFilename.c_str() ) ) )
		return false;
    return true;

}

KD3DWndClass* KWndManager::FindWnd( const std::string& strName ,KD3DWndClass *pkWnd, const UINT uiIndex )
{
	KD3DWndClass* pkResult;
	static UINT stc_Index = 0;
	if ( strName == pkWnd->GetWndName() )
	{
		stc_Index++;
		if( uiIndex == stc_Index )
		{
			stc_Index = 0;
			return pkWnd;
		}
	}
	for(UINT i = 0 ; i < pkWnd->GetChildNum(); i++)
	{
        pkResult = FindWnd( strName , pkWnd->GetChild(i), uiIndex );
		if ( pkResult )
			return pkResult;

	}
	stc_Index = 0;
	return NULL;
}

HRESULT KWndManager::LoadUIScript()
{

    HRESULT hr;
	extern LPDIRECT3DDEVICE9 g_pd3dDevice;	
    
	// read texture tables
    if ( SUCCEEDED( hr = m_LuaMgr.BeginTable( "texture" ) ) )
    {
        for ( UINT i = 1; ; ++i )
        {
            std::string strTexFileName;
            if ( SUCCEEDED( hr = m_LuaMgr.GetValue( i, strTexFileName ) ) )
            {
				if(!strTexFileName.empty())
					m_vecStrTexFileName.push_back( strTexFileName );
				else
					break;
            }
            else 
				break; // no more data
        }

        if ( FAILED( hr = m_LuaMgr.EndTable() ) ) return hr; // invalid file format
    }

    // read scene tables (modified 2011. 09. 05)
    // 캡션(설명)정보 불러온다.
    LoadCaption();

    m_vecUIScriptFile.clear();
    if ( SUCCEEDED( hr = m_LuaMgr.BeginTable( "scene" ) ) )
    {
        tagUIScriptFile kUiScriptFileInfo;

        for ( UINT i = 1; ; ++i )
        {
            // stg 파일명을 받아 저장
            if ( SUCCEEDED( hr = m_LuaMgr.GetValue( i, kUiScriptFileInfo.strFileName ) ) )
            {
                // 파일에 맞는 설명을 찾아 저장
                if ( SUCCEEDED( InsertCaptionData( &kUiScriptFileInfo ) ) )
                {
                    m_vecUIScriptFile.push_back( kUiScriptFileInfo );
                }
            }
            else
                break;// no more data
        }

        if ( FAILED( hr = m_LuaMgr.EndTable() ) ) return hr; // invalid file format
    }

	hr = S_OK;
	for(UINT i = 0; i < m_vecStrTexFileName.size(); i++)
	{
		
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		/*
		if( FAILED( hr = D3DXCreateTextureFromFile( g_pd3dDevice, m_vecStrTexFileName[i].c_str() , &pTexture ) ) )
			return hr;
		*/
		GCMassFile::MASSFILE_MEMBERFILEINFO* Info = m_pMassFileManager->LoadDataFile( m_vecStrTexFileName[i] );

		if( Info != NULL )
		{
			pTexture = NULL;
            D3DXIMAGE_INFO	TexInfo;

            D3DFORMAT dwFormat = D3DFMT_UNKNOWN;

            if( FAILED(D3DXCreateTextureFromFileInMemoryEx( g_pd3dDevice, (void*)Info->pRealData, Info->ulSize, 
                D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, dwFormat/*3DFMT_A8R8G8B8*/, 
                D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, 
                &TexInfo,
                NULL, &pTexture ) ))
            {
                pTexture = NULL;
                hr = E_FAIL;
            }
        }
        if(pTexture)
            m_mapTexture.insert(std::pair<int,LPDIRECT3DTEXTURE9>( i,pTexture) );
	}

	return hr;
}



HRESULT KWndManager::DataMapping()
{
	HRESULT hr;

	if ( FAILED( m_LuaMgr.BeginTable( "scene_part" ) ) ) 
		return S_FALSE;
	//m_LuaMgr.BeginTable( 1 ) ;

	for(int i = 1; ; i++)
	{
		if( SUCCEEDED(m_LuaMgr.BeginTable(i,false)) ) 
		{
			KD3DWndClass* d3dwindow = new KD3DWndClass();
			if( FAILED ( hr = LoadUIWindowTable(d3dwindow, m_pWndRoot)))
				return hr;
			m_pWndRoot->AddChildWnd(d3dwindow);
			if( FAILED ( hr = (HRESULT)m_LuaMgr.EndTable(false) ) ) 
				return hr;
		}
		else 
			break;
	}

	m_LuaMgr.EndTable();


//  m_pWndRoot->SortByIndex();


	//if ( SUCCEEDED( hr = m_LuaMgr.BeginTable( "number" , false ) ) )
	//{
	//	for( UINT i = 1 ; m_LuaMgr.BeginTable(i) ; i++)
	//	{
	//		KD3DNumClass* d3dNum = new KD3DNumClass();
	//		if( FAILED ( hr = LoadUINumberTable(d3dNum) ) )
	//			return hr;
	//		m_vecNum.push_back(d3dNum);
	//		if( FAILED ( hr = (HRESULT)m_LuaMgr.EndTable(false) ) )
	//			return hr;

	//	}
	//	m_LuaMgr.EndTable(false);
	//}

	
	
	return S_OK;
}

HRESULT KWndManager::LoadUIWindowTable( KD3DWndClass* pkWnd, KD3DWndClass* pkParent )
{
    HRESULT hr;

    std::string strWndName;
    std::string strType;

    DWORD   dwWidth;
    DWORD   dwHeight;
	BOOL	bVisible;
	int		bPixelValue;
    int     iDefTexIdx; // "default" texture index
    int     iHovTexIdx; // "hover"   texture index
    int     iSelTexIdx; // "select"  texture index
    int     iActTexIdx; // "active"  texture index
    int     iLocTexIdx; // "lock"    texture index

    float   tu1def, tv1def, tu2def, tv2def; // "default" texture coordinates
    float   tu1hov, tv1hov, tu2hov, tv2hov; // "hover"   texture coordinates
    float   tu1sel, tv1sel, tu2sel, tv2sel; // "select"  texture coordinates
    float   tu1act, tv1act, tu2act, tv2act; // "active"  texture coordinates
    float   tu1loc, tv1loc, tu2loc, tv2loc; // "lock"    texture coordinates

    float   fCreatePosX, fCreatePosY;
    float   fWindowPosX, fWindowPosY;
    float   fDestroyPosX, fDestroyPosY;

    std::string strZOrd;
    float   fMoveGapTime;
    std::string strMotion;
    std::string strAlpha;
    BOOL    bAutoCreate;
    BOOL    bPredraw;
    std::string strToolTip;
	std::string strHotKey;
	int iHotKey;
    DWORD nSortIndex;

    // read values
    if ( FAILED(m_LuaMgr.GetValue( "name",        strWndName )) )
	{
		char WndName[MAX_NAME];
		//sprintf(WndName, "Noname_%d",m_iID); // unsafe
		sprintf_s( WndName, MAX_NAME, "Noname_%d",m_iID);
		strWndName = WndName;
		m_iID++;
	}
    
    //  디폴트값 바꾸면 세이브할때 내용도 변경해야 합니다.	
    if ( FAILED(m_LuaMgr.GetValue( "sort_index",   nSortIndex ))  ) nSortIndex = INT_MAX;

	if ( FAILED(m_LuaMgr.GetValue( "visible",	   bVisible ))  )	 bVisible = TRUE;

    if ( FAILED(m_LuaMgr.GetValue( "type",		   strType ))  )	 strType = "";

    if ( FAILED(m_LuaMgr.GetValue( "width",       dwWidth ))  )      dwWidth = 0;
    if ( FAILED(m_LuaMgr.GetValue( "height",      dwHeight ))  )     dwHeight = 0;

    if ( FAILED(m_LuaMgr.GetValue( "def_tex_idx", iDefTexIdx )) )    iDefTexIdx = -1;
	if ( FAILED(m_LuaMgr.GetValue( "pixelvalue", bPixelValue ))  )   bPixelValue = 0;
	if ( FAILED(m_LuaMgr.GetValue( "def_tu1",     tu1def )  ))       tu1def = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "def_tv1",     tv1def )  ))       tv1def = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "def_tu2",     tu2def )  ))       tu2def = 1.0f;
    if ( FAILED(m_LuaMgr.GetValue( "def_tv2",     tv2def ) ))        tv2def = 1.0f;

    if ( FAILED(m_LuaMgr.GetValue( "hov_tex_idx", iHovTexIdx )  ))   iHovTexIdx = -1;
    if ( FAILED(m_LuaMgr.GetValue( "hov_tu1",     tu1hov )  ))       tu1hov = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "hov_tv1",     tv1hov )  ))       tv1hov = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "hov_tu2",     tu2hov )  ))       tu2hov = 1.0f;
    if ( FAILED(m_LuaMgr.GetValue( "hov_tv2",     tv2hov )  ))       tv2hov = 1.0f;

    if ( FAILED(m_LuaMgr.GetValue( "sel_tex_idx", iSelTexIdx )  ))   iSelTexIdx = -1;
    if ( FAILED(m_LuaMgr.GetValue( "sel_tu1",     tu1sel )  ))       tu1sel = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "sel_tv1",     tv1sel ) ))        tv1sel = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "sel_tu2",     tu2sel )  ))       tu2sel = 1.0f;
    if ( FAILED(m_LuaMgr.GetValue( "sel_tv2",     tv2sel )  ))       tv2sel = 1.0f;

    if ( FAILED(m_LuaMgr.GetValue( "act_tex_idx", iActTexIdx )  ))   iActTexIdx = -1;
    if ( FAILED(m_LuaMgr.GetValue( "act_tu1",     tu1act )  ))       tu1act = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "act_tv1",     tv1act ) ))        tv1act = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "act_tu2",     tu2act )  ))       tu2act = 1.0f;
    if ( FAILED(m_LuaMgr.GetValue( "act_tv2",     tv2act )  ))       tv2act = 1.0f;

    if ( FAILED(m_LuaMgr.GetValue( "loc_tex_idx", iLocTexIdx )  ))   iLocTexIdx = -1;
    if ( FAILED(m_LuaMgr.GetValue( "loc_tu1",     tu1loc )  ))       tu1loc = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "loc_tv1",     tv1loc )  ))       tv1loc = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "loc_tu2",     tu2loc )  ))       tu2loc = 1.0f;
    if ( FAILED(m_LuaMgr.GetValue( "loc_tv2",     tv2loc )  ))       tv2loc = 1.0f;

    if ( FAILED(m_LuaMgr.GetValue( "create_x",    fCreatePosX )  ))  fCreatePosX = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "create_y",    fCreatePosY )  ))  fCreatePosY = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "window_x",    fWindowPosX )  ))  fWindowPosX = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "window_y",    fWindowPosY )  ))  fWindowPosY = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "destroy_x",   fDestroyPosX )  )) fDestroyPosX = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "destroy_y",   fDestroyPosY )  )) fDestroyPosY = 0.0f;

    if ( FAILED(m_LuaMgr.GetValue( "move_gap",    fMoveGapTime )  )) fMoveGapTime = 0.0f;
    if ( FAILED(m_LuaMgr.GetValue( "motion",      strMotion ) ))     strMotion = "linear";
    if ( FAILED(m_LuaMgr.GetValue( "alpha",       strAlpha ) ))      strAlpha = "linear";
    if ( FAILED(m_LuaMgr.GetValue( "auto_create", bAutoCreate ) ))   bAutoCreate = TRUE;
    if ( FAILED(m_LuaMgr.GetValue( "predraw",     bPredraw ) ))      bPredraw = FALSE;
    if ( FAILED(m_LuaMgr.GetValue( "tool_tip",    strToolTip )  ))   strToolTip = "";

	if ( FAILED(m_LuaMgr.GetValue( "hotkey",      iHotKey ) ))      iHotKey = -1;
	if ( FAILED(m_LuaMgr.GetValue( "hotkeystyle", strHotKey ) ))    strHotKey = "";


	for(int i = 1 ; ; i++)
	{
		if( FAILED(m_LuaMgr.BeginTable(i)) ) 
			break;

		std::string strChildType;
		if ( FAILED(m_LuaMgr.GetValue( "type",		   strChildType ))  )		 strChildType = "";

		KD3DWndClass* d3dwindow = new KD3DWndClass();

		if(  FAILED(hr = LoadUIWindowTable(d3dwindow,pkWnd) ) )
			return hr;
		pkWnd->AddChildWnd(d3dwindow);
		if( FAILED( hr = m_LuaMgr.EndTable() ) )
			return hr;
	}

    if( bPixelValue == 0 )
    {
        tu1def = (tu1def *512.0f) - 0.5f;
        tv1def = (tv1def *512.0f) - 0.5f;
        tu2def = (tu2def *512.0f) - 0.5f;
        tv2def = (tv2def *512.0f) - 0.5f;

        tu1hov = (tu1hov *512.0f) - 0.5f;
        tv1hov = (tv1hov *512.0f) - 0.5f;
        tu2hov = (tu2hov *512.0f) - 0.5f;
        tv2hov = (tv2hov *512.0f) - 0.5f;	

        tu1sel = (tu1sel *512.0f) - 0.5f;
        tv1sel = (tv1sel *512.0f) - 0.5f;
        tu2sel = (tu2sel *512.0f) - 0.5f;
        tv2sel = (tv2sel *512.0f) - 0.5f;	

        tu1act = (tu1act *512.0f) - 0.5f;
        tv1act = (tv1act *512.0f) - 0.5f;
        tu2act = (tu2act *512.0f) - 0.5f;
        tv2act = (tv2act *512.0f) - 0.5f;

        tu1loc = (tu1loc *512.0f) - 0.5f;
        tv1loc = (tv1loc *512.0f) - 0.5f;
        tu2loc = (tu2loc *512.0f) - 0.5f;
        tv2loc = (tv2loc *512.0f) - 0.5f;
    }

    //KD3DWndClass kWndClass;
    pkWnd->SetVisible( bVisible );
    pkWnd->SetWndName( strWndName );
    pkWnd->SetSortIndex( nSortIndex );
    pkWnd->SetWndType( strType );
    pkWnd->SetParent( pkParent );
    pkWnd->SetWidth( dwWidth );
    pkWnd->SetHeight( dwHeight );

    pkWnd->SetCreatedPos( D3DXVECTOR2( fCreatePosX, fCreatePosY ) );
    pkWnd->SetWindowPos( D3DXVECTOR2( fWindowPosX, fWindowPosY ) );
    pkWnd->SetDestroyPos( D3DXVECTOR2( fDestroyPosX, fDestroyPosY ) );

    pkWnd->SetMoveGap( fMoveGapTime );
    pkWnd->SetPixelValue( TRUE );

    pkWnd->SetDefTextureIDx( iDefTexIdx, true );
    pkWnd->SetHovTextureIDx( iHovTexIdx, true );
    pkWnd->SetSelTextureIDx( iSelTexIdx, true );
    pkWnd->SetActTextureIDx( iActTexIdx, true );
    pkWnd->SetLocTextureIDx( iLocTexIdx, true );

    pkWnd->SetDefTexCoords( D3DXVECTOR4( tu1def, tv1def, tu2def, tv2def ), true );
    pkWnd->SetHovTexCoords( D3DXVECTOR4( tu1hov, tv1hov, tu2hov, tv2hov ), true );
    pkWnd->SetSelTexCoords( D3DXVECTOR4( tu1sel, tv1sel, tu2sel, tv2sel ), true );
    pkWnd->SetActTexCoords( D3DXVECTOR4( tu1act, tv1act, tu2act, tv2act ), true );
    pkWnd->SetLocTexCoords( D3DXVECTOR4( tu1loc, tv1loc, tu2loc, tv2loc ), true );

    pkWnd->SetToolTip( strToolTip );
    pkWnd->SetPreDraw( bPredraw );
    pkWnd->SetAutoCreate( bAutoCreate );
    //	pkWnd->strZOrder	= strZOrd;
	
    return S_OK;
}

HRESULT KWndManager::LoadUINumberTable( KD3DNumClass* pkWnd )
{
	std::string strType;
	int	iWidth,iHeight;
	int iTexID;
	float fTu1 , fTv1 , fTu2 , fTv2;
	float fOffset;

	if ( FAILED(m_LuaMgr.GetValue( "type",		   strType ))  )		 strType = "";
	if ( FAILED(m_LuaMgr.GetValue( "width",		   iWidth ) ) )		 iWidth = 0;
	if ( FAILED(m_LuaMgr.GetValue( "height",		   iHeight ))  )		 iHeight = 0;
	if ( FAILED(m_LuaMgr.GetValue( "tex_idx",		   iTexID ))  )		 strType = -1;

	if ( FAILED(m_LuaMgr.GetValue( "tu1",			   fTu1 ))  )		 fTu1 = -1;
	if ( FAILED(m_LuaMgr.GetValue( "tv1",			   fTv1 ))  )		 fTv1 = -1;
	if ( FAILED(m_LuaMgr.GetValue( "tu2",			   fTu2 ))  )		 fTu2 = -1;
	if ( FAILED(m_LuaMgr.GetValue( "tv2",			   fTv2 ))  )		 fTv2 = -1;

	if ( FAILED(m_LuaMgr.GetValue( "offset",		   fOffset ))  )		 fOffset = -1;
	
	pkWnd->strType = strType;
	pkWnd->iWidth = iWidth;
	pkWnd->iHeight = iHeight;
	pkWnd->iTexIDs = iTexID;

	pkWnd->fTu1	= fTu1;
	pkWnd->fTu2 = fTu2;
	pkWnd->fTv1 = fTv1;
	pkWnd->fTv2 = fTv2;
    
	pkWnd->offset = fOffset;

	return S_OK;

}

void KWndManager::MoveWindow( KD3DWndClass* pkWnd, KD3DWndClass* pkParent )
{
	if( pkWnd->GetParent() )
	{
		for (UINT i = 0; i < pkWnd->GetParent()->GetChildNum(); i++ )
		{
			if( pkWnd->GetParent()->GetChild(i) == pkWnd )
				pkWnd->GetParent()->RemoveChildWnd( i );
		}	
	}
	

	if( !pkParent )
	{
		return;
	}
	else
	{
		pkParent->AddChildWnd(pkWnd);
		pkWnd->SetParent( pkParent );
	}	
}

void KWndManager::InsertWindow( KD3DWndClass* pkWnd, KD3DWndClass* pkParent )
{
	/*if( pkWnd->pParent )
	{
		for (UINT i = 0; i < pkWnd->GetParent()->GetChildNum(); i++ )
		{
			if( pkWnd->GetParent()->GetChild(i) == pkWnd )
				pkWnd->GetParent()->vecChild.erase( pkWnd->pParent->vecChild.begin() + i );
		}	
	}*/


	if( !pkParent )
	{
		return;
	}
	else
	{
		pkParent->AddChildWnd(pkWnd);
		pkWnd->SetParent( pkParent );
	}
}

void KWndManager::InsertWindow( KD3DWndClass* pkWnd, KD3DWndClass* pkParent, UINT nIndex )
{
	/*if( pkWnd->pParent )
	{
	for (UINT i = 0; i < pkWnd->GetParent()->GetChildNum(); i++ )
	{
	if( pkWnd->GetParent()->GetChild(i) == pkWnd )
	pkWnd->GetParent()->vecChild.erase( pkWnd->pParent->vecChild.begin() + i );
	}	
	}*/


	if( !pkParent )
	{
		return;
	}
	else
	{
		pkParent->AddChildWnd(pkWnd, nIndex);
		pkWnd->SetParent( pkParent );
	}
}


//------------------------------------------------------------------------
//	NAME : CopyWindow()
//	DESC : 단순 포인터 복사만 하던 기능을 깊은 복사로 전환하여
//			씬 편집시 로드를 다시해도 클립보드 내용이 유지 된다.
//	DATE : 20091019
//------------------------------------------------------------------------
void KWndManager::CopyWindow( KD3DWndClass* pkWnd )
{
	if( m_pClipBoard )
	{
		//	소멸자에서 호출하게끔 했지만 작동하지 않는다.
		//	그래서 delete 이전에 항상 호출해야 메모리 누수가 없다.
		m_pClipBoard->DeleteChildren();
		
		delete m_pClipBoard;
		m_pClipBoard = NULL;
	}
	m_pClipBoard = new KD3DWndClass;
	m_pClipBoard->DeepCopy( pkWnd );
}



void KWndManager::PasteWindow( KD3DWndClass* pkParent , int iIndex )
{
    m_pSourceWnd = m_pClipBoard;
    PasteWindowContent( pkParent, iIndex );
}



void KWndManager::PasteWindowContent( KD3DWndClass* pkParent , int iIndex )
{
    if ( !m_pSourceWnd )
        return;
    KD3DWndClass* pkWnd = new KD3DWndClass(*m_pSourceWnd);
    pkWnd->SetParent( pkParent );

    if( iIndex == -1 )
        pkParent->AddChildWnd( pkWnd );
    else
        pkParent->SetChild( iIndex, pkWnd );

    for( int i = 0 ; i <  pkWnd->GetChildNum() ; i++ )
    {
        m_pSourceWnd = pkWnd->GetChild(i);
        PasteWindowContent(pkWnd , i);
    }
}

void KWndManager::DeleteWindow( KD3DWndClass* pkWnd )
{
	if ( pkWnd == m_pClipBoard ) 
		return;

	

	for( UINT i = 0; i < pkWnd->GetParent()->GetChildNum() ; i++ )
	{
		if ( pkWnd->GetParent()->GetChild(i) == pkWnd )
		{
			pkWnd->GetParent()->RemoveChildWnd( i );
			delete pkWnd;
			return;
		}
	}
	
}


std::string KWndManager::ToString(KD3DWndClass* pkWnd)
{
	KWndDataPackage Package;
    Package.SetTexFileString(m_vecStrTexFileName);

	std::string Result;
	std::string strTab;

	for ( UINT iTab = 0 ; iTab < m_uiTab ; iTab++ )
		strTab += "\t";
	Result += strTab;
	
	Result += "{\n";
	if( m_uiTab == 1 && m_bStart == false)
	{
		Result += strTab + "\tscene_name = \"main\", \n"; 
		Result += strTab + "\tscene_type = \"gc_scene\", \n"; 
		m_bStart = true;
	}
	
	++m_uiTab;

	Package.PushWndSimple(pkWnd);
	Result += Package.ToString( m_uiTab );

	for( UINT i = 0 ; i < pkWnd->GetChildNum() ; i++ )
	{
		Result += ToString(pkWnd->GetChild(i));
	}

	--m_uiTab;
	
	strTab = "";
	for ( UINT iTab = 0 ; iTab < m_uiTab ; iTab++ )
		strTab += "\t";

	Result += strTab;
	if( m_uiTab == 0 )
		Result += "}\n";
	else
		Result += "},\n";

	return Result;
	
}


HRESULT KWndManager::SaveToFile(const std::string& strFileName, bool bSort /*= false*/ )
{
	std::string strSaveData;

    if ( bSort ) 
    {
        m_pWndRoot->CalcSortIndex();

        //  이름순으로 정렬해서 저장
        m_pWndRoot->SortByName();
    }


	//strSaveData += TextureTableToString();
	//strSaveData += NumberTableToString();
	strSaveData += m_pWndRoot->GetWndName() + " = ";

	strSaveData += ToString(m_pWndRoot);

    if ( bSort )
    {
        //  SortByIndex로 다시 돌림.
        m_pWndRoot->SortByIndex();
    }


	
	m_bStart = false;
	m_uiTab = 0;
	//CStdioFile file;
	//file.Open(strFileName.c_str(),CFile::modeCreate|CFile::modeWrite);

	//file.Write( strSaveData.c_str(),strSaveData.size() );

	//file.Close();

    HANDLE hWrite = CreateFile( strFileName.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if( INVALID_HANDLE_VALUE == hWrite )
    {
        MessageBoxA(NULL, "저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인, 또는 읽기전용인지 확인해 보세요.", "알림", MB_OK);
        return IDCANCEL;
    }

    DWORD dwSize = 0;
    WriteFile(hWrite, strSaveData.c_str(), strSaveData.size(), &dwSize, 0);
    CloseHandle(hWrite);

	return IDOK;
}

HRESULT KWndManager::SaveToFile(KD3DWndClass* pkWnd, const std::string& strFileName)
{
    std::string strSaveData;
    strSaveData = "scene_part = ";

    strSaveData += ToString(pkWnd);

    m_bStart = true;
    m_uiTab = 0;
    CStdioFile file;
    file.Open( strFileName.c_str(),CFile::modeCreate|CFile::modeWrite);

    file.Write( strSaveData.c_str(),strSaveData.size() );

    file.Close();

    return IDOK;
}

HRESULT	KWndManager::Z_PosForth(KD3DWndClass* pkWnd)
{
	bool Checker = false;
	UINT i = 0;
	for ( i = 0 ; i < pkWnd->GetParent()->GetChildNum() ; i++ )
	{
		if ( pkWnd->GetParent()->GetChild(i) == pkWnd )
		{
			Checker = true;
			break;
		}
	}

	//부모의 벡터에 pkWnd가 없거나 제일 위에 있는 윈도우다.
	if ( !Checker || i == ( pkWnd->GetParent()->GetChildNum() - 1 ) )
	{
		return FALSE;
	}

    pkWnd->GetParent()->SwapChildPos( i, i+1 );

	return TRUE;
		
}
HRESULT	KWndManager::Z_PosBack(KD3DWndClass* pkWnd)
{
	bool Checker = false;
    UINT i = 0;
	for ( i = 0 ; i < pkWnd->GetParent()->GetChildNum() ; i++ )
	{
		if ( pkWnd->GetParent()->GetChild(i) == pkWnd )
		{
			Checker = true;
			break;
		}
	}

	//부모의 벡터에 pkWnd가 없거나 제일 아래에 있는 윈도우다.
	if ( !Checker || i < 1 )
	{
		return FALSE;
	}

    pkWnd->GetParent()->SwapChildPos( i, i-1 );
	return TRUE;

}
HRESULT	KWndManager::Z_PosTop(KD3DWndClass* pkWnd)
{
	for(UINT i = 0 ; i < pkWnd->GetParent()->GetChildNum() ; i++  )
		if ( Z_PosForth(pkWnd) != TRUE )
			break;
	return TRUE;
	
}
HRESULT	KWndManager::Z_PosBottom(KD3DWndClass* pkWnd)
{

	for(UINT i = 0 ; i < pkWnd->GetParent()->GetChildNum() ; i++  )
		if ( Z_PosBack(pkWnd) != TRUE )
			break;
	return TRUE;
}

HRESULT	KWndManager::Z_PosMove(KD3DWndClass* pkWnd, int nCount )
{
    if ( nCount > 0 )
    {
        for(UINT i = 0 ; i < nCount ; i++  )
        {
            if ( Z_PosForth(pkWnd) != TRUE )
                break;
        }
    }
    else
    {
        nCount *= -1;
        for(UINT i = 0 ; i < nCount ; i++  )
        {
            if ( Z_PosBack(pkWnd) != TRUE )
                break;
        }
    }

    return IDOK;
}

void KWndManager::SwapWnd( KD3DWndClass** pkWnd1 , KD3DWndClass** pkWnd2 )
{
	KD3DWndClass* temp = *pkWnd1;
	*pkWnd1 = *pkWnd2;
	*pkWnd2 = temp;
}

std::string KWndManager::TextureTableToString()
{
	std::string strResult = "";

	strResult += "texture = {\n";
	
	for ( UINT i = 0; i < m_vecStrTexFileName.size() ; i ++ )
	{
		strResult += "\t";
		strResult += "\"" + m_vecStrTexFileName[i] + "\",";
		strResult += "\n";
	}
	strResult += "}\n";

	return strResult;
}
std::string KWndManager::NumberTableToString()
{
	std::string strResult = "";

	strResult += "number = {\n";

	for ( int i = 0 ; i < m_vecNum.size() ; i++ )
	{
		KWndDataPackage package;
        package.SetTexFileString(m_vecStrTexFileName);
		package.PushData("type",	&m_vecNum[i]->strType);
		package.PushData("width",	&m_vecNum[i]->iWidth);
		package.PushData("height",	&m_vecNum[i]->iHeight);
		package.PushData("tex_idx",	&m_vecNum[i]->iTexIDs);

		package.PushData("tu1",		&m_vecNum[i]->fTu1);
		package.PushData("tv1",		&m_vecNum[i]->fTv1);
		package.PushData("tu2",		&m_vecNum[i]->fTu2);
		package.PushData("tv2",		&m_vecNum[i]->fTv2);

		package.PushData("offset",	&m_vecNum[i]->offset);
		strResult += "\t{\n";
		strResult += package.ToString(2);
		strResult += "\t},\n";

	}
	
	strResult += "}\n";
	return strResult;
}

HRESULT KWndManager::DropTexture(UINT index)
{
	m_vecStrTexFileName.erase( m_vecStrTexFileName.begin() + index );

    std::map<int,LPDIRECT3DTEXTURE9>::iterator mIt = m_mapTexture.find(index);
    if(mIt == m_mapTexture.end()) return S_OK;
    if(mIt->second == NULL)
    {
        m_mapTexture.erase(mIt);
        return S_OK;
    }


    mIt->second->Release();
    m_mapTexture.erase(mIt);
	ArrangeDeletedTextureIndex(m_pWndRoot,index);
	return S_OK;	
}

HRESULT KWndManager::ArrangeDeletedTextureIndex(KD3DWndClass* pkWnd , UINT index)
{
	
    if(pkWnd->GetActTextureIDx() == index)
        pkWnd->SetActTextureIDx( -1 );
	if(pkWnd->GetDefTextureIDx() == index)
		pkWnd->SetDefTextureIDx( -1 );
    if(pkWnd->GetHovTextureIDx() == index)
        pkWnd->SetHovTextureIDx( -1 );
    if(pkWnd->GetLocTextureIDx() == index)
        pkWnd->SetLocTextureIDx( -1 );
    if(pkWnd->GetSelTextureIDx() == index)
        pkWnd->SetSelTextureIDx( -1 );


    if(pkWnd->GetActTextureIDx() > index)
        pkWnd->SetActTextureIDx( pkWnd->GetActTextureIDx() - 1 );
	if(pkWnd->GetDefTextureIDx() > index)
		pkWnd->SetDefTextureIDx( pkWnd->GetDefTextureIDx() - 1 );
    if(pkWnd->GetHovTextureIDx() > index)
        pkWnd->SetHovTextureIDx( pkWnd->GetHovTextureIDx() - 1 );
    if(pkWnd->GetLocTextureIDx() > index)
        pkWnd->SetLocTextureIDx( pkWnd->GetLocTextureIDx() - 1 );
    if(pkWnd->GetSelTextureIDx() > index)
        pkWnd->SetSelTextureIDx( pkWnd->GetSelTextureIDx() - 1 );


	for(UINT i = 0; i < pkWnd->GetChildNum(); i++)
	{
		ArrangeDeletedTextureIndex(pkWnd->GetChild(i),index);
	}
	
	return S_OK;
}

//-------------------------------------------------------------
// 2011. 09. 01 수정   (이재준)
// 불러오기 창에서 파일 캡션을 보여주기 위해 둘다 읽어야 한다. 
//-------------------------------------------------------------
HRESULT KWndManager::InsertCaptionData( tagUIScriptFile* pFileInfo )
{
    for ( int i = 0; i < m_vecCaptionData.size(); ++i ) {
        if ( strstr( pFileInfo->strFileName.c_str(), m_vecCaptionData[i].strFileName.c_str() ) ) {
            pFileInfo->strFileCaption = m_vecCaptionData[i].strFileCaption.c_str();
            return S_OK;
        }
    }

    pFileInfo->strFileCaption = "";
    return S_OK;
}


//-------------------------------------------------------------------------
// 캡션 읽어오기
//-------------------------------------------------------------------------
BOOL KWndManager::LoadCaption()
{
    if(FAILED( m_LuaMgr.DoFile( "desc.lua" ) ) )
        return FALSE;

    HRESULT hr;

    /////////////////////////////////////////////
    m_vecCaptionData.clear();
    if ( SUCCEEDED( hr = m_LuaMgr.BeginTable( "caption" ) ) ) {
        tagUIScriptFile kUiScriptFileInfo;

        for ( UINT i = 1; ; ++i ) {
            if ( SUCCEEDED(m_LuaMgr.BeginTable(i,false)) ) {
                if ( SUCCEEDED( LoadUIScriptTable( &kUiScriptFileInfo ) ) ) {
                    if (!kUiScriptFileInfo.strFileName.empty());
                    m_vecCaptionData.push_back( kUiScriptFileInfo );
                }
                m_LuaMgr.EndTable(false);
            }
            else
                break;// no more data
        }

        if ( FAILED( hr = m_LuaMgr.EndTable() ) ) return false; // invalid file format
    }

    return TRUE;
}


HRESULT KWndManager::LoadUIScriptTable( tagUIScriptFile* pFileInfo )
{
    HRESULT hr;

    if ( FAILED(hr = m_LuaMgr.GetValue( "filename", pFileInfo->strFileName ))  )   pFileInfo->strFileName = "";
    if ( FAILED(hr = m_LuaMgr.GetValue( "caption", pFileInfo->strFileCaption ))  ) pFileInfo->strFileCaption = "";

    return hr;
}